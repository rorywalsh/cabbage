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

class CabbageLookAndFeel2	: public LookAndFeel_V3
{
public:
    CabbageLookAndFeel2();
    ~CabbageLookAndFeel2() {};
	
	void drawToggleButton (Graphics &g, ToggleButton &button, bool isMouseOverButton, bool isButtonDown);
    Image drawToggleImage (float width, float height, bool isToggleOn, const Colour colour, const bool isRect, const float corners);
	void drawFromSVG(Graphics &g, String svgString, int width, int height, int newWidth, int newHeight, AffineTransform affine);

private:




};

#endif  // CABBAGELOOKANDFEEL2_H_INCLUDED
