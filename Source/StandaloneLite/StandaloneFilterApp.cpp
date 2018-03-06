#include "../JuceLibraryCode/JuceHeader.h"
#include "StandaloneFilterWindow.h"
#include "../CabbageCommonHeaders.h"

//==============================================================================
class StandaloneFilterApp  : public JUCEApplication
{
public:
    StandaloneFilterApp()
    {
        // PluginHostType::jucePlugInClientCurrentWrapperType = AudioProcessor::wrapperType_Standalone;

        PropertiesFile::Options options;

        options.applicationName     = getApplicationName();
        options.filenameSuffix      = ".settings";
        options.osxLibrarySubFolder = "Application Support";
#if JUCE_LINUX
        options.folderName          = "~/.config";
#else
        options.folderName          = "";
#endif

        appProperties.setStorageParameters (options);
    }

    const String getApplicationName() override              { return "Cabbage Lite"; }
    const String getApplicationVersion() override           { return "2.0"; }
    bool moreThanOneInstanceAllowed() override              { return true; }
    void anotherInstanceStarted (const String&) override    {}

    virtual StandaloneFilterWindow* createWindow()
    {
#ifdef JucePlugin_PreferredChannelConfigurations
        StandalonePluginHolder::PluginInOuts channels[] = { JucePlugin_PreferredChannelConfigurations };
#endif

        return new StandaloneFilterWindow (getApplicationName(),  getCommandLineParameters(),
                                           LookAndFeel::getDefaultLookAndFeel().findColour (ResizableWindow::backgroundColourId),
                                           appProperties.getUserSettings(),
                                           false, {}, nullptr
#ifdef JucePlugin_PreferredChannelConfigurations
                                           , juce::Array<StandalonePluginHolder::PluginInOuts> (channels, juce::numElementsInArray (channels))
#endif
                                          );
    }

    //==============================================================================
    void initialise (const String&) override
    {
        mainWindow = createWindow();

#if JUCE_IOS || JUCE_ANDROID
        Desktop::getInstance().setKioskModeComponent (mainWindow, false);
#endif

        mainWindow->setVisible (true);
    }

    void shutdown() override
    {
        mainWindow = nullptr;
        appProperties.saveIfNeeded();
    }

    //==============================================================================
    void systemRequestedQuit() override
    {
        quit();
    }

protected:
    ApplicationProperties appProperties;
    ScopedPointer<StandaloneFilterWindow> mainWindow;
};

#if JucePlugin_Build_Standalone && JUCE_IOS

bool JUCE_CALLTYPE juce_isInterAppAudioConnected()
{
    if (auto holder = StandalonePluginHolder::getInstance())
        return holder->isInterAppAudioConnected();

    return false;
}

void JUCE_CALLTYPE juce_switchToHostApplication()
{
    if (auto holder = StandalonePluginHolder::getInstance())
        holder->switchToHostApplication();
}

#if JUCE_MODULE_AVAILABLE_juce_gui_basics
Image JUCE_CALLTYPE juce_getIAAHostIcon (int size)
{
    if (auto holder = StandalonePluginHolder::getInstance())
        return holder->getIAAHostIcon (size);

    return Image();
}
#endif
#endif

START_JUCE_APPLICATION (StandaloneFilterApp)
