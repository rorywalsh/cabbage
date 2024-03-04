#ifndef WEBSOCKET_IO_COMMON_H
#define WEBSOCKET_IO_COMMON_H

#include "csdl.h"

#include <libwebsockets.h>
#include <stdbool.h>


const int LibWebsocketLogLevel;

const char *SharedWebsocketDataGlobalVariableName;
const int WebsocketBufferCount;
const int WebsocketInitialMessageSize;

enum {
    StringType = 1,
    FloatArrayType = 2
};

typedef struct PortKey
{
    MYFLT port;
    int nullTerminator;
} PortKey;

typedef struct WebsocketMessage {
    size_t size;
    char *buffer;
} WebsocketMessage;

typedef struct WebsocketPathData {
    void *messageMutex;
    void *previousMessageMutex;
    WebsocketMessage message;
    WebsocketMessage previousMessage;
    bool sent;
} WebsocketPathData;

typedef struct Websocket {
    CSOUND *csound;
    CS_HASH_TABLE *pathGetFloatsHashTable; // key = path string, value = WebsocketPathData containing a MYFLT array.
    CS_HASH_TABLE *pathGetStringHashTable; // key = path string, value = WebsocketPathData containing a string
    CS_HASH_TABLE *pathSetFloatsHashTable; // key = path string, value = WebsocketPathData containing a MYFLT array.
    CS_HASH_TABLE *pathSetStringHashTable; // key = path string, value = WebsocketPathData containing a string
    int refCount;
    struct lws_context *context;
    struct lws_protocols *protocols;
    struct lws_context_creation_info info;
    char *receiveBuffer;
    int receiveBufferSize;
    int receiveBufferIndex;
    void *processThread;
    bool isRunning;
} Websocket;

typedef struct {
    CS_HASH_TABLE *portWebsocketHashTable; // key = port float as string, value = Websocket
} SharedWebsocketData;

typedef struct {
    PortKey portKey;
    CSOUND *csound;
    Websocket *websocket;
} WS_common;

void initPlugin();
void initPortKey(PortKey *portKey, MYFLT port);

Websocket *getWebsocket(CSOUND *csound, int port, WS_common *p);

WebsocketPathData *getWebsocketPathData(CSOUND *csound, CS_HASH_TABLE *pathHashTable, char *path);

int32_t noop_perf(CSOUND *csound, void *p);

#endif
