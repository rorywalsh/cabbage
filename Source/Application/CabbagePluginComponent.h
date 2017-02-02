/*
  ==============================================================================

    CabbagePluginComponent.h
    Created: 25 Jan 2017 3:21:05pm
    Author:  rory

  ==============================================================================
*/

#ifndef CABBAGEPLUGINCOMPONENT_H_INCLUDED
#define CABBAGEPLUGINCOMPONENT_H_INCLUDED

#include "../Audio/Graph/AudioGraph.h"
class CabbageGraphComponent;

//==============================================================================
class CabbagePluginComponent    : public Component
{
public:
    CabbagePluginComponent (AudioGraph& graph_, const uint32 filterID_);
    ~CabbagePluginComponent();
    void mouseDown (const MouseEvent& e) override;
    void mouseDrag (const MouseEvent& e) override;
    void mouseUp (const MouseEvent& e) override;
    bool hitTest (int x, int y) override;
    void paint (Graphics& g) override;
    void resized() override;
    void getPinPos (const int index, const bool isInput, float& x, float& y);
    void update();
    AudioGraph& graph;
    const uint32 filterID;
    int numInputs, numOutputs;

private:
    int pinSize;
    Point<int> originalPos;
    Font font;
    int numIns, numOuts;
    DropShadowEffect shadow;

    CabbageGraphComponent* getCabbageGraphComponent() const noexcept;
    CabbagePluginComponent (const CabbagePluginComponent&);
    CabbagePluginComponent& operator= (const CabbagePluginComponent&);
};

//==============================================================================
class ConnectorComponent   : public Component,
    public SettableTooltipClient
{
public:
    ConnectorComponent (AudioGraph& graph_);

    void setInput (const uint32 sourceFilterID_, const int sourceFilterChannel_);
    void setOutput (const uint32 destFilterID_, const int destFilterChannel_);
    void dragStart (int x, int y);
    void dragEnd (int x, int y);
    void update();
    void resizeToFit();
    void getPoints (float& x1, float& y1, float& x2, float& y2) const;
    void paint (Graphics& g) override;
    bool hitTest (int x, int y) override;
    void mouseDown (const MouseEvent&) override;
    void mouseDrag (const MouseEvent& e) override;
    void mouseUp (const MouseEvent& e) override;
    void resized() override;

    uint32 sourceFilterID, destFilterID;
    int sourceFilterChannel, destFilterChannel;

private:
    AudioGraph& graph;
    float lastInputX, lastInputY, lastOutputX, lastOutputY;
    Path linePath, hitPath;
    bool dragging;

    CabbageGraphComponent* getCabbageGraphComponent() const noexcept;

    void getDistancesFromEnds (int x, int y, double& distanceFromStart, double& distanceFromEnd) const
    {
        float x1, y1, x2, y2;
        getPoints (x1, y1, x2, y2);

        distanceFromStart = juce_hypot (x - (x1 - getX()), y - (y1 - getY()));
        distanceFromEnd = juce_hypot (x - (x2 - getX()), y - (y2 - getY()));
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ConnectorComponent)
};

//=============================================================================
class PinComponent   : public Component,
    public SettableTooltipClient
{
public:
    PinComponent (AudioGraph& graph_, const uint32 filterID_, const int index_, const bool isInput_);
    void paint (Graphics& g) override;
    void mouseDown (const MouseEvent& e) override;
    void mouseDrag (const MouseEvent& e) override;
    void mouseUp (const MouseEvent& e) override;

    const uint32 filterID;
    const int index;
    const bool isInput;
    int busIdx;

private:
    AudioGraph& graph;

    CabbageGraphComponent* getCabbageGraphComponent() const noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PinComponent)
};

#endif  // CABBAGEPLUGINCOMPONENT_H_INCLUDED
