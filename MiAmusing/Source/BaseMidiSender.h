/*
  ==============================================================================

    BaseMidiSender.h
    Created: 18 May 2017 11:13:19am
    Author:  ayup1

  ==============================================================================
*/

#pragma once

#include<vector>
//#include "AudioManager.h"
namespace Amusing
{
	class AudioManager;
}

class TimeLine
{
public:
	TimeLine();
	~TimeLine();

	void setAudioManager(Amusing::AudioManager* m_audioManager);
	void setPeriod(int m_period);
	void setMidiTime(int idx, int newTime);
	void setId(int m_Id);
	int getId();

	void process(int time);

private:
	static const int maxSize = 128;
	double midiTimes[maxSize]; // times to send MIDI
	double midiOffTimes[maxSize];
	int midiTimesSize;
	int midiOfftimesSize;
	
	bool noteOffSent;

	int Id; // to �ake link between the timeLine and the graphic object that it represents

	// parameter of the notes to send
	int noteNumber;
	int velocity;
	int duration;
	int period; // period, to be sure we don't set a noteOff signal after the end of the period...

	// reference to the audioManager to send the MIDI
	Amusing::AudioManager* audioManager;
};

