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
    ~Soundfiler();

    double getCurrentPlayPos()
    {
        return currentPlayPosition;
    }

    int getCurrentPlayPosInSamples()
    {
        return currentPlayPosition * sampleRate;
    }

    int getLoopLengthInSamples()
    {
        return loopLength * sampleRate;
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
    };
    void setBackgroundColour (String tableColour)
    {
        bgColour = Colour::fromString (tableColour);
    };

    void setZoomFactor (double amount);
    void setFile (const File& file);
    void mouseWheelMove (const MouseEvent&, const MouseWheelDetails& wheel);
    void setWaveform (AudioSampleBuffer buffer, int channels);
    void createImage (String filename);

private:
    Image img;
    bool selectableRange;
    bool showScrubber;
    //Graphics& graphics;
    int imgCount;
    Range<double> visibleRange;
    double zoom;
    ScopedPointer<DrawableRectangle> currentPositionMarker;
    ScopedPointer<ScrollBar> scrollbar;
    void setRange (Range<double> newRange);
    void resized();
    void paint (Graphics& g);
    void mouseDown (const MouseEvent& e);
    void mouseUp (const MouseEvent& e);
    void mouseEnter (const MouseEvent& e);
    void mouseDrag (const MouseEvent& e);
    void mouseExit (const MouseEvent& e);
    bool reDraw;
    double scrubberPosition;
    void scrollBarMoved (ScrollBar* scrollBarThatHasMoved, double newRangeStart);
    void changeListenerCallback (ChangeBroadcaster* source);
    ScopedPointer<ZoomButton> zoomIn, zoomOut;

    AudioFormatManager formatManager;
    float sampleRate;
    float regionWidth;
    Image waveformImage;
    AudioThumbnailCache thumbnailCache;
    ScopedPointer<AudioThumbnail> thumbnail;
    Colour colour, bgColour;
    int mouseDownX, mouseUpX;
    Rectangle<int> localBounds;
    double loopLength;
    double loopStart;
    double currentPlayPosition;
    bool drawWaveform;
};


#endif // SOUNDFILEWAVEFORM_H
