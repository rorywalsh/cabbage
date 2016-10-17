/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "CsoundPluginProcessor.h"
#include "CsoundPluginEditor.h"


//==============================================================================
CsoundAudioProcessorEditor::CsoundAudioProcessorEditor (CsoundAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
}

CsoundAudioProcessorEditor::~CsoundAudioProcessorEditor()
{
}

//==============================================================================
void CsoundAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colours::white);

}

void CsoundAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
