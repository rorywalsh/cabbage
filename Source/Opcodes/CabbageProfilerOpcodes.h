#pragma once


#include "../CabbageCommonHeaders.h"
#include "../Widgets/CabbageWidgetData.h"
#undef _CR
#include <plugin.h>

#include "JuceHeader.h"






//====================================================================================================

//====================================================================================================

struct CabbageProfilerOpcodes : csnd::Plugin<6, 7>
{
    int init();
    int kperf();
  
};
