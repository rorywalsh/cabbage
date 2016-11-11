/*
  ==============================================================================

    CabbageLookAndFeel2.cpp
    Created: 10 Nov 2016 5:36:11pm
    Author:  rory

  ==============================================================================
*/

#include "CabbageLookAndFeel2.h"

//Cabbage IDE look and feel class
CabbageLookAndFeel2::CabbageLookAndFeel2()
{

}

//===========================================================================================
// methods for drawing toggle buttons / checkboxes
void CabbageLookAndFeel2::drawToggleButton (Graphics &g, ToggleButton &button, bool isMouseOverButton, bool isButtonDown)
{
	Image image;
	const String imgButtonOn = button.getProperties().getWithDefault(CabbageIdentifierIds::imgbuttonon, "").toString();
	const String imgButtonOff = button.getProperties().getWithDefault(CabbageIdentifierIds::imgbuttonoff, "").toString();
	const int corners = button.getProperties().getWithDefault(CabbageIdentifierIds::corners, 2.f);
	const bool isRectangle = button.getProperties().getWithDefault(CabbageIdentifierIds::shape, false);
    float fontSize = jmin (15.0f, button.getHeight() * 0.85f);
    const float tickWidth = button.getHeight() * .9f;	
	

    if (button.hasKeyboardFocus (true))
    {
        g.setColour (button.findColour (TextEditor::focusedOutlineColourId));
        g.drawRect (0, 0, button.getWidth(), button.getHeight());
    }
	
	if(button.getToggleState())
	{		
		if(File::getCurrentWorkingDirectory().getChildFile(imgButtonOn).existsAsFile())
		{
			image = ImageCache::getFromFile(File(imgButtonOn));
			image = image.rescaled(button.getWidth(), button.getHeight());
		}
		else
		{
			image = drawToggleImage(tickWidth, button.getHeight(), button.getToggleState(), 
									button.findColour(TextButton::ColourIds::buttonOnColourId), isRectangle, corners);
		}			
	}
	else
	{		
		if(File::getCurrentWorkingDirectory().getChildFile(imgButtonOff).existsAsFile())
		{
			if(imgButtonOff.containsIgnoreCase(".svg"))
			{
				const int corners = button.getProperties().getWithDefault(CabbageIdentifierIds::corners, 2.f);
				//image = drawFromSVG(imgButtonOff, imgWidth, imgHeight, AffineTransform::identity);				
			
			}
			else
			{
				image = ImageCache::getFromFile(File(imgButtonOff));
				image = image.rescaled(button.getWidth(), button.getHeight());		
			}
		}
		else
		{
			image = drawToggleImage(tickWidth, button.getHeight(), button.getToggleState(), 
									button.findColour(TextButton::ColourIds::buttonColourId), isRectangle, corners);
		}
	}

	g.drawImage(image, 4.0f, (button.getHeight() - tickWidth) * 0.5f,
				button.getWidth()-4, tickWidth, 0, 0, button.getWidth(), button.getHeight(), false);	

	g.setColour (button.getToggleState() ==true ? button.findColour(TextButton::textColourOnId) : button.findColour(TextButton::textColourOffId));	
    g.setFont (fontSize);

    if (! button.isEnabled())
        g.setOpacity (0.5f);

    const int textX = (int) tickWidth + 10;

    g.drawFittedText (button.getButtonText(),
                      textX, 0,
                      button.getWidth() - textX - 2, button.getHeight(),
                      Justification::centredLeft, 10);	
	
}

Image CabbageLookAndFeel2::drawToggleImage (float width, float height, bool isToggleOn, const Colour colour, const bool isRectangle, const float corners)
{
	Image img = Image(Image::ARGB, width, height, true);
	Graphics g (img);
	float opacity = 0;

	if (isRectangle) 
	{
		if (isToggleOn == true)
		{
			g.setColour (colour);
			g.fillRoundedRectangle(width*0.01, height*0.01, width*0.93, height*0.93, corners);
			opacity = 0.4;
		}
		else
		{
			for (float i=0.01; i<0.05; i+=0.01)
			{
				g.setColour (Colour::fromRGBA (0, 0, 0, 255/(i*100)));
				g.fillRoundedRectangle (width*i+1, height*i+1,
										width*0.95, height*0.95, corners);
			}

			Colour bg1 = colour.darker();
			Colour bg2 = colour;
			ColourGradient cg = ColourGradient (bg1, 0, 0, bg2, width*0.5, height*0.5, false);
			g.setGradientFill (cg);
			g.fillRoundedRectangle (width*0.01, height*0.01, width*0.93, height*0.93, corners);
			opacity = 0.2;
		}

		// For emphasising the top and left edges to give the illusion that light is shining on them
		ColourGradient edgeHighlight = ColourGradient (Colours::whitesmoke, 0, 0,
									   Colours::transparentWhite, 0, height*0.1, false);
		g.setGradientFill (edgeHighlight);
		g.setOpacity (opacity);
		g.fillRoundedRectangle (0, 0, width*0.95, height*0.95, corners);

		ColourGradient edgeHighlight2 = ColourGradient (Colours::whitesmoke, 0, 0,
										Colours::transparentWhite, height*0.1, 0, false);
		g.setGradientFill (edgeHighlight2);
		g.setOpacity (opacity);
		g.fillRoundedRectangle (0, 0, width*0.95, height*0.95, corners);
	}
	else   //else if round toggle
	{
		ColourGradient base = ColourGradient (Colours::white, width*-0.3, height*-0.3, Colours::black,
											  width*0.8, height*0.8, false);
		g.setGradientFill(base);
		g.fillEllipse (0, 0, width, height);
		g.setColour(Colours::black);
		g.fillEllipse(width*0.09, height*0.09, width*0.82, height*0.82);
		Colour outline = Colour::fromRGB(70, 70, 70);
		g.setColour(outline.withAlpha(colour.getAlpha()));
		g.fillEllipse(width*0.04, height*0.04, width*0.92, height*0.92);

		if (isToggleOn)   //on
		{
			g.setColour(colour);
			g.fillEllipse(width*0.09, height*0.09, width*0.82, height*0.82);
		}
		else   //off
		{
			g.setColour(Colours::black);
			g.fillEllipse(width*0.09, height*0.09, width*0.82, height*0.82);

			Colour bg1 = Colour::fromRGBA (25, 25, 28, 255);
			Colour bg2 = Colour::fromRGBA (15, 15, 18, 255);
			ColourGradient cg = ColourGradient (bg1, 0, 0, bg2, width*0.5, height*0.5, false);
			g.setGradientFill (cg);
			g.setOpacity(0.4);
			g.fillEllipse(width*0.1, height*0.1, width*0.8, height*0.8);
		}
	}
	return img;
}

Image CabbageLookAndFeel2::drawFromSVG(String svgString, int width, int height, AffineTransform affine)
{
	Image svgImg;
	svgImg = Image(Image::ARGB, width, height, true);
	ScopedPointer<XmlElement> svg (XmlDocument::parse(svgString));
	if(svg == nullptr)
		return Image::null;

	ScopedPointer<Drawable> drawable;

	Graphics graph(svgImg);
	if (svg != nullptr)
	{
		drawable = Drawable::createFromSVG (*svg);
		drawable->draw(graph, 1.f, affine);
		return svgImg;
	}
	return Image::null;
}