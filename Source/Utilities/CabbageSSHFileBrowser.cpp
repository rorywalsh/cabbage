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
CabbageSSHFileBrowser::CabbageSSHFileBrowser(String ip, String homeDir)
    :ipAddress(ip), homeDirectory(homeDir)
{
    setOpaque (true);

    addAndMakeVisible (launchButton);
    launchButton.setButtonText ("Launch Child Process");
    launchButton.addListener (this);

    addAndMakeVisible (pingButton);
    pingButton.setButtonText ("Send Ping");
    pingButton.addListener (this);

    addAndMakeVisible (killButton);
    killButton.setButtonText ("Kill Child Process");
    killButton.addListener (this);
    addAndMakeVisible (filesListBox);
    filesListBox.setColour(ListBox::ColourIds::backgroundColourId, Colour(20, 20, 20));
    filesListBox.setModel(this);
    filePath.add(homeDirectory.trim());
    String command2("ssh " + ipAddress + " ls -l "+homeDirectory);
    launchChildProcess(command2);

}

CabbageSSHFileBrowser::~CabbageSSHFileBrowser()
{
}

void CabbageSSHFileBrowser::paint (Graphics& g)
{
    g.fillAll(Colours::grey);
}

void CabbageSSHFileBrowser::resized()
{
    Rectangle<int> area (getLocalBounds());
    Rectangle<int> top (area.removeFromTop (40));
    launchButton.setBounds (top.removeFromLeft (180).reduced (8));
    pingButton.setBounds (top.removeFromLeft (180).reduced (8));
    killButton.setBounds (top.removeFromLeft (180).reduced (8));
    filesListBox.setBounds (area.reduced (8));
}

int CabbageSSHFileBrowser::getNumRows()
{
    return filesAndFoldersToDisplay.size();
}

void CabbageSSHFileBrowser::listBoxItemDoubleClicked(int row, const MouseEvent &e)
{
    bool isDirectory;
    if(filesAndFoldersToDisplay[row].substring(0, 1)=="d")
        isDirectory = true;

    const String name = filesAndFoldersToDisplay[row].toUTF8().getAddress();
    if(isDirectory)
    {
        filePath.add("/" + getFileOrFolderName(name));
        String command("ssh " + ipAddress + " ls -l "+filePath.joinIntoString(""));
        CabbageUtilities::debug(command);
        launchChildProcess(command);
    }
    else
    {

        String command("ssh " + ipAddress + " ls -l "+filePath.joinIntoString("")+"/"+getFileOrFolderName(name));
        CabbageUtilities::debug(command);
    }

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
        g.setColour(Colours::whitesmoke);

    else if(filesAndFoldersToDisplay[rowNumber].substring(0, 1)=="d")
        g.setColour(Colours::lime);
    else
        g.setColour(Colours::cornflowerblue);

    g.drawFittedText(filesAndFoldersToDisplay[rowNumber], Rectangle<int> (width, height), Justification::centredLeft, 0);
}


// invoked by the 'launch' button.
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

// invoked by the 'ping' button.
void CabbageSSHFileBrowser::pingChildProcess()
{

}

// invoked by the 'kill' button.
void CabbageSSHFileBrowser::killChildProcess()
{
    childProcess.kill();
}

void CabbageSSHFileBrowser::buttonClicked (Button* button)
{
    if (button == &pingButton)    pingChildProcess();
    if (button == &killButton)    killChildProcess();
}

