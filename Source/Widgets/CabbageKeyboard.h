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

#ifndef CABBAGEKEYBOARD_H_INCLUDED
#define CABBAGEKEYBOARD_H_INCLUDED


#include "../CabbageCommonHeaders.h"
#include "CabbageWidgetBase.h"
#include "../Audio/Plugins/CabbagePluginEditor.h"


// Add any new custom widgets here to avoid having to edit makefiles and projects
// Each Cabbage widget should inherit from ValueTree listener, and CabbageWidgetBase
class CabbageKeyboard : public MidiKeyboardComponent, public ValueTree::Listener, public CabbageWidgetBase
{
public:

    enum Direction
    {
        left,
        right,
        up,
        down,
        undefined
    };
    
    struct PathPoint
    {
        PathPoint(float left, float top, float radius)
        :x(left), y(top), corners(radius){}
        float x;
        float y;
        float corners;
    };
    
    
    CabbageKeyboard (ValueTree wData, CabbagePluginEditor* _owner, MidiKeyboardState& state);
    ~CabbageKeyboard() override {
        widgetData.removeListener(this);
    }
    
    
    void drawBlackNote (int midiNoteNumber,
                                Graphics& g, Rectangle<float> area,
                                bool isDown, bool isOver,
                                Colour noteFillColour) override;
    
    void drawWhiteNote (int midiNoteNumber,
                                Graphics& g, Rectangle<float> area,
                                bool isDown, bool isOver,
                                Colour lineColour, Colour textColour) override;

    
    void drawNoteOutline(Graphics& g, int midiNote, Rectangle<float> area);
    
    Direction getNextDirection(std::vector<PathPoint>, std::size_t segment);
    void drawRoundedPath(Graphics& g, std::vector<PathPoint>, float lineThickness);
    
    void mouseUpOnKey (int midiNoteNumber, const MouseEvent &e) override;
    bool mouseDraggedToKey (int midiNoteNumber, const MouseEvent &e) override;
    
    //VlaueTree::Listener virtual methods....
    void valueTreePropertyChanged (ValueTree& valueTree, const Identifier&) override;
    void valueTreeChildAdded (ValueTree&, ValueTree&)override {}
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override {}

    ValueTree widgetData;

    void updateColours(ValueTree& wData);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabbageKeyboard)
    
private:
    int scrollbars;
    float keyWidth, corners;
    String kind;
    float outlineThickness = 1.f;
    float lineThickness = 1.f;
    CabbagePluginEditor* owner;
    Colour mouseOverOutlineColour;
    Rectangle<float> blackNoteArea;
    float blackNoteLength = 0.7f;
    float blackNoteWidth = 1.f;
    bool scrollBars = true;
    std::unique_ptr<Drawable> outlineDrawable;
    
    Direction prevDirection;
};


#endif  // CABBAGEKEYBOARD_H_INCLUDED
