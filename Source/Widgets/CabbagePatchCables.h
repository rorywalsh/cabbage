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
class CabbagePluginEditor;

#pragma once

class CabbagePatchCables : public Component, public ValueTree::Listener
{
    Point<float> mousePos;
    Rectangle<float> originalDimensions;
    ValueTree widgetData;
    CabbagePluginEditor* owner;
    bool canDrawCable = false;
    
    struct Cable{
        Point<int> start;
        Point<int> end;
    };
    
    std::vector<Cable> cables;
    
public:
    CabbagePatchCables(ValueTree wData, CabbagePluginEditor* _owner);
    void paint(Graphics& g);
    void setStart(Point<int> pos, String channel);
    void setEnd(Point<int> pos, String channel);
    void resizeComponent(const MouseEvent& e);
    void addCable(Point<int> position);
    void enabledDrawing(bool enable){        canDrawCable = enable;    }
};
