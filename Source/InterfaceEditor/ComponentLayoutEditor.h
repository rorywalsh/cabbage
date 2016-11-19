/*
  ==============================================================================

    ComponentLayoutEditor.h
    Created: 26 Oct 2016 9:18:16pm
    Author:  rory

  ==============================================================================
*/

#ifndef COMPONENTLAYOUTEDITOR_H_INCLUDED
#define COMPONENTLAYOUTEDITOR_H_INCLUDED


/*
 *  ComponentLayoutEditor.h
 *
 *  Original written by Haydxn
 *  Modified by Jordan Hochenbaum on 10/25/10.
 *  http://www.rawmaterialsoftware.com/viewtopic.php?f=6&t=2635
 *
 */

#include "../CabbageCommonHeaders.h"

class ComponentOverlay;
class CabbagePluginEditor;
class ComponentLayoutEditor;
//=============================================================================
class SelectedComponents   : public SelectedItemSet<ComponentOverlay*>
{
public:
    void itemSelected (ComponentOverlay* item);
    void itemDeselected (ComponentOverlay* item);
};

//=============================================================================
class ComponentLayoutEditor   :   public Component, public LassoSource <ComponentOverlay*>
{
public:
    ComponentLayoutEditor (ValueTree valueTree);
    ~ComponentLayoutEditor ();

    ValueTree widgetData;
    void resized ();
    void paint (Graphics& g);
    void setTargetComponent (Component* target);
    void bindWithTarget ();
    void updateFrames ();
    void enablementChanged ();
    void mouseUp(const MouseEvent& e);
    void mouseDrag(const MouseEvent& e);
    void mouseDown(const MouseEvent& e);
    const Component* getTarget();
    void findLassoItemsInArea (Array <ComponentOverlay*>& results, const juce::Rectangle<int>& area);
    void updateCodeEditor();
    void updateSelectedComponentBounds();
    void setComponentBoundsProperties(Component* child, Rectangle<int> bounds);

    SelectedItemSet <ComponentOverlay*>& getLassoSelection();
    LassoComponent <ComponentOverlay*> lassoComp;
    SelectedComponents selectedComponents;
    Point<int> currentMouseCoors;
    void resetAllInterest();
    CabbagePluginEditor* getPluginEditor();

private:
    virtual ComponentOverlay* createAlias (Component* child);
    SafePointer<Component> target;
    OwnedArray<ComponentOverlay> frames;


};


#endif  // COMPONENTLAYOUTEDITOR_H_INCLUDED
