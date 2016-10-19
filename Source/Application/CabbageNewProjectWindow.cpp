/*
  ==============================================================================

    CabbageNewProjectWindow.cpp
    Created: 17 Oct 2016 4:05:51pm
    Author:  rory

  ==============================================================================
*/

#include "CabbageNewProjectWindow.h"
#include "CabbageApplication.h"
#include "../Utilities/CabbageStrings.h"


CabbageProjectWindow::CabbageProjectWindow(CabbageApplication* owner, ValueTree valueTree):
owner(owner),
valueTree(valueTree),
newInstrumentButton("newInstrument", this), 
newEffectButton("newEffect", this), 
newCsoundFileButton("newCsound", this)
{
	
	CabbageUtilities::debug(Desktop::getInstance().getDisplays().getMainDisplay().dpi);
	addAndMakeVisible(newCsoundFileButton);
	addAndMakeVisible(newInstrumentButton);
	addAndMakeVisible(newEffectButton);
	newCsoundFileButton.addListener(this);
	newInstrumentButton.addListener(this);
	newEffectButton.addListener(this);
	
	//Image instrumentImage = ImageFileFormat::loadFrom(File("Normal.png"));
	const Image instrumentImage = ImageCache::getFromMemory (CabbageBinaryData::InstrumentButton_png, CabbageBinaryData::InstrumentButton_pngSize);
	newInstrumentButton.setImages(true, true, true, instrumentImage, 1, Colours::transparentBlack, instrumentImage,
					 1, Colours::transparentBlack, instrumentImage, .8, Colours::transparentBlack, 0);
	newInstrumentButton.setTooltip("New Plugin Instrument");

	const Image effectImage = ImageCache::getFromMemory (CabbageBinaryData::EffectButton_png, CabbageBinaryData::EffectButton_pngSize);
	newEffectButton.setImages(true, true, true, effectImage, 1, Colours::transparentBlack, effectImage,
					 1, Colours::transparentBlack, effectImage, .8, Colours::transparentBlack, 0);
	newEffectButton.setTooltip("New Plugin Effect");
					 
	const Image csoundImage = ImageCache::getFromMemory (CabbageBinaryData::CsoundFileButton_png, CabbageBinaryData::CsoundFileButton_pngSize);
	newCsoundFileButton.setImages(true, true, true, csoundImage, 1, Colours::transparentBlack, csoundImage,
					 1, Colours::transparentBlack, csoundImage, .8, Colours::transparentBlack, 0);
	newCsoundFileButton.setTooltip("New Csound file");

}

void CabbageProjectWindow::buttonClicked(Button* button)
{
	CabbageIDELookAndFeel lookAndFeel;
	if(button->getName()=="newCsound")
	{
		FileChooser fc ("Select file name and location", File::getSpecialLocation(File::SpecialLocationType::userHomeDirectory));

		if (fc.browseForFileToSave(false))
		{
				if(fc.getResult().existsAsFile())
				{
					const int result = CabbageUtilities::showYesNoMessage("Do you wish to overwrite\nexiting file?", &lookAndFeel);
					if(result==0)
					{	
						fc.getResult().replaceWithText(CabbageStrings::getNewCsoundFileText());
						owner->openFile (fc.getResult(), "Csound");
						delete this->getParentComponent();
					}
				}		
		}	
	}
	else 
		CabbageUtilities::showMessage("Warning", "Yet to be implemented", &lookAndFeel);
}