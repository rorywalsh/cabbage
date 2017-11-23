<Cabbage>
form caption("Test2") size(800, 300), pluginID("def1")
stringsequencer bounds(10, 10, 200, 200), channels("step", "track1", "track2", "track3"), bpm(120), showstepnumbers(2), numberofsteps(8)
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -m0d
</CsOptions>
<CsInstruments>
; Initialize the global variables. 
sr = 44100
ksmps = 32
nchnls = 2
0dbfs = 1


instr 1 
    if changed:k(chnget:k("step")) == 1 then
        event "i", "Track1", 0, 1
    endif
endin
 
instr Track1
    SCellData chnget "track1"
    if strlen(SCellData)>0 then
        iFreq = strtod:i(chnget:S("track1"))
        k1 expon 1, p3, 0.001
        a2 oscili k1, cpsmidinn(iFreq)
        outs a2, a2
    endif
endin

</CsInstruments>
<CsScore>
f1 0 1024 -7 0.01 1024 1
;starts instrument 1 and runs it for a week
i1 0 z
</CsScore>
</CsoundSynthesizer>
