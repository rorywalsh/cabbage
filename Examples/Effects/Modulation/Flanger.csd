
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; Flanger.csd
; Written by Iain McCurdy, 2012.

<Cabbage>
form caption("Flanger") size(510,100), pluginId("flan")
image pos(0, 0), size(510,100), colour("lightgreen"), shape("rounded"), outlineColour("white"), outlineThickness(4) 
line     bounds( 15, 10,190,  2)
#define RSLIDER_ATTRIBUTES # colour("LightGreen"),  colour(230,255,230), trackerColour( 50,150, 50), textColour(  0, 30,  0) #
label    bounds( 75,  7, 70, 10), text("MODULATION"), fontColour("white"), colour(100,110,130)

rslider  bounds(  5, 20, 70, 70), text("Rate"),     channel("rate"),  range(0.001, 40, 0.15, 0.5, 0.001),       $RSLIDER_ATTRIBUTES
rslider  bounds( 75, 20, 70, 70), text("Depth"),    channel("depth"), range(0, 0.01, 0.005,1,0.0001),           $RSLIDER_ATTRIBUTES
rslider  bounds(145, 20, 70, 70), text("Offset"),   channel("delay"), range(0.00002, 0.1, 0.0001, 0.5, 0.0001), $RSLIDER_ATTRIBUTES
rslider  bounds(215, 20, 70, 70), text("Feedback"), channel("fback"), range(-1, 1, 0),                          $RSLIDER_ATTRIBUTES
rslider  bounds(370, 20, 70, 70), text("Mix"),      channel("mix"),   range(0, 1.00, 0.5),                      $RSLIDER_ATTRIBUTES
rslider  bounds(435, 20, 70, 70), text("Level"),    channel("level"), range(0, 1.00, 1),                        $RSLIDER_ATTRIBUTES

checkbox bounds(290, 20, 80, 15), colour("yellow"),  channel("ThruZero"),  value(1), text("Thru.Zero"), fontColour:0(  0, 30,  0), fontColour:1(  0, 30,  0)
label    bounds(295, 47, 65, 12), text("LFO Shape:"), fontColour(  0, 30,  0)
combobox bounds(290, 60, 80, 18), channel("lfoshape"), value(1), text("parabola", "sine", "triangle", "randomi", "randomh")
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

opcode    Flanger,a,akkkkk
    ain,krate,kdepth,kdelay,kfback,klfoshape    xin            ;read in input arguments
    iparabola    ftgentmp    0, 0, 131072, 19, 0.5, 1, 180, 1    ;u-shape parabola for lfo
    isine        ftgentmp    0, 0, 131072, 19, 1, 0.5, 0,   0.5     ;sine-shape for lfo
    itriangle    ftgentmp    0, 0, 131072, 7, 0,131072/2,1,131072/2,0;triangle-shape for lfo
    adlt        interp        kdelay                            ;a new a-rate variable 'adlt' is created by interpolating the k-rate variable 'kdlt'
    if klfoshape==1 then
     amod        oscili        kdepth, krate, iparabola            ;oscillator that makes use of the positive domain only u-shape parabola
    elseif klfoshape==2 then
     amod        oscili        kdepth, krate, isine            ;oscillator that makes use of the positive domain only sine wave
    elseif klfoshape==3 then
     amod        oscili        kdepth, krate, itriangle            ;oscillator that makes use of the positive domain only triangle
    elseif klfoshape==4 then    
     amod        randomi        0,kdepth,krate,1
    else    
     amod        randomh        0,kdepth,krate,1
    endif
    adlt        sum        adlt, amod                ;static delay time and modulating delay time are summed
    adelsig        flanger     ain, adlt, kfback , 1.2            ;flanger signal created
    adelsig        dcblock        adelsig
    aout        sum        ain*0.5, adelsig*0.5            ;create dry/wet mix 
            xout        aout                    ;send audio back to caller instrument
endop

instr 1
    krate chnget "rate"                ;read in widgets
    kdepth chnget "depth"
    kdelay chnget "delay"
    kfback chnget "fback"
    klevel chnget "level"
    klfoshape chnget "lfoshape"
    kThruZero chnget "ThruZero"
    kmix chnget "mix"
    
    a1,a2    ins                    ;read live stereo audio input
    ;a1    pinkish    0.2                ;for testing...
    ;a2    pinkish    0.2

    kporttime    linseg    0,0.001,0.1
    kdelay    portk    kdelay,kporttime

    afla1    Flanger    a1,krate,kdepth,kdelay,kfback,klfoshape    ;call udo (left channel)
    afla2    Flanger    a2,krate,kdepth,kdelay,kfback,klfoshape    ;call udo (right channel)

    if kThruZero==1 then                ;if 'Thru.Zero' mode is selected...
     a1    delay    a1,0.00002
     a2    delay    a2,0.00002
     a1    ntrpol    -a1,afla1,kmix            ;invert delayed dry signal and mix with flanger signal
     a2    ntrpol    -a2,afla2,kmix
    else                        ;otherwise... (standard flanger)
     a1    ntrpol    a1,afla1,kmix            ;create mix between dry signal and flanger signal
     a2    ntrpol    a2,afla2,kmix
    endif
        outs    a1*klevel,a2*klevel        ;send audio to outputs, scale amplitude according to GUI knob value
endin

</CsInstruments>

<CsScore>
i 1 0 [60*60*24*7]
</CsScore>

</CsoundSynthesizer>