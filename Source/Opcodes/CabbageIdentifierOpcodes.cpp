/*
  ==============================================================================

    cabbageIdentifierOpcodes.cpp
    Created: 12 Mar 2021 12:47:32pm
    Author:  walshr

  ==============================================================================
*/

#include "../Audio/Plugins/CsoundPluginProcessor.h"
#include "CabbageIdentifierOpcodes.h"

int GetCabbageStringIdentifierSingle::getAttribute()
{
    //    CabbageWidgetIdentifiers::IdentifierData data;
    //    if(in_count() == 0)
    //        return NOTOK;
    //
    //    String channelName(inargs.str_data(0).data);
    //
    //    if (csound->get_csound()->GetChannelPtr(csound->get_csound(), &value, channelName.getCharPointer(),
    //                                            CSOUND_STRING_CHANNEL | CSOUND_OUTPUT_CHANNEL) == CSOUND_SUCCESS)
    //    {
    //        outargs.str_data(0).data = csound->strdup(((STRINGDAT*)value)->data);
    //    }
    //
    
    return OK;
}

//====================================================================================================


int GetCabbageIdentifierSingle::getAttribute()
{
    String name(inargs.str_data(0).data);
    String identifier(inargs.str_data(1).data);
    
    vt = (CabbageWidgetsValueTree**)csound->query_global_variable("cabbageWidgetsValueTree");
    CabbageWidgetsValueTree* varData;
    
    if (vt != nullptr)
    {
        varData = *vt;
    }
    else
    {
        csound->create_global_variable("cabbageWidgetsValueTree", sizeof(CabbageWidgetsValueTree*));
        vt = (CabbageWidgetsValueTree**)csound->query_global_variable("cabbageWidgetsValueTree");
        *vt = new CabbageWidgetsValueTree();
        varData = *vt;
        csound->message("Creating new internal state object...\n");
    }
    
    const auto child = varData->data.getChildWithName(name);    
    outargs[0] = child.getProperty(identifier);

    
    return OK;
}

//====================================================================================================


int GetCabbageStringValueIdentifier::getAttribute()
{
    CabbageWidgetIdentifiers::IdentifierData data;
    if(in_count() == 0)
        return NOTOK;
    
    String channelName(inargs.str_data(0).data);
    
   if (csound->get_csound()->GetChannelPtr(csound->get_csound(), &value, channelName.getCharPointer(),
                                                CSOUND_STRING_CHANNEL | CSOUND_OUTPUT_CHANNEL) == CSOUND_SUCCESS)
        {
            outargs.str_data(0).data = csound->strdup(((STRINGDAT*)value)->data);
        }

    
    return OK;
}

//====================================================================================================

int GetCabbageValueIdentifier::getAttribute()
{
    CabbageWidgetIdentifiers::IdentifierData data;
    if(in_count() == 0)
        return NOTOK;
    
    String channelName(inargs.str_data(0).data);
    
    if (csound->get_csound()->GetChannelPtr(csound->get_csound(), &value, channelName.getCharPointer(),
                                            CSOUND_CONTROL_CHANNEL | CSOUND_OUTPUT_CHANNEL) == CSOUND_SUCCESS)
    {
        outargs[0] = *value;
    }
    
    return OK;
}

//====================================================================================================

int SetCabbageValueIdentifier::setAttribute()
{
    CabbageWidgetIdentifiers::IdentifierData data;
    int trigger = outargs[2];
    
    String name(outargs.str_data(0).data);
    
    data.identifier = "value";
    data.name = name;
    
    vt = (CabbageWidgetIdentifiers**)csound->query_global_variable("cabbageWidgetData");
    CabbageWidgetIdentifiers* varData;
    
    if (vt != nullptr)
    {
        varData = *vt;
    }
    else
    {
        csound->create_global_variable("cabbageWidgetData", sizeof(CabbageWidgetIdentifiers*));
        vt = (CabbageWidgetIdentifiers**)csound->query_global_variable("cabbageWidgetData");
        *vt = new CabbageWidgetIdentifiers();
        varData = *vt;
        csound->message("Creating new internal state object...\n");
    }
    
    if(trigger == 1)
    {
        data.args = outargs[1];
        varData->data.add(data);
    }
    
    return OK;
}

//====================================================================================================
int SetCabbageIdentifier::setAttribute()
{
    CabbageWidgetIdentifiers::IdentifierData data;
    int trigger = outargs[0];
    String name(outargs.str_data(1).data);
    String identifier(outargs.str_data(2).data);
    data.identifier = identifier.isEmpty() ? "empty" : identifier;
    data.name = name;
    
    vt = (CabbageWidgetIdentifiers**)csound->query_global_variable("cabbageWidgetData");
    CabbageWidgetIdentifiers* varData;
    
    if (vt != nullptr)
    {
        varData = *vt;
    }
    else
    {
        csound->create_global_variable("cabbageWidgetData", sizeof(CabbageWidgetIdentifiers*));
        vt = (CabbageWidgetIdentifiers**)csound->query_global_variable("cabbageWidgetData");
        *vt = new CabbageWidgetIdentifiers();
        varData = *vt;
        csound->message("Creating new internal state object...\n");
    }
    
    if(trigger == 1)
    {
        if(identifier.isEmpty())
        {
            data.isSingleIdent = false;
            data.args = String(outargs.str_data(3).data);
            DBG(String(outargs.str_data(3).data));
        }
        else
        {
            for ( int i = 3 ; i < in_count(); i++)
            {
                if(outargs.str_data(i).data)
                    data.args.append(String(outargs.str_data(i).data));
                else
                    data.args.append(outargs[i]);
            }
        }
        varData->data.add(data);
    }
    return OK;
}

