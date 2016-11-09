/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
VumeterAudioProcessorEditor::VumeterAudioProcessorEditor (VumeterAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (100, 200);

}

VumeterAudioProcessorEditor::~VumeterAudioProcessorEditor()
{
}

//==============================================================================
void VumeterAudioProcessorEditor::paint (Graphics& g)
{
	g.fillAll(Colours::black);
	g.setColour(Colours::green);
	g.fillRect(Rectangle<float>(0.f, getHeight()-leftChannelAmp, 50.f, 400.f));
	g.fillRect(Rectangle<float>(50.f, getHeight()-rightChannelAmp, 50.f, 400.f));
}

void VumeterAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
