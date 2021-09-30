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

#include "CabbagePath.h"
#include "../Audio/Plugins/CabbagePluginEditor.h"


CabbagePath::CabbagePath (ValueTree wData, CabbagePluginEditor* _owner)
: corners (CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::corners)),
CabbageWidgetBase(_owner),
fontstyle (CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::fontstyle)),
counter (0),
text (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::text)),
colour (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::colour)),
widgetData (wData)
{
    setName (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::name));
    
    widgetData.addListener (this);              //add listener to valueTree so it gets notified when a widget's property changes
    initialiseCommonAttributes (this, wData);   //initialise common attributes such as bounds, name, rotation, etc..

}

void CabbagePath::paint (Graphics& g)
{
    g.fillAll(Colours::transparentBlack);
    g.setColour (Colour::fromString (colour));
    //g.fillRoundedRectangle (getLocalBounds().toFloat(), corners);
    
    Path path;
    path.startNewSubPath(points[0], points[1]);
    for ( int i = 2; i <= points.size()-2 ; i+=2)
    {
        path.lineTo(points[i], points[i+1]);
    }
    
    g.strokePath(path, juce::PathStrokeType(1.5));
}


void CabbagePath::valueTreePropertyChanged (ValueTree& valueTree, const Identifier& prop)
{
   
    if (colour != CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::colour))
    {
        colour = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::colour);
    }
   
    //points = CabbageWidgetData::getProperty(widgetData, CabbageIdentifierIds::points);
    handleCommonUpdates (this, valueTree, false, prop);      //handle comon updates such as bounds, alpha, rotation, visible, etc
    points = CabbageWidgetData::getProperty(widgetData, CabbageIdentifierIds::points);
    DBG(points.size());
    repaint();
    
}
