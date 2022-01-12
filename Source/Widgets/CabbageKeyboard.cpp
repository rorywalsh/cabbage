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
    outlineThickness(CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::outlinethickness)),
    lineThickness(CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::linethickness)),
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
    mouseOverOutlineColour = Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::mouseoverkeyoutlinecolour));
    
}

void CabbageKeyboard::mouseUpOnKey (int midiNoteNumber, const MouseEvent &e)
{
    repaint();
}

bool CabbageKeyboard::mouseDraggedToKey (int midiNoteNumber, const MouseEvent &e)
{
    repaint();
    return true;
}

void CabbageKeyboard::drawWhiteNote (int midiNoteNumber, Graphics& g, Rectangle<float> area,
                                           bool isDown, bool isOver, Colour lineColour, Colour textColour)
{
    auto c = Colours::transparentWhite;

    if (isDown)  c = findColour (keyDownOverlayColourId);
    if (isOver)  c = findColour (mouseOverKeyOverlayColourId);

    g.setColour (c);
    g.fillRect (area);
    
    if (isDown)
    {
        g.setColour(mouseOverOutlineColour);
        g.drawRoundedRectangle(area, 0, outlineThickness);
    }
    

    
    auto text = getWhiteNoteText (midiNoteNumber);

    if (text.isNotEmpty())
    {
        auto fontHeight = jmin (12.0f, keyWidth * 0.9f);

        g.setColour (textColour);
        g.setFont (Font (fontHeight).withHorizontalScale (0.8f));
        g.drawText (text, area.withTrimmedLeft (1.0f).withTrimmedBottom (2.0f), Justification::centredBottom, false);
    }

    if (! lineColour.isTransparent())
    {
        g.setColour (lineColour);

        g.fillRect (area.withWidth (lineThickness));

        if (midiNoteNumber == 127)
        {
            g.fillRect (area.expanded (lineThickness, 0).removeFromRight (lineThickness));
        }
    }
    

}

void CabbageKeyboard::drawBlackNote (int /*midiNoteNumber*/, Graphics& g, Rectangle<float> area,
                                           bool isDown, bool isOver, Colour noteFillColour)
{
    auto c = noteFillColour;

    if (isDown)  c = findColour (keyDownOverlayColourId);
    if (isOver)  c = findColour (mouseOverKeyOverlayColourId);

    if (isDown)
    {
        g.setColour(mouseOverOutlineColour);
        g.drawRoundedRectangle(area, 0, outlineThickness);
    }
    
    g.setColour(findColour (MidiKeyboardComponent::keySeparatorLineColourId));
    g.drawRoundedRectangle(area, 0, lineThickness);
    g.setColour (c);
    g.fillRect (area);

    if (isDown)
    {
        g.setColour (noteFillColour);
        g.drawRect (area);
    }
    else
    {
        g.setColour (c);
        auto sideIndent = 1.0f / 8.0f;
        auto topIndent = 7.0f / 8.0f;
        auto w = area.getWidth();
        auto h = area.getHeight();

        g.fillRect (area.reduced (w * sideIndent, 0).removeFromTop   (h * topIndent));
    }
}
