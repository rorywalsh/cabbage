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
}

CabbageCodeEditorComponent::~CabbageCodeEditorComponent()
{

}

void CabbageCodeEditorComponent::loadContent(String content)
{
    Timer::callAfterDelay(1000, [this, content]()
    {
        auto jsCode = "updateText(`" + content + "`);";
        webView->evaluateJavascript(jsCode.toStdString());
    });
    
}

void CabbageCodeEditorComponent::resized()
{
    nativeWindow.setBounds(0, 0, static_cast<int>(getWidth() * .97), getHeight());
}
