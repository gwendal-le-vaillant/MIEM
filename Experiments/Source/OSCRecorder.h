/*
  ==============================================================================

    OSCRecorder.h
    Created: 7 Feb 2019 9:09:19pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include <vector>
#include <chrono>
#include <memory>
#include <string>

#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/xml_parser.hpp"
namespace bptree = boost::property_tree;

#include "OSCListenerForwarder.h" // MiemClock

#include "ExperimentState.h"
#include "MiemExpePreset.h"

#include "UserQuestionsManager.h"
#include "OSCRecorderConnection.h"

#include "ReaperOscController.h"

class OSCRecorderComponent;
class OSCListenerForwarder;
class MainComponent;

class OSCRecorderTimer; // defined at end of file


class OSCRecorder : public UserQuestionsManager,
                    public juce::Timer // timer for OSC data retrieving, and forced graphical updates
{
    
    // ===================================================================
    // ===================================================================
    // ================== PARAMETERS OF THE EXPERIMENT ===================
    // ===================================================================
    // ===================================================================
    public :
    
    static std::string GetLanguage() {return std::string("FR");}
    
    const int ExperimentPresetsCount = 20; // 2 presets per synth
    
    const int TrialSynthsCount = 2;
    const int TrialPresetsCount = TrialSynthsCount; // 2 different synths
    
    static const int ReaperWhiteNoiseTrackNumber = 1;
    static const int ReaperWhiteNoiseTrackBpm = 110;
    
#ifdef __MIEM_SHORT_DELAYS
    static const int delayAfterFinished_ms = 2000;
    static const int ListenAutoTriggerDelay_s = 3;
    static const int SearchAutoTriggerDelay_s = 1;
    const int listeningTime_ms = 1000;
#else
    static const int delayAfterFinished_ms = 8000;
    static const int ListenAutoTriggerDelay_s = 9;
    static const int SearchAutoTriggerDelay_s = 5;
    const int listeningTime_ms = 20000; // 15s sont un peu trop courtes...
#endif
    
    // 30s = valeur qui permet d'imposer un rythme,
    // sans couper trop souvent la recherche...
    // 35s = légèrement plus long, moins de coupure sans faire
    // baisser sensiblement les performances
    // 40s = toujours des coupures ?
    // 60s = beaucoup trop long
    static const int ResearchTimeMax_ms = 40000;
    
    static const int WhiteNoiseStartDelay_ms = 1500; ///< Delay after entering "finished" state
    static const int WhiteNoisePrematureEnd_ms = 500; ///< White noise ends a bit before the "finished" state actually ends
    
    // - - - SHORT/DEMO EXPERIMENT - - -
    const bool ShortExperiment = false;
    const int ShortExperimentPresetsCount = 2;
    private :
    const bool randomizePresets = true;
    public :
    // - - - SHORT/DEMO EXPERIMENT - - -
    
    const int tcpTimeOut_ms = 3000; // default Juce value : 3000 ms
    
    // Éviter stockage dans un drive (iCloud, Google, etc...) au cas où
    // le drive commence à modifier les fichiers d'expé (on copiera juste tout le dossier
    // à chaque fois... dans un drive si + pratique)
    // Chemins relatifs par rapport au dossier de build (Debug ou Release, même chose)
    const char filesSavingPath[1024] = "../../../../ExperimentsDataProcessing/";
    
    
    
    
    const char experimentGeneralDataFile[1024] = "A_OSC_Recorder_Experiment.xml";
    
    
    // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
    private :
    
    // - - - CONFIGURATION to be read from the general data file - - -
    std::string tcpServerName;
    int tcpServerPort = -1;
    int udpOscPort = -1;
    int currentExperimentUid = -1;
    // this tree contains the values, ready to be XML-re-written
    std::shared_ptr<bptree::ptree> parametersTree;
    
    // links to view
    MainComponent* const mainComponent;
    
    // sub-modules, init at construction only
    OSCRecorderComponent& recorderComponent;
    std::shared_ptr<OSCRecorderConnection> tcpConnection;
    std::shared_ptr<OSCListenerForwarder> oscRealtimeListener;
    std::shared_ptr<ReaperOscController> reaperController;
    
    bool hasConnectionBeenLostDuringLastStep = false;
    ExperimentState stateBeforeConnectionLost = ExperimentState::NotInitialized;
    
    ExperimentState state = ExperimentState::NotInitialized;
    
    /// \brief Index qui représente l'étape de l'expérience à laquelle on est rendu
    ///
    /// Les indexes -1 et -2 correspondent au "tours d'essais" qui ne seront pas comptabilisés
    int currentPresetStep = -(int)TrialPresetsCount - 1;
    /// \brief sorted by index (NOT randomized). Last presets in the list are trial presets
    std::vector<std::shared_ptr<MiemExpePreset>> presets;
    /// \brief tranforms a currentPresetIndex (might be negative for trial presets)
    /// to a random preset index from the vector of all presets
    /// (not randomized for the trial presets)
    std::map<int, size_t> presetRandomIdx;
    
    // Time-related data
    MiemClock::time_point startTimePt;
    juce::Time startSystemTime;
    std::unique_ptr<OSCRecorderTimer> researchDurationTimer;
    MiemClock::time_point currentResearchStartTimePt;
    
    // XML and data files
    std::string dataFilePath; // CSV file
    std::string infoFilePath; // XML file
    
    
    
    // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
    
    
    
    
    // = = = = = = = = = = METHODS = = = = = = = = = =
    public :
    OSCRecorder(MainComponent* _mainComponent);
    virtual ~OSCRecorder() {}
    
    void BeginExperiment();
    
    // Internal management
    protected :
    void reinitExperiment();
    void tryConnectSocketAndContinueExpe();
    
    /// \brief Triggers the interactive experiment of the next preset, or the next big step
    /// of the experiment
    ///
    /// might be called at startup or after a deconnection
    void nextExperimentStep();
    /// \brief Goes to the next preset
    ///
    /// Manages if the next preset does not exist (then, switches to the final questions)
    void nextPreset();
    
    void startRecording();
    void stopRecording();
    
    
    /// \brief Main State-Changing (mode managing) function
    void changeState(ExperimentState newState);
    
    
    // OSC control of Reaper and MIEM Controller
    protected :
    /// \brief Selects the scene displayed on Miem Controller app.
    void selectNewMiemScene(bool selectEmptyScene = false);
    
    
    
    // - - - - - Periodic updates - - - - -
    protected :
    virtual void timerCallback() override;
    public :
    void AttributeTimerCallback(juce::Timer* timer);
    
    
    public :
    
    // - - - - - Events froms View - - - - -
    void OnButtonClicked(ExperimentState requestedState);
    
    // overrides from the UserQuestionsManager parent abstract class
    virtual void OnIntroFinished(OSCRecorderIntroComponent* sender) override;
    virtual void OnFirstQuestionsAnswered(UserQuestions* sender) override;
    virtual void OnFinalQuestionsAnswered(UserFinalQuestions* sender) override;
    
    /// \brief Returns whether it is possible to quit now, or not
    bool OnQuitRequest();
    
    // - - - - - Events to View - - - - -
    protected :
    void displayError(String errorMsg);
    void computeAndDisplayTotalScore(); ///< Simple sum of all scores (trials excluded)

    
    // - - - - - Events froms network - - - - -
    public :
    void OnConnectionLost();
    
    
    // - - - - - Events to MIEM Controller via network - - - - -
    protected :
    bool selectMiemControllerScene(int sceneIndex);
    
    
    
    // - - - - - XML and DATA Save and Read - - - - -
    protected :
    /// \brief allows to check whether the files can be created or not. If not,
    /// we do not event start the experiment
    void createNewDataFiles();
    /// \brief The path is a const char already defined, and the names
    /// will be built automatically from the current date and time
    void saveEverythingToFiles(int lastPresetStepToSave);
    /// \brief Gets the UID, and... other parameters
    void getExperimentParametersFromXmlFile();
    

    
    
};




class OSCRecorderTimer : public juce::Timer {
    OSCRecorder* oscRecorder = 0;
    
    public :
    OSCRecorderTimer(OSCRecorder* _oscRecorder)
    { oscRecorder = _oscRecorder; }
    virtual ~OSCRecorderTimer() {}
    virtual void timerCallback() override
    { oscRecorder->AttributeTimerCallback(this); }
};


