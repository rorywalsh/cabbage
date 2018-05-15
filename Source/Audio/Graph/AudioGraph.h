/*
  Copyright (C) 2016 Rory Walsh

  Cabbage is free software; you can redistribute it
  and/or modify it under the terms of the GNU General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  Cabbage is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.
//
  You should have received a copy of the GNU General Public
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

class CabbageMainComponent;
const char* const filenameSuffix = ".cabbagepatch";
const char* const filenameWildcard = "*.cabbagepatch";

class AudioGraph :  public FileBasedDocument, public AudioProcessorListener
{

public:
    // This creates new instances of the plugin..
    GenericCabbagePluginProcessor* JUCE_CALLTYPE createGenericPluginFilter (File inputFile)
    {
		const int numChannels = CabbageUtilities::getHeaderInfo(inputFile.loadFileAsString(), "nchnls");
        return new GenericCabbagePluginProcessor (inputFile, numChannels, numChannels);
    }

    CabbagePluginProcessor* JUCE_CALLTYPE createCabbagePluginFilter (File inputFile)
    {
		const int numChannels = CabbageUtilities::getHeaderInfo(inputFile.loadFileAsString(), "nchnls");
        return new CabbagePluginProcessor (inputFile, numChannels, numChannels);
    }

    AudioGraph (CabbageMainComponent& owner, PropertySet* settingsToUse,
                bool takeOwnershipOfSettings = true,
                const String& preferredDefaultDeviceName = String(),
                const AudioDeviceManager::AudioDeviceSetup* preferredSetupOptions = nullptr);
    ~AudioGraph();

    enum InternalNodes
    {
        MIDIInput = 0,
        AudioInput,
        AudioOutput
    };


    int getNumPlugins() const noexcept;
    bool addPlugin (File inputFile, int32 nodeId);
    AudioProcessorGraph::Node::Ptr createNode (const PluginDescription& desc, int32 uid);
    AudioProcessorGraph::Node* createCabbageNode (const PluginDescription& desc, int32 nodeId);
    AudioProcessorGraph::Node* createInternalNode (const PluginDescription& desc, int32 nodeId);

    const PluginDescription getPluginDescriptor (String type, String name, int32 nodeId, String inputfile = "");
    void setDefaultConnections (int nodeId);
    void createInternalFilters();
    void showCodeEditorForNode (int32 nodeId);
    void updateBusLayout (AudioProcessor* selectedProcessor);
    int getNumberOfParameters();
    virtual void deletePlugin();
    static String getFilePatterns (const String& fileSuffix);
    void setXmlAudioSettings (XmlElement* xmlSettingsString);
    AudioDeviceSelectorComponent* getAudioDeviceSelector();
    String getDeviceManagerSettings();
    void setNodePosition (uint32 nodeId, double x, double y);
    Point<double> getNodePosition (uint32 nodeId) const;
    AudioProcessorGraph& getGraph() noexcept         { return graph; }

    //==============================================================================
    int getNumConnections() const noexcept;
    const AudioProcessorGraph::Connection* getConnection (const int index) const noexcept;

    const AudioProcessorGraph::Connection* getConnectionBetween (uint32 sourceFilterUID, int sourceFilterChannel,
                                                                 uint32 destFilterUID, int destFilterChannel) const noexcept;

    bool canConnect (uint32 sourceFilterUID, int sourceFilterChannel,
                     uint32 destFilterUID, int destFilterChannel) const noexcept;

    bool addConnection (uint32 sourceFilterUID, int sourceFilterChannel,
                        uint32 destFilterUID, int destFilterChannel);

    void removeConnection (const int index);

    void removeConnection (uint32 sourceFilterUID, int sourceFilterChannel,
                           uint32 destFilterUID, int destFilterChannel);

    void removeFilter (const uint32 id);
    void disconnectFilter (const uint32 id);
    void clear();

    //==============================================================================
    void startPlaying();
    void stopPlaying();
    void reloadAudioDeviceState (const String& preferredDefaultDeviceName,
                                 const AudioDeviceManager::AudioDeviceSetup* preferredSetupOptions);
    //==============================================================================
    String getCsoundOutput (int32 nodeId);
    //==============================================================================
    const AudioProcessorGraph::Node::Ptr getNode (const int index) const noexcept;
    const AudioProcessorGraph::Node::Ptr getNodeForId (const uint32 uid) const noexcept;
    //==============================================================================
    void audioProcessorParameterChanged (AudioProcessor*, int, float) override {}
    void audioProcessorChanged (AudioProcessor*) override { changed(); }
    //==============================================================================
    void newDocument();
    String getDocumentTitle() override;
    Result loadDocument (const File& file) override;
    Result saveDocument (const File& file) override;
    File getLastDocumentOpened() override;
    void setLastDocumentOpened (const File& file) override;
    FileBasedDocument::SaveResult saveGraph (bool saveAs = false);
    //==============================================================================
    XmlElement* createXml() const;
    XmlElement* createConnectionsXml() const;
    void restoreFromXml (const XmlElement& xml);
    void restoreConnectionsFromXml (const XmlElement& xml);

    OptionalScopedPointer<PropertySet> settings;

    AudioProcessorGraph graph;
    AudioDeviceManager deviceManager;
    AudioProcessorPlayer player;
    ScopedPointer<XmlElement> xmlSettings;
    bool isInput;
    int currentBus;
    //bool getIsCabbageFile()    {        return isCabbageFile;    }

    static const int midiChannelNumber;
    CabbageMainComponent* getParent() {  return &owner;  }
private:
    CabbageMainComponent& owner;
    Array<int> internalNodeIds;
    CabbageIDELookAndFeel lookAndFeel;
    ScopedPointer<AudioPluginFormatManager> formatManager;
    //bool isCabbageFile = false;
    void createNodeFromXml (const XmlElement& xml);
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
    static Point<int> getPositionOfCurrentlyOpenWindow (const uint32 node);


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
