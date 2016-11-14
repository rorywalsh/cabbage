/*
  ==============================================================================

    CabbageImage.h
    Created: 14 Nov 2016 2:30:11pm
    Author:  rory

  ==============================================================================
*/

#ifndef CABBAGEIMAGE_H_INCLUDED
#define CABBAGEIMAGE_H_INCLUDED

#include "../CabbageCommonHeaders.h"
#include "CabbageWidgetBase.h"

class CabbagePluginEditor;

class CabbageImage : public Component, public ValueTree::Listener, public CabbageWidgetBase
{
    String name, tooltipText, shape;
	File imgFile;
	CabbagePluginEditor* owner;
	float corners;
	int lineThickness;
	ValueTree widgetData;
	Colour outlineColour, mainColour;

public:

    CabbageImage(ValueTree cAttr, CabbagePluginEditor* _owner);
    ~CabbageImage(){};

	void valueTreePropertyChanged (ValueTree& valueTree, const Identifier&);
	void paint(Graphics& g);
	
    void valueTreeChildAdded (ValueTree&, ValueTree&)override {}
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override {}

	String getTooltip()
	{
		return tooltipText;
	}
	
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabbageImage);
};




#endif  // CABBAGEIMAGE_H_INCLUDED
