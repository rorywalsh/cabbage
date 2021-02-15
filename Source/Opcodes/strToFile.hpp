/*
    Copyright (C) 2018 Rory Walsh

    You should have received a copy of the GNU Lesser General Public
    License along with Csound; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
    02110-1301 USA
 */

#include <plugin.h>
#include <string>
#include <vector>
//#include <modload.h>
#include <fstream>


//==========================================================
//removes a number of occurrences of one string from another
//==========================================================
struct StrToFile : csnd::Plugin<1, 3>
{
    int init()
    {
       
        int mode = 0;
        if (in_count() < 2)
        {
            csound->message ("You did not pass have enough arguments to strtofile\n");
            return NOTOK;
        }

		char* inString = inargs.str_data(0).data;
		char* fileName = inargs.str_data(1).data;
        

        if(in_count()>2)  
            mode = inargs[2];

        std::ofstream fileStream;

        if (mode == 1)
        {
            fileStream.open (fileName, std::ios::in | std::ios::app);
        }
        else
            fileStream.open (fileName, std::ios::in | std::ios::trunc);

        fileStream << inString;

        if (!fileStream.is_open())
        {
            csound->message ("*** strToFile could not open file for writing ***");
            outargs[0] = 0;
        }

        fileStream.close();
        outargs[0] = 1;
        return OK;
    }
};

//void csnd::on_load (Csound* csound)
//{
//    csnd::plugin<StrToFile> (csound, "strToFile.SSO", "i", "SSO", csnd::thread::i);
//}

