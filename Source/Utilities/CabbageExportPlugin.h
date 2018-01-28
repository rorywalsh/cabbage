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
    int setUniquePluginId (File binFile, File csdFile, String pluginId);
    void writePluginFileToDisk (File fc, File csdFile, File VSTData, String fileExtension, String pluginId, bool encrypt = false);
    void addFilesToPluginBundle (File csdFile, File exportDir);
    void exportPlugin (String type, File csdFile, String pluginId, bool encrypt = false);

    String encodeString (const String &textToEncrypt)
    {
#ifdef CabbagePro
        return encode(textToEncrypt);
#endif
    }
    static String decodeString (const String &textToDecrypt)
    {
#ifdef CabbagePro
        return decode(textToDecrypt);
#endif
    }


};


