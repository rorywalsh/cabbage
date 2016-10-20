/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "CabbageCsound.h"

//==============================================================================
/**
*/
class CsoundAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    CsoundAudioProcessor(File csoundInputFile);
    ~CsoundAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

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
	
	//=============================================================================
	String getCsoundOutput();
	
private:

    //==============================================================================
	MidiBuffer midiOutputBuffer;
	MidiBuffer midiBuffer;
	String csoundOutput;
	ScopedPointer<CSOUND_PARAMS> csoundParams;
	int csCompileResult, numCsoundChannels, pos;
	MidiKeyboardState keyboardState;
    MYFLT cs_scale;
    MYFLT *CSspin, *CSspout;       
    int csndIndex;                         
    int csdKsmps;
	ScopedPointer<CabbageCsound> csound;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CsoundAudioProcessor)
	
};


#endif  // PLUGINPROCESSOR_H_INCLUDED
