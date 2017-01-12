/*
  ==============================================================================

    SceneEvent.h
    Created: 10 Jan 2017 4:11:07pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef SCENEEVENT_H_INCLUDED
#define SCENEEVENT_H_INCLUDED



#include <memory>


#include "GraphicEvent.h"

#include "EditableScene.h"


namespace Miam
{
    
    
    enum class SceneEventType {
        
        /// None,
        
        NothingHappened, // not used ?
        SceneChanged,
        
    };
    
    
    
    class SceneEvent : public GraphicEvent
    {
        protected :
        SceneEventType eventType = SceneEventType::SceneChanged;
        MultiSceneCanvasInteractor* canvasManager;
        std::shared_ptr<EditableScene> oldScene;
        std::shared_ptr<EditableScene> newScene;
        
        public :
        SceneEventType GetType() {return eventType;}
        
        MultiSceneCanvasInteractor* GetParentCanvas() {return canvasManager;}
        std::shared_ptr<EditableScene> GetOldScene() {return oldScene;}
        std::shared_ptr<EditableScene> GetNewScene() {return newScene;}
        
        // Construction and Destruction
        SceneEvent(MultiSceneCanvasInteractor* canvasManager_, std::shared_ptr<EditableScene> oldScene_, std::shared_ptr<EditableScene> newScene_);
        virtual ~SceneEvent() {}
        
    };
}


#endif  // SCENEEVENT_H_INCLUDED