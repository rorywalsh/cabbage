/*
  Copyright (C) 2016 Rory Walsh

  Cabbage is free software; you can redistribute it
  and/or modify it under the terms of the GNU General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  Cabbage is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU General Public
  License along with Csound; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
  02111-1307 USA
*/

#ifndef CABBAGENEWPROJECTWINDOW_H_INCLUDED
#define CABBAGENEWPROJECTWINDOW_H_INCLUDED

#include "../BinaryData/CabbageBinaryData.h"
#include "../LookAndFeel/CabbageIDELookAndFeel.h"

class CabbageContentComponent;

class CabbageProjectWindow : public Component, public Button::Listener
{
    class SimpleImageButton : public ImageButton
    {
    public:
        SimpleImageButton (String name, CabbageProjectWindow* owner): owner (owner), name (name), ImageButton (name) {}
        ~SimpleImageButton() {}

        void mouseEnter (const MouseEvent& e)
        {
            if (name == "newInstrument")
                owner->setInformationString ("Choose this option to create a new synth instrument. This option should be used for any instruments that will be generating sounds in reponse to MIDI notes.");

            else if (name == "newEffect")
                owner->setInformationString ("Choose this option to create a new effect based instrument. This option should be used for any instruments that will be processing incoming audio.");

            else if (name == "newCsound")
                owner->setInformationString ("Choose this option to create a new Csound file. This option should be used for any old-school Csound instruments/files that don't use a GUI.");

            owner->repaint();
        }

        void mouseExit (const MouseEvent& e)
        {
            owner->setInformationString ("Please choose a project type.");
            owner->repaint();
        }

    private:
        CabbageProjectWindow* owner;
        String name;

    };
public:
    CabbageProjectWindow (CabbageContentComponent* owner);
    ~CabbageProjectWindow() {}

    void setInformationString (String informationStr)
    {
        information = informationStr;
    }

    void buttonClicked (Button* button);

    void paint (Graphics& g)
    {
        g.fillAll (Colour (147, 210, 0));
        g.setColour (Colours::white);
        g.setFont (Font (20));
        g.drawFittedText (information, 0, 200, getWidth(), 100, Justification::centred, 10);
    }

    void resized() override
    {
        Rectangle<int> r (getLocalBounds());

        newInstrumentButton.setBounds (50, 50, 150, 150);
        newEffectButton.setBounds (250, 50, 150, 150);
        newCsoundFileButton.setBounds (450, 50, 150, 150);
    }

private:
    void writeNewFile (File fc, String fileText);
    void createNewFile (String type);
    CabbageContentComponent* owner;
    String information;
    SimpleImageButton newInstrumentButton, newEffectButton, newCsoundFileButton;
};


#endif  // CABBAGENEWPROJECTWINDOW_H_INCLUDED
