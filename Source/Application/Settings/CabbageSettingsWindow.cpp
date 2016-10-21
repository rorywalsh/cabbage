/*
  ==============================================================================

    CabbageSettingsWindow.cpp
    Created: 14 Oct 2016 4:12:45pm
    Author:  rory

  ==============================================================================
*/

#include "CabbageSettingsWindow.h"

CabbageSettingsWindow::CabbageSettingsWindow(ValueTree tree, AudioDeviceSelectorComponent* audioDevice): 
Component(""), 
valueTree(tree),
audioSettingsButton("AudioSettingsButton"),
miscSettingsButton("MiscSettingsButton"),
colourSettingsButton("ColourSettingsButton"),
audioDeviceSelector(audioDevice)
{
    addColourProperties();
    addAndMakeVisible (colourPanel);
	colourPanel.setVisible(false);
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
        props.add (new ColourPropertyComponent(valueTree, index));

    colourPanel.clear();
    colourPanel.addProperties (props);
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
	
	
	//colourPanel.setBounds (r.removeFromTop (getHeight() - 28).reduced (4, 2));
	//loadButton.setBounds (r.removeFromLeft (getWidth() / 2).reduced (10, 4));
	//saveButton.setBounds (r.reduced (10, 3));
}

void CabbageSettingsWindow::paint(Graphics& g)
{
	g.fillAll(Colour(147, 210, 0));
}
	
void CabbageSettingsWindow::buttonClicked(Button* button)
{
	if(button->getName()=="AudioSettingsButton")
	{
		audioDeviceSelector->setVisible(true);
		colourPanel.setVisible(false);
	}
	else if(button->getName()=="ColourSettingsButton")
	{
		audioDeviceSelector->setVisible(false);
		colourPanel.setVisible(true);			
	}
}