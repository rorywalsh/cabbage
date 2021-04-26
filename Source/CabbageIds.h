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
        newTextFile             = 0x200022,
        open                    = 0x200023,
        openFolder              = 0x200024,
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
		toggleProperties		= 0x200047,
		importTheme				= 0x200048,
		exportTheme				= 0x200049,
        closeProject            = 0x200051,
        saveProject             = 0x200060,
        saveAll                 = 0x200080,
        runDiagnostics          = 0x200072,
        startAudioGraph         = 0x200076,
        exportAsVSTSynth        = 0x200077,
        exportAsVSTEffect       = 0x200074,
        exportAsVST3Synth       = 0x2000771,
        exportAsVST3Effect      = 0x2000742,
        exportAsAUSynth         = 0x200177,
        exportAsAUEffect        = 0x200174,
        exportAsFMODFxPlugin = 0x200075,
        exportAsFMODSoundPlugin = 0x200078,
        stopAudioGraph          = 0x200079,
        clearConsole            = 0x20007a,
        exportAsAUMIDIFx         = 0x20017b,
        exportAsVCVRackModule   = 0x022017c,
        exportAsStandaloneApp   = 0x022017d,
        closeAllDocuments           = 0x201000,
        exportAsVSTEffectEncrypted  = 0x201002,
        exportAsVSTSynthEncrypted   = 0x201003,
        exportAsVST3EffectEncrypted  = 0x2010021,
        exportAsVST3SynthEncrypted   = 0x2010032,
        exportAsAUEffectEncrypted   = 0x201004,
        exportAsAUSynthEncrypted    = 0x201005,
        showFindPanel           = 0x2010a0,
        findSelection           = 0x2010a1,
        findNext                = 0x2010a2,
        findPrevious            = 0x2010a3,
        showReplacePanel        = 0x2010a4,
        buildNow                = 0x41001a,
        buildNoConnect          = 0x41001c,
        nextTab                 = 0x41001b,
        zoomIn                  = 0x410017,
        zoomOut                 = 0x410018,
        zoomInConsole           = 0x410117,
        zoomOutConsole          = 0x410118,
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
        toggleComments      = 0x512020,
        del                 = 0x512026,
        selectAll           = 0x512027,
        deselectAll         = 0x512028,
        editMode            = 0x512029,
        toggleFileBrowser   = 0x512030,

        showGenericWidgetWindow = 0x612000,
        batchConvertExamplesAU    = 0x612001,
        batchConvertExamplesVST    = 0x612003,
        showConsole                 = 0x612004,
        showPluginListEditor    = 0x612005,
        autoReloadFromDisk    = 0x612006,
        addCabbageSection    = 0x612007,
        convertToCamelCase    = 0x612008,
        restartAudioDevice    = 0x612009,
        convertToLowerCase    = 0x6120011,

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
    static const Identifier menuBarText = "Interface - MenuBar Text";
    static const Identifier menuBarMouseOverBackground = "Interface - MenuBar MouseOver Background";
    static const Identifier popupMenuMouseOverBackground = "Interface - PopupMenu MouseOver Background";
    static const Identifier popupMenuBackground = "Interface - PopupMenu Background";
    static const Identifier popupMenuText = "Interface - PopupMenu Text";
    static const Identifier popupMenuHighlightedText = "Interface - PopupMenu Highlighted Text";
    static const Identifier mainBackground = "Interface - Main Background";
	static const Identifier statusBar = "Interface - Status Bar";
	static const Identifier fileTabBar = "Interface - File Tab Bar";
	static const Identifier fileTabButton = "Interface - File Tab Button";
	static const Identifier fileTabText = "Interface - File Tab Text";
	static const Identifier fileTabPlayButton = "Interface - File Tab Play Button";
	static const Identifier patcher = "Interface - Patcher";
	static const Identifier statusBarText = "Interface - Status Bar Text";
    static const Identifier propertyPanelBackground = "Interface - Property Panel Background";
    static const Identifier propertyLabelBackground = "Interface - Property Label Background";
    static const Identifier propertyLabelText = "Interface - Property Label Text";
    static const Identifier alertWindowBackground = "Interface - Alert Window Background";
    static const Identifier codeBackground = "Editor - Code Background";
    static const Identifier scrollbars = "Editor - Scrollbars";
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
        add ("eventsequencer");
        add ("hslider2");
        add ("hslider3");
        add ("rslider");
        add ("vslider");
        add ("vslider2");
        add ("hrange");
        add ("vrange");
        add ("vslider3");
        add ("optionbutton");
        add ("combobox");
        add ("checkbox");
        add ("encoder");
        add ("xypad");
        add ("button");
		add ("nslider"); //fix for stupid live issue with number boxes
    }

    ~CabbageControlWidgetStrings() {}
};

// this array holds the names of all widgets that can only be controlled via the plugin editor GUI
class CabbageLayoutWidgetStrings : public StringArray
{
public:
    CabbageLayoutWidgetStrings()
    {
		add ("keyboarddisplay");
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
        add ("eventsequencer");
        add ("filebutton");
        add ("loadbutton");
        add ("listbox");
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
        add ("doubleClickTogglesEdit");
        add ("scrubberposition_table");
        add ("tableBackgroundColour");
        add ("arrowBackgroundColour");
        add ("amprange_tablenumber");
        add ("textBoxOutlineColour");
        add ("trackerOutsideRadius");
        add ("trackerInsideRadius");
        add ("surrogatelinenumber");
        add ("mouseOverKeyColour");
        add ("keypressBaseOctave");
        add ("keySeparatorColour");
        add ("valueTextBoxBounds");
        add ("amprange_quantise");
        add ("currentNoteColour");
        add ("backgroundColour");
        add ("trackerThickness");
        add ("activeCellColour");
        add ("outlineThickness");
        add ("scrubberPosition");
        add ("titleBarGradient");
        add ("mouseInteraction");
        add ("whiteNoteColour");
        add ("blackNoteColour");
        add ("highlightColour");
        add ("tableGridColour");
        add ("trackerBgColour");
        add ("backgroundColor");
        add ("showStepNumbers");
        add ("markerThickness");
        add ("titleBarColour");
        add ("tableGridColor");
        add ("signalVariable");
        add ("protectedItems");
        add ("overlayColour");
        add ("keydownColour");
        add ("lineThickness");
        add ("outlineColour");
        add ("numberOfSteps");
        add ("textBoxColour");
        add ("trackerColour");
        add ("trackerRadius");
        add ("sliderBounds");
        add ("markerColour");
        add ("valueTextBox");
        add ("fontColour:1");
        add ("amprange_min");
        add ("amprange_max");
        add ("trackerColor");
        add ("refreshFiles");
        add ("tableNumbers");
        add ("identChannel");
        add ("fontColour:0");
        add ("macroStrings");
        add ("tableColour:");
        add ("meterColour:");
        add ("popupPostfix");
        add ("manufacturer");
        add ("valuePostfix");
        add ("showSrubber");
        add ("markerStart");
		add ("keyReleased");
        add ("orientation");
        add ("tableColour");
        add ("meterColour");
        add ("widgetArray");
        add ("arrowColour");
        add ("channelType");
        add ("sampleRange");
        add ("fontColor:0");
        add ("tableNumber");
        add ("displayType");
        add ("fontColor:1");
        add ("baseChannel");
        add ("popupPrefix");
        add ("automatable");
        add ("valuePrefix");
        add ("caretColour");
        add ("automation");
        add ("autoUpdate");
        add ("matrixSize");
        add ("fontColour");
        add ("textColour");
        add ("updateRate");
        add ("gapMarkers");
        add ("guiRefresh");
        add ("tableColor");
        add ("radioGroup");
        add ("macroNames");
        add ("ballColour");
		add ("keyPressed");
        add ("scrollBars");
        add ("cellHeight");
        add ("opcodeDir");
        add ("markerEnd");
        add ("menuColor");
        add ("cellWidth");
        add ("popupText");
        add ("textColor");
        add ("fontStyle");
        add ("fontColor");
        add ("filmstrip");
        add ("arraySize");
        add ("increment");
        add ("nameSpace");
        add ("rowPrefix");
        add ("colPrefix");
		add ("sidechain");
        add ("populate");		
        add ("keyWidth");
        add ("pluginId");
		add ("fontSize");
        add ("cvOutput");
        add ("imgDebug");
        add ("position");
        add ("colour:0");
        add ("colour:1");
        add ("typeface");
        add ("channels");
        add ("velocity");
        add ("ampRange");
        add ("cellData");
        add ("isparent");
        add ("latency");
        add ("cvInput");
        add ("color:0");
        add ("color:1");
        add ("caption");
        add ("textBox");
        add ("corners");
        add ("color:1");
        add ("middleC");
        add ("channel");
        add ("imgpath");
        add ("address");
        add ("imgFile");
        add ("visible");
        add ("latched");
        add ("reScale");
        add ("toFront");
        add ("guiMode");
        add ("bundle");
        add ("import");
        add ("bounds");
        add ("colour");
        add ("rangeY");
        add ("rangeX");
        add ("rotate");
        add ("active");
        add ("shape");
        add ("screw");
        add ("scale");
        add ("popup");
        add ("alpha");
        add ("items");
        add ("light");
        add ("color");
        add ("value");
        add ("range");
        add ("plant");
		add ("style");
        add ("align");
        add ("sort");
        add ("file");
        add ("wrap");
        add ("text");
        add ("crop");
        add ("name");
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
};

// this array holds all identifier strings
class CabbageIdentifierStringsNonCamelCase : public StringArray
{
public:
    CabbageIdentifierStringsNonCamelCase()  // longest strings appear first otherwise identifiers will be missed in parsing
    {
        add ("scrubberposition_sample");
        add ("doubleclicktogglesedit");
        add ("scrubberposition_table");
        add ("tablebackgroundcolour");
        add ("arrowbackgroundcolour");
        add ("amprange_tablenumber");
        add ("textboxoutlinecolour");
        add ("trackeroutsideradius");
        add ("trackerinsideradius");
        add ("surrogatelinenumber");
        add ("mouseoverkeycolour");
        add ("keypressbaseoctave");
        add ("keyseparatorcolour");
        add ("valuetextboxbounds");
        add ("amprange_quantise");
        add ("currentnotecolour");
        add ("backgroundcolour");
        add ("trackerthickness");
        add ("activecellcolour");
        add ("outlinethickness");
        add ("scrubberposition");
        add ("titlebargradient");
        add ("mouseinteraction");
        add ("whitenotecolour");
        add ("blacknotecolour");
        add ("highlightcolour");
        add ("tablegridcolour");
        add ("trackerbgcolour");
        add ("backgroundcolor");
        add ("showstepnumbers");
        add ("markerthickness");
        add ("titlebarcolour");
        add ("tablegridcolor");
        add ("signalvariable");
        add ("protecteditems");
        add ("overlaycolour");
        add ("keydowncolour");
        add ("linethickness");
        add ("outlinecolour");
        add ("numberofsteps");
        add ("textboxcolour");
        add ("trackercolour");
        add ("trackerradius");
        add ("sliderBounds");
        add ("markercolour");
        add ("valuetextbox");
        add ("fontcolour:1");
        add ("amprange_min");
        add ("amprange_max");
        add ("trackercolor");
        add ("refreshfiles");
        add ("tablenumbers");
        add ("identchannel");
        add ("fontcolour:0");
        add ("macrostrings");
        add ("tablecolour:");
        add ("metercolour:");
        add ("popuppostfix");
        add ("manufacturer");
        add ("valuepostfix");
        add ("showscrubber");
        add ("markerstart");
        add ("keyreleased");
        add ("orientation");
        add ("tablecolour");
        add ("metercolour");
        add ("widgetarray");
        add ("arrowcolour");
        add ("channeltype");
        add ("samplerange");
        add ("fontcolor:0");
        add ("tablenumber");
        add ("displaytype");
        add ("fontcolor:1");
        add ("basechannel");
        add ("popupprefix");
        add ("automatable");
        add ("valueprefix");
        add ("caretcolour");
        add ("automation");
        add ("autoupdate");
        add ("matrixsize");
        add ("fontcolour");
        add ("textcolour");
        add ("updaterate");
        add ("gapmarkers");
        add ("guirefresh");
        add ("tablecolor");
        add ("radiogroup");
        add ("macronames");
        add ("ballcolour");
        add ("keypressed");
        add ("scrollbars");
        add ("cellheight");
        add ("opcodedir");
        add ("markerend");
        add ("menucolor");
        add ("cellwidth");
        add ("popuptext");
        add ("textcolor");
        add ("fontstyle");
        add ("fontcolor");
        add ("filmstrip");
        add ("arraysize");
        add ("increment");
        add ("namespace");
        add ("rowprefix");
        add ("colprefix");
        add ("sidechain");
        add ("populate");
        add ("keywidth");
        add ("pluginid");
        add ("fontsize");
        add ("cvoutput");
        add ("imgdebug");
        add ("position");
        add ("colour:0");
        add ("colour:1");
        add ("typeface");
        add ("channels");
        add ("velocity");
        add ("amprange");
        add ("celldata");
        add ("isparent");
        add ("latency");
        add ("cvinput");
        add ("color:0");
        add ("color:1");
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
        add ("tofront");
        add ("guimode");
        add ("bundle");
        add ("import");
        add ("bounds");
        add ("colour");
        add ("rangey");
        add ("rangex");
        add ("rotate");
        add ("active");
        add ("shape");
        add ("screw");
        add ("scale");
        add ("popup");
        add ("alpha");
        add ("items");
        add ("light");
        add ("color");
        add ("value");
        add ("range");
        add ("plant");
        add ("style");
        add ("align");
        add ("sort");
        add ("file");
        add ("wrap");
        add ("text");
        add ("crop");
        add ("name");
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
};
namespace CabbageWidgetTypes
{
    //list of static consts for types of widgets
	static const String button = "button";
    static const String optionbutton = "optionbutton";
	static const String checkbox = "checkbox";
	static const String combobox = "combobox";
	static const String csoundoutput = "csoundoutput";
	static const String directorylist = "directorylist";
	static const String encoder = "encoder";
	static const String eventsequencer = "eventsequencer";
	static const String fftdisplay = "fftdisplay";
	static const String filebutton = "filebutton";
	static const String form = "form";
	static const String gentable = "gentable";
	static const String groupbox = "groupbox";
	static const String hmeter = "hmeter";
	static const String hrange = "hrange";
	static const String hslider = "hslider";
	static const String hslider2 = "hslider2";
	static const String hslider3 = "hslider3";
	static const String image = "image";
    static const String screw = "screw";
    static const String cvoutput = "cvoutput";
    static const String cvinput = "cvinput";
    static const String light = "light";

    static const String infobutton = "infobutton";
    static const String json = "json";
	static const String keyboard = "keyboard";
	static const String keyboarddisplay = "keyboarddisplay";
	static const String label = "label";
	static const String line = "line";
	static const String listbox = "listbox";
	static const String loadbutton = "loadbutton";
	static const String nslider = "nslider";
	static const String oscserver = "oscserver";
	static const String popupmenu = "popupmenu";
	static const String rslider = "rslider";
	static const String signaldisplay = "signaldisplay";
	static const String soundfiler = "soundfiler";
	static const String sourcebutton = "sourcebutton";
	static const String table = "table";
	static const String textbox = "textbox";
	static const String texteditor = "texteditor";
	static const String vmeter = "vmeter";
	static const String vrange = "vrange";
	static const String vslider = "vslider";
	static const String vslider2 = "vslider2";
	static const String vslider3 = "vslider3";
	static const String xypad = "xypad";
};

namespace CabbageIdentifierIds
{
    // list of static consts for each identifiers
    static const Identifier active = "active";
    static const Identifier activecellcolour = "activeCellColour";
    static const Identifier align = "align";
    static const Identifier autoupdate = "autoUpdate";
    static const Identifier allowboundsupdate = "allowBoundsUpdate";
    static const Identifier alpha = "alpha";
    static const Identifier amprange = "ampRange";
    static const Identifier amprange_max = "amprange_max";
    static const Identifier amprange_min = "amprange_min";
    static const Identifier amprange_quantise = "amprange_quantise";
    static const Identifier amprange_tablenumber = "amprange_tablenumber";
    static const Identifier arraysize = "arraySize";
    static const Identifier arrowbackgroundcolour = "arrowBackgroundColour";
    static const Identifier arrowcolour = "arrowColour";
    static const Identifier automation = "automation";
    static const Identifier automatable = "automatable";
    static const Identifier author = "author";
    static const Identifier backgroundcolour = "backgroundColour";
    static const Identifier ballcolour = "ballColour";
    static const Identifier basechannel = "baseChannel";
    static const Identifier basetype = "baseType";
    static const Identifier blacknotecolour = "blackNoteColour";
    static const Identifier keydowncolour = "keydownColour";
    static const Identifier bold = "bold";
    static const Identifier bounds = "bounds";
    static const Identifier bpm = "bpm";
    static const Identifier bundle = "bundle";
    static const Identifier caption = "caption";
    static const Identifier caretcolour = "caretColour";
    static const Identifier celldata = "celldata";
    static const Identifier channel = "channel";
    static const Identifier channeltype = "channelType";
    static const Identifier child = "child";
    static const Identifier colour = "colour";
    static const Identifier colouroff = "colour:0";
    static const Identifier colouron = "colour:1";
    static const Identifier colprefix = "colPrefix";
    static const Identifier comborange = "comborange";
    static const Identifier comboindex = "comboindex";
    static const Identifier corners = "corners";
    static const Identifier crop = "crop";
    static const Identifier cropheight = "cropHeight";
    static const Identifier cropwidth = "cropWidth";
    static const Identifier cropx = "cropX";
    static const Identifier cropy = "cropY";
    static const Identifier currenttext = "currentText";
    static const Identifier currentWidgetChannel = "CURRENT_WIDGET";
    static const Identifier currentPresetName = "currentpresetname";
    static const Identifier decimalplaces = "decimalplaces";
    static const Identifier displaytype = "displayType";
    static const Identifier doubleclicktogglesedit = "doubleClickTogglesEdit";
    static const Identifier drawmode = "drawMode";
    static const Identifier endpoint = "endPoint";
    static const Identifier endpos = "endPos";
    static const Identifier gapmarkers = "gapMarkers";
    static const Identifier macronames = "macronames";
    static const Identifier manufacturer = "manufacturer";
    static const Identifier ffttablenumber = "ffttablenumber";
    static const Identifier file = "file";
    static const Identifier filetype = "fileType";
    static const Identifier fill = "fill";
    static const Identifier filmstrip = "filmstrip";
    static const Identifier filmStripRemoveFrom1 = "filmStripRemoveFrom1";
    static const Identifier filmStripRemoveFrom2 = "filmStripRemoveFrom2";
    static const Identifier filmstripimage = "filmStripImage";
    static const Identifier filmstripframes = "filmStripFrames";
    static const Identifier fillcolour = "fillColour";
    static const Identifier fontcolour = "fontColour";
    static const Identifier fontstyle = "fontStyle";
    static const Identifier fontsize = "fontSize";
    static const Identifier gradient = "gradient";
    static const Identifier guirefresh = "guiRefresh";
    static const Identifier guimode = "guiMode";
    static const Identifier polling = "polling";
    static const Identifier queue = "queue";
    static const Identifier height = "height";
    static const Identifier highlightcolour = "highlightColour";
    static const Identifier identchannel = "identChannel";
    static const Identifier identchannelarray = "identChannelArray";
    static const Identifier identchannelmessage = "identChannelMessage";
    static const Identifier imgbuttonoff = "imgbuttonoff";
    static const Identifier imgbuttonon = "imgbuttonon";
    static const Identifier imgbuttonover = "imgbuttonover";
    static const Identifier imgdebug = "imgdebug";
    static const Identifier imgfile = "imgFile";
    static const Identifier imggroupbox = "imggroupbox";
    static const Identifier imgpath = "imgpath";
    static const Identifier imgslider = "imgslider";
    static const Identifier imgsliderbg = "imgsliderbg";
    static const Identifier import = "import";
    static const Identifier importfiles = "importfiles";
    static const Identifier increment = "increment";
    static const Identifier isparent = "isparent";
    static const Identifier items = "items";
    static const Identifier keypressbaseoctave = "keyPressBaseOctave";
    static const Identifier keyseparatorcolour = "keySeparatorColour";
    static const Identifier keywidth = "keyWidth";
    static const Identifier kind = "kind";
    static const Identifier latched = "latched";
    static const Identifier left = "left";
    static const Identifier linenumber = "lineNumber";
    static const Identifier latency = "latency";
    static const Identifier linethickness = "lineThickness";
    static const Identifier logger = "logger";
    static const Identifier macrostrings = "macrostrings";
    static const Identifier markercolour = "markerColour";
    static const Identifier markerend = "markerEnd";
    static const Identifier markerstart = "markerStart";
    static const Identifier markerthickness = "markerThickness";
    static const Identifier matrixcols = "matrixCols";
    static const Identifier matrixrows = "matrixRows";
    static const Identifier max = "max";
    static const Identifier maxenabled = "maxenabled";
    static const Identifier maxvalue = "maxValue";
    static const Identifier maxx = "maxX";
    static const Identifier maxy = "maxY";
    static const Identifier menucolour = "menuColour";
    static const Identifier metercolour = "meterColour";
    static const Identifier middlec = "middleC";
    static const Identifier midichan = "midichan";
    static const Identifier midictrl = "midictrl";
    static const Identifier min = "min";
    static const Identifier minenabled = "minenabled";
    static const Identifier minvalue = "minValue";
    static const Identifier minx = "minxX";
    static const Identifier miny = "minY";
    static const Identifier mode = "mode";
    static const Identifier mouseinteraction = "mouseInteraction";
    static const Identifier mouseoverkeycolour = "mouseOverKeyColour";
    static const Identifier name = "name";
    static const Identifier keypressed = "keyPressed";
    static const Identifier keyreleased = "keyReleased";
    static const Identifier oncolour = "onColour";
    static const Identifier onfontcolour = "onFontColour";
    static const Identifier opcodedir = "opcodeDir";
    static const Identifier orientation = "orientation";
    static const Identifier outlinecolour = "outlineColour";
    static const Identifier outlinethickness = "outlineThickness";
    static const Identifier overlaycolour = "overlayColour";
    static const Identifier parentcomponent = "parentcomponent";
    static const Identifier parentdir = "parentDir";
    static const Identifier pivotx = "pivotX";
    static const Identifier pivoty = "pivotY";
    static const Identifier plant = "plant";
    static const Identifier pluginid = "pluginId";
    static const Identifier populate = "populate";
    static const Identifier popup = "popup";
    static const Identifier pos = "pos";
    static const Identifier position = "position";
    static const Identifier popuppostfix = "popupPostfix";
    static const Identifier popupprefix = "popupPrefix";
    static const Identifier popuptext = "popupText";
    static const Identifier protecteditems = "protectedItems";
    static const Identifier radiogroup = "radioGroup";
    static const Identifier range = "range";
    static const Identifier rangex = "rangeX";
    static const Identifier rangey = "rangeY";
    static const Identifier readonly = "readOnly";
    static const Identifier refreshfiles = "refreshFiles";
    static const Identifier scale = "scale";
    static const Identifier resize = "resize";
    static const Identifier resizemode = "resizeMode";
    static const Identifier rotate = "rotate";
    static const Identifier rowprefix = "rowPrefix";
    static const Identifier samplerange = "sampleRange";
    static const Identifier scalex = "scaleX";
    static const Identifier scaley = "scaleY";
    static const Identifier scrollbars = "scrollBars";
    static const Identifier scrubberposition = "scrubberPosition";
    static const Identifier showscrubber = "showScrubber";
    static const Identifier scrubberposition_sample = "scrubberposition_sample";
    static const Identifier scrubberposition_table = "scrubberposition_table";
    static const Identifier shape = "shape";
    static const Identifier show = "show";
    static const Identifier sort = "sort";
    static const Identifier sidechain = "sidechain";
    static const Identifier signalvariable = "signalVariable";
    static const Identifier size = "size";
    static const Identifier sliderbounds = "sliderBounds";
    static const Identifier sliderrange = "sliderRange";
    static const Identifier sliderskew = "sliderSkew";
    static const Identifier socketaddress = "socketAddress";
    static const Identifier socketport = "socketPort";
    static const Identifier stack = "stack";
    static const Identifier startpoint = "startPoint";
    static const Identifier startpos = "startPos";
    static const Identifier style = "style";
    static const Identifier surrogatelinenumber = "surrogatelinenumber";
    static const Identifier tabbed = "tabbed";
    static const Identifier tablebackgroundcolour = "tableBackgroundColour";
    static const Identifier tablecolour = "tableColour";
    static const Identifier tableconfig = "tableConfig";
    static const Identifier tablegridcolour = "tableGridColour";
    static const Identifier tablenumber = "tableNumber";
    static const Identifier tabpage = "tabpage";
    static const Identifier tabs = "tabs";
    static const Identifier text = "text";
    static const Identifier textboxcolour = "textBoxColour";
    static const Identifier textboxoutlinecolour = "textBoxOutlineColour";
    static const Identifier textcolour = "textColour";
    static const Identifier titlebarcolour = "titleBarColour";
    static const Identifier titlebargradient = "titleBarGradient";
    static const Identifier tofront = "toFront";
    static const Identifier top = "top";
    static const Identifier trackercolour = "trackerColour";
    static const Identifier trackerbgcolour = "trackerBgColour";
    static const Identifier trackerthickness = "trackerThickness";
    static const Identifier trackeroutsideradius = "trackerOutsideRadius";
    static const Identifier trackerinsideradius = "trackerInsideRadius";
    static const Identifier type = "type";
    static const Identifier typeface = "typeface";
    static const Identifier update = "update";
    static const Identifier value = "value";
    static const Identifier valuetextbox = "valueTextBox";
    static const Identifier valuetextboxbounds = "valueTextBoxBounds";
    static const Identifier valuex = "valueX";
    static const Identifier valuey = "valueY";
    static const Identifier valueprefix = "valuePrefix";
    static const Identifier valuepostfix = "valuePostfix";
    static const Identifier velocity = "velocity";
    static const Identifier visible = "visible";
    static const Identifier visiblelength = "visibleLength";
    static const Identifier whitenotecolour = "whiteNoteColour";
    static const Identifier widgetarray = "widgetArray";
    static const Identifier width = "width";
    static const Identifier workingdir = "workingDir";
    static const Identifier wrap = "wrap";
    static const Identifier xchannel = "xChannel";
    static const Identifier xyautoindex = "xyautoindex";
    static const Identifier xychannel = "xyChannel";
    static const Identifier ychannel = "yChannel";
    static const Identifier zoom = "zoom";
    static const String cellheight = "cellHeight";
    static const String cellwidth = "cellWidth";
    static const String csdfile = "csdfile";
    static const String hostbpm = "HOST_BPM";
    static const String hostppqpos = "HOST_PPQ_POS";
    static const String lastFileDropped = "LAST_FILE_DROPPED";
    static const String lastTextDropped = "LAST_TEXT_DROPPED";
    static const String index = "index";
    static const String isplaying = "IS_PLAYING";
    static const String isrecording = "IS_RECORDING";
    static const String mousedownleft = "MOUSE_DOWN_LEFT";
    static const String mousedownlmiddle = "MOUSE_DOWN_MIDDLE";
    static const String mousedownright = "MOUSE_DOWN_RIGHT";
    static const String mousex = "MOUSE_X";
    static const String mousey = "MOUSE_Y";
    static const String nsp = "namespace";
    static const String numberofsteps = "numberOfSteps";
    static const String showstepnumbers = "showStepNumbers";
    static const String stringchannel = "string";
    static const String timeinsamples = "TIME_IN_SAMPLES";
    static const String timeinseconds = "TIME_IN_SECONDS";
    static const String timeSigDenom = "TIME_SIG_DENOM";
    static const String timeSigNum = "TIME_SIG_NUM";
    static const String updaterate = "updaterate";
    
};
namespace CabbageIdentifierIds2
{
    // list of static consts for each identifiers
	static const Identifier active = "active";
	static const Identifier activecellcolour = "activecellcolour";
	static const Identifier align = "align";
    static const Identifier autoupdate = "autoupdate";
	static const Identifier allowboundsupdate = "allowboundsupdate";
	static const Identifier alpha = "alpha";
	static const Identifier amprange = "amprange";
	static const Identifier amprange_max = "amprange_max";
	static const Identifier amprange_min = "amprange_min";
	static const Identifier amprange_quantise = "amprange_quantise";
	static const Identifier amprange_tablenumber = "amprange_tablenumber";
	static const Identifier arraysize = "arraysize";
	static const Identifier arrowbackgroundcolour = "arrowbackgroundcolour";
	static const Identifier arrowcolour = "arrowcolour";
    static const Identifier automation = "automation";
    static const Identifier automatable = "automatable";
	static const Identifier author = "author";
	static const Identifier backgroundcolour = "backgroundcolour";
	static const Identifier ballcolour = "ballcolour";
	static const Identifier basechannel = "basechannel";
	static const Identifier basetype = "basetype";
    static const Identifier blacknotecolour = "blacknotecolour";
    static const Identifier keydowncolour = "keydowncolour";
	static const Identifier bold = "bold";
    static const Identifier bounds = "bounds";
	static const Identifier bpm = "bpm";
	static const Identifier bundle = "bundle";
	static const Identifier caption = "caption";
	static const Identifier celldata = "celldata";
	static const Identifier channel = "channel";
	static const Identifier channeltype = "channeltype";
	static const Identifier child = "child";
	static const Identifier colour = "colour";
    static const Identifier colouroff = "colour:0";
    static const Identifier colouron = "colour:1";
	static const Identifier colprefix = "colprefix";
	static const Identifier comborange = "comborange";
	static const Identifier comboindex = "comboindex";
	static const Identifier corners = "corners";
	static const Identifier crop = "crop";
	static const Identifier cropheight = "cropheight";
	static const Identifier cropwidth = "cropwidth";
	static const Identifier cropx = "cropx";
	static const Identifier cropy = "cropy";
	static const Identifier currenttext = "currenttext";
    static const Identifier currentWidgetChannel = "CURRENT_WIDGET";
    static const Identifier currentPresetName = "currentpresetname";
	static const Identifier decimalplaces = "decimalplaces";
	static const Identifier displaytype = "displaytype";
	static const Identifier drawmode = "drawmode";
	static const Identifier endpoint = "endpoint";
	static const Identifier endpos = "endpos";
    static const Identifier gapmarkers = "gapmarkers";
	static const Identifier macronames = "macronames";
    static const Identifier manufacturer = "manufacturer";
	static const Identifier ffttablenumber = "ffttablenumber";
	static const Identifier file = "file";
	static const Identifier filetype = "filetype";
	static const Identifier fill = "fill";
    static const Identifier filmstrip = "filmstrip";
    static const Identifier filmStripRemoveFrom1 = "filmStripRemoveFrom1";
    static const Identifier filmStripRemoveFrom2 = "filmStripRemoveFrom2";
    static const Identifier filmstripimage = "filmstripimage";
    static const Identifier filmstripframes = "filmstripframes";
	static const Identifier fillcolour = "fillcolour";
	static const Identifier fontcolour = "fontcolour";
	static const Identifier fontstyle = "fontstyle";
	static const Identifier fontsize = "fontsize";
	static const Identifier gradient = "gradient";
	static const Identifier guirefresh = "guirefresh";
    static const Identifier guimode = "guimode";
    static const Identifier polling = "polling";
    static const Identifier queue = "queue";
	static const Identifier height = "height";
	static const Identifier highlightcolour = "highlightcolour";
	static const Identifier identchannel = "identchannel";
	static const Identifier identchannelarray = "identchannelarray";
	static const Identifier identchannelmessage = "identchannelmessage";
	static const Identifier imgbuttonoff = "imgbuttonoff";
	static const Identifier imgbuttonon = "imgbuttonon";
    static const Identifier imgbuttonover = "imgbuttonover";
	static const Identifier imgdebug = "imgdebug";
	static const Identifier imgfile = "imgfile";
	static const Identifier imggroupbox = "imggroupbox";
	static const Identifier imgpath = "imgpath";
	static const Identifier imgslider = "imgslider";
	static const Identifier imgsliderbg = "imgsliderbg";
	static const Identifier import = "import";
	static const Identifier importfiles = "importfiles";
	static const Identifier increment = "increment";
    static const Identifier isparent = "isparent";
	static const Identifier items = "items";
	static const Identifier keypressbaseoctave = "keypressbaseoctave";
	static const Identifier keyseparatorcolour = "keyseparatorcolour";
	static const Identifier keywidth = "keywidth";
	static const Identifier kind = "kind";
	static const Identifier latched = "latched";
	static const Identifier left = "left";
	static const Identifier linenumber = "linenumber";
    static const Identifier latency = "latency";
	static const Identifier linethickness = "linethickness";
	static const Identifier logger = "logger";
	static const Identifier macrostrings = "macrostrings";
    static const Identifier markercolour = "markercolour";
    static const Identifier markerend = "markerend";
    static const Identifier markerstart = "markerstart";
    static const Identifier markerthickness = "markerthickness";
	static const Identifier matrixcols = "matrixcols";
	static const Identifier matrixrows = "matrixrows";
	static const Identifier max = "max";
	static const Identifier maxenabled = "maxenabled";
	static const Identifier maxvalue = "maxvalue";
	static const Identifier maxx = "maxx";
	static const Identifier maxy = "maxy";
	static const Identifier menucolour = "menucolour";
	static const Identifier metercolour = "metercolour";
	static const Identifier middlec = "middlec";
	static const Identifier midichan = "midichan";
	static const Identifier midictrl = "midictrl";
	static const Identifier min = "min";
	static const Identifier minenabled = "minenabled";
	static const Identifier minvalue = "minvalue";
	static const Identifier minx = "minx";
	static const Identifier miny = "miny";
	static const Identifier mode = "mode";
    static const Identifier mouseinteraction = "mouseinteraction";
	static const Identifier mouseoverkeycolour = "mouseoverkeycolour";
	static const Identifier name = "name";
	static const Identifier keypressed = "keypressed";
	static const Identifier keyreleased = "keyreleased";
	static const Identifier oncolour = "oncolour";
	static const Identifier onfontcolour = "onfontcolour";
    static const Identifier opcodedir = "opcodedir";
	static const Identifier orientation = "orientation";
	static const Identifier outlinecolour = "outlinecolour";
	static const Identifier outlinethickness = "outlinethickness";
	static const Identifier overlaycolour = "overlaycolour";
	static const Identifier parentcomponent = "parentcomponent";
	static const Identifier parentdir = "parentdir";
	static const Identifier pivotx = "pivotx";
	static const Identifier pivoty = "pivoty";
	static const Identifier plant = "plant";
	static const Identifier pluginid = "pluginid";
	static const Identifier populate = "populate";
	static const Identifier popup = "popup";
    static const Identifier pos = "pos";
    static const Identifier position = "position";
	static const Identifier popuppostfix = "popuppostfix";
	static const Identifier popupprefix = "popupprefix";
	static const Identifier popuptext = "popuptext";
	static const Identifier radiogroup = "radiogroup";
	static const Identifier range = "range";
	static const Identifier rangex = "rangex";
	static const Identifier rangey = "rangey";
	static const Identifier readonly = "readonly";
	static const Identifier refreshfiles = "refreshfiles";
	static const Identifier scale = "scale";
	static const Identifier resize = "resize";
	static const Identifier resizemode = "resizemode";
	static const Identifier rotate = "rotate";
	static const Identifier rowprefix = "rowprefix";
	static const Identifier samplerange = "samplerange";
	static const Identifier scalex = "scalex";
	static const Identifier scaley = "scaley";
	static const Identifier scrollbars = "scrollbars";
	static const Identifier scrubberposition = "scrubberposition";
    static const Identifier showscrubber = "showscrubber";
	static const Identifier scrubberposition_sample = "scrubberposition_sample";
	static const Identifier scrubberposition_table = "scrubberposition_table";
	static const Identifier shape = "shape";
	static const Identifier show = "show";
    static const Identifier sort = "sort";
	static const Identifier sidechain = "sidechain";
	static const Identifier signalvariable = "signalvariable";
    static const Identifier size = "size";
	static const Identifier sliderrange = "sliderrange";
	static const Identifier sliderskew = "sliderskew";
	static const Identifier socketaddress = "socketaddress";
	static const Identifier socketport = "socketport";
	static const Identifier stack = "stack";
	static const Identifier startpoint = "startpoint";
	static const Identifier startpos = "startpos";
	static const Identifier style = "style";
	static const Identifier surrogatelinenumber = "surrogatelinenumber";
	static const Identifier tabbed = "tabbed";
	static const Identifier tablebackgroundcolour = "tablebackgroundcolour";
	static const Identifier tablecolour = "tablecolour";
	static const Identifier tableconfig = "tableconfig";
	static const Identifier tablegridcolour = "tablegridcolour";
	static const Identifier tablenumber = "tablenumber";
	static const Identifier tabpage = "tabpage";
	static const Identifier tabs = "tabs";
	static const Identifier text = "text";
	static const Identifier textboxcolour = "textboxcolour";
	static const Identifier textboxoutlinecolour = "textboxoutlinecolour";
	static const Identifier textcolour = "textcolour";
	static const Identifier titlebarcolour = "titlebarcolour";
	static const Identifier titlebargradient = "titlebargradient";
    static const Identifier tofront = "tofront";
    static const Identifier top = "top";
	static const Identifier trackercolour = "trackercolour";
    static const Identifier trackerbgcolour = "trackerbgcolour";
	static const Identifier trackerthickness = "trackerthickness";
    static const Identifier trackeroutsideradius = "trackeroutsideradius";
    static const Identifier trackerinsideradius = "trackerinsideradius";
	static const Identifier type = "type";
    static const Identifier typeface = "typeface";
    static const Identifier update = "update";
	static const Identifier value = "value";
	static const Identifier valuetextbox = "valuetextbox";
    static const Identifier valuetextboxbounds = "valuetextboxbounds";
	static const Identifier valuex = "valuex";
	static const Identifier valuey = "valuey";
    static const Identifier valueprefix = "valueprefix";
    static const Identifier valuepostfix = "valuepostfix";
	static const Identifier velocity = "velocity";
	static const Identifier visible = "visible";
	static const Identifier visiblelength = "visiblelength";
	static const Identifier whitenotecolour = "whitenotecolour";
	static const Identifier widgetarray = "widgetarray";
	static const Identifier width = "width";
	static const Identifier workingdir = "workingdir";
	static const Identifier wrap = "wrap";
	static const Identifier xchannel = "xchannel";
	static const Identifier xyautoindex = "xyautoindex";
	static const Identifier xychannel = "xychannel";
	static const Identifier ychannel = "ychannel";
	static const Identifier zoom = "zoom";
	static const String cellheight = "cellheight";
	static const String cellwidth = "cellwidth";
	static const String csdfile = "csdfile";
	static const String hostbpm = "HOST_BPM";
	static const String hostppqpos = "HOST_PPQ_POS";
    static const String lastFileDropped = "LAST_FILE_DROPPED";
    static const String lastTextDropped = "LAST_TEXT_DROPPED";
    static const String index = "index";
	static const String isplaying = "IS_PLAYING";
	static const String isrecording = "IS_RECORDING";
	static const String mousedownleft = "MOUSE_DOWN_LEFT";
	static const String mousedownlmiddle = "MOUSE_DOWN_MIDDLE";
	static const String mousedownright = "MOUSE_DOWN_RIGHT";
	static const String mousex = "MOUSE_X";
	static const String mousey = "MOUSE_Y";
	static const String nsp = "namespace";
	static const String numberofsteps = "numberofsteps";
	static const String showstepnumbers = "showstepnumbers";
	static const String stringchannel = "string";
	static const String timeinsamples = "TIME_IN_SAMPLES";
	static const String timeinseconds = "TIME_IN_SECONDS";
	static const String timeSigDenom = "TIME_SIG_DENOM";
	static const String timeSigNum = "TIME_SIG_NUM";
	static const String updaterate = "updaterate";

};


// this array holds the names of all widgets that can inserted when in GUI edit mode
class CabbagePopupWidgets : public StringPairArray
{
public:
    CabbagePopupWidgets()
    {
        set ("Button", CabbageWidgetTypes::button);                 //1
        set ("Button - File", CabbageWidgetTypes::filebutton);      //2
        set ("Button - Info", CabbageWidgetTypes::infobutton);      //3
        set ("Checkbox", CabbageWidgetTypes::checkbox);             //4
        set ("ComboBox", CabbageWidgetTypes::combobox);             //5
        set ("Csound Output", CabbageWidgetTypes::csoundoutput);    //6
        set ("CV Output - VCVRack", CabbageWidgetTypes::cvoutput);  //7
        set ("CV Input - VCVRack", CabbageWidgetTypes::cvoutput);   //8
        set ("Endless Encoder", CabbageWidgetTypes::encoder);       //9
        set ("Gentable", CabbageWidgetTypes::gentable);             //10
        set ("Groupbox", CabbageWidgetTypes::groupbox);             //11
        set ("Horizontal Meter", CabbageWidgetTypes::hmeter);       //12
        set ("Image", CabbageWidgetTypes::image);                   //13
        set ("Keyboard", CabbageWidgetTypes::keyboard);             //14
        set ("Label", CabbageWidgetTypes::label);                   //15
        set ("light", CabbageWidgetTypes::light);                   //16

        set ("Signal Display", CabbageWidgetTypes::signaldisplay);  //17
        set ("Slider - Horizontal", CabbageWidgetTypes::hslider);   //18
        set ("Slider - Rotary", CabbageWidgetTypes::rslider);       //19
        set ("Slider - Vertical", CabbageWidgetTypes::vslider);     //20
        set ("Slider - Range - Vertical", CabbageWidgetTypes::vrange);      //21
        set ("Slider - Range - Horizontal", CabbageWidgetTypes::hrange);    //22
        set ("Slider - Number Box", CabbageWidgetTypes::nslider);       //23
        set ("Soundfiler", CabbageWidgetTypes::soundfiler);             //24
        set ("String Sequencer", CabbageWidgetTypes::eventsequencer);   //25
        set ("Text Box", CabbageWidgetTypes::textbox);                  //26
        set ("Text Editor", CabbageWidgetTypes::texteditor);            //27
        set ("Vertical Meter", CabbageWidgetTypes::vmeter);             //28
        set ("XY Pad", CabbageWidgetTypes::xypad);                      //29
        set ("Listbox", CabbageWidgetTypes::listbox);                   //30
        set ("Button - Option", CabbageWidgetTypes::optionbutton);                   //30

    }
};

class CabbageIdentifierPropertyStringPairs : public StringPairArray
{
public:
    CabbageIdentifierPropertyStringPairs()
    {
        set ("Font Size", CabbageIdentifierIds::fontsize.toString());
        set ("Active", CabbageIdentifierIds::active.toString());
        set ("Align", CabbageIdentifierIds::align.toString());
        set ("Alpha", CabbageIdentifierIds::alpha.toString());
        set ("Array Size", CabbageIdentifierIds::arraysize.toString());
        set ("Arrows Background", CabbageIdentifierIds::arrowbackgroundcolour.toString());
        set ("Arrows", CabbageIdentifierIds::arrowcolour.toString());
        set ("Background Image", CabbageIdentifierIds::imgsliderbg.toString());
        set ("Background", CabbageIdentifierIds::backgroundcolour.toString());
        set ("Ball", CabbageIdentifierIds::ballcolour.toString());
        set ("Base Channel", CabbageIdentifierIds::basechannel.toString());
        set ("Black Notes", CabbageIdentifierIds::blacknotecolour.toString());
        set ("Cell Height", CabbageIdentifierIds::cellheight);
        set ("Cell Width", CabbageIdentifierIds::cellwidth);
        set ("Channel Type", CabbageIdentifierIds::channeltype.toString());
        set ("Channel", CabbageIdentifierIds::channel.toString());
        set ("Colour", CabbageIdentifierIds::colour.toString());
        set ("Colour: Off", CabbageIdentifierIds::colour.toString());
        set ("Colour: On", CabbageIdentifierIds::oncolour.toString());
        set ("Corners", CabbageIdentifierIds::corners.toString());
        set ("End Index", CabbageIdentifierIds::endpos.toString());
        set ("File", CabbageIdentifierIds::file.toString());
        set ("Fill", CabbageIdentifierIds::fill.toString());
        set ("Font", CabbageIdentifierIds::fontcolour.toString());
        set ("Filmstrip Image", CabbageIdentifierIds::filmstrip.toString());
        set ("Filmstrip Frames", CabbageIdentifierIds::filmstripframes.toString());
        set ("Font: Off", CabbageIdentifierIds::fontcolour.toString());
        set ("Font: On", CabbageIdentifierIds::onfontcolour.toString());
        set ("Groupbox Image", CabbageIdentifierIds::imggroupbox.toString());
        set ("Height", CabbageIdentifierIds::height.toString());
        set ("Ident Channel", CabbageIdentifierIds::identchannel.toString());
        set ("Image File", CabbageIdentifierIds::imgslider.toString());
        set ("Increment", CabbageIdentifierIds::increment.toString());
        set ("Inner Radius", CabbageIdentifierIds::trackerinsideradius.toString());
        set ("Key Separator", CabbageIdentifierIds::keyseparatorcolour.toString());
        set ("Line Thickness", CabbageIdentifierIds::linethickness.toString());
        set ("Marker", CabbageIdentifierIds::markercolour.toString());
        set ("Max Amp", CabbageIdentifierIds::amprange_max.toString());
        set ("Max: X", CabbageIdentifierIds::maxx.toString());
        set ("Max: Y", CabbageIdentifierIds::maxy.toString());
        set ("Maximum", CabbageIdentifierIds::max.toString());
        set ("Menu Colour", CabbageIdentifierIds::menucolour.toString());
        set ("Meter", CabbageIdentifierIds::metercolour.toString());
        set ("Min Amp", CabbageIdentifierIds::amprange_min.toString());
        set ("Min: X", CabbageIdentifierIds::minx.toString());
        set ("Min: Y", CabbageIdentifierIds::miny.toString());
        set ("Minimum", CabbageIdentifierIds::min.toString());
        set ("Mode", CabbageIdentifierIds::mode.toString());
        set ("Mouse Over", CabbageIdentifierIds::mouseoverkeycolour.toString());
        set ("Off Image", CabbageIdentifierIds::imgbuttonoff.toString());
        set ("On Image", CabbageIdentifierIds::imgbuttonon.toString());
        set ("Orientation", CabbageIdentifierIds::orientation.toString());
        set ("Outer Radius", CabbageIdentifierIds::trackeroutsideradius.toString());
        set ("Outline Thickness", CabbageIdentifierIds::outlinethickness.toString());
        set ("Outline", CabbageIdentifierIds::outlinecolour.toString());
        set ("Over Image", CabbageIdentifierIds::imgbuttonover.toString());
        set ("Overlay Colour", CabbageIdentifierIds::overlaycolour.toString());
        set ("Pivot X", CabbageIdentifierIds::pivotx.toString());
        set ("Pivot Y", CabbageIdentifierIds::pivoty.toString());
        set ("Populate", CabbageIdentifierIds::populate.toString());
        set ("Popup Text", CabbageIdentifierIds::popuptext.toString());
        set ("Quantise", CabbageIdentifierIds::amprange_quantise.toString());
        set ("Radio Group", CabbageIdentifierIds::radiogroup.toString());
        set ("Rotary Image", CabbageIdentifierIds::imgslider.toString());
        set ("Rotate", CabbageIdentifierIds::rotate.toString());
        set ("Scrubber Pos", CabbageIdentifierIds::scrubberposition_sample.toString());
        set ("Scrubber Table", CabbageIdentifierIds::scrubberposition_table.toString());
        set ("Selected Row", CabbageIdentifierIds::highlightcolour.toString());
        set ("Shape", CabbageIdentifierIds::shape.toString());
        set ("Skew", CabbageIdentifierIds::sliderskew.toString());
        set ("Soundfiler Background", CabbageIdentifierIds::tablebackgroundcolour.toString());
        set ("Start Index", CabbageIdentifierIds::startpos.toString());
        set ("Table Background", CabbageIdentifierIds::tablebackgroundcolour.toString());
        set ("Table Grid", CabbageIdentifierIds::tablegridcolour.toString());
        set ("Table No.", CabbageIdentifierIds::amprange_tablenumber.toString());
        set ("Table Numbers", CabbageIdentifierIds::tablenumber.toString());
        set ("Tables", CabbageIdentifierIds::tablecolour.toString());
        set ("Text Align", CabbageIdentifierIds::align.toString());
        set ("Text Colour", CabbageIdentifierIds::textcolour.toString());
        set ("Text", CabbageIdentifierIds::text.toString());
        set ("Tracker Thickness", CabbageIdentifierIds::trackerthickness.toString());
        set ("Tracker", CabbageIdentifierIds::trackercolour.toString());
        set ("Value Box Colour", CabbageIdentifierIds::textboxcolour.toString());
        set ("Value Box Outline", CabbageIdentifierIds::textboxoutlinecolour.toString());
        set ("Value Box", CabbageIdentifierIds::valuetextbox.toString());
        set ("Value Max", CabbageIdentifierIds::maxvalue.toString());
        set ("Value Min", CabbageIdentifierIds::minvalue.toString());
        set ("Value X", CabbageIdentifierIds::valuex.toString());
        set ("Value Y", CabbageIdentifierIds::valuey.toString());
        set ("Value", CabbageIdentifierIds::value.toString());
        set ("Velocity", CabbageIdentifierIds::velocity.toString());
        set ("Visible", CabbageIdentifierIds::visible.toString());
        set ("White Notes", CabbageIdentifierIds::whitenotecolour.toString());
        set ("Width", CabbageIdentifierIds::width.toString());
        set ("Y Position", CabbageIdentifierIds::top.toString());
        set ("Zoom", CabbageIdentifierIds::zoom.toString());
        set ("Zoom", CabbageIdentifierIds::zoom.toString());
        set ("X Position", CabbageIdentifierIds::left.toString());
    }
};

static const String opcodeHints =
"\"0dbfs\";\"Orchestra Syntax:Header\";\"Sets the value of 0 decibels using full scale amplitude.\";\"0dbfs = iarg\"\n"
"\"kr\";\"Orchestra Syntax:Header\";\"Sets the control rate.\";\"kr = iarg\"\n"
"\"ksmps\";\"Orchestra Syntax:Header\";\"Sets the number of samples in a control period.\";\"ksmps = iarg\"\n"
"\"nchnls\";\"Orchestra Syntax:Header\";\"Sets the number of channels of audio output.\";\"nchnls = iarg\"\n"
"\"nchnls_i\";\"Orchestra Syntax:Header\";\"Sets the number of channels of audio input.\";\"nchnls_i = iarg\"\n"
"\"sr\";\"Orchestra Syntax:Header\";\"Sets the audio sampling rate.\";\"sr = iarg\"\n"
"\"endin\";\"Orchestra Syntax:Block Statements\";\"Ends the current instrument block.\";\"endin\"\n"
"\"endop\";\"Orchestra Syntax:Block Statements\";\"Marks the end of an user-defined opcode block.\";\"endop\"\n"
"\"instr\";\"Orchestra Syntax:Block Statements\";\"Starts an instrument block.\";\"instr i, j, ...\"\n"
"\"opcode\";\"Orchestra Syntax:Block Statements\";\"Defines the start of user-defined opcode block.\";\"opcode name, outtypes, intypes\"\n"
"\"#define\";\"Orchestra Syntax:Macros\";\"Defines a macro.\";\"#define NAME # replacement text #\"\n"
"\"$NAME\";\"Orchestra Syntax:Macros\";\"Calls a defined macro.\";\"$NAME\"\n"
"\"#ifdef\";\"Orchestra Syntax:Macros\";\"Conditional reading of code.\";\"#ifdef NAME\"\n"
"\"#else\";\"Orchestra Syntax:Macros\";\"Conditional reading of code.\";\"#else\"\n"
"\"#end\";\"Orchestra Syntax:Macros\";\"Conditional reading of code.\";\"#end\"\n"
"\"#ifndef\";\"Orchestra Syntax:Macros\";\"Conditional reading of code.\";\"#ifndef NAME\"\n"
"\"#else\";\"Orchestra Syntax:Macros\";\"Conditional reading of code.\";\"#else\"\n"
"\"#end\";\"Orchestra Syntax:Macros\";\"Conditional reading of code.\";\"#end\"\n"
"\"#include\";\"Orchestra Syntax:Macros\";\"Includes an external file for processing.\";\"#include \"filename\"\"\n"
"\"#undef\";\"Orchestra Syntax:Macros\";\"Un-defines a macro.\";\"#undef NAME\"\n"
"\"adsyn\";\"Signal Generators:Additive Synthesis/Resynthesis\";\"Output is an additive set of individually controlled sinusoids, using an oscillator bank.\";\"ares adsyn kamod, kfmod, ksmod, ifilcod\"\n"
"\"adsynt\";\"Signal Generators:Additive Synthesis/Resynthesis\";\"Performs additive synthesis with an arbitrary number of partials, not necessarily harmonic.\";\"ares adsynt kamp, kcps, iwfn, ifreqfn, iampfn, icnt [, iphs]\"\n"
"\"adsynt2\";\"Signal Generators:Additive Synthesis/Resynthesis\";\"Performs additive synthesis with an arbitrary number of partials -not necessarily harmonic- with interpolation.\";\"ar adsynt2 kamp, kcps, iwfn, ifreqfn, iampfn, icnt [, iphs]\"\n"
"\"hsboscil\";\"Signal Generators:Additive Synthesis/Resynthesis\";\"An oscillator which takes tonality and brightness as arguments.\";\"ares hsboscil kamp, ktone, kbrite, ibasfreq, iwfn, ioctfn  [, ioctcnt] [, iphs]\"\n"
"\"lfo\";\"Signal Generators:Basic Oscillators\";\"A low frequency oscillator of various shapes.\";\"kres lfo kamp, kcps [, itype]\"\n"
"\"oscbnk\";\"Signal Generators:Basic Oscillators\";\"Mixes the output of any number of oscillators.\";\"ares oscbnk kcps, kamd, kfmd, kpmd, iovrlap, iseed, kl1minf, kl1maxf,  kl2minf, kl2maxf, ilfomode, keqminf, keqmaxf, keqminl, keqmaxl,  keqminq, keqmaxq, ieqmode, kfn [, il1fn] [, il2fn] [, ieqffn]  [, ieqlfn] [, ieqqfn] [, itabl] [, ioutfn]\"\n"
"\"oscil\";\"Signal Generators:Basic Oscillators\";\"A simple oscillator.\";\"ares oscil xamp, xcps, ifn [, iphs]\"\n"
"\"oscil3\";\"Signal Generators:Basic Oscillators\";\"A simple oscillator with cubic interpolation.\";\"ares oscil3 xamp, xcps, ifn [, iphs]\"\n"
"\"oscili\";\"Signal Generators:Basic Oscillators\";\"A simple oscillator with linear interpolation.\";\"ares oscili xamp, xcps, ifn [, iphs]\"\n"
"\"oscilikt\";\"Signal Generators:Basic Oscillators\";\"A linearly interpolated oscillator that allows changing the table number at k-rate.\";\"ares oscilikt xamp, xcps, kfn [, iphs] [, istor]\"\n"
"\"osciliktp\";\"Signal Generators:Basic Oscillators\";\"A linearly interpolated oscillator that allows allows phase modulation.\";\"ares osciliktp kcps, kfn, kphs [, istor]\"\n"
"\"oscilikts\";\"Signal Generators:Basic Oscillators\";\"A linearly interpolated oscillator with sync status that allows changing the table number at k-rate.\";\"ares oscilikts xamp, xcps, kfn, async, kphs [, istor]\"\n"
"\"osciln\";\"Signal Generators:Basic Oscillators\";\"Accesses table values at a user-defined frequency.\";\"ares osciln kamp, ifrq, ifn, itimes\"\n"
"\"oscilx\";\"Signal Generators:Basic Oscillators\";\"(same as osciln) Accesses table values at a user-defined frequency.\";\"ares osciln kamp, ifrq, ifn, itimes\"\n"
"\"oscils\";\"Signal Generators:Basic Oscillators\";\"A simple, fast sine oscillator\";\"ares oscils iamp, icps, iphs [, iflg]\"\n"
"\"poscil\";\"Signal Generators:Basic Oscillators\";\"High precision oscillator.\";\"ares poscil aamp, acps, ifn [, iphs]\"\n"
"\"poscil3\";\"Signal Generators:Basic Oscillators\";\"High precision oscillator with cubic interpolation.\";\"ares poscil3 aamp, acps, ifn [, iphs]\"\n"
"\"vibr\";\"Signal Generators:Basic Oscillators\";\"Easier-to-use user-controllable vibrato.\";\"kout vibr kAverageAmp, kAverageFreq, ifn\"\n"
"\"vibrato\";\"Signal Generators:Basic Oscillators\";\"Generates a natural-sounding user-controllable vibrato.\";\"kout vibrato kAverageAmp, kAverageFreq, kRandAmountAmp, kRandAmountFreq, kAmpMinRate, kAmpMaxRate, kcpsMinRate, kcpsMaxRate, ifn [, iphs\"\n"
"\"buzz\";\"Signal Generators:Dynamic Spectrum Oscillators\";\"Output is a set of harmonically related sine partials.\";\"ares buzz xamp, xcps, knh, ifn [, iphs]\"\n"
"\"gbuzz\";\"Signal Generators:Dynamic Spectrum Oscillators\";\"Output is a set of harmonically related cosine partials.\";\"ares gbuzz xamp, xcps, knh, klh, kmul, ifn [, iphs]\"\n"
"\"mpulse\";\"Signal Generators:Dynamic Spectrum Oscillators\";\"Generates a set of impulses.\";\"ares mpulse kamp, kintvl [, ioffset]\"\n"
"\"vco\";\"Signal Generators:Dynamic Spectrum Oscillators\";\"Implementation of a band limited, analog modeled oscillator.\";\"ares vco xamp, xcps, iwave, kpw [, ifn] [, imaxd] [, ileak] [, inyx]  [, iphs] [, iskip]\"\n"
"\"vco2\";\"Signal Generators:Dynamic Spectrum Oscillators\";\"Implementation of a band-limited oscillator using pre-calculated tables.\";\"ares vco2 kamp, kcps [, imode] [, kpw] [, kphs] [, inyx]\"\n"
"\"vco2ft\";\"Signal Generators:Dynamic Spectrum Oscillators\";\"Returns a table number at k-time for a given oscillator frequency and wavform.\";\"kfn vco2ft kcps, iwave [, inyx]\"\n"
"\"vco2ift\";\"Signal Generators:Dynamic Spectrum Oscillators\";\"Returns a table number at i-time for a given oscillator frequency and wavform.\";\"ifn vco2ift icps, iwave [, inyx]\"\n"
"\"vco2init\";\"Signal Generators:Dynamic Spectrum Oscillators\";\"Calculates tables for use by vco2 opcode.\";\"ifn vco2init iwave [, ibasfn] [, ipmul] [, iminsiz] [, imaxsiz] [, isrcft]\"\n"
"\"crossfm\";\"Signal Generators:FM Synthesis\";\"Two mutually frequency and/or phase modulated oscillators.\";\"a1, a2 crossfm xfrq1, xfrq2, xndx1, xndx2, kcps, ifn1, ifn2 [, iphs1] [, iphs2]\"\n"
"\"crossfmi\";\"Signal Generators:FM Synthesis\";\"Two mutually frequency and/or phase modulated oscillators.\";\"a1, a2 crossfmi xfrq1, xfrq2, xndx1, xndx2, kcps, ifn1, ifn2 [, iphs1] [, iphs2]\"\n"
"\"crosspm\";\"Signal Generators:FM Synthesis\";\"Two mutually frequency and/or phase modulated oscillators.\";\"a1, a2 crosspm xfrq1, xfrq2, xndx1, xndx2, kcps, ifn1, ifn2 [, iphs1] [, iphs2]\"\n"
"\"crosspmi\";\"Signal Generators:FM Synthesis\";\"Two mutually frequency and/or phase modulated oscillators.\";\"a1, a2 crosspmi xfrq1, xfrq2, xndx1, xndx2, kcps, ifn1, ifn2 [, iphs1] [, iphs2]\"\n"
"\"crossfmpm\";\"Signal Generators:FM Synthesis\";\"Two mutually frequency and/or phase modulated oscillators.\";\"a1, a2 crossfmpm xfrq1, xfrq2, xndx1, xndx2, kcps, ifn1, ifn2 [, iphs1] [, iphs2]\"\n"
"\"crossfmpmi\";\"Signal Generators:FM Synthesis\";\"Two mutually frequency and/or phase modulated oscillators.\";\"a1, a2 crossfmpmi xfrq1, xfrq2, xndx1, xndx2, kcps, ifn1, ifn2 [, iphs1] [, iphs2]\"\n"
"\"fmb3\";\"Signal Generators:FM Synthesis\";\"Uses FM synthesis to create a Hammond B3 organ sound.\";\"ares fmb3 kamp, kfreq, kc1, kc2, kvdepth, kvrate, ifn1, ifn2, ifn3,  ifn4, ivfn\"\n"
"\"fmbell\";\"Signal Generators:FM Synthesis\";\"Uses FM synthesis to create a tublar bell sound.\";\"ares fmbell kamp, kfreq, kc1, kc2, kvdepth, kvrate, ifn1, ifn2, ifn3,  ifn4, ivfn[, isus]\"\n"
"\"fmmetal\";\"Signal Generators:FM Synthesis\";\"Uses FM synthesis to create a\";\"ares fmmetal kamp, kfreq, kc1, kc2, kvdepth, kvrate, ifn1, ifn2, ifn3,  ifn4, ivfn\"\n"
"\"fmpercfl\";\"Signal Generators:FM Synthesis\";\"Uses FM synthesis to create a percussive flute sound.\";\"ares fmpercfl kamp, kfreq, kc1, kc2, kvdepth, kvrate, ifn1, ifn2,  ifn3, ifn4, ivfn\"\n"
"\"fmrhode\";\"Signal Generators:FM Synthesis\";\"Uses FM synthesis to create a Fender Rhodes electric piano sound.\";\"ares fmrhode kamp, kfreq, kc1, kc2, kvdepth, kvrate, ifn1, ifn2,  ifn3, ifn4, ivfn\"\n"
"\"fmvoice\";\"Signal Generators:FM Synthesis\";\"FM Singing Voice Synthesis\";\"ares fmvoice kamp, kfreq, kvowel, ktilt, kvibamt, kvibrate, ifn1,  ifn2, ifn3, ifn4, ivibfn\"\n"
"\"fmwurlie\";\"Signal Generators:FM Synthesis\";\"Uses FM synthesis to create a Wurlitzer electric piano sound.\";\"ares fmwurlie kamp, kfreq, kc1, kc2, kvdepth, kvrate, ifn1, ifn2, ifn3,  ifn4, ivfn\"\n"
"\"foscil\";\"Signal Generators:FM Synthesis\";\"A basic frequency modulated oscillator.\";\"ares foscil xamp, kcps, xcar, xmod, kndx, ifn [, iphs]\"\n"
"\"foscili\";\"Signal Generators:FM Synthesis\";\"Basic frequency modulated oscillator with linear interpolation.\";\"ares foscili xamp, kcps, xcar, xmod, kndx, ifn [, iphs]\"\n"
"\"diskgrain\";\"Signal Generators:Granular Synthesis\";\"Synchronous granular synthesis, using a soundfile as source.\";\"asig diskgrain Sfname, kamp, kfreq, kpitch, kgrsize, kprate,  ifun, iolaps [,imaxgrsize , ioffset]\"\n"
"\"fof\";\"Signal Generators:Granular Synthesis\";\"Produces sinusoid bursts useful for formant and granular synthesis.\";\"ares fof xamp, xfund, xform, koct, kband, kris, kdur, kdec, iolaps,  ifna, ifnb, itotdur [, iphs] [, ifmode] [, iskip]\"\n"
"\"fof2\";\"Signal Generators:Granular Synthesis\";\"Produces sinusoid bursts including k-rate incremental indexing with each successive burst.\";\"ares fof2 xamp, xfund, xform, koct, kband, kris, kdur, kdec, iolaps,  ifna, ifnb, itotdur, kphs, kgliss [, iskip]\"\n"
"\"fog\";\"Signal Generators:Granular Synthesis\";\"Audio output is a succession of grains derived from data in a stored function table\";\"ares fog xamp, xdens, xtrans, aspd, koct, kband, kris, kdur, kdec,  iolaps, ifna, ifnb, itotdur [, iphs] [, itmode] [, iskip]\"\n"
"\"grain\";\"Signal Generators:Granular Synthesis\";\"Generates granular synthesis textures.\";\"ares grain xamp, xpitch, xdens, kampoff, kpitchoff, kgdur, igfn,  iwfn, imgdur [, igrnd]\"\n"
"\"grain2\";\"Signal Generators:Granular Synthesis\";\"Easy-to-use granular synthesis texture generator.\";\"ares grain2 kcps, kfmd, kgdur, iovrlp, kfn, iwfn [, irpow]  [, iseed] [, imode]\"\n"
"\"grain3\";\"Signal Generators:Granular Synthesis\";\"Generate granular synthesis textures with more user control.\";\"ares grain3 kcps, kphs, kfmd, kpmd, kgdur, kdens, imaxovr, kfn, iwfn,  kfrpow, kprpow [, iseed] [, imode]\"\n"
"\"granule\";\"Signal Generators:Granular Synthesis\";\"A more complex granular synthesis texture generator.\";\"ares granule xamp, ivoice, iratio, imode, ithd, ifn, ipshift, igskip,  igskip_os, ilength, kgap, igap_os, kgsize, igsize_os, iatt, idec  [, iseed] [, ipitch1] [, ipitch2] [, ipitch3] [, ipitch4] [, ifnenv]\"\n"
"\"partikkel\";\"Signal Generators:Granular Synthesis\";\"Granular synthesizer with \"per grain\" control over many of its parameters. Has a sync input to sychronize its internal grain scheduler clock to an external clock source.\";\"a1 [, a2, a3, a4, a5, a6, a7, a8] partikkel agrainfreq,  kdistribution, idisttab, async, kenv2amt, ienv2tab, ienv_attack,  ienv_decay, ksustain_amount, ka_d_ratio, kduration, kamp, igainmasks,  kwavfreq, ksweepshape, iwavfreqstarttab, iwavfreqendtab, awavfm,  ifmamptab, kfmenv, icosine, ktraincps, knumpartials, kchroma,  ichannelmasks, krandommask, kwaveform1, kwaveform2, kwaveform3,  kwaveform4, iwaveamptab, asamplepos1, asamplepos2, asamplepos3,  asamplepos4, kwavekey1, kwavekey2, kwavekey3, kwavekey4, imax_grains  [, iopcode_id]\"\n"
"\"partikkelsync\";\"Signal Generators:Granular Synthesis\";\"Outputs\";\"async [,aphase] partikkelsync iopcode_id\"\n"
"\"sndwarp\";\"Signal Generators:Granular Synthesis\";\"Reads a mono sound sample from a table and applies time-stretching and/or pitch modification.\";\"ares [, ac] sndwarp xamp, xtimewarp, xresample, ifn1, ibeg, iwsize,  irandw, ioverlap, ifn2, itimemode\"\n"
"\"sndwarpst\";\"Signal Generators:Granular Synthesis\";\"Reads a stereo sound sample from a table and applies time-stretching and/or pitch modification.\";\"ar1, ar2 [,ac1] [, ac2] sndwarpst xamp, xtimewarp, xresample, ifn1,  ibeg, iwsize, irandw, ioverlap, ifn2, itimemode\"\n"
"\"syncgrain\";\"Signal Generators:Granular Synthesis\";\"Synchronous granular synthesis.\";\"asig syncgrain kamp, kfreq, kpitch, kgrsize, kprate, ifun1,  ifun2, iolaps\"\n"
"\"syncloop\";\"Signal Generators:Granular Synthesis\";\"Synchronous granular synthesis.\";\"asig syncloop kamp, kfreq, kpitch, kgrsize, kprate, klstart,  klend, ifun1, ifun2, iolaps[,istart, iskip]\"\n"
"\"vosim\";\"Signal Generators:Granular Synthesis\";\"Simple vocal simulation based on glottal pulses with formant characteristics.\";\"ar vosim kamp, kFund, kForm, kDecay, kPulseCount, kPulseFactor, ifn [, iskip]\"\n"
"\"hvs1\";\"Signal Generators:Hyper Vectorial Synthesis\";\"Allows one-dimensional Hyper Vectorial Synthesis (HVS) controlled by externally-updated k-variables.\";\"hvs1 kx, inumParms, inumPointsX, iOutTab, iPositionsTab, iSnapTab [, iConfigTab]\"\n"
"\"hvs2\";\"Signal Generators:Hyper Vectorial Synthesis\";\"Allows two-dimensional Hyper Vectorial Synthesis (HVS) controlled by externally-updated k-variables.\";\"hvs2 kx, ky, inumParms, inumPointsX, inumPointsY, iOutTab, iPositionsTab, iSnapTab [, iConfigTab]\"\n"
"\"hvs3\";\"Signal Generators:Hyper Vectorial Synthesis\";\"Allows three-dimensional Hyper Vectorial Synthesis (HVS) controlled by externally-updated k-variables.\";\"hvs3 kx, ky, kz, inumParms, inumPointsX, inumPointsY, inumPointsZ, iOutTab, iPositionsTab, iSnapTab [, iConfigTab]\"\n"
"\"expcurve\";\"Signal Generators:Linear and Exponential Generators\";\"This opcode implements a formula for generating a normalised exponential curve in range 0 - 1. It is based on the Max / MSP work of Eric Singer (c) 1994.\";\"kout expcurve kindex, ksteepness\"\n"
"\"expon\";\"Signal Generators:Linear and Exponential Generators\";\"Trace an exponential curve between specified points.\";\"ares expon ia, idur, ib\"\n"
"\"expseg\";\"Signal Generators:Linear and Exponential Generators\";\"Trace a series of exponential segments between specified points.\";\"ares expseg ia, idur1, ib [, idur2] [, ic] [...]\"\n"
"\"expsega\";\"Signal Generators:Linear and Exponential Generators\";\"An exponential segment generator operating at a-rate.\";\"ares expsega ia, idur1, ib [, idur2] [, ic] [...]\"\n"
"\"expsegb\";\"Signal Generators:Linear and Exponential Generators\";\"Trace a series of exponential segments between specified absolute points.\";\"ares expsegb ia, itim1, ib [, itim2] [, ic] [...]\"\n"
"\"expsegba\";\"Signal Generators:Linear and Exponential Generators\";\"An exponential segment generator operating at a-rate with absolute times.\";\"ares expsegba ia, itim1, ib [, itim2] [, ic] [...]\"\n"
"\"expsegr\";\"Signal Generators:Linear and Exponential Generators\";\"Trace a series of exponential segments between specified points including a release segment.\";\"ares expsegr ia, idur1, ib [, idur2] [, ic] [...], irel, iz\"\n"
"\"gainslider\";\"Signal Generators:Linear and Exponential Generators\";\"An implementation of a logarithmic gain curve which is similar to the gainslider~ object from Cycling 74 Max / MSP.\";\"kout gainslider kindex\"\n"
"\"jspline\";\"Signal Generators:Linear and Exponential Generators\";\"A jitter-spline generator.\";\"ares jspline xamp, kcpsMin, kcpsMax\"\n"
"\"line\";\"Signal Generators:Linear and Exponential Generators\";\"Trace a straight line between specified points.\";\"ares line ia, idur, ib\"\n"
"\"linseg\";\"Signal Generators:Linear and Exponential Generators\";\"Trace a series of line segments between specified points.\";\"ares linseg ia, idur1, ib [, idur2] [, ic] [...]\"\n"
"\"linsegb\";\"Signal Generators:Linear and Exponential Generators\";\"Trace a series of line segments between specified absolute points.\";\"ares linsegb ia, itim1, ib [, itim2] [, ic] [...]\"\n"
"\"linsegr\";\"Signal Generators:Linear and Exponential Generators\";\"Trace a series of line segments between specified points including a release segment.\";\"ares linsegr ia, idur1, ib [, idur2] [, ic] [...], irel, iz\"\n"
"\"logcurve\";\"Signal Generators:Linear and Exponential Generators\";\"This opcode implements a formula for generating a normalised logarithmic curve in range 0 - 1. It is based on the Max / MSP work of Eric Singer (c) 1994.\";\"kout logcurve kindex, ksteepness\"\n"
"\"loopseg\";\"Signal Generators:Linear and Exponential Generators\";\"Generate control signal consisting of linear segments delimited by two or more specified points.\";\"ksig loopseg kfreq, ktrig, ktime0, kvalue0 [, ktime1] [, kvalue1]  [, ktime2] [, kvalue2] [...]\"\n"
"\"loopsegp\";\"Signal Generators:Linear and Exponential Generators\";\"Control signals based on linear segments.\";\"ksig loopsegp kphase, kvalue0, kdur0, kvalue1  [, kdur1, ... , kdurN-1, kvalueN]\"\n"
"\"looptseg\";\"Signal Generators:Linear and Exponential Generators\";\"Generate control signal consisting of exponential or linear segments delimited by two or more specified points.\";\"ksig looptseg kfreq, ktrig, ktime0, kvalue0, ktype0, [, ktime1] [, kvalue1] [,ktype1]  [, ktime2] [, kvalue2] [,ktype2] [...]\"\n"
"\"loopxseg\";\"Signal Generators:Linear and Exponential Generators\";\"Generate control signal consisting of exponential segments delimited by two or more specified points.\";\"ksig loopxseg kfreq, ktrig, ktime0, kvalue0 [, ktime1] [, kvalue1]  [, ktime2] [, kvalue2] [...]\"\n"
"\"lpshold\";\"Signal Generators:Linear and Exponential Generators\";\"Generate control signal consisting of held segments.\";\"ksig lpshold kfreq, ktrig, ktime0, kvalue0 [, ktime1] [, kvalue1]  [, ktime2] [, kvalue2] [...]\"\n"
"\"lpsholdp\";\"Signal Generators:Linear and Exponential Generators\";\"Control signals based on held segments.\";\"ksig lpsholdp kphase, ktrig, ktime0, kvalue0 [, ktime1] [, kvalue1]  [, ktime2] [, kvalue2] [...]\"\n"
"\"rspline\";\"Signal Generators:Linear and Exponential Generators\";\"Generate random spline curves.\";\"ares rspline xrangeMin, xrangeMax, kcpsMin, kcpsMax\"\n"
"\"scale\";\"Signal Generators:Linear and Exponential Generators\";\"Arbitrary signal scaling.\";\"kscl scale kinput, kmax, kmin\"\n"
"\"transeg\";\"Signal Generators:Linear and Exponential Generators\";\"Constructs a user-definable envelope.\";\"ares transeg ia, idur, itype, ib [, idur2] [, itype] [, ic] ...\"\n"
"\"transegb\";\"Signal Generators:Linear and Exponential Generators\";\"Constructs a user-definable envelope in absolute time.\";\"ares transegb ia, itim, itype, ib [, itim2] [, itype] [, ic] ...\"\n"
"\"transegr\";\"Signal Generators:Linear and Exponential Generators\";\"Constructs a user-definable envelope with extended release segment.\";\"ares transegr ia, idur, itype, ib [, idur2] [, itype] [, ic] ...\"\n"
"\"adsr\";\"Signal Generators:Envelope Generators\";\"Calculates the classical ADSR envelope using linear segments.\";\"ares adsr iatt, idec, islev, irel [, idel]\"\n"
"\"envlpx\";\"Signal Generators:Envelope Generators\";\"Applies an envelope consisting of 3 segments.\";\"ares envlpx xamp, irise, idur, idec, ifn, iatss, iatdec [, ixmod]\"\n"
"\"envlpxr\";\"Signal Generators:Envelope Generators\";\"The\";\"ares envlpxr xamp, irise, idec, ifn, iatss, iatdec [, ixmod] [,irind]\"\n"
"\"linen\";\"Signal Generators:Envelope Generators\";\"Applies a straight line rise and decay pattern to an input amp signal.\";\"ares linen xamp, irise, idur, idec\"\n"
"\"linenr\";\"Signal Generators:Envelope Generators\";\"The\";\"ares linenr xamp, irise, idec, iatdec\"\n"
"\"madsr\";\"Signal Generators:Envelope Generators\";\"Calculates the classical ADSR envelope using the\";\"ares madsr iatt, idec, islev, irel [, idel] [, ireltim]\"\n"
"\"mxadsr\";\"Signal Generators:Envelope Generators\";\"Calculates the classical ADSR envelope using the\";\"ares mxadsr iatt, idec, islev, irel [, idel] [, ireltim]\"\n"
"\"xadsr\";\"Signal Generators:Envelope Generators\";\"Calculates the classical ADSR envelope.\";\"ares xadsr iatt, idec, islev, irel [, idel]\"\n"
"\"bamboo\";\"Signal Generators:Models and Emulations\";\"Semi-physical model of a bamboo sound.\";\"ares bamboo kamp, idettack [, inum] [, idamp] [, imaxshake] [, ifreq]  [, ifreq1] [, ifreq2]\"\n"
"\"barmodel\";\"Signal Generators:Models and Emulations\";\"Creates a tone similar to a struck metal bar.\";\"ares barmodel kbcL, kbcR, iK, ib, kscan, iT30, ipos, ivel, iwid\"\n"
"\"cabasa\";\"Signal Generators:Models and Emulations\";\"Semi-physical model of a cabasa sound.\";\"ares cabasa iamp, idettack [, inum] [, idamp] [, imaxshake]\"\n"
"\"chuap\";\"Signal Generators:Models and Emulations\";\"Simulates Chua's oscillator, an LRC oscillator with an active resistor, proved capable of bifurcation and chaotic attractors, with k-rate control of circuit elements.\";\"aI3, aV2, aV1 chuap kL, kR0, kC1, kG, kGa, kGb, kE, kC2, iI3, iV2, iV1, ktime_step\"\n"
"\"crunch\";\"Signal Generators:Models and Emulations\";\"Semi-physical model of a crunch sound.\";\"ares crunch iamp, idettack [, inum] [, idamp] [, imaxshake]\"\n"
"\"dripwater\";\"Signal Generators:Models and Emulations\";\"Semi-physical model of a water drop.\";\"ares dripwater kamp, idettack [, inum] [, idamp] [, imaxshake] [, ifreq]  [, ifreq1] [, ifreq2]\"\n"
"\"gendy\";\"Signal Generators:Models and Emulations\";\"Dynamic stochastic approach to waveform synthesis conceived by Iannis Xenakis.\";\"ares gendy kamp, kampdist, kdurdist, kadpar, kddpar, kminfreq, kmaxfreq,  kampscl, kdurscl [, initcps] [, knum]\"\n"
"\"gendyc\";\"Signal Generators:Models and Emulations\";\"Dynamic stochastic approach to waveform synthesis using cubic interpolation.\";\"ares gendyc kamp, kampdist, kdurdist, kadpar, kddpar, kminfreq, kmaxfreq,  kampscl, kdurscl [, initcps] [, knum]\"\n"
"\"gendyx\";\"Signal Generators:Models and Emulations\";\"Variation of the dynamic stochastic approach to waveform synthesis conceived by Iannis Xenakis.\";\"ares gendyx kamp, kampdist, kdurdist, kadpar, kddpar, kminfreq, kmaxfreq,  kampscl, kdurscl, kcurveup, kcurvedown [, initcps] [, knum]\"\n"
"\"gogobel\";\"Signal Generators:Models and Emulations\";\"Audio output is a tone related to the striking of a cow bell or similar.\";\"ares gogobel kamp, kfreq, ihrd, ipos, imp, kvibf, kvamp, ivfn\"\n"
"\"guiro\";\"Signal Generators:Models and Emulations\";\"Semi-physical model of a guiro sound.\";\"ares guiro kamp, idettack [, inum] [, idamp] [, imaxshake] [, ifreq] [, ifreq1]\"\n"
"\"lorenz\";\"Signal Generators:Models and Emulations\";\"Implements the Lorenz system of equations.\";\"ax, ay, az lorenz ksv, krv, kbv, kh, ix, iy, iz, iskip [, iskipinit]\"\n"
"\"mandel\";\"Signal Generators:Models and Emulations\";\"Mandelbrot set\";\"kiter, koutrig mandel ktrig, kx, ky, kmaxIter\"\n"
"\"mandol\";\"Signal Generators:Models and Emulations\";\"An emulation of a mandolin.\";\"ares mandol kamp, kfreq, kpluck, kdetune, kgain, ksize, ifn [, iminfreq]\"\n"
"\"marimba\";\"Signal Generators:Models and Emulations\";\"Physical model related to the striking of a wooden block.\";\"ares marimba kamp, kfreq, ihrd, ipos, imp, kvibf, kvamp, ivibfn, idec  [, idoubles] [, itriples]\"\n"
"\"moog\";\"Signal Generators:Models and Emulations\";\"An emulation of a mini-Moog synthesizer.\";\"ares moog kamp, kfreq, kfiltq, kfiltrate, kvibf, kvamp, iafn, iwfn, ivfn\"\n"
"\"planet\";\"Signal Generators:Models and Emulations\";\"Simulates a planet orbiting in a binary star system.\";\"ax, ay, az planet kmass1, kmass2, ksep, ix, iy, iz, ivx, ivy, ivz, idelta  [, ifriction] [, iskip]\"\n"
"\"prepiano\";\"Signal Generators:Models and Emulations\";\"Creates a tone similar to a piano string prepared in a Cageian fashion.\";\"ares prepiano ifreq, iNS, iD, iK,  iT30,iB, kbcl, kbcr, imass, ifreq, iinit, ipos, ivel, isfreq,  isspread[, irattles, irubbers]\"\n"
"\"sandpaper\";\"Signal Generators:Models and Emulations\";\"Semi-physical model of a sandpaper sound.\";\"ares sandpaper iamp, idettack [, inum] [, idamp] [, imaxshake]\"\n"
"\"sekere\";\"Signal Generators:Models and Emulations\";\"Semi-physical model of a sekere sound.\";\"ares sekere iamp, idettack [, inum] [, idamp] [, imaxshake]\"\n"
"\"shaker\";\"Signal Generators:Models and Emulations\";\"Sounds like the shaking of a maraca or similar gourd instrument.\";\"ares shaker kamp, kfreq, kbeans, kdamp, ktimes [, idecay]\"\n"
"\"sleighbells\";\"Signal Generators:Models and Emulations\";\"Semi-physical model of a sleighbell sound.\";\"ares sleighbells kamp, idettack [, inum] [, idamp] [, imaxshake] [, ifreq]  [, ifreq1] [, ifreq2]\"\n"
"\"stix\";\"Signal Generators:Models and Emulations\";\"Semi-physical model of a stick sound.\";\"ares stix iamp, idettack [, inum] [, idamp] [, imaxshake]\"\n"
"\"tambourine\";\"Signal Generators:Models and Emulations\";\"Semi-physical model of a tambourine sound.\";\"ares tambourine kamp, idettack [, inum] [, idamp] [, imaxshake] [, ifreq]  [, ifreq1] [, ifreq2]\"\n"
"\"vibes\";\"Signal Generators:Models and Emulations\";\"Physical model related to the striking of a metal block.\";\"ares vibes kamp, kfreq, ihrd, ipos, imp, kvibf, kvamp, ivibfn, idec\"\n"
"\"voice\";\"Signal Generators:Models and Emulations\";\"An emulation of a human voice.\";\"ares voice kamp, kfreq, kphoneme, kform, kvibf, kvamp, ifn, ivfn\"\n"
"\"phasor\";\"Signal Generators:Phasors\";\"Produce a normalized moving phase value.\";\"ares phasor xcps [, iphs]\"\n"
"\"phasorbnk\";\"Signal Generators:Phasors\";\"Produce an arbitrary number of normalized moving phase values.\";\"ares phasorbnk xcps, kndx, icnt [, iphs]\"\n"
"\"syncphasor\";\"Signal Generators:Phasors\";\"Produces a normalized moving phase value with sync input and output.\";\"aphase, asyncout syncphasor xcps, asyncin, [, iphs]\"\n"
"\"betarand\";\"Signal Generators:Random (Noise) Generators\";\"Beta distribution random number generator (positive values only).\";\"ares betarand krange, kalpha, kbeta\"\n"
"\"bexprnd\";\"Signal Generators:Random (Noise) Generators\";\"Exponential distribution random number generator.\";\"ares bexprnd krange\"\n"
"\"cauchy\";\"Signal Generators:Random (Noise) Generators\";\"Cauchy distribution random number generator.\";\"ares cauchy kalpha\"\n"
"\"cauchyi\";\"Signal Generators:Random (Noise) Generators\";\"Cauchy distribution random number generator with interpolation.\";\"ares cauchyi klambda, xamp, xcps\"\n"
"\"cuserrnd\";\"Signal Generators:Random (Noise) Generators\";\"Continuous USER-defined-distribution RaNDom generator.\";\"aout cuserrnd kmin, kmax, ktableNum\"\n"
"\"duserrnd\";\"Signal Generators:Random (Noise) Generators\";\"Discrete USER-defined-distribution RaNDom generator.\";\"aout duserrnd ktableNum\"\n"
"\"dust\";\"Signal Generators:Random (Noise) Generators\";\"Random impulses.\";\"ares dust kamp, kdensity\"\n"
"\"dust2\";\"Signal Generators:Random (Noise) Generators\";\"Random impulses.\";\"ares dust2 kamp, kdensity\"\n"
"\"exprand\";\"Signal Generators:Random (Noise) Generators\";\"Exponential distribution random number generator (positive values only).\";\"ares exprand klambda\"\n"
"\"exprandi\";\"Signal Generators:Random (Noise) Generators\";\"Exponential distribution random number generator with interpolation (positive values only).\";\"ares exprandi klambda, xamp, xcps\"\n"
"\"fractalnoise\";\"Signal Generators:Random (Noise) Generators\";\"A fractal noise generator.\";\"ares fractalnoise kamp, kbeta\"\n"
"\"gauss\";\"Signal Generators:Random (Noise) Generators\";\"Gaussian distribution random number generator.\";\"ares gauss krange\"\n"
"\"gaussi\";\"Signal Generators:Random (Noise) Generators\";\"Gaussian distribution random number generator with interpolation.\";\"ares gaussi krange, xamp, xcps\"\n"
"\"gausstrig\";\"Signal Generators:Random (Noise) Generators\";\"Random impulses around a certain frequency.\";\"ares gausstrig kamp, kcps, kdev [, imode]\"\n"
"\"jitter\";\"Signal Generators:Random (Noise) Generators\";\"Generates a segmented line whose segments are randomly generated.\";\"kout jitter kamp, kcpsMin, kcpsMax\"\n"
"\"jitter2\";\"Signal Generators:Random (Noise) Generators\";\"Generates a segmented line with user-controllable random segments.\";\"kout jitter2 ktotamp, kamp1, kcps1, kamp2, kcps2, kamp3, kcps3\"\n"
"\"linrand\";\"Signal Generators:Random (Noise) Generators\";\"Linear distribution random number generator (positive values only).\";\"ares linrand krange\"\n"
"\"noise\";\"Signal Generators:Random (Noise) Generators\";\"A white noise generator with an IIR lowpass filter.\";\"ares noise xamp, kbeta\"\n"
"\"pcauchy\";\"Signal Generators:Random (Noise) Generators\";\"Cauchy distribution random number generator (positive values only).\";\"ares pcauchy kalpha\"\n"
"\"pinkish\";\"Signal Generators:Random (Noise) Generators\";\"Generates approximate pink noise.\";\"ares pinkish xin [, imethod] [, inumbands] [, iseed] [, iskip]\"\n"
"\"poisson\";\"Signal Generators:Random (Noise) Generators\";\"Poisson distribution random number generator (positive values only).\";\"ares poisson klambda\"\n"
"\"rand\";\"Signal Generators:Random (Noise) Generators\";\"Generates a controlled random number series.\";\"ares rand xamp [, iseed] [, isel] [, ioffset]\"\n"
"\"randh\";\"Signal Generators:Random (Noise) Generators\";\"Generates random numbers and holds them for a period of time.\";\"ares randh xamp, xcps [, iseed] [, isize] [, ioffset]\"\n"
"\"randi\";\"Signal Generators:Random (Noise) Generators\";\"Generates a controlled random number series with interpolation between each new number.\";\"ares randi xamp, xcps [, iseed] [, isize] [, ioffset]\"\n"
"\"random\";\"Signal Generators:Random (Noise) Generators\";\"Generates a controlled pseudo-random number series between min and max values.\";\"ares random kmin, kmax\"\n"
"\"randomh\";\"Signal Generators:Random (Noise) Generators\";\"Generates random numbers with a user-defined limit and holds them for a period of time.\";\"ares randomh kmin, kmax, xcps [,imode] [,ifirstval]\"\n"
"\"randomi\";\"Signal Generators:Random (Noise) Generators\";\"Generates a user-controlled random number series with interpolation between each new number.\";\"ares randomi kmin, kmax, xcps [,imode] [,ifirstval]\"\n"
"\"rnd31\";\"Signal Generators:Random (Noise) Generators\";\"31-bit bipolar random opcodes with controllable distribution.\";\"ax rnd31 kscl, krpow [, iseed]\"\n"
"\"seed\";\"Signal Generators:Random (Noise) Generators\";\"Sets the global seed value.\";\"seed ival\"\n"
"\"trandom\";\"Signal Generators:Random (Noise) Generators\";\"Generates a controlled pseudo-random number series between min and max values according to a trigger.\";\"kout trandom ktrig, kmin, kmax\"\n"
"\"trirand\";\"Signal Generators:Random (Noise) Generators\";\"Triangular distribution random number generator\";\"ares trirand krange\"\n"
"\"unirand\";\"Signal Generators:Random (Noise) Generators\";\"Uniform distribution random number generator (positive values only).\";\"ares unirand krange\"\n"
"\"urandom\";\"Signal Generators:Random (Noise) Generators\";\"truly random opcodes with controllable range.\";\"ax urandom [imin, imax]\"\n"
"\"urd\";\"Signal Generators:Random (Noise) Generators\";\"A discrete user-defined-distribution random generator that can be used as a function.\";\"aout = urd(ktableNum)\"\n"
"\"weibull\";\"Signal Generators:Random (Noise) Generators\";\"Weibull distribution random number generator (positive values only).\";\"ares weibull ksigma, ktau\"\n"
"\"bbcutm\";\"Signal Generators:Sample Playback\";\"Generates breakbeat-style cut-ups of a mono audio stream.\";\"a1 bbcutm asource, ibps, isubdiv, ibarlength, iphrasebars, inumrepeats  [, istutterspeed] [, istutterchance] [, ienvchoice ]\"\n"
"\"bbcuts\";\"Signal Generators:Sample Playback\";\"Generates breakbeat-style cut-ups of a stereo audio stream.\";\"a1,a2 bbcuts asource1, asource2, ibps, isubdiv, ibarlength, iphrasebars,  inumrepeats [, istutterspeed] [, istutterchance] [, ienvchoice]\"\n"
"\"flooper\";\"Signal Generators:Sample Playback\";\"Function-table-based crossfading looper.\";\"asig flooper kamp, kpitch, istart, idur, ifad, ifn\"\n"
"\"flooper2\";\"Signal Generators:Sample Playback\";\"Function-table-based crossfading looper.\";\"asig flooper2 kamp, kpitch, kloopstart, kloopend, kcrossfade, ifn  [, istart, imode, ifenv, iskip]\"\n"
"\"fluidAllOut\";\"Signal Generators:Sample Playback\";\"Collects all audio from all Fluidsynth engines in a performance\";\"aleft, aright fluidAllOut\"\n"
"\"fluidCCi\";\"Signal Generators:Sample Playback\";\"Sends a MIDI controller data message to fluid.\";\"fluidCCi iEngineNumber, iChannelNumber, iControllerNumber, iValue\"\n"
"\"fluidCCk\";\"Signal Generators:Sample Playback\";\"Sends a MIDI controller data message to fluid.\";\"fluidCCk iEngineNumber, iChannelNumber, iControllerNumber, kValue\"\n"
"\"fluidControl\";\"Signal Generators:Sample Playback\";\"Sends MIDI note on, note off, and other messages to a SoundFont preset.\";\"fluidControl ienginenum, kstatus, kchannel, kdata1, kdata2\"\n"
"\"fluidEngine\";\"Signal Generators:Sample Playback\";\"Instantiates a fluidsynth engine.\";\"ienginenum fluidEngine [iReverbEnabled] [, iChorusEnabled] [,iNumChannels] [, iPolyphony]\"\n"
"\"fluidLoad\";\"Signal Generators:Sample Playback\";\"Loads a SoundFont into a fluidEngine, optionally listing SoundFont contents.\";\"isfnum fluidLoad soundfont, ienginenum[, ilistpresets]\"\n"
"\"fluidNote\";\"Signal Generators:Sample Playback\";\"Plays a note on a channel in a fluidSynth engine.\";\"fluidNote ienginenum, ichannelnum, imidikey, imidivel\"\n"
"\"fluidOut\";\"Signal Generators:Sample Playback\";\"Outputs sound from a given fluidEngine\";\"aleft, aright fluidOut ienginenum\"\n"
"\"fluidProgramSelect\";\"Signal Generators:Sample Playback\";\"Assigns a preset from a SoundFont to a channel on a fluidEngine.\";\"fluidProgramSelect ienginenum, ichannelnum, isfnum, ibanknum, ipresetnum\"\n"
"\"fluidSetInterpMethod\";\"Signal Generators:Sample Playback\";\"Set interpolation method for channel in Fluid Engine\";\"fluidSetInterpMethod ienginenum, ichannelnum, iInterpMethod\"\n"
"\"loscil\";\"Signal Generators:Sample Playback\";\"Read sampled sound from a table.\";\"ar1 [,ar2] loscil xamp, kcps, ifn [, ibas] [, imod1] [, ibeg1] [, iend1]  [, imod2] [, ibeg2] [, iend2]\"\n"
"\"loscil3\";\"Signal Generators:Sample Playback\";\"Read sampled sound from a table using cubic interpolation.\";\"ar1 [,ar2] loscil3 xamp, kcps, ifn [, ibas] [, imod1] [, ibeg1] [, iend1]  [, imod2] [, ibeg2] [, iend2]\"\n"
"\"loscilx\";\"Signal Generators:Sample Playback\";\"Loop oscillator.\";\"ar1 [, ar2, ar3, ar4, ar5, ar6, ar7, ar8, ar9, ar10, ar11, ar12, ar13, ar14,  ar15, ar16] loscilx xamp, kcps, ifn  [, iwsize, ibas, istrt, imod1, ibeg1, iend1]\"\n"
"\"lphasor\";\"Signal Generators:Sample Playback\";\"Generates a table index for sample playback\";\"ares lphasor xtrns [, ilps] [, ilpe] [, imode] [, istrt] [, istor]\"\n"
"\"lposcil\";\"Signal Generators:Sample Playback\";\"Read sampled sound from a table with looping and high precision.\";\"ares lposcil kamp, kfreqratio, kloop, kend, ifn [, iphs]\"\n"
"\"lposcil3\";\"Signal Generators:Sample Playback\";\"Read sampled sound from a table with high precision and cubic interpolation.\";\"ares lposcil3 kamp, kfreqratio, kloop, kend, ifn [, iphs]\"\n"
"\"lposcila\";\"Signal Generators:Sample Playback\";\"Read sampled sound from a table with looping and high precision.\";\"ar lposcila aamp, kfreqratio, kloop, kend, ift [,iphs]\"\n"
"\"lposcilsa\";\"Signal Generators:Sample Playback\";\"Read stereo sampled sound from a table with looping and high precision.\";\"ar1, ar2 lposcilsa aamp, kfreqratio, kloop, kend, ift [,iphs]\"\n"
"\"lposcilsa2\";\"Signal Generators:Sample Playback\";\"Read stereo sampled sound from a table with looping and high precision.\";\"ar1, ar2 lposcilsa2 aamp, kfreqratio, kloop, kend, ift [,iphs]\"\n"
"\"sfilist\";\"Signal Generators:Sample Playback\";\"Prints a list of all instruments of a previously loaded SoundFont2 (SF2) file.\";\"sfilist ifilhandle\"\n"
"\"sfinstr\";\"Signal Generators:Sample Playback\";\"Plays a SoundFont2 (SF2) sample instrument, generating a stereo sound.\";\"ar1, ar2 sfinstr ivel, inotenum, xamp, xfreq, instrnum, ifilhandle  [, iflag] [, ioffset]\"\n"
"\"sfinstr3\";\"Signal Generators:Sample Playback\";\"Plays a SoundFont2 (SF2) sample instrument, generating a stereo sound with cubic interpolation.\";\"ar1, ar2 sfinstr3 ivel, inotenum, xamp, xfreq, instrnum, ifilhandle  [, iflag] [, ioffset]\"\n"
"\"sfinstr3m\";\"Signal Generators:Sample Playback\";\"Plays a SoundFont2 (SF2) sample instrument, generating a mono sound with cubic interpolation.\";\"ares sfinstr3m ivel, inotenum, xamp, xfreq, instrnum, ifilhandle  [, iflag] [, ioffset]\"\n"
"\"sfinstrm\";\"Signal Generators:Sample Playback\";\"Plays a SoundFont2 (SF2) sample instrument, generating a mono sound.\";\"ares sfinstrm ivel, inotenum, xamp, xfreq, instrnum, ifilhandle  [, iflag] [, ioffset]\"\n"
"\"sfload\";\"Signal Generators:Sample Playback\";\"Loads an entire SoundFont2 (SF2) sample file into memory.\";\"ir sfload \"filename\"\"\n"
"\"sflooper\";\"Signal Generators:Sample Playback\";\"Plays a SoundFont2 (SF2) sample preset, generating a stereo sound, with user-defined time-varying crossfade looping.\";\"ar1, ar2 sflooper ivel, inotenum, kamp, kpitch, ipreindex, kloopstart, kloopend, kcrossfade  [, istart, imode, ifenv, iskip]\"\n"
"\"sfpassign\";\"Signal Generators:Sample Playback\";\"Assigns all presets of a SoundFont2 (SF2) sample file to a sequence of progressive index numbers.\";\"sfpassign istartindex, ifilhandle[, imsgs]\"\n"
"\"sfplay\";\"Signal Generators:Sample Playback\";\"Plays a SoundFont2 (SF2) sample preset, generating a stereo sound.\";\"ar1, ar2 sfplay ivel, inotenum, xamp, xfreq, ipreindex [, iflag] [, ioffset] [, ienv]\"\n"
"\"sfplay3\";\"Signal Generators:Sample Playback\";\"Plays a SoundFont2 (SF2) sample preset, generating a stereo sound with cubic interpolation.\";\"ar1, ar2 sfplay3 ivel, inotenum, xamp, xfreq, ipreindex [, iflag] [, ioffset] [, ienv]\"\n"
"\"sfplay3m\";\"Signal Generators:Sample Playback\";\"Plays a SoundFont2 (SF2) sample preset, generating a mono sound with cubic interpolation.\";\"ares sfplay3m ivel, inotenum, xamp, xfreq, ipreindex [, iflag] [, ioffset] [, ienv]\"\n"
"\"sfplaym\";\"Signal Generators:Sample Playback\";\"Plays a SoundFont2 (SF2) sample preset, generating a mono sound.\";\"ares sfplaym ivel, inotenum, xamp, xfreq, ipreindex [, iflag] [, ioffset] [, ienv]\"\n"
"\"sfplist\";\"Signal Generators:Sample Playback\";\"Prints a list of all presets of a SoundFont2 (SF2) sample file.\";\"sfplist ifilhandle\"\n"
"\"sfpreset\";\"Signal Generators:Sample Playback\";\"Assigns an existing preset of a SoundFont2 (SF2) sample file to an index number.\";\"ir sfpreset iprog, ibank, ifilhandle, ipreindex\"\n"
"\"sndloop\";\"Signal Generators:Sample Playback\";\"A sound looper with pitch control.\";\"asig, krec sndloop ain, kpitch, ktrig, idur, ifad\"\n"
"\"waveset\";\"Signal Generators:Sample Playback\";\"A simple time stretch by repeating cycles.\";\"ares waveset ain, krep [, ilen]\"\n"
"\"scanhammer\";\"Signal Generators:Scanned Synthesis\";\"Copies from one table to another with a gain control.\";\"scanhammer isrc, idst, ipos, imult\"\n"
"\"scans\";\"Signal Generators:Scanned Synthesis\";\"Generate audio output using scanned synthesis.\";\"ares scans kamp, kfreq, ifn, id [, iorder]\"\n"
"\"scantable\";\"Signal Generators:Scanned Synthesis\";\"A simpler scanned synthesis implementation.\";\"aout scantable kamp, kpch, ipos, imass, istiff, idamp, ivel\"\n"
"\"scanu\";\"Signal Generators:Scanned Synthesis\";\"Compute the waveform and the wavetable for use in scanned synthesis.\";\"scanu init, irate, ifnvel, ifnmass, ifnstif, ifncentr, ifndamp, kmass,  kstif, kcentr, kdamp, ileft, iright, kpos, kstrngth, ain, idisp, id\"\n"
"\"xscanmap\";\"Signal Generators:Scanned Synthesis\";\"Allows the position and velocity of a node in a scanned process to be read.\";\"kpos, kvel xscanmap iscan, kamp, kvamp [, iwhich]\"\n"
"\"xscans\";\"Signal Generators:Scanned Synthesis\";\"Fast scanned synthesis waveform and the wavetable generator.\";\"ares xscans kamp, kfreq, ifntraj, id [, iorder]\"\n"
"\"xscansmap\";\"Signal Generators:Scanned Synthesis\";\"Allows the position and velocity of a node in a scanned process to be read.\";\"xscansmap kpos, kvel, iscan, kamp, kvamp [, iwhich]\"\n"
"\"xscanu\";\"Signal Generators:Scanned Synthesis\";\"Compute the waveform and the wavetable for use in scanned synthesis.\";\"xscanu init, irate, ifnvel, ifnmass, ifnstif, ifncentr, ifndamp, kmass,  kstif, kcentr, kdamp, ileft, iright, kpos, kstrngth, ain, idisp, id\"\n"
"\"STKBandedWG\";\"Signal Generators:STK Opcodes\";\"STKBandedWG uses banded waveguide techniques to model a variety of sounds.\";\"asignal STKBandedWG ifrequency, iamplitude, [kc1, kv1[, kc2, kv2[, kc3, kv3[, kc4, kv4[, kc5, kv5[, kc6, kv6[, kc7, kv7]]]]]]]\"\n"
"\"STKBeeThree\";\"Signal Generators:STK Opcodes\";\"STK Hammond-oid organ-like FM synthesis instrument.\";\"asignal STKBeeThree ifrequency, iamplitude, [kc1, kv1[, kc2, kv2[, kc3, kv3[, kc4, kv4[, kc5, kv5]]]]]\"\n"
"\"STKBlowBotl\";\"Signal Generators:STK Opcodes\";\"STKBlowBotl uses a helmholtz resonator (biquad filter) with a polynomial jet excitation.\";\"asignal STKBlowBotl ifrequency, iamplitude, [kc1, kv1[, kc2, kv2[, kc3, kv3[, kc4, kv4]]]]\"\n"
"\"STKBlowHole\";\"Signal Generators:STK Opcodes\";\"STK clarinet physical model with one register hole and one tonehole.\";\"asignal STKBlowHole ifrequency, iamplitude, [kc1, kv1[, kc2, kv2[, kc3, kv3[, kc4, kv4[, kc5, kv5]]]]]\"\n"
"\"STKBowed\";\"Signal Generators:STK Opcodes\";\"STKBowed is a bowed string instrument.\";\"asignal STKBowed ifrequency, iamplitude, [kc1, kv1[, kc2, kv2[, kc3, kv3[, kc4, kv4[, kc5, kv5]]]]]\"\n"
"\"STKBrass\";\"Signal Generators:STK Opcodes\";\"STKBrass is a simple brass instrument.\";\"asignal STKBrass ifrequency, iamplitude, [kc1, kv1[, kc2, kv2[, kc3, kv3[, kc4, kv4[, kc5, kv5]]]]]\"\n"
"\"STKClarinet\";\"Signal Generators:STK Opcodes\";\"STKClarinet uses a simple clarinet physical model.\";\"asignal STKClarinet ifrequency, iamplitude, [kc1, kv1[, kc2, kv2[, kc3, kv3[, kc4, kv4[, kc5, kv5]]]]]\"\n"
"\"STKDrummer\";\"Signal Generators:STK Opcodes\";\"STKDrummer is a drum sampling synthesizer.\";\"asignal STKDrummer ifrequency, iamplitude\"\n"
"\"STKFMVoices\";\"Signal Generators:STK Opcodes\";\"STKFMVoices is a singing FM synthesis instrument.\";\"asignal STKFMVoices ifrequency, iamplitude, [kc1, kv1[, kc2, kv2[, kc3, kv3[, kc4, kv4[, kc5, kv5]]]]]\"\n"
"\"STKFlute\";\"Signal Generators:STK Opcodes\";\"STKFlute uses a simple flute physical model.\";\"asignal STKFlute ifrequency, iamplitude, [kc1, kv1[, kc2, kv2[, kc3, kv3[, kc4, kv4[, kc5, kv5]]]]]\"\n"
"\"STKHevyMetl\";\"Signal Generators:STK Opcodes\";\"STKHevyMetl produces metal sounds.\";\"asignal STKHevyMetl ifrequency, iamplitude, [kc1, kv1[, kc2, kv2[, kc3, kv3[, kc4, kv4[, kc5, kv5]]]]]\"\n"
"\"STKMandolin\";\"Signal Generators:STK Opcodes\";\"STKMandolin produces mamdolin-like sounds.\";\"asignal STKMandolin ifrequency, iamplitude, [kc1, kv1[, kc2, kv2[, kc3, kv3[, kc4, kv4[, kc5, kv5]]]]]\"\n"
"\"STKModalBar\";\"Signal Generators:STK Opcodes\";\"STKModalBar is a resonant bar instrument.\";\"asignal STKModalBar ifrequency, iamplitude, [kc1, kv1[, kc2, kv2[, kc3, kv3[, kc4, kv4[, kc5, kv5[, kc6, kv6[, kc7, kv7]]]]]]]\"\n"
"\"STKMoog\";\"Signal Generators:STK Opcodes\";\"STKMoog produces moog-like swept filter sounds.\";\"asignal STKMoog ifrequency, iamplitude, [kc1, kv1[, kc2, kv2[, kc3, kv3[, kc4, kv4[, kc5, kv5]]]]]\"\n"
"\"STKPercFlut\";\"Signal Generators:STK Opcodes\";\"STKPercFlut is a percussive flute FM synthesis instrument.\";\"asignal STKPercFlut ifrequency, iamplitude, [kc1, kv1[, kc2, kv2[, kc3, kv3[, kc4, kv4[, kc5, kv5]]]]]\"\n"
"\"STKPlucked\";\"Signal Generators:STK Opcodes\";\"STKPlucked uses a plucked string physical model.\";\"asignal STKPlucked ifrequency, iamplitude\"\n"
"\"STKResonate\";\"Signal Generators:STK Opcodes\";\"STKResonate is a noise driven formant filter.\";\"asignal STKResonate ifrequency, iamplitude, [kc1, kv1[, kc2, kv2[, kc3, kv3[, kc4, kv4[, kc5, kv5]]]]]\"\n"
"\"STKRhodey\";\"Signal Generators:STK Opcodes\";\"STK Fender Rhodes-like electric piano FM synthesis instrument.\";\"asignal STKRhodey ifrequency, iamplitude, [kc1, kv1[, kc2, kv2[, kc3, kv3[, kc4, kv4[, kc5, kv5]]]]]\"\n"
"\"STKSaxofony\";\"Signal Generators:STK Opcodes\";\"STKSaxofony is a faux conical bore reed instrument.\";\"asignal STKSaxofony ifrequency, iamplitude, [kc1, kv1[, kc2, kv2[, kc3, kv3[, kc4, kv4[, kc5, kv5[, kc6, kv6[, kc7, kv7]]]]]]]\"\n"
"\"STKShakers\";\"Signal Generators:STK Opcodes\";\"STKShakers is an instrument that simulates environmental sounds or collisions of multiple independent sound producing objects.\";\"asignal STKShakers ifrequency, iamplitude, [kc1, kv1[, kc2, kv2[, kc3, kv3[, kc4, kv4[, kc5, kv5[, kc6, kv6]]]]]]\"\n"
"\"STKSimple\";\"Signal Generators:STK Opcodes\";\"STKSimple is a wavetable/noise instrument.\";\"asignal STKSimple ifrequency, iamplitude, [kc1, kv1[, kc2, kv2[, kc3, kv3[, kc4, kv4]]]]\"\n"
"\"STKSitar\";\"Signal Generators:STK Opcodes\";\"STKSitar uses a plucked string physical model.\";\"asignal STKSitar ifrequency, iamplitude\"\n"
"\"STKStifKarp\";\"Signal Generators:STK Opcodes\";\"STKStifKarp is a plucked stiff string instrument.\";\"asignal STKStifKarp ifrequency, iamplitude, [kc1, kv1[, kc2, kv2[, kc3, kv3]]]\"\n"
"\"STKTubeBell\";\"Signal Generators:STK Opcodes\";\"STKTubeBell is a tubular bell (orchestral chime) FM synthesis instrument.\";\"asignal STKTubeBell ifrequency, iamplitude, [kc1, kv1[, kc2, kv2[, kc3, kv3[, kc4, kv4[, kc5, kv5]]]]]\"\n"
"\"STKVoicForm\";\"Signal Generators:STK Opcodes\";\"STKVoicForm is a four formant synthesis instrument.\";\"asignal STKVoicForm ifrequency, iamplitude, [kc1, kv1[, kc2, kv2[, kc3, kv3[, kc4, kv4[, kc5, kv5]]]]]\"\n"
"\"STKWhistle\";\"Signal Generators:STK Opcodes\";\"STKWhistle produces whistle sounds.\";\"asignal STKWhistle ifrequency, iamplitude, [kc1, kv1[, kc2, kv2[, kc3, kv3[, kc4, kv4[, kc5, kv5]]]]]\"\n"
"\"STKWurley\";\"Signal Generators:STK Opcodes\";\"STKWurley simulates a Wurlitzer electric piano FM synthesis instrument.\";\"asignal STKWurley ifrequency, iamplitude, [kc1, kv1[, kc2, kv2[, kc3, kv3[, kc4, kv4[, kc5, kv5]]]]]\"\n"
"\"oscil1\";\"Signal Generators:Table Access\";\"Accesses table values by incremental sampling.\";\"kres oscil1 idel, kamp, idur, ifn\"\n"
"\"oscil1i\";\"Signal Generators:Table Access\";\"Accesses table values by incremental sampling with linear interpolation.\";\"kres oscil1i idel, kamp, idur, ifn\"\n"
"\"ptable\";\"Signal Generators:Table Access\";\"Accesses table values by direct indexing.\";\"ares ptable andx, ifn [, ixmode] [, ixoff] [, iwrap]\"\n"
"\"ptable3\";\"Signal Generators:Table Access\";\"Accesses table values by direct indexing with cubic interpolation.\";\"ares ptable3 andx, ifn [, ixmode] [, ixoff] [, iwrap]\"\n"
"\"ptablei\";\"Signal Generators:Table Access\";\"Accesses table values by direct indexing with linear interpolation.\";\"ares ptablei andx, ifn [, ixmode] [, ixoff] [, iwrap]\"\n"
"\"tab_i\";\"Signal Generators:Table Access\";\"Fast table opcodes.\";\"ir tab_i indx, ifn[, ixmode]\"\n"
"\"tab\";\"Signal Generators:Table Access\";\"Fast table opcodes.\";\"kr tab kndx, ifn[, ixmode]\"\n"
"\"tabw_i\";\"Signal Generators:Table Access\";\"Fast table opcodes.\";\"tabw_i isig, indx, ifn [,ixmode]\"\n"
"\"tabw\";\"Signal Generators:Table Access\";\"Fast table opcodes.\";\"tabw ksig, kndx, ifn [,ixmode]\"\n"
"\"table\";\"Signal Generators:Table Access\";\"Accesses table values by direct indexing.\";\"ares table andx, ifn [, ixmode] [, ixoff] [, iwrap]\"\n"
"\"table3\";\"Signal Generators:Table Access\";\"Accesses table values by direct indexing with cubic interpolation.\";\"ares table3 andx, ifn [, ixmode] [, ixoff] [, iwrap]\"\n"
"\"tablei\";\"Signal Generators:Table Access\";\"Accesses table values by direct indexing with linear interpolation.\";\"ares tablei andx, ifn [, ixmode] [, ixoff] [, iwrap]\"\n"
"\"wterrain\";\"Signal Generators:Wave Terrain Synthesis\";\"A simple wave-terrain synthesis opcode.\";\"aout wterrain kamp, kpch, k_xcenter, k_ycenter, k_xradius, k_yradius,  itabx, itaby\"\n"
"\"pluck\";\"Signal Generators:Waveguide Physical Modeling\";\"Produces a naturally decaying plucked string or drum sound.\";\"ares pluck kamp, kcps, icps, ifn, imeth [, iparm1] [, iparm2]\"\n"
"\"repluck\";\"Signal Generators:Waveguide Physical Modeling\";\"Physical model of the plucked string.\";\"ares repluck iplk, kamp, icps, kpick, krefl, axcite\"\n"
"\"streson\";\"Signal Generators:Waveguide Physical Modeling\";\"A string resonator with variable fundamental frequency.\";\"ares streson asig, kfr, ifdbgain\"\n"
"\"wgbow\";\"Signal Generators:Waveguide Physical Modeling\";\"Creates a tone similar to a bowed string.\";\"ares wgbow kamp, kfreq, kpres, krat, kvibf, kvamp, ifn [, iminfreq]\"\n"
"\"wgbowedbar\";\"Signal Generators:Waveguide Physical Modeling\";\"A physical model of a bowed bar.\";\"ares wgbowedbar kamp, kfreq, kpos, kbowpres, kgain [, iconst] [, itvel]  [, ibowpos] [, ilow]\"\n"
"\"wgbrass\";\"Signal Generators:Waveguide Physical Modeling\";\"Creates a tone related to a brass instrument.\";\"ares wgbrass kamp, kfreq, ktens, iatt, kvibf, kvamp, ifn [, iminfreq]\"\n"
"\"wgclar\";\"Signal Generators:Waveguide Physical Modeling\";\"Creates a tone similar to a clarinet.\";\"ares wgclar kamp, kfreq, kstiff, iatt, idetk, kngain, kvibf, kvamp, ifn  [, iminfreq]\"\n"
"\"wgflute\";\"Signal Generators:Waveguide Physical Modeling\";\"Creates a tone similar to a flute.\";\"ares wgflute kamp, kfreq, kjet, iatt, idetk, kngain, kvibf, kvamp, ifn  [, iminfreq] [, ijetrf] [, iendrf]\"\n"
"\"wgpluck\";\"Signal Generators:Waveguide Physical Modeling\";\"A high fidelity simulation of a plucked string.\";\"ares wgpluck icps, iamp, kpick, iplk, idamp, ifilt, axcite\"\n"
"\"wgpluck2\";\"Signal Generators:Waveguide Physical Modeling\";\"Physical model of the plucked string.\";\"ares wgpluck2 iplk, kamp, icps, kpick, krefl\"\n"
"\"dumpk\";\"Signal I/O:File I/O\";\"Periodically writes an orchestra control-signal value to an external file.\";\"dumpk ksig, ifilname, iformat, iprd\"\n"
"\"dumpk2\";\"Signal I/O:File I/O\";\"Periodically writes two orchestra control-signal values to an external file.\";\"dumpk2 ksig1, ksig2, ifilname, iformat, iprd\"\n"
"\"dumpk3\";\"Signal I/O:File I/O\";\"Periodically writes three orchestra control-signal values to an external file.\";\"dumpk3 ksig1, ksig2, ksig3, ifilname, iformat, iprd\"\n"
"\"dumpk4\";\"Signal I/O:File I/O\";\"Periodically writes four orchestra control-signal values to an external file.\";\"dumpk4 ksig1, ksig2, ksig3, ksig4, ifilname, iformat, iprd\"\n"
"\"ficlose\";\"Signal I/O:File I/O\";\"Closes a previously opened file.\";\"ficlose ihandle\"\n"
"\"fin\";\"Signal I/O:File I/O\";\"Read signals from a file at a-rate.\";\"fin ifilename, iskipframes, iformat, ain1 [, ain2] [, ain3] [,...]\"\n"
"\"fini\";\"Signal I/O:File I/O\";\"Read signals from a file at i-rate.\";\"fini ifilename, iskipframes, iformat, in1 [, in2] [, in3] [, ...]\"\n"
"\"fink\";\"Signal I/O:File I/O\";\"Read signals from a file at k-rate.\";\"fink ifilename, iskipframes, iformat, kin1 [, kin2] [, kin3] [,...]\"\n"
"\"fiopen\";\"Signal I/O:File I/O\";\"Opens a file in a specific mode.\";\"ihandle fiopen ifilename, imode\"\n"
"\"fout\";\"Signal I/O:File I/O\";\"Outputs a-rate signals to an arbitrary number of channels.\";\"fout ifilename, iformat, aout1 [, aout2, aout3,...,aoutN]\"\n"
"\"fouti\";\"Signal I/O:File I/O\";\"Outputs i-rate signals of an arbitrary number of channels to a specified file.\";\"fouti ihandle, iformat, iflag, iout1 [, iout2, iout3,....,ioutN]\"\n"
"\"foutir\";\"Signal I/O:File I/O\";\"Outputs i-rate signals from an arbitrary number of channels to a specified file.\";\"foutir ihandle, iformat, iflag, iout1 [, iout2, iout3,....,ioutN]\"\n"
"\"foutk\";\"Signal I/O:File I/O\";\"Outputs k-rate signals of an arbitrary number of channels to a specified file, in raw (headerless) format.\";\"foutk ifilename, iformat, kout1 [, kout2, kout3,....,koutN]\"\n"
"\"fprintks\";\"Signal I/O:File I/O\";\"Similar to printks but prints to a file.\";\"fprintks \"filename\", \"string\", [, kval1] [, kval2] [...]\"\n"
"\"fprints\";\"Signal I/O:File I/O\";\"Similar to prints but prints to a file.\";\"fprints \"filename\", \"string\" [, ival1] [, ival2] [...]\"\n"
"\"readk\";\"Signal I/O:File I/O\";\"Periodically reads an orchestra control-signal value from an external file.\";\"kres readk ifilname, iformat, iprd\"\n"
"\"readk2\";\"Signal I/O:File I/O\";\"Periodically reads two orchestra control-signal values from an external file.\";\"kr1, kr2 readk2 ifilname, iformat, iprd\"\n"
"\"readk3\";\"Signal I/O:File I/O\";\"Periodically reads three orchestra control-signal values from an external file.\";\"kr1, kr2, kr3 readk3 ifilname, iformat, iprd\"\n"
"\"readk4\";\"Signal I/O:File I/O\";\"Periodically reads four orchestra control-signal values from an external file.\";\"kr1, kr2, kr3, kr4 readk4 ifilname, iformat, iprd\"\n"
"\"diskin\";\"Signal I/O:Signal Input\";\"Deprecated. Reads audio data from an external device or stream and can alter its pitch.\";\"ar1 [, ar2 [, ar3 [, ... arN]]] diskin ifilcod, kpitch [, iskiptim]  [, iwraparound] [, iformat] [, iskipinit]\"\n"
"\"diskin2\";\"Signal I/O:Signal Input\";\"Reads audio data from a file, and can alter its pitch using one of several available interpolation types, as well as convert the sample rate to match the orchestra sr setting.\";\"a1[, a2[, ... aN]] diskin2 ifilcod, kpitch[, iskiptim  [, iwrap[, iformat [, iwsize[, ibufsize[, iskipinit]]]]]]\"\n"
"\"in\";\"Signal I/O:Signal Input\";\"Reads mono audio data from an external device or stream.\";\"ar1 in\"\n"
"\"in32\";\"Signal I/O:Signal Input\";\"Reads a 32-channel audio signal from an external device or stream.\";\"ar1, ar2, ar3, ar4, ar5, ar6, ar7, ar8, ar9, ar10, ar11, ar12, ar13, ar14,  ar15, ar16, ar17, ar18, ar19, ar20, ar21, ar22, ar23, ar24, ar25, ar26,  ar27, ar28, ar29, ar30, ar31, ar32 in32\"\n"
"\"inch\";\"Signal I/O:Signal Input\";\"Reads from numbered channels in an external audio signal or stream.\";\"ain1[, ...] inch kchan1[,...]\"\n"
"\"inh\";\"Signal I/O:Signal Input\";\"Reads six-channel audio data from an external device or stream.\";\"ar1, ar2, ar3, ar4, ar5, ar6 inh\"\n"
"\"ino\";\"Signal I/O:Signal Input\";\"Reads eight-channel audio data from an external device or stream.\";\"ar1, ar2, ar3, ar4, ar5, ar6, ar7, ar8 ino\"\n"
"\"inq\";\"Signal I/O:Signal Input\";\"Reads quad audio data from an external device or stream.\";\"ar1, ar2, ar3, a4 inq\"\n"
"\"inrg\";\"Signal I/O:Signal Input\";\"Allow input from a range of adjacent audio channels from the audio input device\";\"inrg kstart, ain1 [,ain2, ain3, ..., ainN]\"\n"
"\"ins\";\"Signal I/O:Signal Input\";\"Reads stereo audio data from an external device or stream.\";\"ar1, ar2 ins\"\n"
"\"invalue\";\"Signal I/O:Signal Input\";\"Reads a k-rate signal from a user-defined channel.\";\"kvalue invalue \"channel name\"\"\n"
"\"inx\";\"Signal I/O:Signal Input\";\"Reads a 16-channel audio signal from an external device or stream.\";\"ar1, ar2, ar3, ar4, ar5, ar6, ar7, ar8, ar9, ar10, ar11, ar12,  ar13, ar14, ar15, ar16 inx\"\n"
"\"inz\";\"Signal I/O:Signal Input\";\"Reads multi-channel audio samples into a ZAK array from an external device or stream.\";\"inz ksig1\"\n"
"\"mp3in\";\"Signal I/O:Signal Input\";\"Reads stereo audio data from an external MP3 file.\";\"ar1, ar2 mp3in ifilcod[, iskptim, iformat, iskipinit, ibufsize]\"\n"
"\"soundin\";\"Signal I/O:Signal Input\";\"Reads audio data from an external device or stream.\";\"ar1[, ar2[, ar3[, ... a24]]] soundin ifilcod [, iskptim] [, iformat]  [, iskipinit] [, ibufsize]\"\n"
"\"mdelay\";\"Signal I/O:Signal Output\";\"A MIDI delay opcode.\";\"mdelay kstatus, kchan, kd1, kd2, kdelay\"\n"
"\"monitor\";\"Signal I/O:Signal Output\";\"Returns the audio spout frame.\";\"aout1 [,aout2 ... aoutX] monitor\"\n"
"\"out\";\"Signal I/O:Signal Output\";\"Writes mono audio data to an external device or stream.\";\"out asig\"\n"
"\"out32\";\"Signal I/O:Signal Output\";\"Writes 32-channel audio data to an external device or stream.\";\"out32 asig1, asig2, asig3, asig4, asig5, asig6, asig7, asig8, asig10,  asig11, asig12, asig13, asig14, asig15, asig16, asig17, asig18,  asig19, asig20, asig21, asig22, asig23, asig24, asig25, asig26,  asig27, asig28, asig29, asig30, asig31, asig32\"\n"
"\"outc\";\"Signal I/O:Signal Output\";\"Writes audio data with an arbitrary number of channels to an external device or stream.\";\"outc asig1 [, asig2] [...]\"\n"
"\"outch\";\"Signal I/O:Signal Output\";\"Writes multi-channel audio data, with user-controllable channels, to an external device or stream.\";\"outch kchan1, asig1 [, kchan2] [, asig2] [...]\"\n"
"\"outh\";\"Signal I/O:Signal Output\";\"Writes 6-channel audio data to an external device or stream.\";\"outh asig1, asig2, asig3, asig4, asig5, asig6\"\n"
"\"outo\";\"Signal I/O:Signal Output\";\"Writes 8-channel audio data to an external device or stream.\";\"outo asig1, asig2, asig3, asig4, asig5, asig6, asig7, asig8\"\n"
"\"outq\";\"Signal I/O:Signal Output\";\"Writes 4-channel audio data to an external device or stream.\";\"outq asig1, asig2, asig3, asig4\"\n"
"\"outq1\";\"Signal I/O:Signal Output\";\"Writes samples to quad channel 1 of an external device or stream.\";\"outq1 asig\"\n"
"\"outq2\";\"Signal I/O:Signal Output\";\"Writes samples to quad channel 2 of an external device or stream.\";\"outq2 asig\"\n"
"\"outq3\";\"Signal I/O:Signal Output\";\"Writes samples to quad channel 3 of an external device or stream.\";\"outq3 asig\"\n"
"\"outq4\";\"Signal I/O:Signal Output\";\"Writes samples to quad channel 4 of an external device or stream.\";\"outq4 asig\"\n"
"\"outrg\";\"Signal I/O:Signal Output\";\"Allow output to a range of adjacent audio channels on the audio output device\";\"outrg kstart, aout1 [,aout2, aout3, ..., aoutN]\"\n"
"\"outs\";\"Signal I/O:Signal Output\";\"Writes stereo audio data to an external device or stream.\";\"outs asig1, asig2\"\n"
"\"outs1\";\"Signal I/O:Signal Output\";\"Writes samples to stereo channel 1 of an external device or stream.\";\"outs1 asig\"\n"
"\"outs2\";\"Signal I/O:Signal Output\";\"Writes samples to stereo channel 2 of an external device or stream.\";\"outs2 asig\"\n"
"\"outvalue\";\"Signal I/O:Signal Output\";\"Sends a k-rate signal or string to a user-defined channel.\";\"outvalue \"channel name\", kvalue\"\n"
"\"outx\";\"Signal I/O:Signal Output\";\"Writes 16-channel audio data to an external device or stream.\";\"outx asig1, asig2, asig3, asig4, asig5, asig6, asig7, asig8,  asig9, asig10, asig11, asig12, asig13, asig14, asig15, asig16\"\n"
"\"outz\";\"Signal I/O:Signal Output\";\"Writes multi-channel audio data from a ZAK array to an external device or stream.\";\"outz ksig1\"\n"
"\"soundout\";\"Signal I/O:Signal Output\";\"Deprecated. Writes audio output to a disk file.\";\"soundout asig1, ifilcod [, iformat]\"\n"
"\"soundouts\";\"Signal I/O:Signal Output\";\"Deprecated. Writes audio output to a disk file.\";\"soundouts asigl, asigr, ifilcod [, iformat]\"\n"
"\"chani\";\"Signal I/O:Software Bus\";\"Reads data from the software bus\";\"kval chani kchan\"\n"
"\"chano\";\"Signal I/O:Software Bus\";\"Send data to the outwards software bus\";\"chano kval, kchan\"\n"
"\"chn_k\";\"Signal I/O:Software Bus\";\"Declare a channel of the named software bus.\";\"chn_k Sname, imode[, itype, idflt, imin, imax]\"\n"
"\"chn_a\";\"Signal I/O:Software Bus\";\"Declare a channel of the named software bus.\";\"chn_a Sname, imode\"\n"
"\"chn_S\";\"Signal I/O:Software Bus\";\"Declare a channel of the named software bus.\";\"chn_S Sname, imode\"\n"
"\"chnclear\";\"Signal I/O:Software Bus\";\"Clears an audio output channel of the named software bus.\";\"chnclear Sname\"\n"
"\"chnexport\";\"Signal I/O:Software Bus\";\"Export a global variable as a channel of the bus.\";\"gival chnexport Sname, imode[, itype, idflt, imin, imax]\"\n"
"\"chnget\";\"Signal I/O:Software Bus\";\"Reads data from the software bus.\";\"ival chnget Sname\"\n"
"\"chnmix\";\"Signal I/O:Software Bus\";\"Writes audio data to the named software bus, mixing to the previous output.\";\"chnmix aval, Sname\"\n"
"\"chnparams\";\"Signal I/O:Software Bus\";\"Query parameters of a channel.\";\"itype, imode, ictltype, idflt, imin, imax chnparams\"\n"
"\"chnrecv\";\"Signal I/O:Software Bus\";\"Recieves data from the software bus.\";\"ival chnrecv Sname\"\n"
"\"chnsend\";\"Signal I/O:Software Bus\";\"Sends data via the named software bus.\";\"chnsend ival, Sname\"\n"
"\"chnset\";\"Signal I/O:Software Bus\";\"Writes data to the named software bus.\";\"chnset ival, Sname\"\n"
"\"setksmps\";\"Signal I/O:Software Bus\";\"Sets the local ksmps value in a user-defined opcode block.\";\"setksmps iksmps\"\n"
"\"xin\";\"Signal I/O:Software Bus\";\"Passes variables to a user-defined opcode block,\";\"xinarg1 [, xinarg2] ... [xinargN] xin\"\n"
"\"xout\";\"Signal I/O:Software Bus\";\"Retrieves variables from a user-defined opcode block,\";\"xout xoutarg1 [, xoutarg2] ... [, xoutargN]\"\n"
"\"dispfft\";\"Signal I/O:Printing and Display\";\"Displays the Fourier Transform of an audio or control signal.\";\"dispfft xsig, iprd, iwsiz [, iwtyp] [, idbout] [, iwtflg]\"\n"
"\"display\";\"Signal I/O:Printing and Display\";\"Displays the audio or control signals as an amplitude vs. time graph.\";\"display xsig, iprd [, inprds] [, iwtflg]\"\n"
"\"flashtxt\";\"Signal I/O:Printing and Display\";\"Allows text to be displayed from instruments like sliders\";\"flashtxt iwhich, String\"\n"
"\"print\";\"Signal I/O:Printing and Display\";\"Displays the values init (i-rate) variables.\";\"print iarg [, iarg1] [, iarg2] [...]\"\n"
"\"printf_i\";\"Signal I/O:Printing and Display\";\"printf-style formatted output\";\"printf_i Sfmt, itrig, [iarg1[, iarg2[, ... ]]]\"\n"
"\"printf\";\"Signal I/O:Printing and Display\";\"printf-style formatted output\";\"printf Sfmt, ktrig, [xarg1[, xarg2[, ... ]]]\"\n"
"\"printk\";\"Signal I/O:Printing and Display\";\"Prints one k-rate value at specified intervals.\";\"printk itime, kval [, ispace]\"\n"
"\"printk2\";\"Signal I/O:Printing and Display\";\"Prints a new value every time a control variable changes.\";\"printk2 kvar [, inumspaces]\"\n"
"\"printks\";\"Signal I/O:Printing and Display\";\"Prints at k-rate using a printf() style syntax.\";\"printks \"string\", itime [, kval1] [, kval2] [...]\"\n"
"\"prints\";\"Signal I/O:Printing and Display\";\"Prints at init-time using a printf() style syntax.\";\"prints \"string\" [, kval1] [, kval2] [...]\"\n"
"\"filebit\";\"Signal I/O:Soundfile Queries\";\"Returns the number of bits in each sample in a sound file.\";\"ir filebit ifilcod [, iallowraw]\"\n"
"\"filelen\";\"Signal I/O:Soundfile Queries\";\"Returns the length of a sound file.\";\"ir filelen ifilcod, [iallowraw]\"\n"
"\"filenchnls\";\"Signal I/O:Soundfile Queries\";\"Returns the number of channels in a sound file.\";\"ir filenchnls ifilcod [, iallowraw]\"\n"
"\"filepeak\";\"Signal I/O:Soundfile Queries\";\"Returns the peak absolute value of a sound file.\";\"ir filepeak ifilcod [, ichnl]\"\n"
"\"filesr\";\"Signal I/O:Soundfile Queries\";\"Returns the sample rate of a sound file.\";\"ir filesr ifilcod [, iallowraw]\"\n"
"\"filevalid\";\"Signal I/O:Soundfile Queries\";\"Checks that a file can be used.\";\"ir filevalid ifilcod\"\n"
"\"mp3len\";\"Signal I/O:Soundfile Queries\";\"Returns the length of an MP3 sound file.\";\"ir mp3len ifilcod\"\n"
"\"balance\";\"Signal Modifiers:Amplitude Modifiers\";\"Adjust one audio signal according to the values of another.\";\"ares balance asig, acomp [, ihp] [, iskip]\"\n"
"\"clip\";\"Signal Modifiers:Amplitude Modifiers\";\"Clips a signal to a predefined limit.\";\"ares clip asig, imeth, ilimit [, iarg]\"\n"
"\"compress\";\"Signal Modifiers:Amplitude Modifiers\";\"Compress, limit, expand, duck or gate an audio signal.\";\"ar compress aasig, acsig, kthresh, kloknee, khiknee, kratio, katt, krel, ilook\"\n"
"\"dam\";\"Signal Modifiers:Amplitude Modifiers\";\"A dynamic compressor/expander.\";\"ares dam asig, kthreshold, icomp1, icomp2, irtime, iftime\"\n"
"\"gain\";\"Signal Modifiers:Amplitude Modifiers\";\"Adjusts the amplitude audio signal according to a root-mean-square value.\";\"ares gain asig, krms [, ihp] [, iskip]\"\n"
"\"convolve\";\"Signal Modifiers:Convolution and Morphing\";\"Convolves a signal and an impulse response.\";\"ar1 [, ar2] [, ar3] [, ar4] convolve ain, ifilcod [, ichannel]\"\n"
"\"cross2\";\"Signal Modifiers:Convolution and Morphing\";\"Cross synthesis using FFT's.\";\"ares cross2 ain1, ain2, isize, ioverlap, iwin, kbias\"\n"
"\"dconv\";\"Signal Modifiers:Convolution and Morphing\";\"A direct convolution opcode.\";\"ares dconv asig, isize, ifn\"\n"
"\"ftconv\";\"Signal Modifiers:Convolution and Morphing\";\"Low latency multichannel convolution, using a function table as impulse response source.\";\"a1[, a2[, a3[, ... a8]]] ftconv ain, ift, iplen[, iskipsamples  [, iirlen[, iskipinit]]]\"\n"
"\"ftmorf\";\"Signal Modifiers:Convolution and Morphing\";\"Morphs between multiple ftables as specified in a list.\";\"ftmorf kftndx, iftfn, iresfn\"\n"
"\"pconvolve\";\"Signal Modifiers:Convolution and Morphing\";\"Convolution based on a uniformly partitioned overlap-save algorithm\";\"ar1 [, ar2] [, ar3] [, ar4] pconvolve ain, ifilcod [, ipartitionsize, ichannel]\"\n"
"\"delay\";\"Signal Modifiers:Delay\";\"Delays an input signal by some time interval.\";\"ares delay asig, idlt [, iskip]\"\n"
"\"delay1\";\"Signal Modifiers:Delay\";\"Delays an input signal by one sample.\";\"ares delay1 asig [, iskip]\"\n"
"\"delayk \";\"Signal Modifiers:Delay\";\"Delays an input signal by some time interval.\";\"kr delayk ksig, idel[, imode]\"\n"
"\"vdel_k \";\"Signal Modifiers:Delay\";\"Delays an input signal by some time interval.\";\"kr vdel_k ksig, kdel, imdel[, imode]\"\n"
"\"delayr\";\"Signal Modifiers:Delay\";\"Reads from an automatically established digital delay line.\";\"ares delayr idlt [, iskip]\"\n"
"\"delayw\";\"Signal Modifiers:Delay\";\"Writes the audio signal to a digital delay line.\";\"delayw asig\"\n"
"\"deltap\";\"Signal Modifiers:Delay\";\"Taps a delay line at variable offset times.\";\"ares deltap kdlt\"\n"
"\"deltap3\";\"Signal Modifiers:Delay\";\"Taps a delay line at variable offset times, uses cubic interpolation.\";\"ares deltap3 xdlt\"\n"
"\"deltapi\";\"Signal Modifiers:Delay\";\"Taps a delay line at variable offset times, uses interpolation.\";\"ares deltapi xdlt\"\n"
"\"deltapn\";\"Signal Modifiers:Delay\";\"Taps a delay line at variable offset times.\";\"ares deltapn xnumsamps\"\n"
"\"deltapx\";\"Signal Modifiers:Delay\";\"Read from or write to a delay line with interpolation.\";\"aout deltapx adel, iwsize\"\n"
"\"deltapxw\";\"Signal Modifiers:Delay\";\"Mixes the input signal to a delay line.\";\"deltapxw ain, adel, iwsize\"\n"
"\"multitap\";\"Signal Modifiers:Delay\";\"Multitap delay line implementation.\";\"ares multitap asig [, itime1, igain1] [, itime2, igain2] [...]\"\n"
"\"newopcodename\";\"Signal Modifiers:Delay\";\"Short description. Single line for opcode listing.\";\"outarg1, outarg2 newopcodename inarg1, inarg2\"\n"
"\"vdelay\";\"Signal Modifiers:Delay\";\"An interpolating variable time delay.\";\"ares vdelay asig, adel, imaxdel [, iskip]\"\n"
"\"vdelay3\";\"Signal Modifiers:Delay\";\"A variable time delay with cubic interpolation.\";\"ares vdelay3 asig, adel, imaxdel [, iskip]\"\n"
"\"vdelayx\";\"Signal Modifiers:Delay\";\"A variable delay opcode with high quality interpolation.\";\"aout vdelayx ain, adl, imd, iws [, ist]\"\n"
"\"vdelayxq\";\"Signal Modifiers:Delay\";\"A 4-channel variable delay opcode with high quality interpolation.\";\"aout1, aout2, aout3, aout4 vdelayxq ain1, ain2, ain3, ain4, adl, imd, iws [, ist]\"\n"
"\"vdelayxs\";\"Signal Modifiers:Delay\";\"A stereo variable delay opcode with high quality interpolation.\";\"aout1, aout2 vdelayxs ain1, ain2, adl, imd, iws [, ist]\"\n"
"\"vdelayxw\";\"Signal Modifiers:Delay\";\"Variable delay opcodes with high quality interpolation.\";\"aout vdelayxw ain, adl, imd, iws [, ist]\"\n"
"\"vdelayxwq\";\"Signal Modifiers:Delay\";\"Variable delay opcodes with high quality interpolation.\";\"aout1, aout2, aout3, aout4 vdelayxwq ain1, ain2, ain3, ain4, adl,  imd, iws [, ist]\"\n"
"\"vdelayxws\";\"Signal Modifiers:Delay\";\"Variable delay opcodes with high quality interpolation.\";\"aout1, aout2 vdelayxws ain1, ain2, adl, imd, iws [, ist]\"\n"
"\"bformdec\";\"Signal Modifiers:Panning and Spatialization\";\"Deprecated. Decodes an ambisonic B format signal.\";\"ao1, ao2 bformdec isetup, aw, ax, ay, az [, ar, as, at, au, av  [, abk, al, am, an, ao, ap, aq]]\"\n"
"\"bformdec1\";\"Signal Modifiers:Panning and Spatialization\";\"Decodes an ambisonic B format signal\";\"ao1, ao2 bformdec1 isetup, aw, ax, ay, az [, ar, as, at, au, av  [, abk, al, am, an, ao, ap, aq]]\"\n"
"\"bformenc\";\"Signal Modifiers:Panning and Spatialization\";\"Deprecated. Codes a signal into the ambisonic B format.\";\"aw, ax, ay, az bformenc asig, kalpha, kbeta, kord0, kord1\"\n"
"\"bformenc1\";\"Signal Modifiers:Panning and Spatialization\";\"Codes a signal into the ambisonic B format.\";\"aw, ax, ay, az bformenc1 asig, kalpha, kbeta\"\n"
"\"hrtfearly\";\"Signal Modifiers:Panning and Spatialization\";\"Generates 3D binaural audio with high-fidelity early reflections in a parametric room using a Phase Truncation algorithm.\";\"aleft, aright, irt60low, irt60high, imfp hrtfearly asrc, ksrcx, ksrcy, ksrcz, klstnrx, klstnry, klstnrz,  ifilel, ifiler, idefroom [,ifade, isr, iorder, ithreed, kheadrot, iroomx, iroomy, iroomz, iwallhigh,  iwalllow, iwallgain1, iwallgain2, iwallgain3, ifloorhigh, ifloorlow, ifloorgain1, ifloorgain2,  ifloorgain3, iceilinghigh, iceilinglow, iceilinggain1, iceilinggain2, iceilinggain3]\"\n"
"\"hrtfer\";\"Signal Modifiers:Panning and Spatialization\";\"Creates 3D audio for two speakers.\";\"aleft, aright hrtfer asig, kaz, kelev, HRTFcompact\"\n"
"\"hrtfmove\";\"Signal Modifiers:Panning and Spatialization\";\"Generates dynamic 3d binaural audio for headphones using magnitude interpolation and phase truncation.\";\"aleft, aright hrtfmove asrc, kAz, kElev, ifilel, ifiler [, imode, ifade, isr]\"\n"
"\"hrtfmove2\";\"Signal Modifiers:Panning and Spatialization\";\"Generates dynamic 3d binaural audio for headphones using a Woodworth based spherical head model with improved low frequency phase accuracy.\";\"aleft, aright hrtfmove2 asrc, kAz, kElev, ifilel, ifiler [,ioverlap, iradius, isr]\"\n"
"\"hrtfstat\";\"Signal Modifiers:Panning and Spatialization\";\"Generates static 3d binaural audio for headphones using a Woodworth based spherical head model with improved low frequency phase accuracy.\";\"aleft, aright hrtfstat asrc, iAz, iElev, ifilel, ifiler [,iradius, isr]\"\n"
"\"locsend\";\"Signal Modifiers:Panning and Spatialization\";\"Distributes the audio signals of a previous\";\"a1, a2 locsend\"\n"
"\"locsig\";\"Signal Modifiers:Panning and Spatialization\";\"Takes an input signal and distributes between 2 or 4 channels.\";\"a1, a2 locsig asig, kdegree, kdistance, kreverbsend\"\n"
"\"pan\";\"Signal Modifiers:Panning and Spatialization\";\"Distribute an audio signal amongst four channels.\";\"a1, a2, a3, a4 pan asig, kx, ky, ifn [, imode] [, ioffset]\"\n"
"\"pan2\";\"Signal Modifiers:Panning and Spatialization\";\"Distribute an audio signal across two channels.\";\"a1, a2 pan2 asig, xp [, imode]\"\n"
"\"space\";\"Signal Modifiers:Panning and Spatialization\";\"Distributes an input signal among 4 channels using cartesian coordinates.\";\"a1, a2, a3, a4 space asig, ifn, ktime, kreverbsend, kx, ky\"\n"
"\"spat3d\";\"Signal Modifiers:Panning and Spatialization\";\"Positions the input sound in a 3D space and allows moving the sound at k-rate.\";\"aW, aX, aY, aZ spat3d ain, kX, kY, kZ, idist, ift, imode, imdel, iovr [, istor]\"\n"
"\"spat3di\";\"Signal Modifiers:Panning and Spatialization\";\"Positions the input sound in a 3D space with the sound source position set at i-time.\";\"aW, aX, aY, aZ spat3di ain, iX, iY, iZ, idist, ift, imode [, istor]\"\n"
"\"spat3dt\";\"Signal Modifiers:Panning and Spatialization\";\"Can be used to render an impulse response for a 3D space at i-time.\";\"spat3dt ioutft, iX, iY, iZ, idist, ift, imode, irlen [, iftnocl]\"\n"
"\"spdist\";\"Signal Modifiers:Panning and Spatialization\";\"Calculates distance values from xy coordinates.\";\"k1 spdist ifn, ktime, kx, ky\"\n"
"\"spsend\";\"Signal Modifiers:Panning and Spatialization\";\"Generates output signals based on a previously defined\";\"a1, a2, a3, a4 spsend\"\n"
"\"vbap16\";\"Signal Modifiers:Panning and Spatialization\";\"Distributes an audio signal among 16 channels.\";\"ar1, ..., ar16 vbap16 asig, kazim [, kelev] [, kspread]\"\n"
"\"vbap16move\";\"Signal Modifiers:Panning and Spatialization\";\"Distribute an audio signal among 16 channels with moving virtual sources.\";\"ar1, ..., ar16 vbap16move asig, idur, ispread, ifldnum, ifld1  [, ifld2] [...]\"\n"
"\"vbap4\";\"Signal Modifiers:Panning and Spatialization\";\"Distributes an audio signal among 4 channels.\";\"ar1, ar2, ar3, ar4 vbap4 asig, kazim [, kelev] [, kspread]\"\n"
"\"vbap4move\";\"Signal Modifiers:Panning and Spatialization\";\"Distributes an audio signal among 4 channels with moving virtual sources.\";\"ar1, ar2, ar3, ar4 vbap4move asig, idur, ispread, ifldnum, ifld1  [, ifld2] [...]\"\n"
"\"vbap8\";\"Signal Modifiers:Panning and Spatialization\";\"Distributes an audio signal among 8 channels.\";\"ar1, ..., ar8 vbap8 asig, kazim [, kelev] [, kspread]\"\n"
"\"vbap8move\";\"Signal Modifiers:Panning and Spatialization\";\"Distributes an audio signal among 8 channels with moving virtual sources.\";\"ar1, ..., ar8 vbap8move asig, idur, ispread, ifldnum, ifld1  [, ifld2] [...]\"\n"
"\"vbaplsinit\";\"Signal Modifiers:Panning and Spatialization\";\"Configures VBAP output according to loudspeaker parameters.\";\"vbaplsinit idim, ilsnum [, idir1] [, idir2] [...] [, idir32]\"\n"
"\"vbapz\";\"Signal Modifiers:Panning and Spatialization\";\"Writes a multi-channel audio signal to a ZAK array.\";\"vbapz inumchnls, istartndx, asig, kazim [, kelev] [, kspread]\"\n"
"\"vbapzmove\";\"Signal Modifiers:Panning and Spatialization\";\"Writes a multi-channel audio signal to a ZAK array with moving virtual sources.\";\"vbapzmove inumchnls, istartndx, asig, idur, ispread, ifldnum, ifld1,  ifld2, [...]\"\n"
"\"alpass\";\"Signal Modifiers:Reverberation\";\"Reverberates an input signal with a flat frequency response.\";\"ares alpass asig, krvt, ilpt [, iskip] [, insmps]\"\n"
"\"babo\";\"Signal Modifiers:Reverberation\";\"A physical model reverberator.\";\"a1, a2 babo asig, ksrcx, ksrcy, ksrcz, irx, iry, irz [, idiff] [, ifno]\"\n"
"\"comb\";\"Signal Modifiers:Reverberation\";\"Reverberates an input signal with a\";\"ares comb asig, krvt, ilpt [, iskip] [, insmps]\"\n"
"\"freeverb\";\"Signal Modifiers:Reverberation\";\"Opcode version of Jezar's Freeverb\";\"aoutL, aoutR freeverb ainL, ainR, kRoomSize, kHFDamp[, iSRate[, iSkip]]\"\n"
"\"nestedap\";\"Signal Modifiers:Reverberation\";\"Three different nested all-pass filters.\";\"ares nestedap asig, imode, imaxdel, idel1, igain1 [, idel2] [, igain2]  [, idel3] [, igain3] [, istor]\"\n"
"\"nreverb\";\"Signal Modifiers:Reverberation\";\"A reverberator consisting of 6 parallel comb-lowpass filters.\";\"ares nreverb asig, ktime, khdif [, iskip] [,inumCombs] [, ifnCombs]  [, inumAlpas] [, ifnAlpas]\"\n"
"\"reverb\";\"Signal Modifiers:Reverberation\";\"Reverberates an input signal with a\";\"ares reverb asig, krvt [, iskip]\"\n"
"\"reverb2\";\"Signal Modifiers:Reverberation\";\"Same as the nreverb opcode.\";\"ares reverb2 asig, ktime, khdif [, iskip] [,inumCombs]  [, ifnCombs] [, inumAlpas] [, ifnAlpas]\"\n"
"\"reverbsc\";\"Signal Modifiers:Reverberation\";\"8 delay line stereo FDN reverb, based on work by Sean Costello\";\"aoutL, aoutR reverbsc ainL, ainR, kfblvl, kfco[, israte[, ipitchm[, iskip]]]\"\n"
"\"valpass\";\"Signal Modifiers:Reverberation\";\"Variably reverberates an input signal with a flat frequency response.\";\"ares valpass asig, krvt, xlpt, imaxlpt [, iskip] [, insmps]\"\n"
"\"vcomb\";\"Signal Modifiers:Reverberation\";\"Variably reverberates an input signal with a\";\"ares vcomb asig, krvt, xlpt, imaxlpt [, iskip] [, insmps]\"\n"
"\"denorm\";\"Signal Modifiers:Sample Level Operators\";\"Mixes low level noise to a list of a-rate signals\";\"denorm a1[, a2[, a3[, ... ]]]\"\n"
"\"diff\";\"Signal Modifiers:Sample Level Operators\";\"Modify a signal by differentiation.\";\"ares diff asig [, iskip]\"\n"
"\"downsamp\";\"Signal Modifiers:Sample Level Operators\";\"Modify a signal by down-sampling.\";\"kres downsamp asig [, iwlen]\"\n"
"\"fold\";\"Signal Modifiers:Sample Level Operators\";\"Adds artificial foldover to an audio signal.\";\"ares fold asig, kincr\"\n"
"\"integ\";\"Signal Modifiers:Sample Level Operators\";\"Modify a signal by integration.\";\"ares integ asig [, iskip]\"\n"
"\"interp\";\"Signal Modifiers:Sample Level Operators\";\"Converts a control signal to an audio signal using linear interpolation.\";\"ares interp ksig [, iskip] [, imode]\"\n"
"\"ntrpol\";\"Signal Modifiers:Sample Level Operators\";\"Calculates the weighted mean value of two input signals.\";\"ares ntrpol asig1, asig2, kpoint [, imin] [, imax]\"\n"
"\"a\";\"Signal Modifiers:Sample Level Operators\";\"Converts a k-rate parameter to an a-rate value with interpolation.\";\"a(x) (control-rate args only)\"\n"
"\"i\";\"Signal Modifiers:Sample Level Operators\";\"Returns an init-type equivalent of a k-rate argument, or directly returns an i-rate argument.\";\"i(x) (control-rate or init-rate arg)\"\n"
"\"k\";\"Signal Modifiers:Sample Level Operators\";\"Converts a i-rate parameter to an k-rate value.\";\"k(x) (i-rate args only)\"\n"
"\"samphold\";\"Signal Modifiers:Sample Level Operators\";\"Performs a sample-and-hold operation on its input.\";\"ares samphold asig, agate [, ival] [, ivstor]\"\n"
"\"upsamp\";\"Signal Modifiers:Sample Level Operators\";\"Modify a signal by up-sampling.\";\"ares upsamp ksig\"\n"
"\"vaget\";\"Signal Modifiers:Sample Level Operators\";\"Access values of the current buffer of an a-rate variable by indexing.\";\"kval vaget kndx, avar\"\n"
"\"vaset\";\"Signal Modifiers:Sample Level Operators\";\"Write value of into the current buffer of an a-rate variable by index.\";\"vaset kval, kndx, avar\"\n"
"\"limit\";\"Signal Modifiers:Signal Limiters\";\"Sets the lower and upper limits of the value it processes.\";\"ares limit asig, klow, khigh\"\n"
"\"mirror\";\"Signal Modifiers:Signal Limiters\";\"Reflects the signal that exceeds the low and high thresholds.\";\"ares mirror asig, klow, khigh\"\n"
"\"wrap\";\"Signal Modifiers:Signal Limiters\";\"Wraps-around the signal that exceeds the low and high thresholds.\";\"ares wrap asig, klow, khigh\"\n"
"\"distort\";\"Signal Modifiers:Special Effects\";\"Distort an audio signal via waveshaping and optional clipping.\";\"ar distort asig, kdist, ifn[, ihp, istor]\"\n"
"\"distort1\";\"Signal Modifiers:Special Effects\";\"Modified hyperbolic tangent distortion.\";\"ares distort1 asig, kpregain, kpostgain, kshape1, kshape2[, imode]\"\n"
"\"flanger\";\"Signal Modifiers:Special Effects\";\"A user controlled flanger.\";\"ares flanger asig, adel, kfeedback [, imaxd]\"\n"
"\"harmon\";\"Signal Modifiers:Special Effects\";\"Analyze an audio input and generate harmonizing voices in synchrony.\";\"ares harmon asig, kestfrq, kmaxvar, kgenfreq1, kgenfreq2, imode,  iminfrq, iprd\"\n"
"\"harmon2\";\"Signal Modifiers:Special Effects\";\"Analyze an audio input and generate harmonizing voices in synchrony with formants preserved.\";\"ares harmon2 asig, koct, kfrq1, kfrq2, icpsmode, ilowest[, ipolarity]\"\n"
"\"harmon3\";\"Signal Modifiers:Special Effects\";\"Analyze an audio input and generate harmonizing voices in synchrony with formants preserved.\";\"ares harmon3 asig, koct, kfrq1,  kfrq2, kfrq3, icpsmode, ilowest[, ipolarity]\"\n"
"\"harmon4\";\"Signal Modifiers:Special Effects\";\"Analyze an audio input and generate harmonizing voices in synchrony with formants preserved.\";\"ares harmon4 asig, koct, kfrq1,  kfrq2, kfrq3, kfrq4, icpsmode, ilowest[, ipolarity]\"\n"
"\"phaser1\";\"Signal Modifiers:Special Effects\";\"First-order allpass filters arranged in a series.\";\"ares phaser1 asig, kfreq, kord, kfeedback [, iskip]\"\n"
"\"phaser2\";\"Signal Modifiers:Special Effects\";\"Second-order allpass filters arranged in a series.\";\"ares phaser2 asig, kfreq, kq, kord, kmode, ksep, kfeedback\"\n"
"\"atone\";\"Signal Modifiers:Standard Filters\";\"A hi-pass filter whose transfer functions are the complements of the\";\"ares atone asig, khp [, iskip]\"\n"
"\"atonex\";\"Signal Modifiers:Standard Filters\";\"Emulates a stack of filters using the atone opcode.\";\"ares atonex asig, khp [, inumlayer] [, iskip]\"\n"
"\"biquad\";\"Signal Modifiers:Standard Filters\";\"A sweepable general purpose biquadratic digital filter.\";\"ares biquad asig, kb0, kb1, kb2, ka0, ka1, ka2 [, iskip]\"\n"
"\"biquada\";\"Signal Modifiers:Standard Filters\";\"A sweepable general purpose biquadratic digital filter with a-rate parameters.\";\"ares biquada asig, ab0, ab1, ab2, aa0, aa1, aa2 [, iskip]\"\n"
"\"butbp\";\"Signal Modifiers:Standard Filters\";\"Same as the butterbp opcode.\";\"ares butbp asig, kfreq, kband [, iskip]\"\n"
"\"butbr\";\"Signal Modifiers:Standard Filters\";\"Same as the butterbr opcode.\";\"ares butbr asig, kfreq, kband [, iskip]\"\n"
"\"buthp\";\"Signal Modifiers:Standard Filters\";\"Same as the butterhp opcode.\";\"ares buthp asig, kfreq [, iskip]\"\n"
"\"butlp\";\"Signal Modifiers:Standard Filters\";\"Same as the butterlp opcode.\";\"ares butlp asig, kfreq [, iskip]\"\n"
"\"butterbp\";\"Signal Modifiers:Standard Filters\";\"A band-pass Butterworth filter.\";\"ares butterbp asig, kfreq, kband [, iskip]\"\n"
"\"butterbr\";\"Signal Modifiers:Standard Filters\";\"A band-reject Butterworth filter.\";\"ares butterbr asig, kfreq, kband [, iskip]\"\n"
"\"butterhp\";\"Signal Modifiers:Standard Filters\";\"A high-pass Butterworth filter.\";\"ares butterhp asig, kfreq [, iskip]\"\n"
"\"butterlp\";\"Signal Modifiers:Standard Filters\";\"A low-pass Butterworth filter.\";\"ares butterlp asig, kfreq [, iskip]\"\n"
"\"clfilt\";\"Signal Modifiers:Standard Filters\";\"Implements low-pass and high-pass filters of different styles.\";\"ares clfilt asig, kfreq, itype, inpol [, ikind] [, ipbr] [, isba] [, iskip]\"\n"
"\"doppler\";\"Signal Modifiers:Standard Filters\";\"A fast and robust method for approximating sound propagation, achieving convincing Doppler shifts without having to solve equations.\";\"ashifted doppler asource, ksourceposition, kmicposition [, isoundspeed, ifiltercutoff]\"\n"
"\"mode\";\"Signal Modifiers:Standard Filters\";\"A filter that simulates a mass-spring-damper system\";\"aout mode ain, kfreq, kQ [, iskip]\"\n"
"\"tone\";\"Signal Modifiers:Standard Filters\";\"A first-order recursive low-pass filter with variable frequency response.\";\"ares tone asig, khp [, iskip]\"\n"
"\"tonex\";\"Signal Modifiers:Standard Filters\";\"Emulates a stack of filters using the tone opcode.\";\"ares tonex asig, khp [, inumlayer] [, iskip]\"\n"
"\"areson\";\"Signal Modifiers:Standard Filters:Resonant\";\"A notch filter whose transfer functions are the complements of the reson opcode.\";\"ares areson asig, kcf, kbw [, iscl] [, iskip]\"\n"
"\"bqrez\";\"Signal Modifiers:Standard Filters:Resonant\";\"A second-order multi-mode filter.\";\"ares bqrez asig, xfco, xres [, imode] [, iskip]\"\n"
"\"lowpass2\";\"Signal Modifiers:Standard Filters:Resonant\";\"A resonant lowpass filter.\";\"ares lowpass2 asig, kcf, kq [, iskip]\"\n"
"\"lowres\";\"Signal Modifiers:Standard Filters:Resonant\";\"Another resonant lowpass filter.\";\"ares lowres asig, kcutoff, kresonance [, iskip]\"\n"
"\"lowresx\";\"Signal Modifiers:Standard Filters:Resonant\";\"Simulates layers of serially connected resonant lowpass filters.\";\"ares lowresx asig, kcutoff, kresonance [, inumlayer] [, iskip]\"\n"
"\"lpf18\";\"Signal Modifiers:Standard Filters:Resonant\";\"A 3-pole sweepable resonant lowpass filter.\";\"ares lpf18 asig, kfco, kres, kdist [, iskip]\"\n"
"\"moogladder\";\"Signal Modifiers:Standard Filters:Resonant\";\"Moog ladder lowpass filter.\";\"asig moogladder ain, kcf, kres[, istor]\"\n"
"\"moogvcf\";\"Signal Modifiers:Standard Filters:Resonant\";\"A digital emulation of the Moog diode ladder filter configuration.\";\"ares moogvcf asig, xfco, xres [,iscale, iskip]\"\n"
"\"moogvcf2\";\"Signal Modifiers:Standard Filters:Resonant\";\"A digital emulation of the Moog diode ladder filter configuration.\";\"ares moogvcf2 asig, xfco, xres [,iscale, iskip]\"\n"
"\"reson\";\"Signal Modifiers:Standard Filters:Resonant\";\"A second-order resonant filter.\";\"ares reson asig, kcf, kbw [, iscl] [, iskip]\"\n"
"\"resonr\";\"Signal Modifiers:Standard Filters:Resonant\";\"A bandpass filter with variable frequency response.\";\"ares resonr asig, kcf, kbw [, iscl] [, iskip]\"\n"
"\"resonx\";\"Signal Modifiers:Standard Filters:Resonant\";\"Emulates a stack of filters using the reson opcode.\";\"ares resonx asig, kcf, kbw [, inumlayer] [, iscl] [, iskip]\"\n"
"\"resony\";\"Signal Modifiers:Standard Filters:Resonant\";\"A bank of second-order bandpass filters, connected in parallel.\";\"ares resony asig, kbf, kbw, inum, ksep [, isepmode] [, iscl] [, iskip]\"\n"
"\"resonz\";\"Signal Modifiers:Standard Filters:Resonant\";\"A bandpass filter with variable frequency response.\";\"ares resonz asig, kcf, kbw [, iscl] [, iskip]\"\n"
"\"rezzy\";\"Signal Modifiers:Standard Filters:Resonant\";\"A resonant low-pass filter.\";\"ares rezzy asig, xfco, xres [, imode, iskip]\"\n"
"\"statevar\";\"Signal Modifiers:Standard Filters:Resonant\";\"State-variable filter.\";\"ahp,alp,abp,abr statevar ain, kcf, kq [, iosamps, istor]\"\n"
"\"svfilter\";\"Signal Modifiers:Standard Filters:Resonant\";\"A resonant second order filter, with simultaneous lowpass, highpass and bandpass outputs.\";\"alow, ahigh, aband svfilter asig, kcf, kq [, iscl]\"\n"
"\"tbvcf\";\"Signal Modifiers:Standard Filters:Resonant\";\"Models some of the filter characteristics of a Roland TB303 voltage-controlled filter.\";\"ares tbvcf asig, xfco, xres, kdist, kasym [, iskip]\"\n"
"\"vlowres\";\"Signal Modifiers:Standard Filters:Resonant\";\"A bank of filters in which the cutoff frequency can be separated under user control.\";\"ares vlowres asig, kfco, kres, iord, ksep\"\n"
"\"aresonk\";\"Signal Modifiers:Standard Filters:Control\";\"A notch filter whose transfer functions are the complements of the reson opcode.\";\"kres aresonk ksig, kcf, kbw [, iscl] [, iskip]\"\n"
"\"atonek\";\"Signal Modifiers:Standard Filters:Control\";\"A hi-pass filter whose transfer functions are the complements of the\";\"kres atonek ksig, khp [, iskip]\"\n"
"\"lineto\";\"Signal Modifiers:Standard Filters:Control\";\"Generate glissandos starting from a control signal.\";\"kres lineto ksig, ktime\"\n"
"\"port\";\"Signal Modifiers:Standard Filters:Control\";\"Applies portamento to a step-valued control signal.\";\"kres port ksig, ihtim [, isig]\"\n"
"\"portk\";\"Signal Modifiers:Standard Filters:Control\";\"Applies portamento to a step-valued control signal.\";\"kres portk ksig, khtim [, isig]\"\n"
"\"resonk\";\"Signal Modifiers:Standard Filters:Control\";\"A second-order resonant filter.\";\"kres resonk ksig, kcf, kbw [, iscl] [, iskip]\"\n"
"\"resonxk\";\"Signal Modifiers:Standard Filters:Control\";\"Control signal resonant filter stack.\";\"kres resonxk ksig, kcf, kbw[, inumlayer, iscl, istor]\"\n"
"\"tlineto\";\"Signal Modifiers:Standard Filters:Control\";\"Generate glissandos starting from a control signal.\";\"kres tlineto ksig, ktime, ktrig\"\n"
"\"tonek\";\"Signal Modifiers:Standard Filters:Control\";\"A first-order recursive low-pass filter with variable frequency response.\";\"kres tonek ksig, khp [, iskip]\"\n"
"\"dcblock\";\"Signal Modifiers:Specialized Filters\";\"A DC blocking filter.\";\"ares dcblock ain [, igain]\"\n"
"\"dcblock2\";\"Signal Modifiers:Specialized Filters\";\"A DC blocking filter.\";\"ares dcblock2 ain [, iorder] [, iskip]\"\n"
"\"eqfil\";\"Signal Modifiers:Specialized Filters\";\"Equalizer filter\";\"asig eqfil ain, kcf, kbw, kgain[, istor]\"\n"
"\"filter2\";\"Signal Modifiers:Specialized Filters\";\"Performs filtering using a transposed form-II digital filter lattice with no time-varying control.\";\"ares filter2 asig, iM, iN, ib0, ib1, ..., ibM, ia1, ia2, ..., iaN\"\n"
"\"fofilter\";\"Signal Modifiers:Specialized Filters\";\"Formant filter.\";\"asig fofilter ain, kcf, kris, kdec[, istor]\"\n"
"\"hilbert\";\"Signal Modifiers:Specialized Filters\";\"A Hilbert transformer.\";\"ar1, ar2 hilbert asig\"\n"
"\"nlfilt\";\"Signal Modifiers:Specialized Filters\";\"A filter with a non-linear effect.\";\"ares nlfilt ain, ka, kb, kd, kC, kL\"\n"
"\"pareq\";\"Signal Modifiers:Specialized Filters\";\"Implementation of Zoelzer's parametric equalizer filters.\";\"ares pareq asig, kc, kv, kq [, imode] [, iskip]\"\n"
"\"rbjeq\";\"Signal Modifiers:Specialized Filters\";\"Parametric equalizer and filter opcode with 7 filter types, based on algorithm by Robert Bristow-Johnson.\";\"ar rbjeq asig, kfco, klvl, kQ, kS[, imode]\"\n"
"\"zfilter2\";\"Signal Modifiers:Specialized Filters\";\"Performs filtering using a transposed form-II digital filter lattice with radial pole-shearing and angular pole-warping.\";\"ares zfilter2 asig, kdamp, kfreq, iM, iN, ib0, ib1, ..., ibM,  ia1,ia2, ..., iaN\"\n"
"\"wguide1\";\"Signal Modifiers:Waveguides\";\"A simple waveguide model consisting of one delay-line and one first-order lowpass filter.\";\"ares wguide1 asig, xfreq, kcutoff, kfeedback\"\n"
"\"wguide2\";\"Signal Modifiers:Waveguides\";\"A model of beaten plate consisting of two parallel delay-lines and two first-order lowpass filters.\";\"ares wguide2 asig, xfreq1, xfreq2, kcutoff1, kcutoff2,  kfeedback1, kfeedback2\"\n"
"\"chebyshevpoly\";\"Signal Modifiers:Waveshaping\";\"Efficiently evaluates the sum of Chebyshev polynomials of arbitrary order.\";\"aout chebyshevpoly ain, k0 [, k1 [, k2 [...]]]\"\n"
"\"pdclip\";\"Signal Modifiers:Waveshaping\";\"Performs linear clipping on an audio signal or a phasor.\";\"aout pdclip ain, kWidth, kCenter [, ibipolar [, ifullscale]]\"\n"
"\"pdhalf\";\"Signal Modifiers:Waveshaping\";\"Distorts a phasor for reading the two halves of a table at different rates.\";\"aout pdhalf ain, kShapeAmount [, ibipolar [, ifullscale]]\"\n"
"\"pdhalfy\";\"Signal Modifiers:Waveshaping\";\"Distorts a phasor for reading two unequal portions of a table in equal periods.\";\"aout pdhalfy ain, kShapeAmount [, ibipolar [, ifullscale]]\"\n"
"\"powershape\";\"Signal Modifiers:Waveshaping\";\"Waveshapes a signal by raising it to a variable exponent.\";\"aout powershape ain, kShapeAmount [, ifullscale]\"\n"
"\"max\";\"Signal Modifiers:Comparators and Accumulators\";\"Produces a signal that is the maximum of any number of input signals.\";\"amax max ain1, ain2 [, ain3] [, ain4] [...]\"\n"
"\"max_k\";\"Signal Modifiers:Comparators and Accumulators\";\"Local maximum (or minimum) value of an incoming asig signal\";\"knumkout max_k asig, ktrig, itype\"\n"
"\"maxabs\";\"Signal Modifiers:Comparators and Accumulators\";\"Produces a signal that is the maximum of the absolute values of any number of input signals.\";\"amax maxabs ain1, ain2 [, ain3] [, ain4] [...]\"\n"
"\"maxabsaccum\";\"Signal Modifiers:Comparators and Accumulators\";\"Accumulates the maximum of the absolute values of audio signals.\";\"maxabsaccum aAccumulator, aInput\"\n"
"\"maxaccum\";\"Signal Modifiers:Comparators and Accumulators\";\"Accumulates the maximum value of audio signals.\";\"maxaccum aAccumulator, aInput\"\n"
"\"min\";\"Signal Modifiers:Comparators and Accumulators\";\"Produces a signal that is the minimum of any number of input signals.\";\"amin min ain1, ain2 [, ain3] [, ain4] [...]\"\n"
"\"minabs\";\"Signal Modifiers:Comparators and Accumulators\";\"Produces a signal that is the minimum of the absolute values of any number of input signals.\";\"amin minabs ain1, ain2 [, ain3] [, ain4] [...]\"\n"
"\"minabsaccum\";\"Signal Modifiers:Comparators and Accumulators\";\"Accumulates the minimum of the absolute values of audio signals.\";\"minabsaccum aAccumulator, aInput\"\n"
"\"minaccum\";\"Signal Modifiers:Comparators and Accumulators\";\"Accumulates the minimum value of audio signals.\";\"minaccum aAccumulator, aInput\"\n"
"\"clockoff\";\"Instrument Control:Clock Control\";\"Stops one of a number of internal clocks.\";\"clockoff inum\"\n"
"\"clockon\";\"Instrument Control:Clock Control\";\"Starts one of a number of internal clocks.\";\"clockon inum\"\n"
"\"ihold\";\"Instrument Control:Duration Control\";\"Creates a held note.\";\"ihold\"\n"
"\"turnoff\";\"Instrument Control:Duration Control\";\"Enables an instrument to turn itself off.\";\"turnoff\"\n"
"\"turnoff2\";\"Instrument Control:Duration Control\";\"Turn off instance(s) of other instruments at performance time.\";\"turnoff2 kinsno, kmode, krelease\"\n"
"\"turnon\";\"Instrument Control:Duration Control\";\"Activate an instrument for an indefinite time.\";\"turnon insnum [, itime]\"\n"
"\"event\";\"Instrument Control:Invocation\";\"Generates a score event from an instrument.\";\"event \"scorechar\", kinsnum, kdelay, kdur, [, kp4] [, kp5] [, ...]\"\n"
"\"event_i\";\"Instrument Control:Invocation\";\"Generates a score event from an instrument.\";\"event_i \"scorechar\", iinsnum, idelay, idur, [, ip4] [, ip5] [, ...]\"\n"
"\"mute\";\"Instrument Control:Invocation\";\"Mutes/unmutes new instances of a given instrument.\";\"mute insnum [, iswitch]\"\n"
"\"remove\";\"Instrument Control:Invocation\";\"Removes the definition of an instrument.\";\"remove insnum\"\n"
"\"schedkwhen\";\"Instrument Control:Invocation\";\"Adds a new score event generated by a k-rate trigger.\";\"schedkwhen ktrigger, kmintim, kmaxnum, kinsnum, kwhen, kdur  [, ip4] [, ip5] [...]\"\n"
"\"schedkwhennamed\";\"Instrument Control:Invocation\";\"Similar to schedkwhen but uses a named instrument at init-time.\";\"schedkwhennamed ktrigger, kmintim, kmaxnum, \"name\", kwhen, kdur  [, ip4] [, ip5] [...]\"\n"
"\"schedule\";\"Instrument Control:Invocation\";\"Adds a new score event.\";\"schedule insnum, iwhen, idur [, ip4] [, ip5] [...]\"\n"
"\"schedwhen\";\"Instrument Control:Invocation\";\"Adds a new score event.\";\"schedwhen ktrigger, kinsnum, kwhen, kdur [, ip4] [, ip5] [...]\"\n"
"\"scoreline\";\"Instrument Control:Invocation\";\"Issues one or more score line events from an instrument.\";\"scoreline Sin, ktrig\"\n"
"\"scoreline_i\";\"Instrument Control:Invocation\";\"Issues one or more score line events from an instrument at i-time.\";\"scoreline_i Sin\"\n"
"\"cggoto\";\"Instrument Control:Program Flow Control\";\"Conditionally transfer control on every pass.\";\"cggoto condition, label\"\n"
"\"cigoto\";\"Instrument Control:Program Flow Control\";\"Conditionally transfer control during the i-time pass.\";\"cigoto condition, label\"\n"
"\"ckgoto\";\"Instrument Control:Program Flow Control\";\"Conditionally transfer control during the p-time passes.\";\"ckgoto condition, label\"\n"
"\"cngoto\";\"Instrument Control:Program Flow Control\";\"Transfers control on every pass when a condition is not true.\";\"cngoto condition, label\"\n"
"\"else\";\"Instrument Control:Program Flow Control\";\"Executes a block of code when an \"if...then\" condition is false.\";\"else\"\n"
"\"elseif\";\"Instrument Control:Program Flow Control\";\"Defines another \"if...then\" condition when a \"if...then\" condition is false.\";\"elseif xa R xb then\"\n"
"\"endif\";\"Instrument Control:Program Flow Control\";\"Closes a block of code that begins with an \"if...then\" statement.\";\"endif\"\n"
"\"goto\";\"Instrument Control:Program Flow Control\";\"Transfer control on every pass.\";\"goto label\"\n"
"\"if\";\"Instrument Control:Program Flow Control\";\"Branches conditionally at initialization or during performance time.\";\"if ia R ib igoto label\"\n"
"\"igoto\";\"Instrument Control:Program Flow Control\";\"Transfer control during the i-time pass.\";\"igoto label\"\n"
"\"kgoto\";\"Instrument Control:Program Flow Control\";\"Transfer control during the p-time passes.\";\"kgoto label\"\n"
"\"loop_ge\";\"Instrument Control:Program Flow Control\";\"Looping constructions.\";\"loop_ge indx, idecr, imin, label\"\n"
"\"loop_gt\";\"Instrument Control:Program Flow Control\";\"Looping constructions.\";\"loop_gt indx, idecr, imin, label\"\n"
"\"loop_le\";\"Instrument Control:Program Flow Control\";\"Looping constructions.\";\"loop_le indx, incr, imax, label\"\n"
"\"loop_lt\";\"Instrument Control:Program Flow Control\";\"Looping constructions.\";\"loop_lt indx, incr, imax, label\"\n"
"\"tigoto\";\"Instrument Control:Program Flow Control\";\"Transfer control at i-time when a new note is being tied onto a previously held note\";\"tigoto label\"\n"
"\"timout\";\"Instrument Control:Program Flow Control\";\"Conditional branch during p-time depending on elapsed note time.\";\"timout istrt, idur, label\"\n"
"\"until\";\"Instrument Control:Program Flow Control\";\"A syntactic looping construction.\";\"until condition do ... od\"\n"
"\"active\";\"Instrument Control:Realtime Performance Control\";\"Returns the number of active instances of an instrument.\";\"ir active insnum [,iopt]\"\n"
"\"cpuprc\";\"Instrument Control:Realtime Performance Control\";\"Control allocation of cpu resources on a per-instrument basis, to optimize realtime output.\";\"cpuprc insnum, ipercent\"\n"
"\"exitnow\";\"Instrument Control:Realtime Performance Control\";\"Exit Csound as fast as possible, with no cleaning up.\";\"exitnow\"\n"
"\"jacktransport\";\"Instrument Control:Realtime Performance Control\";\"Start/stop jack_transport and can optionally relocate the playback head.\";\"jacktransport icommand [, ilocation]\"\n"
"\"maxalloc\";\"Instrument Control:Realtime Performance Control\";\"Limits the number of allocations of an instrument.\";\"maxalloc insnum, icount\"\n"
"\"prealloc\";\"Instrument Control:Realtime Performance Control\";\"Creates space for instruments but does not run them.\";\"prealloc insnum, icount\"\n"
"\"init\";\"Instrument Control:Initialization and Reinitialization\";\"Puts the value of the i-time expression into a k-, a-rate or t- variable.\";\"ares init iarg\"\n"
"\"nstrnum\";\"Instrument Control:Initialization and Reinitialization\";\"Returns the number of a named instrument.\";\"insno nstrnum \"name\"\"\n"
"\"p\";\"Instrument Control:Initialization and Reinitialization\";\"Show the value in a given p-field.\";\"p(x)\"\n"
"\"passign\";\"Instrument Control:Initialization and Reinitialization\";\"Assigns a range of p-fields to ivariables.\";\"ivar1, ... passign [istart]\"\n"
"\"pset\";\"Instrument Control:Initialization and Reinitialization\";\"Defines and initializes numeric arrays at orchestra load time.\";\"pset icon1 [, icon2] [...]\"\n"
"\"reinit\";\"Instrument Control:Initialization and Reinitialization\";\"Suspends a performance while a special initialization pass is executed.\";\"reinit label\"\n"
"\"rigoto\";\"Instrument Control:Initialization and Reinitialization\";\"Transfers control during a reinit pass.\";\"rigoto label\"\n"
"\"rireturn\";\"Instrument Control:Initialization and Reinitialization\";\"Terminates a reinit pass.\";\"rireturn\"\n"
"\"tival\";\"Instrument Control:Initialization and Reinitialization\";\"Puts the value of the instrument's internal\";\"ir tival\"\n"
"\"button\";\"Instrument Control:Sensing and Control\";\"Sense on-screen controls.\";\"kres button knum\"\n"
"\"changed\";\"Instrument Control:Sensing and Control\";\"k-rate signal change detector.\";\"ktrig changed kvar1 [, kvar2,..., kvarN]\"\n"
"\"checkbox\";\"Instrument Control:Sensing and Control\";\"Sense on-screen controls.\";\"kres checkbox knum\"\n"
"\"control\";\"Instrument Control:Sensing and Control\";\"Configurable slider controls for realtime user input.\";\"kres control knum\"\n"
"\"follow\";\"Instrument Control:Sensing and Control\";\"Envelope follower unit generator.\";\"ares follow asig, idt\"\n"
"\"follow2\";\"Instrument Control:Sensing and Control\";\"Another controllable envelope extractor.\";\"ares follow2 asig, katt, krel\"\n"
"\"getcfg\";\"Instrument Control:Sensing and Control\";\"Return Csound settings.\";\"Svalue getcfg iopt\"\n"
"\"metro\";\"Instrument Control:Sensing and Control\";\"Trigger Metronome\";\"ktrig metro kfreq [, initphase]\"\n"
"\"miditempo\";\"Instrument Control:Sensing and Control\";\"Returns the current tempo at k-rate, of either the MIDI file (if available) or the score\";\"ksig miditempo\"\n"
"\"p5gconnect\";\"Instrument Control:Sensing and Control\";\"Reads data from a P5 Glove controller.\";\"p5gconnect\"\n"
"\"p5gdata\";\"Instrument Control:Sensing and Control\";\"Reads data fields from an external P5 Glove.\";\"kres p5gdata kcontrol\"\n"
"\"pcount\";\"Instrument Control:Sensing and Control\";\"Returns the number of pfields belonging to a note event.\";\"icount pcount\"\n"
"\"peak\";\"Instrument Control:Sensing and Control\";\"Maintains the output equal to the highest absolute value received.\";\"kres peak asig\"\n"
"\"pindex\";\"Instrument Control:Sensing and Control\";\"Returns the value of a specified pfield.\";\"ivalue pindex ipfieldIndex\"\n"
"\"pitch\";\"Instrument Control:Sensing and Control\";\"Tracks the pitch of a signal.\";\"koct, kamp pitch asig, iupdte, ilo, ihi, idbthresh [, ifrqs] [, iconf]  [, istrt] [, iocts] [, iq] [, inptls] [, irolloff] [, iskip]\"\n"
"\"pitchamdf\";\"Instrument Control:Sensing and Control\";\"Follows the pitch of a signal based on the AMDF method.\";\"kcps, krms pitchamdf asig, imincps, imaxcps [, icps] [, imedi]  [, idowns] [, iexcps] [, irmsmedi]\"\n"
"\"ptrack\";\"Instrument Control:Sensing and Control\";\"Tracks the pitch of a signal.\";\"kcps, kamp ptrack asig, ihopsize[,ipeaks]\"\n"
"\"rewindscore\";\"Instrument Control:Sensing and Control\";\"Rewinds the playback position of the current score performance.\";\"rewindscore\"\n"
"\"rms\";\"Instrument Control:Sensing and Control\";\"Determines the root-mean-square amplitude of an audio signal.\";\"kres rms asig [, ihp] [, iskip]\"\n"
"\"sensekey\";\"Instrument Control:Sensing and Control\";\"Returns the ASCII code of a key that has been pressed.\";\"kres[, kkeydown] sensekey\"\n"
"\"sense\";\"Instrument Control:Sensing and Control\";\"(same as sensekey) Returns the ASCII code of a key that has been pressed.\";\"kres[, kkeydown] sensekey\"\n"
"\"seqtime\";\"Instrument Control:Sensing and Control\";\"Generates a trigger signal according to the values stored in a table.\";\"ktrig_out seqtime ktime_unit, kstart, kloop, kinitndx, kfn_times\"\n"
"\"seqtime2\";\"Instrument Control:Sensing and Control\";\"Generates a trigger signal according to the values stored in a table.\";\"ktrig_out seqtime2 ktrig_in, ktime_unit, kstart, kloop, kinitndx, kfn_times\"\n"
"\"setctrl\";\"Instrument Control:Sensing and Control\";\"Configurable slider controls for realtime user input.\";\"setctrl inum, ival, itype\"\n"
"\"setscorepos  ipos\";\"Instrument Control:Sensing and Control\";\"Sets the playback position of the current score performance to a given position.\";\"setscorepos ipos\"\n"
"\"splitrig\";\"Instrument Control:Sensing and Control\";\"Split a trigger signal\";\"splitrig ktrig, kndx, imaxtics, ifn, kout1 [,kout2,...,koutN]\"\n"
"\"tempest\";\"Instrument Control:Sensing and Control\";\"Estimate the tempo of beat patterns in a control signal.\";\"ktemp tempest kin, iprd, imindur, imemdur, ihp, ithresh, ihtim, ixfdbak,  istartempo, ifn [, idisprd] [, itweek]\"\n"
"\"tempo\";\"Instrument Control:Sensing and Control\";\"Apply tempo control to an uninterpreted score.\";\"tempo ktempo, istartempo\"\n"
"\"tempoval\";\"Instrument Control:Sensing and Control\";\"Reads the current value of the tempo.\";\"kres tempoval\"\n"
"\"timedseq\";\"Instrument Control:Sensing and Control\";\"Time Variant Sequencer\";\"ktrig timedseq ktimpnt, ifn, kp1 [,kp2, kp3, ...,kpN]\"\n"
"\"trigger\";\"Instrument Control:Sensing and Control\";\"Informs when a krate signal crosses a threshold.\";\"kout trigger ksig, kthreshold, kmode\"\n"
"\"trigseq\";\"Instrument Control:Sensing and Control\";\"Accepts a trigger signal as input and outputs a group of values.\";\"trigseq ktrig_in, kstart, kloop, kinitndx, kfn_values, kout1 [, kout2] [...]\"\n"
"\"wiiconnect\";\"Instrument Control:Sensing and Control\";\"Reads data from a number of external Nintendo Wiimote controllers.\";\"ires wiiconnect [itimeout, imaxnum]\"\n"
"\"wiidata\";\"Instrument Control:Sensing and Control\";\"Reads data fields from a number of external Nintendo Wiimote controllers.\";\"kres wiidata kcontrol[, knum]\"\n"
"\"wiirange\";\"Instrument Control:Sensing and Control\";\"Sets scaling and range limits for certain Wiimote fields.\";\"wiirange icontrol, iminimum, imaximum[, inum]\"\n"
"\"wiisend\";\"Instrument Control:Sensing and Control\";\"Sends data to one of a number of external Nintendo Wiimote controllers.\";\"kres wiisend kcontrol, kvalue[, knum]\"\n"
"\"xyin\";\"Instrument Control:Sensing and Control\";\"Sense the cursor position in an output window\";\"kx, ky xyin iprd, ixmin, ixmax, iymin, iymax [, ixinit] [, iyinit]\"\n"
"\"pop\";\"Instrument Control:Stacks\";\"Pops values from the global stack.\";\"xval1, [xval2, ... , xval31] pop\"\n"
"\"pop_f\";\"Instrument Control:Stacks\";\"Pops an f-sig frame from the global stack.\";\"fsig pop_f\"\n"
"\"push\";\"Instrument Control:Stacks\";\"Pushes a value into the global stack.\";\"push xval1, [xval2, ... , xval31]\"\n"
"\"push_f\";\"Instrument Control:Stacks\";\"Pushes an f-sig frame into the global stack.\";\"push_f fsig\"\n"
"\"stack\";\"Instrument Control:Stacks\";\"Initializes the stack.\";\"stack iStackSize\"\n"
"\"subinstr\";\"Instrument Control:Subinstrument Control\";\"Creates and runs a numbered instrument instance.\";\"a1, [...] [, a8] subinstr instrnum [, p4] [, p5] [...]\"\n"
"\"subinstrinit\";\"Instrument Control:Subinstrument Control\";\"Creates and runs a numbered instrument instance at init-time.\";\"subinstrinit instrnum [, p4] [, p5] [...]\"\n"
"\"date\";\"Instrument Control:Time Reading\";\"Returns the number seconds since a base date.\";\"ir date\"\n"
"\"dates\";\"Instrument Control:Time Reading\";\"Returns as a string the date and time specified.\";\"Sir dates [ itime]\"\n"
"\"readclock\";\"Instrument Control:Time Reading\";\"Reads the value of an internal clock.\";\"ir readclock inum\"\n"
"\"rtclock\";\"Instrument Control:Time Reading\";\"Read the real time clock from the operating system.\";\"ires rtclock\"\n"
"\"timeinstk\";\"Instrument Control:Time Reading\";\"Read absolute time in k-rate cycles.\";\"kres timeinstk\"\n"
"\"timeinsts\";\"Instrument Control:Time Reading\";\"Read absolute time in seconds.\";\"kres timeinsts\"\n"
"\"timek\";\"Instrument Control:Time Reading\";\"Read absolute time in k-rate cycles.\";\"ires timek\"\n"
"\"times\";\"Instrument Control:Time Reading\";\"Read absolute time in seconds.\";\"ires times\"\n"
"\"JackoAudioIn \";\"Jacko Opcodes\";\"Receives an audio signal from a Jack port.\";\"asignal JackoAudioIn ScsoundPortName\"\n"
"\"JackoAudioInConnect \";\"Jacko Opcodes\";\"Creates an audio connection from a Jack port to Csound.\";\"JackoAudioInConnect SexternalPortName, ScsoundPortName\"\n"
"\"JackoAudioOut \";\"Jacko Opcodes\";\"Sends an audio signal to a Jack port.\";\"JackoAudioOut ScsoundPortName, asignal\"\n"
"\"JackoAudioOutConnect \";\"Jacko Opcodes\";\"Creates an audio connection from Csound to a Jack port.\";\"JackoAudioOutConnect ScsoundPortName, SexternalPortName\"\n"
"\"JackoFreewheel \";\"Jacko Opcodes\";\"Turns Jack's freewheeling mode on or off.\";\"JackoFreewheel [ienabled]\"\n"
"\"JackoInfo \";\"Jacko Opcodes\";\"Prints information about the Jack system.\";\"JackoInfo\"\n"
"\"JackoInit \";\"Jacko Opcodes\";\"Initializes Csound as a Jack client.\";\"JackoInit SclientName, ServerName\"\n"
"\"JackoMidiInConnect \";\"Jacko Opcodes\";\"Creates a MIDI connection from a Jack port to Csound.\";\"JackoMidiInConnect SexternalPortName, ScsoundPortName\"\n"
"\"JackoMidiOut \";\"Jacko Opcodes\";\"Sends a MIDI channel message to a Jack port.\";\"JackoMidiOut ScsoundPortName, kstatus, kchannel, kdata1[, kdata2]\"\n"
"\"JackoMidiOutConnect \";\"Jacko Opcodes\";\"Creates a MIDI connection from Csound to a Jack port.\";\"JackoMidiOutConnect ScsoundPortName, SexternalPortName\"\n"
"\"JackoNoteOut \";\"Jacko Opcodes\";\"Sends a MIDI channel message to a Jack port.\";\"JackoNoteOut ScsoundPortName, kstatus, kchannel, kdata1[, kdata2]\"\n"
"\"JackoOn \";\"Jacko Opcodes\";\"Enables or disables all Jack ports.\";\"JackoOn [iactive]\"\n"
"\"JackoTransport \";\"Jacko Opcodes\";\"Control the Jack transport.\";\"JackoTransport kcommand, [kposition]\"\n"
"\"lua_exec\";\"Lua Opcodes\";\"Executes an arbitrary block of Lua code from the Csound orchestra.\";\"lua_exec Sluacode\"\n"
"\"lua_iopcall\";\"Lua Opcodes\";\"Calls a Lua opcode at i-rate only. Any number of output and/or input arguments may be passed. All arguments must be passed on the right-hand side. Outputs are returned in the argument.\";\"lua_iopcall Sname, ...\"\n"
"\"lua_ikopcall\";\"Lua Opcodes\";\"Calls a Lua opcode at i-rate only. Any number of output and/or input arguments may be passed. All arguments must be passed on the right-hand side. Outputs are returned in the argument.\";\"lua_ikopcall Sname, ...\"\n"
"\"lua_iaopcall\";\"Lua Opcodes\";\"Calls a Lua opcode at i-rate only. Any number of output and/or input arguments may be passed. All arguments must be passed on the right-hand side. Outputs are returned in the argument.\";\"lua_iaopcall Sname, ...\"\n"
"\"lua_iopcall_off\";\"Lua Opcodes\";\"Calls a Lua opcode at i-rate only. Any number of output and/or input arguments may be passed. All arguments must be passed on the right-hand side. Outputs are returned in the argument.\";\"lua_iopcall_off Sname, ...\"\n"
"\"lua_ikopcall_off\";\"Lua Opcodes\";\"Calls a Lua opcode at i-rate only. Any number of output and/or input arguments may be passed. All arguments must be passed on the right-hand side. Outputs are returned in the argument.\";\"lua_ikopcall_off Sname, ...\"\n"
"\"lua_iaopcall_off\";\"Lua Opcodes\";\"Calls a Lua opcode at i-rate only. Any number of output and/or input arguments may be passed. All arguments must be passed on the right-hand side. Outputs are returned in the argument.\";\"lua_iaopcall_off Sname, ...\"\n"
"\"lua_opdef\";\"Lua Opcodes\";\"Define an opcode in Lua at i-rate. The opcode can take any number of output and/or input arguments of any type.\";\"lua_opdef Sname, Sluacode\"\n"
"\"serialBegin\";\"Serial I/O\";\"Open a serial port.\";\"iPort serialBegin SPortName [, ibaudRate]\"\n"
"\"serialEnd\";\"Serial I/O\";\"Close a serial port.\";\"serialEnd iPort\"\n"
"\"serialFlush\";\"Serial I/O\";\"Flush data from a serial port.\";\"serialFlush iPort\"\n"
"\"serialPrint\";\"Serial I/O\";\"Print data from a serial port.\";\"serialPrint iPort\"\n"
"\"serialRead\";\"Serial I/O\";\"Read data from a serial port.\";\"kByte serialRead iPort\"\n"
"\"serialWrite\";\"Serial I/O\";\"Write data to a serial port.\";\"serialWrite iPort, iByte\"\n"
"\"serialWrite_i\";\"Serial I/O\";\"Write data to a serial port.\";\"serialWrite_i iPort, iByte\"\n"
"\"ftfree\";\"Table Control\";\"Deletes function table.\";\"ftfree ifno, iwhen\"\n"
"\"ftgen\";\"Table Control\";\"Generate a score function table from within the orchestra.\";\"gir ftgen ifn, itime, isize, igen, iarga [, iargb ] [...]\"\n"
"\"ftgentmp\";\"Table Control\";\"Generate a score function table from within the orchestra, which is deleted at the end of the note.\";\"ifno ftgentmp ip1, ip2dummy, isize, igen, iarga, iargb, ...\"\n"
"\"sndload\";\"Table Control\";\"Loads a sound file into memory for use by\";\"sndload Sfname[, ifmt[, ichns[, isr[, ibas[, iamp[, istrt  [, ilpmod[, ilps[, ilpe]]]]]]]]]\"\n"
"\"ftchnls\";\"Table Control:Table Queries\";\"Returns the number of channels in a stored function table.\";\"ftchnls(x) (init-rate args only)\"\n"
"\"ftcps\";\"Table Control:Table Queries\";\"Returns the base frequency of a stored function table in Hz.\";\"ftcps(x) (init-rate args only)\"\n"
"\"ftlen\";\"Table Control:Table Queries\";\"Returns the size of a stored function table.\";\"ftlen(x) (init-rate args only)\"\n"
"\"ftlptim\";\"Table Control:Table Queries\";\"Returns the loop segment start-time of a stored function table number.\";\"ftlptim(x) (init-rate args only)\"\n"
"\"ftsr\";\"Table Control:Table Queries\";\"Returns the sampling-rate of a stored function table.\";\"ftsr(x) (init-rate args only)\"\n"
"\"nsamp\";\"Table Control:Table Queries\";\"Returns the number of samples loaded into a stored function table number.\";\"nsamp(x) (init-rate args only)\"\n"
"\"tableng\";\"Table Control:Table Queries\";\"Interrogates a function table for length.\";\"ires tableng ifn\"\n"
"\"tabsum\";\"Table Control:Table Queries\";\"Adding values in a range of a table.\";\"kr tabsum ifn[[, kmin] [, kmax]]\"\n"
"\"tb0_init\";\"Table Control:Table Queries\";\"Table Read Access inside expressions.\";\"tb0_init ifn\"\n"
"\"tb1_init\";\"Table Control:Table Queries\";\"Table Read Access inside expressions.\";\"tb1_init ifn\"\n"
"\"tb2_init\";\"Table Control:Table Queries\";\"Table Read Access inside expressions.\";\"tb2_init ifn\"\n"
"\"tb3_init\";\"Table Control:Table Queries\";\"Table Read Access inside expressions.\";\"tb3_init ifn\"\n"
"\"tb4_init\";\"Table Control:Table Queries\";\"Table Read Access inside expressions.\";\"tb4_init ifn\"\n"
"\"tb5_init\";\"Table Control:Table Queries\";\"Table Read Access inside expressions.\";\"tb5_init ifn\"\n"
"\"tb6_init\";\"Table Control:Table Queries\";\"Table Read Access inside expressions.\";\"tb6_init ifn\"\n"
"\"tb7_init\";\"Table Control:Table Queries\";\"Table Read Access inside expressions.\";\"tb7_init ifn\"\n"
"\"tb8_init\";\"Table Control:Table Queries\";\"Table Read Access inside expressions.\";\"tb8_init ifn\"\n"
"\"tb9_init\";\"Table Control:Table Queries\";\"Table Read Access inside expressions.\";\"tb9_init ifn\"\n"
"\"tb10_init\";\"Table Control:Table Queries\";\"Table Read Access inside expressions.\";\"tb10_init ifn\"\n"
"\"tb11_init\";\"Table Control:Table Queries\";\"Table Read Access inside expressions.\";\"tb11_init ifn\"\n"
"\"tb12_init\";\"Table Control:Table Queries\";\"Table Read Access inside expressions.\";\"tb12_init ifn\"\n"
"\"tb13_init\";\"Table Control:Table Queries\";\"Table Read Access inside expressions.\";\"tb13_init ifn\"\n"
"\"tb14_init\";\"Table Control:Table Queries\";\"Table Read Access inside expressions.\";\"tb14_init ifn\"\n"
"\"tb15_init\";\"Table Control:Table Queries\";\"Table Read Access inside expressions.\";\"tb15_init ifn\"\n"
"\"tb0\";\"Table Control:Table Queries\";\"Table Read Access inside expressions.\";\"iout = tb0(iIndex)\"\n"
"\"tb1\";\"Table Control:Table Queries\";\"Table Read Access inside expressions.\";\"iout = tb1(iIndex)\"\n"
"\"tb2\";\"Table Control:Table Queries\";\"Table Read Access inside expressions.\";\"iout = tb2(iIndex)\"\n"
"\"tb3\";\"Table Control:Table Queries\";\"Table Read Access inside expressions.\";\"iout = tb3(iIndex)\"\n"
"\"tb4\";\"Table Control:Table Queries\";\"Table Read Access inside expressions.\";\"iout = tb4(iIndex)\"\n"
"\"tb5\";\"Table Control:Table Queries\";\"Table Read Access inside expressions.\";\"iout = tb5(iIndex)\"\n"
"\"tb6\";\"Table Control:Table Queries\";\"Table Read Access inside expressions.\";\"iout = tb6(iIndex)\"\n"
"\"tb7\";\"Table Control:Table Queries\";\"Table Read Access inside expressions.\";\"iout = tb7(iIndex)\"\n"
"\"tb8\";\"Table Control:Table Queries\";\"Table Read Access inside expressions.\";\"iout = tb8(iIndex)\"\n"
"\"tb9\";\"Table Control:Table Queries\";\"Table Read Access inside expressions.\";\"iout = tb9(iIndex)\"\n"
"\"tb10\";\"Table Control:Table Queries\";\"Table Read Access inside expressions.\";\"iout = tb10(iIndex)\"\n"
"\"tb11\";\"Table Control:Table Queries\";\"Table Read Access inside expressions.\";\"iout = tb11(iIndex)\"\n"
"\"tb12\";\"Table Control:Table Queries\";\"Table Read Access inside expressions.\";\"iout = tb12(iIndex)\"\n"
"\"tb13\";\"Table Control:Table Queries\";\"Table Read Access inside expressions.\";\"iout = tb13(iIndex)\"\n"
"\"tb14\";\"Table Control:Table Queries\";\"Table Read Access inside expressions.\";\"iout = tb14(iIndex)\"\n"
"\"tb15\";\"Table Control:Table Queries\";\"Table Read Access inside expressions.\";\"iout = tb15(iIndex)\"\n"
"\"tableikt\";\"Table Control:Dynamic Selection\";\"Provides k-rate control over table numbers.\";\"ares tableikt xndx, kfn [, ixmode] [, ixoff] [, iwrap]\"\n"
"\"tablekt\";\"Table Control:Dynamic Selection\";\"Provides k-rate control over table numbers.\";\"ares tablekt xndx, kfn [, ixmode] [, ixoff] [, iwrap]\"\n"
"\"tablexkt\";\"Table Control:Dynamic Selection\";\"Reads function tables with linear, cubic, or sinc interpolation.\";\"ares tablexkt xndx, kfn, kwarp, iwsize [, ixmode] [, ixoff] [, iwrap]\"\n"
"\"ftload\";\"Table Control:Read/Write Opreations\";\"Load a set of previously-allocated tables from a file.\";\"ftload \"filename\", iflag, ifn1 [, ifn2] [...]\"\n"
"\"ftloadk\";\"Table Control:Read/Write Opreations\";\"Load a set of previously-allocated tables from a file.\";\"ftloadk \"filename\", ktrig, iflag, ifn1 [, ifn2] [...]\"\n"
"\"ftsave\";\"Table Control:Read/Write Opreations\";\"Save a set of previously-allocated tables to a file.\";\"ftsave \"filename\", iflag, ifn1 [, ifn2] [...]\"\n"
"\"ftsavek\";\"Table Control:Read/Write Opreations\";\"Save a set of previously-allocated tables to a file.\";\"ftsavek \"filename\", ktrig, iflag, ifn1 [, ifn2] [...]\"\n"
"\"ptablew\";\"Table Control:Read/Write Opreations\";\"Change the contents of existing function tables of any length.\";\"ptablew asig, andx, ifn [, ixmode] [, ixoff] [, iwgmode]\"\n"
"\"tablecopy\";\"Table Control:Read/Write Opreations\";\"Simple, fast table copy opcode.\";\"tablecopy kdft, ksft\"\n"
"\"tablegpw\";\"Table Control:Read/Write Opreations\";\"Writes a table's guard point.\";\"tablegpw kfn\"\n"
"\"tableicopy\";\"Table Control:Read/Write Opreations\";\"Simple, fast table copy opcode.\";\"tableicopy idft, isft\"\n"
"\"tableigpw\";\"Table Control:Read/Write Opreations\";\"Writes a table's guard point.\";\"tableigpw ifn\"\n"
"\"tableimix\";\"Table Control:Read/Write Opreations\";\"Mixes two tables.\";\"tableimix idft, idoff, ilen, is1ft, is1off, is1g, is2ft, is2off, is2g\"\n"
"\"tableiw\";\"Table Control:Read/Write Opreations\";\"Change the contents of existing function tables.\";\"tableiw isig, indx, ifn [, ixmode] [, ixoff] [, iwgmode]\"\n"
"\"tablemix\";\"Table Control:Read/Write Opreations\";\"Mixes two tables.\";\"tablemix kdft, kdoff, klen, ks1ft, ks1off, ks1g, ks2ft, ks2off, ks2g\"\n"
"\"tablera\";\"Table Control:Read/Write Opreations\";\"Reads tables in sequential locations.\";\"ares tablera kfn, kstart, koff\"\n"
"\"tablew\";\"Table Control:Read/Write Opreations\";\"Change the contents of existing function tables.\";\"tablew asig, andx, ifn [, ixmode] [, ixoff] [, iwgmode]\"\n"
"\"tablewa\";\"Table Control:Read/Write Opreations\";\"Writes tables in sequential locations.\";\"kstart tablewa kfn, asig, koff\"\n"
"\"tablewkt\";\"Table Control:Read/Write Opreations\";\"Change the contents of existing function tables.\";\"tablewkt asig, andx, kfn [, ixmode] [, ixoff] [, iwgmode]\"\n"
"\"tabmorph\";\"Table Control:Read/Write Opreations\";\"Allow morphing between a set of tables.\";\"kout tabmorph kindex, kweightpoint, ktabnum1, ktabnum2,  ifn1, ifn2 [, ifn3, ifn4, ...,ifnN]\"\n"
"\"tabmorpha\";\"Table Control:Read/Write Opreations\";\"Allow morphing between a set of tables at audio rate with interpolation.\";\"aout tabmorpha aindex, aweightpoint, atabnum1, atabnum2,  ifn1, ifn2 [, ifn3, ifn4, ... ifnN]\"\n"
"\"tabmorphak\";\"Table Control:Read/Write Opreations\";\"Allow morphing between a set of tables at audio rate with interpolation.\";\"aout tabmorphak aindex, kweightpoint, ktabnum1, ktabnum2,  ifn1, ifn2 [, ifn3, ifn4, ... ifnN]\"\n"
"\"tabmorphi\";\"Table Control:Read/Write Opreations\";\"Allow morphing between a set of tables with interpolation.\";\"kout tabmorphi kindex, kweightpoint, ktabnum1, ktabnum2,  ifn1, ifn2 [, ifn3, ifn4, ..., ifnN]\"\n"
"\"tabplay\";\"Table Control:Read/Write Opreations\";\"Playing-back control signals.\";\"tabplay ktrig, knumtics, kfn, kout1 [,kout2,..., koutN]\"\n"
"\"tabrec\";\"Table Control:Read/Write Opreations\";\"Recording of control signals.\";\"tabrec ktrig_start, ktrig_stop, knumtics, kfn, kin1 [,kin2,...,kinN]\"\n"
"\"FLgroup\";\"FLTK:Containers\";\"A FLTK container opcode that groups child widgets.\";\"FLgroup \"label\", iwidth, iheight, ix, iy [, iborder] [, image]\"\n"
"\"FLgroupEnd\";\"FLTK:Containers\";\"Marks the end of a group of FLTK child widgets.\";\"FLgroupEnd\"\n"
"\"FLpack\";\"FLTK:Containers\";\"Provides the functionality of compressing and aligning FLTK widgets.\";\"FLpack iwidth, iheight, ix, iy, itype, ispace, iborder\"\n"
"\"FLpackEnd\";\"FLTK:Containers\";\"Marks the end of a group of compressed or aligned FLTK widgets.\";\"FLpackEnd\"\n"
"\"FLpanel\";\"FLTK:Containers\";\"Creates a window that contains FLTK widgets.\";\"FLpanel \"label\", iwidth, iheight [, ix] [, iy] [, iborder] [, ikbdcapture] [, iclose]\"\n"
"\"FLpanelEnd\";\"FLTK:Containers\";\"Marks the end of a group of FLTK widgets contained inside of a window (panel).\";\"FLpanelEnd\"\n"
"\"FLscroll\";\"FLTK:Containers\";\"A FLTK opcode that adds scroll bars to an area.\";\"FLscroll iwidth, iheight [, ix] [, iy]\"\n"
"\"FLscrollEnd\";\"FLTK:Containers\";\"A FLTK opcode that marks the end of an area with scrollbars.\";\"FLscrollEnd\"\n"
"\"FLtabs\";\"FLTK:Containers\";\"Creates a tabbed FLTK interface.\";\"FLtabs iwidth, iheight, ix, iy\"\n"
"\"FLtabsEnd\";\"FLTK:Containers\";\"Marks the end of a tabbed FLTK interface.\";\"FLtabsEnd\"\n"
"\"FLcount\";\"FLTK:Valuators\";\"A FLTK widget opcode that creates a counter.\";\"kout, ihandle FLcount \"label\", imin, imax, istep1, istep2, itype,  iwidth, iheight, ix, iy, iopcode [, kp1] [, kp2] [, kp3] [...] [, kpN]\"\n"
"\"FLjoy\";\"FLTK:Valuators\";\"A FLTK opcode that acts like a joystick.\";\"koutx, kouty, ihandlex, ihandley FLjoy \"label\", iminx, imaxx, iminy,  imaxy, iexpx, iexpy, idispx, idispy, iwidth, iheight, ix, iy\"\n"
"\"FLknob\";\"FLTK:Valuators\";\"A FLTK widget opcode that creates a knob.\";\"kout, ihandle FLknob \"label\", imin, imax, iexp, itype, idisp, iwidth,  ix, iy [, icursorsize]\"\n"
"\"FLroller\";\"FLTK:Valuators\";\"A FLTK widget that creates a transversal knob.\";\"kout, ihandle FLroller \"label\", imin, imax, istep, iexp, itype, idisp,  iwidth, iheight, ix, iy\"\n"
"\"FLslider\";\"FLTK:Valuators\";\"Puts a slider into the corresponding FLTK container.\";\"kout, ihandle FLslider \"label\", imin, imax, iexp, itype, idisp, iwidth,  iheight, ix, iy\"\n"
"\"FLtext\";\"FLTK:Valuators\";\"A FLTK widget opcode that creates a textbox.\";\"kout, ihandle FLtext \"label\", imin, imax, istep, itype, iwidth,  iheight, ix, iy\"\n"
"\"FLbox\";\"FLTK:Other\";\"A FLTK widget that displays text inside of a box.\";\"ihandle FLbox \"label\", itype, ifont, isize, iwidth, iheight, ix, iy [, image]\"\n"
"\"FLbutBank\";\"FLTK:Other\";\"A FLTK widget opcode that creates a bank of buttons.\";\"kout, ihandle FLbutBank itype, inumx, inumy, iwidth, iheight, ix, iy,  iopcode [, kp1] [, kp2] [, kp3] [, kp4] [, kp5] [....] [, kpN]\"\n"
"\"FLbutton\";\"FLTK:Other\";\"A FLTK widget opcode that creates a button.\";\"kout, ihandle FLbutton \"label\", ion, ioff, itype, iwidth, iheight, ix,  iy, iopcode [, kp1] [, kp2] [, kp3] [, kp4] [, kp5] [....] [, kpN]\"\n"
"\"FLcloseButton\";\"FLTK:Other\";\"A FLTK widget opcode that creates a button that will close the panel window it is a part of.\";\"ihandle FLcloseButton \"label\", iwidth, iheight, ix, iy\"\n"
"\"FLexecButton\";\"FLTK:Other\";\"A FLTK widget opcode that creates a button that executes a command.\";\"ihandle FLexecButton \"command\", iwidth, iheight, ix, iy\"\n"
"\"FLgetsnap\";\"FLTK:Other\";\"Retrieves a previously stored FLTK snapshot.\";\"inumsnap FLgetsnap index [, igroup]\"\n"
"\"FLhvsBox\";\"FLTK:Other\";\"Displays a box with a grid useful for visualizing two-dimensional Hyper Vectorial Synthesis.\";\"ihandle FLhvsBox inumlinesX, inumlinesY, iwidth, iheight, ix, iy [, image]\"\n"
"\"FLkeyIn\";\"FLTK:Other\";\"Reports keys pressed (on alphanumeric keyboard) when an FLTK panel has focus.\";\"kascii FLkeyIn [ifn]\"\n"
"\"FLloadsnap\";\"FLTK:Other\";\"Loads all snapshots into the memory bank of the current orchestra.\";\"FLloadsnap \"filename\" [, igroup]\"\n"
"\"FLmouse\";\"FLTK:Other\";\"Returns the mouse position and the state of the three mouse buttons.\";\"kx, ky, kb1, kb2, kb3 FLmouse [imode]\"\n"
"\"FLprintk\";\"FLTK:Other\";\"A FLTK opcode that prints a k-rate value at specified intervals.\";\"FLprintk itime, kval, idisp\"\n"
"\"FLprintk2\";\"FLTK:Other\";\"A FLTK opcode that prints a new value every time a control-rate variable changes.\";\"FLprintk2 kval, idisp\"\n"
"\"FLrun\";\"FLTK:Other\";\"Starts the FLTK widget thread.\";\"FLrun\"\n"
"\"FLsavesnap\";\"FLTK:Other\";\"Saves all snapshots currently created into a file.\";\"FLsavesnap \"filename\" [, igroup]\"\n"
"\"FLsetsnap\";\"FLTK:Other\";\"Stores the current status of all FLTK valuators into a snapshot location.\";\"inumsnap, inumval FLsetsnap index [, ifn, igroup]\"\n"
"\"FLsetSnapGroup\";\"FLTK:Other\";\"Determines the snapshot group for FL valuators.\";\"FLsetSnapGroup igroup\"\n"
"\"FLsetVal\";\"FLTK:Other\";\"Sets the value of a FLTK valuator at control-rate.\";\"FLsetVal ktrig, kvalue, ihandle\"\n"
"\"FLsetVal_i\";\"FLTK:Other\";\"Sets the value of a FLTK valuator to a number provided by the user.\";\"FLsetVal_i ivalue, ihandle\"\n"
"\"FLslidBnk\";\"FLTK:Other\";\"A FLTK widget containing a bank of horizontal sliders.\";\"FLslidBnk \"names\", inumsliders [, ioutable] [, iwidth] [, iheight] [, ix]  [, iy] [, itypetable] [, iexptable] [, istart_index] [, iminmaxtable]\"\n"
"\"FLslidBnk2\";\"FLTK:Other\";\"A FLTK widget containing a bank of horizontal sliders.\";\"FLslidBnk2 \"names\", inumsliders, ioutable, iconfigtable [,iwidth, iheight, ix, iy, istart_index]\"\n"
"\"FLslidBnk2Set\";\"FLTK:Other\";\"modify the values of a slider bank.\";\"FLslidBnk2Set ihandle, ifn [, istartIndex, istartSlid, inumSlid]\"\n"
"\"FLslidBnk2Setk\";\"FLTK:Other\";\"modify the values of a slider bank.\";\"FLslidBnk2Setk ktrig, ihandle, ifn [, istartIndex, istartSlid, inumSlid]\"\n"
"\"FLslidBnkGetHandle\";\"FLTK:Other\";\"gets the handle of last slider bank created.\";\"ihandle FLslidBnkGetHandle\"\n"
"\"FLslidBnkSet\";\"FLTK:Other\";\"modify the values of a slider bank.\";\"FLslidBnkSet ihandle, ifn [, istartIndex, istartSlid, inumSlid]\"\n"
"\"FLslidBnkSetk\";\"FLTK:Other\";\"modify the values of a slider bank.\";\"FLslidBnkSetk ktrig, ihandle, ifn [, istartIndex, istartSlid, inumSlid]\"\n"
"\"FLupdate\";\"FLTK:Other\";\"Same as the FLrun opcode.\";\"FLupdate\"\n"
"\"FLvalue\";\"FLTK:Other\";\"Shows the current value of a FLTK valuator.\";\"ihandle FLvalue \"label\", iwidth, iheight, ix, iy\"\n"
"\"FLvkeybd\";\"FLTK:Other\";\"An FLTK widget opcode that creates a virtual keyboard widget.\";\"FLvkeybd \"keyboard.map\", iwidth, iheight, ix, iy\"\n"
"\"FLvslidBnk\";\"FLTK:Other\";\"A FLTK widget containing a bank of vertical sliders.\";\"FLvslidBnk \"names\", inumsliders [, ioutable] [, iwidth] [, iheight] [, ix]  [, iy] [, itypetable] [, iexptable] [, istart_index] [, iminmaxtable]\"\n"
"\"FLvslidBnk2\";\"FLTK:Other\";\"A FLTK widget containing a bank of vertical sliders.\";\"FLvslidBnk2 \"names\", inumsliders, ioutable, iconfigtable [,iwidth, iheight, ix, iy, istart_index]\"\n"
"\"FLxyin\";\"FLTK:Other\";\"Senses the mouse cursor position in a user-defined area inside an FLpanel.\";\"koutx, kouty, kinside FLxyin ioutx_min, ioutx_max, iouty_min, iouty_max,  iwindx_min, iwindx_max, iwindy_min, iwindy_max [, iexpx, iexpy, ioutx, iouty]\"\n"
"\"vphaseseg\";\"FLTK:Other\";\"Allows one-dimensional HVS (Hyper-Vectorial Synthesis).\";\"vphaseseg kphase, ioutab, ielems, itab1,idist1,itab2  [,idist2,itab3, ... ,idistN-1,itabN]\"\n"
"\"FLcolor\";\"FLTK:Appearance\";\"A FLTK opcode that sets the primary colors.\";\"FLcolor ired, igreen, iblue [, ired2, igreen2, iblue2]\"\n"
"\"FLcolor2\";\"FLTK:Appearance\";\"A FLTK opcode that sets the secondary (selection) color.\";\"FLcolor2 ired, igreen, iblue\"\n"
"\"FLhide\";\"FLTK:Appearance\";\"Hides the target FLTK widget.\";\"FLhide ihandle\"\n"
"\"FLlabel\";\"FLTK:Appearance\";\"A FLTK opcode that modifies the appearance of a text label.\";\"FLlabel isize, ifont, ialign, ired, igreen, iblue\"\n"
"\"FLsetAlign\";\"FLTK:Appearance\";\"Sets the text alignment of a label of a FLTK widget.\";\"FLsetAlign ialign, ihandle\"\n"
"\"FLsetBox\";\"FLTK:Appearance\";\"Sets the appearance of a box surrounding a FLTK widget.\";\"FLsetBox itype, ihandle\"\n"
"\"FLsetColor\";\"FLTK:Appearance\";\"Sets the primary color of a FLTK widget.\";\"FLsetColor ired, igreen, iblue, ihandle\"\n"
"\"FLsetColor2\";\"FLTK:Appearance\";\"Sets the secondary (or selection) color of a FLTK widget.\";\"FLsetColor2 ired, igreen, iblue, ihandle\"\n"
"\"FLsetFont\";\"FLTK:Appearance\";\"Sets the font type of a FLTK widget.\";\"FLsetFont ifont, ihandle\"\n"
"\"FLsetPosition\";\"FLTK:Appearance\";\"Sets the position of a FLTK widget.\";\"FLsetPosition ix, iy, ihandle\"\n"
"\"FLsetSize\";\"FLTK:Appearance\";\"Resizes a FLTK widget.\";\"FLsetSize iwidth, iheight, ihandle\"\n"
"\"FLsetText\";\"FLTK:Appearance\";\"Sets the label of a FLTK widget.\";\"FLsetText \"itext\", ihandle\"\n"
"\"FLsetTextColor\";\"FLTK:Appearance\";\"Sets the color of the text label of a FLTK widget.\";\"FLsetTextColor ired, iblue, igreen, ihandle\"\n"
"\"FLsetTextSize\";\"FLTK:Appearance\";\"Sets the size of the text label of a FLTK widget.\";\"FLsetTextSize isize, ihandle\"\n"
"\"FLsetTextType\";\"FLTK:Appearance\";\"Sets some font attributes of the text label of a FLTK widget.\";\"FLsetTextType itype, ihandle\"\n"
"\"FLshow\";\"FLTK:Appearance\";\"Restores the visibility of a previously hidden FLTK widget.\";\"FLshow ihandle\"\n"
"\"clear\";\"Mathematical Operations:Comparators and Accumulators\";\"Zeroes a list of audio signals.\";\"clear avar1 [, avar2] [, avar3] [...]\"\n"
"\"vincr\";\"Mathematical Operations:Comparators and Accumulators\";\"Accumulates audio signals.\";\"vincr accum, aincr\"\n"
"\"abs\";\"Mathematical Operations:Mathematical Functions\";\"Returns an absolute value.\";\"abs(x) (no rate restriction)\"\n"
"\"ceil\";\"Mathematical Operations:Mathematical Functions\";\"Returns the smallest integer not less than\";\"ceil(x) (init-, control-, or audio-rate arg allowed)\"\n"
"\"exp\";\"Mathematical Operations:Mathematical Functions\";\"Returns e raised to the x-th power.\";\"exp(x) (no rate restriction)\"\n"
"\"floor\";\"Mathematical Operations:Mathematical Functions\";\"Returns the largest integer not greater than\";\"floor(x) (init-, control-, or audio-rate arg allowed)\"\n"
"\"frac\";\"Mathematical Operations:Mathematical Functions\";\"Returns the fractional part of a decimal number.\";\"frac(x) (init-rate or control-rate args, also works at audio rate in Csound5)\"\n"
"\"int\";\"Mathematical Operations:Mathematical Functions\";\"Extracts an integer from a decimal number.\";\"int(x) (init-rate or control-rate, also works at audio rate in Csound5)\"\n"
"\"log\";\"Mathematical Operations:Mathematical Functions\";\"Returns a natural log.\";\"log(x) (no rate restriction)\"\n"
"\"log10\";\"Mathematical Operations:Mathematical Functions\";\"Returns a base 10 log.\";\"log10(x) (no rate restriction)\"\n"
"\"logbtwo\";\"Mathematical Operations:Mathematical Functions\";\"Performs a logarithmic base two calculation.\";\"logbtwo(x) (init-rate or control-rate args only)\"\n"
"\"powoftwo\";\"Mathematical Operations:Mathematical Functions\";\"Performs a power-of-two calculation.\";\"powoftwo(x) (init-rate or control-rate args only)\"\n"
"\"round\";\"Mathematical Operations:Mathematical Functions\";\"Returns the integer value nearest to\";\"round(x) (init-, control-, or audio-rate arg allowed)\"\n"
"\"sqrt\";\"Mathematical Operations:Mathematical Functions\";\"Returns a square root value.\";\"sqrt(x) (no rate restriction)\"\n"
"\"cos\";\"Mathematical Operations:Trigonometric Functions\";\"Performs a cosine function.\";\"cos(x) (no rate restriction)\"\n"
"\"cosh\";\"Mathematical Operations:Trigonometric Functions\";\"Performs a hyperbolic cosine function.\";\"cosh(x) (no rate restriction)\"\n"
"\"cosinv\";\"Mathematical Operations:Trigonometric Functions\";\"Performs a arccosine function.\";\"cosinv(x) (no rate restriction)\"\n"
"\"sin\";\"Mathematical Operations:Trigonometric Functions\";\"Performs a sine function.\";\"sin(x) (no rate restriction)\"\n"
"\"sinh\";\"Mathematical Operations:Trigonometric Functions\";\"Performs a hyperbolic sine function.\";\"sinh(x) (no rate restriction)\"\n"
"\"sininv\";\"Mathematical Operations:Trigonometric Functions\";\"Performs an arcsine function.\";\"sininv(x) (no rate restriction)\"\n"
"\"tan\";\"Mathematical Operations:Trigonometric Functions\";\"Performs a tangent function.\";\"tan(x) (no rate restriction)\"\n"
"\"tanh\";\"Mathematical Operations:Trigonometric Functions\";\"Performs a hyperbolic tangent function.\";\"tanh(x) (no rate restriction)\"\n"
"\"taninv\";\"Mathematical Operations:Trigonometric Functions\";\"Performs an arctangent function.\";\"taninv(x) (no rate restriction)\"\n"
"\"ampdb\";\"Mathematical Operations:Amplitude Functions\";\"Returns the amplitude equivalent of the decibel value x.\";\"ampdb(x) (no rate restriction)\"\n"
"\"ampdbfs\";\"Mathematical Operations:Amplitude Functions\";\"Returns the amplitude equivalent (in 16-bit signed integer scale) of the full scale decibel (dB FS) value\";\"ampdbfs(x) (no rate restriction)\"\n"
"\"db\";\"Mathematical Operations:Amplitude Functions\";\"Returns the amplitude equivalent for a given decibel amount.\";\"db(x)\"\n"
"\"dbamp\";\"Mathematical Operations:Amplitude Functions\";\"Returns the decibel equivalent of the raw amplitude\";\"dbamp(x) (init-rate or control-rate args only)\"\n"
"\"dbfsamp\";\"Mathematical Operations:Amplitude Functions\";\"Returns the decibel equivalent of the raw amplitude\";\"dbfsamp(x) (init-rate or control-rate args only)\"\n"
"\"birnd\";\"Mathematical Operations:Random Functions\";\"Returns a random number in a bi-polar range.\";\"birnd(x) (init- or control-rate only)\"\n"
"\"rnd\";\"Mathematical Operations:Random Functions\";\"Returns a random number in a unipolar range at the rate given by the input argument.\";\"rnd(x) (init- or control-rate only)\"\n"
"\"divz\";\"Mathematical Operations:Opcode Equivalents of Functions\";\"Safely divides two numbers.\";\"ares divz xa, xb, ksubst\"\n"
"\"mac\";\"Mathematical Operations:Opcode Equivalents of Functions\";\"Multiplies and accumulates a- and k-rate signals.\";\"ares mac ksig1, asig1 [, ksig2] [, asig2] [, ksig3] [, asig3] [...]\"\n"
"\"maca\";\"Mathematical Operations:Opcode Equivalents of Functions\";\"Multiply and accumulate a-rate signals only.\";\"ares maca asig1 , asig2 [, asig3] [, asig4] [, asig5] [...]\"\n"
"\"polynomial\";\"Mathematical Operations:Opcode Equivalents of Functions\";\"Efficiently evaluates a polynomial of arbitrary order.\";\"aout polynomial ain, k0 [, k1 [, k2 [...]]]\"\n"
"\"pow\";\"Mathematical Operations:Opcode Equivalents of Functions\";\"Computes one argument to the power of another argument.\";\"ares pow aarg, kpow [, inorm]\"\n"
"\"product\";\"Mathematical Operations:Opcode Equivalents of Functions\";\"Multiplies any number of a-rate signals.\";\"ares product asig1, asig2 [, asig3] [...]\"\n"
"\"sum\";\"Mathematical Operations:Opcode Equivalents of Functions\";\"Sums any number of a-rate signals.\";\"ares sum asig1 [, asig2] [, asig3] [...]\"\n"
"\"taninv2\";\"Mathematical Operations:Opcode Equivalents of Functions\";\"Returns an arctangent.\";\"ares taninv2 ay, ax\"\n"
"\"cent\";\"Pitch Converters:Functions\";\"Calculates a factor to raise/lower a frequency by a given amount of cents.\";\"cent(x)\"\n"
"\"cpsmidinn\";\"Pitch Converters:Functions\";\"Converts a Midi note number value to cycles-per-second.\";\"cpsmidinn (MidiNoteNumber) (init- or control-rate args only)\"\n"
"\"cpsoct\";\"Pitch Converters:Functions\";\"Converts an octave-point-decimal value to cycles-per-second.\";\"cpsoct (oct) (no rate restriction)\"\n"
"\"cpspch\";\"Pitch Converters:Functions\";\"Converts a pitch-class value to cycles-per-second.\";\"cpspch (pch) (init- or control-rate args only)\"\n"
"\"octave\";\"Pitch Converters:Functions\";\"Calculates a factor to raise/lower a frequency by a given amount of octaves.\";\"octave(x)\"\n"
"\"octcps\";\"Pitch Converters:Functions\";\"Converts a cycles-per-second value to octave-point-decimal.\";\"octcps (cps) (init- or control-rate args only)\"\n"
"\"octmidinn\";\"Pitch Converters:Functions\";\"Converts a Midi note number value to octave-point-decimal.\";\"octmidinn (MidiNoteNumber) (init- or control-rate args only)\"\n"
"\"octpch\";\"Pitch Converters:Functions\";\"Converts a pitch-class value to octave-point-decimal.\";\"octpch (pch) (init- or control-rate args only)\"\n"
"\"pchmidinn\";\"Pitch Converters:Functions\";\"Converts a Midi note number value to octave point pitch-class units.\";\"pchmidinn (MidiNoteNumber) (init- or control-rate args only)\"\n"
"\"pchoct\";\"Pitch Converters:Functions\";\"Converts an octave-point-decimal value to pitch-class.\";\"pchoct (oct) (init- or control-rate args only)\"\n"
"\"semitone\";\"Pitch Converters:Functions\";\"Calculates a factor to raise/lower a frequency by a given amount of semitones.\";\"semitone(x)\"\n"
"\"cps2pch\";\"Pitch Converters:Tuning Opcodes\";\"Converts a pitch-class value into cycles-per-second (Hz) for equal divisions of the octave.\";\"icps cps2pch ipch, iequal\"\n"
"\"cpstun\";\"Pitch Converters:Tuning Opcodes\";\"Returns micro-tuning values at k-rate.\";\"kcps cpstun ktrig, kindex, kfn\"\n"
"\"cpstuni\";\"Pitch Converters:Tuning Opcodes\";\"Returns micro-tuning values at init-rate.\";\"icps cpstuni index, ifn\"\n"
"\"cpsxpch\";\"Pitch Converters:Tuning Opcodes\";\"Converts a pitch-class value into cycles-per-second (Hz) for equal divisions of any interval.\";\"icps cpsxpch ipch, iequal, irepeat, ibase\"\n"
"\"aftouch\";\"Real-time MIDI:Input\";\"Get the current after-touch value for this channel.\";\"kaft aftouch [imin] [, imax]\"\n"
"\"chanctrl\";\"Real-time MIDI:Input\";\"Get the current value of a MIDI channel controller.\";\"ival chanctrl ichnl, ictlno [, ilow] [, ihigh]\"\n"
"\"ctrl14\";\"Real-time MIDI:Input\";\"Allows a floating-point 14-bit MIDI signal scaled with a minimum and a maximum range.\";\"idest ctrl14 ichan, ictlno1, ictlno2, imin, imax [, ifn]\"\n"
"\"ctrl21\";\"Real-time MIDI:Input\";\"Allows a floating-point 21-bit MIDI signal scaled with a minimum and a maximum range.\";\"idest ctrl21 ichan, ictlno1, ictlno2, ictlno3, imin, imax [, ifn]\"\n"
"\"ctrl7\";\"Real-time MIDI:Input\";\"Allows a floating-point 7-bit MIDI signal scaled with a minimum and a maximum range.\";\"idest ctrl7 ichan, ictlno, imin, imax [, ifn]\"\n"
"\"ctrlinit\";\"Real-time MIDI:Input\";\"Sets the initial values for a set of MIDI controllers.\";\"ctrlinit ichnl, ictlno1, ival1 [, ictlno2] [, ival2] [, ictlno3]  [, ival3] [,...ival32]\"\n"
"\"initc14\";\"Real-time MIDI:Input\";\"Initializes the controllers used to create a 14-bit MIDI value.\";\"initc14 ichan, ictlno1, ictlno2, ivalue\"\n"
"\"initc21\";\"Real-time MIDI:Input\";\"Initializes the controllers used to create a 21-bit MIDI value.\";\"initc21 ichan, ictlno1, ictlno2, ictlno3, ivalue\"\n"
"\"initc7\";\"Real-time MIDI:Input\";\"Initializes the controller used to create a 7-bit MIDI value.\";\"initc7 ichan, ictlno, ivalue\"\n"
"\"massign\";\"Real-time MIDI:Input\";\"Assigns a MIDI channel number to a Csound instrument.\";\"massign ichnl, insnum[, ireset]\"\n"
"\"midic14\";\"Real-time MIDI:Input\";\"Allows a floating-point 14-bit MIDI signal scaled with a minimum and a maximum range.\";\"idest midic14 ictlno1, ictlno2, imin, imax [, ifn]\"\n"
"\"midic21\";\"Real-time MIDI:Input\";\"Allows a floating-point 21-bit MIDI signal scaled with a minimum and a maximum range.\";\"idest midic21 ictlno1, ictlno2, ictlno3, imin, imax [, ifn]\"\n"
"\"midic7\";\"Real-time MIDI:Input\";\"Allows a floating-point 7-bit MIDI signal scaled with a minimum and a maximum range.\";\"idest midic7 ictlno, imin, imax [, ifn]\"\n"
"\"midictrl\";\"Real-time MIDI:Input\";\"Get the current value (0-127) of a specified MIDI controller.\";\"ival midictrl inum [, imin] [, imax]\"\n"
"\"notnum\";\"Real-time MIDI:Input\";\"Get a note number from a MIDI event.\";\"ival notnum\"\n"
"\"pchbend\";\"Real-time MIDI:Input\";\"Get the current pitch-bend value for this channel.\";\"ibend pchbend [imin] [, imax]\"\n"
"\"pgmassign\";\"Real-time MIDI:Input\";\"Assigns an instrument number to a specified MIDI program.\";\"pgmassign ipgm, inst[, ichn]\"\n"
"\"polyaft\";\"Real-time MIDI:Input\";\"Returns the polyphonic after-touch pressure of the selected note number.\";\"ires polyaft inote [, ilow] [, ihigh]\"\n"
"\"veloc\";\"Real-time MIDI:Input\";\"Get the velocity from a MIDI event.\";\"ival veloc [ilow] [, ihigh]\"\n"
"\"nrpn\";\"Real-time MIDI:Output\";\"Sends a Non-Registered Parameter Number to the MIDI OUT port.\";\"nrpn kchan, kparmnum, kparmvalue\"\n"
"\"outiat\";\"Real-time MIDI:Output\";\"Sends MIDI aftertouch messages at i-rate.\";\"outiat ichn, ivalue, imin, imax\"\n"
"\"outic\";\"Real-time MIDI:Output\";\"Sends MIDI controller output at i-rate.\";\"outic ichn, inum, ivalue, imin, imax\"\n"
"\"outic14\";\"Real-time MIDI:Output\";\"Sends 14-bit MIDI controller output at i-rate.\";\"outic14 ichn, imsb, ilsb, ivalue, imin, imax\"\n"
"\"outipat\";\"Real-time MIDI:Output\";\"Sends polyphonic MIDI aftertouch messages at i-rate.\";\"outipat ichn, inotenum, ivalue, imin, imax\"\n"
"\"outipb\";\"Real-time MIDI:Output\";\"Sends MIDI pitch-bend messages at i-rate.\";\"outipb ichn, ivalue, imin, imax\"\n"
"\"outipc\";\"Real-time MIDI:Output\";\"Sends MIDI program change messages at i-rate\";\"outipc ichn, iprog, imin, imax\"\n"
"\"outkat\";\"Real-time MIDI:Output\";\"Sends MIDI aftertouch messages at k-rate.\";\"outkat kchn, kvalue, kmin, kmax\"\n"
"\"outkc\";\"Real-time MIDI:Output\";\"Sends MIDI controller messages at k-rate.\";\"outkc kchn, knum, kvalue, kmin, kmax\"\n"
"\"outkc14\";\"Real-time MIDI:Output\";\"Sends 14-bit MIDI controller output at k-rate.\";\"outkc14 kchn, kmsb, klsb, kvalue, kmin, kmax\"\n"
"\"outkpat\";\"Real-time MIDI:Output\";\"Sends polyphonic MIDI aftertouch messages at k-rate.\";\"outkpat kchn, knotenum, kvalue, kmin, kmax\"\n"
"\"outkpb\";\"Real-time MIDI:Output\";\"Sends MIDI pitch-bend messages at k-rate.\";\"outkpb kchn, kvalue, kmin, kmax\"\n"
"\"outkpc\";\"Real-time MIDI:Output\";\"Sends MIDI program change messages at k-rate.\";\"outkpc kchn, kprog, kmin, kmax\"\n"
"\"ampmidi\";\"Real-time MIDI:Converters\";\"Get the velocity of the current MIDI event.\";\"iamp ampmidi iscal [, ifn]\"\n"
"\"ampmidid\";\"Real-time MIDI:Converters\";\"Musically map MIDI velocity to peak amplitude within a specified dynamic range in decibels.\";\"iamplitude ampmidid ivelocity, idecibels\"\n"
"\"cpsmidi\";\"Real-time MIDI:Converters\";\"Get the note number of the current MIDI event, expressed in cycles-per-second.\";\"icps cpsmidi\"\n"
"\"cpsmidib\";\"Real-time MIDI:Converters\";\"Get the note number of the current MIDI event and modify it by the current pitch-bend value, express it in cycles-per-second.\";\"icps cpsmidib [irange]\"\n"
"\"cpstmid\";\"Real-time MIDI:Converters\";\"Get a MIDI note number (allows customized micro-tuning scales).\";\"icps cpstmid ifn\"\n"
"\"octmidi\";\"Real-time MIDI:Converters\";\"Get the note number, in octave-point-decimal units, of the current MIDI event.\";\"ioct octmidi\"\n"
"\"octmidib\";\"Real-time MIDI:Converters\";\"Get the note number of the current MIDI event and modify it by the current pitch-bend value, express it in octave-point-decimal.\";\"ioct octmidib [irange]\"\n"
"\"pchmidi\";\"Real-time MIDI:Converters\";\"Get the note number of the current MIDI event, expressed in pitch-class units.\";\"ipch pchmidi\"\n"
"\"pchmidib\";\"Real-time MIDI:Converters\";\"Get the note number of the current MIDI event and modify it by the current pitch-bend value, express it in pitch-class units.\";\"ipch pchmidib [irange]\"\n"
"\"midiin\";\"Real-time MIDI:Generic I/O\";\"Returns a generic MIDI message received by the MIDI IN port.\";\"kstatus, kchan, kdata1, kdata2 midiin\"\n"
"\"midiout\";\"Real-time MIDI:Generic I/O\";\"Sends a generic MIDI message to the MIDI OUT port.\";\"midiout kstatus, kchan, kdata1, kdata2\"\n"
"\"release\";\"Real-time MIDI:Event Extenders\";\"Indicates whether a note is in its\";\"kflag release\"\n"
"\"xtratim\";\"Real-time MIDI:Event Extenders\";\"Extend the duration of real-time generated events.\";\"xtratim iextradur\"\n"
"\"midion\";\"Real-time MIDI:Note Output\";\"Generates MIDI note messages at k-rate.\";\"midion kchn, knum, kvel\"\n"
"\"midion2\";\"Real-time MIDI:Note Output\";\"Sends noteon and noteoff messages to the MIDI OUT port.\";\"midion2 kchn, knum, kvel, ktrig\"\n"
"\"moscil\";\"Real-time MIDI:Note Output\";\"Sends a stream of the MIDI notes.\";\"moscil kchn, knum, kvel, kdur, kpause\"\n"
"\"noteoff\";\"Real-time MIDI:Note Output\";\"Send a noteoff message to the MIDI OUT port.\";\"noteoff ichn, inum, ivel\"\n"
"\"noteon\";\"Real-time MIDI:Note Output\";\"Send a noteon message to the MIDI OUT port.\";\"noteon ichn, inum, ivel\"\n"
"\"noteondur\";\"Real-time MIDI:Note Output\";\"Sends a noteon and a noteoff MIDI message both with the same channel, number and velocity.\";\"noteondur ichn, inum, ivel, idur\"\n"
"\"noteondur2\";\"Real-time MIDI:Note Output\";\"Sends a noteon and a noteoff MIDI message both with the same channel, number and velocity.\";\"noteondur2 ichn, inum, ivel, idur\"\n"
"\"midichannelaftertouch\";\"Real-time MIDI:MIDI/Score Interoperability\";\"Gets a MIDI channel's aftertouch value.\";\"midichannelaftertouch xchannelaftertouch [, ilow] [, ihigh]\"\n"
"\"midichn\";\"Real-time MIDI:MIDI/Score Interoperability\";\"Returns the MIDI channel number from which the note was activated.\";\"ichn midichn\"\n"
"\"midicontrolchange\";\"Real-time MIDI:MIDI/Score Interoperability\";\"Gets a MIDI control change value.\";\"midicontrolchange xcontroller, xcontrollervalue [, ilow] [, ihigh]\"\n"
"\"mididefault\";\"Real-time MIDI:MIDI/Score Interoperability\";\"Changes values, depending on MIDI activation.\";\"mididefault xdefault, xvalue\"\n"
"\"midinoteoff\";\"Real-time MIDI:MIDI/Score Interoperability\";\"Gets a MIDI noteoff value.\";\"midinoteoff xkey, xvelocity\"\n"
"\"midinoteoncps\";\"Real-time MIDI:MIDI/Score Interoperability\";\"Gets a MIDI note number as a cycles-per-second frequency.\";\"midinoteoncps xcps, xvelocity\"\n"
"\"midinoteonkey\";\"Real-time MIDI:MIDI/Score Interoperability\";\"Gets a MIDI note number value.\";\"midinoteonkey xkey, xvelocity\"\n"
"\"midinoteonoct\";\"Real-time MIDI:MIDI/Score Interoperability\";\"Gets a MIDI note number value as octave-point-decimal value.\";\"midinoteonoct xoct, xvelocity\"\n"
"\"midinoteonpch\";\"Real-time MIDI:MIDI/Score Interoperability\";\"Gets a MIDI note number as a pitch-class value.\";\"midinoteonpch xpch, xvelocity\"\n"
"\"midipitchbend\";\"Real-time MIDI:MIDI/Score Interoperability\";\"Gets a MIDI pitchbend value.\";\"midipitchbend xpitchbend [, ilow] [, ihigh]\"\n"
"\"midipolyaftertouch\";\"Real-time MIDI:MIDI/Score Interoperability\";\"Gets a MIDI polyphonic aftertouch value.\";\"midipolyaftertouch xpolyaftertouch, xcontrollervalue [, ilow] [, ihigh]\"\n"
"\"midiprogramchange\";\"Real-time MIDI:MIDI/Score Interoperability\";\"Gets a MIDI program change value.\";\"midiprogramchange xprogram\"\n"
"\"mclock\";\"Real-time MIDI:System Realtime\";\"Sends a MIDI CLOCK message.\";\"mclock ifreq\"\n"
"\"mrtmsg\";\"Real-time MIDI:System Realtime\";\"Send system real-time messages to the MIDI OUT port.\";\"mrtmsg imsgtype\"\n"
"\"s16b14\";\"Real-time MIDI:Slider Banks\";\"Creates a bank of 16 different 14-bit MIDI control message numbers.\";\"i1,...,i16 s16b14 ichan, ictlno_msb1, ictlno_lsb1, imin1, imax1,  initvalue1, ifn1,..., ictlno_msb16, ictlno_lsb16, imin16, imax16, initvalue16, ifn16\"\n"
"\"s32b14\";\"Real-time MIDI:Slider Banks\";\"Creates a bank of 32 different 14-bit MIDI control message numbers.\";\"i1,...,i32 s32b14 ichan, ictlno_msb1, ictlno_lsb1, imin1, imax1,  initvalue1, ifn1,..., ictlno_msb32, ictlno_lsb32, imin32, imax32, initvalue32, ifn32\"\n"
"\"slider16\";\"Real-time MIDI:Slider Banks\";\"Creates a bank of 16 different MIDI control message numbers.\";\"i1,...,i16 slider16 ichan, ictlnum1, imin1, imax1, init1, ifn1,...,  ictlnum16, imin16, imax16, init16, ifn16\"\n"
"\"slider16f\";\"Real-time MIDI:Slider Banks\";\"Creates a bank of 16 different MIDI control message numbers, filtered before output.\";\"k1,...,k16 slider16f ichan, ictlnum1, imin1, imax1, init1, ifn1,  icutoff1,..., ictlnum16, imin16, imax16, init16, ifn16, icutoff16\"\n"
"\"slider16table\";\"Real-time MIDI:Slider Banks\";\"Stores a bank of 16 different MIDI control messages to a table.\";\"kflag slider16table ichan, ioutTable, ioffset, ictlnum1, imin1, imax1,  init1, ifn1, .... , ictlnum16, imin16, imax16, init16, ifn16\"\n"
"\"slider16tablef\";\"Real-time MIDI:Slider Banks\";\"Stores a bank of 16 different MIDI control messages to a table, filtered before output.\";\"kflag slider16tablef ichan, ioutTable, ioffset, ictlnum1, imin1, imax1,  init1, ifn1, icutoff1, .... , ictlnum16, imin16, imax16, init16, ifn16, icutoff16\"\n"
"\"slider32\";\"Real-time MIDI:Slider Banks\";\"Creates a bank of 32 different MIDI control message numbers.\";\"i1,...,i32 slider32 ichan, ictlnum1, imin1, imax1, init1, ifn1,...,  ictlnum32, imin32, imax32, init32, ifn32\"\n"
"\"slider32f\";\"Real-time MIDI:Slider Banks\";\"Creates a bank of 32 different MIDI control message numbers, filtered before output.\";\"k1,...,k32 slider32f ichan, ictlnum1, imin1, imax1, init1, ifn1, icutoff1,  ..., ictlnum32, imin32, imax32, init32, ifn32, icutoff32\"\n"
"\"slider32table\";\"Real-time MIDI:Slider Banks\";\"Stores a bank of 32 different MIDI control messages to a table.\";\"kflag slider32table ichan, ioutTable, ioffset, ictlnum1, imin1,  imax1, init1, ifn1, .... , ictlnum32, imin32, imax32, init32, ifn32\"\n"
"\"slider32tablef\";\"Real-time MIDI:Slider Banks\";\"Stores a bank of 32 different MIDI control messages to a table, filtered before output.\";\"kflag slider32tablef ichan, ioutTable, ioffset, ictlnum1, imin1, imax1,  init1, ifn1, icutoff1, .... , ictlnum32, imin32, imax32, init32, ifn32, icutoff32\"\n"
"\"slider64\";\"Real-time MIDI:Slider Banks\";\"Creates a bank of 64 different MIDI control message numbers.\";\"i1,...,i64 slider64 ichan, ictlnum1, imin1, imax1, init1, ifn1,...,  ictlnum64, imin64, imax64, init64, ifn64\"\n"
"\"slider64f\";\"Real-time MIDI:Slider Banks\";\"Creates a bank of 64 different MIDI control message numbers, filtered before output.\";\"k1,...,k64 slider64f ichan, ictlnum1, imin1, imax1, init1, ifn1,  icutoff1,..., ictlnum64, imin64, imax64, init64, ifn64, icutoff64\"\n"
"\"slider64table\";\"Real-time MIDI:Slider Banks\";\"Stores a bank of 64 different MIDI control messages to a table.\";\"kflag slider64table ichan, ioutTable, ioffset, ictlnum1, imin1,  imax1, init1, ifn1, .... , ictlnum64, imin64, imax64, init64, ifn64\"\n"
"\"slider64tablef\";\"Real-time MIDI:Slider Banks\";\"Stores a bank of 64 different MIDI control messages to a table, filtered before output.\";\"kflag slider64tablef ichan, ioutTable, ioffset, ictlnum1, imin1, imax1,  init1, ifn1, icutoff1, .... , ictlnum64, imin64, imax64, init64, ifn64, icutoff64\"\n"
"\"slider8\";\"Real-time MIDI:Slider Banks\";\"Creates a bank of 8 different MIDI control message numbers.\";\"i1,...,i8 slider8 ichan, ictlnum1, imin1, imax1, init1, ifn1,...,  ictlnum8, imin8, imax8, init8, ifn8\"\n"
"\"slider8f\";\"Real-time MIDI:Slider Banks\";\"Creates a bank of 8 different MIDI control message numbers, filtered before output.\";\"k1,...,k8 slider8f ichan, ictlnum1, imin1, imax1, init1, ifn1, icutoff1,  ..., ictlnum8, imin8, imax8, init8, ifn8, icutoff8\"\n"
"\"slider8table\";\"Real-time MIDI:Slider Banks\";\"Stores a bank of 8 different MIDI control messages to a table.\";\"kflag slider8table ichan, ioutTable, ioffset, ictlnum1, imin1, imax1,  init1, ifn1,..., ictlnum8, imin8, imax8, init8, ifn8\"\n"
"\"slider8tablef\";\"Real-time MIDI:Slider Banks\";\"Stores a bank of 8 different MIDI control messages to a table, filtered before output.\";\"kflag slider8tablef ichan, ioutTable, ioffset, ictlnum1, imin1, imax1,  init1, ifn1, icutoff1, .... , ictlnum8, imin8, imax8, init8, ifn8, icutoff8\"\n"
"\"sliderKawai\";\"Real-time MIDI:Slider Banks\";\"Creates a bank of 16 different MIDI control message numbers from a KAWAI MM-16 midi mixer.\";\"k1, k2, ...., k16 sliderKawai imin1, imax1, init1, ifn1,  imin2, imax2, init2, ifn2, ..., imin16, imax16, init16, ifn16\"\n"
"\"alwayson\";\"Signal Flow Graph Opcodes\";\"Activates the indicated instrument in the orchestra header, without need for an i statement.\";\"alwayson Tinstrument [p4, ..., pn]\"\n"
"\"connect \";\"Signal Flow Graph Opcodes\";\"Connects a source outlet to a sink inlet.\";\"connect Tsource1, Soutlet1, Tsink1, Sinlet1\"\n"
"\"ftgenonce\";\"Signal Flow Graph Opcodes\";\"Generate a score function table from within the orchestra, which is deleted at the end of the note.\";\"ifno ftgenonce ip1, ip2dummy, isize, igen, iarga, iargb, ...\"\n"
"\"inleta \";\"Signal Flow Graph Opcodes\";\"Receives an arate signal into an instrument through a named port.\";\"asignal inleta Sname\"\n"
"\"inletf \";\"Signal Flow Graph Opcodes\";\"Receives an frate signal (fsig) into an instrument from a named port.\";\"fsignal inletf Sname\"\n"
"\"inletk \";\"Signal Flow Graph Opcodes\";\"Receives a krate signal into an instrument from a named port.\";\"ksignal inletk Sname\"\n"
"\"inletkid \";\"Signal Flow Graph Opcodes\";\"Receives a krate signal into an instrument from a named port.\";\"ksignal inletkid Sname, SinstanceID\"\n"
"\"outleta \";\"Signal Flow Graph Opcodes\";\"Sends an arate signal out from an instrument to a named port.\";\"outleta Sname, asignal\"\n"
"\"outletf \";\"Signal Flow Graph Opcodes\";\"Sends a frate signal (fsig) out from an instrument to a named port.\";\"outletf Sname, fsignal\"\n"
"\"outletk \";\"Signal Flow Graph Opcodes\";\"Sends a krate signal out from an instrument to a named port.\";\"outletk Sname, ksignal\"\n"
"\"outletkid \";\"Signal Flow Graph Opcodes\";\"Sends a krate signal out from an instrument to a named port.\";\"outletkid Sname, SinstanceID, ksignal\"\n"
"\"ktableseg\";\"Spectral Processing:STFT\";\"Deprecated.\";\"ktableseg ifn1, idur1, ifn2 [, idur2] [, ifn3] [...]\"\n"
"\"pvadd\";\"Spectral Processing:STFT\";\"Reads from a\";\"ares pvadd ktimpnt, kfmod, ifilcod, ifn, ibins [, ibinoffset]  [, ibinincr] [, iextractmode] [, ifreqlim] [, igatefn]\"\n"
"\"pvbufread\";\"Spectral Processing:STFT\";\"Reads from a phase vocoder analysis file and makes the retrieved data available.\";\"pvbufread ktimpnt, ifile\"\n"
"\"pvcross\";\"Spectral Processing:STFT\";\"Applies the amplitudes from one phase vocoder analysis file to the data from a second file.\";\"ares pvcross ktimpnt, kfmod, ifile, kampscale1, kampscale2 [, ispecwp]\"\n"
"\"pvinterp\";\"Spectral Processing:STFT\";\"Interpolates between the amplitudes and frequencies of two phase vocoder analysis files.\";\"ares pvinterp ktimpnt, kfmod, ifile, kfreqscale1, kfreqscale2,  kampscale1, kampscale2, kfreqinterp, kampinterp\"\n"
"\"pvoc\";\"Spectral Processing:STFT\";\"Implements signal reconstruction using an fft-based phase vocoder.\";\"ares pvoc ktimpnt, kfmod, ifilcod [, ispecwp] [, iextractmode]  [, ifreqlim] [, igatefn]\"\n"
"\"pvread\";\"Spectral Processing:STFT\";\"Reads from a phase vocoder analysis file and returns the frequency and amplitude from a single analysis channel or bin.\";\"kfreq, kamp pvread ktimpnt, ifile, ibin\"\n"
"\"tableseg\";\"Spectral Processing:STFT\";\"Creates a new function table by making linear segments between values in stored function tables.\";\"tableseg ifn1, idur1, ifn2 [, idur2] [, ifn3] [...]\"\n"
"\"tablexseg\";\"Spectral Processing:STFT\";\"Creates a new function table by making exponential segments between values in stored function tables.\";\"tablexseg ifn1, idur1, ifn2 [, idur2] [, ifn3] [...]\"\n"
"\"vpvoc\";\"Spectral Processing:STFT\";\"Implements signal reconstruction using an fft-based phase vocoder and an extra envelope.\";\"ares vpvoc ktimpnt, kfmod, ifile [, ispecwp] [, ifn]\"\n"
"\"lpfreson\";\"Spectral Processing:LPC\";\"Resynthesises a signal from the data passed internally by a previous lpread, applying formant shifting.\";\"ares lpfreson asig, kfrqratio\"\n"
"\"lpinterp\";\"Spectral Processing:LPC\";\"Computes a new set of poles from the interpolation between two analysis.\";\"lpinterp islot1, islot2, kmix\"\n"
"\"lpread\";\"Spectral Processing:LPC\";\"Reads a control file of time-ordered information frames.\";\"krmsr, krmso, kerr, kcps lpread ktimpnt, ifilcod [, inpoles] [, ifrmrate]\"\n"
"\"lpreson\";\"Spectral Processing:LPC\";\"Resynthesises a signal from the data passed internally by a previous lpread.\";\"ares lpreson asig\"\n"
"\"lpslot\";\"Spectral Processing:LPC\";\"Selects the slot to be use by further lp opcodes.\";\"lpslot islot\"\n"
"\"specaddm\";\"Spectral Processing:Non-Standard\";\"Perform a weighted add of two input spectra.\";\"wsig specaddm wsig1, wsig2 [, imul2]\"\n"
"\"specdiff\";\"Spectral Processing:Non-Standard\";\"Finds the positive difference values between consecutive spectral frames.\";\"wsig specdiff wsigin\"\n"
"\"specdisp\";\"Spectral Processing:Non-Standard\";\"Displays the magnitude values of the spectrum.\";\"specdisp wsig, iprd [, iwtflg]\"\n"
"\"specfilt\";\"Spectral Processing:Non-Standard\";\"Filters each channel of an input spectrum.\";\"wsig specfilt wsigin, ifhtim\"\n"
"\"spechist\";\"Spectral Processing:Non-Standard\";\"Accumulates the values of successive spectral frames.\";\"wsig spechist wsigin\"\n"
"\"specptrk\";\"Spectral Processing:Non-Standard\";\"Estimates the pitch of the most prominent complex tone in the spectrum.\";\"koct, kamp specptrk wsig, kvar, ilo, ihi, istr, idbthresh, inptls,  irolloff [, iodd] [, iconfs] [, interp] [, ifprd] [, iwtflg]\"\n"
"\"specscal\";\"Spectral Processing:Non-Standard\";\"Scales an input spectral datablock with spectral envelopes.\";\"wsig specscal wsigin, ifscale, ifthresh\"\n"
"\"specsum\";\"Spectral Processing:Non-Standard\";\"Sums the magnitudes across all channels of the spectrum.\";\"ksum specsum wsig [, interp]\"\n"
"\"spectrum\";\"Spectral Processing:Non-Standard\";\"Generate a constant-Q, exponentially-spaced DFT.\";\"wsig spectrum xsig, iprd, iocts, ifrqa [, iq] [, ihann] [, idbout]  [, idsprd] [, idsinrs]\"\n"
"\"binit\";\"Spectral Processing:Streaming\";\"PVS tracks to amplitude+frequency conversion.\";\"fsig binit fin, isize\"\n"
"\"partials\";\"Spectral Processing:Streaming\";\"Partial track spectral analysis.\";\"ftrks partials ffr, fphs, kthresh, kminpts, kmaxgap, imaxtracks\"\n"
"\"pvs2tab\";\"Spectral Processing:Streaming\";\"Copies spectral data to t-variables.\";\"kframe pvs2tab tvar, fsig\"\n"
"\"pvsadsyn\";\"Spectral Processing:Streaming\";\"Resynthesize using a fast oscillator-bank.\";\"ares pvsadsyn fsrc, inoscs, kfmod [, ibinoffset] [, ibinincr] [, iinit]\"\n"
"\"pvsanal\";\"Spectral Processing:Streaming\";\"Generate an fsig from a mono audio source ain, using phase vocoder overlap-add analysis.\";\"fsig pvsanal ain, ifftsize, ioverlap, iwinsize, iwintype [, iformat] [, iinit]\"\n"
"\"pvsarp\";\"Spectral Processing:Streaming\";\"Arpeggiate the spectral components of a streaming pv signal.\";\"fsig pvsarp fsigin, kbin, kdepth, kgain\"\n"
"\"pvsbandp\";\"Spectral Processing:Streaming\";\"A band pass filter working in the spectral domain.\";\"fsig pvsbandp fsigin, xlowcut, xlowfull,  xhighfull, xhighcut[, ktype]\"\n"
"\"pvsbandr\";\"Spectral Processing:Streaming\";\"A band reject filter working in the spectral domain.\";\"fsig pvsbandr fsigin, xlowcut, xlowfull,  xhighfull, xhighcut[, ktype]\"\n"
"\"pvsbin\";\"Spectral Processing:Streaming\";\"Obtain the amp and freq values off a PVS signal bin.\";\"kamp, kfr pvsbin fsig, kbin\"\n"
"\"pvsblur\";\"Spectral Processing:Streaming\";\"Average the amp/freq time functions of each analysis channel for a specified time.\";\"fsig pvsblur fsigin, kblurtime, imaxdel\"\n"
"\"pvsbuffer\";\"Spectral Processing:Streaming\";\"This opcode creates and writes to a circular buffer for f-signals (streaming PV signals).\";\"ihandle, ktime pvsbuffer fsig, ilen\"\n"
"\"pvsbufread\";\"Spectral Processing:Streaming\";\"This opcode reads a circular buffer of f-signals (streaming PV signals).\";\"fsig pvsbufread ktime, khandle[, ilo, ihi, iclear]\"\n"
"\"pvsbufread2\";\"Spectral Processing:Streaming\";\"This opcode reads a circular buffer of f-signals (streaming PV signals), with binwise additional delays.\";\"fsig pvsbufread2 ktime, khandle, ift1, ift2\"\n"
"\"pvscale\";\"Spectral Processing:Streaming\";\"Scale the frequency components of a pv stream.\";\"fsig pvscale fsigin, kscal[, kkeepform, kgain, kcoefs]\"\n"
"\"pvscent\";\"Spectral Processing:Streaming\";\"Calculate the spectral centroid of a signal.\";\"kcent pvscent fsig\"\n"
"\"pvscross\";\"Spectral Processing:Streaming\";\"Performs cross-synthesis between two source fsigs.\";\"fsig pvscross fsrc, fdest, kamp1, kamp2\"\n"
"\"pvsdemix\";\"Spectral Processing:Streaming\";\"Spectral azimuth-based de-mixing of stereo sources.\";\"fsig pvsdemix fleft, fright, kpos, kwidth, ipoints\"\n"
"\"pvsdiskin\";\"Spectral Processing:Streaming\";\"Read a selected channel from a PVOC-EX analysis file.\";\"fsig pvsdiskin SFname,ktscal,kgain[,ioffset, ichan]\"\n"
"\"pvsdisp\";\"Spectral Processing:Streaming\";\"Displays a PVS signal as an amplitude vs. freq graph.\";\"pvsdisp fsig[, ibins, iwtflg]\"\n"
"\"pvsfilter\";\"Spectral Processing:Streaming\";\"Multiply amplitudes of a pvoc stream by those of a second pvoc stream, with dynamic scaling.\";\"fsig pvsfilter fsigin, fsigfil, kdepth[, igain]\"\n"
"\"pvsfread\";\"Spectral Processing:Streaming\";\"Read a selected channel from a PVOC-EX analysis file.\";\"fsig pvsfread ktimpt, ifn [, ichan]\"\n"
"\"pvsfreeze\";\"Spectral Processing:Streaming\";\"Freeze the amplitude and frequency time functions of a pv stream according to a control-rate trigger.\";\"fsig pvsfreeze fsigin, kfreeza, kfreezf\"\n"
"\"pvsftr\";\"Spectral Processing:Streaming\";\"Reads amplitude and/or frequency data from function tables.\";\"pvsftr fsrc, ifna [, ifnf]\"\n"
"\"pvsftw\";\"Spectral Processing:Streaming\";\"Writes amplitude and/or frequency data to function tables.\";\"kflag pvsftw fsrc, ifna [, ifnf]\"\n"
"\"pvsfwrite\";\"Spectral Processing:Streaming\";\"Write a fsig to a PVOCEX file.\";\"pvsfwrite fsig, ifile\"\n"
"\"pvsgain\";\"Spectral Processing:Streaming\";\"Scale the amplitude of a pv stream.\";\"fsig pvsgain fsigin, kgain\"\n"
"\"pvshift\";\"Spectral Processing:Streaming\";\"Shift the frequency components of a pv stream, stretching/compressing its spectrum.\";\"fsig pvshift fsigin, kshift, klowest[, kkeepform, igain, kcoefs]\"\n"
"\"pvsifd\";\"Spectral Processing:Streaming\";\"Instantaneous Frequency Distribution, magnitude and phase analysis.\";\"ffr,fphs pvsifd ain, ifftsize, ihopsize, iwintype[,iscal]\"\n"
"\"pvsin\";\"Spectral Processing:Streaming\";\"Retrieve an fsig from the input software bus, a pvs equivalent to chani.\";\"fsig pvsin kchan[, isize, iolap, iwinsize, iwintype, iformat]\"\n"
"\"pvsinfo\";\"Spectral Processing:Streaming\";\"Get information from a PVOC-EX formatted source.\";\"ioverlap, inumbins, iwinsize, iformat pvsinfo fsrc\"\n"
"\"pvsinit\";\"Spectral Processing:Streaming\";\"Initialise a spectral (f) variable to zero.\";\"fsig pvsinit isize[, iolap, iwinsize, iwintype, iformat]\"\n"
"\"pvslock\";\"Spectral Processing:Streaming\";\"Frequency lock an input fsig\";\"fsig pvslock fsigin, klock\"\n"
"\"pvsmaska\";\"Spectral Processing:Streaming\";\"Modify amplitudes using a function table, with dynamic scaling.\";\"fsig pvsmaska fsrc, ifn, kdepth\"\n"
"\"pvsmix\";\"Spectral Processing:Streaming\";\"Mix 'seamlessly' two pv signals.\";\"fsig pvsmix fsigin1, fsigin2\"\n"
"\"pvsmooth\";\"Spectral Processing:Streaming\";\"Smooth the amplitude and frequency time functions of a pv stream using parallel 1st order lowpass IIR filters with time-varying cutoff frequency.\";\"fsig pvsmooth fsigin, kacf, kfcf\"\n"
"\"pvsmorph\";\"Spectral Processing:Streaming\";\"Performs morphing (or interpolation) between two source fsigs.\";\"fsig pvsmorph fsig1, fsig2, kampint, kfrqint\"\n"
"\"pvsosc\";\"Spectral Processing:Streaming\";\"PVS-based oscillator simulator.\";\"fsig pvsosc kamp, kfreq, ktype, isize [,ioverlap] [, iwinsize] [, iwintype] [, iformat]\"\n"
"\"pvsout\";\"Spectral Processing:Streaming\";\"Write a fsig to the pvs output bus.\";\"pvsout fsig, kchan\"\n"
"\"pvspitch\";\"Spectral Processing:Streaming\";\"Track the pitch and amplitude of a PVS signal.\";\"kfr, kamp pvspitch fsig, kthresh\"\n"
"\"pvstencil\";\"Spectral Processing:Streaming\";\"Transforms a pvoc stream according to a masking function table.\";\"fsig pvstencil fsigin, kgain, klevel, iftable\"\n"
"\"pvsvoc\";\"Spectral Processing:Streaming\";\"Combine the spectral envelope of one fsig with the excitation (frequencies) of another.\";\"fsig pvsvoc famp, fexc, kdepth, kgain [,kcoefs]\"\n"
"\"pvswarp\";\"Spectral Processing:Streaming\";\"Warp the spectral envelope of a PVS signal\";\"fsig pvswarp fsigin, kscal, kshift[, klowest, kmeth, kgain, kcoefs]\"\n"
"\"pvsynth\";\"Spectral Processing:Streaming\";\"Resynthesise using a FFT overlap-add.\";\"ares pvsynth fsrc, [iinit]\"\n"
"\"resyn\";\"Spectral Processing:Streaming\";\"Streaming partial track additive synthesis with cubic phase interpolation with pitch control and support for timescale-modified input\";\"asig resyn fin, kscal, kpitch, kmaxtracks, ifn\"\n"
"\"sinsyn\";\"Spectral Processing:Streaming\";\"Streaming partial track additive synthesis with cubic phase interpolation\";\"asig sinsyn fin, kscal, kmaxtracks, ifn\"\n"
"\"tab2pvs\";\"Spectral Processing:Streaming\";\"Copies spectral data from t-variables.\";\"fsig tab2pvs tvar[,ihopsize, iwinsize, iwintype]\"\n"
"\"tradsyn\";\"Spectral Processing:Streaming\";\"Streaming partial track additive synthesis\";\"asig tradsyn fin, kscal, kpitch, kmaxtracks, ifn\"\n"
"\"trcross\";\"Spectral Processing:Streaming\";\"Streaming partial track cross-synthesis.\";\"fsig trcross fin1, fin2, ksearch, kdepth [, kmode]\"\n"
"\"trfilter\";\"Spectral Processing:Streaming\";\"Streaming partial track filtering.\";\"fsig trfilter fin, kamnt, ifn\"\n"
"\"trhighest\";\"Spectral Processing:Streaming\";\"Extracts the highest-frequency track from a streaming track input signal.\";\"fsig, kfr, kamp trhighest fin1, kscal\"\n"
"\"trlowest\";\"Spectral Processing:Streaming\";\"Extracts the lowest-frequency track from a streaming track input signal.\";\"fsig, kfr, kamp trlowest fin1, kscal\"\n"
"\"trmix\";\"Spectral Processing:Streaming\";\"Streaming partial track mixing.\";\"fsig trmix fin1, fin2\"\n"
"\"trscale\";\"Spectral Processing:Streaming\";\"Streaming partial track frequency scaling.\";\"fsig trscale fin, kpitch[, kgain]\"\n"
"\"trshift\";\"Spectral Processing:Streaming\";\"Streaming partial track frequency scaling.\";\"fsig trshift fin, kpshift[, kgain]\"\n"
"\"trsplit\";\"Spectral Processing:Streaming\";\"Streaming partial track frequency splitting.\";\"fsiglow, fsighi trsplit fin, ksplit[, kgainlow, kgainhigh]\"\n"
"\"ATSadd\";\"Spectral Processing:ATS\";\"uses the data from an ATS analysis file to perform additive synthesis.\";\"ar ATSadd ktimepnt, kfmod, iatsfile, ifn, ipartials[, ipartialoffset,  ipartialincr, igatefn]\"\n"
"\"ATSaddnz\";\"Spectral Processing:ATS\";\"uses the data from an ATS analysis file to perform noise resynthesis.\";\"ar ATSaddnz ktimepnt, iatsfile, ibands[, ibandoffset, ibandincr]\"\n"
"\"ATSbufread\";\"Spectral Processing:ATS\";\"reads data from and ATS data file and stores it in an internal data table of frequency, amplitude pairs.\";\"ATSbufread ktimepnt, kfmod, iatsfile, ipartials[, ipartialoffset,  ipartialincr]\"\n"
"\"ATScross\";\"Spectral Processing:ATS\";\"perform cross synthesis from ATS analysis files.\";\"ar ATScross ktimepnt, kfmod, iatsfile, ifn, kmylev, kbuflev, ipartials  [, ipartialoffset, ipartialincr]\"\n"
"\"ATSinfo\";\"Spectral Processing:ATS\";\"reads data out of the header of an ATS file.\";\"idata ATSinfo iatsfile, ilocation\"\n"
"\"ATSinterpread\";\"Spectral Processing:ATS\";\"allows a user to determine the frequency envelope of any\";\"kamp ATSinterpread kfreq\"\n"
"\"ATSpartialtap\";\"Spectral Processing:ATS\";\"returns a frequency, amplitude pair from an\";\"kfrq, kamp ATSpartialtap ipartialnum\"\n"
"\"ATSread\";\"Spectral Processing:ATS\";\"reads data from an ATS file.\";\"kfreq, kamp ATSread ktimepnt, iatsfile, ipartial\"\n"
"\"ATSreadnz\";\"Spectral Processing:ATS\";\"reads data from an ATS file.\";\"kenergy ATSreadnz ktimepnt, iatsfile, iband\"\n"
"\"ATSsinnoi\";\"Spectral Processing:ATS\";\"uses the data from an ATS analysis file to perform resynthesis.\";\"ar ATSsinnoi ktimepnt, ksinlev, knzlev, kfmod, iatsfile, ipartials  [, ipartialoffset, ipartialincr]\"\n"
"\"lorismorph\";\"Spectral Processing:Loris\";\"Morphs two stored sets of bandwidth-enhanced partials and stores a new set of partials representing the morphed sound. The morph is performed by linearly interpolating the parameter envelopes (frequency, amplitude, and bandwidth, or noisiness) of the bandwidth-enhanced partials according to control-rate frequency, amplitude, and bandwidth morphing functions.\";\"lorismorph isrcidx, itgtidx, istoreidx, kfreqmorphenv, kampmorphenv, kbwmorphenv\"\n"
"\"lorisplay\";\"Spectral Processing:Loris\";\"renders a stored set of bandwidth-enhanced partials using the method of Bandwidth-Enhanced Additive Synthesis implemented in the Loris software, applying control-rate frequency, amplitude, and bandwidth scaling envelopes.\";\"ar lorisplay ireadidx, kfreqenv, kampenv, kbwenv\"\n"
"\"lorisread\";\"Spectral Processing:Loris\";\"Imports a set of bandwidth-enhanced partials from a SDIF-format data file, applying control-rate frequency, amplitude, and bandwidth scaling envelopes, and stores the modified partials in memory.\";\"lorisread ktimpnt, ifilcod, istoreidx, kfreqenv, kampenv, kbwenv[, ifadetime]\"\n"
"\"strget\";\"Strings:Definition\";\"Set string variable to value from strset table or string p-field\";\"Sdst strget indx\"\n"
"\"strset\";\"Strings:Definition\";\"Allows a string to be linked with a numeric value.\";\"strset iarg, istring\"\n"
"\"puts\";\"Strings:Manipulation\";\"Print a string constant or variable\";\"puts Sstr, ktrig[, inonl]\"\n"
"\"sprintf\";\"Strings:Manipulation\";\"printf-style formatted output to a string variable.\";\"Sdst sprintf Sfmt, xarg1[, xarg2[, ... ]]\"\n"
"\"sprintfk\";\"Strings:Manipulation\";\"printf-style formatted output to a string variable at k-rate.\";\"Sdst sprintfk Sfmt, xarg1[, xarg2[, ... ]]\"\n"
"\"strcat\";\"Strings:Manipulation\";\"Concatenate strings\";\"Sdst strcat Ssrc1, Ssrc2\"\n"
"\"strcatk\";\"Strings:Manipulation\";\"Concatenate strings (k-rate)\";\"Sdst strcatk Ssrc1, Ssrc2\"\n"
"\"strcmp\";\"Strings:Manipulation\";\"Compare strings\";\"ires strcmp S1, S2\"\n"
"\"strcmpk\";\"Strings:Manipulation\";\"Compare strings\";\"kres strcmpk S1, S2\"\n"
"\"strcpy\";\"Strings:Manipulation\";\"Assign value to a string variable\";\"Sdst strcpy Ssrc\"\n"
"\"strcpyk\";\"Strings:Manipulation\";\"Assign value to a string variable (k-rate)\";\"Sdst strcpyk Ssrc\"\n"
"\"strindex\";\"Strings:Manipulation\";\"Return the position of the first occurence of a string in another string\";\"ipos strindex S1, S2\"\n"
"\"strindexk\";\"Strings:Manipulation\";\"Return the position of the first occurence of a string in another string\";\"kpos strindexk S1, S2\"\n"
"\"strlen\";\"Strings:Manipulation\";\"Return the length of a string\";\"ilen strlen Sstr\"\n"
"\"strlenk\";\"Strings:Manipulation\";\"Return the length of a string\";\"klen strlenk Sstr\"\n"
"\"strrindex\";\"Strings:Manipulation\";\"Return the position of the last occurence of a string in another string\";\"ipos strrindex S1, S2\"\n"
"\"strrindexk\";\"Strings:Manipulation\";\"Return the position of the last occurence of a string in another string\";\"kpos strrindexk S1, S2\"\n"
"\"strsub\";\"Strings:Manipulation\";\"Extract a substring\";\"Sdst strsub Ssrc[, istart[, iend]]\"\n"
"\"strsubk\";\"Strings:Manipulation\";\"Extract a substring\";\"Sdst strsubk Ssrc, kstart, kend\"\n"
"\"strchar\";\"Strings:Conversion\";\"Return the ASCII code of a character in a string\";\"ichr strchar Sstr[, ipos]\"\n"
"\"strchark\";\"Strings:Conversion\";\"Return the ASCII code of a character in a string\";\"kchr strchark Sstr[, kpos]\"\n"
"\"strlower\";\"Strings:Conversion\";\"Convert a string to lower case\";\"Sdst strlower Ssrc\"\n"
"\"strlowerk\";\"Strings:Conversion\";\"Convert a string to lower case\";\"Sdst strlowerk Ssrc\"\n"
"\"strtod\";\"Strings:Conversion\";\"Converts a string to a float (i-rate).\";\"ir strtod Sstr\"\n"
"\"strtodk\";\"Strings:Conversion\";\"Converts a string to a float (k-rate).\";\"kr strtodk Sstr\"\n"
"\"strtol\";\"Strings:Conversion\";\"Converts a string to a signed integer (i-rate).\";\"ir strtol Sstr\"\n"
"\"strtolk\";\"Strings:Conversion\";\"Converts a string to a signed integer (k-rate).\";\"kr strtolk Sstr\"\n"
"\"strupper\";\"Strings:Conversion\";\"Convert a string to upper case\";\"Sdst strupper Ssrc\"\n"
"\"strupperk\";\"Strings:Conversion\";\"Convert a string to upper case\";\"Sdst strupperk Ssrc\"\n"
"\"vtaba\";\"Vectorial:Tables\";\"Read vectors (from tables -or arrays of vectors).\";\"vtaba andx, ifn, aout1 [, aout2, aout3, .... , aoutN ]\"\n"
"\"vtabi\";\"Vectorial:Tables\";\"Read vectors (from tables -or arrays of vectors).\";\"vtabi indx, ifn, iout1 [, iout2, iout3, .... , ioutN ]\"\n"
"\"vtabk\";\"Vectorial:Tables\";\"Read vectors (from tables -or arrays of vectors).\";\"vtabk kndx, ifn, kout1 [, kout2, kout3, .... , koutN ]\"\n"
"\"vtable1k\";\"Vectorial:Tables\";\"Read a vector (several scalars simultaneously) from a table.\";\"vtable1k kfn,kout1 [, kout2, kout3, .... , koutN ]\"\n"
"\"vtablea\";\"Vectorial:Tables\";\"Read vectors (from tables -or arrays of vectors).\";\"vtablea andx, kfn, kinterp, ixmode, aout1 [, aout2, aout3, .... , aoutN ]\"\n"
"\"vtablei\";\"Vectorial:Tables\";\"Read vectors (from tables -or arrays of vectors).\";\"vtablei indx, ifn, interp, ixmode, iout1 [, iout2, iout3, .... , ioutN ]\"\n"
"\"vtablek\";\"Vectorial:Tables\";\"Read vectors (from tables -or arrays of vectors).\";\"vtablek kndx, kfn, kinterp, ixmode, kout1 [, kout2, kout3, .... , koutN ]\"\n"
"\"vtablewa\";\"Vectorial:Tables\";\"Write vectors (to tables -or arrays of vectors).\";\"vtablewa andx, kfn, ixmode, ainarg1 [, ainarg2, ainarg3 , .... , ainargN ]\"\n"
"\"vtablewi\";\"Vectorial:Tables\";\"Write vectors (to tables -or arrays of vectors).\";\"vtablewi indx, ifn, ixmode, inarg1 [, inarg2, inarg3 , .... , inargN ]\"\n"
"\"vtablewk\";\"Vectorial:Tables\";\"Write vectors (to tables -or arrays of vectors).\";\"vtablewk kndx, kfn, ixmode, kinarg1 [, kinarg2, kinarg3 , .... , kinargN ]\"\n"
"\"vtabwa\";\"Vectorial:Tables\";\"Write vectors (to tables -or arrays of vectors).\";\"vtabwa andx, ifn, ainarg1 [, ainarg2, ainarg3 , .... , ainargN ]\"\n"
"\"vtabwi\";\"Vectorial:Tables\";\"Write vectors (to tables -or arrays of vectors).\";\"vtabwi indx, ifn, inarg1 [, inarg2, inarg3 , .... , inargN ]\"\n"
"\"vtabwk\";\"Vectorial:Tables\";\"Write vectors (to tables -or arrays of vectors).\";\"vtabwk kndx, ifn, kinarg1 [, kinarg2, kinarg3 , .... , kinargN ]\"\n"
"\"vadd\";\"Vectorial:Scalar operations\";\"Adds a scalar value to a vector in a table.\";\"vadd ifn, kval, kelements [, kdstoffset] [, kverbose]\"\n"
"\"vadd_i\";\"Vectorial:Scalar operations\";\"Adds a scalar value to a vector in a table.\";\"vadd_i ifn, ival, ielements [, idstoffset]\"\n"
"\"vexp\";\"Vectorial:Scalar operations\";\"Performs power-of operations between a vector and a scalar\";\"vexp ifn, kval, kelements [, kdstoffset] [, kverbose]\"\n"
"\"vexp_i\";\"Vectorial:Scalar operations\";\"Performs power-of operations between a vector and a scalar\";\"vexp_i ifn, ival, ielements[, idstoffset]\"\n"
"\"vmult\";\"Vectorial:Scalar operations\";\"Multiplies a vector in a table by a scalar value.\";\"vmult ifn, kval, kelements [, kdstoffset] [, kverbose]\"\n"
"\"vmult_i\";\"Vectorial:Scalar operations\";\"Multiplies a vector in a table by a scalar value.\";\"vmult_i ifn, ival, ielements [, idstoffset]\"\n"
"\"vpow\";\"Vectorial:Scalar operations\";\"Raises each element of a vector to a scalar power.\";\"vpow ifn, kval, kelements [, kdstoffset] [, kverbose]\"\n"
"\"vpow_i\";\"Vectorial:Scalar operations\";\"Raises each element of a vector to a scalar power\";\"vpow_i ifn, ival, ielements [, idstoffset]\"\n"
"\"vaddv\";\"Vectorial:Vectorial operations\";\"Performs addition between two vectorial control signals.\";\"vaddv ifn1, ifn2, kelements [, kdstoffset] [, ksrcoffset] [,kverbose]\"\n"
"\"vaddv_i\";\"Vectorial:Vectorial operations\";\"Performs addition between two vectorial control signals at init time.\";\"vaddv_i ifn1, ifn2, ielements [, idstoffset] [, isrcoffset]\"\n"
"\"vcopy\";\"Vectorial:Vectorial operations\";\"Copies between two vectorial control signals\";\"vcopy ifn1, ifn2, kelements [, kdstoffset] [, ksrcoffset] [, kverbose]\"\n"
"\"vcopy_i\";\"Vectorial:Vectorial operations\";\"Copies a vector from one table to another.\";\"vcopy_i ifn1, ifn2, ielements [,idstoffset, isrcoffset]\"\n"
"\"vdivv\";\"Vectorial:Vectorial operations\";\"Performs division between two vectorial control signals\";\"vdivv ifn1, ifn2, kelements [, kdstoffset] [, ksrcoffset] [,kverbose]\"\n"
"\"vdivv_i\";\"Vectorial:Vectorial operations\";\"Performs division between two vectorial control signals at init time.\";\"vdivv_i ifn1, ifn2, ielements [, idstoffset] [, isrcoffset]\"\n"
"\"vexpv\";\"Vectorial:Vectorial operations\";\"Performs exponential operations between two vectorial control signals\";\"vexpv ifn1, ifn2, kelements [, kdstoffset] [, ksrcoffset] [,kverbose]\"\n"
"\"vexpv_i\";\"Vectorial:Vectorial operations\";\"Performs exponential operations between two vectorial control signals at init time.\";\"vexpv_i ifn1, ifn2, ielements [, idstoffset] [, isrcoffset]\"\n"
"\"vmap\";\"Vectorial:Vectorial operations\";\"Maps elements from a vector according to indexes contained in another vector.\";\"vmap ifn1, ifn2, ielements [,idstoffset, isrcoffset]\"\n"
"\"vmultv\";\"Vectorial:Vectorial operations\";\"Performs mutiplication between two vectorial control signals\";\"vmultv ifn1, ifn2, kelements [, kdstoffset] [, ksrcoffset] [,kverbose]\"\n"
"\"vmultv_i\";\"Vectorial:Vectorial operations\";\"Performs mutiplication between two vectorial control signals at init time.\";\"vmultv_i ifn1, ifn2, ielements [, idstoffset] [, isrcoffset]\"\n"
"\"vpowv\";\"Vectorial:Vectorial operations\";\"Performs power-of operations between two vectorial control signals\";\"vpowv ifn1, ifn2, kelements [, kdstoffset] [, ksrcoffset] [,kverbose]\"\n"
"\"vpowv_i\";\"Vectorial:Vectorial operations\";\"Performs power-of operations between two vectorial control signals at init time.\";\"vpowv_i ifn1, ifn2, ielements [, idstoffset] [, isrcoffset]\"\n"
"\"vsubv\";\"Vectorial:Vectorial operations\";\"Performs subtraction between two vectorial control signals\";\"vsubv ifn1, ifn2, kelements [, kdstoffset] [, ksrcoffset] [,kverbose]\"\n"
"\"vsubv_i\";\"Vectorial:Vectorial operations\";\"Performs subtraction between two vectorial control signals at init time.\";\"vsubv_i ifn1, ifn2, ielements [, idstoffset] [, isrcoffset]\"\n"
"\"vexpseg\";\"Vectorial:Envelopes\";\"Vectorial envelope generator\";\"vexpseg ifnout, ielements, ifn1, idur1, ifn2 [, idur2, ifn3 [...]]\"\n"
"\"vlinseg\";\"Vectorial:Envelopes\";\"Vectorial envelope generator\";\"vlinseg ifnout, ielements, ifn1, idur1, ifn2 [, idur2, ifn3 [...]]\"\n"
"\"vlimit\";\"Vectorial:Limiting and Wrapping\";\"Limiting and Wrapping Vectorial Signals\";\"vlimit ifn, kmin, kmax, ielements\"\n"
"\"vmirror\";\"Vectorial:Limiting and Wrapping\";\"Limiting and Wrapping Vectorial Signals\";\"vmirror ifn, kmin, kmax, ielements\"\n"
"\"vwrap\";\"Vectorial:Limiting and Wrapping\";\"Limiting and Wrapping Vectorial Signals\";\"vwrap ifn, kmin, kmax, ielements\"\n"
"\"vdelayk\";\"Vectorial:Delay Paths\";\"k-rate variable time delay.\";\"kout vdelayk iksig, kdel, imaxdel [, iskip, imode]\"\n"
"\"vecdelay\";\"Vectorial:Delay Paths\";\"Vectorial Control-rate Delay Paths\";\"vecdelay ifn, ifnIn, ifnDel, ielements, imaxdel [, iskip]\"\n"
"\"vport\";\"Vectorial:Delay Paths\";\"Vectorial Control-rate Delay Paths\";\"vport ifn, khtime, ielements [, ifnInit]\"\n"
"\"vrandh\";\"Vectorial:Random\";\"Generates a vector of random numbers stored into a table, holding the values for a period of time.\";\"vrandh ifn, krange, kcps, ielements [, idstoffset] [, iseed]  [, isize] [, ioffset]\"\n"
"\"vrandi\";\"Vectorial:Random\";\"Generate a sort of 'vectorial band-limited noise'\";\"vrandi ifn, krange, kcps, ielements [, idstoffset] [, iseed]  [, isize] [, ioffset]\"\n"
"\"cell\";\"Vectorial:Cellular Automata\";\"Cellular Automaton\";\"cell ktrig, kreinit, ioutFunc, initStateFunc, iRuleFunc, ielements\"\n"
"\"vcella\";\"Vectorial:Cellular Automata\";\"Cellular Automata\";\"vcella ktrig, kreinit, ioutFunc, initStateFunc,  iRuleFunc, ielements, irulelen [, iradius]\"\n"
"\"zacl\";\"Zak Patch System\";\"Clears one or more variables in the za space.\";\"zacl kfirst, klast\"\n"
"\"zakinit\";\"Zak Patch System\";\"Establishes zak space.\";\"zakinit isizea, isizek\"\n"
"\"zamod\";\"Zak Patch System\";\"Modulates one a-rate signal by a second one.\";\"ares zamod asig, kzamod\"\n"
"\"zar\";\"Zak Patch System\";\"Reads from a location in za space at a-rate.\";\"ares zar kndx\"\n"
"\"zarg\";\"Zak Patch System\";\"Reads from a location in za space at a-rate, adds some gain.\";\"ares zarg kndx, kgain\"\n"
"\"zaw\";\"Zak Patch System\";\"Writes to a za variable at a-rate without mixing.\";\"zaw asig, kndx\"\n"
"\"zawm\";\"Zak Patch System\";\"Writes to a za variable at a-rate with mixing.\";\"zawm asig, kndx [, imix]\"\n"
"\"zir\";\"Zak Patch System\";\"Reads from a location in zk space at i-rate.\";\"ir zir indx\"\n"
"\"ziw\";\"Zak Patch System\";\"Writes to a zk variable at i-rate without mixing.\";\"ziw isig, indx\"\n"
"\"ziwm\";\"Zak Patch System\";\"Writes to a zk variable to an i-rate variable with mixing.\";\"ziwm isig, indx [, imix]\"\n"
"\"zkcl\";\"Zak Patch System\";\"Clears one or more variables in the zk space.\";\"zkcl kfirst, klast\"\n"
"\"zkmod\";\"Zak Patch System\";\"Facilitates the modulation of one signal by another.\";\"kres zkmod ksig, kzkmod\"\n"
"\"zkr\";\"Zak Patch System\";\"Reads from a location in zk space at k-rate.\";\"kres zkr kndx\"\n"
"\"zkw\";\"Zak Patch System\";\"Writes to a zk variable at k-rate without mixing.\";\"zkw ksig, kndx\"\n"
"\"zkwm\";\"Zak Patch System\";\"Writes to a zk variable at k-rate with mixing.\";\"zkwm ksig, kndx [, imix]\"\n"
"\"dssiactivate\";\"Plugin Hosting:DSSI and LADSPA\";\"Activates or deactivates a DSSI or LADSPA plugin.\";\"dssiactivate ihandle, ktoggle\"\n"
"\"dssiaudio\";\"Plugin Hosting:DSSI and LADSPA\";\"Processes audio using a LADSPA or DSSI plugin.\";\"[aout1, aout2, ..., aout9] dssiaudio ihandle, [ain1, ain2, ..., ain9]\"\n"
"\"dssictls\";\"Plugin Hosting:DSSI and LADSPA\";\"Send control information to a LADSPA or DSSI plugin.\";\"dssictls ihandle, iport, kvalue, ktrigger\"\n"
"\"dssiinit\";\"Plugin Hosting:DSSI and LADSPA\";\"Loads a DSSI or LADSPA plugin.\";\"ihandle dssiinit ilibraryname, iplugindex [, iverbose]\"\n"
"\"dssilist\";\"Plugin Hosting:DSSI and LADSPA\";\"Lists all available DSSI and LADSPA plugins.\";\"dssilist\"\n"
"\"vstaudio\";\"Plugin Hosting:VST\";\"VST audio output.\";\"aout1,aout2 vstaudio instance, [ain1, ain2]\"\n"
"\"vstaudiog\";\"Plugin Hosting:VST\";\"VST audio output.\";\"aout1,aout2 vstaudiog instance, [ain1, ain2]\"\n"
"\"vstbankload\";\"Plugin Hosting:VST\";\"Loads parameter banks to a VST plugin.\";\"vstbankload instance, ipath\"\n"
"\"vstedit\";\"Plugin Hosting:VST\";\"Opens the GUI editor window for a VST plugin.\";\"vstedit instance\"\n"
"\"vstinfo\";\"Plugin Hosting:VST\";\"Displays the parameters and the programs of a VST plugin.\";\"vstinfo instance\"\n"
"\"vstinit\";\"Plugin Hosting:VST\";\"Load a VST plugin into memory for use with the other vst4cs opcodes.\";\"instance vstinit ilibrarypath [,iverbose]\"\n"
"\"vstmidiout\";\"Plugin Hosting:VST\";\"Sends MIDI information to a VST plugin.\";\"vstmidiout instance, kstatus, kchan, kdata1, kdata2\"\n"
"\"vstnote\";\"Plugin Hosting:VST\";\"Sends a MIDI note with definite duration to a VST plugin.\";\"vstnote instance, kchan, knote, kveloc, kdur\"\n"
"\"vstparamset \";\"Plugin Hosting:VST\";\"Used for parameter comunication to and from a VST plugin.\";\"vstparamset instance, kparam, kvalue\"\n"
"\"vstparamget \";\"Plugin Hosting:VST\";\"Used for parameter comunication to and from a VST plugin.\";\"kvalue vstparamget instance, kparam\"\n"
"\"vstprogset\";\"Plugin Hosting:VST\";\"Loads parameter banks to a VST plugin.\";\"vstprogset instance, kprogram\"\n"
"\"OSCinit\";\"OSC\";\"Start a listening process for OSC messages to a particular port.\";\"ihandle OSCinit iport\"\n"
"\"OSClisten\";\"OSC\";\"Listen for OSC messages to a particular path.\";\"kans OSClisten ihandle, idest, itype [, xdata1, xdata2, ...]\"\n"
"\"OSCsend\";\"OSC\";\"Sends data to other processes using the OSC protocol\";\"OSCsend kwhen, ihost, iport, idestination, itype [, kdata1, kdata2, ...]\"\n"
"\"remoteport\";\"Network\";\"Defines the port for use with the remote system.\";\"remoteport iportnum\"\n"
"\"sockrecv\";\"Network\";\"Receives data from other processes using the low-level UDP or TCP protocols\";\"asig sockrecv iport, ilength\"\n"
"\"sockrecvs\";\"Network\";\"Receives data from other processes using the low-level UDP or TCP protocols\";\"asigl, asigr sockrecvs iport, ilength\"\n"
"\"strecv\";\"Network\";\"Receives data from other processes using the low-level UDP or TCP protocols\";\"asig strecv Sipaddr, iport\"\n"
"\"socksend\";\"Network\";\"Sends data to other processes using the low-level UDP or TCP protocols\";\"socksend asig, Sipaddr, iport, ilength\"\n"
"\"socksends\";\"Network\";\"Sends data to other processes using the low-level UDP or TCP protocols\";\"socksends asigl, asigr, Sipaddr, iport, ilength\"\n"
"\"stsend\";\"Network\";\"Sends data to other processes using the low-level UDP or TCP protocols\";\"stsend asig, Sipaddr, iport\"\n"
"\"insglobal\";\"Remote Opcodes\";\"An opcode which can be used to implement a remote orchestra. This opcode will send note events from a source machine to many destinations.\";\"insglobal isource, instrnum [,instrnum...]\"\n"
"\"insremot\";\"Remote Opcodes\";\"An opcode which can be used to implement a remote orchestra. This opcode will send note events from a source machine to one destination.\";\"insremot idestination, isource, instrnum [,instrnum...]\"\n"
"\"midglobal\";\"Remote Opcodes\";\"An opcode which can be used to implement a remote midi orchestra. This opcode will broadcast the midi events to all the machines involved in the remote concert.\";\"midglobal isource, instrnum [,instrnum...]\"\n"
"\"midremot\";\"Remote Opcodes\";\"An opcode which can be used to implement a remote midi orchestra. This opcode will send midi events from a source machine to one destination.\";\"midremot idestination, isource, instrnum [,instrnum...]\"\n"
"\"MixerClear\";\"Mixer Opcodes\";\"Resets all channels of a buss to 0.\";\"MixerClear\"\n"
"\"MixerGetLevel\";\"Mixer Opcodes\";\"Gets the level of a send to a buss.\";\"kgain MixerGetLevel isend, ibuss\"\n"
"\"MixerReceive\";\"Mixer Opcodes\";\"Receives an arate signal from a channel of a buss.\";\"asignal MixerReceive ibuss, ichannel\"\n"
"\"MixerSend\";\"Mixer Opcodes\";\"Mixes an arate signal into a channel of a buss.\";\"MixerSend asignal, isend, ibuss, ichannel\"\n"
"\"MixerSetLevel\";\"Mixer Opcodes\";\"Sets the level of a send to a buss.\";\"MixerSetLevel isend, ibuss, kgain\"\n"
"\"MixerSetLevel_i\";\"Mixer Opcodes\";\"Sets the level of a send to a buss.\";\"MixerSetLevel_i isend, ibuss, igain\"\n"
"\"pyassign\";\"Python Opcodes\";\"Assign the value of the given Csound variable to a Python variable possibly destroying its previous content.\";\"pyassign \"variable\", kvalue\"\n"
"\"pyassigni\";\"Python Opcodes\";\"Assign the value of the given Csound variable to a Python variable possibly destroying its previous content.\";\"pyassigni \"variable\", ivalue\"\n"
"\"pylassign\";\"Python Opcodes\";\"Assign the value of the given Csound variable to a Python variable possibly destroying its previous content.\";\"pylassign \"variable\", kvalue\"\n"
"\"pylassigni\";\"Python Opcodes\";\"Assign the value of the given Csound variable to a Python variable possibly destroying its previous content.\";\"pylassigni \"variable\", ivalue\"\n"
"\"pyassignt\";\"Python Opcodes\";\"Assign the value of the given Csound variable to a Python variable possibly destroying its previous content.\";\"pyassignt ktrigger, \"variable\", kvalue\"\n"
"\"pylassignt\";\"Python Opcodes\";\"Assign the value of the given Csound variable to a Python variable possibly destroying its previous content.\";\"pylassignt ktrigger, \"variable\", kvalue\"\n"
"\"pycall\";\"Python Opcodes\";\"Invoke the specified Python callable at k-time and i-time (i suffix), passing the given arguments. The call is perfomed in the global environment, and the result (the returning value) is copied into the Csound output variables specified.\";\"pycall \"callable\", karg1, ...\"\n"
"\"pycall1\";\"Python Opcodes\";\"Invoke the specified Python callable at k-time and i-time (i suffix), passing the given arguments. The call is perfomed in the global environment, and the result (the returning value) is copied into the Csound output variables specified.\";\"kresult pycall1 \"callable\", karg1, ...\"\n"
"\"pycall2\";\"Python Opcodes\";\"Invoke the specified Python callable at k-time and i-time (i suffix), passing the given arguments. The call is perfomed in the global environment, and the result (the returning value) is copied into the Csound output variables specified.\";\"kresult1, kresult2 pycall2 \"callable\", karg1, ...\"\n"
"\"pycall3\";\"Python Opcodes\";\"Invoke the specified Python callable at k-time and i-time (i suffix), passing the given arguments. The call is perfomed in the global environment, and the result (the returning value) is copied into the Csound output variables specified.\";\"kr1, kr2, kr3 pycall3 \"callable\", karg1, ...\"\n"
"\"pycall4\";\"Python Opcodes\";\"Invoke the specified Python callable at k-time and i-time (i suffix), passing the given arguments. The call is perfomed in the global environment, and the result (the returning value) is copied into the Csound output variables specified.\";\"kr1, kr2, kr3, kr4 pycall4 \"callable\", karg1, ...\"\n"
"\"pycall5\";\"Python Opcodes\";\"Invoke the specified Python callable at k-time and i-time (i suffix), passing the given arguments. The call is perfomed in the global environment, and the result (the returning value) is copied into the Csound output variables specified.\";\"kr1, kr2, kr3, kr4, kr5 pycall5 \"callable\", karg1, ...\"\n"
"\"pycall6\";\"Python Opcodes\";\"Invoke the specified Python callable at k-time and i-time (i suffix), passing the given arguments. The call is perfomed in the global environment, and the result (the returning value) is copied into the Csound output variables specified.\";\"kr1, kr2, kr3, kr4, kr5, kr6 pycall6 \"callable\", karg1, ...\"\n"
"\"pycall7\";\"Python Opcodes\";\"Invoke the specified Python callable at k-time and i-time (i suffix), passing the given arguments. The call is perfomed in the global environment, and the result (the returning value) is copied into the Csound output variables specified.\";\"kr1, kr2, kr3, kr4, kr5, kr6, kr7 pycall7 \"callable\", karg1, ...\"\n"
"\"pycall8\";\"Python Opcodes\";\"Invoke the specified Python callable at k-time and i-time (i suffix), passing the given arguments. The call is perfomed in the global environment, and the result (the returning value) is copied into the Csound output variables specified.\";\"kr1, kr2, kr3, kr4, kr5, kr6, kr7, kr8 pycall8 \"callable\", karg1, ...\"\n"
"\"pycallt\";\"Python Opcodes\";\"Invoke the specified Python callable at k-time and i-time (i suffix), passing the given arguments. The call is perfomed in the global environment, and the result (the returning value) is copied into the Csound output variables specified.\";\"pycallt ktrigger, \"callable\", karg1, ...\"\n"
"\"pycall1t\";\"Python Opcodes\";\"Invoke the specified Python callable at k-time and i-time (i suffix), passing the given arguments. The call is perfomed in the global environment, and the result (the returning value) is copied into the Csound output variables specified.\";\"kresult pycall1t ktrigger, \"callable\", karg1, ...\"\n"
"\"pycall2t\";\"Python Opcodes\";\"Invoke the specified Python callable at k-time and i-time (i suffix), passing the given arguments. The call is perfomed in the global environment, and the result (the returning value) is copied into the Csound output variables specified.\";\"kresult1, kresult2 pycall2t ktrigger, \"callable\", karg1, ...\"\n"
"\"pycall3t\";\"Python Opcodes\";\"Invoke the specified Python callable at k-time and i-time (i suffix), passing the given arguments. The call is perfomed in the global environment, and the result (the returning value) is copied into the Csound output variables specified.\";\"kr1, kr2, kr3 pycall3t ktrigger, \"callable\", karg1, ...\"\n"
"\"pycall4t\";\"Python Opcodes\";\"Invoke the specified Python callable at k-time and i-time (i suffix), passing the given arguments. The call is perfomed in the global environment, and the result (the returning value) is copied into the Csound output variables specified.\";\"kr1, kr2, kr3, kr4 pycall4t ktrigger, \"callable\", karg1, ...\"\n"
"\"pycall5t\";\"Python Opcodes\";\"Invoke the specified Python callable at k-time and i-time (i suffix), passing the given arguments. The call is perfomed in the global environment, and the result (the returning value) is copied into the Csound output variables specified.\";\"kr1, kr2, kr3, kr4, kr5 pycall5t ktrigger, \"callable\", karg1, ...\"\n"
"\"pycall6t\";\"Python Opcodes\";\"Invoke the specified Python callable at k-time and i-time (i suffix), passing the given arguments. The call is perfomed in the global environment, and the result (the returning value) is copied into the Csound output variables specified.\";\"kr1, kr2, kr3, kr4, kr5, kr6 pycall6t ktrigger, \"callable\", karg1, ...\"\n"
"\"pycall7t\";\"Python Opcodes\";\"Invoke the specified Python callable at k-time and i-time (i suffix), passing the given arguments. The call is perfomed in the global environment, and the result (the returning value) is copied into the Csound output variables specified.\";\"kr1, kr2, kr3, kr4, kr5, kr6, kr7 pycall7t ktrigger, \"callable\", karg1, ...\"\n"
"\"pycall8t\";\"Python Opcodes\";\"Invoke the specified Python callable at k-time and i-time (i suffix), passing the given arguments. The call is perfomed in the global environment, and the result (the returning value) is copied into the Csound output variables specified.\";\"kr1, kr2, kr3, kr4, kr5, kr6, kr7, kr8 pycall8t ktrigger, \"callable\", karg1, ...\"\n"
"\"pycalli\";\"Python Opcodes\";\"Invoke the specified Python callable at k-time and i-time (i suffix), passing the given arguments. The call is perfomed in the global environment, and the result (the returning value) is copied into the Csound output variables specified.\";\"pycalli \"callable\", karg1, ...\"\n"
"\"pycall1i\";\"Python Opcodes\";\"Invoke the specified Python callable at k-time and i-time (i suffix), passing the given arguments. The call is perfomed in the global environment, and the result (the returning value) is copied into the Csound output variables specified.\";\"iresult pycall1i \"callable\", iarg1, ...\"\n"
"\"pycall2i\";\"Python Opcodes\";\"Invoke the specified Python callable at k-time and i-time (i suffix), passing the given arguments. The call is perfomed in the global environment, and the result (the returning value) is copied into the Csound output variables specified.\";\"iresult1, iresult2 pycall2i \"callable\", iarg1, ...\"\n"
"\"pycall3i\";\"Python Opcodes\";\"Invoke the specified Python callable at k-time and i-time (i suffix), passing the given arguments. The call is perfomed in the global environment, and the result (the returning value) is copied into the Csound output variables specified.\";\"ir1, ir2, ir3 pycall3i \"callable\", iarg1, ...\"\n"
"\"pycall4i\";\"Python Opcodes\";\"Invoke the specified Python callable at k-time and i-time (i suffix), passing the given arguments. The call is perfomed in the global environment, and the result (the returning value) is copied into the Csound output variables specified.\";\"ir1, ir2, ir3, ir4 pycall4i \"callable\", iarg1, ...\"\n"
"\"pycall5i\";\"Python Opcodes\";\"Invoke the specified Python callable at k-time and i-time (i suffix), passing the given arguments. The call is perfomed in the global environment, and the result (the returning value) is copied into the Csound output variables specified.\";\"ir1, ir2, ir3, ir4, ir5 pycall5i \"callable\", iarg1, ...\"\n"
"\"pycall6i\";\"Python Opcodes\";\"Invoke the specified Python callable at k-time and i-time (i suffix), passing the given arguments. The call is perfomed in the global environment, and the result (the returning value) is copied into the Csound output variables specified.\";\"ir1, ir2, ir3, ir4, ir5, ir6 pycall6i \"callable\", iarg1, ...\"\n"
"\"pycall7i\";\"Python Opcodes\";\"Invoke the specified Python callable at k-time and i-time (i suffix), passing the given arguments. The call is perfomed in the global environment, and the result (the returning value) is copied into the Csound output variables specified.\";\"ir1, ir2, ir3, ir4, ir5, ir6, ir7 pycall7i \"callable\", iarg1, ...\"\n"
"\"pycall8i\";\"Python Opcodes\";\"Invoke the specified Python callable at k-time and i-time (i suffix), passing the given arguments. The call is perfomed in the global environment, and the result (the returning value) is copied into the Csound output variables specified.\";\"ir1, ir2, ir3, ir4, ir5, ir6, ir7, ir8 pycall8i \"callable\", iarg1, ...\"\n"
"\"pycalln\";\"Python Opcodes\";\"Invoke the specified Python callable at k-time and i-time (i suffix), passing the given arguments. The call is perfomed in the global environment, and the result (the returning value) is copied into the Csound output variables specified.\";\"pycalln \"callable\", nresults, kresult1, ..., kresultn, karg1, ...\"\n"
"\"pycallni\";\"Python Opcodes\";\"Invoke the specified Python callable at k-time and i-time (i suffix), passing the given arguments. The call is perfomed in the global environment, and the result (the returning value) is copied into the Csound output variables specified.\";\"pycallni \"callable\", nresults, iresult1, ..., iresultn, iarg1, ...\"\n"
"\"pylcall\";\"Python Opcodes\";\"Invoke the specified Python callable at k-time and i-time (i suffix), passing the given arguments. The call is perfomed in the global environment, and the result (the returning value) is copied into the Csound output variables specified.\";\"pylcall \"callable\", karg1, ...\"\n"
"\"pylcall1\";\"Python Opcodes\";\"Invoke the specified Python callable at k-time and i-time (i suffix), passing the given arguments. The call is perfomed in the global environment, and the result (the returning value) is copied into the Csound output variables specified.\";\"kresult pylcall1 \"callable\", karg1, ...\"\n"
"\"pylcall2\";\"Python Opcodes\";\"Invoke the specified Python callable at k-time and i-time (i suffix), passing the given arguments. The call is perfomed in the global environment, and the result (the returning value) is copied into the Csound output variables specified.\";\"kresult1, kresult2 pylcall2 \"callable\", karg1, ...\"\n"
"\"pylcall3\";\"Python Opcodes\";\"Invoke the specified Python callable at k-time and i-time (i suffix), passing the given arguments. The call is perfomed in the global environment, and the result (the returning value) is copied into the Csound output variables specified.\";\"kr1, kr2, kr3 pylcall3 \"callable\", karg1, ...\"\n"
"\"pylcall4\";\"Python Opcodes\";\"Invoke the specified Python callable at k-time and i-time (i suffix), passing the given arguments. The call is perfomed in the global environment, and the result (the returning value) is copied into the Csound output variables specified.\";\"kr1, kr2, kr3, kr4 pylcall4 \"callable\", karg1, ...\"\n"
"\"pylcall5\";\"Python Opcodes\";\"Invoke the specified Python callable at k-time and i-time (i suffix), passing the given arguments. The call is perfomed in the global environment, and the result (the returning value) is copied into the Csound output variables specified.\";\"kr1, kr2, kr3, kr4, kr5 pylcall5 \"callable\", karg1, ...\"\n"
"\"pylcall6\";\"Python Opcodes\";\"Invoke the specified Python callable at k-time and i-time (i suffix), passing the given arguments. The call is perfomed in the global environment, and the result (the returning value) is copied into the Csound output variables specified.\";\"kr1, kr2, kr3, kr4, kr5, kr6 pylcall6 \"callable\", karg1, ...\"\n"
"\"pylcall7\";\"Python Opcodes\";\"Invoke the specified Python callable at k-time and i-time (i suffix), passing the given arguments. The call is perfomed in the global environment, and the result (the returning value) is copied into the Csound output variables specified.\";\"kr1, kr2, kr3, kr4, kr5, kr6, kr7 pylcall7 \"callable\", karg1, ...\"\n"
"\"pylcall8\";\"Python Opcodes\";\"Invoke the specified Python callable at k-time and i-time (i suffix), passing the given arguments. The call is perfomed in the global environment, and the result (the returning value) is copied into the Csound output variables specified.\";\"kr1, kr2, kr3, kr4, kr5, kr6, kr7, kr8 pylcall8 \"callable\", karg1, ...\"\n"
"\"pylcallt\";\"Python Opcodes\";\"Invoke the specified Python callable at k-time and i-time (i suffix), passing the given arguments. The call is perfomed in the global environment, and the result (the returning value) is copied into the Csound output variables specified.\";\"pylcallt ktrigger, \"callable\", karg1, ...\"\n"
"\"pylcall1t\";\"Python Opcodes\";\"Invoke the specified Python callable at k-time and i-time (i suffix), passing the given arguments. The call is perfomed in the global environment, and the result (the returning value) is copied into the Csound output variables specified.\";\"kresult pylcall1t ktrigger, \"callable\", karg1, ...\"\n"
"\"pylcall2t\";\"Python Opcodes\";\"Invoke the specified Python callable at k-time and i-time (i suffix), passing the given arguments. The call is perfomed in the global environment, and the result (the returning value) is copied into the Csound output variables specified.\";\"kresult1, kresult2 pylcall2t ktrigger, \"callable\", karg1, ...\"\n"
"\"pylcall3t\";\"Python Opcodes\";\"Invoke the specified Python callable at k-time and i-time (i suffix), passing the given arguments. The call is perfomed in the global environment, and the result (the returning value) is copied into the Csound output variables specified.\";\"kr1, kr2, kr3 pylcall3t ktrigger, \"callable\", karg1, ...\"\n"
"\"pylcall4t\";\"Python Opcodes\";\"Invoke the specified Python callable at k-time and i-time (i suffix), passing the given arguments. The call is perfomed in the global environment, and the result (the returning value) is copied into the Csound output variables specified.\";\"kr1, kr2, kr3, kr4 pylcall4t ktrigger, \"callable\", karg1, ...\"\n"
"\"pylcall5t\";\"Python Opcodes\";\"Invoke the specified Python callable at k-time and i-time (i suffix), passing the given arguments. The call is perfomed in the global environment, and the result (the returning value) is copied into the Csound output variables specified.\";\"kr1, kr2, kr3, kr4, kr5 pylcall5t ktrigger, \"callable\", karg1, ...\"\n"
"\"pylcall6t\";\"Python Opcodes\";\"Invoke the specified Python callable at k-time and i-time (i suffix), passing the given arguments. The call is perfomed in the global environment, and the result (the returning value) is copied into the Csound output variables specified.\";\"kr1, kr2, kr3, kr4, kr5, kr6 pylcall6t ktrigger, \"callable\", karg1, ...\"\n"
"\"pylcall7t\";\"Python Opcodes\";\"Invoke the specified Python callable at k-time and i-time (i suffix), passing the given arguments. The call is perfomed in the global environment, and the result (the returning value) is copied into the Csound output variables specified.\";\"kr1, kr2, kr3, kr4, kr5, kr6, kr7 pylcall7t ktrigger, \"callable\", karg1, ...\"\n"
"\"pylcall8t\";\"Python Opcodes\";\"Invoke the specified Python callable at k-time and i-time (i suffix), passing the given arguments. The call is perfomed in the global environment, and the result (the returning value) is copied into the Csound output variables specified.\";\"kr1, kr2, kr3, kr4, kr5, kr6, kr7, kr8 pylcall8t ktrigger, \"callable\", karg1, ...\"\n"
"\"pylcalli\";\"Python Opcodes\";\"Invoke the specified Python callable at k-time and i-time (i suffix), passing the given arguments. The call is perfomed in the global environment, and the result (the returning value) is copied into the Csound output variables specified.\";\"pylcalli \"callable\", karg1, ...\"\n"
"\"pylcall1i\";\"Python Opcodes\";\"Invoke the specified Python callable at k-time and i-time (i suffix), passing the given arguments. The call is perfomed in the global environment, and the result (the returning value) is copied into the Csound output variables specified.\";\"iresult pylcall1i \"callable\", iarg1, ...\"\n"
"\"pylcall2i\";\"Python Opcodes\";\"Invoke the specified Python callable at k-time and i-time (i suffix), passing the given arguments. The call is perfomed in the global environment, and the result (the returning value) is copied into the Csound output variables specified.\";\"iresult1, iresult2 pylcall2i \"callable\", iarg1, ...\"\n"
"\"pylcall3i\";\"Python Opcodes\";\"Invoke the specified Python callable at k-time and i-time (i suffix), passing the given arguments. The call is perfomed in the global environment, and the result (the returning value) is copied into the Csound output variables specified.\";\"ir1, ir2, ir3 pylcall3i \"callable\", iarg1, ...\"\n"
"\"pylcall4i\";\"Python Opcodes\";\"Invoke the specified Python callable at k-time and i-time (i suffix), passing the given arguments. The call is perfomed in the global environment, and the result (the returning value) is copied into the Csound output variables specified.\";\"ir1, ir2, ir3, ir4 pylcall4i \"callable\", iarg1, ...\"\n"
"\"pylcall5i\";\"Python Opcodes\";\"Invoke the specified Python callable at k-time and i-time (i suffix), passing the given arguments. The call is perfomed in the global environment, and the result (the returning value) is copied into the Csound output variables specified.\";\"ir1, ir2, ir3, ir4, ir5 pylcall5i \"callable\", iarg1, ...\"\n"
"\"pylcall6i\";\"Python Opcodes\";\"Invoke the specified Python callable at k-time and i-time (i suffix), passing the given arguments. The call is perfomed in the global environment, and the result (the returning value) is copied into the Csound output variables specified.\";\"ir1, ir2, ir3, ir4, ir5, ir6 pylcall6i \"callable\", iarg1, ...\"\n"
"\"pylcall7i\";\"Python Opcodes\";\"Invoke the specified Python callable at k-time and i-time (i suffix), passing the given arguments. The call is perfomed in the global environment, and the result (the returning value) is copied into the Csound output variables specified.\";\"ir1, ir2, ir3, ir4, ir5, ir6, ir7 pylcall7i \"callable\", iarg1, ...\"\n"
"\"pylcall8i\";\"Python Opcodes\";\"Invoke the specified Python callable at k-time and i-time (i suffix), passing the given arguments. The call is perfomed in the global environment, and the result (the returning value) is copied into the Csound output variables specified.\";\"ir1, ir2, ir3, ir4, ir5, ir6, ir7, ir8 pylcall8i \"callable\", iarg1, ...\"\n"
"\"pylcalln\";\"Python Opcodes\";\"Invoke the specified Python callable at k-time and i-time (i suffix), passing the given arguments. The call is perfomed in the global environment, and the result (the returning value) is copied into the Csound output variables specified.\";\"pylcalln \"callable\", nresults, kresult1, ..., kresultn, karg1, ...\"\n"
"\"pylcallni\";\"Python Opcodes\";\"Invoke the specified Python callable at k-time and i-time (i suffix), passing the given arguments. The call is perfomed in the global environment, and the result (the returning value) is copied into the Csound output variables specified.\";\"pylcallni \"callable\", nresults, iresult1, ..., iresultn, iarg1, ...\"\n"
"\"pyeval\";\"Python Opcodes\";\"Evaluate a generic Python expression and store the result in a Csound variable at k-time or i-time (i suffix).\";\"kresult pyeval \"expression\"\"\n"
"\"pyevali\";\"Python Opcodes\";\"Evaluate a generic Python expression and store the result in a Csound variable at k-time or i-time (i suffix).\";\"iresult pyevali \"expression\"\"\n"
"\"pyleval\";\"Python Opcodes\";\"Evaluate a generic Python expression and store the result in a Csound variable at k-time or i-time (i suffix).\";\"kresult pyleval \"expression\"\"\n"
"\"pylevali\";\"Python Opcodes\";\"Evaluate a generic Python expression and store the result in a Csound variable at k-time or i-time (i suffix).\";\"iresult pylevali \"expression\"\"\n"
"\"pyevalt\";\"Python Opcodes\";\"Evaluate a generic Python expression and store the result in a Csound variable at k-time or i-time (i suffix).\";\"kresult pyevalt ktrigger, \"expression\"\"\n"
"\"pylevalt\";\"Python Opcodes\";\"Evaluate a generic Python expression and store the result in a Csound variable at k-time or i-time (i suffix).\";\"kresult pylevalt ktrigger, \"expression\"\"\n"
"\"pyexec\";\"Python Opcodes\";\"Execute a script from a file at k-time or i-time (i suffix).\";\"pyexec \"filename\"\"\n"
"\"pyexeci\";\"Python Opcodes\";\"Execute a script from a file at k-time or i-time (i suffix).\";\"pyexeci \"filename\"\"\n"
"\"pylexec\";\"Python Opcodes\";\"Execute a script from a file at k-time or i-time (i suffix).\";\"pylexec \"filename\"\"\n"
"\"pylexeci\";\"Python Opcodes\";\"Execute a script from a file at k-time or i-time (i suffix).\";\"pylexeci \"filename\"\"\n"
"\"pyexect\";\"Python Opcodes\";\"Execute a script from a file at k-time or i-time (i suffix).\";\"pyexect ktrigger, \"filename\"\"\n"
"\"plyexect\";\"Python Opcodes\";\"Execute a script from a file at k-time or i-time (i suffix).\";\"plyexect ktrigger, \"filename\"\"\n"
"\"pyinit\";\"Python Opcodes\";\"Initialize the Python interpreter.\";\"pyinit\"\n"
"\"pyrun\";\"Python Opcodes\";\"Run a Python statement or block of statements.\";\"pyrun \"statement\"\"\n"
"\"pyruni\";\"Python Opcodes\";\"Run a Python statement or block of statements.\";\"pyruni \"statement\"\"\n"
"\"pylrun\";\"Python Opcodes\";\"Run a Python statement or block of statements.\";\"pylrun \"statement\"\"\n"
"\"pylruni\";\"Python Opcodes\";\"Run a Python statement or block of statements.\";\"pylruni \"statement\"\"\n"
"\"pyrunt\";\"Python Opcodes\";\"Run a Python statement or block of statements.\";\"pyrunt ktrigger, \"statement\"\"\n"
"\"pylrunt\";\"Python Opcodes\";\"Run a Python statement or block of statements.\";\"pylrunt ktrigger, \"statement\"\"\n"
"\"imagecreate\";\"Image Processing Opcodes\";\"Create an empty image of a given size.\";\"iimagenum imagecreate iwidth, iheight\"\n"
"\"imagefree\";\"Image Processing Opcodes\";\"Frees memory allocated for a previously loaded or created image.\";\"imagefree iimagenum\"\n"
"\"imagegetpixel\";\"Image Processing Opcodes\";\"Return the RGB pixel values of a previously opened or created image.\";\"ared, agreen, ablue imagegetpixel iimagenum, ax, ay\"\n"
"\"imageload\";\"Image Processing Opcodes\";\"Load an image.\";\"iimagenum imageload filename\"\n"
"\"imagesave\";\"Image Processing Opcodes\";\"Save a previously created image.\";\"imagesave iimagenum, filename\"\n"
"\"imagesetpixel\";\"Image Processing Opcodes\";\"Set the RGB value of a pixel inside a previously opened or created image.\";\"imagesetpixel iimagenum, ax, ay, ared, agreen, ablue\"\n"
"\"imagesize\";\"Image Processing Opcodes\";\"Return the width and height of a previously opened or created image.\";\"iwidth, iheight imagesize iimagenum\"\n"
"\"fareylen\";\"Miscellaneous\";\"returns the length of a Farey Sequence.\";\"kfl fareylen kfn\"\n"
"\"fareyleni\";\"Miscellaneous\";\"returns the length of a Farey Sequence.\";\"ifl fareyleni ifn\"\n"
"\"modmatrix\";\"Miscellaneous\";\"Modulation matrix opcode with optimizations for sparse matrices.\";\"modmatrix iresfn, isrcmodfn, isrcparmfn, imodscale, inum_mod,  inum_parm, kupdate\"\n"
"\"system_i\";\"Miscellaneous\";\"Call an external program via the system call\";\"ires system_i itrig, Scmd, [inowait]\"\n"
"\"system\";\"Miscellaneous\";\"Call an external program via the system call\";\"kres system ktrig, Scmd, [knowait]\"\n"
"\"tableshuffle\";\"Miscellaneous\";\"shuffles the content of a function table so that each element of the source table is put into a different random position.\";\"tableshuffle ktablenum\"\n"
"\"tableshufflei\";\"Miscellaneous\";\"shuffles the content of a function table so that each element of the source table is put into a different random position.\";\"tableshufflei itablenum\"\n"
"\"bmopen\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"bmoscil\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"bmoscili\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"bmscan\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"bmscani\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"bmtable\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"bmtablei\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"call\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"calld\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"callm\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"callmd\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"DsCommitDeferredSettings\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"DsCommitDeferredSettings_i\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"DsConeAngles\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"DsConeAngles_i\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"DsConeOrientation\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"DsConeOrientation_i\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"DsConeOutsideVolume\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"DsConeOutsideVolume_i\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"DsEaxSetDeferredFlag\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"DsListenerDistanceFactor\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"DsListenerDistanceFactor_i\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"DsListenerOrientation\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"DsListenerOrientation_i\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"DsListenerPosition\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"DsListenerPosition_i\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"DsListenerRolloffFactor\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"DsListenerRolloffFactor_i\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"DsListenerSetAll\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"DsListenerSetAll_i\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"DsMaxDistance\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"DsMaxDistance_i\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"DsMinDistance\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"DsMinDistance_i\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"DsMode\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"DsMode_i\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"DsPosition\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"DsPosition_i\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"DsSetAll\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"DsSetAll_i\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"EaxListenerAirAbsorption\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"EaxListenerAirAbsorption_i\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"EaxListenerAll\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"EaxListenerAll_i\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"EaxListenerDecayTime\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"EaxListenerDecayTime_i\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"EaxListenerDecayTimeHfRatio\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"EaxListenerDecayTimeHfRatio_i\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"EaxListenerEnvDiffusion\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"EaxListenerEnvDiffusion_i\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"EaxListenerEnvironment\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"EaxListenerEnvironment_i\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"EaxListenerEnvSize\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"EaxListenerEnvSize_i\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"EaxListenerFlags\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"EaxListenerFlags_i\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"EaxListenerReflections\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"EaxListenerReflections_i\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"EaxListenerReflectionsDelay\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"EaxListenerReflectionsDelay_i\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"EaxListenerReverb\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"EaxListenerReverb_i\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"EaxListenerReverbDelay\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"EaxListenerReverbDelay_i\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"EaxListenerRoom\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"EaxListenerRoom_i\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"EaxListenerRoomHF\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"EaxListenerRoomHF_i\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"EaxListenerRoomRolloff\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"EaxListenerRoomRolloff_i\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"EaxSourceAirAbsorption\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"EaxSourceAirAbsorption_i\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"EaxsourceAll\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"EaxSourceAll_i\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"EaxSourceDirect\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"EaxSourceDirect_i\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"EaxSourceDirectHF\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"EaxSourceDirectHF_i\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"EaxSourceFlags\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"EaxSourceFlags_i\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"EaxSourceObstruction\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"EaxSourceObstruction_i\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"EaxSourceObstructionRatio\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"EaxSourceObstructionRatio_i\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"EaxSourceOcclusion\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"EaxSourceOcclusion_i\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"EaxSourceOcclusionRatio\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"EaxSourceOcclusionRatio_i\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"EaxSourceOcclusionRoomRatio\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"EaxSourceOcclusionRoomRatio_i\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"EaxSourceOutsideVolumeHF\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"EaxSourceOutsideVolumeHF_i\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"EaxSourceRoom\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"EaxSourceRoom_i\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"EaxSourceRoomHF\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"EaxSourceRoomHF_i\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"EaxSourceRoomRolloff\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"EaxSourceRoomRolloff_i\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"Flbox\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"FLgroup_end\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"FLpack_end\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"FLpanel_end\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"FLscroll_end\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"FLsetImage\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"FLsetOverlay\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"FLtabs_end\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"FLtextSize\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"fof3\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"foscili2\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"ftlen2\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GL3dStudioModel\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLabs\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLacos\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glArrayElement\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLasin\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLatan\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glBegin\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glBindTexture\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLbirnd\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glBlendFunc\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glCallList\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glClear\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glClearColor\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLclearwhen\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glClipPlane\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glColor\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glColor3\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glColorPointer\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLcos\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLcosh\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glCullFace\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glDisable\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glDisableClientState\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLdiv\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glDrawArrays\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glDrawBuffer\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glDrawElements\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glEdgeFlagPointer\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLelse\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glEnable\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glEnableClientState\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glEnd\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLend_for\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLend_if\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glEndList\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glEvalCoord1\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glEvalCoord2\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glEvalMesh1\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glEvalMesh2\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLexp\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLfont3d\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLfor\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLfps\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLfrac\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glFrontFace\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glFrustum\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLfullscreen\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glHint\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLif\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glIndexPointer\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLinsert\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLinsert_i\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLint\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glLight\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glLightModel\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glLightModelv\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glLightv\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glLineWidth\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glLoadIdentity\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glLoadMatrix\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glLoadMatrixv\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLloadPicasso\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLloadTexture\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLlog\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLlog10\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glMap1\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glMap2\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glMapGrid1\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glMapGrid2\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glMaterial\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glMatrixMode\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLmetro\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLmod\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLmul\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glMultMatrix\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glMultMatrixv\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glNewList\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glNormal3\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glNormalPointer\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLopenVideoFile\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glOrtho\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLoscil\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLoscili\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLpanel\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLpanel_end\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLphasor\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glPixelMap\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glPixelTransfer\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glPopMatrix\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glPushMatrix\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLratio\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLredraw\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLrender3dStudioModel\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLrenderPicasso\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLrnd\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glRotate\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glScale\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glShadeModel\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLshininess\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLsin\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLsinh\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLsqr\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLsub\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLsum\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLtab\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLtabw\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLtan\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLtanh\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLtexCircle\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glTexCoord2\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glTexCoordPointer\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLtexCube\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glTexEnvfv\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glTexEnvi\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glTexGenfv\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glTexGeni\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLtexSquare\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLtext3d\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glTranslate\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"gluBeginCurve\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"gluBeginSurface\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"gluBeginTrim\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"gluCylinder\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"gluDisk\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"gluEndCurve\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"gluEndSurface\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"gluEndTrim\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"gluLookAt\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"gluNewNurbsRenderer\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"gluNewQuadric\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"gluNurbsCurve\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"gluNurbsProperty\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"gluNurbsSurface\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"gluPerspective\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"gluPwlCurve\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"gluQuadricDrawStyle\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"gluQuadricNormals\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"gluQuadricTexture\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"gluSphere\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glutCube\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glutDodecahedron\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"gluTessBeginContour\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"gluTessBeginPolygon\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"gluTessEndContour\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"gluTessEndPolygon\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"gluTessVertex\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"gluTessVertexv\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glutIcosahedron\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glutOctahedron\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glutSphere\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glutTetrahedron\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glutTorus\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLvadd\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLvaddv\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLvcopy\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLvdivv\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glVertex3\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"glVertexPointer\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLvexp\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLvexpv\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLvideo2tex\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLvideoCapture\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLvlimit\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLvmap\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLvmirror\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLvmult\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLvmultv\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLvphaseseg\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLvpow\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLvpowv\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLvsubv\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLvtab\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLvtabw\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLvwrap\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLwaitInitStage\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"GLwavefrontModel\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"img2GLtex\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"imgConvolve\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"imgCreate\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"imgGain\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"imgPoint\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"Init3dAudio\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"InitEAX\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"loscil2\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"lposcint\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"lposcinta\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"lposcintsa\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"lposcintsa2\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"marimjpff\";\"CSoundAV Opcodes\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"add\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"and\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"cogoto\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"connect\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"convle\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"ctlchn\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"delayk\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"div\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"ephasor\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"FLhvsBoxSetValue\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"flooper3\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"FLsetVali\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"inleta\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"inletf\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"inletk\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"inletkid\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_add_mc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_add_mr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_add_vc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_add_vr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_assign_mc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_assign_mr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_assign_t\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_assign_vc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_assign_vr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_conjugate_mc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_conjugate_mr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_conjugate_vc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_conjugate_vr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_distance_vc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_distance_vr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_divide_mc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_divide_mr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_divide_vc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_divide_vr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_dot_mc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_dot_mc_vc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_dot_mr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_dot_mr_vr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_dot_vc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_dot_vr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_get_mc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_get_mr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_get_vc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_get_vr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_invert_mc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_invert_mr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_lower_solve_mc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_lower_solve_mr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_lu_det_mc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_lu_det_mr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_lu_factor_mc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_lu_factor_mr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_lu_solve_mc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_lu_solve_mr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_mc_create\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_mc_set\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_mr_create\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_mr_set\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_multiply_mc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_multiply_mr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_multiply_vc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_multiply_vr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_norm_euclid_mc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_norm_euclid_mr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_norm_euclid_vc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_norm_euclid_vr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_norm_inf_mc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_norm_inf_mr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_norm_inf_vc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_norm_inf_vr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_norm_max_mc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_norm_max_mr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_norm1_mc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_norm1_mr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_norm1_vc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_norm1_vr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_print_mc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_print_mr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_print_vc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_print_vr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_qr_eigen_mc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_qr_eigen_mr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_qr_factor_mc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_qr_factor_mr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_qr_sym_eigen_mc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_qr_sym_eigen_mr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_random_mc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_random_mr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_random_vc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_random_vr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_size_mc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_size_mr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_size_vc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_size_vr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_subtract_mc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_subtract_mr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_subtract_vc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_subtract_vr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_t_assign\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_trace_mc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_trace_mr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_transpose_mc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_transpose_mr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_upper_solve_mc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_upper_solve_mr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_vc_create\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_vc_set\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_vr_create\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_i_vr_set\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_a_assign\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_add_mc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_add_mr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_add_vc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_add_vr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_assign_a\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_assign_f\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_assign_mc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_assign_mr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_assign_t\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_assign_vc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_assign_vr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_conjugate_mc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_conjugate_mr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_conjugate_vc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_conjugate_vr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_current_f\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_current_vr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_distance_vc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_distance_vr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_divide_mc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_divide_mr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_divide_vc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_divide_vr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_dot_mc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_dot_mc_vc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_dot_mr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_dot_mr_vr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_dot_vc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_dot_vr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_f_assign\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_get_mc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_get_mr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_get_vc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_get_vr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_invert_mc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_invert_mr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_lower_solve_mc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_lower_solve_mr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_lu_det_mc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_lu_det_mr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_lu_factor_mc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_lu_factor_mr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_lu_solve_mc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_lu_solve_mr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_mc_set\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_mr_set\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_multiply_mc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_multiply_mr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_multiply_vc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_multiply_vr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_norm_euclid_mc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_norm_euclid_mr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_norm_euclid_vc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_norm_euclid_vr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_norm_inf_mc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_norm_inf_mr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_norm_inf_vc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_norm_inf_vr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_norm_max_mc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_norm_max_mr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_norm1_mc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_norm1_mr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_norm1_vc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_norm1_vr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_qr_eigen_mc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_qr_eigen_mr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_qr_factor_mc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_qr_factor_mr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_qr_sym_eigen_mc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_qr_sym_eigen_mr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_random_mc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_random_mr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_random_vc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_random_vr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_subtract_mc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_subtract_mr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_subtract_vc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_subtract_vr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_t_assign\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_trace_mc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_trace_mr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_upper_solve_mc\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_upper_solve_mr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_vc_set\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"la_k_vr_set\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"lpform\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"maxk\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"median\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"mediank\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"midipgm\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"mincer\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"mod\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"mul\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"mutex_lock\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"mutex_locki\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"mutex_unlock\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"mutex_unlocki\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"nlalp\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"not\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"or\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"OSCrecv\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"outleta\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"outletf\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"outletk\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"outletkid\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"pgmchn\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"pitchac\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"pvsenvftw\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"pvstanal\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"qinf\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"qnan\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"readks\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"setscorepos\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"shl\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"shr\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"sub\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"tablefilter\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"tablefilteri\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"tabref\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"temposcal\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"vdel_k\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"xor\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"until\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"enduntil\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"do\";\"Uncategorized\";\"(Description not available)\";\"(Syntax not available)\"\n"
"\"cabbageSet\";\"Set a Cabbage widget identifier\";\"Set a Cabbage widget identifier.\";\"cabbageSet kTrig, SChannel, SIdentifier, XArgs   |   cabbageSet kTrig, SChannel, SIdentifierString   |   cabbageSet SChannel, SIdentifierString\n"
"\"cabbageGet\";\"Get a Cabbage widget identifier\";\"Get a Cabbage widget identifier.\";\"kVal/kIdent[] [, kTrig] cabbageGet SChannel, SIdentifier   |   SVal/SIdent[] [, kTrig] cabbageGet SChannel, SIdentifier   |   kChannelValue [, kTrig] cabbageGet SChannel   |   SChannelValue [, kTrig] cabbageGet SChannel\n"
"\"cabbageGetValue\";\"Gets the value of Cabbage widget\";\"Gets the value of Cabbage widget.\";\"kValue [, kTrig] cabbageGetValue SChannel   |   SValue [, kTrig] cabbageGetValue SChannel   |   cabbageSetValue SChannel, kValue [, kTrig]\n"
"\"cabbageSetValue\";\"Sets the value of Cabbage widget\";\"Sets the value of Cabbage widget.\";\"cabbageSetValue SChannel, kValue [, kTrig]\n"
"\"cabbageCreate\";\"Creates a new Cabbage widget.\";\"Creates a new Cabbage widget. i-rate only\";\"cabbageCreate SCabbageCodeString\n";

static const char* const CsoundKeywords[] =
{
    "scale", "!=", "#define", "#include", "#undef", "#ifdef", "#ifndef", "$", "%", "&&", ">", ">=", "<", "<=", "*", "+", "-", "/", "=", "=+", "==", "ˆ", "||", "0dbfs", "<<", ">>", "&",
    "|", "¬", "#", "a", "alwayson", "ampmidid", "ATSadd", "ATSaddnz", "ATSbufread", "ATScross", "ATSinfo", "ATSinterpread", "ATSread", "ATSreadnz", "ATSpartialtap", "ATSsinnoi", "automatable", "barmodel", "bformenc1",
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
    "listbox", "hrange", "vrange", "active", "align", "alpha", "ampRange", "bounds", "caption", "channel", "channelArray", "channelType", "child", "colour", "colour:0", "colour:1", "corners", "displayType", "file",
    "fontColour", "fontStyle", "fontStyle", "guiRefresh", "highlightColour", "identChannel", "items", "latched", "line", "middleC", "max", "min", "mode", "outlineColour", "outlineThickness", "plant", "pluginId", "populate",
    "popup", "popupText", "popupPostfix", "popupPrefix", "range", "rangeX", "rangeY", "rotate", "sampleRange", "scrubberpos", "scrubberPosition", "shape", "show", "size", "sliderincr", "svgfile", "svgpath", "tableBackgroundColour", "tableColour",
    "tableGridColour", "tableNumber", "text", "textColour", "textBox", "trackerColour", "trackerThickness", "trackerOuterRadius", "trackerInnerRadius", "typeface", "widgetArray", "wrap", "value", "velocity", "visible", "zoom", "zkwm", "maxarray", "fillarray", "lenarray",
    "od", "gentable", "texteditor", "textBox", "sprintfk", "strcpyk", "sprintf", "strcmpk", "strcmp", "a", "abetarand", "abexprnd", "infobutton", "groupbox", "do", "popupmenu", "filebutton", "until",
    "enduntil", "soundfiler", "combobox", "vslider", "vslider2", "vslider3", "hslider2", "define", "hslider3", "hslider", "rslider", "groupbox", "combobox", "xypad", "image", "plant", "csoundoutput", "button", "optionbutton", "form", "checkbox",
    "tab", "abs", "acauchy", "active", "adsr", "adsyn", "adsynt", "adsynt2", "aexprand", "aftouch", "agauss", "agogobel", "alinrand", "alpass", "ampdb", "ampdbfs", "ampmidi", "apcauchy", "apoisson", "apow", "areson",
    "aresonk", "atone", "atonek", "atonex", "atrirand", "aunirand", "aweibull", "babo", "balance", "bamboo", "bbcutm", "bbcuts", "betarand", "bexprnd", "bformenc", "bformdec", "biquad", "biquada", "birnd",
    "bqrez", "butbp", "butbr", "buthp", "butlp", "butterbp", "butterbr", "butterhp", "butterlp", "button", "buzz", "cabasa", "cauchy",
    "ceil", "cent", "cggoto", "chanctrl", "changed", "chani", "chano", "checkbox", "chn", "chnclear", "chnexport", "chnget", "chnmix", "chnparams", "chnset", "cigoto", "ckgoto", "clear", "clfilt", "clip",
    "clock", "clockoff", "clockon", "cngoto", "comb", "control", "convle", "convolve", "cos", "cosh", "cosinv", "cps2pch", "cpsmidi", "cpsmidib", "cpsmidib", "cpsoct", "cpspch",
    "cpstmid", "cpstun", "cpstuni", "cpsxpch", "cpuprc", "cross2", "crunch", "ctrl14", "ctrl21", "ctrl7", "ctrlinit", "cuserrnd", "dam", "db", "dbamp", "dbfsamp", "dcblock", "dconv", "delay", "delay1",
    "delayk", "delayr", "delayw", "deltap", "deltap3", "deltapi", "deltapn", "deltapx", "deltapxw", "denorm", "diff", "diskin", "diskin2", "dispfft", "display", "distort1", "divz", "downsamp", "dripwater",
    "dssiactivate", "dssiaudio", "dssictls", "dssiinit", "dssilist", "dumpk", "dumpk2", "dumpk3", "dumpk4", "duserrnd", "else", "elseif", "endif", "endin", "endop", "envlpx", "envlpxr", "event", "event_i",
    "exitnow", "exp", "expon", "exprand", "expseg", "expsega", "expsegr", "filelen", "filenchnls", "filepeak", "filesr", "filter2", "fin", "fini", "fink", "fiopen", "flanger", "flashtxt", "FLbox", "FLbutBank",
    "FLbutton", "FLcolor", "FLcolor2", "FLcount", "FLgetsnap", "FLgroup", "FLgroupEnd", "FLgroupEnd", "FLhide", "FLjoy", "fontSize", "FLkeyb", "FLknob", "FLlabel", "FLloadsnap", "flooper", "floor", "FLpack", "FLpackEnd",
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
    "loop", "loopseg", "loopsegp", "lorenz", "lorisread", "lorismorph", "lorisplay", "loscil", "liveconv", "loscil3", "lowpass2", "lowres", "lineThickness", "lowresx", "lpf18", "lpfreson", "lphasor", "lpinterp", "lposcil", "lposcil3",
    "lpread", "lpreson", "lpshold", "lpsholdp", "lpslot", "mac", "maca", "madsr", "mandel", "mandol", "marimba", "massign", "maxalloc", "max_k", "mclock", "mdelay", "metro", "midic14", "midic21", "midic7",
    "midichannelaftertouch", "midichn", "midicontrolchange", "midictrl", "mididefault", "midiin", "midinoteoff", "midinoteoncps", "midinoteonkey", "midinoteonoct", "midinoteonpch", "midion", "midion2",
    "midiout", "midipitchbend", "midipolyaftertouch", "midiprogramchange", "miditempo", "mirror", "MixerSetLevel", "MixerGetLevel", "MixerSend", "MixerReceive", "MixerClear", "moog", "moogladder", "mouseInteraction"
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
    "ziwm", "zkcl", "zkmod", "zkr", "zkw", "zkwm ", "signaldisplay", "signalVariable", "updateRate", "menucolour", "ATSadd", "ATSaddnz", "ATSbufread", "ATScross", "ATSinfo", "ATSinterpread",
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
    "trlowest", "trmix", "trscale", "trshift", "trsplit", "unwrap", "urandom", "vactrol", "vadd_i", "vaddv_i", "vaget", "valuePostfix", "valuePrefix", "vaset", "vbap", "vbapg", "vbapgmove", "vbapmove", "vdel_k", "vdivv_i", "vexp_i", "vexpv_i", "vmult_i",
    "vmultv_i", "vosim", "vphaseseg", "vpow_i", "vpowv_i", "vsubv_i", "vtable1k", "wiiconnect", "wiidata", "wiirange", "wiisend", "window", "writescratch", "zkwm", "then", "while", "od", "do", "endwhile", "trigexpseg", "triglinseg", "setStateValue", "writeStateValue", "writeStateData", "readStateData", "channelStateRecall", "channelStateSave", "cabbageSetValue", "cabbageGetValue", "cabbageGet", "cabbageSet", "cabbageGetCurrentWidget", "guiMode", "cabbageCreate", "showsSrubber", "titleBarColour", "FLslidBnkGetHandle", "K35_hpf", "K35_lpf", "OSCbundle", "OSCcount", "OSCinitM", "OSCraw", "OSCsend_lo", "S", "a", "allpole", "ampmidicurve", "apoleparams", "arduinoRead", "arduinoStart", "arduinoStop", "balance2", "beadsynt", "beosc", "bob", "bpf", "bpfcos", "cbrt", "chngeta", "chngeti", "chngetk", "chngetks", "chngets", "chnseta", "chnseti", "chnsetk", "chnsetks", "chnsets", "cmp", "cntCreate", "cntCycles", "cntRead", "cntReset", "cntState", "count", "count_i", "dct", "dctinv", "deinterleave", "diode_ladder", "dot", "faustdsp", "faustplay", "fmanal", "fmax", "fmin", "fmod", "ftaudio", "ftexists", "ftom", "ftprint", "ftset", "ftslice", "ftslicei", "getftargs", "getrowlin", "gtf", "hilbert2", "hypot", "i", "interleave", "k", "lag", "lagud", "lastcycle", "limit1", "lincos", "linlin", "loscil3phs", "loscilphs", "lpcanal", "lpcfilter", "lufs", "metro2", "mfb", "midiarp", "midichannelaftertouch", "midicontrolchange", "midiout_i", "midipolyaftertouch", "midiprogramchange", "moogvcf", "mtof", "mton", "nstrstr", "ntof", "ntom", "nxtpow2", "p", "pchtom", "pows", "printarray", "println", "printsk", "pvsbandwidth", "pvscfs", "pvslpc", "pvstrace", "randc", "reshapearray", "resonbnk", "rndseed", "sc_lag", "sc_lagud", "sc_phasor", "sc_trig", "schedulek", "select", "slicearray_i", "sorta", "sortd", "squinewave", "sterrain", "string2array", "strstrip", "tab2array", "tabrowlin", "trighold", "trigphasor", "trim", "trim_i", "tvconv", "vps", "websocket", "xyscale", "zdf_1pole", "zdf_1pole_mode", "zdf_2pole", "zdf_2pole_mode", "zdf_ladder", "readOnly", "doubleClickTogglesEdit", "caretColour", "valueTextBox","showScrubber", "scrollbar", "sort", "textBoxOutlineColour", "filmstrip", "valueTextBoxBounds", "sliderBounds", "keySeparatorColour", "protectedItems" "blackNoteColour","whiteNoteColour", "markerColour", "mouseInteraction", "backgroundColour",
    0
};
#endif  // CABBAGECOMMANDIDS_H_INCLUDED
