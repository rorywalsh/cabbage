/*
Copyright (C) 2016 Rory Walsh

Cabbage is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

Cabbage is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU General Public
License along with Csound; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
02111-1307 USA
*/


#ifndef CABBAGELOOKANDFEEL2_H_INCLUDED
#define CABBAGELOOKANDFEEL2_H_INCLUDED

#include "JuceHeader.h"
#include "../CabbageCommonHeaders.h"

inline std::unique_ptr<Drawable> createDrawableFromSVG (const char* data)
{
    auto xml = parseXML (data);
    jassert (xml != nullptr);
    return Drawable::createFromSVG (*xml);
}

class CabbageFoldersLookAndFeel   : public LookAndFeel_V4
{
public:
    CabbageFoldersLookAndFeel(){}
    ~CabbageFoldersLookAndFeel() override {}
    
    const Drawable* getDefaultFolderImage() override;
private:
    std::unique_ptr<Drawable> newFolderImage;
};

class CabbageLookAndFeel2   : public LookAndFeel_V3
{
public:
    CabbageLookAndFeel2();
    ~CabbageLookAndFeel2() override {}

    float titlebarContrastingGradient = 0; // 0.0: no gradient in the titlebar - 1.0: max contrasting gradient

    void drawDocumentWindowTitleBar (DocumentWindow& window, Graphics& g,
		int w, int h, int titleSpaceX, int titleSpaceW,
		const Image* icon, bool drawTitleTextOnLeft) override;
	
    void drawToggleButton (Graphics& g, ToggleButton& button, bool isMouseOverButton, bool isButtonDown) override;

    void drawRotarySlider (Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, Slider& slider)override;

    void drawLinearSlider (Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const Slider::SliderStyle style, Slider& slider) override;

    void drawLinearSliderBackground (Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const Slider::SliderStyle style, Slider& slider) override;

    void drawLinearSliderThumb (Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const Slider::SliderStyle style, Slider& slider) override;
    
    Slider::SliderLayout getSliderLayout (Slider&) override;

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

    void drawScrollbar (Graphics& g, ScrollBar& scrollbar, int x, int y, int width,
                                             int height,
                                             bool isScrollbarVertical,
                                             int thumbStartPosition,
                                             int thumbSize,
                                             bool isMouseOver,
                                             bool isMouseDown) override;

//    void setDefaultFont(File fontFile);
    void drawTooltip(Graphics&, const String& text, int width, int height) override;
    Font getTextButtonFont (TextButton&, int buttonHeight) override;
    Font getComboBoxFont (ComboBox&) override;
    Font getLabelFont (Label&) override;
    Font getSliderPopupFont (Slider&) override;
    void positionComboBoxText (ComboBox&, Label&) override;
    Font customFont;

//    Font getAlertWindowTitleFont() override;
//    Font getAlertWindowMessageFont() override;
//    Font getAlertWindowFont() override;
//
//    Font getPopupMenuFont() override;
//    Font getMenuBarFont (MenuBarComponent&, int itemIndex, const String& itemText) override;
private:

    

};

#endif  // CABBAGELOOKANDFEEL2_H_INCLUDED
