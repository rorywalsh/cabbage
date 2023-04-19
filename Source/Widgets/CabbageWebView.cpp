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
#include "../Audio/Plugins/CabbagePluginEditor.h"




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
    
    const int port = CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::serverport);
    const int wsPort = CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::websocketport);
    const String mntPoint = CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::mountPoint);
#if !Cabbage_IDE_Build
    if (!server->isRunning())
        server->start(port, mntPoint.toStdString());
#else
    auto* server = CabbageHttpServer::getInstance();
    if(!server->isRunning())
        server->start(port, mntPoint.toStdString());
#endif

    webView->navigate("http://127.0.0.1:" + std::to_string(port) + "/index.html?wsPort="+ std::to_string(wsPort));

  
    hComp.setHWND(webView->getViewHandle());

    setName (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::name));
    widgetData.addListener (this);              //add listener to valueTree so it gets notified when a widget's property changes
    initialiseCommonAttributes (this, wData);   //initialise common attributes such as bounds, name, rotation, etc..
   
}

CabbageWebView::~CabbageWebView()
{
   
}

void CabbageWebView::resized() 
{
    hComp.setBounds(getLocalBounds());       
}

void CabbageWebView::valueTreePropertyChanged (ValueTree& valueTree, const Identifier& prop)
{   
    //handleCommonUpdates (this, valueTree, false, prop);      //handle comon updates such as bounds, alpha, rotation, visible, etc
}
