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
#include "../Audio/Graph/AudioGraph.h"
#include "../Settings/CabbageSettings.h"
#include "CabbagePluginComponent.h"
#include "CabbageGraphComponent.h"
#include "FileTab.h"

class CabbageDocumentWindow;
class FileTab;

class CabbageMainComponent
    : public Component,
      public Button::Listener,
      public ActionListener,
      public ChangeListener,
      public Timer,
      public ComboBox::Listener,
	  public FileDragAndDropTarget
{
public:

    //==============================================================================
    CabbageMainComponent (CabbageDocumentWindow* owner, CabbageSettings* settings);
    ~CabbageMainComponent();
    //==============================================================================
    void changeListenerCallback (ChangeBroadcaster* source);
    void actionListenerCallback (const String& message);
    void buttonClicked (Button* button);
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged);
    void updateCodeInEditor (CabbagePluginEditor* pluginEditor, bool replaceExistingLine);
    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void resizeAllWindows (int height);
    void createEditorForAudioGraphNode (Point<int> position);
    void createAudioGraph();
    void createCodeEditorForFile (File file);
    void createNewProject();
	bool isInterestedInFileDrag(const StringArray &files);
	void filesDropped(const StringArray &files, int x, int y);
    Image createBackground();
    void removeEditor();
    //==============================================================================
    void launchSSHFileBrowser (String mode);
    void setEditMode (bool enable);
    const File openFile (String filename = "");
    void closeDocument();
    void showSettingsDialog();
    void saveDocument (bool saveAs = false, bool recompile = true);
    void runCsoundForNode (String file);
    void stopCsoundForNode (String file);
    void stopAudioGraph();
    void startAudioGraph();
    void bringCodeEditorToFront (File file);
    void updateEditorColourScheme();
    void addInstrumentsAndRegionsToCombobox();
    void setLookAndFeelColours();
    void showGraph();
    void saveGraph (bool saveAs);
    void openGraph (File fileToOpen = File());
    File getCurrentCsdFile ();
    void setCurrentCsdFile (File file);
    void writeFileToDisk (File file);
	int testFileForErrors(String file);
    //==============================================================================
    void handleToolbarButtons (ToolbarButton* toolbarButton);
    void handleFileTabs (DrawableButton* button);
    void handleFileTab (FileTab* drawableButton) ;
    void addFileTab (File file);
    void arrangeFileTabs();
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
    CabbagePluginEditor* getCabbagePluginEditor();
    CabbagePluginProcessor* getCabbagePluginProcessor();
    CabbageOutputConsole* getCurrentOutputConsole();
    CabbageCodeEditorComponent* getCurrentCodeEditor();
    CabbageEditorContainer* getCurrentEditorContainer();
    String getAudioDeviceSettings();
    int getStatusbarYPos();
    CabbageSettings* getCabbageSettings() {      return cabbageSettings; }
    AudioGraph* getAudioGraph() {                return audioGraph;  }
    NamedValueSet& getNodeIds() {                    return nodeIdsForPlugins;   }
    //==============================================================================
    ScopedPointer<CabbagePropertiesPanel> propertyPanel;
    OwnedArray<CabbageEditorContainer> editorAndConsole;
    OwnedArray<WebBrowserComponent> htmlHelpPages;
    ScopedPointer<CabbageIDELookAndFeel> lookAndFeel;
    Toolbar toolbar;
    //==============================================================================
    void timerCallback();
    void launchHelpfile (String type);
	TextButton cycleTabsButton;

private:
	int getTabFileIndex(File file);
    OwnedArray<FileTab> fileTabs;
    bool fileNeedsSaving = false;
    String searchString = String::empty;
    String replaceString = String::empty;
    bool isCaseSensitive = false;
    File tempFile;
    CabbageDocumentWindow* owner;
    //ScopedPointer<CabbageIDELookAndFeel> lookAndFeel;
    CabbageToolbarFactory factory;
    Image bgImage;
    //File currentCsdFile;
    const int statusBarHeight = 25;
    CabbageSettings* cabbageSettings;
    int currentFileIndex = 0;
    int numberOfFiles = 0;
    ScopedPointer<AudioGraph> audioGraph;
    ScopedPointer<CabbageGraphComponent> graphComponent;
    bool isGUIEnabled = false;
    String consoleMessages;
    const int toolbarThickness = 35;
    class FindPanel;
    ScopedPointer<FindPanel> findPanel;
    TooltipWindow tooltipWindow;
    NamedValueSet nodeIdsForPlugins;


    class AudioGraphDocumentWindow : public DocumentWindow
    {
        Colour colour;
    public:
        AudioGraphDocumentWindow (String caption, Colour backgroundColour)
            : DocumentWindow (caption, backgroundColour, DocumentWindow::TitleBarButtons::allButtons), colour (backgroundColour)
        {
            setSize (600, 600);
            setName (caption);
            this->setResizable (false, true);
        }

        void closeButtonPressed() override {    setVisible (false);  }
        void paint (Graphics& g) { g.fillAll (colour); }
    };

    ScopedPointer<AudioGraphDocumentWindow> audioGraphWindow;

    class HtmlHelpDocumentWindow : public DocumentWindow
    {
        Colour colour;
		
		class HtmlHelpBrowser : public WebBrowserComponent, public ActionBroadcaster
		{
		public:
			HtmlHelpBrowser():WebBrowserComponent()	{}
			bool pageAboutToLoad(const String &newURL) override
			{
				if(newURL.contains(".csd"))
				{
					sendActionMessage(newURL);
					return false;
				}
				else
					return true;
			}
		};
			
		
        ScopedPointer<HtmlHelpBrowser> htmlPage;
    public:
        HtmlHelpDocumentWindow (String caption, Colour backgroundColour)
            : DocumentWindow (caption, backgroundColour, DocumentWindow::TitleBarButtons::allButtons), colour (backgroundColour)
        {
            setName (caption);
            htmlPage = new HtmlHelpBrowser();
            htmlPage->setSize (1000, 800);
            this->setResizable (false, true);
            setContentOwned (htmlPage, true);
            setSize (1000, 800);
        }

		HtmlHelpBrowser* getHtmlHelpBrowser(){	return htmlPage;	}
		
        void loadPage (String url)
        {
            htmlPage->goToURL (url);
        }

        void closeButtonPressed() override {    setVisible (false);  }

    };

    ScopedPointer<HtmlHelpDocumentWindow> helpWindow;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabbageMainComponent)
};

//==============================================================================
class CabbageMainComponent::FindPanel  : public Component,
    private TextEditor::Listener,
    private ButtonListener
{
public:
    FindPanel (String searchString, bool isCaseSensitive, bool withReplace)
        : caseButton ("Case-sensitive"),
          findPrev ("<"),
          findNext (">"),
          replace ("Replace"),
          replaceAll ("Replace All"),
          showReplaceControls (withReplace)
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
