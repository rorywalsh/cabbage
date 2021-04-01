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

#include "CabbageTextEditor.h"

#include "CabbageCsoundConsole.h"
#include "../Audio/Plugins/CabbagePluginEditor.h"

CabbageTextEditor::CabbageTextEditor (ValueTree wData, CabbagePluginEditor* _owner)
    : owner (_owner),
    widgetData (wData),
    textEditor (this),
    isMultiline (CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::wrap))
{
    setName (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::name));
    widgetData.addListener (this);              //add listener to valueTree so it gets notified when a widget's property changes
    initialiseCommonAttributes (this, wData);   //initialise common attributes such as bounds, name, rotation, etc..

    
    const int readOnly = CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::readonly);
    textEditor.setReadOnly(readOnly == 1 ? true : false);
    
    textEditor.toggleEditOnDoubleClick = CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::doubleclicktogglesedit);
    
    
    
    
    
    
    
    addAndMakeVisible (textEditor);
    textEditor.setMultiLine (isMultiline, false);
    textEditor.setScrollbarsShown (false);
    textEditor.addListener (this);
    textEditor.addKeyListener (this);
    textEditor.setColour (TextEditor::textColourId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::fontcolour)));
    textEditor.setColour (TextEditor::backgroundColourId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::colour)));
    textEditor.setColour (TextEditor::outlineColourId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::outlinecolour)));
    textEditor.setColour (TextEditor::focusedOutlineColourId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::outlinecolour)));
    textEditor.setColour (TextEditor::highlightColourId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::fontcolour)).contrasting (.5f));
    textEditor.setColour(CaretComponent::ColourIds::caretColourId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::caretcolour)));
    
    const String filename = CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::file);
    const File textFile(File::getCurrentWorkingDirectory().getChildFile(filename).getFullPathName());

    if (textFile.existsAsFile())
        textEditor.setText(textFile.loadFileAsString(), false);
    else
        textEditor.setText (getCurrentText(widgetData), dontSendNotification);

}

void CabbageTextEditor::textEditorReturnKeyPressed (TextEditor&)
{
    if (!isMultiline)
        sendTextToCsound();
    else
        textEditor.insertTextAtCaret ("\n");
}

void CabbageTextEditor::resized()
{
    textEditor.setBounds (getLocalBounds());
}
void CabbageTextEditor::sendTextToCsound()
{
    strings.add (textEditor.getText());
    strings.removeDuplicates (false);
    stringIndex = strings.size() - 1;
    setWidgetText (textEditor.getText());
    CabbageWidgetData::setStringProp (widgetData, CabbageIdentifierIds::text, getText());
    owner->sendChannelStringDataToCsound (getChannel(), textEditor.getText());
}

bool CabbageTextEditor::keyPressed (const juce::KeyPress& key, Component*)
{
    if (!isMultiline)
    {
        if (key.getTextDescription().contains ("cursor up"))
        {
            textEditor.setText (strings[jmax (0, stringIndex--)]);

            if (stringIndex < 1)
                stringIndex = 0;
        }
        else if (key.getTextDescription().contains ("cursor down"))
        {
            textEditor.setText (strings[jmin (strings.size() - 1, stringIndex++)]);

            if (stringIndex > strings.size() - 1)
                stringIndex = strings.size() - 1;
        }
    }
    else
    {
        if (key.getKeyCode() ==  KeyPress::returnKey && key.getModifiers().isCommandDown())
        {
            sendTextToCsound();
        }
    }

    return false;
}

void CabbageTextEditor::valueTreePropertyChanged (ValueTree& valueTree, const Identifier& prop)
{
    textEditor.setColour (TextEditor::textColourId, Colour::fromString (CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::fontcolour)));
    textEditor.setColour (TextEditor::backgroundColourId, Colour::fromString (CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::colour)));
    textEditor.setColour (TextEditor::outlineColourId, Colour::fromString (CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::outlinecolour)));
    textEditor.setColour (TextEditor::focusedOutlineColourId, Colour::fromString (CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::outlinecolour)));
    textEditor.setColour (TextEditor::highlightColourId, Colour::fromString (CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::fontcolour)).contrasting (.5f));
    lookAndFeelChanged();
    repaint();
    handleCommonUpdates (this, valueTree, false, prop);      //handle comon updates such as bounds, alpha, rotation, visible, etc
    
    const String filename = CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::file);
    const File textFile(File::getCurrentWorkingDirectory().getChildFile(filename).getFullPathName());
    
    if (textFile.existsAsFile())
        textEditor.setText(textFile.loadFileAsString(), false);
    else
        textEditor.setText (getCurrentText(valueTree), dontSendNotification);

    sendTextToCsound();
}
