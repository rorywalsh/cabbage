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

#include "CabbageXYPad.h"
#include "../Audio/Plugins/CabbagePluginEditor.h"
  
CabbageXYPad::CabbageXYPad (ValueTree wData, CabbagePluginEditor* editor)
    : owner (editor),
    fontColour (Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::fontcolour))),
    textColour (Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::textcolour))),
    colour (Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::colour))),
    bgColour (Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::backgroundcolour))),
    ballColour (Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::ballcolour))),
    xValueLabel(),
    yValueLabel(),
    minX (CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::minx)),
    maxX (CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::maxx)),
    minY (CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::miny)),
    maxY (CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::maxy)),
    valueX (CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::valuex)),
    valueY (CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::valuey)),
    ball(),
    widgetData (wData)
{
    setName (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::name));
    widgetData.addListener (this);              //add listener to valueTree so it gets notified when a widget's property changes
    initialiseCommonAttributes (this, wData);   //initialise common attributes such as bounds, name, rotation, etc..

    const juce::Point<float> pos (getValueAsPosition (juce::Point<float> (valueX, valueY)));
    ball.setBounds (pos.getX(), pos.getY(), 20, 20);
    ball.setInterceptsMouseClicks (false, false);
    addAndMakeVisible (ball);

    xAxis.setRange (minX, maxX);
    xAxis.setName (getName() + "_x");
    yAxis.setRange (minY, maxY);
    yAxis.setName (getName() + "_y");

    ball.setColour (ballColour);
    
    const auto prefixes = CabbageWidgetData::getProperty (wData, CabbageIdentifierIds::valueprefix);
    if (prefixes.isArray())
    {
        xPrefix = prefixes[0];
        if (prefixes.size() > 1)
        {
            yPrefix = prefixes[1];
        }
    }
    
    const auto postfixes = CabbageWidgetData::getProperty (wData, CabbageIdentifierIds::valuepostfix);
    if (postfixes.isArray())
    {
        xPostfix = postfixes[0];
        if (postfixes.size() > 1)
        {
            yPostfix = postfixes[1];
        }
    }

    xValueLabel.setColour (Label::textColourId, fontColour);
    yValueLabel.setColour (Label::textColourId, fontColour);
    xValueLabel.setJustificationType (Justification::centred);
    xValueLabel.setText (createValueText(valueX, 3, xPrefix, xPostfix), dontSendNotification);

    yValueLabel.setText (createValueText(valueY, 3, yPrefix, yPostfix), dontSendNotification);
    yValueLabel.setJustificationType (Justification::centred);
    xValueLabel.setFont (Font (12, 1));
    yValueLabel.setFont (Font (12, 1));
    addAndMakeVisible (xValueLabel);
    addAndMakeVisible (yValueLabel);
}

CabbageXYPad::~CabbageXYPad()
{
    widgetData.removeListener(this);
    CabbageUtilities::debug ("Existing xypad");
}
//==================================================================
void CabbageXYPad::mouseDown (const MouseEvent& e)
{
    xAxis.startedDragging();
    yAxis.startedDragging();
    owner->enableXYAutomator (getName(), false);
    ball.setTopLeftPosition (juce::Point<int> (e.getPosition().getX() - ball.getWidth()*.5f, e.getPosition().getY() - ball.getWidth()*.5f));
    mouseDownXY.setXY (ball.getPosition().getX() + ball.getWidth()*.5f, ball.getPosition().getY() + ball.getHeight()*.5f);
    setPositionAsValue (ball.getPosition().toFloat());
    isAutomating = false;
    repaint();
}

void CabbageXYPad::mouseDrag (const MouseEvent& e)
{
    if (e.mouseWasDraggedSinceMouseDown())
    {
        ball.setTopLeftPosition (constrainPosition (mouseDownXY.getX() + e.getDistanceFromDragStartX(), mouseDownXY.getY() + e.getDistanceFromDragStartY()));
        setPositionAsValue (ball.getPosition().toFloat());
        repaint();

        currentMouseXY = ball.getPosition().toFloat();

        if (e.mods.isRightButtonDown())
            rightMouseButtonDown = true;
    }
}

void CabbageXYPad::mouseUp (const MouseEvent& e)
{
    if (e.mods.isRightButtonDown())
    {
        rightMouseButtonDown = false;

        const juce::Point<float> valueStart (getPositionAsValue (mouseDownXY));
        const juce::Point<float> valueEnd (getPositionAsValue (e.getPosition().toFloat()));

        dragLine = Line<float> (valueStart.getX(), valueStart.getY(), valueEnd.getX(), valueEnd.getY());
        owner->enableXYAutomator (getName(), true, dragLine);
        isAutomating = true;
    }
    
    xAxis.stoppedDragging();
    yAxis.stoppedDragging();
}

void CabbageXYPad::changeListenerCallback (ChangeBroadcaster* source)
{
    if (XYPadAutomator* xyAuto = dynamic_cast<XYPadAutomator*> (source))
    {
        juce::Point<float> pos (getValueAsPosition (juce::Point<float> (xyAuto->getPosition().getX(), xyAuto->getPosition().getY())));
        pos.addXY (-ball.getWidth() / 2, -ball.getWidth() / 2);
        ball.setBounds (pos.getX(), pos.getY(), 20, 20);

        setValues (xyAuto->getPosition().getX(), xyAuto->getPosition().getY());

        if (xyAuto->getShouldRepaintBackground() == true)
        {
            repaint();
            xyAuto->setRepaintBackground (false);
        }
    }
}
//==================================================================
void CabbageXYPad::valueTreePropertyChanged (ValueTree& valueTree, const Identifier& prop)
{
    if (prop != CabbageIdentifierIds::valuex && prop != CabbageIdentifierIds::valuey) //not updating xy values here...
    {
        handleCommonUpdates (this, valueTree, false, prop);      //handle comon updates such as bounds, alpha, rotation, visible, etc
        fontColour = Colour::fromString (CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::fontcolour));
        textColour = Colour::fromString (CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::textcolour));
        colour = Colour::fromString (CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::colour));
        ballColour = Colour::fromString (CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::ballcolour));
        bgColour = Colour::fromString (CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::backgroundcolour));
        xValueLabel.setColour (Label::textColourId, fontColour);
        yValueLabel.setColour (Label::textColourId, fontColour);
        ball.setColour (ballColour);
        ball.repaint();
        repaint();
    }
    else
    {
        //need to add a flag to xypad to disable dragging if users want to set values manually
        const float xPos = CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::valuex);
        const float yPos = CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::valuey);
        //setValues(xPos, maxY - yPos);
        juce::Point<float> pos (getValueAsPosition (juce::Point<float> (xPos, maxY - yPos)));
        //pos.addXY(-ball.getWidth() / 2, -ball.getWidth() / 2);
        ball.setTopLeftPosition (constrainPosition (pos.getX(), pos.getY()));
        repaint();
    }
}

//==================================================================
void CabbageXYPad::paint (Graphics& g)
{
    //main background
    g.fillAll (Colour (30, 30, 30));
    g.setColour (colour);
    g.fillRoundedRectangle (0, 0, getWidth(), getHeight(), 5);

    float borderWidth = CabbageUtilities::getBorderWidth();
    g.setColour (CabbageUtilities::getBorderColour());
    g.drawRoundedRectangle (borderWidth / 2, borderWidth / 2, getWidth() - borderWidth, getHeight() - borderWidth,
                            5, borderWidth);

    //text label
    g.setColour (textColour);
    Font font = owner->customFont;
    
    g.setFont (font);
    float strWidth = font.getStringWidthFloat (getText());
    g.drawText (getText(), getWidth() - strWidth - 10, getHeight() - (font.getHeight() + 3),
                strWidth, font.getHeight(), 1, false);

    //xypad background
    g.setColour (bgColour);
    g.fillRoundedRectangle (xyPadRect, 5);

    ColourGradient vGradient = ColourGradient (Colours::transparentBlack, 0, 0,
                                               Colours::transparentBlack, 0, getHeight(), false);
    vGradient.addColour (0.5, CabbageUtilities::getComponentSkin());
    g.setGradientFill (vGradient);
    g.drawLine (xyPadRect.getWidth() / 2, 0, xyPadRect.getWidth() / 2, xyPadRect.getHeight(), 1);

    ColourGradient hGradient = ColourGradient (Colours::transparentBlack, 0, 0,
                                               Colours::transparentBlack, xyPadRect.getWidth(), 0, false);
    hGradient.addColour (0.5, CabbageUtilities::getComponentSkin());
    g.setGradientFill (hGradient);
    g.drawLine (0, xyPadRect.getHeight() / 2, xyPadRect.getWidth(), xyPadRect.getHeight() / 2, 1);

    if (rightMouseButtonDown)
    {
        g.setColour (fontColour);
        g.drawLine (mouseDownXY.getX(), mouseDownXY.getY(), currentMouseXY.getX() + ball.getWidth() / 2, currentMouseXY.getY() + ball.getWidth() / 2);
    }

    //funky lines that move with ball
    if (isAutomating == false)
    {
        ColourGradient vLineCg = ColourGradient (Colours::transparentBlack, 0, 0, Colours::transparentBlack,
                                                 0, getHeight(), false);
        vLineCg.addColour (jlimit (0.f, 1.f, ((ball.getY() + (ball.getWidth() / 2.f)) / xyPadRect.getHeight())), ballColour);
        g.setGradientFill (vLineCg);
        g.setOpacity (1);
        g.drawLine (ball.getX() + ball.getWidth() / 2, 0, ball.getX() + ball.getWidth() / 2, xyPadRect.getHeight());

        ColourGradient hLineCg = ColourGradient (Colours::transparentBlack, 0, 0, Colours::transparentBlack,
                                                 getWidth(), 0, false);

        hLineCg.addColour (jmax (0.f, jmin (1.f, ((ball.getX() + (ball.getWidth() / 2.f)) / xyPadRect.getHeight()))), ballColour);
        g.setGradientFill (hLineCg);
        g.setOpacity (1);
        g.drawLine (0, ball.getY() + ball.getWidth() / 2, xyPadRect.getWidth(), ball.getY() + ball.getWidth() / 2);
    }
}

void CabbageXYPad::resized()
{
    xyPadRect.setWidth (getWidth()*.98);
    xyPadRect.setHeight (getHeight()*.85);
    xyPadRect.setTop (getHeight()*.02);
    xyPadRect.setLeft (getWidth()*.02);
    xValueLabel.setBounds (0, xyPadRect.getHeight(), getWidth() / 2, 20);
    yValueLabel.setBounds (getWidth()*.5, xyPadRect.getHeight(), getWidth() / 2, 20);
}

//==================================================================
juce::Point<int> CabbageXYPad::constrainPosition (float x, float y)
{
    const int xPos = jlimit (xyPadRect.getX(), (xyPadRect.getWidth() + xyPadRect.getX()) - ball.getWidth(), x - ball.getWidth() / 2.f);
    const int yPos = jlimit (xyPadRect.getY(), (xyPadRect.getHeight() + xyPadRect.getY()) - ball.getHeight(), y - ball.getHeight() / 2.f);
    return juce::Point<int> (xPos, yPos);
}

juce::Point<float> CabbageXYPad::getPositionAsValue (juce::Point<float> position)
{
    const float xVal = jlimit (minX, maxX, jmap (position.getX(), xyPadRect.getX(), xyPadRect.getWidth() - ball.getWidth(), minX, maxX));
    const float yVal = jlimit (minY, maxY, jmap (position.getY(), xyPadRect.getY(), xyPadRect.getHeight() - ball.getHeight(), minY, maxY));
    setValues (xVal, yVal);

    return juce::Point<float> (xVal, yVal);
}

void CabbageXYPad::setPositionAsValue (juce::Point<float> position)
{
    const float xVal = jlimit (minX, maxX, jmap (position.getX(), xyPadRect.getX(), xyPadRect.getWidth() - ball.getWidth(), minX, maxX));
    const float yVal = jlimit (minY, maxY, jmap (position.getY(), xyPadRect.getY(), xyPadRect.getHeight() - ball.getHeight(), minY, maxY));
    setValues (xVal, yVal);
}

juce::Point<float> CabbageXYPad::getValueAsPosition (juce::Point<float> position)
{
    //setValues (position.getX(), maxY - position.getY());
    const float xPos = jmap (position.getX(), minX, maxX, xyPadRect.getX() + ball.getWidth() / 2, xyPadRect.getWidth() - ball.getWidth() / 2);
    const float yPos = jmap (position.getY(), minY, maxY, xyPadRect.getY() + ball.getWidth() / 2, xyPadRect.getHeight() - ball.getWidth() / 2);

    return juce::Point<float> (xPos, yPos);
}

void CabbageXYPad::setValues (float x, float y, bool notify)
{
    xAxis.setValue (x, sendNotification);
    yAxis.setValue (minY + (maxY - y), sendNotification);
    xValueLabel.setText (createValueText(x, 3, xPrefix, xPostfix), dontSendNotification);
    yValueLabel.setText (createValueText(minY + (maxY - y), 3, yPrefix, yPostfix), dontSendNotification);
}
//========================================================================
XYPadAutomator::XYPadAutomator (String name, CabbagePluginParameter* xParam, CabbagePluginParameter* yParam, AudioProcessor* _owner)
    : name (name), xParam (xParam), yParam (yParam), owner (_owner)
{}


void XYPadAutomator::timerCallback()
{
    xValue += xValueIncrement;
    position.setX (xValue);

    yValue += yValueIncrement;
    position.setY (yValue);

    // If a border is hit then the increment value should be reversed...
    if (xValue <= xMin)
    {
        xValue = xMin;
        xValueIncrement *= -1;
    }
    else if (xValue >= xMax)
    {
        xValue = xMax;
        xValueIncrement *= -1;
    }

    if (yValue <= yMin)
    {
        yValue = yMin;
        yValueIncrement *= -1;
    }
    else if (yValue >= yMax)
    {
        yValue = yMax;
        yValueIncrement *= -1;
    }

    if (isPluginEditorOpen && xParam!=nullptr && yParam!=nullptr) //only update GUI is editor is open
        sendSynchronousChangeMessage();
    else
    {
        xParam->setValue (xValue);
        yParam->setValue (yValue);
    }
}
