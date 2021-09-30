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

class CabbagePluginEditor;

class CabbageForm : public CabbageWidgetBase, public Component,
public FileDragAndDropTarget,
public TextDragAndDropTarget,
public ValueTree::Listener
{
    Colour colour;
    CabbagePluginEditor* owner;
    int latency = 32;
    int openGL = 0;
    
public:
    
    CabbageForm (CabbagePluginEditor* _owner);
    
    ~CabbageForm() override {
        widgetData.removeListener(this);
    }
    
    void setValueTree(ValueTree vt)
    {
        widgetData = vt;
        widgetData.addListener (this);
        setName (CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::name));
    }
    
    void setColour (Colour col)
    {
        colour = col;
    }
    
    void paint (Graphics& g)  override
    {
        //g.setOpacity (0);
        g.fillAll (colour);
    }
    
    bool isInterestedInFileDrag (const StringArray& /*files*/) override{ return true; }
    void fileDragEnter (const StringArray& /*files*/, int /*x*/, int /*y*/) override{}
    void fileDragMove (const StringArray& /*files*/, int /*x*/, int /*y*/) override {}
    void fileDragExit (const StringArray& /*files*/) override {}
    void filesDropped (const StringArray& files, int x, int y) override;
    
    bool isInterestedInTextDrag (const String& /*files*/) override{ return true; }
    void textDragEnter (const String& /*files*/, int /*x*/, int /*y*/) override {}
    void textDragMove (const String& /*files*/, int /*x*/, int /*y*/) override {}
    void textDragExit (const String& /*files*/) override {}
    void textDropped (const String& text, int x, int y) override;
    
    //ValueTree::Listener virtual methods....
    void valueTreePropertyChanged (ValueTree& valueTree, const Identifier&) override;
    void valueTreeChildAdded (ValueTree&, ValueTree&)override {}
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override {}
    
    ValueTree widgetData;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabbageForm)
};


