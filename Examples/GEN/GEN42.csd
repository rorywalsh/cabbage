
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; GEN42.csd
; Demonstration of GEN42
; Written by Iain McCurdy, 2014.

<Cabbage>
form caption("GEN42"), size(420,495), pluginId("gn42"), colour(210,110, 80,200)

gentable bounds(  5,  5, 410, 145), identChannel("table1"), tableNumber(1), outlineThickness(2), tableColour("yellow"), ampRange(24,96,1), zoom(-1), fill(0)

image    bounds(  5,  5, 1, 145), identChannel("Scrubber"), alpha(0.5)


groupbox bounds(0, 155,420,240), text("Histogram"), plant("histogram"), fontColour("white"){
rslider bounds(  0, 25, 70, 70), channel("prob1"), text("Prob. 1"), valueTextBox(1), textBox(1), range(0, 100, 24,1,1), colour(210,110, 80,200), trackerColour("white")
rslider bounds( 50, 25, 70, 70), channel("prob2"), text("Prob. 2"), valueTextBox(1), textBox(1), range(0, 100, 64,1,1), colour(210,110, 80,200), trackerColour("white")
rslider bounds(100, 25, 70, 70), channel("prob3"), text("Prob. 3"), valueTextBox(1), textBox(1), range(0, 100, 64,1,1), colour(210,110, 80,200), trackerColour("white")
rslider bounds(150, 25, 70, 70), channel("prob4"), text("Prob. 4"), valueTextBox(1), textBox(1), range(0, 100, 34,1,1), colour(210,110, 80,200), trackerColour("white")
rslider bounds(200, 25, 70, 70), channel("prob5"), text("Prob. 5"), valueTextBox(1), textBox(1), range(0, 100, 64,1,1), colour(210,110, 80,200), trackerColour("white")
rslider bounds(250, 25, 70, 70), channel("prob6"), text("Prob. 6"), valueTextBox(1), textBox(1), range(0, 100, 84,1,1), colour(210,110, 80,200), trackerColour("white")
rslider bounds(300, 25, 70, 70), channel("prob7"), text("Prob. 7"), valueTextBox(1), textBox(1), range(0, 100, 64,1,1), colour(210,110, 80,200), trackerColour("white")
rslider bounds(350, 25, 70, 70), channel("prob8"), text("Prob. 8"), valueTextBox(1), textBox(1), range(0, 100, 64,1,1), colour(210,110, 80,200), trackerColour("white")

rslider bounds(  0, 95, 70, 70), channel("min1"), text("Min 1"), valueTextBox(1), textBox(1), range(24, 96, 50,1,1), colour(210,110, 80,200), trackerColour("white")
rslider bounds( 50, 95, 70, 70), channel("min2"), text("Min 2"), valueTextBox(1), textBox(1), range(24, 96, 62,1,1), colour(210,110, 80,200), trackerColour("white")
rslider bounds(100, 95, 70, 70), channel("min3"), text("Min 3"), valueTextBox(1), textBox(1), range(24, 96, 48,1,1), colour(210,110, 80,200), trackerColour("white")
rslider bounds(150, 95, 70, 70), channel("min4"), text("Min 4"), valueTextBox(1), textBox(1), range(24, 96, 44,1,1), colour(210,110, 80,200), trackerColour("white")
rslider bounds(200, 95, 70, 70), channel("min5"), text("Min 5"), valueTextBox(1), textBox(1), range(24, 96, 56,1,1), colour(210,110, 80,200), trackerColour("white")
rslider bounds(250, 95, 70, 70), channel("min6"), text("Min 6"), valueTextBox(1), textBox(1), range(24, 96, 54,1,1), colour(210,110, 80,200), trackerColour("white")
rslider bounds(300, 95, 70, 70), channel("min7"), text("Min 7"), valueTextBox(1), textBox(1), range(24, 96, 52,1,1), colour(210,110, 80,200), trackerColour("white")
rslider bounds(350, 95, 70, 70), channel("min8"), text("Min 8"), valueTextBox(1), textBox(1), range(24, 96, 76,1,1), colour(210,110, 80,200), trackerColour("white")

rslider bounds(  0,165, 70, 70), channel("max1"), text("Max 1"), valueTextBox(1), textBox(1), range(24, 96, 51,1,1), colour(210,110, 80,200), trackerColour("white")
rslider bounds( 50,165, 70, 70), channel("max2"), text("Max 2"), valueTextBox(1), textBox(1), range(24, 96, 61,1,1), colour(210,110, 80,200), trackerColour("white")
rslider bounds(100,165, 70, 70), channel("max3"), text("Max 3"), valueTextBox(1), textBox(1), range(24, 96, 49,1,1), colour(210,110, 80,200), trackerColour("white")
rslider bounds(150,165, 70, 70), channel("max4"), text("Max 4"), valueTextBox(1), textBox(1), range(24, 96, 44,1,1), colour(210,110, 80,200), trackerColour("white")
rslider bounds(200,165, 70, 70), channel("max5"), text("Max 5"), valueTextBox(1), textBox(1), range(24, 96, 57,1,1), colour(210,110, 80,200), trackerColour("white")
rslider bounds(250,165, 70, 70), channel("max6"), text("Max 6"), valueTextBox(1), textBox(1), range(24, 96, 53,1,1), colour(210,110, 80,200), trackerColour("white")
rslider bounds(300,165, 70, 70), channel("max7"), text("Max 7"), valueTextBox(1), textBox(1), range(24, 96, 53,1,1), colour(210,110, 80,200), trackerColour("white")
rslider bounds(350,165, 70, 70), channel("max8"), text("Max 8"), valueTextBox(1), textBox(1), range(24, 96, 75,1,1), colour(210,110, 80,200), trackerColour("white")
}

groupbox bounds(0, 395,420,100), text("Synthesiser"), plant("synth"), fontColour("white"){
checkbox bounds( 15, 50,115, 17), channel("SynthOnOff"), text("On/Off"),  value(0), colour:1("yellow"), colour:0("black"), shape("square")
rslider  bounds( 75, 25, 70, 70), channel("lev"),  text("Level"), valueTextBox(1), textBox(1), textBox(1), range(0, 1.00, 0.7), colour(210,110, 80,200), trackerColour("white")
rslider  bounds(125, 25, 70, 70), channel("rate"), text("Rate"), valueTextBox(1), textBox(1),  textBox(1), range(0.2, 5.00, 2), colour(210,110, 80,200), trackerColour("white")
rslider  bounds(175, 25, 70, 70), channel("dur"),  text("Dur."), valueTextBox(1), textBox(1),  textBox(1), range(0.1, 3.00, 2), colour(210,110, 80,200), trackerColour("white")
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

; default waveform        
gihist    ftgen    1,0, 512, -42, 0,0, 100 ;

instr    1
    iftlen    =    ftlen(1)        ; length of function  table
    kftlen    init    iftlen
    
    ; read in widgets
    kmin1    chnget    "min1"
    kmin2    chnget    "min2"
    kmin3    chnget    "min3"
    kmin4    chnget    "min4"
    kmin5    chnget    "min5"
    kmin6    chnget    "min6"
    kmin7    chnget    "min7"
    kmin8    chnget    "min8"

    kmax1    chnget    "max1"
    kmax2    chnget    "max2"
    kmax3    chnget    "max3"
    kmax4    chnget    "max4"
    kmax5    chnget    "max5"
    kmax6    chnget    "max6"
    kmax7    chnget    "max7"
    kmax8    chnget    "max8"

    kprob1    chnget    "prob1"
    kprob2    chnget    "prob2"
    kprob3    chnget    "prob3"
    kprob4    chnget    "prob4"
    kprob5    chnget    "prob5"
    kprob6    chnget    "prob6"
    kprob7    chnget    "prob7"
    kprob8    chnget    "prob8"

    gklev    chnget    "lev"
    gkSynthOnOff    chnget    "SynthOnOff"
    gkrate    chnget    "rate"
    gkdur    chnget    "dur"

    ktrig    changed        kmin1,kmin2,kmin3,kmin4,kmin5,kmin6,kmin7,kmin8, kmax1,kmax2,kmax3,kmax4,kmax5,kmax6,kmax7,kmax8, kprob1,kprob2,kprob3,kprob4,kprob5,kprob6,kprob7,kprob8
    if ktrig==1 then    ; peg rate of update. Tables updated at this rate. If too slow, glitching will be heard in the output, particularly if random movement speed is high. If too high CPU performance will suffer.
     reinit    UPDATE
    endif
    UPDATE:
    gihist    ftgen    1,0, iftlen, -42, i(kmin1), i(kmax1), i(kprob1),\
                                      i(kmin2), i(kmax2), i(kprob2),\
                                      i(kmin3), i(kmax3), i(kprob3),\
                                      i(kmin4), i(kmax4), i(kprob4),\
                                      i(kmin5), i(kmax5), i(kprob5),\
                                      i(kmin6), i(kmax6), i(kprob6),\
                                      i(kmin7), i(kmax7), i(kprob7),\
                                      i(kmin8), i(kmax8), i(kprob8)


    rireturn

    if ktrig==1 then            ; 
     chnset    "tableNumber(1)", "table1"    ; update table display    
    endif

    ; TRIGGER SOME NOTES
    kNoteTrig    init    1
    krhy    trandom    kNoteTrig,0,3
    kNoteTrig    metro    gkrate*(2^int(krhy))
    krate        randomi    0.5,4,1,1
    schedkwhen    kNoteTrig*gkSynthOnOff,0,0,2,0,gkdur
endin

; SCALE FOR REFLECTION DEPENDENT UPON MIDI NOTE NUMBER
;giScal    ftgen    0,0,128, -27,  0, 0.9, 24, 0.9, 36, 0.85, 48, 0.75, 60, 0.65, 72, 0.35, 84, 0.001, 96, 0.001, 127;, 0.001
giScal    ftgen    0,0,128, -27,  0, 0.983, 24, 0.983, 36, 0.971, 48, 0.939, 60, 0.855, 72, 0.747, 84, 0.364, 96, 0.001, 127

gasendL,gasendR    init    0

instr    2
    iNdx      random       0, 1                                ; generate a random value
    Smsg      sprintf      "pos(%d,5)",5+iNdx*410              ; create widget message for moving random index indicator 
              chnset       Smsg, "Scrubber"                    ; send position message to widget
    iNote table     iNdx,gihist,1                     ; read a random value from the function table
    aEnv  linsegr    0, 0.005, 1, p3-0.105, 1, 0.1, 0 ; amplitude envelope
    iPlk  random    0.1, 0.3                         ; point at which to pluck the string
    iDtn  random    -0.05, 0.05                      ; random detune
    irefl    table    iNote, giScal
    aSig  wgpluck2  0.58, 0.5*gklev, cpsmidinn(iNote+iDtn), iPlk, irefl        
    kcf    expon    cpsoct(rnd(6)+6),p3,50
    aSig    clfilt    aSig, kcf, 0, 2                    ; butterworth lowpass filter     
    aL,aR pan2      aSig * aEnv, rnd(0.5)+0.5        ; random panning   
          outs      aL, aR
    gasendL    =    gasendL + aL*0.2
    gasendR    =    gasendR + aR*0.2
                                                                                 
endin

instr    99
    aL,aR    reverbsc    gasendL,gasendR,0.82,7000
        outs        aL,aR
        clear        gasendL,gasendR
endin

</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
i 99 0 [3600*24*7]
f 0 3600
</CsScore>

</CsoundSynthesizer>
