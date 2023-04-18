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
#ifndef Cabbage_Lite
    class CabbageMainComponent;
#endif
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
    ~ComponentLayoutEditor() override;

    ValueTree widgetData;
    void resized () override;
    void paint (Graphics& g)  override;
    void setTargetComponent (Component* target);
    void bindWithTarget ();
    void updateFrames ();
    void enablementChanged () override;
    void mouseUp (const MouseEvent& e) override;
    void mouseDrag (const MouseEvent& e) override;
    void mouseDown (const MouseEvent& e) override;
    const Component* getTarget();
    void findLassoItemsInArea (Array <ComponentOverlay*>& results, const juce::Rectangle<int>& area) override;
    void updateCodeEditor();
    void updateSelectedComponentBounds();
    void setComponentBoundsProperties (Component* child, juce::Rectangle<int> bounds);

    SelectedItemSet <ComponentOverlay*>& getLassoSelection() override;
    LassoComponent <ComponentOverlay*> lassoComp;
    SelectedComponents selectedComponents;
    juce::Point<int> currentMouseCoors;
    void resetAllInterest();
    CabbagePluginEditor* getPluginEditor();
#ifndef Cabbage_Lite
    CabbageMainComponent* getContentComponent();
#endif
private:
    virtual ComponentOverlay* createAlias (Component* child);
    SafePointer<Component> target;
    OwnedArray<ComponentOverlay> frames;
    CabbageLookAndFeel2 lookAndFeel;


};


#endif  // COMPONENTLAYOUTEDITOR_H_INCLUDED
