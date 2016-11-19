/*
  Copyright (C) 2016 Rory Walsh

  Cabbage is free software; you can redistribute it
  and/or modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  Cabbage is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with Csound; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
  02111-1307 USA
*/

#ifndef JUCE_PluginWrapperWindow_H_INCLUDED
#define JUCE_PluginWrapperWindow_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "../Plugins/CsoundPluginEditor.h"
#include "../Plugins/CsoundPluginProcessor.h"
#include "../Plugins/GenericCabbagePluginProcessor.h"
#include "../Plugins/CabbagePluginProcessor.h"
#include "../Plugins/CabbagePluginEditor.h"
#include "../../Settings/CabbageSettings.h"


class AudioGraph
{
public:
    // This creates new instances of the plugin..
    GenericCabbagePluginProcessor* JUCE_CALLTYPE createGenericPluginFilter(File inputFile)
    {
        return new GenericCabbagePluginProcessor(inputFile);
    }

    CabbagePluginProcessor* JUCE_CALLTYPE createCabbagePluginFilter(File inputFile)
    {
        return new CabbagePluginProcessor(inputFile);
    }

    AudioGraph (PropertySet* settingsToUse, File inputFile,
                bool takeOwnershipOfSettings = true,
                const String& preferredDefaultDeviceName = String(),
                const AudioDeviceManager::AudioDeviceSetup* preferredSetupOptions = nullptr);
    ~AudioGraph();
    void createPlugin(File inputFile);
    void updateBusLayout(AudioProcessor* selectedProcessor);
    int getNumberOfParameters();
    virtual void deletePlugin();

    static String getFilePatterns (const String& fileSuffix);

    void setXmlAudioSettings(XmlElement* xmlSettingsString);
    AudioDeviceSelectorComponent* getAudioDeviceSelector();
    String getDeviceManagerSettings();
    //==============================================================================
    void startPlaying();
    void stopPlaying();
    void reloadAudioDeviceState (const String& preferredDefaultDeviceName,
                                 const AudioDeviceManager::AudioDeviceSetup* preferredSetupOptions);
    //==============================================================================
    String getCsoundOutput();
    //==============================================================================
    void savePluginState();
    void reloadPluginState();
    AudioProcessor* getProcessor();
    //==============================================================================
    OptionalScopedPointer<PropertySet> settings;
    AudioProcessor* processor;
    AudioProcessorGraph graph;
    AudioDeviceManager deviceManager;
    AudioProcessorPlayer player;
    ScopedPointer<XmlElement> xmlSettings;
    bool isInput;
    int currentBus;
    bool isCabbageFile=false;

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

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioGraph)
};

//
//inline String toString (PluginWindow::WindowFormatType type)
//{
//    switch (type)
//    {
//        case PluginWindow::Normal:     return "Normal";
//        case PluginWindow::Generic:    return "Generic";
//        case PluginWindow::Programs:   return "Programs";
//        case PluginWindow::Parameters: return "Parameters";
//        default:                       return String();
//    }
//};


class PluginWindow  : public DocumentWindow
{
public:
    enum WindowFormatType
    {
        Normal = 0,
        Generic,
        Programs,
        Parameters,
        AudioIO,
        NumTypes
    };

    PluginWindow (Component* pluginEditor, AudioProcessorGraph::Node*, WindowFormatType, AudioProcessorGraph&);
    ~PluginWindow();

    static PluginWindow* getWindowFor (AudioProcessorGraph::Node*, WindowFormatType, AudioProcessorGraph&);

    static void closeCurrentlyOpenWindowsFor (const uint32 nodeId);
    static void closeAllCurrentlyOpenWindows();
    static Point<int> getPositionOfCurrentlyOpenWindow(const uint32 node);


    void moved() override;
    void closeButtonPressed() override;


private:
    AudioProcessorGraph& graph;
    AudioProcessorGraph::Node* owner;
    WindowFormatType type;

    float getDesktopScaleFactor() const override
    {
        return 1.0f;
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginWindow)
};

#endif   // JUCE_PluginWrapperWindow_H_INCLUDED
