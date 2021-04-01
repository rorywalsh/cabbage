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

#ifndef CABBAGETEXTEDITOR_H_INCLUDED
#define CABBAGETEXTEDITOR_H_INCLUDED

#include "../CabbageCommonHeaders.h"
#include "CabbageWidgetBase.h"

class CabbagePluginEditor;

class CabbageTextEditor : public Component, public ValueTree::Listener, public CabbageWidgetBase, public TextEditor::Listener, public KeyListener
{
public:

    class CustomTextEditor : public TextEditor //custom text editor with right-click popup menu
    {
    public:
        bool toggleEditOnDoubleClick = false;
        
        explicit CustomTextEditor (CabbageTextEditor* _owner): TextEditor (""), owner (_owner) {}
        ~CustomTextEditor() {}

        void addPopupMenuItems (PopupMenu& menuToAddTo, const MouseEvent* mouseClickEvent) override
        {
            menuToAddTo.addItem (1, "Cut");
            menuToAddTo.addItem (2, "Copy");
            menuToAddTo.addItem (3, "Paste");
            menuToAddTo.addItem (4, "Select All");
            menuToAddTo.addSeparator();
            menuToAddTo.addItem (5, "Send text");
        }

        void mouseDoubleClick(const MouseEvent &e) override
        {
            if(toggleEditOnDoubleClick){
                if(isReadOnly())
                    setReadOnly(false);
                else
                    setReadOnly(true);
            }
        }
        
        void performPopupMenuAction (int menuItemID) override
        {
            if (menuItemID == 1)
                cutToClipboard();
            else if (menuItemID == 2)
                copyToClipboard();
            else if (menuItemID == 3)
                pasteFromClipboard();
            else if (menuItemID == 4)
                selectAll();
            else if (menuItemID == 5)
                owner->sendTextToCsound();
        }

    private:
        CabbageTextEditor* owner;

    };


    CabbageTextEditor (ValueTree wData, CabbagePluginEditor* _owner);
    ~CabbageTextEditor() {};

    CabbagePluginEditor* owner;

    //ValueTree::Listener virtual methods....
    void valueTreePropertyChanged (ValueTree& valueTree, const Identifier&) override;
    void valueTreeChildAdded (ValueTree&, ValueTree&)override {};
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override {};

    ValueTree widgetData;

    void sendTextToCsound();
    bool keyPressed (const juce::KeyPress& key, Component*) override;
    void textEditorReturnKeyPressed (TextEditor&) override;
    void resized() override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabbageTextEditor);

private:
    CustomTextEditor textEditor;
    bool isMultiline = false;
    int stringIndex = 0;
    StringArray strings;
};




#endif  // CABBAGETEXTEDITOR_H_INCLUDED
