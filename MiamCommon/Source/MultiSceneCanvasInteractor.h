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

#ifndef MULTISCENECANVASINTERACTOR_H_INCLUDED
#define MULTISCENECANVASINTERACTOR_H_INCLUDED

#include <memory>
#include <vector>
#include <list>

#include <deque>

#include <mutex>

#include "SceneCanvasComponent.h"
#include "MultiSceneCanvasComponent.h" // forward-declare not enough (template fcts)

#include "EditableScene.h"

#include "CanvasManagerMode.h"

#include "EditableArea.h"
#include "EditablePolygon.h"

#include "MiamExceptions.h"
#include "XmlUtils.h"

#include "InterpolationTypes.h"

#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/xml_parser.hpp"
namespace bptree = boost::property_tree;


class MultiSceneCanvasComponent;


namespace Miam {
    // pre-declaration for pointers
    class IGraphicSessionManager;
    class EditableScene;
    
    
    /// \brief Manages the interaction with some Miam::EditableScene
    ///
    ///
    class MultiSceneCanvasInteractor :
        public AsyncUpdater,
        public std::enable_shared_from_this<MultiSceneCanvasInteractor>,
        private juce::Timer // for triggering pre-computations after resizes
    {
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        protected :
        
        
        // Current internal running mode
        CanvasManagerMode mode;
        
        /// \brief Pointer back to the unique edition manager (parent of this manager)
        IGraphicSessionManager* graphicSessionManager = 0;

        /// \brief The associated Juce canvas component
        ///
        /// The pointer was actually extracted from a std::unique_ptr (do not
        /// delete it from inside this class)
        MultiSceneCanvasComponent* canvasComponent;
		// warning : because the scenes might refuse some pre computation
		// orders (if they are already busy precomputing...), these values
		// might not reflect the actual size of computations (and anyway, 
		// the scenes are smaller than the multiscene canvas)
        int lastOnResizeAutoPreComputationWidth = -1;
        int lastOnResizeAutoPreComputationHeight = -1;
        
        
        // The scenes
        std::vector< std::shared_ptr<EditableScene> > scenes;
        
        /// \brief The currently selected scene
        ///
        /// There should always be at least one scene alive, to prevent undefined
        /// behavior
        std::shared_ptr<EditableScene> selectedScene = nullptr;
        
        
        
        // - - - - - Thread-safe attributes (OpenGL async drawing) - - - - -
        
        std::mutex asyncDrawableObjectsMutex;
        // Synchronized with the UNIQUE selected scene's drawable objects
        std::list<std::shared_ptr<IDrawableArea>> asyncDrawableObjects;
        
        /// \brief Maps an original area to its asynchronously drawn copy. Accessed
        /// from the main "presenter (juce) message thread" only.
        ///
        /// Not always protected by mutex locks ! As its elements are shared pointers,
        /// which allows MT-safe access
        ///
        /// The iterator of a list are not invalidated (unless elements are deleted),
        /// so we can use them while is does not modify anything about the whole
        /// container.
        /// Source : http://en.cppreference.com/w/cpp/container#Thread_safety
        ///
        std::map<std::shared_ptr<IDrawableArea>, std::list<std::shared_ptr<IDrawableArea>>::iterator> originalToAsyncObject;
        
        
        // - - - - - Interp data pre-computation. Called from juce message thread - - - - -
        int delayForRecomputeAfterResize_ms = 5000;
        int nbCoresToUse = 1;
        int currentPreComputationBatchStartIdx = -1;
        int currentPreComputationBatchIdx = -1;
        int currentPreComputationBatchSize = 0;
        bool shouldRestartPreComputationBatch = false;
        int numberOfWaitingRestarts = 0; // should never be > 1 !!!
        
        protected :
        SceneCanvasComponent::Id selfId; ///< To be transformed to a name...
        
        
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        
    public:
        MultiSceneCanvasInteractor(IGraphicSessionManager* _graphicSessionManager, MultiSceneCanvasComponent* _canvasComponent, SceneCanvasComponent::Id _selfId);
        
        virtual ~MultiSceneCanvasInteractor();
        
        
        public :
        /// \biref Updates data, refreshes then actually paints what's necessary
        /// (and asked to repaint).
        void CallRepaint();
        
        void RecomputeAreaExciterInteractions();
        
        /// \brief To be called by the child multi scene canvas component
        ///
        /// Works with the internal timer to detect actual end of resize phases (when the OS
        /// wants to show a "smooth" multi-resize to the user)
        void OnComponentResized(int newWidth, int newHeight, Component* childComponent);
        
        // - - - Overrides of juce callbacks (see inherited classes) - - -
        private :
        /// \brief Timer for detection the end of resize phases (when the OS
        /// wants to show a "smooth" multi-resize to the user)
        virtual void timerCallback() override;
        protected :
        void handleAsyncUpdate() override;
        
        
        // ----- Running Mode -----
        public :
        /// \brief Default acceptation of mode change (to be overriden)
        ///
        /// Tries to set the next running mode, then tells the
        /// Miam::graphicSessionManager what happened
        /// The Miam::IGraphicSessionManager might then itself change its mode
        virtual void SetMode(CanvasManagerMode _mode);
        /// \brief Gets the current running mode
        virtual CanvasManagerMode GetMode() {return mode;}
        
        
        
        // ------ Setters and Getters ------
        public :
        
        SceneCanvasComponent::Id GetId() {return selfId;}
        
        MultiSceneCanvasComponent* GetMultiSceneCanvasComponent()
        { return canvasComponent; }
        uint64_t GetNextAreaId();
        
        // Scenes
        virtual size_t GetScenesCount() {return scenes.size();}
        virtual std::shared_ptr<EditableScene> GetScene(size_t i) {return scenes[i];}
        /// \brief Returns the index of the scene if it's actually a scene of this canvas,
        /// or -1 if not.
        int64_t GetSceneIndex(std::shared_ptr<InteractiveScene> sceneToLocate);
        
        /// \brief Mostly performs multiple downcasts from Miam::EditableScene
        /// to Miam::InteractiveScene
        virtual std::vector<std::shared_ptr<InteractiveScene>> GetInteractiveScenes();
        
        // for this to remain master of the modifications applied to it
        //virtual std::shared_ptr<EditableScene> GetSelectedScene() {return selectedScene;}
        
        // à DÉGAGER SI LA MAP DE EDITABLEAREA VERS SON INDICE MARCHE BIEN
        virtual int GetSelectedSceneId();
        
        std::chrono::time_point<std::chrono::steady_clock> GetCommonTimePoint() const;
        
        InterpolationType GetInterpolatorType() const;
        
        SceneConstrainer::ConstraintType GetGlobalExcitersConstraint();
        
        void DisplayInfo(String info, int priority = 0);
        
        
        
        // - - - - - Events notifications (should not happen often...) - - - - -
        public :
        /// \brief Internally dispatches the event and also notifies the GraphicSessionManager
        ///
        /// Copy-constructor of the event called : else, there would be cast
        /// issues
        void handleAndSendEventSync(std::shared_ptr<GraphicEvent> graphicE);
        
        
        // - - - - - Internal events management - - - - -
        protected :
        /// \brief Can be called alone, or from the MultiArea version of this function
        /// (for optimization purposes).
        ///
        /// Should notify the Graphic Session Manager only when directly called (not when called from
        /// the multi-area event version).
        void handleAndSendAreaEventSync(std::shared_ptr<AreaEvent>& areaE, bool notifyGraphicSessionManager = true);
        /// \brief Splits a multi-area event into the sub area events. The main event is also turned into
        /// a single area event.
        ///
        /// Might be called from the AreaEvent-typed function
        void handleAndSendMultiAreaEventSync(std::shared_ptr<MultiAreaEvent>& multiAreaE);
        
        // Peut être override par les classes dérivées
        virtual void processSingleAreaEventSync(std::shared_ptr<AreaEvent>& areaE);
        
        
        // - - - - - Thread-safe methods (for OpenGL async drawing) - - - - -
        
        protected : // internal routines
        void recreateAllAsyncDrawableObjects();
        void addAsyncDrawableObject(int insertionIdInScene, std::shared_ptr<IDrawableArea> originalAreaToAdd);
        void updateAsyncDrawableObject(std::shared_ptr<IDrawableArea> originalAreaToUpdate);
        void deleteAsyncDrawableObject(std::shared_ptr<IDrawableArea> originalAreaToDelete);

        public : // external interfacing
        // 3 following functions, to be used very carefully from the OpenGL renderer !
        void LockAsyncDrawableObjects() { asyncDrawableObjectsMutex.lock(); }
        std::list<std::shared_ptr<IDrawableArea>>& GetAsyncDrawableObjects()
        { return asyncDrawableObjects; }
        void UnlockAsyncDrawableObjects() { asyncDrawableObjectsMutex.unlock(); }
        
        
        // - - - - - Interp data pre-computation. Called from juce message thread - - - - -
        public :
        /// \brief Will trigger the pre-computations for all scenes, using
        /// all CPUs but one.
        void TriggerInteractionDataPreComputation();
        protected :
        /// \brief Function that will self-call itself with a delay, while the current
        /// batch is not finished. When the batch is finished, the function will trigger
        /// the next batch (or no batch if all is finished, and sends the proper event)
        void waitOrTriggerPreComputationBatch();
        void triggerNextPreComputationbatch();
        void onPreComputationFinished();
        
        // - - - - - Selection management (scenes only) - - - - -
        public :
        virtual void SelectScene(int id);
        
  
        
        // ------ Scenes managing : Add and Delete ------
        /// \brief Adds a Miam::EditableScene
        virtual void AddScene(std::string name, bool selectNewScene = false);
        /// \brief Adds a Miam::EditableScene
        virtual void AddScene(std::shared_ptr<EditableScene> newScene, bool selectNewScene = false);
        /// \Returns wether the selected scene has been deleted or not (if it
        /// was the last one).
        virtual bool DeleteScene();
        protected :
        /// \brief Deletes a scene without doing any checks.
        /// Be careful not to force-delete the selected scene !!
        ///
        /// Sends an event about the deletion.
        virtual void forceDeleteScene(size_t sceneIndexToDelete);
        public :
        
        
        
        // ------ areas managing : Add and Delete ------
        
        protected :
        /// \brief Does what it says, and sends an event also
        void addAreaToScene(std::shared_ptr<EditableScene> scene_, std::shared_ptr<IInteractiveArea> area_);
        public :
        /// \brief Adds an area (which can also be an exciter) to a scene referenced
        /// by its index.
        ///
        /// Function that is mostly intended to be used when loading a new session.
        void AddAreaToScene(size_t sceneIndex, std::shared_ptr<IInteractiveArea> area_);
        
        
        // ---------- Exciters management ----------
    
        /// \brief Adds a default initial exciter to the selected scene
        void OnAddExciter();
        /// \brief Tries to delete an initial exciter from the selected scene
        void OnDeleteExciter();
        
        

        // ---------- Events from CanvasComponent (from View) ----------
        public :
        void OnCanvasMouseDown(const MouseEvent& mouseE);
        void OnCanvasMouseDrag(const MouseEvent& mouseE);
        void OnCanvasMouseUp(const MouseEvent& mouseE);
        void OnResized();
        
        
        
        // - - - - - XML import/export - - - - -
        public :
        
        /// \brief Implements final actions after the Miam::IGraphicSessionManager
        /// has finished to call loading functions of this instance.
        virtual void OnXmlLoadFinished();
        
        virtual std::shared_ptr<bptree::ptree> GetTree();
        /// \brief Charge les scènes, vides, depuis un arbre XML (arbre partant du tag
        /// <canvas>). Ne charge pas leur contenu : les aires graphiques concrètes (classes dérivées
        /// de IDrawableArea) seront créées dans
        /// le GraphicSessionManager réel de l'appli.
        ///
        /// \param Le template T sert à préciser le type de scène qui sera
        /// créé : soit du EditableScene (parce que les InteractiveScene
        /// ne sont utilisées toutes seules nulle part), soit n'importe quelle
        /// classe enfant qui en a hérité (AmusingScene, ou autre...)
        ///
        /// \return A vector of sub-trees, each sub-tree being the content
        /// of a given scene (including the <scene> tag)
        template<class T>
        std::vector< std::shared_ptr<bptree::ptree> >
        SetScenesFromTree(bptree::ptree& canvasTree)
        {
            std::vector< std::shared_ptr<bptree::ptree> > sceneTrees; // return values

            // Reinit dans tous les cas
            while (scenes.size() > 0)
                forceDeleteScene(scenes.size()-1);
            
            // Et si l'arbre fourni était vide, c'était une simple commande de réinitialisation
            // Donc on n'exécute le code d'ajout que s'il contient des trucs
            bool sceneFound = false;
            if ( ! canvasTree.empty() )
            {
                size_t indexesCount = XmlUtils::CheckIndexes(canvasTree.get_child("canvas"),
                                                             "scenes", "scene");
                
                std::vector< std::shared_ptr<T> > scenesToAdd;
                scenesToAdd.resize(indexesCount);
                sceneTrees.resize(indexesCount);
                
                // Pre-loading of new scenes (and sub-trees to return) from tree
                for (auto& sceneChild : canvasTree.get_child("canvas.scenes"))
                {
                    // XML reading, and data prepared to be sent back to the
                    // IGraphicSessionManager
                    sceneFound = true;
                    auto index = sceneChild.second.get<size_t>("<xmlattr>.index");
                    // Creation of the scene (addition later)
                    scenesToAdd[index] = std::make_shared<T>(shared_from_this(), canvasComponent->GetCanvas());
                    scenesToAdd[index]->SetName(sceneChild.second.get("name", "Unnamed scene"));
                    // And ordered creation of the sub-tree
                    /* C'est normal que l'on n'ajoute pas les aires ici directement !!
                     On ne connaît pas, à cet endroit, le type effectif des classes à charger (classes
                     d'aires graphiques filles dépendant de l'appli finale...)
                     On garde simplement les sous-arbres pour le graphic session manager fasse la
                     création effective des classes enfant.
                     */
                    sceneTrees[index] = std::make_shared<bptree::ptree>();
                    sceneTrees[index]->add_child("scene", sceneChild.second);
                }
                // Actual ordered insertion within the canvas manager
                for (size_t i=0 ; i<scenesToAdd.size() ; i++)
                    AddScene(scenesToAdd[i]); // T must be EditableScene or children
            }
            
            // If no scene found in file.... We'll be nice and add an empty one
            if (!sceneFound)
            {
                auto scene = std::make_shared<T>(shared_from_this(), canvasComponent->GetCanvas());
                scene->SetName("[Scène vide]");
                AddScene( scene ); // EditableScene or children
            }
            
            // Graphical updates before returning
            canvasComponent->UpdateSceneButtons(GetInteractiveScenes());
            
            
            return sceneTrees;
        }
    };
    
}



#endif  // MULTISCENECANVASINTERACTOR_H_INCLUDED
