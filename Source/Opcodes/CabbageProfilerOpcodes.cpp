//
//  CabbageMIDIOpcode.cpp
//  Cabbage
//
//  Created by Rory on 09/02/2023.
//

#include "CabbageProfilerOpcodes.h"


int CabbageProfilerStart::init()
{
 
    return OK;
}



int CabbageProfilerStart::kperf()
{
    std::string identifier(args.str_data(0).data);
    
    profiler = (Profiler**)csound->query_global_variable(identifier.c_str());
    Profiler* profilerData;

    if (profiler != nullptr)
    {
        profilerData = *profiler;
    }
    else
    {
        csound->create_global_variable(identifier.c_str(), sizeof(Profiler*));
        profiler = (Profiler**)csound->query_global_variable(identifier.c_str());
        *profiler = new Profiler();
        profilerData = *profiler;
        profilerData->timer = new ProfilerTimer(identifier.c_str());
    }
    
    profilerData->timer->Start();
    return OK;
}
    
int CabbageProfilerStop::init()
{
 
 
    return OK;
}



int CabbageProfilerStop::kperf()
{
    std::string identifier(inargs.str_data(0).data);
    
    profiler = (Profiler**)csound->query_global_variable(identifier.c_str());
    Profiler* profilerData;

    if (profiler != nullptr)
    {
        profilerData = *profiler;
    }
    else
    {
        csound->create_global_variable(identifier.c_str(), sizeof(Profiler*));
        profiler = (Profiler**)csound->query_global_variable(identifier.c_str());
        *profiler = new Profiler();
        profilerData = *profiler;
        profilerData->timer = new ProfilerTimer(identifier.c_str());
    }
    
    profilerData->timer->Stop();
    outargs[0] = profilerData->timer->getAverage();
    
    return OK;
}
