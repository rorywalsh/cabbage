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
MainContentComponent::MainContentComponent(ValueTree settings): settings(settings)
{
	//editorAndConsole.add(new EditorAndConsoleContentComponent(settings));
	currentFileIndex++;
    //addAndMakeVisible(editorAndConsole[0]);
	addAndMakeVisible(propertyPanel = new CabbagePropertiesPanel(settings));
	propertyPanel->setVisible(false);
 	setSize (1200, 800);
	bgImage = createBackground();	
}

MainContentComponent::~MainContentComponent()
{
    editorAndConsole.clear();
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
	editorAndConsole.add(new EditorAndConsoleContentComponent(settings));
	addAndMakeVisible(editorAndConsole[editorAndConsole.size()-1]);
	addAndMakeVisible(propertyPanel = new CabbagePropertiesPanel(settings));
	propertyPanel->setVisible(false);
	editorAndConsole[editorAndConsole.size()-1]->setVisible(true);
	openFiles.add(file);
	editorAndConsole[editorAndConsole.size()-1]->editor->loadContent(file.loadFileAsString());
	currentFileIndex = editorAndConsole.size()-1;
	CabbageUtilities::debug("Number of open files", editorAndConsole.size());
	resized();
	
	if(currentFileIndex==1)
	{
		addFileTabButton(openFiles[0], 10);
		addFileTabButton(openFiles[1], 105);
	}
	else if(currentFileIndex>1)
	{
		addFileTabButton(openFiles[currentFileIndex], 10+currentFileIndex*95);
	}
}

void MainContentComponent::addFileTabButton(File file, int xPos)
{
	TextButton* fileButton;
	fileTabs.add(fileButton = new TextButton(file.getFileName()));
	addAndMakeVisible(fileButton);
	fileButton->setBounds(xPos, 3, 90, 20);	
	fileButton->addListener(this);
}

void MainContentComponent::buttonClicked(Button* button)
{
	if(const TextButton* textButton = dynamic_cast<TextButton*>(button))
	{
		const int fileIndex = fileTabs.indexOf(textButton);
		editorAndConsole[fileIndex]->toFront(true);
		if(CabbageMainDocumentWindow* docWindow = this->findParentComponentOfClass<CabbageMainDocumentWindow>())
			docWindow->setName(openFiles[fileIndex].getFileName());
	}	
}

void MainContentComponent::paint (Graphics& g)
{	
	if(editorAndConsole.size()==0)
		g.drawImage(bgImage, getLocalBounds().toFloat());
		
	else
		g.fillAll( CabbageSettings::getColourFromValueTree(settings, CabbageColourIds::lineNumberBackground, Colour(50,50,50)));

}

void MainContentComponent::resizeAllEditorAndConsoles(int height)
{
	const bool isPropPanelVisible = propertyPanel->isVisible();
	for( EditorAndConsoleContentComponent* editor : editorAndConsole )
		editor->setBounds(0, height, getWidth() - (isPropPanelVisible ? 200 : 0), getHeight());
}

void MainContentComponent::resized()
{
	const int heightOfTabButtons = (editorAndConsole.size()>1) ? 25 : 0; 	
	
	if(propertyPanel->isVisible())
	{
		propertyPanel->setBounds(getWidth()-200, 0, 200, getHeight());
	} 		
	
	resizeAllEditorAndConsoles(heightOfTabButtons);
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
	int editorIndex = mainContentComponent->editorAndConsole.size()-1;
	mainContentComponent->editorAndConsole[editorIndex]->updateLookAndFeel();
}

