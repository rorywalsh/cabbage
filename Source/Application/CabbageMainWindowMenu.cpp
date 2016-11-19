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
enum
{
    recentProjectsBaseID = 100,
    activeDocumentsBaseID = 300,
    colourSchemeBaseID = 1000
};

//==============================================================================
// This file contains all implementations for methods relating to
// CabbageDocumentWindow menubar
//==============================================================================
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
    menu.addCommandItem (&commandManager, CommandIDs::newProject);
    menu.addSeparator();
    menu.addCommandItem (&commandManager, CommandIDs::open);

    PopupMenu recentFilesMenu;
    cabbageSettings->updateRecentFilesList();
    cabbageSettings->recentFiles.createPopupMenuItems (recentFilesMenu, recentProjectsBaseID, true, true);
    menu.addSubMenu ("Open Recent", recentFilesMenu);

    menu.addSeparator();
    menu.addCommandItem (&commandManager, CommandIDs::closeDocument);
    menu.addCommandItem (&commandManager, CommandIDs::saveDocument);
    menu.addCommandItem (&commandManager, CommandIDs::saveDocumentAs);
    menu.addCommandItem (&commandManager, CommandIDs::saveAll);

    menu.addSeparator();
    menu.addCommandItem (&commandManager, CommandIDs::closeProject);
    menu.addCommandItem (&commandManager, CommandIDs::saveProject);
    menu.addSeparator();
    menu.addCommandItem (&commandManager, CommandIDs::settings);
    menu.addSeparator();

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
    menu.addSeparator();
    menu.addCommandItem (&commandManager, CommandIDs::showFindPanel);
    menu.addCommandItem (&commandManager, CommandIDs::findSelection);
    menu.addCommandItem (&commandManager, CommandIDs::findNext);
    menu.addCommandItem (&commandManager, CommandIDs::findPrevious);
}

void CabbageDocumentWindow::createViewMenu (PopupMenu& menu)
{
    menu.addSeparator();
    menu.addCommandItem (&commandManager, CommandIDs::showGenericWidgetWindow);
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
    menu.addCommandItem (&commandManager, CommandIDs::runCode);
    menu.addCommandItem (&commandManager, CommandIDs::stopCode);
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
        openFile (cabbageSettings->recentFiles.getFile (menuItemID - recentProjectsBaseID));
    }
}

void CabbageDocumentWindow::getAllCommands (Array <CommandID>& commands)
{

    const CommandID ids[] = { CommandIDs::newProject,
                              CommandIDs::open,
                              CommandIDs::closeAllDocuments,
                              CommandIDs::saveDocument,
                              CommandIDs::saveDocumentAs,
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
                              CommandIDs::showGenericWidgetWindow
                            };

    commands.addArray (ids, numElementsInArray (ids));
}

void CabbageDocumentWindow::getCommandInfo (CommandID commandID, ApplicationCommandInfo& result)
{
    bool shouldShowEditMenu = false;
    if(getCurrentCodeEditor()!= nullptr)
        shouldShowEditMenu = true;

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

    case CommandIDs::saveDocumentAs:
        result.setInfo ("Save file as...", "Save a document", CommandCategories::general, 0);
        result.defaultKeypresses.add (KeyPress ('s', ModifierKeys::shiftModifier | ModifierKeys::commandModifier, 0));
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
        break;

    case CommandIDs::saveAll:
        result.setInfo ("Save All", "Saves all open documents", CommandCategories::general, 0);
        result.defaultKeypresses.add (KeyPress ('s', ModifierKeys::commandModifier | ModifierKeys::altModifier, 0));
        break;

    //edit commands
    case CommandIDs::undo:
        result.setInfo (String("Undo"), String("Undo last action"), CommandCategories::edit, 0);
        result.addDefaultKeypress ('z', ModifierKeys::commandModifier);
        result.setActive((shouldShowEditMenu ? true : false));
        break;
    case CommandIDs::redo:
        result.setInfo (String("Redo"), String("Redo last action"), CommandCategories::edit, 0);
        result.addDefaultKeypress ('z', ModifierKeys::shiftModifier | ModifierKeys::commandModifier);
        result.setActive((shouldShowEditMenu ? true : false));
        break;
    case CommandIDs::cut:
        result.setInfo (String("Cut"), String("Cut selection"), CommandCategories::edit, 0);
        result.addDefaultKeypress ('x', ModifierKeys::commandModifier);
        result.setActive((shouldShowEditMenu ? true : false));
        break;
    case CommandIDs::copy:
        result.setInfo (String("Copy"), String("Copy selection"), CommandCategories::edit, 0);
        result.addDefaultKeypress ('c', ModifierKeys::commandModifier);
        result.setActive((shouldShowEditMenu ? true : false));
        break;
    case CommandIDs::paste:
        result.setInfo (String("Paste"), String("Paste selection"), CommandCategories::edit, 0);
        result.addDefaultKeypress ('v', ModifierKeys::commandModifier);
        result.setActive((shouldShowEditMenu ? true : false));
        break;
    case CommandIDs::columnEdit:
        result.setInfo (String("Column Edit mode"), String("Column Edit"), CommandCategories::edit, 0);
        result.addDefaultKeypress ('l', ModifierKeys::commandModifier);
        result.setActive((shouldShowEditMenu ? true : false));
        break;
    case CommandIDs::toggleComments:
        result.setInfo (String("Toggle comments"), String("Toggle comments"), CommandCategories::edit, 0);
        result.addDefaultKeypress ('/', ModifierKeys::commandModifier);
        result.setActive((shouldShowEditMenu ? true : false));
        break;
    case CommandIDs::searchReplace:
        result.setInfo(String("Search or Replace"), String("Search Replace"), CommandCategories::edit, 0);
        result.addDefaultKeypress ('f', ModifierKeys::commandModifier);
        result.setActive((shouldShowEditMenu ? true : false));
        break;
    case CommandIDs::zoomIn:
        result.setInfo (String("Zoom in"), String("Zoom in"), CommandCategories::edit, 0);
        result.addDefaultKeypress('[', ModifierKeys::commandModifier);
        result.setActive((shouldShowEditMenu ? true : false));
        break;
    case CommandIDs::zoomOut:
        result.setInfo (String("Zoom out"), String("Zoom out"), CommandCategories::edit, 0);
        result.addDefaultKeypress (']', ModifierKeys::commandModifier);
        result.setActive((shouldShowEditMenu ? true : false));
        break;
    case CommandIDs::showGenericWidgetWindow:
        result.setInfo (String("Show Generic Widget Window"), String("Show genric channel based widgets"), CommandCategories::general, 0);
        break;
    case CommandIDs::editMode:
        result.setInfo (String("Edit Mode"), String("Edit Mode"), CommandCategories::edit, 0);
        result.addDefaultKeypress ('e', ModifierKeys::commandModifier);
        break;
    default:

        break;
    }
}

bool CabbageDocumentWindow::perform (const InvocationInfo& info)
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
        setEditMode(false);
        isGUIEnabled = false;
        break;
    case CommandIDs::saveDocumentAs:
        saveDocument(true);
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
        setEditMode(isGUIEnabled);
        break;
    case CommandIDs::showGenericWidgetWindow:
        break;
    default:
        break;
    }

    return true;
}
