#pragma once


#include "../CabbageCommonHeaders.h"
#include "../Widgets/CabbageWidgetData.h"
#undef _CR
#include <plugin.h>

#include "JuceHeader.h"
#include "utils.hpp"

#include "simpleble/SimpleBLE.h"

struct CabbageBTOpcode : csnd::Plugin<1, 1>
{
    int init();
    int kperf();
    
    std::optional<SimpleBLE::Adapter> adapter_optional;
    std::vector<SimpleBLE::Peripheral> peripherals;
    bool deviceFound = false;
    int deviceIndex = 0;
};
