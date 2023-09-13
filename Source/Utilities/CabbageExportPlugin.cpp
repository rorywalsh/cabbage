/*
 ==============================================================================
 
 CabbageExportPlugin.cpp
 Created: 27 Nov 2017 2:37:25pm
 Author:  rory
 
 ==============================================================================
 */

#include "CabbageExportPlugin.h"
#include <fstream>

//===============   methods for exporting plugins ==============================
void PluginExporter::exportPlugin (String type, File csdFile, String pluginId, String destination, bool promptForFilename, bool encrypt)
{
    
    File outputFile;
    if(csdFile.hasFileExtension(".csd") == false)
        return;
    
    
    if(csdFile.existsAsFile())
    {
        
        String pluginFilename, fileExtension;
        File thisFile = File::getSpecialLocation (File::currentApplicationFile);
#if defined(JUCE_LINUX)	
        String currentApplicationDirectory = "/usr/bin";
#else
        String currentApplicationDirectory = thisFile.getParentDirectory().getFullPathName();
#endif
        
        
        if (CabbageUtilities::getTargetPlatform() == CabbageUtilities::TargetPlatformTypes::Linux)
        {
            if(type == "Standalone")
                fileExtension = "";
            else
                fileExtension = "so";
        }
        else if (CabbageUtilities::getTargetPlatform() == CabbageUtilities::TargetPlatformTypes::OSX)
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
        
// #if CabbagePro && JUCE_MAC
//         const String pluginDesc = String(JucePlugin_Manufacturer);
// #else
        const String pluginDesc = String(PluginDesc);
// #endifs

#ifdef JUCE_LINUX
    if(type == "VST3i")
        currentApplicationDirectory = currentApplicationDirectory+"/CabbagePluginSynth.vst3/Contents/x86_64-linux";
    else if(type == "VST3")
        currentApplicationDirectory = currentApplicationDirectory+"/CabbagePluginEffect.vst3/Contents/x86_64-linux";
#endif

        if (type == "VSTi" || type == "AUi" || type == "VST3i")
            pluginFilename = currentApplicationDirectory + String ("/"+pluginDesc.replace(" ", "_")+"Synth." + fileExtension);
        else  if (type == "VST" || type == "AU" || type == "VST3")
            pluginFilename = currentApplicationDirectory + String ("/"+pluginDesc.replace(" ", "_")+"Effect." + fileExtension);
        else  if (type == "AUMIDIFx")
            pluginFilename = currentApplicationDirectory + String ("/"+pluginDesc.replace(" ", "_")+"MidiEffect." + fileExtension);
        else if (type.contains (String ("LV2-ins")))
            pluginFilename = currentApplicationDirectory + String ("/"+pluginDesc.replace(" ", "_")+"SynthLV2." + fileExtension);
        else if (type.contains (String ("LV2-fx")))
            pluginFilename = currentApplicationDirectory + String ("/"+pluginDesc.replace(" ", "_")+"EffectLV2." + fileExtension);
        else if (type == "VCVRack")
        {
            fileExtension = "";
            pluginFilename = currentApplicationDirectory+"/CabbageRack/";
            if(!File(pluginFilename).exists())
                pluginFilename = File::getSpecialLocation (File::currentApplicationFile).getParentDirectory().getFullPathName()+"/CabbageRack/";
        }
		else if (type == "Unity")
		{
			fileExtension = ((CabbageUtilities::getTargetPlatform() == CabbageUtilities::TargetPlatformTypes::OSX) ? String("bundle") : String("dll"));
			pluginFilename = currentApplicationDirectory + ((CabbageUtilities::getTargetPlatform() == CabbageUtilities::TargetPlatformTypes::OSX) ? String("/AudioPluginDemo.bundle") : String("/AudioPluginDemo.dll"));
		}
        else if (type == "FMOD")
        {
            fileExtension = ((CabbageUtilities::getTargetPlatform() == CabbageUtilities::TargetPlatformTypes::OSX) ? String("bundle") : String("dll"));
            pluginFilename = currentApplicationDirectory + ((CabbageUtilities::getTargetPlatform() == CabbageUtilities::TargetPlatformTypes::OSX) ? String("/fmod_csound.dylib") : String("/fmod_csound64.dll"));
        }
        else if (type == "FMODFx")
        {
            fileExtension = ((CabbageUtilities::getTargetPlatform() == CabbageUtilities::TargetPlatformTypes::OSX) ? String("bundle") : String("dll"));
            pluginFilename = currentApplicationDirectory + ((CabbageUtilities::getTargetPlatform() == CabbageUtilities::TargetPlatformTypes::OSX) ? String("/fmod_csound_fx.dylib") : String("/fmod_csound64_fx.dll"));
            
        }
        else  if (type == "Standalone")
        {
            if (CabbageUtilities::getTargetPlatform() == CabbageUtilities::TargetPlatformTypes::Linux)
                pluginFilename = currentApplicationDirectory + "/" + pluginDesc;
            else
                pluginFilename = currentApplicationDirectory + "/" + pluginDesc + "." + fileExtension;
        }
        
#if !CLIConverter
        File VSTData (pluginFilename);
#else
        File VSTData (pluginFilename.replace("/CLIConverter/Contents", ""));
#endif
        
        if (!VSTData.exists())
        {
            CabbageUtilities::showMessage("Error", pluginFilename + " cannot be found? It should be in the Cabbage root folder", &lookAndFeel);
            return;
        }
        
        //if batch converting plugins
        if(File(destination).exists())
        {
            const String newFile = File(destination).getChildFile(csdFile.getFileName()).getFullPathName();
            writePluginFileToDisk(newFile, csdFile, VSTData, fileExtension, pluginId, type,
                                  encrypt);
        }
        else if(promptForFilename == false)
        {
            String newFile = destination+"/"+csdFile.getFileNameWithoutExtension();

            writePluginFileToDisk(newFile, csdFile, VSTData, fileExtension, pluginId, type,
                                  encrypt);
            
        }
        else
        {
            
            FileChooser fc("Save file as..", csdFile.getParentDirectory().getFullPathName(), "*." + fileExtension,
                           CabbageUtilities::shouldUseNativeBrowser());
            
            if (fc.browseForFileToSave(false))
            {
                outputFile = fc.getResult();
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
    
    if (CabbageUtilities::getTargetPlatform() == CabbageUtilities::TargetPlatformTypes::OSX)
    {
        if(adhocSign == true)
        {
            String binaryFile = outputFile.getFullPathName()+"/Contents/MacOS/"+outputFile.getFileNameWithoutExtension();
            
            String command = "codesign -s - "+binaryFile+" --timestamp --deep --force";
            childProcess.start (command);
            StringArray output;
            output.addLines (childProcess.readAllProcessOutput());
            DBG(output.joinIntoString("\n"));
        }
    }
    
    
}


void PluginExporter::writePluginFileToDisk (File fc, File csdFile, File VSTData, String fileExtension, String pluginId, String type,  bool encrypt)
{
    
//#if !CLIConverter
    File exportedPlugin (fc.withFileExtension (fileExtension).getFullPathName());
//#else
//    File exportedPlugin (fc.withFileExtension (fileExtension).getFullPathName().replace("/CLIConverter", ""));
//#endif
    
    
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
    if (CabbageUtilities::getTargetPlatform() == CabbageUtilities::TargetPlatformTypes::OSX)
    {
        if(File(exportedPlugin).exists() )
        {
            if(!File(exportedPlugin).deleteRecursively())
                jassertfalse;
        }
    }
    
#if CLIConverter
    if(exportedPlugin.existsAsFile())
        DBG("plugin exists");

    auto mkdir = "mkdir " + exportedPlugin.getParentDirectory().getFullPathName().toStdString();
    system(mkdir.c_str());
    auto command = "cp -Rf " + VSTData.getFullPathName().toStdString() + " " +exportedPlugin.getFullPathName().toStdString();
    system(command.c_str());
    
#else
    if (!VSTData.copyFileTo (exportedPlugin))
    {

        Logger::writeToLog("Could not create plugin file. Check write access");

        CabbageUtilities::showMessage ("Error", "Exporting: " + csdFile.getFullPathName() + ", Can't copy plugin to this location. It currently be in use, or you may be trying to install to a system folder you don't have permission to write in. Please try exporting to a different location.", &lookAndFeel);

        return;
    }
#endif
    
    File exportedCsdFile;
    
    
    if (CabbageUtilities::getTargetPlatform() == CabbageUtilities::TargetPlatformTypes::OSX)
    {

        const String pluginDesc = VSTData.getFileNameWithoutExtension();

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
		else if (fileExtension.contains("bundle"))
		{
			exportedCsdFile = fc.withFileExtension(".csd").getFullPathName();
			exportedCsdFile.replaceWithText(csdFile.loadFileAsString());
		}
        else
        {
            if(encrypt)
            {
                exportedCsdFile.replaceWithText(encodeString(csdFile));
            }
            else
                exportedCsdFile.replaceWithText (csdFile.loadFileAsString());
            
            File bin;
            if(fileExtension.containsIgnoreCase("app"))
               bin = File(exportedPlugin.getFullPathName() + String ("/Contents/MacOS/"+pluginDesc+"Effect"));
            else
                  bin = File(exportedPlugin.getFullPathName() + String ("/Contents/MacOS/"+pluginDesc));
            
            setUniquePluginId (bin, exportedCsdFile, pluginId);
            
            File pl (exportedPlugin.getFullPathName() + String ("/Contents/Info.plist"));
            String newPList = pl.loadFileAsString();
            
            if(fileExtension.containsIgnoreCase("vst") || fileExtension.containsIgnoreCase("app"))
            {
                
                File pluginBinary (exportedPlugin.getFullPathName() + String ("/Contents/MacOS/") + fc.getFileNameWithoutExtension());

                if (bin.moveFileTo (pluginBinary) == false)
                    CabbageUtilities::showMessage ("Error", "Could not copy library binary file. Make sure the two Cabbage .vst files are located in the Cabbage.app folder", &lookAndFeel);

#if CabbagePro
                newPList = newPList.replace (pluginDesc+"Effect", fc.getFileNameWithoutExtension());
                newPList = newPList.replace (pluginDesc+"MidiEffect", fc.getFileNameWithoutExtension());
                newPList = newPList.replace (pluginDesc, fc.getFileNameWithoutExtension());
#else
                newPList = newPList.replace (pluginDesc+"MidiEffect", fc.getFileNameWithoutExtension());
                newPList = newPList.replace (pluginDesc, fc.getFileNameWithoutExtension());
#endif
            }
            
            
            String manu(CabbageManufacturer);
            const String pluginName = "<string>" +manu + ": " + fc.getFileNameWithoutExtension() + "</string>";
            const String toReplace = "<string>"+manu+": "+VSTData.getFileNameWithoutExtension()+"</string>";

            
#if CabbagePro
            
            //be sure to remove CabbageAudio from plugin plist..
            const String toReplace2 = "<string>CabbageAudio: CabbageEffectNam<string>";
            newPList = newPList.replace (toReplace2, pluginName);

            
            if(fileExtension.containsIgnoreCase("component"))
            {
                newPList = newPList.replace ("<string>CabbagePlugin</string>", "<string>" + pluginDesc + "</string>");
                newPList = newPList.replace (pluginDesc+"MidiEffect", fc.getFileNameWithoutExtension());
            }
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

        setUniquePluginId(exportedPlugin, exportedCsdFile, pluginId);
        addFilesToPluginBundle(csdFile, exportedPlugin);
    }
            
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
    std::fstream mFile (binFile.getFullPathName().toUTF8(), std::ios_base::in | std::ios_base::out | std::ios_base::binary);
    
    for(int i = 0 ; i < 2 ; i++)
    {
        if (mFile.is_open())
        {
            mFile.seekg (0, std::ios::end);
            file_size = mFile.tellg();
            unsigned char* buffer = (unsigned char*)malloc (sizeof (unsigned char) * file_size);
            
            //set plugin ID, do this a few times in case the plugin ID appear in more than one place.
            for (int r = 0; r < 10; r++)
            {
                mFile.seekg (0, std::ios::beg);
                mFile.read ((char*)&buffer[0], file_size);
                loc = cabbageFindPluginId (buffer, file_size, pluginIDToReplace[i]);
                
                if (loc < 0)
                {
                    break;
                }
                else
                {
                    mFile.seekg (loc, std::ios::beg);
                    mFile.write (pluginId.toUTF8(), 4);
                }
            }
            
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
