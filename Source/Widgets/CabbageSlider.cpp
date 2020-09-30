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

#include "CabbageSlider.h"
#include "../Audio/Plugins/CabbagePluginEditor.h"


void FilmStripSlider::paint(Graphics& g)
{
    if (imageIsNull == false)
    {
        const float sliderPos = (float)valueToProportionOfLength(getValue());

        int value = sliderPos * (numFrames_ - 1);
        if (isHorizontal_) {
            g.drawImage(filmStrip, 0, 0, getWidth(), getHeight(),
                value * frameWidth, 0, frameWidth, frameHeight);
        }
        else {
            if (getSliderStyle() == SliderStyle::RotaryVerticalDrag)
            {
                g.drawImage(filmStrip, getWidth() * .175f, getHeight() * .175f, getWidth() * .65f, getHeight() * .65f,
                    0, value * frameHeight, frameWidth, frameHeight);
            }
            else if(getSliderStyle() == SliderStyle::LinearVertical)
            {
                g.drawImage(filmStrip, getWidth() * .175f, getHeight() * .16f, getWidth() * .65f, getHeight() * .75f,
                    0, value * frameHeight, frameWidth, frameHeight);
            }
            else if (getSliderStyle() == SliderStyle::LinearHorizontal)
            {
                g.drawImage(filmStrip, getWidth()*.01f, getHeight() * .2f, getWidth() * .78f, getHeight() * .65f,
                    0, value * frameHeight, frameWidth, frameHeight);
            }
        }
    }
}


CabbageSlider::CabbageSlider (ValueTree wData, CabbagePluginEditor* _owner)
    : owner (_owner),
      widgetData (wData),

      sliderType (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::kind)),
      channel (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::channel)),
popupBubble (250), 
filmSlider(wData, CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::filmstripframes), CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::filmstriporientation) == "vertical" ? false : true)
{

    setName (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::name));
	widgetData.addListener (this);
    setLookAndFeelColours (widgetData);
    addAndMakeVisible (textLabel);
    if (CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::filmstripimage).length() == 0)
    {
        addAndMakeVisible(&slider);
        slider.setName(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::name));
        slider.getProperties().set("trackerthickness", CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::trackerthickness));
        slider.getProperties().set("trackerbgcolour", CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::trackerbgcolour));
        slider.getProperties().set("markercolour", CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::markercolour));
        slider.getProperties().set("markerthickness", CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::markerthickness));
        slider.getProperties().set("markerstart", CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::markerstart));
        slider.getProperties().set("markerend", CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::markerend));
        slider.getProperties().set("gapmarkers", CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::gapmarkers));
        setImgProperties(this->slider, wData, "slider");
        setImgProperties(this->slider, wData, "sliderbg");
        
        initialiseSlider(wData, slider);
        setTextBoxOrientation(sliderType, shouldShowTextBox);
        const String sliderImg = CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::imgslider);
        const String sliderImgBg = CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::imgsliderbg);
        const String globalStyle = owner->globalStyle;
        if (CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::style) == "flat"
            && sliderImg.isEmpty() && sliderImgBg.isEmpty())
        {
            slider.setLookAndFeel(&flatLookAndFeel);
        }
        slider.setTextValueSuffix(postfix);
    }        
    else 
    {
        isFilmStripSlider = true;
        filmSlider.setName(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::name));
        addAndMakeVisible(&filmSlider);
        initialiseSlider(wData, filmSlider);
        filmSlider.setTextValueSuffix(postfix);
        setTextBoxOrientation(sliderType, shouldShowTextBox);
        filmSlider.getProperties().set("showTextBox", shouldShowTextBox);
        sliderIncrement = CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::increment);
        sliderSkew = CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::sliderskew);
        min = CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::min);
        max = CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::max);
        value = CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::value);
    }
        
    prefix = CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::valueprefix);
    postfix = CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::valuepostfix);
    popupPrefix = CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::popupprefix);
    popupPostfix = CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::popuppostfix);
    

    
    textLabel.setVisible (false);
    initialiseCommonAttributes (this, wData);
    createPopupBubble();
}

CabbageSlider::~CabbageSlider()
{
    slider.setLookAndFeel (nullptr);
    filmSlider.setLookAndFeel(nullptr);
}

void CabbageSlider::initialiseSlider (ValueTree wData, Slider& currentSlider)
{
    decimalPlaces = CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::decimalplaces);
    sliderIncrement = CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::increment);
    sliderSkew = CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::sliderskew);
    min = CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::min);
    max = CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::max);
    value = CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::value);
    shouldShowTextBox = CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::valuetextbox);
    trackerThickness = CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::trackerthickness);
    trackerInnerRadius = CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::trackerinsideradius);
    trackerOuterRadius = CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::trackeroutsideradius);
    currentSlider.getProperties().set ("trackerthickness", trackerThickness);
    currentSlider.getProperties().set ("trackerinnerradius", trackerInnerRadius);
    currentSlider.getProperties().set ("trackerouterradius", trackerOuterRadius);

    currentSlider.setSkewFactor (sliderSkew);
    currentSlider.setRange (min, max, sliderIncrement);


    const String popup = CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::popuptext);
    if (popup == "0" || (popup == "" && popupPrefix == "" && popupPostfix == "" && shouldShowTextBox == 1))
        shouldDisplayPopup = false;
    else
        shouldDisplayPopup = true;

    if (getCurrentText (wData).isNotEmpty())
        textLabel.setVisible (true);
    else
        textLabel.setVisible (false);

    currentSlider.setDoubleClickReturnValue (true, value);
    setSliderVelocity (wData);
    currentSlider.addMouseListener (this, false);
    currentSlider.setRotaryParameters (float_Pi * 1.2f, float_Pi * 2.8f, false);

    if (sliderType.contains ("rotary"))
    {
        currentSlider.setSliderStyle (Slider::RotaryVerticalDrag);
        currentSlider.setRotaryParameters (MathConstants<float>::pi * 1.2f, MathConstants<float>::pi * 2.8f, true);
    }
    else if (sliderType.contains ("vertical"))
        currentSlider.setSliderStyle (Slider::LinearVertical);
    else if (sliderType.contains ("horizontal"))
        currentSlider.setSliderStyle (Slider::LinearHorizontal);


}

void CabbageSlider::setTextBoxOrientation (String type, int shouldShowTextBox)
{
    if (shouldShowTextBox == 1)
    {
        if(!shouldDisplayPopup)
            shouldDisplayPopup = false;
        setTextBoxWidth();
    }
    else
    {
        getSlider().setTextBoxStyle (Slider::NoTextBox, false, 0, 0);
    }
}

void CabbageSlider::setTextBoxWidth()
{
    if (sliderType.contains ("horizontal"))
        getSlider().setTextBoxStyle (Slider::TextBoxRight, false, jmin (55.f, getWidth()*.65f), 15);
    else
        getSlider().setTextBoxStyle (Slider::TextBoxBelow, false, jmin (55.f, getWidth()*.65f), 15);
}

void CabbageSlider::resized()
{
    if (sliderType.contains("rotary"))
    {
        if (getText().isNotEmpty())
        {
            textLabel.setText(getText(), dontSendNotification);
            textLabel.setJustificationType(Justification::centred);
            textLabel.setVisible(true);

            if (shouldShowTextBox == 1)
            {
                textLabel.setBounds(0, 0, getWidth(), getHeight()*.2f);
                if (isFilmStripSlider)
                    getSlider().setBounds(0, 0, getWidth(), getHeight());
                else
                    getSlider().setBounds(0, getHeight()*.2f, getWidth(), getHeight() - getHeight()*.2f);
            }
            else
            {
                textLabel.setBounds(0, getHeight() - getHeight()*.2f, getWidth(), getHeight() * .2f);
                if(isFilmStripSlider)
                    getSlider().setBounds(-getWidth() * .1f, -getHeight()*.1f, getWidth() + getWidth() * .2f, getHeight()+ getHeight() * .1f);
                else
                    getSlider().setBounds(0, 0, getWidth(), getHeight() - getHeight() * .15f);
            }
        }
        else
            if (isFilmStripSlider)
            {
                if (shouldShowTextBox)
                    getSlider().setBounds(-getWidth() / 16.f, -getHeight() / 8.0f, getWidth()+ getWidth() / 8.0f, getHeight()+ getHeight() / 8.0f);
                else
                    getSlider().setBounds(-getWidth() / 8.0f, -getHeight() / 8.0f, getWidth() + getWidth() * .25f, getHeight() + getHeight() * .25f);
            }
               
            else
                getSlider().setBounds(0, 0, getWidth(), getHeight());

        if(shouldShowTextBox == 1)
            setTextBoxWidth();
    }

    //else if vertical
    else if (sliderType.contains("vertical"))
    {
        if (getText().isNotEmpty())
        {
            textLabel.setJustificationType(Justification::centred);
            textLabel.setText(getText(), dontSendNotification);
            textLabel.setVisible(true);

            if (shouldShowTextBox == 1)
            {
                textLabel.setBounds(0, 1, getWidth(), getHeight()* .1f);
                if(isFilmStripSlider)
                    getSlider().setBounds(0, -+getHeight() / 13.0f, getWidth(), getHeight()+getHeight()/13.0f);
                else
                    getSlider().setBounds(0, getHeight() * .1f, getWidth(), getHeight() - getHeight()*.1f);
            }
            else
            {
                textLabel.setBounds(0, getHeight() - getHeight() * .1f, getWidth(), getHeight()*.1f);
                if (isFilmStripSlider)
                    getSlider().setBounds(0, -getHeight() * .2f, getWidth(), getHeight()+ getHeight()*.2f);
                else
                    getSlider().setBounds(0, 0, getWidth(), getHeight() - getHeight() * .1f);
            }
        }
        else
        {
            if (isFilmStripSlider)
            {
                if (shouldShowTextBox)
                    getSlider().setBounds(0, -getHeight() / 5.71f, getWidth(), getHeight() + getHeight() / 5.71f);
                else
                    getSlider().setBounds(0, -getHeight() * .2f, getWidth(), getHeight() + getHeight() / 3.332f);
            }
               
            else
                getSlider().setBounds(0, 0, getWidth(), getHeight());
        }

        if (shouldShowTextBox == 1)
            setTextBoxWidth();
    }

    //else if horizontal
    else
    {
        if (getText().isNotEmpty())
        {
            //h=60, w=300
            const float width = textLabel.getFont().getStringWidthFloat(getText()) + 10.f;
            textLabel.setText(getText(), dontSendNotification);
            textLabel.setVisible(true);

            if (shouldShowTextBox == 1)
            {
                textLabel.setBounds(0, 0, width, getHeight());
                getSlider().setBounds(width - 3, 0, getWidth() - (width - 4), getHeight());
            }
            else
            {
                textLabel.setBounds(0, 0, width, getHeight());
                if (isFilmStripSlider)
                    getSlider().setBounds(width - 3, 0, getWidth() + getWidth() / 4.27f - (width - 4), getHeight());
                else
                    getSlider().setBounds(width - 3, 0, getWidth() - (width - 4), getHeight());
            }
        }
        else
        {
            if (isFilmStripSlider) 
            {
                if (shouldShowTextBox)
                    getSlider().setBounds(0, 0, getWidth(), getHeight());
                else
                    getSlider().setBounds(0, 0, getWidth() + getWidth() / 3.75f, getHeight());
            }
                
            else
                getSlider().setBounds(0, 0, getWidth(), getHeight());
        }


        if (shouldShowTextBox == 1)
            setTextBoxWidth();
            

        if (shouldShowTextBox == 1)
            setTextBoxWidth();
    }

   getSlider().setValue(value, dontSendNotification);
    
}

void CabbageSlider::createPopupBubble()
{
    //create popup display for showing value of sliders.
    popupBubble.setColour (BubbleComponent::backgroundColourId, Colours::white);
    popupBubble.setBounds (0, 0, 50, 20);
    owner->addChildComponent (popupBubble);
    popupBubble.setVisible (false);
    popupBubble.setAlwaysOnTop (true);
}

void CabbageSlider::showPopupBubble (int time)
{
    popupText = createPopupBubbleText(getSlider().getValue(),
                                      decimalPlaces,
                                      channel,
                                      popupPrefix,
                                      popupPostfix);

   popupBubble.showAt (&getSlider(), AttributedString (popupText), time);

}

void CabbageSlider::mouseDrag (const MouseEvent& event)
{
    if (shouldDisplayPopup)
        showPopupBubble (150);
}

void CabbageSlider::mouseMove (const MouseEvent& event)
{
    if (shouldDisplayPopup)
        showPopupBubble (150);
}

void CabbageSlider::mouseEnter (const MouseEvent& event)
{
    if (shouldDisplayPopup)
        showPopupBubble (1000);
}

void CabbageSlider::mouseExit (const MouseEvent& event)
{
    if (shouldDisplayPopup)
        popupBubble.setVisible (false);
}

void CabbageSlider::setSliderVelocity (ValueTree wData)
{
    velocity = CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::velocity);

    if (velocity > 0)
    {
        getSlider().setVelocityModeParameters (velocity, 1, 0.0, true);
        getSlider().setVelocityBasedMode (true);
    }
    else
        getSlider().setVelocityBasedMode (false);
}

void CabbageSlider::setLookAndFeelColours (ValueTree wData)
{
    textLabel.setColour (Label::outlineColourId, Colours::transparentBlack);
    getSlider().setColour (Slider::textBoxHighlightColourId, Colours::lime.withAlpha (.2f));

    getSlider().setColour (Slider::thumbColourId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::colour)));
    getSlider().setColour (Slider::trackColourId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::trackercolour)));
    getSlider().setColour (Slider::rotarySliderOutlineColourId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::outlinecolour)));


    getSlider().setColour (TextEditor::textColourId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::fontcolour)));
    textLabel.setColour (Label::textColourId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::textcolour)));

    getSlider().setColour (Slider::textBoxTextColourId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::fontcolour)));
    getSlider().setColour (Slider::textBoxBackgroundColourId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::textboxcolour)));
    getSlider().setColour (Slider::textBoxHighlightColourId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::textboxcolour)).contrasting());
    getSlider().setColour (Slider::textBoxOutlineColourId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::textboxoutlinecolour)));

    getSlider().setColour (Label::textColourId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::fontcolour)));
    getSlider().setColour (Label::backgroundColourId, CabbageUtilities::getBackgroundSkin());

    getSlider().getProperties().set("markercolour", CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::markercolour));
    
    getSlider().setColour (Label::outlineColourId, CabbageUtilities::getBackgroundSkin());
    getSlider().lookAndFeelChanged();
}

//==============================================================================
void CabbageSlider::valueTreePropertyChanged (ValueTree& valueTree, const Identifier& prop)
{

    if (prop == CabbageIdentifierIds::value)
    {
        getSlider().setValue (CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::value), dontSendNotification);
    }
    else
    {
        textLabel.setText (getCurrentText (valueTree), dontSendNotification);
        textLabel.setVisible (getCurrentText (valueTree).isNotEmpty() ? true : false);
        shouldShowTextBox = CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::valuetextbox);
        setTextBoxOrientation (sliderType, shouldShowTextBox);
        getSlider().setTooltip (getCurrentPopupText (valueTree));
        
        getSlider().getProperties().set ("trackerthickness", CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::trackerthickness));
        getSlider().getProperties().set ("trackerinnerradius", CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::trackerinsideradius));
        getSlider().getProperties().set ("trackerouterradius", CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::trackeroutsideradius));
        
        handleCommonUpdates (this, valueTree);
        setLookAndFeelColours (valueTree);

        //resized();
    }

}
