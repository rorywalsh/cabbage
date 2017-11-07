<Cabbage>
form caption("Grid Sequencer") size(544, 480), colour(120, 120, 120),pluginID("add1"), guirefresh(128), import("gridSequencer.xml")
gridSequencer bounds(7, 7, 535, 337), channel("grid"), namespace("rw")
button bounds(10, 430, 148, 40) channel("random") text("Randomise", "Randomise") 
button bounds(10, 390, 148, 40) channel("clear") text("Clear", "Clear") 
button bounds(10, 350, 148, 40) channel("play") text("Play", "Stop") 
hslider bounds(162, 354, 361, 36) channel("tempo") range(0.1, 10, 8, 1, 0.5) text("Tempo") increment(0.5) 
button bounds(162, 394, 148, 40) channel("bass"), text("Change bass") 
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-d -n -+rtmidi=null -m0d
</CsOptions>
<CsInstruments>
sr = 44100
ksmps = 64
nchnls = 2
0dbfs=1
;RW 2018
;----------------------------------------
;main instrument: always on and getting values
;from checkboxes
;----------------------------------------
instr 1
kRow = 1
kBeat init 1
kScale[] fillarray 60, 62, 64, 65, 67, 69, 71, 72
if chnget:k("play") == 1 then
    if metro:k(chnget:k("tempo")) == 1 then
        until kRow>8 do
            kValue rw_GetGridValue "grid", kBeat, kRow
            if kValue == 1 then
                event "i", "SYNTH", 0, 1, kRow, kBeat
                ;event "i", "SYNTH_2", 0, 5, kScale[8-kRow]+12
            endif
            kRow = kRow+1
        od
        kBeat = kBeat<16 ? kBeat+1 : 1
        event "i", "UPDATE_SCRUBBER", 0, 0, kBeat
    endif
endif

if changed(chnget:k("random")) == 1 then
    event "i", "RANDOMISE", 0, 0
endif

if changed(chnget:k("clear")) == 1 then
    event "i", "CLEAR", 0, 0
endif


endin

instr 2
    kScale[] fillarray 60, 67, 65, 67, 65, 60, 64, 62
    kFreq init 0
    if changed(chnget:k("random")) == 1 then
        kFreq = kFreq<8 ? kFreq+1 : 0
    endif

    aEnv = .05
    aOscil oscil aEnv, cpsmidinn(kScale[kFreq]), 2
    kGain jspline .25, .1, 2
    outs aOscil*(abs(kGain)+.5), aOscil*(abs(kGain)+.5)
endin
;----------------------------------------
;simple synth that will generate tones if checkboxes
;are enabled. This instrument is being triggered from instr 1
;----------------------------------------
instr SYNTH
iScale[] fillarray 60, 62, 64, 65, 67, 69, 71, 72
iPattern[] fillarray 60, 0, 60, 0, 72, 0, 65, 0, 60, 0, 65, 48, 72, 0, 65, 0 
iArp[] fillarray 1, 2, 3, 4
    if p4 == 8 then
        p3 = .1
        aEnv expon .2, p3, .001
        aFreq expon 100, p3, 50
        aOscil oscil aEnv, aFreq, 2
        outs aOscil, aOscil
    elseif p4 == 7 then
        p3 = .1
        aEnv expon .2, p3, .001
        aNoise rand .1
        aOscil oscil aEnv, 200, 2
        outs (aOscil+aNoise)*aEnv, (aOscil+aNoise)*aEnv
    elseif p4 == 6 then
        p3 = .1
        aEnv expon .2, p3, .001
        aNoise rand .1
        aOscil oscil aEnv, 200, 2
        outs (aOscil+aNoise)*aEnv, (aOscil+aNoise)*aEnv
    elseif p4 == 5 then
        p3 = .4
        aEnv expon .1, p3, .001
        aOscil oscili aEnv, cpsmidinn(iScale[p5])*.5, 2
        outs aOscil, aOscil
    elseif p4 == 4 then
        p3 = .1
        aEnv linen .3, .1, p3, .1
        aOscil oscili aEnv, cpsmidinn(iPattern[p5]), 2
        outs aOscil, aOscil
    elseif p4 == 3 then
        p3 = .1
        aEnv expon .2, p3, .001
        aFreq expon 400, p3, 350
        aOscil oscil aEnv, aFreq, 1
        outs aOscil, aOscil
    elseif p4 == 2 then
        p3 = .1
        aEnv expon .1, p3, .001
        aFreq expon 500, p3, 600
        aOscil oscil aEnv, aFreq, 2
        outs aOscil, aOscil
    elseif p4 == 1 then
        p3 = .1
        aEnv expon .2, p3, .001
        aOscil oscil aEnv, cpsmidinn(iScale[p5]), 2
        outs aOscil, aOscil 
    endif  
endin

instr SYNTH_2
    iRow = p4
    kEnv expon 1/10, p3, 0.0001
    aSin oscili kEnv, cpsmidinn(iRow)
    outs aSin, aSin
endin 

;---------------------------------------
;simple instrument that will randomise the values
;of the grid, the rw_RandomiseGrid UDO is i-rate only
;---------------------------------------
instr RANDOMISE
    rw_RandomiseGrid "grid"
endin

;---------------------------------------
;simple instrument that will clear the values
;of the grid, the rw_ClearGrid UDO is i-rate only
;---------------------------------------
instr CLEAR
    rw_ClearGrid "grid"
endin

;---------------------------------------
;another simple instrument that will update 
;the scrubber positions in the grid, 
;the rw_SetGridScrubberPosition UDO is i-rate only
;---------------------------------------
instr UPDATE_SCRUBBER
    rw_SetGridScrubberPosition "grid", p4
endin
</CsInstruments>
<CsScore>
f99 0 4 -2 1 2 3 4
f1 0 4 10 1
f2 0 4 10 1 1
;main instrument
i1 0 z
i2 0 z
</CsScore>
</CsoundSynthesizer>
