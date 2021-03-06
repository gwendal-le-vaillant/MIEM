/* 
 * This file is part of the MIEM distribution (https://github.com/gwendal-le-vaillant/MIEM).
 * Copyright (c) 2019 Gwendal Le Vaillant.
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


#include <iostream>

#include <iomanip> // setprecision
#include <sstream> // stringstream


#include "FrequencyMeasurer.h"

using namespace Miam;


FrequencyMeasurer::FrequencyMeasurer(std::string name_, int averagingFramesCount_, bool delayMeasureStart)
:
name(name_),
beginStatistics(!delayMeasureStart), // we do start right there if there is a delay
minDuration_us(std::chrono::microseconds(1<<30)), // us : signed integer on at least 55 bits, doc says !
maxDuration_us(std::chrono::microseconds(0)),
averagingFramesCount(averagingFramesCount_),
lastSequenceFramesCount(0),
isFreshAverageAvailable(false)
{
	init();
}


void FrequencyMeasurer::init()
{
    lastFrameTimePt = MeasureClock::now();
    lastSequenceTimePt = lastFrameTimePt;
    lastSequenceFramesCount = 0;
}


// = = = = = = = = = = Getters and Setters = = = = = = = = = =

double FrequencyMeasurer::GetLastFrequency_Hz() const
{
    return 1000.0/GetLastDuration_ms();
}

long FrequencyMeasurer::GetElapsedTimeSinceLastNewFrame_us() const
{
    auto currentTimePt = MeasureClock::now();
    std::chrono::microseconds elapsedTimeSinceLastNewFrame = std::chrono::duration_cast<std::chrono::microseconds>(currentTimePt - lastFrameTimePt);
    
    return (long)elapsedTimeSinceLastNewFrame.count();
}

double FrequencyMeasurer::GetLastDuration_ms() const
{
    return (double)(lastFrameDuration.count())/1000.0;
}



void FrequencyMeasurer::OnNewFrame()
{
    // 1-frame values in any case
    auto currentTimePt = MeasureClock::now();
    lastFrameDuration = std::chrono::duration_cast<std::chrono::microseconds>(currentTimePt - lastFrameTimePt);
    lastFrameTimePt = currentTimePt;

	// Initialization/Launching phase
	// (not counting Average/Min/Max yet)
	if (!beginStatistics)
	{
		if (lastSequenceFramesCount>averagingFramesCount)
			beginStatistics = true;
	}
	// Normal case, computing statistic values
	else
	{
		// Comparison of min/max values on 1 frame
		if (lastFrameDuration < minDuration_us)
			minDuration_us = lastFrameDuration;
		else if (lastFrameDuration > maxDuration_us)
			maxDuration_us = lastFrameDuration;
		// Average computing when necessary
		if (lastSequenceFramesCount >= averagingFramesCount)
		{
			auto totalDuration_us = std::chrono::duration_cast<std::chrono::microseconds>(currentTimePt - lastSequenceTimePt);
			averageFrequency = (double)lastSequenceFramesCount * 1000000.0f / (double)(totalDuration_us.count());
			isFreshAverageAvailable = true;
            
			// reinit of sequence counter
			lastSequenceTimePt = currentTimePt;
			lastSequenceFramesCount = -1;
		}
    }
	// In any case : increment of the sequence counter
	lastSequenceFramesCount++;
}


std::string FrequencyMeasurer::GetInfo(bool displayMinMax)
{
    std::string result = name + " F=";
    {
        std::stringstream numberStream;
        numberStream << std::fixed << std::setprecision(digits) << averageFrequency;
        result += numberStream.str();
        result += "Hz ";
    }
	if (displayMinMax)
	{
		{
			result += "Tmin=";
			std::stringstream numberStream;
			numberStream << std::fixed << std::setprecision(digits) << ((double)(minDuration_us.count()) / 1000.0);
			result += numberStream.str();
			result += "ms ";
		}
		{
			result += "Tmax=";
			std::stringstream numberStream;
			numberStream << std::fixed << std::setprecision(digits) << ((double)(maxDuration_us.count()) / 1000.0);
			result += numberStream.str();
			result += "ms";
		}
	}
    
    isFreshAverageAvailable = false;
    
    return result;
}


