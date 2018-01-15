/*
  ==============================================================================

    CabbageLookAndFeel2.h
    Created: 10 Nov 2016 5:36:11pm
    Author:  rory

  ==============================================================================
*/

#ifndef CABBAGELOOKANDFEEL2_H_INCLUDED
#define CABBAGELOOKANDFEEL2_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "../CabbageCommonHeaders.h"

class CabbageLookAndFeel2   : public LookAndFeel_V3
{
public:
    CabbageLookAndFeel2();
    ~CabbageLookAndFeel2() {};

    void drawToggleButton (Graphics& g, ToggleButton& button, bool isMouseOverButton, bool isButtonDown) override;

    void drawRotarySlider (Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, Slider& slider)override;

    void drawLinearSlider (Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const Slider::SliderStyle style, Slider& slider) override;

    void drawLinearSliderBackground (Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const Slider::SliderStyle style, Slider& slider) override;

    void drawLinearSliderThumb (Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const Slider::SliderStyle style, Slider& slider) override;

    Slider::SliderLayout getSliderLayout (Slider& slider) override;

    static Image drawToggleImage (float width, float height, bool isToggleOn, const Colour colour, const bool isRect, const float corners);

    void drawGroupComponentOutline (Graphics& g, int w, int h, const String& text, const Justification& position, GroupComponent& group) override;

    void drawButtonBackground (Graphics& g, Button& button, const Colour& backgroundColour, bool isMouseOverButton, bool isButtonDown) override;

    void drawComboBox (Graphics& g, int width, int height, bool /*isButtonDown*/, int /*buttonX*/, int /*buttonY*/, int /*buttonW*/, int /*buttonH*/, ComboBox& box) override;

    void drawPopupMenuBackground (Graphics& g, int width, int height) override;

    void drawPopupMenuItem (Graphics&, const Rectangle<int>& area,
                            bool isSeparator, bool isActive, bool isHighlighted, bool isTicked, bool hasSubMenu,
                            const String& text, const String& shortcutKeyText,
                            const Drawable* icon, const Colour* textColour) override;

    void drawAlertBox (Graphics& g, AlertWindow& alert, const Rectangle<int>& textArea, TextLayout& textLayout) override;
    Image drawCheckMark();

    static void drawFromSVG (Graphics& g, File svgFile, int x, int y, int newWidth, int newHeight, AffineTransform affine);

    void drawSphericalThumb (Graphics& g, const float x, const float y, const float w, const float h, const Colour& colour, const float outlineThickness);

    void drawTwoValueThumb (Graphics& g, float x, float y, float diameter, const Colour& colour, float outlineThickness, int direction);

    int getMinimumScrollbarThumbSize (ScrollBar& scrollbar) override;

    int getDefaultScrollbarWidth() override;

    int getScrollbarButtonSize (ScrollBar& scrollbar) override;

    void drawButtonText (Graphics& g, TextButton& button, bool /*isMouseOverButton*/, bool /*isButtonDown*/) override;

    void drawLabel (Graphics&, Label&) override;


    void setDefaultFont(File fontFile);
//    Font getTextButtonFont (TextButton&, int buttonHeight) override;
//    Font getComboBoxFont (ComboBox&) override;
//    Font getLabelFont (Label&) override;
//    Font getSliderPopupFont (Slider&) override;

//    Font getAlertWindowTitleFont() override;
//    Font getAlertWindowMessageFont() override;
//    Font getAlertWindowFont() override;
//
//    Font getPopupMenuFont() override;
//    Font getMenuBarFont (MenuBarComponent&, int itemIndex, const String& itemText) override;
private:

    Font customFont;


};

#endif  // CABBAGELOOKANDFEEL2_H_INCLUDED
