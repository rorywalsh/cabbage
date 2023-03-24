
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; phaser1.csd
; Written by Iain McCurdy, 2012.

<Cabbage>
form caption("phaser1") size(400,180), pluginId("phs1")

#define RSLIDER_ATTRIBUTES colour("Blue"), colour(200,150,150), textColour("LightGrey"), trackerColour(250,200,200), valueTextBox(1)

image        pos(0, 0), size(400,180), colour(80,20,20), shape("rounded"), outlineColour("white"), outlineThickness(4) 
label     bounds( 10, 15, 55, 12), text("INPUT"), fontColour("LightGrey")
checkbox  bounds( 10, 30, 55, 12), text("Live"), fontColour:0("LightGrey"), fontColour:1("LightGrey"), channel("input"),  value(1), radioGroup(1)
checkbox  bounds( 10, 45, 55, 12), text("Noise"), fontColour:0("LightGrey"), fontColour:1("LightGrey"), channel("input2"), value(0), radioGroup(1)
rslider   bounds( 60, 11, 70, 80),  text("Frequency"), channel("freq"), range(20.0, 5000, 160, 0.25, 0.1) $RSLIDER_ATTRIBUTES
rslider   bounds(125, 11, 70, 80),  text("Feedback"), channel("feedback"), range(-0.99, 0.99, 0.9) $RSLIDER_ATTRIBUTES
rslider   bounds(190, 11, 70, 80), text("N.Ords."), channel("ord"), range(1, 256, 32, 0.5,1) $RSLIDER_ATTRIBUTES
rslider   bounds(255, 11, 70, 80), text("Mix"), channel("mix"), range(0, 1.00, 1) $RSLIDER_ATTRIBUTES
rslider   bounds(320, 11, 70, 80), text("Level"), channel("level"), range(0, 1.00, 0.7) $RSLIDER_ATTRIBUTES
}

label     bounds( 5, 60, 60, 12), text("FREQ."), fontColour("white")
combobox  bounds( 5, 73, 60, 20), channel("FreqSource"), value(1), text("Dial", "MIDI")

keyboard bounds(  5,100,390, 80)
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-dm0 -n -+rtmidi=NULL -M0
</CsOptions>

<CsInstruments>

; sr set by host
ksmps         =     32    ;NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls         =     2    ;NUMBER OF CHANNELS (2=STEREO)
0dbfs        =    1

;Author: Iain McCurdy (2012)

massign 0,2


instr    1
 kFreqSource   chnget "FreqSource"
    gkfreq        chnget    "freq"                    ;READ WIDGETS...
    gkfeedback    chnget    "feedback"
    gkord        chnget    "ord"
    gkmix        chnget    "mix"
    gklevel        chnget    "level"
    gkinput        chnget    "input"

 if changed:k(kFreqSource)==1 then
  if kFreqSource==1 then
   event "i",2,0,-1
  else
   event "i",-2,0,-1
  endif
 endif
 
endin




instr    2
        kporttime    linseg    0,0.01,0.03                ;CREATE A VARIABLE THAT WILL BE USED FOR PORTAMENTO TIME

 ; MIDI AND GUI INTEROPERABILITY
 iMIDIflag    =    0                 ; IF MIDI ACTIVATED = 1, NON-MIDI = 0
 mididefault    1, iMIDIflag         ; IF NOTE IS MIDI ACTIVATED REPLACE iMIDIflag WITH '1'
 
 if iMIDIflag==1 then                ; IF THIS IS A MIDI ACTIVATED NOTE...
  icps     cpsmidi
  kfreq      =         icps
 else
  kfreq     =         gkfreq
    kfreq        portk    kfreq, kporttime            ;PORTAMENTO IS APPLIED TO 'SMOOTH' SLIDER MOVEMENT    
 endif                        ; END OF THIS CONDITIONAL BRANCH
 
    if gkinput==1 then
     asigL,asigR    ins
    else
     asigL    pinker
     asigR    pinker
    endif

    kSwitch        changed    gkord                    ;GENERATE A MOMENTARY '1' PULSE IN OUTPUT 'kSwitch' IF ANY OF THE SCANNED INPUT VARIABLES CHANGE. (OUTPUT 'kSwitch' IS NORMALLY ZERO)
    if    kSwitch=1    then                    ;IF I-RATE VARIABLE CHANGE TRIGGER IS '1'...
        reinit    UPDATE                        ;BEGIN A REINITIALISATION PASS FROM LABEL 'UPDATE'
    endif                                ;END OF CONDITIONAL BRANCH
    UPDATE:                                ;BEGIN A REINITIALISATION PASS FROM HERE
    aphaserl    phaser1        asigL, kfreq, gkord, gkfeedback    ;PHASER1 IS APPLIED TO THE LEFT CHANNEL
    aphaserr    phaser1        asigR, kfreq, gkord, gkfeedback    ;PHASER1 IS APPLIED TO THE RIGHT CHANNEL
    rireturn                            ;RETURN FROM REINITIALISATION PASS TO PERFORMANCE TIME PASSES
    amixL        ntrpol    asigL,aphaserl,gkmix
    amixR        ntrpol    asigR,aphaserr,gkmix
            outs    amixL*gklevel, amixR*gklevel        ;PHASER OUTPUT ARE SENT TO THE SPEAKERS
endin
        
</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
</CsScore>


</CsoundSynthesizer>



























