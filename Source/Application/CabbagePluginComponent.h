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
class PinComponent;

//==============================================================================
class CabbagePluginComponent    : public Component
{
public:
    CabbagePluginComponent (AudioGraph& p, AudioProcessorGraph::NodeID id);
    ~CabbagePluginComponent();
    void mouseDown (const MouseEvent& e) override;
    void mouseDrag (const MouseEvent& e) override;
    void mouseUp (const MouseEvent& e) override;
    bool hitTest (int x, int y) override;
    void paint (Graphics& g) override;
    void resized() override;
    Point<float> getPinPos (int index, bool isInput) const;
    void update();
    AudioGraph& graph;



    const AudioProcessorGraph::NodeID pluginID;
    OwnedArray<PinComponent> pins;
    int numInputs = 0, numOutputs = 0;
    int pinSize = 16;
    Point<int> originalPos;
    Font font { 13.0f, Font::bold };
    int numIns = 0, numOuts = 0;
    DropShadowEffect shadow;
private:

    CabbageLookAndFeel2 lookAndFeel;
    CabbageGraphComponent* getCabbageGraphComponent() const noexcept;
    CabbagePluginComponent (const CabbagePluginComponent&);
    CabbagePluginComponent& operator= (const CabbagePluginComponent&);
};

//==============================================================================
class ConnectorComponent   : public Component,
    public SettableTooltipClient
{
public:
    ConnectorComponent (CabbageGraphComponent& p);

    void setInput (AudioProcessorGraph::NodeAndChannel newSource);
    void setOutput (AudioProcessorGraph::NodeAndChannel newDest);
    void dragStart (Point<float> pos);
    void dragEnd (Point<float> pos);
    void update();
    void resizeToFit();
    void getPoints (Point<float>& p1, Point<float>& p2) const;
    void paint (Graphics& g) override;
    bool hitTest (int x, int y) override;
    void mouseDown (const MouseEvent&) override;
    void mouseDrag (const MouseEvent& e) override;
    void mouseUp (const MouseEvent& e) override;
    void resized() override;

    AudioProcessorGraph::NodeID sourceFilterID, destFilterID;
    int sourceFilterChannel, destFilterChannel;
    AudioProcessorGraph::Connection connection { { {}, 0 }, { {}, 0 } };
    Point<float> lastInputPos, lastOutputPos;
    
private:
    AudioGraph& graph;
    CabbageGraphComponent& panel;
    
    float lastInputX, lastInputY, lastOutputX, lastOutputY;
    Path linePath, hitPath;
    bool dragging;


    void getDistancesFromEnds (Point<float> p, double& distanceFromStart, double& distanceFromEnd) const
    {
        Point<float> p1, p2;
        getPoints (p1, p2);
        
        distanceFromStart = p1.getDistanceFrom (p);
        distanceFromEnd   = p2.getDistanceFrom (p);
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ConnectorComponent)
};

//=============================================================================
class PinComponent   : public Component,
    public SettableTooltipClient
{
public:
    PinComponent (CabbageGraphComponent& p, AudioProcessorGraph::NodeAndChannel pinToUse, bool isIn);
    void paint (Graphics& g) override;
    void mouseDown (const MouseEvent& e) override;
    void mouseDrag (const MouseEvent& e) override;
    void mouseUp (const MouseEvent& e) override;

    CabbageGraphComponent& panel;
    //AudioGraph& graph;
    AudioProcessorGraph::NodeAndChannel pin;
    const bool isInput;
    int busIdx = 0;

private:
    AudioGraph& graph;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PinComponent)
};

#endif  // CABBAGEPLUGINCOMPONENT_H_INCLUDED
