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
    
    CabbageWidgetIdentifiers(){}
    Array<IdentifierData, CriticalSection> data;
};

//Some of these classes use a revered input/output system to get around the issue of 0 outputs..

struct SetCabbageValueIdentifier : csnd::Plugin<3, 0>
{
    CabbageWidgetIdentifiers** vt = nullptr;
    int init(){ setAttribute(); return OK;};
    int kperf(){ setAttribute(); return OK;}
    int setAttribute();
};

struct SetCabbageIdentifier : csnd::Plugin<64, 0>
{
    CabbageWidgetIdentifiers** vt = nullptr;
    int init(){ setAttribute(); return OK;};
    int kperf(){ setAttribute(); return OK;};
    int setAttribute();
};

struct GetCabbageValueIdentifier : csnd::Plugin<1, 1>
{
    MYFLT* value;
    CabbageWidgetIdentifiers** vt = nullptr;
    int init(){ getAttribute(); return OK;};
    int kperf(){ getAttribute(); return OK;};
    int getAttribute();
};

struct GetCabbageStringValueIdentifier : csnd::Plugin<1, 1>
{
    MYFLT* value;
    CabbageWidgetIdentifiers** vt = nullptr;
    int init(){ getAttribute(); return OK;};
    int kperf(){ getAttribute(); return OK;};
    int getAttribute();
};

struct GetCabbageIdentifierSingle : csnd::Plugin<1, 2>
{
    MYFLT* value;
    CabbageWidgetsValueTree** vt = nullptr;
    int init(){ getAttribute(); return OK;};
    int kperf(){ getAttribute(); return OK;};
    int getAttribute();
};

struct GetCabbageStringIdentifierSingle : csnd::Plugin<1, 2>
{
    MYFLT* value;
    CabbageWidgetsValueTree** vt = nullptr;
    int init(){ getAttribute(); return OK;};
    int kperf(){ getAttribute(); return OK;};
    int getAttribute();
};

    
