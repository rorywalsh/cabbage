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


#include "../CabbageCommonHeaders.h"
#include "CabbageWidgetBase.h"


#pragma once

class CabbagePluginEditor;


class CabbageLight : public Component, public ValueTree::Listener, public CabbageWidgetBase
{
    String name, tooltipText, shape;
    File imgFile;
    CabbagePluginEditor* owner ={};
    float corners, cropx, cropy, cropwidth, cropheight;
    int lineThickness;
    ValueTree widgetData;
    Colour outlineColour, mainColour;
    bool isLineWidget = false;
    bool currentToggleValue = 0;
    Image img;

public:

    CabbageLight (ValueTree cAttr, CabbagePluginEditor* _owner);
    ~CabbageLight() {};

    void valueTreePropertyChanged (ValueTree& valueTree, const Identifier&)  override;
    void paint (Graphics& g) override;

    void valueTreeChildAdded (ValueTree&, ValueTree&) override {}
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override {}
    String getTooltip()
    {
        return tooltipText;
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabbageLight);
};

class CabbageScrew : public Component, public ValueTree::Listener, public CabbageWidgetBase
{
    String name, tooltipText, shape;
    ValueTree widgetData;
    Image img;
    String svgText;
    CabbagePluginEditor* owner = {};

public:

    explicit CabbageScrew (ValueTree cAttr, CabbagePluginEditor* _owner);
    ~CabbageScrew() {
        widgetData.removeListener(this);
    };

    void valueTreePropertyChanged (ValueTree& valueTree, const Identifier&)  override;
    void paint (Graphics& g) override;

    void valueTreeChildAdded (ValueTree&, ValueTree&) override {}
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override {}

    String getTooltip()
    {
        return tooltipText;
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabbageScrew);
};

class CabbagePort : public Component, public ValueTree::Listener, public CabbageWidgetBase
{
    String name, tooltipText, shape;
    ValueTree widgetData;
    Colour outlineColour, mainColour;
    Image img;
    String svgText;
    CabbagePluginEditor* owner = {};

public:

    explicit CabbagePort (ValueTree cAttr, CabbagePluginEditor* _owner);
    ~CabbagePort() {};

    void valueTreePropertyChanged (ValueTree& valueTree, const Identifier&)  override;
    void paint (Graphics& g) override;

    void valueTreeChildAdded (ValueTree&, ValueTree&) override {}
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override {}

    String getTooltip()
    {
        return tooltipText;
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabbagePort);
};


