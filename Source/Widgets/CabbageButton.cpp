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

#include "CabbageButton.h"


CabbageButton::CabbageButton (ValueTree wData, CabbagePluginEditor* _owner)
    : widgetData (wData),
      owner(_owner),
      TextButton()
{
    widgetData.addListener (this);              //add listener to valueTree so it gets notified when a widget's property changes
    initialiseCommonAttributes (this, wData);   //initialise common attributes such as bounds, name, rotation, etc..
    setButtonText (getTextArray()[getValue()]);

    setClickingTogglesState (true);

    setToggleState ((bool)getValue(), dontSendNotification);

    const String radioId = CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::radiogroup);
    
    if (radioId.isNotEmpty())
    {
        const int id = owner->radioGroups.getWithDefault(radioId, -1);
        CabbageUtilities::debug(id);
        if(id != -1)
            setRadioGroupId (id);
        else{
            owner->radioGroups.set(radioId, owner->radioGroups.size()+100);
            setRadioGroupId (owner->radioGroups.getWithDefault(radioId, -1));
            CabbageUtilities::debug(owner->radioGroups.getWithDefault(radioId, -1).toString());
        }
    }



    setImgProperties (*this, wData, "buttonon");
    setImgProperties (*this, wData, "buttonoff");
    setImgProperties (*this, wData, "buttonover");

	const String imgOff = CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::imgbuttonoff);
	const String imgOver = CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::imgbuttonover);
	const String imgOn = CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::imgbuttonon);

    setLookAndFeelColours (wData);
	
	getProperties().set("outlinecolour", CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::outlinecolour));
	getProperties().set("outlinethickness", CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::outlinethickness));
	getProperties().set("corners", CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::corners));

    if(owner->globalStyle == "legacy")
    {
        return;
    }
    
	//if users are passing custom images, use old style look and feel
	if (CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::style) == "flat" &&
		imgOff.isEmpty() && imgOn.isEmpty() && imgOver.isEmpty())
	{
		setLookAndFeel(&flatLookAndFeel);
	}


}

void CabbageButton::setLookAndFeelColours (ValueTree wData)
{
    setColour (TextButton::textColourOffId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::fontcolour)));
    setColour (TextButton::buttonColourId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::colour)));
    setColour (TextButton::textColourOnId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::onfontcolour)));
	setColour(TextButton::buttonOnColourId, Colour::fromString(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::oncolour)));

}

void CabbageButton::valueTreePropertyChanged (ValueTree& valueTree, const Identifier& prop)
{

    if (prop == CabbageIdentifierIds::value)
    {
        CabbageUtilities::debug(CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::name));
        CabbageUtilities::debug(CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::value));
        setValue (CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::value));
        setToggleState (getValue() == 0 ? false : true, dontSendNotification);
        setButtonText (getTextArray()[getValue()]);
    }
    else
    {
        setLookAndFeelColours (valueTree);
        handleCommonUpdates (this, valueTree);      //handle comon updates such as bounds, alpha, rotation, visible, etc
        populateTextArrays (valueTree);
        //const String newText = CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::text);
        //if(newText != getTextArray()[getValue()])
        //CabbageUtilities::debug(getTextArray()[getValue()]);
        setButtonText (getTextArray()[getValue()]);
    }
}

