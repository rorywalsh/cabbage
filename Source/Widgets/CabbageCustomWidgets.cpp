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

#include "CabbageCustomWidgets.h"

DemoCabbageWidget::DemoCabbageWidget(ValueTree wData):
widgetData(wData)
{
	setName(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::name));
	widgetData.addListener(this); 				//add listener to valueTree so it gets notified when a widget's property changes
	initialiseCommonAttributes(this, wData); 	//initialise common attributes such as bounds, name, rotation, etc..	
}

void DemoCabbageWidget::valueTreePropertyChanged (ValueTree& valueTree, const Identifier& prop)
{
	if(prop==CabbageIdentifierIds::value)
    {
		//set value. This is only needed for widgets that can have their value changed directly using a chnset
    }
	else
	{

		
		
		handleCommonUpdates(this, valueTree);		//handle common updates such as bounds, alpha, rotation, visible, etc	
	}
}
//add any new custom widgets here to avoid having to eidt makefiles and projects