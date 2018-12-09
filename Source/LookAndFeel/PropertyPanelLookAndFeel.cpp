#include "PropertyPanelLookAndFeel.h"

void PropertyPanelLookAndFeel::setBackgroundColour(Colour bg)
{
    backgroundColour = bg;
}

void PropertyPanelLookAndFeel::drawPropertyPanelSectionHeader(Graphics& g, const String& name, bool isOpen, int width, int height)
{
    auto buttonSize = height * 0.75f;
    auto buttonIndent = (height - buttonSize) * 0.5f;

    drawTreeviewPlusMinusBox (g, Rectangle<float> (buttonIndent, buttonIndent, buttonSize, buttonSize), backgroundColour, isOpen, false);

    auto textX = (int)(buttonIndent * 2.0f + buttonSize + 2.0f);

    g.setColour (backgroundColour.contrasting());
    g.setFont (Font (height * 0.7f, Font::bold));
    g.drawText (name, textX, 0, width - textX - 4, height, Justification::centredLeft, true);
}