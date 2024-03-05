#include "get.h"
#include "set.h"

//static OENTRY localops[] = {
//    {
//        .opname = "websocket_getArray_i",
//        .dsblksiz = sizeof(WS_get),
//        .thread = 3,
//        .outypes = "i[]",
//        .intypes = "cS",
//        .iopadr = (SUBR) websocket_get_init,
//        .kopadr = noop_perf,
//        .aopadr = NULL
//    },
//    {
//        .opname = "websocket_getArray_i",
//        .dsblksiz = sizeof(WS_get),
//        .thread = 3,
//        .outypes = "i[]",
//        .intypes = "iS",
//        .iopadr = (SUBR) websocket_get_init,
//        .kopadr = noop_perf,
//        .aopadr = NULL
//    },
//    {
//        .opname = "websocket_getArray_k",
//        .dsblksiz = sizeof(WS_get),
//        .thread = 3,
//        .outypes = "k[]",
//        .intypes = "cS",
//        .iopadr = (SUBR) websocket_get_init,
//        .kopadr = (SUBR) websocket_getArray_perf,
//        .aopadr = NULL
//    },
//    {
//        .opname = "websocket_getArray_k",
//        .dsblksiz = sizeof(WS_get),
//        .thread = 3,
//        .outypes = "k[]",
//        .intypes = "iS",
//        .iopadr = (SUBR) websocket_get_init,
//        .kopadr = (SUBR) websocket_getArray_perf,
//        .aopadr = NULL
//    },
//    {
//        .opname = "websocket_getString_i",
//        .dsblksiz = sizeof(WS_get),
//        .thread = 3,
//        .outypes = "S",
//        .intypes = "cS",
//        .iopadr = (SUBR) websocket_get_init,
//        .kopadr = noop_perf,
//        .aopadr = NULL
//    },
//    {
//        .opname = "websocket_getString_i",
//        .dsblksiz = sizeof(WS_get),
//        .thread = 3,
//        .outypes = "S",
//        .intypes = "iS",
//        .iopadr = (SUBR) websocket_get_init,
//        .kopadr = noop_perf,
//        .aopadr = NULL
//    },
//    {
//        .opname = "websocket_getString_k",
//        .dsblksiz = sizeof(WS_get),
//        .thread = 3,
//        .outypes = "S",
//        .intypes = "cS",
//        .iopadr = (SUBR) websocket_get_init,
//        .kopadr = (SUBR) websocket_getString_perf,
//        .aopadr = NULL
//    },
//    {
//        .opname = "websocket_getString_k",
//        .dsblksiz = sizeof(WS_get),
//        .thread = 3,
//        .outypes = "S",
//        .intypes = "iS",
//        .iopadr = (SUBR) websocket_get_init,
//        .kopadr = (SUBR) websocket_getString_perf,
//        .aopadr = NULL
//    },
//    {
//        .opname = "websocket_set_i",
//        .dsblksiz = sizeof(WS_set),
//        .thread = 3,
//        .outypes = "",
//        .intypes = "cSi[]",
//        .iopadr = (SUBR) websocket_set_init,
//        .kopadr = noop_perf,
//        .aopadr = NULL
//    },
//    {
//        .opname = "websocket_set_i",
//        .dsblksiz = sizeof(WS_set),
//        .thread = 3,
//        .outypes = "",
//        .intypes = "iSi[]",
//        .iopadr = (SUBR) websocket_set_init,
//        .kopadr = noop_perf,
//        .aopadr = NULL
//    },
//    {
//        .opname = "websocket_set_i",
//        .dsblksiz = sizeof(WS_set),
//        .thread = 3,
//        .outypes = "",
//        .intypes = "cSS",
//        .iopadr = (SUBR) websocket_set_init,
//        .kopadr = noop_perf,
//        .aopadr = NULL
//    },
//    {
//        .opname = "websocket_set_i",
//        .dsblksiz = sizeof(WS_set),
//        .thread = 3,
//        .outypes = "",
//        .intypes = "iSS",
//        .iopadr = (SUBR) websocket_set_init,
//        .kopadr = noop_perf,
//        .aopadr = NULL
//    },
//    {
//        .opname = "websocket_set_k",
//        .dsblksiz = sizeof(WS_set),
//        .thread = 3,
//        .outypes = "",
//        .intypes = "cSk[]",
//        .iopadr = (SUBR) websocket_set_init,
//        .kopadr = (SUBR) websocket_setArray_perf,
//        .aopadr = NULL
//    },
//    {
//        .opname = "websocket_set_k",
//        .dsblksiz = sizeof(WS_set),
//        .thread = 3,
//        .outypes = "",
//        .intypes = "iSk[]",
//        .iopadr = (SUBR) websocket_set_init,
//        .kopadr = (SUBR) websocket_setArray_perf,
//        .aopadr = NULL
//    },
//    {
//        .opname = "websocket_set_k",
//        .dsblksiz = sizeof(WS_set),
//        .thread = 3,
//        .outypes = "",
//        .intypes = "cSS",
//        .iopadr = (SUBR) websocket_set_init,
//        .kopadr = (SUBR) websocket_setString_perf,
//        .aopadr = NULL
//    },
//    {
//        .opname = "websocket_set_k",
//        .dsblksiz = sizeof(WS_set),
//        .thread = 3,
//        .outypes = "",
//        .intypes = "iSS",
//        .iopadr = (SUBR) websocket_set_init,
//        .kopadr = (SUBR) websocket_setString_perf,
//        .aopadr = NULL
//    },
//};
//
//LINKAGE
