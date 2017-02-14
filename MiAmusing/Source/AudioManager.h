/*
  ==============================================================================

    AudioManager.h
    Created: 19 Jan 2017 10:18:34am
    Author:  ayup1

  ==============================================================================
*/

#ifndef AUDIOMANAGER_H_INCLUDED
#define AUDIOMANAGER_H_INCLUDED

#include <thread>
#include <vector>

#include "../JuceLibraryCode/JuceHeader.h"
#include "AmusingModel.h"
#include "AmuSignal.h"
#include "SquareSignal2.h"



// Pre-declaration for pointer members
namespace Amusing {

	class AmusingModel;//AudioManager;

	enum AudioManagerState
	{
		Play,
		Pause,
		Stop
	};


//==============================================================================
/*
*/
	class AudioManager : public AudioAppComponent
	{
	public:
		AudioManager(AmusingModel *m_mode);
		~AudioManager();

		void paint(Graphics&) override;
		void resized() override;

		void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
		void releaseResources() override;
		void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) override;

		void askParameter();
		void chooseAudioType(int position, int type);
		void AncienchooseAudioType(int type,double duration);
	private:
		SquareSignal2 *ondeCarre;
		void trackVectorHandler(bool deactivation, int type);
		void changeState(AudioManagerState nexState);
		void playAllSources();
		void stopAllSources();

		void sendPosition();

		void verifyAllSource();
		void HandleEvent();

		AmusingModel *model;
		MixerAudioSource *mixer;

		//std::thread activationThread;// s[2];

		AudioManagerState state;

		std::vector<int> sourceControled;
		void playAllControledSources();

		int currentSamplesPerBlock;
		double currentSampleRate;

		int useADSR;
		int count;
		double div;

		std::vector<std::shared_ptr<AmuSignal>> trackVector;
		std::vector<bool> activeVector;
		const int Nmax = 1024;
		int Nsources;

		bool beginTest;

	};
}

#endif  // AUDIOMANAGER_H_INCLUDED
