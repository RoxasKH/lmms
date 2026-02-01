/*
 * WaverSampleMap.h - declaration of class WaverSampleMap
 *                          (instrument-plugin for using audio-files)
 *
 * Copyright (c) 2004-2014 Tobias Doerffel <tobydox/at/users.sourceforge.net>
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

#ifndef LMMS_WAVER_SAMPLE_MAP_H
#define LMMS_WAVER_SAMPLE_MAP_H


#include "AutomatableModel.h"
#include "ComboBoxModel.h"

#include "Instrument.h"
#include "Note.h"
#include "Sample.h"
#include "LmmsTypes.h"
#include "NotePlayHandle.h"

namespace lmms
{

class WaverSampleMap: public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString name READ name CONSTANT)

public:
	WaverSampleMap(Instrument* parent);

	void playNote(NotePlayHandle * _n, SampleFrame* _working_buffer);
	void deleteNotePluginData(NotePlayHandle * _n);

	void setAudioFile(const QString& _audio_file, bool _rename = true);

	auto beatLen(NotePlayHandle* note, float _baseFreq) const -> f_cnt_t;

	float desiredReleaseTimeMs() const
	{
		return 3.f;
	}

	gui::PluginView* instantiateView(QWidget * _parent);

	Sample const & sample() const { return m_sample; }

	FloatModel & ampModel() { return m_ampModel; }
	FloatModel & startPointModel() { return m_startPointModel; }
	FloatModel & endPointModel() { return m_endPointModel; }
	FloatModel & loopPointModel() { return m_loopPointModel; }
	BoolModel & reverseModel() { return m_reverseModel; }
	IntModel & loopModel() { return m_loopModel; }
	BoolModel & stutterModel() { return m_stutterModel; }
	ComboBoxModel & interpolationModel() { return m_interpolationModel; }
	QString & name() { return m_name; }

signals:
	void sampleChanged();

private:
	Sample m_sample;

	QObject* m_parent;
	FloatModel m_ampModel;
	FloatModel m_startPointModel;
	FloatModel m_endPointModel;
	FloatModel m_loopPointModel;
	BoolModel m_reverseModel;
	IntModel m_loopModel;
	BoolModel m_stutterModel;
	ComboBoxModel m_interpolationModel;
	QString m_name;

	f_cnt_t m_nextPlayStartPoint;
	bool m_nextPlayBackwards;

	void pointChanged();
} ;

} // namespace lmms

#endif // LMMS_WAVER_SAMPLE_MAP_H
