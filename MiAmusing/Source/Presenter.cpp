/*
  ==============================================================================

    Presenter.cpp
    Created: 11 Mar 2016 4:45:22pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "Presenter.h"

#include "AmusingModel.h"
#include "View.h"


#include "JuceHeader.h"

#include "AnimatedPolygon.h"
#include "Follower.h"
#include "GraphicEvent.h"
#include "MultiAreaEvent.h"


using namespace Amusing;


// - - - - - Contruction and Destruction - - - - -

Presenter::Presenter(View* _view) :
    view(_view), test(),
    appMode(AppMode::Loading), // app is loading while the Model hasn't fully loaded yet

    graphicSessionManager(this, _view)
{
	test.insert(std::pair<int, double>(2, 5.5));
    // After all sub-modules are built, the presenter refers itself to the View
    view->CompleteInitialization(this);
    view->GetMainContentComponent()->resized();
    
    // HERE, WE SHOULD LOAD THE DEFAULT FILE
    //graphicSessionManager.__LoadDefaultTest();
	lastPosition = 0;
    
    appModeChangeRequest(AppMode::None);
	Nsources = 0;
	Nfollower = 0;
	tempo = 4;
	SetAllChannels();
}

void Presenter::SetAllChannels()
{
	graphicSessionManager.SetAllChannels();
}


void Presenter::CompleteInitialisation(AmusingModel* _model)
{
    // Self init
    model = _model;
	//view->CompleteInitialization(model);
	//graphicSessionManager.CompleteInitialization(model);
}





AppMode Presenter::appModeChangeRequest(AppMode newAppMode)
{
    // First check : are we running a new mode ?
    if (newAppMode != appMode)
    {
        view->ChangeAppMode(AppMode::Loading);
        
        appMode = newAppMode;
        view->ChangeAppMode(appMode);
    }
    
    
    // Actual new mode
    return appMode;
}

void Presenter::setTempo(int newTempo)
{
	tempo = newTempo;
}

int Presenter::getTempo()
{
	return tempo;
}

void Presenter::setChannel(std::shared_ptr<EditableScene> scene,int channel)
{
	DBG("size of the map = " + (String)sceneToChannel.size());
	sceneToChannel[scene] = channel;
	//test[5] = 2;
	//test.insert(std::pair<int, double>(3, 5.8));
}

int Presenter::getChannel(std::shared_ptr<EditableScene> scene)
{
	if (sceneToChannel.find(scene) == sceneToChannel.end())
		sceneToChannel[scene] = 1;
	return sceneToChannel[scene];
}

int Presenter::getSourceID(std::shared_ptr<IEditableArea> area)
{
	/*
	if (areaToSource.find(area) == areaToSource.end())
	{
		// area has no associated source yet
		areaToSource[area] = Nsources;
		sourceToArea[Nsources] = area;
		++Nsources;
	}
	return areaToSource[area];
	*/
	
	if (areaToSourceMulti.left.find(area) == areaToSourceMulti.left.end())
	{
		//areaToSourceMulti.left[area] = Nsources;
		std::pair<std::shared_ptr<IEditableArea>, int> newPair(area, Nsources);
		areaToSourceMulti.left.insert(newPair);
		++Nsources;
	}
	
	return areaToSourceMulti.left.at(area);
}

int Presenter::getCtrlSourceId(std::shared_ptr<Follower> follower)
{
	if (followerToCtrlSource.left.find(follower) == followerToCtrlSource.left.end())
	{
		DBG("ajoute : " + (String)Nfollower);
		std::pair<std::shared_ptr<Follower>, int> newPair(follower, Nfollower);
		followerToCtrlSource.left.insert(newPair);
		++Nfollower;
	}
	return followerToCtrlSource.left.at(follower);
}

std::shared_ptr<IEditableArea> Presenter::getAreaFromSource(int source)
{
	/*
	if (sourceToArea.find(source) == sourceToArea.end())
	{
		// source has no associated area
	}
	return sourceToArea[source];
	*/
	if (areaToSourceMulti.right.find(source) == areaToSourceMulti.right.end())
	{
		DBG("problemes");
	}
	return areaToSourceMulti.right.at(source);
}

std::shared_ptr<Follower> Presenter::getFollowerFromCtrl(int ctrlId)
{
	if (followerToCtrlSource.right.find(ctrlId) == followerToCtrlSource.right.end())
	{
		DBG("pas de follower associe");
		return nullptr;
	}
	return followerToCtrlSource.right.at(ctrlId);
}

static int updatesCount = 0;
void Presenter::Update() // remettre l'interieur dans graphsessionmanager
{
	/*
	auto param = AsyncParamChange();
	for (int i = 0; i<100; i++)
	{
		updatesCount++;
		param.IntegerValue = updatesCount;
		SendParamChange(param);
	}
	DBG("la dedans");
	DBG(std::to_string(param.IntegerValue));
	*/
	//DBG("La");
	AsyncParamChange param;
	
	std::shared_ptr<GraphicEvent> graphicE;

	
	while (model->TryGetAsyncParamChange(param))
	{
		switch (param.Type)
		{
		case AsyncParamChange::ParamType::Activate :
			break;
		case AsyncParamChange::ParamType::Duration :
			DBG("new duration");
			break;
		case AsyncParamChange::ParamType::Position :
			
			
				//DBG("recu : " + (String)(1000 * param.DoubleValue));
				//DBG("param received");
				//graphicSessionManager.OnAudioPosition(param.DoubleValue);
				//graphicSessionManager.SetAllAudioPositions(param.DoubleValue);
			lastPosition = param.DoubleValue;
				
			break;

		default:
			break;
		}
	}
	graphicSessionManager.SetAllAudioPositions(lastPosition);
}

AudioDeviceManager& Presenter::getAudioDeviceManager()
{
	return model->getAudioDeviceManager();
}