/*
  ==============================================================================

    EditorAndConsoleContentComponent.cpp
    Created: 30 Oct 2016 1:42:59pm
    Author:  rory

  ==============================================================================
*/

#include "EditorAndConsoleContentComponent.h"

EditorAndConsoleContentComponent::EditorAndConsoleContentComponent(ValueTree settings)
: horizontalResizerBar(this, settings), settings(settings),
statusBar(settings)
{
	
	addAndMakeVisible(horizontalResizerBar);
	addAndMakeVisible(statusBar);
	addAndMakeVisible(editor = new CabbageCodeEditorComponent(&statusBar, settings, csoundDocument, &csoundTokeniser));
	addAndMakeVisible(outputConsole = new CabbageOutputConsole(settings));
	editor->setLineNumbersShown(true);
	editor->setFont(Font(String("DejaVu Sans Mono"), 17, 0));
	editor->setVisible(true);
	outputConsole->setVisible(true);
	horizontalResizerBar.setVisible(true);	
	setSize(1000, 800);	
	const int width = settings.getChildWithName("General").getProperty("LastKnownWidth");	
	const int height = settings.getChildWithName("General").getProperty("LastKnownHeight");	
	horizontalBarPosition = settings.getChildWithName("General").getProperty("HorizontalBarLastPosition");
	editor->setBounds(0, 0, getWidth(), 500);	
	horizontalResizerBar.setBounds(0, 500, getWidth(), getHeight()*.01);			
}

EditorAndConsoleContentComponent::~EditorAndConsoleContentComponent()
{
	editor = nullptr;
	outputConsole = nullptr;
}
void EditorAndConsoleContentComponent::openFile(File file)
{
	editor->setVisible(true);
	outputConsole->setVisible(true);
	horizontalResizerBar.setVisible(true);
	editor->loadContent(file.loadFileAsString());
}

void EditorAndConsoleContentComponent::resized()
{
	editor->setBounds(0, 0, getWidth(), horizontalResizerBar.getY());
	const int consoleY = horizontalResizerBar.getY()+horizontalResizerBar.getHeight();
	const int consoleHeight = getHeight()-(consoleY+statusBarHeight+5);
	outputConsole->setBounds(0, consoleY, getWidth(), consoleHeight);
	statusBar.setBounds(0, getHeight()-30, getWidth(), 30); 			
}

void EditorAndConsoleContentComponent::updateEditorColourScheme()
{
	editor->updateColourScheme();
	outputConsole->updateColourScheme();
	horizontalResizerBar.repaint();	
	statusBar.repaint();	
}