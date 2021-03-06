/* 
 * This file is part of the MIEM distribution (https://github.com/gwendal-le-vaillant/MIEM).
 * Copyright (c) 2017 Gwendal Le Vaillant.
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

#ifndef GRAPHICSESSIONMANAGER_H_INCLUDED
#define GRAPHICSESSIONMANAGER_H_INCLUDED


#include <memory>
#include <vector>
#include <deque>
#include <map>
#include <string>

#include <boost/property_tree/xml_parser.hpp>

#include "IGraphicSessionManager.h"

#include "EditablePolygon.h"

#include "Juceheader.h" // for : Point<float>, uint64

#include "MultiSceneCanvasManager.h"
#include "SceneCanvasComponent.h"

#include "editScene.h"

namespace Miam
{
	class IPresenter;
}

class EditScene;

class MultiCanvasComponentAmusing;

namespace Amusing {

	// Simple declarations
	class View;
	class Presenter;
	

	/// \brief Sub-module belonging to the Presenter module, which handles the editing
	/// of all of the EditableArea.
	///
	/// Actually owns all the EditablePolygons (and any other EditableArea in the future).
	///
	/// References itself to some components, for these components to transfer events to this sub-module
	/// directly, and not to the Presenter.
	class GraphicSessionManager : public Miam::IGraphicSessionManager
	{

	public :
		// =========================================
		
		double vitesse;
		void SetFromTree(bptree::ptree& graphicSessionTree) override;
		// =========================================

		// = = = = = = = = = = ATTRIBUTES = = = = = = = = = =

		// Graphical objects belong to the Presenter module, not to the View
	private:
		// links back to the View module
		View* view;
		EditScene *editScene;
		Presenter *myPresenter;
		MultiCanvasComponentAmusing *myMultiCanvasComponent;
		
		// Graphic Manager mode = useless for now (will rely on the managed
		// canvases modes)
		//GraphicSessionMode mode;

		// Display & editing attributes for Areas

		//std::shared_ptr<IEditableArea> areaToCopy = nullptr; ///< vector index of




															 // = = = = = = = = = = METHODS = = = = = = = = = =

	public:
		/// \brief Construction (the whole Presenter module is built after the View).
		GraphicSessionManager(Presenter* presenter_, View* view_);
		//GraphicSessionManager(IPresenter* presenter_, View* view_);

		/// \brief Destruction and the editor and the canvases
		~GraphicSessionManager();

		// Debug purposes only
		void __LoadDefaultTest();



		// ---- Getters and Setters -----
	public:

		/// \brief Gets the currently selected area, or nullptr if nothing selected
		std::shared_ptr<IEditableArea> GetSelectedArea();



		// ------ canvas managing ------
	public:
		/// \brief Sets the new active canvas and updates corresponding graphic objects. Called by the newly selected canvas itself.
		///
		/// Tells other canvases to unselect any previously selected area
		

	protected:
		std::shared_ptr<MultiSceneCanvasManager> getSelectedCanvasAsManager();

	protected:
		std::shared_ptr<MultiSceneCanvasEditor> getSelectedCanvasAsEditable();


		// ----- Running mode -----
	public:

		void CanvasModeChanged(CanvasManagerMode) override;

		


		// ----- Events from the Presenter itself -----
		virtual void HandleEventSync(std::shared_ptr<GraphicEvent> event_) override;



		// ----- Event to View -----
		void DisplayInfo(String info,int priority = 0) override;

		void OnAddSquare(float x = 0.5f, float y = 0.5f);
		void OnAddAndSelectSquare(const MouseEvent& mouseE);
		void TransmitMouseDrag(const MouseEvent& mouseE);
		void SendDeletingMouseDown(const MouseEvent& mouseE);
		void OnAddTriangle();
		void OnAddHexa();
		void OnAddCircle();
		void OnAddTrueCircle();
		void hideAddPolygon();
		void showAddPolygon();
		void OnAddComplete();
		void OnSoundClick();
		void OnDelete();
		void OnTestChangeSound();
		void OnSave(std::string filename);

		void OnLoad(std::string filename);

		void OnFollowerTranslation(std::shared_ptr<GraphicEvent> graphicE);
		void OnAudioPosition(double position);
		void SetAllAudioPositions(double position);
		void SetAudioPositions(std::shared_ptr<Cursor> area, double position);

		void lookForAreasConcerned(Colour colourConcerned); //look for the areas concerned by this update to send msg to the audio model to update the corresponding timeLines

		void OnPlayClicked();
		void OnPauseClicked();
		void OnStopClicked();
		void OnTempoChanged(int newTempo);
		void OnMasterVolumeChanged(float newVolume);

		void OnDeviceOptionsClicked();
		void SetAllChannels(); // bouger d'endroit dans le constructeur pour la comprehension (pas view event)
		void setSamplesColor(int Nsamples, Colour colorCode[]);
		void SetMidiChannel(int ch);

		void setSpeedArea(std::shared_ptr<IEditableArea> area, double speed);
		double getSpeed(std::shared_ptr<IEditableArea> area);
		void setVelocityArea(std::shared_ptr<IEditableArea> area, double velocity);
		void setOctave(std::shared_ptr<IEditableArea> area, int newOctave);
		double getVelocity(std::shared_ptr<IEditableArea> area);
		int getOctave(std::shared_ptr<IEditableArea> area);
		int getColor(std::shared_ptr<IEditableArea> area);
		void setColor(std::shared_ptr<IEditableArea> area, int colourIdx);
		int getTempo();

	private :
		bool deleting;
		int tempo;
		int circleToNote(int numCirc);
		std::shared_ptr<IDrawableArea> temporaryArea;
	};


}


#endif  // GRAPHICSESSIONMANAGER_H_INCLUDED
