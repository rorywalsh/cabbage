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



//=================================================================================================================
CabbageDocumentWindow::CabbageDocumentWindow (String name)  : DocumentWindow(name,
            Colours::lightgrey,
            DocumentWindow::allButtons)
{
    setTitleBarButtonsRequired(DocumentWindow::allButtons, false);
    setUsingNativeTitleBar (true);
    setResizable(true, true);
    centreWithSize (getWidth(), getHeight());
    setVisible (true);

	initSettings();
    setContentOwned (content = new CabbageContentComponent(this, cabbageSettings), true);
    content->propertyPanel->setVisible(false);
	cabbageSettings->addChangeListener(content);
    setMenuBar(this, 25);
    getMenuBarComponent()->setLookAndFeel(getContentComponent()->lookAndFeel);

    content->createAudioGraph();

    if(cabbageSettings->getUserSettings()->getIntValue("OpenMostRecentFileOnStartup")==1)
    {
        cabbageSettings->updateRecentFilesList();
        content->openFile(cabbageSettings->getMostRecentFile().getFullPathName());
    }

    setApplicationCommandManagerToWatch(&commandManager);
    commandManager.registerAllCommandsForTarget(this);
    addKeyListener(commandManager.getKeyMappings());

#if JUCE_MAC
    MenuBarModel::setMacMainMenu (this, nullptr, "Open Recent");
#endif
	setLookAndFeel(&getContentComponent()->getLookAndFeel());
	lookAndFeelChanged();
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
    //lookAndFeel->refreshLookAndFeel(cabbageSettings->getValueTree());
}

CabbageDocumentWindow::~CabbageDocumentWindow()
{
    setMenuBar(nullptr);

#if JUCE_MAC
    MenuBarModel::setMacMainMenu (nullptr);
#endif
    cabbageSettings->setProperty("audioSetup", getContentComponent()->getAudioDeviceSettings());
	cabbageSettings->closeFiles();
	
    //lookAndFeel = nullptr;

}

void CabbageDocumentWindow::buttonClicked(Button* button)
{
	if(const ToolbarButton* toolbarButton = dynamic_cast<ToolbarButton*>(button))
	{
		CabbageUtilities::debug(toolbarButton->getName());
	}
}
//=======================================================================================
CabbageContentComponent* CabbageDocumentWindow::getContentComponent()
{
    return content;
}

//==============================================================================
void CabbageDocumentWindow::showSettingsDialog()
{
	content->showSettingsDialog();
}

//==============================================================================
void CabbageDocumentWindow::createNewProject()
{
    content->createNewProject();
}
//==============================================================================
void CabbageDocumentWindow::askUserToOpenFile()
{
    content->openFile();
}

//==============================================================================
void CabbageDocumentWindow::saveDocument(bool saveAs)
{
	content->saveDocument(saveAs);
}

//==============================================================================
void CabbageDocumentWindow::runCode()
{
	content->runCode();
}

void CabbageDocumentWindow::stopCode()
{
    content->stopCode();
}

//==============================================================================
bool CabbageDocumentWindow::closeAllDocuments (bool askUserToSave)
{
    // return openDocumentManager.closeAll (askUserToSave);
    return true;
}

bool CabbageDocumentWindow::closeAllMainWindows()
{
    //return server != nullptr || mainWindowList.askAllWindowsToClose();
    return true;
}
void CabbageDocumentWindow::closeButtonPressed()
{
    JUCEApplicationBase::quit();
}