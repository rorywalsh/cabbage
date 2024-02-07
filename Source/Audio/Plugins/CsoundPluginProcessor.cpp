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

#include <memory>
#include "CsoundPluginEditor.h"

//==============================================================================
CsoundPluginProcessor::CsoundPluginProcessor (File selectedCsdFile, const BusesProperties& ioBuses)
    :
#if Stereo_Mono_Only
    AudioProcessor (BusesProperties()
                         #if ! JucePlugin_IsMidiEffect
                          #if ! JucePlugin_IsSynth
                           .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                          #endif
                           .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                         #endif
                           ),
#else
    AudioProcessor (ioBuses),
#endif
csdFile (selectedCsdFile)
{
    hostInfo = {};  
	matchingNumberOfIOChannels = getTotalNumInputChannels()==getTotalNumOutputChannels();

#if ! JucePlugin_IsSynth && ! Cabbage_IDE_Build
    numCsoundInputChannels = getTotalNumInputChannels();
    numCsoundOutputChannels = getTotalNumOutputChannels();
    CabbageUtilities::debug("Cabbage Csound Constructor - Requested input channels:", numCsoundInputChannels);
    CabbageUtilities::debug("Cabbage Csound Constructor - Requested output channels:", numCsoundOutputChannels);
      
#else  
    numCsoundOutputChannels = getTotalNumOutputChannels();
    CabbageUtilities::debug("Constructor - Requested output channels:", numCsoundOutputChannels);
    
    backgroundThread.startThread();

#endif

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
        
        destroyCsoundGlobalVars();
#if !defined(Cabbage_Lite) && !JucePlugin_Build_Standalone
		csound = nullptr;
#endif
		csoundParams = nullptr;
		editorBeingDeleted(this->getActiveEditor());
	}
}

//==============================================================================
void CsoundPluginProcessor::startRecording(const File& file, int bitDepth)
{
    stopRecording();

    if (samplingRate > 0)
    {
        // Create an OutputStream to write to our destination file...
        file.deleteFile();

        if (auto fileStream = std::unique_ptr<FileOutputStream> (file.createOutputStream()))
        {
            // Now create a WAV writer object that writes to our output stream...
            WavAudioFormat wavFormat;

            if (auto writer = wavFormat.createWriterFor (fileStream.get(), samplingRate, numCsoundOutputChannels, bitDepth, {}, 0))
            {
                fileStream.release(); // (passes responsibility for deleting the stream to the writer object that is now using it)

                // Now we'll create one of these helper objects which will act as a FIFO buffer, and will
                // write the data to disk on our background thread.
                threadedWriter.reset (new AudioFormatWriter::ThreadedWriter (writer, backgroundThread, 32768));

                // And now, swap over our active writer pointer so that the audio callback will start using it..
                const ScopedLock sl (writerLock);
                activeWriter = threadedWriter.get();
            }
        }
    }
}

void CsoundPluginProcessor::stopRecording()
{
    // First, clear this pointer to stop the audio callback from using our writer object..
    {
        const ScopedLock sl (writerLock);
        activeWriter = nullptr;
    }

    // Now we can delete the writer object. It's done in this order because the deletion could
    // take a little time while remaining data gets flushed to disk, so it's best to avoid blocking
    // the audio callback while this happens.

    threadedWriter.reset();
}
//==============================================================================
void CsoundPluginProcessor::destroyCsoundGlobalVars()
{
    if(getCsound())
    {
        auto** pd = (CabbagePersistentData**)getCsound()->QueryGlobalVariable("cabbageData");
        if (pd != nullptr)
            getCsound()->DestroyGlobalVariable("cabbageData");

        auto** wi = (CabbageWidgetIdentifiers**)getCsound()->QueryGlobalVariable("cabbageWidgetData");
        if (wi != nullptr)
            getCsound()->DestroyGlobalVariable("cabbageWidgetData");


        auto** vt = (CabbageWidgetsValueTree**)getCsound()->QueryGlobalVariable("cabbageWidgetsValueTree");
        if (vt != nullptr) {
            getCsound()->DestroyGlobalVariable("cabbageWidgetsValueTree");
        }
        
        auto** ps = (CabbageWidgetsValueTree**)getCsound()->QueryGlobalVariable("cabbageGlobalPreset");
        if (ps != nullptr) {
            getCsound()->DestroyGlobalVariable("cabbageGlobalPreset");
        }
    }
}

void CsoundPluginProcessor::createCsoundGlobalVars(const ValueTree& cabbageData)
{
    auto** pd = (CabbagePersistentData**)getCsound()->QueryGlobalVariable("cabbageData");
    if (pd == nullptr) {
        getCsound()->CreateGlobalVariable("cabbageData", sizeof(CabbagePersistentData*));
        pd = (CabbagePersistentData**)getCsound()->QueryGlobalVariable("cabbageData");
        *pd = new CabbagePersistentData();
        auto pdClass = *pd;
        pdClass->data = getInternalState().toStdString();
        //DBG(pdClass->data);
    }

    auto** wi = (CabbageWidgetIdentifiers**)getCsound()->QueryGlobalVariable("cabbageData");
    if (wi == nullptr) {
        getCsound()->CreateGlobalVariable("cabbageWidgetData", sizeof(CabbageWidgetIdentifiers*));
    }

#if Bluetooth
    auto** ps = (CabbagePresetData**)getCsound()->QueryGlobalVariable("cabbageGlobalPreset");
    if (ps == nullptr) {
        getCsound()->CreateGlobalVariable("cabbageGlobalPreset", sizeof(CabbagePresetData*));
        ps = (CabbagePresetData**)getCsound()->QueryGlobalVariable("cabbageGlobalPreset");
        *ps = new CabbagePresetData();
        auto p = *ps;
        p->data = "";
    }
#endif
    auto** vt = (CabbageWidgetsValueTree**)getCsound()->QueryGlobalVariable("cabbageWidgetsValueTree");
    if (vt == nullptr) {
        getCsound()->CreateGlobalVariable("cabbageWidgetsValueTree", sizeof(CabbageWidgetsValueTree*));
        vt = (CabbageWidgetsValueTree**)getCsound()->QueryGlobalVariable("cabbageWidgetsValueTree");
        *vt = new CabbageWidgetsValueTree();
        auto valueTree = *vt;
        valueTree->data = cabbageData;
    }
}
// 
//==============================================================================
bool CsoundPluginProcessor::setupAndCompileCsound(File currentCsdFile, File filePath, int sr, bool debugMode)
{
    
    csdFile = currentCsdFile;
    String csdFileText;
    StringArray csdLines;

    csdLines.addLines(csdFile.loadFileAsString());
    csdFileText = csdFile.loadFileAsString();
   
    for (auto line : csdLines)
    {
        ValueTree temp("temp");
        
        if(line.contains("populate") && line.contains("snaps") && line.contains("combobox"))
            line = line.replace("combobox", "combobox channelType(\"string\")");
        
        CabbageWidgetData::setWidgetState(temp, line, 0);
        const String filetype = CabbageWidgetData::getStringProp(temp, CabbageIdentifierIds::filetype);
        const String typeOfWidget = CabbageWidgetData::getStringProp(temp, CabbageIdentifierIds::type);

        if (typeOfWidget == CabbageWidgetTypes::form)
        {
            if(CabbageWidgetData::getStringProp(temp, CabbageIdentifierIds::opcodedir).isNotEmpty()) {
                const String opcodeDir = csdFile.getParentDirectory().getChildFile(
                        CabbageWidgetData::getStringProp(temp, CabbageIdentifierIds::opcodedir)).getFullPathName();
#if JUCE_MAC
                csoundSetGlobalEnv("OPCODE6DIR64", opcodeDir.toUTF8().getAddress());
#else
                csoundSetOpcodedir(opcodeDir.toUTF8().getAddress());
#endif
            }
#if Cabbage_IDE_Build == 0
            if (CabbageWidgetData::getStringProp(temp, CabbageIdentifierIds::opcode6dir64).isNotEmpty())
            {
                const String opcodeDir = csdFile.getParentDirectory().getChildFile(
                    CabbageWidgetData::getStringProp(temp, CabbageIdentifierIds::opcode6dir64)).getFullPathName();
                //
#ifdef JUCE_WINDOWS
                //csound->SetGlobalEnv("OPCODE6DIR64", opcodeDir.toUTF8().getAddress());
                String env = "OPCODE6DIR64=" + opcodeDir;
                _putenv(env.toUTF8().getAddress());
#endif
             }
#endif
            if (CabbageWidgetData::getNumProp(temp, CabbageIdentifierIds::latency) == -1) {
                preferredLatency = -1;
            }
        }

    }
    
    CabbageUtilities::debug(csdFile.getFullPathName());
    
    // the host should respect the default inputs and outs, which are determined by the
    // nhcnls and nchnls_i variables in Csound. But the host is king. If it requested a different
    // config, we must adhere to it.

#if ! JucePlugin_IsSynth
    const int inputs = getTotalNumInputChannels() - numSideChainChannels;
    numCsoundInputChannels = inputs + numSideChainChannels;
    numCsoundOutputChannels = getTotalNumOutputChannels();
    CabbageUtilities::debug("SetupAndCompile - Requested input channels:", numCsoundInputChannels);
#else
    //numCsoundOutputChannels = getBus(false, 0)->getNumberOfChannels();
    numCsoundOutputChannels = CabbageUtilities::getHeaderInfo(csdFileText, "nchnls");
    //numCsoundOutputChannels = getTotalNumOutputChannels();
#endif


    CabbageUtilities::debug("SetupAndCompile - Requested output channels:", numCsoundOutputChannels);
    CabbageUtilities::debug("SetupAndCompile - Requested sampling rate:", samplingRate);
    //int test = csound->SetGlobalEnv("OPCODE6DIR64", );
    CabbageUtilities::debug("Env var set");
    //csoundSetOpcodedir("/Library/Frameworks/CsoundLib64.framework/Versions/6.0/Resources/Opcodes64");
    
    Logger::writeToLog(String::formatted("Resetting csound ...\ncsound = 0x%p", csound.get()));

    //reset Csound in case it is hanging around from a previous run
    resetCsound();
	csound = std::make_unique<Csound> ();
    
	csdFilePath = filePath;
	//csdFilePath.setAsCurrentWorkingDirectory();
	csound->SetHostImplementedMIDIIO(true);
	csound->SetHostImplementedAudioIO(1, 0);
	csound->SetHostData(this);

    csnd::plugin<StrToFile>((csnd::Csound*) getCsound()->GetCsound(), "strToFile.SSO", "i", "SSO", csnd::thread::i);
    csnd::plugin<FileToStr>((csnd::Csound*) getCsound()->GetCsound(), "fileToStr.i", "S", "S", csnd::thread::i);

    csnd::plugin<ChannelStateSave>((csnd::Csound*) getCsound()->GetCsound(), "cabbageChannelStateSave.i", "i", "S", csnd::thread::i);
    csnd::plugin<ChannelStateSave>((csnd::Csound*) getCsound()->GetCsound(), "cabbageChannelStateSave.k", "k", "S", csnd::thread::k);

    csnd::plugin<ChannelStateRecall>((csnd::Csound*) getCsound()->GetCsound(), "cabbageChannelStateRecall.i", "i", "S", csnd::thread::i);
    csnd::plugin<ChannelStateRecall>((csnd::Csound*) getCsound()->GetCsound(), "cabbageChannelStateRecall.k", "k", "SO", csnd::thread::k);
    csnd::plugin<ChannelStateRecall>((csnd::Csound*) getCsound()->GetCsound(), "cabbageChannelStateRecall.k", "k", "SS[]", csnd::thread::k);

    
    csnd::plugin<StrToArray>((csnd::Csound*) getCsound()->GetCsound(), "strToArray.ii", "S[]", "SS", csnd::thread::i);
    csnd::plugin<StrRemove>((csnd::Csound*) getCsound()->GetCsound(), "strRemove.ii", "S", "SSo", csnd::thread::i);

    csnd::plugin<WriteStateData>((csnd::Csound*) getCsound()->GetCsound(), "cabbageWriteStateData.ss", "", "iS", csnd::thread::i);
    csnd::plugin<ReadStateData>((csnd::Csound*) getCsound()->GetCsound(), "cabbageReadStateData.i", "S", "", csnd::thread::ik);

//    csnd::plugin<StateDataIsValid>((csnd::Csound*)getCsound()->GetCsound(), "cabbageHasStateData.k", "i", "", csnd::thread::i);
    csnd::plugin<StateDataIsValid>((csnd::Csound*)getCsound()->GetCsound(), "cabbageHasStateData.k", "k", "", csnd::thread::k);
    

//    csnd::plugin<GetStateFloatValue>((csnd::Csound*) getCsound()->GetCsound(), "cabbageGetStateValue.s", "i", "S", csnd::thread::i);
    csnd::plugin<GetStateFloatValue>((csnd::Csound*) getCsound()->GetCsound(), "cabbageGetStateValue.s", "k", "S", csnd::thread::k);
    csnd::plugin<GetStateFloatValueArray>((csnd::Csound*) getCsound()->GetCsound(), "cabbageGetStateValue.s", "k[]", "S", csnd::thread::k);
//    csnd::plugin<GetStateFloatValueArray>((csnd::Csound*) getCsound()->GetCsound(), "cabbageGetStateValue.s", "i[]", "S", csnd::thread::i);
    csnd::plugin<GetStateStringValue>((csnd::Csound*) getCsound()->GetCsound(), "cabbageGetStateValue.s", "S", "S", csnd::thread::ik);
    csnd::plugin<GetStateStringValueArray>((csnd::Csound*) getCsound()->GetCsound(), "cabbageGetStateValue.s", "S[]", "S", csnd::thread::ik);

    csnd::plugin<SetStateFloatData>((csnd::Csound*) getCsound()->GetCsound(), "cabbageSetStateValue.s", "", "Sk", csnd::thread::k);
//    csnd::plugin<SetStateFloatData>((csnd::Csound*) getCsound()->GetCsound(), "cabbageSetStateValue.s", "", "Si", csnd::thread::i);

//    csnd::plugin<SetStateFloatArrayData>((csnd::Csound*) getCsound()->GetCsound(), "cabbageSetStateValue.s", "", "Si[]", csnd::thread::i);
    csnd::plugin<SetStateFloatArrayData>((csnd::Csound*) getCsound()->GetCsound(), "cabbageSetStateValue.s", "", "Sk[]", csnd::thread::k);

//    csnd::plugin<SetStateStringData>((csnd::Csound*) getCsound()->GetCsound(), "cabbageSetStateValue.s", "", "SS", csnd::thread::i);
    csnd::plugin<SetStateStringData>((csnd::Csound*) getCsound()->GetCsound(), "cabbageSetStateValue.s", "", "SS", csnd::thread::k);

//    csnd::plugin<SetStateStringArrayData>((csnd::Csound*) getCsound()->GetCsound(), "cabbageSetStateValue.s", "", "SS[]", csnd::thread::i);
    csnd::plugin<SetStateStringArrayData>((csnd::Csound*) getCsound()->GetCsound(), "cabbageSetStateValue.s", "", "SS[]", csnd::thread::k);

   
    csnd::plugin<SetCabbageIdentifierITimeSArgs>((csnd::Csound*) getCsound()->GetCsound(), "cabbageSet", "", "SW", csnd::thread::i);
    csnd::plugin<SetCabbageIdentifierITime>((csnd::Csound*) getCsound()->GetCsound(), "cabbageSet", "", "SSN", csnd::thread::i);

    
    //csnd::plugin<SetCabbageIdentifierSArgs>((csnd::Csound*) csound->GetCsound(), "cabbageSet", "", "kSSW", csnd::thread::ik);
    csnd::plugin<SetCabbageIdentifierSArgs>((csnd::Csound*) getCsound()->GetCsound(), "cabbageSet", "", "kSS", csnd::thread::ik);
    csnd::plugin<SetCabbageIdentifier>((csnd::Csound*) getCsound()->GetCsound(), "cabbageSet", "", "kSSM", csnd::thread::ik);
    csnd::plugin<SetCabbageIdentifierArray>((csnd::Csound*) getCsound()->GetCsound(), "cabbageSet", "", "kSSk[]", csnd::thread::ik);
    csnd::plugin<SetCabbageIdentifierSArgs>((csnd::Csound*) getCsound()->GetCsound(), "cabbageSet", "", "kSW", csnd::thread::ik);
    
    csnd::plugin<SetCabbageValueIdentifierITime>((csnd::Csound*) getCsound()->GetCsound(), "cabbageSetValue", "", "Si", csnd::thread::i);
    csnd::plugin<SetCabbageValueIdentifier>((csnd::Csound*) getCsound()->GetCsound(), "cabbageSetValue", "", "SkP", csnd::thread::k);
    
    csnd::plugin<SetCabbageValueIdentifierSArgsITime>((csnd::Csound*) getCsound()->GetCsound(), "cabbageSetValue", "", "SS", csnd::thread::i);
    csnd::plugin<SetCabbageValueIdentifierSArgs>((csnd::Csound*) getCsound()->GetCsound(), "cabbageSetValue", "", "SSk", csnd::thread::k);
    
    csnd::plugin<GetCabbageValue>((csnd::Csound*) getCsound()->GetCsound(), "cabbageGetValue", "k", "S", csnd::thread::ik);
    csnd::plugin<GetCabbageValueArray>((csnd::Csound*) getCsound()->GetCsound(), "cabbageGetValue", "k[]", "S[]", csnd::thread::ik);
    csnd::plugin<GetCabbageValue>((csnd::Csound*) getCsound()->GetCsound(), "cabbageGetValue", "i", "S", csnd::thread::i);
    csnd::plugin<GetCabbageValueWithTrigger>((csnd::Csound*) getCsound()->GetCsound(), "cabbageGetValue", "kk", "So", csnd::thread::ik);
    csnd::plugin<GetCabbageValueArrayWithTrigger>((csnd::Csound*) getCsound()->GetCsound(), "cabbageGetValue", "k[]k[]", "S[]", csnd::thread::ik);
    
    csnd::plugin<GetCabbageStringValue>((csnd::Csound*) getCsound()->GetCsound(), "cabbageGetValue", "S", "S", csnd::thread::ik);
    csnd::plugin<GetCabbageStringValueArray>((csnd::Csound*) getCsound()->GetCsound(), "cabbageGetValue", "S[]", "S[]", csnd::thread::ik);
    csnd::plugin<GetCabbageStringValueWithTrigger>((csnd::Csound*) getCsound()->GetCsound(), "cabbageGetValue", "Sk", "Sj", csnd::thread::ik);
    csnd::plugin<GetCabbageStringValueArrayWithTrigger>((csnd::Csound*) getCsound()->GetCsound(), "cabbageGetValue", "S[]k[]", "S[]", csnd::thread::ik);
    csnd::plugin<GetCabbageIdentifierArray>((csnd::Csound*) getCsound()->GetCsound(), "cabbageGet", "k[]", "SS", csnd::thread::k);
    csnd::plugin<GetCabbageIdentifierArray>((csnd::Csound*) getCsound()->GetCsound(), "cabbageGet", "i[]", "SS", csnd::thread::i);

    csnd::plugin<CabbageValueChanged>((csnd::Csound*) getCsound()->GetCsound(), "cabbageChanged", "Sk", "S[]", csnd::thread::ik);
    csnd::plugin<CabbageValueChangedIndex>((csnd::Csound*) getCsound()->GetCsound(), "cabbageChanged", "kk", "S[]", csnd::thread::ik);
    
    csnd::plugin<CabbageValueChangedIndex>((csnd::Csound*) getCsound()->GetCsound(), "cabbageChanged", "kk", "S[]kM", csnd::thread::ik);
    csnd::plugin<CabbageValueChanged>((csnd::Csound*) getCsound()->GetCsound(), "cabbageChanged", "Sk", "S[]kM", csnd::thread::ik);
    
    csnd::plugin<GetCabbageStringIdentifierArray>((csnd::Csound*) getCsound()->GetCsound(), "cabbageGet", "S[]", "SS", csnd::thread::ik);
    csnd::plugin<GetCabbageIdentifierSingle>((csnd::Csound*) getCsound()->GetCsound(), "cabbageGet", "k", "SS", csnd::thread::ik);
    csnd::plugin<GetCabbageIdentifierSingleWithTrigger>((csnd::Csound*) getCsound()->GetCsound(), "cabbageGet", "kk", "SS", csnd::thread::ik);
    csnd::plugin<GetCabbageIdentifierSingleITime>((csnd::Csound*) getCsound()->GetCsound(), "cabbageGet", "i", "SS", csnd::thread::i);
    csnd::plugin<GetCabbageStringIdentifierSingle>((csnd::Csound*) getCsound()->GetCsound(), "cabbageGet", "S", "SS", csnd::thread::ik);

    csnd::plugin<GetCabbageReservedChannelStringWithTrigger>((csnd::Csound*) getCsound()->GetCsound(), "cabbageGet", "Sk", "S", csnd::thread::ik);
    csnd::plugin<GetCabbageReservedChannelString>((csnd::Csound*) getCsound()->GetCsound(), "cabbageGet", "S", "S", csnd::thread::ik);
    
    csnd::plugin<GetCabbageReservedChannelDataWithTrigger>((csnd::Csound*) getCsound()->GetCsound(), "cabbageGet", "kk", "S", csnd::thread::ik);
    csnd::plugin<GetCabbageReservedChannelData>((csnd::Csound*) getCsound()->GetCsound(), "cabbageGet", "k", "S", csnd::thread::ik);

    csnd::plugin<CreateCabbageWidget>((csnd::Csound*) getCsound()->GetCsound(), "cabbageCreate", "", "SS", csnd::thread::i);

    csnd::plugin<CabbageCopyFile>((csnd::Csound*) getCsound()->GetCsound(), "cabbageCopyFile", "", "SW", csnd::thread::i);
    csnd::plugin<CabbageFindFilesI>((csnd::Csound*) getCsound()->GetCsound(), "cabbageFindFiles", "S[]", "SW", csnd::thread::i);
    csnd::plugin<CabbageFindFilesK>((csnd::Csound*) getCsound()->GetCsound(), "cabbageFindFiles", "S[]", "kSW", csnd::thread::ik);
    csnd::plugin<CabbageGetFilename>((csnd::Csound*) getCsound()->GetCsound(), "cabbageGetFilename", "S", "S", csnd::thread::ik);
    csnd::plugin<CabbageGetFilePath>((csnd::Csound*) getCsound()->GetCsound(), "cabbageGetFilePath", "S", "S", csnd::thread::ik);
    csnd::plugin<CabbageGetFileExtension>((csnd::Csound*) getCsound()->GetCsound(), "cabbageGetFileExtension", "S", "S", csnd::thread::ik);
    csnd::plugin<CabbageGetFileNoExtension>((csnd::Csound*) getCsound()->GetCsound(), "cabbageGetFileNoExtension", "S", "S", csnd::thread::ik);

    csnd::plugin<CabbageGetWidgetChannels>((csnd::Csound*)getCsound()->GetCsound(), "cabbageGetWidgetChannels", "S[]", "W", csnd::thread::i);

    csnd::plugin<CabbageMidiReader>((csnd::Csound*) getCsound()->GetCsound(), "cabbageMidiFileReader", "k[]k[]k[]k[]kk", "Sikkkko", csnd::thread::ik);
    csnd::plugin<CabbageMidiFileInfo>((csnd::Csound*) getCsound()->GetCsound(), "cabbageMidiFileInfo", "", "S", csnd::thread::i);
    csnd::plugin<CabbageMidiListener>((csnd::Csound*)getCsound()->GetCsound(), "cabbageMidiListener", "k[]k[]k[]k", "O", csnd::thread::ik);
    csnd::plugin<CabbageMidiSender>((csnd::Csound*)getCsound()->GetCsound(), "cabbageMidiSender", "", "", csnd::thread::i);
    
    csnd::plugin<CabbageProfilerStart>((csnd::Csound*)getCsound()->GetCsound(), "cabbageProfilerStart", "", "S", csnd::thread::k);
    csnd::plugin<CabbageProfilerStop>((csnd::Csound*)getCsound()->GetCsound(), "cabbageProfilerStop", "k", "S", csnd::thread::k);
#if Bluetooth
    csnd::plugin<CabbageBTOpcode>((csnd::Csound*)getCsound()->GetCsound(), "cabbageBluetooth", "k", "SS", csnd::thread::ik);
#endif
   // csnd::plugin<CabbageFileLoader>((csnd::Csound*)getCsound()->GetCsound(), "cabbageFileLoader", "", "S", csnd::thread::i);
   // csnd::plugin<CabbageFileLoader>((csnd::Csound*)getCsound()->GetCsound(), "cabbageFileLoader", "", "S[]", csnd::thread::i);
//    csnd::plugin<CabbageFileReader>((csnd::Csound*)getCsound()->GetCsound(), "cabbageOggReader", "aa", "Skii", csnd::thread::ia);

	csound->CreateMessageBuffer(0);
	csound->SetExternalMidiInOpenCallback(OpenMidiInputDevice);
	csound->SetExternalMidiReadCallback(ReadMidiData);
	csound->SetExternalMidiOutOpenCallback(OpenMidiOutputDevice);
	csound->SetExternalMidiWriteCallback(WriteMidiData);
	csoundParams = nullptr;
	csoundParams = std::make_unique<CSOUND_PARAMS> ();

	csoundParams->displays = 0;

	csound->SetIsGraphable(true);
	csound->SetMakeGraphCallback(makeGraphCallback);
	csound->SetDrawGraphCallback(drawGraphCallback);
	csound->SetKillGraphCallback(killGraphCallback);
	csound->SetExitGraphCallback(exitGraphCallback);
	csound->SetOption((char*)"-n");
	csound->SetOption((char*)"-d");
	csound->SetOption((char*)"-b0");
    
    addMacros(csdFileText);

	if (debugMode)
	{
		csoundDebuggerInit(csound->GetCsound());
		csoundSetBreakpointCallback(csound->GetCsound(), breakpointCallback, (void*)this);
		csoundSetInstrumentBreakpoint(csound->GetCsound(), 1, 413);
		csoundParams->ksmps_override = 4410;
	}

    if(hostRequestedMono)
    {
        //this mode is for logic and cubase as they both allow weird mono/stereo configs
        numCsoundOutputChannels = 1;
        numCsoundInputChannels = 1;
        csoundParams->nchnls_override = 1;
        csoundParams->nchnls_i_override = 1;
    }
    else
    {
        csoundParams->nchnls_override = numCsoundOutputChannels;
        csoundParams->nchnls_i_override = numCsoundInputChannels;
    }
//    csoundParams->nchnls_override = numCsoundOutputChannels;
//    csoundParams->nchnls_i_override = numCsoundInputChannels;

    
    // Update the matchingNumberOfIOChannels flag so the MacOS auval tool doesn't crash when validating
    // different I/O channel configurations.
    if (csoundParams->nchnls_i_override != csoundParams->nchnls_override)
    {
        matchingNumberOfIOChannels = false;
    }
	
	const int requestedKsmpsRate = CabbageUtilities::getHeaderInfo(csdFileText, "ksmps");
	const int requestedSampleRate = CabbageUtilities::getHeaderInfo(csdFileText, "sr");

	
	if (requestedKsmpsRate == -1)
		csoundParams->ksmps_override = 32;

	csoundParams->sample_rate_override = requestedSampleRate>0 ? requestedSampleRate : sr;

    if(preferredLatency == -1)
        csoundParams->ksmps_override = 1;

	csound->SetParams(csoundParams.get());
    
//#ifdef CabbagePro
//    compileCsdString(csdFileText);
//    //DBG(csdFileText);
//    csound->Start();
//#else
    if (csdFileText.contains("<Csound") || csdFileText.contains("</Csound"))
    {
        compileCsdFile(csdFile);
    }



	if (csdCompiledWithoutError())
	{
		csdKsmps = csound->GetKsmps();
		CSspout = csound->GetSpout();
		CSspin = csound->GetSpin();
		cs_scale = csound->Get0dBFS();
		csndIndex = csound->GetKsmps();
        const String version = String("Cabbage version:")+ProjectInfo::versionString+String("\n");
        csound->Message(version.toRawUTF8());
        
#if CabbagePro
        const String encryptedOrcCode = Encrypt::decode(csdFile, "orc");
        const String encryptedScoCode = Encrypt::decode(csdFile, "sco");
        if(encryptedOrcCode.isNotEmpty())
            csound->CompileOrc(encryptedOrcCode.toUTF8().getAddress());
        if(encryptedScoCode.isNotEmpty())
            csound->ReadScore(encryptedScoCode.toUTF8().getAddress());
        
        //compileCsdString(encryptedCsdCode);
#endif
    }
	else
		CabbageUtilities::debug("Csound could not compile your file?");

    return csdCompiledWithoutError();

}


void CsoundPluginProcessor::createFileLogger (File csoundFile)
{
    String logFileName = csoundFile.getParentDirectory().getFullPathName() + String ("/") + csoundFile.getFileNameWithoutExtension() + String ("_Log.txt");
    fileLogger.reset (new FileLogger (File (logFileName), String ("Cabbage Log..")));
    Logger::setCurrentLogger (fileLogger.get());
}
//==============================================================================
void CsoundPluginProcessor::resetFilebuttons(ValueTree cabbageData)
{

    if (!csdCompiledWithoutError())
    {
        Logger::writeToLog("csound not compiled");
        return;
    }


    for (int i = 0; i < cabbageData.getNumChildren(); i++)
    {
        const String typeOfWidget = CabbageWidgetData::getStringProp(cabbageData.getChild(i), CabbageIdentifierIds::type);
        const String identChannel = CabbageWidgetData::getStringProp(cabbageData.getChild(i), CabbageIdentifierIds::identchannel);


        if (CabbageWidgetData::getStringProp(cabbageData.getChild(i), CabbageIdentifierIds::channeltype) == "string")
        {
            if (typeOfWidget == CabbageWidgetTypes::filebutton)
            {
                const String mode = CabbageWidgetData::getStringProp(cabbageData.getChild(i), CabbageIdentifierIds::mode);
                if (mode == "file" || mode == "save" || mode == "directory")
                {
                    //if a SR change is made on startup, the channel will already have been set when Csound is recompiled, hence no 
                    //trigger updates will take place with changed2 or cabbageGetValue opcodes. Commenting this out for now to resolve this.. 
                    csound->SetStringChannel(CabbageWidgetData::getStringProp(cabbageData.getChild(i), CabbageIdentifierIds::channel).getCharPointer(),
                        "");
                    //csound->PerformKsmps();
                }
            }
        }
    }
}

//==============================================================================
void CsoundPluginProcessor::initAllCsoundChannels (ValueTree cabbageData)
{
    Logger::writeToLog("initAllCsoundChannels (ValueTree cabbageData) ...");
    if (!csound)
    {
        Logger::writeToLog("csound not initialized");
        return;
    }
    else
    {
        Logger::writeToLog(String::formatted("csound = 0x%p", csound.get()));
        Logger::writeToLog(String::formatted("handle = 0x%p", csound->GetCsound()));
    }
    
    if (!csdCompiledWithoutError())
    {
        Logger::writeToLog("csound not compiled");
        return;
    }
    

    for (int i = 0; i < cabbageData.getNumChildren(); i++)
    {
        const String typeOfWidget = CabbageWidgetData::getStringProp (cabbageData.getChild (i), CabbageIdentifierIds::type);
        const String identChannel = CabbageWidgetData::getStringProp (cabbageData.getChild (i), CabbageIdentifierIds::identchannel);
        
        if(typeOfWidget == CabbageWidgetTypes::form)
        {
            const int latency = int(CabbageWidgetData::getNumProp (cabbageData.getChild (i), CabbageIdentifierIds::latency));
            preferredLatency = latency;
            //DBG(CabbageWidgetData::getNumProp(cabbageData.getChild(i), CabbageIdentifierIds::width)
            csound->SetChannel("SCREEN_WIDTH", CabbageWidgetData::getNumProp(cabbageData.getChild(i), CabbageIdentifierIds::width));
            csound->SetChannel("SCREEN_HEIGHT", CabbageWidgetData::getNumProp(cabbageData.getChild(i), CabbageIdentifierIds::height));
        }

        if (CabbageWidgetData::getStringProp (cabbageData.getChild (i), CabbageIdentifierIds::channeltype) == "string")
        {
            if (typeOfWidget == CabbageWidgetTypes::filebutton)
            {
                const String mode = CabbageWidgetData::getStringProp (cabbageData.getChild (i), CabbageIdentifierIds::mode);
                if( mode == "file" || mode == "save" || mode == "directory")
                {
                    //if a SR change is made on startup, the channel will already have been set when Csound is recompiled, hence no 
                    //trigger updates will take place with changed2 or cabbageGetValue opcodes. Commenting this out for now to resolve this.. 
                    csound->SetStringChannel (CabbageWidgetData::getStringProp (cabbageData.getChild (i), CabbageIdentifierIds::channel).getCharPointer(),
                                          CabbageWidgetData::getStringProp (cabbageData.getChild (i), CabbageIdentifierIds::file).toUTF8().getAddress());
                }
            }

            else
            {
                if (typeOfWidget == CabbageWidgetTypes::combobox || typeOfWidget == CabbageWidgetTypes::listbox)
                {
                    const String fileType = CabbageWidgetData::getStringProp(cabbageData.getChild(i), CabbageIdentifierIds::filetype);
                    
                    //if we are dealing with a combobox that reads files from a directory, we need to load them before the GUI opens...
                    if (!CabbageWidgetData::getStringProp(cabbageData.getChild(i), CabbageIdentifierIds::filetype).contains("preset")
                        && !CabbageWidgetData::getStringProp(cabbageData.getChild(i), CabbageIdentifierIds::filetype).contains("*.snaps")
                        && !CabbageWidgetData::getStringProp(cabbageData.getChild(i), CabbageIdentifierIds::filetype).contains(".snaps")
                        && !CabbageWidgetData::getStringProp(cabbageData.getChild(i), CabbageIdentifierIds::filetype).contains("snaps"))
                    {
                        const String relativeDir = CabbageWidgetData::getStringProp(cabbageData.getChild(i), CabbageIdentifierIds::currentdir);
                        const String workingDir = csdFilePath.getChildFile(relativeDir).getFullPathName();

                        if(relativeDir.isNotEmpty() && workingDir.isNotEmpty())
                        {
                            int numOfFiles;
                            Array<File> folderFiles;
                            StringArray comboItems;
                            CabbageUtilities::searchDirectoryForFiles(workingDir, fileType, folderFiles, comboItems, numOfFiles);
                            const String currentValue = CabbageWidgetData::getStringProp(cabbageData.getChild(i), CabbageIdentifierIds::value);

                            const int index = comboItems.indexOf(currentValue);
                            const String test = folderFiles[index].getFullPathName();
                            const String channel = CabbageWidgetData::getStringProp(cabbageData.getChild(i), CabbageIdentifierIds::channel);
                            csound->SetStringChannel(CabbageWidgetData::getStringProp(cabbageData.getChild(i), CabbageIdentifierIds::channel).getCharPointer(),
                                folderFiles[index-1].getFileNameWithoutExtension().toUTF8().getAddress());
                        }
                        else
                        {
                            String currentValue = CabbageWidgetData::getStringProp(cabbageData.getChild(i), CabbageIdentifierIds::value);
                            var items = CabbageWidgetData::getProperty(cabbageData.getChild(i), CabbageIdentifierIds::text);
                            //DBG(items.size());
                            const int index = items.indexOf(currentValue);
                            if(index == -1 && items.isArray())
                            {
                                currentValue = items[0].toString();
                            
                                const String channel = CabbageWidgetData::getStringProp(cabbageData.getChild(i), CabbageIdentifierIds::channel);
                            
                                csound->SetStringChannel(CabbageWidgetData::getStringProp(cabbageData.getChild(i), CabbageIdentifierIds::channel).getCharPointer(), currentValue.toUTF8().getAddress());
                            }
                        }
                        
                    }
                    else{
                        csound->SetStringChannel(CabbageWidgetData::getStringProp(cabbageData.getChild(i), CabbageIdentifierIds::channel).getCharPointer(),
                                                 CabbageWidgetData::getProperty(cabbageData.getChild(i), CabbageIdentifierIds::value).toString().toUTF8().getAddress());
                    }
                }
                else if (typeOfWidget == CabbageWidgetTypes::texteditor)
                {
                    csound->SetStringChannel(CabbageWidgetData::getStringProp(cabbageData.getChild(i), CabbageIdentifierIds::channel).getCharPointer(),
                                             CabbageWidgetData::getStringProp(cabbageData.getChild(i), CabbageIdentifierIds::text).toUTF8().getAddress());
                }
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
                    csound->SetChannel (channels[1].toString().getCharPointer(), float (maxValue));
                }

            }
            else if (CabbageWidgetData::getStringProp (cabbageData.getChild (i), CabbageIdentifierIds::type) == CabbageWidgetTypes::cvoutput
                     ||CabbageWidgetData::getStringProp (cabbageData.getChild (i), CabbageIdentifierIds::type) == CabbageWidgetTypes::cvinput)
            {
                //don't set up any channels for these widgets, even though they use the channel() identifier..
            }
            else
            {
                const float value = CabbageWidgetData::getProperty (cabbageData.getChild (i), CabbageIdentifierIds::value);
                const String channel = CabbageWidgetData::getStringProp (cabbageData.getChild (i), CabbageIdentifierIds::channel);
                csound->SetChannel (channel.getCharPointer(), value);
                
                if(firstInit)
                {
                    if(typeOfWidget == CabbageWidgetTypes::rslider || typeOfWidget == CabbageWidgetTypes::vslider || typeOfWidget == CabbageWidgetTypes::hslider)
                    {
                        //set doubleclick return value..
                        CabbageWidgetData::setNumProp(cabbageData.getChild(i), CabbageIdentifierIds::defaultValue, value);
                        
                    }
                }
            }

        }

    }

   createCsoundGlobalVars(cabbageData);
    
    
    if (CabbageUtilities::getTargetPlatform() == CabbageUtilities::TargetPlatformTypes::Win)
    {
        csound->SetChannel ("CSD_PATH", csdFilePath.getFullPathName().replace ("\\", "\\\\").toUTF8().getAddress());
		csound->SetStringChannel("USER_HOME_DIRECTORY", CabbageUtilities::getRealUserHomeDirectory().getFullPathName().replace("\\", "\\\\").toUTF8().getAddress());
		csound->SetStringChannel("USER_DESKTOP_DIRECTORY", File::getSpecialLocation(File::userDesktopDirectory).getFullPathName().replace("\\", "\\\\").toUTF8().getAddress());
		csound->SetStringChannel("USER_MUSIC_DIRECTORY", File::getSpecialLocation(File::userMusicDirectory).getFullPathName().replace("\\", "\\\\").toUTF8().getAddress());
		csound->SetStringChannel("USER_APPLICATION_DIRECTORY", File::getSpecialLocation(File::userApplicationDataDirectory).getFullPathName().replace("\\", "\\\\").toUTF8().getAddress());
		csound->SetStringChannel("USER_DOCUMENTS_DIRECTORY", File::getSpecialLocation(File::userDocumentsDirectory).getFullPathName().replace("\\", "\\\\").toUTF8().getAddress());
    }
    else
    {
        csound->SetChannel ("CSD_PATH", csdFilePath.getFullPathName().toUTF8().getAddress());
		csound->SetStringChannel("USER_HOME_DIRECTORY", CabbageUtilities::getRealUserHomeDirectory().getFullPathName().toUTF8().getAddress());
		csound->SetStringChannel("USER_DESKTOP_DIRECTORY", File::getSpecialLocation(File::userDesktopDirectory).getFullPathName().toUTF8().getAddress());
		csound->SetStringChannel("USER_MUSIC_DIRECTORY", File::getSpecialLocation(File::userMusicDirectory).getFullPathName().toUTF8().getAddress());
		csound->SetStringChannel("USER_APPLICATION_DATA_DIRECTORY", File::getSpecialLocation(File::userApplicationDataDirectory).getFullPathName().toUTF8().getAddress());
		csound->SetStringChannel("USER_DOCUMENTS_DIRECTORY", File::getSpecialLocation(File::userDocumentsDirectory).getFullPathName().toUTF8().getAddress());
    }

    csound->SetStringChannel ("LAST_FILE_DROPPED", const_cast<char*> (""));
    
    csound->SetChannel("PRESET_STATE", -1.0);

    csound->SetChannel("IS_BYPASSED", 0.0);
    //csdFilePath.setAsCurrentWorkingDirectory();
    csound->SetChannel("HOST_BUFFER_SIZE", csdKsmps);
    csound->SetChannel("HOME_FOLDER_UID", File::getSpecialLocation (File::userHomeDirectory).getFileIdentifier());

    time_t seconds_past_epoch = time(nullptr);
    csound->SetChannel("SECONDS_SINCE_EPOCH", (intmax_t)seconds_past_epoch);
    // convert now to string form
    char* dt = ctime(&seconds_past_epoch);
    std::stringstream strStream;
    strStream << dt << std::endl;
    csound->SetStringChannel("CURRENT_DATE_TIME", String(strStream.str()).toUTF8().getAddress());

	if(CabbageUtilities::getTargetPlatform() == CabbageUtilities::TargetPlatformTypes::Linux)
    {
		csound->SetChannel ("LINUX", 1.0);
        csound->SetChannel ("Linux", 1.0);
    }
	if(CabbageUtilities::getTargetPlatform() == CabbageUtilities::TargetPlatformTypes::OSX)
    {
		csound->SetChannel ("MAC", 1.0);
        csound->SetChannel ("Macos", 1.0);
        csound->SetChannel ("MACOS", 1.0);
    }
	if(CabbageUtilities::getTargetPlatform() == CabbageUtilities::TargetPlatformTypes::Win)
    {
		csound->SetChannel ("Windows", 1.0);
        csound->SetChannel ("WINDOWS", 1.0);
    }

#if Cabbage_IDE_Build == 0
    PluginHostType pluginType;
    if (pluginType.isFruityLoops())
        csound->SetChannel ("FLStudio", 1.0);
    else if (pluginType.isAbletonLive())
        csound->SetChannel ("AbletonLive", 1.0);
    else if (pluginType.isLogic())
    {
        csound->SetChannel ("Logic", 1.0);
        isLogic = true;
    }
    else if (pluginType.getHostPath().contains("LMMS"))
    {
        csound->SetChannel("LMMS", 1.0);
        isLMMS = true;
    }
    else if (pluginType.isArdour())
        csound->SetChannel ("Ardour", 1.0);
    else if (pluginType.isCubase())
        csound->SetChannel ("Cubase", 1.0);
    else if (pluginType.isSonar())
        csound->SetChannel ("Sonar", 1.0);
    else if (pluginType.isNuendo())
        csound->SetChannel ("Nuendo", 1.0);
    else if (pluginType.isReaper())
        csound->SetChannel ("Reaper", 1.0);
    else if (pluginType.isRenoise())
        csound->SetChannel ("Renoise", 1.0);
    else if (pluginType.isWavelab())
        csound->SetChannel ("Wavelab", 1.0);
    else if (pluginType.isMainStage())
        csound->SetChannel ("Mainstage", 1.0);
    else if (pluginType.isGarageBand())
        csound->SetChannel ("Garageband", 1.0);
    else if (pluginType.isSamplitude())
        csound->SetChannel ("Samplitude", 1.0);
    else if (pluginType.isStudioOne())
        csound->SetChannel ("StudioOne", 1.0);
    else if (pluginType.isBitwigStudio())
        csound->SetChannel ("Bitwig", 1.0);
    else if (pluginType.isTracktion())
        csound->SetChannel ("Tracktion", 1.0);
    else if (pluginType.isAdobeAudition())
        csound->SetChannel ("AdobeAudition", 1.0);
#endif
             
    if (CabbageUtilities::getTarget() != CabbageUtilities::TargetTypes::IDE) {
        csound->SetChannel("IS_A_PLUGIN", 1.0);
    }

    if (getPlayHead() != nullptr && getPlayHead()->getCurrentPosition (hostInfo))
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

    //csound->Message("Running single k-cycle...\n");
    
    csound->PerformKsmps();
//
//    //run through a set of preCycles...
//    for( int i = 0 ; i < preCycles ; i++ )
//        csound->PerformKsmps();
    
    //csound->Message("Rewinding...\n");
    //csound->SetChannel ("IS_EDITOR_OPEN", 0.0);
    csound->SetChannel ("MOUSE_DOWN_LEFT", 0.0);
    csound->SetChannel ("MOUSE_DOWN_RIGHT", 0.0);
    csound->SetChannel ("MOUSE_DOWN_MIDDLE", 0.0);
    
    Logger::writeToLog("initAllCsoundChannels (ValueTree cabbageData) - done");
    firstInit = false;
}
//==============================================================================
void CsoundPluginProcessor::addMacros (String& csdText)
{
    StringArray csdArray;
    String macroName, macroText;

    csdArray.addLines (csdText);
//
//    String width = "--macro:SCREEN_WIDTH="+String(screenWidth);
//    String height = "--macro:SCREEN_HEIGHT="+String(screenHeight);
//    csound->SetOption (width.toUTF8().getAddress());
//    csound->SetOption (height.toUTF8().getAddress());
    
    auto inCabbageSection = false;

    for (int i = 0; i < csdArray.size(); i++)
    {
        if (!inCabbageSection && csdArray[i].contains ("<Cabbage"))
            inCabbageSection = true;

        if (inCabbageSection)
        {
            if (csdArray[i].trim().substring (0, 7) == "#define")
            {
			    StringArray tokens;
                //CabbageUtilities::debug(csdArray[i]);
                tokens.addTokens (csdArray[i].replace("#", "").trim() , " ");
                tokens.removeEmptyStrings();
                macroName = tokens[1].trim();
                tokens.remove (0);
                macroText = "\"" + tokens.joinIntoString (" ").replace (" ", "\ ").replace("\"", "\\\"")+"\"";
                //macroText = tokens.joinIntoString(" ");
                String fullMacro = "--omacro:" + macroName + "=" + macroText;// + "\"";
                csound->SetOption (fullMacro.toUTF8().getAddress());
            }
        }

        if (csdArray[i].contains ("</Cabbage>"))
            i = csdArray.size();
    }

}

//==============================================================================
void CsoundPluginProcessor::createMatrixEventSequencer(int rows, int cols, String channel)
{
    auto* matrix = new MatrixEventSequencer(channel);

    for (int i = 0 ; i < cols ; i++)
    {
        matrix->events.add (new StringArray());

        for ( int y = 0 ; y < rows ; y++)
        {
            matrix->events[i]->add(" ");
        }
    }

    matrixEventSequencers.add(matrix);
	numMatrixEventSequencers = matrixEventSequencers.size();


}

void CsoundPluginProcessor::setMatrixEventSequencerCellData(int col, int row, const String& channel, String data)
{
	if (numMatrixEventSequencers > 0)
	{
		for (auto matrixEventSequencer : matrixEventSequencers)
		{
			if (matrixEventSequencer->channel == channel)
			{
				matrixEventSequencer->setEventString(col, row, data);
			}
		}
	}
}

//==============================================================================
StringArray CsoundPluginProcessor::getTableStatement (int tableNum)
{
    StringArray fdata;
    fdata.add (String());

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

        const int tableSize = csound->TableLength (tableNum);

        if (tableSize < 0)
            return points;

        std::vector<MYFLT> temp (tableSize);

        csound->TableCopyOut (tableNum, &temp[0]);

        if (tableSize > 0)
            points = Array<float, CriticalSection> (&temp[0], tableSize);
    }

    return points;
}

//==============================================================================
String CsoundPluginProcessor::getCsoundOutput()
{
    if (csound!=nullptr)
    {
        const int messageCnt = csound->GetMessageCnt();
        csoundOutput = "";

        if (messageCnt == 0)
            return csoundOutput;

        while (csound->GetMessageCnt() > 0)
        {
            String message = CharPointer_UTF8(csound->GetFirstMessage());
            if(!message.contains("midi channel") && !message.contains("is muted") && !message.contains("Score finished in csoundPerformKsmps()"))
                csoundOutput += message;
            csound->PopFirstMessage();
        }

        Logger::writeToLog (csoundOutput);

        if (disableLogging)
            this->suspendProcessing (true);

        return csoundOutput;
    }

    return String();
}

//==============================================================================
const String CsoundPluginProcessor::getName() const
{
    String pluginString =  File::getSpecialLocation(File::currentExecutableFile).getFileNameWithoutExtension();
    //return JucePlugin_Name;
    return pluginString;
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
    ignoreUnused(index);
}

const String CsoundPluginProcessor::getProgramName (int index)
{
    ignoreUnused(index);
    return String();
}

void CsoundPluginProcessor::changeProgramName (int index, const String& newName)
{
    ignoreUnused(index, newName);
}

//==============================================================================
void CsoundPluginProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    if(getCsound()!= nullptr)
        csound->SetChannel("HOST_BUFFER_SIZE", samplesPerBlock);
#if Cabbage_IDE_Build == 0
    PluginHostType pluginType;
    
    if (pluginType.isCubase())
        hostIsCubase = true;
#endif

    const int inputs = getTotalNumInputChannels();
    const int outputs = getTotalNumOutputChannels();

#if ! JucePlugin_IsSynth
    if(getBusesLayout().getMainOutputChannelSet() == AudioChannelSet::mono())
        hostRequestedMono = true;
    else
        hostRequestedMono = false;
    
    CabbageUtilities::debug("CsoundPluginProcessor::prepareToPlay - inputBuses:", getBusCount(true));
    CabbageUtilities::debug("CsoundPluginProcessor::prepareToPlay - inputs:", inputs);
    CabbageUtilities::debug("CsoundPluginProcessor::prepareToPlay - Requested input channels:", numCsoundInputChannels);
#endif
    //const int outputs = getBus(false, 0)->getNumberOfChannels();
    CabbageUtilities::debug("CsoundPluginProcessor::prepareToPlay - outputBuses:", getBusCount(false));
    CabbageUtilities::debug("CsoundPluginProcessor::prepareToPlay - outputs:", outputs);
    CabbageUtilities::debug("CsoundPluginProcessor::prepareToPlay - Requested output channels:", numCsoundOutputChannels);

    CabbageUtilities::debug("CsoundPluginProcessor::prepareToPlay - Sampling rate:", samplingRate);

    //weird thing in FL Studio where outputs is 0 at some point, causing Csound to recompile, causing issues with channels
    if (outputs != 0)
    {
        if (((samplingRate != sampleRate))
#if ! JucePlugin_IsSynth
            || numCsoundInputChannels != inputs
#endif
            || numCsoundOutputChannels != outputs)
        {
            //if sampling rate is other than default or has been changed, recompile..
            samplingRate = (double)sampleRate;
            //the problem here is channels have already been instantiated, so no change triggers will take place..
            CabbageUtilities::debug("CsoundPluginProcessor::prepareToPlay - calling setupAndCompileCsound()");
            setupAndCompileCsound(csdFile, csdFilePath, samplingRate);
        }
    }
    if (preferredLatency == -1)
        this->setLatencySamples(0);
    else
	    this->setLatencySamples(preferredLatency == 0 ? csound->GetKsmps() : preferredLatency);
}

void CsoundPluginProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void CsoundPluginProcessor::handleAsyncUpdate()
{
    if(polling == 1)
    {
        getChannelDataFromCsound();
        sendChannelDataToCsound();
    }
    else if(polling == 0)
    {
        //getIdentifierDataFromCsound();
    }
    else{
        getChannelDataFromCsound();
        sendChannelDataToCsound();
        getIdentifierDataFromCsound();
    }
    
}

void CsoundPluginProcessor::sendHostDataToCsound()
{
//    if (CabbageUtilities::getTarget() != CabbageUtilities::TargetTypes::IDE)
//    {
        if (AudioPlayHead* const ph = getPlayHead())
        {
            AudioPlayHead::CurrentPositionInfo hostPlayHeadInfo = {};
            
            if (ph->getCurrentPosition (hostPlayHeadInfo))
            {
                if(csound)
                {
                    csound->SetChannel (CabbageIdentifierIds::hostbpm.toUTF8(), hostPlayHeadInfo.bpm);
                    csound->SetChannel (CabbageIdentifierIds::timeinseconds.toUTF8(), hostPlayHeadInfo.timeInSeconds);
                    csound->SetChannel (CabbageIdentifierIds::isplaying.toUTF8(), hostPlayHeadInfo.isPlaying);
                    csound->SetChannel (CabbageIdentifierIds::isrecording.toUTF8(), hostPlayHeadInfo.isRecording);
                    csound->SetChannel (CabbageIdentifierIds::hostppqpos.toUTF8(), hostPlayHeadInfo.ppqPosition);
                    csound->SetChannel (CabbageIdentifierIds::timeinsamples.toUTF8(), hostPlayHeadInfo.timeInSamples);
                    csound->SetChannel (CabbageIdentifierIds::timeSigDenom.toUTF8(), hostPlayHeadInfo.timeSigDenominator);
                    csound->SetChannel (CabbageIdentifierIds::timeSigNum.toUTF8(), hostPlayHeadInfo.timeSigNumerator);
                }
            }
        }
//    }
}

void CsoundPluginProcessor::performCsoundKsmps()
{
    if(csound == nullptr)
        return;
    
    result = csound->PerformKsmps();

    if (result == 0)
    {
        //slow down calls to these functions, no need for them to be firing at k-rate
        if(polling != 0)
        {
            if (guiCycles > guiRefreshRate)
            {
                guiCycles = 0;
                triggerAsyncUpdate();
            }
            else
                ++guiCycles;
        }
        else{
            //triggerAsyncUpdate();
        }
        //trigger any Csound score event on each k-boundary
        //triggerCsoundEvents();
        sendHostDataToCsound();

        disableLogging = false;
    }
    else
    {
        disableLogging = true;
        return; //return as soon as Csound has stopped
    }
}


template< typename Type >
void CsoundPluginProcessor::processIOBuffers(int bufferType, Type* buffer, int samplePosition, int csndPosition)
{
    //csdPos should go from 0 to (ksmps * number of channels)...
	if (bufferType == BufferType::inputOutput)
	{
       // DBG("Input/Output Buffer Pos: +" + String(csndPosition));
        MYFLT sample = buffer[samplePosition] * cs_scale;
        CSspin[csndPosition] = sample;
        
        //if we want 0 latency, we have to fill Csound spin buffer before we call performKsmps()
        if (preferredLatency == -1)
            performCsoundKsmps();
        buffer[samplePosition] = (CSspout[csndPosition] / cs_scale);
	}
	else if (bufferType == BufferType::output)
	{
        //DBG("Output Buffer Pos: +" + String(csndPosition));
        buffer[samplePosition] = (CSspout[csndPosition] / cs_scale);
	}
	else //input
	{
        if(buffer != nullptr)
        {
            //DBG("Input Buffer Pos: +" + String(csndPosition) + "\n");
            CSspin[csndPosition] = buffer[samplePosition] * cs_scale;
        }
        else
            CSspin[csndPosition] = 0;
	}
}

void CsoundPluginProcessor::processBlock(AudioBuffer< float >& buffer, MidiBuffer& midiMessages)
{
    processBlockListener.updateBlockTime();
    canUpdate.store(false);
	processSamples(buffer, midiMessages);
    canUpdate.store(true);
}

void CsoundPluginProcessor::processBlock(AudioBuffer< double >& buffer, MidiBuffer& midiMessages)
{
    processBlockListener.updateBlockTime();
    canUpdate.store(false);
	processSamples(buffer, midiMessages);
    canUpdate.store(true);
}

template< typename Type >
void CsoundPluginProcessor::processSamples(AudioBuffer< Type >& buffer, MidiBuffer& midiMessages)
{
	ScopedNoDenormals noDenormals;
    auto mainOutput = getBusBuffer(buffer, false, 0);
#if !JucePlugin_IsSynth
    auto mainInput = getBusBuffer(buffer, true, 0);
#endif
    
    Type** sideChainBuffer = nullptr;

	if (supportsSidechain)
	{
		sideChainBuffer = getBusBuffer(buffer, true, getBusCount(true)-1).getArrayOfWritePointers();
		numSideChainChannels = getBusBuffer(buffer, true, getBusCount(true) - 1).getNumChannels();
	}
#if !Cabbage_IDE_Build
    Type** ioBuffer = buffer.getArrayOfWritePointers();
#endif
#if !JucePlugin_IsSynth && !Cabbage_IDE_Build
	Type** inputBuffer = mainInput.getArrayOfWritePointers();
#endif
    const int numSamples = buffer.getNumSamples();

	const int outputChannelCount = (numCsoundOutputChannels > getTotalNumOutputChannels() ? getTotalNumOutputChannels() : numCsoundOutputChannels);
	const int inputChannelCount = (numCsoundInputChannels > getTotalNumInputChannels() ? getTotalNumInputChannels() : numCsoundInputChannels);

	//if no inputs are used clear buffer in case it's not empty..
	if (getTotalNumInputChannels() == 0)
		buffer.clear();

	keyboardState.processNextMidiBuffer(midiMessages, 0, numSamples, true);
    
    if(isLMMS)
	    midiBuffer.addEvents(midiMessages, 0, numSamples, 0);
    
    int samplePos = 0;
    MidiMessage message;
    MidiBuffer::Iterator iter (midiMessages);

	if (csdCompiledWithoutError())
	{
		////mute unused channels
		for (int channelsToClear = outputChannelCount; channelsToClear < getTotalNumOutputChannels(); ++channelsToClear)
		{
			buffer.clear(channelsToClear, 0, buffer.getNumSamples());
		}

		for (int i = 0; i < numSamples; i++, ++csndIndex)
		{
			if (csndIndex >= csdKsmps)
			{
                //don't call performKsmps here if we want 0 latency
                if(preferredLatency != -1)
				    performCsoundKsmps();
				csndIndex = 0;
			}
            
            if (isLMMS == false)
            {
                while (iter.getNextEvent(message, samplePos))
                {
                    //if current sample position matches time code for MIDI event, add it to buffer that gets sent to Csound as incoming MIDI...
                    if (samplePos == i)
                        midiBuffer.addEvent(message, samplePos);
                }

                iter.setNextSamplePosition(0);
            }
            
            //reset the iterator each time, so that we can step through the events again to see if they should be added
            
	
#if !JucePlugin_IsSynth
            const int numInputBuses = getBusCount(true);
            pos = csndIndex * inputChannelCount;
            
            for (int busIndex = 0; busIndex < numInputBuses; busIndex++)
            {
                auto inputBus = getBusBuffer(buffer, true, busIndex);
                Type** inputBuffer = inputBus.getArrayOfWritePointers();

                for (int channel = 0; channel < inputBus.getNumChannels(); channel++)
                {
                    processIOBuffers(BufferType::input, inputBuffer[channel], i, pos++);
                }
            }

            const int numOutputBuses = getBusCount(false);
            pos = csndIndex* outputChannelCount;
            
            for (int busIndex = 0; busIndex < numOutputBuses; busIndex++)
            {
                auto outputBus = getBusBuffer(buffer, false, busIndex);
                Type** outputBuffer = outputBus.getArrayOfWritePointers();

                for (int channel = 0; channel < outputBus.getNumChannels(); channel++)
                {
                    processIOBuffers(BufferType::output, outputBuffer[channel], i, pos++);
                }
            }
#else
            const int channelNum = buffer.getNumChannels();
            pos = csndIndex * channelNum;
            for (int channel = 0; channel < outputChannelCount; channel++)
            {
                processIOBuffers(BufferType::output, ioBuffer[channel], i, pos);
                pos++;
            }

#endif
		}
    }//if not compiled just mute output
    else
    {
        for (int channel = 0; channel < getTotalNumOutputChannels(); ++channel)
        {
            buffer.clear (channel, 0, buffer.getNumSamples());
        }
    }

    AudioBuffer<float> writerBuffer;
    writerBuffer.makeCopyOf(buffer);
    if (activeWriter.load() != nullptr)
    {
         activeWriter.load()->write (writerBuffer.getArrayOfWritePointers(), writerBuffer.getNumSamples());
    }
#if JucePlugin_ProducesMidiOutput

	if (!midiOutputBuffer.isEmpty())
	{
		midiMessages.clear();
		midiMessages.swapWith(midiOutputBuffer);
	}
	else
		midiMessages.clear();

#endif
}

//==============================================================================
void CsoundPluginProcessor::breakpointCallback (CSOUND* csound, debug_bkpt_info_t* bkpt_info, void* userdata)
{

    auto* ud = static_cast<CsoundPluginProcessor*>(userdata);
    const String instrument = "Instrument" + String (bkpt_info->breakpointInstr->p1);
    debug_variable_t* vp = bkpt_info->instrVarList;

    while (vp)
    {
        if (vp->name[0] != '#')
        {

            if (strcmp (vp->typeName, "i") == 0
                || strcmp (vp->typeName, "k") == 0)
            {
                auto* data = (MYFLT*) vp->data;
                ud->breakPointData.set (instrument, vp->name, data[0]);
            }
            else if (strcmp (vp->typeName, "S") == 0)
            {
                ud->breakPointData.set (instrument, vp->name, String ((char*)vp->data));
            }
            else if (strcmp (vp->typeName, "a") == 0)
            {
                auto* data = (MYFLT*) vp->data;
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
CsoundPluginProcessor::SignalDisplay* CsoundPluginProcessor::getSignalArray (String variableName, String displayType) const
{
    for (auto signalArray : signalArrays)
    {
        const String test = signalArray->caption;
        if (signalArray->caption.isNotEmpty() && signalArray->caption.contains (variableName))
        {
            const String varName = signalArray->variableName;
            if (displayType.isEmpty()){
                return signalArray;
            }

            else if (displayType == "waveform" && !signalArray->caption.contains ("fft")){
                return signalArray;
            }

            else if (displayType == "lissajous" && !signalArray->caption.contains ("fft")){
                return signalArray;
            }

            else if (displayType != "waveform" && signalArray->caption.contains ("fft")){
                return signalArray;
            }
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
    auto* midiData = static_cast<CsoundPluginProcessor*>(userData);

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
    auto* userData = static_cast<CsoundPluginProcessor*>(_userData);

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

void CsoundPluginProcessor::makeGraphCallback (CSOUND* csound, WINDAT* windat, const char* name)
{
    ignoreUnused(name);
    auto* ud = static_cast<CsoundPluginProcessor*>(csoundGetHostData (csound));
    auto* display = new SignalDisplay (String (windat->caption), (int)windat->windid, (float)windat->oabsmax, (int)windat->min, (int)windat->max, (int)windat->npts);

    bool addDisplay = true;

    for (auto signalArray : ud->signalArrays)
    {
        if (signalArray->caption == windat->caption)
            addDisplay  = false;
    }

    if (addDisplay && !String(windat->caption).contains("ftable"))
    {
        const String captionName = String(windat->caption).substring(String(windat->caption).indexOf("signal ")+7);
        const int posColon = String(captionName).indexOf(":");
        const int posComma = String(captionName).indexOf(",");
        String variableName = "";
        if(posComma != -1)
            variableName = captionName.substring(0, posComma);
        else
            variableName = captionName.substring(0, posColon);

        display->variableName = variableName;
        ud->signalArrays.add (display);
        ud->updateSignalDisplay.set(variableName, false);
    }
}

void CsoundPluginProcessor::drawGraphCallback (CSOUND* csound, WINDAT* windat)
{
    ignoreUnused(csound);
    auto* ud = static_cast<CsoundPluginProcessor*> (csoundGetHostData (csound));
    Array<float, CriticalSection> tablePoints;
    //only take all samples if dealing with fft, waveforms and lissajous curves can be drawn with less samples
    tablePoints = Array<float, CriticalSection> (&windat->fdata[0], windat->npts);
    ud->getSignalArray (windat->caption)->setPoints (tablePoints);
    const String test = ud->getSignalArray (windat->caption)->variableName;
    if(ud->getSignalArray (windat->caption)->variableName.isNotEmpty())
        ud->updateSignalDisplay.set(ud->getSignalArray (windat->caption)->variableName, true);
    
}

void CsoundPluginProcessor::killGraphCallback (CSOUND* csound, WINDAT* windat)
{
    ignoreUnused(csound, windat);

}

int CsoundPluginProcessor::exitGraphCallback (CSOUND* csound)
{
    ignoreUnused(csound);
    return 0;
}





