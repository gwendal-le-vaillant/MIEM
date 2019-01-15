/*
  ==============================================================================

    Presenter.cpp
    Created: 11 Mar 2016 4:45:22pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "Presenter.h"

#include "Model.h"
#include "View.h"

#include "JuceHeader.h"


#include "AppPurpose.h"

using namespace Miam;


// - - - - - Contruction and Destruction - - - - -
AppPurpose App::appPurpose = AppPurpose::Spatialisation;

Presenter::Presenter(View* _view) :
    PlayerPresenter(_view),

    view(_view),

    graphicSessionManager(this, _view)
{
    // After all sub-modules are built, the presenter refers itself to the View
    view->CompleteInitialization(this);
    //view->GetMainContentComponent()->resized();
    view->OnNewVolumes(lastLowCorrelationVolume, lastHighCorrelationVolume);
    
    appModeChangeRequest(PlayerAppMode::Loading);
}
void Presenter::CompleteInitialisation(Model* _model)
{
    // - - - Init des attributs privés puis des parents - - -
    model = _model;
    
    PlayerPresenter::CompleteInitialisation(model);
}


// = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =



// = = = = = = = = = = PERIODIC UPDATES = = = = = = = = = =
void Presenter::Update()
{
    AsyncParamChange paramChange;
    while (model->TryGetAsyncParamChange(paramChange))
    {
        bool wasVolumeUpdated = false;
        switch(paramChange.Type)
        {
            case AsyncParamChange::ParamType::Volume_CorrelatedInputs :
                lastHighCorrelationVolume = paramChange.DoubleValue;
                wasVolumeUpdated = true;
                break;
            case AsyncParamChange::ParamType::Volume_DecorrelatedInputs :
                lastLowCorrelationVolume = paramChange.DoubleValue;
                wasVolumeUpdated = true;
                break;
                
            // Ack : model is actually stopped
            case AsyncParamChange::ParamType::Stopped :
                lastLowCorrelationVolume = 0.0;
                lastHighCorrelationVolume = 0.0;
                wasVolumeUpdated = true;
                break;
            
            default : break;
        }
        
        if (wasVolumeUpdated)
            view->OnNewVolumes(lastLowCorrelationVolume, lastHighCorrelationVolume);
    }
}


