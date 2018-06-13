/*
  This file is part of the JUCE library.
  Copyright (c) 2015 - ROLI Ltd.

  Permission is granted to use this software under the terms of either:
   a) the GPL v2 (or any later version)
   b) the Affero GPL v3

   Details of these licenses can be found at: www.gnu.org/licenses
*/

#include "CabbageGraphComponent.h"
#include "CabbageMainComponent.h"
#include "../Audio/Plugins/CabbagePluginProcessor.h"

CabbageGraphComponent::CabbageGraphComponent (AudioGraph& graph_, CabbageMainComponent& owner_)
    : graph (graph_),
      owner (owner_)
      //lookAndFeel()
{
    graph.addChangeListener (this);
    setOpaque (false);
}

CabbageGraphComponent::~CabbageGraphComponent()
{
    graph.removeChangeListener (this);
    draggingConnector = nullptr;
    //deleteAllChildren();
}

void CabbageGraphComponent::paint (Graphics& g)
{
    g.fillAll (Colour (uint8 (20), uint8 (20), uint8 (20)));
}

void CabbageGraphComponent::mouseDown (const MouseEvent& e)
{
    if (e.mods.isPopupMenu())
    {
        Uuid uniqueID;
        PopupMenu m, subMenu1, subMenu2;
        m.setLookAndFeel (&lookAndFeel);
        const String examplesDir = owner.getCabbageSettings()->getUserSettings()->getValue ("CabbageExamplesDir", "");
        CabbageUtilities::addExampleFilesToPopupMenu (subMenu1, exampleFiles, examplesDir, "*.csd", 3000);

        const String userFilesDir = owner.getCabbageSettings()->getUserSettings()->getValue ("UserFilesDir", "");
        CabbageUtilities::addFilesToPopupMenu (subMenu2, userFiles, userFilesDir, 10000);

        m.addItem (1, "Open file..");
        m.addSubMenu ("Examples", subMenu1);
        m.addSubMenu ("User files", subMenu2);
        const int r = m.show();

        if ( r  == 1 )
        {
            File newlyOpenedFile = owner.openFile ();

            if (newlyOpenedFile.existsAsFile())
            {
                owner.runCsoundForNode (newlyOpenedFile.getFullPathName());
            }
        }

        else if ( r > 1 && r < 10000)
        {
            owner.openFile (exampleFiles[r - 3000].getFullPathName());
            owner.runCsoundForNode (exampleFiles[r - 3000].getFullPathName());
        }

        else if ( r >= 10000)
        {
            owner.openFile (userFiles[r - 10000].getFullPathName());
            owner.runCsoundForNode (userFiles[r - 10000].getFullPathName());
        }

    }
}

void CabbageGraphComponent::createNewPlugin (const PluginDescription* desc, int x, int y)
{
    //graph.addFilter (desc, x / (double) getWidth(), y / (double) getHeight());
}

CabbagePluginComponent* CabbageGraphComponent::getComponentForFilter (const uint32 filterID) const
{
    for (auto* fc : nodes)
        if (fc->filterID == filterID)
            return fc;
    
    return nullptr;
}

ConnectorComponent* CabbageGraphComponent::getComponentForConnection (const AudioProcessorGraph::Connection& conn) const
{
    for (auto* cc : connectors)
        if (cc->connection == conn)
            return cc;
    
    return nullptr;
}

PinComponent* CabbageGraphComponent::findPinAt (Point<float> pos) const
{
    for (auto* fc : nodes)
    {
        // NB: A Visual Studio optimiser error means we have to put this Component* in a local
        // variable before trying to cast it, or it gets mysteriously optimised away..
        auto* comp = fc->getComponentAt (pos.toInt() - fc->getPosition());
        
        if (auto* pin = dynamic_cast<PinComponent*> (comp))
            return pin;
    }
    
    return nullptr;
}


void CabbageGraphComponent::resized()
{
    updateComponents();
}

void CabbageGraphComponent::changeListenerCallback (ChangeBroadcaster*)
{
    updateComponents();
}

void CabbageGraphComponent::updateComponents()
{
    for (int i = nodes.size(); --i >= 0;)
        if (graph.graph.getNodeForId (nodes.getUnchecked(i)->filterID) == nullptr)
            nodes.remove (i);
    
    for (int i = connectors.size(); --i >= 0;)
        if (! graph.graph.isConnected (graph.getConnections()[i]))
            graph.graph.removeConnection(graph.getConnections()[i]);
    
    for (auto* fc : nodes)
        fc->update();
    
    for (auto* cc : connectors)
        cc->update();
    
    for (auto* f : graph.graph.getNodes())
    {
        if (getComponentForFilter (f->nodeID) == 0)
        {
            auto* comp = nodes.add (new CabbagePluginComponent (graph, f->nodeID));
            addAndMakeVisible (comp);
            comp->update();
        }
    }
    
    for (auto& c : graph.graph.getConnections())
    {
        if (getComponentForConnection (c) == 0)
        {
            auto* comp = connectors.add (new ConnectorComponent (graph));
            addAndMakeVisible (comp);
            
            comp->setInput (c.source);
            comp->setOutput (c.destination);
        }
    }
//    for (int i = (int)graph.graph.getConnections().size(); --i >= 0;)
//        if (! graph.graph.isConnected (graph.graph.getConnections()[i]))
//            graph.graph.getConnections().erase(graph.graph.getConnections().begin()+i);
//
//    for (int i = getNumChildComponents(); --i >= 0;)
//    {
//        if (CabbagePluginComponent* const fc = dynamic_cast<CabbagePluginComponent*> (getChildComponent (i)))
//            fc->update();
//    }
//
////    for (int i = getNumChildComponents(); --i >= 0;)
////    {
////        if (CabbagePluginComponent* const fc = dynamic_cast<CabbagePluginComponent*> (getChildComponent (i)))
////            nodes.remove (i);
//
//    for (int i = getNumChildComponents(); --i >= 0;)
//    {
//        ConnectorComponent* const cc = dynamic_cast<ConnectorComponent*> (getChildComponent (i));
//
//        if (cc != nullptr && cc != draggingConnector)
//        {
//            if (graph.getConnectionBetween (cc->sourceFilterID, cc->sourceFilterChannel,
//                                            cc->destFilterID, cc->destFilterChannel) == nullptr)
//            {
//                delete cc;
//            }
//            else
//            {
//                cc->update();
//            }
//        }
//    }
//
//    for (int i = graph.getNumPlugins(); --i >= 0;)
//    {
//        const AudioProcessorGraph::Node::Ptr f (graph.getNode (i));
//
//        if (getComponentForFilter (f->nodeID) == 0)
//        {
//            CabbagePluginComponent* const comp = new CabbagePluginComponent (graph, f->nodeID);
//            addAndMakeVisible (comp);
//            comp->update();
//        }
//    }
//
//    for (int i = graph.getNumConnections(); --i >= 0;)
//    {
//        const AudioProcessorGraph::Connection* const c = graph.getConnection (i);
//
//        if (getComponentForConnection (*c) == 0)
//        {
//            ConnectorComponent* const comp = new ConnectorComponent (graph);
//            addAndMakeVisible (comp);
//
//            comp->setInput (c->source.nodeID, c->source.channelIndex);
//            comp->setOutput (c->destination.nodeID, c->destination.channelIndex);
//        }
//    }
}

void CabbageGraphComponent::beginConnectorDrag (AudioProcessorGraph::NodeAndChannel newSource,
                                                AudioProcessorGraph::NodeAndChannel newDest,
                                                const MouseEvent& e)
{
//    draggingConnector = dynamic_cast<ConnectorComponent*> (e.originalComponent);
//
//    if (draggingConnector == nullptr)
//        draggingConnector = new ConnectorComponent (graph);
//
//    draggingConnector->setInput (newSource);
//    draggingConnector->setOutput (newDest);
//
//    addAndMakeVisible (draggingConnector);
//    draggingConnector->toFront (false);
//
//    dragConnector (e);
    auto* c = dynamic_cast<ConnectorComponent*> (e.originalComponent);
    connectors.removeObject (c, false);
    draggingConnector.reset (c);
    
    if (draggingConnector == nullptr)
        draggingConnector.reset (new ConnectorComponent (graph));
    
    draggingConnector->setInput (newSource);
    draggingConnector->setOutput (newDest);
    
    addAndMakeVisible (draggingConnector.get());
    draggingConnector->toFront (false);
    
    dragConnector (e);
}

void CabbageGraphComponent::dragConnector (const MouseEvent& e)
{
    auto e2 = e.getEventRelativeTo (this);
    
    if (draggingConnector != nullptr)
    {
        draggingConnector->setTooltip ({});
        
        auto pos = e2.position;
        
        if (auto* pin = findPinAt (pos))
        {
            auto connection = draggingConnector->connection;
            
            if (connection.source.nodeID == 0 && ! pin->isInput)
            {
                connection.source = {pin->filterID, pin->index};
            }
            else if (connection.destination.nodeID == 0 && pin->isInput)
            {
                connection.destination = {pin->filterID, pin->index};
            }
            
            if (graph.graph.canConnect (connection))
            {
                pos = (pin->getParentComponent()->getPosition() + pin->getBounds().getCentre()).toFloat();
                draggingConnector->setTooltip (pin->getTooltip());
            }
        }
        
        if (draggingConnector->connection.source.nodeID == 0)
            draggingConnector->dragStart (pos);
        else
            draggingConnector->dragEnd (pos);
    }
}

void CabbageGraphComponent::endDraggingConnector (const MouseEvent& e)
{
    if (draggingConnector == nullptr)
        return;
    
    draggingConnector->setTooltip ({});
    
    auto e2 = e.getEventRelativeTo (this);
    auto connection = draggingConnector->connection;
    
    draggingConnector = nullptr;
    
    if (auto* pin = findPinAt (e2.position))
    {
        if (connection.source.nodeID == 0)
        {
            if (pin->isInput)
                return;
            
            connection.source = {pin->filterID, pin->index};
        }
        else
        {
            if (! pin->isInput)
                return;
            
            connection.destination = {pin->filterID, pin->index};
        }
        
        graph.graph.addConnection (connection);
    }
}


