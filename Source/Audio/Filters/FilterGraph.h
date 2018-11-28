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
#include "../Plugins/CabbagePluginProcessor.h"
#include "../Plugins/GenericCabbagePluginProcessor.h"

//==============================================================================
/**
    A collection of filters and some connections between them.
*/
class FilterGraph   : public FileBasedDocument,
                      public AudioProcessorListener,
                      private ChangeListener
{

public:
    //==============================================================================
    FilterGraph (AudioPluginFormatManager&);
    ~FilterGraph();

    //==============================================================================
    using NodeID = AudioProcessorGraph::NodeID;

    void addPlugin (const PluginDescription&, Point<double>);


	//================================  FILTERGRAPH MODS  =================================

	enum InternalNodes
	{
		//matches the order in which these nodes are added
		AudioInput = 1,
		MIDIInput,
		AudioOutput
	};

	//==============================================================================
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

		return String::empty;
	}

	XmlElement* createConnectionsXml()
	{
		auto* xml = new XmlElement("FILTERGRAPH");

		for (auto* node : graph.getNodes())
			xml->addChildElement(createNodeXml(node));

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

	static XmlElement* createNodeXml(AudioProcessorGraph::Node* const node) noexcept
	{
		PluginDescription pd;

		if (AudioPluginInstance* plugin = dynamic_cast <AudioPluginInstance*> (node->getProcessor()))
			plugin->fillInPluginDescription(pd);
		else if (dynamic_cast <CabbagePluginProcessor*> (node->getProcessor()) ||
			dynamic_cast <CsoundPluginProcessor*> (node->getProcessor()))
		{
			//grab description of native plugin for saving...
			String xmlPluginDescriptor = node->properties.getWithDefault("pluginDesc", "").toString();
			//cUtils::debug(xmlPluginDescriptor);
			XmlElement* xmlElem;
			xmlElem = XmlDocument::parse(xmlPluginDescriptor);
			pd.loadFromXml(*xmlElem);
		}


		XmlElement* e = new XmlElement("FILTER");
		e->setAttribute("uid", (int)node->nodeID.uid);
		e->setAttribute("x", node->properties["x"].toString());
		e->setAttribute("y", node->properties["y"].toString());
		e->setAttribute("uiLastX", node->properties["uiLastX"].toString());
		e->setAttribute("uiLastY", node->properties["uiLastY"].toString());
		e->addChildElement(pd.createXml());

		XmlElement* state = new XmlElement("STATE");

		MemoryBlock m;
		node->getProcessor()->getStateInformation(m);
		state->addTextElement(m.toBase64Encoding());
		e->addChildElement(state);

		return e;
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

	AudioProcessor* createCabbageProcessor(const String filename)
	{
		AudioProcessor* processor;
		const bool isCabbageFile = CabbageUtilities::hasCabbageTags(File(filename));
		const int numChannels = CabbageUtilities::getHeaderInfo(filename, "nchnls");

		if (isCabbageFile)
			processor = new CabbagePluginProcessor(File(filename), numChannels, numChannels);
		else
			processor = new GenericCabbagePluginProcessor(File(filename), numChannels, numChannels);

		AudioProcessor::setTypeOfNextNewPlugin(AudioProcessor::wrapperType_Undefined);
		jassert(processor != nullptr);
		processor->disableNonMainBuses();
		processor->setRateAndBufferSizeDetails(44100, 512);

		return processor;
	}

	void addCabbagePlugin(const PluginDescription& desc, Point<double> pos)
	{
		AudioProcessorGraph::NodeID nodeId(desc.uid);
		AudioProcessor* processor = createCabbageProcessor(desc.fileOrIdentifier);
		const bool isCabbageFile = CabbageUtilities::hasCabbageTags(File(desc.fileOrIdentifier));

		if (auto* plugin = graph.getNodeForId(nodeId))
		{
			ScopedPointer<XmlElement> xml = createConnectionsXml();
			graph.disconnectNode(nodeId);
			plugin->getProcessor()->editorBeingDeleted(plugin->getProcessor()->getActiveEditor());
			graph.removeNode(nodeId);
			graph.releaseResources();

			if (auto node = graph.addNode(processor, nodeId))
			{
				node->properties.set("x", pos.x);
				node->properties.set("y", pos.y);

				//createNodeFromXml(*nodeXml);
				//setNodePosition(nodeId, Point<double>(pos.getX(), pos.getY()));
				//restoreConnectionsFromXml(*xml);
				xml = nullptr;
				//pluginFiles.add(inputFile.getFullPathName());
				return;
			}
		}

		else
		{
			if (auto node = graph.addNode(processor, nodeId))
			{
				node->properties.set("x", pos.x);
				node->properties.set("y", pos.y);
				ScopedPointer<XmlElement> xmlElem;
				xmlElem = desc.createXml();
				node->properties.set("pluginFile", desc.fileOrIdentifier);
				node->properties.set("pluginType", isCabbageFile == true ? "Cabbage" : "Csound");
				node->properties.set("pluginName", "Test");
				node->properties.set("pluginDesc", xmlElem->createDocument(""));
				changed();
			}
		}

		
		

		setDefaultConnections(nodeId);
	}

	Point<int> getPositionOfCurrentlyOpenWindow(AudioProcessorGraph::NodeID  nodeId)
	{
		for (auto* w : activePluginWindows)
			CabbageUtilities::debug("Hell");

		return Point<int>(-1000, -1000);
	}

	void setDefaultConnections(AudioProcessorGraph::NodeID nodeId)
	{
		setNodePosition(nodeId, Point<double>(4 + (graph.getNumNodes() - 3)*.05, .5));
		AudioProcessorGraph::NodeAndChannel inputL = { (AudioProcessorGraph::NodeID) InternalNodes::AudioInput, 0 };
		AudioProcessorGraph::NodeAndChannel inputR = { (AudioProcessorGraph::NodeID) InternalNodes::AudioInput, 1 };

		AudioProcessorGraph::NodeAndChannel nodeL = { (AudioProcessorGraph::NodeID) nodeId, 0 };
		AudioProcessorGraph::NodeAndChannel nodeR = { (AudioProcessorGraph::NodeID) nodeId, 1 };

		AudioProcessorGraph::NodeAndChannel outputL = { (AudioProcessorGraph::NodeID) InternalNodes::AudioOutput, 0 };
		AudioProcessorGraph::NodeAndChannel outputR = { (AudioProcessorGraph::NodeID) InternalNodes::AudioOutput, 1 };

		bool connectInput1 = graph.addConnection({ inputL, nodeL });
		bool connectInput2 = graph.addConnection({ inputR, nodeR });

		bool connection1 = graph.addConnection({ nodeL, outputL });
		bool connection2 = graph.addConnection({ nodeR, outputR });
		
		AudioProcessorGraph::NodeAndChannel midiIn = { (AudioProcessorGraph::NodeID) InternalNodes::MIDIInput, AudioProcessorGraph::midiChannelIndex };
		AudioProcessorGraph::NodeAndChannel midiOut = { (AudioProcessorGraph::NodeID) nodeId, AudioProcessorGraph::midiChannelIndex };

		bool connection3 = graph.addConnection({ midiIn, midiOut });

	}
	//======================================================================================

    AudioProcessorGraph::Node::Ptr getNodeForName (const String& name) const;

    void setNodePosition (NodeID, Point<double>);
    Point<double> getNodePosition (NodeID) const;

    //==============================================================================
    void clear();

    PluginWindow* getOrCreateWindowFor (AudioProcessorGraph::Node*, PluginWindow::Type);
    bool closeAnyOpenPluginWindows();

    //==============================================================================
    void audioProcessorParameterChanged (AudioProcessor*, int, float) override {}
    void audioProcessorChanged (AudioProcessor*) override { changed(); }

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

    static File getDefaultGraphDocumentOnMobile();

    //==============================================================================
    AudioProcessorGraph graph;



private:
    //==============================================================================
    AudioPluginFormatManager& formatManager;
	OwnedArray<PluginWindow> activePluginWindows;

    NodeID lastUID;
    NodeID getNextUID() noexcept;

    void createNodeFromXml (const XmlElement& xml);
    void addFilterCallback (AudioPluginInstance*, const String& error, Point<double>);
    void changeListenerCallback (ChangeBroadcaster*) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterGraph)
};
