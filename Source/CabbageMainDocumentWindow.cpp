/*
  ==============================================================================

    CabbageMainWindow.cpp
    Created: 11 Oct 2016 12:26:56pm
    Author:  rory

  ==============================================================================
*/

#include "CabbageMainDocumentWindow.h"
#include "./Settings/CabbageSettings.h"



	
//==============================================================================
MainContentComponent::MainContentComponent(ValueTree settings)
{
    addAndMakeVisible(editorAndConsole = new EditorAndConsoleContentComponent(settings));
	addAndMakeVisible(propertyPanel = new CabbagePropertiesPanel(settings));
	propertyPanel->setVisible(false);
 	setSize (1200, 800);
	bgImage = createBackground();	
}

MainContentComponent::~MainContentComponent()
{
    editorAndConsole = nullptr;
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
	editorAndConsole->setVisible(true);
	editorAndConsole->editor->loadContent(file.loadFileAsString());
}

void MainContentComponent::paint (Graphics& g)
{	
	if(!editorAndConsole->isVisible())
		g.drawImage(bgImage, getLocalBounds().toFloat());
	else
		g.fillAll(Colours::transparentBlack);
}

void MainContentComponent::resized()
{
	if(propertyPanel->isVisible())
	{
		propertyPanel->setBounds(getWidth()-200, 0, 200, getHeight());
		editorAndConsole->setBounds(0, 0, getWidth()-propertyPanel->getWidth(), getHeight());
	} 
	else
		editorAndConsole->setBounds(0, 0, getWidth(), getHeight());
}

//=================================================================================================================
CabbageMainDocumentWindow::CabbageMainDocumentWindow (String name, CabbageSettings* settings)  : DocumentWindow(name,
            Colours::lightgrey,
            DocumentWindow::allButtons),
			cabbageSettings(settings->getValueTree())
{
	lookAndFeel = new LookAndFeel_V2();
    setUsingNativeTitleBar (true);
    setContentOwned (mainContentComponent = new MainContentComponent(cabbageSettings), true);
    this->setResizable(true, true);
    centreWithSize (getWidth(), getHeight());
    setVisible (true);	
	setLookAndFeel(lookAndFeel);
}

MainContentComponent* CabbageMainDocumentWindow::getMainContentComponent()
{
	return mainContentComponent;
}

void CabbageMainDocumentWindow::updateEditorColourScheme()
{
	this->getLookAndFeel().setColour(PropertyComponent::ColourIds::backgroundColourId, CabbageSettings::getColourFromValueTree(cabbageSettings, CabbageColourIds::propertyLabelBackground, Colour(50,50,50)));
	this->getLookAndFeel().setColour(PropertyComponent::ColourIds::labelTextColourId, CabbageSettings::getColourFromValueTree(cabbageSettings, CabbageColourIds::propertyLabelText, Colour(50,50,50)));
	this->lookAndFeelChanged();
	mainContentComponent->propertyPanel->setBackgroundColour(CabbageSettings::getColourFromValueTree(cabbageSettings, CabbageColourIds::propertyPanelBackground, Colour(50,50,50)));
	mainContentComponent->propertyPanel->setBorderColour(CabbageSettings::getColourFromValueTree(cabbageSettings, CabbageColourIds::consoleOutline, Colour(50,50,50)));
	mainContentComponent->editorAndConsole->editor->updateColourScheme();
	mainContentComponent->editorAndConsole->outputConsole->updateColourScheme();
	mainContentComponent->editorAndConsole->horizontalResizerBar.repaint();	
	mainContentComponent->editorAndConsole->statusBar.repaint();	
}
