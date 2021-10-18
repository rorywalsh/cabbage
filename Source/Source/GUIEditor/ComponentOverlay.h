/*
  ==============================================================================

    ComponentOverlay.h
    Created: 18 Nov 2016 1:41:26pm
    Author:  rory

  ==============================================================================
*/

#ifndef COMPONENTOVERLAY_H_INCLUDED
#define COMPONENTOVERLAY_H_INCLUDED

#include "../CabbageCommonHeaders.h"

#include "ComponentLayoutEditor.h"

class CabbagePluginEditor;
class ComponentLayoutEditor;

//=============================================================================
class ComponentOverlay   :   public Component, public KeyListener
{
public:
    ComponentOverlay (Component* targetChild, ComponentLayoutEditor* layoutEditor);
    ~ComponentOverlay () override;
    void resized () override;
    void paint (Graphics& g) override;
    const Component* getTargetChild ();
    void updateFromTarget ();
    void applyToTarget ();
    virtual void userChangedBounds ();
    virtual void userStartedChangingBounds () {}
    virtual void userStoppedChangingBounds () {}
    bool boundsChangedSinceStart ();
    bool keyPressed (const KeyPress& key, Component* originatingComponent) override;

    void mouseEnter (const MouseEvent& e) override;
    void mouseExit (const MouseEvent& e) override;
    void mouseDown (const MouseEvent& e) override;
    void mouseUp (const MouseEvent& e) override;
    void mouseDrag (const MouseEvent& e) override;

    void setInterest (String isInteresting)
    {
        interest = isInteresting;
    }

    CabbagePluginEditor* getPluginEditor();
    void updateBoundsDataForTarget();
    void setBoundsForChildren();
    bool mouseDownSelectStatus = false;

    const Component* getTarget()
    {
        return target;
    }

private:
    CriticalSection bounds;
    std::unique_ptr<ComponentBoundsConstrainer>  constrainer;
    ComponentDragger dragger;
    SafePointer<Component> target;
    Array<juce::Rectangle<int> > childBounds;
    CabbageLookAndFeel2 lookAndFeel;
    String interest;
    bool userAdjusting;
    Rectangle<int> startBounds;
    std::unique_ptr<ComponentBoundsConstrainer> resizeContainer;
    ResizableBorderComponent* resizer;
    ComponentLayoutEditor* layoutEditor;

};


#endif  // COMPONENTOVERLAY_H_INCLUDED
