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

#include "CabbagePatchCables.h"
#include "../Audio/Plugins/CabbagePluginEditor.h"

CabbagePatchCables::CabbagePatchCables(ValueTree wData, CabbagePluginEditor* _owner):
Component(),
widgetData(wData),
owner(_owner)
{
    widgetData.addListener (this);
    setInterceptsMouseClicks(false, true);
    this->setWantsKeyboardFocus (false);
   
    float width = CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::width);
    float height = CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::height);
    
    originalDimensions = {CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::left)+width*.4f, CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::top)+height*.4f,
       CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::width)*.6f, CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::height)*.6f};
    setSize(300, 300);
    
    addCable({0,0});
}

void CabbagePatchCables::addCable(Point<int> position)
{
    cables.push_back({position, Point<int>(0,0)});
}

void CabbagePatchCables::setStart(Point<int> pos, String channel)
{
    cables[0].start = pos;
}

void CabbagePatchCables::setEnd(Point<int> pos, String channel)
{
    cables[0].end = pos;
//    CabbageUtilities::debug(cables[0].start.getX(), cables[0].start.getY());
//    CabbageUtilities::debug(cables[0].end.getX(), cables[0].end.getY());
}

void CabbagePatchCables::paint(Graphics& g)
{
    g.fillAll(Colours::transparentWhite);
    Path p;
    if(canDrawCable)
    {
        for(auto &cable : cables){
            g.setColour(Colours::green);
            p.startNewSubPath(cable.start.getX(), cable.start.getY());
            CabbageUtilities::debug((cable.end.getX() - cable.start.getX())*.5);
            p.quadraticTo( (cable.end.getX() - cable.start.getX())*.5 + cable.start.getX(), max(cable.end.getY(), cable.start.getY())*1.5, cable.end.getX(), cable.end.getY() );
//            g.drawLine(cable.start.getX(), cable.start.getY(), cable.end.getX(), cable.end.getY(), 10);
            g.strokePath (p, PathStrokeType(10));
        }
        
    }
    
}

void CabbagePatchCables::resizeComponent(const MouseEvent& e)
{
//
//    mousePos = e.getPosition().toFloat();
//    int newXPos = (mousePos.getX() < 0 ? owner->getMouseXYRelative().getX() : originalDimensions.getX());
//    int newYPos = (mousePos.getY() < 0 ? owner->getMouseXYRelative().getY() : originalDimensions.getY());
//    int newHeight = (newYPos < originalDimensions.getY() ? originalDimensions.getY()-owner->getMouseXYRelative().getY() : owner->getMouseXYRelative().getY()-originalDimensions.getY()) + originalDimensions.getHeight() *.5f;
//    int newWidth = (newXPos < originalDimensions.getX() ? originalDimensions.getX()- owner->getMouseXYRelative().getX() : owner->getMouseXYRelative().getX()-originalDimensions.getX()) + originalDimensions.getWidth() *.5f;
//    setBounds(newXPos, newYPos, newWidth, newHeight);
//    repaint();
}

