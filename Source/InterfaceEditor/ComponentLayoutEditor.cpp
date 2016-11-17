/*
  ==============================================================================

    ComponentLayoutEditor.cpp
    Created: 26 Oct 2016 9:18:16pm
    Author:  rory

  ==============================================================================
*/

#include "ComponentLayoutEditor.h"
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
void SelectedComponents::itemSelected (ChildAlias* item)
{
    item->repaint ();
}

void SelectedComponents::itemDeselected (ChildAlias* item)
{
    item->repaint ();
}

#include "ComponentLayoutEditor.h"

ChildAlias::ChildAlias (Component* targetChild)
:   target (targetChild)
{	
	resizeContainer = new ComponentBoundsConstrainer();
	resizeContainer->setMinimumSize(10, 10); //set minimum size so objects cant be resized too small
	resizer = new ResizableBorderComponent (this,resizeContainer);
	addAndMakeVisible (resizer);
	resizer->addMouseListener (this,false);
	constrainer = new ComponentBoundsConstrainer();
	interest = false;
	userAdjusting = false;
	updateFromTarget ();
	setRepaintsOnMouseActivity (true);
}

ChildAlias::~ChildAlias ()
{
	delete resizer;
}

void ChildAlias::resized ()
{
	resizer->setBounds (0,0,getWidth(),getHeight());
	if (resizer->isMouseButtonDown ())
	{
		applyToTarget ();
	}
}

void ChildAlias::paint (Graphics& g)
{
	
	Colour c = Colours::white;
	if (interest=="selected")
	{
		Path selectedRect;
		selectedRect.addRectangle(getLocalBounds().reduced(2));
		g.setColour(c);
		

		const float dashLengths[] = { 10.0f, 10.0f };
		PathStrokeType stroke(1.0, PathStrokeType::mitered);
		stroke.createDashedStroke (selectedRect, selectedRect, dashLengths, 2);		
        g.strokePath(selectedRect, stroke);
	}

	g.setColour (c);
	g.drawRect (0,0,getWidth(),getHeight(),1);
}

//===========================================================================
SelectedItemSet <ChildAlias*>& ChildAlias::getLassoSelection()
{
	if(ComponentLayoutEditor* c = this->findParentComponentOfClass<ComponentLayoutEditor>())
	{
		return c->getLassoSelection();
	}
}

const Component* ChildAlias::getTargetChild ()
{
	return target.getComponent ();
}
//===========================================================================
void ChildAlias::updateFromTarget ()
{
	if (target != NULL)
		//if (!target.hasBeenDeleted ())
	{
		setBounds ( target.getComponent ()->getBounds () );
	}
}

void ChildAlias::applyToTarget ()
{
	if (target != NULL)
		//!target.hasBeenDeleted ())
	{
		Component* c = (Component*) target.getComponent ();
		//c->toFront(false); //added this to bring the the component to the front
		c->setBounds (getBounds ());
		userChangedBounds ();
	}
}
//===========================================================================
void ChildAlias::userChangedBounds ()
{
	//update minimum onscreen amounts so that object can't be resized past the screen area
	resizeContainer->setMinimumOnscreenAmounts(getHeight()+target.getComponent()->getHeight(), getWidth()+target.getComponent()->getWidth(), 
											   getHeight()+target.getComponent()->getHeight(), getWidth()+target.getComponent()->getWidth());
}

void ChildAlias::userStartedChangingBounds ()
{
}

void ChildAlias::userStoppedChangingBounds ()
{
}

bool ChildAlias::boundsChangedSinceStart ()
{
	return startBounds != getBounds ();
}
//===========================================================================
void ChildAlias::mouseDown (const MouseEvent& e)
{
	toFront (true);
	
	if (e.eventComponent == resizer)
	{

	}
	else
	{
		//added a constrainer so that components can't be dragged off-screen
		constrainer->setMinimumOnscreenAmounts(getHeight(), getWidth(), getHeight(), getWidth());
		dragger.startDraggingComponent(this, e);		
	}
	userAdjusting = true;
	startBounds = getBounds ();
	userStartedChangingBounds ();	
	StringArray compNames;
	
	if(getLassoSelection().getNumSelected()>0)
	for ( ChildAlias* child : getLassoSelection() )
	{
		compNames.add(child->getName());
	}
	else
		compNames.add(getName());
		
	getPluginEditor()->setCurrentlySelectedComponents(compNames);
	getPluginEditor()->sendChangeMessage();
	
	if(getLassoSelection().getNumSelected()==0)
		getComponentLayoutEditor()->resetAllInterest();
	
	interest = "selected";
	
}

void ChildAlias::mouseUp (const MouseEvent& e)
{	
	if (e.eventComponent == resizer)
	{
	}
	else
	{
		//add this to reset MainComponent to have keyboard focus so that keyboard shortcuts (eg. lock/unlock) still work / intercept the messages
		getTopLevelComponent()->getChildComponent(0)->grabKeyboardFocus(); 

	}
	if (userAdjusting) userStoppedChangingBounds ();
	userAdjusting = false;
	
	ComponentLayoutEditor::updateSelectedComponentBounds(getLassoSelection());
}

void ChildAlias::mouseDrag (const MouseEvent& e)
{
	
	if (e.eventComponent == resizer)
	{
	}
	else
	{
		bool multipleSelection = false;
		if (!e.mouseWasClicked ())
		{
			for ( ChildAlias* child : getLassoSelection() )
			{
				const int dragX = e.getDistanceFromDragStartX();
				const int dragY = e.getDistanceFromDragStartY();
				const int gridSize = 2;
				int selectedCompsPosX = child->getProperties().getWithDefault("originalX", 1);
				int selectedCompsPosY = child->getProperties().getWithDefault("originalY", 1);
				selectedCompsPosY = selectedCompsPosY+dragY;
				selectedCompsPosY = selectedCompsPosY/gridSize*gridSize;
				selectedCompsPosX = selectedCompsPosX+dragX;
				selectedCompsPosX = selectedCompsPosX/gridSize*gridSize;
				//needs fixing for multiple objects. For now leaving it disabled...
				//restrictBounds(selectedCompsPosX, selectedCompsPosY);
				child->setTopLeftPosition(selectedCompsPosX, selectedCompsPosY);
				child->applyToTarget();				
				multipleSelection = true;
			}

			if(multipleSelection == false)
			{
				dragger.dragComponent(this,e, constrainer);
				applyToTarget ();
			}

		}
	}
	updateBoundsDataForTarget();	
}

void ChildAlias::mouseEnter (const MouseEvent& e)
{
	repaint ();
}

void ChildAlias::mouseExit (const MouseEvent& e)
{
	repaint ();
}

//===========================================================================
ComponentLayoutEditor* ChildAlias::getComponentLayoutEditor()
{
	if(ComponentLayoutEditor* c = dynamic_cast <ComponentLayoutEditor*> (getParentComponent()))
		return c;
	else
		return nullptr;
}

CabbagePluginEditor* ChildAlias::getPluginEditor()
{
	return getComponentLayoutEditor()->getPluginEditor();
}	
//===========================================================================

void ChildAlias::updateBoundsDataForTarget()
{
	bool multipleSelection = false;
	
	for ( ChildAlias* child : getLassoSelection() )
	{
		ValueTree valueTree = CabbageWidgetData::getValueTreeForComponent(getComponentLayoutEditor()->widgetData,child->target.getComponent()->getName());
		CabbageWidgetData::setNumProp(valueTree, CabbageIdentifierIds::left, child->target.getComponent()->getX());
		CabbageWidgetData::setNumProp(valueTree, CabbageIdentifierIds::top, child->target.getComponent()->getY());
		CabbageWidgetData::setNumProp(valueTree, CabbageIdentifierIds::width, child->target.getComponent()->getWidth());
		CabbageWidgetData::setNumProp(valueTree, CabbageIdentifierIds::height, child->target.getComponent()->getHeight());	
	}
	
	if(multipleSelection==false)
	{
		ValueTree valueTree = CabbageWidgetData::getValueTreeForComponent(getComponentLayoutEditor()->widgetData,target.getComponent()->getName());
		CabbageWidgetData::setNumProp(valueTree, CabbageIdentifierIds::left, target.getComponent()->getX());
		CabbageWidgetData::setNumProp(valueTree, CabbageIdentifierIds::top, target.getComponent()->getY());
		CabbageWidgetData::setNumProp(valueTree, CabbageIdentifierIds::width, target.getComponent()->getWidth());
		CabbageWidgetData::setNumProp(valueTree, CabbageIdentifierIds::height, target.getComponent()->getHeight());
	}
	
	getPluginEditor()->sendChangeMessage();
}







//=============================================================================
ComponentLayoutEditor::ComponentLayoutEditor (ValueTree valueTree)
:   target (0), widgetData(valueTree)
{
	setColour (ComponentLayoutEditor::aliasIdleColour,Colours::lightgrey.withAlpha(0.2f));
	setColour (ComponentLayoutEditor::aliasHoverColour, Colours::yellow.withAlpha(0.5f));
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
		if(ChildAlias* child = dynamic_cast<ChildAlias*>(frames[i]))
			child->setInterest("none");
    }
    repaint();
}
//==================================================================================================================
void ComponentLayoutEditor::updateSelectedComponentBounds(SelectedItemSet <ChildAlias*> selectedComponents)
{
    for(ChildAlias* child : selectedComponents)
    {	
		child->getProperties().set("originalX", child->getBounds().getX());
		child->getProperties().set("originalY", child->getBounds().getY());
		child->getProperties().set("originalWidth", child->getBounds().getWidth());
		child->getProperties().set("originalHeight", child->getBounds().getHeight());
	}	
	
}
void ComponentLayoutEditor::mouseUp(const MouseEvent& e)
{

    updateSelectedComponentBounds(selectedComponents);
    lassoComp.endLasso();
    removeChildComponent (&lassoComp);
}

void ComponentLayoutEditor::mouseDrag (const MouseEvent& e)
{
    selectedComponents.deselectAll();
    lassoComp.toFront (false);
    lassoComp.dragLasso (e);
    //currentEvent = "mouseDragLayoutEditor";
    //sendChangeMessage();
}

void ComponentLayoutEditor::mouseDown (const MouseEvent& e)
{
    selectedComponents.deselectAll();
	
    resetAllInterest();
	
    //selectedLineNumbers.clear();

    if(e.mods.isPopupMenu())
    {
        currentMouseCoors = e.getPosition();
        //currentEvent = "triggerPopupMenu";
        //sendChangeMessage();
    }
    else
    {
        addChildComponent (&lassoComp);
        lassoComp.beginLasso (e, this);
    }

}

//==================================================================================================================
void ComponentLayoutEditor::findLassoItemsInArea (Array <ChildAlias*>& results, const Rectangle<int>& area)
{
    const Rectangle<int> lasso (area);

    for (int i = 0; i < getNumChildComponents()-1; i++)
    {
        ChildAlias* c = (ChildAlias*)getChildComponent(i);
        if (c->getBounds().intersects (lasso))
        {
            results.addIfNotAlreadyThere(c);
            selectedComponents.addToSelection(c);
            //Logger::writeToLog(c->getName());
        }
        else
            selectedComponents.deselect(c);
    }
}

SelectedItemSet <ChildAlias*>& ComponentLayoutEditor::getLassoSelection()
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
		//if (target && !target->hasBeenDeleted ())
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
		//if (target && !target->hasBeenDeleted ())
	{
		Component* t = (Component*) target.getComponent ();
		int n = t->getNumChildComponents ();
		for (int i=0; i<n; i++)
		{
			Component* c = t->getChildComponent (i);
			if (c)
			{
                ChildAlias* alias = createAlias (c);
                if (alias)
                {
					alias->setName(c->getName());
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

ChildAlias* ComponentLayoutEditor::createAlias (Component* child)
{
	return new ChildAlias (child);
}
