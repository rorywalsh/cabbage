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

ChildAlias::ChildAlias (Component* targetChild, ComponentLayoutEditor* owner)
:   target (targetChild), layoutEditor(owner)
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
	{
		
		Component* c = (Component*) target.getComponent ();
		c->setBounds (getBounds ());
		
		if(startBounds.getTopLeft()==c->getBounds().getTopLeft())
		{
			for (int i=0; i<c->getNumChildComponents(); i++)
			{
				Component* comp = c->getChildComponent (i);
				const float x = ((float)c->getWidth()/(float)startBounds.getWidth());
				const float y = ((float)c->getHeight()/(float)startBounds.getHeight());
				const float left = childBounds[i].getX()*x;
				const float top = childBounds[i].getY()*y;
				const float width = childBounds[i].getWidth()*x;
				const float height = childBounds[i].getHeight()*y;
				comp->setBounds(left, top, width, height);
				
			}
		}		
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

bool ChildAlias::boundsChangedSinceStart ()
{
	return startBounds != getBounds ();
}
//===========================================================================
void ChildAlias::mouseDown (const MouseEvent& e)
{	
	layoutEditor->updateSelectedComponentBounds();
	mouseDownSelectStatus = layoutEditor->getLassoSelection().addToSelectionOnMouseDown (this, e.mods);
	if (e.eventComponent != resizer)
	{
		//added a constrainer so that components can't be dragged off-screen
		constrainer->setMinimumOnscreenAmounts(getHeight(), getWidth(), getHeight(), getWidth());
		dragger.startDraggingComponent(this, e);		
	}

    
	setBoundsForChildren();
	
	userAdjusting = true;
	startBounds = getBounds ();
	userStartedChangingBounds ();	
	layoutEditor->updateCodeEditor();
	
	if(layoutEditor->getLassoSelection().getNumSelected()==1)
		layoutEditor->resetAllInterest();
	
	interest = "selected";
	
}

void ChildAlias::mouseUp (const MouseEvent& e)
{	
	
	if (e.eventComponent != resizer)
	{
		//add this to reset MainComponent to have keyboard focus so that keyboard shortcuts (eg. lock/unlock) still work / intercept the messages
		getTopLevelComponent()->getChildComponent(0)->grabKeyboardFocus(); 
	}
	
	if (userAdjusting) userStoppedChangingBounds ();
		userAdjusting = false;

	layoutEditor->getLassoSelection().addToSelectionOnMouseUp(this, e.mods, true, mouseDownSelectStatus);	
	layoutEditor->updateSelectedComponentBounds();
}

void ChildAlias::mouseDrag (const MouseEvent& e)
{
	
	if (e.eventComponent != resizer)
	{
		bool multipleSelection = false;
		if (!e.mods.isPopupMenu())
		{
			for ( ChildAlias* child : layoutEditor->getLassoSelection() )
			{
				const int gridSize = 2;
				const int selectedCompsPosX = ((int(child->getProperties().getWithDefault("originalX", 1)) + e.getDistanceFromDragStartX() ) / gridSize) * gridSize;
				const int selectedCompsPosY = ((int(child->getProperties().getWithDefault("originalY", 1)) + e.getDistanceFromDragStartY() ) / gridSize) * gridSize;
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
	if(layoutEditor->getLassoSelection().getNumSelected()>1)
		resizer->setVisible(false);
	else
		resizer->setVisible(true);
		
	repaint ();
}

void ChildAlias::mouseExit (const MouseEvent& e)
{
	repaint ();
}

//===========================================================================
CabbagePluginEditor* ChildAlias::getPluginEditor()
{
	return layoutEditor->getPluginEditor();
}	

void ChildAlias::setBoundsForChildren()
{
	Component* c = (Component*) target.getComponent ();
    childBounds.clear();

    for(int i=0; i<c->getNumChildComponents(); i++)
    {
        childBounds.add(c->getChildComponent(i)->getBounds());
    }	
	
}
//===========================================================================

void ChildAlias::updateBoundsDataForTarget()
{
	bool multipleSelection = false;
	
	for ( ChildAlias* child : layoutEditor->getLassoSelection() )
	{
		ValueTree valueTree = CabbageWidgetData::getValueTreeForComponent(layoutEditor->widgetData,child->target.getComponent()->getName());
		CabbageWidgetData::setNumProp(valueTree, CabbageIdentifierIds::left, child->target.getComponent()->getX());
		CabbageWidgetData::setNumProp(valueTree, CabbageIdentifierIds::top, child->target.getComponent()->getY());
		CabbageWidgetData::setNumProp(valueTree, CabbageIdentifierIds::width, child->target.getComponent()->getWidth());
		CabbageWidgetData::setNumProp(valueTree, CabbageIdentifierIds::height, child->target.getComponent()->getHeight());	
	}
	
	if(multipleSelection==false)
	{
		Component* c = (Component*) target.getComponent ();
		for(int i=0; i<c->getNumChildComponents(); i++)
		{		
			const Component* child = target.getComponent()->getChildComponent(i);
			CabbageUtilities::debug(child->getName());
			ValueTree valueTree = CabbageWidgetData::getValueTreeForComponent(layoutEditor->widgetData,child->getName());
			CabbageWidgetData::setNumProp(valueTree, CabbageIdentifierIds::left, child->getX());
			CabbageWidgetData::setNumProp(valueTree, CabbageIdentifierIds::top, child->getY());
			CabbageWidgetData::setNumProp(valueTree, CabbageIdentifierIds::width, child->getWidth());
			CabbageWidgetData::setNumProp(valueTree, CabbageIdentifierIds::height, child->getHeight());
		}
	}
	
	getPluginEditor()->sendChangeMessage();
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
		if(ChildAlias* child = dynamic_cast<ChildAlias*>(frames[i]))
			child->setInterest("none");
    }
    repaint();
}
//==================================================================================================================
void ComponentLayoutEditor::updateSelectedComponentBounds()
{
    for(ChildAlias* child : selectedComponents)
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
		for ( ChildAlias* child : getLassoSelection() )
		{
			compNames.add(child->getName());
			for( int i = 0 ; i < child->getTarget()->getNumChildComponents() ; i++)
			{
				CabbageUtilities::debug(child->getTarget()->getChildComponent(i)->getName());
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

ChildAlias* ComponentLayoutEditor::createAlias (Component* child)
{
	return new ChildAlias (child, this);
}
