#pragma once

#include "../CabbageCommonHeaders.h"
#include "httplib.h"


class CabbageHttpServer : public Thread
{
public:
    CabbageHttpServer() : Thread("HttpServer") {

    }
    ~CabbageHttpServer() {
#if Cabbage_IDE_Build
        clearSingletonInstance();
#endif
    }
    void start(int portNumber, std::string mountPoint);

    bool isRunning() const noexcept { return isThreadRunning(); }
    httplib::Server& getHttpServer() { return mServer; }
//
//    void fileChanged(juce::File f, FileSystemWatcher::FileSystemEvent fsEvent) override
//    {
//         switch (fsEvent)
//            {
//            case FileSystemWatcher::fileCreated: DBG("Created");
//            case FileSystemWatcher::fileUpdated: DBG("Updated");
//            case FileSystemWatcher::fileDeleted: DBG("Deleted");
//            case FileSystemWatcher::fileRenamedOldName: DBG("Renamed From");
//            case FileSystemWatcher::fileRenamedNewName: DBG("Renamed To");
//            default: return;
//            }
//    }


#if Cabbage_IDE_Build
    JUCE_DECLARE_SINGLETON(CabbageHttpServer, true)
#endif

protected:
    void run() override;

private:
    httplib::Server          mServer;
    int                      mPortNumber;
    std::string mountPoint = "";
};

