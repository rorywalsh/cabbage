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

    String encodeString (const String& plain)
    {
        RSAKey publicKey {"5,5a21cd11b4ca9a97a11defd6501012a2a5cddb673d06e61232301abcc0cd84e5"};
        
        CharPointer_UTF8 utf8 = plain.toUTF8();
        CharPointer_UTF8::CharType* utf8Address = utf8.getAddress();
        MemoryBlock plainMemoryBlock (utf8Address, utf8.sizeInBytes());
        
        BigInteger sourceInteger;
        sourceInteger.loadFromMemoryBlock (plainMemoryBlock);
        
        if (!sourceInteger.isZero())
        {
            // Encode
            BigInteger encodedInteger (sourceInteger);
            publicKey.applyToValue (encodedInteger);
            
            MemoryBlock encodedMemoryBlock = encodedInteger.toMemoryBlock();
            
            return encodedMemoryBlock.toBase64Encoding();
        }
        else
        {
            return String::empty;
        }
    }
    
    String decodeString (const String& encoded)
    {
        RSAKey privateKey {"361447d76c798ff493dec31a3009a4c7459113493da51955861f92f0dc14260d,5a21cd11b4ca9a97a11defd6501012a2a5cddb673d06e61232301abcc0cd84e5"};
        
        MemoryBlock encodedMemoryBlock;
        encodedMemoryBlock.fromBase64Encoding (encoded);
        
        BigInteger encodedInteger;
        encodedInteger.loadFromMemoryBlock (encodedMemoryBlock);
        
        if (!encodedInteger.isZero())
        {
            // Decode
            BigInteger decodedInteger (encodedInteger);
            privateKey.applyToValue (decodedInteger);
            
            MemoryBlock decodedMemoryBlock = decodedInteger.toMemoryBlock();
            
            return decodedMemoryBlock.toString();
        }
        else
        {
            return String::empty;
        }
    }
};


