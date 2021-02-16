<Cabbage>
form caption("Untitled") size(400, 300), colour(58, 110, 182), pluginid("def1")
rslider bounds(296, 162, 100, 100), channel("gain"), range(0, 1, 0.5, 1, .01), text("Gain"), trackercolour("lime"), outlinecolour(0, 0, 0, 50), textcolour("black")
rslider bounds(192, 162, 100, 100), channel("feedback"), range(0, 0.5, 0, 1, 0.01), text("Feed"), trackercolour(0, 255, 0, 255), outlinecolour(0, 0, 0, 50), textcolour(0, 0, 0, 255)
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



instr WriteJSON

    iRes writeJSONToChannel 0, "dataChan", "{ \"happy\": true, \"pi\": 3.141}"

    if iRes == -1 then
        prints "Couldn't write JSON data"
    endif

endin

instr UpdateJSON

    iRes writeJSONToChannel 1, "dataChan", {{
        { 
        "happy": true, "pi": 1.141, "test": 12
        }
    }}

    if iRes == -1 then
        prints "Couldn't write JSON data"
    endif

endin

instr ShowJSON
    prints chnget:S("dataChan")
endin

instr GetJSONFloatValue
    kVal getJSONFloatValue "dataChan", "pi3"
    printk2 kVal
endin

</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
f0 z
;starts instrument 1 and runs it for a week
i"WriteJSON" 0 1
i"ShowJSON" 1 1
i"UpdateJSON" 2 1
i"ShowJSON" 2 1
i"GetJSONFloatValue" 3 1
</CsScore>
</CsoundSynthesizer>
