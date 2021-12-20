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

#ifndef CABBAGESIGNALDISPLAY_H_INCLUDED
#define CABBAGESIGNALDISPLAY_H_INCLUDED


#include "../CabbageCommonHeaders.h"
#include "CabbageWidgetBase.h"

#include "Legacy/FrequencyRangeDisplayComponent.h"

class CabbagePluginEditor;

class CabbageSignalDisplay : public Component, public ValueTree::Listener, public CabbageWidgetBase, public ChangeListener,
    public Timer, private ScrollBar::Listener
{

    String name, displayType;
    RoundButton zoomInButton, zoomOutButton;
    Array<float, CriticalSection> signalFloatArray;
    Array<float, CriticalSection> signalFloatArray2;
    var signalVariables;
    int tableNumber, freq, shouldDrawSonogram, leftPos, scrollbarHeight,
        minFFTBin, maxFFTBin, vectorSize, zoomLevel, scopeWidth, lineThickness;
    Colour fontColour, colour, backgroundColour, outlineColour;
    ScrollBar scrollbar;
    bool isScrollbarShowing;
    float rotate;
    bool shouldPaint {false};
    int updateRate {200};

    Image spectrogramImage, spectroscopeImage;
    FrequencyRangeDisplayComponent freqRangeDisplay;
    Range<int> freqRange;
    float skew = 1;

    CabbagePluginEditor* owner;

public:

    CabbageSignalDisplay (ValueTree wData, CabbagePluginEditor* owner);
    ~CabbageSignalDisplay() override {
        widgetData.removeListener(this);
    }

    //ValueTree::Listener virtual methods....
    void valueTreePropertyChanged (ValueTree& valueTree, const Identifier&) override;
    void valueTreeChildAdded (ValueTree&, ValueTree&)override {}
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override {}

    ValueTree widgetData;

    void setBins (int min, int max);
    void scrollBarMoved (ScrollBar* scrollBarThatHasMoved, double newRangeStart) override;
    void changeListenerCallback (ChangeBroadcaster* source) override;
    void drawSonogram();
    void drawSpectroscope (Graphics& g);
    void drawWaveform (Graphics& g);
    void drawLissajous (Graphics& g);
    void paint (Graphics& g) override;
    void setSignalFloatArray (Array<float, CriticalSection> _points);
    void setSignalFloatArraysForLissajous (Array<float, CriticalSection> _points1, Array<float, CriticalSection> _points2);
    void resized() override;
    void mouseMove (const MouseEvent& e) override;
    void showPopup (String text);
    void showScrollbar (bool show);
    void zoomOut (int factor = 1);
    void zoomIn (int factor = 1);
    void timerCallback() override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabbageSignalDisplay)
};


//===========================================================================

#endif  // CABBAGESIGNALDISPLAY_H_INCLUDED
