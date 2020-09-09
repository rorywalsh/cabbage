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
class AudioProccessor;

// Add any new custom widgets here to avoid having to edit makefiles and projects
// Each Cabbage widget should inherit from ValueTree listener, and CabbageWidgetBase
class CabbageXYPad
    : public Component,
      public ValueTree::Listener,
      public CabbageWidgetBase,
      public ChangeListener
{
    CabbagePluginEditor* owner;
    Colour fontColour, textColour, colour, bgColour, ballColour;
    Rectangle<float> xyPadRect;
    Line<float> dragLine;
    bool isAutomating = false;
    Label xValueLabel, yValueLabel;

    Slider xAxis, yAxis;
    float minX, maxX, minY, maxY, valueX, valueY;
    bool rightMouseButtonDown = false;

    Point<float> currentMouseXY;
    Point<float> mouseDownXY;
    
    String xPrefix = "";
    String xPostfix = "";
    String yPrefix = "";
    String yPostfix = "";

    class XYBall : public Component
    {
        Point<int> ballXY;
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
public:

    CabbageXYPad (ValueTree wData, CabbagePluginEditor* editor);
    ~CabbageXYPad();
    //ValueTree::Listener virtual methods....
    void valueTreePropertyChanged (ValueTree& valueTree, const Identifier&) override;
    void valueTreeChildAdded (ValueTree&, ValueTree&)override {};
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override {};

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
    Rectangle<float> getXYPadRect()
    {
        return xyPadRect;
    }

    void setValues (float x, float y, bool notify = true);

    Point<int> constrainPosition (float x, float y);
    Point<float> getPositionAsValue (Point<float> position);
    Point<float> getValueAsPosition (Point<float> position);
    void setPositionAsValue (Point<float> position);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabbageXYPad);
};

//=============================================================================
class XYPadAutomator : public ChangeBroadcaster, public Timer
{
    String name;
    CabbageAudioParameter* xParam, *yParam;
    float xValue = 0;
    float yValue = 0;
    float xValueIncrement = 1;
    float yValueIncrement = 1;
    Line<float> dragLine;
    bool isPluginEditorOpen = false;
    bool repaintBackground = false;
    Point<double> position;
    float xMin, xMax, yMin, yMax, velocity;
    AudioProcessor* owner;

public:
    XYPadAutomator (String name, CabbageAudioParameter* xParam, CabbageAudioParameter* yParam, AudioProcessor* _owner);

    ~XYPadAutomator()
    {
        stopTimer();
        removeAllChangeListeners();
    }

    void timerCallback();
    String getName()
    {
        return name;
    }
    void setDragLine (const Line<float>& lineToDrag)
    {
        this->dragLine = lineToDrag;
    }
    void setIsPluginEditorOpen (bool pluginEditorIsOpen)
    {
        this->isPluginEditorOpen = pluginEditorIsOpen;
    }
    void setName (const String& nameToUse)
    {
        this->name = nameToUse;
    }
    void setPosition (const Point<double>& positionToUse)
    {
        this->position = positionToUse;
    }
    void setRepaintBackground (bool backgroundToPaint)
    {
        this->repaintBackground = backgroundToPaint;
    }
    void setVelocity (float velocityToUse)
    {
        this->velocity = velocityToUse;
    }
    void setXMax (float xMaxToUse)
    {
        this->xMax = xMaxToUse;
    }
    void setXMin (float xMinToUse)
    {
        this->xMin = xMinToUse;
    }
    void setXValue (float xValueToUse)
    {
        this->xValue = xValueToUse;
    }
    void setXValueIncrement (float xValueIncrementToUse)
    {
        this->xValueIncrement = xValueIncrementToUse;
    }
    void setYMax (float yMaxToUse)
    {
        this->yMax = yMaxToUse;
    }
    void setYMin (float yMinToUse)
    {
        this->yMin = yMinToUse;
    }
    void setYValue (float yValueToUse)
    {
        this->yValue = yValueToUse;
    }
    void setYValueIncrement (float yValueIncrementToUse)
    {
        this->yValueIncrement = yValueIncrementToUse;
    }
    bool getIsPluginEditorOpen() const
    {
        return isPluginEditorOpen;
    }
    const Point<double>& getPosition() const
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
