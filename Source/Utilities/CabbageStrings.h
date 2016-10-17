/*
  ==============================================================================

    CabbageStrings.h
    Created: 17 Oct 2016 4:15:42pm
    Author:  rory

  ==============================================================================
*/

#ifndef CABBAGESTRINGS_H_INCLUDED
#define CABBAGESTRINGS_H_INCLUDED

class CabbageStrings
{
	
public:
		CabbageStrings()
		{
			
			
		};
		
		~CabbageStrings(){};
		
	
	static String getNewCsoundFileText()
	{
		String newCsoundFile = 	"<CsoundSynthesizer>\n"
										"<CsOptions>\n"
										"-n -d \n"
										"</CsOptions>\n"
										"<CsInstruments>\n"
										"; Initialize the global variables. \n"
										"sr = 44100\n"
										"ksmps = 32\n"
										"nchnls = 2\n"
										"0dbfs = 1\n"
										"\n"
										"instr 1\n"
										"\n"
										"endin\n"
										"\n"
										"</CsInstruments>\n"
										"<CsScore>\n"
										"i1 0 10\n"
										"</CsScore>\n"
										"</CsoundSynthesizer>";
		return newCsoundFile;
	}
};



#endif  // CABBAGESTRINGS_H_INCLUDED
