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

#pragma once
#include "../CabbageCommonHeaders.h"
#include "CabbageWidgetBase.h"


#include "../httplib.h"



class HttpServer : public Thread
{
public:
	HttpServer() : Thread("HttpServer")
	{
	}

	void start(int portNumber)
	{
		mPortNumber = portNumber;
		if (mServer.set_mount_point("/", "C://Users//rory//OneDrive//Csoundfiles"))
			DBG("success");

		mServer.Post("/login", [this](const httplib::Request& req, httplib::Response& res)
		{
			jassertfalse;
		});

		// ...

		mServer.Get("/logout", [this](const httplib::Request& req, httplib::Response& res)
		{
			jassertfalse;
		});

		startThread();
	}

	void stop()
	{
		mServer.stop();
	}

	void test() {
		mServer.Get("/index.html", [&](const httplib::Request& req, httplib::Response& res) {
			if (req.has_param("parameter1")) {
				auto value1 = req.get_param_value("parameter1");
			}
		});
	}

	bool isRunning() const noexcept
	{
		return isThreadRunning();
	}

protected:
	void run() override
	{
		mServer.listen("127.0.0.1", mPortNumber);
	}

protected:
	httplib::Server          mServer;
	int                      mPortNumber;
};


class CabbageWebView : public Component, public ValueTree::Listener, public CabbageWidgetBase
{

    float rotate, corners;
    int pivotx, pivoty;
	CabbagePluginEditor* owner;
	HttpServer server;
	std::unique_ptr<WebBrowserComponent> webView;
public:

    CabbageWebView (ValueTree wData, CabbagePluginEditor* _owner);
    ~CabbageWebView() override {}

    //ValueTree::Listener virtual methods....
    void valueTreePropertyChanged (ValueTree& valueTree, const Identifier&) override;
    void valueTreeChildAdded (ValueTree&, ValueTree&)override {}
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override {}

    ValueTree widgetData;

	void resized() override;



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabbageWebView)
};

