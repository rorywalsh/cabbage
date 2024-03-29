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
#include "../Audio/Plugins/CabbagePluginEditor.h"

CabbageRangeSlider::CabbageRangeSlider (ValueTree wData, CabbagePluginEditor* _owner):
    owner (_owner),
    textColour (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::textcolour)),
    slider (this),
    popupBubble (250),
    widgetData (wData),
    CabbageWidgetBase(_owner)
{
    setName (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::name));
    widgetData.addListener (this);              //add listener to valueTree so it gets notified when a widget's property changes
    initialiseCommonAttributes (this, wData);   //initialise common attributes such as bounds, name, rotation, etc..

    isVertical = CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::kind) == "horizontal" ? false : true;

    if (isVertical == true)
        slider.setSliderStyle (Slider::SliderStyle::TwoValueVertical);
    else
        slider.setSliderStyle (Slider::SliderStyle::TwoValueHorizontal);

    slider.setName (getName());
    addAndMakeVisible (&slider);

    minValue = CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::minvalue);
    maxValue = CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::maxvalue);

    min = CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::min);
    max = CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::max);

    decimalPlaces = CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::decimalplaces);
    sliderIncrement = CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::increment);
    sliderSkew = CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::sliderskew);

    slider.setRange (min, max, sliderIncrement);
    slider.setTextBoxStyle (Slider::NoTextBox, false, 0, 0);
    slider.setSkewFactor (sliderSkew);
    slider.setMinAndMaxValues (minValue, maxValue);

    addAndMakeVisible (&textLabel);
    textLabel.setVisible (false);


    setLookAndFeelColours (widgetData);
    createPopupBubble();

    
    const String globalStyle = owner->globalStyle;
    if(globalStyle == "legacy")
    {
        return;
    }
    else if (CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::style) == "flat")
    {
        slider.setLookAndFeel(&flatLookAndFeel);
    }
    
    prefix = CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::valueprefix);
    postfix = CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::valuepostfix);
    popupPrefix  = CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::popupprefix);
    popupPostfix = CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::popuppostfix);
    
    const String popup = CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::popuptext);
    if (popup == "0" || (popup == "" && popupPrefix == "" && popupPostfix == ""))
        shouldDisplayPopup = false;
    else
        shouldDisplayPopup = true;
    
    resized();
}

void CabbageRangeSlider::setSliderValues (ValueTree wData)
{
    minValue = CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::minvalue);
    maxValue  = CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::maxvalue);

    min = CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::min);
    max = CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::max);

    decimalPlaces = CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::decimalplaces);
    sliderIncrement = CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::increment);
    sliderSkew = CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::sliderskew);

    slider.setRange (min, max, sliderIncrement);
    slider.setTextBoxStyle (Slider::NoTextBox, false, 0, 0);
    slider.setSkewFactor (sliderSkew);

    slider.setMinAndMaxValues (minValue, maxValue, dontSendNotification);
    owner->sendChannelDataToCsound(getChannelArray()[0], minValue);
    owner->sendChannelDataToCsound(getChannelArray()[1], maxValue);
}

void CabbageRangeSlider::setCurrentValues (float newMin, float newMax)
{
    minValue = newMax;
    maxValue = newMax;
    CabbageWidgetData::setNumProp (widgetData, CabbageIdentifierIds::minvalue, minValue);
    CabbageWidgetData::setNumProp (widgetData, CabbageIdentifierIds::maxvalue, maxValue);
}

void CabbageRangeSlider::createPopupBubble()
{
    //create popup display for showing value of sliders.
    if(!shouldDisplayPopup)
        return;
    
    popupBubble.setColour (BubbleComponent::backgroundColourId, Colours::white);
    popupBubble.setBounds (0, 0, 50, 20);
    owner->addChildComponent (popupBubble);
    popupBubble.setVisible (false);
    popupBubble.setAlwaysOnTop (true);
}

void CabbageRangeSlider::showPopup (int displayTime)
{
    if(!shouldDisplayPopup)
        return;
    
    String popupText;

    if (getTooltipText().isNotEmpty())
        popupText = getTooltipText();
    else
        popupText = getChannelArray()[0] + ": " + createValueText(slider.getMinValue(), 2, popupPrefix, popupPostfix) +
        +"\n" + getChannelArray()[1] + ": " + createValueText(slider.getMaxValue(), 2, popupPrefix, popupPostfix);

    popupBubble.showAt (&slider, AttributedString (popupText), displayTime);
}


void CabbageRangeSlider::setLookAndFeelColours (ValueTree wData)
{

    textLabel.setColour (Label::outlineColourId, Colours::transparentBlack);
    slider.setColour (Slider::textBoxHighlightColourId, Colours::lime.withAlpha (.2f));

    slider.setColour (Slider::thumbColourId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::colour)));
    slider.setColour (Slider::backgroundColourId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::trackerbgcolour)));
    slider.setColour (Slider::trackColourId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::trackercolour)));
    slider.setColour (Slider::rotarySliderOutlineColourId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::outlinecolour)));


    slider.setColour (TextEditor::textColourId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::fontcolour)));
    textLabel.setColour (Label::textColourId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::textcolour)));

    slider.setColour (Slider::textBoxTextColourId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::fontcolour)));
    slider.setColour (Slider::textBoxBackgroundColourId, Colours::black);
    slider.setColour (Slider::textBoxHighlightColourId, Colours::white);

    slider.setColour (Label::textColourId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::fontcolour)));
    slider.setColour (Label::backgroundColourId, CabbageUtilities::getBackgroundSkin());

    slider.setColour (Label::outlineColourId, CabbageUtilities::getBackgroundSkin());
    slider.lookAndFeelChanged();
}

void CabbageRangeSlider::resized()
{
    if (getText().isNotEmpty())
    {
        if (isVertical)
        {
            textLabel.setBounds (0, getHeight() - 20, getWidth(), 20);
            textLabel.setJustificationType (Justification::centred);
            textLabel.setText (getText(), dontSendNotification);
            textLabel.setVisible (true);
            slider.setBounds (0, 4, getWidth(), getHeight() - 20);
        }
        else
        {
            float width = textLabel.getFont().getStringWidthFloat (getText()) + 10.f;
            textLabel.setBounds (0, 0, width, getHeight());
            textLabel.setText (getText(), dontSendNotification);
            textLabel.setVisible (true);
            slider.setBounds (width, 0, getWidth() - (width * 1.10), getHeight());
        }

    }
    else
        slider.setBounds (getLocalBounds());
}

void CabbageRangeSlider::valueTreePropertyChanged (ValueTree& valueTree, const Identifier& prop)
{
    if (prop == CabbageIdentifierIds::minvalue || prop == CabbageIdentifierIds::maxvalue)
    {
        setSliderValues (valueTree);
    }
    else if (prop == CabbageIdentifierIds::minvalue)
    {
        setSliderValues(valueTree);
    }
    else
    {
        textLabel.setText (getCurrentText (valueTree), dontSendNotification);
        textLabel.setVisible (getCurrentText (valueTree).isNotEmpty() ? true : false);
        slider.setTooltip (getCurrentPopupText (valueTree));
        handleCommonUpdates (this, valueTree, prop);
        setLookAndFeelColours (valueTree);
    }
}

//======================================================================================
RangeSlider::RangeSlider (CabbageRangeSlider* _owner)
    : owner (_owner),
    mouseDragBetweenThumbs {false}
{
    // setSliderStyle (Slider::TwoValueHorizontal);
}

RangeSlider::~RangeSlider ()
{

}

void RangeSlider::mouseDown (const MouseEvent& event)
{
    if (getSliderStyle() == Slider::TwoValueHorizontal)
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
    else
    {
        const float currentMouseY = getHeight() - event.getPosition().getY();
        const int thumbRadius = getLookAndFeel().getSliderThumbRadius (*this);
        yMinAtThumbDown = valueToProportionOfLength (getMinValue()) * getHeight();
        yMaxAtThumbDown = valueToProportionOfLength (getMaxValue()) * getHeight();

        if (currentMouseY > yMinAtThumbDown + thumbRadius && currentMouseY < yMaxAtThumbDown - thumbRadius)
        {
            mouseDragBetweenThumbs = true;
        }
        else
        {
            mouseDragBetweenThumbs = false;
            Slider::mouseDown (event);
        }

    }

    owner->showPopup (1000);
}

void RangeSlider::mouseDrag (const MouseEvent& event)
{
    if (getSliderStyle() == Slider::TwoValueHorizontal)
    {
        const float distanceFromStart = event.getDistanceFromDragStartX();

        if (mouseDragBetweenThumbs)
        {
            double ratioMin = (xMinAtThumbDown + distanceFromStart) / getWidth();
            double ratioMax = (xMaxAtThumbDown + distanceFromStart) / getWidth();
            float newMin = proportionOfLengthToValue (std::min(std::max(ratioMin, 0.0), 1.0 ));
            float newMax = proportionOfLengthToValue (std::min(std::max(ratioMax, 0.0), 1.0 ));
            if (newMin > getMinimum())
                setMinValue (newMin);
            if (newMax < getMaximum())
                setMaxValue (newMax);
        }
        else
        {
            Slider::mouseDrag (event);
        }
    }
    else
    {
        const float distanceFromStart = event.getDistanceFromDragStartY();

        if (mouseDragBetweenThumbs)
        {
            double ratioMin = (yMinAtThumbDown - distanceFromStart) / getHeight();
            double ratioMax = (yMaxAtThumbDown - distanceFromStart) / getHeight();
            float newMin = proportionOfLengthToValue (std::min(std::max(ratioMin, 0.0), 1.0 ));
            float newMax = proportionOfLengthToValue (std::min(std::max(ratioMax, 0.0), 1.0 ));
            
            if (newMin > getMinimum())
                setMinValue (newMin);
            if (newMax < getMaximum())
                setMaxValue (newMax);
        }
        else
        {
            Slider::mouseDrag (event);
        }

    }

    //owner->setCurrentValues(getMinValue(), getMaxValue());

    owner->showPopup (1000);
}

void RangeSlider::mouseExit (const MouseEvent& event)
{
    owner->showPopup (10);
}

void RangeSlider::mouseEnter (const MouseEvent& e)
{
    owner->showPopup (5000);
}
