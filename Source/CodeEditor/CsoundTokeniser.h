/*
  Copyright (C) 2013 Rory Walsh

  Cabbage is free software; you can redistribute it
  and/or modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  Cabbage is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with Csound; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
  02111-1307 USA
*/

#ifndef __CSOUND_TOKER__
#define __CSOUND_TOKER__

#include "../../JuceLibraryCode/JuceHeader.h"

class CsoundTokeniser : public CodeTokeniser
{
public:
    CsoundTokeniser() {}
    ~CsoundTokeniser() {}

    //==============================================================================
    enum TokenType
    {
        tokenType_error = 0,
        tokenType_comment,
        tokenType_builtInKeyword,
        tokenType_identifier,
        tokenType_integerLiteral,
        tokenType_floatLiteral,
        tokenType_stringLiteral,
        tokenType_operator,
        tokenType_bracket,
        tokenType_punctuation,
        tokenType_preprocessor,
        tokenType_csdTag
    };

    CodeEditorComponent::ColourScheme getDefaultColourScheme()
    {
        struct Type
        {
            const char* name;
            uint32 colour;
        };

        const Type types[] =
        {
            { "Error",              Colours::black.getARGB() },
            { "Comment",            Colours::green.getARGB() },
            { "Keyword",            Colours::blue.getARGB() },
            { "Identifier",         Colours::black.getARGB() },
            { "Integer",            Colours::orange.getARGB() },
            { "Float",              Colours::black.getARGB() },
            { "String",             Colours::red.getARGB() },
            { "Operator",           Colours::pink.getARGB() },
            { "Bracket",            Colours::darkgreen.getARGB() },
            { "Punctuation",        Colours::black.getARGB() },
            { "Preprocessor Text",  Colours::green.getARGB() },
            { "Csd Tag",            Colours::brown.getARGB() }
        };

        CodeEditorComponent::ColourScheme cs;

        for ( std::size_t i = 0; i < int (sizeof (types) / sizeof (types[0])); ++i) // (NB: numElementsInArray doesn't work here in GCC4.2)
            cs.set (types[i].name, Colour (types[i].colour));

        return cs;
    }

    CodeEditorComponent::ColourScheme getDarkColourScheme()
    {
        struct Type
        {
            const char* name;
            uint32 colour;
        };

        const Type types[] =
        {
            { "Error",              Colours::white.getARGB() },
            { "Comment",            Colours::green.getARGB() },
            { "Keyword",            Colours::cornflowerblue.getARGB() },
            { "Identifier",         Colours::white.getARGB() },
            { "Integer",            Colours::orange.getARGB() },
            { "Float",              Colours::pink.getARGB() },
            { "String",             Colours::red.getARGB() },
            { "Operator",           Colours::pink.getARGB() },
            { "Bracket",            Colours::darkgreen.getARGB() },
            { "Punctuation",        Colours::white.getARGB() },
            { "Preprocessor Text",  Colours::green.getARGB() },
            { "Csd Tag",            Colours::brown.getARGB() }
        };

        CodeEditorComponent::ColourScheme cs;

        for ( std::size_t i = 0; i < sizeof (types) / sizeof (types[0]); ++i)  // (NB: numElementsInArray doesn't work here in GCC4.2)
            cs.set (types[i].name, Colour (types[i].colour));

        return cs;
    }


private:
    //==============================================================================
    StringArray getTokenTypes()
    {
        StringArray s;
        s.add ("Error");
        s.add ("Comment");
        s.add ("C++ keyword");
        s.add ("Identifier");
        s.add ("Integer literal");
        s.add ("Float literal");
        s.add ("String literal");
        s.add ("Operator");
        s.add ("Bracket");
        s.add ("Punctuation");
        s.add ("Preprocessor line");
        s.add ("CSD Tag");
        return s;
    }

    //==============================================================================
    void skipQuotedString (CodeDocument::Iterator& source)
    {
        const juce_wchar quote = source.nextChar();

        for (;;)
        {
            const juce_wchar c = source.nextChar();

            if (c == quote || c == 0)
                break;

            if (c == '\\')
                source.skip();
        }
    }


    //==============================================================================
    void skipCSDTag (CodeDocument::Iterator& source) noexcept
    {
        for (;;)
        {
            const juce_wchar c = source.nextChar();

            if (c == 0 || (c == '>'))
                break;
        }
    }

    //==============================================================================
    bool isIdentifierStart (const char c)
    {
        return CharacterFunctions::isLetter (c)
               || c == '_' || c == '@';
    }

    //==============================================================================
    bool isIdentifierBody (const char c)
    {
        return CharacterFunctions::isLetter (c)
               || CharacterFunctions::isDigit (c)
               || c == '_' || c == '@';
    }

    //==============================================================================
    bool isReservedKeyword (String::CharPointerType token, const int tokenLength) noexcept
    {
        //populate char array with Csound keywords
        //this list of keywords is not completely up to date!
        static const char* const keywords[] =
        {
            "scale", "!=", "#define", "#include", "#undef", "#ifdef", "#ifndef", "$", "%", "&&", ">", ">=", "<", "<=", "*", "+", "-", "/", "=", "=+", "==", "ˆ", "||", "0dbfs", "<<", ">>", "&",
            "|", "¬", "#", "a", "alwayson", "ampmidid", "ATSadd", "ATSaddnz", "ATSbufread", "ATScross", "ATSinfo", "ATSinterpread", "ATSread", "ATSreadnz", "ATSpartialtap", "ATSsinnoi", "barmodel", "bformenc1",
            "bformdec1", "binit", "cauchyi", "cell", "centroid", "chebyshevpoly", "chnrecv", "chnsend", "chuap", "combinv", "compress", "compileorc", "compilestr", "connect", "copya2ftab", "copyf2array", "cosseg",
            "cossegb", "cossegr", "cpsmidinn", "cpumeter", "crossfm", "date", "dates", "dcblock2", "diskgrain", "distort", "doppler", "dust", "dust2", "ephasor", "eqfil", "evalstr", "expcurve", "exprandi",
            "expsegb", "expsegba", "faustaudio", "faustcompile", "faustctl", "faustgen", "fareylen", "fareyleni", "ficlose", "filebit", "filevalid", "array", "FLcloseButton", "FLexecButton", "FLgroup_end", "FLhvsBox",
            "FLhvsBoxSetValue", "FLkeyIn", "FLmouse", "flooper2", "FLpack_end", "FLsetSnapGroup", "FLslidBnk2", "FLslidBnkGetHandle", "FLslidBnkSet", "FLslidBnkSetk", "FLslidBnk2Set", "FLslidBnk2Setk", "fluidProgramSelect", "fluidSetInterpMethod", "FLvkeybd",
            "FLvslidBnk", "FLvslidBnk2", "FLxyin", "fractalnoise", "ftcps", "ftgenonce", "gainslider", "gaussi", "gausstrig", "genarray", "genarray_i", "gendy", "gendyc", "gendyx", "getcfg", "harmon2", "hrtfearly", "hrtfmove",
            "hrtfmove2", "hrtfreverb", "hrtfstat", "hvs1", "hvs2", "hvs3", "i", "imagecreate", "imagefree", "imagegetpixel", "imageload", "imagesave", "imagesetpixel", "imagesize", "inleta", "inletk", "inletkid", "inletf", "inrg",
            "insremot", "insglobal", "instance", "JackoAudioIn", "JackoAudioInConnect", "JackoAudioOut", "JackoAudioOutConnect", "JackoFreewheel", "JackoInfo", "JackoInit", "JackoMidiInConnect", "JackoMidiOutConnect", "JackoMidiOut", "JackoNoteOut", "JackoOn", "JackoTransport",
            "jacktransport", "joystick", "k", "kill", "linsegb", "log2", "logcurve", "loop_ge", "loop_gt", "loop_le", "loop_lt", "looptseg", "loopxseg", "loscilx", "lposcila", "lposcilsa", "lposcilsa2", "lua_exec",
            "lua_opdef", "lua_opcall", "maparray", "max", "maxabs", "maxabsaccum", "maxaccum", "median", "mediank", "midglobal", "midichannelaftertouch", "midicontrolchange", "midifilestatus", "midipolyaftertouch", "midiprogramchange", "midremot",
            "min", "minabs", "minabsaccum", "minaccum", "mincer", "minarray", "MixerSetLevel_i", "mode", "modmatrix", "monitor", "moogvcf2", "mp3in", "mp3len", "nchnls_i", "nlfilt2", "octmidib", "octmidinn", "outleta",
            "outletf", "outletk", "outletkid", "outrg", "p5gconnect", "p5gdata", "p", "pan2", "partikkel", "partikkelsync", "passign", "pchmidib", "pchmidinn", "pcount", "pdclip", "pdhalf", "pdhalfy", "pindex", "platerev",
            "plltrack", "polynomial", "pop", "pop_f", "powershape", "prepiano", "ptable", "ptablei", "ptable3", "ptablew", "ptrack", "push", "push_f", "pvsbandp", "pvsbandr", "pvsbin", "pvsbuffer", "pvsbufread", "pvsbufread2", "pvsdiskin",
            "pvsdisp", "pvsfreeze", "pvsfwrite", "pvsgain", "pvsin", "pvslock", "pvsmorph", "pvsmooth", "pvsout", "pvsosc", "pvspitch", "pvstanal", "pvswarp", "pvs2tab", "pyassign", "pyeval", "pyexec", "pyinit",
            "pyrun", "pwd", "qinf", "qnan", "readf", "readfi", "readscore", "readscratch", "remoteport", "remove", "resyn", "return", "rewindscore", "round", "scalearray", "scoreline", "scoreline_i", "serialBegin", "serialEnd",
            "serialFlush", "serialPrint", "serialRead", "serialWrite_i", "serialWrite", "setscorepos", "sflooper", "signum", "slider16table", "slider16tablef", "slider32table", "slider32tablef", "slider64table", "slider64tablef", "slider8table",
            "slider8tablef", "sliderKawai", "sndload", "sockrecv", "socksend", "stack", "STKBandedWG", "STKBeeThree", "STKBlowBotl", "STKBlowHole", "STKBowed", "STKBrass", "STKClarinet", "STKDrummer", "STKFlute", "STKFMVoices",
            "STKHevyMetl", "STKMandolin", "STKModalBar", "STKMoog", "STKPercFlut", "STKPlucked", "STKResonate", "STKRhodey", "STKSaxofony", "STKShakers", "STKSimple", "STKSitar", "STKStifKarp", "STKTubeBell", "STKVoicForm", "STKWhistle",
            "STKWurley", "strchar", "strchark", "strcatk", "strfromurl", "strindex", "strindexk", "strlen", "strlenk", "strlower", "strlowerk", "strrindex", "strrindexk", "strsub", "strsubk", "strupper", "strupperk",
            "sumarray", "syncloop", "syncphasor", "system", "tablefilter", "tablefilteri", "tableshuffle", "tabmorph", "tabmorpha", "tabmorphak", "tabmorphi", "tabplay", "slicearray", "tabsum", "tab2pvs", "temposcal", "trandom", "transegb",
            "transegr", "trcross", "trfilter", "trhighest", "trlowest", "trmix", "trscale", "trshift", "trsplit", "urandom", "vadd_i", "vaddv_i", "vaget", "vaset", "vbap", "vbapmove", "vbapg", "vbapgmove", "vdivv_i",
            "vexp_i", "vexpv_i", "vmult_i", "vmultv_i", "vosim", "vphaseseg", "vpow_i", "vpowv_i", "vsubv_i", "vtable1k", "wiiconnect", "wiidata", "wiirange", "wiisend", "writescratch", "encoder", "fftdisplay", "keyboard", "label",
            "listbox", "hrange", "vrange", "active", "align", "alpha", "amprange", "bounds", "caption", "channel", "channelarray", "channeltype", "child", "colour", "colour:0", "colour:1", "corners", "displaytype", "file",
            "fontcolour", "fontstyle", "guirefresh", "highlightcolour", "identchannel", "items", "latched", "line", "middlec", "max", "min", "mode", "outlinecolour", "outlinethickness", "plant", "pluginid", "populate",
            "popup", "popuptext", "range", "rangex", "rangey", "rotate", "samplerange", "scrubberpos", "scrubberposition", "shape", "show", "size", "sliderincr", "svgfile", "svgpath", "tablebackgroundcolour", "tablecolour",
            "tablegridcolour", "tablenumber", "text", "textcolour", "textbox", "trackercolour", "trackerthickness", "widgetarray", "wrap", "value", "velocity", "visible", "zoom", "zkwm", "maxarray", "fillarray", "lenarray",
            "od", "gentable", "texteditor", "textbox", "sprintfk", "strcpyk", "sprintf", "strcmpk", "strcmp", "a", "abetarand", "abexprnd", "infobutton", "groupbox", "do", "popupmenu", "filebutton", "until",
            "enduntil", "soundfiler", "combobox", "vslider", "vslider2", "vslider3", "hslider2", "define", "hslider3", "hslider", "rslider", "groupbox", "combobox", "xypad", "image", "plant", "csoundoutput", "button", "form", "checkbox",
            "tab", "abs", "acauchy", "active", "adsr", "adsyn", "adsynt", "adsynt2", "aexprand", "aftouch", "agauss", "agogobel", "alinrand", "alpass", "ampdb", "ampdbfs", "ampmidi", "apcauchy", "apoisson", "apow", "areson",
            "aresonk", "atone", "atonek", "atonex", "atrirand", "aunirand", "aweibull", "babo", "balance", "bamboo", "bbcutm", "bbcuts", "betarand", "bexprnd", "bformenc", "bformdec", "biquad", "biquada", "birnd",
            "bqrez", "butbp", "butbr", "buthp", "butlp", "butterbp", "butterbr", "butterhp", "butterlp", "button", "buzz", "cabasa", "cauchy",
            "ceil", "cent", "cggoto", "chanctrl", "changed", "chani", "chano", "checkbox", "chn", "chnclear", "chnexport", "chnget", "chnmix", "chnparams", "chnset", "cigoto", "ckgoto", "clear", "clfilt", "clip",
            "clock", "clockoff", "clockon", "cngoto", "comb", "control", "convle", "convolve", "cos", "cosh", "cosinv", "cps2pch", "cpsmidi", "cpsmidib", "cpsmidib", "cpsoct", "cpspch",
            "cpstmid", "cpstun", "cpstuni", "cpsxpch", "cpuprc", "cross2", "crunch", "ctrl14", "ctrl21", "ctrl7", "ctrlinit", "cuserrnd", "dam", "db", "dbamp", "dbfsamp", "dcblock", "dconv", "delay", "delay1",
            "delayk", "delayr", "delayw", "deltap", "deltap3", "deltapi", "deltapn", "deltapx", "deltapxw", "denorm", "diff", "diskin", "diskin2", "dispfft", "display", "distort1", "divz", "downsamp", "dripwater",
            "dssiactivate", "dssiaudio", "dssictls", "dssiinit", "dssilist", "dumpk", "dumpk2", "dumpk3", "dumpk4", "duserrnd", "else", "elseif", "endif", "endin", "endop", "envlpx", "envlpxr", "event", "event_i",
            "exitnow", "exp", "expon", "exprand", "expseg", "expsega", "expsegr", "filelen", "filenchnls", "filepeak", "filesr", "filter2", "fin", "fini", "fink", "fiopen", "flanger", "flashtxt", "FLbox", "FLbutBank",
            "FLbutton", "FLcolor", "FLcolor2", "FLcount", "FLgetsnap", "FLgroup", "FLgroupEnd", "FLgroupEnd", "FLhide", "FLjoy", "FLkeyb", "FLknob", "FLlabel", "FLloadsnap", "flooper", "floor", "FLpack", "FLpackEnd",
            "FLpackEnd", "FLpanel", "FLpanelEnd", "FLpanel_end", "FLprintk", "FLprintk2", "FLroller", "FLrun", "FLsavesnap", "FLscroll", "FLscrollEnd", "FLscroll_end", "FLsetAlign", "FLsetBox", "FLsetColor",
            "FLsetColor2", "FLsetFont", "FLsetPosition", "FLsetSize", "FLsetsnap", "FLsetText", "FLsetTextColor", "FLsetTextSize", "FLsetTextType", "FLsetVal_i", "FLsetVal", "FLshow", "FLslidBnk", "FLslider",
            "FLtabs", "FLtabsEnd", "FLtabs_end", "FLtext", "FLupdate", "fluidAllOut", "fluidCCi", "fluidCCk", "fluidControl", "fluidEngine", "fluidLoad", "fluidNote", "fluidOut", "fluidProgramSelect", "FLvalue",
            "fmb3", "fmbell", "fmmetal", "fmpercfl", "fmrhode", "fmvoice", "fmwurlie", "fof", "fof2", "fofilter", "fog", "fold", "follow", "follow2", "foscil", "foscili", "fout", "fouti", "foutir", "foutk", "fprintks",
            "fprints", "frac", "freeverb", "ftchnls", "ftconv", "ftfree", "ftgen", "ftgentmp", "ftlen", "ftload", "ftloadk", "ftlptim", "ftmorf", "ftsave", "ftsavek", "ftsr", "gain", "gauss", "gbuzz", "gogobel", "goto",
            "grain", "grain2", "grain3", "granule", "guiro", "harmon", "hilbert", "hrtfer", "hsboscil", "i", "ibetarand", "ibexprnd", "icauchy", "ictrl14", "ictrl21", "ictrl7", "iexprand", "if", "igauss", "igoto", "ihold",
            "ilinrand", "imidic14", "imidic21", "imidic7", "in", "in32", "inch", "inh", "init", "initc14", "initc21", "initc7", "ink", "ino", "inq", "ins", "instimek", "instimes", "instr", "int", "integ", "interp", "invalue",
            "inx", "inz", "ioff", "ion", "iondur", "iondur2", "ioutat", "ioutc", "ioutc14", "ioutpat", "ioutpb", "ioutpc", "ipcauchy", "ipoisson", "ipow", "is16b14", "is32b14", "islider16", "islider32", "islider64",
            "islider8", "itablecopy", "itablegpw", "itablemix", "itablew", "itrirand", "iunirand", "iweibull", "jitter", "jitter2", "jspline", "k", "kbetarand", "kbexprnd", "kcauchy", "kdump", "kdump2", "kdump3",
            "kdump4", "kexprand", "kfilter2", "kgauss", "kgoto", "klinrand", "kon", "koutat", "koutc", "koutc14", "koutpat", "koutpb", "koutpc", "kpcauchy", "kpoisson", "kpow", "kr", "kread", "kread2", "kread3",
            "kread4", "ksmps", "ktableseg", "ktrirand", "kunirand", "kweibull", "lfo", "limit", "line", "linen", "linenr", "lineto", "linrand", "linseg", "linsegr", "locsend", "locsig", "log", "log10", "logbtwo",
            "loop", "loopseg", "loopsegp", "lorenz", "lorisread", "lorismorph", "lorisplay", "loscil", "loscil3", "lowpass2", "lowres", "lowresx", "lpf18", "lpfreson", "lphasor", "lpinterp", "lposcil", "lposcil3",
            "lpread", "lpreson", "lpshold", "lpsholdp", "lpslot", "mac", "maca", "madsr", "mandel", "mandol", "marimba", "massign", "maxalloc", "max_k", "mclock", "mdelay", "metro", "midic14", "midic21", "midic7",
            "midichannelaftertouch", "midichn", "midicontrolchange", "midictrl", "mididefault", "midiin", "midinoteoff", "midinoteoncps", "midinoteonkey", "midinoteonoct", "midinoteonpch", "midion", "midion2",
            "midiout", "midipitchbend", "midipolyaftertouch", "midiprogramchange", "miditempo", "mirror", "MixerSetLevel", "MixerGetLevel", "MixerSend", "MixerReceive", "MixerClear", "moog", "moogladder",
            "moogvcf", "moscil", "mpulse", "mrtmsg", "multitap", "mute", "mxadsr", "nchnls", "nestedap", "nlfilt", "noise", "noteoff", "noteon", "noteondur", "noteondur2", "notnum", "nreverb", "nrpn", "nsamp",
            "nstrnum", "ntrpol", "octave", "octcps", "octmidi", "octmidib octmidib", "octpch", "opcode", "OSCsend", "OSCinit", "OSClisten", "oscbnk", "oscil", "oscil1", "oscil1i", "oscil3", "oscili", "oscilikt",
            "osciliktp", "oscilikts", "osciln", "oscils", "oscilx", "out", "out32", "outc", "outch", "outh", "outiat", "outic", "outic14", "outipat", "outipb", "outipc", "outk", "outkat", "outkc", "outkc14",
            "outkpat", "outkpb", "outkpc", "outo", "outq", "outq1", "outq2", "outq3", "outq4", "outs", "outs1", "outs2", "outvalue", "outx", "outz", "p", "pan", "pareq", "partials", "pcauchy", "pchbend", "pchmidi",
            "pchmidib pchmidib", "pchoct", "pconvolve", "peak", "peakk", "pgmassign", "phaser1", "phaser2", "phasor", "phasorbnk", "pinkish", "pitch", "pitchamdf", "planet", "pluck", "poisson", "polyaft",
            "port", "portk", "poscil", "poscil3", "pow", "powoftwo", "prealloc", "print", "printf", "printk", "printk2", "printks", "prints", "product", "pset", "puts", "pvadd", "pvbufread", "pvcross", "pvinterp",
            "pvoc", "pvread", "pvsadsyn", "pvsanal", "pvsarp", "pvscross", "pvscent", "pvsdemix", "pvsfread", "pvsftr", "pvsftw", "pvsifd", "pvsinfo", "pvsinit", "pvsmaska", "pvsynth", "pvscale", "pvshift",
            "pvsmix", "pvsfilter", "pvsblur", "pvstencil", "pvsvoc", "pyassign Opcodes", "pycall", "pyeval Opcodes", "pyexec Opcodes", "pyinit Opcodes", "pyrun Opcodes", "rand", "randh", "randi", "random",
            "randomh", "randomi", "rbjeq", "readclock", "readk", "readk2", "readk3", "readk4", "reinit", "release", "repluck", "reson", "resonk", "resonr", "resonx", "resonxk", "resony", "resonz", "resyn resyn",
            "reverb", "reverb2", "reverbsc", "rezzy", "rigoto", "rireturn", "rms", "rnd", "rnd31", "rspline", "rtclock", "s16b14", "s32b14", "samphold", "sandpaper", "scanhammer", "scans", "scantable", "scanu",
            "schedkwhen", "schedkwhennamed", "schedule", "schedwhen", "seed", "sekere", "semitone", "sense", "sensekey", "seqtime", "seqtime2", "setctrl", "setksmps", "sfilist", "sfinstr", "sfinstr3", "sfinstr3m",
            "sfinstrm", "sfload", "sfpassign", "sfplay", "sfplay3", "sfplay3m", "sfplaym", "sfplist", "sfpreset", "shaker", "sin", "sinh", "sininv", "sinsyn", "sleighbells", "slider16", "slider16f", "slider32",
            "slider32f", "slider64", "slider64f", "slider8", "slider8f", "sndloop", "sndwarp", "sndwarpst", "soundin", "soundout", "soundouts", "space", "spat3d", "spat3di", "spat3dt", "spdist", "specaddm",
            "specdiff", "specdisp", "specfilt", "spechist", "specptrk", "specscal", "specsum", "spectrum", "splitrig", "spsend", "sprintf", "sqrt", "sr", "statevar", "stix", "strcpy", "strcat", "strcmp", "streson",
            "strget", "strset", "strtod", "strtodk", "strtol", "strtolk", "subinstr", "subinstrinit", "sum", "svfilter", "syncgrain", "timedseq", "tb", "tb3_init", "tb4_init", "tb5_init", "tb6_init", "tb7_init",
            "tb8_init", "tb9_init", "tb10_init", "tb11_init", "tb12_init", "tb13_init", "tb14_init", "tb15_init", "tab", "tabrec", "table", "table3", "tablecopy", "tablegpw", "tablei", "tableicopy", "tableigpw",
            "tableikt", "tableimix", "tableiw", "tablekt", "tablemix", "tableng", "tablera", "tableseg", "tablew", "tablewa", "tablewkt", "tablexkt", "tablexseg", "tambourine", "tan", "tanh", "taninv", "taninv2",
            "tbvcf", "tempest", "tempo", "tempoval", "tigoto", "timeinstk", "timeinsts", "timek", "times", "timout", "tival", "tlineto", "tone", "tonek", "tonex", "tradsyn", "transeg", "trigger", "trigseq", "trirand",
            "turnoff", "turnoff2", "turnon", "unirand", "upsamp", "urd", "vadd", "vaddv", "valpass", "vbap16", "vbap16move", "vbap4", "vbap4move", "vbap8", "vbap8move", "vbaplsinit", "vbapz", "vbapzmove", "vcella",
            "vco", "vco2", "vco2ft", "vco2ift", "vco2init", "vcomb", "vcopy", "vcopy_i", "vdelay", "vdelay3", "vdelayx", "vdelayxq", "vdelayxs", "vdelayxw", "vdelayxwq", "vdelayxws", "vdivv", "vdelayk", "vecdelay",
            "veloc", "vexp", "vexpseg", "vexpv", "vibes", "vibr", "vibrato", "vincr", "vlimit", "vlinseg", "vlowres", "vmap", "vmirror", "vmult", "vmultv", "voice", "vport", "vpow", "vpowv", "vpvoc", "vrandh", "vrandi",
            "vstaudio", "vstaudiog", "vstbankload", "vstedit", "vstinit", "vstinfo", "vstmidiout", "vstnote", "vstparamset", "vstparamget", "vstprogset", "vsubv", "vtablei", "vtablek", "vtablea", "vtablewi",
            "vtablewk", "vtablewa", "vtabi", "vtabk", "vtaba", "vtabwi", "vtabwk", "vtabwa", "vwrap", "waveset", "weibull", "wgbow", "wgbowedbar", "wgbrass", "wgclar", "wgflute", "wgpluck", "wgpluck2", "wguide1",
            "wguide2", "wrap", "wterrain", "xadsr", "xin", "xout", "xscanmap", "xscansmap", "xscans", "xscanu", "xtratim", "xyin", "zacl", "zakinit", "zamod", "zar", "zarg", "zaw", "zawm", "zfilter2", "zir", "ziw",
            "ziwm", "zkcl", "zkmod", "zkr", "zkw", "zkwm ", "signaldisplay", "signalvariable", "updaterate", "menucolour", "ATSadd", "ATSaddnz", "ATSbufread", "ATScross", "ATSinfo", "ATSinterpread",
            "ATSpartialtap", "ATSread", "ATSreadnz", "ATSsinnoi", "FLcloseButton", "FLexecButton", "FLgroup_end", "FLhvsBox", "FLhvsBoxSetValue", "FLkeyIn", "FLmouse", "FLpack_end", "FLsetSnapGroup",
            "FLsetVali", "FLslidBnk2", "FLslidBnk2Set", "FLslidBnk2Setk", "FLslidBnkGetHandle", "FLslidBnkSet", "FLslidBnkSetk", "FLvkeybd", "FLvslidBnk", "FLvslidBnk2", "FLxyin", "MixerSetLevel_i",
            "S", "a", "alwayson", "ampmidid", "barmodel", "bformdec1", "bformenc1", "binit", "buchla", "c2r", "cauchyi", "cell", "centroid", "ceps", "cepsinv", "changed2", "chebyshevpoly", "chn_S", "chn_a",
            "chn_k", "chuap", "cingoto", "cmplxprod", "cnkgoto", "combinv", "compilecsd", "compileorc", "compilestr", "compress", "compress2", "connect", "copya2ftab", "copyf2array", "cosseg", "cossegb",
            "cossegr", "cpsmidinn", "crossfm", "crossfmi", "crossfmpm", "crossfmpmi", "crosspm", "crosspmi", "ctlchn", "date", "dates", "dcblock2", "directory", "diskgrain", "distort", "doppler", "dust",
            "dust2", "ephasor", "eqfil", "evalstr", "exciter", "expcurve", "exprandi", "expsegb", "expsegba", "fareylen",
            "fareyleni", "faustaudio", "faustcompile", "faustctl", "faustgen", "fft", "fftinv", "ficlose", "filebit", "filescal", "filevalid", "flooper2", "fluidProgramSelect", "fluidSetInterpMethod",
            "fractalnoise", "framebuffer", "ftcps", "ftgenonce", "ftresize", "ftresizei", "ftsamplebank", "gainslider", "gaussi", "gausstrig", "genarray", "genarray_i", "gendy", "gendyc", "gendyx", "getcfg",
            "getcol", "getrow", "getseed", "harmon2", "harmon3", "harmon4", "hdf5read", "hdf5write", "hrtfearly", "hrtfmove", "hrtfmove2", "hrtfreverb", "hrtfstat", "hvs1", "hvs2", "hvs3", "i", "imagecreate",
            "imagefree", "imagegetpixel", "imageload", "imagesave", "imagesetpixel", "imagesize", "inleta", "inletf", "inletk", "inletkid", "inletv", "inrg", "insglobal", "insremot", "k", "linsegb", "log2",
            "logcurve", "loop_ge", "loop_gt", "loop_le", "loop_lt", "looptseg", "loopxseg", "loscilx", "lpform", "lposcila", "lposcilsa", "lposcilsa2", "mags", "maparray", "maparray_i", "max", "maxabs",
            "maxabsaccum", "maxaccum", "median", "mediank", "midglobal", "midichannelaftertouch", "midicontrolchange", "midifilestatus", "midipgm", "midipolyaftertouch", "midiprogramchange",
            "midremot", "min", "minabs", "minabsaccum", "minaccum", "minarray", "mincer", "mode", "modmatrix", "monitor", "moogladder2", "moogvcf2", "mp3bitrate", "mp3in", "mp3len", "mp3nchnls", "mp3scal",
            "mp3scal_check", "mp3scal_load", "mp3scal_load2", "mp3scal_play", "mp3scal_play2", "mp3sr", "mvchpf", "mvclpf1", "mvclpf2", "mvclpf3", "mvclpf4", "nchnls_hw", "nlalp", "nlfilt2", "nstance", "octmidib",
            "octmidinn", "olabuffer", "outleta", "outletf", "outletk", "outletkid", "outletv", "outrg", "p", "pan2", "part2txt", "partikkel", "partikkelget", "partikkelset", "partikkelsync", "passign", "paulstretch",
            "pchmidib", "pchmidinn", "pcount", "pdclip", "pdhalf", "pdhalfy", "pgmchn", "phs", "pindex", "pinker", "pitchac", "platerev", "plltrack", "pol2rect", "polynomial", "powershape", "prepiano", "print_type",
            "printf_i", "printks2", "ptable", "ptable3", "ptablei", "ptableiw", "ptablew", "ptrack", "pvs2array", "pvs2tab", "pvsbandp", "pvsbandr", "pvsbin", "pvsbuffer", "pvsbufread", "pvsbufread2", "pvsceps",
            "pvsdiskin", "pvsdisp", "pvsenvftw", "pvsfreeze", "pvsfromarray", "pvsfwrite", "pvsgain", "pvsgendy", "pvsin", "pvslock", "pvsmooth", "pvsmorph", "pvsosc", "pvsout", "pvspitch", "pvstanal", "pvswarp",
            "pwd", "pyassign", "pyassigni", "pyassignt", "pycall1", "pycall1i", "pycall1t", "pycall2", "pycall2i", "pycall2t", "pycall3", "pycall3i", "pycall3t", "pycall4", "pycall4i", "pycall4t", "pycall5", "pycall5i",
            "pycall5t", "pycall6", "pycall6i", "pycall6t", "pycall7", "pycall7i", "pycall7t", "pycall8", "pycall8i", "pycall8t", "pycalli", "pycalln", "pycallni", "pycallt", "pyeval", "pyevali", "pyevalt", "pyexec",
            "pyexeci", "pyexect", "pyinit", "pylassign", "pylassigni", "pylassignt", "pylcall", "pylcall1", "pylcall1i", "pylcall1t", "pylcall2", "pylcall2i", "pylcall2t", "pylcall3", "pylcall3i", "pylcall3t", "pylcall4",
            "pylcall4i", "pylcall4t", "pylcall5", "pylcall5i", "pylcall5t", "pylcall6", "pylcall6i", "pylcall6t", "pylcall7", "pylcall7i", "pylcall7t", "pylcall8", "pylcall8i", "pylcall8t", "pylcalli", "pylcalln", "pylcallni",
            "pylcallt", "pyleval", "pylevali", "pylevalt", "pylexec", "pylexeci", "pylexect", "pylrun", "pylruni", "pylrunt", "pyrun", "pyruni", "pyrunt", "qinf", "qnan", "r2c", "readf", "readfi", "readks", "readscore",
            "readscratch", "rect2pol", "remoteport", "remove", "resyn", "return", "rewindscore", "rfft", "rifft", "round", "scalearray", "scoreline", "scoreline_i", "serialBegin", "serialEnd", "serialFlush", "serialPrint",
            "serialRead", "serialWrite", "serialWrite_i", "setcol", "setrow", "setscorepos", "sflooper", "shiftin", "shiftout", "signalflowgraph", "signum", "slicearray", "slider16table", "slider16tablef", "slider32table",
            "slider32tablef", "slider64table", "slider64tablef", "slider8table", "slider8tablef", "sliderKawai", "sockrecv", "sockrecvs", "socksend", "socksends", "strcatk", "strchar", "strchark", "strecv", "strfromurl",
            "strindex", "strindexk", "strlen", "strlenk", "strlower", "strlowerk", "strrindex", "strrindexk", "strsub", "strsubk", "strupper", "strupperk", "stsend", "sumarray", "syncloop", "syncphasor", "system", "system_i",
            "tab2pvs", "tab_i", "tabifd", "table3kt", "tablefilter", "tablefilteri", "tableshuffle", "tableshufflei", "tabmorph", "tabmorpha", "tabmorphak", "tabmorphi", "tabplay", "tabsum", "tabw", "tabw_i", "tb0", "tb0_init",
            "tb1", "tb10", "tb11", "tb12", "tb13", "tb14", "tb15", "tb1_init", "tb2", "tb2_init", "tb3", "tb4", "tb5", "tb6", "tb7", "tb8", "tb9", "temposcal", "trandom", "transegb", "transegr", "trcross", "trfilter", "trhighest",
            "trlowest", "trmix", "trscale", "trshift", "trsplit", "unwrap", "urandom", "vactrol", "vadd_i", "vaddv_i", "vaget", "vaset", "vbap", "vbapg", "vbapgmove", "vbapmove", "vdel_k", "vdivv_i", "vexp_i", "vexpv_i", "vmult_i",
            "vmultv_i", "vosim", "vphaseseg", "vpow_i", "vpowv_i", "vsubv_i", "vtable1k", "wiiconnect", "wiidata", "wiirange", "wiisend", "window", "writescratch", "zkwm", 0
        };


        const char* const* k;

        if (tokenLength < 2 || tokenLength > 16)
            return false;

        else
            k = keywords;


        int i = 0;

        while (k[i] != 0)
        {
            if (token.compare (CharPointer_ASCII (k[i])) == 0)
                return true;

            ++i;
        }

        return false;
    }

    //==============================================================================
    int parseIdentifier (CodeDocument::Iterator& source) noexcept
    {
        int tokenLength = 0;
        String::CharPointerType::CharType possibleIdentifier [100];
        String::CharPointerType possible (possibleIdentifier);

        while (isIdentifierBody (source.peekNextChar()))
        {
            const juce_wchar c = source.nextChar();

            if (tokenLength < 20)
                possible.write (c);

            ++tokenLength;
        }

        if (tokenLength > 1 && tokenLength <= 16)
        {
            possible.writeNull();

            if (isReservedKeyword (String::CharPointerType (possibleIdentifier), tokenLength))
                return CsoundTokeniser::tokenType_builtInKeyword;
        }

        return CsoundTokeniser::tokenType_identifier;
    }

    //==============================================================================
    int readNextToken (CodeDocument::Iterator& source)
    {
        int result = tokenType_error;
        source.skipWhitespace();
        char firstChar = source.peekNextChar();

        switch (firstChar)
        {
            case 0:
                source.skip();
                break;

            case ';':
                source.skipToEndOfLine();
                result = tokenType_comment;
                break;

            case '"':
                //   case T('\''):
                skipQuotedString (source);
                result = tokenType_stringLiteral;
                break;


            case '/':
                //result = CPlusPlusCodeTokeniser::tokenType_operator;
                source.skip();

                if (source.peekNextChar() == '=')
                {
                    source.skip();
                }
                else if (source.peekNextChar() == '/')
                {
                    // result = CPlusPlusCodeTokeniser::tokenType_comment;
                    source.skipToEndOfLine();
                }
                else if (source.peekNextChar() == '*')
                {
                    source.skip();
                    result = tokenType_comment;
                    bool lastWasStar = false;

                    for (;;)
                    {
                        const juce_wchar c = source.nextChar();

                        if (c == 0 || (c == '/' && lastWasStar))
                            break;

                        lastWasStar = (c == '*');
                    }

                }

                break;


            case '<':
                source.skip();

                if ((source.peekNextChar() == 'C') ||
                    (source.peekNextChar() == '/'))
                {
                    skipCSDTag (source);
                    result = tokenType_csdTag;
                }

                break;

            default:
                if (isIdentifierStart (firstChar))
                {
                    result = parseIdentifier (source);
                }
                else
                    source.skip();

                break;
        }

        //jassert (result != tokenType_unknown);
        return result;
    }
};

#endif