/*
  ==============================================================================

    CabbagePluginProcessor.cpp
    Created: 24 Oct 2016 6:38:24pm
    Author:  rory

  ==============================================================================
*/

#include "CabbagePluginProcessor.h"
#include "CabbagePluginEditor.h"

CabbagePluginProcessor::CabbagePluginProcessor(File inputFile)
:CsoundPluginProcessor(inputFile), 
csdFile(inputFile),
cabbageWidgets("CabbageWidgetData")
{	
	
	if(inputFile.existsAsFile())
		parseCsdFile();

}

CabbagePluginProcessor::~CabbagePluginProcessor()
{

}

void CabbagePluginProcessor::parseCsdFile()
{
	StringArray linesFromCsd;
	linesFromCsd.addLines(csdFile.loadFileAsString());
	
	for( int lineNumber = 0; lineNumber < linesFromCsd.size() ; lineNumber++ )
	{
		if(linesFromCsd[lineNumber].equalsIgnoreCase("</Cabbage>"))
			lineNumber = linesFromCsd.size()+1;
		
		CabbageWidget singleWidget(linesFromCsd[lineNumber], lineNumber);
		if(singleWidget.getProperty(CabbageIdentifierIds::basetype).toString().isNotEmpty())
			cabbageWidgets.addChild(singleWidget, -1, 0);			
	}
	


	CabbageUtilities::writeValueTreeToFile(cabbageWidgets, "/home/rory/Desktop/test.xml");	
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

void CabbagePluginProcessor::sendChannelDataToCsound()
{
	const OwnedArray<AudioProcessorParameter>& params = getParameters();
	for(int i=0;i<params.size();i++)
	{
		AudioParameterFloat* param = dynamic_cast<AudioParameterFloat*> (params[i]);
		getCsound()->SetChannel(param->name.toUTF8(), *param);
	}
}

