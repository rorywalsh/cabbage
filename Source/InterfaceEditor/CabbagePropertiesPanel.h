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
class CabbagePropertiesPanel   : 
public Component, 
public ChangeBroadcaster, 
public TextPropertyComponent::Listener,
public ChangeListener
{
public:
    CabbagePropertiesPanel(ValueTree widgetData);
    void paint (Graphics& g) override;
    void resized() override;
	void setPropertyByName(ValueTree widgetData, String name, var value);
	void textPropertyComponentChanged (TextPropertyComponent * comp);
	void changeListenerCallback(juce::ChangeBroadcaster *source);
	void updateProperties(ValueTree widgetData);

private:
    PropertyPanel propertyPanel;
	ValueTree widgetData;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabbagePropertiesPanel)
};

#endif  // CABBAGEPROPERTIESCOMPONENT_H_INCLUDED
