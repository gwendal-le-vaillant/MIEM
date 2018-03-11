/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.2.1

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include <cmath>

#include "SettingsManager.h"
#include "AudioDefines.h"
#include "InterpolationTypes.h"

#include "XmlUtils.h"
//[/Headers]

#include "HardwareConfigurationComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
using namespace Miam;
//[/MiscUserDefs]

//==============================================================================
HardwareConfigurationComponent::HardwareConfigurationComponent ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (inputsCountSlider = new Slider ("Inputs Count slider"));
    inputsCountSlider->setRange (1, 1024, 1);
    inputsCountSlider->setSliderStyle (Slider::IncDecButtons);
    inputsCountSlider->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    inputsCountSlider->setColour (Slider::textBoxTextColourId, Colours::black);
    inputsCountSlider->addListener (this);

    addAndMakeVisible (outputsCountSlider = new Slider ("Outputs Count slider"));
    outputsCountSlider->setRange (1, 1024, 1);
    outputsCountSlider->setSliderStyle (Slider::IncDecButtons);
    outputsCountSlider->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    outputsCountSlider->setColour (Slider::textBoxTextColourId, Colours::black);
    outputsCountSlider->addListener (this);

    addAndMakeVisible (inputsCountLabel = new Label ("Inputs Count label",
                                                     TRANS("Number of input channels:")));
    inputsCountLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    inputsCountLabel->setJustificationType (Justification::centredRight);
    inputsCountLabel->setEditable (false, false, false);
    inputsCountLabel->setColour (Label::textColourId, Colours::black);
    inputsCountLabel->setColour (TextEditor::textColourId, Colours::black);
    inputsCountLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (outputsCountLabel = new Label ("outputs Count label",
                                                      TRANS("Number of output channels:")));
    outputsCountLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    outputsCountLabel->setJustificationType (Justification::centredRight);
    outputsCountLabel->setEditable (false, false, false);
    outputsCountLabel->setColour (Label::textColourId, Colours::black);
    outputsCountLabel->setColour (TextEditor::textColourId, Colours::black);
    outputsCountLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (oscPluginToggleButton = new ToggleButton ("OSC Plugin toggle button"));
    oscPluginToggleButton->setButtonText (TRANS("Send OSC to Miam Matrix Router remote plug-in"));
    oscPluginToggleButton->setToggleState (true, dontSendNotification);
    oscPluginToggleButton->setColour (ToggleButton::textColourId, Colours::black);

    addAndMakeVisible (udpPortLabel = new Label ("UPD Port Label",
                                                 TRANS("Plug-in listening on UDP port:")));
    udpPortLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    udpPortLabel->setJustificationType (Justification::centredRight);
    udpPortLabel->setEditable (false, false, false);
    udpPortLabel->setColour (Label::textColourId, Colours::black);
    udpPortLabel->setColour (TextEditor::textColourId, Colours::black);
    udpPortLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (udpPortTextEditor = new TextEditor ("UDP Port Text Editor"));
    udpPortTextEditor->setMultiLine (false);
    udpPortTextEditor->setReturnKeyStartsNewLine (false);
    udpPortTextEditor->setReadOnly (false);
    udpPortTextEditor->setScrollbarsShown (true);
    udpPortTextEditor->setCaretVisible (true);
    udpPortTextEditor->setPopupMenuEnabled (true);
    udpPortTextEditor->setText (TRANS("8001"));

    addAndMakeVisible (keyboardToggleButton = new ToggleButton ("Keyboard toggle button"));
    keyboardToggleButton->setButtonText (TRANS("Edit routing matrices from keyboard"));
    keyboardToggleButton->addListener (this);
    keyboardToggleButton->setColour (ToggleButton::textColourId, Colours::black);

    addAndMakeVisible (ipAddressLabel = new Label ("Ip Address Label",
                                                   TRANS("Plug-in host IP address:")));
    ipAddressLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    ipAddressLabel->setJustificationType (Justification::centredRight);
    ipAddressLabel->setEditable (false, false, false);
    ipAddressLabel->setColour (Label::textColourId, Colours::black);
    ipAddressLabel->setColour (TextEditor::textColourId, Colours::black);
    ipAddressLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (ipAddressTextEditor = new TextEditor ("IP Address Text Editor"));
    ipAddressTextEditor->setMultiLine (false);
    ipAddressTextEditor->setReturnKeyStartsNewLine (false);
    ipAddressTextEditor->setReadOnly (false);
    ipAddressTextEditor->setScrollbarsShown (true);
    ipAddressTextEditor->setCaretVisible (true);
    ipAddressTextEditor->setPopupMenuEnabled (true);
    ipAddressTextEditor->setText (TRANS("127.0.0.1"));

    addAndMakeVisible (inputNamesToggleButton = new ToggleButton ("Input Names toggle button"));
    inputNamesToggleButton->setButtonText (TRANS("Display names"));
    inputNamesToggleButton->addListener (this);
    inputNamesToggleButton->setColour (ToggleButton::textColourId, Colours::black);

    addAndMakeVisible (outputNamesToggleButton = new ToggleButton ("Output Names toggle button"));
    outputNamesToggleButton->setButtonText (TRANS("Display names"));
    outputNamesToggleButton->addListener (this);
    outputNamesToggleButton->setColour (ToggleButton::textColourId, Colours::black);

    addAndMakeVisible (interpolationTypeComboBox = new ComboBox ("Interpolation Type combo box"));
    interpolationTypeComboBox->setEditableText (false);
    interpolationTypeComboBox->setJustificationType (Justification::centredLeft);
    interpolationTypeComboBox->setTextWhenNothingSelected (String());
    interpolationTypeComboBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    interpolationTypeComboBox->addListener (this);

    addAndMakeVisible (interpolationTypeLabel = new Label ("Interpolation Type label",
                                                           TRANS("Type of interpolation:")));
    interpolationTypeLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    interpolationTypeLabel->setJustificationType (Justification::centredRight);
    interpolationTypeLabel->setEditable (false, false, false);
    interpolationTypeLabel->setColour (Label::textColourId, Colours::black);
    interpolationTypeLabel->setColour (TextEditor::textColourId, Colours::black);
    interpolationTypeLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));


    //[UserPreSize]

    // Sauvegarde des valeurs SPAT par défaut pour l'instant dans le Projucer
    inputLabelSpatText = inputsCountLabel->getText();
    inputLabelGenericText = TRANS("Number of parameters:");

    // Sliders max values from defines
    inputsCountSlider->setRange (1, Miam_MaxNumInputs, 1);
    outputsCountSlider->setRange (1, Miam_MaxNumOutputs, 1);
    
    // Remplissage de la combobox interpolation, sans choix par défaut
    // Le zéro doit être "aucun interpolateur" pour correspondre aux indices des combobox....
    assert((int)(InterpolationType::None) == 0);
    for (int i=1 ; i < (int)InterpolationType::InterpolationTypesCount ; i++)
        interpolationTypeComboBox->addItem(InterpolationTypes::GetInterpolationName((InterpolationType)i),
                                           i);

    // OSC plugin control is the only choice for now (defaultly activated)
    oscPluginToggleButton->setEnabled(false);

    // listeners
    udpPortTextEditor->addListener(this);
    ipAddressTextEditor->addListener(this);

    //[/UserPreSize]

    setSize (1024, 600);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

HardwareConfigurationComponent::~HardwareConfigurationComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    inputsCountSlider = nullptr;
    outputsCountSlider = nullptr;
    inputsCountLabel = nullptr;
    outputsCountLabel = nullptr;
    oscPluginToggleButton = nullptr;
    udpPortLabel = nullptr;
    udpPortTextEditor = nullptr;
    keyboardToggleButton = nullptr;
    ipAddressLabel = nullptr;
    ipAddressTextEditor = nullptr;
    inputNamesToggleButton = nullptr;
    outputNamesToggleButton = nullptr;
    interpolationTypeComboBox = nullptr;
    interpolationTypeLabel = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void HardwareConfigurationComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xffafafaf));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void HardwareConfigurationComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    inputsCountSlider->setBounds ((getWidth() / 2) + -8 - 150, 16, 150, 24);
    outputsCountSlider->setBounds ((getWidth() / 2) + -8 - 150, 48, 150, 24);
    inputsCountLabel->setBounds ((getWidth() / 2) + -156 - 208, 16 + 0, 208, 24);
    outputsCountLabel->setBounds ((getWidth() / 2) + -156 - 208, 48 + 0, 208, 24);
    oscPluginToggleButton->setBounds ((getWidth() / 2) - (400 / 2), 160, 400, 24);
    udpPortLabel->setBounds ((getWidth() / 2) - 336, 224 + 0, 336, 24);
    udpPortTextEditor->setBounds ((getWidth() / 2) + 8, 224, 64, 24);
    keyboardToggleButton->setBounds ((getWidth() / 2) - (400 / 2), getHeight() - 48, 400, 24);
    ipAddressLabel->setBounds ((getWidth() / 2) - 336, 192 + 0, 336, 24);
    ipAddressTextEditor->setBounds ((getWidth() / 2) + 8, 192, 120, 24);
    inputNamesToggleButton->setBounds ((getWidth() / 2) + 8, 16, 400, 24);
    outputNamesToggleButton->setBounds ((getWidth() / 2) + 8, 48, 400, 24);
    interpolationTypeComboBox->setBounds ((getWidth() / 2) + 8, 104, 320, 24);
    interpolationTypeLabel->setBounds ((getWidth() / 2) - 336, 104 + 0, 336, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void HardwareConfigurationComponent::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == inputsCountSlider)
    {
        //[UserSliderCode_inputsCountSlider] -- add your slider handling code here..
        settingsManager->OnInOutChannelsCountChanged((int) std::round(inputsCountSlider->getValue()), (int) std::round(outputsCountSlider->getValue()));
        //[/UserSliderCode_inputsCountSlider]
    }
    else if (sliderThatWasMoved == outputsCountSlider)
    {
        //[UserSliderCode_outputsCountSlider] -- add your slider handling code here..
        settingsManager->OnInOutChannelsCountChanged((int) std::round(inputsCountSlider->getValue()), (int) std::round(outputsCountSlider->getValue()));
        //[/UserSliderCode_outputsCountSlider]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void HardwareConfigurationComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == keyboardToggleButton)
    {
        //[UserButtonCode_keyboardToggleButton] -- add your button handler code here..
        settingsManager->OnAllowKeyboardEdition(keyboardToggleButton->getToggleState());
        //[/UserButtonCode_keyboardToggleButton]
    }
    else if (buttonThatWasClicked == inputNamesToggleButton)
    {
        //[UserButtonCode_inputNamesToggleButton] -- add your button handler code here..
        settingsManager->OnInOutNamesDisplayedChanged(inputNamesToggleButton->getToggleState(),
                                                      outputNamesToggleButton->getToggleState());
        //[/UserButtonCode_inputNamesToggleButton]
    }
    else if (buttonThatWasClicked == outputNamesToggleButton)
    {
        //[UserButtonCode_outputNamesToggleButton] -- add your button handler code here..
        settingsManager->OnInOutNamesDisplayedChanged(inputNamesToggleButton->getToggleState(),
                                                      outputNamesToggleButton->getToggleState());
        //[/UserButtonCode_outputNamesToggleButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void HardwareConfigurationComponent::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == interpolationTypeComboBox)
    {
        //[UserComboBoxCode_interpolationTypeComboBox] -- add your combo box handling code here..
        int chosenId = interpolationTypeComboBox->getSelectedId();
        if (0 < chosenId && chosenId < (int)InterpolationType::InterpolationTypesCount)
            settingsManager->OnInterpolationTypeChanged((InterpolationType)chosenId);
        //[/UserComboBoxCode_interpolationTypeComboBox]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void HardwareConfigurationComponent::visibilityChanged()
{
    //[UserCode_visibilityChanged] -- Add your code here...

    // Selon le mode : on affiche ou non les paramètres d'E/S
    if (settingsManager)
    {
        switch (settingsManager->GetSessionPurpose())
        {
            case AppPurpose::Spatialisation :
                inputsCountLabel->setText(inputLabelSpatText, NotificationType::sendNotification);
                outputsCountLabel->setVisible(true);
                outputsCountSlider->setVisible(true);
                outputNamesToggleButton->setVisible(true);
                // input name toggle :  coché, mais garde la valeur d'activation précédente
                inputNamesToggleButton->setEnabled(true);
                // type d'interpolateur verrouillé mais affiché
                interpolationTypeLabel->setEnabled(false);
                interpolationTypeComboBox->setSelectedId((int)InterpolationType::Matrix_ConstantVolumeInterpolation, NotificationType::dontSendNotification);
                interpolationTypeComboBox->setEnabled(false);
                break;

            case AppPurpose::GenericController :
                inputsCountLabel->setText(inputLabelGenericText, NotificationType::sendNotification);
                outputsCountLabel->setVisible(false);
                outputsCountSlider->setVisible(false);
                outputNamesToggleButton->setVisible(false);
                // input name toggle : coché mais désactivé
                inputNamesToggleButton->setToggleState(true, dontSendNotification);
                inputNamesToggleButton->setEnabled(false);
                // type d'interpolateur libre
                interpolationTypeLabel->setEnabled(true);
                interpolationTypeComboBox->setEnabled(true);
                break;

            default :
                break;
        }
    }

    //[/UserCode_visibilityChanged]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void HardwareConfigurationComponent::textEditorReturnKeyPressed(TextEditor& editorThatHasChanged)
{
    if (&editorThatHasChanged == ipAddressTextEditor.get())
    {
        // Value stays visible only if correct
        std::string ipAddress = ipAddressTextEditor->getText().toStdString();
        if ( ! XmlUtils::IsIpv4AddressValid(ipAddress))
            ipAddressTextEditor->setText("", NotificationType::dontSendNotification);
        else
            settingsManager->OnIpAddressChanged(ipAddress);
    }
    else if (&editorThatHasChanged == udpPortTextEditor.get())
    {
        bool enteredValueIsCorrect = true;
        int parsedValue = -1;
        // Value stays visible only if correct
        try {
            parsedValue = std::stoi(udpPortTextEditor->getText().toStdString());
        } catch (std::exception) {
            enteredValueIsCorrect = false;
        }
        if (parsedValue <= 0 || 65535 < parsedValue)
            enteredValueIsCorrect = false;
        if ( ! enteredValueIsCorrect)
            udpPortTextEditor->setText("", NotificationType::dontSendNotification);
        else
            settingsManager->OnUdpPortChanged(parsedValue);
    }
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="HardwareConfigurationComponent"
                 componentName="" parentClasses="public Component, public TextEditor::Listener"
                 constructorParams="" variableInitialisers="" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330" fixedSize="0" initialWidth="1024"
                 initialHeight="600">
  <METHODS>
    <METHOD name="visibilityChanged()"/>
  </METHODS>
  <BACKGROUND backgroundColour="ffafafaf"/>
  <SLIDER name="Inputs Count slider" id="77ed5b9e29dce02e" memberName="inputsCountSlider"
          virtualName="" explicitFocusOrder="0" pos="-8Cr 16 150 24" textboxtext="ff000000"
          min="1.00000000000000000000" max="1024.00000000000000000000"
          int="1.00000000000000000000" style="IncDecButtons" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.00000000000000000000"
          needsCallback="1"/>
  <SLIDER name="Outputs Count slider" id="ca10a4b3f2104985" memberName="outputsCountSlider"
          virtualName="" explicitFocusOrder="0" pos="-8Cr 48 150 24" textboxtext="ff000000"
          min="1.00000000000000000000" max="1024.00000000000000000000"
          int="1.00000000000000000000" style="IncDecButtons" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.00000000000000000000"
          needsCallback="1"/>
  <LABEL name="Inputs Count label" id="5edb179087fb7973" memberName="inputsCountLabel"
         virtualName="" explicitFocusOrder="0" pos="-156Cr 0 208 24" posRelativeY="77ed5b9e29dce02e"
         textCol="ff000000" edTextCol="ff000000" edBkgCol="0" labelText="Number of input channels:"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.00000000000000000000" kerning="0.00000000000000000000"
         bold="0" italic="0" justification="34"/>
  <LABEL name="outputs Count label" id="5b4dd76a50f5f0d4" memberName="outputsCountLabel"
         virtualName="" explicitFocusOrder="0" pos="-156Cr 0 208 24" posRelativeY="ca10a4b3f2104985"
         textCol="ff000000" edTextCol="ff000000" edBkgCol="0" labelText="Number of output channels:"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.00000000000000000000" kerning="0.00000000000000000000"
         bold="0" italic="0" justification="34"/>
  <TOGGLEBUTTON name="OSC Plugin toggle button" id="74b5dae6c2ea74a2" memberName="oscPluginToggleButton"
                virtualName="" explicitFocusOrder="0" pos="0Cc 160 400 24" txtcol="ff000000"
                buttonText="Send OSC to Miam Matrix Router remote plug-in" connectedEdges="0"
                needsCallback="0" radioGroupId="0" state="1"/>
  <LABEL name="UPD Port Label" id="8d369e08975b779c" memberName="udpPortLabel"
         virtualName="" explicitFocusOrder="0" pos="0Cr 0 336 24" posRelativeX="dfbb24a51fa3d6c0"
         posRelativeY="e4ef4437203ce19e" textCol="ff000000" edTextCol="ff000000"
         edBkgCol="0" labelText="Plug-in listening on UDP port:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.00000000000000000000" kerning="0.00000000000000000000"
         bold="0" italic="0" justification="34"/>
  <TEXTEDITOR name="UDP Port Text Editor" id="e4ef4437203ce19e" memberName="udpPortTextEditor"
              virtualName="" explicitFocusOrder="0" pos="8C 224 64 24" initialText="8001"
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TOGGLEBUTTON name="Keyboard toggle button" id="8c809b7ecaa8a037" memberName="keyboardToggleButton"
                virtualName="" explicitFocusOrder="0" pos="0Cc 48R 400 24" posRelativeX="dfbb24a51fa3d6c0"
                posRelativeW="dfbb24a51fa3d6c0" txtcol="ff000000" buttonText="Edit routing matrices from keyboard"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="Ip Address Label" id="2066f0f6ef12dcf0" memberName="ipAddressLabel"
         virtualName="" explicitFocusOrder="0" pos="0Cr 0 336 24" posRelativeX="dfbb24a51fa3d6c0"
         posRelativeY="6997b5b4dc28675a" textCol="ff000000" edTextCol="ff000000"
         edBkgCol="0" labelText="Plug-in host IP address:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.00000000000000000000" kerning="0.00000000000000000000"
         bold="0" italic="0" justification="34"/>
  <TEXTEDITOR name="IP Address Text Editor" id="6997b5b4dc28675a" memberName="ipAddressTextEditor"
              virtualName="" explicitFocusOrder="0" pos="8C 192 120 24" initialText="127.0.0.1"
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TOGGLEBUTTON name="Input Names toggle button" id="b7d5b33136328768" memberName="inputNamesToggleButton"
                virtualName="" explicitFocusOrder="0" pos="8C 16 400 24" txtcol="ff000000"
                buttonText="Display names" connectedEdges="0" needsCallback="1"
                radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="Output Names toggle button" id="ba9f950d6ef902d9" memberName="outputNamesToggleButton"
                virtualName="" explicitFocusOrder="0" pos="8C 48 400 24" txtcol="ff000000"
                buttonText="Display names" connectedEdges="0" needsCallback="1"
                radioGroupId="0" state="0"/>
  <COMBOBOX name="Interpolation Type combo box" id="900a9258dc365ad0" memberName="interpolationTypeComboBox"
            virtualName="" explicitFocusOrder="0" pos="8C 104 320 24" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <LABEL name="Interpolation Type label" id="184dbfa5837bdd41" memberName="interpolationTypeLabel"
         virtualName="" explicitFocusOrder="0" pos="0Cr 0 336 24" posRelativeX="dfbb24a51fa3d6c0"
         posRelativeY="900a9258dc365ad0" textCol="ff000000" edTextCol="ff000000"
         edBkgCol="0" labelText="Type of interpolation:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.00000000000000000000" kerning="0.00000000000000000000"
         bold="0" italic="0" justification="34"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
