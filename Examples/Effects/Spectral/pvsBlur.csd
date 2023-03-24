
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; pvsBlur.csd
; Written by Iain McCurdy, 2012.

<Cabbage>
form caption("pvsBlur"), size(305,125) colour( 70, 90,100), pluginId("blur"), guiMode("queue")
image    bounds(0, 0,305,125), colour( 70, 90,100), shape("rounded"), outlineColour("white"), outlineThickness(5) 
label    bounds(15,20, 60,13), text("FFT Size"), fontColour("white")
combobox bounds(15,35, 60,20), text("128","256","512","1024","2048","4096","8192"), channel("FFT"), value(4), fontColour(220,220,255)
rslider  bounds( 80, 10, 70, 70), text("Input Gain"), channel("InGain"),   range(0, 1.00, 1,0.5),            textColour("white"), colour( 30, 50, 60),trackerColour("white")
rslider  bounds(150, 10, 70, 70), text("Mix"),       channel("mix"),       range(0, 1.00, 1),                textColour("white"), colour( 30, 50, 60),trackerColour("white")
rslider  bounds(220, 10, 70, 70), text("Level"),     channel("lev"),       range(0, 1.00, 0.5, 0.5),         textColour("white"), colour( 30, 50, 60),trackerColour("white")
hslider  bounds( 10, 70,280, 40), channel("blurtime"),  range(0, 5.00, 0.0, 0.5, 0.0001),                    textColour("white"), colour( 10, 30, 40),trackerColour("white")
label    bounds( 10,103,280, 11), text("Blur Time"), fontColour("white"), align("centre")
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-d -n
</CsOptions>
<CsInstruments>

; sr is set by host
ksmps         =     64
nchnls         =     2
0dbfs        =    1    ;MAXIMUM AMPLITUDE

; Author: Iain McCurdy (2012)
; http://iainmccurdy.org/csound.html


opcode    pvsblur_module,a,akkki
    ain,kblurtime,kmix,klev,iFFT    xin
    print iFFT
    f_anal      pvsanal    ain, iFFT, iFFT/4, iFFT, 1        ; ANALYSE AUDIO INPUT SIGNAL AND OUTPUT AN FSIG
    f_blur      pvsblur    f_anal, kblurtime, 5.1            ; BLUR AMPLITUDE AND FREQUENCY VALUES OF AN F-SIGNAL
    aout        pvsynth    f_blur                            ; RESYNTHESIZE THE f-SIGNAL AS AN AUDIO SIGNAL
    amix        ntrpol     ain, aout, kmix                   ; CREATE DRY/WET MIX
                xout       amix*klev    
endop

instr    1
    kblurtime   cabbageGetValue    "blurtime"
    kInGain     cabbageGetValue    "InGain"
    kmix        cabbageGetValue    "mix"
    klev        cabbageGetValue    "lev"

    ainL,ainR   ins
    ;ainL,ainR    diskin    "808loop.wav",1,0,1    ;USE FOR TESTING
    ;ainL,ainR    diskin    "/Users/iainmccurdy/Documents/iainmccurdy.org/CsoundRealtimeExamples/SourceMaterials/SynthPad.wav",1,0,1    ;USE FOR TESTING
    ainL         *=        a(kInGain)
    ainR         *=        a(kInGain)
    kporttime    linseg    0,0.001,0.02
    kblurtime    portk     kblurtime,kporttime

    /* SET FFT ATTRIBUTES */
    kFFT        cabbageGetValue    "FFT"
    kFFT        init    4
    ktrig       changed    kFFT
    if ktrig==1 then
     reinit update
    endif
    update:    
    aoutL        pvsblur_module    ainL,kblurtime,kmix,klev,2^(i(kFFT)+6)
    aoutR        pvsblur_module    ainR,kblurtime,kmix,klev,2^(i(kFFT)+6)
    rireturn
                outs    aoutL,aoutR
endin

</CsInstruments>

<CsScore>
i 1 0 [60*60*24*7]
</CsScore>

</CsoundSynthesizer>