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

CabbageButton::CabbageButton (ValueTree wData)
    : widgetData (wData),
      TextButton()
{
    widgetData.addListener (this);              //add listener to valueTree so it gets notified when a widget's property changes
    initialiseCommonAttributes (this, wData);   //initialise common attributes such as bounds, name, rotation, etc..
    setButtonText (getTextArray()[getValue()]);

    setClickingTogglesState (true);

    setToggleState ((bool)getValue(), dontSendNotification);


    if (CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::radiogroup) != 0)
        setRadioGroupId (CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::radiogroup));

    setImgProperties (*this, wData, "buttonon");
    setImgProperties (*this, wData, "buttonoff");

    setLookAndFeelColours (wData);
}

void CabbageButton::setLookAndFeelColours (ValueTree wData)
{
    setColour (TextButton::textColourOffId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::fontcolour)));
    setColour (TextButton::buttonColourId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::colour)));
    setColour (TextButton::textColourOnId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::onfontcolour)));
    setColour (TextButton::buttonOnColourId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::oncolour)));
}

void CabbageButton::valueTreePropertyChanged (ValueTree& valueTree, const Identifier& prop)
{

    if (prop == CabbageIdentifierIds::value)
    {
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
        setButtonText (getTextArray()[getValue()]);
    }
}