#ifndef WEBSOCKET_IO_SET_H
#define WEBSOCKET_IO_SET_H
#if WebUI

#include "common.h"

typedef struct WS_set
{
    OPDS h;
    MYFLT *port;
    STRINGDAT *path;
    void *input; // ARRAYDAT * or STRINGDAT*
    WS_common common;
    char *msgPre; // path + '\0 + data type + data { (uint32 array length + array values) or (string) }.
    size_t msgPreSize;
} WS_set;

int32_t websocket_set_init(CSOUND *csound, WS_set *p);

int32_t websocket_setArray_perf(CSOUND *csound, WS_set *p);
int32_t websocket_setString_perf(CSOUND *csound, WS_set *p);

#endif
#endif
