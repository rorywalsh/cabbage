/*
  ==============================================================================

    CabbageEncoder.cpp
    Created: 8 Dec 2016 4:32:12pm
    Author:  rory

  ==============================================================================
*/

#include "CabbageEncoder.h"
#include "../Audio/Plugins/CabbagePluginEditor.h"

CabbageEncoder::CabbageEncoder (ValueTree wData, CabbagePluginEditor* _owner)
    : widgetData (wData),
      owner (_owner),
      popupBubble (250),
      outlinecolour (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::outlinecolour)),
      colour (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::colour)),
      trackercolour (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::trackercolour)),
      text (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::text)),
      textcolour (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::textcolour)),
      decimalPlaces (CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::decimalplaces)),
      shouldShowValueTextBox (CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::valuetextbox))
{
    setName (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::name));
    widgetData.addListener (this);              //add listener to valueTree so it gets notified when a widget's property changes
    initialiseCommonAttributes (this, wData);   //initialise common attributes such as bounds, name, rotation, etc..
    textLabel.setColour (Label::textColourId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::textcolour)));
    min = CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::min);
    max = CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::max);
    sliderIncr = CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::increment);
    skew = CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::sliderskew);
    value = CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::value);
    currentEncValue = value;
    addAndMakeVisible (textLabel);
    addAndMakeVisible (valueLabel);
    valueLabel.setVisible (true);
    valueLabel.setText (String (value), dontSendNotification);
    textLabel.setVisible (true);
    createPopupBubble();
    valueLabel.setEditable (true);
    valueLabel.addListener (this);
    textLabel.setColour (Label::textColourId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::textcolour)));
    valueLabel.setColour (Label::textColourId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::fontcolour)));
    valueLabel.setColour (Label::backgroundColourId, Colours::black);
    valueLabel.setColour (Label::outlineColourId, Colours::whitesmoke);

}

void CabbageEncoder::createPopupBubble()
{
    //create popup display for showing value of sliders.
    popupBubble.setColour (BubbleComponent::backgroundColourId, Colours::white);
    popupBubble.setBounds (0, 0, 50, 20);
    owner->addChildComponent (popupBubble);
    popupBubble.setVisible (false);
    popupBubble.setAlwaysOnTop (true);
}

void CabbageEncoder::labelTextChanged (Label* label)
{
    float value = jlimit (min, max, label->getText().getFloatValue());
    sliderPos = 0;
    currentEncValue = value;
    valueLabel.setText (String (value, 2), dontSendNotification);
    owner->sendChannelDataToCsound (getChannel(), currentEncValue);
    showPopup();
}

void CabbageEncoder::mouseWheelMove (const MouseEvent& event, const MouseWheelDetails& wheel)
{
    if (CabbageWidgetData::getNumProp (widgetData, CabbageIdentifierIds::active) == 1)
    {
        if (wheel.deltaY < 0)
        {
            currentEncValue -= sliderIncr;
            sliderPos = sliderPos + 50;
        }
        else
        {
            currentEncValue += sliderIncr;
            sliderPos = sliderPos - 50;
        }


        repaint();
        owner->sendChannelDataToCsound (getChannel(), currentEncValue);
        showPopup();
    }
}

void CabbageEncoder::mouseDown (const MouseEvent& e)
{
    if (CabbageWidgetData::getNumProp (widgetData, CabbageIdentifierIds::active) == 1)
    {
        if (e.getNumberOfClicks() > 1)
        {
            sliderPos = 0;
            currentEncValue = startingValue;
            repaint();
            owner->sendChannelDataToCsound (getChannel(), currentEncValue);
            showPopup();
        }
    }
}

void CabbageEncoder::mouseEnter (const MouseEvent& e)
{
    isMouseOver = true;
    showPopup (5000);
}

void CabbageEncoder::mouseDrag (const MouseEvent& e)
{
    if (CabbageWidgetData::getNumProp (widgetData, CabbageIdentifierIds::active) == 1)
    {
        if (yAxis != e.getOffsetFromDragStart().getY())
        {
            sliderPos = sliderPos + (e.getOffsetFromDragStart().getY() < yAxis ? -50 : 50);
            currentEncValue = CabbageUtilities::roundToPrec (currentEncValue + (e.getOffsetFromDragStart().getY() < yAxis ? sliderIncr : -sliderIncr), 5);

            if (CabbageWidgetData::getNumProp (widgetData, CabbageIdentifierIds::minenabled) == 1)
                currentEncValue = jmax (min, currentEncValue);

            if (CabbageWidgetData::getNumProp (widgetData, CabbageIdentifierIds::maxenabled) == 1)
                currentEncValue = jmin (max, currentEncValue);

            yAxis = e.getOffsetFromDragStart().getY();
            repaint();
            valueLabel.setText (String (currentEncValue), dontSendNotification);

            //  valueLabel.setText(String(currentEncValue, 2), dontSendNotification);
            owner->sendChannelDataToCsound (getChannel(), currentEncValue);
            showPopup();
        }
    }
}

void CabbageEncoder::showPopup (int displayTime)
{
    if (shouldDisplayPopup)
    {
        String popupText;

        if (getTooltipText().isNotEmpty())
            popupText = getTooltipText();
        else
            popupText = getChannel() + ": " + String (currentEncValue);

        popupBubble.showAt (this, AttributedString (popupText), displayTime);
    }
}

void CabbageEncoder::mouseExit (const MouseEvent& e)
{
    isMouseOver = false;
    showPopup (10);
    repaint();
}

void CabbageEncoder::paint (Graphics& g)
{
    float rotaryStartAngle = 0;
    float rotaryEndAngle = 2 * 3.14;

    const float radius = jmin (slider.getWidth() / 2, slider.getHeight() / 2) - 2.0f;
    const float diameter = radius * 2.f;
    const float centreX = getWidth() * 0.5f;
    const float centreY = slider.getY() + slider.getHeight() * 0.5f;
    const float rx = centreX - radius;
    const float ry = centreY - radius;
    const float rw = radius * 2.0f;
    const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    if (radius > 12.0f)
    {
        g.setColour (Colour::fromString (outlinecolour).withAlpha (isMouseOver ? 1.0f : 0.7f));
        //g.drawEllipse(slider.reduced(1.f).toFloat(), isMouseOver ? 1.5f : 1.f);
        //g.drawEllipse(slider.reduced(getWidth()*.11).toFloat(), isMouseOver ? 1.5f : 1.f);

        Path newPolygon;
        Point<float> centre (centreX, centreY);

        if (diameter >= 25)   //If diameter is >= 40 then polygon has 12 steps
        {
            newPolygon.addPolygon (centre, 12.f, radius, 0.f);
            newPolygon.applyTransform (AffineTransform::rotation (angle,
                                                                  centreX, centreY));
        }
        else //Else just use a circle. This is clearer than a polygon when very small.
            newPolygon.addEllipse (-radius * .2, -radius * .2, radius * .3f, radius * .3f);

        g.setColour (Colour::fromString (colour));

        Colour thumbColour = Colour::fromString (colour).withAlpha (isMouseOver ? 1.0f : 0.9f);
        ColourGradient cg = ColourGradient (Colours::white, 0, 0, thumbColour, diameter * 0.6, diameter * 0.4, false);
        //if(slider.findColour (Slider::thumbColourId)!=Colour(0.f,0.f,0.f,0.f))
        g.setGradientFill (cg);
        g.fillPath (newPolygon);


        g.setColour (Colour::fromString (trackercolour).withAlpha (isMouseOver ? 1.0f : 0.9f));

        const float thickness = 0.7f;
        {
            Path filledArc;
            filledArc.addPieSegment (rx, ry, rw, rw, angle - .25, angle + .25f, thickness);
            g.fillPath (filledArc);
        }
    }
    else
    {
        Path p;
        g.setColour (Colour::fromString (colour).withAlpha (isMouseOver ? 1.0f : 0.7f));
        p.addEllipse (-0.4f * rw, -0.4f * rw, rw * 0.8f, rw * 0.8f);
        g.fillPath (p, AffineTransform::rotation (angle).translated (centreX, centreY));

        //if (slider.isEnabled())
        g.setColour (Colour::fromString (trackercolour).withAlpha (isMouseOver ? 0.7f : 0.5f));
        //else
        //    g.setColour (Colour (0x80808080));

        p.addEllipse (-0.4f * rw, -0.4f * rw, rw * 0.8f, rw * 0.8f);
        PathStrokeType (rw * 0.1f).createStrokedPath (p, p);

        p.addLineSegment (Line<float> (0.0f, 0.0f, 0.0f, -radius), rw * 0.1f);

        g.fillPath (p, AffineTransform::rotation (angle).translated (centreX, centreY));

    }
}

void CabbageEncoder::resized()
{
    if (text.isNotEmpty() && shouldShowValueTextBox > 0)
    {
        textLabel.setBounds (0, 0, getWidth(), 20);
        textLabel.setText (text, dontSendNotification);
        textLabel.setJustificationType (Justification::centred);
        textLabel.setVisible (true);
        valueLabel.setVisible (true);
        slider.setBounds (20, 20, getWidth() - 40, getHeight() - 40);
        valueLabel.setBounds (getWidth() / 3, getHeight() - 15, getWidth() / 3, 15);
        valueLabel.setJustificationType (Justification::centred);
        valueLabel.setText (String (currentEncValue, 2), dontSendNotification);
    }
    else if (text.isNotEmpty() && shouldShowValueTextBox == 0)
    {
        textLabel.setBounds (0, getHeight() - 20, getWidth(), 20);
        textLabel.setText (text, dontSendNotification);
        textLabel.setJustificationType (Justification::centred);
        textLabel.setVisible (true);
        valueLabel.setVisible (false);
        slider.setBounds (10, 0, getWidth() - 20, getHeight() - 20);
    }
    else if (shouldShowValueTextBox > 0)
    {
        textLabel.setVisible (false);
        valueLabel.setVisible (true);
        slider.setBounds (0, 0, getWidth() - 20, getHeight() - 20);
        valueLabel.setBounds (getWidth() / 3, getHeight() - 15, getWidth() / 3, 15);
        valueLabel.setJustificationType (Justification::centred);
        valueLabel.setText (String (currentEncValue, 2), dontSendNotification);
    }
    else
    {
        textLabel.setVisible (false);
        valueLabel.setVisible (false);
        slider.setBounds (0, 0, getWidth(), getHeight());
    }

    repaint();
}

void CabbageEncoder::valueTreePropertyChanged (ValueTree& valueTree, const Identifier& prop)
{

    if (prop == CabbageIdentifierIds::value)
    {
        currentEncValue = CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::value);
        valueLabel.setText (String (currentEncValue), dontSendNotification);
        //sliderPos = 1.f/currentEncValue;
        repaint();
    }
    else
    {
        colour = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::colour);
        textcolour = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::fontcolour);
        trackercolour = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::trackercolour);
        outlinecolour = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::outlinecolour);
        shouldShowValueTextBox = CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::valuetextbox);
        text = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::text);
        textcolour = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::textcolour);
        textLabel.setColour (Label::textColourId, Colour::fromString (CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::textcolour)));
        valueLabel.setColour (Label::textColourId, Colour::fromString (CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::fontcolour)));

        handleCommonUpdates (this, valueTree);      //handle comon updates such as bounds, alpha, rotation, visible, etc
        resized();
    }
}