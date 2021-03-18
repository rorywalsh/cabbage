<Cabbage>
form caption("Presets") size(360, 200), colour(34, 34, 34), pluginid("MPre"), scale(1), identchannel("formIdent")
image bounds(0, 0, 366, 229), colour(0, 0, 0, 0)
keyboard bounds(10, 103, 350, 70)
rslider bounds(12, 9, 86, 90), channel("att"), range(0, 1, 0.01, 1, 0.001), text("Att.")
rslider bounds(99, 9, 86, 90), channel("dec"), range(0, 1, 0.4, 1, 0.001), text("Dec.")
rslider bounds(187, 9, 86, 90), channel("sus"), range(0, 1, 0.7, 1, 0.001), text("Sus.")
rslider bounds(274, 9, 86, 90), channel("rel"), range(0, 1, 0.8, 1, 0.001), text("Rel.")
combobox bounds(10, 180, 80, 20), mode("resize"), value(3)
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 --midi-key-cps=4 --midi-velocity-amp=5
</CsOptions>
<CsInstruments>
; Initialize the global variables. 
;sr is set by the host
ksmps = 32
nchnls = 2
0dbfs = 1

;instrument will be triggered by keyboard widget
instr 1

endin

</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
i1 0 z
</CsScore>
</CsoundSynthesizer>