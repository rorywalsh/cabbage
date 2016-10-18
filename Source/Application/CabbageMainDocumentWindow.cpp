/*
  ==============================================================================

    CabbageMainWindow.cpp
    Created: 11 Oct 2016 12:26:56pm
    Author:  rory

  ==============================================================================
*/

#include "CabbageMainDocumentWindow.h"
#include "CabbageSettings.h"

	


CabbageMainDocumentWindow::CabbageMainDocumentWindow (String name, CabbageSettings* settings)  : DocumentWindow(name,
            Colours::lightgrey,
            DocumentWindow::allButtons)
{
    setUsingNativeTitleBar (true);
    setContentOwned (mainContentComponent = new MainContentComponent(settings->getValueTree()), true);
    this->setResizable(true, true);
    centreWithSize (getWidth(), getHeight());
    setVisible (true);

	
	pluginWindow = new StandaloneFilterWindow("CsoundPlugin",
			Colours::black,
			settings->getUserSettings(),
			false);
	
	pluginWindow->setVisible(true);
	//pluginHolder = new CabbagePluginHolder(settings->getUserSettings(), false);
	
}

MainContentComponent* CabbageMainDocumentWindow::getMainContentComponent()
{
	return mainContentComponent;
}
