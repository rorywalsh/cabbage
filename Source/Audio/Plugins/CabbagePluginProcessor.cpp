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

#ifndef Cabbage_IDE_Build
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    const File csdFile = File::getSpecialLocation (File::currentExecutableFile).withFileExtension (String (".csd")).getFullPathName();

    if (!csdFile.existsAsFile())
    {
        CabbageUtilities::debug (csdFile.getFullPathName());
    }

    return new CabbagePluginProcessor (csdFile);
};
#endif

//============================================================================
CabbagePluginProcessor::CabbagePluginProcessor (File inputFile)
    : CsoundPluginProcessor (inputFile),
      csdFile (inputFile),
      cabbageWidgets ("CabbageWidgetData")
{

    //initAllCsoundChannels(cabbageWidgets);
    if (inputFile.existsAsFile())
    {
        parseCsdFile (inputFile.loadFileAsString());
        createParameters();
    }

    initAllCsoundChannels (cabbageWidgets);
}

CabbagePluginProcessor::~CabbagePluginProcessor()
{
    xyAutomator.clear();
}

//==============================================================================
void CabbagePluginProcessor::parseCsdFile (String csdText)
{
    StringArray linesFromCsd;
    cabbageWidgets.removeAllChildren (0);
    linesFromCsd.addLines (csdText);
    String parentComponent, previousComponent;

    searchForMacros (linesFromCsd);

    for ( int lineNumber = 0; lineNumber < linesFromCsd.size() ; lineNumber++ )
    {
        if (linesFromCsd[lineNumber].equalsIgnoreCase ("</Cabbage>"))
            return;

        const String widgetTreeIdentifier = "WidgetFromLine_" + String (lineNumber);
        ValueTree tempWidget (widgetTreeIdentifier);

        String currentLineOfCabbageCode = linesFromCsd[lineNumber];

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
        CabbageWidgetData::setWidgetState (tempWidget, currentLineOfCabbageCode + " " + expandedMacroText, lineNumber);
        CabbageWidgetData::setNumProp (tempWidget, CabbageIdentifierIds::linenumber, lineNumber);
        CabbageWidgetData::setStringProp (tempWidget, CabbageIdentifierIds::csdfile, csdFile.getFullPathName());
        //CabbageUtilities::debug(CabbageWidgetData::getStringProp(temp, CabbageIdentifierIds::csdfile));
        CabbageWidgetData::setStringProp (tempWidget, CabbageIdentifierIds::expandedmacrotext, expandedMacroText);
		
		if(CabbageWidgetData::getStringProp(tempWidget, CabbageIdentifierIds::type) == CabbageIdentifierIds::form)
		{
			const String caption = CabbageWidgetData::getStringProp(tempWidget, CabbageIdentifierIds::caption);
			this->setPluginName(caption.length()>0 ? caption : "Untitled");
		}
		
        if (currentLineOfCabbageCode.contains ("}"))
        {
            parentComponent = "";
        }

        if (parentComponent.isNotEmpty())
            CabbageWidgetData::setStringProp (tempWidget, CabbageIdentifierIds::parentcomponent, parentComponent);

        if (CabbageWidgetData::getProperty (tempWidget, CabbageIdentifierIds::basetype).toString() == "interactive" ||
            CabbageWidgetData::getProperty (tempWidget, CabbageIdentifierIds::basetype).toString() == "layout" )
        {
            cabbageWidgets.addChild (tempWidget, -1, 0);

            if (CabbageWidgetData::getProperty (tempWidget, CabbageIdentifierIds::widgetarray).size() > 0 &&
                CabbageWidgetData::getProperty (tempWidget, CabbageIdentifierIds::identchannelarray).size() > 0)
            {
                for (int i = 0; i < CabbageWidgetData::getProperty (tempWidget, CabbageIdentifierIds::widgetarray).size(); i++)
                {
                    ValueTree copy = tempWidget.createCopy();
                    CabbageWidgetData::setStringProp (copy, CabbageIdentifierIds::channel, CabbageWidgetData::getProperty (tempWidget, CabbageIdentifierIds::widgetarray)[i]);
                    CabbageWidgetData::setStringProp (copy, CabbageIdentifierIds::identchannel, CabbageWidgetData::getProperty (tempWidget, CabbageIdentifierIds::identchannelarray)[i]);
                    cabbageWidgets.addChild (copy, -1, 0);
                }
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
                macroText.set ("$" + tokens[1], " " + csdLine.substring (csdLine.indexOf (tokens[1]) + tokens[1].length()) + " ");
            }
        }
    }
}

const String CabbagePluginProcessor::getExpandedMacroText (const String line, ValueTree wData)
{
    String csdLine;
    var macroNames;

    for (int cnt = 0 ; cnt < macroText.size() ; cnt++)
    {
        if (line.contains (macroText.getName (cnt).toString()))
        {
            csdLine += macroText.getWithDefault (macroText.getName (cnt), "").toString() + " ";
            macroNames.append (macroText.getName (cnt).toString());
        }
    }

    CabbageWidgetData::setProperty (wData, CabbageIdentifierIds::macronames, macroNames);

    return csdLine;;
}

//right now we rebuild the entire GUi each time something changes,
void CabbagePluginProcessor::updateWidgets (String csdText)
{
    CabbagePluginEditor* editor = static_cast<CabbagePluginEditor*> (this->getActiveEditor());
    parseCsdFile (csdText);
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
            const int value = CabbageWidgetData::getNumProp (cabbageWidgets.getChild (i), CabbageIdentifierIds::value);

            if (controlWidgetTypes.contains (CabbageWidgetData::getStringProp (cabbageWidgets.getChild (i), CabbageIdentifierIds::type)))
            {

                if (typeOfWidget == CabbageIdentifierIds::xypad)
                {
                    const var channel = CabbageWidgetData::getProperty (cabbageWidgets.getChild (i), CabbageIdentifierIds::channel);
                    addParameter (new CabbageAudioParameter (cabbageWidgets.getChild (i), *getCsound(), channel[0] , name + "_x", 0, 1, value));
                    addParameter (new CabbageAudioParameter (cabbageWidgets.getChild (i), *getCsound(), channel[1], name + "_y", 0, 1, value));
                }

                else if (typeOfWidget.contains ("range"))
                {
                    const var channel = CabbageWidgetData::getProperty (cabbageWidgets.getChild (i), CabbageIdentifierIds::channel);
                    const int minValue = CabbageWidgetData::getNumProp (cabbageWidgets.getChild (i), CabbageIdentifierIds::minvalue);
                    const int maxValue = CabbageWidgetData::getNumProp (cabbageWidgets.getChild (i), CabbageIdentifierIds::maxvalue);
                    addParameter (new CabbageAudioParameter (cabbageWidgets.getChild (i), *getCsound(), channel[0], name + "_min", 0, 1, minValue));
                    addParameter (new CabbageAudioParameter (cabbageWidgets.getChild (i), *getCsound(), channel[1], name + "_max", 0, 1, maxValue));
                }
                else
                {
                    addParameter (new CabbageAudioParameter (cabbageWidgets.getChild (i), *getCsound(), channel, name, 0, 1, value));
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
    copyXmlToBinary (savePluginState ("CABBAGE_PLUGIN_SETTINGS"), destData);
}

void CabbagePluginProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    ScopedPointer<XmlElement> xmlElement = getXmlFromBinary (data, sizeInBytes);
    restorePluginState (xmlElement);
}

//==============================================================================
XmlElement CabbagePluginProcessor::savePluginState (String xmlTag)
{
    XmlElement xml (xmlTag);

    for (int i = 0 ; i < cabbageWidgets.getNumChildren() ; i++)
    {
        const String widgetName = CabbageWidgetData::getStringProp (cabbageWidgets.getChild (i), CabbageIdentifierIds::name);
        const String type = CabbageWidgetData::getStringProp (cabbageWidgets.getChild (i), CabbageIdentifierIds::type);

        const float value = CabbageWidgetData::getNumProp (cabbageWidgets.getChild (i), CabbageIdentifierIds::value);

        if (type == CabbageIdentifierIds::texteditor)
        {
            const String text = CabbageWidgetData::getStringProp (cabbageWidgets.getChild (i), CabbageIdentifierIds::text);
            xml.setAttribute (widgetName, text);
        }
        else if (type == CabbageIdentifierIds::filebutton)
        {
            const String file = CabbageWidgetData::getStringProp (cabbageWidgets.getChild (i), CabbageIdentifierIds::file);
            xml.setAttribute (widgetName, file);
        }
        else
            xml.setAttribute (widgetName, value);
    }

    return xml;
}

void CabbagePluginProcessor::restorePluginState (XmlElement* xmlState)
{
    if (xmlState != nullptr)
    {
        for (int i = 0; i < xmlState->getNumAttributes(); i++)
        {
            ValueTree valueTree = CabbageWidgetData::getValueTreeForComponent (cabbageWidgets, xmlState->getAttributeName (i));
            const String type = CabbageWidgetData::getStringProp (cabbageWidgets.getChild (i), CabbageIdentifierIds::type);

            if (type == CabbageIdentifierIds::texteditor)
                CabbageWidgetData::setStringProp (valueTree, CabbageIdentifierIds::text, xmlState->getAttributeValue (i));
            else if (type == CabbageIdentifierIds::filebutton)
            {
                CabbageWidgetData::setStringProp (valueTree, CabbageIdentifierIds::file, xmlState->getAttributeValue (i));
            }
            else
            {
                CabbageWidgetData::setNumProp (valueTree, CabbageIdentifierIds::value, xmlState->getAttributeValue (i).getFloatValue());
            }
        }

        initAllCsoundChannels (cabbageWidgets);
    }

    xmlState = nullptr;
}

//==============================================================================
void CabbagePluginProcessor::receiveChannelDataFromCsound()
{
    for ( int i = 0; i < cabbageWidgets.getNumChildren(); i++)
    {
        const String channel = CabbageWidgetData::getStringProp (cabbageWidgets.getChild (i), CabbageIdentifierIds::channel);
        const float value = CabbageWidgetData::getNumProp (cabbageWidgets.getChild (i), CabbageIdentifierIds::value);
        const String identChannel = CabbageWidgetData::getStringProp (cabbageWidgets.getChild (i), CabbageIdentifierIds::identchannel);
        const String identChannelMessage = CabbageWidgetData::getStringProp (cabbageWidgets.getChild (i), CabbageIdentifierIds::identchannelmessage);

        if (getCsound()->GetChannel (channel.toUTF8()) != value)
            CabbageWidgetData::setNumProp (cabbageWidgets.getChild (i), CabbageIdentifierIds::value, getCsound()->GetChannel (channel.toUTF8()));

        if (identChannel.isNotEmpty())
        {
            getCsound()->GetStringChannel (identChannel.toUTF8(), tmp_string);
            String identifierText (tmp_string);

            if (identifierText != identChannelMessage)
            {
                CabbageWidgetData::setCustomWidgetState (cabbageWidgets.getChild (i), " " + String (tmp_string));

                if (identifierText.contains ("tablenumber")) //update even if table number has not changed
                    CabbageWidgetData::setProperty (cabbageWidgets.getChild (i), CabbageIdentifierIds::update, 1);

                getCsound()->SetChannel (identChannel.toUTF8(), (char*)"");
            }
        }
    }
}

//================================================================================
void CabbagePluginProcessor::addXYAutomator (CabbageXYPad* xyPad, ValueTree wData)
{
    int indexOfAutomator = -1;

    for ( int i = 0 ; i < xyAutomator.size() ; i++ )
    {
        if (xyPad->getName() == xyAutomator[i]->getName())
            indexOfAutomator = i;
    }

    if (indexOfAutomator == -1)
    {
        XYPadAutomator* xyAuto;
        CabbageAudioParameter* xParameter = getParameterForXYPad (xyPad->getName() + "_x");
        CabbageAudioParameter* yParameter = getParameterForXYPad (xyPad->getName() + "_y");

        if (xParameter && yParameter)
        {
            xyAutomator.add (xyAuto = new XYPadAutomator (xyPad->getName(), xParameter, yParameter));
            xyAuto->setXMin (CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::minx));
            xyAuto->setYMin (CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::miny));
            xyAuto->setXMax (CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::maxx));
            xyAuto->setYMax (CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::maxy));
            xyAuto->addChangeListener (xyPad);
        }
    }
    else
    {
        xyAutomator[indexOfAutomator]->addChangeListener (xyPad);
    }
}

void CabbagePluginProcessor::enableXYAutomator (String name, bool enable, Line<float> dragLine)
{

    for ( XYPadAutomator* xyAuto : xyAutomator)
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











