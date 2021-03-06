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

#pragma once

#include "IPresenter.h"

#include "PeriodicUpdateThread.h"

#include <cmath>
#include <memory>

#include "JuceHeader.h"


namespace Miam {
    
    
    // Forward declarations
    class MatrixRouterAudioProcessor;
    class NetworkModel;
    class MatrixRouterAudioProcessorEditor;
    class OscMatrixComponent;
    
    
    /// \brief Base class of the PRESENTER module (see MVP scheme)
    class Presenter : public IPresenter
    {
        
        
        
        // ================== ATTRIBUTES ===================
        
        // - - - - - links to other modules - - - - -
        private :
        MatrixRouterAudioProcessor& model;
        std::shared_ptr<NetworkModel> networkModel;
        
        // - - - - - graphical objects - - - - -
        /// \brief Kept within the presenter, sent to the ProcessorEditor (=View)
        /// when necessary (when recreated by the user) for the View to become its parent
        ScopedPointer<OscMatrixComponent> oscMatrixComponent;
        
        std::string oscAddressCopy;
        
        // - - - - - time management - - - - -
        const double updateFrequency_Hz = 15.0;
        const long updatePeriod_us = 66667; // rien de bien diabolique, correspond à 15Hz
        bool isModelReadyToReceive = false; // But model can force "false"
        
        // ================== METHODS ===================
        
        // - - - - - Construction & destruction - - - - -
        public :
        Presenter(MatrixRouterAudioProcessor& _model, std::shared_ptr<NetworkModel> _networkModel);
        ~Presenter();
        
        // Callback from the Model, after the constructor of the plugin editor
        void OnPluginEditorCreated(MatrixRouterAudioProcessorEditor* view);

        // - - - - - Lock-free Communication - - - - -
        virtual void SendParamChange(AsyncParamChange& paramChange) override;
        virtual bool TrySendParamChange(AsyncParamChange& paramChange) override;
        
        // - - - - - Setters and Getters - - - - -
        OscMatrixComponent* GetOscMatrixComponent() {return oscMatrixComponent;}
        
        
        // - - - - - Periodic Update - - - - -
        const int GetUpdateFrequency_Hz() const {return (int)round(updateFrequency_Hz);}
        // Called on the Juce UI thread from View's Timer
        void UpdateFromView(MatrixRouterAudioProcessorEditor* view);
        // Override method that should not be called here (particular situation : fresh
        // view pointer necessary
        void Update() override;
        
    
        // - - - - - Callbacks from View - - - - -
        /// \brief Translates the data into a Miam::AsyncParamChange, then sends the data
        /// to the Model.
        void OnSliderValueChanged(int row, int col, double value);
        void OnMatrixZeroed();
        void OnUdpPortChanged(int udpPort);
        void OnAttackDurationChanged(double attackDuration);
        
        // - - - - - Synchronous callbacks from Model - - - - -
        // Bien vérifier à un moment que tout ça passe bien sur le thread UI dans Juce !
        void OnNewUdpPort(int udpPort, bool isConnected);
    };
    
}
