/*
  Copyright (C) 2012 Damien Rennick

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

#include "XYPad.h"

XYHandleComponent::XYHandleComponent()
{
}

XYHandleComponent::~XYHandleComponent()
{
}

XYCanvas* XYHandleComponent::getParentComponent()
{
    return (XYCanvas*)Component::getParentComponent();
}

void XYHandleComponent::paint (Graphics& g)
{
    g.setColour (Colours::cornflowerblue);
    g.setOpacity(0.5);
    g.fillEllipse (getWidth()*0.25, getHeight()*0.25, getWidth()*0.5, getHeight()*0.5);
    g.setColour (Colours::aqua.withMultipliedSaturation(0.5));
    g.fillEllipse (getWidth()*0.4, getHeight()*0.4, getWidth()*0.2, getHeight()*0.2);
}

void XYHandleComponent::mouseEnter (const MouseEvent& e)
{
    setMouseCursor (MouseCursor::DraggingHandCursor);
}

void XYHandleComponent::mouseDown (const MouseEvent& e)
{
    setMouseCursor (MouseCursor::DraggingHandCursor);
    dragger.startDraggingComponent (this, e);
}

void XYHandleComponent::mouseDrag (const MouseEvent& e)
{
    //checking constraints so that the handle isn't dragged outside of its parent
    ScopedPointer<ComponentBoundsConstrainer> constrainer;
    constrainer = new ComponentBoundsConstrainer();
    constrainer->setMinimumOnscreenAmounts (0xffffff, 0xffffff,
                                            0xffffff, 0xffffff);

    dragger.dragComponent (this, e, constrainer);
    getParentComponent()->updatePath();
    getParentComponent()->repaint();
}

/*
  ============================================================================

   XYToggle

  ============================================================================
*/
XYToggle::XYToggle(int imageType, Colour col)
    : imageType(imageType), colourWhenOn(col)
{
}

XYToggle::~XYToggle()
{
}

void XYToggle::resized()
{
    img = XYImages::getImage(imageType, getWidth(), getHeight());
}

void XYToggle::paintButton (Graphics& g, bool isMouseOverButton, bool isButtonDown)
{
    g.setColour(CabbageUtilities::getDarkerBackgroundSkin());
    g.fillRoundedRectangle(0, 0, getWidth(), getHeight(), getHeight()/5);

    if (this->getToggleState() == true)
        g.setColour(colourWhenOn);
    else
    {
        if (isMouseOverButton)
            g.setColour(CabbageUtilities::getComponentSkin().withMultipliedBrightness(2));
        else
            g.setColour(CabbageUtilities::getComponentSkin());
    }
    g.drawImage(img, 0, 0, getWidth(), getHeight(), 0, 0, img.getWidth(), img.getHeight(), true);
    g.setOpacity(0.7);
    g.drawRoundedRectangle(0.5, 0.5, getWidth()-1, getHeight()-1, getHeight()/5, 1);

}

/*
  ============================================================================

   XYValueDisplay Component

  ============================================================================
*/
XYValueDisplay::XYValueDisplay(Colour col)
    : colour(col)
{
}

XYValueDisplay::~XYValueDisplay()
{
}

void XYValueDisplay::setValue (String val)
{
    value = val;
    repaint();
}

void XYValueDisplay::paint (Graphics& g)
{
    Font font = (CabbageUtilities::getValueFont());
    g.setFont (font);
    g.setColour (colour);
    g.setOpacity(0.6);
    value = CabbageUtilities::cabbageString(value, font, getWidth());
    g.drawText (value, 0, 0, getWidth(), getHeight(), Justification::centred, false);
}

/*
  ============================================================================

   XYCanvasBackground

  ============================================================================
*/
XYCanvasBackground::XYCanvasBackground()
{
}

XYCanvasBackground::~XYCanvasBackground()
{
}

void XYCanvasBackground::resized()
{
}

void XYCanvasBackground::paint(Graphics& g)
{
    g.setColour (CabbageUtilities::getBackgroundSkin());
    g.fillRoundedRectangle (0, 0, getWidth(), getHeight(), 5);

    // Grid lines
    ColourGradient vGradient = ColourGradient(Colours::transparentBlack, 0, 0,
                               Colours::transparentBlack, 0, getHeight(), false);
    vGradient.addColour(0.5, CabbageUtilities::getComponentSkin());
    g.setGradientFill(vGradient);
    g.drawLine(getWidth()/2, 0, getWidth()/2, getHeight(), 1);

    ColourGradient hGradient = ColourGradient(Colours::transparentBlack, 0, 0,
                               Colours::transparentBlack, getWidth(), 0, false);
    hGradient.addColour(0.5, CabbageUtilities::getComponentSkin());
    g.setGradientFill(hGradient);
    g.drawLine(0, getHeight()/2, getWidth(), getHeight()/2, 1);
}



/*
  ============================================================================

   XYCanvas

  ============================================================================
*/
XYCanvas::XYCanvas(Colour ballCol, float xMinimum, float xMaximum, float yMinimum, float yMaximum)
    : ballColour(ballCol),
      xMin(xMinimum),
      xMax(xMaximum),
      yMin(yMinimum),
      yMax(yMaximum)
{
    paintStaticBall = true;

    xRange = xMax-xMin;
    yRange = yMax-yMin;
}

XYCanvas::~XYCanvas()
{
}

void XYCanvas::resized()
{
    cacheStaticBallImage();
    cacheMovingBallImage();
}

XYPad* XYCanvas::getParentComponent()
{
    return (XYPad*)Component::getParentComponent();
}

void XYCanvas::cacheStaticBallImage()
{
    Image img = Image(Image::ARGB, ballSize*1.12, ballSize*1.12, true);
    Graphics g(img);

    // Shadow
    g.setColour(Colour::fromFloatRGBA(0, 0, 0, 80));
    g.fillEllipse(ballSize*0.12, ballSize*0.12, ballSize, ballSize);

    g.setColour(ballColour);
    g.fillEllipse (0, 0, img.getWidth(), img.getHeight());

    ImageCache::addImageToCache(img, 28);
    staticBallImage = ImageCache::getFromHashCode(28);
}

void XYCanvas::cacheMovingBallImage()
{
    Image img = Image(Image::ARGB, ballSize*1.12, ballSize*1.12, true);
    Graphics g(img);

    // Shadow
    g.setColour(Colour::fromFloatRGBA(0, 0, 0, 80));
    g.fillEllipse(ballSize*0.12, ballSize*0.12, ballSize, ballSize);

    g.setColour(ballColour);
    g.setOpacity(0.5);
    g.fillEllipse ((ballSize*0.3), (ballSize*0.3),
                   ballSize*0.4, ballSize*0.4);
    g.setColour (ballColour.withMultipliedSaturation(5));
    g.drawEllipse (ballSize*0.05, ballSize*0.05,
                   ballSize*0.9, ballSize*0.9, ballSize*0.1);

    ImageCache::addImageToCache(img, 29);
    movingBallImage = ImageCache::getFromHashCode(29);
}

void XYCanvas::paint(Graphics& g)
{
    // Ball path
    if ((path.getLength() > 0) && (pathOpacity > 0))
    {
        if (toggleId == 0)
        {
            ColourGradient cg = ColourGradient (Colours::transparentBlack, path.getPointAlongPath(0).getX(),
                                                path.getPointAlongPath(0).getY(),
                                                ballColour.withMultipliedSaturation(5), path.getPointAlongPath(path.getLength()).getX(),
                                                path.getPointAlongPath(path.getLength()).getY(), false);
            g.setGradientFill(cg);
        }
        else
            g.setColour(ballColour.withMultipliedSaturation(5));

        g.setOpacity (pathOpacity);
        g.strokePath (path, PathStrokeType(pathThickness));
    }

    // Ball h and v lines
    if (ballLineOpacity > 0)
    {
        ColourGradient vLineCg = ColourGradient(Colours::transparentBlack, 0, 0, Colours::transparentBlack,
                                                0, getHeight(), false);
        vLineCg.addColour(jlimit(0.f, 1.f, ((ballY+(ballSize/2)) / getHeight())), ballColour);
        //vLineCg.addColour(jmax (0.f, jmin (1.f, ((ballY+(ballSize/2)) / getHeight()))), ballColour);
        g.setGradientFill(vLineCg);
        g.setOpacity(1);
        g.drawLine(ballX+ballSize/2, 0, ballX+ballSize/2, getHeight(), ballLineOpacity);

        ColourGradient hLineCg = ColourGradient(Colours::transparentBlack, 0, 0, Colours::transparentBlack,
                                                getWidth(), 0, false);
        //hLineCg.addColour((ballX+(ballSize/2)) / getWidth(), ballColour);
        hLineCg.addColour(jmax (0.f, jmin (1.f, ((ballX+(ballSize/2)) / getHeight()))), ballColour);
        g.setGradientFill(hLineCg);
        g.setOpacity(1);
        g.drawLine(0, ballY+ballSize/2, getWidth(), ballY+ballSize/2, ballLineOpacity);

    }

    // Ball
    g.setOpacity(1);
    if (paintStaticBall)
        g.drawImage(staticBallImage, ballX, ballY, ballSize, ballSize, 0, 0, staticBallImage.getWidth(), staticBallImage.getHeight(), false);
    else
        g.drawImage(movingBallImage, ballX, ballY, ballSize, ballSize, 0, 0, movingBallImage.getWidth(), movingBallImage.getHeight(), false);
}

void XYCanvas::mouseEnter (const MouseEvent& e)
{
    setMouseCursor(MouseCursor::CrosshairCursor);
}

void XYCanvas::mouseDown (const MouseEvent& e)
{
    setMouseCursor(MouseCursor::NoCursor);
    ballLineOpacity = 0.5;
}

void XYCanvas::mouseDrag (const MouseEvent& e)
{
    setMouseCursor(MouseCursor::NoCursor);
}

void XYCanvas::mouseUp (const MouseEvent& e)
{
    setMouseCursor(MouseCursor::CrosshairCursor);
    startTimer(1, 150); //ball path
    startTimer(2, 250);	//ball cross hair lines
}

Point<float> XYCanvas::checkBounds(Point<float> pt)
{
    //Checks that the pt(x,y) is inside the available bounds
    if (pt.getX() < ballSize/2)
        pt.setX (ballSize/2);
    else if (pt.getX() > (getWidth()-(ballSize/2)))
        pt.setX (getWidth() - (ballSize/2));
    if (pt.getY() < ballSize/2)
        pt.setY (ballSize/2);
    else if (pt.getY() > (getHeight()-(ballSize/2)))
        pt.setY (getHeight() - (ballSize/2));

    return pt;
}

void XYCanvas::setBallPositionFromXYValues(float xValue, float yValue)
{
    //Sets the ball position from the x and y output values
    ballX = (((xValue-xMin)/xRange)*(getWidth()-ballSize));
    ballY = ((1-((yValue-yMin)/yRange))*(getHeight()-ballSize));
    repaint();
}

void XYCanvas::setBallAndHandleSize(float size)
{
    //making the overall handle component the same size as the ball.
    //This makes it easier for controlling bounds.
    ballSize = handleSize = size;
}

float XYCanvas::getBallX()
{
    return ballX;
}

float XYCanvas::getBallY()
{
    return ballY;
}

XYHandleComponent* XYCanvas::makeHandle(Point<float> pt)
{
    pt = checkBounds(pt);

    XYHandleComponent* handle = new XYHandleComponent();
    handle->setBounds (pt.getX()-(handleSize/2), pt.getY()-(handleSize/2),
                       handleSize, handleSize); //their centre will be where the mouse was clicked
    if (toggleId == 1)
        addAndMakeVisible (handle); //only visible if not in normal mode
    repaint();
    return handle;
}

void XYCanvas::setStartHandle (Point<float> pt)
{
    handles.set (0, makeHandle (pt)); //'set'replaces element
}

void XYCanvas::setEndHandle (Point<float> pt)
{
    handles.set (1, makeHandle (pt));
    pathOpacity = 1;
    pathThickness = 2;
    updatePath();
}

void XYCanvas::clearHandles()
{
    handles.clear(true);
    updatePath();
}

void XYCanvas::updatePath()
{
    path.clear();
    if (handles.size() > 1)   //should be at least 2 handles
    {
        path.startNewSubPath (handles[0]->getX()+(handleSize/2), handles[0]->getY()+(handleSize/2));
        path.lineTo (handles[1]->getX()+(handleSize/2), handles[1]->getY()+(handleSize/2));
        getParentComponent()->xyPadAutomation->setBallPath (path);
    }
    repaint();
}

void XYCanvas::setToggleId(int id)
{
    toggleId = id;
}

void XYCanvas::useStaticBall(bool useStaticBall)
{
    paintStaticBall = useStaticBall;
}

void XYCanvas::startBallPathTimer()
{
    // This is called by XYPad upon re-instantiation if type 2 automation is on.
    startTimer(1, 150);
}

/*
  ============================================================================

   XYPad

  ============================================================================
*/

XYPad::XYPad(XYPadAutomation* xyPadAutomationPtr, String title, int minXValue, int maxXValue,
             int minYValue,
             int maxYValue,
             int numberOfDecimalPlaces,
             Colour ballCol,
             Colour fontCol,
             Colour textCol,
             float initXVal,
             float initYVal)
    :
    title(title),
    decimalPlaces(numberOfDecimalPlaces),
    initX(initXVal),
    initY(initYVal)

{
    // checking if xyPadAutomation is a valid object
    if (xyPadAutomationPtr)
        xyPadAutomation = xyPadAutomationPtr;
    else
    {
        xyPadAutomation = new XYPadAutomation();

    }

    //every time "this" gets initialised it updates the counter in the automation class
    xyPadAutomation->updateCreationCounter();

    lookAndFeel = new CabbageLookAndFeel();
    Component::setLookAndFeel(lookAndFeel);


    Logger::writeToLog("-----xyPad Constructor-----");
    // Min and max values
    xMin = minXValue;
    xMax = maxXValue;
    yMin = minYValue;
    yMax = maxYValue;
    xRange = xMax - xMin;
    //Logger::writeToLog("xRange:"+String(xRange));
    yRange = yMax - yMin;
    //Logger::writeToLog("yRange:"+String(yRange));
    // Colours
    ballColour = ballCol;

    fontColour = textCol;
    textColour = fontCol;
    if (fontColour != CabbageUtilities::getComponentFontColour())
        toggleColour = fontColour;
    else
        toggleColour = Colours::cornflowerblue;

    // Canvas
    canvas = new XYCanvas(ballColour, xMin, xMax, yMin, yMax);
    canvasBackground = new XYCanvasBackground();


    // Value displays
    for (int i=0; i<2; i++)
    {
        valueDisplays.add (new XYValueDisplay(textColour));
        addAndMakeVisible (valueDisplays[i]);
    }

    // Speed slider
    speedSlider = new Slider("Speed");

    // Toggles
    xyToggles.add(new XYToggle(0, toggleColour));
    xyToggles.add(new XYToggle(1, toggleColour));
    for (int i=0; i<2; i++)
    {
        addAndMakeVisible(xyToggles[i]);
        xyToggles[i]->addListener(this);
    }

    title.append (name, 100);

    if (decimalPlaces < 0)
        decimalPlaces = 0;
    format << "%." << decimalPlaces << "f";

    //create basic look and feel
    lookAndFeelBasic = new CabbageLookAndFeelBasic();
    speedSlider->setColour(Slider::thumbColourId, Colours::red);
    speedSlider->setLookAndFeel(lookAndFeelBasic);

}

XYPad::~XYPad()
{
    Logger::writeToLog("XYPad destructor");
    xyToggles.clear();
}

void XYPad::resized()
{
    // Toggle bounds
    float toggleWidth;
    if (getWidth() >= 150)
        toggleWidth = 30;
    else
        toggleWidth = getWidth()/5;

    for (int i=0; i<2; i++)
        xyToggles[i]->setBounds(5+(i*(toggleWidth+5)), getHeight()-18, toggleWidth, 15);

    currentSelectedToggle = xyPadAutomation->getSelectedToggle();
    xyToggles[currentSelectedToggle]->setToggleState(true, dontSendNotification);

    // Ball
    ballSize = (float)getWidth()/10;
    if (ballSize > 18)
        ballSize = 18;		//maximum size
    else if (ballSize < 8)
        ballSize = 8;		//minimum size

    xyPadAutomation->setBallSize (ballSize);
    canvas->setBallAndHandleSize(ballSize);

    // Canvas
    canvasBackground->setBounds(5, 5, getWidth()-10, getHeight()-45);
    addAndMakeVisible(canvasBackground);
    canvas->setBounds(5, 5, getWidth()-10, getHeight()-45);
    addAndMakeVisible(canvas);
    canvas->addMouseListener(this, true); //so that "this" also gets the mouse events for canvas
    canvas->setToggleId(currentSelectedToggle);

    // Value displays
    valueDisplays[0]->setBounds(3, getHeight()-38, getWidth()*0.5-6, 12);
    valueDisplays[1]->setBounds(getWidth()*0.5+3, getHeight()-38, getWidth()*0.5-6, 12);

    // Speed slider
    speedSlider->setBounds(getWidth()*0.3, canvas->getBottom()-25,
                           getWidth()*0.4, 15);
    speedSlider->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    speedSlider->setSliderStyle(Slider::LinearHorizontal);
    speedSlider->setRange(0, 1, 0.01);
    speedSlider->setValue(xyPadAutomation->getSpeedSliderValue());
    addAndMakeVisible(speedSlider);
    speedSlider->addListener(this);
    speedSlider->addMouseListener(this, false);
    speedSlider->setAlpha(0.1);

    speedSlider->setLookAndFeel(lookAndFeelBasic); //setting look and feel for slider
    speedSlider->setColour(Slider::thumbColourId, Colours::white);
    //if the plugin is already automating from a previous instance then we need to reset certain things
    //such as handles and automation type.
    if (xyPadAutomation->isAutomating())
    {
        if (xyPadAutomation->getSelectedToggle() == 1)
        {
            canvas->setStartHandle(xyPadAutomation->getStartHandle());
            canvas->setEndHandle(xyPadAutomation->getEndHandle());
            canvas->startBallPathTimer(); //we only want to redraw the path for type 2 automation
        }
        canvas->useStaticBall(false);
    }
    else if (xyPadAutomation->getCreationCounter() == 1)   //else if this is the first time being initialised
    {
        xyPadAutomation->setBoundsForAutomation (canvas->getBounds());
        xyPadAutomation->setMinMaxValues (xMin, xMax, yMin, yMax);
    }

    //setting ball position from x and y output values, this means the pad will open with the current x and y values
    canvas->setBallPositionFromXYValues (initX, initY);
    displayXYValues(initX, initY);

}

void XYPad::buttonClicked(Button* button)
{
    if (button == xyToggles[0])
    {
        xyToggles[1]->setToggleState(false, dontSendNotification);
        currentSelectedToggle = 0;
    }
    else if (button == xyToggles[1])
    {
        xyToggles[0]->setToggleState(false, dontSendNotification);
        currentSelectedToggle = 1;
    }

    if (!xyPadAutomation->isAutomating()) //only change toggle id when not automating
        canvas->setToggleId(currentSelectedToggle);

}

void XYPad::sliderValueChanged(Slider* slider)
{
    if (slider == speedSlider)
        xyPadAutomation->setSpeedSliderValue(slider->getValue());
}


void XYPad::paint (Graphics& g)
{
    g.setColour (CabbageUtilities::getComponentSkin());
    g.fillRoundedRectangle (0, 0, getWidth(), getHeight(), 5);

    // Border outline
    float borderWidth = CabbageUtilities::getBorderWidth();
    g.setColour (CabbageUtilities::getBorderColour());
    g.drawRoundedRectangle (borderWidth/2, borderWidth/2, getWidth()-borderWidth, getHeight()-borderWidth,
                            5, borderWidth);

    // For drawing the title
    g.setColour(fontColour);
    Font font = CabbageUtilities::getComponentFont();
    g.setFont (font);
    title = CabbageUtilities::cabbageString (title, font, canvas->getRight()-(getWidth()/2));
    float strWidth = font.getStringWidthFloat (title);
    g.drawText (title, canvas->getRight()-strWidth, getHeight()-(font.getHeight()+3),
                strWidth, font.getHeight(), 1, false);
}

void XYPad::mouseDown (const MouseEvent& e)
{
    if (canvas->isMouseOver())
    {
        if (xyPadAutomation->isAutomating())
        {
            xyPadAutomation->cancelAutomation();
            canvas->setToggleId(currentSelectedToggle);
            canvas->clearHandles();
        }

        Point<float> click = canvas->checkBounds(Point<float>(e.x, e.y));
        if (e.mods.isRightButtonDown())
        {
            canvas->setStartHandle(click);
            canvas->useStaticBall(false);
        }
        else if (e.mods.isLeftButtonDown())
        {
            canvas->useStaticBall(true);
            speedSlider->setValue(0); //resetting slider
        }
        float xValue = getXValueWhenNotAutomating(click.getX());
        float yValue = getYValueWhenNotAutomating(click.getY());
        xyPadAutomation->setXValue(xValue);
        xyPadAutomation->setYValue(yValue);
        displayXYValues(xValue, yValue);
        canvasHasFocus = true;
    }
}

void XYPad::mouseDrag (const MouseEvent& e)
{
    if ((canvas->isMouseButtonDownAnywhere()) && (canvasHasFocus))
    {
        Point<float> click = canvas->checkBounds(Point<float>(e.x, e.y));
        float xValue = getXValueWhenNotAutomating(click.getX());
        float yValue = getYValueWhenNotAutomating(click.getY());
        xyPadAutomation->setXValue(xValue);
        xyPadAutomation->setYValue(yValue);
        displayXYValues(xValue, yValue);
        if (e.mods.isRightButtonDown())
            canvas->setEndHandle(click);
    }
}

void XYPad::mouseUp (const MouseEvent& e)
{
    canvasHasFocus = false;
    if (e.mods.isRightButtonDown())
    {
        xyPadAutomation->beginAutomation(currentSelectedToggle);
        speedSlider->setValue(xyPadAutomation->getSpeedSliderValue(), dontSendNotification); //not sending update message so that it won't call sliderValueChange()
    }
}

void XYPad::mouseEnter(const MouseEvent& e)
{
    if ((speedSlider->isMouseOver()) && (xyPadAutomation->isAutomating()))
    {
        speedSlider->setEnabled(true);
        speedSlider->setAlpha(1.0);
    }
    else
    {
        speedSlider->setEnabled(false);
        speedSlider->setAlpha(0.1);
    }
}

void XYPad::displayXYValues (float xValue, float yValue)
{
    valueDisplays[0]->setValue(CabbageUtilities::setDecimalPlaces(xValue, 2));
    valueDisplays[1]->setValue(CabbageUtilities::setDecimalPlaces(yValue, 2));
    //Logger::writeToLog("yValue:"+String(yValue));
    canvas->setBallPositionFromXYValues(xValue, yValue);
}

float XYPad::getXValueWhenNotAutomating(float x)
{
    //Returns the x output value, not the ball's x pixel position!!
    //Only used when automation is off.
    return (((x-ballSize/2) / (canvas->getWidth()-ballSize)) * xRange) + xMin;
}

float XYPad::getYValueWhenNotAutomating(float y)
{
    //Returns the y output value, not the ball's y pixel position!!
    //Only used when automation is off.
    return ((1-((y-ballSize/2)/(canvas->getHeight()-ballSize))) * yRange) + yMin;
}

//========= These method are used by the Plugin Editor ============================================
void XYPad::setXYValues (float x, float y)
{
    //This method uses the x and y output values passed in from the plugin processor and converts them back
    //into the centre coordinates of the ball.
    if (xyPadAutomation->isAutomating())
        displayXYValues(x, y);
}

void XYPad::setXYValuesFromNormalised (float xNorm, float yNorm)
{
    //This method uses normalised values passed in from the plugin processor and converts them back
    //into the centre coordinates of the ball.
    if (xyPadAutomation->isAutomating())
    {
        float x = (xNorm*xRange) + xMin;
        float y = (yNorm*yRange) + yMin;
        displayXYValues(x, y);
    }
}


