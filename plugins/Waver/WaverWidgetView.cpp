/*
 * WaverWidgetView.cpp - controls the UI for Waver
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

/*
You have to install qt5-declarative qt5-quickcontrols2
 */

#include "WaverWidgetView.h"

#include <QDropEvent>
#include <QPainter>
#include <QPushButton>
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QQuickWidget>
#include <QQmlEngine>
#include <QQmlContext>
#include <QResource>

#include "Clipboard.h"
#include "DataFile.h"
#include "InstrumentView.h"
#include "PixmapButton.h"
#include "SampleLoader.h"
#include "Waver.h"
#include "StringPairDrag.h"
#include "Track.h"
#include "embed.h"

namespace lmms {

    namespace gui {

        WaverWidgetView::WaverWidgetView(Waver* instrument, QWidget* parent)
            : InstrumentView(instrument, parent)
            , m_waverParent(instrument)
        {
            // window settings
            setAcceptDrops(true);
            setAutoFillBackground(true);

            setMaximumSize(QSize(10000, 10000));
            setMinimumSize(QSize(750, 600));

            QVBoxLayout *layout = new QVBoxLayout(this);
            setLayout(layout);

            // Set up the QQuickWidget
            QQuickWidget *quickWidget = new QQuickWidget(this);

            // Expose the model to QML
            quickWidget->rootContext()->setContextProperty("waverModel", instrument);

            quickWidget->setSource(QUrl(QStringLiteral("qrc:/artwork/waver/WaverView.qml")));
            quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
            layout->addWidget(quickWidget);

            // Ensure QML root object is resized with the widget
            quickWidget->setMinimumSize(QSize(0, 0)); 

            setLayout(layout); 

            update();
        }

        void WaverWidgetView::openFiles()
        {
            const auto audioFile = SampleLoader::openAudioFile();
            if (audioFile.isEmpty()) { return; }
            m_waverParent->updateFile(audioFile);
        }

        // all the drag stuff is copied from AudioFileProcessor
        void WaverWidgetView::dragEnterEvent(QDragEnterEvent* dee)
        {
            // For mimeType() and MimeType enum class
            using namespace Clipboard;

            if (dee->mimeData()->hasFormat(mimeType(MimeType::StringPair)))
            {
                QString txt = dee->mimeData()->data(mimeType(MimeType::StringPair));
                if (txt.section(':', 0, 0) == QString("clip_%1").arg(static_cast<int>(Track::Type::Sample)))
                {
                    dee->acceptProposedAction();
                }
                else if (txt.section(':', 0, 0) == "samplefile") { dee->acceptProposedAction(); }
                else { dee->ignore(); }
            }
            else { dee->ignore(); }
        }

        void WaverWidgetView::dropEvent(QDropEvent* de)
        {
            QString type = StringPairDrag::decodeKey(de);
            QString value = StringPairDrag::decodeValue(de);
            if (type == "samplefile")
            {
                // set m_wf wave file
                m_waverParent->updateFile(value);
                return;
            }
            else if (type == QString("clip_%1").arg(static_cast<int>(Track::Type::Sample)))
            {
                DataFile dataFile(value.toUtf8());
                m_waverParent->updateFile(dataFile.content().firstChild().toElement().attribute("src"));
                de->accept();
                return;
            }

            de->ignore();
        }

    } // namespace gui

} // namespace lmms
