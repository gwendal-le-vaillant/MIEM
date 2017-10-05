/*
  ==============================================================================

    GraphicSpatSessionManager.h
    Created: 13 Jun 2017 9:11:20pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include <memory>

#include "IGraphicSessionManager.h"
#include "SpatPresenter.h"
#include "SpatArea.h"

#include "SpatInterpolator.hpp"

namespace Miam
{
    class GraphicSpatSessionManager : public IGraphicSessionManager
    {
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        
        private :
        SpatPresenter* presenter;
        
        protected :
        
        // For communication with Model
        std::shared_ptr<SpatInterpolator<double>> spatInterpolator;
        
        
        
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        
        public :
        
        // - - - - - Construction/destruction, and helpers - - - - -
        
        /// \brief Construction (the whole Presenter module is built after the View).
        GraphicSpatSessionManager(SpatPresenter* _presenter);
        
        /// \Brief to be called after Model construction (which occurs
        /// after Presenter construction)
        void CompleteInitialisation(std::shared_ptr<SpatInterpolator<double>> _spatInterpolator);
        
        /// \brief Destruction and the editor and the canvases
        virtual ~GraphicSpatSessionManager() {}
        

        
        // - - - - - XML import/export - - - - -
        virtual void SetFromTree(bptree::ptree& graphicSessionTree) override;
        /// \Brief Finishes to XML-load spat attributes of an area, from the inner
        /// content of the <spatstate> tag.
        ///
        /// \param spatStateTree may be a std::nullptr
        virtual void LoadSpatAreaLinks(std::shared_ptr<SpatArea> area, std::shared_ptr<bptree::ptree> spatStateTree);
        
    };
}