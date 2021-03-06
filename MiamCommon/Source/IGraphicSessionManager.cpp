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

#include "IGraphicSessionManager.h"
#include "IPresenter.h"


using namespace Miam;



IGraphicSessionManager::IGraphicSessionManager(IPresenter* presenter_)
:
presenter(presenter_),
multiCanvasComponent(new MultiCanvasComponent(this)),

// À mettre à jour après le chargement d'un nouveau fichier
nextAreaId(0) // plus tard : valeur contenue dans le fichier de sauvegarde

{
}




IGraphicSessionManager::~IGraphicSessionManager()
{
    delete multiCanvasComponent;
}





// = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =

uint64_t IGraphicSessionManager::GetNextAreaId()
{
    uint64_t areaIdBackup = nextAreaId;
    nextAreaId++;
    return areaIdBackup;
}

std::chrono::time_point<std::chrono::steady_clock> IGraphicSessionManager::GetCommonTimePoint() const
{
    return presenter->GetCommonTimePoint();
}

InterpolationType IGraphicSessionManager::GetInterpolatorType() const
{
    return presenter->GetInterpolatorType();
}
SceneConstrainer::ConstraintType IGraphicSessionManager::GetGlobalExcitersConstraint() const
{
    if (! bypassGlobalExcitersConstraint)
        return globalExcitersConstraint;
    else
        return SceneConstrainer::ConstraintType::Bypass;
}

// = = = = = = = = = = METHODS = = = = = = = = = =




// - - - - - - canvases managing - - - - - -
void IGraphicSessionManager::SetSelectedCanvas(std::shared_ptr<MultiSceneCanvasInteractor> selectedCanvas_)
{
    
    // We do something only if there has been a change
    if (selectedCanvas != selectedCanvas_)
    {
        // At first : unselection of previous canvas...
        if (selectedCanvas)
        {
            selectedCanvas->SetMode(CanvasManagerMode::Unselected);
            selectedCanvas->CallRepaint();
        }
        
        selectedCanvas = selectedCanvas_;
        
        selectedCanvas->SetMode(CanvasManagerMode::SceneOnlySelected);
        
        //setMode(GraphicSessionMode::CanvasSelected);
        
        // Ligne ci-dessous pose un soucis de re-dessin à chaque clic sur un excitateur...
        //multiCanvasComponent->resized();
    }
    else
    {
        // rien du tout
    }
}
void IGraphicSessionManager::TriggerInteractionDataPreComputation()
{
    // All computations will be launched now, each canvas will take N-1 CPUs.
    // -> not made for several canvases at the moment... If this is the case,
    // a short overload of the CPUs might happen
    assert(canvasManagers.size() == 1);
    
    // Direct launch of all computation threads....
    for (auto &canvasInteractor : canvasManagers)
        canvasInteractor->TriggerInteractionDataPreComputation();
}

// - - - - - Events from a member of the Presenter module itself - - - - -

void IGraphicSessionManager::CallPresenterUpdate() {presenter->Update();}


// = = = = = = = = = = Mouse Events = = = = = = = = = =

void IGraphicSessionManager::OnBackgroundMouseDown(const MouseEvent &event)
{
    if (event.source.getIndex() == 0) // mouse or first touch only
    {
        if ( abs(event.getPosition().getX() - multiCanvasComponent->GetBarXPos()) < 10 )
        {// 10 px de marge des 2 côtés pour chopper la barre
            mouseResizingCanvas = true;
        }
    }
}
void IGraphicSessionManager::OnBackgroundMouseDrag(const MouseEvent &event)
{
    if (mouseResizingCanvas)
    {
        if (event.source.getIndex() == 0) // mouse or first touch only
        {
            // FOR NOW, A CANVAS HAS 100PX MINIMUM SIZE
            const int xPos = event.getPosition().getX();
            if ( xPos > 116
                && xPos < (multiCanvasComponent->getWidth()-116))
            multiCanvasComponent->SetBarXPos(event.getPosition().getX());
        }
    }
}
void IGraphicSessionManager::OnBackgroundMouseUp(const MouseEvent& /* event */)
{
    mouseResizingCanvas = false;
}



// = = = = = = = = = = XML import/export = = = = = = = = = =
std::shared_ptr<bptree::ptree> IGraphicSessionManager::GetCanvasesTree()
{
    bptree::ptree canvasesInnerTree;
    for (auto &canvasInteractor : canvasManagers)
        canvasesInnerTree.add_child("canvas", *(canvasInteractor->GetTree()));
    auto canvasesTree = std::make_shared<bptree::ptree>();
    // Proper return, all <canvas> tags within a <canvases> tag
    canvasesTree->add_child("canvases", canvasesInnerTree);
    return canvasesTree;
}


std::vector< std::shared_ptr<bptree::ptree> >
IGraphicSessionManager::ExtractCanvasesSubTrees(bptree::ptree& canvasesTree)
{
    bptree::ptree canvasesNode;
    try {
        canvasesNode = canvasesTree.get_child("canvases");
    }
    catch (bptree::ptree_error& e) {
        throw XmlReadException("Inside <graphicsession> node : ", e);
    }
    std::vector< std::shared_ptr<bptree::ptree> > canvasTrees;
    size_t canvasesCount = 0;
    for (auto& canvasChild : canvasesNode)
    {
        if (canvasChild.first != "canvas")
            throw XmlReadException("<canvases> children can be <canvas> tags only.");
        // Si on a bien le bon noeud, on continue :
        canvasTrees.push_back(std::make_shared<bptree::ptree>());
        // nul en terme d'optimisation mais au moins ça marche........
        canvasTrees.back()->add_child("canvas", canvasChild.second);
        //write_xml(std::cout, canvasTrees.back()->get_child("canvas.scenes"));
        // For post-checkings
        canvasesCount++;
    }
    if (canvasesCount != canvasManagers.size())
        throw XmlReadException(boost::lexical_cast<std::string>(canvasesCount) + " <canvas> elements found, but exactly " + boost::lexical_cast<std::string>(canvasManagers.size()) + " are required");
    return canvasTrees;
}


