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

// CabbageParser.cpp

#include "CabbageParser.h"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <regex>
#include <iomanip>
#include <numeric>
#include <algorithm>
#include <cctype>
#include <unordered_set>

#ifdef CabbagePro
#include "encrypt.h"
#endif

// choc string utility class
#include <choc/text/choc_StringUtilities.h>

namespace cabbage
{

std::string Parser::removeQuotes(const std::string &str)
{
    std::string result = str;
    result.erase(std::remove(result.begin(), result.end(), '\"'), result.end());
    return result;
}

bool Parser::isWidget(const std::string &target)
{
    std::vector<std::string> widgetTypes = cabbage::WidgetDescriptors::getWidgetTypes();
    return std::find(widgetTypes.begin(), widgetTypes.end(), target) != widgetTypes.end();
}

std::vector<nlohmann::json> Parser::parseCsdForWidgets(const std::string &csdFile, std::string *outError)
{
    std::vector<nlohmann::json> widgets;
    std::string jsonError;
    std::string content;

#ifdef CabbagePro
    // Pro version: Check if file is encrypted
    if (Decrypt::isEncrypted(csdFile))
    {
        try {
            content = Decrypt::getCsdText(csdFile);
            lattice::logDebug << "Successfully decrypted CSD file for widget parsing";
            lattice::logDebug << content;
        }
        catch (const std::exception& e) {
            lattice::logError << "Failed to decrypt CSD file: " << e.what();
            if (outError) {
                *outError = "Failed to decrypt CSD file: " + std::string(e.what());
            }
            return widgets;
        }
    }
    else
    {
        // Regular unencrypted CSD file
        std::ifstream file(csdFile);
        if (!file.is_open())
        {
            lattice::logError << "Error opening CSD file: " << csdFile;
            return widgets;
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        content = buffer.str();
    }
#else
    // Free version: Only handle unencrypted files
    std::ifstream file(csdFile);
    if (!file.is_open())
    {
        lattice::logError << "Error opening CSD file: " << csdFile;
        return widgets;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    content = buffer.str();
#endif

    std::regex cabbageRegex(R"(<Cabbage>([\s\S]*?)</Cabbage>)");
    std::smatch cabbageMatch;

    if (std::regex_search(content, cabbageMatch, cabbageRegex))
    {
        std::string cabbageContent = cabbageMatch[1].str();

        // Route on the parsed document rather than raw text. JSONC comments may
        // sit between the key, colon and value, e.g. "type" /* form */ : "form",
        // which a "type"\s*:\s*"form" regex cannot see. Parsing here also avoids
        // matching "type": "form" when it only appears inside a comment/string.
        const auto parsedCabbage = nlohmann::json::parse(cabbageContent, nullptr,
                                                         /*allow_exceptions=*/false,
                                                         /*ignore_comments=*/true);

        const nlohmann::json *widgetArray = nullptr;
        if (!parsedCabbage.is_discarded())
        {
            if (parsedCabbage.is_array())
            {
                widgetArray = &parsedCabbage;
            }
            else if (parsedCabbage.is_object() && parsedCabbage.contains("widgets") &&
                     parsedCabbage["widgets"].is_array())
            {
                widgetArray = &parsedCabbage["widgets"];
            }
        }

        bool foundFormWidget = false;
        if (widgetArray)
        {
            foundFormWidget = std::any_of(widgetArray->begin(), widgetArray->end(),
                                          [](const nlohmann::json &widget)
                                          {
                                              return widget.is_object() && widget.contains("type") &&
                                                     widget["type"].is_string() &&
                                                     widget["type"].get<std::string>() == "form";
                                          });
        }

        if (foundFormWidget)
        {
            jsonError = parseContent(cabbageContent, widgets);
        }
        else
        {
            //this needs testing - esp for pro builds
            std::regex includeRegex(R"(#include\s*\"([^\"]+\.json)\")");
            std::smatch includeMatch;

            // A valid JSON section without a form is a sidecar-backed document,
            // not an include directive. Only plain-text sections can #include.
            if (parsedCabbage.is_discarded() &&
                std::regex_search(cabbageContent, includeMatch, includeRegex))
            {
                std::string includeFilename = includeMatch[1].str();
                std::filesystem::path includePath(includeFilename);

                if (!includePath.is_absolute())
                {
                    std::filesystem::path csdPath(csdFile);
                    includePath = csdPath.parent_path() / includePath;
                }

                parseJsonFile(includePath.string(), widgets);
            }
            else
            {
                std::string fallbackJsonFile = csdFile.substr(0, csdFile.find_last_of('.')) + ".json";
                parseJsonFile(fallbackJsonFile, widgets);
            }
        }
    }
    else
    {
        lattice::logError << "No <Cabbage> section found in the file: " << csdFile;
    }

    if (outError && !jsonError.empty())
    {
        *outError = jsonError;
    }

    return widgets;
}

std::string Parser::parseContent(const std::string &content, std::vector<nlohmann::json> &widgets)
{
    cabbage::ScopedPhaseTimer parseTimer("parseContent (CSD JSON parse + descriptor load)");
    std::string errorMessage;
    try
    {
        auto jsonRoot = nlohmann::json::parse(content, nullptr, /*allow_exceptions=*/false,
                                              /*ignore_comments=*/true);
        if (jsonRoot.is_discarded())
        {
            errorMessage = "JSON Parse Error: invalid or malformed Cabbage JSON";
            lattice::logError << errorMessage;
            return errorMessage;
        }

        // Support both old array format and new object format with "widgets" key
        nlohmann::json widgetsArray;
        if (jsonRoot.is_array())
        {
            // Legacy format: root is array
            widgetsArray = jsonRoot;
            lattice::logDebug << "Parsing Cabbage JSON using legacy array format";
        }
        else if (jsonRoot.is_object() && jsonRoot.contains("widgets") && jsonRoot["widgets"].is_array())
        {
            // New format: root is object with "widgets" key
            widgetsArray = jsonRoot["widgets"];
            lattice::logDebug << "Parsing Cabbage JSON using new object format with 'widgets' key";
        }
        else
        {
            errorMessage = "Invalid Cabbage JSON structure: Expected either an array of widgets or an object with a 'widgets' array";
            lattice::logError << errorMessage;
            return errorMessage;
        }

        // Process widgets array
        for (auto &item : widgetsArray)
        {
            if (item.is_object())
            {
                // Check if "type" field exists and is a string
                if (!item.contains("type") || !item["type"].is_string())
                {
                    lattice::logError << "Widget is missing valid 'type' field - Skipping this widget and continuing...";
                    lattice::logDebug << "Invalid widget JSON: " << item.dump();
                    continue;
                }

                std::string widgetType = item["type"].get<std::string>();
                auto j = WidgetDescriptors::get(widgetType);
                if (!j.is_null())
                {
                    initialiseWidgetJson(j, item, widgets.size());
                    widgets.push_back(j);
                }
                else
                {
                    lattice::logError << "Widget type is not valid: " << widgetType
                                     << " - Skipping this widget and continuing...";
                    // Continue processing other widgets instead of crashing
                }
            }
        }
    }
    catch (const nlohmann::json::parse_error &e)
    {
        errorMessage = "JSON Parse Error: " + std::string(e.what());
        lattice::logError << errorMessage;
    }
    return errorMessage;
}

void Parser::parseJsonFile(const std::string &filename, std::vector<nlohmann::json> &widgets)
{
    std::string content;

#ifdef CabbagePro
    // Pro version: Check if file is encrypted (could be .ejson or .ecsd)
    if (Decrypt::isEncrypted(filename))
    {
        try {
            content = Decrypt::getCsdText(filename);
            lattice::logDebug << "Successfully decrypted JSON file for widget parsing: " << filename;
        }
        catch (const std::exception& e) {
            lattice::logError << "Failed to decrypt JSON file: " << e.what();
            return;
        }
    }
    else
    {
        // Regular unencrypted JSON file
        std::ifstream jsonFile(filename);
        if (!jsonFile.is_open())
        {
            lattice::logDebug << "Error opening JSON file: " << filename;
            return;
        }

        std::stringstream buffer;
        buffer << jsonFile.rdbuf();
        content = buffer.str();
    }
#else
    // Free version: Only handle unencrypted files
    std::ifstream jsonFile(filename);
    if (!jsonFile.is_open())
    {
        lattice::logDebug << "Error opening JSON file: " << filename;
        return;
    }

    std::stringstream buffer;
    buffer << jsonFile.rdbuf();
    content = buffer.str();
#endif

    parseContent(content, widgets);
}


void Parser::initialiseWidgetJson(nlohmann::json &jsonObj, const nlohmann::json &incomingJson, size_t numWidgets)
{
    try
    {
        // Validate input JSON objects
        if (!jsonObj.is_object()) {
            lattice::logDebug << jsonObj.dump(4);
            lattice::logError << "Target jsonObj is not a valid JSON object";
            return;
        }
        
        if (!incomingJson.is_object()) {
            lattice::logError << "Incoming JSON is not a valid JSON object";
            return;
        }
        
        // Check for required 'type' field in target JSON
        if (!jsonObj.contains("type") || !jsonObj["type"].is_string()) {
            lattice::logError << "Target JSON object is missing required 'type' field or it's not a string";
            return;
        }
        
        const std::string widgetType = jsonObj["type"].get<std::string>();
        
        // Assign widget id from first channel if not provided in incoming JSON, otherwise auto-assign
        // Form is a special case with a fixed "MainForm" id
        if (widgetType != "form")
        {
            if (!incomingJson.contains("id"))
            {
                bool assigned = false;
                if (incomingJson.contains("channels") && incomingJson["channels"].is_array() && !incomingJson["channels"].empty())
                {
                    auto& firstChannel = incomingJson["channels"][0];
                    if (firstChannel.contains("id") && firstChannel["id"].is_string())
                    {
                        jsonObj["id"] = escapeJSON(firstChannel["id"].get<std::string>());
                        assigned = true;
                    }
                }
                if (!assigned)
                {
                    std::string autoId = widgetType + std::to_string(static_cast<int>(numWidgets));
                    jsonObj["id"] = autoId;
                    // Also set the first channel's id if channels exist
                    if (incomingJson.contains("channels") && incomingJson["channels"].is_array() && !incomingJson["channels"].empty())
                    {
                        jsonObj["channels"][0]["id"] = autoId;
                    }
                    lattice::logDebug << incomingJson.dump(4);
                    lattice::logError << "Widget type '" << widgetType << "' is missing an id property. "
                                      << "Automatically assigned: \"" << autoId
                                      << "\". Assign your own id property to avoid unexpected behavior.";
                }
            }
        }
        else
        {
            jsonObj["id"] = "MainForm";
        }

        mergeJsonProperties(jsonObj, incomingJson);
        
        // Assign default ranges to channels that don't have them
        assignDefaultRangesToChannels(jsonObj);
    }
    catch (const nlohmann::json::exception &e)
    {
        lattice::logError << "JSON exception in initialiseWidgetJson: " << e.what();
    }
    catch (const std::exception &e)
    {
        lattice::logError << "Unexpected exception in initialiseWidgetJson: " << e.what();
    }

}

void Parser::mergeJsonProperties(nlohmann::json &jsonObj, const nlohmann::json &incomingJson)
{
    // Check for required 'type' field in target JSON
    if (!jsonObj.contains("type") || !jsonObj["type"].is_string()) {
        lattice::logError << "Target JSON object is missing required 'type' field or it's not a string";
        return;
    }
    
    const std::string widgetType = jsonObj["type"].get<std::string>();
    
    // Known boolean properties that may be encoded as numeric 1/0 by Csound.
    // We include both top-level keys and common nested property names (eg. "logarithmic").
    static const std::unordered_set<std::string> booleanProperties = {
        "visible", "automatable", "active", "popup",
        "svgElement", "valueTextBox", "moveBehind", "filmStrip", "logarithmic",
        "preset", "session"
    };

    // Iterate through incoming JSON properties
    for (auto it = incomingJson.begin(); it != incomingJson.end(); ++it)
    {
        const std::string &key = it.key();
        const auto &value = it.value();

        if (key == "type")
            continue;

        try
        {
            if (key == "bounds" || key == "range" || key == "size")
            {
                if (value.is_object())
                {
                    for (auto &[propKey, val] : value.items())
                    {
                        jsonObj[key][propKey] = val;
                    }
                }
                else
                {
                    lattice::logError << "Property '" << key << "' should be an object for widget type: " << widgetType;
                }
            }
            else if (key == "sampleRange")
            {
                if (value.is_array() && value.size() == 2)
                {
                    if (value[0].is_number() && value[1].is_number()) {
                        jsonObj["startSample"] = value[0].get<int>();
                        jsonObj["endSample"] = value[1].get<int>();
//                            lattice::logDebug << "Set sample range for widget type: " << widgetType;
                    } else {
                        lattice::logError << "sampleRange array elements must be numbers for widget type: " << widgetType;
                    }
                }
                else
                {
                    lattice::logError << "sampleRange must be an array with exactly 2 elements for widget type: " << widgetType;
                }
            }
            else if (key == "populate")
            {
                if (value.is_object())
                {
                    // Merge populate configuration properties (don't replace entire object)
                    // This allows updating just directory or fileType without losing other properties
                    if (!jsonObj.contains("populate") || !jsonObj["populate"].is_object())
                    {
                        jsonObj["populate"] = nlohmann::json::object();
                    }

                    for (auto &[popKey, popVal] : value.items())
                    {
                        jsonObj["populate"][popKey] = popVal;
                    }

                    // CRITICAL: Cannot do file I/O here - we're likely holding a mutex
                    // Just store the config, actual population happens elsewhere
//                    lattice::logDebug << "Merged populate config (processing deferred to avoid blocking)";
                }
                else
                {
                    lattice::logDebug << "populate property must be an object for widget type: " << widgetType;
                }
            }
            else if (key == "persistence")
            {
                if (value.is_object())
                {
                    // Initialize persistence object if it doesn't exist
                    if (!jsonObj.contains("persistence") || !jsonObj["persistence"].is_object())
                    {
                        jsonObj["persistence"] = nlohmann::json::object();
                    }

                    // Merge persistence properties
                    for (auto &[persKey, persVal] : value.items())
                    {
                        // Convert numeric 1/0 to boolean for preset/session
                        if ((persKey == "preset" || persKey == "session") && persVal.is_number())
                        {
                            jsonObj["persistence"][persKey] = (persVal.get<double>() != 0);
                        }
                        else
                        {
                            jsonObj["persistence"][persKey] = persVal;
                        }
                    }

                    // Auto-cascade logic: if preset is set to false and session wasn't explicitly provided,
                    // automatically set session to false as well
                    if (jsonObj["persistence"].contains("preset") &&
                        jsonObj["persistence"]["preset"].is_boolean() &&
                        jsonObj["persistence"]["preset"].get<bool>() == false)
                    {
                        // Only auto-set session to false if it wasn't explicitly provided in the incoming JSON
                        if (!value.contains("session"))
                        {
                            jsonObj["persistence"]["session"] = false;
                        }
                    }
                }
                else
                {
                    lattice::logDebug << "persistence property must be an object for widget type: " << widgetType;
                }
            }
            else if (key == "items")
            {
                if (value.is_array())
                {
                    lattice::logDebug << "Size of items: " << value.size();
                    // Validate array contains strings
                    bool allStrings = std::all_of(value.begin(), value.end(),
                                                [](const nlohmann::json& item) { return item.is_string(); });

                    
                    if (allStrings)
                    {
                        jsonObj["items"] = value;

                        // Update the channel's range min/max based on items count
                        if (jsonObj.contains("channels") && jsonObj["channels"].is_array() && !jsonObj["channels"].empty())
                        {
                            jsonObj["channels"][0]["range"]["min"] = 0;
                            jsonObj["channels"][0]["range"]["max"] = static_cast<int>(value.size() - 1);
                        }
                    }
                    else
                    {
                        lattice::logDebug << "items array must contain only string values for widget type: " << widgetType;
                    }
                }
                else
                {
                    lattice::logDebug << "items property must be an array for widget type: " << widgetType;
                }
            }
            else if (key == "samples")
            {
                if (value.is_array())
                {
                    bool allNumbers = std::all_of(value.begin(), value.end(),
                                                [](const nlohmann::json& item) { return item.is_number(); });
                    
                    if (allNumbers) {
                        jsonObj["samples"] = value.get<std::vector<double>>();
//                            lattice::logDebug << "Set samples array with " << value.size() << " numeric values";
                    } else {
                        lattice::logDebug << "samples array must contain only numeric values for widget type: " << widgetType;
                    }
                }
                else
                {
                    lattice::logDebug << "samples property must be an array for widget type: " << widgetType;
                }
            }
            else if (key == "color")
            {
                if (value.is_object())
                {
                    parseColorProperties(value, jsonObj[key]);
//                        lattice::logDebug << "Parsed color object for widget type: " << widgetType;
                }
                else if (value.is_string() || value.is_number())
                {
                    jsonObj[key] = parseColorValue(value);
//                        lattice::logDebug << "Parsed color value for widget type: " << widgetType;
                }
                else
                {
                    lattice::logDebug << "color property must be an object, string, or number for widget type: " << widgetType;
                }
            }
            else if (key == "file")
            {
                if (value.is_string())
                {
                    std::string filePath = cabbage::Utils::sanitisePath(value.get<std::string>());
                    jsonObj["file"] = filePath;
//                        lattice::logDebug << "Set file path: " << filePath << " for widget type: " << widgetType;
                }
                else
                {
                    lattice::logDebug << "file property must be a string for widget type: " << widgetType;
                }
            }
            else if (key == "channel")
            {
                if (value.is_string())
                {
                    jsonObj["channel"] = value.get<std::string>();
//                        lattice::logDebug << "Set channel: " << value.get<std::string>() << " for widget type: " << widgetType;
                }
                else if (value.is_object())
                {
                    // Handle channel object with id and possibly other properties
                    for (auto &[chKey, chVal] : value.items())
                    {
                        jsonObj["channel"][chKey] = chVal;
                    }
//                        lattice::logDebug << "Set channel object for widget type: " << widgetType;
                }
                else
                {
                    lattice::logDebug << "channel property must be a string or object for widget type: " << widgetType;
                }
            }
            else if (key == "channels")
            {
                if (value.is_array())
                {
                    // Ensure jsonObj has a channels array initialized
                    if (!jsonObj.contains("channels") || !jsonObj["channels"].is_array())
                    {
                        jsonObj["channels"] = nlohmann::json::array();
                    }
                    
                    // Handle channels array
                    for (size_t i = 0; i < value.size(); ++i)
                    {
                        if (value[i].is_object())
                        {
                            // Ensure the channel element exists
                            if (i >= jsonObj["channels"].size())
                            {
                                jsonObj["channels"].push_back(nlohmann::json::object());
                            }
                            else if (!jsonObj["channels"][i].is_object())
                            {
                                jsonObj["channels"][i] = nlohmann::json::object();
                            }
                            
                            for (auto &[chKey, chVal] : value[i].items())
                            {
                                jsonObj["channels"][i][chKey] = chVal;
                            }
                        }
                        else
                        {
                            lattice::logDebug << "channels array elements must be objects for widget type: " << widgetType;
                        }
                    }
//                        lattice::logDebug << "Set channels array with " << value.size() << " elements for widget type: " << widgetType;
                }
                else
                {
                    lattice::logDebug << "channels property must be an array for widget type: " << widgetType;
                }
            }
            else
            {
                // For nested objects (like label, thumb, track, valueText), merge properties
                if (value.is_object() && jsonObj.contains(key) && jsonObj[key].is_object())
                    {
                        // Recursively merge nested object properties. When nested properties
                        // come from Csound they are sometimes encoded as numeric 1/0 —
                        // convert those to real booleans for known boolean properties
                        for (auto &[nestedKey, nestedVal] : value.items())
                        {
                            if (booleanProperties.find(nestedKey) != booleanProperties.end() && nestedVal.is_number())
                            {
                                jsonObj[key][nestedKey] = (nestedVal.get<double>() != 0);
                            }
                            else
                            {
                                jsonObj[key][nestedKey] = nestedVal;
                            }
                        }
    //                    lattice::logDebug << "Merged nested object property '" << key << "' for widget type: " << widgetType;
                    }
                else
                {
                    // Check if this is a known boolean property and the value is numeric (from Csound)
                    if (booleanProperties.find(key) != booleanProperties.end() && value.is_number())
                    {
                        // Convert numeric 1/0 from Csound to boolean true/false for JavaScript
                        jsonObj[key] = (value.get<double>() != 0);
//                        lattice::logDebug << "Converted numeric " << value << " to boolean for property '" << key << "'";
                    }
                    else
                    {
                        // For all other properties, just assign the value directly
                        jsonObj[key] = value;
//                        lattice::logDebug << "Set property '" << key << "' for widget type: " << widgetType;
                    }
                }
            }
        }
        catch (const nlohmann::json::exception &e)
        {
            std::string widgetId = jsonObj.value("id", "unknown");
            lattice::logDebug << "JSON processing error for key '" << key << "' in widget '" << widgetId << "' (type '" << widgetType << "'): " << e.what();
        }
        catch (const std::exception &e)
        {
            std::string widgetId = jsonObj.value("id", "unknown");
            lattice::logDebug << "Unexpected error processing key '" << key << "' in widget '" << widgetId << "' (type '" << widgetType << "'): " << e.what();
        }
    }
}

void Parser::parseStroke(const nlohmann::json &strokeValue, nlohmann::json &target)
{
    if (strokeValue.contains("color"))
    {
        target["color"] = parseColorValue(strokeValue["color"]);
    }
    if (strokeValue.contains("width"))
    {
        target["width"] = strokeValue["width"];
    }
}

void Parser::parseColorProperties(const nlohmann::json &value, nlohmann::json &target)
{
    for (auto &[key, val] : value.items())
    {
        if (key == "fill" || key == "background")
        {
            target[key] = parseColorValue(val);
        }
        else if (key == "stroke" && val.is_object())
        {
            parseStroke(val, target[key]);
        }
        else if (key == "width" && val.is_number())
        {
            // Width is a numeric property, not a color - assign directly
            target[key] = val;
        }
        else if (val.is_object())
        {
            parseColorProperties(val, target[key]);
        }
        else if (val.is_number())
        {
            // Other numeric properties in color objects (like opacity) - assign directly
            target[key] = val;
        }
        else
        {
            target[key] = parseColorValue(val);
        }
    }
}

std::string Parser::parseColorValue(const nlohmann::json &value)
{
    if (value.is_array() && value.size() >= 3)
    {
        return rgbToHex(value.get<std::vector<double>>());
    }
    else if (value.is_string())
    {
        std::string colorStr = value.get<std::string>();
        
        // Improved CSS rgb()/rgba() parsing:
        // Accepts integer (0-255), decimal (0-1) and percentage (0% - 100%) values.
        std::regex rgbaRegex(R"(^rgba\(\s*([^,]+)\s*,\s*([^,]+)\s*,\s*([^,]+)\s*,\s*([^\)]+)\s*\)$)", std::regex::icase);
        std::regex rgbRegex(R"(^rgb\(\s*([^,]+)\s*,\s*([^,]+)\s*,\s*([^\)]+)\s*\)$)", std::regex::icase);

        std::smatch match;
        auto parseComponent = [](const std::string &s) -> int {
            std::string str = s;
            // trim
            str.erase(0, str.find_first_not_of(" \t\n\r"));
            str.erase(str.find_last_not_of(" \t\n\r") + 1);

            bool isPercent = false;
            if (!str.empty() && str.back() == '%')
            {
                isPercent = true;
                str = str.substr(0, str.size() - 1);
            }

            double val = 0.0;
            try
            {
                val = std::stod(str);
            }
            catch (...) { val = 0.0; }

            int out;
            if (isPercent)
            {
                // percentage -> 0..255
                out = static_cast<int>(std::round(std::clamp(val, 0.0, 100.0) * 255.0 / 100.0));
            }
            else
            {
                // if value looks like 0..1 use that scale, otherwise assume 0..255
                if (val >= 0.0 && val <= 1.0)
                    out = static_cast<int>(std::round(val * 255.0));
                else
                    out = static_cast<int>(std::round(std::clamp(val, 0.0, 255.0)));
            }

            return std::clamp(out, 0, 255);
        };

        auto parseAlpha = [](const std::string &s) -> double {
            std::string str = s;
            // trim
            str.erase(0, str.find_first_not_of(" \t\n\r"));
            str.erase(str.find_last_not_of(" \t\n\r") + 1);

            bool isPercent = false;
            if (!str.empty() && str.back() == '%')
            {
                isPercent = true;
                str = str.substr(0, str.size() - 1);
            }

            double val = 1.0;
            try { val = std::stod(str); } catch (...) { val = 1.0; }

            if (isPercent)
            {
                val = std::clamp(val / 100.0, 0.0, 1.0);
            }
            else
            {
                // If given as 0..255 (unlikely for alpha) treat >1 as 1
                if (val > 1.0)
                    val = 1.0;
                val = std::clamp(val, 0.0, 1.0);
            }
            return val;
        };

        if (std::regex_match(colorStr, match, rgbaRegex))
        {
            int r = parseComponent(match[1].str());
            int g = parseComponent(match[2].str());
            int b = parseComponent(match[3].str());
            double a = parseAlpha(match[4].str());

            int alpha = static_cast<int>(std::round(a * 255.0));
            return rgbaToHex(r, g, b, alpha);
        }
        else if (std::regex_match(colorStr, match, rgbRegex))
        {
            int r = parseComponent(match[1].str());
            int g = parseComponent(match[2].str());
            int b = parseComponent(match[3].str());

            std::vector<double> rgb = {static_cast<double>(r), static_cast<double>(g), static_cast<double>(b)};
            return rgbToHex(rgb);
        }
        else
        {
            // Fall back to existing hex/named color validation
            return validateHexString(colorStr);
        }
    }
    return "#000000";
}

std::string Parser::rgbToHex(const std::vector<double> &rgb)
{
    auto clampByte = [](int v) { return std::clamp(v, 0, 255); };
    int r = clampByte(static_cast<int>(std::round(rgb.size() > 0 ? rgb[0] : 0.0)));
    int g = clampByte(static_cast<int>(std::round(rgb.size() > 1 ? rgb[1] : 0.0)));
    int b = clampByte(static_cast<int>(std::round(rgb.size() > 2 ? rgb[2] : 0.0)));

    std::ostringstream hex;
    hex << "#" << std::hex << std::setw(2) << std::setfill('0') << std::nouppercase
        << (r >> 4 & 0xF) << (r & 0xF); // placeholder - we'll format properly below
    // Proper formatting using stringstream with manipulators for each byte
    std::ostringstream out;
    out << "#";
    out << std::hex << std::setw(2) << std::setfill('0') << std::nouppercase << std::uppercase;
    // we need to ensure two hex digits per byte, so cast to unsigned and mask
    {
        std::ostringstream tmp; tmp << std::hex << std::setw(2) << std::setfill('0') << (unsigned)(r & 0xFF);
        out << tmp.str();
    }
    {
        std::ostringstream tmp; tmp << std::hex << std::setw(2) << std::setfill('0') << (unsigned)(g & 0xFF);
        out << tmp.str();
    }
    {
        std::ostringstream tmp; tmp << std::hex << std::setw(2) << std::setfill('0') << (unsigned)(b & 0xFF);
        out << tmp.str();
    }
    std::string result = out.str();
    // make lowercase to match existing style
    std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) { return std::tolower(c); });
    return result;
}

std::string Parser::rgbaToHex(int r, int g, int b, int a)
{
    auto clampByte = [](int v) { return std::clamp(v, 0, 255); };
    unsigned rr = static_cast<unsigned>(clampByte(r)) & 0xFF;
    unsigned gg = static_cast<unsigned>(clampByte(g)) & 0xFF;
    unsigned bb = static_cast<unsigned>(clampByte(b)) & 0xFF;
    unsigned aa = static_cast<unsigned>(clampByte(a)) & 0xFF;

    std::ostringstream out;
    out << "#";
    {
        std::ostringstream tmp; tmp << std::hex << std::setw(2) << std::setfill('0') << (unsigned)rr; out << tmp.str();
    }
    {
        std::ostringstream tmp; tmp << std::hex << std::setw(2) << std::setfill('0') << (unsigned)gg; out << tmp.str();
    }
    {
        std::ostringstream tmp; tmp << std::hex << std::setw(2) << std::setfill('0') << (unsigned)bb; out << tmp.str();
    }
    {
        std::ostringstream tmp; tmp << std::hex << std::setw(2) << std::setfill('0') << (unsigned)aa; out << tmp.str();
    }

    std::string result = out.str();
    std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) { return std::tolower(c); });
    return result;
}

std::string Parser::validateHexString(const std::string &str)
{
    std::regex hexRegex("^#([0-9A-Fa-f]{6}|[0-9A-Fa-f]{8})$");

    if (std::regex_match(str, hexRegex))
    {
        return str;
    }
    else
    {
        return Colours::getColour(str);
    }
}

std::string Parser::escapeJSON(const std::string &str)
{
    std::string escaped;

    for (char c : str)
    {
        switch (c)
        {
        case '\"':
            escaped += "\\\"";
            break;
        case '\\':
            escaped += "\\\\";
            break;
        case '\b':
            escaped += "\\b";
            break;
        case '\f':
            escaped += "\\f";
            break;
        case '\n':
            escaped += "\\n";
            break;
        case '\r':
            escaped += "\\r";
            break;
        case '\t':
            escaped += "\\t";
            break;
        default:
            escaped += c;
            break;
        }
    }

    return escaped;
}

void Parser::assignDefaultRangesToChannels(nlohmann::json &jsonObj)
{
    try
    {
        // Get widget type and load defaults from JS
        std::string widgetType = jsonObj.contains("type") && jsonObj["type"].is_string()
            ? jsonObj["type"].get<std::string>() : "";

        nlohmann::json widgetDefaults;
        if (!widgetType.empty())
        {
            widgetDefaults = cabbage::WidgetDescriptors::get(widgetType);
        }

        // Handle channels array case
        if (jsonObj.contains("channels") && jsonObj["channels"].is_array())
        {
            // Iterate through channels and ensure each has a complete range object
            for (size_t i = 0; i < jsonObj["channels"].size(); i++)
            {
                auto& channel = jsonObj["channels"][i];
                if (channel.is_object())
                {
                    // Try to get default channel properties from widget defaults
                    nlohmann::json defaultChannel;
                    if (!widgetDefaults.empty() &&
                        widgetDefaults.contains("channels") &&
                        widgetDefaults["channels"].is_array() &&
                        i < widgetDefaults["channels"].size())
                    {
                        defaultChannel = widgetDefaults["channels"][i];
                    }

                    // If channel doesn't have a type, copy from defaults
                    if (!channel.contains("type") && defaultChannel.contains("type"))
                    {
                        channel["type"] = defaultChannel["type"];
                    }

                    // Default to 'drag' interaction, but use 'click' for certain widget types
                    std::string interaction = "drag";
                    if (widgetType == "button" || widgetType == "checkbox" || widgetType == "optionButton" ||
                        widgetType == "radioGroup" || widgetType == "checkBox")
                    {
                        interaction = "click";
                    }
                    
                    // Create default range with all required properties
                    nlohmann::json defaultRange = {
                        {"min", 0.0},
                        {"max", 1.0},
                        {"defaultValue", 0.0},
                        {"value", 0.0},  // Will be set to defaultValue below
                        {"skew", 1.0},
                        {"increment", interaction == "click" ? 1.0 : 0.001}
                    };
                    
                    // If channel doesn't have a range object at all, assign the default
                    if (!channel.contains("range") || !channel["range"].is_object())
                    {
                        channel["range"] = defaultRange;
                    }
                    else
                    {
                        // Ensure all required properties exist in the existing range object
                        auto& range = channel["range"];
                        if (!range.contains("min") || !range["min"].is_number())
                            range["min"] = defaultRange["min"];
                        if (!range.contains("max") || !range["max"].is_number())
                            range["max"] = defaultRange["max"];
                        if (!range.contains("defaultValue") || !range["defaultValue"].is_number())
                            range["defaultValue"] = defaultRange["defaultValue"];
                        // Initialize value from defaultValue if not present
                        if (!range.contains("value") || !range["value"].is_number())
                            range["value"] = range["defaultValue"];
                        if (!range.contains("skew") || !range["skew"].is_number())
                            range["skew"] = defaultRange["skew"];
                        if (!range.contains("increment") || !range["increment"].is_number())
                            range["increment"] = defaultRange["increment"];

                    }
                }
            }
        }
    }
    catch (const nlohmann::json::exception &e)
    {
        lattice::logError << "JSON exception in assignDefaultRangesToChannels: " << e.what();
    }
    catch (const std::exception &e)
    {
        lattice::logError << "Unexpected exception in assignDefaultRangesToChannels: " << e.what();
    }
}

//=====================================================================================
// Maximum items a single populate scan may produce. Bounds scan time, the
// widgetUpdate payload, and webview render cost (lists render every row -
// there is no virtualization). Hit => warning + truncation (see below).
//=====================================================================================
static constexpr size_t kMaxPopulateFiles = 2000;

//=====================================================================================
// Natural (numeric-aware), case-insensitive ordering so numbered samples sort
// as kick1, kick2, ..., kick10 rather than kick1, kick10, kick2.
//=====================================================================================
static bool naturalLess(const std::string &a, const std::string &b)
{
    size_t i = 0, j = 0;
    const auto lower = [](unsigned char c) { return static_cast<char>(std::tolower(c)); };
    while (i < a.size() && j < b.size())
    {
        const bool da = std::isdigit(static_cast<unsigned char>(a[i])) != 0;
        const bool db = std::isdigit(static_cast<unsigned char>(b[j])) != 0;
        if (da && db)
        {
            size_t e1 = i, e2 = j;
            while (e1 < a.size() && std::isdigit(static_cast<unsigned char>(a[e1])) != 0)
                ++e1;
            while (e2 < b.size() && std::isdigit(static_cast<unsigned char>(b[e2])) != 0)
                ++e2;
            const size_t len1 = e1 - i, len2 = e2 - j;
            if (len1 != len2)
                return len1 < len2;
            const int cmp = a.compare(i, len1, b, j, len2);
            if (cmp != 0)
                return cmp < 0;
            i = e1;
            j = e2;
        }
        else
        {
            const char ca = lower(static_cast<unsigned char>(a[i]));
            const char cb = lower(static_cast<unsigned char>(b[j]));
            if (ca != cb)
                return ca < cb;
            ++i;
            ++j;
        }
    }
    // At least one side exhausted: shorter sorts first, identical => false.
    return (i == a.size()) && (j != b.size());
}

//=====================================================================================
// Collect files for populate listings. Exists because populate needs three
// things lattice::File::getFilesOfType cannot express: flat (non-recursive)
// scans, early termination at a result cap, and no-throw traversal that
// survives unreadable subdirectories (one bad dir must not void the scan).
// Extension matching uses lowercased dot-form tokens (".wav"); matchAllFiles
// covers the "*" filter. Never throws.
//=====================================================================================
static std::vector<std::string> collectPopulateFiles(
    const std::string &directory,
    const std::unordered_set<std::string> &extensions,
    bool matchAllFiles,
    bool recursive,
    size_t maxFiles,
    bool &wasTruncated)
{
    namespace fs = std::filesystem;
    std::vector<std::string> files;
    wasTruncated = false;
    std::error_code ec;

    auto tryAdd = [&](const fs::directory_entry &entry) -> bool {
        std::error_code ec2;
        if (!entry.is_regular_file(ec2) || ec2)
            return true; // skip, keep going
        if (!matchAllFiles)
        {
            std::string ext = entry.path().extension().string();
            std::transform(ext.begin(), ext.end(), ext.begin(),
                           [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
            if (extensions.find(ext) == extensions.end())
                return true;
        }
        if (files.size() >= maxFiles)
        {
            wasTruncated = true;
            return false; // stop
        }
        files.push_back(entry.path().string());
        return true;
    };

    if (!recursive)
    {
        fs::directory_iterator it(directory, fs::directory_options::skip_permission_denied, ec);
        if (ec)
            return files;
        const fs::directory_iterator end;
        for (; it != end; it.increment(ec))
        {
            if (ec)
                break;
            if (!tryAdd(*it))
                break;
        }
    }
    else
    {
        fs::recursive_directory_iterator it(directory, fs::directory_options::skip_permission_denied, ec);
        if (ec)
            return files;
        const fs::recursive_directory_iterator end;
        for (; it != end; it.increment(ec))
        {
            if (ec)
                break;
            if (!tryAdd(*it))
                break;
        }
    }
    return files;
}

//=====================================================================================
// Normalize a populate fileType filter so the documented and legacy forms all
// work. Matching downstream is verbatim against ".ext", so without this step
// "*.wav" (documented), "wav" and "" (the default) all match nothing.
// Rules per ';'-separated token: "*" stays (match all); a leading "*" is
// stripped ("*.wav" -> ".wav"); a missing leading dot is added ("wav" ->
// ".wav"). An empty/missing filter means "all files" ("*").
//=====================================================================================
static std::string normalizePopulateFileTypes(const std::string &fileTypes)
{
    auto trim = [](std::string s)
    {
        const char *ws = " \t\r\n";
        s.erase(0, s.find_first_not_of(ws));
        if (!s.empty())
            s.erase(s.find_last_not_of(ws) + 1);
        return s;
    };

    std::stringstream ss(fileTypes);
    std::string token, out;
    bool first = true;
    while (std::getline(ss, token, ';'))
    {
        token = trim(token);
        if (token.empty())
            continue;
        if (token != "*")
        {
            if (token[0] == '*')
                token.erase(0, 1);
            if (!token.empty() && token[0] != '.')
                token = "." + token;
        }
        if (!first)
            out += ";";
        out += token;
        first = false;
    }
    return out.empty() ? std::string("*") : out;
}

//=====================================================================================
// Process populate configuration asynchronously to avoid blocking calling thread
// This is critical when called from audio thread or during state save operations
// The issue is that populate needs to do file I/O which can be slow,
// so we offload that work to a background thread and notify via callback when done
//=====================================================================================
void Parser::processPopulateAsync(const std::string& widgetChannel, const nlohmann::json& populateConfig, 
                                   std::function<void(const nlohmann::json&)> callback)
{
    std::thread([widgetChannel, populateConfig, callback]() {
        try {
            lattice::logDebug << "processPopulateAsync thread started for widget: " << widgetChannel;

            // Small delay to allow any pending file operations (like saves) to complete
            // This prevents race conditions where cabbageSet triggers a populate refresh
            // immediately after cabbageSaveState but before the file is written to disk
            std::this_thread::sleep_for(std::chrono::milliseconds(150));

            // All file I/O happens on this background thread
            nlohmann::json result;

            // Make a mutable copy of populateConfig for normalization
            nlohmann::json config = populateConfig;

            lattice::logDebug << "populateConfig received: " << config.dump();

            // Validate required fields
            if (!config.contains("directories")) {
                lattice::logError << "populate missing 'directories' for widget: " << widgetChannel;
                return;
            }

            // Convert single string to array for consistency
            if (config["directories"].is_string()) {
                std::string singleDir = config["directories"].get<std::string>();
                config["directories"] = nlohmann::json::array({singleDir});
                lattice::logDebug << "Converted directories to array: " << config["directories"].dump();
            }

            if (!config["directories"].is_array()) {
                lattice::logError << "populate 'directories' must be a string or array for widget: " << widgetChannel;
                return;
            }

            // Normalize the fileType filter (see normalizePopulateFileTypes):
            // empty/missing means "all files"; "*.ext" and bare "ext" become
            // ".ext". Without this, the documented "*.wav" form and the ""
            // default match nothing downstream.
            if (!config.contains("fileType") || !config["fileType"].is_string() ||
                config["fileType"].get<std::string>().empty())
            {
                config["fileType"] = "*";
                lattice::logDebug << "populate fileType empty/missing for widget: " << widgetChannel
                                  << " - matching all files";
            }
            else
            {
                const std::string rawType = config["fileType"].get<std::string>();
                const std::string normalizedType = normalizePopulateFileTypes(rawType);
                if (normalizedType != rawType)
                    lattice::logDebug << "populate fileType normalized '" << rawType << "' -> '"
                                      << normalizedType << "' for widget: " << widgetChannel;
                config["fileType"] = normalizedType;
            }

            std::string fileType = cabbage::Utils::sanitisePath(config["fileType"].get<std::string>());
            lattice::logDebug << "fileType: " << fileType;

            // Collect directories from array
            std::vector<std::string> directories;
            for (const auto& dir : config["directories"]) {
                if (dir.is_string()) {
                    directories.push_back(dir.get<std::string>());
//                    lattice::logDebug << "Adding directory: " << dir.get<std::string>();
                }
            }

            // Collect files from all directories, avoiding duplicates.
            // Flat (non-recursive) by default - nested scans are opt-in via
            // "recursive": true. Results are capped (kMaxPopulateFiles) so
            // pathological trees can't stall the scan or flood the UI with an
            // unrenderable list; the cap applies in scan order, before sorting.
            const bool recursive = config.value("recursive", false);
            const bool matchAll = (fileType == "*");
            std::unordered_set<std::string> extensions;
            if (!matchAll)
            {
                std::stringstream ss(fileType);
                std::string token;
                while (std::getline(ss, token, ';'))
                {
                    std::transform(token.begin(), token.end(), token.begin(),
                                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
                    if (!token.empty())
                        extensions.insert(token);
                }
            }

            std::vector<std::string> files;
            std::unordered_set<std::string> seenFiles;
            bool truncated = false;
            for (const auto& directory : directories) {
//                lattice::logDebug << "Scanning directory: " << directory;
                try {
                    // If directory doesn't exist, warn and continue
                    if (!std::filesystem::exists(directory) || !std::filesystem::is_directory(directory)) {
                        lattice::logError << "populate directory does not exist or is not a directory: " << directory;
                        continue;
                    }

                    const size_t budget = (files.size() >= kMaxPopulateFiles) ? 0
                                                                             : (kMaxPopulateFiles - files.size());
                    bool dirTruncated = false;
                    auto dirFiles = collectPopulateFiles(directory, extensions, matchAll, recursive,
                                                          budget, dirTruncated);
                    truncated = truncated || dirTruncated;
                    lattice::logDebug << "Found " << dirFiles.size() << " files in directory: " << directory
                                      << (recursive ? " (recursive)" : " (flat)");
                    for (const auto &f : dirFiles) {
                        if (seenFiles.insert(f).second) {
                            files.push_back(f);
                        } else {
                            lattice::logDebug << "Skipping duplicate file: " << f;
                        }
                    }
                    if (truncated)
                        break; // budget exhausted - further directories can't contribute
                } catch (const std::exception &e) {
                    lattice::logError << "Error scanning directory '" << directory << "': " << e.what();
                    continue;
                } catch (...) {
                    lattice::logError << "Unknown error scanning directory: " << directory;
                    continue;
                }
            }

            if (files.empty()) {
                // Build directory list string for logging
                std::string dirList;
                for (size_t i = 0; i < directories.size(); ++i) {
                    dirList += directories[i];
                    if (i < directories.size() - 1) dirList += ", ";
                }
                lattice::logError << "No files found in directories [" << dirList << "] with type '" << fileType << "'";
            }
            
            // Apply sorting
            std::string orderType = config.value("order", "alphanumeric");
            
            if (orderType == "date") {
                // Cache file modification times to avoid race conditions during sort
                struct FileWithTime {
                    std::string path;
                    std::filesystem::file_time_type time;
                };
                
                std::vector<FileWithTime> filesWithTime;
                filesWithTime.reserve(files.size());
                
                for (const auto &file : files) {
                    try {
                        if (std::filesystem::exists(file)) {
                            filesWithTime.push_back({file, std::filesystem::last_write_time(file)});
                        } else {
                            // File disappeared, use epoch time
                            filesWithTime.push_back({file, std::filesystem::file_time_type{}});
                        }
                    } catch (...) {
                        // Error reading time, use epoch time
                        filesWithTime.push_back({file, std::filesystem::file_time_type{}});
                    }
                }
                
                // Sort using cached times
                std::sort(filesWithTime.begin(), filesWithTime.end(),
                    [](const FileWithTime &a, const FileWithTime &b) -> bool {
                        return a.time < b.time;
                    });
                
                // Extract sorted paths
                files.clear();
                files.reserve(filesWithTime.size());
                for (const auto &item : filesWithTime) {
                    files.push_back(item.path);
                }
                
            } else if (orderType == "size") {
                // Cache file sizes to avoid race conditions during sort
                struct FileWithSize {
                    std::string path;
                    std::uintmax_t size;
                };
                
                std::vector<FileWithSize> filesWithSize;
                filesWithSize.reserve(files.size());
                
                for (const auto &file : files) {
                    try {
                        if (std::filesystem::exists(file)) {
                            filesWithSize.push_back({file, std::filesystem::file_size(file)});
                        } else {
                            // File disappeared, use size 0
                            filesWithSize.push_back({file, 0});
                        }
                    } catch (...) {
                        // Error reading size, use size 0
                        filesWithSize.push_back({file, 0});
                    }
                }
                
                // Sort using cached sizes (largest first)
                std::sort(filesWithSize.begin(), filesWithSize.end(),
                    [](const FileWithSize &a, const FileWithSize &b) -> bool {
                        return a.size > b.size;
                    });
                
                // Extract sorted paths
                files.clear();
                files.reserve(filesWithSize.size());
                for (const auto &item : filesWithSize) {
                    files.push_back(item.path);
                }
            }
            else
            {
                // Default order: natural sort (numeric-aware) replaces the
                // lattice traversal order now that collection is cabbage-side.
                std::sort(files.begin(), files.end(), naturalLess);
            }

            if (truncated) {
                lattice::logError << "populate results truncated to " << kMaxPopulateFiles
                                    << " items for widget '" << widgetChannel
                                    << "' - refine directories/fileType";
            }

            // Build result JSON
            result["populate"] = config;

            std::vector<std::string> items;

            lattice::logDebug << "Total files found: " << files.size();

            if (!files.empty()) {
                // Always provide full paths - frontend can strip them for display if needed
                items = files;
                lattice::logDebug << "Setting items to files list";
            } else if (config.contains("labelWhenEmpty") && !directories.empty()) {
                // Only use labelWhenEmpty if directories were actually specified
                // Check that at least one directory is non-empty
                bool hasValidDirectory = std::any_of(directories.begin(), directories.end(),
                    [](const std::string& dir) { return !dir.empty(); });
                if (hasValidDirectory) {
                    items.push_back(config["labelWhenEmpty"].get<std::string>());
                    lattice::logDebug << "No files found, using labelWhenEmpty";
                } else {
                    lattice::logDebug << "No valid directories specified, skipping labelWhenEmpty";
                }
            } else {
                lattice::logDebug << "No files found and no labelWhenEmpty";
            }

            result["items"] = items;
            lattice::logDebug << "Result items count: " << result["items"].size();

            // Notify via callback
            if (callback) {
                lattice::logDebug << "Calling callback with result";
            } else {
                lattice::logError << "No callback provided for populate";
            }
            if (callback) {
                callback(result);
            }
            
        } catch (const std::exception &e) {
            lattice::logError << "Error in processPopulateAsync: " << e.what();
        }
    }).detach();
}

} // namespace cabbage
