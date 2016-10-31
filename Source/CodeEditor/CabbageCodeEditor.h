/*
  ==============================================================================

    CabbageCodeEditor.h
    Created: 16 Oct 2016 2:59:39pm
    Author:  rory

  ==============================================================================
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
