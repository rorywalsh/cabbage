#include <iostream>
#include "CSDParser.hpp"
#include "XMLWriter.hpp"
#include "XMLStringBuilder.hpp"

int main()
{
    //Assuming these values are known in cabbage, otherwise we will need to retrieve from csd parsing
    std::string pluginName = "tone";
    std::string pluginType = "SourcePlugin";//"EffectPlugin"
    std::string pluginID = "3000";

    //this parses the CSD for Parameters
    CSDParser parser(pluginName + ".csd");

    //If the Parser was succsessfull
    if (parser.Parse())
    {
        //We no longer need XMLWriter class that writes out to file using ifstream

        //this class bulids an xml string based on the properties provided
        XMLStringBuilder builder(parser.GetParameters(), pluginName, pluginType, pluginID);
        std::cout << builder.GetXMLString() << std::endl;
    }
   
}

