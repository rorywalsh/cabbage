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

#include "JuceHeader.h"
#include "Application/CabbageDocumentWindow.h"
#include "Cabbage.h"
#include "Utilities/CabbageUtilities.h"


//==============================================================================
Cabbage::Cabbage()
{
}
//==============================================================================
void Cabbage::initialise (const String& commandLine)
{
    documentWindow.reset (new CabbageDocumentWindow (getApplicationName(), getCommandLineParameters()));

    if (commandLine.isEmpty())
    {
        isRunningCommandLine = false;
    }

    if (sendCommandLineToPreexistingInstance())
    {
        return;
    }
    
    SystemStats::setApplicationCrashHandler ([](void*) {        
        std::ofstream myfile;
        String rootDir = File::getSpecialLocation(File::currentExecutableFile).getParentDirectory().getFullPathName();
        myfile.open(String(rootDir+"/CabbageDump.txt").toStdString());
        myfile << SystemStats::getStackBacktrace();
        myfile.close();        
    });

}

void Cabbage::anotherInstanceStarted (const String& commandLine)
{
    if (CabbageDocumentWindow* mainApp = dynamic_cast<CabbageDocumentWindow*> (documentWindow.get()))
    {
        if (!commandLine.contains("--export-"))
        {
            mainApp->openFile(commandLine);
        }
    }
}

//==============================================================================
void Cabbage::shutdown()
{
    if (! isRunningCommandLine)
        Logger::writeToLog ("Shutdown");
}


//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (Cabbage)
