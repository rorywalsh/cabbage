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

#ifndef JUCE_STANDALONEFILTERWINDOW_H_INCLUDED
#define JUCE_STANDALONEFILTERWINDOW_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "CsoundPluginEditor.h"
#include "CsoundPluginProcessor.h"
#include "../Application/CabbageSettings.h"


extern AudioProcessor* JUCE_CALLTYPE createPluginFilter();

//==============================================================================
/**
    An object that creates and plays a standalone instance of an AudioProcessor.

    The object will create your processor using the same createPluginFilter()
    function that the other plugin wrappers use, and will run it through the
    computer's audio/MIDI devices using AudioDeviceManager and AudioProcessorPlayer.
*/
class StandalonePluginHolder
   #if JUCE_IOS || JUCE_ANDROID
    , private Timer
   #endif
   
{
public:

		// This creates new instances of the plugin..
	AudioProcessor* JUCE_CALLTYPE createPluginFilter()
	{
		return new CsoundAudioProcessor();
	}

    /** Creates an instance of the default plugin.

        The settings object can be a PropertySet that the class should use to store its
        settings - the takeOwnershipOfSettings indicates whether this object will delete
        the settings automatically when no longer needed. The settings can also be nullptr.

        A default device name can be passed in.

        Preferably a complete setup options object can be used, which takes precedence over
        the preferredDefaultDeviceName and allows you to select the input & output device names,
        sample rate, buffer size etc.

        In all instances, the settingsToUse will take precedence over the "preferred" options if not null.
    */
    StandalonePluginHolder (ValueTree settingsToUse,
                            bool takeOwnershipOfSettings = true,
                            const String& preferredDefaultDeviceName = String(),
                            const AudioDeviceManager::AudioDeviceSetup* preferredSetupOptions = nullptr)

        : settings (settingsToUse)
    {
        createPlugin();
        setupAudioDevices (preferredDefaultDeviceName, preferredSetupOptions);
        startPlaying();

    }

    virtual ~StandalonePluginHolder()
    {
       #if JUCE_IOS || JUCE_ANDROID
        stopTimer();
       #endif

        deletePlugin();
        shutDownAudioDevices();
    }

    //==============================================================================
    virtual void createPlugin()
    {

      #if JUCE_MODULE_AVAILABLE_juce_audio_plugin_client
        processor = ::createPluginFilterOfType (AudioProcessor::wrapperType_Standalone);
      #else
        AudioProcessor::setTypeOfNextNewPlugin (AudioProcessor::wrapperType_Standalone);
        processor = createPluginFilter();
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

    //==============================================================================
    void startPlaying()
    {
        player.setProcessor (processor);
    }

    void stopPlaying()
    {
        player.setProcessor (nullptr);
    }

    //==============================================================================
    /** Shows an audio properties dialog box modally. */
    void showAudioSettingsDialog()
    {
        DialogWindow::LaunchOptions o;
        o.content.setOwned (new AudioDeviceSelectorComponent (deviceManager,
                                                              processor->getTotalNumInputChannels(),
                                                              processor->getTotalNumInputChannels(),
                                                              processor->getTotalNumOutputChannels(),
                                                              processor->getTotalNumOutputChannels(),
                                                              true, false,
                                                              true, false));
        o.content->setSize (500, 450);

        o.dialogTitle                   = TRANS("Audio Settings");
        o.dialogBackgroundColour        = Colour (0xfff0f0f0);
        o.escapeKeyTriggersCloseButton  = true;
        o.useNativeTitleBar             = true;
        o.resizable                     = false;

        o.launchAsync();
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
	

	void setAudioSettingsXMLStartup(String xmlSettingsString)
	{
		xmlSettings = xmlSettingsString;
	}

    void reloadAudioDeviceState (const String& preferredDefaultDeviceName,
                                 const AudioDeviceManager::AudioDeviceSetup* preferredSetupOptions)
    {
        ScopedPointer<XmlElement> savedState;
		
		String xmlAudioState = CabbageSettings::get(settings, "AudioSettings", "audioSetup");

		savedState = XmlDocument::parse(xmlAudioState);//settings->getXmlValue ("audioSetup");

        deviceManager.initialise (processor->getTotalNumInputChannels(),
                                  processor->getTotalNumOutputChannels(),
                                  savedState,
                                  true,
                                  preferredDefaultDeviceName,
                                  preferredSetupOptions);
    }


	String getDeviceManagerSettings()
	{
		ScopedPointer<XmlElement> xml (deviceManager.createStateXml());
		return xml->createDocument("");
	}
	
    //==============================================================================
    ValueTree settings;
    ScopedPointer<AudioProcessor> processor;
    AudioDeviceManager deviceManager;
    AudioProcessorPlayer player;
	String xmlSettings;

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

   #if JUCE_IOS || JUCE_ANDROID
    void timerCallback() override
    {
        StringArray midiInputDevices = MidiInput::getDevices();
        if (midiInputDevices != lastMidiDevices)
        {
            {
                const int n = lastMidiDevices.size();
                for (int i = 0; i < n; ++i)
                {
                    const String& oldDevice = lastMidiDevices[i];

                    if (! midiInputDevices.contains (oldDevice))
                    {
                        deviceManager.setMidiInputEnabled (oldDevice, false);
                        deviceManager.removeMidiInputCallback (oldDevice, &player);
                    }
                }
            }

            {
                const int n = midiInputDevices.size();
                for (int i = 0; i < n; ++i)
                {
                    const String& newDevice = midiInputDevices[i];

                    if (! lastMidiDevices.contains (newDevice))
                    {
                        deviceManager.addMidiInputCallback (newDevice, &player);
                        deviceManager.setMidiInputEnabled (newDevice, true);
                    }
                }
            }
        }
    }
   #endif

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StandalonePluginHolder)
};


//==============================================================================
class StandaloneFilterWindow    : public DocumentWindow,
                                  public ButtonListener   // (can't use Button::Listener due to VC2005 bug)
{
public:
    //==============================================================================
    StandaloneFilterWindow (const String& title,
                            Colour backgroundColour,
                            ValueTree settings,
                            bool takeOwnershipOfSettings,
                            const String& preferredDefaultDeviceName = String(),
                            const AudioDeviceManager::AudioDeviceSetup* preferredSetupOptions = nullptr)
        : DocumentWindow (title, backgroundColour, DocumentWindow::minimiseButton | DocumentWindow::closeButton),
          optionsButton ("options")
    {
        setTitleBarButtonsRequired (DocumentWindow::minimiseButton | DocumentWindow::closeButton, false);

        Component::addAndMakeVisible (optionsButton);
        optionsButton.addListener (this);
        optionsButton.setTriggeredOnMouseDown (true);

        pluginHolder = new StandalonePluginHolder (settings, takeOwnershipOfSettings,
                                                   preferredDefaultDeviceName, preferredSetupOptions);

        createEditorComp();

		const int x = CabbageSettings::get(settings, "Misc", "windowX").getIntValue();
		const int y = CabbageSettings::get(settings, "Misc", "windowY").getIntValue();


            if (x != -100 && y != -100)
                setBoundsConstrained (juce::Rectangle<int> (x, y, getWidth(), getHeight()));
            else
                centreWithSize (getWidth(), getHeight());
    }

    ~StandaloneFilterWindow()
    {
		CabbageSettings::set(pluginHolder->settings, "Misc", "windowX", getX());
		CabbageSettings::set(pluginHolder->settings, "Misc", "windowY", getY());


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

    void deleteEditorComp()
    {
        if (AudioProcessorEditor* ed = dynamic_cast<AudioProcessorEditor*> (getContentComponent()))
        {
            pluginHolder->processor->editorBeingDeleted (ed);
            clearContentComponent();
        }
    }

    /** Deletes and re-creates the plugin, resetting it to its default state. */
    void resetToDefaultState()
    {
        pluginHolder->stopPlaying();
        deleteEditorComp();
        pluginHolder->deletePlugin();

        pluginHolder->createPlugin();
        createEditorComp();
        pluginHolder->startPlaying();
    }

    //==============================================================================
    void closeButtonPressed() override
    {
        JUCEApplicationBase::quit();
    }

    void buttonClicked (Button*) override
    {
        PopupMenu m;
        m.addItem (1, TRANS("Audio Settings..."));
        m.addSeparator();
        m.addItem (2, TRANS("Save current state..."));
        m.addItem (3, TRANS("Load a saved state..."));
        m.addSeparator();
        m.addItem (4, TRANS("Reset to default state"));

        m.showMenuAsync (PopupMenu::Options(),
                         ModalCallbackFunction::forComponent (menuCallback, this));
    }

    void handleMenuResult (int result)
    {
        switch (result)
        {
            case 1:  pluginHolder->showAudioSettingsDialog(); break;
            case 4:  resetToDefaultState(); break;
            default: break;
        }
    }

	void showAudioSettings()
	{
		pluginHolder->showAudioSettingsDialog();
	}

    static void menuCallback (int result, StandaloneFilterWindow* button)
    {
        if (button != nullptr && result != 0)
            button->handleMenuResult (result);
    }

    void resized() override
    {
        DocumentWindow::resized();
        optionsButton.setBounds (8, 6, 60, getTitleBarHeight() - 8);
    }

    ScopedPointer<StandalonePluginHolder> pluginHolder;

private:
    //==============================================================================
    TextButton optionsButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StandaloneFilterWindow)
};


#endif   // JUCE_STANDALONEFILTERWINDOW_H_INCLUDED
