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
			colours.add("Interface - MenuBar Background");
			colours.add("Interface - MenuBar Text");
			colours.add("Interface - MenuBar MouseOver Background");
			colours.add("Interface - PopupMenu MouseOver Background");
			colours.add("Interface - PopupMenu Background");
			colours.add("Interface - PopupMenu Text");
			colours.add("Interface - PopupMenu Highlighted Text");
			colours.add("Interface - Main Background");
			colours.add("Interface - Status Bar");
			colours.add("Interface - Status Bar Text");
			colours.add("Interface - Alart Window Background");
			colours.add("Editor - Code Background");
			colours.add("Editor - Line Numbers Background");
			colours.add("Editor - Line Numbers");
			colours.add("Editor - Selected Text Background");
			colours.add("Editor - Caret");
			colours.add("Editor - Identifier");
			colours.add("Editor - Operator");
			colours.add("Editor - Keyword");
			colours.add("Editor - Comment");
			colours.add("Editor - Numbers");
			colours.add("Editor - Csd Tags");
			colours.add("Console - Text");
			colours.add("Console - Background");
			colours.add("Console - Outline");			
			return colours;
	}
};



#endif  // CABBAGESTRINGS_H_INCLUDED
