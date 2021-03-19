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
    const String data = child.getProperty(identifier)[0].toString();
    outargs.str_data(0).data = csound->strdup(data.toUTF8().getAddress());
    
    
    return OK;
}

//====================================================================================================

int GetCabbageIdentifierArray::getAttribute()
{
    csnd::Vector<MYFLT>& out = outargs.myfltvec_data(0);
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
    
    var args = child.getProperty(identifier);
    
    if(Identifier(identifier) == CabbageIdentifierIds::bounds)
    {
        out.init(csound, 4);
        out[0] = child.getProperty(CabbageIdentifierIds::left);
        out[1] = child.getProperty(CabbageIdentifierIds::top);
        out[2] = child.getProperty(CabbageIdentifierIds::width);
        out[3] = child.getProperty(CabbageIdentifierIds::height);
    }
    else if(identifier.contains("colour"))
    {
        out.init(csound, 4);
        const Colour colour = Colour::fromString(args.toString());
        out[0] = colour.getRed();
        out[1] = colour.getGreen();
        out[2] = colour.getBlue();
        out[3] = colour.getAlpha();
    }
    
    
    return OK;
}

//=================================================================================================
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

int GetCabbageStringIdentifierArray::getAttribute()
{
    csnd::Vector<STRINGDAT>& out = outargs.vector_data<STRINGDAT>(0);
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
    
    var args = child.getProperty(identifier);
    
    if(Identifier(identifier) == CabbageIdentifierIds::text)
    {
        const int size = args.size();
        out.init(csound, size);
        for ( int i = 0 ; i < size ; i++)
        {
            out[i].data = csound->strdup(args[i].toString().toUTF8().getAddress());
        }
        
    }
    
    
    return OK;
}
//====================================================================================================

int GetCabbageStringValue::getAttribute()
{

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

int GetCabbageValue::getAttribute()
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
//-------------------------------------------------------------------------------------------
int GetCabbageStringValueWithTrigger::getAttribute()
{
    
    if(in_count() == 0)
        return NOTOK;
    
    String channelName(inargs.str_data(0).data);
    
    if (csound->get_csound()->GetChannelPtr(csound->get_csound(), &value, channelName.getCharPointer(),
                                            CSOUND_STRING_CHANNEL | CSOUND_OUTPUT_CHANNEL) == CSOUND_SUCCESS)
    {
        if(!currentString){
            currentString = csound->strdup(((STRINGDAT*)value)->data);
        }
        
        if(strcmp(currentString, ((STRINGDAT*)value)->data) != 0)
        {
            currentString = csound->strdup(((STRINGDAT*)value)->data);
            outargs[1] = 1;
        }
        else
            outargs[1] = 0;
        
        outargs.str_data(0).data = csound->strdup(currentString);
    }
    
    
    return OK;
}

int GetCabbageValueWithTrigger::getAttribute()
{
    if(in_count() == 0)
        return NOTOK;
    
    String channelName(inargs.str_data(0).data);
    
    if (csound->get_csound()->GetChannelPtr(csound->get_csound(), &value, channelName.getCharPointer(),
                                            CSOUND_CONTROL_CHANNEL | CSOUND_OUTPUT_CHANNEL) == CSOUND_SUCCESS)
    {

        if(*value != currentValue)
        {
            currentValue = *value;
            outargs[1] = 1;
        }
        else
            outargs[1] = 0;
        
        outargs[0] = currentValue;
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

//====================================================================================================
int GetCabbageReservedChannelStringWithTrigger::getAttribute()
{
       
    if (csound->get_csound()->GetChannelPtr(csound->get_csound(), &value, inargs.str_data(0).data,
                                            CSOUND_STRING_CHANNEL | CSOUND_OUTPUT_CHANNEL) == CSOUND_SUCCESS)
    {
      
        if(!channelString){
            channelString = csound->strdup(((STRINGDAT*)value)->data);
        }
        
        if(strcmp(channelString, ((STRINGDAT*)value)->data) != 0)
        {
            channelString = csound->strdup(((STRINGDAT*)value)->data);
            outargs[1] = 1;
        }
        else
            outargs[1] = 0;
        
        outargs.str_data(0).data = csound->strdup(channelString);
    }
    return OK;
}

//-----------------------------------------------------------------------------------------------------
int GetCabbageReservedChannelDataWithTrigger::getAttribute()
{
    
    if (csound->get_csound()->GetChannelPtr(csound->get_csound(), &value, inargs.str_data(0).data,
                                            CSOUND_CONTROL_CHANNEL | CSOUND_OUTPUT_CHANNEL) == CSOUND_SUCCESS)
    {
        
        if(*value != currentValue)
        {
            currentValue = *value;
            outargs[1] = 1;
        }
        else
            outargs[1] = 0;
        
        outargs[0] = currentValue;
    }
    return OK;
}

//====================================================================================================
int GetCabbageReservedChannelString::getAttribute()
{
    
    if (csound->get_csound()->GetChannelPtr(csound->get_csound(), &value, inargs.str_data(0).data,
                                            CSOUND_STRING_CHANNEL | CSOUND_OUTPUT_CHANNEL) == CSOUND_SUCCESS)
    {
        
        outargs.str_data(0).data = csound->strdup(((STRINGDAT*)value)->data);
    }
    return OK;
}

//-----------------------------------------------------------------------------------------------------
int GetCabbageReservedChannelData::getAttribute()
{
    
    if (csound->get_csound()->GetChannelPtr(csound->get_csound(), &value, inargs.str_data(0).data,
                                            CSOUND_CONTROL_CHANNEL | CSOUND_OUTPUT_CHANNEL) == CSOUND_SUCCESS)
    {
        outargs[0] = *value;
    }
    return OK;
}
