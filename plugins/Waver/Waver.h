/*
 * Waver.h - declaration of class Waver
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

#ifndef WAVER_H
#define WAVER_H

#include <QList>

#include "AutomatableModel.h"
#include "ComboBoxModel.h"
#include "Instrument.h"
#include "Note.h"
#include "Sample.h"
#include "WaverWidgetView.h"
#include "WaverSampleMap.h"

namespace lmms {

class Waver : public Instrument
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QList<WaverSampleMap*> sampleMap READ sampleMap NOTIFY sampleMapChanged)
    Q_PROPERTY(WaverSampleMap* selectedSampleMap READ selectedSampleMap NOTIFY selectedSampleMapChanged)

public slots:
    void updateFile(QString file);

public:
    Waver(InstrumentTrack* instrumentTrack);

    void loadFile(const QString& file) override;

    void createSampleMap(QString file);
    Q_INVOKABLE void selectSampleMap(WaverSampleMap* sampleMap);

    void playNote(NotePlayHandle* handle, SampleFrame* workingBuffer) override;
	void deleteNotePluginData(NotePlayHandle* handle) override;

    void saveSettings(QDomDocument& document, QDomElement& element) override;
    void loadSettings(const QDomElement& element) override;

    QString getSampleName() { return m_originalSample.sampleFile(); }

    QString nodeName() const override;
    gui::PluginView* instantiateView(QWidget* parent) override;

    QString name() const { return m_name; }
    QList<WaverSampleMap*> sampleMap() const { return m_sampleMap; };
    WaverSampleMap* selectedSampleMap() const { return m_selectedSampleMap; };
    

signals:
    void sampleMapChanged();
	void selectedSampleMapChanged();

private:
    FloatModel m_noteThreshold;
    FloatModel m_fadeOutFrames;
    IntModel m_originalBPM;
    ComboBoxModel m_sliceSnap;
    BoolModel m_enableSync;

    QString m_name = "Waver";

    Sample m_originalSample;

    std::vector<float> m_slicePoints;

    InstrumentTrack* m_parentTrack;

    friend class gui::WaverWidgetView;

    QList<WaverSampleMap*> m_sampleMap;
    WaverSampleMap* m_selectedSampleMap;
};

} // namespace lmms

#endif // LMMS_WAVER_H
