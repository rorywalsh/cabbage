/*
  ==============================================================================

    CabbageCombobox.h
    Created: 1 Nov 2016 12:06:26pm
    Author:  rory

  ==============================================================================
*/

#ifndef CABBAGECOMBOBOX_H_INCLUDED
#define CABBAGECOMBOBOX_H_INCLUDED

#include "../CabbageCommonHeaders.h"
#include "CabbageCustomWidgets.h"

class CabbagePluginEditor;
class CabbageComboBox : public ComboBox, public ValueTree::Listener, public CabbageWidgetBase
{
    int offX, offY, offWidth, offHeight, pivotx, pivoty, refresh;
    String name, tooltipText, caption, text, colour, fontcolour, filetype;
    float rotate;
    File pluginDir;
    CabbagePluginEditor* owner;
	ValueTree widgetData;
	bool isPresetCombo = false;

public:

    CabbageComboBox(ValueTree &cAttr, CabbagePluginEditor* _owner);
    ~CabbageComboBox();

	void valueTreePropertyChanged (ValueTree& valueTree, const Identifier&);
    void valueTreeChildAdded (ValueTree&, ValueTree&)override {};
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override {};


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabbageComboBox);
};

#endif  // CABBAGECOMBOBOX_H_INCLUDED
