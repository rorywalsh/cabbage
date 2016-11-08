/*
  Copyright (C) 2016 Rory Walsh

  Cabbage is free software; you can redistribute it
  and/or modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  Cabbage is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with Csound; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
  02111-1307 USA
*/
#ifndef CABBAGECODEEDITOR_H_INCLUDED
#define CABBAGECODEEDITOR_H_INCLUDED

#include "../CabbageIds.h"
#include "CsoundTokeniser.h"


class CabbageCodeEditorComponent : public CodeEditorComponent, public CodeDocument::Listener
{
public:
    CabbageCodeEditorComponent(Component* statusBar, ValueTree valueTree, CodeDocument &document, CodeTokeniser *codeTokeniser);
    ~CabbageCodeEditorComponent(){};
	void updateColourScheme();
    CodeDocument::Position positionInCode;
	ValueTree valueTree;
	void codeDocumentTextDeleted(int,int){};
    void codeDocumentTextInserted(const juce::String &,int);
	void displayOpcodeHelpInStatusBar(String lineFromCsd);
	void insertCodeAndHighlightLine(int lineNumber, String codeToInsert);
	String getLineText();
	bool keyPressed (const KeyPress& key) override;
	void undoText();
	void handleTabKey(String direction);
	
	void setOpcodeStrings(String opcodes)
    {
        opcodeStrings.addLines(opcodes);
    }
private:
	Component* statusBar;
	StringArray opcodeStrings;
};



#endif  // CABBAGECODEEDITOR_H_INCLUDED
