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

#include "CabbageCodeEditor.h"
#include "../Settings/CabbageSettings.h"
#include "../Utilities/CabbageUtilities.h"
#include "../Application/CabbageDocumentWindow.h"

//==============================================================================
CabbageCodeEditorComponent::CabbageCodeEditorComponent(Component* statusBar, ValueTree valueTree, CodeDocument &document, CodeTokeniser *codeTokeniser)
    : CodeEditorComponent(document, codeTokeniser), valueTree(valueTree), statusBar(statusBar)
{
	
    String opcodeFile = File(File::getSpecialLocation(File::currentExecutableFile)).getParentDirectory().getFullPathName();
    opcodeFile += "/opcodes.txt";
    Logger::writeToLog(opcodeFile);

    if(File(opcodeFile).existsAsFile())
        setOpcodeStrings(File(opcodeFile).loadFileAsString());	
	
	document.addListener(this);
	this->setColour(CodeEditorComponent::ColourIds::lineNumberBackgroundId, CabbageSettings::getColourFromValueTree(valueTree, CabbageColourIds::lineNumberBackground, Colour(70,70,70)));
	this->setColour(CodeEditorComponent::ColourIds::lineNumberTextId, CabbageSettings::getColourFromValueTree(valueTree, CabbageColourIds::lineNumbers, Colours::white));
}

void CabbageCodeEditorComponent::updateColourScheme()
{
	struct Type
	{
		const char* name;
		uint32 colour;
	};

	const Type types[] =
	{
		{ "Error",              CabbageSettings::getColourFromValueTree(valueTree, CabbageColourIds::numbers, Colours::grey.darker()).getARGB() },
		{ "Comment",            CabbageSettings::getColourFromValueTree(valueTree, CabbageColourIds::comment, Colours::grey.darker()).getARGB() },
		{ "Keyword",            CabbageSettings::getColourFromValueTree(valueTree, CabbageColourIds::keyword, Colours::grey.darker()).getARGB() },
		{ "Identifier",         CabbageSettings::getColourFromValueTree(valueTree, CabbageColourIds::identifierLiteral, Colours::grey.darker()).getARGB() },
		{ "Integer",            Colours::grey.darker().getARGB() },
		{ "Float",              Colours::grey.darker().getARGB() },
		{ "String",             CabbageSettings::getColourFromValueTree(valueTree, CabbageColourIds::stringLiteral, Colours::grey.darker()).getARGB() },
		{ "Operator",           Colours::grey.darker().getARGB() },
		{ "Bracket",            Colours::grey.darker().getARGB() },
		{ "Punctuation",        Colours::grey.darker().getARGB() },
		{ "Preprocessor Text",  Colours::grey.darker().getARGB() },
		{ "Csd Tag",			CabbageSettings::getColourFromValueTree(valueTree, CabbageColourIds::csdtags, Colours::grey.darker()).getARGB()}
	};

	CodeEditorComponent::ColourScheme cs;

	setColour(CodeEditorComponent::ColourIds::lineNumberBackgroundId, CabbageSettings::getColourFromValueTree(valueTree, CabbageColourIds::lineNumberBackground, Colour(70,70,70)));
	setColour(CodeEditorComponent::ColourIds::lineNumberTextId, CabbageSettings::getColourFromValueTree(valueTree, CabbageColourIds::lineNumbers, Colours::white));
	setColour(CodeEditorComponent::ColourIds::backgroundColourId, CabbageSettings::getColourFromValueTree(valueTree, CabbageColourIds::codeBackground, Colours::white));
	setColour(CaretComponent::ColourIds::caretColourId, CabbageSettings::getColourFromValueTree(valueTree, CabbageColourIds::caret, Colours::white));
	setColour(CodeEditorComponent::ColourIds::highlightColourId, CabbageSettings::getColourFromValueTree(valueTree, CabbageColourIds::selectTextBackground, Colours::white));

	for (int i = 0; i < sizeof (types) / sizeof (types[0]); ++i)  // (NB: numElementsInArray doesn't work here in GCC4.2)
		cs.set (types[i].name, Colour (types[i].colour));

	this->setColourScheme(cs);
}

void CabbageCodeEditorComponent::codeDocumentTextInserted(const String &text,int)
{
    const String lineFromCsd = getDocument().getLine(getDocument().findWordBreakBefore(getCaretPos()).getLineNumber());
    displayOpcodeHelpInStatusBar(lineFromCsd);
}

//==============================================================================
void CabbageCodeEditorComponent::displayOpcodeHelpInStatusBar(String lineFromCsd)
{
    String opcodeHelpString;
    StringArray syntaxTokens, csdLineTokens;
    csdLineTokens.clear();
    csdLineTokens.addTokens(lineFromCsd, " ,\t", "");

    for(int i=0; i<opcodeStrings.size(); i++)
    {
        opcodeHelpString = opcodeStrings[i];
        syntaxTokens.clear();
        syntaxTokens.addTokens(opcodeHelpString, ";", "\"");
        if(syntaxTokens.size()>3)
            for(int x=0; x<csdLineTokens.size(); x++)
            {
                if(syntaxTokens[0].removeCharacters("\"")==csdLineTokens[x].trim())
                {
                    if(syntaxTokens[0].length()>3)
                    {
						if(EditorAndConsoleContentComponent::StatusBar* bar = dynamic_cast<EditorAndConsoleContentComponent::StatusBar*>(statusBar))
							bar->setText(syntaxTokens);
                        x=csdLineTokens.size();
                        i=opcodeStrings.size();
                    }
                }
            }
    }
}

bool CabbageCodeEditorComponent::keyPressed (const KeyPress& key)
{
    //Logger::writeToLog(String(key.getKeyCode()));
    if (key.getTextDescription().contains("cursor up") || key.getTextDescription().contains("cursor down"))
    {
  
    }

    this->getParentComponent()->repaint();

    if (key == KeyPress ('z', ModifierKeys::commandModifier, 0))
        undoText();

    if (! TextEditorKeyMapper<CodeEditorComponent>::invokeKeyFunction (*this, key))
    {

        if (key == KeyPress::returnKey)
            handleReturnKey();

        else if (key == KeyPress::escapeKey)
            handleEscapeKey();
        //else if (key == KeyPress (']', ModifierKeys::commandModifier, 0))   indentSelection();
        else if (key.getTextCharacter() >= ' ')
        {
            insertTextAtCaret (String::charToString (key.getTextCharacter()));
            scrollToKeepCaretOnScreen();
        }
        else if(key.getKeyCode() ==  KeyPress::tabKey && key.getModifiers().isShiftDown())
            handleTabKey("backwards");
        else if(key ==  KeyPress::tabKey)
            handleTabKey("forwards");

        else
            return false;
    }
    //handleUpdateNowIfNeeded();
    return true;
}
//==============================================================================
void CabbageCodeEditorComponent::handleTabKey(String direction)
{

 

}
//==============================================================================
void CabbageCodeEditorComponent::undoText()
{
    CodeDocument::Position startPos = getCaretPos();
    //getDocument().undo();
    moveCaretTo(startPos, false);
}
//==============================================================================
String CabbageCodeEditorComponent::getLineText()
{
	return String::empty;
}

void CabbageCodeEditorComponent::insertCode(int lineNumber, String codeToInsert, bool highlightLine)
{
    StringArray csdLines;
    csdLines.addLines(getDocument().getAllContent());
	csdLines.set(lineNumber, codeToInsert);
	getDocument().replaceAllContent(csdLines.joinIntoString("\n"));
	
	if(highlightLine)
	{
		moveCaretTo(CodeDocument::Position(this->getDocument(), lineNumber, 0), false);
		moveCaretTo(CodeDocument::Position(this->getDocument(), lineNumber, 4096), true);
	}
}