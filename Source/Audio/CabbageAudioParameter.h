/*
  ==============================================================================

    AudioPramaters.h
    Created: 26 Oct 2016 10:43:15am
    Author:  rory

  ==============================================================================
*/

#ifndef AUDIOPRAMATERS_H_INCLUDED
#define AUDIOPRAMATERS_H_INCLUDED

#include "../CabbageCommonHeaders.h"
#include "CabbagePluginProcessor.h"

class CabbageAudioParameter : public AudioParameterFloat
{
	
public:
	CabbageAudioParameter(Csound &csound, String channel, String name, float minValue, float maxValue, float def)
	:AudioParameterFloat(channel, name, minValue, maxValue, def), name(name), channel(channel), csound(csound)
	{}
	~CabbageAudioParameter(){}
	
	float getValue() const override
	{

	}
	
	void setValue (float newValue) override
	{
		csound.SetChannel(channel.toUTF8(), newValue);
	}

	String channel, name;
	Csound csound;
};

#endif  // AUDIOPRAMATERS_H_INCLUDED
