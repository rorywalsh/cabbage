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

#ifndef CABBAGELABEL_H_INCLUDED
#define CABBAGELABEL_H_INCLUDED

#include "../CabbageCommonHeaders.h"
#include "CabbageWidgetBase.h"

class CabbagePluginEditor;

class CabbageLabel : public Component, public ValueTree::Listener, public CabbageWidgetBase
{

    float rotate, corners;
    int pivotx, pivoty, fontstyle;
    CabbagePluginEditor* owner;
    int counter;
    String text, colour, fontcolour, align;
    Justification textAlign;

public:

    CabbageLabel (ValueTree wData, CabbagePluginEditor* _owner);
    ~CabbageLabel() {};

    //ValueTree::Listener virtual methods....
    void valueTreePropertyChanged (ValueTree& valueTree, const Identifier&);
    void valueTreeChildAdded (ValueTree&, ValueTree&)override {};
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override {};

    ValueTree widgetData;

    void resized() override {};
    void paint (Graphics& g)  override;
    void mouseDown (const MouseEvent& e)  override;
    void setText (String _text);


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabbageLabel);
};



#endif  // CABBAGELABEL_H_INCLUDED
