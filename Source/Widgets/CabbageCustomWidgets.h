/*
  ==============================================================================

    CabbageCustomWidgets.h
    Created: 28 Oct 2016 8:23:33am
    Author:  rory

  ==============================================================================
*/

#ifndef CABBAGECUSTOMWIDGETS_H_INCLUDED
#define CABBAGECUSTOMWIDGETS_H_INCLUDED

#include "../CabbageCommonHeaders.h"

//simple class for taking care of some widget housekeeping
class CabbageWidgetUtilities
{
public:
	CabbageWidgetUtilities(){}
	~CabbageWidgetUtilities(){}
	
	static void handleBoundsUpdate(Component* child, ValueTree data);
};

//add any new custom widgets here to avoid having to eidt makefiles and projects



#endif  // CABBAGECUSTOMWIDGETS_H_INCLUDED
