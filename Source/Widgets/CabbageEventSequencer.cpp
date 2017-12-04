/*
  Copyright (C) 2017 Rory Walsh

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

#include "CabbageEventSequencer.h"
#include "../Audio/Plugins/CabbagePluginEditor.h"

CabbageEventSequencer::CabbageEventSequencer (ValueTree wData, CabbagePluginEditor* _owner)
    : widgetData (wData),
      owner (_owner),
      seqContainer(),
      vp ("SequencerContainer")
{
    setName (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::name));
    widgetData.addListener (this);              //add listener to valueTree so it gets notified when a widget's property changes
    initialiseCommonAttributes (this, wData);   //initialise common attributes such as bounds, name, rotation, etc..
    addAndMakeVisible (vp);
    vp.setViewedComponent (&seqContainer);

    numRows = CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::matrixrows);
    numColumns = CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::matrixcols);
    arrangeTextEditors(wData);

    bpm  = 60 / CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::bpm) * 1000;
    previousBpm = bpm;

    //if(CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::active) == 1)
    //    startTimer(bpm);

    setColours(wData);
    updateCurrentStepPosition();

    var props = CabbageWidgetData::getProperty(wData, CabbageIdentifierIds::celldata);

    if (props.size()==3)
    {
        setCellData(int(props[0]), int(props[1]), props[2].toString());
    }

    owner->createEventMatrix(numColumns, numRows, getChannel());

}

CabbageEventSequencer::~CabbageEventSequencer()
{
    stopTimer();
    textFields.getUnchecked (0)->clear();
    textFields.clear();
}

void CabbageEventSequencer::arrangeTextEditors(ValueTree wData)
{
    const int width = CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::width);
    const int height = CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::height);
    int cellHeight = CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::cellheight);
    int cellWidth = CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::cellwidth);



    const int showNumbers = CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::showstempnumbers);
    createNumberLabels(wData, cellHeight, showNumbers);

    if(cellWidth == 0)
        seqContainer.setBounds (getLocalBounds().withHeight (numRows * cellHeight).withWidth(getLocalBounds().getWidth()));
    else
        seqContainer.setBounds (getLocalBounds().withHeight (numRows * cellHeight).withWidth(cellWidth*(numColumns+1)));

    if (height <= cellHeight * numRows)
    {
        if(cellWidth == 0)
        {
            vp.setScrollBarsShown (true, false);
            cellWidth = (width - vp.getScrollBarThickness() - (showNumbers > 0 ? numbersWidth : 0)) / numColumns;
        }
        else
        {
            vp.setScrollBarsShown (true, true);
            // cellWidth = cellWidth;//(width - vp.getScrollBarThickness() - (showNumbers > 0 ? numbersWidth : 0)) / numColumns;
        }
    }
    else
    {
        vp.setScrollBarsShown (false, false);
        cellWidth = (width - vp.getScrollBarThickness() - (showNumbers > 0 ? numbersWidth : 0)) / numColumns;

    }

    for (int i = 0 ; i < numColumns ; i++)
    {
        textFields.add (new OwnedArray<TextEditor>());

        for ( int y = 0 ; y < numRows ; y++)
        {
            TextEditor* tf = new TextEditor();
            seqContainer.addAndMakeVisible (tf);
            tf->getProperties().set ("Column", var (i));
            tf->getProperties().set ("Row", var (y));
            tf->addKeyListener (this);
            tf->setBounds ((showNumbers > 0 ? 20 : 0) + cellWidth * i, y * cellHeight, cellWidth, cellHeight);
            textFields[i]->add (tf);
        }
    }
}

void CabbageEventSequencer::createNumberLabels(ValueTree wData, int height, int showNumbers)
{
    if (showNumbers > 0)
    {
        for ( int i = 0 ; i < numRows ; i++)
        {

            Label* numberLabel = new Label ("Number" + String (i + 1), String (i + 1));

            if (i % showNumbers == 0)
            {
                numberLabel->setColour(Label::outlineColourId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::outlinecolour)));
                numberLabel->setColour(Label::backgroundColourId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::backgroundcolour)));
            }

            numberLabel->setBounds (0, i * height, numbersWidth, height);
            seqContainer.addAndMakeVisible (numberLabel);
            stepNumbers.add (numberLabel);
        }
    }
}

TextEditor* CabbageEventSequencer::getEditor (int column, int row)
{
    return textFields[column]->operator[] (row);
}

void CabbageEventSequencer::setColours(ValueTree wData)
{
    for(int x = 0 ; x < numColumns ; x++)
        for(int y = 0 ; y < numRows ; y++)
        {
            getEditor(x, y)->setColour(TextEditor::backgroundColourId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::backgroundcolour)));
            getEditor(x, y)->setColour(TextEditor::textColourId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::fontcolour)));
            getEditor(x, y)->setColour(TextEditor::highlightColourId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::activecellcolour)));
            getEditor(x, y)->setColour(TextEditor::outlineColourId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::outlinecolour)));
            getEditor(x, y)->setColour(CaretComponent::caretColourId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::fontcolour)));
        }

    for( int i = 0 ; i < stepNumbers.size(); i++)
    {
        stepNumbers[i]->setColour(Label::textColourId, Colour::fromString(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::textcolour)));
        if (i % int(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::showstempnumbers)) == 0)
        {
            stepNumbers[i]->setColour(Label::outlineColourId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::outlinecolour)));
            stepNumbers[i]->setColour(Label::backgroundColourId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::backgroundcolour)));
        }
    }
}

void CabbageEventSequencer::hiResTimerCallback()
{
//    owner->sendChannelDataToCsound (stepChannel.toUTF8(), currentBeat);
//    updateCurrentStepPosition();
//    if(bpm != previousBpm)
//    {
//        startTimer(bpm);
//        previousBpm = bpm;
//    }
//
//    currentBeat = (currentBeat < numRows - 1 ? currentBeat + 1 : 0);
}

void CabbageEventSequencer::updateCurrentStepPosition()
{
    //for ( int i = 0 ; i < numColumns ; i++)
    //    owner->sendScoreEventToCsound(getEditor (i, currentBeat)->getText().toUTF8());
        // owner->sendChannelStringDataToCsound (getEditor (i, currentBeat)->getProperties().getWithDefault ("Channel", ""), getEditor (i, currentBeat)->getText().toUTF8());

    const MessageManagerLock j;
    for ( int x = 0 ; x < numColumns ; x++)
        for ( int y = 0 ; y < numRows ; y++)
        {
            if( currentBeat == y)
                getEditor(x, y)->setColour(TextEditor::backgroundColourId, Colour::fromString (CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::highlightcolour)));
            else
                getEditor(x, y)->setColour(TextEditor::backgroundColourId, Colour::fromString (CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::backgroundcolour)));


            getEditor(x, y)->lookAndFeelChanged();
        }
}

void CabbageEventSequencer::setCurrentRow(int row)
{
//    const int restrictedRow = jlimit(0, numRows-1, row);
//    const int currentBeat = jlimit(0, numRows-1, row);
//
//    for ( int i = 0 ; i < numColumns ; i++)
//        owner->sendChannelStringDataToCsound (getEditor (i, currentBeat)->getProperties().getWithDefault ("Channel", ""), getEditor (i, currentBeat)->getText().toUTF8());
//
//    for ( int x = 0 ; x < numColumns ; x++)
//        for ( int y = 0 ; y < numRows ; y++)
//        {
//            if( restrictedRow == y)
//                getEditor(x, y)->setColour(TextEditor::backgroundColourId, Colour::fromString (CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::highlightcolour)));
//            else
//                getEditor(x, y)->setColour(TextEditor::backgroundColourId, Colour::fromString (CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::backgroundcolour)));
//
//            const MessageManagerLock j;
//            getEditor(x, y)->lookAndFeelChanged();
//        }
}

void CabbageEventSequencer::highlightEditorText (int col, int row)
{
    for ( int i = 0 ; i < numColumns ; i++)
        for ( int y = 0 ; y < numRows ; y++)
        {
            if (i == col && y == row)
            {
                getEditor (col, row)->setHighlightedRegion (Range<int> (0, getEditor (col, row)->getText().length()));
                vp.setViewPosition (0, (getEditor (col, row)->getY() > getHeight() / 2 ? getEditor (col, row)->getY() - getHeight() + getEditor (col, row)->getHeight() * 3 : 0));
            }
            else
                getEditor (i, y)->setHighlightedRegion (Range<int> (0, 0));
        }
}
void CabbageEventSequencer::resized()
{
    //need to resize children according to vp size..
    vp.setBounds (getLocalBounds());
}

void CabbageEventSequencer::setCellData(int col, int row, const String data)
{
    if(col<numColumns && row<numRows)
    {
        getEditor(col, row)->setText(data);
        getEditor(col, row)->setText(data);
        owner->setEventMatrixData(col, row, getChannel(), data);
    }


    //setMatrixEventSequencerCellData(int row, int col, String channel, String data)
}

bool CabbageEventSequencer::keyPressed (const KeyPress& key, Component* originatingComponent)
{
    if (TextEditor* ted = dynamic_cast<TextEditor*> (originatingComponent))
    {
        const int currentColumn = int (ted->getProperties().getWithDefault ("Column", 0));
        const int currentRow = int (ted->getProperties().getWithDefault ("Row", 0));

        if (key.getModifiers().isCtrlDown() && key.isKeyCode (KeyPress::rightKey) ||
            key.getModifiers().isCtrlDown() && key.isKeyCode (KeyPress::leftKey) ||
            key.isKeyCode (KeyPress::downKey) ||
            key.isKeyCode (KeyPress::upKey) ||
            key.isKeyCode (KeyPress::returnKey))
            swapFocusForEditors (key, currentColumn, currentRow);
    }

    return false;
}

void CabbageEventSequencer::swapFocusForEditors (KeyPress key, int col, int row)
{
    int newRow, newCol;

    if (key.getModifiers().isCtrlDown() && key.isKeyCode (KeyPress::rightKey))
    {
        newCol = (col < numColumns - 1 ? col + 1 : 0);
        newRow = row;
    }

    else if (key.getModifiers().isCtrlDown() && key.isKeyCode (KeyPress::leftKey))
    {
        newCol = (col > 0 ? col - 1 : numColumns - 1);
        newRow = row;
    }

    else if (key.isKeyCode (KeyPress::downKey))
    {
        newRow = (row < numRows - 1 ? row + 1 : 0);
        newCol = col;
    }

    else if (key.isKeyCode (KeyPress::upKey))
    {
        newRow = (row > 0 ? row - 1 : numRows - 1);
        newCol = col;
    }

    else if (key.isKeyCode (KeyPress::returnKey))
    {
        setCellData(col, row, getText());
        newRow = (row < numRows - 1 ? row + 1 : 0);
        newCol = col;
    }

    highlightEditorText (newCol, newRow);
    getEditor (newCol, newRow)->grabKeyboardFocus();

}

void CabbageEventSequencer::valueTreePropertyChanged (ValueTree& valueTree, const Identifier& prop)
{
    if(prop == CabbageIdentifierIds::value)
    {
        currentBeat = CabbageWidgetData::getNumProp(widgetData, CabbageIdentifierIds::value);
        updateCurrentStepPosition();

    }

    else if(prop == CabbageIdentifierIds::bpm)
        bpm = 60 / CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::bpm) * 1000;

    else if(prop == CabbageIdentifierIds::celldata)
    {
        var props = CabbageWidgetData::getProperty(valueTree, CabbageIdentifierIds::celldata);
        if (props.size()==3)
        {
            setCellData(int(props[0]), int(props[1]), props[2].toString());
            //CabbageUtilities::debug(props[2].toString());
        }
    }

    else if(prop == CabbageIdentifierIds::scrubberposition)
    {
        const int position = CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::scrubberposition);
        currentBeat = (position < 0 ? currentBeat : position);
        if(position >= 0)
            CabbageWidgetData::setNumProp (valueTree, CabbageIdentifierIds::scrubberposition, -1);
    }

    else if(prop == CabbageIdentifierIds::active)
    {
        handleCommonUpdates(this, valueTree);      //handle common updates such as bounds, alpha, rotation, visible, etc

        if(getActive()==0)
            stopTimer();
        else
        {
            currentBeat = 0;
            //startTimer(bpm);
        }
    }
    else
    {
        repaint();
        handleCommonUpdates(this, valueTree);      //handle common updates such as bounds, alpha, rotation, visible, etc
        setColours(valueTree);

    }
}

