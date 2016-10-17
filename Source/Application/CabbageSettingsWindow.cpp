/*
  ==============================================================================

    CabbageSettingsWindow.cpp
    Created: 14 Oct 2016 4:12:45pm
    Author:  rory

  ==============================================================================
*/

#include "CabbageSettingsWindow.h"

CabbageSettingsWindow::CabbageSettingsWindow(ValueTree tree): Component(""), valueTree(tree)
{
    addColourProperties();
    addAndMakeVisible (panel);
}

void CabbageSettingsWindow::addColourProperties()
{
    Array<PropertyComponent*> props;

    for (int index = 0; index < valueTree.getChildWithName("Colours").getNumProperties(); ++index)
        props.add (new ColourPropertyComponent(valueTree, index));

    panel.clear();
    panel.addProperties (props);
}