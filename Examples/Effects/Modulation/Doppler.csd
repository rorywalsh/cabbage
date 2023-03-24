
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; doppler.csd
; Written by Iain McCurdy, 2013

; CONTROLS
; Input          -    select input: either left channel, right channel, or a mix of both channels
; Shape          -    shape of the LFO moving the source: either sine, triangle, random spline OR manual
;                     note, if 'random' is chosen, 'speed' can take a little while to respond to changes
;                     made to the 'speed' control is the speed was previously slow
;                     If 'manual' is selected source position is controlled using the on screen slider
; Room Size       -   effectively the dpeth of the doppler pitch modulating effect
; Speed           -   speed of the LFO moving the source with respect to the mic. position
; Depth           -   amplitude of the LFO moving the source
; Smoothing       -   a smoothing filter applied to doppler pitch modulation. Its effect can be subtle.
; Mix             -   a dry/wet mixer. Mixing the dry and wet signals can be used to create chorus effects.
; Ampscale        -   amount of amplitude drop off as the source moves away from the source. 
                      Kind of like another room size control
; Pan Depth       -   Amount of left-right movement in the output as the source swings past the microphone
; Out Amp         -   scales the output signal
; Mic.Position    -   Position of the microphone
; Source Position -   Location of the source (for display only unless 'manual' shape is chosen)

<Cabbage>
form caption("-oOo-"), size(610, 180), colour("Black"), pluginId("dopp")
label    bounds(20, 10, 35,11), text("Input:")
combobox bounds(10, 22, 60,18), channel("input"), value(4), text("left","right","mixed","test")
label    bounds(20, 50, 35,11), text("Shape:")
combobox bounds(10, 62, 60,18), channel("shape"), value(1), text("sine","triangle","random","manual")

rslider bounds( 75, 10, 80, 80), channel("RoomSize"), range(0.1,100,40,0.5,0.5), text("Room Size"), textBox(1), valueTextBox(1), colour( 45, 45, 45), trackerColour(200,200,200)
rslider bounds(140, 10, 80, 80), channel("speed"), range(0,10,0.08,0.5,0.01), text("Speed"), textBox(1), valueTextBox(1),        colour( 45, 45, 45), trackerColour(200,200,200)
rslider bounds(205, 10, 80, 80), channel("depth"), range(0,0.5,0.5,0.5,0.01), text("Depth"), textBox(1), valueTextBox(1),        colour( 45, 45, 45), trackerColour(200,200,200)
rslider bounds(270, 10, 80, 80), channel("filtercutoff"), range(1,20,6,1,1), text("Smooth"), textBox(1), valueTextBox(1),     colour( 45, 45, 45), trackerColour(200,200,200)
rslider bounds(335, 10, 80, 80), channel("ampscale"), range(0,1,0.98), text("Amp.Scl."), textBox(1), valueTextBox(1),           colour( 45, 45, 45), trackerColour(200,200,200)
rslider bounds(400, 10, 80, 80), channel("PanDep"), range(0,0.5,0.4), text("Pan Depth"), textBox(1), valueTextBox(1),            colour( 45, 45, 45), trackerColour(200,200,200)
rslider bounds(465, 10, 80, 80), channel("mix"), range(0,1,1), text("Mix"), textBox(1), valueTextBox(1),                         colour( 45, 45, 45), trackerColour(200,200,200)
rslider bounds(530, 10, 80, 80), channel("OutAmp"), range(0,1,0.5), text("Level"), textBox(1), valueTextBox(1),                  colour( 45, 45, 45), trackerColour(200,200,200)

hslider bounds( 10, 86,590, 40), channel("microphone"), range(0,1.00,0.5), text("Mic. Position"),   textBox(1), valueTextBox(1), colour(100,100,100), trackerColour(200,200,200)
hslider bounds( 10,111,590, 40), channel("source"),     range(0,1.00,0.5), text("Source Position"), textBox(1), valueTextBox(1), colour(100,100,100), trackerColour(200,200,200)

label    bounds( 7, 155,120,18), text("D O P P L E R"),  fontColour(100,100,100)
label    bounds( 5, 153,120,18), text("D O P P L E R"),  fontColour(180,180,180)

label   bounds(232,160, 165, 12), text("Author: Iain McCurdy |2013|"), FontColour("grey")

</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-dm0 -n -+rtmidi=NULL -M0
</CsOptions>

<CsInstruments>

; sr set by host
ksmps  = 32 ; NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls = 2  ; NUMBER OF CHANNELS (2=STEREO)
0dbfs  = 1  ; MAXIMUM AMPLITUDE VALUE

;AMPLITUDE SCALING CURVE
giampcurve  ftgen    0,0,131072,5,0.01,131072*0.5,1,131072*0.5,0.01

instr    1
gkinput          chnget    "input"  
gkRoomSize       chnget    "RoomSize"  
gkspeed          chnget    "speed"     
gkdepth          chnget    "depth"     
gkfiltercutoff   chnget    "filtercutoff"
gkfiltercutoff   init      6
gkampscale       chnget    "ampscale"
gkPanDep         chnget    "PanDep"
kmix             chnget    "mix"                ; READ IN DRY/WET CROSSFADER WIDGET
gkOutAmp         chnget    "OutAmp"
gkmicrophone     chnget    "microphone"
gkshape          chnget    "shape"
gkshape          init      1

/* INPUT */
aL,aR        ins
if gkinput=1 then
 asig    =    aL
elseif gkinput=2 then
 asig    =    aR
elseif gkinput=3 then
 asig    =    (aL+aR)*0.677
else
  ;INPUT TONE=============================================(for testing)
 asig            vco2      0.5, 300        ;GENERATE TONE
 asig            tone      asig, 1200    ;LOW PASS FILTER TO SOFTEN THE TONE
 ;=======================================================
endif     


;LFO(modulates source position)=========================
ktrig            changed   gkshape                ; IF I-RATE VARIABLE SLIDER IS CHANGED GENERATE A '1'
if ktrig=1 then                                   ; IF TRIGGER IS '1'...
    reinit RESTART_LFO                            ; BEGIN A REINITIALISATION PASS FROM LABEL 'UPDATE' 
endif                                             ; END OF THIS CONDITIONAL BRANCH
RESTART_LFO:                                      ; LABEL CALLED 'UPDATE'
if i(gkshape)=3 then                              ; IF 'RANDOM' SHAPE IS SELECTED...
 gksource       rspline    0.5-gkdepth, 0.5+gkdepth, gkspeed,gkspeed*2
elseif i(gkshape)=4 then                          ; IF 'MANUAL' SHAPE IS SELECTED...
 gksource       chnget     "source"               ; READ SOURCE POSITION FROM SLIDER 
else
 gksource       lfo        gkdepth, gkspeed, i(gkshape)-1 ; LFO
 gksource       =          gksource + 0.5         ; OFFSET INTO THE POSITIVE DOMAIN
endif                                                
                rireturn
                chnset     gksource,"source"
;======================================================

kporttime       linseg     0, 0.001, 0.1          ; RAMPING UP PORTAMENTO TIME VARIABLE    

;DOPPLER================================================
ispeedofsound   init       340.29                 ; SPEED OF SOUND DEFINED
if gkshape==4   then
 ksource        portk      gksource,kporttime     ; SMOOTH SOURCE POSITION MOVEMENT
else
 ksource        =          gksource
endif
kmicrophone     portk      gkmicrophone, kporttime                 ; SMOOTH MICROPHOPNE POSITION MOVEMENT
ktrig           changed    gkfiltercutoff                          ; IF I-RATE VARIABLE SLIDER IS CHANGED GENERATE A '1'
if ktrig=1 then                                                    ; IF TRIGGER IS '1'...
    reinit UPDATE                                                  ; BEGIN A REINITIALISATION PASS FROM LABEL 'UPDATE' 
endif                                                              ; END OF THIS CONDITIONAL BRANCH
UPDATE:                                                            ; LABEL CALLED 'UPDATE'
kdisp           limit      ksource-(kmicrophone-0.5), 0, 1         ; CALCULATE DISPLACEMENT (DISTANCE) BETWEEN SOURCE AND MICROPHONE AND LIMIT VALUE TO LIE BETWEEN ZERO AND 1
kamp            table      kdisp, giampcurve,1                     ; READ AMPLITUDE SCALING VALUE FROM TABLE
kamp            ntrpol     1, kamp, gkampscale                     ; CALCULATE AMOUNT OF AMPLITUDE SCALING DESIRED BY THE USER FROM THE ON SCREEN SLIDER
aout            doppler    asig*kamp, ksource*gkRoomSize, kmicrophone*gkRoomSize, ispeedofsound, i(gkfiltercutoff)    ; APPLY DOPPLER EFFECT
rireturn                                                           ; RETURN FROM REINITIALISATION PASS
kpan            =          (gksource<gkmicrophone?0.5+gkPanDep:0.5-gkPanDep)    ; CALCULATE PAN VALUE ACCORDING TO SOURCE AND MIC POSITION
kpan            portk      kpan, kporttime                         ; APPLY PORTAMENTO SMOOTHING TO PAN POSITION VALUE 
aL              ntrpol     asig,aout*sqrt(kpan)*gkOutAmp,kmix      ; DRY/WET MIX LEFT CHANNEL
aR              ntrpol     asig,aout*sqrt(1-kpan)*gkOutAmp,kmix    ; DRY/WET MIX RIGHT CHANNEL
                outs       aL, aR                                  ; SEND AUDIO TO OUTPUTS AND APPLY PANNING
endin

</CsInstruments>

<CsScore>
i 1 0 3600    ;DUMMY SCORE EVENT - PERMITS REAL-TIME PERFORMANCE FOR 1 HOUR
</CsScore>

</CsoundSynthesizer>