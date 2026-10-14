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
#undef _CR
/**
 * There is a conflict between the preprocessor definition "_CR" in the
 * standard C++ library and in Csound. To work around this, undefine "_CR" and
 * include ALL standard library include files BEFORE including ANY Csound
 * include files.
 */

#include <plugin.h>
#include <nlohmann/json.hpp>
#include <type_traits>
#include <mutex>
#include <map>
#include <lattice/LatticeUtils.h>

#define IS_OK 0
#define NOT_OK 1

// Process-wide memo for trigger opcodes (cabbageJsonGet-with-trigger and
// cabbageGet-with-trigger: "fire when the result changed").
// Rationale: Csound allocates opcode instances as raw memory and never runs
// C++ constructors, so per-instance std::string state is fatal (harmless on
// libc++ thanks to SSO, instant UB on libstdc++/MSVC). Keying by content
// keeps sharing transparent: a missing entry behaves exactly like a freshly
// constructed empty previous-result.
struct TriggerMemo
{
    std::mutex mutex;
    std::map<std::string, std::string> lastResults; // key: subject + '\x1F' + path
};

// Returns true when newResult differs from the recorded result for key
// (or key is unseen and newResult is non-empty), and records newResult.
// Thread-safe. Entries are bounded in practice by distinct subscriptions.
inline bool checkTriggerChanged(const std::string &key, const std::string &newResult)
{
    static TriggerMemo memo;
    std::lock_guard<std::mutex> lock(memo.mutex);
    if (memo.lastResults.size() > 4096)
        memo.lastResults.clear();
    auto it = memo.lastResults.find(key);
    const bool changed = (it == memo.lastResults.end()) ? !newResult.empty() : (it->second != newResult);
    memo.lastResults[key] = newResult;
    return changed;
}

// Numeric counterpart of the above, for value getters such as the trigger
// forms of cabbageGetValue. Uses exact != comparison, matching the previous
// per-instance currentValue logic. An unseen key behaves like a freshly
// zero-initialised instance: it reports changed iff newValue != 0.
inline bool checkTriggerChanged(const std::string &key, MYFLT newValue)
{
    struct NumericTriggerMemo
    {
        std::mutex mutex;
        std::map<std::string, MYFLT> lastValues; // key: channel
    };
    static NumericTriggerMemo memo;
    std::lock_guard<std::mutex> lock(memo.mutex);
    if (memo.lastValues.size() > 4096)
        memo.lastValues.clear();
    auto it = memo.lastValues.find(key);
    const bool changed = (it == memo.lastValues.end()) ? (newValue != 0) : (it->second != newValue);
    memo.lastValues[key] = newValue;
    return changed;
}

struct CabbageOpcodeData
{
    enum MessageType
    {
        Value,
        Identifier,
        Widget,
        Generic
    };

    enum PassType
    {
        Init,
        Perf
    };

    enum ArgType
    {
        Scalar,
        String,
        Array,
        StringArray
    };

    nlohmann::json cabbageJson = {};
    std::string channel = {};
    std::string identifier = {};
    MessageType type;
    bool skipPopulateProcessing = false;  // Set to true to prevent populate re-triggering
};

template <std::size_t NumInputParams>
struct CabbageOpcodes
{
    std::vector<nlohmann::json> **wd = nullptr;
    char *name = NULL;
    char *identifier = NULL;
    cs_float *value = {};
    cs_float *str = {};

    static bool hasNullTerminator(const char *str, size_t length)
    {
        for (size_t i = 0; i < length; ++i)
        {
            if (str[i] == '\0')
            {
                return true;
            }
        }
        return false;
    }

    static std::string removeNullTerminator(const std::string &str)
    {
        // Create a copy of the string
        std::string result = str;

        // If the last character is null terminator, remove it
        if (!result.empty() && result.back() == '\0')
        {
            result.pop_back();
        }

        return result;
    }

    // Helper to safely extract string from Csound STRINGDAT
    // Uses strlen to find the actual null-terminated string length,
    // preventing garbage characters from being included
    static std::string getSafeString(csnd::Param<NumInputParams> &args, int argIndex)
    {
        auto &strData = args.str_data(argIndex);
        if (strData.data == nullptr || strData.size == 0)
            return std::string();
        // Use strlen to find actual string length (up to null terminator)
        // Csound's size field may or may not include the null terminator,
        // so we use a reasonable upper bound and let strnlen find the actual end
        size_t maxLen = strData.size + 1; // Allow for null terminator at position size
        size_t len = strnlen(strData.data, maxLen);
        return std::string(strData.data, len);
    }

    bool hasNullTerminator(const std::string &str)
    {
        const char *cStr = str.c_str();
        size_t length = str.size();

        // Check if the character after the last element is a null terminator
        return cStr[length] == '\0';
    }

    CabbageOpcodeData getValueIdentData(csnd::Param<NumInputParams> &args, bool init, int nameIndex, int /*identIndex*/)
    {
        CabbageOpcodeData data;
        if (init)
        {
            if (args.str_data(nameIndex).size == 0)
                name = {};
            else
                name = args.str_data(nameIndex).data;
        }

        data.cabbageJson["value"] = 0;
        // Use getSafeString to properly extract string with correct length
        data.channel = getSafeString(args, nameIndex);
        return data;
    }

    // this check for brackets within strings, when argument is in form of text("this is a string")
    bool containsIllegalCharsWithinParentheses(const std::string &str)
    {
        size_t openParenthesis = str.find('(');
        if (openParenthesis == std::string::npos)
        {
            return false; // No opening parenthesis found
        }

        size_t closeParenthesis = str.rfind(')');
        if (closeParenthesis == std::string::npos || closeParenthesis < openParenthesis)
        {
            return false; // No closing parenthesis found or closing parenthesis is before opening parenthesis
        }

        // Check for () character within the parentheses
        for (size_t i = openParenthesis + 1; i < closeParenthesis; ++i)
        {
            if (str[i] == ')' || str[i] == '(')
                return true;
        }
        return false;
    }

    // this check for brackets within strings, when argument is in form of "this is a string", i.e, no identifier
    bool containsIllegalChars(const std::string &str)
    {
        // Check for both '(' and ')' using ||
        if (str.find('(') != std::string::npos || str.find(')') != std::string::npos)
        {
            return true;
        }
        return false;
    }

    // Function to split a dot notation string into a vector of keys
    std::vector<std::string> split(const std::string &str, char delimiter = '.')
    {
        std::vector<std::string> tokens;
        std::stringstream ss(str);
        std::string token;

        while (std::getline(ss, token, delimiter))
        {
            tokens.push_back(token);
        }
        return tokens;
    }

    // Function to set a value in a JSON object using dot notation
    void setJsonValue(nlohmann::json &jsonObj, const std::string &dotNotation, const nlohmann::json &value)
    {
        std::vector<std::string> keys = split(dotNotation, '.');
        nlohmann::json *current = &jsonObj;
        for (size_t i = 0; i < keys.size(); ++i)
        {
            const std::string &key = keys[i];

            // If we're at the last key, set the value
            if (i == keys.size() - 1)
            {
                (*current)[key] = value;
            }
            else
            {
                // If the key doesn't exist, create a new object
                if (!(*current).contains(key))
                {
                    (*current)[key] = nlohmann::json::object();
                }
                // Move deeper into the JSON structure
                current = &(*current)[key];
            }
        }
    }

    // Function to access a JSON object using dot notation
    nlohmann::json getJsonValue(const nlohmann::json &jsonObj, const std::string &jsonString)
    {
        if (jsonString.find(".") == std::string::npos)
        {
            if (jsonObj.is_array() && !jsonString.empty() && std::all_of(jsonString.begin(), jsonString.end(), ::isdigit))
            {
                size_t idx = std::stoul(jsonString);
                return (idx < jsonObj.size()) ? jsonObj[idx] : nullptr;
            }
            if (jsonObj.contains(jsonString))
                return jsonObj[jsonString];

            return nullptr; // Return null if the key does not exist
        }

        //        _log(jsonObj.dump(4));
        // else deal with dot notation
        std::vector<std::string> keys = split(jsonString, '.');
        nlohmann::json current = jsonObj;
        for (const auto &key : keys)
        {
            if (current.is_array() && !key.empty() && std::all_of(key.begin(), key.end(), ::isdigit))
            {
                size_t idx = std::stoul(key);
                if (idx >= current.size())
                    return nullptr;
                current = current[idx];
            }
            else if (current.contains(key))
            {
                current = current[key];
            }
            else
            {
                return nullptr; // Return null if the key does not exist
            }
        }
        return current;
    }

    void updateWidgetJson(nlohmann::json &jsonObj, csnd::Param<NumInputParams> &args, int argIndex,
                          std::string identifier, CabbageOpcodeData::ArgType argType)
    {
        // check if the identifier is already a JSON object, i.e, as in the case below
        // cabbageSet metro(1), "infoText", sprintf({{"text":"%s"}}, SText)
        std::vector<cs_float> array;
        auto j = parseAndFormatJson(identifier);
        auto it = j.begin();
        if (it == j.end())
        {
            // parseAndFormatJson returned an empty object — identifier was blank or
            // unparseable (e.g. a channel value that hasn't been set yet).  Bail
            // out cleanly rather than dereferencing a past-the-end iterator, which
            // would fire an nlohmann assert() and abort() the host process.
            lattice::logError << "cabbageSet: empty or unresolvable identifier '" << identifier
                                << "' for channel '" << name << "' — no update applied";
            return;
        }
        if (it.value().is_null())
        {
            if (identifier.find(".") == std::string::npos)
            {
                if (argType == CabbageOpcodeData::ArgType::String)
                {
                    jsonObj[identifier] = getSafeString(args, argIndex);
                }
                else if (argType == CabbageOpcodeData::ArgType::Array)
                {
                    if (args.myfltvec_data(argIndex).len() > 0)
                    {
                        csnd::Vector<cs_float> &arrayArgs = args.myfltvec_data(argIndex);
                        std::vector<cs_float> array(arrayArgs.begin(), arrayArgs.end());
                        jsonObj[identifier] = array;
                    }
                }
                else if (argType == CabbageOpcodeData::ArgType::StringArray)
                {
                    if (args.template vector_data<STRINGDAT>(argIndex).len() > 0)
                    {
                        csnd::Vector<STRINGDAT>& arrayArgs = args.template vector_data<STRINGDAT>(argIndex);
                        std::vector<std::string> array;
                        array.reserve(arrayArgs.len());
                        for (const auto& strdat : arrayArgs) {
                            // Use strnlen to find actual string length
                            // Allow for null terminator at position size
                            size_t maxLen = static_cast<size_t>(strdat.size) + 1;
                            size_t len = strnlen(strdat.data, maxLen);
                            array.push_back(std::string(strdat.data, len));
                        }
                        jsonObj[identifier] = array;
                    }
                }
                else
                {
                    jsonObj[identifier] = args[argIndex];
                }
            }
            else
            {
                // dot notation
                if (argType == CabbageOpcodeData::ArgType::String)
                {
                    setJsonValue(jsonObj, getSafeString(args, argIndex - 1), getSafeString(args, argIndex));
                }
                else if (argType == CabbageOpcodeData::ArgType::Scalar)
                {
                    setJsonValue(jsonObj, getSafeString(args, argIndex - 1), args[argIndex]);
                }
                else if (argType == CabbageOpcodeData::ArgType::Array)
                {
                    if (args.myfltvec_data(argIndex).len() > 0)
                    {
                        csnd::Vector<cs_float> &arrayArgs = args.myfltvec_data(argIndex);
                        std::vector<cs_float> array(arrayArgs.begin(), arrayArgs.end());
                        setJsonValue(jsonObj, getSafeString(args, argIndex - 1), array);
                    }
                }
                else if (argType == CabbageOpcodeData::ArgType::StringArray)
                {
                    if (args.template vector_data<STRINGDAT>(argIndex).len() > 0)
                    {
                        csnd::Vector<STRINGDAT>& arrayArgs = args.template vector_data<STRINGDAT>(argIndex);
                        std::vector<std::string> array;
                        array.reserve(arrayArgs.len());
                        for (const auto& strdat : arrayArgs) {
                            // Use strnlen to find actual string length
                            // Allow for null terminator at position size
                            size_t maxLen = static_cast<size_t>(strdat.size) + 1;
                            size_t len = strnlen(strdat.data, maxLen);
                            array.push_back(std::string(strdat.data, len));
                        }
                        setJsonValue(jsonObj, getSafeString(args, argIndex - 1), array);
                    }
                }
            }
        }
        else
        {
            // identifier arg was well formed JSON
            jsonObj = j;
        }
    }

    bool testForValidNumberOfInputs(int totalInputs, int minInputs) { return (totalInputs >= minInputs); }

    nlohmann::json parseAndFormatJson(const std::string &jsonString)
    {
        // Wrap the input string with braces to form a complete JSON object
        std::string wrappedJson = "{" + jsonString + "}";

        try
        {
            // Attempt to parse the wrapped JSON string
            if (nlohmann::json::accept(wrappedJson))
                return nlohmann::json::parse(wrappedJson);
            else
            {
                // If parsing fails, create a new JSON object with an empty key
                nlohmann::json fallbackJson;
                fallbackJson[split(jsonString)[0]] = nullptr;
                return fallbackJson;
            }
        }
        catch (const nlohmann::json::parse_error &e)
        {
            lattice::logDebug << "JSON parse error: " << e.what();
        }
        return {};
    }

    CabbageOpcodeData getIdentData(csnd::Csound *csound, csnd::Param<NumInputParams> &args, bool init, int channelIndex,
                                   int identIndex)
    {
        CabbageOpcodeData data;
        if (init)
        {
            if (args.str_data(channelIndex).size == 0)
                name = {};
            else
            {
                name = args.str_data(channelIndex).data;
            }

            if (args.str_data(identIndex).size == 0)
                identifier = {};
            else
            {
                if (containsIllegalCharsWithinParentheses(args.str_data(identIndex).data))
                {
                    csound->message("Cabbage Warning: Ill-formatted arguments passed to channel:\"" + data.channel +
                                    "\" Check for brackets within strings..");
                }
                identifier = args.str_data(identIndex).data;
            }
        }

        // Use getSafeString to properly extract strings with correct length
        data.identifier = getSafeString(args, identIndex);
        data.channel = getSafeString(args, channelIndex);

        try
        {
            // Use sanitized data.identifier, not the raw identifier member variable
            data.cabbageJson = parseAndFormatJson(data.identifier);
        }
        catch (const nlohmann::json::parse_error &e)
        {
            lattice::logDebug << "JSON parse error: ", e.what();
        }

        return data;
    }
};
