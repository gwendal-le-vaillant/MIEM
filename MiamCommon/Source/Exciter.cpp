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

#include "Exciter.h"

#include "MultiAreaEvent.h"

#include <cmath>
#include <algorithm>
#include <numeric> // std::accumulate

#include "SceneCanvasComponent.h" // pour récupérer width/height

#include "MultiSceneCanvasComponent.h" // to get the height to the scene's buttons

#include "AreasGroup.h"

using namespace Miam;

// = = = = = = = = = = Construction/Destruction + polymorphic cloning = = = = = = = = = =

Exciter::Exciter(bptree::ptree & areaTree, std::chrono::time_point<SteadyClock> commonStartTimePoint_, int additionnalTouchGrabRadius_)
:
EditableEllipse(areaTree),

additionnalTouchGrabRadius(additionnalTouchGrabRadius_),
commonStartTimePt(commonStartTimePoint_)
{
    init();
    
    // Paramètres facultatifs
    try {
        isAnimationSynchronized = areaTree.get<bool>("is_animation_sync");
    }
    catch (bptree::ptree_error&) { }
    
}

Exciter::Exciter(uint64_t uniqueId, std::chrono::time_point<SteadyClock> commonStartTimePoint_, int additionnalTouchGrabRadius_)
:
// AUTO RESIZING
// Taille = 5% du canevas (de la + petite taille). Ratio inconnu, 1 par défaut...
// EditableEllipse(uniqueId, bpt(0.5, 0.5), 0.05, Colours::lightgrey, 1.0f),

// FIXED SIZE IN PIXELS
EditableEllipse(uniqueId, bpt(0.5, 0.5), (MultiSceneCanvasComponent::SceneButtonsHeight/2)+8, Colours::lightgrey),

additionnalTouchGrabRadius(additionnalTouchGrabRadius_),
commonStartTimePt(commonStartTimePoint_)
{
    init();
}

Exciter::~Exciter()
{
    // Dé-exitation des aires qui existent encore
    for (auto& areaInteractingData : areasInteractingWith)
    {
        // ici : pas de test sur le lock... de toute manière la classe est détruite juste après
        if (auto area = areaInteractingData.Area.lock())
            area->OnExciterDestruction();
    }
}
void Exciter::onCloned()
{
    // D'abord on supprime tous les index locaux
    while (areasInteractingWith.size() > 0)
        deleteLinksToArea(areasInteractingWith.size() - 1); // suppresion du dernier (car vecteurs)
    // Puis on appelle les parents qui devraient faire le même genre de chose
    EditableEllipse::onCloned();
}



void Exciter::init()
{
    interpolationType = InterpolationType::None;
    
    // Centre (voir DrawableArea)
    displayCenter = true;
    
    SetNameVisible(false);
    
    SetActive(false);
    SetOpacityMode(OpacityMode::Mid);
    SetEnableTranslationOnly(true);
    
    volume = 0.0;
    startTimePt = SteadyClock::now();
    
    // Par défaut : volume de 1
    SetVolume(1.0);
    isAnimationSynchronized = true;
}
double Exciter::computeXScale(float _canvasRatio)
{
	return 0.5f + 0.5f / _canvasRatio;
}
double Exciter::computeYScale(float _canvasRatio)
{
	return 0.5f + 0.5f * _canvasRatio;
}



void Exciter::SetVolume(double volume_)
{
    volume = volume_;
    
    // Pour l'instant rien, mais à l'avenir ça devra afficher graphiquement le résultat
}

// = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
void Exciter::SetIsAnimationSynchronized(bool isSynchronized_)
{
    // On suppose que le point de départ commun a bien été initialisé auparavant...
    
    // Dans tous les cas on sauvegarde l'état
    isAnimationSynchronized = isSynchronized_;
}


// = = = = = = = = = = Display = = = = = = = = = =
void Exciter::Paint(Graphics& g)
{
    UpdateDynamicBrightness(SteadyClock::now());
    
    // Parent painting
    EditableEllipse::Paint(g);
}
AreaEventType Exciter::UpdateDynamicBrightness(const std::chrono::time_point<SteadyClock>& timePoint)
{
    std::chrono::duration<double> duration; // par défaut, en secondes
    if (isAnimationSynchronized)
        duration = timePoint - commonStartTimePt;
    else
        duration = timePoint - startTimePt;
    
    // Création d'une couleur opaque (alpha=1.0f)
    // à partir de la luminosité seulement (sat et teinte = 0.0f)
    double brightness = deltaBrightnessOffset + deltaBrightnessAmplitude
                                        * std::cos(omega * duration.count());
    
    // Application
    contourColour = Colour(0.0f, 0.0f, (float)brightness, 1.0f);
#ifdef __MIEM_VBO
    RefreshOpenGLBuffers();
#endif
    
    return AreaEventType::ColorChanged;
}

// = = = = = = = = = = Interactions = = = = = = = = = =
bool Exciter::HitTest(bpt T) const
{
    if (additionnalTouchGrabRadius == 0)
        return EditableEllipse::HitTest(T);
    else
    {
        // Pas besoin d'appliquer le facteur d'échelle ? Il faudrait fouiller le code
        // de Guillaume (avec les xscale et yscale) pour en être sûr
        // ATTENTION a et b sont les 1/2 longueurs des axes
        double aInPx_touch = aInPixels + additionnalTouchGrabRadius;
        double bInPx_touch = bInPixels + additionnalTouchGrabRadius;
        // Point à tester, dans le repère du centre de l'ellipse
        bpt relativeT = T; // copie car soustraction boost se fait par référence
        boost::geometry::subtract_point(relativeT, centerInPixels);
        // On applique juste l'inéquation paramétrique de l'ellipse
        if ( (std::pow(relativeT.get<0>() / aInPx_touch, 2)
              + std::pow(relativeT.get<1>() / bInPx_touch, 2) )
            < 1.0 )
            return true;
        else
            return false;
    }
}

void Exciter::OnAreaExcitedByThis(std::shared_ptr<IInteractiveArea> areaExcitedByThis, double interactionWeight)
{
    size_t areaIndex = findAreaInteractingIndex(areaExcitedByThis);
    // Si l'aire n'existait pas, on l'ajoute
	if (areaIndex == areasInteractingWith.size())
	{
		areasInteractingWith.push_back(AreaInteractingData(areaExcitedByThis));
	}
    // Mise à jour dans tous les cas
    areasInteractingWith[areaIndex].InteractionWeight = interactionWeight;
    
    // Pas de mise à jour des excitations : cela dépend des modifs apportées à TOUTES les aires,
    // on attend l'ordre de notification (par la scène mère, ou autre...)
}
void Exciter::OnAreaNotExcitedByThis(std::shared_ptr<IInteractiveArea> areaExcitedByThis)
{
    // On fait la recherche, et on vérifie le résultat par sécurité...
    size_t areaIndex = findAreaInteractingIndex(areaExcitedByThis);
    if ( areaIndex < areasInteractingWith.size() )
    {
        // D'abord on impose une mise à jour côté aire graphique (excitation nulle forcée)
        // Pas de check du lock : on vient de retrouver l'index de l'aire graphique (via shared_ptr....)
        areasInteractingWith[areaIndex].Area.lock()->OnNewExcitementAmount(getCastedSharedFromThis(), Excitement());
        
        // Puis : Suppression effective
        deleteLinksToArea(areaIndex);
    }
    else
        DBG("Double notification de fin d'excitation d'une aire par cet excitateur");
}
size_t Exciter::findAreaInteractingIndex(std::shared_ptr<IInteractiveArea> areaToFind)
{
    // naive research
    size_t i = 0;
    for (i = 0 ; i < areasInteractingWith.size() ; i++ )
    {
        auto lockedAreaInteractingWith = tryLockAreaOrDelete(i);
        if ( lockedAreaInteractingWith == areaToFind )
            return i;
    }
    // if nothing found :
    return i;
}
std::shared_ptr<IInteractiveArea> Exciter::tryLockAreaOrDelete(size_t areaLocalIndex)
{
    auto lockedAreaInteractingWith = areasInteractingWith[areaLocalIndex].Area.lock();
    if (lockedAreaInteractingWith) // area is valid
        return lockedAreaInteractingWith;
    else // area has been destructed
    {
        deleteLinksToArea(areaLocalIndex);
        return nullptr;
    }
}
void Exciter::deleteLinksToArea(size_t areaLocalIndex)
{
    auto areaIt = areasInteractingWith.begin();
    std::advance(areaIt, areaLocalIndex);
    areasInteractingWith.erase(areaIt);
}
void Exciter::updateExcitationAmounts()
{
    // On appliquera le volume le + tard possible (on fait tout par rapport à 1, puis on diminue ensuite...)
    
    // On calcule d'abord le total des poids d'interaction, et on en profite pour dégager toutes les aires
    // qui ne peuvent pas être lockées
    double totalInteractionWeight = 0.0;
    for (size_t i=0 ; i < areasInteractingWith.size() ; i++)
    {
        // Si n'existe plus : on fait i-- car on aura un i++ au moment de passer l'itération suivante
        if (! tryLockAreaOrDelete(i))
            i--;
        // Sinon, OK, on ajoute bien le poids concerné
        else
            totalInteractionWeight += areasInteractingWith[i].InteractionWeight;
    }
    
    
    // - - - - - Calcul des excitations, tel que la somme des excitations vaut 1 - - - - -
    // en vérifiant les cas bizarres limites...
    // Pour être sûr même si des erreurs
    // numériques sont là...
    
    // Si on a un poids total non-valide : répartition uniforme sur toutes les aires
    if (totalInteractionWeight <= 0.0)
    {
        double uniformWeight = 1.0 / (double)(areasInteractingWith.size());
        for (auto &areaData : areasInteractingWith)
            areaData.ExcitementAmount.Linear = uniformWeight;
    }
    // Si le poids total est OK : on normalise simplement les poids pour calculer les excitations
    else
    {
        for (auto &areaData : areasInteractingWith)
            areaData.ExcitementAmount.Linear = areaData.InteractionWeight / totalInteractionWeight;
    }
    
    
    // - - - - - Distorsion logarithmique, pour donner la précision aux faibles volumes - - - - -
    // Voir feuille de calcul Matlab + explications papier nécessaires....
    // - - - - -         Ou aucune distorsion si interpolation linéaire simple          - - - - -
    for (auto &areaData : areasInteractingWith)
    {
        switch (interpolationType)
        {
            // - - - distorsion pour les faibles volumes - - -
            case InterpolationType::Matrix_ConstantAmplitude :
            case InterpolationType::Matrix_ConstantPower :
                areaData.ExcitementAmount.Audio
                    = AudioUtils<double>::ApplyLowVolumePrecisionDistorsion(areaData.ExcitementAmount.Linear);
                break;
                
            // - - - pas de distorsion - - -
            case InterpolationType::Matrix_Linear :
                areaData.ExcitementAmount.Audio = areaData.ExcitementAmount.Linear;
                break;
                
            default : // None, (Count,) etc...
                areaData.ExcitementAmount.Audio = 1.0;
                break;
        }
    }
    
    
    // - - - - - Normalisation pour les coeffs Audio, selon le type d'interpolation - - - - -
    double normalisationFactor = 0.0;
    switch (interpolationType)
    {
        // - - - Normalisation de la somme des amplitudes - - -
        case InterpolationType::Matrix_ConstantAmplitude :
            for (auto &areaData : areasInteractingWith)
                normalisationFactor += areaData.ExcitementAmount.Audio;
            for (auto &areaData : areasInteractingWith)
                areaData.ExcitementAmount.Audio = areaData.ExcitementAmount.Audio / normalisationFactor;
            break;
            
        // - - - Normalisation de la somme du carré des amplitudes - - -
        case InterpolationType::Matrix_ConstantPower :
            for (auto &areaData : areasInteractingWith)
                normalisationFactor += std::pow(areaData.ExcitementAmount.Audio, 2);
            normalisationFactor = std::sqrt(normalisationFactor); // == sqrt( somme( amplitudes^2 ) )
            for (auto &areaData : areasInteractingWith)
                areaData.ExcitementAmount.Audio = areaData.ExcitementAmount.Audio / normalisationFactor;
            break;
            
        // - - - cas où l'on ne fait rien - - -
        case InterpolationType::Matrix_Linear :
        default :
            break;
    }
    
    
    // - - - - - Application du volume, de manière uniforme, à la toute fin - - - - -
    for (auto &areaData : areasInteractingWith)
    {
        areaData.ExcitementAmount.Linear *= volume;
        areaData.ExcitementAmount.Audio *= volume;
    }
}

void Exciter::NotifyNewExcitationToAreas()
{
    // Préparatifs internes
    updateExcitationAmounts();
    //auto multiAreaE = std::make_shared<MultiAreaEvent>(); // le premier event va rester Nothing....
    
    // Notifications à toutes les aires
    for (size_t i = 0; i<areasInteractingWith.size() ; i++)
    {
        auto lockedAreaInteractingWith = areasInteractingWith[i].Area.lock();
        // Ici on ne doit plus rater de lock()... Car les weak_ptr 'morts' auraient dû être supprimés
        // au update juste au-dessus
#ifdef __MIAM_DEBUG
        if ( ! (lockedAreaInteractingWith) )
            throw std::logic_error("weak_ptr sur Aire référencée dans un excitateur n'est plus valide...");
#endif
        lockedAreaInteractingWith->OnNewExcitementAmount(getCastedSharedFromThis(),
                                                         areasInteractingWith[i].ExcitementAmount);
    }
}

int Exciter::FindAreasGroupIndex()
{
    int groupIndex = (int)AreasGroup::SpecialIds::Background;
    
    // on va parcourir les aires dans l'ordre... et tant que le calcul d'image est bien fait
    // (vérifications internes dans l'algo de InteractiveScene), pas de soucis à prévoir.
    for (size_t k = 0 ;
         (k<areasInteractingWith.size() && (groupIndex == (int)AreasGroup::SpecialIds::Background)) ;
         k++)
    {
        if (auto areaPtr = areasInteractingWith[k].Area.lock())
            if (auto groupPtr = areaPtr->GetAreasGroup())
                groupIndex = groupPtr->GetIndexInScene();
    }
    
    return groupIndex;
}



// = = = = = = = = = = XML import/export = = = = = = = = = =
std::shared_ptr<bptree::ptree> Exciter::GetTree()
{
    auto inheritedTree = EditableEllipse::GetTree();
    
    inheritedTree->put("is_animation_sync", isAnimationSynchronized);
    
    return inheritedTree;
}


