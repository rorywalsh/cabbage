//
//  CabbageSocketOpcode.cpp
//  Cabbage
//
//  Created by Rory on 09/02/2023.
//

#include "CabbageWebUIOpcodes.h"
#include "../Widgets/CabbageWidgetData.h"

int CabbageWebSendScalar::sendScalarToWebUI(bool init)
{
    String cabbageWidget = {}, eventName = {};

    if (in_count() == 3)
    {
        cabbageWidget = args.str_data(0).data;
        eventName = args.str_data(1).data;
        trigger = 1;
    }
    else
    {
        trigger = int(args[0]);
        cabbageWidget = args.str_data(1).data;
        eventName = args.str_data(2).data;
    }
        
    if(init)
    {
        csound->plugin_deinit(this);
        valueTree = CabbageWebOpcodes::assignValueTree(vt, csound, cabbageWidget);
    }
    
    if(trigger)
    {
        if(valueTree.getType() != Identifier("null"))
        {
            //      const MessageManagerLock mmLock;
            auto value = args[in_count() == 3 ? 2 : 3];
            var v(new DynamicObject);
            v.getDynamicObject()->setProperty("name", eventName);
            v.getDynamicObject()->setProperty("data", value);
            valueTree.setProperty("jsonData", v, nullptr);
        }
    }
    
    if(valueTree.getType() == Identifier("null") && init)
        csound->message("Could not find widget with channel name:" + cabbageWidget.toStdString());
    
    return OK;
}

//===========================================================================================
int CabbageWebSendArray::sendArrayToWebUI(bool init)
{
    String cabbageWidget = {}, eventName = {};

    if (in_count() == 3)
    {
        cabbageWidget = args.str_data(0).data;
        eventName = args.str_data(1).data;
        trigger = 1;
    }
    else
    {
        trigger = int(args[0]);
        cabbageWidget = args.str_data(1).data;
        eventName = args.str_data(2).data;
    }
        
    if(init)
    {
        csound->plugin_deinit(this);
        valueTree = CabbageWebOpcodes::assignValueTree(vt, csound, cabbageWidget);
    }
    
    if(trigger)
    {
        if(valueTree.getType() != Identifier("null"))
        {
            //      const MessageManagerLock mmLock;
            csnd::Vector<MYFLT>& arrayData = args.myfltvec_data(in_count() == 3 ? 2 : 3);
            String array = "[";
            int index = 0;
            for (auto v : arrayData)
            {
                if(index<arrayData.len()-1)
                    array += String(v) + ", ";
                else
                    array += String(v) + "]";
                index++;
            }
            
            var v(new DynamicObject);
            v.getDynamicObject()->setProperty("name", eventName);
            v.getDynamicObject()->setProperty("data", array);
            valueTree.setProperty("jsonData", v, nullptr);
        }
    }
    
    if(valueTree.getType() == Identifier("null") && init)
        csound->message("Could not find widget with channel name:" + cabbageWidget.toStdString());
    
    return OK;
}

int CabbageWebSendASig::sendASigToWebUI(bool init)
{
    String cabbageWidget = {}, eventName = {};

    if (in_count() == 3)
    {
        cabbageWidget = args.str_data(0).data;
        eventName = args.str_data(1).data;
        trigger = 1;
    }
    else
    {
        trigger = int(args[0]);
        cabbageWidget = args.str_data(1).data;
        eventName = args.str_data(2).data;
    }
        
    if(init)
    {
        csound->plugin_deinit(this);
        valueTree = CabbageWebOpcodes::assignValueTree(vt, csound, cabbageWidget);
    }
    
    if(trigger)
    {
        if(valueTree.getType() != Identifier("null"))
        {
            //      const MessageManagerLock mmLock;
            csnd::AudioSig in(this, args(in_count() == 3 ? 2 : 3));
            
            String array = "[";
            int index = 0;
            for(auto &s : in)
            {
                if(index<ksmps()-1)
                    array += String(s) + ", ";
                else
                    array += String(s) + "]";
                index++;
            }
            
            var v(new DynamicObject);
            v.getDynamicObject()->setProperty("name", eventName);
            v.getDynamicObject()->setProperty("data", array);
            valueTree.setProperty("jsonData", v, nullptr);
        }
    }
    
    if(valueTree.getType() == Identifier("null") && init)
        csound->message("Could not find widget with channel name:" + cabbageWidget.toStdString());
    
    return OK;
}

//===========================================================================================
int CabbageWebSendTable::sendTableToWebUI(bool init)
{
    String cabbageWidget = {}, eventName = {};

    if (in_count() == 3)
    {
        cabbageWidget = args.str_data(0).data;
        eventName = args.str_data(1).data;
        trigger = 1;
    }
    else
    {
        trigger = int(args[0]);
        cabbageWidget = args.str_data(1).data;
        eventName = args.str_data(2).data;
    }
        
    if(init)
    {
        csound->plugin_deinit(this);
        table.init(csound, args(in_count() == 3 ? 2 : 3));
        valueTree = CabbageWebOpcodes::assignValueTree(vt, csound, cabbageWidget);
    }
    
    if(trigger)
    {
        if(valueTree.getType() != Identifier("null"))
        {
            //      const MessageManagerLock mmLock;
            String array = "[";
            int index = 0;
            for (auto v : table)
            {
                if(index<table.len()-1)
                    array += String(v) + ", ";
                else
                    array += String(v) + "]";
                index++;
            }
            
            var v(new DynamicObject);
            v.getDynamicObject()->setProperty("name", eventName);
            v.getDynamicObject()->setProperty("data", array);
            valueTree.setProperty("jsonData", v, nullptr);
        }
    }
    
    if(valueTree.getType() == Identifier("null") && init)
        csound->message("Could not find widget with channel name:" + cabbageWidget.toStdString());
    
    return OK;
}

//===========================================================================================
int CabbageWebSendString::sendStringToWebUI(bool init)
{
    String cabbageWidget(args.str_data(0).data);
    
    if(init)
    {
        csound->plugin_deinit(this);
        valueTree = CabbageWebOpcodes::assignValueTree(vt, csound, cabbageWidget);
    }

    if(valueTree.getType() != Identifier("null"))
    {
//      const MessageManagerLock mmLock;
        auto value = String(args.str_data(2).data);
//        Identifier prop(args.str_data(1).data);
//        var v(new DynamicObject);
//        v.getDynamicObject()->setProperty(prop, value);
//        valueTree.setProperty("jsonData", v, nullptr);
    }
        
    
    if(valueTree.getType() == Identifier("null") && init)
        csound->message("Could not find widget with channel name:" + cabbageWidget.toStdString());
    
    return OK;
}
