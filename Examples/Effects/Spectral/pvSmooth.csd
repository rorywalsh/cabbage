
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; pvSmooth.csd
; Written by Iain McCurdy, 2012.
; FFT feedback is disabled if amplitude smoothing is increased beyond zero. If this is not done the instrument will fail. 

; link    -    links 'Amp.Smooth' and 'Frq.Smooth'

<Cabbage>
form caption("pvSmooth") size(505, 90), pluginId("smoo"), colour("Black") guiMode("queue")
image bounds(0, 0, 505, 90), colour("Cream"), outlineColour("silver"), outlineThickness(5)
label pos(-52, -30), size(615, 150), fontColour(210,105, 30, 80), text("smooth"), shape("rounded"), outlineColour("white"), outlineThickness(4)
rslider bounds( 10, 8, 75, 75), text("Amp.Smooth"), channel("acf"),       range(0, 1.00, 0,16, 0.0001), textColour(138, 54, 15), colour("chocolate"), trackerColour(138, 54, 15)
checkbox bounds( 70, 8, 70,10), channel("link"), text("Link"), fontColour:0(138, 54, 15), fontColour:1(138, 54, 15), colour("yellow")
rslider bounds( 90, 8, 75, 75), text("Frq.Smooth"), channel("fcf"),       range(0, 1.00, 0,16, 0.0001), textColour(138, 54, 15), colour("chocolate"), trackerColour(138, 54, 15)
rslider bounds(170, 8, 75, 75), text("Feedback"),   channel("FB"),        range(0, 0.999, 0, 1,0.001),    textColour(138, 54, 15), colour("chocolate"), trackerColour(138, 54, 15)
label    bounds(250,20, 70,13), text("FFT Size"), fontColour(138, 54, 15)
combobox bounds(250,35, 70,20), text("128","256","512","1024","2048","4096","8192","16384"), channel("FFT"), value(4), fontColour(255,255,200)
checkbox bounds(250,60, 90,10), channel("delay"), text("Delay Comp."), fontColour:0(138, 54, 15), fontColour:1(138, 54, 15), colour("yellow")
rslider bounds(330, 8, 75, 75), text("Mix"),        channel("mix"),       range(0, 1.00, 1),              textColour(138, 54, 15), colour("chocolate"), trackerColour(138, 54, 15)
rslider bounds(410, 8, 75, 75), text("Level"),      channel("lev"),       range(0, 1.00, 0.5),            textColour(138, 54, 15), colour("chocolate"), trackerColour(138, 54, 15)
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-dm0 -n
</CsOptions>
<CsInstruments>

; sr set by host
ksmps        =    32
nchnls       =    2
0dbfs        =    1    ;MAXIMUM AMPLITUDE

;Iain McCurdy
;http://iainmccurdy.org/csound.html
;Spectral smoothing effect.

opcode    pvsmooth_module,a,akkkkkii
    ain,kacf,kfcf,kfeedback,kmix,klev,iFFT,idelay    xin
    f_FB        pvsinit  iFFT,iFFT/4,iFFT,1, 0                             ; INITIALISE FEEDBACK FSIG
    f_anal      pvsanal  ain, iFFT,iFFT/4,iFFT,1                           ; ANALYSE AUDIO INPUT SIGNAL AND OUTPUT AN FSIG
    f_mix       pvsmix   f_anal, f_FB                                      ; MIX AUDIO INPUT WITH FEEDBACK SIGNAL
    f_smooth    pvsmooth f_mix, kacf, kfcf                                 ; BLUR AMPLITUDE AND FREQUENCY VALUES OF AN F-SIGNAL
    f_FB        pvsgain  f_smooth, kfeedback                               ; CREATE FEEDBACK F-SIGNAL FOR NEXT PASS
    aout        pvsynth  f_smooth                                          ; RESYNTHESIZE THE f-SIGNAL AS AN AUDIO SIGNAL
    if idelay==1 then
     ain        delay    ain,(iFFT)/sr
    endif
    amix        ntrpol   ain, aout, kmix                                   ; CREATE DRY/WET MIX
                xout     amix*klev
endop

instr    1
    ainL,ainR    ins
    ;ainL,ainR    diskin    "808loop.wav",1,0,1    ;USE FOR TESTING
    ;ainL,ainR    diskin    "SynthPad.wav",1,0,1    ;USE FOR TESTING
    ;ainL         vco2      0.1,cpsoct(rspline:k(6,9,0.2,0.1))
    ;ainR         =         ainL
    
    kdelay      cabbageGetValue    "delay"
    kfeedback   cabbageGetValue    "FB"
    kacf        cabbageGetValue    "acf"
    kfcf        cabbageGetValue    "fcf"
    klink       cabbageGetValue    "link"
    if klink==1 then
     if changed(kacf)==1 then
                cabbageSetValue    "fcf",kacf
     elseif changed(kfcf)==1 then
                cabbageSetValue    "acf",kfcf
     endif
    endif
    kfeedback   =                  (kacf>0?0:kfeedback)        ; feedback + amplitude smoothing can cause failure so we must protect against this
    kacf        =                  1-kacf
    kfcf        =                  1-kfcf
    kporttime   linseg             0,0.001,0.02
    kmix        cabbageGetValue    "mix"
    klev        cabbageGetValue    "lev"
    kFFT        cabbageGetValue    "FFT"
    kFFT        init      4
    if changed:k(kFFT)==1 then
     reinit update
    endif
    update:
    aoutL       pvsmooth_module    ainL,kacf,kfcf,kfeedback,kmix,klev,2^(i(kFFT)+6),i(kdelay)
    aoutR       pvsmooth_module    ainR,kacf,kfcf,kfeedback,kmix,klev,2^(i(kFFT)+6),i(kdelay)
                outs               aoutL,aoutR
endin

</CsInstruments>

<CsScore>
i 1 0 [60*60*24*7]
</CsScore>

</CsoundSynthesizer>