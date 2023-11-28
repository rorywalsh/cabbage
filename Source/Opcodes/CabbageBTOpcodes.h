#pragma once


#include "../CabbageCommonHeaders.h"
#include "../Widgets/CabbageWidgetData.h"
#undef _CR
#include <plugin.h>

#include "JuceHeader.h"
#include "utils.hpp"

#include "simpleble/SimpleBLE.h"
#include <thread>

class CabbagePresetData
{
public:
    
    CabbagePresetData(){}
    
    std::string data = "";
    int size = 0;
};

struct CabbageBTOpcode : csnd::Plugin<1, 2>
{
    enum ProgressState
    {
        Idle = 0,
        Scanning = 1,
        Connecting = 2,
        Connected = 3,
        FindingCharacteristics = 4,
        Transfering = 5,
        Disconnected = 6
    };
    
    int deinit(){  btThread.join(); return OK; }
    int init();
    int kperf();
    CabbagePresetData** globalPresetData = nullptr;
    CabbagePresetData* presetData = nullptr;
    std::thread btThread;
    int progressState;
    bool running;
    static void scanConnectAndTransfer(int& state, std::string data, String deviceName, String characteristic);

};
