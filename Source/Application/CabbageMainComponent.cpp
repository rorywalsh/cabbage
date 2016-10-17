/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "CabbageMainComponent.h"
#include "../BinaryData/CabbageBinaryData.h"


//==============================================================================
MainContentComponent::MainContentComponent(ValueTree settings)
{
    addAndMakeVisible(editor = new CabbageCodeEditorComponent(settings, csoundDocument, &csoundTokeniser));
    addAndMakeVisible(outputConsole = new CabbageOutputConsoleComponent(settings));
    editor->setLineNumbersShown(true);
    editor->setFont(Font(14));
    editor->setVisible(false);
	outputConsole->setVisible(false);
	
	setSize (1200, 800);
	bgImage = createBackground();
	
}

MainContentComponent::~MainContentComponent()
{
    editor = nullptr;
    outputConsole = nullptr;
}

Image MainContentComponent::createBackground()
{
	Image backgroundImg;
	backgroundImg = Image(Image::RGB, getWidth(), getHeight(), true);
	ScopedPointer<Drawable> drawable;
	const int whiteScale = 150;
	Graphics g(backgroundImg);
    {
		g.fillAll(Colour(whiteScale,whiteScale,whiteScale));
		Random pos, width, colour;
		for(int i=0;i<getWidth();)
		{
			const int brightness = colour.nextInt(Range<int>(whiteScale, whiteScale+5));
			g.setColour(Colour(brightness, brightness, brightness));
			g.drawLine(i, 0, i+width.nextInt(Range<int>(0, 10)), getHeight() );
			g.drawLine(0, i, getWidth(), i+width.nextInt(Range<int>(0, 10)));
			i += pos.nextInt(Range<int>(0, 5));
		}
		
		const Image cabbageLogo = ImageCache::getFromMemory (CabbageBinaryData::CabbageLogoBig_png, CabbageBinaryData::CabbageLogoBig_pngSize);
		g.drawImage(cabbageLogo, getLocalBounds().toFloat(), RectanglePlacement::Flags::doNotResize);
		return backgroundImg;
	}
}

void MainContentComponent::openFile(File file)
{
	editor->setVisible(true);
	outputConsole->setVisible(true);
	editor->loadContent(file.loadFileAsString());
}

void MainContentComponent::paint (Graphics& g)
{
	g.drawImage(bgImage, getLocalBounds().toFloat());
}

void MainContentComponent::resized()
{
    Rectangle<int> area (getLocalBounds());
    outputConsole->setBounds(area.removeFromBottom(getHeight()*.15));
    editor->setBounds(area);
}
