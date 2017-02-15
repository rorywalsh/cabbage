/*
==============================================================================

CabbagePluginComponent.cpp
Created: 25 Jan 2017 3:21:05pm
Author:  rory

==============================================================================
*/

#include "CabbagePluginComponent.h"
#include "CabbageGraphComponent.h"

CabbagePluginComponent::CabbagePluginComponent (AudioGraph& graph_, const uint32 filterID_)
    : graph (graph_),
      filterID (filterID_),
      numInputs (0),
      numOutputs (0),
      pinSize (16),
      font (13.0f, Font::bold),
      numIns (0),
      numOuts (0)
{
    shadow.setShadowProperties (DropShadow (Colours::black.withAlpha (0.5f), 3, Point<int> (0, 1)));
    setComponentEffect (&shadow);

    setSize (150, 60);
}

CabbagePluginComponent::~CabbagePluginComponent()
{
    deleteAllChildren();
}

void CabbagePluginComponent::mouseDown (const MouseEvent& e)
{
    originalPos = localPointToGlobal (Point<int>());

    toFront (true);

    if (e.mods.isPopupMenu())
    {
        PopupMenu m;
        m.addItem (1, "Delete this filter");
        m.addItem (2, "Disconnect all pins");
        m.addSeparator();
        m.addItem (3, "Show plugin UI");
        m.addItem (4, "Show all programs");
        m.addItem (5, "Show all parameters");
        m.addSeparator();
        m.addItem (6, "Configure Audio I/O");
        m.addItem (7, "Test state save/load");

        const int r = m.show();

        if (r == 1)
        {
            //graph.removeFilter (filterID);
            return;
        }
        else if (r == 2)
        {
            //graph.disconnectFilter (filterID);
        }
        else
        {
            if (AudioProcessorGraph::Node::Ptr f = graph.getNodeForId (filterID))
            {
                AudioProcessor* const processor = f->getProcessor();
                jassert (processor != nullptr);

                if (r == 7)
                {
                    MemoryBlock state;
                    processor->getStateInformation (state);
                    processor->setStateInformation (state.getData(), (int) state.getSize());
                }
                else
                {
                    PluginWindow::WindowFormatType type = processor->hasEditor() ? PluginWindow::Normal
                                                          : PluginWindow::Generic;

                    switch (r)
                    {
                        case 4:
                            type = PluginWindow::Programs;
                            break;

                        case 5:
                            type = PluginWindow::Parameters;
                            break;

                        case 6:
                            type = PluginWindow::AudioIO;
                            break;

                        default:
                            break;
                    };

                    if (PluginWindow* const w = PluginWindow::getWindowFor (f, type, graph.getGraph()))
                        w->toFront (true);
                }
            }
        }
    }
}

void CabbagePluginComponent::mouseDrag (const MouseEvent& e)
{
    if (! e.mods.isPopupMenu())
    {
        Point<int> pos (originalPos + Point<int> (e.getDistanceFromDragStartX(), e.getDistanceFromDragStartY()));

        if (getParentComponent() != nullptr)
            pos = getParentComponent()->getLocalPoint (nullptr, pos);

        graph.setNodePosition (filterID,
                               (pos.getX() + getWidth() / 2) / (double) getParentWidth(),
                               (pos.getY() + getHeight() / 2) / (double) getParentHeight());

        getCabbageGraphComponent()->updateComponents();
		getCabbageGraphComponent()->repaint();
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
        if (const AudioProcessorGraph::Node::Ptr f = graph.getNodeForId (filterID))
            if (PluginWindow* const w = PluginWindow::getWindowFor (f, PluginWindow::Normal, graph.getGraph()))
                w->toFront (true);
    }
}

bool CabbagePluginComponent::hitTest (int x, int y)
{
    for (int i = getNumChildComponents(); --i >= 0;)
        if (getChildComponent (i)->getBounds().contains (x, y))
            return true;

    return x >= 3 && x < getWidth() - 6 && y >= pinSize && y < getHeight() - pinSize;
}

void CabbagePluginComponent::paint (Graphics& g)
{
    const int x = 4;
    const int y = pinSize;
    const int w = getWidth() - x * 2;
    const int h = getHeight() - pinSize * 2;

    g.setColour(CabbageUtilities::getComponentSkin());
    //g.setColour(Colour(220, 220, 220));
    g.fillRoundedRectangle(x, y, w, h, 5);

    g.drawRoundedRectangle(x, y, w, h, 5, 1.f);
    g.setColour (Colour(120, 120, 120));
    g.setFont (CabbageUtilities::getComponentFont());
    g.drawFittedText (getName(),
                      x + 4, y - 2, w - 8, h - 4,
                      Justification::centred, 2);

    g.setOpacity(0.2);
    g.setColour(Colours::green.withAlpha(.3f));
    g.drawRoundedRectangle(x+0.5, y+0.5, w-1, h-1, 5, 1.0f);
}

void CabbagePluginComponent::resized()
{
    if (AudioProcessorGraph::Node::Ptr f = graph.getNodeForId (filterID))
    {
        if (AudioProcessor* const processor = f->getProcessor())
        {
            for (int i = 0; i < getNumChildComponents(); ++i)
            {
                if (PinComponent* const pc = dynamic_cast<PinComponent*> (getChildComponent (i)))
                {
                    const bool isInput = pc->isInput;
                    int busIdx, channelIdx;

                    channelIdx =
                        processor->getOffsetInBusBufferForAbsoluteChannelIndex (isInput, pc->index, busIdx);

                    const int total = isInput ? numIns : numOuts;
                    const int index = pc->index == AudioGraph::midiChannelNumber ? (total - 1) : pc->index;

                    const float totalSpaces = static_cast<float> (total) + (static_cast<float> (jmax (0, processor->getBusCount (isInput) - 1)) * 0.5f);
                    const float indexPos = static_cast<float> (index) + (static_cast<float> (busIdx) * 0.5f);

                    pc->setBounds (proportionOfWidth ((1.0f + indexPos) / (totalSpaces + 1.0f)) - pinSize / 2,
                                   pc->isInput ? 0 : (getHeight() - pinSize),
                                   pinSize, pinSize);
                }
            }
        }
    }
}

void CabbagePluginComponent::getPinPos (const int index, const bool isInput, float& x, float& y)
{
    for (int i = 0; i < getNumChildComponents(); ++i)
    {
        if (PinComponent* const pc = dynamic_cast<PinComponent*> (getChildComponent (i)))
        {
            if (pc->index == index && isInput == pc->isInput)
            {
                x = getX() + pc->getX() + pc->getWidth() * 0.5f;
                y = getY() + pc->getY() + pc->getHeight() * 0.5f;
                break;
            }
        }
    }
}

void CabbagePluginComponent::update()
{
    const AudioProcessorGraph::Node::Ptr f (graph.getNodeForId (filterID));

    if (f == nullptr)
    {
        delete this;
        return;
    }

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

	if(CabbagePluginProcessor* cabbagePlugin = dynamic_cast<CabbagePluginProcessor*>(f->getProcessor()))
		setName (cabbagePlugin->getPluginName());
	else
		setName (f->getProcessor()->getName());

    {
        Point<double> p = graph.getNodePosition (filterID);
        setCentreRelative ((float) p.x, (float) p.y);
    }

    if (numIns != numInputs || numOuts != numOutputs)
    {
        numInputs = numIns;
        numOutputs = numOuts;

        deleteAllChildren();

        int i;

        for (i = 0; i < f->getProcessor()->getTotalNumInputChannels(); ++i)
            addAndMakeVisible (new PinComponent (graph, filterID, i, true));

        if (f->getProcessor()->acceptsMidi())
            addAndMakeVisible (new PinComponent (graph, filterID, AudioGraph::midiChannelNumber, true));

        for (i = 0; i < f->getProcessor()->getTotalNumOutputChannels(); ++i)
            addAndMakeVisible (new PinComponent (graph, filterID, i, false));

        if (f->getProcessor()->producesMidi())
            addAndMakeVisible (new PinComponent (graph, filterID, AudioGraph::midiChannelNumber, false));

        resized();
    }
	
	repaint();
}

CabbageGraphComponent* CabbagePluginComponent::getCabbageGraphComponent() const noexcept
{
    return findParentComponentOfClass<CabbageGraphComponent>();
}

//==================================================================================================================
ConnectorComponent::ConnectorComponent (AudioGraph& graph_)
    : sourceFilterID (0),
      destFilterID (0),
      sourceFilterChannel (0),
      destFilterChannel (0),
      graph (graph_),
      lastInputX (0),
      lastInputY (0),
      lastOutputX (0),
      lastOutputY (0)
{
    setAlwaysOnTop (true);
}

void ConnectorComponent::setInput (const uint32 sourceFilterID_, const int sourceFilterChannel_)
{
    if (sourceFilterID != sourceFilterID_ || sourceFilterChannel != sourceFilterChannel_)
    {
        sourceFilterID = sourceFilterID_;
        sourceFilterChannel = sourceFilterChannel_;
        update();
    }
}

void ConnectorComponent::setOutput (const uint32 destFilterID_, const int destFilterChannel_)
{
    if (destFilterID != destFilterID_ || destFilterChannel != destFilterChannel_)
    {
        destFilterID = destFilterID_;
        destFilterChannel = destFilterChannel_;
        update();
    }
}

void ConnectorComponent::dragStart (int x, int y)
{
    lastInputX = (float) x;
    lastInputY = (float) y;
    resizeToFit();
}

void ConnectorComponent::dragEnd (int x, int y)
{
    lastOutputX = (float) x;
    lastOutputY = (float) y;
    resizeToFit();
}

void ConnectorComponent::update()
{
    float x1, y1, x2, y2;
    getPoints (x1, y1, x2, y2);

    if (lastInputX != x1
        || lastInputY != y1
        || lastOutputX != x2
        || lastOutputY != y2)
    {
        resizeToFit();
    }
}

void ConnectorComponent::resizeToFit()
{
    float x1, y1, x2, y2;
    getPoints (x1, y1, x2, y2);

    const Rectangle<int> newBounds ((int) jmin (x1, x2) - 4,
                                    (int) jmin (y1, y2) - 4,
                                    (int) std::abs (x1 - x2) + 8,
                                    (int) std::abs (y1 - y2) + 8);

    if (newBounds != getBounds())
        setBounds (newBounds);
    else
        resized();

    repaint();
}

void ConnectorComponent::getPoints (float& x1, float& y1, float& x2, float& y2) const
{
    x1 = lastInputX;
    y1 = lastInputY;
    x2 = lastOutputX;
    y2 = lastOutputY;

    if (CabbageGraphComponent* const hostPanel = getCabbageGraphComponent())
    {
        if (CabbagePluginComponent* srcFilterComp = hostPanel->getComponentForFilter (sourceFilterID))
            srcFilterComp->getPinPos (sourceFilterChannel, false, x1, y1);

        if (CabbagePluginComponent* dstFilterComp = hostPanel->getComponentForFilter (destFilterID))
            dstFilterComp->getPinPos (destFilterChannel, true, x2, y2);
    }
}

void ConnectorComponent::paint (Graphics& g)
{
    if (sourceFilterChannel == AudioGraph::midiChannelNumber
        || destFilterChannel == AudioGraph::midiChannelNumber)
    {
        g.setColour (Colours::red);
    }
    else
    {
        g.setColour (Colours::green);
    }

    g.fillPath (linePath);
}

bool ConnectorComponent::hitTest (int x, int y)
{
    if (hitPath.contains ((float) x, (float) y))
    {
        double distanceFromStart, distanceFromEnd;
        getDistancesFromEnds (x, y, distanceFromStart, distanceFromEnd);

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
        getCabbageGraphComponent()->dragConnector (e);
    }
    else if (e.mouseWasDraggedSinceMouseDown())
    {
        dragging = true;

        graph.removeConnection (sourceFilterID, sourceFilterChannel, destFilterID, destFilterChannel);

        double distanceFromStart, distanceFromEnd;
        getDistancesFromEnds (e.x, e.y, distanceFromStart, distanceFromEnd);
        const bool isNearerSource = (distanceFromStart < distanceFromEnd);

        getCabbageGraphComponent()->beginConnectorDrag (isNearerSource ? 0 : sourceFilterID,
                                                        sourceFilterChannel,
                                                        isNearerSource ? destFilterID : 0,
                                                        destFilterChannel,
                                                        e);
    }
}

void ConnectorComponent::mouseUp (const MouseEvent& e)
{
    if (dragging)
        getCabbageGraphComponent()->endDraggingConnector (e);
}

void ConnectorComponent::resized()
{
    float x1, y1, x2, y2;
    getPoints (x1, y1, x2, y2);

    lastInputX = x1;
    lastInputY = y1;
    lastOutputX = x2;
    lastOutputY = y2;

    x1 -= getX();
    y1 -= getY();
    x2 -= getX();
    y2 -= getY();

    linePath.clear();
    linePath.startNewSubPath (x1, y1);
    linePath.cubicTo (x1, y1 + (y2 - y1) * 0.33f,
                      x2, y1 + (y2 - y1) * 0.66f,
                      x2, y2);

    PathStrokeType wideStroke (8.0f);
    wideStroke.createStrokedPath (hitPath, linePath);

    PathStrokeType stroke (2.5f);
    stroke.createStrokedPath (linePath, linePath);

    const float arrowW = 5.0f;
    const float arrowL = 4.0f;

    Path arrow;
    arrow.addTriangle (-arrowL, arrowW,
                       -arrowL, -arrowW,
                       arrowL, 0.0f);

    arrow.applyTransform (AffineTransform()
                          .rotated (float_Pi * 0.5f - (float) atan2 (x2 - x1, y2 - y1))
                          .translated ((x1 + x2) * 0.5f,
                                       (y1 + y2) * 0.5f));

    linePath.addPath (arrow);
    linePath.setUsingNonZeroWinding (true);
}

CabbageGraphComponent* ConnectorComponent::getCabbageGraphComponent() const noexcept
{
    return findParentComponentOfClass<CabbageGraphComponent>();
}

//===================================================================================
PinComponent::PinComponent (AudioGraph& graph_,
                            const uint32 filterID_, const int index_, const bool isInput_)
    : filterID (filterID_),
      index (index_),
      isInput (isInput_),
      busIdx (0),
      graph (graph_)
{
    if (const AudioProcessorGraph::Node::Ptr node = graph.getNodeForId (filterID_))
    {
        String tip;

        if (index == AudioGraph::midiChannelNumber)
        {
            tip = isInput ? "MIDI Input"
                  : "MIDI Output";
        }
        else
        {
            const AudioProcessor& processor = *node->getProcessor();

            int channel;
            channel = processor.getOffsetInBusBufferForAbsoluteChannelIndex (isInput, index, busIdx);

            if (const AudioProcessor::Bus* bus = processor.getBus (isInput, busIdx))
                tip = bus->getName() + String (": ")
                      + AudioChannelSet::getAbbreviatedChannelTypeName (bus->getCurrentLayout().getTypeOfChannel (channel));
            else
                tip = (isInput ? "Main Input: "
                       : "Main Output: ") + String (index + 1);

        }

        setTooltip (tip);
    }

    setSize (16, 16);
}

void PinComponent::paint (Graphics& g)
{
    const float w = (float) getWidth();
    const float h = (float) getHeight();

    Path p;
    p.addEllipse (w * 0.25f, h * 0.25f, w * 0.5f, h * 0.5f);

    p.addRectangle (w * 0.4f, isInput ? (0.5f * h) : 0.0f, w * 0.2f, h * 0.5f);

    g.setColour (index == AudioGraph::midiChannelNumber ? Colours::cornflowerblue : Colours::green);
    g.fillPath (p);
	
}

void PinComponent::mouseDown (const MouseEvent& e)
{
    getCabbageGraphComponent()->beginConnectorDrag (isInput ? 0 : filterID,
                                                    index,
                                                    isInput ? filterID : 0,
                                                    index,
                                                    e);
}

void PinComponent::mouseDrag (const MouseEvent& e)
{
    getCabbageGraphComponent()->dragConnector (e);
}

void PinComponent::mouseUp (const MouseEvent& e)
{
    getCabbageGraphComponent()->endDraggingConnector (e);
}

CabbageGraphComponent* PinComponent::getCabbageGraphComponent() const noexcept
{
    return findParentComponentOfClass<CabbageGraphComponent>();
}