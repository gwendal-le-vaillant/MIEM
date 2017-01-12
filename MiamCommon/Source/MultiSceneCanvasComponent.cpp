/*
  ==============================================================================

    MultiSceneCanvasComponent.cpp
    Created: 16 Nov 2016 5:35:10pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/


#include "MultiSceneCanvasComponent.h"





// = = = = = = = = = = GETTERS and SETTERS = = = = = = = = = =

void MultiSceneCanvasComponent::SetIsSelectedForEditing(bool isSelected)
{
    childrenCanvas->SetIsSelectedForEditing(isSelected);
}






//==============================================================================
MultiSceneCanvasComponent::MultiSceneCanvasComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    // Children canvas at first
    childrenCanvas = new SceneCanvasComponent();
    addAndMakeVisible(childrenCanvas);
    
    // Buttons then
    // A CHANGER
    sceneChoiceTextButtons.push_back(new TextButton("Default unique scene"));
    addAndMakeVisible(sceneChoiceTextButtons.back());
}

MultiSceneCanvasComponent::~MultiSceneCanvasComponent()
{
    delete childrenCanvas;
}


void MultiSceneCanvasComponent::LinkToManager(MultiSceneCanvasInteractor* _canvasManager)
{
    canvasManager = _canvasManager;
}




void MultiSceneCanvasComponent::paint (Graphics& /*g*/) // unused Graphics context
{
    //g.fillAll (Colours::mediumpurple);   // PURPLE background for debug
}

void MultiSceneCanvasComponent::resized()
{
    // Children display canvas on the bottom
    childrenCanvas->setSize(getWidth(), getHeight() -24 -space);
    childrenCanvas->setTopLeftPosition(0, 24 +space);
    
    // Buttons positionning
    updateSceneButtonsBounds();
}

// - - - - - Buttons management - - - - -
void MultiSceneCanvasComponent::UpdateSceneButtons(std::vector< std::shared_ptr<InteractiveScene> > scenes, int sceneThatHasChanged)
{
    // Update of all buttons
    if (sceneThatHasChanged == -1)
    {
        int selectedSceneId = canvasManager->GetSelectedSceneId();
        
        // At first, we remove all buttons from the parent
        for (size_t i = 0; i<sceneChoiceTextButtons.size() ; i++)
            removeChildComponent(sceneChoiceTextButtons[i]);
        sceneChoiceTextButtons.resize(0); // actual buttons deletion (scoped ptrs)
        
        // Then we create the new ones
        for (size_t i = 0; i<scenes.size() ; i++)
        {
            addButton(scenes[i]->GetName());
            if (i == selectedSceneId)
            {
                sceneChoiceTextButtons[i]->setColour(TextButton::buttonColourId, Colours::black);
                sceneChoiceTextButtons[i]->setColour(TextButton::textColourOffId, Colours::white);
            }
            else
                sceneChoiceTextButtons[i]->setColour(TextButton::buttonColourId, Colours::silver);
        }
        
        // Graphical updates (minimal)
        updateSceneButtonsBounds();
    }
    // Update of one precise button
    else
    {
        //throw std::runtime_error("Not Implemented. Update all scenes or nothing...");
        sceneChoiceTextButtons[sceneThatHasChanged]->setButtonText(scenes[sceneThatHasChanged]->GetName());
    }
}

void MultiSceneCanvasComponent::updateSceneButtonsBounds()
{
    int buttonWidth = roundFloatToInt(((float)(getWidth())-(float)(space))/(float)(sceneChoiceTextButtons.size()))-space;
    for (size_t i=0 ; i<sceneChoiceTextButtons.size() ; i++)
    {
        sceneChoiceTextButtons[i]->setBounds(space+(int)(i)*(buttonWidth+space), 0, buttonWidth, 24);
    }
}


void MultiSceneCanvasComponent::addButton(std::string buttonName)
{
    sceneChoiceTextButtons.push_back(new TextButton(buttonName));
    addAndMakeVisible(sceneChoiceTextButtons.back());
    sceneChoiceTextButtons.back()->addListener(this);
}

void MultiSceneCanvasComponent::buttonClicked(Button* buttonThatWasClicked)
{
    bool buttonFound = false; // else, the buttons could keep selecting themselves ??
    for (size_t i=0 ; i<sceneChoiceTextButtons.size() && !buttonFound ; i++)
    {
        if (buttonThatWasClicked == sceneChoiceTextButtons[i].get())
        {
            buttonFound = true;
            canvasManager->SelectScene((int)i);
        }
    }
}

    