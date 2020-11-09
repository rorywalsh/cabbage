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
#include <fstream>


struct FileToStr : csnd::Plugin<1, 1>
{
    int init()
    {
        std::string line;
        std::string lines;
        char* fileName = inargs.str_data (0).data;
        std::ifstream fileStream (fileName);

        if (fileStream.is_open())
        {
            while ( getline (fileStream, line) )
            {
                lines.append (line);
                lines.append ("\n");
            }
        }
        else
        {
            csound->message ("fileToStr could not open file for reading");
            return NOTOK;
        }

        fileStream.close();
        outargs.str_data (0).data = csound->strdup ((char*)lines.c_str());
        return OK;
    }
};

void csnd::on_load (Csound* csound)
{
    csnd::plugin<FileToStr> (csound, "fileToStr.i", "S", "S", csnd::thread::i);
}

