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
#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include <csound.hpp>
#include <csdebug.h>
#include "../../Utilities/CabbageUtilities.h"
#include "CabbageCsoundBreakpointData.h"

//==============================================================================
/**
*/
class CsoundPluginProcessor  : public AudioProcessor, public AsyncUpdater
{

public:
    //==============================================================================
    CsoundPluginProcessor(File csoundInputFile, bool debugMode = false);
    ~CsoundPluginProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
#endif

    virtual void processBlock (AudioSampleBuffer&, MidiBuffer&) override;

    //==============================================================================
    virtual AudioProcessorEditor* createEditor() override;
    virtual bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================
    //Csound API functions for deailing with midi input
    static int OpenMidiInputDevice(CSOUND * csnd, void **userData, const char *devName);
    static int OpenMidiOutputDevice(CSOUND * csnd, void **userData, const char *devName);
    static int ReadMidiData(CSOUND *csound, void *userData, unsigned char *mbuf, int nbytes);
    static int WriteMidiData(CSOUND *csound, void *userData, const unsigned char *mbuf, int nbytes);

    //graphing functions
    static void makeGraphCallback(CSOUND *csound, WINDAT *windat, const char *name);
    static void drawGraphCallback(CSOUND *csound, WINDAT *windat);
    static void killGraphCallback(CSOUND *csound, WINDAT *windat);
    static int exitGraphCallback(CSOUND *csound);


    void handleAsyncUpdate();
	//csound breakpint function
	static void breakpointCallback(CSOUND *csound, debug_bkpt_info_t *bkpt_info, void *udata);
	CabbageCsoundBreakpointData breakPointData;
	
	ValueTree getBreakpointData(){
		return breakPointData.valueTree;
	}
    //=============================================================================
    //Implement these to send and receive channel data to Csound. Typically used when
    //a component is updated and its value is sent to Csound, or when a Csound channel
    //is updated in the Csound orchestra and we need to update the Cabbage components.
    //Note that sendChannelDataToCsound() if we subclass the AudioprocessorParameter clas
    //as is done in CabbagePluginprocessor.
    virtual void sendChannelDataToCsound() {};
    virtual void receiveChannelDataFromCsound() {};
    virtual void initAllCsoundChannels(ValueTree cabbageData);
    //=============================================================================
    String getCsoundOutput();

    void compileCsdFile(File csdFile)
    {
        csCompileResult = csound->Compile(const_cast<char*>(csdFile.getFullPathName().toUTF8().getAddress()));
    }

    bool csdCompiledWithoutError()
    {
        return csCompileResult==0 ? true : false;
    }

    Csound* getCsound()
    {
        return csound;
    }

    CSOUND* getCsoundStruct()
    {
        return csound->GetCsound();
    }

    int setGUIRefreshRate(int rate)
    {
        guiRefreshRate = rate;
    }

	MidiKeyboardState keyboardState;
	
private:
    //==============================================================================
    MidiBuffer midiOutputBuffer;
    int guiCycles = 0;
    int guiRefreshRate = 50;
    MidiBuffer midiBuffer;
    String csoundOutput;
    ScopedPointer<CSOUND_PARAMS> csoundParams;
    int csCompileResult, numCsoundChannels, pos;
    
    MYFLT cs_scale;
    MYFLT *CSspin, *CSspout;
    int csndIndex;
    int csdKsmps;
    ScopedPointer<Csound> csound;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CsoundPluginProcessor)

};


#endif  // PLUGINPROCESSOR_H_INCLUDED
