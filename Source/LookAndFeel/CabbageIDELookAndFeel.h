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
#ifndef CABBAGEIDELOOKANDFEEL_H_INCLUDED
#define CABBAGEIDELOOKANDFEEL_H_INCLUDED

#include "JuceHeader.h"
#include "../Settings/CabbageSettings.h"
#include "../CabbageIds.h"
#include "../Utilities/CabbageUtilities.h"
#include "../BinaryData/CabbageBinaryData.h"


class CabbageIDELookAndFeel : public LookAndFeel_V3
{
public:
    CabbageIDELookAndFeel();
    ~CabbageIDELookAndFeel() {};

    void refreshLookAndFeel (ValueTree valueTree);

    void drawMenuBarBackground (Graphics& g, int width, int height, bool isMouseOverBar, MenuBarComponent& menuBar) override;
    void drawMenuBarItem (Graphics& g, int width, int height, int itemIndex,
                          const String& itemText,
                          bool isMouseOverItem,
                          bool isMenuOpen,
                          bool isMouseOverBar,
                          MenuBarComponent& menuBar) override;
    void drawPopupMenuBackground (Graphics& g, int width, int height) override;
    void drawPopupMenuItem (Graphics&, const Rectangle<int>& area,
                            bool isSeparator, bool isActive, bool isHighlighted, bool isTicked, bool hasSubMenu,
                            const String& text, const String& shortcutKeyText,
                            const Drawable* icon, const Colour* textColour) override;

    void drawAlertBox (Graphics& g,
                       AlertWindow& alert,
                       const Rectangle<int>& textArea,
                       TextLayout& textLayout) override;
    void drawScrollbar (Graphics& g, ScrollBar& scrollbar, int x, int y, int width,
                        int height,
                        bool isScrollbarVertical,
                        int thumbStartPosition,
                        int thumbSize,
                        bool isMouseOver,
                        bool isMouseDown) override;

    ImageEffectFilter* getScrollbarEffect() override;
    int getMinimumScrollbarThumbSize (ScrollBar& scrollbar) override;
    int getDefaultScrollbarWidth() override;
    int getScrollbarButtonSize (ScrollBar& scrollbar) override;

    Rectangle<int> getPropertyComponentContentPosition (PropertyComponent& component) override;

private:
    ValueTree colourTree;



};


#endif  // CABBAGEIDELOOKANDFEEL_H_INCLUDED