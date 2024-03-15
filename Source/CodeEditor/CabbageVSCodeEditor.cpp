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
CabbageVSCodeEditorComponent::CabbageVSCodeEditorComponent (CabbageEditorContainer* owner, Component* statusBar, ValueTree valueTree)
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

CabbageVSCodeEditorComponent::~CabbageVSCodeEditorComponent()
{

}

void CabbageVSCodeEditorComponent::loadContent(String content, int lineNumber, int delay)
{
    
    
    Timer::callAfterDelay(delay, [this, lineNumber, delay, content]()
    {
        auto jsCode = "updateText(`" + content + "`, " + String(lineNumber+1) +");";
        webView->evaluateJavascript(jsCode.toStdString());
        insertSuggestion();
    });
    
    currentLineNumber = lineNumber;
    allContent = content;
    
}

void CabbageVSCodeEditorComponent::resized()
{
    nativeWindow.setBounds(0, 0, static_cast<int>(getWidth()), getHeight());
}

void CabbageVSCodeEditorComponent::insertSuggestion()
{
    //auto xmlDoc = XmlDocument(CabbageStrings::getOpcodesXml());
    


    //webView->evaluateJavascript(newSuggestion.toStdString());
}

void CabbageVSCodeEditorComponent::removeCurrentLine()
{
    StringArray lines;
    lines.addLines(allContent);
    lines.remove(currentLineNumber);
    loadContent(lines.joinIntoString("\n"), currentLineNumber, 100);
}

String CabbageVSCodeEditorComponent::getLineText(int line)
{
    StringArray lines;
    lines.addLines(allContent);
    return lines[line];
}

void CabbageVSCodeEditorComponent::insertCode(int lineNumber, String updatedText, bool, bool)
{
    StringArray lines;
    lines.addLines(allContent);
    lines.set(lineNumber, updatedText);
    loadContent(lines.joinIntoString("\n"), lineNumber, 100);
}

void CabbageVSCodeEditorComponent::updateBoundsText (int lineNumber, String codeToInsert, bool shouldHighlight)
{
    StringArray csdLines;
    csdLines.addLines (allContent);
    const int currentIndexOfBounds = csdLines[lineNumber].indexOf("bounds");
    const int newIndexOfBounds = csdLines[lineNumber].indexOf("bounds");
    const String currentLine = csdLines[lineNumber];
    const String currentBounds = currentLine.substring(currentIndexOfBounds, currentLine.indexOf(currentIndexOfBounds, ")")+1);
    const String newBounds = codeToInsert.substring(newIndexOfBounds, codeToInsert.indexOf(newIndexOfBounds, ")")+1);
    
    if(currentIndexOfBounds == -1)
        csdLines.insert (lineNumber, codeToInsert);
    else
        csdLines.set (lineNumber, currentLine.replace(currentBounds, newBounds));

    loadContent(csdLines.joinIntoString ("\n"), lineNumber, 100);

}

