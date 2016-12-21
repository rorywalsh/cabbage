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

#include "CabbageRangeSlider.h"

CabbageRangeSlider::CabbageRangeSlider(ValueTree wData):
widgetData(wData), mouseDragBetweenThumbs {false}
{
	setName(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::name));
	widgetData.addListener(this); 				//add listener to valueTree so it gets notified when a widget's property changes
	initialiseCommonAttributes(this, wData); 	//initialise common attributes such as bounds, name, rotation, etc..	
	setSliderStyle (Slider::TwoValueHorizontal);
	setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	setMinValue(0);
	setMaxValue(100);
	setRange(0, 100, .01);
	setLookAndFeel(&lookAndFeel);
}

	
	
void CabbageRangeSlider::valueTreePropertyChanged (ValueTree& valueTree, const Identifier& prop)
{
	if(prop==CabbageIdentifierIds::value)
    {
		//set value. This is only needed for widgets that can have their value changed directly using a chnset
    }
	else
	{

		
		
		handleCommonUpdates(this, valueTree);		//handle comon updates such as bounds, alpha, rotation, visible, etc	
	}
}

// To enable the section between the two thumbs to be draggable.
void CabbageRangeSlider::mouseDown (const MouseEvent& event)
{
    const float currentMouseX = event.getPosition().getX();
    const int thumbRadius = getLookAndFeel().getSliderThumbRadius (*this);
    xMinAtThumbDown = valueToProportionOfLength (getMinValue()) * getWidth();
    xMaxAtThumbDown = valueToProportionOfLength (getMaxValue()) * getWidth();

    if (currentMouseX > xMinAtThumbDown + thumbRadius && currentMouseX < xMaxAtThumbDown - thumbRadius)
    {
        mouseDragBetweenThumbs = true;
    }
    else
    {
        mouseDragBetweenThumbs = false;
        Slider::mouseDown (event);
    }
}

// To enable the section between the two thumbs to be draggable.
void CabbageRangeSlider::mouseDrag (const MouseEvent& event)
{
    const float distanceFromStart = event.getDistanceFromDragStartX();
    
    if (mouseDragBetweenThumbs)
    {        
        setMinValue (proportionOfLengthToValue ((xMinAtThumbDown + distanceFromStart) / getWidth()));   
        setMaxValue (proportionOfLengthToValue ((xMaxAtThumbDown + distanceFromStart) / getWidth()));    
    }
    else
    {
        Slider::mouseDrag (event);
    }  
}

// This makes one thumb slide if the other is moved against it.
void CabbageRangeSlider::valueChanged()
{   
    if (getMinValue() == getMaxValue())
    {
        const int minimalIntervalBetweenMinAndMax = 1;
        if (getMaxValue() + minimalIntervalBetweenMinAndMax <= getMaximum())
        {
            setMaxValue(getMaxValue() + minimalIntervalBetweenMinAndMax);
        }
        else
        {
            setMinValue(getMinValue() - minimalIntervalBetweenMinAndMax);
        }
    }
}
