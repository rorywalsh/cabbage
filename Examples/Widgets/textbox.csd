<Cabbage>
form caption("Textbox Example") size(400, 300), colour(220, 220, 220), pluginID("def1")
label bounds(8, 6, 368, 20), text("Basic Usage"), fontColour("black")
textBox bounds(10, 30, 380, 267) identChannel("widgetIdent"), file("Textbox.csd")
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 -m0d 
</CsOptions>
<CsInstruments>
; Initialize the global variables. 
sr = 44100
ksmps = 32
nchnls = 2
0dbfs = 1

;basic usage
instr 1
 
endin

</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
f0 z
;starts instrument 1 and runs it for a week
i1 0 z
i2 0 z
</CsScore>
</CsoundSynthesizer>
