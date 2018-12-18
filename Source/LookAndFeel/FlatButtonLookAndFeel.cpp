#include "FlatButtonLookAndFeel.h"

void FlatButtonLookAndFeel::drawButtonBackground(Graphics &g, Button &button, const Colour &backgroundColour, bool isMouseOverButton, bool isButtonDown)
{
    const int width = button.getWidth();
    const int height = button.getHeight();
    
    const bool toggleState = button.getToggleState();
    
    Colour bg = button.findColour (toggleState == true ? TextButton::buttonOnColourId : TextButton::buttonColourId);

    if (isButtonDown == true || isMouseOverButton == true)
        bg = bg.contrasting (0.2f);

	const int corners = button.getProperties().getWithDefault("cornersize", 0);
	const Colour outlineColour(Colour::fromString(button.getProperties().getWithDefault("outlinecolour", Colours::white.toString()).toString()));
	const int outlineThickness = button.getProperties().getWithDefault("outlinethickness", 0);
	const int offset = outlineThickness == 0 ? 0 : outlineThickness * .5;
	g.setColour(outlineColour);
	g.fillRoundedRectangle(0, 0, width, height, corners);
	g.setColour(bg);
	g.fillRoundedRectangle(offset, offset, width - outlineThickness, height - outlineThickness, corners);
	
}

void FlatButtonLookAndFeel::drawButtonText(Graphics &g, TextButton &button, bool isMouseOverButton, bool isButtonDown)
{
    Font font (getTextButtonFont (button, button.getHeight() * 1.6));
    g.setFont (font);
    g.setColour (button.findColour (button.getToggleState() ? TextButton::textColourOnId
        : TextButton::textColourOffId)
        .withMultipliedAlpha (button.isEnabled() ? 1.0f : 0.5f));

    const int yIndent = jmin (4, button.proportionOfHeight (0.3f));
    const int cornerSize = jmin (button.getHeight(), button.getWidth()) / 2;

    const int fontHeight = roundToInt (font.getHeight() * 0.6f);
    const int leftIndent = jmin (fontHeight, 2 + cornerSize / (button.isConnectedOnLeft() ? 4 : 2));
    const int rightIndent = jmin (fontHeight, 2 + cornerSize / (button.isConnectedOnRight() ? 4 : 2));
    const int textWidth = button.getWidth() - leftIndent - rightIndent;

    if (textWidth > 0)
        g.drawFittedText (button.getButtonText(),
            leftIndent, yIndent, textWidth, button.getHeight() - yIndent * 2,
            Justification::centred, 2);

}

void FlatButtonLookAndFeel::drawDocumentWindowTitleBar (DocumentWindow& window, Graphics& g,
    int w, int h, int titleSpaceX, int titleSpaceW,
    const Image* icon, bool drawTitleTextOnLeft)
{
    if (w * h == 0)
        return;

    const bool isActive = window.isActiveWindow();

    g.setGradientFill (ColourGradient::vertical (window.getBackgroundColour(), 0,
        window.getBackgroundColour().contrasting (isActive ? titlebarContrastingGradient : std::max (0.0f, titlebarContrastingGradient - 0.10f)), (float)h));
    g.fillAll();

    Font font (h * 0.65f, Font::bold);
    g.setFont (font);

    int textW = font.getStringWidth (window.getName());
    int iconW = 0;
    int iconH = 0;

    if (icon != nullptr)
    {
        iconH = (int)font.getHeight();
        iconW = icon->getWidth() * iconH / icon->getHeight() + 4;
    }

    textW = jmin (titleSpaceW, textW + iconW);
    int textX = drawTitleTextOnLeft ? titleSpaceX
        : jmax (titleSpaceX, (w - textW) / 2);

    if (textX + textW > titleSpaceX + titleSpaceW)
        textX = titleSpaceX + titleSpaceW - textW;

    if (icon != nullptr)
    {
        g.setOpacity (isActive ? 1.0f : 0.6f);
        g.drawImageWithin (*icon, textX, (h - iconH) / 2, iconW, iconH,
            RectanglePlacement::centred, false);
        textX += iconW;
        textW -= iconW;
    }

    if (window.isColourSpecified (DocumentWindow::textColourId) || isColourSpecified (DocumentWindow::textColourId))
    {
        Colour fontcolour = window.findColour (DocumentWindow::textColourId);
        if (fontcolour.getAlpha() != 0)
            g.setColour (fontcolour.contrasting (isActive ? 0.0f : 0.4f));
        else
            g.setColour (fontcolour);
    }
    else
        g.setColour (window.getBackgroundColour().contrasting (isActive ? 0.7f : 0.4f));

    g.drawText (window.getName(), textX, 0, textW, h, Justification::centredLeft, true);
}
