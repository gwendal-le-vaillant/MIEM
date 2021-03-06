/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.3.2

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"



namespace Amusing
{
	class GraphicSessionManager;

}
using namespace Amusing;

class CustomButtonLookAndFeel : public LookAndFeel_V4
{
	//void drawButtonBackground(Graphics &, Button &, const Colour &backgroundColour, bool isMouseOverButton, bool isButtonDown) override = 0;
	Font getTextButtonFont(TextButton&, int buttonHeight) override
	{
		return Font(20.0f);
	}
	Label* createSliderTextBox(Slider &currentSlider) override
	{
		Label *currentLabel = LookAndFeel_V4::createSliderTextBox(currentSlider);
		currentLabel->setLookAndFeel(this);
		return currentLabel;
	}
	Font getLabelFont(Label &) override
	{
		return Font(28.0f);
	}
	Slider::SliderLayout getSliderLayout(Slider &currentSlider) override
	{
		Slider::SliderLayout currentSliderLayout = LookAndFeel_V4::getSliderLayout(currentSlider);
		if (currentSliderLayout.sliderBounds.getHeight() > currentSliderLayout.textBoxBounds.getHeight())
		{
			int total = currentSliderLayout.sliderBounds.getHeight() + currentSliderLayout.textBoxBounds.getHeight();
			currentSliderLayout.sliderBounds.setHeight(total / 2);
			currentSliderLayout.textBoxBounds.setHeight(total / 2);
			if (currentSliderLayout.sliderBounds.getY() < currentSliderLayout.textBoxBounds.getY())
				currentSliderLayout.textBoxBounds.setY(currentSliderLayout.sliderBounds.getY() + total / 2);
			else
				currentSliderLayout.sliderBounds.setY(currentSliderLayout.textBoxBounds.getY() + total / 2);
		}
		return currentSliderLayout;
	}
};

//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class EditScene  : public Component,
                   public Slider::Listener,
                   public Button::Listener
{
public:
    //==============================================================================
    EditScene ();
    ~EditScene();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void CompleteInitialization(GraphicSessionManager* _graphicSessionManager);//, MultiCanvasComponent* _multiCanvasComponent);
	void setMidiChannel(int chan);
	void hideAddPolygon();
	void showAddPolygon();
	void setTempoSlider(int newTempo);
	void buttonStateChanged(Button *concernedButton) override;
	void mouseUp(const MouseEvent& e) override;
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    void mouseExit (const MouseEvent& e) override;
    void mouseDrag (const MouseEvent& e) override;

    // Binary resources:
    static const char* stopOn_png;
    static const int stopOn_pngSize;
    static const char* pauseOn_png;
    static const int pauseOn_pngSize;
    static const char* optionOn_png;
    static const int optionOn_pngSize;
    static const char* lectureOn_png;
    static const int lectureOn_pngSize;
    static const char* deleteOn_png;
    static const int deleteOn_pngSize;
    static const char* lecture_png;
    static const int lecture_pngSize;
    static const char* pause_png;
    static const int pause_pngSize;
    static const char* stop_png;
    static const int stop_pngSize;
    static const char* delete_png;
    static const int delete_pngSize;
    static const char* option_png;
    static const int option_pngSize;


private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    GraphicSessionManager* graphicSessionManager = 0;
	bool isAddEnabled;
	bool saveFileExists;
	int prepareToAdd; // 0 = no shape to add, 1 = square, 2 = triangle, 3 = hexa
	bool hasExited;
	Point<int> exitPosition; // position où la sourie a quitté le menu pour entrer dans le canvas
	CustomButtonLookAndFeel customLAF;
	//MultiCanvasComponent* multiCanvasComponent = 0;

    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<GroupComponent> groupComponent;
    std::unique_ptr<Slider> timeSlider;
    std::unique_ptr<ImageButton> imgPlayButton;
    std::unique_ptr<ImageButton> imgStopButton;
    std::unique_ptr<ImageButton> imgPauseButton;
    std::unique_ptr<ImageButton> imgOptionButton;
    std::unique_ptr<ImageButton> imgDeleteButton;
    std::unique_ptr<TextButton> textButton;
    std::unique_ptr<Label> label;
    std::unique_ptr<ShapeButton> addCarreShapeButton;
    std::unique_ptr<ShapeButton> addTriangleShapeButton2;
    std::unique_ptr<ShapeButton> addHexaShapeButton;
    std::unique_ptr<TextButton> saveButton;
    std::unique_ptr<TextButton> loadButton;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EditScene)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
