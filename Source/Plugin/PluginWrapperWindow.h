/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2015 - ROLI Ltd.

   Permission is granted to use this software under the terms of either:
   a) the GPL v2 (or any later version)
   b) the Affero GPL v3

   Details of these licenses can be found at: www.gnu.org/licenses

   JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

   ------------------------------------------------------------------------------

   To release a closed-source product which uses JUCE, commercial licenses are
   available: visit www.juce.com for more information.

  ==============================================================================
*/

#ifndef JUCE_PluginWrapperWindow_H_INCLUDED
#define JUCE_PluginWrapperWindow_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "CsoundPluginEditor.h"
#include "CsoundPluginProcessor.h"
#include "GenericCabbagePluginProcessor.h"
#include "../Application/Settings/CabbageSettings.h"

class PluginWrapper
{
public:
	// This creates new instances of the plugin..
	GenericCabbagePluginProcessor* JUCE_CALLTYPE createCsoundPluginFilter(File inputFile)
	{
		return new GenericCabbagePluginProcessor(inputFile);
	}


    PluginWrapper (PropertySet* settingsToUse, File inputFile,
                            bool takeOwnershipOfSettings = true,
                            const String& preferredDefaultDeviceName = String(),
                            const AudioDeviceManager::AudioDeviceSetup* preferredSetupOptions = nullptr)

        : settings (settingsToUse, takeOwnershipOfSettings)
    {
        createPlugin(inputFile);
        setupAudioDevices (preferredDefaultDeviceName, preferredSetupOptions);
        reloadPluginState();
        startPlaying();

    }

    virtual ~PluginWrapper()
    {
        deletePlugin();
        shutDownAudioDevices();
    }

    //==============================================================================
    virtual void createPlugin(File inputFile)
    {

      #if JUCE_MODULE_AVAILABLE_juce_audio_plugin_client
        processor = ::createPluginFilterOfType (AudioProcessor::wrapperType_Standalone);
      #else
        AudioProcessor::setTypeOfNextNewPlugin (AudioProcessor::wrapperType_Standalone);
        processor = createCsoundPluginFilter(inputFile);	
        AudioProcessor::setTypeOfNextNewPlugin (AudioProcessor::wrapperType_Undefined);
      #endif
        jassert (processor != nullptr); // Your createPluginFilter() function must return a valid object!

        processor->disableNonMainBuses();
        processor->setRateAndBufferSizeDetails(44100, 512);
    }

    virtual void deletePlugin()
    {
        stopPlaying();
        processor = nullptr;
    }

    static String getFilePatterns (const String& fileSuffix)
    {
        if (fileSuffix.isEmpty())
            return String();

        return (fileSuffix.startsWithChar ('.') ? "*" : "*.") + fileSuffix;
    }

	void setXmlAudioSettings(XmlElement* xmlSettingsString)
	{
		xmlSettings = xmlSettingsString;
		setupAudioDevices ( String(), nullptr);
		startPlaying();
	}

	AudioDeviceSelectorComponent* getAudioDeviceSelector()
	{
		return new AudioDeviceSelectorComponent (deviceManager,
															  processor->getTotalNumInputChannels(),
															  processor->getTotalNumInputChannels(),
															  processor->getTotalNumOutputChannels(),
															  processor->getTotalNumOutputChannels(),
															  true, false,
															  true, false);
	}

	String getDeviceManagerSettings()
	{
		if(deviceManager.getCurrentAudioDevice())
		{
			ScopedPointer<XmlElement> xml (deviceManager.createStateXml());
			return xml->createDocument("");
		}
		else return String::empty;
	}

    //==============================================================================
    void startPlaying()
    {
        player.setProcessor (processor);
    }

    void stopPlaying()
    {
        player.setProcessor (nullptr);
    }

    void reloadAudioDeviceState (const String& preferredDefaultDeviceName,
                                 const AudioDeviceManager::AudioDeviceSetup* preferredSetupOptions)
    {
        ScopedPointer<XmlElement> savedState;

        if (settings != nullptr)
            savedState = settings->getXmlValue ("audioSetup");

        deviceManager.initialise (processor->getTotalNumInputChannels(),
                                  processor->getTotalNumOutputChannels(),
                                  savedState,
                                  true,
                                  preferredDefaultDeviceName,
                                  preferredSetupOptions);
    }

    //==============================================================================
	String getCsoundOutput()
	{
		return dynamic_cast<GenericCabbagePluginProcessor*>(getProcessor())->getCsoundOutput();
	}

    //==============================================================================
    void savePluginState()
    {
        if (settings != nullptr && processor != nullptr)
        {
            MemoryBlock data;
            processor->getStateInformation (data);

            settings->setValue ("filterState", data.toBase64Encoding());
        }
    }

    void reloadPluginState()
    {
        if (settings != nullptr)
        {
            MemoryBlock data;

            if (data.fromBase64Encoding (settings->getValue ("filterState")) && data.getSize() > 0)
                processor->setStateInformation (data.getData(), (int) data.getSize());
        }
    }

	AudioProcessor* getProcessor()
	{
		return processor;
	}

    //==============================================================================
    OptionalScopedPointer<PropertySet> settings;
    ScopedPointer<AudioProcessor> processor;
    AudioDeviceManager deviceManager;
    AudioProcessorPlayer player;
	ScopedPointer<XmlElement> xmlSettings;

   #if JUCE_IOS || JUCE_ANDROID
    StringArray lastMidiDevices;
   #endif

private:
    void setupAudioDevices (const String& preferredDefaultDeviceName,
                            const AudioDeviceManager::AudioDeviceSetup* preferredSetupOptions)
    {
        deviceManager.addAudioCallback (&player);
        deviceManager.addMidiInputCallback (String(), &player);

        reloadAudioDeviceState (preferredDefaultDeviceName, preferredSetupOptions);
    }

    void shutDownAudioDevices()
    {
        deviceManager.removeMidiInputCallback (String(), &player);
        deviceManager.removeAudioCallback (&player);
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginWrapper)
};


//==============================================================================
/**
    A class that can be used to run a simple standalone application containing your filter.

    Just create one of these objects in your JUCEApplicationBase::initialise() method, and
    let it do its work. It will create your filter object using the same createPluginFilter() function
    that the other plugin wrappers use.
*/
class PluginWrapperWindow    : public DocumentWindow
{
public:
 
    PluginWrapperWindow (const String& title, File inputFile,
                            Colour backgroundColour,
                            PropertySet* settingsToUse,
                            bool takeOwnershipOfSettings,
                            const String& preferredDefaultDeviceName = String(),
                            const AudioDeviceManager::AudioDeviceSetup* preferredSetupOptions = nullptr)
        : DocumentWindow (title, backgroundColour, DocumentWindow::minimiseButton | DocumentWindow::closeButton)
    {
        setTitleBarButtonsRequired (DocumentWindow::minimiseButton | DocumentWindow::closeButton, false);

        pluginHolder = new PluginWrapper (settingsToUse, inputFile, takeOwnershipOfSettings,
                                                   preferredDefaultDeviceName, preferredSetupOptions);
												   
		

        createEditorComp();

        if (PropertySet* props = pluginHolder->settings)
        {
            const int x = props->getIntValue ("windowX", -100);
            const int y = props->getIntValue ("windowY", -100);

            if (x != -100 && y != -100)
                setBoundsConstrained (juce::Rectangle<int> (x, y, getWidth(), getHeight()));
            else
                centreWithSize (getWidth(), getHeight());
        }
        else
        {
            centreWithSize (getWidth(), getHeight());
        }
    }

    ~PluginWrapperWindow()
    {
        if (PropertySet* props = pluginHolder->settings)
        {
            //props->setValue ("windowX", getX());
            //props->setValue ("windowY", getY());
        }

        pluginHolder->stopPlaying();
        deleteEditorComp();
        pluginHolder = nullptr;
    }

    //==============================================================================
    AudioProcessor* getAudioProcessor() const noexcept      { return pluginHolder->processor; }
    AudioDeviceManager& getDeviceManager() const noexcept   { return pluginHolder->deviceManager; }

    void createEditorComp()
    {
        setContentOwned (getAudioProcessor()->createEditorIfNeeded(), true);
    }

	int getNumberOfParameters()
	{
		return pluginHolder->getProcessor()->getParameters().size();
	}

    void deleteEditorComp()
    {
        if (AudioProcessorEditor* ed = dynamic_cast<AudioProcessorEditor*> (getContentComponent()))
        {
            pluginHolder->processor->editorBeingDeleted (ed);
            clearContentComponent();
        }
    }

    /** Deletes and re-creates the plugin, resetting it to its default state. */
    void resetToDefaultState(File inputFile)
    {
        pluginHolder->stopPlaying();
        deleteEditorComp();
        pluginHolder->deletePlugin();

        pluginHolder->createPlugin(inputFile);
        createEditorComp();
        pluginHolder->startPlaying();
    }

    //==============================================================================
    void closeButtonPressed() override
    {
        JUCEApplicationBase::quit();
    }

    void resized() override
    {
        DocumentWindow::resized();
        optionsButton.setBounds (8, 6, 60, getTitleBarHeight() - 8);
    }

	PluginWrapper* getPluginWrapper()
	{
		return pluginHolder;
	}

    ScopedPointer<PluginWrapper> pluginHolder;

private:
    //==============================================================================
    TextButton optionsButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginWrapperWindow)
};


#endif   // JUCE_PluginWrapperWindow_H_INCLUDED
