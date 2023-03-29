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


CabbageWebView::CabbageWebView (ValueTree wData, CabbagePluginEditor* o)
    : widgetData (wData),
    CabbageWidgetBase(o), webView(), hComp()
{
    addAndMakeVisible(hComp);
    //webView.setHTML(R"xxx(
    //  <!DOCTYPE html> <html>
    //    <head> <title>Page Title</title> </head>
    //    <script>
    //      var eventCounter = 0;
    //      // invokes a call to eventCallbackFn() and displays the return value
    //      function sendEvent()
    //      {
    //        // When you invoke a function, it returns a Promise object
    //        eventCallbackFn({ counter: ++eventCounter }, "Hello World")
    //          .then ((result) => { document.getElementById ("eventResultDisplay").innerText = result; });
    //      }
    //    </script>
    //    <body>
    //      <h1>WebView</h1>
    //      <p id="injectedJS"></p>
    //    </body>
    //  </html>
    //)xxx");

    webView.setHTML(R"xxx(
    <!DOCTYPE html>
        <html>
        <body>

        <canvas id="myCanvas" width="300" height="150" style="border:1px solid #d3d3d3;">
        Your browser does not support the HTML5 canvas tag.</canvas>

        <script>
            globalColr = "blue"
            var c = document.getElementById("myCanvas");
            var ctx = c.getContext("2d");

             globalThis.setColour = function(){
                ctx.fillStyle = "blue";
                ctx.beginPath();
                ctx.rect(20, 20, 150, 100);
                ctx.fill();
            }

           
        </script> 

    </body>
    </html>

       )xxx");
 
    hComp.setHWND(webView.getViewHandle());

    setName (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::name));
    widgetData.addListener (this);              //add listener to valueTree so it gets notified when a widget's property changes
    initialiseCommonAttributes (this, wData);   //initialise common attributes such as bounds, name, rotation, etc..


    Timer::callAfterDelay(5000, [this]()
    {
        //const std::string script(R"xxx(document.getElementById("injectedJS").innerText = "Updated on the fly")xxx");
        const std::string script(R"xxx(globalThis.setColour();)xxx");
        webView.evaluateJavascript(script);
    });
}



void CabbageWebView::resized() 
{
    hComp.setBounds(getLocalBounds());       
}

void CabbageWebView::valueTreePropertyChanged (ValueTree& valueTree, const Identifier& prop)
{
   
    //handleCommonUpdates (this, valueTree, false, prop);      //handle comon updates such as bounds, alpha, rotation, visible, etc
}


