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
    String pluginName;
public:
    GenericCabbagePluginProcessor (File inputFile, const BusesProperties ioBuses);
    ~GenericCabbagePluginProcessor();

    controlChannelInfo_s* csoundChanList;

    OwnedArray<AudioParameterFloat> parameters;

    void sendChannelDataToCsound() override;
    void setPluginName (String name) {    pluginName = name;  }
    String getPluginName() { return pluginName;  }

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;
};


#endif  // GENERICCABBAGEPLUGINPROCESSOR_H_INCLUDED
