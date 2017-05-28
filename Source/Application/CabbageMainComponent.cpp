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

#include "CabbageMainComponent.h"
#include "CabbageDocumentWindow.h"
#include "../Utilities/CabbageNewProjectWindow.h"
#include "../Utilities/CabbageSSHFileBrowser.h"
#include "../Utilities/CabbageStrings.h"

//==============================================================================
CabbageMainComponent::CabbageMainComponent (CabbageDocumentWindow* owner, CabbageSettings* settings)
    : cabbageSettings (settings),
      owner (owner),
      factory (this),
      tooltipWindow (this, 300)
{
    getLookAndFeel().setColour (TooltipWindow::ColourIds::backgroundColourId, Colours::whitesmoke);
    addAndMakeVisible (propertyPanel = new CabbagePropertiesPanel (cabbageSettings->valueTree));
    propertyPanel->setVisible (false);
    setSize (1200, 800);

    bgImage = createBackground();
    audioGraphWindow = new AudioGraphDocumentWindow ("Cabbage Patcher", Colours::white);
    audioGraphWindow->setVisible (false);

    addAndMakeVisible (toolbar);
    toolbar.addDefaultItems (factory);
    propertyPanel->addChangeListener (this);
    factory.combo->getCombo().addListener (this);

    setLookAndFeelColours();
    createAudioGraph(); //set up graph even though no file is selected. Allows users to change audio devices from the get-go..
}

CabbageMainComponent::~CabbageMainComponent()
{
    editorAndConsole.clear();
    graphComponent = nullptr;
    audioGraph = nullptr;

    if (tempFile.existsAsFile())
        tempFile.deleteFile();
}

void CabbageMainComponent::paint (Graphics& g)
{
    if (editorAndConsole.size() == 0)
        g.drawImage (bgImage, getLocalBounds().toFloat());
    else
        g.fillAll ( CabbageSettings::getColourFromValueTree (cabbageSettings->valueTree, CabbageColourIds::lineNumberBackground, Colour (50, 50, 50)));

}

void CabbageMainComponent::setLookAndFeelColours()
{
    lookAndFeel = new CabbageIDELookAndFeel();
    lookAndFeel->setColour (Slider::ColourIds::thumbColourId, Colour (110, 247, 0));
    lookAndFeel->setColour (ScrollBar::backgroundColourId, Colour (70, 70, 70));
    toolbar.setColour (Toolbar::backgroundColourId, CabbageSettings::getColourFromValueTree (cabbageSettings->getValueTree(), CabbageColourIds::menuBarBackground, Colour (50, 50, 50)));
    toolbar.setColour (Toolbar::ColourIds::buttonMouseOverBackgroundColourId, CabbageSettings::getColourFromValueTree (cabbageSettings->getValueTree(), CabbageColourIds::menuBarBackground, Colour (50, 50, 50)).contrasting (.3f));
    toolbar.setColour (Toolbar::ColourIds::buttonMouseDownBackgroundColourId, CabbageSettings::getColourFromValueTree (cabbageSettings->getValueTree(), CabbageColourIds::menuBarBackground, Colour (50, 50, 50)).contrasting (.5f));
    owner->lookAndFeelChanged();
    lookAndFeel->refreshLookAndFeel (cabbageSettings->getValueTree());
    lookAndFeelChanged();
    toolbar.repaint();
}
//==============================================================================
void CabbageMainComponent::buttonClicked (Button* button)
{
    if (FileTabButton* tabButton = dynamic_cast<FileTabButton*> (button))
        handleFileTab (tabButton);
    else if (ToolbarButton* toolbarButton = dynamic_cast<ToolbarButton*> (button))
        handleToolbarButtons (toolbarButton);
    else if (DrawableButton* drawableButton = dynamic_cast<DrawableButton*> (button))
        handleFileTabButtons (drawableButton);
}

void CabbageMainComponent::handleFileTab (FileTabButton* tabButton)
{
    CabbageUtilities::debug (tabButton->getFilename());

    if (tabButton->getName().contains ("html"))
        jassert (false);

    currentFileIndex = fileTabs.indexOf (tabButton);
    editorAndConsole[currentFileIndex]->toFront (true);
    //getCurrentCodeEditor()->scrollToLine(10);

    //bringCodeEditorToFront(openFiles[currentFileIndex]);

    if (CabbageDocumentWindow* docWindow = this->findParentComponentOfClass<CabbageDocumentWindow>())
    {
        docWindow->setName (openFiles[currentFileIndex].getFullPathName());
        addInstrumentsAndRegionsToCombobox();
    }

    for ( auto button : fileTabs )
    {
        if (button != tabButton)
            button->disableButtons (true);
        else
            button->disableButtons (false);
    }
}

void CabbageMainComponent::bringCodeEditorToFront (File file)
{
    const int fileIndex = openFiles.indexOf (file);

    if (fileIndex >= 0)
    {
        fileTabs[fileIndex]->setToggleState (true, sendNotification);
    }
    else
        this->openFile (file.getFullPathName());
}

void CabbageMainComponent::handleToolbarButtons (ToolbarButton* toolbarButton)
{
    if (toolbarButton->getName() == "new")             createNewProject();
    else if (toolbarButton->getName() == "open")       openFile();
    else if (toolbarButton->getName() == "save")       saveDocument (false);
    else if (toolbarButton->getName() == "save as")    saveDocument (true);
    else if (toolbarButton->getName() == "settings")   showSettingsDialog();
    else if (toolbarButton->getName() == "cut")        getCurrentCodeEditor()->cut();
    else if (toolbarButton->getName() == "copy")       getCurrentCodeEditor()->copy();
    else if (toolbarButton->getName() == "paste")      getCurrentCodeEditor()->paste();
    else if (toolbarButton->getName() == "togglePlay")
    {
        if (toolbarButton->getToggleState())
            this->startAudioGraph();
        else
            this->stopAudioGraph();
    }
}

void CabbageMainComponent::handleFileTabButtons (DrawableButton* drawableButton)
{
    if (drawableButton->getName() == "playButton")
    {
        if (drawableButton->getToggleState() == true)
            saveDocument();
        else
            stopCsoundForNode (drawableButton->getProperties().getWithDefault ("filename", ""));

    }
    else if (drawableButton->getName() == "closeButton")
    {
        if (FileTabButton* tabButton = drawableButton->findParentComponentOfClass<FileTabButton>())
        {
            currentFileIndex = fileTabs.indexOf (tabButton);
            editorAndConsole[currentFileIndex]->toFront (true);
            closeDocument();
        }
    }
    else if (drawableButton->getName() == "showEditorButton")
    {
        if (FileTabButton* tabButton = drawableButton->findParentComponentOfClass<FileTabButton>())
        {
            const String filename = tabButton->getFilename();
            const int nodeId = nodeIdsForPlugins.getWithDefault (filename, -99);

            if (nodeId != -99)
            {
                const Point<int> pos (PluginWindow::getPositionOfCurrentlyOpenWindow (nodeId));
                createEditorForAudioGraphNode (pos);
            }
        }

    }
    else if (drawableButton->getName() == "editGUIButton")
    {
        this->saveDocument();
        setEditMode (true);
    }
}

void CabbageMainComponent::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    int index = comboBoxThatHasChanged->getSelectedId();
    const int lineToScrollTo = getCurrentCodeEditor()->instrumentsAndRegions.getValueAt (index - 1);
    getCurrentCodeEditor()->scrollToLine (lineToScrollTo);
}
//==============================================================================
void CabbageMainComponent::changeListenerCallback (ChangeBroadcaster* source)
{
    if (dynamic_cast<CabbageSettings*> (source)) // update lookandfeel whenever a user changes colour settings
    {
        lookAndFeel->refreshLookAndFeel (cabbageSettings->getValueTree());
        lookAndFeelChanged();
        owner->lookAndFeelChanged();
        updateEditorColourScheme();
    }

    else if (CabbagePluginEditor* editor = dynamic_cast<CabbagePluginEditor*> (source)) // update Cabbage code when user drags a widget around
    {
        propertyPanel->setVisible (true);
        propertyPanel->setEnabled (true);
        resized();
        ValueTree widgetData = editor->getValueTreesForCurrentlySelectedComponents()[0];
        propertyPanel->saveOpenessState();
        propertyPanel->updateProperties (widgetData);

        if (CabbageWidgetData::getNumProp (widgetData, CabbageIdentifierIds::linenumber) > 9999) //if widget was added in edit mode...
        {
            StringArray csdArray;
            csdArray.addLines (getCurrentCodeEditor()->getDocument().getAllContent());

            for ( int i = 0 ; i < csdArray.size() ; i++ )
            {
                if (csdArray[i].contains ("</Cabbage>")) //add new text just before the end of the Cabbage section of code
                {
                    CabbageWidgetData::setNumProp (widgetData, CabbageIdentifierIds::linenumber, i);
                    updateCodeInEditor (editor, false);
                    updateEditorColourScheme(); //keep look and feel updated..
                    return;
                }
            }
        }
        else
        {
            updateCodeInEditor (editor, true);
            updateEditorColourScheme(); //keep look and feel updated..
        }
    }

    else if (dynamic_cast<CabbagePropertiesPanel*> (source)) // update code when a user changes a property
    {
        if (CabbagePluginEditor* ed = getCabbagePluginEditor())
        {
            updateCodeInEditor (ed, true);
        }
    }


    else if (CabbageCodeEditorComponent* codeEditor = dynamic_cast<CabbageCodeEditorComponent*> (source)) // update code when a user changes a property
    {
        if (getCabbagePluginEditor() != nullptr && getCabbagePluginEditor()->isEditModeEnabled())
            this->getCabbagePluginProcessor()->updateWidgets (codeEditor->getAllText());
    }
}

void CabbageMainComponent::actionListenerCallback (const String& message)
{
    if (message.contains ("delete:"))
    {
        const int lineNumber = String (message.replace ("delete:", "")).getIntValue();
        getCurrentCodeEditor()->removeLine (lineNumber);
    }
}
//=======================================================================================
void CabbageMainComponent::updateCodeInEditor (CabbagePluginEditor* editor, bool replaceExistingLine)
{
    propertyPanel->addChangeListener (this);

    for (ValueTree wData : editor->getValueTreesForCurrentlySelectedComponents())
    {
        const int lineNumber = CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::linenumber);
        const String parent = CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::parentcomponent); // if widget has a parent don't highlight line

        const String currentLineText = getCurrentCodeEditor()->getLineText (lineNumber);

        const String macroText = CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::expandedmacrotext);

        String macroNames;

        for ( int i = 0 ; i < CabbageWidgetData::getProperty (wData, CabbageIdentifierIds::macronames).size() ; i++ )
            macroNames += CabbageWidgetData::getProperty (wData, CabbageIdentifierIds::macronames)[i].toString() + " ";


        const String newText = CabbageWidgetData::getCabbageCodeFromIdentifiers (wData, currentLineText, macroText);
        macroNames = macroNames.length() > 1 ? macroNames : "";

        getCurrentCodeEditor()->insertCode (lineNumber, newText + " " + macroNames, replaceExistingLine, parent.isEmpty());

    }
}
//==============================================================================
void CabbageMainComponent::timerCallback()
{

    if (openFiles.size() > 0)
    {
        int32 nodeId = int32 (nodeIdsForPlugins.getWithDefault (getCurrentCsdFile().getFullPathName(), -99));

        if (audioGraph->graph.getNodeForId (nodeId) != nullptr && audioGraph->graph.getNodeForId (nodeId)->getProcessor()->isSuspended() == true)
        {
            stopCsoundForNode ("");
            stopTimer();
        }

        if (getCurrentCsdFile().existsAsFile())
        {
            const String csoundOutputString = audioGraph->getCsoundOutput (nodeId);
            consoleMessages += csoundOutputString;

            if (csoundOutputString.length() > 0)
                getCurrentOutputConsole()->setText (csoundOutputString);

        }
    }
}
//==============================================================================
void CabbageMainComponent::updateEditorColourScheme()
{
    getLookAndFeel().setColour (PropertyComponent::ColourIds::backgroundColourId, CabbageSettings::getColourFromValueTree (cabbageSettings->getValueTree(), CabbageColourIds::propertyLabelBackground, Colour (50, 50, 50)));
    getLookAndFeel().setColour (PropertyComponent::ColourIds::labelTextColourId, CabbageSettings::getColourFromValueTree (cabbageSettings->getValueTree(), CabbageColourIds::propertyLabelText, Colour (50, 50, 50)));
    lookAndFeelChanged();
    propertyPanel->setBackgroundColour (CabbageSettings::getColourFromValueTree (cabbageSettings->getValueTree(), CabbageColourIds::consoleOutline, Colour (50, 50, 50)));
    propertyPanel->setBorderColour (CabbageSettings::getColourFromValueTree (cabbageSettings->getValueTree(), CabbageColourIds::consoleOutline, Colour (50, 50, 50)));

    if (getCurrentEditorContainer() != nullptr)
        getCurrentEditorContainer()->updateLookAndFeel();

    toolbar.setColour (Toolbar::backgroundColourId, CabbageSettings::getColourFromValueTree (cabbageSettings->getValueTree(), CabbageColourIds::menuBarBackground, Colour (50, 50, 50)));
    toolbar.repaint();
}
//==============================================================================
Image CabbageMainComponent::createBackground()
{
    Image backgroundImg;
    backgroundImg = Image (Image::RGB, getWidth(), getHeight(), true);
    ScopedPointer<Drawable> drawable;
    const int whiteScale = 150;
    Graphics g (backgroundImg);
    {
        g.fillAll (Colour (whiteScale, whiteScale, whiteScale));
        Random pos, width, colour;

        for (int i = 0; i < getWidth();)
        {
            const int brightness = colour.nextInt (Range<int> (whiteScale, whiteScale + 5));
            g.setColour (Colour (brightness, brightness, brightness));
            g.drawLine (i, 0, i + width.nextInt (Range<int> (0, 10)), getHeight() );
            g.drawLine (0, i, getWidth(), i + width.nextInt (Range<int> (0, 10)));
            i += pos.nextInt (Range<int> (0, 5));
        }

        const Image cabbageLogo = ImageCache::getFromMemory (CabbageBinaryData::CabbageLogoBig_png, CabbageBinaryData::CabbageLogoBig_pngSize);
        g.drawImage (cabbageLogo, getLocalBounds().toFloat(), RectanglePlacement::Flags::doNotResize);
        return backgroundImg;
    }
}

//==============================================================================
void CabbageMainComponent::addFileTabButton (File file)
{
    FileTabButton* fileButton;
    CabbageUtilities::debug (file.getFullPathName());
    fileTabs.add (fileButton = new FileTabButton (file.getFileName(), file.getFullPathName(), file.hasFileExtension (".html") ? false : true));


    addAndMakeVisible (fileButton);
    fileButton->addListener (this);
    fileButton->setRadioGroupId (99);
    fileButton->setClickingTogglesState (true);
    fileButton->setLookAndFeel (lookAndFeel);
    fileButton->setToggleState (true, dontSendNotification);
    currentFileIndex = fileTabs.size() - 1;
    fileButton->addButtonListeners (this);

    for ( auto button : fileTabs )
    {
        if (button != fileButton)
            button->disableButtons (true);
    }

}

void CabbageMainComponent::arrangeFileTabButtons()
{
    int xPos = 10;
    const int numTabs = jmax (3, fileTabs.size());

    for ( auto fileButton : fileTabs)
    {
        const int width = propertyPanel->isVisible() ? getWidth() - propertyPanel->getWidth() - 15 : getWidth() - 25;
        fileButton->setBounds (xPos, toolbarThickness + 3, width / numTabs, 30);
        xPos += width / numTabs + 4;
    }
}

//==============================================================================
void CabbageMainComponent::addInstrumentsAndRegionsToCombobox()
{
    factory.combo->clearItemsFromComboBox();

    const NamedValueSet instrRegions = getCurrentCodeEditor()->instrumentsAndRegions;

    for ( int i = 0 ; i < instrRegions.size() ; i++)
    {
        factory.combo->addItemsToComboBox (instrRegions.getName (i).toString(), i + 1);
    }
}
//==============================================================================
void CabbageMainComponent::resizeAllWindows (int height)
{
    const bool isPropPanelVisible = propertyPanel->isVisible();

    for ( CabbageEditorContainer* editor : editorAndConsole )
    {
        editor->statusBar.setSize (getWidth(), 28);
        editor->setBounds (0, height, getWidth() - (isPropPanelVisible ? 200 : 0), getHeight() - 5);
    }

    for ( WebBrowserComponent* browser : htmlHelpPages )
    {
        browser->setBounds (0, height, getWidth(), getHeight() - 5);
    }

    arrangeFileTabButtons();
}

//==============================================================================
void CabbageMainComponent::createAudioGraph()
{
    const Point<int> lastPoint = PluginWindow::getPositionOfCurrentlyOpenWindow (1);

    if (lastPoint.getX() > 0)
    {
        cabbageSettings->setProperty ("windowX", lastPoint.getX());
        cabbageSettings->setProperty ("windowY", lastPoint.getY());
    }

    audioGraph = new AudioGraph (*this, cabbageSettings->getUserSettings(), false);
    audioGraph->setXmlAudioSettings (cabbageSettings->getUserSettings()->getXmlValue ("audioSetup"));
    graphComponent = new CabbageGraphComponent (*audioGraph, *this);
    audioGraphWindow->setContentNonOwned (graphComponent, false);
}
//==================================================================================
void CabbageMainComponent::showGraph()
{
    audioGraphWindow->setUsingNativeTitleBar (true);
    audioGraphWindow->setVisible (true);
    audioGraphWindow->setTopLeftPosition (getWidth() - audioGraphWindow->getWidth(), 10);
    audioGraphWindow->setAlwaysOnTop (true);
}
//==============================================================================
void CabbageMainComponent::createEditorForAudioGraphNode (Point<int> position)
{

    int32 nodeId = int32 (nodeIdsForPlugins.getWithDefault (getCurrentCsdFile().getFullPathName(), -99));

    if (AudioProcessorGraph::Node::Ptr f = audioGraph->graph.getNodeForId (nodeId))
    {
        PluginWindow::WindowFormatType type = f->getProcessor()->hasEditor() ? PluginWindow::Normal
                                              : PluginWindow::Generic;

        if (PluginWindow* const w = PluginWindow::getWindowFor (f, type, audioGraph->graph))
        {
            w->toFront (true);

            const int alwaysOnTop = cabbageSettings->getUserSettings()->getIntValue ("SetAlwaysOnTopPlugin");

            if (alwaysOnTop == 1)
                w->setAlwaysOnTop (true);
            else
                w->setAlwaysOnTop (false);

            if (position.getY() > 0 && position.getX() > 0)
                w->setTopLeftPosition (position.getX(), position.getY());
        }
    }
}
//==============================================================================
CabbageEditorContainer* CabbageMainComponent::getCurrentEditorContainer()
{
    if (editorAndConsole.size() > 0)
        return editorAndConsole[currentFileIndex];

    return nullptr;
}
//==================================================================================
CabbageCodeEditorComponent* CabbageMainComponent::getCurrentCodeEditor()
{
    if (getCurrentEditorContainer())
        return getCurrentEditorContainer()->editor;
    else
        return nullptr;
}
//==================================================================================
CabbageOutputConsole* CabbageMainComponent::getCurrentOutputConsole()
{
    if (getCurrentEditorContainer())
        return getCurrentEditorContainer()->outputConsole;
    else
        return nullptr;
}
//==================================================================================
String CabbageMainComponent::getAudioDeviceSettings()
{
    if (audioGraph != nullptr && audioGraph->getDeviceManagerSettings().isNotEmpty())
        return audioGraph->getDeviceManagerSettings();
    else
        return String::empty;
}
//==================================================================================
CabbagePluginEditor* CabbageMainComponent::getCabbagePluginEditor()
{
    if (audioGraph != nullptr && nodeIdsForPlugins.size() > 0)
    {
        const int32 nodeId = int32 (nodeIdsForPlugins.getWithDefault (getCurrentCsdFile().getFullPathName(), -99));

        if (nodeId != 99)
            if (AudioProcessorGraph::Node::Ptr f = audioGraph->graph.getNodeForId (nodeId))
            {
                AudioProcessor* const processor = f->getProcessor();

                //need to check what kind of processor we are dealing with!
                if (CabbagePluginEditor* editor = dynamic_cast<CabbagePluginEditor*> (processor->getActiveEditor()))
                    return editor;
            }
    }

    return nullptr;
}
//==================================================================================
CabbagePluginProcessor* CabbageMainComponent::getCabbagePluginProcessor()
{
    const int32 nodeId = int32 (nodeIdsForPlugins.getWithDefault (getCurrentCsdFile().getFullPathName(), -99));

    if (AudioProcessorGraph::Node::Ptr f = audioGraph->graph.getNodeForId (nodeId))
    {
        if (CabbagePluginProcessor* const processor = dynamic_cast<CabbagePluginProcessor*> (f->getProcessor()))
            return processor;
    }

    return nullptr;
}
//==================================================================================
int CabbageMainComponent::getStatusbarYPos()
{
    return getCurrentEditorContainer()->getStatusBarPosition();
}
//=======================================================================================
void CabbageMainComponent::setEditMode (bool enable)
{
    const int nodeId = nodeIdsForPlugins.getWithDefault (getCurrentCsdFile().getFullPathName(), -99);

    if ( nodeId == -99)
        return;

    const bool isCabbageFile = CabbageUtilities::hasCabbageTags (getCurrentCsdFile());

    if (isCabbageFile == true)
    {
        if (!getCabbagePluginEditor())
        {
            //createAudioGraph();
            audioGraph->addPlugin (getCurrentCsdFile(), nodeId);
            const Point<int> pos (PluginWindow::getPositionOfCurrentlyOpenWindow (nodeId));
            createEditorForAudioGraphNode (pos);
        }

        getCabbagePluginEditor()->addChangeListener (this);
        getCabbagePluginEditor()->addActionListener (this);

        if (enable == true)
        {
            audioGraph->getNodeForId (nodeId)->getProcessor()->suspendProcessing (true);
            fileTabs[currentFileIndex]->getPlayButton().setToggleState (false, dontSendNotification);
            //audioGraph->stopPlaying();
            propertyPanel->setInterceptsMouseClicks (true, true);
        }
        else
        {
            //audioGraph->startPlaying();
            propertyPanel->setInterceptsMouseClicks (false, false);
        }

        getCabbagePluginEditor()->enableEditMode (enable);
    }
}
//=======================================================================================
void CabbageMainComponent::showSettingsDialog()
{
    DialogWindow::LaunchOptions o;
    o.content.setOwned (new CabbageSettingsWindow (*cabbageSettings, audioGraph->getAudioDeviceSelector()));
    o.content->setSize (500, 450);
    o.dialogTitle = TRANS ("Cabbage Settings");
    o.dialogBackgroundColour = Colour (0xfff0f0f0);
    o.escapeKeyTriggersCloseButton = true;
    o.useNativeTitleBar = true;
    o.resizable = false;
    o.runModal();
}
//==============================================================================
void CabbageMainComponent::createNewProject()
{
    DialogWindow::LaunchOptions o;
    o.content.setOwned (new CabbageProjectWindow (this));
    o.content->setSize (650, 350);

    o.dialogTitle = TRANS ("Select new project type");
    o.dialogBackgroundColour = Colour (0xfff0f0f0);
    o.escapeKeyTriggersCloseButton = true;
    o.useNativeTitleBar = true;
    o.resizable = false;

    o.launchAsync();
}
bool CabbageMainComponent::isInterestedInFileDrag(const StringArray & 	files)
{
	CabbageUtilities::debug("Hello");
	return true;
}

void CabbageMainComponent::filesDropped(const StringArray &files, int x, int y)
{
	for (auto file : files)
		openFile(file);
}
//==============================================================================
void CabbageMainComponent::launchSSHFileBrowser (String mode)
{
    const String sshAddress = cabbageSettings->getUserSettings()->getValue ("SSHAddress");
    const String sshHome = cabbageSettings->getUserSettings()->getValue ("SSHHomeDir");
    DialogWindow::LaunchOptions o;
    o.content.setOwned (new CabbageSSHFileBrowser (sshAddress, sshHome, this, mode, getCurrentCsdFile().getFullPathName()));
    o.content->setSize (650, 350);

    o.dialogTitle = TRANS ("Browse Raspberry PI for files..");
    o.dialogBackgroundColour = Colour (0xfff0f0f0);
    o.escapeKeyTriggersCloseButton = true;
    o.useNativeTitleBar = true;
    o.resizable = false;

    o.launchAsync();
}
//==============================================================================
void CabbageMainComponent::openGraph (File fileToOpen)
{
    PluginDescription desc;
    Array<int32> uuids;
    Array<File> files;

    if (fileToOpen.existsAsFile() == false)
    {
        FileChooser fc ("Open File", cabbageSettings->getMostRecentFile().getParentDirectory(), "*.cabbage", CabbageUtilities::shouldUseNativeBrowser());

        if (fc.browseForFileToOpen())
        {
            fileToOpen = fc.getResult();
        }
    }

    nodeIdsForPlugins.clear();
    XmlDocument doc (fileToOpen);
    ScopedPointer<XmlElement> xml (doc.getDocumentElement());

    if (xml == nullptr || ! xml->hasTagName ("FILTERGRAPH"))
        return;

    cabbageSettings->updateRecentFilesList (fileToOpen);

    forEachXmlChildElementWithTagName (*xml, filter, "FILTER")
    uuids.add (filter->getIntAttribute ("uid"));

    forEachXmlChildElementWithTagName (*xml, filter, "FILTER")
    forEachXmlChildElementWithTagName (*filter, plugin, "PLUGIN")
    {
        const String pluginFile = plugin->getStringAttribute ("file");
        files.add (File (pluginFile));
    }

    for ( int i = 0 ; i < files.size() ; i++)
    {
        if (files[i].existsAsFile() && openFiles.contains (files[i]) == false)
        {
            nodeIdsForPlugins.set (files[i].getFullPathName(), uuids[i]);
            openFile (files[i].getFullPathName());
        }
    }

    audioGraph->loadDocument (fileToOpen);
}
//==================================================================================
File CabbageMainComponent::getCurrentCsdFile ()
{
    return openFiles[currentFileIndex];
}

void CabbageMainComponent::setCurrentCsdFile (File file)
{
    openFiles.set (currentFileIndex, file);
}
//==================================================================================
void CabbageMainComponent::saveGraph (bool saveAs)
{
    audioGraph->saveGraph (saveAs);
}
//==================================================================================
const File CabbageMainComponent::openFile (String filename)
{
    stopTimer();
    stopCsoundForNode (filename);
    File currentCsdFile;

    if (File (filename).existsAsFile() == false)
    {
        FileChooser fc ("Open File", cabbageSettings->getMostRecentFile().getParentDirectory(), "*.csd", CabbageUtilities::shouldUseNativeBrowser());

        if (fc.browseForFileToOpen())
        {
            if (openFiles.contains (File (filename)) && currentFileIndex > -1)
            {
                CabbageUtilities::showMessage ("File is already open", lookAndFeel);
                return File();
            }
            else
                currentCsdFile = fc.getResult();
        }
        else
            return File();
    }
    else
        currentCsdFile = File (filename);

    cabbageSettings->updateRecentFilesList (currentCsdFile);
    createCodeEditorForFile (currentCsdFile);
    return currentCsdFile;

}
//==================================================================================
void CabbageMainComponent::launchHelpfile (String type)
{
    String url = "";

    if (helpWindow == nullptr)
    {
        helpWindow = new HtmlHelpDocumentWindow ("Help", Colour (20, 20, 20));
    }

    CodeDocument::Position pos1, pos2;
    pos1 = getCurrentCodeEditor()->getDocument().findWordBreakBefore (getCurrentCodeEditor()->getCaretPos());
    pos2 = getCurrentCodeEditor()->getDocument().findWordBreakAfter (getCurrentCodeEditor()->getCaretPos());
    String keyword = getCurrentCodeEditor()->getDocument().getTextBetween (pos1, pos2).trim();

    const String csoundHelpDir = cabbageSettings->getUserSettings()->getValue ("CsoundManualDir");
    const String cabbageHelpDir = cabbageSettings->getUserSettings()->getValue ("CabbageManualDir");

    if (type == "context")
    {
        CabbageControlWidgetStrings controlWidgets;
        CabbageLayoutWidgetStrings layoutWidgets;

        if (controlWidgets.contains (keyword) || layoutWidgets.contains (keyword))
        {
            if (keyword.contains ("slider"))
                url = cabbageHelpDir + "/sliders.html";
            else if (keyword == "filebutton")
                url = cabbageHelpDir + "/filebutton.html";
            else if (keyword == "infobutton")
                url = cabbageHelpDir + "/infobutton.html";
            else
                url = cabbageHelpDir + "/" + keyword + String (".html");
        }
        else
            url = csoundHelpDir + "/" + keyword + String (".html");
    }
    else if (type == "csound")
        url = csoundHelpDir + "/index.html";

    else
        url = cabbageHelpDir + "/index.html";

    if (File (url).existsAsFile())
    {
        helpWindow->loadPage ("file://" + url);
        helpWindow->setVisible (true);
        helpWindow->toFront (true);

    }
}

//==================================================================================
void CabbageMainComponent::createCodeEditorForFile (File file)
{
    CabbageEditorContainer* editorConsole;
    editorAndConsole.add (editorConsole = new CabbageEditorContainer (cabbageSettings));
    addAndMakeVisible (editorConsole);
    addAndMakeVisible (propertyPanel = new CabbagePropertiesPanel (cabbageSettings->valueTree));
    propertyPanel->setVisible (false);
    editorConsole->setVisible (true);
    editorConsole->toFront (true);
    openFiles.add (file);
    editorConsole->editor->loadContent (file.loadFileAsString());
    editorConsole->editor->parseTextForInstrumentsAndRegions();
    editorConsole->editor->startThread();
    numberOfFiles = editorAndConsole.size();
    currentFileIndex = editorAndConsole.size() - 1;
    addFileTabButton (openFiles[numberOfFiles - 1]);
    getCurrentCodeEditor()->addChangeListener (this);
    getCurrentCodeEditor()->setSavePoint();
    owner->setName ("Cabbage " + openFiles[currentFileIndex].getFullPathName());
    addInstrumentsAndRegionsToCombobox();
    repaint();
    resized();
}
//==============================================================================
void CabbageMainComponent::saveDocument (bool saveAs, bool recompile)
{

    if (saveAs == true)
    {

        stopCsoundForNode (getCurrentCsdFile().getFullPathName());

        isGUIEnabled = false;

        if (getCabbagePluginEditor() != nullptr)
            getCabbagePluginEditor()->enableEditMode (false);

        FileChooser fc ("Select file name and location", getCurrentCsdFile().getParentDirectory(), "*.csd", CabbageUtilities::shouldUseNativeBrowser());

        if (fc.browseForFileToSave (false))
        {
            if (fc.getResult().withFileExtension ("csd").existsAsFile())
            {
                const int result = CabbageUtilities::showYesNoMessage ("Do you wish to overwrite\nexiting file?", lookAndFeel);

                if (result == 1)
                    writeFileToDisk (fc.getResult().withFileExtension (".csd"));
            }
            else
                writeFileToDisk (fc.getResult().withFileExtension (".csd"));

            getCurrentCodeEditor()->setSavePoint();
        }
    }
    else
    {
        stopCsoundForNode (getCurrentCsdFile().getFullPathName());;
        isGUIEnabled = false;

        if (getCabbagePluginEditor() != nullptr)
            getCabbagePluginEditor()->enableEditMode (false);

        if (getCurrentCodeEditor()->hasFileChanged())
        {
            if (getCurrentCsdFile().existsAsFile())
                getCurrentCsdFile().replaceWithText (getCurrentCodeEditor()->getDocument().getAllContent());
        }

        if (cabbageSettings->getUserSettings()->getIntValue ("CompileOnSave") == 1)
        {
            propertyPanel->setEnabled (false);

            if (recompile == true)
            {
                runCsoundForNode (getCurrentCsdFile().getFullPathName());
                fileTabs[currentFileIndex]->getPlayButton().setToggleState (true, dontSendNotification);
            }
        }
    }
}
//==================================================================================
void CabbageMainComponent::writeFileToDisk (File file)
{
    setCurrentCsdFile (file);

    getCurrentCsdFile().replaceWithText (getCurrentCodeEditor()->getAllText());
    owner->setName ("Cabbage " + getCurrentCsdFile().getFullPathName());
    addInstrumentsAndRegionsToCombobox();
    fileTabs[currentFileIndex]->setButtonText (getCurrentCsdFile().getFileName());
    cabbageSettings->updateRecentFilesList (getCurrentCsdFile());
}
//==================================================================================
void CabbageMainComponent::closeDocument()
{
    if (editorAndConsole.size() > 0)
    {
        if (getCurrentCodeEditor()->hasFileChanged() == true)
        {
            const int result = CabbageUtilities::showYesNoMessage ("File has been modified, do you wish to save?\nexiting file?", lookAndFeel, 1);

            if (result == 1)
            {
                saveDocument (false, false);
                removeEditor();
            }
            else if (result == 2)
                removeEditor();
        }
        else
        {
            removeEditor();
        }
    }
}
//==================================================================================
void CabbageMainComponent::removeEditor()
{
    editorAndConsole.removeObject (getCurrentEditorContainer());
    fileTabs.remove (currentFileIndex);
    openFiles.remove (currentFileIndex);
    currentFileIndex = (currentFileIndex > 0 ? currentFileIndex - 1 : 0);

    if (editorAndConsole.size() != 0)
    {
        if (currentFileIndex > -1)
        {
            editorAndConsole[currentFileIndex]->toFront (true);
        }

        if (fileTabs.size() > 0)
        {
            fileTabs[currentFileIndex]->setToggleState (true, dontSendNotification);
            owner->setName ("Cabbage " + openFiles[currentFileIndex].getFullPathName());
            arrangeFileTabButtons();
        }
        else
        {
            owner->setName ("Cabbage Csound IDE");
        }
    }

    //currentCsdFile = openFiles[currentFileIndex];

    repaint();


}
//==============================================================================
String CabbageMainComponent::getSearchString()
{
    return searchString;
}

void CabbageMainComponent::setSearchString (const String& s)
{
    searchString = s;
}

void CabbageMainComponent::setReplaceString (const String& s)
{
    replaceString = s;
}

bool CabbageMainComponent::isCaseSensitiveSearch()
{
    return isCaseSensitive;
}
//==================================================================================
void CabbageMainComponent::setCaseSensitiveSearch (bool b)
{
    isCaseSensitive = b;
}
//==================================================================================
int CabbageMainComponent::findNext (bool forwards)
{
    if (findPanel != nullptr)
    {
        return getCurrentCodeEditor()->findText (searchString, forwards, isCaseSensitiveSearch(), forwards);
    }

    return -1;
}
//==================================================================================
void CabbageMainComponent::replaceText (bool replaceAll)
{
    if (findPanel != nullptr)
    {
        if ( replaceAll == false )
            getCurrentCodeEditor()->replaceText (searchString, replaceString);
        else
        {
            do
            {
                getCurrentCodeEditor()->replaceText (searchString, replaceString);
            }
            while (findNext (true) >= 0);
        }
    }
}
//==============================================================================
int CabbageMainComponent::testFileForErrors(String file)
{
	ChildProcess process;
	const String applicationDir = File::getSpecialLocation (File::currentExecutableFile).getParentDirectory().getFullPathName();
	process.start(applicationDir+"/testCsoundFile "+file);

	process.readAllProcessOutput();
	
	const int exitCode = process.getExitCode();

	if(exitCode==1)
	{
		process.start("csound "+file);
		this->getCurrentOutputConsole()->setText(process.readAllProcessOutput());
		stopCsoundForNode (file);
		return 1;
	}
	
	return exitCode;
	
}
void CabbageMainComponent::runCsoundForNode (String file)
{
	
	if(testFileForErrors(file)==0) //if Csound seg faults it will take Cabbage down. best to test the instrumnet in a seperate process first. 
	{
		if (File (file).existsAsFile())
		{
			//PluginWindow::closeAllCurrentlyOpenWindows();
			//audioGraph = nullptr;
			//createAudioGraph(); //in future versions we can simply edit the node in question and reconnect within the graph
			int32 node = nodeIdsForPlugins.getWithDefault (file, -99);

			if (node == -99)
			{
				Uuid uniqueID;
				node = int32 (*uniqueID.getRawData());
				nodeIdsForPlugins.set (file, node);
			}

			Point<int> pos (PluginWindow::getPositionOfCurrentlyOpenWindow (node));
			PluginWindow::closeCurrentlyOpenWindowsFor (node);

			if (pos.getX() == -1000 && pos.getY() == -1000)
			{
				Random rand;//nextInt();
				pos.setX (rand.nextInt (Range<int> (getWidth() / 2, getWidth() / 1.8)));
				pos.setY (rand.nextInt (Range<int> (getHeight() / 2, (getHeight() / 2) + 100)));
			}

			audioGraph->addPlugin (getCurrentCsdFile(), node);
			createEditorForAudioGraphNode (pos);
			startTimer (100);
			factory.togglePlay (true);
			graphComponent->updateComponents();
		}
		else
			CabbageUtilities::showMessage ("Warning", "Please open a file first", lookAndFeel);
	}
}

void CabbageMainComponent::stopCsoundForNode (String file)
{
    if (nodeIdsForPlugins.size() > 0 && File (file).existsAsFile())
    {
        const int32 nodeId = nodeIdsForPlugins.getWithDefault (file, -99);

        if (audioGraph->getNodeForId (nodeId) != nullptr)
            audioGraph->getNodeForId (nodeId)->getProcessor()->suspendProcessing (true);
    }
}
//==================================================================================
void CabbageMainComponent::startAudioGraph()
{
    factory.togglePlay (true);
    audioGraph->startPlaying();
}
//==================================================================================
void CabbageMainComponent::stopAudioGraph()
{
    stopTimer();
    factory.togglePlay (false);

    if (audioGraph)
        audioGraph->stopPlaying();

}
//==============================================================================
void CabbageMainComponent::showFindPanel (bool withReplace)
{
    if (findPanel == nullptr)
    {
        findPanel = new FindPanel (getSearchString(), isCaseSensitive, withReplace);
        addAndMakeVisible (findPanel);
        resized();
    }

    if (findPanel != nullptr)
    {
        findPanel->findEditor.grabKeyboardFocus();
        findPanel->findEditor.selectAll();
    }
}
//==================================================================================
void CabbageMainComponent::hideFindPanel()
{
    findPanel = nullptr;
    getCurrentCodeEditor()->grabKeyboardFocus();
}
//==============================================================================
void CabbageMainComponent::resized()
{
    const int heightOfTabButtons = (editorAndConsole.size() > 0) ? 35 : 0;
    toolbar.setBounds (getLocalBounds().removeFromTop  (toolbarThickness));

    if (propertyPanel->isVisible())
    {
        propertyPanel->setBounds (getWidth() - 200, toolbar.getHeight(), 200, getHeight() - toolbar.getHeight());
    }

    resizeAllWindows (toolbarThickness + heightOfTabButtons);


    if (findPanel != nullptr)
    {
        findPanel->setTopRightPosition (getWidth() - 16, 70);
    }

    arrangeFileTabButtons();
}
