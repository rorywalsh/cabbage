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

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "../Utilities/CabbageUtilities.h"


class FileTab : public TextButton
{
    DrawableButton play, close, showEditor, editGUI;
    File csdFile;

    class Overlay : public Component
    {
    public:
        Overlay(): Component() {}
        void paint (Graphics& g)  override
        {
            g.fillAll (Colours::black.withAlpha (.5f));
        }
    };

    Overlay overlay;
public:


    FileTab (String name, String filename);
    const String getFilename() { return csdFile.getFullPathName();    }

    void drawButtonShape (Graphics& g, const Path& outline, Colour baseColour, float height);
    void paintButton (Graphics& g, bool isMouseOverButton, bool isButtonDown);
    void addButtonListeners (Button::Listener* listener);
    void disableButtons (bool disable);
    void resized();
    void setDrawableImages (DrawableButton& button, int width, int height, String type);
    void drawButtonText (Graphics& g);

    File getFile() {     return csdFile; }
    void setFile (File file) {        csdFile = file;   }
    DrawableButton& getPlayButton() {    return play;    }
    DrawableButton& getShowEditorButton() {  return showEditor;  }
    DrawableButton& getCloseFileEditorButton() { return close;   }
    DrawableButton& getEditGUIButton() { return editGUI; }
};
