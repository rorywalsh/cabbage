
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; pvsWarp.csd
; Written by Iain McCurdy, 2012.

<Cabbage>
form caption("pvsWarp") size(535, 90), pluginId("warp"), guiMode("queue")
image pos(0, 0), size(535, 90), colour( 80, 80,135,220), shape("rounded"), outlineColour("white"), outlineThickness(4) 
rslider bounds( 10, 11, 70, 70), text("Scale"),    channel("scal"),      range(0.1, 4, 1, 0.5, 0.001), colour("LightSlateGrey"), textColour("white"), trackerColour("white")
rslider bounds( 80, 11, 70, 70), text("Shift"),    channel("shift"),     range(-5000, 5000, 0),        colour("LightSlateGrey"), textColour("white"), trackerColour("white")
rslider bounds(150, 11, 70, 70), text("Feedback"), channel("FB"),        range(0, 0.99, 0),            colour("LightSlateGrey"), textColour("white"), trackerColour("white")
label    bounds(220,20, 60,13), text("FFT Size"), fontColour("white")
combobox bounds(220,35, 60,20), text("128","256","512","1024","2048","4096","8192"), channel("FFT"), value(4), fontColour("lightblue")
checkbox bounds(290,30, 95,15), channel("DelayComp"), text("Delay Comp."), fontColour:0("white"), fontColour:1("white"), colour("lightblue")
rslider bounds(385, 11, 70, 70), text("Mix"),      channel("mix"),       range(0, 1.00, 1),            colour("LightSlateGrey"), textColour("white"), trackerColour("white")
rslider bounds(455, 11, 70, 70), text("Level"),    channel("lev"),       range(0, 1.00, 0.5),          colour("LightSlateGrey"), textColour("white"), trackerColour("white")
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-d -n
</CsOptions>

<CsInstruments>

; sr is set by host
ksmps        =    32
nchnls       =    2
0dbfs        =    1    ;MAXIMUM AMPLITUDE

;Author: Iain McCurdy (2012)
;http://iainmccurdy.org/csound.html

opcode    pvswarp_module,a,akkkkii
    ain,kscal,kshift,kfeedback,kmix,iFFT,iDelayComp    xin
    print iFFT
    aout        init    0
    f_anal      pvsanal ain+(aout*kfeedback), iFFT, iFFT/4, iFFT, 1                  ; ANALYSE AUDIO INPUT SIGNAL AND OUTPUT AN FSIG
    f_warp      pvswarp f_anal, kscal, kshift                                        ; WARP SPECTRAL ENVELOPE VALUES OF AN F-SIGNAL USING BOTH SCALING AND SHIFTING
    aout        pvsynth f_warp                                                       ; RESYNTHESIZE THE f-SIGNAL AS AN AUDIO SIGNAL
    if(kfeedback>0) then
     aout       clip    aout,0,0dbfs
    endif
    if iDelayComp==1 then
     ain        delay   ain,iFFT/sr
    endif
    amix        ntrpol  ain, aout, kmix                                              ; CREATE DRY/WET MIX
                xout    amix    
endop

instr    1
    ainL,ainR    ins
    
    ;ainL,ainR    diskin    "SynthPad.wav",1,0,1    ;USE FOR TESTING
    ;ainL         vco2               0.2,330
    ;ainR         =                  ainL
    
    kscal        cabbageGetValue    "scal"
    kshift       cabbageGetValue    "shift"
    kfeedback    cabbageGetValue    "FB"
    kmix         cabbageGetValue    "mix"
    klev         cabbageGetValue    "lev"
    kDelayComp   cabbageGetValue    "DelayComp"
    ainL         =                  ainL*klev
    ainR         =                  ainR*klev

    /* SET FFT ATTRIBUTES */
    kFFT        cabbageGetValue     "FFT"
    kFFT        init                4
    if changed:k(kFFT)==1 then
                reinit              update
    endif
    update:
    aoutL        pvswarp_module     ainL,kscal,kshift,kfeedback,kmix,2^(i(kFFT)+6),i(kDelayComp)
    aoutR        pvswarp_module     ainR,kscal,kshift,kfeedback,kmix,2^(i(kFFT)+6),i(kDelayComp)
    rireturn
                 outs               aoutL,aoutR
endin

</CsInstruments>

<CsScore>
i 1 0 z
</CsScore>

</CsoundSynthesizer>
