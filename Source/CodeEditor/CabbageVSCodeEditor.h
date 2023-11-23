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
#ifndef CABBAGEVSCODEEDITOR_H_INCLUDED
#define CABBAGEVSCODEEDITOR_H_INCLUDED

#include "../CabbageIds.h"
#include "CsoundTokeniser.h"
#include "../CabbageCommonHeaders.h"
#include "../Utilities/CabbageStrings.h"


#ifndef JUCE_LINUX

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

    void mouseDown(const MouseEvent &event) override
    {
        jassertfalse;
    }

};
class CabbageEditorContainer;

class CabbageCodeEditorComponent :
   public Component, public ChangeBroadcaster
{

public:
    CabbageCodeEditorComponent (CabbageEditorContainer* owner, Component* statusBar, ValueTree valueTree);
    ~CabbageCodeEditorComponent() override;
    void hide();
    void resized() override;
    NativeWindowComponent nativeWindow;
    void loadContent(String content);
    void updateColourScheme(bool isCsdFile=true){  }
    void setFont(Font f){  }
    void setFontSize(int size){  }
    void startThread(){  }
    void cut(){ jassertfalse; }
    void copy(){ jassertfalse; }
    void paste(){ jassertfalse; }
    void redo(){ jassertfalse; }
    void undo(){ jassertfalse; }
    void selectAll(){ jassertfalse; }
    void parseTextForInstrumentsAndRegions(){ }
    void insertCode(int lineNumber, String updatedText, bool, bool){ jassertfalse; }
    void scrollToLine(int line){ jassertfalse; }
    String getAllText(){    jassertfalse;   return ""; }
    String getLineText(int line){    jassertfalse;   return ""; }
    int getCaretPos(){     jassertfalse;   return -1; }
    void setLineNumbersShown(bool){jassertfalse;}
    void updateCurrenLineMarker(){ jassertfalse; }
    void toggleComments(){jassertfalse;}
    void runInDebugMode(){jassertfalse;}
    void zoomIn(){jassertfalse;}
    void zoomOut(){jassertfalse;}
    bool hasFileChanged(){ jassertfalse; return true; }
    void setSavePoint(){ }
    int findText (String, bool, bool, bool){ jassertfalse;}
    String getSelectedText(){ jassertfalse; return ""; }
    void setAllText(String){jassertfalse;}
    void replaceText(String, String){jassertfalse;}
    String getAllContent(){ jassertfalse; return ""; }

    NamedValueSet instrumentsAndRegions;
private:
    std::unique_ptr<choc::ui::WebView> webView;
};

#endif
#endif  // CABBAGEVSCODEEDITOR_H_INCLUDED
