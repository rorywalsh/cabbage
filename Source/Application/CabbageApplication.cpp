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

    cabbageMainDocumentWindow = new CabbageMainDocumentWindow (getApplicationName(), cabbageSettings->getValueTree());
    cabbageMainDocumentWindow->setTitleBarButtonsRequired(DocumentWindow::allButtons, false);
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


    initCommandManager();
    menuModel = new MainMenuModel();
    cabbageMainDocumentWindow->setMenuBar(menuModel, 24);

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
        cabbageMainDocumentWindow->lookAndFeelChanged();
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
    const char* const names[] = { "File", "Edit", "View", "Build", "Window", "Tools", nullptr };
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

    PopupMenu recentFiles;
//    settings->recentFiles.createPopupMenuItems (recentFiles, recentProjectsBaseID, true, true);
//    menu.addSubMenu ("Open Recent", recentFiles);

    menu.addSeparator();
    menu.addCommandItem (commandManager, CommandIDs::closeDocument);
    menu.addCommandItem (commandManager, CommandIDs::saveDocument);
    menu.addCommandItem (commandManager, CommandIDs::saveDocumentAs);
    menu.addCommandItem (commandManager, CommandIDs::saveAll);
    menu.addSeparator();
    menu.addCommandItem (commandManager, CommandIDs::closeProject);
    menu.addCommandItem (commandManager, CommandIDs::saveProject);
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
    menu.addCommandItem (commandManager, CommandIDs::showFilePanel);
    menu.addCommandItem (commandManager, CommandIDs::showConfigPanel);
    menu.addCommandItem (commandManager, CommandIDs::showBuildTab);
    menu.addCommandItem (commandManager, CommandIDs::showProjectSettings);
    menu.addCommandItem (commandManager, CommandIDs::showProjectModules);
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
    menu.addCommandItem (commandManager, CommandIDs::showGlobalPreferences);
    menu.addSeparator();
    menu.addCommandItem (commandManager, CommandIDs::showUTF8Tool);
    menu.addCommandItem (commandManager, CommandIDs::showSVGPathTool);
    menu.addCommandItem (commandManager, CommandIDs::showTranslationTool);
    menu.addSeparator();
    menu.addCommandItem (commandManager, CommandIDs::loginLogout);
}

//==============================================================================
void CabbageApplication::handleMainMenuCommand (int menuItemID)
{
    if (menuItemID >= recentProjectsBaseID && menuItemID < recentProjectsBaseID + 100)
    {
        // open a file from the "recent files" menu
//        openFile (settings->recentFiles.getFile (menuItemID - recentProjectsBaseID));
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
                              CommandIDs::saveAll,
                              CommandIDs::showGlobalPreferences
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

    case CommandIDs::showGlobalPreferences:
        result.setInfo ("Global Preferences...", "Shows the global preferences window.", CommandCategories::general, 0);
        break;

    case CommandIDs::closeAllDocuments:
        result.setInfo ("Close All Documents", "Closes all open documents", CommandCategories::general, 0);
//        result.setActive (openDocumentManager.getNumOpenDocuments() > 0);
        break;

    case CommandIDs::saveAll:
        result.setInfo ("Save All", "Saves all open documents", CommandCategories::general, 0);
        result.defaultKeypresses.add (KeyPress ('s', ModifierKeys::commandModifier | ModifierKeys::altModifier, 0));
        break;

    case CommandIDs::showUTF8Tool:
        result.setInfo ("UTF-8 String-Literal Helper", "Shows the UTF-8 string literal utility", CommandCategories::general, 0);
        break;

    case CommandIDs::showSVGPathTool:
        result.setInfo ("SVG Path Helper", "Shows the SVG->Path data conversion utility", CommandCategories::general, 0);
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
    case CommandIDs::saveAll:
        break;
    case CommandIDs::closeAllDocuments:
        closeAllDocuments (true);
        break;
    case CommandIDs::showGlobalPreferences:
        int i;
        break;
    default:
        return JUCEApplication::perform (info);
    }

    return true;
}

//==============================================================================
void CabbageApplication::createNewProject()
{
    Logger::writeToLog("CreateNewProject");

    //cabbageSettings->set("Colours", ColourIds::popupMenuBackground, Colours::red.toString());

    DialogWindow::LaunchOptions o;
    o.content.setOwned(new CabbageProjectWindow(cabbageSettings->getValueTree()));
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

void CabbageApplication::askUserToOpenFile()
{
    FileChooser fc ("Open File");

    if (fc.browseForFileToOpen())
        openFile (fc.getResult());
}

bool CabbageApplication::openFile (const File& file)
{
    //return mainWindowList.openFile (file);
    return true;
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
    cabbageMainDocumentWindow->setMenuBar(nullptr);

#if JUCE_MAC
    MenuBarModel::setMacMainMenu (nullptr);
#endif

    menuModel = nullptr;
    commandManager = nullptr;
    cabbageSettings->closeFiles();
    cabbageSettings = nullptr;
    appearanceEditorWindow = nullptr;
    globalPreferencesWindow = nullptr;

    LookAndFeel::setDefaultLookAndFeel (nullptr);

    if (! isRunningCommandLine)
        Logger::writeToLog ("Shutdown");

    deleteLogger();
}
//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (CabbageApplication)
