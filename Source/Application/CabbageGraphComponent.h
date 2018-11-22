/*
  Copyright (C) 2017 Rory Walsh

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

#ifndef CABBAGEGRAPHCOMPONENT_H_INCLUDED
#define CABBAGEGRAPHCOMPONENT_H_INCLUDED

#include "../CabbageCommonHeaders.h"
#include "../Audio/Graph/AudioGraph.h"
#include "CabbagePluginComponent.h"

class CabbageMainComponent;


class CabbageGraphComponent : public Component,
    public ChangeListener
{
public:
    CabbageGraphComponent (AudioGraph& graph, CabbageMainComponent& owner);
    ~CabbageGraphComponent();

    void paint (Graphics& g) override;
    void mouseDown (const MouseEvent& e) override;


    OwnedArray<CabbagePluginComponent> nodes;
    OwnedArray<ConnectorComponent> connectors;

    
    CabbagePluginComponent* getComponentForFilter (AudioProcessorGraph::NodeID filterID) const;
    ConnectorComponent* getComponentForConnection (const AudioProcessorGraph::Connection& conn) const;
    PinComponent* findPinAt (Point<float> pos) const;

    void resized() override;
    void changeListenerCallback (ChangeBroadcaster*) override;
    void updateComponents();

    //==============================================================================
    void beginConnectorDrag (AudioProcessorGraph::NodeAndChannel newSource,
                             AudioProcessorGraph::NodeAndChannel newDest,
                             const MouseEvent& e);
    void dragConnector (const MouseEvent& e);
    void endDraggingConnector (const MouseEvent& e);

    //==============================================================================
    AudioGraph& graph;
private:
    Array<File> exampleFiles;
    Array<File> userFiles;
    CabbageLookAndFeel2 lookAndFeel;

    CabbageMainComponent& owner;
    ScopedPointer<ConnectorComponent> draggingConnector;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabbageGraphComponent)
};




#endif  // CABBAGEGRAPHCOMPONENT_H_INCLUDED
