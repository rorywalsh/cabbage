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
#include <vector>
#include <modload.h>

//removes a number of occurrences of one string from another
struct StrRemove : csnd::Plugin<1, 3>
{
    int init()
    {
        return parseStringAndFillStruct (this);
    }

    int parseStringAndFillStruct (Plugin* opcodeData)
    {
        int occurrences = -1;
        int strsRemoved = 0;
        char* inString = opcodeData->inargs.str_data (0).data;
        char* inStripString = opcodeData->inargs.str_data (1).data;

        if (opcodeData->in_count() > 2)
            occurrences = opcodeData->inargs[2];

        std::string input (inString);
        std::string stripString (inStripString);
        std::string::size_type index = input.find (stripString);

        while (index != std::string::npos)
        {
            input.erase (index, stripString.length());
            index = input.find (stripString, index);
            strsRemoved += 1;

            if (strsRemoved == occurrences)
                break;
        }

        opcodeData->outargs.str_data (0).data = csound->strdup ((char*)input.c_str());

        return OK;
    }
};

