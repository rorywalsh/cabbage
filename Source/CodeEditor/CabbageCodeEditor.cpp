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

#include "CabbageCodeEditor.h"
//#include "../Settings/CabbageSettings.h"
//#include "../Utilities/CabbageUtilities.h"
#include "../Application/CabbageDocumentWindow.h"

//==============================================================================
CabbageCodeEditorComponent::CabbageCodeEditorComponent (CabbageEditorContainer* owner, Component* statusBar, ValueTree valueTree, CodeDocument& document, CodeTokeniser* codeTokeniser)
    : CodeEditorComponent (document, codeTokeniser),
      valueTree (valueTree),
      statusBar (statusBar),
      owner (owner),
      autoCompleteListBox(),
      Thread ("parseVariablesThread"),
      debugLabel (""),
      currentLineMarker(),
      lookAndFeel3(),
      lookAndFeel3temp()
{
    //setMouseClickGrabsKeyboardFocus (true);
    String opcodeFile = File (File::getSpecialLocation (File::currentExecutableFile)).getParentDirectory().getFullPathName();
    opcodeFile += "/opcodes.txt";
    this->setLookAndFeel (&lookAndFeel3);
    setScrollbarThickness (20);

    addToGUIEditorPopup = new AddCodeToGUIEditorComponent (this, "Add to code repository", Colour (25, 25, 25));
    addToGUIEditorPopup->setVisible (false);

    if (File (opcodeFile).existsAsFile())
        setOpcodeStrings (File (opcodeFile).loadFileAsString());

    document.addListener (this);
    const Colour lineNumberBackground = CabbageSettings::getColourFromValueTree (valueTree, CabbageColourIds::lineNumberBackground, Colour (70, 70, 70));
    this->setColour (CodeEditorComponent::ColourIds::lineNumberBackgroundId, lineNumberBackground);
    this->setColour (CodeEditorComponent::ColourIds::lineNumberTextId, CabbageSettings::getColourFromValueTree (valueTree, CabbageColourIds::lineNumbers, Colours::white));

    autoCompleteListBox.setRowHeight (listBoxRowHeight);
    autoCompleteListBox.setModel (this);
    autoCompleteListBox.addKeyListener (this);
    this->addKeyListener (this);

    owner->addChildComponent (autoCompleteListBox);
    owner->addChildComponent (debugLabel);
    debugLabel.setColour (Label::backgroundColourId, Colours::whitesmoke);
    debugLabel.setFont (Font (String ("DejaVu Sans Mono"), 17, 0));

    currentLineMarker.setBounds (13, 0, 20, getFontSize());
    currentLineMarker.setColour (lineNumberBackground.contrasting().withAlpha (.3f));
    addAndMakeVisible (currentLineMarker);

}

CabbageCodeEditorComponent::~CabbageCodeEditorComponent()
{
    setLookAndFeel (nullptr);
}

void CabbageCodeEditorComponent::updateColourScheme (bool isCsdFile)
{
    struct Type
    {
        const char* name;
        uint32 colour;
    };

    setColour (CodeEditorComponent::ColourIds::lineNumberBackgroundId,
               CabbageSettings::getColourFromValueTree (valueTree, CabbageColourIds::lineNumberBackground,
                                                        Colour (70, 70, 70)));
    setColour (CodeEditorComponent::ColourIds::lineNumberTextId,
               CabbageSettings::getColourFromValueTree (valueTree, CabbageColourIds::lineNumbers, Colours::white));
    setColour (CodeEditorComponent::ColourIds::backgroundColourId,
               CabbageSettings::getColourFromValueTree (valueTree, CabbageColourIds::codeBackground, Colours::white));
    setColour (CaretComponent::ColourIds::caretColourId,
               CabbageSettings::getColourFromValueTree (valueTree, CabbageColourIds::caret, Colours::white));
    setColour (CodeEditorComponent::ColourIds::highlightColourId,
               CabbageSettings::getColourFromValueTree (valueTree, CabbageColourIds::selectTextBackground,
                                                        Colours::white));

    if (isCsdFile)
    {
        const Type types[] =
        {
            {
                "Error",             CabbageSettings::getColourFromValueTree (valueTree,
                CabbageColourIds::numbers,
                Colours::grey.darker()).getARGB()
            },
            {
                "Comment",           CabbageSettings::getColourFromValueTree (valueTree,
                CabbageColourIds::comment,
                Colours::grey.darker()).getARGB()
            },
            {
                "Keyword",           CabbageSettings::getColourFromValueTree (valueTree,
                CabbageColourIds::keyword,
                Colours::grey.darker()).getARGB()
            },
            {
                "Identifier",        CabbageSettings::getColourFromValueTree (valueTree,
                CabbageColourIds::identifierLiteral,
                Colours::grey.darker()).getARGB()
            },
            {"Integer",           Colours::grey.darker().getARGB()},
            {"Float",             Colours::grey.darker().getARGB()},
            {
                "String",            CabbageSettings::getColourFromValueTree (valueTree,
                CabbageColourIds::stringLiteral,
                Colours::grey.darker()).getARGB()
            },
            {"Operator",          Colours::grey.darker().getARGB()},
            {"Bracket",           Colours::grey.darker().getARGB()},
            {"Punctuation",       Colours::grey.darker().getARGB()},
            {"Preprocessor Text", Colours::grey.darker().getARGB()},
            {
                "Csd Tag",           CabbageSettings::getColourFromValueTree (valueTree,
                CabbageColourIds::csdtags,
                Colours::grey.darker()).getARGB()
            }
        };

        CodeEditorComponent::ColourScheme cs;

        for (std::size_t i = 0;
             i < sizeof (types) / sizeof (types[0]); ++i) // (NB: numElementsInArray doesn't work here in GCC4.2)
            cs.set (types[i].name, Colour (types[i].colour));

        this->setColourScheme (cs);
    }
    else
    {
        Colour background = CabbageSettings::getColourFromValueTree (valueTree, CabbageColourIds::lineNumberBackground,
                                                                     Colour (70, 70, 70));



        const Type types[] =
        {
            {
                "Error",             CabbageSettings::getColourFromValueTree (valueTree,
                CabbageColourIds::numbers,
                Colours::grey.darker()).getARGB()
            },
            {
                "Comment",           CabbageSettings::getColourFromValueTree (valueTree,
                CabbageColourIds::comment,
                Colours::grey.darker()).getARGB()
            },
            {"Keyword",           background.contrasting (.3f).getARGB()},
            {
                "Identifier",        CabbageSettings::getColourFromValueTree (valueTree,
                CabbageColourIds::identifierLiteral,
                Colours::grey.darker()).getARGB()
            },
            {"Integer",           background.contrasting (.8f).getARGB()},
            {"Float",             Colours::cornflowerblue.getARGB()},
            {
                "String",            CabbageSettings::getColourFromValueTree (valueTree,
                CabbageColourIds::stringLiteral,
                Colours::grey.darker()).getARGB()
            },
            {"Operator",          Colours::pink.darker().getARGB()},
            {"Bracket",           background.contrasting (.3f).getARGB()},
            {"Punctuation",       Colours::red.darker().getARGB()},
            {"Preprocessor Text", Colours::red.darker().getARGB()},
            {
                "Csd Tag",           CabbageSettings::getColourFromValueTree (valueTree,
                CabbageColourIds::csdtags,
                Colours::grey.darker()).getARGB()
            }
        };

        CodeEditorComponent::ColourScheme cs;

        for (std::size_t i = 0;
             i < sizeof (types) / sizeof (types[0]); ++i) // (NB: numElementsInArray doesn't work here in GCC4.2)
            cs.set (types[i].name, Colour (types[i].colour));

        this->setColourScheme (cs);
    }
}
//==============================================================================
void CabbageCodeEditorComponent::runInDebugMode()
{
    debugModeEnabled = true;
    startTimerHz (5);
}

void CabbageCodeEditorComponent::stopDebugMode()
{
    stopTimer();
    debugLabel.setVisible (false);
    debugModeEnabled = false;
}


bool CabbageCodeEditorComponent::isDebugModeEnabled()
{
    return debugModeEnabled;
}

void CabbageCodeEditorComponent::timerCallback()
{
    if (isDebugModeEnabled() == true)
    {
        MouseInputSource mouse = Desktop::getInstance().getMainMouseSource();
        Point<int> mousePos = getLocalPoint (nullptr, mouse.getScreenPosition()).toInt();

        CodeDocument::Position start, end;
        getDocument().findTokenContaining (getPositionAt (mousePos.x, mousePos.y), start, end);

        if (end.getPosition() > start.getPosition())
        {
            Range<int> selection (start.getPosition(), end.getPosition());

            const String token = getTextInRange (selection);
            const var value = findValueForCsoundVariable (token);

            if (!value.isVoid())
            {
                const String displayText = token + ":" + value.toString();
                debugLabel.setText (displayText, dontSendNotification);
                debugLabel.setVisible (true);
                debugLabel.toFront (true);
                debugLabel.setBounds (mousePos.getX(), mousePos.getY(), debugLabel.getFont().getStringWidth (displayText), 17);
                debugLabel.grabKeyboardFocus();
            }
            else
            {
                debugLabel.setVisible (false);
            }

        }
    }

}

var CabbageCodeEditorComponent::findValueForCsoundVariable (String varName)
{
    return breakpointData.getChildWithName ("Instrument1").getProperty (varName);
}
//==============================================================================
void CabbageCodeEditorComponent::updateCurrenLineMarker (ArrowKeys arrowKey)
{
    currentLineMarker.setVisible (true);
    const Range<int> highlight (getHighlightedRegion());
    CodeDocument::Position start;
    CodeDocument::Position linePos (getDocument(), getCaretPos().getPosition());
    bool draggingBackwards = false;

    if ( highlight.getLength() > 0)
    {
        linePos = CodeDocument::Position (getDocument(), highlight.getEnd());
        start = CodeDocument::Position (getDocument(), highlight.getStart());

        if (start.getPosition() < lastLinePosition)
        {
            linePos = CodeDocument::Position (getDocument(), highlight.getStart());
            currentLineMarker.setBounds (12, getCharacterBounds (linePos).getY(), 20, getFontSize());
            return;
        }

    }

    int xOffset = 0;

    if (arrowKey == ArrowKeys::Up)
        xOffset = -getFontSize();
    else if (arrowKey == ArrowKeys::Down)
        xOffset = getFontSize();

    currentLineMarker.setBounds (12, getCharacterBounds (linePos).getY() + xOffset, 20, getFontSize());
    lastLinePosition = start.getPosition();
}

//==============================================================================
int CabbageCodeEditorComponent::findText (String text, bool forwards, bool caseSensitive, bool skipCurrentSelection)
{
    const Range<int> highlight (getHighlightedRegion());
    const CodeDocument::Position startPos (getDocument(), skipCurrentSelection ? highlight.getEnd()
                                           : highlight.getStart());
    int lineNum = startPos.getLineNumber();
    int linePos = startPos.getIndexInLine();
    int index = -1;

    const int totalLines = getDocument().getNumLines();
    const String searchText (text);

    for (int linesToSearch = totalLines; --linesToSearch >= 0;)
    {
        String line (getDocument().getLine (lineNum));


        if (forwards)
        {
            index = caseSensitive ? line.indexOf (linePos, searchText)
                    : line.indexOfIgnoreCase (linePos, searchText);
        }
        else
        {
            if (linePos >= 0)
                line = line.substring (0, linePos);

            index = caseSensitive ? line.lastIndexOf (searchText)
                    : line.lastIndexOfIgnoreCase (searchText);
        }

        if (index >= 0)
        {
            const CodeDocument::Position p (getDocument(), lineNum, index);
            selectRegion (p, p.movedBy (searchText.length()));
            break;
        }

        if (forwards)
        {
            linePos = 0;
            lineNum = (lineNum + 1) % totalLines;
        }
        else
        {
            if (--lineNum < 0)
                lineNum = totalLines - 1;

            linePos = -1;
        }
    }

    return index;
}


void CabbageCodeEditorComponent::replaceText (String text, String replaceWith)
{
    Range<int> range = getHighlightedRegion();

    if (range.getLength() > 0)
    {
        getDocument().replaceSection (range.getStart(), range.getEnd(), replaceWith);
    }

}
//==============================================================================
void CabbageCodeEditorComponent::sendUpdateMessage (int lineNumber)
{
    allowUpdateOfPluginGUI = false;
    const StringArray csdArray = getAllTextAsStringArray();
    const int cabbageSectionClosingLineNumber = csdArray.indexOf ("</Cabbage>");

    if (allowUpdateOfPluginGUI && lineNumber < cabbageSectionClosingLineNumber)
    {
        //sendChangeMessage();
    }

}
//==============================================================================
// the update messages sent from these method could be plaaced in a timer callback if they
// start to make the editor less responsive...
void CabbageCodeEditorComponent::codeDocumentTextInserted (const String& text, int startIndex)
{
    handleAutoComplete (text);
    const String lineFromCsd = getDocument().getLine (getDocument().findWordBreakBefore (getCaretPos()).getLineNumber());
    displayOpcodeHelpInStatusBar (lineFromCsd);

    lastAction = "insertText";
    const CodeDocument::Position pos (getDocument(), startIndex);
    sendUpdateMessage (pos.getLineNumber());
}

void CabbageCodeEditorComponent::codeDocumentTextDeleted (int startIndex, int endIndex)
{
    const CodeDocument::Position endPos (getDocument(), endIndex);
    lastAction = "removeText";
    sendUpdateMessage (endPos.getLineNumber());
}

void CabbageCodeEditorComponent::insertTextAtCaret (const String& textToInsert)
{
    if (!columnEditMode)
        insertText (textToInsert);
    else
        insertMultiLineTextAtCaret (textToInsert);
}

void CabbageCodeEditorComponent::removeLine (int lineNumber)
{
    allowUpdateOfPluginGUI = true;
    moveCaretTo (CodeDocument::Position (getDocument(), lineNumber, 5000), false);
    moveCaretTo (CodeDocument::Position (getDocument(), lineNumber, 0), true);
    getDocument().replaceSection (getHighlightedRegion().getStart(), getHighlightedRegion().getEnd(), "");
    sendUpdateMessage (lineNumber);
}

void CabbageCodeEditorComponent::removeSelectedText()
{
    getDocument().replaceSection (getHighlightedRegion().getStart(), getHighlightedRegion().getEnd(), "");
}

void CabbageCodeEditorComponent::insertText (String text)
{
    if (this->isHighlightActive())
    {
        getDocument().replaceSection (getHighlightedRegion().getStart(), getHighlightedRegion().getEnd(), "");
    }

    getDocument().insertText (getCaretPos(), text);

    if (variableNamesToShow.size() > 0)
    {
        const int height = jmin (100, variableNamesToShow.size() * listBoxRowHeight);
        autoCompleteListBox.setBounds (getCaretRectangle().getX(), getCaretRectangle().getY() + 16, 300, height);
        autoCompleteListBox.toFront (true);
        autoCompleteListBox.selectRow (0);
    }
}

void CabbageCodeEditorComponent::insertMultiLineTextAtCaret (String text)
{
    //sendActionMessage("make popup invisible");
    StringArray csdArray = getAllTextAsStringArray();
    String curLine;
    CodeDocument::Position newPos, indexPos;

    const CodeDocument::Position startPos (getDocument(), getHighlightedRegion().getStart());
    const CodeDocument::Position endPos (getDocument(), getHighlightedRegion().getEnd());
    int indexInLine = startPos.getIndexInLine();


    for (int i = startPos.getLineNumber(); i < endPos.getLineNumber() + 1; i++)
    {
        while (csdArray[i].length() < indexInLine)
        {
            csdArray.getReference (i).append (" ", 1);
        }

        csdArray.set (i, csdArray[i].replaceSection (indexInLine, 0, text));
    }

    setAllText (csdArray.joinIntoString ("\n"));

    CodeDocument::Position newStartPos (getDocument(), startPos.getLineNumber(), indexInLine + text.length());
    CodeDocument::Position newEndPos (getDocument(), endPos.getLineNumber(), indexInLine + text.length());
    moveCaretTo (newStartPos, false);
    moveCaretTo (newEndPos, true);
}

//==============================================================================
void CabbageCodeEditorComponent::toggleComments()
{

    const CodeDocument::Position startPos (this->getDocument(), getHighlightedRegion().getStart());
    const CodeDocument::Position endPos (this->getDocument(), getHighlightedRegion().getEnd());


    StringArray selectedText;
    selectedText.addLines (getSelectedText());
    StringArray csdArray = getAllTextAsStringArray();

    bool isAlreadyCommented = false;

    for (int i = 0 ; i < commentedSections.size(); i++)
    {
        if (commentedSections[i] == Range<int> (startPos.getLineNumber(), endPos.getLineNumber()))
        {
            isAlreadyCommented = true;
            commentedSections.remove (i);
        }
    }

    if (isAlreadyCommented == false)
        commentedSections.add (Range<int> (startPos.getLineNumber(), endPos.getLineNumber()));

    for (int i = startPos.getLineNumber(); i <= endPos.getLineNumber(); i++)
    {
        String lineText = csdArray[i];

        if (isAlreadyCommented)
            csdArray.set (i, lineText.substring (1));
        else
            csdArray.set (i, ";" + lineText);

    }

    setAllText (csdArray.joinIntoString ("\n"));
    moveCaretTo (CodeDocument::Position (getDocument(), endPos.getLineNumber(), 1000), false);
}
//==============================================================================
void CabbageCodeEditorComponent::displayOpcodeHelpInStatusBar (String lineFromCsd)
{
    String opcodeHelpString;
    StringArray syntaxTokens, csdLineTokens;
    csdLineTokens.clear();
    csdLineTokens.addTokens (lineFromCsd, " ,\t", "");

    for (int i = 0; i < opcodeStrings.size(); i++)
    {
        opcodeHelpString = opcodeStrings[i];
        syntaxTokens.clear();
        syntaxTokens.addTokens (opcodeHelpString, ";", "\"");

        if (syntaxTokens.size() > 3)
            for (int x = 0; x < csdLineTokens.size(); x++)
            {
                if (syntaxTokens[0].removeCharacters ("\"") == csdLineTokens[x].trim())
                {
                    if (syntaxTokens[0].length() > 3)
                    {
                        if (CabbageEditorContainer::StatusBar* bar = dynamic_cast<CabbageEditorContainer::StatusBar*> (statusBar))
                            bar->setText (syntaxTokens);

                        x = csdLineTokens.size();
                        i = opcodeStrings.size();
                    }
                }
            }
    }
}

//==============================================================================
bool CabbageCodeEditorComponent::deleteBackwards (const bool moveInWholeWordSteps)
{
    const CodeDocument::Position startPos (getDocument(), getHighlightedRegion().getStart());
    const CodeDocument::Position endPos (getDocument(), getHighlightedRegion().getEnd());

    if (columnEditMode)
    {
        //sendActionMessage("make popup invisible");
        StringArray csdArray = getAllTextAsStringArray();
        String curLine;
        CodeDocument::Position newPos, indexPos;

        int indexInLine = startPos.getIndexInLine();

        for (int i = startPos.getLineNumber(); i < endPos.getLineNumber() + 1; i++)
        {
            while (csdArray[i].length() < indexInLine)
            {
                csdArray.getReference (i).append (" ", 1);
            }

            csdArray.set (i, csdArray[i].replaceSection (indexInLine - 1, 1, ""));
        }

        setAllText (csdArray.joinIntoString ("\n"));

        const CodeDocument::Position newStartPos (getDocument(), startPos.getLineNumber(), indexInLine - 1);
        const CodeDocument::Position newEndPos (getDocument(), endPos.getLineNumber(), indexInLine - 1);
        moveCaretTo (newStartPos, false);
        moveCaretTo (newEndPos, true);
    }
    else
    {

        if (moveInWholeWordSteps)
        {
            getDocument().deleteSection (getCaretPos().getPosition(), getCaretPos().getPosition() + 1);
            moveCaretTo (getDocument().findWordBreakBefore (getCaretPos()), true);
        }
        else if ( !isHighlightActive() )
        {
            CodeDocument::Position startPos = getCaretPos();
            startPos.moveBy (-1);
            getDocument().deleteSection (startPos.getPosition(), getCaretPos().getPosition());
        }
        else
        {
            getDocument().deleteSection (startPos.getPosition(), endPos.getPosition());
        }

        const CodeDocument::Position pos1 = getDocument().findWordBreakBefore (getCaretPos());
        const CodeDocument::Position pos2 = getDocument().findWordBreakAfter (getCaretPos());
        String currentWord = getDocument().getTextBetween (pos1, pos2);
        variableNamesToShow.clear();
        autoCompleteListBox.setVisible (false);

    }

    scrollToKeepCaretOnScreen();

    return true;
}

//==============================================================================
void CabbageCodeEditorComponent::mouseWheelMove (const MouseEvent& e, const MouseWheelDetails& mouse)
{
    if (e.mods.isCommandDown() && mouse.deltaY > 0)
        zoomIn();
    else if (e.mods.isCommandDown() && mouse.deltaY < 0)
        zoomOut();
    else
    {
        const int numberOfLinesToScroll = owner->settings->getUserSettings()->getIntValue ("numberOfLinesToScroll");

        if (mouse.deltaY < 0)
        {
            scrollBy (numberOfLinesToScroll);
            currentLineMarker.setTopLeftPosition (13, currentLineMarker.getY() - numberOfLinesToScroll * getFontSize());
            MouseWheelDetails w (mouse);
            w.deltaY = 0;
        }
        else
        {
            scrollBy (-numberOfLinesToScroll);
            currentLineMarker.setTopLeftPosition (13, currentLineMarker.getY() + numberOfLinesToScroll * getFontSize());
            MouseWheelDetails w (mouse);
            w.deltaY = 0;
        }
    }

    MouseWheelDetails w (mouse);
    w.deltaY = 0;
}

void CabbageCodeEditorComponent::zoomIn()
{
    setFont (this->getFont().withHeight (currentFontSize < 100 ? ++currentFontSize : 100));
    owner->settings->getUserSettings()->setValue("FontSize", currentFontSize);
}

void CabbageCodeEditorComponent::zoomOut()
{
    setFont (this->getFont().withHeight (currentFontSize > 8 ? --currentFontSize : 8));
    owner->settings->getUserSettings()->setValue("FontSize", currentFontSize);
}

//==============================================================================
void CabbageCodeEditorComponent::paintListBoxItem (int rowNumber, Graphics& g, int width, int height, bool rowIsSelected)
{
    if (rowIsSelected)
        g.fillAll (Colours::whitesmoke.darker (.2));
    else
        g.fillAll (Colours::whitesmoke);

    g.setFont (Font (String ("DejaVu Sans Mono"), 17, 0));
    g.setColour (Colour (20, 20, 20));
    g.drawFittedText (variableNamesToShow[rowNumber], Rectangle<int> (width, height), Justification::centredLeft, 0);
}
//==============================================================================
bool CabbageCodeEditorComponent::deleteForwards (const bool moveInWholeWordSteps)
{
    CodeDocument::Position startPos (getDocument(), getHighlightedRegion().getStart());
    CodeDocument::Position endPos (getDocument(), getHighlightedRegion().getEnd());

    if (columnEditMode)
    {
        //sendActionMessage("make popup invisible");
        StringArray csdArray = getAllTextAsStringArray();

        String curLine;
        CodeDocument::Position newPos, indexPos;

        int indexInLine = startPos.getIndexInLine();

        for (int i = startPos.getLineNumber(); i < endPos.getLineNumber() + 1; i++)
        {
            while (csdArray[i].length() < indexInLine)
            {
                csdArray.getReference (i).append (" ", 1);
            }

            csdArray.set (i, csdArray[i].replaceSection (indexInLine, 1, ""));
        }

        setAllText (csdArray.joinIntoString ("\n"));

        CodeDocument::Position newStartPos (getDocument(), startPos.getLineNumber(), indexInLine);
        CodeDocument::Position newEndPos (getDocument(), endPos.getLineNumber(), indexInLine);
        moveCaretTo (newStartPos, false);
        moveCaretTo (newEndPos, true);
    }

    else
    {
        if (moveInWholeWordSteps)
        {
            getDocument().deleteSection (getCaretPos().getPosition(), getCaretPos().getPosition() + 1);
            moveCaretTo (getDocument().findWordBreakBefore (getCaretPos()), true);
        }
        else if ( !isHighlightActive() )
        {
            startPos.moveBy (+1);
            getDocument().deleteSection (getCaretPos().getPosition(), startPos.getPosition());
        }
        else
        {
            getDocument().deleteSection (startPos.getPosition(), endPos.getPosition());
        }

        const CodeDocument::Position pos1 = getDocument().findWordBreakBefore (getCaretPos());
        const CodeDocument::Position pos2 = getDocument().findWordBreakAfter (getCaretPos());
        String currentWord = getDocument().getTextBetween (pos1, pos2);
        variableNamesToShow.clear();
        autoCompleteListBox.setVisible (false);

        if (currentWord.isNotEmpty())
        {
            showAutoComplete (currentWord);
        }
    }

    return true;
}
//==============================================================================
void CabbageCodeEditorComponent::parseTextForInstrumentsAndRegions()    //this is called on a separate thread..
{
    StringArray csdArray = this->getAllTextAsStringArray();
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

        else if ( csdArray[i].indexOf (";- Region:") != -1)
        {
            const String region = csdArray[i].replace (";- Region:", "");
            instrumentsAndRegions.set (region, i);
        }


        else if ((csdArray[i].indexOf ("instr ") != -1 || csdArray[i].indexOf ("instr	") != -1) &&
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

void CabbageCodeEditorComponent::parseTextForVariables()    //this is called on a separate thread..
{
    String csdText = getDocument().getAllContent();

    StringArray tokens;
    variableNames.clear();
    tokens.addTokens (csdText, "  \n( ) ` ~ ! @ # $ % ^ & * - + = | \\ { } [ ] : ; ' < > , . ? /\t", "");



    for (const String currentWord : tokens)
    {
        if (currentWord.startsWith ("a") || currentWord.startsWith ("i") ||
            currentWord.startsWith ("k") || currentWord.startsWith ("S") ||
            currentWord.startsWith ("f") || currentWord.startsWith ("\""))
        {
            if (currentWord.isNotEmpty())
                variableNames.addIfNotAlreadyThere (currentWord.replace ("\"", ""));
        }
    }
}

void CabbageCodeEditorComponent::handleAutoComplete (String text)
{
    if (owner->settings->getUserSettings()->getIntValue ("DisableAutoComplete"))
    {
        const CodeDocument::Position pos1 = getDocument().findWordBreakBefore (getCaretPos());
        const CodeDocument::Position pos2 = getDocument().findWordBreakAfter (getCaretPos());
        const String currentWord = getDocument().getTextBetween (pos1, pos2).trim();

        if (currentWord.startsWith ("a") || currentWord.startsWith ("i") ||
            currentWord.startsWith ("k") || currentWord.startsWith ("S") ||
            currentWord.startsWith ("f") || currentWord.startsWith ("g"))
        {
            if (text == " " && currentWord.isNotEmpty())
            {
                variableNames.addIfNotAlreadyThere (currentWord);
                autoCompleteListBox.updateContent();
            }
        }

        removeUnlikelyVariables (currentWord);
        autoCompleteListBox.setVisible (false);

        if (currentWord.isNotEmpty())
        {
            if (text != " ")
                showAutoComplete (currentWord);
        }
    }
}

void CabbageCodeEditorComponent::removeUnlikelyVariables (String currentWord)
{

    for ( int i = variableNamesToShow.size() ; i >= 0 ; i--)
    {
        if (variableNamesToShow[i].startsWith (currentWord) == false)
        {
            variableNamesToShow.removeString (variableNamesToShow[i]);
        }
    }

}

void CabbageCodeEditorComponent::showAutoComplete (String currentWord)
{
    for (const String item : variableNames)
    {
        if (item.startsWith (currentWord))
        {
            variableNamesToShow.addIfNotAlreadyThere (item.trim());
            autoCompleteListBox.updateContent();

            autoCompleteListBox.setVisible (true);
        }
    }
}
//===========================================================================================================
void CabbageCodeEditorComponent::mouseDown (const MouseEvent& e)
{
    if (e.mods.isPopupMenu())
    {
        PopupMenu m, subM;
        m.setLookAndFeel (&owner->getLookAndFeel());
        addPopupMenuItems (m, &e);
        m.addItem (10, "Add to code repository");

        StringArray codeSnippets = addItemsToPopupMenu (subM);
        m.addSubMenu ("Insert from code repository", subM);

        const int menuItemID = m.show();

        if (menuItemID == 4099)
            this->cutToClipboard();
        else if (menuItemID == 4100)
            this->copyToClipboard();
        else if (menuItemID == 4101)
            this->pasteFromClipboard();
        else if (menuItemID == 4098)
            this->deleteForwards (true);
        else if (menuItemID == 4102)
            this->selectAll();
        else if (menuItemID == 4104)
            this->undo();
        else if (menuItemID == 4105)
            this->redo();
        else if (menuItemID == 10)
            addToGUIEditorContextMenu();
        else if (menuItemID >= 100)
        {
            this->insertText (codeSnippets[menuItemID - 100]);
        }
    }
    else
        moveCaretTo (getPositionAt (e.x, e.y), e.mods.isShiftDown());
}
//===========================================================================================================
void CabbageCodeEditorComponent::AddCodeToGUIEditorComponent::textEditorReturnKeyPressed (TextEditor&)
{
    if (editor.getText() != "Enter name for GUI code")
    {
        ScopedPointer<XmlElement> repoXml;
        XmlElement* newEntryXml;
        repoXml = owner->owner->settings->getUserSettings()->getXmlValue ("CopeRepoXmlData");

        if (!repoXml)
            repoXml = new XmlElement ("CodeRepoXmlData");

        StringArray snippetNames;

        for ( int i = 0 ; i < repoXml->getNumAttributes() ; i++)
            snippetNames.add (repoXml->getAttributeName (i));

        if (snippetNames.contains (editor.getText()))
        {
            const int result = CabbageUtilities::showYesNoMessage ("Do you wish to overwrite the existing plant?",  &cabbageLoookAndFeel);

            if (result == 1)
            {
                repoXml->setAttribute (editor.getText(), owner->getSelectedText());
                owner->owner->settings->getUserSettings()->setValue ("CopeRepoXmlData", repoXml);
            }
            else
                CabbageUtilities::showMessage ("Nothing written to repository", &cabbageLoookAndFeel);
        }
        else
        {
            repoXml->setAttribute (editor.getText(), owner->getSelectedText());
            owner->owner->settings->getUserSettings()->setValue ("CopeRepoXmlData", repoXml);

        }

        repoXml = nullptr;
    }

    setVisible (false);
}

void CabbageCodeEditorComponent::addToGUIEditorContextMenu()
{
    addToGUIEditorPopup->setVisible (true);
    addToGUIEditorPopup->toFront (true);
}

StringArray CabbageCodeEditorComponent::addItemsToPopupMenu (PopupMenu& m)
{
    StringArray customCodeSnippets;
    ScopedPointer<XmlElement> repoXml;
    XmlElement* newEntryXml, *newEntryXml1;

    repoXml = owner->settings->getUserSettings()->getXmlValue ("CopeRepoXmlData");

    if (!repoXml)
        return StringArray ("");


    for ( int i = 0 ; i < repoXml->getNumAttributes() ; i++)
    {
        m.addItem ( 100 + i, repoXml->getAttributeName (i));
        customCodeSnippets.add (repoXml->getAttributeValue (i));
    }

    return customCodeSnippets;

}
//===========================================================================================================
bool CabbageCodeEditorComponent::keyPressed (const KeyPress& key, Component* originatingComponent)
{
    allowUpdateOfPluginGUI = true;      //allow keystrokes to update GUI

    if (key.getTextDescription().contains ("cursor up"))
    {
        updateCurrenLineMarker (ArrowKeys::Up);
    }
    else if (key.getTextDescription().contains ("cursor down"))
    {
        updateCurrenLineMarker (ArrowKeys::Down);
    }

    if (key == KeyPress ('z', ModifierKeys::commandModifier, 0))
        undoText();

    if (! CustomTextEditorKeyMapper<CodeEditorComponent>::invokeKeyFunction (*this, key))
    {

        if (key == KeyPress ('[', ModifierKeys::commandModifier, 0))
            zoomIn();
        else if (key == KeyPress (']', ModifierKeys::commandModifier, 0))
            zoomOut();

        if (key == KeyPress::returnKey)
            handleReturnKey();

        else if (key == KeyPress::escapeKey)
            handleEscapeKey();

        else  if (key.getModifiers().isCtrlDown() && key.isKeyCode (KeyPress::KeyPress::tabKey))
        {
            sendChangeMessage();
        }
        else  if (key.isKeyCode (KeyPress::upKey || key.isKeyCode (KeyPress::downKey)))
        {
            if (autoCompleteListBox.isVisible())
            {
                const int selectedRow = autoCompleteListBox.getSelectedRow();

                if (key.getTextDescription().contains ("cursor down"))
                    autoCompleteListBox.selectRow (jmax (0, selectedRow) + 1);
                else if (key.getTextDescription().contains ("cursor up"))
                    autoCompleteListBox.selectRow (jmax (0, selectedRow) - 1);

                autoCompleteListBox.scrollToEnsureRowIsOnscreen (autoCompleteListBox.getSelectedRow());
            }
            else if (key.isKeyCode (KeyPress::upKey))
                moveCaretUp (key.getModifiers().isShiftDown());
            else if (key.isKeyCode (KeyPress::downKey))
                moveCaretDown (key.getModifiers().isShiftDown());
        }

        else if (key.getTextCharacter() >= ' ')
        {
            insertTextAtCaret (String::charToString (key.getTextCharacter()));
            scrollToKeepCaretOnScreen();
        }
        else if (key.getKeyCode() ==  KeyPress::tabKey && key.getModifiers().isShiftDown())
            handleTabKey ("backwards");
        else if (key ==  KeyPress::tabKey)
            handleTabKey ("forwards");
        else
        {
            return false;
        }

    }

    updateCurrenLineMarker();
    //getParentComponent()->repaint();
    //handleUpdateNowIfNeeded();
    return true;
}
//==============================================================================
void CabbageCodeEditorComponent::handleTabKey (String direction)
{

    StringArray csdArray = getAllTextAsStringArray();
    const CodeDocument::Position startPos (this->getDocument(), getHighlightedRegion().getStart());
    const CodeDocument::Position endPos (this->getDocument(), getHighlightedRegion().getEnd());

    if (direction.equalsIgnoreCase ("forwards"))
    {
        if (getHighlightedRegion().getLength() == 0) //single line tab
        {
            insertTabAtCaret();
            return;
        }
        else
        {
            indentSelection();
        }
    }
    else if (direction.equalsIgnoreCase ("backwards"))
    {
        unindentSelection();
    }

    autoCompleteListBox.setVisible (false);
}

void CabbageCodeEditorComponent::handleEscapeKey()
{
    autoCompleteListBox.setVisible (false);
}

void CabbageCodeEditorComponent::handleReturnKey ()
{
    if (autoCompleteListBox.isVisible() && autoCompleteListBox.getSelectedRow() != -1)
    {
        const CodeDocument::Position pos1 = getDocument().findWordBreakBefore (getCaretPos());
        const CodeDocument::Position pos2 = getCaretPos();
        getDocument().deleteSection (pos1, pos2);

        insertText (variableNamesToShow[autoCompleteListBox.getSelectedRow()].trim());
        autoCompleteListBox.setVisible (false);
        return;
    }

    if (getSelectedText().length() > 0)
    {
        CodeDocument::Position startPos (this->getDocument(), getHighlightedRegion().getStart());
        CodeDocument::Position endPos (this->getDocument(), getHighlightedRegion().getEnd());
        this->getDocument().deleteSection (startPos, endPos);
        insertNewLine ("\n");
    }
    else
        insertNewLine ("\n");

    scrollToKeepCaretOnScreen();
    autoCompleteListBox.setVisible (false);
}
//==============================================================================
void CabbageCodeEditorComponent::undoText()
{
    CodeDocument::Position startPos = getCaretPos();
    //getDocument().undo();
    moveCaretTo (startPos, false);
}
//==============================================================================
const String CabbageCodeEditorComponent::getLineText (int lineNumber)
{
    const StringArray csdArray = getAllTextAsStringArray();
    return csdArray[lineNumber];
}

//==============================================================================
void CabbageCodeEditorComponent::insertCode (int lineNumber, String codeToInsert, bool replaceExistingLine, bool shouldHighlight)
{
    // This method is called when users move widgets around in GUI edit mode.
    // As the user is updating the plugin GUI, we don't need to, hence the
    // allowUpdateOfPluginGUI is set to false
    allowUpdateOfPluginGUI = false;

    StringArray csdLines;
    csdLines.addLines (getDocument().getAllContent());

    if (replaceExistingLine)
        csdLines.set (lineNumber, codeToInsert);
    else
        csdLines.insert (lineNumber, codeToInsert);

    getDocument().replaceAllContent (csdLines.joinIntoString ("\n"));

    if (shouldHighlight)
        highlightLine (lineNumber);
}

//==============================================================================
void CabbageCodeEditorComponent::updateBoundsText (int lineNumber, String codeToInsert, bool shouldHighlight)
{
    StringArray csdLines;
    csdLines.addLines (getDocument().getAllContent());
    const int currentIndexOfBounds = csdLines[lineNumber].indexOf("bounds");
    const int newIndexOfBounds = csdLines[lineNumber].indexOf("bounds");
    const String currentLine = csdLines[lineNumber];
    const String currentBounds = currentLine.substring(currentIndexOfBounds, currentLine.indexOf(currentIndexOfBounds, ")")+1);
    const String newBounds = codeToInsert.substring(newIndexOfBounds, codeToInsert.indexOf(newIndexOfBounds, ")")+1);
    
    if(currentIndexOfBounds == -1)
        csdLines.insert (lineNumber, codeToInsert);
    else
    {
        CabbageUtilities::debug(currentLine.replace(currentBounds, newBounds));
        csdLines.set (lineNumber, currentLine.replace(currentBounds, newBounds));
    }


    getDocument().replaceAllContent (csdLines.joinIntoString ("\n"));

    if (shouldHighlight)
        highlightLine (lineNumber);
}


StringArray CabbageCodeEditorComponent::getIdentifiersFromString (String code)
{
    StringArray tokens;

    const char* str = code.toUTF8().getAddress();

    do
    {
        const char* begin = str;

        while (*str != ')' && *str)
            str++;

        tokens.add (string (begin + 1, str + 1));
    }
    while (0 != *str++);

    return tokens;
}

void CabbageCodeEditorComponent::insertNewLine (String text)
{
    const CodeDocument::Position pos = getCaretPos();
    StringArray csdArray = getAllTextAsStringArray();
    String currentLine = csdArray[pos.getLineNumber()];
    int numberOfTabs = 0;
    int numberOfSpaces = 0;
    String tabs;

    while (currentLine.substring (numberOfTabs, numberOfTabs + 1).equalsIgnoreCase ("\t"))
    {
        tabs.append ("\t", 8);
        numberOfTabs++;
    }

    while (currentLine.substring (numberOfSpaces, numberOfSpaces + 1).equalsIgnoreCase (" "))
    {
        tabs.append (" ", 8);
        numberOfSpaces++;
    }

    getDocument().insertText (pos, text + tabs);
}
//==============================================================================
void CabbageCodeEditorComponent::highlightLine (int lineNumber)
{
    moveCaretTo (CodeDocument::Position (getDocument(), lineNumber, 5000), false);
    moveCaretTo (CodeDocument::Position (getDocument(), lineNumber, 0), true);
}

void CabbageCodeEditorComponent::highlightLines (int firstLine, int lastLine)
{
    moveCaretTo (CodeDocument::Position (getDocument(), lastLine, 5000), false);
    moveCaretTo (CodeDocument::Position (getDocument(), firstLine, 0), true);
}

//==============================================================================
String CabbageCodeEditorComponent::getSelectedText()
{
    String selectedText = getTextInRange (this->getHighlightedRegion());
    return selectedText;
}

const StringArray CabbageCodeEditorComponent::getAllTextAsStringArray()
{
    StringArray csdArray;
    csdArray.addLines (getDocument().getAllContent());
    return csdArray;
}

const String CabbageCodeEditorComponent::getAllText()
{
    return getDocument().getAllContent();
}

StringArray CabbageCodeEditorComponent::getSelectedTextArray()
{
    StringArray tempArray;
    String selectedText = getTextInRange (this->getHighlightedRegion());
    tempArray.addLines (selectedText);
    return tempArray;
}