<Cabbage>
form caption("Presets") size(370, 280), guiMode("queue"), colour(58, 110, 182), pluginId("MPre")
keyboard bounds(10, 90, 345, 95)
rslider bounds(12, 8, 85, 79), channel("att"), range(0, 1, 0.01), text("Att."), _mood("happy")
rslider bounds(98, 8, 85, 79), channel("dec"), range(0, 1, 0.4), text("Dec."), _mood("sad")
rslider bounds(184, 8, 85, 79), channel("sus"), range(0, 1, 0.7), text("Sus."), _mood("indefferent")
rslider bounds(270, 8, 85, 79), channel("rel"), range(0, 1, 0.8), text("Rel."), _mood("lived")
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
    event_i "i", "Print", 0, 1/kr
endin

instr Print
    SNames[] cabbageGetWidgetChannels "automatable(1), _mood(\"happy\")"
    prints "\nChannels with automatable() set to 1\n"
    printarray SNames
endin
</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
i1 0 z
</CsScore>
</CsoundSynthesizer>