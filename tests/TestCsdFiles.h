#pragma once
#include <string>

namespace TestCsdFiles {

const std::string basicOscillator = R"(
<Cabbage>[
{"type": "form", "caption": "Dummy Example", "size": {"width": 360.0, "height": 460.0}, "guiMode": "queue", "pluginId": "def1"}
]</Cabbage>
<CsoundSynthesizer>
<CsOptions>
</CsOptions>
<CsInstruments>
sr = 44100
ksmps = 32
nchnls = 2
0dbfs = 1

instr 1
    aout oscili 0.1, 440
    outs aout, aout
endin
</CsInstruments>
<CsScore>
i 1 0 10
</CsScore>
</CsoundSynthesizer>
)";

const std::string rotarySliders = R"(
<Cabbage>[
{"type": "form", "caption": "Slider Example", "size": {"width": 360.0, "height": 460.0}, "guiMode": "queue", "pluginId": "def1"},
{"type": "rotarySlider", "bounds": {"left": 20.0, "top": 20.0, "width": 80.0, "height": 80.0}, "channels": [{"event": "valueChanged", "id": "harmonic1", "range": {"min": 0.0, "max": 1.0,  "defaultValue": 0.0, "skew": 1.0, "increment": 0.001}}]},
{"type": "rotarySlider", "bounds": {"left": 100.0, "top": 20.0, "width": 80.0, "height": 80.0}, "channels": [{"event": "valueChanged", "id": "harmonic2", "range": {"min": 0.0, "max": 1.0, "defaultValue": 0.0, "skew": 1.0, "increment": 0.001}}]},
{"type": "rotarySlider", "bounds": {"left": 180.0, "top": 20.0, "width": 80.0, "height": 80.0}, "channels": [{"event": "valueChanged", "id": "harmonic3", "range": {"min": 0.0, "max": 1.0, "defaultValue": 0.0, "skew": 1.0, "increment": 0.001}}]},
{"type": "rotarySlider", "bounds": {"left": 260.0, "top": 20.0, "width": 80.0, "height": 80.0}, "channels": [{"event": "valueChanged", "id": "harmonic4", "range": {"min": 0.0, "max": 1.0, "defaultValue": 0.0, "skew": 1.0, "increment": 0.001}}]},
{"type": "rotarySlider", "bounds": {"left": 20.0, "top": 100.0, "width": 80.0, "height": 80.0}, "channels": [{"event": "valueChanged", "id": "harmonic5", "range": {"min": 0.0, "max": 1.0, "defaultValue": 0.0, "skew": 1.0, "increment": 0.001}}]},
{"type": "rotarySlider", "bounds": {"left": 100.0, "top": 100.0, "width": 80.0, "height": 80.0}, "channels": [{"event": "valueChanged", "id": "harmonic6", "range": {"min": 0.0, "max": 1.0,"defaultValue": 0.0, "skew": 1.0, "increment": 0.001}}]},
{"type": "rotarySlider", "bounds": {"left": 180.0, "top": 100.0, "width": 80.0, "height": 80.0}, "channels": [{"event": "valueChanged", "id": "harmonic7", "range": {"min": 0.0, "max": 1.0,"defaultValue": 0.0, "skew": 1.0, "increment": 0.001}}]},
{"type": "rotarySlider", "bounds": {"left": 260.0, "top": 100.0, "width": 80.0, "height": 80.0}, "channels": [{"event": "valueChanged", "id": "harmonic8", "range": {"min": 0.0, "max": 1.0,"defaultValue": 0.0, "skew": 1.0, "increment": 0.001}}]}
]</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d
</CsOptions>
<CsInstruments>
; Initialize the global variables. 
ksmps = 1
nchnls = 2
0dbfs = 1

; Rory Walsh 2021 
;
; License: CC0 1.0 Universal
; You can copy, modify, and distribute this file, 
; even for commercial purposes, all without asking permission. 

instr 1   
    printf("Harmonic1 value: %f\n", cabbageGetValue:k("harmonic1"), cabbageGetValue:k("harmonic1"))
    printf("Harmonic2 value: %f\n", cabbageGetValue:k("harmonic2"), cabbageGetValue:k("harmonic2"))
    printf("Harmonic3 value: %f\n", cabbageGetValue:k("harmonic3"), cabbageGetValue:k("harmonic3"))
    printf("Harmonic4 value: %f\n", cabbageGetValue:k("harmonic4"), cabbageGetValue:k("harmonic4"))
    printf("Harmonic5 value: %f\n", cabbageGetValue:k("harmonic5"), cabbageGetValue:k("harmonic5"))
    printf("Harmonic6 value: %f\n", cabbageGetValue:k("harmonic6"), cabbageGetValue:k("harmonic6"))
    printf("Harmonic7 value: %f\n", cabbageGetValue:k("harmonic7"), cabbageGetValue:k("harmonic7"))
    printf("Harmonic8 value: %f\n", cabbageGetValue:k("harmonic8"), cabbageGetValue:k("harmonic8"))
endin       

</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
f0 z
;starts instrument 1 and runs it for a week
i1 0 z
</CsScore>
</CsoundSynthesizer>

)";

const std::string cabbageSet = R"(
<Cabbage>[
{"type": "form", "caption": "Slider Example", "size": {"width": 360.0, "height": 460.0}, "guiMode": "queue", "pluginId": "def1"}
]</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d
</CsOptions>
<CsInstruments>
; Initialize the global variables. 
ksmps = 1
nchnls = 2
0dbfs = 1

; Rory Walsh 2021 
;
; License: CC0 1.0 Universal
; You can copy, modify, and distribute this file, 
; even for commercial purposes, all without asking permission. 

instr 1   
    cabbageSet "dummy", "colour.fill", "222222"
    cabbageSet "dummy", "text", "Hello from Csound!"
    cabbageSetValue "dummy", oscili:k(1, 10)
endin           

</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
;starts instrument 1 and runs it for a week
i1 0 .1
</CsScore>
</CsoundSynthesizer>

)";
const std::string cabbageJson = R"(
<Cabbage>[
{"type": "form", "caption": "JSON Opcode Test", "size": {"width": 360.0, "height": 460.0}, "guiMode": "queue", "pluginId": "def1"}
]</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d
</CsOptions>
<CsInstruments>
ksmps = 32
nchnls = 2
0dbfs = 1

instr 1
    Sdoc = "{\"name\":\"test\",\"freq\":440,\"strnum\":\"42\",\"amp\":0.5,\"ok\":true,\"nothing\":null,\"tags\":[\"a\",\"b\",\"c\"],\"gains\":[1,2.5,3],\"nodes\":[{\"id\":\"n1\",\"params\":{\"rate\":2.5}}]}"

    Swave cabbageJsonGet Sdoc, "name"
    cabbageSet "ps_wave", "label.text", Swave
    Smiss cabbageJsonGet Sdoc, "nodes.5.nope"
    cabbageSet "ps_miss", "label.text", Smiss
    Samp cabbageJsonGet Sdoc, "amp"
    cabbageSet "ps_amp", "label.text", Samp
    Stags[] cabbageJsonGet Sdoc, "tags"
    cabbageSet "ps_tag0", "label.text", Stags[0]
    cabbageSet "ps_tag2", "label.text", Stags[2]
    Stype_rate cabbageJsonType Sdoc, "nodes.0.params.rate"
    cabbageSet "ps_type_rate", "label.text", Stype_rate
    Stype_tags cabbageJsonType Sdoc, "tags"
    cabbageSet "ps_type_tags", "label.text", Stype_tags
    Stype_nothing cabbageJsonType Sdoc, "nothing"
    cabbageSet "ps_type_nothing", "label.text", Stype_nothing
    Stype_miss cabbageJsonType Sdoc, "nope"
    cabbageSet "ps_type_miss", "label.text", Stype_miss

    Sset cabbageJsonSet Sdoc, "nodes.0.params.rate", 99
    Snewrate cabbageJsonGet Sset, "nodes.0.params.rate"
    cabbageSet "ps_newrate", "label.text", Snewrate
    Sset2 cabbageJsonSet Sdoc, "brand.new.path", "hi"
    Snewpath cabbageJsonGet Sset2, "brand.new.path"
    cabbageSet "ps_newpath", "label.text", Snewpath
    Ssetf cabbageJsonSet Sdoc, "freq", 880
    Snewfreq cabbageJsonGet Ssetf, "freq"
    cabbageSet "ps_newfreq", "label.text", Snewfreq

    Sbad cabbageJsonGet "{oops", "a"
    cabbageSet "ps_bad", "label.text", Sbad

    irate cabbageJsonGet Sdoc, "nodes.0.params.rate"
    cabbageSetValue "pr_rate", irate
    istrnum cabbageJsonGet Sdoc, "strnum"
    cabbageSetValue "pr_strnum", istrnum
    ibool cabbageJsonGet Sdoc, "ok"
    cabbageSetValue "pr_bool", ibool
    imiss cabbageJsonGet Sdoc, "nope"
    cabbageSetValue "pr_miss", imiss
    ihas1 cabbageJsonHas Sdoc, "nodes.0.id"
    cabbageSetValue "pr_has1", ihas1
    ihasnull cabbageJsonHas Sdoc, "nothing"
    cabbageSetValue "pr_hasnull", ihasnull
    ihas0 cabbageJsonHas Sdoc, "nope"
    cabbageSetValue "pr_has0", ihas0
    ilen cabbageJsonLen Sdoc, "nodes"
    cabbageSetValue "pr_len", ilen
    ilenobj cabbageJsonLen Sdoc, "nodes.0.params"
    cabbageSetValue "pr_lenobj", ilenobj
    ilen0 cabbageJsonLen Sdoc, "nope"
    cabbageSetValue "pr_len0", ilen0
    iempty lenarray Stags
    cabbageSetValue "pr_taglen", iempty
    StagsE[] cabbageJsonGet Sdoc, "name"
    iemptyE lenarray StagsE
    cabbageSetValue "pr_emptyarr", iemptyE

    kfreq cabbageJsonGet Sdoc, "freq"
    cabbageSetValue "pr_freq", kfreq
    kgains[] cabbageJsonGet Sdoc, "gains"
    cabbageSetValue "pr_gain1", kgains[1]
    kcount init 0
    kcount += 1
    SdocA = "{\"v\":0}"
    SdocB = "{\"v\":1}"
    Sdoc2 = kcount % 2 == 0 ? SdocA : SdocB
    Strig, kT cabbageJsonGet Sdoc2, "v"
    kfires init 0
    if kT == 1 then
        kfires += 1
    endif
    cabbageSetValue "pr_trigfires", kfires
endin

</CsInstruments>
<CsScore>
i1 0 1
</CsScore>
</CsoundSynthesizer>

)";
const std::string cabbageJsonFunc = R"(
<Cabbage>[
{"type": "form", "caption": "JSON Functional Syntax Probe", "size": {"width": 360.0, "height": 460.0}, "guiMode": "queue", "pluginId": "def1"}
]</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d
</CsOptions>
<CsInstruments>
ksmps = 32
nchnls = 2
0dbfs = 1

instr 1
    doc:S = "{\"name\":\"test\",\"freq\":440,\"tags\":[\"a\",\"b\"]}"
    wave:S = cabbageJsonGet(doc, "name")
    cabbageSet "ps_fwave", "label.text", wave
    len:i = cabbageJsonLen(doc, "tags")
    cabbageSetValue "pr_flen", len
    type:S = cabbageJsonType(doc, "freq")
    cabbageSet "ps_ftype", "label.text", type
    has:i = cabbageJsonHas(doc, "name")
    cabbageSetValue "pr_fhas", has
    freq:k = cabbageJsonGet(doc, "freq")
    cabbageSetValue "pr_ffreq", freq
    doc2:S = cabbageJsonSet(doc, "freq", 880)
    newfreq:i = cabbageJsonGet(doc2, "freq")
    cabbageSetValue "pr_fnewfreq", newfreq
    id:S, trig:k = cabbageJsonGet(doc, "name")
    cabbageSet trig, "ps_fid", "label.text", id
    fires:k init 0
    if trig == 1 then
        fires = 1
    endif
    cabbageSetValue "pr_ftrigseen", fires
endin

</CsInstruments>
<CsScore>
i1 0 1
</CsScore>
</CsoundSynthesizer>

)";

// Struct-only coverage for the cabbageGetValue trigger forms
// (res:CabbageNumTrig / res:CabbageStrTrig). Trigger history lives in the
// process-wide memo shared with the legacy forms, so a side-by-side pairing
// would let only the first reader of each key observe the edge; the struct
// forms are therefore the SOLE readers here and are checked against
// independently computed expectations (unseen key fires iff non-zero /
// non-empty, then on change only). Mismatches publish via cabbageSetValue.
const std::string trigStructs = R"(
<Cabbage>[
{"type": "form", "caption": "Trig Structs", "size": {"width": 360.0, "height": 200.0}, "guiMode": "queue", "pluginId": "def1"},
{"type": "label", "channels": [{"event": "valueChanged", "id": "tslbl"}], "bounds": {"left": 10.0, "top": 10.0, "width": 300.0, "height": 30.0}, "text": "struct trigger comparison"}
]</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d
</CsOptions>
<CsInstruments>
ksmps = 32
nchnls = 2
0dbfs = 1

instr 1
    kCnt init 0
    kCnt = kCnt + 1

    ; numeric channel steps 0 -> 1 -> 2 every 4 k-cycles
    kMod = kCnt % 12
    kPhas = 0
    if kMod >= 4 && kMod < 8 then
        kPhas = 1
    elseif kMod >= 8 then
        kPhas = 2
    endif
    chnset kPhas, "tsIn"

    ; string channel alternates every 4 k-cycles (S assignment inside a k-branch
    ; is lowered to init-time only; a k-conditional S expression re-evaluates)
    Stmp = kCnt % 8 < 4 ? "hello" : "world"
    chnset Stmp, "tsStr"

    ; --- struct forms are the SOLE readers: a second (legacy) reader on
    ; the same channel would consume the shared-memo edge first
    numRes:CabbageNumTrig cabbageGetValue "tsIn"
    strRes:CabbageStrTrig cabbageGetValue "tsStr"

    ; --- independent expectation under shared-memo semantics.
    ; k-rate S variables do not track across cycles (a plain S copy does
    ; not re-execute), so the string expectation is derived purely from
    ; the k-cycle counter: Stmp reads "hello" exactly when kCnt % 8 < 4,
    ; hence an edge lands on kCnt % 8 == 0 or 4. kFirst marks the first
    ; k-cycle, where the unseen memo key fires because "hello" is
    ; non-empty — mirroring the numeric expectation below.
    kFirst init 1
    kPrevPhas init -1
    kExpNumTrig = 0
    if kFirst == 1 && kPhas != 0 then
        kExpNumTrig = 1
    elseif kFirst == 0 && kPhas != kPrevPhas then
        kExpNumTrig = 1
    endif
    kMod8 = kCnt % 8
    kExpStrTrig = 0
    if kFirst == 1 && strcmpk(Stmp, "") != 0 then
        kExpStrTrig = 1
    elseif kFirst == 0 && (kMod8 == 0 || kMod8 == 4) then
        kExpStrTrig = 1
    endif

    kNumErr init 0
    if numRes.val != kPhas || numRes.trig != kExpNumTrig then
        kNumErr = kNumErr + 1
    endif
    kStrErr init 0
    if strcmpk(strRes.val, Stmp) != 0 || strRes.trig != kExpStrTrig then
        kStrErr = kStrErr + 1
    endif
    kFirst = 0
    kPrevPhas = kPhas

    ; the struct forms must actually fire (drives the > 0 assertions)
    kNumTrigStruct init 0
    kNumTrigStruct = kNumTrigStruct + numRes.trig
    kStrTrigStruct init 0
    kStrTrigStruct = kStrTrigStruct + strRes.trig

    kCycles init 0
    kCycles = kCycles + 1

    cabbageSetValue "tsNumErr", kNumErr
    cabbageSetValue "tsStrErr", kStrErr
    cabbageSetValue "tsNumTrigStruct", kNumTrigStruct
    cabbageSetValue "tsStrTrigStruct", kStrTrigStruct
    cabbageSetValue "tsCycles", kCycles
endin

</CsInstruments>
<CsScore>
i1 0 10
</CsScore>
</CsoundSynthesizer>

)";

// Struct-only coverage for the widget-property and JSON trigger forms
// (cabbageGet / cabbageJsonGet). These two share the process-wide
// TriggerMemo with their legacy siblings, so a side-by-side pairing in one
// run would let only the first reader of each key observe the edge; this CSD
// therefore exercises the struct forms alone against known expected values.
const std::string trigStructsSolo = R"(
<Cabbage>[
{"type": "form", "caption": "Trig Struct Solo", "size": {"width": 360.0, "height": 220.0}, "guiMode": "queue", "pluginId": "def2"},
{"type": "textEditor", "channels": [{"event": "valueChanged", "id": "twg1"}], "bounds": {"left": 10.0, "top": 10.0, "width": 200.0, "height": 40.0}, "text": "alpha", "readOnly": 1.0, "wrap": 1.0, "scrollbars": 1.0}
]</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d
</CsOptions>
<CsInstruments>
ksmps = 32
nchnls = 2
0dbfs = 1

instr 1
    ; widget property form: fires on first sight and when instr 2 changes the text
    propRes:CabbageStrTrig cabbageGet "twg1", "text"
    kPropFires init 0
    if propRes.trig == 1 then
        kPropFires = kPropFires + 1
    endif
    kPropIsBeta init 0
    if strcmpk(propRes.val, "beta") == 0 then
        kPropIsBeta = 1
    endif

    ; JSON form: document alternates every 4 k-cycles, value must track it
    kJCnt init 0
    kJCnt = kJCnt + 1
    Sja = "{\"v\":\"zero\"}"
    Sjb = "{\"v\":\"one\"}"
    Sjd = kJCnt % 8 < 4 ? Sja : Sjb
    jsonRes:CabbageStrTrig cabbageJsonGet Sjd, "v"
    kJsonFires init 0
    if jsonRes.trig == 1 then
        kJsonFires = kJsonFires + 1
    endif
    kJsonErr init 0
    if kJCnt % 8 < 4 then
        if strcmpk(jsonRes.val, "zero") != 0 then
            kJsonErr = kJsonErr + 1
        endif
    else
        if strcmpk(jsonRes.val, "one") != 0 then
            kJsonErr = kJsonErr + 1
        endif
    endif

    kCycles init 0
    kCycles = kCycles + 1

    cabbageSetValue "twgPropFires", kPropFires
    cabbageSetValue "twgPropIsBeta", kPropIsBeta
    cabbageSetValue "twgJsonFires", kJsonFires
    cabbageSetValue "twgJsonErr", kJsonErr
    cabbageSetValue "twgCycles", kCycles
endin

instr 2
    cabbageSet "twg1", "text", "beta"
endin

</CsInstruments>
<CsScore>
i1 0 10
i2 1 1
</CsScore>
</CsoundSynthesizer>

)";
const std::string widgetIdsAndWidget = R"(
<Cabbage>[
{"type": "form", "caption": "Widget Ids Example", "size": {"width": 360.0, "height": 460.0}, "guiMode": "queue", "pluginId": "def1"},
{"type": "rotarySlider", "bounds": {"left": 20.0, "top": 20.0, "width": 80.0, "height": 80.0}, "channels": [{"event": "valueChanged", "id": "widA", "range": {"min": 0.0, "max": 1.0, "defaultValue": 0.0, "skew": 1.0, "increment": 0.001}}]},
{"type": "rotarySlider", "bounds": {"left": 100.0, "top": 20.0, "width": 80.0, "height": 80.0}, "channels": [{"event": "valueChanged", "id": "widB", "range": {"min": 0.0, "max": 1.0, "defaultValue": 0.0, "skew": 1.0, "increment": 0.001}}]},
{"type": "button", "bounds": {"left": 180.0, "top": 20.0, "width": 117.0, "height": 30.0}, "channels": [{"id": "widC", "event": "valueChanged"}], "style": {"off": {"backgroundColor": "#993d00", "textColor": "#ffffff"}}},
{"type": "groupBox", "id": "grp", "bounds": {"left": 20.0, "top": 120.0, "width": 320.0, "height": 120.0}, "children": [
{"type": "rotarySlider", "bounds": {"left": 20.0, "top": 20.0, "width": 80.0, "height": 80.0}, "channels": [{"event": "valueChanged", "id": "childKnob", "range": {"min": 0.0, "max": 1.0, "defaultValue": 0.0, "skew": 1.0, "increment": 0.001}}]}
]}
]</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d
</CsOptions>
<CsInstruments>
ksmps = 32
nchnls = 2
0dbfs = 1

instr 1
    ids:S[] = cabbageGetWidgetIds()
    rot:S[] = cabbageGetWidgetIds("type", "rotarySlider")
    glob:S[] = cabbageGetWidgetIds("type", "*Slider")
    wid:S[] = cabbageGetWidgetIds("channels.0.id", "wid*")
    col:S[] = cabbageGetWidgetIds("style.off.backgroundColor", "#99*")
    json:S = cabbageGetWidget("widA")
    miss:S = cabbageGetWidget("noSuchWidget")

    count:i = lenarray(ids)
    cabbageSetValue("res_allCount", count)
    joined:S = ""
    idx:i = 0
    while idx < count do
        joined = strcat(joined, ids[idx])
        joined = strcat(joined, ",")
        idx += 1
    od
    cabbageSet("res_allIds", "label.text", joined)

    rotCount:i = lenarray(rot)
    cabbageSetValue("res_rotCount", rotCount)
    globCount:i = lenarray(glob)
    cabbageSetValue("res_globCount", globCount)
    widCount:i = lenarray(wid)
    cabbageSetValue("res_widCount", widCount)
    colCount:i = lenarray(col)
    cabbageSetValue("res_colCount", colCount)

    cabbageSet("res_json", "label.text", json)
    cabbageSet("res_miss", "label.text", miss)
endin

</CsInstruments>
<CsScore>
i1 0 1
</CsScore>
</CsoundSynthesizer>

)";
} // namespace TestCsdFiles