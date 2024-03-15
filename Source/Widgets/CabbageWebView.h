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
#include "../../choc/gui/choc_WebView.h"


class NativeWindowComponent :
#ifdef JUCE_WINDOWS
    public HWNDComponent
#elif JUCE_MAC
    public NSViewComponent
#else
    public XEmbedComponent
#endif
{
public:
    NativeWindowComponent() = default;
#if defined(JUCE_WINDOWS) || defined(JUCE_MAC)
    ~NativeWindowComponent() override
    {
#ifdef JUCE_WINDOWS
        setHWND(nullptr);
#elif JUCE_MAC
        setView(nullptr);
#endif
    }

    void setWindow(void* view)
    {
#ifdef JUCE_WINDOWS
        setHWND(view);
#elif JUCE_MAC
        setView(view);
#endif
    }
#else
    jassertfalse;
#endif

//    void mouseDown(const MouseEvent &event) override
//    {
//        jassertfalse;
//    }

};



class CabbageWebView : public Component, public ValueTree::Listener, public CabbageWidgetBase
{

    float rotate, corners;
    int pivotx, pivoty;
	CabbagePluginEditor* owner;

    NativeWindowComponent nwComp;

#if WebUI
	std::unique_ptr<choc::ui::WebView> webView;
#endif

public:

    CabbageWebView (ValueTree wData, CabbagePluginEditor* _owner);
	~CabbageWebView() override;

    void startServer(std::string mountPoint);
    //ValueTree::Listener virtual methods....
    void valueTreePropertyChanged (ValueTree& valueTree, const Identifier&) override;
    void valueTreeChildAdded (ValueTree&, ValueTree&)override {}
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override {}

    ValueTree widgetData;
#if !Cabbage_IDE_Build
    std::unique_ptr<CabbageHttpServer> server;
#endif
    
    
	void resized() override;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabbageWebView)
};

