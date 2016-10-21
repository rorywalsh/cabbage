/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "CabbageMainDocumentWindow.h"
#include "CabbageApplication.h"
#include "../Utilities/CabbageUtilities.h"
#include "CabbageNewProjectWindow.h"

//#include "CabbageSettings.cpp"
//#include "CabbageAppearanceSettings.cpp"
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
	

	createGenericCsoundPluginWrapper();


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
	else if(CabbagePluginWrapper* pluginWrapper = dynamic_cast<CabbagePluginWrapper*>(source))
    {

    }
}

//==============================================================================
void CabbageApplication::initCommandManager()
{
    commandManager = new ApplicationCommandManager();
    commandManager->registerAllCommandsForTarget (this);

//    {
//        CodeDocument doc;
//        CppCodeEditorComponent ed (File(), doc);
//        commandManager->registerAllCommandsForTarget (&ed);
//    }

//    registerGUIEditorCommands();
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
    menu.addCommandItem (commandManager, StandardApplicationCommandIDs::undo);
    menu.addCommandItem (commandManager, StandardApplicationCommandIDs::redo);
    menu.addSeparator();
    menu.addCommandItem (commandManager, StandardApplicationCommandIDs::cut);
    menu.addCommandItem (commandManager, StandardApplicationCommandIDs::copy);
    menu.addCommandItem (commandManager, StandardApplicationCommandIDs::paste);
    menu.addCommandItem (commandManager, StandardApplicationCommandIDs::del);
    menu.addCommandItem (commandManager, StandardApplicationCommandIDs::selectAll);
    menu.addCommandItem (commandManager, StandardApplicationCommandIDs::deselectAll);
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
//   const StringArray presetSchemes (settings->appearance.getPresetSchemes());
//
//    if (presetSchemes.size() > 0)
//    {
//        PopupMenu schemes;
//
//        for (int i = 0; i < presetSchemes.size(); ++i)
//            schemes.addItem (colourSchemeBaseID + i, presetSchemes[i]);
//
//        menu.addSubMenu ("Colour Scheme", schemes);
//    }
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

//    const int numDocs = jmin (50, openDocumentManager.getNumOpenDocuments());
//
//    for (int i = 0; i < numDocs; ++i)
//    {
//        OpenDocumentManager::Document* doc = openDocumentManager.getOpenDocument(i);
//        menu.addItem (activeDocumentsBaseID + i, doc->getName());
//    }

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
        // open a file from the "recent files" menu
        openFile (cabbageSettings->recentFiles.getFile (menuItemID - recentProjectsBaseID));
    }
    else if (menuItemID >= activeDocumentsBaseID && menuItemID < activeDocumentsBaseID + 200)
    {
//        if (OpenDocumentManager::Document* doc = openDocumentManager.getOpenDocument (menuItemID - activeDocumentsBaseID))
//            mainWindowList.openDocument (doc, true);
//        else
//            jassertfalse;
    }
    else if (menuItemID >= colourSchemeBaseID && menuItemID < colourSchemeBaseID + 200)
    {
//        settings->appearance.selectPresetScheme (menuItemID - colourSchemeBaseID);
    }
    else
    {
//        handleGUIEditorMenuCommand (menuItemID);
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
							  CommandIDs::exportAsFMODSoundPlugin
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
        int test2;
        break;
    case CommandIDs::exportAsSynth:
        int test3;
        break;
    case CommandIDs::exportAsFMODSoundPlugin:
        int test4;
        break;
    default:
        return JUCEApplication::perform (info);
    }

    return true;
}
//==============================================================================
void CabbageApplication::showSettingsDialog()
{
	DialogWindow::LaunchOptions o;
    o.content.setOwned(new CabbageSettingsWindow(cabbageSettings->getValueTree(), pluginWrapper->getAudioDeviceSelector()));
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

	
    /*
    DialogWindow::LaunchOptions o;
    o.content.setOwned(new CabbageSettingsWindow(cabbageSettings->getValueTree()));
    o.content->setSize(500, 450);

    o.dialogTitle = TRANS("Cabbage Settings");
    o.dialogBackgroundColour = Colour(0xfff0f0f0);
    o.escapeKeyTriggersCloseButton = true;
    o.useNativeTitleBar = true;
    o.resizable = false;

    o.launchAsync();
    */

//	XmlElement* el = cabbageSettings->getValueTree().createXml();
//	ValueTree values = ValueTree::fromXml(*el);
//	String test = values.getChildWithName("Colours").getProperty("MainBackground").toString();
//	delete el;
//
    //MainWindow* mw = mainWindowList.getOrCreateEmptyWindow();
    //mw->showNewProjectWizard();
    //mainWindowList.avoidSuperimposedWindows (mw);
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
	//CabbageUtilities::debug(getEditor()->getDocument().getAllContent());
	currentCsdFile.replaceWithText(getEditor()->getDocument().getAllContent());
}
//==============================================================================
void CabbageApplication::createGenericCsoundPluginWrapper()
{
	if(!pluginWrapper)
	{
		pluginWrapper = new CabbagePluginWrapper(cabbageSettings->getValueTree(), false);
		pluginWrapper->setXmlAudioSettings(cabbageSettings->getUserSettings()->getXmlValue("audioSetup"));
		if(pluginWrapper->isAudioDeviceOk()==false)
            CabbageUtilities::showMessage("Warning", "Cabbage could not initialise the selected audio device. Please select a valid audio device in Audio Settings", &lookAndFeel);
	}
}

void CabbageApplication::timerCallback()
{
    if(currentCsdFile.existsAsFile())
    {        
        const String csoundOutputString = pluginWrapper->getCsoundOutput();
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
		startTimer(100);
		createGenericCsoundPluginWrapper();
		pluginWrapper->restartPlugin(currentCsdFile);
		
		bool shouldLaunchEditor = true;
		if(shouldLaunchEditor)
		{ 
			if(pluginWindow==nullptr)
			{
				pluginWindow = new PluginWindow("Plugin Interface", CabbageSettings::getColourFromValueTree(cabbageSettings->getValueTree(), CabbageColourIds::genericPluginInterface, Colours::grey));
				pluginWindow->setVisible(true);
			}
			
			pluginWindow->setContentOwned(pluginWrapper->getProcessor()->createEditorIfNeeded(), true);
			
//			o.content->setSize(500, 500);
//			o.dialogTitle = TRANS("Plugin Interface");
//			o.dialogBackgroundColour = Colour(0xfff0f0f0);
//			o.escapeKeyTriggersCloseButton = true;
//			o.useNativeTitleBar = true;
//			o.resizable = false;
		}	

		//pluginWrapper->createPlugin(currentCsdFile);
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
		pluginWrapper->stopPlaying();
	}
}
//==============================================================================
void CabbageApplication::newFile (String type)
{

}
//==============================================================================
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
	
	//CabbageUtilities::debug(pluginWrapper->getDeviceManagerSettings());
	cabbageSettings->setProperty("audioSetup", pluginWrapper->getDeviceManagerSettings());
	
	
    mainDocumentWindow->setMenuBar(nullptr);
#if JUCE_MAC
    MenuBarModel::setMacMainMenu (nullptr);
#endif
    menuModel = nullptr;
    commandManager = nullptr;
    appearanceEditorWindow = nullptr;
    globalPreferencesWindow = nullptr;
	pluginWrapper = nullptr;
    cabbageSettings->closeFiles();
	cabbageSettings = nullptr;
    LookAndFeel::setDefaultLookAndFeel (nullptr);

    if (! isRunningCommandLine)
        Logger::writeToLog ("Shutdown");

    deleteLogger();
}
//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (CabbageApplication)
