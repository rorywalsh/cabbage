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

#include "CabbageDocumentWindow.h"
#include "CabbageNewProjectWindow.h"


//=================================================================================================================
CabbageDocumentWindow::CabbageDocumentWindow (String name)  : DocumentWindow(name,
            Colours::lightgrey,
            DocumentWindow::allButtons),
    lookAndFeel(new CabbageIDELookAndFeel())
{
    setTitleBarButtonsRequired(DocumentWindow::allButtons, false);
    setUsingNativeTitleBar (true);
    setupSettingsFile();

    setResizable(true, true);
    centreWithSize (getWidth(), getHeight());
    setVisible (true);

    setContentOwned (content = new CabbageContentComponent(cabbageSettings->getValueTree()), true);
    content->propertyPanel->setVisible(false);

    lookAndFeel->setColour(Slider::ColourIds::thumbColourId, Colour(110, 247, 0));
    lookAndFeel->setColour(ScrollBar::backgroundColourId, Colour(70, 70, 70));

    setMenuBar(this, 25);
    getMenuBarComponent()->setLookAndFeel(lookAndFeel);

    createAudioGraph();

    if(cabbageSettings->getUserSettings()->getIntValue("OpenMostRecentFileOnStartup")==1)
    {
        cabbageSettings->updateRecentFilesList();
        openFile(cabbageSettings->getMostRecentFile());
    }

    setApplicationCommandManagerToWatch(&commandManager);
    commandManager.registerAllCommandsForTarget(this);
    addKeyListener(commandManager.getKeyMappings());

#if JUCE_MAC
    MenuBarModel::setMacMainMenu (this, nullptr, "Open Recent");
#endif

    content->propertyPanel->addChangeListener(this);

}

void CabbageDocumentWindow::setupSettingsFile()
{
    PropertiesFile::Options options;
    options.applicationName     = "Cabbage2";
    options.filenameSuffix      = "settings";
    options.osxLibrarySubFolder = "Preferences";
#if JUCE_LINUX
    options.folderName          = "~/.config/Cabbage2";
#else
    options.folderName          = "Cabbage2";
#endif

    cabbageSettings = new CabbageSettings();
    cabbageSettings->setStorageParameters (options);
    cabbageSettings->setDefaultSettings();
    cabbageSettings->addChangeListener(this);
    //lookAndFeel->refreshLookAndFeel(cabbageSettings->getValueTree());
}

CabbageDocumentWindow::~CabbageDocumentWindow()
{
    setMenuBar(nullptr);
    cabbageSettings->closeFiles();
    cabbageSettings->setProperty("audioSetup", getAudioDeviceSettings());
#if JUCE_MAC
    MenuBarModel::setMacMainMenu (nullptr);
#endif
    audioGraph = nullptr;
    //lookAndFeel = nullptr;

}


//=======================================================================================
CabbageContentComponent* CabbageDocumentWindow::getMainContentComponent()
{
    return content;
}

CabbageCodeEditorComponent* CabbageDocumentWindow::getCurrentCodeEditor()
{
    if(content->getCurrentCodeEditor())
        return content->getCurrentCodeEditor()->editor;
    else
        return nullptr;
}

CabbageOutputConsole* CabbageDocumentWindow::getCurrentOutputConsole()
{
    if(content->getCurrentCodeEditor())
        return content->getCurrentCodeEditor()->outputConsole;
    else
        return nullptr;
}

String CabbageDocumentWindow::getAudioDeviceSettings()
{
    if(audioGraph->getDeviceManagerSettings().isNotEmpty())
        return audioGraph->getDeviceManagerSettings();
    else
        return String::empty;
}

CabbagePluginEditor* CabbageDocumentWindow::getCabbagePluginEditor()
{
    if (AudioProcessorGraph::Node::Ptr f = audioGraph->graph.getNodeForId (1))
    {
        AudioProcessor* const processor = f->getProcessor();
        //need to check what kind of processor we are dealing with!
        if(CabbagePluginEditor* editor = dynamic_cast<CabbagePluginEditor*>(processor->getActiveEditor()))
            return editor;
    }

    return nullptr;
}

CabbagePluginProcessor* CabbageDocumentWindow::getCabbagePluginProcessor()
{
    if (AudioProcessorGraph::Node::Ptr f = audioGraph->graph.getNodeForId (1))
    {
        if(CabbagePluginProcessor* const processor = dynamic_cast<CabbagePluginProcessor*>(f->getProcessor()))
            return processor;
    }

    return nullptr;
}
//==============================================================================
void CabbageDocumentWindow::updateEditorColourScheme()
{
    this->getLookAndFeel().setColour(PropertyComponent::ColourIds::backgroundColourId, CabbageSettings::getColourFromValueTree(cabbageSettings->getValueTree(), CabbageColourIds::propertyLabelBackground, Colour(50,50,50)));
    this->getLookAndFeel().setColour(PropertyComponent::ColourIds::labelTextColourId, CabbageSettings::getColourFromValueTree(cabbageSettings->getValueTree(), CabbageColourIds::propertyLabelText, Colour(50,50,50)));
    this->lookAndFeelChanged();
    content->propertyPanel->setBackgroundColour(CabbageSettings::getColourFromValueTree(cabbageSettings->getValueTree(), CabbageColourIds::consoleOutline, Colour(50,50,50)));
    content->propertyPanel->setBorderColour(CabbageSettings::getColourFromValueTree(cabbageSettings->getValueTree(), CabbageColourIds::consoleOutline, Colour(50,50,50)));
    int editorIndex = content->editorAndConsole.size()-1;
    content->getCurrentCodeEditor()->updateLookAndFeel();
}
//==============================================================================
void CabbageDocumentWindow::changeListenerCallback(ChangeBroadcaster* source)
{
    if(CabbageSettings* settings = dynamic_cast<CabbageSettings*>(source)) // update lookandfeel whenever a user changes colour settings
    {
        lookAndFeel->refreshLookAndFeel(cabbageSettings->getValueTree());
        lookAndFeelChanged();
        updateEditorColourScheme();
    }

    else if(CabbagePluginEditor* editor = dynamic_cast<CabbagePluginEditor*>(source)) // update Cabbage code when user drags a widget around
    {
        content->propertyPanel->setVisible(true);
        content->resized();
        ValueTree widgetData = editor->getValueTreesForCurrentlySelectedComponents()[0];
        content->propertyPanel->updateProperties(widgetData);

        if(CabbageWidgetData::getNumProp(widgetData, CabbageIdentifierIds::linenumber)>9999) //if widget was added in edit mode...
        {
            StringArray csdArray;
            csdArray.addLines(getCurrentCodeEditor()->getDocument().getAllContent());
            for( int i = 0 ; i < csdArray.size() ; i++ )
            {
                if(csdArray[i].contains("</Cabbage>"))
                {
                    CabbageWidgetData::setNumProp(widgetData, CabbageIdentifierIds::linenumber, i);
                    updateCodeInEditor(editor, false);
                }
            }
        }
        else
        {
            updateCodeInEditor(editor, true);
            updateEditorColourScheme();
        }
    }

    else if(CabbagePropertiesPanel* panel = dynamic_cast<CabbagePropertiesPanel*>(source)) // update code when a user changes a property
    {
        if(CabbagePluginEditor* editor = this->getCabbagePluginEditor())
        {
            updateCodeInEditor(editor, true);
        }
    }
	
	
    else if(CabbageCodeEditorComponent* codeEditor = dynamic_cast<CabbageCodeEditorComponent*>(source)) // update code when a user changes a property
    {
		if(getCabbagePluginProcessor() != nullptr && getCabbagePluginEditor()->isEditModeEnabled())
			this->getCabbagePluginProcessor()->updateWidgets(codeEditor->getAllText());
    }	
}

//=======================================================================================
void CabbageDocumentWindow::updateCodeInEditor(CabbagePluginEditor* editor, bool replaceExistingLine)
{
    content->propertyPanel->addChangeListener(this);
	
    for(ValueTree wData : editor->getValueTreesForCurrentlySelectedComponents())
    {
        const int lineNumber = CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::linenumber);
        const String newText = CabbageWidgetData::getCabbageCodeFromIdentifiers(wData);
		//prevent this change from sending a change message
		getCurrentCodeEditor()->shouldSendUpdateMessage = false;
        getCurrentCodeEditor()->insertCode(lineNumber, newText, replaceExistingLine, editor->getValueTreesForCurrentlySelectedComponents().size()==1);
    }
}

//=======================================================================================
void CabbageDocumentWindow::setEditMode(bool enable)
{
    if(!getCabbagePluginEditor())
	{
		createAudioGraph();
	}

	getCabbagePluginEditor()->addChangeListener(this);
	if(enable==true)
	{
		audioGraph->stopPlaying();
		getMainContentComponent()->propertyPanel->setInterceptsMouseClicks(true, true);
	}
	else
	{
		audioGraph->startPlaying();
		getMainContentComponent()->propertyPanel->setInterceptsMouseClicks(false, false);
	}

	getCabbagePluginEditor()->enableEditMode(enable);
	getMainContentComponent()->propertyPanel->setEnabled(enable);

}
//=======================================================================================
void CabbageDocumentWindow::showSettingsDialog()
{
    DialogWindow::LaunchOptions o;
    o.content.setOwned(new CabbageSettingsWindow(*cabbageSettings, audioGraph->getAudioDeviceSelector()));
    o.content->setSize(500, 450);
    o.dialogTitle = TRANS("Cabbage Settings");
    o.dialogBackgroundColour = Colour(0xfff0f0f0);
    o.escapeKeyTriggersCloseButton = true;
    o.useNativeTitleBar = true;
    o.resizable = false;
    o.launchAsync();
}

//==============================================================================
void CabbageDocumentWindow::createNewProject()
{
    Logger::writeToLog("CreateNewProject");

    DialogWindow::LaunchOptions o;
    o.content.setOwned(new CabbageProjectWindow(this, cabbageSettings->getValueTree()));
    o.content->setSize(650, 350);

    o.dialogTitle = TRANS("Select new project type");
    o.dialogBackgroundColour = Colour(0xfff0f0f0);
    o.escapeKeyTriggersCloseButton = true;
    o.useNativeTitleBar = true;
    o.resizable = false;

    o.launchAsync();
}
//==============================================================================
void CabbageDocumentWindow::askUserToOpenFile()
{
    FileChooser fc ("Open File");

    if (fc.browseForFileToOpen())
        openFile (fc.getResult());
}
//==============================================================================
bool CabbageDocumentWindow::openFile (const File& file, String type)
{
    stopTimer();
    stopCode();

    PluginWindow::closeAllCurrentlyOpenWindows();

    getMainContentComponent()->openFile(file);
	getCurrentCodeEditor()->addChangeListener(this);
    cabbageSettings->updateRecentFilesList(file);
    setName("Cabbage " + file.getFullPathName());
    currentCsdFile = File(file.getFullPathName());
    return true;
}
//==============================================================================
void CabbageDocumentWindow::saveDocument(bool saveAs)
{

    if(saveAs == true)
    {
        isGUIEnabled = false;
        if(getCabbagePluginEditor()!=nullptr)
            getCabbagePluginEditor()->enableEditMode(false);

        FileChooser fc ("Select file name and location", File::getSpecialLocation(File::SpecialLocationType::userHomeDirectory));

        if (fc.browseForFileToSave(false))
        {
            if(fc.getResult().existsAsFile())
            {
                const int result = CabbageUtilities::showYesNoMessage("Do you wish to overwrite\nexiting file?", lookAndFeel);
                if(result==0)
                {
                    fc.getResult().replaceWithText(currentCsdFile.loadFileAsString());
                    openFile(fc.getResult());
                }
            }
            else
            {
                fc.getResult().replaceWithText(currentCsdFile.loadFileAsString());
                openFile(fc.getResult());
            }
        }
    }
    else
    {
        isGUIEnabled = false;
        if(getCabbagePluginEditor()!=nullptr)
            getCabbagePluginEditor()->enableEditMode(false);

        if(currentCsdFile.existsAsFile())
            currentCsdFile.replaceWithText(getCurrentCodeEditor()->getDocument().getAllContent());

        if(cabbageSettings->getUserSettings()->getIntValue("CompileOnSave")==1)
        {
            getMainContentComponent()->propertyPanel->setEnabled(false);
            createAudioGraph();
        }
    }
}

//==============================================================================
void CabbageDocumentWindow::timerCallback()
{
    if(currentCsdFile.existsAsFile())
    {
        const String csoundOutputString = audioGraph->getCsoundOutput();
        consoleMessages+=csoundOutputString;
        if(csoundOutputString.length()>0)
        {
            getCurrentOutputConsole()->setText(csoundOutputString);
        }
    }
}
//==============================================================================
void CabbageDocumentWindow::runCode()
{
    if(currentCsdFile.existsAsFile())
    {
        PluginWindow::closeAllCurrentlyOpenWindows();
        createAudioGraph(); //in future versions we can simply edit the node in question and reconnect within the graph
        startTimer(100);
    }
    else
        CabbageUtilities::showMessage("Warning", "Please open a file first", lookAndFeel);
}

void CabbageDocumentWindow::stopCode()
{
    if(currentCsdFile.existsAsFile())
    {
        stopTimer();
        audioGraph->stopPlaying();
    }
}

//==============================================================================
void CabbageDocumentWindow::createAudioGraph()
{
    const Point<int> lastPoint = PluginWindow::getPositionOfCurrentlyOpenWindow(1);

    if(lastPoint.getX()>0)
    {
        cabbageSettings->setProperty("windowX", lastPoint.getX());
        cabbageSettings->setProperty("windowY", lastPoint.getY());
    }

    audioGraph = new AudioGraph(cabbageSettings->getUserSettings(), currentCsdFile, false);
    audioGraph->setXmlAudioSettings(cabbageSettings->getUserSettings()->getXmlValue("audioSetup"));

    createEditorForAudioGraphNode();
	if(getCurrentCodeEditor() != nullptr)
		getCurrentCodeEditor()->breakpointData = getCabbagePluginProcessor()->breakPointData.valueTree;

}

void CabbageDocumentWindow::createEditorForAudioGraphNode()
{
    const int numParameters = audioGraph->getNumberOfParameters();
    if(numParameters>0)
    {

        if (AudioProcessorGraph::Node::Ptr f = audioGraph->graph.getNodeForId (1))
        {
            AudioProcessor* const processor = f->getProcessor();

            PluginWindow::WindowFormatType type = audioGraph->getProcessor()->hasEditor() ? PluginWindow::Normal
                                                  : PluginWindow::Generic;

            if (PluginWindow* const w = PluginWindow::getWindowFor(f, type, audioGraph->graph))
            {
                w->toFront (true);
                Point<int> point(cabbageSettings->getIntProperty("windowX"),
                                 cabbageSettings->getIntProperty("windowY"));

                if(point.getY()>0 && point.getX()>0)
                    w->setTopLeftPosition(point.getX(), point.getY());
            }
        }
    }
}

//==============================================================================
bool CabbageDocumentWindow::closeAllDocuments (bool askUserToSave)
{
    // return openDocumentManager.closeAll (askUserToSave);
    return true;
}

bool CabbageDocumentWindow::closeAllMainWindows()
{
    //return server != nullptr || mainWindowList.askAllWindowsToClose();
    return true;
}
void CabbageDocumentWindow::closeButtonPressed()
{
    JUCEApplicationBase::quit();
}