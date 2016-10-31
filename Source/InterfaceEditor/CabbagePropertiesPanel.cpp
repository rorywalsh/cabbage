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
		else if(ColourPropertyComponent* colourProperty = dynamic_cast<ColourPropertyComponent*>(comps[i]))
		{
			colourProperty->addChangeListener(owner);
		}				
	}
}
//==============================================================================
static Array<PropertyComponent*> createRotationEditors(CabbagePropertiesPanel* owner, ValueTree valueTree)
{
    Array<PropertyComponent*> comps;

	const float rotate = CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::rotate);
	const float pivotx = CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::pivotx);
	const float pivoty = CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::pivoty);
	
	comps.add (new TextPropertyComponent(Value (var (pivotx)), "Pivot X", 200, false));
	comps.add (new TextPropertyComponent(Value (var (pivoty)), "Pivot Y", 200, false));
	comps.add (new TextPropertyComponent(Value (var (rotate)), "Rotate", 200, false));
	addListener(comps, owner);
    return comps;	
}
//==============================================================================
static Array<PropertyComponent*> createValueEditors(CabbagePropertiesPanel* owner, ValueTree valueTree)
{
    Array<PropertyComponent*> comps;
	
	const String typeOfWidget = CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::type);
	if(typeOfWidget.contains("slider"))
	{
		const float min = CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::min);
		const float max = CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::max);
		const float skew = CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::sliderskew);
		const float incr = CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::sliderincr);
		comps.add(new TextPropertyComponent(Value (var (min)), "Minimum", 200, false));
		comps.add(new TextPropertyComponent(Value (var (max)), "Maximum", 200, false));
		comps.add(new TextPropertyComponent(Value (var (skew)), "Skew", 200, false));
		comps.add(new TextPropertyComponent(Value (var (incr)), "Increment", 200, false));	
	}
	
	const float value = CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::value);
	comps.add(new TextPropertyComponent(Value (var (value)), "Value", 200, false));
	
	addListener(comps, owner);
	
	return comps;
}
//==============================================================================
static Array<PropertyComponent*> createChannelEditors(CabbagePropertiesPanel* owner, ValueTree valueTree)
{
    Array<PropertyComponent*> comps;
	const String channel = CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::channel);
	const String identChannel = CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::identchannel);
    comps.add (new TextPropertyComponent(Value (var (channel)), "Channel", 200, false));
	comps.add (new TextPropertyComponent(Value (var (identChannel)), "Ident Channel", 100, false));
	addListener(comps, owner);
	return comps;
}

//==============================================================================
static Array<PropertyComponent*> createTextEditors(CabbagePropertiesPanel* owner, ValueTree valueTree)
{
    Array<PropertyComponent*> comps;
	const String text = CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::text);
	const String popupText = CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::popuptext);
    comps.add (new TextPropertyComponent(Value (var (text)), "Text", 200, false));
	comps.add (new TextPropertyComponent(Value (var (popupText)), "popup Text", 100, false));
	addListener(comps, owner);
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
Array<PropertyComponent*> CabbagePropertiesPanel::createColourChoosers (ValueTree valueTree)
{
    Array<PropertyComponent*> comps;
	
	const String colourString = CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::colour);
	const String onColourString = CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::oncolour);
	const String onFontColourString = CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::onfontcolour);
	const String fontColourString = CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::fontcolour);
	comps.add(new ColourPropertyComponent("Colour: Off", colourString));
	comps.add(new ColourPropertyComponent("Colour: On", onColourString));
	comps.add(new ColourPropertyComponent("Font: Off", fontColourString));
	comps.add(new ColourPropertyComponent("Font: On", onFontColourString));

	alphaValue.setValue(CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::alpha));
	alphaValue.addListener(this);
	comps.add(new SliderPropertyComponent(alphaValue, "Alpha", 0, 1, .01, 1, 1));
	

	addListener(comps, this);
	return comps;
}
//==============================================================================
Array<PropertyComponent*> CabbagePropertiesPanel::createPositionEditors(ValueTree valueTree)
{
    Array<PropertyComponent*> comps;
	Rectangle<int> bounds = CabbageWidgetData::getBounds(valueTree);
    comps.add (new TextPropertyComponent(Value (var (bounds.getX())), "X Position", 200, false));
	comps.add (new TextPropertyComponent(Value (var (bounds.getY())), "Y Position", 200, false));
	comps.add (new TextPropertyComponent(Value (var (bounds.getWidth())), "Width", 200, false));
	comps.add (new TextPropertyComponent(Value (var (bounds.getHeight())), "Height", 200, false));
	
	isActiveValue.setValue(CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::active));
	isActiveValue.addListener(this);
	isVisibleValue.setValue(CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::visible));
	isVisibleValue.addListener(this);	
	
    comps.add (new BooleanPropertyComponent(isActiveValue, "Active", "Is Active"));
	comps.add (new BooleanPropertyComponent(isVisibleValue, "Visible", "Is Visible"));
	
	
	addListener(comps, this);
    return comps;
}
//==============================================================================
CabbagePropertiesPanel::CabbagePropertiesPanel(ValueTree widgetData)
:widgetData(widgetData)
{
	setOpaque (true);
	setSize(300, 500);
	
	addAndMakeVisible (propertyPanel);
	propertyPanel.addSection ("Bounds", createPositionEditors(widgetData));
	propertyPanel.addSection ("Rotation", createRotationEditors(this, widgetData));
	propertyPanel.addSection ("Channels", createChannelEditors(this, widgetData));
	propertyPanel.addSection ("Values", createValueEditors(this, widgetData));
	propertyPanel.addSection ("Text", createTextEditors(this, widgetData));
	propertyPanel.addSection ("Colours", createColourChoosers(widgetData));

	//ropertyPanel.addSection ("Channels", createChoices (16));
	//propertyPanel.addSection ("Buttons & Toggles", createButtons (20));
}

void CabbagePropertiesPanel::updateProperties(ValueTree wData)
{
	widgetData = wData;
	propertyPanel.clear();
	propertyPanel.addSection ("Bounds", createPositionEditors(widgetData));
	propertyPanel.addSection ("Rotation", createRotationEditors(this, widgetData), false);
	propertyPanel.addSection ("Channels", createChannelEditors(this, widgetData));
	propertyPanel.addSection ("Values", createValueEditors(this, widgetData));
	propertyPanel.addSection ("Text", createTextEditors(this, widgetData));
	propertyPanel.addSection ("Colours", createColourChoosers(widgetData));
	this->setVisible(true);
	
}
void CabbagePropertiesPanel::paint (Graphics& g)
{
	g.fillAll(backgroundColour);
}

void CabbagePropertiesPanel::resized()
{
	propertyPanel.setBounds (getLocalBounds().reduced (4));
}

//==============================================================================
void CabbagePropertiesPanel::setPropertyByName(ValueTree widgetData, String name, var value)
{
	CabbageIdentifierPropertyStringPairs propertyStringPairs;
	const String identifier = propertyStringPairs.getValue(name, "");

	if(identifier.isNotEmpty())
	{
		//CabbageUtilities::debug(value.toString());
		CabbageWidgetData::setStringProp(widgetData, identifier, value);
		//update code in editor when changes are made...
		sendChangeMessage();
	}
}

void CabbagePropertiesPanel::changeListenerCallback(ChangeBroadcaster *source)
{
	if(ColourPropertyComponent* colourProperty = dynamic_cast<ColourPropertyComponent*>(source))
	{
		setPropertyByName(widgetData, colourProperty->getName(), colourProperty->getCurrentColourString());
	}	
}

void CabbagePropertiesPanel::textPropertyComponentChanged(TextPropertyComponent *comp)
{
	//when in edit mode, direct bounds updating is only permitted from properties dialogue
	CabbageWidgetData::setNumProp(widgetData, CabbageIdentifierIds::allowboundsupdate, 1);
	setPropertyByName(widgetData, comp->getName(), comp->getValue());
	CabbageWidgetData::setNumProp(widgetData, CabbageIdentifierIds::allowboundsupdate, 0);	
}
	
void CabbagePropertiesPanel::valueChanged(Value& value)
{
	if(value.refersToSameSourceAs(isActiveValue))
		setPropertyByName(widgetData, "Active", value.getValue());		
	else if(value.refersToSameSourceAs(isVisibleValue))
		setPropertyByName(widgetData, "Visible", value.getValue());		
	else if(value.refersToSameSourceAs(alphaValue))
		setPropertyByName(widgetData, "Alpha", value.getValue());	
}
