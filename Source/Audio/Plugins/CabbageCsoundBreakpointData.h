/*
  ==============================================================================

    CabbageCsoundBreakpointData.h
    Created: 22 Nov 2016 9:49:12pm
    Author:  rory

  ==============================================================================
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
