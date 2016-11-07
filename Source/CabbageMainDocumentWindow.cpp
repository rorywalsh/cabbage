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
	EditorAndConsoleContentComponent* editorConsole;
	editorAndConsole.add(editorConsole = new EditorAndConsoleContentComponent(settings));
	addAndMakeVisible(editorConsole);
	addAndMakeVisible(propertyPanel = new CabbagePropertiesPanel(settings));
	propertyPanel->setVisible(false);
	editorConsole->setVisible(true);
	editorConsole->toFront(true);
	openFiles.add(file);
	editorConsole->editor->loadContent(file.loadFileAsString());
	numberOfFiles = editorAndConsole.size()-1;
	CabbageUtilities::debug("Number of open files", editorAndConsole.size());
	resized();
	
	if(numberOfFiles==1)
	{
		addFileTabButton(openFiles[0], 10);
		addFileTabButton(openFiles[1], 105);
	}
	else if(numberOfFiles>1)
	{
		addFileTabButton(openFiles[numberOfFiles], 10+numberOfFiles*95);
	}
}

void MainContentComponent::addFileTabButton(File file, int xPos)
{
	TextButton* fileButton;
	fileTabs.add(fileButton = new TextButton(file.getFileName()));
	addAndMakeVisible(fileButton);
	fileButton->setBounds(xPos, 3, 90, 20);	
	fileButton->addListener(this);
	fileButton->setRadioGroupId(99);
	fileButton->setClickingTogglesState(true);
	fileButton->setToggleState(true, sendNotification);
	currentFileIndex = fileTabs.size()-1;
}

EditorAndConsoleContentComponent* MainContentComponent::getCurrentCodeEditor()
{	
	return editorAndConsole[currentFileIndex];
}

void MainContentComponent::buttonClicked(Button* button)
{
	if(const TextButton* textButton = dynamic_cast<TextButton*>(button))
	{
		currentFileIndex = fileTabs.indexOf(textButton);
		editorAndConsole[currentFileIndex]->toFront(true);
		if(CabbageMainDocumentWindow* docWindow = this->findParentComponentOfClass<CabbageMainDocumentWindow>())
			docWindow->setName(openFiles[currentFileIndex].getFileName());
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

EditorAndConsoleContentComponent* CabbageMainDocumentWindow::getCurrentCodeEditor()
{
	return mainContentComponent->getCurrentCodeEditor();
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

