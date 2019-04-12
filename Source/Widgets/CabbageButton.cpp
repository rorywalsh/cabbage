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
      button(),
	  popupBubble(250)
{

    widgetData.addListener (this);              //add listener to valueTree so it gets notified when a widget's property changes
    initialiseCommonAttributes (this, wData);   //initialise common attributes such as bounds, name, rotation, etc..
    button.setButtonText (getTextArray()[getValue()]);
	button.addMouseListener(this, false);

	if (CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::popuptext) == "0")
		shouldDisplayPopup = false;
	else
		shouldDisplayPopup = true;

	button.setClickingTogglesState (true);

	button.setToggleState ((bool)getValue(), dontSendNotification);

    const String radioId = CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::radiogroup);
    
    if (radioId.isNotEmpty())
    {
        owner->addToRadioComponents(&button);
        const int id = owner->radioGroups.getWithDefault(radioId, -1);
        CabbageUtilities::debug(id);
        if(id != -1)
			button.setRadioGroupId (id);
        else{
            owner->radioGroups.set(radioId, owner->radioGroups.size()+100);
			button.setRadioGroupId (owner->radioGroups.getWithDefault(radioId, -1));
            CabbageUtilities::debug(owner->radioGroups.getWithDefault(radioId, -1).toString());
        }
    }

	createPopupBubble();

    setImgProperties (*this, wData, "buttonon");
    setImgProperties (*this, wData, "buttonoff");
    setImgProperties (*this, wData, "buttonover");

	const String imgOff = CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::imgbuttonoff);
	const String imgOver = CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::imgbuttonover);
	const String imgOn = CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::imgbuttonon);

    setLookAndFeelColours (wData);
	
	button.getProperties().set("outlinecolour", CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::outlinecolour));
	button.getProperties().set("outlinethickness", CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::outlinethickness));
	button.getProperties().set("corners", CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::corners));

    if(owner->globalStyle == "legacy")
    {
        return;
    }
    
	//if users are passing custom images, use old style look and feel
	if (CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::style) == "flat" &&
		imgOff.isEmpty() && imgOn.isEmpty() && imgOver.isEmpty())
	{
		button.setLookAndFeel(&flatLookAndFeel);
	}

	addAndMakeVisible(button);
	button.setName(getName());
}

void CabbageButton::setLookAndFeelColours (ValueTree wData)
{
	button.setColour (TextButton::textColourOffId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::fontcolour)));
	button.setColour (TextButton::buttonColourId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::colour)));
	button.setColour (TextButton::textColourOnId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::onfontcolour)));
	button.setColour(TextButton::buttonOnColourId, Colour::fromString(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::oncolour)));

}

void CabbageButton::resized()
{
	button.setBounds(getLocalBounds());
}

void CabbageButton::createPopupBubble()
{
	//create popup display for showing value of sliders.
	popupBubble.setColour(BubbleComponent::backgroundColourId, Colours::white);
	popupBubble.setBounds(0, 0, 50, 20);
	owner->addChildComponent(popupBubble);
	popupBubble.setVisible(false);
	popupBubble.setAlwaysOnTop(true);
}

void CabbageButton::showPopupBubble(int time)
{
	if (getTooltipText().isNotEmpty())
		popupText = getTooltipText();

	popupBubble.showAt(&button, AttributedString(popupText), time);

}

void CabbageButton::mouseDrag(const MouseEvent& event)
{
	if (shouldDisplayPopup)
		showPopupBubble(150);
}

void CabbageButton::mouseMove(const MouseEvent& event)
{
	if (shouldDisplayPopup)
		showPopupBubble(150);
}

void CabbageButton::mouseEnter(const MouseEvent& event)
{
	if (shouldDisplayPopup)
		showPopupBubble(1000);
}

void CabbageButton::mouseExit(const MouseEvent& event)
{
	if (shouldDisplayPopup)
		popupBubble.setVisible(false);
}

void CabbageButton::valueTreePropertyChanged (ValueTree& valueTree, const Identifier& prop)
{

    if (prop == CabbageIdentifierIds::value)
    {
        CabbageUtilities::debug(CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::name));
        CabbageUtilities::debug(CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::value));
        setValue (CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::value));
		button.setToggleState (getValue() == 0 ? false : true, dontSendNotification);
		button.setButtonText (getTextArray()[getValue()]);
    }
    else
    {
        setLookAndFeelColours (valueTree);
        handleCommonUpdates (this, valueTree);      //handle comon updates such as bounds, alpha, rotation, visible, etc
        populateTextArrays (valueTree);
        //const String newText = CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::text);
        //if(newText != getTextArray()[getValue()])
        //CabbageUtilities::debug(getTextArray()[getValue()]);
		button.setButtonText (getTextArray()[getValue()]);
    }
}

