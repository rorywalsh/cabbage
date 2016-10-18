/*
  ==============================================================================

    CabbageMainDocumentWindow.h
    Created: 11 Oct 2016 12:43:50pm
    Author:  rory

  ==============================================================================
*/
#ifndef CABBAGEMAINWINDOW_H_INCLUDED
#define CABBAGEMAINWINDOW_H_INCLUDED
#include "CabbageMainComponent.h"

#include "../Plugin/StandaloneFilterWindow.h"

class CabbageSettings;

//==============================================================================
/*
    This class implements the desktop window that contains an instance of
    our MainContentComponent class.
*/
class CabbageMainDocumentWindow    : public DocumentWindow
{	
public:

	class PluginWindow :public DocumentWindow
	{
		public:
			PluginWindow(String title, Colour backgroundColour):DocumentWindow(title, backgroundColour, DocumentWindow::minimiseButton | DocumentWindow::closeButton){}
			~PluginWindow(){}
		
	};



    CabbageMainDocumentWindow (String name, CabbageSettings* settings);
	~CabbageMainDocumentWindow()
	{

	}
	
	void closeButtonPressed() override
	{
		JUCEApplicationBase::quit();
	}
	
	MainContentComponent* getMainContentComponent();
	//ScopedPointer<StandalonePluginHolder> pluginHolder;
	ScopedPointer<MainContentComponent> mainContentComponent;
	ScopedPointer<StandaloneFilterWindow> pluginWindow;


private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabbageMainDocumentWindow)
};

#endif