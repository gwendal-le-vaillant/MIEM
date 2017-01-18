/*
  ==============================================================================

    IGraphicSessionManager.cpp
    Created: 21 Nov 2016 12:10:44pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include <cmath>

#include "IGraphicSessionManager.h"
#include "IPresenter.h"


using namespace Miam;



IGraphicSessionManager::IGraphicSessionManager(IPresenter* presenter_) :
    multiCanvasComponent(new MultiCanvasComponent(this)), // first init of shared_ptr
    frameTriggerComponent(new FrameTriggerComponent(this))
{
    presenter = presenter_;
    
    frameTriggerComponent->setFramesPerSecond(50);
}




IGraphicSessionManager::~IGraphicSessionManager()
{
    delete multiCanvasComponent;
    
    for(size_t i=0 ; i<canvasManagers.size(); i++)
        delete canvasManagers[i];
}





// = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =

uint64_t IGraphicSessionManager::GetNextAreaId()
{
    uint64_t areaIdBackup = nextAreaId;
    nextAreaId++;
    return areaIdBackup;
}


// = = = = = = = = = = METHODS = = = = = = = = = =

// - - - - - Painting (frame triggering) events - - - - -

/// \brief Launches the computation (maybe a repaint) of a new frame. To be called
/// back from the Miam::FrameTriggerComponent, OR from the OpenGL
/// "clock" synced swap function (********************TO BE DEFINED******************)
///
/// If OpenGL does not master the repainting frequency, this launches
/// the update and painting of a new frame.
void IGraphicSessionManager::OnFrameTrigger()
{
    // We trigger repaint only if OpenGL is not the master
    if (!IsOpenGlSwapSynced())
    {
        CallRepaint();
    }
}
void IGraphicSessionManager::CallRepaint()
{
    for (size_t i = 0 ; i<canvasManagers.size() ; i++)
    {
        canvasManagers[i]->CallRepaint(false);
    }
}



// - - - - - Mouse Events - - - - -
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

