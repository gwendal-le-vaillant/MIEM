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

#ifndef CONTROLSTATE_H_INCLUDED
#define CONTROLSTATE_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <numeric> // std::accumulate

#include "SparseMatrix.hpp"
#include "AudioUtils.hpp"

#include "ControlArea.h"

#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/xml_parser.hpp"
namespace bptree = boost::property_tree;

namespace Miam
{
    // Forward declarations
    class ControlArea;
    
    
    /// \brief An abstract class that represents a state of the
    /// speakers' system, that represents a spatialisation state
    /// (or spatialisation effect)
    ///
    /// T is a floating-point type. It might be an int/fixed-point
    /// type some day in the future....
    ///
    /// The number of channels might not be exactly the same as
    /// the parent Miam::StatesInterpolator (and this has to be
    /// properly managed)
    ///
    /// !!! Attention aux fonctions membres virtuelles qui ne peuvent alors
    /// pas être template !!! -> encore vrai sur tous les compilateurs ?
    template<typename T>
    class ControlState
    {
        
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        private :
        /// \brief Pour éviter des boucles de suppression (les aires re-notifient quand on se délie d'elles)
        bool unlinkingFromAreas = false;
        
        
        protected :
        
        // Own attributes
        std::string name;
        Colour colour;
        
        // For convenience and optimizations...
        int index;
        
        
        /// \brief List of areas that represent this spatialization state
        ///
        /// Not the most optimal STL container (research is not optimized)
        ///
        /// \remark Pour accès synchrone aux aires graphiques ! Donc pour édition offline seulement...
        /// On mode de jeu, on devra travailler avec les UIDs qui viennent des paquets lock-free
        std::vector< std::weak_ptr<ControlArea> > linkedAreas;
        
        /// \brief Excitations en cours, issues d'objets graphiques quelconques référencés par leur UID
        std::map< uint64_t, T > areaToExcitement;
        
        /// Copie Thread-safe d'une donnée qu'on pourrait retrouver depuis les linkedAreas
        T excitement;
        
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        public :
        
        /// \returns A string containing the Id (counting from 1) and the name
        /// concatenated
        virtual std::string GetName(bool withId = true) const
        {
            std::string indexString;
            if (withId)
                indexString += boost::lexical_cast<std::string>(index+1) + ". ";
            return indexString + name;
        }
        virtual void SetName(std::string _name)
        {
            name = _name;
            for (auto it = linkedAreas.begin() ; it!=linkedAreas.end() ; it++)
            {
                auto lockedLinkedArea = (*it).lock();
                if (lockedLinkedArea)
                    lockedLinkedArea->OnStateNameChanged();
                else
                    assert(false); // linkedAreas should always be valid...
            }
        }
        virtual Colour GetColour() const {return colour;}
        virtual void SetColour(Colour& _colour)
        {
            colour = _colour;
            // Sent back to all linked areas
            for (size_t i = 0; i < linkedAreas.size() ; i++)
            {
                if (auto sharedPtr = linkedAreas[i].lock())
                    sharedPtr->SetFillColour(colour);
                #ifdef __MIAM_DEBUG
                else
                    throw std::runtime_error("No weak pointer should be unvalid at that point");
                #endif
            }
        }
        
        void SetIndex(int newIndex) {index = newIndex;}
        int GetIndex() const {return index;}
        
        /// \brief Number of actually used outputs
        virtual size_t GetOutputsCount() = 0;
        
        virtual size_t GetLinkedAreasCount() {return linkedAreas.size();}
        virtual std::shared_ptr<ControlArea> GetLinkedArea(size_t i)
        {
            if(auto area = linkedAreas[i].lock())
                return area;
            else
                throw std::runtime_error("Spat state is linked to a non-existant area");
        }
        
        T GetExcitement() const {return excitement;}
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
        ControlState() : colour(127, 255, 0), // chartreuse
                            index(-1)
        {
            // pas possible de le mettre dans la liste d'initialisation
            // car le double ne prend pas (sous XCode) un zéro de type 'void'
            excitement = {};
        }
        
        
        // - - - - - Construction / destruction - - - - -
        virtual ~ControlState()
        {
            // Called when this spat state is unregistered from all areas that
            // kept a shared_ptr to this
        }
        
        
        virtual void ClearExcitements()
        {
            areaToExcitement.clear();
            excitement = {};
        }

        
        // - - - - - Internal linking to spat areas - - - - -
        /// \brief Ordre reçu d'une aire
        void LinkToArea(std::shared_ptr<ControlArea> spatArea)
        {
            // no check for double addition (but it's weak ptrs...)
            linkedAreas.push_back(spatArea);
        }
        /// \brief Ordre reçu d'une aire
        void UnlinkToArea(std::shared_ptr<ControlArea> spatArea)
        {
            if (! unlinkingFromAreas)
            {
                for (auto it = linkedAreas.begin() ; it!=linkedAreas.end() ; it++)
                {
                    auto curAreaPtr = (*it).lock();
                    if (curAreaPtr == spatArea)
                    {
                        linkedAreas.erase(it);
                        break;
                    }
                }
            }
        }
        
        // - - - - - External linking to spat areas - - - - -
        void UnregisterFromAreas()
        {
            unlinkingFromAreas = true;
            
            // does not need to internally unlink from linked areas : the link is a weak_ptr
            // But still needs to unregister from linked areas
            for (size_t i=0 ; i < linkedAreas.size() ; i++)
            {
                auto areaPtr = linkedAreas[i].lock();
                if (areaPtr)
                    areaPtr->LinkToState(nullptr);
            }
            
            unlinkingFromAreas = false;
        }
        
        
        // - - - - - Interpolation computation - - - - -
        
        /// \brief Sauvegarde de l'excitation qui vient d'arriver, en utilisant/actualisation
        /// la map qui permet d'associer des UIDs à des excitations
        virtual void OnNewExcitementAmount(uint64_t senderUID, T newExcitement)
        {
            // Cas 1 : élement existe
            try {
                double& concernedExcitement = areaToExcitement.at(senderUID); // may throw except
                // Cas 1.a : suppression d'un elmt existant, si excitation trop faible
                if (AudioUtils<T>::IsVolumeNegligible(newExcitement))
                    areaToExcitement.erase(senderUID);
                // Cas 1.b : simpe mise à jour d'un elmt existant
                else
                    concernedExcitement = newExcitement;
            }
            // Cas 2 : insertion d'un nouvel elmt dans la map

			// A REFAIRE PROPREMENT SANS UTILISER l'EXCEPTION
			// car ça pourrit totalement la console VS2017... Cette exception arrive
			// si souvent qu'elle n'est pas exceptionnelle....
            catch (std::out_of_range & /*e*/) { // si on a pas trouvé la clé dans la map
                 // on teste quand même si pas trop faible, avant d'insérer
                if ( ! AudioUtils<T>::IsVolumeNegligible(newExcitement) )
                    areaToExcitement[senderUID] = newExcitement; // insertion automatique via []
            }
            
            // à la fin : update direct de la somme (accumulate ne fonctionne pas sur des elts de type map)
            excitement = {};
            for ( auto&& mapPair : areaToExcitement)
                excitement += mapPair.second;
        }
        
        
        
        // - - - - - Output channels (speakers) : add, delete, swap, ... - - - - -
        virtual void AddOutput() = 0;
        virtual void DeleteOutput(size_t i) = 0;
        virtual void SwapOutputs(size_t i, size_t j) = 0;
        
        // - - - - - Property tree (for XML) import/export - - - - -
        virtual std::shared_ptr<bptree::ptree> GetTree()
        {
            auto pTree = std::make_shared<bptree::ptree>();
            // This sub-tree does not know its own "master name tag" = <state>
            pTree->put("name", GetName(false));
            pTree->put("colour", colour.toString().toStdString());
            return pTree;
        }
        /// \brief Receives all the children nodes and contents inside a <state> tag,
        /// but not the <state> and </state> tags themselves.
        virtual void SetFromTree(bptree::ptree & stateTree)
        {
            // Common attributes = name only, for now
            // With default value to avoid try/catch block
            // And automatic data type identifying (= default value type)
            SetName( stateTree.get("name", std::string("unnamed")) );
            colour = Colour::fromString(stateTree.get("colour", std::string("ff7fff00"))); // chartreuse
        }
        
    };
    
}


#endif  // CONTROLSTATE_H_INCLUDED
