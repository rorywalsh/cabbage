/*
  ==============================================================================

    CabbagePluginProcessor.cpp
    Created: 24 Oct 2016 6:38:24pm
    Author:  rory

  ==============================================================================
*/

#include "CabbagePluginProcessor.h"
#include "CabbagePluginEditor.h"


char tmp_string[4096] = {0};
char channelMessage[4096] = {0};


CabbagePluginProcessor::CabbagePluginProcessor(File inputFile)
:CsoundPluginProcessor(inputFile), 
csdFile(inputFile),
cabbageWidgets("CabbageWidgetData")
{	
	
	if(inputFile.existsAsFile())
	{
		parseCsdFile();
		createParameters();
	}

}

CabbagePluginProcessor::~CabbagePluginProcessor()
{

}

//==============================================================================
void CabbagePluginProcessor::parseCsdFile()
{
	StringArray linesFromCsd;
	linesFromCsd.addLines(csdFile.loadFileAsString());
	
	for( int lineNumber = 0; lineNumber < linesFromCsd.size() ; lineNumber++ )
	{
		if(linesFromCsd[lineNumber].equalsIgnoreCase("</Cabbage>"))
			lineNumber = linesFromCsd.size()+1;
		
		const String widgetTreeIdentifier = "WidgetFromLine_"+String(lineNumber);
		ValueTree temp(widgetTreeIdentifier);
		CabbageWidgetData::setWidgetState(temp, linesFromCsd[lineNumber], lineNumber);
		
		if(CabbageWidgetData::getProperty(temp, CabbageIdentifierIds::basetype).toString()=="interactive" ||
			CabbageWidgetData::getProperty(temp, CabbageIdentifierIds::basetype).toString()=="layout" )
		{
			cabbageWidgets.addChild(temp, -1, 0);
		}
		
	}
	
	CabbageUtilities::writeValueTreeToFile(cabbageWidgets, "/home/rory/Desktop/test.xml");

}

//==============================================================================
void CabbagePluginProcessor::createParameters()
{
	for(int i = 0; i < cabbageWidgets.getNumChildren(); i++)
	{
		const String typeOfWidget = CabbageWidgetData::getStringProp(cabbageWidgets.getChild(i), CabbageIdentifierIds::type);
		CabbageControlWidgetStrings controlWidgetTypes;
		if(controlWidgetTypes.contains(typeOfWidget))
		{
			const String name = CabbageWidgetData::getStringProp(cabbageWidgets.getChild(i), CabbageIdentifierIds::name);
			const String channel = CabbageWidgetData::getStringProp(cabbageWidgets.getChild(i), CabbageIdentifierIds::channel);
			const int value = CabbageWidgetData::getNumProp(cabbageWidgets.getChild(i), CabbageIdentifierIds::value);
			
			if(typeOfWidget==CabbageIdentifierIds::checkbox)
				addParameter(new CabbageAudioParameter(*getCsound(), channel, name, 0, 1, value));	
		}
	}	
}

//==============================================================================
bool CabbagePluginProcessor::hasEditor() const	
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* CabbagePluginProcessor::createEditor()
{
    return new CabbagePluginEditor(*this);
}

//==============================================================================
void CabbagePluginProcessor::receiveChannelDataFromCsound()
{

	for( int i = 0; i < cabbageWidgets.getNumChildren(); i++)
	{
		const String channel = CabbageWidgetData::getStringProp(cabbageWidgets.getChild(i), CabbageIdentifierIds::channel);
		const float value = CabbageWidgetData::getNumProp(cabbageWidgets.getChild(i), CabbageIdentifierIds::value);
		const String identChannel = CabbageWidgetData::getStringProp(cabbageWidgets.getChild(i), CabbageIdentifierIds::identchannel);
		const String identChannelMessage = CabbageWidgetData::getStringProp(cabbageWidgets.getChild(i), CabbageIdentifierIds::identchannelmessage);
		
		if(getCsound()->GetChannel(channel.toUTF8())!=value)
			CabbageWidgetData::setNumProp(cabbageWidgets.getChild(i), CabbageIdentifierIds::value, getCsound()->GetChannel(channel.toUTF8()));			
		
		if(identChannel.isNotEmpty())
		{	
			getCsound()->GetStringChannel(identChannel.toUTF8(), tmp_string);
			if(String(tmp_string)!=identChannelMessage)
			{
				CabbageWidgetData::setCustomWidgetState(cabbageWidgets.getChild(i), " "+String(tmp_string));
			}	
		}
				
	}
}

