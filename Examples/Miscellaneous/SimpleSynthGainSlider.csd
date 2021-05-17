<Cabbage>
form size(400, 190), caption("Simple Synth"), pluginId("SSGS")
hslider bounds(5, 110, 380, 50), channel("gain"), range(0, 1, .5), colour("white")
keyboard bounds(10, 5, 380, 100)
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 --midi-key-cps=4 --midi-velocity-amp=5
</CsOptions>
<CsInstruments>
;sr is set by the host
ksmps = 64			;audio samples per control cycle
nchnls = 2			;number of channels
0dbfs=1

instr 1
kGain chnget "gain"
aEnv linenr p5, 0.1, .25, 0.01		;envelope
a1 oscili aEnv, p4, 1			;oscillator, reading funtion table 1
outs a1*kGain, a1*kGain			;outputs, multiplied by gain
endin

</CsInstruments>  
<CsScore>
f1 0 4096 10 1		;function table one.  
f0 3600				;tell Csound to wait for events for 3600 seconds
</CsScore>
</CsoundSynthesizer>