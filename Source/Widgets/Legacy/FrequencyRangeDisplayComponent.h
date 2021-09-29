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

#ifndef FREQUENCYRANGEDISPLAYCOMPONENT_H_INCLUDED
#define FREQUENCYRANGEDISPLAYCOMPONENT_H_INCLUDED

#include "../../CabbageCommonHeaders.h"
#include "../CabbageWidgetBase.h"

class FrequencyRangeDisplayComponent : public Component
{
    Colour fontColour, backgroundColour;
    int minFreq, maxFreq, resolution;

public:
    FrequencyRangeDisplayComponent (Colour fColour, Colour bgColour) :
        Component(),
        fontColour (fColour),
        backgroundColour (bgColour),
        minFreq (0),
        maxFreq (22050),
        resolution (10)
    {}

    ~FrequencyRangeDisplayComponent() override {}

    void resized() override
    {
        if (getWidth() < 400)
            resolution = resolution / 2;
    }

    void setResolution (int res)
    {
        resolution = res;

        if (getWidth() < 400)
            resolution = resolution / 3;

    }

    void paint (Graphics& g)  override
    {
        g.fillAll (backgroundColour);
        g.setColour (fontColour);

        for (int i = 0; i < resolution; i++)
        {
            const int width = getWidth() / resolution;
            int freq = jmap (i, 0, resolution, minFreq, maxFreq);

            String freqStr = String (freq);

            if (freqStr.length() > 4)
                freqStr = freqStr.substring (0, 2) + "." + freqStr.substring (2, 3) + "kHz";
            else if (freqStr.length() > 3)
                freqStr = freqStr.substring (0, 1) + "." + freqStr.substring (1, 2) + "kHz";
            else
                freqStr = freqStr + "Hz";

            g.drawFittedText (String (freqStr), i * width, 4, 35, 7, Justification::left, 1);
        }
    }

    void setMinMax (int min, int max)
    {
        minFreq = min;
        maxFreq = max;
    }
};



#endif  // FREQUENCYRANGEDISPLAYCOMPONENT_H_INCLUDED
