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

#include "../JuceLibraryCode/JuceHeader.h"
#include "SoundBrowser.h"

//==============================================================================
SoundBrowser::SoundBrowser() : soundsWildcardFilter("*.wav;*.aiff", "*", "Sound File Filter"),
	directoryThread("Sound File Scanner Thread"),
	soundList(&soundsWildcardFilter,directoryThread),
	fileTree(soundList)
{
	setOpaque(true);
#if __AMUSINGMOBILE
	soundList.setDirectory(File::getSpecialLocation(File::userDocumentsDirectory), true, true);
#elif __AMUSINGIOS
	soundList.setDirectory(File::getSpecialLocation(File::userDocumentsDirectory), true, true);
#elif __AMUSINGOSX
	soundList.setDirectory(File::getSpecialLocation(File::userDocumentsDirectory), true, true);
#else
	soundList.setDirectory(File("C:\\"),true,true);
#endif
	directoryThread.startThread(1);

	fileTree.addListener(this);
	fileTree.setColour(TreeView::backgroundColourId, Colours::grey);
	addAndMakeVisible(fileTree);
	fileTree.setVisible(false);

	binaryDataExplorer = new BinaryDataExplorer();
	addAndMakeVisible(binaryDataExplorer);


	binaryDataExplorerButton = new TextButton();
	binaryDataExplorerButton->setButtonText("BinaryData");
	binaryDataExplorerButton->addListener(this);
	addAndMakeVisible(binaryDataExplorerButton);
	binaryDataExplorerButton->setAlpha(1.0f);

	filesExplorerButton = new TextButton();
	filesExplorerButton->setButtonText("Files");
	addAndMakeVisible(filesExplorerButton);
	filesExplorerButton->setAlpha(0.5f);
	filesExplorerButton->addListener(this);

	closeButton = new TextButton();
	closeButton->setButtonText("Select");
	closeButton->addListener(this);
	addAndMakeVisible(closeButton);

	cancelButton = new TextButton();
	cancelButton->setButtonText("Cancel");
	cancelButton->addListener(this);
	addAndMakeVisible(cancelButton);

	filesExplorerOpen = false;
}

SoundBrowser::~SoundBrowser()
{
	fileTree.removeListener(this);
}

void SoundBrowser::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    //g.setColour (Colours::grey);
    //g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    //g.setColour (Colours::white);
    //g.setFont (14.0f);
    //g.drawText ("SoundBrowser", getLocalBounds(),
    //            Justification::centred, true);   // draw some placeholder text
}

void SoundBrowser::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
	Rectangle<int> r(getLocalBounds());
	r.reduced(4);

	Rectangle<int> buttonsLocation = r.removeFromTop(24);
	binaryDataExplorerButton->setBounds(buttonsLocation.removeFromLeft((buttonsLocation.getWidth()-4)/2));
	buttonsLocation.removeFromLeft(4);
	filesExplorerButton->setBounds(buttonsLocation);

	Rectangle<int> buttonsArea = r.removeFromBottom(24);
	closeButton->setBounds(buttonsArea.removeFromLeft(buttonsArea.getWidth()/2));
	cancelButton->setBounds(buttonsArea);

	r.removeFromBottom(4);
	fileTree.setBounds(r);
	binaryDataExplorer->setBounds(r);
}

void SoundBrowser::buttonClicked(Button* buttonThatWasClicked)
{
	if (buttonThatWasClicked == closeButton)
	{
		if (filesExplorerOpen)
			mainComponent->CloseSoundBrowser(pathToSound);
		else
			mainComponent->CloseSoundBrowser(binaryDataExplorer->getSelectedFileName());
	}
	else if (buttonThatWasClicked == cancelButton)
		mainComponent->CloseSoundBrowser("");
	else if (buttonThatWasClicked == filesExplorerButton)
	{
		filesExplorerButton->setAlpha(1.0f);
		binaryDataExplorerButton->setAlpha(0.5f);
		fileTree.setVisible(true);
		binaryDataExplorer->setVisible(false);
		filesExplorerOpen = true;
	}
	else if (buttonThatWasClicked == binaryDataExplorerButton)
	{
		filesExplorerButton->setAlpha(0.5f);
		binaryDataExplorerButton->setAlpha(1.0f);
		fileTree.setVisible(false);
		binaryDataExplorer->setVisible(true);
		filesExplorerOpen = false;
	}
}

void SoundBrowser::completeInitialisation(MainContentComponent * m_mainComponent)
{
	mainComponent = m_mainComponent;
}

void SoundBrowser::release()
{
	mainComponent = nullptr;
}

String SoundBrowser::getPathToSound()
{
	return pathToSound;
}

void SoundBrowser::selectionChanged()
{
	pathToSound = fileTree.getSelectedFile().getFullPathName();
}
