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
#include "CabbageCodeEditor.h"
#include "CabbageOutputConsole.h"
#include "../BinaryData/CabbageBinaryData.h"

class CabbageSettings;


//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public Component
{
public:
    //==============================================================================
    MainContentComponent(ValueTree settings);
    ~MainContentComponent();

    void paint (Graphics&) override;
    void resized() override;
	Image createBackground();
	
	void openFile(File file);

    ScopedPointer<CabbageCodeEditorComponent> editor;
    ScopedPointer<CabbageOutputConsole> outputConsole;
    CodeDocument csoundDocument;
    CsoundTokeniser csoundTokeniser;

private:
	Image bgImage;
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

	}
	
	void closeButtonPressed() override
	{
		JUCEApplicationBase::quit();
	}
	
	MainContentComponent* getMainContentComponent();
	ScopedPointer<MainContentComponent> mainContentComponent;



private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabbageMainDocumentWindow)
};

#endif