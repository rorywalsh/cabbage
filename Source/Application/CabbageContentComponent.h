/*
  Copyright (C) 2016 Rory Walsh

  Cabbage is free software; you can redistribute it
  and/or modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  Cabbage is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with Csound; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
  02111-1307 USA
*/

#ifndef CABBAGECONTENT_H_INCLUDED
#define CABBAGECONTENT_H_INCLUDED


#include "../JuceLibraryCode/JuceHeader.h"
#include "../CodeEditor/EditorAndConsoleContentComponent.h"
#include "../BinaryData/CabbageBinaryData.h"
#include "../InterfaceEditor/CabbagePropertiesPanel.h"
#include "../CabbageIds.h"
#include "CabbageToolbarFactory.h"



//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class CabbageContentComponent   : public Component, public Button::Listener
{
public:

    //==============================================================================
    CabbageContentComponent(ValueTree settings);
    ~CabbageContentComponent();

    void paint (Graphics&) override;
    void resized() override;
    Image createBackground();
    void openFile(File file);
    OwnedArray<EditorAndConsoleContentComponent> editorAndConsole;
    void resizeAllEditorAndConsoles(int height);
    ScopedPointer<CabbagePropertiesPanel> propertyPanel;
    OwnedArray<TextButton> fileTabs;
    Array<File> openFiles;
    void buttonClicked(Button* button);
    void addFileTabButton(File file, int xPos);
    CabbageIDELookAndFeel lookAndFeel;
    EditorAndConsoleContentComponent* getCurrentCodeEditor();
	Toolbar toolbar;

private:
	
	CabbageToolbarFactory factory;
    Image bgImage;
    const int statusBarHeight = 25;
    ValueTree settings;
    int currentFileIndex = 0;
    int numberOfFiles = 0;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabbageContentComponent)
};



#endif  // CABBAGECONTENT_H_INCLUDED
