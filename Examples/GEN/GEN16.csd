
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; GEN16.csd
; Written by Iain McCurdy, 2013
; Demonstration of GEN16
; Envelope repeats once every two seconds therefore 1 second = 2048 table points
; Value 1 and Value 4 should probably be zero.
; Durations are specified in table points - 2048 = 1 second
; If the sum of durations exceeds table size they are automatically scaled down in order to prevent crashes.


<Cabbage>
form caption("GEN16"), size(325, 360), pluginId("gn16"), colour(100,100,110)

gentable bounds( 10,  5, 305, 120), tableNumber(1), tableColour("lime"), identChannel("table"), zoom(-1), ampRange(1,0,1)

#define SLIDER_SETTINGS #textBox(1), fontColour("white"), textColour("white"), trackerColour("silver")#

rslider bounds( 10,130, 50, 70), channel("val1"), text("Value.1"), range(0, 1, 0),   colour(80,80,80), $SLIDER_SETTINGS
rslider bounds( 70,130, 50, 70), channel("val2"), text("Value.2"), range(0, 1, 1),   colour(80,80,80), $SLIDER_SETTINGS
rslider bounds(130,130, 50, 70), channel("val3"), text("Value.3"), range(0, 1, 0.2), colour(80,80,80), $SLIDER_SETTINGS
rslider bounds(190,130, 50, 70), channel("val4"), text("Value.4"), range(0, 1, 0),   colour(80,80,80), $SLIDER_SETTINGS

rslider bounds( 40,205, 50, 70), channel("dur1"), text("Dur.1"), range(1, 4096, 80, 1, 1),   colour(80,80,80), $SLIDER_SETTINGS
rslider bounds(100,205, 50, 70), channel("dur2"), text("Dur.2"), range(1, 4096, 1000, 1, 1), colour(80,80,80), $SLIDER_SETTINGS
rslider bounds(160,205, 50, 70), channel("dur3"), text("Dur.3"), range(1, 4096, 3016, 1, 1), colour(80,80,80), $SLIDER_SETTINGS

rslider bounds( 40,280, 50, 70), channel("shp1"), text("Shape.1"), range(-20, 20, 3),   colour(80,80,80), $SLIDER_SETTINGS
rslider bounds(100,280, 50, 70), channel("shp2"), text("Shape.2"), range(-20, 20, -3),  colour(80,80,80), $SLIDER_SETTINGS
rslider bounds(160,280, 50, 70), channel("shp3"), text("Shape.3"), range(-20, 20, 1.5), colour(80,80,80), $SLIDER_SETTINGS

rslider bounds(265,130, 50, 70), channel("speed"), text("Speed"), range(0.25, 8.00, 0.5,0.5,0.001),  colour(80,80,80), $SLIDER_SETTINGS
rslider bounds(265,205, 50, 70), channel("freq"),  text("Freq."), range(50, 5000, 300,0.5,0.1),      colour(80,80,80), $SLIDER_SETTINGS
rslider bounds(265,280, 50, 70), channel("lev"),   text("Level"), range(0, 1.00, 0.5),               colour(80,80,80), $SLIDER_SETTINGS

line bounds(250,130,  2,220), colour("Grey")

</Cabbage>
                    
<CsoundSynthesizer>

<CsOptions>   
-dm0 -n -+rtmidi=NULL -M0
</CsOptions>

<CsInstruments>

; sr set by host
ksmps     =     32        ; NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls     =     2        ; NUMBER OF CHANNELS (1=MONO)
0dbfs    =    1        ; MAXIMUM AMPLITUDE

opcode    butlpsr,a,aa                    ; butterworth lowpass filter with audio-rate variable cutoff frequency
    asig,acf    xin
        setksmps    1
    kcf    downsamp    acf
    asig    butlp    asig,kcf
        xout    asig
endop

instr    1
    ; read in widgets
    gkval1    chnget    "val1"
    gkval2    chnget    "val2"
    gkval3    chnget    "val3"
    gkval4    chnget    "val4"

    gkdur1    chnget    "dur1"
    gkdur2    chnget    "dur2"
    gkdur3    chnget    "dur3"

    gkshp1    chnget    "shp1"
    gkshp2    chnget    "shp2"
    gkshp3    chnget    "shp3"

    ; if any of the variables in the input list are changed, a momentary '1' trigger is generated at the output. This trigger is used to update function tables.
    ktrig    changed        gkval1,gkval2,gkval3,gkval4,gkdur1,gkdur2,gkdur3,gkshp1,gkshp2,gkshp3
    schedkwhen    ktrig,0,0,2,0,0.01    ; call instrument 2 to update tables and some widgets
    
    kspeed    chnget    "speed"
    aphs    phasor    kspeed
    aamp    tablei    aphs,1,1
    kphs    downsamp    aphs
    ;        chnset    kphs,"table"
    kamp    chnget    "lev"
    kfreq    chnget    "freq"
    asig    vco2    kamp, kfreq
    acf    =    cpsoct((aamp*5)+7)
    asig    butlpsr    asig, acf
    asig    =    asig * aamp

        outs    asig,asig
endin

instr    2
    ; Update function table
      if    (i(gkdur1)+i(gkdur2)+i(gkdur3))>4096 then
       idur1    =    i(gkdur1)* (4096/(i(gkdur1)+i(gkdur2)+i(gkdur3)))
       idur2    =    i(gkdur2)* (4096/(i(gkdur1)+i(gkdur2)+i(gkdur3)))
       idur3    =    i(gkdur3)* (4096/(i(gkdur1)+i(gkdur2)+i(gkdur3)))
      else
       idur1    =    i(gkdur1)
       idur2    =    i(gkdur2)
       idur3    =    i(gkdur3)
      endif
    gi1    ftgen    1, 0,   4096,-16, i(gkval1), idur1, i(gkshp1), i(gkval2), idur2, i(gkshp2), i(gkval3), idur3, i(gkshp3), i(gkval4)
    chnset    "tableNumber(1)", "table"    ; update table display    
endin

</CsInstruments>

<CsScore>
; create the function table
f 1 0    4096 -16  1 0 0
; play instrument 1 for 1 hour
i 1 0 3600
</CsScore>

</CsoundSynthesizer>
