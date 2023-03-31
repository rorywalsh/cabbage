#pragma once


#include "../CabbageCommonHeaders.h"
#include "../Widgets/CabbageWidgetData.h"
#undef _CR
#include <plugin.h>

#include "JuceHeader.h"


struct CabbageSocketSender : csnd::Plugin<1, 1>
{
    int init();
    int kperf();
};