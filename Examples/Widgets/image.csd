<Cabbage>
form caption("Image Example") size(380,440), guiMode("queue"), colour(2, 145, 209) pluginId("def1")
texteditor bounds(18, 280, 341, 150) channel("infoText"), readOnly(1), wrap(1), scrollbars(1)

image bounds(72, 22, 234, 234) channel("image1"), file("cabbage.png")
image bounds(42, 26, 280, 190) channel("image2"), file("headphones.png")
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 -m0d --midi-key=4 --midi-velocity-amp=5
</CsOptions>
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

instr 99

    SText  = "An image widget can be used to display images. PNGs, JPEGSs and basic SVGs are supported. As with all Cabbage widgets, an image widget's attributes can be updated in real time by sending updated identifiers data to its channel. Images can also as as buttons, where a mouse click will toggle between 0 and 1.\n\nIn this example the RMS value from a simple loop is used to drive the size of the Cabbage."  
    cabbageSet "infoText", "text", SText
    
endin

instr 1    
    a1, a2 diskin2 "beat.ogg", 1, 0, 1
    outs a1, a2
    
    kRms rms a1+a2, 20
    cabbageSet metro(10), "image1", "bounds", 72, 22, 234+(kRms*10), 234+(kRms*10)

endin
              


</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
f0 z
i99 0 0
i1 1 z
</CsScore>
</CsoundSynthesizer>
