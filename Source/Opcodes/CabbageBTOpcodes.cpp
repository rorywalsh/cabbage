//
//  CabbageMIDIOpcode.cpp
//  Cabbage
//
//  Created by Rory on 09/02/2023.
//

#include "CabbageBTOpcodes.h"


int CabbageBTOpcode::init()
{
    csound->plugin_deinit(this);
    
    if(in_count()!=2)
    {
        csound->init_error("Not enough arguments to cabbageBluetooth...");
        return NOTOK;
    }
    
    globalPresetData = (CabbagePresetData**)csound->query_global_variable("cabbageGlobalPreset");
    
    if (globalPresetData != nullptr)
    {
        presetData = *globalPresetData;
    }
    else
    {
        csound->create_global_variable("cabbageGlobalPreset", sizeof(CabbagePresetData*));
        globalPresetData = (CabbagePresetData**)csound->query_global_variable("cabbageGlobalPreset");
        *globalPresetData = new CabbagePresetData();
        presetData = *globalPresetData;
    }
    
    running = false;
    progressState = CabbageBTOpcode::ProgressState::Idle;
    return OK;
}

int CabbageBTOpcode::kperf()
{

    if (globalPresetData != nullptr)
    {
        presetData = *globalPresetData;
    }
    else
    {
        csound->create_global_variable("cabbageGlobalPreset", sizeof(CabbagePresetData*));
        globalPresetData = (CabbagePresetData**)csound->query_global_variable("cabbageGlobalPreset");
        *globalPresetData = new CabbagePresetData();
        auto p = *globalPresetData;
        p->data = "";
    }
    
    auto deviceName = String(inargs.str_data(0).data);
    auto characteristic = String(inargs.str_data(1).data);//"999949da-f046-4007-8858-b3f3054d3681"
//
    //don't start a thread unless their is data to transfer
    if(!presetData->data.empty() && running == false)
    {
        btThread = std::thread{&CabbageBTOpcode::scanConnectAndTransfer, std::ref(progressState), presetData->data, deviceName, characteristic};
        running = true;
    }
    
    outargs[0] = progressState;
    return OK;
}

void CabbageBTOpcode::scanConnectAndTransfer(int& state, std::string data, String deviceName, String characteristic)
{
   
    auto adapter_optional = Utils::getAdapter();

    if (!adapter_optional.has_value()) {
        return EXIT_FAILURE;
    }

    auto adapter = adapter_optional.value();

    std::vector<SimpleBLE::Peripheral> peripherals;

    bool foundThePi = false;
    int i = 0;
    while(!foundThePi)
    {
        adapter.set_callback_on_scan_found([&](SimpleBLE::Peripheral peripheral) { peripherals.push_back(peripheral); });

        adapter.set_callback_on_scan_start([]() { std::cout << "Scan started." << std::endl; });
        adapter.set_callback_on_scan_stop([]() { std::cout << "Scan stopped." << std::endl; });
        state = CabbageBTOpcode::ProgressState::Scanning;
        // Scan for 5 seconds and return.
        adapter.scan_for(1000);
        
        std::cout << "The following devices were found:" << std::endl;
        for (size_t i = 0; i < peripherals.size(); i++) {
            std::cout << "[" << i << "] " << peripherals[i].identifier() << " [" << peripherals[i].address() << "]"
                      << std::endl;
        }
        
        i = 0;
        for ( auto &p : peripherals)
        {
            if(String(p.identifier()).equalsIgnoreCase(deviceName))
            {
                foundThePi = true;
                break;
            }
            else
                i++;
        }
    }
    auto peripheral = peripherals[i];
          
    if(peripheral.is_connectable())
        std::cout << "Can connect";
    
    std::cout << "Connecting to " << peripheral.identifier() << " [" << peripheral.address() << "]" << std::endl;
    bool notConnected = true;
    while(notConnected)
    {
        state = CabbageBTOpcode::ProgressState::Connecting;
        try
        {
            peripheral.connect();
            notConnected = false;
            state = CabbageBTOpcode::ProgressState::Connected;
        }
        catch (std::exception e)
        { /* Please, at least do some logging or other error handling here*/
            std::cout << "Not yet connected..";
        }
    }
    

    std::cout << "Successfully connected." << std::endl;
    // Store all service and characteristic uuids in a vector.
    std::vector<std::pair<SimpleBLE::BluetoothUUID, SimpleBLE::BluetoothUUID>> uuids;
    for (auto service : peripheral.services()) {
        for (auto characteristic : service.characteristics()) {
//                std::cout << service.uuid() << std::endl;
            std::cout << characteristic.uuid() << std::endl;
            auto a = characteristic.can_write_command();
            auto b = characteristic.can_write_request();
            auto n = characteristic.can_notify();
            if(n)
                std::cout << "can notfiy";
            auto c = characteristic.capabilities();
            uuids.push_back(std::make_pair(service.uuid(), characteristic.uuid()));
        }
    }
    state = CabbageBTOpcode::ProgressState::FindingCharacteristics;
    std::cout << "The following services and characteristics were found:" << std::endl;
    for (size_t i = 0; i < uuids.size(); i++) {
        std::cout << "[" << i << "] " << uuids[i].first << " " << uuids[i].second << std::endl;
    }
    
    std::pair<SimpleBLE::BluetoothUUID, SimpleBLE::BluetoothUUID> serviceChar;
    for ( auto u : uuids)
    {
        if(u.second == characteristic.toStdString())
            serviceChar = u;
    }
    
    
    state = CabbageBTOpcode::ProgressState::Transfering;
    peripheral.write_command(serviceChar.first, serviceChar.second, data);
    peripheral.disconnect();
    state = CabbageBTOpcode::ProgressState::Disconnected;
    return;
}
