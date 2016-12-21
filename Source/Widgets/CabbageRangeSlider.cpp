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
widgetData(wData),
slider(wData)
{
	setName(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::name));
	widgetData.addListener(this); 				//add listener to valueTree so it gets notified when a widget's property changes
	initialiseCommonAttributes(this, wData); 	//initialise common attributes such as bounds, name, rotation, etc..	

    isVertical = CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::kind)=="horizontal" ? false : true;
    //CabbageUtilities::debug(cAttr.getNumProp(CabbageIDs::sliderskew));

    addAndMakeVisible(&slider);
    slider.setBounds(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::left), 
					 CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::top),
                     CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::width), 
					 CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::height));

    initVal1 = CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::minvalue);
    initVal2 = CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::maxvalue);
    min = CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::min);
    max = CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::max);

    const float value1 = jmap(initVal1, min, max, 0.f, 1.f);
    const float value2 = jmap(initVal2, min, max, 0.f, 1.f);

    slider.setValue(value1, value2);

    textLabel.setColour(Label::textColourId, Colour::fromString(textColour));
    textLabel.setColour(Label::outlineColourId, Colours::transparentBlack);
    addAndMakeVisible(&textLabel);
    textLabel.setVisible(false);
}

void CabbageRangeSlider::resized()
{
    if(text.isNotEmpty())
    {
        if(isVertical)
        {
            textLabel.setBounds(0, getHeight()-20, getWidth(), 20);
            textLabel.setJustificationType(Justification::centred);
            textLabel.setText(text, dontSendNotification);
            textLabel.setVisible(true);
            slider.setBounds(0, 4, getWidth(), getHeight()-30);
        }
        else
        {
            float width = textLabel.getFont().getStringWidthFloat(text)+10.f;
            textLabel.setBounds(0, 0, width, getHeight());
            textLabel.setText(text, dontSendNotification);
            textLabel.setVisible(true);
            slider.setBounds(width, 0, getWidth()-(width*1.10), getHeight());
        }

        const float value1 = jmap(initVal1, min, max, 0.f, 1.f);
        const float value2 = jmap(initVal2, min, max, 0.f, 1.f);
        slider.setValue(value1, value2);
    }
    else if(isVertical)
        slider.setBounds(getLocalBounds().withTop(3).withHeight(getHeight()-8));
    else
        slider.setBounds(getLocalBounds().withLeft(3).withWidth(getWidth()-8));
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

//==============================================================================================
// RangeSlider2, homegrown version of JUCE's two value slider, with dragable range
//==============================================================================================
RangeSlider::RangeSlider(ValueTree wData)
    : Component(),
      name(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::name)),
      isVertical(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::kind)=="horizontal" ? false : true),
	  sliderThumb(isVertical),
      currentThumb(0),
      thumbWidth(10),
      thumbHeight(10),
      tooltipText(String::empty),
      shouldDisplayPopup(true),
      initVal(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::value)),
      max(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::max)),
      min(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::min)),
      maxVal(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::maxvalue)),
      minVal(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::minvalue)),
      incr(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::sliderincr)),
      width(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::width)),
      height(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::height)),
      skew(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::sliderskew)),
      colour(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::colour)),
      trackerThickness(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::trackerthickness)),
      trackerColour(Colour::fromString(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::trackercolour))),
      index(-99)
{
    var channelArray = CabbageWidgetData::getProperty(wData, CabbageIdentifierIds::channel);
	channels.add(channelArray[0].toString());
	channels.add(channelArray[1].toString());


    sliderThumb.setColour(Colour::fromString(colour));

#ifdef Cabbage_Build_Standalone
    sendValuesToCsound(minVal, maxVal, 3);
#endif
    addAndMakeVisible(&sliderThumb);

}

void RangeSlider::resized()
{
    thumbIncr = (incr/(max-min))*getWidth();
    sliderThumb.setBounds(0, 0, getWidth(), getHeight());
    sliderThumbBounds = getLocalBounds();
}

int RangeSlider::getSliderPosition(SliderType type)
{
    if(type == SliderType::left)
    {
        const double proportion = abs((minVal- (minVal<0 ? min : 0))/(max-min));
        return (getWidth()*proportion)- (minVal<0 ? thumbWidth : 0);
    }
    else if(type == SliderType::right)
    {
        const double proportion = abs((maxVal-minVal)/(max-min));
        int newWidth = getWidth()*proportion;
        return newWidth + (minVal < 0 ? thumbWidth : 0);
    }
    else if(type == SliderType::top)
    {
        const double proportion = abs((maxVal-max)/(max-min));
        return (getHeight()*proportion);
    }
    else if(type == SliderType::bottom)
    {
        const double newWidth = abs((maxVal-minVal)/(max-min));
        return (getHeight()*newWidth)+ (minVal<0 ? thumbHeight : 0);
    }
}

void RangeSlider::mouseDown(const MouseEvent& event)
{
    if(getThumbOuterBounds(1).contains(event.getMouseDownPosition()))
        currentThumb = 1;
    else if(getThumbOuterBounds(2).contains(event.getMouseDownPosition()))
        currentThumb = 2;
    else if(getThumbOuterBounds(3).contains(event.getMouseDownPosition()))
        currentThumb = 3;

    sliderThumbBounds = sliderThumb.getBounds();
    showPopup();
}

void RangeSlider::mouseMove(const MouseEvent& event)
{
    showPopup();
}

Rectangle<int> RangeSlider::getThumbOuterBounds(int type)
{
    if(!isVertical)
    {
        if(type==1)
            return Rectangle<int>(sliderThumb.getPosition().getX(), 0, thumbWidth, getHeight());
        else if(type==2)
            return Rectangle<int>(sliderThumb.getPosition().getX()+sliderThumb.getWidth()-thumbWidth, 0, thumbWidth, getHeight());
        else if(type==3)
            return Rectangle<int>(sliderThumb.getPosition().getX()+thumbWidth, 0, sliderThumb.getWidth()-thumbWidth, getHeight());
    }
    else
    {
        if(type==1)
            return Rectangle<int>(0, sliderThumb.getPosition().getY(), getWidth(), thumbHeight);
        else if(type==2)
            return Rectangle<int>(0, sliderThumb.getPosition().getY()+sliderThumb.getHeight()-thumbHeight, getWidth(), thumbHeight);
        else if(type==3)
            return Rectangle<int>(0, sliderThumb.getPosition().getY()-thumbHeight, getWidth(), sliderThumb.getPosition().getY()+sliderThumb.getHeight());
    }
}

void RangeSlider::mouseUp(const MouseEvent& event)
{
    sliderThumbBounds = sliderThumb.getBounds();
    currentThumb=0;
}

void RangeSlider::mouseDrag(const MouseEvent& event)
{
    if(isVertical == false)
        horizontalDrag(event);
    else
        verticalDrag(event);

    showPopup();
}

void RangeSlider::verticalDrag(const MouseEvent& event)
{
    if(currentThumb == 1)
    {
        const int bottomThumb = (sliderThumbBounds.getY()+sliderThumbBounds.getHeight())-thumbHeight*2;
        const int yPos = CabbageUtilities::roundToMultiple(jmin(bottomThumb, sliderThumbBounds.getY()+event.getDistanceFromDragStartY()), thumbIncr);
        sliderThumb.setBounds(sliderThumbBounds.withTop(yPos<bottomThumb ? jmax(0, yPos) : bottomThumb));
    }
    else if(currentThumb == 2)
    {
        const int yPos = jlimit(sliderThumbBounds.getY()+thumbHeight*2, getHeight(), sliderThumbBounds.getY()+sliderThumbBounds.getHeight()+event.getDistanceFromDragStartY());
        sliderThumb.setBounds(sliderThumbBounds.withBottom(CabbageUtilities::roundToMultiple(yPos, thumbIncr)));
    }
    else if(currentThumb == 3)
    {
        int yPos = sliderThumbBounds.getPosition().getY()+event.getDistanceFromDragStartY();
        yPos = CabbageUtilities::roundToMultiple(yPos+sliderThumb.getHeight() < getHeight() ? yPos : getHeight()-sliderThumb.getHeight(), thumbIncr);
        sliderThumb.setBounds(0, jmax(0, yPos), getWidth(), sliderThumbBounds.getHeight());
    }

    if(currentThumb!=0)
    {
        const double scaledHeight = (getHeight()-(2*thumbHeight))*.99;//scale down to prevent errors at different sizes...
        double proportion1 = (double)sliderThumb.getPosition().getY()/scaledHeight;
        double proportion2 = ((double)sliderThumb.getPosition().getY()+(double)sliderThumb.getHeight()-thumbHeight*2)/scaledHeight;
        minVal = CabbageUtilities::roundToMultiple(getSkewedValue(proportion1), incr);
        maxVal = CabbageUtilities::roundToMultiple(getSkewedValue(proportion2), incr);
        sendValuesToCsound(minVal, maxVal, 3);
    }
}

void RangeSlider::horizontalDrag(const MouseEvent& event)
{
    if(currentThumb == 1)
    {
        const int rightThumb = jlimit(0, (int)width, (sliderThumbBounds.getX()+sliderThumbBounds.getWidth())-thumbWidth*2);
        const int xPos = CabbageUtilities::roundToMultiple(jmin(rightThumb, sliderThumbBounds.getX()+event.getDistanceFromDragStartX()), thumbIncr);
        sliderThumb.setBounds(sliderThumbBounds.withLeft(xPos<rightThumb ? jmax(0, xPos) : rightThumb));
    }
    else if(currentThumb == 2)
    {
        const int xPos = jlimit(sliderThumbBounds.getX()+thumbWidth*2, getWidth(), sliderThumbBounds.getX()+sliderThumbBounds.getWidth()+event.getDistanceFromDragStartX());
        sliderThumb.setBounds(sliderThumbBounds.withRight(CabbageUtilities::roundToMultiple(xPos, thumbIncr)));
    }
    else if(currentThumb == 3)
    {
        int xPos = sliderThumbBounds.getPosition().getX()+event.getDistanceFromDragStartX();
        xPos = CabbageUtilities::roundToMultiple(xPos+sliderThumb.getWidth() < getWidth() ? xPos : getWidth()-sliderThumb.getWidth(), thumbIncr);
        sliderThumb.setBounds(jmax(0, xPos), 0, jmax(thumbWidth*2, sliderThumbBounds.getWidth()), getHeight());
    }

    if(currentThumb!=0)
    {
        const double scaledWidth = (getWidth()-(2*thumbWidth))*.99;//scale down to prevent errors at different sizes...
        const double proportion1 = (double)sliderThumb.getPosition().getX()/scaledWidth;
        const double proportion2 = ((double)sliderThumb.getPosition().getX()+(double)sliderThumb.getWidth()-thumbWidth*2)/scaledWidth;
        minVal = CabbageUtilities::roundToMultiple(getSkewedValue(proportion1), incr);
        maxVal = CabbageUtilities::roundToMultiple(getSkewedValue(proportion2), incr);
        sendValuesToCsound(minVal, maxVal, currentThumb);
    }

    CabbageUtilities::getBoundsString(sliderThumb.getBounds());

}

void RangeSlider::setValue(float val1, float val2)
{
    //val1 and val2 are scaled between 0 and 1

    if(currentThumb==0)//only update size if the host is controlling the slider
    {
        if(!isVertical)
        {
            int leftPos = getSkewedPosition(val1);
            int rightPos = getSkewedPosition(val2);
            sliderThumb.setBounds(sliderThumbBounds.withLeft(jmax(0, leftPos)));
            sliderThumbBounds = sliderThumb.getBounds();
            sliderThumb.setBounds(sliderThumbBounds.withRight(jlimit(leftPos+thumbWidth, (int)width, rightPos)));
            sliderThumbBounds = sliderThumb.getBounds();
            sliderThumb.setBounds(sliderThumbBounds.withWidth(jlimit(thumbWidth*2, (int)width, sliderThumbBounds.getWidth())));
            sliderThumbBounds = sliderThumb.getBounds();
            if(val1==val2)
                sliderThumb.setCentrePosition(jlimit(thumbWidth, (int)width-thumbWidth, sliderThumbBounds.getCentre().getX()-thumbWidth), sliderThumbBounds.getCentre().getY());

            repaint();
        }
        else
        {
            int topPos = getSkewedPosition(val2);
            int bottomPos = getSkewedPosition(val1);
            sliderThumb.setBounds(sliderThumbBounds.withTop(jmax(0, (int)height-topPos)));
            sliderThumbBounds = sliderThumb.getBounds();
            sliderThumb.setBounds(sliderThumbBounds.withBottom(jmin((int)height, (int)height-bottomPos)));
            sliderThumbBounds = sliderThumb.getBounds();
            sliderThumb.setBounds(sliderThumbBounds.withHeight(jlimit(thumbHeight*2, (int)height, sliderThumbBounds.getHeight())));
            sliderThumbBounds = sliderThumb.getBounds();
            if(val1==val2)
                sliderThumb.setCentrePosition(sliderThumbBounds.getCentre().getX(), jlimit(thumbHeight, (int)height-thumbHeight, sliderThumbBounds.getCentre().getY()-thumbHeight));
            repaint();
        }

    }

}

int RangeSlider::getSkewedPosition(double proportion)
{
    if(isVertical)
    {
        const double skewedPos = pow (1-proportion, skew);
        return skew==1 ? proportion*height : skewedPos*height;
    }
    else
    {
        const double skewedPos = pow (proportion, skew);
        return skew==1 ? proportion*width : skewedPos*width;
    }
}

double RangeSlider::getSkewedValue(double proportion)
{
    if(isVertical)
    {
        const double skewedValue = pow (1-proportion, 1/skew)*(max-min)+min;
        return skew == 1.0 ? jlimit(min, max, (1-proportion)*(max-min)+min) : jlimit(min, max, skewedValue);
    }
    else
    {
        const double skewedValue = pow (proportion, 1/skew)*(max-min)+min;
        return skew == 1.0 ? jlimit(min, max, proportion*(max-min)+min) : jlimit(min, max, skewedValue);
    }
}

void RangeSlider::sendValuesToCsound(double val1, double val2, int thumb)
{

//    if(owner->getFilter()->csoundCompiledOk())
//    {
//#ifdef Cabbage_Build_Standalone
//        if(thumb==1)
//            owner->getFilter()->setParameterNotifyingHost(index, (float)(val1));
//        else if(thumb==2)
//            owner->getFilter()->setParameterNotifyingHost(index+1, (float)(val2));
//        else
//        {
//            owner->getFilter()->setParameterNotifyingHost(index, (float)(val1));
//            owner->getFilter()->setParameterNotifyingHost(index+1, (float)(val2));
//        }
//
//#else
//        const float value1 = jmap(val1, min, max, 0.0, 1.0);
//        const float value2 = jmap(val2, min, max, 0.0, 1.0);
//
//        if(thumb==1)
//            owner->getFilter()->setParameterNotifyingHost(index, value1);
//        else if(thumb==2)
//            owner->getFilter()->setParameterNotifyingHost(index+1, value2);
//        else
//        {
//            owner->getFilter()->setParameterNotifyingHost(index, value1);
//            owner->getFilter()->setParameterNotifyingHost(index+1, value2);
//        }
//
//#endif
//    }

}

void RangeSlider::mouseEnter(const MouseEvent& event)
{
    showPopup();
}

void RangeSlider::paint(Graphics& g)
{
    //the juce native sliders seem to have their width reduced by 18 for som reason or another...


    int sliderThumbWidth=7;

    g.fillAll(Colours::transparentBlack);
    if(!isVertical)
    {
        const int height = getHeight();
        const int width = getWidth()-18;
        g.setColour (Colours::whitesmoke);
        g.setOpacity (0.6);
        const float midPoint = width/2.f+sliderThumbWidth;
        const float markerGap = width/9.f;
        g.drawLine (midPoint, height*0.25, midPoint, height*0.75, 1.5);
        g.setOpacity (0.3);
        for (int i=1; i<5; i++)
        {
            g.drawLine (midPoint+markerGap*i, height*0.3, midPoint+markerGap*i, height*0.7, .7);
            g.drawLine (midPoint-markerGap*i, height*0.3, midPoint-markerGap*i, height*0.7, .7);
        }
        //backgrounds
        g.setColour (Colours::whitesmoke);
        g.setOpacity (0.1);
        g.fillRoundedRectangle (sliderThumbWidth/2.f, height*0.44, width*1.025, height*0.15, height*0.05); //for light effect
        g.setColour (Colour::fromRGBA(5, 5, 5, 255));
        g.fillRoundedRectangle (sliderThumbWidth/2.f, height*0.425, width*1.02, height*0.15, height*0.05); //main rectangle
        //tracker
        g.setColour(trackerColour);
        const float scale = trackerThickness;
        const int iHeight = (height * scale);
        const int iTop = ((height-iHeight)/2.f);
        g.fillRect(sliderThumb.getPosition().getX(), iTop, sliderThumb.getWidth(), iHeight);

    }
    else
    {
        const int width = getWidth();
        const int height = getHeight()-18;
        g.setColour (Colours::whitesmoke);
        g.setOpacity (0.6);
        const float midPoint = height/2.f+sliderThumbWidth;
        const float markerGap = height/9.f;
        g.drawLine (width*0.25, midPoint, width*0.75, midPoint, 1.59);
        g.setOpacity (0.3);

        for (int i=1; i<5; i++)
        {
            g.drawLine (width*0.3, midPoint+markerGap*i, width*0.7, midPoint+markerGap*i, .7);
            g.drawLine (width*0.3, midPoint-markerGap*i, width*0.7, midPoint-markerGap*i, .7);
        }

        g.setColour(Colours::whitesmoke);
        g.setOpacity (0.1);
        g.fillRoundedRectangle(width*0.44, sliderThumbWidth/2.f, width*0.15, height*1.003, width*0.05);
        g.setColour (Colour::fromRGBA(5, 5, 5, 255));
        g.fillRoundedRectangle (width*0.425, sliderThumbWidth/2.f, width*0.15, height*1.029, width*0.05);
        //tracker
        g.setColour(trackerColour);
        const float scale = trackerThickness;
        const int iWidth = (width* scale);
        const int iLeft = ((width-iWidth)/2.f);
        g.fillRect(iLeft, sliderThumb.getPosition().getY(), iWidth, sliderThumb.getHeight());
    }
}

void RangeSlider::showPopup()
{
    if(shouldDisplayPopup)
    {
//        String popupText;
//        if(tooltipText.isNotEmpty())
//            popupText = tooltipText;
//        else if(isVertical)
//            popupText = channels.getReference(0)+": "+String(maxVal)+" "+channels.getReference(1)+": "+String(minVal);
//        else
//            popupText = channels.getReference(0)+": "+String(minVal)+" "+channels.getReference(1)+": "+String(maxVal);
//
//        owner->showBubble(this, popupText);
    }
}

//--------------------------------------------------------
//slider thumb class
//--------------------------------------------------------
RangeSlider::SliderThumb::SliderThumb(bool vertical):
    Component()
{
	isVertical = vertical;
    this->setInterceptsMouseClicks(false, false);
}

void RangeSlider::SliderThumb::paint(Graphics& g)
{
    const int thumbWidth = 10;
    const int thumbHeight = 10;
    //g.fillAll(Colours::red);

    if(isVertical ==false)
    {
        g.setColour(colour.darker(.7));
        g.fillRoundedRectangle(0, getHeight()*.2, thumbWidth, getHeight()*.6, 4);
        g.setColour(colour);
        g.fillRoundedRectangle(0, getHeight()*.2, thumbWidth*.9, getHeight()*.57, 4);

        g.setColour(colour.darker(.7));
        g.fillRoundedRectangle(getWidth()-thumbWidth, getHeight()*.2, thumbWidth, getHeight()*.60, 4);
        g.setColour(colour);
        g.fillRoundedRectangle(getWidth()-thumbWidth, getHeight()*.2, thumbWidth*.9, getHeight()*.57, 4);
    }
    else
    {
        g.setColour(colour.darker(.7));
        g.fillRoundedRectangle(getWidth()*.3, 0, getWidth()*.4, thumbHeight, 4);
        g.setColour(colour);
        g.fillRoundedRectangle(getWidth()*.3, 0, getWidth()*.37, thumbHeight*.9, 4);

        g.setColour(colour.darker(.7));
        g.fillRoundedRectangle(getWidth()*.3, getHeight()-thumbHeight, getWidth()*.4, thumbHeight, 4);
        g.setColour(colour);
        g.fillRoundedRectangle(getWidth()*.3, getHeight()-thumbHeight, getWidth()*.37, thumbHeight*.9, 4);
    }
}