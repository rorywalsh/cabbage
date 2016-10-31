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
    const Colour bgColour(CabbageSettings::getColourFromValueTree(colourTree, CabbageColourIds::menuBarBackground, Colours::grey));
    g.setColour (bgColour);
    g.fillAll();
}

//======== drawPopupMenuBackground ======================================================================
void CabbageIDELookAndFeel::drawPopupMenuBackground (Graphics& g, int width, int height)
{
    const Colour backgroundColour(CabbageSettings::getColourFromValueTree(colourTree, CabbageColourIds::popupMenuBackground, Colours::grey.darker()));
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
        Colour textColour (CabbageSettings::getColourFromValueTree(colourTree, CabbageColourIds::popupMenuText, Colours::white));

        if (textColourToUse != nullptr)
            textColour = *textColourToUse;

        Rectangle<int> r (area.reduced (1));

        if (isHighlighted)
        {
            g.setColour (CabbageSettings::getColourFromValueTree(colourTree, CabbageColourIds::popupMenuMouseOverBackground, Colours::grey));
            g.fillRect (r);

            g.setColour (CabbageSettings::getColourFromValueTree(colourTree, CabbageColourIds::popupMenuHighlightedText, Colours::grey));
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

    const Colour textColour(CabbageSettings::getColourFromValueTree(colourTree, CabbageColourIds::menuBarText, Colours::white));
    const Colour itemColour(CabbageSettings::getColourFromValueTree(colourTree, CabbageColourIds::menuBarMouseOverBackground, Colours::grey));

    if ((isMouseOverItem == true) || (isMenuOpen == true))
    {
        g.setColour (itemColour);
        g.fillRect (0, 0, width, height);
    }

    g.setColour (textColour);
    g.setFont (getMenuBarFont (menuBar, itemIndex, itemText));
    g.drawFittedText (itemText, 0, 0, width, height, Justification::centred, 1);
}

void CabbageIDELookAndFeel::drawAlertBox (Graphics& g,
                                       AlertWindow& alert,
                                       const Rectangle<int>& textArea,
                                       TextLayout& textLayout)
{
    g.fillAll (CabbageSettings::getColourFromValueTree(colourTree, CabbageColourIds::alertWindowBackground, Colour(250,250,250)));

    int iconSpaceUsed = 160;

    if (alert.getAlertType() != AlertWindow::NoIcon)
    {
        Path icon;
        uint32 colour;
        char character;

        if (alert.getAlertType() == AlertWindow::WarningIcon)
        {
			Rectangle<float> rect(alert.getLocalBounds().removeFromLeft(iconSpaceUsed).toFloat());
			const Image warningImage = ImageCache::getFromMemory(CabbageBinaryData::WarningIcon_png, CabbageBinaryData::WarningIcon_pngSize);
			g.drawImage(warningImage, rect.reduced(20));
        }
        if (alert.getAlertType() == AlertWindow::QuestionIcon)
        {
			Rectangle<float> rect(alert.getLocalBounds().removeFromLeft(iconSpaceUsed-20).toFloat());
			const Image warningImage = ImageCache::getFromMemory(CabbageBinaryData::WarningIcon_png, CabbageBinaryData::WarningIcon_pngSize);
			g.drawImage(warningImage, rect.reduced(25));
        }

    }

    g.setColour (alert.findColour (AlertWindow::textColourId));

    textLayout.draw (g, Rectangle<int> (textArea.getX() + iconSpaceUsed-50,
                                        textArea.getY(),
                                        textArea.getWidth() - iconSpaceUsed-40,
                                        textArea.getHeight()).toFloat());

    g.setColour (alert.findColour (AlertWindow::outlineColourId));
    g.drawRect (0, 0, alert.getWidth(), alert.getHeight());
}

//======== Scrollbars ==============================================================================
void CabbageIDELookAndFeel::drawScrollbar (Graphics &g, ScrollBar &scrollbar, int x, int y, int width,
                                        int height,
                                        bool isScrollbarVertical,
                                        int thumbStartPosition,
                                        int thumbSize,
                                        bool isMouseOver,
                                        bool isMouseDown)
{
    g.setColour (Colours::transparentBlack);
    g.fillAll();

    g.setColour (CabbageSettings::getColourFromValueTree(colourTree, CabbageColourIds::codeBackground, Colour(250,250,250)).contrasting(.5f));
    g.drawRect (x, y, width, height);

    if (isScrollbarVertical == false) //horizontal
        g.fillRoundedRectangle (thumbStartPosition+3, 3, jmax(0, thumbSize-6), height-6, 5);
    else //vertical
        g.fillRoundedRectangle (3, thumbStartPosition+3, width-6, jmax(1, thumbSize-6), 5);

}


