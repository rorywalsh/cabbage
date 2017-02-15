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


//==============================================================================
class PluginWindow;


const int AudioGraph::midiChannelNumber = 0x1000;

AudioGraph::AudioGraph (PropertySet* settingsToUse,
                        bool takeOwnershipOfSettings,
                        const String& preferredDefaultDeviceName,
                        const AudioDeviceManager::AudioDeviceSetup* preferredSetupOptions)

    :   settings (settingsToUse, takeOwnershipOfSettings),
        graph(),
        FileBasedDocument (filenameSuffix,
                           filenameWildcard,
                           "Load a filter graph",
                           "Save a filter graph"),
							formatManager(new AudioPluginFormatManager())
{
    graph.prepareToPlay (44100, 512);
    graph.setPlayConfigDetails (2, 2, 44100, 512);

    AudioProcessor::setTypeOfNextNewPlugin (AudioProcessor::wrapperType_Standalone);

    AudioProcessorGraph::AudioGraphIOProcessor* midiNode;
    midiNode = new AudioProcessorGraph::AudioGraphIOProcessor (AudioProcessorGraph::AudioGraphIOProcessor::midiInputNode);
    const AudioProcessorGraph::Node* midiInputNode = graph.addNode (midiNode);

    internalNodeIds.add (midiInputNode->nodeId);
	setNodePosition(internalNodeIds[InternalNodes::MIDIInput], .8, .1);

    AudioProcessorGraph::AudioGraphIOProcessor* inNode;
    inNode = new AudioProcessorGraph::AudioGraphIOProcessor (AudioProcessorGraph::AudioGraphIOProcessor::audioInputNode);
    const AudioProcessorGraph::Node* inputNode = graph.addNode (inNode);
    internalNodeIds.add (inputNode->nodeId);
	setNodePosition(internalNodeIds[InternalNodes::AudioInput], .7, .1);


    AudioProcessorGraph::AudioGraphIOProcessor* outNode;
    outNode = new AudioProcessorGraph::AudioGraphIOProcessor (AudioProcessorGraph::AudioGraphIOProcessor::audioOutputNode);
    const AudioProcessorGraph::Node* outputNode =  graph.addNode (outNode);
    internalNodeIds.add (outputNode->nodeId);
	setNodePosition(internalNodeIds[InternalNodes::AudioOutput], .8, .8);

    setupAudioDevices (preferredDefaultDeviceName, preferredSetupOptions);
    startPlaying();
}

AudioGraph::~AudioGraph()
{
    deletePlugin();
    shutDownAudioDevices();
}

//==============================================================================


void AudioGraph::addPlugin (File inputFile, int32 nodeId)
{
	
    for ( int i = 0 ; i < graph.getNumNodes() ; i++)
	if (graph.getNode (i)->nodeId == nodeId)
	{
		XmlElement* xml = this->createXml();
		xml->writeToFile(File("/home/rory/Desktop/test.xml"), "");
		this->restoreFromXml(*xml);
		return;
	}
	
	createNode(nodeId, inputFile);
	
	setDefaultConnections(nodeId);

}

void AudioGraph::createNode(int32 nodeId, File inputFile)
{
	AudioProcessor* processor;		
		
    isCabbageFile = CabbageUtilities::hasCabbageTags (inputFile);

    if (isCabbageFile)
        processor = createCabbagePluginFilter (inputFile);
    else
        processor = createGenericPluginFilter (inputFile);

    AudioProcessor::setTypeOfNextNewPlugin (AudioProcessor::wrapperType_Undefined);

    jassert (processor != nullptr); 

    processor->disableNonMainBuses();
    processor->setRateAndBufferSizeDetails (44100, 512);
	graph.addNode (processor, nodeId);
	
	String name  = "Csound";
	if(CabbagePluginProcessor* plugin = dynamic_cast<CabbagePluginProcessor*>(processor))
		name = plugin->getPluginName();
	
	fillPluginDescriptor(nodeId, inputFile, name, isCabbageFile == true ? "CabbagePlugin" : "CsoundPlugin"); 

}

void AudioGraph::fillPluginDescriptor(int32 nodeId, File inputFile, String name, String pluginType)
{
	PluginDescription descript;
	descript.fileOrIdentifier = inputFile.getFileNameWithoutExtension();;
	descript.descriptiveName = pluginType+":"+inputFile.getFileNameWithoutExtension();
	descript.name = inputFile.getFileNameWithoutExtension();
	descript.manufacturerName = "CabbageAudio";
	descript.pluginFormatName = "Cabbage";
	ScopedPointer<XmlElement> xmlElem;
	xmlElem = descript.createXml();
	String xmlInfo = xmlElem->createDocument("");
	graph.getNodeForId(nodeId)->properties.set("pluginType", pluginType);
	graph.getNodeForId(nodeId)->properties.set("pluginName", name);
	graph.getNodeForId(nodeId)->properties.set("pluginDesc", xmlInfo);		
}

void AudioGraph::setDefaultConnections(int32 nodeId)
{
	setNodePosition(nodeId, .7+(graph.getNumNodes()-3)*.05, .5);
	
    bool connectInput1 = graph.addConnection (internalNodeIds[InternalNodes::AudioInput], 0, nodeId, 0);
    bool connectInput2 = graph.addConnection (internalNodeIds[InternalNodes::AudioInput], 1, nodeId, 1);

    bool connection1 = graph.addConnection (nodeId, 0, internalNodeIds[InternalNodes::AudioOutput], 0);
    bool connection2 = graph.addConnection (nodeId, 1, internalNodeIds[InternalNodes::AudioOutput], 1);

    bool connection3 = graph.addConnection (internalNodeIds[InternalNodes::MIDIInput], AudioProcessorGraph::midiChannelIndex, nodeId, AudioProcessorGraph::midiChannelIndex);

    if (connection1 == false || connection2 == false || connection3 == false || connectInput1 == false || connectInput2 == false)
        jassertfalse;	
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
String AudioGraph::getCsoundOutput(int32 nodeId)
{
	
    if (graph.getNodeForId(nodeId)->getProcessor() != nullptr)
    {
        if (isCabbageFile)
            return dynamic_cast<CabbagePluginProcessor*> (graph.getNodeForId(nodeId)->getProcessor())->getCsoundOutput();
        else
            return dynamic_cast<GenericCabbagePluginProcessor*> (graph.getNodeForId(nodeId)->getProcessor())->getCsoundOutput();
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

            const String& layout = e->getStringAttribute("layout");

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

    if(AudioPluginInstance* plugin = dynamic_cast <AudioPluginInstance*> (node->getProcessor()))
        plugin->fillInPluginDescription (pd);
    else if(dynamic_cast <CabbagePluginProcessor*> (node->getProcessor())||
            dynamic_cast <CsoundPluginProcessor*> (node->getProcessor()))
    {
        //grab description of native plugin for saving...
        String xmlPluginDescriptor = node->properties.getWithDefault("pluginDesc", "").toString();
        //cUtils::debug(xmlPluginDescriptor);
        XmlElement* xmlElem;
        xmlElem = XmlDocument::parse(xmlPluginDescriptor);
        pd.loadFromXml(*xmlElem);
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

void AudioGraph::createNodeFromXml (const XmlElement& xml)
{
    PluginDescription pd;

    forEachXmlChildElement (xml, e)
    {
        if (pd.loadFromXml (*e))
            break;
    }

    String errorMessage;

    AudioPluginInstance* instance = formatManager->createPluginInstance (pd, graph.getSampleRate(), graph.getBlockSize(), errorMessage);

    if (instance == nullptr)
        return;

    if (const XmlElement* const layoutEntity = xml.getChildByName ("LAYOUT"))
    {
        AudioProcessor::BusesLayout layout = instance->getBusesLayout();

        for (bool isInput : { true, false })
            readBusLayoutFromXml (layout, instance, *layoutEntity, isInput);

        instance->setBusesLayout (layout);
    }

    AudioProcessorGraph::Node::Ptr node (graph.addNode (instance, (uint32) xml.getIntAttribute ("uid")));

    if (const XmlElement* const state = xml.getChildByName ("STATE"))
    {
        MemoryBlock m;
        m.fromBase64Encoding (state->getAllSubText());

        node->getProcessor()->setStateInformation (m.getData(), (int) m.getSize());
    }

    node->properties.set ("x", xml.getDoubleAttribute ("x"));
    node->properties.set ("y", xml.getDoubleAttribute ("y"));

    for (int i = 0; i < PluginWindow::NumTypes; ++i)
    {
        PluginWindow::WindowFormatType type = (PluginWindow::WindowFormatType) i;

//        if (xml.hasAttribute (getOpenProp (type)))
//        {
//            node->properties.set (getLastXProp (type), xml.getIntAttribute (getLastXProp (type)));
//            node->properties.set (getLastYProp (type), xml.getIntAttribute (getLastYProp (type)));
//            node->properties.set (getOpenProp (type), xml.getIntAttribute (getOpenProp (type)));
//
//            if (node->properties[getOpenProp (type)])
//            {
//                jassert (node->getProcessor() != nullptr);
//
//                if (PluginWindow* const w = PluginWindow::getWindowFor (node, type, graph))
//                    w->toFront (true);
//            }
//        }
    }
}

XmlElement* AudioGraph::createXml() const
{
    XmlElement* xml = new XmlElement ("FILTERGRAPH");

    for (int i = 0; i < graph.getNumNodes(); ++i)
        xml->addChildElement (createNodeXml (graph.getNode (i)));

    for (int i = 0; i < graph.getNumConnections(); ++i)
    {
        const AudioProcessorGraph::Connection* const fc = graph.getConnection(i);

        XmlElement* e = new XmlElement ("CONNECTION");

        e->setAttribute ("srcFilter", (int) fc->sourceNodeId);
        e->setAttribute ("srcChannel", fc->sourceChannelIndex);
        e->setAttribute ("dstFilter", (int) fc->destNodeId);
        e->setAttribute ("dstChannel", fc->destChannelIndex);

        xml->addChildElement (e);
    }

    return xml;
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

    setAlwaysOnTop (true);
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

        Component dummyModalComp;
        dummyModalComp.enterModalState();
        MessageManager::getInstance()->runDispatchLoopUntil (150);
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