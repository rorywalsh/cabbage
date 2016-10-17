/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"


//==============================================================================
/**
*/
class CsoundAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    CsoundAudioProcessor();
    ~CsoundAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool setPreferredBusArrangement (bool isInput, int bus, const AudioChannelSet& preferredSet) override;
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

	//midi class stuff....
	class MIDIClass
	{
		int channel, xPos, yPos;

	public:
		MIDIClass() {}
		~MIDIClass() {}

		void setXY(int x, int y)
		{
			xPos = x;
			yPos = y;
		}

		void setMIDIChannel(int newChannelNumber)
		{
			channel = newChannelNumber;
		}

	};
	OwnedArray<MIDIClass> midiClassVector;

	void addMIDIClass(int x, int y)
	{
		MIDIClass* midiClass = new MIDIClass();
		midiClass->setMIDIChannel(1);
		midiClass->setXY(x, y);
		midiClassVector.add(midiClass);
	}

	void setDeviceManager(AudioDeviceManager* dev)
	{
		devManager = dev;
	}

	MidiBuffer midiBuffer;

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CsoundAudioProcessor)
	//this will hold an instance of our device manager so that we can pass
	//MIDI events from our processor to our main application
	AudioDeviceManager* devManager;
	MidiOutput* midiOut;
};


#endif  // PLUGINPROCESSOR_H_INCLUDED
