/*
  ==============================================================================

    cabbageIdentifierOpcodes.h
    Created: 12 Mar 2021 12:49:09pm
    Author:  walshr

  ==============================================================================
*/

#include <plugin.h>
#include "../CabbageCommonHeaders.h"
#include "../Widgets/CabbageWidgetData.h"
#include "../JuceLibraryCode/JuceHeader.h"

#pragma once


class CabbageWidgetsValueTree
{
public:
   CabbageWidgetsValueTree(){}
   ~CabbageWidgetsValueTree(){}

   ValueTree data;
};

class CabbageWidgetIdentifiers
{
public:
    struct IdentifierData
    {
        Identifier identifier, name;
        bool isSingleIdent = true;
        var args;
    };
    
    CabbageWidgetIdentifiers(){
        data.clear();
    }
    Array<IdentifierData, CriticalSection> data;
};

//Some of these classes use a reversed input/output system to get around the issue of 0 outputs..
struct SetCabbageValueIdentifier : csnd::Plugin<3, 0>
{
    MYFLT* value;
    CabbageWidgetIdentifiers** vt = nullptr;
    int init(){ return setAttribute(); };
    int kperf(){ return setAttribute(); }
    int setAttribute();
};

struct SetCabbageIdentifierSArgs : csnd::Plugin<64, 0>
{
    CabbageWidgetIdentifiers** vt = nullptr;
    int init(){ return setAttribute(); };
    int kperf(){ return setAttribute(); };
    int setAttribute();
};

struct SetCabbageIdentifier : csnd::Plugin<64, 0>
{
    CabbageWidgetIdentifiers** vt = nullptr;
    int init(){ return setAttribute(); };
    int kperf(){ return setAttribute(); };
    int setAttribute();
};

struct SetCabbageIdentifierITime : csnd::Plugin<64, 0>
{
    CabbageWidgetIdentifiers** vt = nullptr;
    int init(){ return setAttribute(); };
    int setAttribute();
};

struct SetCabbageIdentifierITimeSArgs : csnd::Plugin<64, 0>
{
    CabbageWidgetIdentifiers** vt = nullptr;
    int init(){ return setAttribute(); };
    int setAttribute();
};
//================================================================
struct GetCabbageValue : csnd::Plugin<1, 1>
{
    MYFLT* value;
    int init(){ return getAttribute(); };
    int kperf(){ return getAttribute(); };
    int getAttribute();
};

struct GetCabbageStringValue : csnd::Plugin<1, 1>
{
    MYFLT* value;
    int init(){ return getAttribute(); };
    int kperf(){ return getAttribute(); };
    int getAttribute();
};

struct GetCabbageValueWithTrigger : csnd::Plugin<2, 1>
{
    MYFLT* value;
    MYFLT currentValue;
    int init(){ return getAttribute(); };
    int kperf(){ return getAttribute(); };
    int getAttribute();
};

struct GetCabbageStringValueWithTrigger : csnd::Plugin<2, 1>
{
    MYFLT* value;
    char* currentString;
    int init(){ return getAttribute(); };
    int kperf(){ return getAttribute(); };
    int getAttribute();
};

//================================================================================================================
struct CreateCabbageWidget : csnd::Plugin<1, 0>
{
    MYFLT* value;
    CabbageWidgetsValueTree** vt = nullptr;
    int init(){ return createWidget(); };
    int createWidget();
};
//================================================================================================================
struct GetCabbageIdentifierSingle : csnd::Plugin<1, 2>
{
    MYFLT* value;
    CabbageWidgetsValueTree** vt = nullptr;
    int init(){ return getAttribute();  };
    int kperf(){ return getAttribute();  };
    int getAttribute();
};

struct GetCabbageIdentifierArray : csnd::Plugin<1, 2>
{
    MYFLT* value;
    CabbageWidgetsValueTree** vt = nullptr;
    int init(){ return getAttribute();  };
    int kperf(){ return getAttribute();  };
    int getAttribute();
};

struct GetCabbageStringIdentifierSingle : csnd::Plugin<1, 2>
{
    MYFLT* value;
    CabbageWidgetsValueTree** vt = nullptr;
    int init(){ return getAttribute(); };
    int kperf(){ return getAttribute(); };
    int getAttribute();
};

struct GetCabbageStringIdentifierArray : csnd::Plugin<1, 2>
{
    MYFLT* value;
    CabbageWidgetsValueTree** vt = nullptr;
    int init(){ return getAttribute(); };
    int kperf(){ return getAttribute(); };
    int getAttribute();
};

//================================================================================================================
// Opcodes to get reserved channel data with optional trigger signal output
struct GetCabbageReservedChannelStringWithTrigger : csnd::Plugin<2, 1>
{
    MYFLT *value;
    char* channelString;
    int init(){ return getAttribute(); };
    int kperf(){ return getAttribute();};
    int deinit(){ free(channelString);  return OK;  }
    int getAttribute();
};

struct GetCabbageReservedChannelDataWithTrigger : csnd::Plugin<2, 1>
{
    MYFLT *value;
    MYFLT currentValue;
    int init(){ return getAttribute(); };
    int kperf(){ return getAttribute(); };
    int deinit(){ return OK;  }
    int getAttribute();
};

//================================================================================================================
// Opcodes to get reserved channel data
struct GetCabbageReservedChannelString : csnd::Plugin<1, 1>
{
    MYFLT *value;
    char* channelString;
    int init(){ return getAttribute(); };
    int kperf(){ return getAttribute(); };
    int deinit(){ free(channelString);  return OK;  }
    int getAttribute();
};

struct GetCabbageReservedChannelData : csnd::Plugin<1, 1>
{
    MYFLT *value;
    MYFLT* currentValue;
    int init(){ return getAttribute(); };
    int kperf(){ return getAttribute(); };
    int deinit(){ return OK;  }
    int getAttribute();
};
