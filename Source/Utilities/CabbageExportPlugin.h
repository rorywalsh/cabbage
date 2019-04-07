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

#pragma once


#include "../CabbageCommonHeaders.h"
#include "../LookAndFeel/CabbageIDELookAndFeel.h"
#ifdef CabbagePro
#include "encrypt.h"
#endif
class PluginExporter
{
CabbageIDELookAndFeel lookAndFeel;

public:
    PluginExporter():lookAndFeel() {}

    long cabbageFindPluginId (unsigned char* buf, size_t len, const char* s);
    int setUniquePluginId (File binFile, File csdFile, String pluginId, bool isRSRC=false);
    void writePluginFileToDisk (File fc, File csdFile, File VSTData, String fileExtension, String pluginId, String type, bool encrypt = false);
    void addFilesToPluginBundle (File csdFile, File exportDir);
    void exportPlugin (String type, File csdFile, String pluginId, String destination="", bool promptForFilename = true, bool encrypt = false);

    String encodeString (File csdFile)
    {
#ifdef CabbagePro

        StringArray csdLines;
        csdLines.addLines(csdFile.loadFileAsString());
        int startOfCsoundCode = 0;

        for ( int i = 0 ; i < csdLines.size(); i++)
        {
            if(csdLines[i].contains("</Cabbage>"))
                startOfCsoundCode = i+1;

        }

        const int numberOfLines = csdLines.size();
        StringArray cabbageSection = csdLines;
        cabbageSection.removeRange(startOfCsoundCode, csdLines.size()-startOfCsoundCode);
        csdLines.removeRange(0, startOfCsoundCode);
        const String encodedText = Encrypt::encode(csdLines.joinIntoString("\n"));
        csdLines = cabbageSection;
        csdLines.add("\n");
        csdLines.add("<EncryptedSection>\n"+encodedText+"\n</EncryptedSection>");
        return csdLines.joinIntoString("\n");
#endif
        return "";
    }
    static String decodeString (File csdFile)
    {
#ifdef CabbagePro
        return Encrypt::decode(csdFile);
#endif
        return "";
    }


};


