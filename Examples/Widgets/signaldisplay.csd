<Cabbage>
form caption("Signaldisplay Example") size(400, 300), colour(220, 220, 220), pluginid("SDis")
label bounds(8, 6, 368, 20), text("Basic Usage"), fontcolour("black")
signaldisplay bounds(8, 30, 380, 170), colour("lime"), backgroundcolour("black"), displaytype("waveform"), signalvariable("aSig")
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 -m0d --displays
</CsOptions>
<CsInstruments>
; Initialize the global variables. 
sr = 44100
ksmps = 32
nchnls = 2
0dbfs = 1

giWave ftgen 1, 0, 4096, 10, 1, 1, 1, 1 
seed 0 
;basic usage
instr 1
    aSig randi 1, 200
    display	aSig, .5, 1   
endin      

</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
f0 z
;starts instrument 1 and runs it for a week
i1 0 z
;i2 0 z
</CsScore>
</CsoundSynthesizer>
