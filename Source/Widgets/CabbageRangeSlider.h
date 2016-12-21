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

#ifndef CABBAGERANGESLIDER_H_INCLUDED
#define CABBAGERANGESLIDER_H_INCLUDED

#include "../CabbageCommonHeaders.h"
#include "CabbageWidgetBase.h"

//============================================================
// homegrown version of JUCE's two value, slider with dragable range
// see below for CabbageRangeSlider class...
//============================================================
class RangeSlider	:	public Component
{
    String name, tooltipText;
    StringArray channels;
    bool shouldDisplayPopup, isVertical;
    String colour, text;
    Colour trackerColour;
    double min, trackerThickness, initVal, minVal, maxVal, max, incr, skew;
    float thumbIncr;
    int thumbWidth, thumbHeight, rangeDistance, currentThumb, index;
    float width, height;


public:
    Rectangle<int> sliderThumbBounds;

    class SliderThumb : public Component
    {
        Colour colour;
		bool isVertical;

    public:
        SliderThumb(bool certical);
        ~SliderThumb(){}
        void resized(){}
        void paint(Graphics& g);
        void setColour(Colour _colour)
        {
            colour = _colour;
        };
    };


    RangeSlider(ValueTree wData);
    ~RangeSlider(){}

    enum SliderType
    {
        left,
        right,
        top,
        bottom
    };


    void setIndex(int _index)
    {
        index = _index;
    }

    void setValue(float val1, float val2);
    void resized();
    void mouseDown(const MouseEvent& event);
    void mouseUp(const MouseEvent& event);
    void mouseEnter(const MouseEvent& event);
    void mouseDrag(const MouseEvent& event);
    void mouseMove(const MouseEvent& event);
    void horizontalDrag(const MouseEvent& event);
    void verticalDrag(const MouseEvent& event);
    void paint(Graphics& g);
    int getSliderPosition(SliderType type);
    void showPopup();
    double getSkewedValue(double proportion);
    int getSkewedPosition(double proportion);
    void sendValuesToCsound(double val1, double val2, int thumb);

    Rectangle<int> getThumbOuterBounds(int type);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RangeSlider);

private:
    SliderThumb sliderThumb;
};

//========================================================================================
class CabbageRangeSlider : public Slider, public ValueTree::Listener, public CabbageWidgetBase
{
    String name, text, textColour;
    RangeSlider slider;
    Label textLabel;
    bool isVertical;
    float initVal1, initVal2, min, max;
	
public:

    CabbageRangeSlider(ValueTree wData);
    ~CabbageRangeSlider() {};
	
	void resized();

    //ValueTree::Listener virtual methods....
    void valueTreePropertyChanged (ValueTree& valueTree, const Identifier&);
    void valueTreeChildAdded (ValueTree&, ValueTree&)override {};
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override {};

    ValueTree widgetData;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabbageRangeSlider);
};


#endif  // CABBAGERANGESLIDER_H_INCLUDED
