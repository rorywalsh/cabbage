 <Cabbage>
form caption("Internal State Opcodes") size(400, 150), pluginId("def1")
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

instr UpdateData
    cabbageWriteStateData 1, {{
        { 
        "happy": true, 
        "pi": 1.141, 
        "test": "hello", 
        "list":[1, 0, 2, 3, 4, 5, 2, 3, 6], 
        "stringList":["hi", "who", "goes", "there"]
        }
    }}
endin

instr ShowData
    prints cabbageReadStateData()
endin

instr GetFloatValue
    kVal cabbageGetStateValue "pi"
    printk2 kVal
endin

instr GetFloatArrayValue
    kVal[] cabbageGetStateValue "list"
    printarray kVal, 1, "%d"
endin

instr GetStringValue
    SVal cabbageGetStateValue "test"
    prints SVal
endin

instr GetStringArrayValue
    SVal[] cabbageGetStateValue "stringList"
    printarray SVal, 1, "%s"
endin

instr SetFloatValue
    cabbageSetStateValue "pi", chnget:i("sliderValue")
endin

instr SetFloatArrayValue
    kArr[] fillarray 1, 2, 3, 4
    cabbageSetStateValue "list", kArr
endin

instr SetStringArrayValue
    SArr[] fillarray "1", "2", "3", "4"
    cabbageSetStateValue "stringList", SArr
endin
</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
f0 z
i2 0 z
;starts instrument 1 and runs it for a week
;i"WriteData" 0 1
;i"ShowData" + 1  
;i"UpdateData" 0 1
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
