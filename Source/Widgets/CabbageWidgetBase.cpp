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

//==============================================================================
__attribute__((unused)) static CabbagePluginEditor* getPluginEditor(Component* child)
{
    if (CabbagePluginEditor* c = child->findParentComponentOfClass<CabbagePluginEditor>())
        return c;
    else
        return nullptr;
}


void CabbageWidgetBase::initialiseCommonAttributes (Component* child, ValueTree data)
{
    toFront = -99;
    csdFile = CabbageWidgetData::getStringProp (data, CabbageIdentifierIds::csdfile);
    rotate = CabbageWidgetData::getNumProp (data, CabbageIdentifierIds::rotate);
    pivotx = CabbageWidgetData::getNumProp (data, CabbageIdentifierIds::pivotx);
    pivoty = CabbageWidgetData::getNumProp (data, CabbageIdentifierIds::pivoty);
    visible = CabbageWidgetData::getNumProp (data, CabbageIdentifierIds::visible);
    active = CabbageWidgetData::getNumProp (data, CabbageIdentifierIds::active);
    channel = CabbageWidgetData::getStringProp (data, CabbageIdentifierIds::channel);
    file = CabbageWidgetData::getStringProp (data, CabbageIdentifierIds::file);
    tooltipText = CabbageWidgetData::getStringProp (data, CabbageIdentifierIds::popuptext);
    child->setBounds (CabbageWidgetData::getBounds (data));
    child->setName (CabbageWidgetData::getStringProp (data, CabbageIdentifierIds::name));
    text = CabbageWidgetData::getStringProp (data, CabbageIdentifierIds::text);
    currentValue = CabbageWidgetData::getNumProp (data, CabbageIdentifierIds::value);
    value = currentValue;
    valuex = CabbageWidgetData::getNumProp (data, CabbageIdentifierIds::valuex);
    valuey = CabbageWidgetData::getNumProp (data, CabbageIdentifierIds::valuey);
    lineNumber = CabbageWidgetData::getNumProp (data, CabbageIdentifierIds::linenumber);
    child->getProperties().set (CabbageIdentifierIds::linenumber, lineNumber);
    populateTextArrays (data);
    //now initialise everything that can be updated using ident channels
    handleCommonUpdates (child, data, true, Identifier());
}

void CabbageWidgetBase::handleCommonUpdates (Component* child, ValueTree data, bool calledFromConstructor, const Identifier& prop)
{
    if (calledFromConstructor == false)
    {
        if (getPluginEditor (child) != nullptr && getPluginEditor (child)->isEditModeEnabled() == false)
        {
            if(prop == CabbageIdentifierIds::bounds){
                var bounds = CabbageWidgetData::getProperty(data, CabbageIdentifierIds::bounds);

                child->setBounds (bounds[0], bounds[1], bounds[2], bounds[3]);
            }
            else if(prop == CabbageIdentifierIds::pos || prop == CabbageIdentifierIds::position)
            {
                var pos = CabbageWidgetData::getProperty(data, CabbageIdentifierIds::position);
                child->setTopLeftPosition(pos[0], pos[1]);
            }
            else if(prop == CabbageIdentifierIds::size)
            {
                var size = CabbageWidgetData::getProperty(data, CabbageIdentifierIds::position);
                child->setTopLeftPosition(size[0], size[1]);
            }
            else
                child->setBounds (CabbageWidgetData::getBounds (data));
        }
        else if (CabbageWidgetData::getNumProp (data, CabbageIdentifierIds::allowboundsupdate) == 1)
        {
            child->setBounds (CabbageWidgetData::getBounds (data));
            getPluginEditor (child)->updateLayoutEditorFrames();
        }
    }

    if ( rotate != CabbageWidgetData::getNumProp (data, CabbageIdentifierIds::rotate) || calledFromConstructor)
    {
        rotate = CabbageWidgetData::getNumProp (data, CabbageIdentifierIds::rotate);
        child->setTransform (AffineTransform::rotation ( rotate, child->getX() + CabbageWidgetData::getNumProp (data,
                                                         CabbageIdentifierIds::pivotx),
                                                         child->getY() + CabbageWidgetData::getNumProp (data, CabbageIdentifierIds::pivoty)));
    }

    if ( toFront != CabbageWidgetData::getNumProp (data, CabbageIdentifierIds::tofront) || calledFromConstructor)
    {
        toFront = CabbageWidgetData::getNumProp (data, CabbageIdentifierIds::tofront);
        CabbageWidgetData::setNumProp (data, CabbageIdentifierIds::tofront, 0);
        child->toFront(true);
    }

    if ( visible != CabbageWidgetData::getNumProp (data, CabbageIdentifierIds::visible) || calledFromConstructor)
    {
        visible = CabbageWidgetData::getNumProp (data, CabbageIdentifierIds::visible);
        child->setVisible ( visible == 1 ? true : false);
        //child->setEnabled ( visible == 1 ? true : false);
    }

    if (text != CabbageWidgetData::getStringProp (data, CabbageIdentifierIds::text) || calledFromConstructor)
    {
        text = CabbageWidgetData::getStringProp (data, CabbageIdentifierIds::text);
    }

    if ( active != CabbageWidgetData::getNumProp (data, CabbageIdentifierIds::active) || calledFromConstructor)
    {
        //string sequencer uses active to stop sequencing..
        active = CabbageWidgetData::getNumProp (data, CabbageIdentifierIds::active);
        if (CabbageWidgetData::getStringProp(data, CabbageIdentifierIds::type) != "eventsequencer")
        child->setEnabled ( active == 1 ? true : false);
    }

    if ( alpha != CabbageWidgetData::getNumProp (data, CabbageIdentifierIds::alpha) || calledFromConstructor)
    {
        alpha = CabbageWidgetData::getNumProp (data, CabbageIdentifierIds::alpha);
        child->setAlpha ( alpha);
    }

    if ( file != CabbageWidgetData::getStringProp (data, CabbageIdentifierIds::file) || calledFromConstructor)
    {
        file = CabbageWidgetData::getStringProp (data, CabbageIdentifierIds::file);
    }

    if( behind != CabbageWidgetData::getStringProp (data, CabbageIdentifierIds::sendbehind))
    {
        behind = CabbageWidgetData::getStringProp (data, CabbageIdentifierIds::sendbehind);
        if(child->getParentComponent() != nullptr)
            DBG(child->getParentComponent()->getName());
//        if(getPluginEditor (child) != nullptr)
//            getPluginEditor(child)->moveBehind(CabbageWidgetData::getStringProp (data, CabbageIdentifierIds::channel), "");
    }
    
    populateTextArrays (data);
}

String CabbageWidgetBase::getCurrentText (ValueTree data)
{
    if ( text != CabbageWidgetData::getStringProp (data, CabbageIdentifierIds::text))
    {
        text = CabbageWidgetData::getStringProp (data, CabbageIdentifierIds::text);
        return  text;
    }

    return  text;
}

String CabbageWidgetBase::getCurrentPopupText (ValueTree data)
{
    if ( tooltipText != CabbageWidgetData::getStringProp (data, CabbageIdentifierIds::popuptext))
    {
        tooltipText = CabbageWidgetData::getStringProp (data, CabbageIdentifierIds::popuptext);
        return  tooltipText;
    }

    return  tooltipText;
}

void CabbageWidgetBase::setChannel (ValueTree data)
{
    if ( channel != CabbageWidgetData::getStringProp (data, CabbageIdentifierIds::channel))
    {
        channel = CabbageWidgetData::getStringProp (data, CabbageIdentifierIds::channel);
        CabbageWidgetData::setProperty (data, CabbageIdentifierIds::channel,  channel);
    }
}

float CabbageWidgetBase::getCurrentValue (ValueTree data)
{
    if ( currentValue != CabbageWidgetData::getNumProp (data, CabbageIdentifierIds::value))
    {
        currentValue = CabbageWidgetData::getNumProp (data, CabbageIdentifierIds::value);
        return  currentValue;
    }

    return  currentValue;
}

void CabbageWidgetBase::populateTextArrays (ValueTree data)
{
    channelArray.clear();
    textArray.clear();
    const Array<var>* channelArrayVar = CabbageWidgetData::getProperty (data, CabbageIdentifierIds::channel).getArray();

    if (channelArrayVar && channelArrayVar->size() > 1)
    {
        for (int i = 0 ; i < channelArrayVar->size() ; i++ )
            channelArray.add (channelArrayVar->getReference (i).toString());
    }
    else
        channelArray.add (CabbageWidgetData::getStringProp (data, CabbageIdentifierIds::channel));  //make sure we have at least two items in array

    channelArray.add (CabbageWidgetData::getStringProp (data, CabbageIdentifierIds::channel));

    const Array<var>* textArrayVar = CabbageWidgetData::getProperty (data, CabbageIdentifierIds::text).getArray();

    if (textArrayVar && textArrayVar->size() > 1)
    {
        for (int i = 0 ; i < textArrayVar->size() ; i++ )
        {
            textArray.add (textArrayVar->getReference (i).toString());
        }
    }
    else
        textArray.add (CabbageWidgetData::getStringProp (data, CabbageIdentifierIds::text));  //make sure we have at least two items in array

    textArray.add (CabbageWidgetData::getStringProp (data, CabbageIdentifierIds::text));
}

int CabbageWidgetBase::getSVGWidth (File svgFile)
{
    std::unique_ptr<XmlElement> svg (XmlDocument::parse (svgFile.loadFileAsString()));

    for (int i = 0; i < svg->getNumAttributes(); i++)
    {
        if (svg->getAttributeName (i) == "width")
            return svg->getAttributeValue (i).getIntValue();
    }

    return 0;
}

int CabbageWidgetBase::getSVGHeight (File svgFile)
{
    std::unique_ptr<XmlElement> svg (XmlDocument::parse (svgFile.loadFileAsString()));

    for (int i = 0; i < svg->getNumAttributes(); i++)
    {
        if (svg->getAttributeName (i) == "height")
            return svg->getAttributeValue (i).getIntValue();
    }

    return 0;
}

String CabbageWidgetBase::createPopupBubbleText(double val, int decimalPlaces,
                                                const String& widgetChannel,
                                                const String& escapedPrefix,
                                                const String& escapedPostfix)
{
    String popupText;
    tooltipText = getTooltipText();
    
    if (tooltipText.isNotEmpty())
        popupText = tooltipText;
    else if ( escapedPostfix.isNotEmpty() || escapedPrefix.isNotEmpty() )
        popupText = createValueText(val, decimalPlaces, escapedPrefix, escapedPostfix);
    else
        popupText = widgetChannel + ": " + createValueText(val, decimalPlaces);
    
    return popupText;
}
