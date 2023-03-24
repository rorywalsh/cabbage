
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; pvsAdsyn.csd
; Written by Iain McCurdy, 2012.

; Encapsulation of the pvsadsyn opcode.
; This opcode takes a streaming phase vocoding analysis and reconstitutes it as an audio signal with user-definable parameters
;  for the number of bins to include, the bin from which to begin resynthesis (bin offset) and the option of skipping bins and not
;  resynthesising them one after another (Bin Incr.)

<Cabbage>
form caption("pvsAdsyn") size(580, 100), pluginId("adsy"), guiMode("queue")
image pos(0, 0), size(580,100), colour(200,100,50,150), shape("rounded"), outlineColour("white"), outlineThickness(5) 
rslider  bounds( 10, 15, 70, 70),  text("Freq.Mod."),  channel("fmod"),      range(0.25, 4, 1),        textColour("white"), colour(200,100,50,250), trackerColour(tan)
rslider  bounds( 80, 15, 70, 70),  text("Num.Osc.s"),  channel("noscs"),     range(1, 1024, 256,1,1),  textColour("white"), colour(200,100,50,250), trackerColour(tan)
rslider  bounds(150, 15, 70, 70), text("Bin Offset"), channel("binoffset"), range(0, 256, 1,1,1),     textColour("white"), colour(200,100,50,250), trackerColour(tan)
rslider  bounds(220, 15, 70, 70), text("Bin Incr."),  channel("binincr"),   range(1, 32, 1,1,1),      textColour("white"), colour(200,100,50,250), trackerColour(tan)
label    bounds(295, 25, 60, 13), text("FFT Size"), fontColour("white")
combobox bounds(295, 40, 60, 18), text("128","256","512","1024","2048","4096","8192"), channel("FFT"), value(4), fontColour(255,255,200)
rslider  bounds(360, 15, 70, 70), text("Feedback"),   channel("feedback"),  range(0, 0.99, 0),        textColour("white"), colour(200,100,50,250), trackerColour(tan)
rslider  bounds(430, 15, 70, 70), text("Mix"),        channel("mix"),       range(0, 1.00, 1),        textColour("white"), colour(200,100,50,250), trackerColour(tan)
rslider  bounds(500, 15, 70, 70), text("Level"),      channel("lev"),       range(0, 5.00, 0.5, 0.5), textColour("white"), colour(200,100,50,250), trackerColour(tan)
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-d -n
</CsOptions>

<CsInstruments>

; sr is set by host
ksmps        =    32
nchnls       =    2
0dbfs        =    1    ; MAXIMUM AMPLITUDE

;Author: Iain McCurdy (2012)
;http://iainmccurdy.org/csound.html

opcode    pvsadsyn_module,a,akkkkkki
    ain,kfmod,knoscs,kbinoffset,kbinincr,kfeedback,kmix,iFFT    xin
    aresyn       init     0
    f_anal       pvsanal  ain+(aresyn*kfeedback), iFFT, iFFT/4, iFFT, 1        ; ANALYSE AUDIO INPUT SIGNAL AND OUTPUT AN FSIG
    knossc       init     256
    ktrig        changed  knoscs,kbinoffset,kbinincr
    if ktrig==1 then
     reinit    UPDATE2
    endif
    UPDATE2:
    inoscs       init     i(knoscs)    
    ibinoffset   init     i(kbinoffset)
    ibinincr     init     i(kbinincr)
    inoscs       limit    inoscs, 1, (((iFFT*0.5)+1)-ibinoffset)/ibinincr
    aresyn       pvsadsyn f_anal, inoscs, kfmod , i(kbinoffset), i(kbinincr)    ; RESYNTHESIZE FROM THE fsig USING pvsadsyn
    rireturn
    amix         ntrpol   ain, aresyn, kmix                                     ; CREATE DRY/WET MIX
                 xout     amix
endop

instr    1
    ; audio input
    ainL,ainR    ins
    ;ainL poscil  0.1,330
    ;ainR = ainL
    ;ainL,ainR    diskin    "/Users/iainmccurdy/Documents/iainmccurdy.org/CsoundRealtimeExamples/SourceMaterials/SynthPad.wav",1,0,1    ;USE FOR TESTING
    kmix         cabbageGetValue   "mix"
    kfmod        cabbageGetValue   "fmod"
    knoscs       cabbageGetValue   "noscs"
    knoscs       init              256
    kbinoffset   cabbageGetValue   "binoffset"
    kbinoffset   init              1
    kbinincr     cabbageGetValue   "binincr"
    kbinincr     init              1
    klev         cabbageGetValue   "lev"
    kfeedback    cabbageGetValue   "feedback"
    
    /* SET FFT ATTRIBUTES */
    kFFT         cabbageGetValue    "FFT"
    ;;printk2 2^(katt_table+5)
    kFFT         init      4
    if changed:k(kFFT)==1 then
     reinit UPDATE
    endif
    UPDATE:
    aoutL        pvsadsyn_module    ainL,kfmod,knoscs,kbinoffset,kbinincr,kfeedback,kmix,2^(i(kFFT)+6)
    aoutR        pvsadsyn_module    ainR,kfmod,knoscs,kbinoffset,kbinincr,kfeedback,kmix,2^(i(kFFT)+6)
    rireturn
                 outs               aoutL*klev,aoutR*klev
endin

</CsInstruments>

<CsScore>
i 1 0 z
</CsScore>

</CsoundSynthesizer>