
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; AutopanTremolo.csd
; 
; An autopan/tremolo effect in which a variety of LFO shapes can be employed.
; 
; LFO Shapes
; ----------
; sine
; triangle
; square
; Randomi    -    an interpolating random function
; Randomh    -    a 'sample and hold' type function
; Rspline    -    random spline
; User    -    an LFO shape (best understood when using tremolo function) 
;         in which the user can define the bias (percussive type : reverse percussive) 
;         and duty emphasis (emphasis of high state/loud against low state/quiet)

<Cabbage>
form caption("Autopan / Tremolo") size(565, 105), pluginId("aptr"), guiRefresh(32)
image pos( 0,  0),                size(565, 105),     colour("Maroon"), shape("rounded"), outlineColour("white"), outlineThickness(4) 
rslider  bounds(  5,  6, 90, 90), text("Freq.[Hz]"),  channel("rate"),  range(0.1, 50, 0.5, 0.5),   valueTextBox(1), textBox(1), trackerColour("tomato")
rslider  bounds( 80,  6, 90, 90), text("Tempo[BPM]"), channel("tempo"), range(6, 3000, 30, 0.5, 1), valueTextBox(1), textBox(1), trackerColour("tomato")
rslider  bounds(175,  6, 90, 90), text("Depth"),      channel("depth"), range(0, 1.00, 1, 0.5),     valueTextBox(1), textBox(1), trackerColour("tomato")

line     bounds(360, 10, 107, 2), colour("Grey")
label    bounds(393,  5, 43, 12), text("USER"), colour("Maroon")
rslider  bounds(345, 16, 80, 80), text("Bias"),      channel("bias"), range(-1, 1.00, 0),          valueTextBox(1), textBox(1), trackerColour("tomato")
rslider  bounds(405, 16, 80, 80), text("Duty"),      channel("duty"), range(0.01, 2000.00, 1, 0.25, 0.001),          valueTextBox(1), textBox(1), trackerColour("tomato")

rslider  bounds(470,  6, 90, 90), text("Level"),      channel("level"), range(0, 1.00, 1),          valueTextBox(1), textBox(1), trackerColour("tomato")

checkbox bounds(160, 40, 25, 25), colour("yellow"), channel("indicator"),  value(0), shape("ellipse"), active(0)
combobox bounds(260, 13,  90,20), channel("mode"), value(1), text("Autopan", "Tremolo")
combobox bounds(260, 38,  90,20), channel("wave"), value(1), text("Sine", "Triangle", "Square", "Randomi", "Randomh", "Rspline", "User")
checkbox bounds(260, 63, 90, 15), text("TEST TONE"), colour("lime"), channel("test"),  value(0)
</Cabbage>

<CsoundSynthesizer>
<CsOptions>
-d -n
</CsOptions>
<CsInstruments>
; sr set by host
ksmps = 32
nchnls = 2
0dbfs = 1

;Author: Iain McCurdy (2012)

gihanning    ftgen    0, 0, 8192, 20, 2, 1

opcode    PanTrem,aa,aakkkKkK
    ainL,ainR,krate,kdepth,kmode,kwave,kbias,kduty    xin    ;READ IN INPUT ARGUMENTS
    ktrig    changed    kwave                ;IF LFO WAVEFORM TYPE IS CHANGED GENERATE A MOMENTARY '1' (BANG)
    if ktrig=1 then                    ;IF A 'BANG' HAS BEEN GENERATED IN THE ABOVE LINE
        reinit    UPDATE                ;BEGIN A REINITIALIZATION PASS FROM LABEL 'UPDATE' SO THAT LFO WAVEFORM TYPE CAN BE UPDATED
    endif                        ;END OF THIS CONDITIONAL BRANCH
    UPDATE:                        ;LABEL CALLED UPDATE
    iwave    init        i(kwave)
    iwave    limit    iwave,    0, 6            ;
    if iwave==3 then                ;if 'randomi' is chosen...
     klfo    randomi    -kdepth,kdepth,krate,1
    elseif iwave==4 then                ;or if 'randomh' is chosen...
     klfo    randomh    -kdepth,kdepth,krate,1        
    elseif iwave==5 then                ;or if 'rspline' is chosen...
     klfo    rspline    -kdepth,kdepth,krate,krate*2        
    elseif iwave==6 then                ;or if 'user' has been chosen...
     aphs    phasor    krate                ;create a linear pointer from 0 to 1 
     aphs     pdhalf    aphs, kbias            ;distort the linearity using pdhalf
     kphs    downsamp    aphs            ;downsample to krate
     klfo    tablei    kphs,gihanning,1        ;read 
     klfo    pow    klfo,kduty
     klfo    =    ((klfo*2)-1)*kdepth
    else                        ;otherwise (use lfo opcode)
     klfo    lfo    kdepth, krate, iwave        ;CREATE AN LFO
    endif
    rireturn                    ;RETURN FROM REINITIALIZATION PASS
    klfo    =    (klfo*0.5)+0.5            ;RESCALE AND OFFSET LFO SO IT STAYS WITHIN THE RANGE 0 - 1 ABOUT THE VALUE 0.5
    if iwave=2||iwave==4 then            ;IF SQUARE WAVE MODULATION HAS BEEN CHOSEN...
        klfo    portk    klfo, 0.001        ;SMOOTH THE SQUARE WAVE A TINY BIT TO PREVENT CLICKS
    endif                        ;END OF THIS CONDITIONAL BRANCH    
    if kmode=0 then    ;PAN                ;IF PANNING MODE IS CHOSEN FROM BUTTON BANK...
        alfo    interp    klfo            ;INTERPOLATE K-RATE LFO AND CREATE A-RATE VARIABLE
        aoutL    =    ainL*sqrt(alfo)        ;REDEFINE GLOBAL AUDIO LEFT CHANNEL SIGNAL WITH AUTO-PANNING
        aoutR    =    ainR*(1-sqrt(alfo))    ;REDEFINE GLOBAL AUDIO RIGHT CHANNEL SIGNAL WITH AUTO-PANNING
        kindicator    =    (klfo>0.5?1:0)
        chnset    kindicator,"indicator"
    elseif kmode=1 then    ;TREM            ;IF TREMOLO MODE IS CHOSEN FROM BUTTON BANK...
        kindicator    =    (klfo>0.5?1:0)
        if changed:k(kindicator)==1 then
         chnset    kindicator,"indicator"
        endif
        klfo    =    klfo+(0.5-(kdepth*0.5))    ;MODIFY LFO AT ZERO DEPTH VALUE IS 1 AND AT MAX DEPTH CENTRE OF MODULATION IS 0.5
        alfo    interp    klfo            ;INTERPOLATE K-RATE LFO AND CREATE A-RATE VARIABLE
        aoutL    =    ainL*(alfo^2)        ;REDEFINE GLOBAL AUDIO LEFT CHANNEL SIGNAL WITH TREMELO
        aoutR    =    ainR*(alfo^2)        ;REDEFINE GLOBAL AUDIO RIGHT CHANNEL SIGNAL WITH TREMELO
    endif                        ;END OF THIS CONDITIONAL BRANCH
        xout    aoutL,aoutR            ;SEND AUDIO BACK TO CALLER INSTRUMENT
endop

instr 1
krate chnget "rate"
ktempo chnget "tempo"
kdepth chnget "depth"
kmode chnget "mode"
kwave chnget "wave"
kbias chnget "bias"
kduty chnget "duty"
klevel chnget "level"
ktest    chnget    "test"

ktrig    changed    krate
ktrig2    changed    ktempo
if ktrig=1 then
 chnset    krate*60,"tempo"
elseif ktrig2=1 then
 chnset    ktempo/60,"rate"
endif

if ktest=1 then
 a1    vco2    0.2,300,4,0.5
 a2    =    a1
else
 a1,a2    ins
endif

a1,a2    PanTrem    a1,a2,krate,kdepth,kmode-1,kwave-1,kbias,kduty
a1    =    a1 * klevel
a2    =    a2 * klevel
    outs    a1,a2
endin

</CsInstruments>

<CsScore>
i 1 0 [60*60*24*7]
</CsScore>

</CsoundSynthesizer>