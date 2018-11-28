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
	void addCabbagePlugin(const PluginDescription& desc, Point<double> pos)
	{
		AudioProcessor* processor;
		bool isCabbageFile = CabbageUtilities::hasCabbageTags(File(desc.fileOrIdentifier));
		const int numChannels = CabbageUtilities::getHeaderInfo(desc.fileOrIdentifier, "nchnls");

		if (isCabbageFile)
			processor = new CabbagePluginProcessor(File(desc.fileOrIdentifier), numChannels, numChannels);
		else
			processor = new GenericCabbagePluginProcessor(File(desc.fileOrIdentifier), numChannels, numChannels);

		AudioProcessor::setTypeOfNextNewPlugin(AudioProcessor::wrapperType_Undefined);
		jassert(processor != nullptr);
		processor->disableNonMainBuses();
		processor->setRateAndBufferSizeDetails(44100, 512);
		AudioProcessorGraph::NodeID nodeId(desc.uid);
		if (auto node = graph.addNode(processor, nodeId))
		{
			node->properties.set("x", pos.x);
			node->properties.set("y", pos.y);
			changed();
		}
	}

	Point<int> getPositionOfCurrentlyOpenWindow(AudioProcessorGraph::NodeID  nodeId)
	{
		for (auto* w : activePluginWindows)
			CabbageUtilities::debug("Hell");

		return Point<int>(-1000, -1000);
	}

	PluginWindow* getOrCreateWindowForCabbagePlugin(AudioProcessorGraph::Node* node, PluginWindow::Type type)
	{
		jassert(node != nullptr);

#if JUCE_IOS || JUCE_ANDROID
		closeAnyOpenPluginWindows();
#else
		for (auto* w : activePluginWindows)
			if (w->node.get() == node && w->type == type)
				return w;
#endif

		if (auto* processor = node->getProcessor())
		{
			if (auto* plugin = dynamic_cast<AudioPluginInstance*> (processor))
			{
				auto description = plugin->getPluginDescription();

				if (description.pluginFormatName == "Internal")
				{
					// getCommandManager().invokeDirectly (CommandIDs::showAudioSettings, false);
					return nullptr;
				}
			}

#if JUCE_WINDOWS && JUCE_WIN_PER_MONITOR_DPI_AWARE
			if (!node->properties["DPIAware"]
				&& !node->getProcessor()->getName().contains("Kontakt")) // Kontakt doesn't behave correctly in DPI unaware mode...
			{
				ScopedDPIAwarenessDisabler disableDPIAwareness;
				return activePluginWindows.add(new PluginWindow(node, type, activePluginWindows));
			}
#endif

			return activePluginWindows.add(new PluginWindow(node, type, activePluginWindows));
		}

		return nullptr;
	}

	AudioProcessorGraph::Node::Ptr FilterGraph::getNodeForId(AudioProcessorGraph::NodeID nodeId) const
	{
		if (auto* n = graph.getNodeForId(nodeId))
			return n;

		return nullptr;
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
