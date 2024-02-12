//
//  CabbageMIDIOpcode.cpp
//  Cabbage
//
//  Created by Rory on 09/02/2023.
//

#include <sstream>
#include "CabbageProfilerOpcodes.h"


int CabbageProfilerStart::init()
{
    std::string identifier(args.str_data(0).data);
    std::string block(args.str_data(1).data);
    
    profiler = (Profiler**)csound->query_global_variable(identifier.c_str());
    Profiler* profilerData;

    if (profiler != nullptr)
    {
        profilerData = *profiler;
        profilerData->timer[block].reset(new ProfilerTimer());
        profilerData->timer[block]->start();
    }
    else
    {
        csound->create_global_variable(identifier.c_str(), sizeof(Profiler*));
        profiler = (Profiler**)csound->query_global_variable(identifier.c_str());
        *profiler = new Profiler();
        profilerData = *profiler;
        profilerData->timer[block].reset(new ProfilerTimer());
        profilerData->timer[block]->start();
    }

    return OK;
}



int CabbageProfilerStart::kperf()
{
    std::string identifier(args.str_data(0).data);
    std::string block(args.str_data(1).data);
    
    profiler = (Profiler**)csound->query_global_variable(identifier.c_str());
    Profiler* profilerData;

    if (profiler != nullptr)
    {
        profilerData = *profiler;
    }
    else
        return NOTOK;
    
    profilerData->timer[block]->start();
    return OK;
}
    

int CabbageProfilerStop::kperf()
{
    std::string identifier(inargs.str_data(0).data);
    std::string block(inargs.str_data(1).data);
    
    profiler = (Profiler**)csound->query_global_variable(identifier.c_str());
    Profiler* profilerData;

    if (profiler != nullptr)
    {
        profilerData = *profiler;
    }
    else
        return NOTOK;
    
    if(profilerData->timer[block])
    {
        profilerData->timer[block]->stop();
        outargs[0] = profilerData->timer[block]->getAverage();
    }
    
    return OK;
}

int CabbageProfilerPrint::kperf()
{
    std::string identifier(args.str_data(0).data);
    int trig = args[1];
    
    profiler = (Profiler**)csound->query_global_variable(identifier.c_str());
    Profiler* profilerData;

    if (profiler != nullptr)
    {
        profilerData = *profiler;
    }
    else
        return NOTOK;
    

    
    if(trig == 1)
    {
        std::map<std::string, std::unique_ptr<ProfilerTimer>>::iterator it;
        std::stringstream output = {};
        output << identifier << " | ";
        for (auto const& t : profilerData->timer)
        {
            if(t.second.get())
                output << t.first << ":" << String(t.second.get()->getAverage(), 4).paddedLeft(' ', 10).toStdString() << "\t\t";
        }

        csound->message(output.str());
    }

    
    return OK;
}
