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

#include "MainComponent.h"

#include "View.h"
using namespace Miam;


//==============================================================================
MainContentComponent::MainContentComponent()
{
    // Tooltip window added here
    tooltipWindow.reset(new TooltipWindow(this));
    addChildComponent(tooltipWindow.get());
    
    // We add all big components from here (components edited from the introJucer)
    // Background is automatically visible, after startup though
    addChildComponent(mainBackgroundComponent = new MainBackgroundComponent());
    addChildComponent(startupComponent = new StartupComponent());
    // Other display modes aren't displayed by default
    addChildComponent(speakersEditionComponent = new SpeakersEditionComponent());
    addChildComponent(spatStatesEditionComponent = new StatesEditionComponent());
    addChildComponent(sceneEditionComponent = new SceneEditionComponent());
    addChildComponent(hardwareConfigurationComponent = new HardwareConfigurationComponent());
    secondLevelComponents.push_back(speakersEditionComponent);
    secondLevelComponents.push_back(spatStatesEditionComponent);
    secondLevelComponents.push_back(sceneEditionComponent);
    secondLevelComponents.push_back(hardwareConfigurationComponent);
        
    /* On doit préciser une taille (pour donner un repère aux enfants)
     *
     * As the "MainBackground" child's size is specified from the IntroJucer,
     * we can use it to specify its "Main Component" parent's size
     */
    setSize(mainBackgroundComponent->getWidth(),mainBackgroundComponent->getHeight());
    
    
    // Changement du look'n'feel, qui sera répercuté sur tous les enfants
    setLookAndFeel(&lookAndFeel);
}

MainContentComponent::~MainContentComponent()
{
    delete hardwareConfigurationComponent;
    delete sceneEditionComponent;
    delete spatStatesEditionComponent;
    delete speakersEditionComponent;
    
    delete startupComponent;
    delete mainBackgroundComponent;
    
    // Sinon assertion lancée.... Pas sérieux de Juce mais bon...
    setLookAndFeel(nullptr);
}




// ============== SETTERS AND GETTERS ==============
void MainContentComponent::CompleteInitialization(Presenter* _presenter)
{
    presenter = _presenter;
    
    mainBackgroundComponent->CompleteInitialization(presenter);
    startupComponent->CompleteInitialization(presenter);
}

void MainContentComponent::SetMiamView(Miam::View* _view)
{
    view = _view;
    
    mainBackgroundComponent->setMiamView(view);
}





void MainContentComponent::ChangeAppMode(AppMode newAppMode)
{
    // Firstly, we hide everything
    mainBackgroundComponent->setVisible(false);
    for (size_t i=0 ; i<secondLevelComponents.size() ; i++)
    {
        secondLevelComponents[i]->setVisible(false);
    }
    
    
    switch (newAppMode)
    {
        case AppMode::Startup :
            startupComponent->setVisible(true);
            mainBackgroundComponent->setVisible(false);
            break;
            
        case  AppMode::Loading :
            // all component remains unvisible, only background component is visible
            startupComponent->setVisible(false);
            mainBackgroundComponent->setVisible(true);
            break;
            
        case AppMode::EditSpeakers :
            startupComponent->setVisible(false);
            mainBackgroundComponent->setVisible(true);
            
            speakersEditionComponent->setVisible(true);
            break;
            
        case AppMode::EditControlStates :
            startupComponent->setVisible(false);
            mainBackgroundComponent->setVisible(true);
            
            spatStatesEditionComponent->setVisible(true);
            break;
            
        case AppMode::EditControlScenes :
            startupComponent->setVisible(false);
            mainBackgroundComponent->setVisible(true);
            
            sceneEditionComponent->setVisible(true);
            break;
        
        case AppMode::EditSettings :
            startupComponent->setVisible(false);
            mainBackgroundComponent->setVisible(true);
            
            hardwareConfigurationComponent->setVisible(true);
            break;
            
        case AppMode::MiemPlayerLaunched :
            DisplayInfo("Can't launch the external Spat player yet!");
            break;
            
        default :
            break;
    }

    // Dans le doute...
    resized();
}
void MainContentComponent::DisplayInfo(const String& message, int priority)
{
    mainBackgroundComponent->DisplayInfo(message, priority);
}





void MainContentComponent::paint (Graphics& /*g*/)
{
}

void MainContentComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    
    // Background must always be resized
    mainBackgroundComponent->setBounds(getLocalBounds());
    startupComponent->setBounds(getLocalBounds());
    // Other children are only resized if visible
    int yChildrenOffset = 5*8; // 5 cases depuis le haut (pour l'instant)
    for (size_t i=0;i<secondLevelComponents.size();i++)
    {
        //if (secondLevelComponents[i]->isVisible()) { // EVERYTHING ACTUALIZED, VISIBLE OR NOT
            secondLevelComponents[i]->setBounds(0, yChildrenOffset, getWidth(), getHeight()-yChildrenOffset);
        //}
    }
}

bool MainContentComponent::keyPressed(const KeyPress& key)
{
    bool keyWasUsed = false;
    
    
    // ====================== Keyboard SHORTCUTS =====================
    if (key.getModifiers().isCommandDown())
    {
        keyWasUsed = true;
        
        // Switch MAIN tab
        if (key.getKeyCode() == mainBackgroundComponent->GetSwitchTabCommandKey())
            mainBackgroundComponent->TriggerTabSwitch(presenter->getAppMode());
        // Switch to Conf tab
        else if (key.getKeyCode() == mainBackgroundComponent->GetConfigurationTabCommandKey())
            mainBackgroundComponent->TriggerConfigurationTab();
        // Si on MAJ en plus, alors sauvegarde SOUS. Sinon juste une sauvegarde
        else if (key.getKeyCode() == mainBackgroundComponent->GetSaveCommandKey())
            mainBackgroundComponent->TriggerSave(key.getModifiers().isShiftDown());
        // Open (Load)
        else if (key.getKeyCode() == mainBackgroundComponent->GetLoadCommandKey())
            mainBackgroundComponent->TriggerLoad();
        
        // If no key was used... We make it back to false
        else
            keyWasUsed = false;
    }
    
    // Forced callback to parent, if unused
    if (! keyWasUsed)
        return getParentComponent()->keyPressed(key);
    // or return true to prevent the event to be passed-on.
    else
        return keyWasUsed;
}

void MainContentComponent::modifierKeysChanged (const ModifierKeys& /*modifiers*/)
{
}


