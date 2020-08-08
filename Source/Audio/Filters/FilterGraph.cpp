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

#include <JuceHeader.h>
//#include "../UI/MainHostWindow.h"
#include "FilterGraph.h"
#include "InternalFilters.h"
#include "../UI/GraphEditorPanel.h"
#include "../Plugins/CabbagePluginProcessor.h"

//==============================================================================
FilterGraph::FilterGraph (AudioPluginFormatManager& fm)
    : FileBasedDocument (getFilenameSuffix(),
                         getFilenameWildcard(),
                         "Load a filter graph",
                         "Save a filter graph"),
      formatManager (fm)
{

    newDocument();
    graph.addListener (this);
    graph.setPlayHead(this);
}

FilterGraph::~FilterGraph()
{
	closeAnyOpenPluginWindows();
    graph.removeListener (this);
    graph.removeChangeListener (this);
    graph.clear();
}

FilterGraph::NodeID FilterGraph::getNextUID() noexcept
{
    return FilterGraph::NodeID (++(lastUID.uid));
}

//==============================================================================
void FilterGraph::changeListenerCallback (ChangeBroadcaster*)
{
    changed();

    for (int i = activePluginWindows.size(); --i >= 0;)
        if (! graph.getNodes().contains (activePluginWindows.getUnchecked(i)->node))
            activePluginWindows.remove (i);
}

AudioProcessorGraph::Node::Ptr FilterGraph::getNodeForName (const String& name) const
{
    for (auto* node : graph.getNodes())
        if (auto p = node->getProcessor())
            if (p->getName().equalsIgnoreCase (name))
                return node;

    return nullptr;
}

void FilterGraph::addPlugin (const PluginDescription& desc, Point<double> pos)
{
	formatManager.createPluginInstanceAsync(desc,
		graph.getSampleRate(),
		graph.getBlockSize(),
		[this, pos](std::unique_ptr<AudioPluginInstance> instance, const String& error)
		{
			addPluginCallback(std::move(instance), error, pos);
		});
  
}

void FilterGraph::addPluginCallback(std::unique_ptr<AudioPluginInstance> instance,
	const String& error, Point<double> pos)
{
	if (instance == nullptr)
	{
		AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon,
			TRANS("Couldn't create plugin"),
			error);
	}
	else
	{
		instance->enableAllBuses();

		if (auto node = graph.addNode(std::move(instance)))
		{
			//RW
			node->getProcessor()->setPlayHead(graph.getPlayHead());
			//----------------------------------------------
			node->properties.set("x", pos.x);
			node->properties.set("y", pos.y);
			changed();
		}
	}
}

void FilterGraph::setNodePosition (NodeID nodeID, Point<double> pos)
{
    if (auto* n = graph.getNodeForId (nodeID))
    {
		n->properties.set ("x", jlimit (0.0, 1.0, pos.x));
        n->properties.set ("y", jlimit (0.0, 1.0, pos.y));
    }
}

Point<double> FilterGraph::getNodePosition (NodeID nodeID) const
{
    if (auto* n = graph.getNodeForId (nodeID))
        return { static_cast<double> (n->properties ["x"]),
                 static_cast<double> (n->properties ["y"]) };

    return {};
}

//==============================================================================
void FilterGraph::clear()
{
    closeAnyOpenPluginWindows();
    graph.clear();
    changed();
}

PluginWindow* FilterGraph::getOrCreateWindowFor (AudioProcessorGraph::Node* node, PluginWindow::Type type)
{
    jassert (node != nullptr);

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
        if (! node->properties["DPIAware"]
            && ! node->getProcessor()->getName().contains ("Kontakt")) // Kontakt doesn't behave correctly in DPI unaware mode...
        {
            ScopedDPIAwarenessDisabler disableDPIAwareness;
			PluginWindow* w = new PluginWindow(node, type, activePluginWindows);
			//w->setVisible(false);
            return activePluginWindows.add (w);
        }
       #endif

		PluginWindow* w = new PluginWindow(node, type, activePluginWindows);
		//w->setVisible(false);
        return activePluginWindows.add (w);
    }

    return nullptr;
}

bool FilterGraph::closeAnyOpenPluginWindows()
{
	for (auto* w : activePluginWindows)
		w->node->getProcessor()->editorBeingDeleted(w->node->getProcessor()->getActiveEditor());

    bool wasEmpty = activePluginWindows.isEmpty();
    activePluginWindows.clear();
    return ! wasEmpty;
}

//==============================================================================
String FilterGraph::getDocumentTitle()
{
    if (! getFile().exists())
        return "Unnamed";

    return getFile().getFileNameWithoutExtension();
}

void FilterGraph::newDocument()
{
    clear();
    setFile ({});

    graph.removeChangeListener (this);

    InternalPluginFormat internalFormat;

    addPlugin (internalFormat.audioInDesc,  { 0.5,  0.1 });
    addPlugin (internalFormat.midiInDesc,   { 0.25, 0.1 });
    addPlugin (internalFormat.audioOutDesc, { 0.5,  0.9 });

    MessageManager::callAsync ([this] () {
        setChangedFlag (false);
        setChangedFlag (false);
        graph.addChangeListener (this);
    } );
}

Result FilterGraph::loadDocument (const File& file)
{
	//RW
	currentFile = file;

    XmlDocument doc (file);
    std::unique_ptr<XmlElement> xml (doc.getDocumentElement());

    if (xml == nullptr || ! xml->hasTagName ("FILTERGRAPH"))
        return Result::fail ("Not a valid filter graph file");

    graph.removeChangeListener (this);
    restoreFromXml (*xml);

    MessageManager::callAsync ([this] () {
        setChangedFlag (false);
        graph.addChangeListener (this);
    } );

    setLastDocumentOpened(file);

    return Result::ok();
}

Result FilterGraph::saveDocument (const File& file)
{
	currentFile = file;
    std::unique_ptr<XmlElement> xml (createXml());

    if (! xml->writeToFile (file, {}))
        return Result::fail ("Couldn't write to the file");

    return Result::ok();
}

File FilterGraph::getLastDocumentOpened()
{
    //RecentlyOpenedFilesList recentFiles;
    //recentFiles.restoreFromString (getAppProperties().getUserSettings()
    //                                    ->getValue ("recentFilterGraphFiles"));

    //return recentFiles.getFile (0);
    if(currentFile.existsAsFile())
    	return currentFile;

	return File();
}

void FilterGraph::setLastDocumentOpened (const File& file)
{
    //RecentlyOpenedFilesList recentFiles;
    //recentFiles.restoreFromString (getAppProperties().getUserSettings()
    //                                    ->getValue ("recentFilterGraphFiles"));

    //recentFiles.addFile (file);

    //getAppProperties().getUserSettings()
    //    ->setValue ("recentFilterGraphFiles", recentFiles.toString());
    currentFile = file;
}

//==============================================================================
static void readBusLayoutFromXml (AudioProcessor::BusesLayout& busesLayout, AudioProcessor* plugin,
                                  const XmlElement& xml, const bool isInput)
{
    auto& targetBuses = (isInput ? busesLayout.inputBuses
                                 : busesLayout.outputBuses);
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
                if (! plugin->addBus (isInput))
                    return;

            for (int actualIdx = targetBuses.size() - 1; actualIdx < busIdx; ++actualIdx)
                targetBuses.add (plugin->getChannelLayoutOfBus (isInput, busIdx));

            auto layout = e->getStringAttribute ("layout");

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

//==============================================================================
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


void FilterGraph::createNodeFromXml(const XmlElement& xml)
{
	PluginDescription pd;

	forEachXmlChildElement(xml, e)
	{
		if (pd.loadFromXml(*e))
			break;
	}

	String errorMessage;

	if (auto instance = formatManager.createPluginInstance(pd, graph.getSampleRate(),
		graph.getBlockSize(), errorMessage))
	{
		if (auto* layoutEntity = xml.getChildByName("LAYOUT"))
		{
			auto layout = instance->getBusesLayout();

			readBusLayoutFromXml(layout, instance.get(), *layoutEntity, true);
			readBusLayoutFromXml(layout, instance.get(), *layoutEntity, false);

			instance->setBusesLayout(layout);
		}

		if (auto node = graph.addNode(std::move(instance), NodeID((uint32)xml.getIntAttribute("uid"))))
		{
			if (auto* state = xml.getChildByName("STATE"))
			{
				MemoryBlock m;
				m.fromBase64Encoding(state->getAllSubText());

				node->getProcessor()->setStateInformation(m.getData(), (int)m.getSize());
			}

			node->properties.set("x", xml.getDoubleAttribute("x"));
			node->properties.set("y", xml.getDoubleAttribute("y"));

			for (int i = 0; i < (int)PluginWindow::Type::numTypes; ++i)
			{
				auto type = (PluginWindow::Type) i;

				if (xml.hasAttribute(PluginWindow::getOpenProp(type)))
				{
					node->properties.set(PluginWindow::getLastXProp(type), xml.getIntAttribute(PluginWindow::getLastXProp(type)));
					node->properties.set(PluginWindow::getLastYProp(type), xml.getIntAttribute(PluginWindow::getLastYProp(type)));
					node->properties.set(PluginWindow::getOpenProp(type), xml.getIntAttribute(PluginWindow::getOpenProp(type)));

					if (node->properties[PluginWindow::getOpenProp(type)])
					{
						jassert(node->getProcessor() != nullptr);

						if (auto w = getOrCreateWindowFor(node, type))
							w->toFront(true);
					}
				}
			}
		}
	}
	//mod RW
	else
	{
		const Point<double> pos(xml.getDoubleAttribute("x"), xml.getDoubleAttribute("y"));
		String pluginFile = currentFile.getParentDirectory().getChildFile(pd.fileOrIdentifier).getFullPathName();
        
        pd.fileOrIdentifier = pluginFile;
		addCabbagePlugin(pd, pos);

		if (auto * layoutEntity = xml.getChildByName("LAYOUT"))
		{
			auto layout = graph.getNodeForId(AudioProcessorGraph::NodeID(pd.uid))->getProcessor()->getBusesLayout();
            auto pluginProcessor = graph.getNodeForId(AudioProcessorGraph::NodeID(pd.uid))->getProcessor();
            
			readBusLayoutFromXml(layout, pluginProcessor, *layoutEntity, true);
			readBusLayoutFromXml(layout, pluginProcessor, *layoutEntity, false);

			pluginProcessor->setBusesLayout(layout);
		}

		if (auto * state = xml.getChildByName("STATE"))
		{
			MemoryBlock m;
			m.fromBase64Encoding(state->getAllSubText());
			graph.getNodeForId(AudioProcessorGraph::NodeID(pd.uid))->getProcessor()->setStateInformation(m.getData(), (int)m.getSize());
		}

		if (auto* node = graph.getNodeForId(AudioProcessorGraph::NodeID(pd.uid)))
			if (auto w = getOrCreateWindowFor(node, PluginWindow::Type::normal))
				w->toFront(true);
	}
}

XmlElement* FilterGraph::createXml() const
{
	auto* xml = new XmlElement("FILTERGRAPH");

	//mod RW
	for (auto* node : graph.getNodes())
	{
		PluginDescription pd = getPluginDescriptor(node->nodeID, node->properties.getWithDefault("pluginFile", ""));
		const String file = File::getCurrentWorkingDirectory().getChildFile(pd.fileOrIdentifier).getRelativePathFrom(File(currentFile).getParentDirectory());
		node->properties.set("pluginFile", file);
		xml->addChildElement(createXmlForNode(node));
	}
		

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

void FilterGraph::restoreFromXml(const XmlElement& xml)
{
	clear();

	forEachXmlChildElementWithTagName(xml, e, "FILTER")
	{
		createNodeFromXml(*e);
		changed();
	}

	forEachXmlChildElementWithTagName(xml, e, "CONNECTION")
	{
		graph.addConnection({ { NodeID((uint32)e->getIntAttribute("srcFilter")), e->getIntAttribute("srcChannel") },
			{ NodeID((uint32)e->getIntAttribute("dstFilter")), e->getIntAttribute("dstChannel") } });
	}

	graph.removeIllegalConnections();
}

File FilterGraph::getDefaultGraphDocumentOnMobile()
{
    auto persistantStorageLocation = File::getSpecialLocation (File::userApplicationDataDirectory);
    return persistantStorageLocation.getChildFile ("state.filtergraph");
}
