/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.3

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include "PlayerPresenter.h"
#include "PlayerView.h"

#include "MultiCanvasComponent.h"
//[/Headers]

#include "PlayerBackgroundComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
using namespace Miam;
//[/MiscUserDefs]

//==============================================================================
PlayerBackgroundComponent::PlayerBackgroundComponent ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    multiCanvasComponent = nullptr; // n'arrivera que + tard dans le code...
    //[/Constructor_pre]

    mainInfoLabel.reset (new Label ("Main Info label",
                                    TRANS("MIEM Spat Player")));
    addAndMakeVisible (mainInfoLabel.get());
    mainInfoLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    mainInfoLabel->setJustificationType (Justification::centredLeft);
    mainInfoLabel->setEditable (false, false, false);
    mainInfoLabel->setColour (Label::textColourId, Colour (0xff909090));
    mainInfoLabel->setColour (TextEditor::textColourId, Colours::black);
    mainInfoLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    mainMenuImageButton.reset (new ImageButton ("Main Menu image button"));
    addAndMakeVisible (mainMenuImageButton.get());
    mainMenuImageButton->setButtonText (TRANS("new button"));
    mainMenuImageButton->addListener (this);

    mainMenuImageButton->setImages (false, true, true,
                                    ImageCache::getFromMemory (menu_icon_png2, menu_icon_png2Size), 1.000f, Colour (0x00000000),
                                    Image(), 1.000f, Colour (0x00000000),
                                    ImageCache::getFromMemory (menu_activated_icon_png2, menu_activated_icon_png2Size), 1.000f, Colour (0x00000000));
    mainInfoLabel2.reset (new Label ("Main Info label 2",
                                     TRANS("[complementary information]")));
    addAndMakeVisible (mainInfoLabel2.get());
    mainInfoLabel2->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    mainInfoLabel2->setJustificationType (Justification::centredTop);
    mainInfoLabel2->setEditable (false, false, false);
    mainInfoLabel2->setColour (Label::textColourId, Colour (0xff909090));
    mainInfoLabel2->setColour (TextEditor::textColourId, Colours::black);
    mainInfoLabel2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    mainSlider.reset (new Slider ("Main slider"));
    addAndMakeVisible (mainSlider.get());
    mainSlider->setTooltip (TRANS("Master Gain (applied to the entire state)"));
    mainSlider->setRange (-60, 6, 0);
    mainSlider->setSliderStyle (Slider::LinearVertical);
    mainSlider->setTextBoxStyle (Slider::TextBoxAbove, true, 40, 20);
    mainSlider->setColour (Slider::backgroundColourId, Colours::black);
    mainSlider->setColour (Slider::thumbColourId, Colours::white);
    mainSlider->setColour (Slider::trackColourId, Colour (0xff909090));
    mainSlider->addListener (this);

    masterGainLabel.reset (new Label ("Master Gain label",
                                      TRANS("Master\n"
                                      "Gain\n"
                                      "dB FS")));
    addAndMakeVisible (masterGainLabel.get());
    masterGainLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    masterGainLabel->setJustificationType (Justification::centred);
    masterGainLabel->setEditable (false, false, false);
    masterGainLabel->setColour (Label::textColourId, Colour (0xff909090));
    masterGainLabel->setColour (TextEditor::textColourId, Colours::black);
    masterGainLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));


    //[UserPreSize]
    mainMenuComponent.reset( new PlayerMainMenuComponent() );
    addChildComponent(mainMenuComponent.get());
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
	mainInfoLabelDefaultTextColour = mainInfoLabel->findColour(Label::textColourId);
	mainInfoLabelDefaultFont = mainInfoLabel->getFont();
    mainInfoLabel->setText(App::GetNameWithVersion(), NotificationType::sendNotification);

    // Slider default behavior
    //isMainSliderEnabled = true;
    mainSlider->setNumDecimalPlacesToDisplay(0);

    //[/Constructor]
}

PlayerBackgroundComponent::~PlayerBackgroundComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    mainMenuComponent = nullptr;
    //[/Destructor_pre]

    mainInfoLabel = nullptr;
    mainMenuImageButton = nullptr;
    mainInfoLabel2 = nullptr;
    mainSlider = nullptr;
    masterGainLabel = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void PlayerBackgroundComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff303030));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void PlayerBackgroundComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    mainInfoLabel->setBounds (8, getHeight() - 4 - 20, 384, 20);
    mainMenuImageButton->setBounds (getWidth() - 28, getHeight() - 28, 28, 28);
    mainInfoLabel2->setBounds (400, getHeight() - 4 - 20, getWidth() - 432, 20);
    mainSlider->setBounds (getWidth() - 56, (getHeight() / 2) + 24 - ((getHeight() - 160) / 2), 56, getHeight() - 160);
    masterGainLabel->setBounds (getWidth() - 58, ((getHeight() / 2) + 24 - ((getHeight() - 160) / 2)) + -56, roundToInt (56 * 1.0000f), 48);
    //[UserResized] Add your own custom resize handling here..

    // Attention : remove from machin retourne le petit morceau découpé seulement !
    auto menusRectangle = getLocalBounds();
    menusRectangle.removeFromBottom(mainMenuImageButton->getHeight());
    // Application : on laisse la place pour la barre de titre
    mainMenuComponent->setBounds(menusRectangle);
    // SI NECESSAIRE : slider à droite pour le MASTER GAIN
    // Avec une hauteur maximale (sinon trop de distance à parcourir au doigt)
    if (isMainSliderEnabled)
    {
        // Slider itself
        if (mainSlider->getHeight() > mainSliderMaxHeight)
        {
            mainSlider->setSize(mainSlider->getWidth(), mainSliderMaxHeight);
            // Positionned on the RIGHT border
            mainSlider->setTopRightPosition(getWidth(), // int computation... OK.
                                            getHeight()/2 - mainSliderMaxHeight/2 + 12);
        }
        // Slider label
#ifdef JUCE_ANDROID
        // Additional Width for Android (or text is on 4 lines...)
        int previousGainLabelW = masterGainLabel->getWidth();
        masterGainLabel->setBounds(masterGainLabel->getBounds().withWidth(previousGainLabelW + 4));
#endif
        masterGainLabel->setTopRightPosition(getWidth(),
                                             mainSlider->getPosition().y - masterGainLabel->getHeight() - 8);
    }
    // Rectangle + grand pour les canevas
    if (multiCanvasComponent)
    {
        auto canvasesRectangle = getLocalBounds();
        canvasesRectangle.removeFromBottom(mainInfoLabel->getHeight());
        if (isMainSliderEnabled)
            canvasesRectangle.removeFromRight(mainSlider->getWidth());
        multiCanvasComponent->setBounds(canvasesRectangle);
    }

    // For really small screens : we adjust data... and don't display the 2nd label
//#if defined(JUCE_IOS)
    // Convient pour l'iPad pro du LARAS (petite largeur 768px)
    const int smallScreenWidthLimit = 800; // ne doit prendre en compte le rétina
    /*
#else
    const int smallScreenWidthLimit = 600;
#endif
     */
    if (getWidth() < smallScreenWidthLimit)
    {
        Rectangle<int> bounds2 = mainInfoLabel2->getBounds();
        bounds2.setWidth(1);
        mainInfoLabel2->setBounds(bounds2); // pas trop de bug si on ne laisse qu'un seul px ?

        Rectangle<int> bounds1 = mainInfoLabel->getBounds();
        bounds1.setWidth(getWidth() - 8 - 8 - 28); // -36 pour le bouton de menu
        mainInfoLabel->setBounds(bounds1);
    }

    //[/UserResized]
}

void PlayerBackgroundComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == mainMenuImageButton.get())
    {
        //[UserButtonCode_mainMenuImageButton] -- add your button handler code here..
        presenter->OnMainMenuButtonClicked();
        //[/UserButtonCode_mainMenuImageButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void PlayerBackgroundComponent::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == mainSlider.get())
    {
        //[UserSliderCode_mainSlider] -- add your slider handling code here..
        auto sliderValue = mainSlider->getValue();
        if (sliderValue < -59.5) // min à -60dB considéré -inf
            mainSlider->setColour(Slider::ColourIds::textBoxTextColourId, Colours::transparentBlack);
        else
            mainSlider->setColour(Slider::ColourIds::textBoxTextColourId, Colours::white);
        // Value is sent to presenter, if exists
        if (presenter)
            presenter->OnMainSliderValueChanged_dB(sliderValue);
        else
            assert(false); // the presenter should be set at this point... Don't force-move the slider before !
        //[/UserSliderCode_mainSlider]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void PlayerBackgroundComponent::CompleteInitialization(PlayerPresenter* presenter_, PlayerView* _view)
{
    presenter = presenter_;
    view = _view;
    mainMenuComponent->SetPresenter(presenter);
}
void PlayerBackgroundComponent::CompleteInitialization(MultiCanvasComponent* multiCanvasComponent_)
{
    multiCanvasComponent = multiCanvasComponent_;
    addAndMakeVisible(multiCanvasComponent);

    multiCanvasComponent->CompleteInitialization();
}
void PlayerBackgroundComponent::DisplayInfo(const String& stringToDisplay, bool isImportant)
{
    mainInfoLabel->setText(stringToDisplay, NotificationType::sendNotification);
	if (!isImportant)
	{
		mainInfoLabel->setColour(Label::textColourId, mainInfoLabelDefaultTextColour);
		mainInfoLabel->setFont(mainInfoLabelDefaultFont);
	}
	else
	{
		auto boldenedFont = mainInfoLabelDefaultFont;
		boldenedFont.setBold(true);
		mainInfoLabel->setColour(Label::textColourId, Colours::indianred);
		mainInfoLabel->setFont(boldenedFont);
	}
}
void PlayerBackgroundComponent::DisplayInfo2(const String& stringToDisplay)
{
    mainInfoLabel2->setText(stringToDisplay, NotificationType::sendNotification);
}

void PlayerBackgroundComponent::ChangeAppMode(PlayerAppMode newAppMode)
{
    switch (newAppMode) {
        case PlayerAppMode::Null:
            //
            break;
        case PlayerAppMode::None:
            //
            break;
        case PlayerAppMode::Loading:
            //
            break;
        case PlayerAppMode::MainMenu:
            mainInfoLabel->setVisible(true);
            multiCanvasComponent->setVisible(false);
            mainMenuComponent->setVisible(true);
            mainSlider->setVisible(false);
            masterGainLabel->setVisible(false);
            break;
        case PlayerAppMode::LoadingFile:
            multiCanvasComponent->setVisible(false);
            mainMenuComponent->setVisible(false);
            mainSlider->setVisible(false);
            masterGainLabel->setVisible(false);
            // Le FileChooser est géré pour l'instant directement par le Presenter...
            break;


        case PlayerAppMode::Stopped:
            mainInfoLabel->setVisible(true);
            multiCanvasComponent->setVisible(false);
            mainMenuComponent->setVisible(true);
            mainSlider->setVisible(false);
            masterGainLabel->setVisible(false);
            break;
        case PlayerAppMode::Playing:
            // On laisse un délai, pour que le menu ait le temps d'afficher l'état "play" en vert clignotant
            mainMenuComponent->PrepareToPlay(1000);
            Timer::callAfterDelay(1000,
                                  [this]
                                  {
                                      mainMenuComponent->setVisible(false);
                                      // !! VBO/texture are only created after this
                                      // (on GL context creation)
                                      multiCanvasComponent->setVisible(true);
                                      mainSlider->setVisible(isMainSliderEnabled);
                                      masterGainLabel->setVisible(isMainSliderEnabled);
                                      presenter->OnViewIsPreparingToPlay(false);
                                  });
#ifdef __MIEM_EXPERIMENTS
            mainInfoLabel->setVisible(false);
#endif
            break;

        default:
            break;
    }

    // Dans tous les cas : transmission au menu (qui affiche des choses en fonction)
    mainMenuComponent->ChangeAppMode(newAppMode);
}
void PlayerBackgroundComponent::SetMainSliderEnabled(bool shouldBeEnabled)
{
    isMainSliderEnabled = shouldBeEnabled;
    resized();
}
void PlayerBackgroundComponent::SetMainSliderVolume_dB(double volume_dB)
{
	switch (presenter->GetSessionPurpose())
	{
		case AppPurpose::GenericController :
			// GenCon sessions are not supposed to use the main slider....
			assert(false);
			return;

		case AppPurpose::Spatialisation :
			mainSlider->setValue(volume_dB, NotificationType::sendNotificationAsync);
			break;

        default:
            break;
	}
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="PlayerBackgroundComponent"
                 componentName="" parentClasses="public Component" constructorParams=""
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff303030"/>
  <LABEL name="Main Info label" id="346700aa23dd510d" memberName="mainInfoLabel"
         virtualName="" explicitFocusOrder="0" pos="8 4Rr 384 20" textCol="ff909090"
         edTextCol="ff000000" edBkgCol="0" labelText="MIEM Spat Player"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="33"/>
  <IMAGEBUTTON name="Main Menu image button" id="83c438e933714a80" memberName="mainMenuImageButton"
               virtualName="" explicitFocusOrder="0" pos="0Rr 0Rr 28 28" buttonText="new button"
               connectedEdges="0" needsCallback="1" radioGroupId="0" keepProportions="1"
               resourceNormal="menu_icon_png2" opacityNormal="1.0" colourNormal="0"
               resourceOver="" opacityOver="1.0" colourOver="0" resourceDown="menu_activated_icon_png2"
               opacityDown="1.0" colourDown="0"/>
  <LABEL name="Main Info label 2" id="c1d25bb923263634" memberName="mainInfoLabel2"
         virtualName="" explicitFocusOrder="0" pos="400 4Rr 432M 20" textCol="ff909090"
         edTextCol="ff000000" edBkgCol="0" labelText="[complementary information]"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="12"/>
  <SLIDER name="Main slider" id="8924a1c10657725b" memberName="mainSlider"
          virtualName="" explicitFocusOrder="0" pos="0Rr 24.5Cc 56 160M"
          tooltip="Master Gain (applied to the entire state)" bkgcol="ff000000"
          thumbcol="ffffffff" trackcol="ff909090" min="-60.0" max="6.0"
          int="0.0" style="LinearVertical" textBoxPos="TextBoxAbove" textBoxEditable="0"
          textBoxWidth="40" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <LABEL name="Master Gain label" id="398893cc183b859b" memberName="masterGainLabel"
         virtualName="" explicitFocusOrder="0" pos="58R -56 100% 48" posRelativeY="8924a1c10657725b"
         posRelativeW="8924a1c10657725b" textCol="ff909090" edTextCol="ff000000"
         edBkgCol="0" labelText="Master&#10;Gain&#10;dB FS" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="36"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif

//==============================================================================
// Binary resources - be careful not to edit any of these sections!

// JUCER_RESOURCE: menu_icon_png, 16232, "../../SpatCommon/Resources/Menu_icon.png"
static const unsigned char resource_PlayerBackgroundComponent_menu_icon_png[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,1,98,0,0,1,98,8,6,0,0,0,204,92,210,122,0,0,0,1,115,82,71,66,0,174,206,
28,233,0,0,0,9,112,72,89,115,0,0,46,35,0,0,46,35,1,120,165,63,118,0,0,3,170,105,84,88,116,88,77,76,58,99,111,109,46,97,100,111,98,101,46,120,109,112,0,0,0,0,0,60,120,58,120,109,112,109,101,116,97,32,120,
109,108,110,115,58,120,61,34,97,100,111,98,101,58,110,115,58,109,101,116,97,47,34,32,120,58,120,109,112,116,107,61,34,88,77,80,32,67,111,114,101,32,53,46,52,46,48,34,62,10,32,32,32,60,114,100,102,58,82,
68,70,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,47,50,50,45,114,100,102,45,115,121,110,116,97,120,45,110,115,35,
34,62,10,32,32,32,32,32,32,60,114,100,102,58,68,101,115,99,114,105,112,116,105,111,110,32,114,100,102,58,97,98,111,117,116,61,34,34,10,32,32,32,32,32,32,32,32,32,32,32,32,120,109,108,110,115,58,120,109,
112,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,34,10,32,32,32,32,32,32,32,32,32,32,32,32,120,109,108,110,115,58,116,105,102,102,61,34,104,116,
116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,116,105,102,102,47,49,46,48,47,34,10,32,32,32,32,32,32,32,32,32,32,32,32,120,109,108,110,115,58,101,120,105,102,61,34,104,116,116,112,58,
47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,101,120,105,102,47,49,46,48,47,34,62,10,32,32,32,32,32,32,32,32,32,60,120,109,112,58,77,111,100,105,102,121,68,97,116,101,62,50,48,49,55,45,49,48,45,
49,50,84,50,48,58,49,48,58,54,52,60,47,120,109,112,58,77,111,100,105,102,121,68,97,116,101,62,10,32,32,32,32,32,32,32,32,32,60,120,109,112,58,67,114,101,97,116,111,114,84,111,111,108,62,80,105,120,101,
108,109,97,116,111,114,32,51,46,55,60,47,120,109,112,58,67,114,101,97,116,111,114,84,111,111,108,62,10,32,32,32,32,32,32,32,32,32,60,116,105,102,102,58,79,114,105,101,110,116,97,116,105,111,110,62,49,
60,47,116,105,102,102,58,79,114,105,101,110,116,97,116,105,111,110,62,10,32,32,32,32,32,32,32,32,32,60,116,105,102,102,58,67,111,109,112,114,101,115,115,105,111,110,62,53,60,47,116,105,102,102,58,67,111,
109,112,114,101,115,115,105,111,110,62,10,32,32,32,32,32,32,32,32,32,60,116,105,102,102,58,82,101,115,111,108,117,116,105,111,110,85,110,105,116,62,50,60,47,116,105,102,102,58,82,101,115,111,108,117,116,
105,111,110,85,110,105,116,62,10,32,32,32,32,32,32,32,32,32,60,116,105,102,102,58,89,82,101,115,111,108,117,116,105,111,110,62,51,48,48,60,47,116,105,102,102,58,89,82,101,115,111,108,117,116,105,111,110,
62,10,32,32,32,32,32,32,32,32,32,60,116,105,102,102,58,88,82,101,115,111,108,117,116,105,111,110,62,51,48,48,60,47,116,105,102,102,58,88,82,101,115,111,108,117,116,105,111,110,62,10,32,32,32,32,32,32,
32,32,32,60,101,120,105,102,58,80,105,120,101,108,88,68,105,109,101,110,115,105,111,110,62,51,53,52,60,47,101,120,105,102,58,80,105,120,101,108,88,68,105,109,101,110,115,105,111,110,62,10,32,32,32,32,
32,32,32,32,32,60,101,120,105,102,58,67,111,108,111,114,83,112,97,99,101,62,49,60,47,101,120,105,102,58,67,111,108,111,114,83,112,97,99,101,62,10,32,32,32,32,32,32,32,32,32,60,101,120,105,102,58,80,105,
120,101,108,89,68,105,109,101,110,115,105,111,110,62,51,53,52,60,47,101,120,105,102,58,80,105,120,101,108,89,68,105,109,101,110,115,105,111,110,62,10,32,32,32,32,32,32,60,47,114,100,102,58,68,101,115,
99,114,105,112,116,105,111,110,62,10,32,32,32,60,47,114,100,102,58,82,68,70,62,10,60,47,120,58,120,109,112,109,101,116,97,62,10,253,200,34,124,0,0,59,87,73,68,65,84,120,1,237,157,107,176,157,213,121,223,
143,208,209,209,205,18,72,66,2,221,144,4,226,98,33,48,96,144,32,132,142,19,40,157,241,56,113,28,199,105,242,33,73,59,137,157,15,77,47,147,116,50,238,164,245,135,164,153,56,153,38,159,210,105,99,39,211,
73,50,211,92,60,46,177,147,113,198,20,98,79,136,227,34,48,22,8,48,96,4,18,186,128,46,150,108,17,221,142,36,212,245,223,156,159,120,180,88,239,190,156,115,246,218,251,200,255,119,102,157,231,89,207,90,
239,101,253,182,223,31,175,95,109,29,141,140,120,51,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,
48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,
48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,
48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,152,22,2,179,166,229,40,253,63,200,232,198,141,27,239,186,234,170,171,182,46,92,184,112,221,188,121,243,174,157,63,127,254,
245,115,231,206,189,238,178,203,46,27,75,109,100,214,172,89,23,53,93,146,106,49,182,58,161,78,223,209,4,76,96,120,8,156,63,127,254,194,197,144,199,168,156,246,214,91,111,141,164,54,126,250,244,233,157,
39,79,158,252,246,169,83,167,94,57,126,252,248,238,3,7,14,60,254,242,203,47,63,145,14,116,246,194,193,134,56,25,102,17,95,182,126,253,250,91,215,173,91,247,177,36,224,143,143,141,141,45,151,112,103,207,
158,61,162,72,147,108,45,226,33,254,95,152,47,205,4,122,36,128,116,181,27,121,140,72,88,81,34,38,78,72,121,228,220,185,115,173,250,248,248,248,161,36,228,207,238,222,189,251,115,187,118,237,122,38,29,
238,173,30,47,165,218,244,161,20,241,218,181,107,55,223,118,219,109,159,89,180,104,209,61,18,111,108,8,88,181,40,225,40,99,209,227,9,153,60,18,229,73,57,214,156,155,128,9,12,158,0,194,229,74,232,43,230,
121,148,176,198,16,176,234,202,99,123,243,205,55,191,190,125,251,246,79,236,217,179,231,89,142,61,76,113,168,68,188,116,233,210,197,183,220,114,203,175,173,90,181,234,87,71,71,71,71,212,162,132,201,145,
49,17,233,150,34,176,115,249,230,125,230,57,154,128,9,12,134,0,162,229,236,177,175,188,169,241,36,76,140,2,38,63,123,246,236,136,218,254,253,251,127,103,199,142,29,191,121,228,200,145,99,156,103,24,226,
208,136,120,195,134,13,91,222,255,254,247,255,117,122,239,187,98,206,156,57,45,9,35,98,162,68,140,124,201,37,212,248,52,140,140,5,55,230,244,59,65,183,160,59,17,242,184,9,76,141,64,20,108,211,145,226,
28,4,172,185,228,196,248,84,44,233,230,50,150,124,85,71,196,138,233,61,242,193,167,158,122,234,67,175,188,242,138,222,33,15,197,54,20,34,222,180,105,211,253,183,222,122,235,151,146,128,199,162,132,121,
42,206,69,140,132,17,48,17,241,18,69,24,177,18,115,234,77,245,124,158,251,38,96,2,253,33,16,165,27,207,64,61,70,4,76,140,34,86,142,140,227,147,176,242,51,103,206,92,144,177,242,212,198,159,121,230,153,
15,62,255,252,243,143,198,115,14,42,31,184,136,147,128,63,122,243,205,55,127,46,253,97,220,44,9,87,34,206,101,44,241,34,99,73,119,50,34,22,224,118,210,109,55,54,168,15,199,231,53,129,75,157,0,146,205,
215,25,235,228,200,55,198,118,34,46,61,13,79,72,184,37,230,148,159,127,238,185,231,62,150,132,252,249,252,252,181,251,3,21,241,251,222,247,190,159,76,18,254,11,228,155,100,220,18,46,125,158,136,37,222,
216,144,177,228,169,188,244,68,44,144,200,149,8,220,188,79,221,209,4,76,96,176,4,144,46,87,65,63,198,38,17,231,79,195,241,169,88,82,150,132,137,233,27,21,173,190,98,122,42,254,151,79,63,253,244,95,114,
206,65,196,209,65,156,84,231,92,189,122,245,13,239,125,239,123,255,84,210,149,128,145,111,140,81,196,189,62,17,235,28,8,151,168,26,91,169,198,152,163,9,152,192,96,8,32,92,206,78,63,198,38,17,231,175,38,
120,34,206,31,214,56,54,81,30,58,124,248,240,246,125,251,246,189,68,173,118,28,148,136,23,108,217,178,229,243,73,192,173,119,194,200,55,10,89,226,85,29,1,19,121,26,230,41,56,143,2,40,201,34,90,34,245,
201,2,142,199,153,236,49,188,159,9,124,191,16,64,156,147,93,111,220,159,60,70,229,241,181,4,125,158,138,37,97,185,65,125,28,129,23,242,123,57,237,59,38,31,61,244,208,67,91,211,245,158,152,236,53,79,101,
191,129,136,248,129,7,30,248,237,247,188,231,61,155,17,176,98,148,48,117,201,151,150,191,154,16,92,0,19,1,45,32,192,38,2,41,239,83,119,52,1,19,24,30,2,72,87,87,68,30,163,114,181,40,99,229,242,4,242,85,
148,144,241,66,233,222,231,56,233,111,236,110,150,151,30,121,228,145,127,59,8,10,213,69,124,221,117,215,253,64,250,155,114,191,132,96,145,46,81,66,214,152,250,204,81,68,196,146,174,242,40,226,8,154,92,
48,1,79,44,1,110,55,86,154,239,154,9,152,192,244,18,64,176,165,163,50,22,35,242,140,81,18,86,67,194,237,252,192,121,216,159,125,229,165,228,167,63,219,185,115,231,63,50,167,86,172,46,226,244,85,181,95,
71,176,200,182,20,169,49,55,138,24,200,138,18,41,17,169,54,197,110,160,178,111,55,115,61,199,4,76,96,114,4,16,107,167,189,227,60,196,169,125,200,37,81,229,200,84,46,80,142,35,116,63,211,226,126,113,31,
246,85,148,159,146,136,31,232,116,93,211,61,94,85,196,233,191,54,239,191,252,242,203,239,71,174,138,8,87,49,54,198,36,96,229,237,68,12,232,24,1,165,90,220,242,126,28,115,110,2,38,48,120,2,146,100,220,
98,95,121,108,18,174,250,81,166,202,113,129,142,83,186,231,227,49,52,95,79,210,138,242,147,60,149,100,252,141,120,13,253,206,171,138,56,253,233,228,167,144,170,98,55,45,138,88,208,213,143,255,181,83,14,
116,128,231,49,66,100,44,214,156,155,128,9,12,15,1,73,50,223,168,197,136,76,37,80,121,64,17,169,226,4,29,71,121,190,31,115,153,47,17,171,201,73,242,84,18,241,135,243,107,232,103,191,154,136,245,117,181,
43,174,184,226,71,17,43,18,206,251,212,21,25,83,164,33,97,4,76,68,176,241,3,224,67,104,2,200,62,77,227,174,155,128,9,212,33,128,40,75,103,139,99,200,87,243,200,17,45,247,190,162,164,26,55,142,193,62,136,
24,249,34,98,69,121,74,190,170,249,117,182,106,34,78,191,206,242,126,100,26,35,194,141,53,229,8,57,175,231,34,142,240,201,249,0,212,47,109,77,245,210,92,215,76,192,4,234,18,64,154,241,172,177,134,76,137,
186,159,149,115,255,115,127,19,57,142,230,32,96,188,34,207,72,190,244,137,242,213,37,41,226,229,203,151,255,24,139,84,140,162,205,243,56,47,207,17,177,32,79,246,105,56,255,128,248,160,28,77,192,4,6,75,
0,161,230,87,161,58,155,114,250,228,18,172,238,107,26,115,229,15,109,154,167,57,68,57,71,125,36,172,190,190,234,134,111,146,175,244,106,226,127,180,118,174,240,163,202,19,113,90,212,123,22,47,94,124,63,
139,84,68,168,228,140,169,78,141,60,142,177,95,147,136,197,44,138,54,230,21,120,250,20,38,96,2,83,32,80,186,95,37,79,234,228,138,218,20,99,206,169,169,49,7,151,72,190,242,137,4,76,13,223,224,25,197,228,
171,7,228,173,67,135,14,253,19,199,236,103,172,34,226,244,253,188,59,211,162,103,35,209,184,96,242,8,133,154,34,251,228,81,31,76,222,4,138,15,140,216,4,175,211,120,211,126,174,155,128,9,244,143,64,20,
40,103,225,94,213,152,242,24,153,35,63,148,246,85,77,30,81,84,147,136,145,113,244,76,204,39,92,52,91,222,74,34,254,42,231,232,103,172,34,226,101,203,150,221,133,84,137,81,172,64,136,181,60,215,7,64,141,
92,145,38,72,202,217,98,78,77,177,169,30,231,56,55,1,19,24,12,1,238,207,146,84,53,166,122,30,163,100,229,8,109,170,41,103,12,119,244,18,175,188,242,202,59,211,161,190,170,227,245,123,171,34,226,244,15,
125,174,204,1,244,34,95,129,215,254,138,121,78,77,160,148,179,197,156,26,177,221,24,115,28,77,192,4,234,19,144,56,181,149,238,81,141,149,234,92,165,28,161,167,93,205,81,174,77,251,168,150,251,71,125,30,
10,155,92,148,254,145,226,85,28,187,223,177,138,136,211,130,86,3,7,32,234,83,139,185,198,99,63,159,195,88,30,5,74,53,182,152,83,115,52,1,19,24,110,2,83,189,111,115,47,168,47,167,68,65,71,7,229,190,137,
99,242,86,45,90,85,68,156,126,127,196,218,184,64,242,60,2,145,58,253,78,81,176,52,39,198,86,103,226,7,99,177,230,220,4,76,96,248,9,240,132,172,43,229,62,230,201,152,24,87,209,228,138,220,41,244,219,69,
121,43,30,187,159,121,45,17,175,3,144,22,174,28,0,228,68,198,153,95,138,2,66,157,60,198,60,87,223,155,9,152,192,204,35,160,251,156,13,41,171,134,132,137,154,67,142,27,74,49,122,135,241,82,77,99,233,137,
120,29,231,238,119,172,37,226,85,81,176,165,60,214,0,212,46,2,70,115,180,229,145,241,118,145,125,218,205,241,152,9,152,64,255,9,32,217,118,103,226,126,69,184,49,106,63,198,21,187,105,185,128,113,16,245,
244,238,120,101,187,235,153,206,177,42,34,238,6,74,183,115,0,206,124,250,49,70,64,154,231,205,4,76,96,184,9,52,221,167,37,65,107,110,148,112,220,151,186,106,83,109,73,200,213,228,241,246,31,45,246,249,
51,106,7,68,167,6,100,41,82,227,18,155,250,165,122,94,227,24,142,38,96,2,51,131,128,238,225,210,125,76,141,200,106,74,125,106,49,42,47,245,169,19,57,110,191,99,21,17,179,136,184,56,114,69,109,77,145,49,
198,91,147,59,252,232,101,110,135,67,121,216,4,76,96,8,8,232,158,238,229,190,206,231,179,111,41,198,185,140,107,201,49,239,55,130,42,34,102,161,44,140,200,98,155,198,227,60,230,2,36,142,197,156,113,71,
19,48,129,75,155,64,188,239,149,211,39,178,122,250,165,72,77,115,149,199,198,254,53,98,21,17,199,133,176,112,22,28,199,148,199,241,216,143,243,242,57,165,177,88,115,110,2,38,112,105,16,224,222,143,171,
41,213,24,103,172,41,106,158,198,24,103,63,234,177,223,207,188,138,136,243,133,230,139,206,251,237,22,220,203,220,118,199,241,152,9,152,192,165,71,96,42,126,96,95,69,242,90,132,170,136,152,197,116,90,
92,167,113,142,227,104,2,38,96,2,83,37,48,76,190,169,42,226,169,130,243,254,38,96,2,38,112,41,18,168,34,98,190,11,72,204,65,82,39,230,227,238,155,128,9,152,192,116,19,104,242,77,83,125,186,207,31,143,
87,69,196,58,97,92,92,204,227,197,116,147,79,101,223,110,142,239,57,38,96,2,51,151,192,84,252,16,247,141,121,13,26,213,68,204,98,88,160,34,185,198,200,155,34,251,151,230,150,198,98,205,185,9,152,192,165,
65,0,63,196,213,80,35,182,27,99,14,81,115,149,211,39,82,143,199,234,103,94,69,196,44,148,69,198,200,88,92,120,28,47,213,99,45,207,213,247,102,2,38,112,233,19,192,19,90,105,83,30,199,152,19,163,242,216,
103,62,53,245,107,108,213,69,204,194,89,104,55,125,129,96,126,55,80,56,102,55,115,61,199,4,76,96,248,9,244,122,79,231,243,233,151,162,86,175,58,49,230,173,98,133,31,85,68,28,23,24,23,9,148,210,56,181,
60,150,246,137,115,148,179,197,185,212,28,77,192,4,102,14,129,166,123,88,117,109,113,156,60,31,163,207,124,98,156,223,148,107,110,141,173,202,111,95,75,191,29,255,108,90,232,40,139,213,111,203,39,87,204,
251,113,76,185,54,197,248,189,191,216,39,39,230,224,84,111,183,197,227,182,155,231,49,19,48,129,233,35,208,233,190,108,58,19,251,17,53,47,230,177,175,250,100,218,132,147,206,54,93,195,116,215,171,136,
248,204,153,51,123,230,206,157,187,33,2,209,66,59,9,56,206,143,121,14,65,34,213,56,81,227,189,200,85,251,122,51,1,19,24,110,2,241,62,37,87,236,181,229,222,193,69,121,125,124,124,124,79,45,34,85,94,77,
156,62,125,122,183,96,53,45,24,144,128,96,30,245,82,140,128,52,174,141,72,30,247,107,77,240,15,19,48,129,25,67,32,222,191,249,189,173,69,196,26,253,124,31,250,77,78,193,57,113,30,185,188,85,11,86,149,
39,226,180,160,189,44,56,70,224,196,8,132,118,17,232,157,32,197,167,98,29,207,155,9,152,192,204,36,16,239,95,242,24,149,119,211,162,107,58,185,72,222,170,69,171,138,136,79,158,60,185,47,7,80,234,11,100,
94,151,76,105,128,22,28,229,249,166,121,212,99,158,207,107,234,107,31,111,38,96,2,253,37,192,61,218,235,89,226,126,202,233,147,231,49,119,137,250,204,41,141,229,181,36,226,125,189,94,227,100,231,87,17,
241,137,19,39,90,79,196,249,66,187,233,235,223,143,210,188,40,73,106,138,2,27,199,122,5,17,247,213,177,188,153,128,9,212,37,208,237,125,199,188,24,149,199,150,59,69,99,121,173,219,190,188,85,139,68,21,
17,191,241,198,27,95,223,184,113,227,200,185,115,231,90,80,20,99,3,140,106,146,43,162,165,46,89,42,87,4,44,115,114,145,210,215,60,242,118,48,53,207,155,9,152,192,112,19,136,247,41,185,98,108,114,4,125,
114,28,66,196,59,234,147,199,200,60,197,215,95,127,253,31,107,81,169,34,226,157,59,119,238,184,231,158,123,78,164,197,45,208,162,227,98,35,132,244,175,166,94,16,53,162,213,120,190,33,88,197,188,241,33,
169,78,174,253,217,39,63,150,251,38,96,2,195,73,32,222,191,186,66,250,49,42,167,201,43,202,113,12,110,193,55,165,58,181,56,103,98,191,19,47,191,252,242,179,181,200,84,17,113,90,204,248,209,163,71,191,
176,98,197,138,159,214,130,1,84,138,60,17,107,44,151,44,80,52,71,91,62,142,108,21,249,176,168,209,231,24,142,38,96,2,195,79,32,191,111,233,43,230,13,153,230,17,207,224,30,34,245,24,25,147,175,18,157,241,
90,132,106,137,120,36,189,158,248,155,101,203,150,253,244,217,179,103,71,70,71,71,139,50,214,152,36,155,11,22,24,124,8,234,43,207,231,170,206,190,202,217,144,49,125,71,19,48,129,153,65,32,222,243,186,
98,228,27,115,213,144,175,114,137,21,161,70,201,146,203,51,228,77,81,190,170,73,168,154,136,247,236,217,243,232,77,55,221,148,214,125,110,182,64,232,53,4,13,1,75,172,202,145,105,46,80,62,20,69,141,17,
227,252,184,79,204,5,53,239,215,4,237,115,153,128,9,116,79,128,123,157,61,232,199,168,156,38,241,42,143,2,86,46,159,228,77,242,165,22,243,80,59,39,95,113,238,26,177,154,136,15,30,60,120,32,181,223,95,
181,106,213,191,215,226,1,32,25,75,192,180,92,170,17,188,230,170,207,92,229,249,124,100,75,4,98,222,167,238,104,2,38,48,156,4,184,247,117,117,228,49,42,167,73,186,177,69,199,196,28,217,230,145,57,138,
242,148,124,85,147,74,53,17,107,81,207,61,247,220,239,45,95,190,252,223,37,161,206,66,192,68,228,138,88,129,144,131,103,30,81,243,149,199,253,144,46,145,99,229,125,234,142,38,96,2,195,69,128,251,158,171,
162,31,163,114,201,151,216,78,196,233,215,44,92,120,10,206,37,28,251,105,222,121,121,138,243,214,138,85,69,188,127,255,254,215,14,29,58,244,71,43,87,174,252,5,68,74,140,34,141,139,23,228,216,152,79,212,
126,202,227,254,49,215,177,212,47,109,77,245,210,92,215,76,192,4,250,71,64,247,120,105,139,117,60,160,121,228,37,17,235,169,86,117,9,86,57,162,149,140,75,77,227,136,250,240,225,195,127,40,79,149,174,165,
159,181,170,34,214,66,158,126,250,233,79,95,121,229,149,255,58,201,115,54,50,205,69,202,130,129,77,20,92,205,141,79,209,72,55,143,58,6,162,37,114,220,24,219,141,197,121,206,77,192,4,250,71,64,247,120,
105,163,30,35,62,32,70,25,35,97,197,40,97,100,139,136,145,51,245,137,120,110,251,246,237,191,93,186,142,126,215,170,139,56,189,123,73,95,43,222,249,111,174,191,254,250,255,137,136,145,104,190,216,8,90,
223,180,64,196,196,40,112,114,29,35,30,47,23,109,222,207,207,233,190,9,152,192,96,9,32,93,174,130,62,62,80,93,121,20,48,253,40,98,201,53,202,24,9,43,166,223,172,118,225,233,24,25,203,75,242,19,231,173,
25,171,139,88,139,219,182,109,219,103,211,87,217,254,69,122,50,254,72,148,102,92,56,96,5,27,9,235,73,88,77,112,145,56,2,230,56,49,234,120,185,120,243,126,60,167,115,19,48,129,225,32,160,251,159,141,92,
177,212,228,136,216,228,7,154,36,27,91,148,113,204,143,28,57,242,144,188,196,57,107,199,242,203,211,10,87,177,120,241,226,165,15,62,248,224,179,11,22,44,88,57,103,206,156,145,177,177,177,17,69,154,228,
171,92,226,85,142,132,21,37,95,69,73,21,17,19,17,173,34,185,150,19,243,124,121,237,198,242,185,238,155,128,9,76,47,1,68,155,31,53,214,17,176,230,144,199,39,226,118,34,150,148,37,93,158,124,17,48,79,197,
233,119,74,188,254,240,195,15,111,62,118,236,216,145,252,26,106,245,7,242,68,172,197,105,209,79,62,249,228,7,183,108,217,242,247,169,187,72,53,0,43,2,54,127,26,70,196,18,47,13,233,170,175,141,62,121,140,
202,243,205,34,206,137,184,111,2,245,8,232,126,47,109,212,99,36,71,194,234,227,10,34,79,195,138,241,105,56,23,177,132,124,234,212,169,55,229,161,65,74,88,107,31,216,19,49,224,175,185,230,154,77,91,183,
110,253,234,188,121,243,150,231,79,195,146,112,254,52,220,36,226,118,79,196,185,104,243,62,215,226,104,2,38,48,88,2,136,150,171,160,31,163,114,53,100,140,128,137,185,136,17,114,254,84,156,36,124,232,241,
199,31,255,192,107,175,189,246,60,231,27,84,28,184,136,181,240,171,175,190,122,253,189,247,222,251,213,249,243,231,175,147,140,17,48,81,242,141,141,39,97,162,196,106,17,15,234,127,66,62,175,9,76,31,1,
132,203,17,233,199,168,92,173,27,17,35,225,248,100,44,33,167,223,145,190,251,107,95,251,218,7,210,95,101,222,197,185,6,25,135,66,196,2,144,254,162,199,213,119,221,117,215,31,95,113,197,21,15,34,96,98,
148,112,254,68,156,75,88,125,154,142,171,60,198,86,103,226,7,99,177,230,220,4,76,160,62,1,68,27,207,76,45,70,229,121,227,73,152,24,159,136,75,34,254,238,119,191,251,240,19,79,60,241,115,233,239,52,188,
17,207,55,200,124,104,68,60,1,97,214,45,183,220,242,225,27,111,188,241,15,210,63,54,186,162,36,98,158,130,99,148,80,253,68,60,200,255,25,249,220,38,48,125,4,16,175,142,72,30,35,34,110,122,34,86,61,202,
152,167,225,244,47,110,28,124,241,197,23,127,113,199,142,29,250,205,106,229,23,211,211,183,140,158,142,52,108,34,110,93,252,210,165,75,23,39,33,127,50,189,178,248,229,36,227,185,18,178,68,203,147,113,
46,225,92,196,60,233,230,81,7,167,214,11,165,201,236,211,203,241,61,215,4,46,117,2,136,180,155,117,198,185,228,49,42,167,149,100,140,132,53,54,33,225,211,233,21,196,239,37,1,127,58,125,77,237,88,55,215,
80,123,206,80,138,56,64,88,184,121,243,230,31,90,179,102,205,207,36,57,127,52,137,248,162,191,141,199,83,48,81,194,164,233,24,49,167,31,142,221,117,106,17,119,141,202,19,77,160,72,0,145,22,7,11,197,56,
159,28,249,198,88,18,241,196,19,241,185,36,221,207,239,221,187,247,79,159,125,246,217,175,164,83,28,47,156,102,104,74,195,46,226,11,160,210,187,227,43,174,186,234,170,155,23,45,90,180,50,253,161,222,154,
244,253,227,117,233,155,22,235,210,211,242,156,52,233,60,50,214,14,41,111,253,223,142,146,64,53,207,155,9,152,192,240,18,144,72,243,13,25,167,177,150,179,212,159,152,55,43,61,245,158,73,223,128,216,157,
190,15,188,59,253,33,220,222,55,223,124,243,245,3,7,14,60,151,222,5,127,55,63,142,251,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,
96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,
96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,
96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,
96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,
96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,51,148,192,172,153,114,221,43,86,172,184,110,
245,234,213,183,45,92,184,112,205,130,5,11,214,205,159,63,255,186,121,243,230,109,152,61,123,246,156,89,179,102,141,92,118,217,101,173,165,40,87,211,150,106,231,91,73,250,65,141,126,41,118,51,167,180,
159,107,38,96,2,189,17,56,127,254,194,173,217,184,99,156,243,214,91,111,181,110,106,213,168,167,90,43,63,119,238,220,153,83,167,78,189,122,242,228,201,157,39,78,156,216,125,252,248,241,189,251,246,237,
219,126,240,224,193,157,141,7,31,178,129,161,22,241,149,87,94,185,234,186,235,174,251,208,202,149,43,63,145,4,252,254,36,221,150,112,37,93,154,228,169,92,49,54,113,70,172,196,88,27,178,207,193,151,99,
2,38,48,65,0,209,170,75,30,163,114,26,50,86,140,45,201,121,36,9,249,27,175,191,254,250,103,118,238,220,249,55,135,15,31,222,63,204,128,135,82,196,18,240,29,119,220,241,187,203,150,45,251,41,201,55,54,
73,55,10,89,146,109,39,226,201,74,56,238,55,204,31,160,175,205,4,102,10,1,100,218,205,245,50,183,20,145,176,98,73,196,146,176,154,198,206,158,61,219,202,143,30,61,250,231,79,62,249,228,175,12,171,144,
135,77,196,99,91,183,110,253,133,245,235,215,255,183,57,115,230,204,31,29,29,29,81,203,69,204,211,48,177,36,99,125,216,200,148,24,107,221,252,143,193,115,76,192,4,234,19,64,190,58,51,121,140,202,213,74,
18,230,169,24,25,43,74,198,106,103,206,156,57,185,107,215,174,255,248,248,227,143,255,97,58,244,120,253,149,53,159,113,104,68,188,102,205,154,235,239,188,243,206,255,147,94,65,108,70,192,196,40,226,248,
52,28,69,140,140,181,84,229,52,150,30,101,204,28,198,28,77,192,4,6,79,0,217,114,37,177,143,124,169,33,97,245,145,47,49,74,56,138,24,33,167,87,22,207,166,167,227,31,223,187,119,239,183,57,215,160,227,80,
136,56,61,1,223,182,101,203,150,191,155,59,119,238,18,201,55,61,13,183,158,132,123,17,113,254,122,66,96,145,47,17,216,121,159,186,163,9,152,192,96,9,32,90,174,130,126,140,202,213,144,49,2,38,182,19,113,
122,42,110,61,29,159,62,125,250,232,182,109,219,126,56,61,33,111,231,92,131,140,3,23,241,13,55,220,240,131,233,125,240,195,99,99,99,243,37,224,92,194,37,25,243,36,76,148,88,163,136,5,84,53,132,75,4,116,
222,167,238,104,2,38,48,88,2,8,151,171,160,143,124,85,39,239,70,196,188,35,70,192,19,175,40,244,154,98,100,124,124,252,228,83,79,61,245,224,75,47,189,244,15,156,111,80,113,116,80,39,214,121,111,188,241,
198,31,190,253,246,219,191,156,36,60,154,90,75,194,136,152,40,17,243,106,66,185,132,27,95,79,72,170,81,194,234,35,218,152,179,78,198,232,59,154,128,9,12,23,1,228,171,171,34,143,17,17,43,230,50,214,211,
176,106,8,88,174,80,206,67,91,116,69,58,252,252,244,16,248,149,228,132,7,95,124,241,197,175,12,146,194,192,68,156,190,17,177,250,182,219,110,123,40,74,24,25,75,184,185,136,17,114,183,34,70,184,68,32,231,
125,234,142,38,96,2,195,65,0,233,114,53,244,163,128,201,155,68,44,1,243,126,24,9,235,222,71,196,28,59,197,209,228,161,191,74,223,166,216,244,157,239,124,103,95,168,87,77,7,37,226,57,247,222,123,239,159,
37,241,46,150,112,213,144,48,253,169,136,88,4,5,61,74,55,230,145,112,83,61,206,113,110,2,38,208,63,2,136,54,63,67,172,35,94,205,33,87,108,18,113,46,97,4,156,223,239,19,199,90,44,31,125,241,139,95,188,
63,29,254,76,126,29,53,250,3,17,241,125,247,221,247,201,197,139,23,223,23,165,155,203,152,167,98,253,151,141,167,97,229,106,252,23,142,40,184,121,19,188,8,61,230,37,176,157,198,75,251,184,102,2,38,48,
121,2,146,96,187,45,142,147,79,136,243,93,50,150,144,213,120,18,198,13,188,150,40,249,33,30,75,62,146,151,30,123,236,177,223,104,119,77,253,26,171,46,226,13,27,54,220,186,118,237,218,95,207,37,156,247,
17,113,46,225,92,196,2,28,255,107,135,80,243,8,64,234,244,29,77,192,4,134,139,0,210,229,170,232,199,136,68,243,39,98,245,145,48,81,50,214,22,239,125,246,143,81,94,74,126,250,194,171,175,190,250,12,231,
174,21,171,139,120,211,166,77,255,69,114,69,180,185,128,233,51,46,241,230,50,6,176,192,230,18,86,45,2,111,202,107,1,246,121,76,192,4,122,39,128,116,181,39,121,140,81,160,185,140,241,3,17,7,16,57,102,126,
12,189,206,120,239,123,223,251,159,147,136,127,178,247,43,158,218,30,85,69,188,126,253,250,155,150,44,89,242,19,136,56,70,9,184,169,143,140,5,54,62,17,11,108,73,196,66,18,161,199,60,199,213,110,44,159,
235,190,9,152,192,244,19,64,176,165,35,199,49,242,40,80,229,185,136,115,47,196,227,198,125,181,31,77,18,86,91,186,116,233,199,228,169,244,253,226,23,226,126,253,206,171,138,56,125,93,237,63,33,213,38,
233,198,39,98,230,176,79,73,196,130,158,55,65,67,176,196,28,100,83,61,159,231,190,9,152,64,29,2,136,54,63,27,245,24,163,80,229,5,9,149,168,123,91,125,109,165,251,156,125,145,176,34,34,158,120,42,254,100,
18,241,191,106,29,160,210,143,106,34,94,181,106,213,53,233,151,249,252,44,114,85,68,176,68,198,74,125,213,212,4,155,38,200,106,234,107,163,79,222,42,78,212,201,123,137,165,15,177,151,253,61,215,4,76,224,
157,87,11,147,97,129,124,181,47,57,34,141,49,74,88,247,173,132,170,45,222,195,204,71,192,120,70,115,229,30,100,156,190,90,251,115,201,87,159,218,191,127,255,107,173,131,84,248,81,77,196,233,69,248,3,200,
23,169,34,94,234,121,100,94,140,72,88,17,241,198,152,195,143,31,68,206,179,221,88,62,215,125,19,48,129,254,17,64,178,165,51,196,49,114,164,74,148,92,115,15,112,44,230,16,229,25,100,28,159,134,163,103,
146,175,238,79,34,254,95,28,163,223,177,154,136,211,47,118,255,145,184,208,201,228,146,175,246,19,112,68,172,168,141,15,129,60,70,229,77,155,101,220,68,198,117,19,168,67,64,130,108,119,31,106,156,141,
28,169,170,142,132,21,75,27,115,21,17,112,39,255,36,95,253,104,58,214,37,39,226,121,151,95,126,249,7,17,105,39,8,113,156,125,98,84,142,120,243,168,15,34,255,80,243,126,233,195,114,205,4,76,96,48,4,218,
221,159,146,103,28,87,95,91,148,171,124,192,60,205,205,231,203,39,204,239,70,196,58,158,124,149,78,51,47,181,83,58,95,191,183,42,79,196,55,221,116,211,237,9,198,152,128,104,145,106,200,182,148,83,139,
81,57,77,160,149,3,157,40,88,241,67,136,121,59,144,221,206,107,119,12,143,153,128,9,244,78,0,177,54,237,201,189,201,60,245,145,46,114,37,74,178,242,130,54,230,35,97,69,141,43,234,93,112,201,63,170,105,
255,137,177,49,121,235,133,23,94,248,122,211,181,77,103,189,138,136,151,47,95,126,55,18,101,161,244,89,124,172,199,26,243,136,250,32,148,199,40,32,234,171,177,197,156,26,177,221,24,115,28,77,192,4,250,
79,32,222,139,200,179,116,86,205,99,188,148,35,97,69,141,203,17,218,180,143,114,69,121,37,202,88,245,220,53,244,21,147,183,182,94,82,34,78,255,216,231,106,45,186,180,112,234,49,2,82,53,229,244,75,145,
113,65,87,206,22,115,106,165,216,237,188,210,190,174,153,128,9,76,141,0,114,213,81,122,189,23,53,159,253,229,10,100,204,21,105,76,115,52,166,60,58,166,155,60,121,107,13,199,234,119,172,242,68,156,126,
225,251,170,110,22,30,231,0,144,154,250,52,213,200,137,2,165,60,110,121,63,142,57,55,1,19,24,60,129,110,239,81,164,26,175,56,175,201,11,60,17,235,184,234,107,142,26,117,124,210,41,106,255,244,47,197,175,
138,231,235,103,94,69,196,233,159,189,95,203,194,1,68,63,70,141,197,113,250,157,162,0,105,78,140,173,206,196,15,198,98,205,185,9,152,192,240,19,144,68,181,197,123,56,23,48,171,160,94,242,5,158,97,140,
126,30,53,78,45,253,70,200,181,28,187,223,177,138,136,211,19,241,186,8,128,60,143,0,200,235,121,95,80,168,145,199,152,231,234,123,51,1,19,152,121,4,116,159,179,69,41,35,93,106,154,195,92,220,80,138,157,
28,163,113,246,147,183,56,119,191,99,21,17,167,47,80,175,97,113,196,8,36,46,158,241,78,17,48,154,167,45,143,140,55,69,230,55,141,187,110,2,38,80,151,64,148,106,233,204,220,179,81,194,212,226,124,213,186,
105,120,7,23,209,215,190,202,229,173,120,220,126,230,85,68,156,22,53,43,95,36,160,98,157,90,187,40,24,113,156,126,140,202,217,52,215,155,9,152,192,240,19,104,186,87,115,65,107,94,73,198,204,139,126,200,
115,81,136,181,118,254,145,183,106,81,123,251,59,30,125,62,155,22,174,45,2,32,207,235,244,137,236,171,190,182,166,126,169,158,215,222,62,130,127,154,128,9,204,36,2,165,251,152,26,81,235,81,30,251,121,
141,241,24,227,156,88,39,215,120,141,173,138,136,181,16,22,166,216,212,167,30,35,57,251,169,223,105,235,101,110,167,99,121,220,4,76,96,240,4,240,71,183,87,146,207,199,9,165,24,231,230,121,183,231,155,
234,188,42,34,102,113,57,4,93,124,28,203,199,233,179,200,216,111,202,153,235,104,2,38,112,105,19,136,14,136,43,205,235,244,75,145,154,246,39,87,36,143,199,237,103,94,69,196,113,1,44,176,105,177,113,92,
251,209,111,58,70,172,55,205,207,231,184,111,2,38,48,51,9,52,249,160,87,159,196,227,196,125,243,122,45,74,213,68,60,93,11,140,199,169,5,201,231,49,1,19,152,25,4,166,226,135,184,111,204,107,172,188,138,
136,89,20,49,95,24,117,98,62,238,190,9,152,128,9,76,55,129,38,223,52,213,167,251,252,241,120,85,68,28,79,232,220,4,76,192,4,76,224,98,2,85,69,204,247,252,46,190,132,119,122,157,198,223,153,233,204,4,76,
192,4,166,70,96,152,124,83,69,196,90,112,92,116,204,133,50,239,183,195,219,203,220,118,199,241,152,9,152,192,165,71,96,42,126,96,95,69,242,90,132,170,136,56,46,134,5,54,45,54,142,107,63,250,77,199,136,
245,166,249,249,28,247,77,192,4,102,38,129,201,250,128,253,242,40,10,170,53,213,107,81,170,34,98,22,218,180,216,166,113,230,3,35,246,155,114,192,178,143,163,9,152,192,165,73,160,201,1,177,174,149,211,
47,69,106,249,188,88,175,65,175,138,136,89,164,22,199,2,201,99,159,121,49,146,51,79,253,110,182,94,231,119,115,76,207,49,1,19,24,12,1,124,209,237,217,243,249,248,160,20,227,220,60,239,246,124,83,157,87,
69,196,113,241,44,148,168,5,144,199,121,177,30,23,201,92,106,249,62,212,217,63,246,157,155,128,9,204,60,2,220,227,241,202,169,17,25,43,245,169,197,168,188,212,167,78,228,184,253,142,85,126,251,90,250,
237,248,231,83,155,197,226,244,219,242,75,57,181,118,145,239,248,105,142,54,245,149,199,24,161,49,47,214,200,57,22,125,71,19,48,129,193,16,104,119,159,230,87,196,220,24,149,151,250,212,155,98,116,81,62,
39,141,157,203,207,221,175,126,21,17,159,57,115,102,79,250,37,203,215,196,133,10,0,16,136,113,188,93,46,24,72,84,243,162,132,213,143,227,173,78,195,15,230,54,12,187,108,2,38,48,68,4,226,253,74,78,212,
101,42,239,165,225,157,60,234,24,170,37,111,237,171,181,252,42,175,38,78,159,62,189,27,64,44,154,126,140,26,107,55,206,220,8,157,60,70,242,56,95,53,111,38,96,2,51,139,0,247,176,34,27,121,140,121,174,126,
169,53,57,6,239,16,181,175,188,197,57,251,29,171,60,17,167,5,237,141,0,200,243,8,56,234,250,165,205,212,242,24,193,148,158,136,53,30,159,154,227,124,231,38,96,2,51,139,128,238,255,184,209,207,189,208,
212,199,41,140,211,207,163,198,169,141,143,143,239,137,231,236,103,94,75,196,251,89,92,183,17,32,146,41,45,66,148,164,219,109,200,185,221,28,198,16,54,125,71,19,48,129,122,4,116,95,119,179,197,121,228,
56,65,81,110,201,251,248,134,58,253,78,81,243,79,157,58,245,122,55,215,53,29,115,170,136,248,196,137,19,23,158,136,59,1,208,56,80,37,72,158,138,85,87,95,81,53,162,32,104,126,175,50,141,243,181,191,55,
19,48,129,193,18,232,116,31,198,113,229,244,21,163,55,98,159,92,227,189,54,121,171,22,145,42,34,62,116,232,208,227,215,94,123,109,11,196,185,115,231,58,2,209,28,201,22,241,34,77,197,40,97,129,85,141,113,
65,47,229,37,152,154,27,55,246,139,53,231,38,96,2,253,37,144,223,135,77,103,139,243,200,21,99,139,50,46,73,23,247,16,59,205,145,183,154,174,103,186,235,85,68,252,194,11,47,124,243,206,59,239,28,79,0,198,
180,120,129,104,106,18,173,154,198,37,71,162,22,142,44,145,177,250,121,211,7,195,60,62,48,250,237,224,49,183,221,28,143,153,128,9,212,37,144,223,151,244,21,75,45,151,171,252,129,115,242,88,114,16,115,
210,216,184,188,85,107,181,85,68,156,22,115,234,123,223,251,222,151,198,198,198,126,172,180,248,88,67,196,81,176,26,103,3,190,230,197,57,228,154,23,197,171,156,15,143,99,148,98,220,167,52,238,154,9,152,
192,244,19,232,230,222,140,115,200,21,243,214,78,194,209,49,157,114,29,71,190,74,171,61,53,253,43,46,31,177,150,136,71,14,30,60,248,215,75,151,46,157,148,136,203,151,254,182,112,17,48,81,115,163,84,99,
94,58,14,227,124,192,165,57,174,153,128,9,244,151,64,187,251,47,142,145,43,198,38,121,170,143,140,59,201,182,211,120,242,213,23,251,187,226,139,143,94,77,196,123,246,236,121,100,227,198,141,35,163,163,
163,23,189,150,56,123,246,108,235,85,196,236,217,179,71,200,145,42,49,94,114,132,207,184,158,142,181,209,39,111,21,39,234,228,147,137,58,174,55,19,48,129,169,17,208,189,219,235,22,247,33,199,1,58,22,2,
86,13,9,43,150,68,43,191,168,174,152,183,124,126,242,213,163,189,94,235,84,230,87,19,241,254,253,251,95,59,124,248,240,159,92,125,245,213,63,43,233,210,36,81,36,44,225,197,87,14,44,44,126,0,154,203,7,
129,120,5,158,28,105,230,145,99,17,25,167,239,104,2,38,48,88,2,220,231,249,85,196,58,247,62,53,68,28,99,46,85,164,27,37,204,156,146,156,211,31,210,253,137,124,149,95,71,63,251,213,68,172,69,188,248,226,
139,191,117,229,149,87,190,75,196,146,111,73,192,44,28,232,154,163,60,206,143,251,33,99,237,23,69,27,115,142,73,108,55,198,28,71,19,48,129,254,18,224,30,207,207,18,235,228,138,177,69,9,43,151,100,21,115,
201,166,191,178,172,191,182,220,170,199,28,81,51,95,158,202,175,163,223,253,170,34,222,181,107,215,11,155,54,109,250,92,146,241,199,226,19,113,20,43,50,141,130,4,58,79,195,204,87,20,240,184,15,185,192,
197,99,148,250,253,134,235,227,155,128,9,76,142,0,210,101,111,250,184,32,143,242,0,141,167,93,162,4,139,128,163,116,75,249,145,35,71,62,39,79,113,222,90,177,170,136,181,168,111,125,235,91,255,245,158,
123,238,105,137,184,233,105,150,197,3,91,128,99,30,69,220,116,12,36,76,212,49,99,206,57,28,77,192,4,134,143,128,238,119,54,242,24,241,129,34,2,38,118,122,34,230,105,56,70,164,44,63,113,222,154,177,186,
136,95,125,245,213,103,214,172,89,243,169,117,235,214,253,186,196,136,84,149,71,81,150,64,151,158,162,75,34,230,56,68,1,141,121,14,184,221,88,62,215,125,19,48,129,233,37,128,96,243,163,198,58,121,244,
2,57,2,38,242,36,76,140,79,196,81,190,121,158,254,128,238,83,242,83,126,29,53,250,213,69,172,69,61,246,216,99,159,94,178,100,201,63,191,252,242,203,239,203,37,8,92,162,224,74,192,250,182,133,114,193,85,
63,10,60,151,177,206,161,227,198,99,199,92,227,249,214,105,60,159,239,190,9,152,192,212,9,232,62,111,218,226,24,62,208,92,114,69,57,129,136,31,136,18,176,124,193,211,110,46,222,216,63,118,236,88,210,210,
99,159,110,186,150,126,215,7,246,189,172,101,203,150,173,126,224,129,7,158,79,191,167,120,113,250,139,30,35,115,230,204,25,33,74,186,234,43,210,226,211,112,39,17,35,213,60,2,147,58,125,71,19,48,129,225,
32,32,169,230,27,53,197,188,245,42,98,158,142,211,111,86,107,189,55,86,76,191,29,242,216,35,143,60,178,233,59,223,249,78,181,223,63,156,175,113,32,79,196,186,8,45,122,251,246,237,63,118,251,237,183,63,
156,186,23,174,67,160,75,112,37,100,4,76,228,73,152,40,193,210,116,14,132,75,140,53,229,222,76,192,4,134,143,0,226,213,149,145,199,168,156,86,114,5,175,36,136,241,137,24,17,235,105,88,18,78,237,172,60,
52,72,9,107,157,3,123,34,214,201,181,221,112,195,13,63,120,199,29,119,60,156,158,134,231,235,41,56,127,18,206,159,136,163,132,227,235,9,4,28,163,142,143,132,137,170,177,149,106,140,57,154,128,9,212,35,
128,104,227,25,99,13,241,150,162,100,76,147,124,149,231,18,46,201,56,61,9,159,252,230,55,191,249,224,75,47,189,244,15,241,188,131,200,7,46,98,45,122,253,250,245,183,109,217,178,229,239,210,107,138,37,
18,111,148,177,196,203,211,176,242,118,34,230,201,88,199,68,200,202,181,229,210,205,251,111,207,242,79,19,48,129,65,17,136,226,213,53,196,62,2,166,206,147,176,234,72,152,136,132,21,37,96,34,79,195,138,
73,194,71,183,109,219,246,195,233,171,106,219,7,181,222,120,222,161,16,177,46,40,125,147,226,250,187,238,186,235,161,5,11,22,220,204,83,48,17,1,19,121,18,38,34,221,60,234,184,8,151,168,26,91,169,198,152,
163,9,152,64,61,2,81,186,156,149,90,140,202,243,134,128,137,81,196,81,194,60,21,31,63,126,252,217,39,159,124,242,199,247,238,221,251,109,206,53,232,56,52,34,158,0,49,150,158,140,63,177,97,195,134,223,
73,79,197,243,75,34,70,190,146,178,68,74,95,121,108,58,30,162,37,198,218,196,249,122,10,241,56,61,237,232,201,38,240,125,76,0,145,118,139,32,206,39,143,81,57,13,249,170,47,233,210,143,50,70,192,233,189,
240,201,244,245,180,95,77,79,194,159,73,215,50,222,237,245,212,152,55,108,34,110,173,57,253,205,187,85,233,247,23,255,110,250,138,219,79,197,215,18,146,110,254,106,66,114,84,93,49,111,58,88,148,103,204,
35,220,166,122,156,227,220,4,76,96,250,9,32,216,252,200,177,78,174,152,55,137,87,53,4,172,136,144,121,26,86,60,122,244,232,159,167,167,224,95,73,191,239,102,127,126,174,97,232,15,165,136,1,35,33,167,127,
217,227,71,86,175,94,253,241,244,202,226,253,185,132,17,48,177,147,136,117,220,94,165,219,235,124,174,221,209,4,76,224,98,2,8,245,226,106,185,23,231,146,43,230,173,36,98,100,156,254,169,163,111,236,219,
183,239,179,175,188,242,202,95,15,171,128,89,253,80,139,152,139,84,92,177,98,197,117,73,200,183,47,92,184,112,117,146,242,186,249,243,231,95,55,111,222,188,13,73,206,115,36,75,100,172,185,200,51,213,46,
124,41,145,154,198,243,173,221,88,62,215,125,19,48,129,233,35,128,100,75,71,140,99,73,174,45,87,81,83,68,194,233,137,247,76,250,135,62,95,61,121,242,228,206,36,223,221,233,29,112,242,239,190,111,166,223,
41,188,179,116,92,215,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,
4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,
76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,
192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,
4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,
76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,192,4,76,
192,4,76,192,4,76,192,4,102,32,129,89,51,229,154,175,72,219,85,87,93,117,243,162,69,139,86,206,159,63,127,205,130,5,11,214,169,93,118,217,101,115,210,26,206,167,56,50,107,214,219,203,73,249,121,173,139,
126,92,163,230,121,51,1,19,24,94,2,111,189,245,214,187,46,238,252,249,214,45,61,146,198,90,55,185,250,19,243,102,165,120,230,196,137,19,187,213,78,158,60,185,247,205,55,223,124,253,192,129,3,207,125,55,
109,239,58,208,144,22,134,93,196,11,55,111,222,252,67,215,92,115,205,207,36,15,127,116,118,218,36,210,188,73,184,165,38,230,200,152,88,250,28,218,141,149,230,187,102,2,38,48,61,4,16,108,126,180,88,39,
87,44,53,9,57,111,231,210,150,60,252,249,215,94,123,237,79,159,125,246,217,175,164,227,31,207,207,49,76,253,161,20,241,210,165,75,23,223,114,203,45,159,188,250,234,171,127,121,206,156,57,115,147,127,71,
212,36,96,34,50,150,68,149,43,230,77,160,163,100,99,206,135,80,170,49,230,104,2,38,208,127,2,136,54,158,41,214,200,155,36,172,122,20,113,114,112,171,175,168,118,230,204,153,211,111,188,241,198,239,237,
216,177,227,211,71,142,28,57,22,207,51,44,249,176,137,120,86,18,240,135,111,188,241,198,63,152,55,111,222,10,73,119,116,116,180,37,223,146,140,37,209,40,100,245,105,2,28,115,250,221,128,215,126,222,76,
192,4,166,151,0,66,237,116,212,56,143,60,70,229,52,4,172,62,2,86,13,9,43,158,61,123,182,213,63,117,234,212,193,23,95,124,241,23,147,144,191,144,174,225,237,119,29,157,46,166,210,248,208,24,103,249,242,
229,87,223,117,215,93,127,156,94,65,60,40,249,198,134,132,137,200,151,136,112,243,40,134,72,149,8,215,188,79,221,209,4,76,160,62,1,68,27,207,76,45,70,229,121,67,198,196,40,97,68,44,25,211,210,43,139,255,
251,196,19,79,252,236,161,67,135,222,136,231,27,100,62,20,34,78,175,32,214,223,123,239,189,95,77,127,8,183,78,2,78,175,35,122,22,113,254,122,66,80,145,45,17,208,121,159,186,163,9,152,192,96,9,32,93,174,
130,62,242,85,157,92,226,85,142,128,137,237,68,156,94,83,180,132,156,254,80,111,247,215,190,246,181,15,164,87,22,187,56,215,32,227,192,69,156,254,32,110,211,214,173,91,191,154,94,69,44,151,128,145,48,
177,233,245,4,79,195,8,152,136,100,21,99,30,33,83,143,53,231,38,96,2,131,39,128,120,185,18,250,200,87,117,242,110,68,204,107,9,69,36,172,168,150,94,85,28,122,252,241,199,63,144,254,64,239,121,206,55,168,
56,58,168,19,235,188,235,215,175,191,109,203,150,45,127,159,36,188,8,9,19,227,171,9,229,241,181,132,242,92,196,136,55,70,157,35,74,55,230,26,43,109,221,204,41,237,231,154,9,152,64,103,2,136,181,211,204,
56,143,28,1,231,145,39,97,69,61,13,19,149,203,21,146,112,131,47,150,223,125,247,221,255,47,141,253,179,93,187,118,109,239,116,77,253,28,31,152,136,23,47,94,188,244,206,59,239,252,18,18,30,27,27,107,61,
13,119,18,113,46,225,248,36,172,92,27,50,38,143,81,121,187,205,34,110,71,199,99,38,48,53,2,72,181,211,81,226,60,114,4,172,125,121,26,86,45,138,56,74,88,34,142,2,142,174,8,247,249,34,121,40,125,155,98,
243,177,99,199,142,116,186,174,126,141,15,74,196,151,165,119,194,127,152,222,9,175,148,120,145,48,81,181,166,39,226,118,34,70,192,68,65,11,192,91,12,243,126,191,192,250,184,38,96,2,147,35,128,120,181,
119,158,35,227,24,39,35,226,120,101,242,144,124,244,183,127,251,183,63,145,234,239,254,219,36,113,114,159,242,129,136,56,189,142,248,248,178,101,203,62,194,211,111,148,113,172,69,25,75,192,72,152,200,
127,225,136,37,1,231,226,205,251,125,226,234,195,154,128,9,76,146,64,148,175,14,65,63,70,68,204,147,49,50,230,117,132,98,254,68,92,186,247,57,142,124,36,47,109,219,182,237,15,38,121,217,83,218,173,186,
136,87,172,88,113,93,218,254,59,146,141,226,69,200,26,83,206,28,197,118,34,70,192,121,132,76,233,3,40,213,152,239,104,2,38,80,151,0,146,205,207,74,61,70,228,25,35,34,214,67,153,114,197,252,1,13,63,112,
14,246,103,95,121,41,189,43,126,228,224,193,131,59,153,83,43,86,23,241,251,222,247,190,79,38,201,206,142,178,69,186,49,146,35,227,40,98,32,3,154,136,92,35,112,106,57,208,166,122,62,207,125,19,48,129,58,
4,144,109,60,91,172,33,78,141,147,75,162,202,145,169,92,208,36,226,184,95,220,135,125,83,156,125,251,237,183,127,242,203,95,254,242,199,227,53,212,200,171,138,120,213,170,85,215,164,191,184,241,243,77,
18,150,124,105,204,145,128,149,247,42,98,193,139,178,141,121,13,176,62,135,9,152,192,212,8,72,150,108,228,49,34,83,98,16,234,133,63,176,215,125,95,186,247,181,15,45,238,151,94,81,252,124,242,212,111,236,
223,191,255,53,206,93,35,86,21,241,205,55,223,172,223,29,49,139,167,220,118,49,62,17,35,97,197,248,52,28,159,132,1,14,116,162,32,198,188,91,168,147,217,167,219,99,123,158,9,124,191,17,144,244,38,179,197,
253,200,17,168,162,28,160,24,101,170,92,247,47,45,158,151,125,227,124,189,75,86,95,81,126,146,167,146,136,255,67,220,175,223,121,53,17,167,119,195,87,165,246,75,81,170,237,68,156,207,67,194,68,65,238,
86,196,130,104,177,246,251,127,74,62,190,9,76,47,1,73,147,141,60,70,164,170,136,15,16,106,46,225,56,87,121,46,98,73,24,231,200,83,169,253,86,122,87,124,128,243,247,59,86,19,241,218,181,107,239,79,11,213,
118,225,15,225,148,231,125,94,67,68,73,51,79,226,85,14,244,92,196,130,149,127,0,212,250,13,210,199,55,1,19,152,126,2,146,38,27,121,140,202,213,116,223,75,174,138,218,136,113,95,230,106,158,252,130,180,
113,144,100,172,191,252,145,250,179,229,171,36,226,255,205,254,253,142,213,68,156,126,159,196,135,88,176,98,158,199,62,50,102,30,81,226,141,13,233,198,40,96,241,67,136,121,191,97,250,248,38,96,2,211,79,
0,241,234,200,228,72,149,136,3,36,87,109,241,190,103,14,17,9,227,21,245,121,34,86,46,25,203,87,233,48,151,156,136,199,150,44,89,242,97,45,156,167,90,32,148,98,156,19,197,75,93,144,149,3,63,198,214,167,
144,125,16,170,197,15,134,57,142,38,96,2,195,75,64,226,212,198,189,171,190,242,24,145,107,62,87,251,49,38,199,144,75,212,106,37,239,168,134,99,228,171,116,136,177,212,198,117,172,126,111,85,158,136,211,
247,243,110,73,139,92,192,34,21,105,17,72,172,197,60,2,82,93,31,6,81,57,77,176,148,179,197,156,154,163,9,152,192,204,32,16,239,95,137,148,62,210,69,174,68,173,138,177,184,66,213,240,9,49,247,78,116,204,
196,216,130,141,27,55,110,126,249,229,151,159,138,199,234,87,94,69,196,233,49,255,30,1,40,44,246,66,141,113,230,0,76,81,31,0,49,207,213,167,9,146,242,24,91,157,54,63,152,223,102,138,135,76,192,4,250,72,
160,36,207,252,116,186,79,59,205,147,35,244,180,171,185,202,155,4,172,57,122,21,161,57,19,210,189,144,171,70,91,185,114,229,15,92,82,34,78,255,200,231,26,22,215,107,4,106,220,143,154,162,154,182,152,211,
111,13,116,249,131,227,116,57,221,211,76,192,4,166,72,0,177,246,122,239,105,126,220,87,185,154,4,43,79,176,81,143,238,232,37,151,183,56,86,191,99,149,39,226,244,75,53,86,247,2,160,52,23,209,150,162,32,
241,97,18,99,173,223,16,125,124,19,48,129,222,9,196,123,181,211,222,146,106,167,249,140,43,170,201,35,37,25,51,86,242,76,172,205,157,59,119,117,167,235,154,174,241,42,34,78,11,106,61,17,3,128,168,69,231,
185,250,189,54,193,208,62,49,182,58,19,63,24,139,53,231,38,96,2,195,79,64,34,213,22,239,225,38,41,51,71,49,111,8,86,117,242,78,81,222,170,69,168,150,136,215,229,0,0,133,140,233,199,121,177,150,231,17,
144,198,180,17,243,188,53,232,31,38,96,2,51,142,64,188,167,163,148,145,49,53,45,140,185,138,147,109,209,71,73,196,235,106,1,171,34,226,244,215,6,215,230,96,248,175,81,172,71,8,177,222,41,207,63,132,94,
224,233,216,222,76,192,4,6,79,32,74,181,116,53,220,171,81,194,212,226,252,78,190,200,61,131,139,242,122,250,253,232,107,227,113,251,153,87,17,113,90,224,104,132,147,47,56,239,199,185,128,38,2,35,246,201,
137,204,33,54,213,25,119,52,1,19,24,60,129,210,125,90,146,179,230,149,100,204,92,141,119,211,218,121,103,98,172,138,31,69,254,157,63,98,236,243,231,16,193,232,84,157,250,204,201,35,251,81,87,159,188,149,
132,31,113,110,40,59,53,1,19,152,33,4,154,238,225,210,125,95,154,27,107,228,68,33,32,207,35,99,181,48,85,17,113,190,72,250,44,182,83,159,121,189,64,209,49,189,153,128,9,92,26,4,162,35,186,89,81,62,31,
31,196,24,231,148,234,212,186,57,223,84,231,84,23,177,46,152,5,2,34,246,243,241,82,95,181,184,177,63,181,188,79,221,209,4,76,224,210,33,16,239,243,166,92,171,101,172,20,85,107,170,215,36,85,69,196,113,
65,113,209,228,157,96,197,253,153,27,1,198,241,120,204,88,119,110,2,38,48,243,9,148,238,111,106,196,184,74,106,77,81,115,53,150,143,83,143,199,234,103,94,77,196,44,116,170,11,140,199,233,39,24,31,219,
4,76,96,230,17,152,138,31,226,190,49,175,65,161,138,136,89,20,49,95,24,117,98,62,238,190,9,152,128,9,76,55,129,38,223,52,213,167,251,252,241,120,85,68,28,79,232,220,4,76,192,4,76,224,98,2,85,69,204,247,
252,46,190,132,119,122,157,198,223,153,233,204,4,76,192,4,166,70,96,152,124,83,69,196,90,112,92,116,204,133,50,239,183,195,219,203,220,118,199,241,152,9,152,192,165,71,96,42,126,96,95,69,242,90,132,170,
136,56,46,134,5,54,45,54,142,107,63,250,241,24,228,165,177,82,141,249,142,38,96,2,51,155,64,233,254,46,213,88,37,99,77,81,243,52,198,56,251,81,143,253,126,230,85,68,204,66,89,44,145,197,54,141,199,121,
204,37,198,177,152,247,19,150,143,109,2,38,48,60,4,226,125,175,156,62,145,43,165,95,138,212,52,87,121,108,236,95,35,86,17,49,11,97,145,234,147,43,210,47,69,106,204,107,77,238,240,163,151,185,29,14,229,
97,19,48,129,33,33,208,203,125,173,185,113,62,121,41,198,185,121,94,107,233,85,68,204,226,74,81,11,85,189,41,50,214,154,16,230,230,253,210,188,188,198,62,142,38,96,2,51,135,128,238,227,252,94,166,79,100,
53,165,62,181,24,149,151,250,121,141,227,246,59,86,249,237,66,233,159,48,73,235,59,63,75,139,156,106,211,119,252,116,12,54,250,49,50,166,24,231,82,31,196,247,4,57,183,163,9,152,64,51,129,210,253,154,207,
206,231,168,79,211,92,242,201,70,253,147,75,218,87,222,202,207,221,175,126,21,17,159,57,115,230,245,244,75,150,87,1,134,133,78,44,150,69,119,13,80,48,144,169,142,17,37,172,126,28,111,117,178,31,204,201,
202,238,154,128,9,12,57,129,120,239,42,47,245,169,119,138,242,80,201,69,218,79,245,244,15,140,190,94,11,71,149,87,19,227,227,227,187,129,194,194,35,132,82,141,249,165,40,56,212,201,99,36,143,115,84,243,
102,2,38,48,243,8,112,31,43,106,107,234,231,99,113,30,121,244,78,169,134,139,52,118,234,212,169,221,173,19,86,248,81,229,137,56,137,120,79,90,224,61,44,50,194,32,103,76,0,168,233,151,51,3,43,143,145,77,
167,39,98,237,235,205,4,76,96,102,19,136,247,49,121,238,133,166,62,78,97,156,126,30,53,78,77,222,170,69,172,138,136,211,127,89,246,197,5,106,161,177,159,195,137,125,201,88,243,145,45,99,83,1,196,107,141,
169,28,195,251,154,128,9,244,135,128,238,241,166,141,177,24,113,66,30,163,103,24,67,178,221,140,201,91,77,215,49,221,245,90,34,222,31,1,40,79,239,95,90,45,230,179,103,207,190,240,95,35,213,163,132,213,
71,198,140,245,10,3,1,243,33,246,186,191,231,155,128,9,212,33,208,116,143,82,143,81,185,154,188,144,231,212,20,243,38,7,169,22,93,20,231,36,17,239,175,179,218,145,145,42,34,62,124,248,240,147,235,214,
173,187,176,232,184,216,78,185,228,73,211,92,109,81,208,140,41,234,67,80,108,218,58,141,55,237,231,186,9,152,64,61,2,186,79,75,91,172,43,47,53,124,162,49,242,201,70,121,171,116,29,253,168,85,17,241,129,
3,7,158,76,48,206,165,54,91,80,248,47,80,140,212,37,89,181,40,88,242,8,64,53,205,99,19,120,213,98,212,152,106,113,211,120,63,183,252,124,253,60,151,143,109,2,181,9,244,251,254,105,90,15,231,45,69,213,
228,15,162,242,216,162,103,186,201,39,92,116,78,222,106,186,158,233,174,87,17,241,161,67,135,254,233,216,177,99,143,206,153,51,231,65,64,76,44,246,162,215,19,18,171,94,79,104,142,114,230,148,22,141,240,
20,243,156,15,75,117,114,142,193,92,250,211,29,243,243,77,247,241,125,60,19,184,212,9,148,238,161,88,83,78,159,92,174,80,174,136,55,162,107,168,197,72,174,121,49,87,95,190,146,183,106,177,174,34,98,45,
38,45,234,175,174,184,226,138,11,34,62,123,246,236,5,233,42,151,120,213,148,75,150,202,75,210,4,188,198,149,107,78,222,128,87,218,191,84,99,190,163,9,152,192,112,16,208,189,29,183,216,199,1,26,151,64,
233,43,167,149,228,170,154,252,66,36,167,175,72,75,175,37,30,138,231,239,119,94,77,196,187,119,239,126,244,218,107,175,189,176,80,22,156,71,132,220,36,76,160,43,34,235,40,98,1,139,251,198,60,194,108,170,
199,57,206,77,192,4,234,18,208,125,157,111,177,70,174,24,27,2,38,70,175,72,184,81,186,249,88,236,147,239,218,181,235,239,242,235,232,103,191,154,136,247,237,219,247,210,209,163,71,191,176,124,249,242,
15,11,138,94,65,208,144,111,147,88,35,128,28,126,190,143,230,34,89,34,251,231,125,234,142,38,96,2,195,69,0,225,114,85,244,99,196,5,146,175,114,36,172,136,80,163,128,17,114,140,154,151,247,229,41,249,138,
115,215,136,213,68,172,197,188,240,194,11,191,177,100,201,146,15,243,30,88,0,148,151,68,204,226,115,240,113,174,114,65,151,96,243,198,254,185,124,243,62,243,28,77,192,4,134,135,0,247,61,87,68,95,49,230,
37,9,151,68,156,126,205,194,69,194,141,242,37,71,202,242,20,231,173,21,171,138,120,231,206,157,223,216,180,105,211,163,75,151,46,189,63,23,42,34,205,23,14,120,69,1,46,137,91,199,138,251,35,219,60,230,
199,238,166,207,49,186,153,235,57,38,96,2,205,4,16,104,243,140,119,143,176,79,41,226,4,162,252,128,132,21,37,88,228,170,92,50,46,53,68,172,248,189,239,125,239,81,121,234,221,87,210,223,74,85,17,107,41,
207,63,255,252,167,238,190,251,238,70,17,71,241,9,112,108,146,112,148,177,230,70,9,171,79,211,185,56,22,81,181,210,214,105,188,180,143,107,38,96,2,211,67,64,247,120,211,198,88,140,209,9,202,229,4,162,
196,139,140,163,132,115,17,71,249,198,167,101,249,169,233,90,250,89,175,46,226,244,95,155,127,220,176,97,195,239,175,92,185,242,151,120,42,142,242,212,98,75,208,5,87,77,251,16,145,48,49,30,7,185,18,129,
152,247,169,59,154,128,9,12,15,1,28,160,43,34,143,81,185,154,92,64,84,222,173,136,227,147,49,146,78,223,27,254,125,249,105,16,20,46,254,219,14,245,174,96,193,71,62,242,145,199,23,45,90,180,121,108,108,
108,68,45,125,199,248,66,84,62,58,58,218,170,233,41,88,185,34,13,129,19,17,176,250,218,232,147,183,138,19,117,242,126,68,75,190,31,84,125,204,97,35,128,16,251,121,93,241,28,228,138,228,8,88,125,229,177,
73,198,177,33,90,162,36,156,126,161,79,235,53,133,162,218,155,111,190,249,236,67,15,61,180,53,173,233,68,63,215,213,116,236,234,79,196,19,23,114,98,219,182,109,31,189,239,190,251,118,36,121,141,197,139,
3,54,160,37,97,229,138,130,155,203,88,242,203,159,136,17,98,30,227,121,24,139,53,231,38,96,2,131,35,128,100,227,21,80,139,17,71,40,226,9,68,28,5,76,142,128,137,60,13,7,25,143,203,71,233,188,3,145,176,
214,59,40,17,143,232,235,33,223,250,214,183,126,38,253,225,221,95,0,190,4,88,48,75,18,150,144,17,48,81,114,165,233,152,200,150,24,107,156,211,209,4,76,96,184,8,32,93,93,21,121,140,185,39,144,177,92,161,
28,1,43,74,190,138,188,7,70,194,68,201,88,30,170,253,117,181,156,248,192,68,172,11,121,250,233,167,255,50,65,61,119,243,205,55,127,46,117,91,175,73,128,204,127,225,120,34,142,79,194,72,88,2,46,73,24,25,
183,19,112,28,203,161,184,111,2,38,80,159,0,178,229,204,177,143,23,74,17,87,32,97,34,34,150,140,105,8,121,226,105,248,252,115,207,61,247,177,103,158,121,230,243,156,115,80,113,80,239,136,47,90,111,122,
42,190,255,214,91,111,253,82,122,55,60,22,223,15,75,194,146,46,49,202,24,9,71,17,147,235,224,200,152,19,229,226,205,251,204,115,52,1,19,24,12,129,40,94,93,65,236,35,96,234,146,45,181,40,226,40,97,68,76,
68,194,19,79,195,227,73,192,31,76,223,146,120,116,48,171,189,248,172,67,33,98,93,82,250,235,207,119,221,113,199,29,127,51,111,222,188,21,18,47,45,23,177,100,27,159,136,37,84,4,140,124,137,58,46,194,37,
170,22,183,166,122,156,227,220,4,76,160,127,4,162,112,57,75,172,145,35,222,24,17,50,50,150,116,163,140,121,53,193,19,177,98,250,61,195,7,159,122,234,169,15,189,242,202,43,79,112,190,65,199,161,17,177,
64,164,191,232,177,248,150,91,110,249,181,85,171,86,253,106,20,49,50,206,37,172,126,148,48,185,142,85,146,177,234,218,44,223,183,57,248,167,9,12,27,1,164,203,117,209,71,190,170,43,71,192,228,136,56,74,
88,82,166,33,226,253,251,247,255,206,142,29,59,126,243,200,145,35,199,56,199,48,196,161,18,49,64,214,174,93,187,249,182,219,110,251,76,250,122,219,61,188,142,32,70,25,75,168,200,151,88,18,112,20,111,204,
57,159,163,9,152,192,240,16,64,190,186,34,242,24,149,171,33,227,24,243,39,98,68,156,190,158,246,245,237,219,183,127,98,207,158,61,207,14,207,74,223,185,146,161,20,241,196,229,93,182,126,253,250,91,211,
191,236,241,177,171,174,186,234,227,233,187,198,203,163,132,149,71,249,198,28,217,230,145,101,83,167,239,104,2,38,48,28,4,16,46,87,67,63,70,229,106,81,192,244,121,34,86,76,127,32,119,40,253,37,141,207,
166,223,252,248,185,244,219,212,158,73,199,124,251,159,248,225,224,67,20,135,89,196,17,211,232,198,141,27,239,74,66,222,186,112,225,194,117,233,61,242,181,243,231,207,191,126,238,220,185,215,37,1,143,
53,73,24,225,18,57,96,222,167,238,104,2,38,48,88,2,8,151,171,160,175,152,231,234,75,184,169,141,159,62,125,122,231,201,147,39,191,157,222,255,190,114,252,248,241,221,73,192,143,191,252,242,203,122,7,124,
150,99,57,154,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,
152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,
152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,
152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,
124,31,16,248,255,13,164,179,32,34,250,224,211,0,0,0,0,73,69,78,68,174,66,96,130,0,0};

const char* PlayerBackgroundComponent::menu_icon_png = (const char*) resource_PlayerBackgroundComponent_menu_icon_png;
const int PlayerBackgroundComponent::menu_icon_pngSize = 16232;

// JUCER_RESOURCE: menu_activated_icon_png, 17153, "../../SpatCommon/Resources/Menu_activated_icon.png"
static const unsigned char resource_PlayerBackgroundComponent_menu_activated_icon_png[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,1,98,0,0,1,98,8,6,0,0,0,204,92,210,122,0,0,0,1,115,82,71,66,
0,174,206,28,233,0,0,0,9,112,72,89,115,0,0,46,35,0,0,46,35,1,120,165,63,118,0,0,3,170,105,84,88,116,88,77,76,58,99,111,109,46,97,100,111,98,101,46,120,109,112,0,0,0,0,0,60,120,58,120,109,112,109,101,116,
97,32,120,109,108,110,115,58,120,61,34,97,100,111,98,101,58,110,115,58,109,101,116,97,47,34,32,120,58,120,109,112,116,107,61,34,88,77,80,32,67,111,114,101,32,53,46,52,46,48,34,62,10,32,32,32,60,114,100,
102,58,82,68,70,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,47,50,50,45,114,100,102,45,115,121,110,116,97,120,45,110,
115,35,34,62,10,32,32,32,32,32,32,60,114,100,102,58,68,101,115,99,114,105,112,116,105,111,110,32,114,100,102,58,97,98,111,117,116,61,34,34,10,32,32,32,32,32,32,32,32,32,32,32,32,120,109,108,110,115,58,
120,109,112,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,34,10,32,32,32,32,32,32,32,32,32,32,32,32,120,109,108,110,115,58,116,105,102,102,61,34,
104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,116,105,102,102,47,49,46,48,47,34,10,32,32,32,32,32,32,32,32,32,32,32,32,120,109,108,110,115,58,101,120,105,102,61,34,104,116,116,
112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,101,120,105,102,47,49,46,48,47,34,62,10,32,32,32,32,32,32,32,32,32,60,120,109,112,58,77,111,100,105,102,121,68,97,116,101,62,50,48,49,55,45,49,
48,45,49,50,84,50,48,58,49,48,58,50,53,60,47,120,109,112,58,77,111,100,105,102,121,68,97,116,101,62,10,32,32,32,32,32,32,32,32,32,60,120,109,112,58,67,114,101,97,116,111,114,84,111,111,108,62,80,105,120,
101,108,109,97,116,111,114,32,51,46,55,60,47,120,109,112,58,67,114,101,97,116,111,114,84,111,111,108,62,10,32,32,32,32,32,32,32,32,32,60,116,105,102,102,58,79,114,105,101,110,116,97,116,105,111,110,62,
49,60,47,116,105,102,102,58,79,114,105,101,110,116,97,116,105,111,110,62,10,32,32,32,32,32,32,32,32,32,60,116,105,102,102,58,67,111,109,112,114,101,115,115,105,111,110,62,53,60,47,116,105,102,102,58,67,
111,109,112,114,101,115,115,105,111,110,62,10,32,32,32,32,32,32,32,32,32,60,116,105,102,102,58,82,101,115,111,108,117,116,105,111,110,85,110,105,116,62,50,60,47,116,105,102,102,58,82,101,115,111,108,117,
116,105,111,110,85,110,105,116,62,10,32,32,32,32,32,32,32,32,32,60,116,105,102,102,58,89,82,101,115,111,108,117,116,105,111,110,62,51,48,48,60,47,116,105,102,102,58,89,82,101,115,111,108,117,116,105,111,
110,62,10,32,32,32,32,32,32,32,32,32,60,116,105,102,102,58,88,82,101,115,111,108,117,116,105,111,110,62,51,48,48,60,47,116,105,102,102,58,88,82,101,115,111,108,117,116,105,111,110,62,10,32,32,32,32,32,
32,32,32,32,60,101,120,105,102,58,80,105,120,101,108,88,68,105,109,101,110,115,105,111,110,62,51,53,52,60,47,101,120,105,102,58,80,105,120,101,108,88,68,105,109,101,110,115,105,111,110,62,10,32,32,32,
32,32,32,32,32,32,60,101,120,105,102,58,67,111,108,111,114,83,112,97,99,101,62,49,60,47,101,120,105,102,58,67,111,108,111,114,83,112,97,99,101,62,10,32,32,32,32,32,32,32,32,32,60,101,120,105,102,58,80,
105,120,101,108,89,68,105,109,101,110,115,105,111,110,62,51,53,52,60,47,101,120,105,102,58,80,105,120,101,108,89,68,105,109,101,110,115,105,111,110,62,10,32,32,32,32,32,32,60,47,114,100,102,58,68,101,
115,99,114,105,112,116,105,111,110,62,10,32,32,32,60,47,114,100,102,58,82,68,70,62,10,60,47,120,58,120,109,112,109,101,116,97,62,10,250,180,102,247,0,0,62,240,73,68,65,84,120,1,237,157,105,176,93,213,
121,166,175,208,213,213,128,6,102,5,129,6,91,145,161,0,97,100,3,102,80,140,65,198,36,148,109,72,28,167,227,31,118,218,21,15,63,226,30,42,233,74,165,43,221,254,145,116,42,67,17,255,114,87,55,216,20,229,
161,112,2,69,51,165,176,65,17,147,101,136,193,32,140,16,49,40,34,154,24,36,89,128,102,113,37,161,94,239,181,30,241,177,216,251,156,115,175,206,94,231,8,222,93,181,248,190,53,236,181,247,122,14,251,185,
75,91,87,247,14,12,248,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,
1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,
1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,
1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,48,1,19,232,10,129,113,93,153,165,225,73,14,30,60,56,184,114,229,202,11,214,175,95,255,145,157,59,119,206,77,229,253,187,118,237,90,176,119,239,222,249,7,14,
28,24,122,243,205,55,7,210,152,1,162,114,21,29,121,140,109,35,3,252,31,19,48,129,190,33,48,110,220,91,74,34,143,81,185,202,49,199,28,115,56,142,31,63,126,120,210,164,73,107,142,61,246,216,213,83,167,78,
125,33,149,117,115,230,204,249,233,194,133,11,31,79,99,247,247,205,226,90,220,200,91,171,110,49,168,23,93,73,160,199,172,94,189,250,220,103,158,121,230,179,47,190,248,226,151,223,120,227,141,147,247,239,
223,63,144,196,59,34,92,162,228,139,128,137,221,22,49,50,239,5,7,95,211,4,142,86,2,8,116,180,247,207,121,85,81,109,42,81,196,202,85,146,144,15,199,193,193,193,129,137,19,39,110,57,237,180,211,190,117,
206,57,231,220,186,96,193,130,167,211,121,111,142,246,94,74,141,239,75,17,63,251,236,179,231,252,203,191,252,203,13,175,191,254,250,197,146,47,69,242,165,72,186,200,88,162,68,194,68,228,153,71,129,165,
237,72,33,119,107,158,35,189,15,159,111,2,189,36,128,48,187,113,15,113,46,242,24,149,35,97,242,92,194,18,178,138,100,76,57,238,184,227,30,189,232,162,139,190,114,214,89,103,61,211,141,251,236,246,28,125,
37,226,173,91,183,78,127,248,225,135,255,124,237,218,181,127,186,111,223,190,1,21,36,204,110,88,49,74,88,57,69,98,172,42,130,150,75,51,175,119,27,172,231,51,1,19,24,29,1,132,203,89,212,21,99,142,136,21,
99,97,71,44,249,230,34,158,48,97,194,128,202,188,121,243,254,238,163,31,253,232,95,157,120,226,137,219,185,78,63,196,190,17,241,170,85,171,46,124,240,193,7,239,222,179,103,207,41,72,152,152,203,56,238,
138,163,132,149,231,34,22,228,40,221,152,199,15,160,174,61,142,113,110,2,38,208,61,2,200,53,159,49,182,147,43,198,82,37,99,118,194,85,18,150,156,135,134,134,70,100,60,121,242,228,205,31,255,248,199,63,
121,198,25,103,60,158,95,187,87,245,190,16,241,147,79,62,185,36,237,132,239,25,30,30,30,74,101,64,5,9,87,237,138,171,68,92,37,97,228,154,199,8,155,190,216,230,220,4,76,160,28,1,100,27,175,72,91,140,81,
196,202,227,110,88,121,43,17,179,35,150,140,15,149,225,180,51,190,250,67,31,250,208,178,120,221,94,229,61,23,241,79,126,242,147,207,60,246,216,99,183,166,191,140,27,135,132,115,17,35,99,9,152,87,20,18,
47,239,136,145,48,81,114,69,176,49,23,100,218,143,20,120,183,230,57,210,251,240,249,38,208,15,4,16,102,55,238,37,206,133,124,53,47,185,98,190,35,150,132,145,113,124,53,129,128,137,65,196,250,203,188,131,
23,94,120,225,103,47,189,244,210,219,186,113,223,71,50,71,79,69,188,124,249,242,223,75,18,254,71,137,55,137,120,164,32,99,201,87,185,196,139,136,145,240,88,119,196,2,213,169,64,59,29,119,36,240,125,174,
9,188,151,8,68,193,182,90,119,28,71,174,152,151,78,118,196,18,112,124,45,129,136,147,132,37,226,145,114,193,5,23,252,135,197,139,23,223,210,234,158,154,238,235,153,136,215,172,89,243,129,187,239,190,123,
101,250,94,224,33,36,172,136,136,21,37,96,10,239,137,145,112,220,13,179,19,38,74,162,136,52,143,0,165,157,186,163,9,152,64,111,9,32,93,238,130,122,140,200,56,223,17,179,27,230,245,132,228,171,194,78,88,
17,9,43,34,97,197,244,61,200,195,159,250,212,167,22,206,159,63,255,121,174,93,58,14,150,190,160,174,151,36,56,229,250,235,175,191,45,137,247,109,18,70,200,236,134,145,49,18,30,237,142,248,208,181,70,150,
88,37,222,170,182,145,193,254,143,9,152,64,81,2,200,54,94,148,182,24,17,177,98,171,29,113,220,176,85,109,218,178,235,12,45,93,186,244,182,228,131,143,164,121,119,199,190,82,121,79,68,124,235,173,183,254,
237,142,29,59,206,137,187,95,36,28,219,216,13,43,34,227,8,88,59,96,118,193,68,201,149,34,136,200,150,8,216,188,78,187,163,9,152,64,111,8,32,92,174,78,61,70,229,177,68,25,179,27,86,196,19,185,23,152,91,
145,121,52,135,124,36,47,165,230,255,20,199,148,202,139,139,120,197,138,21,151,220,127,255,253,95,139,194,141,175,36,98,142,136,145,112,221,142,88,82,205,129,35,90,162,128,198,188,73,192,165,174,211,228,
26,60,183,9,228,4,16,98,222,222,68,61,94,139,28,113,198,88,39,98,249,32,47,122,46,121,54,171,230,216,176,97,195,215,146,159,126,176,104,209,162,71,154,88,83,171,57,139,139,56,125,171,218,95,32,216,40,
227,170,156,113,85,34,174,131,12,108,128,19,129,144,215,105,119,52,1,19,232,31,2,200,87,119,68,30,229,73,46,17,43,103,23,204,174,56,250,1,39,176,186,120,46,34,231,60,249,41,141,251,56,99,75,197,162,34,
126,250,233,167,63,124,223,125,247,45,145,96,37,94,68,75,157,182,24,37,97,245,215,237,134,1,14,108,162,0,70,233,198,60,135,219,170,47,31,235,186,9,152,64,247,8,72,138,117,71,236,35,71,162,68,68,42,15,
40,199,7,196,232,3,93,39,63,79,231,32,97,197,87,95,125,117,137,60,117,238,185,231,62,81,119,95,77,180,23,21,113,250,106,243,117,228,139,96,169,19,145,48,117,198,213,137,88,160,5,29,224,68,193,82,206,17,
115,218,186,29,75,92,163,219,247,236,249,76,96,180,4,144,226,104,207,27,237,248,120,29,114,68,74,148,72,149,71,33,71,9,235,154,122,46,243,243,25,143,132,245,29,22,114,142,162,60,149,78,187,102,180,247,
123,36,227,139,137,88,223,174,118,251,237,183,127,90,66,69,174,200,182,46,50,142,115,120,1,31,255,22,84,144,163,136,5,67,109,81,138,49,143,176,234,218,227,24,231,38,96,2,205,19,64,148,249,149,98,59,185,
34,37,23,49,207,126,254,108,199,115,115,9,75,198,124,187,155,92,180,101,203,150,79,203,87,37,191,157,173,152,136,159,123,238,185,37,8,181,147,136,156,227,88,68,204,87,60,98,132,207,7,64,212,7,27,243,252,
131,142,245,78,199,197,115,156,155,128,9,140,158,0,98,108,119,102,28,71,174,72,145,3,36,86,61,187,121,137,115,51,94,99,217,5,71,183,40,151,115,36,100,229,242,85,58,191,216,247,21,23,19,241,198,141,27,
175,205,23,78,93,0,200,137,72,55,175,199,221,112,149,136,5,31,161,18,227,7,82,213,22,251,157,155,128,9,244,150,0,194,141,119,65,91,140,200,181,149,128,53,135,198,201,35,138,173,68,140,115,20,211,207,64,
215,171,137,255,19,239,161,201,188,136,136,19,168,169,215,93,119,221,18,45,16,177,198,24,219,129,65,63,245,24,17,176,98,213,135,128,108,137,17,96,85,91,236,119,110,2,38,208,123,2,8,151,59,161,30,163,114,
149,42,7,196,243,24,39,9,227,26,237,138,113,76,85,76,63,11,253,227,105,222,169,233,220,157,204,213,100,44,34,226,244,67,222,207,79,139,29,31,101,26,115,129,160,30,115,218,98,140,18,30,139,136,115,152,
22,115,78,196,117,19,40,75,64,162,204,143,252,185,100,76,140,202,41,249,249,212,233,207,37,28,157,82,149,203,87,242,86,154,231,65,230,106,50,22,17,241,230,205,155,47,168,90,172,218,114,241,74,174,106,
207,133,91,87,143,95,13,5,138,15,144,8,188,188,78,187,163,9,152,64,111,9,212,61,155,72,87,119,167,49,170,231,17,209,230,43,160,61,122,67,94,209,78,184,206,69,180,227,160,77,155,54,189,187,68,156,126,217,
231,169,44,142,24,23,157,183,229,125,234,175,42,250,80,242,194,135,22,63,24,141,241,97,2,38,112,116,17,64,186,220,53,117,34,237,138,242,67,60,114,17,203,41,236,138,113,9,158,169,138,26,147,126,65,241,
172,56,103,147,121,145,29,241,238,221,187,79,139,139,39,87,4,66,108,171,203,245,1,208,71,174,168,67,49,230,57,52,250,242,118,215,77,192,4,250,147,0,59,224,186,187,211,51,173,49,18,172,14,185,65,135,218,
148,19,113,6,49,58,71,57,237,49,170,61,137,248,180,145,9,11,252,167,136,136,211,143,186,156,29,23,153,231,2,218,170,77,253,140,33,143,81,156,168,195,76,245,86,71,187,254,86,231,186,207,4,76,160,251,4,
36,206,120,228,207,168,250,99,27,117,185,3,25,171,95,245,216,167,58,237,185,103,90,213,229,173,120,63,77,230,69,68,156,126,15,221,220,86,11,166,15,88,49,42,175,43,128,161,95,117,229,28,49,167,205,209,
4,76,160,63,9,180,122,94,17,171,238,60,207,213,198,185,138,85,69,142,81,123,140,121,142,135,104,79,34,158,171,185,75,28,69,68,156,254,217,242,44,22,9,140,188,78,123,21,196,188,77,96,234,218,232,235,4,
158,230,240,97,2,38,208,123,2,146,107,171,131,103,21,9,19,227,57,180,225,6,57,70,59,101,234,68,68,139,131,98,212,24,234,201,91,167,198,249,155,204,139,136,24,0,44,50,214,71,147,3,34,63,71,237,106,139,
113,164,18,218,169,59,154,128,9,244,31,1,158,223,252,206,114,65,107,28,194,37,114,14,125,170,51,159,34,5,1,83,175,138,113,76,202,91,127,117,224,194,93,136,191,122,203,221,133,137,90,77,81,181,96,218,226,
194,105,235,36,198,235,105,188,14,34,121,172,143,12,240,127,76,192,4,142,42,2,184,32,222,52,207,53,145,62,198,214,69,141,203,251,218,249,135,185,155,142,61,23,113,21,156,8,139,254,24,171,160,232,28,142,
152,211,230,104,2,38,112,244,18,192,9,172,160,238,25,143,227,200,91,69,205,215,174,159,107,54,25,139,191,154,104,181,104,245,233,32,198,156,182,24,149,83,31,57,241,8,254,211,173,121,142,224,22,124,170,
9,188,103,8,228,175,28,198,186,240,248,220,198,57,99,59,115,199,54,229,237,10,231,149,136,69,68,92,181,144,8,37,246,211,14,164,216,71,206,24,234,49,182,234,99,92,39,99,24,235,104,2,38,208,125,2,85,207,
96,20,105,221,21,117,94,221,184,186,62,181,115,189,152,199,107,228,237,121,61,142,109,34,47,34,98,22,165,168,35,198,188,175,137,69,50,39,215,165,238,104,2,38,208,63,4,120,62,235,68,59,218,59,197,45,85,
231,113,173,124,76,94,175,58,183,137,182,34,239,136,227,141,3,32,182,181,202,59,1,51,218,57,91,93,207,125,38,96,2,189,37,208,233,243,220,233,184,186,213,28,233,249,117,243,142,165,189,184,136,199,114,
147,62,199,4,76,192,4,222,205,4,138,139,184,234,143,29,85,109,64,87,95,171,126,141,107,215,207,92,142,38,96,2,253,79,160,211,231,185,147,113,173,198,180,234,43,77,169,200,59,98,45,152,162,5,2,160,170,
173,73,0,92,183,159,254,72,210,228,122,61,183,9,28,77,4,120,62,187,117,207,173,230,163,79,145,92,215,165,30,219,186,117,63,173,230,41,34,226,120,3,44,144,152,247,69,16,85,99,52,94,237,117,50,109,213,199,
181,170,230,173,155,143,115,28,77,192,4,186,71,160,234,25,236,100,246,86,231,213,245,169,61,47,249,181,232,143,237,117,243,197,49,221,202,139,136,152,69,178,176,88,143,121,213,162,98,63,185,164,169,156,
163,27,18,141,243,49,175,163,9,152,64,127,19,192,9,60,191,117,49,95,69,60,175,85,158,159,215,84,189,200,59,226,86,11,109,215,215,233,194,53,15,7,115,82,119,52,1,19,56,186,9,228,207,116,124,222,243,149,
209,23,35,231,143,38,230,243,54,89,47,46,98,253,52,164,209,192,104,53,22,48,26,163,131,24,219,243,54,250,28,77,192,4,250,159,0,207,127,188,83,158,105,162,250,24,215,205,200,207,56,142,215,110,42,47,242,
106,34,45,104,127,42,131,72,184,46,182,131,8,4,141,139,175,39,200,99,100,172,162,198,183,58,186,241,106,163,213,252,238,51,1,19,120,39,129,118,207,101,126,70,28,79,174,24,115,206,161,189,42,182,242,143,
250,232,79,231,238,103,190,166,99,17,17,79,156,56,113,67,250,225,240,239,19,20,22,89,21,51,8,181,95,229,16,110,132,67,27,145,190,78,36,171,251,242,97,2,38,208,159,4,242,231,147,186,98,44,186,251,88,175,
202,171,188,163,113,184,39,246,203,91,165,136,20,17,241,164,73,147,214,165,5,190,47,46,54,207,129,65,140,64,244,163,234,104,207,69,27,65,197,62,4,172,243,58,57,24,223,201,88,143,49,1,19,56,50,2,157,62,
151,241,42,241,28,229,117,5,119,168,159,28,223,208,70,61,143,177,127,242,228,201,235,226,245,155,204,75,137,120,99,190,224,188,174,95,115,157,183,169,46,9,43,74,148,42,202,117,168,125,180,71,43,217,234,
3,240,97,2,38,208,123,2,117,207,98,108,87,158,23,252,161,246,170,156,182,86,49,122,40,109,32,55,150,162,81,68,196,199,30,123,236,139,90,60,139,84,164,228,80,212,174,223,160,26,219,217,17,231,224,75,65,
242,117,76,192,4,202,18,208,179,158,31,180,197,152,59,65,117,185,131,246,232,17,229,56,40,182,227,34,250,136,242,86,126,15,77,213,139,136,120,234,212,169,27,89,28,139,6,10,117,98,4,68,174,62,118,196,17,
132,218,216,25,11,124,28,67,61,142,87,155,142,86,59,227,56,222,185,9,152,64,57,2,60,159,85,87,140,125,202,169,43,202,19,196,152,203,27,56,36,70,92,19,35,253,156,163,40,111,85,221,75,19,109,69,68,60,119,
238,220,71,87,173,90,117,120,23,28,1,144,11,4,185,162,4,171,130,92,213,150,31,8,149,49,250,48,200,53,150,15,139,113,156,79,59,117,71,19,48,129,254,36,16,159,85,242,24,149,203,29,196,232,145,152,71,183,
116,146,235,220,57,115,230,60,82,138,74,17,17,47,92,184,112,229,210,165,75,119,39,0,83,128,48,56,56,56,34,94,69,138,94,73,208,159,139,56,10,22,56,8,54,246,197,92,227,84,231,131,227,188,186,200,124,117,
253,110,55,1,19,56,114,2,157,62,143,92,41,142,87,78,157,92,210,84,81,93,254,160,142,75,234,34,222,81,127,204,85,31,26,26,218,125,238,185,231,62,195,61,52,29,139,136,56,1,26,254,206,119,190,115,231,222,
189,123,63,23,23,172,28,72,202,247,239,223,63,2,4,9,199,29,49,146,140,31,130,250,85,71,190,68,65,99,124,158,119,19,104,188,70,55,231,245,92,38,112,52,17,224,153,108,226,158,227,220,228,138,177,32,97,69,
249,68,49,186,37,138,22,255,196,254,152,51,246,164,147,78,186,51,93,99,184,137,53,85,205,89,68,196,186,240,236,217,179,255,233,229,151,95,254,28,226,157,48,97,194,136,120,137,218,13,43,207,37,28,111,90,
240,53,78,135,114,1,207,101,29,101,172,113,185,44,243,186,198,248,48,1,19,232,31,2,122,182,227,17,235,202,243,130,136,145,48,142,201,165,27,235,202,229,27,21,114,250,117,190,124,21,239,161,233,188,152,
136,207,62,251,236,101,43,86,172,56,176,111,223,190,241,200,151,133,171,46,193,82,144,107,190,248,248,1,32,108,181,33,95,162,206,67,184,68,230,202,235,180,59,154,128,9,244,7,1,61,211,241,160,30,163,114,
21,36,172,168,130,132,21,163,95,170,132,75,63,49,140,57,32,95,197,123,104,58,47,38,226,153,51,103,110,186,229,150,91,190,249,194,11,47,252,23,189,130,64,198,200,87,17,185,34,75,34,16,128,175,200,88,164,
173,177,177,232,156,120,126,204,153,47,198,118,253,113,172,115,19,48,129,35,39,160,231,184,213,17,251,201,21,99,169,19,177,228,42,25,7,185,190,35,87,95,44,58,71,245,180,27,254,166,124,213,234,222,186,
221,87,76,196,186,241,197,139,23,127,99,195,134,13,255,57,137,120,156,196,43,33,35,98,196,154,11,49,126,0,124,213,83,100,60,81,231,197,162,235,197,185,98,174,62,142,186,118,250,29,77,192,4,154,37,192,
51,158,95,37,182,147,43,198,18,157,160,156,29,113,20,49,178,77,127,1,167,191,132,59,92,104,207,226,65,121,42,191,151,166,235,69,69,60,107,214,172,245,55,223,124,243,141,73,198,95,202,5,140,80,181,224,
92,142,2,47,200,18,55,224,25,79,212,57,121,169,154,43,159,187,105,192,158,223,4,76,96,116,4,144,46,103,81,143,2,38,199,7,196,40,226,248,202,65,2,150,112,137,228,136,25,25,39,71,125,91,158,226,218,165,
98,81,17,107,81,233,171,205,223,220,118,219,109,95,76,34,30,159,203,56,74,50,194,7,178,98,60,71,185,68,172,243,136,200,88,215,98,62,98,108,83,238,195,4,76,160,255,8,240,236,235,206,200,99,84,174,34,31,
16,37,96,213,91,137,24,233,198,136,128,15,137,249,192,165,151,94,250,183,189,32,82,92,196,233,155,164,215,220,119,223,125,127,244,244,211,79,255,223,42,121,70,8,85,144,181,43,214,121,72,88,57,5,9,35,94,
234,204,73,59,245,60,182,235,207,199,187,110,2,38,48,54,2,122,182,91,29,177,95,57,117,114,197,184,65,67,194,138,241,181,4,239,125,227,14,56,253,84,181,1,21,9,153,168,60,253,5,221,31,201,79,173,238,171,
169,190,226,34,214,66,174,188,242,202,111,109,218,180,233,170,244,237,108,191,45,249,69,33,171,31,216,128,230,171,157,228,171,28,9,19,243,57,84,167,104,62,229,28,49,167,205,209,4,76,160,127,8,32,93,221,
17,57,78,136,81,126,80,61,74,88,126,104,39,226,184,35,38,79,127,57,119,187,188,212,43,10,61,17,113,130,247,230,182,109,219,190,244,253,239,127,255,162,36,198,83,145,99,14,25,192,233,91,222,70,4,172,221,
176,32,75,192,72,152,221,112,148,185,230,163,8,44,243,231,121,55,160,199,185,187,49,159,231,48,129,163,137,128,158,217,110,31,113,78,242,232,6,114,54,106,68,94,75,32,99,118,195,138,18,46,187,98,118,193,
196,244,195,125,94,254,244,167,63,253,165,52,239,232,127,164,99,151,22,223,19,17,235,222,103,204,152,241,234,243,207,63,127,245,143,126,244,163,135,83,117,154,218,170,0,71,9,199,239,178,64,196,138,146,
97,39,34,174,147,102,93,187,238,201,135,9,152,64,179,4,244,220,231,7,109,49,226,7,162,4,172,92,226,101,211,134,140,37,95,10,239,129,217,253,18,37,226,244,51,135,119,164,157,240,213,242,81,126,15,37,235,
239,36,80,242,234,233,90,171,87,175,62,235,222,123,239,125,112,247,238,221,39,15,15,15,15,80,36,224,88,144,48,81,226,69,198,72,152,40,177,82,180,156,40,218,152,215,45,181,147,49,117,231,186,221,4,76,160,
61,1,4,91,53,50,246,145,43,198,130,132,227,110,24,25,75,192,236,138,145,48,17,9,43,78,153,50,101,203,85,87,93,245,177,5,11,22,60,91,117,31,37,219,122,46,98,45,54,189,43,158,119,199,29,119,60,184,115,231,
206,185,146,175,100,156,75,88,2,70,194,18,112,148,48,2,38,34,97,132,154,199,28,48,253,121,187,235,38,96,2,205,19,64,182,241,74,180,197,24,69,172,28,9,19,217,13,35,225,124,71,204,171,9,197,244,35,46,215,
93,123,237,181,31,59,245,212,83,215,198,235,246,42,239,11,17,107,241,155,55,111,254,181,31,254,240,135,223,217,186,117,235,39,16,177,196,43,33,35,225,40,98,73,55,202,88,50,205,69,172,121,213,30,69,27,
115,245,231,71,187,254,124,188,235,38,96,2,163,39,128,96,171,206,140,125,228,138,177,72,190,170,71,9,43,71,198,72,88,81,226,37,106,39,124,226,137,39,222,247,91,191,245,91,127,112,202,41,167,188,82,117,
253,94,180,245,141,136,181,248,36,193,113,143,62,250,232,53,233,103,82,92,159,126,217,232,41,117,18,174,218,17,231,18,150,80,41,128,205,37,155,215,25,231,104,2,38,80,150,0,194,141,87,165,45,10,56,230,
72,152,136,132,137,185,140,211,251,224,205,139,22,45,250,234,197,23,95,172,159,172,246,214,183,82,197,139,246,40,239,43,17,195,32,237,138,167,47,95,190,252,207,214,174,93,251,199,73,198,19,217,17,35,224,
184,19,150,128,41,136,55,143,154,55,151,110,94,231,218,142,38,96,2,189,33,128,120,185,58,245,40,223,152,75,192,170,75,188,85,50,70,196,105,71,252,198,188,121,243,190,161,127,76,150,118,195,219,153,191,
159,98,95,138,24,64,73,150,199,166,29,242,229,107,214,172,249,252,47,127,249,203,207,36,41,143,252,107,60,137,55,202,88,82,109,183,35,206,197,155,215,185,166,163,9,152,64,239,8,32,95,221,1,57,242,165,
13,1,199,168,28,33,43,38,249,30,72,63,83,248,182,249,243,231,127,47,237,128,31,72,115,236,234,221,170,218,95,185,175,69,28,111,255,181,215,94,59,46,237,144,207,126,253,245,215,79,221,177,99,199,233,250,
139,61,149,36,228,9,73,170,58,70,118,189,18,114,58,14,30,138,111,219,9,211,22,231,173,202,53,151,15,19,48,129,102,8,32,216,86,179,75,172,28,140,63,212,54,14,1,31,18,244,184,36,222,125,250,203,55,149,105,
211,166,109,60,238,184,227,94,78,59,224,85,199,31,127,252,235,204,225,104,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,
2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,
2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,
2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,
2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,
2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,112,148,19,24,119,180,220,255,250,245,235,231,175,93,187,246,
188,109,219,182,157,190,115,231,206,185,59,118,236,152,191,119,239,222,247,237,223,191,127,194,193,131,7,7,222,124,243,205,129,24,15,173,235,160,162,250,116,168,191,213,209,174,191,213,185,238,51,1,19,
232,156,192,184,113,173,213,67,255,49,199,28,195,164,35,39,168,93,109,49,14,14,14,238,155,52,105,210,191,79,155,54,109,205,212,169,83,215,205,152,49,99,227,188,121,243,158,154,51,103,206,26,78,238,247,
216,154,70,143,239,126,203,150,45,179,126,254,243,159,127,242,133,23,94,248,74,18,239,135,147,116,7,14,28,56,48,34,86,162,36,139,132,115,17,171,142,92,137,90,82,204,123,188,68,95,222,4,76,32,16,64,192,
106,34,87,140,57,34,86,155,114,202,248,241,227,71,114,197,36,231,129,36,230,39,222,255,254,247,223,240,193,15,126,240,159,78,62,249,228,151,194,101,250,46,237,75,17,75,192,75,151,46,253,251,77,155,54,
253,190,228,75,145,124,41,146,111,148,177,228,26,133,140,108,21,201,69,63,230,173,62,141,78,199,181,154,195,125,38,96,2,111,17,64,166,111,181,84,103,140,171,138,106,83,65,198,85,18,150,136,145,177,132,
172,50,115,230,204,127,184,242,202,43,255,164,95,133,220,87,34,78,242,27,90,182,108,217,151,86,173,90,117,221,240,240,240,228,125,251,246,29,150,48,187,97,98,20,177,164,137,132,137,136,52,143,250,232,
105,171,254,223,192,173,38,96,2,189,34,128,124,117,125,242,24,115,9,83,143,187,225,92,194,18,241,132,9,19,6,134,134,134,246,156,125,246,217,255,109,201,146,37,223,78,231,13,247,106,141,85,215,237,27,17,
175,89,179,102,193,253,247,223,255,255,182,111,223,126,78,146,240,128,36,156,139,24,9,199,93,177,196,75,145,96,171,10,11,207,5,156,215,25,231,104,2,38,80,150,0,178,229,170,177,174,188,170,176,27,38,178,
19,174,19,241,33,25,15,76,159,62,253,153,43,174,184,226,119,230,207,159,191,154,235,245,58,246,133,136,211,14,248,188,7,30,120,224,254,221,187,119,31,47,249,70,17,231,50,70,194,241,181,68,39,34,206,165,
155,215,123,253,65,248,250,38,240,94,39,16,229,43,22,212,171,36,172,54,4,76,172,19,177,4,76,73,187,226,145,124,202,148,41,175,93,126,249,229,87,164,29,242,83,253,192,189,231,34,126,234,169,167,22,63,244,
208,67,247,165,239,128,152,44,1,83,216,17,19,217,13,19,59,21,177,32,35,93,98,14,190,174,61,31,231,186,9,152,64,119,9,32,219,124,86,218,99,84,30,11,2,38,34,98,189,138,96,87,44,1,171,142,128,21,41,233,59,
45,246,92,118,217,101,159,56,239,188,243,150,231,215,47,93,239,169,136,159,120,226,137,43,126,252,227,31,223,155,36,60,40,1,191,241,198,27,45,69,28,37,60,22,17,11,110,59,233,182,235,47,253,1,249,122,38,
240,110,35,128,92,235,214,21,251,201,163,128,201,17,48,17,17,35,97,222,13,87,137,120,226,196,137,35,66,78,50,222,191,120,241,226,79,156,127,254,249,15,212,221,79,137,246,193,18,23,169,186,198,134,13,27,
78,187,61,29,73,190,131,8,152,88,181,43,150,132,91,137,88,2,141,127,81,135,80,243,200,189,208,78,221,209,4,76,160,183,4,144,46,119,65,61,198,40,97,229,72,88,177,74,196,218,176,105,87,156,191,190,212,53,
228,128,52,199,224,35,143,60,114,71,242,209,89,179,103,207,126,145,107,151,142,61,17,113,2,48,225,166,155,110,250,65,218,9,79,71,190,138,20,137,56,190,43,70,194,173,68,156,75,24,209,230,49,2,166,47,182,
57,55,1,19,40,79,0,217,198,43,211,22,163,114,21,137,151,136,140,163,136,37,224,88,16,49,158,224,58,135,230,155,158,190,93,246,7,201,7,75,82,125,31,125,37,99,79,68,124,215,93,119,253,217,214,173,91,127,
163,78,194,236,136,17,114,39,34,150,84,171,138,96,230,194,205,235,37,129,251,90,38,96,2,245,4,36,198,120,80,63,36,204,183,189,35,86,27,18,206,119,196,185,132,245,204,75,194,249,193,188,242,145,188,148,
250,255,50,31,83,162,254,246,85,23,184,226,47,126,241,139,115,239,185,231,158,159,167,221,240,64,44,236,134,145,51,18,174,250,203,58,1,21,232,252,171,92,46,98,45,7,233,18,243,37,214,181,231,227,92,55,
1,19,104,134,0,178,205,103,167,61,70,196,73,204,69,172,122,254,151,117,241,59,38,244,23,117,122,63,76,73,239,136,7,98,185,250,234,171,63,120,230,153,103,62,157,223,75,211,245,226,59,226,199,31,127,252,
127,198,29,47,57,34,166,94,39,226,252,43,29,127,212,32,34,86,69,114,65,140,121,132,90,215,30,199,56,55,1,19,104,142,0,162,205,175,16,219,17,175,198,144,75,186,202,145,49,175,38,228,2,229,56,129,24,159,
245,56,7,231,43,62,246,216,99,255,35,93,226,247,242,123,105,186,94,84,196,207,61,247,220,153,119,223,125,247,239,178,203,229,61,112,148,110,149,136,171,94,77,8,46,69,128,171,138,224,69,248,85,245,166,
1,123,126,19,48,129,209,17,144,36,227,65,29,121,230,17,145,106,147,134,140,181,43,142,78,96,62,206,213,57,202,57,151,243,54,111,222,252,89,121,234,140,51,206,248,5,231,148,136,69,69,156,190,218,252,119,
201,87,98,69,194,185,148,99,157,60,23,49,2,38,10,184,114,192,11,28,2,38,2,51,175,211,238,104,2,38,208,31,4,36,200,120,80,143,17,137,18,17,106,116,66,244,1,243,105,124,60,7,1,75,220,42,114,78,250,83,187,
222,21,255,71,206,41,17,139,137,248,165,151,94,154,115,243,205,55,127,1,9,107,193,228,121,68,192,68,68,204,107,9,96,19,1,30,163,224,69,233,198,188,83,176,99,57,167,211,185,61,206,4,222,43,4,36,190,177,
28,241,60,114,69,138,158,127,229,72,152,24,61,16,159,225,56,86,110,145,132,21,17,48,50,126,229,149,87,254,32,249,234,235,179,102,205,90,63,150,251,30,203,57,197,68,188,114,229,202,143,35,84,162,32,32,
225,92,186,140,137,145,191,160,35,234,131,16,104,34,208,243,40,48,180,141,5,146,207,49,1,19,40,79,64,226,228,32,143,17,177,234,249,71,194,146,107,254,172,107,28,133,113,138,236,134,145,113,116,77,250,
177,11,75,210,181,111,226,250,77,199,98,34,222,184,113,227,167,226,66,59,205,217,5,199,40,240,20,65,87,81,93,7,117,192,229,31,74,187,118,250,29,77,192,4,202,18,64,178,249,85,99,59,66,37,234,249,86,206,
115,175,40,193,114,48,78,81,242,149,119,144,112,43,7,165,95,68,241,233,52,199,187,75,196,9,206,164,235,174,187,238,234,40,83,65,80,29,24,244,85,213,233,35,70,9,43,143,31,130,62,0,213,99,28,169,132,255,
208,31,154,156,154,128,9,244,1,1,9,179,234,160,61,70,229,42,18,107,238,128,56,7,99,228,22,36,156,187,39,247,206,171,175,190,122,117,154,115,82,58,119,111,156,171,169,188,200,142,120,197,138,21,139,210,
66,135,16,169,22,13,136,216,70,59,109,138,18,109,94,71,196,185,132,17,44,81,208,98,62,86,136,221,152,99,172,215,246,121,38,112,180,18,144,0,71,123,84,61,107,154,135,118,230,84,164,168,79,130,205,15,245,
227,15,245,171,200,37,242,140,118,205,122,39,76,29,199,224,160,20,135,228,173,52,231,163,249,188,77,212,139,136,56,189,150,184,136,133,142,53,70,249,198,92,31,2,69,128,248,192,136,57,180,186,246,124,156,
235,38,96,2,71,70,96,172,207,26,178,229,234,204,163,118,229,49,42,151,96,229,132,120,168,93,174,81,100,12,34,238,212,65,201,91,31,73,115,190,123,68,156,126,217,231,105,124,101,138,16,98,27,114,85,63,237,
180,213,69,125,40,121,225,195,224,195,171,171,211,238,104,2,38,208,95,4,120,118,37,208,170,67,253,177,79,126,136,117,157,163,186,10,238,144,87,180,11,198,47,106,199,69,85,185,218,146,183,78,175,186,126,
19,109,69,118,196,123,246,236,153,21,129,212,229,17,18,99,98,212,7,160,186,34,69,117,29,212,201,71,26,179,255,104,140,15,19,48,129,254,39,32,137,142,246,121,197,1,138,242,66,20,177,234,81,198,209,53,117,
185,188,85,138,84,41,17,207,22,136,209,20,96,234,28,0,147,231,81,176,24,3,56,213,59,57,58,29,215,201,92,30,99,2,38,48,122,2,18,102,126,140,229,185,148,23,56,16,185,218,240,133,230,164,222,73,220,181,107,
215,108,230,107,58,150,18,241,220,78,22,46,80,17,22,245,186,8,28,250,85,87,206,17,115,218,28,77,192,4,250,139,64,187,231,20,81,51,14,201,86,173,66,99,170,10,254,137,125,180,213,197,244,243,111,230,86,
93,163,137,182,34,34,78,63,63,226,116,45,182,21,4,245,1,36,142,235,52,23,28,141,141,113,164,82,243,31,198,214,116,187,217,4,76,160,48,1,132,155,95,54,62,171,72,152,24,199,210,214,202,25,56,166,46,234,
92,250,146,136,223,93,239,136,211,194,198,197,5,106,161,173,96,213,245,69,232,113,140,218,85,143,113,164,18,218,169,59,154,128,9,244,39,1,158,225,252,238,162,160,53,6,225,18,53,158,156,177,209,15,163,
201,163,155,228,173,252,94,154,170,191,243,155,239,26,184,210,104,64,116,58,54,222,166,206,209,65,36,143,245,145,1,254,143,9,152,192,81,71,32,127,142,169,19,181,32,229,77,148,82,176,250,94,196,64,142,
177,10,78,254,161,84,141,113,155,9,152,192,209,73,0,201,114,247,241,121,167,141,72,95,140,156,63,218,200,156,77,199,34,239,136,227,226,181,160,88,143,121,213,98,213,207,57,121,228,220,170,243,186,209,
198,181,187,49,151,231,48,129,247,42,1,94,23,52,177,254,248,140,198,215,19,180,19,117,237,60,87,189,85,105,226,126,235,230,44,34,226,120,113,96,16,243,190,28,76,236,39,175,58,183,147,62,198,40,182,154,
35,142,115,110,2,38,112,100,4,58,121,214,58,149,181,230,170,27,91,215,167,118,238,33,230,113,85,85,237,156,19,199,53,149,23,17,49,139,100,97,49,198,188,169,69,198,121,185,94,108,115,110,2,38,208,91,2,
241,185,172,19,237,104,239,48,206,153,159,75,159,34,185,198,80,143,109,249,185,77,212,139,188,35,142,55,94,181,192,170,54,206,1,12,245,170,216,234,252,56,190,211,113,241,28,231,38,96,2,101,9,116,250,156,
118,50,174,213,152,86,125,101,87,60,48,80,92,196,165,23,232,235,153,128,9,152,64,191,19,40,46,226,209,254,177,67,227,219,157,211,174,159,15,161,211,113,140,119,52,1,19,120,247,18,232,39,31,20,121,71,172,
5,83,244,177,2,128,54,98,137,143,156,107,115,173,126,250,227,9,247,228,104,2,239,37,2,249,51,217,233,218,219,157,215,202,43,156,155,143,161,78,127,167,247,114,164,227,138,136,56,222,36,11,36,230,125,17,
68,213,24,141,87,123,157,64,91,245,197,107,145,215,93,131,126,98,221,245,232,119,52,1,19,120,139,64,167,207,213,91,103,116,150,181,154,183,174,79,237,244,197,60,94,177,170,157,115,226,184,166,242,34,34,
102,145,44,44,214,99,94,181,200,120,142,250,85,151,20,105,87,91,46,73,198,168,175,91,71,188,94,183,230,244,60,38,96,2,71,70,64,207,37,69,51,241,156,230,49,246,145,115,94,93,212,184,82,71,145,119,196,117,
11,237,164,93,32,52,46,198,145,74,246,31,198,208,156,215,105,119,52,1,19,56,250,8,224,10,238,188,213,243,77,95,140,156,63,218,200,245,154,142,125,47,226,58,112,17,76,4,94,213,30,219,156,155,128,9,28,93,
4,120,190,185,107,234,196,216,174,182,110,22,230,110,58,22,121,53,145,126,87,212,193,4,103,156,126,103,20,101,44,176,128,193,185,212,121,85,17,35,125,26,91,119,228,175,52,234,198,185,221,4,76,160,89,2,
173,158,211,120,101,198,197,168,156,194,88,234,163,141,242,19,231,164,95,173,116,128,249,154,142,69,68,60,52,52,180,33,253,218,145,57,44,80,17,33,19,99,91,28,87,149,35,220,8,135,54,162,250,218,137,86,
115,251,48,1,19,232,127,2,241,89,37,207,221,160,85,228,109,121,29,223,212,69,141,167,111,194,132,9,47,150,34,83,228,213,196,164,73,147,214,177,184,86,17,104,140,161,94,23,129,68,127,252,32,98,78,127,30,
57,223,209,4,76,160,247,4,242,231,51,214,117,119,212,243,60,214,25,147,199,42,167,208,86,23,39,79,158,188,78,115,151,56,138,236,136,167,76,153,178,177,110,177,85,237,64,164,79,63,172,89,109,170,107,151,
171,168,54,98,167,160,242,29,178,230,244,97,2,38,208,159,4,170,158,79,218,20,201,229,1,229,121,164,141,246,88,87,91,187,146,68,188,161,20,153,34,34,78,11,122,137,69,235,87,90,215,229,244,241,107,175,25,
167,40,137,70,9,199,15,33,23,108,41,120,190,142,9,152,64,115,4,120,198,185,66,172,43,175,42,114,133,218,163,59,200,163,95,200,213,87,151,167,13,228,203,92,187,233,88,68,196,83,167,78,29,217,17,179,96,
197,170,28,40,252,218,107,118,189,106,71,196,17,136,250,227,161,15,0,41,199,156,49,85,109,244,57,154,128,9,244,7,1,61,167,85,7,237,49,42,87,65,182,117,49,250,134,49,117,30,98,172,188,85,117,31,77,180,
21,17,241,233,167,159,254,211,85,171,86,141,200,151,197,183,139,200,24,1,19,115,8,180,19,245,161,40,215,17,115,206,83,155,15,19,48,129,163,135,64,124,102,201,21,99,65,174,49,182,115,76,187,126,121,171,
20,165,34,34,94,180,104,209,138,101,203,150,13,167,133,15,13,14,14,30,22,114,204,115,40,234,147,80,181,235,85,31,114,141,96,244,65,168,31,9,51,134,168,177,49,143,231,214,181,199,49,206,77,192,4,202,18,
64,180,249,85,105,143,81,185,138,228,75,84,30,93,146,215,213,39,183,68,247,84,229,169,109,88,222,202,239,163,169,122,17,17,39,72,123,111,188,241,198,123,210,175,167,190,54,7,1,20,69,237,130,37,214,88,
16,38,81,32,248,0,52,78,185,250,98,209,24,198,19,213,86,117,180,235,175,58,199,109,38,96,2,221,37,160,231,184,238,136,125,60,251,121,172,219,9,231,146,173,171,227,33,36,126,194,9,39,220,147,174,177,183,
238,158,186,221,94,68,196,186,233,180,205,191,123,203,150,45,215,178,224,170,152,139,56,202,21,97,234,3,208,56,62,8,181,199,93,177,174,197,88,98,108,83,238,195,4,76,160,255,9,232,25,231,32,143,81,121,
213,110,88,109,85,194,173,114,78,93,219,156,57,115,238,226,218,37,98,49,17,47,92,184,240,159,87,174,92,121,248,143,5,2,144,190,97,122,96,255,254,253,35,109,202,227,78,56,151,171,96,8,60,18,142,99,5,94,
210,141,69,227,187,37,226,56,143,230,245,97,2,38,48,122,2,72,180,211,51,227,120,114,197,188,196,221,176,114,118,181,117,146,149,107,40,120,40,31,123,246,217,103,47,235,244,62,187,49,174,152,136,103,205,
154,181,254,123,223,251,222,119,55,110,220,248,5,4,44,9,43,103,39,76,68,168,44,48,7,31,37,140,176,137,72,51,143,204,69,59,117,71,19,48,129,254,34,128,116,185,43,234,49,226,132,92,194,136,56,138,21,233,
230,17,9,199,118,181,205,156,57,243,187,242,21,215,47,17,139,137,88,139,185,240,194,11,255,122,211,166,77,95,136,2,150,124,17,112,148,105,20,38,208,21,5,58,142,207,207,209,121,20,93,51,206,83,85,87,155,
15,19,48,129,254,34,128,116,117,87,228,120,64,109,242,0,245,40,227,170,221,112,20,109,204,211,143,94,56,188,51,86,59,98,150,167,116,141,146,71,81,17,159,113,198,25,191,248,238,119,191,123,235,75,47,189,
244,89,4,92,37,213,8,0,216,138,2,46,137,11,118,60,79,178,141,66,214,249,81,198,212,227,188,117,109,249,24,215,77,192,4,154,39,160,231,187,234,160,61,70,229,42,200,24,17,203,11,202,37,84,229,136,85,81,
210,165,72,186,72,152,136,160,79,57,229,148,91,229,169,170,123,105,178,173,168,136,181,144,244,213,230,127,221,115,207,61,135,69,44,129,82,242,221,171,198,71,216,146,176,234,146,176,64,115,94,148,48,2,
38,106,14,29,85,115,87,181,253,106,180,255,107,2,38,80,154,0,178,141,215,165,13,249,170,175,74,194,81,198,185,136,145,44,34,158,56,113,226,97,41,211,134,156,229,167,120,253,82,121,113,17,159,121,230,153,
79,223,121,231,157,95,127,254,249,231,255,2,129,18,243,69,3,92,144,37,94,149,86,59,105,228,27,163,230,204,133,155,215,243,235,186,110,2,38,208,59,2,200,87,119,64,174,24,243,184,65,139,18,198,19,146,49,
5,201,18,145,47,145,246,249,243,231,127,93,126,234,197,202,171,255,60,208,240,157,36,17,78,184,233,166,155,150,165,111,103,251,141,244,189,197,3,121,25,30,30,30,216,183,111,223,0,81,59,97,74,43,17,35,
116,68,172,101,32,93,98,190,180,186,246,124,156,235,38,96,2,205,17,64,178,241,10,180,197,168,156,130,128,137,72,88,17,9,43,34,90,162,118,196,177,164,159,14,57,112,210,73,39,253,248,139,95,252,226,146,
52,247,190,120,15,165,242,226,59,98,45,76,139,221,176,97,195,231,110,191,253,246,103,83,117,122,148,161,32,3,86,81,80,37,101,32,119,42,98,93,71,243,50,55,81,237,28,85,109,244,57,154,128,9,148,35,160,231,
62,63,104,139,81,185,138,220,64,196,23,56,34,138,88,242,165,176,3,86,228,245,132,98,18,241,246,43,175,188,242,115,105,190,158,72,88,235,238,137,136,117,225,217,179,103,191,248,179,159,253,236,218,229,
203,151,223,151,132,56,40,168,58,20,35,96,224,182,219,17,231,187,97,73,22,209,18,53,127,204,85,31,203,209,141,57,198,114,93,159,99,2,71,11,1,158,231,177,220,111,60,151,92,49,47,8,152,136,43,234,68,204,
142,56,19,241,254,75,46,185,228,90,249,104,44,247,218,173,115,222,249,101,168,91,51,119,56,207,83,79,61,181,248,161,135,30,186,111,239,222,189,147,245,42,130,162,93,112,44,18,177,118,195,68,137,87,117,
69,36,76,148,40,145,101,204,117,75,180,119,120,123,35,195,198,114,206,104,230,247,88,19,120,47,16,64,170,237,214,26,199,145,87,73,88,109,81,194,202,163,132,149,179,27,38,198,93,113,218,9,239,185,236,178,
203,62,113,222,121,231,45,111,119,79,77,247,247,92,196,90,96,250,201,108,231,61,240,192,3,247,167,95,167,116,124,149,136,217,13,35,97,94,79,32,97,34,210,141,81,243,35,82,162,218,56,170,218,232,115,52,
1,19,104,158,0,178,141,87,162,45,70,229,177,32,97,98,213,142,152,119,196,185,136,211,207,72,127,237,242,203,47,191,34,253,11,186,167,226,117,123,149,247,133,136,181,248,53,107,214,44,72,63,161,237,246,
29,59,118,156,29,255,146,78,187,226,78,68,44,161,230,59,98,36,155,199,8,155,190,216,230,220,4,76,160,44,1,132,203,85,169,199,24,37,172,28,1,19,91,137,24,33,107,71,60,125,250,244,103,174,184,226,138,223,
73,223,37,177,154,235,245,58,246,141,136,5,34,73,113,232,222,123,239,253,202,115,207,61,247,119,73,198,147,115,9,199,29,177,164,219,233,171,137,67,115,31,102,61,86,249,142,245,188,195,23,118,98,2,239,
65,2,200,180,147,165,199,177,228,49,42,87,145,124,137,202,37,97,162,242,248,93,19,65,194,123,210,63,214,248,211,171,174,186,234,134,116,238,112,39,247,83,106,76,95,137,152,69,167,111,107,155,181,116,233,
210,191,79,255,28,250,247,235,118,195,81,194,236,132,137,18,38,69,115,70,129,198,156,235,85,181,209,231,104,2,38,208,28,1,201,52,63,98,27,185,98,44,185,136,163,132,217,25,71,25,167,159,31,241,15,233,59,
35,254,228,228,147,79,126,41,191,94,63,212,223,73,161,31,238,234,208,61,72,200,43,86,172,248,212,218,181,107,191,188,125,251,246,15,243,110,56,151,48,2,38,86,73,24,217,18,59,93,230,104,199,119,58,175,
199,153,192,123,141,0,82,109,183,238,56,14,249,234,28,114,197,42,17,231,50,78,175,32,158,152,55,111,222,183,210,15,120,191,187,95,5,12,139,190,22,49,55,169,184,126,253,250,249,73,200,139,182,109,219,118,
218,206,157,59,231,166,119,201,243,211,119,90,188,47,237,152,39,72,150,72,152,120,232,220,131,170,115,212,73,181,174,157,243,28,77,192,4,154,33,16,165,27,175,16,219,37,216,244,140,166,166,119,190,150,
80,91,218,249,238,75,223,1,241,239,211,166,77,91,147,126,207,220,186,25,51,102,188,152,4,188,34,253,76,225,53,113,78,231,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,
38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,
38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,
38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,
38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,
38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,38,96,2,71,49,129,
113,71,203,189,191,246,218,107,199,253,219,191,253,219,217,59,119,238,60,117,199,142,29,167,167,56,87,229,192,129,3,19,14,254,234,24,72,97,224,205,55,223,212,146,14,30,138,35,109,172,145,54,234,49,234,
92,31,38,96,2,229,8,140,27,87,175,159,99,142,57,230,240,141,48,238,80,219,56,69,181,29,42,227,198,143,31,191,111,234,212,169,235,84,166,77,155,182,49,197,151,127,253,215,127,125,213,241,199,31,255,250,
225,73,250,60,169,39,209,7,55,158,228,120,236,163,143,62,122,249,154,53,107,62,191,101,203,150,207,236,223,191,127,124,18,239,128,138,164,26,11,18,86,164,104,9,49,167,222,110,105,58,199,135,9,152,64,119,
9,32,212,118,179,50,174,42,30,146,239,0,50,86,140,37,73,121,64,101,112,112,240,192,201,39,159,124,219,252,249,243,191,119,241,197,23,63,144,206,219,213,238,186,189,236,239,75,17,111,221,186,117,250,242,
229,203,255,108,237,218,181,127,188,111,223,190,137,73,192,3,42,72,152,216,109,17,91,192,189,252,95,209,215,126,175,16,64,176,117,235,141,253,136,87,99,201,21,115,17,75,190,106,67,196,135,100,44,33,15,
76,152,48,225,141,121,243,230,125,99,241,226,197,127,115,226,137,39,110,175,187,110,47,219,251,74,196,73,132,227,210,14,248,154,21,43,86,92,191,103,207,158,83,146,132,71,4,28,69,140,144,145,176,164,44,
129,170,30,35,82,205,163,96,211,86,18,124,47,174,89,114,125,190,214,187,139,64,148,97,169,149,229,215,164,174,24,243,92,194,236,136,145,177,228,27,69,124,72,198,3,147,39,79,222,188,104,209,162,175,166,
29,242,157,105,190,190,250,99,111,223,136,120,243,230,205,191,246,195,31,254,240,59,105,55,252,137,225,225,225,1,36,76,140,50,230,213,4,177,74,194,18,31,242,35,234,127,168,152,199,255,193,234,218,227,
24,231,38,96,2,221,35,128,92,243,25,99,187,114,234,228,138,185,140,145,176,98,149,132,145,241,208,208,208,192,9,39,156,176,244,234,171,175,254,194,41,167,156,242,74,126,237,94,213,251,66,196,47,191,252,
242,188,59,238,184,227,65,253,229,155,196,139,136,149,215,137,56,74,152,221,177,100,154,23,129,69,178,196,8,187,170,45,246,59,55,1,19,104,150,0,162,141,87,161,45,70,229,148,209,138,56,189,158,208,43,138,
1,137,88,81,127,177,119,237,181,215,126,236,212,83,79,93,27,175,219,171,188,231,34,94,189,122,245,89,247,222,123,239,131,187,119,239,62,89,2,166,32,97,34,175,36,136,146,111,148,177,132,202,206,88,57,69,
96,163,108,99,94,7,189,147,49,117,231,186,221,4,76,160,154,0,82,173,238,125,171,53,142,67,188,234,37,87,108,37,226,248,106,2,1,19,37,98,202,148,41,83,182,92,117,213,85,31,91,176,96,193,179,111,93,189,
55,89,79,69,252,252,243,207,159,247,163,31,253,232,225,244,62,120,218,27,111,188,113,88,194,236,136,21,37,222,184,43,150,124,41,236,132,137,200,55,70,97,141,98,141,121,21,242,118,253,85,231,184,205,4,
76,160,51,2,81,178,117,103,196,49,228,138,121,225,221,48,145,215,18,241,213,132,4,44,49,179,19,70,194,19,39,78,28,105,75,239,141,119,252,230,111,254,230,71,63,240,129,15,60,85,119,63,37,218,7,75,92,164,
234,26,219,182,109,59,225,251,223,255,254,61,72,24,17,19,217,9,19,243,119,196,113,55,204,78,152,136,136,117,93,196,74,140,109,85,247,229,54,19,48,129,222,16,64,186,92,157,122,140,200,56,223,17,171,94,
37,98,121,66,50,150,27,112,6,126,56,228,132,105,75,151,46,189,39,249,232,156,25,51,102,188,202,181,75,199,158,136,56,1,56,38,73,248,219,187,118,237,58,85,187,94,201,151,194,171,9,118,197,236,134,21,5,
178,234,213,132,128,230,18,6,182,128,30,2,62,194,54,230,165,97,251,122,38,96,2,173,9,32,93,70,81,71,192,49,70,25,75,194,200,88,59,96,121,66,2,150,51,40,185,35,116,13,205,39,15,221,117,215,93,223,78,110,
248,221,84,31,249,23,97,92,191,84,236,137,136,211,87,160,47,111,218,180,233,183,145,112,148,47,59,226,40,98,100,156,139,88,96,99,65,190,68,65,140,226,141,121,4,92,215,30,199,56,55,1,19,232,46,1,73,176,
238,136,125,202,169,147,43,74,188,121,145,144,229,9,34,187,225,40,97,93,51,206,163,92,62,146,151,82,215,245,117,247,212,100,123,113,17,175,95,191,126,254,109,183,221,246,191,163,124,17,50,18,86,148,124,
145,49,175,37,216,13,199,175,112,0,38,34,97,228,74,20,196,152,119,19,106,83,243,118,243,30,61,151,9,116,155,128,4,214,196,145,207,139,52,117,45,114,197,186,29,177,252,160,93,113,213,38,45,222,47,115,33,
243,85,171,86,253,239,228,167,127,158,51,103,206,154,56,174,68,94,92,196,250,23,115,73,176,227,37,90,74,148,50,50,110,39,226,8,185,74,194,200,145,40,152,49,47,1,215,215,48,1,19,24,61,1,9,50,30,212,17,
103,140,72,84,242,85,206,123,226,232,7,158,123,162,230,214,28,156,203,57,41,142,151,159,82,183,118,198,69,143,162,34,126,233,165,151,230,252,224,7,63,248,195,42,1,71,241,230,185,234,117,187,97,128,11,
114,44,162,8,120,98,36,91,213,22,251,157,155,128,9,148,35,128,108,243,43,210,30,163,114,10,50,37,226,3,98,124,206,149,215,157,135,140,211,191,105,248,195,228,169,191,156,53,107,214,250,252,94,154,172,
23,21,113,250,106,163,159,29,49,78,98,205,11,175,33,216,29,171,95,185,4,172,188,149,136,5,56,47,130,198,135,64,204,65,214,181,231,227,92,55,1,19,104,150,0,162,205,175,66,123,140,200,148,136,132,21,17,
176,34,207,55,81,115,231,231,32,224,240,189,199,227,228,169,52,244,191,230,247,210,100,189,152,136,211,203,240,153,233,59,37,190,38,161,34,215,92,198,212,233,143,81,57,239,134,21,115,224,130,77,17,48,
224,19,99,91,147,64,61,183,9,152,192,145,17,64,186,204,66,61,198,40,84,185,0,25,227,128,248,220,107,158,120,46,99,145,176,162,68,172,34,7,109,216,176,225,107,201,87,127,61,115,230,204,77,220,67,211,177,
152,136,211,139,240,37,105,145,227,37,212,118,5,33,43,198,177,136,88,224,149,11,54,95,249,226,7,16,63,132,152,11,102,94,111,26,176,231,55,1,19,24,29,1,164,201,89,212,99,84,174,130,132,21,37,212,232,1,
206,39,106,188,36,140,128,163,91,148,203,55,135,100,60,94,190,74,231,221,204,185,77,199,98,34,78,95,101,62,41,121,86,45,30,225,18,243,49,212,217,9,19,5,189,74,196,130,22,133,27,243,78,128,142,118,124,
39,115,122,140,9,188,215,9,32,210,209,112,136,231,144,43,198,162,231,85,117,69,100,156,95,67,253,249,78,88,190,97,55,140,132,229,22,21,249,42,205,241,238,18,113,2,52,244,141,111,124,227,26,132,26,35,11,
87,155,114,250,104,207,163,196,171,54,65,87,30,69,44,248,106,87,225,136,57,109,121,236,100,76,126,142,235,38,96,2,71,78,64,130,108,119,196,49,202,169,147,235,249,149,100,121,142,137,154,151,49,234,151,
91,226,142,88,187,95,156,131,103,240,207,47,127,249,203,107,210,60,67,233,252,225,118,247,215,141,254,34,59,226,39,159,124,114,97,250,138,51,133,197,198,197,179,112,197,216,159,231,72,87,237,228,2,94,
37,98,129,137,31,70,204,187,1,205,115,152,128,9,116,135,64,187,103,83,34,101,12,185,162,14,69,114,141,145,100,57,212,46,87,48,70,125,42,209,43,184,71,109,49,87,61,125,163,192,148,167,159,126,250,156,52,
223,147,204,217,100,44,34,226,244,237,32,23,71,0,85,185,132,90,213,174,182,216,135,132,17,48,81,144,244,97,240,161,81,175,130,23,199,84,245,187,205,4,76,160,55,4,16,43,87,231,89,85,123,204,169,51,158,
200,121,138,106,147,63,20,171,68,92,231,27,156,147,254,113,199,37,105,154,119,143,136,211,207,25,62,157,197,177,248,40,87,218,24,19,101,203,56,132,27,251,244,193,196,34,248,124,88,68,181,113,84,181,209,
231,104,2,38,208,123,2,60,163,185,88,213,30,219,98,93,78,136,125,90,133,234,242,137,98,116,134,242,232,27,114,198,224,32,69,121,171,20,145,34,59,226,244,67,53,78,3,0,49,7,144,131,160,174,40,232,42,49,
167,45,143,128,83,123,126,84,181,229,99,92,55,1,19,232,111,2,122,142,115,241,234,142,163,11,228,10,237,130,163,71,148,35,218,216,142,139,232,35,202,91,165,72,20,17,241,222,189,123,79,143,11,175,202,89,
124,222,39,184,121,155,234,17,122,204,1,167,182,78,142,78,199,117,50,151,199,152,128,9,140,141,64,46,214,177,60,151,242,2,7,59,97,98,149,67,170,218,162,135,228,45,230,107,58,150,18,241,220,170,69,199,
54,129,87,157,24,115,181,117,82,4,75,227,226,145,215,99,159,115,19,48,129,254,32,208,234,57,69,210,140,81,157,60,191,123,181,87,21,92,163,62,242,170,24,251,211,207,74,159,155,207,223,84,189,136,136,211,
15,242,153,13,156,92,176,177,14,24,198,214,69,193,200,251,104,139,81,121,187,67,243,248,48,1,19,232,61,1,132,155,223,73,124,70,145,48,49,142,165,45,119,67,172,227,152,232,29,245,87,181,203,91,113,254,
38,243,34,34,78,139,28,100,161,44,186,42,70,96,85,57,32,212,167,131,49,228,49,42,231,96,60,117,71,19,48,129,254,35,80,245,156,230,114,214,24,132,75,212,74,200,25,143,27,90,197,92,198,26,27,61,149,234,
69,252,168,251,127,235,27,239,84,107,232,136,48,88,124,108,27,107,206,237,234,124,29,196,216,158,183,209,231,104,2,38,208,255,4,112,67,188,83,158,105,162,250,24,215,205,40,87,149,58,138,139,120,180,160,
198,10,34,126,72,99,157,195,231,153,128,9,244,7,1,188,193,221,180,122,190,233,139,145,243,71,19,185,86,137,88,100,235,29,23,175,69,197,122,204,171,22,28,251,201,243,57,168,115,190,198,53,125,148,184,70,
211,107,240,252,38,208,45,2,188,18,232,214,124,157,206,19,159,195,248,122,130,118,162,230,203,115,213,219,149,78,239,227,72,199,21,17,113,188,73,96,16,243,190,28,76,236,39,175,58,183,147,62,198,16,91,
205,195,24,71,19,48,129,246,4,186,245,44,181,19,186,174,83,55,166,174,79,237,220,95,204,227,170,170,218,57,39,142,107,42,47,34,98,22,201,194,98,140,121,83,139,140,243,114,189,216,230,220,4,76,160,63,8,
240,124,214,201,54,222,165,198,182,26,167,126,230,139,231,41,167,61,31,67,157,254,252,188,166,234,69,222,17,199,155,175,90,96,85,27,231,0,134,250,145,196,86,215,57,146,121,125,174,9,152,192,209,71,160,
159,124,80,92,196,71,223,199,229,59,54,1,19,48,129,102,9,20,23,113,171,63,74,84,45,85,227,71,123,78,213,60,106,235,214,60,117,243,187,221,4,76,224,232,33,208,79,62,40,242,142,24,153,178,240,24,243,190,
166,63,70,174,221,79,127,44,105,122,205,158,223,4,142,6,2,60,155,157,222,107,187,241,184,165,106,62,206,205,199,80,167,191,234,220,38,218,138,136,184,234,198,235,22,74,59,64,234,206,173,19,169,206,171,
235,139,115,113,157,216,70,222,201,249,140,117,52,1,19,232,140,64,171,103,174,179,25,90,255,169,182,110,126,181,211,23,243,120,205,170,118,206,137,227,154,202,139,136,152,69,118,18,181,208,42,0,121,155,
234,18,38,177,155,128,242,107,117,115,110,207,101,2,38,208,61,2,122,86,41,113,214,170,103,56,182,113,78,171,24,231,107,58,47,242,142,184,221,98,1,212,201,56,198,10,76,39,121,211,0,61,191,9,152,64,25,2,
117,207,123,188,58,14,81,27,249,145,196,56,119,147,121,207,69,172,31,222,44,80,68,229,58,170,224,1,130,49,121,61,182,115,62,99,28,77,192,4,142,78,2,249,179,204,115,78,100,85,177,206,57,49,106,92,172,119,
146,51,119,211,177,200,171,137,36,217,131,105,209,58,222,38,92,213,171,74,148,178,250,117,196,113,188,146,0,14,245,24,233,227,124,234,138,26,231,195,4,76,160,255,8,84,61,175,241,46,243,254,232,5,141,107,
85,207,189,18,199,146,199,49,242,86,188,118,147,121,17,17,79,152,48,225,229,180,168,89,44,82,49,47,2,65,63,80,136,106,71,178,180,229,80,232,39,170,191,78,184,154,195,135,9,152,192,209,67,32,62,179,228,
184,160,46,182,242,73,238,31,234,154,139,124,104,104,232,229,82,132,138,188,154,152,60,121,242,58,22,151,199,184,112,242,24,149,215,21,32,209,175,122,85,78,27,145,243,28,77,192,4,250,139,0,207,104,30,
117,151,180,229,121,172,51,38,143,242,142,218,136,49,207,157,196,152,73,147,38,173,211,220,37,142,34,59,226,180,160,13,105,113,23,87,45,152,69,231,125,128,82,187,126,46,40,245,184,227,85,155,142,216,86,
85,167,77,81,7,231,253,170,230,255,154,128,9,244,43,129,252,89,165,30,163,242,88,112,138,218,240,74,204,105,107,23,229,173,82,92,138,136,120,202,148,41,47,178,232,241,227,199,31,134,163,54,213,243,54,
198,198,40,217,170,168,77,135,228,172,156,118,197,186,67,125,250,32,124,152,128,9,28,93,4,242,231,150,186,98,94,240,133,218,201,171,98,116,78,238,30,234,138,199,30,123,236,139,165,104,21,17,113,90,208,
75,2,18,23,169,188,85,145,104,145,173,206,69,184,17,140,250,227,161,15,0,233,70,49,211,30,199,58,55,1,19,232,111,2,122,110,227,65,61,70,229,42,85,194,141,109,209,53,180,199,54,114,245,41,87,148,183,226,
245,155,204,139,136,120,230,204,153,63,251,215,127,253,215,90,241,178,120,0,40,234,215,90,3,76,177,74,196,2,67,59,81,31,138,114,34,240,84,143,135,198,248,48,1,19,232,31,2,249,51,26,239,140,190,24,149,
83,228,8,229,209,25,121,30,253,162,188,93,145,183,226,61,52,153,23,17,241,69,23,93,244,179,229,203,151,31,72,11,215,241,14,0,131,131,131,3,251,247,239,31,105,87,174,157,174,198,41,34,216,58,8,244,199,
200,135,149,203,54,214,25,83,55,175,219,77,192,4,122,75,32,127,70,169,43,230,37,74,183,202,49,180,201,47,228,173,98,26,119,64,222,42,69,160,136,136,19,180,157,55,220,112,195,178,244,235,169,63,33,16,117,
69,187,96,21,1,210,152,40,215,8,36,126,8,81,214,136,54,143,156,75,59,117,71,19,48,129,254,36,160,103,60,30,212,99,196,3,81,194,202,115,193,226,27,218,169,215,69,141,155,49,99,198,178,52,255,206,120,15,
77,230,69,68,172,5,156,126,250,233,119,188,250,234,171,29,137,24,185,18,115,0,124,0,196,40,108,100,75,212,185,49,207,231,106,213,151,143,117,221,4,76,160,251,4,244,28,183,58,232,143,145,103,95,177,78,
196,136,150,141,29,245,78,226,156,57,115,110,111,117,79,221,238,43,38,226,51,206,56,99,217,170,85,171,14,239,118,171,96,176,35,70,192,185,36,129,175,113,228,250,16,226,120,206,201,99,14,142,254,188,221,
117,19,48,129,222,17,208,115,157,31,180,197,24,159,127,229,200,152,93,111,148,111,250,7,101,181,127,10,207,61,196,249,11,22,44,184,63,191,143,38,235,197,68,60,127,254,252,231,111,186,233,166,59,95,121,
229,149,107,244,62,88,0,4,8,249,42,74,168,185,84,163,48,129,175,24,199,214,157,19,207,141,121,59,160,163,25,219,110,46,247,155,128,9,188,147,128,158,225,118,71,28,67,174,152,23,36,172,136,72,163,136,145,
173,124,211,170,224,164,147,78,58,233,78,249,170,221,253,117,179,191,152,136,117,211,31,254,240,135,255,242,222,123,239,189,70,48,36,227,40,225,92,196,44,50,66,143,192,17,49,81,242,140,69,231,71,161,198,
156,185,243,49,177,221,185,9,152,64,25,2,72,182,234,106,244,197,88,231,132,42,17,203,53,18,108,250,231,202,35,18,38,230,66,70,194,138,242,84,213,189,52,217,86,84,196,231,158,123,238,19,55,222,120,227,
178,45,91,182,44,169,146,48,34,141,11,6,186,32,75,222,200,24,1,19,57,55,70,205,147,11,56,175,199,107,57,55,1,19,232,61,1,164,203,157,80,199,5,49,226,3,34,59,98,9,149,130,116,145,176,34,37,239,59,225,132,
19,150,201,83,92,187,84,44,42,98,45,234,67,31,250,208,215,239,191,255,254,195,34,174,18,41,139,7,56,144,21,163,192,227,185,202,163,132,17,46,81,115,198,156,107,56,154,128,9,244,31,1,228,171,59,35,199,
7,68,249,64,57,126,64,194,249,107,137,40,91,4,156,75,153,49,242,83,47,104,20,23,241,162,69,139,30,185,229,150,91,190,185,110,221,186,175,69,145,34,81,192,71,192,128,142,34,214,185,188,206,208,185,237,
68,156,75,56,175,247,2,190,175,105,2,38,240,22,1,61,243,241,160,30,163,114,74,20,177,228,171,58,50,206,119,195,18,109,148,176,242,137,19,39,190,173,109,246,236,217,223,148,159,226,61,148,202,223,190,242,
66,87,77,18,156,114,253,245,215,255,116,219,182,109,231,236,221,187,119,32,125,127,241,225,50,60,60,60,64,217,183,111,223,0,69,175,37,242,247,202,136,28,9,19,145,172,34,185,150,22,115,150,90,213,70,159,
163,9,152,64,243,4,16,109,188,82,108,67,188,234,39,143,18,86,30,37,28,119,196,236,116,115,17,75,194,148,244,195,125,244,125,195,207,124,245,171,95,253,72,154,127,119,188,143,82,121,241,29,241,33,152,187,
215,172,89,243,153,187,239,190,123,101,18,225,80,92,44,160,21,35,96,9,89,95,229,162,140,91,137,24,193,230,49,94,139,190,216,230,220,4,76,160,55,4,244,204,199,131,122,140,202,85,90,137,88,158,144,140,37,
95,229,200,56,238,136,145,240,161,56,124,229,149,87,126,38,205,219,19,9,107,205,61,17,177,46,172,111,15,73,255,236,249,243,143,63,254,248,63,170,206,17,65,243,199,12,73,88,185,36,44,176,188,39,142,34,
142,187,97,9,150,162,121,17,46,145,107,197,216,170,47,142,115,110,2,38,208,29,2,122,214,235,14,250,98,196,13,68,54,106,68,124,161,40,79,68,9,231,50,142,34,62,255,252,243,63,95,250,219,213,242,117,215,
147,200,71,54,84,255,201,79,126,242,153,199,30,123,236,214,244,122,98,28,175,36,20,121,37,65,100,39,76,228,253,112,148,49,242,141,81,183,141,100,137,249,82,234,218,243,113,174,155,128,9,116,159,0,178,
141,51,211,22,35,2,38,34,96,34,34,150,128,145,49,2,38,102,187,226,131,23,94,120,225,103,47,189,244,210,219,226,181,123,145,247,92,196,90,116,18,241,146,71,30,121,228,158,36,224,33,100,140,128,21,37,95,
10,187,225,78,69,140,100,137,64,206,235,180,59,154,128,9,244,134,0,210,229,234,212,17,111,30,17,48,17,17,35,225,170,93,113,16,241,240,37,151,92,114,117,18,241,50,174,215,203,216,23,34,22,128,103,158,121,
230,130,135,30,122,232,159,246,236,217,115,138,228,43,33,75,190,71,42,98,205,141,116,137,106,211,145,215,127,213,234,255,154,128,9,148,38,128,116,185,46,245,24,163,136,37,95,213,145,176,98,43,17,243,154,
66,34,78,191,186,109,243,101,151,93,246,201,115,206,57,231,113,174,215,235,216,55,34,22,136,173,91,183,78,127,248,225,135,255,124,237,218,181,127,202,142,152,157,176,162,118,193,68,189,146,96,87,44,161,
242,142,152,136,100,243,168,235,208,166,188,169,163,196,53,154,186,119,207,107,2,57,1,132,152,183,119,187,30,175,67,30,163,242,40,97,213,37,96,132,204,110,152,152,239,138,231,205,155,247,119,31,253,232,
71,255,234,196,19,79,220,222,237,123,63,146,249,250,74,196,44,228,217,103,159,61,231,209,71,31,189,33,125,123,219,197,185,136,121,53,17,69,140,124,137,146,32,34,140,185,230,167,157,107,85,197,78,198,84,
157,231,54,19,48,129,246,4,16,107,221,200,216,175,156,58,185,98,148,49,187,97,34,59,99,36,172,152,190,61,237,209,139,47,190,248,43,103,157,117,214,51,117,215,237,101,123,95,138,88,64,146,12,143,89,189,
122,245,185,233,149,197,103,95,124,241,197,47,167,191,204,59,185,106,55,44,249,34,96,98,148,47,82,37,30,154,251,136,152,199,185,142,104,34,159,108,2,239,50,2,72,115,172,203,138,231,147,199,168,92,37,23,
49,18,38,74,190,233,59,35,182,156,118,218,105,223,74,175,32,110,77,63,77,237,233,116,222,219,127,183,218,88,111,178,129,243,250,86,196,113,173,73,124,131,43,87,174,188,96,227,198,141,31,217,190,125,251,
220,93,187,118,189,127,231,206,157,11,210,63,6,153,159,118,200,67,8,152,168,115,37,75,10,245,108,206,88,117,110,2,38,208,99,2,8,151,219,160,174,72,81,31,18,86,219,33,241,14,167,127,148,177,102,234,212,
169,171,211,239,153,123,97,250,244,233,235,210,207,63,255,233,194,133,11,31,79,99,246,51,159,163,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,
128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,
128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,
128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,
128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,128,9,152,192,123,128,192,255,7,240,159,65,235,62,86,2,190,0,0,0,0,73,69,78,68,174,66,96,130,0,0};

const char* PlayerBackgroundComponent::menu_activated_icon_png = (const char*) resource_PlayerBackgroundComponent_menu_activated_icon_png;
const int PlayerBackgroundComponent::menu_activated_icon_pngSize = 17153;

// JUCER_RESOURCE: menu_icon_png2, 17759, "../../SpatCommon/Resources/Menu_icon.png"
static const unsigned char resource_PlayerBackgroundComponent_menu_icon_png2[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,1,98,0,0,1,98,8,6,0,0,0,204,92,210,122,0,0,0,1,115,82,71,66,0,174,206,
28,233,0,0,0,9,112,72,89,115,0,0,46,35,0,0,46,35,1,120,165,63,118,0,0,3,170,105,84,88,116,88,77,76,58,99,111,109,46,97,100,111,98,101,46,120,109,112,0,0,0,0,0,60,120,58,120,109,112,109,101,116,97,32,120,
109,108,110,115,58,120,61,34,97,100,111,98,101,58,110,115,58,109,101,116,97,47,34,32,120,58,120,109,112,116,107,61,34,88,77,80,32,67,111,114,101,32,53,46,52,46,48,34,62,10,32,32,32,60,114,100,102,58,82,
68,70,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,47,50,50,45,114,100,102,45,115,121,110,116,97,120,45,110,115,35,
34,62,10,32,32,32,32,32,32,60,114,100,102,58,68,101,115,99,114,105,112,116,105,111,110,32,114,100,102,58,97,98,111,117,116,61,34,34,10,32,32,32,32,32,32,32,32,32,32,32,32,120,109,108,110,115,58,120,109,
112,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,34,10,32,32,32,32,32,32,32,32,32,32,32,32,120,109,108,110,115,58,116,105,102,102,61,34,104,116,
116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,116,105,102,102,47,49,46,48,47,34,10,32,32,32,32,32,32,32,32,32,32,32,32,120,109,108,110,115,58,101,120,105,102,61,34,104,116,116,112,58,
47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,101,120,105,102,47,49,46,48,47,34,62,10,32,32,32,32,32,32,32,32,32,60,120,109,112,58,77,111,100,105,102,121,68,97,116,101,62,50,48,49,55,45,49,48,45,
49,50,84,50,49,58,49,48,58,49,53,60,47,120,109,112,58,77,111,100,105,102,121,68,97,116,101,62,10,32,32,32,32,32,32,32,32,32,60,120,109,112,58,67,114,101,97,116,111,114,84,111,111,108,62,80,105,120,101,
108,109,97,116,111,114,32,51,46,55,60,47,120,109,112,58,67,114,101,97,116,111,114,84,111,111,108,62,10,32,32,32,32,32,32,32,32,32,60,116,105,102,102,58,79,114,105,101,110,116,97,116,105,111,110,62,49,
60,47,116,105,102,102,58,79,114,105,101,110,116,97,116,105,111,110,62,10,32,32,32,32,32,32,32,32,32,60,116,105,102,102,58,67,111,109,112,114,101,115,115,105,111,110,62,53,60,47,116,105,102,102,58,67,111,
109,112,114,101,115,115,105,111,110,62,10,32,32,32,32,32,32,32,32,32,60,116,105,102,102,58,82,101,115,111,108,117,116,105,111,110,85,110,105,116,62,50,60,47,116,105,102,102,58,82,101,115,111,108,117,116,
105,111,110,85,110,105,116,62,10,32,32,32,32,32,32,32,32,32,60,116,105,102,102,58,89,82,101,115,111,108,117,116,105,111,110,62,51,48,48,60,47,116,105,102,102,58,89,82,101,115,111,108,117,116,105,111,110,
62,10,32,32,32,32,32,32,32,32,32,60,116,105,102,102,58,88,82,101,115,111,108,117,116,105,111,110,62,51,48,48,60,47,116,105,102,102,58,88,82,101,115,111,108,117,116,105,111,110,62,10,32,32,32,32,32,32,
32,32,32,60,101,120,105,102,58,80,105,120,101,108,88,68,105,109,101,110,115,105,111,110,62,51,53,52,60,47,101,120,105,102,58,80,105,120,101,108,88,68,105,109,101,110,115,105,111,110,62,10,32,32,32,32,
32,32,32,32,32,60,101,120,105,102,58,67,111,108,111,114,83,112,97,99,101,62,49,60,47,101,120,105,102,58,67,111,108,111,114,83,112,97,99,101,62,10,32,32,32,32,32,32,32,32,32,60,101,120,105,102,58,80,105,
120,101,108,89,68,105,109,101,110,115,105,111,110,62,51,53,52,60,47,101,120,105,102,58,80,105,120,101,108,89,68,105,109,101,110,115,105,111,110,62,10,32,32,32,32,32,32,60,47,114,100,102,58,68,101,115,
99,114,105,112,116,105,111,110,62,10,32,32,32,60,47,114,100,102,58,82,68,70,62,10,60,47,120,58,120,109,112,109,101,116,97,62,10,42,234,135,15,0,0,64,0,73,68,65,84,120,1,237,125,123,176,94,247,85,157,245,
86,36,89,178,44,217,216,242,219,17,19,72,60,134,68,227,40,182,226,248,213,48,201,228,1,13,147,50,20,38,127,80,74,121,205,48,3,204,180,161,133,180,205,12,208,41,48,195,80,40,5,74,103,50,83,90,154,105,120,
36,41,132,224,72,137,172,72,138,81,66,77,28,160,120,18,63,20,197,113,100,189,98,201,122,187,191,117,239,93,186,235,238,187,127,231,156,251,248,206,247,221,123,215,111,230,124,123,237,181,247,254,61,150,
190,179,125,252,221,215,85,87,121,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,
96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,
96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,
96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,
96,5,172,128,21,176,2,243,162,192,178,121,153,101,192,147,60,248,224,131,43,47,93,186,180,115,253,250,245,59,87,174,92,121,219,138,21,43,238,40,246,91,151,47,95,126,231,178,101,203,86,151,107,108,7,109,
22,73,204,225,150,163,159,229,48,183,201,102,243,52,229,59,102,5,22,186,2,175,188,242,202,140,143,144,213,68,78,125,226,38,91,98,231,47,95,190,252,229,139,23,47,254,67,233,19,95,41,246,153,211,167,79,
31,44,125,226,224,158,61,123,46,206,120,147,67,40,24,229,70,188,252,254,251,239,255,206,210,124,223,187,110,221,186,127,86,26,221,117,104,118,188,160,21,155,95,141,211,156,136,51,191,198,129,231,224,154,
244,109,173,128,21,152,170,0,155,230,84,118,210,203,226,145,163,79,139,106,98,181,138,153,3,110,226,250,198,153,51,103,254,107,105,202,31,222,187,119,239,95,151,248,101,228,140,226,24,201,70,92,26,240,
221,155,55,111,254,157,242,212,187,147,77,150,13,144,62,196,140,152,92,147,69,12,131,243,141,123,211,253,54,158,113,91,43,96,5,154,21,96,179,140,89,145,175,249,228,155,172,198,128,163,95,158,146,15,30,
63,126,252,71,74,67,126,34,238,99,20,252,145,106,196,59,118,236,216,180,117,235,214,159,47,79,192,63,195,38,171,22,130,213,124,198,50,91,227,148,7,230,136,77,154,124,102,103,146,155,213,155,179,2,11,93,
1,54,189,46,231,200,114,35,167,62,113,147,213,24,112,230,147,47,79,200,191,122,244,232,209,15,30,58,116,232,100,151,253,246,149,51,50,141,248,129,7,30,184,183,60,5,255,113,105,108,215,107,179,141,24,194,
40,71,159,22,49,98,181,77,120,172,160,188,176,150,190,218,166,152,230,25,91,1,43,48,174,0,27,98,166,71,22,83,142,152,22,115,16,195,42,102,44,242,244,163,45,159,39,191,80,154,241,119,239,219,183,239,64,
182,183,97,112,35,209,136,223,242,150,183,124,215,150,45,91,62,86,154,221,42,52,60,94,16,132,88,109,228,233,119,177,154,19,49,124,14,172,215,117,204,36,183,235,156,206,179,2,11,65,1,54,196,46,123,173,
229,42,159,97,114,77,22,49,141,211,87,139,61,210,47,205,248,194,177,99,199,222,249,153,207,124,230,47,186,236,125,208,57,221,187,205,128,118,242,208,67,15,253,147,77,155,54,253,207,242,29,16,165,159,77,
54,97,197,88,90,125,224,200,209,239,98,53,7,152,131,243,210,167,173,241,140,219,90,1,43,144,43,192,230,24,163,25,175,28,113,23,139,28,205,163,95,227,24,47,205,248,149,19,39,78,124,127,249,206,138,63,140,
251,235,219,31,106,35,126,248,225,135,191,191,52,225,63,136,77,182,230,67,156,24,83,142,184,201,106,44,98,248,24,93,27,111,215,188,241,89,253,106,5,22,175,2,108,122,109,39,204,242,148,203,48,185,204,42,
7,204,11,251,32,110,178,165,25,95,117,242,228,201,31,216,189,123,247,31,180,237,125,144,241,161,53,226,123,239,189,247,219,110,184,225,134,39,74,51,155,242,113,68,108,180,153,15,65,148,87,159,184,201,
106,12,24,3,243,213,70,83,172,86,99,222,10,88,129,201,207,117,51,45,208,32,117,168,79,220,100,53,6,156,249,228,107,22,141,184,196,46,60,255,252,243,223,113,224,192,129,191,213,253,244,137,235,221,103,
128,187,184,251,238,187,215,223,126,251,237,159,43,223,158,246,90,109,168,51,193,216,158,230,171,79,156,89,229,34,134,143,209,165,241,118,201,25,159,205,175,86,96,105,40,192,70,216,116,218,44,71,185,12,
147,83,27,113,230,131,107,187,38,26,241,85,229,7,65,190,244,244,211,79,191,241,137,39,158,56,221,180,255,65,197,86,14,106,226,166,121,111,186,233,166,255,56,151,38,220,214,128,217,36,163,197,158,200,69,
156,249,224,48,180,102,156,241,171,21,176,2,81,129,236,62,97,131,100,46,115,148,7,71,63,195,228,104,49,87,134,57,7,215,234,98,49,15,70,249,41,188,215,150,190,244,43,165,17,255,88,151,186,249,206,233,253,
137,184,124,155,218,253,229,59,36,62,3,1,186,92,229,139,120,99,103,166,205,106,144,64,62,195,99,19,76,228,48,78,174,139,175,185,196,88,207,195,10,88,129,73,5,186,52,194,152,83,243,149,39,134,109,194,140,
211,98,103,192,124,234,37,175,54,198,202,119,82,188,229,211,159,254,244,222,201,83,245,131,122,127,34,222,184,113,227,191,195,209,216,56,155,44,154,111,22,207,234,149,35,110,178,26,3,198,104,106,174,77,
177,241,106,191,90,129,165,173,64,188,71,208,240,226,96,14,99,153,143,24,120,230,232,28,202,43,102,14,57,212,178,30,125,4,13,55,27,200,215,49,209,159,30,86,174,15,60,117,23,3,94,177,60,13,223,83,158,134,
63,135,195,227,170,53,218,182,24,235,177,93,98,88,245,137,51,171,92,196,240,57,56,39,253,38,59,147,220,166,121,28,179,2,11,69,1,54,186,46,251,173,229,42,159,97,114,106,35,206,124,112,241,138,79,191,136,
71,14,126,249,97,143,157,229,71,161,63,215,229,92,243,149,211,235,19,113,249,175,205,7,98,227,196,65,148,155,41,142,245,244,187,88,228,96,196,38,26,253,241,172,201,215,182,248,100,166,145,21,88,188,10,
100,247,1,155,98,60,53,115,107,113,228,35,167,41,206,57,187,230,49,159,150,123,160,143,181,148,163,95,250,212,47,148,156,119,50,175,15,219,219,19,241,174,93,187,94,83,190,93,237,239,112,240,120,101,79,
198,25,23,235,40,162,242,16,141,62,113,102,149,3,198,224,124,227,222,212,215,166,216,212,76,123,86,192,10,64,129,166,166,26,99,234,19,103,86,185,136,225,55,93,217,211,47,243,53,6,92,190,157,237,219,247,
239,223,255,119,125,253,75,246,246,68,188,97,195,134,183,178,65,70,139,195,70,110,38,190,214,103,152,156,218,136,51,31,28,6,246,50,202,99,212,247,55,202,218,45,244,189,177,25,141,226,57,240,190,28,149,
253,97,31,122,159,68,159,49,242,229,215,239,190,181,104,186,248,26,241,154,53,107,222,173,111,22,28,92,47,196,154,124,141,69,92,171,37,159,89,229,34,134,207,129,181,102,51,102,91,55,155,181,92,179,116,
21,232,243,125,54,155,166,138,253,101,117,53,190,237,95,114,46,117,58,183,234,198,230,139,56,249,181,107,215,162,95,253,134,214,12,18,207,174,203,204,112,71,229,99,137,171,111,188,241,198,227,229,144,
43,112,80,189,226,71,16,109,190,214,18,99,59,196,176,244,155,172,198,34,134,143,193,185,198,189,153,189,206,165,118,102,43,57,219,10,244,167,64,214,84,187,172,158,213,41,151,97,114,153,5,167,60,253,204,
234,199,14,136,171,159,225,9,238,210,215,191,254,245,205,229,55,180,125,179,203,249,230,154,211,203,71,19,229,105,248,158,210,152,166,53,97,54,43,216,218,133,3,214,98,89,189,230,19,103,86,185,136,225,
115,112,13,250,93,236,108,106,186,204,235,28,43,176,212,21,64,35,213,251,43,250,81,159,24,87,63,195,194,173,88,189,122,245,27,203,124,143,198,57,7,225,247,214,136,227,230,33,38,47,196,50,220,196,33,166,
117,89,46,227,153,85,46,98,248,28,92,135,126,87,59,219,186,174,243,59,207,10,44,20,5,164,185,13,100,203,181,249,51,94,239,75,98,230,209,98,147,192,171,86,173,186,167,192,197,211,136,203,143,15,222,136,
67,235,197,127,17,114,240,51,172,28,107,104,25,83,155,205,67,78,109,196,153,95,227,192,207,199,192,190,61,172,192,168,41,128,38,84,27,51,125,207,106,115,211,57,107,188,230,204,4,103,123,142,123,213,53,
137,163,197,154,228,74,223,218,54,147,61,204,37,183,151,39,226,114,160,155,116,147,109,141,83,5,212,220,54,140,53,152,67,76,27,231,4,143,161,124,230,143,37,37,121,228,109,173,192,98,83,32,222,19,179,61,
31,27,90,172,143,188,250,196,53,139,185,24,139,243,214,124,228,99,224,92,17,147,227,153,57,55,108,249,125,56,139,174,17,223,140,131,242,162,96,153,223,20,99,126,180,89,13,115,16,3,86,219,132,99,108,172,
48,121,225,156,73,200,148,21,88,146,10,176,201,241,240,217,61,194,70,199,28,245,21,51,174,86,227,192,24,217,26,172,137,57,177,30,181,228,52,151,92,249,198,129,155,57,215,160,109,47,79,196,229,64,183,197,
131,168,128,192,244,51,204,88,156,131,62,107,152,71,31,113,229,212,111,194,136,97,176,118,220,235,254,58,219,186,238,43,56,211,10,12,79,1,54,173,184,131,166,247,61,107,52,135,13,15,243,100,152,28,109,
204,163,15,155,141,182,181,56,175,90,157,167,252,159,252,173,234,15,18,247,210,136,219,62,35,230,1,85,56,197,136,195,111,186,116,14,214,182,89,206,203,218,204,239,26,211,60,99,43,176,152,21,224,125,213,
116,70,52,55,29,90,195,24,57,54,66,228,19,215,44,115,96,89,15,92,27,186,86,196,168,175,173,51,193,223,80,155,119,190,249,94,26,113,182,105,136,160,66,18,43,79,204,88,54,15,57,230,170,15,204,218,154,141,
249,244,181,86,185,12,115,238,44,102,206,10,44,102,5,216,220,226,25,227,61,161,121,140,145,83,31,120,162,9,166,22,235,104,126,92,55,250,154,27,113,211,58,200,45,87,111,95,77,31,74,35,142,231,163,79,171,
98,83,88,196,154,174,152,167,115,112,222,104,53,71,235,137,163,101,125,228,237,91,129,165,170,64,237,158,96,147,165,46,154,199,24,57,245,219,154,35,230,211,124,206,159,89,230,33,134,181,212,87,46,139,
101,243,13,146,235,189,17,83,124,30,42,250,20,72,227,89,14,227,180,49,135,126,205,102,235,112,46,181,172,87,174,11,158,109,93,151,185,157,99,5,134,165,64,108,102,181,125,232,251,63,214,48,70,30,126,196,
228,162,229,122,228,233,103,54,230,208,167,141,53,228,213,198,156,65,249,189,52,98,28,140,151,30,4,28,135,98,114,51,181,217,26,152,131,115,211,42,23,49,124,12,205,29,103,166,191,118,201,153,94,101,198,
10,44,92,5,154,222,243,108,166,241,116,172,137,113,240,228,50,172,28,231,172,205,197,120,180,217,28,154,211,22,215,220,65,226,241,191,67,52,200,21,102,56,55,133,70,153,98,78,3,78,47,242,153,205,234,99,
94,150,147,113,172,235,186,54,243,109,173,192,82,81,128,247,70,237,188,217,125,165,156,98,157,163,198,51,7,113,189,200,211,106,61,49,109,204,137,60,227,131,182,67,107,196,122,96,197,77,7,206,242,186,114,
58,111,86,195,120,45,6,190,22,99,173,173,21,176,2,227,15,80,181,123,165,198,119,209,45,171,237,202,117,153,127,152,57,67,107,196,109,135,142,2,71,191,173,222,113,43,96,5,22,183,2,139,169,39,140,108,35,
94,220,111,33,159,206,10,88,1,43,48,169,192,208,26,49,63,164,199,86,20,115,107,145,139,62,243,50,59,147,220,172,62,227,6,49,103,182,142,57,43,176,24,20,24,196,253,18,231,140,254,66,214,109,104,141,120,
54,162,65,120,94,172,159,205,63,134,214,40,198,156,209,231,58,140,53,197,53,215,216,10,44,69,5,112,127,52,221,35,49,166,190,226,174,218,161,38,214,69,95,231,106,138,33,175,45,174,115,205,39,238,165,17,
207,228,112,154,171,56,30,26,177,44,174,28,113,180,113,174,232,51,63,242,244,17,111,203,97,174,173,21,88,10,10,116,185,39,186,222,51,49,79,125,197,170,43,120,94,202,3,103,53,228,162,141,181,125,249,189,
124,31,177,138,161,31,176,67,4,250,93,112,155,40,181,57,178,186,152,139,28,238,5,24,113,12,229,198,8,121,97,142,80,41,108,154,35,45,48,105,5,70,72,129,174,239,243,218,150,179,250,200,69,31,115,41,87,195,
181,53,201,207,182,142,245,125,217,222,26,49,15,68,97,216,156,224,103,152,249,176,177,70,99,89,60,155,147,28,45,235,184,54,125,216,140,3,143,161,177,113,166,253,149,251,111,207,116,134,21,88,248,10,52,
189,223,179,152,114,196,209,66,21,114,196,234,71,213,16,211,120,134,201,181,217,56,247,32,252,222,27,49,15,129,195,179,169,101,152,226,48,7,117,228,56,7,109,204,161,207,121,51,139,90,228,113,78,214,128,
207,56,240,24,140,141,123,211,95,117,158,233,81,51,86,96,225,43,208,118,15,100,39,204,106,148,203,48,57,90,204,171,152,62,184,120,233,30,180,70,177,230,12,27,247,210,136,203,225,47,150,131,142,173,165,
141,10,162,208,111,194,16,137,121,153,96,42,46,242,232,19,103,22,243,196,53,57,55,215,226,60,228,97,25,83,78,113,86,163,113,99,43,176,20,20,104,186,15,98,76,125,226,204,42,71,220,164,37,114,52,47,195,
228,50,91,56,244,173,94,70,47,141,248,210,165,75,135,203,159,29,185,29,39,226,129,217,208,224,215,48,242,53,6,63,27,204,65,44,155,143,156,90,228,162,14,28,7,231,201,56,230,104,140,92,23,203,185,187,228,
58,199,10,140,138,2,179,125,191,235,254,179,57,34,167,62,113,180,152,147,28,49,252,218,85,219,3,231,104,179,151,47,95,254,170,206,49,72,220,75,35,46,7,126,182,28,226,118,61,8,69,208,102,24,49,242,53,79,
235,137,25,167,31,231,64,156,28,45,114,35,6,167,115,33,30,185,49,66,94,152,35,84,21,234,220,213,36,7,172,192,2,85,160,203,251,59,203,81,46,195,228,96,35,86,46,147,141,241,88,135,92,229,106,126,105,196,
232,91,189,140,94,26,241,196,19,241,148,3,177,137,81,144,216,24,149,71,33,253,41,147,20,39,206,131,184,206,21,243,99,60,203,205,56,206,195,245,224,215,246,196,92,91,43,176,20,21,104,186,47,178,152,114,
196,77,22,49,198,161,47,253,104,169,61,121,205,37,206,44,235,74,35,62,76,60,104,219,87,35,158,246,136,79,33,217,216,224,43,198,193,155,26,98,20,166,150,75,158,54,214,169,143,28,12,238,173,107,76,243,140,
173,128,21,24,87,32,187,143,168,141,198,218,48,226,204,201,48,185,154,197,154,140,101,152,28,45,114,49,46,94,188,120,100,12,244,240,210,75,35,46,7,58,204,195,241,76,177,233,197,70,25,125,214,53,89,173,
81,172,53,202,43,214,156,136,187,230,197,58,251,86,96,169,43,16,239,123,234,17,121,245,137,51,171,28,112,215,11,235,50,55,195,228,96,49,144,187,232,158,136,207,158,61,251,217,171,175,190,122,252,132,19,
175,56,40,71,108,116,209,71,30,184,56,116,142,24,179,63,174,64,166,155,181,89,56,10,44,244,247,120,109,255,145,87,159,56,179,202,1,243,170,253,139,50,30,235,144,207,24,49,45,249,210,183,246,129,235,99,
244,242,68,252,141,111,124,227,137,45,91,182,156,89,190,124,249,186,120,168,216,116,163,143,252,140,107,226,227,26,75,217,231,27,112,41,107,224,179,15,79,129,218,251,47,242,234,19,103,86,57,224,204,39,
159,89,40,65,94,113,194,157,57,122,244,232,255,237,75,185,21,125,44,84,26,241,165,237,219,183,223,189,98,197,138,187,116,189,248,180,22,125,228,102,156,206,49,83,12,193,107,163,22,171,241,181,121,204,
91,1,43,48,222,240,50,29,178,251,137,28,45,234,136,213,70,156,249,224,120,113,30,250,53,171,121,192,231,206,157,251,163,125,251,246,253,33,112,31,163,151,39,98,28,164,60,230,127,108,245,234,213,223,7,
204,230,10,81,116,68,31,177,140,99,13,98,156,139,28,109,83,108,38,57,154,11,92,91,143,121,182,86,96,169,43,208,116,207,66,155,24,87,159,152,150,249,244,213,2,243,210,60,114,53,171,185,53,124,230,204,153,
143,35,214,215,232,173,17,159,56,113,226,47,55,110,220,120,169,28,108,5,4,138,3,13,46,227,51,14,181,224,99,83,108,226,52,86,195,181,121,117,175,168,197,136,107,107,142,177,21,88,138,10,240,222,104,58,
123,204,81,63,195,228,212,70,156,249,224,120,97,63,196,153,213,248,4,190,116,234,212,169,79,2,247,53,122,249,104,2,135,57,114,228,200,75,119,222,121,231,117,229,39,236,222,152,29,174,214,216,50,30,98,
198,145,113,49,167,230,199,90,254,99,213,242,193,51,39,218,166,26,199,172,192,98,80,32,190,231,233,55,157,45,203,1,167,67,125,226,204,42,7,172,23,230,83,31,184,124,247,195,52,46,203,99,221,203,47,191,
252,159,247,238,221,251,191,116,111,131,198,189,61,17,227,32,47,190,248,226,175,110,219,182,237,39,10,156,242,45,16,104,182,16,33,27,145,135,159,53,103,173,101,78,205,34,151,49,214,69,159,57,140,183,173,
201,60,204,227,97,5,172,192,248,61,86,211,33,222,39,234,19,55,89,141,1,235,133,53,233,103,77,152,49,181,82,243,74,249,191,247,95,169,237,123,80,124,111,79,196,56,192,225,195,135,79,220,113,199,29,183,
149,167,226,215,235,129,186,54,57,173,81,12,65,57,20,147,83,171,113,197,200,129,31,57,214,50,86,139,51,207,214,10,44,101,5,218,238,19,198,85,35,189,167,50,76,78,107,51,76,110,46,182,124,45,235,191,237,
217,179,231,67,186,191,62,112,175,79,196,56,80,121,42,254,165,242,84,252,190,2,175,252,71,0,194,205,118,160,150,141,60,195,228,104,177,78,196,224,56,7,227,176,24,202,143,51,205,255,165,103,142,218,108,
14,141,27,91,129,81,85,0,247,202,92,71,109,142,200,171,79,156,89,229,34,134,63,219,171,156,243,210,177,99,199,126,121,174,231,157,77,253,148,143,8,102,51,193,108,106,30,121,228,145,159,40,95,184,251,79,
104,80,184,202,247,23,143,89,250,93,44,214,213,60,245,137,51,171,92,196,153,15,46,14,172,235,97,5,172,192,116,5,216,24,167,71,38,153,152,163,126,134,201,169,141,56,243,193,53,93,241,99,139,242,5,186,159,
124,244,209,71,127,115,114,167,253,161,43,79,165,253,45,121,213,85,95,249,202,87,14,189,250,213,175,126,125,249,190,226,215,204,166,169,81,116,238,89,125,226,154,69,13,99,196,153,175,28,215,161,69,172,
237,98,174,173,21,88,44,10,180,189,231,187,222,51,212,131,243,169,31,49,231,204,172,114,58,23,241,76,236,249,243,231,255,244,19,159,248,196,207,150,245,231,254,191,0,60,196,12,108,239,31,77,76,236,237,
114,249,33,143,31,186,241,198,27,191,88,252,27,240,68,220,101,64,88,62,5,51,159,28,124,226,54,203,92,88,254,135,0,53,234,3,147,27,11,200,11,107,132,154,6,107,181,211,18,77,88,129,69,164,64,151,247,125,
204,169,249,228,155,172,198,128,245,130,172,234,43,214,167,225,242,219,33,159,71,63,42,233,151,135,245,79,49,212,255,199,126,243,155,223,188,99,235,214,173,123,74,99,219,144,125,60,1,81,216,120,163,141,
49,250,180,108,150,180,228,97,49,148,207,252,177,164,36,143,252,76,108,92,107,38,181,206,181,2,195,84,0,205,107,174,163,54,71,228,213,39,166,197,30,128,233,171,141,60,125,90,212,162,241,210,146,159,176,
47,149,31,101,126,240,177,199,30,59,52,150,48,164,151,161,54,98,156,249,190,251,238,187,235,186,235,174,219,93,26,241,86,52,172,46,23,234,152,151,97,114,153,85,46,98,248,24,93,27,103,215,188,241,89,253,
106,5,22,175,2,104,106,93,70,150,167,92,134,201,101,86,185,136,225,55,93,165,57,31,125,225,133,23,30,222,191,127,255,223,116,217,251,32,115,134,222,136,113,184,242,100,124,231,181,215,94,187,167,124,102,
124,11,27,108,102,145,171,188,250,196,180,218,36,137,105,153,3,171,67,227,93,120,205,49,182,2,86,96,186,2,108,142,49,146,241,202,17,211,162,30,152,190,218,136,51,31,156,94,229,227,136,231,202,119,72,224,
73,248,203,113,111,195,240,71,162,17,227,224,187,118,237,218,182,121,243,230,15,173,90,181,234,17,52,68,189,16,87,159,13,147,28,227,93,172,230,0,99,112,190,113,111,242,181,198,79,102,52,163,185,214,55,
207,238,168,21,232,79,1,54,183,217,174,88,171,143,188,250,196,93,172,230,0,243,194,126,137,105,47,92,184,240,232,241,227,199,223,87,126,169,79,111,191,248,189,77,183,145,105,196,19,27,93,246,208,67,15,
189,103,211,166,77,191,85,62,170,184,30,28,155,45,109,228,232,119,177,154,19,49,124,140,46,205,179,75,206,248,108,126,181,2,75,75,1,52,187,182,145,229,40,151,97,114,77,22,49,141,71,191,124,20,241,194,
201,147,39,127,124,247,238,221,31,41,123,108,223,104,219,65,230,49,62,106,141,120,236,104,59,118,236,216,84,190,136,247,115,235,214,173,251,169,210,244,214,100,77,24,137,145,39,151,89,229,34,134,143,209,
212,96,155,98,227,213,126,181,2,86,64,21,96,83,84,142,56,139,41,151,97,114,153,85,46,226,226,159,43,191,77,237,215,203,23,229,126,241,208,161,67,39,185,135,81,178,35,217,136,41,208,235,94,247,186,13,215,
95,127,253,195,235,215,175,127,95,249,21,154,223,83,154,225,10,54,68,88,197,168,169,249,26,139,56,243,107,28,248,153,14,238,105,166,117,206,183,2,163,162,0,27,219,92,247,83,155,71,121,197,88,143,126,23,
171,57,5,95,42,223,27,252,199,167,79,159,254,80,249,130,220,167,158,124,242,201,151,230,186,255,65,214,143,116,35,214,131,151,47,232,109,198,47,150,47,191,167,98,91,185,110,46,215,109,197,191,181,52,186,
85,229,99,140,41,255,155,17,125,54,195,98,167,228,97,126,198,38,214,154,22,215,61,180,225,178,110,91,138,227,86,96,81,40,192,111,7,155,195,97,174,244,30,54,80,157,171,112,99,241,24,43,235,94,169,67,62,
252,146,115,161,124,241,237,217,242,183,49,159,193,223,199,196,31,253,44,254,23,203,23,226,142,235,156,198,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,
1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,
1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,
1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,
1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,
1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,
1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,
1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,176,128,21,88,182,80,246,126,239,189,247,110,95,187,118,237,27,214,172,89,115,243,202,149,43,111,91,190,124,249,157,197,222,94,246,191,122,217,178,101,175,
148,107,236,40,180,112,128,17,27,11,76,248,196,180,154,79,46,216,177,250,178,94,160,237,90,1,43,208,164,192,229,203,151,25,110,236,51,175,188,114,229,22,101,254,85,202,21,188,44,248,99,121,224,16,43,206,
249,139,23,47,62,93,214,251,114,177,207,156,59,119,238,240,217,179,103,63,191,127,255,254,167,174,76,56,226,160,81,160,97,239,125,231,206,157,55,95,125,245,213,239,90,183,110,221,63,47,77,247,245,19,141,
117,108,91,17,131,84,142,190,218,136,51,191,198,129,111,26,29,26,122,83,185,99,86,96,193,41,160,205,177,235,230,179,154,200,169,79,76,139,117,128,233,171,141,184,52,229,47,156,57,115,230,119,191,249,205,
111,126,244,224,193,131,135,187,238,113,24,121,35,217,136,209,128,183,110,221,250,107,171,87,175,126,47,155,43,27,93,230,67,56,242,196,20,83,235,34,71,63,214,116,225,53,199,216,10,88,129,186,2,108,144,
49,35,227,149,35,166,69,61,49,172,98,198,34,79,255,252,249,243,31,62,122,244,232,79,143,106,67,30,169,70,188,125,251,246,53,183,221,118,219,191,216,184,113,227,127,40,13,116,173,54,81,54,218,200,225,31,
160,137,139,113,248,28,172,171,249,228,213,198,26,141,25,91,1,43,48,174,0,155,100,77,143,24,175,249,202,19,195,42,198,26,145,163,175,121,5,159,61,117,234,212,191,124,230,153,103,254,203,83,79,61,117,174,
182,183,97,240,35,211,136,119,237,218,245,154,45,91,182,124,100,197,138,21,175,69,179,139,23,196,81,78,253,12,147,131,197,96,3,165,29,103,39,121,250,154,171,92,13,199,249,106,121,230,173,192,98,85,128,
205,174,203,249,178,220,200,209,167,197,188,196,106,155,112,140,193,231,117,233,210,165,47,189,248,226,139,239,217,183,111,223,223,119,217,115,31,57,35,209,136,239,191,255,254,55,92,123,237,181,143,150,
47,136,93,163,205,54,195,16,69,121,245,137,51,171,92,196,153,15,14,195,141,118,92,135,153,190,46,54,221,120,99,207,84,7,231,79,54,209,168,69,212,84,253,12,147,131,85,140,121,201,41,175,92,196,229,11,123,
39,142,29,59,246,200,222,189,123,63,31,247,53,12,127,232,141,184,52,225,7,74,19,254,243,242,36,92,253,40,66,27,47,49,196,106,194,140,171,109,194,136,97,180,53,144,182,248,248,44,126,181,2,75,87,1,54,195,
154,2,89,92,185,12,147,83,219,132,17,107,187,74,51,62,91,154,241,219,74,51,254,116,109,175,125,241,67,109,196,15,60,240,192,91,55,111,222,252,127,202,147,240,74,54,213,204,66,140,54,94,115,136,51,171,
28,48,70,214,92,51,110,60,187,254,58,155,154,250,108,142,88,129,209,85,128,77,112,38,59,204,106,34,71,159,22,243,19,171,109,194,136,197,56,57,90,124,107,221,4,190,120,226,196,137,183,239,217,179,231,47,
103,114,150,249,206,29,90,35,190,231,158,123,110,185,233,166,155,190,88,154,240,198,172,201,214,56,8,16,99,202,17,103,86,185,136,51,31,28,135,155,44,149,176,181,2,221,21,96,67,140,21,145,87,63,195,228,
212,70,12,191,198,49,22,45,26,114,185,78,29,57,114,228,174,199,31,127,252,185,184,207,190,252,161,52,226,29,59,118,172,218,182,109,219,238,85,171,86,237,138,77,181,171,15,129,52,87,125,226,204,42,7,140,
145,53,217,140,27,207,158,250,218,53,111,106,149,61,43,176,120,20,96,243,107,59,81,150,23,57,245,137,51,171,28,112,230,147,111,178,124,50,190,112,225,194,103,191,246,181,175,61,120,232,208,161,11,109,
231,24,68,124,229,32,38,109,155,243,186,235,174,251,215,115,105,194,109,13,152,205,49,90,236,139,92,196,153,15,14,67,107,198,25,191,90,1,43,64,5,154,238,15,54,72,228,106,30,121,114,234,71,140,28,112,180,
156,139,121,153,15,174,203,224,250,165,31,221,87,190,86,245,111,74,205,7,186,212,205,119,78,239,79,196,247,221,119,223,119,126,203,183,124,203,23,240,35,195,16,97,62,46,136,194,121,136,155,172,198,34,
134,143,193,127,160,113,175,249,117,38,185,205,51,57,106,5,22,150,2,218,12,219,118,158,229,42,151,97,114,153,85,14,120,54,23,159,136,89,251,252,243,207,191,161,252,104,244,23,218,206,50,223,241,222,159,
136,203,127,117,126,129,141,139,205,179,102,179,102,13,1,178,124,229,137,155,172,198,128,49,184,175,113,111,234,107,83,108,106,166,61,43,176,116,20,104,186,47,216,40,169,134,230,50,70,14,126,196,228,50,
139,57,145,207,121,184,134,242,136,241,66,195,205,6,215,100,108,226,169,248,123,233,247,101,123,125,34,46,223,170,246,218,242,177,196,147,56,60,175,172,217,118,137,65,32,230,209,146,163,5,79,60,6,202,
75,198,197,28,230,54,241,154,211,134,185,102,91,158,227,86,96,212,20,200,26,221,76,247,88,155,35,242,244,105,177,14,49,172,98,198,200,107,76,57,226,248,228,11,62,114,244,95,120,225,133,187,202,15,123,
60,57,211,115,206,37,191,215,39,226,242,163,203,239,103,211,164,197,230,137,103,106,99,173,250,196,153,85,46,98,248,24,216,75,219,232,146,211,54,135,227,86,96,148,21,232,250,30,103,35,204,206,194,57,98,
14,120,229,232,211,198,185,106,60,242,154,98,140,103,243,145,195,62,48,7,198,53,215,92,243,254,98,126,112,204,233,233,165,189,219,204,211,70,202,103,195,183,221,120,227,141,79,227,176,217,149,61,25,103,
92,172,197,246,148,163,79,139,24,241,24,40,47,228,34,159,249,172,161,213,90,114,182,86,192,10,76,42,160,205,117,146,157,68,49,174,126,134,193,145,87,27,121,250,153,229,211,46,99,77,62,98,229,59,40,238,
56,112,224,192,211,147,187,30,44,234,237,137,120,253,250,245,111,213,134,57,95,24,242,232,92,234,19,171,85,28,155,106,244,145,203,209,20,99,142,173,21,176,2,147,15,58,104,122,217,192,189,164,177,232,119,
169,201,114,154,184,120,255,170,143,189,208,39,222,176,97,195,91,203,124,191,219,52,231,124,198,122,107,196,175,122,213,171,222,165,27,199,193,245,66,76,125,10,19,185,232,199,58,245,137,51,171,28,48,6,
215,28,247,38,95,107,252,100,198,224,208,48,215,30,220,169,60,115,95,10,104,195,235,107,77,174,131,247,238,76,215,159,105,77,215,252,120,31,169,79,204,38,140,253,151,63,66,129,126,213,91,35,238,229,163,
137,242,215,53,94,85,126,138,238,68,57,48,254,154,198,148,134,27,63,126,104,243,99,61,68,83,78,125,226,204,42,23,49,124,14,204,61,223,99,16,115,206,247,30,61,223,210,83,96,166,77,179,171,66,181,121,149,
207,48,57,181,17,103,62,56,189,218,62,134,96,46,243,38,236,249,242,241,196,53,229,91,217,94,238,122,206,185,228,245,242,68,140,63,113,84,154,207,180,38,204,134,4,203,11,135,33,102,60,114,140,103,188,114,
196,153,85,14,24,67,215,27,103,114,142,177,185,218,108,189,185,206,233,122,43,48,27,5,208,140,22,234,251,177,105,239,49,166,126,134,201,21,187,186,252,89,182,29,69,203,199,102,163,231,76,107,250,106,196,
111,138,27,99,51,229,63,126,147,207,28,204,17,177,214,49,70,78,243,53,198,189,144,171,249,109,60,227,163,108,227,25,71,121,175,222,219,116,5,208,24,250,24,131,122,159,176,177,197,51,212,248,152,215,213,
143,243,81,55,61,151,230,100,88,57,212,149,191,16,132,190,181,120,26,113,249,184,225,38,28,76,47,10,76,14,62,49,108,244,25,171,241,177,70,243,136,97,49,152,219,132,199,18,229,69,107,132,54,180,2,3,85,
96,169,189,239,180,25,170,176,202,43,142,57,244,163,110,90,147,225,140,43,191,154,247,38,206,55,104,219,203,19,113,249,195,159,219,244,32,16,137,23,120,197,209,103,172,205,106,29,114,233,55,89,141,141,
21,148,23,214,210,87,219,20,211,60,99,43,176,212,21,64,99,195,200,238,25,109,122,200,137,190,114,89,76,227,192,109,67,231,200,48,57,88,12,236,25,184,244,173,197,213,136,203,127,89,110,193,225,120,169,
112,250,15,165,113,197,20,135,156,90,206,21,57,250,172,85,219,132,117,62,226,154,197,26,30,86,192,10,76,254,4,28,181,200,238,13,109,116,204,99,19,132,175,152,113,181,49,30,125,205,5,70,28,131,123,209,
124,226,104,145,207,186,242,127,242,55,195,239,99,244,242,68,92,14,116,107,60,76,108,148,20,43,227,25,139,115,192,103,62,173,114,196,153,85,46,98,248,28,77,107,51,167,102,231,82,91,155,211,188,21,24,134,
2,108,78,181,181,155,222,235,172,141,57,108,130,152,51,195,228,104,99,30,125,216,108,232,122,217,28,228,212,114,30,212,150,7,200,105,125,139,241,249,182,125,53,226,177,71,124,28,142,23,15,162,98,213,48,
114,89,87,179,58,31,231,105,179,156,151,181,153,223,53,166,121,198,86,96,177,41,192,123,169,235,185,208,220,56,98,45,99,228,217,8,145,79,92,179,204,129,101,61,112,109,232,90,17,163,190,182,206,4,63,229,
35,213,218,26,243,193,247,210,136,203,129,49,166,237,87,57,98,88,98,20,68,127,218,36,19,68,204,227,28,109,150,243,49,143,62,108,198,105,92,241,76,114,181,206,216,10,44,100,5,216,220,226,25,226,253,160,
121,140,145,83,31,120,162,9,166,22,235,104,126,92,55,250,154,203,185,57,71,211,58,200,45,215,244,166,21,23,152,39,191,151,70,28,247,58,113,200,43,116,118,222,152,131,100,114,209,94,153,104,34,135,62,231,
173,89,228,49,22,107,232,171,141,185,26,51,182,2,75,81,129,218,61,193,38,75,77,52,143,49,114,234,55,53,71,204,133,26,205,231,252,209,50,135,60,235,104,201,195,102,156,198,251,192,189,55,98,28,90,135,250,
196,180,204,131,31,57,198,96,99,92,125,214,69,203,186,56,143,250,89,78,140,183,249,92,183,45,207,113,43,48,170,10,196,166,214,101,159,250,190,143,245,140,145,135,31,49,57,181,88,55,230,213,246,18,215,
64,158,206,133,121,232,115,14,250,106,25,27,180,237,165,17,227,96,188,244,64,224,56,20,147,131,173,241,154,67,60,219,220,172,46,227,184,142,218,174,121,90,99,108,5,22,146,2,109,239,113,54,199,218,153,
88,31,243,192,147,203,176,114,58,119,141,215,28,226,153,228,178,102,24,118,249,48,22,109,90,19,194,113,40,86,14,60,47,242,180,90,67,28,45,115,107,150,249,89,28,49,189,178,28,115,86,96,41,41,160,247,67,
219,189,19,117,209,124,197,154,167,188,98,230,128,211,139,60,173,214,16,183,89,214,246,101,71,174,17,55,29,156,226,105,78,198,105,60,195,90,163,24,185,209,215,250,166,152,230,25,91,129,165,172,64,211,
125,210,20,155,141,102,217,124,25,55,155,185,251,172,25,90,35,86,177,20,207,199,225,231,123,62,236,105,16,115,206,199,89,61,135,21,24,69,5,6,113,191,196,57,163,63,138,58,116,221,211,208,26,113,219,6,163,
200,209,215,250,166,152,230,25,91,1,43,96,5,186,40,208,119,79,25,217,70,204,15,241,41,90,244,201,195,198,88,244,53,215,216,10,88,129,197,161,192,32,239,243,65,206,157,169,63,180,70,172,7,85,156,109,146,
28,243,104,193,43,102,94,155,213,26,197,77,243,33,143,87,219,252,142,91,129,165,172,64,211,125,210,116,191,197,88,212,48,139,119,229,56,87,150,207,216,48,109,47,141,24,135,207,4,104,227,154,226,93,230,
100,125,180,81,112,198,201,71,159,60,45,226,188,200,217,90,129,165,170,0,239,5,218,154,14,136,119,25,49,79,125,197,156,11,156,94,228,105,107,53,136,51,22,45,107,251,178,189,124,31,49,15,131,195,214,62,
123,209,88,13,115,158,154,157,73,157,230,98,190,204,7,95,219,47,98,24,168,107,27,109,115,180,213,59,110,5,134,165,64,151,247,119,219,222,178,57,34,23,125,204,169,92,13,207,100,109,157,163,173,174,239,
120,175,141,24,135,131,24,218,152,212,87,172,66,80,64,173,107,138,235,60,196,209,182,237,133,243,115,109,248,181,245,153,91,179,58,71,45,199,188,21,88,76,10,52,189,231,99,76,125,226,104,163,54,140,71,
158,62,226,181,156,54,158,113,90,206,57,72,219,123,35,198,97,120,64,54,54,248,196,60,108,204,209,58,230,208,106,173,206,69,92,179,58,39,231,224,186,136,145,227,58,26,35,71,27,115,201,219,90,129,197,172,
64,211,61,161,231,206,242,34,71,191,201,50,134,185,129,107,151,174,205,92,181,138,117,206,88,215,151,223,75,35,46,7,197,255,191,151,94,53,249,83,115,20,130,28,82,128,41,138,242,200,165,15,156,141,172,
142,243,213,44,230,225,188,177,30,49,114,192,24,204,29,247,166,190,198,220,169,81,123,86,96,105,41,208,116,63,196,152,250,196,53,11,21,17,99,188,77,85,205,213,26,226,22,123,169,109,254,249,138,247,213,
136,15,151,38,118,11,15,141,205,179,169,129,107,195,200,215,90,248,113,232,28,136,193,231,220,153,213,28,98,216,184,14,231,205,98,224,178,161,53,89,220,156,21,88,168,10,196,251,163,235,57,178,58,229,50,
76,78,45,49,214,5,110,187,184,191,88,199,250,38,123,233,210,165,35,172,31,180,237,165,17,151,3,61,83,254,74,199,45,122,24,10,195,134,137,88,13,51,166,245,138,57,23,243,232,115,190,104,107,121,58,39,106,
48,56,151,198,136,153,67,159,182,169,134,57,182,86,96,49,41,208,245,61,31,243,212,207,48,57,216,136,233,215,116,100,13,243,232,35,95,57,245,117,174,203,151,47,63,171,254,32,113,95,141,248,171,229,15,241,
93,57,135,54,48,8,66,63,98,20,104,236,202,4,1,32,135,194,178,70,253,144,222,169,225,103,245,220,11,231,203,114,24,179,181,2,75,89,129,166,123,35,198,212,39,110,178,136,105,156,126,180,212,159,60,252,136,
201,105,46,185,210,136,191,74,126,208,118,178,59,14,112,165,139,23,47,30,89,179,102,205,149,21,32,6,135,54,81,197,136,71,159,53,153,173,229,146,167,205,106,201,53,229,52,197,88,111,107,5,172,192,84,5,
244,94,215,136,242,109,24,113,230,100,152,92,205,98,93,198,50,92,227,208,183,16,235,99,244,210,136,203,71,19,135,33,132,14,52,54,12,242,177,209,69,159,181,53,30,113,141,41,102,109,215,28,205,143,53,49,
182,144,124,104,226,49,250,10,240,158,24,253,157,182,239,176,118,150,200,171,79,156,89,229,128,245,210,221,40,31,107,144,199,56,49,173,230,150,190,181,184,158,136,207,157,59,119,240,234,171,175,198,89,
175,12,30,24,68,108,154,209,103,14,44,234,216,80,116,14,196,6,49,178,189,204,199,58,60,195,124,204,213,117,142,62,244,234,186,23,231,141,166,2,131,120,143,212,230,84,62,195,228,212,70,76,31,106,2,243,
162,186,244,153,71,95,243,137,105,153,123,254,252,249,3,156,103,208,182,151,39,226,179,103,207,126,190,28,238,124,57,204,234,120,160,216,232,50,31,53,20,71,235,99,174,198,70,29,103,231,25,196,158,135,
209,240,7,113,142,165,58,103,95,239,147,65,234,155,157,33,114,234,19,103,86,57,224,204,39,159,197,112,78,198,51,44,220,249,242,0,121,8,126,31,163,151,70,188,127,255,254,151,223,253,238,119,255,249,234,
213,171,223,205,67,177,65,80,44,240,89,99,213,56,107,251,180,88,159,123,237,115,221,249,90,107,216,250,205,215,57,60,207,194,84,160,246,254,139,188,250,196,153,85,14,56,243,201,215,44,148,100,76,177,114,
229,105,248,19,232,91,125,169,222,75,35,198,97,94,126,249,229,143,106,35,198,161,117,100,77,56,227,180,102,54,24,235,178,177,42,198,92,209,231,252,53,158,113,91,43,96,5,166,43,128,251,166,54,98,76,125,
226,204,42,7,156,249,228,105,177,7,226,38,171,121,229,255,226,63,90,219,251,32,248,222,26,241,233,211,167,63,185,113,227,198,234,25,32,80,28,25,23,115,106,62,106,217,112,107,57,25,95,171,227,94,102,51,
103,182,142,57,43,176,88,21,224,189,82,59,95,140,171,159,97,112,228,213,70,158,126,23,139,189,49,47,195,47,189,244,210,39,107,251,31,4,191,98,16,147,102,115,62,247,220,115,39,239,188,243,206,237,229,251,
137,239,142,241,90,115,171,241,168,135,136,113,52,113,26,171,97,206,167,113,114,180,136,233,69,222,214,10,44,85,5,244,126,232,114,239,168,78,154,159,97,114,106,35,206,124,112,179,189,202,199,18,255,125,
247,238,221,191,175,251,28,52,238,237,137,24,7,57,117,234,212,47,149,239,39,254,193,120,40,10,25,249,242,13,213,87,149,159,200,139,244,152,192,177,73,99,14,114,138,81,76,159,86,185,136,225,99,32,23,131,
115,142,57,201,11,243,146,80,43,213,54,119,235,4,78,176,2,243,172,192,92,222,207,181,173,212,230,84,190,13,35,206,156,12,199,24,115,186,90,236,157,185,39,78,156,248,165,218,89,6,197,247,254,141,165,239,
120,199,59,254,119,105,198,239,65,19,194,133,70,75,156,89,141,67,132,44,39,242,244,51,171,92,196,153,15,142,3,107,123,88,1,43,208,174,0,27,99,150,25,99,234,103,152,156,218,136,225,243,194,154,196,120,
152,35,142,54,139,149,239,148,248,200,199,63,254,241,239,205,246,61,72,174,183,143,38,120,136,235,174,187,238,255,173,95,191,254,71,217,80,201,207,151,133,216,24,53,171,177,136,233,179,22,190,142,248,
15,73,95,115,140,173,192,82,82,128,247,64,180,153,6,204,209,24,56,142,12,147,203,172,114,192,51,189,176,110,172,249,198,55,190,241,3,135,15,31,126,158,123,234,203,14,229,17,239,109,111,123,219,191,93,
183,110,221,7,216,140,103,107,33,146,214,210,167,69,140,120,12,148,23,114,145,207,124,214,180,197,52,207,216,10,88,129,73,5,216,44,39,153,113,20,121,245,51,76,14,54,226,204,103,94,155,213,167,226,242,
5,186,127,255,201,79,126,242,3,113,175,125,248,189,63,17,227,80,155,54,109,218,87,158,138,31,46,31,59,220,58,155,67,82,120,173,37,71,139,24,49,172,98,214,41,207,252,200,197,92,198,213,50,199,214,10,44,
69,5,244,94,136,88,245,208,24,121,114,234,3,43,15,220,198,197,28,214,55,89,109,194,23,46,92,248,108,121,26,254,161,175,125,237,107,151,199,22,235,249,101,40,79,196,56,227,61,247,220,115,203,77,55,221,
244,197,210,140,55,234,83,109,27,70,109,204,33,71,203,167,222,104,25,135,197,96,124,220,155,238,147,207,114,53,102,108,5,172,64,174,0,27,100,140,70,94,253,12,147,83,27,49,252,26,199,88,180,104,198,229,
58,117,228,200,145,187,30,127,252,241,231,226,62,251,242,135,242,68,140,195,149,131,159,186,225,134,27,254,170,124,225,238,159,150,134,56,253,91,35,68,1,138,43,212,24,84,62,195,228,96,219,48,38,100,30,
173,174,71,174,201,106,190,177,21,88,236,10,52,221,11,140,169,6,228,96,57,200,169,31,49,243,213,54,97,157,147,56,90,121,26,190,120,242,228,201,119,127,246,179,159,253,107,174,59,12,59,180,39,98,30,246,
254,251,239,127,224,218,107,175,253,243,21,43,86,172,197,19,42,159,82,137,107,22,245,140,101,152,156,218,38,140,24,6,215,31,247,166,190,54,197,166,102,218,179,2,75,91,1,54,202,76,133,44,166,92,134,201,
169,109,194,136,181,93,165,25,159,61,118,236,216,219,246,238,221,251,233,108,159,125,114,67,111,196,56,108,105,198,111,40,205,248,209,242,49,197,53,108,174,53,139,124,141,209,167,101,179,140,150,113,88,
12,198,199,189,233,126,27,207,184,173,21,176,2,205,10,176,97,198,172,200,171,159,97,114,176,138,49,47,57,229,149,139,184,52,225,19,165,9,63,82,154,240,231,227,190,134,225,143,68,35,198,193,119,237,218,
245,154,210,140,255,168,252,228,221,183,107,163,37,70,14,49,155,40,125,141,17,171,237,138,145,199,193,53,232,215,108,215,188,90,189,121,43,176,208,21,96,243,235,114,142,44,55,114,244,105,49,47,177,218,
38,28,99,240,121,149,223,51,252,165,23,95,124,241,61,251,246,237,251,251,46,123,238,35,103,100,26,49,14,187,125,251,246,53,119,220,113,199,143,110,216,176,225,151,75,131,27,251,168,2,60,27,46,173,114,
196,180,108,140,180,228,213,2,99,104,78,230,143,37,201,75,204,151,144,161,21,176,2,162,0,27,161,80,83,96,140,215,124,229,137,213,54,97,196,66,252,108,249,22,181,127,245,149,175,124,229,183,159,122,234,
169,115,83,54,52,100,103,164,26,49,181,216,185,115,231,205,91,183,110,253,181,242,219,218,222,139,230,199,11,113,98,54,69,181,138,57,87,19,199,28,206,171,62,49,235,233,47,52,187,208,247,191,208,244,158,
239,253,178,145,204,247,188,125,206,215,116,134,24,83,63,195,228,96,21,227,60,145,163,15,91,126,127,196,135,143,30,61,250,211,7,15,30,60,220,231,217,187,174,53,146,141,152,155,71,67,46,223,115,252,238,
181,107,215,254,112,249,200,226,245,104,42,108,44,77,152,245,154,27,185,154,15,158,117,204,233,98,103,83,211,101,94,231,88,129,81,86,128,205,176,235,30,179,252,200,169,79,76,139,117,136,97,21,51,70,190,
252,205,185,47,148,95,103,249,123,229,187,34,254,116,84,27,48,117,27,233,70,204,77,194,222,123,239,189,219,203,79,227,237,88,181,106,213,77,165,41,223,86,190,176,119,103,177,183,151,208,234,210,4,95,97,
35,164,69,13,48,98,192,24,26,27,103,114,142,177,9,59,86,95,214,11,180,93,43,96,5,218,20,192,183,137,77,140,106,175,97,51,101,34,172,114,5,47,11,254,88,42,56,196,138,115,190,52,221,167,203,90,95,46,246,
153,242,195,25,95,61,115,230,204,161,242,139,221,159,26,75,244,139,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,
10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,
10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,
10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,
10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,
10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,
10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,
10,44,124,5,150,45,148,35,188,249,205,111,222,188,98,197,138,187,86,174,92,185,173,92,55,151,235,182,226,223,186,108,217,178,85,203,151,47,127,69,207,17,253,146,115,85,185,166,228,48,31,49,25,105,142,
196,27,97,89,183,49,238,160,21,88,44,10,92,190,124,121,174,71,185,114,227,189,242,202,244,219,174,112,99,241,24,43,235,94,169,195,6,224,151,156,11,151,46,93,122,246,226,197,139,207,148,235,112,185,142,
20,255,139,143,61,246,216,241,185,110,178,175,250,41,135,234,107,209,174,235,188,238,117,175,219,112,253,245,215,63,188,126,253,250,247,173,94,189,250,123,74,211,92,193,198,9,171,24,115,182,249,154,195,
61,176,134,62,109,141,103,188,171,157,175,121,186,174,231,60,43,48,8,5,98,67,156,205,26,181,57,34,175,62,113,23,171,57,5,95,58,127,254,252,31,159,62,125,250,67,47,188,240,194,167,158,124,242,201,151,102,
179,231,190,106,70,178,17,239,216,177,99,211,214,173,91,127,110,221,186,117,63,85,26,217,26,52,51,54,180,38,12,209,52,79,253,38,28,99,240,117,112,78,229,140,173,128,21,104,87,128,205,49,203,140,49,245,
51,76,46,179,202,69,92,252,115,103,206,156,249,245,163,71,143,254,226,161,67,135,78,102,123,25,54,55,106,141,120,217,67,15,61,244,158,77,155,54,253,86,249,223,252,235,33,14,27,47,109,228,232,119,177,154,
19,49,124,140,174,77,183,107,222,248,172,126,181,2,139,95,1,54,192,182,147,102,121,202,101,152,92,147,69,76,227,244,201,149,143,49,94,56,121,242,228,143,239,222,189,251,35,101,143,211,63,15,105,219,248,
0,227,35,211,136,119,237,218,181,109,243,230,205,31,90,181,106,213,35,108,186,180,56,63,49,173,114,196,93,172,230,0,99,96,206,108,212,248,44,55,227,230,90,159,205,105,206,10,12,75,1,54,180,217,172,95,
171,141,188,250,196,93,172,230,0,243,194,94,137,105,47,92,184,240,232,241,227,199,223,183,111,223,190,35,179,57,203,32,106,242,14,52,136,149,26,230,44,95,136,187,243,218,107,175,221,83,190,248,118,11,
27,109,102,49,133,242,234,103,152,28,44,134,54,70,197,49,54,150,44,47,49,87,66,134,86,192,10,52,40,128,230,87,27,49,166,62,49,45,230,0,166,175,54,226,204,7,167,87,249,98,222,115,199,142,29,123,176,124,
65,239,203,181,253,245,201,15,189,17,223,119,223,125,119,93,119,221,117,187,203,71,17,91,181,201,54,97,8,164,113,250,180,218,56,137,163,69,46,6,249,113,111,252,53,227,52,78,220,53,143,249,182,86,96,49,
43,192,6,216,116,198,44,71,185,12,71,142,190,90,224,204,39,159,217,242,81,197,209,242,133,188,135,247,239,223,255,55,77,123,238,35,54,212,70,92,158,132,119,148,47,202,237,41,77,120,3,154,90,118,65,132,
140,103,19,100,140,121,180,26,7,135,65,46,226,204,175,113,224,187,14,93,175,107,141,243,172,192,168,42,192,70,55,155,253,213,106,149,111,195,136,51,71,109,196,204,163,197,126,137,97,75,3,86,255,165,242,
69,60,60,25,31,154,205,185,230,171,102,104,141,248,141,111,124,227,150,27,111,188,241,139,165,9,223,80,174,106,179,101,163,101,83,83,63,114,16,37,227,34,175,226,49,159,92,244,201,211,182,197,153,103,107,
5,150,170,2,104,118,77,35,198,107,190,242,192,244,213,70,204,188,38,30,49,109,198,229,99,138,231,203,184,235,115,159,251,220,139,77,251,30,100,108,229,32,39,111,152,123,121,249,56,226,247,203,103,194,
55,160,177,205,244,194,188,90,163,126,134,193,97,104,19,173,225,241,204,241,87,205,81,190,134,103,154,95,155,199,188,21,24,117,5,216,232,178,125,102,247,129,230,107,28,60,125,230,192,39,79,14,235,100,
60,57,238,131,62,234,180,150,113,90,60,252,161,25,99,160,15,161,31,21,248,143,203,53,231,159,84,193,156,51,29,67,121,34,126,228,145,71,126,124,227,198,141,191,9,209,186,62,13,35,87,47,28,180,230,51,214,
100,17,195,192,28,58,162,175,49,224,182,120,204,183,111,5,150,154,2,77,13,16,90,196,120,205,39,159,89,229,128,51,159,124,205,234,83,49,114,78,157,58,245,147,143,62,250,232,111,14,227,223,107,106,23,234,
97,7,59,119,238,252,214,109,219,182,253,109,105,104,99,63,37,55,219,70,140,134,200,166,216,132,113,36,205,227,17,201,213,124,173,99,206,76,109,92,99,166,245,206,183,2,163,164,0,155,221,108,246,148,213,
70,78,125,226,204,42,23,49,252,174,87,108,196,165,238,210,145,35,71,190,253,224,193,131,255,48,155,51,206,165,166,247,143,38,182,108,217,242,254,210,160,198,154,48,27,21,236,92,46,8,192,250,12,147,131,
197,224,186,17,143,5,67,156,156,90,173,87,222,216,10,44,102,5,186,188,239,217,24,163,14,172,213,120,228,224,51,78,28,45,230,37,23,113,92,179,205,199,60,58,202,218,43,202,183,209,190,191,112,63,164,124,
31,120,234,78,6,188,226,155,222,244,166,219,203,211,240,151,139,0,24,87,154,103,211,83,113,140,97,139,90,75,172,60,113,147,213,24,48,6,230,170,141,166,88,173,198,188,21,88,202,10,176,169,102,26,196,152,
250,196,77,86,99,192,122,97,61,126,254,155,60,245,94,201,141,49,248,229,122,165,124,225,238,206,3,7,14,60,157,237,123,80,92,175,79,196,229,105,248,103,74,67,195,152,210,76,113,184,200,53,249,181,124,229,
137,51,171,28,48,6,214,139,35,227,98,142,250,51,205,215,90,99,43,176,208,20,96,51,172,237,91,239,135,152,139,152,114,234,19,215,44,214,99,76,215,38,135,121,241,0,71,139,6,155,13,228,115,32,23,126,169,
91,118,205,53,215,252,108,225,127,146,177,62,236,228,78,6,188,218,61,247,220,115,195,173,183,222,122,184,28,246,202,199,18,56,56,175,248,228,219,198,35,142,193,188,232,51,214,100,17,195,96,237,184,55,
221,39,175,54,214,104,204,216,10,44,69,5,180,177,214,206,31,115,106,62,249,204,42,7,156,249,228,105,179,167,95,198,96,53,94,240,165,103,159,125,246,150,242,11,130,190,86,59,199,124,243,189,61,17,151,255,
202,252,163,178,249,21,56,128,54,79,226,26,207,120,102,179,26,114,180,168,35,30,3,229,133,92,228,51,159,53,180,90,75,206,214,10,88,129,201,251,10,141,173,54,112,255,104,60,250,177,46,139,103,92,172,83,
31,235,161,70,135,250,26,159,192,43,202,119,117,189,181,228,127,72,107,6,137,123,107,196,107,215,174,125,39,14,175,2,208,159,139,133,56,90,175,62,177,90,197,186,23,240,24,25,215,196,143,21,249,197,10,
88,129,43,10,240,30,210,134,123,37,216,17,96,14,173,143,126,199,105,198,210,184,31,214,36,141,119,44,164,124,249,21,188,239,40,228,226,106,196,229,23,188,151,223,235,190,250,93,16,68,47,156,190,201,167,
128,154,19,113,211,28,140,101,86,57,96,12,174,55,238,77,190,214,248,201,12,35,43,96,5,162,2,184,111,180,153,50,30,121,245,21,51,127,46,86,155,43,231,193,26,28,196,154,7,92,250,213,59,209,183,202,47,148,
63,207,220,65,218,94,158,136,203,79,173,220,93,62,3,94,135,67,199,11,135,83,46,250,20,74,115,136,219,114,25,207,172,114,192,24,92,107,220,155,124,173,241,147,25,70,86,192,10,140,154,2,218,92,185,55,229,
20,243,30,39,87,252,117,229,247,224,124,71,169,123,156,181,131,180,189,52,226,242,177,196,125,241,16,56,56,47,196,136,97,163,79,110,44,32,47,90,67,172,181,196,153,85,14,24,163,105,157,241,12,191,90,1,
43,48,72,5,216,8,155,214,200,114,192,197,17,239,103,173,203,176,114,168,45,125,107,87,153,115,241,52,98,252,177,79,54,74,90,138,166,62,49,44,134,250,93,176,214,100,24,28,6,231,143,56,243,193,97,104,205,
56,227,87,43,96,5,154,20,208,198,166,121,145,143,190,230,18,119,201,97,110,102,181,62,195,228,96,49,112,191,151,255,139,191,57,155,107,16,92,47,79,196,229,151,106,220,20,55,207,198,10,94,113,244,25,107,
179,90,135,92,250,77,86,99,99,5,19,47,172,87,142,184,41,198,28,91,43,176,148,21,208,102,22,117,96,195,139,124,244,153,71,91,139,71,62,243,117,142,12,147,163,197,28,192,248,139,241,217,124,131,224,250,
106,196,141,79,196,60,24,155,45,124,197,234,147,167,141,181,224,53,159,88,109,19,70,12,131,243,140,123,249,107,151,156,188,210,172,21,88,92,10,160,113,113,212,238,11,109,116,204,85,78,49,227,106,99,28,
62,70,211,122,26,215,122,226,204,142,77,90,94,22,221,19,113,17,234,86,30,142,22,226,81,192,26,70,174,198,88,171,150,113,218,88,3,158,156,218,136,51,191,198,129,159,203,224,158,230,50,135,107,173,192,32,
21,96,147,235,186,70,211,123,154,115,105,78,228,216,16,177,30,113,180,26,211,125,113,46,229,128,227,122,244,227,188,53,191,52,226,105,125,43,174,49,95,126,111,79,196,20,6,98,80,16,114,60,76,228,51,159,
245,209,234,28,172,107,179,113,253,204,231,188,109,49,205,51,182,2,11,93,1,222,59,179,61,135,54,199,56,23,27,31,231,86,159,184,102,81,131,24,70,156,119,140,12,47,154,27,49,234,107,235,128,47,141,120,218,
71,170,97,250,121,115,123,105,196,229,192,43,51,209,148,107,194,26,171,157,28,57,154,71,92,179,152,135,177,136,185,134,198,201,181,217,217,212,180,205,233,184,21,24,37,5,216,208,154,246,20,239,3,173,97,
140,28,252,136,201,69,139,53,99,125,219,62,226,220,156,3,124,156,95,253,130,123,233,143,216,79,111,11,169,88,56,44,46,142,12,199,28,228,146,139,150,243,48,135,62,231,173,217,152,31,235,232,171,229,92,
202,25,91,129,165,164,64,219,61,192,198,167,154,176,70,99,202,69,156,53,73,204,135,188,108,14,93,11,56,230,208,143,245,177,110,88,126,239,141,152,130,243,192,234,43,102,28,22,124,45,150,197,53,151,56,
90,214,193,114,48,135,62,108,198,105,124,46,120,144,115,207,101,95,174,93,154,10,176,89,205,245,244,250,190,142,115,50,166,60,56,250,196,209,98,79,228,136,97,89,7,172,3,185,24,140,199,90,240,228,154,172,
206,57,72,220,107,35,166,56,217,129,52,86,195,89,157,114,51,169,211,92,204,209,230,235,58,138,99,157,198,140,173,192,66,82,96,182,239,101,54,187,236,172,58,167,230,129,111,242,227,92,154,175,56,230,69,
95,115,21,199,188,97,251,203,251,216,0,4,192,21,71,27,87,139,115,190,90,156,235,48,30,45,227,176,156,43,114,234,43,102,62,173,198,140,173,192,82,84,128,247,2,109,77,3,196,117,68,159,49,242,209,34,78,142,
185,228,192,243,210,24,227,228,88,223,102,153,223,151,237,165,17,103,135,161,16,136,41,206,114,201,101,121,25,199,252,154,109,170,169,197,192,215,98,181,117,204,91,129,165,168,64,211,125,18,99,234,43,
238,170,91,86,147,113,93,231,27,86,222,208,26,241,32,15,188,16,255,33,6,169,135,231,182,2,125,43,48,136,123,48,206,25,253,190,207,56,159,235,45,202,70,60,159,2,121,46,43,96,5,172,192,160,21,88,148,141,
88,191,8,48,104,1,61,191,21,176,2,211,21,24,196,61,24,231,140,254,244,93,44,28,102,104,141,88,69,84,220,36,93,150,151,113,77,115,32,214,84,83,139,129,175,197,218,214,115,220,10,44,37,5,154,238,147,24,
83,95,113,87,189,178,154,140,235,58,223,176,242,134,214,136,107,7,86,17,21,51,31,156,94,228,105,181,70,49,227,209,102,57,25,199,58,196,120,145,179,181,2,75,93,1,222,19,109,247,206,108,116,210,57,21,115,
46,93,187,22,215,92,98,181,172,163,213,88,31,184,151,239,35,214,195,233,7,236,224,233,43,214,131,215,120,205,33,174,229,146,167,69,126,13,235,92,192,220,31,121,181,152,163,105,52,213,54,213,57,102,5,70,
77,129,182,247,122,211,126,179,218,200,169,79,76,139,185,107,184,105,221,88,167,185,156,143,86,99,195,192,189,52,98,61,24,14,174,13,42,250,200,141,92,244,117,62,226,152,67,191,102,227,58,200,195,208,189,
49,39,227,193,181,13,206,217,150,231,184,21,88,108,10,52,189,247,99,76,125,226,104,163,62,140,71,94,253,152,67,159,86,115,21,51,78,171,177,65,225,222,27,49,14,130,3,106,195,163,79,91,203,105,18,129,243,
81,60,248,156,175,102,185,14,108,86,175,235,113,94,229,136,89,75,223,214,10,44,21,5,154,238,11,213,32,203,139,28,253,38,203,24,230,6,110,186,186,174,207,57,105,181,174,47,220,75,35,46,7,124,165,52,171,
41,63,86,195,67,147,134,15,172,188,226,54,65,98,174,206,199,121,163,197,156,186,126,230,115,93,230,209,87,203,181,149,51,182,2,75,89,129,166,123,34,198,212,39,174,89,104,138,24,227,109,26,107,110,196,
156,171,102,75,126,243,103,143,109,139,207,32,222,75,35,190,124,249,242,243,165,145,221,200,125,105,83,195,89,233,55,97,214,214,172,206,129,28,248,156,47,179,154,67,12,139,92,14,206,25,121,198,51,171,
53,89,220,156,21,88,200,10,232,253,49,147,115,100,117,202,101,152,156,90,98,172,13,220,118,113,143,177,142,245,45,246,121,214,15,218,246,213,136,159,41,127,183,238,74,35,166,40,108,90,234,103,24,34,48,
55,19,132,53,204,163,143,26,224,104,107,121,156,155,107,113,30,242,106,153,163,28,112,83,77,204,181,111,5,22,147,2,93,222,251,49,71,253,12,147,131,141,152,126,77,67,214,48,143,62,242,149,171,213,95,186,
116,233,217,90,108,190,249,94,26,113,57,208,225,242,135,248,174,236,157,77,140,98,168,175,24,5,209,191,50,137,128,152,3,95,231,6,206,56,206,207,220,204,231,50,92,131,190,214,144,179,181,2,86,96,92,129,
166,251,35,198,212,39,110,178,140,97,37,224,218,197,127,11,198,213,103,45,45,231,84,91,254,79,254,48,107,6,109,39,187,227,0,87,42,141,248,171,58,61,15,11,174,169,65,34,174,185,240,187,14,157,87,107,148,
87,140,156,232,179,174,198,51,190,16,45,206,228,49,58,10,204,246,125,62,58,39,200,119,82,59,87,228,213,39,206,44,56,229,233,55,89,236,76,227,244,163,213,121,17,187,120,241,226,17,216,62,70,95,141,248,
8,15,201,67,177,17,144,215,102,167,56,230,211,135,101,45,185,172,142,177,217,218,185,206,137,250,81,28,81,187,81,220,163,247,52,26,10,204,245,189,146,213,71,78,125,226,204,42,7,204,43,83,138,49,214,32,
71,57,242,106,21,151,7,200,197,213,136,207,159,63,255,120,20,138,7,6,175,205,78,49,107,50,46,214,181,229,50,94,179,217,26,25,87,171,39,143,26,29,122,78,229,141,173,192,66,84,96,182,239,231,172,78,185,
12,147,203,44,56,94,208,145,88,109,198,147,163,69,62,49,45,185,11,23,46,76,235,91,99,201,3,120,233,229,137,248,236,217,179,127,85,14,119,169,236,127,133,158,129,77,139,7,71,76,49,252,182,102,216,22,199,
28,125,14,236,159,231,234,115,93,175,101,5,250,80,32,222,159,93,215,204,234,148,107,194,140,169,5,206,124,242,180,216,31,113,180,26,35,166,45,185,151,202,3,228,231,224,247,49,166,52,198,65,45,248,220,
115,207,157,223,190,125,251,155,203,119,78,188,154,107,100,205,42,114,209,103,109,95,22,255,112,179,25,250,15,62,155,122,215,88,129,81,81,64,223,203,115,185,31,226,121,226,92,234,19,55,89,196,52,78,63,
179,88,91,121,245,51,12,174,52,225,79,61,250,232,163,191,7,220,199,232,229,137,24,7,57,119,238,220,159,174,94,189,250,173,60,20,69,164,143,166,27,185,232,51,183,201,162,102,54,13,188,86,87,227,155,246,
160,177,217,156,65,235,141,173,192,66,86,160,246,254,143,188,250,196,153,85,46,98,248,243,117,149,255,139,255,147,62,117,239,173,17,191,244,210,75,159,220,176,97,67,122,182,172,9,103,92,86,12,225,103,
210,120,53,95,49,230,142,62,215,171,241,140,219,90,1,43,48,93,1,220,55,217,136,188,250,25,38,7,27,113,230,51,79,45,246,81,243,53,70,124,250,244,233,191,4,238,107,244,242,209,4,14,83,62,158,120,177,124,
60,177,163,124,60,241,154,120,184,172,145,102,92,172,107,242,245,31,8,121,244,99,77,228,163,207,124,240,188,200,217,90,1,43,48,85,1,222,35,77,247,145,86,196,60,250,93,44,114,52,143,254,108,44,246,196,
186,242,69,186,143,125,234,83,159,250,13,221,231,160,113,111,79,196,56,200,169,83,167,62,184,101,203,150,119,233,161,106,79,190,20,88,115,21,35,142,90,109,216,228,52,47,195,109,121,92,91,231,214,121,24,
87,174,150,171,57,198,86,96,49,41,144,221,7,181,243,213,114,149,207,48,57,181,17,103,62,184,182,11,123,205,114,78,158,60,249,193,218,57,6,197,79,253,94,171,65,173,34,243,190,235,93,239,250,212,170,85,
171,30,66,227,90,190,124,249,149,102,10,63,94,181,56,166,139,185,202,17,55,89,141,1,99,96,206,218,104,138,213,106,204,91,129,165,172,0,27,100,166,65,140,169,79,220,100,53,6,204,11,107,17,171,45,63,37,
215,137,47,79,195,187,63,250,209,143,62,156,237,121,144,92,175,79,196,56,72,121,42,254,64,121,42,126,8,24,66,53,13,136,135,102,140,60,90,54,96,173,67,156,141,82,177,230,16,107,92,49,226,240,49,56,215,
152,51,241,194,24,185,44,135,49,91,43,176,20,21,136,247,72,166,65,150,163,28,49,45,230,32,134,109,194,89,156,92,87,139,254,148,237,123,208,92,253,17,112,128,43,191,253,237,111,255,173,87,189,234,85,63,
198,166,58,23,139,109,178,62,195,228,96,49,180,129,42,142,177,177,228,137,151,152,167,49,99,43,96,5,234,10,176,113,198,140,200,171,159,97,114,176,77,152,241,174,86,159,148,203,119,74,252,246,159,253,217,
159,253,88,220,107,31,126,111,95,172,211,195,172,91,183,110,207,198,141,27,191,167,60,229,94,167,252,76,48,255,49,88,163,62,113,180,200,37,23,49,125,141,131,195,0,23,175,241,136,95,173,128,21,160,2,241,
30,105,186,151,88,3,171,121,25,38,167,54,226,204,7,215,245,42,63,206,252,165,242,13,5,239,253,250,215,191,126,65,247,214,23,30,202,19,49,14,119,239,189,247,126,219,13,55,220,240,68,121,218,92,133,39,206,
174,23,106,99,174,114,196,153,85,46,226,204,7,199,129,53,61,172,128,21,232,174,0,155,99,86,17,99,234,103,152,156,218,136,225,215,56,198,162,157,120,34,190,240,252,243,207,127,199,129,3,7,254,54,219,107,
31,220,80,158,136,113,176,195,135,15,31,221,182,109,219,83,107,214,172,249,222,46,7,165,192,154,171,92,134,201,209,162,54,226,204,87,142,235,129,203,46,198,109,173,192,82,84,32,187,39,200,69,61,200,195,
114,144,83,63,98,230,103,86,235,137,105,49,15,113,147,45,223,37,241,190,199,30,123,108,55,215,29,134,29,250,99,222,131,15,62,248,125,215,92,115,205,255,40,31,83,148,135,206,169,79,198,16,36,114,244,53,
150,97,114,106,155,48,98,24,152,191,54,154,98,181,26,243,86,96,41,42,128,198,87,27,89,76,185,12,147,83,219,132,17,211,11,123,81,127,226,73,248,149,19,39,78,124,255,158,61,123,254,176,182,215,190,248,122,
215,233,107,7,101,157,183,188,229,45,223,117,237,181,215,126,172,52,186,214,143,41,176,45,54,99,54,70,250,140,53,89,141,1,99,112,158,113,111,242,181,198,79,102,180,163,249,152,163,125,21,103,88,129,193,
40,192,102,55,215,217,107,243,68,158,62,45,214,37,206,172,114,17,195,111,184,46,28,59,118,236,157,159,249,204,103,254,98,174,103,155,143,250,145,104,196,56,200,174,93,187,222,180,117,235,214,63,41,79,
198,215,179,177,70,139,60,229,212,39,110,178,26,139,24,62,70,151,198,217,37,103,124,54,191,90,129,165,165,0,155,97,211,169,99,78,147,207,88,147,213,24,112,230,147,135,45,79,195,47,28,61,122,244,187,247,
237,219,119,160,105,159,125,198,70,166,17,227,208,59,118,236,216,84,154,241,207,151,239,170,248,153,90,195,37,143,252,136,201,101,86,185,136,51,31,28,135,27,47,149,176,181,2,51,83,128,77,49,171,138,49,
245,51,76,46,179,202,1,103,62,184,51,103,206,252,106,105,194,31,60,116,232,208,201,108,79,195,226,70,170,17,83,132,251,239,191,255,238,205,155,55,255,78,249,59,119,59,193,177,225,210,42,71,220,197,106,
14,48,70,214,100,51,110,60,123,250,235,76,114,167,87,155,177,2,11,95,1,54,189,46,39,201,114,35,167,62,113,23,171,57,192,188,176,175,242,103,143,14,30,63,126,252,71,246,238,221,251,68,151,125,246,157,51,
146,141,120,66,132,229,165,33,127,231,250,245,235,223,187,118,237,218,31,46,191,44,104,43,155,30,172,98,228,103,28,121,181,192,24,172,31,247,166,251,228,179,92,141,25,91,1,43,80,87,128,205,49,203,136,
177,154,175,60,49,172,98,204,31,185,242,189,193,71,203,15,105,252,94,249,77,106,31,46,13,248,175,75,202,229,108,31,163,192,141,114,35,190,162,79,249,206,138,149,69,212,157,165,41,239,44,79,201,183,149,
207,145,239,44,118,123,105,206,119,150,134,186,26,137,108,172,209,106,172,9,35,198,43,148,205,16,0,0,1,66,73,68,65,84,193,57,232,215,108,215,188,90,189,121,43,176,88,20,96,83,236,114,158,44,55,114,244,
105,49,47,113,98,207,151,254,240,229,242,212,251,84,249,252,23,246,153,210,124,15,150,254,112,176,124,71,196,197,46,123,114,142,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,
172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,
172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,
172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,
172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,96,5,172,128,21,176,2,86,192,10,88,1,43,176,72,20,248,255,25,38,153,197,54,44,145,4,0,0,0,0,73,69,78,68,174,66,96,130,0,0};

const char* PlayerBackgroundComponent::menu_icon_png2 = (const char*) resource_PlayerBackgroundComponent_menu_icon_png2;
const int PlayerBackgroundComponent::menu_icon_png2Size = 17759;

// JUCER_RESOURCE: menu_activated_icon_png2, 20814, "../../SpatCommon/Resources/Menu_activated_icon.png"
static const unsigned char resource_PlayerBackgroundComponent_menu_activated_icon_png2[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,1,98,0,0,1,98,8,6,0,0,0,204,92,210,122,0,0,0,1,115,82,71,
66,0,174,206,28,233,0,0,0,9,112,72,89,115,0,0,46,35,0,0,46,35,1,120,165,63,118,0,0,3,170,105,84,88,116,88,77,76,58,99,111,109,46,97,100,111,98,101,46,120,109,112,0,0,0,0,0,60,120,58,120,109,112,109,101,
116,97,32,120,109,108,110,115,58,120,61,34,97,100,111,98,101,58,110,115,58,109,101,116,97,47,34,32,120,58,120,109,112,116,107,61,34,88,77,80,32,67,111,114,101,32,53,46,52,46,48,34,62,10,32,32,32,60,114,
100,102,58,82,68,70,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,47,50,50,45,114,100,102,45,115,121,110,116,97,120,
45,110,115,35,34,62,10,32,32,32,32,32,32,60,114,100,102,58,68,101,115,99,114,105,112,116,105,111,110,32,114,100,102,58,97,98,111,117,116,61,34,34,10,32,32,32,32,32,32,32,32,32,32,32,32,120,109,108,110,
115,58,120,109,112,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,34,10,32,32,32,32,32,32,32,32,32,32,32,32,120,109,108,110,115,58,116,105,102,102,
61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,116,105,102,102,47,49,46,48,47,34,10,32,32,32,32,32,32,32,32,32,32,32,32,120,109,108,110,115,58,101,120,105,102,61,34,104,116,
116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,101,120,105,102,47,49,46,48,47,34,62,10,32,32,32,32,32,32,32,32,32,60,120,109,112,58,77,111,100,105,102,121,68,97,116,101,62,50,48,49,55,
45,49,48,45,49,50,84,50,49,58,49,48,58,54,49,60,47,120,109,112,58,77,111,100,105,102,121,68,97,116,101,62,10,32,32,32,32,32,32,32,32,32,60,120,109,112,58,67,114,101,97,116,111,114,84,111,111,108,62,80,
105,120,101,108,109,97,116,111,114,32,51,46,55,60,47,120,109,112,58,67,114,101,97,116,111,114,84,111,111,108,62,10,32,32,32,32,32,32,32,32,32,60,116,105,102,102,58,79,114,105,101,110,116,97,116,105,111,
110,62,49,60,47,116,105,102,102,58,79,114,105,101,110,116,97,116,105,111,110,62,10,32,32,32,32,32,32,32,32,32,60,116,105,102,102,58,67,111,109,112,114,101,115,115,105,111,110,62,53,60,47,116,105,102,102,
58,67,111,109,112,114,101,115,115,105,111,110,62,10,32,32,32,32,32,32,32,32,32,60,116,105,102,102,58,82,101,115,111,108,117,116,105,111,110,85,110,105,116,62,50,60,47,116,105,102,102,58,82,101,115,111,
108,117,116,105,111,110,85,110,105,116,62,10,32,32,32,32,32,32,32,32,32,60,116,105,102,102,58,89,82,101,115,111,108,117,116,105,111,110,62,51,48,48,60,47,116,105,102,102,58,89,82,101,115,111,108,117,116,
105,111,110,62,10,32,32,32,32,32,32,32,32,32,60,116,105,102,102,58,88,82,101,115,111,108,117,116,105,111,110,62,51,48,48,60,47,116,105,102,102,58,88,82,101,115,111,108,117,116,105,111,110,62,10,32,32,
32,32,32,32,32,32,32,60,101,120,105,102,58,80,105,120,101,108,88,68,105,109,101,110,115,105,111,110,62,51,53,52,60,47,101,120,105,102,58,80,105,120,101,108,88,68,105,109,101,110,115,105,111,110,62,10,
32,32,32,32,32,32,32,32,32,60,101,120,105,102,58,67,111,108,111,114,83,112,97,99,101,62,49,60,47,101,120,105,102,58,67,111,108,111,114,83,112,97,99,101,62,10,32,32,32,32,32,32,32,32,32,60,101,120,105,
102,58,80,105,120,101,108,89,68,105,109,101,110,115,105,111,110,62,51,53,52,60,47,101,120,105,102,58,80,105,120,101,108,89,68,105,109,101,110,115,105,111,110,62,10,32,32,32,32,32,32,60,47,114,100,102,
58,68,101,115,99,114,105,112,116,105,111,110,62,10,32,32,32,60,47,114,100,102,58,82,68,70,62,10,60,47,120,58,120,109,112,109,101,116,97,62,10,43,101,189,119,0,0,64,0,73,68,65,84,120,1,237,189,11,176,165,
213,121,158,73,211,87,26,68,35,209,13,13,234,230,230,134,70,92,91,106,174,2,228,22,22,82,203,162,165,32,203,138,85,73,41,30,79,38,51,241,184,202,174,56,83,51,206,36,81,57,74,226,76,77,156,42,213,76,18,
231,30,171,102,28,69,150,37,203,96,212,18,200,238,8,16,230,210,150,220,32,196,205,136,251,173,17,2,196,165,175,116,214,187,251,60,232,229,99,253,255,222,251,156,189,255,179,207,233,111,85,173,253,126,
223,251,125,107,253,255,122,247,254,95,109,29,186,79,31,118,88,142,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,
72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,
84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,
5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,
32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,24,137,2,11,70,178,203,152,55,217,180,105,211,162,119,188,227,29,23,175,90,181,234,226,229,203,151,159,124,196,
17,71,156,186,116,233,210,211,203,60,237,240,195,15,95,178,96,193,130,195,10,30,38,28,100,234,118,213,215,134,94,235,53,150,23,214,144,71,236,87,143,253,153,167,2,115,85,129,3,7,14,180,222,122,172,123,
30,99,114,71,197,131,204,215,95,127,253,176,50,247,236,222,189,251,161,50,31,120,237,181,215,126,240,234,171,175,62,178,115,231,206,219,158,127,254,249,219,182,109,219,182,175,245,70,39,164,56,201,70,
124,248,150,45,91,54,172,93,187,246,231,87,174,92,249,63,46,94,188,120,149,204,22,195,109,194,52,226,9,249,100,229,109,204,107,5,48,205,166,67,198,186,231,196,109,168,90,191,41,19,86,79,13,197,237,221,
187,119,231,115,207,61,247,31,30,123,236,177,223,191,246,218,107,191,91,238,245,245,166,251,157,109,126,34,141,120,243,230,205,231,157,123,238,185,255,246,200,35,143,188,24,243,5,101,180,49,198,124,49,
103,242,38,148,232,212,136,121,35,196,71,174,150,199,126,242,196,84,224,80,83,0,67,141,231,142,60,57,168,126,98,97,140,225,106,136,249,170,70,44,140,49,220,43,175,188,114,219,93,119,221,245,183,182,110,
221,186,35,222,231,36,228,19,101,196,27,55,110,92,113,225,133,23,254,131,227,142,59,238,215,23,46,92,248,134,225,70,227,197,112,157,151,129,194,99,178,112,18,218,57,197,112,109,216,107,42,47,244,147,251,
26,231,6,141,107,251,13,186,54,251,82,129,113,43,128,33,14,123,157,218,186,200,145,183,161,106,253,38,134,171,62,98,76,23,164,70,190,127,255,254,195,158,125,246,217,223,190,227,142,59,62,187,125,251,246,
23,135,61,223,56,251,39,198,136,175,190,250,234,75,203,183,224,63,92,178,100,201,113,24,108,27,202,204,168,19,11,153,53,83,150,144,212,133,228,53,116,174,215,88,94,88,67,30,177,95,61,246,103,158,10,204,
85,5,100,114,109,35,214,61,39,174,161,56,166,246,39,22,98,184,112,228,24,45,61,228,77,184,103,207,158,103,239,190,251,238,143,149,31,87,252,89,219,25,186,172,77,132,17,127,226,19,159,248,224,250,245,235,
175,91,180,104,209,98,25,104,237,219,48,166,11,202,244,98,44,14,158,184,134,18,24,158,216,177,45,86,77,67,235,103,50,102,186,126,38,215,206,181,169,64,155,2,50,180,233,140,218,58,231,106,49,156,48,198,
112,142,152,175,56,98,12,23,142,188,134,250,86,44,126,223,190,125,123,239,187,239,190,171,191,244,165,47,125,99,58,103,29,245,154,153,185,201,8,238,230,83,159,250,212,39,127,234,167,126,234,11,197,124,
23,184,1,123,140,225,70,148,153,137,171,161,56,120,221,38,185,35,124,13,157,83,172,161,181,181,209,196,215,122,147,75,5,230,147,2,50,191,218,136,188,231,196,53,20,199,212,190,50,77,114,208,57,204,86,53,
226,26,98,192,170,17,23,60,240,224,131,15,126,234,11,95,248,194,127,173,157,161,75,174,238,44,29,221,65,49,225,79,173,91,183,238,247,100,166,24,47,232,92,52,96,114,25,96,140,49,90,241,196,17,117,60,56,
226,26,58,167,88,67,235,250,141,65,122,250,237,145,245,84,96,18,21,144,225,245,27,177,199,115,226,26,58,167,56,78,153,40,28,49,166,43,158,184,134,102,190,111,244,137,211,44,102,252,215,138,25,255,94,191,
115,141,179,222,223,85,198,116,245,15,126,240,131,103,94,116,209,69,59,138,241,46,198,124,35,202,76,225,48,220,136,50,61,56,204,85,57,113,27,234,104,170,183,97,175,104,125,228,190,198,185,67,37,70,183,
67,229,188,253,206,41,35,56,148,71,237,252,145,35,111,67,213,218,38,6,172,30,12,215,99,56,71,76,88,28,49,38,44,78,63,166,184,237,182,219,206,191,241,198,27,191,63,91,239,225,162,217,184,240,121,231,157,
119,228,187,223,253,238,63,104,51,97,12,24,196,108,107,40,83,192,124,65,231,20,251,212,153,201,137,107,232,156,98,13,173,107,27,253,234,109,107,179,150,10,204,21,5,100,126,77,35,214,60,39,174,161,115,
138,125,202,48,149,235,249,38,214,179,70,206,243,28,145,123,140,207,37,215,82,93,62,84,252,232,75,229,79,84,92,180,99,199,142,87,88,211,37,206,138,17,95,113,197,21,255,247,178,101,203,206,194,100,7,69,
137,94,155,188,33,170,241,70,120,12,231,40,145,201,137,107,232,156,98,134,214,142,122,140,99,207,81,223,99,238,55,63,21,112,99,154,206,9,155,214,59,79,92,67,231,20,199,169,231,25,3,38,214,243,162,62,158,
227,26,234,44,109,207,21,215,45,127,91,247,172,203,46,187,236,159,23,35,254,219,211,57,255,76,215,140,222,77,250,220,209,199,62,246,177,43,54,108,216,240,45,137,233,6,220,20,171,207,123,201,35,74,108,
113,142,49,214,173,137,243,9,87,67,231,20,107,104,109,211,104,171,53,173,73,62,21,152,139,10,96,96,181,123,143,53,207,137,107,40,206,121,197,152,47,53,114,71,197,77,179,246,163,8,255,177,4,49,88,140,248,
125,95,253,234,87,111,170,157,107,156,92,231,223,136,207,56,227,140,223,116,99,197,128,157,243,152,58,168,90,156,50,64,56,98,97,140,225,36,40,177,144,220,177,45,238,45,152,122,97,189,115,25,167,2,135,
146,2,152,167,159,57,114,158,19,59,42,174,229,122,174,49,93,213,245,188,69,244,103,153,216,239,37,198,92,71,188,98,159,229,79,112,253,102,161,175,140,107,198,157,119,106,196,229,119,71,92,120,212,81,71,
189,31,211,4,221,100,21,147,55,33,235,28,245,6,40,175,33,111,142,163,132,37,39,174,161,115,138,53,180,110,208,49,76,239,160,123,102,95,42,208,181,2,110,94,109,215,142,125,158,19,215,208,57,197,62,245,
92,99,198,122,158,84,115,84,28,167,238,81,92,219,240,107,16,203,159,202,95,46,187,232,186,235,174,187,189,109,237,168,107,157,26,113,249,75,27,159,113,243,108,139,7,49,97,122,36,56,123,17,243,198,136,
39,6,37,34,177,144,188,134,206,245,26,167,94,88,231,220,160,241,76,214,14,122,141,236,75,5,102,162,128,140,105,216,81,91,227,28,113,13,157,83,236,83,38,172,103,70,156,35,188,238,83,124,219,115,213,182,
191,95,75,241,233,167,159,254,15,203,150,87,15,123,254,153,244,119,102,196,229,23,249,172,63,250,232,163,63,130,97,70,196,84,157,135,139,232,61,30,235,141,32,231,141,81,78,236,40,209,200,137,107,232,156,
98,134,214,246,27,131,244,244,219,35,235,169,192,164,40,128,153,53,221,79,173,238,28,113,13,157,83,236,83,207,17,185,98,12,152,88,247,35,142,209,246,220,177,79,27,202,167,244,199,107,191,241,141,111,220,
203,158,227,198,206,140,248,164,147,78,186,10,83,116,196,56,133,24,110,141,139,181,152,75,124,214,17,11,153,170,105,144,131,206,17,11,53,212,195,240,184,141,163,214,37,214,238,173,203,235,231,181,102,
79,1,25,202,36,140,218,125,56,87,139,197,193,19,147,203,88,225,244,249,86,28,13,88,124,156,81,11,246,99,175,126,200,53,214,172,89,115,85,217,107,254,25,241,177,199,30,251,81,140,114,38,24,13,56,238,165,
55,70,156,99,124,179,200,245,166,197,24,78,168,161,58,195,99,56,176,173,70,79,98,42,48,159,20,192,228,106,103,242,90,45,134,19,198,88,185,158,39,106,66,12,18,84,93,113,219,96,95,245,248,94,181,152,125,
229,29,170,151,127,136,226,163,101,217,255,211,182,255,40,107,157,124,35,46,127,62,239,109,229,135,224,87,74,60,38,6,74,14,194,79,23,181,15,107,217,179,31,74,80,245,212,176,71,90,157,220,123,157,155,110,
204,245,167,187,62,215,165,2,51,85,192,141,107,216,189,106,107,157,171,197,112,194,126,83,207,7,198,203,186,65,239,177,223,222,218,87,158,65,159,114,253,71,59,249,214,45,183,220,242,227,65,175,51,147,
190,78,140,184,252,88,226,194,34,228,66,137,25,77,146,124,16,140,223,134,201,125,95,237,67,46,140,49,156,68,107,138,169,57,42,102,104,93,211,104,171,53,173,73,62,21,152,107,10,52,153,97,141,135,3,117,
86,98,33,19,158,92,40,83,36,87,93,207,23,134,204,243,91,123,230,180,166,182,31,123,129,110,194,138,181,151,60,164,196,11,79,56,225,132,139,202,30,223,236,109,52,230,151,78,140,184,124,205,191,16,163,69,
60,242,233,34,38,204,122,4,84,78,236,200,117,29,165,45,57,113,13,197,105,168,183,54,154,248,90,111,114,169,192,124,83,0,211,243,115,69,142,188,134,206,41,246,169,103,75,6,233,61,92,7,142,220,145,61,228,
7,196,142,110,186,226,149,171,215,121,249,86,217,115,254,24,113,249,235,204,39,96,120,110,156,112,142,212,133,240,112,202,251,197,172,139,200,94,142,122,227,200,137,107,40,78,67,189,181,209,196,215,122,
147,75,5,230,155,2,50,178,56,34,71,94,67,231,20,199,169,189,189,135,107,193,81,215,51,239,49,251,136,87,140,201,234,121,141,156,242,104,198,229,31,169,56,145,107,141,27,59,249,70,92,14,244,78,12,15,212,
193,17,68,49,147,122,172,145,131,172,247,220,247,136,117,245,249,148,176,181,28,30,225,213,227,35,230,131,214,188,47,227,84,96,62,40,128,17,198,103,66,188,115,158,179,198,81,49,61,196,32,230,168,92,3,
84,28,141,149,186,158,125,245,129,138,117,63,158,179,86,28,177,122,124,150,47,144,243,203,136,203,63,123,191,198,15,72,44,17,136,251,33,189,142,138,227,212,62,222,211,111,95,213,53,154,208,107,189,70,
235,37,111,67,246,109,235,201,90,42,48,233,10,184,1,114,175,181,207,54,166,71,143,231,196,53,20,167,169,161,125,101,142,26,122,150,197,43,87,140,105,42,142,67,125,244,131,218,171,54,125,175,88,167,86,
190,64,174,137,215,24,87,222,213,55,226,147,117,88,29,48,30,186,150,171,207,39,61,172,7,225,29,169,69,244,30,143,37,172,242,54,244,90,175,113,234,133,117,206,205,229,120,190,157,103,82,223,11,12,103,82,
239,175,118,95,253,62,27,156,137,190,90,174,154,248,38,84,141,169,231,151,216,77,88,188,6,134,172,189,152,172,137,200,62,194,166,117,126,13,246,43,70,124,82,77,139,113,112,157,24,241,226,197,139,123,63,
35,214,1,56,100,19,74,196,88,115,46,198,202,157,211,90,114,48,238,23,115,238,171,134,206,41,214,208,250,65,199,48,189,131,238,153,125,115,91,129,185,250,153,144,145,53,13,206,68,79,45,87,77,124,13,125,
95,245,200,24,25,122,142,163,81,138,243,193,158,226,137,181,207,32,83,251,168,47,98,249,55,52,87,247,200,14,94,58,49,98,14,56,136,40,77,61,253,76,85,245,182,158,88,243,123,170,197,104,207,27,212,148,195,
179,135,231,25,167,2,243,73,1,127,22,100,118,181,161,30,175,121,78,28,145,125,224,181,94,207,171,16,67,86,77,67,168,154,120,197,195,206,65,215,78,93,107,240,111,92,189,187,155,254,75,39,70,236,34,18,115,
203,81,200,41,1,134,22,56,238,211,47,247,235,212,98,191,63,98,250,60,247,56,158,205,107,25,167,2,243,73,1,125,214,221,112,199,117,54,158,169,26,138,27,118,234,62,107,107,224,65,122,148,119,49,58,51,98,
63,24,177,144,209,198,121,143,98,122,61,102,47,106,109,216,182,78,53,13,223,239,32,115,240,21,222,185,24,15,210,19,215,100,158,10,204,21,5,48,224,97,63,231,90,215,182,198,235,241,26,172,19,210,167,216,
249,24,83,135,151,190,112,196,32,60,189,160,234,93,140,78,140,216,15,226,7,246,152,158,54,78,61,94,87,236,131,218,160,125,177,159,117,142,49,86,174,17,175,125,144,109,127,157,206,154,246,29,179,154,10,
140,79,1,204,48,94,161,233,115,76,191,215,49,77,223,3,14,140,53,229,190,7,57,253,170,41,102,40,175,77,234,172,167,135,188,134,77,156,248,113,142,206,140,88,34,180,13,23,73,125,244,131,206,177,15,107,64,
231,137,65,122,64,120,161,184,26,58,215,107,40,47,244,146,59,182,213,188,47,227,84,96,46,40,208,239,243,236,102,168,243,208,239,188,56,114,226,136,104,193,122,114,33,107,21,179,142,88,57,83,92,211,136,
251,146,71,212,122,184,166,189,198,197,119,98,196,253,14,23,235,228,142,30,55,9,166,158,90,31,156,139,232,189,190,159,247,54,197,236,227,117,184,73,199,185,120,207,147,174,233,40,239,207,141,103,148,251,
142,99,47,255,44,249,125,195,195,41,143,113,228,116,127,244,248,189,210,231,72,28,251,148,171,6,42,246,188,87,104,121,161,151,245,45,173,35,47,117,98,196,220,181,31,20,110,82,177,223,189,246,171,15,122,
174,81,237,51,232,245,178,111,178,21,152,205,207,67,205,8,7,85,75,247,221,182,190,169,222,196,15,122,221,97,250,102,83,219,126,247,249,230,63,140,215,175,59,235,35,85,96,146,63,24,35,61,104,110,54,39,
20,200,207,227,236,189,77,157,124,35,230,127,41,193,120,220,200,215,242,26,23,247,81,174,190,233,124,160,88,23,145,107,192,123,174,120,58,215,138,123,144,39,166,2,115,81,1,61,27,113,68,142,188,9,227,122,
114,250,155,242,38,62,174,83,95,141,99,125,172,247,235,245,117,163,136,39,230,27,113,60,184,114,166,31,212,251,60,110,18,178,105,15,231,227,62,92,47,242,49,231,154,236,85,171,179,87,98,42,48,159,20,104,
251,204,199,231,32,230,81,135,218,94,112,244,250,30,30,171,238,121,140,149,51,107,123,197,245,244,116,141,157,124,35,142,135,69,24,23,45,246,212,132,160,31,100,13,251,241,237,84,185,98,144,62,246,164,
15,158,156,126,161,6,123,16,11,169,57,167,152,225,117,184,65,145,251,24,180,63,251,82,129,81,40,48,147,207,44,215,175,237,225,92,45,22,231,188,246,106,226,168,249,245,98,47,123,129,244,58,122,205,99,122,
156,243,152,250,184,176,19,35,230,64,66,98,14,20,57,242,26,106,13,235,107,117,56,12,20,228,90,53,108,234,113,222,99,237,161,92,131,123,233,37,225,133,158,64,183,166,109,251,181,46,204,98,42,208,129,2,
131,124,62,99,143,231,196,53,20,215,52,117,52,95,51,147,62,246,242,253,106,28,117,213,186,24,157,24,177,14,194,193,106,34,82,247,154,31,222,249,24,215,214,170,7,222,247,201,56,21,72,5,102,166,0,207,86,
211,46,177,238,57,113,13,197,49,181,55,113,19,250,245,135,237,105,218,31,158,189,181,111,87,163,19,35,118,161,56,24,156,114,226,233,98,109,15,56,225,108,12,157,101,58,223,138,103,227,94,243,154,169,64,
63,5,244,121,30,100,196,62,207,137,107,40,110,152,169,95,250,51,76,191,238,221,251,201,35,198,30,213,187,24,157,24,113,17,109,95,249,173,71,139,106,135,116,142,88,7,39,238,135,122,67,248,77,77,244,194,
129,109,66,106,77,205,48,157,247,152,123,171,173,137,215,209,186,218,24,100,109,109,93,114,169,64,23,10,52,125,110,251,93,187,182,14,14,212,30,196,194,24,195,57,234,57,86,14,82,83,30,57,106,53,228,218,
181,26,28,61,194,178,247,62,97,23,163,19,35,222,183,111,223,227,229,31,251,60,133,195,246,67,137,203,175,171,115,147,21,47,19,3,155,246,145,112,170,105,176,87,47,9,47,234,193,20,137,65,181,198,88,156,
247,43,215,128,59,152,245,127,213,190,57,82,129,249,160,64,237,179,28,57,207,137,29,99,172,156,169,231,151,120,88,212,90,214,123,60,232,62,123,247,238,125,162,171,247,168,19,35,222,179,103,207,163,229,
183,221,159,50,168,0,234,147,112,109,166,75,221,247,20,167,225,38,174,156,94,237,23,167,234,218,67,124,68,106,66,213,107,185,115,189,134,62,47,236,211,167,45,203,169,192,172,41,160,231,96,58,163,182,206,
57,226,26,138,107,155,122,134,85,23,18,123,191,243,177,30,115,246,241,245,181,184,24,241,163,211,209,97,58,107,186,50,226,199,227,65,37,78,52,69,255,246,235,253,222,171,88,67,107,137,57,56,6,76,29,140,
215,137,57,125,53,132,211,253,48,180,222,115,248,26,170,215,199,160,235,124,77,198,169,192,108,42,48,204,103,54,246,122,78,92,67,113,76,157,85,207,54,185,144,220,81,49,147,94,175,195,69,164,199,247,141,
61,202,203,23,200,199,187,210,189,19,35,222,181,107,215,19,126,80,9,129,153,213,248,104,148,244,106,157,134,114,226,40,20,102,12,170,87,113,28,186,46,215,81,141,88,60,57,8,87,203,123,205,246,162,125,124,
248,90,231,51,78,5,230,170,2,253,62,211,177,238,57,177,176,45,166,94,51,77,113,51,153,109,123,83,19,22,223,122,178,171,247,168,19,35,126,237,181,215,30,151,112,50,41,144,3,147,99,132,50,205,166,154,68,
97,143,38,129,180,22,227,101,79,56,114,246,137,57,188,99,140,107,185,184,218,208,254,57,82,129,249,162,128,158,163,126,35,246,120,78,92,67,113,206,147,203,31,20,131,196,202,107,147,58,200,58,71,214,169,
135,190,24,171,103,247,238,221,243,235,27,241,206,157,59,191,125,210,73,39,189,97,194,58,36,38,40,68,4,68,137,53,250,245,33,232,103,110,212,181,151,12,153,189,148,19,59,178,39,235,64,120,161,134,243,7,
153,58,71,173,13,107,123,181,245,103,45,21,232,74,1,61,39,211,25,181,117,206,213,98,113,240,196,77,40,15,80,205,81,241,160,211,215,114,13,214,82,99,111,234,207,60,243,204,45,211,209,98,58,107,58,249,70,
124,239,189,247,238,216,176,97,195,171,197,128,150,235,176,50,34,14,175,184,109,114,40,245,244,27,218,147,111,212,142,77,251,107,63,106,196,142,49,174,229,226,24,131,220,35,189,137,169,192,92,86,64,207,
90,211,136,53,207,137,29,99,172,220,39,6,89,67,113,62,247,239,223,255,166,220,107,49,214,53,224,184,30,121,193,87,31,122,232,161,191,104,58,227,168,249,78,140,248,123,223,251,222,158,151,94,122,233,218,
99,142,57,230,175,202,172,116,88,12,208,99,56,144,195,42,111,27,136,232,63,146,80,191,214,169,198,126,53,164,143,107,128,240,66,13,231,15,50,117,142,218,164,99,237,60,147,126,207,243,249,254,244,57,157,
203,163,118,255,206,213,98,113,240,196,77,40,159,160,166,152,41,142,216,77,216,99,234,53,244,245,170,115,141,23,95,124,241,58,249,86,87,239,73,39,70,172,195,148,175,249,215,29,125,244,209,61,35,86,46,
35,192,12,136,35,210,39,172,13,68,195,128,61,87,172,253,84,139,251,42,215,128,39,174,161,115,138,53,88,127,48,107,126,29,180,175,121,135,172,164,2,147,161,128,158,167,65,70,236,243,156,184,134,206,41,
102,186,57,138,195,76,61,134,107,66,153,114,155,49,215,246,122,238,185,231,254,120,144,243,142,170,167,51,35,126,244,209,71,111,92,183,110,221,254,34,214,66,25,148,68,195,168,132,113,114,64,239,129,227,
77,138,57,63,142,80,93,235,200,227,222,228,90,31,99,246,20,207,240,88,92,204,233,115,28,164,199,251,51,78,5,38,93,1,61,87,253,70,236,241,188,22,139,131,39,174,161,252,66,188,99,140,163,217,246,203,181,
158,201,222,202,203,186,253,143,60,242,200,13,253,206,58,202,122,103,70,188,109,219,182,167,207,61,247,220,127,189,106,213,170,95,193,164,28,21,147,115,192,152,75,172,56,196,97,184,138,181,38,34,123,71,
212,94,112,196,236,47,62,114,177,70,30,145,181,145,207,60,21,152,235,10,212,158,65,63,83,173,14,7,170,159,88,24,99,56,71,55,93,241,158,99,166,142,152,48,72,45,230,240,66,246,45,223,134,127,231,166,155,
110,122,202,207,53,238,184,51,35,214,65,30,120,224,129,223,62,246,216,99,255,215,98,84,61,151,195,176,132,196,234,139,185,56,31,254,6,17,99,198,160,246,80,141,189,106,168,61,197,131,196,228,142,189,166,
169,23,239,115,222,227,65,122,188,63,227,84,96,210,21,208,243,212,111,196,30,207,107,177,56,120,226,26,98,148,170,17,187,137,214,98,76,23,164,71,121,228,168,21,60,240,131,31,252,224,159,247,59,231,168,
235,157,26,241,214,173,91,31,62,227,140,51,254,115,49,227,255,65,7,193,172,64,231,252,160,131,190,81,110,194,218,83,235,132,181,201,181,184,118,19,114,31,212,155,114,248,97,48,238,57,204,218,236,77,5,
70,169,0,207,216,76,246,172,237,225,28,113,13,157,83,28,167,140,18,14,211,84,78,220,132,24,110,205,124,225,232,209,30,207,63,255,252,127,146,79,205,68,135,233,172,237,212,136,117,131,223,255,254,247,127,
235,210,75,47,253,116,9,23,198,27,174,25,147,196,102,240,70,212,80,107,121,99,100,200,202,37,44,177,114,246,39,142,57,215,113,30,78,8,239,92,27,31,251,50,79,5,230,155,2,254,124,250,217,34,79,14,170,87,
49,57,113,45,215,115,76,157,216,81,113,211,196,100,29,137,125,141,184,50,247,223,127,255,253,255,204,207,209,85,220,185,17,223,112,195,13,15,156,120,226,137,191,122,202,41,167,252,191,126,200,38,147,163,
135,55,162,9,181,94,53,71,197,18,91,88,155,218,27,158,152,235,137,103,120,28,251,232,169,97,92,87,235,73,46,21,152,11,10,232,217,26,100,196,62,207,107,177,56,120,226,54,212,243,172,186,163,27,106,45,198,
120,65,245,120,204,154,199,30,123,236,87,229,79,131,156,115,212,61,63,113,155,81,239,220,190,223,225,191,246,107,191,246,149,21,43,86,124,84,223,88,203,175,200,236,125,115,141,232,53,197,77,83,134,71,
77,49,57,113,13,117,123,240,196,142,49,174,229,226,52,180,79,142,84,224,80,84,0,19,141,103,143,188,231,196,142,49,86,30,167,12,19,142,24,19,237,135,24,175,208,99,173,83,94,254,220,240,31,125,238,115,159,
187,166,156,227,224,47,180,137,7,26,115,222,249,55,226,169,243,188,254,237,111,127,251,151,174,188,242,202,187,151,46,93,186,186,118,70,222,24,213,16,191,9,101,132,170,73,84,25,178,114,197,66,114,204,
82,200,212,222,49,22,167,65,127,140,123,197,80,135,235,135,190,103,191,222,172,167,2,179,169,128,63,127,131,222,71,109,141,115,181,88,28,60,113,204,245,44,123,141,220,81,113,219,172,153,175,250,197,151,
223,178,246,244,173,183,222,250,75,229,156,179,98,194,210,119,182,140,248,176,219,111,191,253,135,229,143,178,93,93,254,234,243,182,69,139,22,29,229,111,54,111,132,56,127,3,106,177,196,148,217,70,196,
96,197,19,59,106,111,114,98,161,134,120,134,199,177,70,79,27,239,61,25,167,2,243,73,1,127,78,253,92,145,247,188,22,139,131,39,110,66,61,207,170,213,80,92,219,172,153,113,249,71,43,94,190,235,174,187,174,
150,31,249,25,186,142,127,226,56,93,95,121,234,122,31,249,200,71,206,57,255,252,243,255,180,152,241,74,126,52,33,99,245,88,121,109,202,36,197,215,80,92,219,212,229,169,19,59,198,184,150,139,99,104,175,
28,169,192,161,164,0,230,89,59,115,172,121,78,236,24,99,229,77,179,102,194,234,109,51,97,213,48,98,226,242,77,248,185,29,59,118,92,121,253,245,215,223,85,59,67,151,220,68,184,199,7,62,240,129,211,46,184,
224,130,109,229,95,241,88,59,172,9,71,35,150,33,194,97,180,142,18,151,60,198,228,142,49,86,206,208,62,115,117,204,229,123,159,171,154,183,221,55,70,212,214,51,201,181,166,251,119,158,24,212,121,20,147,
215,98,56,16,19,86,78,44,140,49,92,68,204,184,152,240,99,119,222,121,231,166,27,111,188,241,161,73,208,117,98,156,228,178,203,46,59,241,162,139,46,250,252,219,222,246,182,159,145,145,182,77,153,8,117,
98,97,140,225,34,74,120,56,98,199,24,215,242,38,78,124,142,84,224,80,83,64,198,24,71,228,60,39,118,140,177,242,218,196,116,85,35,198,112,225,200,35,202,136,95,126,249,229,111,150,31,69,124,250,150,91,
110,233,236,23,191,71,109,98,62,49,70,60,117,99,11,62,249,201,79,126,252,244,211,79,255,87,229,71,21,199,97,182,142,152,173,56,98,71,12,150,58,185,246,39,22,146,183,161,215,122,11,166,94,88,239,220,32,
241,116,215,13,178,119,246,164,2,163,84,64,134,54,157,81,91,231,28,113,27,170,230,117,114,76,55,230,240,142,209,128,149,151,159,7,63,91,254,118,239,47,127,241,139,95,252,114,57,219,244,14,56,29,81,6,88,
51,105,70,220,187,229,141,27,55,174,40,243,239,29,127,252,241,191,90,126,86,188,20,35,150,145,197,24,78,200,140,38,12,175,205,99,12,231,216,22,171,198,208,94,243,109,204,199,51,77,210,123,132,193,76,210,
61,141,234,94,106,103,139,28,121,27,170,230,117,114,33,102,11,71,238,24,227,242,45,120,119,249,237,143,159,219,190,125,251,63,45,243,197,81,157,119,148,251,76,180,147,108,218,180,233,168,213,171,87,95,
121,194,9,39,124,186,252,46,227,191,82,76,98,33,38,219,132,24,109,19,74,60,204,134,30,231,218,98,213,52,88,127,48,27,238,117,38,107,135,187,82,118,167,2,211,83,0,19,156,206,234,218,90,231,106,177,56,120,
71,248,38,116,195,165,71,220,20,191,255,133,23,94,248,195,167,158,122,234,243,79,63,253,244,159,148,95,58,246,242,116,206,211,213,154,137,54,98,23,225,242,203,47,127,123,49,229,115,150,47,95,126,226,178,
101,203,214,20,60,185,224,73,229,27,243,226,210,119,0,99,214,154,18,247,114,197,24,223,148,233,190,193,123,77,177,134,214,29,140,222,252,170,189,103,50,184,135,153,236,145,107,83,129,217,80,0,99,156,238,
181,101,138,181,81,248,55,121,143,95,103,202,72,23,192,129,226,89,39,78,121,25,11,202,55,222,189,229,31,250,124,244,213,87,95,125,164,224,227,5,159,44,230,123,247,205,55,223,252,163,218,181,147,75,5,82,
129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,
72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,
84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,
5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,
32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,
82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,
21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,152,131,
10,44,152,43,247,252,161,15,125,104,221,170,85,171,222,115,228,145,71,174,89,182,108,217,201,71,28,113,196,105,75,151,46,61,229,240,195,15,95,82,230,129,5,11,14,30,69,88,242,195,132,83,179,87,243,186,
206,76,174,94,6,156,231,133,59,64,238,24,123,189,150,113,42,112,40,42,112,224,64,245,81,57,172,240,11,98,205,243,215,95,127,189,39,23,156,163,226,50,123,235,21,171,215,235,37,95,80,230,158,221,187,119,
63,252,218,107,175,61,180,107,215,174,71,94,121,229,149,199,119,238,220,249,231,95,255,250,215,31,156,43,239,195,68,27,241,166,77,155,214,156,122,234,169,91,142,63,254,248,255,169,24,239,187,49,216,38,
148,57,50,49,88,16,30,3,37,215,27,5,215,22,199,154,242,218,240,189,106,245,228,82,129,185,174,0,70,216,118,142,216,227,121,45,22,7,79,60,8,98,204,53,44,198,252,157,103,158,121,230,223,253,224,7,63,184,
118,219,182,109,143,183,221,239,108,215,38,210,136,101,192,231,157,119,222,191,56,230,152,99,126,94,70,234,83,70,71,78,44,140,49,92,68,9,14,71,92,67,231,20,51,180,182,223,24,164,167,223,30,89,79,5,38,
81,1,204,178,237,222,106,61,206,17,215,208,57,197,77,19,227,85,157,88,24,99,184,23,94,120,225,247,119,236,216,241,119,38,213,144,251,187,74,155,226,35,174,173,91,183,110,233,101,151,93,246,63,175,93,187,
246,255,90,184,112,225,50,12,55,162,140,14,142,88,24,99,56,71,221,50,57,113,13,157,83,204,208,218,182,209,175,222,182,54,107,169,192,92,80,0,179,108,187,215,216,227,57,113,13,157,83,92,155,152,173,106,
196,24,46,28,185,227,254,253,251,119,61,246,216,99,255,251,45,183,220,242,111,30,124,240,193,221,109,247,223,117,173,221,85,58,188,155,205,155,55,175,63,247,220,115,191,92,126,4,113,22,38,43,44,134,252,
134,193,58,47,195,35,199,88,149,215,98,204,145,90,204,117,76,184,182,56,214,148,55,13,223,175,169,39,249,84,96,46,41,32,147,27,100,196,62,207,107,177,56,120,226,152,99,184,212,61,199,108,85,35,110,195,
242,35,139,123,238,186,235,174,143,111,221,186,245,190,65,206,211,69,207,68,24,241,150,45,91,222,83,76,248,155,139,22,45,58,70,198,139,193,10,49,98,208,107,50,59,114,197,228,196,109,40,113,85,111,67,175,
245,26,203,11,107,200,155,112,208,190,166,245,201,167,2,147,166,0,230,216,239,190,98,159,231,196,109,168,90,219,28,212,132,181,71,249,22,252,22,115,22,183,111,223,190,23,138,25,255,204,181,215,94,251,
231,253,206,211,69,125,214,141,248,154,107,174,249,233,179,206,58,107,107,49,225,55,126,20,225,102,236,177,155,110,140,101,124,226,28,21,215,166,132,133,39,174,161,56,13,245,198,81,227,98,79,45,159,238,
186,218,94,201,165,2,227,80,0,147,28,118,239,218,186,200,145,215,208,57,197,181,137,9,215,208,57,197,154,209,136,61,47,102,188,235,158,123,238,217,252,149,175,124,229,191,13,123,214,81,247,191,213,97,
70,125,133,150,253,138,9,95,117,246,217,103,95,95,204,118,145,76,212,77,183,41,142,6,28,205,151,28,163,85,174,1,194,99,136,253,144,219,167,143,92,88,227,188,62,201,241,92,190,247,73,214,117,186,247,134,
9,77,119,253,108,175,171,221,127,228,200,219,80,53,166,206,132,185,194,9,225,28,99,172,92,211,141,183,22,23,51,222,119,239,189,247,126,248,15,254,224,15,110,156,77,13,103,205,136,223,247,190,247,173,189,
226,138,43,238,46,223,132,143,198,116,65,153,38,49,40,46,78,153,9,28,177,163,226,166,41,209,169,17,59,182,197,170,49,180,71,142,84,224,80,86,64,230,88,27,206,215,98,56,97,140,225,34,186,225,170,134,225,
122,12,215,100,196,24,50,184,119,239,222,151,110,190,249,230,115,190,245,173,111,61,86,59,71,23,220,162,46,46,18,175,177,113,227,198,197,23,93,116,209,127,169,153,48,198,11,98,180,49,135,151,17,198,24,
131,165,70,174,251,32,6,225,106,232,156,98,134,214,14,58,134,233,29,116,207,236,75,5,186,84,64,38,55,232,168,245,58,71,92,67,113,76,93,79,49,198,11,175,103,93,28,168,231,203,99,158,107,80,251,244,123,
6,229,67,23,92,112,193,23,202,95,4,217,180,125,251,246,189,90,211,245,152,45,35,254,63,203,223,144,187,12,3,197,100,155,80,125,212,88,227,200,155,81,67,222,144,136,18,26,142,184,134,206,41,102,104,237,
168,199,56,246,28,245,61,230,126,115,95,1,153,218,56,70,109,95,231,136,107,232,156,98,166,158,115,98,76,89,207,137,56,161,234,226,21,215,166,206,41,222,177,151,76,189,176,247,81,71,29,245,222,13,27,54,
252,253,98,196,159,241,122,87,241,232,221,164,207,157,255,236,207,254,236,134,11,47,188,240,59,18,16,115,117,244,216,123,98,172,156,201,27,82,195,218,155,3,167,91,141,49,183,47,158,225,177,184,152,211,
215,132,195,246,55,237,147,124,42,208,181,2,50,170,97,70,236,247,188,22,139,131,39,110,66,140,184,134,226,124,242,99,7,113,196,142,138,153,244,40,191,243,206,59,223,243,181,175,125,237,59,195,156,121,
20,189,157,127,35,94,191,126,253,63,196,64,29,101,192,110,194,30,211,7,71,14,202,232,98,44,14,158,24,148,112,77,49,53,71,197,26,90,83,27,77,124,173,55,185,84,96,62,41,128,137,198,51,69,158,28,84,191,98,
242,90,44,78,38,73,77,207,184,114,80,207,157,106,60,203,142,218,191,233,185,100,63,245,48,224,206,56,227,140,191,95,140,248,231,224,187,194,78,141,248,195,31,254,240,89,229,175,45,95,131,105,10,163,185,
198,188,95,143,196,246,253,200,253,77,81,157,92,194,18,11,61,239,37,83,117,248,200,145,131,236,65,158,152,10,28,106,10,200,196,106,195,249,90,140,249,105,109,45,22,167,231,139,154,231,60,195,50,102,226,
248,44,146,131,92,199,239,213,247,86,188,98,197,138,143,151,95,48,118,118,249,133,65,223,243,190,113,199,157,26,241,153,103,158,249,27,18,197,141,179,22,99,198,66,143,107,189,206,177,183,99,140,37,168,
56,159,112,142,109,177,106,12,237,147,35,21,56,148,21,144,129,213,134,243,181,24,78,232,83,123,145,235,249,34,198,116,65,158,97,229,62,106,207,164,246,240,193,158,53,44,223,138,127,163,24,241,95,247,254,
113,199,157,185,72,249,217,240,201,151,92,114,201,195,50,78,55,215,104,182,228,222,19,215,184,249,18,75,252,24,243,70,181,161,4,230,141,107,66,222,4,234,228,96,19,79,61,49,21,152,175,10,68,131,227,156,
145,39,7,213,167,152,156,184,13,101,184,170,11,99,12,39,228,103,193,196,181,159,5,195,69,44,127,172,184,183,254,246,219,111,63,181,252,21,232,135,57,207,184,177,179,111,196,229,87,89,94,133,81,14,138,
77,102,220,182,94,166,168,186,163,226,218,148,184,240,196,142,109,177,106,62,180,79,142,84,224,80,82,0,19,173,157,217,107,181,24,78,24,99,56,71,153,170,158,49,144,184,118,109,231,216,27,206,247,244,152,
125,229,29,226,87,175,94,125,85,89,243,239,88,55,110,236,204,136,203,47,117,223,34,241,220,36,49,84,120,114,199,104,198,170,193,121,159,98,246,169,161,56,159,18,150,60,198,228,66,13,245,249,136,185,215,
136,7,233,161,55,49,21,152,11,10,68,83,171,221,115,236,241,188,22,139,99,106,63,98,71,76,50,34,207,175,99,188,39,237,211,180,175,95,67,254,225,249,219,223,254,246,45,101,221,252,50,226,75,47,189,244,136,
163,143,62,122,179,155,37,177,68,84,60,232,108,50,97,214,179,159,163,191,81,49,214,155,4,167,88,67,185,99,140,123,197,169,23,122,157,203,56,21,56,148,20,192,236,226,153,157,175,197,24,159,214,17,215,80,
207,88,52,97,229,253,70,188,102,109,111,113,218,75,254,65,172,235,21,191,250,144,124,235,214,91,111,125,173,223,117,70,81,239,228,27,241,9,39,156,240,158,114,208,37,58,160,14,44,36,198,64,71,137,236,205,
117,98,46,225,188,70,94,67,231,20,51,180,126,152,49,108,255,48,123,103,111,42,48,14,5,220,200,6,217,191,214,239,28,113,13,197,57,47,115,132,19,234,249,113,51,110,187,31,223,71,125,190,143,199,236,39,239,
17,239,134,92,184,37,229,199,19,27,203,242,155,219,174,53,170,90,39,70,92,14,116,137,132,212,129,65,140,55,230,240,96,191,58,125,142,172,17,198,24,78,2,198,24,174,134,226,52,180,166,223,24,164,167,223,
30,89,79,5,38,81,1,76,174,237,222,98,15,57,168,181,138,201,107,177,158,33,241,110,200,172,107,187,182,247,176,175,188,129,152,61,181,63,188,27,50,177,234,43,87,174,188,164,236,55,127,140,184,252,35,159,
239,212,161,57,188,144,9,79,30,81,117,159,170,123,94,139,233,97,47,245,16,131,122,195,218,98,213,53,212,227,35,230,94,83,220,175,30,251,51,79,5,230,154,2,50,179,182,17,235,158,19,59,42,174,229,145,211,
53,225,6,185,62,251,10,121,214,133,242,3,106,124,11,198,128,85,131,43,255,72,241,59,219,174,51,202,90,39,223,136,203,129,78,140,66,120,142,56,53,83,117,19,109,170,59,239,123,249,53,124,31,9,232,53,207,
17,87,117,134,199,244,82,107,194,184,166,169,47,249,84,96,174,40,128,9,182,125,182,49,61,206,228,121,140,213,35,142,25,115,241,206,41,198,36,123,133,202,139,214,232,89,119,100,127,16,211,213,57,212,203,
158,206,171,166,47,144,149,75,140,133,234,196,136,151,44,89,178,86,7,243,233,230,233,38,233,61,196,244,42,247,94,120,144,58,61,32,251,52,161,148,85,173,134,61,210,234,77,57,252,176,200,117,135,93,151,
253,169,192,168,20,144,65,13,50,250,125,86,181,79,236,241,189,137,233,115,84,76,221,239,69,207,176,120,204,82,53,143,233,101,47,250,65,221,79,156,170,69,211,141,61,202,23,47,94,188,134,253,199,141,157,
24,113,249,95,150,147,106,7,109,226,36,148,79,250,196,41,166,6,15,58,239,189,212,35,74,92,56,98,199,182,88,53,13,173,207,145,10,204,101,5,102,250,25,198,64,125,159,200,97,148,53,148,118,90,171,90,109,
98,188,160,250,163,145,234,121,215,208,122,197,142,190,39,235,212,67,172,107,215,166,124,171,183,105,7,47,157,24,113,249,95,150,119,214,14,90,227,48,80,175,213,56,234,170,197,58,57,72,111,19,74,103,213,
106,232,92,175,97,234,133,126,231,250,197,211,89,211,111,207,172,167,2,227,80,64,230,53,232,224,115,237,107,34,167,92,245,38,244,181,241,186,122,142,49,97,55,79,241,26,236,171,189,197,121,46,110,186,179,
252,158,226,19,227,189,140,43,239,196,136,203,205,23,45,166,47,136,175,117,115,141,177,231,190,70,177,215,36,166,215,61,39,22,106,168,207,71,204,189,166,184,95,61,246,103,158,10,76,162,2,109,159,227,38,
211,100,141,215,197,145,19,71,228,252,240,234,103,202,120,53,84,3,245,44,107,96,202,202,213,175,158,81,205,169,107,189,249,225,239,93,117,60,47,157,24,177,139,72,236,199,113,241,224,35,71,174,58,113,196,
182,154,247,198,62,207,21,107,168,223,177,151,24,79,94,67,214,214,106,201,165,2,115,89,1,12,111,20,103,232,183,151,215,121,166,132,240,138,135,153,186,103,250,61,22,71,14,122,95,175,56,230,151,206,140,
152,195,198,131,194,115,112,144,115,147,215,144,189,134,237,141,123,177,143,120,70,83,76,47,125,77,232,235,155,122,146,79,5,230,146,2,24,96,219,61,183,245,180,213,180,167,215,61,86,141,231,73,56,76,92,
91,203,30,113,31,120,95,163,184,139,209,137,17,115,16,63,104,19,71,15,168,190,24,215,56,122,132,212,65,106,49,247,94,143,123,27,148,23,56,214,193,131,94,135,155,75,56,215,239,127,46,105,221,118,175,50,
157,185,48,218,62,47,156,193,123,156,115,99,109,138,165,129,175,65,19,250,65,93,131,62,245,40,231,186,196,142,177,135,188,134,206,41,238,106,116,102,196,8,165,131,69,145,56,172,247,212,56,234,96,211,94,
212,193,216,87,227,233,113,140,113,45,23,199,240,125,225,18,83,129,126,10,204,197,207,141,27,161,206,231,103,160,6,135,129,170,143,24,132,19,210,175,152,65,159,163,106,234,245,9,199,58,71,223,183,41,166,
191,95,157,190,81,227,193,159,122,143,122,215,105,236,231,2,104,185,242,200,193,179,125,172,123,78,44,36,102,29,251,212,248,216,67,222,111,159,126,123,177,79,98,42,48,31,20,224,121,168,125,238,35,23,243,
120,126,246,138,188,114,214,58,18,123,63,28,200,90,114,16,190,182,214,185,174,227,78,190,17,35,2,24,15,25,249,90,94,227,226,62,202,99,95,173,167,198,177,14,172,245,68,110,152,222,184,182,171,124,46,220,
99,87,90,204,197,235,232,155,224,164,15,125,198,226,125,214,56,157,3,62,226,168,207,168,253,227,168,113,222,227,117,143,189,103,92,113,39,70,60,174,155,31,197,190,77,130,59,239,49,215,172,113,212,6,193,
153,174,31,228,26,217,51,247,21,232,234,115,18,141,116,88,229,116,159,113,15,231,60,30,197,222,181,61,134,209,106,152,222,218,181,70,205,117,98,196,188,65,96,191,67,12,218,215,111,159,65,234,186,86,237,
77,113,222,99,246,172,113,212,6,65,173,207,145,10,204,23,5,106,159,103,231,60,246,51,55,241,195,246,168,191,223,94,94,247,216,175,53,91,113,39,70,220,239,112,18,37,10,227,92,140,181,159,115,236,15,199,
255,250,130,170,187,217,170,143,1,47,142,254,200,169,151,53,212,156,99,47,161,215,157,207,56,21,152,47,10,240,44,52,157,39,214,61,39,6,181,135,98,207,125,95,120,71,98,250,148,195,57,214,120,174,199,90,
207,89,235,156,247,141,51,238,204,136,227,33,93,36,14,232,61,53,142,58,168,30,246,17,98,130,196,32,125,236,73,159,243,226,232,23,106,192,17,11,169,57,167,152,225,117,184,196,84,96,190,43,80,251,220,59,
87,139,197,69,30,14,148,110,244,128,112,222,3,231,168,88,35,174,59,200,190,153,175,113,190,142,250,184,176,19,35,230,64,81,56,29,42,114,228,96,236,17,239,28,125,142,24,40,216,91,208,240,210,212,227,188,
199,218,70,185,6,247,210,75,42,47,244,85,74,73,165,2,115,78,129,126,159,119,29,40,246,120,78,92,67,113,76,246,137,185,243,138,25,109,125,170,105,120,15,121,196,166,94,245,141,123,116,98,196,58,4,135,36,
70,24,120,207,225,106,189,222,167,186,143,88,243,125,220,20,157,247,245,209,112,85,139,92,204,125,61,49,215,106,186,14,125,137,169,192,92,84,160,223,231,58,214,61,39,22,214,98,233,17,121,122,155,144,53,
113,29,218,70,222,243,218,90,214,117,137,157,24,177,31,156,216,15,41,142,9,79,78,63,185,234,196,17,189,70,44,212,96,159,131,89,253,85,61,152,104,189,163,63,59,136,81,247,223,37,59,82,129,201,85,160,223,
179,20,235,158,19,11,219,226,88,163,191,141,247,158,90,44,69,107,188,239,25,123,186,122,23,58,49,226,131,231,63,176,160,73,4,231,249,117,119,145,195,224,248,141,75,170,19,11,53,248,45,76,170,105,176,23,
177,246,96,246,26,202,139,122,197,197,1,15,82,103,239,218,26,245,80,167,63,98,211,186,216,151,121,42,48,91,10,244,251,12,55,221,87,109,157,115,196,53,20,55,232,212,115,173,73,191,199,112,51,69,157,177,
236,177,191,233,172,163,230,59,49,226,61,123,246,60,94,126,201,242,218,169,195,189,33,224,76,197,210,122,204,214,247,130,227,122,131,136,166,245,50,73,208,215,136,211,112,19,173,113,190,166,41,102,93,
83,61,249,84,96,174,41,80,251,76,71,142,188,13,85,171,77,140,22,164,71,121,141,163,62,40,74,111,245,70,220,183,111,223,147,61,178,131,151,78,140,120,239,222,189,143,232,159,75,146,104,152,93,155,72,244,
169,151,169,126,120,161,70,220,11,94,223,140,21,131,236,209,132,218,155,189,34,114,29,33,111,86,19,39,190,223,208,254,57,82,129,73,86,192,63,231,195,220,103,109,157,115,196,53,20,215,54,245,60,171,46,
36,142,253,240,142,244,131,113,13,123,58,210,179,123,247,238,71,135,57,255,76,122,59,49,226,114,160,39,150,47,95,254,22,161,37,142,140,201,127,164,128,8,17,233,5,17,206,15,143,241,138,35,214,254,92,131,
94,173,133,23,106,144,171,70,14,194,145,11,157,83,238,131,61,157,35,110,91,71,79,98,42,48,201,10,244,251,12,199,186,231,196,142,138,107,185,56,61,239,212,99,172,220,167,247,123,47,235,29,219,234,212,202,
255,147,127,162,171,247,161,19,35,222,181,107,215,147,81,4,55,62,29,156,188,13,213,167,209,102,116,24,48,125,234,133,243,189,117,63,228,222,171,88,131,107,128,7,217,159,240,228,17,213,175,189,115,164,
2,243,85,129,126,159,239,88,247,156,216,81,113,45,23,135,41,122,44,174,105,14,211,23,123,149,251,44,70,60,191,126,52,81,140,248,113,9,39,147,2,57,176,114,25,37,121,68,250,49,68,246,104,251,144,123,111,
140,219,114,237,233,117,207,253,122,244,56,231,113,191,186,247,102,156,10,204,37,5,244,124,246,27,177,135,28,212,122,197,228,181,24,78,207,127,140,197,13,51,89,15,250,90,113,77,215,40,190,53,191,190,17,
63,243,204,51,183,173,93,187,246,77,38,172,195,203,176,52,61,134,115,164,174,55,80,124,219,144,176,50,118,13,246,16,167,56,242,212,233,173,161,56,141,120,221,152,31,236,106,126,29,182,191,121,167,172,
164,2,221,41,160,103,103,208,81,235,117,142,184,134,226,156,119,115,164,6,39,28,116,106,109,236,245,253,216,147,62,106,194,231,158,123,238,207,6,61,251,76,251,58,249,209,196,83,79,61,245,231,229,192,123,
138,25,45,209,193,101,74,154,30,195,213,80,135,20,239,216,75,42,47,244,73,72,25,175,239,39,206,115,122,155,48,94,143,190,120,217,38,62,246,101,158,10,204,55,5,244,76,213,70,228,201,219,80,181,166,137,
97,58,42,158,201,228,90,236,161,156,184,224,158,167,159,126,122,123,237,108,227,224,58,49,226,91,111,189,245,181,43,174,184,98,235,138,21,43,62,42,211,210,97,133,49,134,115,228,208,226,218,6,162,242,99,
14,80,235,162,33,107,159,120,13,246,143,72,111,188,54,125,145,111,202,135,237,111,218,39,249,84,160,75,5,244,92,13,51,98,191,231,196,53,116,78,49,83,94,17,99,56,97,156,251,247,239,127,19,23,243,216,31,
247,226,90,47,189,244,210,215,229,91,195,156,125,38,189,157,24,177,110,112,231,206,157,215,30,125,244,209,31,213,193,101,74,160,98,29,222,115,113,76,173,85,28,7,130,129,50,91,13,229,152,48,6,44,142,253,
28,213,79,78,236,24,227,90,46,78,163,118,143,7,43,249,154,10,204,79,5,244,92,53,13,175,213,98,56,97,140,225,28,221,48,197,43,111,155,131,24,176,175,143,123,254,232,71,63,186,182,233,108,227,224,59,51,
226,242,115,226,27,78,61,245,212,222,25,48,45,76,176,150,195,13,122,104,222,52,55,97,113,126,141,90,172,253,185,86,19,114,15,212,201,193,38,158,122,98,42,48,95,21,208,51,86,27,145,39,7,181,70,49,57,113,
27,186,25,199,216,77,53,154,112,204,189,215,99,174,45,174,252,88,226,134,218,185,198,197,117,102,196,215,95,127,253,35,235,215,175,255,255,222,254,246,183,255,117,140,75,72,172,3,198,28,46,30,30,193,28,
49,96,113,218,7,100,207,38,244,107,168,167,150,115,125,234,228,96,19,79,61,49,21,152,175,10,232,57,171,141,200,147,131,90,163,152,156,184,13,101,144,170,59,186,145,122,140,249,58,18,123,95,45,46,223,134,
255,255,173,91,183,62,92,59,215,184,184,206,140,88,7,184,247,222,123,127,235,226,139,47,174,26,49,70,201,65,221,220,120,179,84,107,123,163,180,70,117,71,246,109,66,237,169,90,13,157,139,113,111,129,189,
176,135,81,25,166,2,243,94,1,127,54,253,176,145,39,111,67,213,168,203,32,53,132,240,196,142,138,153,234,171,153,173,115,138,61,103,173,227,253,247,223,255,91,126,150,46,226,78,141,248,107,95,251,218,61,
103,158,121,230,151,143,57,230,152,143,235,112,110,94,77,177,139,192,155,20,57,222,40,237,161,216,81,113,219,244,251,80,159,231,49,174,229,189,5,246,194,30,70,101,152,10,204,59,5,106,207,34,135,140,53,
207,137,107,40,174,109,202,44,85,175,161,27,169,98,204,54,34,125,240,228,224,139,47,190,248,229,175,127,253,235,223,227,44,93,97,167,70,172,67,221,119,223,125,159,189,240,194,11,223,100,196,110,94,30,
187,8,188,113,226,252,205,106,250,145,132,122,180,87,211,212,62,212,98,76,46,212,168,221,83,141,59,216,253,214,215,97,122,223,186,58,153,84,96,118,21,240,103,175,223,157,196,94,207,107,177,56,120,226,
38,148,89,170,86,67,140,20,116,163,37,6,189,39,114,229,219,240,63,238,119,198,113,212,59,55,226,242,179,226,239,174,89,179,230,55,79,56,225,132,207,196,3,53,25,22,111,148,250,227,155,36,81,49,99,173,87,
221,81,113,109,106,47,120,98,199,182,88,53,13,173,159,47,99,62,157,101,46,188,39,254,153,158,11,247,219,239,30,107,231,113,174,22,195,9,99,12,231,168,103,93,121,13,49,215,136,24,173,35,49,189,228,79,62,
249,228,63,42,255,175,253,59,253,206,58,142,122,231,70,172,67,108,223,190,253,159,188,255,253,239,255,192,145,71,30,121,89,60,148,191,33,170,241,70,120,12,39,33,49,97,98,71,140,182,9,181,39,53,98,161,
134,27,147,199,7,171,111,174,195,181,97,109,143,182,254,172,165,2,147,160,0,207,227,160,247,18,251,61,175,197,226,224,137,65,93,83,207,51,185,144,220,49,198,202,53,49,88,143,225,132,196,170,191,252,242,
203,223,254,238,119,191,59,43,223,134,117,206,217,50,226,189,197,132,63,85,254,146,199,221,139,22,45,58,26,147,2,117,99,12,137,207,240,55,68,177,250,225,136,29,21,55,77,237,73,141,216,177,45,86,141,161,
61,114,164,2,135,178,2,254,140,186,14,206,215,98,56,97,140,225,34,202,52,197,57,198,88,57,211,205,150,24,164,167,252,222,225,151,238,188,243,206,95,40,95,16,247,250,253,119,25,207,138,17,235,128,223,250,
214,183,30,91,181,106,213,207,189,235,93,239,250,90,73,23,197,55,66,61,254,38,44,92,184,240,141,92,2,234,155,240,76,190,13,99,160,77,168,235,83,139,177,114,13,175,31,100,38,255,117,46,222,243,228,171,
58,186,59,228,57,24,221,142,221,236,84,187,239,200,145,183,161,106,94,215,179,14,7,194,57,198,88,185,166,155,110,45,46,38,188,175,252,119,171,159,147,31,117,163,84,253,42,179,254,117,238,154,107,174,249,
233,179,206,58,107,107,249,102,188,76,198,42,195,197,100,107,185,56,153,137,247,40,135,115,132,143,40,41,224,136,107,232,156,98,134,214,14,58,134,233,29,116,207,236,75,5,186,86,0,115,236,119,221,90,159,
115,196,53,116,78,113,109,98,184,170,17,99,186,181,220,205,87,117,207,139,7,239,186,231,158,123,54,127,229,43,95,249,111,253,206,53,238,250,224,142,50,198,59,217,178,101,203,123,206,61,247,220,111,22,
51,62,38,26,177,27,110,140,101,114,110,204,158,43,110,154,58,10,53,226,26,58,167,152,161,181,131,140,65,251,6,217,43,123,82,129,217,86,0,163,236,119,31,177,207,115,226,54,84,173,109,98,184,234,193,132,
61,118,206,141,23,94,92,49,225,23,238,186,235,174,159,185,246,218,107,255,188,223,121,186,168,15,230,40,29,220,201,230,205,155,215,159,119,222,121,95,89,182,108,217,187,220,112,101,204,50,52,231,220,124,
137,233,17,198,88,185,6,181,152,83,235,53,77,245,213,226,216,71,79,68,246,143,124,230,169,192,124,80,64,166,215,54,106,117,231,136,107,40,142,169,107,16,11,163,1,83,195,96,233,33,111,194,215,94,123,237,
158,98,194,31,47,127,123,238,190,182,115,116,89,155,24,35,214,161,215,173,91,183,244,242,203,47,255,95,202,31,111,251,103,197,128,123,63,170,136,6,140,241,194,203,244,224,98,172,220,167,174,81,203,225,
29,99,92,203,197,197,161,253,115,164,2,243,93,1,153,94,219,136,117,207,137,107,232,156,226,218,172,25,50,166,171,126,226,136,229,155,240,174,199,31,127,252,255,184,249,230,155,127,231,193,7,31,220,221,
118,255,93,215,38,210,53,54,109,218,180,230,252,243,207,255,23,229,215,102,254,60,134,235,40,179,195,124,65,231,220,108,107,177,68,22,223,134,94,235,53,150,23,214,144,215,112,144,158,218,186,228,82,129,
185,164,0,134,217,116,207,177,238,57,113,27,170,214,111,98,200,53,140,38,252,194,11,47,252,254,142,29,59,254,206,182,109,219,30,111,186,231,217,228,39,210,136,17,68,134,124,202,41,167,124,116,245,234,
213,127,243,136,35,142,120,55,166,219,132,110,186,244,96,140,94,211,254,228,196,92,147,254,200,215,114,214,128,190,22,46,49,21,152,175,10,96,164,181,243,197,154,231,181,88,28,60,113,45,199,116,233,33,
175,97,249,17,196,119,202,111,81,251,247,15,63,252,240,31,77,170,1,163,221,68,27,49,55,41,252,208,135,62,180,110,229,202,149,27,143,58,234,168,119,150,159,35,159,92,140,249,180,165,75,151,158,82,12,119,
73,153,7,48,65,161,155,112,201,123,53,175,107,63,114,245,50,224,60,215,122,114,199,216,235,181,140,83,129,67,85,1,204,51,158,191,24,229,155,188,198,251,100,162,26,112,142,138,203,92,48,133,111,252,156,
152,126,237,91,230,158,242,47,197,63,92,140,247,161,242,239,204,61,82,254,114,198,19,229,159,57,218,94,126,103,196,131,189,141,243,37,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,
72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,
84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,
5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,
32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,
82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,
21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,
129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,96,238,43,176,96,174,28,225,242,203,47,127,251,234,213,171,207,89,190,
124,249,137,203,150,45,91,83,240,228,130,39,45,92,184,112,113,57,195,129,195,15,63,252,176,5,11,22,28,38,36,87,32,78,163,240,7,166,176,151,235,133,26,4,61,228,224,212,158,164,211,194,120,173,105,109,146,
139,82,129,89,80,224,192,129,222,163,51,173,43,191,254,250,235,213,117,133,127,147,247,248,53,180,166,228,11,224,192,169,189,22,76,213,15,35,223,191,127,255,222,93,187,118,61,250,234,171,175,62,82,240,
241,130,79,62,253,244,211,119,223,124,243,205,63,170,94,124,2,201,55,137,49,105,247,183,105,211,166,163,138,249,94,121,194,9,39,124,250,152,99,142,249,43,197,204,22,202,20,53,101,108,96,140,149,55,77,
157,145,26,177,99,91,172,154,134,214,79,119,204,100,237,116,175,153,235,82,129,233,40,128,1,14,179,182,182,198,185,90,12,39,140,49,92,13,49,100,213,136,133,83,241,254,23,94,120,225,15,159,122,234,169,
207,23,83,254,147,109,219,182,189,60,204,57,186,238,157,190,163,140,241,78,55,110,220,184,162,204,191,119,252,241,199,255,106,249,198,187,20,243,197,120,155,80,38,71,13,179,117,212,45,147,199,152,220,
81,177,134,214,196,81,227,98,207,160,249,40,247,26,244,154,217,151,10,184,2,24,160,115,195,198,181,61,34,71,14,234,26,138,201,107,49,28,136,233,42,39,174,161,56,205,242,141,121,247,51,207,60,243,185,237,
219,183,255,211,50,95,28,246,92,93,244,191,213,97,186,184,106,243,53,22,124,242,147,159,252,248,233,167,159,254,175,22,45,90,116,156,27,48,177,76,43,198,112,66,102,52,100,93,146,154,144,188,13,123,77,
229,133,126,114,95,227,220,176,113,109,223,97,247,200,254,84,96,148,10,200,220,166,59,106,107,35,71,222,134,170,121,157,28,179,141,57,60,198,171,58,177,227,190,125,251,158,125,224,129,7,126,249,139,95,
252,226,151,203,25,167,127,208,233,10,212,178,110,98,140,248,178,203,46,59,241,162,139,46,250,252,219,222,246,182,159,193,104,155,80,6,70,141,216,81,113,219,148,30,212,137,29,219,98,213,24,218,227,80,
29,135,242,217,107,239,57,198,81,171,29,10,92,211,249,157,175,197,112,194,24,195,69,196,120,197,19,99,184,112,228,53,252,241,143,127,252,205,219,111,191,253,211,183,220,114,203,147,147,242,222,76,132,
147,124,224,3,31,56,237,130,11,46,216,182,100,201,146,181,229,71,17,111,152,44,102,219,132,50,3,213,28,99,172,60,78,137,15,71,236,216,22,171,166,161,245,227,26,227,220,123,92,247,156,251,206,77,5,100,
92,163,26,181,189,156,171,197,112,194,24,195,57,98,188,226,136,29,107,198,235,92,249,49,69,111,221,158,61,123,30,187,243,206,59,55,221,120,227,141,15,141,234,252,51,217,103,124,110,50,224,93,125,228,35,
31,57,231,252,243,207,255,211,197,139,23,175,116,195,29,196,144,101,88,172,193,88,49,230,152,235,118,224,64,184,26,58,167,88,67,235,106,163,137,175,245,38,151,10,204,39,5,48,207,120,166,200,123,78,236,
24,99,114,76,86,57,211,57,76,86,53,226,38,196,132,85,87,92,204,248,185,29,59,118,92,121,253,245,215,223,21,239,191,235,188,238,44,29,221,69,49,225,141,27,54,108,216,86,126,30,124,148,12,212,205,215,99,
106,50,60,140,215,17,94,24,99,184,136,58,34,28,177,99,140,107,185,56,13,237,147,35,21,56,20,21,192,48,227,217,35,239,57,177,99,140,149,215,166,76,20,158,56,26,175,234,110,186,212,225,28,203,207,141,95,
254,139,191,248,139,77,127,252,199,127,188,61,158,161,203,124,81,151,23,243,107,149,159,7,31,123,238,185,231,94,23,77,216,13,216,99,55,222,24,203,8,197,57,198,88,185,166,70,91,204,61,210,75,63,60,232,
117,184,126,56,157,53,253,246,204,122,42,48,78,5,100,106,195,140,216,239,121,45,22,7,95,139,225,48,93,61,231,196,122,158,84,23,250,84,93,195,185,150,51,28,37,31,218,185,115,231,57,229,231,198,63,108,233,
27,107,105,182,140,248,240,247,190,247,189,255,113,233,210,165,171,37,44,134,219,132,222,227,134,171,152,41,209,189,230,111,66,83,44,101,85,107,195,94,209,250,200,125,141,115,115,61,70,143,185,126,142,
73,189,127,76,103,82,239,111,186,247,85,59,87,228,200,219,80,181,218,212,179,45,94,38,171,216,145,231,27,94,53,125,235,133,231,51,29,145,179,150,255,54,181,250,210,75,47,253,143,197,136,175,41,92,253,
111,160,208,60,38,156,149,255,79,253,139,191,248,139,191,124,242,201,39,255,75,9,215,100,190,145,87,47,83,53,137,74,46,36,119,84,28,115,233,8,47,36,175,161,115,189,198,242,194,26,242,26,14,210,83,91,151,
92,42,48,23,20,192,72,155,238,53,214,61,39,174,161,56,231,21,203,84,225,61,135,23,50,85,231,199,14,112,66,56,199,24,43,127,244,209,71,127,229,119,127,247,119,255,101,211,185,198,201,119,110,196,87,93,
117,213,233,229,127,125,190,95,204,180,247,183,228,220,112,155,98,55,220,182,88,6,136,41,199,88,57,83,130,18,11,201,29,219,226,222,130,169,23,214,59,151,113,42,112,40,41,32,3,172,13,231,107,49,156,144,
169,125,136,29,101,168,228,196,110,182,77,177,27,174,122,60,87,28,230,254,219,110,187,237,93,55,220,112,195,3,181,243,140,147,235,252,71,19,239,122,215,187,126,163,102,194,24,44,102,172,188,41,166,215,
81,134,72,142,201,42,39,142,40,81,225,136,107,40,78,67,189,113,212,184,216,51,108,62,142,61,135,189,135,236,63,180,20,144,193,141,98,212,246,113,142,184,134,206,41,142,83,207,5,156,98,153,170,176,223,
212,185,212,19,7,123,129,83,245,133,235,215,175,255,141,98,196,191,20,251,199,157,191,245,14,199,120,197,205,155,55,159,114,201,37,151,60,84,12,118,129,76,18,163,21,198,216,115,239,197,108,107,40,193,
197,243,230,120,12,231,168,163,146,19,115,124,241,145,139,53,242,38,100,143,166,122,242,169,192,92,82,64,166,213,111,196,30,207,107,177,56,120,226,26,202,120,225,137,133,77,147,111,190,170,243,173,215,
227,38,174,252,41,138,3,119,220,113,199,105,91,183,110,125,184,223,89,71,89,239,244,27,113,249,171,203,191,94,204,177,103,194,53,35,117,67,86,61,154,177,115,113,189,76,15,14,115,85,78,92,67,9,9,79,236,
24,227,90,46,206,135,246,203,145,10,204,103,5,48,206,166,51,198,186,231,196,142,49,86,30,167,158,43,56,197,50,85,158,221,26,234,222,196,199,193,181,156,103,95,97,241,140,5,167,158,122,234,223,45,245,95,
241,158,113,199,111,189,211,49,93,113,211,166,77,171,175,188,242,202,199,203,65,139,191,30,252,219,115,110,188,196,160,76,52,198,226,152,170,17,75,240,24,195,213,222,36,56,29,53,198,112,200,160,186,143,
152,123,141,120,144,30,122,19,83,129,185,164,64,205,200,226,253,199,30,207,137,29,99,172,60,78,25,175,56,199,24,43,143,223,132,225,224,249,38,92,195,242,109,184,183,190,224,254,242,219,218,214,222,116,
211,77,79,197,179,141,43,239,236,27,241,73,39,157,244,129,98,80,189,255,64,39,163,138,198,73,14,186,209,194,9,155,120,213,226,190,228,194,56,37,40,28,113,13,157,83,172,161,117,131,142,97,122,7,221,51,
251,82,129,174,21,192,44,251,93,55,246,121,78,92,67,231,20,51,49,91,61,71,138,29,137,227,61,197,103,142,189,212,71,236,200,190,242,16,241,197,99,22,150,63,213,117,85,49,226,207,199,189,199,149,119,102,
196,229,87,90,94,237,134,58,76,140,249,130,90,235,177,132,247,253,200,133,253,166,132,165,135,24,177,197,51,60,110,227,168,205,7,172,157,121,62,156,171,235,51,232,225,158,239,163,118,70,231,106,177,56,
120,226,136,250,12,138,195,44,21,139,83,174,161,152,233,26,171,79,3,164,166,60,78,121,71,228,86,174,92,249,145,178,102,126,25,241,217,103,159,189,228,232,163,143,222,162,3,75,52,76,211,99,56,71,55,219,
166,216,251,217,175,134,226,226,212,155,3,71,236,24,227,90,46,206,135,246,203,145,10,204,119,5,162,193,249,121,99,205,115,98,199,24,43,247,137,9,11,233,229,185,197,144,253,250,30,211,239,251,121,172,245,
242,16,56,174,181,98,197,138,171,229,91,223,251,222,247,246,248,126,227,138,59,249,70,124,230,153,103,158,87,140,116,185,196,211,161,17,209,77,180,22,211,239,53,55,100,231,137,89,227,200,245,106,40,97,
225,137,17,91,60,195,99,56,97,19,239,61,147,24,207,213,251,158,68,45,71,113,79,24,198,40,246,234,122,143,166,123,119,190,22,139,131,39,174,161,62,171,24,164,99,191,115,178,151,250,136,65,246,145,111,136,
243,92,113,225,151,159,118,218,105,231,23,35,190,163,223,117,70,81,239,196,136,87,173,90,245,94,137,169,67,131,110,156,226,152,240,211,69,223,159,61,225,200,37,28,177,144,188,134,206,245,26,167,94,88,
231,92,91,60,108,127,219,94,89,75,5,186,84,64,70,53,200,168,245,57,71,92,67,113,76,93,75,102,72,94,235,111,187,31,239,103,15,127,222,21,203,95,168,77,25,239,27,134,172,154,184,242,227,212,203,202,117,
230,143,17,31,113,196,17,107,116,56,4,112,81,162,225,182,213,124,143,184,142,156,107,208,203,126,158,235,77,132,23,122,222,75,166,234,181,152,94,106,77,200,190,77,245,228,83,129,185,166,0,6,215,118,223,
177,199,243,24,147,11,153,218,91,177,158,31,33,134,12,239,168,56,14,173,209,179,238,168,184,54,181,183,174,163,254,24,139,43,191,11,103,77,220,127,92,121,39,223,136,203,191,182,252,78,29,152,233,166,8,
135,32,170,197,73,15,124,191,94,246,7,233,23,106,176,31,8,87,67,231,20,107,176,207,193,172,255,235,176,253,253,119,204,142,84,160,59,5,100,98,26,131,124,142,233,229,238,60,39,22,106,47,207,21,51,85,147,
49,106,232,25,246,62,113,228,138,227,160,230,168,216,103,52,93,93,79,215,129,87,174,89,124,235,196,184,255,184,242,78,140,184,252,118,163,53,28,14,212,193,227,164,22,145,62,241,181,184,198,209,27,247,
170,229,18,87,124,13,123,164,213,155,114,248,97,145,235,14,187,46,251,83,129,81,43,128,121,197,125,7,253,140,106,189,247,214,114,237,205,117,28,21,251,212,51,237,185,175,147,97,170,238,67,189,186,54,235,
64,113,113,170,134,233,122,28,251,228,91,126,141,113,198,93,25,241,73,241,144,253,114,9,164,30,48,246,215,120,113,240,96,219,30,18,150,125,137,29,99,92,203,197,105,104,159,28,169,192,92,86,96,216,207,
176,204,207,7,235,225,107,57,134,233,200,30,234,23,239,83,134,169,161,231,217,13,24,35,213,26,77,213,217,211,99,106,181,254,200,177,23,88,254,213,160,147,184,183,113,99,39,70,92,14,244,150,111,196,28,
182,134,152,40,72,15,57,40,222,227,216,215,84,167,15,148,200,138,107,232,92,175,97,234,133,126,231,6,141,103,178,118,208,107,100,95,42,48,19,5,100,106,253,70,252,28,179,198,121,204,209,107,77,28,61,126,
93,61,223,226,49,97,55,79,213,52,216,143,94,93,95,147,92,117,98,95,79,95,19,22,223,122,167,223,203,56,227,78,140,184,28,116,81,211,97,71,193,75,228,218,62,77,124,236,149,192,226,106,232,92,175,161,188,
208,75,222,132,131,246,53,173,79,62,21,152,45,5,226,103,183,102,146,241,222,180,166,169,175,169,214,196,215,246,22,167,126,102,204,225,71,129,218,187,248,71,39,254,168,107,117,114,33,9,163,1,246,18,123,
137,194,209,27,249,182,188,159,233,122,61,238,239,57,183,197,189,130,240,244,122,94,139,107,235,106,125,201,165,2,115,65,1,62,207,77,70,59,202,51,232,90,250,230,234,131,235,11,53,245,60,15,243,237,150,
189,88,175,220,247,140,185,247,177,118,156,216,137,17,55,29,50,30,150,28,129,56,184,243,49,166,135,107,176,214,145,152,222,218,30,244,68,100,95,214,130,244,145,247,195,97,251,251,237,151,245,84,160,11,
5,162,241,182,125,142,213,219,86,31,244,126,227,62,228,160,174,161,152,161,188,54,169,11,189,30,115,106,240,194,174,71,39,70,172,131,106,248,129,253,160,206,19,215,144,61,192,90,207,176,156,239,69,44,
212,208,94,12,143,219,56,106,137,169,192,124,80,160,246,185,175,157,11,147,244,154,115,30,55,245,56,239,49,247,224,168,56,78,173,25,180,199,251,184,150,115,196,212,198,141,157,24,177,14,225,7,83,236,147,
186,247,112,112,231,136,29,21,51,227,26,250,124,255,90,47,125,17,89,199,190,181,124,208,154,247,101,156,10,204,117,5,252,28,114,188,194,0,0,13,49,73,68,65,84,27,169,206,194,179,163,152,26,156,155,48,49,
88,235,23,199,160,207,81,53,237,205,164,151,235,129,145,103,93,27,239,107,61,102,205,184,240,205,127,24,111,92,87,233,179,111,237,192,226,224,99,172,237,168,249,214,77,125,253,122,125,63,239,141,177,231,
172,17,199,244,123,201,56,21,152,207,10,240,153,7,253,172,145,83,206,32,6,197,199,126,122,169,69,244,181,212,224,192,126,188,234,12,214,128,226,61,166,111,156,216,201,55,98,14,5,246,59,208,160,125,253,
246,153,111,245,212,101,190,189,163,227,59,143,190,65,30,202,163,246,172,212,184,73,209,104,34,190,17,79,138,24,147,126,31,135,250,195,53,233,239,79,222,95,42,48,93,5,58,249,70,204,205,13,106,36,131,246,
177,239,161,132,169,205,161,244,110,231,89,167,171,64,237,57,169,113,211,221,127,212,235,58,249,70,220,79,128,166,58,188,208,99,137,64,238,130,52,245,245,235,245,253,188,55,198,158,179,70,92,228,253,158,
50,78,5,230,163,2,124,238,65,63,99,228,252,249,32,6,181,46,246,199,189,232,1,125,45,189,112,96,236,117,158,53,17,189,199,227,216,55,142,188,179,111,196,126,48,143,57,148,56,231,137,65,245,17,59,42,102,
242,51,32,229,138,65,95,203,245,232,165,230,253,113,93,236,213,26,231,216,67,216,111,196,117,253,250,179,158,10,116,173,128,62,255,211,25,181,117,206,17,71,228,90,226,227,84,173,214,95,235,99,159,136,
172,135,39,143,168,58,28,189,93,97,103,70,204,33,57,168,208,39,7,118,46,246,178,7,72,61,174,141,166,170,186,27,96,92,199,122,214,145,179,206,251,217,199,57,239,103,77,228,200,219,214,209,147,152,10,76,
170,2,253,62,191,177,238,121,140,201,133,62,117,246,90,238,188,235,227,189,181,216,215,169,78,94,195,38,174,183,104,140,47,157,24,49,226,248,57,34,23,115,122,17,78,57,177,247,18,11,49,82,143,181,14,94,
177,198,176,249,193,85,111,93,7,207,158,228,186,126,142,84,96,190,43,208,246,57,247,90,45,134,19,50,209,139,154,114,106,142,145,247,60,198,228,66,13,246,33,142,200,181,65,213,187,24,157,25,177,14,131,
8,126,72,231,70,17,115,29,253,61,116,253,125,116,13,143,123,68,120,209,117,249,166,27,74,111,74,219,250,218,106,111,218,36,147,84,96,30,40,160,207,123,211,136,53,207,137,29,21,215,114,248,81,162,238,185,
109,191,88,111,58,227,168,249,78,140,248,224,217,15,244,254,84,119,155,8,109,53,126,193,71,191,30,153,175,122,52,64,143,197,201,116,107,198,75,13,100,157,247,122,77,117,31,170,49,124,13,92,98,42,48,151,
21,240,207,119,211,57,98,143,231,196,53,20,199,212,222,122,222,201,35,182,213,98,111,204,107,107,117,189,216,55,197,253,228,129,22,49,198,209,137,17,239,221,187,247,233,242,219,238,79,136,135,29,52,119,
19,142,177,107,131,9,171,135,223,206,228,223,138,107,6,172,123,16,223,132,218,159,26,215,82,174,209,102,182,244,176,38,49,21,152,175,10,212,62,235,145,35,111,67,213,106,19,243,4,213,163,216,39,235,188,
7,110,80,212,251,163,94,112,223,190,125,79,247,146,14,94,58,49,226,61,123,246,60,82,126,201,242,64,70,44,33,101,112,24,41,230,41,129,168,129,53,129,85,99,45,200,30,77,168,125,84,171,161,222,3,106,188,
31,202,53,212,223,111,208,219,175,47,235,169,192,164,42,48,200,231,156,123,143,189,158,19,215,80,28,83,123,233,57,38,23,146,131,94,35,142,53,229,113,210,235,24,215,81,43,190,245,40,231,26,55,118,98,196,
187,119,239,126,124,249,242,229,111,24,41,7,149,0,50,75,114,71,213,48,64,241,228,142,53,113,48,95,199,90,159,246,212,254,76,114,71,173,227,30,216,35,230,240,244,122,174,88,251,229,72,5,230,155,2,109,159,
235,88,243,156,216,81,113,45,23,167,231,157,58,177,176,54,233,167,198,186,26,178,151,215,34,87,140,248,241,174,222,183,78,140,184,28,232,137,120,96,12,80,135,39,118,196,72,157,163,87,216,54,180,70,195,
209,247,33,214,61,17,199,254,222,6,182,7,117,173,169,13,237,211,84,171,245,39,151,10,204,117,5,218,62,239,94,171,197,112,194,24,195,129,24,164,163,226,182,92,107,233,169,97,220,155,156,117,194,93,187,
118,61,217,213,123,212,137,17,191,246,218,107,79,114,80,137,130,105,193,213,208,251,136,37,138,214,50,216,71,117,31,244,56,42,142,83,107,224,136,29,219,98,213,124,112,45,231,50,78,5,230,179,2,122,110,
155,134,215,106,49,156,48,198,112,66,61,219,228,196,66,143,233,139,60,61,172,119,244,94,214,59,42,46,95,32,231,151,17,191,240,194,11,119,172,89,179,230,77,6,44,33,48,193,126,168,55,155,30,173,99,96,126,
124,123,142,60,117,214,58,170,183,150,55,237,17,121,114,71,174,231,92,198,169,192,124,84,64,70,213,52,98,141,28,212,58,98,33,19,158,92,232,102,74,44,140,49,156,35,235,157,35,230,26,158,43,118,254,249,
231,159,191,163,233,140,163,230,59,249,70,252,200,35,143,220,121,246,217,103,239,47,7,93,40,179,210,129,133,126,104,55,197,24,235,208,226,154,134,246,145,25,107,120,204,53,132,212,227,222,236,219,132,
92,147,122,83,14,63,8,198,189,6,89,147,61,169,64,23,10,232,249,153,238,136,107,155,114,120,71,197,76,93,223,77,17,30,78,200,84,109,255,254,253,111,228,240,131,162,214,211,27,174,179,255,169,167,158,186,
125,186,90,12,187,174,19,35,190,229,150,91,126,252,190,247,189,239,79,142,58,234,168,171,48,33,161,79,4,113,142,88,135,82,236,216,75,166,94,48,89,237,225,177,214,40,23,170,198,126,32,251,41,39,118,140,
113,45,23,199,96,31,242,196,84,96,190,42,160,231,169,105,196,154,231,196,142,49,86,238,83,70,73,238,166,73,28,17,99,6,99,61,230,218,219,57,229,47,191,252,242,159,202,183,154,206,56,106,190,19,35,214,77,
63,251,236,179,127,116,228,145,71,94,165,3,203,176,64,76,17,19,243,156,88,235,169,43,150,80,62,120,147,220,132,219,12,152,125,217,179,31,234,90,244,248,117,219,248,216,151,121,42,48,95,21,136,207,35,231,
116,158,184,13,85,107,154,242,11,106,152,166,114,98,97,205,120,107,156,175,33,142,123,255,240,135,63,252,42,231,232,2,59,51,226,39,158,120,226,134,147,79,62,185,119,38,140,80,34,16,99,116,158,195,53,9,
129,120,110,192,226,180,14,84,140,41,179,183,246,35,22,146,215,176,87,44,47,244,53,229,240,77,24,215,55,245,37,159,10,76,138,2,122,134,134,25,177,191,41,135,119,84,92,203,229,17,212,136,29,21,215,230,
160,6,28,215,114,173,226,87,55,14,115,246,153,246,118,102,196,91,183,110,189,239,156,115,206,185,118,197,138,21,91,116,120,140,73,72,172,195,144,59,231,135,68,40,222,52,213,224,100,184,138,65,237,225,
113,220,155,92,123,112,189,136,94,83,204,160,143,124,16,156,206,154,65,246,205,158,84,96,212,10,248,243,53,232,222,181,53,206,17,215,208,57,197,62,229,23,228,196,24,168,120,98,161,27,48,177,144,216,123,
155,226,151,94,122,233,186,111,124,227,27,247,14,122,238,81,244,117,102,196,186,217,251,238,187,239,179,23,92,112,193,22,197,152,146,48,198,228,234,243,33,209,227,16,167,126,222,40,98,97,147,9,179,191,
208,99,237,29,115,231,252,218,244,57,215,20,15,211,219,180,71,242,169,192,108,40,80,123,230,154,238,163,214,235,28,113,13,157,83,28,167,76,83,92,13,107,134,138,241,130,181,158,38,238,129,7,30,248,108,
211,25,199,197,119,106,196,215,94,123,237,29,103,158,121,230,159,150,255,104,247,126,14,228,38,229,49,117,144,55,74,121,124,147,180,14,142,88,40,161,101,198,138,125,106,15,242,24,147,59,42,214,208,154,
218,104,226,107,189,201,165,2,243,73,1,127,46,253,92,145,247,156,88,216,22,171,166,103,152,62,207,49,81,56,114,208,13,152,88,200,164,47,162,254,35,221,117,215,93,215,217,159,150,64,179,78,141,88,23,189,
255,254,251,63,179,97,195,134,158,17,187,129,53,197,90,35,177,25,254,166,16,243,205,87,185,246,1,99,204,53,132,76,237,235,124,45,231,218,244,145,59,182,213,188,47,227,84,96,190,40,160,231,172,105,196,
26,121,27,170,230,117,114,153,101,140,225,162,145,198,220,77,216,107,240,206,41,254,203,191,252,203,207,52,157,105,156,124,231,70,252,213,175,126,245,166,242,151,59,254,245,202,149,43,255,54,230,5,250,
65,227,27,162,26,111,70,68,9,136,25,107,47,114,98,97,156,218,15,142,184,134,206,41,102,104,237,76,199,40,246,152,233,61,228,250,84,128,103,109,38,74,52,237,225,60,113,13,157,83,92,155,122,174,197,215,
80,92,109,98,184,17,213,43,14,94,249,115,207,61,247,59,242,167,153,232,48,221,181,157,27,177,110,244,166,155,110,250,223,62,252,225,15,255,244,178,101,203,206,26,230,198,107,111,142,56,25,26,111,16,63,
138,144,176,196,24,46,168,107,198,24,206,49,198,181,92,28,67,123,230,72,5,14,5,5,244,188,53,141,88,243,156,184,134,206,41,246,169,231,153,156,216,81,113,109,186,209,18,71,3,86,94,126,13,195,61,229,207,
13,255,221,166,51,141,155,159,21,35,222,177,99,199,43,171,87,175,254,185,11,47,188,112,71,57,224,226,120,72,127,67,84,227,13,136,40,225,101,182,154,196,160,76,81,49,134,27,81,251,194,17,59,182,197,170,
49,180,71,142,84,224,80,86,128,231,53,106,224,124,45,134,19,198,24,46,162,158,105,113,142,49,86,206,196,124,149,19,87,112,239,119,190,243,157,79,200,151,226,25,186,202,103,197,136,117,56,253,241,144,99,
143,61,246,111,172,91,183,238,247,252,176,188,33,226,136,227,155,65,46,19,84,44,145,101,198,138,197,41,23,214,190,17,107,95,213,52,107,113,143,156,234,169,197,109,28,181,26,114,189,90,45,185,84,96,210,
20,208,179,52,236,168,173,113,174,22,139,131,175,197,112,122,166,137,133,228,194,24,195,129,24,175,114,98,199,242,115,225,95,188,241,198,27,191,63,236,121,71,217,63,235,95,231,126,225,23,126,225,175,22,
51,254,47,11,23,46,92,32,227,44,248,198,183,220,90,142,185,10,153,209,116,233,193,112,35,74,64,56,226,26,58,167,152,161,181,253,198,32,61,253,246,200,122,42,48,73,10,200,0,251,141,90,143,115,196,53,116,
78,113,109,98,186,170,17,99,184,112,228,142,24,175,56,226,242,47,112,28,40,38,252,169,47,124,225,11,255,181,223,185,198,93,239,239,40,227,190,131,178,255,39,62,241,137,15,174,95,191,254,186,69,139,22,
45,198,92,163,33,195,99,178,228,50,60,231,48,88,144,26,198,8,175,99,197,24,78,168,193,154,131,217,91,115,248,90,175,215,50,78,5,230,163,2,50,190,166,17,107,158,19,215,80,28,83,123,19,203,64,137,133,228,
142,49,86,206,196,124,201,139,9,239,45,127,175,225,234,47,125,233,75,223,104,58,67,151,252,68,24,177,14,188,101,203,150,75,202,223,188,251,106,249,39,149,142,147,121,54,25,177,204,17,19,118,147,37,6,49,
217,26,234,122,240,196,66,13,241,142,189,196,120,114,144,126,242,196,84,224,80,83,64,198,88,27,145,39,7,181,70,49,185,35,124,13,49,92,213,136,49,88,56,114,71,204,184,252,27,154,207,222,125,247,221,31,
43,127,175,225,207,106,247,61,27,220,196,24,177,14,191,113,227,198,21,229,63,224,253,131,227,142,59,238,215,221,108,137,101,122,196,24,110,68,12,22,84,93,131,92,232,57,113,13,123,141,229,133,53,228,222,
235,220,116,226,218,222,211,217,39,215,164,2,211,85,64,230,53,138,81,219,39,114,228,109,168,154,79,221,27,134,235,60,156,99,140,149,251,44,191,124,236,183,239,184,227,142,207,110,223,190,253,197,81,156,
121,84,123,76,148,17,115,168,205,155,55,159,87,190,29,255,219,242,219,218,46,118,227,141,166,75,13,147,165,78,94,67,93,3,158,216,177,45,142,53,229,113,104,239,28,169,192,124,86,0,19,109,58,99,172,55,229,
240,142,49,86,94,155,24,174,106,196,24,46,28,185,240,149,87,94,185,173,124,11,254,91,229,119,222,232,79,106,77,220,152,100,215,56,188,252,184,98,195,218,181,107,127,190,252,233,138,191,89,126,100,177,
18,163,109,66,153,32,53,204,22,148,242,196,66,242,26,58,215,107,44,47,172,33,175,245,120,45,227,84,224,80,81,64,198,215,52,188,86,139,225,28,21,215,114,120,140,87,57,113,13,203,143,32,158,43,191,206,242,
223,63,246,216,99,191,95,126,12,241,221,114,143,63,249,231,125,154,110,120,150,248,73,54,226,55,36,217,180,105,211,162,119,188,227,29,23,175,90,181,234,226,242,151,64,78,62,226,136,35,78,43,184,110,201,
146,37,167,21,227,93,18,205,55,230,218,104,80,19,118,195,245,152,61,222,184,169,134,32,174,105,104,75,58,21,152,179,10,96,146,109,7,136,61,158,19,183,161,106,76,93,135,24,195,245,188,112,229,159,151,219,
243,80,249,199,62,31,44,127,49,67,248,200,206,157,59,111,43,255,212,209,109,219,182,109,219,215,118,159,89,75,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,
84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,
5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,
32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,
82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,84,32,21,72,5,82,129,121,166,192,127,7,216,46,47,85,96,184,39,162,0,0,0,0,73,69,78,68,174,
66,96,130,0,0};

const char* PlayerBackgroundComponent::menu_activated_icon_png2 = (const char*) resource_PlayerBackgroundComponent_menu_activated_icon_png2;
const int PlayerBackgroundComponent::menu_activated_icon_png2Size = 20814;


//[EndFile] You can add extra defines here...
//[/EndFile]

