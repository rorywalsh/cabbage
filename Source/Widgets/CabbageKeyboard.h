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

#ifndef CABBAGEKEYBOARD_H_INCLUDED
#define CABBAGEKEYBOARD_H_INCLUDED


#include "../CabbageCommonHeaders.h"
#include "CabbageWidgetBase.h"


// Add any new custom widgets here to avoid having to edit makefiles and projects
// Each Cabbage widget should inherit from ValueTree listener, and CabbageWidgetBase
class CabbageKeyboard : public MidiKeyboardComponent, public ValueTree::Listener, public CabbageWidgetBase
{
    int scrollbars;
    float keyWidth;
    String kind;

public:

    CabbageKeyboard (ValueTree wData, MidiKeyboardState& state);
    ~CabbageKeyboard() {
        widgetData.removeListener(this);
    };

    //VlaueTree::Listener virtual methods....
    void valueTreePropertyChanged (ValueTree& valueTree, const Identifier&) override;
    void valueTreeChildAdded (ValueTree&, ValueTree&)override {};
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override {};

    ValueTree widgetData;

    void updateColours(ValueTree& wData);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabbageKeyboard);
};


#endif  // CABBAGEKEYBOARD_H_INCLUDED
