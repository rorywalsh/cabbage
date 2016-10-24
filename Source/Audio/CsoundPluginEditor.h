/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "CsoundPluginProcessor.h"


//==============================================================================
/**
*/
class CsoundPluginEditor  : public AudioProcessorEditor
{
public:
    CsoundPluginEditor (CsoundPluginProcessor&);
    ~CsoundPluginEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:

    CsoundPluginProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CsoundPluginEditor)
};


#endif  // PLUGINEDITOR_H_INCLUDED
