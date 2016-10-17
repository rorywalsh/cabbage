/*
  ==============================================================================

    CabbageStandaloneLookAndFeel.cpp
    Created: 13 Oct 2016 11:28:50am
    Author:  rory

  ==============================================================================
*/

#include "CabbageIDELookAndFeel.h"


//Cabbage IDE look and feel class
CabbageIDELookAndFeel::CabbageIDELookAndFeel()
{

}

//======== Update value tree ======================================================================
void CabbageIDELookAndFeel::refreshLookAndFeel(ValueTree valueTree)
{
    colourTree = valueTree;
}
//======== Menubar background ======================================================================
void CabbageIDELookAndFeel::drawMenuBarBackground(Graphics &g, int width, int height, bool isMouseOverBar, MenuBarComponent &menuBar)
{
    const Colour bgColour(CabbageSettings::getColourFromValueTree(colourTree, ColourIds::menuBarBackground, Colours::grey));
    g.setColour (bgColour);
    g.fillAll();
}

//======== drawPopupMenuBackground ======================================================================
void CabbageIDELookAndFeel::drawPopupMenuBackground (Graphics& g, int width, int height)
{
    const Colour backgroundColour(CabbageSettings::getColourFromValueTree(colourTree, ColourIds::popupMenuBackground, Colours::grey.darker()));
    g.fillAll (backgroundColour);

}
//======== drawPopupMenuItem ======================================================================
void CabbageIDELookAndFeel::drawPopupMenuItem (Graphics& g, const Rectangle<int>& area,
        const bool isSeparator, const bool isActive,
        const bool isHighlighted, const bool isTicked,
        const bool hasSubMenu, const String& text,
        const String& shortcutKeyText,
        const Drawable* icon, const Colour* const textColourToUse)
{
    if (isSeparator)
    {
        Rectangle<int> r (area.reduced (5, 0));
        r.removeFromTop (r.getHeight() / 2 - 1);

        g.setColour (Colour (0x33000000));
        g.fillRect (r.removeFromTop (1));

        g.setColour (Colour (0x66ffffff));
        g.fillRect (r.removeFromTop (1));
    }
    else
    {
        Colour textColour (CabbageSettings::getColourFromValueTree(colourTree, ColourIds::popupMenuText, Colours::white));

        if (textColourToUse != nullptr)
            textColour = *textColourToUse;

        Rectangle<int> r (area.reduced (1));

        if (isHighlighted)
        {
            g.setColour (CabbageSettings::getColourFromValueTree(colourTree, ColourIds::popupMenuMouseOverBackground, Colours::grey));
            g.fillRect (r);

            g.setColour (CabbageSettings::getColourFromValueTree(colourTree, ColourIds::popupMenuHighlightedText, Colours::grey));
        }
        else
        {
            g.setColour (textColour);
        }

        if (! isActive)
            g.setOpacity (0.3f);

        Font font (getPopupMenuFont());

        const float maxFontHeight = area.getHeight() / 1.3f;

        if (font.getHeight() > maxFontHeight)
            font.setHeight (maxFontHeight);

        g.setFont (font);

        Rectangle<float> iconArea (r.removeFromLeft ((r.getHeight() * 5) / 4).reduced (3).toFloat());

        if (icon != nullptr)
        {
            icon->drawWithin (g, iconArea, RectanglePlacement::centred | RectanglePlacement::onlyReduceInSize, 1.0f);
        }
        else if (isTicked)
        {
            const Path tick (getTickShape (1.0f));
            g.fillPath (tick, tick.getTransformToScaleToFit (iconArea, true));
        }

        if (hasSubMenu)
        {
            const float arrowH = 0.6f * getPopupMenuFont().getAscent();

            const float x = (float) r.removeFromRight ((int) arrowH).getX();
            const float halfH = (float) r.getCentreY();

            Path p;
            p.addTriangle (x, halfH - arrowH * 0.5f,
                           x, halfH + arrowH * 0.5f,
                           x + arrowH * 0.6f, halfH);

            g.fillPath (p);
        }

        r.removeFromRight (3);
        g.drawFittedText (text, r, Justification::centredLeft, 1);

        if (shortcutKeyText.isNotEmpty())
        {
            Font f2 (font);
            f2.setHeight (f2.getHeight() * 0.75f);
            f2.setHorizontalScale (0.95f);
            g.setFont (f2);

            g.drawText (shortcutKeyText, r, Justification::centredRight, true);
        }
    }
}
//======== Menubar item background ======================================================================
void CabbageIDELookAndFeel::drawMenuBarItem(Graphics & g, int width, int height, int itemIndex,
        const String &itemText,
        bool isMouseOverItem,
        bool isMenuOpen,
        bool isMouseOverBar,
        MenuBarComponent &menuBar)
{

    const Colour textColour(CabbageSettings::getColourFromValueTree(colourTree, ColourIds::menuBarText, Colours::white));
    const Colour itemColour(CabbageSettings::getColourFromValueTree(colourTree, ColourIds::menuBarMouseOverBackground, Colours::grey));

    if ((isMouseOverItem == true) || (isMenuOpen == true))
    {
        g.setColour (itemColour);
        g.fillRect (0, 0, width, height);
    }

    g.setColour (textColour);
    g.setFont (getMenuBarFont (menuBar, itemIndex, itemText));
    g.drawFittedText (itemText, 0, 0, width, height, Justification::centred, 1);
}