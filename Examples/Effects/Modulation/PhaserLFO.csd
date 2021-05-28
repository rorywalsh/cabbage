
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; PhaserLFO.csd
; Written by Iain McCurdy, 2012.

<Cabbage>
form caption("Phaser LFO") size(580, 95), pluginId("phsr")
image          bounds(0, 0, 580, 95), colour( 100, 110, 130), shape("rounded"), outlineColour("white"), outlineThickness(4)
label    bounds(10,  8, 75, 10), text("INPUT"), fontColour("White")
combobox bounds(10, 18, 75, 20), channel("input"), value(1), text("Live","Test Tone","Noise")
label    bounds(10, 43, 75, 10), text("LFO SHAPE"), fontColour("White")
combobox bounds(10, 53, 75, 20), channel("shape"), value(1), text("Triangle","Sine","Square","Saw Up","Saw Down","Exp","Log","Rand.Int","Rand.S&H")
line    bounds( 95, 10,190,  2)
label   bounds(155,  7, 70, 10), text("MODULATION"), fontColour("white"), colour(100,110,130)
rslider bounds( 85, 20, 70, 70), text("Rate"),      channel("rate"),     range(0,100.00,0.5,0.5, 0.0001),   colour(50,40,110), textColour("White"), trackerColour(255,255,200)
rslider bounds(155, 20, 70, 70), text("Depth"),     channel("depth"),    range(0, 1.00, 0.5, 1, .01),       colour(50,40,110), textColour("White"), trackerColour(255,255,200)
rslider bounds(225, 20, 70, 70), text("Offset"),    channel("freq"),     range(0, 1.00, 0.4, 1, .01),       colour(50,40,110), textColour("White"), trackerColour(255,255,200)
rslider bounds(295, 20, 70, 70), text("Feedback"),  channel("fback"),    range(0, 1.00, 0.4, 1, .01),       colour(50,40,110), textColour("White"), trackerColour(255,255,200)
rslider bounds(365, 20, 70, 70), text("Stages"),    channel("stages"),   range(1, 64,8, 1, 1),              colour(50,40,110), textColour("White"), trackerColour(255,255,200)
rslider bounds(435, 20, 70, 70), text("Mix"),       channel("mix"),      range(0, 1.00,0.5, 1, .01),        colour(50,40,110), textColour("White"), trackerColour(255,255,200)
rslider bounds(505, 20, 70, 70), text("Level"),     channel("level"),    range(0, 1.00, 1, 1, .01),         colour(50,40,110), textColour("White"), trackerColour(255,255,200)
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
;http://iainmccurdy.org/csound.html

giLog   ftgen  0,0,4096,19, 0.5, 1, 0, 0
giExp   ftgen  0,0,4096,19, 0.5, 1, 180, 1


opcode    Phaser,a,akkkKki
    ain,krate,kdepth,kfreq,kstages,kfback,ishape    xin        ;READ IN INPUT ARGUMENTS
    if ishape=1 then
     klfo    lfo    kdepth*0.5, krate, 1                ;LFO FOR THE PHASER (TRIANGULAR SHAPE)
    elseif ishape=2 then
     klfo    lfo    kdepth*0.5, krate, 0                ;LFO FOR THE PHASER (SINE SHAPE)
    elseif ishape=3 then
     klfo    lfo    kdepth*0.5, krate, 2                ;LFO FOR THE PHASER (SQUARE SHAPE)
    elseif ishape=4 then
     klfo    lfo    kdepth, krate, 4                    ;LFO FOR THE PHASER (SAWTOOTH)
    elseif ishape=5 then
     klfo    lfo    kdepth, krate, 5                    ;LFO FOR THE PHASER (SAWTOOTH)
    elseif ishape=6 then
     klfo    oscil  kdepth, krate, giExp                ;LFO FOR THE PHASER (EXP)
    elseif ishape=7 then
     klfo    oscil  kdepth, krate, giLog                ;LFO FOR THE PHASER (LOG)
    elseif ishape=8 then
     klfo    randomi    -kdepth*0.5, kdepth*0.5, krate*8        ;LFO FOR THE PHASER (RANDOMI SHAPE)
     klfo    portk    klfo, 1/(krate*8)                ;SMOOTH CHANGES OF DIRECTION
    elseif ishape=9 then
     klfo    randomh    -kdepth*0.5, kdepth*0.5, krate            ;LFO FOR THE PHASER (RANDOMH SHAPE)
    endif    
    aout    phaser1    ain, cpsoct((klfo+(kdepth*0.5)+kfreq)), kstages, kfback    ;PHASER1 IS APPLIED TO THE INPUT AUDIO
        xout    aout                            ;SEND AUDIO BACK TO CALLER INSTRUMENT
endop

opcode    PhaserSt,aa,aakkkKki
    ainL,ainR,krate,kdepth,kfreq,kstages,kfback,ishape    xin        ;READ IN INPUT ARGUMENTS
    if ishape=1 then
     klfo    lfo    kdepth*0.5, krate, 1                ;LFO FOR THE PHASER (TRIANGULAR SHAPE)
    elseif ishape=2 then
     klfo    lfo    kdepth*0.5, krate, 0                ;LFO FOR THE PHASER (SINE SHAPE)
    elseif ishape=3 then
     klfo    lfo    kdepth*0.5, krate, 2                ;LFO FOR THE PHASER (SQUARE SHAPE)
    elseif ishape=4 then
     klfo    lfo    kdepth, krate, 4                    ;LFO FOR THE PHASER (SAWTOOTH)
    elseif ishape=5 then
     klfo    lfo    kdepth, krate, 5                    ;LFO FOR THE PHASER (SAWTOOTH)
    elseif ishape=6 then
     klfo    oscil  kdepth, krate, giExp                ;LFO FOR THE PHASER (EXP)
    elseif ishape=7 then
     klfo    oscil  kdepth, krate, giLog                ;LFO FOR THE PHASER (LOG)
    elseif ishape=8 then
     klfo    randomi    -kdepth*0.5, kdepth*0.5, krate*8        ;LFO FOR THE PHASER (RANDOMI SHAPE)
     klfo    portk    klfo, 1/(krate*8)                ;SMOOTH CHANGES OF DIRECTION
    elseif ishape=9 then
     klfo    randomh    -kdepth*0.5, kdepth*0.5, krate            ;LFO FOR THE PHASER (RANDOMH SHAPE)
    endif    
    aoutL    phaser1    ainL, cpsoct((klfo+(kdepth*0.5)+kfreq)), kstages, kfback    ;PHASER1 IS APPLIED TO THE INPUT AUDIO
    aoutR    phaser1    ainR, cpsoct((klfo+(kdepth*0.5)+kfreq)), kstages, kfback    ;PHASER1 IS APPLIED TO THE INPUT AUDIO
        xout    aoutL,aoutR                            ;SEND AUDIO BACK TO CALLER INSTRUMENT
endop

        instr 1
kporttime    linseg        0,0.001,0.05
krate        chnget        "rate"
kdepth         chnget         "depth"
kdepth        portk        kdepth,kporttime
kfreq         chnget         "freq"
kfreq        portk        kfreq,kporttime
kfback         chnget         "fback"
kstages     chnget         "stages"
klevel         chnget         "level"
kmix        chnget        "mix"
kshape        chnget        "shape"
kshape        init        1

/* INPUT */
kinput        chnget    "input"
if kinput=1 then
 a1,a2        ins
elseif kinput=2 then
 a1        vco2        0.1,200
 a2        =        a1
else
 a1        pinkish        0.1
 a2        pinkish        0.1
endif

ktrig        changed        kshape,kstages                            ; reinitialise for i-rate parms
if ktrig=1 then
 reinit     RESTART_PHASER
endif
RESTART_PHASER:
;aPhs1        Phaser        a1,krate,kdepth*8,(kfreq*5)+6,kstages,kfback*0.9,i(kshape)    ; call UDO
;aPhs2        Phaser        a2,krate,kdepth*8,(kfreq*5)+6,kstages,kfback*0.9,i(kshape)
aPhs1,aPhs2    PhaserSt    a1,a2,krate,kdepth*8,(kfreq*10)+4,kstages,kfback*0.9,i(kshape)    ; use stereo version to ensure lfo sync for random lfos

rireturn
a1        ntrpol        a1,aPhs1,kmix                            ; wet/dry mix
a2        ntrpol        a2,aPhs2,kmix
        outs        a1* klevel, a2* klevel
        endin

</CsInstruments>

<CsScore>
i 1 0 [60*60*24*7]
</CsScore>

</CsoundSynthesizer>