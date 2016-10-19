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
#include "../Utilities/CabbageUtilities.h"


CabbageProjectWindow::CabbageProjectWindow(CabbageApplication* owner, ValueTree valueTree):
owner(owner),
valueTree(valueTree),
newInstrumentButton("newInstrument", this), 
newEffectButton("newEffect", this), 
newCsoundFileButton("newCsound", this)
{
	addAndMakeVisible(newCsoundFileButton);
	addAndMakeVisible(newInstrumentButton);
	addAndMakeVisible(newEffectButton);
	newCsoundFileButton.addListener(this);
	newInstrumentButton.addListener(this);
	newEffectButton.addListener(this);
	
	//Image instrumentImage = ImageFileFormat::loadFrom(File("Normal.png"));
	const Image instrumentImage = ImageCache::getFromMemory (CabbageBinaryData::InstrumentButton_png, CabbageBinaryData::InstrumentButton_pngSize);
	CabbageUtilities::setImagesForButton(&newInstrumentButton, instrumentImage);

	const Image effectImage = ImageCache::getFromMemory (CabbageBinaryData::EffectButton_png, CabbageBinaryData::EffectButton_pngSize);
	CabbageUtilities::setImagesForButton(&newEffectButton, effectImage);
					 
	const Image csoundImage = ImageCache::getFromMemory (CabbageBinaryData::CsoundFileButton_png, CabbageBinaryData::CsoundFileButton_pngSize);
	CabbageUtilities::setImagesForButton(&newCsoundFileButton, csoundImage);
	

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