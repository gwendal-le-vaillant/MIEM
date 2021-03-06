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

#include "View.h"

#include "Presenter.h"

using namespace Miam;
using namespace Amusing;


// Default constructor
View::View(MainContentComponent* _mainContentComponent)
{
    mainContentComponent = _mainContentComponent;
    
    
    mainContentComponent->SetMiamView(this);
}


// Default destructor
View::~View()
{
}

// Called by the Presenter during its own construction, after the View is constructed
// ?? Règle précise pour appeler les "compléments d'initialisation" ??
//
void View::CompleteInitialization(Presenter* _presenter)
{
    presenter = _presenter;
    
    mainContentComponent->CompleteInitialization(presenter);
}
void View::CompleteInitialization(GraphicSessionManager* _graphicSessionManager, MultiCanvasComponentAmusing* _multiCanvasComponent)
{
    mainContentComponent->CompleteInitialization(_graphicSessionManager, _multiCanvasComponent);
}

void View::CompleteInitialization(AmusingModel* _model)
{
	model = _model;
	mainContentComponent->CompleteInitialization(model);
}

void View::ReleaseOpengGLResources()
{
	mainContentComponent->ReleaseOpengGLResources();
	mainContentComponent->HideOpenGLCanevas();
}

void View::setSampleColor(const int numSamples, Colour colorCode[])
{
	mainContentComponent->setSamplesColor(numSamples, colorCode);
}

void Amusing::View::setDefaultPath(String defaultPath)
{
	mainContentComponent->setDefaultPath(defaultPath);
}

void Amusing::View::setSoundPath(int idx, String _path)
{
	mainContentComponent->setSoundPath(idx, _path);
}

void View::ButtonClicked(const String& /*name*/)
{
    throw std::runtime_error("Unimplemented behavior on button click");
    /*
    AppMode answeredAppMode = AppMode::Null;
    if (name == "Speakers text button")
        answeredAppMode = presenter->appModeChangeRequest(AppMode::EditSpeakers);
    else if (name == "Speakers Groups text button")
        answeredAppMode = presenter->appModeChangeRequest(AppMode::EditSpeakersGroups);
    else if (name == "Scenes text button")
        answeredAppMode = presenter->appModeChangeRequest(AppMode::EditSpatScenes);
    else if (name == "Hardware Configuration text button")
        answeredAppMode = presenter->appModeChangeRequest(AppMode::EditHardwareConfiguration);
    else if (name == "Start text button")
        answeredAppMode = presenter->appModeChangeRequest(AppMode::MiamSpatPlaying);
    */
    
    // This is a behavior : defined in presenter then....
    /*if (answeredAppMode != AppMode::Null)
        mainContentComponent->ChangeAppMode(answeredAppMode);*/
}



void View::ChangeAppMode(AppMode /*newAppMode*/)
{
    std::cerr << "Changement de mode à implémenter" << std::endl;
    //throw std::runtime_error("Unimplemented behavior on app mode change");
}
void View::DisplayInfo(const String& /*message*/)
{
    throw std::runtime_error("Unimplemented behavior on info display request");
}

void View::ShowDeviceOptionsDialog()
{
	mainContentComponent->ReleaseOpengGLResources();
	mainContentComponent->ShowDeviceOptionsDialog();
}

void View::ShowSoundManager()
{
	mainContentComponent->ReleaseOpengGLResources();
	mainContentComponent->ShowSoundManagerComponent();
}

void View::removeDeviceManagerFromOptionWindow()
{
	mainContentComponent->removeDeviceManagerFromOptionWindow();
}

std::shared_ptr<bptree::ptree> View::GetAudioSettingsTree()
{
	auto viewTree = std::make_shared<bptree::ptree>();

	auto viewInnerTree =  std::make_shared<bptree::ptree>();
	auto soundTree = mainContentComponent->GetSoundTree();
	viewInnerTree->add_child("soundTree", *soundTree);

	viewTree->add_child("sound", *viewInnerTree);
	return viewTree;//soundTree;//canvasInnerTree;
}

void View::setSoundsSettings(bptree::ptree tree)
{
	bptree::ptree soundTree;
	try
	{
		soundTree = tree.get_child("sound");
	}
	catch (bptree::ptree_error& e)
	{
		throw XmlReadException("<view> : error extracting <sound> nodes : ", e);
	}

	if (!soundTree.empty())
	{
		mainContentComponent->setSoundSettings(soundTree);
	}
}


//void View::setDeviceSelectorComponent(AudioDeviceManager* deviceManager)
//{
//	mainContentComponent->CreateDeviceSelector(deviceManager);
//}
