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

#include "../JuceLibraryCode/JuceHeader.h"
#include "FilterGraph.h"
#include "InternalFilters.h"



//==============================================================================
const int FilterGraph::midiChannelNumber = 0x1000;

FilterGraph::FilterGraph (AudioPluginFormatManager& formatManager_)
    : FileBasedDocument (filenameSuffix,
                         filenameWildcard,
                         "Load a filter graph",
                         "Save a filter graph"),
      formatManager (formatManager_), lastUID (0)
{
    InternalPluginFormat internalFormat;

    addFilter (internalFormat.getDescriptionFor (InternalPluginFormat::audioInputFilter),  0.5f,  0.1f);
    addFilter (internalFormat.getDescriptionFor (InternalPluginFormat::midiInputFilter),   0.25f, 0.1f);
    addFilter (internalFormat.getDescriptionFor (InternalPluginFormat::audioOutputFilter), 0.5f,  0.9f);

    graph.addListener (this);

    setChangedFlag (false);
}

FilterGraph::~FilterGraph()
{
    graph.addListener (this);
    graph.clear();
}

uint32 FilterGraph::getNextUID() noexcept
{
    return ++lastUID;
}

//==============================================================================
int FilterGraph::getNumFilters() const noexcept
{
    return graph.getNumNodes();
}

const AudioProcessorGraph::Node::Ptr FilterGraph::getNode (const int index) const noexcept
{
    return graph.getNode (index);
}

const AudioProcessorGraph::Node::Ptr FilterGraph::getNodeForId (const uint32 uid) const noexcept
{
    return graph.getNodeForId (uid);
}

void FilterGraph::addFilter (const PluginDescription* desc, double x, double y)
{
    if (desc != nullptr)
    {
        struct AsyncCallback : public AudioPluginFormat::InstantiationCompletionCallback
        {
            AsyncCallback (FilterGraph* myself, double inX, double inY)
                : owner (myself), posX (inX), posY (inY)
            {}

            void completionCallback (AudioPluginInstance* instance, const String& error) override
            {
                owner->addFilterCallback (instance, error, posX, posY);
            }

            FilterGraph* owner;
            double posX, posY;
        };

        formatManager.createPluginInstanceAsync (*desc, graph.getSampleRate(), graph.getBlockSize(),
                                                 new AsyncCallback (this, x, y));
    }
}

void FilterGraph::addFilterCallback (AudioPluginInstance* instance, const String& error, double x, double y)
{
    if (instance == nullptr)
    {
        AlertWindow::showMessageBox (AlertWindow::WarningIcon,
                                     TRANS("Couldn't create filter"),
                                     error);
    }
    else
    {
        instance->enableAllBuses();
        AudioProcessorGraph::Node* node = graph.addNode (instance);

        if (node != nullptr)
        {
            node->properties.set ("x", x);
            node->properties.set ("y", y);
            changed();
        }
    }
}

void FilterGraph::removeFilter (const uint32 id)
{
   // PluginWindow::closeCurrentlyOpenWindowsFor (id);

    if (graph.removeNode (id))
        changed();
}

void FilterGraph::disconnectFilter (const uint32 id)
{
    if (graph.disconnectNode (id))
        changed();
}

void FilterGraph::removeIllegalConnections()
{
    if (graph.removeIllegalConnections())
        changed();
}

void FilterGraph::setNodePosition (const uint32 nodeId, double x, double y)
{
    if (AudioProcessorGraph::Node::Ptr n = graph.getNodeForId (nodeId))
    {
        n->properties.set ("x", jlimit (0.0, 1.0, x));
        n->properties.set ("y", jlimit (0.0, 1.0, y));
    }
}

Point<double> FilterGraph::getNodePosition (const uint32 nodeId) const
{
    if (AudioProcessorGraph::Node::Ptr n = graph.getNodeForId (nodeId))
        return Point<double> (static_cast<double> (n->properties ["x"]),
                              static_cast<double> (n->properties ["y"]));

    return Point<double>();
}

//==============================================================================
int FilterGraph::getNumConnections() const noexcept
{
    return graph.getNumConnections();
}

const AudioProcessorGraph::Connection* FilterGraph::getConnection (const int index) const noexcept
{
    return graph.getConnection (index);
}

const AudioProcessorGraph::Connection* FilterGraph::getConnectionBetween (uint32 sourceFilterUID, int sourceFilterChannel,
                                                                          uint32 destFilterUID, int destFilterChannel) const noexcept
{
    return graph.getConnectionBetween (sourceFilterUID, sourceFilterChannel,
                                       destFilterUID, destFilterChannel);
}

bool FilterGraph::canConnect (uint32 sourceFilterUID, int sourceFilterChannel,
                              uint32 destFilterUID, int destFilterChannel) const noexcept
{
    return graph.canConnect (sourceFilterUID, sourceFilterChannel,
                             destFilterUID, destFilterChannel);
}

bool FilterGraph::addConnection (uint32 sourceFilterUID, int sourceFilterChannel,
                                 uint32 destFilterUID, int destFilterChannel)
{
    const bool result = graph.addConnection (sourceFilterUID, sourceFilterChannel,
                                             destFilterUID, destFilterChannel);

    if (result)
        changed();

    return result;
}

void FilterGraph::removeConnection (const int index)
{
    graph.removeConnection (index);
    changed();
}

void FilterGraph::removeConnection (uint32 sourceFilterUID, int sourceFilterChannel,
                                    uint32 destFilterUID, int destFilterChannel)
{
    if (graph.removeConnection (sourceFilterUID, sourceFilterChannel,
                                destFilterUID, destFilterChannel))
        changed();
}

void FilterGraph::clear()
{
   // PluginWindow::closeAllCurrentlyOpenWindows();

    graph.clear();
    changed();
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

   // setFile (File());

    InternalPluginFormat internalFormat;

    addFilter (internalFormat.getDescriptionFor (InternalPluginFormat::audioInputFilter),  0.5f,  0.1f);
    addFilter (internalFormat.getDescriptionFor (InternalPluginFormat::midiInputFilter),   0.25f, 0.1f);
    addFilter (internalFormat.getDescriptionFor (InternalPluginFormat::audioOutputFilter), 0.5f,  0.9f);

    setChangedFlag (false);
}

Result FilterGraph::loadDocument (const File& file)
{
    XmlDocument doc (file);
    ScopedPointer<XmlElement> xml (doc.getDocumentElement());

    if (xml == nullptr || ! xml->hasTagName ("FILTERGRAPH"))
        return Result::fail ("Not a valid filter graph file");

    restoreFromXml (*xml);
    return Result::ok();
}

Result FilterGraph::saveDocument (const File& file)
{
    ScopedPointer<XmlElement> xml (createXml());

    if (! xml->writeToFile (file, String()))
        return Result::fail ("Couldn't write to the file");

    return Result::ok();
}

File FilterGraph::getLastDocumentOpened()
{
//    RecentlyOpenedFilesList recentFiles;
//    recentFiles.restoreFromString (getAppProperties().getUserSettings()
//                                        ->getValue ("recentFilterGraphFiles"));
//
//    return recentFiles.getFile (0);
}

void FilterGraph::setLastDocumentOpened (const File& file)
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

//==============================================================================
static XmlElement* createNodeXml (AudioProcessorGraph::Node* const node) noexcept
{
 
}

void FilterGraph::createNodeFromXml (const XmlElement& xml)
{
 
}

XmlElement* FilterGraph::createXml() const
{

}

void FilterGraph::restoreFromXml (const XmlElement& xml)
{
 
}
