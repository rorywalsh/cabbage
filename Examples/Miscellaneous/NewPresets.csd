<Cabbage>
form caption("Presets") size(370, 280), guiMode("queue") pluginId("MPre")
keyboard bounds(14, 122, 345, 95)
rslider bounds(16, 40, 85, 79), channel("att"), range(0, 1, 0.01), text("Att.")
rslider bounds(102, 40, 85, 79), channel("dec"), range(0, 1, 0.4), text("Dec.")
rslider bounds(188, 40, 85, 79), channel("sus"), range(0, 1, 0.7), text("Sus.")
rslider bounds(274, 40, 85, 79), channel("rel"), range(0, 1, 0.7, 1, 0.001), text("Rel.")
presetbutton bounds(16, 6, 86, 27), corners(5) channel("test"), userFolder("Presets", "*.psts"), textColour(250), highlightedItemColour(10, 147, 210) highlightedTextColour(255, 255, 255)
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