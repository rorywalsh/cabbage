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

//simple class for taking care of some widget housekeeping
class CabbageWidgetBase
{
public:
	CabbageWidgetBase(){}
	~CabbageWidgetBase(){}

	int pivotx, pivoty, visible, active;
    float rotate;
	
	void initialiseCommonAttributes(ValueTree valueTree)
	{
		rotate = CabbageWidget::getNumProp(valueTree, CabbageIdentifierIds::rotate);
		pivotx = CabbageWidget::getNumProp(valueTree, CabbageIdentifierIds::pivotx);
		pivoty = CabbageWidget::getNumProp(valueTree, CabbageIdentifierIds::pivoty);
		visible = CabbageWidget::getNumProp(valueTree, CabbageIdentifierIds::visible);
		active = CabbageWidget::getNumProp(valueTree, CabbageIdentifierIds::active);		
	}
	
	void handleCommonUpdates(Component* child, ValueTree data);

};

//add any new custom widgets here to avoid having to eidt makefiles and projects



#endif  // CABBAGECUSTOMWIDGETS_H_INCLUDED
