/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2013 - Raw Material Software Ltd.

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

#include "../Utilities/CabbageUtilities.h"
#include "../Utilities/CabbageExportPlugin.h"
#include "../Audio/Plugins/CabbagePluginProcessor.h"
#include "../Audio/Plugins/CabbagePluginEditor.h"
#include "../Audio/Plugins/CsoundPluginEditor.h"
#include "../Audio/Plugins/CsoundPluginProcessor.h"
#include "../Audio/Plugins/GenericCabbagePluginProcessor.h"
#include "../Audio/Plugins/CabbagePluginProcessor.h"
#include "../Audio/Plugins/CabbagePluginEditor.h"
#include "StandalonePluginHolder.h"



//==============================================================================
/**
    A class that can be used to run a simple standalone application containing your filter.

    Just create one of these objects in your JUCEApplicationBase::initialise() method, and
    let it do its work. It will create your filter object using the same createPluginFilter() function
    that the other plugin wrappers use.
*/
class StandaloneFilterWindow    : public DocumentWindow,
    public Button::Listener,
    public Timer
{
public:
    //==============================================================================
    typedef StandalonePluginHolder::PluginInOuts PluginInOuts;

    //==============================================================================
    /** Creates a window with a given title and colour.
        The settings object can be a PropertySet that the class should use to
        store its settings (it can also be null). If takeOwnershipOfSettings is
        true, then the settings object will be owned and deleted by this object.
    */
    StandaloneFilterWindow (const String& title,
                            const String commandLineParams,
                            Colour backgroundColour,
                            PropertySet* settingsToUse,
                            bool takeOwnershipOfSettings,
                            const String& preferredDefaultDeviceName = String(),
                            const AudioDeviceManager::AudioDeviceSetup* preferredSetupOptions = nullptr,
                            const Array<PluginInOuts>& constrainToConfiguration = Array<PluginInOuts> ())
        : DocumentWindow (title, backgroundColour, DocumentWindow::minimiseButton | DocumentWindow::closeButton),
          optionsButton ("Options")
    {
        setTitleBarButtonsRequired (DocumentWindow::minimiseButton | DocumentWindow::closeButton, false);

        Component::addAndMakeVisible (optionsButton);
        optionsButton.addListener (this);
        optionsButton.setTriggeredOnMouseDown (true);
        pluginHolder = new StandalonePluginHolder (settingsToUse, takeOwnershipOfSettings,
                                                   preferredDefaultDeviceName, preferredSetupOptions,
                                                   constrainToConfiguration);
        setName ("CabbageLite");
        setAlwaysOnTop (true);
#if JUCE_IOS || JUCE_ANDROID
        setFullScreen (true);
        setContentOwned (new MainContentComponent (*this), false);
        Desktop::getInstance().setKioskModeComponent (this, false);
#else
        setContentOwned (new MainContentComponent (*this), true);

        if (auto* props = pluginHolder->settings.get())
        {
            const int x = props->getIntValue ("windowX", -100);
            const int y = props->getIntValue ("windowY", -100);

            if (x != -100 && y != -100)
                setBoundsConstrained ({ x, y, getWidth(), getHeight() });
            else
                centreWithSize (300, 300);

            this->setAlwaysOnTop(props->getIntValue("AlwaysOnTop"));

        }
        else
        {
            centreWithSize (300, 300);
        }

#endif

        startTimer (200);

        CabbageIDELookAndFeel lAndF;
        outputConsole = new CsoundOutputWindow();
        outputConsole->getEditor().setFont(Font(14, 1));
		if (commandLineParams.isNotEmpty())
        {
            String commandLine = commandLineParams.replace("-NSDocumentRevisionsDebugMode YES", "");
			String fileToOpen = File::getSpecialLocation(File::currentExecutableFile).getParentDirectory().getFullPathName() + "/" + commandLine.trim().removeCharacters("\"");

            if ((SystemStats::getOperatingSystemType() & SystemStats::MacOSX) != 0)
                //hocus pocus for OSX. It seems to append some gibbrish to the command line flags
                commandLine = commandLineParams.substring (0, commandLineParams.indexOf ("-") - 1);


            if (commandLine.contains ("--export-VSTi"))
            {
                String inputFileName = commandLine.substring (commandLine.indexOf ("--export-VSTi") + 13).trim().removeCharacters ("\"");

                if (File (inputFileName).existsAsFile())
                {
                    //openFile (inputFileName);
                    CabbageIDELookAndFeel lAndF;
#ifdef CabbagePro
					pluginExporter.exportPlugin("VSTi", File(inputFileName), getPluginInfo(csdFile, "id"), "", false, true);
#else
                    pluginExporter.exportPlugin ("VSTi", File (inputFileName), getPluginInfo(csdFile, "id"));
#endif
					JUCEApplicationBase::quit();
                }

            }
            else if (commandLine.contains ("--export-VST "))
            {
                String inputFileName = commandLine.substring (commandLine.indexOf ("--export-VST") + 12).trim().removeCharacters ("\"");

                if (File (inputFileName).existsAsFile())
                {
#ifdef CabbagePro
					pluginExporter.exportPlugin("VST", File(inputFileName), getPluginInfo(csdFile, "id"), "", false, true);
#else
                    pluginExporter.exportPlugin ("VST", File (inputFileName), getPluginInfo(csdFile, "id"));
#endif
                    JUCEApplicationBase::quit();
                }

            }
            else if (File(fileToOpen).existsAsFile())   //first try to open a file that resides in the same dir as the exe
            {
                openFile (fileToOpen);
                return;
            }
            else if(File(commandLine).existsAsFile()) //now try to open a file that contains a full file path
            {
                openFile (commandLine);
                return;
            }
        }

        pluginHolder->stopPlaying();
        
#ifdef JUCE_IOS
        const String csdText = "<Cabbage>\n"
        "form size(1000, 800), colour(\"red)\n"
        "label bounds(10, 10, 800, 200), text(\"Hello\")\n"
        "</Cabbage>\n"
        "<CsoundSynthesizer>\n"
        "<CsOptions>\n"
        "-odac\n"
        "</CsOptions>\n"
        "<CsInstruments>\n"
        "sr = 44100\n"
        "ksmps = 64\n"
        "nchnls = 2\n"
        "0dbfs = 1\n"
        "\n"
        "instr 1\n"
        "a1 oscili 1, 400\n"
        "outs a1, a1  \n"
        "endin\n"
        "\n"
        "</CsInstruments>\n"
        "<CsScore>\n"
        "i1 0 z\n"
        "</CsScore>\n"
        "</CsoundSynthesizer>";
        File tempFile(File::getSpecialLocation(File::SpecialLocationType::tempDirectory).getFullPathName()+"test.csd");
        tempFile.replaceWithText(csdText);
        CabbageUtilities::debug(tempFile.loadFileAsString());
        resetPlugin(tempFile);
#endif
    }

	const String getPluginInfo(File csdFile, String info)
	{
		StringArray csdLines;
		csdLines.addLines(csdFile.loadFileAsString());

		for (auto line : csdLines)
		{
			ValueTree temp("temp");
			CabbageWidgetData::setWidgetState(temp, line, 0);

			if (CabbageWidgetData::getStringProp(temp, CabbageIdentifierIds::type) == CabbageWidgetTypes::form)
			{
				if (info == "id")
					return CabbageWidgetData::getStringProp(temp, CabbageIdentifierIds::pluginid);
				else if (info == "manufacturer")
					return CabbageWidgetData::getStringProp(temp, CabbageIdentifierIds::manufacturer);
			}

		}

		return String();
	}


    ~StandaloneFilterWindow()
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
    AudioProcessor* getAudioProcessor() const noexcept      { return pluginHolder->processor; }
    AudioDeviceManager& getDeviceManager() const noexcept   { return pluginHolder->deviceManager; }

    /** Deletes and re-creates the plugin, resetting it to its default state. */
    void resetPlugin (File file)
    {
        pluginHolder->stopPlaying();
        clearContentComponent();
        pluginHolder->deletePlugin();

        if (auto* props = pluginHolder->settings.get())
        {
            props->removeValue ("filterState");
        }


        file.getParentDirectory().setAsCurrentWorkingDirectory();
        pluginHolder->createPlugin (file);
        setContentOwned (new MainContentComponent (*this), true);
        pluginHolder->startPlaying();

        if (file.existsAsFile())
            cabbageFiledOpened = true;

    }

    //==============================================================================
    void timerCallback()
    {
        int64 modTime = csdFile.getLastModificationTime().toMilliseconds();

        if (modTime != lastModified && csdFile.existsAsFile())
        {
            lastModified = csdFile.getLastModificationTime().toMilliseconds();
            CabbageUtilities::debug("resetting file due to update of file on disk");
            resetPlugin (csdFile);
        }

        if (CabbagePluginProcessor* plugin = dynamic_cast<CabbagePluginProcessor*> (this->getAudioProcessor()))
        {
            csoundOutput = plugin->getCsoundOutput();

            if (cabbageFiledOpened)
            {
                if(csoundOutput.length()>0)
                    outputConsole->setText(csoundOutput);
            }
        }
    }

    //==============================================================================
    void closeButtonPressed() override
    {
        JUCEApplicationBase::quit();
    }

    void buttonClicked (Button*) override
    {
        PopupMenu m;
        m.addItem (1, TRANS ("Audio/MIDI Settings..."));
        m.addSeparator();
        m.addItem (2, "Open Csound file");

        if ( this->isAlwaysOnTop() )
            m.addItem (3, "Disable always on top");
        else
            m.addItem (3, "Enable always on top");

        m.addSeparator();
        m.addItem (4, "Toggle output console");
        m.addItem (5, "About Cabbage");
        m.showMenuAsync (PopupMenu::Options(),
                         ModalCallbackFunction::forComponent (menuCallback, this));
    }

    void openFile(String inputFile)
    {
        if(File(inputFile).existsAsFile())
        {
            csdFile = File(inputFile);
            csdFile.getParentDirectory().setAsCurrentWorkingDirectory();
            resetPlugin (csdFile);
            lastModified = csdFile.getLastModificationTime().toMilliseconds();
            setName (getInstrumentname());
        }
        else
        {
            FileChooser fc ("Open File", File (""), "*.csd", CabbageUtilities::shouldUseNativeBrowser());

            if (fc.browseForFileToOpen())
            {
                csdFile = fc.getResult();
                csdFile.getParentDirectory().setAsCurrentWorkingDirectory();
                resetPlugin (fc.getResult());
                lastModified = csdFile.getLastModificationTime().toMilliseconds();
                setName (getInstrumentname());
            }
        }
    }

    void showOutputConsole()
    {
        outputConsole->setVisible (!outputConsole->isVisible());
    }

    void handleMenuResult (int result)
    {
        CabbageIDELookAndFeel lAndF;
        if (auto* props = pluginHolder->settings.get())
            props->setValue("AlwaysOnTop", this->isAlwaysOnTop()==true ? 1 : 0);



        switch (result)
        {
            case 1:
                pluginHolder->showAudioSettingsDialog();
                break;

            case 2:
                openFile("");
                break;

            case 3:
                setAlwaysOnTop (!isAlwaysOnTop());
                break;

            case 4:
                showOutputConsole();
                break;

            case 5:
                CabbageUtilities::showMessage (String (CABBAGE_VERSION), &lAndF);
                break;

            default:
                break;
        }
    }

    const String getInstrumentname()
    {
        StringArray csdLines;
        csdLines.addLines (csdFile.loadFileAsString());

        for (auto line : csdLines)
        {
            ValueTree temp ("temp");
            CabbageWidgetData::setWidgetState (temp, line, 0);

            if (CabbageWidgetData::getStringProp (temp, CabbageIdentifierIds::type) == CabbageWidgetTypes::form)
                return CabbageWidgetData::getStringProp (temp, CabbageIdentifierIds::caption);
        }
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

    virtual StandalonePluginHolder* getPluginHolder()    { return pluginHolder; }

    ScopedPointer<StandalonePluginHolder> pluginHolder;

private:
    //==============================================================================
    class MainContentComponent : public Component, private Value::Listener,
        Button::Listener,
        ComponentListener
    {
    public:
        MainContentComponent (StandaloneFilterWindow& filterWindow)
            : owner (filterWindow), notification (this),
              editor (owner.getAudioProcessor()->createEditorIfNeeded()),
              shouldShowNotification (false)
        {
            Value& inputMutedValue = owner.pluginHolder->getMuteInputValue();

            if (editor != nullptr)
            {
                editor->addComponentListener (this);
                componentMovedOrResized (*editor, false, true);

                addAndMakeVisible (editor);
            }

            addChildComponent (notification);

            if (owner.pluginHolder->getProcessorHasPotentialFeedbackLoop())
            {
                inputMutedValue.addListener (this);
                shouldShowNotification = inputMutedValue.getValue();
            }

            inputMutedChanged (shouldShowNotification);
        }

        ~MainContentComponent()
        {
            if (editor != nullptr)
            {
                editor->removeComponentListener (this);
                owner.pluginHolder->processor->editorBeingDeleted (editor);
                editor = nullptr;
            }
        }

        void paint (Graphics& g)  override
        {
            g.fillAll (Colour (50, 50, 50));
            const Image cabbageLogo = ImageCache::getFromMemory (CabbageBinaryData::CabbageLogoBig_png, CabbageBinaryData::CabbageLogoBig_pngSize);
            g.drawImage (cabbageLogo, getLocalBounds().toFloat(), RectanglePlacement::Flags::stretchToFit);
        }

        void resized() override
        {
            auto r = getLocalBounds();

            if (shouldShowNotification)
                notification.setBounds (r.removeFromTop (NotificationArea::height));

            editor->setBounds (r);
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

                settingsButton.addListener (settingsButtonListener);

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

            setSize (editor->getWidth(),
                     editor->getHeight()
                     + (shouldShowNotification ? NotificationArea::height : 0));
        }

        void valueChanged (Value& value) override     { inputMutedChanged (value.getValue()); }
        void buttonClicked (Button*) override
        {
#if JUCE_IOS || JUCE_ANDROID
            owner.pluginHolder->getMuteInputValue().setValue (false);
#else
            owner.pluginHolder->showAudioSettingsDialog();
#endif
        }

        //==============================================================================
        void componentMovedOrResized (Component&, bool, bool wasResized) override
        {
            if (wasResized && editor != nullptr)
                setSize (editor->getWidth(),
                         editor->getHeight() + (shouldShowNotification ? NotificationArea::height : 0));
        }

        //==============================================================================
        StandaloneFilterWindow& owner;
        NotificationArea notification;
        ScopedPointer<AudioProcessorEditor> editor;
        bool shouldShowNotification;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
    };

    //==============================================================================
    TextButton optionsButton;
    File csdFile;
    int64 lastModified;
    ScopedPointer<CsoundOutputWindow> outputConsole;
    bool cabbageFiledOpened = false;
    String csoundOutput;
    PluginExporter pluginExporter;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StandaloneFilterWindow)
};

StandalonePluginHolder* StandalonePluginHolder::getInstance()
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

#endif   // JUCE_STANDALONEFILTERWINDOW_H_INCLUDED
