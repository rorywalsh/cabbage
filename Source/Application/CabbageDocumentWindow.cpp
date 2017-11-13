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

#include "CabbageDocumentWindow.h"


enum
{
    recentProjectsBaseID = 100,
    activeDocumentsBaseID = 300,
    examplesMenuBaseID = 3000,
    colourSchemeBaseID = 1000
};

//=================================================================================================================
CabbageDocumentWindow::CabbageDocumentWindow (String name, String commandLineParams)  : DocumentWindow (name,
                                                                                  Colours::lightgrey,
                                                                                  DocumentWindow::allButtons),
																				  lookAndFeel(new LookAndFeel_V3()),
																				  commandLineArgs(commandLineParams)
																				  
{
    setTitleBarButtonsRequired (DocumentWindow::allButtons, false);
    setUsingNativeTitleBar (true);
    setResizable (true, true);
    centreWithSize (getWidth(), getHeight());
    setVisible (true);


	Desktop::getInstance().setDefaultLookAndFeel (lookAndFeel);	//set default look and feel for project
	getLookAndFeel().setColour(PopupMenu::ColourIds::highlightedBackgroundColourId, Colour(200, 200, 200));

    initSettings();
    setContentOwned (content = new CabbageMainComponent (this, cabbageSettings), true);
    content->propertyPanel->setVisible (false);
    cabbageSettings->addChangeListener (content);
    setMenuBar (this, 25);
    getMenuBarComponent()->setLookAndFeel (getContentComponent()->lookAndFeel);


	if(commandLineArgs.isNotEmpty())
	{
		if (SystemStats::getOperatingSystemType() == SystemStats::OperatingSystemType::MacOSX)
		//hocus pocus for OSX. It seems to append some gibbrish to the command line flags
		commandLineParams = commandLineParams.substring(0, commandLineParams.indexOf("-")-1);


		if(commandLineParams.contains("--export-VSTi"))
		{
			String inputFileName = commandLineParams.substring(commandLineParams.indexOf("--export-VSTi")+13).trim().removeCharacters("\"");
			if(File(inputFileName).existsAsFile())
			{
				content->openFile(inputFileName);
				CabbageUtilities::exportPlugin("VSTi", File(inputFileName), &getLookAndFeel(), getPluginId(File(inputFileName)));
				JUCEApplicationBase::quit();
			}

		}
		else if(commandLineParams.contains("--export-VST "))
		{
			String inputFileName = commandLineParams.substring(commandLineParams.indexOf("--export-VST")+12).trim().removeCharacters("\"");
			if(File(inputFileName).existsAsFile())
			{
				content->openFile(inputFileName);
				CabbageUtilities::exportPlugin("VST", File(inputFileName),  &getLookAndFeel(), getPluginId(File(inputFileName)));
				JUCEApplicationBase::quit();
			}

		}
		else if(File::getCurrentWorkingDirectory().getChildFile (commandLineParams.trim().removeCharacters("\"")).existsAsFile())
		{
			String CSDFile = commandLineParams.trim().removeCharacters("\"");;
			content->openFile(CSDFile);
		}
	}
	

    else if (cabbageSettings->getUserSettings()->getIntValue ("OpenMostRecentFileOnStartup") == 1)
    {
        const String lastOpenedFile = cabbageSettings->getUserSettings()->getValue("MostRecentFile", "");
        cabbageSettings->updateRecentFilesList();
        StringArray currentFiles;
		const int numberOfFileToOpen = cabbageSettings->getUserSettings()->getIntValue ("NumberOfOpenFiles");
		for( int i = 0 ; i < numberOfFileToOpen; i++ )
		{
			if(File(cabbageSettings->getMostRecentFile(i).getFullPathName()).existsAsFile() &&
                    cabbageSettings->getMostRecentFile(i).getFullPathName()!=lastOpenedFile)
            {
                content->openFile(cabbageSettings->getMostRecentFile(i).getFullPathName(), false);
                currentFiles.add(cabbageSettings->getMostRecentFile(i).getFullPathName());
            }
		}

        content->openFile(lastOpenedFile, false);
    }

    setApplicationCommandManagerToWatch (&commandManager);
    commandManager.registerAllCommandsForTarget (this);
    addKeyListener (commandManager.getKeyMappings());

#if JUCE_MAC
    MenuBarModel::setMacMainMenu (this, nullptr, "Open Recent");
#endif
    setLookAndFeel (&getContentComponent()->getLookAndFeel());
    lookAndFeelChanged();


    const int width = cabbageSettings->getUserSettings()->getIntValue ("IDE_LastKnownWidth");
    const int height = cabbageSettings->getUserSettings()->getIntValue ("IDE_LastKnownHeight");
    const int x = cabbageSettings->getUserSettings()->getIntValue ("IDE_LastKnownX");
    const int y = cabbageSettings->getUserSettings()->getIntValue ("IDE_LastKnownY");
    this->setTopLeftPosition (x, y);
    setSize (width, height);

}

void CabbageDocumentWindow::initSettings()
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
}

CabbageDocumentWindow::~CabbageDocumentWindow()
{
    setMenuBar (nullptr);

#if JUCE_MAC
    MenuBarModel::setMacMainMenu (nullptr);
#endif

    cabbageSettings->setProperty ("IDE_LastKnownWidth", getWidth());
    cabbageSettings->setProperty ("IDE_LastKnownHeight", getHeight());
    cabbageSettings->setProperty ("IDE_LastKnownX", getX());
    cabbageSettings->setProperty ("IDE_LastKnownY", getY());

    if (getContentComponent()->getCurrentCodeEditor() && getContentComponent()->getStatusbarYPos()<(getHeight()-50))
        cabbageSettings->setProperty ("IDE_StatusBarPos", getContentComponent()->getStatusbarYPos());

    cabbageSettings->setProperty ("audioSetup", getContentComponent()->getAudioDeviceSettings());
    cabbageSettings->closeFiles();

    setLookAndFeel(nullptr);

}
//=======================================================================================
CabbageMainComponent* CabbageDocumentWindow::getContentComponent()
{
    return content;
}

void CabbageDocumentWindow::maximiseButtonPressed()
{
    getContentComponent()->resizeAllWindows (getHeight());
}

void CabbageDocumentWindow::closeButtonPressed()
{
    CabbageIDELookAndFeel lookAndFeel;

    if (getContentComponent()->getAudioGraph()->hasChangedSinceSaved())
    {
        const int result = CabbageUtilities::showYesNoMessage ("Save changes made to Cabbage\npatch?", &lookAndFeel, 1);

        if (result == 1)
        {
            if (getContentComponent()->getAudioGraph()->saveGraph() == FileBasedDocument::SaveResult::userCancelledSave)
                return;
        }
        else if (result == 2)
        {
            JUCEApplicationBase::quit();
            return;
        }
        else
            return;
    }

    JUCEApplicationBase::quit();

}

StringArray CabbageDocumentWindow::getMenuBarNames()
{
    const char* const names[] = { "File", "Edit", "Tools", "View", "Help", nullptr };
    return StringArray (names);
}


PopupMenu CabbageDocumentWindow::getMenuForIndex (int topLevelMenuIndex, const String& menuName)
{
    PopupMenu menu;

    if (menuName == "File")             createFileMenu   (menu);
    else if (menuName == "Edit")        createEditMenu   (menu);
    else if (menuName == "View")        createViewMenu   (menu);
    else if (menuName == "Tools")       createToolsMenu  (menu);
    else if (menuName == "Help")        createHelpMenu   (menu);
    else                                jassertfalse; // names have changed?

    return menu;

}

void CabbageDocumentWindow::createFileMenu (PopupMenu& menu)
{
    menu.addCommandItem (&commandManager, CommandIDs::newFile);
    menu.addSeparator();
    menu.addCommandItem (&commandManager, CommandIDs::open);
    menu.addCommandItem (&commandManager, CommandIDs::openCabbagePatch);

    PopupMenu recentFilesMenu;
    cabbageSettings->updateRecentFilesList();
    cabbageSettings->recentFiles.createPopupMenuItems (recentFilesMenu, recentProjectsBaseID, true, true);
    menu.addSubMenu ("Open Recent", recentFilesMenu);

    const String examplesDir = cabbageSettings->getUserSettings()->getValue ("CabbageExamplesDir", "");

    PopupMenu examplesMenu;
    CabbageUtilities::addExampleFilesToPopupMenu (examplesMenu, exampleFiles, examplesDir, "*.csd", examplesMenuBaseID);

    menu.addSubMenu ("Examples", examplesMenu);
    menu.addSeparator();
    menu.addCommandItem (&commandManager, CommandIDs::saveDocument);
    menu.addCommandItem (&commandManager, CommandIDs::saveDocumentAs);
    menu.addSeparator();
    menu.addCommandItem (&commandManager, CommandIDs::saveGraph);
    menu.addCommandItem (&commandManager, CommandIDs::saveGraphAs);
    menu.addSeparator();
    menu.addCommandItem (&commandManager, CommandIDs::saveAll);
    menu.addSeparator();
    menu.addCommandItem (&commandManager, CommandIDs::openFromRPi);
    menu.addCommandItem (&commandManager, CommandIDs::saveDocumentToRPi);
    menu.addSeparator();
    menu.addCommandItem (&commandManager, CommandIDs::exportAsEffect);
    menu.addCommandItem (&commandManager, CommandIDs::exportAsSynth);

    if (SystemStats::getOperatingSystemType() != SystemStats::OperatingSystemType::Linux)
        menu.addCommandItem (&commandManager, CommandIDs::exportAsFMODSoundPlugin);

    menu.addSeparator();
    menu.addCommandItem (&commandManager, CommandIDs::closeProject);
    menu.addCommandItem (&commandManager, CommandIDs::saveProject);
    menu.addSeparator();
    menu.addCommandItem (&commandManager, CommandIDs::closeDocument);
#if ! JUCE_MAC
    menu.addSeparator();
    menu.addCommandItem (&commandManager, StandardApplicationCommandIDs::quit);
#endif
}

void CabbageDocumentWindow::createEditMenu (PopupMenu& menu)
{
    PopupMenu subMenu;
    menu.addCommandItem (&commandManager, CommandIDs::undo);
    menu.addCommandItem (&commandManager, CommandIDs::redo);
    menu.addSeparator();
    menu.addCommandItem (&commandManager, CommandIDs::cut);
    menu.addCommandItem (&commandManager, CommandIDs::copy);
    menu.addCommandItem (&commandManager, CommandIDs::paste);
    menu.addSeparator();

    menu.addSeparator();
    menu.addCommandItem (&commandManager, CommandIDs::del);
    menu.addCommandItem (&commandManager, CommandIDs::selectAll);
    menu.addCommandItem (&commandManager, CommandIDs::deselectAll);
    menu.addSeparator();
    menu.addCommandItem (&commandManager, CommandIDs::editMode);
    menu.addCommandItem (&commandManager, CommandIDs::toggleComments);
    menu.addSeparator();
    menu.addCommandItem (&commandManager, CommandIDs::zoomIn);
    menu.addCommandItem (&commandManager, CommandIDs::zoomOut);
	
	subMenu.addCommandItem (&commandManager, CommandIDs::zoomInConsole);
    subMenu.addCommandItem (&commandManager, CommandIDs::zoomOutConsole);
	subMenu.addCommandItem (&commandManager, CommandIDs::clearConsole);
	
	
    menu.addCommandItem (&commandManager, CommandIDs::showFindPanel);
    menu.addCommandItem (&commandManager, CommandIDs::showReplacePanel);

    menu.addCommandItem (&commandManager, CommandIDs::findSelection);
    menu.addCommandItem (&commandManager, CommandIDs::findNext);
    menu.addCommandItem (&commandManager, CommandIDs::findPrevious);
	menu.addSeparator();
	menu.addSubMenu("Console", subMenu);
	menu.addSeparator();	
    menu.addSeparator();
    menu.addCommandItem (&commandManager, CommandIDs::settings);

}

void CabbageDocumentWindow::createViewMenu (PopupMenu& menu)
{
	menu.addCommandItem (&commandManager, CommandIDs::nextTab);
    menu.addCommandItem (&commandManager, CommandIDs::showGraph);
    menu.addSeparator();
}


void CabbageDocumentWindow::createToolsMenu (PopupMenu& menu)
{
	menu.addCommandItem(&commandManager, CommandIDs::buildNow);
    menu.addSeparator();
	menu.addCommandItem (&commandManager, CommandIDs::startAudioGraph);
    menu.addCommandItem (&commandManager, CommandIDs::stopAudioGraph);
    menu.addSeparator();
    menu.addCommandItem (&commandManager, CommandIDs::exportAsSynth);
    menu.addCommandItem (&commandManager, CommandIDs::exportAsEffect);
    menu.addCommandItem (&commandManager, CommandIDs::exportAsFMODSoundPlugin);
    menu.addSeparator();
}

void CabbageDocumentWindow::createHelpMenu (PopupMenu& menu)
{
    menu.addCommandItem (&commandManager, CommandIDs::cabbageHelp);
    menu.addCommandItem (&commandManager, CommandIDs::csoundHelp);
    menu.addSeparator();
    menu.addCommandItem (&commandManager, CommandIDs::contextHelp);
    menu.addSeparator();
	menu.addCommandItem (&commandManager, CommandIDs::about);
}

void CabbageDocumentWindow::menuItemSelected (int menuItemID, int topLevelMenuIndex)
{
    if (menuItemID >= recentProjectsBaseID && menuItemID < recentProjectsBaseID + 100)
    {
        const File file = cabbageSettings->recentFiles.getFile (menuItemID - recentProjectsBaseID).getFullPathName();

        if (file.hasFileExtension (".cabbage"))
            getContentComponent()->openGraph (cabbageSettings->recentFiles.getFile (menuItemID - recentProjectsBaseID));
        else
            getContentComponent()->openFile (cabbageSettings->recentFiles.getFile (menuItemID - recentProjectsBaseID).getFullPathName());
    }
    else if (menuItemID >= examplesMenuBaseID && menuItemID < exampleFiles.size() + examplesMenuBaseID)
    {
        getContentComponent()->openFile (exampleFiles[menuItemID - examplesMenuBaseID].getFullPathName());
    }
}

void CabbageDocumentWindow::focusGained (FocusChangeType cause) //grab focus when user clicks on editor
{
    if (getContentComponent() && getContentComponent()->getCurrentCodeEditor())
    {
        getContentComponent()->getCurrentCodeEditor()->setWantsKeyboardFocus (true);
        getContentComponent()->getCurrentCodeEditor()->grabKeyboardFocus();
    }
}

void CabbageDocumentWindow::getAllCommands (Array <CommandID>& commands)
{

    const CommandID ids[] = { CommandIDs::openCabbagePatch,
                              CommandIDs::newFile,
                              CommandIDs::open,
                              CommandIDs::openFromRPi,
                              CommandIDs::closeAllDocuments,
                              CommandIDs::closeDocument,
                              CommandIDs::saveDocument,
							  CommandIDs::buildNow,
                              CommandIDs::saveGraph,
                              CommandIDs::saveGraphAs,
                              CommandIDs::saveDocumentToRPi,
                              CommandIDs::saveDocumentAs,
                              CommandIDs::examples,
                              CommandIDs::settings,
                              CommandIDs::startAudioGraph,
                              CommandIDs::stopAudioGraph,
                              CommandIDs::exportAsSynth,
                              CommandIDs::selectAll,
                              CommandIDs::exportAsEffect,
							  CommandIDs::nextTab,
                              CommandIDs::exportAsFMODSoundPlugin,
                              CommandIDs::copy,
                              CommandIDs::cut,
							  CommandIDs::clearConsole,
                              CommandIDs::toggleComments,
                              CommandIDs::zoomIn,
                              CommandIDs::zoomOut,
                              CommandIDs::zoomInConsole,
                              CommandIDs::zoomOutConsole,
                              CommandIDs::paste,
                              CommandIDs::undo,
                              CommandIDs::showFindPanel,
                              CommandIDs::showReplacePanel,
                              CommandIDs::redo,
                              CommandIDs::editMode,
                              CommandIDs::showGraph,
                              CommandIDs::about,
                              CommandIDs::runDiagnostics,
                              CommandIDs::csoundHelp,
                              CommandIDs::cabbageHelp,
                              CommandIDs::contextHelp,
                              CommandIDs::showGenericWidgetWindow
                            };

    commands.addArray (ids, numElementsInArray (ids));
}

void CabbageDocumentWindow::getCommandInfo (CommandID commandID, ApplicationCommandInfo& result)
{
    bool shouldShowEditMenu = false;

    if (getContentComponent()->getCurrentEditorContainer() != nullptr)
        shouldShowEditMenu = true;

    switch (commandID)
    {
        case CommandIDs::newFile:
            result.setInfo ("New Csound file", "Create a new Csound file", CommandCategories::general, 0);
            result.defaultKeypresses.add (KeyPress ('n', ModifierKeys::commandModifier, 0));
            break;

        case CommandIDs::openCabbagePatch:
            result.setInfo ("Open Cabbage patch...", "Opens a Cabbage patch", CommandCategories::general, 0);
            result.defaultKeypresses.add (KeyPress ('o', ModifierKeys::commandModifier | ModifierKeys::altModifier, 0));
            break;

        case CommandIDs::open:
            result.setInfo ("Open Csound file", "Opens a project", CommandCategories::general, 0);
            result.defaultKeypresses.add (KeyPress ('o', ModifierKeys::commandModifier, 0));
            break;

        case CommandIDs::closeDocument:
            result.setInfo ("Close file", "Closes a file", CommandCategories::general, 0);
            result.defaultKeypresses.add (KeyPress ('w', ModifierKeys::commandModifier, 0));
            break;

        case CommandIDs::examples:
            result.setInfo ("Examples", "Open an example", CommandCategories::general, 0);
            break;

        case CommandIDs::openFromRPi:
            result.setInfo ("Open Csound file from RPi", "Opens a file from a RPi", CommandCategories::general, 0);
            result.defaultKeypresses.add (KeyPress ('o', ModifierKeys::commandModifier | ModifierKeys::shiftModifier, 0));
            break;

        case CommandIDs::saveDocument:
            result.setInfo ("Save file", "Save a document", CommandCategories::general, 0);
            result.defaultKeypresses.add (KeyPress ('s', ModifierKeys::commandModifier, 0));
            break;

        case CommandIDs::saveGraph:
            result.setInfo ("Save Cabbage patch", "Save a patch document", CommandCategories::general, 0);
            result.defaultKeypresses.add (KeyPress ('s', ModifierKeys::commandModifier | ModifierKeys::altModifier, 0));
            break;

        case CommandIDs::saveGraphAs:
            result.setInfo ("Save Cabbage patch as...", "Save a patch document as", CommandCategories::general, 0);
            result.defaultKeypresses.add (KeyPress ('s', ModifierKeys::commandModifier | ModifierKeys::altModifier | ModifierKeys::shiftModifier, 0));
            break;

        case CommandIDs::saveDocumentToRPi:
            result.setInfo ("Save Csound file to RPi", "Save a document to RPi", CommandCategories::general, 0);
            result.defaultKeypresses.add (KeyPress ('s', ModifierKeys::commandModifier, 0));
            break;

        case CommandIDs::saveDocumentAs:
            result.setInfo ("Save file as...", "Save a document", CommandCategories::general, 0);
            result.defaultKeypresses.add (KeyPress ('s', ModifierKeys::shiftModifier | ModifierKeys::commandModifier, 0));
            break;

        case CommandIDs::settings:
            result.setInfo ("Settings", "Change Cabbage settings", CommandCategories::general, 0);
            break;

        case CommandIDs::buildNow:
            result.setInfo ("Build instrument", "Builds the current instrument", CommandCategories::general, 0);
            result.defaultKeypresses.add (KeyPress ('s', ModifierKeys::commandModifier, 0));
            break;
			
        case CommandIDs::startAudioGraph:
            result.setInfo ("Start graph", "Starts the audio signal graph", CommandCategories::general, 0);
            result.defaultKeypresses.add (KeyPress (KeyPress::F4Key, ModifierKeys::noModifiers, 0));
            break;

        case CommandIDs::stopAudioGraph:
            result.setInfo ("Stop graph", "Stop the audio signal graph", CommandCategories::general, 0);
            result.defaultKeypresses.add (KeyPress (KeyPress::F5Key, ModifierKeys::noModifiers, 0));
            break;

        case CommandIDs::nextTab:
            result.setInfo ("Next Tab", "Go to next tab", CommandCategories::general, 0);
            result.addDefaultKeypress (KeyPress::tabKey, ModifierKeys::commandModifier);
            break;

        case CommandIDs::exportAsSynth:
            result.setInfo ("Export as Plugin Synth", "Exports as plugin", CommandCategories::general, 0);
            break;

        case CommandIDs::exportAsEffect:
            result.setInfo ("Export as Plugin Effect", "Exports as plugin", CommandCategories::general, 0);
            break;

        case CommandIDs::exportAsFMODSoundPlugin:
            result.setInfo ("Export as FMOD Sound Plugin", "Exports as plugin", CommandCategories::general, 0);
            break;

        case CommandIDs::closeAllDocuments:
            result.setInfo ("Close All Documents", "Closes all open documents", CommandCategories::general, 0);
            break;

        case CommandIDs::saveAll:
            result.setInfo ("Save All", "Saves all open documents", CommandCategories::general, 0);
            result.defaultKeypresses.add (KeyPress ('s', ModifierKeys::commandModifier | ModifierKeys::altModifier, 0));
            break;

        //edit commands
        case CommandIDs::undo:
            result.setInfo (String ("Undo"), String ("Undo last action"), CommandCategories::edit, 0);
            result.addDefaultKeypress ('z', ModifierKeys::commandModifier);
            result.setActive ((shouldShowEditMenu ? true : false));
            break;

        case CommandIDs::redo:
            result.setInfo (String ("Redo"), String ("Redo last action"), CommandCategories::edit, 0);
            result.addDefaultKeypress ('z', ModifierKeys::shiftModifier | ModifierKeys::commandModifier);
            result.setActive ((shouldShowEditMenu ? true : false));
            break;

        case CommandIDs::cut:
            result.setInfo (String ("Cut"), String ("Cut selection"), CommandCategories::edit, 0);
            result.addDefaultKeypress ('x', ModifierKeys::commandModifier);
            result.setActive ((shouldShowEditMenu ? true : false));
            break;

        case CommandIDs::copy:
            result.setInfo (String ("Copy"), String ("Copy selection"), CommandCategories::edit, 0);
            result.addDefaultKeypress ('c', ModifierKeys::commandModifier);
            result.setActive ((shouldShowEditMenu ? true : false));
            break;

        case CommandIDs::paste:
            result.setInfo (String ("Paste"), String ("Paste selection"), CommandCategories::edit, 0);
            result.addDefaultKeypress ('v', ModifierKeys::commandModifier);
            result.setActive ((shouldShowEditMenu ? true : false));
            break;

        case CommandIDs::selectAll:
            result.setInfo (String ("Select all"), String ("Select all"), CommandCategories::edit, 0);
            result.addDefaultKeypress ('a', ModifierKeys::commandModifier);
            result.setActive ((shouldShowEditMenu ? true : false));
            break;


        case CommandIDs::columnEdit:
            result.setInfo (String ("Column Edit mode"), String ("Column Edit"), CommandCategories::edit, 0);
            result.addDefaultKeypress ('l', ModifierKeys::commandModifier);
            result.setActive ((shouldShowEditMenu ? true : false));
            break;

        case CommandIDs::toggleComments:
            result.setInfo (String ("Toggle comments"), String ("Toggle comments"), CommandCategories::edit, 0);
            result.addDefaultKeypress ('/', ModifierKeys::commandModifier);
            result.setActive ((shouldShowEditMenu ? true : false));
            break;

        case CommandIDs::searchReplace:
            result.setInfo (String ("Search or Replace"), String ("Search Replace"), CommandCategories::edit, 0);
            result.addDefaultKeypress ('f', ModifierKeys::commandModifier);
            result.setActive ((shouldShowEditMenu ? true : false));
            break;

        case CommandIDs::zoomIn:
            result.setInfo (String ("Zoom in"), String ("Zoom in"), CommandCategories::edit, 0);
            result.addDefaultKeypress ('[', ModifierKeys::commandModifier);
            result.setActive ((shouldShowEditMenu ? true : false));
            break;

        case CommandIDs::zoomOut:
            result.setInfo (String ("Zoom out"), String ("Zoom out"), CommandCategories::edit, 0);
            result.addDefaultKeypress (']', ModifierKeys::commandModifier);
            result.setActive ((shouldShowEditMenu ? true : false));
            break;
			
        case CommandIDs::zoomInConsole:
            result.setInfo (String ("Zoom in console"), String ("Zoom in console"), CommandCategories::edit, 0);
            result.addDefaultKeypress ('{', ModifierKeys::shiftModifier | ModifierKeys::commandModifier);
            result.setActive ((shouldShowEditMenu ? true : false));
            break;

        case CommandIDs::zoomOutConsole:
            result.setInfo (String ("Zoom out console"), String ("Zoom out console"), CommandCategories::edit, 0);
            result.addDefaultKeypress ('}', ModifierKeys::shiftModifier | ModifierKeys::commandModifier);
            result.setActive ((shouldShowEditMenu ? true : false));
            break;

        case CommandIDs::clearConsole:
            result.setInfo (String ("Clear console"), String ("Clear console"), CommandCategories::edit, 0);
            result.addDefaultKeypress ('W', ModifierKeys::shiftModifier | ModifierKeys::commandModifier);
            result.setActive ((shouldShowEditMenu ? true : false));
            break;
			
        case CommandIDs::showGenericWidgetWindow:
            result.setInfo (String ("Show Generic Widget Window"), String ("Show genric channel based widgets"), CommandCategories::general, 0);
            break;

        case CommandIDs::editMode:
            result.setInfo (String ("Edit Mode"), String ("Edit Mode"), CommandCategories::edit, 0);
            result.addDefaultKeypress ('e', ModifierKeys::commandModifier);
            result.setTicked (getContentComponent()->getCabbagePluginEditor() == nullptr ? false : getContentComponent()->getCabbagePluginEditor()->isEditModeEnabled());
            result.setActive ((shouldShowEditMenu ? true : false));
            break;

        case CommandIDs::showFindPanel:
            result.setInfo (TRANS ("Find"), TRANS ("Searches for text in the current document."), "Editing", 0);
            result.defaultKeypresses.add (KeyPress ('f', ModifierKeys::commandModifier, 0));
            break;

        case CommandIDs::showReplacePanel:
            result.setInfo (TRANS ("Replace"), TRANS ("Replaces text in the current document."), "Editing", 0);
            result.defaultKeypresses.add (KeyPress ('r', ModifierKeys::commandModifier, 0));
            break;

        case CommandIDs::findNext:
            result.setInfo (TRANS ("Find Next"), TRANS ("Searches for the next occurrence of the current search-term."), "Editing", 0);
            result.defaultKeypresses.add (KeyPress ('g', ModifierKeys::commandModifier, 0));
            break;

        case CommandIDs::about:
            result.setInfo (TRANS ("About"), TRANS ("About."), CommandCategories::general, 0);
            break;

        case CommandIDs::showGraph:
            result.setInfo (TRANS ("Show Cabbage Patcher"), TRANS ("Graph."), CommandCategories::view, 0);
            result.defaultKeypresses.add (KeyPress ('p', ModifierKeys::commandModifier, 0));
            break;

        case CommandIDs::findPrevious:
            result.setInfo (TRANS ("Find Previous"), TRANS ("Searches for the previous occurrence of the current search-term."), "Editing", 0);
            result.defaultKeypresses.add (KeyPress ('g', ModifierKeys::commandModifier | ModifierKeys::shiftModifier, 0));
            break;

        case CommandIDs::runDiagnostics:
            result.setInfo (String ("Run diagnostics"), String ("Run diagnostics"), CommandCategories::edit, 0);
            result.addDefaultKeypress ('d', ModifierKeys::commandModifier);
            break;

        // help command
        case CommandIDs::csoundHelp:
            result.setInfo (TRANS ("Csound Manual"), TRANS ("Open Csound manual"), "Help", 0);

            if (CabbageUtilities::getTargetPlatform() == CabbageUtilities::TargetPlatformTypes::OSX)
                result.defaultKeypresses.add (KeyPress ('2', ModifierKeys::commandModifier, 0));
            else
                result.defaultKeypresses.add (KeyPress (KeyPress::F2Key));

            break;

        case CommandIDs::cabbageHelp:
            result.setInfo (TRANS ("Cabbage Manual"), TRANS ("Open Cabbage manual"), "Help", 0);

            if (CabbageUtilities::getTargetPlatform() == CabbageUtilities::TargetPlatformTypes::OSX)
                result.defaultKeypresses.add (KeyPress ('3', ModifierKeys::commandModifier, 0));
            else
                result.defaultKeypresses.add (KeyPress (KeyPress::F3Key));

            break;

        case CommandIDs::contextHelp:
            result.setInfo (TRANS ("Context Help"), TRANS ("Context Help"), "Help", 0);

            if (CabbageUtilities::getTargetPlatform() == CabbageUtilities::TargetPlatformTypes::OSX)
                result.defaultKeypresses.add (KeyPress ('1', ModifierKeys::commandModifier, 0));
            else
                result.defaultKeypresses.add (KeyPress (KeyPress::F1Key));

            break;

        default:

            break;
    }
}

bool CabbageDocumentWindow::perform (const InvocationInfo& info)
{
    String title (CABBAGE_VERSION);
    CabbageIDELookAndFeel tempLookAndFeel;

    switch (info.commandID)
    {
        case CommandIDs::newFile:
            getContentComponent()->createNewProject();
            return true;

        case CommandIDs::openCabbagePatch:
            getContentComponent()->openGraph();
            return true;

        case CommandIDs::open:
            getContentComponent()->openFile();
            return true;

        case CommandIDs::openFromRPi:
            getContentComponent()->launchSSHFileBrowser ("open");
            return true;

        case CommandIDs::saveDocumentAs:
            getContentComponent()->saveDocument (true);
            return true;

        case CommandIDs::saveGraph:
            getContentComponent()->saveGraph (false);
            return true;

        case CommandIDs::saveGraphAs:
            getContentComponent()->saveGraph (true);
            return true;

		case CommandIDs::buildNow:
        case CommandIDs::saveDocument:
            getContentComponent()->saveDocument();
            getContentComponent()->setEditMode (false);
            isGUIEnabled = false;
            break;

        case CommandIDs::closeDocument:
            getContentComponent()->closeDocument();
            return true;

        case CommandIDs::closeAllDocuments:
            return true;

        case CommandIDs::nextTab:
            getContentComponent()->handleFileTab(nullptr, true);
			return true;
			
        case CommandIDs::settings:
            getContentComponent()->showSettingsDialog();
            return true;

        case CommandIDs::startAudioGraph:
            getContentComponent()->startAudioGraph();
            return true;

        case CommandIDs::exportAsEffect:
            CabbageUtilities::exportPlugin ("VST", getContentComponent()->getCurrentCsdFile(),  &getLookAndFeel(), getPluginId(getContentComponent()->getCurrentCsdFile().getFullPathName()));
            return true;

        case CommandIDs::exportAsSynth:
            CabbageUtilities::exportPlugin ("VSTi", getContentComponent()->getCurrentCsdFile(),  &getLookAndFeel(), getPluginId(getContentComponent()->getCurrentCsdFile().getFullPathName()));
            return true;

        case CommandIDs::toggleComments:
            getContentComponent()->getCurrentCodeEditor()->toggleComments();
            return true;

        case CommandIDs::runDiagnostics:
            getContentComponent()->getCurrentCodeEditor()->runInDebugMode();
            return true;

        case CommandIDs::zoomIn:
            getContentComponent()->getCurrentCodeEditor()->zoomIn();
            return true;

        case CommandIDs::zoomOut:
            getContentComponent()->getCurrentCodeEditor()->zoomOut();
            return true;
			
        case CommandIDs::zoomInConsole:
            getContentComponent()->getCurrentOutputConsole()->zoom(true);
            return true;

        case CommandIDs::zoomOutConsole:
            getContentComponent()->getCurrentOutputConsole()->zoom(false);
            return true;

        case CommandIDs::findNext:
            getContentComponent()->findNext (true);
            return true;

        case CommandIDs::findPrevious:
            getContentComponent()->findNext  (false);
            return true;

        case CommandIDs::showFindPanel:
            getContentComponent()->showFindPanel (false);
            return true;

        case CommandIDs::showReplacePanel:
            getContentComponent()->showFindPanel (true);
            return true;

        case CommandIDs::clearConsole:
            getContentComponent()->getCurrentOutputConsole()->clearText();
            return true;
			
        case CommandIDs::exportAsFMODSoundPlugin:
            return true;

        case CommandIDs::copy:
            getContentComponent()->getCurrentCodeEditor()->copy();
            return true;

        case CommandIDs::selectAll:
            getContentComponent()->getCurrentCodeEditor()->selectAll();
            return true;

        case CommandIDs::cut:
            getContentComponent()->getCurrentCodeEditor()->cut();
            return true;

        case CommandIDs::undo:
            getContentComponent()->getCurrentCodeEditor()->undo();
            return true;

        case CommandIDs::redo:
            getContentComponent()->getCurrentCodeEditor()->redo();
            return true;

        case CommandIDs::paste:
            getContentComponent()->getCurrentCodeEditor()->paste();
            return true;

        case CommandIDs::saveDocumentToRPi:
            getContentComponent()->saveDocument();
            getContentComponent()->launchSSHFileBrowser ("save");
            break;

        case CommandIDs::stopAudioGraph:
            getContentComponent()->stopAudioGraph();
            //getContentComponent()->getCurrentCodeEditor()->stopDebugMode();
            break;

        case CommandIDs::about:
            CabbageUtilities::showMessage (title, &tempLookAndFeel);
            break;

        case CommandIDs::showGraph:
            getContentComponent()->showGraph();
            break;

        case CommandIDs::contextHelp:
            getContentComponent()->launchHelpfile ("context");
            break;

        case CommandIDs::csoundHelp:
            getContentComponent()->launchHelpfile ("csound");
            break;

        case CommandIDs::cabbageHelp:
            getContentComponent()->launchHelpfile ("cabbage");
            break;

        case CommandIDs::editMode:
            getContentComponent()->setEditMode (isGUIEnabled = ! isGUIEnabled);

            if (isGUIEnabled == false)
                getContentComponent()->saveDocument();

            break;

        default:
            break;
    }

    return true;
}

const String CabbageDocumentWindow::getPluginId (File csdFile)
{
	StringArray csdLines;
	csdLines.addLines (csdFile.loadFileAsString());

	for (auto line : csdLines)
	{
		ValueTree temp ("temp");
		CabbageWidgetData::setWidgetState (temp, line, 0);

		if (CabbageWidgetData::getStringProp (temp, CabbageIdentifierIds::type) == CabbageWidgetTypes::form)
			return CabbageWidgetData::getStringProp (temp, CabbageIdentifierIds::pluginid);
	}

	return String::empty;
}