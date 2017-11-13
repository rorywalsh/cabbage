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

class CabbageStringSequencer : public Component, public ValueTree::Listener, public CabbageWidgetBase, public KeyListener
{
public:

    // This is a custom Label component, which we use for the table's editable text columns.
    class EditableTextField  : public TextEditor
    {
    public:
        EditableTextField (CabbageStringSequencer& td);

//        void mouseDown (const MouseEvent& event) override
//        {
//            Label::mouseDown (event);
//        }
//
//        void textWasEdited() override
//        {
//            //owner.setText (columnId, row, getText());
//        }
//
//        // Our demo code will call this when we may need to update our contents
//        void setRowAndColumn (const int newRow, const int newColumn)
//        {
//            row = newRow;
//            columnId = newColumn;
//            //setText (owner.getText(columnId, row), dontSendNotification);
//        }
//
//        void paint (Graphics& g) override
//        {
//            auto& lf = getLookAndFeel();
//            if (! dynamic_cast<LookAndFeel_V4*> (&lf))
//                lf.setColour (textColourId, Colours::black);
//
//            Label::paint (g);
//        }

    private:
        CabbageStringSequencer& owner;
        int row, columnId;
        Colour textColour;
    };

    CabbageStringSequencer (ValueTree wData, CabbagePluginEditor* _owner);
    ~CabbageStringSequencer();

    CabbagePluginEditor* owner;

    //ValueTree::Listener virtual methods....
    void valueTreePropertyChanged (ValueTree& valueTree, const Identifier&) override;
    void valueTreeChildAdded (ValueTree&, ValueTree&)override {};
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override {};
    bool keyPressed (const KeyPress &key, Component *originatingComponent) override;
    TextEditor* getEditor(int column, int row);
    void swapFocusForEditors(KeyPress key, int col, int row);
    void highlightEditorText(int col, int row);
    ValueTree widgetData;

    void resized();

    OwnedArray<OwnedArray<TextEditor>> textFields;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabbageStringSequencer);

private:
    int numColumns = 0;
    int numRows = 0;
    Viewport vp;
    Component seqContainer;
};