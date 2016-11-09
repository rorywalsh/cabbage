/*
  ==============================================================================

    GenericCabbagePluginProcessor.h
    Created: 21 Oct 2016 10:50:44am
    Author:  rory

  ==============================================================================
*/



#ifndef GENERICCABBAGEPLUGINPROCESSOR_H_INCLUDED
#define GENERICCABBAGEPLUGINPROCESSOR_H_INCLUDED

#include "CsoundPluginProcessor.h"

class GenericCabbagePluginProcessor  : public CsoundPluginProcessor
{
public:
	GenericCabbagePluginProcessor(File inputFile);
	~GenericCabbagePluginProcessor();	
	
	controlChannelInfo_s* csoundChanList;
	
	OwnedArray<AudioParameterFloat> parameters;
	
	void sendChannelDataToCsound();

	//==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;
};


#endif  // GENERICCABBAGEPLUGINPROCESSOR_H_INCLUDED
