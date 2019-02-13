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

    // H/V/ROTARY SLIDERS L&F:
    void drawLinearSliderBackground (Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const Slider::SliderStyle style, Slider& slider) override;

    void drawLinearSliderThumb (Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const Slider::SliderStyle style, Slider& slider) override;

    void drawThumb (Graphics& g, const float x, const float y, const float w, const float h, const Colour& colour, const float outlineThickness);

    void drawTwoValueThumb (Graphics& g, float x, float y, float diameter, const Colour& colour, float outlineThickness, int direction);

    void drawRotarySlider (Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider);
    // -----------------------
};
