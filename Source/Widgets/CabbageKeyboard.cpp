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
    corners (CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::corners)),
    keyWidth (CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::keywidth)),
    outlineThickness(CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::outlinethickness)),
    lineThickness(CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::linethickness)),
    blackNoteLength(CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::blacknotelength)),
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
    setColour (MidiKeyboardComponent::shadowColourId, Colours::transparentBlack);
    mouseOverOutlineColour = Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::mousedownkeyoutlinecolour));
    
    setBlackNoteLengthProportion(blackNoteLength);
    //setBlackNoteWidthProportion(.5f);
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

const String CabbageKeyboard::getNoteOutline(int midiNote, Rectangle<float> area)
{
    String svgViewBox = "<svg viewBox=\"0 0 " + String(area.getWidth()) + " " + String(area.getHeight()) + "\">";
    if(midiNote == 29)
    {
        return svgViewBox + "<path d=\"M14 1H48V168C48 170.8 51.3333 172.167 53 172.5H59.5L60 325.5H13.5C6.3 325.5 2.16667 317.833 1 314V11C1 4 7 1 14 1Z\" stroke=\"#F8FDFF\" fill-opacity=\"0.0\" stroke-width=\"2\"/>" + "</svg>";
    }
    
    switch(midiNote % 12)
    {
        case 2:
        case 7:
        case 9:
            return svgViewBox + "<path d=\"M12.5 167.5V1H47.5V166.5C47.5 169.3 50.1667 172 51.5 172.5H59.5V324.5H1V172.5H9C11.4 172.5 12.1667 168.833 12.5 167.5Z\" stroke=\"#F8FDFF\" fill-opacity=\"0.0\" stroke-width=\"2\"/>\n" + "</svg>";
        case 4:
        case 11:
            return svgViewBox +  "<path d=\"M60 1H12.5V168C12.5 171.2 9.83333 172.333 8.5 172.5H1V325.5H60.5L60 1Z\" stroke=\"#F8FDFF\" fill-opacity=\"0.0\" stroke-width=\"2\"/>\n" + "</svg>";
            
        case 0:
        case 5:
            return svgViewBox + "<path d=\"M61 325.5H1.5V1H49V168C49 171.6 52.3333 172.333 54 172.5H61V325.5Z\" stroke=\"#F8FDFF\" fill-opacity=\"0.0\" stroke-width=\"2\"/>" + "</svg>";
        default:
            return svgViewBox + "<path d=\"M25 1H1.5V168C1.5 170.4 4.5 171.667 6 172H19C23 172 24.6667 169.333 25 168V1Z\" stroke=\"#F8FDFF\" fill-opacity=\"0.0\" stroke-width=\"2\"/>\n" + "</svg>";

    }
}


void CabbageKeyboard::drawNoteOutline(Graphics& g, int midiNote, Rectangle<float> area)
{
    area = area.withX(area.getX()+(midiNote/29.f-1)*6.f);
    if(getNoteOutline(midiNote, area).isNotEmpty())
    {
        std::unique_ptr<XmlElement> svgElement(XmlDocument::parse(getNoteOutline(midiNote, area)));
        if(svgElement != nullptr)
        {
            g.fillAll(Colours::transparentBlack);
            outlineDrawable = Drawable::createFromSVG(*svgElement);
            outlineDrawable->setTransformToFit(area, RectanglePlacement::stretchToFit);
            outlineDrawable->draw(g, 1.f, {});
        }
    }
    
//    const float w = area.getWidth();
//    const float h = area.getHeight();
//    const float x = area.getX()+outlineThickness/2.f;
//    const float y = area.getY();
//
//    Path p;
//    p.startNewSubPath(x, y+area.getHeight()-outlineThickness/2.f);
//    switch(midiNote % 12)
//    {
//        case 0:
//            p.lineTo (x + w, y + h - outlineThickness/2.f);
//            p.lineTo (x + w, y + blackNoteArea.getHeight()+outlineThickness/2.f);
//            p.lineTo (x + w*.53f, y + blackNoteArea.getHeight()+outlineThickness/2.f);
//            p.lineTo (x + w*.53f, y);
//            p.lineTo (x, y);
//            p.lineTo (x, y+area.getHeight());
//            break;
//        case 2:
//            p.lineTo (x + w, y + h - outlineThickness/2.f);
//            p.lineTo (x + w, y + blackNoteArea.getHeight()+outlineThickness/2.f);
//            p.lineTo (x + w*.63f, y + blackNoteArea.getHeight()+outlineThickness/2.f);
//            p.lineTo (x + w*.63f, y);
//            p.lineTo (x + w*.31f, y);
//            p.lineTo (x + w*.31f, + blackNoteArea.getHeight()+outlineThickness/2.f);
//            p.lineTo (x, y + blackNoteArea.getHeight()+outlineThickness/2.f);
//            p.lineTo (x, y+area.getHeight());
//            break;
//        case 4:
//            p.lineTo (x + w, y + h - outlineThickness/2.f);
//            p.lineTo (x + w, y);
//            p.lineTo (x + w*.44f, y);
//            p.lineTo (x + w*.44f, y + blackNoteArea.getHeight()+outlineThickness/2.f);
//            p.lineTo (x, y + blackNoteArea.getHeight()+outlineThickness/2.f);
//            p.lineTo (x, y + area.getHeight() - outlineThickness/2.f);
//            break;
//        case 5:
//            p.lineTo (x + w, y + h - outlineThickness/2.f);
//            p.lineTo (x + w, y + blackNoteArea.getHeight()+outlineThickness/2.f);
//            p.lineTo (x + w*.47f, y + blackNoteArea.getHeight()+outlineThickness/2.f);
//            p.lineTo (x + w*.47f, y);
//            p.lineTo (x, y);
//            p.lineTo (x, y+area.getHeight() - outlineThickness/2.f);
//            break;
//        case 7:
//            p.lineTo (x + w, y + h - outlineThickness/2.f);
//            p.lineTo (x + w, y + blackNoteArea.getHeight()+outlineThickness/2.f);
//            p.lineTo (x + w*.59f, y + blackNoteArea.getHeight()+outlineThickness/2.f);
//            p.lineTo (x + w*.59f, y);
//            p.lineTo (x + w*.21f, y);
//            p.lineTo (x + w*.21f, y + blackNoteArea.getHeight()+outlineThickness/2.f);
//            p.lineTo (x, y + blackNoteArea.getHeight()+outlineThickness/2.f);
//            p.lineTo (x, y + area.getHeight() - outlineThickness/2.f);
//            break;
//        case 9:
//            p.lineTo (x + w, y + h - outlineThickness/2.f);
//            p.lineTo (x + w, y + blackNoteArea.getHeight()+outlineThickness/2.f);
//            p.lineTo (x + w*.69f, y + blackNoteArea.getHeight()+outlineThickness/2.f);
//            p.lineTo (x + w*.69f, y);
//            p.lineTo (x + w*.35f, y);
//            p.lineTo (x + w*.35f, y + blackNoteArea.getHeight()+outlineThickness/2.f);
//            p.lineTo (x, y + blackNoteArea.getHeight()+outlineThickness/2.f);
//            p.lineTo (x, y+area.getHeight() - outlineThickness/2.f);
//            break;
//        case 11:
//            p.lineTo (x + w, y + h - outlineThickness/2.f);
//            p.lineTo (x + w, y);
//            p.lineTo (x + w*.49f, y);
//            p.lineTo (x + w*.49f, y + blackNoteArea.getHeight()+outlineThickness/2.f);
//            p.lineTo (x, y + blackNoteArea.getHeight()+outlineThickness/2.f);
//            p.lineTo (x, y+area.getHeight() - outlineThickness/2.f);
//            break;
//
//    }
//    g.strokePath(p, PathStrokeType(outlineThickness));
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
        if(outlineThickness>0)
        {
            drawNoteOutline(g, midiNoteNumber, area);
        }
        else
            g.drawRoundedRectangle(area, corners, outlineThickness);
        //g.setColour(mouseOverOutlineColour);
        //
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


void CabbageKeyboard::drawBlackNote (int midiNoteNumber, Graphics& g, Rectangle<float> area,
                                           bool isDown, bool isOver, Colour noteFillColour)
{
    auto c = noteFillColour;

    
    if (isDown)
    {
        c = findColour (keyDownOverlayColourId).getAlpha() == 0.f ? noteFillColour : findColour (keyDownOverlayColourId);
        g.setColour(mouseOverOutlineColour);
        if(outlineThickness>0)
        {
            Rectangle<float> nextWhiteNote = getRectangleForKey(midiNoteNumber+1);
            
            drawNoteOutline(g, midiNoteNumber, area.withWidth(nextWhiteNote.getWidth()*.4f).withX(nextWhiteNote.getX()-nextWhiteNote.getWidth()*.2f));
        }
    }
    
    if (isOver)  c = findColour (mouseOverKeyOverlayColourId).getAlpha() == 0.f ? noteFillColour : findColour (mouseOverKeyOverlayColourId);


        
    g.setColour(findColour (MidiKeyboardComponent::keySeparatorLineColourId));
    g.drawRoundedRectangle(area, 0, lineThickness);
    g.setColour (c);
    g.fillRect (area);

    if (isDown)
    {
        g.setColour(mouseOverOutlineColour);
        g.drawRoundedRectangle (area, 0, outlineThickness);
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
