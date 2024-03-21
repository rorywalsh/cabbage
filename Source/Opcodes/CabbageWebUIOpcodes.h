#pragma once


#include "../CabbageCommonHeaders.h"
#include "../Widgets/CabbageWidgetData.h"
#undef _CR
#include <plugin.h>

#include "JuceHeader.h"
#include "CabbageIdentifierOpcodes.h"

struct CabbageWebOpcodes
{
    
    static ValueTree assignValueTree(CabbageWidgetsValueTree** vt, csnd::Csound* csound, String cabbageWidget)
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
        

        return varData->data.getChildWithName(Identifier(cabbageWidget));

        //for( int i = 0 ; i < varData->data.getNumChildren() ; i++){
        //    if(varData->data.getChild(i).getType() == Identifier(cabbageWidget))
        //        return varData->data.getChild(i);
        //}
        
        return ValueTree("null");
    }
};

//===============================================================================
struct CabbageWebSendScalar : csnd::InPlug<4>
{
    CabbageWidgetsValueTree** vt = nullptr;
    CabbageWidgetsValueTree* varData;
    int trigger = 0;
    
    int deinit(){
        varData = nullptr;
        vt = nullptr;
        return OK;
    }
    int init(){        return sendScalarToWebUI(true);    }
    int kperf(){       return sendScalarToWebUI(false);   }
    int sendScalarToWebUI(bool init);
};

struct CabbageWebSendArray : csnd::InPlug<4>
{
    CabbageWidgetsValueTree** vt = nullptr;
    CabbageWidgetsValueTree* varData;
    int trigger = 0;
    
    int deinit(){
        varData = nullptr;
        vt = nullptr;
        return OK;
    }
    int init(){        return sendArrayToWebUI(true);    }
    int kperf(){       return sendArrayToWebUI(false);   }
    int sendArrayToWebUI(bool init);
};

struct CabbageWebSendASig : csnd::InPlug<4>
{
    CabbageWidgetsValueTree** vt = nullptr;
    CabbageWidgetsValueTree* varData;
    int trigger = 0;
    
    int deinit(){
        varData = nullptr;
        vt = nullptr;
        return OK;
    }
    int init(){        return sendASigToWebUI(true);    }
    int aperf(){       return sendASigToWebUI(false);   }
    int sendASigToWebUI(bool init);
};

struct CabbageWebSendString : csnd::InPlug<4>
{
    CabbageWidgetsValueTree** vt = nullptr;
    CabbageWidgetsValueTree* varData;
    int trigger = 0;
    
    int deinit(){
        varData = nullptr;
        vt = nullptr;
        return OK;
    }
    int init(){        return sendStringToWebUI(true);    }
    int kperf(){       return sendStringToWebUI(false);   }
    int sendStringToWebUI(bool init);
};

struct CabbageWebSendTable : csnd::InPlug<4>
{
    csnd::Table table;
    CabbageWidgetsValueTree** vt = nullptr;
    CabbageWidgetsValueTree* varData;
    int trigger = 0;
    
    int deinit(){
        varData = nullptr;
        vt = nullptr;
        return OK;
    }
    int init(){        return sendTableToWebUI(true);    }
    int kperf(){       return sendTableToWebUI(false);   }
    int sendTableToWebUI(bool init);
};

//======================================================================================

