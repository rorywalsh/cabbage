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

#ifndef CABBAGEGENTABLE_H_INCLUDED
#define CABBAGEGENTABLE_H_INCLUDED


#include "../CabbageCommonHeaders.h"
#include "CabbageWidgetBase.h"
#include "Legacy/TableManager.h"

class CabbagePluginEditor;

class CabbageGenTable : public Component, public ValueTree::Listener, public CabbageWidgetBase, public ChangeListener
{
    String colour;
    String fontcolour;
    String file, tooltipText;
    var ampranges;
    float zoom, rotate;
    float startpos, endpos;
    double sampleRate;
    int pivotx, pivoty;
    double scrubberPos;
    var ampRanges;
	CabbagePluginEditor* owner;
	TableManager table;
	double scrubberPosition;
	Array <float, CriticalSection> tableValues;
	AudioSampleBuffer tableBuffer;
	var tables;
public:

    CabbageGenTable(ValueTree wData, CabbagePluginEditor* owner);
    ~CabbageGenTable() {};

    //ValueTree::Listener virtual methods....
    void valueTreePropertyChanged (ValueTree& valueTree, const Identifier&);
    void valueTreeChildAdded (ValueTree&, ValueTree&)override {};
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override {};
	void initialiseGenTable(ValueTree widgetData);
    void changeListenerCallback(ChangeBroadcaster *source);

	
	void resized();

    ValueTree widgetData;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabbageGenTable);
};



#endif  // CABBAGEGENTABLE_H_INCLUDED
