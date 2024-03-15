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

class Encrypt
{
public:
    static const String encode(const String &textToEncrypt)
    {
        const String key ="332c5fcb220f6fcd";
        MemoryBlock memoryBlock;
        memoryBlock.loadFromHexString (String::toHexString(textToEncrypt.toUTF8(), (int) textToEncrypt.getNumBytesAsUTF8()));

        BlowFish blowFish (key.toUTF8(), (int) key.getNumBytesAsUTF8());
        blowFish.encrypt (memoryBlock);

        return memoryBlock.toBase64Encoding();
    }

    static const String decode(const File csdFile, String section)
    {
        const String csdFileText = csdFile.loadFileAsString();
        
        const int encryptedOrcStartIndex = csdFileText.indexOf("/* Encrypted orc")  == -1 ? -1 : csdFileText.indexOf("/* Encrypted orc") + 16;
        const int encryptedOrcEndIndex = csdFileText.indexOf("End of encrypted orc */");
        const int encryptedScoStartIndex = csdFileText.indexOf("/* Encrypted sco") == -1 ? -1 : csdFileText.indexOf("/* Encrypted sco") + 16;
        const int encryptedScoEndIndex = csdFileText.indexOf("End of encrypted sco */");
                                                                  
                                                                  
                                                        
        //if files was not encrypted..
        if(encryptedOrcStartIndex<0)
           return "";


        const String encodedOrc = csdFileText.substring(encryptedOrcStartIndex, encryptedOrcEndIndex);
        const String encodedSco = csdFileText.substring(encryptedScoStartIndex, encryptedScoEndIndex);
        
        const String key ="332c5fcb220f6fcd";
        MemoryBlock orcMemoryBlock;
        orcMemoryBlock.fromBase64Encoding (encodedOrc);
        BlowFish orcBlowFish (key.toUTF8(), (int) key.getNumBytesAsUTF8());
        orcBlowFish.decrypt (orcMemoryBlock);
        
        MemoryBlock scoMemoryBlock;
        scoMemoryBlock.fromBase64Encoding (encodedSco);
        BlowFish scoBlowFish (key.toUTF8(), (int) key.getNumBytesAsUTF8());
        scoBlowFish.decrypt (scoMemoryBlock);
        
        if(section == "orc")
            return orcMemoryBlock.toString();
        else
            return scoMemoryBlock.toString();
        
    }
};
