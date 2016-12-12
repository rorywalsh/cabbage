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

CabbageSoundfiler::CabbageSoundfiler(ValueTree wData)
	: widgetData(wData),
	colour(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::colour)),
	fontcolour(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::fontcolour)),
	file(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::file)),
	zoom(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::zoom)),
	scrubberPos(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::scrubberposition)),
	soundfiler(44100, Colour::fromString(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::fontcolour)), 
									Colour::fromString(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::fontcolour)))
{
	addAndMakeVisible(soundfiler);
	setName(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::name));
	widgetData.addListener(this); 				//add listener to valueTree so it gets notified when a widget's property changes
	initialiseCommonAttributes(this, wData); 	//initialise common attributes such as bounds, name, rotation, etc..	
	

	sampleRate = 44100;
	soundfiler.setZoomFactor(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::zoom));


	//if no channels are set remove the selectable range feature

	if(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::scrubberposition)<0)
		soundfiler.shouldShowScrubber(false);

	if(CabbageWidgetData::getProperty(wData, CabbageIdentifierIds::channel).size()==0)
		soundfiler.setIsRangeSelectable(false);
		
	soundfiler.setFile(file);
	
}

void CabbageSoundfiler::resized()
{
	soundfiler.setBounds(0, 0, getWidth(), getHeight());
}
	
void CabbageSoundfiler::setFile(String newFile)
{
	soundfiler.setFile(File(newFile));
}

int CabbageSoundfiler::setWaveform(AudioSampleBuffer buffer, int channels)
{
	soundfiler.setWaveform(buffer, channels);
}

int CabbageSoundfiler::getPosition()
{
	return soundfiler.getCurrentPlayPosInSamples();
}

int CabbageSoundfiler::getLoopLength()
{
	return soundfiler.getLoopLengthInSamples();
}
	
void CabbageSoundfiler::valueTreePropertyChanged (ValueTree& valueTree, const Identifier& prop)
{
	if(file!=CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::file))
	{
		file = CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::file);
		setFile(file);		
	}

		
		
	handleCommonUpdates(this, valueTree);		//handle comon updates such as bounds, alpha, rotation, visible, etc	
}