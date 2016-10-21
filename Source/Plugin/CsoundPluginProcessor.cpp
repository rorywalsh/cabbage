/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "CsoundPluginProcessor.h"
#include "CsoundPluginEditor.h"
#include "../Utilities/CabbageUtilities.h"


//==============================================================================
CsoundAudioProcessor::CsoundAudioProcessor(File csdFile)
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::octagonal(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::octagonal(), true)
                     #endif
                       )
#endif
{
	CabbageUtilities::debug("Plugin constructor");
	csound = new CabbageCsound();

    csound->SetHostImplementedMIDIIO(true);
    csound->SetHostData(this);

    csound->CreateMessageBuffer(0);
    csound->SetExternalMidiInOpenCallback(OpenMidiInputDevice);
    csound->SetExternalMidiReadCallback(ReadMidiData);
    csound->SetExternalMidiOutOpenCallback(OpenMidiOutputDevice);
    csound->SetExternalMidiWriteCallback(WriteMidiData);
	csoundParams = nullptr;
    csoundParams = new CSOUND_PARAMS();
	
    csoundParams->displays = 0;
    csound->SetParams(csoundParams);
    csound->SetOption((char*)"-n");
    csound->SetOption((char*)"-d");

    csound->compileCsdFile(csdFile);
    numCsoundChannels = csound->GetNchnls();

    csdFile.getParentDirectory().setAsCurrentWorkingDirectory();
	
    if(csound->compiledWithoutError())
    {	
		csdKsmps = csound->GetKsmps();
        if(csound->GetSpout()==nullptr);
        CSspout = csound->GetSpout();
        CSspin  = csound->GetSpin();
        cs_scale = csound->Get0dBFS();
        csndIndex = csound->GetKsmps();
        this->setLatencySamples(csound->GetKsmps());
	}
	else
		CabbageUtilities::debug("Csound could not compile your file?");
		
}

CsoundAudioProcessor::~CsoundAudioProcessor()
{
	CabbageUtilities::debug("Plugin destructor");
	if(csound)
    {
        this->getCallbackLock().enter();
        Logger::writeToLog("about to cleanup Csound");
        csound = nullptr;
        Logger::writeToLog("Csound cleaned up");
    }
}

//==============================================================================
String CsoundAudioProcessor::getCsoundOutput()
{
    const int messageCnt = csound->GetMessageCnt();
    csoundOutput = "";
    if(messageCnt==0)
        return csoundOutput;

    for(int i=0; i<messageCnt; i++)
    {
        csoundOutput+=csound->GetFirstMessage();
        csound->PopFirstMessage();
    }

    return csoundOutput;
}

//==============================================================================
const String CsoundAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool CsoundAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool CsoundAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

double CsoundAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int CsoundAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int CsoundAudioProcessor::getCurrentProgram()
{
    return 0;
}

void CsoundAudioProcessor::setCurrentProgram (int index)
{
}

const String CsoundAudioProcessor::getProgramName (int index)
{
    return String();
}

void CsoundAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void CsoundAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void CsoundAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool CsoundAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void CsoundAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    float** audioBuffers = buffer.getArrayOfWritePointers();
    const int numSamples = buffer.getNumSamples();
    float samp;
	int result = -1;

#if defined(Cabbage_Build_Standalone) || defined(AndroidBuild)
    const int output_channel_count = numCsoundChannels;
#else
    const int output_channel_count = getTotalNumOutputChannels();
#endif



        //if no inputs are used clear buffer in case it's not empty..
        if(getTotalNumInputChannels()==0)
            buffer.clear();

        if(csound->compiledWithoutError())
        {
            keyboardState.processNextMidiBuffer (midiMessages, 0, numSamples, true);
            midiBuffer = midiMessages;

#if JucePlugin_ProducesMidiOutput
            if(!midiOutputBuffer.isEmpty())
            {
                midiMessages.swapWith(midiOutputBuffer);
            }
            else
                midiMessages.clear();
#endif

            for(int i=0; i<numSamples; i++, ++csndIndex)
            {
                if(csndIndex == csdKsmps)
                {
                    result = csound->PerformKsmps();

                    if(result!=0)
                        this->suspendProcessing(true);

                    csndIndex = 0;
                }
                if(result==0)
                {
                    pos = csndIndex * output_channel_count;
                    for(int channel = 0; channel < output_channel_count; ++channel)
                    {
                        float *&current_buffer = audioBuffers[channel];
                        samp = *current_buffer * cs_scale;
                        CSspin[pos] = samp;
                        *current_buffer = (CSspout[pos] / cs_scale);
                        ++current_buffer;
                        ++pos;

                    }
                }
                else
                    buffer.clear();
            }


        }//if not compiled just mute output
        else
        {
            for(int channel = 0; channel < output_channel_count; ++channel)
            {
                for(int i=0; i<numSamples; ++i, ++csndIndex)
                {
                    audioBuffers[channel][i]=0;
                }
            }
        }
}


//==============================================================================
bool CsoundAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* CsoundAudioProcessor::createEditor()
{
    return new CsoundAudioProcessorEditor (*this);
}

//==============================================================================
void CsoundAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void CsoundAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//======================== CSOUND MIDI FUNCTIONS ================================
int CsoundAudioProcessor::OpenMidiInputDevice(CSOUND * csound, void **userData, const char* /*devName*/)
{
    *userData = csoundGetHostData(csound);
    if(!userData)
        CabbageUtilities::debug("\n\ncan't open midi in\n\n");
    return 0;
}

//==============================================================================
// Reads MIDI input data from host, gets called every time there is MIDI input to our plugin
//==============================================================================
int CsoundAudioProcessor::ReadMidiData(CSOUND* /*csound*/, void *userData,
        unsigned char *mbuf, int nbytes)
{
    CsoundAudioProcessor *midiData = (CsoundAudioProcessor *)userData;
    if(!userData)
    {
        CabbageUtilities::debug("\nInvalid");
        return 0;
    }

    int cnt=0;

    if(!midiData->midiBuffer.isEmpty() && cnt <= (nbytes - 3))
    {
        MidiMessage message(0xf4, 0, 0, 0);
        MidiBuffer::Iterator i (midiData->midiBuffer);
        int messageFrameRelativeTothisProcess;
        while (i.getNextEvent (message, messageFrameRelativeTothisProcess))
        {
            const uint8* data = message.getRawData();
            *mbuf++ = *data++;
            *mbuf++ = *data++;
            *mbuf++ = *data++;

            if(message.isProgramChange() || message.isChannelPressure())
                cnt+=2;
            else
                cnt+=3;
        }
        midiData->midiBuffer.clear();
    }
    return cnt;

}

//==============================================================================
// Opens MIDI output device, adding -QN to your CsOptions will causes this method to be called
// as soon as your plugin loads
//==============================================================================
int CsoundAudioProcessor::OpenMidiOutputDevice(CSOUND * csound, void **userData, const char* /*devName*/)
{
#if !defined(AndroidBuild)
    *userData = csoundGetHostData(csound);
    if(!userData)
        Logger::writeToLog("\n\ncan't open midi out\n\n");
#endif
    return 0;
}

//==============================================================================
// Write MIDI data to plugin's MIDI output. Each time Csound outputs a midi message this
// method should be called. Note: you must have -Q set in your CsOptions
//==============================================================================
int CsoundAudioProcessor::WriteMidiData(CSOUND* /*csound*/, void *_userData,
        const unsigned char *mbuf, int nbytes)
{
    CsoundAudioProcessor *userData = (CsoundAudioProcessor *)_userData;
    if(!userData)
    {
        CabbageUtilities::debug("\n\nInvalid");
        return 0;
    }

    MidiMessage message(mbuf, nbytes, 0);

    userData->midiOutputBuffer.addEvent(message, 0);
    return nbytes;
}

//===========================================================================================
// graphing functions...
//===========================================================================================

void CsoundAudioProcessor::makeGraphCallback(CSOUND *csound, WINDAT *windat, const char * /*name*/)
{
    CsoundAudioProcessor *ud = (CsoundAudioProcessor *) csoundGetHostData(csound);
}

void CsoundAudioProcessor::drawGraphCallback(CSOUND *csound, WINDAT *windat)
{
    CsoundAudioProcessor *ud = (CsoundAudioProcessor *) csoundGetHostData(csound);

}

void CsoundAudioProcessor::killGraphCallback(CSOUND *csound, WINDAT *windat)
{
    CsoundAudioProcessor *udata = (CsoundAudioProcessor *) csoundGetHostData(csound);
    CabbageUtilities::debug("killGraphCallback");
}

int CsoundAudioProcessor::exitGraphCallback(CSOUND *csound)
{
    CsoundAudioProcessor *udata = (CsoundAudioProcessor *) csoundGetHostData(csound);
    CabbageUtilities::debug("exitGraphCallback");
    return 0;
}




