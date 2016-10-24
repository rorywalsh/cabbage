/*
  ==============================================================================

    CabbagePluginEditor.cpp
    Created: 24 Oct 2016 6:38:36pm
    Author:  rory

  ==============================================================================
*/

#include "CabbagePluginEditor.h"


//==============================================================================
CabbagePluginEditor::CabbagePluginEditor (CabbagePluginProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
}

CabbagePluginEditor::~CabbagePluginEditor()
{
}

//==============================================================================
void CabbagePluginEditor::paint (Graphics& g)
{
    g.fillAll (Colours::white);
}

void CabbagePluginEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
