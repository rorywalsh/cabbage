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

#include "JuceHeader.h"
#include <csound.hpp>
#include <csdebug.h>
#include "csdl.h"
#include <cwindow.h>
#include "../../Opcodes/opcodes.hpp"
#include "../../Opcodes/CabbageMidiOpcodes.h"
#include "../../Opcodes/CabbageProfilerOpcodes.h"
#ifdef WebUI
#include "../../Opcodes/CabbageWebUIOpcodes.h"
#endif
#include "../../Opcodes/CabbageIdentifierOpcodes.h"
//#include "../../Opcodes/CabbageFileReaderOpcodes.h"
#include "../../Utilities/CabbageUtilities.h"
#include "CabbageCsoundBreakpointData.h"
#if CabbagePro
#include "../../Utilities/encrypt.h"
#endif

#if Bluetooth
#include "../../Opcodes/CabbageBTOpcodes.h"
#endif

//this class comes courtesy of @EyalAmir from TAP discord..
class ProcessBlockTimeListener
{
public:
    void updateBlockTime() { lastTime = getCurrentTime(); }

    double getTimeSinceLastBlock() const
    {
        return getCurrentTime() - lastTime.load();
    }

private:
    static double getCurrentTime()
    {
        return juce::Time::getMillisecondCounterHiRes();
    }

    std::atomic<double> lastTime {getCurrentTime()};

};

//==============================================================================
class CsoundPluginProcessor : public AudioProcessor, public AsyncUpdater
{
public:
    //==============================================================================
	CsoundPluginProcessor(File csoundInputFile, 
        const BusesProperties& ioBuses);
	~CsoundPluginProcessor() override;


    void destroyCsoundGlobalVars();
    void createCsoundGlobalVars(const ValueTree& cabbageData);
	bool supportsSidechain = false;
	bool matchingNumberOfIOChannels = true;
	void resetCsound();
	//==============================================================================
	//pass the path to the temp file, along with the path to the original csd file so we can set correct working dir
	bool setupAndCompileCsound(File csdFile, File filePath, int sr = 44100, bool debugMode = false);
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

//=======================================================================================
#if Stereo_Mono_Only
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override
    {
#if JucePlugin_IsMidiEffect
      juce::ignoreUnused (layouts);
      return true;
#else
      // This is the place where you check if the layout is supported.
      // In this template code we only support mono or stereo.
      // Some plugin hosts, such as certain GarageBand versions, will only
      // load plugins that support stereo bus layouts.
      if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
       && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
          return false;

      // This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
      if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
          return false;
#endif

      return true;
#endif
        }
#else
//=======================================================================================
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override
    {
        ignoreUnused(layouts);
#if ! Cabbage_IDE_Build
        if(AudioProcessor::wrapperType == wrapperType_AudioUnit)
        {
            if((layouts.getMainInputChannels() > 0 && layouts.getMainInputChannels() < 32 ) &&
               (layouts.getMainOutputChannels() > 0 && layouts.getMainOutputChannels() < 32))
                return true;
        }
        else if(AudioProcessor::wrapperType == wrapperType_VST)
        {
            //force Reaper and Live to take whatever IOs we give it
            if(PluginHostType().isReaper() || PluginHostType().isAbletonLive())
                return false;
        }
        else if(AudioProcessor::wrapperType == wrapperType_VST3)
        {
            return true;
        }

#endif
#ifdef JUCE_LINUX
        return false;
#else
        return true;
#endif
    }
#endif
//=======================================================================================

    void performCsoundKsmps();
	int result = -1;
    bool isLMMS = false;
    bool firstInit = true;
	virtual void processBlock(AudioBuffer< float >&, MidiBuffer&) override;
	virtual void processBlock(AudioBuffer< double >&, MidiBuffer&) override;
	template< typename Type >
	void processSamples(AudioBuffer< Type >&, MidiBuffer&);
	//bool supportsDoublePrecisionProcessing() const override { return true; }

    virtual void processBlockBypassed (AudioBuffer< float > &buffer, MidiBuffer &midiMessages) override {
        ignoreUnused(buffer, midiMessages);
    }

	enum BufferType {
		inputOutput,
		output,
		input
	};

	template< typename Type >
    void processIOBuffers(int bufferType, Type* buffer, int samplePos, int csdPos);

    int numSideChainChannels = 0;
    //==============================================================================
    virtual AudioProcessorEditor* createEditor() override;
    virtual bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    void resetFilebuttons(ValueTree cabbageData);
    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;
    //==============================================================================
    virtual void getStateInformation(MemoryBlock& destData) override {};
    virtual void setStateInformation(const void* data, int sizeInBytes) override {};

    //==============================================================================
    bool hostRequestedMono = false;
    bool isLogic = false;
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

    int getChnsetGestureMode()
    {
        if(csound)
        {
            return int(csound->GetChannel("CSOUND_GESTURES"));
        }
        
        return 0;
    }
    
    StringArray getTableStatement (int tableNum);
    const Array<float, CriticalSection> getTableFloats (int tableNum);

    AudioPlayHead::CurrentPositionInfo hostInfo = {};

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

        explicit MatrixEventSequencer(String csoundChannel):channel(csoundChannel){}
        ~MatrixEventSequencer(){ events.clear();}

    };

	int numMatrixEventSequencers = 0;
    void createMatrixEventSequencer(int rows, int cols, String channel);
    void setMatrixEventSequencerCellData(int row, int col, const String& channel, String data);

    virtual void sendChannelDataToCsound() {}
    virtual void getIdentifierDataFromCsound() {}
    void sendHostDataToCsound();
    virtual void getChannelDataFromCsound() {}
    virtual void initAllCsoundChannels (ValueTree cabbageData);
    //=============================================================================
    void addMacros (String& csdText);
    String getCsoundOutput();

    void compileCsdFile (File csoundFile)
    {
        csCompileResult = csound->Compile (csoundFile.getFullPathName().toUTF8().getAddress());
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
    
    void setNumPreCycles (int num)
    {
        preCycles = num;			
    }
    
    void startRecording(const File& file, int bitDepth = 32);
    void stopRecording();
    bool isRecording() const
    {
        return activeWriter.load() != nullptr;
    }
    
    
    int preCycles = 0;
    MidiKeyboardState keyboardState;
    bool hostIsCubase = false;

    //==================================================================================
    class SignalDisplay
    {
    public:
        float yScale = 0.f;
        int windid = 0, min1 = 0, max1 = 0, size = 0;
        String caption = {}, variableName = {};

        SignalDisplay (String _caption, int _id, float _scale, int _min, int _max, int _size):
            yScale (_scale),
            windid (_id),
            min1 (_min),
            max1 (_max),
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

    bool shouldUpdateSignalDisplay(String signalDisplayName)
    {
        bool returnVal = bool(updateSignalDisplay.getWithDefault(signalDisplayName, false));
        updateSignalDisplay.set(signalDisplayName, false);
        return returnVal;
    }

    TimeSliceThread backgroundThread { "Audio Recorder Thread" }; // the thread that will write our audio data to disk
    std::unique_ptr<AudioFormatWriter::ThreadedWriter> threadedWriter; // the FIFO used to buffer the incoming data
    std::atomic<AudioFormatWriter::ThreadedWriter*> activeWriter { nullptr };
    std::unique_ptr<AudioData::Converter> converter;
    CriticalSection writerLock;
    OwnedArray<MatrixEventSequencer> matrixEventSequencers;
    OwnedArray <SignalDisplay, CriticalSection> signalArrays;   //holds values from FFT function table created using dispfft
    CsoundPluginProcessor::SignalDisplay* getSignalArray (String variableName, String displayType = "") const;

    String getInternalState()
    {
        return internalStateData;
    }


    
    void pollingChannels(int shouldPoll)
    {
        polling = shouldPoll;
    }
    
    void setPreferredLatency(int latency)
    {
        preferredLatency = latency;
        setLatencySamples(preferredLatency);
    }
    
    int pollingChannels()
    {
        return polling;
    }
    
    bool wasRecompiled() { return recompiledOnPrepareToPlay;   }
    void resetRecompiled() { recompiledOnPrepareToPlay = false; }
    ProcessBlockTimeListener processBlockListener;
private:
    //==============================================================================
    bool recompiledOnPrepareToPlay = false;
    int polling = 1;
    MidiBuffer midiOutputBuffer;
    int guiCycles = 0;
    int guiRefreshRate = 128;
    MidiBuffer midiBuffer = {};
    String csoundOutput = {};
    std::unique_ptr<CSOUND_PARAMS> csoundParams;
    int csCompileResult = -1;
    int numCsoundOutputChannels = 0;
    int numCsoundInputChannels = 0;
    int pos = 0;
    NamedValueSet updateSignalDisplay;
    MYFLT cs_scale = 0.0;
    bool testLogicForMono = true;
    MYFLT *CSspin = nullptr;
    MYFLT *CSspout = nullptr;
    int samplingRate = 44100;
    int csndIndex = 0;
    int csdKsmps = 0;
    File csdFile = {}, csdFilePath = {};
    std::unique_ptr<Csound> csound;
    std::unique_ptr<FileLogger> fileLogger;
//    int busIndex = 0;
    bool disableLogging = false;
	int preferredLatency = 32;
    String internalStateData = {};



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CsoundPluginProcessor)

};


#endif  // PLUGINPROCESSOR_H_INCLUDED
