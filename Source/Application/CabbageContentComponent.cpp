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

#include "CabbageContentComponent.h"
#include "CabbageDocumentWindow.h"
#include "../Utilities/CabbageNewProjectWindow.h"
#include "../Utilities/CabbageSSHFileBrowser.h"
#include "../Utilities/CabbageStrings.h"

//==============================================================================
CabbageContentComponent::CabbageContentComponent (CabbageDocumentWindow* owner, CabbageSettings* settings)
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
	audioGraphWindow = new AudioGraphDocumentWindow("Cabbage Patcher", Colours::white);
	audioGraphWindow->setVisible(false);
	
    addAndMakeVisible (toolbar);
    toolbar.addDefaultItems (factory);
    propertyPanel->addChangeListener (this);
    factory.combo->getCombo().addListener (this);

    setLookAndFeelColours();
    createAudioGraph(); //set up graph even though no file is selected. Allows users to change audio devices from the get-go..
}

CabbageContentComponent::~CabbageContentComponent()
{

    editorAndConsole.clear();
	graphComponent = nullptr;
    audioGraph = nullptr;

    if (tempFile.existsAsFile())
        tempFile.deleteFile();
}

void CabbageContentComponent::paint (Graphics& g)
{
    if (editorAndConsole.size() == 0)
        g.drawImage (bgImage, getLocalBounds().toFloat());
    else
        g.fillAll ( CabbageSettings::getColourFromValueTree (cabbageSettings->valueTree, CabbageColourIds::lineNumberBackground, Colour (50, 50, 50)));

}

void CabbageContentComponent::setLookAndFeelColours()
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

bool CabbageContentComponent::setCurrentCsdFile (File file)
{
    if (currentCsdFile == file && currentFileIndex > -1)
    {
        CabbageUtilities::showMessage ("File is already open", lookAndFeel);
        return false;
    }
    else
    {
        currentCsdFile = file;
        Uuid uniqueID;
        nodeIdsForFiles.set (file.getFullPathName(), int32 (*uniqueID.getRawData()));
    }

    return true;
}
//==============================================================================
void CabbageContentComponent::buttonClicked (Button* button)
{
    if (const FileTabButton* tabButton = dynamic_cast<FileTabButton*> (button))
    {
        currentFileIndex = fileTabs.indexOf (tabButton);
        editorAndConsole[currentFileIndex]->toFront (true);

        if (CabbageDocumentWindow* docWindow = this->findParentComponentOfClass<CabbageDocumentWindow>())
        {
            docWindow->setName (openFiles[currentFileIndex].getFullPathName());
            currentCsdFile = openFiles[currentFileIndex];
            addInstrumentsAndRegionsToCombobox();
        }
    }
    else if (const ToolbarButton* toolbarButton = dynamic_cast<ToolbarButton*> (button))
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
                this->stopAudioGraph();
            else
                this->startAudioGraph();
        }
    }
	else if(DrawableButton* runButton = dynamic_cast<DrawableButton*>(button))
	{
		
		if(runButton->getToggleState() == true)
		{
			runCsoundForNode(runButton->getTooltip());
		}
		else
			stopCsoundForNode(runButton->getTooltip());
			
	}
}

void CabbageContentComponent::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    int index = comboBoxThatHasChanged->getSelectedId();
    const int lineToScrollTo = getCurrentCodeEditor()->instrumentsAndRegions.getValueAt (index - 1);
    getCurrentCodeEditor()->scrollToLine (lineToScrollTo);
}
//==============================================================================
void CabbageContentComponent::changeListenerCallback (ChangeBroadcaster* source)
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

void CabbageContentComponent::actionListenerCallback (const String& message)
{
    if (message.contains ("delete:"))
    {
        const int lineNumber = String (message.replace ("delete:", "")).getIntValue();
        getCurrentCodeEditor()->removeLine (lineNumber);
    }
}
//=======================================================================================
void CabbageContentComponent::updateCodeInEditor (CabbagePluginEditor* editor, bool replaceExistingLine)
{
    propertyPanel->addChangeListener (this);

    for (ValueTree wData : editor->getValueTreesForCurrentlySelectedComponents())
    {
        const int lineNumber = CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::linenumber);
        const String parent = CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::parentcomponent); // if widget has a parent don't highlight line

        const String currentLineText = getCurrentCodeEditor()->getLineText (lineNumber);

        const String macroText = CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::expandedmacrotext);

        String macroNames (" ");

        for ( int i = 0 ; i < CabbageWidgetData::getProperty (wData, CabbageIdentifierIds::macronames).size() ; i++ )
            macroNames += CabbageWidgetData::getProperty (wData, CabbageIdentifierIds::macronames)[i].toString() + " ";


        const String newText = CabbageWidgetData::getCabbageCodeFromIdentifiers (wData, currentLineText, macroText);

        getCurrentCodeEditor()->insertCode (lineNumber, newText + macroNames, replaceExistingLine, parent.isEmpty());

    }
}
//==============================================================================
void CabbageContentComponent::timerCallback()
{
//    int32 nodeId = int32 (nodeIdsForFiles.getWithDefault (currentCsdFile.getFullPathName(), -99));	
//    if (audioGraph->graph.getNodeForId (nodeId)->getProcessor()->isSuspended() == true)
//    {
//        stopCsoundForNode("");
//        stopTimer();
//    }
//
//    if (currentCsdFile.existsAsFile())
//    {
//        const String csoundOutputString = audioGraph->getCsoundOutput(nodeId);
//        consoleMessages += csoundOutputString;
//
//        if (csoundOutputString.length() > 0)
//            getCurrentOutputConsole()->setText (csoundOutputString);
//
//    }
}
//==============================================================================
void CabbageContentComponent::updateEditorColourScheme()
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
Image CabbageContentComponent::createBackground()
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
void CabbageContentComponent::addFileTabButton (File file)
{
    FileTabButton* fileButton;
    fileTabs.add (fileButton = new FileTabButton (file.getFileName(), file.getFullPathName()));

    addAndMakeVisible (fileButton);
    fileButton->addListener (this);
    fileButton->setRadioGroupId (99);
    fileButton->setClickingTogglesState (true);
    fileButton->setLookAndFeel (lookAndFeel);
    fileButton->setToggleState (true, dontSendNotification);
    currentFileIndex = fileTabs.size() - 1;
	fileButton->getPlayButton().addListener(this);
}

void CabbageContentComponent::arrangeFileTabButtons()
{
    int xPos = 10;
    const int numTabs = jmax (3, fileTabs.size());

    for ( auto fileButton : fileTabs)
    {
        const int width = propertyPanel->isVisible() ? getWidth() - propertyPanel->getWidth() - 15 : getWidth() - 25;
        fileButton->setBounds (xPos, toolbarThickness + 3, width / numTabs, 25);
        xPos += width / numTabs + 4;
    }
}

//==============================================================================
void CabbageContentComponent::addInstrumentsAndRegionsToCombobox()
{
    factory.combo->clearItemsFromComboBox();

    const NamedValueSet instrRegions = getCurrentCodeEditor()->instrumentsAndRegions;

    for ( int i = 0 ; i < instrRegions.size() ; i++)
    {
        factory.combo->addItemsToComboBox (instrRegions.getName (i).toString(), i + 1);
    }
}
//==============================================================================
void CabbageContentComponent::resizeAllEditorAndConsoles (int height)
{
    const bool isPropPanelVisible = propertyPanel->isVisible();

    for ( CabbageEditorContainer* editor : editorAndConsole )
    {
        editor->statusBar.setSize (getWidth(), 28);
        editor->setBounds (0, height, getWidth() - (isPropPanelVisible ? 200 : 0), getHeight());
    }

    arrangeFileTabButtons();
}

//==============================================================================
void CabbageContentComponent::createAudioGraph()
{
    const Point<int> lastPoint = PluginWindow::getPositionOfCurrentlyOpenWindow (1);

    if (lastPoint.getX() > 0)
    {
        cabbageSettings->setProperty ("windowX", lastPoint.getX());
        cabbageSettings->setProperty ("windowY", lastPoint.getY());
    }

    audioGraph = new AudioGraph (cabbageSettings->getUserSettings(), false);
    audioGraph->setXmlAudioSettings (cabbageSettings->getUserSettings()->getXmlValue ("audioSetup"));
    graphComponent = new CabbageGraphComponent (*audioGraph);
	audioGraphWindow->setContentNonOwned(graphComponent, false);

	
}

void CabbageContentComponent::showGraph()
{
	audioGraphWindow->setUsingNativeTitleBar (true);
	audioGraphWindow->setVisible(true);
	audioGraphWindow->setTopLeftPosition(getWidth()-audioGraphWindow->getWidth(), 10);
	audioGraphWindow->setAlwaysOnTop(true);
}
//==============================================================================
void CabbageContentComponent::createEditorForAudioGraphNode()
{
    int32 nodeId = int32 (nodeIdsForFiles.getWithDefault (currentCsdFile.getFullPathName(), -99));

    if (AudioProcessorGraph::Node::Ptr f = audioGraph->graph.getNodeForId (nodeId))
    {
        PluginWindow::WindowFormatType type = f->getProcessor()->hasEditor() ? PluginWindow::Normal
                                              : PluginWindow::Generic;

        if (PluginWindow* const w = PluginWindow::getWindowFor (f, type, audioGraph->graph))
        {
            w->toFront (true);
            Point<int> point (cabbageSettings->getIntProperty ("windowX"),
                              cabbageSettings->getIntProperty ("windowY"));

            if (point.getY() > 0 && point.getX() > 0)
                w->setTopLeftPosition (point.getX(), point.getY());
        }
    }
}

//==============================================================================
CabbageEditorContainer* CabbageContentComponent::getCurrentEditorContainer()
{
    if (editorAndConsole.size() > 0)
        return editorAndConsole[currentFileIndex];

    return nullptr;
}

CabbageCodeEditorComponent* CabbageContentComponent::getCurrentCodeEditor()
{
    if (getCurrentEditorContainer())
        return getCurrentEditorContainer()->editor;
    else
        return nullptr;
}

CabbageOutputConsole* CabbageContentComponent::getCurrentOutputConsole()
{
    if (getCurrentEditorContainer())
        return getCurrentEditorContainer()->outputConsole;
    else
        return nullptr;
}

String CabbageContentComponent::getAudioDeviceSettings()
{
    if (audioGraph != nullptr && audioGraph->getDeviceManagerSettings().isNotEmpty())
        return audioGraph->getDeviceManagerSettings();
    else
        return String::empty;
}

CabbagePluginEditor* CabbageContentComponent::getCabbagePluginEditor()
{
    if (audioGraph != nullptr)
    {
        int32 nodeId = int32 (nodeIdsForFiles.getWithDefault (currentCsdFile.getFullPathName(), -99));

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

CabbagePluginProcessor* CabbageContentComponent::getCabbagePluginProcessor()
{
    int32 nodeId = int32 (nodeIdsForFiles.getWithDefault (currentCsdFile.getFullPathName(), -99));

    if (AudioProcessorGraph::Node::Ptr f = audioGraph->graph.getNodeForId (nodeId))
    {
        if (CabbagePluginProcessor* const processor = dynamic_cast<CabbagePluginProcessor*> (f->getProcessor()))
            return processor;
    }

    return nullptr;
}

int CabbageContentComponent::getStatusbarYPos()
{
    return getCurrentEditorContainer()->getStatusBarPosition();
}

//=======================================================================================
void CabbageContentComponent::setEditMode (bool enable)
{
    if (audioGraph->getIsCabbageFile() == true)
    {
        if (!getCabbagePluginEditor())
        {
            //createAudioGraph();
            audioGraph->addPlugin (currentCsdFile, nodeIdsForFiles.getWithDefault (currentCsdFile.getFullPathName(), -99));
            createEditorForAudioGraphNode();
        }

        getCabbagePluginEditor()->addChangeListener (this);
        getCabbagePluginEditor()->addActionListener (this);

        if (enable == true)
        {
            audioGraph->stopPlaying();
            propertyPanel->setInterceptsMouseClicks (true, true);
        }
        else
        {
            audioGraph->startPlaying();
            propertyPanel->setInterceptsMouseClicks (false, false);
        }

        getCabbagePluginEditor()->enableEditMode (enable);
    }
}
//=======================================================================================
void CabbageContentComponent::showSettingsDialog()
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
void CabbageContentComponent::createNewProject()
{
    Logger::writeToLog ("CreateNewProject");

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

//==============================================================================
void CabbageContentComponent::launchSSHFileBrowser (String mode)
{
    const String sshAddress = cabbageSettings->getUserSettings()->getValue ("SSHAddress");
    const String sshHome = cabbageSettings->getUserSettings()->getValue ("SSHHomeDir");
    DialogWindow::LaunchOptions o;
    o.content.setOwned (new CabbageSSHFileBrowser (sshAddress, sshHome, this, mode, currentCsdFile.getFullPathName()));
    o.content->setSize (650, 350);

    o.dialogTitle = TRANS ("Browse Raspberry PI for files..");
    o.dialogBackgroundColour = Colour (0xfff0f0f0);
    o.escapeKeyTriggersCloseButton = true;
    o.useNativeTitleBar = true;
    o.resizable = false;

    o.launchAsync();
}
//==============================================================================
void CabbageContentComponent::openGraph()
{
	FileChooser fc ("Open File", cabbageSettings->getMostRecentFile().getParentDirectory(), "*.cabbage");

	if (fc.browseForFileToOpen())
	{
		
	}
}

void CabbageContentComponent::openFile (String filename)
{

    stopTimer();
    stopCsoundForNode(filename);

    //PluginWindow::closeAllCurrentlyOpenWindows();


    if (File (filename).existsAsFile() == false)
    {
        FileChooser fc ("Open File", cabbageSettings->getMostRecentFile().getParentDirectory(), "*.csd");

        if (fc.browseForFileToOpen())
        {
            if (setCurrentCsdFile (fc.getResult()) == false)
                return;
        }
        else
            return;
    }
    else if (setCurrentCsdFile (File (filename)) == false)
        return;


    cabbageSettings->updateRecentFilesList (currentCsdFile);

    CabbageEditorContainer* editorConsole;
    editorAndConsole.add (editorConsole = new CabbageEditorContainer (cabbageSettings));
    addAndMakeVisible (editorConsole);
    addAndMakeVisible (propertyPanel = new CabbagePropertiesPanel (cabbageSettings->valueTree));
    propertyPanel->setVisible (false);
    editorConsole->setVisible (true);
    editorConsole->toFront (true);
    openFiles.add (currentCsdFile);
    editorConsole->editor->loadContent (currentCsdFile.loadFileAsString());
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
void CabbageContentComponent::saveDocument (bool saveAs, bool recompile)
{
    getCurrentCodeEditor()->setSavePoint();

    if (saveAs == true)
    {
        stopCsoundForNode(currentCsdFile.getFullPathName());

        isGUIEnabled = false;

        if (getCabbagePluginEditor() != nullptr)
            getCabbagePluginEditor()->enableEditMode (false);

        FileChooser fc ("Select file name and location", File::getSpecialLocation (File::SpecialLocationType::userHomeDirectory), "*.csd");

        if (fc.browseForFileToSave (false))
        {
            if (fc.getResult().withFileExtension ("csd").existsAsFile())
            {
                const int result = CabbageUtilities::showYesNoMessage ("Do you wish to overwrite\nexiting file?", lookAndFeel);

                if (result == 0)
                {
                    //fc.getResult().withFileExtension("csd").replaceWithText(currentCsdFile.loadFileAsString());
                    currentCsdFile = fc.getResult().withFileExtension (".csd");
                    currentCsdFile.replaceWithText (getCurrentCodeEditor()->getAllText());
                    owner->setName ("Cabbage " + currentCsdFile.getFullPathName());
                    fileTabs[currentFileIndex]->setButtonText (currentCsdFile.getFileName());
                    addInstrumentsAndRegionsToCombobox();
                    cabbageSettings->updateRecentFilesList (currentCsdFile);
                }
            }
            else
            {
                //fc.getResult().withFileExtension("csd").replaceWithText(currentCsdFile.loadFileAsString());
                currentCsdFile = fc.getResult().withFileExtension (".csd");
                currentCsdFile.replaceWithText (getCurrentCodeEditor()->getAllText());
                owner->setName ("Cabbage " + currentCsdFile.getFullPathName());
                addInstrumentsAndRegionsToCombobox();
                fileTabs[currentFileIndex]->setButtonText (currentCsdFile.getFileName());
                cabbageSettings->updateRecentFilesList (currentCsdFile);
            }
        }
    }
    else
    {
        stopCsoundForNode(currentCsdFile.getFullPathName());;
        isGUIEnabled = false;

        if (getCabbagePluginEditor() != nullptr)
            getCabbagePluginEditor()->enableEditMode (false);

        if (currentCsdFile.existsAsFile())
            currentCsdFile.replaceWithText (getCurrentCodeEditor()->getDocument().getAllContent());

        if (cabbageSettings->getUserSettings()->getIntValue ("CompileOnSave") == 1)
        {
            propertyPanel->setEnabled (false);

            if (recompile == true)
                runCsoundForNode(currentCsdFile.getFullPathName());
        }
    }
}

void CabbageContentComponent::closeDocument()
{
    if (editorAndConsole.size() > 0)
    {
        if (getCurrentCodeEditor()->hasFileChanged() == true)
        {
            const int result = CabbageUtilities::showYesNoMessage ("File has been modified, do you wish to save?\nexiting file?", lookAndFeel, 1);

            if (result == 0 || result == 1)
            {
                if (result == 0)
                    saveDocument (false, false);

                removeEditor();
            }

        }
        else
        {
            removeEditor();
        }
    }
}

void CabbageContentComponent::removeEditor()
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

    repaint();


}
//==============================================================================
String CabbageContentComponent::getSearchString()
{
    return searchString;
}

void CabbageContentComponent::setSearchString (const String& s)
{
    searchString = s;
}

void CabbageContentComponent::setReplaceString (const String& s)
{
    replaceString = s;
}

bool CabbageContentComponent::isCaseSensitiveSearch()
{
    return isCaseSensitive;
}

void CabbageContentComponent::setCaseSensitiveSearch (bool b)
{
    isCaseSensitive = b;
}

int CabbageContentComponent::findNext (bool forwards)
{
    if (findPanel != nullptr)
    {
        return getCurrentCodeEditor()->findText (searchString, forwards, isCaseSensitiveSearch(), forwards);
    }

    return -1;
}

void CabbageContentComponent::replaceText (bool replaceAll)
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
void CabbageContentComponent::runCsoundForNode(String file)
{
    if (File(file).existsAsFile())
    {
        //PluginWindow::closeAllCurrentlyOpenWindows();
        //audioGraph = nullptr;
        //createAudioGraph(); //in future versions we can simply edit the node in question and reconnect within the graph
        int32 node = nodeIdsForFiles.getWithDefault (file, -99);
        PluginWindow::closeCurrentlyOpenWindowsFor (node);
        audioGraph->addPlugin (currentCsdFile, node);
        createEditorForAudioGraphNode();
        startTimer (100);
        factory.togglePlay (true);
		graphComponent->updateComponents();
    }
    else
        CabbageUtilities::showMessage ("Warning", "Please open a file first", lookAndFeel);
}

void CabbageContentComponent::stopCsoundForNode(String file)
{
	int32 nodeId = nodeIdsForFiles.getWithDefault (file, -99);
	if(audioGraph->getNodeForId(nodeId) != nullptr)
		audioGraph->getNodeForId(nodeId)->getProcessor()->suspendProcessing(true);
}

void CabbageContentComponent::startAudioGraph()
{
	factory.togglePlay (false);
	audioGraph->startPlaying();
}

void CabbageContentComponent::stopAudioGraph()
{
    stopTimer();
    factory.togglePlay (true);

    if (audioGraph)
        audioGraph->stopPlaying();

}

//==============================================================================
void CabbageContentComponent::showFindPanel (bool withReplace)
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

void CabbageContentComponent::hideFindPanel()
{
    findPanel = nullptr;
    getCurrentCodeEditor()->grabKeyboardFocus();
}

//==============================================================================
void CabbageContentComponent::resized()
{
    const int heightOfTabButtons = (editorAndConsole.size() > 0) ? 30 : 0;
    toolbar.setBounds (getLocalBounds().removeFromTop  (toolbarThickness));

    if (propertyPanel->isVisible())
    {
        propertyPanel->setBounds (getWidth() - 200, toolbar.getHeight(), 200, getHeight() - toolbar.getHeight());
    }

    resizeAllEditorAndConsoles (toolbarThickness + heightOfTabButtons);

    if (findPanel != nullptr)
    {
        findPanel->setTopRightPosition (getWidth() - 16, 70);
    }

    arrangeFileTabButtons();
	
}