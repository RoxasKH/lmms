/*
 * Waver.cpp - multi-sampler plugin
 *
 * Copyright (c) 2025 RoxasKH <asketch36@gmail.com>
 *
 * This file is part of LMMS - https://lmms.io
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program (see COPYING); if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA.
 *
 */

#include "Waver.h"

#include <QDomElement>
#include <cmath>
#include <fftw3.h>

#include "Engine.h"
#include "InstrumentTrack.h"
#include "PathUtil.h"
#include "SampleLoader.h"
#include "WaverWidgetView.h"
#include "Song.h"
#include "embed.h"
#include "interpolation.h"
#include "plugin_export.h"

namespace lmms {

    extern "C" {
        Plugin::Descriptor PLUGIN_EXPORT waver_plugin_descriptor = {
            LMMS_STRINGIFY(PLUGIN_NAME),
            "Waver",
            QT_TRANSLATE_NOOP("PluginBrowser", "MultiSampler"),
            "RoxasKH <asketch36@gmail.com>",
            0x0100,
            Plugin::Type::Instrument,
            new PluginPixmapLoader("logo"),
            nullptr,
            nullptr,
        };
    } // end extern

    Waver::Waver(InstrumentTrack* instrumentTrack)
        : Instrument(instrumentTrack, &waver_plugin_descriptor)
        , m_noteThreshold(0.6f, 0.0f, 2.0f, 0.01f, this, tr("Note threshold"))
        , m_fadeOutFrames(10.0f, 0.0f, 100.0f, 0.1f, this, tr("FadeOut"))
        , m_originalBPM(1, 1, 999, this, tr("Original bpm"))
        , m_sliceSnap(this, tr("Slice snap"))
        , m_enableSync(false, this, tr("BPM sync"))
        , m_originalSample()
        , m_parentTrack(instrumentTrack)
    {
        m_sliceSnap.addItem("Off");
        m_sliceSnap.addItem("1/1");
        m_sliceSnap.addItem("1/2");
        m_sliceSnap.addItem("1/4");
        m_sliceSnap.addItem("1/8");
        m_sliceSnap.addItem("1/16");
        m_sliceSnap.addItem("1/32");
        m_sliceSnap.setValue(0);
    }

    void Waver::updateFile(QString file)
    {
        if (auto buffer = gui::SampleLoader::createBufferFromFile(file)) { m_originalSample = Sample(std::move(buffer)); }

        emit dataChanged();
    }

    void Waver::loadFile(const QString& file)
    {
        updateFile(file);
    }

    void Waver::saveSettings(QDomDocument& document, QDomElement& element)
    {
        element.setAttribute("version", "1");
        element.setAttribute("src", m_originalSample.sampleFile());
        if (m_originalSample.sampleFile().isEmpty())
        {
            element.setAttribute("sampledata", m_originalSample.toBase64());
        }

        element.setAttribute("totalSlices", static_cast<int>(m_slicePoints.size()));
        for (auto i = std::size_t{0}; i < m_slicePoints.size(); i++)
        {
            element.setAttribute(tr("slice_%1").arg(i), m_slicePoints[i]);
        }

        m_fadeOutFrames.saveSettings(document, element, "fadeOut");
        m_noteThreshold.saveSettings(document, element, "threshold");
        m_originalBPM.saveSettings(document, element, "origBPM");
        m_enableSync.saveSettings(document, element, "syncEnable");
    }

    void Waver::loadSettings(const QDomElement& element)
    {
        if (auto srcFile = element.attribute("src"); !srcFile.isEmpty())
        {
            if (QFileInfo(PathUtil::toAbsolute(srcFile)).exists())
            {
                auto buffer = gui::SampleLoader::createBufferFromFile(srcFile);
                m_originalSample = Sample(std::move(buffer));
            }
            else
            {
                QString message = tr("Sample not found: %1").arg(srcFile);
                Engine::getSong()->collectError(message);
            }
        }
        else if (auto sampleData = element.attribute("sampledata"); !sampleData.isEmpty())
        {
            auto buffer = gui::SampleLoader::createBufferFromBase64(sampleData);
            m_originalSample = Sample(std::move(buffer));
        }

        if (!element.attribute("totalSlices").isEmpty())
        {
            int totalSlices = element.attribute("totalSlices").toInt();
            m_slicePoints = {};
            for (int i = 0; i < totalSlices; i++)
            {
                m_slicePoints.push_back(element.attribute(tr("slice_%1").arg(i)).toFloat());
            }
        }

        m_fadeOutFrames.loadSettings(element, "fadeOut");
        m_noteThreshold.loadSettings(element, "threshold");
        m_originalBPM.loadSettings(element, "origBPM");
        m_enableSync.loadSettings(element, "syncEnable");

        emit dataChanged();
    }

    QString Waver::nodeName() const
    {
        return waver_plugin_descriptor.name;
    }

    gui::PluginView* Waver::instantiateView(QWidget* parent)
    {
        return new gui::WaverWidgetView(this, parent);
    }

    extern "C" {
        PLUGIN_EXPORT Plugin* lmms_plugin_main(Model* m, void*)
        {
            return new Waver(static_cast<InstrumentTrack*>(m));
        }
    } // extern

} // namespace lmms