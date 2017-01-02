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

#include "CsoundPluginProcessor.h"
#include "CsoundPluginEditor.h"
#include "../../Utilities/CabbageUtilities.h"
#include "../../Widgets/CabbageWidgetData.h"


//==============================================================================
CsoundPluginProcessor::CsoundPluginProcessor(File csdFile, bool debugMode)
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor (BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
                      .withInput  ("Input",  AudioChannelSet::stereo(), true)
#endif
                      .withOutput ("Output", AudioChannelSet::stereo(), true)
#endif
                     )
#endif
{
    CabbageUtilities::debug("Plugin constructor");
    csound = new Csound();

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

    csound->SetIsGraphable(true);
    csound->SetMakeGraphCallback(makeGraphCallback);
    csound->SetDrawGraphCallback(drawGraphCallback);
    csound->SetKillGraphCallback(killGraphCallback);
    csound->SetExitGraphCallback(exitGraphCallback);

    csound->SetParams(csoundParams);
    csound->SetOption((char*)"-n");
    csound->SetOption((char*)"-d");


    if(debugMode)
    {
        csoundDebuggerInit(csound->GetCsound());
        csoundSetBreakpointCallback(csound->GetCsound(), breakpointCallback, (void*)this);
        csoundSetInstrumentBreakpoint(csound->GetCsound(), 1, 413);
    }

    compileCsdFile(csdFile);
    numCsoundChannels = csound->GetNchnls();

    addMacros(csdFile.getFullPathName());
    csdFile.getParentDirectory().setAsCurrentWorkingDirectory();

    if(csdCompiledWithoutError())
    {
        csdKsmps = csound->GetKsmps();
        if(csound->GetSpout()==nullptr);
        CSspout = csound->GetSpout();
        CSspin  = csound->GetSpin();
        cs_scale = csound->Get0dBFS();
        csndIndex = csound->GetKsmps();

        //hack to allow tables to be set up correctly.
        // might be best to simply do an init run?
        csound->PerformKsmps();
        csound->SetScoreOffsetSeconds(0);
        csound->RewindScore();

        this->setLatencySamples(csound->GetKsmps());
    }
    else
        CabbageUtilities::debug("Csound could not compile your file?");

}

CsoundPluginProcessor::~CsoundPluginProcessor()
{
    CabbageUtilities::debug("Plugin destructor");

    if(csound)
    {
        csound = nullptr;
        csoundParams = nullptr;
        editorBeingDeleted (this->getActiveEditor());
    }
}
//==============================================================================
void CsoundPluginProcessor::initAllCsoundChannels(ValueTree cabbageData)
{
    for(int i = 0; i < cabbageData.getNumChildren(); i++)
    {
        const String typeOfWidget = CabbageWidgetData::getStringProp(cabbageData.getChild(i), CabbageIdentifierIds::type);
        if(CabbageWidgetData::getStringProp(cabbageData.getChild(i), "channeltype")=="string")
        {
            csound->SetChannel(CabbageWidgetData::getStringProp(cabbageData.getChild(i), CabbageIdentifierIds::channel).getCharPointer(),
                               CabbageWidgetData::getStringProp(cabbageData.getChild(i), CabbageIdentifierIds::text).toUTF8().getAddress());

        }
        else
        {
            csound->SetChannel(CabbageWidgetData::getStringProp(cabbageData.getChild(i), CabbageIdentifierIds::channel).getCharPointer(),
                               CabbageWidgetData::getNumProp(cabbageData.getChild(i), CabbageIdentifierIds::value));
        }

    }
}
//==============================================================================
void CsoundPluginProcessor::addMacros(String csdText)
{
    StringArray csdArray;
    String macroName, macroText;

    csdArray.addLines(csdText);
    for(int i=0; i<csdArray.size(); i++)
    {
        if(csdArray[i].trim().substring(0, 7)=="#define")
        {
            StringArray tokens;
            tokens.addTokens(csdArray[i].replace("#", "").trim() ," ");
            macroName = tokens[1];
            tokens.remove(0);
            tokens.remove(0);
            macroText = "\\\"" + tokens.joinIntoString(" ").replace("\"", "\\\\\\\"")+"\\\"";
            String fullMacro = "--omacro:"+macroName+"="+macroText+"\"";
            csound->SetOption(fullMacro.toUTF8().getAddress());
        }

        if(csdArray[i].contains("</Cabbage>"))
            i=csdArray.size();
    }
}
//==============================================================================
StringArray CsoundPluginProcessor::getTableStatement(int tableNum)
{
    StringArray fdata;
    fdata.add(String::empty);
    if(csCompileResult==OK)
    {
        MYFLT* argsPtr, *temp;
        int noOfArgs = csoundGetTableArgs(csound->GetCsound(), &argsPtr, tableNum);
        if(noOfArgs!=-1)
        {
            int tableSize = csound->GetTable(temp, tableNum);
            fdata.add(String(tableNum));
            fdata.add("0");
            fdata.add(String(tableSize));
            if(noOfArgs==0)
                fdata.add(String(1));
            else
                for(int i=0; i<noOfArgs; i++)
                {
                    fdata.add(String(argsPtr[i]));
                }
        }
    }
    return fdata;
}
//==============================================================================
const Array<float, CriticalSection> CsoundPluginProcessor::getTableFloats(int tableNum)
{
    Array<float, CriticalSection> points;
    if(csCompileResult==OK)
    {
        points.clear();

        int tableSize=0;
#ifndef Cabbage_No_Csound

        tableSize = csound->TableLength(tableNum);
        temp.clear();
        //not good if table size is -1!
        if(tableSize<0)
            return points;

        temp.reserve(tableSize);
        csound->TableCopyOut(tableNum, &temp[0]);
#else
        float *temp;
#endif
        if(tableSize>0)
            points = Array<float, CriticalSection>(&temp[0], tableSize);
    }
    return points;
}

int CsoundPluginProcessor::checkTable(int tableNum)
{
#ifndef Cabbage_No_Csound
    return  csound->TableLength(tableNum);
#else
    return -1;
#endif
}


//==============================================================================
String CsoundPluginProcessor::getCsoundOutput()
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
const String CsoundPluginProcessor::getName() const
{
    return JucePlugin_Name;
}

bool CsoundPluginProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool CsoundPluginProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

double CsoundPluginProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int CsoundPluginProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
    // so this should be at least 1, even if you're not really implementing programs.
}

int CsoundPluginProcessor::getCurrentProgram()
{
    return 0;
}

void CsoundPluginProcessor::setCurrentProgram (int index)
{
}

const String CsoundPluginProcessor::getProgramName (int index)
{
    return String();
}

void CsoundPluginProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void CsoundPluginProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void CsoundPluginProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool CsoundPluginProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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


void CsoundPluginProcessor::handleAsyncUpdate()
{
    receiveChannelDataFromCsound();
}

void CsoundPluginProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
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

    if(csdCompiledWithoutError())
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

                //slow down calls to these functions, no need for them to be firing at k-rate
                if (guiCycles>guiRefreshRate)
                {
                    guiCycles = 0;
                    triggerAsyncUpdate();

                }
                else
                    ++guiCycles;

                sendChannelDataToCsound();
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

void CsoundPluginProcessor::breakpointCallback(CSOUND *csound, debug_bkpt_info_t *bkpt_info, void *userdata)
{

    CsoundPluginProcessor* ud = (CsoundPluginProcessor *) userdata;
    const String instrument = "Instrument"+String(bkpt_info->breakpointInstr->p1);
    debug_variable_t *vp = bkpt_info->instrVarList;

    while (vp)
    {
        if (vp->name[0] != '#')
        {

            if (strcmp(vp->typeName, "i") == 0
                    || strcmp(vp->typeName, "k") == 0)
            {
                MYFLT *data = (MYFLT *) vp->data;
                ud->breakPointData.set(instrument, vp->name, data[0]);
            }
            else if(strcmp(vp->typeName, "S") == 0)
            {
                ud->breakPointData.set(instrument, vp->name, String((char *)vp->data));
            }
            else if (strcmp(vp->typeName, "a") == 0)
            {
                MYFLT *data = (MYFLT *) vp->data;
                ud->breakPointData.set(instrument, vp->name, String(data[0]));
            }
            else
            {

            }
        }
        vp = vp->next;
    }

    csoundDebugContinue(csound);


}

//==============================================================================
CsoundPluginProcessor::SignalDisplay* CsoundPluginProcessor::getSignalArray(String variableName, String displayType)
{
    for(int i=0; i<signalArrays.size(); i++)
    {
        if(signalArrays[i]->caption.contains(variableName))
        {
            if(displayType.isEmpty())
                return signalArrays[i];

            else if(displayType=="waveform" && !signalArrays[i]->caption.contains("fft"))
                return signalArrays[i];

            else if(displayType=="lissajous" && !signalArrays[i]->caption.contains("fft"))
                return signalArrays[i];

            else if(displayType!="waveform" && signalArrays[i]->caption.contains("fft"))
                return signalArrays[i];
        }
    }

    return new SignalDisplay("", -1, 0, 0, 0, 0);
}
//==============================================================================
bool CsoundPluginProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* CsoundPluginProcessor::createEditor()
{
    return new CsoundPluginEditor (*this);
}

//==============================================================================
void CsoundPluginProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void CsoundPluginProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//======================== CSOUND MIDI FUNCTIONS ================================
int CsoundPluginProcessor::OpenMidiInputDevice(CSOUND * csound, void **userData, const char* /*devName*/)
{
    *userData = csoundGetHostData(csound);
    if(!userData)
        CabbageUtilities::debug("\n\ncan't open midi in\n\n");
    return 0;
}

//==============================================================================
// Reads MIDI input data from host, gets called every time there is MIDI input to our plugin
//==============================================================================
int CsoundPluginProcessor::ReadMidiData(CSOUND* /*csound*/, void *userData,
                                        unsigned char *mbuf, int nbytes)
{
    CsoundPluginProcessor *midiData = (CsoundPluginProcessor *)userData;
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
int CsoundPluginProcessor::OpenMidiOutputDevice(CSOUND * csound, void **userData, const char* /*devName*/)
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
int CsoundPluginProcessor::WriteMidiData(CSOUND* /*csound*/, void *_userData,
        const unsigned char *mbuf, int nbytes)
{
    CsoundPluginProcessor *userData = (CsoundPluginProcessor *)_userData;
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

void CsoundPluginProcessor::makeGraphCallback(CSOUND *csound, WINDAT *windat, const char * /*name*/)
{
    CsoundPluginProcessor *ud = (CsoundPluginProcessor *) csoundGetHostData(csound);
    CsoundPluginProcessor::SignalDisplay* display = new CsoundPluginProcessor::SignalDisplay(String(windat->caption),windat->windid, windat->oabsmax, windat->min, windat->max, windat->npts);

    bool addDisplay = true;
    for(int i=0; i<ud->signalArrays.size(); i++)
    {
        if(ud->signalArrays[i]->caption==windat->caption)
            addDisplay  = false;
    }

    if(addDisplay)
        ud->signalArrays.add(display);
}

void CsoundPluginProcessor::drawGraphCallback(CSOUND *csound, WINDAT *windat)
{
    CsoundPluginProcessor *ud = (CsoundPluginProcessor *) csoundGetHostData(csound);

    Array<float, CriticalSection> tablePoints;
    //only take all sample sif dealing with fft, waveforms and lissajous curves can be drawn with less samples
    tablePoints = Array<float, CriticalSection>(&windat->fdata[0], windat->npts);

    ud->getSignalArray(windat->caption)->setPoints(tablePoints);
    //ud->signalArrays.getUnchecked(windat->windid)->setPoints(tablePoints);
    ud->updateSignalDisplay = true;
}

void CsoundPluginProcessor::killGraphCallback(CSOUND *csound, WINDAT *windat)
{
    CsoundPluginProcessor *udata = (CsoundPluginProcessor *) csoundGetHostData(csound);
    CabbageUtilities::debug("killGraphCallback");
}

int CsoundPluginProcessor::exitGraphCallback(CSOUND *csound)
{
    CsoundPluginProcessor *udata = (CsoundPluginProcessor *) csoundGetHostData(csound);
    CabbageUtilities::debug("exitGraphCallback");
    return 0;
}




