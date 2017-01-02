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

#ifndef CABBAGESSHFILEBROWSER_H_INCLUDED
#define CABBAGESSHFILEBROWSER_H_INCLUDED

#include "../CabbageCommonHeaders.h"

//==============================================================================
class CabbageSSHFileBrowser   : public Component,
    private Button::Listener,
    public ListBoxModel
{
public:
    CabbageSSHFileBrowser(String ip, String homeDir);
    ~CabbageSSHFileBrowser();
    void paint (Graphics& g) override;
    void resized() override;
    int getNumRows();
    void listBoxItemDoubleClicked(int row, const MouseEvent &e);

    void paintListBoxItem (int rowNumber, Graphics& g,
                           int width, int height, bool rowIsSelected);

    void selectedRowsChanged (int /*lastRowselected*/) {};
    ListBox filesListBox;;
    // Appends a message to the textbox that's shown in the demo as the console
    void logMessage (const String& message);
    void launchChildProcess(const String command);
    void pingChildProcess();
    void killChildProcess();

private:
    String ipAddress, homeDirectory;
    TextButton launchButton, pingButton, killButton;
    const String getFileOrFolderName(String text);
    //TextEditor testResultsBox;
    ChildProcess childProcess;
    StringArray filesAndFoldersToDisplay, filePath;

    void buttonClicked (Button* button) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabbageSSHFileBrowser)
};

#endif  // CABBAGESSHFILEBROWSER_H_INCLUDED
