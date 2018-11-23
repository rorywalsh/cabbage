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
    connectors.clear();
    nodes.clear();
	deleteAllChildren();
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


CabbagePluginComponent* CabbageGraphComponent::getComponentForFilter (AudioProcessorGraph::NodeID filterID) const
{
    for (auto *fc : nodes)
        if (fc->pluginID == filterID)
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
        if (graph.graph.getNodeForId (nodes.getUnchecked(i)->pluginID) == nullptr)
            nodes.remove (i);
    
    for (int i = connectors.size(); --i >= 0;)
        if (! graph.graph.isConnected (connectors.getUnchecked(i)->connection))
            connectors.remove (i);
    
    for (auto* fc : nodes)
        fc->update();
    
    for (auto* cc : connectors)
        cc->update();
    
    for (auto* f : graph.graph.getNodes())
    {
        if (getComponentForFilter (f->nodeID) == 0)
        {
            auto* comp = nodes.add (new CabbagePluginComponent(graph, f->nodeID));
            addAndMakeVisible (comp);
            comp->update();
        }
    }
    
    for (auto& c : graph.graph.getConnections())
    {
        if (getComponentForConnection (c) == 0)
        {
            auto* comp = connectors.add (new ConnectorComponent(*this));
            addAndMakeVisible (comp);
            
            comp->setInput (c.source);
            comp->setOutput (c.destination);
        }
    }
}

void CabbageGraphComponent::beginConnectorDrag (AudioProcessorGraph::NodeAndChannel newSource,
                                                AudioProcessorGraph::NodeAndChannel newDest,
                                                const MouseEvent& e)
{
    auto* c = dynamic_cast<ConnectorComponent*> (e.originalComponent);
    connectors.removeObject (c, false);
    draggingConnector.reset (c);
    
    if (draggingConnector == nullptr)
        draggingConnector.reset (new ConnectorComponent (*this));
    
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
            
            if (connection.source.nodeID == AudioProcessorGraph::NodeID() && ! pin->isInput)
            {
                connection.source = pin->pin;
            }
            else if (connection.destination.nodeID == AudioProcessorGraph::NodeID() && pin->isInput)
            {
                connection.destination = pin->pin;
            }
            
            if (graph.graph.canConnect (connection))
            {
                pos = (pin->getParentComponent()->getPosition() + pin->getBounds().getCentre()).toFloat();
                draggingConnector->setTooltip (pin->getTooltip());
            }
        }
        
        if (draggingConnector->connection.source.nodeID == AudioProcessorGraph::NodeID())
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
        if (connection.source.nodeID == AudioProcessorGraph::NodeID())
        {
            if (pin->isInput)
                return;
            
            connection.source = pin->pin;
        }
        else
        {
            if (! pin->isInput)
                return;
            
            connection.destination = pin->pin;
        }
        
        graph.graph.addConnection (connection);
    }
}


