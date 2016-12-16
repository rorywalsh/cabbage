/*
  Copyright (C) 2016 Rory Walsh

  Cabbage is free software; you can redistribute it
  and/or modify it under the terms of the GNU General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  Cabbage is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU General Public
  License along with Csound; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
  02111-1307 USA
*/

#include "CabbageWidgetBase.h"
#include "../CabbageCommonHeaders.h"
#include "../Audio/Plugins/CabbagePluginEditor.h"


void CabbageWidgetBase::initialiseCommonAttributes(Component* child, ValueTree data)
{
    _rotate = CabbageWidgetData::getNumProp(data, CabbageIdentifierIds::rotate);
    _pivotx = CabbageWidgetData::getNumProp(data, CabbageIdentifierIds::pivotx);
    _pivoty = CabbageWidgetData::getNumProp(data, CabbageIdentifierIds::pivoty);
    _visible = CabbageWidgetData::getNumProp(data, CabbageIdentifierIds::visible);
    _active = CabbageWidgetData::getNumProp(data, CabbageIdentifierIds::active);
	_channel = CabbageWidgetData::getStringProp(data, CabbageIdentifierIds::channel);
    _tooltipText = CabbageWidgetData::getStringProp(data, CabbageIdentifierIds::popuptext);
    child->setBounds(CabbageWidgetData::getBounds(data));
    child->setName(CabbageWidgetData::getStringProp(data, CabbageIdentifierIds::name));
	_text = CabbageWidgetData::getStringProp(data, CabbageIdentifierIds::text);
	_currentValue = CabbageWidgetData::getNumProp(data, CabbageIdentifierIds::value);
	
	populateTextArrays(data);
	//now initialise everything that can be updated using ident channels
	handleCommonUpdates(child, data, true);
}

void CabbageWidgetBase::handleCommonUpdates(Component* child, ValueTree data, bool calledFromConstructor)
{
	if(calledFromConstructor == false)
	{
		if(getPluginEditor(child)->isEditModeEnabled() == false)
			child->setBounds(CabbageWidgetData::getBounds(data));

		else if(CabbageWidgetData::getNumProp(data, CabbageIdentifierIds::allowboundsupdate)==1)
		{
			child->setBounds(CabbageWidgetData::getBounds(data));
			getPluginEditor(child)->updateLayoutEditorFrames();
		}
	}
    if(_rotate!=CabbageWidgetData::getNumProp(data, CabbageIdentifierIds::rotate))
    {
        _rotate = CabbageWidgetData::getNumProp(data, CabbageIdentifierIds::rotate);
        child->setTransform(AffineTransform::rotation(_rotate, child->getX()+CabbageWidgetData::getNumProp(data,
                            CabbageIdentifierIds::pivotx),
                            child->getY()+CabbageWidgetData::getNumProp(data, CabbageIdentifierIds::pivoty)));
    }

    if(_visible != CabbageWidgetData::getNumProp(data, CabbageIdentifierIds::visible))
    {
        _visible = CabbageWidgetData::getNumProp(data, CabbageIdentifierIds::visible);
        child->setVisible(_visible==1 ? true : false);
        child->setEnabled(_visible==1 ? true : false);
    }

    if(_active != CabbageWidgetData::getNumProp(data, CabbageIdentifierIds::active))
    {
        _active = CabbageWidgetData::getNumProp(data, CabbageIdentifierIds::active);
        child->setEnabled(_active==1 ? true : false);
    }

    if(_alpha != CabbageWidgetData::getNumProp(data, CabbageIdentifierIds::alpha))
    {
        _alpha = CabbageWidgetData::getNumProp(data, CabbageIdentifierIds::alpha);
        child->setAlpha(_alpha);
    }

	populateTextArrays(data);

}


String CabbageWidgetBase::getCurrentText(ValueTree data)
{
    if(_text!=CabbageWidgetData::getStringProp(data, CabbageIdentifierIds::text))
    {
        _text = CabbageWidgetData::getStringProp(data, CabbageIdentifierIds::text);
        return _text;
    }

    return _text;
}

String CabbageWidgetBase::getCurrentPopupText(ValueTree data)
{
    if(_tooltipText!=CabbageWidgetData::getStringProp(data, CabbageIdentifierIds::popuptext))
    {
        _tooltipText = CabbageWidgetData::getStringProp(data, CabbageIdentifierIds::popuptext);
        return _tooltipText;
    }

    return _tooltipText;
}

void CabbageWidgetBase::setChannel(ValueTree data)
{
    if(_channel != CabbageWidgetData::getStringProp(data, CabbageIdentifierIds::channel))
    {
        _channel = CabbageWidgetData::getStringProp(data, CabbageIdentifierIds::channel);
        CabbageWidgetData::setProperty(data, CabbageIdentifierIds::channel, _channel);
    }
}

float CabbageWidgetBase::getCurrentValue(ValueTree data)
{
    if(_currentValue!=CabbageWidgetData::getNumProp(data, CabbageIdentifierIds::value))
    {
        _currentValue = CabbageWidgetData::getNumProp(data, CabbageIdentifierIds::value);
        return _currentValue;
    }

    return _currentValue;
}

void CabbageWidgetBase::populateTextArrays(ValueTree data)
{
	const Array<var>* channelArray = CabbageWidgetData::getProperty(data, CabbageIdentifierIds::channel).getArray();
	if(channelArray && channelArray->size()>1)
	{
		for(int i = 0 ; i < channelArray->size() ; i++ )
			_channelArray.add(channelArray->getReference(i).toString());
	}
	else
		_channelArray.add(CabbageWidgetData::getStringProp(data, CabbageIdentifierIds::channel));	//make sure we have at least two items in array
		_channelArray.add(CabbageWidgetData::getStringProp(data, CabbageIdentifierIds::channel));
	
	const Array<var>* textArray = CabbageWidgetData::getProperty(data, CabbageIdentifierIds::text).getArray();
	if(textArray && textArray->size()>1)
	{
		for(int i = 0 ; i < textArray->size() ; i++ )
		{
			_textArray.add(textArray->getReference(i).toString());
		}
	}
	else
		_channelArray.add(CabbageWidgetData::getStringProp(data, CabbageIdentifierIds::channel));	//make sure we have at least two items in array
		_channelArray.add(CabbageWidgetData::getStringProp(data, CabbageIdentifierIds::channel));	
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