#include <juce_core/juce_core.h>
#include "JuceHeader.h"
#include "../Utilities/CabbageExportPlugin.h"
#include "../CabbageCommonHeaders.h"
#include "../Utilities/CabbageUtilities.h"


const String getPluginInfo (File csdFile, String info)
{
    StringArray csdLines;
    csdLines.addLines (csdFile.loadFileAsString());
    
    for (auto line : csdLines)
    {
        ValueTree temp ("temp");
        CabbageWidgetData::setWidgetState (temp, line, 0);
        
        if (CabbageWidgetData::getStringProp (temp, CabbageIdentifierIds::type) == CabbageWidgetTypes::form)
        {
            if(info == "id")
                return CabbageWidgetData::getStringProp (temp, CabbageIdentifierIds::pluginid);
            else if(info == "manufacturer")
                return CabbageWidgetData::getStringProp (temp, CabbageIdentifierIds::manufacturer);
        }
        
    }
    
    return String();
}

int main (int argc, char* argv[])
{
    PluginExporter pluginExporter;
    String args;
    // Your code goes here!
    juce::ignoreUnused (argc, argv);
    
    std::cout << "Usage: CLIConverter --export-TYPE=\"name of csd file\" --destination=\"some absolute or relative dir\"\n";
    std::cout << "If you leave out the destiontation, exports will be placed ino the same folder as the csd file\n\n";

    
    for( int i = 0 ; i < argc ; i++)
    {
        args.append(argv[i], 1000);
    }
    
    String file = args.substring(args.indexOf("--export-")+9);
    String type = file.substring(0, file.indexOf("="));
    file = file.substring(file.indexOf("=")+1, file.indexOf("--") != -1 ? file.indexOf("--") : 300);

    File csdFile(File::getSpecialLocation(File::SpecialLocationType::hostApplicationPath).getChildFile(file));
    String destination = args.substring(args.indexOf("--destination=")+14);
    File pluginDestination(File::getSpecialLocation(File::SpecialLocationType::hostApplicationPath).getChildFile(destination));


//    if(!pluginDestination.exists())
//    {
//        std::string c;
//        pluginDestination = csdFile.getParentDirectory();
//        std::cout << "You did not specify an export location. As a result, your plugin will be created in the same directory as its csd file. This may lead to some csd files being overwritten during the exporting process. Do you wish to continue? Y/N";
//        
//        if(strcmp(argv[argc-1], "Y") != 0)
//        {
//            getline(std::cin, c);
//            if(c == "N" || c =="n")
//                return 0;
//        }
//    }

    
#if CabbagePro
    pluginExporter.exportPlugin (type, csdFile,  getPluginInfo (csdFile, "id"), pluginDestination.getFullPathName(), false, true);
#else
    pluginExporter.exportPlugin (type, csdFile,  getPluginInfo (csdFile, "id"), pluginDestination.getFullPathName(), false, false);
#endif
    
    
    
    return 0;
}
