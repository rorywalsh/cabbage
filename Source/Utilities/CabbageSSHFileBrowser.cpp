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

#include "CabbageSSHFileBrowser.h"
#include "../Application/CabbageContentComponent.h"

CabbageSSHFileBrowser::CabbageSSHFileBrowser(String ip, String homeDir,CabbageContentComponent* owner, String mode, String currentFileFilePath)
    :ipAddress(ip), homeDirectory(homeDir), owner(owner), currentLocalFilePath(currentFileFilePath)
{
    setOpaque (true);

    addAndMakeVisible (currentDirectoryLabel);

    addAndMakeVisible (filesListBox);
    filesListBox.setColour(ListBox::ColourIds::backgroundColourId, Colour(20, 20, 20));
    filesListBox.setModel(this);
    filePath.add(homeDirectory.trim());
    
	const String command("ssh " + ipAddress + " ls -l "+homeDirectory);
    launchChildProcess(command);
	
	if(mode=="save")
	{
		currentDirectoryLabel.setReadOnly(false);
		labelPrefix = "Save file as: ";
	}
	else
		currentDirectoryLabel.setReadOnly(true);

	currentDirectoryLabel.setFont(Font("DejaVu Sans Mono", 15, 0));
	currentDirectoryLabel.setText(labelPrefix + homeDirectory, dontSendNotification);
	currentDirectoryLabel.setColour(Label::textColourId, Colours::whitesmoke);
	currentDirectoryLabel.setColour(Label::backgroundColourId, Colour(20, 20, 20));
	currentDirectoryLabel.setColour(Label::ColourIds::textWhenEditingColourId, Colour(200, 200, 200));
	currentDirectoryLabel.setColour(TextEditor::ColourIds::highlightedTextColourId, Colour(200, 200, 200));
	currentDirectoryLabel.setColour(TextEditor::ColourIds::highlightColourId, Colour(130, 130, 130));	
	currentDirectoryLabel.addListener(this);
}

CabbageSSHFileBrowser::~CabbageSSHFileBrowser()
{
}

void CabbageSSHFileBrowser::textEditorReturnKeyPressed(TextEditor &editor)
{
	String command("scp " + currentLocalFilePath + " " + ipAddress + ":" + editor.getText().substring(labelPrefix.length()));
	launchChildProcess(command);
	delete this->getParentComponent();
}

void CabbageSSHFileBrowser::paint (Graphics& g)
{
    g.fillAll(Colours::grey);
}

void CabbageSSHFileBrowser::resized()
{
    Rectangle<int> area (getLocalBounds());
	currentDirectoryLabel.setBounds(area.removeFromTop(30).reduced(8).withHeight(25));
    filesListBox.setBounds (area.reduced (8));
}

int CabbageSSHFileBrowser::getNumRows()
{
    return filesAndFoldersToDisplay.size();
}

void CabbageSSHFileBrowser::listBoxItemDoubleClicked(int row, const MouseEvent &e)
{
    bool isDirectory = false;
	CabbageUtilities::debug(filesAndFoldersToDisplay[row]);
    if(filesAndFoldersToDisplay[row].substring(0, 1)=="d")
        isDirectory = true;

    const String name = filesAndFoldersToDisplay[row].toUTF8().getAddress();

	if(name=="Double-click to go back a directory..")
	{
		filePath.remove(filePath.size()-1);
		launchChildProcess(getSSHLsCommand(filePath.joinIntoString("")));
		currentDirectoryLabel.setText(labelPrefix + filePath.joinIntoString(""), dontSendNotification);
	}
	else if(isDirectory)
    {
		filePath.add("/" + getFileOrFolderName(name));
		launchChildProcess(getSSHLsCommand(filePath.joinIntoString("")));
		currentDirectoryLabel.setText(labelPrefix + filePath.joinIntoString(""), dontSendNotification);
    }
    else
    {
		String command;
        FileChooser fc ("Select file name and location", File::getSpecialLocation(File::SpecialLocationType::userHomeDirectory));
		CabbageIDELookAndFeel lookAndFeel;
        if (fc.browseForFileToSave(false))
        {
			File newFile = fc.getResult();
            if(fc.getResult().existsAsFile())
            {
                const int result = CabbageUtilities::showYesNoMessage("Do you wish to overwrite\nexiting file?", &lookAndFeel);
				command = ("scp " + ipAddress + ":"+filePath.joinIntoString("")+"/"+getFileOrFolderName(name) + " "
						       + newFile.getFullPathName());
				if(result==0)
                {
					launchChildProcess(command);
					owner->openFile(newFile.getFullPathName());
					delete this->getParentComponent();
				}
            }
            else
            {
				command = ("scp " + ipAddress + ":"+filePath.joinIntoString("")+"/"+getFileOrFolderName(name) + " "
						       + newFile.getFullPathName());
				launchChildProcess(command);
				owner->openFile(newFile.getFullPathName());
				delete this->getParentComponent();
            }
        }		
    }
}

const String CabbageSSHFileBrowser::getSSHLsCommand(String directory)
{
	return String("ssh " + ipAddress + " ls -l "+directory);	
}

const String CabbageSSHFileBrowser::getFileOrFolderName(String text)
{
    StringArray tokens;
    const char* str = text.toUTF8().getAddress();
    do
    {
        const char *begin = str;

        while(*str != ' ' && *str)
            str++;
        const String toke(string(begin, str+1));
        if(toke.trim().isNotEmpty())
        {
            tokens.add(toke.trim());
        }
    }
    while (0 != *str++);

    tokens.removeRange(0, 8);
    return tokens.joinIntoString(" ");
}

void CabbageSSHFileBrowser::paintListBoxItem (int rowNumber, Graphics& g,
        int width, int height, bool rowIsSelected)
{
    if (rowIsSelected)
        g.fillAll (Colour(50, 50, 50));
    else
        g.fillAll(Colour(20, 20, 20));

    g.setFont(Font(String("DejaVu Sans Mono"), 15, 0));

    if(filesAndFoldersToDisplay[rowNumber]=="Double-click to go back a directory..")
        g.setColour(Colours::lime.darker());

    else if(filesAndFoldersToDisplay[rowNumber].substring(0, 1)=="d")
        g.setColour(Colours::lime);
    else
        g.setColour(Colours::cornflowerblue);

    g.drawFittedText(filesAndFoldersToDisplay[rowNumber], Rectangle<int> (width, height).withLeft(8), Justification::centredLeft, 0);
}


void CabbageSSHFileBrowser::launchChildProcess(String command)
{
    childProcess.start(command);
    StringArray files;
    files.addLines(childProcess.readAllProcessOutput());

    for( int i = files.size() ; i >= 0 ; i-- )
    {
        CabbageUtilities::debug(getFileOrFolderName(files[i]));
        if(files[i].length()<24)
            files.remove(i);

        if(getFileOrFolderName(files[i]).contains(".") && !getFileOrFolderName(files[i]).contains(".csd"))
        {
            CabbageUtilities::debug(getFileOrFolderName(files[i]));
            files.remove(i);
        }
    }

    filesAndFoldersToDisplay = files;
    filesAndFoldersToDisplay.insert(0, "Double-click to go back a directory..");

    filesListBox.updateContent();
}

