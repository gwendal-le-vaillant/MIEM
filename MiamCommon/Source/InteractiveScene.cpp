/*
  ==============================================================================

    InteractiveScene.cpp
    Created: 17 Nov 2016 4:53:59pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "InteractiveScene.h"

#include "DrawableArea.h"

#include "MultiSceneCanvasInteractor.h"
#include "InteractiveArea.h"
#include "Exciter.h"

#include "MultiAreaEvent.h"

#ifdef _MSC_VER
	#include "Windows.h" // for OutputDebugString(), std cout does not work...
#endif

using namespace Miam;


// - - - - - Construction and Destruction (and helpers) - - - - -

InteractiveScene::InteractiveScene(std::shared_ptr<MultiSceneCanvasInteractor> canvasManager_, SceneCanvasComponent* canvasComponent_, ExcitersBehaviorType excitersBehavior_)
:
canvasManager(canvasManager_),
canvasComponent(canvasComponent_),
excitersBehavior(excitersBehavior_)
{
    name = "Default Scene";
    
#ifdef __MIEM_VBO
    this->startTimerHz(24);
#endif
}


InteractiveScene::~InteractiveScene()
{
    
}






// = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =

size_t InteractiveScene::GetDrawableObjectsCount()
{
    return areas.size() + currentExciters.size();
}
std::shared_ptr<IDrawableArea> InteractiveScene::GetDrawableObject(size_t i)
{
    if (i < areas.size())
        return areas[i];
    else
        return currentExciters[i-areas.size()];
}


size_t InteractiveScene::GetInteractiveAreasCount()
{
    return areas.size();
}
std::shared_ptr<IInteractiveArea> InteractiveScene::GetInteractiveArea(size_t i)
{
    return areas[i];
}

std::shared_ptr<MultiAreaEvent> InteractiveScene::setSelectedExciter(std::shared_ptr<Exciter> exciterToSelect)
{
    auto areaE = std::make_shared<MultiAreaEvent>();
    // Désélection si nécessaire
    if (selectedExciter)
    {
        areaE = std::make_shared<MultiAreaEvent>(selectedExciter, AreaEventType::Unselected, shared_from_this());
        // On ne change l'opacité que s'il n'était pas libre
        selectedExciter->SetOpacityMode(OpacityMode::Independent);
        selectedExciter = nullptr;
    }
    // Sélection du nouvel excitateur
    if (exciterToSelect)
    {
        selectedExciter = exciterToSelect;
        selectedExciter->SetOpacityMode(OpacityMode::High);
        // Si rien ne s'est encore passé : on re-crée un event simple
        if (areaE->GetType() == AreaEventType::NothingHappened)
            areaE = std::make_shared<MultiAreaEvent>(selectedExciter, AreaEventType::Selected, shared_from_this());
        // Sinon, on crée un nouveau multi area event
        else
        {
            auto multiAreaE_temp = std::make_shared<MultiAreaEvent>(selectedExciter, AreaEventType::Selected, shared_from_this());
            multiAreaE_temp->AddAreaEvent(areaE); // areaE en tant que AreaEvent simple : uniquement partie principale sera traitée par la suite
            areaE = multiAreaE_temp; // échange, les shared_ptr gardent les 2 events proprement
        }
    }
    
    return areaE;
}

void InteractiveScene::SetName(std::string _name)
{
    name = _name;
    
    // graphic updates called from parent (all scenes at the same time...)
}

void InteractiveScene::SetExcitersOpacityMode(OpacityMode opacityMode)
{
    for (size_t i=0 ; i < currentExciters.size() ; i++)
        currentExciters[i]->SetOpacityMode(opacityMode);
}
void InteractiveScene::SetAreasOpacityMode(OpacityMode opacityMode)
{
    for (size_t i=0 ; i < areas.size() ; i++)
        areas[i]->SetOpacityMode(opacityMode);
}



// = = = = = = = = = = METHODS = = = = = = = = = =




// - - - - - Areas Managing : Add and Delete - - - - -


std::shared_ptr<AreaEvent> InteractiveScene::AddArea(std::shared_ptr<IInteractiveArea> newArea)
{
    areas.push_back(newArea);
    
    // Forced graphical updates
    newArea->CanvasResized(canvasComponent);

#if defined(__MIEM_VBO)
    updateAreasZoffset({});
#endif
    
    return std::make_shared<AreaEvent>(newArea, AreaEventType::Added, (int)areas.size()-1, shared_from_this());
}


// - - - - - Areas managing : graphic helpers - - - - -
void InteractiveScene::timerCallback()
{
    // empty event to start with
    std::shared_ptr<MultiAreaEvent> multiAreaE = std::make_shared<MultiAreaEvent>(nullptr, AreaEventType::NothingHappened, nullptr);
    
    std::chrono::time_point<SteadyClock> timePoint = SteadyClock::now();
    // Refresh of all exciters
    for (auto& exciter : currentExciters)
    {
        auto eventType = exciter->UpdateDynamicBrightness(timePoint);
        multiAreaE->AddAreaEvent(std::make_shared<AreaEvent>(exciter, eventType, shared_from_this()));
    }
    
    // Multi-area event sent back to the canvas manager
    auto lockedCanvasManager = canvasManager.lock();
    if (lockedCanvasManager)
        lockedCanvasManager->handleAndSendEventSync(multiAreaE);
    else
        assert(false); // the canvas manager should be lockable at this point...
}


// - - - - - Areas Z-order helper functions - - - - -
GLfloat InteractiveScene::computeAreaZ(size_t areaIndex)
{
    return (GLfloat)(areas.size()) - (GLfloat)areaIndex;
}
GLfloat InteractiveScene::computeExciterZ(size_t exciterIndex)
{
    return - 1.0f - (GLfloat)exciterIndex;
}
void InteractiveScene::updateAreasZoffset(std::initializer_list<size_t> areasIndexes)
{
#ifdef __MIAM_DEBUG
    // If assert occured : please carefully increase the OpenGL camera "far" and "near" parameters
    assert((areas.size() + currentExciters.size()) < 1024); // or we would risk Z-issues
#endif
    if (areasIndexes.size() == 0)
    {
        for (size_t i=0 ; i<areas.size() ; i++)
            areas[i]->setZoffset(computeAreaZ(i));
        for (size_t i=0 ; i<currentExciters.size() ; i++)
            currentExciters[i]->setZoffset(computeExciterZ(i));
    }
    for (size_t i : areasIndexes)
        areas[i]->setZoffset(computeAreaZ(i));
}




// - - - - - Exciters Managing - - - - -

std::shared_ptr<AreaEvent> InteractiveScene::AddDefaultExciter()
{
    auto canvasManagerLocked = canvasManager.lock();
    if (! canvasManagerLocked)
        throw std::logic_error("Cannot add a new current exciter : cannot get a Unique ID from the canvas manager (not linked to this)");
        
    auto exciter = std::make_shared<Exciter>(canvasManagerLocked->GetNextAreaId(),
                                             canvasManagerLocked->GetCommonTimePoint(),
                                             Exciter::AdditionnalGrabRadius::Medium);
    return AddExciter(exciter);
}
std::shared_ptr<AreaEvent> InteractiveScene::AddExciter(std::shared_ptr<Exciter> newExciter, bool forceSelect)
{
    if (auto canvasManagerLocked = canvasManager.lock())
        newExciter->SetInterpolationType(canvasManagerLocked->GetInterpolatorType());
    else
        assert(0); // on devrait toujours pouvoir acquérir un canvasmanager à ce stade...
    currentExciters.push_back(newExciter);
    
    // Forced graphical updates
    newExciter->CanvasResized(canvasComponent);
#if defined(__MIEM_VBO)
    updateAreasZoffset({});
#endif
    
    // WARNING
    // The vector index is the one relative to all drawable objects....
    auto multiAreaE = std::make_shared<MultiAreaEvent>(newExciter, AreaEventType::Added, (int) getExciterDrawingIndex(currentExciters.size()-1), shared_from_this());
    
    // !!! Sélection forcée, si demandée seulement
    // Demandée automatiquement via paramètre par défaut
    if (forceSelect)
        multiAreaE->AddAreaEvent( setSelectedExciter(newExciter) );
    
    return multiAreaE;
}

std::shared_ptr<AreaEvent> InteractiveScene::DeleteCurrentExciterByIndex(size_t excitersVectorIndex)
{
    auto deletedExciter = currentExciters[excitersVectorIndex];
    
    // petite optimisation : on utilise pop_back sur le vector (censé pas trop le modifier...) si
    // on demande d'enlever le dernier élément
    if ( excitersVectorIndex == currentExciters.size() - 1)
        currentExciters.pop_back();
    // Sinon si c'est un élément au milieu on fait sans rechigner... tout petit vecteur en vrai
    else
        currentExciters.erase(currentExciters.begin() + excitersVectorIndex);
    
#if defined(__MIEM_VBO)
    updateAreasZoffset({});
#endif
    
    // We force the unlink for the new excitements to be applied now (and not after
    // all graphic updates are done)
    auto multiAreaE = std::make_shared<MultiAreaEvent>();
    for (size_t i = 0; i < areas.size() ; i++)
        multiAreaE->AddAreaEvent(areas[i]->UpdateInteraction(deletedExciter, true)); // forced disable
    // Addition of the deletion event
    multiAreaE->AddAreaEvent(std::make_shared<AreaEvent>(deletedExciter, AreaEventType::Deleted, shared_from_this()));
    
    // final return
    return multiAreaE;
}
std::shared_ptr<AreaEvent> InteractiveScene::DeleteSelectedExciter()
{
    auto areaE = std::make_shared<AreaEvent>();
    
    // Recherche inverse de l'index, et dé-sélection et suppression directement
    for (size_t i=0 ; i<currentExciters.size() && selectedExciter ; i++)
    {
        if (currentExciters[i] == selectedExciter)
        {
            std::shared_ptr<MultiAreaEvent> multiAreaE_temp = setSelectedExciter(nullptr);
            multiAreaE_temp->AddAreaEvent( DeleteCurrentExciterByIndex(i) );
            areaE = multiAreaE_temp;
        }
    }
    
    return areaE;
}

std::shared_ptr<MultiAreaEvent> InteractiveScene::ResetCurrentExcitersToInitialExciters()
{
    auto multiAreaE = setSelectedExciter(nullptr);
    
    // Création des évènement de suppression des excitateurs courants
    // qui vont être supprimés
    while ( currentExciters.size() > 0 )
    {
        multiAreaE->AddAreaEvent(DeleteCurrentExciterByIndex(currentExciters.size()-1));
    }
    // Duplication des excitateurs initiaux
    for (size_t i = 0 ; i<initialExciters.size() ; i++)
    {
        // Et donc : le volume est cloné lui aussi
        std::shared_ptr<IDrawableArea> clonedCopy = initialExciters[i]->Clone();
        std::shared_ptr<Exciter> exciter;
        if ( ! (exciter = std::dynamic_pointer_cast<Exciter>(clonedCopy)) )
            throw std::logic_error("Cloned exciter cannot be dynamically casted to a Miam::Exciter...");
        
        // Ajout propre du nouvel élément
        auto canvasManagerLocked = canvasManager.lock();
        if (! canvasManagerLocked)
            throw std::logic_error("Cannot add a new current exciter : cannot get a Unique ID from the canvas manager (not linked to this)");
        exciter->SetId(canvasManagerLocked->GetNextAreaId());
        auto areaAddedE = AddExciter(exciter);
        
        // Puis gestion de l'évènement : dans tous les cas on ajoute
        // (peu importe que l'évènement principal du multievent soit NothingHappened, ou non
        multiAreaE->AddAreaEvent(areaAddedE);
    }

    return multiAreaE;
}

void InteractiveScene::SaveCurrentExcitersToInitialExciters(bool deleteCurrentExciters)
{
    // On veut créer une sauvegarde à un instant figé dans le temps
    // donc on fait une copie des excitateurs, et pas seulement
    // des pointeurs sur les excitateurs
    initialExciters.clear();
    for (size_t i = 0 ; i<currentExciters.size() ; i++)
    {
        std::shared_ptr<IDrawableArea> clonedCopy = currentExciters[i]->Clone();
        std::shared_ptr<Exciter> exciter;
        if (! (exciter = std::dynamic_pointer_cast<Exciter>(clonedCopy)) )
            throw std::logic_error("Cloned exciter cannot be dynamically casted to a Miam::Exciter...");
        
        initialExciters.push_back( exciter );
    }
    
    // Sans notification....
    if (deleteCurrentExciters)
        currentExciters.clear();
    
    // Aucun évènement renvoyé
}


// - - - - - Selection events managing (orders from parent manager) - - - - -
std::shared_ptr<MultiAreaEvent> InteractiveScene::OnSelection(bool resetExciters)
{
    auto multiAreaE = std::make_shared<MultiAreaEvent>();
    
    if (resetExciters)
        multiAreaE = ResetCurrentExcitersToInitialExciters();

    // On actualise l'influence des excitateurs, dans TOUS les modes de jeu !
    //if (canvasManager.lock()->GetMode() == CanvasManagerMode::PlayingWithExciters)
    {
        // à l'avenir : transitions douces par Timers !!
        // à l'avenir : transitions douces par Timers !!
        // à l'avenir : transitions douces par Timers !!
        // à l'avenir : transitions douces par Timers !!
        // à l'avenir : transitions douces par Timers !!
        // à l'avenir : transitions douces par Timers !!
        // à l'avenir : transitions douces par Timers !!
        for (auto& exciter : currentExciters)
            exciter->SetVolume(1.0);
        
        // Pour l'instant tout dans une fonction bête et méchante, séparée
        // On remplace bien sauvagement les évènements créés précédemmet !
        // L'idée c'est qu'on oublie les events graphiques à la transition entre scènes...
        multiAreaE = RecomputeAreaExciterInteractions();
    }
    
    return multiAreaE;
}
std::shared_ptr<MultiAreaEvent> InteractiveScene::OnUnselection(bool shutExcitersDown)
{
    // D'abord on arrête les manipulations graphiques
    auto multiAreaE = StopCurrentTransformations();
    
    // Puis on éteint les excitateurs
    // Puis on éteint les excitateurs
    // Puis on éteint les excitateurs
    // Puis on éteint les excitateurs
    // Puis on éteint les excitateurs
    // Puis on éteint les excitateurs (EXCITATION AVEC VOLUME)
    if (shutExcitersDown)
    {
        for (size_t i=0 ; i<currentExciters.size() ; i++)
            currentExciters[i]->SetVolume(0.0);
        multiAreaE->AddAreaEvent(RecomputeAreaExciterInteractions());
    }
    
    // à l'avenir : transitions douces par Timers !!
    // à l'avenir : transitions douces par Timers !!
    // à l'avenir : transitions douces par Timers !!
    // à l'avenir : transitions douces par Timers !!
    // à l'avenir : transitions douces par Timers !!
    // à l'avenir : transitions douces par Timers !!
    // à l'avenir : transitions douces par Timers !!
    
    return multiAreaE;
}



// - - - - - Canvas (mouse) events managing - - - - -

std::shared_ptr<GraphicEvent> InteractiveScene::OnCanvasMouseDown(const MouseEvent& unconstrainedMouseE)
{
    // UN-constrained for collision detection
    auto mouseE = unconstrainedMouseE; // = constrainMouseEvent(unconstrainedMouseE);
    
	// default : empty AREA event
    auto graphicE = std::make_shared<AreaEvent>();

    switch(excitersBehavior)
    {
        case ExcitersBehaviorType::AppearOnTouch :
            throw std::runtime_error("Exciter appearing on touch not implemented");
            break;
            
        case ExcitersBehaviorType::ManualAddAndDelete :
        case ExcitersBehaviorType::ManualAddAndDelete_AutoDisappear :
        {
            // Security : if the touch source ID already exists... What do we do ?
            
            // In this case, exciters already exist. We try to move one that is not
            // already linked to a touch source

			// While no exciter is selected : we look for a new one to select,
			// starting from the area on the upper layer (last draw on canvas)
            auto eventType = AreaEventType::NothingHappened;
            for (int i=(int)currentExciters.size()-1; (i>=0)&&(eventType != AreaEventType::PointDragBegins) ; i--)
            {
                // Function will check first if the area is not already dragged
                eventType = currentExciters[i]->TryBeginPointMove(mouseE.position.toDouble());
                if (eventType == AreaEventType::PointDragBegins)
                {
                    // Indicates the the move can begin
                    touchSourceToEditableArea[mouseE.source.getIndex()] = currentExciters[i];
					graphicE = std::make_shared<AreaEvent>(currentExciters[i], eventType, shared_from_this());
                    
                    // Tracking for constrained applications (__MIEM_EXPERIMENTS)
                    beginTouchConstraint(mouseE,
                                         canvasComponent->getWidth(), canvasComponent->getHeight(),
                                         currentExciters[i]->GetCenterInPixels(),
                                         name);
                    
                    // Le dernier excitateur choisi devient l'excitateur sélectionné !
                    auto multiAreaE_temp = setSelectedExciter(currentExciters[i]);
                    multiAreaE_temp->AddAreaEvent(graphicE);
                    graphicE = multiAreaE_temp;
                }
            }
            
            // Si rien n'a été sélectionné à cet endroit : dé-sélecion de l'excitateur
            if (eventType == AreaEventType::NothingHappened)
                graphicE = setSelectedExciter(nullptr);
            
            break;
        }
            
        default :
            break;
    }
    
	return graphicE;
}
std::shared_ptr<GraphicEvent> InteractiveScene::OnCanvasMouseDrag(const MouseEvent& unconstrainedMouseE)
{
    // bypass-function if __MIEM_EXPERIMENTS is not defined
    auto mouseE = constrainMouseEvent(unconstrainedMouseE,
                                      canvasComponent->getWidth(), canvasComponent->getHeight());
    
    // Default empty event...
    auto graphicE = std::make_shared<GraphicEvent>();
    
    int touchIndex = mouseE.source.getIndex();
    auto mapIt = touchSourceToEditableArea.find(touchIndex);
    // If the touch is related to an area being moved
    if (mapIt != touchSourceToEditableArea.end())
    {
        auto exciter = std::dynamic_pointer_cast<Exciter>(mapIt->second);
        if (! exciter)
            throw std::logic_error("An interactive scene can handle Miam::Exciters only");
        
        // Création de l'évènement de l'excitateur seul, pour renvoi (dans un multi event, dans le doute)
        AreaEventType eventType = exciter->TryMovePoint(mouseE.position.toDouble());
        auto simpleAreaE = std::make_shared<AreaEvent>(exciter, eventType, shared_from_this());
        
        // Test sur toutes les aires
        auto multiAreaE = testAreasInteractionsWithExciter(exciter);
        multiAreaE->AddAreaEvent(simpleAreaE); // excitateur lui-même

        graphicE = multiAreaE;
    }
    
    return graphicE;
}
std::shared_ptr<GraphicEvent> InteractiveScene::OnCanvasMouseUp(const MouseEvent& unconstrainedMouseE)
{
    /// UN-constrained
    // (((bypass-function if __MIEM_EXPERIMENTS is not defined)))
    auto mouseE = unconstrainedMouseE; // constrainMouseEvent(unconstrainedMouseE);
    // and constraint is removed
    endTouchConstraint(mouseE);
    
	std::shared_ptr<GraphicEvent> graphicE(new GraphicEvent());

    switch(excitersBehavior)
    {
        case ExcitersBehaviorType::AppearOnTouch :
            throw std::runtime_error("Exciter appearing on touch not implemented");
            break;
            
        case ExcitersBehaviorType::ManualAddAndDelete :
        case ExcitersBehaviorType::ManualAddAndDelete_AutoDisappear :
        // We end the move, if the touch was actually related to an area
        {
            int touchIndex = mouseE.source.getIndex();
            // May not always exist (suddens touch state reinit on scene change, ...)
            auto mapIt = touchSourceToEditableArea.find(touchIndex);
            if (mapIt != touchSourceToEditableArea.end())
            {
                mapIt->second->EndPointMove();
                touchSourceToEditableArea.erase(mapIt);
				// no event : exciter remains alive
				//graphicE = 
            }
            break;
        }
            
        default :
            break;
    }
    return graphicE;
}

std::shared_ptr<MultiAreaEvent> InteractiveScene::StopCurrentTransformations()
{
    auto multiAreaE = std::make_shared<MultiAreaEvent>();
    
    // We stop all current movements
    // and filter all future undesired touch events
    for (auto it = touchSourceToEditableArea.begin();
         it != touchSourceToEditableArea.end();)
    {
        // filtering at first,
        std::shared_ptr<IEditableArea> editableArea = it->second;
        it = touchSourceToEditableArea.erase(it); // increments to next valid
        // Actual stop after
        AreaEventType eventType = editableArea->EndPointMove();
        
        // Storage of event in vector (for events back sending)
        multiAreaE->AddAreaEvent(new AreaEvent(editableArea, eventType, shared_from_this()));
        
        // constraints (MIEM EXPERIMENTS)
        clearConstraints();
    }
    return multiAreaE;
}



// = = = = = = = = = = Quantification, gestion des interactions = = = = = = = = = =

std::shared_ptr<MultiAreaEvent> InteractiveScene::testAreasInteractionsWithExciter(std::shared_ptr<Exciter>& exciter)
{
    auto multiAreaE = std::make_shared<MultiAreaEvent>(); // event de base restera "NothingHappened"...
    
    // PASSE 1
    for (size_t i = 0 ; i < areas.size() ; i++)
    {
        // Si qqchose a changé, on renvoit l'info
        auto areaE = areas[i]->UpdateInteraction(exciter);
        if ( areaE->GetType() == AreaEventType::ExcitementAmountChanged )
        {
            areaE->SetConcernedScene(shared_from_this()); // pas précisé dans l'aire, qui ne connaît pas son parent.
            multiAreaE->AddAreaEvent( areaE );
        }
    }
    // PASSE 2 (à l'intérieur de l'excitateur, car il enregistre les modifs !)
    exciter->NotifyNewExcitationToAreas();
    
    return multiAreaE;
}

std::shared_ptr<MultiAreaEvent> InteractiveScene::RecomputeAreaExciterInteractions()
{
    auto multiAreaE = std::make_shared<MultiAreaEvent>();
    
    for (size_t i=0 ; i<currentExciters.size() ; i++)
        testAreasInteractionsWithExciter(currentExciters[i]);
    
    // Plutôt que de renvoyer des dizaines d'évènements....
    // On recrée juste un évènement pour chaque aire
    for (size_t i=0 ; i<areas.size() ; i++)
        multiAreaE->AddAreaEvent(new AreaEvent(areas[i], AreaEventType::ExcitementAmountChanged, shared_from_this()));
    
    return multiAreaE;
}





// = = = = = = = = = = Pre-Computation of Interaction Data = = = = = = = = = =


void InteractiveScene::PreComputeInteractionData()
{
    if (auto canvasLocked = canvasManager.lock())
        canvasLocked->DisplayInfo("Computing Interaction data...");
    
    auto startTime = std::chrono::steady_clock::now();
    std::cout << "[InteractiveScene.cpp] Starting Pre-Computation of interaction data..." << std::endl;
    
    const size_t canvasW = canvasComponent->getWidth();
    const size_t canvasH = canvasComponent->getHeight();
    
    // Réinitialisation des groupes
    areasGroups.clear();
    AreasGroup::ReinitRandomColours(3); // static call, seed reinit there. 3 is nice.
    // Réinit à l'intérieur des aires -> nécessaire ? Va être fait + tard....
    
    // Initialisation : mise à zéro du tableau des "couleurs" (en réalité : pointeurs)
    std::vector<AreasGroup*> groupsImage;
    // CONVENTION : axe y vers le bas, indice i (numéro de ligne de matrice)
    // axe x vers la droite, indice j (numéro de colonne de matrice)
    groupsImage.resize(canvasW * canvasH, nullptr);
    
    // BOUCLE PRINCIPALE : parcours de tous les pixels, tests d'interaction avec toutes les aires
    // (arrêt dès qu'on en touche une)
    // -> cette boucle ne stocke rien dans les aires (elle ne fait que détecter leur présence)
    // V1 : présence ou non -> OK
    // V2 : nb adjacents -> OK
    // V3 : finale
    bool foundArea;
    std::vector<AreasGroup*> adjacentGroups;
    adjacentGroups.reserve(4); // always 4 reserved: left, topleft, top, topright
    for (size_t i=0 ; i<canvasH ; i++)
    {
        for (size_t j=0 ; j<canvasW ; j++)
        {
            foundArea = false;
            for (size_t k = 0 ; (k<areas.size() && (! foundArea)) ; k++)
                foundArea = areas[k]->HitTest(bpt((double)j, (double)i));
            // Si collision avec une aire : on analyse les voisins pour compléter une aire,
            // ou en fusionner...
            if (foundArea)
            {
                // Décompte du nombre de voisins appartenant déjà à un groupe
                adjacentGroups.clear();
                if (j >= 1)
                {
                    // left pixel
                    if (groupsImage[i*canvasW + (j-1)] != nullptr)
                        adjacentGroups.push_back(groupsImage[i*canvasW + (j-1)]);
                    // top-left pixel
                    if (groupsImage[(i-1)*canvasW + (j-1)] != nullptr)
                        adjacentGroups.push_back(groupsImage[(i-1)*canvasW + (j-1)]);
                }
                if (i >= 1)
                {
                    // top pixel
                    if (groupsImage[(i-1)*canvasW + j] != nullptr)
                        adjacentGroups.push_back(groupsImage[(i-1)*canvasW + j]);
                    // top-right pixel
                    if (j < canvasW)
                        if (groupsImage[(i-1)*canvasW + (j+1)] != nullptr)
                            adjacentGroups.push_back(groupsImage[(i-1)*canvasW + (j+1)]);
                }
                // (3 actions possibles)
                // 1 - création nouveau groupe
                // Il y aura, avant le tri, de très nombreux groupes
                if (adjacentGroups.size() == 0)
                {
                    areasGroups.push_back(std::make_shared<AreasGroup>());
                    groupsImage[i*canvasW + j] = areasGroups.back().get();
                }
                // 2 - insertion dans groupe existant
                else if (adjacentGroups.size() == 1) // METTRE EGAAAAAAAAAAAAAAAAAL
                    groupsImage[i*canvasW + j] = adjacentGroups[0];
                // 3 - insertion dans le premier vu, puis fusion groupes existants
                // (propagation prioritaire par la gauche)
                else
                {
                }
            }
        }
    }
    
    // Assignation des groupes aux aires : chaque aire prend le groupe
    // qui est au niveau de son centre
    
#ifdef __MIEM_DISPLAY_SCENE_PRE_COMPUTATION
    // Construction + Affichage de l'image des groupes dans un fichier .png temporaire
    Image groupsColourImage(Image::PixelFormat::ARGB, (int)canvasW, (int)canvasH, false);
    for (size_t i=0 ; i<canvasH ; i++)
    {
        for (size_t j=0 ; j<canvasW ; j++)
        {
            if (groupsImage[i*canvasW+j] != nullptr)
                groupsColourImage.setPixelAt((int)j, (int)i,
                                             groupsImage[i*canvasW+j]->GetColour());
            else
                groupsColourImage.setPixelAt((int)j, (int)i,
                                             Colours::black);
        }
    }
    { // tentative destruction avant re-lecture
        File pngFile("./Debug_AreasGroups.png");
        if (pngFile.existsAsFile())
            pngFile.deleteFile();
    }
    FileOutputStream stream ( File("./Debug_AreasGroups.png") );
    PNGImageFormat pngWriter;
    bool couldWrite = pngWriter.writeImageToStream(groupsColourImage, stream);
    // The image must be written in Debug mode....
    assert(couldWrite);
#endif
    
    // Affichage du temps total de traitement
    auto processDuration = std::chrono::steady_clock::now() - startTime;
    std::cout << "[InteractiveScene.cpp] Pre-Computation finished. Duration = " <<
    std::chrono::duration_cast<std::chrono::milliseconds>(processDuration).count() << " ms" << std::endl;
}






// = = = = = = = = = = XML import/export = = = = = = = = = =
std::shared_ptr<bptree::ptree> InteractiveScene::GetTree() const
{
    auto sceneTree = std::make_shared<bptree::ptree>();
    sceneTree->put("name", name);
    bptree::ptree areasTree;
    
    // Ajout des aires interactives
    for (size_t i=0; i < areas.size() ; i++)
    {
        auto areaTree = areas[i]->GetTree();
        areaTree->put("<xmlattr>.index", i);
        areasTree.add_child("area", *areaTree);
    }
    // Ajout des excitateurs : on prend les initiaux, pas les actifs ! Choix discutable...
    for (size_t i=0; i < initialExciters.size() ; i++)
    {
        auto areaTree = initialExciters[i]->GetTree();
        areaTree->put("<xmlattr>.index", getExciterDrawingIndex(i));
        areasTree.add_child("area", *areaTree);
    }
    
    sceneTree->add_child("areas", areasTree);
    return sceneTree;
}




