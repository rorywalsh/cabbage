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

#include "CabbageStringSequencer.h"
#include "../Audio/Plugins/CabbagePluginEditor.h"

CabbageStringSequencer::CabbageStringSequencer (ValueTree wData, CabbagePluginEditor* _owner)
    : widgetData (wData),
      owner (_owner),
      seqContainer(),
      vp("SequencerContainer")
{
    setName (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::name));
    widgetData.addListener (this);              //add listener to valueTree so it gets notified when a widget's property changes
    initialiseCommonAttributes (this, wData);   //initialise common attributes such as bounds, name, rotation, etc..
    int cellWidth = 0;
    const int cellHeight = 25;
    addAndMakeVisible(vp);
    vp.setViewedComponent(&seqContainer);

    const int width = CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::width);
    const int height = CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::height);
    numRows = CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::numberofsteps);
    var channels = CabbageWidgetData::getProperty(wData, CabbageIdentifierIds::channel);

    if(channels.size()<2)
    {
        CabbageUtilities::showMessage("Make sure you have at least 2 channels declared for stringsequencer");
        return;
    }

    numColumns = channels.size()-1;
    stepChannel = channels[0];



    const int showNumbers = CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::showstempnumbers);
    if(showNumbers>0)
    {
        for( int i = 0 ; i < numRows ; i++)
        {

            Label* numberLabel = new Label("Number"+String(i+1), String(i+1));
            if(i%showNumbers==0)
                numberLabel->setColour(Label::textColourId, Colours::red);
            numberLabel->setBounds(0, i*cellHeight, 20, cellHeight);
            addAndMakeVisible(numberLabel);
            stepNumbers.add(numberLabel);
        }
    }

    if(height<cellHeight*numRows)
    {
        vp.setScrollBarsShown(true, false);
        cellWidth = (width-vp.getScrollBarThickness()-(showNumbers>0 ? 20 : 0))/numColumns;
    }
    else
    {
        vp.setScrollBarsShown(false, false);
        cellWidth = width-(showNumbers>0 ? 20 : 0)/numColumns;
    }

    for(int i = 0 ; i < numColumns ; i++)
    {
        textFields.add(new OwnedArray<TextEditor>());
        for( int y = 0 ; y < numRows ; y++)
        {
            TextEditor* tf = new TextEditor();
            seqContainer.addAndMakeVisible(tf);
            tf->setColour(TextEditor::outlineColourId, Colour(20, 20, 20));
            tf->getProperties().set("Column", var(i));
            tf->getProperties().set("Row", var(y));
            tf->getProperties().set("Channel", channels[i+1]);
            tf->addKeyListener(this);
            tf->setBounds((showNumbers>0 ? 20 : 0)+cellWidth*i, y*cellHeight, cellWidth, cellHeight);
            textFields[i]->add(tf);
        }
    }

    startTimer(60/CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::bpm)*1000);
    seqContainer.setBounds(getLocalBounds().withHeight(numRows*cellHeight));

}

CabbageStringSequencer::~CabbageStringSequencer()
{
    textFields.getUnchecked(0)->clear();
    textFields.clear();
}

TextEditor* CabbageStringSequencer::getEditor(int column, int row)
{
    return textFields[column]->operator[](row);
}

void CabbageStringSequencer::hiResTimerCallback()
{
    currentBeat = (currentBeat<numRows -1 ? currentBeat+1 : 0);
    owner->sendChannelDataToCsound(stepChannel.toUTF8(), currentBeat);
    for( int i = 0 ; i < numColumns ; i++)
        owner->sendChannelStringDataToCsound(getEditor(i, currentBeat)->getProperties().getWithDefault("Channel", ""), getEditor(i, currentBeat)->getText().toUTF8());
    //CabbageUtilities::debug(currentBeat);
}

void CabbageStringSequencer::highlightEditorText(int col, int row)
{
    for( int i = 0 ; i < numColumns ; i++)
        for( int y = 0 ; y < numRows ; y++)
        {
            if(i == col && y == row)
            {
                getEditor(col, row)->setHighlightedRegion(Range<int>(0, getEditor(col, row)->getText().length()));
                vp.setViewPosition(0, (getEditor(col, row)->getY()>getHeight()/2 ? getEditor(col, row)->getY()-getHeight()+getEditor(col, row)->getHeight()*3 : 0));
            }
            else
                getEditor(i, y)->setHighlightedRegion(Range<int>(0, 0));
        }
}
void CabbageStringSequencer::resized()
{
    //need to resize children according to vp size..
    vp.setBounds(getLocalBounds());
}


bool CabbageStringSequencer::keyPressed (const KeyPress &key, Component *originatingComponent)
{
    if(TextEditor* ted = dynamic_cast<TextEditor*>(originatingComponent))
    {
        const int currentColumn = int(ted->getProperties().getWithDefault("Column", 0));
        const int currentRow = int(ted->getProperties().getWithDefault("Row", 0));
        if(key.getModifiers().isCtrlDown() && key.isKeyCode (KeyPress::rightKey) ||
                key.getModifiers().isCtrlDown() && key.isKeyCode (KeyPress::leftKey) ||
                key.isKeyCode (KeyPress::downKey) ||
                key.isKeyCode (KeyPress::upKey) ||
                key.isKeyCode (KeyPress::returnKey))
        swapFocusForEditors(key, currentColumn, currentRow);
    }
}

void CabbageStringSequencer::swapFocusForEditors(KeyPress key, int col, int row)
{
    int newRow, newCol;
    if(key.getModifiers().isCtrlDown() && key.isKeyCode (KeyPress::rightKey))
    {
        newCol = (col < numColumns - 1 ? col + 1 : 0);
        newRow = row;
    }

    else if(key.getModifiers().isCtrlDown() && key.isKeyCode (KeyPress::leftKey))
    {
        newCol = (col > 0 ? col - 1 : numColumns - 1);
        newRow = row;
    }

    else if(key.isKeyCode (KeyPress::downKey))
    {
        newRow = (row < numRows - 1 ? row + 1 : 0);
        newCol = col;
    }

    else if(key.isKeyCode (KeyPress::upKey))
    {
        newRow = (row > 0 ? row - 1 : numRows - 1);
        newCol = col;
    }

    else if(key.isKeyCode (KeyPress::returnKey))
    {
        newRow = (row < numRows - 1 ? row + 1 : 0);
        newCol = col;
    }


    highlightEditorText(newCol, newRow);
    getEditor(newCol, newRow)->grabKeyboardFocus();

}

void CabbageStringSequencer::valueTreePropertyChanged (ValueTree& valueTree, const Identifier& prop)
{
    repaint();
    handleCommonUpdates (this, valueTree);      //handle comon updates such as bounds, alpha, rotation, visible, etc
}

