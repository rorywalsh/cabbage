#include "set.h"

static void sendWebsocketPathData(CSOUND *csound, struct lws *websocket, WebsocketPathData *pathData)
{
    csound->LockMutex(pathData->messageMutex);

    if (!pathData->sent) {
        lws_write(websocket, (unsigned char*) pathData->message.buffer, pathData->message.size, LWS_WRITE_BINARY);
        pathData->sent = true;
    }

    csound->UnlockMutex(pathData->messageMutex);
}

static void sendWebsocketPathHashTable(CSOUND *csound, struct lws *websocket, CS_HASH_TABLE *pathHashTable)
{
    CONS_CELL *pathItem = csound->GetHashTableValues(csound, pathHashTable);
    while (pathItem) {
        WebsocketPathData *pathData = pathItem->value;
        sendWebsocketPathData(csound, websocket, pathData);
        pathItem = pathItem->next;
    }
}

int32_t onWebsocketWritable(struct lws *websocket)
{
    const struct lws_protocols *protocol = lws_get_protocol(websocket);
    Websocket *ws = protocol->user;
    CSOUND *csound = ws->csound;

    sendWebsocketPathHashTable(csound, websocket, ws->pathSetFloatsHashTable);
    sendWebsocketPathHashTable(csound, websocket, ws->pathSetStringHashTable);

    return OK;
}

int32_t websocket_set_init(CSOUND *csound, WS_set *p)
{
    initPlugin();

    p->common.csound = csound;

    initPortKey(&p->common.portKey, *p->port);
    p->common.websocket = getWebsocket(csound, *p->port, &p->common);

    size_t pathLength = strlen(p->path->data);

    const CS_TYPE *type = csound->GetTypeForArg(p->input);
    const char *typeName = type->varTypeName;
    const uint8_t dataType = ('S' == typeName[0]) ? StringType : FloatArrayType;

    p->msgPreSize = pathLength + 2; // Path length + null terminator + data type.
    p->msgPre = csound->Calloc(csound, p->msgPreSize);

    char *d = p->msgPre;
    memcpy(d, p->path->data, strlen(p->path->data));
    d += pathLength + 1; // Advance past path's null terminator.

    *d = dataType;

    switch (dataType) {
        case FloatArrayType:
            return websocket_setArray_perf(csound, p);
        case StringType:
            return websocket_setString_perf(csound, p);
        default:
            break;
    }

    return OK;
}

static void writeWebsocketPathDataMessage(CSOUND *csound, CS_HASH_TABLE *pathHashTable, WS_set *p, int dataType, const void *data, size_t dataSize)
{
    WebsocketPathData *pathData = getWebsocketPathData(csound, pathHashTable, p->path->data);

    csound->LockMutex(pathData->messageMutex);

    const size_t msgSize = p->msgPreSize + dataSize + ((FloatArrayType == dataType) ? 4 : 0);

    // NB: malloc is used instead of csound->Malloc because csound->Free crashes after the buffer is given to lws_write.
    WebsocketMessage *msg = &pathData->message;
    if (msg->size < msgSize) {
        free(msg->buffer);
        msg->buffer = malloc(2 * msgSize);
        msg->size = msgSize;
    }

    char *d = msg->buffer;
    memcpy(d, p->msgPre, p->msgPreSize); // Path and data type.
    d += p->msgPreSize;

    if (FloatArrayType == dataType) {
        *((uint32_t*) d) = dataSize / sizeof(MYFLT); // Array length.
        d += 4;
    }

    memcpy(d, data, dataSize); // Array or String data.

    pathData->sent = false;

    csound->UnlockMutex(pathData->messageMutex);
}

int32_t websocket_setArray_perf(CSOUND *csound, WS_set *p) {
    ARRAYDAT *input = p->input;
    writeWebsocketPathDataMessage(csound, p->common.websocket->pathSetFloatsHashTable, p, FloatArrayType, input->data, input->allocated);
    return OK;
}

int32_t websocket_setString_perf(CSOUND *csound, WS_set *p) {
    STRINGDAT *input = p->input;
    writeWebsocketPathDataMessage(csound, p->common.websocket->pathSetStringHashTable, p, StringType, input->data, input->size);
    return OK;
}
