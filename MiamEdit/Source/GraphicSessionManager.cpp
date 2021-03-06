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

#include "GraphicSessionManager.h"


// Other includes

#include "View.h"
#include "Presenter.h"

#include "SceneEvent.h"

#include "ControlPolygon.h"
#include "XmlUtils.h"

#include "HelpTexts.h"

using namespace Miam;


// ========== CONSTRUCTION and DESTRUCTION ==========

GraphicSessionManager::GraphicSessionManager(View* _view, Presenter* presenter_) :
    GraphicControlSessionManager(presenter_),
    view(_view),
    presenter(presenter_),
	mode(GraphicSessionMode::Null)
{
    setMode(GraphicSessionMode::Loading);
    
    
    
    // Links to the view module
    sceneEditionComponent = view->GetMainContentComponent()->GetSceneEditionComponent();
    
    // **************  NOMBRE DE CANEVAS EST DÉFINI ICI ###################
    // ((2))     1 SEUL MAINTENANT en l'occurence pour tous les programmes MIEM
    // On doit créer les sous-objets graphiques de canevas (View) avant de
    // les transmettre au sous-module de gestion de canevas (Presenter) que l'on crée
    // d'ailleurs ici aussi.
    canvasManagers.push_back(std::shared_ptr<MultiSceneCanvasInteractor>(new MultiSceneCanvasEditor(this, multiCanvasComponent->AddCanvas(), SceneCanvasComponent::Id::Canvas1)));    
    //canvasManagers.push_back(std::shared_ptr<MultiSceneCanvasInteractor>(new MultiSceneCanvasEditor(this, multiCanvasComponent->AddCanvas(), SceneCanvasComponent::Id::Canvas2)));
    
    // Links to the view module
    sceneEditionComponent->CompleteInitialization(this, multiCanvasComponent);

    for (size_t i=0 ; i<canvasManagers.size() ; i++)
    {
        // After canvases are created : creation of 1 empty scene (to avoid bugs...)
        // On sélectionne directement cette scène
        canvasManagers[i]->AddScene("[scène vide]", true);
    }
    
    // Finally, state of the presenter
    setMode(GraphicSessionMode::Loaded);
    // And states of the canvases
    for (size_t i=0 ; i<canvasManagers.size() ; i++)
    {
        canvasManagers[i]->SetMode(CanvasManagerMode::Unselected);
    }
    
    
    // SÉLECTION/CHARGEMENT D'UN TRUC PAR DÉFAUT
    SetSelectedCanvas(canvasManagers.front());
     
}

GraphicSessionManager::~GraphicSessionManager() {}



// ===== SETTERS AND GETTERS =====
uint64_t GraphicSessionManager::GetNextAreaId()
{
    uint64_t areaIdBackup = nextAreaId;
    nextAreaId++;
    return areaIdBackup;
}
AppPurpose GraphicSessionManager::GetSessionPurpose()
{
    return presenter->GetSessionPurpose();
}
std::shared_ptr<ControlArea> GraphicSessionManager::GetSelectedArea()
{
    if (selectedCanvas)
    {
        auto canvasSelectedArea = getSelectedCanvasAsEditable()->GetSelectedArea();
        
        // If no area is selected, we send nullptr.
        if (canvasSelectedArea == nullptr)
            return nullptr;
        
        // But if something is selected, it must be castable to Control Area
        if (auto spatArea = std::dynamic_pointer_cast<ControlArea>(canvasSelectedArea))
            return spatArea;
        else
            throw std::runtime_error("Currently selected area cannot be casted to Miam::ControlArea");
    }
    else
        return nullptr;
}
void GraphicSessionManager::SetGlobalExcitersConstraint(SceneConstrainer::ConstraintType constraint)
{
    // parent call
    this->GraphicControlSessionManager::SetGlobalExcitersConstraint(constraint);
    // then added behavior
    
    // No undefined constraint should be transmitted at this point !
    assert(constraint != SceneConstrainer::ConstraintType::None);
    
    sceneEditionComponent->SetExciterConstraintButtonEnabled(
        (constraint != SceneConstrainer::ConstraintType::Bypass)
        && (constraint != SceneConstrainer::ConstraintType::None));
}






// ===== UTILITIES =====



void GraphicSessionManager::SetSelectedCanvas(std::shared_ptr<MultiSceneCanvasInteractor> _selectedCanvas)
{
    
    // We do something only if there has been a change
    if (selectedCanvas != _selectedCanvas)
    {
        // At first : unselection of previous canvas...
        if (selectedCanvas)
        {
            selectedCanvas->SetMode(CanvasManagerMode::Unselected);
            selectedCanvas->CallRepaint();
        }
    
        
        selectedCanvas = _selectedCanvas;
        getSelectedCanvasAsEditable(); // just for the internal test of "editability"
        
        // To force updates (PLUS MAINTENANT !!!!)
        //selectedCanvas->SelectScene(selectedCanvas->GetSelectedSceneId());
        selectedCanvas->SetMode(CanvasManagerMode::SceneOnlySelected);
    
        setMode(GraphicSessionMode::CanvasSelected);
    }
    else
    {
        // rien du tout
    }
}

std::shared_ptr<MultiSceneCanvasEditor> GraphicSessionManager::getSelectedCanvasAsEditable()
{
    std::shared_ptr<MultiSceneCanvasEditor> canvasPtr = std::dynamic_pointer_cast<MultiSceneCanvasEditor>( selectedCanvas);
    if (canvasPtr)
        return canvasPtr;
    else throw std::runtime_error("Canvas not selected, or canvas cannot be casted as a Miam::MultiSceneCanvasEditor");
}



/* Regroups all necessary actions on a mode change,
 * moreover : does not always necessarily allow the mode change requested !
 */
void GraphicSessionManager::setMode(GraphicSessionMode newMode)
{
    // Post-processing necessary, or not ?
    bool forceExcitersRefresh = false;
    
    // OLD mode managing
    switch (mode)
    {
        case GraphicSessionMode::Null :
            break; // direct break, we don't do anything for now
            
        case GraphicSessionMode::Loading :
            // bypass of everything if the session is still loading
            if (newMode != GraphicSessionMode::Loaded)
                return;
            break;
            
        // If we **really** quit the exciter-related modes
        case GraphicSessionMode::ExcitersEdition :
        case GraphicSessionMode::ExciterSelected :
            if (newMode != GraphicSessionMode::ExcitersEdition
                && newMode != GraphicSessionMode::ExciterSelected)
            {
                presenter->TryModelStop();
            }
            break;
            
        default:
            // Closing of anything pop-up-like displayed on a mode change
            // Pour éviter le problème d'assertion de Juce, qui ne ferme pas les popups comme il faut avec les menus déroulants... Tjs bugué sous Juce 5.1.2
            sceneEditionComponent->CloseTemporaryDisplayedObjects();
    }
    
    // NEW mode managing
    switch(newMode)
    {
        case GraphicSessionMode::Null :
            std::cout << "Presenter:: (GraphicSessionManager) : mode d'édition \"Null\" non-implémenté" << std::endl;
            break;
            
        case GraphicSessionMode::Loading :
            std::cout << "Presenter:: (GraphicSessionManager) : mode d'édition \"Loading\" non-implémenté" << std::endl;
            break;
            
        case GraphicSessionMode::Loaded :
            // nothing to paste when freshly loaded
            sceneEditionComponent->SetPasteEnabled(false);
            
        case GraphicSessionMode::NothingSelected :
            sceneEditionComponent->SetCanvasGroupHidden(true);
            sceneEditionComponent->SetAreaGroupReduced(true);
            sceneEditionComponent->SetSpatGroupReduced(true);
            sceneEditionComponent->SetInitialStateGroupHidden(true);
            sceneEditionComponent->resized(); // right menu update
            break;
            
        case  GraphicSessionMode::CanvasSelected :
            sceneEditionComponent->SetCanvasInfo(selectedCanvas->GetId());
            sceneEditionComponent->SetCanvasGroupHidden(false);
            sceneEditionComponent->SetDeleteSceneButtonEnabled(getSelectedCanvasAsEditable()->GetScenesCount() > 1);
            sceneEditionComponent->SetAreaGroupHidden(false);
            sceneEditionComponent->SetAreaGroupReduced(true);
            if (areaToCopy)
                sceneEditionComponent->SetPasteEnabled(true);
            sceneEditionComponent->SetSpatGroupReduced(true);
            sceneEditionComponent->SetInitialStateGroupHidden(false);
            sceneEditionComponent->SetInitialStateGroupReduced(true);
            sceneEditionComponent->resized(); // right menu update
            
            view->DisplayInfo("Edition of a canvas and its scenes.");
            break;
            
        case GraphicSessionMode::AreaSelected :
            if (GetSelectedArea() == nullptr)
                throw std::runtime_error("No area selected, mode cannot be AreaSelected");
            
            sceneEditionComponent->SetEnabledAllControls(true, true); // as we may come from "waiting for something creation/deletion"
            sceneEditionComponent->SetCanvasGroupHidden(true);
            sceneEditionComponent->SetAreaGroupHidden(false);
            sceneEditionComponent->SetAreaGroupReduced(false);
            if (areaToCopy)
                sceneEditionComponent->SetPasteEnabled(true);
            sceneEditionComponent->SetSpatGroupReduced(false);
            sceneEditionComponent->SelectSpatState(GetSelectedArea()->GetStateIndex());
            sceneEditionComponent->SetInitialStateGroupHidden(true);
            sceneEditionComponent->resized(); // right menu update
            
            view->DisplayInfo("Edition of an area.");
            break;
            
            /*
         case GraphicSessionMode::EditingArea :
            break;*/
            
            
        case GraphicSessionMode::ExcitersEdition :
            forceExcitersRefresh = true;
            sceneEditionComponent->SetCanvasGroupHidden(true);
            sceneEditionComponent->SetAreaGroupHidden(true);
            sceneEditionComponent->SetSpatGroupReduced(true);
            sceneEditionComponent->SetInitialStateGroupHidden(false);
            sceneEditionComponent->SetInitialStateGroupReduced(false);
            sceneEditionComponent->SetDeleteExciterButtonEnabled(false);
            sceneEditionComponent->resized(); // right menu update
            view->DisplayInfo("Edition of the exciters' initial position.");
            // If we **really** enter the exciter-related modes
            if (mode != GraphicSessionMode::ExciterSelected)
                enterModelPlayMode();
            break;
            
        case GraphicSessionMode::ExciterSelected :
            forceExcitersRefresh = true;
            sceneEditionComponent->SetDeleteExciterButtonEnabled(true);
            // If we **really** enter the exciter-related modes
            if (mode != GraphicSessionMode::ExcitersEdition)
                enterModelPlayMode();
            break;
            
        case GraphicSessionMode::WaitingForPointCreation :
            sceneEditionComponent->DisableAllButtonsBut("Add Point text button");
            break;
            
        case GraphicSessionMode::WaitingForPointDeletion :
            sceneEditionComponent->DisableAllButtonsBut("Delete Point text button");
            break;
            
        default :
            break;
    }
    
    // Internal order : we finally don't even discuss it
    mode = newMode;
    
    // Post-change processings
	// only if we are now in a "stable" fully-defined mode
	if (mode != GraphicSessionMode::Null
		&& mode != GraphicSessionMode::Loading)
	{
		if (forceExcitersRefresh)
		{
			if (selectedCanvas)
				selectedCanvas->RecomputeAreaExciterInteractions(); // internal events sending
			else
				assert(false); // we should never enter exciter mode without a canvas selected
		}
		if (sceneEditionComponent)
			sceneEditionComponent->SetInfoHelpText(HelpTexts::GetScenesContextualHelp(mode, GetSessionPurpose()));
	}
}

void GraphicSessionManager::enterModelPlayMode()
{
    presenter->TryConnectModelToRemote();
    presenter->TryModelPlay();
}


void GraphicSessionManager::CanvasModeChanged(CanvasManagerMode canvasMode)
{    
    switch (canvasMode)
    {
        case CanvasManagerMode::SceneOnlySelected :
            setMode(GraphicSessionMode::CanvasSelected);
            break;
            
        case CanvasManagerMode::AreaSelected :
            setMode(GraphicSessionMode::AreaSelected);
            break;
            
            
        case CanvasManagerMode::ExcitersEdition :
            setMode(GraphicSessionMode::ExcitersEdition);
            break;
        case CanvasManagerMode::ExciterSelected :
            setMode(GraphicSessionMode::ExciterSelected);
            break;
            
            
        case CanvasManagerMode::WaitingForPointCreation :
            setMode(GraphicSessionMode::WaitingForPointCreation);
            break;
        case CanvasManagerMode::WaitingForPointDeletion :
            setMode(GraphicSessionMode::WaitingForPointDeletion);
            break;
            
        default :
            break;
    }
}





// ===== EVENTS FROM THE PRESENTER ITSELF =====

void GraphicSessionManager::OnEnterSpatScenesEdition()
{
    auto tempStates = spatInterpolator->GetStates();
    sceneEditionComponent->UpdateStatesList(tempStates);
    
    // Forced update of canvases
    for (size_t i=0 ; i<canvasManagers.size() ; i++)
        canvasManagers[i]->OnResized();
    // To update menus related to the selected area
    if (auto selectedArea = getSelectedCanvasAsEditable()->GetSelectedArea())
        getSelectedCanvasAsEditable()->SetSelectedArea(selectedArea);
}
std::shared_ptr<bptree::ptree> GraphicSessionManager::OnLeaveSpatScenesEdition()
{
    // Back to "normal" mode. Real use :
    // ensure that the play mode will be disabled
    // ! We send the order through the Canvas !
    if (selectedCanvas)
        selectedCanvas->SetMode(CanvasManagerMode::SceneOnlySelected);
    
    // Save to XML (Presenter does it)
    return GetCanvasesTree();
}

void GraphicSessionManager::HandleEventSync(std::shared_ptr<GraphicEvent> event_)
{
    // Multi-event : redispatched
    if (auto multiAreaE = std::dynamic_pointer_cast<MultiAreaEvent>(event_))
    {
        // For all other events : simple send if not recursively a multi-area one
        for (size_t i=0 ; i < multiAreaE->GetOtherEventsCount() ; i++)
        {
            if (auto multiAreaE_internal
                = std::dynamic_pointer_cast<MultiAreaEvent>(multiAreaE->GetOtherEvent(i)))
                HandleEventSync(multiAreaE_internal);
            else
                handleSingleEventSync(multiAreaE->GetOtherEvent(i));
        }
        // The main event has to be copy-reconstructed and downcasted
        auto downcastedCopiedAreaEvent = std::make_shared<AreaEvent>( (AreaEvent*)(multiAreaE.get()) );
        handleSingleEventSync(downcastedCopiedAreaEvent);
    }
    // Else, it is a single event...
    else
        handleSingleEventSync(event_);
}

void GraphicSessionManager::handleSingleEventSync(std::shared_ptr<GraphicEvent> event_)
{
#ifdef __MIAM_DEBUG
    if (auto multiAreaE = std::dynamic_pointer_cast<MultiAreaEvent>(event_))
    {
        // No internal redispatching at the moment
        assert(false);
    }
#endif
    if (std::shared_ptr<AreaEvent> areaE = std::dynamic_pointer_cast<AreaEvent>(event_))
    {
        // Event about an Area in particular : we may have to update the spat mix !
        if (auto area = std::dynamic_pointer_cast<ControlArea>(areaE->GetConcernedArea()))
        {
            AsyncParamChange paramChange;
            
            switch (areaE->GetType())
            {
                // On n'envoie l'excitation que pendant l'édition de l'état initial de scène
                case AreaEventType::ExcitementAmountChanged :
                    if (mode == GraphicSessionMode::ExcitersEdition
                        || mode == GraphicSessionMode::ExciterSelected)
                    {
                        // Et on n'envoie que les excitations liées réellement à un état de spat
                        if (area->GetStateIndex() >= 0)
                        {
                            paramChange = buildExcitementParamChange(area);
                            presenter->SendParamChange(paramChange);
                            //std::cout << "excit = " << paramChange.DoubleValue << std::endl;
                        }
                    }
                    break;
                    
                default:
                    break;
            }
        }
    }
    else if (std::shared_ptr<SceneEvent> sceneE = std::dynamic_pointer_cast<SceneEvent>(event_))
    {
        sceneEditionComponent->SetSceneName(sceneE->GetNewScene()->GetName());
    }
}






// ===== EVENTS TO VIEW =====

void GraphicSessionManager::DisplayInfo(String info, int priority)
{
    view->DisplayInfo(info.toStdString(), priority);
}






// = = = = = = = = = = Events from View, transmitted the a canvas  = = = = = = = = = =

void GraphicSessionManager::OnAddScene()
{
    if (selectedCanvas)
        selectedCanvas->AddScene("Scene " + boost::lexical_cast<std::string>(selectedCanvas->GetScenesCount()+1),
                                 true); // ajout avec sélection
    
    else throw std::runtime_error("No canvas selected : cannot add a scene (no canvas should be selected at this point");
}
void GraphicSessionManager::OnDeleteScene()
{
    if (selectedCanvas)
    {
        if(! selectedCanvas->DeleteScene())
            throw std::runtime_error("Cannot delete a scene, only 1 is left (the delete scene button should not have been clicked");
        
    }
    else throw std::runtime_error("No canvas selected : cannot add a scene (no canvas should be selected at this point)");
}
void GraphicSessionManager::OnSceneLeft()
{
    getSelectedCanvasAsEditable()->MoveSelectedSceneTowardsFirst();
}
void GraphicSessionManager::OnSceneRight()
{
    getSelectedCanvasAsEditable()->MoveSelectedSceneTowardsLast();
}
void GraphicSessionManager::OnSceneNameChange(std::string _name)
{
    if (mode != GraphicSessionMode::Loaded && mode != GraphicSessionMode::Loading)
    {
        if (_name.length() > 0) // we do not consider empty strings
            getSelectedCanvasAsEditable()->SetSelectedSceneName(_name);
    }
}



void GraphicSessionManager::OnAddPoint()
{
    // Only if the button is not clicked a second time in a row
    if (mode != GraphicSessionMode::WaitingForPointCreation)
    {
        if (selectedCanvas != 0)
            selectedCanvas->SetMode(CanvasManagerMode::WaitingForPointCreation);
        else
            throw std::runtime_error("Cannot add a point to an area because no canvas is currently selected");
    }
    // Else, we cancel the point creation (an area was necessarily selected if we were to destroy something)
    else
        selectedCanvas->SetMode(CanvasManagerMode::AreaSelected);
}
void GraphicSessionManager::OnDeletePoint()
{
    // Only if the button is not clicked a second time in a row
    if (mode != GraphicSessionMode::WaitingForPointDeletion)
    {
        if (selectedCanvas != 0)
            selectedCanvas->SetMode(CanvasManagerMode::WaitingForPointDeletion);
        else
            throw std::runtime_error("Cannot add a point to an area because no canvas is currently selected");
    }
    // Else, we cancel the point deletion (an area was necessarily selected if we were to destroy something)
    else
        selectedCanvas->SetMode(CanvasManagerMode::AreaSelected);
}
void GraphicSessionManager::OnAddArea(int areaType)
{
    if (selectedCanvas)
    {
        // Information needed
        float ratio = selectedCanvas->GetMultiSceneCanvasComponent()->GetCanvas()->GetRatio();
        int polygonPointsCount;
        if (areaType >= AreaDefaultType::Polygon)
            polygonPointsCount = areaType;
        else
            throw std::logic_error("Cannot add something else than polygons at the moment");
        // Actual addition here
        bpt centerPoint(0.5, 0.5);
        auto spatPolygon = std::make_shared<ControlPolygon>(GetNextAreaId(), centerPoint, polygonPointsCount, 0.15f, Colours::grey, ratio);
        getSelectedCanvasAsEditable()->AddArea(spatPolygon);
        selectedCanvas->CallRepaint();
    }
    else
        throw std::runtime_error("Cannot add a new area : no canvas selected.");
}
void GraphicSessionManager::OnDeleteArea()
{
    if (selectedCanvas)
    {
        getSelectedCanvasAsEditable()->DeleteSelectedArea();
    }
    else
        throw std::runtime_error("No canvas selected. Delete Area button should not be clickable at the moment !");
}

void GraphicSessionManager::OnNewColour(Colour colour)
{
    if (selectedCanvas)
        getSelectedCanvasAsEditable()->OnNewColour(colour);
    else throw std::runtime_error("Cannot set a new colour : no canvas selected");
}


void GraphicSessionManager::OnSendToBack()
{
    if (selectedCanvas)
        getSelectedCanvasAsEditable()->OnSendToBack();
    else throw std::runtime_error("Cannot send something to back : no canvas selected");
}

void GraphicSessionManager::OnSendBackward()
{
    if (selectedCanvas)
        getSelectedCanvasAsEditable()->OnSendBackward();
    else throw std::runtime_error("Cannot send something backward : no canvas selected");
}
void GraphicSessionManager::OnBringForward()
{
    if (selectedCanvas)
        getSelectedCanvasAsEditable()->OnBringForward();
    else throw std::runtime_error("Cannot bring something forward : no canvas selected");
}
void GraphicSessionManager::OnBringToFront()
{
    if (selectedCanvas)
        getSelectedCanvasAsEditable()->OnBringToFront();
    else throw std::runtime_error("Cannot bring something to front : no canvas selected");
}
void GraphicSessionManager::OnAddExciter()
{
    if (selectedCanvas)
        getSelectedCanvasAsEditable()->OnAddExciter();
    else throw std::runtime_error("Cannot send 'add exciter' command to a canvas (none selected)");
}
void GraphicSessionManager::OnDeleteExciter()
{
    if (selectedCanvas)
        getSelectedCanvasAsEditable()->OnDeleteExciter();
    else throw std::runtime_error("Cannot send 'delete exciter' command to a canvas (none selected)");
}



// = = = = = = = = = = Events from View, internally processed  = = = = = = = = = =

void GraphicSessionManager::OnCopyArea()
{
    if(selectedCanvas)
    {
        auto localAreaToCopy = getSelectedCanvasAsEditable()->GetSelectedArea();
        if (localAreaToCopy)
            areaToCopy = localAreaToCopy;
        else
            throw std::runtime_error("Cannot copy an area... No area selected in SceneCanvasComponent::Id" + boost::lexical_cast<std::string>(selectedCanvas->GetId()));
    }
    else
        throw std::runtime_error("Cannot copy an area if no canvas is selected...");
    
    // Graphical Update
    sceneEditionComponent->SetPasteEnabled(true);
}
void GraphicSessionManager::OnPasteArea()
{
    if (selectedCanvas)
    {
        if (areaToCopy)
        {
            // On va forcer l'appel au constructeur de copie
            // Création du shared_ptr directe (c'est seulement à partir de là
            // que l'on pourra appeler shared_from_this() ) :
            std::shared_ptr<IDrawableArea> newDrawbleArea = areaToCopy->Clone();
            std::shared_ptr<ControlArea> newArea;
            
            // If cast does not work...
            if (!(newArea = std::dynamic_pointer_cast<ControlArea>(newDrawbleArea)))
                throw std::runtime_error("Area to copy canot be casted to a spat type");
            
            // Puis : même procédure pour les cas possibles
            // Modification du polygone copié
            newArea->SetId(GetNextAreaId());
            
            // BESOIN DE SAVOIR SI ON CHANGE DE CANVAS OU NON ? obsolète avec le canevas unique...
            // Si on change, besoin d'appeler une fonction du genre :
            // RescaleForCanvas(SceneCanvasComponent* )
            
            // Juste translation par rapport à l'original (si possible) (should be in View...)
            float canvasH = (float)selectedCanvas->GetMultiSceneCanvasComponent()->GetCanvas()->getHeight();
            float canvasW = (float)selectedCanvas->GetMultiSceneCanvasComponent()->GetCanvas()->getWidth();
            newArea->Translate(Point<double>( (newArea->GetRight() + 20 > canvasW ) ? -20 : +20 ,
                                              (newArea->GetBottom() + 20 > canvasH ) ? -20 : +20 ));
            
            getSelectedCanvasAsEditable()->AddArea(newArea);
            getSelectedCanvasAsEditable()->SetSelectedArea(newArea);
            
            // Graphical Update
            selectedCanvas->CallRepaint();
        }
        else
            throw std::runtime_error("No valid area can be pasted from the clipboard.");
    }
    else
        throw std::logic_error("Cannot paste an area : no canvas selected. Paste button should not be clickable at the moment.");
}

void GraphicSessionManager::OnSpatStateChanged(int spatStateIdx)
{
    // We just don't process the "void" combo box callback for now
    if (spatStateIdx < 0)
        return;
    if (auto selectedArea = GetSelectedArea())
    {
        // No check on index (view data is supposed to be up-to-date)
        selectedArea->LinkToState(spatInterpolator->GetState((size_t)spatStateIdx));
        // Forced unoptimized update of everything
        getSelectedCanvasAsEditable()->OnResized();
    }
}


void GraphicSessionManager::OnEnterExcitersEdition()
{
    if (selectedCanvas)
        selectedCanvas->SetMode(CanvasManagerMode::ExcitersEdition);
    else
        throw std::logic_error("Cannot switch to 'Exciters Edition mode' because no canvas is currently selected");
}
void GraphicSessionManager::OnQuitExcitersEdition()
{
    if (selectedCanvas)
        selectedCanvas->SetMode(CanvasManagerMode::SceneOnlySelected);
    else
        throw std::logic_error("Cannot quit 'Exciters Edition mode' because no canvas is currently selected");
}


