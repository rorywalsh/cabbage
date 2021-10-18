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

class CabbageMainComponent;

class CabbageProjectWindow : public Component, public Button::Listener
{
    class SimpleImageButton : public ImageButton
    {
    public:
        SimpleImageButton (String name, CabbageProjectWindow* owner): ImageButton (name), owner (owner), name (name) {}
        ~SimpleImageButton() {}

        void mouseEnter (const MouseEvent& e)
        {
            if (name == "newInstrument")
                owner->setInformationString ("Choose this option to create a new synth instrument. This option should be used for any instruments that will be generating sounds in reponse to MIDI notes.");

            else if (name == "newEffect")
                owner->setInformationString ("Choose this option to create a new effect based instrument. This option should be used for any instruments that will be processing incoming audio.");

            else if (name == "newCsound")
                owner->setInformationString ("Choose this option to create a new Csound file. This option should be used for any old-school Csound instruments that don't use a GUI.");
            
            else if (name == "newRackModule")
                owner->setInformationString ("Choose this option to create a new VCV Rack module file. Note Cabbage Rack modules only support basic widgets. See the docs for more details.");

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
    CabbageProjectWindow (CabbageMainComponent* owner);
    ~CabbageProjectWindow() {}

    void setInformationString (String informationStr)
    {
        information = informationStr;
    }

    void buttonClicked (Button* button) override;

	void paint(Graphics& g)  override;

    void resized() override
    {
        // Rectangle<int> r (getLocalBounds());

        newInstrumentButton.setBounds (50, 50, 150, 150);
        newEffectButton.setBounds (250, 50, 150, 150);
        newCsoundFileButton.setBounds (450, 50, 150, 150);
        newRackModuleFileButton.setBounds(650, 50, 150, 150);
    }

private:
    void writeNewFile (File fc, String fileText);
    void createNewFile (String type);
    CabbageMainComponent* owner;
    String information;
    SimpleImageButton newInstrumentButton, newEffectButton, newCsoundFileButton, newRackModuleFileButton;
};


#endif  // CABBAGENEWPROJECTWINDOW_H_INCLUDED
