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

#include "CabbageProcessor.h"
#include <iostream>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <thread>
#include "CabbageUtils.h"

namespace
{
// The listbox value is an index into its items, so the channel range must always cover
// the current item list. Unlike comboBox/optionButton the range normally already exists
// (it comes from the widget's own defaults), so it is kept in sync rather than only
// filled in when missing.
void syncListBoxRangeWithItems(const nlohmann::json &widget, nlohmann::json &channel)
{
    if (!widget.contains("items") || !widget["items"].is_array() || widget["items"].empty())
        return;

    const float maxIndex = static_cast<float>(widget["items"].size() - 1);

    if (!channel.contains("range") || !channel["range"].is_object())
    {
        channel["range"] = {
            {"min", 0.0f},
            {"max", maxIndex},
            {"defaultValue", 0.0f},
            {"increment", 1.0f},
            {"skew", 1.0f}
        };
        return;
    }

    auto &range = channel["range"];
    range["min"] = 0.0f;
    range["max"] = maxIndex;

    if (!range.contains("increment") || !range["increment"].is_number())
        range["increment"] = 1.0f;
    if (!range.contains("skew") || !range["skew"].is_number())
        range["skew"] = 1.0f;

    for (const char *key : {"defaultValue", "value"})
    {
        if (range.contains(key) && range[key].is_number())
            range[key] = std::clamp(range[key].get<float>(), 0.0f, maxIndex);
    }
}
} // namespace

//========================================================================================
pluginType *LatticeProcessorPluginFactory::createPlugin(const clap_host *host)
{
    // create a new instance of CabbageProcessor
    auto *processor = new CabbageProcessor();
    return new pluginType(host, *processor);
}
//========================================================================================
// CabbageProcessor Constructor
// =============================
// DESIGN: Single Source of Truth for CSD File Path
//
// The CSD file path is cached in cabbage::File via setupRootDirectory().
// All code should use cabbage::File::getCsdFileAndPath() to retrieve the path.
//
// Flow:
// 1. Call setupRootDirectory() to determine root path, cache CSD path, and handle cabz extraction
// 2. Set mount point based on cabz or root path
// 3. All subsequent calls (WidgetDescriptors, parseCsdForWidgets, etc.) use the cached path
//========================================================================================
CabbageProcessor::CabbageProcessor(std::string csdFile, std::string config)
#if LATTICE_HAS_ARA
    : lattice::AraProcessor<CabbageProcessor>()
#else
    : lattice::Processor()
#endif
    , cabbage(*this)
{
    channelConfig = config;

    // Setup root directory and handle cabz extraction
    auto [mountPoint, cabzTemp] = cabbage::File::setupRootDirectory(csdFile);
    setMountPoint(mountPoint);
    cabzTempDir = cabzTemp;

#ifdef CabbagePro
    // Increment reference count for this instance if we have a temp dir
    if (!cabzTempDir.empty())
    {
        cabbage::File::incrementTempDirRef(cabzTempDir);
    }
#endif

#ifndef CabbageApp
    initialiseAudioEngine();
#endif
    initialiseAraCompanion();
}

//========================================================================================
// Starts the ARA worker thread (plugin builds) or launches standalone analysis.
// No-op when neither LATTICE_HAS_ARA nor CabbageApp is defined.
//========================================================================================
void CabbageProcessor::initialiseAraCompanion()
{
#if LATTICE_HAS_ARA
    startAraWorker();
#endif

#ifdef CabbageApp
    if (auto json = cabbage::File::parseCabbageSection(cabbage::File::getCsdFileAndPath()))
    {
        auto testFiles = cabbage::Utils::getTopLevelProperty<std::vector<std::string>>(*json, "ara.testFiles");
        if (testFiles && !testFiles->empty())
        {
            auto csdDir = lattice::File::getParentDirectory(cabbage::File::getCsdFileAndPath());
            araTestThread = std::thread([this, files = std::move(*testFiles), dir = std::move(csdDir)]() {
                for (const auto& f : files)
                {
                    std::filesystem::path filePath(f);
                    if (filePath.is_relative())
                        filePath = std::filesystem::path(dir) / filePath;
                    performAraAnalysisFromFile(filePath.lexically_normal().string());
                }
            });
        }
    }
#endif
}

//========================================================================================
// Initialise (or re-initialise) the Csound engine.
// On first call: sets up Csound, buses, parameters, editor size, and idle thread.
// On SR change:  saves UI state, tears down Csound, rebuilds it with the new rate,
//               then restores UI state so the user never sees a reset.
// NOTE: Must be called while audioEngineMutex is held exclusively (i.e. from prepareToPlay).
//========================================================================================
void CabbageProcessor::initialiseAudioEngine()
{
    const bool isReinit = cabbage.isEngineRunning();
    nlohmann::json savedState;

    // On re-init, detect a config change by comparing the active Lattice channel
    // counts against our stored totals.  The host has already called
    // selectAudioPortsConfig(id) so getChannelConfig() reflects the new selection.
    const bool configChanged = isReinit &&
        (getChannelConfig().getTotalNumInputChannels()  != totalNumInputs ||
         getChannelConfig().getTotalNumOutputChannels() != totalNumOutputs);

    // Register named audio port configs on first init only, BEFORE setupCsound().
    // This ensures the host can query audioPortsCount() even if Csound fails to
    // compile (which would cause an early return below, leaving configs empty).
    // On re-init the host has already called selectAudioPortsConfig(id); the
    // configs vector is still intact — re-calling addChannels() would duplicate them.
    if (!isReinit)
    {
        addChannels(channelConfig);
    }

    // Configure the file logger before setupCsound() so diagnostics emitted
    // during Csound compile/start (which can hang) are captured to disk.
    if (!isReinit)
    {
        if (auto json = cabbage::File::parseCabbageSection(cabbage::File::getCsdFileAndPath()))
        {
#ifndef CabbageApp
            configureLogger(*json);
#endif
        }
    }

    if (isReinit)
    {
        // Snapshot all live Csound channel values before we destroy the engine
        savedState = cabbage.saveWidgetJsonData(false);
        // Stop the idle thread — it accesses Csound state and must not run during teardown
        stopIdleThread();
        cabbage.teardownCsound();
    }

    if (!cabbage.setupCsound())
    {
        auto errors = cabbage.getCompileErrors();
        lattice::logError << "COMPILE ERRORS:\n" << errors;
        compileErrorHtml = generateErrorPageHtml(errors);
        hasCompileErrors = true;
        lattice::logDebug << "Generated error HTML, length: " << compileErrorHtml.length();
        setEditorSize(550, 350);
        return;
    }

    if (configChanged)
    {
        // The host switched to a different config — sync our stored totals.
        auto ioConfig = getChannelConfig();
        totalNumInputs  = ioConfig.getTotalNumInputChannels();
        totalNumOutputs = ioConfig.getTotalNumOutputChannels();
        matchingNumInputsOutputs = totalNumInputs == totalNumOutputs;
    }

    if (!isReinit)
    {
        // One-time setup: parameters and UI metadata (host-facing, must not repeat).
        setWebViewSendFunctionName("postMessage");
        addParameters();

        if (auto json = cabbage::File::parseCabbageSection(cabbage::File::getCsdFileAndPath()))
        {
            auto w = cabbage::Utils::getFormProperty<int>(*json, "size.width");
            auto h = cabbage::Utils::getFormProperty<int>(*json, "size.height");
            if (w.has_value() && h.has_value())
            {
                setEditorSize(w.value(), h.value());
                cabbage.setControlChannel("SCREEN_WIDTH", w.value());
                cabbage.setControlChannel("WINDOW_WIDTH", w.value());
                cabbage.setControlChannel("WINDOW_HEIGHT", h.value());
                cabbage.setControlChannel("SCREEN_HEIGHT", h.value());
            }

            // Where should the plugin UI get its DPI scale factor from?
            // "host" (default) uses the scale factor sent by the host and falls
            // back to the OS reported DPI. "os" always uses the OS reported
            // DPI and ignores whatever the host sends.
            if (auto scaleSource =
                    cabbage::Utils::getFormProperty<std::string>(*json, "scaleSource"))
            {
                if (scaleSource.value() == "os")
                    setScaleSource(lattice::Processor::ScaleSource::OS);
                else
                    setScaleSource(lattice::Processor::ScaleSource::Host);
            }
        }

    }
    else if (!savedState.is_null())
    {
        // SR reinit: restore all channel values so the UI is unchanged from the user's perspective
        // false = session load, checks persistence.session
        cabbage.loadWidgetState(savedState, false);
    }

    startOnIdle();
}

CabbageProcessor::~CabbageProcessor()
{
    suspendProcessing();

    // Drain any pending operations from queues before stopping threads
    // This prevents race conditions where the audio thread might try to
    // access the queue while it's being destroyed
    allowDequeuing = false;

    // Give any in-flight queue operations time to complete
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    stopIdleThread();

#if LATTICE_HAS_ARA
    stopAraWorker();
#endif

#ifdef CabbageApp
    if (araTestThread.joinable())
        araTestThread.join();
#endif

#ifdef CabbagePro
    // Decrement reference count for temp directory - will clean up only if this is the last instance
    if (!cabzTempDir.empty())
    {
        cabbage::File::decrementTempDirRef(cabzTempDir);
    }
#endif
}

//========================================================================================
// Configure logger from the top-level JSON (plugin mode only)
//========================================================================================
void CabbageProcessor::configureLogger(const nlohmann::json& json)
{
    auto loggerEnabled = cabbage::Utils::getTopLevelProperty<bool>(json, "logger.enabled");
    if (!loggerEnabled.has_value() || !loggerEnabled.value())
        return;

    auto logFile = cabbage::Utils::getTopLevelProperty<std::string>(json, "logger.file");
    if (!logFile.has_value() || logFile->empty())
    {
        lattice::logError << "Logger enabled but no file path specified";
        return;
    }

    auto replace = cabbage::Utils::getTopLevelProperty<bool>(json, "logger.replace");

    // Resolve log file path relative to CSD file if not absolute
    std::string logFilePath = logFile.value();
    if (!std::filesystem::path(logFilePath).is_absolute())
    {
        auto csdDir = lattice::File::getParentDirectory(cabbage::File::getCsdFileAndPath());
        logFilePath = lattice::File::joinPath(csdDir, logFilePath);
    }

    // Ensure parent directory exists
    std::filesystem::path parentDir = std::filesystem::path(logFilePath).parent_path();
    if (!parentDir.empty() && !std::filesystem::exists(parentDir))
    {
        try
        {
            std::filesystem::create_directories(parentDir);
        }
        catch (const std::exception& e)
        {
            std::cerr << "Failed to create log directory: " << parentDir << " - " << e.what() << std::endl;
        }
    }

    // If replace=true, delete existing log file before setting
    if (replace.value_or(false) && lattice::File::exists(logFilePath))
        std::remove(logFilePath.c_str());

    try
    {
        lattice::Logger::getInstance().setLogFile(logFilePath);
        lattice::logDebug << "Logger configured: " << logFilePath
                         << " (replace=" << (replace.value_or(false) ? "true" : "false") << ")";
    }
    catch (const std::exception& e)
    {
        std::cerr << "Failed to configure logger: " << e.what() << std::endl;
    }
}

//========================================================================================
// Add channels based on channelConfig property
//========================================================================================
void CabbageProcessor::addChannels(const std::string &config)
{
    auto file = cabbage::File::getCsdFileAndPath();
    if (!lattice::File::exists(file))
    {
        lattice::logError << "addChannels: CSD file not found: '" << file
                          << "' — falling back to default stereo I/O";
        // Add a default stereo config so the host can at least instantiate the plugin.
        // setupCsound() will subsequently fail and display an error page.
        addAudioPortsConfig("Stereo", "2", "2");
        totalNumInputs  = 2;
        totalNumOutputs = 2;
        matchingNumInputsOutputs = true;
        return;
    }

    // Get the semicolon-separated config string: "Name:ins|outs;Name2:ins2|outs2"
    const auto configStr = config.empty()
        ? cabbage::Engine::getIOChannalConfig(file)
        : config;

    // Parse and register each named config with the Lattice audio ports API
    std::istringstream ss(configStr);
    std::string entry;
    while (std::getline(ss, entry, ';'))
    {
        auto colon = entry.find(':');
        if (colon == std::string::npos) continue;
        const std::string name = entry.substr(0, colon);
        const std::string io   = entry.substr(colon + 1);
        auto pipe = io.find('|');
        if (pipe == std::string::npos) continue;
        addAudioPortsConfig(name, io.substr(0, pipe), io.substr(pipe + 1));
    }

    activeChannelConfig = configStr;

    // Update totals from the now-active (first/default) config
    auto ioConfig = getChannelConfig();
    totalNumInputs  = ioConfig.getTotalNumInputChannels();
    totalNumOutputs = ioConfig.getTotalNumOutputChannels();
    matchingNumInputsOutputs = totalNumInputs == totalNumOutputs;
}

//========================================================================================
// Add parameters based on widget declarations
//========================================================================================
void CabbageProcessor::addParameters()
{
    cabbage::ScopedPhaseTimer addParamsTimer("addParameters");
    lattice::logDebug << "=== Starting addParameters() ===";
    lattice::logDebug << "Total widgets: " << cabbage.getWidgets().size();

    for (auto &w : cabbage.getWidgets())
    {
        std::string widgetType = w.contains("type") ? w["type"].get<std::string>() : "unknown";
        bool hasChildren = w.contains("children");
        bool childrenIsArray = hasChildren && w["children"].is_array();
    

        if (hasChildren && !childrenIsArray)
        {
            lattice::logError << "Widget '" << widgetType << "' has a 'children' property but it's not an array. "
                              << "Children must be defined as an array, e.g., \"children\": [{...}]. "
                              << "Found type: " << w["children"].type_name();
        }

        addParametersForWidget(w);

        // ALWAYS check for child widgets, even if parent is not automatable
        // (containers like image, groupbox are not automatable but their children might be)
        if (w.contains("children") && w["children"].is_array())
        {
            for (auto &child : w["children"])
            {
                // Work directly with the child widget, not a temporary copy
                std::string childType = child.contains("type") ? child["type"].get<std::string>() : "unknown";
                addParametersForWidget(child);

                // Recursively process grandchildren
                if (child.contains("children") && child["children"].is_array())
                {
                    for (auto &grandchild : child["children"])
                    {
                        std::string grandchildType =
                            grandchild.contains("type") ? grandchild["type"].get<std::string>() : "unknown";
                        addParametersForWidget(grandchild);
                    }
                }
            }
        }
    }

    lattice::logDebug << "=== Finished addParameters() ===";
}

//========================================================================================
// Add parameter for a single widget if it meets the criteria. Parameters must be set
// up to comminucate on an id provided by the widget.channels array object. The top level
// widget.id property is used for UI updating only.
//========================================================================================
void CabbageProcessor::addParametersForWidget(nlohmann::json &w)
{
    std::string widgetType = w.contains("type") ? w["type"].get<std::string>() : "unknown";
    std::string widgetId = w.contains("id") && w["id"].is_string() ? w["id"].get<std::string>() : "unknown";

    // Check for automatable - expect boolean true
    bool isAutomatable = w.contains("automatable") && w["automatable"].is_boolean() && w["automatable"].get<bool>();

    // Ensure default ranges are set if missing
    cabbage::Parser::assignDefaultRangesToChannels(w);

    // Check if channel type is numeric (default) or explicitly set to number
    bool isNumericChannel = true;
    if (w.contains("channels") && w["channels"].is_array() && !w["channels"].empty()) {
        auto& firstChannel = w["channels"][0];

        if (firstChannel.contains("type") && firstChannel["type"].is_string()) {
            std::string channelType = firstChannel["type"].get<std::string>();
            isNumericChannel = (channelType == "number");
        }
    }

    lattice::logDebug << "addParametersForWidget: type=" << widgetType << ", id=" << widgetId
                     << ", automatable=" << isAutomatable << ", isNumericChannel=" << isNumericChannel;

    if (isAutomatable && isNumericChannel)
    {
        try
        {
            // New schema: channels array
            if (w.contains("channels") && w["channels"].is_array())
            {
                int currentIndex = cabbage.getCurrentParameterCount();
                for (auto &ch : w["channels"])
                {
                    if (!ch.contains("id") || !ch["id"].is_string())
                        continue;

                    const std::string channel = ch["id"].get<std::string>();
                    // Use label.name for DAW display if provided, otherwise fall back to channel id.
                    const bool hasLabelObj = ch.contains("label") && ch["label"].is_object();
                    const std::string paramLabel = (hasLabelObj && ch["label"].contains("name") && ch["label"]["name"].is_string())
                                                       ? ch["label"]["name"].get<std::string>()
                                                       : channel;
                    const std::string paramPrefix = (hasLabelObj && ch["label"].contains("prefix") && ch["label"]["prefix"].is_string())
                                                        ? ch["label"]["prefix"].get<std::string>() : std::string{};
                    const std::string paramSuffix = (hasLabelObj && ch["label"].contains("suffix") && ch["label"]["suffix"].is_string())
                                                        ? ch["label"]["suffix"].get<std::string>() : std::string{};
                    const std::string event = ch.contains("event") && ch["event"].is_string()
                                                  ? ch["event"].get<std::string>()
                                                  : std::string("valueChanged");
//                    const bool isClickEvent = (event.find("mousePress") == 0) || (event.find("mouseRelease") == 0) ||
//                                              (event.find("mouseClick") == 0);

                    // For comboBox/optionButton, create default range based on items if not provided
                    const std::string widgetType = w["type"].get<std::string>();
                    if ((widgetType == "comboBox" || widgetType == "optionButton") && !ch.contains("range"))
                    {
                        size_t itemCount = (w.contains("items") && w["items"].is_array()) ? w["items"].size() : 3;
                        bool hasIndexOffset =
                            w.contains("indexOffset") && w["indexOffset"].is_boolean() && w["indexOffset"].get<bool>();
                        ch["range"] = {
                            {"min", hasIndexOffset ? 1 : 0},
                            {"max", static_cast<int>(hasIndexOffset ? itemCount : itemCount - 1)},
                            {"defaultValue", hasIndexOffset ? 1 : 0},
                            {"increment", 1},
                            {"skew", 1}
                        };
                    }

                    if (widgetType == "listBox")
                        syncListBoxRangeWithItems(w, ch);

                    const float minVal = ch["range"]["min"].get<float>();

                    // Determine max value - widgets send denormalized index values for comboBox/optionButton
                    float maxVal = ch["range"]["max"].get<float>();
                    float minValAdjusted = minVal;
                    if (widgetType == "comboBox" || widgetType == "optionButton")
                    {
                        bool hasIndexOffset =
                            w.contains("indexOffset") && w["indexOffset"].is_boolean() && w["indexOffset"].get<bool>();
                        size_t itemCount = (w.contains("items") && w["items"].is_array()) ? w["items"].size() : 3;
                        if (hasIndexOffset)
                        {
                            minValAdjusted = 1.0f;
                            maxVal = static_cast<float>(itemCount);
                        }
                        else
                        {
                            minValAdjusted = 0.0f;
                            maxVal = static_cast<float>(itemCount - 1);
                        }
                    }

                    const float defVal = ch["range"]["defaultValue"].get<float>();
                    const float incVal = ch["range"]["increment"].get<float>();
                    const float skewVal = ch["range"]["skew"].get<float>();

                    // Use range.value if set, otherwise use defaultValue for initial Csound channel
                    float initialValue = defVal;
                    if (ch["range"].contains("value") && ch["range"]["value"].is_number())
                    {
                        initialValue = ch["range"]["value"].get<float>();
                    }
                    else
                    {
                        // Store defaultValue as value for consistency
                        ch["range"]["value"] = defVal;
                    }

                    // CRITICAL: Parameter value field must be NORMALIZED [0-1] for internal routing
                    // We need to normalize the default value before passing to constructor
                    float normalizedDefault = (defVal - minValAdjusted) / (maxVal - minValAdjusted);
                    if (skewVal != 1.0f && normalizedDefault > 0.0f) {
                        normalizedDefault = std::pow(normalizedDefault, 1.0f / skewVal);
                    }

                    lattice::logDebug << "Creating parameter '" << paramLabel << "' (channel: " << channel << "): min=" << minValAdjusted
                                     << ", max=" << maxVal << ", default=" << defVal
                                     << ", normalizedDefault=" << normalizedDefault << ", initial=" << initialValue
                                     << ", inc=" << incVal << ", skew=" << skewVal;

                    // Use label for DAW display, store channel ID separately for Csound lookup
                    addParameter({paramLabel, minValAdjusted, maxVal, normalizedDefault, incVal, skewVal, paramPrefix, paramSuffix});
                    parameterChannelIds.push_back(channel);

                    // Set initial value in Csound using range.value (or defaultValue if not set)
                    cabbage.setControlChannel(channel, initialValue);

                    // Store parameterIndex in each channel object
                    ch["parameterIndex"] = currentIndex;
                    lattice::logDebug << "Added parameter for channel '" << channel << "' (label: '" << paramLabel
                                      << "') with parameterIndex " << currentIndex << " and default value of : " << defVal;
                    currentIndex++;
                }

                return;
            }
        }
        catch (nlohmann::json::exception &e)
        {
            lattice::logError << "JSON error while adding parameter for widget: " << e.what() << "\n" << w.dump(4);
            // Don't crash - just skip this widget and continue
        }
    }
    else
    {
        // Non-automatable widget - still needs Csound channel created for cabbageGetValue/cabbageSetValue
        try
        {
            if (w.contains("channels") && w["channels"].is_array())
            {
                const std::string widgetType = w.contains("type") && w["type"].is_string()
                    ? w["type"].get<std::string>() : "";

                lattice::logDebug << "Processing non-automatable widget type: " << widgetType;

                for (auto &ch : w["channels"])
                {
                    if (!ch.contains("id") || !ch["id"].is_string())
                        continue;

                    const std::string channel = ch["id"].get<std::string>();
                    lattice::logDebug << "  Processing channel: " << channel << " in widget type: " << widgetType;

                    // For comboBox/optionButton, create default range based on items if not provided
                    if ((widgetType == "comboBox" || widgetType == "optionButton") && !ch.contains("range"))
                    {
                        size_t itemCount = (w.contains("items") && w["items"].is_array()) ? w["items"].size() : 3;
                        bool hasIndexOffset =
                            w.contains("indexOffset") && w["indexOffset"].is_boolean() && w["indexOffset"].get<bool>();
                        ch["range"] = {
                            {"min", hasIndexOffset ? 1 : 0},
                            {"max", static_cast<int>(hasIndexOffset ? itemCount : itemCount - 1)},
                            {"defaultValue", hasIndexOffset ? 1 : 0},
                            {"increment", 1},
                            {"skew", 1}
                        };
                    }

                    if (widgetType == "listBox")
                        syncListBoxRangeWithItems(w, ch);

                    // Check channel type - default to "number" if not specified
                    std::string channelType = "number";
                    if (ch.contains("type") && ch["type"].is_string()) {
                        channelType = ch["type"].get<std::string>();
                    }

                    // Create appropriate channel type
                    if (channelType == "string")
                    {
                        // For string channels, set empty string as default
                        cabbage.getCsound()->SetStringChannel(channel.c_str(), "");
                        lattice::logDebug << "    Created string channel for non-automatable widget '" << channel << "'";
                    }
                    else
                    {
                        // For numeric channels, use default value from range (or 0 if no range)
                        float defVal = 0.0f;
                        if (ch.contains("range") && ch["range"].contains("defaultValue") && ch["range"]["defaultValue"].is_number())
                        {
                            defVal = ch["range"]["defaultValue"].get<float>();
                        }
                        cabbage.setControlChannel(channel, defVal);

                        // Also store value in range for state saving
                        if (ch.contains("range"))
                        {
                            ch["range"]["value"] = defVal;
                        }

                        lattice::logDebug << "Created numeric channel for non-automatable widget '" << channel
                                          << "' with default value " << defVal;
                    }
                }
            }
            // Legacy schema: single id
            else if (w.contains("id") && w["id"].is_string())
            {
                const std::string channel = w["id"].get<std::string>();
                float defVal = 0.0f;
                if (w.contains("value"))
                {
                    if (w["value"].is_number())
                        defVal = w["value"].get<float>();
                    else if (w["value"].is_boolean())
                        defVal = w["value"].get<bool>() ? 1.0f : 0.0f;
                }

                cabbage.setControlChannel(channel, defVal);
                lattice::logDebug << "Created channel for non-automatable widget (legacy) '" << channel
                                  << "' with default value " << defVal;
            }
        }
        catch (nlohmann::json::exception &e)
        {
            lattice::logError << "JSON error while creating channel for non-automatable widget: " << e.what();
        }
    }
}

//========================================================================================
// Main processing function - this is called by the CLAP process function
//========================================================================================
void CabbageProcessor::process(float **inputs, float **outputs, std::size_t blockSize)
{
    if (!processingEnabled.load(std::memory_order_relaxed))
        return;

#if LATTICE_HAS_ARA
    flushAraEvents();
#endif

    // Try to acquire a shared lock without blocking. If prepareToPlay() holds the
    // exclusive lock (rebuilding Csound due to an SR change) we must not touch any
    // Csound state — return silence for this block and try again next time.
    std::shared_lock lock(audioEngineMutex, std::try_to_lock);
    if (!lock.owns_lock())
        return;

    // Get transport info from host and update Csound channels
    auto transport = getTransportInfo();
    
    // Update transport-related control channels
    cabbage.setControlChannel("HOST_BPM", transport.tempo);
    cabbage.setControlChannel("HOST_IS_PLAYING", transport.isPlaying ? 1.0f : 0.0f);
    cabbage.setControlChannel("HOST_IS_RECORDING", transport.isRecording ? 1.0f : 0.0f);
    cabbage.setControlChannel("HOST_TIME_IN_SECONDS", transport.songPosSeconds);
    cabbage.setControlChannel("HOST_TIME_IN_SAMPLES", transport.songPosSeconds * sampleRate);
    cabbage.setControlChannel("HOST_TIME_SIG_NUM", static_cast<float>(transport.timeSigNum));
    cabbage.setControlChannel("HOST_TIME_SIG_DENOM", static_cast<float>(transport.timeSigDenom));
    cabbage.setControlChannel("HOST_PPQ_POS", transport.barStart);
    
    // only process audio if Csound has compiled successfully.
    if (cabbage.csdCompiledWithoutError())
    {
        for (int i = 0; i < static_cast<int>(blockSize); i++, ++csndIndex)
        {
            if (csndIndex >= cabbage.getKsmps())
            {
                cabbage.performKsmps();
                csndIndex = 0;
            }

            // maintain an absolute sample counter so outgoing MIDI can be timestamped
            cabbage.incrementTotalSamples();
            // In cases where we have the same number of inputs/outputs we can read
            // and write in the same loop. In cases where we have a different number
            // of inputs/outputs we first iterate over the inputs, and then the outputs.
            // This adds a little overhead, hence this is only done when needed.
            if (matchingNumInputsOutputs)
            {
                for (int channel = 0; channel < totalNumOutputs; channel++)
                {
                    pos = csndIndex * totalNumOutputs;
                    cabbage.setSpIn(channel + pos, inputs[channel][i]);
                    // outputs[channel][i] = inputs[channel][i] + cabbage.getSpOut(channel + pos);
                    outputs[channel][i] = cabbage.getSpOut(channel + pos);
                }
            }
            else
            {
                // Process inputs first
                for (int inputChannel = 0; inputChannel < totalNumInputs; inputChannel++)
                {
                    pos = csndIndex * totalNumInputs; // Position in interleaved array
                    cabbage.setSpIn(inputChannel + pos, inputs[inputChannel][i]);
                }

                // Process outputs
                for (int outputChannel = 0; outputChannel < totalNumOutputs; outputChannel++)
                {
                    pos = csndIndex * totalNumOutputs; // Position in interleaved array
                    // Fill output buffer from Csound's processed output
                    outputs[outputChannel][i] = cabbage.getSpOut(outputChannel + pos);
                }
            }
        }

        // Flush channel cache once per audio block (not per sample!)
        cabbage.flushChannelCache();
    }
    else
    {
        // calling this once here in case errors are missed in vscode logger
        //        cabbage.displayAndClearCompileErrors();

        // zero outputs so we don't get unwanted signal when csound fails
        for (int i = 0; i < static_cast<int>(blockSize); i++)
            for (int channel = 0; channel < totalNumOutputs; channel++)
                outputs[channel][i] = 0;
    }
}

#ifdef CabbageApp
namespace {
// Strict UTF-8 validity check (RFC 3629): rejects overlongs, surrogates
// and code points past U+10FFFF, matching nlohmann::json's serializer.
bool isValidUtf8(const std::string &s)
{
    const auto *b = reinterpret_cast<const unsigned char *>(s.data());
    size_t i = 0, n = s.size();
    while (i < n)
    {
        unsigned char c = b[i];
        size_t len;
        if (c < 0x80) { ++i; continue; }
        else if (c >= 0xC2 && c <= 0xDF) len = 2;
        else if (c >= 0xE0 && c <= 0xEF) len = 3;
        else if (c >= 0xF0 && c <= 0xF4) len = 4;
        else return false;
        if (i + len > n) return false;
        uint32_t cp = 0;
        for (size_t k = 0; k < len; ++k)
        {
            unsigned char d = b[i + k];
            if (k > 0 && (d >> 6) != 0x2) return false;
            cp = (cp << 6) | (d & (k == 0 ? (len == 2 ? 0x1F : len == 3 ? 0x0F : 0x07) : 0x3F));
        }
        if ((len == 2 && cp < 0x80) || (len == 3 && cp < 0x800) || (len == 4 && cp < 0x10000) ||
            cp > 0x10FFFF || (cp >= 0xD800 && cp <= 0xDFFF))
            return false;
        i += len;
    }
    return true;
}

void collectBadStrings(const nlohmann::json &j, const std::string &path, std::ostringstream &os)
{
    if (j.is_string())
    {
        const std::string s = j.get<std::string>();
        if (!isValidUtf8(s))
        {
            static constexpr char hex[] = "0123456789ABCDEF";
            os << " [" << path << " len=" << s.size() << " hex=";
            const size_t n = std::min<size_t>(s.size(), 32);
            for (size_t k = 0; k < n; ++k)
                os << hex[(s[k] >> 4) & 0xF] << hex[s[k] & 0xF] << ' ';
            if (s.size() > 32) os << "...";
            os << "]";
        }
    }
    else if (j.is_object())
    {
        for (auto it = j.begin(); it != j.end(); ++it)
            collectBadStrings(it.value(), path + "/" + it.key(), os);
    }
    else if (j.is_array())
    {
        for (size_t k = 0; k < j.size(); ++k)
            collectBadStrings(j[k], path + "/" + std::to_string(k), os);
    }
}
} // namespace

void CabbageProcessor::logCorruptOpcodePayload(const CabbageOpcodeData &data)
{
    std::ostringstream os;
    os << "corrupt payload channel='" << data.channel << "'";
    collectBadStrings(data.cabbageJson, "$", os);
    // Full shape with invalid bytes substituted (never throws), so the log
    // shows which sibling values are intact around the corrupt one.
    try
    {
        os << " sanitized=" << data.cabbageJson.dump(-1, ' ', false, nlohmann::json::error_handler_t::replace);
    }
    catch (...)
    {
    }
    lattice::logError << os.str();
}

void CabbageProcessor::invokeHostCallback(const CabbageOpcodeData &data)
{
    if (!hostCallback)
        return;
    try
    {
        hostCallback(data);
    }
    catch (const std::exception &e)
    {
        lattice::logError << "hostCallback failed for channel '" << data.channel << "': " << e.what();
        logCorruptOpcodePayload(data);
    }
    catch (...)
    {
        lattice::logError << "hostCallback failed for channel '" << data.channel << "' (unknown exception)";
        logCorruptOpcodePayload(data);
    }
}
#endif

//========================================================================================
// Single populate trigger shared by the plugin updateUI() path and the
// CabbageApp (re)compile path. Iterates engine widgets and refreshes each
// configured one via runPopulateAsync (async scan, queue delivery).
// Widgets with no directory configured are skipped quietly (debug only):
// scanning is meaningless for defaults, and a warning per compile would be
// noise. runPopulateAsync still validates defensively.
//========================================================================================
void CabbageProcessor::triggerInitialPopulate()
{
    for (auto &w : cabbage.getWidgets())
    {
        if (!w.contains("populate") || !w["populate"].is_object())
            continue;

        std::string widgetChannel;
        if (w.contains("id") && w["id"].is_string())
        {
            widgetChannel = w["id"].get<std::string>();
        }
        else if (w.contains("channels") && w["channels"].is_array() && !w["channels"].empty() &&
                 w["channels"][0].contains("id") && w["channels"][0]["id"].is_string())
        {
            widgetChannel = w["channels"][0]["id"].get<std::string>();
        }

        if (widgetChannel.empty())
        {
            lattice::logDebug << "Skipping initial populate: widget has no channel id";
            continue;
        }

        const auto &pop = w["populate"];
        const bool hasDir =
            (pop.contains("directories") && pop["directories"].is_string() &&
             !pop["directories"].get<std::string>().empty()) ||
            (pop.contains("directories") && pop["directories"].is_array() &&
             std::any_of(pop["directories"].begin(), pop["directories"].end(), [](const nlohmann::json &dir) { return dir.is_string() && !dir.get<std::string>().empty(); }));
        if (!hasDir || !pop.contains("fileType"))
        {
            lattice::logDebug << "Skipping initial populate for '" << widgetChannel
                              << "': no directories configured";
            continue;
        }

        runPopulateAsync(widgetChannel, pop);
    }
}

//========================================================================================
// Runs a populate directory scan for a widget channel; see declaration.
//========================================================================================
void CabbageProcessor::runPopulateAsync(const std::string &widgetChannel, nlohmann::json populateConfig)
{
    // Normalize directories: convert single string to array for consistency
    if (populateConfig.contains("directories") && populateConfig["directories"].is_string()) {
        std::string singleDir = populateConfig["directories"].get<std::string>();
        populateConfig["directories"] = nlohmann::json::array({singleDir});
        lattice::logDebug << "Converted single string directories to array for widget: " << widgetChannel;
    }

    // Verify it has the required fields before processing - including non-empty directory strings.
    // Note: fileType presence (even empty) is enough here; processPopulateAsync
    // normalizes empty/missing filters to "match all".
    if (!(populateConfig.contains("directories") && populateConfig["directories"].is_array() &&
          !populateConfig["directories"].empty() &&
          std::any_of(populateConfig["directories"].begin(), populateConfig["directories"].end(), [](const nlohmann::json& dir) { return dir.is_string() && !dir.get<std::string>().empty(); }) &&
          populateConfig.contains("fileType")))
    {
        lattice::logError << "Populate config missing directories array or fileType for: " << widgetChannel;
        lattice::logError << "Current populate config: " << populateConfig.dump();
        return;
    }

    lattice::logDebug << "Triggering processPopulateAsync for widget: " << widgetChannel;

    // Process populate on background thread, then update widget with results
    cabbage::Parser::processPopulateAsync(widgetChannel, populateConfig,
        [this, widgetChannel](const nlohmann::json& result) {
            lattice::logDebug << "Populate callback received for widget: " << widgetChannel;
            lattice::logDebug << "Result contains items: " << (result.contains("items") ? "yes" : "no");
            if (result.contains("items")) {
                lattice::logDebug << "Items count: " << result["items"].size();
            }

            // Update widget with populated items (this runs on background thread)
            cabbage.updateWidget(widgetChannel, [result](nlohmann::json &j) {
                if (result.contains("items")) {
                    j["items"] = result["items"];
                    lattice::logDebug << "Updated widget items in internal state";
                }
                // Note: We DON'T update j["populate"] here to avoid triggering
                // another populate detection in onIdle (which would create an infinite loop)
            });

            // Enqueue widget update to be sent from idle thread
            // Create opcode data to trigger a widget update on the idle thread
            CabbageOpcodeData opcodeUpdate;
            opcodeUpdate.channel = widgetChannel;
            opcodeUpdate.type = CabbageOpcodeData::MessageType::Identifier;
            opcodeUpdate.identifier = "items";  // This will trigger a full widget update
            opcodeUpdate.cabbageJson = result;  // Contains the items
            opcodeUpdate.skipPopulateProcessing = true;  // CRITICAL: Prevent infinite loop

            // Enqueue to opcode queue so it's processed by onIdle
            cabbage.opcodeData.enqueue(opcodeUpdate);
            lattice::logDebug << "Enqueued widget update for idle thread processing";
        });
}

//========================================================================================
// onIdle function
//========================================================================================
void CabbageProcessor::onIdle()
{
    if (!isIdleThreadRunning())
        return;

#if LATTICE_HAS_ARA
    // Retry pending sources that were accessible but not yet analysed.
    // Use isMyAudioSource() first to avoid analysing sources from other instances
    // (araAccessibleSources is populated by the shared document controller, so it
    // can contain sources from every track that has this plugin). Fall back to
    // all unanalysed sources only when isMyAudioSource() finds nothing (hosts
    // that don't assign a PlaybackRenderer role).
    if (uiIsOpen)
    {
        std::vector<ARA::PlugIn::AudioSource*> toAnalyse;
        {
            std::lock_guard<std::mutex> lk(araSourcesMutex);
            for (auto* src : araAccessibleSources)
            {
                if (araAnalysedSources.count(src) == 0 && isMyAudioSource(src))
                    toAnalyse.push_back(src);
            }
            // Fallback: no PlaybackRenderer role assigned by host.
            if (toAnalyse.empty())
            {
                for (auto* src : araAccessibleSources)
                {
                    if (araAnalysedSources.count(src) == 0)
                        toAnalyse.push_back(src);
                }
            }
            for (auto* src : toAnalyse)
                araAnalysedSources.insert(src);
        }
        for (auto* src : toAnalyse)
        {
            lattice::logDebug << "ARA: onIdle retry — queuing source='" << (src ? src->getName() : "null") << "'";
            enqueueAraSource(src);
        }
    }
#endif

    cabbage.processCsoundMessages();

#ifndef CabbageApp
    if (uiIsOpen)
    {
#endif

#if defined(LINUX) && !defined(CabbageApp)
        nlohmann::json message;
        while (memoryQueue.receiveFromChild(message))
        {
            OnMessageFromWebView(message.dump(4).c_str());
        }
#endif

#ifndef CabbageApp
    }
#endif

     CabbageOpcodeData data;

    if (allowDequeuing)
    {
        // Dequeue all messages. For most channels we deduplicate (keep latest
        // per channel AND per message kind), but preserve Generic messages
        // and the special channel "cabbageSendMessageQ7mX2b" in FIFO order.
        // Kind-aware dedup matters because flushChannelCache() may emit both
        // a Value message and an Identifier message for the same channel in
        // one block (e.g. cabbageSetValue + cabbageSet on "gain" in the same
        // k-cycle) — collapsing those to one would drop an update.
        struct DedupKey
        {
            std::string channel;
            CabbageOpcodeData::MessageType type;
            bool operator==(const DedupKey &other) const
            {
                return channel == other.channel && type == other.type;
            }
        };
        struct DedupKeyHash
        {
            size_t operator()(const DedupKey &key) const
            {
                return std::hash<std::string>()(key.channel) ^ (std::hash<int>()(static_cast<int>(key.type)) << 1);
            }
        };
        std::unordered_map<DedupKey, CabbageOpcodeData, DedupKeyHash> latestMessages;
        std::vector<CabbageOpcodeData> preservedMessages;

        while (cabbage.opcodeData.try_dequeue(data))
        {
            if (data.type == CabbageOpcodeData::MessageType::Generic ||
                data.channel == "cabbageSendMessageQ7mX2b")
            {
                // Preserve every Generic/message-queue event in arrival order
                preservedMessages.push_back(data);
            }
            else
            {
                // For all other channels, keep only the latest message per kind
                latestMessages[{data.channel, data.type}] = data;
            }
        }

        // Build a processing list: first the preserved (FIFO), then the latest-per-channel
        std::vector<CabbageOpcodeData> messagesToProcess;
        messagesToProcess.reserve(preservedMessages.size() + latestMessages.size());
        for (const auto &m : preservedMessages)
            messagesToProcess.push_back(m);
        for (const auto &kv : latestMessages)
            messagesToProcess.push_back(kv.second);

        for (const auto &latestData : messagesToProcess)
        {
            // Make an explicit copy to ensure deep copy of JSON data
            CabbageOpcodeData dataCopy = latestData;

            // Generic messages are forwarded directly to the frontend and should
            // not be merged into widget state.
            if (dataCopy.type == CabbageOpcodeData::MessageType::Generic)
            {
#ifdef CabbageApp
                invokeHostCallback(dataCopy);
#else
                cabbage.processCsoundMessages();
                updateWidgetData(dataCopy);
#endif
                continue;
            }

            // Check if this message contains a populate update AND should be processed
            bool hasPopulateUpdate = dataCopy.cabbageJson.contains("populate") && !dataCopy.skipPopulateProcessing;

            cabbage.updateWidget(dataCopy.channel, [dataCopy](nlohmann::json &j) {
                cabbage::Parser::mergeJsonProperties(j, dataCopy.cabbageJson);
            });

            // If this update included populate data and should be processed, trigger async processing
            if (hasPopulateUpdate)
            {
                auto widgetOpt = cabbage.getWidgetCopyById(dataCopy.channel);
//                lattice::logDebug << "Checking widget for populate config: " << dataCopy.channel;
//                if (widgetOpt.has_value()) {
//                    if (widgetOpt->contains("populate")) {
//                        lattice::logDebug << "Widget has populate field, type: " << (*widgetOpt)["populate"].type_name();
//                    } else {
//                        lattice::logDebug << "Widget does NOT have populate field";
//                    }
//                }

                if (widgetOpt.has_value() && widgetOpt->contains("populate") && (*widgetOpt)["populate"].is_object())
                {
                    runPopulateAsync(dataCopy.channel, (*widgetOpt)["populate"]);
                }
                else
                {
                    lattice::logError << "Widget not found or populate not an object for: " << dataCopy.channel;
                    if (widgetOpt.has_value()) {
                        lattice::logError << "Widget JSON: " << widgetOpt->dump();
                    }
                }
            }


#ifdef CabbageApp
            invokeHostCallback(dataCopy);
#else
            cabbage.processCsoundMessages();
            updateWidgetData(dataCopy);
#endif
        }
    }
}

//========================================================================================
// this function will be called from the onIdle function if Csound has sent update messages
//========================================================================================
void CabbageProcessor::updateWidgetData(const CabbageOpcodeData &data)
{
    // Handle generic messages from cabbageSendMessage - send directly to frontend
    if (data.type == CabbageOpcodeData::MessageType::Generic)
    {
        sendWebViewMessage(data.cabbageJson);
        return;
    }

    // Handle batch updates from loadWidgetState
    if (data.channel == "BATCH-UPDATE-7f3d2a" && data.cabbageJson.contains("command") &&
        data.cabbageJson["command"] == "batchWidgetUpdate")
    {
//        lattice::logDebug << "Processing batch widget update with " << data.cabbageJson["widgets"].size() << " widgets";
        sendWebViewMessage(data.cabbageJson);
        return;
    }

    // For value-only updates, update the channel's range.value (not top-level value)
    if (data.type == CabbageOpcodeData::MessageType::Value)
    {
        float value = 0.0f;
        bool updated = cabbage.updateWidget(data.channel, [data, &value](nlohmann::json &j) {
            // Extract value from the message
            if (data.cabbageJson.contains("value") && data.cabbageJson["value"].is_number())
            {
                value = data.cabbageJson["value"].get<float>();
                
                // Update the first channel's range.value (proper location for values)
                if (j.contains("channels") && j["channels"].is_array() && !j["channels"].empty())
                {
                    auto& firstChannel = j["channels"][0];
                    if (firstChannel.contains("range") && firstChannel["range"].is_object())
                    {
                        firstChannel["range"]["value"] = value;
                    }
                }
            }
        });
        
        if (updated)
        {
            // Send proper JSON message like CabbageApp does
            nlohmann::json msg;
            msg["command"] = "widgetUpdate";
            msg["id"] = data.channel;
            msg["value"] = value;
            sendWebViewMessage(msg);
        }
    }
    else
    {
        // For full widget updates, send the complete widget JSON
        auto updatedWidgetJsonOpt = processOpcodeData(data);
        if (updatedWidgetJsonOpt.has_value())
        {
            auto &j = updatedWidgetJsonOpt.value();
            // Send proper JSON message like CabbageApp does
            nlohmann::json msg;
            msg["command"] = "widgetUpdate";
            msg["id"] = data.channel;
            msg["widgetJson"] = j.dump(); // Send as JSON string, consistent with updateUI()
            
            lattice::logDebug << "Sending full widget update to UI for channel: " << data.channel;
            sendWebViewMessage(msg);
        }
        else
        {
            lattice::logError << "processOpcodeData returned nullopt for channel: " << data.channel;
        }
    }
}

//========================================================================================
// Process opcode data and return the updated widget JSON if applicable
// Note: Value-only updates are now handled in updateWidgetData() to use the float overload
//========================================================================================
std::optional<nlohmann::json> CabbageProcessor::processOpcodeData(const CabbageOpcodeData &data)
{
    if (data.type == CabbageOpcodeData::MessageType::Identifier || data.type == CabbageOpcodeData::MessageType::Value)
    {
        auto widgetOpt = cabbage.getWidgetCopyById(data.channel);
        if (widgetOpt)
        {
            auto j = widgetOpt.value();
            if (j["type"].get<std::string>() == "genTable")
            {
                cabbage.updateFunctionTable(data, j);
            }
            cabbage::Parser::mergeJsonProperties(j, data.cabbageJson);
            return j;
        }
    }
    else if (data.type == CabbageOpcodeData::MessageType::Widget)
    {
        auto widgetOpt = cabbage.getWidgetCopyById(data.channel);
        if (widgetOpt)
        {
            lattice::logDebug << "A widget with channel: " << data.channel
                              << " already exists and cannot be overwritten.";
            return std::nullopt;
        }
        else
        {
            lattice::logDebug << "Creating widget: " << data.channel;

            // Get the widget type
            std::string widgetType = data.cabbageJson["type"].get<std::string>();

            // Get the default widget descriptor
            nlohmann::json newWidget = cabbage::WidgetDescriptors::get(widgetType);
            if (newWidget.is_null())
            {
                lattice::logError << "Unknown widget type: " << widgetType << " - cannot create widget";
                return std::nullopt;
            }

            // Update the widget with properties from the opcode
            cabbage::Parser::initialiseWidgetJson(newWidget, data.cabbageJson, cabbage.getWidgets().size());

            // Add the new widget to the widgets array
            cabbage.getWidgets().push_back(newWidget);

            // Debug: Check if widget can now be found
            auto testWidgetOpt = cabbage.getWidgetCopyById(data.channel);
            if (!testWidgetOpt)
            {
                lattice::logError << "Widget was added but cannot be found: " << data.channel;
            }

            return newWidget;
        }
    }

    return std::nullopt;
}

void CabbageProcessor::onIdleScheduler()
{
    while (isIdleRunning)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(16)); // ~60Hz update rate for smoother UI
        onIdle();
    }
}

// Start idle thread in a separate non-realtime thread
void CabbageProcessor::startOnIdle()
{
    isIdleRunning = true;
    idleThread = std::thread(&CabbageProcessor::onIdleScheduler, this);
}

void CabbageProcessor::stopIdleThread()
{
    isIdleRunning.store(false);
    // Give the idle thread time to exit its current onIdle() call
    // before we try to join it. This prevents race conditions where
    // the thread might be in the middle of a queue operation.
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    if (idleThread.joinable())
    {
        idleThread.join(); // Ensure the thread is joined before reset
    }
}
//========================================================================================
// Triggered when Cabbage is ready to start processing messages
//========================================================================================
void CabbageProcessor::onWebViewIsReady()
{
    lattice::logDebug << "onWebViewIsReady() called — thread: " << std::this_thread::get_id();

    // If there were compile errors, display the error page
    if (hasCompileErrors)
    {
        lattice::logDebug << "Displaying error page in webview (called from main thread)";
        setWebViewHtml(compileErrorHtml);
        return;
    }

#ifdef CabbageApp
    // For CabbageApp, don't call updateUI() here because:
    // 1. The webview sends cabbageIsReadyToLoad explicitly when ready
    // 2. setCabbageIsReady() will be called which enables the opcode queue
    // 3. Calling updateUI() here creates a race condition where widgets are sent
    //    both from the opcode queue (with table data) and from updateUI() (without table data)
    // 4. The updateUI() call after setCabbageIsReady() in onMessageFromWebView handles initial state
    lattice::logDebug << "onWebViewIsReady() called for CabbageApp - skipping updateUI(), waiting for cabbageIsReadyToLoad";
#else
    // For plugins, update UI immediately when webview is ready
    updateUI();
#endif

#if LATTICE_HAS_ARA
    // Call the base class to complete the ARA webview handshake.
    lattice::logDebug << "onWebViewIsReady() calling AraProcessor base";
    AraProcessor<CabbageProcessor>::onWebViewIsReady();
    lattice::logDebug << "onWebViewIsReady() base class call done";
#endif
}

void CabbageProcessor::setCabbageIsReady()
{
    lattice::logDebug << "setCabbageIsReady() called";
    uiIsOpen = true;
    allowDequeuing = true;

#ifndef CabbageApp
    // We update the UI each time the plugin window is shown
    lattice::logDebug << "Calling updateUI() from setCabbageIsReady()";
    updateUI();
    lattice::logDebug << "updateUI() call completed";
#endif

#if LATTICE_HAS_ARA
    // The document controller may have enabled sample access for sources before
    // this (the UI) instance had its window open, so those events were skipped
    // in araDidEnableSamplesAccess.  Trigger deferred analysis for any source
    // that still has access enabled now that the UI is ready.
    //
    // Prefer isMyAudioSource() to restrict analysis to sources that belong to
    // this instance. Some ARA hosts assign the plugin without a PlaybackRenderer
    // role, in which case getPlaybackRegions() returns empty and isMyAudioSource()
    // yields nothing — only then do we fall back to analysing all unanalysed
    // accessible sources (safe because uiIsOpen means this is always the UI
    // instance, never a background renderer).
    {
        std::vector<ARA::PlugIn::AudioSource*> toAnalyse;
        {
            std::lock_guard<std::mutex> lk(araSourcesMutex);
            lattice::logDebug << "ARA setCabbageIsReady: araAccessibleSources.size()=" << araAccessibleSources.size();

            // First pass: collect only sources that belong to this instance.
            for (auto* src : araAccessibleSources)
            {
                const bool mine     = isMyAudioSource(src);
                const bool analysed = araAnalysedSources.count(src) > 0;
                lattice::logDebug << "ARA setCabbageIsReady: source='" << (src ? src->getName() : "null")
                                 << "' isMyAudioSource=" << mine
                                 << " alreadyAnalysed=" << analysed;
                if (!analysed && mine)
                    toAnalyse.push_back(src);
            }

            // Fallback: if isMyAudioSource() identified nothing (host didn't
            // assign a PlaybackRenderer), queue all unanalysed sources.
            if (toAnalyse.empty())
            {
                lattice::logDebug << "ARA setCabbageIsReady: isMyAudioSource() found no matches — falling back to all unanalysed sources";
                for (auto* src : araAccessibleSources)
                {
                    if (araAnalysedSources.count(src) == 0)
                        toAnalyse.push_back(src);
                }
            }
        }
        for (auto* src : toAnalyse)
        {
            lattice::logDebug << "ARA: queuing deferred analysis from setCabbageIsReady for source='" << (src ? src->getName() : "null") << "'";
            araAnalysedSources.insert(src);
            enqueueAraSource(src);
        }
    }
#endif
}

//========================================================================================
// Callback function - triggered when a message is sent from the webview. Values are
// denormalized (in the actual parameter range) and will be normalized for host communication.
//========================================================================================
void CabbageProcessor::onMessageFromWebView(const nlohmann::json &j)
{
    // Handle both array-wrapped messages (legacy plugin format) and plain object messages (VSCode extension)
    nlohmann::json incomingMessage;
    if (j.is_array() && !j.empty())
    {
        incomingMessage = j.at(0);
    }
    else if (j.is_object())
    {
        incomingMessage = j;
    }
    else
    {
        lattice::logError << "Invalid message format received from webview: " << j.dump();
        return;
    }

    // Unpack 'obj' field if present (for plugin mode messages)
    if (incomingMessage.contains("obj") && incomingMessage["obj"].is_string())
    {
        try
        {
            auto objData = nlohmann::json::parse(incomingMessage["obj"].get<std::string>());
            // Merge obj data into the message, keeping the command field
            std::string cmd = incomingMessage["command"].get<std::string>();
            incomingMessage = objData;
            incomingMessage["command"] = cmd;
        }
        catch (const nlohmann::json::exception &e)
        {
            lattice::logError << "Failed to parse 'obj' field: " << e.what();
        }
    }

    // Try to handle with Engine first
    if (cabbage.processWebViewCommand(incomingMessage))
    {
        return;
    }

    // Handle environment-specific commands
    const std::string command = incomingMessage["command"].get<std::string>();

    if (command == "cabbageIsReadyToLoad")
    {
        lattice::logDebug << "CabbageProcessor: Calling queueGenTableUpdates() for webview reconnection";
        setCabbageIsReady();
        // Note: setCabbageIsReady() already calls updateUI() in plugin mode (#ifndef CabbageApp).
        // For CabbageApp mode, call it here since setCabbageIsReady() skips it.
#ifdef CabbageApp
        updateUI();
#endif
        // Re-queue table data for the reconnected webview
        cabbage.queueGenTableUpdates();
    }
    else if (command == "parameterChange")
    {
        lattice::logDebug << "WARNING: Something is using the old parameterChange message!!";
    }
    else if (command == "midiMessage")
    {
        try
        {
            // obj field was already parsed and merged into incomingMessage at line 845-849
            addNoteEventFromJson(incomingMessage);
        }
        catch (const std::exception &e)
        {
            lattice::logError << "Failed to process midiMessage: " << e.what();
        }
    }
    else if (command == "fileOpen")
    {
        try
        {
            // obj field was already parsed and merged into incomingMessage at line 845-849

            // Extract channel
            std::string channel = incomingMessage.value("channel", "");
            if (channel.empty())
            {
                lattice::logError << "fileOpen message missing channel";
                return;
            }

            // Extract options
            std::string directory = incomingMessage.value("directory", "");
            std::string filters = incomingMessage.value("filters", "*");
            bool openAtLastKnownLocation = incomingMessage.value("openAtLastKnownLocation", true);

            // Open native file dialog
            openFileDialog(channel, directory, filters, openAtLastKnownLocation);
        }
        catch (const nlohmann::json::exception &e)
        {
            lattice::logError << "Failed to process fileOpen: " << e.what();
        }
    }
    else if (command == "requestResize")
    {
        try
        {
            uint32_t width = incomingMessage.value("width", 0u);
            uint32_t height = incomingMessage.value("height", 0u);

            if (width == 0 || height == 0)
            {
                lattice::logError << "requestResize: invalid dimensions (width=" << width << ", height=" << height << ")";
                return;
            }

            // Request resize from host via the callback
            bool accepted = requestGuiResize(width, height);

            // If host accepted, manually apply the resize since some hosts don't call guiSetSize back
            if (accepted)
            {
                applyGuiResize(width, height);
            }

            // Send result back to webview
            nlohmann::json response;
            response["command"] = "resizeResponse";
            response["accepted"] = accepted;
            response["width"] = width;
            response["height"] = height;
            sendWebViewMessage(response);

            lattice::logDebug << "requestResize: " << width << "x" << height << " -> " << (accepted ? "accepted" : "rejected");
        }
        catch (const nlohmann::json::exception &e)
        {
            lattice::logError << "Failed to process requestResize: " << e.what();
        }
    }
}

//========================================================================================
// Adds a note event from a JSON message
//========================================================================================
void CabbageProcessor::addNoteEventFromJson(const nlohmann::json &j)
{
    uint8_t statusByte = j["statusByte"].get<uint8_t>();
    uint8_t dataByte1 = j["dataByte1"].get<uint8_t>();
    uint8_t dataByte2 = j["dataByte2"].get<uint8_t>();

    // Determine the event type based on MIDI status byte
    lattice::NoteEvent::Type eventType;
    if ((statusByte & 0xF0) == 0x90)
    {
        // MIDI note-on with velocity 0 is treated as note-off
        eventType = (dataByte2 == 0) ? lattice::NoteEvent::Type::noteOff : lattice::NoteEvent::Type::noteOn;
    }
    else if ((statusByte & 0xF0) == 0x80)
    { // Note-off message
        eventType = lattice::NoteEvent::Type::noteOff;
    }
    else
    {
        // Handle other cases or throw an error
        throw std::runtime_error("Unsupported MIDI message type");
    }

    // Convert MIDI velocity (0-127) to normalized velocity (0.0-1.0)
    double velocity = static_cast<double>(dataByte2) / 127.0;

    // Create the NoteEvent

    auto noteEvent = lattice::NoteEvent(eventType, static_cast<int16_t>(dataByte1), velocity, -1, 0);
    addNoteEvent(noteEvent);
}

//========================================================================================
// Update UI - we typically call this when we want to update widgets in the UI
//========================================================================================
void CabbageProcessor::updateUI()
{
    // iterate over all widget objects and send to webview
    int widgetsSent = 0;
    for (auto &w : cabbage.getWidgets())
    {
        std::string channelStr;
        // All widgets use channels array - check that first
        if (w.contains("channels") && w["channels"].is_array() && !w["channels"].empty() &&
            w["channels"][0].contains("id") && w["channels"][0]["id"].is_string())
        {
            channelStr = w["channels"][0]["id"].get<std::string>();
        }
        // Fallback for legacy widgets (form, etc.)
        else if (w.contains("id") && w["id"].is_string())
        {
            channelStr = w["id"].get<std::string>();
        }
        else
        {
            lattice::logDebug << "Widget skipped - no valid channel identifier";
            continue;
        }

        // Make a copy to avoid corrupting the original widget JSON
        auto widgetCopy = w;

        // Before sending, sync parameter values to widget JSON copy
        // This ensures UI reflects current parameter state when window reopens
        if (widgetCopy.contains("channels") && widgetCopy["channels"].is_array())
        {
            for (auto &channel : widgetCopy["channels"])
            {
                if (channel.contains("parameterIndex") && channel["parameterIndex"].is_number())
                {
                    int paramIdx = channel["parameterIndex"].get<int>();
                    if (paramIdx >= 0 && paramIdx < static_cast<int>(getParameters().size()))
                    {
                        // Get normalized value from parameter
                        float normalizedValue = getParameters()[paramIdx].value;

                        // SAFETY CHECK: Normalized values must be in [0, 1] range
                        // If not, the parameter system may not have initialized correctly
                        if (normalizedValue < 0.0f || normalizedValue > 1.0f)
                        {
                            lattice::logError << "Parameter " << paramIdx << " (" << channelStr
                                             << ") has invalid normalized value: " << normalizedValue
                                             << " (expected 0-1). Using range default instead.";

                            // Use default value from widget definition
                            if (channel.contains("range") && channel["range"].contains("defaultValue"))
                            {
                                float defaultValue = channel["range"]["defaultValue"].get<float>();
                                channel["range"]["value"] = defaultValue;

                                // Also normalize and store back to parameter to fix it
                                float fixedNormalized = getParameter(paramIdx).toNormalised(defaultValue);
                                getParameters()[paramIdx].value = fixedNormalized;
                                lattice::logDebug << "Fixed parameter " << paramIdx << " to normalized value: " << fixedNormalized;
                            }
                            continue; // Skip to next channel
                        }

                        // Denormalize to actual range for UI
                        float denormalizedValue = getParameter(paramIdx).fromNormalised(normalizedValue);

                        // Update the widget copy's range.value
                        if (!channel.contains("range"))
                        {
                            channel["range"] = nlohmann::json::object();
                        }
                        channel["range"]["value"] = denormalizedValue;
                    }
                }
            }
        }

        // Send proper JSON message like CabbageApp does
        nlohmann::json msg;
        msg["command"] = "widgetUpdate";
        msg["id"] = channelStr;
        msg["widgetJson"] = widgetCopy.dump(); // Send as JSON string, like CabbageApp does
        if (uiIsOpen)
        {
            sendWebViewMessage(msg);
            widgetsSent++;
        }
        else
        {
            lattice::logDebug << "Widget not sent (uiIsOpen=false): " << channelStr;
        }
    }

    lattice::logDebug << "Total widgets sent: " << widgetsSent;

    // Initial populate for widgets that configure it. Shares the trigger and
    // delivery path with runtime refreshes and the CabbageApp load path, so
    // results arrive identically (async scan + opcode-queue delivery).
    triggerInitialPopulate();

    // Check if editor has any pending messages when loaded..
    for (const auto &param : webviewMessageQueue)
    {
        bool updated = cabbage.updateWidget(param.name, [&](nlohmann::json &j) {
            j["value"] = param.value;
        });
        
        if (updated)
        {
            // Send proper JSON message like CabbageApp does
            nlohmann::json msg;
            msg["command"] = "widgetUpdate";
            msg["id"] = param.name;
            msg["value"] = param.value;
            cabbage.setControlChannel(param.name, param.value);
            sendWebViewMessage(msg);
        }
    }
    webviewMessageQueue.clear();
}
//========================================================================================
// Plugin state/loading functions
//========================================================================================
nlohmann::json CabbageProcessor::savePluginState()
{
    // Use the Engine utility function to save complete widget JSON data
    // false = session save (DAW), checks persistence.session
    return cabbage.saveWidgetJsonData(false);
}

void CabbageProcessor::loadPluginState(nlohmann::json state)
{
    // Use the Engine utility function to load complete widget state
    // This handles: widgets array, Csound channels, parameters, and UI updates
    // false = session load (DAW), checks persistence.session (matches savePluginState)
    cabbage.loadWidgetState(state, false);
}

//========================================================================================
// This is called from the host
//========================================================================================
void CabbageProcessor::setParameter(int paramId, double value)
{
    // value parameter is normalized [0-1] from host
    // Denormalize to actual range
    float denormalValue = getParameter(paramId).fromNormalised(value);

    // Quantize to increment if specified
    const auto& param = getParameter(paramId);
    if (param.increment > 0.0f) {
        denormalValue = std::round(denormalValue / param.increment) * param.increment;
        // Clamp to range after quantization to handle edge cases
        denormalValue = std::max(param.min, std::min(param.max, denormalValue));
    }

    // After quantization, re-normalize for storage
    float normalizedValue = getParameter(paramId).toNormalised(denormalValue);
    getParameters()[paramId].value = normalizedValue;


    // Use parameterChannelIds for fast O(1) lookup of actual Csound channel ID
    // (parameter name may be a human-readable label for DAW display)
    const auto channel = parameterChannelIds[paramId];

    // cabbage2 -> cabbage3 combobox quirk
    auto widgetOpt = std::optional<std::reference_wrapper<nlohmann::json>>();
    for (auto &w : cabbage.getWidgets())
    {
        // New schema: channels array
        if (w.contains("channels") && w["channels"].is_array())
        {
            for (const auto &ch : w["channels"])
            {
                if (ch.contains("id") && ch["id"].is_string() && ch["id"].get<std::string>() == channel)
                {
                    widgetOpt = w;
                    break;
                }
            }
            if (widgetOpt)
                break;
        }
    }
    if (widgetOpt)
    {
        auto &j = widgetOpt->get();
        std::string widgetType = j["type"].get<std::string>();

        // Update the channel's range.value (not top-level value property)
        if (j.contains("channels") && j["channels"].is_array() && !j["channels"].empty())
        {
            auto& firstChannel = j["channels"][0];
            if (firstChannel.contains("range") && firstChannel["range"].is_object())
            {
                firstChannel["range"]["value"] = denormalValue;
            }
        }

        // For comboBox and optionButton, the frontend already sends the correct index
        // (not a normalized value), so we should use it directly
        if (widgetType == "comboBox" || widgetType == "optionButton")
        {
            // The denormalValue is already the index from the frontend
            size_t index = static_cast<size_t>(round(denormalValue));

            if (j.contains("indexOffset") && j["indexOffset"].is_boolean() && j["indexOffset"].get<bool>())
            {
                index += 1;
            }
            
            // Update channel range value with the index
            if (j.contains("channels") && j["channels"].is_array() && !j["channels"].empty())
            {
                auto& firstChannel = j["channels"][0];
                if (firstChannel.contains("range") && firstChannel["range"].is_object())
                {
                    firstChannel["range"]["value"] = static_cast<double>(index);
                }
            }
            
            cabbage.setControlChannel(channel, index);
            return;
        }
    }

    cabbage.setControlChannel(channel, denormalValue);
}

#ifdef CabbageApp
//========================================================================================
// Update actual channel counts for standalone mode when audio device has fewer channels
// than configured. This prevents buffer overruns when channelConfig specifies more
// channels than the audio device supports. This method can only REDUCE channel counts,
// never increase them.
//========================================================================================
void CabbageProcessor::setActualChannelCounts(int inputs, int outputs)
{
    if (inputs < totalNumInputs)
    {
        lattice::logDebug << "Reducing input channels from " << totalNumInputs << " to " << inputs
                         << " (audio device limitation)";
        totalNumInputs = inputs;
    }
    if (outputs < totalNumOutputs)
    {
        lattice::logDebug << "Reducing output channels from " << totalNumOutputs << " to " << outputs
                         << " (audio device limitation)";
        totalNumOutputs = outputs;
    }
    matchingNumInputsOutputs = totalNumInputs == totalNumOutputs;
}
#endif

void CabbageProcessor::prepareToPlay(double sr, uint32_t /*minFrameCount*/, uint32_t /*maxFrameCount*/)
{
    // Acquire exclusive ownership: blocks until any in-flight process() block finishes,
    // then prevents process() from entering while we set up / rebuild Csound.
    std::unique_lock lock(audioEngineMutex);

    const bool needsInit = !cabbage.isEngineRunning();
    const bool srChanged = !needsInit && std::abs(sr - sampleRate) > 0.01;
    // Config change: the host called selectAudioPortsConfig(id) before prepareToPlay(),
    // so the active Lattice config already reflects the new selection.
    const bool configChanged = !needsInit &&
        (getChannelConfig().getTotalNumInputChannels()  != totalNumInputs ||
         getChannelConfig().getTotalNumOutputChannels() != totalNumOutputs);
    sampleRate = sr;

    if (needsInit || srChanged || configChanged)
        initialiseAudioEngine();

    processingEnabled.store(true, std::memory_order_release);
}

//========================================================================================
//=============================== CSOUND MIDI FUNCTIONS ==================================
//========================================================================================
// Opens MIDI input device
//========================================================================================
int CabbageProcessor::OpenMidiInputDevice(CSOUND *csound, void **userData, const char * /*devName*/)
{
    *userData = csoundGetHostData(csound);
    return 0;
}

//========================================================================================
// Reads MIDI input data from host, gets called every time there is MIDI input to our plugin
//========================================================================================
int CabbageProcessor::ReadMidiData(CSOUND * /*csound*/, void *userData, unsigned char *mbuf, int nbytes)
{
    auto *pluginData = static_cast<cabbage::Engine *>(userData);
    if (!userData)
    {
        cabbage::Utils::check(userData, "\nInvalid user data");
        return 0;
    }

    int cnt = 0;
    auto &noteEvents = pluginData->getProcessor().getNoteEvents();

    if (!noteEvents.empty()) {
        //lattice::logDebug << "ReadMidiData: " << noteEvents.size() << " note events in queue";
    }

    while (!noteEvents.empty() && cnt + 3 <= nbytes)
    {
        const auto &event = noteEvents.front(); // Get event

        // Skip unsupported types (e.g., noteChoke)
        if (event.type != lattice::NoteEvent::Type::noteOn && event.type != lattice::NoteEvent::Type::noteOff)
        {
            noteEvents.pop_front();
            continue;
        }

        // Convert to MIDI message
        uint8_t statusByte = (event.type == lattice::NoteEvent::Type::noteOn) ? 0x90 : 0x80;
        uint8_t velocity = static_cast<uint8_t>(event.velocity * 127.0); // Scale to 0-127

        // Write MIDI bytes
        *mbuf++ = statusByte; // Status (note-on/off + channel 1)
        *mbuf++ = event.key;  // MIDI note number (0-127)
        *mbuf++ = velocity;   // Velocity (0-127)

        cnt += 3;
        noteEvents.pop_front(); // Remove processed event
    }

    return cnt;
}

//========================================================================================
// Opens MIDI output device, adding -QN to your CsOptions will causes this method to be called
// as soon as your plugin loads
//========================================================================================
int CabbageProcessor::OpenMidiOutputDevice(CSOUND *csound, void **userData, const char * /*devName*/)
{
    *userData = csoundGetHostData(csound);
    return 0;
}

//========================================================================================
// Write MIDI data to plugin's MIDI output. Each time Csound outputs a midi message this
// method should be called. Note: you must have -Q set in your CsOptions
//========================================================================================
int CabbageProcessor::WriteMidiData(CSOUND* csound, void *_userData, const unsigned char *mbuf, int nbytes)
{
    auto *engineData = static_cast<cabbage::Engine *>(_userData);
    if (!engineData || nbytes <= 0)
        return 0;

    // Get processor from engine to access MIDI output callback
    auto& processor = engineData->getProcessor();

    double absSamplePos = 0.0;
    try {
        absSamplePos = double(csoundGetCurrentTimeSamples(csound) - engineData->getTotalSamples());
    } catch (...) {
        absSamplePos = 0.0;
    }

    // lattice::logDebug << "WriteMidiData: nbytes=" << nbytes
    //                   << " status=0x" << std::hex << (int)mbuf[0] << std::dec
    //                   << " engineData=" << engineData
    //                   << " processor=" << &processor;

    // Pass raw MIDI directly to host
    processor.sendRawMidi(mbuf, nbytes, absSamplePos);  

    return nbytes;
}


//========================================================================================
// Open native file dialog for fileButton
//========================================================================================
void CabbageProcessor::openFileDialog(const std::string &channel, const std::string &directory,
                                      const std::string &filters, bool openAtLastKnownLocation)
{
    std::string initialDir = directory;
    if (initialDir.empty() && openAtLastKnownLocation)
    {
        initialDir = cabbage::File::getParentDirectory(cabbage::File::getCsdFileAndPath());
    }

    std::string path = cabbage::File::browseForFile("Choose a file", initialDir, filters);
    if (!path.empty())
    {
        // Send the path to Csound via the channel
        cabbage.setStringChannel(channel, path.c_str());
        lattice::logDebug << "File selected for channel " << channel << ": " << path;
    }
}

//========================================================================================
// Generate error page HTML with compile errors overlaid
//========================================================================================
std::string CabbageProcessor::generateErrorPageHtml(const std::string &errors)
{
    std::string html = errorPageHtml;
    // Find the closing </style> tag to add overlay styles
    size_t styleEnd = html.find("</style>");
    if (styleEnd != std::string::npos)
    {
        std::string overlayStyles = "\n    .error-overlay {\n"
                                    "      position: absolute;\n"
                                    "      inset: 20px;\n" // equal spacing on all sides
                                    "      background-color: rgba(255, 255, 255, 0.9);\n"
                                    "      border: 2px solid #ff0000;\n"
                                    "      border-radius: 8px;\n"
                                    "      padding: 15px;\n"
                                    "      font-family: monospace;\n"
                                    "      font-size: 12px;\n"
                                    "      color: #000;\n"
                                    "      max-height: calc(100% - 40px);\n"
                                    "      overflow-y: auto;\n"
                                    "      z-index: 1000;\n"
                                    "      box-shadow: 0 4px 8px rgba(0,0,0,0.3);\n"
                                    "    }\n"
                                    "    body {\n"
                                    "      position: relative;\n"
                                    "    }\n";
        html.insert(styleEnd, overlayStyles);
    }

    // Find the closing </body> tag to add the error overlay div
    size_t bodyEnd = html.find("</body>");
    if (bodyEnd != std::string::npos)
    {
        std::string errorOverlay = "\n  <div class=\"error-overlay\">\n"
                                   "    <h3 style=\"color: #ff0000; margin-top: 0;\">Csound Compile Errors:</h3>\n"
                                   "    <pre style=\"margin: 0; white-space: pre-wrap; font-size: 11px;\">" +
                                   errors +
                                   "</pre>\n"
                                   "  </div>\n";
        html.insert(bodyEnd, errorOverlay);
    }
    else
    {
        // Fallback: append to end if no </body> found
        html += "\n  <div style=\"position: absolute; inset: 20px; background-color: rgba(255, 255, 255, 0.95); "
                "border: 2px solid #ff0000; border-radius: 8px; padding: 15px; font-family: monospace; font-size: "
                "12px; color: #000; max-height: calc(100% - 40px); overflow-y: auto; z-index: 1000; box-shadow: 0 4px "
                "8px rgba(0,0,0,0.3);\">\n"
                "    <h3 style=\"color: #ff0000; margin-top: 0;\">Csound Compile Errors:</h3>\n"
                "    <pre style=\"margin: 0; white-space: pre-wrap; font-size: 11px;\">" +
                errors +
                "</pre>\n"
                "  </div>\n";
    }
    return html;
}
