<Cabbage>
form caption("Info Button Example") size(380, 300), guiMode("queue"), , colour(2, 145, 209) pluginId("def1")

infobutton bounds(16, 12, 340, 36) file("https://cabbageaudio.com/docs/introduction/") text("Load Cababge Documentation"), corners(5)

texteditor bounds(16, 68, 341, 72) channel("infoText"), readOnly(1), wrap(1), scrollbars(1)
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 -m0d 
</CsOptions>e
<CsInstruments>
; Initialize the global variables. 
ksmps = 32
nchnls = 2
0dbfs = 1

; Rory Walsh 2021 
;
; License: CC0 1.0 Universal
; You can copy, modify, and distribute this file, 
; even for commercial purposes, all without asking permission. 

instr 1
    SText  = "An infobutton can be used to load a html file, or a hyperlink. It doesn't receive any information from Csound, but you can still update its appearance and position by sending identifier data to its channel"
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
