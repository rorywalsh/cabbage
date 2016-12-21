/*
  Copyright (C) 2016 Rory Walsh

  Cabbage is free software; you can redistribute it
  and/or modify it under the terms of the GNU General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  Cabbage is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU General Public
  License along with Csound; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
  02111-1307 USA
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
        else if(CabbageFilePropertyComponent* fileComp = dynamic_cast<CabbageFilePropertyComponent*>(comps[i]))
        {
            fileComp->filenameComp.addListener(owner);
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
static void createMultiLineTextEditors(ValueTree valueTree, Array<PropertyComponent*> &comps, Identifier identifier, String label)
{
	StringArray items;
	const Array<var>* array = CabbageWidgetData::getProperty(valueTree, identifier).getArray();
	if(array)
	{
		for( int i = 0 ; i < array->size(); i++)
		{
			items.add(array->getReference(i).toString());
		}

		comps.add (new TextPropertyComponent(Value (var (items.joinIntoString("\n"))), label, 1000, true));
	}
	else
	{
		var text = CabbageWidgetData::getProperty(valueTree, CabbageIdentifierIds::text);
		StringArray stringArray;
		stringArray.addLines(text.toString());
		comps.add (new TextPropertyComponent(Value (var (stringArray.joinIntoString("\n"))), label, 1000, true));
	}	
}

//==============================================================================
static Array<PropertyComponent*> createChannelEditors(CabbagePropertiesPanel* owner, ValueTree valueTree)
{
    Array<PropertyComponent*> comps;
    var channel = CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::channel);
    var identChannel = CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::identchannel);
	
	const Array<var>* array = CabbageWidgetData::getProperty(valueTree, CabbageIdentifierIds::channel).getArray();
	if(array && array->size()>1)
		createMultiLineTextEditors(valueTree, comps, CabbageIdentifierIds::channel, "Channel");
	else
		comps.add (new TextPropertyComponent(Value(channel), "Channel", 200, false));
    
	comps.add (new TextPropertyComponent(Value(identChannel), "Ident Channel", 100, false));
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
        comps.add (new ChoicePropertyComponent(Value (Random::getSystemRandom().nextInt (6)), "Choice Property " + String (i + 1), choices, choiceVars));
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
	//setColour(TextEditor::highlightColourId, Colour(100, 100, 100));
	propertyPanel.getLookAndFeel().setColour (TextEditor::ColourIds::highlightedTextColourId, Colours::hotpink);
	
	const String typeOfWidget = CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::type);
	
    propertyPanel.addSection ("Bounds", createPositionEditors(widgetData));
    propertyPanel.addSection ("Rotation", createRotationEditors(this, widgetData));
    propertyPanel.addSection ("Channels", createChannelEditors(this, widgetData));
    propertyPanel.addSection ("Values", createValueEditors(this, widgetData));
    propertyPanel.addSection ("Text", createTextEditors(widgetData));
    propertyPanel.addSection ("Colours", createColourChoosers(widgetData));
    propertyPanel.addSection ("Images", createFileEditors(widgetData));
    propertyPanel.addSection ("Misc", createMiscEditors(widgetData));


    //ropertyPanel.addSection ("Channels", createChoices (16));
    //propertyPanel.addSection ("Buttons & Toggles", createButtons (20));
}

void CabbagePropertiesPanel::updateProperties(ValueTree wData)
{
    widgetData = wData;
	
	propertyPanel.getLookAndFeel().setColour (TextEditor::ColourIds::highlightedTextColourId, Colours::hotpink);
	const String typeOfWidget = CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::type);
	
    propertyPanel.clear();
    propertyPanel.addSection ("Bounds", createPositionEditors(widgetData));
    propertyPanel.addSection ("Rotation", createRotationEditors(this, widgetData), false);
    propertyPanel.addSection ("Channels", createChannelEditors(this, widgetData));
    propertyPanel.addSection ("Values", createValueEditors(this, widgetData));
    propertyPanel.addSection ("Text", createTextEditors(widgetData));
    propertyPanel.addSection ("Colours", createColourChoosers(widgetData));
    propertyPanel.addSection ("Images", createFileEditors(widgetData));
    propertyPanel.addSection ("Misc", createMiscEditors(widgetData));
    this->setVisible(true);

}
void CabbagePropertiesPanel::paint (Graphics& g)
{
    g.fillAll(backgroundColour.withAlpha(1.f));
}

void CabbagePropertiesPanel::resized()
{
    propertyPanel.setBounds (getLocalBounds().reduced (4));
}

//==============================================================================
void CabbagePropertiesPanel::setPropertyByName(String name, var value)
{
    CabbageIdentifierPropertyStringPairs propertyStringPairs;
    const String identifier = propertyStringPairs.getValue(name, "");

    if(identifier.isNotEmpty())
    {
        CabbageWidgetData::setProperty(widgetData, identifier, value);
		
		if(identifier==CabbageIdentifierIds::sliderincr.toString())
			CabbageWidgetData::setProperty(widgetData, CabbageIdentifierIds::decimalplaces, 
											CabbageUtilities::getNumberOfDecimalPlaces(StringArray(value.toString())));
											
        sendChangeMessage();	//update code in editor when changes are made...
    }
}

void CabbagePropertiesPanel::changeListenerCallback(ChangeBroadcaster *source)
{
    if(ColourPropertyComponent* colourProperty = dynamic_cast<ColourPropertyComponent*>(source))
    {
        setPropertyByName(colourProperty->getName(), colourProperty->getCurrentColourString());
    }
}

void CabbagePropertiesPanel::textPropertyComponentChanged(TextPropertyComponent *comp)
{
    //when in edit mode, direct bounds updating is only permitted from properties dialogue
    CabbageWidgetData::setNumProp(widgetData, CabbageIdentifierIds::allowboundsupdate, 1);
    setPropertyByName(comp->getName(), comp->getValue());
    CabbageWidgetData::setNumProp(widgetData, CabbageIdentifierIds::allowboundsupdate, 0);
}

void CabbagePropertiesPanel::valueChanged(Value& value)
{
    if(value.refersToSameSourceAs(isActiveValue))
        setPropertyByName("Active", value.getValue());
		
    else if(value.refersToSameSourceAs(isVisibleValue))
        setPropertyByName("Visible", value.getValue());
		
    else if(value.refersToSameSourceAs(alphaValue))
        setPropertyByName("Alpha", value.getValue());
		
    else if(value.refersToSameSourceAs(sliderNumberBoxValue))
        setPropertyByName("Value Box", value.getValue());
		
    else if(value.refersToSameSourceAs(shapeValue))
    {
        if(value.getValue().isInt())
            setPropertyByName("Shape", int(value.getValue())==0 ? "square" : "circle");
    }

    else if(value.refersToSameSourceAs(velocityValue))
    {
        if(value.getValue().isDouble())
            setPropertyByName("Velocity", value.getValue());
    }
	
    else if(value.refersToSameSourceAs(alignValue))
    {
        if(value.getValue().isInt())
		{
			if(int(value.getValue())==0)
				setPropertyByName("Align", "centre");
			else if(int(value.getValue())==1)
				setPropertyByName("Align", "left");
			else if(int(value.getValue())==2)
				setPropertyByName("Align", "right");   
			else if(int(value.getValue())==3)
				setPropertyByName("Align", "above");  
			else if(int(value.getValue())==4)
				setPropertyByName("Align", "below");          
		}
    }
	
    else if(value.refersToSameSourceAs(fileModeValue))
    {
        if(value.getValue().isInt())
		{
			if(int(value.getValue())==0)
				setPropertyByName("Mode", "file");
			else if(int(value.getValue())==1)
				setPropertyByName("Align", "directory");
			else if(int(value.getValue())==2)
				setPropertyByName("Align", "snapshot");         
		}
    }
}

void CabbagePropertiesPanel::filenameComponentChanged (FilenameComponent* fileComponent)
{
    fileComponent->setTooltip(fileComponent->getCurrentFileText());
    setPropertyByName(fileComponent->getName(), fileComponent->getCurrentFileText());
    const String csdFile = CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::csdfile);
}
//==============================================================================
Array<PropertyComponent*> CabbagePropertiesPanel::createTextEditors(ValueTree valueTree)
{
    Array<PropertyComponent*> comps;
    const String typeOfWidget = CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::type);

    const bool isMultiline = typeOfWidget=="combobox" || typeOfWidget.contains("button") ? true : false;

    if(isMultiline==true)
    {
		createMultiLineTextEditors(valueTree, comps, CabbageIdentifierIds::text, "Text");
    }
    else
    {
        const String text = CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::text);
        comps.add (new TextPropertyComponent(Value (var (text)), "Text", 1000, isMultiline));
    }

    const String popupText = CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::popuptext);
    comps.add (new TextPropertyComponent(Value (var (popupText)), "popup Text", 100, false));
    addListener(comps, this);
    return comps;
}

//==============================================================================
Array<PropertyComponent*> CabbagePropertiesPanel::createColourChoosers (ValueTree valueTree)
{
    Array<PropertyComponent*> comps;

    const String colourString = CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::colour);
    const String onColourString = CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::oncolour);
    const String onFontColourString = CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::onfontcolour);
    const String fontColourString = CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::fontcolour);


    const String typeOfWidget = CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::type);
    if(typeOfWidget == "checkbox" || typeOfWidget.contains("button"))
    {
        comps.add(new ColourPropertyComponent("Colour: Off", colourString));
		
        if( typeOfWidget != "filebutton" && typeOfWidget != "infobutton")
			comps.add(new ColourPropertyComponent("Colour: On", onColourString));
			
        comps.add(new ColourPropertyComponent("Font: Off", fontColourString));
		
		if( typeOfWidget != "filebutton" && typeOfWidget != "infobutton")
			comps.add(new ColourPropertyComponent("Font: On", onFontColourString));
			
    }
    else if(typeOfWidget == "combobox")
    {
        const String menuColourString = CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::menucolour);
        comps.add(new ColourPropertyComponent("Colour", colourString));
        comps.add(new ColourPropertyComponent("Font", fontColourString));
    }
    else if(typeOfWidget == "image" || typeOfWidget == "soundfiler")
    {
        const String outlineColourString = CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::outlinecolour);
		const String backgroundColour = CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::tablebackgroundcolour);
		
        comps.add(new ColourPropertyComponent("Colour", colourString));
		if(typeOfWidget == "soundfiler")
			comps.add(new ColourPropertyComponent("Soundfiler Background", backgroundColour));
		else
			comps.add(new ColourPropertyComponent("Outline Colour", outlineColourString));
    }
    else if(typeOfWidget.contains("slider") || typeOfWidget=="encoder")
    {
        const String outlineColourString = CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::outlinecolour);
        const String textColourString = CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::textcolour);
        const String fontColourString = CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::fontcolour);
        const String trackerColourString = CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::trackercolour);
        
		comps.add(new ColourPropertyComponent("Colour", colourString));		
		comps.add(new ColourPropertyComponent("Text Colour", textColourString));
		comps.add(new ColourPropertyComponent("Font", fontColourString));
		comps.add(new ColourPropertyComponent("Outline", outlineColourString));
		comps.add(new ColourPropertyComponent("Tracker", trackerColourString));
    }
	else if(typeOfWidget == "label" || typeOfWidget == "groupbox" || typeOfWidget == "numberbox" || typeOfWidget == "csoundoutput" || typeOfWidget == "textbox")
	{
		const String fontColourString = CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::fontcolour);
		const String textColourString = CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::textcolour);
		const String outlineColourString = CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::outlinecolour);
		comps.add(new ColourPropertyComponent("Colour", colourString));	
		comps.add(new ColourPropertyComponent("Font", fontColourString));
		
		if(typeOfWidget == "groupbox")
			comps.add(new ColourPropertyComponent("Outline", outlineColourString));
		else if(typeOfWidget == "numberbox")
			comps.add(new ColourPropertyComponent("Text Colour", textColourString));
	}

	else if(typeOfWidget == "keyboard")
	{
		const String whiteNotes = CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::whitenotecolour);
		const String blackNotes = CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::blacknotecolour);
		const String noteSeparator = CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::noteseparatorcolour);
		const String arrowBg = CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::arrowbackgroundcolour);
		const String arrow = CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::arrowcolour);
		
		comps.add(new ColourPropertyComponent("White Notes", whiteNotes));	
		comps.add(new ColourPropertyComponent("Black Notes", blackNotes));
		comps.add(new ColourPropertyComponent("Notes Separator", noteSeparator));
		comps.add(new ColourPropertyComponent("Arrows Background", arrowBg));
		comps.add(new ColourPropertyComponent("Arrows", arrow));

	}
	
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
Array<PropertyComponent*> CabbagePropertiesPanel::createFileEditors(ValueTree valueTree)
{
    Array<PropertyComponent*> comps;

    if(CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::type) == "checkbox")
    {
        comps.add (new CabbageFilePropertyComponent("On Image", false, true));
        comps.add (new CabbageFilePropertyComponent("Off Image", false, true));
    }
    else if(CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::type) == "combobox")
    {

    }

    addListener(comps, this);
    return comps;
}
//==============================================================================
Array<PropertyComponent*> CabbagePropertiesPanel::createMiscEditors(ValueTree valueTree)
{
    Array<PropertyComponent*> comps;
    var corners = valueTree.getProperty(CabbageIdentifierIds::corners);
	const String typeOfWidget = CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::type);
	
    if(corners.isVoid()==false)
    {
        comps.add (new TextPropertyComponent(Value (corners), "Corners", 200, false));
    }

    if(typeOfWidget == "checkbox" || typeOfWidget == "image")
    {
        shapeValue.addListener(this);
        StringArray choices;
        Array<var> choiceVars;

        choices.add ("Square");
        choices.add ("Circle");
        choiceVars.add (0);
        choiceVars.add (1);
		
        if(CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::shape) == "square")
            shapeValue.setValue(0);
		else
			shapeValue.setValue(1);
		
        comps.add (new ChoicePropertyComponent(shapeValue, "Shape", choices, choiceVars));
		
    }

    if(typeOfWidget == "label" || typeOfWidget == "groupbox" || typeOfWidget == "numberbox")
    {
        alignValue.addListener(this);
        StringArray choices;
        Array<var> choiceVars;
		
		const String typeOfWidget = CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::type);


        choices.add ("Centre");
        choices.add ("Left");
		choices.add ("Right");
        choiceVars.add (0);
        choiceVars.add (1);
		choiceVars.add (2);
		
		if(typeOfWidget == "numberbox")
		{
			choices.remove(0);
			choiceVars.remove(0);
			choices.add ("Above");
			choices.add ("Below");
			choiceVars.add (3);
			choiceVars.add (4);
		}
		
		if(CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::align) == "centre")
            alignValue.setValue(0);
		else if(CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::align) == "left")
            alignValue.setValue(1);
		else if(CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::align) == "right")
            alignValue.setValue(2);	
		else if(CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::align) == "above")
            alignValue.setValue(3);	
		else if(CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::align) == "below")
            alignValue.setValue(4);	
		
	
        comps.add (new ChoicePropertyComponent(alignValue, typeOfWidget=="numberbox" ? "Align Text" : "Align", choices, choiceVars));
		

    }
	
    if(typeOfWidget == "combobox" || typeOfWidget == "soundfiler")
    {
        comps.add (new CabbageFilePropertyComponent("File", false, true));
		
		if(typeOfWidget == "soundfiler")
		{
			var zoom = valueTree.getProperty(CabbageIdentifierIds::zoom);
			const String zoomValue = String(CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::min), 2);
			comps.add (new TextPropertyComponent(Value (zoomValue), "Zoom", 200, false));
		}
    }
	
    else if(typeOfWidget == "image" || typeOfWidget == "groupbox")
    {
        var outline = valueTree.getProperty(CabbageIdentifierIds::outlinethickness);
        comps.add (new TextPropertyComponent(Value (outline), "Outline Thickness", 200, false));
    }
	
	else if(typeOfWidget.contains("slider") || typeOfWidget=="encoder")
	{
		sliderNumberBoxValue.setValue(CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::valuetextbox));
		sliderNumberBoxValue.addListener(this);
		comps.add (new BooleanPropertyComponent(sliderNumberBoxValue, "Value Box", "Is Visible"));		
	} 

    else if(typeOfWidget == "filebutton")
    {
        shapeValue.addListener(this);
        StringArray choices;
        Array<var> choiceVars;

        choices.add ("File");
        choices.add ("Directory");
		choices.add ("Snapshot");
        choiceVars.add (0);
        choiceVars.add (1);
		choiceVars.add (2);
		
        if(CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::mode) == "file")
            shapeValue.setValue(0);
		else if(CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::mode) == "directory")
			shapeValue.setValue(1);
		else 
			shapeValue.setValue(2);
			
        comps.add (new ChoicePropertyComponent(shapeValue, "Mode", choices, choiceVars));
		
    }	

    addListener(comps, this);
    return comps;
}

//==============================================================================
Array<PropertyComponent*> CabbagePropertiesPanel::createValueEditors(CabbagePropertiesPanel* owner, ValueTree valueTree)
{
    Array<PropertyComponent*> comps;
	const int decimalPlaces = CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::decimalplaces); //get precision of number to display
    const String typeOfWidget = CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::type);
    if(typeOfWidget.contains("slider") || typeOfWidget=="encoder")
    {
        const String min = String(CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::min), decimalPlaces);
        const String max = String(CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::max), decimalPlaces);
        const String skew = String(CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::sliderskew), decimalPlaces);
        const String incr = String(CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::sliderincr), decimalPlaces);
		
        comps.add(new TextPropertyComponent(Value(min), "Minimum", 8, false));
        comps.add(new TextPropertyComponent(Value(max), "Maximum", 8, false));
        if(typeOfWidget.contains("slider"))
			comps.add(new TextPropertyComponent(Value(skew), "Skew", 8, false));
        
		comps.add(new TextPropertyComponent(Value(incr), "Increment", 8, false));

		if(typeOfWidget.contains("slider"))
		{
			velocityValue.setValue(CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::velocity));
			velocityValue.addListener(this);
			comps.add(new SliderPropertyComponent(velocityValue, "Velocity", 0, 50, .01, .25, false));
		}
		
		
    }
	
	if(typeOfWidget == "xypad")
    {
        const String minx = String(CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::minx), decimalPlaces);
        const String maxx = String(CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::maxx), decimalPlaces);
        const String miny = String(CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::miny), decimalPlaces);
        const String maxy = String(CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::maxy), decimalPlaces);
        const String valuex = String(CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::valuex), decimalPlaces);
        const String valuey = String(CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::valuey), decimalPlaces);
		
		comps.add(new TextPropertyComponent(Value(minx), "Min: X", 8, false));
		comps.add(new TextPropertyComponent(Value(maxx), "Max: X", 8, false));
		comps.add(new TextPropertyComponent(Value(miny), "Min: Y", 8, false));
		comps.add(new TextPropertyComponent(Value(maxy), "Max: Y", 8, false));
		comps.add(new TextPropertyComponent(Value(valuex), "Value X", 8, false));
		comps.add(new TextPropertyComponent(Value(valuey), "Value Y", 8, false));		
	}
	else
	{
		const String value = String(CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::value), decimalPlaces);
		comps.add(new TextPropertyComponent(Value(value), "Value", 8, false));
	}
    addListener(comps, owner);

    return comps;
}
//==============================================================================

