/*
  ==============================================================================

    CabbageGenericPluginLookAndFeel.h
    Created: 4 Nov 2016 7:02:54pm
    Author:  rory

  ==============================================================================
*/

#ifndef CABBAGEGENERICPLUGINLOOKANDFEEL_H_INCLUDED
#define CABBAGEGENERICPLUGINLOOKANDFEEL_H_INCLUDED


#ifndef CABBAGEIDELOOKANDFEEL_H_INCLUDED
#define CABBAGEIDELOOKANDFEEL_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "Settings/CabbageSettings.h"
#include "CabbageIds.h"
#include "Utilities/CabbageUtilities.h"
#include "BinaryData/CabbageBinaryData.h"


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
