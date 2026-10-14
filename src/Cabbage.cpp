/*
 * Copyright (c) 2024 Rory Walsh
 *
 * This file is part of Cabbage3
 *
 * Cabbage3 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Cabbage3 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Cabbage3.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "Cabbage.h"

#ifdef CabbagePro
#include "encrypt.h"
#endif
#include "CabbageProcessor.h"
#include "opcodes/CabbageARAOpcodes.h"

#include <choc/text/choc_StringUtilities.h>
#include <algorithm>

namespace cabbage
{

Engine::Engine(CabbageProcessor &p)
    : processor(p)
{
};

Engine::~Engine()
{
    shuttingDown.store(true, std::memory_order_release);

    {
        std::lock_guard<std::mutex> lock(channelCacheMutex);
        channelCache.clear();
        dirtyValueChannels.clear();
        dirtyIdentifierChannels.clear();
    }

    if (csound)
    {
        csCompileResult = false;
        csound.reset();
    }
}

void Engine::teardownCsound()
{
    if (csound)
    {
        csCompileResult = -1;
        csSpin = nullptr;
        csdKsmps = 0;
        csScale = 0.0;
        csound.reset();
    }
}

void Engine::addOpcodes()
{
    // The order in which these are registered is important!

    // Plugin structs must be registered before any struct-typed opcode entry
    // and before Compile() resolves orchestra types. When unavailable (a
    // Csound older than csound PR #3356) the struct overloads are skipped and
    // only the legacy multi-output forms remain.
    CSOUND *cs = csound->GetCsound();
    lattice::logDebug << "Cabbage: RegisterStruct API present: "
                     << (cs != nullptr && cs->RegisterStruct != nullptr ? "yes" : "no");
    const CabbageTrigStructStatus trigStructs = registerCabbageTrigStructs(cs);
    lattice::logDebug << "Cabbage: RegisterStruct CabbageNumTrig: "
                     << (trigStructs.numOk ? "ok" : "FAILED")
                     << ", CabbageStrTrig: " << (trigStructs.strOk ? "ok" : "FAILED");
    if (!trigStructs.numOk || !trigStructs.strOk)
    {
        lattice::logError << "Cabbage: struct trigger outputs partially/fully disabled - "
                            "legacy multi-output forms remain";
        if (cs != nullptr)
            cs->Message(cs, "Cabbage: RegisterStruct unavailable - struct trigger outputs disabled\n");
    }

    csnd::plugin<CabbageSetValue>((csnd::Csound *)csound->GetCsound(), "cabbageSetValue", "", "Si", csnd::thread::i);
    csnd::plugin<CabbageSetValue>((csnd::Csound *)csound->GetCsound(), "cabbageSetValue", "", "SkP", csnd::thread::k);

    csnd::plugin<CabbageSetInitString>((csnd::Csound *)getCsound()->GetCsound(), "cabbageSet", "", "SW", csnd::thread::i);
    csnd::plugin<CabbageSetPerfString>((csnd::Csound *)getCsound()->GetCsound(), "cabbageSet", "", "kSSW", csnd::thread::k);
    
    csnd::plugin<CabbageSetInitMYFLT>((csnd::Csound *)getCsound()->GetCsound(), "cabbageSet", "", "SSM", csnd::thread::i);
    csnd::plugin<CabbageSetPerfMYFLT>((csnd::Csound *)getCsound()->GetCsound(), "cabbageSet", "", "kSSM", csnd::thread::k);
    

    csnd::plugin<CabbageSetInitMYFLTArray>((csnd::Csound *)getCsound()->GetCsound(), "cabbageSet", "", "SSi[]", csnd::thread::i);
    csnd::plugin<CabbageSetPerfMYFLTArray>((csnd::Csound *)getCsound()->GetCsound(), "cabbageSet", "", "kSSk[]", csnd::thread::k);
    csnd::plugin<CabbageSetInitStringArray>((csnd::Csound *)getCsound()->GetCsound(), "cabbageSet", "", "SSS[]", csnd::thread::i);

    csnd::plugin<CabbageGetValue>((csnd::Csound *)getCsound()->GetCsound(), "cabbageGetValue", "i", "S", csnd::thread::i);
    csnd::plugin<CabbageGetValue>((csnd::Csound *)getCsound()->GetCsound(), "cabbageGetValue", "k", "S", csnd::thread::ik);
    csnd::plugin<CabbageGetValueString>((csnd::Csound *)getCsound()->GetCsound(), "cabbageGetValue", "S", "S", csnd::thread::i);
    csnd::plugin<CabbageGetValueWithTrigger>((csnd::Csound *)getCsound()->GetCsound(), "cabbageGetValue", "kk", "S", csnd::thread::ik);
    csnd::plugin<CabbageGetValueStringWithTrigger>((csnd::Csound *)getCsound()->GetCsound(), "cabbageGetValue", "Sk", "S", csnd::thread::k);
    if (trigStructs.numOk)
    {
        const int32_t rc = csnd::plugin<CabbageGetValueStruct>(
            (csnd::Csound *)getCsound()->GetCsound(), "cabbageGetValue", ":CabbageNumTrig;", "S", csnd::thread::ik);
        lattice::logDebug << "Cabbage: cabbageGetValue :CabbageNumTrig; registration rc=" << rc;
    }
    if (trigStructs.strOk)
    {
        const int32_t rc = csnd::plugin<CabbageGetValueStringStruct>(
            (csnd::Csound *)getCsound()->GetCsound(), "cabbageGetValue", ":CabbageStrTrig;", "S", csnd::thread::k);
        lattice::logDebug << "Cabbage: cabbageGetValue :CabbageStrTrig; registration rc=" << rc;
    }

    csnd::plugin<CabbageGetStringArray>((csnd::Csound *)getCsound()->GetCsound(), "cabbageGet", "S[]", "SS", csnd::thread::i);
    csnd::plugin<CabbageGetMYFLT>((csnd::Csound *)getCsound()->GetCsound(), "cabbageGet", "k", "SW", csnd::thread::ik);
    csnd::plugin<CabbageGetMYFLT>((csnd::Csound *)getCsound()->GetCsound(), "cabbageGet", "i", "SS", csnd::thread::i);
    csnd::plugin<CabbageGetString>((csnd::Csound *)getCsound()->GetCsound(), "cabbageGet", "S", "SS", csnd::thread::i);
    
    csnd::plugin<CabbageGetStringWithTrigger>((csnd::Csound *)getCsound()->GetCsound(), "cabbageGet", "Sk", "SS", csnd::thread::k);
    if (trigStructs.strOk)
    {
        const int32_t rc = csnd::plugin<CabbageGetStringStruct>(
            (csnd::Csound *)getCsound()->GetCsound(), "cabbageGet", ":CabbageStrTrig;", "SS", csnd::thread::k);
        lattice::logDebug << "Cabbage: cabbageGet :CabbageStrTrig; registration rc=" << rc;
    }

    csnd::plugin<CabbageWidgetHasKey>((csnd::Csound *)getCsound()->GetCsound(), "cabbageHasKey", "i", "SS", csnd::thread::i);
    csnd::plugin<CabbageWidgetHasKey>((csnd::Csound *)getCsound()->GetCsound(), "cabbageHasKey", "k", "SW", csnd::thread::ik);

    csnd::plugin<CabbageJsonGetString>((csnd::Csound *)getCsound()->GetCsound(), "cabbageJsonGet", "S", "SS", csnd::thread::i);
    csnd::plugin<CabbageJsonGetString>((csnd::Csound *)getCsound()->GetCsound(), "cabbageJsonGet", "S", "SS", csnd::thread::k);
    csnd::plugin<CabbageJsonGetStringWithTrigger>((csnd::Csound *)getCsound()->GetCsound(), "cabbageJsonGet", "Sk", "SS", csnd::thread::k);
    if (trigStructs.strOk)
    {
        const int32_t rc = csnd::plugin<CabbageJsonGetStringStruct>(
            (csnd::Csound *)getCsound()->GetCsound(), "cabbageJsonGet", ":CabbageStrTrig;", "SS", csnd::thread::k);
        lattice::logDebug << "Cabbage: cabbageJsonGet :CabbageStrTrig; registration rc=" << rc;
    }
    csnd::plugin<CabbageJsonGetNumber>((csnd::Csound *)getCsound()->GetCsound(), "cabbageJsonGet", "i", "SS", csnd::thread::i);
    csnd::plugin<CabbageJsonGetNumber>((csnd::Csound *)getCsound()->GetCsound(), "cabbageJsonGet", "k", "SS", csnd::thread::ik);
    csnd::plugin<CabbageJsonGetStringArray>((csnd::Csound *)getCsound()->GetCsound(), "cabbageJsonGet", "S[]", "SS", csnd::thread::i);
    csnd::plugin<CabbageJsonGetNumberArray>((csnd::Csound *)getCsound()->GetCsound(), "cabbageJsonGet", "k[]", "SS", csnd::thread::i);
    csnd::plugin<CabbageJsonHas>((csnd::Csound *)getCsound()->GetCsound(), "cabbageJsonHas", "i", "SS", csnd::thread::i);
    csnd::plugin<CabbageJsonHas>((csnd::Csound *)getCsound()->GetCsound(), "cabbageJsonHas", "k", "SS", csnd::thread::ik);
    csnd::plugin<CabbageJsonLen>((csnd::Csound *)getCsound()->GetCsound(), "cabbageJsonLen", "i", "SS", csnd::thread::i);
    csnd::plugin<CabbageJsonLen>((csnd::Csound *)getCsound()->GetCsound(), "cabbageJsonLen", "k", "SS", csnd::thread::ik);
    csnd::plugin<CabbageJsonType>((csnd::Csound *)getCsound()->GetCsound(), "cabbageJsonType", "S", "SS", csnd::thread::i);
    csnd::plugin<CabbageJsonType>((csnd::Csound *)getCsound()->GetCsound(), "cabbageJsonType", "S", "SS", csnd::thread::k);
    csnd::plugin<CabbageJsonSetString>((csnd::Csound *)getCsound()->GetCsound(), "cabbageJsonSet", "S", "SSS", csnd::thread::i);
    csnd::plugin<CabbageJsonSetString>((csnd::Csound *)getCsound()->GetCsound(), "cabbageJsonSet", "S", "SSS", csnd::thread::k);
    csnd::plugin<CabbageJsonSetNumber>((csnd::Csound *)getCsound()->GetCsound(), "cabbageJsonSet", "S", "SSM", csnd::thread::i);
    csnd::plugin<CabbageJsonSetNumber>((csnd::Csound *)getCsound()->GetCsound(), "cabbageJsonSet", "S", "SSM", csnd::thread::k);

    csnd::plugin<CabbageCreate>((csnd::Csound *)getCsound()->GetCsound(), "cabbageCreate", "", "S", csnd::thread::i);
    csnd::plugin<CabbageDump>((csnd::Csound *)getCsound()->GetCsound(), "cabbageDump", "", "So", csnd::thread::i);
    csnd::plugin<CabbageDumpWithTrigger>((csnd::Csound *)getCsound()->GetCsound(), "cabbageDump", "", "kSo", csnd::thread::ik);
    
    csnd::plugin<CabbageSaveState>((csnd::Csound *)getCsound()->GetCsound(), "cabbageSaveState", "", "S", csnd::thread::i);
    csnd::plugin<CabbageSaveStateSelected>((csnd::Csound *)getCsound()->GetCsound(), "cabbageSaveState", "", "SS[]", csnd::thread::i);
    csnd::plugin<CabbageLoadState>((csnd::Csound *)getCsound()->GetCsound(), "cabbageLoadState", "", "S", csnd::thread::i);

    csnd::plugin<CabbageSendMessage>((csnd::Csound *)getCsound()->GetCsound(), "cabbageSendMessage", "", "S", csnd::thread::i);
    csnd::plugin<CabbageSendMessage>((csnd::Csound *)getCsound()->GetCsound(), "cabbageSendMessage", "", "kS", csnd::thread::k);
    

    csnd::plugin<CabbageGetFiles>((csnd::Csound *)getCsound()->GetCsound(), "cabbageGetFiles", "S[]", "SS", csnd::thread::i);
    csnd::plugin<CabbageCreateFileName>((csnd::Csound *)getCsound()->GetCsound(), "cabbageCreateFileName", "S", "SS", csnd::thread::i);
    csnd::plugin<CabbageJoinPath>((csnd::Csound *)getCsound()->GetCsound(), "cabbageJoinPath", "S", "SW", csnd::thread::i);

    csnd::plugin<CabbageGetWidgetIds>((csnd::Csound *)getCsound()->GetCsound(), "cabbageGetWidgetIds", "S[]", "", csnd::thread::i);
    csnd::plugin<CabbageGetWidgetIdsFiltered>((csnd::Csound *)getCsound()->GetCsound(), "cabbageGetWidgetIds", "S[]", "SS", csnd::thread::i);
    csnd::plugin<CabbageGetWidget>((csnd::Csound *)getCsound()->GetCsound(), "cabbageGetWidget", "S", "S", csnd::thread::i);
    // Deprecated alias: cabbageGetWidgets returned ids only; use cabbageGetWidgetIds.
    csnd::plugin<CabbageGetWidgetIds>((csnd::Csound *)getCsound()->GetCsound(), "cabbageGetWidgets", "S[]", "", csnd::thread::i);

#if LATTICE_HAS_ARA || defined(CabbageApp)
    csnd::plugin<CabbageAraGetNum>((csnd::Csound *)getCsound()->GetCsound(), "cabbageAraGet", "i", "S", csnd::thread::i);
    csnd::plugin<CabbageAraGetNum>((csnd::Csound *)getCsound()->GetCsound(), "cabbageAraGet", "i", "Si", csnd::thread::i);
    csnd::plugin<CabbageAraGetNum>((csnd::Csound *)getCsound()->GetCsound(), "cabbageAraGet", "k", "S", csnd::thread::k);
    csnd::plugin<CabbageAraGetNum>((csnd::Csound *)getCsound()->GetCsound(), "cabbageAraGet", "k", "Sk", csnd::thread::k);
    csnd::plugin<CabbageAraGetString>((csnd::Csound *)getCsound()->GetCsound(), "cabbageAraGet", "S", "S", csnd::thread::i);
    csnd::plugin<CabbageAraGetString>((csnd::Csound *)getCsound()->GetCsound(), "cabbageAraGet", "S", "Si", csnd::thread::i);
    csnd::plugin<CabbageAraGetUpdate>((csnd::Csound *)getCsound()->GetCsound(), "cabbageAraGetUpdate", "k", "", csnd::thread::ik);

    csnd::plugin<CabbageAraGetUpdateEvent>((csnd::Csound *)getCsound()->GetCsound(), "cabbageAraGetUpdateEvent", "Sk", "", csnd::thread::ik);
    if (trigStructs.strOk)
    {
        const int32_t rc = csnd::plugin<CabbageAraGetUpdateEventStruct>(
            (csnd::Csound *)getCsound()->GetCsound(), "cabbageAraGetUpdateEvent", ":CabbageStrTrig;", "", csnd::thread::ik);
        lattice::logDebug << "Cabbage: cabbageAraGetUpdateEvent :CabbageStrTrig; registration rc=" << rc;
    }
    csnd::plugin<CabbageAraGetSourceSamplesAudio>((csnd::Csound *)getCsound()->GetCsound(), "cabbageAraGetSourceSamples", "a", "aii", csnd::thread::ia);
    csnd::plugin<CabbageAraGetSourceSamplesK>((csnd::Csound *)getCsound()->GetCsound(), "cabbageAraGetSourceSamples", "k", "kii", csnd::thread::k);
    csnd::plugin<CabbageAraGetSourceSamplesArray>((csnd::Csound *)getCsound()->GetCsound(), "cabbageAraGetSourceSamples", "k[]", "iiii", csnd::thread::i);
    csnd::plugin<CabbageAraGetSourceSamplesIArray>((csnd::Csound *)getCsound()->GetCsound(), "cabbageAraGetSourceSamples", "i[]", "iiii", csnd::thread::i);
    csnd::plugin<CabbageAraGetStateJson>((csnd::Csound *)getCsound()->GetCsound(), "cabbageAraGetStateJson", "S", "ko", csnd::thread::k);
    csnd::plugin<CabbageAraDump>((csnd::Csound *)getCsound()->GetCsound(), "cabbageAraDump", "", "", csnd::thread::i);
    csnd::plugin<CabbageAraDump>((csnd::Csound *)getCsound()->GetCsound(), "cabbageAraDump", "", "k", csnd::thread::k);
#endif
}

//========================================================================================
// Determine the number of input and output channels from channelConfig or CSD file.
// Uses the first entry in the channelConfig array; falls back to CSD nchnls declarations.
//========================================================================================
std::pair<int, int> Engine::determineChannelConfiguration(const std::string& csdFile)
{
    // On re-init the processor's configs vector is already populated and
    // activeConfigIndex reflects the host's selectAudioPortsConfig() call.
    // Use those counts directly so the correct Csound channel count is set.
    const auto& cfg = processor.getChannelConfig();
    if (!cfg.isEmpty())
        return {cfg.getTotalNumInputChannels(), cfg.getTotalNumOutputChannels()};

    // First init: configs not yet registered — read the first entry from the JSON array.
    if (auto json = cabbage::File::parseCabbageSection(csdFile))
    {
        if (json->contains("channelConfig") && (*json)["channelConfig"].is_array())
        {
            const auto& cfgArray = (*json)["channelConfig"];
            if (!cfgArray.empty())
            {
                const auto& first = cfgArray[0];
                const std::string ins  = first.value("ins",  "2");
                const std::string outs = first.value("outs", "2");

                // Sum '+'-separated bus counts (e.g. "2+1" → 3)
                auto sumBuses = [](const std::string& part) -> int {
                    int total = 0;
                    std::istringstream ss(part);
                    std::string token;
                    while (std::getline(ss, token, '+'))
                        if (!token.empty()) total += std::stoi(token);
                    return total;
                };

                return {std::max(1, sumBuses(ins)), std::max(1, sumBuses(outs))};
            }
        }
    }

    // No channelConfig in JSON — fall back to nchnls/nchnls_i declared in the CSD orchestra.
    const int ins  = cabbage::File::getNumberOfInputChannels(csdFile);
    const int outs = cabbage::File::getNumberOfOutputChannels(csdFile);
    if (ins > 0 && outs > 0)
        return {ins, outs};

    // Ultimate fallback — default to 1 in / 2 out (safer than stereo in/out).
    // nchnls from the CSD is never used to determine routing.
    return {1, 2};
}

bool Engine::setupCsound()
{
    const std::string csdFile = cabbage::File::getCsdFileAndPath();
    lattice::logDebug << csdFile;
    csound = std::make_unique<Csound>();
    csound->SetHostMIDIIO();
    csound->SetHostAudioIO();
    csound->SetHostData(this);

    lattice::logDebug << "Csound SetHostData: enginePtr=" << this << " processorPtr=" << &processor;

    // Message buffer must exist before addOpcodes(): the RegisterStruct
    // fallback path reports via csound->Message(), which is lost otherwise.
    csound->CreateMessageBuffer(0);

    addOpcodes();
    csound->SetExternalMidiInOpenCallback(CabbageProcessor::OpenMidiInputDevice);
    csound->SetExternalMidiReadCallback(CabbageProcessor::ReadMidiData);
    csound->SetExternalMidiOutOpenCallback(CabbageProcessor::OpenMidiOutputDevice);
    csound->SetExternalMidiWriteCallback(CabbageProcessor::WriteMidiData);

    csound->SetOption((char *)"-n");
    csound->SetOption((char *)"-d");
    csound->SetOption((char *)"-b0");
    csound->SetOption(std::string("--sample-rate=" + std::to_string(processor.getSampleRate())).c_str());

    // Determine channel configuration from channelConfig property or CSD file
    auto [numInputs, numOutputs] = determineChannelConfiguration(csdFile);

    csound->SetOption(std::string("--nchnls=" + std::to_string(numOutputs)).c_str());
    csound->SetOption(std::string("--nchnls_i=" + std::to_string(numInputs)).c_str());

    // csdFile should already be set by CabbageProcessor constructor - single source of truth
    if (csdFile.empty())
    {
        compileErrors = "No CSD file path was provided to this plugin instance.";
        lattice::logError << "setupCsound: csdFile is empty! Should be set by CabbageProcessor constructor.";
        return false;
    }

    bool exists = std::filesystem::exists(csdFile);
    if (exists)
    {
        // Pre-Compile probe: confirms plugin struct types survived in the
        // type pool up to the point of orchestra parsing.
        {
            CSOUND *raw = csound->GetCsound();
            const bool numVisible = cabbageFindStructType(raw, "CabbageNumTrig") != nullptr;
            const bool strVisible = cabbageFindStructType(raw, "CabbageStrTrig") != nullptr;
            lattice::logDebug << "Cabbage: pre-Compile type pool: CabbageNumTrig "
                             << (numVisible ? "visible" : "MISSING") << ", CabbageStrTrig "
                             << (strVisible ? "visible" : "MISSING");
        }
        // Check for compile time errors
#ifdef CabbagePro
        // Pro version: Check if file is encrypted
        if (Decrypt::isEncrypted(csdFile))
        {
            try {
                std::string decryptedCsd = Decrypt::getCsdText(csdFile);
                csCompileResult = csound->CompileCSD(decryptedCsd.c_str(), 1);
            }
            catch (const std::exception& e) {
                lattice::logError << "Failed to decrypt CSD file: " << e.what();
                csCompileResult = -1;
            }
        }
        else
        {
            csCompileResult = csound->Compile(csdFile.c_str());
        }
#else
        csCompileResult = csound->Compile(csdFile.c_str());
#endif
        setReservedChannels();
        // No check for i-time errors and instr0 issues
        if (csound->Start() == CSOUND_SUCCESS && csdCompiledWithoutError())
        {
            csdKsmps = csound->GetKsmps();
            csSpin = csound->GetSpin();
            csScale = csound->Get0dBFS();
            // NOTE: keep log statements single-line. The VS Code frontend
            // splits backend stdout on newlines and only recognises the
            // level prefix on the first line, so embedded '\n' would leak
            // continuation lines into non-verbose output.
            lattice::logDebug << "Resetting csound ...";
            lattice::logDebug << "csound = " << csound.get();
        }
        else
        {
            // Csound could not compile your file?
            while (csound->GetMessageCnt() > 0)
            {
                compileErrors += csound->GetFirstMessage();
                csound->PopFirstMessage();
            }
            return false;
        }

        widgets.clear();
        std::string jsonError;
        widgets = cabbage::Parser::parseCsdForWidgets(csdFile, &jsonError);
        csoundOutputEnabled = hasCsoundOutputWidget();
        lattice::logDebug << "hasCsoundOutputWidget: " << (csoundOutputEnabled ? "true" : "false");

        // If there was a JSON parse error, add it to compileErrors and return false
        if (!jsonError.empty())
        {
            compileErrors += "\n" + jsonError;
            return false;
        }

        // Initialise genTable widgets that have file properties
        initialiseGenTableWidgets();

        // Queue table data updates for genTable widgets with tableNumber > 0
        // The queued messages will be held until allowDequeuing is set to true
        // (which happens when webview sends cabbageIsReadyToLoad)
        queueGenTableUpdates();

        return true;
    }
    else
    {
        compileErrors = "CSD file not found: " + csdFile
                        + "\n\nPlease check that the file exists and that the plugin is pointing "
                          "to the correct location.";
        lattice::logError << "setupCsound: " << compileErrors;
        return false;
    }
}

//===========================================================================================
void Engine::initParameter(nlohmann::json &w)
{
    // Ensure default ranges are set if missing
    cabbage::Parser::assignDefaultRangesToChannels(w);

    // Handle new schema: channels array
    if (w.contains("channels") && w["channels"].is_array())
    {
        for (const auto &ch : w["channels"])
        {
            if (!ch.contains("id") || !ch["id"].is_string())
                continue;
            std::string channel = cabbage::Parser::removeQuotes(ch["id"].get<std::string>());

            if (ch.contains("range"))
            {
                auto &channelRange = ch["range"];
                float defaultValue =
                    channelRange.contains("value")
                        ? channelRange["value"].get<float>()
                        : (channelRange.contains("defaultValue") ? channelRange["defaultValue"].get<float>() : 0.5f);

                parameterChannels.push_back({channel, defaultValue});
                csound->SetControlChannel(channel.c_str(), defaultValue);
                numberOfParameters++;
            }
        }
    }
    // Handle old schema: multi-channel widgets (e.g., xyPad with x and y)
    else if (w.contains("channel") && w["channel"].is_object())
    {
        lattice::logError << "Old schema is no longer supported";
    }
    // Handle old schema: single-channel widgets
    else if (w.contains("channel") && w["channel"].is_string())
    {
        std::string channel = cabbage::Parser::removeQuotes(w["channel"].get<std::string>());
        float defaultValue = 0.5f;

        if (w.contains("range"))
        {
            defaultValue = w["range"]["defaultValue"].get<float>();
        }
        else if (w.contains("defaultValue"))
        {
            defaultValue = w["defaultValue"].get<float>();
        }

        parameterChannels.push_back({channel, defaultValue});
        csound->SetControlChannel(channel.c_str(), defaultValue);
        numberOfParameters++;
    }
}

//===========================================================================================
void Engine::setReservedChannels()
{
    auto path = cabbage::File::getParentDirectory(cabbage::File::getCsdFileAndPath());
    csound->SetStringChannel("CSD_PATH", (char *)path.c_str());


    csound->SetControlChannel("ARA_UPDATE", 0.0f);
#ifndef CabbageApp
    csound->SetControlChannel("IS_A_PLUGIN", 1.0f);
#else
    csound->SetControlChannel("IS_A_PLUGIN", 0.0f);
#endif

#ifdef LATTICE_HAS_ARA
    csound->SetControlChannel("ARA_PLUGIN", 1.0f);
#endif
    // Set all reserved directory channels using lattice::File::getSpecialLocation
    // All paths are converted to generic string format, i.e, forward slashes, for cross-platform compatibility
    csound->SetStringChannel("USER_HOME_DIRECTORY",
        (char *)std::filesystem::path(lattice::File::getSpecialLocation("USER_HOME_DIRECTORY")).generic_string().c_str());

    csound->SetStringChannel("USER_DOCUMENTS_DIRECTORY",
        (char *)std::filesystem::path(lattice::File::getSpecialLocation("USER_DOCUMENTS_DIRECTORY")).generic_string().c_str());

    csound->SetStringChannel("USER_DESKTOP_DIRECTORY",
        (char *)std::filesystem::path(lattice::File::getSpecialLocation("USER_DESKTOP_DIRECTORY")).generic_string().c_str());

    csound->SetStringChannel("USER_MUSIC_DIRECTORY",
        (char *)std::filesystem::path(lattice::File::getSpecialLocation("USER_MUSIC_DIRECTORY")).generic_string().c_str());

    csound->SetStringChannel("USER_MOVIES_DIRECTORY",
        (char *)std::filesystem::path(lattice::File::getSpecialLocation("USER_MOVIES_DIRECTORY")).generic_string().c_str());

    csound->SetStringChannel("USER_PICTURES_DIRECTORY",
        (char *)std::filesystem::path(lattice::File::getSpecialLocation("USER_PICTURES_DIRECTORY")).generic_string().c_str());

    csound->SetStringChannel("USER_APPLICATION_DATA_DIRECTORY",
        (char *)std::filesystem::path(lattice::File::getSpecialLocation("USER_APPLICATION_DATA_DIRECTORY")).generic_string().c_str());

    csound->SetStringChannel("COMMON_APPLICATION_DATA_DIRECTORY",
        (char *)std::filesystem::path(lattice::File::getSpecialLocation("COMMON_APPLICATION_DATA_DIRECTORY")).generic_string().c_str());

    csound->SetStringChannel("COMMON_DOCUMENTS_DIRECTORY",
        (char *)std::filesystem::path(lattice::File::getSpecialLocation("COMMON_DOCUMENTS_DIRECTORY")).generic_string().c_str());

    csound->SetStringChannel("WINDOWS_SYSTEM_DIRECTORY",
        (char *)std::filesystem::path(lattice::File::getSpecialLocation("WINDOWS_SYSTEM_DIRECTORY")).generic_string().c_str());

    csound->SetStringChannel("GLOBAL_APPLICATIONS_DIRECTORY",
        (char *)std::filesystem::path(lattice::File::getSpecialLocation("GLOBAL_APPLICATIONS_DIRECTORY")).generic_string().c_str());
}

//===========================================================================================
std::string Engine::createGlobalStruct()
{
    // Struct member declarations — order must match the init values below
    std::string structDef = "struct CabbageStruct ";
    structDef += "csdPath:S, araUpdate:k, isAPlugin:i, araPlugin:i";
    structDef += ", userHomeDirectory:S, userDocumentsDirectory:S, userDesktopDirectory:S";
    structDef += ", userMusicDirectory:S, userMoviesDirectory:S, userPicturesDirectory:S";
    structDef += ", userApplicationDataDirectory:S, commonApplicationDataDirectory:S";
    structDef += ", commonDocumentsDirectory:S, windowsSystemDirectory:S, globalApplicationsDirectory:S";
    structDef += "\n";

    auto getDir = [](const std::string &key) {
        return std::filesystem::path(lattice::File::getSpecialLocation(key)).generic_string();
    };

    auto csdPath = std::filesystem::path(cabbage::File::getParentDirectory(cabbage::File::getCsdFileAndPath())).generic_string();

#ifndef CabbageApp
    const int isAPlugin = 1;
#else
    const int isAPlugin = 0;
#endif

    std::string initLine = "cabbage@global:CabbageStruct init ";
    initLine += "\"" + csdPath + "\", ";
    initLine += "0, ";
    initLine += std::to_string(isAPlugin);
#ifdef LATTICE_HAS_ARA
    initLine += ", 1";
#else
    initLine += ", 0";
#endif
    initLine += ", \"" + getDir("USER_HOME_DIRECTORY") + "\"";
    initLine += ", \"" + getDir("USER_DOCUMENTS_DIRECTORY") + "\"";
    initLine += ", \"" + getDir("USER_DESKTOP_DIRECTORY") + "\"";
    initLine += ", \"" + getDir("USER_MUSIC_DIRECTORY") + "\"";
    initLine += ", \"" + getDir("USER_MOVIES_DIRECTORY") + "\"";
    initLine += ", \"" + getDir("USER_PICTURES_DIRECTORY") + "\"";
    initLine += ", \"" + getDir("USER_APPLICATION_DATA_DIRECTORY") + "\"";
    initLine += ", \"" + getDir("COMMON_APPLICATION_DATA_DIRECTORY") + "\"";
    initLine += ", \"" + getDir("COMMON_DOCUMENTS_DIRECTORY") + "\"";
    initLine += ", \"" + getDir("WINDOWS_SYSTEM_DIRECTORY") + "\"";
    initLine += ", \"" + getDir("GLOBAL_APPLICATIONS_DIRECTORY") + "\"";

    return structDef + initLine;
}

//===========================================================================================
int Engine::getCurrentParameterCount()
{
    return static_cast<int>(processor.getParameters().size());
}

//===========================================================================================
int Engine::getNumberOfParameters(const std::string &csdFile)
{
    std::vector<nlohmann::json> widgets =
        cabbage::Parser::parseCsdForWidgets(csdFile.empty() ? cabbage::File::getCsdFileAndPath() : csdFile);
    int numParams = 0;
    for (auto &w : widgets)
    {
        // Check for automatable - expect boolean true
        if (w.contains("automatable") && w["automatable"].is_boolean() && w["automatable"].get<bool>())
            numParams++;
    }

    return numParams;
}

const std::string Engine::getIOChannalConfig(const std::string &csdFile)
{
    // channelConfig in the JSON is authoritative — it drives the channel count,
    // so validating it against nchnls (which it is meant to override) is wrong.
    // getChannelConfig() already returns "Stereo:2|2" as a safe fallback.
    return cabbage::Utils::getChannelConfig(csdFile);
}

//===========================================================================================

void Engine::setControlChannel(const std::string channel, cs_float value)
{
    // update Csound channel, and update ParameterChannel values..
    csound->SetControlChannel(channel.c_str(), value);
}

void Engine::setStringChannel(const std::string channel, std::string data)
{
    lattice::logDebug << "setStringChannel[" << channel << "]: Setting to '" << data << "'";
    // update Csound channel
    csound->SetStringChannel(channel.c_str(), (char *)data.c_str());
}

//=============================================================================================
void Engine::processCsoundMessages()
{
    auto *csound = getCsound();
    if (!csound)
        return;

    // set max number of message to prevent UI freezing..
    const int maxMessagesPerCycle = 100;
    int processedCount = 0;

    while (csound->GetMessageCnt() > 0 && processedCount++ < maxMessagesPerCycle)
    {
        const char *msg = csound->GetFirstMessage();
        if (!msg)
        {
            csound->PopFirstMessage();
            continue;
        }

        std::string message(msg);

        if (!message.empty() && message.back() == '\n')
        {
            message.pop_back();
        }

        // Skip repetitive "end of Performance" messages
        if (message.find("end of Performance") != std::string::npos)
        {
            csound->PopFirstMessage();
            continue;
        }

        if (csoundOutputEnabled)
        {
            // Engine does not own the frontend transport (webview/stdout).
            // Queue a Generic opcode message and let the wrapper layer forward it:
            // - Plugin mode: CabbageProcessor::updateWidgetData() -> sendWebViewMessage()
            // - CabbageApp: CabbageAudioApp::hostCallback() -> sendJsonMessage()
            CabbageOpcodeData data;
            data.type = CabbageOpcodeData::MessageType::Generic;
            data.channel = "csoundOutput";
            data.cabbageJson["command"] = "csoundOutputUpdate";
            data.cabbageJson["text"] = message;
            opcodeData.enqueue(data);
        }

        lattice::logDebug << message; // Log the message
        csound->PopFirstMessage();   // Remove from queue
    }
}

bool Engine::hasCsoundOutputWidget() const
{
    std::lock_guard<std::mutex> lock(widgetsMutex);

    std::function<bool(const nlohmann::json &)> containsCsoundOutput =
        [&](const nlohmann::json &widget) -> bool
    {
        if (!widget.is_object())
            return false;

        if (widget.contains("type") && widget["type"].is_string() &&
            choc::text::trim(widget["type"].get<std::string>()) == "csoundOutput")
            return true;

        if (widget.contains("children") && widget["children"].is_array())
        {
            for (const auto &child : widget["children"])
            {
                if (containsCsoundOutput(child))
                    return true;
            }
        }

        return false;
    };

    for (const auto &widget : widgets)
    {
        if (containsCsoundOutput(widget))
            return true;
    }

    return false;
}

//===========================================================================================
size_t Engine::getIndexForParamChannel(std::string name)
{
    auto it = std::find_if(parameterChannels.begin(), parameterChannels.end(),
                           [&name](const ParameterChannel &paramChannel) { return paramChannel.name == name; });

    if (it != parameterChannels.end())
    {
        size_t index = std::distance(parameterChannels.begin(), it);
        return index;
    }

    return -1;
}

//===========================================================================================
std::string Engine::extractChannelName(const nlohmann::json &widget)
{
    // First priority: explicit widget id
    if (widget.contains("id") && widget["id"].is_string())
    {
        return cabbage::Parser::removeQuotes(widget["id"].get<std::string>());
    }

    // Second priority: new channels array format
    if (widget.contains("channels") && widget["channels"].is_array() && !widget["channels"].empty())
    {
        auto &firstChannel = widget["channels"][0];
        if (firstChannel.contains("id") && firstChannel["id"].is_string())
        {
            return cabbage::Parser::removeQuotes(firstChannel["id"].get<std::string>());
        }
    }

    // Third priority: legacy channel formats for backward compatibility
    if (widget.contains("channel"))
    {
        if (widget["channel"].is_string())
        {
            lattAssert(widget["channel"].is_string(), "Channel should be a string");
        }
        else if (widget["channel"].is_object() && widget["channel"].contains("id"))
        {
            return cabbage::Parser::removeQuotes(widget["channel"]["id"].get<std::string>());
        }
    }

    return "";
}

//===========================================================================================
std::optional<std::reference_wrapper<nlohmann::json>> Engine::findWidgetInArray(nlohmann::json &jsonArray,
                                                                                const std::string &channel)
{
    for (auto &w : jsonArray)
    {
        // New schema: channels array
        if (w.contains("channels") && w["channels"].is_array())
        {
            for (const auto &ch : w["channels"])
            {
                if (ch.contains("id") && ch["id"].is_string() && cabbage::Parser::removeQuotes(ch["id"]) == channel)
                {
                    return std::ref(w);
                }
            }
        }
        // Old schema: channel as string or object
        if (w.contains("channel"))
        {
            // Handle single-channel widgets (channel is a string)
            if (w["channel"].is_string() && cabbage::Parser::removeQuotes(w["channel"]) == channel)
            {
                return std::ref(w);
            }
            // Handle multi-channel widgets (channel is an object with x/y properties)
            else if (w["channel"].is_object())
            {
                for (auto &[key, value] : w["channel"].items())
                {
                    if (value.is_string() && cabbage::Parser::removeQuotes(value) == channel)
                    {
                        return std::ref(w);
                    }
                }
            }
        }
        if (w.contains("children") && w["children"].is_array())
        {
            auto child = findWidgetInArray(w["children"], channel);
            if (child)
                return child;
        }
    }
    return std::nullopt;
}

// Overload for std::vector<nlohmann::json>
// Thread-safe version that returns a copy
std::optional<nlohmann::json> Engine::getWidgetCopyById(const std::string &channel)
{
    std::lock_guard<std::mutex> lock(widgetsMutex);
    
    for (const auto &w : widgets)
    {
        if (!w.is_object())
            continue;
            
        // Check channels array
        if (w.contains("channels") && w["channels"].is_array())
        {
            for (const auto &ch : w["channels"])
            {
                if (ch.is_object() && ch.contains("id") && ch["id"].is_string() && 
                    cabbage::Parser::removeQuotes(ch["id"]) == channel)
                {
                    return w; // Return a copy
                }
            }
        }

        if (w.contains("id") && w["id"].is_string() && 
            cabbage::Parser::removeQuotes(w["id"]) == channel)
        {
            return w; // Return a copy
        }
        
        // Check children recursively
        if (w.contains("children") && w["children"].is_array())
        {
            auto childOpt = findWidgetInArray(const_cast<nlohmann::json&>(w["children"]), channel);
            if (childOpt)
                return childOpt->get(); // Return a copy
        }
    }
    return std::nullopt;
}

// Thread-safe atomic update: read-modify-write
bool Engine::updateWidget(const std::string &channel, std::function<void(nlohmann::json&)> modifier)
{
    std::lock_guard<std::mutex> lock(widgetsMutex);
    
    for (auto &w : widgets)
    {
        if (!w.is_object())
            continue;
            
        // Check channels array
        if (w.contains("channels") && w["channels"].is_array())
        {
            for (const auto &ch : w["channels"])
            {
                if (ch.is_object() && ch.contains("id") && ch["id"].is_string() && 
                    cabbage::Parser::removeQuotes(ch["id"]) == channel)
                {
                    modifier(w);
                    return true;
                }
            }
        }

        if (w.contains("id") && w["id"].is_string() && 
            cabbage::Parser::removeQuotes(w["id"]) == channel)
        {
            modifier(w);
            return true;
        }
        
        // Check children recursively
        if (w.contains("children") && w["children"].is_array())
        {
            // For children, we need to search recursively
            std::function<bool(nlohmann::json&)> searchChildren = [&](nlohmann::json &arr) -> bool {
                for (auto &child : arr)
                {
                    if (!child.is_object())
                        continue;
                        
                    if (child.contains("channels") && child["channels"].is_array())
                    {
                        for (const auto &ch : child["channels"])
                        {
                            if (ch.is_object() && ch.contains("id") && ch["id"].is_string() && 
                                cabbage::Parser::removeQuotes(ch["id"]) == channel)
                            {
                                modifier(child);
                                return true;
                            }
                        }
                    }
                    
                    if (child.contains("id") && child["id"].is_string() && 
                        cabbage::Parser::removeQuotes(child["id"]) == channel)
                    {
                        modifier(child);
                        return true;
                    }
                    
                    if (child.contains("children") && child["children"].is_array())
                    {
                        if (searchChildren(child["children"]))
                            return true;
                    }
                }
                return false;
            };
            
            if (searchChildren(w["children"]))
                return true;
        }
    }
    return false;
}

std::optional<std::reference_wrapper<nlohmann::json>> Engine::getWidgetFromId(std::vector<nlohmann::json> &widgets,
                                                                                 const std::string &channel)
{
    std::lock_guard<std::mutex> lock(widgetsMutex);
    
    for (auto &w : widgets)
    {
        // Safety check: ensure w is a valid object
        if (!w.is_object())
        {
            lattice::logError << "Engine::getWidgetFromId: encountered non-object widget, skipping";
            continue;
        }
        
        // New schema: channels array
        if (w.contains("channels") && w["channels"].is_array())
        {
            for (const auto &ch : w["channels"])
            {
                if (ch.is_object() && ch.contains("id") && ch["id"].is_string() && cabbage::Parser::removeQuotes(ch["id"]) == channel)
                {
                    return std::ref(w);
                }
            }
        }

        if (w.contains("id") && w["id"].is_string() && cabbage::Parser::removeQuotes(w["id"]) == channel)
        {
            return std::ref(w);
        }
        
        if (w.contains("children") && w["children"].is_array())
        {
            auto child = findWidgetInArray(w["children"], channel);
            if (child)
                return child;
        }
    }
    return std::nullopt;
}

// Overload for nlohmann::json (assuming it's an array)
std::optional<std::reference_wrapper<nlohmann::json>> Engine::getWidgetByChannel(nlohmann::json &widgets,
                                                                                 const std::string &channel)
{
    return findWidgetInArray(widgets, channel);
}

const std::string Engine::updateWidgetState(nlohmann::json j)
{
    // Accept either 'id' (new) or 'channel' (legacy)
    std::string channel;
    if (j.contains("id") && j["id"].is_string())
    {
        channel = j["id"].get<std::string>();
    }
    else if (j.contains("channel") && j["channel"].is_string())
    {
        channel = j["channel"].get<std::string>();
    }
    else
    {
        lattice::logError << "updateWidgetState: missing 'id' or 'channel' field";
        return "";
    }
    
    std::string result;
    bool updated = updateWidget(channel, [&](nlohmann::json &w) {
        w.merge_patch(j);
        result = getUpdatedWidgetJsonStr(channel, w.dump(), false);
    });
    
    return updated ? result : "";
}
//===========================================================================================

std::string Engine::getUpdatedWidgetJsonStr(const std::string &channel, std::string data, bool includeValue)
{
    std::string result;
    if (includeValue)
    {
        // Parse the data to extract the value from channels[0].range.value
        auto widgetJson = nlohmann::json::parse(data, nullptr, /*allow_exceptions=*/false);
        if (widgetJson.is_discarded())
            return result;
        float value = 0.0f;

        // Read from channels[0].range.value (new architecture)
        if (widgetJson.contains("channels") && widgetJson["channels"].is_array() && 
            !widgetJson["channels"].empty())
        {
            auto& firstChannel = widgetJson["channels"][0];
            if (firstChannel.contains("range") && firstChannel["range"].contains("value"))
            {
                value = firstChannel["range"]["value"].get<float>();
            }
            else if (firstChannel.contains("range") && firstChannel["range"].contains("defaultValue"))
            {
                value = firstChannel["range"]["defaultValue"].get<float>();
            }
        }
        else
        {
            value = 0.0f; // Default fallback value
        }
        // Escape the data string for JSON by replacing backslashes and quotes
        std::string escapedData = data;
        size_t pos = 0;
        while ((pos = escapedData.find('\\', pos)) != std::string::npos) {
            escapedData.replace(pos, 1, "\\\\");
            pos += 2;
        }
        pos = 0;
        while ((pos = escapedData.find('"', pos)) != std::string::npos) {
            escapedData.replace(pos, 1, "\\\"");
            pos += 2;
        }
        
        result = choc::text::replace(R"(
        {
            "command": "widgetUpdate",
            "id": "$CHANNEL",
            "widgetJson": "$DATA",
            "value": $VALUE
        }
        )",
                                     "$CHANNEL", channel, "$DATA", escapedData, "$VALUE", std::to_string(value));
    }
    else
    {
        result = choc::text::replace(R"(
        {
            command: "widgetUpdate",
            id: "$CHANNEL",
            widgetJson: `$DATA`
        }
        )",
                                     "$CHANNEL", channel, "$DATA", data);
    }
    return result;
}

std::string Engine::getUpdatedWidgetJsonStr(const std::string &channel, float value)
{
    std::string result;
    result = choc::text::replace(R"(
    {
        command: "widgetUpdate",
        id: "$CHANNEL",
        value: $VALUE
    }
    )",
    "$CHANNEL", channel, "$VALUE", std::to_string(value));

    return result.c_str();
}

void Engine::updateFunctionTable(CabbageOpcodeData data, nlohmann::json &jsonObj)
{
    if (jsonObj["type"].get<std::string>() == "genTable")
    {
        if (data.cabbageJson.contains("tableNumber") && data.cabbageJson["tableNumber"].get<int>() != -9999)
        {
            try
            {
                cabbage::Parser::mergeJsonProperties(jsonObj, data.cabbageJson);

                // Get tableNumber from data.cabbageJson since initialiseWidgetJson may not set it reliably
                int tableNumber = -1;
                if (data.cabbageJson.contains("tableNumber") && data.cabbageJson["tableNumber"].is_number())
                {
                    tableNumber = data.cabbageJson["tableNumber"].get<int>();
                }
                else if (jsonObj.contains("tableNumber") && jsonObj["tableNumber"].is_number())
                {
                    tableNumber = jsonObj["tableNumber"].get<int>();
                }

                if (tableNumber != -1)
                {
                    const int tableSize = getCsound()->TableLength(tableNumber);

                    if (tableSize != -1)
                    {
                        cs_float *tablePtr = nullptr;
                        auto length = csound->GetTable(&tablePtr, tableNumber);
                        std::vector<cs_float> temp(tablePtr, tablePtr + length);
                        setTableJSON(data.channel, temp, jsonObj);
                    }
                }
            }
            catch (nlohmann::json::exception &e)
            {
                lattice::logDebug << "JSON Error:" << e.what();
            }
        }
        else if (data.cabbageJson.contains("file"))
        {
            try
            {

                cabbage::Parser::mergeJsonProperties(jsonObj, data.cabbageJson);

                auto soundfile = cabbage::File::readAudioFile<cs_float>(jsonObj["file"].get<std::string>(), static_cast<int>(sampleRate));
                auto samples = soundfile.audioData;

                if (samples.size() == 0)
                    return;

                setTableJSON(data.channel, samples, jsonObj);
            }
            catch (nlohmann::json::exception &e)
            {
                lattice::logDebug << "JSON Error:" << e.what();
            }
        }
    }
}

void Engine::setTableJSON(std::string /*channel*/, std::vector<cs_float> samples, nlohmann::json &jsonObj)
{
    // this is a condensed version of the sample data that is passed around between C++ and JS.
    std::vector<cs_float> widgetSampleData;

    // Ensure range object exists and set default y-axis range for waveform display
    if (!jsonObj.contains("range"))
    {
        jsonObj["range"] = nlohmann::json::object();
    }
    if (!jsonObj["range"].contains("y"))
    {
        jsonObj["range"]["y"] = {{"min", -1.0}, {"max", 1.0}};
    }

    // Handle nested range structure for genTable (sampleRange for sample selection)
    int startSample = 0;
    int endSample = static_cast<int>(samples.size());

    if (jsonObj.contains("range") && jsonObj["range"].is_object())
    {
        if (jsonObj["range"].contains("x") && jsonObj["range"]["x"].is_object())
        {
            // Legacy nested structure: range.x.start/end for sample range
            if (jsonObj["range"]["x"].contains("start"))
                startSample = jsonObj["range"]["x"]["start"].get<int>();
            if (jsonObj["range"]["x"].contains("end"))
                endSample = jsonObj["range"]["x"]["end"].get<int>() == -1 ? static_cast<int>(samples.size())
                                                                          : jsonObj["range"]["x"]["end"].get<int>();
        }
        else if (jsonObj["range"].contains("start"))
        {
            // Legacy flat structure: range.start/end
            startSample = jsonObj["range"]["start"].get<int>() == 0 ? 0 : jsonObj["range"]["start"].get<int>();
            endSample = jsonObj["range"]["end"].get<int>() == -1 ? static_cast<int>(samples.size())
                                                                 : jsonObj["range"]["end"].get<int>();
        }
    }

    // no point in sending more samples that can be displayed per pixel...
    const float incr = float(endSample - startSample) / ((jsonObj["bounds"]["width"].get<float>()));
    lattice::logDebug << "Updating function table";
    for (int i = startSample; i < static_cast<int>(endSample); i += static_cast<int>(incr))
    {
        widgetSampleData.push_back(samples[int(i)]);
    }

    jsonObj["samples"] = widgetSampleData;
    jsonObj["totalSamples"] = static_cast<int>(samples.size());
}

void Engine::initialiseGenTableWidgets()
{
    for (auto &widget : widgets)
    {
        if (widget["type"].get<std::string>() == "genTable" && widget.contains("file") && widget["file"].is_string() &&
            !widget["file"].get<std::string>().empty())
        {
            try
            {
                const int tableNumber = widget["tableNumber"].get<int>();
                auto soundfile = cabbage::File::readAudioFile<cs_float>(widget["file"].get<std::string>(), static_cast<int>(sampleRate));
                auto samples = soundfile.audioData;

                if (samples.size() > 0)
                {
                    std::stringstream ss;
                    ss << "giTable" << tableNumber << " ftgen " << tableNumber << ", 0, " << samples.size()
                       << ", -7, 0, 0";
                    getCsound()->CompileOrc(ss.str().c_str());
                    const int tableSize = getCsound()->TableLength(tableNumber);

                    if (tableSize != -1)
                    {
                        cs_float *tablePtr = nullptr;
                        getCsound()->GetTable(&tablePtr, tableNumber);
                        std::memcpy(tablePtr, samples.data(),
                                    std::min(tableSize, static_cast<int>(samples.size())) * sizeof(cs_float));
                        setTableJSON("", samples, widget);
                    }
                    else
                    {
                        lattice::logError << "Failed to create/update Csound table for genTable: "
                                          << widget["file"].get<std::string>();
                    }
                }
                else
                {
                    lattice::logError << "No samples loaded from file: " << widget["file"].get<std::string>();
                }
            }
            catch (const std::exception &e)
            {
                lattice::logError << "Failed to load genTable file: " << e.what();
            }
        }
        else if (widget["type"].get<std::string>() == "genTable" && widget.contains("tableNumber") &&
                 widget["tableNumber"].is_number() && widget["tableNumber"] != -9999)
        {
            std::string channelName;
            if (widget["channel"].is_string())
            {
                lattice::logDebug << "Error widget[\"channel\"] " << widget["channel"] << "should not be string ";
            }
            else if (widget["channel"].is_object() && widget["channel"].contains("id"))
            {
                channelName = cabbage::Parser::removeQuotes(widget["channel"]["id"].get<std::string>());
            }
            else
            {
                // Skip if no valid channel
                continue;
            }
            CabbageOpcodeData data;
            data.channel = channelName;
            data.type = CabbageOpcodeData::MessageType::Identifier;
            data.cabbageJson = {{"tableNumber", widget["tableNumber"].get<int>()}};
            opcodeData.enqueue(data);
        }
    }
}

void Engine::queueGenTableUpdates()
{
    // Automatically queue table data updates for genTable widgets with tableNumber > 0.
    // This handles function tables defined in the Csound score (e.g., f 1 0 8 -2 ...)
    // which don't exist until after Csound's init pass. Without this, users would need
    // to manually call cabbageSet "tableId", "tableNumber", N to trigger the update.
    // Iterate through all widgets looking for genTable widgets with tableNumber > 0
    for (auto &widget : widgets)
    {
        if (widget["type"].get<std::string>() == "genTable" && widget.contains("tableNumber") &&
            widget["tableNumber"].is_number_integer())
        {
            const int tableNumber = widget["tableNumber"].get<int>();

            // Only process if tableNumber is valid (> 0)
            if (tableNumber > 0)
            {
                // Check if this table exists in Csound
                const int tableSize = getCsound()->TableLength(tableNumber);

                if (tableSize > 0)
                {
                    // Get the channel name
                    std::string channelId = extractChannelName(widget);

                    if (!channelId.empty())
                    {
                        // Create an opcode data message to trigger table update
                        CabbageOpcodeData data;
                        data.channel = channelId;
                        data.type = CabbageOpcodeData::MessageType::Identifier;
                        data.cabbageJson["tableNumber"] = tableNumber;

                        // Queue the update - this will be processed in onIdle()
                        opcodeData.enqueue(data);

                        lattice::logDebug << "Queued automatic table update for genTable '" << channelId
                                          << "' with tableNumber " << tableNumber << " (size: " << tableSize << ")";
                    }
                }
            }
        }
    }
}

const std::string Engine::getCsoundOutputUpdateScript(const std::string &output)
{
    auto outputText = choc::text::replace(output, "`", "");

    std::string result;
    result = choc::text::replace(R"(
         window.postMessage({ command: "csoundOutputUpdate", text: `$OUTPUT_TEXT` });
        )",
                                 "$OUTPUT_TEXT", outputText);

    return result.c_str();
}

//===========================================================================================

float Engine::remap(double n, double start1, double stop1, double start2, double stop2)
{
    return static_cast<float>(((n - start1) / (stop1 - start1)) * (stop2 - start2) + start2);
}

float Engine::getFullRangeValue(std::string channel, float normalValue)
{
    for (const auto &w : getWidgets())
    {
        if (w["channel"] == channel && w.contains("range"))
        {
            return Engine::remap(normalValue, 0.f, 1.f, w["range"]["min"], w["range"]["max"]);
        }
        else
            return normalValue;
    }

    return normalValue;
}

//===========================================================================================
std::string Engine::removeControlCharacters(const std::string &input)
{
    std::string result;
    for (char c : input)
    {
        if (!iscntrl(static_cast<unsigned char>(c)) || c == ' ')
        {
            result += c;
        }
    }
    return result;
}

// Check if a widget has a specific channel (searches id then channels array)
bool Engine::hasChannel(const nlohmann::json &widget, const std::string &channel)
{
    // First check widget["id"]
    if (widget.contains("id") && widget["id"].is_string())
    {
        std::string widgetId = cabbage::Parser::removeQuotes(widget["id"].get<std::string>());
        if (widgetId == channel)
        {
            return true;
        }
    }

    // Then check widget["channels"] array
    if (widget.contains("channels") && widget["channels"].is_array())
    {
        for (const auto &ch : widget["channels"])
        {
            if (ch.contains("id") && ch["id"].is_string())
            {
                std::string chId = cabbage::Parser::removeQuotes(ch["id"].get<std::string>());
                if (chId == channel)
                {
                    return true;
                }
            }
        }
    }

    return false;
}

void Engine::updateChannelCache(const CabbageOpcodeData &data)
{
    if (shuttingDown.load(std::memory_order_acquire))
        return;

    std::lock_guard<std::mutex> lock(channelCacheMutex);

    auto it = channelCache.find(data.channel);
    if (it == channelCache.end())
    {
        channelCache[data.channel] = data;
    }
    else if (data.type == CabbageOpcodeData::MessageType::Value)
    {
        // Value-only updates refresh just the value field. The cached type is
        // deliberately left untouched so a pending identifier update for the
        // same channel within this block is preserved (previously this line
        // downgraded the entry to Value, causing flushChannelCache() to drop
        // the identifier payload entirely).
        it->second.cabbageJson["value"] = data.cabbageJson["value"];
    }
    else
    {
        it->second.cabbageJson.merge_patch(data.cabbageJson);
        // If the new data is of type Identifier, we must update the cached type to Identifier
        // so that the full JSON is sent to the frontend, not just the value.
        if (data.type == CabbageOpcodeData::MessageType::Identifier)
        {
            it->second.type = CabbageOpcodeData::MessageType::Identifier;
        }
        if (!data.identifier.empty())
        {
            it->second.identifier = data.identifier;
        }
    }

    // Track dirtiness per kind so flush can emit a value message and/or an
    // identifier message for the same channel independently.
    if (data.type == CabbageOpcodeData::MessageType::Value)
    {
        dirtyValueChannels.insert(data.channel);
    }
    else
    {
        dirtyIdentifierChannels.insert(data.channel);
    }
}

bool Engine::isValueDifferent(const CabbageOpcodeData &data)
{
    if (shuttingDown.load(std::memory_order_acquire))
        return false;

    std::lock_guard<std::mutex> lock(channelCacheMutex);

    if (channelCache.find(data.channel) == channelCache.end())
    {
        return true;
    }

    const auto &cachedData = channelCache[data.channel];

    // Helper lambda to check if json is subset
    auto isSubset = [](const nlohmann::json &j1, const nlohmann::json &j2) -> bool {
        if (j1.is_object() && j2.is_object())
        {
            for (auto it = j1.begin(); it != j1.end(); ++it)
            {
                if (j2.find(it.key()) == j2.end())
                {
                    return false;
                }
                if (it.value() != j2[it.key()])
                {
                    return false;
                }
            }
            return true;
        }
        return j1 == j2;
    };

    if (data.type == CabbageOpcodeData::MessageType::Value)
    {
        // Either side may lack a "value" key (e.g. a channel previously touched
        // only by cabbageSet identifiers). Missing counts as different so the
        // update is not wrongly suppressed. contains() keeps this branch
        // no-throw on the audio thread.
        const auto &cachedJson = cachedData.cabbageJson;
        const bool hasCached = cachedJson.contains("value");
        const bool hasNew = data.cabbageJson.contains("value");
        if (!hasCached || !hasNew)
        {
            return true;
        }
        return cachedJson["value"] != data.cabbageJson["value"];
    }
    else
    {
        // Check if new data is already contained in cached data
        return !isSubset(data.cabbageJson, cachedData.cabbageJson);
    }
}

void Engine::flushChannelCache()
{
    if (shuttingDown.load(std::memory_order_acquire))
        return;

    std::lock_guard<std::mutex> lock(channelCacheMutex);

    // Identifier updates first. The top-level "value" key is stripped here:
    // the latest value travels in its own minimal message below, so a k-rate
    // cabbageSetValue can no longer bury a same-block cabbageSet identifier
    // update (or vice versa). Emitting two messages instead of one merged
    // message also matches the existing frontend contract, which handles
    // {id, value} and {id, widgetJson} independently.
    for (const auto &channel : dirtyIdentifierChannels)
    {
        auto it = channelCache.find(channel);
        if (it == channelCache.end())
        {
            continue;
        }
        auto &cachedData = it->second;

        nlohmann::json identJson = cachedData.cabbageJson;
        identJson.erase("value");

        if (identJson.empty())
        {
            // Degenerate case, e.g. cabbageSet with identifier "value": the
            // payload is just a value, so send it on the value path.
            if (cachedData.cabbageJson.contains("value"))
            {
                CabbageOpcodeData valueData;
                valueData.channel = cachedData.channel;
                valueData.type = CabbageOpcodeData::MessageType::Value;
                valueData.cabbageJson["value"] = cachedData.cabbageJson["value"];
                opcodeData.enqueue(valueData);
                dirtyValueChannels.erase(channel);
            }
        }
        else
        {
            CabbageOpcodeData identData;
            identData.channel = cachedData.channel;
            identData.type = CabbageOpcodeData::MessageType::Identifier;
            identData.identifier = cachedData.identifier;
            identData.skipPopulateProcessing = cachedData.skipPopulateProcessing;
            identData.cabbageJson = std::move(identJson);
            opcodeData.enqueue(identData);
        }

        // Evict identifier keys so stale payloads (e.g. genTable samples) are
        // never resent at value rate. The last value is retained for
        // isValueDifferent() dedup; pure-identifier entries are removed.
        if (cachedData.cabbageJson.contains("value"))
        {
            CabbageOpcodeData evicted;
            evicted.channel = cachedData.channel;
            evicted.type = CabbageOpcodeData::MessageType::Value;
            evicted.cabbageJson["value"] = cachedData.cabbageJson["value"];
            it->second = std::move(evicted);
        }
        else
        {
            channelCache.erase(it);
        }
    }
    dirtyIdentifierChannels.clear();

    // Value-only updates collapse to one minimal message per channel.
    for (const auto &channel : dirtyValueChannels)
    {
        auto it = channelCache.find(channel);
        if (it == channelCache.end())
        {
            continue;
        }
        const auto &cachedJson = it->second.cabbageJson;
        auto valueIt = cachedJson.find("value");
        if (valueIt == cachedJson.end())
        {
            continue;
        }
        CabbageOpcodeData minimalData;
        minimalData.channel = it->second.channel;
        minimalData.type = CabbageOpcodeData::MessageType::Value;
        minimalData.cabbageJson["value"] = *valueIt;
        opcodeData.enqueue(minimalData);
        // The cache entry (last value) is intentionally retained for
        // isValueDifferent() dedup across blocks.
    }
    dirtyValueChannels.clear();
}

//=====================================================================================
// State Management Utilities - Used by both opcodes and CabbageProcessor
//=====================================================================================

nlohmann::json Engine::saveWidgetJsonData(bool isPresetSave)
{
    // Make a quick copy of widgets to avoid holding mutex during filtering/serialization
    std::vector<nlohmann::json> widgetsCopy;
    {
        std::lock_guard<std::mutex> lock(widgetsMutex);
        widgetsCopy = widgets;
    }

    // Filter widgets without holding the mutex (avoid blocking audio thread)
    nlohmann::json state;
    nlohmann::json filteredWidgets = nlohmann::json::array();

    for (auto widget : widgetsCopy)
    {
        if (widget.is_object())
        {
            // Check persistence settings based on save type
            bool shouldInclude = true;
            if (widget.contains("persistence") && widget["persistence"].is_object())
            {
                const auto& persistence = widget["persistence"];

                if (isPresetSave)
                {
                    // For preset saves (DAW), check persistence.preset
                    if (persistence.contains("preset") && persistence["preset"].is_boolean())
                    {
                        shouldInclude = persistence["preset"].get<bool>();
                    }
                }
                else
                {
                    // For session saves (opcode), check persistence.session
                    if (persistence.contains("session") && persistence["session"].is_boolean())
                    {
                        shouldInclude = persistence["session"].get<bool>();
                    }
                }
            }
            // Legacy support: fall back to presetIgnore if persistence object doesn't exist
            else if (widget.contains("presetIgnore") && widget["presetIgnore"].is_boolean())
            {
                shouldInclude = !widget["presetIgnore"].get<bool>();
            }

            // Only add widget if it should be included
            if (shouldInclude)
            {
                // Update channel range values from Csound channels before saving
                if (widget.contains("channels") && widget["channels"].is_array())
                {
                    for (auto& channel : widget["channels"])
                    {
                        if (channel.is_object() && channel.contains("id") && channel["id"].is_string())
                        {
                            std::string channelId = channel["id"].get<std::string>();

                            // Check if this is a string channel
                            bool isStringChannel = channel.contains("type") &&
                                                   channel["type"].is_string() &&
                                                   channel["type"].get<std::string>() == "string";

                            if (isStringChannel)
                            {
                                // For string channels, the stringValue is already in the widget JSON
                                // (set by processWebViewCommand when the UI sends the value)
                                // Just log it for debugging
                                if (channel.contains("stringValue") && channel["stringValue"].is_string())
                                {
                                    lattice::logDebug << "Saved string channel '" << channelId
                                                      << "' value: " << channel["stringValue"].get<std::string>();
                                }
                            }
                            else
                            {
                                // Read current value from Csound control channel
                                cs_float* channelPtr = nullptr;
                                if (csoundGetChannelPtr(csound->GetCsound(), (void**)&channelPtr, channelId.c_str(),
                                                         CSOUND_CONTROL_CHANNEL | CSOUND_OUTPUT_CHANNEL) == CSOUND_SUCCESS)
                                {
                                    if (channelPtr != nullptr)
                                    {
                                        cs_float currentValue = *channelPtr;

                                        // Update the range.value with the current channel value
                                        if (channel.contains("range") && channel["range"].is_object())
                                        {
                                            channel["range"]["value"] = currentValue;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                
                // Remove top-level value property if it exists (shouldn't be there)
                if (widget.contains("value"))
                {
                    widget.erase("value");
                }
                
                // Remove populate config - it's a configuration property, not state
                // Saving it can cause issues if it was partially updated via cabbageSet
                if (widget.contains("populate"))
                {
                    widget.erase("populate");
                }

                filteredWidgets.push_back(widget);
            }
        }
    }

    // Debug: log string channel values in saved state
    for (const auto& w : filteredWidgets)
    {
        if (w.contains("channels") && w["channels"].is_array())
        {
            for (const auto& ch : w["channels"])
            {
                if (ch.contains("stringValue"))
                {
                    std::string chId = ch.value("id", "unknown");
                    lattice::logDebug << "saveWidgetJsonData: String channel '" << chId
                                     << "' stringValue: " << ch["stringValue"].dump();
                }
            }
        }
    }

    state["cabbageWidgetsState"] = filteredWidgets;
    return state;
}

nlohmann::json Engine::saveWidgetChannelData(const std::unordered_set<std::string>& fullJsonIds, bool allChannelDataOnly)
{
    std::vector<nlohmann::json> widgetsCopy;
    {
        std::lock_guard<std::mutex> lock(widgetsMutex);
        widgetsCopy = widgets;
    }

    nlohmann::json state;
    nlohmann::json filteredWidgets = nlohmann::json::array();

    for (auto widget : widgetsCopy)
    {
        if (!widget.is_object())
            continue;

        // Check persistence.session (same as existing opcode save behaviour)
        bool shouldInclude = true;
        if (widget.contains("persistence") && widget["persistence"].is_object())
        {
            const auto& persistence = widget["persistence"];
            if (persistence.contains("session") && persistence["session"].is_boolean())
                shouldInclude = persistence["session"].get<bool>();
        }
        else if (widget.contains("presetIgnore") && widget["presetIgnore"].is_boolean())
        {
            shouldInclude = !widget["presetIgnore"].get<bool>();
        }

        if (!shouldInclude)
            continue;

        // Determine widget match ID: top-level "id" if present, else first channel id
        std::string matchId;
        if (widget.contains("id") && widget["id"].is_string())
            matchId = widget["id"].get<std::string>();
        else if (widget.contains("channels") && widget["channels"].is_array() &&
                 !widget["channels"].empty())
        {
            const auto& firstChannel = widget["channels"][0];
            if (firstChannel.is_object() && firstChannel.contains("id") &&
                firstChannel["id"].is_string())
                matchId = firstChannel["id"].get<std::string>();
        }

        // Widgets in fullJsonIds get full JSON; others get channel data only.
        // When allChannelDataOnly is set, all widgets get channel data only.
        const bool isChannelDataOnly = allChannelDataOnly || (fullJsonIds.empty() ? false : fullJsonIds.count(matchId) == 0);

        // Update channel values from Csound for all widgets
        if (widget.contains("channels") && widget["channels"].is_array())
        {
            for (auto& channel : widget["channels"])
            {
                if (channel.is_object() && channel.contains("id") && channel["id"].is_string())
                {
                    const std::string channelId = channel["id"].get<std::string>();
                    cs_float* channelPtr = nullptr;
                    if (csoundGetChannelPtr(csound->GetCsound(), (void**)&channelPtr, channelId.c_str(),
                                           CSOUND_CONTROL_CHANNEL | CSOUND_OUTPUT_CHANNEL) == CSOUND_SUCCESS
                        && channelPtr != nullptr)
                    {
                        if (channel.contains("range") && channel["range"].is_object())
                            channel["range"]["value"] = *channelPtr;
                    }
                }
            }
        }

        if (isChannelDataOnly)
        {
            // Channel data only: id + channels array (contains range.value)
            nlohmann::json entry = nlohmann::json::object();
            if (widget.contains("id") && widget["id"].is_string())
                entry["id"] = widget["id"];
            if (widget.contains("channels"))
                entry["channels"] = widget["channels"];
            filteredWidgets.push_back(entry);
        }
        else
        {
            // Full JSON entry (same as saveWidgetJsonData)
            if (widget.contains("value"))
                widget.erase("value");
            if (widget.contains("populate"))
                widget.erase("populate");
            filteredWidgets.push_back(widget);
        }
    }

    state["cabbageWidgetsState"] = filteredWidgets;
    return state;
}

void Engine::loadWidgetState(const nlohmann::json &state, bool isPresetLoad)
{
    // Debug: log incoming state for string channels
    if (state.contains("cabbageWidgetsState") && state["cabbageWidgetsState"].is_array())
    {
        for (const auto& w : state["cabbageWidgetsState"])
        {
            if (w.contains("channels") && w["channels"].is_array())
            {
                for (const auto& ch : w["channels"])
                {
                    if (ch.contains("stringValue"))
                    {
                        std::string chId = ch.value("id", "unknown");
                        lattice::logDebug << "loadWidgetState: Incoming state has string channel '"
                                         << chId << "' stringValue: " << ch["stringValue"].dump();
                    }
                }
            }
        }
    }

    // Check if we have the widget state
    if (!state.contains("cabbageWidgetsState")) {
        lattice::logError << "Invalid state: missing 'cabbageWidgetsState' key";
        return;
    }
    
    // Validate that cabbageWidgetsState is an array
    if (!state["cabbageWidgetsState"].is_array()) {
        lattice::logError << "Invalid state: 'cabbageWidgetsState' is not an array";
        return;
    }
    
    // Instead of replacing widgets, we'll merge the channel values from the loaded state.
    // Full-JSON entries (those with a "type" key) also have all their properties merged back.
    std::unordered_map<std::string, std::unordered_map<std::string, double>> channelUpdates;
    std::unordered_map<std::string, nlohmann::json> fullJsonUpdates;

    try {
        for (const auto& savedWidget : state["cabbageWidgetsState"]) {
            if (!savedWidget.is_object()) {
                lattice::logError << "Invalid state: found non-object widget in state, aborting load";
                return;
            }

            std::string widgetId;
            if (savedWidget.contains("id") && savedWidget["id"].is_string())
                widgetId = cabbage::Parser::removeQuotes(savedWidget["id"]);

            std::string firstChannelId;
            if (savedWidget.contains("channels") && savedWidget["channels"].is_array() &&
                !savedWidget["channels"].empty())
            {
                const auto& ch0 = savedWidget["channels"][0];
                if (ch0.is_object() && ch0.contains("id") && ch0["id"].is_string())
                    firstChannelId = cabbage::Parser::removeQuotes(ch0["id"]);
            }
            const std::string matchId = widgetId.empty() ? firstChannelId : widgetId;

            // Full-JSON entry: has a "type" field
            if (savedWidget.contains("type") && !matchId.empty())
                fullJsonUpdates[matchId] = savedWidget;

            // Extract channel values from all entries
            if (savedWidget.contains("channels") && savedWidget["channels"].is_array())
            {
                for (const auto& channel : savedWidget["channels"])
                {
                    if (!channel.is_object())
                        continue;

                    std::string channelId;
                    if (channel.contains("id") && channel["id"].is_string())
                        channelId = cabbage::Parser::removeQuotes(channel["id"]);

                    if (channel.contains("range") && channel["range"].is_object() &&
                        channel["range"].contains("value"))
                    {
                        double value = 0.0;
                        if (channel["range"]["value"].is_number())
                            value = channel["range"]["value"].get<double>();
                        else if (channel["range"]["value"].is_null() &&
                                 channel["range"].contains("defaultValue") &&
                                 channel["range"]["defaultValue"].is_number())
                            value = channel["range"]["defaultValue"].get<double>();

                        if (!channelId.empty())
                            channelUpdates[channelId][matchId] = value;
                    }
                }
            }
        }
    }
    catch (const std::exception& e) {
        lattice::logError << "Exception during widget state preparation: " << e.what();
        return;
    }

    // Runtime-only keys that must never be overwritten from saved state
    static const std::unordered_set<std::string> runtimeKeys = {
        "parameterIndex", "value", "origBounds", "groupBaseBounds"
    };

    // Merge all updates into the live widgets array (mutex held for entire merge)
    {
        std::lock_guard<std::mutex> lock(widgetsMutex);

        lattice::logDebug << "Merging " << channelUpdates.size() << " channel updates and "
                          << fullJsonUpdates.size() << " full-JSON updates into existing widgets";

        for (auto& widget : widgets)
        {
            if (!widget.is_object())
                continue;

            // Resolve match ID for this widget (mirrors save-side logic)
            std::string widgetId;
            if (widget.contains("id") && widget["id"].is_string())
                widgetId = cabbage::Parser::removeQuotes(widget["id"]);

            std::string firstChannelId;
            if (widget.contains("channels") && widget["channels"].is_array() &&
                !widget["channels"].empty())
            {
                const auto& ch0 = widget["channels"][0];
                if (ch0.is_object() && ch0.contains("id") && ch0["id"].is_string())
                    firstChannelId = cabbage::Parser::removeQuotes(ch0["id"]);
            }
            const std::string matchId = widgetId.empty() ? firstChannelId : widgetId;

            // Apply full-JSON properties if this widget has a saved full-JSON entry
            if (!matchId.empty() && fullJsonUpdates.count(matchId) > 0)
            {
                const auto& savedFull = fullJsonUpdates[matchId];
                for (auto it = savedFull.begin(); it != savedFull.end(); ++it)
                {
                    // Skip runtime-only keys that should never be overwritten
                    if (runtimeKeys.count(it.key()) > 0)
                        continue;
                    widget[it.key()] = it.value();
                }
            }

            // Apply channel value updates
            if (widget.contains("channels") && widget["channels"].is_array())
            {
                for (auto& channel : widget["channels"])
                {
                    if (!channel.is_object())
                        continue;

                    std::string channelId;
                    if (channel.contains("id") && channel["id"].is_string())
                        channelId = cabbage::Parser::removeQuotes(channel["id"]);

                    if (!channelId.empty() && channelUpdates.count(channelId) > 0)
                    {
                        auto& updates = channelUpdates[channelId];
                        if (!channel.contains("range"))
                            channel["range"] = nlohmann::json::object();
                        if (updates.count(matchId) > 0)
                            channel["range"]["value"] = updates[matchId];
                    }
                }
            }
        }
    }

    // Update Csound channels and parameters (no mutex needed - reading from local copy)
    // Make a local copy to avoid holding lock during Csound calls
    std::vector<nlohmann::json> widgetsCopy;
    {
        std::lock_guard<std::mutex> lock(widgetsMutex);
        widgetsCopy = widgets;
    }
    
    for (const auto &widget : widgetsCopy)
    {
        // Skip widgets excluded from restore based on persistence settings
        bool shouldInclude = true;
        if (widget.contains("persistence") && widget["persistence"].is_object())
        {
            const auto &persistence = widget["persistence"];
            if (isPresetLoad)
            {
                // For preset loads, check persistence.preset
                if (persistence.contains("preset") && persistence["preset"].is_boolean())
                    shouldInclude = persistence["preset"].get<bool>();
            }
            else
            {
                // For session loads, check persistence.session
                if (persistence.contains("session") && persistence["session"].is_boolean())
                    shouldInclude = persistence["session"].get<bool>();
            }
        }
        else if (widget.contains("presetIgnore") && widget["presetIgnore"].is_boolean())
        {
            shouldInclude = !widget["presetIgnore"].get<bool>();
        }
        if (!shouldInclude)
            continue;

        if (widget.contains("channels") && widget["channels"].is_array())
        {
            for (const auto &channel : widget["channels"])
            {
                if (!channel.is_object() || !channel.contains("id") || !channel["id"].is_string())
                    continue;
                    
                std::string channelId = channel["id"].get<std::string>();

                // Check if this is a string channel
                bool isStringChannel = channel.contains("type") &&
                                       channel["type"].is_string() &&
                                       channel["type"].get<std::string>() == "string";

                if (isStringChannel)
                {
                    // Get string value from channel.stringValue
                    if (channel.contains("stringValue") && channel["stringValue"].is_string())
                    {
                        std::string stringValue = channel["stringValue"].get<std::string>();
                        lattice::logDebug << "Loading string channel '" << channelId << "' value: " << stringValue;
                        // Update Csound string channel
                        setStringChannel(channelId, stringValue);
                    }
                    else
                    {
                        lattice::logDebug << "String channel '" << channelId << "' has no stringValue in saved state";
                    }
                }
                // Get value from channel.range.value (proper location for multi-channel widgets)
                else if (channel.contains("range") && channel["range"].is_object() &&
                    channel["range"].contains("value") && channel["range"]["value"].is_number())
                {
                    float value = channel["range"]["value"].get<float>();

                    // Update Csound control channel
                    setControlChannel(channelId, value);

                    // If this is an automatable parameter, update it for the host
                    if (channel.contains("parameterIndex") && channel["parameterIndex"].is_number())
                    {
                        int paramIdx = channel["parameterIndex"].get<int>();
                        if (paramIdx < static_cast<int>(processor.getParameters().size()))
                        {
                            auto &param = processor.getParameters()[paramIdx];
                            float normalizedValue = param.toNormalised(value);
                            param.value = normalizedValue;

                            // Notify host of parameter change
                            processor.addParameterChange({paramIdx, normalizedValue, lattice::ParamChangeType::Value});
                        }
                    }
                }
            }
        }
    }
    
    // Queue UI updates for all widgets as a single batch message
    // This is more efficient than sending 148 individual messages
    // and prevents message flooding that could overwhelm the WebView
    nlohmann::json batchUpdate;
    batchUpdate["command"] = "batchWidgetUpdate";
    batchUpdate["widgets"] = nlohmann::json::array();
    
    int widgetCount = 0;
    for (const auto &widget : widgetsCopy) {
        // Skip widgets excluded from restore based on persistence settings
        bool shouldInclude = true;
        if (widget.contains("persistence") && widget["persistence"].is_object())
        {
            const auto &persistence = widget["persistence"];
            if (isPresetLoad)
            {
                // For preset loads, check persistence.preset
                if (persistence.contains("preset") && persistence["preset"].is_boolean())
                    shouldInclude = persistence["preset"].get<bool>();
            }
            else
            {
                // For session loads, check persistence.session
                if (persistence.contains("session") && persistence["session"].is_boolean())
                    shouldInclude = persistence["session"].get<bool>();
            }
        }
        else if (widget.contains("presetIgnore") && widget["presetIgnore"].is_boolean())
        {
            shouldInclude = !widget["presetIgnore"].get<bool>();
        }
        if (!shouldInclude)
            continue;
        
        std::string channelId;
        if (widget.contains("id") && widget["id"].is_string()) {
            channelId = widget["id"].get<std::string>();
        } else if (widget.contains("channels") && widget["channels"].is_array() && 
                   !widget["channels"].empty() && widget["channels"][0].contains("id")) {
            channelId = widget["channels"][0]["id"].get<std::string>();
        }
        
        if (!channelId.empty()) {
            batchUpdate["widgets"].push_back({
                {"id", channelId},
                {"widgetJson", widget.dump()}
            });
            widgetCount++;
        }
    }
    
    // Send single batch message instead of queuing many individual messages
    if (widgetCount > 0) {
//        lattice::logDebug << "Batch update structure: command=" << batchUpdate["command"]
//                         << ", widgets count=" << batchUpdate["widgets"].size()
//                         << ", first widget id=" << (batchUpdate["widgets"].size() > 0 ? batchUpdate["widgets"][0]["id"].get<std::string>() : "none");
//        
        // Queue as a special batch message that bypasses normal deduplication
        CabbageOpcodeData data;
        data.channel = "BATCH-UPDATE-7f3d2a";
        data.type = CabbageOpcodeData::MessageType::Identifier;
        data.cabbageJson = batchUpdate;
        opcodeData.enqueue(data);
    }
    
    lattice::logDebug << "Widget state loaded successfully. Queued batch update with " << widgetCount << " widgets for UI";
}

//=====================================================================================
// Process webview commands - central handler for UI messages
// Returns true if handled, false if environment-specific handling needed
//=====================================================================================
bool Engine::processWebViewCommand(const nlohmann::json &message)
{
    if (!message.contains("command"))
    {
        lattice::logError << "Message missing command field";
        return false;
    }

    const std::string command = message["command"].get<std::string>();

    // Handle cabbageIsReadyToLoad
    if (command == "cabbageIsReadyToLoad")
    {
        // This is typically handled by the processor to trigger UI updates
        return false; // Let processor handle this
    }

    // Handle widgetStateUpdate
    else if (command == "widgetStateUpdate")
    {
        updateWidgetState(message);
        return true;
    }

    // Handle controlData - route based on whether channel is automatable
    else if (command == "controlData")
    {
        // Extract channel
        std::string channel = message.value("channel", "");
        if (channel.empty())
        {
            lattice::logError << "controlData message missing channel";
            return false;
        }

        // Verbose arrival log (pairs with "Processor is null" drops in
        // CabbageAudioApp): lets tests and log readers correlate every UI
        // gesture that reached the backend with the resulting update.
        lattice::logDebug << "controlData received: channel='" << channel
                          << "' gesture='" << message.value("gesture", "complete")
                          << "' t+" << cabbage::millisSinceStart() << "ms";

        // Extract value - can be string or number
        nlohmann::json valueJson = message["value"];
        std::string gesture = message.value("gesture", "complete");

        // Check if this channel has a parameterIndex (is automatable)
        bool isAutomatable = false;
        int paramIdx = -1;

        // Look through all widgets to find this channel
        for (auto &widget : getWidgets())
        {
            if (widget.contains("channels") && widget["channels"].is_array())
            {
                for (auto &ch : widget["channels"])
                {
                    if (ch.contains("id") && ch["id"].is_string() && ch["id"].get<std::string>() == channel)
                    {
                        // Found the channel, check if it has parameterIndex
                        if (ch.contains("parameterIndex") && ch["parameterIndex"].is_number())
                        {
                            paramIdx = ch["parameterIndex"].get<int>();
                            if (paramIdx >= 0)
                            {
                                isAutomatable = true;
                            }
                        }
                        break;
                    }
                }
                if (isAutomatable) break;
            }
        }

        if (isAutomatable)
        {
            // For automatable channels, handle parameter update directly
            // Create a parameterChange message format that handleParameterUpdate expects
            nlohmann::json paramMessage = {
                {"paramIdx", paramIdx},
                {"channel", channel},
                {"value", valueJson},  // Pass the original value (string or number)
                {"gesture", gesture}
            };

            // Handle the parameter update directly in the Engine
            std::string gestureResult = handleParameterUpdate(paramMessage);
            if (!gestureResult.empty())
            {
                // Notify processor of the parameter change for DAW automation
                processor.addParameterChange({paramIdx, processor.getParameters()[paramIdx].value,
                                            gestureResult == "begin" ? lattice::ParamChangeType::GestureBegin :
                                            gestureResult == "value" ? lattice::ParamChangeType::Value :
                                            gestureResult == "end" ? lattice::ParamChangeType::GestureEnd :
                                            lattice::ParamChangeType::Complete});
            }
            return true; // Handled
        }
        else
        {
            // For non-automatable channels, route to channelData
            nlohmann::json channelMessage = {
                {"command", "channelData"},
                {"channel", channel}
            };

            // Set appropriate data field based on value type
            if (valueJson.is_string())
            {
                channelMessage["stringData"] = valueJson.get<std::string>();
            }
            else if (valueJson.is_number())
            {
                channelMessage["floatData"] = valueJson.get<double>();
            }
            else
            {
                lattice::logError << "controlData value must be string or number, got: " << valueJson.type_name();
                return false;
            }

            // Process channelData directly
            return processWebViewCommand(channelMessage);
        }
    }

    // Handle channelData
    else if (command == "channelData")
    {
        // Extract channel - accept 'id' (new) or 'channel' (legacy)
        std::string channel;
        
        // Try 'id' first (new format)
        if (message.contains("id"))
        {
            if (message["id"].is_string())
            {
                channel = message["id"].get<std::string>();
            }
            else if (message["id"].is_object() && message["id"].contains("id"))
            {
                channel = message["id"]["id"].get<std::string>();
            }
        }
        // Fall back to 'channel' (legacy format)
        else if (message.contains("channel"))
        {
            if (message["channel"].is_string())
            {
                channel = message["channel"].get<std::string>();
            }
            else if (message["channel"].is_object() && message["channel"].contains("id"))
            {
                channel = message["channel"]["id"].get<std::string>();
            }
            else if (message["channel"].is_object())
            {
                // For multi-channel, find the first string value
                for (auto &[key, value] : message["channel"].items())
                {
                    if (value.is_string())
                    {
                        channel = value.get<std::string>();
                        break;
                    }
                }
            }
        }
        
        if (channel.empty())
        {
            lattice::logError << "Invalid channel format in channelData message: " << message.dump();
            return false;
        }

        // Check if we have string data or float data
        if (message.contains("stringData"))
        {
            std::string stringData = message.value("stringData", "");
            // Set the Csound string channel
            setStringChannel(channel, stringData);

            // Update the widget JSON - set channel.stringValue for string channels
            updateWidget(channel, [&](nlohmann::json &j) {
                if (j.contains("channels") && j["channels"].is_array() && !j["channels"].empty())
                {
                    auto& firstChannel = j["channels"][0];
                    firstChannel["stringValue"] = stringData;
                }
            });
        }
        else if (message.contains("floatData"))
        {
            double floatData = message.value("floatData", 0.0);
            // Set the Csound control channel
            setControlChannel(channel, floatData);
            
            // Update the widget JSON - set channel.range.value for number channels
            updateWidget(channel, [&](nlohmann::json &j) {
                if (j.contains("channels") && j["channels"].is_array() && !j["channels"].empty())
                {
                    auto& firstChannel = j["channels"][0];
                    if (firstChannel.contains("range") && firstChannel["range"].is_object())
                    {
                        firstChannel["range"]["value"] = floatData;
                    }
                }
            });
        }
        else
        {
            lattice::logError << "channelData message missing both stringData and floatData fields";
            return false;
        }
      
        return true;
    }
    else if (command == "midiMessage")
    {
        //handled elsewhere..
         return false;
    }

    // Unknown command
    return false;
}

//=====================================================================================
// Handle parameter update from UI - validates, normalizes, and applies changes
// Returns gesture string for plugin automation, or empty string on failure
//=====================================================================================
std::string Engine::handleParameterUpdate(const nlohmann::json &message)
{
    // Check for old format (wrapped in "obj") - no longer supported
    if (message.contains("obj"))
    {
        lattice::logDebug << "parameterChange message using deprecated 'obj' wrapper format is no longer supported. "
                             "Please update to use direct properties.";
        return "";
    }

    // Validate required fields
    if (!message.contains("paramIdx") || !message.contains("value") || !message.contains("channel"))
    {
        lattice::logError << "parameterChange message missing required fields (paramIdx, value, or channel)";
        return "";
    }

    // Extract paramIdx
    int paramIdx = message["paramIdx"].get<int>();
    if (paramIdx < 0 || paramIdx >= static_cast<int>(processor.getParameters().size()))
    {
        lattice::logError << "Invalid paramIdx: " << paramIdx 
                         << " (valid range: 0-" << processor.getParameters().size() - 1 << ")";
        return "";
    }

    // Extract denormalized value
    double denormValue = message["value"].get<double>();

    // Extract channel - can be a string or an object with 'id'
    std::string channel;
    if (message["channel"].is_string())
    {
        channel = message["channel"].get<std::string>();
    }
    else if (message["channel"].is_object() && message["channel"].contains("id"))
    {
        channel = message["channel"]["id"].get<std::string>();
    }
    else
    {
        lattice::logError << "Invalid channel format in parameterChange message";
        return "";
    }

    // Extract gesture (optional, defaults to "complete")
    std::string gesture = message.value("gesture", "complete");

    // Normalize the value
    auto param = processor.getParameter(paramIdx);
    double normalizedValue = param.toNormalised(denormValue);

    // Update parameter value (this will apply quantization if increment > 0)
    // Note: setParameter already sends the denormalized value to Csound, so we don't need to do it here
    processor.setParameter(paramIdx, normalizedValue);

    // Get the quantized value from the parameter (setParameter may have quantized it)
    float quantizedValue = processor.getParameters()[paramIdx].value;

    // Update widget JSON - set channel.range.value for number channels
    updateWidget(channel, [&](nlohmann::json &j) {
        if (j.contains("channels") && j["channels"].is_array() && !j["channels"].empty())
        {
            auto& firstChannel = j["channels"][0];
            if (firstChannel.contains("range") && firstChannel["range"].is_object())
            {
                firstChannel["range"]["value"] = quantizedValue;
            }
        }
    });

    // Verbose applied log for discrete gestures (drag "value" gestures are
    // excluded to avoid flooding the log): proves the channel was written.
    if (gesture != "value")
    {
        lattice::logDebug << "controlData applied: channel='" << channel << "' value=" << quantizedValue
                          << " t+" << cabbage::millisSinceStart() << "ms";
    }

    return gesture;
}

} // namespace cabbage
