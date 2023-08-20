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

#ifndef CABBAGESOUNDFILER_H_INCLUDED
#define CABBAGESOUNDFILER_H_INCLUDED

#include "../CabbageCommonHeaders.h"
#include "CabbageWidgetBase.h"
#include "Legacy/Soundfiler.h"

class CabbagePluginEditor;
// Add any new custom widgets here to avoid having to edit makefiles and projects
// Each Cabbage widget should inherit from ValueTree listener, and CabbageWidgetBase
class CabbageSoundfiler : public Component, public ValueTree::Listener, public CabbageWidgetBase, public ChangeListener
{

    Soundfiler soundfiler;
    float scrubberPosition;
    String file;
    float zoom, rotate;
    int pivotx, pivoty;
    double sampleRate;
    float scrubberPos;

    CabbagePluginEditor* owner;
    Array <float, CriticalSection> tableValues;
    
public:

    CabbageSoundfiler (ValueTree wData, CabbagePluginEditor* _owner, int sr);
    ~CabbageSoundfiler() override {
        widgetData.removeListener(this);
    }

    void resized() override;

    void setFile (String newFile);
    void setWaveform (AudioSampleBuffer buffer, int sr, int channels);
    int getScrubberPosition();
    int getLoopLength();

    //ValueTree::Listener virtual methods....
    void valueTreePropertyChanged (ValueTree& valueTree, const Identifier&) override;
    void valueTreeChildAdded (ValueTree&, ValueTree&)override {}
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override {}

    void changeListenerCallback (ChangeBroadcaster* source) override;

    ValueTree widgetData;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabbageSoundfiler)
};



#endif  // CABBAGESOUNDFILER_H_INCLUDED
