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

#ifndef CABBAGESLIDER_H_INCLUDED
#define CABBAGESLIDER_H_INCLUDED

#include "../CabbageCommonHeaders.h"
#include "CabbageWidgetBase.h"

class CabbagePluginEditor;

class CabbageSlider : public Slider, public ValueTree::Listener, public CabbageWidgetBase
{
    CabbagePluginEditor* owner;
    ValueTree widgetData;	
	ScopedPointer<Label> textLabel;
	float velocity, sliderIncrement, sliderSkew;
	String colour, fontColour, textColour, outlineColour, sliderType, trackerColour;
	
public:
	CabbageSlider(ValueTree cAttr, CabbagePluginEditor* _owner);
	~CabbageSlider();
	
	void setLookAndFeelColours(ValueTree wData);
	void setSliderVelocity(ValueTree wData);
	
};



#endif  // CABBAGESLIDER_H_INCLUDED
