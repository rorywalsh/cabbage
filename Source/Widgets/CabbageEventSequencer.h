/*
  Copyright (C) 2017 Rory Walsh

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

class CabbageEventSequencer : public Component, public ValueTree::Listener, public CabbageWidgetBase, public KeyListener
{
public:

    CabbageEventSequencer (ValueTree wData, CabbagePluginEditor* _owner);
    ~CabbageEventSequencer();



    void resized();
    bool keyPressed (const KeyPress& key, Component* originatingComponent) override;
    TextEditor* getEditor (int column, int row);
    void swapFocusForEditors (KeyPress key, int col, int row);
    void highlightEditorText (int col, int row);
    void setCellData(int col, int row, const String data);
    void updateCurrentStepPosition();
    void arrangeTextEditors(ValueTree wData);

    //ValueTree::Listener virtual methods....
    void valueTreePropertyChanged (ValueTree& valueTree, const Identifier&) override;
    void valueTreeChildAdded (ValueTree&, ValueTree&)override {};
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override {};
    void setColours(ValueTree wData);
    void createNumberLabels(ValueTree wData, int height, int showNumbers);

    ValueTree widgetData;



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabbageEventSequencer);

private:
    int numColumns = 0;
    int numRows = 0;
    int currentBeat = 0;
    int numbersWidth = 20;
    int bpm = 120;
    int previousBpm = 120;
    Viewport vp;
    Component seqContainer;
    OwnedArray<OwnedArray<TextEditor>> textFields;
    OwnedArray<Label> stepNumbers;
    CabbagePluginEditor* owner;
    String stepChannel = "";
};