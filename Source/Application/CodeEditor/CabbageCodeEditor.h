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

class CabbageCodeEditorComponent : public CodeEditorComponent
{
public:

    CodeDocument::Position positionInCode;
    CabbageCodeEditorComponent(ValueTree valueTree, CodeDocument &document, CodeTokeniser *codeTokeniser);
	void updateColourScheme();
    ~CabbageCodeEditorComponent() {};
	ValueTree valueTree;
};



#endif  // CABBAGECODEEDITOR_H_INCLUDED
