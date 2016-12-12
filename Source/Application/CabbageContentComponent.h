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

#ifndef CABBAGECONTENT_H_INCLUDED
#define CABBAGECONTENT_H_INCLUDED


#include "../JuceLibraryCode/JuceHeader.h"
#include "../CodeEditor/EditorAndConsoleContentComponent.h"
#include "../BinaryData/CabbageBinaryData.h"
#include "../InterfaceEditor/CabbagePropertiesPanel.h"
#include "../CabbageIds.h"
#include "CabbageToolbarFactory.h"
#include "../Audio/Graph/AudioGraph.h"
#include "../Settings/CabbageSettings.h"

class CabbageDocumentWindow;


class CabbageContentComponent   
	: public Component, 
	public Button::Listener,
	public ChangeListener,
	public Timer,
	public ComboBox::Listener
{
public:

    //==============================================================================
    CabbageContentComponent(CabbageDocumentWindow* owner, CabbageSettings* settings);
    ~CabbageContentComponent();
	//==============================================================================
    void changeListenerCallback(ChangeBroadcaster* source);
    void buttonClicked(Button* button);
	void comboBoxChanged (ComboBox *comboBoxThatHasChanged);
    void updateCodeInEditor(CabbagePluginEditor* pluginEditor, bool replaceExistingLine);
	//==============================================================================
    void paint (Graphics&) override;
    void resized() override;  
    void resizeAllEditorAndConsoles(int height);	
	void createEditorForAudioGraphNode();
    void createAudioGraph();
	void rebuildAudioGraph();
    void addFileTabButton(File file, int xPos);
    Image createBackground();
	//==============================================================================
	void showAudioSettings();
    void createNewProject();
	void setEditMode(bool enable);
    void openFile(String filename="");
    bool closeAllDocuments (bool askUserToSave);
    bool closeAllMainWindows();
    void showSettingsDialog();
    void saveDocument(bool saveAs=false);
    void runCode();
    void stopCode();
    void showGenericWidgetWindow();
    void hideGenericWidgetWindow(bool freeContent=false);
    void createGenericCsoundPluginWrapper();
    void initSettings();
	void updateEditorColourScheme();
	void addInstrumentsAndRegionsToCombobox();
	
	void setLookAndFeelColours();
	//==============================================================================
    CabbagePluginEditor* getCabbagePluginEditor();
	CabbagePluginProcessor* getCabbagePluginProcessor();
    CabbageOutputConsole* getCurrentOutputConsole();
    CabbageCodeEditorComponent* getCurrentCodeEditor();
	//==============================================================================
	String getAudioDeviceSettings();
	void timerCallback();
	
    ScopedPointer<CabbagePropertiesPanel> propertyPanel;
    OwnedArray<TextButton> fileTabs;
    Array<File> openFiles;
	OwnedArray<EditorAndConsoleContentComponent> editorAndConsole;
	ScopedPointer<CabbageIDELookAndFeel> lookAndFeel;
    EditorAndConsoleContentComponent* getCurrentEditorAndConsole();
	Toolbar toolbar;

	bool setCurrentCsdFile(File file);
	
private:
	CabbageDocumentWindow* owner;
	//ScopedPointer<CabbageIDELookAndFeel> lookAndFeel;
	CabbageToolbarFactory factory;
    Image bgImage;
	File currentCsdFile;
    const int statusBarHeight = 25;
    CabbageSettings* cabbageSettings;
    int currentFileIndex = 0;
    int numberOfFiles = 0;
	ScopedPointer<AudioGraph> audioGraph;
    bool isGUIEnabled = false;
    String consoleMessages;
    const int toolbarThickness = 35;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabbageContentComponent)
};



#endif  // CABBAGECONTENT_H_INCLUDED
