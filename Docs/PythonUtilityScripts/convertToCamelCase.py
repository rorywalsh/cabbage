from os import listdir
from shutil import copyfile
from distutils.dir_util import copy_tree
import os.path
import os
import sys
from os.path import isfile, join, basename

print("Usage: python convertToCamelCase.py ./path_to_files")

if len(sys.argv) != 2:
	print("You must supply a directory")
	exit(1)

currentDir = os.path.abspath(sys.argv[1])
exampleFiles = listdir(currentDir)
os.chdir(currentDir)
print(currentDir)

newFileText = ""

camelCaseIdentifiers = []

camelCaseIdentifiers.append("scrubberposition_sample");
camelCaseIdentifiers.append("doubleClickTogglesEdit");
camelCaseIdentifiers.append("scrubberposition_table");
camelCaseIdentifiers.append("tableBackgroundColour");
camelCaseIdentifiers.append("arrowBackgroundColour");
camelCaseIdentifiers.append("amprange_tablenumber");
camelCaseIdentifiers.append("textBoxOutlineColour");
camelCaseIdentifiers.append("trackerOutsideRadius");
camelCaseIdentifiers.append("trackerInsideRadius");
camelCaseIdentifiers.append("surrogatelinenumber");
camelCaseIdentifiers.append("mouseOverKeyColour");
camelCaseIdentifiers.append("keypressBaseOctave");
camelCaseIdentifiers.append("keySeparatorColour");
camelCaseIdentifiers.append("amprange_quantise");
camelCaseIdentifiers.append("currentNoteColour");
camelCaseIdentifiers.append("backgroundColour");
camelCaseIdentifiers.append("trackerThickness");
camelCaseIdentifiers.append("activeCellColour");
camelCaseIdentifiers.append("outlineThickness");
camelCaseIdentifiers.append("scrubberPosition");
camelCaseIdentifiers.append("titleBarGradient");
camelCaseIdentifiers.append("mouseInteraction");
camelCaseIdentifiers.append("whiteNoteColour");
camelCaseIdentifiers.append("blackNoteColour");
camelCaseIdentifiers.append("highlightColour");
camelCaseIdentifiers.append("tableGridColour");
camelCaseIdentifiers.append("trackerBgColour");
camelCaseIdentifiers.append("backgroundColor");
camelCaseIdentifiers.append("showStepNumbers");
camelCaseIdentifiers.append("markerThickness");
camelCaseIdentifiers.append("titleBarColour");
camelCaseIdentifiers.append("tableGridColor");
camelCaseIdentifiers.append("signalVariable");
camelCaseIdentifiers.append("overlayColour");
camelCaseIdentifiers.append("keydownColour");
camelCaseIdentifiers.append("lineThickness");
camelCaseIdentifiers.append("outlineColour");
camelCaseIdentifiers.append("numberOfSteps");
camelCaseIdentifiers.append("textBoxColour");
camelCaseIdentifiers.append("trackerColour");
camelCaseIdentifiers.append("trackerRadius");
camelCaseIdentifiers.append("showSrubber");
camelCaseIdentifiers.append("markerColour");
camelCaseIdentifiers.append("valueTextBox");
camelCaseIdentifiers.append("fontColour:1");
camelCaseIdentifiers.append("amprange_min");
camelCaseIdentifiers.append("amprange_max");
camelCaseIdentifiers.append("trackerColor");
camelCaseIdentifiers.append("refreshFiles");
camelCaseIdentifiers.append("tableNumbers");
camelCaseIdentifiers.append("identChannel");
camelCaseIdentifiers.append("fontColour:0");
camelCaseIdentifiers.append("macroStrings");
camelCaseIdentifiers.append("tableColour:");
camelCaseIdentifiers.append("meterColour:");
camelCaseIdentifiers.append("popupPostfix");
camelCaseIdentifiers.append("manufacturer");
camelCaseIdentifiers.append("valuePostfix");
camelCaseIdentifiers.append("markerStart");
camelCaseIdentifiers.append("keyReleased");
camelCaseIdentifiers.append("orientation");
camelCaseIdentifiers.append("tableColour");
camelCaseIdentifiers.append("meterColour");
camelCaseIdentifiers.append("widgetArray");
camelCaseIdentifiers.append("arrowColour");
camelCaseIdentifiers.append("channelType");
camelCaseIdentifiers.append("sampleRange");
camelCaseIdentifiers.append("fontColor:0");
camelCaseIdentifiers.append("tableNumber");
camelCaseIdentifiers.append("displayType");
camelCaseIdentifiers.append("fontColor:1");
camelCaseIdentifiers.append("baseChannel");
camelCaseIdentifiers.append("popupPrefix");
camelCaseIdentifiers.append("automatable");
camelCaseIdentifiers.append("valuePrefix");
camelCaseIdentifiers.append("caretColour");
camelCaseIdentifiers.append("automation");
camelCaseIdentifiers.append("autoUpdate");
camelCaseIdentifiers.append("matrixSize");
camelCaseIdentifiers.append("fontColour");
camelCaseIdentifiers.append("textColour");
camelCaseIdentifiers.append("updateRate");
camelCaseIdentifiers.append("gapMarkers");
camelCaseIdentifiers.append("guiRefresh");
camelCaseIdentifiers.append("tableColor");
camelCaseIdentifiers.append("radioGroup");
camelCaseIdentifiers.append("macroNames");
camelCaseIdentifiers.append("ballColour");
camelCaseIdentifiers.append("keyPressed");
camelCaseIdentifiers.append("scrollBars");
camelCaseIdentifiers.append("cellHeight");
camelCaseIdentifiers.append("opcodeDir");
camelCaseIdentifiers.append("markerEnd");
camelCaseIdentifiers.append("menuColor");
camelCaseIdentifiers.append("cellWidth");
camelCaseIdentifiers.append("popupText");
camelCaseIdentifiers.append("textColor");
camelCaseIdentifiers.append("fontStyle");
camelCaseIdentifiers.append("fontColor");
camelCaseIdentifiers.append("filmStrip");
camelCaseIdentifiers.append("arraySize");
camelCaseIdentifiers.append("increment");
camelCaseIdentifiers.append("nameSpace");
camelCaseIdentifiers.append("rowPrefix");
camelCaseIdentifiers.append("colPrefix");
camelCaseIdentifiers.append("sideChain");
camelCaseIdentifiers.append("populate");		
camelCaseIdentifiers.append("keyWidth");
camelCaseIdentifiers.append("pluginId");
camelCaseIdentifiers.append("fontSize");
camelCaseIdentifiers.append("cvOutput");
camelCaseIdentifiers.append("imgDebug");
camelCaseIdentifiers.append("position");
camelCaseIdentifiers.append("colour:0");
camelCaseIdentifiers.append("colour:1");
camelCaseIdentifiers.append("typeface");
camelCaseIdentifiers.append("channels");
camelCaseIdentifiers.append("velocity");
camelCaseIdentifiers.append("ampRange");
camelCaseIdentifiers.append("cellData");
camelCaseIdentifiers.append("isparent");
camelCaseIdentifiers.append("latency");
camelCaseIdentifiers.append("cvInput");
camelCaseIdentifiers.append("color:0");
camelCaseIdentifiers.append("color:1");
camelCaseIdentifiers.append("caption");
camelCaseIdentifiers.append("textBox");
camelCaseIdentifiers.append("corners");
camelCaseIdentifiers.append("color:1");
camelCaseIdentifiers.append("middleC");
camelCaseIdentifiers.append("channel");
camelCaseIdentifiers.append("imgpath");
camelCaseIdentifiers.append("address");
camelCaseIdentifiers.append("imgFile");
camelCaseIdentifiers.append("visible");
camelCaseIdentifiers.append("latched");
camelCaseIdentifiers.append("reScale");
camelCaseIdentifiers.append("toFront");
camelCaseIdentifiers.append("guiMode");
camelCaseIdentifiers.append("bundle");
camelCaseIdentifiers.append("import");
camelCaseIdentifiers.append("bounds");
camelCaseIdentifiers.append("colour");
camelCaseIdentifiers.append("rangeY");
camelCaseIdentifiers.append("rangeX");
camelCaseIdentifiers.append("rotate");
camelCaseIdentifiers.append("active");
camelCaseIdentifiers.append("shape");
camelCaseIdentifiers.append("screw");
camelCaseIdentifiers.append("scale");
camelCaseIdentifiers.append("popup");
camelCaseIdentifiers.append("alpha");
camelCaseIdentifiers.append("items");
camelCaseIdentifiers.append("light");
camelCaseIdentifiers.append("color");
camelCaseIdentifiers.append("value");
camelCaseIdentifiers.append("range");
camelCaseIdentifiers.append("plant");
camelCaseIdentifiers.append("style");
camelCaseIdentifiers.append("align");
camelCaseIdentifiers.append("file");
camelCaseIdentifiers.append("wrap");
camelCaseIdentifiers.append("text");
camelCaseIdentifiers.append("crop");
camelCaseIdentifiers.append("name");
camelCaseIdentifiers.append("zoom");
camelCaseIdentifiers.append("size");
camelCaseIdentifiers.append("show");
camelCaseIdentifiers.append("kind");
camelCaseIdentifiers.append("fill");
camelCaseIdentifiers.append("mode");
camelCaseIdentifiers.append("min");
camelCaseIdentifiers.append("max");
camelCaseIdentifiers.append("pos");
camelCaseIdentifiers.append("bpm");

lowerCaseIdentifiers = []
lowerCaseIdentifiers.append("scrubberposition_sample");
lowerCaseIdentifiers.append("doubleclicktogglesedit");
lowerCaseIdentifiers.append("scrubberposition_table");
lowerCaseIdentifiers.append("tablebackgroundcolour");
lowerCaseIdentifiers.append("arrowbackgroundcolour");
lowerCaseIdentifiers.append("amprange_tablenumber");
lowerCaseIdentifiers.append("textboxoutlinecolour");
lowerCaseIdentifiers.append("trackeroutsideradius");
lowerCaseIdentifiers.append("trackerinsideradius");
lowerCaseIdentifiers.append("surrogatelinenumber");
lowerCaseIdentifiers.append("mouseoverkeycolour");
lowerCaseIdentifiers.append("keypressbaseoctave");
lowerCaseIdentifiers.append("keyseparatorcolour");
lowerCaseIdentifiers.append("amprange_quantise");
lowerCaseIdentifiers.append("currentnotecolour");
lowerCaseIdentifiers.append("backgroundcolour");
lowerCaseIdentifiers.append("trackerthickness");
lowerCaseIdentifiers.append("activecellcolour");
lowerCaseIdentifiers.append("outlinethickness");
lowerCaseIdentifiers.append("scrubberposition");
lowerCaseIdentifiers.append("titlebargradient");
lowerCaseIdentifiers.append("mouseinteraction");
lowerCaseIdentifiers.append("whitenotecolour");
lowerCaseIdentifiers.append("blacknotecolour");
lowerCaseIdentifiers.append("highlightcolour");
lowerCaseIdentifiers.append("tablegridcolour");
lowerCaseIdentifiers.append("trackerbgcolour");
lowerCaseIdentifiers.append("backgroundcolor");
lowerCaseIdentifiers.append("showstepnumbers");
lowerCaseIdentifiers.append("markerthickness");
lowerCaseIdentifiers.append("titlebarcolour");
lowerCaseIdentifiers.append("tablegridcolor");
lowerCaseIdentifiers.append("signalvariable");
lowerCaseIdentifiers.append("overlaycolour");
lowerCaseIdentifiers.append("keydowncolour");
lowerCaseIdentifiers.append("linethickness");
lowerCaseIdentifiers.append("outlinecolour");
lowerCaseIdentifiers.append("numberofsteps");
lowerCaseIdentifiers.append("textboxcolour");
lowerCaseIdentifiers.append("trackercolour");
lowerCaseIdentifiers.append("trackerradius");
lowerCaseIdentifiers.append("showscrubber");
lowerCaseIdentifiers.append("markercolour");
lowerCaseIdentifiers.append("valuetextbox");
lowerCaseIdentifiers.append("fontcolour:1");
lowerCaseIdentifiers.append("amprange_min");
lowerCaseIdentifiers.append("amprange_max");
lowerCaseIdentifiers.append("trackercolor");
lowerCaseIdentifiers.append("refreshfiles");
lowerCaseIdentifiers.append("tablenumbers");
lowerCaseIdentifiers.append("identchannel");
lowerCaseIdentifiers.append("fontcolour:0");
lowerCaseIdentifiers.append("macrostrings");
lowerCaseIdentifiers.append("tablecolour:");
lowerCaseIdentifiers.append("metercolour:");
lowerCaseIdentifiers.append("popuppostfix");
lowerCaseIdentifiers.append("manufacturer");
lowerCaseIdentifiers.append("valuepostfix");
lowerCaseIdentifiers.append("markerstart");
lowerCaseIdentifiers.append("keyreleased");
lowerCaseIdentifiers.append("orientation");
lowerCaseIdentifiers.append("tablecolour");
lowerCaseIdentifiers.append("metercolour");
lowerCaseIdentifiers.append("widgetarray");
lowerCaseIdentifiers.append("arrowcolour");
lowerCaseIdentifiers.append("channeltype");
lowerCaseIdentifiers.append("samplerange");
lowerCaseIdentifiers.append("fontcolor:0");
lowerCaseIdentifiers.append("tablenumber");
lowerCaseIdentifiers.append("displaytype");
lowerCaseIdentifiers.append("fontcolor:1");
lowerCaseIdentifiers.append("basechannel");
lowerCaseIdentifiers.append("popupprefix");
lowerCaseIdentifiers.append("automatable");
lowerCaseIdentifiers.append("valueprefix");
lowerCaseIdentifiers.append("caretcolour");
lowerCaseIdentifiers.append("automation");
lowerCaseIdentifiers.append("autoupdate");
lowerCaseIdentifiers.append("matrixsize");
lowerCaseIdentifiers.append("fontcolour");
lowerCaseIdentifiers.append("textcolour");
lowerCaseIdentifiers.append("updaterate");
lowerCaseIdentifiers.append("gapmarkers");
lowerCaseIdentifiers.append("guirefresh");
lowerCaseIdentifiers.append("tablecolor");
lowerCaseIdentifiers.append("radiogroup");
lowerCaseIdentifiers.append("macronames");
lowerCaseIdentifiers.append("ballcolour");
lowerCaseIdentifiers.append("keypressed");
lowerCaseIdentifiers.append("scrollbars");
lowerCaseIdentifiers.append("cellheight");
lowerCaseIdentifiers.append("opcodedir");
lowerCaseIdentifiers.append("markerend");
lowerCaseIdentifiers.append("menucolor");
lowerCaseIdentifiers.append("cellwidth");
lowerCaseIdentifiers.append("popuptext");
lowerCaseIdentifiers.append("textcolor");
lowerCaseIdentifiers.append("fontstyle");
lowerCaseIdentifiers.append("fontcolor");
lowerCaseIdentifiers.append("filmstrip");
lowerCaseIdentifiers.append("arraysize");
lowerCaseIdentifiers.append("increment");
lowerCaseIdentifiers.append("namespace");
lowerCaseIdentifiers.append("rowprefix");
lowerCaseIdentifiers.append("colprefix");
lowerCaseIdentifiers.append("sidechain");
lowerCaseIdentifiers.append("populate");
lowerCaseIdentifiers.append("keywidth");
lowerCaseIdentifiers.append("pluginid");
lowerCaseIdentifiers.append("fontsize");
lowerCaseIdentifiers.append("cvoutput");
lowerCaseIdentifiers.append("imgdebug");
lowerCaseIdentifiers.append("position");
lowerCaseIdentifiers.append("colour:0");
lowerCaseIdentifiers.append("colour:1");
lowerCaseIdentifiers.append("typeface");
lowerCaseIdentifiers.append("channels");
lowerCaseIdentifiers.append("velocity");
lowerCaseIdentifiers.append("amprange");
lowerCaseIdentifiers.append("celldata");
lowerCaseIdentifiers.append("isparent");
lowerCaseIdentifiers.append("latency");
lowerCaseIdentifiers.append("cvinput");
lowerCaseIdentifiers.append("color:0");
lowerCaseIdentifiers.append("color:1");
lowerCaseIdentifiers.append("caption");
lowerCaseIdentifiers.append("textbox");
lowerCaseIdentifiers.append("corners");
lowerCaseIdentifiers.append("color:1");
lowerCaseIdentifiers.append("middlec");
lowerCaseIdentifiers.append("channel");
lowerCaseIdentifiers.append("imgpath");
lowerCaseIdentifiers.append("address");
lowerCaseIdentifiers.append("imgfile");
lowerCaseIdentifiers.append("visible");
lowerCaseIdentifiers.append("latched");
lowerCaseIdentifiers.append("rescale");
lowerCaseIdentifiers.append("tofront");
lowerCaseIdentifiers.append("guimode");
lowerCaseIdentifiers.append("bundle");
lowerCaseIdentifiers.append("import");
lowerCaseIdentifiers.append("bounds");
lowerCaseIdentifiers.append("colour");
lowerCaseIdentifiers.append("rangey");
lowerCaseIdentifiers.append("rangex");
lowerCaseIdentifiers.append("rotate");
lowerCaseIdentifiers.append("active");
lowerCaseIdentifiers.append("shape");
lowerCaseIdentifiers.append("screw");
lowerCaseIdentifiers.append("scale");
lowerCaseIdentifiers.append("popup");
lowerCaseIdentifiers.append("alpha");
lowerCaseIdentifiers.append("items");
lowerCaseIdentifiers.append("light");
lowerCaseIdentifiers.append("color");
lowerCaseIdentifiers.append("value");
lowerCaseIdentifiers.append("range");
lowerCaseIdentifiers.append("plant");
lowerCaseIdentifiers.append("style");
lowerCaseIdentifiers.append("align");
lowerCaseIdentifiers.append("file");
lowerCaseIdentifiers.append("wrap");
lowerCaseIdentifiers.append("text");
lowerCaseIdentifiers.append("crop");
lowerCaseIdentifiers.append("name");
lowerCaseIdentifiers.append("zoom");
lowerCaseIdentifiers.append("size");
lowerCaseIdentifiers.append("show");
lowerCaseIdentifiers.append("kind");
lowerCaseIdentifiers.append("fill");
lowerCaseIdentifiers.append("mode");
lowerCaseIdentifiers.append("min");
lowerCaseIdentifiers.append("max");
lowerCaseIdentifiers.append("pos");
lowerCaseIdentifiers.append("bpm");


for root, dirs, files in os.walk(currentDir):
    for f in files:
        filename = os.path.relpath(os.path.join(root, f), ".")
        newFileText = ""
        if ".csd" in filename:
            print(filename)
            


            with open(filename, "rt") as inputFile:
                newFileText = inputFile.read()
                for i in range(len(lowerCaseIdentifiers)):
                    if lowerCaseIdentifiers[i] != camelCaseIdentifiers[i]:
                        # print("repalcing " + lowerCaseIdentifiers[i] + "with " + camelCaseIdentifiers[i])
                        newFileText = newFileText.replace(lowerCaseIdentifiers[i], camelCaseIdentifiers[i])
            #     for line in inputFile:
            #         if line.find("sr") > -1:
            #             if line[0:line.find("=")].replace(' ', '').replace('\t', '') == "sr":
            #                 line = ";sr is set by the host\n"
            #         # if "form" in line and "caption" in line and "size" in line:
            #         #     line = line.replace('\n', '') +" style(\"legacy\")\n"

            #         newFileText = newFileText+line

            with open(filename, "w") as f:
                f.write(newFileText)

