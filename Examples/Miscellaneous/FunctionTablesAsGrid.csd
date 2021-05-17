<Cabbage>
form caption("Grid Sequencer") size(580, 370), pluginId("FTaG")

gentable bounds(10, 10, 560, 245), tableNumber(11:12:13:14:15:16:17:18), active(1), tableBackgroundColour(0, 0, 0, 0), tableColour(147, 210, 0) zoom(-1), ampRange(0, 1, -1, 1)
rslider bounds(14, 264, 60, 60) channel("bpmSlider") range(20, 500, 200, 1, 0.001) text("BPM")
image bounds(10, 250, 560, 5) corners(3), identChannel("progressBar") 
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-d -n -+rtmidi=null -m0d -Q0
</CsOptions>
<CsInstruments>
;sr is set by the host
ksmps = 32
nchnls = 2
0dbfs=1
;RW 2014

;----------------------------------------
;main instrument
;----------------------------------------
instr 1
k1 metro chnget:k("bpmSlider")/60
kCnt init 0
if k1 == 1 then
    kCnt = (kCnt<15 ? kCnt+1 : 0)
    if tab:k(kCnt, 11) == 1 then
        event "i", "SimpleSynth", 0, 2, 72
    endif
    if tab:k(kCnt, 12) == 1 then
        event "i", "SimpleSynth", 0, 2, 71
    endif
    if tab:k(kCnt, 13) == 1 then
        event "i", "SimpleSynth", 0, 2, 69
    endif
    if tab:k(kCnt, 14) == 1 then
        event "i", "SimpleSynth", 0, 2, 67
    endif
    if tab:k(kCnt, 15) == 1 then
        event "i", "SimpleSynth", 0, 2, 65
    endif
    if tab:k(kCnt, 16) == 1 then
        event "i", "SimpleSynth", 0, 2, 64
    endif
    if tab:k(kCnt, 17) == 1 then
        event "i", "SimpleSynth", 0, 2, 62
    endif
    if tab:k(kCnt, 18) == 1 then
        event "i", "SimpleSynth", 0, 2, 60
    endif
endif

;update progressbar
kProgress phasor 60/chnget:k("bpmSlider")
if metro(16) == 1 then
    SProgress sprintfk "size(%d, 5)", (560/16)*(kCnt+1)
    chnset SProgress, "progressBar"
endif

endin

instr SimpleSynth
a1 oscili expon:a(.2, p3, 0.001), cpsmidinn(p4)
outs a1, a1
endin

</CsInstruments>
<CsScore>
;8 tables of 16 samples
f11 0 16 -2 0
f12 0 16 -2 0
f13 0 16 -2 0
f14 0 16 -2 0  
f15 0 16 -2 0 
f16 0 16 -2 0 
f17 0 16 -2 0 
f18 0 16 -2 0 

;main instrument
i1 2 1000
</CsScore>
</CsoundSynthesizer>