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
	
	static const StringArray getColourIDStrings()
	{
			StringArray colours;
			colours.add("MenuBarBackground");
			colours.add("MenuBarText");
			colours.add("MenuBarMouseOverBackground");
			colours.add("PopupMenuMouseOverBackground");
			colours.add("PopupMenuBackground");
			colours.add("PopupMenuText");
			colours.add("PopupMenuHighlightedText");
			colours.add("MainBackground");
			colours.add("CodeBackground");
			colours.add("LineNumberBackground");
			colours.add("LineNumbers");
			colours.add("PlainText");
			colours.add("SelectTextBackground");
			colours.add("Caret");
			colours.add("Preprocessor");
			colours.add("Punctuation");
			colours.add("Bracket");
			colours.add("String");
			colours.add("Float");
			colours.add("Integer");
			colours.add("Identifier");
			colours.add("Operator");
			colours.add("Keyword");
			colours.add("Comment");
			colours.add("Error");
			colours.add("AlartWindowBackground");
			return colours;
	}
};



#endif  // CABBAGESTRINGS_H_INCLUDED
