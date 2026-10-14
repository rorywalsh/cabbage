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
#undef OK

#include <fstream>
#include <string>
#include <array>
#include <algorithm>
#include <complex>
#include <cstring>
#include <iostream>

/**
 * There is a conflict between the preprocessor definition "_CR" in the
 * standard C++ library and in Csound. To work around this, undefine "_CR" and
 * include ALL standard library include files BEFORE including ANY Csound
 * include files.
 */
#undef _CR

#include <plugin.h>
#include "CabbageOpcodes.h"

// SWidgetIds[] cabbageGetWidgetIds
// Returns an array of widget channel IDs, flattened to include nested
// children[]. Uses the top-level id if it exists, otherwise falls back
// to channels[0].id.
struct CabbageGetWidgetIds : csnd::Plugin<1, 0>
{
    int init() { return getIds(); };
    int getIds();
};

// SWidgetIds[] cabbageGetWidgetIds SfilterPath, SfilterValue
// As above, but only returns IDs of widgets where the dot-notation property
// at SfilterPath (e.g. "type", "style.off.backgroundColor") matches the
// case-sensitive glob pattern in SfilterValue ("*" and "?" wildcards;
// a pattern without wildcards is an exact match).
struct CabbageGetWidgetIdsFiltered : csnd::Plugin<1, 2>, CabbageOpcodes<2>
{
    int init() { return getIds(); };
    int getIds();
};

// SJson cabbageGetWidget Schannel
// Returns the compact JSON object for the widget with the given channel id
// (top-level id or any channels[].id, including nested children).
// Returns an empty string and posts a message if the id is not found.
struct CabbageGetWidget : csnd::Plugin<1, 1>
{
    int init() { return getWidget(); };
    int getWidget();
};
