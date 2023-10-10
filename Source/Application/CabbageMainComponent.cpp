/*
  Copyright (C) 2016 Rory Walsh

  Cabbage is free software; you can redistribute it
  and/or modify it under the terms of the GNU General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  Cabbage is distributed in the hope that it will besave useful,
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
#include "../Audio/UI/PluginWindow.h"
#include "../Audio/Filters/InternalFilters.h"
#include "../Utilities/CabbageStrings.h"

class CabbageMainComponent::PluginListWindow  : public DocumentWindow
{
public:
    PluginListWindow (CabbageMainComponent& mw, AudioPluginFormatManager& pluginFormatManager)
    : DocumentWindow ("Available Plugins",
                      LookAndFeel::getDefaultLookAndFeel().findColour (ResizableWindow::backgroundColourId),
                      DocumentWindow::minimiseButton | DocumentWindow::closeButton),
    owner (mw)
    {
        setLookAndFeel(&lookAndFeel);

		auto deadMansPedalFile = owner.getCabbageSettings()->getUserSettings()->getFile().getSiblingFile ("RecentlyCrashedPluginsList");
        
        setContentOwned (new CabbagePluginListComponent (pluginFormatManager,
                                                  owner.knownPluginList,
                                                  deadMansPedalFile,
                                                  owner.getCabbageSettings()->getUserSettings(), true), true);

        setResizable (true, false);
        setResizeLimits (300, 400, 1200, 500);
        setTopLeftPosition (60, 60);
        
        restoreWindowStateFromString (owner.getCabbageSettings()->getUserSettings()->getValue ("listWindowPos"));
        setVisible (true);
    }
    
    ~PluginListWindow()
    {
        setLookAndFeel(nullptr);
        owner.getCabbageSettings()->getUserSettings()->setValue ("listWindowPos", getWindowStateAsString());
        clearContentComponent();
    }
    
    void closeButtonPressed() override
    {
        owner.pluginListWindow = nullptr;
    }
    
private:
    CabbageMainComponent& owner;
    LookAndFeel_V4 lookAndFeel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginListWindow)
};

//==============================================================================
CabbageMainComponent::CabbageMainComponent (CabbageDocumentWindow* owner, CabbageSettings* settings)
    : goUpButton(),
      lookAndFeel(new CabbageIDELookAndFeel()),
      cycleTabsButton ("..."),
      resizerBar(settings->getValueTree(), this),
wildcardFilter(new WildcardFileFilter("*.csd;*.sco;*.orc;*.txt;*.js;*.html;*.snaps;*.plant;*.xml", "*.*", "")),
      fileTree(FileBrowserComponent::FileChooserFlags::openMode | FileBrowserComponent::FileChooserFlags::canSelectFiles, File::getSpecialLocation (File::currentExecutableFile), wildcardFilter.get(), nullptr),
      owner(owner),
      lookAndFeel4(),
      factory (this),
      cabbageSettings (settings)
{
  
    cycleTabsButton.setColour (TextButton::ColourIds::buttonColourId, Colour (100, 100, 100));
    getLookAndFeel().setColour (TooltipWindow::ColourIds::backgroundColourId, Colours::whitesmoke);
    propertyPanel.reset (new CabbagePropertiesPanel (ValueTree("empty")));
    addAndMakeVisible (propertyPanel.get());
    propertyPanel->setVisible (false);
    setSize (1200, 800);

    bgImage = createBackground();
	formatManager.addDefaultFormats();
	
	formatManager.addFormat(new InternalPluginFormat());

    filterGraphWindow.reset (new FilterGraphDocumentWindow("FilterGraph", Colour(200, 200, 200), this));
    filterGraphWindow->setVisible (false);

    InternalPluginFormat internalFormat;
    internalFormat.getAllTypes (internalTypes);
    
    std::unique_ptr<XmlElement> savedPluginList (cabbageSettings->getUserSettings()->getXmlValue ("pluginList"));
    
    if (savedPluginList != nullptr)
        knownPluginList.recreateFromXml (*savedPluginList);
    
    for (auto& t : internalTypes)
        knownPluginList.addType (t);
    
    pluginSortMethod = (KnownPluginList::SortMethod) cabbageSettings->getUserSettings()->getIntValue ("pluginSortMethod", KnownPluginList::sortByManufacturer);
    
    knownPluginList.addChangeListener (this);
    
    addAndMakeVisible (toolbar);
    
    
	factory.setIconsPath(cabbageSettings->getUserSettings()->getValue("CustomThemeDir"));
    toolbar.addDefaultItems (factory);
    propertyPanel->addChangeListener (this);
    factory.combo->getCombo().addListener (this);
    cycleTabsButton.setSize (50, 28);
    cycleTabsButton.addListener (this);
    
	fileTree.setLookAndFeel(&lookAndFeel4);
    //fileTree.lookAndFeelChanged();

    createFilterGraph(); //set up graph even though no file is selected. Allows users to change audio devices from the get-go..

	reloadAudioDeviceState();
    String lastOpenDir = cabbageSettings->getUserSettings()->getValue ("lastOpenedDir", "");
	if (File(lastOpenDir).exists() == false)
	{
		fileTree.setRoot(File::getSpecialLocation(File::SpecialLocationType::userDocumentsDirectory));
	}
	else
		fileTree.setRoot(File(lastOpenDir));

    fileTree.addListener(this);
 	fileTree.setColour(TreeView::backgroundColourId, Colours::grey);
	addAndMakeVisible(fileTree);
    
    if(cabbageSettings->getUserSettings()->getIntValue("ShowFileBrowser")==0)
    {
        resizerBar.setVisible(false);
        resizerBarCurrentXPos = 0;
        resized();
    }
    
    addAndMakeVisible(resizerBar);
    resizerBar.addMouseListener(this, true);
	setLookAndFeelColours();
    addAndMakeVisible(goUpButton);

	socket = std::make_unique<DatagramSocket>(true);
}

CabbageMainComponent::~CabbageMainComponent()
{
	socket->shutdown();
    pluginListWindow = nullptr;
    fileTree.setLookAndFeel(nullptr);
    knownPluginList.removeChangeListener (this);
    
    editorAndConsole.clear();
    setLookAndFeel(nullptr);

    if (tempFile.existsAsFile())
        tempFile.deleteFile();

    stopFilterGraph();
	graphComponent = nullptr;
}

void CabbageMainComponent::paint (Graphics& g)
{
    if (editorAndConsole.size() == 0)
        g.drawImage (bgImage, getLocalBounds().toFloat());
	else
	{
		g.fillAll(CabbageSettings::getColourFromValueTree(cabbageSettings->valueTree, CabbageColourIds::fileTabBar, Colour(50, 50, 50)));
	}
}

void CabbageMainComponent::fileClicked (const File &file, const MouseEvent &e)
{
    bringCodeEditorToFront(file);
}

void CabbageMainComponent::fileDoubleClicked (const File &file)
{
    openFile(file.getFullPathName());
}

void CabbageMainComponent::browserRootChanged(const File &newRoot)
{
	cabbageSettings->setProperty("lastOpenedDir", newRoot.getFullPathName());
}
//===============================================================================================================
void CabbageMainComponent::exportTheme()
{
	FileChooser fc("Export theme XML", cabbageSettings->getMostRecentFile(0).getParentDirectory(), "*.xml", CabbageUtilities::shouldUseNativeBrowser());


	if (fc.browseForFileToSave(true))
	{
		//doing this manually as the settings file fails some XML tests...
		File themeFile(fc.getResult());
		StringArray theme;
		String xmlHeader = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<PROPERTIES>";
		theme.addLines(xmlHeader);
		const StringArray colourIDStrings = CabbageStrings::getColourIDStrings();

		for (int i = 0; i < colourIDStrings.size(); i++)
		{
			const auto colourValue = cabbageSettings->getValueTree().getChildWithName("Colours").getProperty(colourIDStrings[i]);
			theme.add("<VALUE name=\"Colours_" + colourIDStrings[i] + "\" val=\"" + colourValue.toString() + "\"/>");
		}

		theme.add("</PROPERTIES>");
		themeFile.replaceWithText(theme.joinIntoString("\n"));
	}
}

void CabbageMainComponent::importTheme()
{
	FileChooser fc("Open theme XML", cabbageSettings->getMostRecentFile(0).getParentDirectory(), "*.xml", CabbageUtilities::shouldUseNativeBrowser());

	if (fc.browseForFileToOpen())
	{

		XmlDocument myDocument(fc.getResult());
		std::unique_ptr<XmlElement> myElement(myDocument.getDocumentElement());

		forEachXmlChildElement(*myElement, e)
		{
			cabbageSettings->setProperty(e->getStringAttribute("name"), e->getStringAttribute("val"));
		}

		cabbageSettings->setProperty("CustomThemeDir", fc.getResult().getParentDirectory().getFullPathName());
	}
}



//=====================================================================================================================
void CabbageMainComponent::setLookAndFeelColours()
{
    lookAndFeel->setColour (Slider::ColourIds::thumbColourId, Colour (110, 247, 0));
    lookAndFeel->setColour (ScrollBar::backgroundColourId, Colour (70, 70, 70));
    toolbar.setColour (Toolbar::backgroundColourId, CabbageSettings::getColourFromValueTree (cabbageSettings->getValueTree(), CabbageColourIds::menuBarBackground, Colour (50, 50, 50)));
    toolbar.setColour (Toolbar::ColourIds::buttonMouseOverBackgroundColourId, CabbageSettings::getColourFromValueTree (cabbageSettings->getValueTree(), CabbageColourIds::menuBarBackground, Colour (50, 50, 50)).contrasting (.3f));
    toolbar.setColour (Toolbar::ColourIds::buttonMouseDownBackgroundColourId, CabbageSettings::getColourFromValueTree (cabbageSettings->getValueTree(), CabbageColourIds::menuBarBackground, Colour (50, 50, 50)).contrasting (.5f));
    owner->lookAndFeelChanged();
    lookAndFeel->refreshLookAndFeel (cabbageSettings->getValueTree());
    lookAndFeelChanged();
    toolbar.repaint();
	fileTree.getLookAndFeel().setColour(ListBox::backgroundColourId, CabbageSettings::getColourFromValueTree(cabbageSettings->getValueTree(), CabbageColourIds::codeBackground, Colour(50, 50, 50)).darker());
    fileTree.getLookAndFeel().setColour(DirectoryContentsDisplayComponent::textColourId, Colours::whitesmoke);
    fileTree.getLookAndFeel().setColour(DirectoryContentsDisplayComponent::ColourIds::highlightColourId, Colours::whitesmoke.darker());
	fileTree.getLookAndFeel().setColour(ScrollBar::thumbColourId, CabbageSettings::getColourFromValueTree(cabbageSettings->getValueTree(), CabbageColourIds::codeBackground, Colour(50, 50, 50)).brighter());
    goUpButton.upArrowColour = CabbageSettings::getColourFromValueTree(cabbageSettings->getValueTree(), CabbageColourIds::codeBackground, Colour(50, 50, 50)).brighter(.6f);
    goUpButton.repaint();
	fileTree.lookAndFeelChanged();
	fileTree.repaint();
	resized();

}
//==============================================================================
void CabbageMainComponent::buttonClicked (Button* button)
{
	hideFindPanel();
    
    if (FileTab* tabButton = dynamic_cast<FileTab*> (button))
    {
        handleFileTab (tabButton);
    }
    else
    {

            if (ToolbarButton* toolbarButton = dynamic_cast<ToolbarButton*> (button))
                handleToolbarButtons (toolbarButton);
            else if (DrawableButton* drawableButton = dynamic_cast<DrawableButton*> (button))
                handleFileTabs (drawableButton);
            else if (TextButton* textButton = dynamic_cast<TextButton*> (button))
            {
                PopupMenu menu;

                for ( int i = 0 ; i < fileTabs.size() ; i++)
                    if (fileTabs[i]->isVisible() == false)
                        menu.addItem (i + 1, fileTabs[i]->getName());

                menu.showMenuAsync(juce::PopupMenu::Options(), [this](int result) {

                    if ( result > 0 )
                    {
                        this->fileTabs.move (result - 1, this->fileTabs.size() - 1);
                        this->editorAndConsole.move (result - 1, this->fileTabs.size() - 1);
                        this->arrangeFileTabs();
                        this->fileTabs[this->fileTabs.size() - 1]->setToggleState (true, sendNotification);
                        this->setCurrentCsdFile (this->fileTabs[this->fileTabs.size() - 1]->getFile());
                    }
                });
            }
    }
    
    if(fileTree.isVisible())
    {
        StringArray files;
        for ( int i = 0 ; i < fileTabs.size() ; i++)
            if (fileTabs[i]->isVisible() == false)
            {
                files.add(fileTabs[i]->getFile().getFullPathName());
            }
        
        const int result = files.indexOf(getCurrentCsdFile().getFullPathName()) + 1;
        
        if ( result > 0 )
        {
            fileTabs.move (result - 1, fileTabs.size() - 1);
            editorAndConsole.move (result - 1, fileTabs.size() - 1);
            arrangeFileTabs();
            fileTabs[fileTabs.size() - 1]->setToggleState (true, sendNotification);
            setCurrentCsdFile (fileTabs[fileTabs.size() - 1]->getFile());
        }
        if(fileTabs.size()>0)
        {
            fileTree.setRoot(getCurrentCsdFile().getParentDirectory());
            Timer::callAfterDelay(100, [this](){
                fileTree.setFileName(getCurrentCsdFile().getFullPathName());
            });
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
			{
				button->disableButtons(true);
				button->setToggleState(false, dontSendNotification);
			}
			else
			{
				button->disableButtons(false);
			}
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
			{
				fileTabs[i]->setToggleState(false, dontSendNotification);
				fileTabs[i]->disableButtons(true);
			}
        }

        this->arrangeFileTabs();
    }


    fileTree.refresh();

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
//    else
//        this->openFile (file.getFullPathName());

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
    else if (toolbarButton->getName() == "save")
    {
        if(fileTabs.size() > 0)
            saveDocument (false);
    }
    else if (toolbarButton->getName() == "save as")
    {
        if(fileTabs.size() > 0)
            saveDocument (true);
    }
    else if (toolbarButton->getName() == "settings")
    {
        showSettingsDialog();
    }
    else if (toolbarButton->getName() == "cut")
    {
        if(fileTabs.size() > 0)
            getCurrentCodeEditor()->cut();
    }
    else if (toolbarButton->getName() == "copy")
    {
        if(fileTabs.size() > 0)
            getCurrentCodeEditor()->copy();
    }
    else if (toolbarButton->getName() == "paste")
    {
        if(fileTabs.size() > 0)
            getCurrentCodeEditor()->paste();
    }
    else if (toolbarButton->getName() == "togglePlay")
    {
        if (toolbarButton->getToggleState())
            this->startFilterGraph();
        else
            this->stopFilterGraph();
    }
    else if (toolbarButton->getName() == "toggleRecord")
    {
        bitDepth = cabbageSettings->getUserSettings()->getIntValue ("RecordingBitDepth");
        
        if (toolbarButton->getToggleState())
        {
            shouldRecord = true;
            factory.startRecordingTimer(true);
        }
        else
        {
            shouldRecord = false;
        }
    }
}

void CabbageMainComponent::handleFileTabs (DrawableButton* drawableButton)
{
    if (drawableButton->getName() == "playButton")
    {
        if (drawableButton->getProperties().getWithDefault ("state", "") == "off")
            saveDocument(false, true);
        else
        {
            stopCsoundForNode (drawableButton->getProperties().getWithDefault ("filename", ""));
            factory.startRecordingTimer(false);
            factory.setRecordButtonState("enabled");
            shouldRecord = false;
        }

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
        if (CabbageUtilities::hasCabbageTags(fileTabs[currentFileIndex]->getFilename()))
        {
            if (FileTab* tabButton = drawableButton->findParentComponentOfClass<FileTab>())
            {
                const String filename = tabButton->getFilename();
                AudioProcessorGraph::NodeID nodeId(fileTabs[currentFileIndex]->uniqueFileId);

                if (AudioProcessorGraph::Node::Ptr f = getFilterGraph()->graph.getNodeForId (nodeId))
                {
                    if (auto f = getFilterGraph()->graph.getNodeForId (nodeId))
                        if (auto* w = getFilterGraph()->getOrCreateWindowFor (f, PluginWindow::Type::normal))
                        {
                            CabbagePluginProcessor* cabbagePlugin = getCabbagePluginProcessor();
                            String pluginName = cabbagePlugin->getPluginName();
                            w->setName (pluginName.length() > 0 ? pluginName : "Plugin has no name?");
                            w->toFront (true);
                            w->setVisible (true);
                        }
                }
            }
        }
        else
            CabbageUtilities::showMessage("No Cabbage code block found. GUI editing only works in Cabbage .csd files", lookAndFeel.get());
    }
    else if (drawableButton->getName() == "editGUIButton")
    {
        if (CabbageUtilities::hasCabbageTags(fileTabs[currentFileIndex]->getFilename()))
        {
            if (isGUIEnabled == false)
            {
                //this->saveDocument();
                enableEditMode();
                if (FileTab* tabButton = drawableButton->findParentComponentOfClass<FileTab>())
                {
                    tabButton->getPlayButton().getProperties().set("state", "off");
                    resized();
                }
            }
            else
            {
                this->saveDocument();
                propertyPanel->setVisible(false);
                resized();
            }
        }
        else
            CabbageUtilities::showMessage("No Cabbage code block found. GUI editing only works in Cabbage .csd files", lookAndFeel.get());
    }
}

void CabbageMainComponent::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    if(editorAndConsole.size() > 0)
    {
        int index = comboBoxThatHasChanged->getSelectedId();
        const int lineToScrollTo = getCurrentCodeEditor()->instrumentsAndRegions.getValueAt (index - 1);
        getCurrentCodeEditor()->scrollToLine (lineToScrollTo);
    }
}

//==============================================================================
void CabbageMainComponent::mouseExit (const MouseEvent& e)
{
    if (e.eventComponent->getName() == "ResizerBar")
        resizerBar.setMouseCursor (MouseCursor::NormalCursor);
}

void CabbageMainComponent::mouseDown (const MouseEvent& e)
{
    if (e.eventComponent->getName() == "ResizerBar")
        startingVBarDragPos = resizerBar.getPosition().getX();
}

void CabbageMainComponent::mouseUp (const MouseEvent& e)
{

}

void CabbageMainComponent:: mouseEnter (const MouseEvent& e)
{
    
    
    if (e.eventComponent->getName() == "ResizerBar")
        resizerBar.setMouseCursor (MouseCursor::LeftRightResizeCursor);
}

void CabbageMainComponent::mouseDrag (const MouseEvent& e)
{
    if (e.eventComponent->getName() == "ResizerBar")
    {
//        CabbageUtilities::debug(startingVBarDragPos + e.getDistanceFromDragStartX());
        resizerBar.setBounds (startingVBarDragPos + e.getDistanceFromDragStartX(), 0, resizerBar.getWidth(), resizerBar.getHeight());
        resizerBarCurrentXPos = startingVBarDragPos + e.getDistanceFromDragStartX();
        resized();
    }
}
//==============================================================================
void CabbageMainComponent::changeListenerCallback (ChangeBroadcaster* source)
{
	if (dynamic_cast<PluginWindow*> (source)) // update lookandfeel whenever a user changes colour settings
	{
		propertyPanel->setVisible(false);
        if(getFileTab(getCurrentFileIndex()))
        getFileTab(getCurrentFileIndex())->getEditGUIButton().setToggleState(false, dontSendNotification);
        isGUIEnabled = false;
		resized();
	}

	else if (dynamic_cast<AudioDeviceManager*> (source))
	{

//        if (deviceManager.getCurrentAudioDeviceType() == "Windows Audio")
//        {
//            const int dismiss = cabbageSettings->getUserSettings()->getIntValue("AudioDriversWarning_dismiss");
//            if (dismiss == 0)
//            {
//                bool value = CabbageUtilities::showMessageWithHideOption("Audio drivers warning", "In order for edit mode to work with \"Windows Audio\", you need to ensure that the sample rate for \"Windows Audio\" is set to 44100 in your audio system settings. For best audio performance always use ASIO drivers. If you don't have any, consider installing ASIO4ALL which is available for free.", lookAndFeel, dismiss);
//                cabbageSettings->getUserSettings()->setValue("AudioDriversWarning_dismiss", value);
//            }
//        }
		if (deviceManager.getAudioDeviceSetup().outputDeviceName == "" && shouldUpdateAudioSettings == false)
		{
			CabbageUtilities::showMessage("Warning", "No output device selected. Please open Cabbage audio settings and select a valid output device.", lookAndFeel.get());
			deviceManager.closeAudioDevice();
			shouldUpdateAudioSettings = true;
		}
	}

    else if (source == &knownPluginList)
    {
        // save the plugin list every time it gets changed, so that if we're scanning
        // and it crashes, we've still saved the previous ones
        std::unique_ptr<XmlElement> savedPluginList (knownPluginList.createXml());
        
        if (savedPluginList != nullptr)
        {
            cabbageSettings->getUserSettings()->setValue ("pluginList", savedPluginList.get());
            cabbageSettings->saveIfNeeded();
        }
    }
    
    else if (dynamic_cast<CabbageSettings*> (source)) // update lookandfeel whenever a user changes colour settings
    {
        lookAndFeel->refreshLookAndFeel (cabbageSettings->getValueTree());
        lookAndFeelChanged();
        owner->setColour(owner->backgroundColourId, CabbageSettings::getColourFromValueTree(cabbageSettings->valueTree, CabbageColourIds::mainBackground, Colours::lightgrey));
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
            CabbageUtilities::debug(widgetData.getType().toString());
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

                if (CabbageWidgetData::getNumProp(widgetData, CabbageIdentifierIds::linenumber) > 9999) //if widget was added in edit mode...
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
                const juce::Rectangle<int> rect = CabbageWidgetData::getBounds(widgetData);
                const String newBounds = CabbageWidgetData::getBoundsTextAsCabbageCode(rect);
                String newL = CabbageWidgetData::replaceIdentifier(
                        getCurrentCodeEditor()->getLineText(lineNumberOfCustomPlant), "bounds", newBounds);

                getCurrentCodeEditor()->insertCode(lineNumberOfCustomPlant, newL, true, true);

            }
        }
    }

    else if (CabbagePropertiesPanel* props = dynamic_cast<CabbagePropertiesPanel*> (source)) // update code when a user changes a property
    {
        if (props->hide == true)
        {
            props->hide = false; // reset the hide status
            togglePropertyPanel();
			saveDocument();
            getFileTab(getCurrentFileIndex())->getEditGUIButton().setToggleState(false, dontSendNotification);
        }
        else if (CabbagePluginEditor* ed = getCabbagePluginEditor())
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
//    CabbageUtilities::debug(CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::name));
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

    Range<int> cabbageSection = CabbageUtilities::getCabbageSectionRange(getCurrentCodeEditor()->getAllText());
    String name;
    String namespce;
    std::unique_ptr<XmlElement> xml;
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
    saveDocument();
    enableEditMode();

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
        enableEditMode();
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
//                CabbageUtilities::debug(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::type));
//                CabbageUtilities::debug(CabbageWidgetData::getBounds(wData).toString());
//                CabbageUtilities::debug(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::plant));
//                CabbageUtilities::debug("linenUmber:", CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::linenumber));
                
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
        AudioProcessorGraph::NodeID nodeId(fileTabs[currentFileIndex]->uniqueFileId);
        
        if (getFilterGraph()->graph.getNodeForId (nodeId) != nullptr && getFilterGraph()->graph.getNodeForId (nodeId)->getProcessor()->isSuspended() == true)
        {
            stopCsoundForNode ("");
        }

        if (getCurrentCsdFile().existsAsFile())
        {

          const String csoundOutputString = getFilterGraph()->getCsoundOutput (nodeId);

          if (csoundOutputString.length() > 0)
                getCurrentOutputConsole()->setText (csoundOutputString);

        }
        
        if(cabbageSettings->getUserSettings()->getIntValue("AutoReloadFromDisk")){
            const auto originalFileIndex = getCurrentFileIndex();
            for ( int i = 0 ; i < fileTabs.size() ; i++)
            {
                if(fileTabs[i]->lastModified != fileTabs[i]->getFile().getLastModificationTime())
                {
                    currentFileIndex = i;
                    getCurrentEditorContainer()->editor->loadContent(fileTabs[i]->getFile().loadFileAsString());
                    saveDocument();
                }
            }
            currentFileIndex = originalFileIndex;
        }
        
        
        //saveDocument(false, true);
        //DBG("Number of compilations:"+String(compileCounter++));
      
    }
    
    
    
}
//==============================================================================
void CabbageMainComponent::updateEditorColourScheme()
{
    propertyPanel->setColours (CabbageSettings::getColourFromValueTree (cabbageSettings->getValueTree(), CabbageColourIds::propertyPanelBackground, Colour (50, 50, 50)),
    CabbageSettings::getColourFromValueTree (cabbageSettings->getValueTree(), CabbageColourIds::propertyLabelBackground, Colour (50, 50, 50)),
    CabbageSettings::getColourFromValueTree (cabbageSettings->getValueTree(), CabbageColourIds::propertyLabelText, Colour (50, 50, 50)));
    propertyPanel->setBorderColour (CabbageSettings::getColourFromValueTree (cabbageSettings->getValueTree(), CabbageColourIds::propertyPanelBackground, Colour (50, 50, 50)));

	for (auto* tab : fileTabs)
	{
		tab->setButtonColour(CabbageSettings::getColourFromValueTree(cabbageSettings->getValueTree(), CabbageColourIds::fileTabButton, Colour(50, 50, 50)));
		tab->setFontColour(CabbageSettings::getColourFromValueTree(cabbageSettings->getValueTree(), CabbageColourIds::fileTabText, Colour(50, 50, 50)));
		tab->setPlayButtonColour(CabbageSettings::getColourFromValueTree(cabbageSettings->getValueTree(), CabbageColourIds::fileTabPlayButton, Colour(50, 50, 50)));
		tab->repaint();
	}

    if (getCurrentEditorContainer() != nullptr)
        getCurrentEditorContainer()->updateLookAndFeel();

    toolbar.setColour (Toolbar::backgroundColourId, CabbageSettings::getColourFromValueTree (cabbageSettings->getValueTree(), CabbageColourIds::menuBarBackground, Colour (50, 50, 50)));
    toolbar.repaint();

	graphComponent->graphPanel->setBackgroundColour(CabbageSettings::getColourFromValueTree(cabbageSettings->getValueTree(), CabbageColourIds::patcher, Colour(50, 50, 50)));
}
//==============================================================================
Image CabbageMainComponent::createBackground()
{
    Image backgroundImg;
    backgroundImg = Image (Image::RGB, getWidth(), getHeight(), true);
    std::unique_ptr<Drawable> drawable;
    const Colour colour(CabbageSettings::getColourFromValueTree(cabbageSettings->valueTree, CabbageColourIds::fileTabBar, Colour(50, 50, 50)));
    Graphics g (backgroundImg);
    {
		g.fillAll(colour);
        Random pos, width, randBrightness;

        for (int i = 0; i < getWidth();)
        {
            const int brightness = randBrightness.nextFloat();
            g.setColour (colour.darker(brightness*.2));
            g.drawLine (i, 0, i + width.nextInt (Range<int> (0, 10)), getHeight() );
            g.drawLine (0, i, getWidth(), i + width.nextInt (Range<int> (0, 10)));
            i += pos.nextInt (Range<int> (0, 5));
        }

        const Image cabbageLogo = ImageCache::getFromMemory (CabbageBinaryData::CabbageLogoBig_png, CabbageBinaryData::CabbageLogoBig_pngSize);
		//g.drawImage(cabbageLogo, 400, 400, 400, 400, 0, 0, cabbageLogo.getWidth(), cabbageLogo.getHeight(), RectanglePlacement::Flags::doNotResize);
		//g.drawImage(cabbageLogo, getLocalBounds().reduced(.01f, .05f).toFloat(), RectanglePlacement::Flags::onlyReduceInSize);
        g.drawImage(cabbageLogo, {getWidth()/2.f - 175, 200, 350, 400});
		return backgroundImg;
    }
}

//==============================================================================
void CabbageMainComponent::addFileTab (File file)
{

    FileTab* fileButton;
    fileTabs.add (fileButton = new FileTab (file.getFileName(), file.getFullPathName(), file.hasFileExtension(".csd"), cabbageSettings->getUserSettings()->getValue("CustomThemeDir")));


	for ( auto* button : fileTabs)
		button->setToggleState(false, dontSendNotification);

    addAndMakeVisible (fileButton);
    fileButton->addListener (this);
	fileButton->setName(file.getFileName());
    fileButton->setClickingTogglesState (true);
    fileButton->setLookAndFeel (lookAndFeel.get());
    fileButton->setToggleState (true, dontSendNotification);
    currentFileIndex = fileTabs.size() - 1;
    fileButton->addButtonListeners (this);
    fileButton->setButtonColour(CabbageSettings::getColourFromValueTree(cabbageSettings->getValueTree(), CabbageColourIds::fileTabButton, Colour(50, 50, 50)));

    for ( auto button : fileTabs )
    {
        if (button != fileButton)
            button->disableButtons (true);
    }


    arrangeFileTabs();
}

void CabbageMainComponent::arrangeFileTabs()
{
    int xPos = resizerBarCurrentXPos;
    const int numTabs = 5;
    int tabIndex = 0;
    const int width = propertyPanel->isVisible() ? getWidth() - propertyPanel->getWidth() - 30 : getWidth() - 25;
    //const int size = fileTabs.size();
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
	fileTree.setBounds(-15, toolbarThickness, resizerBarCurrentXPos+25, getHeight()-5);
    goUpButton.setBounds(resizerBarCurrentXPos-55, toolbarThickness+5, 45, 22);
    resizerBar.setBounds(resizerBarCurrentXPos-5, toolbar.getHeight(), 3, getHeight());
    
    for ( CabbageEditorContainer* editor : editorAndConsole )
    {
        editor->statusBar.setSize (getWidth(), 28);
        editor->setBounds (resizerBarCurrentXPos, height, getWidth() - (isPropPanelVisible ? 200 : 0)-resizerBarCurrentXPos, getHeight() - 5);
    }

    arrangeFileTabs();
}

//==============================================================================
void CabbageMainComponent::createFilterGraph()
{
	deviceManager.addChangeListener(this);
	graphComponent = new GraphDocumentComponent(formatManager, deviceManager, knownPluginList);
	graphComponent->setSize(600, 400);
	filterGraphWindow->setContentOwned(graphComponent, true);
	getFilterGraph()->setCabbageSettings(cabbageSettings);
}
//==================================================================================
void CabbageMainComponent::showGraph()
{
	filterGraphWindow->setUsingNativeTitleBar (true);
	filterGraphWindow->setVisible (true);
	filterGraphWindow->setTopLeftPosition (getWidth() - filterGraphWindow->getWidth(), 10);
	filterGraphWindow->setAlwaysOnTop (true);
}
//==============================================================================
void CabbageMainComponent::showPluginListEditor()
{
    if (pluginListWindow == nullptr)
        pluginListWindow.reset (new PluginListWindow (*this, formatManager));
    
    pluginListWindow->toFront (true);
}
//==============================================================================
void CabbageMainComponent::createEditorForFilterGraphNode (juce::Point<int> position)
{

    String pluginName = "";
    AudioProcessorGraph::NodeID nodeId(fileTabs[currentFileIndex]->uniqueFileId);

	
    if (AudioProcessorGraph::Node::Ptr f = getFilterGraph()->graph.getNodeForId (nodeId))
    {
        PluginWindow::Type type = f->getProcessor()->hasEditor() ? PluginWindow::Type::normal
                                              : PluginWindow::Type::generic;


		if (CabbagePluginProcessor* cabbagePlugin = dynamic_cast<CabbagePluginProcessor*> (f->getProcessor()))
        {
            pluginName = cabbagePlugin->getPluginName();
            if(shouldRecord == true)
            {
                String time = Time::getCurrentTime().formatted("_%Y%m%d_%H%M%S");
                
                const String filename = getCurrentCsdFile().getFullPathName().substring(0, getCurrentCsdFile().getFullPathName().indexOf("."))+time+".wav";
                cabbagePlugin->startRecording(filename, bitDepth);
                factory.startRecordingTimer(false);
            }
        }

        if (PluginWindow* const w = getFilterGraph()->getOrCreateWindowFor(f.get(), type))
        {
        
			f->properties.set("PluginWindowX", position.getX());
			f->properties.set("PluginWindowY", position.getY());


            if (GenericCabbagePluginProcessor* cabbagePlugin = dynamic_cast<GenericCabbagePluginProcessor*> (f->getProcessor()))
                w->setVisible (false);
			else
			{
				w->toFront(true);
				w->setVisible(true);
			}
	
			w->addChangeListener(this);

            const int alwaysOnTop = cabbageSettings->getUserSettings()->getIntValue ("SetAlwaysOnTopPlugin");

            if (alwaysOnTop == 1)
                w->setAlwaysOnTop (true);
            else
                w->setAlwaysOnTop (false);

            if (position.getY() > 0 && position.getX() > 0)
                w->setTopLeftPosition (position.getX(), position.getY());

            w->setName(pluginName.length()>0 ? pluginName : "Plugin has no name?");
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
//==============================================================================
CabbageEditorContainer* CabbageMainComponent::getEditorContainer(int index)
{
	if (editorAndConsole.size() < index)
		return editorAndConsole[index];

	return nullptr;
}
//==================================================================================
CabbageCodeEditorComponent* CabbageMainComponent::getCurrentCodeEditor()
{
    if (getCurrentEditorContainer())
        return getCurrentEditorContainer()->editor.get();
    else
        return nullptr;
}
//==================================================================================
CabbageOutputConsole* CabbageMainComponent::getCurrentOutputConsole()
{
    if (getCurrentEditorContainer())
        return getCurrentEditorContainer()->outputConsole.get();
    else
        return nullptr;
}

//==================================================================================
CabbagePluginEditor* CabbageMainComponent::getCabbagePluginEditor()
{
    if (fileTabs.size() > 0)
    {
        if (getFilterGraph() !=  nullptr)
        {
            const AudioProcessorGraph::NodeID nodeId(fileTabs[currentFileIndex]->uniqueFileId);
            if (nodeId.uid != -99)
                if (AudioProcessorGraph::Node::Ptr f = getFilterGraph()->graph.getNodeForId(nodeId))
                {
                    if(CabbagePluginProcessor* processor = dynamic_cast<CabbagePluginProcessor*>(f->getProcessor()))
                    {
						return ((CabbagePluginEditor*)processor->getActiveEditor());
                    }
                }
        }
    }
    return nullptr;
  /*  if (fileTabs.size() > 0)
    {
        if (getFilterGraph() != nullptr)
        {

            const AudioProcessorGraph::NodeID nodeId(fileTabs[currentFileIndex]->uniqueFileId);
            if (nodeId.uid != 99)
            {
                auto& pluginWindows = getFilterGraph()->activePluginWindows;
                AudioProcessorGraph::Node::Ptr f = getFilterGraph()->graph.getNodeForId(nodeId);

                for (int i = 0; i < pluginWindows.size(); i++)
                {
                    if (pluginWindows[i]->node == f)
                    {
                        if (CabbagePluginEditor* editor = dynamic_cast<CabbagePluginEditor*> (pluginWindows[i]->getContentComponent()))
                            return editor;
                    }
                }
            }
        }
    }
    return nullptr;*/
}
//==================================================================================
CabbagePluginProcessor* CabbageMainComponent::getCabbagePluginProcessor()
{

    const AudioProcessorGraph::NodeID nodeId(fileTabs[currentFileIndex]->uniqueFileId);

    if (AudioProcessorGraph::Node::Ptr f = getFilterGraph()->graph.getNodeForId (nodeId))
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
void CabbageMainComponent::enableAutoUpdateMode()
{
//    int shouldUpdate = cabbageSettings->getUserSettings()->getIntValue ("AutoLoadFromDisk");
}

void CabbageMainComponent::addCabbageSection()
{
    const String cabbageSection = "<Cabbage>\nform size(500, 300), caption(\"Untitled\")\n</Cabbage>\n";
    getCurrentCodeEditor()->insertCode(0, cabbageSection, false, false);
}

void CabbageMainComponent::enableEditMode()
{
	
	const AudioProcessorGraph::NodeID nodeId(fileTabs[currentFileIndex]->uniqueFileId);
	if (nodeId.uid == -99)
		return;

    
	const bool isCabbageFile = CabbageUtilities::hasCabbageTags(getCurrentCsdFile());

	//stopCsoundForNode(getCurrentCsdFile().getFullPathName());
	//runCsoundForNode(getCurrentCsdFile().getFullPathName());


	if (isCabbageFile == true)
	{
        if (!getCabbagePluginEditor())
        {
            return;
        }
        
        if (CabbagePluginProcessor* cabbagePlugin = dynamic_cast<CabbagePluginProcessor*> (getFilterGraph()->graph.getNodeForId(nodeId)->getProcessor()))
        {
            cabbagePlugin->recreateWidgets(getCurrentCsdFile().loadFileAsString(), true);
        }
        
//        graphComponent->createNewPlugin(FilterGraph::getPluginDescriptor(nodeId, getCurrentCsdFile().getFullPathName()), { graphComponent->getWidth() / 2.f, graphComponent->getHeight() / 2.f });
//        juce::Point<int> pos = getFilterGraph()->getPositionOfCurrentlyOpenWindow(nodeId);
//        createEditorForFilterGraphNode(pos);
//        }

		getCabbagePluginEditor()->addChangeListener(this);
		getCabbagePluginEditor()->addActionListener(this);

		getFilterGraph()->graph.getNodeForId(nodeId)->getProcessor()->suspendProcessing(true);
		fileTabs[currentFileIndex]->getPlayButton().setToggleState(false, dontSendNotification);
		propertyPanel->setInterceptsMouseClicks(true, true);

		//getCabbagePluginEditor()->refreshValueTreeListeners();
		getCabbagePluginEditor()->enableEditMode(true);
		
		isGUIEnabled = true;
	}
}
//=======================================================================================
void CabbageMainComponent::showSettingsDialog()
{
    DialogWindow::LaunchOptions o;

    o.content.setOwned (new CabbageSettingsWindow (*cabbageSettings, new AudioDeviceSelectorComponent(deviceManager,
		0, 256,
		0, 256,
		true, true,
		true, false)));
    o.content->setSize (500, 450);
    o.dialogTitle = TRANS ("Cabbage Settings");
    o.dialogBackgroundColour = Colour (0xfff0f0f0);
    o.escapeKeyTriggersCloseButton = true;
    o.useNativeTitleBar = true;
    o.resizable = false;
    o.launchAsync();

}

String CabbageMainComponent::getDeviceManagerSettings()
{
	if (deviceManager.getCurrentAudioDevice())
	{
		std::unique_ptr<XmlElement> xml(deviceManager.createStateXml());

		if (xml == nullptr)
			return String();
		else
			return xml->createDocument("");
	}
	else return String();
}

void CabbageMainComponent::reloadAudioDeviceState()
{
	std::unique_ptr<XmlElement> savedState;

	if (cabbageSettings != nullptr)
		savedState = cabbageSettings->getUserSettings()->getXmlValue("audioSetup");

	deviceManager.initialise(256,
		256,
		savedState.get(),
		true);

}
//==============================================================================
void CabbageMainComponent::createNewProject()
{
    DialogWindow::LaunchOptions o;
    o.content.setOwned (new CabbageProjectWindow (this));
    o.content->setSize (850, 350);
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
	stopTimer();
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
    std::unique_ptr<XmlElement> xml (doc.getDocumentElement());

    if (xml == nullptr || ! xml->hasTagName ("FILTERGRAPH"))
        return;

    cabbageSettings->updateRecentFilesList (fileToOpen);

    forEachXmlChildElementWithTagName (*xml, filter, "FILTER"){
        CabbageUtilities::debug(filter->getIntAttribute ("uid"));
        uuids.add (filter->getIntAttribute ("uid"));
    }

    int uidOffset = 0;
    forEachXmlChildElementWithTagName (*xml, filter, "FILTER"){
        forEachXmlChildElementWithTagName (*filter, plugin, "PLUGIN")
        {
            String pluginName = plugin->getStringAttribute("file");
            const String pluginFile = fileToOpen.getParentDirectory().getChildFile(pluginName).getFullPathName();
            if(File(pluginFile).existsAsFile())
                files.add (File (pluginFile));
            else
                uidOffset++;
        }
    }

    while(fileTabs.size()>0)
    {
        closeDocument();
    }

    StringArray pluginFiles;
    Result res = getFilterGraph()->loadDocument (fileToOpen);
    
    //getFilterGraph()->closeAnyOpenPluginWindows();
    for ( int i = 0 ; i < files.size() ; i++)
    {
		String fileName = files[i].getFullPathName();
        if (files[i].existsAsFile() && files[i].getFileExtension() != ".cabbage" 
			&& files[i].existsAsFile() && files[i].getFileExtension() != ".dll" 
			&& files[i].existsAsFile() && files[i].getFileExtension() != ".vst3"
			&& files[i].existsAsFile() && files[i].getFileExtension() != ".vst"
			&& files[i].existsAsFile() && files[i].getFileExtension() != ".vst3"
			&& files[i].existsAsFile() && files[i].getFileExtension() != ".component")
        {

            File file = openFile (files[i].getFullPathName());
            fileTabs[getTabFileIndex(files[i])]->uniqueFileId = uuids[i+uidOffset];
			fileTabs[getTabFileIndex(files[i])]->getPlayButton().setToggleState(true, dontSendNotification);
            //runCsoundForNode(files[i].getFullPathName());
       }
    }
	
	getFilterGraph()->bringAllPluginWindowsToFront();

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
    if(saveAs==false)
    {
        if(getFilterGraph()->getLastDocumentOpened().existsAsFile()) {
            getFilterGraph()->saveDocument(getFilterGraph()->getLastDocumentOpened());
            return;
        }
    }

        //getFilterGraph()->saveGraph(saveAs);
        FileChooser fc("Save file as", File::getSpecialLocation(File::SpecialLocationType::userHomeDirectory), "", CabbageUtilities::shouldUseNativeBrowser());

        if(fc.browseForFileToSave(true))
            getFilterGraph()->saveDocument(fc.getResult().withFileExtension(".cabbage"));

    fileTree.refresh();
}
//==================================================================================
void CabbageMainComponent::openFolder ()
{
    
    FileChooser fc ("Open File", cabbageSettings->getMostRecentFile (0).getParentDirectory(), "*.csd;*", CabbageUtilities::shouldUseNativeBrowser());
        

    if (fc.browseForDirectory())
    {
        fileTree.refresh();
    }
    
    cabbageSettings->setProperty("lastOpenedDir", fc.getResult().getFullPathName());
}
//==================================================================================
void CabbageMainComponent::toggleBrowser()
{
    if(resizerBar.isVisible())
    {
        resizerBar.setVisible(false);
        resizerBarCurrentXPos = 0;
        resized();
        cabbageSettings->setProperty ("ShowFileBrowser", 0);
    }
    else
    {
        resizerBarCurrentXPos = 195;
//        fileList.refresh();
        fileTree.refresh();
        resizerBar.setVisible(true);
        cabbageSettings->setProperty ("ShowFileBrowser", 1);
        resized();
    }
}
//==================================================================================
const File CabbageMainComponent::openFile (String filename, bool updateRecentFiles, bool exportingPlugin)
{
    stopTimer();
    //stopCsoundForNode (filename);
    File currentCsdFile;

    if (File (filename).existsAsFile() == false)
    {
        FileChooser fc ("Open File", cabbageSettings->getMostRecentFile (0).getParentDirectory(), "*.csd;*", CabbageUtilities::shouldUseNativeBrowser());


        if (fc.browseForFileToOpen())
        {
			if (fc.getResult().getFileExtension() == ".cabbage")
			{
				openGraph(fc.getResult());
				return File();
			}

            if (getTabFileIndex (File (filename)) >= 0 && currentFileIndex > -1)
            {
                CabbageUtilities::showMessage ("File is already open", lookAndFeel.get());
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

    if (!exportingPlugin)
    {
        createCodeEditorForFile (currentCsdFile);
    }

    return currentCsdFile;

}
//==================================================================================
void CabbageMainComponent::launchHelpfile (String type)
{
    String url = "";
    String keyword = "";
    if (getCurrentCodeEditor())
    {
        CodeDocument::Position pos1, pos2;
        pos1 = getCurrentCodeEditor()->getDocument().findWordBreakBefore(getCurrentCodeEditor()->getCaretPos());
        pos2 = getCurrentCodeEditor()->getDocument().findWordBreakAfter(getCurrentCodeEditor()->getCaretPos());
        keyword = getCurrentCodeEditor()->getDocument().getTextBetween(pos1, pos2).trim();
    }
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
    editorAndConsole.add (editorConsole = new CabbageEditorContainer (cabbageSettings, file.hasFileExtension (".csd") || file.hasFileExtension (".orc")));
    addAndMakeVisible (editorConsole);
	editorConsole->setFile(file);
    propertyPanel.reset (new CabbagePropertiesPanel (cabbageSettings->valueTree));
    addAndMakeVisible (propertyPanel.get());
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
void CabbageMainComponent::saveDocument (bool saveAs, bool compileFromPlayButton)
{
	if (fileTabs.size() > 0)
	{
		stopTimer();
		if (saveAs == true)
		{

			stopCsoundForNode(getCurrentCsdFile().getFullPathName());

			isGUIEnabled = false;

			if (getCabbagePluginEditor() != nullptr)
				getCabbagePluginEditor()->enableEditMode(false);

            FileChooser fc("Select file name and location", getCurrentCsdFile().getParentDirectory(), "*.csd;*.txt;*.js;*.html;*.snaps;*.plant;*.xml", CabbageUtilities::shouldUseNativeBrowser());


			if (fc.browseForFileToSave(false))
			{
				if (fc.getResult().existsAsFile())
				{
					const int result = CabbageUtilities::showYesNoMessage("Do you wish to overwrite\nexiting file?", lookAndFeel.get());

					if (result == 1)
						writeFileToDisk(fc.getResult().withFileExtension(".csd"));
				}
                else
                {
                    if(fc.getResult().getFileName().contains("."))
					    writeFileToDisk(fc.getResult());
                    else
                        writeFileToDisk(fc.getResult().withFileExtension(".csd"));
                }
				getCurrentCodeEditor()->setSavePoint();
			}
            
			addInstrumentsAndRegionsToCombobox();
		}
		else
		{
			//check if file is part of installed example, prevent overwriting...
			String examplesDir = File::getSpecialLocation(File::currentExecutableFile).getParentDirectory().getFullPathName() + "/Examples";

#if defined(LINUX)
			//manualPath = File::getSpecialLocation (File::currentExecutableFile).getParentDirectory().getFullPathName() + "/CsoundDocs";
			examplesDir = "/usr/share/doc/cabbage/Examples";
#elif defined(MACOSX)
			examplesDir = File::getSpecialLocation(File::currentExecutableFile).getParentDirectory().getParentDirectory().getFullPathName() + "/Examples";
#endif

            bool compileOnSave = cabbageSettings->getUserSettings()->getBoolValue("CompileOnSave");

			stopCsoundForNode(getCurrentCsdFile().getFullPathName());;
			isGUIEnabled = false;

			if (getCabbagePluginEditor() != nullptr)
				getCabbagePluginEditor()->enableEditMode(false);

			if (getCurrentCodeEditor()->hasFileChanged())
			{
				if (getCurrentCsdFile().getFullPathName().contains(examplesDir)) {
					CabbageUtilities::showMessage("You cannot overwrite an example file. Please use save-as instead", lookAndFeel.get());
					return;
				}

				if (getCurrentCsdFile().existsAsFile())
					getCurrentCsdFile().replaceWithText(getCurrentCodeEditor()->getDocument().getAllContent());
			
				for (int i = 0; i < editorAndConsole.size(); i++)
				{
					String thisFile = getCurrentCsdFile().getFullPathName();
					String otherFile = editorAndConsole[i]->getFile().getFullPathName();
					if (getCurrentEditorContainer() != editorAndConsole[i] &&
						getCurrentCsdFile().getFullPathName() == editorAndConsole[i]->getFile().getFullPathName())
					{
						editorAndConsole[i]->editor->loadContent(getCurrentCodeEditor()->getDocument().getAllContent());
						stopCsoundForNode(getCurrentCsdFile().getFullPathName(), i);
                        if(compileOnSave)
						    runCsoundForNode(getCurrentCsdFile().getFullPathName(), i);

					}
					
				}
			}


			propertyPanel->setEnabled(false);

            if (compileOnSave || compileFromPlayButton)
            {
                if (getCurrentCsdFile().hasFileExtension((".csd")))
                {
                    runCsoundForNode(getCurrentCsdFile().getFullPathName());
                    fileTabs[currentFileIndex]->getPlayButton().setToggleState(true, dontSendNotification);
                    fileTabs[currentFileIndex]->lastModified = getCurrentCsdFile().getLastModificationTime();
                }
            }

			addInstrumentsAndRegionsToCombobox();
			getCurrentCodeEditor()->setSavePoint();
		}

		StringArray lines;
		lines.addLines(getCurrentCsdFile().loadFileAsString());
		for (int i = 0; i < lines.size(); i++)
		{
			if (lines[i] == "<CsOptions>")
			{
				const String csOptions = lines[i + 1];
				if (csOptions.contains("-+rtaudio"))
					CabbageUtilities::showMessage("You are using -+rtaudio to set an audio device. This is unsupported. Please set the audio device in the Cabbage audio settings and remove the -+rtaudio flag from your CsOptions.", lookAndFeel.get());
				if (csOptions.contains("-+rtmidi=") && (!csOptions.contains("-+rtmidi=NULL") && !csOptions.contains("-+rtmidi=null")))
					CabbageUtilities::showMessage("You are using -+rtmidi to set a MIDI device. This is unsupported. Please use -+rtmidi=NULL in your CsOptions, and select a MIDI device in the Cabbage audio settings.", lookAndFeel.get());

			}
		}

        fileTree.refresh();
	}

}
//==================================================================================
void CabbageMainComponent::writeFileToDisk (File file)
{
    if (file.hasFileExtension("csd;txt;js;html;snaps;plant;xml"))
    {
        file.replaceWithText(getCurrentCodeEditor()->getAllText());
        //setCurrentCsdFile (file);
    }

    //getCurrentCsdFile().replaceWithText (getCurrentCodeEditor()->getAllText());
    openFile(file.getFullPathName());

}
//==================================================================================
void CabbageMainComponent::closeDocument()
{
    if (editorAndConsole.size() > 0)
    {
        if (getCurrentCodeEditor()->hasFileChanged() == true)
        {
            const int result = CabbageUtilities::showYesNoMessage ("File has been modified, do you wish to save?\nexiting file?", lookAndFeel.get(), 1);

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
    
    if(editorAndConsole.size()==0)
        factory.combo->clearItemsFromComboBox();

}
//==================================================================================
void CabbageMainComponent::removeEditor()
{
    cabbageSettings->getUserSettings()->setValue ("FontSizeConsole", getCurrentEditorContainer()->outputConsole.get()->getFontSize());
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
int CabbageMainComponent::sendToPort()
{
	String code = getCurrentCodeEditor()->getSelectedText();
	int port = cabbageSettings->getUserSettings()->getIntValue("UDP Port");
	socket->write("127.0.0.1", 8009, code.toUTF8().getAddress(), code.toUTF8().sizeInBytes());

	return 0;
}

//==================================================================================

int CabbageMainComponent::testFileForErrors (String file)
{
    //this method will start a Csound process and test it for i-time errors and possible infinite loops.
    //It only runs 16 k-cycles, so it will not be able to detect perf-time hangs
    ChildProcess process;
    const String applicationDir = File::getSpecialLocation (File::currentExecutableFile).getParentDirectory().getFullPathName();
    const String processName = applicationDir + "/CabbageCsoundCLI";

    if (File (processName).existsAsFile())
    {
        const String output = process.readAllProcessOutput();

        process.start (processName + " " + file);
        process.waitForProcessToFinish(400);

        const int exitCode = process.getExitCode();

        if (exitCode == 1)
        {
            process.start ("csound " + file);
            this->getCurrentOutputConsole()->setText (process.readAllProcessOutput());
            stopCsoundForNode (file);
            return 1;
        }

        process.kill();
    }

    return 0;

}

void CabbageMainComponent::covertToLowerCase()
{
    String currentFileText = getCurrentCsdFile().loadFileAsString();
    
    CabbageIdentifierStrings camelCaseIdentifiers;

    for ( int i = camelCaseIdentifiers.size() ; i >=0 ; i--)
    {
        if(currentFileText.contains(camelCaseIdentifiers[i]))
        {
            //("Replacing"+camelCaseIdentifiers[i] + " with " + camelCaseIdentifiers[i].toLowerCase());
            currentFileText = currentFileText.replace(camelCaseIdentifiers[i], camelCaseIdentifiers[i].toLowerCase());
        }
    }
    
    getCurrentCodeEditor()->setAllText(currentFileText);
    
}

void CabbageMainComponent::updatePresetFile()
{
    FileChooser fc("Select prest file to convert", getCurrentCsdFile().getParentDirectory(), "*.snaps", CabbageUtilities::shouldUseNativeBrowser());
    

    if (fc.browseForFileToSave(false))
    {
        if (fc.getResult().existsAsFile())
        {
           CabbageUtilities::showMessage("Sorry. You cannot overwrite this file. Please choose a new file name.", lookAndFeel.get());
           return;
        }
        else
        {
            std::unique_ptr<XmlElement> myElement;
            myElement = XmlDocument::parse(getCurrentCsdFile());
            nlohmann::ordered_json j;

            if (myElement)
            {
                for (int i = 0; i < myElement->getNumChildElements(); i++)
                {
                    //none of these are being updated in their respective valueTreeChanged listeners..
                    auto child = myElement->getChildElement(i);
                    if(child->getNumAttributes()!=0)
                    {
                        const String presetName = child->getAttributeValue(0);
                        for ( int i = 1 ; i < child->getNumAttributes() ; i++)
                        {
                            if (child->getAttributeName(i).isQuotedString())
                                j[presetName.toStdString()][child->getAttributeName(i).toStdString()] = child->getAttributeValue(i).toStdString();
                            else
                                j[presetName.toStdString()][child->getAttributeName(i).toStdString()] = child->getDoubleAttribute(child->getAttributeName(i).toStdString(), 0.f);
                        }
                    }
                }
            }

            fc.getResult().replaceWithText(j.dump(4));
        }
    }
    
}

void CabbageMainComponent::covertToCamelCase()
{
    String currentFileText = getCurrentCsdFile().loadFileAsString();
    
    String cabbageCode = currentFileText.substring(0, currentFileText.indexOf("</Cabbage>")+10);
    String csoundCode = currentFileText.substring(currentFileText.indexOf("</Cabbage>")+10);
    
    CabbageIdentifierStrings camelCaseIdentifiers;
    for ( int i = camelCaseIdentifiers.size() ; i >=0 ; i--)
    {
        if(cabbageCode.contains(camelCaseIdentifiers[i].toLowerCase()))
        {
            //DBG("Replacing"+camelCaseIdentifiers[i].toLowerCase() + " with " + camelCaseIdentifiers[i]);
            cabbageCode = cabbageCode.replace(camelCaseIdentifiers[i].toLowerCase(), camelCaseIdentifiers[i]);
        }
    }
    
    
    getCurrentCodeEditor()->setAllText(cabbageCode+csoundCode);
           
}

void CabbageMainComponent::runCsoundForNode (String file, int fileTabIndex)
{
    startFilterGraph();
    if (testFileForErrors (file) == 0) //if Csound seg faults it will take Cabbage down. best to test the instrument in a separate process first.
    {
        if (File (file).existsAsFile())
        {

            StringArray warnings = preCompileCheckForIssues(File(file));

            AudioProcessorGraph::NodeID node(fileTabs[fileTabIndex != -99 ? fileTabIndex : currentFileIndex]->uniqueFileId);

            if (node.uid == -99)
            {
                Uuid uniqueID;
                node.uid = int32(*uniqueID.getRawData());
                fileTabs[fileTabIndex != -99 ? fileTabIndex : currentFileIndex]->uniqueFileId = node.uid;
            }

            Random rand;
            double posOffset = rand.nextDouble() * 0.2;
            juce::Point<double> pluginNodePos(.5 + posOffset, .5 + posOffset);
            juce::Point<int> pluginWindowPos(-1000, -1000);


            if (getFilterGraph()->graph.getNodeForId(node))
            {
                pluginNodePos = juce::Point<double>(getFilterGraph()->graph.getNodeForId(node)->properties.getWithDefault("x", rand.nextDouble()),
                    getFilterGraph()->graph.getNodeForId(node)->properties.getWithDefault("y", rand.nextDouble()));

                pluginWindowPos = juce::Point<int>(getFilterGraph()->graph.getNodeForId(node)->properties.getWithDefault("PluginWindowX", rand.nextInt(Range<int>(0, 500))),
                    getFilterGraph()->graph.getNodeForId(node)->properties.getWithDefault("PluginWindowY", rand.nextInt(Range<int>(0, 500))));

            }




            //getCurrentCsdFile().getParentDirectory().setAsCurrentWorkingDirectory();
            //this will create or update plugin...
            this->getCurrentOutputConsole()->setText("\n/*============================================================*/\n");
            graphComponent->createNewPlugin(FilterGraph::getPluginDescriptor(node, getCurrentCsdFile().getFullPathName()), pluginNodePos);


            createEditorForFilterGraphNode(pluginWindowPos);

            startTimer(500);
            if (getFilterGraph()->graph.getNodeForId(node))
            {
                fileTabs[fileTabIndex != -99 ? fileTabIndex : currentFileIndex]->getPlayButton().getProperties().set("state", "on");
                fileTabs[fileTabIndex != -99 ? fileTabIndex : currentFileIndex]->getPlayButton().setToggleState(true, dontSendNotification);
            }
            else
            {
                fileTabs[fileTabIndex != -99 ? fileTabIndex : currentFileIndex]->getPlayButton().getProperties().set("state", "on");
                fileTabs[fileTabIndex != -99 ? fileTabIndex : currentFileIndex]->getPlayButton().setToggleState(false, dontSendNotification);
            }

            factory.togglePlay(true);
            factory.setRecordButtonState("disabled");
            //hack to allow saving on the fly with JUCE 5.4.7 - needs investigation...

            graphComponent->enableAudioInput();
            
            if(warnings.size()>0)
            {
                Timer::callAfterDelay(1000, [warnings, this](){
                    this->getCurrentOutputConsole()->setText("/*"+ warnings.joinIntoString("\n") + "\n*/\n");
                });
            }
        }
        else
        CabbageUtilities::showMessage("Warning", "Please open a file first", lookAndFeel.get());
    }
}

StringArray CabbageMainComponent::preCompileCheckForIssues(File file)
{
    
    StringArray warnings;
    CabbageIdentifierStrings camelCaseIdentifiers;
    StringArray nonCamelCaseIdentifiers;
    auto fileContents = File(file).loadFileAsString();

    
    if (!fileContents.contains("<Cabbage>") || !fileContents.contains("</Cabbage>"))
    {
        warnings.add("\nWarning: Your .csd file does not contain a Cabbage section. Please make sure you have wrapped your Cabbage code in opening and closing <Cabbage> and </Cabbage> tags");
    }
    
    camelCaseIdentifiers.removeEmptyStrings();
    for (int i = camelCaseIdentifiers.size(); i >= 0; i--)
    {
        if (fileContents.contains(camelCaseIdentifiers[i].toLowerCase()) && camelCaseIdentifiers.contains(camelCaseIdentifiers[i].toLowerCase()) == false && camelCaseIdentifiers[i].isNotEmpty())
        {
            const CodeDocument::Position startPos (getCurrentCodeEditor()->getDocument(), fileContents.indexOf(camelCaseIdentifiers[i].toLowerCase()));
            int lineNum = startPos.getLineNumber() + 1;
            nonCamelCaseIdentifiers.add("\""+camelCaseIdentifiers[i].toLowerCase()+"\"(Line:"+String(lineNum)+")");
        }
    }


    if (nonCamelCaseIdentifiers.size() > 0)
    {
        const String infoText = "\nWarning: The following identifiers are not camelCase: " + nonCamelCaseIdentifiers.joinIntoString(",") + ". Cabbage uses camelCase for all identifiers, i.e, trackercolour() is now trackerColour(). Please use \"Convert Identifiers to camelCase\" from the File menu option to update your code. Or manually change the identifer listed.";
        warnings.addLines(infoText);
    }

    StringArray fileContentStrArray, problemChannels;
    fileContentStrArray.addLines(fileContents);
    StringArray channels;
    int lineIndex = 0;
    String guiMode = "";
    int guiModeWarning = 0;
    
    for (auto string : fileContentStrArray)
    {
        const String widgetTreeIdentifier = "tempWidget";
        ValueTree tempWidget(widgetTreeIdentifier);
        CabbageWidgetData::setWidgetState(tempWidget, string.trimCharactersAtStart(" \t"), -9);
        
        
        
        if(CabbageWidgetData::getStringProp(tempWidget, CabbageIdentifierIds::type) == "form")
        {
            guiMode = CabbageWidgetData::getStringProp(tempWidget, CabbageIdentifierIds::guimode);
            const String pluginId = CabbageWidgetData::getStringProp(tempWidget, CabbageIdentifierIds::pluginid);
            
            if (pluginId.length() != 4)
            {
                const CodeDocument::Position startPos (getCurrentCodeEditor()->getDocument(), fileContents.indexOf(pluginId));
                int lineNum = startPos.getLineNumber() + 1;
                warnings.add("\nWarning: The current pluginId(\""+pluginId+"\") (Line:"+String(lineNum)+") is not valid. A form pluginId() must be an alphanumeric string of 4 characters. ");
            }
        }
        else if(CabbageWidgetData::getStringProp(tempWidget, CabbageIdentifierIds::type) == "gentable")
        {
            const int drawMode = CabbageWidgetData::getNumProp(tempWidget, CabbageIdentifierIds::drawmode);
            if(drawMode != -1)
            {
            warnings.add("\nWarning: drawMode("+String(drawMode)+") (Line:"+String(lineIndex)+") is deprecated. Please use a meter widget");
            }
        }
        
        if (string.indexOf(";") == -1)
        {
            if(CabbageWidgetData::getNumProp(tempWidget, CabbageIdentifierIds::presetignore) == 0 &&
               CabbageWidgetData::getNumProp(tempWidget, CabbageIdentifierIds::automatable) == 1)
                channels.add(CabbageWidgetData::getStringProp(tempWidget, CabbageIdentifierIds::channel));
        }
        if (CabbageWidgetData::getStringProp(tempWidget, CabbageIdentifierIds::channel).contains(" "))
        {
            problemChannels.add("\""+CabbageWidgetData::getStringProp(tempWidget, CabbageIdentifierIds::channel)+"\" (Line:"+String(lineIndex)+")");
        }
        
        if (string.trimCharactersAtEnd("\t ").indexOf("opcode") == 0) {
            StringArray tokens;
            tokens.addTokens(string, false);
            const String newKeyword = tokens[1].removeCharacters((","));

            getCurrentEditorContainer()->csoundTokeniser.udoKeywords.add(newKeyword);

        }
        
        if(guiMode.isNotEmpty())
        {
            if (guiMode != "queue" && fileContents.contains("cabbageSet") && string.indexOf(";") == -1 && guiModeWarning == 0)
            {
                warnings.add("\nWarning: It looks like you are trying to use cabbageSet opcodes without guiMode(\"queue\") enabled. Please enable guiMode(\"queue\"), otherwise cabbageSet will not work");
                guiModeWarning = 1;
            }
            else if (guiMode == "queue" && string.contains("identChannel") && string.indexOf(";") == -1 && guiModeWarning == 0)
            {
                guiModeWarning = 1;
                warnings.add("\nWarning: It looks like you are trying to use identChannels() with guiMode(\"queue\"). These are not compatible.");
            }
        }
        
        lineIndex++;
    }

    channels.removeEmptyStrings();
    channels.appendNumbersToDuplicates(false, true, CharPointer_UTF8("|"), CharPointer_UTF8("|"));
    
    for( int i = channels.size() ; i >= 0 ; i--){
        if(channels[i].indexOf("|") == -1)
            channels.remove(i);
        else
        {
            const String chan = channels[i].substring(0, channels[i].indexOf("|"));
            const CodeDocument::Position startPos (getCurrentCodeEditor()->getDocument(), fileContents.indexOf(chan));
            int lineNum = startPos.getLineNumber() + 1;
            
            channels.set(i, "\""+channels[i].substring(0, channels[i].indexOf("|"))+"\" (Line:"+String(lineNum)+")");
            fileContents = fileContents.replaceFirstOccurrenceOf(chan, "test");            
        }
    }
    
    if(channels.size()>0)
    {
        const String channelError = channels.joinIntoString(",");
        const String infoText = "\nWarning: The following channel names are used across multiple widgets: " + channelError +". This is not permitted and can lead to unexpected behaviour";
        warnings.addLines(infoText);
    }
    
    
    if (problemChannels.size() > 0)
    {
        const String infoText = "\nWarning: White spaces are not supported in channel names. The following channels need to be changed: " + problemChannels.joinIntoString(",");
        warnings.addLines(infoText);
    }
    
    return warnings;
}
    
void CabbageMainComponent::stopCsoundForNode (String file, int fileTabIndex)
{

    if (fileTabs[fileTabIndex != -99 ? fileTabIndex : currentFileIndex] && File (file).existsAsFile())
    {
        AudioProcessorGraph::NodeID nodeId(fileTabs[fileTabIndex != -99 ? fileTabIndex : currentFileIndex]->uniqueFileId);
        
        if (getFilterGraph()->graph.getNodeForId(nodeId) != nullptr)
        {
            if (CabbagePluginProcessor* cabbagePlugin = dynamic_cast<CabbagePluginProcessor*> (getFilterGraph()->graph.getNodeForId(nodeId)->getProcessor()))
            {
                cabbagePlugin->recreateWidgets(getCurrentCsdFile().loadFileAsString(), false);
                cabbagePlugin->stopRecording();
                cabbagePlugin->stopTimer();
                cabbagePlugin->suspendProcessing(true);
                cabbagePlugin->resetCsound();

            }
            else
            {
                getFilterGraph()->graph.getNodeForId(nodeId)->getProcessor()->suspendProcessing(true);
            }
        }
        fileTabs[fileTabIndex != -99 ? fileTabIndex : currentFileIndex]->getPlayButton().getProperties().set("state", "off");
        fileTabs[fileTabIndex != -99 ? fileTabIndex : currentFileIndex]->getPlayButton().setToggleState(false, dontSendNotification);
    }
}
//==================================================================================
void CabbageMainComponent::startFilterGraph()
{
    graphComponent->enableGraph(true);
    factory.togglePlay (true);
}
//==================================================================================
void CabbageMainComponent::stopFilterGraph()
{
     graphComponent->enableGraph(false);
    factory.togglePlay (false);

}
//==============================================================================
void CabbageMainComponent::showFindPanel (bool withReplace)
{
    if (findPanel == nullptr)
    {
        findPanel.reset (new FindPanel (getSearchString(), isCaseSensitive, withReplace));
        addAndMakeVisible (findPanel.get());
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
    if(getCurrentCodeEditor())
        getCurrentCodeEditor()->grabKeyboardFocus();
}
//==============================================================================
void CabbageMainComponent::resized()
{
    const int heightOfTabButtons = (editorAndConsole.size() > 0) ? 35 : 0;
    toolbar.setBounds (getLocalBounds().removeFromTop  (toolbarThickness));

    if (propertyPanel->isVisible())
    {
        propertyPanel->setBounds (getWidth() - 200, toolbarThickness + heightOfTabButtons, 200, getHeight() - (toolbarThickness + heightOfTabButtons));
    }

    resizeAllWindows (toolbarThickness + heightOfTabButtons);

    if (findPanel != nullptr)
    {
        findPanel->setTopRightPosition (getWidth() - 16, 70);
    }

    arrangeFileTabs();
}

