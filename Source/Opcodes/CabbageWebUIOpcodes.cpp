//
//  CabbageSocketOpcode.cpp
//  Cabbage
//
//  Created by Rory on 09/02/2023.
//

#include "CabbageWebUIOpcodes.h"
#include "../Widgets/CabbageWidgetData.h"

int CabbageWebSetScalar::sendScalarToWebUI(bool init)
{
    String cabbageWidget(args.str_data(0).data);
    
    if(init)
    {
        csound->plugin_deinit(this);
        valueTree = CabbageWebOpcodes::assignValueTree(vt, csound, cabbageWidget);
        //valueTree = CabbageWebOpcodes::assignValueTree(csound, cabbageWidget);
    }
//
    if(valueTree.getType() != Identifier("null"))
    {
//            const MessageManagerLock mmLock;
        auto value = args[2];
        Identifier prop(args.str_data(1).data);
        var v(new DynamicObject);
        v.getDynamicObject()->setProperty(prop, value);
        valueTree.setProperty("jsonData", v, nullptr);
    }
        
    
    if(valueTree.getType() == Identifier("null") && init)
        csound->message("Could not find widget with channel name:" + cabbageWidget.toStdString());
    
    return OK;
}
