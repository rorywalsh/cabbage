#pragma once

#include "../CabbageCommonHeaders.h"
#include "httplib.h"

class CabbageHttpServer : public Thread
{
public:
    CabbageHttpServer() : Thread("HttpServer") {	}

    void start(int portNumber, std::string mountPoint);

    bool isRunning() const noexcept { return isThreadRunning(); }
    httplib::Server& getHttpServer() { return mServer; }

protected:
    void run() override;

protected:
    httplib::Server          mServer;
    int                      mPortNumber;
};

