/*
  ==============================================================================

    CabbageSettingsWindow.h
    Created: 14 Oct 2016 4:12:59pm
    Author:  rory

  ==============================================================================
*/

#ifndef CABBAGESETTINGSWINDOW_H_INCLUDED
#define CABBAGESETTINGSWINDOW_H_INCLUDED

#include "../CabbageIds.h"
#include "CabbageSettings.h"
#include "../../Utilities/CabbageColourPropertyComponent.h"
#include "../../Utilities/CabbageUtilities.h"


class CabbageSettingsWindow : public Component, public Button::Listener
{

public:
    CabbageSettingsWindow(ValueTree valueTree, AudioDeviceSelectorComponent* audioDevice);
    ~CabbageSettingsWindow()
	{
		audioDeviceSelector = nullptr;
	
	};

    void addColourProperties();
    void resized();
	void buttonClicked(Button* button);
	void paint(Graphics& g);
private:
    PropertyPanel colourPanel, miscPanel;
	ScopedPointer<AudioDeviceSelectorComponent> audioDeviceSelector;
    ValueTree valueTree;
    TextButton loadButton, saveButton;
	ImageButton audioSettingsButton, colourSettingsButton, miscSettingsButton;


};



#endif  // CABBAGESETTINGSWINDOW_H_INCLUDED
