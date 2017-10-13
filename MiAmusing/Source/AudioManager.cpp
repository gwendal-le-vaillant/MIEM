/*
  ==============================================================================

    AudioManager.cpp
    Created: 19 Jan 2017 10:18:34am
    Author:  ayup1

  ==============================================================================
*/
//..\..\..\..\boost_1_63_0\geometry\algorithms
#include<thread>
#include<cmath>
#include<boost\geometry.hpp>
#include<boost\lockfree\queue.hpp>
#include<boost\lockfree\spsc_queue.hpp>



#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioManager.h"
#include "AsyncParamChange.h"
#include "ADSRSignal.h"
#include "AmuSignal.h"
#include "SquareSignal.h"
#include "TriangleSignal.h"
#include "SinusSignal.h"
using namespace Amusing;
//==============================================================================
AudioManager::AudioManager(AmusingModel *m_model) : model(m_model), Nsources(0), state(Stop)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
	DBG("AudioManager::AudioManager");


	


	
	
	
	

	//initialise(0, 2, nullptr, true);
	//addAudioCallback(this);
	setSource(this);
	runThread = true;
	T = std::thread(&AudioManager::threadFunc, this);

	

	for (int i = 0; i < maxSize; i++)
	{
		timeLines[i] = 0;
		timeLinesKnown[i] = 0;
		playHeads[i] = 0;
		playHeadsKnown[i] = 0;
	}
}

AudioManager::~AudioManager()
{
	if(midiOuput != nullptr)
		for(int i=1; i< 17; i++)
			midiOuput->sendMessageNow(MidiMessage::allNotesOff(i));
	DBG("audioManager destructor");
	
	runThread = false;
	T.join();
	setSource(nullptr);
	
	DBG("audioManager destructor fin");

	
	if (midiOuput == nullptr)
	{
		DBG("midiOuput == nullptr !!!!!!!!!!!");
	}
	else
		DBG("still exist");

	
	model->removeDeviceManagerFromOptionWindow();
	//delete midiOuput;
}


void AudioManager::prepareToPlay(int samplesPerBlockExpected, double _sampleRate)
{
	DBG("AudioManager::prepareToPlay");
	currentSamplesPerBlock = samplesPerBlockExpected;
	currentSampleRate = _sampleRate;
	
	
	metronome.setAudioParameter(samplesPerBlockExpected, _sampleRate);
	
	periode = metronome.BPMtoPeriodInSample(50);//timeToSample(4000);
	position = 0;

	

	midiOuput = model->getMidiOutput();
	
	
	
}
void AudioManager::releaseResources()
{
	DBG("AudioManager::releaseResources");
	for (int j = 0; j < maxSize; j++)
	{
		if (timeLinesKnown[j] != 0)
			timeLinesKnown[j] = 0;

	}

	/*runThread = false;
	T.join();*/
	
	if (midiOuput == nullptr)
	{
		DBG("midiOuput == nullptr !!!!!!!!!!!");
	}
	else
	{
		
		if (midiOuput == nullptr)
			DBG("bien detruit ! ");
		else
			DBG("pas detruit");
	}
}
void AudioManager::getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill)
{
	//DBG(String(position));
	getParameters();
	getNewTimeLines();
	getNewPlayHeads();
	switch (state)
	{
	case Amusing::Play:
		sendPosition();
		for (int i = 0; i < bufferToFill.numSamples; ++i)
		{

			if (midiOuput != nullptr)
			{
				//midiSender->process(position);
				for (int j = 0; j < maxSize; j++)
				{
					if (playHeadsKnown[j] != 0)
						playHeadsKnown[j]->process();


				}
			}
			++position;
			if (position == periode)
			{
				//DBG("TOC");
				position = 0;
			}
		}
		break;
	case Amusing::Pause:
		break;
	case Amusing::Stop:
		position = 0;
		break;
	default:
		break;
	}
	
	bufferToFill.clearActiveBufferRegion();
	//metronome.update();
	//midiBuffer.addEvent(metronome.getNextMidiMsg(), 4);
}

void AudioManager::getNewTimeLines()
{
	TimeLine* ptr;
	while (timeLinesToAudio.pop(ptr))
	{
		if(ptr != 0)
			timeLinesKnown[ptr->getId()] = ptr;
	}
}

void AudioManager::getNewPlayHeads()
{
	PlayHead* ptr;
	while (playHeadsToAudio.pop(ptr))
	{
		if (ptr != 0)
			playHeadsKnown[ptr->getId()] = ptr;
	}
}

void AudioManager::sendMidiMessage(MidiMessage msg)
{
	if (midiOuput != nullptr)
	{
		midiOuput->sendMessageNow(msg);
	}
}

void AudioManager::sendPosition()
{
	
	AsyncParamChange param;
	param.Type = AsyncParamChange::ParamType::Position;
	

	

	for (int j = 0; j < maxSize; j++)
	{
		if (playHeadsKnown[j] != 0)
		{
			param.Id1 = j;
			param.Id2 = playHeadsKnown[j]->getTimeLineId();
			param.DoubleValue = playHeadsKnown[j]->getReadingPosition();
			model->SendParamChange(param);
		}
	}
	if (param.Id1 != 0)
		DBG("2eme tete de lecture");
	//param.Id1 = 0;
	//param.DoubleValue = (double)position / (double)periode; //+ 1.0/8.0;
	//model->SendParamChange(param);
	
}

void AudioManager::getParameters()
{
	//DBG("getParameters");
	Miam::AsyncParamChange param;
	while (model->lookForParameter(param))
	{
		//DBG("paramType = " + (String)((int)param.Type));
		switch (param.Type)
		{
		case Miam::AsyncParamChange::ParamType::Activate:
			if (param.Id2 == 1024) // cr�e ou supprime une timeLine
			{
				switch (param.IntegerValue)
				{
				case 1:

					paramToAllocationThread.push(param);
					break;
				case 0:

					timeLinesKnown[param.Id1] = 0; // so we won't access to the element anymore, we forget it
					paramToAllocationThread.push(param); // we ask to the allocation thread to delete it
					break;
				default:
					DBG("IMPOSSIBLE : 1 to activate, 0 to desactivate");
					break;
				}
			}
			else if (param.Id1 == 1024) // cr�e ou supprime une t�te de lectire
			{
				switch (param.IntegerValue)
				{
				case 1: // cr�ation
					if (playHeadsKnown[param.Id2] == 0)
						paramToAllocationThread.push(param);
					else
						playHeadsKnown[param.Id2]->setSpeed(param.DoubleValue);
					break;
				case 0: // suppression
					playHeadsKnown[param.Id2] = 0;
					paramToAllocationThread.push(param);
					break;
				default:
					break;
				}
			}
			else // (cr�e et) associe une t�te de lecture � une timeLine 
			{
				if (timeLinesKnown[param.Id1] != 0) // pour s'assurer que la timeLines � associer existe
				{
					if (playHeadsKnown[param.Id2] == 0) // la t�te de lecture n'existe pas encore -> demander cr�ation + association
					{
						paramToAllocationThread.push(param);
					}
					else // faire juste l'association car la t�te de lectire existe d�j�
					{
						playHeadsKnown[param.Id2]->LinkTo(timeLinesKnown[param.Id1]);
						playHeadsKnown[param.Id2]->setSpeed(param.DoubleValue);
					}
				}
				else
					paramToAllocationThread.push(param);
			}
			break;
		case Miam::AsyncParamChange::ParamType::Source:
			//DBG("AM : the side " + (String)param.Id2 + " is = " + (String)param.DoubleValue);
			if (timeLinesKnown[param.Id1] != 0) // si != 0 : il existe et on peut le modifier
				timeLinesKnown[param.Id1]->setMidiTime(param.Id2, roundToInt(param.DoubleValue * (double)periode), param.IntegerValue,param.FloatValue);
			else // si == 0, on ne sait pas s'il n'existe pas ou si le thread est encore en train de le creer -> envoyer au thread pour verifier et faire le necessaire
				paramToAllocationThread.push(param);
			break;
		case Miam::AsyncParamChange::ParamType::Play :
			DBG("state = Play;");
			state = Play;
			//DBG("send masterVolume midi : " + (String)roundToInt(param.FloatValue*127.0f));
			//sendMidiMessage(juce::MidiMessage::controllerEvent(1,7,roundToInt(param.FloatValue*127.0f)));//juce::MidiMessage::masterVolume(param.FloatValue));
			//sendMidiMessage(juce::MidiMessage::masterVolume(param.FloatValue));
			//param.IntegerValue = metronome.BPMtoPeriodInSample(param.IntegerValue);//timeToSample(param.IntegerValue);
			//paramToAllocationThread.push(param);
			playHeadsKnown[param.Id1]->setSpeed(param.DoubleValue);
			break;
		case Miam::AsyncParamChange::ParamType::Pause :
			DBG("state = Pause;");
			state = Pause;
			paramToAllocationThread.push(param);
			break;
		case Miam::AsyncParamChange::ParamType::Stop :
			DBG("state = Stop;");
			state = Stop;
			paramToAllocationThread.push(param);
			break;
		case Miam::AsyncParamChange::Update :
			//DBG("Update received");
			if (playHeadsKnown[param.Id2] != 0 && timeLinesKnown[param.Id1] != 0)
			{
				playHeadsKnown[param.Id2]->LinkTo(timeLinesKnown[param.Id1]);
				playHeadsKnown[param.Id2]->setSpeed(param.FloatValue);//param.DoubleValue);
				playHeadsKnown[param.Id2]->setReadingPosition(param.DoubleValue);//param.FloatValue);
				//playHeadsKnown[param.Id2]->setState(PlayHeadState::Pause);
			}
			else
				paramToAllocationThread.push(param);
			break;
		case Miam::AsyncParamChange::Duration :
			state = Play;
			sendMidiMessage(juce::MidiMessage::controllerEvent(1, 7, roundToInt(param.FloatValue*127.0f)));
			//juce::MidiMessage::masterVolume(param.FloatValue));
			sendMidiMessage(juce::MidiMessage::masterVolume(param.FloatValue));
			param.IntegerValue = metronome.BPMtoPeriodInSample(param.IntegerValue);//timeToSample(param.IntegerValue);
			paramToAllocationThread.push(param);
			break;
		case Miam::AsyncParamChange::UdpPort:
			if (timeLinesKnown[param.Id1] != 0)
				timeLinesKnown[param.Id1]->setMidiChannel(param.IntegerValue);
			else
				paramToAllocationThread.push(param);
			break;
		default:
			break;
		}
	}
}

void AudioManager::getAudioThreadMsg()
{
	Miam::AsyncParamChange param;
	while (paramToAllocationThread.pop(param))
	{
		switch (param.Type)
		{
		case Miam::AsyncParamChange::ParamType::Activate :
			//DBG("source : " + (String)param.Id1);
			if (param.Id2 == 1024)
			{
				switch (param.IntegerValue)
				{
				case 0:
					DBG("desactivate source : " + (String)param.Id1);
					
					delete timeLines[param.Id1];
					timeLines[param.Id1] = 0;
					DBG("source : " + (String)param.Id1 + "deleted");
					
					break;
				case 1:
					//DBG("AM : I construct a new polygon with ID : " + (String)param.Id1);
					
					DBG("activate source    : " + (String)param.Id1);
					if (timeLines[param.Id1] == 0)
						timeLines[param.Id1] = new TimeLine();

					timeLines[param.Id1]->setPeriod(periode);
					if (param.FloatValue != 0)
						timeLines[param.Id1]->setSpeed(param.FloatValue);
					else
						timeLines[param.Id1]->playNoteContinuously();

					timeLines[param.Id1]->setAudioManager(this);
					//DBG("midiChannel : " + (String)param.IntegerValue);
					if (param.IntegerValue != 0)
						timeLines[param.Id1]->setMidiChannel(param.IntegerValue);
					//timeLines[param.Id1]->setSpeed(param.FloatValue);
					timeLines[param.Id1]->setId(param.Id1);
					
					timeLinesToAudio.push(timeLines[param.Id1]);
					break;
				default:
					DBG("IMPOSSIBLE");
				}
			}
			else if (param.Id1 == 1024)
			{
				switch (param.IntegerValue)
				{
				case 0:
					delete playHeads[param.Id2];
					playHeads[param.Id2] = 0;
					break;
				case 1:
					if (playHeads[param.Id2] == 0)
						playHeads[param.Id2] = new PlayHead();
					playHeads[param.Id2]->setId(param.Id2);
					playHeads[param.Id2]->setSpeed(param.DoubleValue);
					playHeads[param.Id2]->setAudioManager(this);
					playHeadsToAudio.push(playHeads[param.Id2]);
					break;
				default:
					break;
				}
			}
			else
			{
				playHeads[param.Id2] = new PlayHead(); // first create the PlayHead
				playHeads[param.Id2]->setId(param.Id2);
				playHeads[param.Id2]->LinkTo(timeLines[param.Id1]);
				playHeads[param.Id2]->setSpeed(param.DoubleValue);
				playHeads[param.Id2]->setAudioManager(this);
				playHeadsToAudio.push(playHeads[param.Id2]);
			}
			break;
		case Miam::AsyncParamChange::ParamType::Source:
			//DBG("AM : the side " + (String)param.Id2 + " is = " + (String)param.DoubleValue);
			//DBG("new note received : " + (String)param.IntegerValue);
			if (timeLines[param.Id1] != 0)
				timeLines[param.Id1]->setMidiTime(param.Id2, roundToInt(param.DoubleValue * (double)periode), param.IntegerValue,param.FloatValue);
			break;
		case Miam::AsyncParamChange::ParamType::Play :
			
			break;
		case Miam::AsyncParamChange::Update:
			DBG("Updtae received");
			if (playHeads[param.Id2] != 0 && timeLines[param.Id1] != 0)
			{
				playHeads[param.Id2]->LinkTo(timeLines[param.Id1]);
				playHeads[param.Id2]->setSpeed(param.FloatValue);//param.DoubleValue);
				playHeads[param.Id2]->setReadingPosition(param.DoubleValue);//param.FloatValue);
				//playHeadsKnown[param.Id2]->setState(PlayHeadState::Pause);
			}
			break;
		case Miam::AsyncParamChange::Duration:
			for (int i = 0; i < maxSize; ++i)
			{
				if (timeLines[i] != 0)
					timeLines[i]->setPeriod(param.IntegerValue);
			}
			for (int i = 0; i < maxSize; ++i)
			{
				if (playHeads[i] != 0)
					playHeads[i]->setState(PlayHeadState::Play);
			}
			position = (int)round((double)position * (double)param.IntegerValue / (double)periode);
			periode = param.IntegerValue;
			break;
		case Miam::AsyncParamChange::Pause:
			for (int i = 0; i < maxSize; ++i)
			{
				if (playHeads[i] != 0)
					playHeads[i]->setState(PlayHeadState::Pause);
			}
			break;
		case Miam::AsyncParamChange::Stop:
			for (int i = 0; i < maxSize; ++i)
			{
				if (playHeads[i] != 0)
					playHeads[i]->setState(PlayHeadState::Stop);
			}
			break;
		case Miam::AsyncParamChange::UdpPort:
			if (timeLines[param.Id1] != 0)
				timeLines[param.Id1]->setMidiChannel(param.IntegerValue);
			break;
		default:

			break;
		}
	}
}

void AudioManager::threadFunc()
{
	while (runThread)
	{ 
		getAudioThreadMsg();
	}
	DBG("delete all the timeLines");
	for (int i = 0; i < maxSize; ++i)
	{
		if (timeLines[i] != nullptr)
			delete timeLines[i];
	}
	DBG("delete all the playheads");
	for (int i = 0; i < maxSize; ++i)
	{
		if (playHeads[i] != nullptr)
			delete playHeads[i];
	}
	DBG("exit thread");
}
