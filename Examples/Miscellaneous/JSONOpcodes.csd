 <Cabbage>
form caption("Untitled") size(400, 300), colour(60, 60, 60), pluginId("def1")
rslider bounds(14, 14, 100, 100), channel("sliderValue"), range(0, 1, 0.5, 1, 0.01), text("SliderValue"), trackerColour(0, 255, 0, 255), outlineColour(0, 0, 0, 50), textColour(0, 0, 0, 255)
button bounds(122, 16, 120, 40), channel("showData"), text("Print State Data")
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 -m0d 
</CsOptions>
<CsInstruments>
; Initialize the global variables. 
ksmps = 32
nchnls = 2
0dbfs = 1

instr 1
    a1 oscili 1, 200
    outs a1, a1
endin

instr 2

kSlider chnget "sliderValue"

if changed2(kSlider) == 1 then
    event "i", "SetFloatValue", 0, 0
endif

if changed:k(chnget:k("showData")) == 1 then
    event "i", "ShowData", 0, 0
endif

endin

;instr WriteData
;    iRes writeStateData 0, "{ \"happy\": true, \"pi\": 3.141, \"list\":[1, 0, 2, 3, 4, 5, 2, 3, 6], \"stringList\":[\"Hi\", \"who\", \"goes\", \"there\"]}"
;    if iRes == -1 then
;        prints "Couldn't write JSON data"
;    endif
;endin

instr UpdateData
    iRes writeStateData 1, {{
        { 
        "happy": true, "pi": 1.141, "test": "hello", "list":[1, 0, 2, 3, 4, 5, 2, 3, 6], "stringList":["hi", "who", "goes", "there]
        }
    }}
    if iRes == -1 then
        prints "Couldn't write JSON data"
    endif
endin

instr ShowData
    prints readStateData()
endin

instr GetFloatValue
    kVal getStateValue "pi"
    printk2 kVal
endin

instr GetFloatArrayValue
    kVal[] getStateValue "list"
    printarray kVal, 1, "%d"
endin

instr GetStringValue
    SVal getStateValue "test"
    prints SVal
endin

instr GetStringArrayValue
    SVal[] getStateValue "stringList"
    printarray SVal, 1, "%s"
endin

instr SetFloatValue
    iRes setStateValue "pi", chnget:i("sliderValue")
endin

instr SetFloatArrayValue
    kArr[] fillarray 1, 2, 3, 4
    kRes setStateValue "list", kArr
endin

instr SetStringArrayValue
    SArr[] fillarray "1", "2", "3", "4"
    kRes setStateValue "stringList", SArr
endin
</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
f0 z
i2 0 z
;starts instrument 1 and runs it for a week
;i"WriteData" 0 1
;i"ShowData" + 1  
i"UpdateData" 0 1
;i"ShowData" 2 1
;i"GetFloatValue" 3 1
;i"GetFloatArrayValue" 4 1
;i"GetStringValue" 5 1
;i"GetStringArrayValue" 6 1
;i"SetFloatValue" 2 1
i"SetStringArrayValue" 1 1
i"ShowData" 3 1
i"GetStringArrayValue" 4 1
</CsScore>
</CsoundSynthesizer>
