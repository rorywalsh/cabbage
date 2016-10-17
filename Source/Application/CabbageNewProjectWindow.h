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
#include "CabbageIDELookAndFeel.h"

class CabbageApplication;

class CabbageProjectWindow : public Component, public Button::Listener
{
		class SimpleImageButton : public ImageButton
		{
			public:
				SimpleImageButton(String name, CabbageProjectWindow* owner):owner(owner), name(name), ImageButton(name){}
				~SimpleImageButton(){}
				
				void mouseEnter(const MouseEvent &e)
				{
						if(name=="newInstrument")
							owner->setInformationString("Choose this option to create a new synth instrument. This option should be used for any instruments that will be generating sounds in reponse to MIDI notes.");

						else if(name=="newEffect")
							owner->setInformationString("Choose this option to create a new effect based instrument. This option should be used for any instruments that will be processing incoming audio.");

						else if(name=="newCsound")
							owner->setInformationString("Choose this option to create a new Csound file. This option should be used for any old-school Csound instruments/files that don't use a GUI.");
						
						owner->repaint();
				}
				
				void mouseExit(const MouseEvent &e)
				{
					owner->setInformationString("Please choose a project type.");
					owner->repaint();
				}
				
			private:
				CabbageProjectWindow* owner;
				String name;
			
		};
public:
    CabbageProjectWindow(CabbageApplication* owner, ValueTree valueTree);
    ~CabbageProjectWindow() {}
	
	void setInformationString(String informationStr)
	{
		information = informationStr;
	}

	void buttonClicked(Button* button);
	
	void paint(Graphics& g)
	{
		g.fillAll(Colour(147,210,0));
		g.setColour(Colours::white);
		g.setFont(Font(20));
		g.drawFittedText(information, 0, 200, getWidth(), 100, Justification::centred, 10);
	}

    void resized() override
    {
        Rectangle<int> r (getLocalBounds());
		
		newInstrumentButton.setBounds(50, 50, 150, 150);
		newEffectButton.setBounds(250, 50, 150, 150);
		newCsoundFileButton.setBounds(450, 50, 150, 150);
    }

private:

    ValueTree valueTree;
	String information;
    SimpleImageButton newInstrumentButton, newEffectButton, newCsoundFileButton;
	CabbageApplication* owner;


};


#endif  // CABBAGENEWPROJECTWINDOW_H_INCLUDED
