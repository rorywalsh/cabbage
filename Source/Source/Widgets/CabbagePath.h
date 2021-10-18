/*
 Copyright (C) 2021 Rory Walsh
 
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


#pragma once

#include "../CabbageCommonHeaders.h"
#include "CabbageWidgetBase.h"
#include "../Audio/Plugins/CabbagePluginEditor.h"

class CabbagePath : public Component, public ValueTree::Listener, public CabbageWidgetBase
{
    float rotate, corners;
    int pivotx, pivoty, fontstyle;
    int counter;
    String text, colour;
    int fontsize = 0;
    var points;
    CabbagePluginEditor* owner;
    
public:
    
    CabbagePath (ValueTree wData, CabbagePluginEditor* _owner);
    ~CabbagePath() override {
        widgetData.removeListener(this);
    }
    
    //ValueTree::Listener virtual methods....
    void valueTreePropertyChanged (ValueTree& valueTree, const Identifier&) override;
    void valueTreeChildAdded (ValueTree&, ValueTree&)override {}
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override {}
    
    ValueTree widgetData;
    
    void resized() override {}
    void paint (Graphics& g)  override;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabbagePath)
};


