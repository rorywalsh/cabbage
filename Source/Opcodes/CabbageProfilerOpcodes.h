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
    ProfilerTimer(const char* name)
        : m_Name(name), m_Stopped(false)
    {
        
    }

    ~ProfilerTimer()
    {

    }

    void Start()
    {
        m_StartTimepoint = std::chrono::high_resolution_clock::now();
    }
    
    void Stop()
    {
        auto endTimepoint = std::chrono::high_resolution_clock::now();

        long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
        long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();
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
    const char* m_Name;
    float average = 0.f;
    float count = 1.f;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
    bool m_Stopped;
};
//====================================================================================================

class Profiler
{
public:
    ProfilerTimer* timer;
};

struct CabbageProfilerStart : csnd::InPlug<1>
{
    int init();
    int kperf();
    Profiler** profiler = nullptr;
};

struct CabbageProfilerStop : csnd::Plugin<1, 1>
{
    int init();
    int kperf();
    Profiler** profiler = nullptr;
};
