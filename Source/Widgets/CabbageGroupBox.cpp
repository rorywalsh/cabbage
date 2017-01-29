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

#include "CabbageGroupBox.h"

CabbageGroupBox::CabbageGroupBox(ValueTree wData)
    : widgetData(wData),
      text(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::text)),
      colour(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::colour)),
      fontColour(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::fontcolour)),
      outlineColour(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::outlinecolour)),
      GroupComponent(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::name)),
      outlineThickness(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::outlinethickness)),
      corners(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::corners)),
      justification(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::align))
{
    setName(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::name));
    widgetData.addListener(this); 				//add listener to valueTree so it gets notified when a widget's property changes
    initialiseCommonAttributes(this, wData); 	//initialise common attributes such as bounds, name, rotation, etc..

    setColour(TextButton::buttonColourId, Colour::fromString(colour));
    setColour(GroupComponent::textColourId, Colour::fromString(fontColour));
    setColour(GroupComponent::outlineColourId, Colour::fromString(outlineColour));

    setName(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::name));
    setText(text);
    setWantsKeyboardFocus(false);

    setTextLabelPosition(CabbageUtilities::getJustification(justification));

    getProperties().set("outlinethickness", outlineThickness);

    getProperties().set("cornersize", corners);
    setImgProperties(*this, wData, "groupbox");
}

void CabbageGroupBox::valueTreePropertyChanged (ValueTree& valueTree, const Identifier& prop)
{
	if(DocumentWindow* owner = dynamic_cast<DocumentWindow*>(getParentComponent()))
	{
		const int visible = CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::visible);
		if(visible==1)
			owner->setVisible(true);
		else
			owner->setVisible(false);
		
	}
	
	getProperties().set("groupLine", var(CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::linethickness)));
    getProperties().set("cornersize", CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::corners));
    setTextLabelPosition(CabbageUtilities::getJustification(CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::align)));
    setColour(TextButton::buttonColourId, Colour::fromString(CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::colour)));
    setColour(GroupComponent::textColourId, Colour::fromString(CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::fontcolour)));
    setColour(GroupComponent::outlineColourId, Colour::fromString(CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::outlinecolour)));
    setText(CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::text));
    getProperties().set("cornersize", CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::corners));
    getProperties().set("outlinethickness", CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::outlinethickness));
    handleCommonUpdates(this, valueTree);		//handle comon updates such as bounds, alpha, rotation, visible, etc
}

