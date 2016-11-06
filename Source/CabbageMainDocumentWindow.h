/*
  ==============================================================================

    CabbageMainDocumentWindow.h
    Created: 11 Oct 2016 12:43:50pm
    Author:  rory

  ==============================================================================
*/
#ifndef CABBAGEMAINWINDOW_H_INCLUDED
#define CABBAGEMAINWINDOW_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "CodeEditor/EditorAndConsoleContentComponent.h"
#include "BinaryData/CabbageBinaryData.h"
#include "InterfaceEditor/CabbagePropertiesPanel.h"
#include "CabbageIds.h"

class CabbageSettings;



//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public Component, public Button::Listener
{
public:

    //==============================================================================
    MainContentComponent(ValueTree settings);
    ~MainContentComponent();

    void paint (Graphics&) override;
    void resized() override;
	Image createBackground();	
	void openFile(File file);
    OwnedArray<EditorAndConsoleContentComponent> editorAndConsole;
	void resizeAllEditorAndConsoles(int height);
	ScopedPointer<CabbagePropertiesPanel> propertyPanel;
	OwnedArray<TextButton> fileTabs;
	Array<File> openFiles;
	void buttonClicked(Button* button);
	void addFileTabButton(File file, int xPos);
	
private:
	Image bgImage;
	const int statusBarHeight = 25;
	ValueTree settings;
	int currentFileIndex = 0;
	
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


//==============================================================================
/*
    This class implements the desktop window that contains an instance of
    our MainContentComponent class.
*/
class CabbageMainDocumentWindow    : public DocumentWindow
{	
public:
    CabbageMainDocumentWindow (String name, CabbageSettings* settings);
	~CabbageMainDocumentWindow()
	{
		lookAndFeel = nullptr;
	}
	
	void closeButtonPressed() override
	{
		JUCEApplicationBase::quit();
	}
	
	void updateEditorColourScheme();
	
	MainContentComponent* getMainContentComponent();
	ScopedPointer<MainContentComponent> mainContentComponent;
	ValueTree cabbageSettings;
	ScopedPointer<LookAndFeel_V2> lookAndFeel;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabbageMainDocumentWindow)
};

#endif