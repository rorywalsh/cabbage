#include "get.h"

#if WebUI

int32_t onWebsocketReceive(struct lws *websocket, void *inputData, size_t inputDataSize)
{
    if (inputData && 0 < inputDataSize) {
        const struct lws_protocols *protocol = lws_get_protocol(websocket);
        Websocket *ws = protocol->user;
        CSOUND *csound = ws->csound;

        char *data = NULL;

        int isFinalFragment = lws_is_final_fragment(websocket);
        if (isFinalFragment && 0 == ws->receiveBufferIndex) {
            data = inputData;
        }
        else {
            // Handle partially received messages.
            const int receivedCount = ws->receiveBufferIndex + inputDataSize;
            if (!ws->receiveBuffer) {
                ws->receiveBuffer = csound->Calloc(csound, receivedCount);
                ws->receiveBufferSize = receivedCount;
            }
            else if (ws->receiveBufferSize < receivedCount) {
                char *newReceiveBuffer = csound->Calloc(csound, receivedCount);
                memcpy(newReceiveBuffer, ws->receiveBuffer, ws->receiveBufferSize);
                csound->Free(csound, ws->receiveBuffer);
                ws->receiveBuffer = newReceiveBuffer;
                ws->receiveBufferSize = receivedCount;
            }
            char *receiveBufferDest = ws->receiveBuffer + ws->receiveBufferIndex;
            memcpy(receiveBufferDest, inputData, inputDataSize);
            if (!isFinalFragment) {
                ws->receiveBufferIndex += inputDataSize;
                return OK;
            }
            ws->receiveBufferIndex = 0;

            data = ws->receiveBuffer;
        }

        // Get the path. It should be a null terminated string at the beginning of the received data.
        char *d = data;
        char *path = d;

        size_t pathLength = strlen(path);
        if (pathLength == 0) {
            csound->Message(csound, Str("%s"), "WARNING: websocket path is empty\n");
            return OK;
        }
        d += pathLength + 1;

        // Get the data type. It should be either 1 or 2 for string or floats/doubles array.
        const int8_t type = *d;
        d++;

        size_t bufferSize = 0;
        WebsocketPathData *pathData = NULL;

        // Write the data to the path's message buffer.
        switch (type) {
        case FloatArrayType: {
            const uint32_t *length = (uint32_t*) d;
            d += 4;
            pathData = getWebsocketPathData(csound, ws->pathGetFloatsHashTable, path);
            bufferSize = *length * sizeof(MYFLT);
            break;
        }
        case StringType: {
            pathData = getWebsocketPathData(csound, ws->pathGetStringHashTable, path);
            bufferSize = strlen(d) + 1;
            break;
        }
        default:
            csound->Message(csound, Str("WARNING: Unknown websocket data type %d received\n"), type);
            return OK;
        }

        WebsocketMessage *msg = &pathData->message;

        csound->LockMutex(pathData->messageMutex);

        // NB: malloc is used instead of csound->Malloc for consistency with the set opcodes because csound->Free crashes after the buffer is given to lws_write.
        if (0 < msg->size && msg->size < bufferSize) {
            free(msg->buffer);
            msg->size = 0;
        }
        if (msg->size == 0) {
            msg->buffer = malloc(bufferSize);
            msg->size = bufferSize;
        }
        memcpy(msg->buffer, d, bufferSize);

        csound->UnlockMutex(pathData->messageMutex);
    }

    return OK;
}

int32_t websocket_get_init(CSOUND *csound, WS_get *p)
{
    initPlugin();

    p->common.csound = csound;

    initPortKey(&p->common.portKey, *p->port);
    p->common.websocket = getWebsocket(csound, *p->port, &p->common);

    const CS_TYPE *type = csound->GetTypeForArg(p->output);
    const char *typeName = type->varTypeName;
    const uint8_t dataType = ('S' == typeName[0]) ? StringType : FloatArrayType;

    switch (dataType) {
        case StringType:
            return websocket_getString_perf(csound, p);
        case FloatArrayType:
            return websocket_getArray_perf(csound, p);
        default:
            break;
    }

    return OK;
}

void static readWebsocketPathDataMessage(CSOUND *csound, CS_HASH_TABLE *pathHashTable, WS_get *p, int dataType)
{
    WebsocketPathData *pathData = csound->GetHashTableValue(csound, pathHashTable, p->path->data);
    if (!pathData) {
        return;
    }

    csound->LockMutex(pathData->previousMessageMutex);

    if (0 == csound->LockMutexNoWait(pathData->messageMutex)) {
        // Copy the message to the previous message buffer.
        WebsocketMessage *msg = &pathData->message;
        WebsocketMessage *prevMsg = &pathData->previousMessage;
        if (2 * prevMsg->size < msg->size) {
            csound->Free(csound, prevMsg->buffer);
            prevMsg->buffer = csound->Malloc(csound, 2 * msg->size);
            prevMsg->size = msg->size;
        }
        memcpy(prevMsg->buffer, msg->buffer, msg->size);

        csound->UnlockMutex(pathData->messageMutex);
    }

    // Copy previous message to output.
    WebsocketMessage *msg = &pathData->previousMessage;
    void *outputData = NULL;
    const size_t size = msg->size;

    if (FloatArrayType == dataType) {
        ARRAYDAT *output = p->output;
        if (output->allocated < size) {
            csound->Free(csound, output->data);
            output->data = csound->Calloc(csound, 2 * size);
            output->allocated = 2 * size;
            output->sizes[0] = size / sizeof(MYFLT);
        }
        outputData = output->data;
    }
    else if (StringType == dataType) {
        STRINGDAT *output = p->output;
        if (output->size < (int) size) {
            csound->Free(csound, output->data);
            output->data = csound->Calloc(csound, 2 * size);
            output->size = 2 * size;
        }
        outputData = output->data;
    }

    memcpy(outputData, msg->buffer, size);

    csound->UnlockMutex(pathData->previousMessageMutex);
}

int32_t websocket_getArray_perf(CSOUND *csound, WS_get *p)
{
    readWebsocketPathDataMessage(csound, p->common.websocket->pathGetFloatsHashTable, p, FloatArrayType);
    return OK;
}

int32_t websocket_getString_perf(CSOUND *csound, WS_get *p)
{
    readWebsocketPathDataMessage(csound, p->common.websocket->pathGetStringHashTable, p, StringType);
    return OK;
}

#endif
