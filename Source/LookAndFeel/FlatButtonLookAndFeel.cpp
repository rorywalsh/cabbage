#include "FlatButtonLookAndFeel.h"

void FlatButtonLookAndFeel::drawButtonBackground(Graphics &g, Button &button, const Colour &backgroundColour, bool isMouseOverButton, bool isButtonDown)
{
    const int width = button.getWidth();
    const int height = button.getHeight();
    
    const bool toggleState = button.getToggleState();
    
    Colour bg = button.findColour (toggleState == true ? TextButton::buttonOnColourId : TextButton::buttonColourId);

    if (isButtonDown == true)
        bg = bg.contrasting (0.2f);

    g.setColour (bg);
    g.fillRect (0, 0, width, height);

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
