/*
  ==============================================================================

    ComponentLayoutEditor.cpp
    Created: 26 Oct 2016 9:18:16pm
    Author:  rory

  ==============================================================================
*/

#include "ComponentLayoutEditor.h"
#include "ComponentOverlay.h"
#ifndef Cabbage_Lite
    #include "../Settings/CabbageSettings.h"
    #include "../Application/CabbageMainComponent.h"
#endif
#include "../Audio/Plugins/CabbagePluginEditor.h"

/*
 *  ComponentLayoutEditor.cpp
 *
 *  Original written by Haydxn
 *  Modified by Jordan Hochenbaum on 10/25/10.
 *  Further modified by Rory Walsh 2016
 *  http://www.rawmaterialsoftware.com/viewtopic.php?f=6&t=2635
 *
 */

//this get populated whenever we select multiple objects..
void SelectedComponents::itemSelected (ComponentOverlay* item)
{
    item->repaint ();
}

void SelectedComponents::itemDeselected (ComponentOverlay* item)
{
    item->repaint ();
}

//=============================================================================
ComponentLayoutEditor::ComponentLayoutEditor (ValueTree valueTree)
    :   target (0), widgetData (valueTree), lookAndFeel()
{
    setInterceptsMouseClicks (false, true);
}

ComponentLayoutEditor::~ComponentLayoutEditor ()
{
    target = nullptr;

}

CabbagePluginEditor* ComponentLayoutEditor::getPluginEditor()
{
    if (CabbagePluginEditor* c = this->findParentComponentOfClass<CabbagePluginEditor>())
        return c;
    else
        return nullptr;
}

#ifndef Cabbage_Lite
CabbageMainComponent* ComponentLayoutEditor::getContentComponent()
{
    if (CabbageMainComponent* c = this->findParentComponentOfClass<CabbageMainComponent>())
        return c;
    else

        return nullptr;
}
#endif

void ComponentLayoutEditor::resized ()
{
    for (int i = 0; i < frames.size(); i++)
    {
        frames.getUnchecked (i)->updateFromTarget ();
    }
}

void ComponentLayoutEditor::paint (Graphics& g)
{
}

void ComponentLayoutEditor::resetAllInterest()
{
    for (int i = 0; i < frames.size(); i++)
    {
        if (ComponentOverlay* child = dynamic_cast<ComponentOverlay*> (frames[i]))
            child->setInterest ("none");
    }

    repaint();
}

//==================================================================================================================
void ComponentLayoutEditor::updateSelectedComponentBounds()
{
    for (ComponentOverlay* child : selectedComponents)
    {
        child->setInterest ("selected");
        child->repaint();
        setComponentBoundsProperties (child, child->getBounds());
    }
}

void ComponentLayoutEditor::setComponentBoundsProperties (Component* child, juce::Rectangle<int> bounds)
{
    child->getProperties().set ("originalX", bounds.getX());
    child->getProperties().set ("originalY", bounds.getY());
    child->getProperties().set ("originalWidth", bounds.getWidth());
    child->getProperties().set ("originalHeight", bounds.getHeight());
}

//==================================================================================================================
void ComponentLayoutEditor::updateCodeEditor()
{
    StringArray compNames;

    if (getLassoSelection().getNumSelected() > 0)
        for ( ComponentOverlay* child : getLassoSelection() )
        {
            compNames.add (child->getName());

            for ( int i = 0 ; i < child->getTarget()->getNumChildComponents() ; i++)
            {
                if (child->getTarget()->getChildComponent (i)->getName().isNotEmpty()){
                    compNames.add (child->getTarget()->getChildComponent (i)->getName());
                    CabbageUtilities::debug(child->getTarget()->getChildComponent (i)->getName());
                }
            }
        }
    else
        compNames.add (getName());


    getPluginEditor()->setCurrentlySelectedComponents (compNames);
    getPluginEditor()->sendChangeMessage();
}

//==================================================================================================================
void ComponentLayoutEditor::mouseUp (const MouseEvent& e)
{

    updateSelectedComponentBounds();
    lassoComp.endLasso();
    removeChildComponent (&lassoComp);
}

void ComponentLayoutEditor::mouseDrag (const MouseEvent& e)
{
    selectedComponents.deselectAll();
    lassoComp.toFront (false);
    lassoComp.dragLasso (e);
}

void ComponentLayoutEditor::mouseDown (const MouseEvent& e)
{
    selectedComponents.deselectAll();
    Array<File> customPlants;
    resetAllInterest();

    if (e.mods.isPopupMenu())
    {
        PopupMenu menu;
        menu.setLookAndFeel (&lookAndFeel);
        CabbagePopupWidgets widgets;

        PopupMenu subM;
        subM.addItem (1, "Button");
        subM.addItem (2, "Button - File");
        subM.addItem (3, "Button - Info");
        menu.addSubMenu ("Buttons", subM);
        subM.clear();


        menu.addItem (4, "Checkbox");
        menu.addItem (5, "ComboBox");

        menu.addItem (6, "Csound Output");
        menu.addItem (7, "CV Input - VCVRack");
        menu.addItem (8, "CV Output - VCVRack");

        menu.addItem (9, "Endless Encoder");
        menu.addItem (10, "Gentable");
        menu.addItem (11, "Groupbox");

        menu.addItem (13, "Image");
        menu.addItem (14, "Keyboard");
        menu.addItem (15, "Label");
        menu.addItem (16, "Light - VCVRack");

        menu.addItem (30, "Listbox");
        subM.addItem (12, "Horizontal Meter");
        subM.addItem (28, "Vertical Meter");
        menu.addSubMenu( "Meters", subM);

        subM.clear();
        
        menu.addItem (17, "Signal Display");

        subM.addItem (18, "Slider - Horizontal");
        subM.addItem (19, "Slider - Rotary");
        subM.addItem (20, "Slider - Vertical");
        subM.addItem (21, "Slider - Range - Vertical");
        subM.addItem (22, "Slider - Range - Horizontal");
        subM.addItem (23, "Slider - Number Box");
        menu.addSubMenu("Sliders", subM);
        subM.clear();

        menu.addItem (24, "Soundfiler");
        menu.addItem (25, "Event Sequencer");
        menu.addItem (26, "Text Box");
        menu.addItem (27, "Text Editor");
        menu.addItem (29, "XY Pad");



        CabbageSettings settings;
        settings.setStorageParameters(CabbageUtilities::getStorageProps());
        settings.setDefaultSettings();
        const String plantDir = settings.getUserSettings()->getValue("CabbagePlantDir");
        CabbageUtilities::debug(plantDir);

        PopupMenu subMenu;
        CabbageUtilities::addCustomPlantsToMenu(subMenu, customPlants, plantDir);
        menu.addSubMenu("Custom Plants", subMenu);
        
        menu.showMenuAsync(juce::PopupMenu::Options(), [this, e, customPlants, widgets](int result) {
            if (result > 0 && result < 100)
                this->getPluginEditor()->addNewWidget (widgets.getAllValues()[result - 1], e.getPosition());
            else
                this->getPluginEditor()->addNewWidget (customPlants[result-100].getFullPathName(), e.getPosition(), true);
            currentMouseCoors = e.getPosition();
        });
    }
    else
    {
        addChildComponent (&lassoComp);
        lassoComp.beginLasso (e, this);
    }
}

//==================================================================================================================
void ComponentLayoutEditor::findLassoItemsInArea (Array <ComponentOverlay*>& results, const juce::Rectangle<int>& area)
{
    const juce::Rectangle<int> lasso (area);

    for (int i = 0; i < getNumChildComponents() - 1; i++)
    {
        ComponentOverlay* c = (ComponentOverlay*)getChildComponent (i);

        if (c->getBounds().intersects (lasso))
        {
            results.addIfNotAlreadyThere (c);
            selectedComponents.addToSelection (c);
        }
        else
            selectedComponents.deselect (c);
    }
}

SelectedItemSet <ComponentOverlay*>& ComponentLayoutEditor::getLassoSelection()
{
    return selectedComponents;
}

//==================================================================================================================
void ComponentLayoutEditor::setTargetComponent (Component* targetComp)
{
    jassert (targetComp);
    //jassert (targetComp->getParentComponent() == getParentComponent());

    if (target)
    {
        if (target.getComponent() == targetComp) return;

        deleteAndZero (target);
    }

    target = targetComp;
    bindWithTarget ();
}

void ComponentLayoutEditor::bindWithTarget ()
{
    if (target != NULL)
    {
        Component* t = (Component*) target.getComponent ();
        Component* p = t->getParentComponent ();
        p->addAndMakeVisible (this);
        setBounds (t->getBounds ());
        updateFrames ();
    }
}

void ComponentLayoutEditor::updateFrames ()
{
    selectedComponents.deselectAll();
    frames.clear ();

    if (target != NULL)
    {
        Component* t = (Component*) target.getComponent ();
        int n = t->getNumChildComponents ();

        for (int i = 0; i < n; i++)
        {
            Component* c = t->getChildComponent (i);

            if (c)
            {
                ComponentOverlay* alias = createAlias (c);

                if (alias)
                {
                    alias->setName (c->getName());
                    setComponentBoundsProperties (alias, alias->getBounds());
                    frames.add (alias);
                    addAndMakeVisible (alias);
                }
            }
        }
    }
}

void ComponentLayoutEditor::enablementChanged ()
{
    if (isEnabled ())
    {
        setVisible (true);
    }
    else
    {
        setVisible (false);
    }
}

const Component* ComponentLayoutEditor::getTarget ()
{
    if (target) return target.getComponent ();

    return 0;
}

ComponentOverlay* ComponentLayoutEditor::createAlias (Component* child)
{
    return new ComponentOverlay (child, this);
}
