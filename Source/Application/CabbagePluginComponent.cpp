/*
==============================================================================

CabbagePluginComponent.cpp
Created: 25 Jan 2017 3:21:05pm
Author:  rory

==============================================================================
*/

#include "CabbagePluginComponent.h"
#include "CabbageGraphComponent.h"

CabbagePluginComponent::CabbagePluginComponent(AudioGraph& p, AudioProcessorGraph::NodeID id)  :
    graph (p),
    pluginID (id),
    lookAndFeel()
{
    shadow.setShadowProperties (DropShadow (Colours::black.withAlpha (0.5f), 3, Point<int> (0, 1)));
    setComponentEffect (&shadow);

    setSize (150, 60);
}

CabbagePluginComponent::~CabbagePluginComponent()
{
   // pins.clear();
    //deleteAllChildren();
}

void CabbagePluginComponent::mouseDown (const MouseEvent& e)
{
    originalPos = localPointToGlobal (Point<int>());

    toFront (true);

    if (e.mods.isPopupMenu())
    {
        PopupMenu m;
        m.setLookAndFeel (&lookAndFeel);

        m.addItem (3, "Show plugin interface");
        m.addItem (4, "Show source code in editor");
        m.addSeparator();
        m.addItem (1, "Delete this plugin");
        m.addItem (2, "Disconnect all pins");

        const int r = m.show();

        if (r == 1)
        {
            graph.removeFilter (pluginID);
            return;
        }
        else if (r == 2)
        {
            graph.disconnectFilter (pluginID);
        }
        else if ( r == 3)
        {
            if (AudioProcessorGraph::Node::Ptr f = graph.getNodeForId (pluginID))
            {
                if (auto f = graph.graph.getNodeForId (pluginID))
                    if (auto* w = graph.getOrCreateWindowFor (f, PluginWindow::Type::normal))
                        w->toFront (true);
            }
        }
        else if ( r == 4)
            graph.showCodeEditorForNode (pluginID);
    }
}

void CabbagePluginComponent::mouseDrag (const MouseEvent& e)
{
    if (! e.mods.isPopupMenu())
    {
        auto pos = originalPos + e.getOffsetFromDragStart();

        if (getParentComponent() != nullptr)
            pos = getParentComponent()->getLocalPoint (nullptr, pos);

        pos += getLocalBounds().getCentre();

        graph.setNodePosition (pluginID,
                               { pos.x / (double) getParentWidth(),
                                 pos.y / (double) getParentHeight() });

        getCabbageGraphComponent()->updateComponents();
    }
}

void CabbagePluginComponent::mouseUp (const MouseEvent& e)
{
    if (e.mouseWasDraggedSinceMouseDown())
    {
        graph.setChangedFlag (true);
    }
    else if (e.getNumberOfClicks() == 2)
    {
            if (AudioProcessorGraph::Node::Ptr f = graph.getNodeForId (pluginID))
            {
                if (auto f = graph.graph.getNodeForId (pluginID))
                    if (auto* w = graph.getOrCreateWindowFor (f, PluginWindow::Type::normal))
                        w->toFront (true);
            }

    }
}

bool CabbagePluginComponent::hitTest (int x, int y)
{
    for (auto* child : getChildren())
        if (child->getBounds().contains (x, y))
            return true;

    return x >= 3 && x < getWidth() - 6 && y >= pinSize && y < getHeight() - pinSize;
}

void CabbagePluginComponent::paint (Graphics& g)
{
    const int x = 4;
    const int y = pinSize;
    const int w = getWidth() - x * 2;
    const int h = getHeight() - pinSize * 2;

    g.setColour (CabbageUtilities::getComponentSkin());
    //g.setColour(Colour(220, 220, 220));
    g.fillRoundedRectangle (x, y, w, h, 5);

    g.drawRoundedRectangle (x, y, w, h, 5, 1.f);
    g.setColour (Colour (120, 120, 120));
    g.setFont (CabbageUtilities::getComponentFont());
    g.drawFittedText (getName(),
                      x + 4, y - 2, w - 8, h - 4,
                      Justification::centred, 2);

    g.setOpacity (0.2);
    g.setColour (Colours::green.withAlpha (.3f));
    g.drawRoundedRectangle (x + 0.5, y + 0.5, w - 1, h - 1, 5, 1.0f);
}

void CabbagePluginComponent::resized()
{
    if (auto f = graph.graph.getNodeForId (pluginID))
    {
        if (auto* processor = f->getProcessor())
        {
            for (auto* pin : pins)
            {
                const bool isInput = pin->isInput;
                auto channelIndex = pin->pin.channelIndex;
                int busIdx = 0;
                processor->getOffsetInBusBufferForAbsoluteChannelIndex (isInput, channelIndex, busIdx);

                const int total = isInput ? numIns : numOuts;
                const int index = pin->pin.isMIDI() ? (total - 1) : channelIndex;

                auto totalSpaces = static_cast<float> (total) + (static_cast<float> (jmax (0, processor->getBusCount (isInput) - 1)) * 0.5f);
                auto indexPos = static_cast<float> (index) + (static_cast<float> (busIdx) * 0.5f);

                pin->setBounds (proportionOfWidth ((1.0f + indexPos) / (totalSpaces + 1.0f)) - pinSize / 2,
                                pin->isInput ? 0 : (getHeight() - pinSize),
                                pinSize, pinSize);
            }
        }
    }
}


Point<float> CabbagePluginComponent::getPinPos (int index, bool isInput) const
{
    for (auto* pin : pins)
        if (pin->pin.channelIndex == index && isInput == pin->isInput)
            return getPosition().toFloat() + pin->getBounds().getCentre().toFloat();

    return {};
}

void CabbagePluginComponent::update()
{
    const AudioProcessorGraph::Node::Ptr f (graph.graph.getNodeForId (pluginID));
    jassert (f != nullptr);

    numIns = f->getProcessor()->getTotalNumInputChannels();
    if (f->getProcessor()->acceptsMidi())
        ++numIns;

    numOuts = f->getProcessor()->getTotalNumOutputChannels();
    if (f->getProcessor()->producesMidi())
        ++numOuts;

    int w = 100;
    int h = 60;

    w = jmax (w, (jmax (numIns, numOuts) + 1) * 20);

    const int textWidth = font.getStringWidth (f->getProcessor()->getName());
    w = jmax (w, 16 + jmin (textWidth, 300));
    if (textWidth > 300)
        h = 100;

    setSize (w, h);

    if (CabbagePluginProcessor* cabbagePlugin = dynamic_cast<CabbagePluginProcessor*> (f->getProcessor()))
        setName (cabbagePlugin->getPluginName());
    else if (GenericCabbagePluginProcessor* cabbagePlugin = dynamic_cast<GenericCabbagePluginProcessor*> (f->getProcessor()))
        setName (cabbagePlugin->getPluginName());
    else
        setName (f->getProcessor()->getName());

    {
        auto p = graph.getNodePosition (pluginID);
        setCentreRelative ((float) p.x, (float) p.y);
    }


    if (numIns != numInputs || numOuts != numOutputs)
    {
        numInputs = numIns;
        numOutputs = numOuts;

        pins.clear();

        for (int i = 0; i < f->getProcessor()->getTotalNumInputChannels(); ++i)
            addAndMakeVisible (pins.add (new PinComponent (*getCabbageGraphComponent(), { pluginID, i }, true)));

        if (f->getProcessor()->acceptsMidi())
            addAndMakeVisible (pins.add (new PinComponent (*getCabbageGraphComponent(), { pluginID, AudioProcessorGraph::midiChannelIndex }, true)));

        for (int i = 0; i < f->getProcessor()->getTotalNumOutputChannels(); ++i)
            addAndMakeVisible (pins.add (new PinComponent (*getCabbageGraphComponent(), { pluginID, i }, false)));

        if (f->getProcessor()->producesMidi())
            addAndMakeVisible (pins.add (new PinComponent (*getCabbageGraphComponent(), { pluginID, AudioProcessorGraph::midiChannelIndex }, false)));

        resized();
    }

    repaint();
}

CabbageGraphComponent* CabbagePluginComponent::getCabbageGraphComponent() const noexcept
{
    if(!findParentComponentOfClass<CabbageGraphComponent>())
        jassertfalse;

    return findParentComponentOfClass<CabbageGraphComponent>();
}
//==================================================================================================================
ConnectorComponent::ConnectorComponent (CabbageGraphComponent& panel_)
    : sourceFilterID (0),
      destFilterID (0),
      sourceFilterChannel (0),
      destFilterChannel (0),
      panel (panel_),
      graph(panel_.graph),
      lastInputX (0),
      lastInputY (0),
      lastOutputX (0),
      lastOutputY (0)
{
    setAlwaysOnTop (true);
}

void ConnectorComponent::setInput (AudioProcessorGraph::NodeAndChannel newSource)
{
    if (connection.source != newSource)
    {
        connection.source = newSource;
        update();
    }
}

void ConnectorComponent::setOutput (AudioProcessorGraph::NodeAndChannel newDest)
{
    if (connection.destination != newDest)
    {
        connection.destination = newDest;
        update();
    }
}


void ConnectorComponent::dragStart (Point<float> pos)
{
    lastInputPos = pos;
    resizeToFit();
}

void ConnectorComponent::dragEnd (Point<float> pos)
{
    lastOutputPos = pos;
    resizeToFit();
}

void ConnectorComponent::update()
{
    Point<float> p1, p2;
    getPoints (p1, p2);
    
    if (lastInputPos != p1 || lastOutputPos != p2)
        resizeToFit();
}

void ConnectorComponent::resizeToFit()
{
    Point<float> p1, p2;
    getPoints (p1, p2);
    
    auto newBounds = Rectangle<float> (p1, p2).expanded (4.0f).getSmallestIntegerContainer();
    
    if (newBounds != getBounds())
        setBounds (newBounds);
    else
        resized();
    
    repaint();
}

void ConnectorComponent::getPoints (Point<float>& p1, Point<float>& p2) const
{
    p1 = lastInputPos;
    p2 = lastOutputPos;

    //this can't find the graph component at times...
    if (auto *src = panel.getComponentForFilter(connection.source.nodeID))
        p1 = src->getPinPos(connection.source.channelIndex, false);

    if (auto *dest = panel.getComponentForFilter(connection.destination.nodeID))
        p2 = dest->getPinPos(connection.destination.channelIndex, true);

}

void ConnectorComponent::paint (Graphics& g)
{
    if (connection.source.isMIDI() || connection.destination.isMIDI())
        g.setColour (Colours::red);
    else
        g.setColour (Colours::green);
    
    g.fillPath (linePath);
}

bool ConnectorComponent::hitTest (int x, int y)
{
    if (hitPath.contains ((float) x, (float) y))
    {
        double distanceFromStart, distanceFromEnd;
        getDistancesFromEnds (Point<float>(x, y), distanceFromStart, distanceFromEnd);

        // avoid clicking the connector when over a pin
        return distanceFromStart > 7.0 && distanceFromEnd > 7.0;
    }

    return false;
}

void ConnectorComponent::mouseDown (const MouseEvent&)
{
    dragging = false;
}

void ConnectorComponent::mouseDrag (const MouseEvent& e)
{
    if (dragging)
    {
        panel.dragConnector (e);
    }
    else if (e.mouseWasDraggedSinceMouseDown())
    {
        dragging = true;
        
        graph.graph.removeConnection (connection);
        
        double distanceFromStart, distanceFromEnd;
        getDistancesFromEnds (Point<float>(e.x, e.y), distanceFromStart, distanceFromEnd);
        const bool isNearerSource = (distanceFromStart < distanceFromEnd);
        AudioProcessorGraph::NodeAndChannel dummy { {}, 0 };
        panel.beginConnectorDrag (isNearerSource ? dummy : connection.source,
                                                       isNearerSource ? connection.destination : dummy,
                                                       e);

    }
    
}

void ConnectorComponent::mouseUp (const MouseEvent& e)
{
    if (dragging)
        panel.endDraggingConnector (e);
}

void ConnectorComponent::resized()
{
    Point<float> p1, p2;
    getPoints (p1, p2);
    
    lastInputPos = p1;
    lastOutputPos = p2;
    
    p1 -= getPosition().toFloat();
    p2 -= getPosition().toFloat();

    linePath.clear();
    linePath.startNewSubPath (p1);
    linePath.cubicTo (p1.x, p1.y + (p2.y - p1.y) * 0.33f,
                      p2.x, p1.y + (p2.y - p1.y) * 0.66f,
                      p2.x, p2.y);
    
    PathStrokeType wideStroke (5.0f);
    wideStroke.createStrokedPath (hitPath, linePath);
    
    PathStrokeType stroke (1.5f);
    stroke.createStrokedPath (linePath, linePath);
    
    auto arrowW = 5.0f;
    auto arrowL = 4.0f;
    
    Path arrow;
    arrow.addTriangle (-arrowL, arrowW,
                       -arrowL, -arrowW,
                       arrowL, 0.0f);
    
    arrow.applyTransform (AffineTransform()
                          .rotated (MathConstants<float>::halfPi - (float) atan2 (p2.x - p1.x, p2.y - p1.y))
                          .translated ((p1 + p2) * 0.5f));
    
    linePath.addPath (arrow);
    linePath.setUsingNonZeroWinding (true);
}


//===================================================================================
PinComponent::PinComponent (CabbageGraphComponent& p, AudioProcessorGraph::NodeAndChannel pinToUse, bool isIn)
: panel(p), graph (p.graph), pin (pinToUse), isInput (isIn)
{
    if (auto node = graph.graph.getNodeForId (pin.nodeID))
    {
        String tip;

        if (pin.isMIDI())
        {
            tip = isInput ? "MIDI Input"
                          : "MIDI Output";
        }
        else
        {
            auto& processor = *node->getProcessor();
            auto channel = processor.getOffsetInBusBufferForAbsoluteChannelIndex (isInput, pin.channelIndex, busIdx);

            if (auto* bus = processor.getBus (isInput, busIdx))
                tip = bus->getName() + ": " + AudioChannelSet::getAbbreviatedChannelTypeName (bus->getCurrentLayout().getTypeOfChannel (channel));
            else
                tip = (isInput ? "Main Input: "
                               : "Main Output: ") + String (pin.channelIndex + 1);

        }

        //setTooltip (tip);
    }

    setSize (16, 16);
}

void PinComponent::paint (Graphics& g)
{
    auto w = (float) getWidth();
    auto h = (float) getHeight();

    Path p;
    p.addEllipse (w * 0.25f, h * 0.25f, w * 0.5f, h * 0.5f);
    p.addRectangle (w * 0.4f, isInput ? (0.5f * h) : 0.0f, w * 0.2f, h * 0.5f);

    auto colour = (pin.isMIDI() ? Colours::red : Colours::green);

    g.setColour (colour.withRotatedHue (busIdx / 5.0f));
    g.fillPath (p);
}

void PinComponent::mouseDown (const MouseEvent& e)
{
    AudioProcessorGraph::NodeAndChannel dummy { {}, 0 };
    panel.beginConnectorDrag (isInput ? dummy : pin, isInput ? pin : dummy, e);
}

void PinComponent::mouseDrag (const MouseEvent& e)
{
    panel.dragConnector (e);
}

void PinComponent::mouseUp (const MouseEvent& e)
{
    panel.endDraggingConnector (e);
}

