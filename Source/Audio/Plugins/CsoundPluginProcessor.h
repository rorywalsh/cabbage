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

#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include <csound.hpp>
#include <csdebug.h>
#include "csdl.h"
#include <cwindow.h>
#include "../../Utilities/CabbageUtilities.h"
#include "CabbageCsoundBreakpointData.h"
#ifdef CabbagePro
#include "../../Utilities/encrypt.h"
#endif

//==============================================================================
class CsoundPluginProcessor  : public AudioProcessor, public AsyncUpdater
{
public:
    //==============================================================================
    CsoundPluginProcessor (File csoundInputFile, const int ins = 2, const int outs = 2, bool debugMode = false);
	~CsoundPluginProcessor();
	void resetCsound();
    //==============================================================================
    //pass the path to the temp file, along with the path to the original csd file so we can set correct working dir
	bool setupAndCompileCsound(File csdFile, File filePath, int sr = 44100, bool isMono = false, bool debugMode = false);
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

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
    virtual void getStateInformation (MemoryBlock& destData) override;
    virtual void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================

    //==============================================================================
    //Csound API functions for deailing with midi input
    static int OpenMidiInputDevice (CSOUND* csnd, void** userData, const char* devName);
    static int OpenMidiOutputDevice (CSOUND* csnd, void** userData, const char* devName);
    static int ReadMidiData (CSOUND* csound, void* userData, unsigned char* mbuf, int nbytes);
    static int WriteMidiData (CSOUND* csound, void* userData, const unsigned char* mbuf, int nbytes);

    //graphing functions
    static void makeGraphCallback (CSOUND* csound, WINDAT* windat, const char* name);
    static void drawGraphCallback (CSOUND* csound, WINDAT* windat);
    static void killGraphCallback (CSOUND* csound, WINDAT* windat);
    static int exitGraphCallback (CSOUND* csound);

    //logger
    void createFileLogger (File csdFile);

    void handleAsyncUpdate() override;
    //csound breakpint function
    static void breakpointCallback (CSOUND* csound, debug_bkpt_info_t* bkpt_info, void* udata);
    CabbageCsoundBreakpointData breakPointData;

    ValueTree getBreakpointData()
    {
        return breakPointData.valueTree;
    }

    StringArray getTableStatement (int tableNum);
    const Array<float, CriticalSection> getTableFloats (int tableNum);
    int checkTable (int tableNum);
    AudioPlayHead::CurrentPositionInfo hostInfo;

    class MatrixEventSequencer
    {
    public:
        String channel = "";
        OwnedArray<StringArray> events;
        int position = 0;
        void setEventString(int col, int row, String data)
        {
            events[col]->set(row, data);
        }

        MatrixEventSequencer(String csoundChannel):channel(csoundChannel){}
        ~MatrixEventSequencer(){ events.clear();}

    };

	int numMatrixEventSequencers = 0;
    void createMatrixEventSequencer(int rows, int cols, String channel);
    void setMatrixEventSequencerCellData(int row, int col, String channel, String data);
    //=============================================================================
    //Implement these to init, send and receive channel data to Csound. Typically used when
    //a component is updated and its value is sent to Csound, or when a Csound channel
    //is updated in the Csound orchestra and we need to update the Cabbage components.
    //Note that sendChannelDataToCsound() if we subclass the AudioprocessorParameter clas
    //as is done in CabbagePluginprocessor.
    virtual void triggerCsoundEvents();
    virtual void sendChannelDataToCsound() {};
    void sendHostDataToCsound();
    virtual void getChannelDataFromCsound() {};
    virtual void initAllCsoundChannels (ValueTree cabbageData);
    //=============================================================================
    void addMacros (String csdText);
    const String getCsoundOutput();

    void compileCsdFile (File csdFile)
    {
        csCompileResult = csound->Compile (const_cast<char*> (csdFile.getFullPathName().toUTF8().getAddress()));
    }

    void compileCsdString (String csdFileText)
    {
        csCompileResult = csound->CompileCsdText (const_cast<char*> (csdFileText.toUTF8().getAddress()));
    }

    bool csdCompiledWithoutError()
    {
        return csCompileResult == 0 ? true : false;
    }



    Csound* getCsound()
    {
        return csound.get();
    }

    CSOUND* getCsoundStruct()
    {
        return csound->GetCsound();
    }

    void setGUIRefreshRate (int rate)
    {
        guiRefreshRate = rate;
    }



    int getNumberOfCsoundChannels()
    {
        return numCsoundChannels;
    }

    MidiKeyboardState keyboardState;

    //==================================================================================
    class SignalDisplay
    {
    public:
        float yScale;
        int windid, min , max, size;
        String caption;

        SignalDisplay (String _caption, int _id, float _scale, int _min, int _max, int _size):
            yScale (_scale),
            windid (_id),
            min (_min),
            max (_max),
            size (_size),
            caption (_caption)
        {}

        ~SignalDisplay()
        {
            points.clear();
        }

        Array<float, CriticalSection> getPoints()
        {
            return points;
        }

        void setPoints (Array <float, CriticalSection > tablePoints)
        {
            points.swapWith (tablePoints);
        }

    private:
        Array <float, CriticalSection > points;
    };

    bool shouldUpdateSignalDisplay()
    {
        bool returnVal = updateSignalDisplay;
        updateSignalDisplay = false;
        return returnVal;
    };

    OwnedArray<MatrixEventSequencer> matrixEventSequencers;
    OwnedArray <SignalDisplay, CriticalSection> signalArrays;   //holds values from FFT function table created using dispfft
    CsoundPluginProcessor::SignalDisplay* getSignalArray (String variableName, String displayType = "");

private:
    //==============================================================================
    MidiBuffer midiOutputBuffer;
    int guiCycles = 0;
    int guiRefreshRate = 128;
    MidiBuffer midiBuffer;
    String csoundOutput;
    std::unique_ptr<CSOUND_PARAMS> csoundParams;
    int csCompileResult = -1;
    int numCsoundChannels, pos;
    bool updateSignalDisplay = false;
    MYFLT cs_scale;
    MYFLT* CSspin, *CSspout;
    int samplingRate = 44100;
    int csndIndex;
    int csdKsmps;
    File csdFile , csdFilePath;
    std::unique_ptr<Csound> csound;
    std::unique_ptr<FileLogger> fileLogger;
    int busIndex = 0;
    bool disableLogging = false;



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CsoundPluginProcessor)

};


#endif  // PLUGINPROCESSOR_H_INCLUDED
