/*
  ==============================================================================

    CabbageNewProjectWindow.h
    Created: 16 Oct 2016 7:46:10pm
    Author:  rory

  ==============================================================================
*/

//<div>Icons made by <a href="http://www.flaticon.com/authors/freepik" title="Freepik">Freepik</a> from <a href="http://www.flaticon.com" title="Flaticon">www.flaticon.com</a> is licensed by <a href="http://creativecommons.org/licenses/by/3.0/" title="Creative Commons BY 3.0" target="_blank">CC 3.0 BY</a></div>
#ifndef CABBAGENEWPROJECTWINDOW_H_INCLUDED
#define CABBAGENEWPROJECTWINDOW_H_INCLUDED

#include "../BinaryData/CabbageBinaryData.h"

class CabbageProjectWindow : public Component
{

public:
    CabbageProjectWindow(ValueTree valueTree):valueTree(valueTree),
        newCabbageInstrumentButton("newInstrument"), 
		newCabbageEffectButton("newEffect"), 
		newCsoundFileButton("newCsound")
    {
		CabbageUtilities::debug(Desktop::getInstance().getDisplays().getMainDisplay().dpi);
		addAndMakeVisible(newCsoundFileButton);
		addAndMakeVisible(newCabbageInstrumentButton);
		//addAndMakeVisible(newCabbageEffectButton);
		
		//Image instrumentImage = ImageFileFormat::loadFrom(File("Normal.png"));
        Image instrumentImage = ImageCache::getFromMemory (CabbageBinaryData::CabbageInstrumentButtonNormal_png, CabbageBinaryData::CabbageInstrumentButtonNormal_pngSize);
        newCabbageInstrumentButton.setImages(true, true, true, instrumentImage, 1, Colours::transparentBlack, instrumentImage,
                         1, Colours::transparentBlack, instrumentImage, 1, Colours::transparentBlack, 0);


        Image effectImage = ImageCache::getFromMemory (CabbageBinaryData::CabbageEffectButtonNormal_png, CabbageBinaryData::CabbageEffectButtonNormal_pngSize);
        newCabbageEffectButton.setImages(true, true, true, effectImage, 1, Colours::transparentBlack, effectImage,
                         1, Colours::transparentBlack, effectImage.rescaled(221, 60), 1, Colours::transparentBlack, 0);

    }

    ~CabbageProjectWindow() {}

    void resized() override
    {
        Rectangle<int> r (getLocalBounds());
		
		newCsoundFileButton.setBounds(50, 50, 221, 166);
		newCabbageInstrumentButton.setBounds(50, 50, 221, 166);
    }

private:

    ValueTree valueTree;
    ImageButton newCabbageInstrumentButton, newCabbageEffectButton;
	TextButton newCsoundFileButton;


};


#endif  // CABBAGENEWPROJECTWINDOW_H_INCLUDED
