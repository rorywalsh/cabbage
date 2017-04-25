/*
  Copyright (C) 2009 Rory Walsh

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

#include "Soundfiler.h"

//==============================================================================
// zooming button
//==============================================================================
class ZoomButton : public Component,
    public ChangeBroadcaster

{
public:
    ZoomButton (String type): Component()
    {
        setName (type);
    }
    ~ZoomButton() {}

    void mouseDown (const MouseEvent& e)
    {
        sendChangeMessage();
    }

    void paint (Graphics& g)
    {
        g.fillAll (Colours::transparentBlack);
        g.setColour (Colours::white.withAlpha (.8f));
        g.fillEllipse (0, 0, getWidth(), getHeight());
        g.setColour (Colours::black);
        g.fillRoundedRectangle (getWidth()*.18, getHeight()*.4f, getWidth()*.65, getHeight()*.25, 2);

        if (getName() == "zoomIn")
            g.fillRoundedRectangle (getWidth()*.38f, getHeight()*.20, getWidth()*.25, getHeight()*.65, 2);
    }

};
//==============================================================================
// soundfiler display  component
//==============================================================================

Soundfiler::Soundfiler (int sr, Colour col, Colour bgcol):   thumbnailCache (5), colour (col),                                                        sampleRate (sr),
    currentPlayPosition (0),
    mouseDownX (0),
    mouseUpX (0),
    drawWaveform (false),
    regionWidth (1),
    loopLength (0),
    scrubberPosition (0),
    showScrubber (true),
    selectableRange (true),
    bgColour (bgcol),
    currentPositionMarker (new DrawableRectangle())
{
    formatManager.registerBasicFormats();
    thumbnail = new AudioThumbnail (2, formatManager, thumbnailCache);
    thumbnail->addChangeListener (this);
    //setSize(400, 200);
    sampleRate = sr;
    addAndMakeVisible (scrollbar = new ScrollBar (false));
    scrollbar->setRangeLimits (visibleRange);
    //scrollbar->setAutoHide (false);
    scrollbar->addListener (this);
    currentPositionMarker->setFill (Colours::white.withAlpha (0.85f));
    addAndMakeVisible (currentPositionMarker);
    addAndMakeVisible (zoomIn = new ZoomButton ("zoomIn"));
    addAndMakeVisible (zoomOut = new ZoomButton ("zoomOut"));
    zoomIn->addChangeListener (this);
    zoomOut->addChangeListener (this);
}
//==============================================================================
Soundfiler::~Soundfiler()
{
    scrollbar->removeListener (this);
    thumbnail->removeChangeListener (this);
}
//==============================================================================
void Soundfiler::changeListenerCallback (ChangeBroadcaster* source)
{
    ZoomButton* button = dynamic_cast<ZoomButton*> (source);

    if (button)
    {
        if (button->getName() == "zoomIn")
            setZoomFactor (jmin (1.0, zoom += 0.1));
        else
            setZoomFactor (jmax (0.0, zoom -= 0.1));
    }

    repaint();
}
//==============================================================================
void Soundfiler::resized()
{
    zoomIn->setBounds (getWidth() - 43, getHeight() - 40, 20, 20);
    zoomOut->setBounds (getWidth() - 20, getHeight() - 40, 20, 20);

    if (scrollbar)
        scrollbar->setBounds (getLocalBounds().removeFromBottom (20).reduced (2));
}
//==============================================================================
void Soundfiler::scrollBarMoved (ScrollBar* scrollBarThatHasMoved, double newRangeStart)
{
    if (scrollBarThatHasMoved == scrollbar)
        setRange (visibleRange.movedToStartAt (newRangeStart));
}

void Soundfiler::setFile (const File& file)
{
    if (! file.isDirectory())
    {
        AudioFormatManager format;
        format.registerBasicFormats();
        //registers wav and aif format (just nescearry one time if you alays use the "getInstance()" method)
        AudioFormatReader* reader = format.createReaderFor (file);

        //creates a reader for the result file (may file, if the result/opened file is no wav or aif)
        if (reader) //if a reader got created
        {
            AudioSampleBuffer buffer (reader->numChannels, reader->lengthInSamples);
            buffer.clear();
            buffer.setSize (reader->numChannels, reader->lengthInSamples);
            reader->read (&buffer, 0, buffer.getNumSamples(), 0, true, true);
            setWaveform (buffer, reader->numChannels);
        }

        delete reader;
    }

    repaint (0, 0, getWidth(), getHeight());
}

//==============================================================================
void Soundfiler::setWaveform (AudioSampleBuffer buffer, int channels)
{
    thumbnail->clear();
    repaint();
    thumbnail->reset (channels, 44100, buffer.getNumSamples());
    //thumbnail->clear();
    thumbnail->addBlock (0, buffer, 0, buffer.getNumSamples());
    const Range<double> newRange (0.0, thumbnail->getTotalLength());
    scrollbar->setRangeLimits (newRange);
    setRange (newRange);
    setZoomFactor (zoom);
    repaint();
}

//==============================================================================
void Soundfiler::setZoomFactor (double amount)
{
    if (amount < 0)
    {
        zoomIn->setVisible (false);
        zoomOut->setVisible (false);
    }
    else
    {
        zoomIn->setVisible (true);
        zoomOut->setVisible (true);
    }

    if (thumbnail->getTotalLength() > 0)
    {
        const double newScale = jmax (0.001, thumbnail->getTotalLength() * (1.0 - jlimit (0.0, 0.99, amount)));
        const double timeAtCentre = xToTime (getWidth() / 2.0f);
        setRange (Range<double> (timeAtCentre - newScale * 0.5, timeAtCentre + newScale * 0.5));
    }

    zoom = amount;
    repaint();
}
//==============================================================================
void Soundfiler::setRange (Range<double> newRange)
{
    visibleRange = newRange;
    scrollbar->setCurrentRange (visibleRange);
    repaint();
}
//==============================================================================
void Soundfiler::paint (Graphics& g)
{
    g.fillAll (bgColour);
    g.setColour (colour);

    if (thumbnail->getTotalLength() != 0.0)
    {
        //if(GEN01 then draw thumbnail)
        Rectangle<int> thumbArea (getLocalBounds());
        thumbArea.setHeight (getHeight() - 14);
        thumbArea.setTop (10.f);
        thumbnail->drawChannels (g, thumbArea.reduced (2),
                                 visibleRange.getStart(), visibleRange.getEnd(), .8f);

        //if(regionWidth>1){
        g.setColour (colour.contrasting (.5f).withAlpha (.7f));
        float zoomFactor = thumbnail->getTotalLength() / visibleRange.getLength();

        //regionWidth = (regionWidth=2 ? 2 : regionWidth*zoomFactor)
        if (showScrubber)
            g.fillRect (timeToX (currentPlayPosition), 10.f, (regionWidth == 2 ? 2 : regionWidth * zoomFactor), (float)getHeight() - 26.f);

        //}

    }
    else
    {
        g.setColour (Colours::whitesmoke);
        g.setFont (14.0f);
        g.drawFittedText ("(No audio file loaded)", getLocalBounds(), Justification::centred, 2);
    }
}

//==============================================================================
void Soundfiler::mouseWheelMove (const MouseEvent&, const MouseWheelDetails& wheel)
{
    if (thumbnail->getTotalLength() > 0.5)
    {
        double newStart = visibleRange.getStart() - wheel.deltaX * (visibleRange.getLength()) / 10.0;
        newStart = jlimit (0.0, jmax (0.0, thumbnail->getTotalLength() - (visibleRange.getLength())), newStart);

        setRange (Range<double> (newStart, newStart + visibleRange.getLength()));

        repaint();
    }
}
//==============================================================================
void Soundfiler::mouseDown (const MouseEvent& e)
{
    if (!e.mods.isPopupMenu())
    {
        regionWidth = (1.01 - zoom) * 1.5;
        currentPlayPosition = jmax (0.0, xToTime ((float) e.x));
        loopStart = e.x;
        loopLength =  0;
        repaint();
        sendChangeMessage();
    }
}
//==============================================================================
void Soundfiler::mouseEnter (const MouseEvent& e)
{

}
//==============================================================================
void Soundfiler::mouseExit (const MouseEvent& e)
{

}
//==============================================================================
void Soundfiler::mouseDrag (const MouseEvent& e)
{
    if (selectableRange)
    {
        if (this->getLocalBounds().contains (e.getPosition()))
        {
            if (e.mods.isLeftButtonDown())
            {
                double zoomFactor = visibleRange.getLength() / thumbnail->getTotalLength();
                regionWidth = abs (e.getDistanceFromDragStartX()) * zoomFactor;

                if (e.getDistanceFromDragStartX() < 0)
                    currentPlayPosition = jmax (0.0, xToTime (loopStart + (float)e.getDistanceFromDragStartX()));

                float widthInTime = ((float)e.getDistanceFromDragStartX() / (float)getWidth()) * (float)thumbnail->getTotalLength();
                loopLength = jmax (0.0, widthInTime * zoomFactor);
            }

            repaint();
        }

    }
}
//==============================================================================
void Soundfiler::setScrubberPos (double pos)
{
    if (showScrubber)
    {
        currentPositionMarker->setVisible (true);
        pos = (pos / (thumbnail->getTotalLength() * sampleRate)) * thumbnail->getTotalLength();
        currentPositionMarker->setRectangle (Rectangle<float> (timeToX (pos) - 0.75f, 10,
                                                               1.5f, (float) (getHeight() - scrollbar->getHeight() - 10)));

        if (pos < 0.5)
            setRange (visibleRange.movedToStartAt (0));

        if (visibleRange.getEnd() <= thumbnail->getTotalLength())
            setRange (visibleRange.movedToStartAt (jmax (0.0, pos - (visibleRange.getLength() / 2.0))));

    }
}
//==============================================================================
void Soundfiler::mouseUp (const MouseEvent& e)
{
    sendChangeMessage();
}
