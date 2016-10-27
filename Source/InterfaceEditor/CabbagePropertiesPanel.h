/*
  ==============================================================================

    CabbagePropertiesComponent.h
    Created: 27 Oct 2016 10:08:05am
    Author:  rory

  ==============================================================================
*/

#ifndef CABBAGEPROPERTIESCOMPONENT_H_INCLUDED
#define CABBAGEPROPERTIESCOMPONENT_H_INCLUDED

#include "../CabbageCommonHeaders.h"
#include "CabbageColourProperty.h"

//==============================================================================
class CabbagePropertiesPanel   : public Component, public ChangeBroadcaster, public TextPropertyComponent::Listener
{
public:
    CabbagePropertiesPanel(ValueTree widgetData);
    void paint (Graphics& g) override;
    void resized() override;
	
	void textPropertyComponentChanged (TextPropertyComponent * comp)
	{
		CabbageUtilities::debug(comp->getName());
	}

private:
    PropertyPanel propertyPanel;
	ValueTree widgetData;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabbagePropertiesPanel)
};

#endif  // CABBAGEPROPERTIESCOMPONENT_H_INCLUDED
