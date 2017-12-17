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

#ifndef CABBAGEAPPLICATION_H_INCLUDED
#define CABBAGEAPPLICATION_H_INCLUDED

#include "CabbageCommonHeaders.h"


class CabbageProjectWindow;
class CabbageMainDocumentWindow;

//==============================================================================
class Cabbage  : public JUCEApplication
{
public:
    //==============================================================================
    Cabbage();

    static Cabbage& getApp();
    void changeListenerCallback (ChangeBroadcaster* source) override;
    const String getApplicationName() override
    {
        return ProjectInfo::projectName;
    }
    const String getApplicationVersion() override
    {
        return ProjectInfo::versionString;
    }
    bool moreThanOneInstanceAllowed() override
    {
        return true;
    }

    void timerCallback();
    void initialiseBasics();
    void shutdown();
    bool isRunningCommandLine = false;
    void initialise (const String& commandLine) override;
    void systemRequestedQuit() override
    {
        quit();
    }
    void anotherInstanceStarted (const String& commandLine) override  {}


private:
    ScopedPointer<CabbageDocumentWindow> documentWindow;
};




#endif  // CABBAGEAPPLICATION_H_INCLUDED
