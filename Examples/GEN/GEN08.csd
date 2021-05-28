
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; GEN08.csd
; Written by Iain McCurdy, 2013

; Demonstration of GEN08 (generates as smooth a line as possible between a sequence of user-defined break points)
; The user defines a number of nodes (the amplitude values at these nodes are modulated randomly)
; Random value sequence can be wrapped so that values are repeated for higher nodes.

; Nodes        -    number of nodes
; Wrap        -    number of nodes before wrap-around
; Speed        -    speed of random modulation
; Level        -    amplitude level of the synthesizer
; Reverb    -    amount of reverb 
; Env.Shape    -    Duration of attack and release of synthesizer notes 

<Cabbage>
form caption("GEN08"), size(410, 300), pluginId("gn08"), colour(120,70,170,150)

gentable bounds(  5,  5, 400, 120), identChannel("table1"), tableNumber(1), tableColour("yellow"), ampRange(-1,1,1), outlineThickness(2), tableGridColour(0,0,0,0), zoom(-1), fill(1)

rslider bounds( 15,130, 80, 80), channel("nodes"), text("Nodes"), textBox(1), valueTextBox(1), range(1, 16, 16,1,1),        colour(160,110,210,200), trackerColour("yellow"), outlineColour(100,100,100), fontColour("white"), textColour("white")
rslider bounds( 75,130, 80, 80), channel("wrap"), text("Repeat"), textBox(1), valueTextBox(1), range(2, 16,16,1,1),         colour(160,110,210,200), trackerColour("yellow"), outlineColour(100,100,100), fontColour("white"), textColour("white")
rslider bounds(135,130, 80, 80), channel("speed"), text("Speed"), textBox(1), valueTextBox(1), range(1, 5.00, 1),           colour(160,110,210,200), trackerColour("yellow"), outlineColour(100,100,100), fontColour("white"), textColour("white")
rslider bounds(195,130, 80, 80), channel("level"), text("Level"), textBox(1), valueTextBox(1), range(0, 1.00,0.1),          colour(160,110,210,200), trackerColour("yellow"), outlineColour(100,100,100), fontColour("white"), textColour("white")
rslider bounds(255,130, 80, 80), channel("reverb"), text("Reverb"), textBox(1), valueTextBox(1), range(0, 1.00,0.1),        colour(160,110,210,200), trackerColour("yellow"), outlineColour(100,100,100), fontColour("white"), textColour("white")
rslider bounds(315,130, 80, 80), channel("EnvShape"), text("Env.Shape"), textBox(1), valueTextBox(1), range(0, 2.00,0.5),   colour(160,110,210,200), trackerColour("yellow"), outlineColour(100,100,100), fontColour("white"), textColour("white")

keyboard bounds(  0,220,410, 80)
</Cabbage>
                    
<CsoundSynthesizer>

<CsOptions>   
-dm0 -n -+rtmidi=NULL -M0
</CsOptions>

<CsInstruments>

; sr set by host
ksmps         =     8    ; NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls         =     2    ; NUMBER OF CHANNELS (1=MONO)
0dbfs        =    1    ; MAXIMUM AMPLITUDE
        massign    0,3    ; send all midi notes to instr 3 
        zakinit    16,16

; default waveform        
giwave    ftgen    1,0, 512, 8, 0, 512/16,rnd(2)-1, 512/16,rnd(2)-1, 512/16,rnd(2)-1, 512/16,rnd(2)-1, 512/16,rnd(2)-1, 512/16,rnd(2)-1, 512/16,rnd(2)-1, 512/16,rnd(2)-1, 512/16,rnd(2)-1, 512/16,rnd(2)-1, 512/16,rnd(2)-1, 512/16,rnd(2)-1, 512/16,rnd(2)-1, 512/16,rnd(2)-1, 512/16,rnd(2)-1, 512/16, 0

instr    1
    ; read in widgets
    gknodes    chnget    "nodes"
    gknodes    init    16
    kwrap    chnget    "wrap"
    kwrap    init    16
    kspeed        chnget    "speed"
    gklevel        chnget    "level"
    gkreverb    chnget    "reverb"
    gkEnvShape    chnget    "EnvShape"
    
    iftlen    =    ftlen(1)        ; length of function  table


; define a macro that will be used create a random amplitude
#define    RandStr(N)    
#
    kstr$N    rspline    -0.9,0.9,0.1*kspeed,1*kspeed
        zkw    kstr$N, $N
#
; expand macro multiple times
$RandStr(1)
$RandStr(2)
$RandStr(3)
$RandStr(4)
$RandStr(5)
$RandStr(6)
$RandStr(7)
$RandStr(8)
$RandStr(9)
$RandStr(10)
$RandStr(11)
$RandStr(12)
$RandStr(13)
$RandStr(14)
$RandStr(15)
$RandStr(16)


; define macro for reading random amplitude values        
#define    ReadStr(N)
#
kndx    wrap    $N,1,kwrap
kstr$N    zkr    kndx
#
; expand macro multiple times
$ReadStr(1) 
$ReadStr(2) 
$ReadStr(3) 
$ReadStr(4) 
$ReadStr(5) 
$ReadStr(6) 
$ReadStr(7) 
$ReadStr(8) 
$ReadStr(9) 
$ReadStr(10)
$ReadStr(11)
$ReadStr(12)
$ReadStr(13)
$ReadStr(14)
$ReadStr(15)
$ReadStr(16)

    if metro(200)==1 then    ; peg rate of update. Tables updated at this rate. If too slow, glitching will be heard in the output, particularly if random movement speed is high. If too high CPU performance will suffer.
     reinit    UPDATE
    endif
    UPDATE:
    
    ; generation of wave for each configuration of number of nodes
    ; 1 node
    #define    N    #1#
    if i(gknodes)==$N then
     giwave    ftgen    1,0, iftlen, 8, 0, iftlen/($N+1),i(kstr1),   iftlen/($N+1),   0

    #define    N    #2#
    ; 2 nodes
    elseif i(gknodes)==$N then
     giwave    ftgen    1,0, iftlen, 8, 0, iftlen/($N+1),i(kstr1), iftlen/($N+1),i(kstr2),   iftlen/($N+1),   0

    #define    N    #3#
    elseif i(gknodes)==$N then
     giwave    ftgen    1,0, iftlen, 8, 0, iftlen/($N+1),i(kstr1), iftlen/($N+1),i(kstr2), iftlen/($N+1),i(kstr3),   iftlen/($N+1),   0

    #define    N    #4#
    elseif i(gknodes)==$N then
     giwave    ftgen    1,0, iftlen, 8, 0, iftlen/($N+1),i(kstr1), iftlen/($N+1),i(kstr2), iftlen/($N+1),i(kstr3), iftlen/($N+1),i(kstr4),   iftlen/($N+1),   0

    #define    N    #5#
    elseif i(gknodes)==$N then
     giwave    ftgen    1,0, iftlen, 8, 0, iftlen/($N+1),i(kstr1), iftlen/($N+1),i(kstr2), iftlen/($N+1),i(kstr3), iftlen/($N+1),i(kstr4), iftlen/($N+1),i(kstr5),   iftlen/($N+1),   0

    #define    N    #6#
    elseif i(gknodes)==$N then
     giwave    ftgen    1,0, iftlen, 8, 0, iftlen/($N+1),i(kstr1), iftlen/($N+1),i(kstr2), iftlen/($N+1),i(kstr3), iftlen/($N+1),i(kstr4), iftlen/($N+1),i(kstr5), iftlen/($N+1),i(kstr6),   iftlen/($N+1),   0

    #define    N    #7#
    elseif i(gknodes)==$N then
     giwave    ftgen    1,0, iftlen, 8, 0, iftlen/($N+1),i(kstr1), iftlen/($N+1),i(kstr2), iftlen/($N+1),i(kstr3), iftlen/($N+1),i(kstr4), iftlen/($N+1),i(kstr5), iftlen/($N+1),i(kstr6), iftlen/($N+1),i(kstr7),   iftlen/($N+1),   0

    #define    N    #8#
    elseif i(gknodes)==$N then
     giwave    ftgen    1,0, iftlen, 8, 0, iftlen/($N+1),i(kstr1), iftlen/($N+1),i(kstr2), iftlen/($N+1),i(kstr3), iftlen/($N+1),i(kstr4), iftlen/($N+1),i(kstr5), iftlen/($N+1),i(kstr6), iftlen/($N+1),i(kstr7), iftlen/($N+1),i(kstr8),   iftlen/($N+1),   0

    #define    N    #9#
    elseif i(gknodes)==$N then
     giwave    ftgen    1,0, iftlen, 8, 0, iftlen/($N+1),i(kstr1), iftlen/($N+1),i(kstr2), iftlen/($N+1),i(kstr3), iftlen/($N+1),i(kstr4), iftlen/($N+1),i(kstr5), iftlen/($N+1),i(kstr6), iftlen/($N+1),i(kstr7), iftlen/($N+1),i(kstr8), iftlen/($N+1),i(kstr9),   iftlen/($N+1),   0

    #define    N    #10#
    elseif i(gknodes)==$N then
     giwave    ftgen    1,0, iftlen, 8, 0, iftlen/($N+1),i(kstr1), iftlen/($N+1),i(kstr2), iftlen/($N+1),i(kstr3), iftlen/($N+1),i(kstr4), iftlen/($N+1),i(kstr5), iftlen/($N+1),i(kstr6), iftlen/($N+1),i(kstr7), iftlen/($N+1),i(kstr8), iftlen/($N+1),i(kstr9), iftlen/($N+1),i(kstr10),   iftlen/($N+1),   0

    #define    N    #11#
    elseif i(gknodes)==$N then
     giwave    ftgen    1,0, iftlen, 8, 0, iftlen/($N+1),i(kstr1), iftlen/($N+1),i(kstr2), iftlen/($N+1),i(kstr3), iftlen/($N+1),i(kstr4), iftlen/($N+1),i(kstr5), iftlen/($N+1),i(kstr6), iftlen/($N+1),i(kstr7), iftlen/($N+1),i(kstr8), iftlen/($N+1),i(kstr9), iftlen/($N+1),i(kstr10), iftlen/($N+1),i(kstr11),   iftlen/($N+1),   0

    #define    N    #12#
    elseif i(gknodes)==$N then
     giwave    ftgen    1,0, iftlen, 8, 0, iftlen/($N+1),i(kstr1), iftlen/($N+1),i(kstr2), iftlen/($N+1),i(kstr3), iftlen/($N+1),i(kstr4), iftlen/($N+1),i(kstr5), iftlen/($N+1),i(kstr6), iftlen/($N+1),i(kstr7), iftlen/($N+1),i(kstr8), iftlen/($N+1),i(kstr9), iftlen/($N+1),i(kstr10), iftlen/($N+1),i(kstr11), iftlen/($N+1),i(kstr12),   iftlen/($N+1),   0

    #define    N    #13#
    elseif i(gknodes)==$N then
     giwave    ftgen    1,0, iftlen, 8, 0, iftlen/($N+1),i(kstr1), iftlen/($N+1),i(kstr2), iftlen/($N+1),i(kstr3), iftlen/($N+1),i(kstr4), iftlen/($N+1),i(kstr5), iftlen/($N+1),i(kstr6), iftlen/($N+1),i(kstr7), iftlen/($N+1),i(kstr8), iftlen/($N+1),i(kstr9), iftlen/($N+1),i(kstr10), iftlen/($N+1),i(kstr11), iftlen/($N+1),i(kstr12), iftlen/($N+1),i(kstr13),   iftlen/($N+1),   0

    #define    N    #14#
    elseif i(gknodes)==$N then
     giwave    ftgen    1,0, iftlen, 8, 0, iftlen/($N+1),i(kstr1), iftlen/($N+1),i(kstr2), iftlen/($N+1),i(kstr3), iftlen/($N+1),i(kstr4), iftlen/($N+1),i(kstr5), iftlen/($N+1),i(kstr6), iftlen/($N+1),i(kstr7), iftlen/($N+1),i(kstr8), iftlen/($N+1),i(kstr9), iftlen/($N+1),i(kstr10), iftlen/($N+1),i(kstr11), iftlen/($N+1),i(kstr12), iftlen/($N+1),i(kstr13), iftlen/($N+1),i(kstr14),   iftlen/($N+1),   0

    #define    N    #15#
    elseif i(gknodes)==$N then
     giwave    ftgen    1,0, iftlen, 8, 0, iftlen/($N+1),i(kstr1), iftlen/($N+1),i(kstr2), iftlen/($N+1),i(kstr3), iftlen/($N+1),i(kstr4), iftlen/($N+1),i(kstr5), iftlen/($N+1),i(kstr6), iftlen/($N+1),i(kstr7), iftlen/($N+1),i(kstr8), iftlen/($N+1),i(kstr9), iftlen/($N+1),i(kstr10), iftlen/($N+1),i(kstr11), iftlen/($N+1),i(kstr12), iftlen/($N+1),i(kstr13), iftlen/($N+1),i(kstr14), iftlen/($N+1),i(kstr15),   iftlen/($N+1),   0

    #define    N    #16#
    elseif i(gknodes)==$N then
     giwave    ftgen    1,0, iftlen, 8, 0, iftlen/($N+1),i(kstr1), iftlen/($N+1),i(kstr2), iftlen/($N+1),i(kstr3), iftlen/($N+1),i(kstr4), iftlen/($N+1),i(kstr5), iftlen/($N+1),i(kstr6), iftlen/($N+1),i(kstr7), iftlen/($N+1),i(kstr8), iftlen/($N+1),i(kstr9), iftlen/($N+1),i(kstr10), iftlen/($N+1),i(kstr11), iftlen/($N+1),i(kstr12), iftlen/($N+1),i(kstr13), iftlen/($N+1),i(kstr14), iftlen/($N+1),i(kstr15), iftlen/($N+1),i(kstr16),   iftlen/($N+1),   0

    endif
    
    rireturn

    if metro(32)==1 then            ; peg table display rate. Audio quality is unaffected by this.
     chnset    "tableNumber(1)", "table1"    ; update table display    
    endif
endin

gaSendL,gaSendR    init    0    ; initialise reverb send variables

instr    3
    icps    cpsmidi                    ; CPS from midi note played
    iamp    ampmidi    1                ; amplitude from midi note velocity 
    
    a1    oscili    iamp*gklevel,icps/4,giwave    ; audio oscillator read GEN08 wave created
    a1    *=        oscili:a(1,(icps/4)+oscili:a(icps/100,icps/4,giwave),giwave)    ; ring modulate it with itself
        
    a2    delay    -a1,0.01

    aenv    transegr    0,2*i(gkEnvShape),-4,1,4*i(gkEnvShape),-4,0    ; amplitude envelope

    a1    =    a1 * aenv            ; apply envelope
    a2    =    a2 * aenv            ; apply envelope
    
    gaSendL    =    gaSendL + (a1*gkreverb)
    gaSendR    =    gaSendR + (a2*gkreverb)
        outs    a1*(1-gkreverb), a2*(1-gkreverb)    ; send audio to outputs
endin


instr    Reverb
    a1,a2    reverbsc    gaSendL,gaSendR, 0.85, 8000
        outs        a1,a2
        clear        gaSendL,gaSendR
endin

</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
i "Reverb" 0 [3600*24*7]
</CsScore>

</CsoundSynthesizer>
