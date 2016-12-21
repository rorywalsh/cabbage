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

  Based on the XYPad by Damien Rennick
   
*/

#ifndef CABBAGEXYPAD_H_INCLUDED
#define CABBAGEXYPAD_H_INCLUDED


#include "../CabbageCommonHeaders.h"
#include "CabbageWidgetBase.h"

class CabbagePluginEditor;
class CabbageAudioParameter;

// Add any new custom widgets here to avoid having to edit makefiles and projects
// Each Cabbage widget should inherit from ValueTree listener, and CabbageWidgetBase
class CabbageXYPad 
	: public Component, 
	public ValueTree::Listener, 
	public CabbageWidgetBase,
	public ChangeListener
{
	CabbagePluginEditor* owner;
	Colour fontColour;
	Rectangle<float> xyPadRect;
	Line<float> dragLine;
	bool isAutomating = false;
	Label xValueLabel, yValueLabel;

	Slider xAxis, yAxis;
	float minX, maxX, minY, maxY, valueX, valueY;
	bool rightMouseButtonDown = false;

	Point<float> currentMouseXY;
	Point<float> mouseDownXY;

	class XYBall : public Component
	{
		Point<int> ballXY;
		
		public:
		
			XYBall(){}
			void paint(Graphics& g)
			{
				g.fillAll(Colours::transparentBlack);
				g.setColour(Colours::red);
				g.fillEllipse(getLocalBounds().toFloat());
			}		
	};

	XYBall ball;
public:

    CabbageXYPad(ValueTree wData, CabbagePluginEditor* editor);
    ~CabbageXYPad() {}
    //ValueTree::Listener virtual methods....
    void valueTreePropertyChanged (ValueTree& valueTree, const Identifier&);
    void valueTreeChildAdded (ValueTree&, ValueTree&)override {};
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override {};
	
	void changeListenerCallback(ChangeBroadcaster* source);
	Rectangle<float> getXYPadRect(){	return xyPadRect;	}
	void setValues(float x, float y);

    ValueTree widgetData;
	
	void paint(Graphics& g);
	void resized();
	void mouseDown(const MouseEvent& e);
	void mouseDrag(const MouseEvent& e);
	void mouseUp(const MouseEvent& e);
	
	Slider& getSliderX(){	return xAxis;	}
	Slider& getSliderY(){	return yAxis;	}
	
	Point<int> constrainPosition(float x, float y);
	Point<float> getPositionAsValue(Point<float> position);
	Point<float> getValueAsPosition(Point<float> position);
	
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabbageXYPad);
};

class XYPadAutomator : public ChangeBroadcaster, public Timer
{
String name;
CabbageAudioParameter *xParam, *yParam;

public:
	XYPadAutomator(String name, CabbageAudioParameter* xParam, CabbageAudioParameter* yParam)
	:name(name), xParam(xParam), yParam(yParam){}
	
	~XYPadAutomator()
	{
		removeAllChangeListeners();	
	}
	
	String getName(){	return name;	}
	void timerCallback();
	
	bool repaintBackground = false;
	Point<double> position;
	float xMin, xMax, yMin, yMax, velocity;
	float xValue = 0;
	float yValue = 0;
	float xValueIncrement = 1;
	float yValueIncrement = 1;
	Line<float> dragLine;
	bool isPluginEditorOpen = false;
};

#endif  // CABBAGEXYPAD_H_INCLUDED
