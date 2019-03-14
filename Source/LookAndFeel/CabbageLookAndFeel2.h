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

#include "../JuceLibraryCode/JuceHeader.h"
#include "../CabbageCommonHeaders.h"

static Drawable* createDrawableFromSVG (const char* data)
{
    auto xml = parseXML (data);
    jassert (xml != nullptr);
    return Drawable::createFromSVG (*xml);
}

class CabbageFoldersLookAndFeel   : public LookAndFeel_V4
{
public:
    CabbageFoldersLookAndFeel(){};
    ~CabbageFoldersLookAndFeel() {};
    
    const Drawable* getDefaultFolderImage() override
    {
        if (newFolderImage == nullptr)
            newFolderImage.reset (createDrawableFromSVG (R"svgdata(
                                                      <svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" width="706" height="532">
                                                      <defs>
                                                      <linearGradient id="a">
                                                      <stop stop-color="#adf" offset="0"/>
                                                      <stop stop-color="#ecfaff" offset="1"/>
                                                      </linearGradient>
                                                      <linearGradient id="b" x1=".6" x2="0" y1=".9" xlink:href="#a"/>
                                                      <linearGradient id="c" x1=".6" x2=".1" y1=".9" y2=".3" xlink:href="#a"/>
                                                      </defs>
                                                      <g class="currentLayer">
                                                      <path d="M112.1 104c-8.2 2.2-13.2 11.6-11.3 21l68.3 342.7c1.9 9.4 10.1 15.2 18.4 13l384.3-104.1c8.2-2.2 13.2-11.6 11.3-21l-48-266a15.8 15.8 0 0 0-18.4-12.8l-224.2 38s-20.3-41.3-28.3-39.3z" display="block" fill="url(#b)" stroke="#444444" stroke-width="7"/>
                                                      <path d="M608.6 136.8L235.2 208a22.7 22.7 0 0 0-16 19l-40.8 241c1.7 8.4 9.6 14.5 17.8 12.3l380-104c8-2.2 10.7-10.2 12.3-18.4l38-210.1c.4-15.4-10.4-11.8-18-11.1z" display="block" fill="url(#c)" opacity=".8" stroke="#446c98" stroke-width="7"/>
                                                      </g>
                                                      </svg>
                                                      )svgdata"));
                               
                                  return newFolderImage.get();
    }
private:
    std::unique_ptr<Drawable> newFolderImage;
};

class CabbageLookAndFeel2   : public LookAndFeel_V3
{
public:
    CabbageLookAndFeel2();
    ~CabbageLookAndFeel2() {};

    float titlebarContrastingGradient = 0; // 0.0: no gradient in the titlebar - 1.0: max contrasting gradient

    void drawDocumentWindowTitleBar (DocumentWindow& window, Graphics& g,
		int w, int h, int titleSpaceX, int titleSpaceW,
		const Image* icon, bool drawTitleTextOnLeft) override;
	
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

    void drawScrollbar (Graphics& g, ScrollBar& scrollbar, int x, int y, int width,
                                             int height,
                                             bool isScrollbarVertical,
                                             int thumbStartPosition,
                                             int thumbSize,
                                             bool isMouseOver,
                                             bool isMouseDown);

    void setDefaultFont(File fontFile);
//    Font getTextButtonFont (TextButton&, int buttonHeight) override;
//    Font getComboBoxFont (ComboBox&) override;
    Font getLabelFont (Label&) override;
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
