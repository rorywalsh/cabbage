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
#include "CabbageGetWidgetIdsOpcodes.h"
#include "../CabbageParser.h"

#include <cctype>
#include <vector>

namespace
{

// Read a Csound STRINGDAT safely (see CabbageOpcodes::getSafeString).
std::string safeInputString(STRINGDAT &strData)
{
    if (strData.data == nullptr || strData.size == 0)
        return {};
    const size_t len = strnlen(strData.data, static_cast<size_t>(strData.size) + 1);
    return {strData.data, len};
}

// Primary channel id for a widget: top-level id, else channels[0].id.
// Returns "" when the widget has no id.
std::string widgetId(const nlohmann::json &widget)
{
    if (widget.contains("id") && widget["id"].is_string())
        return cabbage::Parser::removeQuotes(widget["id"].get<std::string>());
    if (widget.contains("channels") && widget["channels"].is_array() && !widget["channels"].empty() &&
        widget["channels"][0].is_object() && widget["channels"][0].contains("id") &&
        widget["channels"][0]["id"].is_string())
    {
        return cabbage::Parser::removeQuotes(widget["channels"][0]["id"].get<std::string>());
    }
    return {};
}

bool isArrayIndex(const std::string &key)
{
    return !key.empty() && std::all_of(key.begin(), key.end(), ::isdigit);
}

// Resolve a dot-notation path (same convention as cabbageSet/cabbageGet)
// against a widget. Returns nullptr when the path does not exist.
const nlohmann::json *resolvePath(const nlohmann::json &widget, const std::string &path)
{
    if (path.empty())
        return nullptr;
    if (path.find('.') == std::string::npos)
    {
        if (widget.is_object() && widget.contains(path))
            return &widget[path];
        return nullptr;
    }
    const nlohmann::json *current = &widget;
    size_t start = 0;
    while (true)
    {
        const size_t dot = path.find('.', start);
        const std::string key = (dot == std::string::npos) ? path.substr(start) : path.substr(start, dot - start);
        if (current->is_array() && isArrayIndex(key))
        {
            const size_t idx = static_cast<size_t>(std::stoul(key));
            if (idx >= current->size())
                return nullptr;
            current = &(*current)[idx];
        }
        else if (current->is_object() && current->contains(key))
        {
            current = &(*current)[key];
        }
        else
        {
            return nullptr;
        }
        if (dot == std::string::npos)
            return current;
        start = dot + 1;
    }
}

// Stringify a JSON value for glob comparison: raw string for strings,
// JSON representation otherwise (numbers, booleans, arrays, objects).
std::string valueToString(const nlohmann::json &val)
{
    if (val.is_string())
        return cabbage::Parser::removeQuotes(val.get<std::string>());
    return val.dump();
}

// Case-sensitive glob match: '*' matches any sequence (incl. empty),
// '?' matches exactly one character, '\' escapes the next character.
bool globMatch(const std::string &value, const std::string &pattern)
{
    size_t v = 0, p = 0;
    size_t star = std::string::npos, starMatch = 0;
    while (v < value.size())
    {
        if (p < pattern.size() && pattern[p] == '\\' && p + 1 < pattern.size())
        {
            if (value[v] != pattern[p + 1])
            {
                if (star == std::string::npos)
                    return false;
                p = star + 1;
                v = ++starMatch;
                continue;
            }
            v++;
            p += 2;
        }
        else if (p < pattern.size() && (pattern[p] == '?' || pattern[p] == value[v]))
        {
            v++;
            p++;
        }
        else if (p < pattern.size() && pattern[p] == '*')
        {
            star = p++;
            starMatch = v;
        }
        else if (star != std::string::npos)
        {
            p = star + 1;
            v = ++starMatch;
        }
        else
        {
            return false;
        }
    }
    while (p < pattern.size() && pattern[p] == '*')
        p++;
    return p == pattern.size();
}

bool widgetMatches(const nlohmann::json &widget, const std::string &filterPath, const std::string &filterValue)
{
    const nlohmann::json *val = resolvePath(widget, filterPath);
    if (val == nullptr || val->is_null())
        return false;
    return globMatch(valueToString(*val), filterValue);
}

// Recursively collect (flatten) widget ids, including nested children[].
// When filterPath is non-empty, only widgets matching the glob are collected.
// Widgets without an id are skipped.
void collectIds(const nlohmann::json &array, std::vector<std::string> &ids, const std::string &filterPath,
                const std::string &filterValue)
{
    if (!array.is_array())
        return;
    const bool filtering = !filterPath.empty();
    for (const auto &widget : array)
    {
        if (!widget.is_object())
            continue;
        if (!filtering || widgetMatches(widget, filterPath, filterValue))
        {
            if (const auto id = widgetId(widget); !id.empty())
                ids.push_back(id);
        }
        if (widget.contains("children") && widget["children"].is_array())
            collectIds(widget["children"], ids, filterPath, filterValue);
    }
}

} // namespace

//=====================================================================================
// SWidgetIds[] cabbageGetWidgetIds
// Returns an array of widget channel IDs, flattened to include nested
// children[]. Uses the top-level id if it exists, otherwise falls back
// to channels[0].id. Widgets without an id are skipped.
//=====================================================================================
int CabbageGetWidgetIds::getIds()
{
    auto *hostData = static_cast<cabbage::Engine *>(csound->host_data());

    // Snapshot the widgets into a json array so the recursive walk below
    // shares one code path for top-level and nested children.
    nlohmann::json array = nlohmann::json::array();
    for (const auto &w : hostData->getWidgets())
        array.push_back(w);

    std::vector<std::string> ids;
    ids.reserve(array.size());
    collectIds(array, ids, {}, {});

    csnd::Vector<STRINGDAT> &out = outargs.vector_data<STRINGDAT>(0);
    out.init(csound, static_cast<int>(ids.size()), this->insdshead);
    if (ids.empty())
        return IS_OK;
    // Detach shared storage before writing (see CabbageJsonOpcodes).
    STRINGDAT *dest = out.writable_data_init(csound, this->insdshead);
    if (dest == nullptr)
        return NOT_OK;

    for (size_t i = 0; i < ids.size(); i++)
    {
        dest[i].size = static_cast<int>(ids[i].size() + 1);
        dest[i].data = csound->strdup(const_cast<char *>(ids[i].c_str()));
    }

    return IS_OK;
}

//=====================================================================================
// SWidgetIds[] cabbageGetWidgetIds SfilterPath, SfilterValue
// As above, but only returns IDs of widgets where the dot-notation property
// at SfilterPath matches the case-sensitive glob pattern in SfilterValue.
//=====================================================================================
int CabbageGetWidgetIdsFiltered::getIds()
{
    auto *hostData = static_cast<cabbage::Engine *>(csound->host_data());
    const std::string filterPath = getSafeString(inargs, 0);
    const std::string filterValue = getSafeString(inargs, 1);

    nlohmann::json array = nlohmann::json::array();
    for (const auto &w : hostData->getWidgets())
        array.push_back(w);

    std::vector<std::string> ids;
    ids.reserve(array.size());
    collectIds(array, ids, filterPath, filterValue);

    csnd::Vector<STRINGDAT> &out = outargs.vector_data<STRINGDAT>(0);
    out.init(csound, static_cast<int>(ids.size()), this->insdshead);
    if (ids.empty())
        return IS_OK;
    // Detach shared storage before writing (see CabbageJsonOpcodes).
    STRINGDAT *dest = out.writable_data_init(csound, this->insdshead);
    if (dest == nullptr)
        return NOT_OK;

    for (size_t i = 0; i < ids.size(); i++)
    {
        dest[i].size = static_cast<int>(ids[i].size() + 1);
        dest[i].data = csound->strdup(const_cast<char *>(ids[i].c_str()));
    }

    return IS_OK;
}

//=====================================================================================
// SJson cabbageGetWidget Schannel
// Returns the compact JSON object for the widget with the given channel id.
// Accepts a top-level id or any channels[].id, including nested children.
// Returns an empty string (plus a console message) when not found.
//=====================================================================================
int CabbageGetWidget::getWidget()
{
    auto *hostData = static_cast<cabbage::Engine *>(csound->host_data());
    const std::string channel = safeInputString(inargs.str_data(0));

    const auto widget = hostData->getWidgetCopyById(channel);
    std::string json;
    if (widget.has_value())
        json = widget->dump();

    if (json.empty() && !widget.has_value())
        csound->message(("cabbageGetWidget: widget '" + channel + "' not found").c_str());

    outargs.str_data(0).size = static_cast<int>(json.size() + 1);
    outargs.str_data(0).data = csound->strdup(const_cast<char *>(json.c_str()));

    return IS_OK;
}
