/*
  ==============================================================================

    CabbagePluginEditor.h
    Created: 24 Oct 2016 6:38:36pm
    Author:  rory

  ==============================================================================
*/

#ifndef CABBAGEPLUGINEDITOR_H_INCLUDED
#define CABBAGEPLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "CabbagePluginProcessor.h"

class CabbagePluginEditor  : public AudioProcessorEditor
{
public:
    CabbagePluginEditor (CabbagePluginProcessor&);
    ~CabbagePluginEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:

    CabbagePluginProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabbagePluginEditor)
};


#endif  // CABBAGEPLUGINEDITOR_H_INCLUDED
