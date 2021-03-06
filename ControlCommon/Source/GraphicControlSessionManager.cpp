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

#include "GraphicControlSessionManager.h"

#include "ControlPresenter.h"
#include "ControlPolygon.h"

#include "XmlUtils.h"

GraphicControlSessionManager::GraphicControlSessionManager(ControlPresenter* _presenter)
:
IGraphicSessionManager(_presenter),
presenter(_presenter)
{}

void GraphicControlSessionManager::CompleteInitialisation(std::shared_ptr<StatesInterpolator<double>> _spatInterpolator)
{
    spatInterpolator = _spatInterpolator;
}


AsyncParamChange GraphicControlSessionManager::buildExcitementParamChange(std::shared_ptr<ControlArea> area)
{
    AsyncParamChange paramChange;
    
    paramChange.Type = AsyncParamChange::ParamType::Excitement;
    paramChange.DoubleValue = area->GetTotalAudioExcitement();

    // Attention : pour les IDs, on déclenche une grosse exception si on dépasse...
    if (area->GetStateIndex() < std::numeric_limits<int>::max())
    paramChange.Id1 = (int)area->GetStateIndex();
    else
    throw std::overflow_error("Spat state Index is too big to fit into an 'int'. Cannot send the concerned lock-free parameter change.");
    if (area->GetId() < std::numeric_limits<int>::max())
    paramChange.Id2 = (int)area->GetId();
    else
    throw std::overflow_error("Area UID is too big to fit into an 'int'. Cannot send the concerned lock-free parameter change.");
    
    return paramChange;
}


// = = = = = = = = = = XML import/export = = = = = = = = = =

void GraphicControlSessionManager::SetFromTree(bptree::ptree& graphicSessionTree)
{
    // Variables qui servent tout au long de cet algorithme de chargement
    bool forceReinit = false;
    int64_t biggestAreaUid = -1;
    
    // Si la commande de réinit était clairement passée à cette fonction :
    if (graphicSessionTree.empty())
        forceReinit = true;
    
    // Peut lancer une exception (et donc il faudra lancer une réinitialisation du module entier)
    std::vector< std::shared_ptr<bptree::ptree> > canvasTrees;
    if ( ! forceReinit )
    {
        try {
            canvasTrees = IGraphicSessionManager::ExtractCanvasesSubTrees(graphicSessionTree);
        }
        // S'il y a une erreur : on quitte la fonction directement
        catch (bptree::ptree_error& e) {
            throw XmlReadException("<graphicsession> : error extracting <canvas> nodes : ", e);
        }
    }
    
    // On fait des actions pour tous les canevas même si une erreur était apparue
    for (size_t i=0 ; i<canvasManagers.size() ; i++)
    {
        // D'abord on crée toutes les scènes
        // Mais quand même, s'il y a eu une erreur, on réinitialise (en envoyant un arbre vide)
        if (forceReinit)
        {
            bptree::ptree emptyTree = bptree::ptree(); // pour le passer en référence
            canvasManagers[i]->SetScenesFromTree<EditableScene>( emptyTree );
        }
        // Si pas de réinit
        else
        {
            auto sceneTrees = canvasManagers[i]->SetScenesFromTree<EditableScene>(*(canvasTrees[i]));
            
            // Puis on parcourt toutes les scènes du canevas en cours, pour
            // n'ajouter que les aires qui correspondent à notre application
            // (seulement les SpatPolygon pour l'instant...)
            for (size_t j=0 ; j<sceneTrees.size() ; j++)
            {
                std::string canvasAndSceneString = "Canvas " + boost::lexical_cast<std::string>(i) + ", Scene " + boost::lexical_cast<std::string>(j) + ": ";
                
                // Première passe de lecture
                size_t areasCount;
                try {
                    areasCount = XmlUtils::CheckIndexes(*(sceneTrees[j]), "scene.areas", "area");
                }
                catch (XmlReadException &e) {
                    throw XmlReadException(canvasAndSceneString, e);
                }
                // Pré-chargement des aires
                std::vector<std::shared_ptr<InteractiveArea>> areas; // y compris les excitateurs
                std::vector< std::shared_ptr<bptree::ptree> > spatStateTrees;
                areas.resize(areasCount);
                spatStateTrees.resize(areasCount);
                for (auto& area : sceneTrees[j]->get_child("scene.areas"))
                {
                    try {
                        auto index = area.second.get<size_t>("<xmlattr>.index");
                        auto type = area.second.get<std::string>("<xmlattr>.type");
                        // Spat Polygones
                        if (type == "SpatPolygon")
                        {
                            try {
                                areas[index] = std::make_shared<ControlPolygon>(area.second);
                                try {
                                    spatStateTrees[index] = std::make_shared<bptree::ptree>(area.second.get_child("spatstate"));
                                }
                                catch (bptree::ptree_bad_path) {/* we do nothing */}
                            }
                            catch (XmlReadException &e) {
                                throw XmlReadException(canvasAndSceneString + e.what());
                            }
                        }
                        // Excitateurs
                        else if (type == "Exciter")
                        {
                            try {
                                areas[index] = std::make_shared<Exciter>(area.second, GetCommonTimePoint(), Exciter::AdditionnalGrabRadius::Medium);
                            }
                            catch (XmlReadException &e) {
                                throw XmlReadException(canvasAndSceneString + e.what());
                            }
                        }
                        // Le reste : on ne prend pas...
                        else
                            throw XmlReadException(canvasAndSceneString + "only SpatPolygon and Exciters objects can be loaded at the moment.");
                        
                        // Recherche de l'UID le plus grand utilisé jusqu'ici
                        if ( areas[index]->GetId() > biggestAreaUid )
                            biggestAreaUid = areas[index]->GetId();
                    }
                    catch (bptree::ptree_error &e) {
                        throw XmlReadException(canvasAndSceneString, e);
                    }
                }
                // Finalement, ajout effectif des aires dans le bon ordre
                for (size_t k=0 ; k<areas.size() ; k++)
                {
                    // Fonction commune à toutes les aires
                    canvasManagers[i]->AddAreaToScene(j, areas[k]);
                    
                    // Spat areas seulement
                    if (auto spatArea = std::dynamic_pointer_cast<ControlArea>(areas[k]))
                        LoadSpatAreaLinks(spatArea, spatStateTrees[k]);
                }
                
                // Actualisations finales pour la scène en cours (après la 2nde passe)
                // ... néant ... on accède le moins possible à des scènes sans passer
                // par le canevas maître
            }
            
        } // fin de condition : si pas de réinitialisation forcée
    } // fin de la boucle : pour chaque canevas
    
    // Bonnes valeurs pour les compteurs internes
    nextAreaId = biggestAreaUid + 1; // On passe à l'UID suivant
    
    // Actualisations pour chaque canevas
    for (auto& canvas : canvasManagers)
        canvas->OnXmlLoadFinished();
}
void GraphicControlSessionManager::LoadSpatAreaLinks(std::shared_ptr<ControlArea> area, std::shared_ptr<bptree::ptree> spatStateTree)
{
    if (spatStateTree == nullptr)
        return;
    
    auto spatStateIndex = spatStateTree->get<int64_t>("<xmlattr>.index", -1);
    if (spatStateIndex >= 0)
        area->LinkToState(spatInterpolator->GetState((size_t)spatStateIndex));
}

