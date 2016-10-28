/*
  ==============================================================================

    CabbagePropertiesComponent.cpp
    Created: 27 Oct 2016 10:08:05am
    Author:  rory

  ==============================================================================
*/

#include "CabbagePropertiesPanel.h"

//==============================================================================
static void addListener(Array<PropertyComponent*> comps, CabbagePropertiesPanel* owner)
{
	for( int i = 0; i < comps.size(); i++)
	{
		if(TextPropertyComponent* textProperty = dynamic_cast<TextPropertyComponent*>(comps[i]))
		{
			textProperty->addListener(owner);
		}			
	}
}
//==============================================================================
static Array<PropertyComponent*> createPositionEditors(CabbagePropertiesPanel* owner, ValueTree valueTree)
{
    Array<PropertyComponent*> comps;
	Rectangle<int> bounds = CabbageWidget::getBounds(valueTree);
    comps.add (new TextPropertyComponent(Value (var (bounds.getX())), "X Position", 200, false));
	comps.add (new TextPropertyComponent(Value (var (bounds.getY())), "Y Position", 200, false));
	comps.add (new TextPropertyComponent(Value (var (bounds.getWidth())), "Width", 200, false));
	comps.add (new TextPropertyComponent(Value (var (bounds.getHeight())), "Height", 200, false));
	addListener(comps, owner);
    return comps;
}

//==============================================================================
static Array<PropertyComponent*> createValueEditors(ValueTree valueTree)
{
    Array<PropertyComponent*> comps;
	
	const String typeOfWidget = CabbageWidget::getStringProp(valueTree, CabbageIdentifierIds::type);
	if(typeOfWidget.contains("slider"))
	{
		const float min = CabbageWidget::getNumProp(valueTree, CabbageIdentifierIds::min);
		const float max = CabbageWidget::getNumProp(valueTree, CabbageIdentifierIds::max);
		const float skew = CabbageWidget::getNumProp(valueTree, CabbageIdentifierIds::sliderskew);
		const float incr = CabbageWidget::getNumProp(valueTree, CabbageIdentifierIds::sliderincr);
		comps.add(new TextPropertyComponent(Value (var (min)), "Min", 200, false));
		comps.add(new TextPropertyComponent(Value (var (max)), "Max", 200, false));
		comps.add(new TextPropertyComponent(Value (var (skew)), "Skew", 200, false));
		comps.add(new TextPropertyComponent(Value (var (incr)), "Increment", 200, false));
		
	}
	
	const float value = CabbageWidget::getNumProp(valueTree, CabbageIdentifierIds::value);
	comps.add(new TextPropertyComponent(Value (var (value)), "Init. Value", 200, false));
	
	return comps;
}
//==============================================================================
static Array<PropertyComponent*> createChannelEditors(ValueTree valueTree)
{
    Array<PropertyComponent*> comps;
	const String channel = CabbageWidget::getStringProp(valueTree, CabbageIdentifierIds::channel);
	const String identChannel = CabbageWidget::getStringProp(valueTree, CabbageIdentifierIds::identchannel);
    comps.add (new TextPropertyComponent(Value (var (channel)), "Channel", 200, false));
	comps.add (new TextPropertyComponent(Value (var (identChannel)), "Ident Channel", 100, false));
	return comps;
}

//==============================================================================
static Array<PropertyComponent*> createColourChoosers (ValueTree valueTree)
{
    Array<PropertyComponent*> comps;
	
	comps.add (new ColourPropertyComponent("Test Colour", 0));
	return comps;
}

//==============================================================================
static Array<PropertyComponent*> createButtons (int howMany)
{
    Array<PropertyComponent*> comps;

    for (int i = 0; i < howMany; ++i)
        comps.add (new BooleanPropertyComponent (Value (Random::getSystemRandom().nextBool()), "Toggle " + String (i + 1), "Description of toggleable thing"));

    return comps;
}
//==============================================================================
static Array<PropertyComponent*> createChoices (int howMany)
{
    Array<PropertyComponent*> comps;

    StringArray choices;
    Array<var> choiceVars;

    for (int i = 0; i < howMany; ++i)
    {
        choices.add ("Item " + String (i));
        choiceVars.add (i);
    }

    for (int i = 0; i < howMany; ++i)
        comps.add (new ChoicePropertyComponent (Value (Random::getSystemRandom().nextInt (6)), "Choice Property " + String (i + 1), choices, choiceVars));
    return comps;
}

//==============================================================================
// Property Panel for editing widgets
//==============================================================================
CabbagePropertiesPanel::CabbagePropertiesPanel(ValueTree widgetData)
:widgetData(widgetData)
{
	setOpaque (true);
	setSize(300, 500);
	addAndMakeVisible (propertyPanel);
	propertyPanel.addSection ("Dimensions", createPositionEditors(this, widgetData));
	propertyPanel.addSection ("Channels", createChannelEditors(widgetData));
	propertyPanel.addSection ("Values", createValueEditors(widgetData));
	propertyPanel.addSection ("Colours", createColourChoosers(widgetData));

	//ropertyPanel.addSection ("Channels", createChoices (16));
	//propertyPanel.addSection ("Buttons & Toggles", createButtons (20));
}

void CabbagePropertiesPanel::paint (Graphics& g)
{
	g.fillAll (Colour::greyLevel (0.8f));
}

void CabbagePropertiesPanel::resized()
{
	propertyPanel.setBounds (getLocalBounds().reduced (4));
}

void CabbagePropertiesPanel::textPropertyComponentChanged(TextPropertyComponent *comp)
{
	//CabbageUtilities::debug(comp->getName());
	//ValueTree tree = CabbageWidget::getValueTreeForComponent(widgetData, comp->getName());
	CabbageWidget::setNumProp(widgetData, CabbageIdentifierIds::allowboundsupdate, 1);
	CabbageWidget::setNumProp(widgetData, CabbageIdentifierIds::left, comp->getText().getIntValue());
	CabbageWidget::setNumProp(widgetData, CabbageIdentifierIds::allowboundsupdate, 0);	
}
