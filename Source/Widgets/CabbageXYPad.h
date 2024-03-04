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
class CabbagePluginParameter;
class CabbagePluginProcessor;

// Add any new custom widgets here to avoid having to edit makefiles and projects
// Each Cabbage widget should inherit from ValueTree listener, and CabbageWidgetBase
class CabbageXYPad
    : public Component,
      public ValueTree::Listener,
      public CabbageWidgetBase,
      public ChangeListener
{
    CabbagePluginEditor* owner;
    Colour fontColour, textColour, colour, bgColour, ballColour, outlineColour;
   juce::Rectangle<float> xyPadRect;
    Line<float> dragLine;
    bool isAutomating = false;
    Label xValueLabel, yValueLabel;

    Slider xAxis, yAxis;
    float minX, maxX, minY, maxY, valueX, valueY;
    bool rightMouseButtonDown = false;

    juce::Point<float> currentMouseXY;
    juce::Point<float> mouseDownXY;
    
    String xPrefix = "";
    String xPostfix = "";
    String yPrefix = "";
    String yPostfix = "";

    class XYBall : public Component
    {
        juce::Point<int> ballXY;
        Colour colour;

    public:
        void setColour (Colour col)
        {
            colour = col;
            repaint();
        }
        XYBall() {}
        void paint (Graphics& g)  override
        {
            g.fillAll (Colours::transparentBlack);
            g.setColour (colour);
            g.fillEllipse (getLocalBounds().toFloat());
        }
    };

    XYBall ball;
    int outlineThickness = 0;
public:

    CabbageXYPad (ValueTree wData, CabbagePluginEditor* editor);
    ~CabbageXYPad() override;
    //ValueTree::Listener virtual methods....
    void valueTreePropertyChanged (ValueTree& valueTree, const Identifier&) override;
    void valueTreeChildAdded (ValueTree&, ValueTree&)override {}
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override {}

    void changeListenerCallback (ChangeBroadcaster* source) override;
    ValueTree widgetData;

    void paint (Graphics& g) override;
    void resized() override;
    void mouseDown (const MouseEvent& e) override;
    void mouseDrag (const MouseEvent& e) override;
    void mouseUp (const MouseEvent& e) override;

    Slider& getSliderX()
    {
        return xAxis;
    }
    Slider& getSliderY()
    {
        return yAxis;
    }
   juce::Rectangle<float> getXYPadRect()
    {
        return xyPadRect;
    }

    void setValues (float x, float y, bool notify = true);

    juce::Point<int> constrainPosition (float x, float y);
    juce::Point<float> getPositionAsValue (juce::Point<float> position);
    juce::Point<float> getValueAsPosition (juce::Point<float> position);
    void setPositionAsValue (juce::Point<float> position);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabbageXYPad)
};

//=============================================================================
class XYPadAutomator : public ChangeBroadcaster, public Timer
{
    String name;
    CabbagePluginParameter* xParam, *yParam;
    float xValue = 0;
    float yValue = 0;
    float xValueIncrement = 1;
    float yValueIncrement = 1;
    Line<float> dragLine;
    bool isPluginEditorOpen = false;
    bool repaintBackground = false;
    juce::Point<double> position;
    float xMin, xMax, yMin, yMax, velocity;
    CabbagePluginProcessor* owner;

public:
    XYPadAutomator (String name, CabbagePluginParameter* xParam, CabbagePluginParameter* yParam, CabbagePluginProcessor* _owner);

    ~XYPadAutomator() override
    {
        stopTimer();
        removeAllChangeListeners();
    }

    void timerCallback() override;
    String getName()
    {
        return name;
    }
    void setDragLine (const Line<float>& newDragLine)
    {
        this->dragLine = newDragLine;
    }
    void setIsPluginEditorOpen (bool isEditorOpen)
    {
        this->isPluginEditorOpen = isEditorOpen;
    }
    void setName (const String& newName)
    {
        this->name = newName;
    }
    void setPosition (const juce::Point<double>& newPosition)
    {
        this->position = newPosition;
    }
    void setRepaintBackground (bool paintBackground)
    {
        this->repaintBackground = paintBackground;
    }
    void setVelocity (float newVelocity)
    {
        this->velocity = newVelocity;
    }
    void setXMax (float newXMax)
    {
        this->xMax = newXMax;
    }
    void setXMin (float newXMin)
    {
        this->xMin = newXMin;
    }
    void setXValue (float newXValue)
    {
        this->xValue = newXValue;
    }
    void setXValueIncrement (float newXValueIncrement)
    {
        this->xValueIncrement = newXValueIncrement;
    }
    void setYMax (float newYMax)
    {
        this->yMax = newYMax;
    }
    void setYMin (float newYMin)
    {
        this->yMin = newYMin;
    }
    void setYValue (float newYValue)
    {
        this->yValue = newYValue;
    }
    void setYValueIncrement (float newYValueIncrement)
    {
        this->yValueIncrement = newYValueIncrement;
    }
    bool getIsPluginEditorOpen() const
    {
        return isPluginEditorOpen;
    }
    const juce::Point<double>& getPosition() const
    {
        return position;
    }
    bool getShouldRepaintBackground() const
    {
        return repaintBackground;
    }
    float getXValueIncrement() const
    {
        return xValueIncrement;
    }

};

#endif  // CABBAGEXYPAD_H_INCLUDED
