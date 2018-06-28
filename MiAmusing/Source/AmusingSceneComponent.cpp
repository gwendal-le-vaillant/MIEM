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
AmusingSceneComponent::AmusingSceneComponent() : SceneCanvasComponent(Npolygons + Npolygons * (Npolygons + 1) / 2, 6*64)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
	currentSideBarType = SideBarType::None;
	isOptionShowed = false;

	showOptionsButton = new TextButton();
	showOptionsButton->setButtonText("opt");
	showOptionsButton->setSize(40, 20);
	showOptionsButton->setTopLeftPosition(0, 0);
	showOptionsButton->addListener(this);
	openGlContext.attachTo(*showOptionsButton);
	//addAndMakeVisible(showOptionsButton);

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

	for (int i = 0; i < Nsamples; ++i)
		buttonsColor.push_back(colorCode[i]);
}

void AmusingSceneComponent::addColourSample(int index, Colour _colour)
{
	areaOptions.addColourSample(index, _colour);
	buttonsColor.insert(buttonsColor.begin() + index, _colour);
}

void AmusingSceneComponent::newOpenGLContextCreated()
{
	DBG("newOpenGL Amusing!!!!!!");

	g_testSideBarVertex_buffer_data[0] = (float)getWidth();
	g_testSideBarVertex_buffer_data[1] = (float)0.0f;
	g_testSideBarVertex_buffer_data[2] = (float)0.0f;
	g_testSideBarVertex_buffer_data[3] = (float)getWidth() - 50.0f;
	g_testSideBarVertex_buffer_data[4] = (float)0.0f;
	g_testSideBarVertex_buffer_data[5] = (float)0.0f;
	g_testSideBarVertex_buffer_data[6] = (float)getWidth() - 50.0f;
	g_testSideBarVertex_buffer_data[7] = (float)getHeight();
	g_testSideBarVertex_buffer_data[8] = (float)0.0f;
	g_testSideBarVertex_buffer_data[9] = (float)getWidth();
	g_testSideBarVertex_buffer_data[10] = (float)getHeight();
	g_testSideBarVertex_buffer_data[11] = (float)0.5f;

	g_testSideBarCoulour_buffer_data[0] = 1.0f;
	g_testSideBarCoulour_buffer_data[1] = 1.0f;
	g_testSideBarCoulour_buffer_data[2] = 1.0f;
	g_testSideBarCoulour_buffer_data[3] = 1.0f;
	g_testSideBarCoulour_buffer_data[4] = 1.0f;
	g_testSideBarCoulour_buffer_data[5] = 1.0f;
	g_testSideBarCoulour_buffer_data[6] = 1.0f;
	g_testSideBarCoulour_buffer_data[7] = 1.0f;
	g_testSideBarCoulour_buffer_data[8] = 1.0f;
	g_testSideBarCoulour_buffer_data[9] = 1.0f;
	g_testSideBarCoulour_buffer_data[10] = 1.0f;
	g_testSideBarCoulour_buffer_data[11] = 1.0f;
	g_testSideBarCoulour_buffer_data[12] = 1.0f;
	g_testSideBarCoulour_buffer_data[13] = 1.0f;
	g_testSideBarCoulour_buffer_data[14] = 1.0f;
	g_testSideBarCoulour_buffer_data[15] = 1.0f;

	g_testSideBarIndex_buffer_data[0] = 0;
	g_testSideBarIndex_buffer_data[1] = 1;
	g_testSideBarIndex_buffer_data[2] = 2;
	g_testSideBarIndex_buffer_data[3] = 3;
	g_testSideBarIndex_buffer_data[4] = 2;
	g_testSideBarIndex_buffer_data[5] = 0;

	SceneCanvasComponent::newOpenGLContextCreated();

	openGlContext.extensions.glGenBuffers(1, &testSideBarVertexBuffer);
	openGlContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, testSideBarVertexBuffer);
	openGlContext.extensions.glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(GLfloat),
		g_testSideBarVertex_buffer_data, GL_STREAM_DRAW);

	openGlContext.extensions.glGenBuffers(1, &testSideBarCoulourBuffer);
	openGlContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, testSideBarCoulourBuffer);
	openGlContext.extensions.glBufferData(GL_ARRAY_BUFFER, 4 * 4 * sizeof(GLfloat), g_testSideBarCoulour_buffer_data, GL_STREAM_DRAW);

	openGlContext.extensions.glGenBuffers(1, &testSideBarIndexBuffer);
	openGlContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, testSideBarIndexBuffer);
	openGlContext.extensions.glBufferData(GL_ELEMENT_ARRAY_BUFFER, 8 * 4 * sizeof(unsigned int), g_testSideBarIndex_buffer_data, GL_STREAM_DRAW);
}

void AmusingSceneComponent::renderOpenGL()
{
	SceneCanvasComponent::renderOpenGL();

	////side bar if needed
	//switch (currentSideBarType)
	//{
	//case GrayScale:
	//	g.setGradientFill(ColourGradient::vertical(Colours::lightgrey, 0.0f, Colours::darkgrey, (float)getHeight()));
	//	g.fillRect(getWidth() - 100, 4, 100 - 4, getHeight() - 8);
	//	break;
	//case TextScale:
	//	break;
	//case ColourButtons:
	//	for (int i = 0; i < (int)buttonsColor.size(); ++i)
	//	{
	//		g.setColour(buttonsColor[i]);
	//		g.fillRect(getWidth() - 100, 4 + i * (getHeight() - 8) / buttonsColor.size(), 100 - 4, (getHeight() - 8) / (int)buttonsColor.size());
	//	}
	//	break;
	//case ColourScale:
	//	g.setGradientFill(ColourGradient::vertical(Colours::red, 0.0f, Colours::blue, (float)getHeight()));
	//	g.fillRect(getWidth() - 100, 4, 100 - 4, getHeight() - 8);
	//	break;
	//case ScaleMarking:
	//	g.setColour(Colours::white);
	//	g.fillRect(getWidth() - 50, 4, 1, getHeight() - 4);
	//	int markSpace = (getHeight() - 4) / numScaleMarking ;
	//	for (int i = 0; i < numScaleMarking; ++i)
	//	{
	//		g.fillRect(getWidth() - 100, 4 + markSpace/2 + i * markSpace, 100, 1);
	//	}
	//}

	g_testSideBarVertex_buffer_data[0] = (float)getWidth();
	g_testSideBarVertex_buffer_data[1] = (float)0.0f;
	g_testSideBarVertex_buffer_data[2] = (float)0.0f;
	g_testSideBarVertex_buffer_data[3] = (float)getWidth() - 150.0f;
	g_testSideBarVertex_buffer_data[4] = (float)0.0f;
	g_testSideBarVertex_buffer_data[5] = (float)0.0f;
	g_testSideBarVertex_buffer_data[6] = (float)getWidth() - 150.0f;
	g_testSideBarVertex_buffer_data[7] = (float)getHeight();
	g_testSideBarVertex_buffer_data[8] = (float)0.0f;
	g_testSideBarVertex_buffer_data[9] = (float)getWidth();
	g_testSideBarVertex_buffer_data[10] = (float)getHeight();
	g_testSideBarVertex_buffer_data[11] = (float)0.0f;

	g_testSideBarCoulour_buffer_data[0] = 1.0f;
	g_testSideBarCoulour_buffer_data[1] = 1.0f;
	g_testSideBarCoulour_buffer_data[2] = 1.0f;
	g_testSideBarCoulour_buffer_data[3] = 1.0f;
	g_testSideBarCoulour_buffer_data[4] = 1.0f;
	g_testSideBarCoulour_buffer_data[5] = 1.0f;
	g_testSideBarCoulour_buffer_data[6] = 1.0f;
	g_testSideBarCoulour_buffer_data[7] = 1.0f;
	g_testSideBarCoulour_buffer_data[8] = 1.0f;
	g_testSideBarCoulour_buffer_data[9] = 1.0f;
	g_testSideBarCoulour_buffer_data[10] = 1.0f;
	g_testSideBarCoulour_buffer_data[11] = 1.0f;
	g_testSideBarCoulour_buffer_data[12] = 1.0f;
	g_testSideBarCoulour_buffer_data[13] = 1.0f;
	g_testSideBarCoulour_buffer_data[14] = 1.0f;
	g_testSideBarCoulour_buffer_data[15] = 1.0f;

	g_testSideBarIndex_buffer_data[0] = 0;
	g_testSideBarIndex_buffer_data[1] = 1;
	g_testSideBarIndex_buffer_data[2] = 2;
	g_testSideBarIndex_buffer_data[3] = 3;
	g_testSideBarIndex_buffer_data[4] = 2;
	g_testSideBarIndex_buffer_data[5] = 0;


	openGlContext.extensions.glEnableVertexAttribArray(position->attributeID);
	openGlContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, testSideBarVertexBuffer);
	openGlContext.extensions.glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * 3 * sizeof(GLfloat), g_testSideBarVertex_buffer_data);
	openGlContext.extensions.glVertexAttribPointer(position->attributeID, 3, GL_FLOAT, GL_FALSE, sizeof(float[3]), 0);
	
	openGlContext.extensions.glEnableVertexAttribArray(colour->attributeID);
	openGlContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, testSideBarCoulourBuffer);
	openGlContext.extensions.glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * 4 * sizeof(GLfloat), g_testSideBarCoulour_buffer_data);
	openGlContext.extensions.glVertexAttribPointer(colour->attributeID, 4, GL_FLOAT, GL_FALSE, sizeof(float[4]), 0);

	openGlContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, testSideBarIndexBuffer);

	glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, (void*)0);

	openGlContext.extensions.glDisableVertexAttribArray(position->attributeID);
	openGlContext.extensions.glDisableVertexAttribArray(colour->attributeID);

	openGlContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, 0);
	openGlContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);




	//auto manager = canvasManager.lock();

	//const double desktopScale = openGlContext.getRenderingScale();
	//std::unique_ptr<LowLevelGraphicsContext> glRenderer(createOpenGLGraphicsContext(openGlContext,
	//	roundToInt(desktopScale * getWidth()),
	//	roundToInt(desktopScale * getHeight())));
	//Graphics g(*glRenderer);

	//g.addTransform(AffineTransform::scale((float)desktopScale));

	//// Pure black background
	//g.fillAll(Colours::black);

	////side bar if needed
	//switch (currentSideBarType)
	//{
	//case GrayScale:
	//	g.setGradientFill(ColourGradient::vertical(Colours::lightgrey, 0.0f, Colours::darkgrey, (float)getHeight()));
	//	g.fillRect(getWidth() - 100, 4, 100 - 4, getHeight() - 8);
	//	break;
	//case TextScale:
	//	break;
	//case ColourButtons:
	//	for (int i = 0; i < (int)buttonsColor.size(); ++i)
	//	{
	//		g.setColour(buttonsColor[i]);
	//		g.fillRect(getWidth() - 100, 4 + i * (getHeight() - 8) / buttonsColor.size(), 100 - 4, (getHeight() - 8) / (int)buttonsColor.size());
	//	}
	//	break;
	//case ColourScale:
	//	g.setGradientFill(ColourGradient::vertical(Colours::red, 0.0f, Colours::blue, (float)getHeight()));
	//	g.fillRect(getWidth() - 100, 4, 100 - 4, getHeight() - 8);
	//	break;
	//case ScaleMarking:
	//	g.setColour(Colours::white);
	//	g.fillRect(getWidth() - 50, 4, 1, getHeight() - 4);
	//	int markSpace = (getHeight() - 4) / numScaleMarking ;
	//	for (int i = 0; i < numScaleMarking; ++i)
	//	{
	//		g.fillRect(getWidth() - 100, 4 + markSpace/2 + i * markSpace, 100, 1);
	//	}
	//}

	//// White interior contour 2px line to show when the canvas is active
	//if (selectedForEditing)
	//{
	//	g.setColour(Colours::white);
	//	g.drawRect(1, 1, getWidth() - 2, getHeight() - 2, 2);
	//}


	//// - - - - - THIRD Duplication of the drawable objects for thread-safe rendering, - - - - -
	//// Lorsque n�cessaire seulement !
	//manager->LockAsyncDrawableObjects();


	//bool areasCountChanged = (manager->GetAsyncDrawableObjects().size() != duplicatedAreas.size());
	//// POUR L'INSTANT ALGO B�TE
	//// on resize le vecteur : la construction des shared n'est pas grave, leur bloc de contr�le reste
	//// en m�moire finalement (on utilisera reset() )
	//if (areasCountChanged)
	//{
	//	canvasAreasPointersCopies.resize(manager->GetAsyncDrawableObjects().size());
	//	duplicatedAreas.resize(manager->GetAsyncDrawableObjects().size());
	//}
	//// V�rification simple de chaque aire 1 par 1
	//size_t itIndex = 0;
	//for (auto it = manager->GetAsyncDrawableObjects().begin();
	//	it != manager->GetAsyncDrawableObjects().end();
	//	++it)
	//{
	//	// S'il y a eu un changement : on re-cr�e un pointeur d�j�, puis
	//	// on fait effectivement la copie d'un nouvel objet
	//	if (canvasAreasPointersCopies[itIndex] != (*it))
	//	{
	//		canvasAreasPointersCopies[itIndex] = (*it);
	//		duplicatedAreas[itIndex] = (*it)->Clone();
	//	}
	//	// Double compteur
	//	itIndex++;
	//}

	//manager->UnlockAsyncDrawableObjects();


	//// - - - - - Areas painting (including exciters if existing) - - - - -
	//// Sans bloquer, du coup, les autres threads (pour r�activit� max...)
	//for (size_t i = 0; i<duplicatedAreas.size(); i++)
	//{
	//	// Peut mettre � jour des images et autres (si l'�chelle a chang�)
	//	duplicatedAreas[i]->SetRenderingScale(desktopScale);
	//	// Dessin effectif
	//	duplicatedAreas[i]->Paint(g);
	//}


	//// Call to a general Graphic update on the whole Presenter module
	//if (!manager->isUpdatePending())
	//	manager->triggerAsyncUpdate();

	//// Time measures just before swap (or the closer that we can get to the swaps)
	//displayFrequencyMeasurer.OnNewFrame();
	///*
	//if (selectedForEditing)
	//{
	//if (displayFrequencyMeasurer.IsFreshAverageAvailable())
	//DBG(displayFrequencyMeasurer.GetInfo());
	//}
	//*/
	//// Forced sleep if drawing is too fast
	//double underTime = desiredPeriod_ms - displayFrequencyMeasurer.GetLastDuration_ms();
	//if (underTime > 0.0)
	//{
	//	Thread::sleep((int)std::floor(underTime));
	//}
	
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
			managerAsManager->ChangeVelocity(newVelocity * 127);
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

void AmusingSceneComponent::mouseDrag(const juce::MouseEvent & event)
{
	if (auto manager = std::dynamic_pointer_cast<Amusing::MultiSceneCanvasManager>(canvasManager.lock()))
		manager->OnCanvasMouseDrag(event);
}

void AmusingSceneComponent::mouseUp(const juce::MouseEvent& event)
{
	if (auto manager = std::dynamic_pointer_cast<Amusing::MultiSceneCanvasManager>(canvasManager.lock()))
		manager->OnCanvasMouseUp(event);
}

void AmusingSceneComponent::ShowSideBar(SideBarType sideBarType)
{
	currentSideBarType = sideBarType;
}
