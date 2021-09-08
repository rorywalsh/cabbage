/*
 ==============================================================================
 
 CabbageExportPlugin.cpp
 Created: 27 Nov 2017 2:37:25pm
 Author:  rory
 
 ==============================================================================
 */

#include "CabbageExportPlugin.h"


//===============   methods for exporting plugins ==============================
void PluginExporter::exportPlugin (String type, File csdFile, String pluginId, String destination, bool promptForFilename, bool encrypt)
{
    if(csdFile.hasFileExtension(".csd") == false)
        return;
    
    
    if(csdFile.existsAsFile())
    {
        
        String pluginFilename, fileExtension;
        File thisFile = File::getSpecialLocation (File::currentApplicationFile);
#if !defined(WIN32) && !defined(MACOSX)
        String currentApplicationDirectory = "/usr/bin";
#else
        String currentApplicationDirectory = thisFile.getParentDirectory().getFullPathName();
#endif
        
        
        if (SystemStats::getOperatingSystemType() == SystemStats::OperatingSystemType::Linux)
        {
            if(type == "Standalone")
                fileExtension = "";
            else
                fileExtension = "so";
        }
        else if ((SystemStats::getOperatingSystemType() & SystemStats::MacOSX) != 0)
        {
            if(type == "Standalone")
                fileExtension = "app";
            else if(type.contains("VST3"))
                fileExtension = "vst3";
            else if(type.contains("VST"))
                fileExtension = "vst";
            else
                fileExtension = "component";
            
            currentApplicationDirectory = thisFile.getFullPathName() + "/Contents";
        }
        else
        {
            if(type == "Standalone")
                fileExtension = "exe";
            else if(type.contains("VST3"))
                fileExtension = "vst3";
            else
                fileExtension = "dll";
        }
        
#if CabbagePro && JUCE_MAC
        const String pluginDesc = String(JucePlugin_Manufacturer);
#else
        const String pluginDesc = String(JucePlugin_Desc);
#endif
        
        if (type == "VSTi" || type == "AUi" || type == "VST3i")
            pluginFilename = currentApplicationDirectory + String ("/"+pluginDesc+"Synth." + fileExtension);
        else  if (type == "VST" || type == "AU" || type == "VST3")
            pluginFilename = currentApplicationDirectory + String ("/"+pluginDesc+"Effect." + fileExtension);
        else  if (type == "AUMIDIFx")
            pluginFilename = currentApplicationDirectory + String ("/"+pluginDesc+"MIDIEffect." + fileExtension);
        else if (type.contains (String ("LV2-ins")))
            pluginFilename = currentApplicationDirectory + String ("/"+pluginDesc+"SynthLV2." + fileExtension);
        else if (type.contains (String ("LV2-fx")))
            pluginFilename = currentApplicationDirectory + String ("/"+pluginDesc+"EffectLV2." + fileExtension);
        else if (type == "VCVRack")
        {
            fileExtension = "";
            pluginFilename = currentApplicationDirectory+"/CabbageRack/";
            if(!File(pluginFilename).exists())
                pluginFilename = File::getSpecialLocation (File::currentApplicationFile).getParentDirectory().getFullPathName()+"/CabbageRack/";
        }
        else  if (type == "FMOD")
        {
            fileExtension = ((SystemStats::getOperatingSystemType() & SystemStats::MacOSX) != 0) ? String("dylib") : String("dll");
            pluginFilename = currentApplicationDirectory + (((SystemStats::getOperatingSystemType() & SystemStats::MacOSX) != 0) ? String("/fmod_csound.dylib") : String("/fmod_csound64.dll"));
        }
        else  if (type == "FMODFx")
        {
            fileExtension = ((SystemStats::getOperatingSystemType() & SystemStats::MacOSX) != 0) ? String("dylib") : String("dll");
            pluginFilename = currentApplicationDirectory + (((SystemStats::getOperatingSystemType() & SystemStats::MacOSX) != 0) ? String("/fmod_csound_fx.dylib") : String("/fmod_csound64_fx.dll"));
            
        }
        else  if (type == "Standalone")
        {
            if (SystemStats::getOperatingSystemType() == SystemStats::OperatingSystemType::Linux)
                pluginFilename = currentApplicationDirectory + "/" + pluginDesc;
            else
                pluginFilename = currentApplicationDirectory + "/" + pluginDesc + "." + fileExtension;
        }
        
        File VSTData (pluginFilename);
        
        if (!VSTData.exists())
        {
            CabbageUtilities::showMessage("Error", pluginFilename + " cannot be found? It should be in the Cabbage root folder", &lookAndFeel);
            return;
        }
        
        //if batch converting plugins
        if(File(destination).exists())
        {
            const String newFile = destination+"/"+csdFile.getFileName();
            writePluginFileToDisk(newFile, csdFile, VSTData, fileExtension, pluginId, type,
                                  encrypt);
        }
        else if(promptForFilename == false)
        {
            String newFile = destination;
            if (newFile == "")
            {
                newFile = csdFile.getParentDirectory().getFullPathName()+"/"+csdFile.getFileNameWithoutExtension();
            }
            writePluginFileToDisk(newFile, csdFile, VSTData, fileExtension, pluginId, type,
                                  encrypt);
            
        }
        else
        {
            
            FileChooser fc("Save file as..", csdFile.getParentDirectory().getFullPathName(), "*." + fileExtension,
                           CabbageUtilities::shouldUseNativeBrowser());
            
            if (fc.browseForFileToSave(false))
            {
                if (fc.getResult().existsAsFile())
                {
                    const int result = CabbageUtilities::showYesNoMessage("Do you wish to overwrite\nexiting file?",
                                                                          &lookAndFeel);
                    
                    if (result == 1)
                    {
                        File oldFile(fc.getResult().getFullPathName());
                        oldFile.moveToTrash();
                        writePluginFileToDisk(fc.getResult(), csdFile, VSTData, fileExtension, pluginId, type,
                                              encrypt);
                    }
                }
                else
                    writePluginFileToDisk(fc.getResult(), csdFile, VSTData, fileExtension, pluginId, type,
                                          encrypt);
                
            }
        }
    }
}


void PluginExporter::writePluginFileToDisk (File fc, File csdFile, File VSTData, String fileExtension, String pluginId, String type,  bool encrypt)
{
    
    File exportedPlugin (fc.withFileExtension (fileExtension).getFullPathName());
    
    //vcv rack export is the same on all platforms..
    if(type=="VCVRack")
    {
        if(!VSTData.copyDirectoryTo(exportedPlugin))
            jassertfalse;
        File rackCsdFile(exportedPlugin.getFullPathName()+"/"+exportedPlugin.getFileName()+".csd");
        //csdFile.moveFileTo(rackCsdFile);
        rackCsdFile.replaceWithText(csdFile.loadFileAsString());
        File oldFile(exportedPlugin.getFullPathName()+"/CabbageRack.csd");
        oldFile.deleteFile();
        //bundle all auxiliary files
        addFilesToPluginBundle(csdFile, exportedPlugin);
        
        File jsonFile(exportedPlugin.getFullPathName()+"/plugin.json");
        StringArray jsonLines;
        jsonLines.addLines(jsonFile.loadFileAsString());
        
        for ( int i = 0 ; i < jsonLines.size() ; i++)
        {
            //replace slugs and name with plugin name
            if(jsonLines[i].contains("\"slug\": \"CabbageRack\","))
                jsonLines.getReference(i) = "\"slug\": \"" + fc.getFileNameWithoutExtension() + "\",";
            if(jsonLines[i].contains("\"name\": \"CabbageRack\","))
                jsonLines.getReference(i) = "\"name\": \""+fc.getFileNameWithoutExtension()+"\",";
            if(jsonLines[i].contains("\"brand\": \"CabbageRack\","))
                jsonLines.getReference(i) = "\"brand\": \""+fc.getFileNameWithoutExtension()+"\",";
            
        }
        
        jsonFile.replaceWithText(jsonLines.joinIntoString("\n"));
        
        return;
    }
    
    //plugin files on OSX are bundles, so we need to recursively delete all files in bundle
    if((SystemStats::getOperatingSystemType() & SystemStats::MacOSX) != 0)
    {
        if(File(exportedPlugin).exists() )
        {
            if(!File(exportedPlugin).deleteRecursively())
                jassertfalse;
        }
    }
    
    
    if (!VSTData.copyFileTo (exportedPlugin))
    {
        CabbageUtilities::showMessage ("Error", "Exporting: " + csdFile.getFullPathName() + ", Can't copy plugin to this location. It currently be in use, or you may be trying to install to a system folder you don't have permission to write in. Please try exporting to a different location.", &lookAndFeel);
        return;
    }
    
    
    File exportedCsdFile;
    
    
    if ((SystemStats::getOperatingSystemType() & SystemStats::MacOSX) != 0)
    {
#ifdef CabbagePro
        const String pluginDesc = String(JucePlugin_Manufacturer);
#else
        const String pluginDesc = String(JucePlugin_Desc);
#endif
        if(fileExtension.containsIgnoreCase("component"))
            exportedCsdFile = exportedPlugin.getFullPathName() + String ("/Contents/"+pluginDesc+".csd");
        else if(fileExtension.containsIgnoreCase("vst"))
            exportedCsdFile = exportedPlugin.getFullPathName() + String ("/Contents/") + fc.getFileNameWithoutExtension() + String (".csd");
        else if(fileExtension.containsIgnoreCase("app"))
            exportedCsdFile = exportedPlugin.getFullPathName() + String ("/Contents/") + fc.getFileNameWithoutExtension() + String (".csd");
        
        if(fileExtension.contains("dylib"))
        {
            exportedCsdFile = fc.withFileExtension (".csd").getFullPathName();
            exportedCsdFile.replaceWithText (csdFile.loadFileAsString());
        }
        else
        {
            if(encrypt)
            {
                exportedCsdFile.replaceWithText(encodeString(csdFile));
            }
            else
                exportedCsdFile.replaceWithText (csdFile.loadFileAsString());
            
            File bin (exportedPlugin.getFullPathName() + String ("/Contents/MacOS/"+pluginDesc));
            //if(bin.exists())showMessage("binary exists");
            
            File pl (exportedPlugin.getFullPathName() + String ("/Contents/Info.plist"));
            String newPList = pl.loadFileAsString();
            
            if(fileExtension.containsIgnoreCase("vst") || fileExtension.containsIgnoreCase("app"))
            {
                File pluginBinary (exportedPlugin.getFullPathName() + String ("/Contents/MacOS/") + fc.getFileNameWithoutExtension());
                
                if (bin.moveFileTo (pluginBinary) == false)
                    CabbageUtilities::showMessage ("Error", "Could not copy library binary file. Make sure the two Cabbage .vst files are located in the Cabbage.app folder", &lookAndFeel);
                
                setUniquePluginId (pluginBinary, exportedCsdFile, pluginId);
                
                newPList = newPList.replace (pluginDesc, fc.getFileNameWithoutExtension());
            }
            
            
            String manu(JucePlugin_Manufacturer);
            const String pluginName = "<string>" +manu + ": " + fc.getFileNameWithoutExtension() + "</string>";
            const String toReplace = "<string>"+manu+": CabbageEffectNam</string>";
#ifdef CabbagePro
            //be sure to remove CabbageAudio from plugin plist..
            const String toReplace2 = "<string>CabbageAudio: CabbageEffectNam<string>";
            newPList = newPList.replace (toReplace2, pluginName);
            if(fileExtension.containsIgnoreCase("component"))
                newPList = newPList.replace ("<string>CabbagePlugin</string>", "<string>" + pluginDesc + "</string>");
            else
                newPList = newPList.replace ("<string>CabbagePlugin</string>", "<string>" + fc.getFileNameWithoutExtension() + "</string>");
            
#endif
            newPList = newPList.replace (toReplace, pluginName);
            if(pluginId.isEmpty())
            {
                CabbageUtilities::showMessage ("Error", "The plugin ID identifier in " + csdFile.getFullPathName() + " is empty, or the pluginid identifier string contains a typo. Certain hosts may not recognise your plugin. Please use a unique ID for each plugin.", &lookAndFeel);
                pluginId = "Cab2";
            }
            
            const String auId = "<string>" + pluginId + "</string>";
            newPList = newPList.replace ("<string>RORY</string>", auId);
            
            pl.replaceWithText (newPList);
        }
        //bundle all auxiliary files
        addFilesToPluginBundle(csdFile, exportedCsdFile);
        
    }
    else
    {
        exportedCsdFile = fc.withFileExtension (".csd").getFullPathName();
        if(encrypt)
        {
            exportedCsdFile.replaceWithText (encodeString(csdFile));
        }
        
        else
            exportedCsdFile.replaceWithText (csdFile.loadFileAsString());
        
        setUniquePluginId (exportedPlugin, exportedCsdFile, pluginId);
        
        //bundle all auxiliary files
        addFilesToPluginBundle(csdFile, exportedPlugin);
    }
    
    if(type.containsIgnoreCase("AU"))
    {
        File pluginBinary (exportedPlugin.getFullPathName() + String ("/Contents/Resources/CabbagePlugin.rsrc"));
        setUniquePluginId (pluginBinary, exportedCsdFile, pluginId);
    }
    
#ifdef CabbagePro //bundle and relink Csound
    //removing this for now to see if I can relink manually...
    //    if ((SystemStats::getOperatingSystemType() & SystemStats::MacOSX) != 0 && encrypt)
    //    {
    //        File csoundFramework(File::getSpecialLocation (File::currentApplicationFile).getFullPathName()+"/Contents/CsoundLib64.framework");
    //        if(!csoundFramework.exists())
    //            jassertfalse;
    //
    //        if(!csoundFramework.copyFileTo(File(exportedPlugin.getFullPathName()+"/Contents/Resources/CsoundLib64.framework")))
    //            jassertfalse;
    //
    //        ChildProcess childProc;
    //        String pluginBinaryPath;
    //
    //        if(fileExtension.containsIgnoreCase("vst"))
    //            pluginBinaryPath = exportedPlugin.getFullPathName() + String ("/Contents/MacOS/") + fc.getFileNameWithoutExtension();
    //        else
    //            pluginBinaryPath = exportedPlugin.getFullPathName() + String ("/Contents/MacOS/CabbagePlugin");
    //
    //
    //
    //        if(!childProc.start("otool -L "+pluginBinaryPath))
    //        {
    //            CabbageUtilities::showMessage("You will need Xcode's CLI developer tools in order to include Csound in your plugin bundle. Please install them and try again.", &lookAndFeel);
    //            return;
    //        }
    //
    //
    //        StringArray lines;
    //        lines.addLines(childProc.readAllProcessOutput());
    //        const String csoundPath = lines[1].substring(0, lines[1].indexOf("("));
    //        const String install_name_tool = "install_name_tool -change " + csoundPath + "@loader_path/../Resources/CsoundLib64.framework/CsoundLib64 /" + pluginBinaryPath;
    //
    //
    //        childProc.start(install_name_tool);
    //        if(childProc.readAllProcessOutput().length()>1)
    //            CabbageUtilities::showMessage("There was a problem relinking the Csound binaries: " + childProc.readAllProcessOutput() + " Please contact Cabbage support and quote the following error message:", &lookAndFeel);
    //
    //
    //
    //
    //    }
#endif
    
    
}

//==============================================================================
// Set unique plugin ID for each plugin based on the file name
//==============================================================================
int PluginExporter::setUniquePluginId (File binFile, File csdFile, String pluginId)
{
    size_t file_size;
    //both mac and Windows encode the plugin IDs differently, so we run through a loop of both possibilities..
    const char* pluginIDToReplace[] = { "RORY", "YROR" };
    
    long loc;
    std::fstream mFile (binFile.getFullPathName().toUTF8(), ios_base::in | ios_base::out | ios_base::binary);
    
    for(int i = 0 ; i < 2 ; i++)
    {
        if (mFile.is_open())
        {
            mFile.seekg (0, ios::end);
            file_size = mFile.tellg();
            unsigned char* buffer = (unsigned char*)malloc (sizeof (unsigned char) * file_size);
            
            //set plugin ID, do this a few times in case the plugin ID appear in more than one place.
            for (int r = 0; r < 10; r++)
            {
                mFile.seekg (0, ios::beg);
                mFile.read ((char*)&buffer[0], file_size);
                loc = cabbageFindPluginId (buffer, file_size, pluginIDToReplace[i]);
                
                if (loc < 0)
                {
                    break;
                }
                else
                {
                    mFile.seekg (loc, ios::beg);
                    mFile.write (pluginId.toUTF8(), 4);
                }
            }
            
            if (SystemStats::getOperatingSystemType() != SystemStats::OperatingSystemType::Linux)
            {
                String manufacturer(JucePlugin_Manufacturer);
                //                mFile.seekg (0, ios::end);
                //                String manuName;
                //                if (manu.length() < 16)
                //                    for (int y = manu.length(); y < manu.length(); y++)
                //                        manu.append (String (" "), 1);
                //set manufacturer do this a few times in case the plugin ID appear in more than one place.
                for (int r = 0; r < 10; r++)
                {
                    mFile.seekg (0, ios::beg);
                    mFile.read ((char*)&buffer[0], file_size);
                    loc = cabbageFindPluginId (buffer, file_size, manufacturer.toUTF8());
                    
                    if (loc < 0)
                    {
                        break;
                    }
                    else
                    {
                        mFile.seekg (loc, ios::beg);
                        mFile.write (manufacturer.toUTF8(), manufacturer.length());
                    }
                }
            }
            
            
            
            
            //set plugin name based on .csd file
            const char* pluginName = "CabbageEffectNam";
            String plugLibName = csdFile.getFileNameWithoutExtension();
            
            if (plugLibName.length() < 16)
                for (int y = plugLibName.length(); y < 16; y++)
                    plugLibName.append (String (" "), 1);
            
            mFile.seekg (0, ios::end);
            //buffer = (unsigned char*)malloc(sizeof(unsigned char)*file_size);
            
            for (int i = 0; i < 5; i++)
            {
                
                mFile.seekg (0, ios::beg);
                mFile.read ((char*)&buffer[0], file_size);
                
                
                loc = cabbageFindPluginId (buffer, file_size, pluginName);
                
                if (loc < 0)
                    break;
                else
                {
                    mFile.seekg (loc, ios::beg);
                    mFile.write (plugLibName.toUTF8(), 16);
                }
            }
            
            loc = cabbageFindPluginId (buffer, file_size, pluginIDToReplace[i]);
            
            free (buffer);
            
        }
        else
            DBG ("===============================\nError/n=======================================\n" + csdFile.getFullPathName()+" File could not be opened");
    }
    mFile.close();
    
    return 1;
}
//==============================================================================
// Bundles files with VST
//==============================================================================
void PluginExporter::addFilesToPluginBundle (File csdFile, File exportDir)
{
    StringArray invalidFiles;
    StringArray csdArray;
    csdArray.addLines (csdFile.loadFileAsString());
    
    if(csdArray.indexOf("<CabbageIncludes>"))
    {
        for (int i = csdArray.indexOf("<CabbageIncludes>") + 1; i < csdArray.indexOf("</CabbageIncludes>"); i++)
        {
            
            File includeFile(csdFile.getParentDirectory().getFullPathName() + "/" + csdArray[i]);
            File newFile(exportDir.getParentDirectory().getFullPathName() + "/" + csdArray[i]);
            
            if (includeFile.exists())
            {
                includeFile.copyFileTo(newFile);
            }
            else
            {
                invalidFiles.add(includeFile.getFullPathName());
            }
        }
        
        if (invalidFiles.size() > 0)
            CabbageUtilities::showMessage(
                                          "Cabbage could not bundle the following files\n" + invalidFiles.joinIntoString("\n") +
                                          "\nPlease make sure they are located in the same folder as your .csd file.", &lookAndFeel);
    }
    
    StringArray linesFromCsd;
    linesFromCsd.addLines(csdFile.loadFileAsString());
    
    for( auto lineOfCode : linesFromCsd)
    {
        if(lineOfCode.contains("</Cabbage>"))
            return;
        
        ValueTree temp ("temp");
        CabbageWidgetData::setWidgetState (temp, lineOfCode, 0);
        if (CabbageWidgetData::getStringProp(temp, CabbageIdentifierIds::type) == CabbageWidgetTypes::form)
        {
            var bundleFiles = CabbageWidgetData::getProperty(temp, CabbageIdentifierIds::bundle);
            if(bundleFiles.size()>0)
                for( int i = 0 ; i < bundleFiles.size() ; i++)
                {
                    const File bundleFile = csdFile.getParentDirectory().getChildFile (bundleFiles[i].toString());
                    File newFile(exportDir.getParentDirectory().getFullPathName() + "/" + bundleFiles[i].toString());
                    
                    if (bundleFile.existsAsFile())
                        bundleFile.copyFileTo(newFile);
                    else if(bundleFile.exists())
                        bundleFile.copyDirectoryTo(newFile);
                    else
                    {
                        invalidFiles.add(csdFile.getParentDirectory().getChildFile (bundleFiles[i].toString()).getFullPathName());
                    }
                }
        }
    }
    
    if (invalidFiles.size() > 0)
        CabbageUtilities::showMessage(
                                      "Cabbage could not bundle the following files\n" + invalidFiles.joinIntoString("\n") +
                                      "\nPlease make sure they are located in the same folder as your .csd file.", &lookAndFeel);
    
}



long PluginExporter::cabbageFindPluginId (unsigned char* buf, size_t len, const char* s)
{
    long i, j;
    size_t slen = strlen (s);
    size_t imax = len - slen - 1;
    long ret = -1;
    int match;
    
    for (i = 0; i < imax; i++)
    {
        match = 1;
        
        for (j = 0; j < slen; j++)
        {
            if (buf[i + j] != s[j])
            {
                match = 0;
                break;
            }
        }
        
        if (match)
        {
            ret = i;
            break;
        }
    }
    
    //return position of plugin ID
    return ret;
}
