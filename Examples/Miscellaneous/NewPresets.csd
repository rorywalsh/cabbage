<Cabbage>
form caption("Presets") size(370, 280), guiMode("queue") colour(58, 110, 182), pluginId("MPre")
keyboard bounds(14, 122, 345, 95)
rslider bounds(16, 40, 85, 79), channel("att"), range(0, 1, 0.01), text("Att.")
rslider bounds(102, 40, 85, 79), channel("dec"), range(0, 1, 0.4), text("Dec.")
rslider bounds(188, 40, 85, 79), channel("sus"), range(0, 1, 0.7), text("Sus.")
combobox bounds(100, 6, 86, 27), populate("*", "#USER_MUSIC_DIRECTORY")
presetbutton bounds(16, 6, 86, 27), corners(5) channel("test"), userFolder("#USER_HOME_DIRECTORY/sourcecode", "*.psts")
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