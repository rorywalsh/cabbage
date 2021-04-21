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
    "vmultv_i", "vosim", "vphaseseg", "vpow_i", "vpowv_i", "vsubv_i", "vtable1k", "wiiconnect", "wiidata", "wiirange", "wiisend", "window", "writescratch", "zkwm", "then", "while", "od", "do", "endwhile", "trigexpseg", "triglinseg", "setStateValue", "writeStateValue", "writeStateData", "readStateData", "channelStateRecall", "channelStateSave", "cabbageSetValue", "cabbageGetValue", "cabbageGet", "cabbageSet", "cabbageGetCurrentWidget", "guiMode", "cabbageCreate", "showsSrubber", "titleBarColour", "FLslidBnkGetHandle", "K35_hpf", "K35_lpf", "OSCbundle", "OSCcount", "OSCinitM", "OSCraw", "OSCsend_lo", "S", "a", "allpole", "ampmidicurve", "apoleparams", "arduinoRead", "arduinoStart", "arduinoStop", "balance2", "beadsynt", "beosc", "bob", "bpf", "bpfcos", "cbrt", "chngeta", "chngeti", "chngetk", "chngetks", "chngets", "chnseta", "chnseti", "chnsetk", "chnsetks", "chnsets", "cmp", "cntCreate", "cntCycles", "cntRead", "cntReset", "cntState", "count", "count_i", "dct", "dctinv", "deinterleave", "diode_ladder", "dot", "faustdsp", "faustplay", "fmanal", "fmax", "fmin", "fmod", "ftaudio", "ftexists", "ftom", "ftprint", "ftset", "ftslice", "ftslicei", "getftargs", "getrowlin", "gtf", "hilbert2", "hypot", "i", "interleave", "k", "lag", "lagud", "lastcycle", "limit1", "lincos", "linlin", "loscil3phs", "loscilphs", "lpcanal", "lpcfilter", "lufs", "metro2", "mfb", "midiarp", "midichannelaftertouch", "midicontrolchange", "midiout_i", "midipolyaftertouch", "midiprogramchange", "moogvcf", "mtof", "mton", "nstrstr", "ntof", "ntom", "nxtpow2", "p", "pchtom", "pows", "printarray", "println", "printsk", "pvsbandwidth", "pvscfs", "pvslpc", "pvstrace", "randc", "reshapearray", "resonbnk", "rndseed", "sc_lag", "sc_lagud", "sc_phasor", "sc_trig", "schedulek", "select", "slicearray_i", "sorta", "sortd", "squinewave", "sterrain", "string2array", "strstrip", "tab2array", "tabrowlin", "trighold", "trigphasor", "trim", "trim_i", "tvconv", "vps", "websocket", "xyscale", "zdf_1pole", "zdf_1pole_mode", "zdf_2pole", "zdf_2pole_mode", "zdf_ladder", "readOnly", "doubleClickTogglesEdit", "caretColour", "valueTextBox","showScrubber", "scrollbar", "sort", "textBoxOutlineColour", "filmstrip", "valueTextBoxBounds", "sliderBounds", "keySeparatorColour", "protectedItems" "blackNoteColour","whiteNoteColour", "markerColour",
    0
};
#endif  // CABBAGECOMMANDIDS_H_INCLUDED
