/*
  Copyright (C) 2017 Rory Walsh

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

#include "FileTab.h"

FileTab::FileTab (String name, String filename):
        TextButton (name, filename),
        csdFile (filename),
        play ("Play", DrawableButton::ButtonStyle::ImageStretched),
        close ("", DrawableButton::ButtonStyle::ImageStretched),
        showEditor ("", DrawableButton::ButtonStyle::ImageStretched),
        editGUI ("", DrawableButton::ButtonStyle::ImageStretched),
        overlay()
{
	
	addChildComponent (overlay);
	overlay.setVisible (false);
	play.setClickingTogglesState (true);
	play.setName ("playButton");

	addAndMakeVisible (close);
	close.setName ("closeButton");
	close.setColour (DrawableButton::ColourIds::backgroundColourId, Colours::transparentBlack);
	close.setColour (DrawableButton::ColourIds::backgroundOnColourId, Colours::transparentBlack);
	close.setTooltip ("Close file");
	close.getProperties().set ("filename", csdFile.getFullPathName());

	addAndMakeVisible (play);
	play.setColour (DrawableButton::ColourIds::backgroundColourId, Colours::transparentBlack);
	play.setColour (DrawableButton::ColourIds::backgroundOnColourId, Colours::transparentBlack);
	play.setClickingTogglesState (true);
	play.getProperties().set ("filename", csdFile.getFullPathName());

	addAndMakeVisible (showEditor);
	showEditor.setName ("showEditorButton");
	showEditor.setColour (DrawableButton::ColourIds::backgroundColourId, Colours::transparentBlack);
	showEditor.setColour (DrawableButton::ColourIds::backgroundOnColourId, Colours::transparentBlack);
	showEditor.setClickingTogglesState (true);
	showEditor.setTooltip ("Show plugin Editor");

	addAndMakeVisible (editGUI);
	editGUI.setName ("editGUIButton");
	editGUI.setColour (DrawableButton::ColourIds::backgroundColourId, Colours::transparentBlack);
	editGUI.setColour (DrawableButton::ColourIds::backgroundOnColourId, Colours::transparentBlack);
	editGUI.setClickingTogglesState (true);
	editGUI.setTooltip ("Edit Plugin GUI");

	setDrawableImages (play, 60, 25, "play");
	setDrawableImages (close, 25, 25, "close");
	setDrawableImages (showEditor, 25, 25, "showEditor");
	setDrawableImages (editGUI, 25, 25, "editGUI"); 
}

void FileTab::drawButtonShape (Graphics& g, const Path& outline, Colour baseColour, float height)
{
	const float mainBrightness = baseColour.getBrightness();
	const float mainAlpha = baseColour.getFloatAlpha();

	g.setGradientFill (ColourGradient (baseColour.brighter (0.2f), 0.0f, 0.0f,
									   baseColour.darker (0.25f), 0.0f, height, false));
	g.fillPath (outline);

	g.setColour (Colours::white.withAlpha (0.4f * mainAlpha * mainBrightness * mainBrightness));
	g.strokePath (outline, PathStrokeType (1.0f), AffineTransform::translation (0.0f, 1.0f)
														.scaled (1.0f, (height - 1.6f) / height));

	g.setColour (Colours::black.withAlpha (0.4f * mainAlpha));
	g.strokePath (outline, PathStrokeType (1.0f));
}


void FileTab::drawButtonText (Graphics& g)
{
	Font font(jmin (15.0f, getHeight() * 0.6f));
    g.setFont (font);
    g.setColour (findColour (getToggleState() ? TextButton::textColourOnId
                                                            : TextButton::textColourOnId)
                       .darker (getToggleState() ? 0.f : 0.5f));

    const int yIndent = jmin (4, proportionOfHeight (0.3f));
    const int cornerSize = jmin (getHeight(), getWidth()) / 2;

    const int fontHeight = roundToInt (font.getHeight() * 0.6f);
    const int leftIndent  = 120;
    const int rightIndent = jmin (fontHeight, 2 + cornerSize / (isConnectedOnRight() ? 4 : 2));
    const int textWidth = getWidth() - leftIndent - rightIndent;

    if (textWidth > 0)
        g.drawFittedText (getButtonText(),
                          leftIndent, yIndent, textWidth, getHeight() - yIndent * 2,
                          Justification::centred, 2);
}
						  
void FileTab::paintButton(Graphics &g, bool isMouseOverButton, bool isButtonDown)
{
	const Colour backgroundColour = Colour(100, 100, 100);
	
	if(isEnabled()==false)
		jassert(false);
	
	Colour baseColour (getToggleState() ? backgroundColour : Colour(30, 30, 30));

	if (isButtonDown || isMouseOverButton)
		baseColour = baseColour.contrasting (isButtonDown ? 0.2f : 0.1f);

	const bool flatOnLeft   = isConnectedOnLeft();
	const bool flatOnRight  = isConnectedOnRight();
	const bool flatOnTop    = isConnectedOnTop();
	const bool flatOnBottom = isConnectedOnBottom();

	const float width  = getWidth() - 1.0f;
	const float height = getHeight() - 1.0f;

	if (width > 0 && height > 0)
	{
		const float cornerSize = 4.0f;

		Path outline;
		outline.addRoundedRectangle (0.5f, 0.5f, width, height, cornerSize, cornerSize,
									 ! (flatOnLeft  || flatOnTop),
									 ! (flatOnRight || flatOnTop),
									 ! (flatOnLeft  || flatOnBottom),
									 ! (flatOnRight || flatOnBottom));

		drawButtonShape (g, outline, baseColour, height);
	}
	
	drawButtonText(g);
}

void FileTab::addButtonListeners (Button::Listener* listener)
{
	play.addListener (listener);
	close.addListener (listener);
	showEditor.addListener (listener);
	editGUI.addListener (listener);
}

void FileTab::disableButtons (bool disable)
{
	if (disable)
	{
		overlay.setVisible (true);
		overlay.toFront (true);
	}
	else
		overlay.setVisible (false);

}

void FileTab::resized()
{
	overlay.setBounds (5, 3, 125, 25);
	play.setBounds (5, 3, 60, 25);
	showEditor.setBounds (67, 3, 30, 25);
	editGUI.setBounds (99, 3, 30, 25);
	close.setBounds (getWidth() - 22, 3, 20, 20);
}

void FileTab::setDrawableImages (DrawableButton& button, int width, int height, String type)
{
	DrawableImage imageNormal, imageNormalPressed, imageDownPressed;

	if (type == "play")
	{
		DrawableImage imageDown;
		imageNormalPressed.setImage (CabbageImages::drawPlayStopIcon (width, height, false, true));
		imageDownPressed.setImage (CabbageImages::drawPlayStopIcon (width, height, true, true));
		imageNormal.setImage (CabbageImages::drawPlayStopIcon (width, height, false));
		imageDown.setImage (CabbageImages::drawPlayStopIcon (width, height, true));
		button.setImages (&imageNormal, &imageNormal, &imageNormalPressed, &imageNormal, &imageDown, nullptr,  &imageDownPressed, &imageDownPressed);
	}
	else if (type == "close")
	{
		imageNormal.setImage (CabbageImages::drawCloseIcon (width, height));
		imageNormalPressed.setImage (CabbageImages::drawCloseIcon (width - 3, height - 3));
		button.setImages (&imageNormal, &imageNormal, &imageNormalPressed, &imageNormal, &imageNormal);
	}
	else if (type == "showEditor")
	{
		imageNormal.setImage (CabbageImages::drawEditorIcon (width, height));
		imageNormalPressed.setImage (CabbageImages::drawEditorIcon (width - 1, height - 1));
		button.setImages (&imageNormal, &imageNormal, &imageNormalPressed, &imageNormal, &imageNormal, nullptr,  &imageNormalPressed, &imageNormalPressed);
	}
	else if (type == "editGUI")
	{
		imageNormal.setImage (CabbageImages::drawEditGUIIcon (width, height));
		imageNormalPressed.setImage (CabbageImages::drawEditGUIIcon (width - 1, height - 1));
		button.setImages (&imageNormal, &imageNormal, &imageNormalPressed, &imageNormal, &imageNormal, nullptr,  &imageNormalPressed, &imageNormalPressed);
	}
}
