/*
 Copyright (C) 2018 Rory Walsh

 You should have received a copy of the GNU Lesser General Public
 License along with Csound; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 02110-1301 USA
 */

#pragma once

#include <plugin.h>
#include <string>
#include <modload.h>
#include <iomanip> 
#include <fstream>
#include <iostream>
#include "json.hpp"
#include <algorithm>

//using json = nlohmann::json;
using json = nlohmann::ordered_json;

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


struct openSvdFile : csnd::Plugin<1, 1>
{
	int init()
	{
		FILE* file = NULL;
		FILE** global = NULL;
		char* fileString = inargs.str_data(0).data;
		csound->create_global_variable("SVD_FILE", sizeof(FILE*));
		global = (FILE**)csound->query_global_variable("SVD_FILE");
		*global = fopen(fileString, "w+");

		if (*global == NULL)
		{
			csound->message("There was a problem opening your file.\n");
			csound->destroy_global_variable("SVD_FILE");
			outargs[0] = -1;
			return NOTOK;
		}

		outargs[0] = 1;
		return OK;
	}
};


struct writeSvdFile : csnd::Plugin<1, 10>
{
	int init()
	{
		json j;
		if (in_count() < 9)
		{
			csound->message("You did not pass have enough arguments to starVoiceWrite\n");
			return NOTOK;
		}

		char* filename = inargs.str_data(0).data;
		csnd::Vector<STRINGDAT>& sampleNamesArr = inargs.vector_data<STRINGDAT>(1);
		csnd::Vector<MYFLT>& channelsArr = inargs.vector_data<MYFLT>(2);
		csnd::Vector<MYFLT>& idsArr = inargs.vector_data<MYFLT>(3);
		csnd::Vector<MYFLT>& notesArr = inargs.vector_data<MYFLT>(4);
		csnd::Vector<MYFLT>& startsArr = inargs.vector_data<MYFLT>(5);
		csnd::Vector<MYFLT>& endsArr = inargs.vector_data<MYFLT>(6);
		csnd::Vector<MYFLT>& fadeInsArr = inargs.vector_data<MYFLT>(7);
		csnd::Vector<MYFLT>& fadeOutsArr = inargs.vector_data<MYFLT>(8);
		csnd::Vector<MYFLT>& pitchesArr = inargs.vector_data<MYFLT>(9);


		const int entries = channelsArr.len();
		for (int i = 0; i < entries; i++)
		{
			j[sampleNamesArr[i].data]["Sample Id"] = idsArr[i];
			j[sampleNamesArr[i].data]["Channels"] = channelsArr[i];
			j[sampleNamesArr[i].data]["Note Number"] = notesArr[i];
			j[sampleNamesArr[i].data]["Starting Sample"] = startsArr[i];
			j[sampleNamesArr[i].data]["Ending Sample"] = endsArr[i];
			j[sampleNamesArr[i].data]["Fade In Time"] = fadeInsArr[i];
			j[sampleNamesArr[i].data]["Fade Out Time"] = fadeOutsArr[i];
			j[sampleNamesArr[i].data]["Pitch"] = pitchesArr[i];
		}

		std::ofstream file;
		file.open(filename);
		file << std::setw(4) << j << std::endl;
		file.close();
		outargs[0] = 1;
		return OK;
	}
};

struct readSvdFile : csnd::Plugin<9, 1>
{
	int init()
	{
		std::vector<std::string> sampleNames;
		std::vector<MYFLT> channels, notes, ids, starts, ends, fadeIns, fadeOuts, pitches;
		std::string filename(inargs.str_data(0).data);
		csnd::Vector<STRINGDAT>& sampleNamesArr = outargs.vector_data<STRINGDAT>(0);
		csnd::Vector<MYFLT>& channelsArr = outargs.vector_data<MYFLT>(1);
		csnd::Vector<MYFLT>& idsArr = outargs.vector_data<MYFLT>(2);
		csnd::Vector<MYFLT>& notesArr = outargs.vector_data<MYFLT>(3);
		csnd::Vector<MYFLT>& startsArr = outargs.vector_data<MYFLT>(4);
		csnd::Vector<MYFLT>& endsArr = outargs.vector_data<MYFLT>(5);
		csnd::Vector<MYFLT>& fadeInsArr = outargs.vector_data<MYFLT>(6);
		csnd::Vector<MYFLT>& fadeOutsArr = outargs.vector_data<MYFLT>(7);
		csnd::Vector<MYFLT>& pitchesArr = outargs.vector_data<MYFLT>(8);

		json j;

		std::ifstream file(filename);
 		if (file.fail())
 		{
 			csound->message("Unable to open file");
 			outargs[0] = 0;
 			return NOTOK;
 		}
		
		j << file;
		
		for (auto iter = j.begin(); iter != j.end(); ++iter) 
		{
			sampleNames.push_back(iter.key());
			int index = 0;
			for (auto tt = iter.value().begin(); tt != iter.value().end(); ++tt, index++)
			{
				if (index == 0)
					ids.push_back(tt.value());
				else if (index == 1)
					channels.push_back(tt.value());
				else if (index == 2)
					notes.push_back(tt.value());
				else if (index == 3)
					starts.push_back(tt.value());
				else if (index == 4)
					ends.push_back(tt.value());
				else if (index == 5)
					fadeIns.push_back(tt.value());
				else if (index == 6)
					fadeOuts.push_back(tt.value());
				else if (index == 7)
					pitches.push_back(tt.value());
			}
		}

		const int entries = fadeIns.size();

		sampleNamesArr.init(csound, entries);		
    	channelsArr.init(csound, entries);
		notesArr.init(csound, entries);
		idsArr.init(csound, entries);
		startsArr.init(csound, entries);
		endsArr.init(csound, entries);
		fadeInsArr.init(csound, entries);
		fadeOutsArr.init(csound, entries);
		pitchesArr.init(csound, entries);

		for (int i = 0; i < entries; i++) {
			sampleNamesArr[i].data = csound->strdup((char*)sampleNames[i].c_str());
			channelsArr[i] = channels[i];
			notesArr[i] = notes[i];
			idsArr[i] = ids[i];
			startsArr[i] = starts[i];
			endsArr[i] = ends[i];
			fadeInsArr[i] = fadeIns[i];
			fadeOutsArr[i] = fadeOuts[i];
			pitchesArr[i] = pitches[i];
		}

		return OK;
	}
};

