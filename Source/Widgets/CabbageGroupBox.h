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

#ifndef CABBAGEGROUPBOX_H_INCLUDED
#define CABBAGEGROUPBOX_H_INCLUDED


#include "../CabbageCommonHeaders.h"
#include "CabbageWidgetBase.h"

class CabbagePluginEditor;

class CabbageGroupBox : public GroupComponent, public ValueTree::Listener, public CabbageWidgetBase, public ChangeListener
{
    int outlineThickness = 0, lineThickness = 0, corners = 0, svgDebug = 0;
    String text = {}, colour = {}, fontColour = {}, justification = {}, outlineColour = {};
    float rotate = 0.f;
    CabbagePluginEditor* owner = {};
    File svgPath = {}, svgFile = {};
    int isVisible = true;
    CabbageLookAndFeel2 lookAndFeel;
public:

    CabbageGroupBox (ValueTree wData, CabbagePluginEditor* _owner);
    ~CabbageGroupBox() {
        widgetData.removeListener(this);
        setLookAndFeel(nullptr);
    };

    //ValueTree::Listener virtual methods....
    void valueTreePropertyChanged (ValueTree& valueTree, const Identifier&) override;
    void valueTreeChildAdded (ValueTree&, ValueTree&)override {};
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override {};

    void changeListenerCallback (ChangeBroadcaster* source)  override;

    ValueTree widgetData;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabbageGroupBox);
};


#endif  // CABBAGEGROUPBOX_H_INCLUDED
