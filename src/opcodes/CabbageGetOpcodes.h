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
#include "CabbageSetOpcodes.h"
#include "CabbageTrigStructs.h"

struct CabbageDump : csnd::InPlug<2>
{
    int init() { return dump(CabbageOpcodeData::PassType::Init); };
    int dump(int init);
};

struct CabbageDumpWithTrigger : csnd::InPlug<3>
{
    int kperf() { return dump(CabbageOpcodeData::PassType::Perf); };
    int dump(int init);
};

struct CabbageGetValue : csnd::Plugin<1, 1>
{
    int init() { return getValue(CabbageOpcodeData::PassType::Init); };
    int kperf() { return getValue(CabbageOpcodeData::PassType::Perf); };
    int getValue(int init);
};

struct CabbageGetValueWithTrigger : csnd::Plugin<2, 1>
{
    int init() { return getValue(CabbageOpcodeData::PassType::Init); };
    int kperf() { return getValue(CabbageOpcodeData::PassType::Perf); };
    int getValue(int init);
};

struct CabbageGetValueString : csnd::Plugin<1, 1>
{
    int init() { return getValue(CabbageOpcodeData::PassType::Init); };
    int kperf() { return getValue(CabbageOpcodeData::PassType::Perf); };
    int getValue(int init);
};

struct CabbageGetValueStringWithTrigger : csnd::Plugin<2, 1>
{
    int init() { return getValue(CabbageOpcodeData::PassType::Init); };
    int kperf() { return getValue(CabbageOpcodeData::PassType::Perf); };
    int getValue(int init);
};

// res:CabbageNumTrig cabbageGetValue "channel"
// Struct overload of the kk trigger form: res.val/res.trig replace kVal/kTrig.
// NOTE: stateless by necessity (see CabbageJsonOpcodes.h): trigger history
// lives in the shared numeric checkTriggerChanged memo, keyed by channel,
// so this form is safe inside k-rate loops over multiple channels.
struct CabbageGetValueStruct : csnd::Plugin<1, 1>
{
    int init()
    {
        return getValue(CabbageOpcodeData::PassType::Init);
    };
    int kperf() { return getValue(CabbageOpcodeData::PassType::Perf); };
    int getValue(int init);
};

// res:CabbageStrTrig cabbageGetValue "channel"
// Struct overload of the Sk trigger form. Trigger history lives in the
// shared checkTriggerChanged memo, keyed by channel — the same memo the Sk
// form uses, so both agree edge-for-edge (safe in k-rate loops). First
// sight of a non-empty string fires, matching the cabbageGet Sk convention;
// an empty channel stays silent.
struct CabbageGetValueStringStruct : csnd::Plugin<1, 1>
{
    int kperf();
};

struct CabbageGetMYFLT : csnd::Plugin<1, 2>, CabbageOpcodes<2>
{
    cs_float *value;
    int init() { return getIdentifier(CabbageOpcodeData::PassType::Init); };
    int kperf() { return getIdentifier(CabbageOpcodeData::PassType::Perf); };
    int getIdentifier(int init);
};

struct CabbageGetString : csnd::Plugin<1, 2>, CabbageOpcodes<2>
{
    int init() { return getIdentifier(CabbageOpcodeData::PassType::Init); };
    int kperf() { return getIdentifier(CabbageOpcodeData::PassType::Perf); };
    int getIdentifier(int init);
};

struct CabbageGetStringArray : csnd::Plugin<1, 2>, CabbageOpcodes<2>
{
    int init() { return getIdentifier(CabbageOpcodeData::PassType::Init); };
    int kperf() { return getIdentifier(CabbageOpcodeData::PassType::Perf); };
    int getIdentifier(int init);
};

struct CabbageGetStringWithTrigger : csnd::Plugin<2, 2>, CabbageOpcodes<2>
{
    // NOTE: stateless by necessity (see CabbageJsonOpcodes.h): trigger
    // history lives in the shared checkTriggerChanged memo, keyed by
    // channel + identifier.
    int kperf() { return getIdentifier(CabbageOpcodeData::PassType::Perf); };
    int getIdentifier(int init);
};

// res:CabbageStrTrig cabbageGet "channel", "identifier"
// Struct overload of the Sk trigger form; shares the same trigger memo.
struct CabbageGetStringStruct : csnd::Plugin<1, 2>, CabbageOpcodes<2>
{
    int kperf() { return getIdentifier(CabbageOpcodeData::PassType::Perf); };
    int getIdentifier(int init);
};

// iHasKey cabbageHasKey "channel", "key"
// Returns 1 if the widget identified by "channel" has the given JSON property key, 0 otherwise.
// Supports dot-notation for nested keys, e.g. "bounds.left".
struct CabbageWidgetHasKey : csnd::Plugin<1, 2>, CabbageOpcodes<2>
{
    int init() { return check(); };
    int kperf() { return check(); };
    int check();
};
