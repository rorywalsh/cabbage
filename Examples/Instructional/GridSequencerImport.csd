<Cabbage>
form caption("Grid Sequencer") size(534, 410), colour("black"),pluginID("add1"), import("gridSequencer.xml")
gridSequencer bounds(10, 10, 518, 342), channel("grid"), namespace("rw")
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-d -n -+rtmidi=null -m0d
</CsOptions>
<CsInstruments>
sr = 44100
ksmps = 32
nchnls = 2
0dbfs=1
;RW 2014

;----------------------------------------
;main instrument
;----------------------------------------
instr 1

if metro(1) == 1 then
    event "i", "Randomise", 0, 0
endif


endin 

instr Randomise
RandomiseGrid "grid"
endin

</CsInstruments>
<CsScore>
;main instrument
i1 0 z
i2 1 0
</CsScore>
</CsoundSynthesizer>