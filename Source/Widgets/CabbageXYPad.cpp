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

#include "CabbageXYPad.h"
#include "../Audio/Plugins/CabbagePluginEditor.h"
#include "../Audio/Plugins/CabbageAudioParameter.h"

CabbageXYPad::CabbageXYPad(ValueTree wData, CabbagePluginEditor* editor):
widgetData(wData),
owner(editor),
ball(),
minX(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::minx)), 
maxX(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::maxx)),
minY(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::miny)), 
maxY(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::maxy)),
valueX(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::valuex)),
valueY(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::valuey)),
yValueLabel(),
xValueLabel(),
fontColour(Colour::fromString(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::fontcolour)))
{
	setName(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::name));
	widgetData.addListener(this); 				//add listener to valueTree so it gets notified when a widget's property changes
	initialiseCommonAttributes(this, wData); 	//initialise common attributes such as bounds, name, rotation, etc..	

	const Point<float> pos(getValueAsPosition(Point<float>(valueX, valueY)));
	ball.setBounds(pos.getX(), pos.getY(), 20, 20);
	ball.setInterceptsMouseClicks(false, false);
	addAndMakeVisible(ball);

	xAxis.setRange(minX, maxX);
	xAxis.setName(getName()+"_x");
	yAxis.setRange(minY, maxY);
	yAxis.setName(getName()+"_y");
	
	xValueLabel.setColour(Label::textColourId, fontColour);
	xValueLabel.setJustificationType(Justification::centred);
	xValueLabel.setText(String(valueX, 3), dontSendNotification);
	yValueLabel.setColour(Label::textColourId, fontColour);
	yValueLabel.setText(String(valueY, 3), dontSendNotification);
	yValueLabel.setJustificationType(Justification::centred);
	xValueLabel.setFont(Font(12, 1));
	yValueLabel.setFont(Font(12, 1));
	addAndMakeVisible(xValueLabel);
	addAndMakeVisible(yValueLabel);
}

//==================================================================
void CabbageXYPad::mouseDown(const MouseEvent& e)
{
	owner->enableXYAutomator(getName(), false);
	ball.setTopLeftPosition(constrainPosition(e.getPosition().getX(), e.getPosition().getY()));
	mouseDownXY.setXY(ball.getPosition().getX()+ball.getWidth()*.5f, ball.getPosition().getY()+ball.getHeight()*.5f); 
	const Point<float> currentValues = getPositionAsValue(ball.getPosition().toFloat());
	isAutomating = false;
	repaint();
}
 
void CabbageXYPad::mouseDrag(const MouseEvent& e)
{
	ball.setTopLeftPosition(constrainPosition(mouseDownXY.getX()+e.getDistanceFromDragStartX(), mouseDownXY.getY()+e.getDistanceFromDragStartY()));
	const Point<float> currentValues = getPositionAsValue(ball.getPosition().toFloat());
	repaint();
	
	currentMouseXY = ball.getPosition().toFloat();
	
	if(e.mods.isRightButtonDown())
		rightMouseButtonDown = true;
}

void CabbageXYPad::mouseUp(const MouseEvent& e)
{
	if(e.mods.isRightButtonDown())
	{
		const float xDistance = mouseDownXY.getX()-e.getPosition().getX();
		const float yDistance = mouseDownXY.getY()-e.getPosition().getY();
		CabbageUtilities::debug("X", xDistance);
		CabbageUtilities::debug("Y", yDistance);	
		rightMouseButtonDown = false;
		
		const Point<float> valueStart(getPositionAsValue(mouseDownXY));
		const Point<float> valueEnd(getPositionAsValue(e.getPosition().toFloat()));
		
		dragLine = Line<float>(valueStart.getX(), valueStart.getY(), valueEnd.getX(), valueEnd.getY());
		owner->enableXYAutomator(getName(), true, dragLine);
		isAutomating = true;
	}
}

void CabbageXYPad::changeListenerCallback(ChangeBroadcaster* source)
{
	if(XYPadAutomator* xyAuto = dynamic_cast<XYPadAutomator*>(source))
	{
		Point<float> pos(getValueAsPosition(Point<float>(xyAuto->position.getX(), xyAuto->position.getY())));
		pos.addXY(-ball.getWidth()/2, -ball.getWidth()/2);
		ball.setBounds(pos.getX(), pos.getY(), 20, 20);
		
		if(xyAuto->repaintBackground==true)
		{
			repaint();
			xyAuto->repaintBackground = false;
		}
	}
}
//==================================================================
void CabbageXYPad::valueTreePropertyChanged (ValueTree& valueTree, const Identifier& prop)
{
	if(prop != CabbageIdentifierIds::value)
		handleCommonUpdates(this, valueTree);		//handle comon updates such as bounds, alpha, rotation, visible, etc	
}

//==================================================================
void CabbageXYPad::paint(Graphics& g)
{
	g.fillAll(Colour(30, 30, 30));
    g.setColour (CabbageUtilities::getComponentSkin());
    g.fillRoundedRectangle (0, 0, getWidth(), getHeight(), 5);

    float borderWidth = CabbageUtilities::getBorderWidth();
    g.setColour (CabbageUtilities::getBorderColour());
    g.drawRoundedRectangle (borderWidth/2, borderWidth/2, getWidth()-borderWidth, getHeight()-borderWidth,
                            5, borderWidth);

    g.setColour(fontColour);
    Font font = CabbageUtilities::getComponentFont();
    g.setFont (font);
    float strWidth = font.getStringWidthFloat (_text);
    g.drawText (_text, getWidth()-strWidth-10, getHeight()-(font.getHeight()+3),
                strWidth, font.getHeight(), 1, false);	

	//xypad background 
    g.setColour (CabbageUtilities::getBackgroundSkin());
    g.fillRoundedRectangle(xyPadRect, 5);

    ColourGradient vGradient = ColourGradient(Colours::transparentBlack, 0, 0,
                               Colours::transparentBlack, 0, getHeight(), false);
    vGradient.addColour(0.5, CabbageUtilities::getComponentSkin());
    g.setGradientFill(vGradient);
    g.drawLine(xyPadRect.getWidth()/2, 0, xyPadRect.getWidth()/2, xyPadRect.getHeight(), 1);

    ColourGradient hGradient = ColourGradient(Colours::transparentBlack, 0, 0,
                               Colours::transparentBlack, xyPadRect.getWidth(), 0, false);
    hGradient.addColour(0.5, CabbageUtilities::getComponentSkin());
    g.setGradientFill(hGradient);
    g.drawLine(0, xyPadRect.getHeight()/2, xyPadRect.getWidth(), xyPadRect.getHeight()/2, 1);
	
	if(rightMouseButtonDown)
	{
		g.setColour(fontColour);
		g.drawLine(mouseDownXY.getX(), mouseDownXY.getY(), currentMouseXY.getX()+ball.getWidth()/2, currentMouseXY.getY()+ball.getWidth()/2);
	}
	
	//funky lines that move with ball
	if(isAutomating == false)
	{
		ColourGradient vLineCg = ColourGradient(Colours::transparentBlack, 0, 0, Colours::transparentBlack,
												0, getHeight(), false);
		vLineCg.addColour(jlimit(0.f, 1.f, ((ball.getY()+(ball.getWidth()/2.f)) / xyPadRect.getHeight())), Colours::red);
		g.setGradientFill(vLineCg);
		g.setOpacity(1);
		g.drawLine(ball.getX()+ball.getWidth()/2, 0, ball.getX()+ball.getWidth()/2, xyPadRect.getHeight());

		ColourGradient hLineCg = ColourGradient(Colours::transparentBlack, 0, 0, Colours::transparentBlack,
												getWidth(), 0, false);
		
		hLineCg.addColour(jmax (0.f, jmin (1.f, ((ball.getX()+(ball.getWidth()/2.f)) / xyPadRect.getHeight()))), Colours::red);
		g.setGradientFill(hLineCg);
		g.setOpacity(1);
		g.drawLine(0, ball.getY()+ball.getWidth()/2, xyPadRect.getWidth(), ball.getY()+ball.getWidth()/2);
	}
}

void CabbageXYPad::resized()
{
	xyPadRect.setWidth(getWidth()*.98);
	xyPadRect.setHeight(getHeight()*.85);
	xyPadRect.setTop(getHeight()*.02);
	xyPadRect.setLeft(getWidth()*.02);
	xValueLabel.setBounds(0, xyPadRect.getHeight(), getWidth()/2, 20);
	yValueLabel.setBounds(getWidth()*.5, xyPadRect.getHeight(), getWidth()/2, 20);
}

//==================================================================
Point<int> CabbageXYPad::constrainPosition(float x, float y)
{
	const int xPos = jlimit(xyPadRect.getX(), (xyPadRect.getWidth()+xyPadRect.getX())-ball.getWidth(), x-ball.getWidth()/2.f);
	const int yPos = jlimit(xyPadRect.getY(), (xyPadRect.getHeight()+xyPadRect.getY())-ball.getHeight(), y-ball.getHeight()/2.f);								
	return Point<int>(xPos, yPos);
}

Point<float> CabbageXYPad::getPositionAsValue(Point<float> position)
{
	const float xVal = jlimit(minX, maxX, jmap(position.getX(), xyPadRect.getX(), xyPadRect.getWidth()-ball.getWidth(), minX, maxX));
	const float yVal = jlimit(minY, maxY, jmap(position.getY(), xyPadRect.getY(), xyPadRect.getHeight()-ball.getHeight(), minY, maxY));
	setValues(xVal, yVal);
	
	return Point<float>(xVal, yVal);
}

Point<float> CabbageXYPad::getValueAsPosition(Point<float> position)
{
	setValues(position.getX(), position.getY());
	const float xPos = jmap(position.getX(), minX, maxX, xyPadRect.getX()+ball.getWidth()/2, xyPadRect.getWidth()-ball.getWidth()/2);
	const float yPos = jmap(position.getY(), minY, maxY, xyPadRect.getY()+ball.getWidth()/2, xyPadRect.getHeight()-ball.getWidth()/2);
	
	return Point<float>(xPos, yPos);
}

void CabbageXYPad::setValues(float x, float y)
{
	xAxis.setValue(x, sendNotification);
	yAxis.setValue(y, sendNotification);	
	xValueLabel.setText(String(x, 3), dontSendNotification);
	yValueLabel.setText(String(y, 3), dontSendNotification);
}
//========================================================================
void XYPadAutomator::timerCallback()
{
	xValue += xValueIncrement;
	position.setX(xValue);
	
	yValue += yValueIncrement;
	position.setY(yValue);
	
	// If a border is hit then the increment value should be reversed...
	if (xValue <= xMin)
	{
		xValue = xMin;
		xValueIncrement*=-1;
	}
	else if (xValue >= xMax)
	{
		xValue = xMax;
		xValueIncrement*=-1;
	}
	if (yValue <= yMin)
	{
		yValue = yMin;
		yValueIncrement*=-1;
	}
	else if (yValue >= yMax)
	{
		yValue = yMax;
		yValueIncrement*=-1;
	}	
	
	if(isPluginEditorOpen)	//only update GUI is editor is open
		sendChangeMessage();
	else
	{
		xParam->setValue(xValue);
		yParam->setValue(yValue);
	}
}