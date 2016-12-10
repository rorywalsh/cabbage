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
	float sliderIncr, value, skew, startingValue, sliderPos, currentValue, max, min, maxEnabled, minEnabled;
	int yAxis, shouldShowTextBox, height, line, progress, decimalPlaces;
	String outlinecolour, colour, trackercolour, text, textcolour, popupText;
	BubbleMessageComponent popupBubble;
	
public:

    CabbageEncoder(ValueTree wData, CabbagePluginEditor* _owner);
    ~CabbageEncoder() {};

	CabbagePluginEditor* owner;
	
    //ValueTree::Listener virtual methods....
    void valueTreePropertyChanged (ValueTree& valueTree, const Identifier&);
    void valueTreeChildAdded (ValueTree&, ValueTree&)override {};
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override {};

	void labelTextChanged (Label *label);
    void mouseDown(const MouseEvent &e);
    void mouseEnter(const MouseEvent &e);
    void mouseDrag(const MouseEvent& e);
    void mouseExit(const MouseEvent &e);
    void paint(Graphics &g);
    void showPopup();
    void resized();
	
	void showPopupBubble();
	void createPopupBubble();
	
	ValueTree widgetData;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabbageEncoder);
};



#endif  // CABBAGEENCODER_H_INCLUDED
