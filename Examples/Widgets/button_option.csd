<Cabbage>
form caption("Option Button") size(380, 500), guiMode("queue"), colour(2, 145, 209) pluginId("def1")

rslider bounds(12, 9, 86, 90), channel("att"), range(0, 1, 0.01, 1, 0.001), text("Att.")
rslider bounds(99, 9, 86, 90), channel("dec"), range(0, 1, 0.4, 1, 0.001), text("Dec.")
rslider bounds(187, 9, 86, 90), channel("sus"), range(0, 1, 0.7, 1, 0.001), text("Sus.")
rslider bounds(274, 9, 86, 90), channel("rel"), range(0, 1, 0.8, 1, 0.001), text("Rel.")

texteditor bounds(18, 256, 341, 208) channel("infoText"), readOnly(1), wrap(1), scrollbars(1)
keyboard bounds(12, 104, 348, 80) channel("keyboard")
optionbutton bounds(260, 188, 100, 30) channel("waveform"), colour:0(147, 210, 0), corners(5), items("Saw", "Square", "Triangle")
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 -m0d --midi-key=4 --midi-velocity-amp=5
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

instr 99

    SText  = "An option button can be used to toggle through different items. The widget will send its current index to is associated channel, starting from 0. In this example an option button is used to toggle through a set of waveform. \n\nWe can't just pass these indices to the vco2 opcode. Instead we use them as indices to simple array that hold the appropriate vco2 waveform modes."
    cabbageSet "infoText", "text", SText

endin

instr 1
    
    iVcoModes[] fillarray 0, 10, 12
    
    iAtt cabbageGetValue "att"
    iDec cabbageGetValue "dec"
    iSus cabbageGetValue "sus"
    iRel cabbageGetValue "rel"
    kEnv madsr iAtt, iDec, iSus, iRel
    aVco vco2 kEnv*p5*.5, cpsmidinn(p4), iVcoModes[cabbageGetValue:i("waveform")]
    outs aVco, aVco    
endin


</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
f0 z
i99 0 z
</CsScore>
</CsoundSynthesizer>
