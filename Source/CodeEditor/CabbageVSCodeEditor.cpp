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
    File vscode("/Users/rwalsh/sourcecode/cabbage/Source/CodeEditor/vscode.html");
    webView->setHTML(vscode.loadFileAsString().toStdString());
    
    auto v = webView->getViewHandle();
    nativeWindow = std::make_unique<NativeWindowComponent>(v);
    nativeWindow->setBounds(0, 0, 400, 400);
    nativeWindow->setWindow(v);
    addAndMakeVisible(nativeWindow.get());
    
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
    });
    
    currentLineNumber = lineNumber;
    allContent = content;
    
}

void CabbageVSCodeEditorComponent::scrollToLine(int line)
{
    auto jsCode = "scrollToLine("+String(line+1)+");";
    webView->evaluateJavascript(jsCode.toStdString());
}

void CabbageVSCodeEditorComponent::resized()
{
    nativeWindow->setBounds(0, 0, static_cast<int>(getWidth()), getHeight());
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

void CabbageVSCodeEditorComponent::parseTextForInstrumentsAndRegions()    //this is called on a separate thread..
{
    StringArray csdArray;
    csdArray.addLines (allContent);
    
   
    instrumentsAndRegions.clear();

    for (int i = 0 ; i < csdArray.size() ; i++)
    {
        if (csdArray[i].indexOf ("<Cabbage>") != -1)
        {
            instrumentsAndRegions.set ("<Cabbage>", i);
        }
        else if (csdArray[i].indexOf ("<CsoundSynthesiser>") != -1 ||
                 csdArray[i].indexOf ("<CsoundSynthesizer>") != -1)
        {
            instrumentsAndRegions.set ("<CsoundSynthesizer>", i);
        }

        else if ( csdArray[i].indexOf (";- Region:") != -1 || csdArray[i].indexOf ("//#") != -1)
        {
            const String region = csdArray[i].replace (";- Region:", "").replace("//#", "");
            instrumentsAndRegions.set (region, i);
        }


        else if ((csdArray[i].indexOf ("instr ") != -1 || csdArray[i].indexOf ("instr    ") != -1) &&
                 csdArray[i].substring (0, csdArray[i].indexOf ("instr")).isEmpty())
        {
            int commentInLine = csdArray[i].indexOf (";");
            String line = csdArray[i];
            String instrumentNameOrNumber = line.substring (csdArray[i].indexOf ("instr") + 6, commentInLine == -1 ? 1024 : commentInLine);
            const String identifier = "instr " + instrumentNameOrNumber.trim();

            if (identifier.isNotEmpty())
                instrumentsAndRegions.set (identifier, i);
        }
    }
}
