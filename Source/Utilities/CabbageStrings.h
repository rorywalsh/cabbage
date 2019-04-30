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

    ~CabbageStrings() {};


    static String getNewCsoundFileText()
    {
        String newCsoundFile =  "<CsoundSynthesizer>\n"
                                "<CsOptions>\n"
                                "-n -d \n"
                                "</CsOptions>\n"
                                "<CsInstruments>\n"
                                "; Initialize the global variables. \n"
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

    static String getNewCabbageInstrumentFileText()
    {
        String newCsoundFile =
            "<Cabbage>\n"
            "form caption(\"Untitled\") size(400, 300), colour(58, 110, 182), pluginid(\"def1\")\n"
            "keyboard bounds(8, 158, 381, 95)\n"
            "</Cabbage>\n"
            "<CsoundSynthesizer>\n"
            "<CsOptions>\n"
            "-n -d -+rtmidi=NULL -M0 -m0d --midi-key-cps=4 --midi-velocity-amp=5\n"
            "</CsOptions>\n"
            "<CsInstruments>\n"
            "; Initialize the global variables. \n"
            "ksmps = 32\n"
            "nchnls = 2\n"
            "0dbfs = 1\n"
            "\n"
            ";instrument will be triggered by keyboard widget\n"
            "instr 1\n"
            "kEnv madsr .1, .2, .6, .4\n"
            "aOut vco2 p5, p4\n"
            "outs aOut*kEnv, aOut*kEnv\n"
            "endin\n"
            "\n"
            "</CsInstruments>\n"
            "<CsScore>\n"
            ";causes Csound to run for about 7000 years...\n"
            "f0 z\n"
            "</CsScore>\n"
            "</CsoundSynthesizer>\n"
            "";

        return newCsoundFile;
    }

    static String getNewCabbageEffectFileText()
    {
        String newCsoundFile =
            "<Cabbage>\n"
            "form caption(\"Untitled\") size(400, 300), colour(58, 110, 182), pluginid(\"def1\")\n"
            "rslider bounds(296, 162, 100, 100), channel(\"gain\"), range(0, 1, 0, 1, .01), text(\"Gain\"), trackercolour(\"lime\"), outlinecolour(0, 0, 0, 50), textcolour(\"black\")\n"
            "\n"
            "</Cabbage>\n"
            "<CsoundSynthesizer>\n"
            "<CsOptions>\n"
            "-n -d -+rtmidi=NULL -M0 -m0d \n"
            "</CsOptions>\n"
            "<CsInstruments>\n"
            "; Initialize the global variables. \n"
            "ksmps = 32\n"
            "nchnls = 2\n"
            "0dbfs = 1\n"
            "\n"
            "\n"
            "instr 1\n"
            "kGain chnget \"gain\"\n"
            "\n"
            "a1 inch 1\n"
            "a2 inch 2\n"
            "\n"
            "outs a1*kGain, a2*kGain\n"
            "endin\n"
            "\n"
            "</CsInstruments>\n"
            "<CsScore>\n"
            ";causes Csound to run for about 7000 years...\n"
            "f0 z\n"
            ";starts instrument 1 and runs it for a week\n"
            "i1 0 [60*60*24*7] \n"
            "</CsScore>\n"
            "</CsoundSynthesizer>\n"
            "";

        return newCsoundFile;
    }

    static const StringArray getColourIDStrings()
    {
        StringArray colours;
        colours.add ("Interface - MenuBar Background");
        colours.add ("Interface - MenuBar Text");
        colours.add ("Interface - MenuBar MouseOver Background");
        colours.add ("Interface - PopupMenu MouseOver Background");
        colours.add ("Interface - PopupMenu Background");
        colours.add ("Interface - PopupMenu Text");
        colours.add ("Interface - PopupMenu Highlighted Text");
        colours.add ("Interface - Main Background");
        colours.add ("Interface - Status Bar");
        colours.add ("Interface - Status Bar Text");
        colours.add ("Interface - Property Panel Background");
        colours.add ("Interface - Property Label Background");
        colours.add ("Interface - Property Label Text");
        colours.add ("Interface - Alart Window Background");
		colours.add("Interface - File Tab Bar");
		colours.add("Interface - File Tab Button");
		colours.add("Interface - File Tab Text");
		colours.add("Interface - File Tab Play Button");
		colours.add("Interface - Patcher");
        colours.add ("Editor - Code Background");
        colours.add ("Editor - Line Numbers Background");
        colours.add ("Editor - Line Numbers");
        colours.add ("Editor - Selected Text Background");
        colours.add ("Editor - Caret");
        colours.add ("Editor - Identifier");
        colours.add ("Editor - Strings");
        colours.add ("Editor - Keyword");
        colours.add ("Editor - Comment");
        colours.add ("Editor - Numbers");
        colours.add ("Editor - Csd Tags");
        colours.add ("Editor - Scrollbars");
        colours.add ("Console - Text");
        colours.add ("Console - Background");
        colours.add ("Console - Outline");
        return colours;
    }
};



#endif  // CABBAGESTRINGS_H_INCLUDED
