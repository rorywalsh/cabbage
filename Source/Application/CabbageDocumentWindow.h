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

#include "JuceHeader.h"
#include "../CodeEditor/CabbageEditorContainer.h"
#include "../BinaryData/CabbageBinaryData.h"
#include "../GUIEditor/CabbagePropertiesPanel.h"
#include "../CabbageIds.h"
#include "CabbageMainComponent.h"
#include "../CodeEditor/CabbageOutputConsole.h"
#include "../Settings/CabbageSettingsWindow.h"
#include "../Settings/CabbageSettings.h"
#include "../Utilities/CabbageExportPlugin.h"



class CabbageDocumentWindow
    : public DocumentWindow,
      public ApplicationCommandTarget,
      public MenuBarModel
{
public:
    //==========================================================
    CabbageDocumentWindow (String name, String commandLineArgs);
    ~CabbageDocumentWindow();
    CabbageMainComponent* getContentComponent();
    //=======================================================
    StringArray getMenuBarNames() override;
    void createFileMenu (PopupMenu&);
    void createEditMenu (PopupMenu&);
    void createViewMenu (PopupMenu&);
    void createHelpMenu (PopupMenu&);
    void createToolsMenu (PopupMenu&);
    PopupMenu createFontMenu();
    void getAllCommands (Array<CommandID>&) override;
    void getCommandInfo (CommandID commandID, ApplicationCommandInfo&) override;
    bool perform (const InvocationInfo&) override;
    PopupMenu getMenuForIndex (int topLevelMenuIndex, const String& menuName) override;
    void menuItemSelected (int menuItemID, int topLevelMenuIndex) override;
    //=======================================================
    void initSettings();
    void closeButtonPressed() override;
    //void maximiseButtonPressed() override;
    void focusGained (FocusChangeType cause) override; //grab focus when user clicks on editor

    std::unique_ptr<CabbageSettings> cabbageSettings;
    ApplicationCommandTarget* getNextCommandTarget() override   {        return findFirstTargetParentComponent();    }
    ApplicationCommandManager& getCommandManager() {     return commandManager;  }
    void exportExamplesToPlugins(String type);
    
    void openFile(String file)
    {
        if(content)
            content->openFile(file);
    }
    
private:
    Array<Font> fonts;
    //=======================================================
    ApplicationCommandManager commandManager;
    PopupMenu createExamplesMenu();
    Array<File> exampleFiles;
    const String getPluginInfo (File csdFile, String info);
    void exportPlugin (String type, File csdFile);
    int setUniquePluginId (File binFile, File csdFile);
    long cabbageFindPluginId (unsigned char* buf, size_t len, const char* s);
    void writePluginFileToDisk (File fc, File csdFile, File VSTData, String ext);
    //=======================================================

    PluginExporter pluginExporter;
    std::unique_ptr<FlatButtonLookAndFeel> lookAndFeel;
    String commandLineArgs = "";
    bool isGUIEnabled = false;
    CabbageMainComponent* content = nullptr;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabbageDocumentWindow)
};

#endif
