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
        
        
        for( int i = 0 ; i < varData->data.getNumChildren() ; i++){
            if(varData->data.getChild(i).getType() == Identifier(cabbageWidget))
                return varData->data.getChild(i);
        }
        
        return ValueTree("null");
    }
};

struct CabbageWebSetScalar : csnd::InPlug<3>
{
    ValueTree valueTree;
    CabbageWidgetsValueTree** vt = nullptr;
    CabbageWidgetsValueTree* varData;
    int deinit(){
        varData = nullptr;
        vt = nullptr;
        return OK;
    }
    int init(){        return sendScalarToWebUI(true);    }
    int kperf(){       return sendScalarToWebUI(false);   }
    int sendScalarToWebUI(bool init);
};

struct CabbageWebSetArray : csnd::InPlug<3>
{
    ValueTree valueTree;
//    int init(){        return sendToWebUI(true);    }
//    int kperf(){       return sendToWebUI(false);   }
//    int sendArrayToWebUI(bool init);
};
