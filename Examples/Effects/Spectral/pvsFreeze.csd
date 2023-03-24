
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; pvsFreeze.csd
; Freeze either amplitude stream or frequency stream (or both) of a streaming FFT analysis
; Changing FFT size can modify the sound quality of the frozen sound

; 'Auto' can be used to automatically trigger a new freeze frame (both amplitude and frequency) if the amplitude of the sound crosses a threshold
; 'Delay' specifies the time delay at which the sound will actually be frozen after the threshold triggering. This can be employed to capture the sustain portion of a sound (and not the attack).
; 'Threshold' specifies the threshold level at which triggering will occur. In a noisy environment, raise 'Threshold'

<Cabbage>
form caption("pvsfreeze"), size(660,100) colour(  0,160,80), pluginId("frez"), guiMode("queue")

image            bounds(  0,  0,270, 80), colour(  0,160,80), shape("rounded"), outlineColour("white"), outlineThickness(4), plant("Freeze") {
checkbox bounds( 15, 10, 180, 25), channel("freezb"), text("Freeze Both"), fontColour:0("white"), fontColour:1("white"), shape("ellipse"), colour("red")
checkbox bounds( 20, 50, 180, 15), channel("lock"), text("Lock Phases"), fontColour:0("white"), fontColour:1("white"), shape("ellipse"), colour("LightBlue")
checkbox bounds(120, 10, 180, 25), channel("freeza"), text("Freeze Amplitudes"), fontColour:0("white"), fontColour:1("white"), shape("ellipse"), colour("yellow")
checkbox bounds(120, 45, 180, 25), channel("freezf"), text("Freeze Frequencies"), fontColour:0("white"), fontColour:1("white"), shape("ellipse"), colour("orange")
}

image bounds(270,  0,190, 80), colour(  0,160,80), shape("rounded"), outlineColour("white"), outlineThickness(4), plant("Auto") {
button  bounds( 15, 25, 40, 20), channel("auto"), text("Auto","Auto"), colour:0(  0,  0,  0), fontColour:0(100,100,100), colour:1(255,100,100), fontColour:1(255,200,200)
rslider bounds( 60, 10, 60, 60), text("Delay"),     channel("delay"),  textColour("white"), range(0.0001, 0.5, 0.1, 1,0.001), colour(  0,100, 50,255), trackerColour("white")
rslider bounds(120, 10, 60, 60), text("Threshold"), channel("thresh"), textColour("white"), range(0.001, 0.5, 0.1,1,0.001),   colour(  0,100, 50,255), trackerColour("white")
}

image   bounds(460,  0,200, 80), colour(  0,160,80), shape("rounded"), outlineColour("white"), outlineThickness(4), plant("Mix") {
label    bounds(10,15, 60,13), text("FFT Size"), fontColour("white")
combobox bounds(10,30, 60,18), text("128","256","512","1024","2048","4096","8192"), channel("FFT"), value(4)
rslider bounds( 70, 10, 60, 60), text("Mix"),      channel("mix"),       range(0, 1.00, 1),        textColour("white"), colour(  0,100, 50,255), trackerColour("white")
rslider bounds(130, 10, 60, 60), text("Level"),    channel("lev"),       range(0, 1.00, 0.5, 0.5), textColour("white"), colour(  0,100, 50,255), trackerColour("white")
}

label   bounds( 2,85,100,10), text("Iain McCurdy . 2017"), fontColour("white")
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-d -n
</CsOptions>
<CsInstruments>

; sr set by host
ksmps        =    64
nchnls       =    2
0dbfs        =    1    ;MAXIMUM AMPLITUDE

;Author: Iain McCurdy (2013)
;http://iainmccurdy.org/csound.html

opcode    pvsfreeze_module,a,akkkkik
    ain,kfreeza,kfreezf,kmix,klev,iFFT,klock    xin

    f_anal      pvsanal   ain, iFFT, iFFT/4, iFFT, 1    ; ANALYSE AUDIO INPUT SIGNAL AND OUTPUT AN FSIG
    f_freeze    pvsfreeze f_anal, kfreeza, kfreezf
    f_lock      pvslock   f_freeze, klock
    aout        pvsynth   f_lock
    amix        ntrpol    ain, aout, kmix               ; CREATE DRY/WET MIX
                xout      amix*klev
endop

instr    1
    kmix        cabbageGetValue    "mix"            ; read in widgets
    klev        cabbageGetValue    "lev"
    kfreeza     cabbageGetValue    "freeza"
    kfreezf     cabbageGetValue    "freeza"
    kfreezb     cabbageGetValue    "freezb"
    klock       cabbageGetValue    "lock"
    
    ; triggering of 'Freeze All' mode
    kon         =    1
    koff        =    0
    ktrigon     trigger            kfreezb,0.5,0
    ktrigoff    trigger            kfreezb,0.5,1
    if(ktrigon==1) then
                cabbageSetValue    "freeza",kon
                cabbageSetValue    "freezf",kon
    elseif(ktrigoff==1) then
                cabbageSetValue    "freeza",koff
                cabbageSetValue    "freezf",koff
    endif

    ; audio input
    ainL,ainR    ins

    ; auto freeze triggering
    kauto       cabbageGetValue    "auto"              ; read in widgets
    kthresh     cabbageGetValue    "thresh"
    kdelay      cabbageGetValue    "delay"
    if kauto==1 then                                   ; if 'Auto' is on
     krms       rms                ainL+ainR           ; scan RMS of audio signal
     ktrig      trigger            krms,kthresh,0      ; if signal crosses threshold upwards                                                                                                                                                                                                                                                                                                                    
     ktrigdel   vdel_k             ktrig,kdelay,0.5    ; delayed version of the trigger
     if ktrig==1 then                                  ; if initial threshold crossing occurs...
                cabbageSetValue    "freeza",koff       ; turn freezing off
                cabbageSetValue    "freezf",koff
     endif
     if ktrigdel==1 then                               ; if delayed trigger is received...
                cabbageSetValue    "freeza",kon        ; turn freezing on
                cabbageSetValue    "freezf",kon
     endif
    endif

    kofftrig    trigger            kauto,0.5,1         ; when 'Auto' is turned off generate a trigger
    if kofftrig==1 then                                ; if 'Auto' is turned off...
                cabbageSetValue    "freeza",koff       ; turn freezing off
                cabbageSetValue    "freezf",koff
    endif    

    /* SET FFT ATTRIBUTES */
     kFFT       cabbageGetValue    "FFT"
    kFFT        init               4
    if changed:k(kFFT)==1 then
                reinit             update
    endif
    update:        
    
    aoutL        pvsfreeze_module  ainL,kfreeza,kfreezf,kmix,klev,2^(i(kFFT)+6), klock
    aoutR        pvsfreeze_module  ainR,kfreeza,kfreezf,kmix,klev,2^(i(kFFT)+6), klock
                 outs              aoutL,aoutR
endin

</CsInstruments>

<CsScore>
i 1 0 z
</CsScore>

</CsoundSynthesizer>