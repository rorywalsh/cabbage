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
    blackNoteWidth(CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::blacknotewidth)),
    widgetData (wData),
    CabbageWidgetBase(_owner)
{
    setOrientation (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::kind) == "horizontal" ? MidiKeyboardComponent::horizontalKeyboard : MidiKeyboardComponent::verticalKeyboardFacingRight);
    setName (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::name));
    widgetData.addListener (this);              //add listener to valueTree so it gets notified when a widget's property changes
    initialiseCommonAttributes (this, wData);   //initialise common attributes such as bounds, name, rotation, etc..


    
    

    setOctaveForMiddleC (CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::middlec));
    setKeyPressBaseOctave (CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::keypressbaseoctave)); // octave num. in [0, 10]
    setKeyWidth (keyWidth);
    setScrollButtonsVisible (scrollbars == 1 ? true : false);
    setWantsKeyboardFocus (true);
    setMouseClickGrabsKeyboardFocus (true);
    updateColours(wData);
    
    if(scrollbars == 1)
    setLowestVisibleKey (CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::value));
    else
        setAvailableRange(CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::value), 127);
    
//    if(CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::scrollbars) == 0)
//        setScrollButtonWidth(0);
}

CabbageKeyboard::Direction CabbageKeyboard::getNextDirection(std::vector<PathPoint> points, std::size_t segment)
{
    if ( points.size() < 2)
        return Direction::undefined;
    
    if ( points[segment+1].x > points[segment].x)
        return Direction::right;
    
    if ( points[segment+1].x < points[segment].x)
        return Direction::left;
    
    if ( points[segment+1].y < points[segment].y)
        return Direction::up;
    
    if ( points[segment+1].y > points[segment].y)
        return Direction::down;
    
    return Direction::undefined;
}

void CabbageKeyboard::drawRoundedPath(Graphics& g, std::vector<PathPoint> points, float lineThickness)
{
    juce::Path p;
    prevDirection = getNextDirection(points, 0);
    p.startNewSubPath(points[0].x+points[0].corners, points[0].y);
    for( std::size_t i = 1 ; i < points.size() ; i++)
    {
        if(getNextDirection(points, i) == Direction::down)
        {
            switch(prevDirection)
            {
                case Direction::right:
                    p.lineTo(points[i].x-points[i].corners, points[i].y);
                    p.cubicTo(points[i].x, points[i].y, points[i].x, points[i].y, points[i].x, points[i].y+points[i].corners);
                    break;
                case Direction::left:
                    p.lineTo(points[i].x+points[i].corners, points[i].y);
                    p.cubicTo(points[i].x, points[i].y, points[i].x, points[i].y, points[i].x, points[i].y+points[i].corners);
                    break;
                default:
                    break;
            }
            prevDirection = Direction::down;
        }
        else if(getNextDirection(points, i) == Direction::up)
        {
            switch(prevDirection)
            {
                case Direction::right:
                    p.lineTo(points[i].x-points[i].corners, points[i].y);
                    p.cubicTo(points[i].x, points[i].y, points[i].x, points[i].y, points[i].x, points[i].y-points[i].corners);
                    break;
                case Direction::left:
                    p.lineTo(points[i].x+points[i].corners, points[i].y);
                    p.cubicTo(points[i].x, points[i].y, points[i].x, points[i].y, points[i].x, points[i].y-points[i].corners);
                    break;
                default:
                    break;
            }
            prevDirection = Direction::up;
        }
        if(getNextDirection(points, i) == Direction::right)
        {
            switch(prevDirection)
            {
                case Direction::up:
                    p.lineTo(points[i].x, points[i].y+points[i].corners);
                    p.cubicTo(points[i].x, points[i].y, points[i].x, points[i].y, points[i].x+points[i].corners, points[i].y);
                    break;
                case Direction::down:
                    p.lineTo(points[i].x, points[i].y-points[i].corners);
                    p.cubicTo(points[i].x, points[i].y, points[i].x, points[i].y, points[i].x+points[i].corners, points[i].y);
                    break;
                default:
                    break;
            }
            prevDirection = Direction::right;
        }
        if(getNextDirection(points, i) == Direction::left)//moving left
        {
            switch(prevDirection)
            {
                case Direction::down:
                    p.lineTo(points[i].x, points[i].y-points[i].corners);
                    p.cubicTo(points[i].x, points[i].y, points[i].x, points[i].y, points[i].x-points[i].corners, points[i].y);
                    break;
                case Direction::up:
                    p.lineTo(points[i].x, points[i].y+points[i].corners);
                    p.cubicTo(points[i].x, points[i].y, points[i].x, points[i].y, points[i].x-points[i].corners, points[i].y);
                    break;
                    
                default:
                    break;
            }
            prevDirection = Direction::left;
        }
    }

    g.strokePath(p, juce::PathStrokeType(lineThickness));
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
    setColour (MidiKeyboardComponent::textLabelColourId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::fontcolour)));
    
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


void CabbageKeyboard::drawNoteOutline(Graphics& g, int midiNote, Rectangle<float> area)
{
    std::vector<PathPoint> points;
    auto blackNote = getRectangleForKey(midiNote + (midiNote % 12 == 11 ? -1 : 1));

    Rectangle<float> nextBlackNoteArea = area.withHeight(getBlackNoteLength()).withWidth(getKeyWidth()*blackNoteWidth).withCentre(blackNote.getCentre().withX(area.getX()+area.getWidth()));
    
    //getLowestVisibleKey();

    if(midiNote == 53)  //first note
    {
        points.push_back({area.getX()+10, area.getY(), 0});
        points.push_back({nextBlackNoteArea.getX(), area.getY(), 0});
        points.push_back({nextBlackNoteArea.getX(), nextBlackNoteArea.getY()+nextBlackNoteArea.getHeight(), 10});
        points.push_back({area.getX()+area.getWidth(), nextBlackNoteArea.getY()+nextBlackNoteArea.getHeight(), 0});
        points.push_back({area.getX()+area.getWidth(), area.getY()+area.getHeight(), 0});
        points.push_back({area.getX(), area.getY()+area.getHeight(), 15});
        points.push_back({area.getX(), area.getY(), 15});
        points.push_back({area.getX()+20, area.getY(), 0});
    }
    else if(midiNote == 83)  //last note
    {
        blackNote = getRectangleForKey(midiNote - 1);
        nextBlackNoteArea = area.withHeight(getBlackNoteLength()).withWidth(getKeyWidth()*blackNoteWidth).withCentre(blackNote.getCentre().withX(area.getX()+area.getWidth()));
        points.push_back({area.getX()+nextBlackNoteArea.getWidth()/2.f, area.getY(), 0});
        points.push_back({area.getWidth()+area.getX(), area.getY(), 15});
        points.push_back({area.getWidth()+area.getX(), area.getY()+area.getHeight(), 15});
        points.push_back({area.getX(), area.getY()+area.getHeight(), 0}); // bottom left
        points.push_back({area.getX(), nextBlackNoteArea.getY()+nextBlackNoteArea.getHeight(), 0});
        points.push_back({area.getX()+nextBlackNoteArea.getWidth()/2.f, nextBlackNoteArea.getY()+nextBlackNoteArea.getHeight(), 10});
        points.push_back({area.getX()+nextBlackNoteArea.getWidth()/2.f, area.getY(), 0});
        points.push_back({(area.getX()+nextBlackNoteArea.getWidth()/2.f) + 10, area.getY(), 0});
    }
    else
    {
        switch(midiNote % 12)
        {
            case 2:
            case 7:
            case 9:
                points.push_back({area.getX()+nextBlackNoteArea.getWidth()/2.f, area.getY(), 0});
                points.push_back({nextBlackNoteArea.getX(), area.getY(), 0});
                points.push_back({nextBlackNoteArea.getX(), nextBlackNoteArea.getY()+nextBlackNoteArea.getHeight(), 10});
                points.push_back({area.getX()+area.getWidth(), nextBlackNoteArea.getY()+nextBlackNoteArea.getHeight(), 0});
                points.push_back({area.getX()+area.getWidth(), area.getY()+area.getHeight(), 0}); // bottom right
                points.push_back({area.getX(), area.getY()+area.getHeight(), 0}); // bottom left
                points.push_back({area.getX(), nextBlackNoteArea.getY()+nextBlackNoteArea.getHeight(), 0});
                points.push_back({area.getX()+nextBlackNoteArea.getWidth()/2.f, nextBlackNoteArea.getY()+nextBlackNoteArea.getHeight(), 10});
                points.push_back({area.getX()+nextBlackNoteArea.getWidth()/2.f, area.getY(), 0});
                points.push_back({area.getX()+30, area.getY(), 0});
                break;
            case 4:
            case 11:
                blackNote = getRectangleForKey(midiNote - 1);
                nextBlackNoteArea = area.withHeight(getBlackNoteLength()).withWidth(getKeyWidth()*blackNoteWidth).withCentre(blackNote.getCentre().withX(area.getX()+area.getWidth()));
                points.push_back({area.getX()+nextBlackNoteArea.getWidth()/2.f, area.getY(), 0});
                points.push_back({area.getWidth()+area.getX(), area.getY(), 0});
                points.push_back({area.getWidth()+area.getX(), area.getY()+area.getHeight(), 0});
                points.push_back({area.getX(), area.getY()+area.getHeight(), 0}); // bottom left
                points.push_back({area.getX(), nextBlackNoteArea.getY()+nextBlackNoteArea.getHeight(), 0});
                points.push_back({area.getX()+nextBlackNoteArea.getWidth()/2.f, nextBlackNoteArea.getY()+nextBlackNoteArea.getHeight(), 10});
                points.push_back({area.getX()+nextBlackNoteArea.getWidth()/2.f, area.getY(), 0});
                points.push_back({(area.getX()+nextBlackNoteArea.getWidth()/2.f) + 10, area.getY(), 0});
                break;
            case 0:
            case 5:
                points.push_back({area.getX(), area.getY(), 0});
                points.push_back({nextBlackNoteArea.getX(), area.getY(), 0});
                points.push_back({nextBlackNoteArea.getX(), nextBlackNoteArea.getY()+nextBlackNoteArea.getHeight(), 10});
                points.push_back({area.getX()+area.getWidth(), nextBlackNoteArea.getY()+nextBlackNoteArea.getHeight(), 0});
                points.push_back({area.getX()+area.getWidth(), area.getY()+area.getHeight(), 0});
                points.push_back({area.getX(), area.getY()+area.getHeight(), 0});
                points.push_back({area.getX(), area.getY(), 0});
                points.push_back({area.getX()+10, area.getY(), 0});
                break;
                
            default:
                points.push_back({area.getX(), area.getY(), 0});
                points.push_back({area.getX()+area.getWidth(), area.getY(), 0});
                points.push_back({area.getX()+area.getWidth(), area.getY()+area.getHeight(), 10});
                points.push_back({area.getX(), area.getY()+area.getHeight(), 10});
                points.push_back({area.getX(), area.getY(), 0});
                
        }
    }
    drawRoundedPath(g, points, 2);
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
            //g.drawRect(area);
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

    
    if (isOver)  c = findColour (mouseOverKeyOverlayColourId).getAlpha() == 0.f ? noteFillColour : findColour (mouseOverKeyOverlayColourId);


        
    g.setColour(findColour (MidiKeyboardComponent::keySeparatorLineColourId));
    g.drawRoundedRectangle(area, 0, lineThickness);
    g.setColour (c);
    g.fillRect (area);

    if (isDown)
    {
        c = findColour (keyDownOverlayColourId).getAlpha() == 0.f ? noteFillColour : findColour (keyDownOverlayColourId);
        g.setColour(mouseOverOutlineColour);
        if(outlineThickness>0)
        {
            auto nextNote = getRectangleForKey(midiNoteNumber+1);
            Rectangle<float> nextBlackNoteArea = area.withWidth(getKeyWidth()*blackNoteWidth).withX(nextNote.getX() - (getKeyWidth()*blackNoteWidth)/2.f);
            drawNoteOutline(g, midiNoteNumber, nextBlackNoteArea);
        }
        else
        {
            g.setColour(mouseOverOutlineColour);
            g.drawRoundedRectangle (area, 0, outlineThickness);
        }
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
