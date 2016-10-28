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


void CabbageWidgetUtilities::handleBoundsUpdate(Component* child, ValueTree data)
{
	if(getPluginEditor(child)->inGUIEditMode() == false)
		child->setBounds(CabbageWidget::getBounds(data));
		
	else if(CabbageWidget::getNumProp(data, CabbageIdentifierIds::allowboundsupdate)==1)
	{
		child->setBounds(CabbageWidget::getBounds(data));
		getPluginEditor(child)->updateLayoutEditorFrames();
	}
}

//add any new custom widgets here to avoid having to eidt makefiles and projects