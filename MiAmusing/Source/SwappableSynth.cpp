/* 
 * This file is part of the MIEM distribution (https://github.com/gwendal-le-vaillant/MIEM).
 * Copyright (c) 2017 Guillaume Villée.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "SwappableSynth.h"


SwappableSynth::SwappableSynth()
{
	//soundA = nullptr;
	//soundB = nullptr;
	synthAPlaying = false;
	G_Off = 1.0;
	G_On = 0.0;
	audioFormatManager.registerBasicFormats();
	skipMutex = false;
	waitForUnlock = true;
	state = SwappableSynthState::Playing;
}

SwappableSynth::~SwappableSynth()
{
}

void SwappableSynth::setBuffersSize(int numChannels, int numSamples)
{
	rampBuffer_On.setSize(numChannels, numSamples);
	rampBuffer_off.setSize(numChannels, numSamples);
}

void SwappableSynth::setCurrentPlaybackSampleRate(double m_sampleRate)
{
	synthA.setCurrentPlaybackSampleRate(m_sampleRate);
	synthB.setCurrentPlaybackSampleRate(m_sampleRate);

}

void SwappableSynth::addVoice()
{
	synthA.addVoice(new SamplerVoice());
	synthB.addVoice(new SamplerVoice());
}

void SwappableSynth::clearSounds()
{
	/*if(synthA.getNumSounds()>0)
		synthA.clearSounds();
	if(synthB.getNumSounds()>0)
		synthB.clearSounds();*/


	//if (soundA != nullptr)
	//	delete soundA; // faudra mettre dans 2 synth� diff�rents
	//if (soundB != nullptr)
	//	delete soundB;
}


void SwappableSynth::setSound(const void * srcData, size_t srcDataSize, bool keepInternalCopyOfData)
{
	state = SwappableSynthState::Loading;
	std::thread (&SwappableSynth::addSoundOnThread, this, srcData, srcDataSize, keepInternalCopyOfData).detach();
	

	//BigInteger allNotes;
	//WavAudioFormat wavFormat;
	//audioReader = wavFormat.createReaderFor(new MemoryInputStream(srcData,
	//	srcDataSize,
	//	keepInternalCopyOfData),
	//	true);
	//allNotes.setRange(0, 128, true);
	//if (newSound != nullptr)
	//	delete newSound; // faudra mettre dans 2 synth� diff�rents 
	//newSound = new SamplerSound("demo sound",
	//	*audioReader,
	//	allNotes,
	//	74,   // root midi note
	//	0.1,  // attack time
	//	0.1,  // release time
	//	10.0  // maximum sample length
	//);
	//synthA.addSound(newSound);
	
}

void SwappableSynth::setSound(String soundPath)
{
	if (soundPath.isEmpty())
		DBG("wtf");
	else
	{
		
		std::thread(&SwappableSynth::addSoundFromExternalFileOnThread, this, soundPath).detach();
	}
}

void SwappableSynth::renderNextBlock(AudioSampleBuffer & outputBuffer, const MidiBuffer & inputBuffer, int startSample, int numSample)
{
	float startGain = 0.0f;
	float endGain = 0.0f;
	rampBuffer_off.clear();
	rampBuffer_On.clear();
	switch (state)
	{
	case SwappableSynthState::Loading :
		if (G_Off >= 0)
		{
			if (synthAPlaying)
			{
				synthA.renderNextBlock(rampBuffer_off, inputBuffer, startSample, numSample);
				//synthB.renderNextBlock(rampBuffer_On, inputBuffer, 0.0f, 0.0f); // juste test...
			}
			else
			{
				synthB.renderNextBlock(rampBuffer_off, inputBuffer, startSample, numSample);
				//synthA.renderNextBlock(rampBuffer_On, inputBuffer, 0.0f, 0.0f); // juste test...
			}
			startGain = G_Off;
			endGain = getNextGainOff(numSample);
			for (int ch = 0; ch < outputBuffer.getNumChannels(); ++ch)
				outputBuffer.addFromWithRamp(ch, startSample, rampBuffer_off.getWritePointer(ch), numSample, startGain, endGain);
		}
		break;
	case SwappableSynthState::Playing :
		if (synthAPlaying)
			synthA.renderNextBlock(outputBuffer, inputBuffer, startSample, numSample);
		else
			synthB.renderNextBlock(outputBuffer, inputBuffer, startSample, numSample);
		break;
	case SwappableSynthState::Swapping :
		if (synthAPlaying)
		{
			synthA.renderNextBlock(rampBuffer_off, inputBuffer, startSample, numSample);
			synthB.renderNextBlock(rampBuffer_On, inputBuffer, startSample, numSample);
		}
		else
		{
			synthB.renderNextBlock(rampBuffer_off, inputBuffer, startSample, numSample);
			synthA.renderNextBlock(rampBuffer_On, inputBuffer, startSample, numSample);
		}

		if (G_Off != 0.0) // si le synth� pr�c�dent n'est pas encore arr�t�, continuer de le diminuer
		{
			startGain = G_Off;
			endGain = getNextGainOff(numSample);
			for (int ch = 0; ch < outputBuffer.getNumChannels(); ++ch)
				outputBuffer.addFromWithRamp(ch, startSample, rampBuffer_off.getWritePointer(ch), numSample, startGain, G_Off);
		}
		
		startGain = G_On;
		endGain = getNextGainOn(numSample);
		for (int ch = 0; ch < outputBuffer.getNumChannels(); ++ch)
			outputBuffer.addFromWithRamp(ch, startSample, rampBuffer_On.getWritePointer(ch), numSample, startGain, G_On);
		
		if (G_On == 1.0 && G_Off == 0)
		{
			G_On = 0.0;
			G_Off = 1.0;
			state = SwappableSynthState::Playing;
			synthAPlaying = !synthAPlaying;
			if (skipMutex)
			{
				waitForUnlock = false;
			}
			
		}

		break;
	default:
		break;
	}
	//synthA.renderNextBlock(outputBuffer, inputBuffer, startSample, numSample);
	
}

void SwappableSynth::skipSwapping(bool m_shouldSkip)
{
	if (m_shouldSkip)
		skipMutex = false;
	else
		skipMutex = true;
}

bool SwappableSynth::setState(SwappableSynthState /*newState*/)
{

	return false;
}

void SwappableSynth::addSoundOnThread(const void * srcData, size_t srcDataSize, bool keepInternalCopyOfData)
{

	if (!synthAPlaying)
	{
		synthA.clearSounds();
	}
	else
		synthB.clearSounds();

	BigInteger allNotes;
	WavAudioFormat wavFormat;
	ScopedPointer<AudioFormatReader> audioReader = wavFormat.createReaderFor(new MemoryInputStream(srcData,
		srcDataSize,
		keepInternalCopyOfData),
		true);
	allNotes.setRange(0, 128, true);
	//if (!synthAPlaying)
	//{
	//	//if(soundA != nullptr)
	//	//	delete soundA;
	//	soundA = new SamplerSound("demo sound",
	//		*audioReader,
	//		allNotes,
	//		74,   // root midi note
	//		0.1,  // attack time
	//		0.1,  // release time
	//		10.0  // maximum sample length
	//	);
	//}
	//else if (synthAPlaying)
	//{
	//	//if(soundB != nullptr)
	//	//	delete soundB;
	//	soundB = new SamplerSound("demo sound",
	//		*audioReader,
	//		allNotes,
	//		74,   // root midi note
	//		0.1,  // attack time
	//		0.1,  // release time
	//		10.0  // maximum sample length
	//	);
	//}
	

	if (!synthAPlaying)
		synthA.addSound(new SamplerSound("demo sound",
			*audioReader,
			allNotes,
			74,   // root midi note
			0.1,  // attack time
			0.1,  // release time
			10.0  // maximum sample length
		));
	else
		synthB.addSound(new SamplerSound("demo sound",
			*audioReader,
			allNotes,
			74,   // root midi note
			0.1,  // attack time
			0.1,  // release time
			10.0  // maximum sample length
		));

	
	//DBG("thread finished : "+ (String)synthA.getNumSounds() + " " + (String)synthB.getNumSounds());
}

void SwappableSynth::addSoundFromExternalFileOnThread(String soundPath)
{
	///////////////////
	synthMtx.lock();///	Mutex to prevent access to one of the internal synth from loading to playing state
					/////////////////// Mutex is release in the renderNextBlock when reaching the Playing state

	// avoid to load two times the same sample
	//if (synthAPlaying)
	//{
	//	if (soundPath == currentPathA) // A joue et c'est d�j� le bon son 
	//		return;
	//	else
	//		currentPathA = soundPath;  // A joue mais ce n'est pas le bon son
	//}
	//else // idem avec B
	//{
	//	if (soundPath == currentPathB)
	//		return;
	//	else
	//		currentPathB = soundPath;
	//}

	

	//if (synthAPlaying)
	//{
	//	currentPathA = soundPath;
	//}
	//else
	//{
	//	currentPathB = soundPath;
	//}

	if (soundPath == currentPathA) // A contient d�j� ce sample
	{
		if (synthAPlaying) // A joue d�j� -> rien � changer : arr�t du thread car rien � changer ! 
		{
			synthMtx.unlock(); 
			return;
		}
		else // A ne joue pas -> il faut faire le swap entre les deux synth� internes !
		{
			// entre dans l'�tat "swapping", lorsqu'on atteint l'�tat "playing" on lib�re le mutex
			state = SwappableSynthState::Swapping;
			waitForUnlock = true;
			while (waitForUnlock)
			{
			}
			/////////////////////
			synthMtx.unlock();/// unlock the mutex -> the internal synths are once again accessible
			/////////////////////
			return;
		}
	}
	else if (soundPath == currentPathB)
	{
		if (synthAPlaying) // A joue alors que B a d�j� le bon sample -> swapping
		{
			// entre dans l'�tat "swapping", lorsqu'on atteint l'�tat "playing" on lib�re le mutex
			state = SwappableSynthState::Swapping;
			waitForUnlock = true;
			while (waitForUnlock)
			{
			}
			/////////////////////
			synthMtx.unlock();/// unlock the mutex -> the internal synths are once again accessible
			/////////////////////
			return;
		}
		else // B joue d�j� -> rien � changer -> on peut direct lib�rer
		{
			synthMtx.unlock();
			return;
		}
	}

	state = SwappableSynth::Loading;


	// si le nom du fichier est un des fichiers embarqu�s -> chargement diff�rent
	bool isBinary = false;
	for (int i = 0; i < BinaryData::namedResourceListSize; ++i)
	{
		if (soundPath == BinaryData::namedResourceList[i])
		{
			isBinary = true;
			int dataSize = 0;
			const void * srcData = BinaryData::getNamedResource(BinaryData::namedResourceList[i], dataSize);
			addSoundOnThread(srcData, dataSize, false);
			DBG("thread finished : " + soundPath);
		}
	}

	if (!isBinary)
	{
		if (!synthAPlaying)
		{
			synthA.clearSounds();
		}
		else
			synthB.clearSounds();


		//const File file("C:\\Users\\ayup1\\Documents\\Juce Test Recording 0.wav"); // Downloads\\Bass-Drum-1.wav");
		const File file(soundPath);
		ScopedPointer<AudioFormatReader> audioReader = audioFormatManager.createReaderFor(file);

		BigInteger allNotes;

		allNotes.setRange(0, 128, true);

		if (!synthAPlaying)
		{
			/*soundA = new SamplerSound("demo sound",
				*audioReader,
				allNotes,
				74,   // root midi note
				0.1,  // attack time
				0.1,  // release time
				10.0  // maximum sample length
			);*/


			synthA.addSound(new SamplerSound(soundPath,
				*audioReader,
				allNotes,
				74,   // root midi note
				0.1,  // attack time
				0.1,  // release time
				10.0  // maximum sample length
			));
		}
		else
		{
			/*soundB = new SamplerSound("demo sound",
				*audioReader,
				allNotes,
				74,   // root midi note
				0.1,  // attack time
				0.1,  // release time
				10.0  // maximum sample length
			);*/
			synthB.addSound(new SamplerSound(soundPath,
				*audioReader,
				allNotes,
				74,   // root midi note
				0.1,  // attack time
				0.1,  // release time
				10.0  // maximum sample length
			));
		}


		

		DBG("thread finished : " + soundPath);//(String)synthA.getNumSounds() + " " + (String)synthB.getNumSounds());
	
	}

	if (synthAPlaying)
		currentPathB = soundPath;
	else
		currentPathA = soundPath;

	if (!skipMutex)
	{
		

		state = SwappableSynthState::Playing;
		synthAPlaying = !synthAPlaying; // le synth� interne courant a chang�
		/////////////////////
		synthMtx.unlock();/// unlock the mutex -> the internal synths are once again accessible
		/////////////////////
		
	}
	else
	{
		// besoin de passer par un �tat de "swapping", � la fin de cet �tat, on pourra de nouveau acc�der au synth�
		state = SwappableSynthState::Swapping;
		waitForUnlock = true;
		while (waitForUnlock)
		{
		}
		/////////////////////
		synthMtx.unlock();/// unlock the mutex -> the internal synths are once again accessible
		/////////////////////
	}
}

float SwappableSynth::getNextGainOff(int numSamples)
{
	G_Off += (float)( ((double)numSamples / synthA.getSampleRate()) * (0.0 - 1.0) / ((double)T_Off / 1000.0));
	if (G_Off < 0.0f)
		G_Off = 0.0f;
	return G_Off;
}

float SwappableSynth::getNextGainOn(int numSamples)
{
	G_On += (float)(((double)numSamples / synthA.getSampleRate()) * (1.0 - 0.0) / ((double)T_On / 1000.0));
	if (G_On > 1.0f)
		G_On = 1.0f;
	return G_On;
}
