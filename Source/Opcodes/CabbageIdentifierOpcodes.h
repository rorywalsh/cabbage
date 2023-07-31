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
#include "JuceHeader.h"

#pragma once

#define I_RATE 1
#define K_RATE 2

class CabbageWidgetsValueTree
{
public:
   CabbageWidgetsValueTree()= default;
   ~CabbageWidgetsValueTree(){}

   ValueTree data;
};

class CabbageWidgetIdentifiers
{
public:
    struct IdentifierData
    {
        Identifier identifier, name;
        bool identWithArgument = false;
        var args;
        bool isValid = false;
        int methodCode = 0;
    };
    
    CabbageWidgetIdentifiers(){
        data.clear();
    }
    Array<IdentifierData> data;

    std::atomic_bool canRead;
};

//Some of these classes use a reversed input/output system to get around the issue of 0 outputs..
struct SetCabbageValueIdentifier : csnd::InPlug<3>
{
    MYFLT* value;
    MYFLT lastValue = 0;
    String name, identifier;
    CabbageWidgetIdentifiers** vt = nullptr;
    int init(){ return setAttribute(I_RATE); }
    int kperf(){ return setAttribute(K_RATE); }
    int setAttribute(int rate);
};

struct SetCabbageValueIdentifierITime : csnd::InPlug<3>
{
    MYFLT* value;
    MYFLT lastValue = 0;
    String name, identifier;
    CabbageWidgetIdentifiers** vt = nullptr;
    int init(){ return setAttribute(I_RATE); }
    int kperf(){ return setAttribute(K_RATE); }
    int setAttribute(int rate);
};

struct SetCabbageValueIdentifierSArgs : csnd::InPlug<3>
{
    MYFLT* str;
    String name, identifier;
    CabbageWidgetIdentifiers** vt = nullptr;
    int init(){ return setAttribute(I_RATE); }
    int kperf(){ return setAttribute(K_RATE); }
    int setAttribute(int rate);
};

struct SetCabbageValueIdentifierSArgsITime : csnd::InPlug<3>
{
    MYFLT* strInput;
    String name, identifier;
    CabbageWidgetIdentifiers** vt = nullptr;
    int init(){ return setAttribute(I_RATE); }
    int setAttribute(int rate);
};

struct SetCabbageIdentifierSArgs : csnd::InPlug<64>
{
    MYFLT* value;
    CabbageWidgetIdentifiers** vt = nullptr;
    int init(){ return setAttribute(I_RATE); }
    int kperf(){ return setAttribute(K_RATE); }
    int setAttribute(int rate);
};

struct SetCabbageIdentifier : csnd::InPlug<64>
{
    MYFLT* value;
    CabbageWidgetIdentifiers** vt = nullptr;
    int init(){ return setAttribute(); }
    int kperf(){ return setAttribute(); }
    int setAttribute();
};

struct SetCabbageIdentifierArray : csnd::InPlug<64>
{
    MYFLT* value;
    CabbageWidgetIdentifiers** vt = nullptr;
    int init(){ return setAttribute(); }
    int kperf(){ return setAttribute(); }
    int setAttribute();
};

struct SetCabbageIdentifierITime : csnd::Plugin<64, 0>
{
    MYFLT* value;
    //String name, identifier;
    CabbageWidgetIdentifiers** vt = nullptr;
    int init(){ return setAttribute(); }
    int setAttribute();
};

struct SetCabbageIdentifierITimeSArgs : csnd::Plugin<64, 0>
{
    //String name, identifier;
    MYFLT* value;
    CabbageWidgetIdentifiers** vt = nullptr;
    int init(){ return setAttribute(); }
    int setAttribute();
};
//================================================================
struct GetCabbageValue : csnd::Plugin<1, 1>
{
    MYFLT* value;
    int init(){ return getAttribute(); }
    int kperf(){ return getAttribute(); }
    int getAttribute();
};

struct GetCabbageValueArray : csnd::Plugin<1, 1>
{
    MYFLT* value;
    int init(){ return getAttribute(); }
    int kperf(){ return getAttribute(); }
    int getAttribute();
};

struct GetCabbageStringValue : csnd::Plugin<1, 1>
{
    MYFLT* value;
    char* currentString;
    int init(){ 
        currentString = "";
        return getAttribute(true); 
    }
    int kperf(){ return getAttribute(false); }
    int getAttribute(bool init);
};

struct GetCabbageStringValueArray : csnd::Plugin<1, 1>
{
    MYFLT* value;
    int init(){ return getAttribute(); }
    int kperf(){ return getAttribute(); }
    int getAttribute();
};

struct GetCabbageStringValueWithTrigger : csnd::Plugin<2, 2>
{
    MYFLT* value;
    char* currentString;
    int kCycleCount = 0;
    int init(){ return getAttribute(true); }
    int kperf(){ return getAttribute(false); }
    int getAttribute(bool init);
};

struct CabbageValueChanged : csnd::Plugin<2, 3>
{
    MYFLT* value;
    int mode = 2;
    MYFLT oldValue[1024] = {0};
    std::vector<STRINGDAT> currentStrings;
    int init(){
        csnd::Vector<STRINGDAT>& inputArgs = inargs.vector_data<STRINGDAT>(0);
        currentStrings.resize(inputArgs.len());
        return OK;
    }
    int kperf(){ return getAttribute(); }
    int deinit(){  currentStrings.clear(); return OK; }
    int getAttribute();
};

struct CabbageValueChangedIndex : csnd::Plugin<2, 3>
{
    MYFLT* value;
    int mode = 2;
    MYFLT oldValue[1024] ={0};
    std::vector<STRINGDAT> currentStrings;
    int init(){
        csnd::Vector<STRINGDAT>& inputArgs = inargs.vector_data<STRINGDAT>(0);
        currentStrings.resize(inputArgs.len());
        return OK;
    }
    int kperf(){ return getAttribute(); }
    int deinit(){  currentStrings.clear(); return OK; }
    int getAttribute();
};

struct GetCabbageStringValueArrayWithTrigger : csnd::Plugin<2, 1>
{
    MYFLT* value;
    std::vector<STRINGDAT> currentStrings;
    int init(){
        csnd::Vector<STRINGDAT>& inputArgs = inargs.vector_data<STRINGDAT>(0);
        currentStrings.resize(inputArgs.len());
        return OK;
    }
    int kperf(){ return getAttribute(); }
    int deinit(){  currentStrings.clear(); return OK; }
    int getAttribute();
};

struct GetCabbageValueWithTrigger : csnd::Plugin<2, 1>
{
    MYFLT* value;
    MYFLT currentValue = 0;
    bool firstRun = true;
    int init(){
        firstRun = true;
        return getAttribute();        
    }
    int kperf(){ return getAttribute(); }
    int getAttribute();
};

struct GetCabbageValueArrayWithTrigger : csnd::Plugin<2, 1>
{
    MYFLT* value;
    MYFLT currentValue[1024] ={0};
    bool firstRun;
    int init(){
        firstRun = true;
        return getAttribute();
    }
    int kperf(){ return getAttribute(); }
    int getAttribute();
};



//================================================================================================================
struct CreateCabbageWidget : csnd::InPlug<2>
{
    MYFLT* value;
    CabbageWidgetsValueTree** vt = nullptr;
    int init(){ return createWidget(); }
    int createWidget();
};
//================================================================================================================
struct GetCabbageIdentifierSingle : csnd::Plugin<1, 2>
{
    MYFLT* value;
    CabbageWidgetsValueTree** vt = nullptr;
    int init(){ return getAttribute();  }
    int kperf(){ return getAttribute();  }
    int getAttribute();
};

struct GetCabbageIdentifierSingleITime : csnd::Plugin<1, 2>
{
    MYFLT* value;
    CabbageWidgetsValueTree** vt = nullptr;
    int init(){ return getAttribute();  }
    int getAttribute();
};

struct GetCabbageIdentifierArray : csnd::Plugin<1, 2>
{
    MYFLT* value;
    CabbageWidgetsValueTree** vt = nullptr;
    int init(){ return getAttribute();  }
    int kperf(){ return getAttribute();  }
    int getAttribute();
};

struct GetCabbageStringIdentifierSingle : csnd::Plugin<1, 2>
{
    MYFLT* value;
    CabbageWidgetsValueTree** vt = nullptr;
    int init(){ return getAttribute(); }
    int kperf(){ return getAttribute(); }
    int getAttribute();
};

struct GetCabbageStringIdentifierArray : csnd::Plugin<1, 2>
{
    MYFLT* value;
    CabbageWidgetsValueTree** vt = nullptr;
    int init(){ return getAttribute(); }
    int kperf(){ return getAttribute(); }
    int getAttribute();
};

struct CabbageGetWidgetChannels : csnd::Plugin<1, 1>
{
    CabbageWidgetsValueTree** vt = nullptr;
    int init() { return getChannels(); }
    int kperf() { return getChannels(); }
    int getChannels();
};

//================================================================================================================
// Opcodes to get reserved channel data with optional trigger signal output
struct GetCabbageReservedChannelStringWithTrigger : csnd::Plugin<2, 1>
{
    MYFLT *value;
    char* channelString;
    int init(){ return getAttribute(); }
    int kperf(){ return getAttribute();}
    int deinit(){ free(channelString);  return OK;  }
    int getAttribute();
};

struct GetCabbageReservedChannelDataWithTrigger : csnd::Plugin<2, 1>
{
    MYFLT *value;
    MYFLT currentValue;
    int init(){ return getAttribute(); }
    int kperf(){ return getAttribute(); }
    int deinit(){ return OK;  }
    int getAttribute();
};

//================================================================================================================
// Opcodes to get reserved channel data
struct GetCabbageReservedChannelString : csnd::Plugin<1, 1>
{
    MYFLT *value;
    char* channelString;
    int init(){ return getAttribute(); }
    int kperf(){ return getAttribute(); }
    int deinit(){ free(channelString);  return OK;  }
    int getAttribute();
};

struct GetCabbageReservedChannelData : csnd::Plugin<1, 1>
{
    MYFLT *value;
    MYFLT* currentValue;
    int init(){ return getAttribute(); }
    int kperf(){ return getAttribute(); }
    int deinit(){ return OK;  }
    int getAttribute();
};

int getFileInfo(csnd::Plugin<1,1>* opcodeData, String type, std::string& currentPath);
struct CabbageGetFilename : csnd::Plugin<1, 1>
{
    std::string currentPath = "";
    int init(){ return getFileInfo(this, "name", currentPath); }
    int kperf(){ return getFileInfo(this, "name", currentPath); }
};
struct CabbageGetFileExtension : csnd::Plugin<1, 1>
{
    std::string currentPath = "";
    int init(){ return getFileInfo(this, "extension", currentPath); }
    int kperf(){ return getFileInfo(this, "extension", currentPath); }
};
struct CabbageGetFileNoExtension : csnd::Plugin<1, 1>
{
    std::string currentPath = "";
    int init(){ return getFileInfo(this, "noExtension", currentPath); }
    int kperf(){ return getFileInfo(this, "noExtension", currentPath); }
};
struct CabbageGetFilePath : csnd::Plugin<1, 1>
{
    std::string currentPath = "";
    int init(){ return getFileInfo(this, "path", currentPath); }
    int kperf(){ return getFileInfo(this, "path", currentPath); }
};

struct CabbageFindFilesI : csnd::Plugin<1, 4>
{
    int init(){ return findFiles(); }
    int kperf(){ return findFiles(); }
    int findFiles();
};

struct CabbageFindFilesK : csnd::Plugin<1, 4>
{
    int init(){ return findFiles(); }
    int kperf(){ return findFiles(); }
    int deinit(){
        return OK;
    }
    int findFiles();
};

struct CabbageCopyFile : csnd::InPlug<64>
{
    int init(){ return copyFiles(); }
    int kperf(){ return copyFiles(); }
    int deinit(){ return OK;  }
    int copyFiles();
};


