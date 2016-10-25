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
		CabbageWidget::setWidgetState(temp, linesFromCsd[lineNumber], lineNumber);
		
		if(CabbageWidget::getProperty(temp, CabbageIdentifierIds::basetype).toString()=="interactive" ||
			CabbageWidget::getProperty(temp, CabbageIdentifierIds::basetype).toString()=="layout" )
		{
			cabbageWidgets.addChild(temp, -1, 0);
		}

		//if(singleWidget.getProperty(CabbageIdentifierIds::basetype).toString().isNotEmpty())
		//	cabbageWidgets.addChild(singleWidget.getValueTree(), -1, 0);			
	}
	
	
	CabbageUtilities::writeValueTreeToFile(cabbageWidgets, "/home/rory/Desktop/test.xml");
	//CabbagePluginEditor* editor = dynamic_cast<CabbagePluginEditor*>(this->createEditorIfNeeded());

	
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

