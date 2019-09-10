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
static void drawFromSVG(Graphics& g, String svgText, int x, int y, int newWidth, int newHeight, AffineTransform affine)
{
    std::unique_ptr<XmlElement> svg(XmlDocument::parse(svgText));
    if(svg == nullptr)
        jassert(false);

    std::unique_ptr<Drawable> drawable;

    if (svg != nullptr)
    {
        drawable = Drawable::createFromSVG (*svg);
        drawable->setTransformToFit(Rectangle<float>(x, y, newWidth, newHeight), RectanglePlacement::stretchToFit);
        drawable->draw(g, 1.f, affine);
    }
}

class CabbageScrew : public Component, public ValueTree::Listener, public CabbageWidgetBase
{
    String name, tooltipText, shape;
    ValueTree widgetData;
    Image img;
    String svgText;

public:

    CabbageScrew (ValueTree cAttr);
    ~CabbageScrew() {};

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

public:

    CabbagePort (ValueTree cAttr);
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


