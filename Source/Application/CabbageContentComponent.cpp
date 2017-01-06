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

//==============================================================================
CabbageContentComponent::CabbageContentComponent(CabbageDocumentWindow* owner, CabbageSettings* settings): cabbageSettings(settings), owner(owner), factory(this)
{
    addAndMakeVisible(propertyPanel = new CabbagePropertiesPanel(cabbageSettings->valueTree));
    propertyPanel->setVisible(false);
    setSize (1200, 800);

    bgImage = createBackground();

    addAndMakeVisible (toolbar);
    toolbar.addDefaultItems (factory);
    propertyPanel->addChangeListener(this);
    factory.combo->getCombo().addListener(this);

    setLookAndFeelColours();
}

CabbageContentComponent::~CabbageContentComponent()
{
    editorAndConsole.clear();
    audioGraph = nullptr;
}

void CabbageContentComponent::paint (Graphics& g)
{
    if(editorAndConsole.size()==0)
        g.drawImage(bgImage, getLocalBounds().toFloat());
    else
        g.fillAll( CabbageSettings::getColourFromValueTree(cabbageSettings->valueTree, CabbageColourIds::lineNumberBackground, Colour(50,50,50)));

}

void CabbageContentComponent::resized()
{
    const int heightOfTabButtons = (editorAndConsole.size()>1) ? 25 : 0;
    if (toolbar.isVertical())
        toolbar.setBounds (getLocalBounds().removeFromLeft (toolbarThickness));
    else
        toolbar.setBounds (getLocalBounds().removeFromTop  (toolbarThickness));

    if(propertyPanel->isVisible())
    {
        propertyPanel->setBounds(getWidth()-200, toolbar.getHeight(), 200, getHeight()-toolbar.getHeight());
    }

    resizeAllEditorAndConsoles(toolbarThickness + heightOfTabButtons);
}

void CabbageContentComponent::setLookAndFeelColours()
{
    lookAndFeel = new CabbageIDELookAndFeel();
    lookAndFeel->setColour(Slider::ColourIds::thumbColourId, Colour(110, 247, 0));
    lookAndFeel->setColour(ScrollBar::backgroundColourId, Colour(70, 70, 70));
    toolbar.setColour(Toolbar::backgroundColourId, CabbageSettings::getColourFromValueTree(cabbageSettings->getValueTree(), CabbageColourIds::menuBarBackground, Colour(50,50,50)));
    toolbar.setColour(Toolbar::ColourIds::buttonMouseOverBackgroundColourId, CabbageSettings::getColourFromValueTree(cabbageSettings->getValueTree(), CabbageColourIds::menuBarBackground, Colour(50,50,50)).contrasting(.3f));
    toolbar.setColour(Toolbar::ColourIds::buttonMouseDownBackgroundColourId, CabbageSettings::getColourFromValueTree(cabbageSettings->getValueTree(), CabbageColourIds::menuBarBackground, Colour(50,50,50)).contrasting(.5f));
    owner->lookAndFeelChanged();
    lookAndFeel->refreshLookAndFeel(cabbageSettings->getValueTree());
    lookAndFeelChanged();
    toolbar.repaint();
}

bool CabbageContentComponent::setCurrentCsdFile(File file)
{
    if(currentCsdFile==file)
    {
        CabbageUtilities::showMessage("File is already open", lookAndFeel);
        return false;
    }
    else
        currentCsdFile = file;

    return true;
}
//==============================================================================
void CabbageContentComponent::buttonClicked(Button* button)
{
    if(const TextButton* tabButton = dynamic_cast<TextButton*>(button))
    {
        currentFileIndex = fileTabs.indexOf(tabButton);
        editorAndConsole[currentFileIndex]->toFront(true);
        if(CabbageDocumentWindow* docWindow = this->findParentComponentOfClass<CabbageDocumentWindow>())
        {
            docWindow->setName(openFiles[currentFileIndex].getFileName());
            currentCsdFile = openFiles[currentFileIndex];
            addInstrumentsAndRegionsToCombobox();
        }
    }
    else if(const ToolbarButton* toolbarButton = dynamic_cast<ToolbarButton*>(button))
    {
        if(toolbarButton->getName()=="new")
        {
            createNewProject();
        }
        else if(toolbarButton->getName()=="open")
        {
            openFile();
        }
        else if(toolbarButton->getName()=="save")
        {
            saveDocument(false);
        }
        else if(toolbarButton->getName()=="save as")
        {
            saveDocument(true);
        }
        else if(toolbarButton->getName()=="togglePlay")
        {
            if(toolbarButton->getToggleState())
                this->runCode();
            else
                this->stopCode();
        }

    }
}

void CabbageContentComponent::comboBoxChanged (ComboBox *comboBoxThatHasChanged)
{
    int index = comboBoxThatHasChanged->getSelectedId();
    const int lineToScrollTo = getCurrentCodeEditor()->instrumentsAndRegions.getValueAt(index-1);
    getCurrentCodeEditor()->scrollToLine(lineToScrollTo);
}
//==============================================================================
void CabbageContentComponent::changeListenerCallback(ChangeBroadcaster* source)
{
    if(CabbageSettings* settings = dynamic_cast<CabbageSettings*>(source)) // update lookandfeel whenever a user changes colour settings
    {
        lookAndFeel->refreshLookAndFeel(cabbageSettings->getValueTree());
        lookAndFeelChanged();
        owner->lookAndFeelChanged();
        updateEditorColourScheme();
    }

    else if(CabbagePluginEditor* editor = dynamic_cast<CabbagePluginEditor*>(source)) // update Cabbage code when user drags a widget around
    {
        propertyPanel->setVisible(true);
        propertyPanel->setEnabled(true);
        resized();
        ValueTree widgetData = editor->getValueTreesForCurrentlySelectedComponents()[0];
        propertyPanel->updateProperties(widgetData);

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
            updateEditorColourScheme(); //keep look and feel updated..
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
        if(getCabbagePluginEditor() != nullptr && getCabbagePluginEditor()->isEditModeEnabled())
            this->getCabbagePluginProcessor()->updateWidgets(codeEditor->getAllText());
    }
}

//=======================================================================================
void CabbageContentComponent::updateCodeInEditor(CabbagePluginEditor* editor, bool replaceExistingLine)
{
    propertyPanel->addChangeListener(this);

    for(ValueTree wData : editor->getValueTreesForCurrentlySelectedComponents())
    {
        const int lineNumber = CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::linenumber);
        const String parent = CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::parentcomponent); // if widget has a parent don't highlight line

        const String currentLineText = getCurrentCodeEditor()->getLineText(lineNumber);

        const String newText = CabbageWidgetData::getCabbageCodeFromIdentifiers(wData, currentLineText);


        getCurrentCodeEditor()->insertCode(lineNumber, newText, replaceExistingLine, parent.isEmpty() == true ? true : false);

    }
}
//==============================================================================
void CabbageContentComponent::timerCallback()
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
void CabbageContentComponent::updateEditorColourScheme()
{
    getLookAndFeel().setColour(PropertyComponent::ColourIds::backgroundColourId, CabbageSettings::getColourFromValueTree(cabbageSettings->getValueTree(), CabbageColourIds::propertyLabelBackground, Colour(50,50,50)));
    getLookAndFeel().setColour(PropertyComponent::ColourIds::labelTextColourId, CabbageSettings::getColourFromValueTree(cabbageSettings->getValueTree(), CabbageColourIds::propertyLabelText, Colour(50,50,50)));
    lookAndFeelChanged();
    propertyPanel->setBackgroundColour(CabbageSettings::getColourFromValueTree(cabbageSettings->getValueTree(), CabbageColourIds::consoleOutline, Colour(50,50,50)));
    propertyPanel->setBorderColour(CabbageSettings::getColourFromValueTree(cabbageSettings->getValueTree(), CabbageColourIds::consoleOutline, Colour(50,50,50)));
    int editorIndex = editorAndConsole.size()-1;
    getCurrentEditorAndConsole()->updateLookAndFeel();
    toolbar.setColour(Toolbar::backgroundColourId, CabbageSettings::getColourFromValueTree(cabbageSettings->getValueTree(), CabbageColourIds::menuBarBackground, Colour(50,50,50)));
    toolbar.repaint();
}
//==============================================================================
Image CabbageContentComponent::createBackground()
{
    Image backgroundImg;
    backgroundImg = Image(Image::RGB, getWidth(), getHeight(), true);
    ScopedPointer<Drawable> drawable;
    const int whiteScale = 150;
    Graphics g(backgroundImg);
    {
        g.fillAll(Colour(whiteScale,whiteScale,whiteScale));
        Random pos, width, colour;
        for(int i=0; i<getWidth();)
        {
            const int brightness = colour.nextInt(Range<int>(whiteScale, whiteScale+5));
            g.setColour(Colour(brightness, brightness, brightness));
            g.drawLine(i, 0, i+width.nextInt(Range<int>(0, 10)), getHeight() );
            g.drawLine(0, i, getWidth(), i+width.nextInt(Range<int>(0, 10)));
            i += pos.nextInt(Range<int>(0, 5));
        }

        const Image cabbageLogo = ImageCache::getFromMemory (CabbageBinaryData::CabbageLogoBig_png, CabbageBinaryData::CabbageLogoBig_pngSize);
        g.drawImage(cabbageLogo, getLocalBounds().toFloat(), RectanglePlacement::Flags::doNotResize);
        return backgroundImg;
    }
}

//==============================================================================
void CabbageContentComponent::addFileTabButton(File file, int xPos)
{
    TextButton* fileButton;
    fileTabs.add(fileButton = new TextButton(file.getFileName()));
    addAndMakeVisible(fileButton);
    fileButton->setBounds(xPos, toolbarThickness+3, 90, 20);
    fileButton->addListener(this);
    fileButton->setRadioGroupId(99);
    fileButton->setClickingTogglesState(true);
    fileButton->setLookAndFeel(lookAndFeel);
    fileButton->setToggleState(true, sendNotification);
    currentFileIndex = fileTabs.size()-1;
}

void CabbageContentComponent::addInstrumentsAndRegionsToCombobox()
{
    factory.combo->clearItemsFromComboBox();

    const NamedValueSet instrRegions = getCurrentCodeEditor()->instrumentsAndRegions;
    for( int i = 0 ; i < instrRegions.size() ; i++)
    {
        factory.combo->addItemsToComboBox(instrRegions.getName(i).toString(), i+1);
    }
}
//==============================================================================
void CabbageContentComponent::resizeAllEditorAndConsoles(int height)
{
    const bool isPropPanelVisible = propertyPanel->isVisible();
    for( CabbageEditorContainer* editor : editorAndConsole )
        editor->setBounds(0, height, getWidth() - (isPropPanelVisible ? 200 : 0), getHeight());
}

//==============================================================================
void CabbageContentComponent::createAudioGraph()
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
//    if(getCurrentCodeEditor() != nullptr)
//        getCurrentCodeEditor()->breakpointData = getCabbagePluginProcessor()->breakPointData.valueTree;


}

//==============================================================================
void CabbageContentComponent::rebuildAudioGraph()
{
    const Point<int> lastPoint = PluginWindow::getPositionOfCurrentlyOpenWindow(1);

    if(lastPoint.getX()>0)
    {
        cabbageSettings->setProperty("windowX", lastPoint.getX());
        cabbageSettings->setProperty("windowY", lastPoint.getY());
    }

    audioGraph->deletePlugin();
    audioGraph->createPlugin(currentCsdFile);
    //audioGraph = new AudioGraph(cabbageSettings->getUserSettings(), currentCsdFile, false);
    //audioGraph->setXmlAudioSettings(cabbageSettings->getUserSettings()->getXmlValue("audioSetup"));

    createEditorForAudioGraphNode();
    if(getCurrentCodeEditor() != nullptr)
        getCurrentCodeEditor()->breakpointData = getCabbagePluginProcessor()->breakPointData.valueTree;

}

void CabbageContentComponent::createEditorForAudioGraphNode()
{
    if (AudioProcessorGraph::Node::Ptr f = audioGraph->graph.getNodeForId (AudioGraph::NodeType::CabbageNode))
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

//==============================================================================
CabbageEditorContainer* CabbageContentComponent::getCurrentEditorAndConsole()
{
    if(editorAndConsole.size()>0)
        return editorAndConsole[currentFileIndex];

    return nullptr;
}

CabbageCodeEditorComponent* CabbageContentComponent::getCurrentCodeEditor()
{
    if(getCurrentEditorAndConsole())
        return getCurrentEditorAndConsole()->editor;
    else
        return nullptr;
}

CabbageOutputConsole* CabbageContentComponent::getCurrentOutputConsole()
{
    if(getCurrentEditorAndConsole())
        return getCurrentEditorAndConsole()->outputConsole;
    else
        return nullptr;
}

String CabbageContentComponent::getAudioDeviceSettings()
{
    if(audioGraph!=nullptr && audioGraph->getDeviceManagerSettings().isNotEmpty())
        return audioGraph->getDeviceManagerSettings();
    else
        return String::empty;
}

CabbagePluginEditor* CabbageContentComponent::getCabbagePluginEditor()
{
    if (AudioProcessorGraph::Node::Ptr f = audioGraph->graph.getNodeForId (AudioGraph::NodeType::CabbageNode))
    {
        AudioProcessor* const processor = f->getProcessor();
        //need to check what kind of processor we are dealing with!
        if(CabbagePluginEditor* editor = dynamic_cast<CabbagePluginEditor*>(processor->getActiveEditor()))
            return editor;
    }

    return nullptr;
}

CabbagePluginProcessor* CabbageContentComponent::getCabbagePluginProcessor()
{
    if (AudioProcessorGraph::Node::Ptr f = audioGraph->graph.getNodeForId (AudioGraph::NodeType::CabbageNode))
    {
        if(CabbagePluginProcessor* const processor = dynamic_cast<CabbagePluginProcessor*>(f->getProcessor()))
            return processor;
    }

    return nullptr;
}

//=======================================================================================
void CabbageContentComponent::setEditMode(bool enable)
{
    if(audioGraph->getIsCabbageFile() == true)
    {
        if(!getCabbagePluginEditor())
        {
            createAudioGraph();
        }

        getCabbagePluginEditor()->addChangeListener(this);
        if(enable==true)
        {
            audioGraph->stopPlaying();
            propertyPanel->setInterceptsMouseClicks(true, true);
        }
        else
        {
            audioGraph->startPlaying();
            propertyPanel->setInterceptsMouseClicks(false, false);
        }

        getCabbagePluginEditor()->enableEditMode(enable);
    }
}
//=======================================================================================
void CabbageContentComponent::showSettingsDialog()
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
void CabbageContentComponent::createNewProject()
{
    Logger::writeToLog("CreateNewProject");

    DialogWindow::LaunchOptions o;
    o.content.setOwned(new CabbageProjectWindow(this));
    o.content->setSize(650, 350);

    o.dialogTitle = TRANS("Select new project type");
    o.dialogBackgroundColour = Colour(0xfff0f0f0);
    o.escapeKeyTriggersCloseButton = true;
    o.useNativeTitleBar = true;
    o.resizable = false;

    o.launchAsync();
}

//==============================================================================
void CabbageContentComponent::launchSSHFileBrowser(String mode)
{
    const String sshAddress = cabbageSettings->getUserSettings()->getValue("SSHAddress");
    const String sshHome = cabbageSettings->getUserSettings()->getValue("SSHHomeDir");
    DialogWindow::LaunchOptions o;
	CabbageUtilities::debug(currentCsdFile.getFullPathName());
    o.content.setOwned(new CabbageSSHFileBrowser(sshAddress, sshHome, this, mode, currentCsdFile.getFullPathName()));
    o.content->setSize(650, 350);

    o.dialogTitle = TRANS("Browse Raspberry PI for files..");
    o.dialogBackgroundColour = Colour(0xfff0f0f0);
    o.escapeKeyTriggersCloseButton = true;
    o.useNativeTitleBar = true;
    o.resizable = false;

    o.launchAsync();
}
//==============================================================================
void CabbageContentComponent::openFile(String filename)
{

    stopTimer();
    stopCode();

    PluginWindow::closeAllCurrentlyOpenWindows();



    if(File(filename).existsAsFile() == false)
    {
        FileChooser fc ("Open File", cabbageSettings->getMostRecentFile().getParentDirectory());

        if (fc.browseForFileToOpen())
        {
            if(setCurrentCsdFile(fc.getResult()) == false)
                return;
        }
    }
    else if(setCurrentCsdFile(File(filename)) == false)
        return;


    cabbageSettings->updateRecentFilesList(currentCsdFile);

    CabbageEditorContainer* editorConsole;
    editorAndConsole.add(editorConsole = new CabbageEditorContainer(cabbageSettings->valueTree));
    addAndMakeVisible(editorConsole);
    addAndMakeVisible(propertyPanel = new CabbagePropertiesPanel(cabbageSettings->valueTree));
    propertyPanel->setVisible(false);
    editorConsole->setVisible(true);
    editorConsole->toFront(true);
    openFiles.add(currentCsdFile);
    editorConsole->editor->loadContent(currentCsdFile.loadFileAsString());
    editorConsole->editor->parseTextForInstrumentsAndRegions();


    editorConsole->editor->startThread();
    numberOfFiles = editorAndConsole.size()-1;
    resized();

    if(numberOfFiles==1)
    {
        addFileTabButton(openFiles[0], 10);
        addFileTabButton(openFiles[1], 105);
    }
    else if(numberOfFiles>1)
    {
        addFileTabButton(openFiles[numberOfFiles], 10+numberOfFiles*95);
    }

    getCurrentCodeEditor()->addChangeListener(this);
    owner->setName("Cabbage " + currentCsdFile.getFullPathName());
    addInstrumentsAndRegionsToCombobox();


}
//==============================================================================
void CabbageContentComponent::saveDocument(bool saveAs)
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
                    openFile(fc.getResult().getFullPathName());
                }
            }
            else
            {
                fc.getResult().replaceWithText(currentCsdFile.loadFileAsString());
                openFile(fc.getResult().getFullPathName());
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
            propertyPanel->setEnabled(false);
            runCode();
        }
    }
}

//==============================================================================
void CabbageContentComponent::runCode()
{
    if(currentCsdFile.existsAsFile())
    {
        PluginWindow::closeAllCurrentlyOpenWindows();
        audioGraph = nullptr;
        createAudioGraph(); //in future versions we can simply edit the node in question and reconnect within the graph

        //rebuildAudioGraph();
        startTimer(100);
        factory.togglePlay(true);
    }
    else
        CabbageUtilities::showMessage("Warning", "Please open a file first", lookAndFeel);
}

void CabbageContentComponent::stopCode()
{
    if(currentCsdFile.existsAsFile())
    {
        stopTimer();
        factory.togglePlay(false);
        audioGraph->stopPlaying();
    }
}
