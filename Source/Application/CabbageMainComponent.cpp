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
      tooltipWindow (this, 300),
      cycleTabsButton ("...")
{

    cycleTabsButton.setColour (TextButton::ColourIds::buttonColourId, Colour (100, 100, 100));
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
    cycleTabsButton.setSize (50, 28);
    cycleTabsButton.addListener (this);
    setLookAndFeelColours();
    createAudioGraph(); //set up graph even though no file is selected. Allows users to change audio devices from the get-go..

}

CabbageMainComponent::~CabbageMainComponent()
{
    editorAndConsole.clear();
    graphComponent = nullptr;
    audioGraph = nullptr;
    setLookAndFeel(nullptr);

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
    if (FileTab* tabButton = dynamic_cast<FileTab*> (button))
        handleFileTab (tabButton);
    else if (ToolbarButton* toolbarButton = dynamic_cast<ToolbarButton*> (button))
        handleToolbarButtons (toolbarButton);
    else if (DrawableButton* drawableButton = dynamic_cast<DrawableButton*> (button))
        handleFileTabs (drawableButton);
    else if (TextButton* textButton = dynamic_cast<TextButton*> (button))
    {
        PopupMenu menu;

        for ( int i = 0 ; i < fileTabs.size() ; i++)
            if (fileTabs[i]->isVisible() == false)
                menu.addItem (i + 1, fileTabs[i]->getName());

        const int result = menu.show();

        if ( result > 0 )
        {
            fileTabs.move (result - 1, fileTabs.size() - 1);
            editorAndConsole.move (result - 1, fileTabs.size() - 1);
            arrangeFileTabs();
            fileTabs[fileTabs.size() - 1]->setToggleState (true, sendNotification);
            setCurrentCsdFile (fileTabs[fileTabs.size() - 1]->getFile());
        }
    }
}

void CabbageMainComponent::handleFileTab (FileTab* tabButton, bool increment)
{

    if (increment && tabButton == nullptr)
    {
        currentFileIndex = (currentFileIndex  < fileTabs.size() - 1 ? currentFileIndex + 1 : 0);
    }
    else
        currentFileIndex = fileTabs.indexOf (tabButton);

    hideFindPanel();
    editorAndConsole[currentFileIndex]->toFront (true);
    cabbageSettings->setProperty ("MostRecentFile", fileTabs[currentFileIndex]->getFile().getFullPathName());


    if (CabbageDocumentWindow* docWindow = this->findParentComponentOfClass<CabbageDocumentWindow>())
    {
        docWindow->setName (fileTabs[currentFileIndex]->getFile().getFullPathName());
        addInstrumentsAndRegionsToCombobox();
    }

    if (increment == false)
    {
        for ( auto button : fileTabs )
        {
            if (button != tabButton)
                button->disableButtons (true);
            else
                button->disableButtons (false);
        }
    }
    else
    {
        for ( int i = 0 ; i < fileTabs.size() ; i++)
        {
            if (currentFileIndex == i)
            {
                fileTabs[i]->setToggleState (true, dontSendNotification);
                fileTabs[i]->disableButtons (false);
            }
            else
                fileTabs[i]->disableButtons (true);

        }

        this->arrangeFileTabs();
    }


}

int CabbageMainComponent::getTabFileIndex (File file)
{
    bool foundFile = false;
    int fileIndex = 0;

    for ( auto tab : fileTabs)
    {
        if (tab->getFilename() == file.getFullPathName())
        {
            foundFile = true;
            break;
        }
        else
            fileIndex++;
    }

    return foundFile == false ? -1 : fileIndex;

}

int CabbageMainComponent::getTabFileIndex (int32 nodeId)
{
    bool foundFile = false;
    int fileIndex = 0;

    for ( auto tab : fileTabs)
    {
        if (tab->uniqueFileId == nodeId)
        {
            foundFile = true;
            break;
        }
        else
            fileIndex++;
    }

    return foundFile == false ? -1 : fileIndex;

}

void CabbageMainComponent::bringCodeEditorToFront (File file)
{
    const int fileIndex = getTabFileIndex (file);

    if (fileIndex >= 0)
    {
        fileTabs[fileIndex]->setToggleState (true, sendNotification);
        currentFileIndex = fileIndex;
    }
    else
        this->openFile (file.getFullPathName());

}

void CabbageMainComponent::bringCodeEditorToFront (FileTab* tab)
{

    tab->setToggleState (true, sendNotification);
    int fileIndex = -1;
    for( int i = 0 ; i < fileTabs.size() ; i++)
    {
        if(fileTabs[i] == tab)
            fileIndex = i;

    }

    currentFileIndex = fileIndex;
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

void CabbageMainComponent::handleFileTabs (DrawableButton* drawableButton)
{
    if (drawableButton->getName() == "playButton")
    {
        if (drawableButton->getProperties().getWithDefault ("state", "") == "off")
            saveDocument();
        else
            stopCsoundForNode (drawableButton->getProperties().getWithDefault ("filename", ""));

    }
    else if (drawableButton->getName() == "closeButton")
    {
        if (FileTab* tabButton = drawableButton->findParentComponentOfClass<FileTab>())
        {
            currentFileIndex = fileTabs.indexOf (tabButton);
            editorAndConsole[currentFileIndex]->toFront (true);
            closeDocument();
        }
    }
    else if (drawableButton->getName() == "showEditorButton")
    {
        if (FileTab* tabButton = drawableButton->findParentComponentOfClass<FileTab>())
        {
            const String filename = tabButton->getFilename();
            int32 nodeId = fileTabs[currentFileIndex]->uniqueFileId;

            if (AudioProcessorGraph::Node::Ptr f = audioGraph->graph.getNodeForId (nodeId))
            {
                PluginWindow::WindowFormatType type = f->getProcessor()->hasEditor() ? PluginWindow::Normal
                                                      : PluginWindow::Generic;

                if (PluginWindow* const w = PluginWindow::getWindowFor (f, type, audioGraph->graph))
                {
                    if (w->isVisible())
                        w->setVisible (false);
                    else
                        w->setVisible (true);
                }
            }

        }

    }
    else if (drawableButton->getName() == "editGUIButton")
    {
        this->saveDocument();
        setEditMode (true);
        if(FileTab* tabButton = drawableButton->findParentComponentOfClass<FileTab>())
        {
            tabButton->setToggleState(false, dontSendNotification);
            tabButton->getPlayButton().getProperties().set("state", "off");
        }

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
        if(File(editor->changeMessage).existsAsFile())  //custom plant has been added...
        {
            insertCustomPlantToEditor(editor);
            //editor->changeMessage = "";
        }
        else
        {
            resized();
            ValueTree widgetData = editor->getValueTreesForCurrentlySelectedComponents()[0];
            const String typeOfWidget = CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::type);
            CabbageLayoutWidgetStrings layoutWidgets;
            CabbageControlWidgetStrings controlWidgets;
            ;

            if (CabbageWidgetData::getNumProp(widgetData, CabbageIdentifierIds::surrogatelinenumber)<0)
                //layoutWidgets.contains(typeOfWidget)
                //    || controlWidgets.contains(typeOfWidget)) //if not custom plant
            {
                //recreate/update code for standard components, dealing with custom plants later
                propertyPanel->setVisible(true);
                propertyPanel->setEnabled(true);
                propertyPanel->saveOpenessState();
                propertyPanel->updateProperties(widgetData);
                resized();

                if (CabbageWidgetData::getNumProp(widgetData, CabbageIdentifierIds::linenumber) >
                    9999) //if widget was added in edit mode...
                {
                    StringArray csdArray;
                    csdArray.addLines(getCurrentCodeEditor()->getDocument().getAllContent());

                    for (int i = 0; i < csdArray.size(); i++)
                    {
                        if (csdArray[i].contains(
                                "</Cabbage>")) //add new text just before the end of the Cabbage section of code
                        {
                            CabbageWidgetData::setNumProp(widgetData, CabbageIdentifierIds::linenumber, i);
                            updateCodeInEditor(editor, false);
                            updateEditorColourScheme(); //keep look and feel updated..
                            return;
                        }
                    }
                }
                else
                {
                    updateCodeInEditor(editor, true);
                    updateEditorColourScheme(); //keep look and feel updated..
                }
            }
            else
            {
                int lineNumberOfCustomPlant = CabbageWidgetData::getNumProp(widgetData,
                                                                            CabbageIdentifierIds::surrogatelinenumber);
                const Rectangle<int> rect = CabbageWidgetData::getBounds(widgetData);
                const String newBounds = CabbageWidgetData::getBoundsTextAsCabbageCode(rect);
                String newLine = CabbageWidgetData::replaceIdentifier(
                        getCurrentCodeEditor()->getLineText(lineNumberOfCustomPlant), "bounds", newBounds);

                getCurrentCodeEditor()->insertCode(lineNumberOfCustomPlant, newLine, true, true);

            }
        }
    }

    else if (dynamic_cast<CabbagePropertiesPanel*> (source)) // update code when a user changes a property
    {
        if (CabbagePluginEditor* ed = getCabbagePluginEditor())
        {

            updateCodeInEditor (ed, true, true);
        }
    }


    else if (CabbageCodeEditorComponent* codeEditor = dynamic_cast<CabbageCodeEditorComponent*> (source))
    {
        handleFileTab (nullptr, true);
    }
}

void CabbageMainComponent::insertCustomPlantToEditor(CabbagePluginEditor* editor)
{
    //add import file to form if not already there...
    ValueTree widgetData = editor->getValueTreeForComponent("form");
    CabbageUtilities::debug(CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::name));
    var importFiles = CabbageWidgetData::getProperty(widgetData, CabbageIdentifierIds::importfiles);

    bool alreadyContainsFile = false;
    for ( int i = 0 ; i < importFiles.size() ; i++)
        if(importFiles[i].toString() == File(editor->changeMessage).getRelativePathFrom(getCurrentCsdFile()))
            alreadyContainsFile = true;

    if(alreadyContainsFile == false)
        importFiles.append(File(editor->changeMessage).getRelativePathFrom(getCurrentCsdFile()));


    CabbageWidgetData::setProperty(widgetData, CabbageIdentifierIds::importfiles, importFiles);
    const int lineNumber = int(CabbageWidgetData::getNumProp(widgetData, CabbageIdentifierIds::linenumber));
    const String currentLineText = getCurrentCodeEditor()->getLineText(lineNumber);
    const String newImportFilesIdentifierString = CabbageWidgetData::getMultiItemTextAsCabbageCode(widgetData, CabbageIdentifierIds::importfiles.toString(), "");
    const String updatedText = CabbageWidgetData::replaceIdentifier(currentLineText, CabbageIdentifierIds::importfiles.toString(), newImportFilesIdentifierString);
    getCurrentCodeEditor()->insertCode(lineNumber, updatedText, true, true);
    //updateCodeInEditor(editor, true);

    Range<int> cabbageSection = CabbageUtilities::getCabbageSectionRange(getCurrentCodeEditor()->getAllText());
    String name;
    String namespce;
    ScopedPointer<XmlElement> xml;
    StringArray cabbageCode;
    xml = XmlDocument::parse (CabbageUtilities::getPlantFileAsXmlString(editor->changeMessage));

    if (xml) //if valid xml
    {
        if (xml->hasTagName ("plant"))
        {
            forEachXmlChildElement (*xml, e)
            {
                if (e->getTagName() == "name")
                    name = e->getAllSubText();
                if (e->getTagName() == "namespace")
                    namespce = e->getAllSubText();
                if (e->getTagName() == "cabbagecode")
                    cabbageCode.addLines (e->getAllSubText().replace("\t", " ").trim());
            }
        }
    }

    ValueTree newWidget("temp1");
    CabbageWidgetData::setWidgetState(newWidget, cabbageCode[0], -1);
    CabbageWidgetData::setNumProp(newWidget, CabbageIdentifierIds::left, editor->customPlantPosition.getX());
    CabbageWidgetData::setNumProp(newWidget, CabbageIdentifierIds::top, editor->customPlantPosition.getY());
    const String newText = name + " " + CabbageWidgetData::getBoundsTextAsCabbageCode(CabbageWidgetData::getBounds(newWidget)) +
    " namespace(\"" + namespce + "\")";

    getCurrentCodeEditor()->insertCode(cabbageSection.getEnd(), newText, false, true);
    //setEditMode(false);
    saveDocument();
    setEditMode(true);

}

void CabbageMainComponent::actionListenerCallback (const String& message)
{
    if (message.contains (".csd"))
    {
        openFile (message.replace ("file://", ""));
    }
    else if (message.contains ("delete:"))
    {
        //deleted whatever line is currently selected, we don't need the know the line number...
        //const int lineNumber = String (message.replace ("delete:", "")).getIntValue();
        //getCurrentCodeEditor()->removeLine (getCurrentCodeEditor()->getSel);
        getCurrentCodeEditor()->removeSelectedText();
        saveDocument();
        setEditMode (true);
    }
}
//=======================================================================================
void CabbageMainComponent::updateCodeInEditor (CabbagePluginEditor* editor, bool replaceExistingLine, bool guiPropUpdate)
{
    propertyPanel->addChangeListener (this);

    if(editor->getValueTreesForCurrentlySelectedComponents().size()>0)
    {
        for (ValueTree wData : editor->getValueTreesForCurrentlySelectedComponents())
        {
            int lineNumber = 0;
            Range<int> cabbageSection = CabbageUtilities::getCabbageSectionRange(getCurrentCodeEditor()->getAllText());

            if (CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::linenumber) >= 1 && CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::surrogatelinenumber)<=0)
            {
                CabbageUtilities::debug(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::type));
                CabbageUtilities::debug(CabbageWidgetData::getBounds(wData).toString());
                CabbageUtilities::debug(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::plant));
                CabbageUtilities::debug("linenUmber:", CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::linenumber));
                
                lineNumber = jmin(cabbageSection.getEnd(),
                                  int(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::linenumber)));
                const String type = CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::type);

                const String parent = CabbageWidgetData::getStringProp(wData,
                                                                       CabbageIdentifierIds::parentcomponent); // if widget has a parent don't highlight line

                const String currentLineText = getCurrentCodeEditor()->getLineText(lineNumber);

                const String newText = CabbageWidgetData::getStringProp(wData, "precedingCharacters")
                                       + CabbageWidgetData::getCabbageCodeFromIdentifiers(wData, (currentLineText ==
                                                                                                  "</Cabbage>" ? ""
                                                                                                               : currentLineText));

                if (isGUIEnabled == true && guiPropUpdate == false)
                    getCurrentCodeEditor()->updateBoundsText(lineNumber, newText, true);
                else
                    getCurrentCodeEditor()->insertCode(lineNumber, newText, replaceExistingLine, parent.isEmpty());

            }

        }
    }
    else
    {
       //custom plant has been inserted...
    }
}
//==============================================================================
void CabbageMainComponent::timerCallback()
{

    if (fileTabs.size() > 0)
    {
        int32 nodeId = fileTabs[currentFileIndex]->uniqueFileId;

        if (audioGraph->graph.getNodeForId (nodeId) != nullptr && audioGraph->graph.getNodeForId (nodeId)->getProcessor()->isSuspended() == true)
        {
            stopCsoundForNode ("");
            stopTimer();
        }

        if (getCurrentCsdFile().existsAsFile())
        {

            const String csoundOutputString = audioGraph->getCsoundOutput (nodeId);

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
void CabbageMainComponent::addFileTab (File file)
{

    FileTab* fileButton;
    fileTabs.add (fileButton = new FileTab (file.getFileName(), file.getFullPathName(), file.hasFileExtension(".csd")));

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


    arrangeFileTabs();
}

void CabbageMainComponent::arrangeFileTabs()
{
    int xPos = 10;
    const int numTabs = 5;
    int tabIndex = 0;
    const int width = propertyPanel->isVisible() ? getWidth() - propertyPanel->getWidth() - 30 : getWidth() - 25;
    const int size = fileTabs.size();
    int startTabIndex = jmax (0, fileTabs.size() - 4);

    for ( auto fileButton : fileTabs)
    {
        if (tabIndex >= startTabIndex && tabIndex < startTabIndex + 4)
        {
            fileButton->setBounds (xPos, toolbarThickness + 3, width / numTabs, 30);
            xPos += width / numTabs + 4;
            fileButton->setVisible (true);
        }
        else
            fileButton->setVisible (false);

        tabIndex++;
    }

    if (fileTabs.size() > 4)
    {
        if (cycleTabsButton.isVisible() == false)
            addAndMakeVisible (cycleTabsButton);

        cycleTabsButton.setBounds (xPos, toolbarThickness + 3, 50, 30);
    }
    else
        cycleTabsButton.setVisible (false);


}

//==============================================================================
void CabbageMainComponent::addInstrumentsAndRegionsToCombobox()
{
    factory.combo->clearItemsFromComboBox();
    getCurrentCodeEditor()->parseTextForInstrumentsAndRegions();
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

    arrangeFileTabs();
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

    int32 nodeId = fileTabs[currentFileIndex]->uniqueFileId;;

    if (AudioProcessorGraph::Node::Ptr f = audioGraph->graph.getNodeForId (nodeId))
    {
        PluginWindow::WindowFormatType type = f->getProcessor()->hasEditor() ? PluginWindow::Normal
                                              : PluginWindow::Generic;

        if (PluginWindow* const w = PluginWindow::getWindowFor (f, type, audioGraph->graph))
        {
            if (GenericCabbagePluginProcessor* cabbagePlugin = dynamic_cast<GenericCabbagePluginProcessor*> (f->getProcessor()))
                w->setVisible (false);
            else
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
	CabbageUtilities::debug(fileTabs[currentFileIndex]->getFilename());
    if (audioGraph != nullptr && fileTabs[currentFileIndex])
    {
        const int32 nodeId = fileTabs[currentFileIndex]->uniqueFileId;

        if (nodeId != 99)
            if (AudioProcessorGraph::Node::Ptr f = audioGraph->graph.getNodeForId (nodeId))
            {
                AudioProcessor* const processor = f->getProcessor();
				if(processor != nullptr)
	                if (CabbagePluginEditor* editor = dynamic_cast<CabbagePluginEditor*> (processor->getActiveEditor()))
		                return editor;
            }
    }

    return nullptr;
}
//==================================================================================
CabbagePluginProcessor* CabbageMainComponent::getCabbagePluginProcessor()
{
    const int32 nodeId = fileTabs[currentFileIndex]->uniqueFileId;

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
    const int nodeId = fileTabs[currentFileIndex]->uniqueFileId;

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
        isGUIEnabled = enable;
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
    o.launchAsync();
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

//==============================================================================
void CabbageMainComponent::createNewTextFile(String contents)
{
    FileChooser fc ("Select file name and location", File::getSpecialLocation (File::SpecialLocationType::userHomeDirectory), "", CabbageUtilities::shouldUseNativeBrowser());

    if (fc.browseForFileToSave (false))
    {
        if (fc.getResult().existsAsFile())
        {
            CabbageIDELookAndFeel lookAndFeel;
            const int result = CabbageUtilities::showYesNoMessage ("Do you wish to overwrite\nexiting file?", &lookAndFeel);

            if (result == 1)
            {
                fc.getResult().replaceWithText (contents);
                createCodeEditorForFile (fc.getResult());
            }
        }
        else
        {
            fc.getResult().replaceWithText (contents);
            createCodeEditorForFile (fc.getResult());
        }
    }
}
//==============================================================================
bool CabbageMainComponent::isInterestedInFileDrag (const StringArray&   files)
{
    return true;
}

void CabbageMainComponent::filesDropped (const StringArray& files, int x, int y)
{
    for (auto file : files)
        openFile (file);
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
        FileChooser fc ("Open File", cabbageSettings->getMostRecentFile (0).getParentDirectory(), "*.cabbage", CabbageUtilities::shouldUseNativeBrowser());

        if (fc.browseForFileToOpen())
        {
            fileToOpen = fc.getResult();
        }
    }


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

    while(fileTabs.size()>0)
    {
        closeDocument();
    }

    StringArray pluginFiles;
    for ( int i = 0 ; i < files.size() ; i++)
    {
        if (files[i].existsAsFile())
        {
            openFile (files[i].getFullPathName());
            const int current = currentFileIndex;
            fileTabs[currentFileIndex]->uniqueFileId = uuids[i];
        }
    }



    audioGraph->loadDocument (fileToOpen);
}
//==================================================================================
File CabbageMainComponent::getCurrentCsdFile ()
{
    if(fileTabs[currentFileIndex])
        return fileTabs[currentFileIndex]->getFile();
    
    return File();
}

void CabbageMainComponent::setCurrentCsdFile (File file)
{
    fileTabs[currentFileIndex]->setFile (file);
}
//==================================================================================
void CabbageMainComponent::saveGraph (bool saveAs)
{
    audioGraph->saveGraph (saveAs);
}
//==================================================================================
const File CabbageMainComponent::openFile (String filename, bool updateRecentFiles)
{
    stopTimer();
    stopCsoundForNode (filename);
    File currentCsdFile;

    if (File (filename).existsAsFile() == false)
    {
        FileChooser fc ("Open File", cabbageSettings->getMostRecentFile (0).getParentDirectory(), "*.csd;*", CabbageUtilities::shouldUseNativeBrowser());

        if (fc.browseForFileToOpen())
        {
            if (getTabFileIndex (File (filename)) >= 0 && currentFileIndex > -1)
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

    if (updateRecentFiles)
    {
        cabbageSettings->updateRecentFilesList (currentCsdFile);
        cabbageSettings->setProperty ("MostRecentFile", currentCsdFile.getFullPathName());
    }

    createCodeEditorForFile (currentCsdFile);

    return currentCsdFile;

}
//==================================================================================
void CabbageMainComponent::launchHelpfile (String type)
{
    String url = "";

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
                url = cabbageHelpDir + "/docs/sliders/index.html";
            else if (keyword == "filebutton")
                url = cabbageHelpDir + "/docs/filebutton/index.html";
            else if (keyword == "infobutton")
                url = cabbageHelpDir + "/docs/infobutton/index.html";
            else
                url = cabbageHelpDir + "/docs/" + keyword + "/index.html";
        }
        else
            url = csoundHelpDir + "/" + keyword +".html";
    }
    else if (type == "csound")
        url = csoundHelpDir + "/index.html";

    else
        url = cabbageHelpDir + "/index.html";

    if (File (url).existsAsFile())
    {
        URL urlHelp (url);
        ChildProcess process;
        File temp (urlHelp.toString (false));
#ifdef LINUX

        if (!process.start (String ("xdg-open " + urlHelp.toString (false)).toUTF8()))
            CabbageUtilities::showMessage ("Couldn't show file, see 'Set Csound manual directory' in Options->Preferences", &getLookAndFeel());

#else
        urlHelp.launchInDefaultBrowser();
#endif

    }
}

//==================================================================================
void CabbageMainComponent::createCodeEditorForFile (File file)
{
    CabbageEditorContainer* editorConsole;
    editorAndConsole.add (editorConsole = new CabbageEditorContainer (cabbageSettings, file.hasFileExtension (".csd")));
    addAndMakeVisible (editorConsole);
    addAndMakeVisible (propertyPanel = new CabbagePropertiesPanel (cabbageSettings->valueTree));
    propertyPanel->setVisible (false);
    editorConsole->setVisible (true);
    editorConsole->toFront (true);

    if (file.hasFileExtension (".csd") == false)
        editorConsole->hideOutputConsole();

    editorConsole->editor->loadContent (file.loadFileAsString());
    editorConsole->editor->parseTextForInstrumentsAndRegions();
    editorConsole->editor->startThread();
    numberOfFiles = editorAndConsole.size();
    currentFileIndex = editorAndConsole.size() - 1;
    addFileTab (file);
    getCurrentCodeEditor()->addChangeListener (this);
    getCurrentCodeEditor()->setSavePoint();
    owner->setName ("Cabbage " + file.getFullPathName());
    addInstrumentsAndRegionsToCombobox();
    repaint();
    resized();

    cabbageSettings->setProperty ("NumberOfOpenFiles", int (editorAndConsole.size()));
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

        addInstrumentsAndRegionsToCombobox();
    }
    else
    {
        //check if file is part of installed example, prevent overwriting...
        String examplesDir = File::getSpecialLocation (File::currentExecutableFile).getParentDirectory().getFullPathName() + "/Examples";

#if defined(LINUX)
        //manualPath = File::getSpecialLocation (File::currentExecutableFile).getParentDirectory().getFullPathName() + "/CsoundDocs";
        examplesDir = "/usr/share/doc/cabbage/Examples";
#elif defined(MACOSX)
    examplesDir = File::getSpecialLocation (File::currentExecutableFile).getParentDirectory().getParentDirectory().getFullPathName() + "/Examples";
#endif



        stopCsoundForNode (getCurrentCsdFile().getFullPathName());;
        isGUIEnabled = false;

        if (getCabbagePluginEditor() != nullptr)
            getCabbagePluginEditor()->enableEditMode (false);

        if (getCurrentCodeEditor()->hasFileChanged())
        {
            if(getCurrentCsdFile().getFullPathName().contains(examplesDir)) {
                CabbageUtilities::showMessage("You cannot save over an example file. Please use save-as instead", lookAndFeel);
                return;
            }

            if (getCurrentCsdFile().existsAsFile())
                getCurrentCsdFile().replaceWithText (getCurrentCodeEditor()->getDocument().getAllContent());
        }

        propertyPanel->setEnabled (false);

        if (recompile == true && getCurrentCsdFile().hasFileExtension ((".csd")))
        {
            runCsoundForNode (getCurrentCsdFile().getFullPathName());
            //fileTabs[currentFileIndex]->getPlayButton().setToggleState (true, dontSendNotification);
        }

        addInstrumentsAndRegionsToCombobox();
    }

    StringArray lines;
    lines.addLines(getCurrentCsdFile().loadFileAsString());
    for( int i = 0 ; i < lines.size() ; i++)
    {
        if(lines[i] == "<CsOptions>")
        {
            const String csOptions = lines[i+1];
            if(csOptions.contains("-+rtaudio"))
                CabbageUtilities::showMessage("You are using -+rtaudio to set an audio device. This is unsupported. Please set the audio device in the Cabbage audio settings and remove the -+rtaudio flag from your CsOptions.", lookAndFeel);
            if(csOptions.contains("-+rtmidi=") && (!csOptions.contains("-+rtmidi=NULL") && !csOptions.contains("-+rtmidi=null")))
                CabbageUtilities::showMessage("You are using -+rtmidi to set a MIDI device. This is unsupported. Please use -+rtmidi=NULL in your CsOptions, and select a MIDI device in the Cabbage audio settings.", lookAndFeel);

        }
    }

}
//==================================================================================
void CabbageMainComponent::writeFileToDisk (File file)
{
    if(file.hasFileExtension(".csd"))
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

    cabbageSettings->setProperty ("NumberOfOpenFiles", int (editorAndConsole.size()));

}
//==================================================================================
void CabbageMainComponent::removeEditor()
{
    editorAndConsole.removeObject (getCurrentEditorContainer());
    fileTabs.remove (currentFileIndex);
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
            owner->setName ("Cabbage " + fileTabs[currentFileIndex]->getFile().getFullPathName());
            arrangeFileTabs();
        }
        else
        {
            owner->setName ("Cabbage Csound IDE");
            propertyPanel->setVisible (false);
        }

        cabbageSettings->setProperty ("MostRecentFile", fileTabs[currentFileIndex]->getFile().getFullPathName());
    }


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
int CabbageMainComponent::testFileForErrors (String file)
{
    ChildProcess process;
    const String applicationDir = File::getSpecialLocation (File::currentExecutableFile).getParentDirectory().getFullPathName();
    const String processName = applicationDir + "/CsoundTest " + file;

    if (File (processName).existsAsFile())
    {
        process.start (processName);

        process.readAllProcessOutput();

        const int exitCode = process.getExitCode();

        if (exitCode == 1)
        {
            process.start ("csound " + file);
            this->getCurrentOutputConsole()->setText (process.readAllProcessOutput());
            stopCsoundForNode (file);
            return 1;
        }
    }

    return 0;

}
void CabbageMainComponent::runCsoundForNode (String file)
{

    if (testFileForErrors (file) == 0) //if Csound seg faults it will take Cabbage down. best to test the instrument in a separate process first.
    {
        if (File (file).existsAsFile())
        {
            //PluginWindow::closeAllCurrentlyOpenWindows();
            //audioGraph = nullptr;
            //createAudioGraph(); //in future versions we can simply edit the node in question and reconnect within the graph
            int32 node = fileTabs[currentFileIndex]->uniqueFileId;

            if (node == -99)
            {
                Uuid uniqueID;
                node = int32 (*uniqueID.getRawData());
                fileTabs[currentFileIndex]->uniqueFileId = node;
            }

            Point<int> pos (PluginWindow::getPositionOfCurrentlyOpenWindow (node));
            PluginWindow::closeCurrentlyOpenWindowsFor (node);

            if (pos.getX() == -1000 && pos.getY() == -1000)
            {
                Random rand;//nextInt();
                pos.setX (rand.nextInt (Range<int> (getWidth() / 2, getWidth() / 1.8)));
                pos.setY (rand.nextInt (Range<int> (getHeight() / 2, (getHeight() / 2) + 100)));
            }

            getCurrentCsdFile().getParentDirectory().setAsCurrentWorkingDirectory();
            const bool pluginAdded = audioGraph->addPlugin (getCurrentCsdFile(), node);
            createEditorForAudioGraphNode (pos);
            startTimer (100);
            if(pluginAdded==false)
            {
                fileTabs[currentFileIndex]->getPlayButton().getProperties().set("state", "off");
                fileTabs[currentFileIndex]->getPlayButton().setToggleState(false, dontSendNotification);
            }
            else
            {
                fileTabs[currentFileIndex]->getPlayButton().getProperties().set("state", "on");
                fileTabs[currentFileIndex]->getPlayButton().setToggleState(true, dontSendNotification);
            }
            factory.togglePlay (true);
            graphComponent->updateComponents();
        }
        else
            CabbageUtilities::showMessage ("Warning", "Please open a file first", lookAndFeel);
    }
}

void CabbageMainComponent::stopCsoundForNode (String file)
{
    if (fileTabs[currentFileIndex] && File (file).existsAsFile())
    {
        const int32 nodeId = fileTabs[currentFileIndex]->uniqueFileId;

        if (audioGraph->getNodeForId (nodeId) != nullptr)
            audioGraph->getNodeForId (nodeId)->getProcessor()->suspendProcessing (true);

        fileTabs[currentFileIndex]->getPlayButton().getProperties().set("state", "off");
        fileTabs[currentFileIndex]->getPlayButton().setToggleState(false, dontSendNotification);
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

    arrangeFileTabs();
}
