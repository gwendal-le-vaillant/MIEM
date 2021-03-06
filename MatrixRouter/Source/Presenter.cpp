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

#include <cmath>

#include "Presenter.h"
#include "AppPurpose.h"

#include "PluginProcessor.h" // class MatrixRouterAudioProcessor
#include "NetworkModel.h"

#include "OscMatrixComponent.h"

#include "MatrixSlider.h" // min and max volumes

#include "ModelDefines.h" // Enums for lock-free communication

#include "HelpContents.h"

using namespace Miam;



AppPurpose App::appPurpose = AppPurpose::Spatialisation;

// =================== Construction & destruction ===================
Presenter::Presenter(MatrixRouterAudioProcessor& _model, std::shared_ptr<NetworkModel> _networkModel)
    :
    model(_model), networkModel(_networkModel),
oscAddressCopy(networkModel->GetOscAddress()) // thread-safe at this point
{
    oscMatrixComponent = new OscMatrixComponent(this);
    oscMatrixComponent->SetNetworkHelpContent(HelpContents::GetOscCommandsHelp());
}

Presenter::~Presenter()
{
}


void Presenter::OnPluginEditorCreated(MatrixRouterAudioProcessorEditor* view)
{
    // Update request, row by row, every 5ms
    AsyncParamChange updateRequest;
    updateRequest.Type = AsyncParamChange::UpdateDisplay;
    for (int i=-1 ; i<JucePlugin_MaxNumInputChannels ; i++)
    {
        // CONVENTION : Id1 represents the row to update
        // EXCEPTION : -1 represents any other info about the matrix (in/out count, ...)
        updateRequest.Id1 = i;
        SendParamChange(updateRequest);
        // Not to charge too much the audio processor
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}



// =================== Lock-free communication ===================
void Presenter::SendParamChange(AsyncParamChange& paramChange)
{
    //if (isModelReadyToReceive)
        LockFreeParamChangeSender::SendParamChange(paramChange);
    //else
        //throw std::runtime_error("Cannot send the parameter (model is not ready)");
}
bool Presenter::TrySendParamChange(AsyncParamChange& paramChange)
{
    if (isModelReadyToReceive)
        return LockFreeParamChangeSender::TrySendParamChange(paramChange);
    else
        return false;
}


// =================== Periodic Update ===================

void Presenter::UpdateFromView(MatrixRouterAudioProcessorEditor* view)
{
    // Have new messages arrived ?
    AsyncParamChange newParamChange;
    while( model.TryGetAsyncParamChange(newParamChange) )
    {
        switch (newParamChange.Type)
        {
            case AsyncParamChange::Activate :
                if (newParamChange.Id1 == ActivateId::PresenterToModelParametersTransmission)
                    isModelReadyToReceive = true;
                break;
                
            case AsyncParamChange::InputsAndOutputsCount :
                oscMatrixComponent->SetActiveSliders(newParamChange.Id1, newParamChange.Id2);
                //oscMatrixComponent->SetActiveSliders(JucePlugin_MaxNumInputChannels, JucePlugin_MaxNumOutputChannels);
                break;
                
            case AsyncParamChange::Duration :
                if (newParamChange.Id1 == DurationId::AttackTime) // Attack time
                    oscMatrixComponent->SetAttackSliderValue( (double) newParamChange.IntegerValue);
                break;
                
            case AsyncParamChange::Volume :
                oscMatrixComponent->SetSliderValue(newParamChange.Id1, newParamChange.Id2, newParamChange.FloatValue);
                break;
                
            default :
                break;
        }
    }
}


void Presenter::Update()
{
    throw std::runtime_error("Classic Update() from Presenter should not be called without specifying a View module (that may not exist when working with audio plug-ins");
}


// =================== Callbacks from View ===================

void Presenter::OnSliderValueChanged(int row, int col, double value)
{
    AsyncParamChange paramChange;
    paramChange.Type = AsyncParamChange::Volume;
    paramChange.Id1 = row;
    paramChange.Id2 = col;
    
    // We keep values over min+0.5dB only
    if (value < (MatrixSlider::GetMinVolume_dB() + 0.5))
        paramChange.FloatValue = 0.0f;
    else
        paramChange.FloatValue = (float)Decibels::decibelsToGain(value);
    
    // Enqueuing
    TrySendParamChange(paramChange);
}
void Presenter::OnMatrixZeroed()
{
    AsyncParamChange paramChange;
    paramChange.Type = AsyncParamChange::Reinitialize;
    // Enqueuing
    TrySendParamChange(paramChange);
}
void Presenter::OnUdpPortChanged(int udpPort)
{
    // At this point : called from the Juce UI thread (so : OK), notifyModel=false
    bool isUdpConnected = networkModel->SetUdpPort(udpPort, false);
    // Self-update
    this->OnNewUdpPort(udpPort, isUdpConnected);
}
void Presenter::OnAttackDurationChanged(double attackDuration)
{
    AsyncParamChange paramChange;
    paramChange.Type = AsyncParamChange::Duration;
    paramChange.Id1 = DurationId::AttackTime;
    paramChange.IntegerValue = (int) std::round(attackDuration);
    
    // Enqueuing
    TrySendParamChange(paramChange);
}


// =================== (possibly) Synchronous callbacks from Model ===================
void Presenter::OnNewUdpPort(int udpPort, bool isConnected)
{
    oscMatrixComponent->SetUdpPortAndMessage(udpPort, isConnected, oscAddressCopy);
}




