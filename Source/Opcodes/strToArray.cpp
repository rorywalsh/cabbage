/*
    Copyright (C) 2018 Rory Walsh

    You should have received a copy of the GNU Lesser General Public
    License along with Csound; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
    02110-1301 USA
 */

#pragma once

#include <plugin.h>
#include <modload.h>
#include <string>
#include <vector>

//==========================================================
//opcode version of the StrToArr UDO written by Joachim Heintz
//==========================================================
struct StrToArray : csnd::Plugin<1, 2>
{

    int init()
    {
        return parseStringAndFillStruct ();
    }

    int parseStringAndFillStruct ()
    {
        csnd::Vector<STRINGDAT>& out = outargs.vector_data<STRINGDAT> (0);

        char* inString = inargs.str_data (0).data;
        char* inStringDeLimiter = inargs.str_data (1).data;


        std::string input (inString);
        std::string delimiter (inStringDeLimiter);
        std::vector<std::string> tokens;

        size_t pos = 0;

        while ((pos = input.find (delimiter)) != std::string::npos)
        {
            tokens.push_back (input.substr (0, pos));
            input.erase (0, pos + delimiter.length());
        }

        if (input.size() > 0)
            tokens.push_back (input);

        out.init (csound, (int)tokens.size());

        for ( int i = 0 ; i < tokens.size() ; i++)
        {
            out[i].data = csound->strdup ((char*)tokens[i].c_str());
        }

        tokens.clear();
        return OK;
    }
};



