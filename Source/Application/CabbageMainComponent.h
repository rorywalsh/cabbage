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
#include "../CodeEditor/CabbageEditorContainer.h"
#include "../BinaryData/CabbageBinaryData.h"
#include "../GUIEditor/CabbagePropertiesPanel.h"
#include "../CabbageIds.h"
#include "CabbageToolbarFactory.h"
#include "../Audio/Filters/FilterGraph.h"
#include "../Audio/UI/GraphEditorPanel.h"
#include "../Settings/CabbageSettings.h"
//#include "CabbagePluginComponent.h"
//#include "CabbageGraphComponent.h"
#include "FileTab.h"
#include "../Audio/Plugins/CabbagePluginProcessor.h"
#include "../Audio/Plugins/CabbagePluginEditor.h"
#include "../Audio/Plugins/GenericCabbagePluginProcessor.h"
#include "../Audio/Plugins/CabbageInternalPluginFormat.h"
#include "../Utilities/CabbagePluginList.h"

class CabbageDocumentWindow;
class FileTab;



class CabbageMainComponent
    : public Component,
      public Button::Listener,
      public ActionListener,
      public ChangeListener,
      public Timer,
      public ComboBox::Listener,
      public FileDragAndDropTarget,
      public FileBrowserListener
{
public:

    void mouseDown (const MouseEvent& e) override;
    void mouseUp (const MouseEvent& e) override;
    void mouseExit (const MouseEvent& e) override;
    void mouseEnter (const MouseEvent& e) override;
    void mouseDrag (const MouseEvent& e) override;
 

    //==============================================================================
    CabbageMainComponent (CabbageDocumentWindow* owner, CabbageSettings* settings);
    ~CabbageMainComponent();
    //==============================================================================
    void changeListenerCallback (ChangeBroadcaster* source) override;
    void actionListenerCallback (const String& message) override;
    void buttonClicked (Button* button) override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    void updateCodeInEditor (CabbagePluginEditor* pluginEditor, bool replaceExistingLine, bool guiPropUpdate =false);
    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void resizeAllWindows (int height);
    void createEditorForFilterGraphNode (Point<int> position);
    void createFilterGraph();
    void createCodeEditorForFile (File file);
    void createNewProject();
    void createNewTextFile(String contents = "");
    bool isInterestedInFileDrag (const StringArray& files) override;
    void filesDropped (const StringArray& files, int x, int y) override;
    Image createBackground();
    void removeEditor();
    //==============================================================================
    void launchSSHFileBrowser (String mode);
    void enableEditMode();
    const File openFile (String filename = "", bool updateRecentFiles = true);
    void closeDocument();
    void showSettingsDialog();
    void saveDocument (bool saveAs = false, bool recompile = true);
    void runCsoundForNode (String file, Point<int> pos = Point<int>(-1000, -1000));
    void stopCsoundForNode (String file);
    void stopFilterGraph();
    void startFilterGraph();
    void bringCodeEditorToFront (File file);
    void bringCodeEditorToFront (FileTab* tab);
    void updateEditorColourScheme();
    void addInstrumentsAndRegionsToCombobox();
    void insertCustomPlantToEditor(CabbagePluginEditor* editor);
    void setLookAndFeelColours();
    void showGraph();
    void showPluginListEditor();
    void saveGraph (bool saveAs);
    void openGraph (File fileToOpen = File());
    File getCurrentCsdFile ();
    void setCurrentCsdFile (File file);
    void writeFileToDisk (File file);
    int testFileForErrors (String file);
    int getCurrentFileIndex(){  return currentFileIndex;    }

    //==============================================================================
    void handleToolbarButtons (ToolbarButton* toolbarButton);
    void handleFileTabs (DrawableButton* button);
    void handleFileTab (FileTab* drawableButton, bool icrement = false) ;
    void addFileTab (File file);
    void arrangeFileTabs();
	void importTheme();
	void exportTheme();

    int getNumberOfFileTabs() {     return fileTabs.size();  };
    FileTab* getFileTab(int index){  return fileTabs[index]; };
    FileTab* getFileTabForNodeId(AudioProcessorGraph::NodeID nodeId)
    {
        for (auto &fileTab : fileTabs)
        {
            if(fileTab->uniqueFileId == nodeId.uid)
                return fileTab;
        }
        jassertfalse;
        return nullptr;
    }
    
    //overlaying this component on FileBrowserComponent to take contorl of up button colour..
    class GoUpButton : public Component
    {
    public:
        GoUpButton() : Component(""), upArrowColour(160, 160, 160){
            setInterceptsMouseClicks(false, true);
        };
        ~GoUpButton(){};
        
        void paint(Graphics& g)
        {
            g.fillAll(Colours::transparentBlack);
            Path arrowPath;
            arrowPath.addArrow({ 23.0f, 16.0f, 23.0f, 5.0f }, 5.0f, 13.0f, 5.0f);
            g.setColour(upArrowColour);
            g.fillPath(arrowPath);
        }
        
        Colour upArrowColour;
    };

    GoUpButton goUpButton;
    
	void selectionChanged() override {};
    void fileClicked (const File &file, const MouseEvent &e) override;
    void fileDoubleClicked (const File &file) override;
	void browserRootChanged(const File &newRoot) override;
    //==============================================================================
    String getSearchString();
    void setSearchString (const String& s);
    void setReplaceString (const String& s);
    bool isCaseSensitiveSearch();
    void setCaseSensitiveSearch (bool b);
    void showFindPanel (bool withReplace);
    void hideFindPanel();
    int findNext (bool forward);
    void replaceText (bool replaceAll);

	//==============================================================================
	AudioDeviceManager deviceManager;
	AudioPluginFormatManager formatManager;
	Array<PluginDescription> internalTypes;
	KnownPluginList knownPluginList;
	KnownPluginList::SortMethod pluginSortMethod;
	String getDeviceManagerSettings();
	void reloadAudioDeviceState();

   
    class PluginListWindow;
    std::unique_ptr<PluginListWindow> pluginListWindow;

    //==============================================================================
    CabbagePluginEditor* getCabbagePluginEditor();
    CabbagePluginProcessor* getCabbagePluginProcessor();
    CabbageOutputConsole* getCurrentOutputConsole();
    CabbageCodeEditorComponent* getCurrentCodeEditor();
    CabbageEditorContainer* getCurrentEditorContainer();

    int getStatusbarYPos();
    CabbageSettings* getCabbageSettings() {      return cabbageSettings; }
    FilterGraph* getFilterGraph() {                return graphComponent->graph.get();  }
    //==============================================================================
    std::unique_ptr<CabbagePropertiesPanel> propertyPanel;
	void togglePropertyPanel()
	{
		if (getCurrentCodeEditor())
		{
			propertyPanel->setVisible(!propertyPanel->isVisible());
			resized();
		}
	}
    
    CabbagePluginEditor* currentEditor = nullptr;
    OwnedArray<CabbageEditorContainer> editorAndConsole;
    std::unique_ptr<CabbageIDELookAndFeel> lookAndFeel;
    Toolbar toolbar;
    
    void openFolder();
    //==============================================================================
    void timerCallback() override;
    void launchHelpfile (String type);
    TextButton cycleTabsButton;
    int duplicationIndex = 0;

	class FilterGraphDocumentWindow : public DocumentWindow
	{
		Colour colour;
		CabbageMainComponent* owner;
	public:
		FilterGraphDocumentWindow(String caption, Colour backgroundColour, CabbageMainComponent* owner)
			: DocumentWindow(caption, backgroundColour, DocumentWindow::TitleBarButtons::allButtons), colour(backgroundColour), owner(owner)
		{
			setSize(600, 600);
			setName(caption);
			this->setTitleBarHeight(15);
			this->setResizable(true, true);
			
		}

		void closeButtonPressed() override { setVisible(false); }
		
		CabbageMainComponent* getOwner() {
			return owner;
		};

	};

    class VerticalResizerBar : public Component
    {
    public:
        VerticalResizerBar (ValueTree valueTree, CabbageMainComponent* parent)
        :   Component ("ResizerBar"),
        valueTree (valueTree)
        // owner (parent)
        {
            
        }
        
        void paint (Graphics& g)  override
        {
            g.fillAll(CabbageSettings::getColourFromValueTree (valueTree, CabbageColourIds::menuBarBackground, Colours::black));
        };
        
        int getCurrentYPos() {   return currentYPos; }
        
    private:
        ValueTree valueTree;
        // int startingYPos;
        int currentYPos = 550;
        // CabbageMainComponent* owner;
    };
    
    VerticalResizerBar resizerBar;
    int startingVBarDragPos = 195;
    int resizerBarCurrentXPos = 195;
    
    void toggleBrowser(); 
    
//    TimeSliceThread directoryThread{ "File Scanner Thread" };
     std::unique_ptr<WildcardFileFilter> wildcardFilter;
//    DirectoryContentsList fileList{ &wildcardFilter, directoryThread };
//    FileTreeComponent fileTree{ fileList };
    FileBrowserComponent fileTree;
//    WildcardFileFilter fileFilter;;
	bool shouldUpdateAudioSettings = false;

private:



    int getTabFileIndex (int32 nodeId);
    int getTabFileIndex (File file);
    OwnedArray<FileTab> fileTabs;
    bool fileNeedsSaving = false;
    String searchString = "";
    String replaceString = "";
    bool isCaseSensitive = false;
    File tempFile;
    CabbageDocumentWindow* owner;
    CabbageFoldersLookAndFeel lookAndFeel4;
    CabbageToolbarFactory factory;
    Image bgImage;
    //File currentCsdFile;
    const int statusBarHeight = 25;
    CabbageSettings* cabbageSettings;
    int currentFileIndex = 0;
    int numberOfFiles = 0;
    //std::unique_ptr<FilterGraph> filterGraph;
    bool isGUIEnabled = false;
    String consoleMessages;
    const int toolbarThickness = 35;
    class FindPanel;
    std::unique_ptr<FindPanel> findPanel;


    GraphDocumentComponent* graphComponent = nullptr;
    std::unique_ptr<FilterGraphDocumentWindow> filterGraphWindow;


    //std::unique_ptr<HtmlHelpDocumentWindow> helpWindow;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabbageMainComponent)
};

//==============================================================================
class CabbageMainComponent::FindPanel  : public Component,
    private TextEditor::Listener,
private Button::Listener
{
public:
    FindPanel (String searchString, bool isCaseSensitive, bool withReplace)
        : caseButton ("Case-sensitive"),
          showReplaceControls (withReplace),
          findPrev ("<"),
          findNext (">"),
          replace ("Replace"),
          replaceAll ("Replace All")
    {
        //find components...
        setSize (260, withReplace == false ? 90 : 180);
        findEditor.setColour (CaretComponent::caretColourId, Colours::black);

        addAndMakeVisible (findEditor);
        findEditor.setName ("findEditor");
        findLabel.setText ("Find:", dontSendNotification);
        findLabel.setColour (Label::textColourId, Colours::white);
        findLabel.attachToComponent (&findEditor, false);

        addAndMakeVisible (caseButton);
        caseButton.setColour (ToggleButton::textColourId, Colours::white);
        caseButton.setToggleState (isCaseSensitive, dontSendNotification);
        caseButton.addListener (this);

        findPrev.setConnectedEdges (Button::ConnectedOnRight);
        findNext.setConnectedEdges (Button::ConnectedOnLeft);
        addAndMakeVisible (findPrev);
        addAndMakeVisible (findNext);
        findNext.addListener (this);
        findPrev.addListener (this);

        setWantsKeyboardFocus (false);
        setFocusContainer (true);
        findPrev.setWantsKeyboardFocus (false);
        findNext.setWantsKeyboardFocus (false);

        findEditor.setText (searchString);
        findEditor.addListener (this);

        //replace components....
        replaceEditor.setColour (CaretComponent::caretColourId, Colours::black);

        addAndMakeVisible (replaceEditor);
        replaceEditor.setName ("replaceEditor");
        replaceLabel.setText ("Replace:", dontSendNotification);
        replaceLabel.setColour (Label::textColourId, Colours::white);
        replaceLabel.attachToComponent (&replaceEditor, false);

        replace.setConnectedEdges (Button::ConnectedOnRight);
        replaceAll.setConnectedEdges (Button::ConnectedOnLeft);
        addAndMakeVisible (replace);
        addAndMakeVisible (replaceAll);
        replace.addListener (this);
        replaceAll.addListener (this);

        setWantsKeyboardFocus (false);
        setFocusContainer (true);
        replace.setWantsKeyboardFocus (false);
        replaceAll.setWantsKeyboardFocus (false);
        replaceEditor.addListener (this);


    }

    void paint (Graphics& g) override
    {
        Path outline;
        outline.addRoundedRectangle (1.0f, 1.0f, getWidth() - 2.0f, getHeight() - 2.0f, 8.0f);

        g.setColour (Colours::black.withAlpha (0.6f));
        g.fillPath (outline);
        g.setColour (Colours::white.withAlpha (0.8f));
        g.strokePath (outline, PathStrokeType (1.0f));
    }

    void resized() override
    {
        int y = 30;
        findEditor.setBounds (10, y, getWidth() - 20, 24);
        y += 30;
        caseButton.setBounds (10, y, getWidth() / 2 - 10, 22);
        findNext.setBounds (getWidth() - 40, y, 30, 22);
        findPrev.setBounds (getWidth() - 70, y, 30, 22);

        y += 50;
        replaceEditor.setBounds (10, y, getWidth() - 20, 24);
        y += 30;
        replace.setBounds (10, y, (getWidth() / 2) - 5, 22);
        replaceAll.setBounds (getWidth() / 2 + 5, y, getWidth() / 2 - 15, 22);

    }

    void buttonClicked (Button* button) override
    {
        if (button->getName() == "Case-sensitive")
            getOwner()->setCaseSensitiveSearch (caseButton.getToggleState());
        else if (button->getName() == ">")
            getOwner()->findNext (true);
        else if (button->getName() == "<")
            getOwner()->findNext (false);
        else if (button->getName() == "Replace")
        {
            getOwner()->replaceText (false);
            getOwner()->findNext (true);
        }
        else if (button->getName() == "Replace All")
        {
            getOwner()->replaceText (true);
        }

    }

    void textEditorTextChanged (TextEditor& editor) override
    {
        getOwner()->setSearchString (findEditor.getText());
        getOwner()->setReplaceString (replaceEditor.getText());

        if (CabbageMainComponent* contentComp = getOwner())
        {
            if (editor.getName() == "findEditor")
                contentComp->findNext (true);
        }

    }

    void textEditorFocusLost (TextEditor&) override {}

    void textEditorReturnKeyPressed (TextEditor& editor) override
    {
        getOwner()->setSearchString (findEditor.getText());

        if (editor.getName() == "findEditor")
        {
            if (CabbageMainComponent* contentComp = getOwner())
            {
                contentComp->findNext (true);
            }
        }
    }

    void textEditorEscapeKeyPressed (TextEditor&) override
    {
        if (CabbageMainComponent* contentComp = getOwner())
            contentComp->hideFindPanel();
    }

    CabbageMainComponent* getOwner() const
    {
        return findParentComponentOfClass <CabbageMainComponent>();
    }

    TextEditor findEditor, replaceEditor;
    Label findLabel, replaceLabel;
    ToggleButton caseButton;
    bool showReplaceControls = false;
    TextButton findPrev, findNext, replace, replaceAll;
};


#endif  // CABBAGECONTENT_H_INCLUDED
