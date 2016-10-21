/*
  ==============================================================================

    GenericCabbagePluginProcessor.cpp
    Created: 21 Oct 2016 10:50:44am
    Author:  rory

  ==============================================================================
*/

#include "GenericCabbagePluginProcessor.h"


GenericCabbagePluginProcessor::GenericCabbagePluginProcessor(File intputFile)
:CsoundPluginProcessor(intputFile)
{	
	
	csoundChanList = NULL;
	int numberOfChannels = csoundListChannels(getCsoundStruct(), &csoundChanList);
	if(numberOfChannels>0)
	{
		CabbageUtilities::debug(csoundChanList[0].name);
		CabbageUtilities::debug(csoundChanList[0].hints.min);
		CabbageUtilities::debug(csoundChanList[0].hints.max);
		CabbageUtilities::debug(csoundChanList[0].hints.dflt);
	}
	
}

GenericCabbagePluginProcessor::~GenericCabbagePluginProcessor()
{
	getCsound()->DeleteChannelList(csoundChanList);
}