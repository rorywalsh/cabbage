
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; pvShift.csd
; Written by Iain McCurdy, 2012.

; Streaming phase vocoding frequency shifter.

<Cabbage>
form caption("pvshift Frequency Shifter") size(510,  90), pluginId("shft"), guiMode("queue")

image bounds( 15,  9, 60, 60), colour(30,30,30,100), shape("ellipse"), outlineThickness(0)
image bounds( 85,  9, 60, 60), colour(30,30,30,100), shape("ellipse"), outlineThickness(0)
image bounds(155,  9, 60, 60), colour(30,30,30,100), shape("ellipse"), outlineThickness(0)
image bounds(225,  9, 60, 60), colour(30,30,30,100), shape("ellipse"), outlineThickness(0)
image bounds(295,  9, 60, 60), colour(30,30,30,100), shape("ellipse"), outlineThickness(0)
image bounds(365,  9, 60, 60), colour(30,30,30,100), shape("ellipse"), outlineThickness(0)
image bounds(435,  9, 60, 60), colour(30,30,30,100), shape("ellipse"), outlineThickness(0)
image bounds(-60,-5,630,110), colour(100,205,60), outlineColour("silver"), outlineThickness(6), shape("ellipse")
rslider bounds( 10, 15, 70, 70), text("Coarse"),     channel("coarse"),    range(-4000, 4000, 0, 1),    colour("orange"), trackerColour("yellow"), textColour("black")
rslider bounds( 80, 15, 70, 70), text("Multiplier"), channel("fine"),      range(-1, 1, 1, 1, 0.0001),  colour("orange"), trackerColour("yellow"), textColour("black")
rslider bounds(150, 15, 70, 70), text("Lowest"),     channel("lowest"),    range( 20, 20000, 20, 0.5),  colour("orange"), trackerColour("yellow"), textColour("black")
rslider bounds(220, 15, 70, 70), text("Feedback"),   channel("FB"),        range(0, 1.00, 0),           colour("orange"), trackerColour("yellow"), textColour("black")
label    bounds(295,20, 60,13), text("FFT Size"), fontColour("black")
combobox bounds(295,35, 60,20), text("128","256","512","1024","2048","4096","8192"), channel("FFT"), value(4), fontColour(220,220,255)
rslider bounds(360, 15, 70, 70), text("Mix"),        channel("mix"),       range(0, 1.00, 1),           colour("orange"), trackerColour("yellow"), textColour("black")
rslider bounds(430, 15, 70, 70), text("Level"),      channel("lev"),       range(0, 1.00, 0.5, 0.5),    colour("orange"), trackerColour("yellow"), textColour("black")
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-d -n
</CsOptions>
<CsInstruments>

; sr set by host
ksmps        =     32
nchnls       =     2
0dbfs        =     1    ;MAXIMUM AMPLITUDE

;Iain McCurdy
;http://iainmccurdy.org/csound.html
;Frequency shifting effect using pvshift opcode.

opcode    pvshift_module,a,akkkkki
    ain,kshift,klowest,kfeedback,kmix,klev,iFFT    xin
    f_FB        pvsinit iFFT,iFFT/4,iFFT,1, 0                           ; INITIALISE FEEDBACK FSIG
    f_anal      pvsanal ain, iFFT,iFFT/4,iFFT,1                         ; ANALYSE AUDIO INPUT SIGNAL AND OUTPUT AN FSIG
    f_mix       pvsmix  f_anal, f_FB                                    ; MIX AUDIO INPUT WITH FEEDBACK SIGNAL
    f_shift     pvshift f_mix, kshift, klowest                          ; SHIFT FREQUENCIES
    f_FB        pvsgain f_shift, kfeedback                              ; CREATE FEEDBACK F-SIGNAL FOR NEXT PASS
    aout        pvsynth f_shift                                         ; RESYNTHESIZE THE f-SIGNAL AS AN AUDIO SIGNAL
    amix        ntrpol  ain, aout, kmix                                 ; CREATE DRY/WET MIX
                xout    amix*klev    
endop

instr    1
    ainL,ainR    ins
    
    ; FOR TESTING
    ;gicos    ftgen    0,0,131072,11,1            ; test tone
    ;ainL    gbuzz    0.1,330,80,1,0.7,gicos    ; test tone
    ;ainR    =        ainL
    ;ainL,ainR    diskin2    "SynthPad.wav",1,0,1
    
    kcoarse    cabbageGetValue    "coarse"                    ; coarse freq. shift
    kfine      cabbageGetValue    "fine"                      ; fine freq. control (multipler)
    kporttime  linseg             0,0.001,0.2
    kshift     lineto             kcoarse*kfine, kporttime    ; ultimate freq. shift is sum of coarse and fine controls
    klowest    cabbageGetValue    "lowest"                    ; lowest shifted frequency
    kFB        cabbageGetValue    "FB"                        ; feedback amount
    kmix       cabbageGetValue    "mix"
    klev       cabbageGetValue    "lev"

    /* SET FFT ATTRIBUTES */
    kFFT       cabbageGetValue    "FFT"
    kFFT       init      4
    if changed:k(kFFT)==1 then
     reinit update
    endif
    update:    
    aoutL        pvshift_module    ainL,kshift,klowest,kFB,kmix,klev,2^(i(kFFT)+6)
    aoutR        pvshift_module    ainR,kshift,klowest,kFB,kmix,klev,2^(i(kFFT)+6)
    rireturn
                 outs              aoutL,aoutR
endin

</CsInstruments>

<CsScore>
i 1 0 [60*60*24*7]
</CsScore>

</CsoundSynthesizer>