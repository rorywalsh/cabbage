/*
 ==============================================================================
 
 cabbageIdentifierOpcodes.cpp
 Created: 12 Mar 2021 12:47:32pm
 Author:  walshr
 
 ==============================================================================
 */

#include "../Audio/Plugins/CsoundPluginProcessor.h"
#include "CabbageIdentifierOpcodes.h"
#include <exception>
#include <iostream>
#include "filesystem.hpp"


//====================================================================================================
int CreateCabbageWidget::createWidget()
{
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
    }
    
    const String widgetTreeIdentifier = "TempWidget";
    ValueTree tempWidget(widgetTreeIdentifier);
    if(in_count()!=2)
    {
        csound->init_error("Not enough arguments\n");
    }
    
    String cabbageWidget(args.str_data(0).data);
    String cabbageCode(args.str_data(1).data);
    if(cabbageCode.isEmpty() || cabbageWidget.isEmpty())
    {
        return NOTOK;
    }
    
    cabbageCode = cabbageWidget + " " + cabbageCode;
    
    CabbageWidgetData::setWidgetState(tempWidget, cabbageCode.trimCharactersAtStart(" \t"),
                                      varData->data.getNumChildren()+1);
    
    //don't duplicate widgets - based on channels..
    bool foundDuplicate = false;
    for ( auto child : varData->data)
    {
        String childChannel = CabbageWidgetData::getStringProp(child, CabbageIdentifierIds::channel);
        String channel = CabbageWidgetData::getStringProp(tempWidget, CabbageIdentifierIds::channel);
        if (childChannel.isNotEmpty() && (childChannel == channel))
            foundDuplicate = true;
    }
    
    if(foundDuplicate)
        return OK;
    
    String widgetNameId = CabbageWidgetData::getStringProp(tempWidget, CabbageIdentifierIds::channel);
    //if no name is specified use generic name
    if(widgetNameId.isEmpty())
        widgetNameId = widgetTreeIdentifier;
    
    ValueTree newWidget(widgetNameId);
    
    newWidget.copyPropertiesFrom(tempWidget, nullptr);
    
    const String typeOfWidget = CabbageWidgetData::getStringProp (newWidget, CabbageIdentifierIds::type);
    CabbageControlWidgetStrings widgets;
    if(widgets.contains(typeOfWidget))
    {
        MYFLT* value;
        const double widgetValue = CabbageWidgetData::getNumProp (newWidget, CabbageIdentifierIds::value);
        const String channel = CabbageWidgetData::getStringProp (newWidget, CabbageIdentifierIds::channel);
        if (csound->get_csound()->GetChannelPtr(csound->get_csound(), &value, channel.getCharPointer(),
                                                CSOUND_CONTROL_CHANNEL | CSOUND_OUTPUT_CHANNEL) == CSOUND_SUCCESS)
        {
            *value = widgetValue;
        }
        
    }
    varData->data.addChild(newWidget, -1,  nullptr);
    
    return OK;
}
//====================================================================================================
int GetCabbageStringIdentifierSingle::getAttribute()
{
    String identifier(inargs.str_data(1).data);
    String name(inargs.str_data(0).data);
    if(name.isEmpty() || identifier.isEmpty())
    {
        return OK;
    }
    
    
    
    
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
    }
    
    const auto child = varData->data.getChildWithName(name);
    if(child.getProperty(identifier).size()>0)
    {
        const String data = child.getProperty(identifier)[0].toString();
        outargs.str_data(0).size = strlen(data.toUTF8().getAddress());
        outargs.str_data(0).data = csound->strdup(data.toUTF8().getAddress());
    }
    else
    {
        outargs.str_data(0).size = 0;
        outargs.str_data(0).data = csound->strdup("");
    }
    
    
    return OK;
}

int GetCabbageIdentifierArray::getAttribute()
{
    csnd::Vector<MYFLT>& out = outargs.myfltvec_data(0);
    String name(inargs.str_data(0).data);
    String identifier(inargs.str_data(1).data);
    
    if(name.isEmpty() || identifier.isEmpty())
        return OK;
    
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
    else if(identifier.containsIgnoreCase("colour"))
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
    
    if(name.isEmpty() || identifier.isEmpty())
        return OK;
    
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
    }
    
    const auto child = varData->data.getChildWithName(name);
    if(child.getProperty(identifier).size()>0)
        outargs[0] = (double)child.getProperty(identifier)[0];
    else
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
    }
    
    const auto child = varData->data.getChildWithName(name);
    
    var args = child.getProperty(identifier);
    
    if(Identifier(identifier) == CabbageIdentifierIds::text)
    {
        const int size = args.size();
        out.init(csound, size);
        for ( int i = 0 ; i < size ; i++)
        {
            out[i].size = strlen(args[i].toString().toUTF8().getAddress());
            out[i].data = csound->strdup(args[i].toString().toUTF8().getAddress());
        }
        
    }
    
    
    return OK;
}

int CabbageGetWidgetChannels::getChannels()
{
    csnd::Vector<STRINGDAT>& out = outargs.vector_data<STRINGDAT>(0);
    
    
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
    }
    
    StringArray channels;
    
    if (in_count() == 1)
    {
        String identifiers(inargs.str_data(0).data);
        CabbageWidgetData::IdentifiersAndParameters idents = CabbageWidgetData::getSetofIdentifiersAndParameters(identifiers);
        
        for (int i = 0; i < idents.identifier.size(); i++)
        {
            
            for (int x = 0; x < varData->data.getNumChildren(); x++)
            {
                const String widgetTreeIdentifier = "TempWidget";
                ValueTree tempWidget(widgetTreeIdentifier);
                CabbageWidgetData::setCustomWidgetState(tempWidget, identifiers);
                if (idents.identifier[i].isNotEmpty())
                {
                    DBG(CabbageWidgetData::getStringProp(varData->data.getChild(x), CabbageIdentifierIds::channel));
                    DBG(CabbageWidgetData::getProperty(tempWidget, Identifier(idents.identifier[i])).toString());
                    DBG(CabbageWidgetData::getProperty(varData->data.getChild(x), Identifier(idents.identifier[i])).toString());
                    
                    if (CabbageWidgetData::getProperty(tempWidget, Identifier(idents.identifier[i])).toString() ==
                        CabbageWidgetData::getProperty(varData->data.getChild(x), Identifier(idents.identifier[i])).toString())
                    {
                        var chans = CabbageWidgetData::getStringProp(varData->data.getChild(x), CabbageIdentifierIds::channel);
                        if (chans.size() > 1)
                        {
                            for (int n = 0; n < chans.size(); n++)
                            {
                                DBG(chans[n].toString());
                                channels.add(chans[n].toString());
                            }
                        }
                        else
                            channels.add(chans.toString());
                    }
                }
            }
        }
        
        const int size = channels.size();
        out.init(csound, size);
        for (int i = 0; i < size; i++)
        {
            out[i].size = strlen(channels[i].toUTF8().getAddress());
            out[i].data = csound->strdup(channels[i].toUTF8().getAddress());
        }
    }
    else
    {
        for (int x = 0; x < varData->data.getNumChildren(); x++)
        {
            var chans = CabbageWidgetData::getStringProp(varData->data.getChild(x), CabbageIdentifierIds::channel);
            if (chans.size() > 1)
            {
                for (int n = 0; n < chans.size(); n++)
                {
                    channels.add(chans[n].toString());
                }
            }
            else
                channels.add(chans.toString());
        }
        
        const int size = channels.size();
        out.init(csound, size);
        for (int i = 0; i < size; i++)
        {
            out[i].size = strlen(channels[i].toUTF8().getAddress());
            out[i].data = csound->strdup(channels[i].toUTF8().getAddress());
        }
    }
    
    
    
    return OK;
}
//====================================================================================================

int GetCabbageStringValue::getAttribute()
{
    
    if(in_count() == 0)
        return NOTOK;
    
    
    if (csound->get_csound()->GetChannelPtr(csound->get_csound(), &value, inargs.str_data(0).data,
                                            CSOUND_STRING_CHANNEL | CSOUND_OUTPUT_CHANNEL) == CSOUND_SUCCESS)
    {
        outargs.str_data(0).size = ((STRINGDAT*)value)->size;
        outargs.str_data(0).data = csound->strdup(((STRINGDAT*)value)->data);
    }
    
    
    return OK;
}

int GetCabbageValue::getAttribute()
{
    if(in_count() == 0)
        return NOTOK;
    
    
    if (csound->get_csound()->GetChannelPtr(csound->get_csound(), &value, inargs.str_data(0).data,
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
    
    
    if (csound->get_csound()->GetChannelPtr(csound->get_csound(), &value, inargs.str_data(0).data,
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
        
        outargs.str_data(0).size = strlen(currentString);
        outargs.str_data(0).data = csound->strdup(currentString);
    }
    
    
    return OK;
}

int GetCabbageValueWithTrigger::getAttribute()
{
    if(in_count() == 0)
        return NOTOK;
    
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

int SetCabbageValueIdentifier::setAttribute()
{
    int trigger = outargs[2];
    
    if(trigger == 0)
        return OK;
    
    CabbageWidgetIdentifiers::IdentifierData data;
    
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
    }
    
    if(trigger == 1)
    {
        if(csound->get_csound()->GetChannelPtr(csound->get_csound(), &value, outargs.str_data(0).data,
                                               CSOUND_CONTROL_CHANNEL | CSOUND_OUTPUT_CHANNEL) == CSOUND_SUCCESS)
        {
            *value = outargs[1];
        }
        data.args = outargs[1];
        varData->data.add(data);
    }
    
    return OK;
}

//====================================================================================================
int SetCabbageIdentifier::setAttribute()
{
    int trigger = args[0];
    
    if(trigger == 0)
        return OK;
    
    CabbageWidgetIdentifiers::IdentifierData data;
    String name(args.str_data(1).data);
    String identifier(args.str_data(2).data);
    data.identifier = identifier;
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
    }
    
    
    
    if(trigger == 1)
    {
        //hack to trigger table update even if table number hasn't changed
        if(identifier == "tableNumber")
        {
            CabbageWidgetIdentifiers::IdentifierData updateData1;
            updateData1.identifier = CabbageIdentifierIds::update;
            updateData1.name = name;
            updateData1.args = 1;
            varData->data.add(updateData1);
        }
        
        if(in_count() == 3)
        {
            data.isSingleIdent = false;
            data.args = String(args.str_data(2).data);
            //DBG(String(outargs.str_data(3).data));
        }
        else
        {
            for ( int i = 3 ; i < in_count(); i++)
            {
                //DBG(outargs[i]);
                data.args.append(args[i]);
            }
        }
        varData->data.add(data);
        
        //hack to trigger table update even if table number hasn't changed
        if(identifier == "tableNumber")
        {
            CabbageWidgetIdentifiers::IdentifierData updateData0;
            updateData0.identifier = CabbageIdentifierIds::update;
            updateData0.name = name;
            updateData0.args = 0;
            varData->data.add(updateData0);
        }
    }
    return OK;
}

int SetCabbageIdentifierSArgs::setAttribute()
{
    //csnd::plugin<SetCabbageIdentifierSArgs>((csnd::Csound*) csound->GetCsound(), "cabbageSet", "", "kSS", csnd::thread::k);
    
    CabbageWidgetIdentifiers::IdentifierData data;
    int trigger = args[0];
    if(trigger == 0)
        return OK;
    
    if(in_count()<3){
        csound->perf_error("Not enough arguments\n", this);
        return NOTOK;
    }
    
    String name(args.str_data(1).data);
    String identifier(args.str_data(2).data);
    data.identifier = identifier;
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
    }
    
    //hack to trigger table update even if table number hasn't changed
    if(identifier == "tableNumber")
    {
        CabbageWidgetIdentifiers::IdentifierData updateData1;
        updateData1.identifier = CabbageIdentifierIds::update;
        updateData1.name = name;
        updateData1.args = 1;
        varData->data.add(updateData1);
    }
    
    if(identifier.isEmpty() || in_count() == 3)
    {
        data.isSingleIdent = false;
        data.args = String(args.str_data(2).data);
    }
    else
    {
        for ( int i = 3 ; i < in_count(); i++)
        {
            data.args.append(String(args.str_data(i).data));
        }
    }
    varData->data.add(data);
    
    //hack to trigger table update even if table number hasn't changed
    if(identifier == "tableNumber")
    {
        CabbageWidgetIdentifiers::IdentifierData updateData0;
        updateData0.identifier = CabbageIdentifierIds::update;
        updateData0.name = name;
        updateData0.args = 0;
        varData->data.add(updateData0);
    }
    
    return OK;
}

//--------------------------------------------------------------------------------------------------------------------------------
int SetCabbageIdentifierITime::setAttribute()
{
    CabbageWidgetIdentifiers::IdentifierData data;
    String name(outargs.str_data(0).data);
    String identifier(outargs.str_data(1).data);
    data.identifier = identifier;
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
    }
    
    //hack to trigger table update even if table number hasn't changed
    if(identifier == "tableNumber")
    {
        CabbageWidgetIdentifiers::IdentifierData updateData1;
        updateData1.identifier = CabbageIdentifierIds::update;
        updateData1.name = name;
        updateData1.args = 1;
        varData->data.add(updateData1);
    }
    
    if(in_count() == 2)
    {
        data.isSingleIdent = false;
        data.args = String(outargs.str_data(1).data);
    }
    else
    {
        for ( int i = 2 ; i < in_count(); i++)
        {
            data.args.append(double(outargs[i]));
        }
    }
    varData->data.add(data);
    
    if(identifier == "tableNumber")
    {
        CabbageWidgetIdentifiers::IdentifierData updateData0;
        updateData0.identifier = CabbageIdentifierIds::update;
        updateData0.name = name;
        updateData0.args = 0;
        varData->data.add(updateData0);
    }
    
    return OK;
}

int SetCabbageIdentifierITimeSArgs::setAttribute()
{
    CabbageWidgetIdentifiers::IdentifierData data;
    String name(outargs.str_data(0).data);
    String identifier(outargs.str_data(1).data);
    data.identifier = identifier;
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
    }
    
    //hack to trigger table update even if table number hasn't changed
    if(identifier.contains("tableNumber"))
    {
        CabbageWidgetIdentifiers::IdentifierData updateData1;
        updateData1.identifier = CabbageIdentifierIds::update;
        updateData1.name = name;
        updateData1.args = 1;
        varData->data.add(updateData1);
    }
    
    if(in_count() == 2)
    {
        data.isSingleIdent = false; // contains more than one identifier....
        data.args = String(outargs.str_data(1).data);
    }
    else
    {
        for ( int i = 2 ; i < in_count(); i++)
        {
            data.args.append(String(outargs.str_data(i).data));
        }
    }
    varData->data.add(data);
    
    if(identifier.contains("tableNumber"))
    {
        CabbageWidgetIdentifiers::IdentifierData updateData0;
        updateData0.identifier = CabbageIdentifierIds::update;
        updateData0.name = name;
        updateData0.args = 0;
        varData->data.add(updateData0);
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
        
        outargs.str_data(0).size = strlen(channelString);
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

//-----------------------------------------------------------------------------------------------------
int getFileInfo(csnd::Plugin<1,1>* opcodeData, String type)
{
    
    
    if(String(opcodeData->inargs.str_data(0).data).isEmpty())
    {
        opcodeData->outargs.str_data(0).size = 0;
        opcodeData->outargs.str_data(0).data = opcodeData->csound->strdup("");
        return OK;
    }
    
    String inputFile = String(opcodeData->inargs.str_data(0).data);
    if(File::isAbsolutePath(inputFile) == false)
    {
        //opcodeData->csound->message(String(inputFile + " is not a valid path").toUTF8().getAddress());
        return OK;
    }
    
    File file(String(opcodeData->inargs.str_data(0).data));
    
    String result = "";
    
    if(type == "name")
        result = file.getFileName();
    if(type == "path")
        result = file.getParentDirectory().getFullPathName();
    if(type == "extension")
        result = file.getFileExtension();
    if(type == "noExtension")
        result = file.getFileNameWithoutExtension();
    
#ifdef JUCE_WINDOWS
    opcodeData->outargs.str_data(0).size = strlen(result.replace("\\", "\\\\").toRawUTF8());
    opcodeData->outargs.str_data(0).data = opcodeData->csound->strdup(result.replace("\\", "\\\\").toUTF8().getAddress());
    return OK;
#endif
    
    opcodeData->outargs.str_data(0).size = strlen(result.toRawUTF8());
    opcodeData->outargs.str_data(0).data = opcodeData->csound->strdup(result.toUTF8().getAddress());
    return OK;
    
}
//-----------------------------------------------------------------------------------------------------
int CabbageFindFilesI::findFiles()
{
    if (in_count() < 1)
    {
        csound->message("Not enough parameters passed to cabbageFindFiles.\n");
        return NOTOK;
    }
    
    String fileExt = "*";
    File::TypesOfFileToFind typeOfFiles = File::TypesOfFileToFind::findFiles;
    csnd::Vector<STRINGDAT>& out = outargs.vector_data<STRINGDAT>(0);
    
    if (in_count() == 3)
        fileExt = String(inargs.str_data(2).data);
    
    if (in_count() == 2)
    {
        const String types = String(inargs.str_data(1).data);
        if(types == "filesAndDirectories" || types == "directoriesAndFiles")
        {
            typeOfFiles = File::TypesOfFileToFind::findFilesAndDirectories;
        }
        else if(types == "directories")
        {
            typeOfFiles = File::TypesOfFileToFind::findDirectories;
        }
    }
    
    Array<File> dirFiles;
    
    File dirToSearch = File::getCurrentWorkingDirectory().getChildFile(String(inargs.str_data(0).data));
    
    
    dirToSearch.findChildFiles (dirFiles, typeOfFiles, false, fileExt);
    out.init(csound, (int)dirFiles.size());
    
    for ( int i = 0 ; i < dirFiles.size() ; i++)
    {
        out[i].size = strlen(dirFiles[i].getFullPathName().toUTF8().getAddress());
        out[i].data = csound->strdup(dirFiles[i].getFullPathName().toUTF8().getAddress());
    }
    return OK;
}

int CabbageFindFilesK::findFiles()
{
    csnd::Vector<STRINGDAT>& outs = outargs.vector_data<STRINGDAT>(0);
    
    if( inargs[0] == 1 )
    {
        if (in_count() < 2)
        {
            csound->message("Not enough parameters passed to cabbageFindFiles.\n");
            return NOTOK;
        }
        
        juce::String fileExt = "*";
        juce::File::TypesOfFileToFind typeOfFiles = juce::File::TypesOfFileToFind::findFiles;
        
        
        if (in_count() == 4)
            fileExt = juce::String(inargs.str_data(2).data);
        
        if (in_count() == 3)
        {
            const juce::String types = juce::String(inargs.str_data(2).data);
            if(types == "filesAndDirectories")
            {
                typeOfFiles = juce::File::TypesOfFileToFind::findFilesAndDirectories;
            }
            else if(types == "directories")
            {
                typeOfFiles = juce::File::TypesOfFileToFind::findDirectories;
            }
        }
        
        dirFiles.clear();
        juce::File dirToSearch = juce::File::getCurrentWorkingDirectory().getChildFile(juce::String(inargs.str_data(1).data));
        dirToSearch.findChildFiles (dirFiles, typeOfFiles, false, fileExt);
        
        outs.init(csound, (int)dirFiles.size());
        
        for ( int i = 0 ; i < dirFiles.size() ; i++)
        {
            outs[i].size = strlen(dirFiles[i].getFullPathName().toUTF8().getAddress());
            outs[i].data = csound->strdup(dirFiles[i].getFullPathName().toUTF8().getAddress());
        }
    }
    
    return OK;
}
//-----------------------------------------------------------------------------------------------------
int CabbageCopyFile::copyFiles()
{
    if (in_count() < 2)
    {
        csound->message("Not enough parameters passed to cabbageCopyFile.\n");
        return NOTOK;
    }
    
    bool folderAlreadyExists = false;
    String newLocation = File::getCurrentWorkingDirectory().getChildFile(String(args.str_data(0).data)).getFullPathName();
    const String extension = File(newLocation).getFileExtension();
    String newFolder = File(newLocation).getParentDirectory().getFullPathName()+"/"+File(newLocation).getFileNameWithoutExtension();
    
    if(File(newLocation).exists() == false)
    {
        ghc::filesystem::create_directory(newFolder.toStdString());
    }
    else
    {
        newFolder = newLocation;
        folderAlreadyExists = true;
    }
    
    
    for ( int i = 1 ; i < in_count() ; i++)
    {
        File file(File::getCurrentWorkingDirectory().getChildFile(String(args.str_data(i).data)));
        File newFile(newFolder+"/"+ String(args.str_data(i).data));
        if(file.existsAsFile())
        {
            const auto copyOptions = ghc::filesystem::copy_options::overwrite_existing;
            ghc::filesystem::copy(file.getFullPathName().toStdString(), newFile.getFullPathName().toStdString(), copyOptions);
        }
        else
        {
            const String message = "cabbageCopyFile - Warning: The file '"+file.getFullPathName() +"' does not exist.";
            csound->message(message.toStdString());
        }
    }
    
    if(folderAlreadyExists == false)
    {
        ghc::filesystem::rename(newFolder.toStdString(), newLocation.toStdString());
    }
    return OK;
}

//====================================================================================================
int GetCabbageReservedChannelString::getAttribute()
{
    String channel(inargs.str_data(0).data);
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