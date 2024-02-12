#pragma once


#include "../CabbageCommonHeaders.h"
#include "../Widgets/CabbageWidgetData.h"
#undef _CR
#include <plugin.h>

#include "JuceHeader.h"





#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>


class ProfilerTimer
{
public:
    void start()
    {
        startPoint = std::chrono::high_resolution_clock::now();
    }
    
    void stop()
    {
        auto endPoint = std::chrono::high_resolution_clock::now();

        long long start = std::chrono::time_point_cast<std::chrono::microseconds>(startPoint).time_since_epoch().count();
        long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endPoint).time_since_epoch().count();
        auto duration = end-start;
        average = average+duration;
        count++;
    }
    
    float getAverage()
    {
        auto a = average/count;
        return a;
    }
    
private:
    float average = 0.f;
    float count = 1.f;
    std::chrono::time_point<std::chrono::high_resolution_clock> startPoint;
};
//====================================================================================================

class Profiler
{
public:
    std::map<std::string, std::unique_ptr<ProfilerTimer>> timer;
};

struct CabbageProfilerStart : csnd::InPlug<2>
{
    int init();
    int kperf();
    Profiler** profiler = nullptr;
};

struct CabbageProfilerStop : csnd::Plugin<1, 2>
{
    int kperf();
    Profiler** profiler = nullptr;
};

struct CabbageProfilerPrint : csnd::InPlug<2>
{
    int kperf();
    Profiler** profiler = nullptr;
};
