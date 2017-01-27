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

#include "CabbageSignalDisplay.h"

#include "../Audio/Plugins/CabbagePluginEditor.h"

CabbageSignalDisplay::CabbageSignalDisplay(ValueTree wData, CabbagePluginEditor* owner)
    : widgetData(wData),
      owner(owner),
      colour(Colour::fromString(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::colour))),
      backgroundColour(Colour::fromString(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::backgroundcolour))),
      fontColour(Colour::fromString(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::fontcolour))),
      minFFTBin(0),
      maxFFTBin(1024),
      vectorSize(512),
      shouldDrawSonogram(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::displaytype)=="spectrogram" ? true : false),
      displayType(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::displaytype)),
      spectrogramImage(Image::RGB, 512, 300, true),
      spectroscopeImage(Image::RGB, 512, 300, true),
      freqRangeDisplay(fontColour, backgroundColour),
      freqRange(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::min), CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::max)),
      scrollbar(false),
      scopeWidth(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::width)),
      zoomInButton("zoomIn", Colours::white),
      zoomOutButton("zoomOut", Colours::white),
      zoomLevel(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::zoom)),
      leftPos(0),
      isScrollbarShowing(false),
      scrollbarHeight(20),
      lineThickness(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::outlinethickness))
{
    setName(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::name));
    widgetData.addListener(this); 				//add listener to valueTree so it gets notified when a widget's property changes
    initialiseCommonAttributes(this, wData); 	//initialise common attributes such as bounds, name, rotation, etc..

    addAndMakeVisible(freqRangeDisplay);

    if(displayType=="waveform" || displayType=="lissajous")
        freqRangeDisplay.setVisible(false);

    addAndMakeVisible(scrollbar);
    scrollbar.setRangeLimits(Range<double>(0, 20));
    zoomInButton.addChangeListener(this);
    zoomOutButton.addChangeListener(this);
    //hide scrollbar, visible not working for disabling it...
    scrollbar.setBounds(-1000, getHeight()-15, getWidth(), 15);
    scrollbar.setAutoHide(false);
    scrollbar.addListener(this);
    if(zoomLevel>=0 && displayType!="lissajous")
    {
        addAndMakeVisible(zoomInButton);
        addAndMakeVisible(zoomOutButton);
    }

    startTimer(updateRate);
}

//====================================================================================
void CabbageSignalDisplay::setBins(int min, int max)
{
    minFFTBin = min;
    maxFFTBin = max;
}

//====================================================================================
void CabbageSignalDisplay::scrollBarMoved (ScrollBar* scrollBarThatHasMoved, double newRangeStart)
{
    ScrollBar* scroll = dynamic_cast<ScrollBar*>(scrollBarThatHasMoved);
    if(scroll)
    {
        float moveBy = newRangeStart/scrollBarThatHasMoved->getCurrentRange().getLength();
        moveBy = freqRangeDisplay.getWidth()*moveBy;
        freqRangeDisplay.setTopLeftPosition(-moveBy, 0);
        leftPos = -moveBy;
    }
}

//====================================================================================
void CabbageSignalDisplay::changeListenerCallback(ChangeBroadcaster *source)
{
    RoundButton* button = dynamic_cast<RoundButton*>(source);

    if(button->getName()=="zoomIn")
        zoomIn();
    else
        zoomOut();
}

//====================================================================================
void CabbageSignalDisplay::zoomIn(int factor)
{
    for(int i=0; i<factor; i++)
    {
        zoomLevel = zoomLevel<20 ? zoomLevel+1 : 20;
        const Range<double> newRange (0.0, 20-zoomLevel);
        scrollbar.setCurrentRange (newRange);
        freqRangeDisplay.setBounds(1, 0, getWidth()*(zoomLevel+1), 18);
        freqRangeDisplay.setResolution(10*zoomLevel+1);
        scopeWidth = freqRangeDisplay.getWidth();
        showScrollbar(true);
    }
}

//====================================================================================
void CabbageSignalDisplay::zoomOut(int factor)
{
    for(int i=0; i<factor; i++)
    {
        zoomLevel = zoomLevel>1 ? zoomLevel-1 : 0;
        const Range<double> newRange (0.0, 20-zoomLevel);
        scrollbar.setCurrentRange (newRange);
        freqRangeDisplay.setBounds(0, 0, getWidth()*jmax(1, zoomLevel+1), 18);
        freqRangeDisplay.setResolution(jmax(10, 10*zoomLevel+1));
        scopeWidth = freqRangeDisplay.getWidth();
        if(zoomLevel<1)
            showScrollbar(false);
    }
}

//====================================================================================
void CabbageSignalDisplay::showScrollbar(bool show)
{
    if(show)
    {
        scrollbar.setBounds(0, getHeight()-scrollbarHeight, getWidth(), scrollbarHeight);
        zoomInButton.setBounds(getWidth()-40, getHeight()-(scrollbarHeight*2+5), 20, 20);
        zoomOutButton.setBounds(getWidth()-20, getHeight()-(scrollbarHeight*2+5), 20, 20);
        isScrollbarShowing=true;
    }
    else
    {
        scrollbar.setBounds(-1000, getHeight()-scrollbarHeight, getWidth(), scrollbarHeight);
        zoomInButton.setBounds(getWidth()-40, getHeight()-scrollbarHeight-5, 20, 20);
        zoomOutButton.setBounds(getWidth()-20, getHeight()-scrollbarHeight-5, 20, 20);
        isScrollbarShowing=false;
    }
}

//====================================================================================
void CabbageSignalDisplay::drawSonogram()
{
    const int rightHandEdge = spectrogramImage.getWidth() - 2;
    const int imageHeight = spectrogramImage.getHeight();

    spectrogramImage.moveImageSection (0, 0, 1, 0, rightHandEdge, imageHeight);

    Graphics g(spectrogramImage);
    Range<float> maxLevel = FloatVectorOperations::findMinAndMax(signalFloatArray.getRawDataPointer(), signalFloatArray.size());

    for (int y = 0; y < imageHeight; y++)
    {
        const int index = jmap(y, 0, imageHeight, 0, vectorSize);
        const float level = jmap (signalFloatArray[index], 0.0f, maxLevel.getEnd(), 0.0f, 1.0f);
        g.setColour(Colour::fromHSV (level, 1.0f, level, 1.0f));
        g.drawHorizontalLine(imageHeight-y, rightHandEdge, rightHandEdge+2);
    }
}

//====================================================================================
void CabbageSignalDisplay::drawSpectroscope(Graphics& g)
{
    g.fillAll(backgroundColour);
    for (int i=0; i<vectorSize; i++)
    {
        const int position = jmap(i, 0, vectorSize, leftPos, scopeWidth);
        const int offset = isScrollbarShowing==true ? scrollbarHeight : 0;
        const int height = getHeight()-offset;
        const int amp = (signalFloatArray[i]*3*height);
        g.setColour(colour);
        g.drawVerticalLine(position, height-amp, height);
    }
}

//====================================================================================
void CabbageSignalDisplay::drawWaveform(Graphics& g)
{
    g.fillAll(backgroundColour);
    const int offset = isScrollbarShowing==true ? scrollbarHeight : 0;
    const int height = getHeight()-offset;
    int prevXPos = 0;
    int prevYPos = jmap(signalFloatArray[0], -1.f, 1.f, 0.f, 1.f)*height;

    for (int i=0; i<vectorSize; i++)
    {
        const int position = jmap(i, 0, vectorSize, leftPos, scopeWidth);
        const int amp = jmap(signalFloatArray[i], -1.f, 1.f, 0.f, 1.f)*height;
        g.setColour(colour);
        g.drawLine(prevXPos, prevYPos, position, amp, lineThickness);
        prevXPos = position;
        prevYPos = amp;
    }
}

//====================================================================================
void CabbageSignalDisplay::drawLissajous(Graphics& g)
{
    g.fillAll(backgroundColour);
    const int offset = isScrollbarShowing==true ? scrollbarHeight : 0;
    const int height = getHeight()-offset;
    int prevXPos = jmap(signalFloatArray[0], -1.f, 1.f, (float)leftPos, (float)scopeWidth);;
    int prevYPos = jmap(signalFloatArray2[1], -1.f, 1.f, 0.f, 1.f)*height;

    for (int i=0; i<vectorSize; i++)
    {
        const int position = jmap(signalFloatArray[i], -1.f, 1.f, (float)leftPos, (float)scopeWidth);
        const int amp = jmap(signalFloatArray2[i], -1.f, 1.f, 0.f, 1.f)*height;
        g.setColour(colour);
        g.drawLine(prevXPos, prevYPos, position, amp, lineThickness);
        prevXPos = position;
        prevYPos = amp;
    }
}

//====================================================================================
void CabbageSignalDisplay:: paint(Graphics& g)
{
    if(shouldPaint)
    {
        if(shouldDrawSonogram)
            g.drawImageWithin(spectrogramImage, 0, 0, getWidth(), getHeight(), RectanglePlacement::stretchToFit);
        else if(displayType=="spectroscope")
            drawSpectroscope(g);
        else if(displayType=="waveform")
            drawWaveform(g);
        else if(displayType=="lissajous")
            drawLissajous(g);
    }

    shouldPaint = false;
}

//====================================================================================
void CabbageSignalDisplay::mouseMove(const MouseEvent &e)
{
    if(shouldDrawSonogram)
    {
        const int position = jmap(e.getPosition().getY(), 0, getHeight(), 22050, 0);
        showPopup(String(position)+"Hz.");
    }
    else
    {
        const int position = jmap(e.getPosition().getX(), 0, scopeWidth, 0, 22050);
        showPopup(String(position)+"Hz.");
    }
}

//====================================================================================
void CabbageSignalDisplay::setSignalFloatArray(Array<float, CriticalSection> _points)
{
    signalFloatArray.swapWith(_points);
    if(displayType=="lissajous" || displayType=="waveform")
        vectorSize = signalFloatArray.size()/2;
    else
        vectorSize = signalFloatArray.size();

    if(vectorSize>0)
    {
        //spectrogram works on a scrolling image, the other displays are drawn directly
        if(displayType=="spectrogram")
            drawSonogram();

        shouldPaint = true;
    }
}

//====================================================================================
void CabbageSignalDisplay::setSignalFloatArraysForLissajous(Array<float, CriticalSection> _points1, Array<float, CriticalSection> _points2)
{
    signalFloatArray.swapWith(_points1);
    signalFloatArray2.swapWith(_points2);
    vectorSize = signalFloatArray.size();

    if(vectorSize>0)
    {
        shouldPaint = true;
    }
}

//====================================================================================
void CabbageSignalDisplay::timerCallback()
{
    if(owner->shouldUpdateSignalDisplay())
    {
        const String variable = CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::signalvariable);
        const String displayType = CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::displaytype);

        if(displayType!="lissajous")
        {
            setSignalFloatArray(owner->getArrayForSignalDisplay(variable, displayType));
        }
        else
        {
            var signalVariables = CabbageWidgetData::getProperty(widgetData, CabbageIdentifierIds::signalvariable);
            if(signalVariables.size()==2)
                setSignalFloatArraysForLissajous(owner->getArrayForSignalDisplay(signalVariables[0], displayType),
                                                 owner->getArrayForSignalDisplay(signalVariables[1], displayType));

        }

        repaint();
    }

}

//====================================================================================
void CabbageSignalDisplay::showPopup(String text)
{

}

//====================================================================================
void CabbageSignalDisplay::resized()
{
    scrollbarHeight = jmin(15.0, getHeight()*.09);
    if(!shouldDrawSonogram)
    {
        freqRangeDisplay.setBounds(1, 0, getWidth(), 18);
        const int offset = isScrollbarShowing==true ? 41 : 22;
        zoomInButton.setBounds(getWidth()-40, getHeight()-offset, 20, 20);
        zoomOutButton.setBounds(getWidth()-20, getHeight()-offset, 20, 20);
    }

    zoomIn(zoomLevel);
}

//====================================================================================
void CabbageSignalDisplay::valueTreePropertyChanged (ValueTree& valueTree, const Identifier& prop)
{
    if(CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::displaytype)!=displayType)
    {
        displayType = CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::displaytype);

        shouldDrawSonogram = displayType=="spectrogram" ? true : false;
        if(shouldDrawSonogram)
        {
            freqRangeDisplay.setVisible(false);
            zoomInButton.setVisible(false);
            zoomOutButton.setVisible(false);
            scrollbar.setVisible(false);
        }
        else if(displayType=="spectroscope")
        {
            freqRangeDisplay.setVisible(true);
            zoomInButton.setVisible(true);
            zoomOutButton.setVisible(true);
        }
        else if(displayType=="waveform")
        {
            freqRangeDisplay.setVisible(false);
            zoomInButton.setVisible(true);
            zoomOutButton.setVisible(true);
        }
        else if(displayType=="lissajous")
        {
            freqRangeDisplay.setVisible(false);
            zoomInButton.setVisible(false);
            zoomOutButton.setVisible(false);
        }
    }

    if(freqRange!=Range<int>(CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::min),
                             CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::max)))
    {
        freqRange = Range<int>(CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::min),
                               CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::max));

        freqRangeDisplay.setMinMax(freqRange.getStart(), freqRange.getEnd());
    }

    if(signalVariables!=CabbageWidgetData::getProperty(valueTree, CabbageIdentifierIds::signalvariable))
    {
        signalVariables = CabbageWidgetData::getProperty(valueTree, CabbageIdentifierIds::signalvariable);
    }

    if(updateRate!= CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::updaterate))
    {
        updateRate = CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::updaterate);
        startTimer(updateRate);
    }

    handleCommonUpdates(this, valueTree);		//handle comon updates such as bounds, alpha, rotation, visible, etc
}