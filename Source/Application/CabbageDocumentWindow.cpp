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
CabbageDocumentWindow::CabbageDocumentWindow (String name, String commandLineParams) : DocumentWindow (name,
                                                                                                       Colours::lightgrey,
                                                                                                       DocumentWindow::allButtons),
pluginExporter(),
lookAndFeel (new FlatButtonLookAndFeel()),
commandLineArgs (commandLineParams)
{
    setTitleBarButtonsRequired (DocumentWindow::allButtons, false);
    
    
    
    setTitleBarHeight (20);
    setResizable (true, true);
    centreWithSize (getWidth(), getHeight());
    setVisible (true);
    
    Desktop::getInstance().setDefaultLookAndFeel (lookAndFeel.get()); //set default look and feel for project
    getLookAndFeel().setColour (PopupMenu::ColourIds::highlightedBackgroundColourId, Colour (200, 200, 200));
    
    initSettings();
    
    auto enableKioskMode = cabbageSettings->getUserSettings()->getIntValue ("enableKioskMode");
    if(enableKioskMode == 1)
        setUsingNativeTitleBar (true);
    else
        setUsingNativeTitleBar (false);
    
    pluginExporter.settingsToUse(cabbageSettings->getUserSettings());
    setColour (backgroundColourId, CabbageSettings::getColourFromValueTree (cabbageSettings->valueTree, CabbageColourIds::mainBackground, Colours::lightgrey));
    setContentOwned (content = new CabbageMainComponent (this, cabbageSettings.get()), true);
    content->propertyPanel->setVisible (false);
    cabbageSettings->addChangeListener (content);
    setMenuBar (this, 18/*25*/);
    getMenuBarComponent()->setLookAndFeel (getContentComponent()->lookAndFeel.get());
    
    auto exportedPlugin = false;
    auto openingFile = false;

    if (commandLineArgs.isNotEmpty())
    {
        const auto commandLineParams = JUCEApplication::getCommandLineParameterArray();

        String outputFileName = "";
        int outputFileNameIndex = -1;
        DBG(commandLineParams.joinIntoString("\n"));
        for ( int x = 0; x < commandLineParams.size() ; x++)
        {
            if(commandLineParams[x].contains("--destination"))
                outputFileNameIndex = x;
        }

        if (outputFileNameIndex > -1 && outputFileNameIndex < commandLineParams.size())
        {
            outputFileName = commandLineParams[outputFileNameIndex].replace("--destination=", "");
        }


        const auto exportTypes = StringArray ("AUi", "AU", "VSTi", "VST3i", "VST3", "VST");
        for (const auto &type : exportTypes)
        {
            const auto param = "--export-" + type;
            const auto paramPro = "--export-pro-" + type;

            if (commandLineParams.joinIntoString(" ").contains (param))
            {
                String inputFileName = "";
                int inputFileNameIndex = -1;
                
                for ( int x = 0; x < commandLineParams.size() ; x++)
                {
                    if(commandLineParams[x].contains(paramPro))
                        inputFileNameIndex = x;
                }
                
                if (inputFileNameIndex > -1 && inputFileNameIndex < commandLineParams.size())
                {
                    inputFileName = commandLineParams[inputFileNameIndex].trim().removeCharacters ("\"");
                    if (File (inputFileName).existsAsFile())
                    {
                        content->openFile (inputFileName, false, true);
                        auto inputFile = File (inputFileName);
                        auto id = getPluginInfo (inputFile, "id");
                        auto promptForFilename = false;
                        pluginExporter.exportPlugin (type, inputFile, id, outputFileName, promptForFilename);
                        JUCEApplicationBase::quit();
                        exportedPlugin = true;
                        break;
                    }
                }
            }
            else if (commandLineParams.joinIntoString(" ").contains (paramPro))
            {
                String inputFileName = "";
                int inputFileNameIndex = -1;
                
                for ( int x = 0; x < commandLineParams.size() ; x++)
                {
                    if(commandLineParams[x].contains(paramPro))
                        inputFileNameIndex = x;
                }

                if (inputFileNameIndex > -1 && inputFileNameIndex < commandLineParams.size())
                {
                    inputFileName = commandLineParams[inputFileNameIndex].replace(paramPro+"=", "");
                    if (File (inputFileName).existsAsFile())
                    {
                        content->openFile (inputFileName, false, true);
                        auto inputFile = File (inputFileName);
                        auto id = getPluginInfo (inputFile, "id");
                        auto promptForFilename = false;
                        pluginExporter.exportPlugin (type, inputFile, id, outputFileName, promptForFilename, true);
                        JUCEApplicationBase::quit();
                        exportedPlugin = true;
                        break;
                    }
                }
            }
        }
        
        if (!exportedPlugin && 0 < commandLineParams.size())
        {
            auto csdFile = commandLineParams[0].trim().removeCharacters ("\"");
            if (File::getCurrentWorkingDirectory().getChildFile (csdFile).existsAsFile())
            {
#if JUCE_MAC
                // CSD file will be opened in Cabbage::anotherInstanceStarted.
#else
                content->openFile (csdFile);
#endif
                openingFile = true;
            }
        }
    }
    
    
    if (!openingFile && cabbageSettings->getUserSettings()->getIntValue ("OpenMostRecentFileOnStartup") == 1)
    {
        const String lastOpenedFile = cabbageSettings->getUserSettings()->getValue ("MostRecentFile", "");
        cabbageSettings->updateRecentFilesList();
        StringArray currentFiles;
        const int numberOfFileToOpen = cabbageSettings->getUserSettings()->getIntValue ("NumberOfOpenFiles");
        
        for ( int i = 0 ; i < numberOfFileToOpen; i++ )
        {
            if (File (cabbageSettings->getMostRecentFile (i).getFullPathName()).existsAsFile())
            {
                if(!cabbageSettings->getMostRecentFile (i).hasFileExtension("cabbage"))
                {
                    content->openFile(cabbageSettings->getMostRecentFile(i).getFullPathName(), false);
                    currentFiles.add(cabbageSettings->getMostRecentFile(i).getFullPathName());
                }
            }
        }
        
        // content->openFile (lastOpenedFile, false);
    }
    
    String lastOpenDir = cabbageSettings->getUserSettings()->getValue ("lastOpenedDir", "");
    //    getContentComponent()->fileTree.
    //    getContentComponent()->fileList.setDirectory(File(lastOpenDir), true, true);
    //    getContentComponent()->fileList.refresh();
    setApplicationCommandManagerToWatch (&commandManager);
    commandManager.registerAllCommandsForTarget (this);
    addKeyListener (commandManager.getKeyMappings());
    
    
#if JUCE_MAC
    setMacMainMenu (this);
#else
    setMenuBar (this);
#endif
    
#if JUCE_MAC
    MenuBarModel::setMacMainMenu (this, nullptr, "Open Recent");
#endif
    setLookAndFeel (lookAndFeel.get()); //(&getContentComponent()->getLookAndFeel());
    lookAndFeelChanged();
    
    
    const int width = cabbageSettings->getUserSettings()->getIntValue ("IDE_LastKnownWidth");
    const int height = cabbageSettings->getUserSettings()->getIntValue ("IDE_LastKnownHeight");
    const int x = cabbageSettings->getUserSettings()->getIntValue ("IDE_LastKnownX");
    const int y = cabbageSettings->getUserSettings()->getIntValue ("IDE_LastKnownY");
    this->setTopLeftPosition (x, y);
    setSize (width, height);
        CabbageUtilities::debug(commandLineArgs);
}

void CabbageDocumentWindow::initSettings()
{
    PropertiesFile::Options options;
    options.applicationName     = "Cabbage2";
    options.filenameSuffix      = "settings";
    options.osxLibrarySubFolder = "Application Support";
#if JUCE_LINUX
    options.folderName          = "~/.config/Cabbage2";
#else
    options.folderName          = "Cabbage2";
#endif
    
    cabbageSettings.reset (new CabbageSettings());
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
    
    if (getContentComponent()->getCurrentCodeEditor() && getContentComponent()->getStatusbarYPos() < (getHeight() - 50))
        cabbageSettings->setProperty ("IDE_StatusBarPos", getContentComponent()->getStatusbarYPos());
    
    cabbageSettings->setProperty ("audioSetup", getContentComponent()->getDeviceManagerSettings());
    cabbageSettings->closeFiles();
    
    setLookAndFeel (nullptr);
    
}
//=======================================================================================
CabbageMainComponent* CabbageDocumentWindow::getContentComponent()
{
    return content;
}

/*void CabbageDocumentWindow::maximiseButtonPressed()
 {
 getContentComponent()->resizeAllWindows (getHeight());
 }*/

void CabbageDocumentWindow::closeButtonPressed()
{
    CabbageIDELookAndFeel lookAndFeel;
    if (getContentComponent()->editorAndConsole.size() > 0)
    {
        if (getContentComponent()->getCurrentCodeEditor()->hasFileChanged() == true)
        {
            const int result = CabbageUtilities::showYesNoMessage("File has been modified, do you wish to save?\nexiting file?", &lookAndFeel, 1);
            
            if (result == 1)
            {
                getContentComponent()->saveDocument(false, false);
                getContentComponent()->removeEditor();
            }
            else if (result == 2)
                getContentComponent()->removeEditor();
            else
                return;
        }
    }
    
    /*
     if (getContentComponent()->getFilterGraph()->hasChangedSinceSaved())
     {
     const int result = CabbageUtilities::showYesNoMessage ("Save changes made to Cabbage\npatch?", &lookAndFeel, 1);
     
     if (result == 1)
     {
     if (getContentComponent()->getFilterGraph()->saveDocument() == FileBasedDocument::SaveResult::userCancelledSave)
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
     */
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

PopupMenu CabbageDocumentWindow::createFontMenu()
{
    PopupMenu menu;
    fonts.clear();
    Font::findFonts(fonts);
    int i = -10000;
    menu.addItem(i++, "Default font (Deja Vu)");
    for (auto& f : fonts)
    {
        menu.addItem(i++, f.getTypefaceName());
    }

    return menu;
}

void CabbageDocumentWindow::createFileMenu (PopupMenu& menu)
{
    menu.addCommandItem (&commandManager, CommandIDs::newFile);
    menu.addCommandItem (&commandManager, CommandIDs::newTextFile);
    menu.addSeparator();
    menu.addCommandItem (&commandManager, CommandIDs::open);
    menu.addCommandItem (&commandManager, CommandIDs::openFolder);
    
	PopupMenu subPatchMenu;
	subPatchMenu.addCommandItem(&commandManager, CommandIDs::openCabbagePatch);
	subPatchMenu.addCommandItem(&commandManager, CommandIDs::saveGraph);
	subPatchMenu.addCommandItem(&commandManager, CommandIDs::saveGraphAs);
	
	menu.addSubMenu("Cabbage patcher", subPatchMenu);
	
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
    menu.addCommandItem (&commandManager, CommandIDs::saveAll);
    menu.addSeparator();
    menu.addCommandItem (&commandManager, CommandIDs::convertToCamelCase);
    menu.addCommandItem (&commandManager, CommandIDs::convertToLowerCase);
    menu.addCommandItem (&commandManager, CommandIDs::updatePresetFile);
    menu.addSeparator();
    menu.addSeparator();
    menu.addCommandItem (&commandManager, CommandIDs::restartAudioDevice);
    menu.addSeparator();
	if(CabbageUtilities::getTargetPlatform() == CabbageUtilities::TargetPlatformTypes::OSX)
    {
        PopupMenu subMenu1, subMenu2, subMenu3;
        subMenu1.addCommandItem (&commandManager, CommandIDs::exportAsVSTEffect);
        subMenu1.addCommandItem (&commandManager, CommandIDs::exportAsVSTSynth);
        subMenu1.addCommandItem (&commandManager, CommandIDs::exportAsVST3Effect);
        subMenu1.addCommandItem (&commandManager, CommandIDs::exportAsVST3Synth);
        subMenu3.addSubMenu("VST Export", subMenu1);
        subMenu2.addCommandItem (&commandManager, CommandIDs::exportAsAUEffect);
        subMenu2.addCommandItem (&commandManager, CommandIDs::exportAsAUSynth);
        subMenu2.addCommandItem (&commandManager, CommandIDs::exportAsAUMIDIFx);
        subMenu3.addSubMenu("AU Export", subMenu2);
        menu.addSubMenu("Export Plugin", subMenu3);
#if CabbagePro
        PopupMenu subMenu4, subMenu5, subMenu6;
        subMenu4.addCommandItem (&commandManager, CommandIDs::exportAsVSTEffectEncrypted);
        subMenu4.addCommandItem (&commandManager, CommandIDs::exportAsVSTSynthEncrypted);
        subMenu4.addCommandItem (&commandManager, CommandIDs::exportAsVST3EffectEncrypted);
        subMenu4.addCommandItem (&commandManager, CommandIDs::exportAsVST3SynthEncrypted);
        subMenu6.addSubMenu("VST Export", subMenu4);
        subMenu5.addCommandItem (&commandManager, CommandIDs::exportAsAUEffectEncrypted);
        subMenu5.addCommandItem (&commandManager, CommandIDs::exportAsAUSynthEncrypted);
        subMenu5.addCommandItem (&commandManager, CommandIDs::exportAsAUMIDIFxEncrypted);
        subMenu6.addSubMenu("AU Export", subMenu5);
        menu.addSubMenu("Export Plugin (Encrypt CSD)", subMenu6);
#endif
        
    }
    else
    {
        PopupMenu subMenu1;
        subMenu1.addCommandItem (&commandManager, CommandIDs::exportAsVSTEffect);
        subMenu1.addCommandItem (&commandManager, CommandIDs::exportAsVSTSynth);
#ifndef JUCE_LINUX
        subMenu1.addCommandItem(&commandManager, CommandIDs::exportAsVST3Effect);
        subMenu1.addCommandItem(&commandManager, CommandIDs::exportAsVST3Synth);
#endif
        menu.addSubMenu("Export plugin", subMenu1);
        
#if CabbagePro
        PopupMenu subMenu2;
        subMenu2.addCommandItem (&commandManager, CommandIDs::exportAsVSTEffectEncrypted);
        subMenu2.addCommandItem (&commandManager, CommandIDs::exportAsVSTSynthEncrypted);
        subMenu2.addCommandItem(&commandManager, CommandIDs::exportAsVST3EffectEncrypted);
        subMenu2.addCommandItem(&commandManager, CommandIDs::exportAsVST3SynthEncrypted);
        menu.addSubMenu("Export plugin (Encrypt CSD)", subMenu2);
#endif
    }
    menu.addSeparator();
	menu.addCommandItem(&commandManager, CommandIDs::exportNativeUnity);
	menu.addCommandItem(&commandManager, CommandIDs::exportAsVCVRackModule);
	menu.addCommandItem (&commandManager, CommandIDs::exportAsStandaloneApp);

#if CabbagePro
    menu.addCommandItem (&commandManager, CommandIDs::exportAsStandaloneEncrypted);
#endif
    if(CabbageUtilities::getTargetPlatform() != CabbageUtilities::TargetPlatformTypes::Linux)
    {
        menu.addCommandItem(&commandManager, CommandIDs::exportAsFMODSoundPlugin);
        menu.addCommandItem(&commandManager, CommandIDs::exportAsFMODFxPlugin);
    }

    
    menu.addSeparator();
    
#if !CabbagePro
    if (CabbageUtilities::getTargetPlatform() == CabbageUtilities::TargetPlatformTypes::OSX)
    {
        PopupMenu batch;
        batch.addCommandItem(&commandManager, CommandIDs::batchConvertExamplesAU);
        batch.addCommandItem(&commandManager, CommandIDs::batchConvertExamplesVST);
        menu.addSubMenu("Batch Convert Examples", batch);
    }
    else
    {
        menu.addCommandItem(&commandManager, CommandIDs::batchConvertExamplesVST);
    }
#endif
    menu.addSeparator();
    menu.addCommandItem(&commandManager, CommandIDs::importTheme);
    menu.addCommandItem(&commandManager, CommandIDs::exportTheme);
    menu.addSeparator();
    
    menu.addCommandItem (&commandManager, CommandIDs::closeProject);
    menu.addCommandItem (&commandManager, CommandIDs::saveProject);
    menu.addSeparator();
    menu.addCommandItem (&commandManager, CommandIDs::closeDocument);
    menu.addSeparator();
    menu.addCommandItem (&commandManager, CommandIDs::autoReloadFromDisk);
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
    menu.addCommandItem (&commandManager, CommandIDs::addCabbageSection);
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
    PopupMenu m = createFontMenu();
    menu.addSubMenu("Select Font", m);
    menu.addSeparator();
	menu.addCommandItem(&commandManager, CommandIDs::sendToPort);
	menu.addSeparator();

    
    subMenu.addCommandItem (&commandManager, CommandIDs::zoomInConsole);
    subMenu.addCommandItem (&commandManager, CommandIDs::zoomOutConsole);
    subMenu.addCommandItem (&commandManager, CommandIDs::clearConsole);
    
    
    menu.addCommandItem (&commandManager, CommandIDs::showFindPanel);
    menu.addCommandItem (&commandManager, CommandIDs::showReplacePanel);
    
    menu.addCommandItem (&commandManager, CommandIDs::findSelection);
    menu.addCommandItem (&commandManager, CommandIDs::findNext);
    menu.addCommandItem (&commandManager, CommandIDs::findPrevious);
    menu.addSeparator();
    menu.addSubMenu ("Console", subMenu);

    menu.addSeparator();
    menu.addCommandItem (&commandManager, CommandIDs::settings);
    
}

void CabbageDocumentWindow::createViewMenu (PopupMenu& menu)
{
    menu.addCommandItem (&commandManager, CommandIDs::nextTab);
    menu.addCommandItem (&commandManager, CommandIDs::showGraph);
    menu.addCommandItem (&commandManager, CommandIDs::showConsole);
    menu.addCommandItem(&commandManager, CommandIDs::toggleProperties);
    menu.addCommandItem(&commandManager, CommandIDs::toggleFileBrowser);
    menu.addCommandItem(&commandManager, CommandIDs::showPluginListEditor);
    menu.addSeparator();
    menu.addCommandItem (&commandManager, CommandIDs::zoomIn);
    menu.addCommandItem (&commandManager, CommandIDs::zoomOut);
}


void CabbageDocumentWindow::createToolsMenu (PopupMenu& menu)
{
    menu.addCommandItem (&commandManager, CommandIDs::buildNow);
    menu.addCommandItem (&commandManager, CommandIDs::buildNoConnect);
    menu.addSeparator();
    menu.addCommandItem (&commandManager, CommandIDs::startAudioGraph);
    menu.addCommandItem (&commandManager, CommandIDs::stopAudioGraph);
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
    else if (menuItemID < 0)
    {
        if (menuItemID + 10000 == 0)
        {
            getContentComponent()->getCurrentEditorContainer()->setDefaultFont();
            cabbageSettings->getUserSettings()->setValue("Font", "Default");
        }
        else
        {
            getContentComponent()->getCurrentCodeEditor()->setFont(fonts[menuItemID + 9999]);
            cabbageSettings->getUserSettings()->setValue("Font", fonts[menuItemID + 9999].getTypefaceName());
            DBG(fonts[menuItemID + 9999].getTypefaceName());
        }
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
        CommandIDs::newTextFile,
        CommandIDs::open,
        CommandIDs::openFolder,
        CommandIDs::openFromRPi,
        CommandIDs::closeAllDocuments,
        CommandIDs::closeDocument,
        CommandIDs::saveDocument,
        CommandIDs::buildNow,
        CommandIDs::buildNoConnect,
        CommandIDs::saveGraph,
        CommandIDs::saveGraphAs,
        CommandIDs::exportTheme,
        CommandIDs::importTheme,
        CommandIDs::saveDocumentToRPi,
        CommandIDs::saveDocumentAs,
        CommandIDs::examples,
        CommandIDs::settings,
        CommandIDs::startAudioGraph,
        CommandIDs::stopAudioGraph,
        CommandIDs::exportAsVSTSynth,
        CommandIDs::exportAsVST3Synth,
        CommandIDs::exportAsAUMIDIFx,
        CommandIDs::exportAsStandaloneApp,
        CommandIDs::exportAsStandaloneEncrypted,
        CommandIDs::selectAll,
        CommandIDs::exportAsVSTEffect,
        CommandIDs::exportAsVST3Effect,
        CommandIDs::exportAsAUEffect,
        CommandIDs::exportAsAUSynth,
        CommandIDs::exportAsAUSynthEncrypted,
        CommandIDs::exportAsAUMIDIFxEncrypted,
        CommandIDs::exportAsAUEffectEncrypted,
        CommandIDs::exportAsVSTSynthEncrypted,
        CommandIDs::exportAsVSTEffectEncrypted,
        CommandIDs::exportAsVST3SynthEncrypted,
        CommandIDs::exportAsVST3EffectEncrypted,
        CommandIDs::exportAsVCVRackModule,
        CommandIDs::nextTab,
        CommandIDs::exportAsFMODSoundPlugin,
        CommandIDs::exportAsFMODFxPlugin,
        CommandIDs::copy,
        CommandIDs::cut,
        CommandIDs::clearConsole,
        CommandIDs::convertToCamelCase,
        CommandIDs::convertToLowerCase,
        CommandIDs::updatePresetFile,
        CommandIDs::restartAudioDevice,
        CommandIDs::toggleComments,
        CommandIDs::zoomIn,
        CommandIDs::addCabbageSection,
        CommandIDs::zoomOut,
        CommandIDs::zoomInConsole,
        CommandIDs::zoomOutConsole,
        CommandIDs::showConsole,
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
        CommandIDs::toggleProperties,
        CommandIDs::contextHelp,
        CommandIDs::showGenericWidgetWindow,
        CommandIDs::batchConvertExamplesAU,
        CommandIDs::batchConvertExamplesVST,
        CommandIDs::toggleFileBrowser,
        CommandIDs::showPluginListEditor,
        CommandIDs::autoReloadFromDisk,
		CommandIDs::sendToPort,
		CommandIDs::exportNativeUnity
    };
    
    commands.addArray (ids, numElementsInArray (ids));
}

void CabbageDocumentWindow::getCommandInfo (CommandID commandID, ApplicationCommandInfo& result)
{
    bool shouldShowEditMenu = false;
    int disableNodeConnections = cabbageSettings->getUserSettings()->getIntValue ("autoConnectNodes", 1);
    int autoReloadFromDisk = cabbageSettings->getUserSettings()->getIntValue ("AutoReloadFromDisk", 0);
    if (getContentComponent()->getCurrentEditorContainer() != nullptr)
        shouldShowEditMenu = true;

    
    switch (commandID)
    {
        case CommandIDs::newFile:
            result.setInfo ("New Csound file", "Create a new Csound file", CommandCategories::general, 0);
            result.defaultKeypresses.add (KeyPress ('n', ModifierKeys::commandModifier, 0));
            break;
            
        case CommandIDs::newTextFile:
            result.setInfo ("New text file", "Create a new text file", CommandCategories::general, 0);
            result.defaultKeypresses.add (KeyPress ('n', ModifierKeys::commandModifier | ModifierKeys::altModifier, 0));
            break;
            
        case CommandIDs::openCabbagePatch:
            result.setInfo ("Open Cabbage patch...", "Opens a Cabbage patch", CommandCategories::general, 0);
            //result.defaultKeypresses.add (KeyPress ('o', ModifierKeys::commandModifier | ModifierKeys::altModifier, 0));
            break;
            
        case CommandIDs::open:
            result.setInfo ("Open Csound file", "Opens a project", CommandCategories::general, 0);
            result.defaultKeypresses.add (KeyPress ('o', ModifierKeys::commandModifier, 0));
            break;
            
        case CommandIDs::openFolder:
            result.setInfo ("Open Folder", "Opens a folder in file browser", CommandCategories::general, 0);
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
            
        case CommandIDs::convertToCamelCase:
            result.setInfo ("Convert Identifiers to camelCase", "Covert Identifiers to camelCase", CommandCategories::general, 0);
            break;
        case CommandIDs::convertToLowerCase:
            result.setInfo ("Convert Identifiers to lower case", "Covert Identifiers to Lower Case", CommandCategories::general, 0);
            break;
        case CommandIDs::updatePresetFile:
            result.setInfo ("Update XML Preset File to JSON", "Update form older version", CommandCategories::general, 0);
            break;
            
        case CommandIDs::restartAudioDevice:
            result.setInfo ("Restart Audio Device", "Restarts audio device", CommandCategories::general, 0);
            result.defaultKeypresses.add (KeyPress ('d', ModifierKeys::commandModifier, 0));
            break;
            
        case CommandIDs::importTheme:
            result.setInfo("Import theme", "import a custom theme", CommandCategories::general, 0);
            break;
            
        case CommandIDs::exportTheme:
            result.setInfo("Export theme", "Export a custom theme", CommandCategories::general, 0);
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
           
        case CommandIDs::addCabbageSection:
            result.setInfo ("Add Cabbage section", "Add a Cababge section", CommandCategories::edit, 0);
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

        case CommandIDs::buildNoConnect:
            result.setInfo ("Auto-connect to graph", "Enables auto connect to audio graph", CommandCategories::general, 0);
            result.defaultKeypresses.add (KeyPress (',', ModifierKeys::commandModifier, 0));
            result.setTicked((disableNodeConnections==1 ? true : false));
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
            
        case CommandIDs::exportAsVSTSynth:
            result.setInfo ("Export as VST Plugin Synth", "Exports as plugin", CommandCategories::general, 0);
            break;
            
        case CommandIDs::exportAsVSTEffect:
            result.setInfo ("Export as VST Plugin Effect", "Exports as plugin", CommandCategories::general, 0);
            break;
            
        case CommandIDs::exportAsVST3Synth:
            result.setInfo ("Export as VST3 Plugin Synth", "Exports as plugin", CommandCategories::general, 0);
            break;
            
        case CommandIDs::exportAsVST3Effect:
            result.setInfo ("Export as VST3 Plugin Effect", "Exports as plugin", CommandCategories::general, 0);
            break;
            
        case CommandIDs::exportAsAUSynth:
            result.setInfo ("Export as AU Plugin Synth", "Exports as plugin", CommandCategories::general, 0);
            break;
            
        case CommandIDs::exportAsAUEffect:
            result.setInfo ("Export as AU Plugin Effect", "Exports as plugin", CommandCategories::general, 0);
            break;
            
        case CommandIDs::exportAsAUMIDIFx:
            result.setInfo ("Export as AU MIDI Effect Plugin", "Exports as MIDI effectplugin", CommandCategories::general, 0);
            break;
            
        case CommandIDs::exportAsVSTSynthEncrypted:
            result.setInfo ("Export as VST Plugin Synth", "Exports as plugin", CommandCategories::general, 0);
            break;
            
        case CommandIDs::exportAsVSTEffectEncrypted:
            result.setInfo ("Export as VST Plugin Effect", "Exports as plugin", CommandCategories::general, 0);
            break;
            
        case CommandIDs::exportAsVST3SynthEncrypted:
            result.setInfo ("Export as VST3 Plugin Synth", "Exports as plugin", CommandCategories::general, 0);
            break;
            
        case CommandIDs::exportAsVST3EffectEncrypted:
            result.setInfo ("Export as VST3 Plugin Effect", "Exports as plugin", CommandCategories::general, 0);
            break;
            
        case CommandIDs::exportAsAUSynthEncrypted:
            result.setInfo ("Export as AU Plugin Synth", "Exports as plugin", CommandCategories::general, 0);
            break;

        case CommandIDs::exportAsAUMIDIFxEncrypted:
            result.setInfo ("Export as AU MIDI Effect Plugin", "Exports as plugin", CommandCategories::general, 0);
            break;
            
        case CommandIDs::exportAsAUEffectEncrypted:
            result.setInfo ("Export as AU Plugin Effect", "Exports as plugin", CommandCategories::general, 0);
            break;
            
        case CommandIDs::exportAsFMODSoundPlugin:
            result.setInfo ("Export as FMOD Sound Plugin", "Exports as plugin", CommandCategories::general, 0);
            break;

		case CommandIDs::exportAsFMODFxPlugin:
			result.setInfo("Export as FMOD Fx Plugin", "Exports as plugin", CommandCategories::general, 0);
			break;

		case CommandIDs::exportNativeUnity:
			result.setInfo("Export as Native Unity Plugin", "Exports as plugin", CommandCategories::general, 0);
			break;
            
        case CommandIDs::batchConvertExamplesAU:
            result.setInfo ("As AU plugins", "Batch export as plugin", CommandCategories::general, 0);
            break;
            
        case CommandIDs::exportAsVCVRackModule:
            result.setInfo ("Export as VCV Rack Module", "VCV Rack export", CommandCategories::general, 0);
            break;
        
        case CommandIDs::exportAsStandaloneApp:
            result.setInfo ("Export as Standalone application", "Standalone export", CommandCategories::general, 0);
            break;
           
        case CommandIDs::exportAsStandaloneEncrypted:
            result.setInfo ("Export as Standalone application (Encrypted)", "Standalone export", CommandCategories::general, 0);
            break;
            
        case CommandIDs::batchConvertExamplesVST:
            if (CabbageUtilities::getTargetPlatform() == CabbageUtilities::TargetPlatformTypes::OSX)
                result.setInfo("As VST plugins", "Batch export folder as plugin", CommandCategories::general, 0);
            else
                result.setInfo("Convert samples to VST plugins", "Batch export folder as plugin", CommandCategories::general, 0);
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
            
		case CommandIDs::sendToPort:
			result.setInfo(String("Send to port"), String("Send to port"), CommandCategories::edit, 0);
			result.addDefaultKeypress('.', ModifierKeys::commandModifier);
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
            
        case CommandIDs::showPluginListEditor:
            result.setInfo (String ("Show 3rd party plugins"), String ("Show 3rd party plugins"), CommandCategories::general, 0);
            break;
            
        case CommandIDs::editMode:
            result.setInfo (String ("Edit Mode"), String ("Edit Mode"), CommandCategories::edit, 0);
            result.addDefaultKeypress ('e', ModifierKeys::commandModifier);
            //result.setTicked (getContentComponent()->getCabbagePluginEditor() == nullptr ? false : getContentComponent()->getCabbagePluginEditor()->isEditModeEnabled());
            result.setActive ((shouldShowEditMenu ? true : false));
            break;
            
        case CommandIDs::showConsole:
            result.setInfo (TRANS ("Show Csound output console"), TRANS ("Shows the Csound console window."), "View", 0);
            break;
            
        case CommandIDs::toggleProperties:
            result.setInfo(TRANS("Toggle Properties"), TRANS("Toggle property panel"), "View", 0);
            result.addDefaultKeypress('h', ModifierKeys::commandModifier);
            result.setActive((shouldShowEditMenu ? true : false));
            break;
            
        case CommandIDs::toggleFileBrowser:
            result.setInfo(TRANS("Toggle File Browser"), TRANS("Toggle file browser"), "View", 0);
            result.addDefaultKeypress('b', ModifierKeys::commandModifier);
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
      
        case CommandIDs::autoReloadFromDisk:
            result.setInfo(TRANS("Auto-load from disk"), TRANS("Enable auto-reload"), "File", 0);
            result.setTicked((autoReloadFromDisk==1 ? true : false));
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
#if CabbagePro
    String aboutInfo = "Copyright 2008\n\nVersion:"+String(ProjectInfo::versionString)+"\nLicensed to: " + String(CabbageManufacturer);
#else
    String aboutInfo = "Copyright 2008\n\nVersion:"+String(ProjectInfo::versionString);
    //String aboutInfo = commandLineArgs;
#endif
    
    pluginExporter.adhocSign = (cabbageSettings->getUserSettings()->getIntValue ("performAdHocCodesign") == 1 ? true : false);
    
    CabbageIDELookAndFeel tempLookAndFeel;
    const File currentFile = getContentComponent()->getCurrentCsdFile();
    switch (info.commandID)
    {
        case CommandIDs::newFile:
            getContentComponent()->createNewProject();
            return true;
            
        case CommandIDs::newTextFile:
            getContentComponent()->createNewTextFile();
            return true;
            
        case CommandIDs::openCabbagePatch:
            getContentComponent()->openGraph();
            return true;
            
        case CommandIDs::open:
            getContentComponent()->openFile();
            return true;
            
        case CommandIDs::openFolder:
            getContentComponent()->openFolder();
            return true;
            
        case CommandIDs::openFromRPi:
            getContentComponent()->launchSSHFileBrowser ("open");
            return true;
            
        case CommandIDs::saveDocumentAs:
            getContentComponent()->saveDocument (true);
            return true;
            
        case CommandIDs::convertToCamelCase:
            getContentComponent()->covertToCamelCase();
            return true;
            
        case CommandIDs::convertToLowerCase:
            getContentComponent()->covertToLowerCase();
            return true;
 
        case CommandIDs::updatePresetFile:
            getContentComponent()->updatePresetFile();
            return true;
            
        case CommandIDs::restartAudioDevice:
            getContentComponent()->reloadAudioDeviceState();
            return true;
            
        case CommandIDs::saveGraph:
            getContentComponent()->saveGraph (false);
            return true;
            
        case CommandIDs::saveGraphAs:
            getContentComponent()->saveGraph (true);
            return true;

        case CommandIDs::addCabbageSection:
            getContentComponent()->addCabbageSection ();
            return true;
            
        case CommandIDs::buildNoConnect:
            if(cabbageSettings->getUserSettings()->getIntValue ("autoConnectNodes") == 1)
                cabbageSettings->getUserSettings()->setValue("autoConnectNodes", 0);
            else
                cabbageSettings->getUserSettings()->setValue("autoConnectNodes", 1);
            return true;

            case CommandIDs::buildNow:
        case CommandIDs::saveDocument:
            
            getContentComponent()->saveDocument();
            
            isGUIEnabled = false;
            break;
            
        case CommandIDs::closeDocument:
            getContentComponent()->closeDocument();
            return true;
            
        case CommandIDs::closeAllDocuments:
            return true;
            
        case CommandIDs::nextTab:
            getContentComponent()->handleFileTab (nullptr, true);
            return true;
            
        case CommandIDs::settings:
            getContentComponent()->showSettingsDialog();
            return true;
            
        case CommandIDs::startAudioGraph:
            getContentComponent()->startFilterGraph();
            return true;
            
        case CommandIDs::exportAsVSTEffect:
            pluginExporter.exportPlugin ("VST", getContentComponent()->getCurrentCsdFile(),  getPluginInfo (currentFile, "id"));
            return true;
            
        case CommandIDs::exportAsVSTSynth:
            pluginExporter.exportPlugin ("VSTi", getContentComponent()->getCurrentCsdFile(),  getPluginInfo (currentFile, "id"));
            return true;
            
        case CommandIDs::exportAsVST3Effect:
            pluginExporter.exportPlugin ("VST3", getContentComponent()->getCurrentCsdFile(),  getPluginInfo (currentFile, "id"));
            return true;
            
        case CommandIDs::exportAsVST3Synth:
            pluginExporter.exportPlugin ("VST3i", getContentComponent()->getCurrentCsdFile(),  getPluginInfo (currentFile, "id"));
            return true;
            
        case CommandIDs::exportAsAUEffect:
            pluginExporter.exportPlugin ("AU", getContentComponent()->getCurrentCsdFile(),  getPluginInfo (currentFile, "id"));
            return true;
            
        case CommandIDs::exportAsAUSynth:
            pluginExporter.exportPlugin ("AUi", getContentComponent()->getCurrentCsdFile(),  getPluginInfo (currentFile, "id"));
            return true;
            
        case CommandIDs::exportAsAUMIDIFx:
            pluginExporter.exportPlugin ("AUMIDIFx", getContentComponent()->getCurrentCsdFile(),  getPluginInfo (currentFile, "id"));
            return true;
            
        case CommandIDs::exportAsVSTEffectEncrypted:
            pluginExporter.exportPlugin ("VST", getContentComponent()->getCurrentCsdFile(),  getPluginInfo (currentFile, "id"), "", true, true);
            return true;
            
        case CommandIDs::exportAsVSTSynthEncrypted:
            pluginExporter.exportPlugin ("VSTi", getContentComponent()->getCurrentCsdFile(),  getPluginInfo (currentFile, "id"), "", true, true);
            return true;
            
        case CommandIDs::exportAsVST3EffectEncrypted:
            pluginExporter.exportPlugin ("VST3", getContentComponent()->getCurrentCsdFile(),  getPluginInfo (currentFile, "id"), "", true, true);
            return true;
            
        case CommandIDs::exportAsVST3SynthEncrypted:
            pluginExporter.exportPlugin ("VST3i", getContentComponent()->getCurrentCsdFile(),  getPluginInfo (currentFile, "id"), "", true, true);
            return true;
            
        case CommandIDs::exportAsAUEffectEncrypted:
            pluginExporter.exportPlugin ("AU", getContentComponent()->getCurrentCsdFile(),  getPluginInfo (currentFile, "id"), "", true, true);
            return true;
            
        case CommandIDs::exportAsAUSynthEncrypted:
            pluginExporter.exportPlugin ("AUi", getContentComponent()->getCurrentCsdFile(),  getPluginInfo (currentFile, "id"), "", true, true);
            return true;

        case CommandIDs::exportAsAUMIDIFxEncrypted:
            pluginExporter.exportPlugin ("AUMIDIFx", getContentComponent()->getCurrentCsdFile(),  getPluginInfo (currentFile, "id"), "", true, true);
            return true;
            
        case CommandIDs::exportAsFMODSoundPlugin:
            pluginExporter.exportPlugin("FMOD", getContentComponent()->getCurrentCsdFile(), getPluginInfo(currentFile, "id"));
            return true;

		case CommandIDs::exportNativeUnity:
			pluginExporter.exportPlugin("Unity", getContentComponent()->getCurrentCsdFile(), getPluginInfo(currentFile, "id"));
			return true;

        case CommandIDs::exportAsFMODFxPlugin:
            pluginExporter.exportPlugin("FMODFx", getContentComponent()->getCurrentCsdFile(), getPluginInfo(currentFile, "id"));
            return true;
            
        case CommandIDs::exportTheme:
            getContentComponent()->exportTheme();
            return true;
            
        case CommandIDs::importTheme:
            getContentComponent()->importTheme();
            return true;
            
        case CommandIDs::exportAsVCVRackModule:
            pluginExporter.exportPlugin ("VCVRack", getContentComponent()->getCurrentCsdFile(),  getPluginInfo (currentFile, "id"));
            return true;
 
        case CommandIDs::exportAsStandaloneApp:
            pluginExporter.exportPlugin ("Standalone", getContentComponent()->getCurrentCsdFile(),  getPluginInfo (currentFile, "id"));
            return true;
            
        case CommandIDs::exportAsStandaloneEncrypted:
            pluginExporter.exportPlugin ("Standalone", getContentComponent()->getCurrentCsdFile(),  getPluginInfo (currentFile, "id"), "", true, true);
            return true;
            
        case CommandIDs::batchConvertExamplesAU:
            exportExamplesToPlugins("AU");
            return true;
            
        case CommandIDs::batchConvertExamplesVST:
            exportExamplesToPlugins("VST");
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
            getContentComponent()->getCurrentOutputConsole()->zoom (true);
            return true;
            
        case CommandIDs::zoomOutConsole:
            getContentComponent()->getCurrentOutputConsole()->zoom (false);
            return true;
            
		case CommandIDs::sendToPort:
			getContentComponent()->sendToPort();
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
            
        case CommandIDs::toggleProperties:
            getContentComponent()->togglePropertyPanel();
            return true;
            
        case CommandIDs::toggleFileBrowser:
            getContentComponent()->toggleBrowser();
            break;
            
            
        case CommandIDs::showReplacePanel:
            getContentComponent()->showFindPanel (true);
            return true;
            
        case CommandIDs::clearConsole:
            getContentComponent()->getCurrentOutputConsole()->clearText();
            return true;
            
        case CommandIDs::copy:
            getContentComponent()->getCurrentCodeEditor()->copy();
            return true;
            
        case CommandIDs::showConsole:
            if (getContentComponent()->getNumberOfFileTabs() > 0)
            {
                getContentComponent()->getCurrentEditorContainer()->statusBar.setTopLeftPosition(0, getHeight()*.66);
                getContentComponent()->getCurrentEditorContainer()->resized();
            }
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
            getContentComponent()->stopFilterGraph();
            //getContentComponent()->getCurrentCodeEditor()->stopDebugMode();
            break;
            
        case CommandIDs::about:
            CabbageUtilities::showMessage (aboutInfo, &tempLookAndFeel);
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
            
        case CommandIDs::showPluginListEditor:
            getContentComponent()->showPluginListEditor();
            break;
            
        case CommandIDs::autoReloadFromDisk:
            if(cabbageSettings->getUserSettings()->getIntValue ("AutoReloadFromDisk") == 1)
                cabbageSettings->getUserSettings()->setValue("AutoReloadFromDisk", 0);
            else
                cabbageSettings->getUserSettings()->setValue("AutoReloadFromDisk", 1);
            
            getContentComponent()->enableAutoUpdateMode();
            break;
            
        case CommandIDs::editMode:
            getContentComponent()->enableEditMode();
            break;
            
        default:
            break;
    }
    
    return true;
}


//==================================================================================================================
void CabbageDocumentWindow::exportExamplesToPlugins(String type)
{
    const String examplesDir = cabbageSettings->getUserSettings()->getValue ("CabbageExamplesDir", "");
    File examples(examplesDir);
    
    StringArray parentFolders;
    parentFolders.add("Instruments");
    parentFolders.add("Effects");
    parentFolders.add("LiveSampling");
    parentFolders.add("FilePlayers");
    CabbageIDELookAndFeel tempLookAndFeel;
    Array<File> instrumentFiles, effectsFiles, samplingFiles, filePlayerFiles;
    int go = CabbageUtilities::showYesNoMessage("This command will export all examples from the 'Instruments', 'Effects', 'Live Sampling' and 'File Players' folders. "
                                                "This could take up to a gigabyte of hard disk space. Do you wish to continue?", &tempLookAndFeel);
    if(go == 1)
    {
        StringArray effectFolders = CabbageExamplesFolder::getEffects();
        StringArray instrumentFolders = CabbageExamplesFolder::getInstruments();
        
        for( auto folder : instrumentFolders)
        {
            File searchDir (examplesDir + "/Instruments/"+ folder);
            searchDir.findChildFiles (instrumentFiles, File::findFiles, false, "*.csd");
            instrumentFiles.sort();
        }
        for( auto folder : effectFolders)
        {
            File searchDir (examplesDir + "/Effects/"+ folder);
            searchDir.findChildFiles (effectsFiles, File::findFiles, false, "*.csd");
            effectsFiles.sort();
        }
        
        {
            File searchDir (examplesDir + "/LiveSampling");
            searchDir.findChildFiles (samplingFiles, File::findFiles, false, "*.csd");
            samplingFiles.sort();
        }
        {
            File searchDir (examplesDir + "/FilePlayers");
            searchDir.findChildFiles (filePlayerFiles, File::findFiles, false, "*.csd");
            filePlayerFiles.sort();
        }

    
        FileChooser fc ("Select file name and location", File("~"), "", CabbageUtilities::shouldUseNativeBrowser());

        if (fc.browseForDirectory())
        {
            const int result = CabbageUtilities::showYesNoMessage ("This will overwrite any existing files, and may take a few moments to process. Do you wish to continue?", &tempLookAndFeel);

            if (result == 1)
            {
                for( auto filename : instrumentFiles)
                    pluginExporter.exportPlugin (type=="AU" ? "AUi" : "VSTi", filename,  getPluginInfo (filename, "id"), fc.getResult().getFullPathName());
                for( auto filename : effectsFiles)
                    pluginExporter.exportPlugin (type=="AU" ? "AU" : "VST", filename,  getPluginInfo (filename, "id"), fc.getResult().getFullPathName());
                for( auto filename : samplingFiles)
                    pluginExporter.exportPlugin (type=="AU" ? "AU" : "VST", filename,  getPluginInfo (filename, "id"), fc.getResult().getFullPathName());
                for( auto filename : filePlayerFiles)
                    pluginExporter.exportPlugin (type=="AU" ? "AU" : "VST", filename,  getPluginInfo (filename, "id"), fc.getResult().getFullPathName());

            }
        }
    }
    
}

const String CabbageDocumentWindow::getPluginInfo (File csdFile, String info)
{
    StringArray csdLines;
    csdLines.addLines (csdFile.loadFileAsString());
    
    for (auto line : csdLines)
    {
        ValueTree temp ("temp");
        CabbageWidgetData::setWidgetState (temp, line, 0);
        
        if (CabbageWidgetData::getStringProp (temp, CabbageIdentifierIds::type) == CabbageWidgetTypes::form)
        {
            if(info == "id")
                return CabbageWidgetData::getStringProp (temp, CabbageIdentifierIds::pluginid);
            else if(info == "manufacturer")
                return CabbageWidgetData::getStringProp (temp, CabbageIdentifierIds::manufacturer);
        }
        
    }
    
    return String();
}
