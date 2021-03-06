/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.5

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"

#include <vector>
#include <memory>

#include "SceneCanvasComponent.h"
#include "MultiSceneCanvasComponent.h"
#include "MultiCanvasComponent.h"
#include "MiamLookAndFeel.h"

#include "DrawableArea.h"

#include "ControlState.hpp"

namespace Miam
{
    // forward declarations
    class GraphicSessionManager;

//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]

 // \brief An important component that corresponds to an Miam::AppMode of the
 // Miam::Presenter.
 //
 // An auto-generated component, created by the Projucer.
 //

                                                                    //[/Comments]
*/
class SceneEditionComponent  : public Component,
                               public TextEditor::Listener,
                               public Button::Listener,
                               public ComboBox::Listener
{
public:
    //==============================================================================
    SceneEditionComponent ();
    ~SceneEditionComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.

    // ----- Construction helpers -----

    /// \brief Links this module to parent and/or managing classes
    void CompleteInitialization(GraphicSessionManager* _graphicSessionManager, MultiCanvasComponent* _multiCanvasComponent);

    // ----- General Setters and Getters -----


    //MultiSceneCanvasComponent* AddCanvas();
    //void DeleteCanvas(SceneCanvasComponent::Id canvasId);




    // ----- Functions that obey orders sent from the Presenter -----

    // - - - - - Common - - - - -
    public :
    void SetEnabledAllControls(bool areEnabled, bool controlsBackUp = true);
    void CloseTemporaryDisplayedObjects();
    void PrepareVisible();
    void PrepareUnvisible();
    void ReleaseOpenGLResources();


    // - - - - - Canvases group - - - - -
    void SetCanvasGroupHidden(bool _isHidden);
    void SetCanvasGroupReduced(bool _isReduced);
    void SetDeleteSceneButtonEnabled(bool isEnabled);
    // - - - - - Area group - - - - -
    void SetAreaGroupHidden(bool _isHidden);
    void SetAreaGroupReduced(bool _isReduced);
    void SetEnabledAreaEditionControls(bool areEnabled);
    void SetEnabledAreaMainControls(bool areEnabled);
    void SetVisibleAreaEditionControls(bool areVisible);
    void DisableAllButtonsBut(juce::String onlyActiveButtonName);
    void SetPasteEnabled(bool _isEnabled); // button with back-up function
    // - - - - - Spat group - - - - -
    void SetSpatGroupHidden(bool _isHidden);
    void SetSpatGroupReduced(bool _isReduced);
    void UpdateStatesList(std::vector< std::shared_ptr<ControlState<double>> > &newSpatStates);
    void SelectSpatState(int index, NotificationType notificationType = NotificationType::dontSendNotification);
    // - - - - - Initial Scene State group - - - - -
    void SetInitialStateGroupHidden(bool _isHidden);
    void SetInitialStateGroupReduced(bool _isReduced);
    void SetDeleteExciterButtonEnabled(bool _isEnabled);
    void SetExciterConstraintButtonEnabled(bool _isEnabled);
    // - - - - - Info group - - - - -
    void SetInfoGroupReduced(bool _isReduced);
    void SetInfoHelpText(const juce::String& infoHelpText);


    // ----- Other setters and getters -----
    // - - - - - Text Values - - - - -
    void SetSceneName(std::string _name);
    void SetCanvasInfo(SceneCanvasComponent::Id _id);


    // Helpers
    private :
    void setVisibleAllAreaControls(bool areVisible);
    void setEnabledSelectedAreaControls(bool areEnabled);
    void setVisibleSpatControls(bool areVisible);

    // Translation avec sauvegarde en interne de ce qui vient d'être effectué
    void areaGroupTranslateY(int dY);
    void spatGroupTranslateY(int dY);
    void initialStateGroupTranslateY(int dY);
    void componentTranslateY(Component* component, int dY);


    public :
    virtual void textEditorTextChanged(TextEditor& _editor) override;



    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    void visibilityChanged() override;
    bool keyPressed (const KeyPress& key) override;
    void modifierKeysChanged (const ModifierKeys& modifiers) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.

    // À DÉGAGER, ces objets n'appartiennent au SceneEditionComponent...
    //std::vector<MultiSceneCanvasComponent*> multiSceneCanvasComponents;
    MultiCanvasComponent* multiCanvasComponent = 0;

    GraphicSessionManager* graphicSessionManager = 0;

    std::unique_ptr<MiamLookAndFeel> transparentLookAndFeel;

    int canvasGroupReducedH,
        areaGroupReducedH,
        spatGroupReducedH,
        initialStateGroupReducedH,
        infoGroupReducedH ;
    int areaGroupCurrentDy = 0,
        spatGroupCurrentDy = 0,
        initialStateGroupCurrentDy = 0,
        infoGroupCurrentDy = 0;
    bool isCanvasGroupHidden = false,
        isAreaGroupReduced = false,
        isAreaGroupHidden = false,
        isSpatGroupHidden = false,
        isInitialStateGroupReduced = false,
        isInitialStateGroupHidden = false,
        isInfoGroupReduced = true; // very simple -> self-managed

    // When all buttons are disabled, to highlight that we can do only 1 action,
    // we need to back up the state of some buttons
    bool pasteTextButtonEnabledBackUp = false;


    // ====================== Keyboard SHORTCUTS =====================
    // codé vite et mal... à refactorer dans les boutons directement en les faisant
    // hériter d'une classe gestionnaire des raccourcis...
    // all working with Cmd (Ctrl) at the moment
    const char editExcitersCmdKey = 'E';
    const char freeExcitersMoveCmdKey = 'F';
    const char copyCmdKey = 'C';
    const char pasteCmdKey = 'V';
    const char newAreaCmdKey = 'N';
	const char newExciterCmdKey = 'N';
    const int deleteAreaCmdKey = juce::KeyPress::deleteKey;
	const int deleteExciterCmdKey = juce::KeyPress::deleteKey;
    const int areaForwardCmdKey = juce::KeyPress::upKey;
    const int areaBackwardCmdKey = juce::KeyPress::downKey;
    const int addPointCmdKey = 'I';
    // working with Cmd and Shift
    const int deletePointCmdShiftKey = 'I';
    const int areaToFrontCmdShiftKey = juce::KeyPress::upKey;
    const int areaToBackCmdShiftKey = juce::KeyPress::downKey;
    const char newSceneCmdShiftKey = 'N';
    const int deleteSceneCmdShiftKey = juce::KeyPress::deleteKey;
    const int sceneLeftCmdShiftKey = juce::KeyPress::leftKey;
    const int sceneRightCmdShiftKey = juce::KeyPress::rightKey;

    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<GroupComponent> infoGroupComponent;
    std::unique_ptr<TextEditor> infoTextEditor;
    std::unique_ptr<GroupComponent> areaGroupComponent;
    std::unique_ptr<GroupComponent> controlGroupComponent;
    std::unique_ptr<TextButton> addPointTextButton;
    std::unique_ptr<TextButton> deletePointTextButton;
    std::unique_ptr<TextButton> copyTextButton;
    std::unique_ptr<TextButton> pasteTextButton;
    std::unique_ptr<TextButton> addAreaTextButton;
    std::unique_ptr<TextButton> deleteAreaTextButton;
    std::unique_ptr<TextButton> sendBackwardTextButton;
    std::unique_ptr<TextButton> bringForwardTextButton;
    std::unique_ptr<TextButton> bringToFrontTextButton;
    std::unique_ptr<TextButton> sendToBackTextButton;
    std::unique_ptr<GroupComponent> canvasGroupComponent;
    std::unique_ptr<ComboBox> spatStatesComboBox;
    std::unique_ptr<Label> controlStateLabel;
    std::unique_ptr<TextButton> addSceneTextButton;
    std::unique_ptr<TextButton> deleteSceneTextButton;
    std::unique_ptr<TextButton> sceneLeftTextButton;
    std::unique_ptr<TextButton> sceneRightTextButton;
    std::unique_ptr<Label> canvasInfoLabel;
    std::unique_ptr<GroupComponent> initialStateGroupComponent;
    std::unique_ptr<TextButton> addExciterTextButton;
    std::unique_ptr<TextButton> deleteExciterTextButton;
    std::unique_ptr<Label> sceneNameLabel;
    std::unique_ptr<TextEditor> sceneNameTextEditor;
    std::unique_ptr<ToggleButton> excitersEditionButton;
    std::unique_ptr<TextButton> showInfoTextButton;
    std::unique_ptr<ToggleButton> excitersConstraintButton;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SceneEditionComponent)
};

//[EndFile] You can add extra defines here...
}    // namespace Miam
//[/EndFile]

