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
		child->setBounds(CabbageWidgetData::getBounds(data));
		
	else if(CabbageWidgetData::getNumProp(data, CabbageIdentifierIds::allowboundsupdate)==1)
	{
		child->setBounds(CabbageWidgetData::getBounds(data));
		getPluginEditor(child)->updateLayoutEditorFrames();
	}
	
	if(rotate!=CabbageWidgetData::getNumProp(data, CabbageIdentifierIds::rotate))
	{
		rotate = CabbageWidgetData::getNumProp(data, CabbageIdentifierIds::rotate);
		child->setTransform(AffineTransform::rotation(rotate, child->getX()+CabbageWidgetData::getNumProp(data, 
													  CabbageIdentifierIds::pivotx), 
													  child->getY()+CabbageWidgetData::getNumProp(data, CabbageIdentifierIds::pivoty)));
	}
	
	if(visible != CabbageWidgetData::getNumProp(data, CabbageIdentifierIds::visible))
	{
		visible = CabbageWidgetData::getNumProp(data, CabbageIdentifierIds::visible);
		child->setVisible(visible==1 ? true : false);
		child->setEnabled(visible==1 ? true : false);
	}

	if(active != CabbageWidgetData::getNumProp(data, CabbageIdentifierIds::active))
	{
		active = CabbageWidgetData::getNumProp(data, CabbageIdentifierIds::active);
		child->setEnabled(active==1 ? true : false);
	}
	
	if(alpha != CabbageWidgetData::getNumProp(data, CabbageIdentifierIds::alpha))
	{
		alpha = CabbageWidgetData::getNumProp(data, CabbageIdentifierIds::alpha);
		child->setAlpha(alpha);
	}	
	
	if(alpha != CabbageWidgetData::getNumProp(data, CabbageIdentifierIds::alpha))
	{
		alpha = CabbageWidgetData::getNumProp(data, CabbageIdentifierIds::alpha);
	}
}


String CabbageWidgetBase::getString(ValueTree data, String identifier)
{
	if(text!=CabbageWidgetData::getStringProp(data, identifier))
	{
		text = CabbageWidgetData::getStringProp(data, identifier);
		return text;
	}

	return text;
}

void CabbageWidgetBase::setChannel(ValueTree data)
{
	if(channel != CabbageWidgetData::getStringProp(data, CabbageIdentifierIds::channel))
	{
		channel = CabbageWidgetData::getStringProp(data, CabbageIdentifierIds::channel);
		CabbageWidgetData::setProperty(data, CabbageIdentifierIds::channel, channel);
	}	
}

float CabbageWidgetBase::getValue(ValueTree data)
{
	if(currentValue!=CabbageWidgetData::getNumProp(data, CabbageIdentifierIds::value))
	{
		currentValue = CabbageWidgetData::getNumProp(data, CabbageIdentifierIds::value);
		return currentValue;
	}

	return currentValue;
}

//add any new custom widgets here to avoid having to eidt makefiles and projects