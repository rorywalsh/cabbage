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
CsoundPluginProcessor::CsoundPluginProcessor (File csdFile, const int ins, const int outs, bool debugMode)
    : csdFile (csdFile), AudioProcessor (BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
                                         .withInput  ("Input",  (ins==2 ? AudioChannelSet::stereo() : AudioChannelSet::discreteChannels (ins)), true)
#endif
                                         .withOutput ("Output", (outs == 2 ? AudioChannelSet::stereo() : AudioChannelSet::discreteChannels (outs)), true)
#endif
                                        )
{

    //this->getBusesLayout().inputBuses.add(AudioChannelSet::discreteChannels(17));

    CabbageUtilities::debug ("Plugin constructor");


}

CsoundPluginProcessor::~CsoundPluginProcessor()
{
	resetCsound();
}

void CsoundPluginProcessor::resetCsound()
{
	Logger::setCurrentLogger(nullptr);

	CabbageUtilities::debug("Plugin destructor");
	Logger::setCurrentLogger(nullptr);

	if (csound)
	{
		csound = nullptr;
		csoundParams = nullptr;
		editorBeingDeleted(this->getActiveEditor());
	}
}

//==============================================================================
//==============================================================================
bool CsoundPluginProcessor::setupAndCompileCsound(File csdFile, File filePath, int sr, bool debugMode)
{
	csound = new Csound();
	csdFilePath = filePath;
	csdFilePath.setAsCurrentWorkingDirectory();
	csound->SetHostImplementedMIDIIO(true);
	csound->SetHostImplementedAudioIO(1, 0);
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


	csound->SetOption((char*)"-n");
	csound->SetOption((char*)"-d");


	if (debugMode)
	{
		csoundDebuggerInit(csound->GetCsound());
		csoundSetBreakpointCallback(csound->GetCsound(), breakpointCallback, (void*)this);
		csoundSetInstrumentBreakpoint(csound->GetCsound(), 1, 413);
		csoundParams->ksmps_override = 4410;
	}

	//instrument must at least be stereo
	numCsoundChannels = CabbageUtilities::getHeaderInfo(csdFile.loadFileAsString(), "nchnls");
	const int requestedSampleRate = CabbageUtilities::getHeaderInfo(csdFile.loadFileAsString(), "sr");
	const int requestedKsmpsRate = CabbageUtilities::getHeaderInfo(csdFile.loadFileAsString(), "ksmps");
	csoundParams->nchnls_override = numCsoundChannels;
	//only override sample rate if user insists
#if !defined(Cabbage_IDE_Build)
	PluginHostType pluginType;
	if (pluginType.isFruityLoops())
	{
		csoundParams->ksmps_override = 1;
	}
#else
	if (requestedKsmpsRate == -1)
		csoundParams->ksmps_override = 32;
#endif
	csoundParams->sample_rate_override = requestedSampleRate>0 ? requestedSampleRate : sr;

	csound->SetParams(csoundParams);

	if (csdFile.loadFileAsString().contains("<Csound") || csdFile.loadFileAsString().contains("</Csound"))
		compileCsdFile(csdFile);
	else
	{
#ifdef CabbagePro
		compileCsdString(Encrypt::decode(csdFile));
		csound->Start();
#endif
}

	addMacros(csdFile.getFullPathName());


	if (csdCompiledWithoutError())
	{
		csdKsmps = csound->GetKsmps();
		CSspout = csound->GetSpout();
		CSspin = csound->GetSpin();
		cs_scale = csound->Get0dBFS();
		csndIndex = csound->GetKsmps();

		this->setLatencySamples(csound->GetKsmps());
	}
	else
		CabbageUtilities::debug("Csound could not compile your file?");

    return csdCompiledWithoutError();
}


void CsoundPluginProcessor::createFileLogger (File csdFile)
{
    String logFileName = csdFile.getParentDirectory().getFullPathName() + String ("/") + csdFile.getFileNameWithoutExtension() + String ("_Log.txt");
    fileLogger = new FileLogger (File (logFileName), String ("Cabbage Log.."));
    Logger::setCurrentLogger (fileLogger);
}
//==============================================================================
void CsoundPluginProcessor::initAllCsoundChannels (ValueTree cabbageData)
{
    for (int i = 0; i < cabbageData.getNumChildren(); i++)
    {
        const String typeOfWidget = CabbageWidgetData::getStringProp (cabbageData.getChild (i), CabbageIdentifierIds::type);

        if (CabbageWidgetData::getStringProp (cabbageData.getChild (i), CabbageIdentifierIds::channeltype) == "string")
        {
            if (typeOfWidget == CabbageWidgetTypes::filebutton)
            {
                csound->SetStringChannel (CabbageWidgetData::getStringProp (cabbageData.getChild (i), CabbageIdentifierIds::channel).getCharPointer(),
                                          CabbageWidgetData::getStringProp (cabbageData.getChild (i), CabbageIdentifierIds::file).toUTF8().getAddress());
            }

            else
            {
                csound->SetStringChannel (CabbageWidgetData::getStringProp (cabbageData.getChild (i), CabbageIdentifierIds::channel).getCharPointer(),
                                          CabbageWidgetData::getStringProp (cabbageData.getChild (i), CabbageIdentifierIds::text).toUTF8().getAddress());
            }


        }
        else
        {
            if (CabbageWidgetData::getStringProp (cabbageData.getChild (i), CabbageIdentifierIds::type) == CabbageWidgetTypes::xypad)
            {
                csound->SetChannel (CabbageWidgetData::getStringProp (cabbageData.getChild (i), CabbageIdentifierIds::xchannel).getCharPointer(),
                                    CabbageWidgetData::getNumProp (cabbageData.getChild (i), CabbageIdentifierIds::valuex));
                csound->SetChannel (CabbageWidgetData::getStringProp (cabbageData.getChild (i), CabbageIdentifierIds::ychannel).getCharPointer(),
                                    CabbageWidgetData::getNumProp (cabbageData.getChild (i), CabbageIdentifierIds::valuey));
            }
            else if (CabbageWidgetData::getStringProp (cabbageData.getChild (i), CabbageIdentifierIds::type) == CabbageWidgetTypes::hrange
                     ||CabbageWidgetData::getStringProp (cabbageData.getChild (i), CabbageIdentifierIds::type) == CabbageWidgetTypes::vrange)
            {
                var channels = CabbageWidgetData::getProperty(cabbageData.getChild(i), CabbageIdentifierIds::channel);
                if(channels.size()==2)
                {
                    const var minValue = CabbageWidgetData::getProperty (cabbageData.getChild (i), CabbageIdentifierIds::minvalue);
                    csound->SetChannel (channels[0].toString().getCharPointer(), float (minValue));

                    const var maxValue = CabbageWidgetData::getProperty (cabbageData.getChild (i), CabbageIdentifierIds::maxvalue);
                    csound->SetChannel (channels[0].toString().getCharPointer(), float (maxValue));
                }

            }
            else
            {
                const var value = CabbageWidgetData::getProperty (cabbageData.getChild (i), CabbageIdentifierIds::value);
                csound->SetChannel (CabbageWidgetData::getStringProp (cabbageData.getChild (i), CabbageIdentifierIds::channel).getCharPointer(),
                                    float (value));
            }

        }

    }

    char path[8192] = {0};

    if (CabbageUtilities::getTargetPlatform() == CabbageUtilities::TargetPlatformTypes::Win32)
    {
        csound->SetChannel ("CSD_PATH", csdFilePath.getParentDirectory().getFullPathName().replace ("\\", "\\\\").toUTF8().getAddress());
    }
    else
    {
        csound->SetChannel ("CSD_PATH", csdFilePath.getFullPathName().toUTF8().getAddress());
    }

    csound->SetStringChannel ("LAST_FILE_DROPPED", "");

    csdFilePath.setAsCurrentWorkingDirectory();

	if(SystemStats::getOperatingSystemType() == SystemStats::OperatingSystemType::Linux)
		csound->SetChannel ("LINUX", 1.0);
	if(SystemStats::getOperatingSystemType() == SystemStats::OperatingSystemType::MacOSX)
		csound->SetChannel ("MAC", 1.0);
	if(SystemStats::getOperatingSystemType() == SystemStats::OperatingSystemType::Windows)
		csound->SetChannel ("Windows", 1.0);		


    if (CabbageUtilities::getTarget() != CabbageUtilities::TargetTypes::IDE)
    {
        csound->SetChannel ("IS_A_PLUGIN", 1.0);

        if (getPlayHead() != 0 && getPlayHead()->getCurrentPosition (hostInfo))
        {
            csound->SetChannel (CabbageIdentifierIds::hostbpm.toUTF8(), hostInfo.bpm);
            csound->SetChannel (CabbageIdentifierIds::timeinseconds.toUTF8(), hostInfo.timeInSeconds);
            csound->SetChannel (CabbageIdentifierIds::isplaying.toUTF8(), hostInfo.isPlaying);
            csound->SetChannel (CabbageIdentifierIds::isrecording.toUTF8(), hostInfo.isRecording);
            csound->SetChannel (CabbageIdentifierIds::hostppqpos.toUTF8(), hostInfo.ppqPosition);
            csound->SetChannel (CabbageIdentifierIds::timeinsamples.toUTF8(), hostInfo.timeInSamples);
            csound->SetChannel (CabbageIdentifierIds::timeSigDenom.toUTF8(), hostInfo.timeSigDenominator);
            csound->SetChannel (CabbageIdentifierIds::timeSigNum.toUTF8(), hostInfo.timeSigNumerator);
        }
    }
    else
        csound->SetChannel ("IS_A_PLUGIN", 0.0);

    csound->PerformKsmps();

}
//==============================================================================
void CsoundPluginProcessor::addMacros (String csdText)
{
    StringArray csdArray;
    String macroName, macroText;

    csdArray.addLines (csdText);
//
//    String width = "--macro:SCREEN_WIDTH="+String(screenWidth);
//    String height = "--macro:SCREEN_HEIGHT="+String(screenHeight);
//    csound->SetOption (width.toUTF8().getAddress());
//    csound->SetOption (height.toUTF8().getAddress());
    StringArray tokens;


    for (int i = 0; i < csdArray.size(); i++)
    {

        if (csdArray[i].trim().substring (0, 7) == "#define")
        {
            tokens.addTokens (csdArray[i].replace ("#", "").trim() , " ");
            macroName = tokens[1];
            tokens.remove (0);
            tokens.remove (0);
            macroText = "\\\"" + tokens.joinIntoString (" ").replace ("\"", "\\\\\\\"") + "\\\"";
            String fullMacro = "--omacro:" + macroName + "=" + macroText + "\"";
            csound->SetOption (fullMacro.toUTF8().getAddress());
        }

        if (csdArray[i].contains ("</Cabbage>"))
            i = csdArray.size();
    }

}

//==============================================================================
void CsoundPluginProcessor::createMatrixEventSequencer(int rows, int cols, String channel)
{
    MatrixEventSequencer* matrix = new MatrixEventSequencer(channel);
    (channel);

    for (int i = 0 ; i < cols ; i++)
    {
        matrix->events.add (new StringArray());

        for ( int y = 0 ; y < rows ; y++)
        {
            matrix->events[i]->add(" ");
        }
    }

    matrixEventSequencers.add(matrix);
}

void CsoundPluginProcessor::setMatrixEventSequencerCellData(int col, int row, String channel, String data)
{
    for (int i = 0 ; i < matrixEventSequencers.size(); i++)
    {
        if (matrixEventSequencers[i]->channel == channel)
        {
            matrixEventSequencers[i]->setEventString(col, row, data);
        }
    }
}

//==============================================================================
StringArray CsoundPluginProcessor::getTableStatement (int tableNum)
{
    StringArray fdata;
    fdata.add (String::empty);

    if (csCompileResult == OK)
    {
        MYFLT* argsPtr, *temp;
        int noOfArgs = csoundGetTableArgs (csound->GetCsound(), &argsPtr, tableNum);

        if (noOfArgs != -1)
        {
            int tableSize = csound->GetTable (temp, tableNum);
            fdata.add (String (tableNum));
            fdata.add ("0");
            fdata.add (String (tableSize));

            if (noOfArgs == 0)
                fdata.add (String (1));
            else
                for (int i = 0; i < noOfArgs; i++)
                {
                    fdata.add (String (argsPtr[i]));
                }
        }
    }

    return fdata;
}
//==============================================================================
const Array<float, CriticalSection> CsoundPluginProcessor::getTableFloats (int tableNum)
{
    Array<float, CriticalSection> points;

    if (csCompileResult == OK)
    {

        const int tableSize = csound->TableLength (tableNum);;

        if (tableSize < 0)
            return points;

        std::vector<double> temp (tableSize);

        csound->TableCopyOut (tableNum, &temp[0]);

        if (tableSize > 0)
            points = Array<float, CriticalSection> (&temp[0], tableSize);
    }

    return points;
}

int CsoundPluginProcessor::checkTable (int tableNum)
{
    return  csound->TableLength (tableNum);
}


//==============================================================================
const String CsoundPluginProcessor::getCsoundOutput()
{
    if (csound!=nullptr)
    {
        const int messageCnt = csound->GetMessageCnt();
        csoundOutput = "";

        if (messageCnt == 0)
            return csoundOutput;

        while (csound->GetMessageCnt() > 0)
        {
            csoundOutput += csound->GetFirstMessage();
            csound->PopFirstMessage();
        }

        Logger::writeToLog (csoundOutput);

        if (disableLogging == true)
            this->suspendProcessing (true);

        return csoundOutput;
    }

    return String::empty;
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
    if(samplingRate != sampleRate)
    {
        //if sampling rate is other than default or has been changed, recompile..
        samplingRate = sampleRate;
        setupAndCompileCsound(csdFile, csdFilePath);
    }
    
    CabbageUtilities::debug(AudioProcessor::getChannelLayoutOfBus(true, 0).getDescription());
	CabbageUtilities::debug(AudioProcessor::getChannelLayoutOfBus(false, 0).getDescription());
}

void CsoundPluginProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

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
//bool CsoundPluginProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
//{
//#if JucePlugin_IsMidiEffect
//    ignoreUnused (layouts);
//    return true;
//#else
//
//    //audio units can only be stereo for now
//    if(this->wrapperType == AudioProcessor::wrapperType_AudioUnit)
//        return true;
//
//    const int inputs = layouts.getNumChannels (true, busIndex);
//    const int outputs = layouts.getNumChannels (false, busIndex);
//
//
//    if (inputs == numCsoundChannels && outputs == numCsoundChannels)
//        return true;
//
//    return false;
//#endif
//}

//==========================================================================
void CsoundPluginProcessor::triggerCsoundEvents()
{

}

void CsoundPluginProcessor::handleAsyncUpdate()
{
    getChannelDataFromCsound();
    sendChannelDataToCsound();
}

void CsoundPluginProcessor::sendHostDataToCsound()
{
    if (CabbageUtilities::getTarget() != CabbageUtilities::TargetTypes::IDE)
    {
        if (AudioPlayHead* const ph = getPlayHead())
        {
            AudioPlayHead::CurrentPositionInfo hostInfo;
            
            if (ph->getCurrentPosition (hostInfo))
            {
                csound->SetChannel (CabbageIdentifierIds::hostbpm.toUTF8(), hostInfo.bpm);
                csound->SetChannel (CabbageIdentifierIds::timeinseconds.toUTF8(), hostInfo.timeInSeconds);
                csound->SetChannel (CabbageIdentifierIds::isplaying.toUTF8(), hostInfo.isPlaying);
                csound->SetChannel (CabbageIdentifierIds::isrecording.toUTF8(), hostInfo.isRecording);
                csound->SetChannel (CabbageIdentifierIds::hostppqpos.toUTF8(), hostInfo.ppqPosition);
                csound->SetChannel (CabbageIdentifierIds::timeinsamples.toUTF8(), hostInfo.timeInSamples);
                csound->SetChannel (CabbageIdentifierIds::timeSigDenom.toUTF8(), hostInfo.timeSigDenominator);
                csound->SetChannel (CabbageIdentifierIds::timeSigNum.toUTF8(), hostInfo.timeSigNumerator);
            }
        }
    }
}
void CsoundPluginProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    float** audioBuffers = buffer.getArrayOfWritePointers();
    const int numSamples = buffer.getNumSamples();
    MYFLT newSamp;
    int result = -1;


	const int output_channel_count = (numCsoundChannels > getTotalNumOutputChannels() ? getTotalNumOutputChannels() : numCsoundChannels);
	const int outputs =  getTotalNumOutputChannels();;



    //if no inputs are used clear buffer in case it's not empty..
    if (getTotalNumInputChannels() == 0)
        buffer.clear();

    if (csdCompiledWithoutError())
    {
        keyboardState.processNextMidiBuffer (midiMessages, 0, numSamples, true);
        midiBuffer = midiMessages;

#if JucePlugin_ProducesMidiOutput

        if (!midiOutputBuffer.isEmpty())
        {
            midiMessages.swapWith (midiOutputBuffer);
        }
        else
            midiMessages.clear();

#endif

        //mute unused channels
        for (int channelsToClear = output_channel_count; channelsToClear < getTotalNumOutputChannels(); ++channelsToClear)
        {
            buffer.clear (channelsToClear, 0, buffer.getNumSamples());
        }

        for (int i = 0; i < numSamples; i++, ++csndIndex)
        {
            if (csndIndex == csdKsmps)
            {
                result = csound->PerformKsmps();

                if (result == 0)
                {
                    //slow down calls to these functions, no need for them to be firing at k-rate
                    if (guiCycles > guiRefreshRate)
                    {
                        guiCycles = 0;
                        triggerAsyncUpdate();
                    }
                    else
                        ++guiCycles;

                    //trigger any Csound score event on each k-boundary
                    triggerCsoundEvents();
                    sendHostDataToCsound();

                    disableLogging = false;
                }
                else
                {
                    disableLogging = true;
                    return; //return as soon as Csound has stopped
                }

                csndIndex = 0;

            }

            if (result == 0)
            {
                pos = csndIndex * output_channel_count;

                for (int channel = 0; channel < output_channel_count; ++channel)
                {
                    float*& current_sample = audioBuffers[channel];
                    newSamp = *current_sample * cs_scale;
                    CSspin[pos] = newSamp;
                    *current_sample = (CSspout[pos] / cs_scale);
                    ++current_sample;
                    ++pos;

                }
            }
            else
                buffer.clear();
        }


    }//if not compiled just mute output
    else
    {
        for (int channel = 0; channel < getTotalNumOutputChannels(); ++channel)
        {
            buffer.clear (channel, 0, buffer.getNumSamples());
        }
    }
}

//==============================================================================
void CsoundPluginProcessor::breakpointCallback (CSOUND* csound, debug_bkpt_info_t* bkpt_info, void* userdata)
{

    CsoundPluginProcessor* ud = (CsoundPluginProcessor*) userdata;
    const String instrument = "Instrument" + String (bkpt_info->breakpointInstr->p1);
    debug_variable_t* vp = bkpt_info->instrVarList;

    while (vp)
    {
        if (vp->name[0] != '#')
        {

            if (strcmp (vp->typeName, "i") == 0
                || strcmp (vp->typeName, "k") == 0)
            {
                MYFLT* data = (MYFLT*) vp->data;
                ud->breakPointData.set (instrument, vp->name, data[0]);
            }
            else if (strcmp (vp->typeName, "S") == 0)
            {
                ud->breakPointData.set (instrument, vp->name, String ((char*)vp->data));
            }
            else if (strcmp (vp->typeName, "a") == 0)
            {
                MYFLT* data = (MYFLT*) vp->data;
                ud->breakPointData.set (instrument, vp->name, String (data[0]));
            }
            else
            {

            }
        }

        vp = vp->next;
    }

    csoundDebugContinue (csound);


}

//==============================================================================
CsoundPluginProcessor::SignalDisplay* CsoundPluginProcessor::getSignalArray (String variableName, String displayType)
{
    for (int i = 0; i < signalArrays.size(); i++)
    {
        if (signalArrays[i]->caption.isNotEmpty() && signalArrays[i]->caption.contains (variableName))
        {
            if (displayType.isEmpty())
                return signalArrays[i];

            else if (displayType == "waveform" && !signalArrays[i]->caption.contains ("fft"))
                return signalArrays[i];

            else if (displayType == "lissajous" && !signalArrays[i]->caption.contains ("fft"))
                return signalArrays[i];

            else if (displayType != "waveform" && signalArrays[i]->caption.contains ("fft"))
                return signalArrays[i];
        }
    }

    return new SignalDisplay ("", -1, 0, 0, 0, 0);
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
int CsoundPluginProcessor::OpenMidiInputDevice (CSOUND* csound, void** userData, const char* /*devName*/)
{
    *userData = csoundGetHostData (csound);
    return 0;
}

//==============================================================================
// Reads MIDI input data from host, gets called every time there is MIDI input to our plugin
//==============================================================================
int CsoundPluginProcessor::ReadMidiData (CSOUND* /*csound*/, void* userData,
                                         unsigned char* mbuf, int nbytes)
{
    CsoundPluginProcessor* midiData = (CsoundPluginProcessor*)userData;

    if (!userData)
    {
        CabbageUtilities::debug ("\nInvalid");
        return 0;
    }

    int cnt = 0;

    if (!midiData->midiBuffer.isEmpty() && cnt <= (nbytes - 3))
    {
        MidiMessage message (0xf4, 0, 0, 0);
        MidiBuffer::Iterator i (midiData->midiBuffer);
        int messageFrameRelativeTothisProcess;

        while (i.getNextEvent (message, messageFrameRelativeTothisProcess))
        {
            const uint8* data = message.getRawData();
            *mbuf++ = *data++;

            if(message.isChannelPressure() || message.isProgramChange())
            {
                *mbuf++ = *data++;
                cnt += 2;
            }
            else
            {
                *mbuf++ = *data++;
                *mbuf++ = *data++;
                cnt  += 3;
            }
        }

        midiData->midiBuffer.clear();
    }

    return cnt;

}

//==============================================================================
// Opens MIDI output device, adding -QN to your CsOptions will causes this method to be called
// as soon as your plugin loads
//==============================================================================
int CsoundPluginProcessor::OpenMidiOutputDevice (CSOUND* csound, void** userData, const char* /*devName*/)
{
    *userData = csoundGetHostData (csound);
    return 0;
}

//==============================================================================
// Write MIDI data to plugin's MIDI output. Each time Csound outputs a midi message this
// method should be called. Note: you must have -Q set in your CsOptions
//==============================================================================
int CsoundPluginProcessor::WriteMidiData (CSOUND* /*csound*/, void* _userData,
                                          const unsigned char* mbuf, int nbytes)
{
    CsoundPluginProcessor* userData = (CsoundPluginProcessor*)_userData;

    if (!userData)
    {
        CabbageUtilities::debug ("\n\nInvalid");
        return 0;
    }

    MidiMessage message (mbuf, nbytes, 0);

    userData->midiOutputBuffer.addEvent (message, 0);
    return nbytes;
}

//===========================================================================================
// graphing functions...
//===========================================================================================

void CsoundPluginProcessor::makeGraphCallback (CSOUND* csound, WINDAT* windat, const char* /*name*/)
{
    CsoundPluginProcessor* ud = (CsoundPluginProcessor*) csoundGetHostData (csound);
    SignalDisplay* display = new SignalDisplay (String (windat->caption), windat->windid, windat->oabsmax, windat->min, windat->max, windat->npts);

    bool addDisplay = true;

    for (int i = 0; i < ud->signalArrays.size(); i++)
    {
        if (ud->signalArrays[i]->caption == windat->caption)
            addDisplay  = false;
    }

    if (addDisplay)
        ud->signalArrays.add (display);
}

void CsoundPluginProcessor::drawGraphCallback (CSOUND* csound, WINDAT* windat)
{
    CsoundPluginProcessor* ud = (CsoundPluginProcessor*) csoundGetHostData (csound);
    Array<float, CriticalSection> tablePoints;
    //only take all samples if dealing with fft, waveforms and lissajous curves can be drawn with less samples
    tablePoints = Array<float, CriticalSection> (&windat->fdata[0], windat->npts);
    ud->getSignalArray (windat->caption)->setPoints (tablePoints);
    ud->updateSignalDisplay = true;
}

void CsoundPluginProcessor::killGraphCallback (CSOUND* csound, WINDAT* windat)
{

}

int CsoundPluginProcessor::exitGraphCallback (CSOUND* csound)
{
    return 0;
}





