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

CabbageSlider::CabbageSlider(ValueTree wData, CabbagePluginEditor* _owner)
	: owner(_owner),
    widgetData(wData),
	popupBubble(250),
	colour(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::colour)),
	fontColour(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::fontcolour)),
	textColour(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::textcolour)),
	trackerColour(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::trackercolour)),
	outlineColour(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::outlinecolour)),
	sliderType(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::kind)),
	channel(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::channel))
{
	setName(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::name));
	widgetData.addListener(this);
	setLookAndFeelColours(widgetData);
	addAndMakeVisible(textLabel);
	addAndMakeVisible(&slider);
	slider.setName(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::name));
	initialiseSlider(wData);
	textLabel.setVisible(false);
	initialiseCommonAttributes(this, wData);
	createPopupBubble();
	setImgProperties(this->slider, wData, "slider");
    setImgProperties(this->slider, wData, "sliderbg");
	slider.setLookAndFeel(&owner->getLookAndFeel());
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
	popupBubble.setVisible(false);
    popupBubble.setAlwaysOnTop(true);	
}

void CabbageSlider::initialiseSlider(ValueTree wData)
{
	decimalPlaces = CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::decimalplaces);
	sliderIncrement = CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::sliderincr);
	sliderSkew = CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::sliderskew);
	min = CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::min);
	max = CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::max);
	value = CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::value);
	shouldShowTextBox = CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::valuetextbox);
	trackerThickness = CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::trackerthickness);
	
	slider.getProperties().set("trackerthickness", trackerThickness);
	
	slider.setSkewFactor(sliderSkew);
	slider.setRange(min, max, sliderIncrement);

	if(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::popuptext)=="0")
		shouldDisplayPopup=false;
	
	if(getCurrentText(wData).isNotEmpty())
		textLabel.setVisible(true);
	else
		textLabel.setVisible(false);
		
    slider.setDoubleClickReturnValue(true, value);		
	setSliderVelocity(wData);	
	slider.addMouseListener(this, false);
	
	setTextBoxOrientation(sliderType, shouldShowTextBox);
	
	slider.setRotaryParameters(float_Pi * 1.2f, float_Pi * 2.8f, false);	
	
	if(sliderType.contains("rotary"))
		slider.setSliderStyle(Slider::RotaryVerticalDrag);
	else if(sliderType.contains("vertical"))
		slider.setSliderStyle(Slider::LinearVertical);
	else if(sliderType.contains("horizontal"))
		slider.setSliderStyle(Slider::LinearHorizontal);	
		
		
}

void CabbageSlider::setTextBoxOrientation(String type, bool shouldShowTextBox)
{
	if(shouldShowTextBox>0)
	{
		shouldDisplayPopup=false;
		if(type.contains("horizontal"))
			slider.setTextBoxStyle(Slider::TextBoxRight, false, 40, 15);		
		else
			slider.setTextBoxStyle(Slider::TextBoxBelow, false, 40, 15);
			
	}	
	else
	{
		slider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
		shouldDisplayPopup=true;
	}
}

void CabbageSlider::resized()
{
	if (sliderType.contains("rotary"))
	{
			if(_text.isNotEmpty())
			{
				
				textLabel.setText(_text, dontSendNotification);
				textLabel.setJustificationType(Justification::centred);
				textLabel.setVisible(true);
					
				if(shouldShowTextBox>0)
				{
					textLabel.setBounds(0, 0, getWidth(), 20);
					slider.setBounds(0, 20, getWidth(), getHeight()-20);
				}
				else
				{
					textLabel.setBounds(0, getHeight()-20, getWidth(), 20);
					slider.setBounds(0, 0, getWidth(), getHeight()-15);
				}
			}
			else
				slider.setBounds(0, 0, getWidth(), getHeight());

	}	
	
	//else if vertical
	else if (sliderType.contains("vertical"))
	{
		if(_text.isNotEmpty())
		{
			textLabel.setJustificationType(Justification::centred);
			textLabel.setText(_text, dontSendNotification);
			textLabel.setVisible(true);
			
			if(shouldShowTextBox>0)
			{
				textLabel.setBounds(0, 1, getWidth(), 20);			
				slider.setBounds(0, 20, getWidth(), getHeight()-20);				
			}
			else
			{
			textLabel.setBounds(0, getHeight()-20, getWidth(), 20);			
			slider.setBounds(0, 0, getWidth(), getHeight()-20);
			}
		}
		else
		{
			slider.setBounds(0, 0, getWidth(), getHeight());
		}
	}
	
	//else if horizontal
	else
	{
		if(_text.isNotEmpty())
		{
			const float width = textLabel.getFont().getStringWidthFloat(_text)+10.f;
			textLabel.setText(_text, dontSendNotification);
			textLabel.setVisible(true);
			
			if(shouldShowTextBox>0)
			{
				textLabel.setBounds(0, 0, width, getHeight());
				slider.setBounds(width-3, 0, getWidth()-(width-4), getHeight());	
			}
			else
			{
				textLabel.setBounds(0, 0, width, getHeight());
				slider.setBounds(width-3, 0, getWidth()-(width-4), getHeight());				
			}
		}
		else
			slider.setBounds(0, 0, getWidth(), getHeight());

	}

	slider.setValue(value, dontSendNotification);	
}

void CabbageSlider::showPopupBubble()
{
	if(_tooltipText.isNotEmpty())
		popupText = _tooltipText;
	else
		popupText = channel+": "+String(CabbageUtilities::roundToPrec(slider.getValue(), decimalPlaces));

	popupBubble.showAt(&slider, AttributedString(popupText), 150);	
	
}

void CabbageSlider::mouseDrag(const MouseEvent& event)
{
	if(shouldDisplayPopup)
		showPopupBubble();
}

void CabbageSlider::mouseMove(const MouseEvent &event)
{
	if(shouldDisplayPopup)
		showPopupBubble();
}

void CabbageSlider::mouseEnter(const MouseEvent &event)
{
	if(shouldDisplayPopup)
		showPopupBubble();
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
	
	textLabel.setColour(Label::outlineColourId, Colours::transparentBlack);
	slider.setColour(Slider::textBoxHighlightColourId, Colours::lime.withAlpha(.2f));
	
	slider.setColour(Slider::thumbColourId, Colour::fromString(colour));
	slider.setColour(Slider::trackColourId, Colour::fromString(trackerColour));
	slider.setColour(Slider::Slider::rotarySliderOutlineColourId, Colour::fromString(outlineColour));
	slider.setColour(Slider::rotarySliderFillColourId, Colour::fromString(colour));
	slider.setColour(TextEditor::textColourId, Colour::fromString(fontColour));
	textLabel.setColour(Label::textColourId, Colour::fromString(textColour));
	
	slider.setColour(Slider::textBoxTextColourId, Colour::fromString(fontColour));
	slider.setColour(Slider::textBoxBackgroundColourId, Colours::black);
	slider.setColour(Slider::textBoxHighlightColourId, Colours::white);

	slider.setColour(Label::textColourId, Colour::fromString(fontColour));
	slider.setColour(Label::backgroundColourId, CabbageUtilities::getBackgroundSkin());
	
	slider.setColour(Label::outlineColourId, CabbageUtilities::getBackgroundSkin());	
	repaint();
}

//==============================================================================
void CabbageSlider::valueTreePropertyChanged (ValueTree& valueTree, const Identifier& prop)
{

    if(prop==CabbageIdentifierIds::value)
    {
		slider.setValue(CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::value), dontSendNotification);
    }
	else
	{
		setLookAndFeelColours(valueTree);

		textLabel.setText(getCurrentText(valueTree), dontSendNotification);
		textLabel.setVisible(getCurrentText(valueTree).isNotEmpty() ? true : false);
		shouldShowTextBox = CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::valuetextbox);
		setTextBoxOrientation(sliderType, shouldShowTextBox);
		slider.setTooltip(getCurrentPopupText(valueTree));		
		handleCommonUpdates(this, valueTree);			
		resized();
	}
	
}