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
CabbageCodeEditorComponent::CabbageCodeEditorComponent(EditorAndConsoleContentComponent* owner, Component* statusBar, ValueTree valueTree, CodeDocument &document, CodeTokeniser *codeTokeniser)
    : CodeEditorComponent(document, codeTokeniser), valueTree(valueTree), statusBar(statusBar), owner(owner), autoCompleteListBox(), Thread("parseVariablesThread")
{

    String opcodeFile = File(File::getSpecialLocation(File::currentExecutableFile)).getParentDirectory().getFullPathName();
    opcodeFile += "/opcodes.txt";
    Logger::writeToLog(opcodeFile);

    if(File(opcodeFile).existsAsFile())
        setOpcodeStrings(File(opcodeFile).loadFileAsString());

    document.addListener(this);
    this->setColour(CodeEditorComponent::ColourIds::lineNumberBackgroundId, CabbageSettings::getColourFromValueTree(valueTree, CabbageColourIds::lineNumberBackground, Colour(70,70,70)));
    this->setColour(CodeEditorComponent::ColourIds::lineNumberTextId, CabbageSettings::getColourFromValueTree(valueTree, CabbageColourIds::lineNumbers, Colours::white));

    autoCompleteListBox.setRowHeight (listBoxRowHeight);
    autoCompleteListBox.setModel (this);
    autoCompleteListBox.addKeyListener(this);
    this->addKeyListener(this);
    owner->addChildComponent(autoCompleteListBox);


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

//==============================================================================
void CabbageCodeEditorComponent::codeDocumentTextInserted(const String &text,int)
{
    handleAutoComplete(text);
    const String lineFromCsd = getDocument().getLine(getDocument().findWordBreakBefore(getCaretPos()).getLineNumber());
    displayOpcodeHelpInStatusBar(lineFromCsd);

	
}

void CabbageCodeEditorComponent::codeDocumentTextDeleted(int,int)
{

}

void CabbageCodeEditorComponent::insertTextAtCaret (const String &textToInsert)
{
    if(!columnEditMode)
        insertText(textToInsert);
    else
        insertMultiLineTextAtCaret(textToInsert);
}


void CabbageCodeEditorComponent::insertText(String text)
{
    if(this->isHighlightActive())
    {
        getDocument().replaceSection(getHighlightedRegion().getStart(), getHighlightedRegion().getEnd(), "");
    }

    getDocument().insertText(getCaretPos(), text);

    if(variableNamesToShow.size()>0)
    {
        const int height = jmin(100, variableNamesToShow.size()*listBoxRowHeight);
        autoCompleteListBox.setBounds(getCaretRectangle().getX(),getCaretRectangle().getY()+16, 300, height);
        autoCompleteListBox.toFront(true);
        autoCompleteListBox.selectRow(0);
    }
}

void CabbageCodeEditorComponent::insertMultiLineTextAtCaret (String text)
{
    //sendActionMessage("make popup invisible");
    StringArray csdArray = getAllTextAsStringArray();
    String curLine;
    CodeDocument::Position newPos, indexPos;

    const CodeDocument::Position startPos(getDocument(), getHighlightedRegion().getStart());
    const CodeDocument::Position endPos(getDocument(), getHighlightedRegion().getEnd());
    int indexInLine = startPos.getIndexInLine();


    for(int i=startPos.getLineNumber(); i<endPos.getLineNumber()+1; i++)
    {
        while(csdArray[i].length()<indexInLine)
        {
            csdArray.getReference(i).append(" ", 1);
        }
        csdArray.set(i, csdArray[i].replaceSection(indexInLine, 0, text));
    }
    setAllText(csdArray.joinIntoString("\n"));

    CodeDocument::Position newStartPos(getDocument(), startPos.getLineNumber(), indexInLine+text.length());
    CodeDocument::Position newEndPos(getDocument(), endPos.getLineNumber(), indexInLine+text.length());
    moveCaretTo(newStartPos, false);
    moveCaretTo(newEndPos, true);
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

//==============================================================================
bool CabbageCodeEditorComponent::deleteBackwards (const bool moveInWholeWordSteps)
{
    const CodeDocument::Position startPos(getDocument(), getHighlightedRegion().getStart());
    const CodeDocument::Position endPos(getDocument(), getHighlightedRegion().getEnd());

    if(columnEditMode)
    {
        //sendActionMessage("make popup invisible");
        StringArray csdArray = getAllTextAsStringArray();
        String curLine;
        CodeDocument::Position newPos, indexPos;

        int indexInLine = startPos.getIndexInLine();

        for(int i=startPos.getLineNumber(); i<endPos.getLineNumber()+1; i++)
        {
            while(csdArray[i].length()<indexInLine)
            {
                csdArray.getReference(i).append(" ", 1);
            }
            csdArray.set(i, csdArray[i].replaceSection(indexInLine-1, 1, ""));
        }
        setAllText(csdArray.joinIntoString("\n"));

        const CodeDocument::Position newStartPos(getDocument(), startPos.getLineNumber(), indexInLine-1);
        const CodeDocument::Position newEndPos(getDocument(), endPos.getLineNumber(), indexInLine-1);
        moveCaretTo(newStartPos, false);
        moveCaretTo(newEndPos, true);
    }
    else
    {

        if (moveInWholeWordSteps)
        {
            getDocument().deleteSection(getCaretPos().getPosition(), getCaretPos().getPosition()+1);
            moveCaretTo (getDocument().findWordBreakBefore (getCaretPos()), true);
        }
        else if ( !isHighlightActive() )
        {
            CodeDocument::Position startPos = getCaretPos();
            startPos.moveBy (-1);
            getDocument().deleteSection(startPos.getPosition(), getCaretPos().getPosition());
        }
        else
        {
            getDocument().deleteSection(startPos.getPosition(), endPos.getPosition());
        }

        const CodeDocument::Position pos1 = getDocument().findWordBreakBefore(getCaretPos());
        const CodeDocument::Position pos2 = getDocument().findWordBreakAfter(getCaretPos());
        String currentWord = getDocument().getTextBetween(pos1, pos2);
        variableNamesToShow.clear();
        autoCompleteListBox.setVisible(false);

    }

    scrollToKeepCaretOnScreen();

    return true;
}
//==============================================================================
bool CabbageCodeEditorComponent::deleteForwards (const bool moveInWholeWordSteps)
{
    CodeDocument::Position startPos(getDocument(), getHighlightedRegion().getStart());
    CodeDocument::Position endPos(getDocument(), getHighlightedRegion().getEnd());

    if(columnEditMode)
    {
        //sendActionMessage("make popup invisible");
        StringArray csdArray = getAllTextAsStringArray();

        String curLine;
        CodeDocument::Position newPos, indexPos;

        int indexInLine = startPos.getIndexInLine();

        for(int i=startPos.getLineNumber(); i<endPos.getLineNumber()+1; i++)
        {
            while(csdArray[i].length()<indexInLine)
            {
                csdArray.getReference(i).append(" ", 1);
            }
            csdArray.set(i, csdArray[i].replaceSection(indexInLine, 1, ""));
        }
        setAllText(csdArray.joinIntoString("\n"));

        CodeDocument::Position newStartPos(getDocument(), startPos.getLineNumber(), indexInLine);
        CodeDocument::Position newEndPos(getDocument(), endPos.getLineNumber(), indexInLine);
        moveCaretTo(newStartPos, false);
        moveCaretTo(newEndPos, true);
    }

    else
    {
        if (moveInWholeWordSteps)
        {
            getDocument().deleteSection(getCaretPos().getPosition(), getCaretPos().getPosition()+1);
            moveCaretTo (getDocument().findWordBreakBefore (getCaretPos()), true);
        }
        else if ( !isHighlightActive() )
        {
            startPos.moveBy (+1);
            getDocument().deleteSection(getCaretPos().getPosition(), startPos.getPosition());
        }
        else
        {
            getDocument().deleteSection(startPos.getPosition(), endPos.getPosition());
        }

        const CodeDocument::Position pos1 = getDocument().findWordBreakBefore(getCaretPos());
        const CodeDocument::Position pos2 = getDocument().findWordBreakAfter(getCaretPos());
        String currentWord = getDocument().getTextBetween(pos1, pos2);
        variableNamesToShow.clear();
        autoCompleteListBox.setVisible(false);

        if(currentWord.isNotEmpty())
        {
            showAutoComplete(currentWord);
        }
    }
    return true;
}
//==============================================================================
void CabbageCodeEditorComponent::parseTextForVariables()	//this is called on a separate thread..
{
    String csdText = getDocument().getAllContent();

    StringArray tokens;
    variableNames.clear();
    tokens.addTokens(csdText, "  \n(),*%=\t", "");

    for(const String currentWord : tokens)
    {
        if(currentWord.startsWith("a") || currentWord.startsWith("i") ||
                currentWord.startsWith("k") || currentWord.startsWith("S") ||
                currentWord.startsWith("f"))
        {
            if(currentWord.isNotEmpty())
                variableNames.addIfNotAlreadyThere(currentWord);
        }
    }
}

void CabbageCodeEditorComponent::handleAutoComplete(String text)
{
    const CodeDocument::Position pos1 = getDocument().findWordBreakBefore(getCaretPos());
    const CodeDocument::Position pos2 = getDocument().findWordBreakAfter(getCaretPos());
    const String currentWord = getDocument().getTextBetween(pos1, pos2).trim();

    if(currentWord.startsWith("a") || currentWord.startsWith("i") ||
            currentWord.startsWith("k") ||currentWord.startsWith("S") ||
            currentWord.startsWith("f") || currentWord.startsWith("g"))
    {
        if(text==" " && currentWord.isNotEmpty())
        {
            variableNames.addIfNotAlreadyThere(currentWord);
            autoCompleteListBox.updateContent();
        }
    }

    variableNamesToShow.clear();
    autoCompleteListBox.setVisible(false);

    if(currentWord.isNotEmpty())
    {
		if(text!=" ")
			showAutoComplete(currentWord);
    }
}


void CabbageCodeEditorComponent::showAutoComplete(String currentWord)
{
    for (const String item : variableNames)
    {
        if (item.startsWith (currentWord))
        {
            variableNamesToShow.addIfNotAlreadyThere(item.trim());
            autoCompleteListBox.updateContent();
			if(variableNamesToShow.size()==1 && variableNamesToShow[0]==currentWord)
			{}
			else
				autoCompleteListBox.setVisible(true);
        }
    }
}
//===========================================================================================================
bool CabbageCodeEditorComponent::keyPressed (const KeyPress &key, Component *originatingComponent)
{
    //Logger::writeToLog(String(key.getKeyCode()));
    if (key.getTextDescription().contains("cursor up") || key.getTextDescription().contains("cursor down"))
    {
        if(autoCompleteListBox.isVisible())
        {
            const int selectedRow = autoCompleteListBox.getSelectedRow();

            if(key.getTextDescription().contains("cursor down"))
                autoCompleteListBox.selectRow(jmax(0, selectedRow+1));
            else if(key.getTextDescription().contains("cursor up"))
                autoCompleteListBox.selectRow(jmax(0, selectedRow-1));

            autoCompleteListBox.scrollToEnsureRowIsOnscreen(autoCompleteListBox.getSelectedRow());
        }
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

    StringArray csdArray = getAllTextAsStringArray();
    const CodeDocument::Position startPos(this->getDocument(), getHighlightedRegion().getStart());
    const CodeDocument::Position endPos(this->getDocument(), getHighlightedRegion().getEnd());

    if(direction.equalsIgnoreCase("forwards"))
    {
        if(getHighlightedRegion().getLength()==0) //single line tab
        {
            insertTabAtCaret();
            return;
        }
        else
        {
            for(int i=startPos.getLineNumber(); i<endPos.getLineNumber()+1; i++)
                csdArray.set(i, "\t"+csdArray[i]);
        }

        setAllText(csdArray.joinIntoString("\n"));
        highlightLines(startPos.getLineNumber(), endPos.getLineNumber());

    }
    else if(direction.equalsIgnoreCase("backwards"))
    {
        if(getHighlightedRegion().getLength()>0)
        {
            for(int i=startPos.getLineNumber(); i<endPos.getLineNumber()+1; i++)
                if(csdArray[i].substring(0, 1).equalsIgnoreCase("\t"))
                    csdArray.set(i, csdArray[i].substring(1));

            setAllText(csdArray.joinIntoString("\n"));
            highlightLines(startPos.getLineNumber(), endPos.getLineNumber());
        }
    }

}

void CabbageCodeEditorComponent::handleReturnKey ()
{
    if(autoCompleteListBox.isVisible() && autoCompleteListBox.getSelectedRow()!=-1)
    {
        const CodeDocument::Position pos1 = getDocument().findWordBreakBefore(getCaretPos());
        const CodeDocument::Position pos2 = getCaretPos();
        getDocument().deleteSection(pos1, pos2);
        insertText(variableNamesToShow[autoCompleteListBox.getSelectedRow()]);
        autoCompleteListBox.setVisible(false);
        return;
    }

    if(getSelectedText().length()>0)
    {
        CodeDocument::Position startPos(this->getDocument(), getHighlightedRegion().getStart());
        CodeDocument::Position endPos(this->getDocument(), getHighlightedRegion().getEnd());
        this->getDocument().deleteSection(startPos, endPos);
        insertNewLine("\n");
    }
    else
        insertNewLine("\n");

    scrollToKeepCaretOnScreen();
    autoCompleteListBox.setVisible(false);
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

//==============================================================================
void CabbageCodeEditorComponent::insertCode(int lineNumber, String codeToInsert, bool replaceExistingLine, bool shouldHighlight)
{
    StringArray csdLines;
    csdLines.addLines(getDocument().getAllContent());

    if(replaceExistingLine)
        csdLines.set(lineNumber, codeToInsert);
    else
        csdLines.insert(lineNumber, codeToInsert);

    getDocument().replaceAllContent(csdLines.joinIntoString("\n"));

    if(shouldHighlight)
        highlightLine(lineNumber);
}

void CabbageCodeEditorComponent::insertNewLine(String text)
{
    const CodeDocument::Position pos = getCaretPos();
    StringArray csdArray = getAllTextAsStringArray();
    String currentLine = csdArray[pos.getLineNumber()];
    int numberOfTabs=0;
    int numberOfSpaces=0;
    String tabs;
    while(currentLine.substring(numberOfTabs, numberOfTabs+1).equalsIgnoreCase("\t"))
    {
        tabs.append("\t", 8);
        numberOfTabs++;
    }

    while(currentLine.substring(numberOfSpaces, numberOfSpaces+1).equalsIgnoreCase(" "))
    {
        tabs.append(" ", 8);
        numberOfSpaces++;
    }

    getDocument().insertText(pos, text+tabs);
}
//==============================================================================
void CabbageCodeEditorComponent::highlightLine(int lineNumber)
{
    moveCaretTo(CodeDocument::Position (getDocument(), lineNumber, 0), false);
    moveCaretTo(CodeDocument::Position (getDocument(), lineNumber, 5000), true);
}

void CabbageCodeEditorComponent::highlightLines(int firstLine, int lastLine)
{
    moveCaretTo(CodeDocument::Position (getDocument(), firstLine, 0), false);
    moveCaretTo(CodeDocument::Position (getDocument(), lastLine, 5000), true);
}

//==============================================================================
String CabbageCodeEditorComponent::getSelectedText()
{
    String selectedText = getTextInRange(this->getHighlightedRegion());
    return selectedText;
}

const StringArray CabbageCodeEditorComponent::getAllTextAsStringArray()
{
    StringArray csdArray;
    csdArray.addLines(getDocument().getAllContent());
    return csdArray;
}

const String CabbageCodeEditorComponent::getAllText()
{
    return getDocument().getAllContent();
}

StringArray CabbageCodeEditorComponent::getSelectedTextArray()
{
    StringArray tempArray;
    String selectedText = getTextInRange(this->getHighlightedRegion());
    tempArray.addLines(selectedText);
    Logger::writeToLog(selectedText);
    return tempArray;
}