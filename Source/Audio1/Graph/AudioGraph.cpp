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

  You should have received a copy of the GNU General Public
  License along with Csound; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
  02111-1307 USA
*/

#include "AudioGraph.h"
#include "../../Application/CabbageMainComponent.h"

//==============================================================================
class PluginWindow;


const int AudioGraph::midiChannelNumber = 0x1000;

AudioGraph::AudioGraph (CabbageMainComponent& owner_, PropertySet* settingsToUse,
                        bool takeOwnershipOfSettings,
                        const String& preferredDefaultDeviceName,
                        const AudioDeviceManager::AudioDeviceSetup* preferredSetupOptions)

    :   settings (settingsToUse, takeOwnershipOfSettings),
        graph(),
        owner (owner_),
        lookAndFeel(),
        FileBasedDocument (filenameSuffix,
                           filenameWildcard,
                           "Load a filter graph",
                           "Save a filter graph"),
        formatManager (new AudioPluginFormatManager())
{
    newDocument();
    graph.prepareToPlay (44100, 512);
    graph.setPlayConfigDetails (2, 2, 44100, 512);
    AudioProcessor::setTypeOfNextNewPlugin (AudioProcessor::wrapperType_Standalone);
    createInternalFilters();
    setupAudioDevices (preferredDefaultDeviceName, preferredSetupOptions);
    startPlaying();

}

AudioGraph::~AudioGraph()
{
    deletePlugin();
    shutDownAudioDevices();
    xmlSettings = nullptr;
}

void AudioGraph::createInternalFilters()
{
    AudioProcessorGraph::Node* midiInputNode = createNode (getPluginDescriptor ("Internal", "Midi Input", internalNodeIds[InternalNodes::MIDIInput]), internalNodeIds[InternalNodes::MIDIInput]);
    internalNodeIds.add (midiInputNode->nodeId);
    setNodePosition (internalNodeIds[InternalNodes::MIDIInput], .4, .1);


    AudioProcessorGraph::Node* inputNode =  createNode (getPluginDescriptor ("Internal", "Audio Input", internalNodeIds[InternalNodes::MIDIInput]), internalNodeIds[InternalNodes::AudioInput]);
    internalNodeIds.add (inputNode->nodeId);
    setNodePosition (internalNodeIds[InternalNodes::AudioInput], .6, .1);

    AudioProcessorGraph::Node* outputNode =  createNode (getPluginDescriptor ("Internal", "Audio Output", internalNodeIds[InternalNodes::MIDIInput]), internalNodeIds[InternalNodes::AudioOutput]);
    internalNodeIds.add (outputNode->nodeId);
    setNodePosition (internalNodeIds[InternalNodes::AudioOutput], .5, .8);
}

//==============================================================================
void AudioGraph::addPlugin (File inputFile, int32 nodeId)
{
    for ( int i = 0 ; i < graph.getNumNodes() ; i++)
    {
        if (graph.getNode (i)->nodeId == nodeId)
        {
            Point<double> position = this->getNodePosition (nodeId);
            ScopedPointer<XmlElement> xml = createConnectionsXml();
            //delete graph.getNodeForId(nodeId)->getProcessor();
            graph.removeNode (nodeId);
            AudioProcessorGraph::Node::Ptr node = createNode (getPluginDescriptor ("Cabbage", "Cabbage", nodeId, inputFile.getFullPathName()), nodeId);
            setNodePosition (nodeId, position.getX(), position.getY());
            restoreConnectionsFromXml (*xml);
            xml = nullptr;
            return;
        }
    }

    setChangedFlag (true);
    createNode (getPluginDescriptor ("Cabbage", "Cabbage", nodeId, inputFile.getFullPathName()), nodeId);
    setDefaultConnections (nodeId);
}
//==============================================================================
const PluginDescription AudioGraph::getPluginDescriptor (String type, String name, int32 nodeId, String inputFile )
{
    PluginDescription descript;
    descript.fileOrIdentifier = inputFile;
    descript.descriptiveName = type + ":" + inputFile;
    descript.name = name;
    descript.numInputChannels = 2;
    descript.numOutputChannels = 2;
    descript.isInstrument = true;
    descript.uid = nodeId;

    descript.manufacturerName = "CabbageAudio";
    descript.pluginFormatName = type;

    return descript;
}
//==============================================================================
void AudioGraph::showCodeEditorForNode (int32 nodeId)
{
    for ( int i = 0 ; i < owner.getNodeIds().size() ; i++)
    {
        if (int32 (owner.getNodeIds().getValueAt (i)) == nodeId)
        {
            const String nodeFilename = owner.getNodeIds().getName (i).toString();
            owner.bringCodeEditorToFront (File (nodeFilename));
        }
    }
}
//==============================================================================
AudioProcessorGraph::Node::Ptr AudioGraph::createNode (const PluginDescription& desc, int32 nodeId)
{
    if (desc.pluginFormatName == "Cabbage")
    {
        AudioProcessor* processor;
        bool isCabbageFile = CabbageUtilities::hasCabbageTags (File (desc.fileOrIdentifier));

        if (isCabbageFile)
            processor = createCabbagePluginFilter (File (desc.fileOrIdentifier));
        else
            processor = createGenericPluginFilter (File (desc.fileOrIdentifier));

        AudioProcessor::setTypeOfNextNewPlugin (AudioProcessor::wrapperType_Undefined);
        jassert (processor != nullptr);
        processor->enableAllBuses();
        const int inputs = processor->getBusCount (true);
        const int outputs = processor->getBusCount (false);

        const int numberOfChannels = static_cast<CsoundPluginProcessor*> (processor)->getNumberOfCsoundChannels();

        processor->disableNonMainBuses();
        processor->setRateAndBufferSizeDetails (44100, 512);

        AudioProcessor::Bus* ins = processor->getBus (true, 0);
        ins->setCurrentLayout (AudioChannelSet::discreteChannels (numberOfChannels));

        AudioProcessor::Bus* outs = processor->getBus (false, 0);
        outs->setCurrentLayout (AudioChannelSet::discreteChannels (numberOfChannels));

        AudioProcessorGraph::Node* node = graph.addNode (processor, nodeId);
        ScopedPointer<XmlElement> xmlElem;
        xmlElem = desc.createXml();
        node->properties.set ("pluginType", isCabbageFile == true ? "Cabbage" : "Csound");
        node->properties.set ("pluginName", "Test");
        node->properties.set ("pluginDesc", xmlElem->createDocument (""));

        return node;

    }
    else if (desc.pluginFormatName == "Internal")
    {
        ScopedPointer<XmlElement> xmlElem;
        xmlElem = desc.createXml();

        if (desc.name == "Midi Input")
        {
            AudioProcessorGraph::AudioGraphIOProcessor* midiNode;
            midiNode = new AudioProcessorGraph::AudioGraphIOProcessor (AudioProcessorGraph::AudioGraphIOProcessor::midiInputNode);
            AudioProcessorGraph::Node* node = graph.addNode (midiNode, nodeId);
            node->properties.set ("pluginDesc", xmlElem->createDocument (""));
            return node;
        }
        else if (desc.name == "Audio Input")
        {
            AudioProcessorGraph::AudioGraphIOProcessor* inNode;
            inNode = new AudioProcessorGraph::AudioGraphIOProcessor (AudioProcessorGraph::AudioGraphIOProcessor::audioInputNode);
            AudioProcessorGraph::Node* node = graph.addNode (inNode, nodeId);
            node->properties.set ("pluginDesc", xmlElem->createDocument (""));
            return node;
        }
        else if (desc.name == "Audio Output")
        {
            AudioProcessorGraph::AudioGraphIOProcessor* outNode;
            outNode = new AudioProcessorGraph::AudioGraphIOProcessor (AudioProcessorGraph::AudioGraphIOProcessor::audioOutputNode);
            //outNode->properties.set("pluginDesc", xmlElem->createDocument(""));
            AudioProcessorGraph::Node* node = graph.addNode (outNode, nodeId);
            node->properties.set ("pluginDesc", xmlElem->createDocument (""));
            return node;
        }
    }

    return nullptr;

}
//==============================================================================
void AudioGraph::setDefaultConnections (int nodeId)
{
    setNodePosition (nodeId, .4 + (graph.getNumNodes() - 3)*.05, .5);

    bool connectInput1 = graph.addConnection (internalNodeIds[InternalNodes::AudioInput], 0, nodeId, 0);
    bool connectInput2 = graph.addConnection (internalNodeIds[InternalNodes::AudioInput], 1, nodeId, 1);

    bool connection1 = graph.addConnection (nodeId, 0, internalNodeIds[InternalNodes::AudioOutput], 0);
    bool connection2 = graph.addConnection (nodeId, 1, internalNodeIds[InternalNodes::AudioOutput], 1);
    //if(connection2 == false && connection1 == true)
    //  graph.addConnection (nodeId, 0, internalNodeIds[InternalNodes::AudioOutput], 1);


    bool connection3 = graph.addConnection (internalNodeIds[InternalNodes::MIDIInput], AudioProcessorGraph::midiChannelIndex, nodeId, AudioProcessorGraph::midiChannelIndex);

    //if (connection1 == false  || connectInput1 == false )
    //    jassertfalse;
}


void AudioGraph::updateBusLayout (AudioProcessor* selectedProcessor)
{
    //    if (AudioProcessor* filter = selectedProcessor)
    //    {
    //        if (AudioProcessor::Bus* bus = filter->getBus (isInput, currentBus))
    //        {
    //            bool test = bus->setNumberOfChannels(8);
    //        }
    //    }
}

//==============================================================================
void AudioGraph::deletePlugin()
{
    PluginWindow::closeAllCurrentlyOpenWindows();
    stopPlaying();
    graph.clear();
}

String AudioGraph::getFilePatterns (const String& fileSuffix)
{
    if (fileSuffix.isEmpty())
        return String();

    return (fileSuffix.startsWithChar ('.') ? "*" : "*.") + fileSuffix;
}

void AudioGraph::setXmlAudioSettings (XmlElement* xmlSettingsString)
{
    xmlSettings = xmlSettingsString;
    setupAudioDevices ( String(), nullptr);
    startPlaying();
}

AudioDeviceSelectorComponent* AudioGraph::getAudioDeviceSelector()
{
    const int numInputs = 2;//processor != nullptr ? processor->getTotalNumInputChannels() : 2;
    const int numOutputs = 2;//processor != nullptr ? processor->getTotalNumOutputChannels() : 2;

    return new AudioDeviceSelectorComponent (deviceManager,
                                             numInputs,
                                             numInputs,
                                             numOutputs,
                                             numOutputs,
                                             true, false,
                                             true, false);
}

String AudioGraph::getDeviceManagerSettings()
{
    if (deviceManager.getCurrentAudioDevice())
    {
        ScopedPointer<XmlElement> xml (deviceManager.createStateXml());

        if (xml == nullptr)
            return String::empty;
        else
            return xml->createDocument ("");
    }
    else return String::empty;
}

//==============================================================================
void AudioGraph::startPlaying()
{
    player.setProcessor (&graph);
}

void AudioGraph::stopPlaying()
{
    player.setProcessor (nullptr);
}

void AudioGraph::reloadAudioDeviceState (const String& preferredDefaultDeviceName,
                                         const AudioDeviceManager::AudioDeviceSetup* preferredSetupOptions)
{
    ScopedPointer<XmlElement> savedState;

    if (settings != nullptr)
        savedState = settings->getXmlValue ("audioSetup");

    deviceManager.initialise (256,
                              256,
                              savedState,
                              true,
                              preferredDefaultDeviceName,
                              preferredSetupOptions);
}

//==============================================================================
String AudioGraph::getCsoundOutput (int32 nodeId)
{
    if (graph.getNodeForId (nodeId) != nullptr &&
        graph.getNodeForId (nodeId)->getProcessor() != nullptr)
    {
        if (graph.getNodeForId (nodeId)->properties.getWithDefault ("pluginType", "").toString() == "Cabbage")
            return dynamic_cast<CabbagePluginProcessor*> (graph.getNodeForId (nodeId)->getProcessor())->getCsoundOutput();
        else
            return dynamic_cast<GenericCabbagePluginProcessor*> (graph.getNodeForId (nodeId)->getProcessor())->getCsoundOutput();
    }

    return String::empty;
}

//==============================================================================
const AudioProcessorGraph::Node::Ptr AudioGraph::getNode (const int index) const noexcept
{
    return graph.getNode (index);
}

const AudioProcessorGraph::Node::Ptr AudioGraph::getNodeForId (const uint32 uid) const noexcept
{
    return graph.getNodeForId (uid);
}

//==============================================================================
int AudioGraph::getNumPlugins() const noexcept
{
    return graph.getNumNodes();
}

void AudioGraph::setNodePosition (const uint32 nodeId, double x, double y)
{
    if (AudioProcessorGraph::Node::Ptr n = graph.getNodeForId (nodeId))
    {
        n->properties.set ("x", jlimit (0.0, 1.0, x));
        n->properties.set ("y", jlimit (0.0, 1.0, y));
    }
}

Point<double> AudioGraph::getNodePosition (const uint32 nodeId) const
{
    if (AudioProcessorGraph::Node::Ptr n = graph.getNodeForId (nodeId))
        return Point<double> (static_cast<double> (n->properties ["x"]),
                              static_cast<double> (n->properties ["y"]));

    return Point<double>();
}
//==============================================================================
int AudioGraph::getNumConnections() const noexcept
{
    return graph.getNumConnections();
}

const AudioProcessorGraph::Connection* AudioGraph::getConnection (const int index) const noexcept
{
    return graph.getConnection (index);
}

const AudioProcessorGraph::Connection* AudioGraph::getConnectionBetween (uint32 sourceFilterUID, int sourceFilterChannel,
                                                                         uint32 destFilterUID, int destFilterChannel) const noexcept
{
    return graph.getConnectionBetween (sourceFilterUID, sourceFilterChannel,
                                       destFilterUID, destFilterChannel);
}

bool AudioGraph::canConnect (uint32 sourceFilterUID, int sourceFilterChannel,
                             uint32 destFilterUID, int destFilterChannel) const noexcept
{
    return graph.canConnect (sourceFilterUID, sourceFilterChannel,
                             destFilterUID, destFilterChannel);
}

bool AudioGraph::addConnection (uint32 sourceFilterUID, int sourceFilterChannel,
                                uint32 destFilterUID, int destFilterChannel)
{
    const bool result = graph.addConnection (sourceFilterUID, sourceFilterChannel,
                                             destFilterUID, destFilterChannel);

    if (result)
        changed();

    return result;
}

void AudioGraph::removeFilter (const uint32 id)
{
    PluginWindow::closeCurrentlyOpenWindowsFor (id);

    if (graph.removeNode (id))
    {
        for ( int i = 0; i < owner.getNodeIds().size() ; i++)
        {
            if ( int32 (owner.getNodeIds().getValueAt (i)) == id)
            {
                owner.getNodeIds().remove (owner.getNodeIds().getName (i));
                break;
            }
        }

        changed();

    }
}

void AudioGraph::disconnectFilter (const uint32 id)
{
    if (graph.disconnectNode (id))
        changed();
}

void AudioGraph::removeConnection (const int index)
{
    graph.removeConnection (index);
    changed();
}

void AudioGraph::removeConnection (uint32 sourceFilterUID, int sourceFilterChannel,
                                   uint32 destFilterUID, int destFilterChannel)
{
    if (graph.removeConnection (sourceFilterUID, sourceFilterChannel,
                                destFilterUID, destFilterChannel))
        changed();
}

void AudioGraph::clear()
{
    PluginWindow::closeAllCurrentlyOpenWindows();

    graph.clear();
    changed();
}

//==============================================================================
static void readBusLayoutFromXml (AudioProcessor::BusesLayout& busesLayout, AudioProcessor* plugin, const XmlElement& xml, const bool isInput)
{
    Array<AudioChannelSet>& targetBuses = (isInput ? busesLayout.inputBuses : busesLayout.outputBuses);
    int maxNumBuses = 0;

    if (auto* buses = xml.getChildByName (isInput ? "INPUTS" : "OUTPUTS"))
    {
        forEachXmlChildElementWithTagName (*buses, e, "BUS")
        {
            const int busIdx = e->getIntAttribute ("index");
            maxNumBuses = jmax (maxNumBuses, busIdx + 1);

            // the number of buses on busesLayout may not be in sync with the plugin after adding buses
            // because adding an input bus could also add an output bus
            for (int actualIdx = plugin->getBusCount (isInput) - 1; actualIdx < busIdx; ++actualIdx)
                if (! plugin->addBus (isInput)) return;

            for (int actualIdx = targetBuses.size() - 1; actualIdx < busIdx; ++actualIdx)
                targetBuses.add (plugin->getChannelLayoutOfBus (isInput, busIdx));

            const String& layout = e->getStringAttribute ("layout");

            if (layout.isNotEmpty())
                targetBuses.getReference (busIdx) = AudioChannelSet::fromAbbreviatedString (layout);
        }
    }

    // if the plugin has more buses than specified in the xml, then try to remove them!
    while (maxNumBuses < targetBuses.size())
    {
        if (! plugin->removeBus (isInput))
            return;

        targetBuses.removeLast();
    }
}

static XmlElement* createBusLayoutXml (const AudioProcessor::BusesLayout& layout, const bool isInput)
{
    const Array<AudioChannelSet>& buses = (isInput ? layout.inputBuses : layout.outputBuses);

    XmlElement* xml = new XmlElement (isInput ? "INPUTS" : "OUTPUTS");

    const int n = buses.size();

    for (int busIdx = 0; busIdx < n; ++busIdx)
    {
        XmlElement* bus = new XmlElement ("BUS");
        bus->setAttribute ("index", busIdx);

        const AudioChannelSet& set = buses.getReference (busIdx);
        const String layoutName = set.isDisabled() ? "disabled" : set.getSpeakerArrangementAsString();

        bus->setAttribute ("layout", layoutName);

        xml->addChildElement (bus);
    }

    return xml;
}

static XmlElement* createNodeXml (AudioProcessorGraph::Node* const node) noexcept
{
    PluginDescription pd;

    if (AudioPluginInstance* plugin = dynamic_cast <AudioPluginInstance*> (node->getProcessor()))
        plugin->fillInPluginDescription (pd);
    else if (dynamic_cast <CabbagePluginProcessor*> (node->getProcessor()) ||
             dynamic_cast <CsoundPluginProcessor*> (node->getProcessor()))
    {
        //grab description of native plugin for saving...
        String xmlPluginDescriptor = node->properties.getWithDefault ("pluginDesc", "").toString();
        //cUtils::debug(xmlPluginDescriptor);
        XmlElement* xmlElem;
        xmlElem = XmlDocument::parse (xmlPluginDescriptor);
        pd.loadFromXml (*xmlElem);
    }


    XmlElement* e = new XmlElement ("FILTER");
    e->setAttribute ("uid", (int) node->nodeId);
    e->setAttribute ("x", node->properties ["x"].toString());
    e->setAttribute ("y", node->properties ["y"].toString());
    e->setAttribute ("uiLastX", node->properties ["uiLastX"].toString());
    e->setAttribute ("uiLastY", node->properties ["uiLastY"].toString());
    e->addChildElement (pd.createXml());

    XmlElement* state = new XmlElement ("STATE");

    MemoryBlock m;
    node->getProcessor()->getStateInformation (m);
    state->addTextElement (m.toBase64Encoding());
    e->addChildElement (state);

    return e;
}

void AudioGraph::restoreFromXml (const XmlElement& xml)
{
    clear();

    forEachXmlChildElementWithTagName (xml, e, "FILTER")
    {
        createNodeFromXml (*e);
        changed();
    }

    forEachXmlChildElementWithTagName (xml, e, "CONNECTION")
    {
        addConnection ((uint32) e->getIntAttribute ("srcFilter"),
                       e->getIntAttribute ("srcChannel"),
                       (uint32) e->getIntAttribute ("dstFilter"),
                       e->getIntAttribute ("dstChannel"));
    }

    graph.removeIllegalConnections();
}

void AudioGraph::restoreConnectionsFromXml (const XmlElement& xml)
{
    forEachXmlChildElementWithTagName (xml, e, "CONNECTION")
    {
        addConnection ((uint32) e->getIntAttribute ("srcFilter"),
                       e->getIntAttribute ("srcChannel"),
                       (uint32) e->getIntAttribute ("dstFilter"),
                       e->getIntAttribute ("dstChannel"));
    }

    graph.removeIllegalConnections();
}

void AudioGraph::createNodeFromXml (const XmlElement& xml)
{

    ScopedPointer<XmlElement> xml2 = XmlDocument::parse (File ("/home/rory/Desktop/NodeTest.xml"));
    PluginDescription desc;

    forEachXmlChildElement (xml, e)
    {
        if (desc.loadFromXml (*e))
            break;
    }

    //xml.writeToFile (File ("/home/rory/Desktop/NodeTest.xml"), "");
    AudioProcessorGraph::Node::Ptr node = createNode (desc, xml.getIntAttribute ("uid"));

    if (node == nullptr)
        jassertfalse;

    if (const XmlElement* const state = xml.getChildByName ("STATE"))
    {
        MemoryBlock m;
        m.fromBase64Encoding (state->getAllSubText());

        node->getProcessor()->setStateInformation (m.getData(), (int) m.getSize());
    }

    node->properties.set ("x", xml.getDoubleAttribute ("x"));
    node->properties.set ("y", xml.getDoubleAttribute ("y"));
    node->properties.set ("uiLastX", xml.getIntAttribute ("uiLastX"));
    node->properties.set ("uiLastY", xml.getIntAttribute ("uiLastY"));
    node->properties.set ("pluginName", desc.name);

    setNodePosition (node->nodeId, xml.getDoubleAttribute ("x"), xml.getDoubleAttribute ("y"));
}

XmlElement* AudioGraph::createXml() const
{
    XmlElement* xml = new XmlElement ("FILTERGRAPH");

    for (int i = 0; i < graph.getNumNodes(); ++i)
        xml->addChildElement (createNodeXml (graph.getNode (i)));

    for (int i = 0; i < graph.getNumConnections(); ++i)
    {
        const AudioProcessorGraph::Connection* const fc = graph.getConnection (i);

        XmlElement* e = new XmlElement ("CONNECTION");

        e->setAttribute ("srcFilter", (int) fc->sourceNodeId);
        e->setAttribute ("srcChannel", fc->sourceChannelIndex);
        e->setAttribute ("dstFilter", (int) fc->destNodeId);
        e->setAttribute ("dstChannel", fc->destChannelIndex);

        xml->addChildElement (e);
    }

    return xml;
}

XmlElement* AudioGraph::createConnectionsXml() const
{
    XmlElement* xml = new XmlElement ("FILTERGRAPH");

    for (int i = 0; i < graph.getNumConnections(); ++i)
    {
        const AudioProcessorGraph::Connection* const fc = graph.getConnection (i);

        XmlElement* e = new XmlElement ("CONNECTION");

        e->setAttribute ("srcFilter", (int) fc->sourceNodeId);
        e->setAttribute ("srcChannel", fc->sourceChannelIndex);
        e->setAttribute ("dstFilter", (int) fc->destNodeId);
        e->setAttribute ("dstChannel", fc->destChannelIndex);

        xml->addChildElement (e);
    }

    return xml;
}
//========================================================================================
String AudioGraph::getDocumentTitle()
{
    if (! getFile().exists())
        return "Unnamed";

    return getFile().getFileNameWithoutExtension();
}

void AudioGraph::newDocument()
{
    clear();
    setFile (File());
    setChangedFlag (false);
}

Result AudioGraph::loadDocument (const File& file)
{
    XmlDocument doc (file);
    ScopedPointer<XmlElement> xml (doc.getDocumentElement());

    if (xml == nullptr || ! xml->hasTagName ("FILTERGRAPH"))
        return Result::fail ("Not a valid filter graph file");

    restoreFromXml (*xml);
    return Result::ok();
}

FileBasedDocument::SaveResult AudioGraph::saveGraph (bool saveAs)
{
    FileChooser fc ("Save file as", File::getSpecialLocation (File::SpecialLocationType::userHomeDirectory), "", CabbageUtilities::shouldUseNativeBrowser());

    if (getFile().existsAsFile() && saveAs == false)
    {
        saveDocument (getFile().withFileExtension (".cabbage"));
    }
    else
    {
        if (fc.browseForFileToSave (false))
        {
            if (fc.getResult().existsAsFile())
            {

                const int result = CabbageUtilities::showYesNoMessage ("Do you wish to overwrite\nexiting file?", &lookAndFeel);

                if (result == 1)
                {
                    saveDocument (fc.getResult().withFileExtension (".cabbage"));
                    return FileBasedDocument::SaveResult::savedOk;
                }
                else
                    return FileBasedDocument::SaveResult::userCancelledSave;
            }
            else
            {
                saveDocument (fc.getResult().withFileExtension (".cabbage"));
                return FileBasedDocument::SaveResult::savedOk;
            }

        }
    }

    return FileBasedDocument::SaveResult::savedOk;
}

Result AudioGraph::saveDocument (const File& file)
{
    ScopedPointer<XmlElement> xml (createXml());

    if (! xml->writeToFile (file, String()))
        return Result::fail ("Couldn't write to the file");

    return Result::ok();
}

File AudioGraph::getLastDocumentOpened()
{
    //    RecentlyOpenedFilesList recentFiles;
    //    recentFiles.restoreFromString (getAppProperties().getUserSettings()
    //                                        ->getValue ("recentFilterGraphFiles"));
    //
    //    return recentFiles.getFile (0);
    return File();
}

void AudioGraph::setLastDocumentOpened (const File& file)
{
    //    RecentlyOpenedFilesList recentFiles;
    //    recentFiles.restoreFromString (getAppProperties().getUserSettings()
    //                                        ->getValue ("recentFilterGraphFiles"));
    //
    //    recentFiles.addFile (file);
    //
    //    getAppProperties().getUserSettings()
    //        ->setValue ("recentFilterGraphFiles", recentFiles.toString());
}
//========================================================================================

static Array <PluginWindow*> activePluginWindows;

PluginWindow::PluginWindow (Component* const pluginEditor,
                            AudioProcessorGraph::Node* const o,
                            WindowFormatType t,
                            AudioProcessorGraph& audioGraph)
    : DocumentWindow (pluginEditor->getName(), Colours::black,
                      DocumentWindow::minimiseButton | DocumentWindow::closeButton),
      graph (audioGraph),
      owner (o),
      type (t)
{
    setSize (400, 300);
    setContentOwned (pluginEditor, true);
    setVisible (true);

    activePluginWindows.add (this);
}

void PluginWindow::closeCurrentlyOpenWindowsFor (const uint32 nodeId)
{
    for (int i = activePluginWindows.size(); --i >= 0;)
        if (activePluginWindows.getUnchecked (i)->owner->nodeId == nodeId)
            delete activePluginWindows.getUnchecked (i);
}

Point<int> PluginWindow::getPositionOfCurrentlyOpenWindow (const uint32 nodeId)
{
    for (int i = activePluginWindows.size(); --i >= 0;)
        if (activePluginWindows.getUnchecked (i)->owner->nodeId == nodeId)
            return Point<int> (activePluginWindows.getUnchecked (i)->getX(), activePluginWindows.getUnchecked (i)->getY());

    return Point<int> (-1000, -1000);
}

void PluginWindow::closeAllCurrentlyOpenWindows()
{
    if (activePluginWindows.size() > 0)
    {
        for (int i = activePluginWindows.size(); --i >= 0;)
            delete activePluginWindows.getUnchecked (i);

        // fixed issue with focus in JUCE 4, doesn't seem to be need in JUCE 5
        //        Component dummyModalComp;
        //        dummyModalComp.enterModalState();
        //        MessageManager::getInstance()->runDispatchLoopUntil (150);
    }
}

//==============================================================================
PluginWindow* PluginWindow::getWindowFor (AudioProcessorGraph::Node* const node,
                                          WindowFormatType type,
                                          AudioProcessorGraph& audioGraph)
{
    jassert (node != nullptr);

    for (int i = activePluginWindows.size(); --i >= 0;)
        if (activePluginWindows.getUnchecked (i)->owner == node
            && activePluginWindows.getUnchecked (i)->type == type)
            return activePluginWindows.getUnchecked (i);

    AudioProcessor* processor = node->getProcessor();
    AudioProcessorEditor* ui = nullptr;

    if (type == Normal)
    {
        ui = processor->createEditorIfNeeded();

        if (ui == nullptr)
            type = Generic;
    }

    if (ui == nullptr)
    {
        if (type == Generic || type == Parameters)
            ui = new GenericAudioProcessorEditor (processor);

        //        else if (type == Programs)
        //            ui = new ProgramAudioProcessorEditor (processor);
        //        else if (type == AudioIO)
        //            ui = new FilterIOConfigurationWindow (processor);
    }

    if (ui != nullptr)
    {
        if (AudioPluginInstance* const plugin = dynamic_cast<AudioPluginInstance*> (processor))
            ui->setName (plugin->getName());

        return new PluginWindow (ui, node, type, audioGraph);
    }

    return nullptr;
}

PluginWindow::~PluginWindow()
{
    activePluginWindows.removeFirstMatchingValue (this);

    if (AudioProcessorEditor* ed = dynamic_cast<AudioProcessorEditor*> (getContentComponent()))
    {
        owner->getProcessor()->editorBeingDeleted (ed);
        ed->setLookAndFeel(nullptr);
        clearContentComponent();
    }
}

void PluginWindow::moved()
{

}

void PluginWindow::closeButtonPressed()
{

    delete this;
}