/*
 Copyright (C) 2018 Rory Walsh

 You should have received a copy of the GNU Lesser General Public
 License along with Csound; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 02110-1301 USA
 */

#include <plugin.h>
#include <string>
#include <modload.h>
#include <iomanip> 
#include <fstream>
#include <iostream>
#include "json.hpp"
#include <algorithm>

using json = nlohmann::json;

//https://stackoverflow.com/questions/4643512/replace-substring-with-another-substring-c
void replaceAll(std::string &s, const std::string &search, const std::string &replace) {
	for (size_t pos = 0; ; pos += replace.length()) {
		// Locate the substring to replace
		pos = s.find(search, pos);
		if (pos == std::string::npos) break;
		// Replace by erasing and inserting
		s.erase(pos, search.length());
		s.insert(pos, replace);
	}
}

struct channelStateSave : csnd::Plugin<1, 1>
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
			const float min = csoundChanList[i].hints.min;
			const float max = (csoundChanList[i].hints.max == 0 ? 1 : csoundChanList[i].hints.max);
			const float defaultValue = csoundChanList[i].hints.dflt;
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
				std::string s(chString);
				replaceAll(s, "\\\\", "/");
				j[csoundChanList[i].name] = std::string(s);
			}
		}

		std::string filename(inargs.str_data(0).data);
		replaceAll(filename, "\\\\", "/");
		std::ofstream file;
		file.open(filename);
		if (file.is_open() == false)
			outargs[0] = 0;
		else
			outargs[0] = 1;

		file << std::setw(4) << j << std::endl;
		file.close();
		csound->message(j.dump());
	}

};


struct channelStateRecall : csnd::Plugin<1, 2>
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

		if(inargs[1] != 0)
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



void csnd::on_load (Csound* csound)
{
    csnd::plugin<channelStateSave> (csound, "channelStateSave.i", "i", "S", csnd::thread::i);
	csnd::plugin<channelStateSave>(csound, "channelStateSave.k", "k", "S", csnd::thread::k);
    csnd::plugin<channelStateRecall> (csound, "channelStateRecall.i", "i", "S", csnd::thread::i);
	csnd::plugin<channelStateRecall> (csound, "channelStateRecall.k", "k", "SO", csnd::thread::k);
	csnd::plugin<channelStateRecall>(csound, "channelStateRecall.k", "k", "SS[]", csnd::thread::k);
}