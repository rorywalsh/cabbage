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

#include "CabbageGroupBox.h"

CabbageGroupBox::CabbageGroupBox(ValueTree wData)
	: widgetData(wData),
	text(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::text)),
	colour(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::colour)),
	left(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::left)),
	top(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::top)),
	fontcolour(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::fontcolour)),
	GroupComponent(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::name)),
	line(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::linethickness)),
	rotate(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::rotate)),
	pivotx(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::pivotx)),
	pivoty(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::pivoty)),
	corners(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::corners))
{
	setName(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::name));
	widgetData.addListener(this); 				//add listener to valueTree so it gets notified when a widget's property changes
	initialiseCommonAttributes(this, wData); 	//initialise common attributes such as bounds, name, rotation, etc..	
	
	setColour(TextButton::buttonColourId, Colour::fromString(colour));
	setName(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::name));
	setColour(GroupComponent::textColourId, Colour::fromString(fontcolour));

    setText(text);
    setWantsKeyboardFocus(false);
	
	if(line==0)
		this->getProperties().set("groupLine", var(0));
	else
		this->getProperties().set("groupLine", line);
	
	getProperties().set("cornersize", corners);
	setImgProperties(*this, wData, "groupbox");
}

void CabbageGroupBox::valueTreePropertyChanged (ValueTree& valueTree, const Identifier& prop)
{
	setColour(TextButton::buttonColourId, Colour::fromString(colour));
	setColour(GroupComponent::textColourId, Colour::fromString(fontcolour));	

	getProperties().set("cornersize", CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::corners));
	handleCommonUpdates(this, valueTree);		//handle comon updates such as bounds, alpha, rotation, visible, etc	
}