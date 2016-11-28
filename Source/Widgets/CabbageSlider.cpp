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

CabbageSlider::CabbageSlider(ValueTree wData, CabbagePluginEditor* _owner)
	: owner(_owner),
    widgetData(wData),
	textLabel(new Label()),
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
	
	sliderIncrement = cAttr.getNumProp(CabbageIDs::sliderincr);
	sliderSkew = cAttr.getNumProp(CabbageIDs::sliderskew);
	setSkewFactor(cAttr.getNumProp(CabbageIDs::sliderskew));
	setRange(cAttr.getNumProp(CabbageIDs::min), cAttr.getNumProp(CabbageIDs::max), cAttr.getNumProp(CabbageIDs::sliderincr));

	if(cAttr.getStringProp(CabbageIDs::popuptext).isNotEmpty())
	{
		if(cAttr.getStringProp(CabbageIDs::popuptext)=="0")
			shouldDisplayPopup=false;
		else
			tooltipText = cAttr.getStringProp(CabbageIDs::popuptext);

	}

        slider->setDoubleClickReturnValue(true, cAttr.getNumProp(CabbageIDs::value));	
	
	initialiseCommonAttributes(this, wData);
}

CabbageSlider::~CabbageSlider()
{
	
}

void CabbageSlider::setSliderVelocity(ValueTree wData)
{
	velocity = CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::velocity);
	if(velocity > 0)
	{
		setVelocityModeParameters(velocity, 1, 0.0, true);
		setVelocityBasedMode(true);
	}
	else
		setVelocityBasedMode(false);	
}

void CabbageSlider::setLookAndFeelColours(ValueTree wData)
{
	textLabel->setColour(Label::textColourId, Colour::fromString(textColour));
	textLabel->setColour(Label::outlineColourId, Colours::transparentBlack);
	setColour(Slider::textBoxHighlightColourId, Colours::lime.withAlpha(.2f));
	setColour(Slider::thumbColourId, Colour::fromString(colour));
	setColour(Label::textColourId, Colour::fromString(fontColour));
	setColour(Label::backgroundColourId, CabbageUtilities::getBackgroundSkin());
	setColour(TextEditor::textColourId, Colour::fromString(fontColour));
	setColour(Slider::textBoxTextColourId, Colour::fromString(fontColour));
	setColour(Slider::textBoxBackgroundColourId, Colours::black);
	setColour(Slider::textBoxHighlightColourId, Colours::white);
	setColour(Slider::trackColourId, Colour::fromString(trackerColour));
	setColour(Label::outlineColourId, CabbageUtilities::getBackgroundSkin());
	setColour(Slider::Slider::rotarySliderOutlineColourId, Colour::fromString(outlineColour));
	textLabel->setColour(Label::outlineColourId, Colours::transparentBlack);
	textLabel->setVisible(false);
}