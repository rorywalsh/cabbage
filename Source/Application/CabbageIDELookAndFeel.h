/*
  ==============================================================================

    CabbageIDELookAndFeel.h
    Created: 13 Oct 2016 11:31:50am
    Author:  rory

  ==============================================================================
*/

#ifndef CABBAGEIDELOOKANDFEEL_H_INCLUDED
#define CABBAGEIDELOOKANDFEEL_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "./Settings/CabbageSettings.h"
#include "CabbageIds.h"
#include "../Utilities/CabbageUtilities.h"
#include "../BinaryData/CabbageBinaryData.h"


class CabbageIDELookAndFeel	: public LookAndFeel_V3
{
public:
    CabbageIDELookAndFeel();
    ~CabbageIDELookAndFeel() {};

    void refreshLookAndFeel(ValueTree valueTree);

    void drawMenuBarBackground(Graphics &g, int width, int height, bool isMouseOverBar, MenuBarComponent &menuBar);
    void drawMenuBarItem(Graphics & g, int width, int height, int itemIndex,
                         const String &itemText,
                         bool isMouseOverItem,
                         bool isMenuOpen,
                         bool isMouseOverBar,
                         MenuBarComponent &menuBar);
    void drawPopupMenuBackground (Graphics& g, int width, int height);
    void drawPopupMenuItem(Graphics& g, const Rectangle<int>& area,
                           const bool isSeparator, const bool isActive,
                           const bool isHighlighted, const bool isTicked,
                           const bool hasSubMenu, const String& text,
                           const String& shortcutKeyText,
                           const Drawable* icon, const Colour* const textColourToUse);

	void drawAlertBox (Graphics& g,
                                       AlertWindow& alert,
                                       const Rectangle<int>& textArea,
                                       TextLayout& textLayout);
private:
    ValueTree colourTree;



};


#endif  // CABBAGEIDELOOKANDFEEL_H_INCLUDED
