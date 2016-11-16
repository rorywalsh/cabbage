/*
  Copyright (C) 2016 Rory Walsh

  Cabbage is free software; you can redistribute it
  and/or modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  Cabbage is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with Csound; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
  02111-1307 USA
*/

#include "CabbageContentComponent.h"
#include "CabbageDocumentWindow.h"

//==============================================================================
CabbageContentComponent::CabbageContentComponent(ValueTree settings): settings(settings)
{
	addAndMakeVisible(propertyPanel = new CabbagePropertiesPanel(settings));
	propertyPanel->setVisible(false);
 	setSize (1200, 800);
	bgImage = createBackground();	
}

CabbageContentComponent::~CabbageContentComponent()
{
    editorAndConsole.clear();
}

Image CabbageContentComponent::createBackground()
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

void CabbageContentComponent::openFile(File file)
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

void CabbageContentComponent::addFileTabButton(File file, int xPos)
{
	TextButton* fileButton;
	fileTabs.add(fileButton = new TextButton(file.getFileName()));
	addAndMakeVisible(fileButton);
	fileButton->setBounds(xPos, 3, 90, 20);	
	fileButton->addListener(this);
	fileButton->setRadioGroupId(99);
	fileButton->setClickingTogglesState(true);
	fileButton->setLookAndFeel(&lookAndFeel);
	fileButton->setToggleState(true, sendNotification);
	currentFileIndex = fileTabs.size()-1;
}

EditorAndConsoleContentComponent* CabbageContentComponent::getCurrentCodeEditor()
{	
	return editorAndConsole[currentFileIndex];
}

void CabbageContentComponent::buttonClicked(Button* button)
{
	if(const TextButton* textButton = dynamic_cast<TextButton*>(button))
	{
		currentFileIndex = fileTabs.indexOf(textButton);
		editorAndConsole[currentFileIndex]->toFront(true);
		if(CabbageDocumentWindow* docWindow = this->findParentComponentOfClass<CabbageDocumentWindow>())
		{
			docWindow->setName(openFiles[currentFileIndex].getFileName());
			docWindow->setCurrentCsdFile(openFiles[currentFileIndex]);
		}
	}	
}

void CabbageContentComponent::paint (Graphics& g)
{	
	if(editorAndConsole.size()==0)
		g.drawImage(bgImage, getLocalBounds().toFloat());
		
	else
		g.fillAll( CabbageSettings::getColourFromValueTree(settings, CabbageColourIds::lineNumberBackground, Colour(50,50,50)));

}

void CabbageContentComponent::resizeAllEditorAndConsoles(int height)
{
	const bool isPropPanelVisible = propertyPanel->isVisible();
	for( EditorAndConsoleContentComponent* editor : editorAndConsole )
		editor->setBounds(0, height, getWidth() - (isPropPanelVisible ? 200 : 0), getHeight());
}

void CabbageContentComponent::resized()
{
	const int heightOfTabButtons = (editorAndConsole.size()>1) ? 25 : 0; 	
	
	if(propertyPanel->isVisible())
	{
		propertyPanel->setBounds(getWidth()-200, 0, 200, getHeight());
	} 		
	
	resizeAllEditorAndConsoles(heightOfTabButtons);
}