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

#include "CabbageSoundfiler.h"
#include "../Audio/Plugins/CabbagePluginEditor.h"

CabbageSoundfiler::CabbageSoundfiler (ValueTree wData, CabbagePluginEditor* _owner, int sr)
    : soundfiler (sr, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::colour)),
                  Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::tablebackgroundcolour))),
    file (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::file)),
    zoom (CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::zoom)),
    scrubberPos (CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::scrubberposition)),
    owner (_owner),
    widgetData (wData),
    CabbageWidgetBase(_owner),
    sampleRate(sr)
{
    addAndMakeVisible (soundfiler);
    setName (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::name));
    widgetData.addListener (this);              //add listener to valueTree so it gets notified when a widget's property changes
    initialiseCommonAttributes (this, wData);   //initialise common attributes such as bounds, name, rotation, etc..



    soundfiler.setZoomFactor (CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::zoom));


    //if no channels are set remove the selectable range feature

    if (CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::scrubberposition) < 0)
        soundfiler.shouldShowScrubber (false);

    if (CabbageWidgetData::getProperty (wData, CabbageIdentifierIds::channel).size() == 0)
        soundfiler.setIsRangeSelectable (false);


    Logger::writeToLog("CabbageSoundfiler::CabbageSoundfiler:\n\tCurrent file is:" + file);
    soundfiler.setFile (File::getCurrentWorkingDirectory().getChildFile(file));
    soundfiler.addChangeListener (this);
    
    auto displayType = CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::displaytype);
    if (displayType == "mono")
    {
        soundfiler.setMonoDisplayType(true);
    }

    var tables = CabbageWidgetData::getProperty (wData, CabbageIdentifierIds::tablenumber);
    
    for (int y = 0; y < tables.size(); y++)
    {
        int tableNumber = tables[y];
        tableValues.clear();
        tableValues = owner->getTableFloats (tableNumber);
        AudioBuffer<float> sampleBuffer;
        sampleBuffer.setSize(1, tableValues.size());
        //has to be a quicker way of doing this...
        for ( int i = 0 ; i < tableValues.size() ; i++){
            sampleBuffer.setSample(0, i, tableValues[i]);
        }

        setWaveform(sampleBuffer, sr, 1);
    }
    
    if (CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::startpos) > -1 && CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::endpos) > 0)
    {
        Range<double> newRange;
        
        newRange.setStart(CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::startpos)/soundfiler.getSampleRate());
        newRange.setEnd(CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::endpos)/soundfiler.getSampleRate());
        soundfiler.setRange (newRange);
    }
    
    const int scrollbars = CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::scrollbars);
    if(scrollbars == 0)
        soundfiler.showScrollbars(false);
    
    soundfiler.shouldShowScrubber(CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::showscrubber) == 1 ? true : false );
}

void CabbageSoundfiler::changeListenerCallback (ChangeBroadcaster* source)
{
    //no need to check source, it has to be a soundfiler object
    const float position = getScrubberPosition();
    const float length = getLoopLength();

    owner->sendChannelDataToCsound (getChannelArray()[0], position);
    CabbageWidgetData::setNumProp(widgetData, CabbageIdentifierIds::regionstart, position);
    CabbageWidgetData::setNumProp(widgetData, CabbageIdentifierIds::regionlength, length);

    if (getChannelArray().size() > 1)
        owner->sendChannelDataToCsound (getChannelArray()[1], length);
}

void CabbageSoundfiler::resized()
{
    soundfiler.setBounds (0, 0, getWidth(), getHeight());
}

void CabbageSoundfiler::setFile (String newFile)
{
    soundfiler.setFile (File::getCurrentWorkingDirectory().getChildFile (newFile));
}

void CabbageSoundfiler::setWaveform (AudioSampleBuffer buffer, int sr, int channels)
{
    soundfiler.setWaveform (buffer, sr, channels);
}

int CabbageSoundfiler::getScrubberPosition()
{
    return soundfiler.getCurrentPlayPosInSamples();
}

int CabbageSoundfiler::getLoopLength()
{
    return soundfiler.getLoopLengthInSamples();
}

void CabbageSoundfiler::valueTreePropertyChanged (ValueTree& valueTree, const Identifier& prop)
{
    if(prop == CabbageIdentifierIds::update)
    {
        if(CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::tablenumber) != -1)
        {
            var tables = CabbageWidgetData::getProperty (valueTree, CabbageIdentifierIds::tablenumber);
            
            for (int y = 0; y < tables.size(); y++)
            {
                int tableNumber = tables[y];
                tableValues.clear();
                tableValues = owner->getTableFloats (tableNumber);
                AudioBuffer<float> sampleBuffer;
                sampleBuffer.setSize(1, tableValues.size());
                //has to be a quicker way of doing this...
                for ( int i = 0 ; i < tableValues.size() ; i++){
                    sampleBuffer.setSample(0, i, tableValues[i]);
                }

                setWaveform(sampleBuffer, sampleRate, 1);
            }
        }
        else
        {
            file = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::file);
            const String fullPath = File (getCsdFile()).getParentDirectory().getChildFile (file).getFullPathName();
            setFile (fullPath);
        }
    }
    
    if (file != CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::file))
    {
        if(File(CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::file)).existsAsFile())
        {
            file = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::file);
            const String fullPath = File (getCsdFile()).getParentDirectory().getChildFile (file).getFullPathName();
            setFile (fullPath);
//            DBG("Soundfiler file:" << fullPath);
        }
    }

    if (prop == CabbageIdentifierIds::regionstart)
    {
        soundfiler.setCurrentPlayPos(static_cast<int>(CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::regionstart)));
    }
    else if (prop == CabbageIdentifierIds::regionlength)
    {
        int length = static_cast<int>(CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::regionlength));
        soundfiler.setRegionWidth(length);
//        if(length == 0)
//            soundfiler.setCurrentPlayPos(-1);
    }

    if (zoom != CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::zoom))
    {
        zoom = CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::zoom);
        soundfiler.setZoomFactor (zoom);
    }

    if(prop == CabbageIdentifierIds::startpos || prop == CabbageIdentifierIds::endpos)
    {
        if (CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::startpos) > -1 && CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::endpos) > 0)
        {
            Range<double> newRange;
            newRange.setStart(CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::startpos)/soundfiler.getSampleRate());
            newRange.setEnd(CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::endpos)/soundfiler.getSampleRate());
            soundfiler.setRange (newRange);
        }
    }
    
    soundfiler.setScrubberPos (CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::scrubberposition));
    soundfiler.setWaveformColour (CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::colour));
    soundfiler.setBackgroundColour (CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::tablebackgroundcolour));
    handleCommonUpdates (this, valueTree, false, prop);      //handle common updates such as bounds, alpha, rotation, visible, etc
    soundfiler.repaint();
    repaint();

}
