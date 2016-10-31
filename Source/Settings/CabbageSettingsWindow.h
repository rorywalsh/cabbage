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
#include "../Utilities/CabbageColourProperty.h"
#include "../Utilities/CabbageFilePropertyComponent.h"
#include "../Utilities/CabbageUtilities.h"
#include "../BinaryData/CabbageBinaryData.h"


class CabbageSettingsWindow : 
public Component, 
public Button::Listener, 
public Value::Listener,
public FilenameComponentListener,
public ChangeListener
{

public:
    CabbageSettingsWindow(CabbageSettings &settings, AudioDeviceSelectorComponent* audioDevice);
    ~CabbageSettingsWindow()
	{
		audioDeviceSelector = nullptr;	
	};

	void changeListenerCallback(ChangeBroadcaster *source);
    void addColourProperties();
	void addMiscProperties();
    void resized();
	void buttonClicked(Button* button);
	void paint(Graphics& g);
	void valueChanged(Value& value);
    void filenameComponentChanged (FilenameComponent*);

	
private:
    PropertyPanel colourPanel, miscPanel;
	ScopedPointer<AudioDeviceSelectorComponent> audioDeviceSelector;
    ValueTree valueTree;
    TextButton loadButton, saveButton;
	ImageButton audioSettingsButton, colourSettingsButton, miscSettingsButton;
	CabbageSettings &settings;
	Value alwaysOnTopValue, showLastOpenedFileValue, compileOnSaveValue;

};



#endif  // CABBAGESETTINGSWINDOW_H_INCLUDED
