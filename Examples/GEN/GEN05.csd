
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; GEN05.csd
; Demonstration of GEN05
; Written by Iain McCurdy, 2014
; 
; GEN05 generates breakpoint functions by joining user-defined values using exponential curves each of user-definable duration.
; Node values must be non-zero and alike in sign.
; 
; In this example the user can input node values of zero but these will be offset using the value of the 'Offset' control.
; It can be observed how changing this offset value will vary the curvature of segments.
; 
; An audio test generator uses this function table as a repeating amplitude envelope. 
; The offset value is subtracted so that the envelope can experience values of zero. 

<Cabbage>
form caption("GEN05"), size(245, 390), pluginId("gn05"), colour(13, 50, 67,50)

label    bounds( 10,  8, 95, 15), text("Edit Method:")
combobox bounds(105,  5, 80, 20), text("sliders","drawing"), value(1), channel("mode")

gentable bounds( 10, 30, 225, 120), tableNumber(1), tableColour("silver"), identChannel("table"), ampRange(0,1.1,1), zoom(-1), active(1)

rslider bounds( 10,150, 50, 70), channel("val1"), text("Value.1"), textBox(1), range(0, 1, 0), colour(100,130,130,250), trackerColour("silver")
rslider bounds( 70,150, 50, 70), channel("val2"), text("Value.2"), textBox(1), range(0, 1, 1), colour(100,130,130,250), trackerColour("silver")
rslider bounds(130,150, 50, 70), channel("val3"), text("Value.3"), textBox(1), range(0, 1, 0), colour(100,130,130,250), trackerColour("silver")

rslider bounds(190,150, 50, 70), channel("offset"), text("Offset"), textBox(1), range(0.0001, 0.1, 0.001), colour(200,130,130, 50), trackerColour("silver")

rslider bounds( 40,225, 50, 70), channel("dur1"), text("Dur.1"), textBox(1), range(0, 4096, 0, 1, 1), colour(130,100,130,250), trackerColour("silver")
rslider bounds(100,225, 50, 70), channel("dur2"), text("Dur.2"), textBox(1), range(0, 4096, 4096, 1, 1), colour(130,100,130,250), trackerColour("silver")
              
line     bounds( 10,300,225,  2), colour("Grey")
checkbox bounds( 15,310,80, 17), channel("TestGen"), text("Test"),  value(1), colour("yellow"), shape("square")
rslider  bounds( 60,310, 70, 70), channel("speed"), text("Speed"), textBox(1), range(0.25, 8.00, 1,0.5,0.001),   colour(250,230,250,250), trackerColour("silver")
rslider  bounds(120,310, 70, 70), channel("freq"),  text("Freq."), textBox(1), range(50, 5000, 500,0.5,0.1),     colour(250,230,250,250), trackerColour("silver")
rslider  bounds(180,310, 70, 70), channel("lev"),   text("Level"), textBox(1), range(0, 1.00, 0.5),              colour(250,230,250,250), trackerColour("silver")

</Cabbage>
                    
<CsoundSynthesizer>

<CsOptions>   
-dm0 -n -+rtmidi=NULL -M0
</CsOptions>

<CsInstruments>

; sr set by host
ksmps         =     32    ;NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls         =     2    ;NUMBER OF CHANNELS (1=MONO)
0dbfs        =    1    ;MAXIMUM AMPLITUDE

instr    1
    kmode    chnget    "mode"
    if kmode==1 then

        ; read in widgets
        gkval1    chnget    "val1"
        gkval2    chnget    "val2"
        gkval3    chnget    "val3"
            
        gkdur1    chnget    "dur1"
        gkdur2    chnget    "dur2"
            
        gkoffset    chnget    "offset"
        gkoffset    init    0.001
        
        ; if any of the variables in the input list are changed, a momentary '1' trigger is generated at the output. This trigger is used to update function tables.
        ktrig    changed        gkval1,gkval2,gkval3,gkdur1,gkdur2,gkoffset
        if ktrig==1 then
         reinit    UPDATE
        endif
        UPDATE:
        ; Update function table
          if    (i(gkdur1)+i(gkdur2))>4096 then                ; if sum of segments exceeds table size...
           idur1    =    i(gkdur1)* (4096/(i(gkdur1)+i(gkdur2)))    ; ...scale segment durations down
           idur2    =    i(gkdur2)* (4096/(i(gkdur1)+i(gkdur2)))
           irem        =    0                    ; remainder duration of table
          else                                ; if sum of segments is less than table size...
           idur1    =    i(gkdur1)
           idur2    =    i(gkdur2)
           irem        =    4096 - (i(gkdur1) + i(gkdur2))        ; remainder duration of table
          endif
        gi1    ftgen    1, 0,   4096, -5, i(gkval1)+i(gkoffset), idur1, i(gkval2)+i(gkoffset), idur2, i(gkval3)+i(gkoffset), irem, i(gkoffset)
        chnset    "tableNumber(1)", "table"    ; update table display    
        rireturn
    endif

    kTestGen    chnget    "TestGen"                ; test generator on/off
    kspeed    chnget    "speed"
    kamp    chnget    "lev"
    kfreq    chnget    "freq"
    aphasor    phasor    kspeed
    aenv    tablei    aphasor,gi1,1
    asig    vco2    0.4*kamp*kTestGen,kfreq,4,0.5            ; triangle audio wave
    asig    =    asig * (aenv - gkoffset)            ; remove GEN05 offset
        outs    asig, asig
endin

</CsInstruments>

<CsScore>
; create the function table
f 1 0    4096 -16  1 0 0
; play instrument 1 for 1 hour
i 1 0 3600
</CsScore>

</CsoundSynthesizer>
