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
#ifndef CABBAGEMAINWINDOW_H_INCLUDED
#define CABBAGEMAINWINDOW_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "../CodeEditor/EditorAndConsoleContentComponent.h"
#include "../BinaryData/CabbageBinaryData.h"
#include "../InterfaceEditor/CabbagePropertiesPanel.h"
#include "../CabbageIds.h"
#include "CabbageContentComponent.h"
#include "../CodeEditor/CabbageOutputConsole.h"
#include "../Audio/Graph/AudioGraph.h"
#include "../Settings/CabbageSettingsWindow.h"
#include "../Settings/CabbageSettings.h"

class CabbageDocumentWindow    : public DocumentWindow,     public ApplicationCommandTarget,
    public MenuBarModel, public ChangeListener, public Timer
{	
public:
	//==========================================================	
    CabbageDocumentWindow (String name);
	~CabbageDocumentWindow();

	CabbagePluginEditor* getPluginEditor();
	CabbageOutputConsole* getCurrentOutputConsole();
	CabbageCodeEditorComponent* getCurrentCodeEditor();	
	CabbageContentComponent* getMainContentComponent();
	ScopedPointer<CabbageContentComponent> content;
	
	//=======================================================
	void changeListenerCallback(ChangeBroadcaster* source);	
	void updateCodeInEditor(CabbagePluginEditor* pluginEditor);
	//=======================================================
    StringArray getMenuBarNames();								// thesew method implementations 
    void createMenu (PopupMenu&, const String& menuName);		// are found in CabbageMainWindowMenu.cpp
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
	void showAudioSettings();
    void createNewProject();
    void askUserToOpenFile();
    bool openFile (const File&, String type="");
    bool closeAllDocuments (bool askUserToSave);
    bool closeAllMainWindows();
	void showSettingsDialog();
	void saveDocument(bool saveAs=false);
	void runCode();
	void stopCode();
	void showGenericWidgetWindow();
	void hideGenericWidgetWindow(bool freeContent=false);
	void createGenericCsoundPluginWrapper();
	void timerCallback();
	void setupSettingsFile();
	void createEditorForAudioGraphNode();
	void createAudioGraph();
	void setEditMode(bool enable);	
	String getAudioDeviceSettings();	
	void closeButtonPressed() override;
	void updateEditorColourScheme();
	
	ScopedPointer<CabbageSettings> cabbageSettings;
	
	
	void setCurrentCsdFile(File file)
	{
		currentCsdFile = file;
	}
	
	ApplicationCommandTarget* getNextCommandTarget()
    {
        return findFirstTargetParentComponent();
    }
	
private:	
	ApplicationCommandManager commandManager;
	ScopedPointer<CabbageIDELookAndFeel> lookAndFeel;
	bool isGUIEnabled = false;
	String consoleMessages;
	File currentCsdFile;
	ScopedPointer<AudioGraph> audioGraph;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabbageDocumentWindow)
};

#endif