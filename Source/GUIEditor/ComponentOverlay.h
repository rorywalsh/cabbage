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
class ComponentOverlay   :   public Component
{
public:
    ComponentOverlay (Component* targetChild, ComponentLayoutEditor* layoutEditor);
    ~ComponentOverlay ();
    void resized ();
    void paint (Graphics& g);
    const Component* getTargetChild ();
    void updateFromTarget ();
    void applyToTarget ();
    virtual void userChangedBounds ();
    virtual void userStartedChangingBounds () {};
    virtual void userStoppedChangingBounds () {};
    bool boundsChangedSinceStart ();
    void mouseEnter (const MouseEvent& e);
    void mouseExit (const MouseEvent& e);
    void mouseDown (const MouseEvent& e);
    void mouseUp (const MouseEvent& e);
    void mouseDrag (const MouseEvent& e);

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
    ScopedPointer<ComponentBoundsConstrainer>  constrainer;
    ComponentDragger dragger;
    SafePointer<Component> target;
    Array<juce::Rectangle<int> > childBounds;
    CabbageLookAndFeel2 lookAndFeel;
    String interest;
    bool userAdjusting;
    Rectangle<int> startBounds;
    ScopedPointer<ComponentBoundsConstrainer> resizeContainer;
    ResizableBorderComponent* resizer;
    ComponentLayoutEditor* layoutEditor;

};


#endif  // COMPONENTOVERLAY_H_INCLUDED
