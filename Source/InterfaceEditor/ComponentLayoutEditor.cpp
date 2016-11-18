/*
  ==============================================================================

    ComponentLayoutEditor.cpp
    Created: 26 Oct 2016 9:18:16pm
    Author:  rory

  ==============================================================================
*/

#include "ComponentLayoutEditor.h"
#include "ComponentOverlay.h"
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
:   target (0), widgetData(valueTree)
{
    setInterceptsMouseClicks (false, true);	
}

ComponentLayoutEditor::~ComponentLayoutEditor ()
{
	target = nullptr;

}

CabbagePluginEditor* ComponentLayoutEditor::getPluginEditor()
{
	if(CabbagePluginEditor* c = this->findParentComponentOfClass<CabbagePluginEditor>())
		return c;
	else
		return nullptr;
}

void ComponentLayoutEditor::resized ()
{
	for (int i=0; i<frames.size(); i++)
	{
		frames.getUnchecked(i)->updateFromTarget ();
	}
}

void ComponentLayoutEditor::paint (Graphics& g)
{
}

void ComponentLayoutEditor::resetAllInterest()
{    
	for(int i=0; i<frames.size(); i++)
    {
		if(ComponentOverlay* child = dynamic_cast<ComponentOverlay*>(frames[i]))
			child->setInterest("none");
    }
    repaint();
}
//==================================================================================================================
void ComponentLayoutEditor::updateSelectedComponentBounds()
{
    for(ComponentOverlay* child : selectedComponents)
    {	
		child->setInterest("selected");
		child->repaint();
		setComponentBoundsProperties(child, child->getBounds());
	}		
}

void ComponentLayoutEditor::setComponentBoundsProperties(Component* child, Rectangle<int> bounds)
{
	child->getProperties().set("originalX", bounds.getX());
	child->getProperties().set("originalY", bounds.getY());
	child->getProperties().set("originalWidth", bounds.getWidth());
	child->getProperties().set("originalHeight", bounds.getHeight());	
}

//==================================================================================================================
void ComponentLayoutEditor::updateCodeEditor()
{
	StringArray compNames;
	
	if(getLassoSelection().getNumSelected()>0)
		for ( ComponentOverlay* child : getLassoSelection() )
		{
			compNames.add(child->getName());
			for( int i = 0 ; i < child->getTarget()->getNumChildComponents() ; i++)
			{
				if(child->getTarget()->getChildComponent(i)->getName().isNotEmpty())
					compNames.add(child->getTarget()->getChildComponent(i)->getName());
			}
		}
		else
			compNames.add(getName());	
		
		
	getPluginEditor()->setCurrentlySelectedComponents(compNames);
	getPluginEditor()->sendChangeMessage();	
}

//==================================================================================================================
void ComponentLayoutEditor::mouseUp(const MouseEvent& e)
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
    resetAllInterest();

    if(e.mods.isPopupMenu())
    {
		PopupMenu menu;
		CabbagePopupWidgets widgets;
		for( int i = 0 ; i < widgets.size() ; i++ )
			menu.addItem(i+1, widgets.getAllKeys()[i]);
			
		const int result = menu.show();		
		getPluginEditor()->addNewWidget(widgets.getAllValues()[result-1], e.getPosition());		
		currentMouseCoors = e.getPosition();	
    }
    else
    {
        addChildComponent (&lassoComp);
        lassoComp.beginLasso (e, this);
    }
}

//==================================================================================================================
void ComponentLayoutEditor::findLassoItemsInArea (Array <ComponentOverlay*>& results, const Rectangle<int>& area)
{
    const Rectangle<int> lasso (area);

    for (int i = 0; i < getNumChildComponents()-1; i++)
    {
        ComponentOverlay* c = (ComponentOverlay*)getChildComponent(i);
        if (c->getBounds().intersects (lasso))
        {
            results.addIfNotAlreadyThere(c);
            selectedComponents.addToSelection(c);
        }
        else
            selectedComponents.deselect(c);
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
	jassert (targetComp->getParentComponent() == getParentComponent());
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
	frames.clear ();
	if (target != NULL)
	{
		Component* t = (Component*) target.getComponent ();
		int n = t->getNumChildComponents ();
		for (int i=0; i<n; i++)
		{
			Component* c = t->getChildComponent (i);
			if (c)
			{
                ComponentOverlay* alias = createAlias (c);
                if (alias)
                {
					alias->setName(c->getName());
					setComponentBoundsProperties(alias, alias->getBounds());
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
