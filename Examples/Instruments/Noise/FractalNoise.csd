
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; FractalNoise.csd
; Written by Iain McCurdy, 2015

; GUI colour indicates noise type: 'white' - 'pink' - 'brown'

<Cabbage>
form caption("Fractal Noise"), size(290,275), pluginId("fnse"), guiRefresh(32)
image bounds(  0,  0,290,275), colour("white"), shape("sharp"), , identChannel("BackgroundColour")
checkbox bounds( 20, 10, 15, 15), channel("onoff"), value(0), fontColour("black"), colour("yellow")
label    bounds( 37, 11, 50, 13), text("On/Off"), fontColour(30,30,30), identChannel("label1")
rslider  bounds(100, 10, 50, 50), channel("amp"),     range(0, 2, 0.2, 0.5, 0.001), textColour("black"), trackerColour("yellow"), colour(30,30,30)
label    bounds(100, 60, 50, 13), text("Amp."), fontColour(30,30,30), identChannel("label2")
rslider  bounds(160, 10, 50, 50), channel("beta"),    range(-2, 5, 0, 1, 0.001),    textColour("black"), trackerColour("yellow"), colour(30,30,30)
label    bounds(160, 60, 50, 13), text("Beta"), fontColour(30,30,30), identChannel("label3")
rslider  bounds(220, 10, 50, 50), channel("width"),   range(0,0.05, 0, 0.5, 0.0001),    textColour("black"), trackerColour("yellow"), colour(30,30,30)
label    bounds(220, 60, 50, 13), text("Width"), fontColour(30,30,30), identChannel("label4")
gentable bounds(  5, 85 ,280, 90), tableNumber(10), ampRange(-1,1,-1), identChannel("table"),zoom(-1)
label    bounds(  7, 85, 100, 11), text("Amp.Waveform"), fontColour(255,255,255,150), align(left)
gentable bounds(  5,180 ,280, 90), tableNumber(11), identChannel("FFT"), ampRange(0,1,-1), outlineThickness(0), sampleRange(0, 128), tableColour("yellow"), zoom(-1)
label    bounds(  7,180, 100, 11), text("FFT Spectrum"), fontColour(255,255,255,150), align(left)
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-dm0 -n -+rtmidi=NULL -M0
</CsOptions>

<CsInstruments>

; sr set by host
ksmps         =     16
nchnls         =     2
0dbfs        =    1

ginoise        ftgen    10,0,128,10,1
giFFT        ftgen    11,0,256,10,1

giFracScal    ftgen    1,0,-700,-27, 0,1.1, 100,1.1, 200,1.1, 300,0.42, 400,0.08, 500,0.008, 600,0.001, 700,0.0002 
;                          white    white     pink     brown     black
giR        ftgen    2,0,-700,-27,  0,255, 200,255,  300,255, 400,80,   700,1 
giG        ftgen    3,0,-700,-27,  0,255, 200,255,  300,130, 400,40,   700,1 
giB        ftgen    4,0,-700,-27,  0,255, 200,255,  300,130, 400,0 ,   700,1 



instr    1
    konoff    chnget    "onoff"        ;read in on/off switch widget value
    kamp        chnget    "amp"
    kamp        port    kamp,0.01
    kbeta        chnget    "beta"
    kbeta        port    kbeta,0.1
    kbeta        init    0
    kwidth        chnget    "width"
    kwidth        portk    kwidth,0.05
    kscal        table    (kbeta+2)/7,giFracScal,1
    aL    fractalnoise    kscal*0.5*konoff, kbeta
    if kwidth>0.0001 then
     aR    vdelay        aL,kwidth*1000,100
    else
     aR    =    aL
    endif
        outs    aL*kamp, aR*kamp

    kptr    init    0
        tabw k(aL), kptr, 10
    kptr    =    (kptr + 1) % 128

    if metro(16)==1 then
     if changed(kbeta)==1 then
      kR    tab    (kbeta+2)/7,giR,1
      kG    tab    (kbeta+2)/7,giG,1
      kB    tab    (kbeta+2)/7,giB,1
      kR    init    255
      kG    init    255
      kB    init    255
      Smsg    sprintfk    "colour(%d,%d,%d)",kR,kG,kB
          chnset        Smsg,"BackgroundColour"
     endif

        chnset    "tableNumbers(10)","table"

    endif

    
    kFlickOn    trigger    kbeta,2,0
    kFlickOff    trigger    kbeta,2,1
    if kFlickOn==1 then
          chnset        "fontColour(255,255,255)","label1"
          chnset        "fontColour(255,255,255)","label2"
          chnset        "fontColour(255,255,255)","label3"
          chnset        "fontColour(255,255,255)","label4"
    elseif kFlickOff==1 then
          chnset        "fontColour(0,0,0)","label1"
          chnset        "fontColour(0,0,0)","label2"
          chnset        "fontColour(0,0,0)","label3"
          chnset        "fontColour(0,0,0)","label4"
    endif
    
    fsig    pvsanal    aL*3, 256,64,256,1
      kflag    pvsftw fsig, 11
     if kflag==1 then
      chnset    "tableNumber(11)", "FFT"
     endif

endin


</CsInstruments>

<CsScore>
i 1 0 [60*60*24*7]    ;instrument that reads in widget data
</CsScore>

</CsoundSynthesizer>