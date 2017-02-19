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
#include <fstream>

enum
{
    recentProjectsBaseID = 100,
    activeDocumentsBaseID = 300,
    examplesMenuBaseID = 3000,
    colourSchemeBaseID = 1000
};

//=================================================================================================================
CabbageDocumentWindow::CabbageDocumentWindow (String name)  : DocumentWindow (name,
                                                                                  Colours::lightgrey,
                                                                                  DocumentWindow::allButtons)
{
    setTitleBarButtonsRequired (DocumentWindow::allButtons, false);
    setUsingNativeTitleBar (true);
    setResizable (true, true);
    centreWithSize (getWidth(), getHeight());
    setVisible (true);


    initSettings();
    setContentOwned (content = new CabbageContentComponent (this, cabbageSettings), true);
    content->propertyPanel->setVisible (false);
    cabbageSettings->addChangeListener (content);
    setMenuBar (this, 25);
    getMenuBarComponent()->setLookAndFeel (getContentComponent()->lookAndFeel);


    if (cabbageSettings->getUserSettings()->getIntValue ("OpenMostRecentFileOnStartup") == 1)
    {
        cabbageSettings->updateRecentFilesList();
        content->openFile (cabbageSettings->getMostRecentFile().getFullPathName());
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

    if (getContentComponent()->getCurrentCodeEditor())
        cabbageSettings->setProperty ("IDE_StatusBarPos", getContentComponent()->getStatusbarYPos());

    cabbageSettings->setProperty ("audioSetup", getContentComponent()->getAudioDeviceSettings());
    cabbageSettings->closeFiles();

    //lookAndFeel = nullptr;

}
//=======================================================================================
CabbageContentComponent* CabbageDocumentWindow::getContentComponent()
{
    return content;
}

void CabbageDocumentWindow::maximiseButtonPressed()
{
    getContentComponent()->resizeAllEditorAndConsoles (getHeight());
}

void CabbageDocumentWindow::closeButtonPressed()
{
    CabbageIDELookAndFeel lookAndFeel;

    if (getContentComponent()->getAudioGraph()->hasChangedSinceSaved())
    {
        const int result = CabbageUtilities::showYesNoMessage ("Save changes made to Cabbage\npatch?", &lookAndFeel, 1);

        if (result == 0)
        {
            if (getContentComponent()->getAudioGraph()->saveGraph() == FileBasedDocument::SaveResult::userCancelledSave)
                return;
        }
        else if (result == 1)
            JUCEApplicationBase::quit();
        else
            return;
    }

    JUCEApplicationBase::quit();

}

StringArray CabbageDocumentWindow::getMenuBarNames()
{
    const char* const names[] = { "File", "Edit", "Tools", "View", nullptr };
    return StringArray (names);
}


PopupMenu CabbageDocumentWindow::getMenuForIndex (int topLevelMenuIndex, const String& menuName)
{
    PopupMenu menu;

    if (menuName == "File")             createFileMenu   (menu);
    else if (menuName == "Edit")        createEditMenu   (menu);
    else if (menuName == "View")        createViewMenu   (menu);
    else if (menuName == "Build")       createBuildMenu  (menu);
    else if (menuName == "Window")      createWindowMenu (menu);
    else if (menuName == "Tools")       createToolsMenu  (menu);
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
    CabbageUtilities::addFilesToPopupMenu (examplesMenu, exampleFiles, examplesDir, "*.csd", examplesMenuBaseID);

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

    menu.addCommandItem (&commandManager, CommandIDs::showFindPanel);
    menu.addCommandItem (&commandManager, CommandIDs::showReplacePanel);

    menu.addCommandItem (&commandManager, CommandIDs::findSelection);
    menu.addCommandItem (&commandManager, CommandIDs::findNext);
    menu.addCommandItem (&commandManager, CommandIDs::findPrevious);
    menu.addSeparator();
    menu.addCommandItem (&commandManager, CommandIDs::settings);

}

void CabbageDocumentWindow::createViewMenu (PopupMenu& menu)
{
    menu.addSeparator();
    menu.addCommandItem (&commandManager, CommandIDs::about);
    menu.addCommandItem (&commandManager, CommandIDs::showGraph);
}

void CabbageDocumentWindow::createBuildMenu (PopupMenu& menu)
{
    menu.addCommandItem (&commandManager, CommandIDs::enableBuild);
    menu.addCommandItem (&commandManager, CommandIDs::toggleContinuousBuild);
    menu.addCommandItem (&commandManager, CommandIDs::buildNow);
    menu.addSeparator();
    menu.addCommandItem (&commandManager, CommandIDs::launchApp);
    menu.addCommandItem (&commandManager, CommandIDs::killApp);
    menu.addCommandItem (&commandManager, CommandIDs::cleanAll);
    menu.addSeparator();
    menu.addCommandItem (&commandManager, CommandIDs::reinstantiateComp);
    menu.addCommandItem (&commandManager, CommandIDs::showWarnings);
    menu.addSeparator();
    menu.addCommandItem (&commandManager, CommandIDs::nextError);
    menu.addCommandItem (&commandManager, CommandIDs::prevError);
}


void CabbageDocumentWindow::createWindowMenu (PopupMenu& menu)
{
    menu.addCommandItem (&commandManager, CommandIDs::closeWindow);
    menu.addSeparator();
    menu.addCommandItem (&commandManager, CommandIDs::goToPreviousDoc);
    menu.addCommandItem (&commandManager, CommandIDs::goToNextDoc);
    menu.addCommandItem (&commandManager, CommandIDs::goToCounterpart);
    menu.addSeparator();
    menu.addSeparator();
    menu.addCommandItem (&commandManager, CommandIDs::closeAllDocuments);
}

void CabbageDocumentWindow::createToolsMenu (PopupMenu& menu)
{
    menu.addCommandItem (&commandManager, CommandIDs::startAudioGraph);
    menu.addCommandItem (&commandManager, CommandIDs::stopAudioGraph);
    menu.addSeparator();
    menu.addCommandItem (&commandManager, CommandIDs::exportAsSynth);
    menu.addCommandItem (&commandManager, CommandIDs::exportAsEffect);
    menu.addCommandItem (&commandManager, CommandIDs::exportAsFMODSoundPlugin);
    menu.addSeparator();
}


void CabbageDocumentWindow::menuItemSelected (int menuItemID, int topLevelMenuIndex)
{
    if (menuItemID >= recentProjectsBaseID && menuItemID < recentProjectsBaseID + 100)
    {
        const File file = cabbageSettings->recentFiles.getFile (menuItemID - recentProjectsBaseID).getFullPathName();

        if (file.hasFileExtension (".csd"))
            getContentComponent()->openFile (cabbageSettings->recentFiles.getFile (menuItemID - recentProjectsBaseID).getFullPathName());

        if (file.hasFileExtension (".cabbage"))
            getContentComponent()->openGraph (cabbageSettings->recentFiles.getFile (menuItemID - recentProjectsBaseID));
    }
    else if (menuItemID >= examplesMenuBaseID && menuItemID < exampleFiles.size() + examplesMenuBaseID)
    {
        //CabbageUtilities::debug(exampleFiles[menuItemID-examplesMenuBaseID].getFullPathName());
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
                              CommandIDs::saveGraph,
                              CommandIDs::saveGraphAs,
                              CommandIDs::saveDocumentToRPi,
                              CommandIDs::saveDocumentAs,
                              CommandIDs::examples,
                              CommandIDs::settings,
                              CommandIDs::startAudioGraph,
                              CommandIDs::stopAudioGraph,
                              CommandIDs::exportAsSynth,
                              CommandIDs::exportAsEffect,
                              CommandIDs::exportAsFMODSoundPlugin,
                              CommandIDs::copy,
                              CommandIDs::cut,
                              CommandIDs::toggleComments,
                              CommandIDs::zoomIn,
                              CommandIDs::zoomOut,
                              CommandIDs::paste,
                              CommandIDs::undo,
                              CommandIDs::showFindPanel,
                              CommandIDs::showReplacePanel,
                              CommandIDs::redo,
                              CommandIDs::editMode,
                              CommandIDs::showGraph,
                              CommandIDs::about,
                              CommandIDs::startLiveDebugger,
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
            result.setInfo ("Save Csound file", "Save a document", CommandCategories::general, 0);
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
            result.setInfo ("Save Csound as...", "Save a document", CommandCategories::general, 0);
            result.defaultKeypresses.add (KeyPress ('s', ModifierKeys::shiftModifier | ModifierKeys::commandModifier, 0));
            break;

        case CommandIDs::settings:
            result.setInfo ("Settings", "Change Cabbage settings", CommandCategories::general, 0);
            break;

        case CommandIDs::startAudioGraph:
            result.setInfo ("Start graph", "Starts the audio signal graph", CommandCategories::general, 0);
            result.defaultKeypresses.add (KeyPress (KeyPress::F4Key, ModifierKeys::noModifiers, 0));
            break;

        case CommandIDs::stopAudioGraph:
            result.setInfo ("Stop graph", "Stop the audio signal graph", CommandCategories::general, 0);
            result.defaultKeypresses.add (KeyPress (KeyPress::F5Key, ModifierKeys::noModifiers, 0));
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

        case CommandIDs::startLiveDebugger:
            result.setInfo (String ("Enable Live Debugger"), String ("Enable Live Debugger"), CommandCategories::edit, 0);
            result.addDefaultKeypress ('d', ModifierKeys::commandModifier);
            result.setTicked (getContentComponent()->getCurrentCodeEditor() == nullptr ? false : getContentComponent()->getCurrentCodeEditor()->isDebugModeEnabled());
            result.setActive ((shouldShowEditMenu ? true : false));
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

        case CommandIDs::settings:
            getContentComponent()->showSettingsDialog();
            return true;

        case CommandIDs::startAudioGraph:
            getContentComponent()->startAudioGraph();
            return true;

        case CommandIDs::exportAsEffect:
            exportPlugin ("VST", getContentComponent()->getCurrentCsdFile());
            return true;

        case CommandIDs::exportAsSynth:
            exportPlugin ("VSTi", getContentComponent()->getCurrentCsdFile());
            return true;

        case CommandIDs::toggleComments:
            getContentComponent()->getCurrentCodeEditor()->toggleComments();
            return true;

        case CommandIDs::startLiveDebugger:
            getContentComponent()->getCurrentCodeEditor()->runInDebugMode();
            return true;

        case CommandIDs::zoomIn:
            getContentComponent()->getCurrentCodeEditor()->zoomIn();
            return true;

        case CommandIDs::zoomOut:
            getContentComponent()->getCurrentCodeEditor()->zoomOut();
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

        case CommandIDs::exportAsFMODSoundPlugin:
            return true;

        case CommandIDs::copy:
            getContentComponent()->getCurrentCodeEditor()->copy();
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

//================================================================================
void CabbageDocumentWindow::exportPlugin (String type, File csdFile)
{
    File thisFile (File::getSpecialLocation (File::currentExecutableFile));
    const String currentApplicationDirectory = thisFile.getParentDirectory().getFullPathName();

    if (SystemStats::getOperatingSystemType() == SystemStats::OperatingSystemType::Linux)
    {
        String pluginFilename;

        if (type.contains ("VSTi"))
            pluginFilename = currentApplicationDirectory + String ("/CabbagePluginSynth.so");
        else if (type.contains (String ("VST")))
            pluginFilename = currentApplicationDirectory + String ("/CabbagePluginEffect.so");
        else if (type.contains (String ("LV2-ins")))
            pluginFilename = currentApplicationDirectory + String ("/CabbagePluginSynthLV2.so");
        else if (type.contains (String ("LV2-fx")))
            pluginFilename = currentApplicationDirectory + String ("/CabbagePluginEffectLV2.so");


        File VSTData (pluginFilename);

        if (!VSTData.exists())
        {
            CabbageUtilities::showMessage (pluginFilename + " cannot be found? It should be in the Cabbage root folder", &getLookAndFeel());
        }

        FileChooser fc ("Save file as..", csdFile.getParentDirectory().getFullPathName(), ".so");

        if (fc.browseForFileToSave (false))
        {
            if (fc.getResult().existsAsFile())
            {
                CabbageIDELookAndFeel lookAndFeelTemp;
                const int result = CabbageUtilities::showYesNoMessage ("Do you wish to overwrite\nexiting file?", &lookAndFeelTemp);

                if (result == 0)
                    writePluginFileToDisk (fc.getResult(), csdFile, VSTData);
            }
            else
                writePluginFileToDisk (fc.getResult(), csdFile, VSTData);
        }
    }
}

void CabbageDocumentWindow::writePluginFileToDisk (File fc, File csdFile, File VSTData)
{
    File dll (fc.withFileExtension (".so").getFullPathName());

    if (!VSTData.copyFileTo (dll))
        CabbageUtilities::showMessage ("Can copy plugin lib, is it in use?", &getLookAndFeel());

    if (fc.withFileExtension (".csd").existsAsFile() == false)
    {
        File exportedCsdFile (fc.withFileExtension (".csd").getFullPathName());
        exportedCsdFile.replaceWithText (csdFile.loadFileAsString());
        setUniquePluginId (dll, exportedCsdFile);
        //bunlde all auxilary files
        //addFilesToPluginBundle(csdFile, dll, &getLookAndFeel());
    }
}

const String CabbageDocumentWindow::getPluginId (File csdFile)
{
    StringArray csdLines;
    csdLines.addLines (csdFile.loadFileAsString());

    for (auto line : csdLines)
    {
        ValueTree temp ("temp");
        CabbageWidgetData::setWidgetState (temp, line, 0);

        if (CabbageWidgetData::getStringProp (temp, CabbageIdentifierIds::type) == CabbageIdentifierIds::form)
            return CabbageWidgetData::getStringProp (temp, CabbageIdentifierIds::pluginid);
    }

    return String::empty;
}
//==============================================================================
// Set unique plugin ID for each plugin based on the file name
//==============================================================================
int CabbageDocumentWindow::setUniquePluginId (File binFile, File csdFile)
{

    size_t file_size;
    const char* pluginID;
    pluginID = "YROR";

    long loc;
    std::fstream mFile (binFile.getFullPathName().toUTF8(), ios_base::in | ios_base::out | ios_base::binary);

    if (mFile.is_open())
    {
        mFile.seekg (0, ios::end);
        file_size = mFile.tellg();
        unsigned char* buffer = (unsigned char*)malloc (sizeof (unsigned char) * file_size);

        //set plugin ID, do this a few times in case the plugin ID appear in more than one place.
        for (int r = 0; r < 10; r++)
        {
            mFile.seekg (0, ios::beg);
            mFile.read ((char*)&buffer[0], file_size);
            loc = cabbageFindPluginId (buffer, file_size, pluginID);

            if (loc < 0)
            {
                //showMessage(String("Internel Cabbage Error: The pluginID was not found"));
                break;
            }
            else
            {
                //showMessage(newID);
                mFile.seekg (loc, ios::beg);
                mFile.write (getPluginId (csdFile).toUTF8(), 4);
            }
        }

        //set plugin name based on .csd file
        const char* pluginName = "CabbageEffectNam";
        String plugLibName = csdFile.getFileNameWithoutExtension();

        if (plugLibName.length() < 16)
            for (int y = plugLibName.length(); y < 16; y++)
                plugLibName.append (String (" "), 1);

        mFile.seekg (0, ios::end);
        //buffer = (unsigned char*)malloc(sizeof(unsigned char)*file_size);

        for (int i = 0; i < 5; i++)
        {

            mFile.seekg (0, ios::beg);
            mFile.read ((char*)&buffer[0], file_size);


            loc = cabbageFindPluginId (buffer, file_size, pluginName);

            if (loc < 0)
                break;
            else
            {
                mFile.seekg (loc, ios::beg);
                mFile.write (csdFile.getFileNameWithoutExtension().toUTF8(), 16);
            }
        }

        free (buffer);

    }
    else
        CabbageUtilities::showMessage ("File could not be opened", &getLookAndFeel());

    mFile.close();
    return 1;
}

long CabbageDocumentWindow::cabbageFindPluginId (unsigned char* buf, size_t len, const char* s)
{
    long i, j;
    size_t slen = strlen (s);
    size_t imax = len - slen - 1;
    long ret = -1;
    int match;

    for (i = 0; i < imax; i++)
    {
        match = 1;

        for (j = 0; j < slen; j++)
        {
            if (buf[i + j] != s[j])
            {
                match = 0;
                break;
            }
        }

        if (match)
        {
            ret = i;
            break;
        }
    }

    //return position of plugin ID
    return ret;
}