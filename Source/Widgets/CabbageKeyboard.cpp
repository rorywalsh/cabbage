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

#include "CabbageKeyboard.h"

CabbageKeyboard::CabbageKeyboard (ValueTree wData, CabbagePluginEditor* _owner, MidiKeyboardState& state)
    : MidiKeyboardComponent (state, MidiKeyboardComponent::horizontalKeyboard),
    scrollbars (CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::scrollbars)),
    keyWidth (CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::keywidth)),
    widgetData (wData),
    CabbageWidgetBase(_owner)
{
    setOrientation (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::kind) == "horizontal" ? MidiKeyboardComponent::horizontalKeyboard : MidiKeyboardComponent::verticalKeyboardFacingRight);
    setName (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::name));
    widgetData.addListener (this);              //add listener to valueTree so it gets notified when a widget's property changes
    initialiseCommonAttributes (this, wData);   //initialise common attributes such as bounds, name, rotation, etc..

    setLowestVisibleKey (CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::value));
    setOctaveForMiddleC (CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::middlec));
    setKeyPressBaseOctave (CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::keypressbaseoctave)); // octave num. in [0, 10]
    setKeyWidth (keyWidth);
    setScrollButtonsVisible (scrollbars == 1 ? true : false);
    setWantsKeyboardFocus (true);
    setMouseClickGrabsKeyboardFocus (true);
    updateColours(wData);

}

void CabbageKeyboard::valueTreePropertyChanged (ValueTree& valueTree, const Identifier& prop)
{
    setOrientation (CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::kind) == "horizontal" ? MidiKeyboardComponent::horizontalKeyboard : MidiKeyboardComponent::verticalKeyboardFacingRight);
    const int width = CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::keywidth);
    setKeyWidth(jmax(1, width));
    updateColours(valueTree);
    handleCommonUpdates (this, valueTree, false, prop);      //handle comon updates such as bounds, alpha, rotation, visible, etc

}


void CabbageKeyboard::updateColours(ValueTree& wData)
{
    setColour (MidiKeyboardComponent::whiteNoteColourId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::whitenotecolour)));
    setColour (MidiKeyboardComponent::blackNoteColourId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::blacknotecolour)));
    setColour (MidiKeyboardComponent::upDownButtonArrowColourId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::arrowcolour)));
    setColour (MidiKeyboardComponent::upDownButtonBackgroundColourId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::arrowbackgroundcolour)));
    setColour (MidiKeyboardComponent::keySeparatorLineColourId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::keyseparatorcolour)));
    setColour (MidiKeyboardComponent::mouseOverKeyOverlayColourId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::mouseoverkeycolour)));
    setColour (MidiKeyboardComponent::keyDownOverlayColourId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::keydowncolour)));
}
