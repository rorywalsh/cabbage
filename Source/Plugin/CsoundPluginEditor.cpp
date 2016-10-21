/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "CsoundPluginProcessor.h"
#include "CsoundPluginEditor.h"


//==============================================================================
CsoundPluginEditor::CsoundPluginEditor (CsoundPluginProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
}

CsoundPluginEditor::~CsoundPluginEditor()
{
}

//==============================================================================
void CsoundPluginEditor::paint (Graphics& g)
{
    g.fillAll (Colours::white);

}

void CsoundPluginEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
