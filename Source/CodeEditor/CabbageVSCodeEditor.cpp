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

#include "CabbageVSCodeEditor.h"
//#include "../Settings/CabbageSettings.h"
//#include "../Utilities/CabbageUtilities.h"
#include "../Application/CabbageDocumentWindow.h"

//==============================================================================
CabbageCodeEditorComponent::CabbageCodeEditorComponent (CabbageEditorContainer* owner, Component* statusBar, ValueTree valueTree)
    : Component ("VS Code Editor")

{
    
    choc::ui::WebView::Options options;
    options.enableDebugMode = true;
    webView.reset(new choc::ui::WebView(options));
    File vscode("~/sourcecode/vscode.html");
    webView->setHTML(vscode.loadFileAsString().toStdString());
    
    auto v = webView->getViewHandle();
    nativeWindow.setBounds(0, 0, 400, 400);
    nativeWindow.setWindow(v);
    addAndMakeVisible(nativeWindow);
    
    webView->bind("textHasChanged", [this](const choc::value::ValueView& args) -> choc::value::Value
        {
        auto j = choc::json::toString(args, true);
        DBG("==================================================");
//        
        auto json = nlohmann::json::parse(j);
        allContent = json[0].get<std::string>();
//        allContent.clear();
//        allContent.addLines(json[0].get<std::string>());
        DBG(allContent);
//        DBG(json[0].get<std::string>());
//        DBG("==================================================");
//        DBG(allContent.joinIntoString("\n").trimCharactersAtEnd("\"").trimCharactersAtStart("\""));
        hasTextChanged = true;
        return choc::value::createString("Text has changed in editor");
    });
}

CabbageCodeEditorComponent::~CabbageCodeEditorComponent()
{

}

void CabbageCodeEditorComponent::loadContent(String content)
{
    Timer::callAfterDelay(1000, [this, content]()
    {
        auto jsCode = "updateText(`" + content + "`);";
//        jassertfalse;
        webView->evaluateJavascript(jsCode.toStdString());
    });
    
}

void CabbageCodeEditorComponent::resized()
{
    nativeWindow.setBounds(0, 0, static_cast<int>(getWidth()), getHeight());
}
