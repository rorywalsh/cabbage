/*
  ==============================================================================

    CabbageCustomWidgets.h
    Created: 28 Oct 2016 8:23:33am
    Author:  rory

  ==============================================================================
*/

#ifndef CABBAGECUSTOMWIDGETS_H_INCLUDED
#define CABBAGECUSTOMWIDGETS_H_INCLUDED

#include "../CabbageCommonHeaders.h"

// Simple base class for taking care of some widget housekeeping. This class looks after
// common memeber variables such as alpha values, tooltiptext, bounds, etc 
// Each cabbage widget should inherit from this class so initialiseCommonAttributes()
// can be called in its valueTreePropertyChanged() method.  
class CabbageWidgetBase
{
public:
	CabbageWidgetBase(){}
	~CabbageWidgetBase(){}

	int pivotx, pivoty, visible, active, value;
    float rotate, alpha, currentValue;
	String tooltipText, text, channel;
	
	void initialiseCommonAttributes(ValueTree valueTree)
	{
		rotate = CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::rotate);
		pivotx = CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::pivotx);
		pivoty = CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::pivoty);
		visible = CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::visible);
		active = CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::active);
		tooltipText = CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::popuptext);				
	}
	
	void handleCommonUpdates(Component* child, ValueTree data);
	String getTooltipText(ValueTree data);
	String getText(ValueTree data);
	void setChannel(ValueTree value);
	float getValue(ValueTree data);

};

// Add any new custom widgets here to avoid having to edit makefiles and projects
// Each Cabbage widget should inherit from ValueTree listener, and CabbageWidgetBase
class DemoCabbageWidget : public Component, public ValueTree::Listener, public CabbageWidgetBase
{
public:

    DemoCabbageWidget(ValueTree widgetData){};
    ~DemoCabbageWidget(){};
	
	//VlaueTree::Listener virtual methods....
    void valueTreePropertyChanged (ValueTree& valueTree, const Identifier&){};
    void valueTreeChildAdded (ValueTree&, ValueTree&)override {};
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override {};
    void resized();

	ValueTree widgetData;

	
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DemoCabbageWidget);
};


#endif  // CABBAGECUSTOMWIDGETS_H_INCLUDED
