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

#include "AudioGraph.h"


//==============================================================================
class PluginWindow;


AudioGraph::AudioGraph(PropertySet* settingsToUse, File inputFile,
						bool takeOwnershipOfSettings,
						const String& preferredDefaultDeviceName,
						const AudioDeviceManager::AudioDeviceSetup* preferredSetupOptions)

	: settings (settingsToUse, takeOwnershipOfSettings), graph()
{
	graph.prepareToPlay(44100, 512);
	graph.setPlayConfigDetails(2, 2, 44100, 512);
	createPlugin(inputFile);
	setupAudioDevices (preferredDefaultDeviceName, preferredSetupOptions);
	reloadPluginState();
	startPlaying();
	
	bool connection1 = graph.addConnection(1, 0, 2, 0);			
	bool connection2 = graph.addConnection(1, 1, 2, 1);	
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
	AudioProcessorGraph::Node* outputNode = graph.addNode(outNode, 2);


	AudioProcessor::setTypeOfNextNewPlugin (AudioProcessor::wrapperType_Standalone);
	bool isCabbageFile = CabbageUtilities::hasCabbageTags(inputFile);
	if(isCabbageFile)
		processor = createCabbagePluginFilter(inputFile);
	else
		processor = createGenericPluginFilter(inputFile);

	AudioProcessor::setTypeOfNextNewPlugin (AudioProcessor::wrapperType_Undefined);


	jassert (processor != nullptr); // Your createPluginFilter() function must return a valid object!

	
	processor->disableNonMainBuses();
	processor->setRateAndBufferSizeDetails(44100, 512);
	
	updateBusLayout(processor);
	
	AudioProcessorGraph::Node* processorNode = graph.addNode(processor, 1);
	

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
	return new AudioDeviceSelectorComponent (deviceManager,
														  processor->getTotalNumInputChannels(),
														  processor->getTotalNumInputChannels(),
														  processor->getTotalNumOutputChannels(),
														  processor->getTotalNumOutputChannels(),
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
	return dynamic_cast<CabbagePluginProcessor*>(getProcessor())->getCsoundOutput();
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















static Array <PluginWindow*> activePluginWindows;

PluginWindow::PluginWindow (Component* const pluginEditor,
                            AudioProcessorGraph::Node* const o,
                            WindowFormatType t,
                            AudioProcessorGraph& audioGraph)
    : DocumentWindow (pluginEditor->getName(), Colours::lightblue,
                      DocumentWindow::minimiseButton | DocumentWindow::closeButton),
      graph (audioGraph),
      owner (o),
      type (t)
{
    setSize (400, 300);
    setContentOwned (pluginEditor, true);
    setTopLeftPosition (Random::getSystemRandom().nextInt (500), Random::getSystemRandom().nextInt (500));   
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

void PluginWindow::closeAllCurrentlyOpenWindows()
{
    if (activePluginWindows.size() > 0)
    {
        for (int i = activePluginWindows.size(); --i >= 0;)
            delete activePluginWindows.getUnchecked (i);

        Component dummyModalComp;
        dummyModalComp.enterModalState();
        MessageManager::getInstance()->runDispatchLoopUntil (50);
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