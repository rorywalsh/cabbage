<Cabbage>
form caption("Custom ADSR plant") size(320, 300), colour(18, 60, 82), import("adsr.plant"), pluginid("ADSR")
adsr bounds(10, 10, 300, 120), channel("adsr"), namespace("cab")
keyboard bounds(8, 158, 300, 95)
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 -m0d --midi-key-cps=4 --midi-velocity-amp=5
</CsOptions>
<CsInstruments>
; Initialize the global variables. 
;sr is set by the host
ksmps = 32
nchnls = 2
0dbfs = 1

;instrument will be triggered by keyboard widget
instr 1
aEnv cab_adsr "adsr"
aOut vco2 p5, p4
outs aOut*aEnv, aOut*aEnv
endin

</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
f0 z
</CsScore>
</CsoundSynthesizer>
