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
    PropertiesFile* settings;
public:
    PluginExporter():lookAndFeel(), settings(nullptr){}
    void settingsToUse(PropertiesFile* cabSettings){   settings = cabSettings; }

    long cabbageFindPluginId (unsigned char* buf, size_t len, const char* s);
    int setUniquePluginId (File binFile, File csdFile, String pluginId);
    void writePluginFileToDisk (File fc, File csdFile, File VSTData, String fileExtension, String pluginId, String type, bool encrypt = false);
    void addFilesToPluginBundle (File csdFile, File exportDir);
    void exportPlugin (String type, File csdFile, String pluginId, String destination="", bool promptForFilename = true, bool encrypt = false);

    String encodeString (File csdFile)
    {
#ifdef CabbagePro
        NamedValueSet tagPositions;
        String csdText = csdFile.loadFileAsString();
        StringArray originalCsdText;
        originalCsdText.addLines(csdText);
        
        for( int i = 0 ; i < originalCsdText.size() ; i++)
        {
            if(originalCsdText[i].contains("<CsInstruments>"))
                tagPositions.set("<CsInstruments>", i);
            if(originalCsdText[i].contains("</CsInstruments>"))
                tagPositions.set("</CsInstruments>", i+1);
            if(originalCsdText[i].contains("<Cabbage>"))
                tagPositions.set("<Cabbage>", i);
            if(originalCsdText[i].contains("</Cabbage>"))
                tagPositions.set("</Cabbage>", i+1);
            if(originalCsdText[i].contains("</CsScore>"))
                tagPositions.set("</CsScore>", i+1);
            if(originalCsdText[i].contains("<CsScore>"))
                tagPositions.set("<CsScore>", i);
        }
        
        const int numberOfOrcLines = int(tagPositions.getWithDefault("</CsInstruments>", 0)) - int(tagPositions.getWithDefault("<CsInstruments>", -1));
        
        const int numberOfScoreLines = int(tagPositions.getWithDefault("</CsScore>", 0)) - int(tagPositions.getWithDefault("<CsScore>", -1));
        
        String orc = originalCsdText.joinIntoString("\n", tagPositions.getWithDefault("<CsInstruments>", -1), numberOfOrcLines);
        String sco = originalCsdText.joinIntoString("\n", tagPositions.getWithDefault("<CsScore>", -1), numberOfScoreLines);

        StringArray orcLines, scoLines;
        orcLines.addLines(orc);
        scoLines.addLines(sco);
        
        //leave header section in place
        std::vector<int> headerDefs;
        for ( int i = 0 ; i < orcLines.size(); i++)
        {
            if(orcLines[i].contains("0dbfs"))
                headerDefs.push_back(i+1);
            else if(orcLines[i].contains("ksmps"))
                headerDefs.push_back(i+1);
            else if(orcLines[i].contains("nchnls"))
                headerDefs.push_back(i+1);
            else if(orcLines[i].contains("nchnls_i"))
                headerDefs.push_back(i+1);
            if(headerDefs.size() == (orc.indexOf("nchnls_i")==-1 ? 3 : 4))
                break;
        }
        
        if(headerDefs.size() < 3)
        {
            CabbageUtilities::showMessage(
                                          "Please make sure that your orc header section contains assignments for ksmps, nchnls and 0dbfs.", &lookAndFeel);
            return "";
        }
        //grab orc / and sco and encrypt..
        auto start = max_element(std::begin(headerDefs), std::end(headerDefs));
        const int lineToStartEncodingFrom = std::distance( headerDefs.begin(), start );
        const String orcToEncode = orcLines.joinIntoString("\n", headerDefs[lineToStartEncodingFrom], orcLines.size()-headerDefs[lineToStartEncodingFrom]-1);
        const String headerSection = orcLines.joinIntoString("\n", 0, headerDefs[lineToStartEncodingFrom])+"\n";
        const String encodedOrc = headerSection + "\ninstr 1\nendin\n\n/* Encrypted orc\n" + Encrypt::encode(orcToEncode) + "\nEnd of encrypted orc */\n</CsInstruments>\n";
        
        const String scoreToEncode = scoLines.joinIntoString("\n", 1, scoLines.size()-2);
        
        const String encodedSco = "<CsScore>\nf0 z\n/* Encrypted sco\n" + Encrypt::encode(scoreToEncode) + "\nEnd of encrypted sco */\n</CsScore>\n";

        //replace sco
        originalCsdText.removeRange(tagPositions.getWithDefault("<CsScore>", -1), numberOfScoreLines);
        originalCsdText.insert(tagPositions.getWithDefault("<CsScore>", -1), encodedSco);
        //replace orc
        originalCsdText.removeRange(tagPositions.getWithDefault("<CsInstruments>", -1), numberOfOrcLines);
        originalCsdText.insert(tagPositions.getWithDefault("<CsInstruments>", -1), encodedOrc);
        
        return originalCsdText.joinIntoString("\n");

#endif
        return "";
    }

};


