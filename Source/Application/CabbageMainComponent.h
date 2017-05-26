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

class CabbageDocumentWindow;
class FileTabButton;

class CabbageMainComponent
    : public Component,
      public Button::Listener,
      public ActionListener,
      public ChangeListener,
      public Timer,
      public ComboBox::Listener
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
    void createGenericCsoundPluginWrapper();
    Image createBackground();
    void removeEditor();
    //==============================================================================
    void showAudioSettings();
    void launchSSHFileBrowser (String mode);
    void setEditMode (bool enable);
    const File openFile (String filename = "");
    bool closeAllDocuments (bool askUserToSave);
    void closeDocument();
    bool closeAllMainWindows();
    void showSettingsDialog();
    void saveDocument (bool saveAs = false, bool recompile = true);
    void runCsoundForNode (String file);
    void stopCsoundForNode (String file);
    void stopAudioGraph();
    void startAudioGraph();
    void showGenericWidgetWindow();
    void bringCodeEditorToFront (File file);
    void hideGenericWidgetWindow (bool freeContent = false);
    void initSettings();
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
    void handleFileTabButtons (DrawableButton* button);
    void handleFileTab (FileTabButton* drawableButton) ;
    void addFileTabButton (File file);
    void arrangeFileTabButtons();
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

private:
    OwnedArray<FileTabButton> fileTabs;
    Array<File> openFiles;
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
        ScopedPointer<WebBrowserComponent> htmlPage;
    public:
        HtmlHelpDocumentWindow (String caption, Colour backgroundColour)
            : DocumentWindow (caption, backgroundColour, DocumentWindow::TitleBarButtons::allButtons), colour (backgroundColour)
        {
            setName (caption);
            htmlPage = new WebBrowserComponent();
            htmlPage->setSize (1000, 800);
            this->setResizable (false, true);
            setContentOwned (htmlPage, true);
            setSize (1000, 800);
        }

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

class FileTabButton : public TextButton
{
    DrawableButton play, close, showEditor, editGUI;
    String filename;
    bool isCsdFile;

    class Overlay : public Component
    {
    public:
        Overlay(): Component() {}
        void paint (Graphics& g)
        {
            g.fillAll (Colours::black.withAlpha (.5f));
        }
    };

    Overlay overlay;
public:


    FileTabButton (String name, String filename, bool csdFile = true):
        TextButton (name, filename),
        filename (filename),
        play ("Play", DrawableButton::ButtonStyle::ImageStretched),
        close ("", DrawableButton::ButtonStyle::ImageStretched),
        showEditor ("", DrawableButton::ButtonStyle::ImageStretched),
        editGUI ("", DrawableButton::ButtonStyle::ImageStretched),
        overlay(),
        isCsdFile (csdFile)
    {
        addChildComponent (overlay);
        overlay.setVisible (false);
        play.setClickingTogglesState (true);
        play.setName ("playButton");

        addAndMakeVisible (close);
        close.setName ("closeButton");
        close.setColour (DrawableButton::ColourIds::backgroundColourId, Colours::transparentBlack);
        close.setColour (DrawableButton::ColourIds::backgroundOnColourId, Colours::transparentBlack);
        close.setTooltip ("Close file");
        close.getProperties().set ("filename", filename);

        addAndMakeVisible (play);
        play.setColour (DrawableButton::ColourIds::backgroundColourId, Colours::transparentBlack);
        play.setColour (DrawableButton::ColourIds::backgroundOnColourId, Colours::transparentBlack);
        play.setClickingTogglesState (true);
        play.getProperties().set ("filename", filename);

        addAndMakeVisible (showEditor);
        showEditor.setName ("showEditorButton");
        showEditor.setColour (DrawableButton::ColourIds::backgroundColourId, Colours::transparentBlack);
        showEditor.setColour (DrawableButton::ColourIds::backgroundOnColourId, Colours::transparentBlack);
        showEditor.setClickingTogglesState (true);
        showEditor.setTooltip ("Show plugin Editor");

        addAndMakeVisible (editGUI);
        editGUI.setName ("editGUIButton");
        editGUI.setColour (DrawableButton::ColourIds::backgroundColourId, Colours::transparentBlack);
        editGUI.setColour (DrawableButton::ColourIds::backgroundOnColourId, Colours::transparentBlack);
        editGUI.setClickingTogglesState (true);
        editGUI.setTooltip ("Edit Plugin GUI");

        setDrawableImages (play, 60, 25, "play");
        setDrawableImages (close, 25, 25, "close");
        setDrawableImages (showEditor, 25, 25, "showEditor");
        setDrawableImages (editGUI, 25, 25, "editGUI");
        
        


    }

    const String getFilename() { return filename;    }

//    void paintButton(Graphics &g, bool isMouseOverButton, bool isButtonDown)
//    {
//        g.setColour(Colour(20, 20, 20));
//        g.fillRoundedRectangle(0, 0, getWidth(), getHeight()+10, 4);
//        g.setColour(Colour(80, 80, 80));
//        g.drawRoundedRectangle(0, 0, getWidth(), getHeight()+10, 4, 4);
//    }
    
    void addButtonListeners (Button::Listener* listener)
    {
        play.addListener (listener);
        close.addListener (listener);
        showEditor.addListener (listener);
        editGUI.addListener (listener);
    }

    void disableButtons (bool disable)
    {
        if (disable)
        {
            overlay.setVisible (true);
            overlay.toFront (true);
        }
        else
            overlay.setVisible (false);

    }

    void resized()
    {
        if (isCsdFile == true)
        {
            overlay.setBounds (5, 3, 125, 25);
            play.setBounds (5, 3, 60, 25);
            showEditor.setBounds (67, 3, 30, 25);
            editGUI.setBounds (99, 3, 30, 25);
        }

        close.setBounds (getWidth() - 22, 3, 20, 20);
    }

    void setDrawableImages (DrawableButton& button, int width, int height, String type)
    {
        DrawableImage imageNormal, imageNormalPressed, imageDownPressed;

        if (type == "play")
        {
            DrawableImage imageDown;
            imageNormalPressed.setImage (CabbageImages::drawPlayStopIcon (width, height, false, true));
            imageDownPressed.setImage (CabbageImages::drawPlayStopIcon (width, height, true, true));
            imageNormal.setImage (CabbageImages::drawPlayStopIcon (width, height, false));
            imageDown.setImage (CabbageImages::drawPlayStopIcon (width, height, true));
            button.setImages (&imageNormal, &imageNormal, &imageNormalPressed, &imageNormal, &imageDown, nullptr,  &imageDownPressed, &imageDownPressed);
        }
        else if (type == "close")
        {
            imageNormal.setImage (CabbageImages::drawCloseIcon (width, height));
            imageNormalPressed.setImage (CabbageImages::drawCloseIcon (width - 3, height - 3));
            button.setImages (&imageNormal, &imageNormal, &imageNormalPressed, &imageNormal, &imageNormal);
        }
        else if (type == "showEditor")
        {
            imageNormal.setImage (CabbageImages::drawEditorIcon (width, height));
            imageNormalPressed.setImage (CabbageImages::drawEditorIcon (width - 1, height - 1));
            button.setImages (&imageNormal, &imageNormal, &imageNormalPressed, &imageNormal, &imageNormal, nullptr,  &imageNormalPressed, &imageNormalPressed);
        }
        else if (type == "editGUI")
        {
            imageNormal.setImage (CabbageImages::drawEditGUIIcon (width, height));
            imageNormalPressed.setImage (CabbageImages::drawEditGUIIcon (width - 1, height - 1));
            button.setImages (&imageNormal, &imageNormal, &imageNormalPressed, &imageNormal, &imageNormal, nullptr,  &imageNormalPressed, &imageNormalPressed);
        }
    }

    DrawableButton& getPlayButton() {    return play;    }
    DrawableButton& getShowEditorButton() {  return showEditor;  }
    DrawableButton& getCloseFileEditorButton() { return close;   }
    DrawableButton& getEditGUIButton() { return editGUI; }
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
