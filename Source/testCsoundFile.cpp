#include <stdio.h>
#include "csound.hpp"
#include <iostream>

using namespace std;

//very basic app that will runs a few k-rate cycles of a Csound file for segfaults.

int main (int argc, char* argv[])
{
    Csound* csound = new Csound();
    csound->CompileCsd (argv[1]);
    csound->Start();

    for ( int i = 0 ; i < 16 ; i++)
        csound->PerformKsmps();

    //free Csound object
    delete csound;
    return 0;
}

