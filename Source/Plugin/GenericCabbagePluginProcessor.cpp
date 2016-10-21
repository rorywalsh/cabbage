/*
  ==============================================================================

    GenericCabbagePluginProcessor.cpp
    Created: 21 Oct 2016 10:50:44am
    Author:  rory

  ==============================================================================
*/

#include "GenericCabbagePluginProcessor.h"
#include "GenericCabbageEditor.h"


GenericCabbagePluginProcessor::GenericCabbagePluginProcessor(File intputFile)
:CsoundPluginProcessor(intputFile)
{	
	
	csoundChanList = NULL;
	int numberOfChannels = csoundListChannels(getCsoundStruct(), &csoundChanList);
	for(int i = 0; i < numberOfChannels; i++ )
	{
		const float min = csoundChanList[i].hints.min;
		const float max = csoundChanList[i].hints.max;
		const float defaultValue = csoundChanList[i].hints.dflt;
		const String channel = csoundChanList[i].name;
		NormalisableRange<float> range(min, max);
		AudioParameterFloat* param = new AudioParameterFloat(channel, channel, range, defaultValue);
		addParameter(param);
		parameters.add(param);		
	}
	
	CabbageUtilities::debug(getParameters().size());
	
}

GenericCabbagePluginProcessor::~GenericCabbagePluginProcessor()
{
	getCsound()->DeleteChannelList(csoundChanList);
	editorBeingDeleted (this->getActiveEditor());
}

//==============================================================================
bool GenericCabbagePluginProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* GenericCabbagePluginProcessor::createEditor()
{
    return new GenericCabbageEditor(*this);
}

void GenericCabbagePluginProcessor::sendChannelDataToCsound()
{
	for(int i=0;i<parameters.size();i++)
		getCsound()->SetChannel(parameters[i]->name.toUTF8(), *parameters[i]);
}

