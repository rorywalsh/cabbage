/*
 Copyright (C) 2016 @maurocsound, Rory Walsh
 
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

#pragma once
#include "JuceHeader.h"
#include "../CabbageCommonHeaders.h"

class FlatButtonLookAndFeel : public LookAndFeel_V3
{
public:
    FlatButtonLookAndFeel();
    ~FlatButtonLookAndFeel();

    float titlebarContrastingGradient = 0; // 0.0: no gradient in the titlebar - 1.0: max contrasting gradient

    void drawButtonBackground (Graphics& g, Button& button, const Colour& backgroundColour, bool isMouseOverButton, bool isButtonDown) override;

    void drawButtonText (Graphics& g, TextButton& button, bool isMouseOverButton, bool isButtonDown) override;

    void drawDocumentWindowTitleBar (DocumentWindow& window, Graphics& g,
        int w, int h, int titleSpaceX, int titleSpaceW,
        const Image* icon, bool drawTitleTextOnLeft) override;

    // H/V/ROTARY SLIDERS L&F:
    void drawLinearSlider(Graphics& g, int x, int y, int width, int height,
        float sliderPos, float minSliderPos, float maxSliderPos,
        const Slider::SliderStyle style, Slider& slider) override;

    void drawLinearSliderBackground (Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const Slider::SliderStyle style, Slider& slider) override;

    void drawLinearSliderThumb (Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const Slider::SliderStyle style, Slider& slider) override;

    void drawThumb (Graphics& g, const float x, const float y, const float w, const float h, const Colour& colour, const float outlineThickness);

    void drawTwoValueThumb (Graphics& g, float x, float y, float diameter, const Colour& colour, float outlineThickness, int direction);

    void drawRotarySlider (Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) override;
    // -----------------------
    
    Font getTextButtonFont (TextButton&, int buttonHeight) override;
    Font getComboBoxFont (ComboBox&) override;
    Font getLabelFont (Label&) override;
    Font getSliderPopupFont (Slider&) override;
    Slider::SliderLayout getSliderLayout(Slider& slider) override;
    
    //    Font getAlertWindowTitleFont() override;
    //    Font getAlertWindowMessageFont() override;
    //    Font getAlertWindowFont() override;
    //
    //    Font getPopupMenuFont() override;
    
    void setDefaultFont(File fontFile)
    {
        if(fontFile.existsAsFile())
        {
            std::unique_ptr<InputStream> inStream (fontFile.createInputStream());
            MemoryBlock mb;
            inStream->readIntoMemoryBlock(mb);
            Typeface::Ptr fontPtr = Typeface::createSystemTypefaceFor (mb.getData(), mb.getSize());
            customFont = Font(fontPtr);
        }
        else
            customFont = CabbageUtilities::getComponentFont();
    }
    
    Font customFont;
private:

};
