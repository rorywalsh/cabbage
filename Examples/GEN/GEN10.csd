
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; GEN10.csd
; Written by Iain McCurdy, 2013
; Demonstration of GEN10
; Create a wave and then play it using the keyboard
; Negative strength values result in inverted phase

<Cabbage>
form caption("GEN10"), size(410, 340), pluginId("gn10"), colour("20,70,170,150")

gentable bounds(  5,  5, 400, 120), identChannel("table1"), tableNumber(1), tableColour("yellow"), zoom(-1)

rslider bounds(  0,130, 60, 70), channel("str1"), text("Str.1"), valueTextBox(1), textBox(1), range(-1.00, 1.00, 1, 1, 0.01), colour(100,100,250,200)
rslider bounds( 50,130, 60, 70), channel("str2"), text("Str.2"), valueTextBox(1), textBox(1), range(-1.00, 1.00, 0, 1, 0.01), colour(100,100,250,200)
rslider bounds(100,130, 60, 70), channel("str3"), text("Str.3"), valueTextBox(1), textBox(1), range(-1.00, 1.00, 0, 1, 0.01), colour(100,100,250,200)
rslider bounds(150,130, 60, 70), channel("str4"), text("Str.4"), valueTextBox(1), textBox(1), range(-1.00, 1.00, 0, 1, 0.01), colour(100,100,250,200)
rslider bounds(200,130, 60, 70), channel("str5"), text("Str.5"), valueTextBox(1), textBox(1), range(-1.00, 1.00, 0, 1, 0.01), colour(100,100,250,200)
rslider bounds(250,130, 60, 70), channel("str6"), text("Str.6"), valueTextBox(1), textBox(1), range(-1.00, 1.00, 0, 1, 0.01), colour(100,100,250,200)
rslider bounds(300,130, 60, 70), channel("str7"), text("Str.7"), valueTextBox(1), textBox(1), range(-1.00, 1.00, 0, 1, 0.01), colour(100,100,250,200)
rslider bounds(350,130, 60, 70), channel("str8"), text("Str.8"), valueTextBox(1), textBox(1), range(-1.00, 1.00, 0, 1, 0.01), colour(100,100,250,200)

checkbox bounds( 24,203, 12, 12), channel("mute1"),  value(0), colour("red"), shape("square")
checkbox bounds( 74,203, 12, 12), channel("mute2"),  value(0), colour("red"), shape("square")
checkbox bounds(124,203, 12, 12), channel("mute3"),  value(0), colour("red"), shape("square")
checkbox bounds(174,203, 12, 12), channel("mute4"),  value(0), colour("red"), shape("square")
checkbox bounds(224,203, 12, 12), channel("mute5"),  value(0), colour("red"), shape("square")
checkbox bounds(274,203, 12, 12), channel("mute6"),  value(0), colour("red"), shape("square")
checkbox bounds(324,203, 12, 12), channel("mute7"),  value(0), colour("red"), shape("square")
checkbox bounds(374,203, 12, 12), channel("mute8"),  value(0), colour("red"), shape("square")

checkbox bounds( 24,228, 12, 12), channel("solo1"),  value(0), colour("yellow"), shape("square")
checkbox bounds( 74,228, 12, 12), channel("solo2"),  value(0), colour("yellow"), shape("square")
checkbox bounds(124,228, 12, 12), channel("solo3"),  value(0), colour("yellow"), shape("square")
checkbox bounds(174,228, 12, 12), channel("solo4"),  value(0), colour("yellow"), shape("square")
checkbox bounds(224,228, 12, 12), channel("solo5"),  value(0), colour("yellow"), shape("square")
checkbox bounds(274,228, 12, 12), channel("solo6"),  value(0), colour("yellow"), shape("square")
checkbox bounds(324,228, 12, 12), channel("solo7"),  value(0), colour("yellow"), shape("square")
checkbox bounds(374,228, 12, 12), channel("solo8"),  value(0), colour("yellow"), shape("square")

label    bounds( 19,216, 24, 10), text("Mute"),  FontColour("white")
label    bounds( 69,216, 24, 10), text("Mute"),  FontColour("white")
label    bounds(119,216, 24, 10), text("Mute"),  FontColour("white")
label    bounds(169,216, 24, 10), text("Mute"),  FontColour("white")
label    bounds(219,216, 24, 10), text("Mute"),  FontColour("white")
label    bounds(269,216, 24, 10), text("Mute"),  FontColour("white")
label    bounds(319,216, 24, 10), text("Mute"),  FontColour("white")
label    bounds(369,216, 24, 10), text("Mute"),  FontColour("white")

label    bounds( 20,241, 21, 10), text("Solo"),  FontColour("white")
label    bounds( 70,241, 21, 10), text("Solo"),  FontColour("white")
label    bounds(120,241, 21, 10), text("Solo"),  FontColour("white")
label    bounds(170,241, 21, 10), text("Solo"),  FontColour("white")
label    bounds(220,241, 21, 10), text("Solo"),  FontColour("white")
label    bounds(270,241, 21, 10), text("Solo"),  FontColour("white")
label    bounds(320,241, 21, 10), text("Solo"),  FontColour("white")
label    bounds(370,241, 21, 10), text("Solo"),  FontColour("white")

keyboard bounds(  0,260,410, 80)
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
        massign    0,3    ; send all midi notes to instr 3 
            
giwave    ftgen    1,0, 4096,10, 0    ; GEN10 generated wave

instr    1
    ; read in widgets
    gkstr1    chnget    "str1"
    gkstr2    chnget    "str2"
    gkstr3    chnget    "str3"
    gkstr4    chnget    "str4"
    gkstr5    chnget    "str5"
    gkstr6    chnget    "str6"
    gkstr7    chnget    "str7"
    gkstr8    chnget    "str8"

    gkmute1    chnget    "mute1"
    gkmute2    chnget    "mute2"
    gkmute3    chnget    "mute3"
    gkmute4    chnget    "mute4"
    gkmute5    chnget    "mute5"
    gkmute6    chnget    "mute6"
    gkmute7    chnget    "mute7"
    gkmute8    chnget    "mute8"

    gksolo1    chnget    "solo1"
    gksolo2    chnget    "solo2"
    gksolo3    chnget    "solo3"
    gksolo4    chnget    "solo4"
    gksolo5    chnget    "solo5"
    gksolo6    chnget    "solo6"
    gksolo7    chnget    "solo7"
    gksolo8    chnget    "solo8"

    kSoloSum    =    gksolo1+gksolo2+gksolo3+gksolo4+gksolo5+gksolo6+gksolo7+gksolo8    ; sum of all solo buttons

#define    SOLO_MUTE_STATUS(N) 
    #
    if gksolo$N==1 then            ; if solo for this voice is on...
     kstatus$N    =    1        ; this partial will be active
    elseif kSoloSum>0 then            ; if any other solo is on...
     kstatus$N    =    0        ; this partial will be inactive
    else                    ; other wise
     kstatus$N    =    (1-gkmute$N)    ; partial will be on if mute is inactive
    endif
    #
    $SOLO_MUTE_STATUS(1)            ; expand macro
    $SOLO_MUTE_STATUS(2)
    $SOLO_MUTE_STATUS(3)
    $SOLO_MUTE_STATUS(4)
    $SOLO_MUTE_STATUS(5)
    $SOLO_MUTE_STATUS(6)
    $SOLO_MUTE_STATUS(7)
    $SOLO_MUTE_STATUS(8)
    ; generate a trigger if any of the input variables changes
    ktrig    changed    gkstr1,gkstr2,gkstr3,gkstr4,gkstr5,gkstr6,gkstr7,gkstr8,kstatus1,kstatus2,kstatus3,kstatus4,kstatus5,kstatus6,kstatus7,kstatus8
    if ktrig==1 then
     reinit UPDATE
    endif
    UPDATE:    
    giwave    ftgen    1,0, 4096,10, i(gkstr1)*i(kstatus1),i(gkstr2)*i(kstatus2),i(gkstr3)*i(kstatus3),i(gkstr4)*i(kstatus4),i(gkstr5)*i(kstatus5),i(gkstr6)*i(kstatus6),i(gkstr7)*i(kstatus7),i(gkstr8)*i(kstatus8)
    rireturn
    if ktrig==1 then
     chnset    "tableNumber(1)", "table1"    ; update table display    
    endif
endin

instr    3
    icps    cpsmidi                ; CPS from midi note played
    iamp    ampmidi    0.5            ; amplitude from midi note velocity 
    a1    oscili    iamp,icps,giwave    ; audio oscillator read GEN10 wave created
    aenv    linsegr    0,0.01,1,0.1,0        ; amplitude envelope to prevent clicks
    a1    =    a1 * aenv        ; apply envelope
        outs    a1, a1            ; send audio to outputs
endin

</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
</CsScore>

</CsoundSynthesizer>
