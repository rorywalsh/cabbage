/*
  ==============================================================================

    CabbageMainWindow.cpp
    Created: 11 Oct 2016 12:26:56pm
    Author:  rory

  ==============================================================================
*/

#include "CabbageMainDocumentWindow.h"



CabbageMainDocumentWindow::CabbageMainDocumentWindow (String name, ValueTree settings)  : DocumentWindow(name,
            Colours::lightgrey,
            DocumentWindow::allButtons)
{
    setUsingNativeTitleBar (true);
    setContentOwned (mainContentComponent = new MainContentComponent(settings), true);
    this->setResizable(true, true);
    centreWithSize (getWidth(), getHeight());
    setVisible (true);
}

MainContentComponent* CabbageMainDocumentWindow::getMainContentComponent()
{
	return mainContentComponent;
}
