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
                owner.getNodeIds().set (newlyOpenedFile.getFullPathName(), int32 (*uniqueID.getRawData()));
                owner.runCsoundForNode (newlyOpenedFile.getFullPathName());
            }
        }

        else if ( r > 1 && r < 10000)
        {
            owner.openFile (exampleFiles[r - 3000].getFullPathName());
            owner.getNodeIds().set (exampleFiles[r - 3000].getFullPathName(), int32 (*uniqueID.getRawData()));
            owner.runCsoundForNode (exampleFiles[r - 3000].getFullPathName());
        }

        else if ( r >= 10000)
        {
            owner.getNodeIds().set (userFiles[r - 10000].getFullPathName(), int32 (*uniqueID.getRawData()));
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
    for (int i = getNumChildComponents(); --i >= 0;)
    {
        if (CabbagePluginComponent* const fc = dynamic_cast<CabbagePluginComponent*> (getChildComponent (i)))
            if (fc->filterID == filterID)
                return fc;
    }

    return nullptr;
}

ConnectorComponent* CabbageGraphComponent::getComponentForConnection (const AudioProcessorGraph::Connection& conn) const
{
    for (int i = getNumChildComponents(); --i >= 0;)
    {
        if (ConnectorComponent* const c = dynamic_cast<ConnectorComponent*> (getChildComponent (i)))
            if (c->sourceFilterID == conn.sourceNodeId
                && c->destFilterID == conn.destNodeId
                && c->sourceFilterChannel == conn.sourceChannelIndex
                && c->destFilterChannel == conn.destChannelIndex)
                return c;
    }

    return nullptr;
}

PinComponent* CabbageGraphComponent::findPinAt (const int x, const int y) const
{
    for (int i = getNumChildComponents(); --i >= 0;)
    {
        if (CabbagePluginComponent* fc = dynamic_cast<CabbagePluginComponent*> (getChildComponent (i)))
        {
            if (PinComponent* pin = dynamic_cast<PinComponent*> (fc->getComponentAt (x - fc->getX(),
                                                                                     y - fc->getY())))
                return pin;
        }
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
    for (int i = getNumChildComponents(); --i >= 0;)
    {
        if (CabbagePluginComponent* const fc = dynamic_cast<CabbagePluginComponent*> (getChildComponent (i)))
            fc->update();
    }

    for (int i = getNumChildComponents(); --i >= 0;)
    {
        ConnectorComponent* const cc = dynamic_cast<ConnectorComponent*> (getChildComponent (i));

        if (cc != nullptr && cc != draggingConnector)
        {
            if (graph.getConnectionBetween (cc->sourceFilterID, cc->sourceFilterChannel,
                                            cc->destFilterID, cc->destFilterChannel) == nullptr)
            {
                delete cc;
            }
            else
            {
                cc->update();
            }
        }
    }

    for (int i = graph.getNumPlugins(); --i >= 0;)
    {
        const AudioProcessorGraph::Node::Ptr f (graph.getNode (i));

        if (getComponentForFilter (f->nodeId) == 0)
        {
            CabbagePluginComponent* const comp = new CabbagePluginComponent (graph, f->nodeId);
            addAndMakeVisible (comp);
            comp->update();
        }
    }

    for (int i = graph.getNumConnections(); --i >= 0;)
    {
        const AudioProcessorGraph::Connection* const c = graph.getConnection (i);

        if (getComponentForConnection (*c) == 0)
        {
            ConnectorComponent* const comp = new ConnectorComponent (graph);
            addAndMakeVisible (comp);

            comp->setInput (c->sourceNodeId, c->sourceChannelIndex);
            comp->setOutput (c->destNodeId, c->destChannelIndex);
        }
    }
}

void CabbageGraphComponent::beginConnectorDrag (const uint32 sourceFilterID, const int sourceFilterChannel,
                                                const uint32 destFilterID, const int destFilterChannel,
                                                const MouseEvent& e)
{
    draggingConnector = dynamic_cast<ConnectorComponent*> (e.originalComponent);

    if (draggingConnector == nullptr)
        draggingConnector = new ConnectorComponent (graph);

    draggingConnector->setInput (sourceFilterID, sourceFilterChannel);
    draggingConnector->setOutput (destFilterID, destFilterChannel);

    addAndMakeVisible (draggingConnector);
    draggingConnector->toFront (false);

    dragConnector (e);
}

void CabbageGraphComponent::dragConnector (const MouseEvent& e)
{
    const MouseEvent e2 (e.getEventRelativeTo (this));

    if (draggingConnector != nullptr)
    {
        draggingConnector->setTooltip (String());

        int x = e2.x;
        int y = e2.y;

        if (PinComponent* const pin = findPinAt (x, y))
        {
            uint32 srcFilter = draggingConnector->sourceFilterID;
            int srcChannel   = draggingConnector->sourceFilterChannel;
            uint32 dstFilter = draggingConnector->destFilterID;
            int dstChannel   = draggingConnector->destFilterChannel;

            if (srcFilter == 0 && ! pin->isInput)
            {
                srcFilter = pin->filterID;
                srcChannel = pin->index;
            }
            else if (dstFilter == 0 && pin->isInput)
            {
                dstFilter = pin->filterID;
                dstChannel = pin->index;
            }

            if (graph.canConnect (srcFilter, srcChannel, dstFilter, dstChannel))
            {
                x = pin->getParentComponent()->getX() + pin->getX() + pin->getWidth() / 2;
                y = pin->getParentComponent()->getY() + pin->getY() + pin->getHeight() / 2;

                draggingConnector->setTooltip (pin->getTooltip());
            }
        }

        if (draggingConnector->sourceFilterID == 0)
            draggingConnector->dragStart (x, y);
        else
            draggingConnector->dragEnd (x, y);
    }
}

void CabbageGraphComponent::endDraggingConnector (const MouseEvent& e)
{
    if (draggingConnector == nullptr)
        return;

    draggingConnector->setTooltip (String());

    const MouseEvent e2 (e.getEventRelativeTo (this));

    uint32 srcFilter = draggingConnector->sourceFilterID;
    int srcChannel   = draggingConnector->sourceFilterChannel;
    uint32 dstFilter = draggingConnector->destFilterID;
    int dstChannel   = draggingConnector->destFilterChannel;

    draggingConnector = nullptr;

    if (PinComponent* const pin = findPinAt (e2.x, e2.y))
    {
        if (srcFilter == 0)
        {
            if (pin->isInput)
                return;

            srcFilter = pin->filterID;
            srcChannel = pin->index;
        }
        else
        {
            if (! pin->isInput)
                return;

            dstFilter = pin->filterID;
            dstChannel = pin->index;
        }

        graph.addConnection (srcFilter, srcChannel, dstFilter, dstChannel);
    }
}


