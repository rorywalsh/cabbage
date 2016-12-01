/*
  Copyright (C) 2016 Rory Walsh

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

#ifndef CABBAGECOMMANDIDS_H_INCLUDED
#define CABBAGECOMMANDIDS_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

namespace CommandIDs
{
enum
{
    newProject              = 0x200010,
    open                    = 0x200020,
    closeDocument           = 0x200030,
    saveDocument            = 0x200040,
    saveDocumentAs          = 0x200041,
    settings				= 0x200042,

    closeProject            = 0x200051,
    saveProject             = 0x200060,
    saveAll                 = 0x200080,
    openInIDE               = 0x200072,
    saveAndOpenInIDE        = 0x200073,
    createNewExporter       = 0x20007d,

    runCode		            = 0x200076,
    exportAsSynth   		= 0x200077,
    exportAsEffect          = 0x200074,
    exportAsFMODSoundPlugin = 0x200078,
    stopCode			    = 0x200079,
    showProjectSettings     = 0x20007a,
    showProjectModules      = 0x20007b,
    enableLiveDebugger      = 0x20007c,

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


    showBuildTab            = 0x410001,
    cleanAll                = 0x410010,
    enableBuild             = 0x410011,
    showWarnings            = 0x410012,
    reinstantiateComp       = 0x410013,
    launchApp               = 0x410014,
    killApp                 = 0x410015,
    buildNow                = 0x41001a,
    toggleContinuousBuild   = 0x41001b,

    enableSnapToGrid        = 0x410016,
    zoomIn                  = 0x410017,
    zoomOut                 = 0x410018,
    zoomNormal              = 0x410019,
    spaceBarDrag            = 0x4100a1,

    nextError               = 0x4100b1,
    prevError               = 0x4100b2,

    loginLogout             = 0x4100c1,

    undo			= 0x512011,
    redo			= 0x512012,
    cut				= 0x512013,
    copy			= 0x512014,
    paste			= 0x512015,
    searchReplace	= 0x512080,
    columnEdit		= 0x512082,
    interfaceMode		= 0x512083,
    csoundMode			= 0x512084,
    genericMode			= 0x512085,
    cabbageMode			= 0x512086,
    whiteBackground		= 0x512018,
    blackBackground		= 0x512019,
    toggleComments		= 0x512020,
    insertFromRepo		= 0x512021,
    addFromRepo			= 0x512022,
    insertRecentEvent	= 0x512023,
    openPythonEditor	= 0x512024,
    fontType			= 0x512025,
    del					= 0x512026,
    selectAll			= 0x512027,
    deselectAll			= 0x512028,
    editMode			= 0x512029,

    showGenericWidgetWindow	= 0x612000,

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
static const Identifier csound("csound");
static const Identifier generic("generic");
static const Identifier cabbage("cabbage");
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
static const Identifier alertWindowBackground = "Editor - Alart Window Background";
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

// this array holds the names of all widgets taht can be controlled via a host and plugin editor GUI
class CabbageControlWidgetStrings : public StringArray
{
public:
    CabbageControlWidgetStrings()
    {
        add("hslider");
        add("hslider2");
        add("hslider3");
        add("rslider");
        add("vslider");
        add("vslider2");
        add("hrange");
        add("vrange");
        add("vslider3");
        add("combobox");
        add("checkbox");
        add("encoder");
        add("numberbox");
        add("xypad");
        add("button");
    }

    ~CabbageControlWidgetStrings() {}
};

// this array holds the names of all widgets that can only be controlled via the plugin editor GUI
class CabbageLayoutWidgetStrings : public StringArray
{
public:
    CabbageLayoutWidgetStrings()
    {
        add("form");
        add("image");
        add("socketsend");
        add("socketreceive");
        add("keyboard");
        add("gentable");
        add("csoundoutput");
        add("textbox");
        add("line");
        add("recordbutton");
        add("label");
        add("hostbpm");
        add("hosttime");
        add("hostplaying");
        add("hostppqpos");
        add("patmatrix");
        add("source");
        add("multitab");
        add("listbox");
        add("infobutton");
        add("filebutton");
        add("loadbutton");
        add("soundfiler");
        add("sourcebutton");
        add("texteditor");
        add("popupmenu");
        add("snapshot");
        add("table");
        add("pvsview");
        add("hostrecording");
        add("directorylist");
        add("transport");
        add("groupbox");
        add("fftdisplay");
        add("signaldisplay");
        add("scope");
        //sample widget
        add("stepper");
    }

    ~CabbageLayoutWidgetStrings() {}
};

class CabbageIdentifierStrings : public StringArray
{
public:
    CabbageIdentifierStrings()
    {
        add("outlinecolour");
        add("outlinecolor");
        add("tablecolour");
        add("tablecolor");
        add("tablegridcolour");
        add("tablegridcolor");
        add("alpha");
        add("bounds");
        add("fontcolour:1");
        add("fontcolor:1");
        add("fontcolour:0");
        add("fontcolor:0");
        add("fontcolour");
        add("fontcolor");
        add("size");
        add("items");
        add("pos");
        add("min");
        add("radiogroup");
        add("max");
        add("value");
        add("fontstyle");
        add("guirefresh");
        add("outlinethickness");
        add("linethickness");
        add("trackerthickness");
        add("populate");
        add("range");
        add("amprange");
        add("popuptext");
        add("address");
        add("scalex");
        add("scaley");
        add("rescale");
        add("rangex");
        add("rangey");
        add("plant");
        add("bold");
        add("highlightcolour");
        add("updaterate");
        add("channeltype");
        add("channels");
        add("channel");
        add("sliderincr");
        add("widgetarray");
        add("channelarray");
        add("valuetextbox");
		add("textbox");
        add("active");
        add("caption");
        add("kind");
        add("align");
        add("velocity");
        add("wrap");
        add("tablebackgroundcolour");
        add("backgroundcolor");
        add("backgroundcolour");
        add("mode");
        add("tablenumber");
        add("tablenumbers");
        add("fill");
        add("logger");
        add("file");
        add("corners");
        add("imgpath");
        add("shape");
        add("rotate");
        add("textcolour");
        add("textcolor");
        add("pluginid");
        add("trackercolour");
        add("trackercolor");
        add("menucolour");
        add("menucolor");
        add("popup");
        add("show");
        add("latched");
        add("displaytype");
        add("identchannel");
        add("visible");
        add("scrubberposition");
        add("scroll");
        add("zoom");
        add("signalvariable");
        add("samplerange");
        add("scrollbars");
        add("colour:0");
        add("colour:1");
        add("colour");
        add("color:0");
        add("color:1");
        add("color");
        add("text");
        add("middlec");
        add("gradient");
        add("imgfile");
        add("imgdebug");
        add("minenabled");
        add("maxenabled");
        add("keywidth");
        add("refreshfiles");
        add("currenttext");
        add("titlebarcolour");
//sample identifiers for stepper widget
        add("numberofsteps");
        add("stepbpm");
    }

    ~CabbageIdentifierStrings()
    {

    }

};

namespace CabbageIdentifierIds
{
// list of static consts for each identifiers
static const Identifier top = "top";
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
static const Identifier channelarray = "widgetarray";
static const Identifier identchannelarray = "identchannelarray";
static const Identifier outlinecolour = "outlinecolour";
static const Identifier fillcolour = "fillcolour";
static const Identifier fill = "fill";
static const Identifier bold = "bold";
static const Identifier textcolour = "textcolour";
static const Identifier trackercolour = "trackercolour";
static const Identifier tablecolour = "tablecolour";
static const Identifier fontcolour= "fontcolour";
static const Identifier onfontcolour= "onfontcolour";
static const Identifier menucolour = "menucolour";
static const Identifier colour = "colour";
static const Identifier oncolour = "oncolour";
static const Identifier tablebackgroundcolour = "tablebackgroundcolour";
static const Identifier backgroundcolour = "backgroundcolour";
static const Identifier items = "items";
static const Identifier text = "text";
static const Identifier popuptext = "popuptext";
static const Identifier allowboundsupdate = "allowboundsupdate";
static const Identifier range = "range";
static const Identifier sliderrange = "sliderrange";
static const Identifier amprange = "amprange";
static const Identifier caption = "caption";
static const Identifier basetype = "basetype";
static const Identifier imgslider = "imgslider";
static const Identifier imgsliderbg = "imgsliderbg";
static const Identifier imggroupbox = "imggroupbox";
static const Identifier imgbuttonon = "imgbuttonon";
static const Identifier imgbuttonoff = "imgbuttonoff";
static const Identifier textbox = "textbox";
static const Identifier valuetextbox = "valuetextbox";
static const Identifier name = "name";
static const Identifier active = "active";
static const Identifier type = "type";
static const Identifier imgdebug = "imgdebug";
static const Identifier imgfile = "imgfile";
static const Identifier parentdir = "parentdir";
static const Identifier corners = "corners";
static const Identifier tablegridcolour= "tablegridcolour";
static const Identifier sliderskew = "sliderskew";
static const Identifier rotate = "rotate";
static const Identifier pivotx = "pivotx";
static const Identifier pivoty = "pivoty";
static const Identifier sliderincr = "sliderincr";
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
static const Identifier include = "include";
static const Identifier radiogroup = "radiogroup";
static const Identifier tabpage = "tabpage";
static const Identifier filetype = "filetype";
static const Identifier workingdir = "workingdir";
static const Identifier signalvariable = "signalvariable";
static const Identifier author = "author";
static const Identifier xychannel = "xychannel";
static const Identifier guirefresh = "guirefresh";
static const Identifier identchannel = "identchannel";
static const Identifier identchannelmessage = "identchannelmessage";
static const Identifier visible = "visible";
static const Identifier linenumber = "linenumber";
static const Identifier scrubberposition = "scrubberposition";
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
static const Identifier fontstyle = "fontstyle";
static const Identifier minenabled = "minenabled";
static const Identifier maxenabled = "maxenabled";
static const Identifier keywidth = "keywidth";
static const Identifier refreshfiles = "refreshfiles";
static const Identifier currenttext = "currenttext";
static const Identifier titlebarcolour =  "titlebarcolour";
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
static const String checkbox = "checkbox";
static const String soundfiler = "sounfiler";
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
static const String directorylist = "directorylist";
static const String index = "index";
static const String xypad = "xypad";
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
static const String csoundoutput = "csoundoutput";
static const String fftdisplay = "fftdisplay";
static const String signaldisplay = "signaldisplay";

static const String csdfile = "csdfile";
};


// this array holds the names of all widgets that can only be controlled via the plugin editor GUI
class CabbagePopupWidgets : public StringPairArray
{
public:
    CabbagePopupWidgets()
    {
        set("Button", CabbageIdentifierIds::button);
        set("Button - File", CabbageIdentifierIds::filebutton);
        set("Button - Info", CabbageIdentifierIds::infobutton);
        set("Checkbox", CabbageIdentifierIds::checkbox);
        set("ComboBox", CabbageIdentifierIds::combobox);
        set("Csound Output", CabbageIdentifierIds::csoundoutput);
        set("Endless Encoder", CabbageIdentifierIds::encoder);
        set("Gentable", CabbageIdentifierIds::gentable);
        set("Horizontal Range Slider", CabbageIdentifierIds::hrange);
        set("Groupbox", CabbageIdentifierIds::groupbox);
        set("Image", CabbageIdentifierIds::image);
        set("Numberbox", CabbageIdentifierIds::numberbox);
        set("Signal Display", CabbageIdentifierIds::signaldisplay);
        set("Slider - Horizontal", CabbageIdentifierIds::hslider);
        set("Slider - Rotary", CabbageIdentifierIds::rslider);
        set("Slider - Vertical", CabbageIdentifierIds::vslider);
        set("Soundfiler", CabbageIdentifierIds::soundfiler);
        set("Text Box", CabbageIdentifierIds::textbox.toString());
        set("Text Editor", CabbageIdentifierIds::texteditor);
        set("Vertical Range Slider", CabbageIdentifierIds::vrange);
        set("XY Pad", CabbageIdentifierIds::xypad);
    }
};


class CabbageIdentifierPropertyStringPairs : public StringPairArray
{
public:
    CabbageIdentifierPropertyStringPairs()
    {
        set("X Position", CabbageIdentifierIds::left.toString());
        set("Y Position", CabbageIdentifierIds::top.toString());
        set("Width", CabbageIdentifierIds::width.toString());
        set("Height", CabbageIdentifierIds::height.toString());

        set("Pivot X", CabbageIdentifierIds::pivotx.toString());
        set("Pivot Y", CabbageIdentifierIds::pivoty.toString());
        set("Rotate", CabbageIdentifierIds::rotate.toString());

        set("Channel", CabbageIdentifierIds::channel.toString());
        set("Ident Channel", CabbageIdentifierIds::identchannel.toString());

        set("Minimum", CabbageIdentifierIds::min.toString());
        set("Maximum", CabbageIdentifierIds::max.toString());
        set("Skew", CabbageIdentifierIds::sliderskew.toString());
        set("Increment", CabbageIdentifierIds::sliderincr.toString());
        set("Value", CabbageIdentifierIds::value.toString());

        set("Text", CabbageIdentifierIds::text.toString());
        set("Popup Text", CabbageIdentifierIds::popuptext.toString());

        set("Active", CabbageIdentifierIds::active.toString());
        set("Visible", CabbageIdentifierIds::visible.toString());

        set("Font: On", CabbageIdentifierIds::onfontcolour.toString());
        set("Font: Off", CabbageIdentifierIds::fontcolour.toString());
        set("Font", CabbageIdentifierIds::fontcolour.toString());
        set("Menu Colour", CabbageIdentifierIds::menucolour.toString());
        set("Colour: Off", CabbageIdentifierIds::colour.toString());
        set("Colour", CabbageIdentifierIds::colour.toString());
        set("Outline", CabbageIdentifierIds::outlinecolour.toString());
		set("Tracker", CabbageIdentifierIds::trackercolour.toString());
		set("Text Colour", CabbageIdentifierIds::textcolour.toString());
		set("Value Box", CabbageIdentifierIds::valuetextbox.toString());
        set("Outline Thickness", CabbageIdentifierIds::linethickness.toString());
        set("Colour: On", CabbageIdentifierIds::oncolour.toString());
        set("Alpha", CabbageIdentifierIds::alpha.toString());
        set("Corners", CabbageIdentifierIds::corners.toString());
        set("Shape", CabbageIdentifierIds::shape.toString());
        set("File", CabbageIdentifierIds::file.toString());
        set("On Image", CabbageIdentifierIds::imgbuttonon.toString());
        set("Off Image", CabbageIdentifierIds::imgbuttonoff.toString());

    }
};


#endif  // CABBAGECOMMANDIDS_H_INCLUDED
