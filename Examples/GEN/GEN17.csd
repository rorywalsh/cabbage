
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; Demonstration of GEN17
; Written by Iain McCurdy, 2014.

; GEN17 is used to create histograms (step functions) in which the use defines locations and values. That value is then held until the next location is specified.

; In this example the histogram is used as a probability distribution from which notes are randomly chosen. Values define (midi) note numbers.

<Cabbage>
form caption("GEN17"), size(420, 395), pluginId("gn17"), colour(120,170,200, 50)

gentable bounds(  5,  5, 410, 115), identChannel("table1"), tableNumber(1), tableColour("yellow"), outlineThickness(2), ampRange(24,72,1), zoom(-1), fill(0)
image    bounds(  5,  5, 1, 115), identChannel("Scrubber"), alpha(0.5)


groupbox bounds(0, 125,420,170), text("Histogram"), plant("histogram"), fontColour("white"){
rslider bounds( 50, 25, 70, 70), channel("loc1"), text("Len. 1"), valueTextBox(1), textBox(1), range(1, 512, 24,1,1), colour(120,170,200,50), trackerColour("white")
rslider bounds(100, 25, 70, 70), channel("loc2"), text("Len. 2"), valueTextBox(1), textBox(1), range(1, 512, 64,1,1), colour(120,170,200,50), trackerColour("white")
rslider bounds(150, 25, 70, 70), channel("loc3"), text("Len. 3"), valueTextBox(1), textBox(1), range(1, 512, 64,1,1), colour(120,170,200,50), trackerColour("white")
rslider bounds(200, 25, 70, 70), channel("loc4"), text("Len. 4"), valueTextBox(1), textBox(1), range(1, 512, 34,1,1), colour(120,170,200,50), trackerColour("white")
rslider bounds(250, 25, 70, 70), channel("loc5"), text("Len. 5"), valueTextBox(1), textBox(1), range(1, 512, 64,1,1), colour(120,170,200,50), trackerColour("white")
rslider bounds(300, 25, 70, 70), channel("loc6"), text("Len. 6"), valueTextBox(1), textBox(1), range(1, 512, 84,1,1), colour(120,170,200,50), trackerColour("white")
rslider bounds(350, 25, 70, 70), channel("loc7"), text("Len. 7"), valueTextBox(1), textBox(1), range(1, 512, 64,1,1), colour(120,170,200,50), trackerColour("white")

rslider bounds(  0, 95, 70, 70), channel("val0"), text("Value 0"), valueTextBox(1), textBox(1), range(24, 127, 50,1,1), colour(120,170,200, 50), trackerColour("white")
rslider bounds( 50, 95, 70, 70), channel("val1"), text("Value 1"), valueTextBox(1), textBox(1), range(24, 72, 62,1,1),  colour(120,170,200, 50), trackerColour("white")
rslider bounds(100, 95, 70, 70), channel("val2"), text("Value 2"), valueTextBox(1), textBox(1), range(24, 72, 48,1,1),  colour(120,170,200, 50), trackerColour("white")
rslider bounds(150, 95, 70, 70), channel("val3"), text("Value 3"), valueTextBox(1), textBox(1), range(24, 72, 44,1,1),  colour(120,170,200, 50), trackerColour("white")
rslider bounds(200, 95, 70, 70), channel("val4"), text("Value 4"), valueTextBox(1), textBox(1), range(24, 72, 66,1,1),  colour(120,170,200, 50), trackerColour("white")
rslider bounds(250, 95, 70, 70), channel("val5"), text("Value 5"), valueTextBox(1), textBox(1), range(24, 72, 54,1,1),  colour(120,170,200, 50), trackerColour("white")
rslider bounds(300, 95, 70, 70), channel("val6"), text("Value 6"), valueTextBox(1), textBox(1), range(24, 72, 52,1,1),  colour(120,170,200, 50), trackerColour("white")
rslider bounds(350, 95, 70, 70), channel("val7"), text("Value 7"), valueTextBox(1), textBox(1), range(24, 72, 46,1,1),  colour(120,170,200, 50), trackerColour("white")
}


groupbox bounds(0, 295,420,100), text("Synthesiser"), plant("synth"), fontColour("white"){
checkbox bounds( 15, 50,115, 17), channel("SynthOnOff"), text("On/Off"),  value(0), colour("yellow"), shape("square")
rslider  bounds( 75, 25, 70, 70), channel("lev"),  text("Level"), valueTextBox(1), textBox(1), range(0, 1.00, 0.7), colour(120,170,200, 50), trackerColour("white")
rslider  bounds(125, 25, 70, 70), channel("rate"), text("Rate"), valueTextBox(1),  textBox(1), range(0.2, 5.00, 1), colour(120,170,200, 50), trackerColour("white")
rslider  bounds(175, 25, 70, 70), channel("dur"),  text("Dur."), valueTextBox(1),  textBox(1), range(0.1, 3.00, 2), colour(120,170,200, 50), trackerColour("white")
}
</Cabbage>
                    
<CsoundSynthesizer>

<CsOptions>   
-dm0 -n -+rtmidi=NULL -M0
</CsOptions>

<CsInstruments>

; sr set by host
ksmps         =     32    ; NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls         =     2    ; NUMBER OF CHANNELS (1=MONO)
0dbfs        =    1    ; MAXIMUM AMPLITUDE

; default histogram        
gihist    ftgen    1,0, 512, -17, 0, 48, 128, 84, 256, 72

instr    1
    iftlen    =    ftlen(1)        ; length of function  table
    kftlen    init    iftlen
    
    ; read in widgets
    kval0    chnget    "val0"
    kval1    chnget    "val1"
    kval2    chnget    "val2"
    kval3    chnget    "val3"
    kval4    chnget    "val4"
    kval5    chnget    "val5"
    kval6    chnget    "val6"
    kval7    chnget    "val7"

    kloc1    chnget    "loc1"
    kloc2    chnget    "loc2"
    kloc3    chnget    "loc3"
    kloc4    chnget    "loc4"
    kloc5    chnget    "loc5"
    kloc6    chnget    "loc6"
    kloc7    chnget    "loc7"

    kloc1    init    64
    kloc2    init    64
    kloc3    init    64
    kloc4    init    64
    kloc5    init    64
    kloc6    init    64
    kloc7    init    64

    gklev    chnget    "lev"
    gkSynthOnOff    chnget    "SynthOnOff"
    gkrate    chnget    "rate"
    gkdur    chnget    "dur"
    
    ktrig    changed        kval0,kval1,kval2,kval3,kval4,kval5,kval6,kval7, kloc1,kloc2,kloc3,kloc4,kloc5,kloc6,kloc7
    if ktrig==1 then    ; peg rate of update. Tables updated at this rate. If too slow, glitching will be heard in the output, particularly if random movement speed is high. If too high CPU performance will suffer.
     reinit    UPDATE
    endif
    UPDATE:
    gihist    ftgen    1,0, iftlen, -17, 0, i(kval0), i(kloc1),\ 
                                         i(kval1), i(kloc1)+i(kloc2), \
                                         i(kval2), i(kloc1)+i(kloc2)+i(kloc3), \
                                         i(kval3), i(kloc1)+i(kloc2)+i(kloc3)+i(kloc4), \
                                         i(kval4), i(kloc1)+i(kloc2)+i(kloc3)+i(kloc4)+i(kloc5), \
                                         i(kval5), i(kloc1)+i(kloc2)+i(kloc3)+i(kloc4)+i(kloc5)+i(kloc6), \
                                         i(kval6), i(kloc1)+i(kloc2)+i(kloc3)+i(kloc4)+i(kloc5)+i(kloc6)+i(kloc7), \
                                         i(kval7)
    rireturn

    if ktrig==1 then            ; 
     chnset    "tableNumber(1)", "table1"    ; update table display    
    endif
    
    ; TRIGGER SOME NOTES
    kNoteTrig    init    1
    krate    rspline    0.5,2,0.01,0.1
    krhy    trandom    kNoteTrig,0,3
    kNoteTrig    metro    gkrate*krate*(2^int(krhy))
    schedkwhen    kNoteTrig*gkSynthOnOff,0,0,2,0,gkdur
endin

; SCALE FOR REFLECTION DEPENDENT UPON MIDI NOTE NUMBER (LESS DAMPING FOR HIGHER NOTES)
giScal    ftgen    0,0,128, -27,  0, 0.9, 24, 0.9, 36, 0.85, 48, 0.75, 60, 0.65, 72, 0.35, 84, 0.001, 96, 0.001, 127;, 0.001
;giScal    ftgen    0,0,128, -27,  0, 0.983, 24, 0.983, 36, 0.971, 48, 0.939, 60, 0.855, 72, 0.747, 84, 0.364, 96, 0.001, 127


instr    2
    iNdx      random       0, 1                                ; generate a random value
    Smsg      sprintf      "pos(%d,5)",5+iNdx*410              ; create widget message for moving random index indicator 
              chnset       Smsg, "Scrubber"                    ; send position message to widget
    iNote table     iNdx,gihist,1                    ; read a random value from the function table
    aEnv  linsegr    0, 0.005, 1, p3-0.105, 1, 0.1, 0        ; amplitude envelope
    iPlk  random    0.1, 0.3                    ; point at which to pluck the string
    iDtn  random    -0.05, 0.05                    ; random detune
    irefl    table    iNote, giScal                    ; read reflection value from giScal table according to note number  
    aSig  wgpluck2  0.58, gklev*0.5, cpsmidinn(iNote+iDtn), iPlk, irefl    ; generate Karplus-Strong plucked string audio 
    kcf    expon    cpsoct(rnd(6)+6),p3,50                ; filter cutoff frequency envelope
    aSig    clfilt    aSig, kcf, 0, 2                    ; butterworth lowpass filter    
    aL,aR pan2      aSig * aEnv, rnd(0.5)+0.5            ; random panning   
          outs      aL, aR                        ; send audio to outputs
endin


</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
</CsScore>

</CsoundSynthesizer>
