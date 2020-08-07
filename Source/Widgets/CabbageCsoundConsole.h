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

#ifndef CABBAGECSOUNDCONSOLE_H_INCLUDED
#define CABBAGECSOUNDCONSOLE_H_INCLUDED

#include "../CabbageCommonHeaders.h"
#include "CabbageWidgetBase.h"

class CabbagePluginEditor;

class CabbageCsoundConsole : public TextEditor, public Timer, public ValueTree::Listener, public CabbageWidgetBase
{
public:

    CabbageCsoundConsole (ValueTree wData, CabbagePluginEditor* _owner);
    ~CabbageCsoundConsole() {};

    void setMonospaced(bool value);
    void setMonospaced(const ValueTree &valueTree);

    CabbagePluginEditor* owner;

    //ValueTree::Listener virtual methods....
    void valueTreePropertyChanged (ValueTree& valueTree, const Identifier&)  override;
    void valueTreeChildAdded (ValueTree&, ValueTree&)override {};
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override {};

    void timerCallback()  override;

    ValueTree widgetData;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabbageCsoundConsole);

private:
    bool monospaced = false;
    Font monospacedFont;
    Font defaultFont;
};



#endif  // CABBAGECSOUNDCONSOLE_H_INCLUDED
