/*
 Copyright (C) 2016 @maurocsound, Rory Walsh
 
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
#include "../JuceLibraryCode/JuceHeader.h"
#include "../CabbageCommonHeaders.h"

class PropertyPanelLookAndFeel : public LookAndFeel_V3
{
public:
    PropertyPanelLookAndFeel() {};
    ~PropertyPanelLookAndFeel() {};

    void setColours (Colour panelBG, Colour labelBG, Colour labelText);

    void drawPropertyPanelSectionHeader(Graphics& g, const String& name, bool isOpen, int width, int height) override;
    void drawPropertyComponentBackground (Graphics&, int width, int height, PropertyComponent&) override;
    void drawPropertyComponentLabel (Graphics&, int width, int height, PropertyComponent&) override;

private:
    Colour panelBGcolour;
    Colour labelBGcolour;
    Colour labelTextColour;
};
