/*
  ==============================================================================

    CabbageWidgetBase.cpp
    Created: 11 Nov 2016 6:58:12pm
    Author:  rory

  ==============================================================================
*/

#include "CabbageWidgetBase.h"
#include "../CabbageCommonHeaders.h"
#include "../Audio/Plugins/CabbagePluginEditor.h"


void CabbageWidgetBase::handleCommonUpdates(Component* child, ValueTree data)
{
	if(getPluginEditor(child)->isEditModeEnabled() == false)
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


String CabbageWidgetBase::getCurrentString(ValueTree data, String identifier)
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

float CabbageWidgetBase::getCurrentValue(ValueTree data)
{
	if(currentValue!=CabbageWidgetData::getNumProp(data, CabbageIdentifierIds::value))
	{
		currentValue = CabbageWidgetData::getNumProp(data, CabbageIdentifierIds::value);
		return currentValue;
	}

	return currentValue;
}

void CabbageWidgetBase::initialiseCommonAttributes(Component* child, ValueTree data)
{
	rotate = CabbageWidgetData::getNumProp(data, CabbageIdentifierIds::rotate);
	pivotx = CabbageWidgetData::getNumProp(data, CabbageIdentifierIds::pivotx);
	pivoty = CabbageWidgetData::getNumProp(data, CabbageIdentifierIds::pivoty);
	visible = CabbageWidgetData::getNumProp(data, CabbageIdentifierIds::visible);
	active = CabbageWidgetData::getNumProp(data, CabbageIdentifierIds::active);
	tooltipText = CabbageWidgetData::getStringProp(data, CabbageIdentifierIds::popuptext);	
	child->setBounds(CabbageWidgetData::getBounds(data));
	child->setName(CabbageWidgetData::getStringProp(data, CabbageIdentifierIds::name));
			
}


int CabbageWidgetBase::getSVGWidth(File svgFile)
{
	ScopedPointer<XmlElement> svg (XmlDocument::parse(svgFile.loadFileAsString()));
	for(int i=0; i<svg->getNumAttributes(); i++)
	{
		if(svg->getAttributeName(i)=="width")
			return svg->getAttributeValue(i).getIntValue();
	}
	return 0;
}

int CabbageWidgetBase::getSVGHeight(File svgFile)
{
	ScopedPointer<XmlElement> svg (XmlDocument::parse(svgFile.loadFileAsString()));
	for(int i=0; i<svg->getNumAttributes(); i++)
	{
		if(svg->getAttributeName(i)=="height")
			return svg->getAttributeValue(i).getIntValue();
	}
	return 0;
}