/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "CabbageMainDocumentWindow.h"
#include "CabbageApplication.h"
#include "Utilities/CabbageUtilities.h"
#include "CabbageNewProjectWindow.h"
#include "Audio/AudioGraph.h"

//==============================================================================
enum
{
    recentProjectsBaseID = 100,
    activeDocumentsBaseID = 300,
    colourSchemeBaseID = 1000
};
//==============================================================================
CabbageApplication::CabbageApplication()
{

}
//==============================================================================
void CabbageApplication::initialise (const String& commandLine)
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

	//filterGraph.

    cabbageSettings = new CabbageSettings();
    cabbageSettings->addChangeListener(this);
    cabbageSettings->setStorageParameters (options);
	

	
	
    //cabbageSettings->setDefaultColourSettings();
    cabbageSettings->setDefaultSettings();
    lookAndFeel.setColour(2, Colours::red);

    mainDocumentWindow = new CabbageMainDocumentWindow (getApplicationName(), cabbageSettings);
    mainDocumentWindow->setTitleBarButtonsRequired(DocumentWindow::allButtons, false);
    initialiseLogger ("IDE_Log_");
    Logger::writeToLog (SystemStats::getOperatingSystemName());
    Logger::writeToLog ("CPU: " + String (SystemStats::getCpuSpeedInMegaherz())
                        + "MHz  Cores: " + String (SystemStats::getNumCpus())
                        + "  " + String (SystemStats::getMemorySizeInMegabytes()) + "MB");

    lookAndFeel.refreshLookAndFeel(cabbageSettings->getValueTree());
    LookAndFeel::setDefaultLookAndFeel (&lookAndFeel);

    if (commandLine.isNotEmpty())
    {
        isRunningCommandLine = false;
    }

    if (sendCommandLineToPreexistingInstance())
    {
        DBG ("Another instance is running - quitting...");
        quit();
        return;
    }
	
	createAudioGraph();

    initCommandManager();
    menuModel = new MainMenuModel();
    mainDocumentWindow->setMenuBar(menuModel, 24);

#if JUCE_MAC
    MenuBarModel::setMacMainMenu (menuModel, nullptr, "Open Recent");
#endif
}

//==============================================================================
void CabbageApplication::changeListenerCallback(ChangeBroadcaster* source)
{
    if(CabbageSettings* settings = dynamic_cast<CabbageSettings*>(source))
    {
        lookAndFeel.refreshLookAndFeel(cabbageSettings->getValueTree());
        mainDocumentWindow->lookAndFeelChanged();
		mainDocumentWindow->updateEditorColourScheme();
		
    }
}

//==============================================================================
void CabbageApplication::initCommandManager()
{
    commandManager = new ApplicationCommandManager();
    commandManager->registerAllCommandsForTarget (this);
}

//==============================================================================
bool CabbageApplication::initialiseLogger (const char* filePrefix)
{
    if (logger == nullptr)
    {
#if JUCE_LINUX
        String folder = "~/.config/CabbageApplication/Logs";
#else
        String folder = "com.Cabbage.CabbageApplication";
#endif

        logger = FileLogger::createDateStampedLogger (folder, filePrefix, ".txt",
                 getApplicationName() + " " + getApplicationVersion()
                 + "  ---  Build date: " __DATE__);
        Logger::setCurrentLogger (logger);
    }

    return logger != nullptr;
}

//==============================================================================
void CabbageApplication::deleteLogger()
{
    Logger::setCurrentLogger (nullptr);
    logger = nullptr;
}

//==============================================================================
CabbageApplication& CabbageApplication::getApp()
{
    CabbageApplication* const app = dynamic_cast<CabbageApplication*> (JUCEApplication::getInstance());
    jassert (app != nullptr);
    return *app;
}

//==============================================================================
ApplicationCommandManager& CabbageApplication::getCommandManager()
{
    ApplicationCommandManager* cm = CabbageApplication::getApp().commandManager;
    jassert (cm != nullptr);
    return *cm;
}

//==============================================================================
PropertiesFile::Options CabbageApplication::getPropertyFileOptionsFor (const String& filename)
{
    PropertiesFile::Options options;
    options.applicationName     = filename;
    options.filenameSuffix      = "settings";
    options.osxLibrarySubFolder = "Application Support";
#if JUCE_LINUX
    options.folderName          = "~/.config/CabbageApplication";
#else
    options.folderName          = "CabbageApplication";
#endif

    return options;
}

//==============================================================================
MenuBarModel* CabbageApplication::getMenuModel()
{
    return menuModel.get();
}

//==============================================================================
StringArray CabbageApplication::getMenuNames()
{
    const char* const names[] = { "File", "Edit", "Tools", "View", nullptr };
    return StringArray (names);
}

//==============================================================================
void CabbageApplication::createMenu (PopupMenu& menu, const String& menuName)
{
    if (menuName == "File")             createFileMenu   (menu);
    else if (menuName == "Edit")        createEditMenu   (menu);
    else if (menuName == "View")        createViewMenu   (menu);
    else if (menuName == "Build")       createBuildMenu  (menu);
    else if (menuName == "Window")      createWindowMenu (menu);
    else if (menuName == "Tools")       createToolsMenu  (menu);
    else                                jassertfalse; // names have changed?
}

//==============================================================================
void CabbageApplication::createFileMenu (PopupMenu& menu)
{
    menu.addCommandItem (commandManager, CommandIDs::newProject);
    menu.addSeparator();
    menu.addCommandItem (commandManager, CommandIDs::open);

    PopupMenu recentFilesMenu;
	cabbageSettings->updateRecentFilesList();
    cabbageSettings->recentFiles.createPopupMenuItems (recentFilesMenu, recentProjectsBaseID, true, true);
    menu.addSubMenu ("Open Recent", recentFilesMenu);

    menu.addSeparator();
    menu.addCommandItem (commandManager, CommandIDs::closeDocument);
    menu.addCommandItem (commandManager, CommandIDs::saveDocument);
    menu.addCommandItem (commandManager, CommandIDs::saveDocumentAs);
    menu.addCommandItem (commandManager, CommandIDs::saveAll);
    menu.addSeparator();
    menu.addCommandItem (commandManager, CommandIDs::closeProject);
    menu.addCommandItem (commandManager, CommandIDs::saveProject);
    menu.addSeparator();
    menu.addCommandItem (commandManager, CommandIDs::settings);
    menu.addSeparator();

#if ! JUCE_MAC
    menu.addSeparator();
    menu.addCommandItem (commandManager, StandardApplicationCommandIDs::quit);
#endif
}

//==============================================================================
void CabbageApplication::createEditMenu (PopupMenu& menu)
{
	PopupMenu subMenu;
    menu.addCommandItem (commandManager, CommandIDs::undo);
    menu.addCommandItem (commandManager, CommandIDs::redo);
    menu.addSeparator();
    menu.addCommandItem (commandManager, CommandIDs::cut);
    menu.addCommandItem (commandManager, CommandIDs::copy);
    menu.addCommandItem (commandManager, CommandIDs::paste);
	menu.addSeparator();
	subMenu.addCommandItem(commandManager, CommandIDs::cabbageMode);
	subMenu.addCommandItem(commandManager, CommandIDs::genericMode);
	subMenu.addCommandItem(commandManager, CommandIDs::csoundMode);
	menu.addSubMenu("Interface Mode", subMenu, (getEditor()->isVisible()?true:false));
	menu.addSeparator();
    menu.addCommandItem (commandManager, CommandIDs::del);
    menu.addCommandItem (commandManager, CommandIDs::selectAll);
    menu.addCommandItem (commandManager, CommandIDs::deselectAll);
    menu.addSeparator();
	menu.addCommandItem (commandManager, CommandIDs::editMode);
	menu.addSeparator();
    menu.addCommandItem (commandManager, CommandIDs::showFindPanel);
    menu.addCommandItem (commandManager, CommandIDs::findSelection);
    menu.addCommandItem (commandManager, CommandIDs::findNext);
    menu.addCommandItem (commandManager, CommandIDs::findPrevious);
}

//==============================================================================
void CabbageApplication::createViewMenu (PopupMenu& menu)
{

    menu.addSeparator();
	menu.addCommandItem (commandManager, CommandIDs::showGenericWidgetWindow);
    createColourSchemeItems (menu);
}

//==============================================================================
void CabbageApplication::createBuildMenu (PopupMenu& menu)
{
    menu.addCommandItem (commandManager, CommandIDs::enableBuild);
    menu.addCommandItem (commandManager, CommandIDs::toggleContinuousBuild);
    menu.addCommandItem (commandManager, CommandIDs::buildNow);

    menu.addSeparator();
    menu.addCommandItem (commandManager, CommandIDs::launchApp);
    menu.addCommandItem (commandManager, CommandIDs::killApp);
    menu.addCommandItem (commandManager, CommandIDs::cleanAll);
    menu.addSeparator();
    menu.addCommandItem (commandManager, CommandIDs::reinstantiateComp);
    menu.addCommandItem (commandManager, CommandIDs::showWarnings);
    menu.addSeparator();
    menu.addCommandItem (commandManager, CommandIDs::nextError);
    menu.addCommandItem (commandManager, CommandIDs::prevError);
}

//==============================================================================
void CabbageApplication::createColourSchemeItems (PopupMenu& menu)
{

}

//==============================================================================
void CabbageApplication::createWindowMenu (PopupMenu& menu)
{
    menu.addCommandItem (commandManager, CommandIDs::closeWindow);
    menu.addSeparator();

    menu.addCommandItem (commandManager, CommandIDs::goToPreviousDoc);
    menu.addCommandItem (commandManager, CommandIDs::goToNextDoc);
    menu.addCommandItem (commandManager, CommandIDs::goToCounterpart);
    menu.addSeparator();
    menu.addSeparator();
    menu.addCommandItem (commandManager, CommandIDs::closeAllDocuments);
}

//==============================================================================
void CabbageApplication::createToolsMenu (PopupMenu& menu)
{
    menu.addCommandItem (commandManager, CommandIDs::runCode);
	menu.addCommandItem (commandManager, CommandIDs::stopCode);
    menu.addSeparator();
    menu.addCommandItem (commandManager, CommandIDs::exportAsSynth);
    menu.addCommandItem (commandManager, CommandIDs::exportAsEffect);
    menu.addCommandItem (commandManager, CommandIDs::exportAsFMODSoundPlugin);
    menu.addSeparator();
}

//==============================================================================
void CabbageApplication::handleMainMenuCommand (int menuItemID)
{
    if (menuItemID >= recentProjectsBaseID && menuItemID < recentProjectsBaseID + 100)
    {
        openFile (cabbageSettings->recentFiles.getFile (menuItemID - recentProjectsBaseID));
    }
}

//==============================================================================
void CabbageApplication::getAllCommands (Array <CommandID>& commands)
{
    JUCEApplication::getAllCommands (commands);

    const CommandID ids[] = { CommandIDs::newProject,
                              CommandIDs::open,
                              CommandIDs::closeAllDocuments,
                              CommandIDs::saveDocument,
							  CommandIDs::settings,
                              CommandIDs::runCode,
							  CommandIDs::stopCode,							  
							  CommandIDs::exportAsSynth,
							  CommandIDs::exportAsEffect,
							  CommandIDs::exportAsFMODSoundPlugin,
							  CommandIDs::copy,
							  CommandIDs::cut,
							  CommandIDs::paste,
							  CommandIDs::undo,
							  CommandIDs::redo,
							  CommandIDs::editMode,
							  //CommandIDs::selectAll,
							  //CommandIDs::del,
							  //CommandIDs::findNext,
							  //CommandIDs::findPrevious,
							  CommandIDs::genericMode,
							  CommandIDs::csoundMode,
							  CommandIDs::cabbageMode,
							  CommandIDs::showGenericWidgetWindow
                            };

    commands.addArray (ids, numElementsInArray (ids));
}

//==============================================================================
void CabbageApplication::getCommandInfo (CommandID commandID, ApplicationCommandInfo& result)
{
    switch (commandID)
    {
    case CommandIDs::newProject:
        result.setInfo ("New Project...", "Creates a new Jucer project", CommandCategories::general, 0);
        result.defaultKeypresses.add (KeyPress ('n', ModifierKeys::commandModifier, 0));
        break;

    case CommandIDs::open:
        result.setInfo ("Open...", "Opens a Jucer project", CommandCategories::general, 0);
        result.defaultKeypresses.add (KeyPress ('o', ModifierKeys::commandModifier, 0));
        break;
	
    case CommandIDs::saveDocument:
        result.setInfo ("Save file...", "Save a document", CommandCategories::general, 0);
        result.defaultKeypresses.add (KeyPress ('s', ModifierKeys::commandModifier, 0));
        break;
	
    case CommandIDs::settings:
        result.setInfo ("Settings", "Change Cabbage settings", CommandCategories::general, 0);
        break;

    case CommandIDs::runCode:
        result.setInfo ("Compile", "Starts Csound and runs code", CommandCategories::general, 0);
		result.defaultKeypresses.add(KeyPress(KeyPress::F5Key, ModifierKeys::noModifiers, 0));
        break;

    case CommandIDs::stopCode:
        result.setInfo ("Cancel Compile", "Starts Csound and runs code", CommandCategories::general, 0);
		result.defaultKeypresses.add(KeyPress(KeyPress::escapeKey, ModifierKeys::noModifiers, 0));
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
//        result.setActive (openDocumentManager.getNumOpenDocuments() > 0);
        break;

    case CommandIDs::saveAll:
        result.setInfo ("Save All", "Saves all open documents", CommandCategories::general, 0);
        result.defaultKeypresses.add (KeyPress ('s', ModifierKeys::commandModifier | ModifierKeys::altModifier, 0));
        break;


   //edit commands
    case CommandIDs::undo:
        result.setInfo (String("Undo"), String("Undo last action"), CommandCategories::edit, 0);
        result.addDefaultKeypress ('z', ModifierKeys::commandModifier);
		result.setActive((getEditor()->isVisible() ? true : false));
        break;
    case CommandIDs::redo:
        result.setInfo (String("Redo"), String("Redo last action"), CommandCategories::edit, 0);
        result.addDefaultKeypress ('z', ModifierKeys::shiftModifier | ModifierKeys::commandModifier);
		result.setActive((getEditor()->isVisible() ? true : false));
        break;
    case CommandIDs::cut:
        result.setInfo (String("Cut"), String("Cut selection"), CommandCategories::edit, 0);
        result.addDefaultKeypress ('x', ModifierKeys::commandModifier);
		result.setActive((getEditor()->isVisible() ? true : false));
        break;
    case CommandIDs::copy:
        result.setInfo (String("Copy"), String("Copy selection"), CommandCategories::edit, 0);
        result.addDefaultKeypress ('c', ModifierKeys::commandModifier);
		result.setActive((getEditor()->isVisible() ? true : false));
        break;
    case CommandIDs::paste:
        result.setInfo (String("Paste"), String("Paste selection"), CommandCategories::edit, 0);
        result.addDefaultKeypress ('v', ModifierKeys::commandModifier);
		result.setActive((getEditor()->isVisible() ? true : false));
        break;
    case CommandIDs::columnEdit:
        result.setInfo (String("Column Edit mode"), String("Column Edit"), CommandCategories::edit, 0);
        //result.setTicked(isColumnModeEnabled);
        result.addDefaultKeypress ('l', ModifierKeys::commandModifier);
		result.setActive((getEditor()->isVisible() ? true : false));
        break;
    case CommandIDs::toggleComments:
        result.setInfo (String("Toggle comments"), String("Toggle comments"), CommandCategories::edit, 0);
        result.addDefaultKeypress ('/', ModifierKeys::commandModifier);
		result.setActive((getEditor()->isVisible() ? true : false));
        break;
    case CommandIDs::searchReplace:
        result.setInfo(String("Search or Replace"), String("Search Replace"), CommandCategories::edit, 0);
        result.addDefaultKeypress ('f', ModifierKeys::commandModifier);
		result.setActive((getEditor()->isVisible() ? true : false));
        break;
    case CommandIDs::zoomIn:
        result.setInfo (String("Zoom in"), String("Zoom in"), CommandCategories::edit, 0);
        result.addDefaultKeypress('[', ModifierKeys::commandModifier);
		result.setActive((getEditor()->isVisible() ? true : false));
        break;
    case CommandIDs::zoomOut:
        result.setInfo (String("Zoom out"), String("Zoom out"), CommandCategories::edit, 0);
        result.addDefaultKeypress (']', ModifierKeys::commandModifier);
		result.setActive((getEditor()->isVisible() ? true : false));
        break;
    case CommandIDs::csoundMode:
        result.setInfo (String("Csound mode"), String("Csound only mode"), CommandCategories::edit, 0);
        result.setTicked(getCurrentInterfaceMode()==CabbageInterfaceModes::csound);
		result.setActive((getEditor()->isVisible() ? true : false));
        break;
    case CommandIDs::genericMode:
        result.setInfo (String("Generic mode"), String("Generic interface mode"), CommandCategories::edit, 0);
        result.setTicked(getCurrentInterfaceMode()==CabbageInterfaceModes::generic);
		result.setActive((getEditor()->isVisible() ? true : false));
        break;
    case CommandIDs::cabbageMode:
        result.setInfo (String("Cabbage mode"), String("Normal Cabbage mode"), CommandCategories::edit, 0);
        result.setTicked(getCurrentInterfaceMode()==CabbageInterfaceModes::cabbage);
		result.setActive((getEditor()->isVisible() ? true : false));
        break;
    case CommandIDs::showGenericWidgetWindow:
        result.setInfo (String("Show Generic Widget Window"), String("Show genric channel based widgets"), CommandCategories::general, 0);
		//result.setActive((getEditor()->isVisible() ? true : false));
        break;
    case CommandIDs::editMode:
        result.setInfo (String("Edit Mode"), String("Edit Mode"), CommandCategories::edit, 0);
        result.addDefaultKeypress ('e', ModifierKeys::commandModifier);
		break;
    default:
        JUCEApplication::getCommandInfo (commandID, result);
        break;
    }
}

//==============================================================================
bool CabbageApplication::perform (const InvocationInfo& info)
{
    switch (info.commandID)
    {
    case CommandIDs::newProject:
        createNewProject();
        break;
    case CommandIDs::open:
        askUserToOpenFile();
        break;
    case CommandIDs::saveDocument:
		saveDocument();
        break;
    case CommandIDs::closeAllDocuments:
        closeAllDocuments (true);
        break;
	case CommandIDs::settings:
		showSettingsDialog();
		break;
    case CommandIDs::runCode:
        runCode();
        break;
    case CommandIDs::stopCode:
        stopCode();
        break;
    case CommandIDs::exportAsEffect:
        
        break;
    case CommandIDs::exportAsSynth:
        
        break;
    case CommandIDs::exportAsFMODSoundPlugin:
        
        break;
	case CommandIDs::undo:
        
        break;
    case CommandIDs::redo:
        
        break;
    case CommandIDs::paste:
        
        break;
    case CommandIDs::editMode:
		isGUIEnabled=!isGUIEnabled;
        enableEditMode(isGUIEnabled);
        break;
    case CommandIDs::genericMode:
        setCurrentInterfaceMode(CabbageInterfaceModes::generic);
        break;
    case CommandIDs::csoundMode:
        setCurrentInterfaceMode(CabbageInterfaceModes::csound);
        break;
    case CommandIDs::cabbageMode:
        setCurrentInterfaceMode(CabbageInterfaceModes::cabbage);
        break;
    case CommandIDs::showGenericWidgetWindow:
        //showGenericWidgetWindow(true);
        break;
    default:
        return JUCEApplication::perform (info);
    }

    return true;
}

Identifier CabbageApplication::getCurrentInterfaceMode()
{
	return currentInterfaceMode;
}

void CabbageApplication::enableEditMode(bool enable)
{
	if (AudioProcessorGraph::Node::Ptr f = audioGraph->graph.getNodeForId (1))
	{
		AudioProcessor* const processor = f->getProcessor();
		//need to check what kind of processor we are dealing with!
		CabbagePluginEditor* editor = dynamic_cast<CabbagePluginEditor*>(processor->getActiveEditor());
		if(editor)
		{
			editor->enableGUIEditor(enable);
		}
	}
}

void CabbageApplication::setCurrentInterfaceMode(Identifier mode)
{
	currentInterfaceMode=mode;
}

void CabbageApplication::showSettingsDialog()
{
	DialogWindow::LaunchOptions o;
    o.content.setOwned(new CabbageSettingsWindow(cabbageSettings->getValueTree(), audioGraph->getAudioDeviceSelector()));
    o.content->setSize(500, 450);
    o.dialogTitle = TRANS("Cabbage Settings");
    o.dialogBackgroundColour = Colour(0xfff0f0f0);
    o.escapeKeyTriggersCloseButton = true;
    o.useNativeTitleBar = true;
    o.resizable = false;
    o.launchAsync();
    
}
//==============================================================================
CodeEditorComponent* CabbageApplication::getEditor()
{
	return mainDocumentWindow->getMainContentComponent()->editor;
}
//==============================================================================
CabbageOutputConsole* CabbageApplication::getOutputConsole()
{
	return mainDocumentWindow->getMainContentComponent()->outputConsole;
}
//==============================================================================
void CabbageApplication::createNewProject()
{
    Logger::writeToLog("CreateNewProject");

    //cabbageSettings->set("Colours", ColourIds::popupMenuBackground, Colours::red.toString());

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
void CabbageApplication::askUserToOpenFile()
{
    FileChooser fc ("Open File");

    if (fc.browseForFileToOpen())
        openFile (fc.getResult());
}
//==============================================================================
bool CabbageApplication::openFile (const File& file, String type)
{
	stopTimer();
	stopCode();

	PluginWindow::closeAllCurrentlyOpenWindows();

    mainDocumentWindow->getMainContentComponent()->openFile(file);
	cabbageSettings->updateRecentFilesList(file);
	mainDocumentWindow->addKeyListener(getCommandManager().getKeyMappings());
	mainDocumentWindow->setName("Cabbage " + file.getFullPathName());
	currentCsdFile = File(file.getFullPathName());	
    return true;
}
//==============================================================================
void CabbageApplication::saveDocument()
{
	if(currentCsdFile.existsAsFile())
		currentCsdFile.replaceWithText(getEditor()->getDocument().getAllContent());
}

void CabbageApplication::timerCallback()
{
    if(currentCsdFile.existsAsFile())
    {        
        const String csoundOutputString = audioGraph->getCsoundOutput();
        consoleMessages+=csoundOutputString;
        if(csoundOutputString.length()>0)
        {
            getOutputConsole()->setText(csoundOutputString);
        }
    }	
}
//==============================================================================
void CabbageApplication::runCode()
{
	if(currentCsdFile.existsAsFile())
	{
		PluginWindow::closeAllCurrentlyOpenWindows();
		//this is overkill, in future we can simple edit the node in question and leave the graph
		createAudioGraph();
		startTimer(100);
	}
	else
		CabbageUtilities::showMessage("Warning", "Please open a file first", &lookAndFeel);
}
//==============================================================================
void CabbageApplication::stopCode()
{
	if(currentCsdFile.existsAsFile())
	{
		stopTimer();
		audioGraph->stopPlaying();
	}
}

//==============================================================================
void CabbageApplication::createAudioGraph()
{
	//pluginWindow = nullptr;
	audioGraph = new AudioGraph(cabbageSettings->getUserSettings(), currentCsdFile, false);
	audioGraph->setXmlAudioSettings(cabbageSettings->getUserSettings()->getXmlValue("audioSetup"));
	
	createEditorForAudioGraphNode();


	//pluginWindow->setVisible( numParameters>0 ? show : false);
	CabbageUtilities::debug("finished setting up");
}

void CabbageApplication::createEditorForAudioGraphNode()
{
	//const bool numParameters = audioGraph->getNumberOfParameters();
	//if(numParameters>0)
		if (AudioProcessorGraph::Node::Ptr f = audioGraph->graph.getNodeForId (1))
		{
			AudioProcessor* const processor = f->getProcessor();
		
			PluginWindow::WindowFormatType type = audioGraph->getProcessor()->hasEditor() ? PluginWindow::Normal
																	 : PluginWindow::Generic;

			if (PluginWindow* const w = PluginWindow::getWindowFor(f, type, audioGraph->graph))
			w->toFront (true);
		}
		
}
//==============================================================================
void CabbageApplication::newFile (String type)
{

}

bool CabbageApplication::closeAllDocuments (bool askUserToSave)
{
    // return openDocumentManager.closeAll (askUserToSave);
    return true;
}

bool CabbageApplication::closeAllMainWindows()
{
    //return server != nullptr || mainWindowList.askAllWindowsToClose();
    return true;
}
//==============================================================================
void CabbageApplication::shutdown()
{
	if(audioGraph->getDeviceManagerSettings().isNotEmpty())
		cabbageSettings->setProperty("audioSetup", audioGraph->getDeviceManagerSettings());	
		
    mainDocumentWindow->setMenuBar(nullptr);
#if JUCE_MAC
    MenuBarModel::setMacMainMenu (nullptr);
#endif
    menuModel = nullptr;
    commandManager = nullptr;
	cabbageSettings->closeFiles();
	cabbageSettings = nullptr;

	audioGraph = nullptr;
	//pluginWrapper->deletePlugin();
	//pluginWrapper = nullptr;	
    LookAndFeel::setDefaultLookAndFeel (nullptr);

    if (! isRunningCommandLine)
        Logger::writeToLog ("Shutdown");

    deleteLogger();
}
//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (CabbageApplication)
