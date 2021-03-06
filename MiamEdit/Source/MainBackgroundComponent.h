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

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"

// #include "View.h" // génère un pb d'includes croisés

#include "FileMenu.h"
#include "ClearLabelTimer.h"

#include "AppPurpose.h"
#include "AppMode.h"

namespace Miam {

    // forward declarations
    class View;
    class Presenter;

//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]

// \brief The background component with a few UI controls (always displayed).
//
// An auto-generated component, created by the Projucer.
//

                                                                    //[/Comments]
*/
class MainBackgroundComponent  : public Component,
                                 public Button::Listener
{
public:
    //==============================================================================
    MainBackgroundComponent ();
    ~MainBackgroundComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.

    void CompleteInitialization(Presenter* _presenter);


    // SETTERS and GETTERS
    void setMiamView(Miam::View* _miamView) {view = _miamView;}
    /// \brief Returns the keyboard key that must be used with the Cmd (or Ctrl) modifier
    /// to trigger a tab switch (false click on a tab)
    char GetSwitchTabCommandKey() {return switchTabCommandKey;}
    char GetConfigurationTabCommandKey() {return configurationTabCommandKey;}
    char GetSaveCommandKey() {return saveCommandKey;}
    char GetLoadCommandKey() {return loadCommandKey;}


    // Orders from Presenter
    void DisplayInfo(const String& message, int priority = 0);


    // translated orders, from View
    void TriggerTabSwitch(Miam::AppMode currentAppMode);
    void TriggerConfigurationTab();
    void TriggerSave(bool saveAsNewFile = false);
    void TriggerLoad();


    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    void visibilityChanged() override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.

    View *view;
    std::unique_ptr<FileMenu> fileMenu;

    int currentDisplayedInfoPriority;
    Miam::ClearLabelTimer clearLabelTimer;

    // par défaut : texte générique
    // on crée le texte de spat dans le constructeur
    String spatScenesText;
    String genericScenesText;
    String spatStatesText;
    String genericStatesText;


    // ====================== Keyboard SHORTCUTS =====================
    const char switchTabCommandKey = 'T';
    const char configurationTabCommandKey = 'Y';
    const char saveCommandKey = 'S'; // OK..... non-négociable !
    const char loadCommandKey = 'O'; // idem (Open...)


    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<TextButton> speakersTextButton;
    std::unique_ptr<TextButton> statesTextButton;
    std::unique_ptr<Label> mainInfoLabel;
    std::unique_ptr<TextButton> scenesTextButton;
    std::unique_ptr<TextButton> hardwareConfTextButton;
    std::unique_ptr<TextButton> startTextButton;
    std::unique_ptr<TextButton> fileTextButton;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainBackgroundComponent)
};

//[EndFile] You can add extra defines here...

} // namespace Miam
//[/EndFile]

