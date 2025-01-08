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



class CabbageEditorContainer;

class CabbageVSCodeEditorComponent :
   public Component, public ChangeBroadcaster
{

public:
    CabbageVSCodeEditorComponent (CabbageEditorContainer* owner, Component* statusBar, ValueTree valueTree);
    ~CabbageVSCodeEditorComponent() override;
    void hide();
    void resized() override;
    std::unique_ptr<NativeWindowComponent> nativeWindow;
    void loadContent(String content, int lineNumber=0, int delay=1000);
    void updateColourScheme(bool isCsdFile=true){  }
    void setFont(Font f){  }
    void setFontSize(int size){  }
    void startThread(){  }
    void removeCurrentLine();
    void cut(){ jassertfalse; }
    void copy(){ jassertfalse; }
    void paste(){ jassertfalse; }
    void redo(){ jassertfalse; }
    void undo(){ jassertfalse; }
    void selectAll(){ jassertfalse; }
    void parseTextForInstrumentsAndRegions(){ }
    void insertCode(int lineNumber, String updatedText, bool, bool);
    void updateBoundsText (int lineNumber, String codeToInsert, bool shouldHighlight);
    void highlightLine(int number);
    void scrollToLine(int line){ jassertfalse; }
    String getAllText(){    return allContent; }
    String getLineText(int line);
    int getCaretPos(){     jassertfalse;   return -1; }
    void setLineNumbersShown(bool){jassertfalse;}
    void updateCurrenLineMarker(){ jassertfalse; }
    void toggleComments(){jassertfalse;}
    void runInDebugMode(){jassertfalse;}
    void zoomIn(){jassertfalse;}
    void zoomOut(){jassertfalse;}
    bool hasFileChanged(){ return hasTextChanged; }
    void setSavePoint(){ }
    void insertSuggestion();
    int findText (String, bool, bool, bool){ jassertfalse;}
    String getSelectedText(){ jassertfalse; return ""; }
    void setAllText(String){jassertfalse;}
    void replaceText(String, String){jassertfalse;}
    String getAllContent(){ return allContent; }

    NamedValueSet instrumentsAndRegions;
private:
    std::unique_ptr<choc::ui::WebView> webView;
    bool hasTextChanged = false;
    String allContent;
    int currentLineNumber = 0;
};


#endif  // CABBAGEVSCODEEDITOR_H_INCLUDED
