/*
  ==============================================================================

    BaseMidiSender.cpp
    Created: 18 May 2017 11:13:19am
    Author:  ayup1

  ==============================================================================
*/

#include "BaseMidiSender.h"
#include <vector>
#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioManager.h"

using namespace Amusing;
using namespace std;

TimeLine::TimeLine()
{
	midiTimesSize = 0;
	midiOfftimesSize = 0;

	
	channel = 1;
	duration = 2 * 10000;

	speed = 1.0f;
	continuous = false;
	

	lastNote = 0;
	t0 = 0;
	position = 0;

	offset = 0;

	chordSize = 0;
}

TimeLine::~TimeLine()
{
	/*if (!continuous)
	{
		MidiMessage midiMsgOff = MidiMessage::noteOff(channel, lastNote);
		audioManager->sendMidiMessage(midiMsgOff);
	}
	else
	{
		for (int i = 0; i < midiTimesSize; i++)
		{
			MidiMessage midiMsg = MidiMessage::noteOff(channel, notes[i], (uint8)100);
			audioManager->sendMidiMessage(midiMsg);
			lastNote = notes[i];
		}
	}*/

	//audioManager->sendMidiMessage(midiMsgOff);
}

void TimeLine::setAudioManager(AudioManager* m_audioManager)
{
	audioManager = m_audioManager;
}

void TimeLine::setPeriod(int m_period)
{
	if (m_period != period)
	{
		
		for (int i = 0; i < midiTimesSize; i++)
		{
			midiTimes[i] = round((double)midiTimes[i] * (double)m_period / (double)period);
			midiOffTimes[i] = round((double)midiOffTimes[i] * (double)m_period / (double)period);
		}
		period = m_period;
		currentPeriod = period / speed;
	}
	testMidi();
}

void TimeLine::setMidiTime(int idx, int newTime, int m_noteNumber,float m_velocity)
{
	newTime =  (int)round((double)newTime * (double)currentPeriod / (double)period);
	while (newTime > currentPeriod)
		newTime -= currentPeriod;
	if (idx < maxSize)
	{
		//DBG("<");
		if (idx < midiTimesSize)
		{
			midiTimes[idx] = newTime;
			if (newTime + duration > currentPeriod) // verif si on depasse pas le temps de la periode !
				midiOffTimes[idx] = newTime + duration - currentPeriod;
			else
				midiOffTimes[idx] = newTime + duration;
			notes[idx] = m_noteNumber;
			velocity[idx] = (int)m_velocity;
		}
		else
		{
			//DBG(">");
			for (int i = 0; i < idx - midiTimesSize - 1; ++i)
			{
				midiTimes[midiTimesSize + i] = 0;
				midiOffTimes[midiTimesSize + i] = 0;
				notes[midiTimesSize + i] = 0;
				velocity[midiTimesSize + i] = 0;
				++midiTimesSize;
				++midiOfftimesSize;
			}
			midiTimes[idx] = newTime;
			if (newTime + duration > currentPeriod) // verif si on depasse pas le temps de la periode !
				midiOffTimes[idx] = (newTime + duration - currentPeriod);
			else
				midiOffTimes[idx] = (newTime + duration);
			notes[idx] = m_noteNumber;
			velocity[idx] = (int)m_velocity;
			++midiTimesSize;
			++midiOfftimesSize;
		}
		//DBG("create note : " + (String)notes[idx]);
		//DBG("BMS : number of corners is now : " + (String)midiTimesSize);
	}
	testMidi();
}

void TimeLine::setMidiChannel(int m_chan)
{
	channel = m_chan;
}

void TimeLine::setSpeed(float newSpeed)
{
	if (continuous == true)
		continuous = false;
	if (newSpeed != speed)
	{
		int newPeriod = (int)round((float)currentPeriod / (newSpeed / speed));
		for (int i = 0; i < midiTimesSize; i++)
		{
			midiTimes[i] = round((double)midiTimes[i] * (double)newPeriod / (double)currentPeriod);
			midiOffTimes[i] = round((double)midiOffTimes[i] * (double)newPeriod / (double)currentPeriod);
		}
		currentPeriod = newPeriod;
		speed = newSpeed;
		
	}
	testMidi();
}

void TimeLine::setId(int m_Id)
{
	Id = m_Id;
}

void TimeLine::setAllVelocities(float m_velocity)
{
	for (int i = 0; i < midiTimesSize; ++i)
	{
		velocity[i] = m_velocity;
	}
}

int TimeLine::getId()
{
	return Id;
}

bool TimeLine::isNoteOnTime(int m_position, int i, bool &m_end, int &m_channel, int &m_note, uint8 &m_velocity)
{

	if (i < midiTimesSize)
	{
		m_end = false; // on a pas encore atteint la fin de la liste de notes (au cas o� il y en a plusieurs � jouer au m�me moment)
		if (m_position == midiTimes[i])
		{
			if (velocity[i] != 0)
			{
				m_channel = channel;
				m_note = notes[i];
				m_velocity = (uint8)velocity[i];
				return true;
			}
			else // si velocit� == 0, pas besoin d'envoyer de note vu qu'on l'entend pas...
				return false;


		}
		else
			return false;

	}
	else
	{
		m_end = true;
		return false;
	}
	
}

bool TimeLine::isNoteOffTime(int m_position, int i, bool &m_end, int &m_channel, int &m_note)
{
	if (i < midiOfftimesSize)
	{
		m_end = false;
		if (m_position == midiOffTimes[i])
		{

			if (velocity[i] != 0)
			{
				m_channel = channel;
				m_note = notes[i];
				return true;
			}
			else // si velocit� == 0, pas besoin d'envoyer de note vu qu'on l'entend pas...
				return false;
		}
		else
			return false;

	}
	else
	{
		m_end = true;
		return false;
	}
}

bool TimeLine::isChordOnTime(int m_position, int &m_channel, int *m_chordToPlay, uint8 &m_velocity)
{
	int num = 0;
	for (int i = 0; i < chordSize; ++i)
	{
		if (chordTimesOn[i] == m_position)
		{
			m_chordToPlay[num] = chordNotesOn[i];
			num++;
		}
	}

	if (num != 0)
	{
		m_channel = channel;
		m_velocity = velocity[0];
		return true;
	}
	else
		return false;
}

bool TimeLine::isChordOffTime(int m_position, int &m_channel, int m_chordToPlay[])
{
	int num = 0;
	for (int i = 0; i < chordSize; ++i)
	{
		if (chordTimesOff[i] == m_position)
		{
			m_chordToPlay[num] = chordNotesOff[i];
			num++;
		}
	}

	if (num != 0)
	{
		m_channel = channel;
		return true;
	}
	else
		return false;
}

void TimeLine::process(int time)
{
	//int b = midiTimesSize;
	//DBG("midiTimes.size() = " + (String)midiTimesSize);
	//DBG("midiOffTimes.size() = " + (String)midiOfftimesSize);
	//DBG("time = " + (String)time);
	//DBG("midiTimesSize : " + (String)midiTimesSize);
	//time -= offset;
	if (!continuous)
	{
		time += offset;
		int oldTime = time;
		time += t0;
		//time += offset;
		while (time >= currentPeriod)
			time -= currentPeriod;
		if (oldTime == period - 1)
			t0 = time + 1;
		//time += offset;
		position = time;
		/*if (time == 0)
			t0 = 1;*/

		
		
		int m_channel, m_note;
		uint8 m_velocity;

		bool m_end = false;
		int i = 0;
		while (m_end == false)
		{
			if (isNoteOnTime(time, i, m_end, m_channel, m_note, m_velocity))
			{
				MidiMessage midiMsg = MidiMessage::noteOn(m_channel, m_note, m_velocity);
				audioManager->sendMidiMessage(midiMsg);
			}
			i++;
		}

		m_end = false;
		i = 0;
		while (m_end == false)
		{
			if (isNoteOffTime(time, i, m_end, m_channel, m_note))
			{
				MidiMessage midiMsgOff = MidiMessage::noteOff(m_channel, m_note);
				audioManager->sendMidiMessage(midiMsgOff);
			}
			i++;
		}

		
	}
}

double TimeLine::getRelativePosition()
{
	return ( (double)position)/(double)currentPeriod;
}

void TimeLine::playNoteContinuously()
{
	continuous = true;
	for (int i = 0; i < midiTimesSize; i++)
	{
			MidiMessage midiMsg = MidiMessage::noteOn(channel, notes[i], (uint8)100);
			audioManager->sendMidiMessage(midiMsg);
			lastNote = notes[i];			
	}
}

void TimeLine::alignWith(TimeLine *ref, double phase)
{
	// to align this timeLine with the reference timeLine, first we need to have the same period, then we'll apply the phase
	
	// to have the same period, we can use setPeriod or setSpeed
	// since changing the speed would need to resend the information of the new speed to the presenter, we'll first try by changing the period
	DBG((String)(ref->getPeriod()) + " " + (String)period);
	DBG((String)(ref->getSpeed()) + " " + (String)speed);
	if (period != ref->getPeriod())
		setPeriod(ref->getPeriod());
	if (speed != ref->getSpeed())
		setSpeed(ref->getSpeed());

	phase = 1 - phase;

	// now that we have the same period, we apply the phase
	int newOffset =  (int)round(phase * (double)period);
	if (newOffset < 0)
		while (newOffset < 0)
			newOffset += currentPeriod;
	else if (newOffset > period)
		while (newOffset > period)
			newOffset -= currentPeriod;

	if (newOffset != offset)
	{
		//applyOffSet(newOffset - offset);
		offset = newOffset;
	}
	testMidi();
}

void TimeLine::addChord(TimeLine * otherTimeLine, int chordTime)
{
	int currentNote = 0;

	// recherche si on jouait d�j� une note � l'instant chordTime
	for (int i = 0; i < midiTimesSize; i++)
	{
		if (midiTimes[i] == chordTime)
		{
			currentNote = notes[i];
			break;
		}
	}
	
	if (currentNote != 0)// si une note �tait jou�e � cette instant elle devient une des notes de l'accord
	{
		int otherChordNote;
		if (otherTimeLine->isNoteAvailable(ChordType::MajorThird, currentNote, otherChordNote))
			createChord(ChordType::MajorThird, chordTime, currentNote, otherChordNote);
		else if(otherTimeLine->isNoteAvailable(ChordType::MinorThird, currentNote, otherChordNote))
			createChord(ChordType::MinorThird, chordTime, currentNote, otherChordNote);
		else if (otherTimeLine->isNoteAvailable(ChordType::MinorThird, currentNote, otherChordNote))
			createChord(ChordType::AugmentedQuart, chordTime, currentNote, otherChordNote);

	} 
	else // sinon on s�lectionnera une des autres notes comme base
	{

	}
}

bool TimeLine::isNoteAvailable(ChordType m_chordType, int baseNote1, int &otherChordNote)
{
	int difference = 13; // impossible
	int baseNote2 = 0;
	switch (m_chordType)
	{
	case MajorThird:
		for (int i = 0; i < midiTimesSize; i++)
		{
			difference = abs(notes[i] - baseNote1);
			if (difference > 11)
			{
				// baseNote2 = octaveNotes1 + position note dans son octave
				baseNote2 = (baseNote1 / 12) + notes[i] % 12;
				difference = abs(notes[i] - baseNote1);
			}
			else
				baseNote2 = notes[i];

			if (difference != 1 || difference != 2 || difference != 6 || difference != 10 || difference != 11)
			{
				otherChordNote = baseNote2;
				return true;
			}
			else
				return false;
		}
		break;
	case MinorThird:
		break;
	case AugmentedQuart:
		break;
	case PerfectChord:
		break;
	default:
		break;
	}
	return false;
}

void TimeLine::createChord(ChordType m_chordType, int m_chordTime, int baseNote1, int baseNote2)
{
	int difference = 13;
	int baseNote3 = 0;
	switch (m_chordType)
	{
	case MajorThird:
		difference = abs(baseNote2 - baseNote1);
		switch (difference)
		{
		case 3 :
			baseNote3 = baseNote1 < baseNote2 ? baseNote1 - 4 : baseNote2 - 4;
			break;
		case 4 :
			baseNote3 = baseNote1 < baseNote2 ? baseNote1 + 7 : baseNote2 + 7;
			break;
		case 5 :
			baseNote3 = baseNote1 < baseNote2 ? baseNote1 -3 : baseNote2 -3;
			break;
		case 7 :
			baseNote3 = baseNote1 > baseNote2 ? baseNote1 - 3 : baseNote2 - 3;
			break;
		case 8 :
			baseNote3 = baseNote1 > baseNote2 ? baseNote1 - 5 : baseNote2 - 5;
			break;
		case 9 :
			baseNote3 = baseNote1 > baseNote2 ? baseNote1 - 4 : baseNote2 - 4;
			break;
		default:
			break;
		}
		break;
	case MinorThird:
		break;
	case AugmentedQuart:
		break;
	case PerfectChord:
		break;
	default:
		break;
	}
	chordTimesOn[chordSize] = m_chordTime;
	chordTimesOn[chordSize+1] = m_chordTime;
	chordTimesOn[chordSize+2] = m_chordTime;
	chordNotesOn[chordSize] = baseNote1;
	chordNotesOn[chordSize + 1] = baseNote2;
	chordNotesOn[chordSize + 2] = baseNote3;
	chordTimesOff[chordSize] = (m_chordTime + duration)%currentPeriod;
	chordTimesOff[chordSize + 1] = (m_chordTime + duration)%currentPeriod;
	chordTimesOff[chordSize + 2] = (m_chordTime + duration)%currentPeriod;
	chordNotesOff[chordSize] = baseNote1;
	chordNotesOff[chordSize + 1] = baseNote2;
	chordNotesOff[chordSize + 2] = baseNote3;
	chordSize += 3;
}

void TimeLine::resetAllChords()
{
	for (int i = 0; i < chordSize; ++i)
	{
		chordTimesOn[i] = 0;
		
		chordNotesOn[i] = 0;
		
		chordTimesOff[i] = 0;
		
		chordNotesOff[i] = 0;
				
	}
	chordSize = 0;
}

void TimeLine::applyOffSet(int _offset)
{
	for (int i = 0; i < midiTimesSize; i++)
	{
		midiTimes[i] += _offset;
		midiOffTimes[i] += _offset;
		if (_offset > 0)
		{
			while (midiTimes[i] > period)
				midiTimes[i] -= period;
			while (midiOffTimes[i] > period)
				midiOffTimes[i] -= period;
		}
		else
		{
			while (midiTimes[i] < 0)
				midiTimes[i] += period;
			while (midiOffTimes[i] < period)
				midiOffTimes[i] += period;
		}
	}

	
}

float TimeLine::getSpeed()
{
	return speed;
}

int TimeLine::getPeriod()
{
	return period;
}

void TimeLine::testMidi()
{
	/*for (int i = 0; i < midiTimesSize; i++)
	{
		if (midiTimes[i] > currentPeriod)
			DBG("connard");
	}*/
}