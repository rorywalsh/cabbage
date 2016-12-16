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

#ifndef CABBAGEPLUGINPROCESSOR_H_INCLUDED
#define CABBAGEPLUGINPROCESSOR_H_INCLUDED

#include "CsoundPluginProcessor.h"
#include "../../Widgets/CabbageWidgetData.h"
#include "../../CabbageIds.h"
#include "CabbageAudioParameter.h"
#include "../../Widgets/Auxiliary/XYPadAutomation.h"

class CabbagePluginProcessor  
	: public ChangeListener, 
	public CsoundPluginProcessor,
	public Timer
{
public:
    CabbagePluginProcessor(File inputFile);
    ~CabbagePluginProcessor();

    ValueTree cabbageWidgets;

    void receiveChannelDataFromCsound();
    void parseCsdFile(String csdText);
    void createParameters();
	void updateWidgets(String csdText);
	void searchAndExpandMacros(StringArray& csdText);

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    File getCurrentCsdFile()
    {
        return csdFile;
    }
    StringArray getCurrentCsdFileAsStringArray()
    {
        StringArray csdArray;
        csdArray.addLines(csdFile.loadFileAsString());
        return csdArray;
    }


	void changeListenerCallback(juce::ChangeBroadcaster*);
	//===== XYPad methods =========
	void timerCallback();
    void addXYAutomater(XYPadAutomation* xyAuto, ValueTree wData);
    XYPadAutomation* getXYAutomater(int index)    {        return xyAutomation[index];    }
    int getXYAutomaterSize()    {        return xyAutomation.size();    }
    void removeXYAutomaters()    {        xyAutomation.clear();    }	
    bool haveXYAutosBeenCreated()    {        return xyAutosCreated;    }
    void setHaveXYAutoBeenCreated(bool val)    {        xyAutosCreated = val;    }
	
private:
    File csdFile;
	NamedValueSet macroText;
	bool xyAutosCreated = false;
	OwnedArray<XYPadAutomation, CriticalSection> xyAutomation;

};



#endif  // CABBAGEPLUGINPROCESSOR_H_INCLUDED
