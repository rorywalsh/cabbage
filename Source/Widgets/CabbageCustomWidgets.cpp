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

#include "CabbageCustomWidgets.h"
#include "../Audio/Plugins/CabbagePluginEditor.h"


DemoCabbageWidget::DemoCabbageWidget (ValueTree wData):
    widgetData (wData)
{
    setName (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::name));
    widgetData.addListener (this);              //add listener to valueTree so it gets notified when a widget's property changes
    initialiseCommonAttributes (this, wData);   //initialise common attributes such as bounds, name, rotation, etc..

}

void DemoCabbageWidget::valueTreePropertyChanged (ValueTree& valueTree, const Identifier& prop)
{
    if (prop == CabbageIdentifierIds::value)
    {
        //set value. This is only needed for widgets that can have their value changed dioverlayRectly using a chnset
    }
    else
    {
        handleCommonUpdates (this, valueTree);      //handle common updates such as bounds, alpha, rotation, visible, etc
    }
}
//add any new custom widgets here to avoid having to edit makefiles and projects
// ===========================================================================
CabbageMeter::CabbageMeter (ValueTree wData, CabbagePluginEditor* _owner)
    : owner (_owner),
    outlineColour (Colour::fromString (CabbageWidgetData::getProperty (wData, CabbageIdentifierIds::outlinecolour).toString())),
    outlineThickness (CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::outlinethickness)),
    corners (CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::corners)),
    isVertical (CabbageWidgetData::getProperty (wData, CabbageIdentifierIds::orientation).toString() == "vertical" ? true : false),
    overlayRect (Colour::fromString (CabbageWidgetData::getProperty (wData, CabbageIdentifierIds::overlaycolour).toString())),
    widgetData (wData)
{
    setName (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::name));
    widgetData.addListener (this);              //add listener to valueTree so it gets notified when a widget's property changes
    initialiseCommonAttributes (this, wData);   //initialise common attributes such as bounds, name, rotation, etc..

    for (int i = 0; i < CabbageWidgetData::getProperty (wData, CabbageIdentifierIds::metercolour).size(); i++)
    {
        gradientColours.add (Colour::fromString (CabbageWidgetData::getProperty (wData, CabbageIdentifierIds::metercolour)[i].toString()));
    }

    if (isVertical)
    {
        ColourGradient vGradient (gradientColours[0], 0.f, 0.f, gradientColours[gradientColours.size() - 1], getWidth(), getHeight(), false);

        for (int i = 1; i < gradientColours.size() - 1; i++)
            vGradient.addColour ((float)i / (float)gradientColours.size(), gradientColours[i]);

        colourGradient = vGradient;
    }
    else
    {
        ColourGradient hGradient (gradientColours[0], 0.f, getHeight(), gradientColours[gradientColours.size() - 1], getWidth(), getHeight(), false);

        for (int i = 1; i < gradientColours.size() - 1; i++)
            hGradient.addColour ((float)i / (float)gradientColours.size(), gradientColours[i]);

        colourGradient = hGradient;
    }

    addAndMakeVisible (overlayRect);

}

void CabbageMeter::resized()
{
    overlayRect.setBounds (0, 0, getWidth(), getHeight());
    setValue (widgetData);
}

void CabbageMeter::paint (Graphics& g)
{
    g.setColour (outlineColour);
    g.fillRoundedRectangle (0, 0, getWidth(), getHeight(), corners);
    g.setGradientFill (colourGradient);
    g.fillRoundedRectangle (outlineThickness, outlineThickness, getWidth() - outlineThickness * 2, getHeight() - outlineThickness * 2, corners);
}

void CabbageMeter::setValue (ValueTree& valueTree)
{
    const float val = CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::value);

    if (isVertical)
        overlayRect.setBounds (outlineThickness, outlineThickness, getWidth() - outlineThickness * 2, (getHeight() * (1.f - val)) - outlineThickness * 2);
    else
        overlayRect.setBounds (outlineThickness + getWidth() * (val), outlineThickness, getWidth() - getWidth() * (val) - outlineThickness * 2, getHeight() - outlineThickness * 2);
}

void CabbageMeter::valueTreePropertyChanged (ValueTree& valueTree, const Identifier& prop)
{
    if (prop == CabbageIdentifierIds::value)
    {
        setValue (valueTree);
    }
    else
    {
        overlayRect.colour = Colour::fromString (CabbageWidgetData::getProperty (valueTree, CabbageIdentifierIds::overlaycolour).toString());
        handleCommonUpdates (this, valueTree);      //handle common updates such as bounds, alpha, rotation, visible, etc
    }
}
