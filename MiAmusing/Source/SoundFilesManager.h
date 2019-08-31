/* 
 * This file is part of the MIEM distribution (https://github.com/gwendal-le-vaillant/MIEM).
 * Copyright (c) 2017 Guillaume Villée.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SoundFileViewer.h"
#include "MainComponent.h"

class SoundFileViewer;
class MainContentComponent;

//==============================================================================
/*
*/
class SoundFilesManager    : public Component, public Button::Listener
{
public:
    SoundFilesManager();
	SoundFilesManager(const int Nsamples, Colour colorCode[]);
    ~SoundFilesManager();

    void paint (Graphics&) override;
    void resized() override;

	void buttonClicked(Button* buttonThatWasClicked) override;

	void completeInitialisation(MainContentComponent *m_mainComponent);
	void setDefaultPath(String m_defaultPath);
	void setSoundPath(int idx, String _path);
	String getDefaultPath();
	void release();

	void loadSoundFile(SoundFileViewer* SoundFileViewer);
	void setCurrentSoundFilePath(String m_soundPath);
	void addSoundFileViewer();

	std::shared_ptr<bptree::ptree> GetSoundTree();

	void SetSoundTree(bptree::ptree tree);

private:
	int Nmax = 7;
	int itemHeight, spaceHeight;
	String defaultPath;
	int sampleToSet;
	ScopedPointer<TextButton> closeSoundFileManagerButton;
	ScopedPointer<TextButton> addSoundFileViewerButton;
	OwnedArray<SoundFileViewer> soundFileViewerArray;

	MainContentComponent* mainComponent;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoundFilesManager)
};
