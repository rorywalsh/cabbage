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

AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    File csdFile;
#ifndef JUCE_MAC
    csdFile = File::getSpecialLocation (File::currentExecutableFile).withFileExtension (String (".csd")).getFullPathName();
#else
    //read .csd file from the correct location within the .vst bundle.
    const String dir = File::getSpecialLocation (File::currentExecutableFile).getParentDirectory().getParentDirectory().getFullPathName();
    const String filename (File::getSpecialLocation (File::currentExecutableFile).withFileExtension (String (".csd")).getFileName());
    csdFile = File (dir + "/" + filename);

#endif
    return new CabbagePluginProcessor (csdFile);
};

//============================================================================
CabbagePluginProcessor::CabbagePluginProcessor (File inputFile)
    : CsoundPluginProcessor (inputFile),
      csdFile (inputFile),
      cabbageWidgets ("CabbageWidgetData")
{

    //initAllCsoundChannels(cabbageWidgets);
    if (inputFile.existsAsFile())
    {
		StringArray linesFromCsd;
		linesFromCsd.addLines (inputFile.loadFileAsString());
		addImportFiles(linesFromCsd);
		File tempFile = File::createTempFile("csoundCabbageCsdText");
		tempFile.replaceWithText(linesFromCsd.joinIntoString("\n")
												.replace("$lt;", "<")
												.replace("&amp;", "&")
												.replace("$quote;", "\"")
												.replace("$gt;", ">"));
		setupAndCompileCsound(tempFile);
		parseCsdFile (linesFromCsd);
        createParameters();
		csoundChanList = NULL;

		initAllCsoundChannels(cabbageWidgets);
    }



    
}

CabbagePluginProcessor::~CabbagePluginProcessor()
{
    for ( auto xyAuto : xyAutomators)
        xyAuto->removeAllChangeListeners();

    xyAutomators.clear();
}

//==============================================================================
void CabbagePluginProcessor::parseCsdFile (StringArray& linesFromCsd)
{
    cabbageWidgets.removeAllChildren (0);
    String parentComponent, previousComponent;

    searchForMacros (linesFromCsd);
	int linesToSkip=0;

    for ( int lineNumber = 0; lineNumber < linesFromCsd.size() ; lineNumber++ )
    {
        if (linesFromCsd[lineNumber].equalsIgnoreCase ("</Cabbage>"))
            return;

        const String widgetTreeIdentifier = "WidgetFromLine_" + String (lineNumber);
        ValueTree tempWidget (widgetTreeIdentifier);

        String currentLineOfCabbageCode = linesFromCsd[lineNumber].replace("\t", " ");

        if (currentLineOfCabbageCode.contains (" \\"))
        {
            for (int index = lineNumber + 1;; index++)
            {

                if (linesFromCsd[index].contains (" \\"))
                    currentLineOfCabbageCode += linesFromCsd[index];
                else
                {
                    currentLineOfCabbageCode += linesFromCsd[index];
                    break;
                }

            }
        }

        const String expandedMacroText = getExpandedMacroText (currentLineOfCabbageCode, tempWidget);

        if ( currentLineOfCabbageCode.indexOf (";") > -1)
            currentLineOfCabbageCode = currentLineOfCabbageCode.substring (0, currentLineOfCabbageCode.indexOf (";"));

        const String comments = currentLineOfCabbageCode.indexOf (";") == -1 ? "" : currentLineOfCabbageCode.substring (currentLineOfCabbageCode.indexOf (";"));
        CabbageWidgetData::setWidgetState (tempWidget, currentLineOfCabbageCode.trimCharactersAtStart (" \t") + " " + expandedMacroText + comments, lineNumber);
        const int partOfPlant = CabbageWidgetData::getNumProp (tempWidget, CabbageIdentifierIds::plant);
		if(partOfPlant>0)
		{
			linesToSkip = partOfPlant;
			CabbageWidgetData::setNumProp (tempWidget, CabbageIdentifierIds::linenumber, 99999+lineNumber);
		}
		CabbageWidgetData::setNumProp (tempWidget, CabbageIdentifierIds::linenumber, lineNumber-linesToSkip);
		
        CabbageWidgetData::setStringProp (tempWidget, CabbageIdentifierIds::csdfile, csdFile.getFullPathName());
        CabbageWidgetData::setStringProp (tempWidget, CabbageIdentifierIds::expandedmacrotext, expandedMacroText);

        if (CabbageWidgetData::getStringProp (tempWidget, CabbageIdentifierIds::type) == CabbageWidgetTypes::form)
        {
            const String caption = CabbageWidgetData::getStringProp (tempWidget, CabbageIdentifierIds::caption);
            this->setPluginName (caption.length() > 0 ? caption : "Untitled");

			

            if (CabbageWidgetData::getNumProp (tempWidget, CabbageIdentifierIds::logger) == 1)
                createFileLogger (this->csdFile);
        }

        if (currentLineOfCabbageCode.contains ("}"))
        {
            parentComponent = "";
        }

        if (parentComponent.isNotEmpty())
            CabbageWidgetData::setStringProp (tempWidget, CabbageIdentifierIds::parentcomponent, parentComponent);

		const String widgetName = CabbageWidgetData::getStringProp(tempWidget, CabbageIdentifierIds::name);

		if(widgetName.isNotEmpty())
			cabbageWidgets.addChild (tempWidget, -1, 0);

		if (CabbageWidgetData::getProperty (tempWidget, CabbageIdentifierIds::widgetarray).size() > 0 &&
			CabbageWidgetData::getProperty (tempWidget, CabbageIdentifierIds::identchannelarray).size() > 0)
		{
			for (int i = 0; i < CabbageWidgetData::getProperty (tempWidget, CabbageIdentifierIds::widgetarray).size(); i++)
			{
				ValueTree copy = tempWidget.createCopy();
				const String chan = CabbageWidgetData::getProperty (tempWidget, CabbageIdentifierIds::widgetarray)[i].toString();
				CabbageWidgetData::setStringProp (copy, CabbageIdentifierIds::channel, CabbageWidgetData::getProperty (tempWidget, CabbageIdentifierIds::widgetarray)[i]);
				CabbageWidgetData::setStringProp (copy, CabbageIdentifierIds::identchannel, CabbageWidgetData::getProperty (tempWidget, CabbageIdentifierIds::identchannelarray)[i]);
				cabbageWidgets.addChild (copy, -1, 0);
			}
		}


        if (currentLineOfCabbageCode.contains ("{"))
        {
            if (currentLineOfCabbageCode.removeCharacters (" ") == "{")
            {
                parentComponent = previousComponent;
            }
            else
            {
                parentComponent = CabbageWidgetData::getProperty (tempWidget, CabbageIdentifierIds::name).toString();
                CabbageWidgetData::setProperty (tempWidget, "containsOpeningCurlyBracket", 1);
            }
        }

        previousComponent = CabbageWidgetData::getProperty (tempWidget, CabbageIdentifierIds::name).toString();
    }
}

void CabbagePluginProcessor::addImportFiles(StringArray& linesFromCsd)
{
	PlantImportStruct importData;
    searchForMacros (linesFromCsd);

	for (int i = 0 ; i < linesFromCsd.size() ; i++)
	{

        ValueTree temp ("temp");
        const String expandedMacroText = getExpandedMacroText (linesFromCsd[i], temp);
		CabbageWidgetData::setWidgetState (temp, linesFromCsd[i]+" "+expandedMacroText, 0);



		if (CabbageWidgetData::getStringProp (temp, CabbageIdentifierIds::type) == CabbageWidgetTypes::form)
		{
			var files = CabbageWidgetData::getProperty (temp, CabbageIdentifierIds::importfiles);
			for( int y = 0 ; y < files.size() ; y++)
			{
				if(csdFile.getParentDirectory().getChildFile (files[y].toString()).existsAsFile())
				{
					StringArray linesFromImportedFile;
                    linesFromImportedFile.addLines(csdFile.getParentDirectory().getChildFile (files[y].toString()).loadFileAsString());
					
					
					ScopedPointer<XmlElement> xml;
					xml = XmlDocument::parse(csdFile.getParentDirectory().getChildFile (files[y].toString()));
					
					if(!xml)
					{				
						for ( int y = linesFromImportedFile.size() ; y >= 0 ; y--)
						{
							linesFromCsd.insert(i+1, linesFromImportedFile[y]);
							//CabbageUtilities::debug(linesFromImportedFile[y]);
						}
					}
					else
					{
						handleXmlImport(importData, xml, linesFromCsd);

					}
				}
			}
		}
	}
		insertPlantCode(importData, linesFromCsd);							
		insertUDOCode(importData, linesFromCsd);

	CabbageUtilities::debug(linesFromCsd.joinIntoString("\n"));	
}


void CabbagePluginProcessor::handleXmlImport(PlantImportStruct& importData, XmlElement* xml, StringArray &linesFromCsd)
{
	if (xml->hasTagName ("plant"))
	{							
		forEachXmlChildElement (*xml, e)
		{
			if(e->getTagName() == "namespace")
				importData.nsp = e->getAllSubText();
			if(e->getTagName() == "name")
				importData.name = e->getAllSubText();
			if(e->getTagName() == "cabbagecode")
			{
				importData.cabbageCode.addLines(e->getAllSubText());
			}
			if(e->getTagName() == "csoundcode")
				importData.csoundCode = e->getAllSubText();	
			if(e->getTagName() == "cabbagecodescript")
			{
				generateCabbageCodeFromJS(importData, e->getAllSubText());		
			}									
		}
		
		
		StringArray strArray;
	}				
}
void CabbagePluginProcessor::insertPlantCode(PlantImportStruct importData, StringArray& linesFromCsd)
{
    searchForMacros (linesFromCsd);
	StringArray importedLines("");
	bool firstPass = true;
	float scaleX = 1;
	float scaleY = 1;
	int lineIndex = 0;
	StringArray copy = linesFromCsd;
	for ( auto str : copy )
	{
		if(str.length()>0)
		{
			//CabbageUtilities::debug(str);
			ValueTree temp ("temp");
            const String expandedMacroText = getExpandedMacroText (str, temp);
			CabbageWidgetData::setWidgetState(temp, str + " " + expandedMacroText, lineIndex++);
			CabbageWidgetData::setCustomWidgetState(temp, str);
			const String type = CabbageWidgetData::getStringProp(temp, CabbageIdentifierIds::type);
			const String nsp = CabbageWidgetData::getStringProp(temp, CabbageIdentifierIds::nsp);
			
			
			if(type==importData.name && importData.nsp==nsp)
			{
				int lineNumber = copy.indexOf(str);
				
				for ( auto plantCode : importData.cabbageCode)
				{
					if(plantCode.isNotEmpty())
					{
						if(plantCode.contains("}") == false)
						{
							ValueTree temp1 ("temp1");
                            const String expandedMacroText = getExpandedMacroText (plantCode, temp);
                            //CabbageUtilities::debug(plantCode);
							CabbageWidgetData::setWidgetState(temp1, plantCode + " " + expandedMacroText, -99);
							CabbageWidgetData::setCustomWidgetState(temp1, plantCode);
							CabbageWidgetData::setNumProp(temp1, CabbageIdentifierIds::plant, importData.cabbageCode.size()+2);
							CabbageWidgetData::setNumProp(temp, CabbageIdentifierIds::plant, importData.cabbageCode.size()+2);
							const int lineNumberPlantAppearsOn = CabbageWidgetData::getNumProp(temp, CabbageIdentifierIds::linenumber);
							CabbageWidgetData::setNumProp(temp1, CabbageIdentifierIds::surrogatelinenumber, lineNumberPlantAppearsOn);

							if(firstPass)
							{
								//CabbageUtilities::debug(lineNumberPlantAppearsOn);
								scaleX = CabbageWidgetData::getNumProp(temp, CabbageIdentifierIds::width)/CabbageWidgetData::getNumProp(temp1, CabbageIdentifierIds::width);
								scaleY = CabbageWidgetData::getNumProp(temp, CabbageIdentifierIds::height)/CabbageWidgetData::getNumProp(temp1, CabbageIdentifierIds::height);
								CabbageWidgetData::setBounds(temp1, CabbageWidgetData::getBounds(temp));
							}
							else
							{
								const float width = CabbageWidgetData::getNumProp(temp1, CabbageIdentifierIds::width)*scaleX;
								const float height = CabbageWidgetData::getNumProp(temp1, CabbageIdentifierIds::height)*scaleY;
								const float top = CabbageWidgetData::getNumProp(temp1, CabbageIdentifierIds::top)*scaleY;
								const float left = CabbageWidgetData::getNumProp(temp1, CabbageIdentifierIds::left)*scaleX;												
								CabbageWidgetData::setNumProp(temp1, CabbageIdentifierIds::width, width);
								CabbageWidgetData::setNumProp(temp1, CabbageIdentifierIds::height, height); 
								CabbageWidgetData::setNumProp(temp1, CabbageIdentifierIds::top, top);
								CabbageWidgetData::setNumProp(temp1, CabbageIdentifierIds::left, left); 

							}
							//add test for multiple channels...
							const String currentChannel = CabbageWidgetData::getStringProp(temp1, CabbageIdentifierIds::channel);
							const String channelPrefix = CabbageWidgetData::getStringProp(temp, CabbageIdentifierIds::channel);
							const String currentIdentChannel = CabbageWidgetData::getStringProp(temp1, CabbageIdentifierIds::identchannel);
							
							

							
							CabbageWidgetData::setStringProp(temp1, CabbageIdentifierIds::channel, channelPrefix+currentChannel);
                            if(CabbageWidgetData::getStringProp(temp1, CabbageIdentifierIds::identchannel).isNotEmpty())
							    CabbageWidgetData::setStringProp(temp1, CabbageIdentifierIds::identchannel, channelPrefix+currentIdentChannel);
							String replacementText = (plantCode.indexOf("{")!=-1 ? 
															CabbageWidgetData::getCabbageCodeFromIdentifiers(temp1, plantCode, "")+"{"
															: CabbageWidgetData::getCabbageCodeFromIdentifiers(temp1, plantCode, ""));

							importedLines.add(replacementText);
							firstPass = false;											
							}
						else
							{
							importedLines.add("}");
							}
						}
					}
					
					for ( int y = importedLines.size() ; y >= 0 ; y--)
						linesFromCsd.insert(lineNumber+1, importedLines[y]);					
								
			}
		}
	}
}

void CabbagePluginProcessor::insertUDOCode(PlantImportStruct importData, StringArray& linesFromCsd)
{
	for ( auto str : linesFromCsd )
	{
		if(str.substring(0, String("<CsInstruments>").length())=="<CsInstruments>")
		{
			StringArray strArray;
			strArray.addLines(importData.csoundCode);
			const int lineToInsertTo = linesFromCsd.indexOf(str);
			for ( int y = strArray.size() ; y >= 0 ; y--)
				linesFromCsd.insert(lineToInsertTo+1, strArray[y]);						
			return;
		}
	}
}

void CabbagePluginProcessor::generateCabbageCodeFromJS(PlantImportStruct &importData, String text)
{
	JavascriptEngine engine;
	engine.maximumExecutionTime = RelativeTime::seconds (5);
	engine.registerNativeObject ("Cabbage", new CabbageJavaClass (this));

	const double startTime = Time::getMillisecondCounterHiRes();

	Result result = engine.execute (text.replace("$lt;", "<")
			.replace("&amp;", "&")
			.replace("$quote;", "\"")
			.replace("$gt;", ">"));

	const double elapsedMs = Time::getMillisecondCounterHiRes() - startTime;

	importData.cabbageCode.addLines(cabbageScriptGeneratedCode.joinIntoString("\n"));

	if (result.failed())
		CabbageUtilities::showMessage("javaScript Error:" + result.getErrorMessage(), &getActiveEditor()->getLookAndFeel());
	
}

void CabbagePluginProcessor::searchForMacros (StringArray& linesFromCsd)
{
    for (String csdLine : linesFromCsd) //deal with Cabbage macros
    {
        StringArray tokens;
        csdLine = csdLine.replace ("\n", " ");
        tokens.addTokens (csdLine, ", ");

        if (tokens[0].containsIgnoreCase ("define"))
        {
            tokens.removeEmptyStrings();

            if (tokens.size() > 1)
            {
                const String currentMacroText = csdLine.substring (csdLine.indexOf (tokens[1]) + tokens[1].length()) + " ";
                //first identifiers are not being used for some reason. This hack fixes that, but should be tidied up..
				macroText.set ("$" + tokens[1], " " + currentMacroText + currentMacroText);
            }
        }
    }
}

const String CabbagePluginProcessor::getExpandedMacroText (String line, ValueTree wData)
{
    String csdLine;
    var macroNames;
	String defineText;

	while(line.contains("$"))
	{
		line = line.substring(line.indexOf("$"), line.length());
		defineText = line.substring(0, (line.indexOf(" ")==-1 ? line.length() : line.indexOf(" ")+1));
		line = line.replace(defineText, "");
		defineText = defineText.substring(0, (defineText.indexOf(" ")!=-1 ? defineText.indexOf(" ") : defineText.length()));
	
		for (int cnt = 0 ; cnt < macroText.size() ; cnt++)
		{
			if (defineText == macroText.getName (cnt).toString())
			{
				csdLine += macroText.getWithDefault (macroText.getName (cnt), "").toString() + " ";
				macroNames.append (macroText.getName (cnt).toString());
			}
		}

		CabbageWidgetData::setProperty (wData, CabbageIdentifierIds::macronames, macroNames);
	}
    return csdLine;
}

//rebuild the entire GUi each time something changes.
void CabbagePluginProcessor::updateWidgets (String csdText)
{
    CabbagePluginEditor* editor = static_cast<CabbagePluginEditor*> (this->getActiveEditor());
	StringArray strings;
	strings.addLines(csdText);
    parseCsdFile (strings);
    editor->createEditorInterface (cabbageWidgets);
    editor->updateLayoutEditorFrames();
}
//==============================================================================
// create parameters for sliders, buttons, comboboxes, checkboxes, encoders and xypads.
// Other widgets can communicate with Csound, but they cannot be automated
void CabbagePluginProcessor::createParameters()
{

    CabbageControlWidgetStrings controlWidgetTypes;

    for (int i = 0; i < cabbageWidgets.getNumChildren(); i++)
    {
        const String typeOfWidget = CabbageWidgetData::getStringProp (cabbageWidgets.getChild (i), CabbageIdentifierIds::type);
        CabbageControlWidgetStrings controlWidgetTypes;

        if (controlWidgetTypes.contains (typeOfWidget))
        {
            const String name = CabbageWidgetData::getStringProp (cabbageWidgets.getChild (i), CabbageIdentifierIds::name);
            const String channel = CabbageWidgetData::getStringProp (cabbageWidgets.getChild (i), CabbageIdentifierIds::channel);
            const var value = CabbageWidgetData::getProperty (cabbageWidgets.getChild (i), CabbageIdentifierIds::value);

            if (controlWidgetTypes.contains (CabbageWidgetData::getStringProp (cabbageWidgets.getChild (i), CabbageIdentifierIds::type)))
            {

                if (typeOfWidget == CabbageWidgetTypes::xypad)
                {
                    const var channel = CabbageWidgetData::getProperty (cabbageWidgets.getChild (i), CabbageIdentifierIds::channel);
					const float increment = CabbageWidgetData::getNumProp(cabbageWidgets.getChild(i), CabbageIdentifierIds::increment);					
                    addParameter (new CabbageAudioParameter (cabbageWidgets.getChild (i), *getCsound(), channel[0] , name + "_x", 0, 1, value, increment, 1));
                    addParameter (new CabbageAudioParameter (cabbageWidgets.getChild (i), *getCsound(), channel[1], name + "_y", 0, 1, value, increment, 1));
                }

                else if (typeOfWidget.contains ("range"))
                {
                    const var channel = CabbageWidgetData::getProperty (cabbageWidgets.getChild (i), CabbageIdentifierIds::channel);
					if (channel.size() > 1)
					{
						const float increment = CabbageWidgetData::getNumProp(cabbageWidgets.getChild(i), CabbageIdentifierIds::increment);
						const int minValue = CabbageWidgetData::getNumProp(cabbageWidgets.getChild(i), CabbageIdentifierIds::minvalue);
						const int maxValue = CabbageWidgetData::getNumProp(cabbageWidgets.getChild(i), CabbageIdentifierIds::maxvalue);
						const float skew = CabbageWidgetData::getNumProp(cabbageWidgets.getChild(i), CabbageIdentifierIds::sliderskew);
						const float min = CabbageWidgetData::getNumProp(cabbageWidgets.getChild(i), CabbageIdentifierIds::min);
						const float max = CabbageWidgetData::getNumProp(cabbageWidgets.getChild(i), CabbageIdentifierIds::max);
						addParameter(new CabbageAudioParameter(cabbageWidgets.getChild(i), *getCsound(), channel[0], name + "_min", min, max, minValue, increment, skew));
						addParameter(new CabbageAudioParameter(cabbageWidgets.getChild(i), *getCsound(), channel[1], name + "_max", min, max, maxValue, increment, skew));
					}
                }
				else if(typeOfWidget == CabbageWidgetTypes::combobox && channel.isNotEmpty())
				{
					const float min = CabbageWidgetData::getNumProp(cabbageWidgets.getChild(i), CabbageIdentifierIds::min);
					const float max = CabbageWidgetData::getNumProp(cabbageWidgets.getChild(i), CabbageIdentifierIds::comborange);
					addParameter (new CabbageAudioParameter (cabbageWidgets.getChild (i), *getCsound(), channel, name, min, max, value, 1, 1));
				}
				else if(typeOfWidget.contains("slider") && channel.isNotEmpty())
				{
					const float increment = CabbageWidgetData::getNumProp(cabbageWidgets.getChild(i), CabbageIdentifierIds::increment);
					const float skew = CabbageWidgetData::getNumProp(cabbageWidgets.getChild(i), CabbageIdentifierIds::sliderskew);
					const float min = CabbageWidgetData::getNumProp(cabbageWidgets.getChild(i), CabbageIdentifierIds::min);
					const float max = CabbageWidgetData::getNumProp(cabbageWidgets.getChild(i), CabbageIdentifierIds::max) > min ? 
										CabbageWidgetData::getNumProp(cabbageWidgets.getChild(i), CabbageIdentifierIds::max) :
										min+1;
					
					addParameter (new CabbageAudioParameter (cabbageWidgets.getChild (i), *getCsound(), channel, name, min, max, value, increment, skew));
				}
                else
                {
					if(channel.isNotEmpty())
						addParameter (new CabbageAudioParameter (cabbageWidgets.getChild (i), *getCsound(), channel, name, 0, 1, value, 1, 1));
                }
            }
        }
    }
}

//==============================================================================
bool CabbagePluginProcessor::hasEditor() const
{
    return true;
}

AudioProcessorEditor* CabbagePluginProcessor::createEditor()
{
    return new CabbagePluginEditor (*this);
}

//==============================================================================
void CabbagePluginProcessor::getStateInformation (MemoryBlock& destData)
{
    copyXmlToBinary (savePluginState ("CABBAGE_PRESETS"), destData);
}

void CabbagePluginProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    ScopedPointer<XmlElement> xmlElement = getXmlFromBinary (data, sizeInBytes);
    restorePluginState (xmlElement);
}

//==============================================================================
XmlElement CabbagePluginProcessor::savePluginState (String xmlTag, File xmlFile)
{
	ScopedPointer<XmlElement> xml;
	
	if(xmlFile.existsAsFile())
	{
		xml = XmlDocument::parse(xmlFile);
		if(!xml)
			xml = new XmlElement("CABBAGE_PRESETS");
		
	}
	else
		xml = new XmlElement("CABBAGE_PRESETS");


	const String presetName = "PRESET"+String(xml->getNumChildElements());
	const String childName = xmlTag+" "+String(xml->getNumChildElements());

	xml->createNewChildElement (presetName);
	xml->getChildByName(presetName)->setAttribute ("PresetName", childName);

    for (int i = 0 ; i < cabbageWidgets.getNumChildren() ; i++)
    {
        const String channelName = CabbageWidgetData::getStringProp (cabbageWidgets.getChild (i), CabbageIdentifierIds::channel);
		//const String widgetName = CabbageWidgetData::getStringProp (cabbageWidgets.getChild (i), CabbageIdentifierIds::name);
		
        const String type = CabbageWidgetData::getStringProp (cabbageWidgets.getChild (i), CabbageIdentifierIds::type);
        const var value = CabbageWidgetData::getProperty (cabbageWidgets.getChild (i), CabbageIdentifierIds::value);

		//only write values for widgets that have channels
		if(channelName.isNotEmpty())
		{
			if (type == CabbageWidgetTypes::texteditor)
			{
				const String text = CabbageWidgetData::getStringProp (cabbageWidgets.getChild (i), CabbageIdentifierIds::text);
				xml->getChildByName(presetName)->setAttribute (channelName, text);
			}
			else if (type == CabbageWidgetTypes::filebutton && !CabbageWidgetData::getStringProp (cabbageWidgets.getChild (i), CabbageIdentifierIds::filetype).contains("snaps"))
			{
				const String file = CabbageWidgetData::getStringProp (cabbageWidgets.getChild (i), CabbageIdentifierIds::file);
				if(file.length()>2)
				{
					const String relativePath = File (file).getRelativePathFrom (File (csdFile));
					xml->getChildByName(presetName)->setAttribute (channelName, relativePath.replaceCharacters("\\", "/"));
				}
			}
			else if(type.contains("range"))//double channel range widgets
			{
				var channels = CabbageWidgetData::getProperty (cabbageWidgets.getChild (i), CabbageIdentifierIds::channel);
				const float minValue = CabbageWidgetData::getNumProp (cabbageWidgets.getChild (i), CabbageIdentifierIds::minvalue);
				const float maxValue = CabbageWidgetData::getNumProp (cabbageWidgets.getChild (i), CabbageIdentifierIds::maxvalue);
				xml->getChildByName(presetName)->setAttribute (channels[0].toString(), minValue);
				xml->getChildByName(presetName)->setAttribute (channels[1].toString(), maxValue);
			}
			else if(type == CabbageWidgetTypes::xypad)//double channel xypad widget
			{
				var channels = CabbageWidgetData::getProperty (cabbageWidgets.getChild (i), CabbageIdentifierIds::channel);
				const float xValue = CabbageWidgetData::getNumProp (cabbageWidgets.getChild (i), CabbageIdentifierIds::valuex);
				const float yValue = CabbageWidgetData::getNumProp (cabbageWidgets.getChild (i), CabbageIdentifierIds::valuey);
				xml->getChildByName(presetName)->setAttribute (channels[0].toString(), xValue);
				xml->getChildByName(presetName)->setAttribute (channels[1].toString(), yValue);
			}
			else
			{
				xml->getChildByName(presetName)->setAttribute (channelName, float(value));
			}
		}
    }

    return *xml;
}

void CabbagePluginProcessor::restorePluginState (XmlElement* xmlState)
{
    if (xmlState != nullptr)
    {		
		//if dealing with session saved by host
		if(xmlState->getNumChildElements()==1)
		{
			setParametersFromXml(xmlState->getChildByName("PRESET0"));
		}
		//else dealing with preset files loaded in editor...
		else
		{
			setParametersFromXml(xmlState);	
		}
        initAllCsoundChannels (cabbageWidgets);
    }

    xmlState = nullptr;
}

void CabbagePluginProcessor::setParametersFromXml(XmlElement* e)
{
	if(e)
	{
		for (int i = 1; i < e->getNumAttributes(); i++)
		{
			ValueTree valueTree = CabbageWidgetData::getValueTreeForComponent (cabbageWidgets, e->getAttributeName (i), true);
			const String type = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::type);

			if (type == CabbageWidgetTypes::texteditor)
				CabbageWidgetData::setStringProp (valueTree, CabbageIdentifierIds::text, e->getAttributeValue (i));
			else if (type == CabbageWidgetTypes::filebutton)
			{
                const String absolutePath = csdFile.getParentDirectory().getFullPathName()+"/"+e->getAttributeValue (i);
                const String path = File(absolutePath).getFullPathName();
				CabbageWidgetData::setStringProp (valueTree, CabbageIdentifierIds::file, absolutePath);
			}
			else if(type == CabbageWidgetTypes::hrange || type == CabbageWidgetTypes::vrange)//double channel range widgets
			{
				CabbageWidgetData::setNumProp (valueTree, CabbageIdentifierIds::minvalue, e->getAttributeValue (i).getFloatValue());
				CabbageWidgetData::setNumProp (valueTree, CabbageIdentifierIds::maxvalue, e->getAttributeValue (i+1).getFloatValue());
				i++;
			}
			else if(type == CabbageWidgetTypes::xypad)//double channel range widgets
			{
				CabbageWidgetData::setNumProp (valueTree, CabbageIdentifierIds::valuex, e->getAttributeValue (i).getFloatValue());
				CabbageWidgetData::setNumProp (valueTree, CabbageIdentifierIds::valuey, e->getAttributeValue (i+1).getFloatValue());
				i++;
			}
			else
			{
				CabbageWidgetData::setNumProp (valueTree, CabbageIdentifierIds::value, e->getAttributeValue (i).getFloatValue());			
			}
		}
	}
}

//==============================================================================
void CabbagePluginProcessor::getChannelDataFromCsound()
{

    for ( int i = 0; i < cabbageWidgets.getNumChildren(); i++)
    {
        const var chanArray = CabbageWidgetData::getProperty (cabbageWidgets.getChild (i), CabbageIdentifierIds::channel);
        const var widgetArray = CabbageWidgetData::getProperty (cabbageWidgets.getChild (i), CabbageIdentifierIds::widgetarray);
        
		StringArray channels;

		if(widgetArray.size()>0)
			channels.add (CabbageWidgetData::getStringProp (cabbageWidgets.getChild (i), CabbageIdentifierIds::channel));
        else if (chanArray.size() == 1)
            channels.add (CabbageWidgetData::getStringProp (cabbageWidgets.getChild (i), CabbageIdentifierIds::channel));
        else if(chanArray.size() > 1)
        {
            for (int j = 0; j < chanArray.size(); j++)
                channels.add (var (chanArray[j]));
        }

        const var value = CabbageWidgetData::getProperty (cabbageWidgets.getChild (i), CabbageIdentifierIds::value);
        const float valuex = CabbageWidgetData::getNumProp (cabbageWidgets.getChild (i), CabbageIdentifierIds::valuex);
        const float valuey = CabbageWidgetData::getNumProp (cabbageWidgets.getChild (i), CabbageIdentifierIds::valuey);
        const String identChannel = CabbageWidgetData::getStringProp (cabbageWidgets.getChild (i), CabbageIdentifierIds::identchannel);
        const String identChannelMessage = CabbageWidgetData::getStringProp (cabbageWidgets.getChild (i), CabbageIdentifierIds::identchannelmessage);
        const String typeOfWidget = CabbageWidgetData::getStringProp (cabbageWidgets.getChild (i), CabbageIdentifierIds::type);

		const String chann = channels[0];

		
		if (channels.size() == 1 && channels[0].isNotEmpty())
		{
				
					if (typeOfWidget != "combobox")	// don't update combobox in here, it will enter a recursive loop
					{
						if (getCsound()->GetChannel (channels[0].toUTF8()) != float(value))
							CabbageWidgetData::setNumProp (cabbageWidgets.getChild (i), CabbageIdentifierIds::value, getCsound()->GetChannel (channels[0].toUTF8()));
					}
					else
					{
						CabbageWidgetData::setNumProp (cabbageWidgets.getChild (i), CabbageIdentifierIds::update, 0);
						if(value.isString() == false)
						{
							if (getCsound()->GetChannel (channels[0].toUTF8()) != float(value))
							{
								CabbageWidgetData::setNumProp (cabbageWidgets.getChild (i), CabbageIdentifierIds::value, getCsound()->GetChannel (channels[0].toUTF8()));
							}
						}
						else
						{
							char tmp_str[4096] = {0};
							getCsound()->GetStringChannel(channels[0].toUTF8(), tmp_str);
							CabbageWidgetData::setProperty (cabbageWidgets.getChild (i), CabbageIdentifierIds::value, String(tmp_str));
						}
					}
		}
		
		//currently only dealing with a max of 2 channels...
		else if(channels.size() == 2 && channels[0].isNotEmpty() && channels[1].isNotEmpty())
		{
			if(getCsound()->GetChannel (channels[0].toUTF8()) != valuex
				|| getCsound()->GetChannel (channels[1].toUTF8()) != valuey)
			{
				if(typeOfWidget == CabbageWidgetTypes::xypad)
				{
					CabbageWidgetData::setNumProp (cabbageWidgets.getChild (i), CabbageIdentifierIds::valuex, getCsound()->GetChannel (channels[0].toUTF8()));
					CabbageWidgetData::setNumProp (cabbageWidgets.getChild (i), CabbageIdentifierIds::valuey, getCsound()->GetChannel (channels[1].toUTF8()));
				}
				else if(typeOfWidget.contains("range"))
				{
					const float minValue = CabbageWidgetData::getNumProp (cabbageWidgets.getChild (i), CabbageIdentifierIds::minvalue);
					const float maxValue = CabbageWidgetData::getNumProp (cabbageWidgets.getChild (i), CabbageIdentifierIds::maxvalue);		
					CabbageWidgetData::setNumProp (cabbageWidgets.getChild (i), CabbageIdentifierIds::minvalue, getCsound()->GetChannel (channels[0].toUTF8()));
					CabbageWidgetData::setNumProp (cabbageWidgets.getChild (i), CabbageIdentifierIds::maxvalue, getCsound()->GetChannel (channels[1].toUTF8()));
				}			
			}	
		}

			
        if (identChannel.isNotEmpty())
        {
            getCsound()->GetStringChannel (identChannel.toUTF8(), tmp_string);
            String identifierText (tmp_string);

            if (identifierText != identChannelMessage)
            {
                CabbageWidgetData::setCustomWidgetState (cabbageWidgets.getChild (i), " " + identifierText);

                if (identifierText.contains ("tablenumber")) //update even if table number has not changed
                    CabbageWidgetData::setProperty (cabbageWidgets.getChild (i), CabbageIdentifierIds::update, 1);

                getCsound()->SetChannel (identChannel.toUTF8(), (char*)"");
				
				CabbageWidgetData::setProperty (cabbageWidgets.getChild (i), CabbageIdentifierIds::update, 0); //reset value for further updates

            }
        }
    }
}

//================================================================================
void CabbagePluginProcessor::addXYAutomator (CabbageXYPad* xyPad, ValueTree wData)
{
    int indexOfAutomator = -1;

    for ( int i = 0 ; i < xyAutomators.size() ; i++ )
    {
        if (xyPad->getName() == xyAutomators[i]->getName())
            indexOfAutomator = i;
    }

    if (indexOfAutomator == -1)
    {
        XYPadAutomator* xyAuto;
        CabbageAudioParameter* xParameter = getParameterForXYPad (xyPad->getName() + "_x");
        CabbageAudioParameter* yParameter = getParameterForXYPad (xyPad->getName() + "_y");

        if (xParameter && yParameter)
        {
            xyAutomators.add (xyAuto = new XYPadAutomator (xyPad->getName(), xParameter, yParameter, this));
            xyAuto->setXMin (CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::minx));
            xyAuto->setYMin (CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::miny));
            xyAuto->setXMax (CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::maxx));
            xyAuto->setYMax (CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::maxy));
            xyAuto->addChangeListener (xyPad);
        }
    }
    else
    {
        xyAutomators[indexOfAutomator]->addChangeListener (xyPad);
    }
}


void CabbagePluginProcessor::enableXYAutomator (String name, bool enable, Line<float> dragLine)
{

    for ( XYPadAutomator* xyAuto : xyAutomators)
    {
        if (name == xyAuto->getName())
        {
            if (enable == true)
            {
                xyAuto->setDragLine (dragLine);
                xyAuto->setXValue (dragLine.getEndX());
                xyAuto->setYValue (dragLine.getEndY());
                xyAuto->setXValueIncrement ((dragLine.getEndX() - dragLine.getStartX())*.05);
                xyAuto->setYValueIncrement ((dragLine.getEndY() - dragLine.getStartY())*.05);
                xyAuto->setRepaintBackground (true);
                xyAuto->setIsPluginEditorOpen (getActiveEditor() != nullptr ? true : false);
                xyAuto->startTimer (20);
            }
            else
                xyAuto->stopTimer();
        }
    }
}

//======================================================================================================
CabbageAudioParameter* CabbagePluginProcessor::getParameterForXYPad (String name)
{
    for (auto param : getParameters())
    {
        if (CabbageAudioParameter* cabbageParam = dynamic_cast<CabbageAudioParameter*> (param))
        {
            if (name == cabbageParam->getWidgetName())
                return dynamic_cast<CabbageAudioParameter*> (cabbageParam);

        }
    }

    return nullptr;
}











