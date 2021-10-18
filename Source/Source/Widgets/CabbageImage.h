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

#ifndef CABBAGEIMAGE_H_INCLUDED
#define CABBAGEIMAGE_H_INCLUDED

#include "../CabbageCommonHeaders.h"
#include "CabbageWidgetBase.h"

class CabbagePluginEditor;

class CabbageImage : public Component, public ValueTree::Listener, public CabbageWidgetBase, public ChangeListener
{
    String name, tooltipText, shape, svgElement, svgViewBox;
    File imgFile;
    CabbagePluginEditor* owner;
    float corners, cropx, cropy, cropwidth, cropheight;
    int lineThickness;
    ValueTree widgetData;
    Colour outlineColour, mainColour;
    bool isLineWidget = false, isParent = false;
    bool currentToggleValue = 0;
    Image img;
    bool usesSVGElement = false;
    double prevWidth = 0, prevHeight = 0;
    std::unique_ptr<Drawable> drawable;
    std::unique_ptr<XmlElement> svg;

public:

    CabbageImage (ValueTree cAttr, CabbagePluginEditor* _owner, bool isLineWidget = false);
    
    ~CabbageImage() override {
        widgetData.removeListener(this);
    }

    void valueTreePropertyChanged (ValueTree& valueTree, const Identifier&)  override;
    void paint (Graphics& g) override;
    String createSVG(ValueTree wData);
    void mouseDown (const MouseEvent& e) override;
	void updateImage(ValueTree& valueTree);
    void resizeAllChildren(ValueTree& valueTree);

    void valueTreeChildAdded (ValueTree&, ValueTree&) override {}
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override {}
    void changeListenerCallback (ChangeBroadcaster* source) override;
    String getTooltip()
    {
        return tooltipText;
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabbageImage)
};




#endif  // CABBAGEIMAGE_H_INCLUDED
