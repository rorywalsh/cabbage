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

    void paint (Graphics& g);
    void mouseDown (const MouseEvent& e);

    void createNewPlugin (const PluginDescription* desc, int x, int y);

    CabbagePluginComponent* getComponentForFilter (uint32 filterID) const;
    ConnectorComponent* getComponentForConnection (const AudioProcessorGraph::Connection& conn) const;
    PinComponent* findPinAt (int x, int y) const;

    void resized();
    void changeListenerCallback (ChangeBroadcaster*);
    void updateComponents();

    //==============================================================================
    void beginConnectorDrag (uint32 sourceFilterID, int sourceFilterChannel,
                             uint32 destFilterID, int destFilterChannel,
                             const MouseEvent& e);
    void dragConnector (const MouseEvent& e);
    void endDraggingConnector (const MouseEvent& e);

    //==============================================================================
private:
    Array<File> exampleFiles;
    Array<File> userFiles;
    CabbageLookAndFeel2 lookAndFeel;
    AudioGraph& graph;
    CabbageMainComponent& owner;
    ScopedPointer<ConnectorComponent> draggingConnector;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabbageGraphComponent)
};




#endif  // CABBAGEGRAPHCOMPONENT_H_INCLUDED
