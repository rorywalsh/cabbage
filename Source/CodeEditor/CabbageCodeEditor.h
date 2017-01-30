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
#ifndef CABBAGECODEEDITOR_H_INCLUDED
#define CABBAGECODEEDITOR_H_INCLUDED

#include "../CabbageIds.h"
#include "CsoundTokeniser.h"
#include "../CabbageCommonHeaders.h"


class CabbageEditorContainer;

class CabbageCodeEditorComponent :
    public CodeEditorComponent,
    public CodeDocument::Listener,
    public ListBoxModel,
    public KeyListener,
    public Thread,
    public ChangeBroadcaster,
    public Timer
{
public:
    CabbageCodeEditorComponent(CabbageEditorContainer* owner, Component* statusBar, ValueTree valueTree, CodeDocument &document, CodeTokeniser *codeTokeniser);
    ~CabbageCodeEditorComponent() {};
    void updateColourScheme();
    CodeDocument::Position positionInCode;
    ValueTree valueTree;
    void codeDocumentTextDeleted(int,int);
    void codeDocumentTextInserted(const juce::String &,int);
    void displayOpcodeHelpInStatusBar(String lineFromCsd);
    const String getLineText(int lineNumber);

    bool keyPressed (const KeyPress &key, Component *originatingComponent) override;
    void undoText();

    String getSelectedText();
    const StringArray getAllTextAsStringArray();
    const String getAllText();
    StringArray getSelectedTextArray();
	
	class CurrentLineMarker : public Component
	{
	public: 
		CurrentLineMarker():Component(){}
		void paint(Graphics& g)
		{
			g.fillAll(Colours::transparentBlack);
			g.setColour(colour);
			g.fillRoundedRectangle(getLocalBounds().toFloat(), 2.f);
		}
		void setColour(Colour col){	colour = col;	}
		Colour colour;
		
	};
	
	CurrentLineMarker currentLineMarker;
	
    void run() // thread for parsing text for variables on startup
    {
        if(parseForVariables==true)
            parseTextForVariables();
        parseForVariables = false;
    };

	void updateCurrenLineMarker();

    void handleTabKey(String direction);
    void handleReturnKey();
    void handleEscapeKey();
	void mouseWheelMove (const MouseEvent& e, const MouseWheelDetails& mouse);
    void insertCode(int lineNumber, String codeToInsert, bool replaceExistingLine, bool highlightLine);
    void insertNewLine(String text);
    void insertTextAtCaret (const String &textToInsert);
    void insertMultiLineTextAtCaret (String text);
    void insertText(String text);
    void highlightLines(int firstLine, int lastLine);
    void highlightLine(int lineNumber);
    void toggleComments();
    void handleAutoComplete(String text);
    void showAutoComplete(String currentWord);
    void parseTextForVariables();
    void parseTextForInstrumentsAndRegions();
	void zoomIn();
	void zoomOut();
    bool deleteForwards (const bool moveInWholeWordSteps);
    bool deleteBackwards (const bool moveInWholeWordSteps);
    void sendUpdateMessage(int lineNumber);
    StringArray getIdentifiersFromString(String code);
	int findText(String text, bool forwards, bool caseSensitive, bool skipCurrentSelection);
	void replaceText(String text, String replaceWith);
    //=========================================================
    NamedValueSet instrumentsAndRegions;
    //=========================================================
    void timerCallback();
    ValueTree breakpointData;
    var findValueForCsoundVariable(String varName);
    bool debugModeEnabled = false;
    void runInDebugMode();
    void stopDebugMode();
    bool isDebugModeEnabled();
    Label debugLabel;
    //=========================================================
    void setAllText(String text){        	getDocument().replaceAllContent(text);    			}
    void setOpcodeStrings(String opcodes){  opcodeStrings.addLines(opcodes);    				}
    int getNumRows(){				        return variableNamesToShow.size();    				}
	bool hasFileChanged(){					return getDocument().hasChangedSinceSavePoint();	}
	void setSavePoint(){					getDocument().setSavePoint();						}
	int getFontSize(){						return currentFontSize;								}
	void setFontSize(int size){				currentFontSize = size;								}
	//=========================================================
	
    void listBoxItemDoubleClicked(int row, const MouseEvent &e) {};
    void paintListBoxItem (int rowNumber, Graphics& g, int width, int height, bool rowIsSelected);
    void selectedRowsChanged (int /*lastRowselected*/) {};
    String lastAction;
    bool allowUpdateOfPluginGUI = false;

private:
	int lastLinePosition = 0;
	int searchStartIndex  = 0;
    Component* statusBar;
    int listBoxRowHeight = 18;
    StringArray opcodeStrings;
    bool parseForVariables = true;
    bool columnEditMode = false;
    ListBox autoCompleteListBox;
    StringArray variableNamesToShow, variableNames;
    CabbageEditorContainer* owner;
    int updateGUICounter = 0;
	int currentFontSize = 17;
};

template <class CallbackClass>
struct CustomTextEditorKeyMapper
{
    /** Checks the keypress and invokes one of a range of navigation functions that
        the target class must implement, based on the key event.
    */
    static bool invokeKeyFunction (CallbackClass& target, const KeyPress& key)
    {
        const ModifierKeys& mods = key.getModifiers();

        const bool isShiftDown   = mods.isShiftDown();
        const bool ctrlOrAltDown = mods.isCtrlDown() || mods.isAltDown();

        int numCtrlAltCommandKeys = 0;
        if (mods.isCtrlDown())    ++numCtrlAltCommandKeys;
        if (mods.isAltDown())     ++numCtrlAltCommandKeys;

        if (key == KeyPress (KeyPress::downKey, ModifierKeys::ctrlModifier, 0) && target.scrollUp())   return true;
        if (key == KeyPress (KeyPress::upKey,   ModifierKeys::ctrlModifier, 0) && target.scrollDown()) return true;

#if JUCE_MAC
        if (mods.isCommandDown() && ! ctrlOrAltDown)
        {
            if (key.isKeyCode (KeyPress::upKey))        return target.moveCaretToTop (isShiftDown);
            if (key.isKeyCode (KeyPress::downKey))      return target.moveCaretToEnd (isShiftDown);
            if (key.isKeyCode (KeyPress::leftKey))      return target.moveCaretToStartOfLine (isShiftDown);
            if (key.isKeyCode (KeyPress::rightKey))     return target.moveCaretToEndOfLine   (isShiftDown);
        }

        if (mods.isCommandDown())
            ++numCtrlAltCommandKeys;
#endif

        if (numCtrlAltCommandKeys < 2)
        {
            if (key.isKeyCode (KeyPress::leftKey))  return target.moveCaretLeft  (ctrlOrAltDown, isShiftDown);
            if (key.isKeyCode (KeyPress::rightKey)) return target.moveCaretRight (ctrlOrAltDown, isShiftDown);

            if (key.isKeyCode (KeyPress::homeKey))  return ctrlOrAltDown ? target.moveCaretToTop         (isShiftDown)
                        : target.moveCaretToStartOfLine (isShiftDown);
            if (key.isKeyCode (KeyPress::endKey))   return ctrlOrAltDown ? target.moveCaretToEnd         (isShiftDown)
                        : target.moveCaretToEndOfLine   (isShiftDown);
        }

        if (numCtrlAltCommandKeys == 0)
        {
            if (key.isKeyCode (KeyPress::pageUpKey))    return target.pageUp   (isShiftDown);
            if (key.isKeyCode (KeyPress::pageDownKey))  return target.pageDown (isShiftDown);
        }

        if (key == KeyPress ('c', ModifierKeys::commandModifier, 0)
                || key == KeyPress (KeyPress::insertKey, ModifierKeys::ctrlModifier, 0))
            return target.copyToClipboard();

        if (key == KeyPress ('x', ModifierKeys::commandModifier, 0)
                || key == KeyPress (KeyPress::deleteKey, ModifierKeys::shiftModifier, 0))
            return target.cutToClipboard();

        if (key == KeyPress ('v', ModifierKeys::commandModifier, 0)
                || key == KeyPress (KeyPress::insertKey, ModifierKeys::shiftModifier, 0))
            return target.pasteFromClipboard();

        // NB: checking for delete must happen after the earlier check for shift + delete
        if (numCtrlAltCommandKeys < 2)
        {
            if (key.isKeyCode (KeyPress::backspaceKey)) return target.deleteBackwards (ctrlOrAltDown);
            if (key.isKeyCode (KeyPress::deleteKey))    return target.deleteForwards  (ctrlOrAltDown);
        }

        if (key == KeyPress ('a', ModifierKeys::commandModifier, 0))
            return target.selectAll();

        if (key == KeyPress ('z', ModifierKeys::commandModifier, 0))
            return target.undo();

        if (key == KeyPress ('y', ModifierKeys::commandModifier, 0)
                || key == KeyPress ('z', ModifierKeys::commandModifier | ModifierKeys::shiftModifier, 0))
            return target.redo();

        return false;
    }
};


#endif  // CABBAGECODEEDITOR_H_INCLUDED
