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
CsoundPluginProcessor::CsoundPluginProcessor (File csdFile, const AudioChannelSet ins, const AudioChannelSet outs)
    : AudioProcessor (BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
                                         .withInput  ("Input",  ins, true)
#endif
                                         .withOutput ("Output", outs, true)
#endif
                                        ),
      csdFile (csdFile)
{
    hostInfo = {};
	matchingNumberOfIOChannels = getTotalNumInputChannels() == getTotalNumOutputChannels() ? true : false;
    numCsoundOutputChannels = getBus(false, 0)->getNumberOfChannels();
    CabbageUtilities::debug("Constructor - Requested output channels:", numCsoundOutputChannels);
#if ! JucePlugin_IsSynth && ! JucePlugin_IsSynth
    numCsoundInputChannels = getBus(true, 0)->getNumberOfChannels();
    CabbageUtilities::debug("Constructor - Requested input channels:", numCsoundInputChannels);
#endif
}

//==============================================================================
// side chain constructor
//==============================================================================
//==============================================================================
CsoundPluginProcessor::CsoundPluginProcessor(File csdFile, const AudioChannelSet ins, const AudioChannelSet outs, const AudioChannelSet sideChainChannels)
	: AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
		.withInput("Input", ins, true).withInput("Sidechain", sideChainChannels, true)
#endif
		.withOutput("Output", outs, true)
#endif
	),
	csdFile(csdFile)
{
    hostInfo = {};
    numCsoundOutputChannels = getBus(false, 0)->getNumberOfChannels();
#if ! JucePlugin_IsSynth && ! JucePlugin_IsSynth

    numCsoundInputChannels = getBus(true, 0)->getNumberOfChannels();
#endif
    //side chaining is supported, and matchingNumberOfIOChannels must false
	matchingNumberOfIOChannels = false;
	supportsSidechain = true;
    numSideChainChannels = getBus(true, 1)->getNumberOfChannels();
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
        CabbagePersistentData** pd = (CabbagePersistentData**)getCsound()->QueryGlobalVariable("cabbageData");
        if(pd != nullptr)
            getCsound()->DestroyGlobalVariable("cabbageData");
        
        CabbageWidgetIdentifiers** wi = (CabbageWidgetIdentifiers**)getCsound()->QueryGlobalVariable("cabbageWidgetData");
        if(wi != nullptr)
            getCsound()->DestroyGlobalVariable("cabbageWidgetData");
        
#if !defined(Cabbage_Lite)
		csound = nullptr;
#endif
		csoundParams = nullptr;
		editorBeingDeleted(this->getActiveEditor());
	}
}

//==============================================================================
//==============================================================================
bool CsoundPluginProcessor::setupAndCompileCsound(File currentCsdFile, File filePath, int sr, bool isMono, bool debugMode)
{
    
    csdFile = currentCsdFile;
    
    StringArray csdLines;
    csdLines.addLines(csdFile.loadFileAsString());
    for (auto line : csdLines)
    {
        ValueTree temp("temp");
        CabbageWidgetData::setWidgetState(temp, line, 0);

        if (CabbageWidgetData::getStringProp(temp, CabbageIdentifierIds::type) == CabbageWidgetTypes::form)
        {
            if(CabbageWidgetData::getStringProp(temp, CabbageIdentifierIds::opcodedir).isNotEmpty()) {
                const String opcodeDir = csdFile.getParentDirectory().getChildFile(
                        CabbageWidgetData::getStringProp(temp, CabbageIdentifierIds::opcodedir)).getFullPathName();
                csoundSetOpcodedir(opcodeDir.toUTF8().getAddress());
            }
            if (CabbageWidgetData::getNumProp(temp, CabbageIdentifierIds::latency) == -1) {
                preferredLatency = -1;
            }
        }
    }
    
    CabbageUtilities::debug(csdFile.getFullPathName());
    
    // the host should respect the default inputs and outs, which are determined by the
    // nhcnls and nchnls_i variables in Csound. But the host is king. If it requested a different
    // config, we must adhere to it.
    numCsoundOutputChannels = getBus(false, 0)->getNumberOfChannels();
    CabbageUtilities::debug("SetupAndCompile - Requested output channels:", numCsoundOutputChannels);
#if ! JucePlugin_IsSynth && ! JucePlugin_IsSynth
    const int inputs = getBus(true, 0)->getNumberOfChannels();
    numCsoundInputChannels = inputs + numSideChainChannels;
    CabbageUtilities::debug("SetupAndCompile - Requested input channels:", numCsoundInputChannels);
#endif
    //int test = csound->SetGlobalEnv("OPCODE6DIR64", );
    CabbageUtilities::debug("Env var set");
    //csoundSetOpcodedir("/Library/Frameworks/CsoundLib64.framework/Versions/6.0/Resources/Opcodes64");
    //Logger::writeToLog(String::formatted("Resetting csound ...\ncsound = 0x%p", csound.get()));
	csound.reset (new Csound());
    
	csdFilePath = filePath;
	csdFilePath.setAsCurrentWorkingDirectory();
	csound->SetHostImplementedMIDIIO(true);
	csound->SetHostImplementedAudioIO(1, 0);
	csound->SetHostData(this);

    csnd::plugin<StrToFile>((csnd::Csound*) csound->GetCsound(), "strToFile.SSO", "i", "SSO", csnd::thread::i);
    csnd::plugin<FileToStr>((csnd::Csound*) csound->GetCsound(), "fileToStr.i", "S", "S", csnd::thread::i);

    csnd::plugin<ChannelStateSave>((csnd::Csound*) csound->GetCsound(), "channelStateSave.i", "i", "S", csnd::thread::i);
    csnd::plugin<ChannelStateSave>((csnd::Csound*) csound->GetCsound(), "channelStateSave.k", "k", "S", csnd::thread::k);

    csnd::plugin<ChannelStateRecall>((csnd::Csound*) csound->GetCsound(), "channelStateRecall.i", "i", "S", csnd::thread::i);
    csnd::plugin<ChannelStateRecall>((csnd::Csound*) csound->GetCsound(), "channelStateRecall.k", "k", "SO", csnd::thread::k);
    csnd::plugin<ChannelStateRecall>((csnd::Csound*) csound->GetCsound(), "channelStateRecall.k", "k", "SS[]", csnd::thread::k);

    
    csnd::plugin<StrToArray>((csnd::Csound*) csound->GetCsound(), "strToArray.ii", "S[]", "SS", csnd::thread::i);
    csnd::plugin<StrRemove>((csnd::Csound*) csound->GetCsound(), "strRemove.ii", "S", "SSo", csnd::thread::i);

    csnd::plugin<WriteStateData>((csnd::Csound*) csound->GetCsound(), "writeStateData.ss", "i", "iS", csnd::thread::i);
    csnd::plugin<ReadStateData>((csnd::Csound*) csound->GetCsound(), "readStateData.i", "S", "", csnd::thread::i);

    csnd::plugin<GetStateFloatValue>((csnd::Csound*) csound->GetCsound(), "getStateValue.s", "i", "S", csnd::thread::i);
    csnd::plugin<GetStateFloatValue>((csnd::Csound*) csound->GetCsound(), "getStateValue.s", "k", "S", csnd::thread::ik);
    csnd::plugin<GetStateFloatValueArray>((csnd::Csound*) csound->GetCsound(), "getStateValue.s", "k[]", "S", csnd::thread::ik);
    csnd::plugin<GetStateStringValue>((csnd::Csound*) csound->GetCsound(), "getStateValue.s", "S", "S", csnd::thread::i);
    csnd::plugin<GetStateStringValueArray>((csnd::Csound*) csound->GetCsound(), "getStateValue.s", "S[]", "S", csnd::thread::ik);

    csnd::plugin<SetStateFloatData>((csnd::Csound*) csound->GetCsound(), "setStateValue.s", "k", "Sk", csnd::thread::ik);
    csnd::plugin<SetStateFloatData>((csnd::Csound*) csound->GetCsound(), "setStateValue.s", "i", "Si", csnd::thread::i);

    csnd::plugin<SetStateFloatArrayData>((csnd::Csound*) csound->GetCsound(), "setStateValue.s", "i", "Si[]", csnd::thread::i);
    csnd::plugin<SetStateFloatArrayData>((csnd::Csound*) csound->GetCsound(), "setStateValue.s", "k", "Sk[]", csnd::thread::ik);

    csnd::plugin<SetStateStringData>((csnd::Csound*) csound->GetCsound(), "setStateValue.s", "i", "SS", csnd::thread::i);
    csnd::plugin<SetStateStringData>((csnd::Csound*) csound->GetCsound(), "setStateValue.s", "k", "SS", csnd::thread::ik);

    csnd::plugin<SetStateStringArrayData>((csnd::Csound*) csound->GetCsound(), "setStateValue.s", "i", "SS[]", csnd::thread::i);
    csnd::plugin<SetStateStringArrayData>((csnd::Csound*) csound->GetCsound(), "setStateValue.s", "k", "SS[]", csnd::thread::ik);

    csnd::plugin<SetCabbageIdentifier>((csnd::Csound*) csound->GetCsound(), "cabbageSet", "", "kSSM", csnd::thread::k);
   
    csnd::plugin<SetCabbageIdentifierITimeSArgs>((csnd::Csound*) csound->GetCsound(), "cabbageSet", "", "SW", csnd::thread::i);
    csnd::plugin<SetCabbageIdentifierITime>((csnd::Csound*) csound->GetCsound(), "cabbageSet", "", "SSN", csnd::thread::i);

    
    //csnd::plugin<SetCabbageIdentifierSArgs>((csnd::Csound*) csound->GetCsound(), "cabbageSet", "", "kSSW", csnd::thread::ik);
    csnd::plugin<SetCabbageIdentifierSArgs>((csnd::Csound*) csound->GetCsound(), "cabbageSet", "", "kSW", csnd::thread::ik);
    csnd::plugin<SetCabbageValueIdentifier>((csnd::Csound*) csound->GetCsound(), "cabbageSetValue", "", "SkP", csnd::thread::ik);
    
    csnd::plugin<GetCabbageValue>((csnd::Csound*) csound->GetCsound(), "cabbageGetValue", "k", "S", csnd::thread::ik);
    csnd::plugin<GetCabbageValue>((csnd::Csound*) csound->GetCsound(), "cabbageGetValue", "i", "S", csnd::thread::ik);
    csnd::plugin<GetCabbageValueWithTrigger>((csnd::Csound*) csound->GetCsound(), "cabbageGetValue", "kk", "S", csnd::thread::ik);
    
    csnd::plugin<GetCabbageStringValue>((csnd::Csound*) csound->GetCsound(), "cabbageGetValue", "S", "S", csnd::thread::ik);
    csnd::plugin<GetCabbageStringValueWithTrigger>((csnd::Csound*) csound->GetCsound(), "cabbageGetValue", "Sk", "S", csnd::thread::ik);

    csnd::plugin<GetCabbageIdentifierArray>((csnd::Csound*) csound->GetCsound(), "cabbageGet", "k[]", "SS", csnd::thread::ik);
    csnd::plugin<GetCabbageStringIdentifierArray>((csnd::Csound*) csound->GetCsound(), "cabbageGet", "S[]", "SS", csnd::thread::ik);
    csnd::plugin<GetCabbageIdentifierSingle>((csnd::Csound*) csound->GetCsound(), "cabbageGet", "k", "SS", csnd::thread::ik);
    csnd::plugin<GetCabbageStringIdentifierSingle>((csnd::Csound*) csound->GetCsound(), "cabbageGet", "S", "SS", csnd::thread::ik);

    csnd::plugin<GetCabbageReservedChannelStringWithTrigger>((csnd::Csound*) csound->GetCsound(), "cabbageGet", "Sk", "S", csnd::thread::ik);
    csnd::plugin<GetCabbageReservedChannelString>((csnd::Csound*) csound->GetCsound(), "cabbageGet", "S", "S", csnd::thread::ik);
    
    csnd::plugin<GetCabbageReservedChannelDataWithTrigger>((csnd::Csound*) csound->GetCsound(), "cabbageGet", "kk", "S", csnd::thread::ik);
    csnd::plugin<GetCabbageReservedChannelData>((csnd::Csound*) csound->GetCsound(), "cabbageGet", "k", "S", csnd::thread::ik);

    csnd::plugin<CreateCabbageWidget>((csnd::Csound*) csound->GetCsound(), "cabbageCreate", "", "S", csnd::thread::i);
    //csound->CreateGlobalVariable("cabbageData", sizeof(CabbagePersistentData*));
    //CabbagePersistentData** pd = (CabbagePersistentData**)csound->QueryGlobalVariable("cabbageData");
    //*pd = new CabbagePersistentData();
    //auto pdClass = *pd;
    //pdClass->data = "{}";
    
	csound->CreateMessageBuffer(0);
	csound->SetExternalMidiInOpenCallback(OpenMidiInputDevice);
	csound->SetExternalMidiReadCallback(ReadMidiData);
	csound->SetExternalMidiOutOpenCallback(OpenMidiOutputDevice);
	csound->SetExternalMidiWriteCallback(WriteMidiData);
	csoundParams = nullptr;
	csoundParams.reset (new CSOUND_PARAMS());

	csoundParams->displays = 0;

	csound->SetIsGraphable(true);
	csound->SetMakeGraphCallback(makeGraphCallback);
	csound->SetDrawGraphCallback(drawGraphCallback);
	csound->SetKillGraphCallback(killGraphCallback);
	csound->SetExitGraphCallback(exitGraphCallback);
	csound->SetOption((char*)"-n");
	csound->SetOption((char*)"-d");
	csound->SetOption((char*)"-b0");
    
    addMacros(csdFile.loadFileAsString());

	if (debugMode)
	{
		csoundDebuggerInit(csound->GetCsound());
		csoundSetBreakpointCallback(csound->GetCsound(), breakpointCallback, (void*)this);
		csoundSetInstrumentBreakpoint(csound->GetCsound(), 1, 413);
		csoundParams->ksmps_override = 4410;
	}

	
    if(isMono)
    {
        //this mode is for logic and cubase
        numCsoundOutputChannels = 1;
        numCsoundInputChannels = 1 + (numSideChainChannels > 0 ? 1 : 0);
        csoundParams->nchnls_override = numCsoundOutputChannels;
        csoundParams->nchnls_i_override = numCsoundInputChannels;
        if (supportsSidechain)
        {
            getBus(true, 1)->setNumberOfChannels(1);
            numSideChainChannels = 1;
        }
    }
    else
    {
        csoundParams->nchnls_override = numCsoundOutputChannels;
        csoundParams->nchnls_i_override = numCsoundInputChannels;
    }
    
    // Update the matchingNumberOfIOChannels flag so the MacOS auval tool doesn't crash when validating
    // different I/O channel configurations.
    if (csoundParams->nchnls_i_override != csoundParams->nchnls_override)
    {
        matchingNumberOfIOChannels = false;
    }
	
#ifdef CabbagePro
	const int requestedSampleRate = CabbageUtilities::getHeaderInfo(Encrypt::decode(csdFile), "sr");
	const int requestedKsmpsRate = CabbageUtilities::getHeaderInfo(Encrypt::decode(csdFile), "ksmps");
#else
	const int requestedKsmpsRate = CabbageUtilities::getHeaderInfo(csdFile.loadFileAsString(), "ksmps");
	const int requestedSampleRate = CabbageUtilities::getHeaderInfo(csdFile.loadFileAsString(), "sr");
#endif
	
	if (requestedKsmpsRate == -1)
		csoundParams->ksmps_override = 32;

	csoundParams->sample_rate_override = requestedSampleRate>0 ? requestedSampleRate : sr;

    if(preferredLatency == -1)
        csoundParams->ksmps_override = 1;

	csound->SetParams(csoundParams.get());
    
    if (csdFile.loadFileAsString().contains("<Csound") || csdFile.loadFileAsString().contains("</Csound"))
    {
        compileCsdFile(csdFile);
    }
    else
	{
#ifdef CabbagePro
		compileCsdString(Encrypt::decode(csdFile));
		csound->Start();
#endif
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
    
    getCsound()->CreateGlobalVariable("cabbageData", sizeof(CabbagePersistentData*));
    CabbagePersistentData** pd = (CabbagePersistentData**)getCsound()->QueryGlobalVariable("cabbageData");
    *pd = new CabbagePersistentData();
    auto pdClass = *pd;
    pdClass->data = getInternalState().toStdString();


    for (int i = 0; i < cabbageData.getNumChildren(); i++)
    {
        const String typeOfWidget = CabbageWidgetData::getStringProp (cabbageData.getChild (i), CabbageIdentifierIds::type);
        if(typeOfWidget == CabbageWidgetTypes::form)
        {
            const int latency = CabbageWidgetData::getNumProp (cabbageData.getChild (i), CabbageIdentifierIds::latency);
            preferredLatency = latency;
            //DBG(CabbageWidgetData::getNumProp(cabbageData.getChild(i), CabbageIdentifierIds::width)
            csound->SetChannel("SCREEN_WIDTH", CabbageWidgetData::getNumProp(cabbageData.getChild(i), CabbageIdentifierIds::width));
            csound->SetChannel("SCREEN_HEIGHT", CabbageWidgetData::getNumProp(cabbageData.getChild(i), CabbageIdentifierIds::height));
        }

        if (CabbageWidgetData::getStringProp (cabbageData.getChild (i), CabbageIdentifierIds::channeltype) == "string")
        {
            if (typeOfWidget == CabbageWidgetTypes::json)
            {
                
            }
            if (typeOfWidget == CabbageWidgetTypes::filebutton)
            {
                const String mode = CabbageWidgetData::getStringProp (cabbageData.getChild (i), CabbageIdentifierIds::mode);
                if( mode == "file" || mode == "save" || mode == "directory")
                {
                    csound->SetStringChannel (CabbageWidgetData::getStringProp (cabbageData.getChild (i), CabbageIdentifierIds::channel).getCharPointer(),
                                          CabbageWidgetData::getStringProp (cabbageData.getChild (i), CabbageIdentifierIds::file).toUTF8().getAddress());
                }
            }

            else
            {
				if (typeOfWidget == CabbageWidgetTypes::combobox)
				{
					//if we are dealing with a combobox that reads files from a directory, we need to load them before the GUI opens...
					if (CabbageWidgetData::getStringProp(cabbageData.getChild(i), "filetype") != "preset"
						&& CabbageWidgetData::getStringProp(cabbageData.getChild(i), "filetype") != "*.snaps"
						&& CabbageWidgetData::getStringProp(cabbageData.getChild(i), "filetype") != ".snaps"
						&& CabbageWidgetData::getStringProp(cabbageData.getChild(i), "filetype") != "snaps")
					{
						const String workingDir = CabbageWidgetData::getStringProp(cabbageData.getChild(i), CabbageIdentifierIds::workingdir);
						const String fileType = CabbageWidgetData::getStringProp(cabbageData.getChild(i), "filetype");
						int numOfFiles;
						Array<File> folderFiles;
						StringArray comboItems;
						CabbageUtilities::searchDirectoryForFiles(cabbageData.getChild(i), workingDir, fileType, folderFiles, comboItems, numOfFiles);
						const String currentValue = CabbageWidgetData::getStringProp(cabbageData.getChild(i), CabbageIdentifierIds::value);

						const int index = comboItems.indexOf(currentValue) + 1;
						const String test = folderFiles[index].getFullPathName();
						const String channel = CabbageWidgetData::getStringProp(cabbageData.getChild(i), CabbageIdentifierIds::channel);

						csound->SetStringChannel(CabbageWidgetData::getStringProp(cabbageData.getChild(i), CabbageIdentifierIds::channel).getCharPointer(),
							folderFiles[index].getFullPathName().replaceCharacters("\\", "/").toUTF8().getAddress());
						
					}
                    else{
//                        const String test = CabbageWidgetData::getProperty(cabbageData.getChild(i), CabbageIdentifierIds::value);
//                        const String channel = CabbageWidgetData::getStringProp(cabbageData.getChild(i), CabbageIdentifierIds::channel);
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
                    csound->SetChannel (channels[0].toString().getCharPointer(), float (maxValue));
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
            }

        }

    }

    getCsound()->CreateGlobalVariable("cabbageWidgetsValueTree", sizeof(CabbageWidgetsValueTree*));
    CabbageWidgetsValueTree** vt = (CabbageWidgetsValueTree**)getCsound()->QueryGlobalVariable("cabbageWidgetsValueTree");
    *vt = new CabbageWidgetsValueTree();
    auto valueTree = *vt;
    valueTree->data = cabbageData;
    
    if (CabbageUtilities::getTargetPlatform() == CabbageUtilities::TargetPlatformTypes::Win32)
    {
        csound->SetChannel ("CSD_PATH", csdFilePath.getParentDirectory().getFullPathName().replace ("\\", "\\\\").toUTF8().getAddress());
    }
    else
    {
        csound->SetChannel ("CSD_PATH", csdFilePath.getFullPathName().toUTF8().getAddress());
    }

    csound->SetStringChannel ("LAST_FILE_DROPPED", const_cast<char*> (""));

    csound->SetChannel ("IS_EDITOR_OPEN", 0.0);
    csdFilePath.setAsCurrentWorkingDirectory();

    DBG(File::getSpecialLocation (File::userHomeDirectory).getFileIdentifier());
    csound->SetChannel("HOME_FOLDER_UID", File::getSpecialLocation (File::userHomeDirectory).getFileIdentifier());

    time_t seconds_past_epoch = time(0);
    csound->SetChannel("SECONDS_SINCE_EPOCH", (intmax_t)seconds_past_epoch);
    // convert now to string form
    char* dt = ctime(&seconds_past_epoch);
    std::stringstream strStream;
    strStream << dt << endl;
    csound->SetStringChannel("CURRENT_DATE_TIME", String(strStream.str()).toUTF8().getAddress());


	if((SystemStats::getOperatingSystemType() & SystemStats::OperatingSystemType::Linux) != 0)
    {
		csound->SetChannel ("LINUX", 1.0);
        csound->SetChannel ("Linux", 1.0);
    }
	if((SystemStats::getOperatingSystemType() & SystemStats::OperatingSystemType::MacOSX) != 0)
    {
		csound->SetChannel ("MAC", 1.0);
        csound->SetChannel ("Macos", 1.0);
        csound->SetChannel ("MACOS", 1.0);
    }
	if((SystemStats::getOperatingSystemType() & SystemStats::OperatingSystemType::Windows) != 0)
    {
		csound->SetChannel ("Windows", 1.0);
        csound->SetChannel ("WINDOWS", 1.0);
    }

    //csound->SetChannel ("CHNSET_GESTURES", 0.0);
    
#if !defined(Cabbage_IDE_Build)
    PluginHostType pluginType;
    if (pluginType.isFruityLoops())
        csound->SetChannel ("FLStudio", 1.0);
    else if (pluginType.isAbletonLive())
        csound->SetChannel ("AbletonLive", 1.0);
    else if (pluginType.isLogic()){
        csound->SetChannel ("Logic", 1.0);
        isLogic = true;
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


    csound->PerformKsmps();

    Logger::writeToLog("initAllCsoundChannels (ValueTree cabbageData) - done");

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
                tokens.addTokens (csdArray[i].replace ("#", "").trim() , " ");
                tokens.removeEmptyStrings();
                macroName = tokens[1].trim();
                tokens.remove (0);
                macroText = "\"" + tokens.joinIntoString (" ").replace (" ", "\ ").replace("\"", "\\\"")+"\"";
                macroText = tokens.joinIntoString(" ");
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
    MatrixEventSequencer* matrix = new MatrixEventSequencer(channel);

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

void CsoundPluginProcessor::setMatrixEventSequencerCellData(int col, int row, String channel, String data)
{
	if (numMatrixEventSequencers > 0)
	{
		for (int i = 0; i < matrixEventSequencers.size(); i++)
		{
			if (matrixEventSequencers[i]->channel == channel)
			{
				matrixEventSequencers[i]->setEventString(col, row, data);
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

    return String();
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
#if !defined(Cabbage_IDE_Build)
    PluginHostType pluginType;
    if (pluginType.isCubase())
        hostIsCubase = true;
#endif
    
#if ! JucePlugin_IsSynth
    const int inputs = getBus(true, 0)->getNumberOfChannels();
    CabbageUtilities::debug("CsoundPluginProcessor::prepareToPlay - inputs:", inputs);
    CabbageUtilities::debug("CsoundPluginProcessor::prepareToPlay - Requested input channels:", numCsoundInputChannels);
#endif
    const int outputs = getBus(false, 0)->getNumberOfChannels();
    CabbageUtilities::debug("CsoundPluginProcessor::prepareToPlay - outputs:", outputs);
    CabbageUtilities::debug("CsoundPluginProcessor::prepareToPlay - Requested output channels:", numCsoundOutputChannels);

    int sideChainChannels = 0;
    if (supportsSidechain)
    {
        numSideChainChannels = getBus(true, 0)->getNumberOfChannels();
    }
    
    
    if((samplingRate != sampleRate)
       || hostRequestedMono
#if ! JucePlugin_IsSynth && ! JucePlugin_IsSynth
       || numCsoundInputChannels != inputs
#endif
       || numCsoundOutputChannels != outputs 
       || numSideChainChannels != sideChainChannels)
    {
        //if sampling rate is other than default or has been changed, recompile..
        samplingRate = sampleRate;
        //allow mono plugins for Logic only..
        if(hostRequestedMono == true)
            setupAndCompileCsound(csdFile, csdFilePath, samplingRate, true);
        else
            setupAndCompileCsound(csdFile, csdFilePath, samplingRate);
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

bool CsoundPluginProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{

#if JucePlugin_IsMidiEffect
        ignoreUnused (layouts);
        return true;
#else
    

    
    const AudioChannelSet& mainInput  = layouts.getMainInputChannelSet();
    const AudioChannelSet& mainOutput = layouts.getMainOutputChannelSet();
    
    CabbageUtilities::debug("MainOutputSize:", mainOutput.size());
    CabbageUtilities::debug("MainInputSize:", mainInput.size());
    
#if  JucePlugin_IsSynth
    if(PluginHostType().isCubase()){
        if (mainInput.size() == 0 && mainOutput.size() == numCsoundOutputChannels)
            return true;
    }
#endif
    
    if(AudioProcessor::wrapperType == wrapperType_AudioUnit)
    {
#if  JucePlugin_IsSynth
        //synth can only be mono or stereo...
//        if (mainInput.size() == 0 && mainOutput.size() == 1)
//            return true;
//        if (mainInput.size() == 0 && mainOutput.size() == 2)
//            return true;
        if (mainInput.size() == 0 && mainOutput.size() == numCsoundOutputChannels)
            return true;

#else
#if !defined(Cabbage_IDE_Build) && !defined(Cabbage_Lite)
        if (PluginHostType().isReaper())
        {
            if(numSideChainChannels > 0)
            {
                if ((mainInput.size() == numCsoundInputChannels - numSideChainChannels || mainInput.size() == mainOutput.size())
                    && mainOutput.size() == numCsoundOutputChannels)
                {
                    return true;
                }
            }
            else
            {
                if(mainInput.size() == numCsoundInputChannels && mainOutput.size() == numCsoundOutputChannels)
                    return true;
            }
            
        }
        else
#endif
        {   //hack to get passed AU validation in logic
            
        if((mainInput.size()> 0 && mainInput.size()<32) && (mainOutput.size()>0 && mainOutput.size()<32))
            return true;
        }
#endif
        return false;
    }

#if defined(Cabbage_IDE_Build) && defined(Cabbage_Lite)
    if(mainInput.size() == numCsoundInputChannels && mainOutput.size() == numCsoundOutputChannels)
        return true;
#endif
    
    if ((mainInput.size() == numCsoundInputChannels - numSideChainChannels || mainInput.size() == mainOutput.size()) 
        && mainOutput.size() == numCsoundOutputChannels)
    {
        return true;
    }
        

//    if(numSideChainChannels==0)
    if (mainInput.size() == 1 && mainOutput.size() == 1)
    {
#if defined(Cabbage_IDE_Build)
        return true;
#else
        if (PluginHostType().isReaper())
        {
            // Return `false` to make Reaper test for layouts other than 1 in and 1 out.
            return false;
        }
        else
        {
            return true;
        }
#endif
    }
        

    return false;
    
#endif

}

//==========================================================================
void CsoundPluginProcessor::triggerCsoundEvents()
{

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
        getIdentifierDataFromCsound();
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
//    }
}

void CsoundPluginProcessor::performCsoundKsmps()
{
	result = csound->PerformKsmps();

	if (result == 0)
	{
        //slow down calls to these functions, no need for them to be firing at k-rate
        if(polling == true){
        if (guiCycles > guiRefreshRate)
        {
            guiCycles = 0;
            triggerAsyncUpdate();
        }
        else
            ++guiCycles;
        }
        else{
            triggerAsyncUpdate();
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
void CsoundPluginProcessor::processCsoundIOBuffers(int bufferType, Type*& buffer, int pos)
{
	if (bufferType == BufferType::inputOutput)
	{
		Type*& current_sample = buffer;
		MYFLT sample = *current_sample * cs_scale;
		CSspin[pos] = sample;
        //if we want 0 latency, we have to fill Csound spin buffer before we call performKsmps()
        if (preferredLatency == -1)
            performCsoundKsmps();
		*current_sample = (CSspout[pos] / cs_scale);
		++current_sample;
	}
	else if (bufferType == BufferType::output)
	{
		Type*& current_sample = buffer;
		*current_sample = (CSspout[pos] / cs_scale);
		++current_sample;
	}
	else //input
	{
        if(buffer != nullptr)
        {
			Type*& current_sample = buffer;
            MYFLT newSamp = *current_sample * cs_scale;
            CSspin[pos] = newSamp;
            current_sample++;
        }
        else
            CSspin[pos] = 0;
	}
}

template< typename Type >
void CsoundPluginProcessor::processCsoundIOSideChainBuffers(int bufferType, Type* buffer, int pos)
{
    if (buffer != nullptr)
    {
        Type* current_sample = buffer;
        MYFLT newSamp = *current_sample * cs_scale;
        CSspin[pos] = newSamp;
        current_sample++;
    }
    else
        CSspin[pos] = 0;
}

void CsoundPluginProcessor::processBlock(AudioBuffer< float >& buffer, MidiBuffer& midiMessages)
{
	processSamples(buffer, midiMessages);
}

void CsoundPluginProcessor::processBlock(AudioBuffer< double >& buffer, MidiBuffer& midiMessages)
{
	processSamples(buffer, midiMessages);
}

template< typename Type >
void CsoundPluginProcessor::processSamples(AudioBuffer< Type >& buffer, MidiBuffer& midiMessages)
{
	ScopedNoDenormals noDenormals;
	auto mainOutput = getBusBuffer(buffer, false, 0);
#if !JucePlugin_IsSynth
	auto mainInput = getBusBuffer(buffer, true, 0);
#endif
    
    const Type** sideChainCubase = nullptr;
    Type** sideChainBuffer = nullptr;

	if (supportsSidechain)
	{
        sideChainCubase = getBusBuffer(buffer, true, 1).getArrayOfReadPointers();
		sideChainBuffer = getBusBuffer(buffer, true, 1).getArrayOfWritePointers();
		numSideChainChannels = getBusBuffer(buffer, true, 1).getNumChannels();
	}

    Type** outputBuffer = mainOutput.getArrayOfWritePointers();
#if !JucePlugin_IsSynth
	Type** inputBuffer = mainInput.getArrayOfWritePointers();
#endif
    const int numSamples = buffer.getNumSamples();

	const int outputChannelCount = (numCsoundOutputChannels > getTotalNumOutputChannels() ? getTotalNumOutputChannels() : numCsoundOutputChannels);
	const int inputChannelCount = (numCsoundInputChannels > getTotalNumInputChannels() ? getTotalNumInputChannels() : numCsoundInputChannels);

	//if no inputs are used clear buffer in case it's not empty..
	if (getTotalNumInputChannels() == 0)
		buffer.clear();

	keyboardState.processNextMidiBuffer(midiMessages, 0, numSamples, true);
    
    //events need to be added at the correct time...
	//midiBuffer.addEvents(midiMessages, 0, numSamples, 0);
    
    int samplePos = 0;
    MidiMessage message;
    MidiBuffer::Iterator iter (midiMessages);

	if (csdCompiledWithoutError())
	{
		//mute unused channels
		for (int channelsToClear = outputChannelCount; channelsToClear < getTotalNumOutputChannels(); ++channelsToClear)
		{
			buffer.clear(channelsToClear, 0, buffer.getNumSamples());
		}

		for (int i = 0; i < numSamples; i++, ++csndIndex)
		{
			if (csndIndex == csdKsmps)
			{
                //don't call performKsmps here if we want 0 latency
                if(preferredLatency != -1)
				    performCsoundKsmps();
				csndIndex = 0;
			}
            
            while(iter.getNextEvent (message, samplePos))
            {
                //if current sample position matches time code for MIDI event, add it to buffer that gets sent to Csound as incoming MIDI...
                if(samplePos == i)
                    midiBuffer.addEvent(message, samplePos);
            }
            
            //reset the iterator each time, so that we can step through the events again to see if they should be added
            iter.setNextSamplePosition(0);
	
#if !JucePlugin_IsSynth
            //if using Logic process inputs and outputs separately - otherwise its mono to stereo features break...
			if (matchingNumberOfIOChannels && !isLogic)
			{
				pos = csndIndex * outputChannelCount;
				for (int channel = 0; channel < outputChannelCount; channel++)
				{
					processCsoundIOBuffers(BufferType::inputOutput, outputBuffer[channel], pos);
					pos++;
				}
			}
			else if (!supportsSidechain)
			{
				pos = csndIndex * inputChannelCount;
				for (int channel = 0; channel < inputChannelCount; channel++)
				{
					processCsoundIOBuffers(BufferType::input, inputBuffer[channel], pos);
					pos++;
				}

				pos = csndIndex * outputChannelCount;
				for (int channel = 0; channel < outputChannelCount; channel++)
				{
					processCsoundIOBuffers(BufferType::output, outputBuffer[channel], pos);
					pos++;
				}
			}
			else {
				//sidechain processing
				pos = csndIndex * inputChannelCount;
				for (int channel = 0; channel < inputChannelCount; channel++)
				{
					if(channel< numSideChainChannels)
						processCsoundIOBuffers(BufferType::input, inputBuffer[channel], pos);
                    else{
                        if(hostIsCubase)
                            processCsoundIOSideChainBuffers(BufferType::input, sideChainCubase[channel-numSideChainChannels], pos);
                        else
                            processCsoundIOBuffers(BufferType::input, sideChainBuffer[channel-numSideChainChannels], pos);
                    }
					pos++;
				}

				pos = csndIndex * outputChannelCount;
				for (int channel = 0; channel < outputChannelCount; channel++)
				{
					processCsoundIOBuffers(BufferType::output, outputBuffer[channel], pos);
					pos++;
				}
			}
#else
            pos = csndIndex * outputChannelCount;
            for (int channel = 0; channel < outputChannelCount; channel++)
            {
                processCsoundIOBuffers(BufferType::output, outputBuffer[channel], pos);
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

    CsoundPluginProcessor* ud = static_cast<CsoundPluginProcessor*>(userdata);
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
        const String test = signalArrays[i]->caption;
        if (signalArrays[i]->caption.isNotEmpty() && signalArrays[i]->caption.contains (variableName))
        {
            const String varName = signalArrays[i]->variableName;
            if (displayType.isEmpty()){
                return signalArrays[i];
            }

            else if (displayType == "waveform" && !signalArrays[i]->caption.contains ("fft")){
                return signalArrays[i];
            }

            else if (displayType == "lissajous" && !signalArrays[i]->caption.contains ("fft")){
                return signalArrays[i];
            }

            else if (displayType != "waveform" && signalArrays[i]->caption.contains ("fft")){
                return signalArrays[i];
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
    CsoundPluginProcessor* midiData = static_cast<CsoundPluginProcessor*>(userData);

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
    CsoundPluginProcessor* userData = static_cast<CsoundPluginProcessor*>(_userData);

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
    CsoundPluginProcessor* ud = static_cast<CsoundPluginProcessor*>(csoundGetHostData (csound));
    SignalDisplay* display = new SignalDisplay (String (windat->caption), (int)windat->windid, windat->oabsmax, windat->min, windat->max, windat->npts);

    bool addDisplay = true;

    for (int i = 0; i < ud->signalArrays.size(); i++)
    {
        if (ud->signalArrays[i]->caption == windat->caption)
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
    CsoundPluginProcessor* ud = static_cast<CsoundPluginProcessor*> (csoundGetHostData (csound));
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

}

int CsoundPluginProcessor::exitGraphCallback (CSOUND* csound)
{
    return 0;
}





