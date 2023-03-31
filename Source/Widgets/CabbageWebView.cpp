/*
  Copyright (C) 2016 Rory Walsh

  Cabbage is free software; you can redistribute it
  and/or modify it under the terms of the GNU General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  Cabbage is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU General Public
  License along with Csound; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
  02111-1307 USA
*/

#include "CabbageWebView.h"

#include <chrono>
#include <thread>

CabbageWebView::CabbageWebView (ValueTree wData, CabbagePluginEditor* o)
    : widgetData (wData),
    CabbageWidgetBase(o), hComp()
{
    setName(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::name));
    setLookAndFeel(nullptr);
    choc::ui::WebView::Options options;
    options.enableDebugMode = true;
    webView.reset(new choc::ui::WebView(options));

    addAndMakeVisible(hComp);
    
    const int port = CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::serverPort);
    const String mntPoint = CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::mountPoint);
    server.start(port, mntPoint.toStdString());
    webView->navigate("http://127.0.0.1:"+std::to_string(port)+"/index.html");

  
    hComp.setHWND(webView->getViewHandle());

    setName (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::name));
    widgetData.addListener (this);              //add listener to valueTree so it gets notified when a widget's property changes
    initialiseCommonAttributes (this, wData);   //initialise common attributes such as bounds, name, rotation, etc..
   
}

CabbageWebView::~CabbageWebView()
{
    //stopping the thread is causing an issue with the look and feel
    //which is still active when the object is being destroyed..
    server.getHttpServer().stop();
    //we have to wait for thread to stop..
    server.stopThread(-1);
}

void CabbageWebView::resized() 
{
    hComp.setBounds(getLocalBounds());       
}

void CabbageWebView::valueTreePropertyChanged (ValueTree& valueTree, const Identifier& prop)
{
   
    //handleCommonUpdates (this, valueTree, false, prop);      //handle comon updates such as bounds, alpha, rotation, visible, etc
}

//====================================================================================
std::string dump_headers(const httplib::Headers& headers) {
    std::string s;
    char buf[BUFSIZ];

    for (const auto& x : headers) {
        snprintf(buf, sizeof(buf), "%s: %s\n", x.first.c_str(), x.second.c_str());
        s += buf;
    }

    return s;
}

std::string dump_multipart_files(const httplib::MultipartFormDataMap& files) {
    std::string s;
    char buf[BUFSIZ];

    s += "--------------------------------\n";

    for (const auto& x : files) {
        const auto& name = x.first;
        const auto& file = x.second;

        snprintf(buf, sizeof(buf), "name: %s\n", name.c_str());
        s += buf;

        snprintf(buf, sizeof(buf), "filename: %s\n", file.filename.c_str());
        s += buf;

        snprintf(buf, sizeof(buf), "content type: %s\n", file.content_type.c_str());
        s += buf;

        snprintf(buf, sizeof(buf), "text length: %zu\n", file.content.size());
        s += buf;

        s += "----------------\n";
    }

    return s;
}


std::string log(const httplib::Request& req, const httplib::Response& res) {
    std::string s;
    char buf[BUFSIZ];

    s += "================================\n";

    snprintf(buf, sizeof(buf), "%s %s %s", req.method.c_str(),
        req.version.c_str(), req.path.c_str());
    s += buf;

    std::string query;
    for (auto it = req.params.begin(); it != req.params.end(); ++it) {
        const auto& x = *it;
        snprintf(buf, sizeof(buf), "%c%s=%s",
            (it == req.params.begin()) ? '?' : '&', x.first.c_str(),
            x.second.c_str());
        query += buf;
    }
    snprintf(buf, sizeof(buf), "%s\n", query.c_str());
    s += buf;

    s += dump_headers(req.headers);
    s += dump_multipart_files(req.files);

    s += "--------------------------------\n";

    snprintf(buf, sizeof(buf), "%d\n", res.status);
    s += buf;
    s += dump_headers(res.headers);

    return s;
}

void HttpServer::run()
{
     mServer.listen("127.0.0.1", mPortNumber);
}

void HttpServer::start(int portNumber, std::string mountPoint)
{
    mPortNumber = portNumber;

    if (mServer.set_mount_point("/", mountPoint))
        DBG("success");

    mServer.set_logger([](const auto& req, const auto& res) {
        DBG(log(req, res));
        });

    mServer.Get("/stop", [&](const auto& /*req*/, auto& /*res*/) {
        mServer.stop();
        });

    startThread();
}
