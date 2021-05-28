
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; GEN07.csd
; Demonstration of GEN07
; Written by Iain McCurdy, 2014
;
; GEN07 generates breakpoint functions by joining user-defined values using straight lines each of user-definable duration.
; The user can toggle between using the sliders to input data for the envelope or by drawing and clicking and dragging on 
; the actual waveform. 
;
; An audio test generator uses this function table as a repeating amplitude envelope. 
; The offset value is subtracted so that the envelope can experience values of zero. 

<Cabbage>
form caption("GEN07"), size(250,400), pluginId("gn07"), colour(13, 50, 67,50)

label    bounds( 10,  8, 95, 15), text("Edit Method:")
combobox bounds(105,  5, 80, 20), text("sliders","drawing"), value(1), channel("mode")

gentable bounds( 15, 30, 225, 120), tableNumber(1), tableColour("blue"), tableBackgroundColour("white"), tableGridColour(230,230,230,50), identChannel("table"), ampRange(0,1,1), zoom(-1), active(1)
label    bounds(  0, 26,15,9), text("1")
label    bounds(  0, 38,15,9), text("0.9")
label    bounds(  0, 50,15,9), text("0.8")
label    bounds(  0, 62,15,9), text("0.7")
label    bounds(  0, 74,15,9), text("0.6")
label    bounds(  0, 86,15,9), text("0.5")
label    bounds(  0, 99,15,9), text("0.4")
label    bounds(  0,110,15,9), text("0.3")
label    bounds(  0,122,15,9), text("0.2")
label    bounds(  0,134,15,9), text("0.1")
label    bounds(  0,146,15,9), text("0")
label    bounds( 31,151,15,9), text("0.1")
label    bounds( 54,151,15,9), text("0.2")
label    bounds( 77,151,15,9), text("0.3")
label    bounds( 99,151,15,9), text("0.4")
label    bounds(122,151,15,9), text("0.5")
label    bounds(145,151,15,9), text("0.6")
label    bounds(167,151,15,9), text("0.7")
label    bounds(189,151,15,9), text("0.8")
label    bounds(212,151,15,9), text("0.9")
label    bounds(232,151,15,9), text("1")

rslider  bounds( 40,160, 50, 70), channel("val1"), text("Value.1"), textBox(1), range(0, 1, 0), colour(100,130,130,250), trackerColour("silver")
rslider  bounds(100,160, 50, 70), channel("val2"), text("Value.2"), textBox(1), range(0, 1, 1), colour(100,130,130,250), trackerColour("silver")
rslider  bounds(160,160, 50, 70), channel("val3"), text("Value.3"), textBox(1), range(0, 1, 0), colour(100,130,130,250), trackerColour("silver")

rslider  bounds( 70,235, 50, 70), channel("dur1"), text("Dur.1"), textBox(1), range(0, 1, 0.2, 1, 0.001), colour(130,100,130,250), trackerColour("silver")
rslider  bounds(130,235, 50, 70), channel("dur2"), text("Dur.2"), textBox(1), range(0, 1, 0.8, 1, 0.001), colour(130,100,130,250), trackerColour("silver")

line     bounds( 10,310,225,  2), colour("Grey")
checkbox bounds( 15,320,80, 17), channel("TestGen"), text("Test"),  value(1), colour("yellow"), shape("square")
rslider  bounds( 60,320, 70, 70), channel("speed"), text("Speed"), textBox(1), range(0.25, 8.00, 1,0.5,0.001),   colour(250,230,250,250), trackerColour("silver")
rslider  bounds(120,320, 70, 70), channel("freq"),  text("Freq."), textBox(1), range(50, 5000, 500,0.5,0.1),     colour(250,230,250,250), trackerColour("silver")
rslider  bounds(180,320, 70, 70), channel("lev"),   text("Level"), textBox(1), range(0, 1.00, 0.5),              colour(250,230,250,250), trackerColour("silver")

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

giTabLen    =    1024

instr    1
    kmode    chnget    "mode"
    if kmode==1 then

        ; read in widgets
        gkval1    chnget    "val1"
        gkval2    chnget    "val2"
        gkval3    chnget    "val3"
            
        gkdur1    chnget    "dur1"
        gkdur2    chnget    "dur2"
        
        gkdur1    *=    giTabLen
        gkdur2    *=    giTabLen
        
        ; if any of the variables in the input list are changed, a momentary '1' trigger is generated at the output. This trigger is used to update function tables.
        ktrig    changed        gkval1,gkval2,gkval3,gkdur1,gkdur2
        if ktrig==1 then
         reinit    UPDATE
        endif
        UPDATE:
        ; Update function table
          if    (i(gkdur1)+i(gkdur2))>giTabLen then                ; if sum of segments exceeds table size...
           idur1    =    i(gkdur1)* (giTabLen/(i(gkdur1)+i(gkdur2)))    ; ...scale segment durations down
           idur2    =    i(gkdur2)* (giTabLen/(i(gkdur1)+i(gkdur2)))
           irem        =    0                    ; remainder duration of table
          else                                ; if sum of segments is less than table size...
           idur1    =    i(gkdur1)
           idur2    =    i(gkdur2)
           irem        =    giTabLen - (i(gkdur1) + i(gkdur2))        ; remainder duration of table
          endif
        gi1    ftgen    1, 0,   giTabLen, -7, i(gkval1), idur1, i(gkval2), idur2, i(gkval3), irem, 0
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
    asig    =    asig * aenv                    ; 
        outs    asig, asig
endin

</CsInstruments>

<CsScore>
; play instrument 1 for 1 hour
i 1 0 3600
</CsScore>

</CsoundSynthesizer>
