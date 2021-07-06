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

#include "FlatButtonLookAndFeel.h"

namespace LookAndFeelHelpers
{
    static Colour createBaseColour (Colour buttonColour,
        bool hasKeyboardFocus,
        bool isMouseOverButton,
        bool isButtonDown) noexcept
    {
        const float sat = hasKeyboardFocus ? 1.3f : 0.9f;
        const Colour baseColour (buttonColour.withMultipliedSaturation (sat));

        if (isButtonDown)      return baseColour.contrasting (0.2f);

        if (isMouseOverButton) return baseColour.contrasting (0.1f);

        return baseColour;
    }

    static TextLayout layoutTooltipText (const String& text, Colour colour) noexcept
    {
        const float tooltipFontSize = 13.0f;
        const int maxToolTipWidth = 400;

        AttributedString s;
        s.setJustification (Justification::centred);
        s.append (text, Font (tooltipFontSize, Font::bold), colour);

        TextLayout tl;
        tl.createLayoutWithBalancedLineLengths (s, (float)maxToolTipWidth);
        return tl;
    }
}

FlatButtonLookAndFeel::FlatButtonLookAndFeel() {
    //setDefaultFont(File("/Users/walshr/Documents/Csoundfiles/RobotoCondensed-Italic.ttf"));
};
FlatButtonLookAndFeel::~FlatButtonLookAndFeel() {};

void FlatButtonLookAndFeel::drawButtonBackground(Graphics &g, Button &button, const Colour &backgroundColour, bool isMouseOverButton, bool isButtonDown)
{
    const int width = button.getWidth();
    const int height = button.getHeight();
    
    const bool toggleState = button.getToggleState();
    
    Colour bg = button.findColour (toggleState == true ? TextButton::buttonOnColourId : TextButton::buttonColourId);

    if (isButtonDown == true)
        bg = bg.contrasting (0.2f);
    else if (isMouseOverButton == true)
        bg = bg.contrasting (0.3f);

	const int corners = button.getProperties().getWithDefault("corners", 0);
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
    Font font (FlatButtonLookAndFeel::getTextButtonFont (button, button.getHeight() * 1.6));
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


//========= linear slider ================================================================================
void FlatButtonLookAndFeel::drawLinearSlider(Graphics& g, int x, int y, int width, int height,
    float sliderPos, float minSliderPos, float maxSliderPos,
    const Slider::SliderStyle style, Slider& slider)
{
    const int filmStrip = slider.getProperties().getWithDefault("filmstrip", 0);
    if (filmStrip == 1)
    {
        g.fillAll(Colours::transparentWhite);
    }
    else
    {
        if (style == Slider::LinearBar || style == Slider::LinearBarVertical)
        {
            g.setColour(slider.findColour(Slider::thumbColourId));
            g.fillRoundedRectangle(x, y, width, height, 3);
        }
        else
        {
            drawLinearSliderBackground(g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);
            drawLinearSliderThumb(g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);
        }
    }
    
}

//=========== Linear Slider Background ===========================================================================
void FlatButtonLookAndFeel::drawLinearSliderBackground (Graphics& g, int x, int y, int width, int height, float sliderPos,
    float minSliderPos,
    float maxSliderPos,
    const Slider::SliderStyle style,
    Slider& slider)
{
    const float sliderRadius = (float)(getSliderThumbRadius (slider) - 2);
    const Colour trackColour (slider.findColour (Slider::trackColourId));
    float zeroPosProportional = 0;

    if (slider.getMinimum() < 0)
        zeroPosProportional = slider.valueToProportionOfLength (0); //takes into account skew factor

    const float trackerThickness = slider.getProperties().getWithDefault ("trackerthickness", .75);
    const int gapMarkers = slider.getProperties().getWithDefault ("gapmarkers", 0);
    const Colour trackerBgColour = Colour::fromString(slider.getProperties().getWithDefault ("trackerbgcolour", Colours::black.toString()).toString());

    Path indent;

    if (slider.isHorizontal())
    {
        width = width - 8;

        // gap markers:
        if (gapMarkers)
        {
            g.setColour (Colours::whitesmoke);
            g.setOpacity (0.6);
            const float midPoint = (width / 2.f + sliderRadius) + 3;
            const float markerGap = width / 9.f;
            g.drawLine (midPoint, height * 0.25, midPoint, height * 0.75, 1.5);
            g.setOpacity (0.3);

            for (int i = 1; i < 5; i++)
            {
                g.drawLine (midPoint + markerGap * i, height * 0.3, midPoint + markerGap * i, height * 0.7, .7);
                g.drawLine (midPoint - markerGap * i, height * 0.3, midPoint - markerGap * i, height * 0.7, .7);
            }
        }

        //backgrounds
        g.setColour (trackerBgColour);
        g.fillRoundedRectangle (sliderRadius*1.5, height * 0.425, width + sliderRadius*1.1, height * 0.15, height * 0.05); //main rectangle
        
        const float scale = trackerThickness;
        const float ih = (height * scale);
        const float iy = ((height - ih) / 2.f);

        //gradient fill for tracker...
        if (slider.getSliderStyle() == Slider::TwoValueHorizontal)
        {
            g.setColour (trackColour);
            const double minPos = slider.valueToProportionOfLength (slider.getMinValue()) * width;
            const double maxPos = slider.valueToProportionOfLength (slider.getMaxValue()) * width;
//            g.fillRoundedRectangle (minPos + sliderRadius * 1.5, height * 0.425, (maxPos - minPos) + (sliderRadius * .5), height * 0.15, height * 0.05);
            g.fillRoundedRectangle (pow(minPos, 1.01) + sliderRadius * 1.5, height * 0.425, (maxPos - minPos), height * 0.15, height * 0.05);
        }
        else
        {
            //set fill colour for tracker...
            g.setColour (trackColour);

            if (slider.getValue() > 0)
                g.fillRoundedRectangle (zeroPosProportional * width + sliderRadius, iy,
                    sliderPos - sliderRadius * 0.5 - zeroPosProportional * width, ih,
                    5.0f);
            else
                g.fillRoundedRectangle (sliderPos, iy,
                    jmax (0.f, zeroPosProportional * width + sliderRadius - sliderPos), ih,
                    5.0f);

        }
    }
    else //vertical
    {
        height = height - 6;

        // gap markers:
        if (gapMarkers)
        {
            g.setColour (Colours::whitesmoke);
            g.setOpacity (0.6);
            const float midPoint = (height / 2.f + sliderRadius) + 3;
            const float markerGap = height / 9.f;
            g.drawLine (width * 0.25, midPoint, width * 0.75, midPoint, 1.59);
            g.setOpacity (0.3);

            for (int i = 1; i < 5; i++)
            {
                g.drawLine (width * 0.3, midPoint + markerGap * i, width * 0.7, midPoint + markerGap * i, .7);
                g.drawLine (width * 0.3, midPoint - markerGap * i, width * 0.7, midPoint - markerGap * i, .7);
            }
        }

        g.setColour (trackerBgColour);
        g.fillRoundedRectangle (width * 0.425, sliderRadius, width * 0.15, height * 1.005 + sliderRadius * 2.0f - 6.0f, width * 0.05);
        
        const float scale = trackerThickness;
        const float iw = (width * scale);
        const float ix = ((width - iw) / 2.f);

        if (slider.getSliderStyle() == Slider::TwoValueVertical)
        {
            g.setColour (trackColour);
            const float minPos = slider.valueToProportionOfLength (slider.getMinValue()) * height;
            const float maxPos = slider.valueToProportionOfLength (slider.getMaxValue()) * height;
            g.fillRoundedRectangle (width * 0.44, jmax (0.f, height - maxPos) + sliderRadius * 1.5f, width * 0.15, maxPos - minPos, width * 0.05);
        }
        else
        {
            g.setColour (trackColour);

            if (slider.getValue() >= 0)
            {
                const int sliderHeight = jmax (0.f, height - sliderPos + sliderRadius * 2.0f - zeroPosProportional * height);
                g.fillRoundedRectangle (ix, y + sliderPos - sliderRadius/* * 2*/,
                    iw, sliderHeight,
                    3.0f);
            }
            else
                g.fillRoundedRectangle (ix, zeroPosProportional * height + sliderRadius,
                    iw, abs(sliderPos - sliderRadius - zeroPosProportional * height),
                    3.0f);
        }
    }

}


//========== Linear Slider Thumb =========================================================================
void FlatButtonLookAndFeel::drawLinearSliderThumb (Graphics& g, int x, int y, int width, int height,
    float sliderPos, float minSliderPos, float maxSliderPos,
    const Slider::SliderStyle style, Slider& slider)
{
    const float sliderRadius = (float)(getSliderThumbRadius (slider) - 2);
    float sliderWidth, sliderHeight;

    Colour knobColour (LookAndFeelHelpers::createBaseColour (slider.findColour (Slider::thumbColourId),
        slider.hasKeyboardFocus (false) && slider.isEnabled(),
        slider.isMouseOverOrDragging() && slider.isEnabled(),
        slider.isMouseButtonDown() && slider.isEnabled()));

    const float outlineThickness = slider.isEnabled() ? 0.8f : 0.3f;

    if (style == Slider::LinearHorizontal || style == Slider::LinearVertical)
    {
        float kx, ky;

        if (style == Slider::LinearVertical)
        {
            kx = x + width * 0.5f;
            ky = sliderPos;
            sliderWidth = sliderRadius * 2.0f;
            sliderHeight = sliderRadius * 1.25f;
        }
        else
        {
            kx = jmap(sliderPos, (float)x, float(x+width), float(x), (x+width)+sliderRadius);
            ky = y + height * 0.5f;
            sliderWidth = sliderRadius *1.25f;
            sliderHeight = sliderRadius * 2.0f;
        }

        drawThumb (g,
            kx - sliderRadius,
            ky - sliderRadius,
            sliderWidth,
            sliderHeight,
            knobColour, outlineThickness);
    }

    if (style == Slider::TwoValueVertical || style == Slider::ThreeValueVertical)
    {
        const float sr = jmin (sliderRadius, width * 0.4f);
        drawTwoValueThumb (g, jmax (0.0f, x + width * 0.5f - sliderRadius * 2.0f),
            minSliderPos - sliderRadius,
            sliderRadius * 2.0f, knobColour, outlineThickness, 1);

        drawTwoValueThumb (g, jmin (x + width - sliderRadius * 2.0f, x + width * 0.5f), maxSliderPos - sr,
            sliderRadius * 2.0f, knobColour, outlineThickness, 3);
    }
    else if (style == Slider::TwoValueHorizontal || style == Slider::ThreeValueHorizontal)
    {
        const float sr = jmin (sliderRadius, height * 0.4f);
        drawTwoValueThumb (g, minSliderPos - sr,
            jmax (0.0f, y + height * 0.5f - sliderRadius * 2.0f) - height * .01,
            sliderRadius * 2.0f, knobColour, outlineThickness, 2);

        drawTwoValueThumb (g, maxSliderPos - sliderRadius,
            jmin (y + height - sliderRadius * 2.0f, y + height * 0.5f) + height * .01,
            sliderRadius * 2.0f, knobColour, outlineThickness, 4);
    }
    
}

//==========================================================================================================================================
void FlatButtonLookAndFeel::drawThumb (Graphics& g, const float x, const float y,
    const float w, const float h, const Colour& colour,
    const float outlineThickness)
{
    ColourGradient cg = ColourGradient (Colours::white, 0, 0, colour, w / 2, h / 2, false);
    cg.addColour (0.4, Colours::white.overlaidWith (colour));
    g.setGradientFill (cg);
    g.fillRoundedRectangle (x, y, w, h, 3);
}
//====================================================================================================
void FlatButtonLookAndFeel::drawTwoValueThumb (Graphics& g, float x, float y, float diameter,
    const Colour& colour, float outlineThickness, int direction)
{
    if (diameter <= outlineThickness)
        return;

    Path p;

    p.addRoundedRectangle(x, y-3, diameter*.6f, diameter, 3);

    p.applyTransform (AffineTransform::rotation (direction * (float_Pi * 0.5f), x + diameter * 0.5f, y + diameter * 0.5f));

    {
        ColourGradient cg (Colours::white.overlaidWith (colour.withMultipliedAlpha (0.7f)), 0, y,
            Colours::white.overlaidWith (colour.withMultipliedAlpha (0.5f)), 0, y + diameter, false);

        cg.addColour (0.4, Colours::white.overlaidWith (colour));

        g.setGradientFill (cg);
        g.fillPath (p);
    }

    ColourGradient cg (Colours::transparentBlack,
        x + diameter * 0.5f, y + diameter * 0.5f,
        Colours::black.withAlpha (0.5f * outlineThickness * colour.getFloatAlpha()),
        x - diameter * 0.2f, y + diameter * 0.5f, true);

    cg.addColour (0.5, Colours::transparentBlack);
    cg.addColour (0.7, Colours::black.withAlpha (0.07f * outlineThickness));

    g.setGradientFill (cg);
    g.fillPath (p);

    g.setColour (Colours::black.withAlpha (0.5f * colour.getFloatAlpha()));
    g.strokePath (p, PathStrokeType (outlineThickness));
}

//==========================================================================================================================================
void FlatButtonLookAndFeel::drawRotarySlider (Graphics& g, int x, int y, int width, int height, float sliderPos,
    const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider)
{
    const int filmStrip = slider.getProperties().getWithDefault("filmstrip", 0);
    if (filmStrip == 1)
    {
        g.fillAll(Colours::transparentWhite);
    }
    else
    {
        const Rectangle<int> bounds = getSliderLayout(slider).sliderBounds;
        const float radius = jmin(bounds.getWidth() / 2, bounds.getHeight() / 2) - 2.0f;
        const float diameter = radius * 2.f;
        const float centreX = bounds.getX()+ bounds.getWidth() * 0.5f;
        const float centreY = bounds.getY() + bounds.getHeight() * 0.5f;
        const float rx = centreX - radius;
        const float ry = centreY - radius;
        const float rw = radius * 2.0f;
        const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
        const bool isMouseOver = slider.isMouseOverOrDragging() && slider.isEnabled();
        Image image;

        const float innerRadiusProportion = slider.getProperties().getWithDefault("trackerinnerradius", .7);
        const float outerRadiusProportion = slider.getProperties().getWithDefault("trackerouterradius", 1);
        const float thumbThickness = abs(outerRadiusProportion - innerRadiusProportion) / 4.0f / 2.0f;
        const float markerThickness = (float)(slider.getProperties().getWithDefault("markerthickness", 1.0f)) * rw * thumbThickness;
        const float markerStart = slider.getProperties().getWithDefault("markerstart", 0.5);
        const float markerEnd = slider.getProperties().getWithDefault("markerend", 0.9);
        const Colour trackerBgColour = Colour::fromString(slider.getProperties().getWithDefault("trackerbgcolour", Colours::black.toString()).toString());
        const Colour markerColour = Colour::fromString(slider.getProperties().getWithDefault("markercolour", Colours::white.toString()).toString());

        slider.setSliderStyle(Slider::RotaryVerticalDrag);

        //tracker
        g.setColour(slider.findColour(Slider::trackColourId).contrasting(isMouseOver ? 0.1f : 0.0f));


        {
            Path filledArc;
            filledArc.addPieSegment(rx, ry, rw, rw, rotaryStartAngle, angle, innerRadiusProportion);
            filledArc.applyTransform(AffineTransform().scaled(outerRadiusProportion, outerRadiusProportion, bounds.getWidth() / 2.f, bounds.getHeight() / 2.f));
            g.fillPath(filledArc);
        }

        // Draw the slider arc background:
        g.setColour(trackerBgColour);
        Path bgArc;
        bgArc.addPieSegment(rx, ry, rw, rw, angle, rotaryEndAngle, innerRadiusProportion);
        bgArc.applyTransform(AffineTransform().scaled(outerRadiusProportion, outerRadiusProportion, bounds.getWidth() / 2.f, bounds.getHeight() / 2.f));
        g.fillPath(bgArc);

        //outlinecolour
        Colour outlineColour = slider.findColour(Slider::rotarySliderOutlineColourId);
        g.setColour(outlineColour);

        Path outlineArc;
        outlineArc.addPieSegment(rx, ry, rw, rw, rotaryStartAngle, rotaryEndAngle, innerRadiusProportion);
        outlineArc.applyTransform(AffineTransform().scaled(outerRadiusProportion, outerRadiusProportion, bounds.getWidth() / 2.f, bounds.getHeight() / 2.f));
        outlineArc.closeSubPath();

        g.strokePath(outlineArc, PathStrokeType(slider.isEnabled() ? (isMouseOver ? 2.0f : 1.2f) : 0.3f));

        Path newPolygon;
        juce::Point<float> centre(centreX, centreY);

        if (diameter >= 25)   //If diameter is >= 40 then polygon has 12 steps
        {
            newPolygon.addPolygon(centre, 24.f, radius * innerRadiusProportion, 0.f);
            newPolygon.applyTransform(AffineTransform::rotation(angle, centreX, centreY));
        }
        else //Else just use a circle. This is clearer than a polygon when very small.
            newPolygon.addEllipse(-radius * .2, -radius * .2, radius * .3f, radius * .3f);

        Colour thumbColour = slider.findColour(Slider::thumbColourId).withAlpha(isMouseOver ? slider.findColour(Slider::thumbColourId).getFloatAlpha() : slider.findColour(Slider::thumbColourId).getFloatAlpha() * 0.9f);

        g.setColour(thumbColour);
        
        if(diameter >= 25)
            g.fillPath(newPolygon);

        // Draw the thumb segment:
        Path p;
        g.setColour(markerColour.getAlpha() == 0 ? markerColour : markerColour.contrasting(isMouseOver ? 0.1f : 0.0f));
        float thumbLength = radius * innerRadiusProportion * 0.95f;
        p.addLineSegment(Line<float>(0.0f, -thumbLength * markerStart, 0.0f, -thumbLength * markerEnd), markerThickness);
        PathStrokeType(markerThickness, juce::PathStrokeType::JointStyle::curved, juce::PathStrokeType::EndCapStyle::rounded).createStrokedPath(p, p);
        g.fillPath(p, AffineTransform::rotation(angle).translated(centreX, centreY));
    }
}

//===================================================================================
Font FlatButtonLookAndFeel::getTextButtonFont (TextButton&, int buttonHeight)
{
    if(customFont.getHeight()>900)
        return Font(jmin(15.0f, buttonHeight * 0.6f));
    
    customFont.setHeight(jmin(15.0f, buttonHeight * 0.6f));
    return customFont;
    
}

Font FlatButtonLookAndFeel::getComboBoxFont (ComboBox& box)
{
    if(customFont.getHeight()>900)
        return Font(jmin (15.0f, box.getHeight() * 0.85f));
    
    customFont.setHeight(jmin (15.0f, box.getHeight() * 0.85f));
    return customFont;
}

Font FlatButtonLookAndFeel::getLabelFont(Label& label)
{
    if(customFont.getHeight()>900)
        return Font();
    
    return customFont;
    //return CabbageUtilities::getComponentFont();
}

Font FlatButtonLookAndFeel::getSliderPopupFont (Slider&)
{
    if(customFont.getHeight()>900)
    {
        Font font(15.0f);
        font.setBold(true);
        return font;
    }
    customFont.setHeight(15.0f);
    customFont.setBold(true);
    return customFont;// (15.0f, Font::bold);
}



Slider::SliderLayout FlatButtonLookAndFeel::getSliderLayout(Slider& slider)
{
    Slider::SliderLayout layout;

    layout.sliderBounds = slider.getLocalBounds();

    int minXSpace = 0;
    int minYSpace = 0;
    
    Slider::TextEntryBoxPosition textBoxPos = slider.getTextBoxPosition();
    
    if (textBoxPos == Slider::TextBoxLeft || textBoxPos == Slider::TextBoxRight)
        minXSpace = 30;
    else
        minYSpace = 15;
    
    Rectangle<int> localBounds = slider.getLocalBounds();
    
    const int textBoxWidth = jmax(0, jmin(slider.getTextBoxWidth(), localBounds.getWidth() - minXSpace));
    const int textBoxHeight = jmax(0, jmin(slider.getTextBoxHeight(), localBounds.getHeight() - minYSpace));
    
    
    if (textBoxPos != Slider::NoTextBox)
    {
        if (slider.isBar())
        {
            layout.textBoxBounds = localBounds;
        }
        else
        {
            layout.textBoxBounds.setWidth(textBoxWidth);
            layout.textBoxBounds.setHeight(textBoxHeight);
            
            if (textBoxPos == Slider::TextBoxLeft)           layout.textBoxBounds.setX(0);
            else if (textBoxPos == Slider::TextBoxRight)     layout.textBoxBounds.setX(localBounds.getWidth() - textBoxWidth);
            else /* above or below -> centre horizontally */ layout.textBoxBounds.setX((localBounds.getWidth() - textBoxWidth) / 2);
            
            if (textBoxPos == Slider::TextBoxAbove)          layout.textBoxBounds.setY(0);
            else if (textBoxPos == Slider::TextBoxBelow)     layout.textBoxBounds.setY(localBounds.getHeight() - textBoxHeight);
            else /* left or right -> centre vertically */    layout.textBoxBounds.setY((localBounds.getHeight() - textBoxHeight) / 2);
        }
    }
    
    layout.sliderBounds = localBounds;
    
    if (slider.isBar())
    {
        layout.sliderBounds.reduce(1, 1);   // bar border
    }
    else
    {
        if (textBoxPos == Slider::TextBoxLeft)       layout.sliderBounds.removeFromLeft(textBoxWidth);
        else if (textBoxPos == Slider::TextBoxRight) layout.sliderBounds.removeFromRight(textBoxWidth);
        else if (textBoxPos == Slider::TextBoxAbove) layout.sliderBounds.removeFromTop(textBoxHeight);
        else if (textBoxPos == Slider::TextBoxBelow) layout.sliderBounds.removeFromBottom(textBoxHeight);
        
        const int thumbIndent = getSliderThumbRadius(slider);
        
        if (slider.isHorizontal())    layout.sliderBounds.reduce(thumbIndent, 0);
        else if (slider.isVertical()) layout.sliderBounds.reduce(0, thumbIndent);
    }
    
    var bounds = slider.getProperties().getWithDefault(CabbageIdentifierIds::valuetextboxbounds, var());
    if(bounds.isArray())
    {
        layout.textBoxBounds = Rectangle<int>(bounds[0], bounds[1], bounds[2], bounds[3]);
    }
    var sliderBounds = slider.getProperties().getWithDefault(CabbageIdentifierIds::sliderbounds, var());
    if(sliderBounds.isArray())
    {
        layout.sliderBounds = Rectangle<int>(sliderBounds[0], sliderBounds[1], sliderBounds[2], sliderBounds[3]);
    }
    
    return layout;
    
}

