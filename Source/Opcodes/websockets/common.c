#include "common.h"

const int LibWebsocketLogLevel = LLL_ERR;

const char *SharedWebsocketDataGlobalVariableName = "SharedWebsocketData";
const int WebsocketBufferCount = 2;
const int WebsocketInitialMessageSize = 1024;

static bool isPluginInitialized = false;

extern int32_t onWebsocketReceive(struct lws *websocket, void *inputData, size_t inputDataSize);
extern int32_t onWebsocketWritable(struct lws *websocket);

static void destroyWebsocketPathHashTable(CSOUND *csound, CS_HASH_TABLE *pathHashTable)
{
    CONS_CELL *pathItem = csound->GetHashTableValues(csound, pathHashTable);
    while (pathItem) {
        WebsocketPathData *pathData = pathItem->value;

        // NB: free is used instead of csound->Free because csound->Free crashes after the buffer is given to lws_write.
        free(pathData->message.buffer);
        pathData->message.size = 0;

        csound->Free(csound, pathData->previousMessage.buffer);
        pathData->previousMessage.size = 0;

        csound->DestroyMutex(pathData->previousMessageMutex);
        csound->DestroyMutex(pathData->messageMutex);
        csound->Free(csound, pathData);

        pathItem = pathItem->next;
    }

    csound->DestroyHashTable(csound, pathHashTable);
}

static void releaseWebsocket(CSOUND *csound, Websocket *ws)
{
    while (!ws->isRunning) {
        csound->Sleep(0);
    }

    ws->isRunning = false;
    lws_cancel_service(ws->context);

    csound->JoinThread(ws->processThread);

    lws_context_destroy(ws->context);

    destroyWebsocketPathHashTable(csound, ws->pathSetStringHashTable);
    destroyWebsocketPathHashTable(csound, ws->pathSetFloatsHashTable);
    destroyWebsocketPathHashTable(csound, ws->pathGetStringHashTable);
    destroyWebsocketPathHashTable(csound, ws->pathGetFloatsHashTable);

    csound->Free(csound, ws->receiveBuffer);
    csound->Free(csound, ws->protocols);
    csound->Free(csound, ws);
}

static int32_t resetSharedData(CSOUND *csound, void *vshared)
{
    SharedWebsocketData *shared = vshared;

    // Destroy all websockets.
    CONS_CELL *websocketItem = csound->GetHashTableValues(csound, shared->portWebsocketHashTable);
    while (websocketItem) {
        Websocket *ws = websocketItem->value;
        releaseWebsocket(csound, ws);
        websocketItem = websocketItem->next;
    }

    csound->DestroyHashTable(csound, shared->portWebsocketHashTable);
    csound->DestroyGlobalVariable(csound, SharedWebsocketDataGlobalVariableName);
    return OK;
}

static SharedWebsocketData *getSharedData(CSOUND *csound)
{
    SharedWebsocketData *shared = csound->QueryGlobalVariable(csound, SharedWebsocketDataGlobalVariableName);
    if (shared) {
        return shared;
    }
    csound->CreateGlobalVariable(csound, SharedWebsocketDataGlobalVariableName, sizeof(SharedWebsocketData));
    shared = csound->QueryGlobalVariable(csound, SharedWebsocketDataGlobalVariableName);
    if (!shared) {
        csound->ErrorMsg(csound, "Websocket: failed to allocate globals");
    }
    shared->portWebsocketHashTable = csound->CreateHashTable(csound);
    csound->RegisterResetCallback(csound, shared, resetSharedData);
    return shared;
}

static WebsocketPathData *createWebsocketPathData(CSOUND *csound)
{
    WebsocketPathData *pathData = csound->Calloc(csound, sizeof(WebsocketPathData));
    pathData->messageMutex = csound->Create_Mutex(false);
    pathData->previousMessageMutex = csound->Create_Mutex(false);
    return pathData;
}

static int32_t callback(
    struct lws *websocket,
    enum lws_callback_reasons reason,
    void *user,
    void *inputData,
    size_t inputDataSize)
{
    IGN(user);

    switch (reason) {
    case LWS_CALLBACK_RECEIVE:
        return onWebsocketReceive(websocket, inputData, inputDataSize);
    case LWS_CALLBACK_SERVER_WRITEABLE:
        return onWebsocketWritable(websocket);
    default:
        break;
    }

    return OK;
}

static uintptr_t processThread(void *vws)
{
    Websocket *ws = vws;
    ws->isRunning = true;

    while (ws->isRunning) {
        lws_service(ws->context, 0);
        lws_callback_on_writable_all_protocol(ws->context, ws->protocols);
    }

    return 0;
}

void initPlugin()
{
    if (isPluginInitialized) {
        return;
    }
    lws_set_log_level(LibWebsocketLogLevel, NULL);
    isPluginInitialized = true;
}

void initPortKey(PortKey *portKey, MYFLT port)
{
    memset(portKey, 0, sizeof(PortKey));

    portKey->port = port;

    char *s = (char*)(&portKey->port);
    for (size_t i = 0; i < sizeof(MYFLT); i++) {
        if (s[i] == 0) {
            s[i] = '~';
        }
    }
}

Websocket *getWebsocket(CSOUND *csound, int port, WS_common *p)
{
    Websocket *ws = NULL;

    SharedWebsocketData *shared = getSharedData(csound);

    char *hashTablePortKey = csound->GetHashTableKey(csound, shared->portWebsocketHashTable, (char*) &p->portKey);
    if (hashTablePortKey) {
        ws = csound->GetHashTableValue(csound, shared->portWebsocketHashTable, hashTablePortKey);
        ws->refCount++;
        return ws;
    }

    ws = csound->Calloc(csound, sizeof(Websocket));
    ws->csound = csound;
    ws->pathGetFloatsHashTable = csound->CreateHashTable(csound);
    ws->pathGetStringHashTable = csound->CreateHashTable(csound);
    ws->pathSetFloatsHashTable = csound->CreateHashTable(csound);
    ws->pathSetStringHashTable = csound->CreateHashTable(csound);
    ws->refCount = 1;

    csound->SetHashTableValue(csound, shared->portWebsocketHashTable, (char*) &p->portKey, ws);

    // Allocate 2 protocols; the actual protocol, and a null protocol at the end. It's a libwebsocket thing.
    ws->protocols = csound->Calloc(csound, sizeof(struct lws_protocols) * 2);

    ws->protocols[0].callback = callback;
    ws->protocols[0].id = 1000;
    ws->protocols[0].name = "csound";
    ws->protocols[0].per_session_data_size = sizeof(void*);
    ws->protocols[0].user = ws;

    ws->info.port = port;
    ws->info.protocols = ws->protocols;
    ws->info.gid = -1;
    ws->info.uid = -1;

    ws->context = lws_create_context(&ws->info);
    if (UNLIKELY(ws->context == NULL)) {
        csound->InitError(csound, Str("cannot start websocket on port %d\n"), port);
    }

    ws->processThread = csound->CreateThread(processThread, ws);

    return ws;
}

WebsocketPathData *getWebsocketPathData(CSOUND *csound, CS_HASH_TABLE *pathHashTable, char *path)
{
    WebsocketPathData *pathData = NULL;

    char *pathKey = csound->GetHashTableKey(csound, pathHashTable, path);
    if (pathKey) {
        pathData = csound->GetHashTableValue(csound, pathHashTable, pathKey);
    }
    else {
        pathData = createWebsocketPathData(csound);
        csound->SetHashTableValue(csound, pathHashTable, path, pathData);
    }

    return pathData;
}

int32_t noop_perf(CSOUND *csound, void *p)
{
    IGN(csound);
    IGN(p);
    return OK;
}
