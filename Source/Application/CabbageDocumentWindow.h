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

#ifndef CABBAGEMAINWINDOW_H_INCLUDED
#define CABBAGEMAINWINDOW_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "../CodeEditor/EditorAndConsoleContentComponent.h"
#include "../BinaryData/CabbageBinaryData.h"
#include "../InterfaceEditor/CabbagePropertiesPanel.h"
#include "../CabbageIds.h"
#include "CabbageContentComponent.h"
#include "../CodeEditor/CabbageOutputConsole.h"
#include "../Settings/CabbageSettingsWindow.h"
#include "../Settings/CabbageSettings.h"




class CabbageDocumentWindow
    : public DocumentWindow,
      public ApplicationCommandTarget,
      public MenuBarModel
{
public:
    //==========================================================
    CabbageDocumentWindow (String name);
    ~CabbageDocumentWindow();
    CabbageContentComponent* getContentComponent();
    //=======================================================
    StringArray getMenuBarNames();
    void createMenu (PopupMenu&, const String& menuName);
    void createFileMenu (PopupMenu&);
    void createEditMenu (PopupMenu&);
    void createViewMenu (PopupMenu&);
    void createBuildMenu (PopupMenu&);
    void createColourSchemeItems (PopupMenu&);
    void createWindowMenu (PopupMenu&);
    void createToolsMenu (PopupMenu&);
    void getAllCommands (Array<CommandID>&) override;
    void getCommandInfo (CommandID commandID, ApplicationCommandInfo&) override;
    bool perform (const InvocationInfo&) override;
    PopupMenu getMenuForIndex(int topLevelMenuIndex, const String& menuName);
    void menuItemSelected (int menuItemID, int topLevelMenuIndex);
    //=======================================================
    bool closeAllDocuments (bool askUserToSave);
    bool closeAllMainWindows();
    void initSettings();
    void closeButtonPressed() override;
    void buttonClicked(Button* button);
    void focusGained (FocusChangeType cause); //grab focus when user clicks on editor

    ScopedPointer<CabbageSettings> cabbageSettings;
    ApplicationCommandTarget* getNextCommandTarget()
    {
        return findFirstTargetParentComponent();
    }

private:
    ApplicationCommandManager commandManager;
    bool isGUIEnabled = false;
    ScopedPointer<CabbageContentComponent> content;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabbageDocumentWindow)
};

#endif