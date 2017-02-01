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

#ifndef CabbageEditorContainer_H_INCLUDED
#define CabbageEditorContainer_H_INCLUDED

#include "../CabbageCommonHeaders.h"
#include "CabbageCodeEditor.h"
#include "CabbageOutputConsole.h"

class CabbageContentComponent;

class CabbageEditorContainer   : public Component
{
public:
    //-------------------------------------------------------------
    class StatusBar : public Component
    {
    public:
        StatusBar (ValueTree valueTree, CabbageEditorContainer* parent)
            :   Component ("StatusBar"),
                valueTree (valueTree),
                owner (parent)
        {
            String initString = (SystemStats::getOperatingSystemName() +
                                 "CPU: " + String (SystemStats::getCpuSpeedInMegaherz())
                                 + "MHz  Cores: " + String (SystemStats::getNumCpus())
                                 + "  " + String (SystemStats::getMemorySizeInMegabytes()) + "MB");
            setText (StringArray (initString));
        }

        void paint (Graphics& g);

        void setText (StringArray text)
        {
            statusText = text;
            repaint();
        }

        int getCurrentYPos() {   return currentYPos; }

    private:
        ValueTree valueTree;
        StringArray statusText;
        int startingYPos;
        bool isActive = false;
        int currentYPos = 550;
        CabbageEditorContainer* owner;
    };

    CabbageContentComponent* getContentComponent();
    //=============================================================================
    CabbageEditorContainer (CabbageSettings* settings);
    ~CabbageEditorContainer();
    void updateLookAndFeel();
    void openFile (File file);
    void resized();

    void updateEditorColourScheme();
    void mouseDown (const MouseEvent& e);
    void mouseUp (const MouseEvent& e);
    void mouseExit (const MouseEvent& e);
    void mouseEnter (const MouseEvent& e);
    void mouseDrag (const MouseEvent& e);

    int getStatusBarPosition();
    ScopedPointer<CabbageCodeEditorComponent> editor;
    ScopedPointer<CabbageOutputConsole> outputConsole;
    //HorizontalResizerBar horizontalResizerBar;
    StatusBar statusBar;
    CodeDocument csoundDocument;
    CsoundTokeniser csoundTokeniser;
    CabbageSettings* settings;

private:
    bool fileNeedsSaving = false;
    int horizontalBarPosition = 0;
    const int statusBarHeight = 25;
    int startingDragPos;

};


#endif  // CabbageEditorContainer_H_INCLUDED
