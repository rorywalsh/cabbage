
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; LiveLooper.csd
; Written by Iain McCurdy 2015

; A simple instrument for recording a sample and then playing it back as a loop with a crossfade employed to smooth the join between the beginning and the end of the loop  (flooper2 opcode).
; Maximum loop time is about a minute and a half at sr = 44100

; The crossfade curve shape and its duration can be modified to suit the material being looped 
; The curve shape is based on a fragment of a sine curve 
; If a dip in power is heard at the crossfade point, 'Shape' should be lowered. Conversely, if a hump in power is heard, 'Shape' should be raised.

; The loop can also be played back using the MIDI keyboard (or an external keyboard) in which case the loop will be transposed according to the note played. 
; Unison will be middle C (NN#60). 

; NB. Loop Mode = "Fwd/Bwd" does not seem to crossfade properly when fading from the backwards pass to the forwards pass. Maybe a bug in flooper2?

<Cabbage>
form size(385,325), caption("Live Looper"), guiRefresh(32), pluginId("LiLp"), colour("Black")
image    bounds(  5,  5,375,110), colour(0,0,0,0), outlineColour("white"), outlineThickness(1), plant("Crossfade"), shape("sharp") {
label    bounds(  0,  4,375, 15), text("Crossfade")
rslider  bounds(  5, 27, 70, 70), range(0.25,2,0.5,0.5), channel("Shape"), text("Shape")
rslider  bounds( 75, 27, 70, 70), range(0.01, 4,1), channel("CFDuration"), text("Duration")
gentable bounds(155, 27,200, 75), tableNumber(1,2), identChannel("table"), fill(0), ampRange(0,1,-1), zoom(-1), tableGridColour(0,0,0,0), tableBackgroundColour(200,200,200), outlineThickness(2), tableColour:0(10,10,10), tableColour:1(10,10,10)
}

image     bounds(  5,120,190,120), colour(0,0,0,0), outlineColour("white"), outlineThickness(1), plant("Loop"), shape("sharp") {
label     bounds(  0,  4,190, 15), text("Loop")
button    bounds( 15, 25, 60, 25), fontColour:0(50,50,50), fontColour:1(205,205,235), colour:0(10,0,0), colour:1(0,0,130), text("Stop","Stop"),     channel("Stop"),   latched(1), radioGroup(1), value(1)
button    bounds( 15, 55, 60, 25), fontColour:0(50,50,50), fontColour:1(255,205,205), colour:0(10,0,0), colour:1(150,0,0), text("Record","Record"), channel("Record"), latched(1), radioGroup(1)
button    bounds( 15, 85, 60, 25), fontColour:0(50,50,50), fontColour:1(205,255,205), colour:0(0,10,0), colour:1(0,150,0), text("Play","Play"),     channel("Play"),   latched(1), radioGroup(1)
nslider   bounds( 90, 25, 85, 35), range(0, 100000,0), channel("LoopDuration"), text("Loop Duration")
label     bounds( 90, 68, 85, 14), text("Loop Mode")
combobox  bounds( 90, 84, 85, 20), items("Fwd","Bwd","Fwd/Bwd"), value(1), channel("LoopMode")
}

image     bounds(200,120,180,120), colour(0,0,0,0), outlineColour("white"), outlineThickness(1), plant("Mixer"), shape("sharp") {
label     bounds(  0,  4,180, 15), text("Mixer")
rslider   bounds( 10, 30, 80, 80), range(0, 1,1), channel("DryLevel"), text("Dry Level")
rslider   bounds( 90, 30, 80, 80), range(0, 1,1), channel("LoopLevel"), text("Loop Level")
}

keyboard  bounds( 0, 245,385, 80)
</Cabbage>

<CsoundSynthesizer>

<CsOptions>   
-dm0 -n -+rtmidi=NULL -M0
</CsOptions>

<CsInstruments>

; sr set by host
ksmps = 16
nchnls = 2
0dbfs = 1
massign 0,3

giCF_Curve   ftgen    1,0,8193,-7,0,8192,1    ; CROSSFADE CURVE
giCF_Curve2  ftgen    2,0,8193,-7,1,8192,0    ; CROSSFADE CURVE
giBufferL    ftgen    3,0,1048576*4,2,0       ; AUDIO BUFFER (LEFT CHANNEL)
giBufferR    ftgen    4,0,1048576*4,2,0       ; AUDIO BUFFER (RIGHT CHANNEL)
gitri        ftgen    0,0,4096,-7,0,2048,1,2048,0

gkLoopDuration init   0                       ; LOOP DURATION (WILL BE UPDATED WHILE RECORDING)
              
instr 1
 gaL,gaR    ins                ; READ AUDIO INPUT
 ;gaL    poscil    0.2,440
 ;gaR    =    gaL

 kShape         chnget   "Shape"                 ; CURVE SHAPE
 kShape         init     0.5                     ; CURVE INITIAL VALUE (TO AVOID AN INITIAL ZERO)
 gkCFDuration   chnget   "CFDuration"            ; CROSSFADE DURATION
 kPortTime      linseg   0,0.01,0.05             ; RAMPING UP PORTAMENTO TIME
 gkDryLevel     chnget   "DryLevel"              ; DRY (LIVE) LEVEL
 gkLoopLevel    chnget   "LoopLevel"             ; LOOPER LEVEL
 gkDryLevel     portk    gkDryLevel,kPortTime    ; SMOOTH CHANGES TO DRY LEVEL VALUE
 gkLoopLevel    portk    gkLoopLevel,kPortTime   ; SMOOTH CHANGES TO LOOPER LEVEL
 gkLoopMode     chnget   "LoopMode"              ; READ IN LOOP MODE COMBOBOX WIDGET
 gkLoopMode     init     1                       ; INITIAL VALUE OF LOOP MODE (TO AVOID AN INITIAL ZERO)
 
 ktrig    changed    kShape                ; GENERATE A TRIGGER (TO REBUILD CROSSFADE FUNCTION) IF SHAPE CONTROL IS MOVED
 if ktrig==1 then                          ; IF A TRIGGER HAS BEEN GENERATED
  reinit RebuildCurve                      ; BEGIN A REINITIALISATION PASS FROM THE LABEL 'RebuildCurve'
 endif                                     ; END OF CONDITIONAL BRANCH
 RebuildCurve:                             ; LABEL. START REINITIALISATION FROM HERE
 icount    =        0                      ; COUNTER FIRST VALUE
 iftlen    =        ftlen(giCF_Curve)      ; FUNCTION TABLE LENGTH
 loop:                                     ; LOOP BEGINNING                                            
 ix        =        icount/(iftlen)                                  
 iy        =        ((sin((ix+1.5)*$M_PI)*0.5)+0.5)^i(kShape)    ; APPLY FORMULA TO DERIVE Y        
           tableiw  iy,icount,giCF_Curve                         ; WRITE Y VALUE TO TABLE                         
 iy        =        ((sin((ix+0.5)*$M_PI)*0.5)+0.5)^i(kShape)    ; APPLY FORMULA TO DERIVE Y        
           tableiw  iy,icount,giCF_Curve2                        ; **THE SECOND TABLE IS ESSENTIALLY FOR DISPLAY PURPOSES ONLY**
           loop_lt  icount,1,iftlen,loop                         ; 
 rireturn                                        ; RETURN FROM REINITIALISATION PASS
 if ktrig==1 then                                ; IF CHANGED SHAPE CONTROL TRIGGER HAS BEEN GENERATED...
           chnset   "tableNumbers(1,2)","table"  ; ...UPDATE CABBAGE TABLE DISPLAY
 endif                                           ; END OF CONDITIONAL BRANCH
 
 gkStop    chnget   "Stop"            ; READ IN TRANSPORT BUTTON WIDGETS
 gkRecord  chnget   "Record"          ;
 gkPlay    chnget   "Play"            ;

 if trigger(gkRecord,0.5,0)==1 then        ; IF RECORD BUTTON IS PRESSED...
           event    "i",2,0,0.1            ; ...START RECORD INSTRUMENT
 endif                                     ; END OF CONDITIONAL BRANCH

 if trigger(gkPlay,0.5,0)==1 then        ; IF PLAY BUTTON IS PRESSED...
           event    "i",3,0,-1           ; ...START PLAY INSTRUMENT WITH A HELD NOTE
 endif                                   ; END OF CONDITIONAL BRANCH

           outs     gaL*gkDryLevel, gaR*gkDryLevel    ; DRY (LIVE) OUTPUT
endin

instr    2    ; Record
 p3        =        (ftlen(giBufferL)-1)/sr   ; p3 (INSTRUMENT DURATION) LIMITED TO THE LENGTH OF THE FUNCTION TABLE
 aPtr      line     0,1,sr                    ; POINTER FOR WRITING INTO THE FUNCTION TABLE
 gkLoopDuration     line    0, 1, 1           ; LOOP DURATION        
           tablew   gaL,aPtr,giBufferL        ; WRITE LEFT CHANNEL TO TABLE
           tablew   gaR,aPtr,giBufferR        ; WRITE RIGHT CHANNEL TO TABLE
 if gkRecord!=1 then                ; IF RECORD BUTTON IS NO LONGER ACTIVE (I.E. STOP OR PLAY HAVE BEEN PRESSED)...
           turnoff                  ; ...TURN THIS INSTRUMENT OFF
 endif                              ; END OF CONDITIONAL BRANCH
 
 kLoopDuration =  int(gkLoopDuration*100) / 100         ; LOOP DURATION VALUE FOR PRINTING TO CABBAGE NUMBERBOX (QUANTISED TO NEAREST 0.01) 
 if changed(kLoopDuration)==1 then                      ; IF QUANTISED LOOP DURATION HAS CHANGED
           chnset   kLoopDuration,"LoopDuration"        ; SEND NEW VALUE TO NUMBERBOX
 endif                                                  ; END OF CONDITIONAL BRANCH
endin

instr    3    ; Play
 iMIDIActiveValue =            1        ; IF MIDI ACTIVATED
 iMIDIflag        =            0        ; IF BUTTON ACTIVATED
                  mididefault  iMIDIActiveValue, iMIDIflag    ; IF NOTE IS MIDI ACTIVATED REPLACE iMIDIflag WITH iMIDIActiveValue 

 if iMIDIflag==0 then                                    ; IF PLAYBACK HAS BEEN TRIGGERED BY THE GUI BUTTON...
  kpitch          =            1                         ; ...PITCH=1 (NO TRANSPOSITION)
 else                                                    ; OTHERWISE (TRIGGERED BY MIDI KEYBOARD)
  kpitch          =            cpsmidi()/cpsmidinn(60)   ; DERIVE RATIO BASED ON NOTE NUMBER 60 AS THE POINT OF UNISON (IE. RATIO=1) 
 endif                                                   ; END OF CONDITIONAL BRANCH
 
 kloopstart       =            0                             ; ALWAYS START LOOP FROM THE BEGINNING
 gkCFDuration    limit    gkCFDuration,0.01,gkLoopDuration/2 ; LIMIT CROSSFADE DURATION TO PREVENT GLITCHES IF CROSSFADE DURATION IS LONG AND RECORDING IS SHORT
 istart           =            0                             ; INITIAL START (BEFORE LOOPING)
 iskip            =            0                             ; INITIALISATION SKIPPING
 if changed(gkLoopMode)==1 then                              ; IF LOOP MODE HAS BEEN CHANGED...
  reinit UpdateFlooper                                       ; ...REINITIALISE FROM LABEL 'UpdateFlooper'
 endif                                                       ; END OF CONDITIONAL BRANCH
 kAntiClick       cossegr      0,0.1,1,36000,1,0.1,0         ; cossegr DOESN'T SEEM TO WORK PROPERLY AT THE MOMENT :-(
 UpdateFlooper:                                              ; LABEL. BEGIN REINITIALISATION PASS FROM HERE

 ; INSTANTIATE flooper2 TWICE. ONCE FOR EACH STEREO CHANNEL
 aL               flooper2     gkLoopLevel*kAntiClick, kpitch, kloopstart, gkLoopDuration, gkCFDuration, giBufferL, istart, i(gkLoopMode)-1, giCF_Curve, iskip
 aR               flooper2     gkLoopLevel*kAntiClick, kpitch, kloopstart, gkLoopDuration, gkCFDuration, giBufferR, istart, i(gkLoopMode)-1, giCF_Curve, iskip
    outs    aL,aR    ; SEND AUDIO TO OUTPUT
 
 if gkPlay!=1&&iMIDIflag==0 then        ; IF 'PLAY' BUTTON IS NOT ACTIVE - I.E. 'STOP' OR 'RECORD' ARE ACTIVE...
                  turnoff               ; TURN THIS INSTRUMENT OFF (PERMIT RELEASE STAGE OF linsegr)
 endif                                  ; END OF CONDITIONAL BRANCH
endin

</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]                
</CsScore>

</CsoundSynthesizer>