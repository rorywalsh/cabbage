/*
 Copyright (C) 2018 Rory Walsh

 You should have received a copy of the GNU Lesser General Public
 License along with Csound; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 02110-1301 USA
 */

#include <plugin.h>
#include <string>
 // #include <iomanip> 
#include <fstream>
// #include <iostream>
#include "json.hpp"
// #include <algorithm>
#include "../CabbageCommonHeaders.h"
using json = nlohmann::json;


class CabbagePersistentData
{
public:
    
    CabbagePersistentData(){}
    
    std::string data = "";
    int size = 0;
};


struct DumpJSONData : csnd::Plugin<1, 1>
{
    int init()
    {
        CabbagePersistentData** pd = (CabbagePersistentData**)csound->query_global_variable("cabbageData");
        if(*pd != nullptr)
        {
            auto perData = *pd;
            csound->message(perData->data);
        }
        
        return OK;
    }
};

struct WriteJSONData : csnd::Plugin<1, 3>
{
    int init()
    {
        writeJsonDataToGlobalVar();
        return OK;
    }

    void writeJsonDataToGlobalVar()
    {

        std::string jsonString(inargs.str_data(1).data);
        std::string jsonData;
        int mode = inargs[0];
        json j;

        CabbagePersistentData** pd = (CabbagePersistentData**)csound->query_global_variable("cabbageData");
        auto perData = *pd;
        if(perData != nullptr)
        {
            jsonData = perData->data;
        }
        else
        {
            csound->message("Internal JSON global var is not valid.");
            return;
        }

        if (json::accept(jsonString) == false)
        {
            csound->message("Invalid JSON data");
            outargs[0] = -1;
            return;
        }

        if (mode == 1)
        {
                j = json::parse(jsonData);
                auto j2 = json::parse(jsonString);
                j.update(j2);
        }
        else
        {
            j = json::parse(jsonString);
        }

        perData->data = j.dump();
    }
};


struct GetJSONFloatValue : csnd::Plugin<1, 2>
{
    int init()
    {
        readDataFromChannel();
        return OK;
    }

    int kperf()
    {
        readDataFromChannel();
        return OK;
    }

    void readDataFromChannel()
    {
        std::string channelName(inargs.str_data(0).data);
        std::string channelKey(inargs.str_data(1).data);
        std::string chString;
        json j;
        MYFLT* value;
        bool success = false;

        if (csound->get_csound()->GetChannelPtr(csound->get_csound(), &value, channelName.c_str(),
            CSOUND_STRING_CHANNEL | CSOUND_OUTPUT_CHANNEL) == CSOUND_SUCCESS)
        {
            chString = ((STRINGDAT*)value)->data;
            std::string channelData(chString);
            j = json::parse(channelData);
        }

        for (json::iterator it = j.begin(); it != j.end(); ++it)
        {
            if (it.key() == channelKey && it.value().is_number_float())
            {
                outargs[0] = it.value();
                success = true;
            }
        }

        if (success == false)
        {
            csound->message("Could not find value for "+ channelKey + "? Check JSON channel data.");
        }
    }
};

//===========================================================================
// Channel State Save/Recall
//===========================================================================

struct ChannelStateSave : csnd::Plugin<1, 1>
{
    int init()
    {
        writeDataToDisk();
        return OK;
    }

    int kperf()
    {
        writeDataToDisk();
        return OK;
    }

    void writeDataToDisk()
    {
        json j;

        controlChannelInfo_s* csoundChanList;
        int numberOfChannels = csound->get_csound()->ListChannels(csound->get_csound(), &csoundChanList);

        for (int i = 0; i < numberOfChannels; i++)
        {
            //            const float min = csoundChanList[i].hints.min;
            //            const float max = (csoundChanList[i].hints.max == 0 ? 1 : csoundChanList[i].hints.max);
            //            const float defaultValue = csoundChanList[i].hints.dflt;
            std::string name;

            MYFLT* value;
            char* chString;

            if (csound->get_csound()->GetChannelPtr(csound->get_csound(), &value, csoundChanList[i].name,
                CSOUND_CONTROL_CHANNEL | CSOUND_OUTPUT_CHANNEL) == CSOUND_SUCCESS)
            {
                j[csoundChanList[i].name] = *value;
            }

            if (csound->get_csound()->GetChannelPtr(csound->get_csound(), &value, csoundChanList[i].name,
                CSOUND_STRING_CHANNEL | CSOUND_OUTPUT_CHANNEL) == CSOUND_SUCCESS)
            {
                chString = ((STRINGDAT*)value)->data;
                String s(chString);
                j[csoundChanList[i].name] = s.replace("\\\\", "/").toStdString();
            }
        }

        String filename(inargs.str_data(0).data);
        std::ofstream file;
        file.open(filename.replace("\\\\", "/").toStdString());
        if (file.is_open() == false)
            outargs[0] = 0;
        else
            outargs[0] = 1;

        file << std::setw(4) << j << std::endl;
        file.close();
        csound->message(j.dump());
    }

};


struct ChannelStateRecall : csnd::Plugin<1, 2>
{
    int init()
    {
        readDataFromDisk();
        return OK;
    }

    int kperf()
    {
        readDataFromDisk();
        return OK;
    }

    void readDataFromDisk()
    {
        json j;
        std::string filename(inargs.str_data(0).data);
        std::vector<std::string> ignoreStrings;

        if (inargs[1] != 0)
        {
            csnd::Vector<STRINGDAT>& in = inargs.vector_data<STRINGDAT>(1);
            for (int i = 0; i < in.len(); i++)
            {
                ignoreStrings.push_back(std::string(in[i].data));
            }
        }

        std::ifstream file(filename);
        if (file.fail())
        {
            csound->message("Unable to open file");
            outargs[0] = 0;
            return;
        }

        j << file;
        MYFLT* value;

        for (json::iterator it = j.begin(); it != j.end(); ++it)
        {
            bool ignore = false;
            std::string channelName = it.key();

            for (int i = 0; i < ignoreStrings.size(); i++)
            {
                if (channelName == ignoreStrings[i])
                    ignore = true;
            }

            if (ignore == false)
            {
                if (it.value().is_number_float())
                {
                    if (csound->get_csound()->GetChannelPtr(csound->get_csound(), &value, channelName.c_str(),
                        CSOUND_CONTROL_CHANNEL | CSOUND_OUTPUT_CHANNEL) == CSOUND_SUCCESS)
                    {
                        *value = it.value();
                    }
                }
                else if (it.value().is_string())
                {
                    if (csound->get_csound()->GetChannelPtr(csound->get_csound(), &value, channelName.c_str(),
                        CSOUND_STRING_CHANNEL | CSOUND_OUTPUT_CHANNEL) == CSOUND_SUCCESS)
                    {
                        std::string string = it.value();
                        ((STRINGDAT*)value)->data = csound->strdup((char*)string.c_str());
                    }
                }
            }
        }
        outargs[0] = 1;
        file.close();
        csound->message(j.dump());
    }

};

//===========================================================================
// FileToStr
//===========================================================================
struct FileToStr : csnd::Plugin<1, 1>
{
    int init()
    {
        std::string line;
        std::string lines;
        char* fileName = inargs.str_data(0).data;
        std::ifstream fileStream(fileName);

        if (fileStream.is_open())
        {
            while (getline(fileStream, line))
            {
                lines.append(line);
                lines.append("\n");
            }
        }
        else
        {
            csound->message("fileToStr could not open file for reading");
            return NOTOK;
        }

        fileStream.close();
        outargs.str_data(0).data = csound->strdup((char*)lines.c_str());
        return OK;
    }
};

//===========================================================================
//removes a number of occurrences of one string from another
//===========================================================================
struct StrRemove : csnd::Plugin<1, 3>
{
    int init()
    {
        return parseStringAndFillStruct(this);
    }

    int parseStringAndFillStruct(Plugin* opcodeData)
    {
        int occurrences = -1;
        int strsRemoved = 0;
        char* inString = opcodeData->inargs.str_data(0).data;
        char* inStripString = opcodeData->inargs.str_data(1).data;

        if (opcodeData->in_count() > 2)
            occurrences = opcodeData->inargs[2];

        std::string input(inString);
        std::string stripString(inStripString);
        std::string::size_type index = input.find(stripString);

        while (index != std::string::npos)
        {
            input.erase(index, stripString.length());
            index = input.find(stripString, index);
            strsRemoved += 1;

            if (strsRemoved == occurrences)
                break;
        }

        opcodeData->outargs.str_data(0).data = csound->strdup((char*)input.c_str());

        return OK;
    }
};

//===========================================================================
// StrToArray
//===========================================================================
struct StrToArray : csnd::Plugin<1, 2>
{
    int init()
    {
        return parseStringAndFillStruct();
    }

    int parseStringAndFillStruct()
    {
        csnd::Vector<STRINGDAT>& out = outargs.vector_data<STRINGDAT>(0);

        char* inString = inargs.str_data(0).data;
        char* inStringDeLimiter = inargs.str_data(1).data;


        std::string input(inString);
        std::string delimiter(inStringDeLimiter);
        std::vector<std::string> tokens;

        size_t pos = 0;

        while ((pos = input.find(delimiter)) != std::string::npos)
        {
            tokens.push_back(input.substr(0, pos));
            input.erase(0, pos + delimiter.length());
        }

        if (input.size() > 0)
            tokens.push_back(input);

        out.init(csound, (int)tokens.size());

        for (int i = 0; i < tokens.size(); i++)
        {
            out[i].data = csound->strdup((char*)tokens[i].c_str());
        }

        tokens.clear();
        return OK;
    }
};
//===========================================================================
// StrToFile
//===========================================================================

struct StrToFile : csnd::Plugin<1, 3>
{
    int init()
    {

        int mode = 0;
        if (in_count() < 2)
        {
            csound->message("You did not pass have enough arguments to strtofile\n");
            return NOTOK;
        }

        char* inString = inargs.str_data(0).data;
        char* fileName = inargs.str_data(1).data;


        if (in_count() > 2)
            mode = inargs[2];

        std::ofstream fileStream;

        if (mode == 1)
        {
            fileStream.open(fileName, std::ios::in | std::ios::app);
        }
        else
            fileStream.open(fileName, std::ios::in | std::ios::trunc);

        fileStream << inString;

        if (!fileStream.is_open())
        {
            csound->message("*** strToFile could not open file for writing ***");
            outargs[0] = 0;
        }

        fileStream.close();
        outargs[0] = 1;
        return OK;
    }
};
//void csnd::on_load (Csound* csound)
//{
//    csnd::plugin<channelStateSave> (csound, "channelStateSave.i", "i", "S", csnd::thread::i);
//    csnd::plugin<channelStateSave>(csound, "channelStateSave.k", "k", "S", csnd::thread::k);
//    csnd::plugin<channelStateRecall> (csound, "channelStateRecall.i", "i", "S", csnd::thread::i);
//    csnd::plugin<channelStateRecall> (csound, "channelStateRecall.k", "k", "SO", csnd::thread::k);
//    csnd::plugin<channelStateRecall>(csound, "channelStateRecall.k", "k", "SS[]", csnd::thread::k);
//}
