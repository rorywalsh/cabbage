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
    String childChannel = "";
    for ( auto child : varData->data)
    {
        childChannel = CabbageWidgetData::getStringProp(child, CabbageIdentifierIds::channel);
        String channel = CabbageWidgetData::getStringProp(tempWidget, CabbageIdentifierIds::channel);
        if (childChannel.isNotEmpty() && (childChannel == channel))
            foundDuplicate = true;
    }
    
    if(foundDuplicate)
    {
        //String warning = "The channel name '"+childChannel + "' is already in use. Please use a unique name for each channel.";
        //csound->message(warning.toStdString());
        return OK;
    }
    
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
        outargs.str_data(0).size = data.length()+1;
        outargs.str_data(0).data = csound->strdup(data.toUTF8().getAddress());
    }
    else
    {
        outargs.str_data(0).size = child.getProperty(identifier).toString().length()+1;
        outargs.str_data(0).data = csound->strdup(child.getProperty(identifier).toString().toUTF8().getAddress());
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
    else if(Identifier(identifier) == CabbageIdentifierIds::range)
    {
        out.init(csound, 5);
        out[0] = child.getProperty(CabbageIdentifierIds::min);
        out[1] = child.getProperty(CabbageIdentifierIds::max);
        out[2] = child.getProperty(CabbageIdentifierIds::value);
        out[3] = child.getProperty(CabbageIdentifierIds::sliderskew);
        out[4] = child.getProperty(CabbageIdentifierIds::increment);
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

int GetCabbageIdentifierSingleWithTrigger::getAttribute()
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
        currentValue = (double)child.getProperty(identifier)[0];
    else
        currentValue = child.getProperty(identifier);
    
    if ( currentValue != value)
    {
        value = currentValue;
//        if(firstRun)
//        {
//            firstRun = false;
//            outargs[1] = 0;
//        }
//        else
            outargs[1] = 1;
    }
    else
        outargs[1] = 0;
    
    outargs[0] = currentValue;
    return OK;
}

int GetCabbageIdentifierSingleITime::getAttribute()
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

//this is used for lines lie SItems[] cabbageGet "combo1", "items"
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
    
    if (name.isEmpty())
        return OK;

    const auto child = varData->data.getChildWithName(name);
    
    var args = child.getProperty(identifier);
    const auto test = child.getProperty(CabbageIdentifierIds::colour).toString();
    
    if(Identifier(identifier) == CabbageIdentifierIds::text || Identifier(identifier) == CabbageIdentifierIds::items)
    {
        if(args.isArray())
        {
            const int size = args.size();
            out.init(csound, size);
            for ( int i = 0 ; i < size ; i++)
            {
                out[i].size = args[i].toString().length()+1;
                out[i].data = csound->strdup(args[i].toString().toUTF8().getAddress());
            }
        }
        else
        {
            const int size = args.size();
            out.init(csound, size);
            for ( int i = 0 ; i < size ; i++)
            {
                out[i].size = args[i].toString().length()+1;
                out[i].data = csound->strdup(args[i].toString().toUTF8().getAddress());
            }
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
        
        for (int x = 0; x < varData->data.getNumChildren(); x++)
        {
            const String widgetTreeIdentifier = "TempWidget";
            ValueTree tempWidget(widgetTreeIdentifier);
            CabbageWidgetData::setCustomWidgetState(tempWidget, identifiers);
           
//                    DBG(CabbageWidgetData::getStringProp(varData->data.getChild(x), CabbageIdentifierIds::channel));
//                    DBG(CabbageWidgetData::getProperty(tempWidget, Identifier(idents.identifier[i])).toString());
//                    DBG(CabbageWidgetData::getProperty(varData->data.getChild(x), Identifier(idents.identifier[i])).toString());
            
            int identMatches[2048] = {0};
            
            
            for (int i = 0; i < idents.identifier.size(); i++)
            {
                if (idents.identifier[i].isNotEmpty())
                {
                    String tempData, childData;
                    if(CabbageWidgetData::getProperty(tempWidget, Identifier(idents.identifier[i])).isArray())
                        tempData = CabbageWidgetData::getProperty(tempWidget, Identifier(idents.identifier[i]))[0].toString();
                    else
                        tempData = CabbageWidgetData::getProperty(tempWidget, Identifier(idents.identifier[i])).toString();

                    if(CabbageWidgetData::getProperty(varData->data.getChild(x), Identifier(idents.identifier[i])).isArray())
                        childData = CabbageWidgetData::getProperty(varData->data.getChild(x), Identifier(idents.identifier[i]))[0].toString();
                    else
                        childData = CabbageWidgetData::getProperty(varData->data.getChild(x), Identifier(idents.identifier[i])).toString();
                    

                    if (tempData == childData)
                    {
                        identMatches[x]++;
                        var chans = CabbageWidgetData::getProperty(varData->data.getChild(x), CabbageIdentifierIds::channel);
                        if (chans.size() > 1)
                        {
                            for (int n = 0; n < chans.size(); n++)
                            {
                                if(identMatches[x] == idents.identifier.size())
                                {
                                    channels.add(chans[n].toString());
                                }
                            }
                        }
                        else
                        {
                            if(identMatches[x] == idents.identifier.size()){
                                if(chans.isArray())
                                {
                                    channels.add(chans[0].toString());
                                }
                                else{
                                    channels.add(chans.toString());
                                }
                            }
                        }
                    }
                }
            }
        }
        
        const int size = channels.size();
        out.init(csound, size);
        for (int i = 0; i < size; i++)
        {
            out[i].size = channels[i].length()+1;
            out[i].data = csound->strdup(channels[i].toUTF8().getAddress());
        }
    }
    else
    {
        for (int x = 0; x < varData->data.getNumChildren(); x++)
        {
            var chans = CabbageWidgetData::getProperty(varData->data.getChild(x), CabbageIdentifierIds::channel);
            //const Array<var>* channelArrayVar =chans.getArray();
            //DBG(channelArrayVar->size());
            if (chans.size() > 1)
            {
                for (int n = 0; n < chans.size(); n++)
                {
                    channels.add(chans[n].toString());
                }
            }
            else{
                if(chans.isArray())
                {
                    channels.add(chans[0].toString());
                }
                else{
                    channels.add(chans.toString());
                }
            }
            
        }
        
        const int size = channels.size();
        out.init(csound, size);
        for (int i = 0; i < size; i++)
        {
            out[i].size = channels[i].length()+1;
            out[i].data = csound->strdup(channels[i].toUTF8().getAddress());
        }
    }
    
    
    
    return OK;
}


int GetCabbageStringValue::getAttribute(bool init)
{
    
    if(in_count() == 0)
        return NOTOK;

    if (csound->get_csound()->GetChannelPtr(csound->get_csound(), &value, inargs.str_data(0).data,
                                            CSOUND_STRING_CHANNEL | CSOUND_OUTPUT_CHANNEL) == CSOUND_SUCCESS)
    {
        if (!currentString)
        {
            currentString = csound->strdup((((STRINGDAT*)value)->data));
        }

        if (strcmp(currentString, ((STRINGDAT*)value)->data) != 0)
        {
            currentString = csound->strdup(((STRINGDAT*)value)->data);
        }

        if (init)
        {
            outargs.str_data(0).size = ((STRINGDAT*)value)->size;
            outargs.str_data(0).data = (((STRINGDAT*)value)->data);
        }
        else //seems I need to use csound->strdup at k-time...
        {
            outargs.str_data(0).size = int(strlen(currentString)) + 1;
            outargs.str_data(0).data = currentString;
        }
    }
    
    
    return OK;
}

int GetCabbageStringValueArray::getAttribute()
{
    
    if(in_count() == 0)
        return NOTOK;
    
    csnd::Vector<STRINGDAT>& inputArgs = inargs.vector_data<STRINGDAT>(0);
    csnd::Vector<STRINGDAT>& out = outargs.vector_data<STRINGDAT>(0);
    out.init(csound, (int)inputArgs.len());
    
    for ( int i = 0 ; i < inputArgs.len() ; i++)
    {
        if (csound->get_csound()->GetChannelPtr(csound->get_csound(), &value, inputArgs[i].data,
                                                CSOUND_STRING_CHANNEL | CSOUND_OUTPUT_CHANNEL) == CSOUND_SUCCESS)
        {
            out[i].size = ((STRINGDAT*)value)->size;
            out[i].data = ((STRINGDAT*)value)->data;
        }
    }
    
    
    return OK;
}

int GetCabbageValue::getAttribute(bool irate)
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

int GetCabbageValueArray::getAttribute()
{
    if(in_count() == 0)
        return NOTOK;
    
    csnd::Vector<STRINGDAT>& inputArgs = inargs.vector_data<STRINGDAT>(0);
    csnd::Vector<MYFLT>& out = outargs.myfltvec_data(0);
    out.init(csound, (int)inputArgs.len());
    
    for ( int i = 0 ; i < (int)inputArgs.len() ; i++)
    {
        if (csound->get_csound()->GetChannelPtr(csound->get_csound(), &value, inputArgs[i].data,
                                                CSOUND_CONTROL_CHANNEL | CSOUND_OUTPUT_CHANNEL) == CSOUND_SUCCESS)
        {
            out[i] = *value;
        }
    }
    
    return OK;
}


int GetCabbageStringValueWithTrigger::getAttribute(bool init)
{
    if(in_count() == 0)
        return NOTOK;
    
    int trigOnInit = 0;

    if(in_count() == 2)
        trigOnInit = inargs[1];

    if (csound->get_csound()->GetChannelPtr(csound->get_csound(), &value, inargs.str_data(0).data,
                                            CSOUND_STRING_CHANNEL | CSOUND_OUTPUT_CHANNEL) == CSOUND_SUCCESS)
    {
        const auto s = csound->strdup(inargs.str_data(0).data);
        if(!currentString){
            DBG(csound->strdup(inargs.str_data(0).data));
            currentString = csound->strdup((((STRINGDAT*)value)->data));
        }
        
        if(strcmp(currentString, ((STRINGDAT*)value)->data) != 0)
        {
            DBG(csound->strdup(inargs.str_data(0).data));
            currentString = csound->strdup(((STRINGDAT*)value)->data);
            outargs[1] = 1;
        }
        else
        {
            if (trigOnInit && !init && kCycleCount == 1) {
                outargs[1] = 1;
                DBG(inargs.str_data(0).data);
            }
                
            else
                outargs[1] = 0;
        }
        
        outargs.str_data(0).size = int(strlen(currentString))+1;
        outargs.str_data(0).data = currentString;
    }
    
    if (init)
        kCycleCount = 0;
    else
        kCycleCount++;

    return OK;
}

int GetCabbageStringValueArrayWithTrigger::getAttribute()
{
    
    if(in_count() == 0)
        return NOTOK;
    
    csnd::Vector<STRINGDAT>& inputArgs = inargs.vector_data<STRINGDAT>(0);
    csnd::Vector<STRINGDAT>& out = outargs.vector_data<STRINGDAT>(0);
    csnd::Vector<MYFLT>& outTriggers = outargs.myfltvec_data(1);
    out.init(csound, (int)inputArgs.len());
    outTriggers.init(csound, (int)inputArgs.len());
    
    for (unsigned long i = 0 ; i < int(inputArgs.len()) ; i++)
    {
    if (csound->get_csound()->GetChannelPtr(csound->get_csound(), &value, inputArgs[i].data,
                                            CSOUND_STRING_CHANNEL | CSOUND_OUTPUT_CHANNEL) == CSOUND_SUCCESS)
    {
            if(currentStrings[i].size == 0){
                currentStrings[i].data = csound->strdup(((STRINGDAT*)value)->data);
                currentStrings[i].size = ((STRINGDAT*)value)->size;
            }
        
            if(strcmp(currentStrings[i].data, ((STRINGDAT*)value)->data) != 0)
            {
                currentStrings[i].data = csound->strdup(((STRINGDAT*)value)->data);
                currentStrings[i].size = ((STRINGDAT*)value)->size;
                outTriggers[i] = 1;
            }
            else
                outTriggers[i] = 0;
        
            out[i].size = currentStrings[i].size+1;
            out[i].data = currentStrings[i].data;
        }
    }
    
    return OK;
}

int GetCabbageValueWithTrigger::getAttribute()
{
    if(in_count() == 0)
        return NOTOK;
    
    if(in_count() > 1)
        triggerOnPerfPass = inargs[1];
    

    if (csound->get_csound()->GetChannelPtr(csound->get_csound(), &value, inargs.str_data(0).data,
                                            CSOUND_CONTROL_CHANNEL | CSOUND_OUTPUT_CHANNEL) == CSOUND_SUCCESS)
    {
        numberOfPasses = (numberOfPasses < 3 ? numberOfPasses+1 : 3);

        if(*value != currentValue)
        {
            currentValue = *value;
            outargs[1] = 1;
            outargs[0] = currentValue;
        }
        else
        {
            if(numberOfPasses == 2 && triggerOnPerfPass>0)
            {
                outargs[1] = 1;
            }
            else
                outargs[1] = 0;
        }
        //   
        
    }
    
    return OK;
}

int GetCabbageValueArrayWithTrigger::getAttribute()
{
    if(in_count() == 0)
        return NOTOK;
    
    csnd::Vector<STRINGDAT>& inputArgs = inargs.vector_data<STRINGDAT>(0);
    csnd::Vector<MYFLT>& out = outargs.myfltvec_data(0);
    csnd::Vector<MYFLT>& outTriggers = outargs.myfltvec_data(1);
    out.init(csound, (int)inputArgs.len());
    outTriggers.init(csound, (int)inputArgs.len());
    
    for ( int i = 0 ; i < int(inputArgs.len()) ; i++)
    {
        if (csound->get_csound()->GetChannelPtr(csound->get_csound(), &value, inputArgs[i].data,
                                                CSOUND_CONTROL_CHANNEL | CSOUND_OUTPUT_CHANNEL) == CSOUND_SUCCESS)
        {
            
            if(*value != currentValue[i])
            {
                currentValue[i] = *value;
//                if(firstRun)
//                {
//                    firstRun = false;
//                    outTriggers[i] = 0;
//                }
//                else
                    outTriggers[i] = 1;
            }
            else
                outTriggers[i] = 0;
            
            out[i] = currentValue[i];
        }
    }
    
    return OK;
}

//=========================================================================================
int CabbageValueChanged::getAttribute()
{
    if(in_count() == 0)
        return NOTOK;

    if(in_count() == 3)
        mode = int(inargs[2]);
    else
        mode = 2;
    
    csnd::Vector<STRINGDAT>& inputArgs = inargs.vector_data<STRINGDAT>(0);
    bool foundAChange = false;
    
    if(mode == 3 && firstPerfPass)
    {
        outargs.str_data(0) = inputArgs[0];
        outargs[1] = 1;
        firstPerfPass = false;
    }
    else
    {
        for (unsigned long i = 0 ; i < int(inputArgs.len()) ; i++)
        {
            if (csound->get_csound()->GetChannelPtr(csound->get_csound(), &value, inputArgs[i].data,
                                                    CSOUND_CONTROL_CHANNEL | CSOUND_OUTPUT_CHANNEL) == CSOUND_SUCCESS)
            {
                if(in_count() > 1 && mode != 3)
                {
                    if(mode == 2)
                    {
                        if ((oldValue[i] <= inargs[1] && *value > inargs[1]) ||
                            (oldValue[i] >= inargs[1] && *value < inargs[1] ) )
                        {
                            outargs.str_data(0).size = inputArgs[i].size;
                            outargs.str_data(0).data = csound->strdup(inputArgs[i].data);
                            foundAChange = true;
                        }
                    }
                    else if(mode == 0)
                    {
                        if (oldValue[i] <= inargs[1] && *value > inargs[1])
                        {
                            outargs.str_data(0).size = inputArgs[i].size;
                            outargs.str_data(0).data = csound->strdup(inputArgs[i].data);
                            foundAChange = true;
                        }
                    }
                    else if(mode == 1)
                    {
                        if (oldValue[i] >= inargs[1] && *value < inargs[1])
                        {
                            outargs.str_data(0).size = inputArgs[i].size;
                            outargs.str_data(0).data = csound->strdup(inputArgs[i].data);
                            foundAChange = true;
                        }
                    }
                    
                    oldValue[i] = *value;
                }
                else
                {
                    if(*value != oldValue[i])
                    {
                        oldValue[i] = *value;
                        outargs.str_data(0).size = inputArgs[i].size;
                        outargs.str_data(0).data = csound->strdup(inputArgs[i].data);
                        foundAChange = true;
                    }
                }
            }
            else if (csound->get_csound()->GetChannelPtr(csound->get_csound(), &value, inputArgs[i].data,
                                                         CSOUND_STRING_CHANNEL | CSOUND_OUTPUT_CHANNEL) == CSOUND_SUCCESS)
            {
                if(int(currentStrings[static_cast<unsigned long>(i)].size) == 0){
                    currentStrings[i].data = csound->strdup(((STRINGDAT*)value)->data);
                    currentStrings[i].size = ((STRINGDAT*)value)->size;
                }
                
                if(strcmp(currentStrings[i].data, ((STRINGDAT*)value)->data) != 0)
                {
                    currentStrings[i].data = csound->strdup(((STRINGDAT*)value)->data);
                    currentStrings[i].size = ((STRINGDAT*)value)->size;
                    foundAChange = true;
                    outargs.str_data(0).size = inputArgs[i].size;
                    outargs.str_data(0).data = csound->strdup(inputArgs[i].data);
                }
            }
        }
        
        if(foundAChange)
            outargs[1] = 1;
        else
            outargs[1] = 0;
    }
    return OK;
}

int CabbageValueChangedIndex::getAttribute()
{
    if(in_count() == 0)
        return NOTOK;
    
    if(in_count() == 3)
        mode = int(inargs[2]);
    else
        mode = 2;
    
    csnd::Vector<STRINGDAT>& inputArgs = inargs.vector_data<STRINGDAT>(0);
    bool foundAChange = false;
    
    if(mode == 3 && firstPerfPass)
    {
        outargs[0] = 0;
        outargs[1] = 1;
        firstPerfPass = false;
    }
    else
    {
        for ( unsigned long i = 0 ; i < int(inputArgs.len()) ; i++)
        {
            if (csound->get_csound()->GetChannelPtr(csound->get_csound(), &value, inputArgs[i].data,
                                                    CSOUND_CONTROL_CHANNEL | CSOUND_OUTPUT_CHANNEL) == CSOUND_SUCCESS)
            {
                if(in_count() > 1 && mode != 3)
                {
                    if(mode == 2)
                    {
                        if ((oldValue[i] <= inargs[1] && *value > inargs[1]) ||
                            (oldValue[i] >= inargs[1] && *value < inargs[1] ) )
                        {
                            //                        outargs.str_data(0).size = inputArgs[i].size;
                            //                        outargs.str_data(0).data = csound->strdup(inputArgs[i].data);
                            outargs[0] = i;
                            foundAChange = true;
                        }
                    }
                    else if(mode == 0)
                    {
                        if (oldValue[i] <= inargs[1] && *value > inargs[1])
                        {
                            //                        outargs.str_data(0).size = inputArgs[i].size;
                            //                        outargs.str_data(0).data = csound->strdup(inputArgs[i].data);
                            outargs[0] = i;
                            foundAChange = true;
                        }
                    }
                    else if(mode == 1)
                    {
                        if (oldValue[i] >= inargs[1] && *value < inargs[1])
                        {
                            //                        outargs.str_data(0).size = inputArgs[i].size;
                            //                        outargs.str_data(0).data = csound->strdup(inputArgs[i].data);
                            outargs[0] = i;
                            foundAChange = true;
                        }
                    }
                    
                    oldValue[i] = *value;
                }
                else
                {
                    if(*value != oldValue[i])
                    {
                        oldValue[i] = *value;
                        //                    outargs.str_data(0).size = inputArgs[i].size;
                        //                    outargs.str_data(0).data = csound->strdup(inputArgs[i].data);
                        foundAChange = true;
                        outargs[0] = i;
                    }
                }
            }
            else if (csound->get_csound()->GetChannelPtr(csound->get_csound(), &value, inputArgs[i].data,
                                                         CSOUND_STRING_CHANNEL | CSOUND_OUTPUT_CHANNEL) == CSOUND_SUCCESS)
            {
                if(strcmp(currentStrings[i].data, ((STRINGDAT*)value)->data) != 0)
                {
                    currentStrings[i].data = csound->strdup(((STRINGDAT*)value)->data);
                    currentStrings[i].size = ((STRINGDAT*)value)->size;
                    foundAChange = true;
                    outargs[0] = i;
                }
            }
        }
        
        if(foundAChange == true)
            outargs[1] = 1;
        else
            outargs[1] = 0;
    }
    return OK;
}

//====================================================================================================
// from cabbageSetValue
int SetCabbageValueIdentifier::setAttribute(bool init)
{
    
    if(in_count()<2){
        csound->perf_error("Not enough arguments\n", this);
        return NOTOK;
    }

    
    vt = (CabbageWidgetIdentifiers**)csound->query_global_variable("cabbageWidgetData");
    CabbageWidgetIdentifiers* varData = CabbageOpcodes::getGlobalvariable(csound, vt);
    //this is k-rate only so set init back to true in order to get the correct channel name on each k-cycle
    CabbageWidgetIdentifiers::IdentifierData data = getValueIdentData(args, true, 0, 1);

    
    int trigger = int(args[2]);

    if(trigger == 0 || args.str_data(0).size == 0)
        return OK;

    varData->data.getLock().enter();
    
    if(trigger == 1)
    {
        if(csound->get_csound()->GetChannelPtr(csound->get_csound(), &value, args.str_data(0).data,
                                               CSOUND_CONTROL_CHANNEL | CSOUND_INPUT_CHANNEL) == CSOUND_SUCCESS)
        {
            *value = args[1];
        }
        
        data.args = args[1];
        varData->data.add(data);

    }
    
    varData->data.getLock().exit();
    return OK;
}

int SetCabbageValueIdentifierITime::setAttribute(bool init)
{
    
    if(in_count()!=2){
        csound->perf_error("Not enough arguments\n", this);
        return NOTOK;
    }

    
    vt = (CabbageWidgetIdentifiers**)csound->query_global_variable("cabbageWidgetData");
    CabbageWidgetIdentifiers* varData = CabbageOpcodes::getGlobalvariable(csound, vt);
    //this is k-rate only so set init back to true in order to get the correct channel name on each k-cycle
    CabbageWidgetIdentifiers::IdentifierData data = getValueIdentData(args, true, 0, 1);
    if(args.str_data(0).size == 0)
        return OK;
    
    varData->data.getLock().enter();
    
    //now update underlying Csound channel
    if(csound->get_csound()->GetChannelPtr(csound->get_csound(), &value, args.str_data(0).data,
                                           CSOUND_CONTROL_CHANNEL | CSOUND_INPUT_CHANNEL) == CSOUND_SUCCESS)
    {
        *value = args[1];
    }
    
    data.args = args[1];
    varData->data.add(data);
      
    varData->data.getLock().exit();

    
    return OK;
}


int SetCabbageValueIdentifierSArgs::setAttribute(bool init)
{
    
    if(in_count()!=3){
        csound->perf_error("Not enough arguments\n", this);
        return NOTOK;
    }

    
    vt = (CabbageWidgetIdentifiers**)csound->query_global_variable("cabbageWidgetData");
    CabbageWidgetIdentifiers* varData = CabbageOpcodes::getGlobalvariable(csound, vt);
    //this is k-rate only so set init back to true in order to get the correct channel name on each k-cycle
    CabbageWidgetIdentifiers::IdentifierData data = getValueIdentData(args, true, 0, 1);
    
    int trigger = int(args[2]);
    
    if(trigger == 0 || args.str_data(0).size == 0)
        return OK;
        
    const String strValue = String(args.str_data(1).data);

    
    varData->data.getLock().enter();

    //now update underlying Csound channel
    if(trigger == 1)
    {
        
        if(csound->get_csound()->GetChannelPtr(csound->get_csound(), &str, args.str_data(0).data,
                                               CSOUND_STRING_CHANNEL | CSOUND_INPUT_CHANNEL) == CSOUND_SUCCESS)
        {
            auto* stringdat = (STRINGDAT*) str;
            stringdat->data = csound->strdup(args.str_data(1).data);
            stringdat->size = strlen(args.str_data(1).data) + 1;
        }
        
        data.args = args.str_data(1).data;
        varData->data.add(data);
        
    }
    
    varData->data.getLock().exit();
    return OK;
}

int SetCabbageValueIdentifierSArgsITime::setAttribute(bool init)
{
    if(in_count()!=2){
        csound->perf_error("Not enough arguments\n", this);
        return NOTOK;
    }
    
    vt = (CabbageWidgetIdentifiers**)csound->query_global_variable("cabbageWidgetData");
    CabbageWidgetIdentifiers* varData = CabbageOpcodes::getGlobalvariable(csound, vt);
    //this is k-rate only so set init back to true in order to get the correct channel name on each k-cycle
    CabbageWidgetIdentifiers::IdentifierData data = getValueIdentData(args, true, 0, 1);
    
    
    if(args.str_data(0).size == 0)
        return OK;
    
    varData->data.getLock().enter();
    //varData->canRead.store(false);
    data.args = args.str_data(1).data;
    varData->data.add(data);
    
    varData->data.getLock().exit();

    return OK;
}
//====================================================================================================
int SetCabbageIdentifier::setAttribute(bool init)
{

    vt = (CabbageWidgetIdentifiers**)csound->query_global_variable("cabbageWidgetData");
    CabbageWidgetIdentifiers* varData = CabbageOpcodes::getGlobalvariable(csound, vt);
    //setting second param to true here so that we can loop through arrays and run loop at k-rate
    CabbageWidgetIdentifiers::IdentifierData identData = getIdentData(args, true, 1, 2);

    
    int trigger = int(args[0]);
    
    if(trigger == 0)
        return OK;

    varData->data.getLock().enter();

    if(trigger == 1)
    {
        //hack to trigger table update even if table number hasn't changed
        triggerTableUpdate(varData, identData, 1);
        
        
        if(in_count() == 3)
        {
            identData.identWithArgument = true;
            identData.args = String(args.str_data(2).data);
        }
        else
        {
            for ( int i = 3 ; i < in_count(); i++)
            {
                identData.args.append(args[i]);
            }
        }
        varData->data.add(identData);
        
        //hack to trigger table update even if table number hasn't changed
        triggerTableUpdate(varData, identData, 0);
        
        if(identData.identifier == CabbageIdentifierIds::value)
        {
            if(csound->get_csound()->GetChannelPtr(csound->get_csound(), &value, args.str_data(1).data,
                                                   CSOUND_CONTROL_CHANNEL | CSOUND_INPUT_CHANNEL) == CSOUND_SUCCESS)
            {
                *value = args[3];
            }
        }
    }

    varData->data.getLock().exit();

    return OK;
}

int SetCabbageIdentifierArray::setAttribute(bool init)
{
    
    vt = (CabbageWidgetIdentifiers**)csound->query_global_variable("cabbageWidgetData");
    CabbageWidgetIdentifiers* varData = CabbageOpcodes::getGlobalvariable(csound, vt);
    //setting second param to true here so that we can loop through arrays and run loop at k-rate
    CabbageWidgetIdentifiers::IdentifierData data = getIdentData(args, true, 1, 2);
    
    
    int trigger = int(args[0]);
    
    if(trigger == 0)
        return OK;
    

    varData->data.getLock().enter();
    csnd::Vector<MYFLT>& inputArgs = args.myfltvec_data(3);
    
    if(trigger == 1)
    {
        //hack to trigger table update even if table number hasn't changed
        triggerTableUpdate(varData, data, 1);
        
        for (int i = 0; i < int(inputArgs.len()); i++)
        {
            data.args.append(inputArgs[i]);
        }
 
        varData->data.add(data);
        
        //hack to trigger table update even if table number hasn't changed
        triggerTableUpdate(varData, data, 0);
        
        if(data.identifier == CabbageIdentifierIds::value)
        {
            if(csound->get_csound()->GetChannelPtr(csound->get_csound(), &value, args.str_data(1).data,
                                                   CSOUND_CONTROL_CHANNEL | CSOUND_INPUT_CHANNEL) == CSOUND_SUCCESS)
            {
                *value = args[3];
            }
        }
    }

    varData->data.getLock().exit();
    return OK;
}

int SetCabbageIdentifierSArgs::setAttribute(bool init)
{

    if(in_count()<3){
        csound->perf_error("Not enough arguments\n", this);
        return NOTOK;
    }
    
    if(String(args.str_data(2).data).isEmpty())
        return OK;
        
    vt = (CabbageWidgetIdentifiers**)csound->query_global_variable("cabbageWidgetData");
    CabbageWidgetIdentifiers* varData = CabbageOpcodes::getGlobalvariable(csound, vt);
    //setting second param to true here so that we can loop through arrays and run loop at k-rate
    CabbageWidgetIdentifiers::IdentifierData data = getIdentData(args, true, 1, 2);
    

    int trigger = int(args[0]);

    if(trigger == 0)
        return OK;
    
    varData->data.getLock().enter();
    
    //hack to trigger table update even if table number hasn't changed
    triggerTableUpdate(varData, data, 1);
    
    if(String(args.str_data(2).data).isEmpty() || in_count() == 3)
    {
        data.identWithArgument = true;
        data.args = String(args.str_data(2).data);
    }
    else
    {
        for ( int i = 3 ; i < int(in_count()); i++)
        {
            data.args.append(String(args.str_data(i).data));
        }
    }
    varData->data.add(data);
    
    triggerTableUpdate(varData, data, 0);
    
    varData->data.getLock().exit();
    return OK;
}

//-----------------------------------------------------------------------------------------------------------------
int SetCabbageIdentifierITime::setAttribute(bool init)
{

    vt = (CabbageWidgetIdentifiers**)csound->query_global_variable("cabbageWidgetData");
    CabbageWidgetIdentifiers* varData = CabbageOpcodes::getGlobalvariable(csound, vt);
    CabbageWidgetIdentifiers::IdentifierData data = getIdentData(outargs, init, 0, 1);
    
    varData->data.getLock().enter();

    //hack to trigger table update even if table number hasn't changed
    triggerTableUpdate(varData, data, 1);

    if(in_count() == 2)
    {
        data.identWithArgument = true;
        data.args = String(outargs.str_data(1).data);
    }
    else
    {
        for ( int i = 2 ; i < int(in_count()); i++)
        {
            data.args.append(double(outargs[i]));
        }
    }
    varData->data.add(data);
    
    if(data.identifier == CabbageIdentifierIds::value)
    {
        if(csound->get_csound()->GetChannelPtr(csound->get_csound(), &value, outargs.str_data(1).data,
                                               CSOUND_CONTROL_CHANNEL | CSOUND_INPUT_CHANNEL) == CSOUND_SUCCESS)
        {
            *value = outargs[2];
        }
    }
    
    triggerTableUpdate(varData, data, 0);
    varData->data.getLock().exit();
    return OK;
}

int SetCabbageIdentifierITimeSArgs::setAttribute(bool init)
{
    vt = (CabbageWidgetIdentifiers**)csound->query_global_variable("cabbageWidgetData");
    CabbageWidgetIdentifiers* varData = CabbageOpcodes::getGlobalvariable(csound, vt);
   
    CabbageWidgetIdentifiers::IdentifierData data = getIdentData(outargs, init, 0, 1);
    
    varData->data.getLock().enter();
    triggerTableUpdate(varData, data, 1);
        
    
    if(in_count() == 2)
    {
        data.identWithArgument = true;
        data.args = String(outargs.str_data(1).data);
    }
    else
    {
        for ( int i = 2 ; i < int(in_count()); i++)
        {
            data.args.append(String(outargs.str_data(i).data));
        }
    }
    varData->data.add(data);
    
    if(data.identifier == CabbageIdentifierIds::value)
    {
        if(csound->get_csound()->GetChannelPtr(csound->get_csound(), &value, outargs.str_data(1).data,
                                               CSOUND_CONTROL_CHANNEL | CSOUND_INPUT_CHANNEL) == CSOUND_SUCCESS)
        {
            *value = outargs[2];
        }
    }
    
    triggerTableUpdate(varData, data, 0);
    varData->data.getLock().exit();

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
        
        outargs.str_data(0).size = int(strlen(channelString));
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
int getFileInfo(csnd::Plugin<1,1>* opcodeData, String type, std::string& currentPath)
{

    if(opcodeData->in_count() == 0)
    {
        return NOTOK;
    }

    if(currentPath != opcodeData->inargs.str_data(0).data)
    {
        currentPath = opcodeData->inargs.str_data(0).data;
        //String inputFile = String(opcodeData->inargs.str_data(0).data);
        if(!File::isAbsolutePath(opcodeData->inargs.str_data(0).data))
        {
            //opcodeData->csound->message(String(inputFile + " is not a valid path").toUTF8().getAddress());
            return OK;
        }
        
        File file(String(opcodeData->inargs.str_data(0).data));
        
        String result;
        
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
#else
        
        opcodeData->outargs.str_data(0).size = result.length()+1;
        opcodeData->outargs.str_data(0).data = opcodeData->csound->strdup(result.toUTF8().getAddress());
#endif
    }
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
    
    if (in_count() > 1)
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
    dirFiles = dirToSearch.findChildFiles (typeOfFiles, false, fileExt);
    out.init(csound, (int)dirFiles.size());
    
    for ( int i = 0 ; i < dirFiles.size() ; i++)
    {
        out[i].size = dirFiles[i].getFullPathName().length()+1;
        out[i].data = csound->strdup(dirFiles[i].getFullPathName().toUTF8().getAddress());
    }
    return OK;
}

int CabbageFindFilesK::findFiles()
{
    if( inargs[0] == 1 )
    {
        if (in_count() < 1)
        {
            csound->message("Not enough parameters passed to cabbageFindFiles.\n");
            return NOTOK;
        }
        
        String fileExt = "*";
        File::TypesOfFileToFind typeOfFiles = File::TypesOfFileToFind::findFiles;
        csnd::Vector<STRINGDAT>& out = outargs.vector_data<STRINGDAT>(0);
        
        if (in_count() == 4)
            fileExt = String(inargs.str_data(3).data);
        
        if (in_count() > 2)
        {
            const String types = String(inargs.str_data(2).data);
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
        File dirToSearch = File::getCurrentWorkingDirectory().getChildFile(String(inargs.str_data(1).data));
        dirFiles = dirToSearch.findChildFiles (typeOfFiles, false, fileExt);

        out.init(csound, (int)dirFiles.size());
        
        for ( int i = 0 ; i < dirFiles.size() ; i++)
        {
            out[i].size = dirFiles[i].getFullPathName().length()+1;
            out[i].data = csound->strdup(dirFiles[i].getFullPathName().toUTF8().getAddress());
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
    
    if(!File(newLocation).exists())
    {
        ghc::filesystem::create_directory(newFolder.toStdString());
    }
    else
    {
        newFolder = newLocation;
        folderAlreadyExists = true;
    }
    
    
    for ( int i = 1 ; i < int(in_count()) ; i++)
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
    
    if(!folderAlreadyExists)
    {
        ghc::filesystem::rename(newFolder.toStdString(), newLocation.toStdString());
    }
    return OK;
}

//====================================================================================================
int GetCabbageReservedChannelString::getAttribute()
{

    if (csound->get_csound()->GetChannelPtr(csound->get_csound(), &value, inargs.str_data(0).data,
                                            CSOUND_STRING_CHANNEL | CSOUND_OUTPUT_CHANNEL) == CSOUND_SUCCESS)
    {
        outargs.str_data(0).size = ((STRINGDAT*)value)->size;
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
