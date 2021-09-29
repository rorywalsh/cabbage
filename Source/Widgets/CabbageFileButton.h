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

#ifndef CABBAGEFILEBUTTON_H_INCLUDED
#define CABBAGEFILEBUTTON_H_INCLUDED

#include "../CabbageCommonHeaders.h"
#include "CabbageWidgetBase.h"
#include "../LookAndFeel/FlatButtonLookAndFeel.h"
#include "../LookAndFeel/CabbageIDELookAndFeel.h"
class CabbagePluginEditor;

class CabbageFileButton : public TextButton, public ValueTree::Listener, public CabbageWidgetBase, public Button::Listener, public Timer
{
    CabbagePluginEditor* owner;
    String mode, filetype, tooltipText;

public:

    CabbageFileButton (ValueTree wData, CabbagePluginEditor* owner);
    ~CabbageFileButton() override {
        stopTimer();  
        setLookAndFeel(nullptr); 
        widgetData.removeListener(this);
    }

    //ValueTree::Listener virtual methods....
    void setFile(ValueTree wData);
    void valueTreePropertyChanged (ValueTree& valueTree, const Identifier&) override;
    void valueTreeChildAdded (ValueTree&, ValueTree&)override {}
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override {}
    String returnValidPath (File path);
    void setLookAndFeelColours (ValueTree wData);
    bool allowPresetChanges(String presetName);
    void buttonClicked (Button* button)  override;
    ValueTree widgetData;
    
    void timerCallback() override;

    FlatButtonLookAndFeel flatLookAndFeel;
    LookAndFeel_V4 lAndF;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabbageFileButton)
};



#endif  // CABBAGEFILEBUTTON_H_INCLUDED
