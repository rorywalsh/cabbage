#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "../CabbageCommonHeaders.h"

class FlatButtonLookAndFeel : public LookAndFeel_V3
{
public:
    FlatButtonLookAndFeel() {};
    ~FlatButtonLookAndFeel() {};

    float titlebarContrastingGradient = 0; // 0.0: no gradient in the titlebar - 1.0: max contrasting gradient

    void drawButtonBackground (Graphics& g, Button& button, const Colour& backgroundColour, bool isMouseOverButton, bool isButtonDown) override;

    void drawButtonText (Graphics& g, TextButton& button, bool isMouseOverButton, bool isButtonDown) override;

    void drawDocumentWindowTitleBar (DocumentWindow& window, Graphics& g,
        int w, int h, int titleSpaceX, int titleSpaceW,
        const Image* icon, bool drawTitleTextOnLeft) override;
};
