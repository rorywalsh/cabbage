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
#ifndef CABBAGEGENERICPLUGINLOOKANDFEEL_H_INCLUDED
#define CABBAGEGENERICPLUGINLOOKANDFEEL_H_INCLUDED


#ifndef CABBAGEIDELOOKANDFEEL_H_INCLUDED
#define CABBAGEIDELOOKANDFEEL_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "../Settings/CabbageSettings.h"
#include "../CabbageIds.h"
#include "../Utilities/CabbageUtilities.h"
#include "../BinaryData/CabbageBinaryData.h"


class CabbageGenericPluginLookAndFeel	: public LookAndFeel_V3
{
public:
    CabbageGenericPluginLookAndFeel();
    ~CabbageGenericPluginLookAndFeel() {};

    void drawLinearSliderThumb (Graphics &g, int x, int y, int width, int height, float sliderPos,
                                float minSliderPos,
                                float maxSliderPos,
                                const Slider::SliderStyle style,
                                Slider &slider) override;

    void drawThumb (Graphics& g, const float x, const float y,
                    const float diameter, const Colour& colour,
                    const float outlineThickness) noexcept;

    void refreshLookAndFeel(ValueTree valueTree);


private:
    ValueTree colourTree;



};


#endif  // CABBAGEIDELOOKANDFEEL_H_INCLUDED



#endif  // CABBAGEGENERICPLUGINLOOKANDFEEL_H_INCLUDED
