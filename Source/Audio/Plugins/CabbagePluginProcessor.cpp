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

#include "CabbagePluginProcessor.h"
#include "CabbagePluginEditor.h"

char tmp_string[4096] = {0};
char channelMessage[4096] = {0};

AudioProcessor *JUCE_CALLTYPE

createPluginFilter() {
    File csdFile;
#ifdef JUCE_WINDOWS
	CabbageUtilities::debug(JucePlugin_Manufacturer);
    csdFile = File::getSpecialLocation(File::currentExecutableFile).withFileExtension(String(".csd")).getFullPathName();
	if (csdFile.existsAsFile() == false)
	{
		String filename = "C:/ProgramData/" + String(JucePlugin_Manufacturer) + "/" + File::getSpecialLocation(File::currentExecutableFile).getFileNameWithoutExtension()+"/"+ File::getSpecialLocation(File::currentExecutableFile).withFileExtension(String(".csd")).getFileName();
		
		csdFile = File(filename);
	}
#elif JUCE_MAC
    //read .csd file from the correct location within the .vst bundle.
    const String dir = File::getSpecialLocation (File::currentExecutableFile).getParentDirectory().getParentDirectory().getFullPathName();
    const String filename (File::getSpecialLocation (File::currentExecutableFile).withFileExtension (String (".csd")).getFileName());
    csdFile = File (dir + "/" + filename);
#else
    CabbageUtilities::debug(JucePlugin_Manufacturer);
    csdFile = File::getSpecialLocation(File::currentExecutableFile).withFileExtension(String(".csd")).getFullPathName();
    if (csdFile.existsAsFile() == false)
    {
        String filename = "/usr/share/" + String(JucePlugin_Manufacturer) + "/" + File::getSpecialLocation(File::currentExecutableFile).getFileNameWithoutExtension()+"/"+ File::getSpecialLocation(File::currentExecutableFile).withFileExtension(String(".csd")).getFileName();

        csdFile = File(filename);
    }
#endif

	if (csdFile.existsAsFile() == false)
		Logger::writeToLog("Could not find .csd file, please make sure it's in the correct folder");

    const int numChannels = CabbageUtilities::getHeaderInfo(csdFile.loadFileAsString(), "nchnls");
    return new CabbagePluginProcessor(csdFile, numChannels, numChannels);
};

//============================================================================
CabbagePluginProcessor::CabbagePluginProcessor(File inputFile, const int ins, const int outs)
        : CsoundPluginProcessor(inputFile, ins, outs),
          csdFile(inputFile),
          cabbageWidgets("CabbageWidgetData") 
{
	createCsound(inputFile);
}


void CabbagePluginProcessor::createCsound(File inputFile, bool shouldCreateParameters)
{
    if (inputFile.existsAsFile()) {
        setWidthHeight();
        StringArray linesFromCsd;
        linesFromCsd.addLines(inputFile.loadFileAsString());

        //only create extended temp file if imported plants are being added...
        if( addImportFiles(linesFromCsd) == true )
        {
        parseCsdFile(linesFromCsd);


        File tempFile = File::createTempFile(inputFile.getFileNameWithoutExtension()+"_temp.csd");
        tempFile.replaceWithText(linesFromCsd.joinIntoString("\n")
                                         .replace("$lt;", "<")
                                         .replace("&amp;", "&")
                                         .replace("$quote;", "\"")
                                         .replace("$gt;", ">"));




        if (setupAndCompileCsound(tempFile, inputFile.getParentDirectory(), samplingRate) == false)
            this->suspendProcessing(true);
            
        }
        
        else{
            parseCsdFile(linesFromCsd);
            if (setupAndCompileCsound(inputFile, inputFile.getParentDirectory(), samplingRate) == false)
                this->suspendProcessing(true);
        }
        
        if (shouldCreateParameters)
            createParameters();

        csoundChanList = NULL;

        initAllCsoundChannels(cabbageWidgets);

    }
}

CabbagePluginProcessor::~CabbagePluginProcessor() {
    for (auto xyAuto : xyAutomators)
        xyAuto->removeAllChangeListeners();

    xyAutomators.clear();

//    cabbageWidgets.removeAllChildren(nullptr);
//    cabbageWidgets.removeAllProperties(nullptr);
}

//==============================================================================
void CabbagePluginProcessor::setWidthHeight() {
    StringArray csdLines;
    csdLines.addLines(csdFile.loadFileAsString());

    for (auto line : csdLines) {
        if(line.contains("</Cabbage>"))
            return;
        
        ValueTree temp("temp");
        CabbageWidgetData::setWidgetState(temp, line, 0);

        if (CabbageWidgetData::getStringProp(temp, CabbageIdentifierIds::type) == CabbageWidgetTypes::form) {
            screenHeight = CabbageWidgetData::getNumProp(temp, CabbageIdentifierIds::height);
            screenWidth = CabbageWidgetData::getNumProp(temp, CabbageIdentifierIds::width);
        }
    }
}

void CabbagePluginProcessor::parseCsdFile(StringArray &linesFromCsd) {
    cabbageWidgets.removeAllChildren(0);
    String parentComponent, previousComponent;
    StringArray parents;


    getMacros(linesFromCsd);

    for (int lineNumber = 0; lineNumber < linesFromCsd.size(); lineNumber++) {
        if (linesFromCsd[lineNumber].equalsIgnoreCase("</Cabbage>"))
            return;

        const String widgetTreeIdentifier = "WidgetFromLine_" + String(lineNumber);
        ValueTree tempWidget(widgetTreeIdentifier);

        String currentLineOfCabbageCode = linesFromCsd[lineNumber].replace("\t", " ");

        if (currentLineOfCabbageCode.contains(" \\")) {
            for (int index = lineNumber + 1;; index++) {

                if (linesFromCsd[index].contains(" \\"))
                    currentLineOfCabbageCode += linesFromCsd[index];
                else {
                    currentLineOfCabbageCode += linesFromCsd[index];
                    break;
                }

            }
        }


        expandMacroText(currentLineOfCabbageCode, tempWidget);
        //note whether lines contains opening and closing bracket so GUI editor can add them back in
        if (currentLineOfCabbageCode.contains("{"))
            CabbageWidgetData::setNumProp(tempWidget, "containsOpeningCurlyBracket", 1);

        if (currentLineOfCabbageCode.contains("}"))
            CabbageWidgetData::setNumProp(tempWidget, "containsClosingCurlyBracket", 1);

        if (currentLineOfCabbageCode.indexOf(";") > -1)
            currentLineOfCabbageCode = currentLineOfCabbageCode.substring(0, currentLineOfCabbageCode.indexOf(";"));

        const String comments = currentLineOfCabbageCode.indexOf(";") == -1 ? "" : currentLineOfCabbageCode.substring(
                currentLineOfCabbageCode.indexOf(";"));
        CabbageWidgetData::setWidgetState(tempWidget, currentLineOfCabbageCode.trimCharactersAtStart(" \t") + comments,
                                          lineNumber);


        if (shouldClosePlant(linesFromCsd, lineNumber))
            parents.remove(parents.size() - 1);

        if (parents.size() > 0)
            CabbageWidgetData::setStringProp(tempWidget, CabbageIdentifierIds::parentcomponent,
                                             parents[parents.size() - 1]);

        CabbageWidgetData::setNumProp(tempWidget, CabbageIdentifierIds::linenumber, lineNumber - linesToSkip);
        CabbageWidgetData::setStringProp(tempWidget, CabbageIdentifierIds::csdfile, csdFile.getFullPathName());


        CabbageWidgetData::setProperty(tempWidget, CabbageIdentifierIds::macronames, macroNames);
        CabbageWidgetData::setProperty(tempWidget, CabbageIdentifierIds::macrostrings, macroStrings);


        const String typeOfWidget = CabbageWidgetData::getStringProp(tempWidget, CabbageIdentifierIds::type);
        for (int i = 0; i < plantStructs.size(); i++) {
            if (plantStructs[i].name == typeOfWidget)
                linesToSkip += plantStructs[i].cabbageCode.size() + 1;
        }

        if (typeOfWidget == CabbageWidgetTypes::form) {
            const String caption = CabbageWidgetData::getStringProp(tempWidget, CabbageIdentifierIds::caption);
            setPluginName(caption.length() > 0 ? caption : "Untitled");

            if (CabbageWidgetData::getNumProp(tempWidget, CabbageIdentifierIds::logger) == 1)
                createFileLogger(this->csdFile);

            setGUIRefreshRate(CabbageWidgetData::getNumProp(tempWidget, CabbageIdentifierIds::guirefresh));
        }

        const String precedingCharacters = currentLineOfCabbageCode.substring(0, currentLineOfCabbageCode.indexOf(
                typeOfWidget));
        CabbageWidgetData::setStringProp(tempWidget, "precedingCharacters", precedingCharacters);

        const String widgetName = CabbageWidgetData::getStringProp(tempWidget, CabbageIdentifierIds::name);

        if (widgetName.isNotEmpty())
            cabbageWidgets.addChild(tempWidget, -1, 0);

        if (CabbageWidgetData::getProperty(tempWidget, CabbageIdentifierIds::widgetarray).size() > 0 &&
            CabbageWidgetData::getProperty(tempWidget, CabbageIdentifierIds::identchannelarray).size() > 0) {
            for (int i = 0;
                 i < CabbageWidgetData::getProperty(tempWidget, CabbageIdentifierIds::widgetarray).size(); i++) {
                ValueTree copy = tempWidget.createCopy();
                const String chan = CabbageWidgetData::getProperty(tempWidget,
                                                                   CabbageIdentifierIds::widgetarray)[i].toString();
                const String iChan = CabbageWidgetData::getProperty(tempWidget,
                                                                    CabbageIdentifierIds::identchannelarray)[i].toString();
                const String name =
                        CabbageWidgetData::getStringProp(tempWidget, CabbageIdentifierIds::name) + String(9999 + i);
                CabbageWidgetData::setStringProp(copy, CabbageIdentifierIds::name, name);
                CabbageWidgetData::setStringProp(copy, CabbageIdentifierIds::channel,
                                                 CabbageWidgetData::getProperty(tempWidget,
                                                                                CabbageIdentifierIds::widgetarray)[i]);
                CabbageWidgetData::setStringProp(copy, CabbageIdentifierIds::identchannel,
                                                 CabbageWidgetData::getProperty(tempWidget,
                                                                                CabbageIdentifierIds::identchannelarray)[i]);
                cabbageWidgets.addChild(copy, -1, 0);
            }
        }


        if (isWidgetPlantParent(linesFromCsd, lineNumber) &&
            currentLineOfCabbageCode.removeCharacters(" ").removeCharacters("\t").substring(0, 1) != "{") {
            //CabbageUtilities::debug (CabbageWidgetData::getProperty (tempWidget, CabbageIdentifierIds::name).toString());
            parents.add(CabbageWidgetData::getProperty(tempWidget, CabbageIdentifierIds::name).toString());
        }


    }
}

bool CabbagePluginProcessor::isWidgetPlantParent(StringArray linesFromCsd, int lineNumber) {
    if (linesFromCsd[lineNumber].contains("{"))
        return true;

    if (linesFromCsd[lineNumber + 1].removeCharacters(" ").removeCharacters("\t").substring(0, 1) == "{")
        return true;

    return false;
}

bool CabbagePluginProcessor::shouldClosePlant(StringArray linesFromCsd, int lineNumber) {
    if (linesFromCsd[lineNumber].contains("}"))
        return true;

    return false;
}

bool CabbagePluginProcessor::addImportFiles(StringArray &linesFromCsd) {

    getMacros(linesFromCsd);
    bool hasImportFiles = false;
    for (int i = 0; i < linesFromCsd.size(); i++) {
        ValueTree temp("temp");
        String newLine = linesFromCsd[i];
        expandMacroText(newLine, temp);
        CabbageWidgetData::setWidgetState(temp, newLine, 0);

        //if form, check for import files..
        if (CabbageWidgetData::getStringProp(temp, CabbageIdentifierIds::type) == CabbageWidgetTypes::form) {
            var files = CabbageWidgetData::getProperty(temp, CabbageIdentifierIds::importfiles);
            
            if(files.size()>0)
                hasImportFiles = true;
            
            for (int y = 0; y < files.size(); y++) {
                CabbageUtilities::debug(
                        csdFile.getParentDirectory().getChildFile(files[y].toString()).getFullPathName());

                if (csdFile.getParentDirectory().getChildFile(files[y].toString()).existsAsFile()) {
                    StringArray linesFromImportedFile;
                    linesFromImportedFile.addLines(
                            csdFile.getParentDirectory().getChildFile(files[y].toString()).loadFileAsString());

                    std::unique_ptr<XmlElement> xml(XmlDocument::parse(CabbageUtilities::getPlantFileAsXmlString(
                            csdFile.getParentDirectory().getChildFile(files[y].toString()))));

                    if (!xml) //if plain text...
                    {
                        for (int y = linesFromImportedFile.size(); y >= 0; y--) {
                            linesFromCsd.insert(i + 1, linesFromImportedFile[y]);
                        }
                    } else//if plant xml
                    {
                        handleXmlImport(xml.get(), linesFromCsd);
                    }
                }
            }
        }
    }

    // once all plants have been imported to plantStructs array,
    // add them to Cabbage section
    insertPlantCode(linesFromCsd);
    
    return hasImportFiles;
}

void CabbagePluginProcessor::handleXmlImport(XmlElement *xml, StringArray &linesFromCsd) {
    PlantImportStruct importData;

    if (xml->hasTagName("plant")) {
        forEachXmlChildElement(*xml, e)
        {
            if (e->getTagName() == "namespace")
                importData.nsp = e->getAllSubText();

            if (e->getTagName() == "name")
                importData.name = e->getAllSubText();

            if (e->getTagName() == "cabbagecode")
                importData.cabbageCode.addLines(e->getAllSubText().replace("\t", " ").trim());

            if (e->getTagName() == "csoundcode")
                importData.csoundCode = e->getAllSubText();

            if (e->getTagName() == "cabbagecodescript")
                generateCabbageCodeFromJS(importData, e->getAllSubText());
        }


        //numberOfLinesInPlantCode += importData.cabbageCode.size()+1;
        insertUDOCode(importData, linesFromCsd);
        plantStructs.add(importData);
    }
}

void CabbagePluginProcessor::insertPlantCode(StringArray &linesFromCsd) {
    getMacros(linesFromCsd);

    const StringArray copy = linesFromCsd;
    int numberOfImportedLines = 0;

    for (int lineIndex = 0; lineIndex < linesFromCsd.size(); lineIndex++) {
        String currentLineOfCode = linesFromCsd[lineIndex];
        if (currentLineOfCode.trim().startsWith("</Cabbage>"))
            return;
        if (currentLineOfCode.isNotEmpty() && currentLineOfCode.substring(0, 1) != ";") {

            float scaleX = 1;
            float scaleY = 1;
            StringArray importedLines("");
            ValueTree temp("temp");
            expandMacroText(currentLineOfCode, temp);
            CabbageWidgetData::setWidgetState(temp, currentLineOfCode.trim(), lineIndex);
            const String type = CabbageWidgetData::getStringProp(temp, CabbageIdentifierIds::type);
            const String nsp = CabbageWidgetData::getStringProp(temp, CabbageIdentifierIds::nsp);

            bool isPlantWidget = true;
            for (int plantIndex = 0; plantIndex < plantStructs.size(); plantIndex++) {
//                CabbageUtilities::debug(type + " " + plantStructs[plantIndex].name.trim());
//                CabbageUtilities::debug(nsp + " " + plantStructs[plantIndex].nsp.trim());
                if (type == plantStructs[plantIndex].name.trim() && plantStructs[plantIndex].nsp.trim() == nsp) {
                    int lineNumberPlantAppearsOn;

                    for (auto plantCode : plantStructs[plantIndex].cabbageCode) {
                        if (plantCode.isNotEmpty()) {
                            if (plantCode.contains("}") == false) {
                                ValueTree temp1("temp1");
                                expandMacroText(plantCode, temp);
                                CabbageWidgetData::setWidgetState(temp1, plantCode.trim(), -99);
                                CabbageWidgetData::setNumProp(temp1, CabbageIdentifierIds::plant,
                                                              plantStructs[plantIndex].cabbageCode.size() + 2);
                                CabbageWidgetData::setNumProp(temp, CabbageIdentifierIds::plant,
                                                              plantStructs[plantIndex].cabbageCode.size() + 2);
                                lineNumberPlantAppearsOn = copy.indexOf(currentLineOfCode.trim());
                                CabbageWidgetData::getNumProp(temp, CabbageIdentifierIds::linenumber);
                                CabbageWidgetData::setNumProp(temp1, CabbageIdentifierIds::surrogatelinenumber,
                                                              lineNumberPlantAppearsOn);

                                if (isPlantWidget) {
                                    scaleX = CabbageWidgetData::getNumProp(temp, CabbageIdentifierIds::width) /
                                             CabbageWidgetData::getNumProp(temp1, CabbageIdentifierIds::width);
                                    scaleY = CabbageWidgetData::getNumProp(temp, CabbageIdentifierIds::height) /
                                             CabbageWidgetData::getNumProp(temp1, CabbageIdentifierIds::height);
                                    CabbageWidgetData::setBounds(temp1, CabbageWidgetData::getBounds(temp));
                                } else {
                                    const float width =
                                            CabbageWidgetData::getNumProp(temp1, CabbageIdentifierIds::width) * scaleX;
                                    const float height =
                                            CabbageWidgetData::getNumProp(temp1, CabbageIdentifierIds::height) * scaleY;
                                    const float top =
                                            CabbageWidgetData::getNumProp(temp1, CabbageIdentifierIds::top) * scaleY;
                                    const float left =
                                            CabbageWidgetData::getNumProp(temp1, CabbageIdentifierIds::left) * scaleX;
                                    CabbageWidgetData::setNumProp(temp1, CabbageIdentifierIds::width, width);
                                    CabbageWidgetData::setNumProp(temp1, CabbageIdentifierIds::height, height);
                                    CabbageWidgetData::setNumProp(temp1, CabbageIdentifierIds::top, top);
                                    CabbageWidgetData::setNumProp(temp1, CabbageIdentifierIds::left, left);

                                }

                                //add test for multiple channels...
                                const String currentChannel = CabbageWidgetData::getStringProp(temp1,
                                                                                               CabbageIdentifierIds::channel);
                                const String channelPrefix = CabbageWidgetData::getStringProp(temp,
                                                                                              CabbageIdentifierIds::channel);
                                const String currentIdentChannel = CabbageWidgetData::getStringProp(temp1,
                                                                                                    CabbageIdentifierIds::identchannel);


                                CabbageWidgetData::setStringProp(temp1, CabbageIdentifierIds::channel,
                                                                 channelPrefix + currentChannel);

                                if (CabbageWidgetData::getStringProp(temp1,
                                                                     CabbageIdentifierIds::identchannel).isNotEmpty())
                                    CabbageWidgetData::setStringProp(temp1, CabbageIdentifierIds::identchannel,
                                                                     channelPrefix + currentIdentChannel);

                                CabbageWidgetData::setProperty(temp1, CabbageIdentifierIds::macronames, macroNames);
                                CabbageWidgetData::setProperty(temp1, CabbageIdentifierIds::macrostrings, macroStrings);

                                String replacementText = (plantCode.indexOf("{") != -1 ?
                                                          CabbageWidgetData::getCabbageCodeFromIdentifiers(temp1,
                                                                                                           plantCode) +
                                                          "{"
                                                                                       : CabbageWidgetData::getCabbageCodeFromIdentifiers(
                                                temp1, plantCode));

                                importedLines.add(replacementText);
                                isPlantWidget = false;
                            } else
                                importedLines.add("}");

                        }
                    }

                    for (int y = 0; y < importedLines.size(); y++) {
                        linesFromCsd.insert(lineIndex + y, importedLines[y]);
                    }
                    lineIndex += importedLines.size();

                    numberOfImportedLines = importedLines.size();
                    importedLines.clear();

                }
            }


        }

    }
}


void CabbagePluginProcessor::insertUDOCode(PlantImportStruct importData, StringArray &linesFromCsd) {
    //todo don't check blocks of commented code
    for (const auto str : linesFromCsd) {
        //str = str.replaceCharacters("\t", " ").trim();
        if (str == "<CsInstruments>") {
            StringArray strArray;
            strArray.addLines(importData.csoundCode);

            const int lineToInsertTo = linesFromCsd.indexOf(str) + 1;

            for (int y = strArray.size(); y >= 0; y--)
                linesFromCsd.insert(lineToInsertTo, strArray[y]);

            return;
        }
    }
}

void CabbagePluginProcessor::generateCabbageCodeFromJS(PlantImportStruct &importData, String text) {
    JavascriptEngine engine;
    engine.maximumExecutionTime = RelativeTime::seconds(5);
    engine.registerNativeObject("Cabbage", new CabbageJavaClass(this));

    const double startTime = Time::getMillisecondCounterHiRes();

    Result result = engine.execute(text.replace("$lt;", "<")
                                           .replace("&amp;", "&")
                                           .replace("$quote;", "\"")
                                           .replace("$gt;", ">"));

    const double elapsedMs = Time::getMillisecondCounterHiRes() - startTime;

    importData.cabbageCode.addLines(cabbageScriptGeneratedCode.joinIntoString("\n"));

    if (result.failed())
        CabbageUtilities::showMessage("javaScript Error:" + result.getErrorMessage(),
                                      &getActiveEditor()->getLookAndFeel());

}


void CabbagePluginProcessor::getMacros(StringArray &linesFromCsd) {
    var tempMacroNames, tempMacroStrings;

    for (String csdLine : linesFromCsd) //deal with Cabbage macros
    {
        StringArray tokens;
        csdLine = csdLine.replace("\n", " ");
        tokens.addTokens(csdLine, ", ");

        if (tokens[0].containsIgnoreCase("define")) {
            tokens.removeEmptyStrings();

            if (tokens.size() > 1) {
                const String currentMacroText =
                        csdLine.substring(csdLine.indexOf(tokens[1]) + tokens[1].length()) + " ";
                macroText.set("$" + tokens[1], " " + currentMacroText);
                tempMacroNames.append("$" + tokens[1]);
                tempMacroStrings.append(currentMacroText.trim());
                macroNames = tempMacroNames;
                macroStrings = tempMacroStrings;
            }
        }
    }

    macroText.set("$SCREEN_WIDTH", " " + String(screenWidth));
    macroText.set("$SCREEN_HEIGHT", " " + String(screenHeight));
    macroNames.append("$SCREEN_WIDTH");
    macroNames.append("$SCREEN_HEIGHT");
    macroStrings.append(String(screenWidth));
    macroStrings.append(String(screenHeight));


}

void CabbagePluginProcessor::expandMacroText(String &line, ValueTree wData) {
    String csdLine;
    var macroNames;
    String defineText;
    String newLine = line;
    String expandedLine = line;


    StringArray tokens;
    tokens.addTokens(line, " ,");

    for (auto token : tokens) {
        if (token.startsWith("$")) {
            for (auto macro : macroText) {
                const String stringToReplace = token.removeCharacters(",() ");

                if (macro.name.toString() == stringToReplace) {
                    line = line.replace(stringToReplace, macro.value.toString());
                }
            }
        }

    }
}

//rebuild the entire GUi each time something changes.
void CabbagePluginProcessor::updateWidgets(String csdText) {
    CabbagePluginEditor *editor = static_cast<CabbagePluginEditor *> (this->getActiveEditor());
    StringArray strings;
    strings.addLines(csdText);
    parseCsdFile(strings);
    editor->createEditorInterface(cabbageWidgets);
    editor->updateLayoutEditorFrames();
}

//==============================================================================
// create parameters for sliders, buttons, comboboxes, checkboxes, encoders and xypads.
// Other widgets can communicate with Csound, but they cannot be automated
void CabbagePluginProcessor::createParameters() {
    CabbageControlWidgetStrings controlWidgetTypes;

    for (int i = 0; i < cabbageWidgets.getNumChildren(); i++) {
        const String typeOfWidget = CabbageWidgetData::getStringProp(cabbageWidgets.getChild(i),
                                                                     CabbageIdentifierIds::type);

        if (controlWidgetTypes.contains(typeOfWidget)) {
            const String name = CabbageWidgetData::getStringProp(cabbageWidgets.getChild(i),
                                                                 CabbageIdentifierIds::name);
            const String channel = CabbageWidgetData::getStringProp(cabbageWidgets.getChild(i),
                                                                    CabbageIdentifierIds::channel);
            const var value = CabbageWidgetData::getProperty(cabbageWidgets.getChild(i), CabbageIdentifierIds::value);

            if (controlWidgetTypes.contains(
                    CabbageWidgetData::getStringProp(cabbageWidgets.getChild(i), CabbageIdentifierIds::type))) {
                if (typeOfWidget == CabbageWidgetTypes::xypad) {
                    const var channel = CabbageWidgetData::getProperty(cabbageWidgets.getChild(i),
                                                                       CabbageIdentifierIds::channel);
                    const float increment = CabbageWidgetData::getNumProp(cabbageWidgets.getChild(i),
                                                                          CabbageIdentifierIds::increment);
                    addParameter(new CabbageAudioParameter(this, cabbageWidgets.getChild(i), *getCsound(), channel[0],
                                                           name + "_x", 0, 1, value, increment, 1));
                    addParameter(new CabbageAudioParameter(this, cabbageWidgets.getChild(i), *getCsound(), channel[1],
                                                           name + "_y", 0, 1, value, increment, 1));
                } else if (typeOfWidget.contains("range")) {
                    const var channel = CabbageWidgetData::getProperty(cabbageWidgets.getChild(i),
                                                                       CabbageIdentifierIds::channel);

                    if (channel.size() > 1) {
                        const float increment = CabbageWidgetData::getNumProp(cabbageWidgets.getChild(i),
                                                                              CabbageIdentifierIds::increment);
                        const int minValue = CabbageWidgetData::getNumProp(cabbageWidgets.getChild(i),
                                                                           CabbageIdentifierIds::minvalue);
                        const int maxValue = CabbageWidgetData::getNumProp(cabbageWidgets.getChild(i),
                                                                           CabbageIdentifierIds::maxvalue);
                        const float skew = CabbageWidgetData::getNumProp(cabbageWidgets.getChild(i),
                                                                         CabbageIdentifierIds::sliderskew);
                        const float min = CabbageWidgetData::getNumProp(cabbageWidgets.getChild(i),
                                                                        CabbageIdentifierIds::min);
                        const float max = CabbageWidgetData::getNumProp(cabbageWidgets.getChild(i),
                                                                        CabbageIdentifierIds::max);
                        addParameter(
                                new CabbageAudioParameter(this, cabbageWidgets.getChild(i), *getCsound(), channel[0],
                                                          name + "_min", min, max, minValue, increment, skew));
                        addParameter(
                                new CabbageAudioParameter(this, cabbageWidgets.getChild(i), *getCsound(), channel[1],
                                                          name + "_max", min, max, maxValue, increment, skew));
                    }
                } else if (typeOfWidget == CabbageWidgetTypes::combobox && channel.isNotEmpty()) {
                    const float min = CabbageWidgetData::getNumProp(cabbageWidgets.getChild(i),
                                                                    CabbageIdentifierIds::min);
                    const float max = CabbageWidgetData::getNumProp(cabbageWidgets.getChild(i),
                                                                    CabbageIdentifierIds::comborange);
                    addParameter(
                            new CabbageAudioParameter(this, cabbageWidgets.getChild(i), *getCsound(), channel, name,
                                                      min, max, value, 1, 1));
                } else if (typeOfWidget.contains("slider") && channel.isNotEmpty()) {
                    const float increment = CabbageWidgetData::getNumProp(cabbageWidgets.getChild(i),
                                                                          CabbageIdentifierIds::increment);
                    const float skew = CabbageWidgetData::getNumProp(cabbageWidgets.getChild(i),
                                                                     CabbageIdentifierIds::sliderskew);
                    const float min = CabbageWidgetData::getNumProp(cabbageWidgets.getChild(i),
                                                                    CabbageIdentifierIds::min);
                    const float max =
                            CabbageWidgetData::getNumProp(cabbageWidgets.getChild(i), CabbageIdentifierIds::max) > min ?
                            CabbageWidgetData::getNumProp(cabbageWidgets.getChild(i), CabbageIdentifierIds::max) :
                            min + 1;

                    addParameter(
                            new CabbageAudioParameter(this, cabbageWidgets.getChild(i), *getCsound(), channel, name,
                                                      min, max, value, increment, skew));
                } else {
                    if (channel.isNotEmpty())
                        addParameter(
                                new CabbageAudioParameter(this, cabbageWidgets.getChild(i), *getCsound(), channel, name,
                                                          0, 1, value, 1, 1));
                }
            }
        }
    }
}

//==============================================================================
bool CabbagePluginProcessor::hasEditor() const {
    return true;
}

AudioProcessorEditor *CabbagePluginProcessor::createEditor() {
    return new CabbagePluginEditor(*this);
}

//==============================================================================
void CabbagePluginProcessor::getStateInformation(MemoryBlock &destData) {
    copyXmlToBinary(savePluginState("CABBAGE_PRESETS"), destData);
}

void CabbagePluginProcessor::setStateInformation(const void *data, int sizeInBytes) {
    std::unique_ptr <XmlElement> xmlElement(getXmlFromBinary(data, sizeInBytes));
    restorePluginState(xmlElement.get());
}

//==============================================================================
XmlElement CabbagePluginProcessor::savePluginState(String xmlTag, File xmlFile, String newPresetName) 
{
    std::unique_ptr<XmlElement> xml;

    if (xmlFile.existsAsFile()) {
        xml = XmlDocument::parse(xmlFile);

        if (!xml)
            xml = std::unique_ptr<XmlElement>(new XmlElement("CABBAGE_PRESETS"));

    } else
        xml = std::unique_ptr<XmlElement>(new XmlElement("CABBAGE_PRESETS"));


    String presetName = "PRESET" + String(xml->getNumChildElements());
    const String childName = newPresetName.isNotEmpty() ? newPresetName : xmlTag + " " + String(xml->getNumChildElements());
    bool presetNameExists = false;
    for( int i = 0 ; i < xml->getNumChildElements() ; i++)
    {
        String preset = "PRESET"+String(i);
        if(auto e = xml->getChildByName(preset))
        {
            if(e->getStringAttribute("PresetName") == childName)
            {
                presetNameExists = true;
                presetName = preset;
            }
        }
            
    }
    
    if(presetNameExists == false)
        xml->createNewChildElement(presetName);
    
    xml->getChildByName(presetName)->setAttribute("PresetName", childName);

    for (int i = 0; i < cabbageWidgets.getNumChildren(); i++) {
        const String channelName = CabbageWidgetData::getStringProp(cabbageWidgets.getChild(i),
                                                                    CabbageIdentifierIds::channel);
        //const String widgetName = CabbageWidgetData::getStringProp (cabbageWidgets.getChild (i), CabbageIdentifierIds::name);

        const String type = CabbageWidgetData::getStringProp(cabbageWidgets.getChild(i), CabbageIdentifierIds::type);
        const var value = CabbageWidgetData::getProperty(cabbageWidgets.getChild(i), CabbageIdentifierIds::value);

        //only write values for widgets that have channels
        if (channelName.isNotEmpty()) {
            if (type == CabbageWidgetTypes::texteditor) {
                const String text = CabbageWidgetData::getStringProp(cabbageWidgets.getChild(i),
                                                                     CabbageIdentifierIds::text);
                xml->getChildByName(presetName)->setAttribute(channelName, text);
            } else if (type == CabbageWidgetTypes::filebutton &&
                       !CabbageWidgetData::getStringProp(cabbageWidgets.getChild(i),
                                                         CabbageIdentifierIds::filetype).contains("snaps")) {
                const String file = CabbageWidgetData::getStringProp(cabbageWidgets.getChild(i),
                                                                     CabbageIdentifierIds::file);

                if (file.length() > 2) {
                    const String relativePath = File(file).getRelativePathFrom(File(csdFile));
                    xml->getChildByName(presetName)->setAttribute(channelName,
                                                                  relativePath.replaceCharacters("\\", "/"));
                }
            } else if (type.contains("range")) //double channel range widgets
            {
                var channels = CabbageWidgetData::getProperty(cabbageWidgets.getChild(i),
                                                              CabbageIdentifierIds::channel);
                const float minValue = CabbageWidgetData::getNumProp(cabbageWidgets.getChild(i),
                                                                     CabbageIdentifierIds::minvalue);
                const float maxValue = CabbageWidgetData::getNumProp(cabbageWidgets.getChild(i),
                                                                     CabbageIdentifierIds::maxvalue);
                xml->getChildByName(presetName)->setAttribute(channels[0].toString(), minValue);
                xml->getChildByName(presetName)->setAttribute(channels[1].toString(), maxValue);
            } else if (type == CabbageWidgetTypes::xypad) //double channel xypad widget
            {
                var channels = CabbageWidgetData::getProperty(cabbageWidgets.getChild(i),
                                                              CabbageIdentifierIds::channel);
                const float xValue = CabbageWidgetData::getNumProp(cabbageWidgets.getChild(i),
                                                                   CabbageIdentifierIds::valuex);
                const float yValue = CabbageWidgetData::getNumProp(cabbageWidgets.getChild(i),
                                                                   CabbageIdentifierIds::valuey);
                xml->getChildByName(presetName)->setAttribute(channels[0].toString(), xValue);
                xml->getChildByName(presetName)->setAttribute(channels[1].toString(), yValue);
            } else {
                xml->getChildByName(presetName)->setAttribute(channelName, float(value));
            }
        }
    }

    return *xml;
}

void CabbagePluginProcessor::restorePluginState(XmlElement *xmlState) {
    if (xmlState != nullptr) {
        //if dealing with session saved by host
        if (xmlState->getNumChildElements() == 1) {
            setParametersFromXml(xmlState->getChildByName("PRESET0"));
        }
            //else dealing with preset files loaded in editor...
        else {
            setParametersFromXml(xmlState);
        }

        initAllCsoundChannels(cabbageWidgets);
    }

    xmlState = nullptr;
}

void CabbagePluginProcessor::setParametersFromXml(XmlElement *e) {
    if (e) {
        for (int i = 1; i < e->getNumAttributes(); i++) {
            ValueTree valueTree = CabbageWidgetData::getValueTreeForComponent(cabbageWidgets, e->getAttributeName(i),
                                                                              true);
            const String type = CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::type);
            const String widgetName = CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::name);

            if (type == CabbageWidgetTypes::texteditor)
                CabbageWidgetData::setStringProp(valueTree, CabbageIdentifierIds::text, e->getAttributeValue(i));
            else if (type == CabbageWidgetTypes::filebutton) {
                const String absolutePath =
                        csdFile.getParentDirectory().getFullPathName() + "/" + e->getAttributeValue(i);
                const String path = File(absolutePath).getFullPathName();
                CabbageWidgetData::setStringProp(valueTree, CabbageIdentifierIds::file, absolutePath);
            } else if (type == CabbageWidgetTypes::hrange ||
                       type == CabbageWidgetTypes::vrange) //double channel range widgets
            {
                CabbageWidgetData::setNumProp(valueTree, CabbageIdentifierIds::minvalue,
                                              e->getAttributeValue(i).getFloatValue());
                CabbageWidgetData::setNumProp(valueTree, CabbageIdentifierIds::maxvalue,
                                              e->getAttributeValue(i + 1).getFloatValue());
                i++;
            } else if (type == CabbageWidgetTypes::xypad) //double channel range widgets
            {
                CabbageWidgetData::setNumProp(valueTree, CabbageIdentifierIds::valuex,
                                              e->getAttributeValue(i).getFloatValue());
                CabbageWidgetData::setNumProp(valueTree, CabbageIdentifierIds::valuey,
                                              e->getAttributeValue(i + 1).getFloatValue());
                i++;
            } else {
                if (CabbageWidgetData::getStringProp(valueTree, "filetype") != "preset"
                    || CabbageWidgetData::getStringProp(valueTree, "filetype") != "*.snaps")
                    CabbageWidgetData::setNumProp(valueTree, CabbageIdentifierIds::value,
                                                  e->getAttributeValue(i).getFloatValue());
                    //now make changes parameter changes so host can see them..
                
                    for (auto param : getParameters())
                    {
                        if (CabbageAudioParameter* cabbageParam = dynamic_cast<CabbageAudioParameter*> (param))
                        {
                            if (widgetName == cabbageParam->getWidgetName())
                            {
                                param->beginChangeGesture();
                                param->setValueNotifyingHost(((CabbageAudioParameter*)param)->range.convertTo0to1 (e->getAttributeValue(i).getFloatValue()));
                                param->endChangeGesture();
                            }
                        }
                    }
            }
        }
    }
}

//==============================================================================
// This method is responsible for updating widget valuetrees based on the current
// data stored in each widget's software channel bus. 
//==============================================================================
void CabbagePluginProcessor::getChannelDataFromCsound() 
{
	for (int i = 0; i < cabbageWidgets.getNumChildren(); i++) 
	{
		const var chanArray = CabbageWidgetData::getProperty(cabbageWidgets.getChild(i), CabbageIdentifierIds::channel);
		const String channelName = (chanArray.size()>0 ? chanArray[0].toString() : chanArray.toString());
		const var widgetArray = CabbageWidgetData::getProperty(cabbageWidgets.getChild(i),
			CabbageIdentifierIds::widgetarray);

		StringArray channels;

		if (widgetArray.size() > 0)
			channels.add(channelName);
		else if (chanArray.size() == 1)
			channels.add(channelName);
		else if (chanArray.size() > 1) {
			for (int j = 0; j < chanArray.size(); j++)
				channels.add(var(chanArray[j]));
		}

		const var value = CabbageWidgetData::getProperty(cabbageWidgets.getChild(i), CabbageIdentifierIds::value);

		const String identChannel = CabbageWidgetData::getStringProp(cabbageWidgets.getChild(i),
			CabbageIdentifierIds::identchannel);

		const String typeOfWidget = CabbageWidgetData::getStringProp(cabbageWidgets.getChild(i),
			CabbageIdentifierIds::type);

		const String chann = channels[0];


		if (channels.size() == 1 && channels[0].isNotEmpty()) {

			if (value.isString() == false) 
			{
				if (getCsound()->GetChannel(channels[0].toUTF8()) != float(value))
				{
					CabbageWidgetData::setNumProp(cabbageWidgets.getChild(i), CabbageIdentifierIds::value,
						getCsound()->GetChannel(channels[0].toUTF8()));
				}

			}
			else 
			{
				char tmp_str[4096] = { 0 };
				getCsound()->GetStringChannel(channels[0].toUTF8(), tmp_str);
				CabbageWidgetData::setProperty(cabbageWidgets.getChild(i), CabbageIdentifierIds::value,
					String(tmp_str));
			}
		}

		//currently only dealing with a max of 2 channels...
		else if (channels.size() == 2 && channels[0].isNotEmpty() && channels[1].isNotEmpty() &&
			typeOfWidget != CabbageWidgetTypes::eventsequencer) 
		{
			const float valuex = CabbageWidgetData::getNumProp(cabbageWidgets.getChild(i), CabbageIdentifierIds::valuex);
			const float valuey = CabbageWidgetData::getNumProp(cabbageWidgets.getChild(i), CabbageIdentifierIds::valuey);
			if (getCsound()->GetChannel(channels[0].toUTF8()) != valuex
				|| getCsound()->GetChannel(channels[1].toUTF8()) != valuey) {
				if (typeOfWidget == CabbageWidgetTypes::xypad) {
					CabbageWidgetData::setNumProp(cabbageWidgets.getChild(i), CabbageIdentifierIds::valuex,
						getCsound()->GetChannel(channels[0].toUTF8()));
					CabbageWidgetData::setNumProp(cabbageWidgets.getChild(i), CabbageIdentifierIds::valuey,
						getCsound()->GetChannel(channels[1].toUTF8()));
				}
				else if (typeOfWidget.contains("range")) {
					const float minValue = CabbageWidgetData::getNumProp(cabbageWidgets.getChild(i),
						CabbageIdentifierIds::minvalue);
					const float maxValue = CabbageWidgetData::getNumProp(cabbageWidgets.getChild(i),
						CabbageIdentifierIds::maxvalue);
					CabbageWidgetData::setNumProp(cabbageWidgets.getChild(i), CabbageIdentifierIds::minvalue,
						getCsound()->GetChannel(channels[0].toUTF8()));
					CabbageWidgetData::setNumProp(cabbageWidgets.getChild(i), CabbageIdentifierIds::maxvalue,
						getCsound()->GetChannel(channels[1].toUTF8()));
				}
			}
		}

		if (identChannel.isNotEmpty()) {
			const String identChannelMessage = CabbageWidgetData::getStringProp(cabbageWidgets.getChild(i),
				CabbageIdentifierIds::identchannelmessage);
			memset(&tmp_string[0], 0, sizeof(tmp_string));
			getCsound()->GetStringChannel(identChannel.toUTF8(), tmp_string);

			const String identifierText(tmp_string);
			//CabbageUtilities::debug(identifierText);
			if (identifierText.isNotEmpty() && identifierText != identChannelMessage) {
				CabbageWidgetData::setCustomWidgetState(cabbageWidgets.getChild(i), " " + identifierText);

				if (identifierText.contains("tablenumber")) //update even if table number has not changed
					CabbageWidgetData::setProperty(cabbageWidgets.getChild(i), CabbageIdentifierIds::update, 1);
				else if (identifierText == CabbageIdentifierIds::tofront.toString() + "()") {
					CabbageWidgetData::setProperty(cabbageWidgets.getChild(i), CabbageIdentifierIds::tofront,
						Random::getSystemRandom().nextInt());
				}

				getCsound()->SetChannel(identChannel.toUTF8(), (char *) "");

				CabbageWidgetData::setProperty(cabbageWidgets.getChild(i), CabbageIdentifierIds::update,
					0); //reset value for further updates

			}
		}
	}
}

void CabbagePluginProcessor::triggerCsoundEvents() {
    for (int x = 0; x < matrixEventSequencers.size(); x++) {
        const ValueTree widgetData = CabbageWidgetData::getValueTreeForComponent(cabbageWidgets,
                                                                                 matrixEventSequencers[x]->channel,
                                                                                 true);
        const String channel = CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::channel);
        const int position = getCsound()->GetChannel(channel.toUTF8());

        if (CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::orientation) == "vertical") {
            for (int i = 0; i < CabbageWidgetData::getNumProp(widgetData, CabbageIdentifierIds::matrixcols); i++) {
                if (matrixEventSequencers[x]->position != position) {
                    String event = matrixEventSequencers[x]->events.getUnchecked(i)->getReference(position);
                    if (event.isNotEmpty()) {
                        getCsound()->InputMessage(event.toUTF8());
                    }
                }
            }
        } else //horizontal
        {
            for (int i = 0; i < CabbageWidgetData::getNumProp(widgetData, CabbageIdentifierIds::matrixrows); i++) {
                if (matrixEventSequencers[x]->position != position) {
                    String event = matrixEventSequencers[x]->events.getUnchecked(position)->getReference(i);
                    if (event.isNotEmpty()) {
                        getCsound()->InputMessage(event.toUTF8());
                    }
                }
            }
        }
        matrixEventSequencers[x]->position = position;
    }
}

//================================================================================
void CabbagePluginProcessor::addXYAutomator(CabbageXYPad *xyPad, ValueTree wData) {
    int indexOfAutomator = -1;

    for (int i = 0; i < xyAutomators.size(); i++) {
        if (xyPad->getName() == xyAutomators[i]->getName())
            indexOfAutomator = i;
    }

    if (indexOfAutomator == -1) {
        XYPadAutomator *xyAuto;
        CabbageAudioParameter *xParameter = getParameterForXYPad(xyPad->getName() + "_x");
        CabbageAudioParameter *yParameter = getParameterForXYPad(xyPad->getName() + "_y");

        if (xParameter && yParameter) {
            xyAutomators.add(xyAuto = new XYPadAutomator(xyPad->getName(), xParameter, yParameter, this));
            xyAuto->setXMin(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::minx));
            xyAuto->setYMin(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::miny));
            xyAuto->setXMax(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::maxx));
            xyAuto->setYMax(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::maxy));
            xyAuto->addChangeListener(xyPad);
        }
    } else {
        xyAutomators[indexOfAutomator]->addChangeListener(xyPad);
    }
}


void CabbagePluginProcessor::enableXYAutomator(String name, bool enable, Line<float> dragLine) {

    for (XYPadAutomator *xyAuto : xyAutomators) {
        if (name == xyAuto->getName()) {
            if (enable == true) {
                xyAuto->setDragLine(dragLine);
                xyAuto->setXValue(dragLine.getEndX());
                xyAuto->setYValue(dragLine.getEndY());
                xyAuto->setXValueIncrement((dragLine.getEndX() - dragLine.getStartX()) * .05);
                xyAuto->setYValueIncrement((dragLine.getEndY() - dragLine.getStartY()) * .05);
                xyAuto->setRepaintBackground(true);
                xyAuto->setIsPluginEditorOpen(getActiveEditor() != nullptr ? true : false);
                xyAuto->startTimer(20);
            } else
                xyAuto->stopTimer();
        }
    }
}

//======================================================================================================
CabbageAudioParameter *CabbagePluginProcessor::getParameterForXYPad(String name) {
    for (auto param : getParameters()) {
        if (CabbageAudioParameter *cabbageParam = dynamic_cast<CabbageAudioParameter *> (param)) {
            if (name == cabbageParam->getWidgetName())
                return dynamic_cast<CabbageAudioParameter *> (cabbageParam);
        }
    }

    return nullptr;
}

//==============================================================================
void CabbagePluginProcessor::setCabbageParameter(String channel, float value) {
    getCsound()->SetChannel(channel.toUTF8().getAddress(), value);
}

void CabbagePluginProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
	
    if (sampleRate != samplingRate) {
		samplingRate = sampleRate;
        CsoundPluginProcessor::prepareToPlay(sampleRate, samplesPerBlock);
        initAllCsoundChannels(cabbageWidgets);
    }
}









