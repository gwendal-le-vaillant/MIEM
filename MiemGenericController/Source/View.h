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
#ifndef VIEW_H_INCLUDED
#define VIEW_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

#include "PlayerView.h"

#include "MainComponent.h"


namespace Miam {
    
    class Presenter; // sufficient declaration for a pointer class member
    class MultiCanvasComponent;
    
    
    
    /// \brief One of the three main modules of the Miam::Edit program, that obeys orders from 
	/// the Presenter, while only managing simple display logic
	///
	/// Read <a href="http://en.wikipedia.org/wiki/Model%E2%80%93view%E2%80%93presenter">MPV scheme</a>
	/// for a better understanding of MiamEdit functionnalities between the Model, the Presenter 
	/// and the View : "The view is a passive interface that displays data (the model) and routes
	/// user commands (events) to the presenter to act upon that data."
	///
	/// \remark Usual GUI controls (created from the Projucer) belong to this module.
    class View : public PlayerView
    {
        
        // ========== ATTRIBUTES ==========
        private :
        Presenter* presenter;
        
        // Owned by the MainWindow within the MiamEditApplication
        // Ne doit être pas être généralisé ! Chaque projet Juce en crée un différent....
        MainContentComponent* mainContentComponent;
        
        


		// ========== METHODS ==========

        public :
        /// \brief Constructor
		///
		/// \param _mainContentComponent Pointer to the unique MainContentComponent, which is
		/// auto-instanciated by Juce within the MainWindow, and sent to the View module from
		/// the MiamEditApplication.
        View(DocumentWindow* mainWindow_, MainContentComponent* _mainContentComponent);
		/// \brief Destructor
        virtual ~View();
        /// \brief Function called after both View and Presenter are contructed
        void CompleteInitialization(Presenter* _presenter);
        /// \brief Function called after both View and Presenter are contructed
        virtual void CompleteInitialization(GraphicSessionPlayer*, MultiCanvasComponent*) override;
        
        virtual void TriggerGLResourcesRelease() override;

        
        // ----- Graphical updates -----
        virtual void ForceResized() override;
        
        
        // ----- Setters and Getters -----
	public :
        
        // Basic
		/// \brief Pointer to the unique MainContentComponent.
        MainContentComponent* GetMainContentComponent()
		{return mainContentComponent;}
        
        // General UI controls and displays

        
    };
    
    
    
    
    
}

#endif  // VIEW_H_INCLUDED
