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
CsoundPluginProcessor::CsoundPluginProcessor (File csdFile, const BusesProperties ioBuses)
    : AudioProcessor (ioBuses),
csdFile (csdFile)
{
    hostInfo = {};  
	matchingNumberOfIOChannels = getTotalNumInputChannels() == getTotalNumOutputChannels() ? true : false;

#if ! JucePlugin_IsSynth && ! Cabbage_IDE_Build
    numCsoundInputChannels = getTotalNumInputChannels();
    numCsoundOutputChannels = getTotalNumOutputChannels();
    CabbageUtilities::debug("Cabbage Csound Constructor - Requested input channels:", numCsoundInputChannels);
    CabbageUtilities::debug("Cabbage Csound Constructor - Requested output channels:", numCsoundOutputChannels);
      
#else  
    numCsoundOutputChannels = getTotalNumOutputChannels();
    CabbageUtilities::debug("Constructor - Requested output channels:", numCsoundOutputChannels);
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
#if !defined(Cabbage_Lite)
		csound = nullptr;
#endif
		csoundParams = nullptr;
		editorBeingDeleted(this->getActiveEditor());
	}
}

//==============================================================================
void CsoundPluginProcessor::destroyCsoundGlobalVars()
{
    if(getCsound())
    {
        CabbagePersistentData** pd = (CabbagePersistentData**)getCsound()->QueryGlobalVariable("cabbageData");
        if (pd != nullptr)
            getCsound()->DestroyGlobalVariable("cabbageData");

        CabbageWidgetIdentifiers** wi = (CabbageWidgetIdentifiers**)getCsound()->QueryGlobalVariable("cabbageWidgetData");
        if (wi != nullptr)
            getCsound()->DestroyGlobalVariable("cabbageWidgetData");


        CabbageWidgetsValueTree** vt = (CabbageWidgetsValueTree**)getCsound()->QueryGlobalVariable("cabbageWidgetsValueTree");
        if (vt != nullptr) {
            getCsound()->DestroyGlobalVariable("cabbageWidgetsValueTree");
        }
    }
}

void CsoundPluginProcessor::createCsoundGlobalVars(ValueTree cabbageData)
{
    CabbagePersistentData** pd = (CabbagePersistentData**)getCsound()->QueryGlobalVariable("cabbageData");
    if (pd == NULL) {
        getCsound()->CreateGlobalVariable("cabbageData", sizeof(CabbagePersistentData*));
        pd = (CabbagePersistentData**)getCsound()->QueryGlobalVariable("cabbageData");
        *pd = new CabbagePersistentData();
        auto pdClass = *pd;
        pdClass->data = getInternalState().toStdString();
    }

    CabbageWidgetIdentifiers** wi = (CabbageWidgetIdentifiers**)getCsound()->QueryGlobalVariable("cabbageData");
    if (wi == NULL) {
        getCsound()->CreateGlobalVariable("cabbageWidgetData", sizeof(CabbageWidgetIdentifiers*));
    }

    CabbageWidgetsValueTree** vt = (CabbageWidgetsValueTree**)getCsound()->QueryGlobalVariable("cabbageWidgetsValueTree");
    if (vt == NULL) {
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
    
#ifdef CabbagePro
    csdLines.addLines(Encrypt::decode(csdFile));
    csdFileText = Encrypt::decode(csdFile);
    //DBG(csdFileText);
#else
    csdLines.addLines(csdFile.loadFileAsString());
    csdFileText = csdFile.loadFileAsString();
#endif
    
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

#if ! JucePlugin_IsSynth
    const int inputs = getTotalNumInputChannels() - numSideChainChannels;
    numCsoundInputChannels = inputs + numSideChainChannels;
    CabbageUtilities::debug("SetupAndCompile - Requested input channels:", numCsoundInputChannels);
#else
    //numCsoundOutputChannels = getBus(false, 0)->getNumberOfChannels();
    numCsoundOutputChannels = CabbageUtilities::getHeaderInfo(csdFileText, "nchnls");
    //numCsoundOutputChannels = getTotalNumOutputChannels();
#endif


    CabbageUtilities::debug("SetupAndCompile - Requested output channels:", numCsoundOutputChannels);
    //int test = csound->SetGlobalEnv("OPCODE6DIR64", );
    CabbageUtilities::debug("Env var set");
    //csoundSetOpcodedir("/Library/Frameworks/CsoundLib64.framework/Versions/6.0/Resources/Opcodes64");
    
    Logger::writeToLog(String::formatted("Resetting csound ...\ncsound = 0x%p", csound.get()));

	csound.reset (new Csound());
    
	csdFilePath = filePath;
	csdFilePath.setAsCurrentWorkingDirectory();
	csound->SetHostImplementedMIDIIO(true);
	csound->SetHostImplementedAudioIO(1, 0);
	csound->SetHostData(this);

    csnd::plugin<StrToFile>((csnd::Csound*) csound->GetCsound(), "strToFile.SSO", "i", "SSO", csnd::thread::i);
    csnd::plugin<FileToStr>((csnd::Csound*) csound->GetCsound(), "fileToStr.i", "S", "S", csnd::thread::i);

    csnd::plugin<ChannelStateSave>((csnd::Csound*) csound->GetCsound(), "cabbageChannelStateSave.i", "i", "S", csnd::thread::i);
    csnd::plugin<ChannelStateSave>((csnd::Csound*) csound->GetCsound(), "cabbageChannelStateSave.k", "k", "S", csnd::thread::k);

    csnd::plugin<ChannelStateRecall>((csnd::Csound*) csound->GetCsound(), "cabbageChannelStateRecall.i", "i", "S", csnd::thread::i);
    csnd::plugin<ChannelStateRecall>((csnd::Csound*) csound->GetCsound(), "cabbageChannelStateRecall.k", "k", "SO", csnd::thread::k);
    csnd::plugin<ChannelStateRecall>((csnd::Csound*) csound->GetCsound(), "cabbageChannelStateRecall.k", "k", "SS[]", csnd::thread::k);

    
    csnd::plugin<StrToArray>((csnd::Csound*) csound->GetCsound(), "strToArray.ii", "S[]", "SS", csnd::thread::i);
    csnd::plugin<StrRemove>((csnd::Csound*) csound->GetCsound(), "strRemove.ii", "S", "SSo", csnd::thread::i);

    csnd::plugin<WriteStateData>((csnd::Csound*) csound->GetCsound(), "cabbageWriteStateData.ss", "", "iS", csnd::thread::i);
    csnd::plugin<ReadStateData>((csnd::Csound*) csound->GetCsound(), "cabbageReadStateData.i", "S", "", csnd::thread::ik);

    csnd::plugin<GetStateFloatValue>((csnd::Csound*) csound->GetCsound(), "cabbageGetStateValue.s", "i", "S", csnd::thread::i);
    csnd::plugin<GetStateFloatValue>((csnd::Csound*) csound->GetCsound(), "cabbageGetStateValue.s", "k", "S", csnd::thread::ik);
    csnd::plugin<GetStateFloatValueArray>((csnd::Csound*) csound->GetCsound(), "cabbageGetStateValue.s", "k[]", "S", csnd::thread::ik);
    csnd::plugin<GetStateFloatValueArray>((csnd::Csound*) csound->GetCsound(), "cabbageGetStateValue.s", "i[]", "S", csnd::thread::i);
    csnd::plugin<GetStateStringValue>((csnd::Csound*) csound->GetCsound(), "cabbageGetStateValue.s", "S", "S", csnd::thread::i);
    csnd::plugin<GetStateStringValueArray>((csnd::Csound*) csound->GetCsound(), "cabbageGetStateValue.s", "S[]", "S", csnd::thread::ik);

    csnd::plugin<SetStateFloatData>((csnd::Csound*) csound->GetCsound(), "cabbageSetStateValue.s", "", "Sk", csnd::thread::ik);
    csnd::plugin<SetStateFloatData>((csnd::Csound*) csound->GetCsound(), "cabbageSetStateValue.s", "", "Si", csnd::thread::i);

    csnd::plugin<SetStateFloatArrayData>((csnd::Csound*) csound->GetCsound(), "cabbageSetStateValue.s", "", "Si[]", csnd::thread::i);
    csnd::plugin<SetStateFloatArrayData>((csnd::Csound*) csound->GetCsound(), "cabbageSetStateValue.s", "", "Sk[]", csnd::thread::ik);

    csnd::plugin<SetStateStringData>((csnd::Csound*) csound->GetCsound(), "cabbageSetStateValue.s", "", "SS", csnd::thread::i);
    csnd::plugin<SetStateStringData>((csnd::Csound*) csound->GetCsound(), "cabbageSetStateValue.s", "", "SS", csnd::thread::ik);

    csnd::plugin<SetStateStringArrayData>((csnd::Csound*) csound->GetCsound(), "cabbageSetStateValue.s", "", "SS[]", csnd::thread::i);
    csnd::plugin<SetStateStringArrayData>((csnd::Csound*) csound->GetCsound(), "cabbageSetStateValue.s", "", "SS[]", csnd::thread::ik);

    
   
    csnd::plugin<SetCabbageIdentifierITimeSArgs>((csnd::Csound*) csound->GetCsound(), "cabbageSet", "", "SW", csnd::thread::i);
    csnd::plugin<SetCabbageIdentifierITime>((csnd::Csound*) csound->GetCsound(), "cabbageSet", "", "SSN", csnd::thread::i);

    
    //csnd::plugin<SetCabbageIdentifierSArgs>((csnd::Csound*) csound->GetCsound(), "cabbageSet", "", "kSSW", csnd::thread::ik);
    csnd::plugin<SetCabbageIdentifierSArgs>((csnd::Csound*) csound->GetCsound(), "cabbageSet", "", "kSS", csnd::thread::k);
    csnd::plugin<SetCabbageIdentifier>((csnd::Csound*) csound->GetCsound(), "cabbageSet", "", "kSSM", csnd::thread::k);
    csnd::plugin<SetCabbageIdentifierSArgs>((csnd::Csound*) csound->GetCsound(), "cabbageSet", "", "kSW", csnd::thread::k);
    
    csnd::plugin<SetCabbageValueIdentifier>((csnd::Csound*) csound->GetCsound(), "cabbageSetValue", "", "SkP", csnd::thread::ik);
    
    csnd::plugin<GetCabbageValue>((csnd::Csound*) csound->GetCsound(), "cabbageGetValue", "k", "S", csnd::thread::ik);
    csnd::plugin<GetCabbageValue>((csnd::Csound*) csound->GetCsound(), "cabbageGetValue", "i", "S", csnd::thread::i);
    csnd::plugin<GetCabbageValueWithTrigger>((csnd::Csound*) csound->GetCsound(), "cabbageGetValue", "kk", "S", csnd::thread::ik);
    
    csnd::plugin<GetCabbageStringValue>((csnd::Csound*) csound->GetCsound(), "cabbageGetValue", "S", "S", csnd::thread::ik);
    csnd::plugin<GetCabbageStringValueWithTrigger>((csnd::Csound*) csound->GetCsound(), "cabbageGetValue", "Sk", "S", csnd::thread::ik);

    csnd::plugin<GetCabbageIdentifierArray>((csnd::Csound*) csound->GetCsound(), "cabbageGet", "k[]", "SS", csnd::thread::k);
    csnd::plugin<GetCabbageIdentifierArray>((csnd::Csound*) csound->GetCsound(), "cabbageGet", "i[]", "SS", csnd::thread::i);
    csnd::plugin<GetCabbageStringIdentifierArray>((csnd::Csound*) csound->GetCsound(), "cabbageGet", "S[]", "SS", csnd::thread::ik);
    csnd::plugin<GetCabbageIdentifierSingle>((csnd::Csound*) csound->GetCsound(), "cabbageGet", "k", "SS", csnd::thread::ik);
    csnd::plugin<GetCabbageStringIdentifierSingle>((csnd::Csound*) csound->GetCsound(), "cabbageGet", "S", "SS", csnd::thread::ik);

    csnd::plugin<GetCabbageReservedChannelStringWithTrigger>((csnd::Csound*) csound->GetCsound(), "cabbageGet", "Sk", "S", csnd::thread::ik);
    csnd::plugin<GetCabbageReservedChannelString>((csnd::Csound*) csound->GetCsound(), "cabbageGet", "S", "S", csnd::thread::ik);
    
    csnd::plugin<GetCabbageReservedChannelDataWithTrigger>((csnd::Csound*) csound->GetCsound(), "cabbageGet", "kk", "S", csnd::thread::ik);
    csnd::plugin<GetCabbageReservedChannelData>((csnd::Csound*) csound->GetCsound(), "cabbageGet", "k", "S", csnd::thread::ik);

    csnd::plugin<CreateCabbageWidget>((csnd::Csound*) csound->GetCsound(), "cabbageCreate", "", "SS", csnd::thread::i);

    csnd::plugin<CabbageCopyFile>((csnd::Csound*) csound->GetCsound(), "cabbageCopyFile", "", "SW", csnd::thread::i);
    csnd::plugin<CabbageFindFilesI>((csnd::Csound*) csound->GetCsound(), "cabbageFindFiles", "S[]", "SW", csnd::thread::i);
    csnd::plugin<CabbageFindFilesK>((csnd::Csound*) csound->GetCsound(), "cabbageFindFiles", "S[]", "kSW", csnd::thread::ik);
    csnd::plugin<CabbageGetFilename>((csnd::Csound*) csound->GetCsound(), "cabbageGetFilename", "S", "S", csnd::thread::ik);
    csnd::plugin<CabbageGetFilePath>((csnd::Csound*) csound->GetCsound(), "cabbageGetFilePath", "S", "S", csnd::thread::ik);
    csnd::plugin<CabbageGetFileExtension>((csnd::Csound*) csound->GetCsound(), "cabbageGetFileExtension", "S", "S", csnd::thread::ik);
    csnd::plugin<CabbageGetFileNoExtension>((csnd::Csound*) csound->GetCsound(), "cabbageGetFileNoExtension", "S", "S", csnd::thread::ik);

    csnd::plugin<CabbageGetWidgetChannels>((csnd::Csound*)csound->GetCsound(), "cabbageGetWidgetChannels", "S[]", "W", csnd::thread::i);

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
    
#ifdef CabbagePro
    compileCsdString(csdFileText);
    //DBG(csdFileText);
    csound->Start();
#else
    if (csdFileText.contains("<Csound") || csdFileText.contains("</Csound"))
    {
        compileCsdFile(csdFile);
    }
#endif


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

   createCsoundGlobalVars(cabbageData);
    
    
    if (CabbageUtilities::getTargetPlatform() == CabbageUtilities::TargetPlatformTypes::Win32)
    {
        csound->SetChannel ("CSD_PATH", csdFilePath.getParentDirectory().getFullPathName().replace ("\\", "\\\\").toUTF8().getAddress());
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
		csound->SetStringChannel("USER_APPLICATION_DIRECTORY", File::getSpecialLocation(File::userApplicationDataDirectory).getFullPathName().toUTF8().getAddress());
		csound->SetStringChannel("USER_DOCUMENTS_DIRECTORY", File::getSpecialLocation(File::userDocumentsDirectory).getFullPathName().toUTF8().getAddress());
    }

    csound->SetStringChannel ("LAST_FILE_DROPPED", const_cast<char*> (""));

    csound->SetChannel ("IS_EDITOR_OPEN", 0.0);
    csdFilePath.setAsCurrentWorkingDirectory();


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

#if !defined(Cabbage_IDE_Build)
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

    //csound->Message("Running single k-cycle...\n");
    csound->PerformKsmps();
    //csound->Message("Rewinding...\n");

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

    const int inputs = getTotalNumInputChannels();
    const int outputs = getTotalNumOutputChannels();

#if ! JucePlugin_IsSynth
    if(getBusesLayout().getMainOutputChannelSet() == AudioChannelSet::mono())
        hostRequestedMono = true;
    CabbageUtilities::debug("CsoundPluginProcessor::prepareToPlay - inputBuses:", getBusCount(true));
    CabbageUtilities::debug("CsoundPluginProcessor::prepareToPlay - inputs:", inputs);
    CabbageUtilities::debug("CsoundPluginProcessor::prepareToPlay - Requested input channels:", numCsoundInputChannels);
#endif
    //const int outputs = getBus(false, 0)->getNumberOfChannels();
    CabbageUtilities::debug("CsoundPluginProcessor::prepareToPlay - outputBuses:", getBusCount(false));
    CabbageUtilities::debug("CsoundPluginProcessor::prepareToPlay - outputs:", outputs);
    CabbageUtilities::debug("CsoundPluginProcessor::prepareToPlay - Requested output channels:", numCsoundOutputChannels);

    if((samplingRate != sampleRate)
#if ! JucePlugin_IsSynth && ! JucePlugin_IsSynth
       || numCsoundInputChannels != inputs
#endif
       || numCsoundOutputChannels != outputs)
    {
        //if sampling rate is other than default or has been changed, recompile..
        samplingRate = sampleRate;
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
void CsoundPluginProcessor::processCsoundIOBuffers(int bufferType, Type* buffer, int samplePosition, int csndPosition)
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
    
    Type** sideChainBuffer = nullptr;

	if (supportsSidechain)
	{
		sideChainBuffer = getBusBuffer(buffer, true, getBusCount(true)-1).getArrayOfWritePointers();
		numSideChainChannels = getBusBuffer(buffer, true, getBusCount(true) - 1).getNumChannels();
	}

    Type** ioBuffer = buffer.getArrayOfWritePointers();
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
    
    if(isLMMS == true)
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

//        for (auto busNr = 0; busNr < getBusCount(false); ++busNr)
//        {
//            auto busBuffer = getBusBuffer(buffer, false, busNr);
//
//            for (int channel = 0; channel < busBuffer.getNumChannels(); channel++)
//            {
//                busBuffer.clear(channel, 0, busBuffer.getNumSamples());
//            }
//        }


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
                    processCsoundIOBuffers(BufferType::input, inputBuffer[channel], i, pos++);
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
                    processCsoundIOBuffers(BufferType::output, outputBuffer[channel], i, pos++);
                }
            }
#else
            const int channelNum = buffer.getNumChannels();
            pos = csndIndex * channelNum;
            for (int channel = 0; channel < outputChannelCount; channel++)
            {
                processCsoundIOBuffers(BufferType::output, ioBuffer[channel], i, pos);
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





