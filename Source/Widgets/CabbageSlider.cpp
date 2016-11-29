/*
  Copyright (C) 2016 Rory Walsh

  Cabbage is free software; you can redistribute it
  and/or modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  Cabbage is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with Csound; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
  02111-1307 USA
*/
#include "CabbageSlider.h"
#include "../Audio/Plugins/CabbagePluginEditor.h"

CabbageSlider::CabbageSlider(ValueTree wData, CabbagePluginEditor* _owner)
	: owner(_owner),
    widgetData(wData),
	popupBubble(250),
	colour(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::colour)),
	fontColour(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::fontcolour)),
	textColour(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::textcolour)),
	trackerColour(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::trackercolour)),
	outlineColour(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::outlinecolour)),
	sliderType(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::kind))
{
	setName(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::name));
	widgetData.addListener(this);
	setLookAndFeelColours(widgetData);
	addAndMakeVisible(textLabel);
	addAndMakeVisible(&slider);
	slider.setName(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::name));
	initialiseSlider(wData);
	initialiseCommonAttributes(this, wData);
	createPopupBubble();
}

CabbageSlider::~CabbageSlider()
{
	
}

void CabbageSlider::createPopupBubble()
{
    //create popup display for showing value of sliders.
    popupBubble.setColour(BubbleComponent::backgroundColourId, Colours::white);
    popupBubble.setBounds(0, 0, 50, 20);
    owner->addChildComponent(popupBubble);
    popupBubble.setAlwaysOnTop(true);	
}

void CabbageSlider::initialiseSlider(ValueTree wData)
{
	sliderIncrement = CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::sliderincr);
	sliderSkew = CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::sliderskew);
	min = CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::min);
	max = CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::max);
	value = CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::value);
	shouldShowTextBox = CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::textbox);
	
	if(shouldShowTextBox<1)
    {
		slider.setTextBoxStyle (Slider::NoTextBox, true, 0, 0);
        shouldDisplayPopup=true;
    }
	
	slider.setSkewFactor(sliderSkew);
	slider.setRange(min, max, sliderIncrement);

	if(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::popuptext)=="0")
			shouldDisplayPopup=false;
			
    slider.setDoubleClickReturnValue(true, value);		
	setSliderVelocity(wData);	
	
	slider.setSliderStyle(Slider::RotaryVerticalDrag);
	slider.setValue(value, dontSendNotification);
	if(shouldShowTextBox>0)
		slider.setTextBoxStyle(Slider::TextBoxBelow, false, 40, 15);

	slider.setRotaryParameters(float_Pi * 1.2f, float_Pi * 2.8f, false);	
}

void CabbageSlider::resized()
{
        if (sliderType.contains("rotary"))
        {
                if(text.isNotEmpty())
                {
                    if(shouldShowTextBox>0)
                    {
                        textLabel.setBounds(0, 0, getWidth(), 20);
                        textLabel.setText(text, dontSendNotification);
                        textLabel.setJustificationType(Justification::centred);
                        textLabel.setVisible(true);
                        slider.setBounds(0, 20, getWidth(), getHeight()-20);
                    }
                    else
                    {
                        textLabel.setBounds(0, getHeight()-20, getWidth(), 20);
                        textLabel.setText(text, dontSendNotification);
                        textLabel.setJustificationType(Justification::centred);
                        textLabel.setVisible(true);
                        slider.setBounds(0, 0, getWidth(), getHeight()-15);
                    }
                }
                else
                    slider.setBounds(0, 0, getWidth(), getHeight());

        }	
}

void CabbageSlider::mouseDrag(const MouseEvent& event)
{
	if(shouldDisplayPopup)
		CabbageUtilities::debug("drag");
}

void CabbageSlider::mouseMove(const MouseEvent &event)
{
	if(shouldDisplayPopup)
		CabbageUtilities::debug("move");
}

void CabbageSlider::mouseEnter(const MouseEvent &event)
{
	if(shouldDisplayPopup)
		CabbageUtilities::debug("enter");
}

void CabbageSlider::setSliderVelocity(ValueTree wData)
{
	velocity = CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::velocity);
	if(velocity > 0)
	{
		slider.setVelocityModeParameters(velocity, 1, 0.0, true);
		slider.setVelocityBasedMode(true);
	}
	else
		slider.setVelocityBasedMode(false);	
}

void CabbageSlider::setLookAndFeelColours(ValueTree wData)
{
	textLabel.setColour(Label::textColourId, Colour::fromString(textColour));
	textLabel.setColour(Label::outlineColourId, Colours::transparentBlack);
	textLabel.setColour(Label::outlineColourId, Colours::transparentBlack);
	textLabel.setVisible(false);
	slider.setColour(Slider::textBoxHighlightColourId, Colours::lime.withAlpha(.2f));
	slider.setColour(Slider::thumbColourId, Colour::fromString(colour));
	slider.setColour(Label::textColourId, Colour::fromString(fontColour));
	slider.setColour(Label::backgroundColourId, CabbageUtilities::getBackgroundSkin());
	slider.setColour(TextEditor::textColourId, Colour::fromString(fontColour));
	slider.setColour(Slider::textBoxTextColourId, Colour::fromString(fontColour));
	slider.setColour(Slider::textBoxBackgroundColourId, Colours::black);
	slider.setColour(Slider::textBoxHighlightColourId, Colours::white);
	slider.setColour(Slider::trackColourId, Colour::fromString(trackerColour));
	slider.setColour(Label::outlineColourId, CabbageUtilities::getBackgroundSkin());
	slider.setColour(Slider::Slider::rotarySliderOutlineColourId, Colour::fromString(outlineColour));
	slider.setColour(Slider::rotarySliderFillColourId, Colour::fromString(colour));
}

//==============================================================================
void CabbageSlider::valueTreePropertyChanged (ValueTree& valueTree, const Identifier& prop)
{

    if(prop==CabbageIdentifierIds::value)
    {

    }
	
}