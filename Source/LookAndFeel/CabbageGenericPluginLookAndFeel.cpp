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

#include "CabbageGenericPluginLookAndFeel.h"

//Cabbage IDE look and feel class
CabbageGenericPluginLookAndFeel::CabbageGenericPluginLookAndFeel()
{
    setColour (Slider::thumbColourId, Colours::green);
    setColour (Label::ColourIds::textColourId, Colour (30, 30, 30));
}

void CabbageGenericPluginLookAndFeel::refreshLookAndFeel (ValueTree valueTree)
{

}

void CabbageGenericPluginLookAndFeel::drawLinearSliderThumb (Graphics& g, int x, int y, int width, int height, float sliderPos,
                                                             float minSliderPos,
                                                             float maxSliderPos,
                                                             const Slider::SliderStyle style,
                                                             Slider& slider)
{

    const float sliderRadius = (float) (getSliderThumbRadius (slider) - 2);

    Colour knobColour (0, 255, 0);

    const float outlineThickness = slider.isEnabled() ? 0.8f : 0.3f;

    if (style == Slider::LinearHorizontal || style == Slider::LinearVertical)
    {
        float kx, ky;

        if (style == Slider::LinearVertical)
        {
            kx = x + width * 0.5f;
            ky = sliderPos;
        }
        else
        {
            kx = sliderPos;
            ky = y + height * 0.5f;
        }

        drawThumb (g,
                   kx - sliderRadius,
                   ky - sliderRadius,
                   sliderRadius * 2.0f,
                   knobColour, outlineThickness);
    }
    else
    {
        if (style == Slider::ThreeValueVertical)
        {
            drawGlassSphere (g, x + width * 0.5f - sliderRadius,
                             sliderPos - sliderRadius,
                             sliderRadius * 2.0f,
                             knobColour, outlineThickness);
        }
        else if (style == Slider::ThreeValueHorizontal)
        {
            drawGlassSphere (g, sliderPos - sliderRadius,
                             y + height * 0.5f - sliderRadius,
                             sliderRadius * 2.0f,
                             knobColour, outlineThickness);
        }

        if (style == Slider::TwoValueVertical || style == Slider::ThreeValueVertical)
        {
            const float sr = jmin (sliderRadius, width * 0.4f);

            drawGlassPointer (g, jmax (0.0f, x + width * 0.5f - sliderRadius * 2.0f),
                              minSliderPos - sliderRadius,
                              sliderRadius * 2.0f, knobColour, outlineThickness, 1);

            drawGlassPointer (g, jmin (x + width - sliderRadius * 2.0f, x + width * 0.5f), maxSliderPos - sr,
                              sliderRadius * 2.0f, knobColour, outlineThickness, 3);
        }
        else if (style == Slider::TwoValueHorizontal || style == Slider::ThreeValueHorizontal)
        {
            const float sr = jmin (sliderRadius, height * 0.4f);

            drawGlassPointer (g, minSliderPos - sr,
                              jmax (0.0f, y + height * 0.5f - sliderRadius * 2.0f),
                              sliderRadius * 2.0f, knobColour, outlineThickness, 2);

            drawGlassPointer (g, maxSliderPos - sliderRadius,
                              jmin (y + height - sliderRadius * 2.0f, y + height * 0.5f),
                              sliderRadius * 2.0f, knobColour, outlineThickness, 4);
        }
    }

}

void CabbageGenericPluginLookAndFeel::drawThumb (Graphics& g, const float x, const float y,
                                                 const float diameter, const Colour& colour,
                                                 const float outlineThickness) noexcept
{
    if (diameter <= outlineThickness)
        return;

    g.setColour ((Colours::black.withAlpha (.5f)));
    g.fillRoundedRectangle (x + 3, y - 4, diameter, diameter * 1.7 + 1, 8);
    //58, 110, 182
    g.setColour (Colour (58, 110, 182));
    g.fillRoundedRectangle (x, y - 6, diameter, diameter * 1.7, 8);
    g.setColour (Colour (58, 110, 182).darker());
    g.drawRoundedRectangle (x, y - 5, diameter, diameter * 1.7, 8.f, 2.f);
}