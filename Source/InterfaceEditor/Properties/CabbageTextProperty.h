/*
  ==============================================================================

    CabbageTextProperty.h
    Created: 27 Oct 2016 10:19:47am
    Author:  rory

  ==============================================================================
*/

#ifndef CABBAGETEXTPROPERTY_H_INCLUDED
#define CABBAGETEXTPROPERTY_H_INCLUDED


#include "../../CabbageCommonHeaders.h"

class CabbageTextProperty  : public TextPropertyComponent,
                               private ChangeListener
{
public:
    CabbageTextProperty (const String& name,
                           const int maxNumChars_,
                           const bool isMultiLine_)
        : TextPropertyComponent (name, maxNumChars_, isMultiLine_)
    {
        
    }

    ~CabbageTextProperty()
    {

    }

    void changeListenerCallback (ChangeBroadcaster*)
    {
        refresh();
    }


};




#endif  // CABBAGETEXTPROPERTY_H_INCLUDED
