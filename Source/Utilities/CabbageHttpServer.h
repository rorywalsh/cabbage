#pragma once

#include "../CabbageCommonHeaders.h"
#include "httplib.h"

class CabbageHttpServer : public Thread
{
public:
    CabbageHttpServer() : Thread("HttpServer") {	}
    ~CabbageHttpServer() {
#if Cabbage_IDE_Build
        clearSingletonInstance();
#endif
    }
    void start(int portNumber, std::string mountPoint);

    bool isRunning() const noexcept { return isThreadRunning(); }
    httplib::Server& getHttpServer() { return mServer; }

#if Cabbage_IDE_Build
    JUCE_DECLARE_SINGLETON(CabbageHttpServer, true)
#endif

protected:
    void run() override;

private:
    httplib::Server          mServer;
    int                      mPortNumber;
};

