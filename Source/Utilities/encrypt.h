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

    static const String decode(const File csdFile)
    {
        const String encryptionStartTag = "<EncryptedSection>";
        const String encryptionEndTag = "</EncryptedSection>";
        const String csdFileText = csdFile.loadFileAsString();
        const String encodedText = csdFileText.substring(csdFileText.indexOf(encryptionStartTag)+encryptionStartTag.length(),
                                                         csdFileText.indexOf(encryptionEndTag));

        const String key ="332c5fcb220f6fcd";
        MemoryBlock memoryBlock;
        memoryBlock.fromBase64Encoding (encodedText);

        BlowFish blowFish (key.toUTF8(), (int) key.getNumBytesAsUTF8());
        blowFish.decrypt (memoryBlock);
        const String decodedText = memoryBlock.toString();
        return decodedText;


    }
};
