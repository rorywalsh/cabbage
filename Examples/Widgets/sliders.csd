<Cabbage>
form caption("Slider Example") size(360, 460), guiMode("queue"), colour(2, 145, 209) pluginId("def1")

texteditor bounds(16, 254, 332, 191) channel("infoText"), readOnly(1), wrap(1), scrollbars(1)

vslider bounds(20, 20, 40, 180) channel("harmonic1") markerColour(0, 0, 0, 255), trackerColour(2, 145, 209), range(0, 1, 0, 1, 0.001), imgFile("slider", "Fader.png")
vslider bounds(60, 20, 40, 180) channel("harmonic2") range(0, 1, 0, 1, 0.001), imgFile("slider", "Fader.png")
vslider bounds(100, 20, 40, 180) channel("harmonic3") range(0, 1, 0, 1, 0.001), imgFile("slider", "Fader.png")
vslider bounds(140, 20, 40, 180) channel("harmonic4") range(0, 1, 0, 1, 0.001), imgFile("slider", "Fader.png")
vslider bounds(180, 20, 40, 180) channel("harmonic5") range(0, 1, 0, 1, 0.001), imgFile("slider", "Fader.png")
vslider bounds(220, 20, 40, 180) channel("harmonic6") range(0, 1, 0, 1, 0.001), imgFile("slider", "Fader.png")
vslider bounds(260, 20, 40, 180) channel("harmonic7") range(0, 1, 0, 1, 0.001), imgFile("slider", "Fader.png")
vslider bounds(300, 20, 40, 180) channel("harmonic8") range(0, 1, 0, 1, 0.001), imgFile("slider", "Fader.png")
checkbox bounds(24, 208, 100, 30) channel("randomise"), colour:1(147, 210, 0), text("Randomise"), fontColour:1("white")
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d
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

giWave ftgen 1, 0, 4096, 10, 1, .2, .1, .2, .1

instr 1

    SText  = "Slider widgets in Cabbage come in a variety of styles. Almost all the widget examples use sliders in some way or another. This simple instrument uses vslider widgets. The fader thumb uses an image loaded from disk. When the 'Randomise' button is pushed, each slider has its position updated according to a simple spline curve.\n\nCabbage sliders can load images for their various parts, background, thumb, etc., or they can use film strips / sprite-sheet type PNGs that contain frames of each state."
    cabbageSet "infoText", "text", SText
    
    a1 oscili tonek(cabbageGetValue:k("harmonic1"), 10), 50, giWave
    a2 oscili tonek(cabbageGetValue:k("harmonic2"), 10), 100, giWave
    a3 oscili tonek(cabbageGetValue:k("harmonic3"), 10), 150, giWave
    a4 oscili tonek(cabbageGetValue:k("harmonic4"), 10), 200, giWave
    a5 oscili tonek(cabbageGetValue:k("harmonic5"), 10), 250, giWave
    a6 oscili tonek(cabbageGetValue:k("harmonic6"), 10), 300, giWave
    a7 oscili tonek(cabbageGetValue:k("harmonic7"), 10), 350, giWave
    a8 oscili tonek(cabbageGetValue:k("harmonic8"), 10), 400, giWave
    
    kRandom cabbageGet "randomise"
    
    if kRandom == 1 then
        cabbageSetValue "harmonic1", abs(jspline:k(.9, .1, .3))
        cabbageSetValue "harmonic2", abs(jspline:k(.9, .1, .3))
        cabbageSetValue "harmonic3", abs(jspline:k(.9, .1, .3))
        cabbageSetValue "harmonic4", abs(jspline:k(.9, .1, .3))
        cabbageSetValue "harmonic5", abs(jspline:k(.9, .1, .3))
        cabbageSetValue "harmonic6", abs(jspline:k(.9, .1, .3))
        cabbageSetValue "harmonic7", abs(jspline:k(.9, .1, .3))
        cabbageSetValue "harmonic8", abs(jspline:k(.9, .1, .3))
    endif
    
    aMix = a1+a2+a3+a4+a5+a6+a7+a8
    out aMix*.1, aMix*.1
endin       

</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
f0 z
;starts instrument 1 and runs it for a week
i1 0 z
</CsScore>
</CsoundSynthesizer>
