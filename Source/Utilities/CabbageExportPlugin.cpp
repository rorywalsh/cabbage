/*
  ==============================================================================

    CabbageExportPlugin.cpp
    Created: 27 Nov 2017 2:37:25pm
    Author:  rory

  ==============================================================================
*/

#include "CabbageExportPlugin.h"


//===============   methods for exporting plugins ==============================
void PluginExporter::exportPlugin (String type, File csdFile, String pluginId)
{
    String pluginFilename, fileExtension, currentApplicationDirectory;
    File thisFile;

    if (SystemStats::getOperatingSystemType() == SystemStats::OperatingSystemType::Linux)
    {
        fileExtension = "so";
        thisFile = File::getSpecialLocation (File::currentExecutableFile);
        currentApplicationDirectory = thisFile.getParentDirectory().getFullPathName();
    }
    else if ((SystemStats::getOperatingSystemType() & SystemStats::MacOSX) != 0)
    {
        fileExtension = "vst";
        thisFile = File::getSpecialLocation (File::currentApplicationFile);
        currentApplicationDirectory = thisFile.getFullPathName() + "/Contents";
    }
    else
    {
        fileExtension = "dll";
        thisFile = File::getSpecialLocation (File::currentApplicationFile);
        currentApplicationDirectory = thisFile.getParentDirectory().getFullPathName();
    }


    if (type.contains ("VSTi"))
        pluginFilename = currentApplicationDirectory + String ("/CabbagePluginSynth." + fileExtension);
    else if (type.contains (String ("VST")))
        pluginFilename = currentApplicationDirectory + String ("/CabbagePluginEffect." + fileExtension);
    else if (type.contains (String ("LV2-ins")))
        pluginFilename = currentApplicationDirectory + String ("/CabbagePluginSynthLV2." + fileExtension);
    else if (type.contains (String ("LV2-fx")))
        pluginFilename = currentApplicationDirectory + String ("/CabbagePluginEffectLV2." + fileExtension);

    File VSTData (pluginFilename);

    if (!VSTData.exists())
    {
        CabbageUtilities::showMessage("Error", pluginFilename + " cannot be found? It should be in the Cabbage root folder", &lookAndFeel);
    }

    FileChooser fc ("Save file as..", csdFile.getParentDirectory().getFullPathName(), "*." + fileExtension, CabbageUtilities::shouldUseNativeBrowser());

    if (fc.browseForFileToSave (false))
    {
        if (fc.getResult().existsAsFile())
        {
            const int result = CabbageUtilities::showYesNoMessage ("Do you wish to overwrite\nexiting file?", &lookAndFeel);

            if (result == 1)
                writePluginFileToDisk (fc.getResult(), csdFile, VSTData, fileExtension, pluginId);
        }
        else
            writePluginFileToDisk (fc.getResult(), csdFile, VSTData, fileExtension, pluginId);
    }

}

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

void PluginExporter::writePluginFileToDisk (File fc, File csdFile, File VSTData, String fileExtension, String pluginId)
{
    File dll (fc.withFileExtension (fileExtension).getFullPathName());

    if (!VSTData.copyFileTo (dll))
        CabbageUtilities::showMessage ("Error", "Can't copy plugin lib, is it currently in use?", &lookAndFeel);


    File exportedCsdFile;

    if ((SystemStats::getOperatingSystemType() & SystemStats::MacOSX) != 0)
    {
        exportedCsdFile = dll.getFullPathName() + String ("/Contents/") + fc.getFileNameWithoutExtension() + String (".csd");
        exportedCsdFile.replaceWithText (csdFile.loadFileAsString());

        File bin (dll.getFullPathName() + String ("/Contents/MacOS/CabbagePlugin"));
        //if(bin.exists())showMessage("binary exists");


        File pluginBinary (dll.getFullPathName() + String ("/Contents/MacOS/") + fc.getFileNameWithoutExtension());

        if (bin.moveFileTo (pluginBinary) == false)
            CabbageUtilities::showMessage ("Error", "Could not copy library binary file. Make sure the two Cabbage .vst files are located in the Cabbage.app folder", &lookAndFeel);

        setUniquePluginId (pluginBinary, exportedCsdFile, pluginId);

        File pl (dll.getFullPathName() + String ("/Contents/Info.plist"));
        String newPList = pl.loadFileAsString();
        //write our identifiers to the plist file
        newPList = newPList.replace ("CabbagePlugin", fc.getFileNameWithoutExtension());

        //write plist file
        pl.replaceWithText (newPList);


    }
    else
    {
        exportedCsdFile = fc.withFileExtension (".csd").getFullPathName();
        exportedCsdFile.replaceWithText (csdFile.loadFileAsString());
        setUniquePluginId (dll, exportedCsdFile, pluginId );
    }

    //bunlde all auxilary files
    addFilesToPluginBundle(csdFile, dll);

}


//==============================================================================
// Set unique plugin ID for each plugin based on the file name
//==============================================================================
int PluginExporter::setUniquePluginId (File binFile, File csdFile, String pluginId)
{
    size_t file_size;
    const char* pluginID;
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
                //showMessage(String("Internel Cabbage Error: The pluginID was not found"));
                break;
            }
            else
            {
                //showMessage(newID);
                mFile.seekg (loc, ios::beg);
                mFile.write (pluginId.toUTF8(), 4);
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
                mFile.write (csdFile.getFileNameWithoutExtension().toUTF8(), 16);
            }
        }

        free (buffer);

    }
    else
        CabbageUtilities::showMessage ("Error", "File could not be opened", &lookAndFeel);

    mFile.close();
    return 1;
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