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

#pragma once
#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include <optional>
#include <unordered_map>
#include <unordered_set>

#include "CabbageUtils.h"
#include "csound.hpp"
#include "CabbageParser.h"
#include <concurrentqueue.h>
#include "opcodes/CabbageSetOpcodes.h"
#include "opcodes/CabbageGetOpcodes.h"
#include "opcodes/CabbageCreateOpcode.h"
#include "opcodes/CabbageStateOpcodes.h"
#include "opcodes/CabbageFileOpcodes.h"
#include "opcodes/CabbageSendMessageOpcode.h"
#include "opcodes/CabbageGetWidgetIdsOpcodes.h"
#include "opcodes/CabbageJsonOpcodes.h"

class CabbageProcessor;
struct CabbageOpcodeData;

namespace cabbage
{

class Engine
{

    // a vector containined all Cabbage widgets
    std::vector<nlohmann::json> widgets;
    mutable std::mutex widgetsMutex;

  public:
    // a parameter struct whose namees match that of the corresponding Csound channel
    struct ParameterChannel
    {
        std::string name;
        void setValue(float v) { value = v; }

        float getValue() { return value; }

        bool hasValueChanged(float newValue) { return init ? (init = false, true) : (value != newValue); }

        float value;
        bool init = true;
    };

    Engine(CabbageProcessor &p);
    ~Engine();

    // Get the Csound object
    Csound *getCsound() { return csound.get(); }

    // Setup Csound
    bool setupCsound();

    // Tear down Csound without destroying the Engine (used when SR changes)
    void teardownCsound();

    // Returns true if the Csound engine has been successfully initialised
    bool isEngineRunning() const { return csound != nullptr; }

    // Compile the CSD file
    void compileCsdFile(std::string csoundFile) { csCompileResult = csound->Compile(csoundFile.c_str()); }

    // Perform KSMPS (control periods)
    int performKsmps()
    {
        csCompileResult = csound->PerformKsmps();
        return csCompileResult;
    }

    // Set input value for a specific index in csSpin array
    void setSpIn(int index, cs_float value) { csSpin[index] = value * csScale; }

    // Get output value for a specific index in csSpout array
    cs_float getSpOut(int index)
    {
        auto spout = csound->GetSpout();
        if (spout)
            return spout[index] / csScale;

        return 0;
    }

    // Check if CSD compiled without error
    bool csdCompiledWithoutError() { return csCompileResult == 0 ? true : false; }

    // Get the KSMPS value
    int getKsmps() { return csdKsmps; }

    // Stop processing
    void stopProcessing() { csCompileResult = -1; }

    // init and set up parameter
    void initParameter(nlohmann::json &w);

    // Get a parameter channel by index
    ParameterChannel &getParameterChannel(int index) { return parameterChannels[index]; }

    // Get the number of parameters
    int getNumberOfParameters() { return numberOfParameters; }

    // Get the widgets
    std::vector<nlohmann::json> &getWidgets() { return widgets; }

    // Thread-safe version that returns a copy
    std::optional<nlohmann::json> getWidgetCopyById(const std::string &channel);
    
    // Thread-safe atomic update: read-modify-write
    bool updateWidget(const std::string &channel, std::function<void(nlohmann::json&)> modifier);
    // Update widget with JSON object
    const std::string updateWidgetState(nlohmann::json j);
    // Get the index for a parameter channel by name
    size_t getIndexForParamChannel(std::string name);

    // Set control channel value
    void setControlChannel(const std::string channel, cs_float value);

    // Set string channel data
    void setStringChannel(const std::string channel, std::string data);

    // Returns number of plugin paremters - even though lots of widgets have channels, only a select few can be plugin
    // parameters
    static int getNumberOfParameters(const std::string &csdFile);

    // Returns the current number of parameters registered
    int getCurrentParameterCount();

    // Return the channel config string, e.g., 'Stereo:2|2'
    static const std::string getIOChannalConfig(const std::string &csdFile);

    // Utility script to remove control characters from string - needed to santise Cabbage code going to JS
    static std::string removeControlCharacters(const std::string &input);

    // Process Csound console messages
    void processCsoundMessages();

    // Return a JS script that will trigger a widget's properties to be updated
    static std::string getUpdatedWidgetJsonStr(const std::string &channel, std::string data, bool includeValue = false);
    static std::string getUpdatedWidgetJsonStr(const std::string &channel, float value);

    // These two methods return combine with getWidgetIdentifierUpdateScript() to return a JS method
    // that packs samples for a given table
    void updateFunctionTable(CabbageOpcodeData data, nlohmann::json &jsonObj);
    static void setTableJSON(std::string channel, std::vector<cs_float> samples, nlohmann::json &jsonObj);

    // Initialise genTable widgets by loading audio files specified in their file property
    void initialiseGenTableWidgets();

    // Extracts the primary channel name from a widget, handling both new and legacy schemas.
    std::string extractChannelName(const nlohmann::json &widget);

    // Queue automatic updates for genTable widgets with tableNumber > 0 after Csound starts
    void queueGenTableUpdates();

    // Returns a script that will update a csoundoutput widget
    const std::string getCsoundOutputUpdateScript(const std::string &output);

    // Setup reserved channel
    void setReservedChannels();

    // Build a Csound UDS struct definition and global instance for all reserved channels
    std::string createGlobalStruct();

    // Get full range value from widget
    static float remap(double n, double start1, double stop1, double start2, double stop2);
    float getFullRangeValue(std::string channel, float normalValue);

    // Check if a widget has a specific channel (searches id then channels array)
    static bool hasChannel(const nlohmann::json &widget, const std::string &channel);

    moodycamel::ConcurrentQueue<CabbageOpcodeData> opcodeData;

    //=====================================================================================
    // Updates the channel cache with new data. This is called by the opcodes.
    //=====================================================================================
    void updateChannelCache(const CabbageOpcodeData &data);

    //=====================================================================================
    // Checks if the value is different from the cached value
    //=====================================================================================
    bool isValueDifferent(const CabbageOpcodeData &data);

    //=====================================================================================
    // Flushes the channel cache to the opcodeData queue. This is called at the end of
    // the processing block.
    //=====================================================================================
    void flushChannelCache();

    std::string getCompileErrors() { return compileErrors; }

    void displayAndClearCompileErrors()
    {
        if (compileErrors.length() > 0)
        {
            lattice::logError << compileErrors.c_str();
            compileErrors.clear();
        }
    }

    CabbageProcessor &getProcessor() { return processor; }

    //=====================================================================================
    // State Management Utilities - Used by both opcodes and CabbageProcessor
    //=====================================================================================
    
    // Save complete widget JSON data to state
    // isPresetSave: true for DAW preset saves (checks persistence.preset), false for session saves (checks persistence.session)
    nlohmann::json saveWidgetJsonData(bool isPresetSave = true);

    // Save widget channel data (id + channels array with range.value) for compact state storage
    // Widgets in fullJsonIds get full JSON; others get channel data only. If allChannelDataOnly=true, all get channel data.
    nlohmann::json saveWidgetChannelData(const std::unordered_set<std::string>& fullJsonIds, bool allChannelDataOnly);

    // Load complete widget state from JSON and update all systems
    // isPresetLoad: true for DAW preset loads (checks persistence.preset), false for session loads (checks persistence.session)
    void loadWidgetState(const nlohmann::json &state, bool isPresetLoad = true);

    //=====================================================================================
    // WebView Command Processing - Central handler for UI messages
    //=====================================================================================
    
    // Process commands from webview (used by both plugin and CabbageApp)
    // Returns true if command was handled, false if it needs environment-specific handling
    bool processWebViewCommand(const nlohmann::json &message);

    // Handle parameter update from UI - validates, normalizes, and applies changes
    // Returns gesture string for plugin automation, or empty string on failure/for standalone
    std::string handleParameterUpdate(const nlohmann::json &message);

    // Manage total number of samples
    long long getTotalSamples() const { return totalSamplesCounter.load(); }
    void incrementTotalSamples(){   totalSamplesCounter.fetch_add(1);   }

  private:
    void addOpcodes();

    bool hasCsoundOutputWidget() const;

    // Determine the number of input and output channels from channelConfig or CSD file
    std::pair<int, int> determineChannelConfiguration(const std::string& csdFile);

    // Helper function that handles the actual searching (internal use only)
    std::optional<std::reference_wrapper<nlohmann::json>> findWidgetInArray(nlohmann::json &jsonArray,
                                                                            const std::string &channel);

    // Legacy method - returns reference wrapper (internal use only, prefer getWidgetCopyById or updateWidget)
    std::optional<std::reference_wrapper<nlohmann::json>> getWidgetFromId(std::vector<nlohmann::json> &widgets,
                                                                            const std::string &channel);
    
    // Overload for nlohmann::json array (internal use only)
    std::optional<std::reference_wrapper<nlohmann::json>> getWidgetByChannel(nlohmann::json &widgets,
                                                                             const std::string &channel);
    
    int numberOfParameters = 0;
    std::vector<ParameterChannel> parameterChannels;
    std::string csoundOutput = {};
    int csCompileResult = -1;
    int csdKsmps = 0;
    cs_float csScale = 0.0;
    cs_float *csSpin = nullptr;
    double sampleRate = 44100;
    std::string compileErrors;
    std::unique_ptr<Csound> csound;
    CabbageProcessor &processor;
    std::atomic<bool> shuttingDown {false};
    mutable std::mutex channelCacheMutex;
    std::unordered_map<std::string, CabbageOpcodeData> channelCache;
    // Per-block dirty tracking, split by message kind. Value updates (k-rate,
    // e.g. cabbageSetValue) must never clobber pending identifier updates
    // (e.g. cabbageSet "bounds.left") for the same channel within one block,
    // and vice versa. Both sets are cleared on every flushChannelCache().
    std::unordered_set<std::string> dirtyValueChannels;
    std::unordered_set<std::string> dirtyIdentifierChannels;
    std::atomic<long long> totalSamplesCounter {0};
    bool csoundOutputEnabled = false;
};
} // namespace cabbage
