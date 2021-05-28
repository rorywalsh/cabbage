
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; pvsBlur.csd
; Written by Iain McCurdy, 2012.

<Cabbage>
form caption("pvsBlur"), size(235,125) colour( 70, 90,100), pluginId("blur")
image             bounds(0, 0,235,125), colour( 70, 90,100), shape("rounded"), outlineColour("white"), outlineThickness(5) 
label    bounds(15,20, 60,13), text("FFT Size"), fontColour("white")
combobox bounds(15,35, 60,20), text("128","256","512","1024","2048","4096","8192"), channel("att_table"), value(4), fontColour(220,220,255)
rslider bounds( 80, 10, 70, 70), text("Mix"),       channel("mix"),       range(0, 1.00, 1),                textColour("white"), colour( 30, 50, 60),trackerColour("white")
rslider bounds(150, 10, 70, 70), text("Level"),     channel("lev"),       range(0, 1.00, 0.5, 0.5),         textColour("white"), colour( 30, 50, 60),trackerColour("white")
hslider bounds( 10, 70,210, 40), channel("blurtime"),  range(0, 2.00, 0.0, 0.5, 0.0001),                    textColour("white"), colour( 10, 30, 40),trackerColour("white")
label   bounds( 85,103, 60, 11), text("Blur Time"), fontColour("white")
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

/* FFT attribute tables */
giFFTattributes1    ftgen    0, 0, 4, -2,  128,  64,  128, 1
giFFTattributes2    ftgen    0, 0, 4, -2,  256, 128,  256, 1
giFFTattributes3    ftgen    0, 0, 4, -2,  512, 128,  512, 1
giFFTattributes4    ftgen    0, 0, 4, -2, 1024, 256, 1024, 1
giFFTattributes5    ftgen    0, 0, 4, -2, 2048, 512, 2048, 1
giFFTattributes6    ftgen    0, 0, 4, -2, 4096,1024, 4096, 1
giFFTattributes7    ftgen    0, 0, 4, -2, 8192,2048, 8192, 1

opcode    pvsblur_module,a,akkkiiii
    ain,kblurtime,kmix,klev,iFFTsize,ioverlap,iwinsize,iwintype    xin
    f_anal      pvsanal    ain, iFFTsize, ioverlap, iwinsize, iwintype        ;ANALYSE AUDIO INPUT SIGNAL AND OUTPUT AN FSIG
    f_blur        pvsblur    f_anal, kblurtime, 2                    ;BLUR AMPLITUDE AND FREQUENCY VALUES OF AN F-SIGNAL
    aout        pvsynth f_blur                                      ;RESYNTHESIZE THE f-SIGNAL AS AN AUDIO SIGNAL
    amix        ntrpol        ain, aout, kmix                    ;CREATE DRY/WET MIX
            xout        amix*klev    
endop

instr    1
    kblurtime    chnget    "blurtime"
    kmix        chnget    "mix"
    klev        chnget    "lev"

    ainL,ainR    ins
    ;ainL,ainR    diskin    "808loop.wav",1,0,1    ;USE FOR TESTING

    /* SET FFT ATTRIBUTES */
    katt_table    chnget    "att_table"    ; FFT atribute table
    katt_table    init    5
    ktrig        changed    katt_table
    if ktrig==1 then
     reinit update
    endif
    update:
    iFFTsize    table    0, giFFTattributes1 + i(katt_table) - 1
    ioverlap    table    1, giFFTattributes1 + i(katt_table) - 1
    iwinsize    table    2, giFFTattributes1 + i(katt_table) - 1
    iwintype    table    3, giFFTattributes1 + i(katt_table) - 1
    /*-------------------*/
    
    kporttime    linseg    0,0.001,0.02
    kblurtime    portk    kblurtime,kporttime
    aoutL        pvsblur_module    ainL,kblurtime,kmix,klev,iFFTsize,ioverlap,iwinsize,iwintype
    aoutR        pvsblur_module    ainR,kblurtime,kmix,klev,iFFTsize,ioverlap,iwinsize,iwintype
                outs    aoutR,aoutR
endin

</CsInstruments>

<CsScore>
i 1 0 [60*60*24*7]
</CsScore>

</CsoundSynthesizer>