/*
  ==============================================================================

    CabbageCustomWidgets.cpp
    Created: 28 Oct 2016 8:23:33am
    Author:  rory

  ==============================================================================
*/

#include "CabbageCustomWidgets.h"
#include "../CabbageCommonHeaders.h"
#include "../Audio/CabbagePluginEditor.h"


void CabbageWidgetBase::handleCommonUpdates(Component* child, ValueTree data)
{
	if(getPluginEditor(child)->inGUIEditMode() == false)
		child->setBounds(CabbageWidget::getBounds(data));
		
	else if(CabbageWidget::getNumProp(data, CabbageIdentifierIds::allowboundsupdate)==1)
	{
		child->setBounds(CabbageWidget::getBounds(data));
		getPluginEditor(child)->updateLayoutEditorFrames();
	}
	
	if(rotate!=CabbageWidget::getNumProp(data, CabbageIdentifierIds::rotate))
	{
		rotate = CabbageWidget::getNumProp(data, CabbageIdentifierIds::rotate);
		child->setTransform(AffineTransform::rotation(rotate, child->getX()+CabbageWidget::getNumProp(data, 
													  CabbageIdentifierIds::pivotx), 
													  child->getY()+CabbageWidget::getNumProp(data, CabbageIdentifierIds::pivoty)));
	}
	
	if(visible != CabbageWidget::getNumProp(data, CabbageIdentifierIds::visible))
	{
		visible = CabbageWidget::getNumProp(data, CabbageIdentifierIds::visible);
		child->setVisible(visible==1 ? true : false);
		child->setEnabled(visible==1 ? true : false);
	}

	if(active != CabbageWidget::getNumProp(data, CabbageIdentifierIds::active))
	{
		active = CabbageWidget::getNumProp(data, CabbageIdentifierIds::active);
		child->setEnabled(visible==1 ? true : false);
	}
	
}


//add any new custom widgets here to avoid having to eidt makefiles and projects