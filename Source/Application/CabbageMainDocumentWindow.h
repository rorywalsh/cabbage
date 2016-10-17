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
#include "CabbagePluginHolder.h"

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
		pluginHolder->stopPlaying();
		deleteEditorComp();
		pluginHolder = nullptr;
	}

    void closeButtonPressed() override
    {
        JUCEApplication::getInstance()->systemRequestedQuit();
    }
	
	AudioProcessor* getAudioProcessor() const noexcept { return pluginHolder->processor; }
	AudioDeviceManager& getDeviceManager() const noexcept { return pluginHolder->deviceManager; }

	void showAudioSettings()
	{
		pluginHolder->showAudioSettingsDialog();
	}
	
	void createEditorComp()
	{
		pluginWindow->setContentOwned(getAudioProcessor()->createEditorIfNeeded(), true);
	}

	void deleteEditorComp()
	{
		if (AudioProcessorEditor* ed = dynamic_cast<AudioProcessorEditor*> (getContentComponent()))
		{
			pluginHolder->processor->editorBeingDeleted(ed);
			clearContentComponent();
		}
	}

	/** Deletes and re-creates the plugin, resetting it to its default state. */
	void resetToDefaultState()
	{
		pluginHolder->stopPlaying();
		deleteEditorComp();
		pluginHolder->deletePlugin();

		if (PropertySet* props = pluginHolder->settings)
			props->removeValue("filterState");

		pluginHolder->createPlugin();
		createEditorComp();
		pluginHolder->startPlaying();
	}
	
	
	MainContentComponent* getMainContentComponent();
	ScopedPointer<CabbagePluginHolder> pluginHolder;
	ScopedPointer<MainContentComponent> mainContentComponent;
	ScopedPointer<PluginWindow> pluginWindow;


private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabbageMainDocumentWindow)
};

#endif