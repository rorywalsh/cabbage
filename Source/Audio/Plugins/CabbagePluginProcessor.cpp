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

#include <memory>
#include "CabbagePluginEditor.h"

#if defined(_CR)
#undef _CR
#endif


char tmp_string[4096] = { 0 };

AudioProcessor* JUCE_CALLTYPE

createPluginFilter() {
	CabbageUtilities::debug("==========================================");
	File csdFile;
#ifdef JUCE_WINDOWS
	CabbageUtilities::debug(CharPointer_UTF8(CabbageManufacturer));
	csdFile = File::getSpecialLocation(File::currentExecutableFile).withFileExtension(String(".csd")).getFullPathName();
	const String homeDrive = File::getSpecialLocation(File::windowsSystemDirectory).getParentDirectory().getParentDirectory().getFullPathName();
	
	if (csdFile.existsAsFile() == false)
	{		
		String filename = homeDrive+"/ProgramData/" + String(CabbageManufacturer) + "/" + File::getSpecialLocation(File::currentExecutableFile).getFileNameWithoutExtension() + "/" + File::getSpecialLocation(File::currentExecutableFile).withFileExtension(String(".csd")).getFileName();
		csdFile = File(filename);
	}
#elif JUCE_MAC
	//read .csd file from the correct location within the .vst bundle.
	const String dir = File::getSpecialLocation(File::currentExecutableFile).getParentDirectory().getParentDirectory().getFullPathName();
    const String pluginBundleName = File::getSpecialLocation(File::currentExecutableFile).getParentDirectory().getParentDirectory().getParentDirectory().getFileNameWithoutExtension();
	const String filename(File::getSpecialLocation(File::currentExecutableFile).withFileExtension(String(".csd")).getFileName());
	csdFile = File(dir + "/" + filename);
    if(csdFile.existsAsFile() == false)
    {
        csdFile = CabbageUtilities::getRealUserHomeDirectory().getFullPathName() + "/Library/" + String(CabbageManufacturer) + "/" + File::getSpecialLocation(File::currentExecutableFile).getFileNameWithoutExtension()+"/"+filename;
        
        if(!csdFile.existsAsFile() && (PluginHostType::getPluginLoadedAs() == AudioProcessor::wrapperType_AudioUnit))
        {
            csdFile = CabbageUtilities::getRealUserHomeDirectory().getFullPathName() + "/Library/" + String(CabbageManufacturer) + "/" + pluginBundleName + "/"+filename;
        }
        
        if(!csdFile.existsAsFile())
            jassertfalse;
    }
#else
	CabbageUtilities::debug(CabbageManufacturer);
	csdFile = File::getSpecialLocation(File::currentExecutableFile).withFileExtension(String(".csd")).getFullPathName();
	if (!csdFile.existsAsFile())
	{
        //In Linux, plugin application directory will reside in ~/Manufacturer/PluginName
		String filename = "~/" + String(CabbageManufacturer) + "/" + File::getSpecialLocation(File::currentExecutableFile).getFileNameWithoutExtension() + "/" + File::getSpecialLocation(File::currentExecutableFile).withFileExtension(String(".csd")).getFileName();

		csdFile = File(filename);
	}
#endif


	if (!csdFile.existsAsFile())
		Logger::writeToLog("Could not find .csd file " + csdFile.getFullPathName() + ", please make sure it's in the correct folder");

	String csdString = csdFile.loadFileAsString();

    return new CabbagePluginProcessor(csdFile, CabbagePluginProcessor::readBusesPropertiesFromXml(csdFile));

}

//============================================================================
CabbagePluginProcessor::CabbagePluginProcessor(const File& inputFile, BusesProperties ioBuses)
	: CsoundPluginProcessor(inputFile, ioBuses),
	cabbageWidgets("CabbageWidgetData"),
	csdFile(inputFile)
{

	CabbageUtilities::debug("Cabbage Processor Constructor - Requested input channels:", getTotalNumInputChannels());
	CabbageUtilities::debug("Cabbage Processor Constructor - Requested output channels:", getTotalNumOutputChannels());
	createCsound(inputFile);
    startTimer(20);

	Logger::writeToLog("CabbagePluginProcessor::CabbagePluginProcessor");

	
}

void CabbagePluginProcessor::createCsound(const File& inputFile, bool shouldCreateParameters)
{
	if (inputFile.existsAsFile()) {
		Logger::writeToLog("CabbagePluginProcessor::createCsound");
		setWidthHeight();
		StringArray linesFromCsd;
		linesFromCsd.addLines(inputFile.loadFileAsString());
        
		//only create extended temp file if imported plants are being added...
		if (addImportFiles(linesFromCsd))
		{
			parseCsdFile(linesFromCsd);


			File tempFile = File::createTempFile(inputFile.getFileNameWithoutExtension() + "_temp.csd");
			tempFile.replaceWithText(linesFromCsd.joinIntoString("\n")
				.replace("$lt;", "<")
				.replace("&amp;", "&")
				.replace("$quote;", "\"")
				.replace("$gt;", ">"));


			//CabbageUtilities::debug(tempFile.loadFileAsString());

			if (!setupAndCompileCsound(tempFile, inputFile.getParentDirectory(), samplingRate))
				this->suspendProcessing(true);

			csdFile = tempFile;

		}

		else {
			parseCsdFile(linesFromCsd);
			csdFile = inputFile;
			if (!setupAndCompileCsound(inputFile, inputFile.getParentDirectory(), samplingRate))
				this->suspendProcessing(true);
		}

        initAllCsoundChannels(cabbageWidgets);
        
		if (shouldCreateParameters)
			createCabbageParameters();

		csoundChanList = nullptr;

		

		csdLastModifiedAt = csdFile.getLastModificationTime().toMilliseconds();
        //getCsound()->SetYieldCallback(CabbagePluginProcessor::csoundYieldCallback);
	}
}

CabbagePluginProcessor::~CabbagePluginProcessor()
{
	for (auto xyAuto : xyAutomators)
		xyAuto->removeAllChangeListeners();

	xyAutomators.clear();
	cabbageWidgets.removeAllChildren(nullptr);

	Logger::writeToLog("CabbagePluginProcessor::~CabbagePluginProcessor");
}


void CabbagePluginProcessor::timerCallback()
{
    if(autoUpdateCount == 0 && autoUpdateIsOn)
    {
        int64 modTime = csdFile.getLastModificationTime().toMilliseconds();

        if (modTime != csdLastModifiedAt && csdFile.existsAsFile())
        {
            csdLastModifiedAt = csdFile.getLastModificationTime().toMilliseconds();
            CabbageUtilities::debug("resetting file due to update of file on disk");
            createCsound(csdFile, false);
        }
    }
    
    if(pollingChannels() == 0)
    {
        if(this->canUpdate.load())
            getIdentifierDataFromCsound();
    }
    
    
    autoUpdateCount = autoUpdateCount < 500 ? autoUpdateCount+1 : 0;
}

//==============================================================================
void CabbagePluginProcessor::setWidthHeight() {
	StringArray csdLines;
	csdLines.addLines(csdFile.loadFileAsString());

	for (const auto& line : csdLines) {
		if (line.contains("</Cabbage>"))
			return;

		ValueTree temp("temp");
		CabbageWidgetData::setWidgetState(temp, line, 0);

		if (CabbageWidgetData::getStringProp(temp, CabbageIdentifierIds::type) == CabbageWidgetTypes::form) {
			screenHeight = CabbageWidgetData::getNumProp(temp, CabbageIdentifierIds::height);
			screenWidth = CabbageWidgetData::getNumProp(temp, CabbageIdentifierIds::width);
		}
	}
}

void CabbagePluginProcessor::parseCsdFile(StringArray& linesFromCsd)
{
	for (const auto& line : linesFromCsd)
	{
		ValueTree temp("temp");
		CabbageWidgetData::setWidgetState(temp, line, 0);

		if (CabbageWidgetData::getStringProp(temp, CabbageIdentifierIds::type) == CabbageWidgetTypes::form)
		{
            if (line.contains("autoUpdate()") || line.contains("autoupdate()"))
            {
                autoUpdateIsOn = true;
            }
            const String font = CabbageWidgetData::getStringProp(temp, CabbageIdentifierIds::typeface);
            if(font.isNotEmpty()){
                const String fontPath = File(getCsdFile()).getParentDirectory().getChildFile(font).getFullPathName();
                if(File(fontPath).existsAsFile())
                {
                    customFont = CabbageUtilities::getFontFromFile(File(fontPath));
                    customFontFile = File(fontPath);
                }
                else
                    customFont = Font(999);
            }
            else
                customFont = Font(999);

		}

	}

	cabbageWidgets.removeAllChildren(nullptr);
	String parentComponent, previousComponent;
	StringArray parents;

    const String csdFileFullPath = csdFile.getFullPathName();
	getMacros(linesFromCsd);

	for (int lineNumber = 0; lineNumber < linesFromCsd.size(); lineNumber++)
    {
        if(linesFromCsd[lineNumber].trimStart().substring(0, 1) == ";")
            continue;
        
		if (linesFromCsd[lineNumber].equalsIgnoreCase("</Cabbage>"))
        {          
			return;
        }
        ValueTree tempWidget(Identifier("WidgetFromLine_" + std::to_string(lineNumber)));

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


		expandMacroText(currentLineOfCabbageCode);
		//note whether lines contains opening and closing bracket so GUI editor can add them back in
		if (currentLineOfCabbageCode.contains("{"))
            CabbageWidgetData::setNumProp(tempWidget, CabbageIdentifierIds::containsOpeningCurlyBracket, 1);

		if (currentLineOfCabbageCode.contains("}"))
            CabbageWidgetData::setNumProp(tempWidget, CabbageIdentifierIds::containsClosingCurlyBracket, 1);

		if (currentLineOfCabbageCode.indexOf(";") > -1 && !currentLineOfCabbageCode.contains("svgElement") && !currentLineOfCabbageCode.contains("populate"))
			currentLineOfCabbageCode = currentLineOfCabbageCode.substring(0, currentLineOfCabbageCode.indexOf(";"));

		const String comments = currentLineOfCabbageCode.indexOf(";") == -1 ? "" : currentLineOfCabbageCode.substring(
			currentLineOfCabbageCode.indexOf(";"));
        
        //force channel type to string if preset combobox
        if(currentLineOfCabbageCode.contains("populate") && currentLineOfCabbageCode.contains("snaps") && currentLineOfCabbageCode.contains("combobox"))
                currentLineOfCabbageCode = currentLineOfCabbageCode.replace("combobox", "combobox channelType(\"string\") automatable(0)");
           
		CabbageWidgetData::setWidgetState(tempWidget, currentLineOfCabbageCode.trimCharactersAtStart(" \t") + comments,
			lineNumber);
        String widgetNameId = CabbageWidgetData::getStringProp(tempWidget, CabbageIdentifierIds::channel);
        //if no name is specified use generic name
        if(widgetNameId.isEmpty())
            widgetNameId = tempWidget.getType().toString();
        

        ValueTree newWidget(widgetNameId);
        newWidget.copyPropertiesFrom(tempWidget, nullptr);
        


		if (shouldClosePlant(linesFromCsd, lineNumber))
			parents.remove(parents.size() - 1);

		if (parents.size() > 0)
			CabbageWidgetData::setStringProp(newWidget, CabbageIdentifierIds::parentcomponent,
				parents[parents.size() - 1]);

		CabbageWidgetData::setNumProp(newWidget, CabbageIdentifierIds::linenumber, lineNumber - linesToSkip);
		CabbageWidgetData::setStringProp(newWidget, CabbageIdentifierIds::csdfile, csdFileFullPath);


		CabbageWidgetData::setProperty(newWidget, CabbageIdentifierIds::macronames, macroNames);
		CabbageWidgetData::setProperty(newWidget, CabbageIdentifierIds::macrostrings, macroStrings);


		const String typeOfWidget = CabbageWidgetData::getStringProp(newWidget, CabbageIdentifierIds::type);
		for (auto && plantStruct : plantStructs) {
			if (plantStruct.name == typeOfWidget)
				linesToSkip += plantStruct.cabbageCode.size() + 1;
		}

		if (typeOfWidget == CabbageWidgetTypes::form) {
			const String caption = CabbageWidgetData::getStringProp(newWidget, CabbageIdentifierIds::caption);
			setPluginName(caption.length() > 0 ? caption : "Untitled");
            
            const String poll = CabbageWidgetData::getStringProp(newWidget, CabbageIdentifierIds::guimode);
            if(poll == "polling")
                pollingChannels(1);
            else if(poll == "queue")
                pollingChannels(0);
            else
                pollingChannels(2);
            
			if (CabbageWidgetData::getNumProp(newWidget, CabbageIdentifierIds::logger) == 1)
				createFileLogger(this->csdFile);

			setGUIRefreshRate(CabbageWidgetData::getNumProp(newWidget, CabbageIdentifierIds::guirefresh));
            
            setNumPreCycles(CabbageWidgetData::getNumProp(newWidget, CabbageIdentifierIds::precycles));
		}
        
		const String precedingCharacters = currentLineOfCabbageCode.substring(0, currentLineOfCabbageCode.indexOf(
			typeOfWidget));
		CabbageWidgetData::setStringProp(newWidget, "precedingCharacters", precedingCharacters);

        const String mode = CabbageWidgetData::getStringProp(newWidget, CabbageIdentifierIds::mode);

        if(mode == "resize" && typeOfWidget == CabbageWidgetTypes::combobox)
        {
            CabbageWidgetData::setStringProp(newWidget, CabbageIdentifierIds::channel, "PluginResizerCombBox");
            CabbageWidgetData::setNumProp(newWidget, CabbageIdentifierIds::automatable, 0);
        }

		const String widgetName = CabbageWidgetData::getStringProp(newWidget, CabbageIdentifierIds::name);
        
		if (widgetName.isNotEmpty())
			cabbageWidgets.addChild(newWidget, -1, nullptr);

        if (isWidgetPlantParent(linesFromCsd, lineNumber) &&
            currentLineOfCabbageCode.removeCharacters(" ").removeCharacters("\t").substring(0, 1) != "{") {
            //CabbageUtilities::debug (CabbageWidgetData::getProperty (newWidget, CabbageIdentifierIds::name).toString());
            parents.add(CabbageWidgetData::getProperty(newWidget, CabbageIdentifierIds::name).toString());
            auto currentChild = cabbageWidgets.getChild(cabbageWidgets.getNumChildren()-1);
            CabbageWidgetData::setNumProp(currentChild, CabbageIdentifierIds::isparent, 1);
        }
        
		if (CabbageWidgetData::getProperty(newWidget, CabbageIdentifierIds::widgetarray).size() > 0 &&
			CabbageWidgetData::getProperty(newWidget, CabbageIdentifierIds::identchannelarray).size() > 0) {
			for (int i = 0;
				i < CabbageWidgetData::getProperty(newWidget, CabbageIdentifierIds::widgetarray).size(); i++) {
				
				const String chan = CabbageWidgetData::getProperty(newWidget,
					CabbageIdentifierIds::widgetarray)[i].toString();
				const String iChan = CabbageWidgetData::getProperty(newWidget,
					CabbageIdentifierIds::identchannelarray)[i].toString();
				const String name =
					CabbageWidgetData::getStringProp(newWidget, CabbageIdentifierIds::name) + String(9999 + i);
                
                //using identifier channels as names for widget arrays..
                ValueTree copy(iChan);
                copy.copyPropertiesFrom(newWidget, nullptr);
				CabbageWidgetData::setStringProp(copy, CabbageIdentifierIds::name, name);
				CabbageWidgetData::setStringProp(copy, CabbageIdentifierIds::channel,
					CabbageWidgetData::getProperty(newWidget,
						CabbageIdentifierIds::widgetarray)[i]);
				CabbageWidgetData::setStringProp(copy, CabbageIdentifierIds::identchannel,
					CabbageWidgetData::getProperty(newWidget,
						CabbageIdentifierIds::identchannelarray)[i]);
				cabbageWidgets.addChild(copy, -1, nullptr);
			}
		}
	}
    

    
}

bool CabbagePluginProcessor::isWidgetPlantParent(StringArray& linesFromCsd, int lineNumber) {
	if (linesFromCsd[lineNumber].contains("{"))
		return true;

	if (linesFromCsd[lineNumber + 1].removeCharacters(" ").removeCharacters("\t").substring(0, 1) == "{")
		return true;

	return false;
}

bool CabbagePluginProcessor::shouldClosePlant(StringArray& linesFromCsd, int lineNumber) {
	if (linesFromCsd[lineNumber].contains("}"))
		return true;

	return false;
}

bool CabbagePluginProcessor::addImportFiles(StringArray& linesFromCsd) {

	getMacros(linesFromCsd);
	bool hasImportFiles = false;
	for (int i = 0; i < linesFromCsd.size(); i++) {
		ValueTree temp("temp");
		String newCsdLine = linesFromCsd[i];
		expandMacroText(newCsdLine);
		CabbageWidgetData::setWidgetState(temp, newCsdLine, 0);

		if (CabbageWidgetData::getStringProp(temp, CabbageIdentifierIds::type) == CabbageWidgetTypes::form) {
			var files = CabbageWidgetData::getProperty(temp, CabbageIdentifierIds::importfiles);

			if (files.size() > 0)
				hasImportFiles = true;

			for (int y = 0; y < files.size(); y++) {
				//                CabbageUtilities::debug(
				//                        csdFile.getParentDirectory().getChildFile(files[y].toString()).getFullPathName());

				if (csdFile.getParentDirectory().getChildFile(files[y].toString()).existsAsFile()) {
					StringArray linesFromImportedFile;
					linesFromImportedFile.addLines(
						csdFile.getParentDirectory().getChildFile(files[y].toString()).loadFileAsString());

					std::unique_ptr<XmlElement> xml(XmlDocument::parse(CabbageUtilities::getPlantFileAsXmlString(
						csdFile.getParentDirectory().getChildFile(files[y].toString()))));

					if (!xml) //if plain text...
					{
						for (int p = linesFromImportedFile.size(); p >= 0; p--) {
							linesFromCsd.insert(i + 1, linesFromImportedFile[p]);
						}
					}
					else//if plant xml
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

void CabbagePluginProcessor::handleXmlImport(XmlElement* xml, StringArray& linesFromCsd) {
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
				importData.csoundCode = e->getAllSubText().replace("$quote;", "\"");

			if (e->getTagName() == "cabbagecodescript")
				generateCabbageCodeFromJS(importData, e->getAllSubText());
		}

		//CabbageUtilities::debug(importData.cabbageCode.joinIntoString("\n"));
		//numberOfLinesInPlantCode += importData.cabbageCode.size()+1;
		insertUDOCode(importData, linesFromCsd);
		plantStructs.add(importData);
	}
}

void CabbagePluginProcessor::insertPlantCode(StringArray& linesFromCsd) {
	getMacros(linesFromCsd);

	const StringArray copy = linesFromCsd;

	for (int lineIndex = 0; lineIndex < linesFromCsd.size(); lineIndex++) {
		String currentLineOfCode = linesFromCsd[lineIndex];
		if (currentLineOfCode.trim().startsWith("</Cabbage>"))
			return;
		if (currentLineOfCode.isNotEmpty() && currentLineOfCode.substring(0, 1) != ";") {

			float scaleX = 1;
			float scaleY = 1;
			StringArray importedLines("");
			ValueTree temp("temp");
			expandMacroText(currentLineOfCode);
			// CabbageUtilities::debug(currentLineOfCode);
			CabbageWidgetData::setWidgetState(temp, currentLineOfCode.trim(), lineIndex);
			const String type = CabbageWidgetData::getStringProp(temp, CabbageIdentifierIds::type);
			const String nsp = CabbageWidgetData::getStringProp(temp, CabbageIdentifierIds::nsp);

			bool isPlantWidget = true;
			for (auto && plantStruct : plantStructs) {
				//                CabbageUtilities::debug(type + " " + plantStructs[plantIndex].name.trim());
				//                CabbageUtilities::debug(nsp + " " + plantStructs[plantIndex].nsp.trim());
				if (type == plantStruct.name.trim() && plantStruct.nsp.trim() == nsp) {
					int lineNumberPlantAppearsOn;

					for (auto plantCode : plantStruct.cabbageCode) {
						if (plantCode.isNotEmpty()) {
							if (!plantCode.contains("}")) {
								ValueTree temp1("temp1");

								expandMacroText(plantCode);
								CabbageWidgetData::setWidgetState(temp1, plantCode.trim(), -99);
								CabbageWidgetData::setNumProp(temp1, CabbageIdentifierIds::plant,
									plantStruct.cabbageCode.size() + 2);
								CabbageWidgetData::setNumProp(temp, CabbageIdentifierIds::plant,
									plantStruct.cabbageCode.size() + 2);
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
								}
								else {
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
								var currentChannel = CabbageWidgetData::getProperty(temp1, CabbageIdentifierIds::channel);
								const String channelPrefix = CabbageWidgetData::getStringProp(temp,
									CabbageIdentifierIds::channel);
								const String currentIdentChannel = CabbageWidgetData::getStringProp(temp1,
									CabbageIdentifierIds::identchannel);


								var chanArray;
								for (int i = 0; i < currentChannel.size(); i++)
								{
									chanArray.append(channelPrefix + currentChannel[i].toString());
								}

								CabbageWidgetData::setProperty(temp1, CabbageIdentifierIds::channel, chanArray);


								if (CabbageWidgetData::getStringProp(temp1,
									CabbageIdentifierIds::identchannel).isNotEmpty())
									CabbageWidgetData::setStringProp(temp1, CabbageIdentifierIds::identchannel,
										channelPrefix + currentIdentChannel);

								CabbageWidgetData::setProperty(temp1, CabbageIdentifierIds::macronames, macroNames);
								CabbageWidgetData::setProperty(temp1, CabbageIdentifierIds::macrostrings, macroStrings);

								//by the time it gets here it's not picked up the right channels....

								var items = CabbageWidgetData::getProperty(temp1, "channel");


								String replacementText = (plantCode.indexOf("{") != -1 ?
									CabbageWidgetData::getCabbageCodeFromIdentifiers(temp1,
										plantCode) +
									"{"
									: CabbageWidgetData::getCabbageCodeFromIdentifiers(
										temp1, plantCode));


								importedLines.add(replacementText);
								isPlantWidget = false;
							}
							else
								importedLines.add("}");

						}
					}

					for (int y = 0; y < importedLines.size(); y++) {
						linesFromCsd.insert(lineIndex + y, importedLines[y]);
					}
					lineIndex += importedLines.size();
					importedLines.clear();

				}
			}


		}

	}
}


void CabbagePluginProcessor::insertUDOCode(const PlantImportStruct& importData, StringArray& linesFromCsd) {
	//todo don't check blocks of commented code
	for (auto str : linesFromCsd) {
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

void CabbagePluginProcessor::generateCabbageCodeFromJS(PlantImportStruct& importData, const String& text) {
	JavascriptEngine engine;
	engine.maximumExecutionTime = RelativeTime::seconds(5);
	engine.registerNativeObject("Cabbage", new CabbageJavaClass(this));


	Result res = engine.execute(text.replace("$lt;", "<")
		.replace("&amp;", "&")
		.replace("$quote;", "\"")
		.replace("$gt;", ">"));


	importData.cabbageCode.addLines(cabbageScriptGeneratedCode.joinIntoString("\n"));

	if (res.failed())
		CabbageUtilities::showMessage("javaScript Error:" + res.getErrorMessage(),
			&getActiveEditor()->getLookAndFeel());

}


void CabbagePluginProcessor::getMacros(const StringArray& linesFromCsd) {
	var tempMacroNames, tempMacroStrings;

	for (String csdLine : linesFromCsd) //deal with Cabbage macros
	{
		StringArray tokens;
		csdLine = csdLine.replace("\n", " ");

		tokens.addTokens(csdLine, ", ");
        tokens.removeEmptyStrings();
        
		bool commented = false;

		if (csdLine.indexOf(";") > -1)
			commented = true;

		if (tokens[0].containsIgnoreCase("define")) {
			//tokens.removeEmptyStrings();

			if (tokens.size() > 1) {
				const String currentMacroText = commented ? " " : csdLine.substring(csdLine.indexOf(tokens[1]) + tokens[1].length()) + " ";
				macroText.set("$" + tokens[1], " " + currentMacroText);              
				tempMacroNames.append("$" + tokens[1]);
				tempMacroStrings.append(" " + currentMacroText.trim());
				macroNames = tempMacroNames;
				macroStrings = tempMacroStrings;
			}
		}
	}

	macroText.set("$SCREEN_WIDTH", String(screenWidth));
	macroText.set("$SCREEN_HEIGHT", String(screenHeight));
	macroNames.append("$SCREEN_WIDTH");
	macroNames.append("$SCREEN_HEIGHT");
	macroStrings.append(String(screenWidth));
	macroStrings.append(String(screenHeight));


}

void CabbagePluginProcessor::expandMacroText(String& line) {
	String csdLine;

	String defineText;
	String expandedLine = line;


	StringArray tokens;
	tokens.addTokens(line.replace("(", "( "), " ,");

	StringArray commentedMacros;
	for (const auto& token : tokens) {
		if (token.startsWith("$")) {
			commentedMacros.add(token);
			for (auto macro : macroText) {
				const String stringToReplace = token.removeCharacters(",() ");
				if (macro.name.toString() == stringToReplace) {
					commentedMacros.removeString(macro.name);
					line = line.replace(stringToReplace, macro.value.toString());
					//CabbageUtilities::debug("MacroValue:"+ macro.value.toString());
				}
			}
		}
	}

	//remove any macros that are not valid... 
	for (const auto& macro : commentedMacros)
		line = line.replace(macro, "");


}

//rebuild the entire GUi each time something changes.
void CabbagePluginProcessor::recreateWidgets(const String& csdText, bool editMode)
{
	if(auto* editor = dynamic_cast<CabbagePluginEditor*> (this->getActiveEditor()))
    {
        StringArray strings;
        strings.addLines(csdText);
        parseCsdFile(strings);
        
        editor->createEditorInterface(cabbageWidgets);
        if(editMode)
            editor->setEditMode(editMode);
        
        editor->updateLayoutEditorFrames();
    }
}

void CabbagePluginProcessor::addCabbageParameter(std::unique_ptr<CabbagePluginParameter> parameter)
{
	if (parameter->getIsAutomatable()) {
		addParameter(parameter->releaseHostParameter());
	}

	parameters.add(parameter.release());
}


void CabbagePluginProcessor::releaseResources()
{
	// When playback stops, you can use this as an opportunity to free up any
	// spare memory, etc.
}

//==============================================================================
// create parameters for sliders, buttons, comboboxes, checkboxes, encoders and xypads.
// Other widgets can communicate with Csound, but they cannot be automated
void CabbagePluginProcessor::createCabbageParameters()
{
	CabbageControlWidgetStrings controlWidgetTypes;

	for (int i = 0; i < cabbageWidgets.getNumChildren(); i++) {
		const bool automatable = CabbageWidgetData::getNumProp(cabbageWidgets.getChild(i),
			CabbageIdentifierIds::automatable);

		String prefix = CabbageWidgetData::getStringProp(cabbageWidgets.getChild(i),
			CabbageIdentifierIds::valueprefix);

		String postfix = CabbageWidgetData::getStringProp(cabbageWidgets.getChild(i),
			CabbageIdentifierIds::valuepostfix);

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
				if (typeOfWidget == CabbageWidgetTypes::xypad)
				{
					const var currentChannel = CabbageWidgetData::getProperty(cabbageWidgets.getChild(i),
						CabbageIdentifierIds::channel);
                    
					const float increment = CabbageWidgetData::getNumProp(cabbageWidgets.getChild(i),
						CabbageIdentifierIds::increment);
					const float minX = CabbageWidgetData::getProperty(cabbageWidgets.getChild(i),
						CabbageIdentifierIds::minx);
					const float minY = CabbageWidgetData::getProperty(cabbageWidgets.getChild(i),
						CabbageIdentifierIds::miny);
					const float maxX = CabbageWidgetData::getProperty(cabbageWidgets.getChild(i),
						CabbageIdentifierIds::maxx);
					const float maxY = CabbageWidgetData::getProperty(cabbageWidgets.getChild(i),
						CabbageIdentifierIds::maxy);

					String xPrefix = "";
					String xPostfix = "";
					String yPrefix = "";
					String yPostfix = "";

					const auto prefixes = CabbageWidgetData::getProperty(cabbageWidgets.getChild(i),
						CabbageIdentifierIds::valueprefix);
					if (prefixes.size() > 0)
					{
						xPrefix = prefixes[0];
						if (prefixes.size() > 1)
						{
							yPrefix = prefixes[1];
						}
					}

					const auto postfixes = CabbageWidgetData::getProperty(cabbageWidgets.getChild(i),
						CabbageIdentifierIds::valuepostfix);
					if (postfixes.size() > 0)
					{
						xPostfix = postfixes[0];
						if (postfixes.size() > 1)
						{
							yPostfix = postfixes[1];
						}
					}


					auto xParam = std::make_unique<CabbagePluginParameter>(this, cabbageWidgets.getChild(i), currentChannel[0],
						name + "_x", minX, maxX, value, increment, 1, automatable,
						xPrefix, xPostfix);
					auto yParam = std::make_unique<CabbagePluginParameter>(this, cabbageWidgets.getChild(i), currentChannel[1],
						name + "_y", minY, maxY, value, increment, 1, automatable,
						yPrefix, yPostfix);

					addCabbageParameter(std::move(xParam));
					addCabbageParameter(std::move(yParam));
				}
				else if (typeOfWidget.contains("range"))
				{
					const var currentChannel = CabbageWidgetData::getProperty(cabbageWidgets.getChild(i),
						CabbageIdentifierIds::channel);

					if (currentChannel.size() > 1) {
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

						auto minParam = std::make_unique<CabbagePluginParameter>(this, cabbageWidgets.getChild(i), currentChannel[0],
							name + "_min", min, max, minValue, increment, skew, automatable, prefix, postfix);
						auto maxParam = std::make_unique<CabbagePluginParameter>(this, cabbageWidgets.getChild(i), currentChannel[1],
							name + "_max", min, max, maxValue, increment, skew, automatable, prefix, postfix);

						addCabbageParameter(std::move(minParam));
						addCabbageParameter(std::move(maxParam));
					}
				}
				else if ((typeOfWidget == CabbageWidgetTypes::combobox || typeOfWidget == CabbageWidgetTypes::optionbutton ) && channel.isNotEmpty())
				{
					//string combobox need to have their range known when creating the plugin parameter... 
					if (CabbageWidgetData::getStringProp(cabbageWidgets.getChild(i), CabbageIdentifierIds::channeltype) == "string")
					{
						const String workingDir = CabbageWidgetData::getStringProp(cabbageWidgets.getChild(i), CabbageIdentifierIds::currentdir);
						const String fileType = CabbageWidgetData::getStringProp(cabbageWidgets.getChild(i), "filetype");
						int numOfFiles;
						Array<File> folderFiles;
						StringArray comboItems;
						CabbageUtilities::searchDirectoryForFiles(workingDir, fileType, folderFiles, comboItems, numOfFiles);
						const String currentValue = CabbageWidgetData::getStringProp(cabbageWidgets.getChild(i), CabbageIdentifierIds::value);
						const float min = CabbageWidgetData::getNumProp(cabbageWidgets.getChild(i),
							CabbageIdentifierIds::min);
						const float max = numOfFiles == 0 ? 1 : numOfFiles;

						const String channelType = CabbageWidgetData::getStringProp(cabbageWidgets.getChild(i),
							CabbageIdentifierIds::channeltype);

						var items = CabbageWidgetData::getProperty(cabbageWidgets.getChild(i),
							CabbageIdentifierIds::text);


						for (int i = 0 ; i < items.size() ; i++)
						{
							comboItems.add(items[i].toString());
						}
						
						auto param = std::make_unique<CabbagePluginParameter>(this, cabbageWidgets.getChild(i), channel, name,
							min, max, value, 1, 1, automatable, "", "", true, comboItems);
						addCabbageParameter(std::move(param));
					}
					else //comboboxes and options button are both set up with the same type of host parameter mapping...
					{
						const float min = CabbageWidgetData::getNumProp(cabbageWidgets.getChild(i),
							CabbageIdentifierIds::min);
						const float max = CabbageWidgetData::getNumProp(cabbageWidgets.getChild(i),
							CabbageIdentifierIds::comborange);

						auto param = std::make_unique<CabbagePluginParameter>(this, cabbageWidgets.getChild(i), channel, name,
							min, max, value, 1, 1, automatable, "", "", true);
						addCabbageParameter(std::move(param));
					}
				}
				else if (typeOfWidget.contains("slider") && channel.isNotEmpty())
				{
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

					auto param = std::make_unique<CabbagePluginParameter>(this, cabbageWidgets.getChild(i), channel, name,
						min, max, value, increment, skew, automatable, prefix, postfix);
					addCabbageParameter(std::move(param));
				}
				else if (typeOfWidget.contains("encoder") && channel.isNotEmpty())
				{
					const float increment = CabbageWidgetData::getNumProp(cabbageWidgets.getChild(i),
						CabbageIdentifierIds::increment);
					const float skew = 1;
					const float min = CabbageWidgetData::getNumProp(cabbageWidgets.getChild(i),
						CabbageIdentifierIds::min);
					auto m = CabbageWidgetData::getNumProp(cabbageWidgets.getChild(i),
						CabbageIdentifierIds::max);
                    const float max = (m <= min ? m+1 : m);

					auto param = std::make_unique<CabbagePluginParameter>(this, cabbageWidgets.getChild(i), channel, name,
						min, max, value, increment, skew, automatable, prefix, postfix);
					addCabbageParameter(std::move(param));
				}
				else
				{
					if (typeOfWidget.contains("button") || typeOfWidget.contains("checkbox"))
					{
						prefix = "";
						postfix = "";
					}

					if (channel.isNotEmpty())
					{
						auto param = std::make_unique<CabbagePluginParameter>(this, cabbageWidgets.getChild(i), channel, name,
							0, 1, value, 1, 1, automatable, prefix, postfix);
						addCabbageParameter(std::move(param));
					}
				}
			}
		}
	}
}

//==============================================================================
bool CabbagePluginProcessor::hasEditor() const {
	return true;
}

AudioProcessorEditor* CabbagePluginProcessor::createEditor() {
	return new CabbagePluginEditor(*this);
}

//==============================================================================
void CabbagePluginProcessor::getStateInformation(MemoryBlock& destData) 
{
    try{
	auto j = nlohmann::json::parse(addPluginPreset("CABBAGE_PRESETS", "", false).toStdString());

	nlohmann::json k, l;
	l["dummy"] = "dummy";
	k["daw state"] = j;
	k["dummy"] = l;
	hostStateData = k;
	MemoryOutputStream(destData, true).writeString(k.dump(4));
    }
    catch (nlohmann::json::exception& e) {
        DBG(e.what());
        jassertfalse;
    }
//  the older xml way of doing it..
//	copyXmlToBinary(savePluginState("CABBAGE_PRESETS"), destData);
//	Logger::writeToLog ("CabbagePluginProcessor::getStateInformation");
}

void CabbagePluginProcessor::setStateInformation(const void* data, int sizeInBytes) 
{
    try{
	
		auto jsonData = nlohmann::json::parse(MemoryInputStream(data, static_cast<size_t> (sizeInBytes), false).readString().toStdString());
        setPluginState(jsonData, "", true); 
		//hostStateData is updated regularly so if a user changes any session state
		//their changes won't be lost when Csound recompiles
		hostStateData = jsonData;
    }
    catch (nlohmann::json::exception& e) {
        DBG(e.what());
        jassertfalse;
    }

//  the older xml way of doing it..
//	std::unique_ptr <XmlElement> xmlElement(getXmlFromBinary(data, sizeInBytes));
//	restorePluginState(xmlElement.get());
//	Logger::writeToLog("CabbagePluginProcessor::setStateInformation");
}

//==============================================================================
String CabbagePluginProcessor::addPluginPreset(String presetName,  const String& fileName, bool remove)
{    
    nlohmann::ordered_json j;
	String presetFileContents = "";
	File presetFile;
	currentPresetName = presetName;
	if (fileName.isNotEmpty())
	{
		presetFile = File(fileName);
		presetFileContents = presetFile.loadFileAsString();
	}

    if(presetFileContents.isEmpty())
    {
        if(presetName.isEmpty())
            presetName = "Preset 1";
    }
    else
    {
        j = nlohmann::ordered_json::parse(presetFileContents.toRawUTF8());
        
        if(remove == false)
        {
            int numberOfPresets = 1;
            for (nlohmann::ordered_json::iterator it = j.begin(); it != j.end(); ++it) {
                numberOfPresets++;
            }
            if(presetName.isEmpty())
                presetName = "Preset " +String(numberOfPresets);

			currentPresetName = presetName;
        }
        else
        {
            StringArray presets;

            for (nlohmann::ordered_json::iterator it = j.begin(); it != j.end(); ++it) {
                if(String(it.key()) == presetName)
                {
                    j.erase(it);
                    break;
                }
            }
            
            for (nlohmann::ordered_json::iterator it = j.begin(); it != j.end(); ++it) {
                presets.add (it.key());
            }


            currentPresetName = presets[presets.size()-1];
            
            presetFile.replaceWithText(String(j.dump(4)));
            
            
			return j[currentPresetName.toStdString()].dump();


        }
    }
    
    
    
    for (int i = 0; i < cabbageWidgets.getNumChildren(); i++) {
        const String channelName = CabbageWidgetData::getStringProp(cabbageWidgets.getChild(i),
                                                                    CabbageIdentifierIds::channel);
        const int ignore = CabbageWidgetData::getNumProp(cabbageWidgets.getChild(i),
                                                                    CabbageIdentifierIds::presetignore);
        const String type = CabbageWidgetData::getStringProp(cabbageWidgets.getChild(i), CabbageIdentifierIds::type);
        
        if(channelName == "PluginResizerCombBox" && ignore==0)
        {
            const var value = CabbageWidgetData::getProperty(cabbageWidgets.getChild(i), CabbageIdentifierIds::value);
            j[currentPresetName.toStdString()][channelName.toStdString()] = float(value);
        }
        else if ((type == CabbageWidgetTypes::combobox ||  type == CabbageWidgetTypes::listbox) && CabbageWidgetData::getStringProp(cabbageWidgets.getChild(i),                                                                                                CabbageIdentifierIds::filetype).contains("snaps"))
        {
            if(presetName == "CABBAGE_PRESETS")
            {
                const String presetN = CabbageWidgetData::getStringProp(cabbageWidgets.getChild(i),CabbageIdentifierIds::value);
                const String presetText = CabbageWidgetData::getStringProp(cabbageWidgets.getChild(i),CabbageIdentifierIds::text);
                j[currentPresetName.toStdString()][channelName.toStdString()] = presetN.toStdString();
            }
        }
       else if (type == CabbageWidgetTypes::presetbutton)
       {
            const String presetN = CabbageWidgetData::getStringProp(cabbageWidgets.getChild(i),CabbageIdentifierIds::value);
            j[currentPresetName.toStdString()][channelName.toStdString()] = presetN.toStdString();
       }
       else if(ignore == 0)
        {
            const var value = CabbageWidgetData::getProperty(cabbageWidgets.getChild(i), CabbageIdentifierIds::value);
            
            //only write values for widgets that have channels
            if (channelName.isNotEmpty()) {
                if (type == CabbageWidgetTypes::texteditor)
				{
                    String text = CabbageWidgetData::getStringProp(cabbageWidgets.getChild(i),
                                                                         CabbageIdentifierIds::text);
                    j[currentPresetName.toStdString()][channelName.toStdString()] = text.toRawUTF8();
                }
				if (type == CabbageWidgetTypes::soundfiler) 
				{
					const String file = CabbageWidgetData::getStringProp(cabbageWidgets.getChild(i),
						CabbageIdentifierIds::file);
					const int scrubberPos = CabbageWidgetData::getNumProp(cabbageWidgets.getChild(i),
						CabbageIdentifierIds::scrubberposition);
					const int regionStart = CabbageWidgetData::getNumProp(cabbageWidgets.getChild(i),
						CabbageIdentifierIds::regionstart);
					const int regionLength = CabbageWidgetData::getNumProp(cabbageWidgets.getChild(i),
						CabbageIdentifierIds::regionlength);

					//j[presetName.toStdString()][String(channelName).toStdString()] = file.toRawUTF8();
					nlohmann::ordered_json b;
                    b[CabbageIdentifierIds::file.toString().toStdString()] = file.replace ("\\", "/").toRawUTF8();
					b[CabbageIdentifierIds::scrubberposition.toString().toStdString()] = scrubberPos;
					b[CabbageIdentifierIds::regionstart.toString().toStdString()] = regionStart;
					b[CabbageIdentifierIds::regionlength.toString().toStdString()] = regionLength;
					j[currentPresetName.toStdString()][String(channelName).toStdString()] = b;

				}
                else if (type == CabbageWidgetTypes::filebutton &&
                         !CabbageWidgetData::getStringProp(cabbageWidgets.getChild(i),
                                                           CabbageIdentifierIds::filetype).contains("snaps"))
                {
                    const int ignorelastdir = CabbageWidgetData::getNumProp(cabbageWidgets.getChild(i),
                                                                              CabbageIdentifierIds::ignorelastdir);
                     const String file = CabbageWidgetData::getStringProp(cabbageWidgets.getChild(i),
                                                                          CabbageIdentifierIds::file);
                     if(ignorelastdir == 0)
                     {
                         if (file.length() > 2) {
                             const String relativePath = File(csdFile).getParentDirectory().getChildFile(file).getFullPathName();
                             j[currentPresetName.toStdString()][channelName.toStdString()] = relativePath.replaceCharacters("\\", "/").toStdString();
                         }
                     }
                }
                else if (type.contains("range")) //double channel range widgets
                {
                    var channels = CabbageWidgetData::getProperty(cabbageWidgets.getChild(i),
                                                                  CabbageIdentifierIds::channel);
                    
                    const float minValue = CabbageWidgetData::getNumProp(cabbageWidgets.getChild(i),
                                                                         CabbageIdentifierIds::minvalue);
                    const float maxValue = CabbageWidgetData::getNumProp(cabbageWidgets.getChild(i),
                                                                         CabbageIdentifierIds::maxvalue);
					nlohmann::ordered_json b;
					b["Range Min"] = minValue;
					b["Range Max"] = maxValue;
					j[currentPresetName.toStdString()][channels[0].toString().toStdString()] = b;

                    //j[currentPresetName.toStdString()][channels[0].toString().toStdString()] = minValue;
                    //j[currentPresetName.toStdString()][channels[1].toString().toStdString()] = maxValue;
                }
                else if (type == CabbageWidgetTypes::xypad) //double channel xypad widget
                {
                    var channels = CabbageWidgetData::getProperty(cabbageWidgets.getChild(i),
                                                                  CabbageIdentifierIds::channel);
                    const float xValue = CabbageWidgetData::getNumProp(cabbageWidgets.getChild(i),
                                                                       CabbageIdentifierIds::valuex);
                    const float yValue = CabbageWidgetData::getNumProp(cabbageWidgets.getChild(i),
                                                                       CabbageIdentifierIds::valuey);
                    
                    //j[currentPresetName.toStdString()][channels[0].toString().toStdString()] = xValue;
                    //j[currentPresetName.toStdString()][channels[1].toString().toStdString()] = yValue;
					nlohmann::ordered_json b;
					b["X"] = xValue;
					b["Y"] = yValue;
					j[currentPresetName.toStdString()][channels[0].toString().toStdString()] = b;
                }
                else if (type == CabbageWidgetTypes::combobox && CabbageWidgetData::getStringProp(cabbageWidgets.getChild(i),
                                                                                                CabbageIdentifierIds::filetype).contains("snaps"))
                {
                        //don't want to grab values for combobox that are used for presets....
                }
                else if ((type == CabbageWidgetTypes::combobox || type == CabbageWidgetTypes::listbox) && CabbageWidgetData::getProperty(cabbageWidgets.getChild(i),
                                                                                                CabbageIdentifierIds::channeltype) == "string")
                {
                    char tmp_str[4096] = { 0 };
                    if(getCsound())
                        getCsound()->GetStringChannel(channelName.getCharPointer(), tmp_str);
                    const String file(tmp_str);
                    j[currentPresetName.toStdString()][channelName.toStdString()] = file.toStdString();
                    
                }
                else
                {
                    j[currentPresetName.toStdString()][channelName.toStdString()] = float(value);
                }
            }
        }
    }
    
	if (getCsound())
	{
		auto** p = (CabbagePersistentData**)getCsound()->QueryGlobalVariable("cabbageData");

		if (p != nullptr)
		{
			auto pdClass = *p;
			j[currentPresetName.toStdString()]["cabbageJSONData"] = pdClass->data;
		}
	}

	if(fileName.isNotEmpty())
		presetFile.replaceWithText(String(j.dump(4)));


    
	return  j[currentPresetName.toStdString()].dump();

}


void CabbagePluginProcessor::setPluginState(nlohmann::ordered_json j, const String presetName, bool hostState)
{
    try{
#if Bluetooth
        auto** p = (CabbagePresetData**)getCsound()->QueryGlobalVariable("cabbageGlobalPreset");

        if (p != nullptr)
        {
            auto preset = *p;
            preset->data = j.dump(4);
            DBG(preset->data);
        }
#endif
        
		DBG(j.dump(4));
	for (nlohmann::ordered_json::iterator itA = j.begin(); itA != j.end(); ++itA)
	{
		if (String(itA.key()) == presetName || hostState == true)
		{
			for (nlohmann::ordered_json::iterator presetData = itA->begin(); presetData != itA->end(); ++presetData)
			{
				if(presetData.key() == "dummy")
                    return;
                

				ValueTree valueTree = CabbageWidgetData::getValueTreeForComponent(cabbageWidgets, presetData.key(), true);
				const String type = CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::type);
				const String widgetName = CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::name);
				const String channelName = CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::channel);

				if (channelName == "PluginResizerCombBox")
					currentPluginScale = presetData.value().get<int>();

				if (type == CabbageWidgetTypes::texteditor)
				{
					CabbageWidgetData::setStringProp(valueTree, CabbageIdentifierIds::text, presetData.value().dump());
				}
				else if ((type == CabbageWidgetTypes::combobox && CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::filetype).contains("snaps")) ||
                         type == CabbageWidgetTypes::presetbutton)
				{
#if !Cabbage_IDE_Build
                    currentPresetName = presetData.value().get<std::string>();
                    CabbageWidgetData::setStringProp(valueTree, CabbageIdentifierIds::value, currentPresetName);
#endif
				}
				else if ((type == CabbageWidgetTypes::combobox || type == CabbageWidgetTypes::listbox) && CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::channeltype) == "string")
				{
					const String test = presetData.value().dump();
					const String stringComboItem = csdFile.getParentDirectory().getChildFile(presetData.value().dump()).existsAsFile() ?
						csdFile.getParentDirectory().getChildFile(presetData.value().dump()).getFileNameWithoutExtension() : presetData.value().dump();

					if (type == CabbageWidgetTypes::combobox)
						CabbageWidgetData::setStringProp(valueTree, CabbageIdentifierIds::value, stringComboItem); //IMPORTANT: - updates the combobox text..

					CabbageWidgetData::setStringProp(valueTree, CabbageIdentifierIds::value, stringComboItem);

				}
				else if (type == CabbageWidgetTypes::filebutton)
				{
					const int ignoreLastDir = CabbageWidgetData::getNumProp(valueTree,
						CabbageIdentifierIds::ignorelastdir);
					if (ignoreLastDir == 0)
					{

						const String absolutePath = String(presetData.value().dump()).removeCharacters("\"");
						/*const String absolutePath =
						csdFile.getParentDirectory().getChildFile(String(presetData.value().dump()).replaceCharacters("\\", "/")).getFullPathName();*/
						CabbageWidgetData::setStringProp(valueTree, CabbageIdentifierIds::file, absolutePath.toUTF8().getAddress());
					}
				}
				else if (type == CabbageWidgetTypes::soundfiler)
				{
					nlohmann::ordered_json b = nlohmann::ordered_json::parse(presetData.value().dump());

					const String absolutePath = String(b[CabbageIdentifierIds::file.toString().toStdString()].dump()).removeCharacters("\"");
					const int scrubberPos = b[CabbageIdentifierIds::scrubberposition.toString().toStdString()].get<int>();
					const int regionStart = b[CabbageIdentifierIds::regionstart.toString().toStdString()].get<int>();
					const int regionLength = b[CabbageIdentifierIds::regionlength.toString().toStdString()].get<int>();

					/*const String absolutePath =
					csdFile.getParentDirectory().getChildFile(String(presetData.value().dump()).replaceCharacters("\\", "/")).getFullPathName();*/
                    //if this is called by the host, it may be that the widgets have not yet being created...
                    Timer::callAfterDelay(100, [valueTree, absolutePath, scrubberPos, regionStart, regionLength]()
                    {
                        CabbageWidgetData::setStringProp(valueTree, CabbageIdentifierIds::file, absolutePath.toUTF8().getAddress());
                        CabbageWidgetData::setNumProp(valueTree, CabbageIdentifierIds::scrubberposition, scrubberPos);
                        CabbageWidgetData::setNumProp(valueTree, CabbageIdentifierIds::regionstart, regionStart);
                        CabbageWidgetData::setNumProp(valueTree, CabbageIdentifierIds::regionlength, regionLength);
                    });
					

				}
				//unique widgets taht take two channels...
				else if (type == CabbageWidgetTypes::hrange ||
					type == CabbageWidgetTypes::vrange) //double channel range widgets
				{
					nlohmann::ordered_json b = nlohmann::ordered_json::parse(presetData.value().dump());
					const float min = b["Range Min"].get<float>();
					const float max = b["Range Max"].get<float>();

					CabbageWidgetData::setNumProp(valueTree, CabbageIdentifierIds::minvalue, min);

					for (auto cabbageParam : getCabbageParameters())
					{
						if (widgetName + "_min" == cabbageParam->getWidgetName())
						{
							cabbageParam->beginChangeGesture();
							cabbageParam->setValueNotifyingHost(cabbageParam->getNormalisableRange().convertTo0to1(min));
							cabbageParam->endChangeGesture();
						}
					}

					CabbageWidgetData::setNumProp(valueTree, CabbageIdentifierIds::maxvalue, max);

					for (auto cabbageParam : getCabbageParameters())
					{
						if (widgetName + "_max" == cabbageParam->getWidgetName())
						{
							cabbageParam->beginChangeGesture();
							cabbageParam->setValueNotifyingHost(cabbageParam->getNormalisableRange().convertTo0to1(max));
							cabbageParam->endChangeGesture();
						}
					}
				}
				else if (type == CabbageWidgetTypes::xypad) //double channel range widgets
				{

					nlohmann::ordered_json b = nlohmann::ordered_json::parse(presetData.value().dump());
					const float x = b["X"].get<float>();
					const float y = b["Y"].get<float>();

					CabbageWidgetData::setNumProp(valueTree, CabbageIdentifierIds::valuex,x);

					for (auto cabbageParam : getCabbageParameters())
					{
						if (widgetName + "_x" == cabbageParam->getWidgetName())
						{
							cabbageParam->beginChangeGesture();
							cabbageParam->setValueNotifyingHost(cabbageParam->getNormalisableRange().convertTo0to1(x));
							cabbageParam->endChangeGesture();
						}
					}


					/**presetData++;*/
					CabbageWidgetData::setNumProp(valueTree, CabbageIdentifierIds::valuey, y);

					for (auto cabbageParam : getCabbageParameters())
					{
						if (widgetName + "_y" == cabbageParam->getWidgetName())
						{
							cabbageParam->beginChangeGesture();
							cabbageParam->setValueNotifyingHost(cabbageParam->getNormalisableRange().convertTo0to1(y));
							cabbageParam->endChangeGesture();
						}
					}

				}
				else
				{
					if (presetData.key() == "cabbageJSONData")
					{
						if (getCsound())
						{
							auto** p = (CabbagePersistentData**)getCsound()->QueryGlobalVariable("cabbageData");

							if (p != nullptr)
							{
								auto pdClass = *p;
								pdClass->data = json::parse(presetData.value().dump());
								Logger::writeToLog(presetData.value().dump());
							}
							else
								Logger::writeToLog("cabbageJSONData is null");
						}
					}
					else
					{
						if (channelName.isNotEmpty())
						{
							if (CabbageWidgetData::getStringProp(valueTree, "filetype") != "preset"
								&& CabbageWidgetData::getStringProp(valueTree, "filetype") != "*.snaps" &&
								CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::channeltype) != "string")


								CabbageWidgetData::setNumProp(valueTree, CabbageIdentifierIds::value,
									presetData.value().get<float>());
							//now make changes parameter changes so host can see them..
							//getParameters().

							for (auto cabbageParam : getCabbageParameters())
							{
								if (widgetName == cabbageParam->getWidgetName())
								{
									cabbageParam->beginChangeGesture();
									cabbageParam->setValueNotifyingHost(cabbageParam->getNormalisableRange().convertTo0to1(presetData.value().get<float>()));
									cabbageParam->endChangeGesture();
								}
							}
						}
					}
				}

			}

		}

    }
    }
    catch (nlohmann::json::exception& e) {
        DBG(e.what());
        jassertfalse;
    }
}

void CabbagePluginProcessor::restorePluginPreset(String presetName, String fileName)
{

    currentPresetName = presetName;
    
    nlohmann::ordered_json j;
    File presetFile(fileName);
    String presetFileContents = presetFile.loadFileAsString();

    try{
	j = nlohmann::ordered_json::parse(presetFileContents.toRawUTF8());
	setPluginState(j, presetName);
    }
    catch (nlohmann::json::exception& e) {
        DBG(e.what());
        jassertfalse;
    }
}
//===============================================================================================

void CabbagePluginProcessor::getIdentifierDataFromCsound()
{
    
    if(!getCsound())
        return;

    pd = (CabbageWidgetIdentifiers**)getCsound()->QueryGlobalVariable("cabbageWidgetData");
    
    if (pd == nullptr)
        return;
    
    identData = *pd;
    
    for(auto && i : identData->data)
    {
        if(i.identifier.getCharPointer().getAddress() != nullptr)
        {
            const auto identifier = i.identifier;
            const auto name = i.name;

            if(cabbageWidgets.getChildWithName(name).isValid())
            {
                const auto child = cabbageWidgets.getChildWithName(name);
                const String widgetType(CabbageWidgetData::getStringProp(child, "type"));

                if(!i.args.isUndefined())
                {
                    if(!i.identWithArgument)
                    {
                        //any widgets taht break identifiers into unique entities must be parsed....
                        if(identifier.toString().containsIgnoreCase("colour"))
                        {
                            String colourTokens;
                            for(int x = 0 ; x < i.args.size() ; x++){
                                colourTokens += String(int(i.args[x])) + ",";
                            }
                            if(identifier.toString().contains(":"))
                                CabbageWidgetData::setColourByNumber(colourTokens.dropLastCharacters(1), cabbageWidgets.getChildWithName(name), identifier.toString());
                            else
                                cabbageWidgets.getChildWithName(name).setProperty(identifier, CabbageWidgetData::getColourFromText(colourTokens.dropLastCharacters(1)).toString(), nullptr);
                        }
                        else if(identifier == CabbageIdentifierIds::populate)
                        {
                            cabbageWidgets.getChildWithName(name).setProperty(CabbageIdentifierIds::refreshfiles,Random::getSystemRandom().nextInt(), nullptr);
                        }
                        else if(identifier == CabbageIdentifierIds::bounds)
                        {
                            CabbageWidgetData::setBounds(cabbageWidgets.getChildWithName(name), juce::Rectangle<int>( i.args[0],
                                                                                                               i.args[1],
                                                                                                               i.args[2],
                                                                                                               i.args[3]));
                        }
                        else if (identifier == CabbageIdentifierIds::rotate)
                        {
                            cabbageWidgets.getChildWithName(name).setProperty(CabbageIdentifierIds::rotate, i.args[0], nullptr);
                            cabbageWidgets.getChildWithName(name).setProperty(CabbageIdentifierIds::pivotx, i.args[1], nullptr);
                            cabbageWidgets.getChildWithName(name).setProperty(CabbageIdentifierIds::pivoty, i.args[2], nullptr);
                        }
                        /*else if (widgetType == CabbageWidgetTypes::hrange || widgetType == CabbageWidgetTypes::hrange &&
                            identifier == CabbageIdentifierIds::value)
                        {
                            var channels = cabbageWidgets.getChildWithName(name).getProperty(CabbageIdentifierIds::channel);
                            const float min = getCsound()->GetChannel(channels[0].toString().toUTF8());
                            const float max = getCsound()->GetChannel(channels[1].toString().toUTF8());
                            cabbageWidgets.getChildWithName(name).setProperty(CabbageIdentifierIds::minvalue, min, nullptr);
                            cabbageWidgets.getChildWithName(name).setProperty(CabbageIdentifierIds::maxvalue, max, nullptr);

                        }*/
                        else
                        {
                            cabbageWidgets.getChildWithName(name).setProperty(identifier,i.args, nullptr);
                        }


                        if(identifier == CabbageIdentifierIds::value && getChnsetGestureMode() == 1)
                        {
                            var channels = cabbageWidgets.getChildWithName(name).getProperty(CabbageIdentifierIds::channel);
                            for (auto cabbageParam : getCabbageParameters())
                            {
                                if (cabbageParam->getChannel() == channels[0].toString())
                                {
    #if !Cabbage_IDE_Build
                                    if(pluginType.isAbletonLive())
                                        if(cabbageParam->isPerformingGesture==true)
                                            cabbageParam->endChangeGesture();
    #endif
                                    cabbageParam->beginChangeGesture();
                                    cabbageParam->setValueNotifyingHost(cabbageParam->getNormalisableRange().convertTo0to1(getCsound()->GetChannel(channels[0].toString().toUTF8())));
    #if !Cabbage_IDE_Build
                                    if(!pluginType.isAbletonLive())
    #endif
                                    cabbageParam->endChangeGesture();
                                }
                            }
                        }
                    }
                    else
                    {                       
                        const auto argString = i.args.toString();
                        CabbageWidgetData::setCustomWidgetState(cabbageWidgets.getChildWithName(name), argString.paddedLeft(' ',1));
                        if(argString.contains(CabbageIdentifierIds::populate))
                        {
                            CabbageWidgetData::setProperty(cabbageWidgets.getChildWithName(name), CabbageIdentifierIds::update, Random::getSystemRandom().nextInt());
                        }
                        /* the following lets up trigger an update even if moveBehind, or toFront have not changed */
                        else if(argString.contains(CabbageIdentifierIds::tofront))
                        {
                            CabbageWidgetData::setProperty(cabbageWidgets.getChildWithName(name), CabbageIdentifierIds::tofront, Random::getSystemRandom().nextInt());
                        }
                        else if(argString.contains(CabbageIdentifierIds::movebehind))
                        {
                            const String moveBehind = CabbageWidgetData::getProperty(cabbageWidgets.getChildWithName(name), CabbageIdentifierIds::movebehind);
                            const String chn = CabbageWidgetData::getProperty(cabbageWidgets.getChildWithName(name), CabbageIdentifierIds::channel)[0];
                            CabbageWidgetData::setProperty(cabbageWidgets.getChildWithName(name), CabbageIdentifierIds::movebehind, "");
                            CabbageWidgetData::setProperty(cabbageWidgets.getChildWithName(name), CabbageIdentifierIds::update, moveBehind);
                        }


                    }
                }
            }
        }
    }

    identData->data.clearQuick();


    
}
//==============================================================================
// This method is responsible for updating widget valuetrees based on the current
// data stored in each widget's software channel bus. 
//==============================================================================
void CabbagePluginProcessor::getChannelDataFromCsound()
{
	if (!getCsound())
		return;

    const int gestureMode = getChnsetGestureMode();
	for (int i = 0; i < cabbageWidgets.getNumChildren(); i++)
	{
		const var chanArray = CabbageWidgetData::getProperty(cabbageWidgets.getChild(i), CabbageIdentifierIds::channel);
		const String channelName = (chanArray.size() > 0 ? chanArray[0].toString() : chanArray.toString());
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
					//now update plugin parameters..
					
					if (gestureMode == 1) // by default, we don't call beginChangeGesture()...
					{
						for (auto cabbageParam : getCabbageParameters())
						{
							if (cabbageParam->getChannel() == channels[0].toUTF8())
							{
								cabbageParam->beginChangeGesture();
								cabbageParam->setValueNotifyingHost(cabbageParam->getNormalisableRange().convertTo0to1(getCsound()->GetChannel(channels[0].toUTF8())));
								cabbageParam->endChangeGesture();
							}
						}
					}
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
					//                    const float minValue = CabbageWidgetData::getNumProp(cabbageWidgets.getChild(i),
					//                        CabbageIdentifierIds::minvalue);
					//                    const float maxValue = CabbageWidgetData::getNumProp(cabbageWidgets.getChild(i),
					//                        CabbageIdentifierIds::maxvalue);
					CabbageWidgetData::setNumProp(cabbageWidgets.getChild(i), CabbageIdentifierIds::minvalue,
						getCsound()->GetChannel(channels[0].toUTF8()));
					CabbageWidgetData::setNumProp(cabbageWidgets.getChild(i), CabbageIdentifierIds::maxvalue,
						getCsound()->GetChannel(channels[1].toUTF8()));
				}
			}
		}

		if (identChannel.isNotEmpty())
        {
			const String identChannelMessage = CabbageWidgetData::getStringProp(cabbageWidgets.getChild(i),
				CabbageIdentifierIds::identchannelmessage);
			memset(&tmp_string[0], 0, sizeof(tmp_string));
			getCsound()->GetStringChannel(identChannel.toUTF8(), tmp_string);

			const String identifierText(tmp_string);
			//CabbageUtilities::debug(identifierText);
			if (identifierText.isNotEmpty() && identifierText != identChannelMessage)
            {
                String padded = identifierText.paddedLeft(' ', 1);
                CabbageWidgetData::setCustomWidgetState(cabbageWidgets.getChild(i), padded);

				if (identifierText.contains("tableNumber")) //update even if table number has not changed
					CabbageWidgetData::setProperty(cabbageWidgets.getChild(i), CabbageIdentifierIds::update, 1);
				else if (identifierText == CabbageIdentifierIds::tofront.toString() + "()") {
					CabbageWidgetData::setProperty(cabbageWidgets.getChild(i), CabbageIdentifierIds::tofront,
						Random::getSystemRandom().nextInt());
				}

				getCsound()->SetChannel(identChannel.toUTF8(), (char*) "");
				
				CabbageWidgetData::setProperty(cabbageWidgets.getChild(i), CabbageIdentifierIds::update,
					0); //reset value for further updates

			}
			else
			{
				float update = CabbageWidgetData::getProperty(cabbageWidgets.getChild(i), CabbageIdentifierIds::update);
				if (update == 1.0f)
					CabbageWidgetData::setProperty(cabbageWidgets.getChild(i), CabbageIdentifierIds::update,
						0);
			}
		}
		// reset value for widgets without identchannel
		else
		{
			//CabbageWidgetData::setProperty(cabbageWidgets.getChild(i), CabbageIdentifierIds::update, 0);
		}
	}
}

//================================================================================
void CabbagePluginProcessor::addXYAutomator(CabbageXYPad* xyPad, const ValueTree& wData) {
	int indexOfAutomator = -1;

	for (int i = 0; i < xyAutomators.size(); i++) {
		if (xyPad->getName() == xyAutomators[i]->getName())
			indexOfAutomator = i;
	}

	if (indexOfAutomator == -1) {
		XYPadAutomator* xyAuto;
		CabbagePluginParameter* xParameter = getParameterForXYPad(xyPad->getName() + "_x");
		CabbagePluginParameter* yParameter = getParameterForXYPad(xyPad->getName() + "_y");

		if (xParameter && yParameter) {
			xyAutomators.add(xyAuto = new XYPadAutomator(xyPad->getName(), xParameter, yParameter, this));
			xyAuto->setXMin(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::minx));
			xyAuto->setYMin(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::miny));
			xyAuto->setXMax(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::maxx));
			xyAuto->setYMax(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::maxy));
			xyAuto->removeAllChangeListeners();
			xyAuto->addChangeListener(xyPad);
		}
	}
	else {
		xyAutomators[indexOfAutomator]->removeAllChangeListeners();
		xyAutomators[indexOfAutomator]->addChangeListener(xyPad);
	}
}


void CabbagePluginProcessor::enableXYAutomator(String name, bool enable, Line<float> dragLine) {

	for (XYPadAutomator* xyAuto : xyAutomators) {
		if (name == xyAuto->getName()) {
			if (enable == true) {
				xyAuto->setDragLine(dragLine);
				xyAuto->setXValue(dragLine.getEndX());
				xyAuto->setYValue(dragLine.getEndY());
				xyAuto->setXValueIncrement((dragLine.getEndX() - dragLine.getStartX()) * .05);
				xyAuto->setYValueIncrement((dragLine.getEndY() - dragLine.getStartY()) * .05);
				xyAuto->setRepaintBackground(true);
                if(editorIsOpen)
                    xyAuto->setIsPluginEditorOpen(true);
				xyAuto->startTimer(20);
			}
			else
				xyAuto->stopTimer();
		}
	}
}

void CabbagePluginProcessor::disableXYAutomators() 
{
	for (XYPadAutomator* xyAuto : xyAutomators) 
	{
		xyAuto->setIsPluginEditorOpen(false);
	}
}

//======================================================================================================
CabbagePluginParameter* CabbagePluginProcessor::getParameterForXYPad(StringRef name) const {
	for (auto param : getCabbageParameters()) {
		if (auto * cabbageParam = dynamic_cast<CabbagePluginParameter*> (param)) {
			if (name == cabbageParam->getWidgetName())
				return dynamic_cast<CabbagePluginParameter*> (cabbageParam);
		}
	}

	return nullptr;
}

//==============================================================================
void CabbagePluginProcessor::setCabbageParameter(String& channel, float value, ValueTree& wData)
{
    if(pollingChannels() == 0){
        MessageManager::callAsync ([ wData, channel, value](){
            const String widgetType = CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::type);
            if(widgetType == CabbageWidgetTypes::hrange || widgetType == CabbageWidgetTypes::vrange)
            {
                var channels = CabbageWidgetData::getProperty(wData, CabbageIdentifierIds::channel);
                if(channel == channels[0].toString())
                    CabbageWidgetData::setNumProp(wData, CabbageIdentifierIds::minvalue, value);
                else
                    CabbageWidgetData::setNumProp(wData, CabbageIdentifierIds::maxvalue, value);
                
            }
            else if(widgetType == CabbageWidgetTypes::xypad)
            {
                var channels = CabbageWidgetData::getProperty(wData, CabbageIdentifierIds::channel);
                if(channel == channels[0].toString())
                    CabbageWidgetData::setNumProp(wData, CabbageIdentifierIds::valuex, value);
                else
                    CabbageWidgetData::setNumProp(wData, CabbageIdentifierIds::valuey, value);
            }
            
            else
                CabbageWidgetData::setNumProp(wData, CabbageIdentifierIds::value, value);
        });
    }
    
    if (getCsound())
		getCsound()->SetChannel(channel.toUTF8().getAddress(), value);
    

    
}

void CabbagePluginProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	//getCsound()->Message("CABBAGE: prepareToPlay() called by host\n");

	bool csoundRecompiled = false;
	String jsonStateData;

	
	//grab the current state so we can reinstate it if Csound is recompiled due to channel/SR changes..
	if (getCsound())
	{
		auto** p = (CabbagePersistentData**)getCsound()->QueryGlobalVariable("cabbageData");

		if (p != nullptr)
		{
			auto pdClass = *p;
			jsonStateData = pdClass->data;
		}
	}


#if !Cabbage_IDE_Build && !Cabbage_Lite

	//samplingRate = sampleRate;
	CsoundPluginProcessor::prepareToPlay(sampleRate, samplesPerBlock);
	
	csoundRecompiled = true;
	if (sampleRate != samplingRate)
	{
		initAllCsoundChannels(cabbageWidgets);
	}
	
#endif

	//DBG(cabbageWidgets.toXmlString());
	if (sampleRate != samplingRate && csoundRecompiled == false) {
		samplingRate = sampleRate;
		CsoundPluginProcessor::prepareToPlay(sampleRate, samplesPerBlock);
		initAllCsoundChannels(cabbageWidgets);
	}

	if (getCsound())
	{
		auto** p = (CabbagePersistentData**)getCsound()->QueryGlobalVariable("cabbageData");

		if (p != nullptr)
		{
			auto pdClass = *p;
			pdClass->data = jsonStateData.toStdString();
		}
	}


	//some host call prepreToPlay mutliple times, this can cause channel changed events to be missed. Therefore it's best 
	//to reinit and update all channels every time Csound is recompiled. 'hostStateData' will always reflect the current state
	if (wasRecompiled())
	{
		initAllCsoundChannels(cabbageWidgets);
		setPluginState(hostStateData, "", true);
		resetRecompiled();
	}
	
	

}









