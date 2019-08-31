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

#include "FourierSignal.h"

FourierSignal::FourierSignal(double m_amplitude, double m_frequency, int m_numHarmonics) :
	AmuSignal(m_amplitude, m_frequency), numHarmonics(m_numHarmonics), phase(0)
{
}

FourierSignal::~FourierSignal()
{
}

void FourierSignal::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	//DBG("TriangleSignal::prepareToPlay");
	
	computeHarmonicsParameters(samplesPerBlockExpected,sampleRate);
}

void FourierSignal::getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill)
{
	if (state == Playing || state == Starting || state == Stopping)
	{
		float* const buffer0 = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
		float* const buffer1 = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);
		for (int i = 0; i < bufferToFill.numSamples; ++i)
		{
			updateSample();
			buffer0[i] = (float)getNextSample();
			buffer1[i] = (float)getNextSample();
		}

		if (state == Starting)
		{
			bufferToFill.buffer->applyGainRamp(0, bufferToFill.numSamples, 0, (float)amplitude);
			changeState(Playing);
		}
		else if (state == Stopping)
		{
			bufferToFill.buffer->applyGainRamp(0, bufferToFill.numSamples, (float)amplitude, 0.0f);
			changeState(Stopped);
		}
	}
	else
		bufferToFill.clearActiveBufferRegion();


}

void FourierSignal::updateSample()
{
	sample = 0;
	for (int j = 0; j < numHarmonics; ++j)
	{
		sample += harmonics[j]->getSample();
	}
}

double FourierSignal::getNextSample()
{
	return sample;
}

void FourierSignal::releaseResources()
{
	DBG("TriangleSignal::releaseResources");

	for (int i = 0; i < numHarmonics; ++i)
		delete harmonics[i];


	delete[] harmonics;
	delete[] harmonicsN;
	delete[] harmonicsA;
}

void FourierSignal::setFrequency(double newFrequency)
{
	frequency = newFrequency;
	if (harmonics != nullptr)
	{
		for (int i = 0; i<numHarmonics; ++i)
			harmonics[i]->setFrequency(harmonicsN[i] * newFrequency);
	}
}

void FourierSignal::setAmplitude(double newAmplitude)
{
	amplitude = newAmplitude;
	if (harmonics != nullptr)
		for (int i = 0; i < numHarmonics; ++i)
			harmonics[i]->setAmplitude(harmonicsA[i] * newAmplitude);
}

void FourierSignal::changeState(TransportState newState)
{
	if (state != newState)
	{
		state = newState;
		switch (state)
		{
		case Stopping:
			// faire une rampe jusque 0 pour que ca craque pas puis mettre etat a stopped
			break;
		case Stopped: // arrete de jouer et remet les harmoniques a leurs phases initaiales
			if (harmonics != nullptr)
				for (int i = 0; i < numHarmonics; ++i)
					harmonics[i]->resetPhase();
			break;
		case Starting:
			// une rampe croissante est effectuee dans le getNextAudioBlock ( avant c'etait juste : state = Playing, et pas de break;)
			break;
		case Playing:
			break;
		case Pausing:
		case Paused: // arrete de jouer mais laisse la phase la ou elle en etait
			state = Paused;
			break;
		default:
			DBG("probleme avec state");
			break;
		}
	}
}

void FourierSignal::computeHarmonicsParameters(int samplesPerBlockExpected, double sampleRate)
{
	int n = 0;
	harmonics = new AmuSinus*[numHarmonics];
	harmonicsN = new int[numHarmonics];
	harmonicsA = new double[numHarmonics];

	for (int i = 0; i < numHarmonics; ++i)
	{
		harmonicsN[i] = 0;
		harmonicsA[i] = 0;
		harmonics[i] = new AmuSinus(harmonicsA[i], n*frequency, PI / 2, sampleRate, samplesPerBlockExpected);
	}
}

AmuSinus** FourierSignal::getHarmonics()
{
	return harmonics;
}

AmuSinus* FourierSignal::getHarmonic(int idx)
{
	if (idx < numHarmonics)
		return harmonics[idx];
	else
		return nullptr;
}

int FourierSignal::getNumHarmonics()
{
	return numHarmonics;
}

int FourierSignal::getHarmonicsN(int idx)
{
	return harmonicsN[idx];
}

double FourierSignal::getHarmonicsA(int idx)
{
	return harmonicsA[idx];
}

double FourierSignal::getAmplitude()
{
	return amplitude;
}

void FourierSignal::setPhase(double newPhase)
{
	for (int i = 0; i < numHarmonics; ++i)
		harmonics[i]->setPhase(harmonicsN[i] * newPhase);
}

void FourierSignal::resetPhase(double newPhase)
{
	for (int i = 0; i < numHarmonics; ++i)
		harmonics[i]->resetPhase(harmonicsN[i] * newPhase);
}

void FourierSignal::shiftPhase(double shift)
{
	for (int i = 0; i < numHarmonics; ++i)
		harmonics[i]->shiftPhase(harmonicsN[i] * shift);
}

double FourierSignal::getPhase()
{
	if (harmonics == nullptr)
		return phase;
	else
		return harmonics[0]->getPhase();
}

double FourierSignal::getFrequency()
{
	return frequency;
}