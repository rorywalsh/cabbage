
#include "XYPadAutomation.h"
#include "XYPad.h"

XYPadAutomation::XYPadAutomation()
{
    ballPathDirection = 1;
    timerInterval = 50;

    selectedToggle = 0;
    speedSliderValue = 0;
    isAutomationOn = false;
    updateCounter = 0;
    paramIndex = 0;
    creationCounter = 0;
}

XYPadAutomation::~XYPadAutomation()
{
}

void XYPadAutomation::updateCreationCounter()
{
    creationCounter++;
}

int XYPadAutomation::getCreationCounter()
{
    return creationCounter;
}

void XYPadAutomation::setBallSize (float size)
{
    ballSize = size;
}

void XYPadAutomation::setBoundsForAutomation(Rectangle<int> bounds)
{
    availableBounds.setWidth(bounds.getWidth()-ballSize);
    availableBounds.setHeight(bounds.getHeight()-ballSize);
}

void XYPadAutomation::setBallPath (Path path)
{
    AffineTransform transform = AffineTransform::translation((ballSize/2)*-1, (ballSize/2)*-1);
    path.applyTransform(transform);
    ballPath = path;
}

bool XYPadAutomation::isAutomating()
{
    return isAutomationOn;
}

void XYPadAutomation::setMinMaxValues (float xMinimum, float xMaximum, float yMinimum, float yMaximum)
{
    xMin = xMinimum;
    yMin = yMinimum;
    xMax = xMaximum;
    yMax = yMaximum;
    xRange = xMax - xMin;
    yRange = yMax - yMin;

    xValue = getMinimumXValue();
    yValue = getMinimumYValue();
}

float XYPadAutomation::getMinimumXValue()
{
    return xMin;
}

float XYPadAutomation::getMinimumYValue()
{
    return yMin;
}

void XYPadAutomation::cancelAutomation()
{
    stopTimer();
    isAutomationOn = false;
    xValueIncrement = yValueIncrement = 0;
}

void XYPadAutomation::setInitialSpeedSliderValue ()
{
    // Function that calculates the initial speed slider value
    float length = ballPath.getLength();
    // getting length of parent's diagonal using pythagoras's theorem
    int sumOfSquares = (availableBounds.getWidth()*availableBounds.getWidth()) +
                       (availableBounds.getHeight()*availableBounds.getHeight());
    int diagonal = pow(sumOfSquares, 0.5); //square root
    speedSliderValue = length / diagonal;
}


void XYPadAutomation::updateIncrements()
{
    xValueIncrement *= speedValue;
    yValueIncrement *= speedValue;
}

void XYPadAutomation::setSpeedSliderValue(float sliderValue)
{
    speedSliderValue = sliderValue;
    updateCounter = 0; //reset our counter
}

void XYPadAutomation::beginAutomation(int selectedToggleButton)
{
    selectedToggle = selectedToggleButton;
    currentPointAlongPath = ballPath.getLength(); //ball begins traversing at the end of the path, rather than the start

    setInitialSpeedSliderValue ();
    Point<float> startDragPos = ballPath.getPointAlongPath(0);
    Point<float> secondPoint = ballPath.getPointAlongPath(1);
    Point<float> endDragPos = ballPath.getPointAlongPath(ballPath.getLength());

    if (selectedToggle == 0)
    {
        xValueIncrement = ((secondPoint.getX()-startDragPos.getX()) / (float)availableBounds.getWidth()) * xRange;
        yValueIncrement = ((secondPoint.getY()-startDragPos.getY()) / (float)availableBounds.getHeight()) * yRange;
        yValueIncrement *= -1; //inverting because y axis is also inverted
    }

    //initialising
    xValue = ((endDragPos.getX()/(float)availableBounds.getWidth()) * xRange) + xMin;
    yValue = ((1 - (endDragPos.getY()/(float)availableBounds.getHeight())) * yRange) + yMin; //inverting y axis
    startTimer (timerInterval);
    isAutomationOn = true;
}

void XYPadAutomation::update()
{
    if (isAutomating())
    {
        updateCounter = 0;
        if (selectedToggle == 0)   //first automation type
        {
            xValue += xValueIncrement*(10*speedSliderValue);
            yValue += yValueIncrement*(10*speedSliderValue);

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
        }
        else if (selectedToggle == 1)    //2nd automation type
        {
            Point<float> pt = ballPath.getPointAlongPath(currentPointAlongPath, AffineTransform::identity);
            xValue = (((pt.getX()/(float)availableBounds.getWidth()) * xRange) + xMin);
            yValue = ((pt.getY()/(float)availableBounds.getHeight()) * yRange);
            yValue = ((yRange-yValue) + yMin); //inverting and adding yMin
            currentPointAlongPath += ballPathDirection*(10*speedSliderValue);
            if (currentPointAlongPath > ballPath.getLength())
            {
                currentPointAlongPath = ballPath.getLength();
                ballPathDirection *= -1;
            }
            else if (currentPointAlongPath < 0)
            {
                currentPointAlongPath = 0;
                ballPathDirection *= -1;
            }
        }
    }
}

void XYPadAutomation::timerCallback()
{
    sendChangeMessage();
}

Point<float> XYPadAutomation::getStartHandle()
{
    Point<float> startHandle = ballPath.getPointAlongPath(0);
    startHandle.setX(startHandle.getX() + (ballSize/2));
    startHandle.setY(startHandle.getY() + (ballSize/2));

    return startHandle;
}

Point<float> XYPadAutomation::getEndHandle()
{
    Point<float> endHandle = ballPath.getPointAlongPath(ballPath.getLength());
    endHandle.setX(endHandle.getX() + (ballSize/2));
    endHandle.setY(endHandle.getY() + (ballSize/2));

    return endHandle;
}

float XYPadAutomation::getXValue()
{
    return xValue;
}

float XYPadAutomation::getYValue()
{
    return yValue;
}

float XYPadAutomation::getNormalisedXValue()
{
    return (xValue-xMin)/xRange;
}

float XYPadAutomation::getNormalisedYValue()
{
    return (yValue-yMin)/yRange;
}

void XYPadAutomation::setXValue(float value)
{
    xValue = value;
    sendChangeMessage();
}

void XYPadAutomation::setYValue(float value)
{
    yValue = value;
    sendChangeMessage();
}

int XYPadAutomation::getSelectedToggle()
{
    return selectedToggle;
}

float XYPadAutomation::getSpeedSliderValue()
{
    return speedSliderValue;
}
