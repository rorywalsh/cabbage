/*
  ==============================================================================

    CabbageSettingsWindow.cpp
    Created: 14 Oct 2016 4:12:45pm
    Author:  rory

  ==============================================================================
*/

#include "CabbageSettingsWindow.h"

static void addFilenameComponentListener(FilenameComponentListener* parent, PropertyComponent* propertyComponent)
{
	if(CabbageFilePropertyComponent* fileComp = dynamic_cast<CabbageFilePropertyComponent*>(propertyComponent))
		fileComp->filenameComp.addListener(parent);
		
}

CabbageSettingsWindow::CabbageSettingsWindow(CabbageSettings &settings, AudioDeviceSelectorComponent* audioDevice): 
Component(""), 
settings(settings),
valueTree(settings.getValueTree()),
audioSettingsButton("AudioSettingsButton"),
miscSettingsButton("MiscSettingsButton"),
colourSettingsButton("ColourSettingsButton"),
audioDeviceSelector(audioDevice)
{
    addColourProperties();
	addMiscProperties();
    addAndMakeVisible (colourPanel);
	addAndMakeVisible (miscPanel);
	colourPanel.setVisible(false);
	miscPanel.setVisible(false);
	addAndMakeVisible(audioSettingsButton);
	addAndMakeVisible(miscSettingsButton);
	addAndMakeVisible(colourSettingsButton);
	addAndMakeVisible(audioDeviceSelector);
	audioSettingsButton.addListener(this);
	miscSettingsButton.addListener(this);
	colourSettingsButton.addListener(this);
	
	const Image audioSettingsImage = ImageCache::getFromMemory (CabbageBinaryData::AudioSettingsButton_png, CabbageBinaryData::AudioSettingsButton_pngSize);
	CabbageUtilities::setImagesForButton(&audioSettingsButton, audioSettingsImage);

	const Image miscSettingsImage = ImageCache::getFromMemory (CabbageBinaryData::MiscSettingsButton_png, CabbageBinaryData::MiscSettingsButton_pngSize);
	CabbageUtilities::setImagesForButton(&miscSettingsButton, miscSettingsImage);
					 
	const Image colourSettingsImage = ImageCache::getFromMemory (CabbageBinaryData::ColourSettingsButton_png, CabbageBinaryData::ColourSettingsButton_pngSize);
	CabbageUtilities::setImagesForButton(&colourSettingsButton, colourSettingsImage);

}

void CabbageSettingsWindow::addColourProperties()
{
    Array<PropertyComponent*> props;

    for (int index = 0; index < valueTree.getChildWithName("Colours").getNumProperties(); ++index)
        props.add (new ColourSettingsPropertyComponent(valueTree, index));

    colourPanel.clear();
    colourPanel.addProperties (props);
}

void CabbageSettingsWindow::addMiscProperties()
{
    Array<PropertyComponent*> props;

	showLastOpenedFileValue.setValue(settings.getUserSettings()->getIntValue("OpenMostRecentFileOnStartup"));
	showLastOpenedFileValue.addListener(this);
	alwaysOnTopValue.setValue(settings.getUserSettings()->getIntValue("SetAlwaysOnTop"));
	alwaysOnTopValue.addListener(this);
	compileOnSaveValue.setValue(settings.getUserSettings()->getIntValue("CompileOnSave"));
	compileOnSaveValue.addListener(this);
	
    props.add (new BooleanPropertyComponent(showLastOpenedFileValue, "Auto-load", "Auto-load last opened file"));
	props.add (new BooleanPropertyComponent(alwaysOnTopValue, "Plugin Window", "Always show plugin on top"));
	props.add (new BooleanPropertyComponent(compileOnSaveValue, "Compiling", "Compile on save"));
	
	props.add (new CabbageFilePropertyComponent("Csound manual dir.", true, false));
	addFilenameComponentListener(this, props[props.size()-1]); 
	props.add (new CabbageFilePropertyComponent("Cabbage plants dir.", true, false));
	addFilenameComponentListener(this, props[props.size()-1]); 
	props.add (new CabbageFilePropertyComponent("Cabbage examples dir.", true, false));
	addFilenameComponentListener(this, props[props.size()-1]); 

	miscPanel.clear();
    miscPanel.addProperties(props);	
}

void CabbageSettingsWindow::resized()
{
	Rectangle<int> r (getLocalBounds());
	audioSettingsButton.setBounds(10, 10, 60, 60);
	miscSettingsButton.setBounds(10, 80, 60, 60);
	colourSettingsButton.setBounds(10, 150, 60, 60);
	
	if(audioDeviceSelector)
		audioDeviceSelector->setBounds(100, 10, r.getWidth()-100, r.getHeight()-20);
	
	colourPanel.setBounds(100, 10, r.getWidth()-100, r.getHeight()-20);
	miscPanel.setBounds(100, 10, r.getWidth()-100, r.getHeight()-20);

}

void CabbageSettingsWindow::paint(Graphics& g)
{
	g.fillAll(Colour(147, 210, 0));
}
//=====================================================================	
void CabbageSettingsWindow::valueChanged(Value& value)
{
	if(value == showLastOpenedFileValue)
		settings.getUserSettings()->setValue("OpenMostRecentFileOnStartup", value.getValue().toString());
	else if(value == alwaysOnTopValue)
		settings.getUserSettings()->setValue("SetAlwaysOnTop", value.getValue().toString());
	else if(value == compileOnSaveValue)
		settings.getUserSettings()->setValue("CompileOnSave", value.getValue().toString());
}

void CabbageSettingsWindow::filenameComponentChanged (FilenameComponent* fileComponent)
{
	CabbageUtilities::debug(fileComponent->getName());
}
	
void CabbageSettingsWindow::buttonClicked(Button* button)
{
	if(button->getName()=="AudioSettingsButton")
	{
		audioDeviceSelector->setVisible(true);
		colourPanel.setVisible(false);
		miscPanel.setVisible(false);
	}
	else if(button->getName()=="ColourSettingsButton")
	{
		audioDeviceSelector->setVisible(false);
		colourPanel.setVisible(true);
		miscPanel.setVisible(false);			
	}
	else if(button->getName()=="MiscSettingsButton")
	{
		audioDeviceSelector->setVisible(false);
		colourPanel.setVisible(false);
		miscPanel.setVisible(true);			
	}
	
}