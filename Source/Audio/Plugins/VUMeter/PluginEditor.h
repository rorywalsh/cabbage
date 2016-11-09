/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================
/**
*/
class VumeterAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    VumeterAudioProcessorEditor (VumeterAudioProcessor&);
    ~VumeterAudioProcessorEditor();


	float leftChannelAmp, rightChannelAmp;
    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    VumeterAudioProcessor& processor;
	

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VumeterAudioProcessorEditor)
};


#endif  // PLUGINEDITOR_H_INCLUDED
