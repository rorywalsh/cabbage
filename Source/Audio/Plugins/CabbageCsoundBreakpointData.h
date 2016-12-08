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

#ifndef CABBAGECSOUNDBREAKPOINTDATA_H_INCLUDED
#define CABBAGECSOUNDBREAKPOINTDATA_H_INCLUDED


#include "../../Utilities/CabbageUtilities.h"
#include "../../CabbageIds.h"

class CabbageCsoundBreakpointData : public CabbageUtilities
{

public:
    CabbageCsoundBreakpointData(): valueTree("BreakPointData")
	{};
    ~CabbageCsoundBreakpointData() {};

	void set(String child, Identifier identifier, var value)
	{
		if(valueTree.getChildWithName(child).isValid())
			valueTree.getChildWithName(child).setProperty(identifier,  value, 0);
		else
		{
			valueTree.addChild(ValueTree(child), -1, 0);
			valueTree.getChildWithName(child).setProperty(identifier,  value, 0);
		}
	}
	
	ValueTree valueTree;
};
#endif  // CABBAGECSOUNDBREAKPOINTDATA_H_INCLUDED
