/*
  ==============================================================================

    GenericCabbagePluginProcessor.h
    Created: 21 Oct 2016 10:50:44am
    Author:  rory

  ==============================================================================
*/

#include "CsoundPluginProcessor.h"

#ifndef GENERICCABBAGEPLUGINPROCESSOR_H_INCLUDED
#define GENERICCABBAGEPLUGINPROCESSOR_H_INCLUDED


class GenericCabbagePluginProcessor  : public CsoundPluginProcessor
{
public:
	GenericCabbagePluginProcessor(File inputFile);
	~GenericCabbagePluginProcessor();	
	
	controlChannelInfo_s* csoundChanList;
};


#endif  // GENERICCABBAGEPLUGINPROCESSOR_H_INCLUDED
