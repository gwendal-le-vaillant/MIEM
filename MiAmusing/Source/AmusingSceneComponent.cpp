/*
  ==============================================================================

    AmusingSceneComponent.cpp
    Created: 7 Jul 2017 3:39:08pm
    Author:  ayup1

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "AmusingSceneComponent.h"
#include "MultiSceneCanvasManager.h"

#include <memory>

using namespace Miam;

//==============================================================================
AmusingSceneComponent::AmusingSceneComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
	isOptionShowed = false;

	showOptionsButton = new TextButton();
	showOptionsButton->setButtonText("opt");
	showOptionsButton->setSize(40, 20);
	showOptionsButton->setTopLeftPosition(0, 0);
	showOptionsButton->addListener(this);
	openGlContext.attachTo(*showOptionsButton);
	addAndMakeVisible(showOptionsButton);

	openGlContext.attachTo(areaOptions);
	addAndMakeVisible(areaOptions); // test pour voir si on le voit bien
	areaOptions.setBounds(0 , 0, 300, 200);
	areaOptions.CompleteInitialization(this);
	areaOptions.setVisible(false);
}

AmusingSceneComponent::~AmusingSceneComponent()
{
	//openGlContext.detach(areaOptions);
	//openGlContext.detach();
	delete showOptionsButton;
}

void AmusingSceneComponent::setSamplesColor(int Nsamples, Colour colorCode[])
{
	areaOptions.setSamplesColor(Nsamples, colorCode);
}

void AmusingSceneComponent::renderOpenGL()
{
	SceneCanvasComponent::renderOpenGL();
	//areaOptions.setBounds(0, 0, getWidth(), getHeight());
}

//void AmusingSceneComponent::mouseDown(const juce::MouseEvent &event)
//{
//	DBG("AmusingSceneComponent::mouseDown");
//}
//
//void AmusingSceneComponent::mouseDrag(const juce::MouseEvent &event)
//{
//	DBG("AmusingSceneComponent::mouseDrag");
//}
//
//void AmusingSceneComponent::mouseUp(const juce::MouseEvent &event)
//{
//	DBG("AmusingSceneComponent::mouseUp");
//}

void AmusingSceneComponent::OnSpeedChanged(double newSpeed)
{
	//DBG("newSpeed = " + (String)newSpeed);
	if (auto manager = canvasManager.lock())
	{
		if (auto managerAsManager = std::dynamic_pointer_cast<Amusing::MultiSceneCanvasManager>(manager))
		{
			managerAsManager->ChangeSpeed(newSpeed);
		}
	}
}

void AmusingSceneComponent::OnVelocityChanged(double newVelocity)
{
	if (auto manager = canvasManager.lock())
	{
		if (auto managerAsManager = std::dynamic_pointer_cast<Amusing::MultiSceneCanvasManager>(manager))
		{
			managerAsManager->ChangeVelocity(newVelocity);
		}
	}
}

void AmusingSceneComponent::OnColourChanged(Colour newColour, int colourIdx)
{
	if (auto manager = canvasManager.lock())
	{
		if (auto managerAsManager = std::dynamic_pointer_cast<Amusing::MultiSceneCanvasManager>(manager))
		{
			managerAsManager->ChangeColour(newColour,colourIdx);
		}
	}
}

void AmusingSceneComponent::OnBaseNoteChanged(int newBaseNote)
{
	DBG("newBaseNote = " + (String)newBaseNote);
	//canvasManager.lock()->
	//if(auto canvasManagerAsManager = std::dynamic_pointer_cast)
	if (auto manager = canvasManager.lock())
	{
		if (auto managerAsManager = std::dynamic_pointer_cast<Amusing::MultiSceneCanvasManager>(manager))
		{
			managerAsManager->ChangeBaseNote(newBaseNote);
		}
	}
}

void AmusingSceneComponent::SetAreaOptionsCenter(bpt center)
{
	showOptionsButton->setTopLeftPosition((int)center.get<0>()+100,(int) center.get<1>() - 150);
	areaOptions.setCentrePosition((int)center.get<0>(), (int)center.get<1>());
	
}

void AmusingSceneComponent::SetAreaOptionsVisible(bool show)
{
	if (show)
	{
		isOptionShowed = true;
		areaOptions.setVisible(true);
		showOptionsButton->setVisible(true);
	}
	else
	{
		isOptionShowed = false;
		areaOptions.setVisible(false);
		showOptionsButton->setVisible(false);
	}
}

void AmusingSceneComponent::SetAreaOptionsVisible(bool show,double speed, double velocity, int currentOctave, int colorIdx)
{
	if (show)
	{
		areaOptions.setSpeedSliderValue(speed);
		areaOptions.setVelocitySliderValue(velocity);
		areaOptions.setOctaveSlider(currentOctave);
		areaOptions.setCurrentColorSelected(colorIdx);
		showOptionsButton->setVisible(true);
	}
	//else
	//	areaOptions.setVisible(false);
}

void AmusingSceneComponent::buttonClicked(Button * buttonClicked)
{
	if (buttonClicked == showOptionsButton)
	{
		if (isOptionShowed)
		{
			areaOptions.setVisible(false);
			isOptionShowed = false;
		}
		else
		{
			areaOptions.setVisible(true);
			isOptionShowed = true;
		}
	}
}

void AmusingSceneComponent::mouseDown(const juce::MouseEvent & event)
{
	SceneCanvasComponent::mouseDown(event);
}

void AmusingSceneComponent::mouseDoubleClick(const juce::MouseEvent & event)
{
	DBG("doubleClick");
	if (auto manager = std::dynamic_pointer_cast<Amusing::MultiSceneCanvasManager>(canvasManager.lock()))
		manager->OnCanvasMouseDoubleClick(event);
}
