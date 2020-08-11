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
#include "../LookAndFeel/FlatButtonLookAndFeel.h"

class CabbagePluginEditor;
class CabbageRangeSlider;

class RangeSlider  : public Slider
{
    CabbageRangeSlider* owner;
public:
    RangeSlider (CabbageRangeSlider* owner);

    ~RangeSlider();

private:
    void mouseDown (const MouseEvent& event) override;
    void mouseExit (const MouseEvent& event) override;
    void mouseDrag (const MouseEvent& event) override;
    void mouseEnter (const MouseEvent& event) override;
    //void valueChanged() override {};

    bool mouseDragBetweenThumbs;
    float xMinAtThumbDown = 0;
    float xMaxAtThumbDown = 0;
    float yMinAtThumbDown = 0;
    float yMaxAtThumbDown = 0;
    
};

class CabbageRangeSlider  : public Component, public ValueTree::Listener, public CabbageWidgetBase
{
    CabbagePluginEditor* owner;
    String name, text, textColour;
    RangeSlider slider;
    Label textLabel;
    bool isVertical;
    bool shouldDisplayPopup;
    float minValue, maxValue, min, max, decimalPlaces, sliderIncrement, sliderSkew;
    BubbleMessageComponent popupBubble;
    String prefix  = "";
    String postfix = "";
    String escapedPrefix  = "";
    String escapedPostfix = "";

    void createPopupBubble();
    void setLookAndFeelColours (ValueTree wData);
    void setSliderValues (ValueTree wData);
    FlatButtonLookAndFeel flatLookAndFeel;

public:
    CabbageRangeSlider (ValueTree wData, CabbagePluginEditor* _owner);
    ~CabbageRangeSlider() {slider.setLookAndFeel (nullptr); setLookAndFeel(nullptr);};

    void setCurrentValues (float min, float max);
    void resized()  override;
    void showPopup (int displayTime);
    //ValueTree::Listener virtual methods....
    void valueTreePropertyChanged (ValueTree& valueTree, const Identifier&)  override;
    void valueTreeChildAdded (ValueTree&, ValueTree&)override {};
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override {};

    RangeSlider& getSlider()
    {
        return slider;
    }

    ValueTree widgetData;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabbageRangeSlider);

};



#endif  // CABBAGERANGESLIDER_H_INCLUDED
