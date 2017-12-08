/*
  Copyright (C) 2009 Rory Walsh

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

#include "TableManager.h"

//==============================================================================
// Class to hold all tables
//==============================================================================
TableManager::TableManager(): Component(), zoom (0.0), largestTable (0), scrubberPosition (0),
    scrubberFreq (0), shouldShowTableButtons (true), shouldShowZoomButtons (true), tableIndex (0),
    mainFooterHeight (25), backgroundColour (CabbageUtilities::getDarkerBackgroundSkin()), scrollbarEnabled (true)
{
    addAndMakeVisible (zoomIn = new RoundButton ("zoomIn", Colours::white));
    addAndMakeVisible (zoomOut = new RoundButton ("zoomOut", Colours::white));
    //setOpaque(true);
    zoomIn->toFront (false);
    zoomIn->addChangeListener (this);
    zoomOut->toFront (false);
    zoomOut->addChangeListener (this);
}

//==============================================================================
void TableManager::paint (Graphics& g)
{
    g.fillAll (backgroundColour);
}
//==============================================================================
void TableManager::changeListenerCallback (ChangeBroadcaster* source)
{
    RoundButton* button = dynamic_cast<RoundButton*> (source);

    if (button)
    {
        if (button->getName() == "zoomIn")
        {
            zoom += 0.1;

            for (int i = 0; i < tables.size(); i++)
            {
                tables[i]->setZoomFactor (jmin (0.9, zoom));
            }
        }
        else if (button->getName() == "zoomOut")
        {
            //zoom out in one go....
            for (int i = 0; i < tables.size(); i++)
                tables[i]->setZoomFactor (0.0);

            zoom = 0.0;
        }
        else
        {
            for (int i = 0; i < tables.size(); i++)
                if (button->getName().getIntValue() == tables[i]->tableNumber)
                {
                    if (button->getMode() == 1)
                    {
                        tables[i]->setVisible (true);
                        button->setVisibilityStatus ("foreground");
                        tables[i]->setAlpha (1.f);

                        for (int c = 0; c < tableButtons.size(); c++)
                            if (button != tableButtons[c])
                            {
                                tableButtons[c]->setVisibilityStatus ("background");
                                button->setMode (0);
                            }

                        tables[i]->toFront (true);

                        if (tables[i]->genRoutine != 2)
                            tables[i]->getHandleViewer()->showHandles (true);
                    }
                    else if (button->getMode() == 0)
                    {
                        tables[i]->setVisible (false);
                        button->setVisibilityStatus ("off");
                    }

                }
                else
                {
                    tables[i]->getHandleViewer()->showHandles (false);
                    tables[i]->setAlpha (.5f);
                }

            bringButtonsToFront();
        }

        repaint();
    }
    else
    {
        if (GenTable* table = dynamic_cast<GenTable*> (source))
        {
            if (table->changeMessage == "updateScrollingPosition")
                if (table)
                {
                    for (int i = 0; i < tables.size(); i++)
                    {
                        if (table != tables[i])
                        {
                            float moveBy = table->getNewRangeStart() / table->visibleRange.getLength();
                            moveBy = tables[i]->visibleRange.getLength() * moveBy;
                            tables[i]->setRange (tables[i]->visibleRange.movedToStartAt (moveBy), true);
                        }
                    }

                }
        }
    }

}
//==============================================================================
void TableManager::addTable (int sr, const Colour col, int gen, var ampRange, int ftnumber, ChangeListener* listener)
{
    GenTable* table = new GenTable();
    table->tableNumber = ftnumber;
    table->addChangeListener (listener);
    table->addChangeListener (this);
    table->scrollbar->addListener (this);
    table->addChangeListener (listener);

    if (ampRange.size() == 0)
    {
        ampRange.insert (0, -1.f);
        ampRange.insert (1, 1.f);
        ampRange.insert (2, 0.f);
        ampRange.insert (3, 0.01);
    }

    table->addTable (sr, col, gen, ampRange);
    addAndMakeVisible (table);
    tables.add (table);
    RoundButton* button = new RoundButton (String (ftnumber), col);
    button->addChangeListener (this);
    addAndMakeVisible (button);
    tableButtons.add (button);
    resized();

    bringButtonsToFront();

}
//==============================================================================
void TableManager::setAmpRanges (var ampRange)
{
    if (ampRange.size() > 2)
    {
        if (int (ampRange[2]) == -1)
            for (int i = 0; i < tables.size(); i++)
            {
                tables[i]->setAmpRanges (ampRange);
            }
        else
        {
            //check for valid table
            if (getTableFromFtNumber (ampRange[2]) != nullptr)
                getTableFromFtNumber (ampRange[2])->setAmpRanges (ampRange);
        }
    }
}
//==============================================================================
void TableManager::setZoomFactor (double newZoom)
{
    for (int i = 0; i < tables.size(); i++)
    {
        if (newZoom < 0 || tables[i]->tableSize <= 2)
        {
            showZoomButtons (scrollbarEnabled == true ? false : false);
            tables[i]->showScrollbar (false);
            tables[i]->zoomButtonsOffset = 0;
            tables[i]->resized();
        }
        else
            tables[i]->setZoomFactor (newZoom);
    }
}

void TableManager::updateScrollbars()
{
    for (int i = 0; i < tables.size() - 1; i++)
    {
        tables[i]->showScrollbar (false);
    }

    tables[tables.size() - 1]->showScrollbar (true);
}

//==============================================================================
void TableManager::repaintAllTables()
{
    for (int i = 0; i < tables.size(); i++)
    {
        tables[i]->repaint();
    }
}
//==============================================================================
void TableManager::setGridColour (Colour col)
{
    for (int i = 0; i < tables.size(); i++)
    {
        tables[i]->gridColour = col;
        tables[i]->repaint();

        if (col.getAlpha() == 0x00)
            tables[i]->drawGrid = false;
    }
}

void TableManager::setBackgroundColour (Colour col)
{
    backgroundColour = col;
    //    for(int i=0; i<tables.size(); i++)
    //    {
    //        tables[i]->setBackgroundColour(col);
    //      if(col.getAlpha()==0xff)
    //          tables[i]->setOpaque(true);
    //    }
}

//==============================================================================
void TableManager::setTableColours (var colours)
{
    const Array<var>* coloursArray = colours.getArray();

    if (coloursArray)
    {
        for (int i = 0; i < tables.size(); i++)
        {
            tables[i]->setColour (Colour::fromString (coloursArray->getReference (i).toString()));
            tables[i]->repaint();
        }
    }
}
//==============================================================================
void TableManager::setRange (double start, double end)
{
    for (int i = 0; i < tables.size(); i++)
    {
        tables[i]->setSampleRange (start, end);
    }
}

void TableManager::setDrawMode (String mode)
{
    for (int i = 0; i < tables.size(); i++)
    {
        if (tables[i]->genRoutine == 2)
            tables[i]->drawAsVUMeter = true;
    }
}

void TableManager::setOutlineThickness (float thickness)
{
    for (int i = 0; i < tables.size(); i++)
    {
        tables[i]->setTraceThickness (thickness);
    }
}

void TableManager::setFill (bool fill)
{
    for (int i = 0; i < tables.size(); i++)
    {
        tables[i]->shouldFillTable (fill);
    }
}

void TableManager::setVUGradient (Array<Colour> colours)
{

    ColourGradient vuGradient (colours[0], 0.f, 0.f, colours[colours.size() - 1], getWidth(), getHeight(), false);

    for (int i = 1; i < colours.size() - 1; i++)
        vuGradient.addColour ((float)i / (float)colours.size(), colours[i]);

    for (int i = 0; i < tables.size(); i++)
    {
        tables[i]->setVUGradient (vuGradient);
    }

}
//==============================================================================
void TableManager::scrollBarMoved (ScrollBar* scrollBarThatHasMoved, double newRangeStart)
{
    ScrollBar* scroll = dynamic_cast<ScrollBar*> (scrollBarThatHasMoved);

    if (scroll)
    {
        for (int i = 0; i < tables.size(); i++)
        {
            float moveBy = newRangeStart / scrollBarThatHasMoved->getCurrentRange().getLength();
            moveBy = tables[i]->visibleRange.getLength() * moveBy;
            tables[i]->setRange (tables[i]->visibleRange.movedToStartAt (moveBy), true);
        }
    }
}

void TableManager::setScrubberPos (double pos, int ftnumber)
{
    if (ftnumber == -1 && tables.size()>0)
    {
        scrubberPosition = pos / tables[0]->tableSize;
        tables[0]->setScrubberPos (scrubberPosition);
    }

    else if (getTableFromFtNumber (ftnumber))
    {
        scrubberPosition = pos / getTableFromFtNumber (ftnumber)->tableSize;

        for (int i = 0; i < tables.size(); i++)
            tables[i]->setScrubberPos (scrubberPosition);
    }
}

void TableManager::timerCallback()
{
    scrubberPosition += scrubberFreq;//getLargestTable()->tableSize*scrubberFreq;

    for (int i = 0; i < tables.size(); i++)
        tables[i]->setScrubberPos (scrubberPosition);
}
//==============================================================================
void TableManager::resized()
{
    //if tables are stacked on each, do the math here...

    for (int i = 0; i < tables.size(); i++)
    {
        tables[i]->setBounds (0, 0, getWidth(), getHeight());
    }

    if (tables.size())
        tables[tables.size() - 1]->shouldDrawGrid (true);

    if (tableConfigList.size() == 1)
        shouldShowTableButtons = false;

    mainFooterHeight = (shouldShowZoomButtons == true ? 25 : 0);
    int ySpacing, yPos = 0, height;

    for (int i = 0; i < tableConfigList.size(); i++)
    {
        if (tableConfigList[i].size() > 0)
            for (int y = 0; y < tableConfigList[i].size(); y++)
            {
                int tableNumber = tableConfigList[i].getArray()->getReference (y);


                //if not the bottom table..
                if (getTableFromFtNumber (tableNumber) != nullptr)
                {
                    if (y != tableConfigList[i].size() - 1)
                    {
                        ySpacing = y * ((getHeight() - mainFooterHeight) / tableConfigList[i].size());
                        height = (getHeight() - mainFooterHeight) / tableConfigList[i].size();
                        getTableFromFtNumber (tableNumber)->showScrollbar (false);
                        getTableFromFtNumber (tableNumber)->mainFooterHeight = 0;
                        getTableFromFtNumber (tableNumber)->paintFooterHeight = 0;
                        yPos = ySpacing + height;
                    }
                    else
                    {
                        height = getHeight() - yPos - 5;
                        ySpacing = yPos;
                    }

                    getTableFromFtNumber (tableNumber)->setBounds (0, ySpacing, getWidth(), height);
                }
            }
    }

    bringButtonsToFront();
}

void TableManager::showZoomButtons (bool show)
{
    shouldShowZoomButtons = show;
    zoomIn->setVisible (show);
    zoomOut->setVisible (show);
    resized();
}

void TableManager::showScrollbar (bool show)
{
    scrollbarEnabled = false;

    for (int i = 0; i < tables.size(); i++)
    {
        tables[i]->showScrollbar (show);
    }
}

void TableManager::showTableButtons (bool show)
{
    shouldShowTableButtons = show;
}

void TableManager::configTableSizes (var intableConfigList)
{
    tableConfigList = intableConfigList;
    resized();
}
//==============================================================================
GenTable* TableManager::getTableFromFtNumber (int ftnumber)
{
    for (int i = 0; i < tables.size(); i++)
    {
        if (tables[i]->tableNumber == ftnumber)
            return tables[i];
    }

    //else return first table, which will be valid
    return tables[0];
}

//==============================================================================
GenTable* TableManager::getLargestTable()
{
    int size = 0, index = 0;

    for (int i = 0; i < tables.size(); i++)
    {
        if (tables[i]->tableSize > size)
        {
            size = tables[i]->tableSize;
            index = i;

        }
    }

    if (tables[index])
        return tables[index];

    return nullptr;
}
//==============================================================================
void TableManager::bringButtonsToFront()
{
    if (shouldShowZoomButtons == true)
    {
        zoomIn->setBounds (getWidth() - 43, getHeight() - 20, 20, 20);
        zoomIn->toFront (true);
        zoomOut->setBounds (getWidth() - 20, getHeight() - 20, 20, 20);
        zoomOut->toFront (true);
    }
    else
    {
        zoomIn->setVisible (false); // setEnabled(false);
        zoomOut->setVisible (false); //setEnabled(false);
    }

    for (int i = 0; i < tableButtons.size(); i++)
    {
        if (shouldShowTableButtons == true)
        {
            tableButtons[i]->setBounds (getWidth() - (shouldShowZoomButtons == true ? 65 : 20) - (i * 18), getHeight() - 18, 15, 15);
            tableButtons[i]->toFront (true);
        }
        else
            tableButtons[i]->setVisible (false);
    }
}

//==============================================================================
void TableManager::setWaveform (AudioSampleBuffer buffer, int ftNumber)
{
    for ( int i = 0; i < tables.size(); i++)
        if (ftNumber == tables[i]->tableNumber)
        {
            tables[i]->setWaveform (buffer);
            return;
        }
}

//==============================================================================
void TableManager::setFile (const File file)
{
    for ( int i = 0; i < tables.size(); i++)
        if (tables[i]->tableNumber == 0)
        {
            tables[i]->setFile (file);
            return;
        }
}

//==============================================================================
void TableManager::setWaveform (Array<float, CriticalSection> buffer, int ftNumber, bool updateRange)
{
    for ( int i = 0; i < tables.size(); i++)
        if (ftNumber == tables[i]->tableNumber)
        {
            tables[i]->setWaveform (buffer, updateRange);
            return;
        }
}
//==============================================================================
void TableManager::enableEditMode (StringArray pFields, int ftNumber)
{
    for ( int i = 0; i < tables.size(); i++)
        if (ftNumber == tables[i]->tableNumber)
            tables[i]->enableEditMode (pFields);
}
//==============================================================================
void TableManager::toggleEditMode (bool enable)
{
    for ( int i = 0; i < tables.size(); i++)
        tables[i]->getHandleViewer()->setVisible (enable);
}
//==============================================================================
void TableManager::bringTableToFront (int ftNumber)
{
    for ( int i = 0; i < tables.size(); i++)
    {

        if (shouldShowTableButtons == true)
            tables[i]->scrollbarReduction = (tables.size() * 20) + 50;
        else
            tables[i]->scrollbarReduction = (shouldShowZoomButtons == true ? 50 : 0);

        if (ftNumber == tables[i]->tableNumber)
        {
            tables[i]->toFront (true);
        }

        tables[i]->resized();
    }

    bringButtonsToFront();
}

//==============================================================================
// GenTable display  component
//==============================================================================
GenTable::GenTable():   thumbnailCache (5),
    currentPlayPosition (0),
    mouseDownX (0),
    mouseUpX (0),
    drawWaveform (false),
    regionWidth (1),
    loopLength (0),
    scrubberPosition (0),
    currentPositionMarker (new DrawableRectangle()),
    genRoutine (0),
    zoom (0.0),
    currentWidth (0),
    normalised (0),
    tableNumber (-1),
    showScroll (true),
    shouldScroll (true),
    mainFooterHeight (25),
    paintFooterHeight (15),
    quantiseSpace (0.01),
    qsteps (0),
    drawAsVUMeter (false),
    zoomButtonsOffset (10),
    drawGrid (false),
    shouldFill (true),
    vuGradient (Colours::yellow, 0.f, 0.f, Colours::red, getWidth(), getHeight(), false)
{
    thumbnail = nullptr;
    addAndMakeVisible (scrollbar = new ScrollBar (false));
    scrollbar->setRangeLimits (visibleRange);
    scrollbar->setAutoHide (false);
    scrollbar->addListener (this);
    addAndMakeVisible (currentPositionMarker);

    handleViewer = new HandleViewer();
    addAndMakeVisible (handleViewer);

    minMax.setStart (0);
    minMax.setEnd (0);
    handleViewer->minMax = minMax;

}
//==============================================================================
GenTable::~GenTable()
{
    scrollbar->removeListener (this);

    if (thumbnail)
        thumbnail->removeChangeListener (this);
}
//==============================================================================
void GenTable::addTable (int sr, const Colour col, int igen, var ampRange)
{
    sampleRate = sr;
    tableColour = col;
    currentPositionMarker->setFill (tableColour.brighter());
    genRoutine = abs (igen);
    handleViewer->handleViewerGen = igen;
    realGenRoutine = igen;
    handleViewer->colour = col;


    setAmpRanges (ampRange);

    //set up table according to type of GEN used to create it
    if (genRoutine == 1)
    {
        formatManager.registerBasicFormats();
        thumbnail = new AudioThumbnail (2, formatManager, thumbnailCache);
        thumbnail->addChangeListener (this);
        setZoomFactor (0.0);
    }
    else
        setBufferedToImage (true);


}
//==============================================================================
void GenTable::setAmpRanges (var ampRange)
{
    if (ampRange.size() > 2)
    {
        if (int (ampRange[2]) == tableNumber || int (ampRange[2]) == -1)
        {
            minMax.setStart (ampRange[0]);
            minMax.setEnd (ampRange[1]);
            handleViewer->minMax = minMax;
        }

        if (ampRange.size() > 3)
        {
            quantiseSpace = ampRange[3];

            qsteps = quantiseSpace / minMax.getEnd();

            if (qsteps == 1)
            {
                handleViewer->setShowingGrid (true);
                handleViewer->showHandles (false);
            }
        }
    }

    repaint();
}
//==============================================================================
void GenTable::changeListenerCallback (ChangeBroadcaster* source)
{
    currentHandle = dynamic_cast<HandleComponent*> (source);

    if (currentHandle)
    {
        //fill coordinate string and send change message to plugin editor to show bubble
        float curY = (float)jlimit (minMax.getStart(), minMax.getEnd(), pixelToAmp (thumbArea.getHeight() - zoomButtonsOffset, minMax, currentHandle->getY()));
        curY = CabbageUtilities::roundToMultiple (curY, quantiseSpace);


        coordinates = "";
        coordinates << roundToIntAccurate (currentHandle->xPosRelative * waveformBuffer.size()) <<
                    ", " << curY;


        //no need to update function table no movement has taken place
        if (currentHandle->mouseStatus != "mouseEnter")
        {
            changeMessage = "updateFunctionTable";
            sendChangeMessage();
        }
        else
        {
            changeMessage = "onMouseEnter";
            sendChangeMessage();

        }

    }
}
//==============================================================================
void GenTable::resized()
{
    if (!displayAsGrid())
        handleViewer->setSize (getWidth(), getHeight() - paintFooterHeight - zoomButtonsOffset);
    else
        handleViewer->setSize (getWidth(), getHeight());

    if (scrollbar)
    {
        if (showScroll)
            scrollbar->setBounds (getLocalBounds().withWidth (getWidth() - scrollbarReduction).removeFromBottom (mainFooterHeight - 5).reduced (2));
        else
            scrollbar->setBounds (-1000, 0, 100, 10);
    }
}

void GenTable::showScrollbar (bool show)
{
    showScroll = show;
    paintFooterHeight = (show == true ? 15 : 0);
    resized();
}


//==============================================================================

void GenTable::scrollBarMoved (ScrollBar* scrollBarThatHasMoved, double newRangeStart)
{
    //visibleRange = visibleRange.movedToStartAt (newRangeStart);
    //setRange (visibleRange.movedToStartAt (newRangeStart), true);
    //sendChangeMessage();
}

void GenTable::setFile (const File& file)
{
    if (file.existsAsFile())
    {
        genRoutine = 1;
        AudioFormatManager format;
        format.registerBasicFormats();

        AudioFormatReader* reader = format.createReaderFor (file);

        if (reader) //if a reader got created
        {
            AudioSampleBuffer buffer (reader->numChannels, reader->lengthInSamples);
            buffer.clear();
            buffer.setSize (reader->numChannels, reader->lengthInSamples);
            reader->read (&buffer, 0, buffer.getNumSamples(), 0, true, true);
            setWaveform (buffer);
        }

        delete reader;
    }

    repaint (0, 0, getWidth(), getHeight());
}

//==============================================================================
Array<double> GenTable::getPfields()
{
    Array<double> values;
    double prevXPos = 0, currXPos = 0, currYPos = 0;

    for (int i = 0; i < handleViewer->handles.size(); i++)
    {
        currYPos = handleViewer->handles[i]->yPosRelative * handleViewer->getHeight();

        if (genRoutine == 7 || genRoutine == 5)
        {
            currXPos = handleViewer->handles[i]->xPosRelative * waveformBuffer.size();

            values.add (jmax (0.0, ceil (currXPos - prevXPos)));
            //hack to prevent csound from bawking with a 0 in gen05
            float amp = pixelToAmp (handleViewer->getHeight(), minMax, currYPos);

            if (genRoutine == 5)
                amp = jmax (0.001f, amp);

            values.add (amp);


            prevXPos = roundToIntAccurate (handleViewer->handles[i]->xPosRelative * waveformBuffer.size());
        }
        else if (genRoutine == 2)
        {
            if (this->displayAsGrid())
            {
                int status = handleViewer->handles[i]->status == true ? 1 : 0;
                float amp =  status;

                values.add (round (amp));
            }
            else
            {
                float amp = pixelToAmp (handleViewer->getHeight(), minMax, currYPos);
                values.add (amp);
            }
        }
    }

    return values;
}

//==============================================================================
void GenTable::setWaveform (AudioSampleBuffer buffer)
{
    //we will deal with large tables as we would a GEN01 for efficiency
    if (genRoutine == 1 || buffer.getNumSamples() > MAX_TABLE_SIZE)
    {
        tableSize = buffer.getNumSamples();
        genRoutine = 1;
        thumbnail->clear();
        repaint();
        thumbnail->reset (buffer.getNumChannels(), 44100, buffer.getNumSamples());
        thumbnail->addBlock (0, buffer, 0, buffer.getNumSamples());
        const Range<double> newRange (0.0, thumbnail->getTotalLength());
        scrollbar->setRangeLimits (newRange);
        setRange (newRange);
        //setZoomFactor(zoom);
        repaint();
    }
}

void GenTable::setWaveform (Array<float, CriticalSection> buffer, bool updateRange)
{
    if (genRoutine != 1)
    {
        waveformBuffer.clear();
        waveformBuffer = buffer;

        //waveformBuffer.swapWith(buffer);
        tableSize = waveformBuffer.size();

        handleViewer->tableSize = tableSize;

        if (updateRange == true)
        {
            const Range<double> newRange (0.0, buffer.size() / sampleRate);
            scrollbar->setRangeLimits (newRange);
            setRange (newRange);
            setZoomFactor (0);
        }

        if (minMax.getLength() == 0)
        {
            minMax = findMinMax (buffer);
            handleViewer->minMax = minMax;
        }

        //if(genRoutine==2 && qsteps==1)
        //  drawGridImage(true, getWidth(), getHeight(), 0.0);

        repaint();
    }

}
//==============================================================================
void GenTable::enableEditMode (StringArray m_pFields)
{
    //turns on edit mode by adding handles to the handleViewer

    //only assign original pfields
    if (m_pFields.size() > 1)
        pFields = m_pFields;

    //only enable editing on non normalising tables
    if (realGenRoutine >= 0)
        return;

    Array<float, CriticalSection> pFieldAmps;
    pFieldAmps.add (pFields[5].getFloatValue());

    for (int i = 6; i < pFields.size(); i += 2)
        pFieldAmps.add (pFields[i + 1].getFloatValue());

    Range<float> pFieldMinMax = findMinMax (pFieldAmps);
    normalised = pFields[4].getIntValue();
    double xPos = 0;
    handleViewer->handles.clear();
    const double thumbHeight = getHeight() - paintFooterHeight - zoomButtonsOffset;
    int pfieldCount = 0;


    if (pFields.size() > 0)
    {
        const double width = (double (getWidth()) / (double)tableSize);

        if (genRoutine == 7 || genRoutine == 5)
        {
            float pFieldAmpValue = (normalised < 0 ? pFields[5].getFloatValue() : pFields[5].getFloatValue() / pFieldMinMax.getEnd());
            const float amp = ampToPixel (thumbHeight, minMax, pFieldAmpValue);
            handleViewer->addHandle (0, ampToPixel (thumbHeight, minMax, pFieldAmpValue), (width > 10 ? width : 15), (width > 10 ? 5 : 15), this->tableColour);

            for (int i = 6; i < pFields.size(); i += 2)
            {
                xPos = xPos + pFields[i].getFloatValue();
                pFieldAmpValue = (normalised < 0 ? pFields[i + 1].getFloatValue() : pFields[i + 1].getFloatValue() / pFieldMinMax.getEnd());
                handleViewer->addHandle (xPos / (double)waveformBuffer.size(), ampToPixel (thumbHeight, minMax, pFieldAmpValue), (width > 10 ? width : 15), (width > 10 ? 5 : 15), this->tableColour);
            }

            handleViewer->fixEdgePoints (genRoutine);
        }
        else if (genRoutine == 2)
        {
            float pFieldAmpValue = (normalised < 0 ? pFields[5].getFloatValue() : pFields[5].getFloatValue() / pFieldMinMax.getEnd());
            handleViewer->addHandle (0, ampToPixel (thumbHeight, minMax, pFieldAmpValue), width + 1, 5, this->tableColour, pFieldAmpValue == 1 ? true : false);

            for (double i = 6; i < pFields.size(); i++)
            {
                pfieldCount++;
                xPos = (i - 5.0) / (double (tableSize)) * tableSize;
                pFieldAmpValue = (normalised < 0 ? pFields[i].getFloatValue() : pFields[i].getFloatValue() / pFieldMinMax.getEnd());
                handleViewer->addHandle (xPos / tableSize, ampToPixel (thumbHeight, minMax, pFieldAmpValue), width + 1, 5, this->tableColour, pFieldAmpValue == 1 ? true : false);
            }

            //initialise all remaining points in the table if user hasn't
            for (double i = pfieldCount; i < tableSize; i++)
            {
                xPos = (i + 1) / (double (tableSize)) * tableSize;
                pFieldAmpValue = pFieldMinMax.getEnd();
                handleViewer->addHandle (xPos / tableSize, ampToPixel (thumbHeight, minMax, pFieldAmpValue), width + 1, 5, this->tableColour, false);
            }

            handleViewer->fixEdgePoints (genRoutine);
            handleViewer->showHandles (false);
        }

    }

}

//==============================================================================
void GenTable::setZoomFactor (double amount)
{
    //set zoom factor, between 0 and 1
    zoom = amount;

    if (genRoutine == 1)
    {
        if (thumbnail->getTotalLength() > 0)
        {
            const double newScale = jmax (0.001, thumbnail->getTotalLength() * (1.0 - jlimit (0.0, 0.99, amount)));
            const double timeAtCentre = xToTime (getWidth() / 2.0f);

            if (amount != 0)
            {
                setRange (Range<double> (timeAtCentre - newScale * 0.5, timeAtCentre + newScale * 0.5));
                setRange (Range<double> (timeAtCentre - newScale * 0.5, timeAtCentre + newScale * 0.5));

            }
            else
                setRange (Range<double> (0, thumbnail->getTotalLength()));
        }
    }
    else
    {
        if (visibleRange.getLength() == 0)
            visibleRange.setLength (waveformBuffer.size() / sampleRate);

        const double newScale = jmax (0.00001, waveformBuffer.size() / sampleRate * (1.0 - jlimit (0.0, 0.9999, amount)));
        const double timeAtCentre = xToTime (getWidth() / 2.0f);

        if (amount != 0)
            setRange (Range<double> (timeAtCentre - newScale * 0.5, timeAtCentre + newScale * 0.5));
        else
            setRange (Range<double> (0, waveformBuffer.size() / sampleRate));
    }

    repaint();
}

//==============================================================================
const Image GenTable::drawGridImage (bool redraw, double width, double height, double offset)
{
    if (redraw == true)
    {
        Image gridImage (Image::RGB, width, height, true);
        Graphics g (gridImage);
        const double widthOfGridElement = width / waveformBuffer.size();
        //g.fillAll(Colours::transparentBlack);
        //g.setColour(Colours::red);
        //draw grid image
        for (double i = 0; i < waveformBuffer.size(); i++)
        {
            g.drawImageAt (CabbageLookAndFeel2::drawToggleImage (widthOfGridElement - 3.f,
                                                                 height,
                                                                 (waveformBuffer[i] > 0.0 ? true : false),
                                                                 (waveformBuffer[i] > 0.0 ? tableColour : backgroundColour),
                                                                 true,
                                                                 4.f),
                           i * (widthOfGridElement) + 2,
                           1.f);
        }


        //g.strokePath(p, PathStrokeType(1));
        //return a clipped image
        Rectangle<int> clippedImage (offset * -1, 0, width, height);
        return gridImage.getClippedImage (clippedImage);
    }
    else
    {
        return Image();
    }

    return Image();
}

//==============================================================================
void GenTable::setSampleRange (double start, double end)
{
    if (genRoutine != 1)
    {
        setRange (Range<double> (start / sampleRate, end / sampleRate));
    }
}
//==============================================================================
void GenTable::mouseWheelMove (const MouseEvent&, const MouseWheelDetails& wheel)
{
    /*
    if(genRoutine==1)
    {
        if (thumbnail->getTotalLength() > 0.0)
        {
            double newStart = visibleRange.getStart() - wheel.deltaX * (visibleRange.getLength()) / 10.0;
            newStart = jlimit (0.0, jmax (0.0, thumbnail->getTotalLength() - (visibleRange.getLength())), newStart);
            setRange (Range<double> (newStart, newStart + visibleRange.getLength()));
            repaint();
        }
    }
    else
    {
            double newStart = visibleRange.getStart() - wheel.deltaX * (visibleRange.getLength()) / 10.0;
            newStart = jlimit (0.0, jmax (0.0, thumbnail->getTotalLength() - (visibleRange.getLength())), newStart);
            setRange (Range<double> (newStart, newStart + visibleRange.getLength()));
            repaint();
    }
     */
}

//==============================================================================
void GenTable::setRange (Range<double> newRange, bool isScrolling)
{
    visibleRange = newRange;

    if (newRange.getLength() > 0)
    {
        //set visible ranges in samples...
        scrollbar->setCurrentRange (visibleRange, dontSendNotification);

        if (genRoutine != 1)
        {


            visibleStart = visibleRange.getStart() * sampleRate;

            visibleEnd = visibleRange.getEnd() * sampleRate;

            visibleLength = visibleRange.getLength() * sampleRate;


            if (!isScrolling)
            {
                double newWidth = double (getWidth()) * (double (waveformBuffer.size()) / visibleLength);
                double leftOffset = newWidth * (visibleStart / (double)waveformBuffer.size());
                handleViewer->setSize (newWidth, handleViewer->getHeight());
                handleViewer->setTopLeftPosition (-leftOffset, 0);
            }
            else
            {
                double leftOffset = handleViewer->getWidth() * (visibleStart / (double)waveformBuffer.size());
                handleViewer->setTopLeftPosition (-leftOffset, 0);
            }

        }
    }

    repaint();
}
//==============================================================================
void GenTable::paint (Graphics& g)
{
    g.fillAll (backgroundColour);
    //set thumbArea, this is the area of the painted image
    thumbArea = getLocalBounds();
    thumbArea.setHeight (getHeight() - paintFooterHeight);
    float prevY = 0, prevX = 0, currY = 0, currX = 0;
    const bool interp = (getWidth() < tableSize ? true : false);
    const double thumbHeight = thumbArea.getHeight() - (showScroll == true ? 10 : 0); //scrollbar thickness
    numPixelsPerIndex = ((double)thumbArea.getWidth() / visibleLength);
    Path vuPath;

    //don't draw a grid when the table itself is a grid
    if (drawGrid == true && qsteps != 1)
    {
        g.setColour (gridColour);
        //g.drawImageAt(backgroundImage, 0, 0, true);
        const double divisors = (getWidth() > 300 ? 20.0 : 10.0);

        for (float i = 0; i < getWidth(); i += (interp ? getWidth() / divisors : numPixelsPerIndex))
            g.drawVerticalLine (i + 1, 0, thumbHeight);

        g.drawVerticalLine (getWidth() - 1, 0, thumbHeight);

        for (double i = 0; i <= thumbHeight; i += (getHeight() + 2.0) / divisors)
            g.drawHorizontalLine (i, 1, getWidth());

        g.drawHorizontalLine (thumbHeight - .5, 1, getWidth());
    }

    //if gen01 then use an audio thumbnail class
    if (genRoutine == 1 || waveformBuffer.size() > MAX_TABLE_SIZE)
    {
        g.setColour (tableColour);
        thumbnail->drawChannels (g, thumbArea.reduced (2), visibleRange.getStart(), visibleRange.getEnd(), .8f);
        g.setColour (tableColour.contrasting (.5f).withAlpha (.7f));
        float zoomFactor = thumbnail->getTotalLength() / visibleRange.getLength();
        regionWidth = (regionWidth == 2 ? 2 : regionWidth * zoomFactor);
    }
    //else draw the waveform directly onto this component
    //edit handles get placed on the handleViewer, which is placed on top of this component
    else
    {
        vuPath.startNewSubPath (0, thumbArea.getHeight() + 5.f);

        //if drawing VU meter then we don't need a high resolution for the drawing.
        float incr = (tableSize <= 2 ? 1 : visibleLength / ((double)thumbArea.getWidth()));
        prevY = ampToPixel (thumbHeight, minMax, waveformBuffer[0]);
        float midPoint;

        if (genRoutine == 7 || genRoutine == 5 || genRoutine == 2 || genRoutine == 27)
        {
            midPoint = ampToPixel (thumbHeight, minMax, minMax.getStart());
        }

        else
            midPoint = ampToPixel (thumbHeight, minMax, minMax.getLength() / 2.f - minMax.getEnd());

        int gridIndex = ceil (visibleStart);

        for (double i = visibleStart; i <= visibleEnd; i += incr)
        {
            //when qsteps == 1 we draw a grid
            if (qsteps == 1)
            {
                if (CabbageUtilities::compDouble (i, gridIndex))
                {
                    gridIndex++;
                    g.drawImageAt (drawGridImage (true, handleViewer->getWidth(), thumbHeight - 4, handleViewer->getX()), 0, 0, false);
                }
            }

            else
            {
                //minMax is the range of the current waveforms amplitude
                float testSample = waveformBuffer[i];
                currY = ampToPixel (thumbHeight, minMax, waveformBuffer[i]);

                if (tableSize <= 2)
                {
                    //if table is size of two or less draw as a VU meter using a path.
                    vuPath.addRectangle (prevX, prevY, prevX + numPixelsPerIndex, thumbHeight);
                    //vuPath.lineTo(prevX+numPixelsPerIndex, prevY);
                }
                else
                {
                    if (shouldFill)
                    {
                        g.setColour (tableColour);
                        g.drawVerticalLine (prevX, (prevY < midPoint ? prevY : midPoint),  (prevY > midPoint ? prevY : midPoint));
                    }

                    if (traceThickness > 0)
                    {
                        g.setColour (tableColour);
                        //draw trace
                        currX = jmax (0.0, (i - visibleStart) * numPixelsPerIndex);
                        g.drawLine (prevX, prevY, currX, currY, traceThickness);
                    }
                }


                prevX = jmax (0.0, (i - visibleStart) * numPixelsPerIndex);
                prevY = currY;
            }
        }
    }

    vuPath.lineTo (prevX, thumbArea.getHeight());
    vuPath.closeSubPath();

    if (tableSize <= 2)
    {
        g.setGradientFill (vuGradient);
        g.fillPath (vuPath);
    }

}

//==============================================================================
float GenTable::ampToPixel (int height, Range<float> minMax, float sampleVal)
{
    //caluclate amp value based on pixel...
    float amp =  (sampleVal - minMax.getStart()) / minMax.getLength();
    return jmax (0.f, ((1 - amp) * height));
}

float GenTable::pixelToAmp (int height, Range<float> minMax, float pixelY)
{
    //caluclate pixel value based on amp...
    float amp =  ((1 - (pixelY / height)) * minMax.getLength()) + minMax.getStart();
    return amp;
}
//==============================================================================
void GenTable::mouseDown (const MouseEvent& e)
{
    if (!e.mods.isPopupMenu())
    {
        if (genRoutine == 1)
        {
            regionWidth = (1.01 - zoom) * 1.5;
            currentPlayPosition = jmax (0.0, xToTime ((float) e.x));
            loopStart = e.x;
            loopLength =  0;
            repaint();
            sendChangeMessage();
        }
    }
}
//==============================================================================
void GenTable::mouseEnter (const MouseEvent& e)
{

}
//==============================================================================
void GenTable::mouseExit (const MouseEvent& e)
{

}
//==============================================================================
void GenTable::mouseDrag (const MouseEvent& e)
{
    if (genRoutine == 1)
    {
        //draw rectangle when users select audio in a gen01 table...
        if (this->getLocalBounds().contains (e.getPosition()))
        {
            if (e.mods.isLeftButtonDown())
            {
                double zoomFactor = visibleRange.getLength() / thumbnail->getTotalLength();
                regionWidth = abs (e.getDistanceFromDragStartX()) * zoomFactor;

                if (e.getDistanceFromDragStartX() < 0)
                    currentPlayPosition = jmax (0.0, xToTime (loopStart + (float)e.getDistanceFromDragStartX()));

                float widthInTime = ((float)e.getDistanceFromDragStartX() / (float)getWidth()) * (float)thumbnail->getTotalLength();
                loopLength = jmax (0.0, widthInTime * zoomFactor);
            }

            repaint();
        }
    }
}


//==============================================================================
void GenTable::setXPosition (double pos)
{
    if (genRoutine == 1)
    {
        setRange (visibleRange.movedToStartAt (pos));
    }
    else
    {
        setRange (visibleRange.movedToStartAt (pos));
    }
}
//==============================================================================
void GenTable::setScrubberPos (double pos)
{

    //set the position of the scrubber. pos will be between 0 and 1
    if (genRoutine == 1)
    {
        currentPositionMarker->setVisible (true);

        //assign time values in seconds to pos..
        double timePos = pos * thumbnail->getTotalLength() * sampleRate;
        timePos = (timePos / (thumbnail->getTotalLength() * sampleRate)) * thumbnail->getTotalLength();
        //set position of scrubber rectangle
        currentPositionMarker->setRectangle (juce::Rectangle<float> (timeToX (timePos) - 0.75f, 0,
                                                                     1.5f, (float) (getHeight() - 20)));

        if (this->showScroll)
        {
            //take care of scrolling...
            if (timePos < thumbnail->getTotalLength() / 25.f)
            {
                setRange (visibleRange.movedToStartAt (0));
                newRangeStart = 0;
            }
            else if (visibleRange.getEnd() <= thumbnail->getTotalLength() && zoom > 0.0)
            {
                setRange (visibleRange.movedToStartAt (jmax (0.0, timePos - (visibleRange.getLength() / 2.0))));
                newRangeStart = jmax (0.0, timePos - (visibleRange.getLength() / 2.0));
                changeMessage = "updateScrollingPosition";
                sendChangeMessage();
            }


        }
    }
    else// if(genRoutine==2)
    {
        currentPositionMarker->setVisible (true);
        double waveformLengthSeconds = (double)waveformBuffer.size() / sampleRate;
        double timePos = pos * waveformLengthSeconds;
        currentPositionMarker->setRectangle (juce::Rectangle<float> (timeToX (timePos), 0,
                                                                     (genRoutine == 2 ? numPixelsPerIndex : 2), thumbArea.getHeight()));

        if (this->showScroll)
        {
            if (timePos < (waveformLengthSeconds) / 25.f)
                setRange (visibleRange.movedToStartAt (0));
            else if (visibleRange.getEnd() <= waveformLengthSeconds && zoom > 0.0)
                setRange (visibleRange.movedToStartAt (jmax (0.0, timePos - (visibleRange.getLength() / 2.0))));
        }
    }

}


//==============================================================================
void GenTable::mouseUp (const MouseEvent& e)
{
    if (genRoutine == 1)
        sendChangeMessage();
}


//===============================================================================
// Component that holds an array of handle points and sits on top of a table
// whilst in edit mode
//==================================================================================
HandleViewer::HandleViewer(): Component()
{

};

HandleViewer::~HandleViewer()
{
};

//==============================================================================
void HandleViewer::addHandle (double x, double y, double /*width*/, double height, Colour handleColour, bool status)
{
    //add a handle component to our handleViewer
    GenTable* table = getParentTable();

    if (table)
    {
        //set up handle, and pass relative x and y values as well as gen and colour
        HandleComponent* handle = new HandleComponent (x, y / getHeight(), handles.size(), false, table->genRoutine, handleColour);
        const double width = (getWidth() / tableSize);
        handle->setSize ((width > 10 ? width + 1 : FIXED_WIDTH), (width > 10 ? 5 : FIXED_WIDTH));
        handle->setPosition (getWidth()*x, y, (handle->getWidth() == FIXED_WIDTH ? true : false));

        handle->addChangeListener (table);
        handle->status = status;

        handles.add (handle);
        addAndMakeVisible (handles[handles.size() - 1]);

    }
}

void HandleViewer::insertHandle (double x, double y, Colour handleColour)
{
    //add a handle component to our handleViewer. This should be combined with
    //above function...

    int indx;
    GenTable* table = findParentComponentOfClass <GenTable>();

    if (table)
    {
        for (int i = 1; i < handles.size(); i++)
        {
            if (x * getWidth() >= handles[i - 1]->getX() && x * getWidth() < handles[i]->getX())
            {
                indx = i;
            }
        }

        HandleComponent* handle = new HandleComponent (x, y / getHeight(), handles.size(), false, table->genRoutine, handleColour);

        const double width = (getWidth() / tableSize);
        handle->setSize ((width > 10 ? width + 1 : FIXED_WIDTH), (width > 10 ? 5 : FIXED_WIDTH));
        handle->setPosition (getWidth()*x, y, (handle->getWidth() == FIXED_WIDTH ? true : false));
        handle->addChangeListener (table);
        handle->setUniqueID (indx);
        addAndMakeVisible (handle);
        handles.insert (indx, handle);
        //handle->status=true;
        handle->sendChangeMessage();
    }

}

//==============================================================================
void HandleViewer::showHandles (bool show)
{
    shouldShowHandles = show;

    for (int i = 0; i < handles.size(); i++)
    {
        //handles[i]->setColour(Colours::transparentBlack);
    }
}
//==============================================================================
void HandleViewer::mouseDown (const MouseEvent& e)
{
    if (handleViewerGen == -5 || handleViewerGen == -7 || handleViewerGen == -2)
        positionHandle (e);
}

void HandleViewer::mouseDrag (const MouseEvent& e)
{
    if (handleViewerGen == -2 && !isShowingGrid())
        positionHandle (e);
}


void HandleViewer::positionHandle (const MouseEvent& e)
{
    //positions the handle when a user sends a mouse down on the handleViewer
    if (handleViewerGen == 1 || tableSize > MAX_TABLE_SIZE )
        return;

    //determine whether of not we are in toggle on.off(grid) mode..
    double steps = (minMax.getEnd() / getParentTable()->quantiseSpace);


    bool handleExists = false;

    if (abs (handleViewerGen) == 2)
        handleExists = true;


    for (int i = 0; i < handles.size(); i++)
    {
        const double handleX = handles[i]->getPosition().getX();
        const double handleXWidth = handles[i]->getWidth();

        if (e.x > handleX && e.x < handleX + handleXWidth)
        {
            if (steps == 1) //if toggle mode is enabled..
            {
                handles[i]->status = !handles[i]->status;
                handles[i]->setTopLeftPosition (handles[i]->getPosition().withY (getSnapYPosition (getHeight()*int (handles[i]->status))));
                handles[i]->setRelativePosition (handles[i]->getPosition().toDouble().withY (getSnapYPosition (getHeight()*double (handles[i]->status))));
                handles[i]->sendChangeMessage();
                handleExists = true;
            }
            else
            {
                handles[i]->setTopLeftPosition (getSnapXPosition (e.x), getSnapYPosition (double (e.y)));
                Point<double> relPos (handles[i]->getPosition().getX(), handles[i]->getPosition().getY());
                handles[i]->setRelativePosition (relPos);
                handles[i]->sendChangeMessage();
                handleExists = true;
            }
        }
    }

    //if handle doesn't exist, create one
    if (handleExists == false)
    {
        insertHandle (getSnapXPosition (e.x) / (double)getWidth(), getSnapYPosition (e.y), colour);
    }

}

double HandleViewer::getSnapXPosition (const double x)
{
    if (getWidth() < getParentTable()->tableSize)
        return x;

    double curTableSize = getParentTable()->tableSize;
    double jump = (double)getWidth() / (double)getParentTable()->tableSize;

    for (double i = 0; i <= curTableSize; i++)
    {
        if (x > i * jump && x < (i + 1)*jump)
        {
            return i * jump;
        }
    }

    return getWidth();
}

double HandleViewer::getSnapYPosition (const double y)
{
    //return snapped position if quantise is one
    double ySnapPos = 0;
    double jump = (getParentTable()->quantiseSpace / minMax.getEnd()) * getHeight();
    ySnapPos = round (y / jump) * jump;
    //    for(double c=0; c<=steps; c++)
    //    {
    //        if(y >= (c*jump)-jump/2.f && y < ((c+1)*jump+(jump/2.f)))
    //            ySnapPos = c*jump;
    //    }

    ySnapPos = round (y / jump) * jump;


    return ySnapPos;

}

//==============================================================================
void HandleViewer::resized()
{

    for (int i = 0; i < handles.size(); i++)
    {
        const float width = getWidth() / tableSize;
        handles[i]->setSize ((width > 10 ? width : FIXED_WIDTH), (width > 10 ? 5 : FIXED_WIDTH));
        const double handleWidth = handles[i]->getWidth();
        handles[i]->setPosition (((double)getWidth()*handles[i]->xPosRelative), ((double)getHeight()*handles[i]->yPosRelative), (handleWidth == FIXED_WIDTH ? true : false));

        if (handles[i]->getWidth() > 15)
            showHandles (false);

        //handles[i]->setVisible(false);

    }
}
//==============================================================================
void HandleViewer::repaint (Graphics& g)
{
    g.fillAll (Colours::transparentBlack);
}
//==============================================================================
void HandleViewer::fixEdgePoints (int gen)
{
    //fix outer handles so they can't be dragged from the edges
    if (gen == 7 || gen == 5)
    {
        if (handles.size() > 1)
        {
            handles[0]->getProperties().set ("fixedPos", true);
            handles[handles.size() - 1]->getProperties().set ("fixedPos", true);
        }
    }
    //with a GEN02 all points are fixed...
    else if (abs (gen) == 2)
    {
        for (int i = 0; i < handles.size(); i++)
        {
            handles[i]->getProperties().set ("fixedPos", true);
        }
    }

}
//==============================================================================
int HandleViewer::getHandleIndex (HandleComponent* thisHandle)
{
    return handles.indexOf (thisHandle);
}

//==============================================================================
HandleComponent* HandleViewer::getPreviousHandle (HandleComponent* thisHandle)
{
    int thisHandleIndex = handles.indexOf (thisHandle);

    if (thisHandleIndex <= 0)
        return 0;
    else
        return handles.getUnchecked (thisHandleIndex - 1);
}
//==============================================================================
HandleComponent* HandleViewer::getNextHandle (HandleComponent* thisHandle)
{
    int thisHandleIndex = handles.indexOf (thisHandle);

    if (thisHandleIndex == -1 || thisHandleIndex >= handles.size() - 1)
        return 0;
    else
        return handles.getUnchecked (thisHandleIndex + 1);
}
//==============================================================================
void HandleViewer::removeHandle (HandleComponent* thisHandle)
{
    if (handles.size() > 0)
    {
        handles.removeObject (thisHandle, true);
    }

    if (handles.size() > 0)
        handles[0]->sendChangeMessage();
}
//==================================================================================
HandleComponent::HandleComponent (double xPos, double yPos, int _index, bool fixed, int gen, Colour _colour):
    index (_index), x (0), y (0), colour (_colour), fixed (fixed), status (false)
{
    //our main handle object. xPos and xPos are always between 0 and 1
    //we convert them to pixel positions later, based on the size of the handleViewer
    xPosRelative = xPos;
    yPosRelative = yPos;
    genRoutine = gen;
    this->setInterceptsMouseClicks (true, false);
    setSize (12, 12);
}
//==================================================================================
HandleComponent::~HandleComponent()
{
}
//==================================================================================
void HandleComponent::setColour (Colour icolour)
{
    colour = icolour;
}

void HandleComponent::paint (Graphics& g)
{
    //g.setColour(Colours::transparentBlack);
    if (abs (genRoutine) != 2)
    {
        //g.drawLine(0, (getHeight()/2.f), getWidth(), (getHeight()/2.f), 1.f);
        //g.drawLine(getWidth()/2.f, 0, getWidth()/2.f, getHeight(), 1.f);
        g.setColour (colour);

        if (getWidth() <= 15)
        {
            //g.setColour(colour.withAlpha(.4f));
            //g.drawLine(0, 7, getWidth(), 7, 1);
            //g.drawLine(7, 0, 7, getHeight(), 1);
            g.drawEllipse (3, 3, 9, 9, 1);
        }
        else
        {
            g.setColour (colour);
            g.drawRoundedRectangle (getLocalBounds().reduced (1.2f).toFloat(), 2.f, 1.f);
            g.setColour (colour.withAlpha (.7f));
            g.drawRoundedRectangle (getLocalBounds().toFloat(), 2.f, 1.f);
        }
    }
}

//==================================================================================
void HandleComponent::removeThisHandle()
{
    getParentHandleViewer()->removeHandle (this);
}
//==================================================================================
void HandleComponent::mouseEnter (const MouseEvent& e)
{
    setMouseCursor (MouseCursor::DraggingHandCursor);
    mouseStatus = "mouseEnter";
    sendChangeMessage();
}
//==================================================================================
void HandleComponent::mouseUp (const MouseEvent& e)
{
    mouseStatus = "mouseUp";
}
//==================================================================================
void HandleComponent::mouseExit (const MouseEvent& e)
{
    mouseStatus = "mouseUp";
}
//==================================================================================
static void popupMenuCallback (int result, HandleComponent* handleComp)
{

    bool fixed = handleComp->getProperties().getWithDefault ("fixedPos", false);

    if (result == 4)
    {
        if (!fixed)
            handleComp->removeThisHandle();
    }
}
//==================================================================================
void HandleComponent::mouseDown (const MouseEvent& e)
{
    //users can delete handles here, and will be able to set the curve type
    //when gen16 is added to the mix.

    if (getWidth() == FIXED_WIDTH)
        x = getX();
    else
        x = getX() + getWidth() / 2.f;

    y = getParentHandleViewer()->getSnapYPosition (getY());


    setMouseCursor (MouseCursor::DraggingHandCursor);
    dragger.startDraggingComponent (this, e);

    if ((e.mods.isShiftDown() == true) && (e.mods.isRightButtonDown() == true))
        removeThisHandle();

    PopupMenu pop, subm;
    pop.setLookAndFeel (&this->getTopLevelComponent()->getLookAndFeel());
    subm.setLookAndFeel (&this->getTopLevelComponent()->getLookAndFeel());

    if (e.mods.isRightButtonDown() == true)
    {
        pop.addItem (4, "Delete");
        pop.showMenuAsync (PopupMenu::Options(), ModalCallbackFunction::forComponent (popupMenuCallback, this));
    }

    mouseStatus = "mouseDown";
    sendChangeMessage();
}
//==================================================================================
void HandleComponent::setPosition (double posX, double posY, bool circularEnv)
{
    if (circularEnv)
        setTopLeftPosition (posX - getWidth() / 2.f, posY - getHeight() / 2.f);
    else
        setTopLeftPosition (posX, posY - getHeight() / 2.f);
}
//==================================================================================
HandleComponent* HandleComponent::getPreviousHandle()
{
    return getParentHandleViewer()->getPreviousHandle (this);
}
//==================================================================================
HandleComponent* HandleComponent::getNextHandle()
{
    return getParentHandleViewer()->getNextHandle (this);
}
//==================================================================================
void HandleComponent::setRelativePosition (Point<double> pos)
{
    //convert position so that it's scaled between 0 and 1
    xPosRelative = jlimit (0.0, 1.0, pos.getX() / (double)this->getParentHandleViewer()->getWidth());
    yPosRelative = jlimit (0.0, 1.0, pos.getY() / (double)this->getParentHandleViewer()->getHeight());
}
//==================================================================================
void HandleComponent::mouseDrag (const MouseEvent& e)
{
    //when a handle is dragged, we update its position and send a message
    //to Cabbage to update the Csound function table(CabbagePluginEditor.cpp)
    HandleComponent* previousHandle = getPreviousHandle();
    HandleComponent* nextHandle = getNextHandle();

    const bool isFixed = this->getProperties().getWithDefault ("fixedPos", false);
    double xPos = x + e.getDistanceFromDragStartX();
    double yPos = y + e.getDistanceFromDragStartY();

    HandleViewer* viewer = getParentHandleViewer();
    const double handleViewerWidth = viewer->getWidth();
    const double genTableWidth = getParentGenTable()->getWidth();

    //if not gen02 it means handles can be moved from left to right
    if (abs (genRoutine) != 2)
    {
        if (getWidth() == FIXED_WIDTH)
        {
            const int previousX = previousHandle == 0 ? 0 : previousHandle->getX() + getWidth() / 2.f;
            const int nextX = nextHandle == 0 ? getParentWidth() : nextHandle->getX() + getWidth() / 2.f;

            if (fixed && xPos > viewer->getWidth() / 2.f)
            {
                xPos = x + getWidth() / 2.f + handleViewerWidth / genTableWidth;
            }

            else if (fixed && xPos < viewer->getWidth() / 2.f)
                xPos = 1.f;

            else if (previousX >= xPos)
                xPos = previousX + 1;

            else if (xPos + getWidth() > nextX)
                xPos = nextX + 1;
        }
        else
        {
            const int previousX = previousHandle == 0 ? 0 : previousHandle->getX() + 1;
            const int nextX = nextHandle == 0 ? getParentWidth() : nextHandle->getX() - 1;

            if (isFixed && xPos > viewer->getWidth() / 2.f)
            {
                xPos = x + getWidth() / 2.f + handleViewerWidth / genTableWidth;
            }

            else if (isFixed && xPos < viewer->getWidth() / 2.f)
                xPos = 1.f;

            else if (xPos - (getWidth() / 2) <= previousX + getWidth())
            {
                xPos = previousX + getWidth() + 1;
            }

            else if (xPos + 1 > nextX)
                xPos = nextX;

        }

    }
    else
    {
        //with a gen02 each handle is fixed in place.
        xPos = x;
    }

    yPos = jlimit (0.0, getParentComponent()->getHeight() + 0.0, yPos + (getHeight() / 2.f));

    setPosition (viewer->getSnapXPosition (xPos), viewer->getSnapYPosition (yPos), (getWidth() == FIXED_WIDTH ? true : false));
    setRelativePosition (Point<double> (viewer->getSnapXPosition (xPos), viewer->getSnapYPosition (yPos)));

    mouseStatus = "mouseDrag";
    sendChangeMessage();
}
