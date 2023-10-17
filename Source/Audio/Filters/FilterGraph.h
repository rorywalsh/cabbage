/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2017 - ROLI Ltd.

   JUCE is an open source library subject to commercial or open-source
   licensing.

   By using JUCE, you agree to the terms of both the JUCE 5 End-User License
   Agreement and JUCE 5 Privacy Policy (both updated and effective as of the
   27th April 2017).

   End User License Agreement: www.juce.com/juce-5-licence
   Privacy Policy: www.juce.com/juce-5-privacy-policy

   Or: You may also use this code under the terms of the GPL v3 (see
   www.gnu.org/licenses).

   JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
   EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
   DISCLAIMED.

  ==============================================================================
*/

#pragma once

#include "../UI/PluginWindow.h"
#include "../../Utilities/CabbageUtilities.h"
#include "../../Settings/CabbageSettings.h"
#include "../Plugins/CabbagePluginProcessor.h"
#include "../Plugins/GenericCabbagePluginProcessor.h"



//==============================================================================
/**
    A collection of filters and some connections between them.
*/
class FilterGraph   : public FileBasedDocument,
                      public AudioProcessorListener,
                      private ChangeListener,
                      //RW
                      public HighResolutionTimer,
                      public AudioPlayHead
{

public:
    //==============================================================================
    FilterGraph (AudioPluginFormatManager&);
    ~FilterGraph() override;

    //==============================================================================
    using NodeID = AudioProcessorGraph::NodeID;

    void addPlugin (const PluginDescription&, juce::Point<double>);
	void addPluginCallback(std::unique_ptr<AudioPluginInstance>, const String& error, juce::Point<double>);



	//================================  FILTERGRAPH MODS  =================================

	enum InternalNodes
	{
		//matches the order in which these nodes are added
		AudioInput = 1,
		MIDIInput,
		AudioOutput
	};

	static const String getInstrumentName(File csdFile)
	{
		StringArray csdLines;
		csdLines.addLines(csdFile.loadFileAsString());

		for (auto line : csdLines)
		{
			ValueTree temp("temp");
			CabbageWidgetData::setWidgetState(temp, line, 0);

			if (CabbageWidgetData::getStringProp(temp, CabbageIdentifierIds::type) == CabbageWidgetTypes::form)
				return CabbageWidgetData::getStringProp(temp, CabbageIdentifierIds::caption);
		}

        return "";
	}

	static const PluginDescription getPluginDescriptor(AudioProcessorGraph::NodeID nodeId, String inputFile)
	{
		PluginDescription descript;
		descript.fileOrIdentifier = inputFile;
		descript.descriptiveName = "Cabbage:" + inputFile;
		descript.name = inputFile;
		descript.numInputChannels = 2;
		descript.numOutputChannels = 2;
		descript.isInstrument = true;
		descript.uniqueId = nodeId.uid;
		descript.manufacturerName = "CabbageAudio";
		descript.pluginFormatName = "Cabbage";

		return descript;
	}

	String getCsoundOutput(AudioProcessorGraph::NodeID nodeId)
	{
		if (graph.getNodeForId(nodeId) != nullptr &&
			graph.getNodeForId(nodeId)->getProcessor() != nullptr)
		{
			if (graph.getNodeForId(nodeId)->properties.getWithDefault("pluginType", "").toString() == "Cabbage")
				return dynamic_cast<CabbagePluginProcessor*> (graph.getNodeForId(nodeId)->getProcessor())->getCsoundOutput();
			else
				return dynamic_cast<GenericCabbagePluginProcessor*> (graph.getNodeForId(nodeId)->getProcessor())->getCsoundOutput();
		}

		return String();
	}


	bool addConnection(AudioProcessorGraph::NodeID  sourceFilterUID, int sourceFilterChannel,
		AudioProcessorGraph::NodeID  destFilterUID, int destFilterChannel)
	{
		AudioProcessorGraph::NodeAndChannel source = { (AudioProcessorGraph::NodeID) sourceFilterUID, sourceFilterChannel };
		AudioProcessorGraph::NodeAndChannel dest = { (AudioProcessorGraph::NodeID) destFilterUID, destFilterChannel };

		const bool result = graph.addConnection({ source, dest });

		if (result)
			changed();

		return result;
	}

	XmlElement* createConnectionsXml()
	{
		auto* xml = new XmlElement("FILTERGRAPH");

		for (auto* node : graph.getNodes())
			xml->addChildElement(createXmlForNode(node));

		for (auto& connection : graph.getConnections())
		{
			auto e = xml->createNewChildElement("CONNECTION");

			e->setAttribute("srcFilter", (int)connection.source.nodeID.uid);
			e->setAttribute("srcChannel", connection.source.channelIndex);
			e->setAttribute("dstFilter", (int)connection.destination.nodeID.uid);
			e->setAttribute("dstChannel", connection.destination.channelIndex);
		}

		return xml;
	}

	static XmlElement* createBusLayoutXml(const AudioProcessor::BusesLayout& layout, const bool isInput)
	{
		auto& buses = isInput ? layout.inputBuses
			: layout.outputBuses;

		auto* xml = new XmlElement(isInput ? "INPUTS" : "OUTPUTS");

		for (int busIdx = 0; busIdx < buses.size(); ++busIdx)
		{
			auto& set = buses.getReference(busIdx);

			auto* bus = xml->createNewChildElement("BUS");
			bus->setAttribute("index", busIdx);
			bus->setAttribute("layout", set.isDisabled() ? "disabled" : set.getSpeakerArrangementAsString());
		}

		return xml;
	}

	static XmlElement* createXmlForNode(AudioProcessorGraph::Node* const node) noexcept
	{
		if (dynamic_cast<AudioPluginInstance*> (node->getProcessor()) ||
			dynamic_cast <CabbagePluginProcessor*> (node->getProcessor()) ||
			dynamic_cast <CsoundPluginProcessor*> (node->getProcessor()))
		{
			auto e = new XmlElement("FILTER");
			e->setAttribute("uid", (int)node->nodeID.uid);
			e->setAttribute("x", node->properties["x"].toString());
			e->setAttribute("y", node->properties["y"].toString());
#if JUCE_WINDOWS && JUCE_WIN_PER_MONITOR_DPI_AWARE
			e->setAttribute("DPIAware", node->properties["DPIAware"].toString());
#endif

			for (int i = 0; i < (int)PluginWindow::Type::numTypes; ++i)
			{
				auto type = (PluginWindow::Type) i;

				if (node->properties.contains(PluginWindow::getOpenProp(type)))
				{
					e->setAttribute(PluginWindow::getLastXProp(type), node->properties[PluginWindow::getLastXProp(type)].toString());
					e->setAttribute(PluginWindow::getLastYProp(type), node->properties[PluginWindow::getLastYProp(type)].toString());
					e->setAttribute(PluginWindow::getOpenProp(type), node->properties[PluginWindow::getOpenProp(type)].toString());
				}
			}

			{
				PluginDescription pd;

				if (auto* plugin = dynamic_cast <AudioPluginInstance*> (node->getProcessor()))
					plugin->fillInPluginDescription(pd);
				else if (auto* plugin2 = dynamic_cast <CabbagePluginProcessor*> (node->getProcessor()))
				{
					//grab description of native plugin for saving...
					pd = getPluginDescriptor(node->nodeID, node->properties.getWithDefault("pluginFile", ""));
				}
				
				e->addChildElement(pd.createXml().release());
			}

			{
				MemoryBlock m;
				node->getProcessor()->getStateInformation(m);
				e->createNewChildElement("STATE")->addTextElement(m.toBase64Encoding());
			}

			auto layout = node->getProcessor()->getBusesLayout();

			auto layouts = e->createNewChildElement("LAYOUT");
			layouts->addChildElement(createBusLayoutXml(layout, true));
			layouts->addChildElement(createBusLayoutXml(layout, false));

			return e;
		}

		jassertfalse;
		return nullptr;
	}
	
	void restoreConnectionsFromXml(const XmlElement& xml)
	{
		forEachXmlChildElementWithTagName(xml, e, "CONNECTION")
		{
			addConnection((AudioProcessorGraph::NodeID) e->getIntAttribute("srcFilter"),
				e->getIntAttribute("srcChannel"),
				(AudioProcessorGraph::NodeID) e->getIntAttribute("dstFilter"),
				e->getIntAttribute("dstChannel"));
		}

		graph.removeIllegalConnections();
	}

	std::unique_ptr < AudioProcessor> createCabbageProcessor(const String filename)
	{
		std::unique_ptr < AudioProcessor> processor;

        if(File(filename).existsAsFile() == false)
            return nullptr;
        
		const bool isCabbageFile = CabbageUtilities::hasCabbageTags(File(filename));
        StringArray csdLines;
        csdLines.addLines(File(filename).loadFileAsString());
        int sideChainChannels = 0;
        for (auto line : csdLines)
        {
            ValueTree temp("temp");
            CabbageWidgetData::setWidgetState(temp, line, 0);
            
            if (CabbageWidgetData::getStringProp(temp, CabbageIdentifierIds::type) == CabbageWidgetTypes::form)
            {
                sideChainChannels = CabbageWidgetData::getProperty(temp, CabbageIdentifierIds::sidechain);
                break;
            }
            
        }
        


        const String csdString = File(filename).loadFileAsString();
        const int numOutChannels = CabbageUtilities::getHeaderInfo(csdString, "nchnls");
        int numInChannels = numOutChannels;
        if (CabbageUtilities::getHeaderInfo(csdString, "nchnls_i") != -1 && CabbageUtilities::getHeaderInfo(csdString, "nchnls_i") != 0)
            numInChannels = CabbageUtilities::getHeaderInfo(csdString, "nchnls_i") - sideChainChannels;

        if(isCabbageFile == false)
        {
                    //GenericCabbagePluginProcessor*
                processor = std::unique_ptr<GenericCabbagePluginProcessor>(new GenericCabbagePluginProcessor(filename, CabbagePluginProcessor::readBusesPropertiesFromXml(filename)));
        }
        else
        {
            if (sideChainChannels != 0)
                processor = std::unique_ptr<CabbagePluginProcessor>(new CabbagePluginProcessor(filename, CabbagePluginProcessor::readBusesPropertiesFromXml(filename)));
            else
                processor = std::unique_ptr<CabbagePluginProcessor>(new CabbagePluginProcessor(filename, CabbagePluginProcessor::readBusesPropertiesFromXml(filename)));
        }
		AudioProcessor::setTypeOfNextNewPlugin(AudioProcessor::wrapperType_Undefined);
		jassert(processor != nullptr);
		//processor->disableNonMainBuses();
		processor->setRateAndBufferSizeDetails(graph.getSampleRate(), 512);

		return processor;
	}

    //RW
    CabbageSettings* settings;
	int currentBPM = 60;
	float PPQN = 24;
	double ppqPosition = 1;
	double subTicks = 0;
	double timeInSeconds = 0;
	bool isPlaying = false;
	bool isLooping = false;
	bool isRecording = false;

	juce::Optional<juce::AudioPlayHead::PositionInfo> juce::AudioPlayHead::getPosition(void) const {
		return {};
	}

	void bringAllPluginWindowsToFront()
	{
		for (auto* w : activePluginWindows)
			w->toFront(true);
	}

    bool getCurrentPosition (CurrentPositionInfo &result)
    {
        result = playHeadPositionInfo;
        return true;
    }

    void setPlayHeadInfo(CurrentPositionInfo info)
    {
        playHeadPositionInfo = info;
    }

    void setIsPlaying(bool val)
    {
        playHeadPositionInfo.isPlaying=val;
    }

    void setIsRecording(bool val)
    {
        playHeadPositionInfo.isRecording=val;
    }

    void setTimeInSeconds(double val)
    {
        playHeadPositionInfo.timeInSeconds=val;
    }

    double getTimeInSeconds()
    {
        return playHeadPositionInfo.timeInSeconds;
    }

    void setPPQPosition(double val)
    {
        playHeadPositionInfo.ppqPosition=val;
    }
    int getPPQPosition()
    {
        return playHeadPositionInfo.ppqPosition;
    }

    void setBpm(int bpm)
    {
        playHeadPositionInfo.bpm = bpm;
    }

	void setBPM(int bpm)
	{
		currentBPM = bpm;
		setBpm(bpm);

		if(isTimerRunning())
		{
			stopTimer();
			startTimer(10);
		}
	}

	void hiResTimerCallback() override
	{
		timeInSeconds+=(getTimerInterval()/1000.f);
		setTimeInSeconds(timeInSeconds);
        ppqPosition+=((float)getTimerInterval()/(1000.f)*(currentBPM/60));
		setPPQPosition(ppqPosition);
        playHeadPositionInfo.timeInSamples = timeInSeconds*graph.getSampleRate();
		subTicks = (subTicks >= 1 ? 0 : (subTicks+(getTimerInterval()*0.001)*currentBPM/60));
	}

	void setIsHostPlaying(bool value, bool reset)
	{
		setIsPlaying(value);
		if(value == true)
		{
			startTimer(10);
		}
		else
			stopTimer();

		if(reset==true)
		{
			timeInSeconds=0;
			setPPQPosition(0);
			setTimeInSeconds(0);
			ppqPosition=0;
			timeInSeconds=0;

		}
	}

    void setCabbageSettings(CabbageSettings* cabbageSettings)
    {
        settings = cabbageSettings;
    }

	void addCabbagePlugin(const PluginDescription& desc, juce::Point<double> pos)
	{
		AudioProcessorGraph::NodeID nodeId(desc.uniqueId);
		std::unique_ptr <AudioProcessor> processor = createCabbageProcessor(desc.fileOrIdentifier);
		const bool isCabbageFile = CabbageUtilities::hasCabbageTags(File(desc.fileOrIdentifier));

		if (auto* plugin = graph.getNodeForId(nodeId))
		{
			plugin->getProcessor()->suspendProcessing(true);
			auto pluginProcessor = dynamic_cast<CsoundPluginProcessor*>(plugin->getProcessor());
			if (pluginProcessor)
			{
				pluginProcessor->resetCsound();
			}

			std::unique_ptr<XmlElement> xml (createConnectionsXml());
			graph.disconnectNode(nodeId);
			plugin->getProcessor()->editorBeingDeleted(plugin->getProcessor()->getActiveEditor());
			
            graph.removeNode(nodeId);
			graph.releaseResources();

			// Reset Csound so the Csound resources for this plugin are released (e.g. OSC ports).
			// 
			// Normally this is done in the plugin processor destructor when Juce cleans up dead graph nodes, but the
			// new plugin processor instance will start Csound before the old one is destroyed, so we need to release
			// Csound resources here instead of waiting until later.


			if (auto node = graph.addNode(std::move(processor), nodeId))
			{
				node->properties.set("pluginFile", desc.fileOrIdentifier);
				node->properties.set("pluginType", isCabbageFile == true ? "Cabbage" : "Csound");
				node->properties.set("pluginName", getInstrumentName(File(desc.fileOrIdentifier)));
				setNodePosition(nodeId, juce::Point<double>(pos.x, pos.y));
				node->getProcessor()->setPlayHead(graph.getPlayHead());
				restoreConnectionsFromXml(*xml);
				xml = nullptr;
				changed();
                graph.prepareToPlay(graph.getSampleRate(), graph.getBlockSize());

#if JUCE_WINDOWS && JUCE_WIN_PER_MONITOR_DPI_AWARE
				node->properties.set("DPIAware", true);
#endif
			}
		}

		else
		{
			if (auto node = graph.addNode(std::move(processor), nodeId))
			{

				setNodePosition(nodeId, pos);
				changed();
				std::unique_ptr<XmlElement> xmlElem;
				xmlElem = desc.createXml();
				node->properties.set("pluginFile", desc.fileOrIdentifier);
				node->properties.set("pluginType", isCabbageFile == true ? "Cabbage" : "Csound");
				node->properties.set("pluginName", getInstrumentName(File(desc.fileOrIdentifier)));
				node->getProcessor()->setPlayHead(graph.getPlayHead());

#if JUCE_WINDOWS && JUCE_WIN_PER_MONITOR_DPI_AWARE
				node->properties.set("DPIAware", true);
#endif
			}
		}

		if(settings->getUserSettings()->getIntValue("autoConnectNodes")==1)
		   setDefaultConnections(nodeId);

	}


	juce::Point<int> getPositionOfCurrentlyOpenWindow(AudioProcessorGraph::NodeID  nodeId)
	{
        ignoreUnused(nodeId);
		for (auto* w : activePluginWindows)
			return w->getPosition();

		return juce::Point<int>(-1000, -1000);
	}

	void setDefaultConnections(AudioProcessorGraph::NodeID nodeId)
	{
		AudioProcessorGraph::NodeAndChannel inputL = { (AudioProcessorGraph::NodeID) InternalNodes::AudioInput, 0 };
		AudioProcessorGraph::NodeAndChannel inputR = { (AudioProcessorGraph::NodeID) InternalNodes::AudioInput, 1 };

		AudioProcessorGraph::NodeAndChannel nodeL = { (AudioProcessorGraph::NodeID) nodeId, 0 };
		AudioProcessorGraph::NodeAndChannel nodeR = { (AudioProcessorGraph::NodeID) nodeId, 1 };
        
		AudioProcessorGraph::NodeAndChannel outputL = { (AudioProcessorGraph::NodeID) InternalNodes::AudioOutput, 0 };
		AudioProcessorGraph::NodeAndChannel outputR = { (AudioProcessorGraph::NodeID) InternalNodes::AudioOutput, 1 };
        
        Array<AudioProcessorGraph::NodeAndChannel> extraHardwareOutputs;
        Array<AudioProcessorGraph::NodeAndChannel> extraPluginOutputs;
        
        // look for extra channels to make connections for, the default is stereo...
        const int numOutputChannels = graph.getNodeForId(nodeId)->getProcessor()->getTotalNumOutputChannels();
        for(int i = 0 ; i < numOutputChannels ; i++)
        {
            extraPluginOutputs.add({ (AudioProcessorGraph::NodeID) nodeId, i });
            extraHardwareOutputs.add({ (AudioProcessorGraph::NodeID) InternalNodes::AudioOutput, i });
        }

        
     
        // make default connections, stereo in, stereo out
		/* bool connectInput1 = */ graph.addConnection({ inputL, nodeL });
		/* bool connectInput2 = */ graph.addConnection({ inputR, nodeR });

		/* bool connection1 = */ graph.addConnection({ nodeL, outputL });
		/* bool connection2 = */ graph.addConnection({ nodeR, outputR });
        
		//now add any extra channels..
        for ( int i = 0 ; i < numOutputChannels ; i++)
        {
            graph.addConnection({ extraPluginOutputs[i], extraHardwareOutputs[i] });
        }
        
        
		AudioProcessorGraph::NodeAndChannel midiIn = { (AudioProcessorGraph::NodeID) InternalNodes::MIDIInput, AudioProcessorGraph::midiChannelIndex };
		AudioProcessorGraph::NodeAndChannel midiOut = { (AudioProcessorGraph::NodeID) nodeId, AudioProcessorGraph::midiChannelIndex };

		/* bool connection3 = */ graph.addConnection({ midiIn, midiOut });

	}
	//======================================================================================

    AudioProcessorGraph::Node::Ptr getNodeForName (const String& name) const;

    void setNodePosition (NodeID, juce::Point<double>);
    juce::Point<double> getNodePosition (NodeID) const;

    //==============================================================================
    void clear();

    PluginWindow* getOrCreateWindowFor (AudioProcessorGraph::Node*, PluginWindow::Type);
    bool closeAnyOpenPluginWindows();

    //==============================================================================
    void audioProcessorParameterChanged (AudioProcessor*, int, float) override {}
    void audioProcessorChanged (AudioProcessor*, const ChangeDetails &details) override { changed(); }

    //==============================================================================
    XmlElement* createXml() const;
    void restoreFromXml (const XmlElement& xml);

    static const char* getFilenameSuffix()      { return ".filtergraph"; }
    static const char* getFilenameWildcard()    { return "*.filtergraph"; }

    //==============================================================================
    void newDocument();
    String getDocumentTitle() override;
    Result loadDocument (const File& file) override;
    Result saveDocument (const File& file) override;
    File getLastDocumentOpened() override;
    void setLastDocumentOpened (const File& file) override;
	File currentFile;
    static File getDefaultGraphDocumentOnMobile();

    //==============================================================================
    AudioProcessorGraph graph;
	OwnedArray<PluginWindow> activePluginWindows;
private:
    //==============================================================================
	AudioPluginFormatManager& formatManager;

    NodeID lastUID;
    NodeID getNextUID() noexcept;
//RW
    AudioPlayHead::CurrentPositionInfo playHeadPositionInfo;
    void createNodeFromXml (const XmlElement& xml);
    void addFilterCallback (AudioPluginInstance*, const String& error, juce::Point<double>);
    void changeListenerCallback (ChangeBroadcaster*) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterGraph)
};
