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

AudioGraph::AudioGraph(PropertySet* settingsToUse, File inputFile,
                       bool takeOwnershipOfSettings,
                       const String& preferredDefaultDeviceName,
                       const AudioDeviceManager::AudioDeviceSetup* preferredSetupOptions)

: settings (settingsToUse, takeOwnershipOfSettings), graph(), processor(nullptr)
{
    graph.prepareToPlay(44100, 512);
    graph.setPlayConfigDetails(2, 2, 44100, 512);
	if(inputFile.existsAsFile())
		createPlugin(inputFile);
		
    setupAudioDevices (preferredDefaultDeviceName, preferredSetupOptions);
    reloadPluginState();
    startPlaying();
}

AudioGraph::~AudioGraph()
{
    deletePlugin();
    shutDownAudioDevices();
}

//==============================================================================
void AudioGraph::createPlugin(File inputFile)
{
    AudioProcessorGraph::AudioGraphIOProcessor* outNode;
    outNode = new AudioProcessorGraph::AudioGraphIOProcessor(AudioProcessorGraph::AudioGraphIOProcessor::audioOutputNode);
    AudioProcessorGraph::Node* outputNode = graph.addNode(outNode, NodeType::Input);

    AudioProcessorGraph::AudioGraphIOProcessor* midiNode;
    midiNode = new AudioProcessorGraph::AudioGraphIOProcessor(AudioProcessorGraph::AudioGraphIOProcessor::midiInputNode);
    AudioProcessorGraph::Node* midiInputNode = graph.addNode(midiNode, NodeType::MidiInput);


    AudioProcessor::setTypeOfNextNewPlugin (AudioProcessor::wrapperType_Standalone);
    isCabbageFile = CabbageUtilities::hasCabbageTags(inputFile);
    if(isCabbageFile)
        processor = createCabbagePluginFilter(inputFile);
    else
        processor = createGenericPluginFilter(inputFile);

    AudioProcessor::setTypeOfNextNewPlugin (AudioProcessor::wrapperType_Undefined);


    jassert (processor != nullptr); // Your createPluginFilter() function must return a valid object!


    processor->disableNonMainBuses();
    processor->setRateAndBufferSizeDetails(44100, 512);

    updateBusLayout(processor);


    AudioProcessorGraph::Node* processorNode = graph.addNode(processor, NodeType::CabbageNode);

    bool connection1 = graph.addConnection(NodeType::CabbageNode, 0, NodeType::Input, 0);
    bool connection2 = graph.addConnection(NodeType::CabbageNode, 1, NodeType::Input, 1);
    bool connection3 = graph.addConnection(NodeType::MidiInput, AudioProcessorGraph::midiChannelIndex, NodeType::CabbageNode, AudioProcessorGraph::midiChannelIndex);

    if(connection1 == false || connection2 == false || connection3 == false)
        jassert(false);
}


void AudioGraph::updateBusLayout(AudioProcessor* selectedProcessor)
{
    if (AudioProcessor* filter = selectedProcessor)
    {
        if (AudioProcessor::Bus* bus = filter->getBus (isInput, currentBus))
        {
            bool test = bus->setNumberOfChannels(8);
        }
    }
}

int AudioGraph::getNumberOfParameters()
{
    return getProcessor()->getParameters().size();
}

void AudioGraph::deletePlugin()
{
    PluginWindow::closeAllCurrentlyOpenWindows();
    stopPlaying();
    graph.clear();
    processor = nullptr;
}

String AudioGraph::getFilePatterns (const String& fileSuffix)
{
    if (fileSuffix.isEmpty())
        return String();

    return (fileSuffix.startsWithChar ('.') ? "*" : "*.") + fileSuffix;
}

void AudioGraph::setXmlAudioSettings(XmlElement* xmlSettingsString)
{
    xmlSettings = xmlSettingsString;
    setupAudioDevices ( String(), nullptr);
    startPlaying();
}

AudioDeviceSelectorComponent* AudioGraph::getAudioDeviceSelector()
{
    const int numInputs = processor!=nullptr ? processor->getTotalNumInputChannels() : 2;
    const int numOutputs = processor!=nullptr ? processor->getTotalNumOutputChannels() : 2;
    
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
    if(deviceManager.getCurrentAudioDevice())
    {
        ScopedPointer<XmlElement> xml (deviceManager.createStateXml());
        if(xml==nullptr)
            return String::empty;
        else
            return xml->createDocument("");
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
String AudioGraph::getCsoundOutput()
{
	if(getProcessor() != nullptr)
	{
		if(isCabbageFile)
			return dynamic_cast<CabbagePluginProcessor*>(getProcessor())->getCsoundOutput();
		else
			return dynamic_cast<GenericCabbagePluginProcessor*>(getProcessor())->getCsoundOutput();
	}
	
	return String::empty;
}

//==============================================================================
void AudioGraph::savePluginState()
{
    if (settings != nullptr && processor != nullptr)
    {
        MemoryBlock data;
        processor->getStateInformation (data);

        settings->setValue ("filterState", data.toBase64Encoding());
    }
}

void AudioGraph::reloadPluginState()
{
    if (settings != nullptr)
    {
        MemoryBlock data;

        if (data.fromBase64Encoding (settings->getValue ("filterState")) && data.getSize() > 0)
            processor->setStateInformation (data.getData(), (int) data.getSize());
    }
}

AudioProcessor* AudioGraph::getProcessor()
{
    return processor;
}

//========================================================================================

static Array <PluginWindow*> activePluginWindows;

PluginWindow::PluginWindow (Component* const pluginEditor,
                            AudioProcessorGraph::Node* const o,
                            WindowFormatType t,
                            AudioProcessorGraph& audioGraph)
    : DocumentWindow (pluginEditor->getName(), Colours::lightgrey,
                      DocumentWindow::minimiseButton | DocumentWindow::closeButton),
      graph (audioGraph),
      owner (o),
      type (t)
{
    setSize (400, 300);
    setContentOwned (pluginEditor, true);
    setVisible (true);

    setAlwaysOnTop(true);
    activePluginWindows.add (this);
}

void PluginWindow::closeCurrentlyOpenWindowsFor (const uint32 nodeId)
{
    for (int i = activePluginWindows.size(); --i >= 0;)
        if (activePluginWindows.getUnchecked(i)->owner->nodeId == nodeId)
            delete activePluginWindows.getUnchecked (i);
}

Point<int> PluginWindow::getPositionOfCurrentlyOpenWindow(const uint32 nodeId)
{
    for (int i = activePluginWindows.size(); --i >= 0;)
        if (activePluginWindows.getUnchecked(i)->owner->nodeId == nodeId)
            return Point<int>(activePluginWindows.getUnchecked(i)->getX(), activePluginWindows.getUnchecked(i)->getY());
    return Point<int>(-1000, -1000);
}

void PluginWindow::closeAllCurrentlyOpenWindows()
{
    if (activePluginWindows.size() > 0)
    {
        for (int i = activePluginWindows.size(); --i >= 0;)
            delete activePluginWindows.getUnchecked (i);

        Component dummyModalComp;
        dummyModalComp.enterModalState();
        MessageManager::getInstance()->runDispatchLoopUntil(150);
    }
}

//==============================================================================
PluginWindow* PluginWindow::getWindowFor (AudioProcessorGraph::Node* const node,
        WindowFormatType type,
        AudioProcessorGraph& audioGraph)
{
    jassert (node != nullptr);

    for (int i = activePluginWindows.size(); --i >= 0;)
        if (activePluginWindows.getUnchecked(i)->owner == node
                && activePluginWindows.getUnchecked(i)->type == type)
            return activePluginWindows.getUnchecked(i);

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