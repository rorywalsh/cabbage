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
static void addListener (Array<PropertyComponent*> comps, CabbagePropertiesPanel* owner)
{
    for ( int i = 0; i < comps.size(); i++)
    {
        if (TextPropertyComponent* textProperty = dynamic_cast<TextPropertyComponent*> (comps[i]))
        {
            textProperty->addListener (owner);
        }
        else if (ColourPropertyComponent* colourProperty = dynamic_cast<ColourPropertyComponent*> (comps[i]))
        {
            colourProperty->addChangeListener (owner);
        }
        else if (ColourMultiPropertyComponent* colourPropMulti = dynamic_cast<ColourMultiPropertyComponent*> (comps[i]))
        {
            colourPropMulti->addChangeListener (owner);
        }
        else if (CabbageFilePropertyComponent* fileComp = dynamic_cast<CabbageFilePropertyComponent*> (comps[i]))
        {
            fileComp->filenameComp.addListener (owner);
        }
    }
}

//==============================================================================
static Array<PropertyComponent*> createRotationEditors (CabbagePropertiesPanel* owner, ValueTree valueTree)
{
    Array<PropertyComponent*> comps;

    const float rotate = CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::rotate);
    const float pivotx = CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::pivotx);
    const float pivoty = CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::pivoty);

    comps.add (new TextPropertyComponent (Value (var (pivotx)), "Pivot X", 200, false));
    comps.add (new TextPropertyComponent (Value (var (pivoty)), "Pivot Y", 200, false));
    comps.add (new TextPropertyComponent (Value (var (rotate)), "Rotate", 200, false));
    addListener (comps, owner);
    return comps;
}

//==============================================================================
static void createMultiLineTextEditors (ValueTree valueTree, Array<PropertyComponent*>& comps, Identifier identifier, String label)
{
    StringArray items;
    const Array<var>* array = CabbageWidgetData::getProperty (valueTree, identifier).getArray();

    if (array)
    {
        for ( int i = 0 ; i < array->size(); i++)
        {
            items.add (array->getReference (i).toString().trim());
        }

        comps.add (new TextPropertyComponent ( Value (var (items.joinIntoString ("\n"))), label, 1000, true));
    }
    else
    {
        var text = CabbageWidgetData::getProperty (valueTree, CabbageIdentifierIds::text);
        StringArray stringArray;
        stringArray.addLines (text.toString());
        comps.add (new TextPropertyComponent (Value (var (stringArray.joinIntoString ("\n"))), label, 1000, true));
    }

    comps[comps.size() - 1]->setPreferredHeight (60);
}

//==============================================================================
// Property Panel for editing widgets
//==============================================================================
CabbagePropertiesPanel::CabbagePropertiesPanel (ValueTree widgetData)
    : widgetData (widgetData), hideButton("x")
{

    setOpaque (true);
    setSize (300, 500);
    
    propertyPanelLook.reset (new PropertyPanelLookAndFeel());
    propertyPanel.setLookAndFeel (propertyPanelLook.get());
    
    flatLook.reset (new FlatButtonLookAndFeel());
    hideButton.setLookAndFeel (flatLook.get());
    hideButton.setColour(TextButton::ColourIds::buttonColourId, backgroundColour);// Colours::black);
    hideButton.setColour(TextButton::ColourIds::textColourOffId, backgroundColour.contrasting(1.0f));//Colours::white);

    addAndMakeVisible (propertyPanel);
	addAndMakeVisible(hideButton);
	hideButton.addListener(this);
    propertyPanel.getLookAndFeel().setColour (TextEditor::ColourIds::highlightedTextColourId, Colours::black);
}

CabbagePropertiesPanel::~CabbagePropertiesPanel()
{
    for (auto open : sectionStates)
        open->xmlElement  = nullptr;

    sectionStates.clear();
    hideButton.setLookAndFeel (nullptr);
    propertyPanel.setLookAndFeel (nullptr);
}

void CabbagePropertiesPanel::buttonClicked(Button *button)
{
    hide = true;
    sendChangeMessage();
}

void CabbagePropertiesPanel::saveOpenessState()
{
    const String name = CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::name);

    if (getSectionState (name) == nullptr)
        sectionStates.add (new SectionState (name, propertyPanel.getOpennessState().release()));
    else
        getSectionState (name)->xmlElement = propertyPanel.getOpennessState();

}

void CabbagePropertiesPanel::updateProperties (ValueTree wData)
{
    widgetData = wData;
    const String name = CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::name);
    const String typeOfWidget = CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::type);
    CabbageImageWidgetStrings imageWidgets;
    propertyPanel.clear();

    propertyPanel.addSection ("Bounds", createPositionEditors (wData));
    propertyPanel.addSection ("Rotation", createRotationEditors (this, wData), false);

    if (typeOfWidget != "gentable")
        propertyPanel.addSection ("Channels", createChannelEditors (wData));

    propertyPanel.addSection ("Values", createValueEditors (this, wData));

    if (typeOfWidget == "gentable")
    {
        propertyPanel.addSection ("AmpRange", createAmpRangeEditors (wData));
        propertyPanel.addSection ("Tables", createTextEditors (wData));
        propertyPanel.addSection ("Sample Range", createTwoValueEditors (wData, CabbageIdentifierIds::samplerange));
        propertyPanel.addSection ("Scrubber Position", createTwoValueEditors (wData, CabbageIdentifierIds::scrubberposition));
    }
    else
        propertyPanel.addSection ("Text", createTextEditors (wData));

    propertyPanel.addSection ("Colours", createColourChoosers (wData));

    if (imageWidgets.contains (typeOfWidget))
        propertyPanel.addSection ("Images", createFileEditors (wData));


    propertyPanel.addSection ("Widget Array", createWidgetArrayEditors (this, wData), false);
    propertyPanel.addSection ("Misc", createMiscEditors (wData));


    if (getSectionState (name) != nullptr)
        propertyPanel.restoreOpennessState (*getSectionState (name)->xmlElement);

    this->setVisible (true);

}

//==============================================================================
void CabbagePropertiesPanel::paint (Graphics& g)
{
    g.fillAll (backgroundColour.withAlpha (1.f));
    
    hideButton.setColour(TextButton::ColourIds::buttonColourId, backgroundColour);
    hideButton.setColour(TextButton::ColourIds::textColourOffId, backgroundColour.contrasting(1.0f));
}

void CabbagePropertiesPanel::resized()
{
	hideButton.setBounds (getWidth() - 23, -2, 20, 12);
    propertyPanel.setBounds (getLocalBounds().reduced (4));
}

//==============================================================================
void CabbagePropertiesPanel::setPropertyByName (String name, var value)
{
    CabbageIdentifierPropertyStringPairs propertyStringPairs;
    const String identifier = propertyStringPairs.getValue (name, "");

    CabbageAmpRangeIdentifiers ampRangeIdentifiers;
    CabbageScrubberPositionIdentifiers scrubberIdentifiers;

    if (identifier.isNotEmpty())
    {
        if (identifier == CabbageIdentifierIds::increment.toString())
        {
            CabbageWidgetData::setProperty (widgetData, CabbageIdentifierIds::decimalplaces,
                                            CabbageUtilities::getNumberOfDecimalPlaces (value.toString()));
            CabbageWidgetData::setProperty (widgetData, identifier, value);
        }

        else if (ampRangeIdentifiers.contains (identifier))
            getAmpRangeForTable (identifier, value);

        else if (scrubberIdentifiers.contains (identifier))
            getScrubberPositionForTable (identifier, value);

        else
        {
            CabbageWidgetData::setProperty (widgetData, identifier, value);
        }

        sendChangeMessage();    //update code in editor when changes are made...
    }
}

//=============================================================================================
void CabbagePropertiesPanel::getAmpRangeForTable (String identifier, var value)
{
    var amprange = CabbageWidgetData::getProperty (widgetData, CabbageIdentifierIds::amprange);

    if (identifier == CabbageIdentifierIds::amprange_min.toString())
        amprange[0] = value;
    else if (identifier == CabbageIdentifierIds::amprange_max.toString())
        amprange[1] = value;
    else if (identifier == CabbageIdentifierIds::amprange_tablenumber.toString())
        amprange[2] = value;
    else if (identifier == CabbageIdentifierIds::amprange_quantise.toString())
        amprange[3] = value;

    CabbageWidgetData::setProperty (widgetData, CabbageIdentifierIds::amprange, amprange);
}

void CabbagePropertiesPanel::getScrubberPositionForTable (String identifier, var value)
{
    var scrubberPosition = CabbageWidgetData::getProperty (widgetData, CabbageIdentifierIds::scrubberposition);

    if (identifier == CabbageIdentifierIds::scrubberposition_sample.toString())
        scrubberPosition[0] = value;
    else if (identifier == CabbageIdentifierIds::scrubberposition_table.toString())
        scrubberPosition[1] = value;


    CabbageWidgetData::setProperty (widgetData, CabbageIdentifierIds::scrubberposition_sample, scrubberPosition[0]);
    CabbageWidgetData::setProperty (widgetData, CabbageIdentifierIds::scrubberposition_table, scrubberPosition[1]);
    CabbageWidgetData::setProperty (widgetData, CabbageIdentifierIds::scrubberposition, scrubberPosition);
}
//=============================================================================================

void CabbagePropertiesPanel::changeListenerCallback (ChangeBroadcaster* source)
{
    if (ColourPropertyComponent* colourProperty = dynamic_cast<ColourPropertyComponent*> (source))
    {
        setPropertyByName (colourProperty->getName(), colourProperty->getCurrentColourString());
    }
    else if (ColourMultiPropertyComponent* colourPropertyMulti = dynamic_cast<ColourMultiPropertyComponent*> (source))
    {
        if (colourPropertyMulti->getName() == "Tables")
        {
            var colours = CabbageWidgetData::getProperty (widgetData, CabbageIdentifierIds::tablecolour);
            var tableColours = colours.clone();

            if (colourPropertyMulti->currentColourIndex > tableColours.size() - 1)
                tableColours.append (colourPropertyMulti->getCurrentColourString());
            else
                tableColours[colourPropertyMulti->currentColourIndex] = colourPropertyMulti->getCurrentColourString();

            CabbageWidgetData::setProperty (widgetData, CabbageIdentifierIds::tablecolour, tableColours);
        }
        else
        {
            var mColours = CabbageWidgetData::getProperty (widgetData, CabbageIdentifierIds::metercolour);
            var meterColours = mColours.clone();

            if (colourPropertyMulti->currentColourIndex > meterColours.size() - 1)
                meterColours.append (colourPropertyMulti->getCurrentColourString());
            else
                meterColours[colourPropertyMulti->currentColourIndex] = colourPropertyMulti->getCurrentColourString();

            CabbageWidgetData::setProperty (widgetData, CabbageIdentifierIds::metercolour, meterColours);
        }

        sendChangeMessage();    //update code in editor when changes are made...
    }
	//this->setVisible(false);
}

void CabbagePropertiesPanel::textPropertyComponentChanged (TextPropertyComponent* comp)
{
    //when in edit mode, direct bounds updating is only permitted from properties dialogue
    CabbageWidgetData::setNumProp (widgetData, CabbageIdentifierIds::allowboundsupdate, 1);
    setPropertyByName (comp->getName(), comp->getValue());
    CabbageWidgetData::setNumProp (widgetData, CabbageIdentifierIds::allowboundsupdate, 0);
}

void CabbagePropertiesPanel::valueChanged (Value& value)
{
    if (value.refersToSameSourceAs (isActiveValue))
    {
        bool val = value.getValue();
        setPropertyByName ("Active", val == true ? 1 : 0);
    }

    else if (value.refersToSameSourceAs (isVisibleValue))
    {
        bool val = value.getValue();
        setPropertyByName ("Visible", val == true ? 1 : 0);
    }

    else if (value.refersToSameSourceAs (alphaValue))
        setPropertyByName ("Alpha", value.getValue());

	else if (value.refersToSameSourceAs(innerRadius))
		setPropertyByName("Inner Radius", value.getValue());

	else if (value.refersToSameSourceAs(outerRadius))
		setPropertyByName("Outer Radius", value.getValue());


    else if (value.refersToSameSourceAs (zoomValue))
        setPropertyByName ("Zoom", value.getValue());

    else if (value.refersToSameSourceAs (sliderNumberBoxValue))
        setPropertyByName ("Value Box", value.getValue());

    else if (value.refersToSameSourceAs (fillTableWaveformValue))
        setPropertyByName ("Fill", value.getValue());

    else if (value.refersToSameSourceAs (channelTypeValue))
    {
        if (value.getValue().isInt())
        {
            if (int (value.getValue()) == 1)
                setPropertyByName ("Channel Type", "string");
        }
    }

    else if (value.refersToSameSourceAs (shapeValue))
    {
        if (value.getValue().isInt())
            setPropertyByName ("Shape", int (value.getValue()) == 0 ? "square" : "circle");
    }

    else if (value.refersToSameSourceAs (velocityValue))
    {
        if (value.getValue().isDouble())
            setPropertyByName ("Velocity", value.getValue());
    }

    else if (value.refersToSameSourceAs (alignValue))
    {
        if (value.getValue().isInt())
        {
            if (int (value.getValue()) == 0)
                setPropertyByName ("Align", "centre");
            else if (int (value.getValue()) == 1)
                setPropertyByName ("Align", "left");
            else if (int (value.getValue()) == 2)
                setPropertyByName ("Align", "right");
            else if (int (value.getValue()) == 3)
                setPropertyByName ("Align", "above");
            else if (int (value.getValue()) == 4)
                setPropertyByName ("Align", "below");
        }
    }

    else if (value.refersToSameSourceAs (fileModeValue))
    {
        if (value.getValue().isInt())
        {
            if (int (value.getValue()) == 0)
                setPropertyByName ("Mode", "file");
            else if (int (value.getValue()) == 1)
                setPropertyByName ("Mode", "directory");
            else if (int (value.getValue()) == 2)
                setPropertyByName ("Mode", "snapshot");
        }
    }
}

void CabbagePropertiesPanel::filenameComponentChanged (FilenameComponent* fileComponent)
{
    DBG(fileComponent->getName());
    if (File (fileComponent->getCurrentFileText()).existsAsFile())
    {
        const String csdFile = CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::csdfile);
        fileComponent->setTooltip (fileComponent->getCurrentFileText());
        const String relativePath = File (fileComponent->getCurrentFileText()).getRelativePathFrom (File (csdFile));
        setPropertyByName (fileComponent->getName(), relativePath);
    }
    else
    {
        fileComponent->setCurrentFile (File (""), false, dontSendNotification);
        fileComponent->setTooltip (fileComponent->getCurrentFileText());
        setPropertyByName (fileComponent->getName(), fileComponent->getCurrentFileText());

    }
}
//==============================================================================
Array<PropertyComponent*> CabbagePropertiesPanel::createChannelEditors (ValueTree valueTree)
{
    Array<PropertyComponent*> comps;
    const var channel = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::channel);
    const var identChannel = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::identchannel);
    const String typeOfWidget = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::type);

    if ( typeOfWidget != "gentable" &&  typeOfWidget != "groupbox")
    {
        const Array<var>* array = CabbageWidgetData::getProperty (valueTree, CabbageIdentifierIds::channel).getArray();

        if (array && array->size() > 1)
            createMultiLineTextEditors (valueTree, comps, CabbageIdentifierIds::channel, "Channel");
        else
            comps.add (new TextPropertyComponent (Value (channel), "Channel", 200, false));
    }

    comps.add (new TextPropertyComponent (Value (identChannel), "Ident Channel", 100, false));

    if (typeOfWidget == "combobox" || typeOfWidget == "listbox")
    {
        channelTypeValue.addListener (this);
        StringArray choices;
        Array<var> choiceVars;

        choices.add ("Number");
        choices.add ("String");
        choiceVars.add (0);
        choiceVars.add (1);

        if (CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::channeltype) == "number")
            channelTypeValue.setValue (0);
        else
            channelTypeValue.setValue (1);

        comps.add (new ChoicePropertyComponent (channelTypeValue, "Channel Type", choices, choiceVars));
    }

    addListener (comps, this);
    return comps;
}
//==============================================================================
Array<PropertyComponent*> CabbagePropertiesPanel::createTextEditors (ValueTree valueTree)
{
    Array<PropertyComponent*> comps;
    const String typeOfWidget = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::type);

    const bool isMultiline = typeOfWidget == "listbox" || typeOfWidget == "combobox" || typeOfWidget.contains ("button") || typeOfWidget == "gentable" ? true : false;

    if (isMultiline == true)
    {
        createMultiLineTextEditors (valueTree, comps,  typeOfWidget == "gentable" ? CabbageIdentifierIds::tablenumber : CabbageIdentifierIds::text,
                                    typeOfWidget == "gentable" ? "Table Numbers" : "Text");
    }
    else
    {
        const String text = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::text);
        comps.add (new TextPropertyComponent (Value (var (text)), "Text", 1000, isMultiline));
    }

    if (typeOfWidget != "gentable")
    {
        const String popupText = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::popuptext);
        comps.add (new TextPropertyComponent (Value (var (popupText)), "popup Text", 100, false));
    }

    addListener (comps, this);
    return comps;
}

//==============================================================================
Array<PropertyComponent*> CabbagePropertiesPanel::createColourChoosers (ValueTree valueTree)
{
    Array<PropertyComponent*> comps;

    const String colourString = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::colour);
    const String onColourString = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::oncolour);
    const String onFontColourString = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::onfontcolour);
    const String fontColourString = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::fontcolour);


    const String typeOfWidget = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::type);

    if (typeOfWidget == "checkbox" || typeOfWidget.contains ("button"))
    {
        comps.add (new ColourPropertyComponent ("Colour: Off", colourString));

        if ( typeOfWidget != "filebutton" && typeOfWidget != "infobutton")
            comps.add (new ColourPropertyComponent ("Colour: On", onColourString));

        comps.add (new ColourPropertyComponent ("Font: Off", fontColourString));

        if ( typeOfWidget != "filebutton" && typeOfWidget != "infobutton")
            comps.add (new ColourPropertyComponent ("Font: On", onFontColourString));

    }
    else if (typeOfWidget == "combobox" || typeOfWidget == "listbox"  )
    {
        comps.add (new ColourPropertyComponent ("Colour", colourString));
        comps.add (new ColourPropertyComponent ("Font", fontColourString));
        if (typeOfWidget == "listbox")
        {
            const String highlightColour = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::highlightcolour);
            comps.add(new ColourPropertyComponent("Selected Row", highlightColour));
        }
    }
    else if (typeOfWidget == "image" || typeOfWidget == "soundfiler")
    {
        const String imgOutlineColourString = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::outlinecolour);
        const String imgBackgroundColour = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::tablebackgroundcolour);

        comps.add (new ColourPropertyComponent ("Colour", colourString));

        if (typeOfWidget == "soundfiler")
            comps.add (new ColourPropertyComponent ("Soundfiler Background", imgBackgroundColour));
        else
            comps.add (new ColourPropertyComponent ("Outline", imgOutlineColourString));
    }
    else if (typeOfWidget.contains ("slider") || typeOfWidget == "encoder" || typeOfWidget.contains ("range"))
    {
        const String outlineColourString = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::outlinecolour);
        const String textColourString = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::textcolour);
        const String fontColourString = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::fontcolour);
        const String trackerColourString = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::trackercolour);
        const String markerColourString = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::markercolour);
        const String textboxColourString = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::textboxcolour);
        const String textboxOutlineColourString = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::textboxoutlinecolour);

        comps.add (new ColourPropertyComponent ("Colour", colourString));

        if (typeOfWidget.contains ("range") == false)
            comps.add (new ColourPropertyComponent ("Text Colour", textColourString));

        comps.add (new ColourPropertyComponent ("Font", fontColourString));

        if (typeOfWidget == "rslider")
        {
            comps.add (new ColourPropertyComponent ("Outline", outlineColourString));
            comps.add (new ColourPropertyComponent ("Marker", markerColourString));
        }

        comps.add (new ColourPropertyComponent ("Tracker", trackerColourString));
        comps.add (new ColourPropertyComponent ("Value Box Colour", textboxColourString));
        comps.add (new ColourPropertyComponent ("Value Box Outline", textboxOutlineColourString));
    }

    else if (typeOfWidget == "label" || typeOfWidget == "groupbox" || typeOfWidget == "numberbox" || typeOfWidget == "csoundoutput" || typeOfWidget == "textbox")
    {
        const String fontColourString = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::fontcolour);
        const String textColourString = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::textcolour);
        const String outlineColourString = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::outlinecolour);
        comps.add (new ColourPropertyComponent ("Colour", colourString));
        comps.add (new ColourPropertyComponent ("Font", fontColourString));

        if (typeOfWidget == "groupbox")
            comps.add (new ColourPropertyComponent ("Outline", outlineColourString));
        else if (typeOfWidget == "numberbox")
            comps.add (new ColourPropertyComponent ("Text Colour", textColourString));
    }

    else if (typeOfWidget == "keyboard")
    {
        const String whiteNotes = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::whitenotecolour);
        const String blackNotes = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::blacknotecolour);
        const String noteSeparator = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::keyseparatorcolour);
        const String arrowBg = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::arrowbackgroundcolour);
        const String arrow = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::arrowcolour);
        const String mouseOverKey = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::mouseoverkeycolour);

        comps.add (new ColourPropertyComponent ("White Notes", whiteNotes));
        comps.add (new ColourPropertyComponent ("Black Notes", blackNotes));
        comps.add (new ColourPropertyComponent ("Key Separator", noteSeparator));
        comps.add (new ColourPropertyComponent ("Arrows Background", arrowBg));
        comps.add (new ColourPropertyComponent ("Arrows", arrow));
        comps.add (new ColourPropertyComponent ("Mouse Over", mouseOverKey));

    }

    else if (typeOfWidget == "gentable")
    {
        const String tableGridColour = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::tablegridcolour);
        const String tableBackgroundColour = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::tablebackgroundcolour);
        const var tableColour = CabbageWidgetData::getProperty (valueTree, CabbageIdentifierIds::tablecolour);

        comps.add (new ColourMultiPropertyComponent ("Tables", tableColour));
        comps.add (new ColourPropertyComponent ("Table Grid", tableGridColour));
        comps.add (new ColourPropertyComponent ("Table Background", tableBackgroundColour));


    }

    else if (typeOfWidget == "vmeter" || typeOfWidget == "hmeter")
    {
        const String overlaycolour = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::overlaycolour);
        const var meterColour = CabbageWidgetData::getProperty (valueTree, CabbageIdentifierIds::metercolour);

        comps.add (new ColourMultiPropertyComponent ("Meters", meterColour));
        comps.add (new ColourPropertyComponent ("Overlay Colour", overlaycolour));


    }

    else if (typeOfWidget == "xypad")
    {
        const String colour = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::colour);
        const String backgroundColour = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::backgroundcolour);
        const String textColour = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::textcolour);
        const String fontColour = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::fontcolour);
        const String ballColour = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::ballcolour);

        comps.add (new ColourPropertyComponent ("Colour", colour));
        comps.add (new ColourPropertyComponent ("Ball", ballColour));
        comps.add (new ColourPropertyComponent ("Background", backgroundColour));
        comps.add (new ColourPropertyComponent ("Text Colour", textColour));
        comps.add (new ColourPropertyComponent ("Font", fontColour));

    }

    alphaValue.setValue (CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::alpha));
    alphaValue.addListener (this);
    comps.add (new SliderPropertyComponent (alphaValue, "Alpha", 0, 1, .01, 1, 1));


    addListener (comps, this);
    return comps;
}
//==============================================================================
Array<PropertyComponent*> CabbagePropertiesPanel::createPositionEditors (ValueTree valueTree)
{
    Array<PropertyComponent*> comps;
    Rectangle<int> bounds = CabbageWidgetData::getBounds (valueTree);
    comps.add (new TextPropertyComponent (Value (var (bounds.getX())), "X Position", 200, false));
    comps.add (new TextPropertyComponent (Value (var (bounds.getY())), "Y Position", 200, false));
    comps.add (new TextPropertyComponent (Value (var (bounds.getWidth())), "Width", 200, false));
    comps.add (new TextPropertyComponent (Value (var (bounds.getHeight())), "Height", 200, false));

    isActiveValue.setValue (CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::active));
    isActiveValue.addListener (this);
    isVisibleValue.setValue (CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::visible));
    isVisibleValue.addListener (this);

    comps.add (new BooleanPropertyComponent (isActiveValue, "Active", "Is Active"));
    comps.add (new BooleanPropertyComponent (isVisibleValue, "Visible", "Is Visible"));


    addListener (comps, this);
    return comps;
}

//==============================================================================
Array<PropertyComponent*> CabbagePropertiesPanel::createFileEditors (ValueTree valueTree)
{
    Array<PropertyComponent*> comps;
    const String typeOfWidget = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::type);
    const String csdFile = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::csdfile);

    if (typeOfWidget == "checkbox" || typeOfWidget == "button")
    {
        const String onFile = CabbageUtilities::getFileAndPath (File (csdFile), CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::imgbuttonon));
        const String offFile = CabbageUtilities::getFileAndPath (File (csdFile), CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::imgbuttonoff));

        comps.add (new CabbageFilePropertyComponent("On Image", false, true, "*", onFile));
        comps.add (new CabbageFilePropertyComponent ("Off Image", false, true, "*", offFile));
    }
    else if (typeOfWidget == "combobox")
    {

    }
    else if (typeOfWidget.contains ("slider"))
    {
        const String sliderFile = CabbageUtilities::getFileAndPath (File (csdFile), CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::imgfile));
        const String sliderBgFile = CabbageUtilities::getFileAndPath (File (csdFile), CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::imgsliderbg));
        comps.add (new CabbageFilePropertyComponent ("Rotary Image", false, true, "*", sliderFile));
        comps.add (new CabbageFilePropertyComponent ("Background Image", false, true, "*", sliderBgFile));
    }

    else if (typeOfWidget == "image")
    {
        const String file = CabbageUtilities::getFileAndPath (File (csdFile), CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::file));
        comps.add (new CabbageFilePropertyComponent ("Image File", false, true, "*", file));
    }

    else if (typeOfWidget == "groupbox")
    {
        const String file = CabbageUtilities::getFileAndPath (File (csdFile), CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::imggroupbox));
        comps.add (new CabbageFilePropertyComponent ("Groupbox Image", false, true, "*", file));
    }

    addListener (comps, this);
    return comps;
}
//==============================================================================
Array<PropertyComponent*> CabbagePropertiesPanel::createTwoValueEditors (ValueTree valueTree, Identifier identifier)
{
    Array<PropertyComponent*> comps;
    const String typeOfWidget = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::type);

    if (identifier.toString() == "samplerange")
    {
        const int startPos = CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::startpos);
        comps.add (new TextPropertyComponent (Value (startPos), "Start Index", 200, false));
        comps[comps.size() - 1]->setTooltip ("Starting value of index");
        const int endPos = CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::endpos);
        comps.add (new TextPropertyComponent (Value (endPos), "End Index", 200, false));
    }
    else if (identifier.toString() == "scrubberposition")
    {
        const int startPos = CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::scrubberposition_sample);
        comps.add (new TextPropertyComponent (Value (startPos), "Scrubber Pos", 200, false));
        const int table = CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::scrubberposition_table);
        comps.add (new TextPropertyComponent (Value (table), "Scrubber Table", 200, false));
    }

    addListener (comps, this);
    return comps;
}
//==============================================================================
Array<PropertyComponent*> CabbagePropertiesPanel::createAmpRangeEditors (ValueTree valueTree)
{
    Array<PropertyComponent*> comps;

    var amprange = valueTree.getProperty (CabbageIdentifierIds::amprange);

    if (amprange.size() == 4)
    {
        comps.add (new TextPropertyComponent (Value (amprange[0]), "Min Amp", 200, false));
        comps.add (new TextPropertyComponent (Value (amprange[1]), "Max Amp", 200, false));
        comps.add (new TextPropertyComponent (Value (amprange[2]), "Table No.", 200, false));
        const String quantiseString = String (float (amprange[3]), 4);
        comps.add (new TextPropertyComponent (Value (quantiseString), "Quantise", 200, false));
    }

    addListener (comps, this);
    return comps;
}
//==============================================================================
Array<PropertyComponent*> CabbagePropertiesPanel::createMiscEditors (ValueTree valueTree)
{
    Array<PropertyComponent*> comps;
    var corners = valueTree.getProperty (CabbageIdentifierIds::corners);
    const String typeOfWidget = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::type);

    if (corners.isVoid() == false)
    {
        comps.add (new TextPropertyComponent (Value (corners), "Corners", 200, false));
    }

    if (typeOfWidget == "checkbox" || typeOfWidget == "image")
    {
        shapeValue.addListener (this);
        StringArray choices;
        Array<var> choiceVars;

        choices.add ("Square");
        choices.add ("Circle");
        choiceVars.add (0);
        choiceVars.add (1);

        if (CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::shape) == "square")
            shapeValue.setValue (0);
        else
            shapeValue.setValue (1);

        comps.add (new ChoicePropertyComponent (shapeValue, "Shape", choices, choiceVars));

    }


    if (typeOfWidget == "label" || typeOfWidget == "groupbox" || typeOfWidget == "numberbox")
    {
        alignValue.addListener (this);
        StringArray choices;
        Array<var> choiceVars;

        const String typeOfWidget = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::type);


        choices.add ("Centre");
        choices.add ("Left");
        choices.add ("Right");
        choiceVars.add (0);
        choiceVars.add (1);
        choiceVars.add (2);

        if (typeOfWidget == "numberbox")
        {
            choices.remove (0);
            choiceVars.remove (0);
            choices.add ("Above");
            choices.add ("Below");
            choiceVars.add (3);
            choiceVars.add (4);
        }

        if (CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::align) == "centre")
            alignValue.setValue (0);
        else if (CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::align) == "left")
            alignValue.setValue (1);
        else if (CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::align) == "right")
            alignValue.setValue (2);
        else if (CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::align) == "above")
            alignValue.setValue (3);
        else if (CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::align) == "below")
            alignValue.setValue (4);


        comps.add (new ChoicePropertyComponent (alignValue, typeOfWidget == "numberbox" ? "Align Text" : "Align", choices, choiceVars));


    }

    if (typeOfWidget == "combobox" || typeOfWidget == "soundfiler")
    {
        comps.add (new CabbageFilePropertyComponent ("File", false, true));

        if (typeOfWidget == "soundfiler")
        {
            var zoom = valueTree.getProperty (CabbageIdentifierIds::zoom);
            const String zoomValue = String (CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::min), 2);
            comps.add (new TextPropertyComponent (Value (zoomValue), "Zoom", 200, false));
        }

        if (typeOfWidget == "combobox")
        {
            createMultiLineTextEditors (valueTree, comps, CabbageIdentifierIds::populate, "Populate");
        }
    }

    else if (typeOfWidget == "image" || typeOfWidget == "groupbox" || typeOfWidget == "vmeter" || typeOfWidget == "hmeter")
    {
        var outline = valueTree.getProperty (CabbageIdentifierIds::outlinethickness);
        comps.add (new TextPropertyComponent (Value (outline), "Outline Thickness", 200, false));

        if (typeOfWidget == "image" || typeOfWidget == "groupbox")
        {
            var line = valueTree.getProperty (CabbageIdentifierIds::linethickness);
            comps.add (new TextPropertyComponent (Value (line), "Line Thickness", 200, false));
        }
    }

    else if (typeOfWidget == "gentable")
    {
        fillTableWaveformValue.addListener (this);
        fillTableWaveformValue.setValue (CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::fill));
        comps.add (new BooleanPropertyComponent (fillTableWaveformValue, "Waveform", "Fill"));

        zoomValue.setValue (CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::zoom));
        zoomValue.addListener (this);
        comps.add (new SliderPropertyComponent (zoomValue, "Zoom", -1, 1, .01, 1, 1));
    }

    else if (typeOfWidget.contains ("slider") || typeOfWidget == "encoder")
    {
        sliderNumberBoxValue.setValue (CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::valuetextbox));
        sliderNumberBoxValue.addListener (this);
        comps.add (new BooleanPropertyComponent (sliderNumberBoxValue, "Value Box", "Is Visible"));

		innerRadius.setValue(CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::trackerinsideradius));
		innerRadius.addListener(this);
		comps.add(new SliderPropertyComponent(innerRadius, "Inner Radius", 0, 1, .01, 1, 1));

		outerRadius.setValue(CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::trackeroutsideradius));
		outerRadius.addListener(this);
		comps.add(new SliderPropertyComponent(outerRadius, "Outer Radius", 0, 1, .01, 1, 1));
    }

    else if (typeOfWidget == "filebutton")
    {
        fileModeValue.addListener (this);
        StringArray choices;
        Array<var> choiceVars;

        choices.add ("File");
        choices.add ("Directory");
        choices.add ("Snapshot");
        choiceVars.add (0);
        choiceVars.add (1);
        choiceVars.add (2);

        if (CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::mode) == "file")
            fileModeValue.setValue (0);
        else if (CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::mode) == "directory")
            fileModeValue.setValue (1);
        else
            fileModeValue.setValue (2);

        comps.add (new ChoicePropertyComponent (fileModeValue, "Mode", choices, choiceVars));

    }

    addListener (comps, this);
    return comps;
}

//==============================================================================
Array<PropertyComponent*> CabbagePropertiesPanel::createWidgetArrayEditors (CabbagePropertiesPanel* owner, ValueTree valueTree)
{
    Array<PropertyComponent*> comps;
    const int widgetArrayChannelSize = CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::arraysize);
    const String channelName  = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::basechannel);

    comps.add (new TextPropertyComponent (Value (channelName), "Base channel", 8, false));
    comps.add (new TextPropertyComponent (Value (widgetArrayChannelSize), "Array Size", 8, false));

    addListener (comps, this);
    return comps;
}
//==============================================================================
Array<PropertyComponent*> CabbagePropertiesPanel::createValueEditors (CabbagePropertiesPanel* owner, ValueTree valueTree)
{
    Array<PropertyComponent*> comps;
    const int decimalPlaces = CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::decimalplaces); //get precision of number to display
    const String typeOfWidget = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::type);

    if (typeOfWidget.contains ("slider") || typeOfWidget == "encoder" || typeOfWidget.contains ("range"))
    {
        const String min = String (CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::min), decimalPlaces);
        const String max = String (CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::max), decimalPlaces);
        const String skew = String (CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::sliderskew), decimalPlaces);
        const String incr = String (CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::increment), decimalPlaces + 2);

        comps.add (new TextPropertyComponent (Value (min), "Minimum", 8, false));
        comps.add (new TextPropertyComponent (Value (max), "Maximum", 8, false));

        if (typeOfWidget.contains ("slider"))
            comps.add (new TextPropertyComponent (Value (skew), "Skew", 8, false));

        comps.add (new TextPropertyComponent (Value (incr), "Increment", 8, false));

        if (typeOfWidget.contains ("slider"))
        {
            velocityValue.setValue (CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::velocity));
            velocityValue.addListener (this);
            comps.add (new SliderPropertyComponent (velocityValue, "Velocity", 0, 50, .01, .25, false));
        }


    }

    if (typeOfWidget == "xypad")
    {
        const String minx = String (CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::minx), decimalPlaces);
        const String maxx = String (CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::maxx), decimalPlaces);
        const String miny = String (CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::miny), decimalPlaces);
        const String maxy = String (CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::maxy), decimalPlaces);
        const String valuex = String (CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::valuex), decimalPlaces);
        const String valuey = String (CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::valuey), decimalPlaces);

        comps.add (new TextPropertyComponent (Value (minx), "Min: X", 8, false));
        comps.add (new TextPropertyComponent (Value (maxx), "Max: X", 8, false));
        comps.add (new TextPropertyComponent (Value (miny), "Min: Y", 8, false));
        comps.add (new TextPropertyComponent (Value (maxy), "Max: Y", 8, false));
        comps.add (new TextPropertyComponent (Value (valuex), "Value X", 8, false));
        comps.add (new TextPropertyComponent (Value (valuey), "Value Y", 8, false));
    }
    else if (typeOfWidget == CabbageWidgetTypes::button || typeOfWidget == CabbageWidgetTypes::checkbox)
    {
        const int radioGroup = CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::radiogroup);
        comps.add (new TextPropertyComponent (Value (radioGroup), "Radio Group", 8, false));
    }
    else
    {
        const String value = String (CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::value), decimalPlaces);

        if (typeOfWidget.contains ("range"))
        {
            const String minValue = String (CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::minvalue), decimalPlaces);
            const String maxValue = String (CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::maxvalue), decimalPlaces);
            comps.add (new TextPropertyComponent (Value (minValue), "Value Min", 8, false));
            comps.add (new TextPropertyComponent (Value (maxValue), "Value Max", 8, false));
        }
        else
            comps.add (new TextPropertyComponent (Value (value), "Value", 8, false));
    }

    addListener (comps, owner);

    return comps;
}
//==============================================================================
