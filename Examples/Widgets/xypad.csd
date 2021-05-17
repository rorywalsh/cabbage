<Cabbage>
form caption("XYPad Example") size(400, 650), guiMode("queue"), colour(2, 145, 209) pluginId("def1")
texteditor bounds(22, 337, 361, 294) channel("infoText"), readOnly(1), wrap(1), scrollbars(1)
xypad bounds(26, 20, 260, 250) channel("x", "y") rangeX(0, 400, 105) rangeY(0, 400, 200)
rslider bounds(300, 190, 73, 71) channel("gain") textColour(255, 255, 255, 255), text("Gain") range(0, 2, 1.158, 1, 0.001)
rslider bounds(300, 110, 73, 71) channel("speed") textColour(255, 255, 255, 255), text("Sample Speed") range(-2, 2, -0.384, 1, 0.001)

rslider bounds(300, 32, 73, 71) channel("wetDry") textColour(255, 255, 255, 255), text("Wet/Dry") range(0, 1, 0, 1, 0.001)

checkbox bounds(28, 274, 104, 26) channel("random"), colour:1(147, 210, 0), fontColour:1(255, 255, 255) text("Randomise")
nslider bounds(136, 274, 61, 26) visible(0), channel("updateSpeed") range(0, 20, 2, 1, 0.01) velocity(50)
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
; You can copy, modify, and distribute this work, 
; even for commercial purposes, all without asking permission. 

instr 1

    SText  = "An xypad provides quick access two parameters at once. It accepts two channels, one for the X values, and one for the Y values. The ball can be moved around by clicking on it, while a right-click drag will cause it to bounce around the screen.\n\nAlthough two channels are passed to this widget, identifier data should only be sent to the first channel, i.e, the X channel.\n\nIn this example a soundfile is being ring modulated by a modulator signal, taht is itself a result of ring modulation suing two sine waves. The signal is then passed to some comb filters, and finally to a simple stereo reverb unit. The ntrpol opcodes is used to balance the wet and dry signals.\n\nA randomise button is added to add some modulation to various parameters. Note that because the xypad has two channels, we cannot use cabbageSetValue to set its value. We must use cabbageSet instead."
    cabbageSet "infoText", "text", SText
    kX = cabbageGetValue:k("x")
    kY = cabbageGetValue:k("y")
    kSpeed = cabbageGetValue:k("speed")
    
    kRand, kTrig cabbageGetValue "random"
    cabbageSet kTrig, "updateSpeed", "visible", kRand
    kUpdateSpeed cabbageGetValue "updateSpeed"
    
    if kRand == 1 then
       cabbageSet metro(kUpdateSpeed), "x", "valueX", random:k(0, 400)
       cabbageSet metro(kUpdateSpeed), "x", "valueY", random:k(0, 400)
       cabbageSetValue "speed", random:k(-2, 2), metro(kUpdateSpeed)    
    endif

    aMod oscili 1, kX
    aCar oscili aMod, kY
    
    aDryL, aDryR diskin2 "808loop.wav", kSpeed, 0, 1 
    aDelL comb aDryL, 5, 0.25
    aDelR comb aDryR, 5, 0.75
    
    aPan jspline 1, .1, .5
    
    aWetL, aWetR reverbsc (aDelL*aCar)*aPan, (aDelR*aCar)*(1-aPan), .7, 1000

    aL ntrpol aWetL, aDryL, cabbageGetValue:k("wetDry")
    aR ntrpol aWetR, aDryR, cabbageGetValue:k("wetDry")
    
    outs aL*cabbageGetValue:k("gain"), aR*cabbageGetValue:k("gain")
    

endin

</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
f0 z
;starts instrument 1 and runs it for a week
i1 0 z
</CsScore>
</CsoundSynthesizer>
