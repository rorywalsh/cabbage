<Cabbage>
form caption("RadioThings") size(440, 180), colour(58, 110, 210), import("plant.plant"), pluginid("CuPI")
radioValueButtonGroup bounds(18, 18, 258, 110), channel("radioGroup"), namespace("rw")
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

instr 1
kValue rw_radioValueButtonGroup "radioGroup"
printk2 kValue
endin

</CsInstruments>
<CsScore>
f1 0 1024 10 1
;causes Csound to run for about 7000 years...
i1 0 z
</CsScore>
</CsoundSynthesizer>
