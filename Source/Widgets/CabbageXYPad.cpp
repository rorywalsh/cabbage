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

#include "CabbageXYPad.h"
#include "../Audio/Plugins/CabbagePluginEditor.h"

CabbageXYPad::CabbageXYPad(ValueTree wData, XYPadAutomation* xyAuto, CabbagePluginEditor* editor, int index):
widgetData(wData),
owner(editor),
XYPad(xyAuto, CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::text),
				CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::minx), 
				CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::maxx), 
				CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::miny), 
				CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::maxy), 
				CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::decimalplaces), 
				Colour::fromString(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::colour)), 
				Colour::fromString(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::fontcolour)), 
				Colour::fromString(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::textcolour)),
				CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::valuex), 
				CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::valuey))
{
	setName(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::name));
	widgetData.addListener(this); 				//add listener to valueTree so it gets notified when a widget's property changes
	initialiseCommonAttributes(this, wData); 	//initialise common attributes such as bounds, name, rotation, etc..	
	
}
	
void CabbageXYPad::valueTreePropertyChanged (ValueTree& valueTree, const Identifier& prop)
{
	
	setXYValues(xyPadAutomation->getXValue(), xyPadAutomation->getYValue());
	
	handleCommonUpdates(this, valueTree);		//handle comon updates such as bounds, alpha, rotation, visible, etc	

}