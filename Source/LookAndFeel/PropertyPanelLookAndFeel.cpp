#include "PropertyPanelLookAndFeel.h"

void PropertyPanelLookAndFeel::setColours(Colour panelBG, Colour labelBG, Colour labelText)
{
    panelBGcolour = panelBG;
    labelBGcolour = labelBG;
    labelTextColour = labelText;
}

void PropertyPanelLookAndFeel::drawPropertyPanelSectionHeader(Graphics& g, const String& name, bool isOpen, int width, int height)
{
    auto buttonSize = height * 0.75f;
    auto buttonIndent = (height - buttonSize) * 0.5f;

    drawTreeviewPlusMinusBox (g, Rectangle<float> (buttonIndent, buttonIndent, buttonSize, buttonSize), panelBGcolour, isOpen, false);

    auto textX = (int)(buttonIndent * 2.0f + buttonSize + 2.0f);

    g.setColour (panelBGcolour.contrasting());
    g.setFont (Font (height * 0.7f, Font::bold));
    g.drawText (name, textX, 0, width - textX - 4, height, Justification::centredLeft, true);
}

void PropertyPanelLookAndFeel::drawPropertyComponentBackground (Graphics& g, int width, int height, PropertyComponent& component)
{
    g.setColour (labelBGcolour);
    g.fillRect (0, 0, width, height - 1);
}

void PropertyPanelLookAndFeel::drawPropertyComponentLabel (Graphics& g, int, int height, PropertyComponent& component)
{
    g.setColour (labelTextColour.withMultipliedAlpha (component.isEnabled() ? 1.0f : 0.6f));

    g.setFont (jmin (height, 24) * 0.65f);

    auto r = getPropertyComponentContentPosition (component);

    g.drawFittedText (component.getName(),
        3, r.getY(), r.getX() - 5, r.getHeight(),
        Justification::centredLeft, 2);
}