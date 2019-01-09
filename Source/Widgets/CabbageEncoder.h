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

#ifndef CABBAGEENCODER_H_INCLUDED
#define CABBAGEENCODER_H_INCLUDED


#include "../CabbageCommonHeaders.h"
#include "CabbageWidgetBase.h"

class CabbagePluginEditor;

class CabbageEncoder : public Component, public ValueTree::Listener, public CabbageWidgetBase, public Label::Listener
{
    bool isMouseOver = false;
    bool shouldDisplayPopup = true;
    Rectangle<float> slider;
    Label textLabel, valueLabel;
    float sliderIncr = 0;
    float value = 0;
    float skew = 0;
    float startingValue = 0;
    float sliderPos = 0;
    float currentEncValue = 0;
    float max = 0;
    float min = 0;
    float maxEnabled = 0;
    float minEnabled = 0;
    int yAxis = 0;
    int shouldShowValueTextBox = 0;
    int height = 10;
    int line = 1;
    int progress = 0;
    int decimalPlaces = 1;
    String outlinecolour, colour, trackercolour, text, textcolour, popupText;
    BubbleMessageComponent popupBubble;
    bool flatStyle = false;

public:

    CabbageEncoder (ValueTree wData, CabbagePluginEditor* _owner);
    ~CabbageEncoder() {};

    CabbagePluginEditor* owner;

    //ValueTree::Listener virtual methods....
    void valueTreePropertyChanged (ValueTree& valueTree, const Identifier&) override;
    void valueTreeChildAdded (ValueTree&, ValueTree&)override {};
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override {};

    void labelTextChanged (Label* label);
    void mouseDown (const MouseEvent& e)  override;
    void mouseEnter (const MouseEvent& e) override;
    void mouseDrag (const MouseEvent& e) override;
    void mouseExit (const MouseEvent& e) override;
    void mouseWheelMove (const MouseEvent& event, const MouseWheelDetails& wheel);
    void paint (Graphics& g) override;
    void showPopup (int displayTime = 250);
    void resized() override;

    void createPopupBubble();

    ValueTree widgetData;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabbageEncoder);
};



#endif  // CABBAGEENCODER_H_INCLUDED
