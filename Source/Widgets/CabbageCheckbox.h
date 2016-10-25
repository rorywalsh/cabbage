/*
  ==============================================================================

    CabbageCheckbox.h
    Created: 25 Oct 2016 9:20:01pm
    Author:  rory

  ==============================================================================
*/

#ifndef CABBAGECHECKBOX_H_INCLUDED
#define CABBAGECHECKBOX_H_INCLUDED

#include "../CabbageCommonHeaders.h"
//==============================================================================
// custom checkbox component with optional surrounding groupbox
//==============================================================================
class CabbageCheckbox : public Component, public ValueTree::Listener
{
    int offX, offY, offWidth, offHeight, pivotx, pivoty, corners;
    float rotate;

    bool isRect;
    String name, caption, tooltipText, buttonText, colour, fontcolour, oncolour;

public:

    CabbageCheckbox(ValueTree widgetData);
    ~CabbageCheckbox(){};
    void valueTreePropertyChanged (ValueTree& valueTree, const Identifier&);
    void valueTreeChildAdded (ValueTree&, ValueTree&)override {};
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override {};
    void resized();

	GroupComponent groupbox;
    ToggleButton button;
	
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabbageCheckbox);
};


#endif  // CABBAGECHECKBOX_H_INCLUDED
