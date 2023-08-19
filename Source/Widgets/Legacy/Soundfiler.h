/*
  Copyright (C) 20139 Rory Walsh

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

#ifndef SOUNDFILEWAVEFORM_H
#define SOUNDFILEWAVEFORM_H

#include "../../CabbageCommonHeaders.h"

class ZoomButton;
//=================================================================
// display a sound file as a waveform..
//=================================================================
class Soundfiler : public Component,
    public ChangeBroadcaster,
    private ScrollBar::Listener,
    public ChangeListener
{
public:
    Soundfiler (int sr, Colour col, Colour fcol);
    ~Soundfiler() override;

    double getCurrentPlayPos()
    {
        return currentPlayPosition;
    }

    int getCurrentPlayPosInSamples()
    {
        return (int) (currentPlayPosition * sampleRate);
    }

    int getSampleRate()
    {
        return sampleRate;
    }
    
    void setSampleRate(int sr)
    {
        sampleRate = sr;
    }
    
    void setCurrentPlayPos(int posInSamples)
    {
        currentPlayPosition  = posInSamples / sampleRate;
    }

    void setRegionWidth(float width)
    {
        const double zoomFactor = visibleRange.getLength() / thumbnail->getTotalLength();
        regionWidth = (( width / ((float)thumbnail->getTotalLength() * sampleRate) ) * (float)getWidth()) * zoomFactor;
    }

    int getLoopLengthInSamples()
    {
        return (int)(loopLength * sampleRate);
    }

    void setIsRangeSelectable (bool isSelectable)
    {
        selectableRange = isSelectable;
    }

    void shouldShowScrubber (bool show)
    {
        showScrubber = show;
        currentPositionMarker->setVisible (show);
    }


    void setScrubberPos (double pos);

    float timeToX (const double time) const
    {
        return getWidth() * (float) ((time - visibleRange.getStart()) / (visibleRange.getLength()));
    }

    double xToTime (const float x) const
    {
        return (x / getWidth()) * (visibleRange.getLength()) + visibleRange.getStart();
    }

    void setWaveformColour (String waveform)
    {
        colour = Colour::fromString (waveform);
    }
    
    void setBackgroundColour (String tableColour)
    {
        bgColour = Colour::fromString (tableColour);
    }

    void setZoomFactor (double amount);
    void setFile (const File& file);
    void mouseWheelMove (const MouseEvent&, const MouseWheelDetails& wheel) override;
    void setWaveform (AudioSampleBuffer buffer, int channels);
    void createImage (String filename);
    void setRange (Range<double> newRange);
    void showScrollbars (bool show);
    void setMonoDisplayType(bool displayMono) { showSingleChannel = displayMono; }

private:
    Image img;
    bool selectableRange;
    bool showScrubber;
    //Graphics& graphics;
    int imgCount;
    Range<double> visibleRange;
    double zoom;
    std::unique_ptr<DrawableRectangle> currentPositionMarker;
    std::unique_ptr<ScrollBar> scrollbar;
    bool validFile = false;
    void resized() override;
    void paint (Graphics& g) override;
    void mouseDown (const MouseEvent& e) override;
    void mouseUp (const MouseEvent& e) override;
    void mouseEnter (const MouseEvent& e) override;
    void mouseDrag (const MouseEvent& e) override;
    void mouseExit (const MouseEvent& e) override;
    bool reDraw;
    double scrubberPosition;
    void scrollBarMoved (ScrollBar* scrollBarThatHasMoved, double newRangeStart) override;
    void changeListenerCallback (ChangeBroadcaster* source) override;
    std::unique_ptr<ZoomButton> zoomIn, zoomOut;

    AudioFormatManager formatManager;
    float sampleRate;
    float sessionSampleRate;
    float regionWidth;
    Image waveformImage;
    AudioThumbnailCache thumbnailCache;
    std::unique_ptr<AudioThumbnail> thumbnail;
    Colour colour, bgColour;
    int mouseDownX, mouseUpX;
   juce::Rectangle<int> localBounds;
    double loopLength;
    double loopStart;
    double currentPlayPosition;
    bool drawWaveform;
    bool showSingleChannel = false;
};


#endif // SOUNDFILEWAVEFORM_H
