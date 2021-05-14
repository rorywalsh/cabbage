<Cabbage>
form caption("Form Example") size(380, 300), guiMode("queue"), colour(2, 145, 209) pluginId("def1")
texteditor bounds(12, 20, 352, 228) channel("infoText"), readOnly(1), wrap(1), scrollbars(1)
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 -m0d --midi-key=4 --midi-velocity-amp=5
</CsOptions>e
<CsInstruments>
; Initialize the global variables. 
ksmps = 16
nchnls = 2
0dbfs = 1

; Rory Walsh 2021 
;
; License: CC0 1.0 Universal
; You can copy, modify, and distribute this file, 
; even for commercial purposes, all without asking permission. 

instr 1

    SText  = "A form represents an instrument plugin window. All widgets are positioned relative to the top left of the form, i.e., position 0,0.\n\nWhen declaring a form, you can also set some useful attributes of your plugin such as the pluginId and plugin delay compensation (latency)."
    cabbageSet "infoText", "text", SText

endin

</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
f0 z
;starts instrument 1 and runs it for a week
i1 0 z
</CsScore>
</CsoundSynthesizer>
