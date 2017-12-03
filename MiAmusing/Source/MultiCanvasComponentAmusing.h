/*
  ==============================================================================

    MultiCanvasComponentAmusing.h
    Created: 30 May 2017 3:19:47pm
    Author:  ayup1

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "MultiCanvasComponent.h"

namespace Miam
{
	class IGraphicSessionManager;
}

class MultiCanvasComponentAmusing : public Miam::MultiCanvasComponent
{
public:
	MultiCanvasComponentAmusing(Miam::IGraphicSessionManager* _graphicSessionManager);
	~MultiCanvasComponentAmusing();

	void setSamplesColor(int Nsamples, Colour colorCode[]);

	void resized() override;

	MultiSceneCanvasComponent* AddCanvas() override;

private:
	SafePointer<AudioDeviceSelectorComponent> audioSetupComp;
	SafePointer<DialogWindow> dialogWindow;
	DialogWindow::LaunchOptions options;
	bool show;
};

