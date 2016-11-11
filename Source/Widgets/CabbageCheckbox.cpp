/*
  Copyright (C) 2016 Rory Walsh

  Cabbage is free software; you can redistribute it
  and/or modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  Cabbage is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with Csound; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
  02111-1307 USA
*/

#include "CabbageCheckbox.h"
#include "../Audio/Plugins/CabbagePluginEditor.h"


//==============================================================================
// custom checkbox component with optional surrounding groupbox
//==============================================================================
CabbageCheckbox::CabbageCheckbox(ValueTree wData) : CabbageWidgetBase(),
name(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::name)),
buttonText(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::text)),
isRect(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::shape).equalsIgnoreCase("square")),
corners(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::corners)),
ToggleButton(""),
widgetData(wData)
{
	widgetData.addListener(this);
	setName(name);

	
	setButtonText(buttonText);
	setAlpha(CabbageWidgetData::getNumProp(widgetData, CabbageIdentifierIds::alpha));

    const float left = CabbageWidgetData::getNumProp(widgetData, CabbageIdentifierIds::left);
    const float top = CabbageWidgetData::getNumProp(widgetData, CabbageIdentifierIds::top);
    const float width = CabbageWidgetData::getNumProp(widgetData, CabbageIdentifierIds::width);
    const float height = CabbageWidgetData::getNumProp(widgetData, CabbageIdentifierIds::height);

	
	if(!CabbageWidgetData::getNumProp(widgetData, CabbageIdentifierIds::visible))
        this->setVisible(false);


	if(CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::popuptext).isNotEmpty())
	{
		tooltipText = CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::popuptext);
		setTooltip(tooltipText);
	}

	if(CabbageWidgetData::getNumProp(widgetData, CabbageIdentifierIds::radiogroup)!=0)
		setRadioGroupId(CabbageWidgetData::getNumProp(widgetData, CabbageIdentifierIds::radiogroup));

	//set initial value if given
	if(CabbageWidgetData::getNumProp(widgetData, CabbageIdentifierIds::value)==1)
		setToggleState(true, dontSendNotification);
	else
		setToggleState(false, dontSendNotification);

	this->setWantsKeyboardFocus(false);
	

	setColour(TextButton::ColourIds::textColourOffId, Colour::fromString(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::fontcolour)));
	setColour(TextButton::ColourIds::textColourOnId, Colour::fromString(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::onfontcolour)));
	setColour(TextButton::buttonColourId, Colour::fromString(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::colour)));
	setColour(TextButton::buttonOnColourId, Colour::fromString(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::oncolour)));
	getProperties().set(CabbageIdentifierIds::shape, CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::shape).equalsIgnoreCase("square"));
	getProperties().set(CabbageIdentifierIds::corners, CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::corners));
	setBounds(left, top, width, height);
	
	initialiseCommonAttributes(wData);		
	setImgProperties(*this, wData, "buttonon");
	setImgProperties(*this, wData, "buttonoff");
	
}

//==============================================================================

//==============================================================================
void CabbageCheckbox::valueTreePropertyChanged (ValueTree& valueTree, const Identifier& prop)
{
	
	if(prop==CabbageIdentifierIds::value)
	{
		bool state = CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::value)==1 ? true : false;
		setToggleState(getValue(valueTree)==1 ? true : false, dontSendNotification);
	}

	else
	{
		handleCommonUpdates(this, valueTree);		
		
		setColour(TextButton::ColourIds::textColourOffId, Colour::fromString(CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::fontcolour)));
		setColour(TextButton::ColourIds::textColourOnId, Colour::fromString(CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::onfontcolour)));
		setColour(TextButton::buttonColourId, Colour::fromString(CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::colour)));
		setColour(TextButton::buttonOnColourId, Colour::fromString(CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::oncolour)));
		getProperties().set(CabbageIdentifierIds::shape, CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::shape).equalsIgnoreCase("square"));
		getProperties().set(CabbageIdentifierIds::corners, CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::corners));

		setButtonText(getString(valueTree, "text"));		
		setTooltip(getString(valueTree, "popuptext"));
	}
	
	repaint();	

}



