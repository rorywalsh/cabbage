#pragma once

#include "../CabbageCommonHeaders.h"
#include "httplib.h"


class CabbageHttpServer : public Thread, public DeletedAtShutdown
{
public:
    CabbageHttpServer() : Thread("HttpServer"), DeletedAtShutdown() {
       
    }
    
    ~CabbageHttpServer() {
#if Cabbage_IDE_Build
        stopThread(100);
        clearSingletonInstance();
#endif
    }
    
    void changeMountPoint(std::string mp);

    void start(std::string mountPoint);

    bool isRunning() const noexcept { return isThreadRunning(); }
    httplib::Server& getHttpServer()
    {
        return mServer;
    }

    int getCurrentPort() { return mPortNumber; }

#if Cabbage_IDE_Build
    JUCE_DECLARE_SINGLETON(CabbageHttpServer, true)
#endif
    
protected:
    void run() override;
    

private:
    httplib::Server          mServer;
    int                      mPortNumber;
    bool isListening = true;
    std::string mountPoint = "";

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CabbageHttpServer);
    
};
