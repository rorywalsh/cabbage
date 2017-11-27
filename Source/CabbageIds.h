/*
  Copyright (C) 2016 Rory Walsh

  Cabbage is free software; you can redistribute it
  and/or modify it under the terms of the GNU General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  Cabbage is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU General Public
  License along with Csound; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
  02111-1307 USA
*/

#ifndef CABBAGECOMMANDIDS_H_INCLUDED
#define CABBAGECOMMANDIDS_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

namespace CommandIDs
{
    enum
    {
        openCabbagePatch        = 0x200010,
        newFile                 = 0x200020,
        open                    = 0x200023,
        openFromRPi             = 0x200021,
        closeDocument           = 0x200030,
        saveDocumentToRPi       = 0x200039,
        saveDocument            = 0x200040,
        saveDocumentAs          = 0x200041,
        settings                = 0x200042,
        examples                = 0x200043,
        showGraph               = 0x200044,
        saveGraph               = 0x200045,
        saveGraphAs             = 0x200046,

        closeProject            = 0x200051,
        saveProject             = 0x200060,
        saveAll                 = 0x200080,
        runDiagnostics          = 0x200072,
        saveAndOpenInIDE        = 0x200073,
        createNewExporter       = 0x20007d,

        startAudioGraph         = 0x200076,
        exportAsSynth           = 0x200077,
        exportAsEffect          = 0x200074,
        exportAsFMODSoundPlugin = 0x200078,
        stopAudioGraph          = 0x200079,
        clearConsole            = 0x20007a,
        showProjectModules      = 0x20007b,
        startLiveDebugger       = 0x20007c,

        closeWindow             = 0x201001,
        closeAllDocuments       = 0x201000,
        goToPreviousDoc         = 0x201002,
        goToNextDoc             = 0x201003,
        goToCounterpart         = 0x201004,
        deleteSelectedItem      = 0x201005,

        showFindPanel           = 0x2010a0,
        findSelection           = 0x2010a1,
        findNext                = 0x2010a2,
        findPrevious            = 0x2010a3,
        showReplacePanel        = 0x2010a4,


        showBuildTab            = 0x410001,
        cleanAll                = 0x410010,
        enableBuild             = 0x410011,
        showWarnings            = 0x410012,
        reinstantiateComp       = 0x410013,
        launchApp               = 0x410014,
        killApp                 = 0x410015,
        buildNow                = 0x41001a,
        nextTab                 = 0x41001b,

        enableSnapToGrid        = 0x410016,
        zoomIn                  = 0x410017,
        zoomOut                 = 0x410018,
        zoomInConsole           = 0x410117,
        zoomOutConsole          = 0x410118,
        zoomNormal              = 0x410019,
        spaceBarDrag            = 0x4100a1,

        nextError               = 0x4100b1,
        prevError               = 0x4100b2,

        about                   = 0x4100c1,
        cabbageHelp             = 0x4100c2,
        csoundHelp              = 0x4100c3,
        contextHelp             = 0x4100c4,

        undo            = 0x512011,
        redo            = 0x512012,
        cut             = 0x512013,
        copy            = 0x512014,
        paste           = 0x512015,
        searchReplace   = 0x512080,
        columnEdit      = 0x512082,
        interfaceMode       = 0x512083,
        csoundMode          = 0x512084,
        genericMode         = 0x512085,
        cabbageMode         = 0x512086,
        whiteBackground     = 0x512018,
        blackBackground     = 0x512019,
        toggleComments      = 0x512020,
        insertFromRepo      = 0x512021,
        addFromRepo         = 0x512022,
        insertRecentEvent   = 0x512023,
        openPythonEditor    = 0x512024,
        fontType            = 0x512025,
        del                 = 0x512026,
        selectAll           = 0x512027,
        deselectAll         = 0x512028,
        editMode            = 0x512029,

        showGenericWidgetWindow = 0x612000,

        lastCommandIDEntry
    };
}

namespace CommandCategories
{
    static const char* const general       = "General";
    static const char* const editing       = "Editing";
    static const char* const view          = "View";
    static const char* const edit          = "Edit";
    static const char* const windows       = "Windows";
}

// Handy list of static Identifiers..
namespace Ids
{
#define DECLARE_ID(name)      const Identifier name (#name)

    DECLARE_ID (name);
    DECLARE_ID (font);
    DECLARE_ID (colour);
    DECLARE_ID (csoundManualDirectory);
    DECLARE_ID (cabbagePlantDirectory);
    DECLARE_ID (cabbageExamplesDirectory);
    const Identifier ID ("id");
    const Identifier ID_uppercase ("ID");
    const Identifier class_ ("class");

#undef DECLARE_ID
};

namespace CabbageInterfaceModes
{
    static const Identifier csound ("csound");
    static const Identifier generic ("generic");
    static const Identifier cabbage ("cabbage");
};

// Handy list of static Identifiers..
namespace CabbageColourIds
{
    static const Identifier menuBarBackground = "Interface - MenuBar Background";
    static const Identifier menuBarText = "Interface - MenuBarText";
    static const Identifier menuBarMouseOverBackground = "Interface - MenuBar MouseOver Background";
    static const Identifier popupMenuMouseOverBackground = "Interface - PopupMenu MouseOver Background";
    static const Identifier popupMenuBackground = "Interface - PopupMenu Background";
    static const Identifier popupMenuText = "Interface - PopupMenu Text";
    static const Identifier popupMenuHighlightedText = "Interface - PopupMenu Highlighted Text";
    static const Identifier mainBackground = "Interface - Main Background";
    static const Identifier statusBar = "Interface - Status Bar";
    static const Identifier statusBarText = "Interface - Status Bar Text";
    static const Identifier propertyPanelBackground = "Interface - Property Panel Background";
    static const Identifier propertyLabelBackground = "Interface - Property Label Background";
    static const Identifier propertyLabelText = "Interface - Property Label Text";
    static const Identifier alertWindowBackground = "Interface - Alert Window Background";
    static const Identifier codeBackground = "Editor - Code Background";
    static const Identifier lineNumberBackground = "Editor - Line Numbers Background";
    static const Identifier lineNumbers = "Editor - Line Numbers";
    static const Identifier selectTextBackground = "Editor - Selected Text Background";
    static const Identifier caret = "Editor - Caret";
    static const Identifier identifierLiteral = "Editor - Identifier";
    static const Identifier stringLiteral = "Editor - Strings";
    static const Identifier keyword = "Editor - Keyword";
    static const Identifier comment = "Editor - Comment";
    static const Identifier numbers = "Editor - Numbers";
    static const Identifier csdtags = "Editor - Csd Tags";
    static const Identifier consoleText = "Console - Text";
    static const Identifier consoleBackground = "Console - Background";
    static const Identifier consoleOutline = "Console - Outline";
    static const Identifier genericPluginInterface = "Generic Plugin - Background";
}

// this array holds the names of all widgets that can be controlled via a host and plugin editor GUI
class CabbageControlWidgetStrings : public StringArray
{
public:
    CabbageControlWidgetStrings()
    {
        add ("hslider");
        add ("stringsequencer");
        add ("hslider2");
        add ("hslider3");
        add ("rslider");
        add ("vslider");
        add ("vslider2");
        add ("hrange");
        add ("vrange");
        add ("vslider3");
        add ("combobox");
        add ("checkbox");
        add ("encoder");
        add ("numberbox");
        add ("xypad");
        add ("button");
    }

    ~CabbageControlWidgetStrings() {}
};

// this array holds the names of all widgets that can only be controlled via the plugin editor GUI
class CabbageLayoutWidgetStrings : public StringArray
{
public:
    CabbageLayoutWidgetStrings()
    {
        add ("form");
        add ("image");
        add ("socketsend");
        add ("socketreceive");
        add ("keyboard");
        add ("gentable");
        add ("csoundoutput");
        add ("textbox");
        add ("label");
        add ("hostbpm");
        add ("hosttime");
        add ("hostplaying");
        add ("hostppqpos");
        add ("source");
        add ("infobutton");
        add ("stringsequencer");
        add ("filebutton");
        add ("loadbutton");
        add ("soundfiler");
        add ("texteditor");
        add ("groupbox");
        add ("fftdisplay");
        add ("signaldisplay");
        add ("line");
        add ("vmeter");
        add ("hmeter");
        //sample widget
        add ("stepper");
    }

    ~CabbageLayoutWidgetStrings() {}

};

// utility string arrays
class CabbageImageWidgetStrings : public StringArray
{
public:
    CabbageImageWidgetStrings()
    {
        add ("rslider");
        add ("vslider");
        add ("hslider");
        add ("checkbox");
        add ("button");
        add ("filebutton");
        add ("groupbox");
        add ("image");
    }

    ~CabbageImageWidgetStrings() {}
};

class CabbageAmpRangeIdentifiers : public StringArray
{
public:
    CabbageAmpRangeIdentifiers()
    {
        add ("amprange_min");
        add ("amprange_tablenumber");
        add ("amprange_quantise");
        add ("amprange_max");
    }

    ~CabbageAmpRangeIdentifiers() {}
};

class CabbageScrubberPositionIdentifiers : public StringArray
{
public:
    CabbageScrubberPositionIdentifiers()
    {
        add ("scrubberposition_sample");
        add ("scrubberposition_table");
    }

    ~CabbageScrubberPositionIdentifiers() {}
};
// this array holds all identifier strings
class CabbageIdentifierStrings : public StringArray
{
public:
    CabbageIdentifierStrings()  // longest strings appear first otherwise identifiers will be missed in parsing
    {
        add ("scrubberposition_sample");
        add ("scrubberposition_table");
        add ("tablebackgroundcolour");
        add ("arrowbackgroundcolour");
        add ("amprange_tablenumber");
        add ("textboxoutlinecolour");
        add ("surrogatelinenumber");
        add ("mouseoeverkeycolour");
        add ("keyseparatorcolour");
        add ("amprange_quantise");
        add ("currentnotecolour");
        add ("expandedmacrotext");
        add ("backgroundcolour");
        add ("trackerthickness");
        add ("activecellcolour");
        add ("outlinethickness");
        add ("scrubberposition");
        add ("whitenotecolour");
        add ("blacknotecolour");
        add ("highlightcolour");
        add ("tablegridcolour");
        add ("backgroundcolor");
        add ("showstepnumbers");
        add ("titlebarcolour");
        add ("tablegridcolor");
        add ("signalvariable");
        add ("overlaycolour");
        add ("linethickness");
        add ("outlinecolour");
        add ("numberofsteps");
        add ("textboxcolour");
        add ("trackercolour");
        add ("valuetextbox");
        add ("fontcolour:1");
        add ("amprange_min");
        add ("amprange_max");
        add ("trackercolor");
        add ("refreshfiles");
        add ("tablenumbers");
        add ("identchannel");
        add ("fontcolour:0");
        add ("tablecolour:");
        add ("metercolour:");
        add ("popuppostfix");
        add ("orientation");
        add ("tablecolour");
        add ("metercolour");
        add ("widgetarray");
        add ("arrowcolour");
        add ("widgetarray");
        add ("channeltype");
        add ("samplerange");
        add ("fontcolor:0");
        add ("tablenumber");
        add ("displaytype");
        add ("fontcolor:1");
        add ("basechannel");
        add ("popupprefix");
        add ("macronames");
        add ("fontcolour");
        add ("textcolour");
        add ("updaterate");
        add ("guirefresh");
        add ("tablecolor");
        add ("radiogroup");
        add ("ballcolour");
        add ("menucolor");
        add ("cellwidth");
        add ("cellheight");
        add ("popuptext");
        add ("textcolor");
        add ("fontstyle");
        add ("fontcolor");
        add ("arraysize");
        add ("increment");
        add ("namespace");
        add ("populate");
        add ("keywidth");
        add ("pluginid");
        add ("imgdebug");
        add ("colour:0");
        add ("colour:1");
        add ("channels");
        add ("velocity");
        add ("amprange");
        add ("color:0");
        add ("caption");
        add ("textbox");
        add ("corners");
        add ("color:1");
        add ("middlec");
        add ("channel");
        add ("imgpath");
        add ("address");
        add ("imgfile");
        add ("visible");
        add ("latched");
        add ("rescale");
        add ("import");
        add ("bounds");
        add ("colour");
        add ("rangey");
        add ("rangex");
        add ("rotate");
        add ("active");
        add ("shape");
        add ("popup");
        add ("alpha");
        add ("items");
        add ("color");
        add ("value");
        add ("range");
        add ("plant");
        add ("align");
        add ("file");
        add ("wrap");
        add ("text");
        add ("crop");
        add ("zoom");
        add ("size");
        add ("show");
        add ("kind");
        add ("fill");
        add ("mode");
        add ("min");
        add ("max");
        add ("pos");
        add ("bpm");
    }

    NamedValueSet getValueSet()
    {
        NamedValueSet valueSet;

        for (int i = 0 ; i < this->size() ; i++)
        {
            valueSet.set (this->getReference (i), i);
        }
    }

};

namespace CabbageWidgetTypes
{
    //list of static consts for types of widgets
    static const String combobox = "combobox";
    static const String numberbox = "numberbox";
    static const String rslider = "rslider";
    static const String hslider = "hslider";
    static const String hslider2 = "hslider2";
    static const String hslider3 = "hslider3";
    static const String vslider = "vslider";
    static const String vslider2 = "vslider2";
    static const String vslider3 = "vslider3";
    static const String vrange = "vrange";
    static const String hrange = "hrange";
    static const String stringsequencer = "stringsequencer";
    static const String checkbox = "checkbox";
    static const String soundfiler = "soundfiler";
    static const String button = "button";
    static const String listbox = "listbox";
    static const String infobutton = "infobutton";
    static const String filebutton = "filebutton";
    static const String loadbutton = "loadbutton";
    static const String sourcebutton = "sourcebutton";
    static const String texteditor = "texteditor";
    static const String encoder = "encoder";
    static const String table = "table";
    static const String gentable = "gentable";
    static const String groupbox = "groupbox";
    static const String image = "image";
    static const String popupmenu = "popupmenu";
    static const String label = "label";
    static const String keyboard = "keyboard";
    static const String oscserver = "oscserver";
    static const String form = "form";
    static const Identifier textbox = "textbox";
    static const String directorylist = "directorylist";
    static const String xypad = "xypad";
    static const String csoundoutput = "csoundoutput";
    static const String fftdisplay = "fftdisplay";
    static const String signaldisplay = "signaldisplay";
    static const String line = "line";
    static const String vmeter = "vmeter";
    static const String hmeter = "hmeter";
};
namespace CabbageIdentifierIds
{
    // list of static consts for each identifiers
    static const Identifier top = "top";
    static const Identifier bpm = "bpm";
    static const Identifier left = "left";
    static const Identifier width = "width";
    static const Identifier height = "height";
    static const Identifier min = "min";
    static const Identifier max = "max";
    static const Identifier minvalue = "minvalue";
    static const Identifier maxvalue = "maxvalue";
    static const Identifier value = "value";
    static const Identifier alpha = "alpha";
    static const Identifier scalex = "scalex";
    static const Identifier scaley = "scaley";
    static const Identifier rescale = "scale";
    static const Identifier channel = "channel";
    static const Identifier expandedmacrotext = "expandedmacrotext";
    static const Identifier macronames = "macronames";
    static const Identifier widgetarray = "widgetarray";
    static const Identifier basechannel = "basechannel";
    static const Identifier arraysize = "arraysize";
    static const Identifier identchannelarray = "identchannelarray";
    static const Identifier outlinecolour = "outlinecolour";
    static const Identifier fillcolour = "fillcolour";
    static const Identifier whitenotecolour = "whitenotecolour";
    static const Identifier blacknotecolour = "blacknotecolour";
    static const Identifier arrowcolour = "arrowcolour";
    static const Identifier arrowbackgroundcolour = "arrowbackgroundcolour";
    static const Identifier keyseparatorcolour = "keyseparatorcolour";
    static const Identifier fill = "fill";
    static const Identifier bold = "bold";
    static const Identifier textcolour = "textcolour";
    static const Identifier trackercolour = "trackercolour";
    static const Identifier tablecolour = "tablecolour";
    static const Identifier metercolour = "metercolour";
    static const Identifier fontcolour = "fontcolour";
    static const Identifier onfontcolour = "onfontcolour";
    static const Identifier menucolour = "menucolour";
    static const Identifier colour = "colour";
    static const Identifier oncolour = "oncolour";
    static const Identifier activecellcolour = "activecellcolour";
    static const Identifier tablebackgroundcolour = "tablebackgroundcolour";
    static const Identifier overlaycolour = "overlaycolour";
    static const Identifier backgroundcolour = "backgroundcolour";
    static const Identifier items = "items";
    static const String numberofsteps = "numberofsteps";
    static const Identifier text = "text";
    static const Identifier ballcolour = "ballcolour";
    static const Identifier popuptext = "popuptext";
    static const Identifier allowboundsupdate = "allowboundsupdate";
    static const Identifier range = "range";
    static const Identifier sliderrange = "sliderrange";
    static const Identifier amprange = "amprange";
    static const Identifier amprange_min = "amprange_min";
    static const Identifier amprange_max = "amprange_max";
    static const Identifier amprange_tablenumber = "amprange_tablenumber";
    static const Identifier amprange_quantise = "amprange_quantise";
    static const Identifier caption = "caption";
    static const Identifier basetype = "basetype";
    static const Identifier imgslider = "imgslider";
    static const Identifier imgsliderbg = "imgsliderbg";
    static const Identifier imggroupbox = "imggroupbox";
    static const Identifier imgbuttonon = "imgbuttonon";
    static const Identifier imgbuttonoff = "imgbuttonoff";
    static const Identifier valuetextbox = "valuetextbox";
    static const Identifier textboxcolour = "textboxcolour";
    static const Identifier textboxoutlinecolour = "textboxoutlinecolour";
    static const Identifier name = "name";
    static const Identifier crop = "crop";
    static const Identifier cropx = "cropx";
    static const Identifier cropy = "cropy";
    static const Identifier cropwidth = "cropwidth";
    static const Identifier cropheight = "cropheight";
    static const Identifier active = "active";
    static const Identifier type = "type";
    static const Identifier imgdebug = "imgdebug";
    static const Identifier imgfile = "imgfile";
    static const Identifier parentdir = "parentdir";
    static const Identifier orientation = "orientation";
    static const Identifier corners = "corners";
    static const Identifier tablegridcolour = "tablegridcolour";
    static const Identifier sliderskew = "sliderskew";
    static const Identifier rotate = "rotate";
    static const Identifier pivotx = "pivotx";
    static const Identifier pivoty = "pivoty";
    static const Identifier increment = "increment";
    static const Identifier midichan = "midichan";
    static const Identifier midictrl = "midictrl";
    static const Identifier kind = "kind";
    static const Identifier decimalplaces = "decimalplaces";
    static const Identifier mode = "mode";
    static const Identifier shape = "shape";
    static const Identifier channeltype = "channeltype";
    static const Identifier comborange = "comborange";
    static const Identifier populate = "populate";
    static const Identifier popup = "popup";
    static const Identifier gradient = "gradient";
    static const Identifier middlec = "middlec";
    static const Identifier displaytype = "displaytype";
    static const Identifier imgpath = "imgpath";
    static const Identifier plant = "plant";
    static const Identifier parentcomponent = "parentcomponent";
    static const Identifier trackerthickness = "trackerthickness";
    static const Identifier outlinethickness = "outlinethickness";
    static const Identifier linethickness = "linethickness";
    static const Identifier tablenumber = "tablenumber";
    static const Identifier tableconfig = "tableconfig";
    static const Identifier resizemode = "resizemode";
    static const Identifier resize = "resize";
    static const Identifier drawmode = "drawmode";
    static const Identifier readonly = "readonly";
    static const Identifier xyautoindex = "xyautoindex";
    static const Identifier file = "file";
    static const Identifier latched = "latched";
    static const Identifier xchannel = "xchannel";
    static const Identifier ychannel = "ychannel";
    static const Identifier minx = "minx";
    static const Identifier miny = "miny";
    static const Identifier maxx = "maxx";
    static const Identifier maxy = "maxy";
    static const Identifier logger = "logger";
    static const Identifier stack = "stack";
    static const Identifier valuex = "valuex";
    static const Identifier valuey = "valuey";
    static const Identifier pluginid = "pluginid";
    static const Identifier tabs = "tabs";
    static const Identifier wrap = "wrap";
    static const Identifier align = "align";
    static const Identifier tabbed = "tabbed";
    static const Identifier rangey = "rangey";
    static const Identifier rangex = "rangex";
    static const Identifier import = "import";
    static const Identifier importfiles = "importfiles";
    static const Identifier radiogroup = "radiogroup";
    static const Identifier tabpage = "tabpage";
    static const Identifier filetype = "filetype";
    static const Identifier workingdir = "workingdir";
    static const Identifier signalvariable = "signalvariable";
    static const Identifier mouseoeverkeycolour = "mouseoeverkeycolour";
    static const Identifier author = "author";
    static const Identifier xychannel = "xychannel";
    static const Identifier guirefresh = "guirefresh";
    static const Identifier identchannel = "identchannel";
    static const Identifier identchannelmessage = "identchannelmessage";
    static const Identifier visible = "visible";
    static const Identifier linenumber = "linenumber";
    static const Identifier surrogatelinenumber = "surrogatelinenumber";
    static const Identifier scrubberposition = "scrubberposition";
    static const Identifier scrubberposition_sample = "scrubberposition_sample";
    static const Identifier scrubberposition_table = "scrubberposition_table";
    static const Identifier startpoint = "startpoint";
    static const Identifier endpoint = "endpoint";
    static const Identifier zoom = "zoom";
    static const Identifier visiblelength = "visiblelength";
    static const Identifier samplerange = "samplerange";
    static const Identifier startpos = "startpos";
    static const Identifier endpos = "endpos";
    static const Identifier show = "show";
    static const Identifier child = "child";
    static const Identifier scrollbars = "scrollbars";
    static const Identifier socketport = "socketport";
    static const Identifier socketaddress = "socketaddress";
    static const Identifier ffttablenumber = "ffttablenumber";
    static const Identifier velocity = "velocity";
    static const Identifier highlightcolour = "highlightcolour";
    static const Identifier update = "update";
    static const Identifier fontstyle = "fontstyle";
    static const Identifier minenabled = "minenabled";
    static const Identifier maxenabled = "maxenabled";
    static const Identifier keywidth = "keywidth";
    static const Identifier refreshfiles = "refreshfiles";
    static const Identifier currenttext = "currenttext";
    static const Identifier popupprefix = "popupprefix";
    static const Identifier popuppostfix = "popuppostfix";
    static const Identifier titlebarcolour =  "titlebarcolour";
    static const String cellwidth = "cellwidth";
    static const String cellheight = "cellheight";
    static const String csdfile = "csdfile";
    static const String index = "index";
    static const String stringchannel = "string";
    static const String hostbpm = "HOST_BPM";
    static const String timeinseconds = "TIME_IN_SECONDS";
    static const String isplaying = "IS_PLAYING";
    static const String isrecording = "IS_RECORDING";
    static const String hostppqpos = "HOST_PPQ_POS";
    static const String timeinsamples = "TIME_IN_SAMPLES";
    static const String timeSigDenom = "TIME_SIG_DENOM";
    static const String timeSigNum = "TIME_SIG_NUM";
    static const String mousex = "MOUSE_X";
    static const String mousey = "MOUSE_Y";
    static const String mousedownleft = "MOUSE_DOWN_LEFT";
    static const String mousedownright = "MOUSE_DOWN_RIGHT";
    static const String mousedownlmiddle = "MOUSE_DOWN_MIDDLE";
    static const String updaterate = "updaterate";
    static const String showstempnumbers = "showstepnumbers";
    static const String nsp = "namespace";

};


// this array holds the names of all widgets that can inserted when in GUI edit mode
class CabbagePopupWidgets : public StringPairArray
{
public:
    CabbagePopupWidgets()
    {
        set ("Button", CabbageWidgetTypes::button);
        set ("Button - File", CabbageWidgetTypes::filebutton);
        set ("Button - Info", CabbageWidgetTypes::infobutton);
        set ("Checkbox", CabbageWidgetTypes::checkbox);
        set ("ComboBox", CabbageWidgetTypes::combobox);
        set ("Csound Output", CabbageWidgetTypes::csoundoutput);
        set ("Endless Encoder", CabbageWidgetTypes::encoder);
        set ("Gentable", CabbageWidgetTypes::gentable);
        set ("Groupbox", CabbageWidgetTypes::groupbox);
        set ("Horizontal Meter", CabbageWidgetTypes::hmeter);
        set ("Image", CabbageWidgetTypes::image);
        set ("Keyboard", CabbageWidgetTypes::keyboard);
        set ("Label", CabbageWidgetTypes::label);
        set ("Numberbox", CabbageWidgetTypes::numberbox);
        set ("Signal Display", CabbageWidgetTypes::signaldisplay);
        set ("Slider - Horizontal", CabbageWidgetTypes::hslider);
        set ("Slider - Rotary", CabbageWidgetTypes::rslider);
        set ("Slider - Vertical", CabbageWidgetTypes::vslider);
        set ("Slider - Range - Vertical", CabbageWidgetTypes::vrange);
        set ("Slider - Range - Horizontal", CabbageWidgetTypes::hrange);
        set ("Soundfiler", CabbageWidgetTypes::soundfiler);
        set ("String Sequencer", CabbageWidgetTypes::stringsequencer);
        set ("Text Box", CabbageWidgetTypes::textbox.toString());
        set ("Text Editor", CabbageWidgetTypes::texteditor);
        set ("Vertical Meter", CabbageWidgetTypes::vmeter);
        set ("XY Pad", CabbageWidgetTypes::xypad);
    }
};

class CabbageIdentifierPropertyStringPairs : public StringPairArray
{
public:
    CabbageIdentifierPropertyStringPairs()
    {
        set ("X Position", CabbageIdentifierIds::left.toString());
        set ("Y Position", CabbageIdentifierIds::top.toString());
        set ("Width", CabbageIdentifierIds::width.toString());
        set ("Height", CabbageIdentifierIds::height.toString());
        set ("Pivot X", CabbageIdentifierIds::pivotx.toString());
        set ("Pivot Y", CabbageIdentifierIds::pivoty.toString());
        set ("Rotate", CabbageIdentifierIds::rotate.toString());
        set ("Channel", CabbageIdentifierIds::channel.toString());
        set ("Ident Channel", CabbageIdentifierIds::identchannel.toString());
        set ("Minimum", CabbageIdentifierIds::min.toString());
        set ("Maximum", CabbageIdentifierIds::max.toString());
        set ("Skew", CabbageIdentifierIds::sliderskew.toString());
        set ("Increment", CabbageIdentifierIds::increment.toString());
        set ("Value", CabbageIdentifierIds::value.toString());
        set ("Text", CabbageIdentifierIds::text.toString());
        set ("Popup Text", CabbageIdentifierIds::popuptext.toString());
        set ("Active", CabbageIdentifierIds::active.toString());
        set ("Visible", CabbageIdentifierIds::visible.toString());
        set ("Font: On", CabbageIdentifierIds::onfontcolour.toString());
        set ("Font: Off", CabbageIdentifierIds::fontcolour.toString());
        set ("Font", CabbageIdentifierIds::fontcolour.toString());
        set ("Menu Colour", CabbageIdentifierIds::menucolour.toString());
        set ("Colour: Off", CabbageIdentifierIds::colour.toString());
        set ("Colour", CabbageIdentifierIds::colour.toString());
        set ("Outline", CabbageIdentifierIds::outlinecolour.toString());
        set ("Tracker", CabbageIdentifierIds::trackercolour.toString());
        set ("Soundfiler Background", CabbageIdentifierIds::tablebackgroundcolour.toString());
        set ("Text Colour", CabbageIdentifierIds::textcolour.toString());
        set ("Value Box", CabbageIdentifierIds::valuetextbox.toString());
        set ("Value Box Colour", CabbageIdentifierIds::textboxcolour.toString());
        set ("Value Box Outline", CabbageIdentifierIds::textboxoutlinecolour.toString());
        set ("Outline Thickness", CabbageIdentifierIds::outlinethickness.toString());
        set ("Line Thickness", CabbageIdentifierIds::linethickness.toString());
        set ("Colour: On", CabbageIdentifierIds::oncolour.toString());
        set ("White Notes", CabbageIdentifierIds::whitenotecolour.toString());
        set ("Black Notes", CabbageIdentifierIds::blacknotecolour.toString());
        set ("Key Separator", CabbageIdentifierIds::keyseparatorcolour.toString());
        set ("Arrows Background", CabbageIdentifierIds::arrowbackgroundcolour.toString());
        set ("Arrows", CabbageIdentifierIds::arrowcolour.toString());
        set ("Alpha", CabbageIdentifierIds::alpha.toString());
        set ("Zoom", CabbageIdentifierIds::zoom.toString());
        set ("Radio Group", CabbageIdentifierIds::radiogroup.toString());
        set ("Corners", CabbageIdentifierIds::corners.toString());
        set ("Shape", CabbageIdentifierIds::shape.toString());
        set ("File", CabbageIdentifierIds::file.toString());
        set ("Image File", CabbageIdentifierIds::file.toString());
        set ("Text Align", CabbageIdentifierIds::align.toString());
        set ("Align", CabbageIdentifierIds::align.toString());
        set ("Velocity", CabbageIdentifierIds::velocity.toString());
        set ("Min: X", CabbageIdentifierIds::minx.toString());
        set ("Max: X", CabbageIdentifierIds::maxx.toString());
        set ("Min: Y", CabbageIdentifierIds::miny.toString());
        set ("Max: Y", CabbageIdentifierIds::maxy.toString());
        set ("Value X", CabbageIdentifierIds::valuex.toString());
        set ("Value Y", CabbageIdentifierIds::valuey.toString());
        set ("Value Min", CabbageIdentifierIds::minvalue.toString());
        set ("Value Max", CabbageIdentifierIds::maxvalue.toString());
        set ("Background", CabbageIdentifierIds::backgroundcolour.toString());
        set ("Ball", CabbageIdentifierIds::ballcolour.toString());
        set ("Mode", CabbageIdentifierIds::mode.toString());
        set ("Base Channel", CabbageIdentifierIds::basechannel.toString());
        set ("Array Size", CabbageIdentifierIds::arraysize.toString());
        set ("Rotary Image", CabbageIdentifierIds::imgslider.toString());
        set ("Background Image", CabbageIdentifierIds::imgsliderbg.toString());
        set ("Groupbox Image", CabbageIdentifierIds::imggroupbox.toString());
        set ("On Image", CabbageIdentifierIds::imgbuttonon.toString());
        set ("Off Image", CabbageIdentifierIds::imgbuttonoff.toString());
        set ("Mouse Over", CabbageIdentifierIds::mouseoeverkeycolour.toString());
        set ("Max Amp", CabbageIdentifierIds::amprange_max.toString());
        set ("Min Amp", CabbageIdentifierIds::amprange_min.toString());
        set ("Table No.", CabbageIdentifierIds::amprange_tablenumber.toString());
        set ("Quantise", CabbageIdentifierIds::amprange_quantise.toString());
        set ("Table Numbers", CabbageIdentifierIds::tablenumber.toString());
        set ("Tables", CabbageIdentifierIds::tablecolour.toString());
        set ("Meter", CabbageIdentifierIds::metercolour.toString());
        set ("Table Grid", CabbageIdentifierIds::tablegridcolour.toString());
        set ("Table Background", CabbageIdentifierIds::tablebackgroundcolour.toString());
        set ("Overlay Colour", CabbageIdentifierIds::overlaycolour.toString());
        set ("Fill", CabbageIdentifierIds::fill.toString());
        set ("Zoom", CabbageIdentifierIds::zoom.toString());
        set ("Start Index", CabbageIdentifierIds::startpos.toString());
        set ("End Index", CabbageIdentifierIds::endpos.toString());
        set ("Scrubber Table", CabbageIdentifierIds::scrubberposition_table.toString());
        set ("Scrubber Pos", CabbageIdentifierIds::scrubberposition_sample.toString());
        set ("Orientation", CabbageIdentifierIds::orientation.toString());
        set ("Populate", CabbageIdentifierIds::populate.toString());
        set ("Channel Type", CabbageIdentifierIds::channeltype.toString());
        set ("Cell Width", CabbageIdentifierIds::cellwidth);
        set ("Cell Height", CabbageIdentifierIds::cellheight);
    }
};


#endif  // CABBAGECOMMANDIDS_H_INCLUDED
