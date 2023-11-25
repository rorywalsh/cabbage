//
//  CabbageMIDIOpcode.cpp
//  Cabbage
//
//  Created by Rory on 09/02/2023.
//

#include "CabbageBTOpcodes.h"


int CabbageBTOpcode::init()
{
    adapter_optional = Utils::getAdapter();

    if (!adapter_optional.has_value()) {
        jassertfalse;
        csound->message("Porlbme with Bluetooth...");
    }

    deviceFound = false;
    deviceIndex = 0;
    
}



int CabbageBTOpcode::kperf()
{
    auto adapter = adapter_optional.value();
    auto deviceName = inargs.str_data(0).data;
    if(!deviceFound)
    {
        adapter.set_callback_on_scan_found([&](SimpleBLE::Peripheral peripheral) { peripherals.push_back(peripheral); });

        adapter.set_callback_on_scan_start([this]() { csound->message("Scan started."); });
        adapter.set_callback_on_scan_stop([this]() { csound->message("Scan stopped."); });
        // Scan for 5 seconds and return.
        adapter.scan_for(500);
        
//        std::cout << "The following devices were found:" << std::endl;
//        for (size_t i = 0; i < peripherals.size(); i++) {
//            std::cout << "[" << i << "] " << peripherals[i].identifier() << " [" << peripherals[i].address() << "]"
//                      << std::endl;
//        }

        for ( auto &p : peripherals)
        {
            if(p.identifier() == deviceName)
            {
                deviceFound = true;
                break;
            }
        }
        deviceIndex++;
    }
    else
    {
        auto peripheral = peripherals[deviceIndex];
        auto adapter = adapter_optional.value();
        csound->perf_error("Found device, ready to connect\n", this);
    }
;
}
