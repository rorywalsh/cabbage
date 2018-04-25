/*
  ==============================================================================

    CabbageExportPlugin.cpp
    Created: 27 Nov 2017 2:37:25pm
    Author:  rory

  ==============================================================================
*/

#include "CabbageExportPlugin.h"


//===============   methods for exporting plugins ==============================
void PluginExporter::exportPlugin (String type, File csdFile, String pluginId, String destination, String manu, bool encrypt)
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

        int platform = 0;
        if (SystemStats::getOperatingSystemType() == SystemStats::OperatingSystemType::Linux)
        {
            fileExtension = "so";
        }
        else if ((SystemStats::getOperatingSystemType() & SystemStats::MacOSX) != 0)
        {
            if(type.contains("VST"))
                fileExtension = "vst";
            else
                fileExtension = "component";

            platform = 2;
            currentApplicationDirectory = thisFile.getFullPathName() + "/Contents";
        }
        else
        {
            platform = 1;
            fileExtension = "dll";
        }


        if (type == "VSTi" || type == "AUi")
            pluginFilename = currentApplicationDirectory + String ("/CabbagePluginSynth." + fileExtension);
        else  if (type == "VST" || type == "AU")
            pluginFilename = currentApplicationDirectory + String ("/CabbagePluginEffect." + fileExtension);
        else  if (type == "AUMIDIFx")
            pluginFilename = currentApplicationDirectory + String ("/CabbagePluginMIDIEffect." + fileExtension);
        else if (type.contains (String ("LV2-ins")))
            pluginFilename = currentApplicationDirectory + String ("/CabbagePluginSynthLV2." + fileExtension);
        else if (type.contains (String ("LV2-fx")))
            pluginFilename = currentApplicationDirectory + String ("/CabbagePluginEffectLV2." + fileExtension);
        else if (type == "VCVRack")
        {
            fileExtension = "";
            pluginFilename = currentApplicationDirectory+"/CabbageRack/";
        }
        else  if (type == "FMOD")
        {
            fileExtension = (platform==1 ? String("dll") : String("dylib"));
            pluginFilename = currentApplicationDirectory + (platform==1 ? String("/fmod_csoundL64.dll") : String("/fmod_csound.dylib"));
            
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
            writePluginFileToDisk(newFile, csdFile, VSTData, fileExtension, pluginId, type, manu,
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
                        writePluginFileToDisk(fc.getResult(), csdFile, VSTData, fileExtension, pluginId, type, manu,
                                              encrypt);
                }
                else
                    writePluginFileToDisk(fc.getResult(), csdFile, VSTData, fileExtension, pluginId, type, manu,
                                          encrypt);

            }
        }
    }
}


void PluginExporter::writePluginFileToDisk (File fc, File csdFile, File VSTData, String fileExtension, String pluginId, String type, String manu, bool encrypt)
{

    File dll (fc.withFileExtension (fileExtension).getFullPathName());

    if (!VSTData.copyFileTo (dll) && type!="VCVRack" )
    {
        CabbageUtilities::showMessage ("Error", "Exporting: " + csdFile.getFullPathName() + ", Can't copy plugin lib, is it currently in use?", &lookAndFeel);
        return;
    }


    File exportedCsdFile;

    //vcv rack export is the same on all platforms..
    if(type=="VCVRack")
    {
        if(!VSTData.copyDirectoryTo(dll))
            jassertfalse;
        File rackCsdFile(dll.getFullPathName()+"/"+dll.getFileName()+".csd");
        //csdFile.moveFileTo(rackCsdFile);
        rackCsdFile.replaceWithText(csdFile.loadFileAsString());
        File oldFile(dll.getFullPathName()+"/CabbageRack.csd");
        oldFile.deleteFile();
        //bundle all auxiliary files
        addFilesToPluginBundle(csdFile, dll);
        return;
    }
    
    if ((SystemStats::getOperatingSystemType() & SystemStats::MacOSX) != 0)
    {
        if(fileExtension.containsIgnoreCase("component"))
            exportedCsdFile = dll.getFullPathName() + String ("/Contents/CabbagePlugin.csd");
        else if(fileExtension.containsIgnoreCase("vst"))
            exportedCsdFile = dll.getFullPathName() + String ("/Contents/") + fc.getFileNameWithoutExtension() + String (".csd");

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

            File bin (dll.getFullPathName() + String ("/Contents/MacOS/CabbagePlugin"));
            //if(bin.exists())showMessage("binary exists");

            File pl (dll.getFullPathName() + String ("/Contents/Info.plist"));
            String newPList = pl.loadFileAsString();
            
            if(fileExtension.containsIgnoreCase("vst"))
            {
                File pluginBinary (dll.getFullPathName() + String ("/Contents/MacOS/") + fc.getFileNameWithoutExtension());

                if (bin.moveFileTo (pluginBinary) == false)
                    CabbageUtilities::showMessage ("Error", "Could not copy library binary file. Make sure the two Cabbage .vst files are located in the Cabbage.app folder", &lookAndFeel);

                setUniquePluginId (pluginBinary, exportedCsdFile, pluginId);
               
                newPList = newPList.replace ("CabbagePlugin", fc.getFileNameWithoutExtension());
            }

            
            String manu(JucePlugin_Manufacturer);
            const String pluginName = "<string>" +manu + ": " + fc.getFileNameWithoutExtension() + "</string>";
            const String toReplace = "<string>"+manu+": CabbageEffectNam</string>";

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

        setUniquePluginId (dll, exportedCsdFile, pluginId);

        //bundle all auxiliary files
        addFilesToPluginBundle(csdFile, dll);
    }
    
    if(type.containsIgnoreCase("AU"))
    {
        File pluginBinary (dll.getFullPathName() + String ("/Contents/Resources/CabbagePlugin.rsrc"));
        setUniquePluginId (pluginBinary, exportedCsdFile, pluginId, true);
    }
}

//==============================================================================
// Set unique plugin ID for each plugin based on the file name
//==============================================================================
int PluginExporter::setUniquePluginId (File binFile, File csdFile, String pluginId, bool isRSRC)
{
    size_t file_size;
    const char* pluginID;
    
    if (isRSRC)
        pluginID = "RORY";
    else
        pluginID = "YROR";
    
    long loc;
    std::fstream mFile (binFile.getFullPathName().toUTF8(), ios_base::in | ios_base::out | ios_base::binary);
    
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
            loc = cabbageFindPluginId (buffer, file_size, pluginID);
            
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
        
        String manu(JucePlugin_Manufacturer);
        mFile.seekg (0, ios::end);
        String manuName;
        if (manu.length() < 16)
            for (int y = manu.length(); y < 16; y++)
                manu.append (String (" "), 1);
        //set manufacturer do this a few times in case the plugin ID appear in more than one place.
        for (int r = 0; r < 10; r++)
        {
            mFile.seekg (0, ios::beg);
            mFile.read ((char*)&buffer[0], file_size);
            loc = cabbageFindPluginId (buffer, file_size, "CabbageAudio");
            
            if (loc < 0)
            {
                break;
            }
            else
            {
                mFile.seekg (loc, ios::beg);
                mFile.write (manu.toUTF8(), 16);
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
        
        loc = cabbageFindPluginId (buffer, file_size, pluginID);
        
        free (buffer);
        
    }
    else
        DBG ("===============================\nError/n=======================================\n" + csdFile.getFullPathName()+" File could not be opened");
    
    mFile.close();
    
    return 1;
}
//==============================================================================
// Bundles files with VST
//==============================================================================
void PluginExporter::addFilesToPluginBundle (File csdFile, File exportDir)
{
    StringArray invalidFiles;
    bool invalidFilename = false;
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
