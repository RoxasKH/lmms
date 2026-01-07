/*
 * WaverSampleMap.cpp - instrument for using audio files
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

#include "WaverSampleMap.h"

#include "InstrumentTrack.h"
#include "PathUtil.h"
#include "SampleLoader.h"
#include "Song.h"

#include "LmmsTypes.h"
#include "plugin_export.h"

#include <QDomElement>


namespace lmms
{

WaverSampleMap::WaverSampleMap(float _baseFreq) :
	m_ampModel( 100, 0, 500, 1, this, tr( "Amplify" ) ),
	m_startPointModel( 0, 0, 1, 0.0000001f, this, tr( "Start of sample" ) ),
	m_endPointModel( 1, 0, 1, 0.0000001f, this, tr( "End of sample" ) ),
	m_loopPointModel( 0, 0, 1, 0.0000001f, this, tr( "Loopback point" ) ),
	m_reverseModel( false, this, tr( "Reverse sample" ) ),
	m_loopModel( 0, 0, 2, this, tr( "Loop mode" ) ),
	m_stutterModel( false, this, tr( "Stutter" ) ),
	m_interpolationModel( this, tr( "Interpolation mode" ) ),
	m_nextPlayStartPoint( 0 ),
	m_nextPlayBackwards( false )
{

	m_baseFreq = baseFreq;

//interpolation modes
	m_interpolationModel.addItem(tr("None"));
	m_interpolationModel.addItem(tr("Linear"));
	m_interpolationModel.addItem(tr("Sinc"));
	m_interpolationModel.setValue(1);

	pointChanged();
}




void WaverSampleMap::playNote(NotePlayHandle * _n, SampleFrame* _working_buffer)
{
	const fpp_t frames = _n->framesLeftForCurrentPeriod();
	const f_cnt_t offset = _n->noteOffset();

	// Magic key - a frequency < 20 (say, the bottom piano note if using
	// a A4 base tuning) restarts the start point. The note is not actually
	// played.
	if( m_stutterModel.value() == true && _n->frequency() < 20.0 )
	{
		m_nextPlayStartPoint = m_sample.startFrame();
		m_nextPlayBackwards = false;
		return;
	}

	if( !_n->m_pluginData )
	{
		if (m_stutterModel.value() == true && m_nextPlayStartPoint >= static_cast<std::size_t>(m_sample.endFrame()))
		{
			// Restart playing the note if in stutter mode, not in loop mode,
			// and we're at the end of the sample.
			m_nextPlayStartPoint = m_sample.startFrame();
			m_nextPlayBackwards = false;
		}
		// set interpolation mode for libsamplerate
		auto interpolationMode = AudioResampler::Mode::Linear;
		switch( m_interpolationModel.value() )
		{
			case 0:
				interpolationMode = AudioResampler::Mode::ZOH;
				break;
			case 1:
				interpolationMode = AudioResampler::Mode::Linear;
				break;
			case 2:
				interpolationMode = AudioResampler::Mode::SincMedium;
				break;
		}

		_n->m_pluginData = new Sample::PlaybackState(interpolationMode);
		static_cast<Sample::PlaybackState*>(_n->m_pluginData)->setFrameIndex(m_nextPlayStartPoint);
		static_cast<Sample::PlaybackState*>(_n->m_pluginData)->setBackwards(m_nextPlayBackwards);

// debug code
/*		qDebug( "frames %d", m_sample->frames() );
		qDebug( "startframe %d", m_sample->startFrame() );
		qDebug( "nextPlayStartPoint %d", m_nextPlayStartPoint );*/
	}

	if( ! _n->isFinished() )
	{
		if (m_sample.play(_working_buffer + offset,
						static_cast<Sample::PlaybackState*>(_n->m_pluginData),
						frames, static_cast<Sample::Loop>(m_loopModel.value()),
						DefaultBaseFreq / _n->frequency()))
		{
			applyRelease( _working_buffer, _n );
		}
		else
		{
			zeroSampleFrames(_working_buffer, frames + offset);
		}
	}
	if( m_stutterModel.value() == true )
	{
		m_nextPlayStartPoint = static_cast<Sample::PlaybackState*>(_n->m_pluginData)->frameIndex();
		m_nextPlayBackwards = static_cast<Sample::PlaybackState*>(_n->m_pluginData)->backwards();
	}
}




void WaverSampleMap::deleteNotePluginData( NotePlayHandle * _n )
{
	delete static_cast<Sample::PlaybackState*>(_n->m_pluginData);
}

void WaverSampleMap::loadFile( const QString & _file )
{
	setAudioFile( _file );
}

auto WaverSampleMap::beatLen(NotePlayHandle* note) const -> f_cnt_t
{
	// If we can play indefinitely, use the default beat note duration
	if (static_cast<Sample::Loop>(m_loopModel.value()) != Sample::Loop::Off) { return 0; }

	// Otherwise, use the remaining sample duration
	const auto baseFreq = instrumentTrack()->baseFreq();
	const auto freqFactor = baseFreq / note->frequency()
		* Engine::audioEngine()->outputSampleRate()
		/ Engine::audioEngine()->baseSampleRate();

	const auto startFrame = m_nextPlayStartPoint >= static_cast<std::size_t>(m_sample.endFrame())
		? m_sample.startFrame()
		: m_nextPlayStartPoint;
	const auto duration = m_sample.endFrame() - startFrame;

	return static_cast<f_cnt_t>(std::floor(duration * freqFactor));
}

void WaverSampleMap::setAudioFile(const QString& _audio_file, bool _rename)
{
	m_sample = Sample(gui::SampleLoader::createBufferFromFile(_audio_file));
}

void WaverSampleMap::pointChanged()
{
	const auto f_start = static_cast<f_cnt_t>(m_startPointModel.value() * m_sample.sampleSize());
	const auto f_end = static_cast<f_cnt_t>(m_endPointModel.value() * m_sample.sampleSize());
	const auto f_loop = static_cast<f_cnt_t>(m_loopPointModel.value() * m_sample.sampleSize());

	m_nextPlayStartPoint = f_start;
	m_nextPlayBackwards = false;

	m_sample.setAllPointFrames(f_start, f_end, f_loop, f_end);
}

} // namespace lmms
