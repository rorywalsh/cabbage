
#pragma once

#if !Cabbage_IDE_Build

#include "../CabbageIds.h"
#include "../CabbageCommonHeaders.h"
#include "../LookAndFeel/CabbageLookAndFeel2.h"

//==============================================================================
/**
    An object that creates and plays a standalone instance of an AudioProcessor.

    The object will create your processor using the same createPluginFilter()
    function that the other plugin wrappers use, and will run it through the
    computer's audio/MIDI devices using AudioDeviceManager and AudioProcessorPlayer.

    @tags{Audio}
*/

class StandalonePluginHolder    : private AudioIODeviceCallback,
                                  private Timer,
                                  private Value::Listener
{
public:
    //==============================================================================
    /** Structure used for the number of inputs and outputs. */
    struct PluginInOuts   { short numIns, numOuts; };

    //==============================================================================
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
    StandalonePluginHolder (PropertySet* settingsToUse,
                            bool takeOwnershipOfSettings = true,
                            const String& preferredDefaultDeviceName = String(),
                            const AudioDeviceManager::AudioDeviceSetup* preferredSetupOptions = nullptr,
                            const Array<PluginInOuts>& channels = Array<PluginInOuts>(),
                           #if JUCE_ANDROID || JUCE_IOS
                            bool shouldAutoOpenMidiDevices = true
                           #else
                            bool shouldAutoOpenMidiDevices = false
                           #endif
                            )

        : settings (settingsToUse, takeOwnershipOfSettings),
          channelConfiguration (channels),
          autoOpenMidiDevices (shouldAutoOpenMidiDevices)
    {
        shouldMuteInput.addListener (this);
        shouldMuteInput = ! isInterAppAudioConnected();

        createPlugin();

        auto inChannels = (channelConfiguration.size() > 0 ? channelConfiguration[0].numIns
                                                           : processor->getMainBusNumInputChannels());

        if (preferredSetupOptions != nullptr)
            options.reset (new AudioDeviceManager::AudioDeviceSetup (*preferredSetupOptions));

        auto audioInputRequired = (inChannels > 0);

        if (audioInputRequired && RuntimePermissions::isRequired (RuntimePermissions::recordAudio)
            && ! RuntimePermissions::isGranted (RuntimePermissions::recordAudio))
            RuntimePermissions::request (RuntimePermissions::recordAudio,
                                         [this, preferredDefaultDeviceName] (bool granted) { init (granted, preferredDefaultDeviceName); });
        else
            init (audioInputRequired, preferredDefaultDeviceName);
    }

    void init (bool enableAudioInput, const String& preferredDefaultDeviceName)
    {
        setupAudioDevices (enableAudioInput, preferredDefaultDeviceName, options.get());
        //reloadPluginState();
        startPlaying();

       if (autoOpenMidiDevices)
           startTimer (500);
    }

    ~StandalonePluginHolder() override
    {
        stopTimer();
        deletePlugin();
        shutDownAudioDevices();
    }

    //==============================================================================
    virtual void createPlugin()
    {
        processor.reset (createPluginFilterOfType (AudioProcessor::wrapperType_Standalone));
        processor->disableNonMainBuses();
        processor->setRateAndBufferSizeDetails (44100, 512);

        processorHasPotentialFeedbackLoop = (getNumInputChannels() > 0 && getNumOutputChannels() > 0);
    }

    virtual void deletePlugin()
    {
        stopPlaying();
        processor = nullptr;
    }

    int getNumInputChannels() const
    {
        if (processor == nullptr)
            return 0;

        return (channelConfiguration.size() > 0 ? channelConfiguration[0].numIns
                                                : processor->getMainBusNumInputChannels());
    }

    int getNumOutputChannels() const
    {
        if (processor == nullptr)
            return 0;

        return (channelConfiguration.size() > 0 ? channelConfiguration[0].numOuts
                                                : processor->getMainBusNumOutputChannels());
    }

    static String getFilePatterns (const String& fileSuffix)
    {
        if (fileSuffix.isEmpty())
            return {};

        return (fileSuffix.startsWithChar ('.') ? "*" : "*.") + fileSuffix;
    }

    //==============================================================================
    Value& getMuteInputValue()                           { return shouldMuteInput; }
    bool getProcessorHasPotentialFeedbackLoop() const    { return false; }
    void valueChanged (Value& value) override            { muteInput = (bool) value.getValue(); }

    //==============================================================================
    File getLastFile() const
    {
        File f;

        if (settings != nullptr)
            f = File (settings->getValue ("lastStateFile"));

        if (f == File())
            f = File::getSpecialLocation (File::userDocumentsDirectory);

        return f;
    }

    void setLastFile (const FileChooser& fc)
    {
        if (settings != nullptr)
            settings->setValue ("lastStateFile", fc.getResult().getFullPathName());
    }

    /** Pops up a dialog letting the user save the processor's state to a file. */
    void askUserToSaveState (const String& fileSuffix = String())
    {
        stateFileChooser = std::make_unique<FileChooser> (TRANS("Save current state"),
                                                          getLastFile(),
                                                          getFilePatterns (fileSuffix));
        auto flags = FileBrowserComponent::saveMode
                   | FileBrowserComponent::canSelectFiles
                   | FileBrowserComponent::warnAboutOverwriting;

        stateFileChooser->launchAsync (flags, [this] (const FileChooser& fc)
        {
            if (fc.getResult() == File{})
                return;

            setLastFile (fc);

            MemoryBlock data;
            processor->getStateInformation (data);

            if (! fc.getResult().replaceWithData (data.getData(), data.getSize()))
                AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon,
                                                  TRANS("Error whilst saving"),
                                                  TRANS("Couldn't write to the specified file!"));
        });
    }

    /** Pops up a dialog letting the user re-load the processor's state from a file. */
    void askUserToLoadState (const String& fileSuffix = String())
    {
        stateFileChooser = std::make_unique<FileChooser> (TRANS("Load a saved state"),
                                                          getLastFile(),
                                                          getFilePatterns (fileSuffix));
        auto flags = FileBrowserComponent::openMode
                   | FileBrowserComponent::canSelectFiles;

        stateFileChooser->launchAsync (flags, [this] (const FileChooser& fc)
        {
            if (fc.getResult() == File{})
                return;

            setLastFile (fc);

            MemoryBlock data;

            if (fc.getResult().loadFileAsData (data))
                processor->setStateInformation (data.getData(), (int) data.getSize());
            else
                AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon,
                                                  TRANS("Error whilst loading"),
                                                  TRANS("Couldn't read from the specified file!"));
        });
    }

    //==============================================================================
    void startPlaying()
    {
        player.setProcessor (processor.get());

       #if JucePlugin_Enable_IAA && JUCE_IOS
        if (auto device = dynamic_cast<iOSAudioIODevice*> (deviceManager.getCurrentAudioDevice()))
        {
            processor->setPlayHead (device->getAudioPlayHead());
            device->setMidiMessageCollector (&player.getMidiMessageCollector());
        }
       #endif
    }

    void stopPlaying()
    {
        player.setProcessor (nullptr);
    }

    //==============================================================================
    /** Shows an audio properties dialog box modally. */
    void showAudioSettingsDialog(Typeface::Ptr customFont)
        {
                DialogWindow::LaunchOptions o;

                int maxNumInputs = 0, maxNumOutputs = 0;

                if (channelConfiguration.size() > 0)
                {
                    auto& defaultConfig = channelConfiguration.getReference (0);

                    maxNumInputs  = jmax (0, (int) defaultConfig.numIns);
                    maxNumOutputs = jmax (0, (int) defaultConfig.numOuts);
                }

                if (auto* bus = processor->getBus (true, 0))
                    maxNumInputs = jmax (0, bus->getDefaultLayout().size());

                if (auto* bus = processor->getBus (false, 0))
                    maxNumOutputs = jmax (0, bus->getDefaultLayout().size());

                auto content = std::make_unique<SettingsComponent> (*this, deviceManager, maxNumInputs, maxNumOutputs, customFont);
                content->centreWithSize(500, 550);
                //content->setBounds (400, 400, 500, 550);
                content->setToRecommendedSize();
                juce::Rectangle<int> r = Desktop::getInstance().getDisplays().getMainDisplay().totalArea;

                
#if ! (JUCE_IOS || JUCE_ANDROID)
                o.content.setOwned (content.release());
        
                o.dialogTitle                   = TRANS("Audio/MIDI Settings");
                o.dialogBackgroundColour        = Colour(15, 15, 15);
                o.escapeKeyTriggersCloseButton  = true;
                o.useNativeTitleBar             = true;
                o.resizable                     = false;
        
                o.launchAsync();
#else
                CallOutBox::launchAsynchronously (std::move(content), r.withY(r.getCentreY()), nullptr);
#endif
        }

    void saveAudioDeviceState()
    {
        if (settings != nullptr)
        {
            auto xml = deviceManager.createStateXml();

            settings->setValue ("audioSetup", xml.get());

           #if ! (JUCE_IOS || JUCE_ANDROID)
            settings->setValue ("shouldMuteInput", (bool) false);//shouldMuteInput.getValue());
           #endif
        }
    }

    void reloadAudioDeviceState (bool enableAudioInput,
                                 const String& preferredDefaultDeviceName,
                                 const AudioDeviceManager::AudioDeviceSetup* preferredSetupOptions)
    {
        std::unique_ptr<XmlElement> savedState;

        if (settings != nullptr)
        {
            savedState = settings->getXmlValue ("audioSetup");

           #if ! (JUCE_IOS || JUCE_ANDROID)
            shouldMuteInput.setValue (false);//settings->getBoolValue ("shouldMuteInput", true));
           #endif
        }

        auto inputChannels  = getNumInputChannels();
        auto outputChannels = getNumOutputChannels();

        if (inputChannels == 0 && outputChannels == 0 && processor->isMidiEffect())
        {
            // add a dummy output channel for MIDI effect plug-ins so they can receive audio callbacks
            outputChannels = 1;
        }

        deviceManager.initialise (enableAudioInput ? inputChannels : 0,
                                  outputChannels,
                                  savedState.get(),
                                  true,
                                  preferredDefaultDeviceName,
                                  preferredSetupOptions);
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

    //==============================================================================
    void switchToHostApplication()
    {
       #if JUCE_IOS
        if (auto device = dynamic_cast<iOSAudioIODevice*> (deviceManager.getCurrentAudioDevice()))
            device->switchApplication();
       #endif
    }

    bool isInterAppAudioConnected()
    {
       #if JUCE_IOS
        if (auto device = dynamic_cast<iOSAudioIODevice*> (deviceManager.getCurrentAudioDevice()))
            return device->isInterAppAudioConnected();
       #endif

        return false;
    }

    Image getIAAHostIcon (int size)
    {
       #if JUCE_IOS && JucePlugin_Enable_IAA
        if (auto device = dynamic_cast<iOSAudioIODevice*> (deviceManager.getCurrentAudioDevice()))
            return device->getIcon (size);
       #else
        ignoreUnused (size);
       #endif

        return {};
    }

    static StandalonePluginHolder* getInstance();

    //==============================================================================
    OptionalScopedPointer<PropertySet> settings;
    std::unique_ptr<AudioProcessor> processor;
    AudioDeviceManager deviceManager;
    AudioProcessorPlayer player;
    Array<PluginInOuts> channelConfiguration;

    // avoid feedback loop by default
    bool processorHasPotentialFeedbackLoop = true;
    std::atomic<bool> muteInput { true };
    Value shouldMuteInput;
    AudioBuffer<float> emptyBuffer;
    bool autoOpenMidiDevices;

    std::unique_ptr<AudioDeviceManager::AudioDeviceSetup> options;
    Array<MidiDeviceInfo> lastMidiDevices;

    std::unique_ptr<FileChooser> stateFileChooser;

private:
    /*  This class can be used to ensure that audio callbacks use buffers with a
        predictable maximum size.

        On some platforms (such as iOS 10), the expected buffer size reported in
        audioDeviceAboutToStart may be smaller than the blocks passed to
        audioDeviceIOCallback. This can lead to out-of-bounds reads if the render
        callback depends on additional buffers which were initialised using the
        smaller size.

        As a workaround, this class will ensure that the render callback will
        only ever be called with a block with a length less than or equal to the
        expected block size.
    */
    class CallbackMaxSizeEnforcer  : public AudioIODeviceCallback
    {
    public:
        explicit CallbackMaxSizeEnforcer (AudioIODeviceCallback& callbackIn)
            : inner (callbackIn) {}

        void audioDeviceAboutToStart (AudioIODevice* device) override
        {
            maximumSize = device->getCurrentBufferSizeSamples();
            storedInputChannels .resize ((size_t) device->getActiveInputChannels() .countNumberOfSetBits());
            storedOutputChannels.resize ((size_t) device->getActiveOutputChannels().countNumberOfSetBits());

            inner.audioDeviceAboutToStart (device);
        }

        void audioDeviceIOCallback (const float** inputChannelData,
                                    int numInputChannels,
                                    float** outputChannelData,
                                    int numOutputChannels,
                                    int numSamples) override
        {
            jassertquiet ((int) storedInputChannels.size()  == numInputChannels);
            jassertquiet ((int) storedOutputChannels.size() == numOutputChannels);

            int position = 0;

            while (position < numSamples)
            {
                const auto blockLength = jmin (maximumSize, numSamples - position);

                initChannelPointers (inputChannelData,  storedInputChannels,  position);
                initChannelPointers (outputChannelData, storedOutputChannels, position);

                inner.audioDeviceIOCallback (storedInputChannels.data(),
                                             (int) storedInputChannels.size(),
                                             storedOutputChannels.data(),
                                             (int) storedOutputChannels.size(),
                                             blockLength);

                position += blockLength;
            }
        }

        void audioDeviceStopped() override
        {
            inner.audioDeviceStopped();
        }

    private:
        struct GetChannelWithOffset
        {
            int offset;

            template <typename Ptr>
            auto operator() (Ptr ptr) const noexcept -> Ptr { return ptr + offset; }
        };

        template <typename Ptr, typename Vector>
        void initChannelPointers (Ptr&& source, Vector&& target, int offset)
        {
            std::transform (source, source + target.size(), target.begin(), GetChannelWithOffset { offset });
        }

        AudioIODeviceCallback& inner;
        int maximumSize = 0;
        std::vector<const float*> storedInputChannels;
        std::vector<float*> storedOutputChannels;
    };

    CallbackMaxSizeEnforcer maxSizeEnforcer { *this };

    //==============================================================================
    class SettingsComponent : public Component
    {
    public:
        SettingsComponent (StandalonePluginHolder& pluginHolder,
                           AudioDeviceManager& deviceManagerToUse,
                           int maxAudioInputChannels,
                           int maxAudioOutputChannels, Typeface::Ptr fontToUse)
            : owner (pluginHolder),
              deviceSelector (deviceManagerToUse,
                              0, maxAudioInputChannels,
                              0, maxAudioOutputChannels,
                              true,
                              (pluginHolder.processor.get() != nullptr && pluginHolder.processor->producesMidi()),
                              true, false),
              font(fontToUse),
              shouldMuteLabel  ("Feedback Loop:", "Feedback Loop:"),
              shouldMuteButton ("Mute audio input")
        {
            
            setOpaque (true);
            deviceSelector.getLookAndFeel().setColour(ListBox::ColourIds::textColourId, Colours::white);
            deviceSelector.getLookAndFeel().setColour(ListBox::ColourIds::backgroundColourId, Colour(52, 52, 52));
            deviceSelector.getLookAndFeel().setColour(ComboBox::ColourIds::textColourId, Colours::white);
            deviceSelector.getLookAndFeel().setColour(ComboBox::ColourIds::backgroundColourId, Colour(52, 52, 52));
            deviceSelector.getLookAndFeel().setColour(TextButton::ColourIds::textColourOffId, Colours::white);
            deviceSelector.getLookAndFeel().setColour(TextButton::ColourIds::textColourOnId, Colours::white);
            deviceSelector.getLookAndFeel().setColour(TextButton::ColourIds::buttonColourId, Colour(52, 52, 52));
            deviceSelector.getLookAndFeel().setColour(PopupMenu::ColourIds::backgroundColourId, Colour(52, 52, 52));
            
            
            deviceSelector.sendLookAndFeelChange();
            
            shouldMuteButton.setClickingTogglesState (true);
            shouldMuteButton.getToggleStateValue().referTo (owner.shouldMuteInput);

            addAndMakeVisible (deviceSelector);

            if (owner.getProcessorHasPotentialFeedbackLoop())
            {
                addAndMakeVisible (shouldMuteButton);
                addAndMakeVisible (shouldMuteLabel);

                shouldMuteLabel.attachToComponent (&shouldMuteButton, true);
            }
        }

        void paint (Graphics& g) override
        {
            g.fillAll (Colour(125, 125, 125));
        }

        void resized() override
        {
            const ScopedValueSetter<bool> scope (isResizing, true);

            auto r = getLocalBounds();

            if (owner.getProcessorHasPotentialFeedbackLoop())
            {
                auto itemHeight = deviceSelector.getItemHeight();
                auto extra = r.removeFromTop (itemHeight);

                auto seperatorHeight = (itemHeight >> 1);
                shouldMuteButton.setBounds (juce::Rectangle<int> (extra.proportionOfWidth (0.35f), seperatorHeight,
                                                            extra.proportionOfWidth (0.60f), deviceSelector.getItemHeight()));

                r.removeFromTop (seperatorHeight);
            }

            deviceSelector.setBounds (r);
        }

        void childBoundsChanged (Component* childComp) override
        {
            if (! isResizing && childComp == &deviceSelector)
                setToRecommendedSize();
        }

        void setToRecommendedSize()
        {
            const auto extraHeight = [&]
            {
                if (! owner.getProcessorHasPotentialFeedbackLoop())
                    return 0;

                const auto itemHeight = deviceSelector.getItemHeight();
                const auto separatorHeight = (itemHeight >> 1);
                return itemHeight + separatorHeight;
            }();

            setSize (getWidth(), deviceSelector.getHeight() + extraHeight);
        }

    private:
        //==============================================================================
        StandalonePluginHolder& owner;
        AudioDeviceSelectorComponent deviceSelector;
        Label shouldMuteLabel;
        ToggleButton shouldMuteButton;
        bool isResizing = false;
        Typeface::Ptr font;

        //==============================================================================
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SettingsComponent)
    };

    //==============================================================================
    void audioDeviceIOCallback (const float** inputChannelData,
                                int numInputChannels,
                                float** outputChannelData,
                                int numOutputChannels,
                                int numSamples) override
    {
        if (muteInput)
        {
            emptyBuffer.clear();
            inputChannelData = emptyBuffer.getArrayOfReadPointers();
        }

        player.audioDeviceIOCallback (inputChannelData, numInputChannels,
                                      outputChannelData, numOutputChannels, numSamples);
    }

    void audioDeviceAboutToStart (AudioIODevice* device) override
    {
        emptyBuffer.setSize (device->getActiveInputChannels().countNumberOfSetBits(), device->getCurrentBufferSizeSamples());
        emptyBuffer.clear();

        player.audioDeviceAboutToStart (device);
        player.setMidiOutput (deviceManager.getDefaultMidiOutput());
    }

    void audioDeviceStopped() override
    {
        player.setMidiOutput (nullptr);
        player.audioDeviceStopped();
        emptyBuffer.setSize (0, 0);
    }

    //==============================================================================
    void setupAudioDevices (bool enableAudioInput,
                            const String& preferredDefaultDeviceName,
                            const AudioDeviceManager::AudioDeviceSetup* preferredSetupOptions)
    {
        deviceManager.addAudioCallback (&maxSizeEnforcer);
        deviceManager.addMidiInputDeviceCallback ({}, &player);

        reloadAudioDeviceState (enableAudioInput, preferredDefaultDeviceName, preferredSetupOptions);
    }

    void shutDownAudioDevices()
    {
        saveAudioDeviceState();

        deviceManager.removeMidiInputDeviceCallback ({}, &player);
        deviceManager.removeAudioCallback (&maxSizeEnforcer);
    }

    void timerCallback() override
    {
        auto newMidiDevices = MidiInput::getAvailableDevices();

        if (newMidiDevices != lastMidiDevices)
        {
            for (auto& oldDevice : lastMidiDevices)
                if (! newMidiDevices.contains (oldDevice))
                    deviceManager.setMidiInputDeviceEnabled (oldDevice.identifier, false);

            for (auto& newDevice : newMidiDevices)
                if (! lastMidiDevices.contains (newDevice))
                    deviceManager.setMidiInputDeviceEnabled (newDevice.identifier, true);

            lastMidiDevices = newMidiDevices;
        }
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StandalonePluginHolder)
};

//==============================================================================
/**
    A class that can be used to run a simple standalone application containing your filter.

    Just create one of these objects in your JUCEApplicationBase::initialise() method, and
    let it do its work. It will create your filter object using the same createPluginFilter() function
    that the other plugin wrappers use.

    @tags{Audio}
*/
class StandaloneFilterWindow    : public DocumentWindow,
                                  private Button::Listener
{
public:
    //==============================================================================
    typedef StandalonePluginHolder::PluginInOuts PluginInOuts;
    juce::Rectangle<int> optionButtonBounds;
    bool hasSettingsButtons = false;
    Typeface::Ptr customFont;
    LookAndFeel_V4 laf4;
    //==============================================================================
    /** Creates a window with a given title and colour.
        The settings object can be a PropertySet that the class should use to
        store its settings (it can also be null). If takeOwnershipOfSettings is
        true, then the settings object will be owned and deleted by this object.
    */
    StandaloneFilterWindow (const String& title,
                            Colour backgroundColour,
                            PropertySet* settingsToUse,
                            bool takeOwnershipOfSettings,
                            const String& preferredDefaultDeviceName = String(),
                            const AudioDeviceManager::AudioDeviceSetup* preferredSetupOptions = nullptr,
                            const Array<PluginInOuts>& constrainToConfiguration = {},
                           #if JUCE_ANDROID || JUCE_IOS
                            bool autoOpenMidiDevices = true
                           #else
                            bool autoOpenMidiDevices = false
                           #endif
                            )
        : DocumentWindow (title, backgroundColour, 0),
          optionsButton ("Options")
    {
        
        const String dir = File::getSpecialLocation(File::currentExecutableFile).getParentDirectory().getParentDirectory().getFullPathName();
        const String pluginBundleName = File::getSpecialLocation(File::currentExecutableFile).getParentDirectory().getParentDirectory().getParentDirectory().getFileNameWithoutExtension();
                
        File csdFile;
        StringArray csdLines;


#ifdef JUCE_WINDOWS
        csdFile = File::getSpecialLocation(File::currentExecutableFile).withFileExtension(String(".csd")).getFullPathName();
        const String homeDrive = File::getSpecialLocation(File::windowsSystemDirectory).getParentDirectory().getParentDirectory().getFullPathName();
        if (csdFile.existsAsFile() == false)
        {
            String filename = homeDrive + "/ProgramData/" + String(CabbageManufacturer) + "/" + File::getSpecialLocation(File::currentExecutableFile).getFileNameWithoutExtension() + "/" + File::getSpecialLocation(File::currentExecutableFile).withFileExtension(String(".csd")).getFileName();
            csdFile = File(filename);
           
        }
#elif JUCE_MAC
        if(csdFile.existsAsFile() == false)
        {
            const String filename(File::getSpecialLocation(File::currentExecutableFile).withFileExtension(String(".csd")).getFileName());
            csdFile = CabbageUtilities::getRealUserHomeDirectory().getFullPathName() + "/Library/" + String(CabbageManufacturer) + "/" + File::getSpecialLocation(File::currentExecutableFile).getFileNameWithoutExtension()+"/"+filename;

            if(!csdFile.existsAsFile() && (PluginHostType::getPluginLoadedAs() == AudioProcessor::wrapperType_AudioUnit))
            {
                csdFile = CabbageUtilities::getRealUserHomeDirectory().getFullPathName() + "/Library/" + String(CabbageManufacturer) + "/" + pluginBundleName + "/"+filename;
            }
        }
#else
        CabbageUtilities::debug(CabbageManufacturer);
        csdFile = File::getSpecialLocation(File::currentExecutableFile).withFileExtension(String(".csd")).getFullPathName();
        if (!csdFile.existsAsFile())
        {
            //In Linux, plugin application directory will reside in ~/Manufacturer/PluginName
            String filename = "~/" + String(CabbageManufacturer) + "/" + File::getSpecialLocation(File::currentExecutableFile).getFileNameWithoutExtension() + "/" + File::getSpecialLocation(File::currentExecutableFile).withFileExtension(String(".csd")).getFileName();

            csdFile = File(filename);
        }
#endif
        
        csdLines.addLines(csdFile.loadFileAsString());
        int t = csdLines.size();
        int titleBarHeight = 22;
        
        for (const auto& line : csdLines)
        {
          if (line.contains("</Cabbage>"))
              break;

          ValueTree temp("temp");
          CabbageWidgetData::setWidgetState(temp, line, 0);
          if (CabbageWidgetData::getStringProp(temp, CabbageIdentifierIds::type) == "form")
          {
              auto typeFace = CabbageWidgetData::getStringProp(temp, CabbageIdentifierIds::typeface);
              titleBarHeight = CabbageWidgetData::getNumProp(temp, CabbageIdentifierIds::titlebarheight);
              if(typeFace.isNotEmpty()){
                  const String fontPath = csdFile.getParentDirectory().getChildFile(typeFace).getFullPathName();
                  if(File(fontPath).existsAsFile())
                  {
                      customFont = CabbageUtilities::getFontFromFile(File(fontPath));
                      getLookAndFeel().setDefaultSansSerifTypeface(customFont);
                  }
                  else
                      customFont = Font(999).getTypeface();
              }
          }
          if (CabbageWidgetData::getStringProp(temp, CabbageIdentifierIds::channel) == "settings") {
              int x = CabbageWidgetData::getNumProp(temp, CabbageIdentifierIds::left);
              int y = CabbageWidgetData::getNumProp(temp, CabbageIdentifierIds::top);
              int w = CabbageWidgetData::getNumProp(temp, CabbageIdentifierIds::width);
              int h = CabbageWidgetData::getNumProp(temp, CabbageIdentifierIds::height);
              optionButtonBounds = {x, y+titleBarHeight, w, h};
              hasSettingsButtons = true;
          }
        }
        
        
       #if JUCE_IOS || JUCE_ANDROID
        setTitleBarHeight (0);
       #else
        setTitleBarHeight (titleBarHeight);
        setTitleBarButtonsRequired (DocumentWindow::minimiseButton | DocumentWindow::closeButton, false);

        Component::addAndMakeVisible (optionsButton);
        optionsButton.setLookAndFeel(&laf4);
        optionsButton.addListener (this);
        optionsButton.setTriggeredOnMouseDown (true);
       #endif

        pluginHolder.reset (new StandalonePluginHolder (settingsToUse, takeOwnershipOfSettings,
                                                        preferredDefaultDeviceName, preferredSetupOptions,
                                                        constrainToConfiguration, autoOpenMidiDevices));

       #if JUCE_IOS || JUCE_ANDROID
        setFullScreen (true);
        setContentOwned (new MainContentComponent (*this), false);
       #else
        setContentOwned (new MainContentComponent (*this), true);

        const auto windowScreenBounds = [this]() -> juce::Rectangle<int>
        {
            const auto width = getWidth();
            const auto height = getHeight();

            const auto& displays = Desktop::getInstance().getDisplays();

            if (auto* props = pluginHolder->settings.get())
            {
                constexpr int defaultValue = -100;

                const auto x = props->getIntValue ("windowX", defaultValue);
                const auto y = props->getIntValue ("windowY", defaultValue);

                if (x != defaultValue && y != defaultValue)
                {
                    const auto screenLimits = displays.getDisplayForRect ({ x, y, width, height })->userArea;

                    return { jlimit (screenLimits.getX(), jmax (screenLimits.getX(), screenLimits.getRight()  - width),  x),
                             jlimit (screenLimits.getY(), jmax (screenLimits.getY(), screenLimits.getBottom() - height), y),
                             width, height };
                }
            }

            const auto displayArea = displays.getPrimaryDisplay()->userArea;

            return { displayArea.getCentreX() - width / 2,
                     displayArea.getCentreY() - height / 2,
                     width, height };
        }();

        setBoundsConstrained (windowScreenBounds);

        if (auto* processor = getAudioProcessor())
            if (auto* editor = processor->getActiveEditor())
                setResizable (editor->isResizable(), false);
       #endif
    }

    ~StandaloneFilterWindow() override
    {
       #if (! JUCE_IOS) && (! JUCE_ANDROID)
        if (auto* props = pluginHolder->settings.get())
        {
            props->setValue ("windowX", getX());
            props->setValue ("windowY", getY());
        }
       #endif

        pluginHolder->stopPlaying();
        clearContentComponent();
        pluginHolder = nullptr;
    }

    //==============================================================================
    AudioProcessor* getAudioProcessor() const noexcept      { return pluginHolder->processor.get(); }
    AudioDeviceManager& getDeviceManager() const noexcept   { return pluginHolder->deviceManager; }

    /** Deletes and re-creates the plugin, resetting it to its default state. */
    void resetToDefaultState()
    {
        pluginHolder->stopPlaying();
        clearContentComponent();
        pluginHolder->deletePlugin();

        if (auto* props = pluginHolder->settings.get())
            props->removeValue ("filterState");

        pluginHolder->createPlugin();
        setContentOwned (new MainContentComponent (*this), true);
        pluginHolder->startPlaying();
    }

    //==============================================================================
    void closeButtonPressed() override
    {
        pluginHolder->savePluginState();

        JUCEApplicationBase::quit();
    }

    void handleMenuResult (int result)
    {

    }

    static void menuCallback (int result, StandaloneFilterWindow* button)
    {
       
    }

    void resized() override
    {
        DocumentWindow::resized();
        
        if(hasSettingsButtons)
        {
            optionsButton.setAlpha(0.f);
            optionsButton.setBounds (optionButtonBounds);
            optionsButton.setAlwaysOnTop(true);
        }
        else
        {
            optionsButton.setBounds (8, 6, 60, getTitleBarHeight() - 8);
        }
    }

    virtual StandalonePluginHolder* getPluginHolder()    { return pluginHolder.get(); }

    std::unique_ptr<StandalonePluginHolder> pluginHolder;

private:
    void buttonClicked (Button*) override
    {
        pluginHolder->showAudioSettingsDialog(customFont);
    }

    //==============================================================================
    class MainContentComponent  : public Component,
                                  private Value::Listener,
                                  private Button::Listener,
                                  private ComponentListener
    {
    public:
        MainContentComponent (StandaloneFilterWindow& filterWindow)
            : owner (filterWindow), notification (this),
              editor (owner.getAudioProcessor()->hasEditor() ? owner.getAudioProcessor()->createEditorIfNeeded()
                                                             : new GenericAudioProcessorEditor (*owner.getAudioProcessor()))
        {
            inputMutedValue.referTo (owner.pluginHolder->getMuteInputValue());

            if (editor != nullptr)
            {
                editor->addComponentListener (this);
                componentMovedOrResized (*editor, false, true);

                addAndMakeVisible (editor.get());
            }

            //addChildComponent (notification);
            shouldShowNotification = false;//owner.pluginHolder->getProcessorHasPotentialFeedbackLoop();

            if (owner.pluginHolder->getProcessorHasPotentialFeedbackLoop())
            {
                inputMutedValue.addListener (this);
                shouldShowNotification = inputMutedValue.getValue();
            }

            inputMutedChanged (shouldShowNotification);
        }

        ~MainContentComponent() override
        {
            if (editor != nullptr)
            {
                editor->removeComponentListener (this);
                owner.pluginHolder->processor->editorBeingDeleted (editor.get());
                editor = nullptr;
            }
        }

        void resized() override
        {
            auto r = getLocalBounds();

            if (shouldShowNotification)
                notification.setBounds (r.removeFromTop (NotificationArea::height));

            if (editor != nullptr)
                editor->setBoundsConstrained (editor->getLocalArea (this, r.toFloat())
                                                     .withPosition (r.getTopLeft().toFloat().transformedBy (editor->getTransform().inverted()))
                                                .toNearestInt());
        }

    private:
        //==============================================================================
        class NotificationArea : public Component
        {
        public:
            enum { height = 30 };
            

            NotificationArea (Button::Listener* settingsButtonListener)
                : notification ("notification", "Audio input is muted to avoid feedback loop"),
                 #if JUCE_IOS || JUCE_ANDROID
                  settingsButton ("Unmute Input")
                 #else
                  settingsButton ("Settings...")
                 #endif
            {
                setOpaque (true);
                notification.setColour (Label::textColourId, Colours::black);	
                addAndMakeVisible (notification);
                addAndMakeVisible (settingsButton);
            }

            void paint (Graphics& g) override
            {
                auto r = getLocalBounds();

                g.setColour (Colours::darkgoldenrod);
                g.fillRect (r.removeFromBottom (1));

                g.setColour (Colours::lightgoldenrodyellow);
                g.fillRect (r);
            }

            void resized() override
            {
                auto r = getLocalBounds().reduced (5);

                settingsButton.setBounds (r.removeFromRight (70));
                notification.setBounds (r);
            }
        private:
            Label notification;
            TextButton settingsButton;
        };

        //==============================================================================
        void inputMutedChanged (bool newInputMutedValue)
        {
            shouldShowNotification = newInputMutedValue;
            notification.setVisible (shouldShowNotification);

           #if JUCE_IOS || JUCE_ANDROID
            resized();
           #else
            if (editor != nullptr)
            {
                const int extraHeight = shouldShowNotification ? NotificationArea::height : 0;
                const auto rect = getSizeToContainEditor();

                if (auto* editorConstrainer = editor->getConstrainer())
                {
                    const auto borders = owner.getContentComponentBorder();
                    const auto extraWindowWidth = borders.getLeftAndRight();
                    const auto extraWindowHeight = extraHeight + borders.getTopAndBottom();

                    owner.setResizeLimits (jmax (10, editorConstrainer->getMinimumWidth()  + extraWindowWidth),
                                           jmax (10, editorConstrainer->getMinimumHeight() + extraWindowHeight),
                                           editorConstrainer->getMaximumWidth()  + extraWindowWidth,
                                           editorConstrainer->getMaximumHeight() + extraWindowHeight);
                }

                setSize (rect.getWidth(), rect.getHeight() + extraHeight);
            }
           #endif
        }

        void valueChanged (Value& value) override     { inputMutedChanged (value.getValue()); }
        void buttonClicked (Button*) override
        {
           #if JUCE_IOS || JUCE_ANDROID
            owner.pluginHolder->getMuteInputValue().setValue (false);
           #else
            owner.pluginHolder->showAudioSettingsDialog(owner.customFont);
           #endif
        }

        //==============================================================================
        void componentMovedOrResized (Component&, bool, bool) override
        {
            if (editor != nullptr)
            {
                auto rect = getSizeToContainEditor();

                setSize (rect.getWidth(),
                         rect.getHeight() + (shouldShowNotification ? NotificationArea::height : 0));
            }
        }

        juce::Rectangle<int> getSizeToContainEditor() const
        {
            if (editor != nullptr)
                return getLocalArea (editor.get(), editor->getLocalBounds());

            return {};
        }

        //==============================================================================
        StandaloneFilterWindow& owner;
        NotificationArea notification;
        std::unique_ptr<AudioProcessorEditor> editor;
        Value inputMutedValue;
        bool shouldShowNotification = false;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
    };

    //==============================================================================
    TextButton optionsButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StandaloneFilterWindow)
};

inline StandalonePluginHolder* StandalonePluginHolder::getInstance()
{
   #if JucePlugin_Enable_IAA || JucePlugin_Build_Standalone
    if (PluginHostType::getPluginLoadedAs() == AudioProcessor::wrapperType_Standalone)
    {
        auto& desktop = Desktop::getInstance();
        const int numTopLevelWindows = desktop.getNumComponents();

        for (int i = 0; i < numTopLevelWindows; ++i)
            if (auto window = dynamic_cast<StandaloneFilterWindow*> (desktop.getComponent (i)))
                return window->getPluginHolder();
    }
   #endif

    return nullptr;
}


#endif
