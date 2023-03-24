
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; Each pad triggers a different synthesised sound based on the Roland 808

; Horizontal position across the pad controls the pan location. This is grabbed at the beginning of the note only.
; Vertical position up the pad controls the velocity (amplitude). This is grabbed at the beginning of the note only.

<Cabbage>
form caption("Drum Pads") size(850, 430), pluginId("DPds"), colour("Black")
image bounds( 10, 10, 200, 200),     colour(150,150,170), shape("rounded"), outlineColour("white"), line(0)
image bounds(220, 10, 200, 200),     colour(150,150,170), shape("rounded"), outlineColour("white"), line(0)
image bounds(430, 10, 200, 200),     colour(150,150,170), shape("rounded"), outlineColour("white"), line(0)
image bounds(640, 10, 200, 200),     colour(150,150,170), shape("rounded"), outlineColour("white"), line(0)
image bounds( 10,220, 200, 200),     colour(150,150,170), shape("rounded"), outlineColour("white"), line(0)
image bounds(220,220, 200, 200),     colour(150,150,170), shape("rounded"), outlineColour("white"), line(0)
image bounds(430,220, 200, 200),     colour(150,150,170), shape("rounded"), outlineColour("white"), line(0)
image bounds(640,220, 200, 200),     colour(150,150,170), shape("rounded"), outlineColour("white"), line(0)
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-n -dm0
</CsOptions>

<CsInstruments>

; sr set by host
ksmps  = 16
nchnls = 2
0dbfs  = 1

gisine       ftgen    0,0,1024,10,1        ; A SINE WAVE
gicos        ftgen    0,0,65536,9,1,1,90    ; A COSINE WAVE

instr    1

 gkMOUSE_X            chnget    "MOUSE_X"
 gkMOUSE_Y            chnget    "MOUSE_Y"
 gkMOUSE_DOWN_LEFT    chnget    "MOUSE_DOWN_LEFT"
#define PAD(N'X'Y'INSTR)
 #
 ix$N       =           $X
 iy$N       =           $Y
 if (gkMOUSE_X > ix$N)  && (gkMOUSE_X < (ix$N+200))  && (gkMOUSE_Y > iy$N)  && (gkMOUSE_Y < (iy$N+200)) then
  kinside$N =           1
 else
  kinside$N =           0
 endif

 if kinside$N = 1 then
  gkx       =           (gkMOUSE_X - ix$N) / 200    ; normalised coordinates
  gky       =           (gkMOUSE_Y - iy$N) / 200
  gky       expcurve    1-gky,8
  ktrig     trigger     gkMOUSE_DOWN_LEFT,0.5,0
            schedkwhen  ktrig,0,0,$INSTR,0,0.01,gkx,gky
 endif
 #

$PAD(1'10'10'101)
$PAD(2'220'10'102)
$PAD(3'430'10'103)
$PAD(4'640'10'104)
$PAD(5'10'220'105)
$PAD(6'220'220'106)
$PAD(7'430'220'107)
$PAD(8'640'220'108)
endin


instr    101    ;BASS DRUM
p3    =    2                                ;NOTE DURATION. SCALED USING GUI 'Decay' KNOB

;SUSTAIN AND BODY OF THE SOUND
kmul    transeg    0.2,p3*0.5,-15,0.01, p3*0.5,0,0                    ;PARTIAL STRENGTHS MULTIPLIER USED BY GBUZZ. DECAYS FROM A SOUND WITH OVERTONES TO A SINE TONE.
kbend   transeg    0.5,1.2,-4, 0,1,0,0                        ;SLIGHT PITCH BEND AT THE START OF THE NOTE 
asig    gbuzz      0.5,50*semitone(kbend),20,1,kmul,gicos                ;GBUZZ TONE
aenv    transeg    1,p3-0.004,-6,0                            ;AMPLITUDE ENVELOPE FOR SUSTAIN OF THE SOUND
aatt    linseg     0,0.004,1                            ;SOFT ATTACK
asig    =          asig*aenv*aatt

;HARD, SHORT ATTACK OF THE SOUND
aenv    linseg     1,0.07,0                            ;AMPLITUDE ENVELOPE (FAST DECAY)                        
acps    expsega    400,0.07,0.001,1,0.001                        ;FREQUENCY OF THE ATTACK SOUND. QUICKLY GLISSES FROM 400 Hz TO SUB-AUDIO
aimp    oscili     aenv,acps,gisine                        ;CREATE ATTACK SOUND

amix    =          ((asig*0.5)+(aimp*0.35))*p5            ;MIX SUSTAIN AND ATTACK SOUND ELEMENTS AND SCALE USING GUI 'Level' KNOB

aL,aR   pan2       amix,p4                            ;PAN THE MONOPHONIC SOUND
        outs       aL,aR                                ;SEND AUDIO TO OUTPUTS
endin

instr    102    ;SNARE DRUM
;SOUND CONSISTS OF TWO SINE TONES, AN OCTAVE APART AND A NOISE SIGNAL
ifrq      =    342        ;FREQUENCY OF THE TONES
iNseDur    =    0.3        ;DURATION OF THE NOISE COMPONENT
iPchDur    =    0.1        ;DURATION OF THE SINE TONES COMPONENT
p3    =    iNseDur     ;p3 DURATION TAKEN FROM NOISE COMPONENT DURATION (ALWATS THE LONGEST COMPONENT)

;SINE TONES COMPONENT
aenv1    expseg    1,iPchDur,0.0001,p3-iPchDur,0.0001        ;AMPLITUDE ENVELOPE
apitch1    oscili    1,ifrq,gisine            ;SINE TONE 1
apitch2    oscili    0.25,ifrq*0.5,gisine        ;SINE TONE 2 (AN OCTAVE LOWER)
apitch    =    (apitch1+apitch2)*0.75                ;MIX THE TWO SINE TONES

;NOISE COMPONENT
aenv2    expon    1,p3,0.0005                    ;AMPLITUDE ENVELOPE
anoise    noise    0.75,0                        ;CREATE SOME NOISE
anoise    butbp    anoise,10000,10000        ;BANDPASS FILTER THE NOISE SIGNAL
anoise    buthp    anoise,1000                    ;HIGHPASS FILTER THE NOISE SIGNAL
kcf    expseg    5000,0.1,3000,p3-0.2,3000            ;CUTOFF FREQUENCY FOR A LOWPASS FILTER
anoise    butlp    anoise,kcf                    ;LOWPASS FILTER THE NOISE SIGNAL
amix    =    ((apitch*aenv1)+(anoise*aenv2))*p5        ;MIX AUDIO SIGNALS AND SCALE ACCORDING TO GUI 'Level' CONTROL
aL,aR    pan2    amix,p4                        ;PAN THE MONOPHONIC AUDIO SIGNAL
    outs    aL,aR                        ;SEND AUDIO TO OUTPUTS
endin

instr    103    ;OPEN HIGH HAT
kFrq1    =    296     ;FREQUENCIES OF THE 6 OSCILLATORS
kFrq2    =    285     
kFrq3    =    365     
kFrq4    =    348     
kFrq5    =    420     
kFrq6    =    835     
p3    =    0.5            ; DURATION OF THE NOTE

; SOUND CONSISTS OF 6 PULSE OSCILLATORS MIXED WITH A NOISE COMPONENT
; PITCHED ELEMENT
aenv    linseg   1,p3-0.05,0.1,0.05,0   ; AMPLITUDE ENVELOPE FOR THE PULSE OSCILLATORS
ipw     =        0.25                   ; PULSE WIDTH
a1      vco2     0.5,kFrq1,2,ipw        ; PULSE OSCILLATORS...
a2      vco2     0.5,kFrq2,2,ipw
a3      vco2     0.5,kFrq3,2,ipw
a4      vco2     0.5,kFrq4,2,ipw
a5      vco2     0.5,kFrq5,2,ipw
a6      vco2     0.5,kFrq6,2,ipw
amix    sum      a1,a2,a3,a4,a5,a6      ; MIX THE PULSE OSCILLATORS
amix    reson    amix,5000,5000,1       ; BANDPASS FILTER THE MIXTURE
amix    buthp    amix,5000              ; HIGHPASS FILTER THE SOUND...
amix    buthp    amix,5000              ; ...AND AGAIN
amix    =        amix*aenv              ; APPLY THE AMPLITUDE ENVELOPE

;NOISE ELEMENT
anoise  noise    0.8,0                  ; GENERATE SOME WHITE NOISE
aenv    linseg   1,p3-0.05,0.1,0.05,0   ; CREATE AN AMPLITUDE ENVELOPE
kcf     expseg   20000,0.7,9000,p3-0.1,9000    ; CREATE A CUTOFF FREQ. ENVELOPE
anoise  butlp    anoise,kcf             ; LOWPASS FILTER THE NOISE SIGNAL
anoise  buthp    anoise,8000            ; HIGHPASS FILTER THE NOISE SIGNAL
anoise  =        anoise*aenv            ; APPLY THE AMPLITUDE ENVELOPE

;MIX PULSE OSCILLATOR AND NOISE COMPONENTS
amix    =         (amix+anoise)*p5*0.55
aL,aR   pan2      amix,p4               ; PAN MONOPHONIC SIGNAL
        outs      aL,aR                 ; SEND TO OUTPUTS
kactive    active p1+1                  ; CHECK NUMBER OF ACTIVE INSTANCES OF CLOSED HIGH HAT INSTRUMENT
if kactive>0 then                       ; IF HIGH-HAT CLOSED IS ACTIVE...
        turnoff                         ; TURN OFF THIS INSTRUMENT
endif
endin

instr    104    ; CLOSED HIGH HAT
kFrq1     =        296             ; FREQUENCIES OF THE 6 OSCILLATORS
kFrq2     =        285     
kFrq3     =        365     
kFrq4     =        348     
kFrq5     =        420     
kFrq6     =        835     
idur      =        0.088           ; DURATION OF THE NOTE
p3        =        idur

iactive    active    p1-1            ;SENSE ACTIVITY OF PREVIOUS INSTRUMENT (OPEN HIGH HAT) 
if iactive>0 then            ;IF 'OPEN HIGH HAT' IS ACTIVE...
 turnoff2    p1-1,0,0        ;TURN IT OFF (CLOSED HIGH HAT TAKES PRESIDENCE)
endif

;PITCHED ELEMENT
aenv      expsega  1,idur,0.001,1,0.001       ;AMPLITUDE ENVELOPE FOR THE PULSE OSCILLATORS
ipw       =        0.25                       ;PULSE WIDTH
a1        vco2     0.5,kFrq1,2,ipw            ;PULSE OSCILLATORS...            
a2        vco2     0.5,kFrq2,2,ipw
a3        vco2     0.5,kFrq3,2,ipw
a4        vco2     0.5,kFrq4,2,ipw
a5        vco2     0.5,kFrq5,2,ipw
a6        vco2     0.5,kFrq6,2,ipw
amix      sum      a1,a2,a3,a4,a5,a6               ; MIX THE PULSE OSCILLATORS
amix      reson    amix,5000,5000,1                ; BANDPASS FILTER THE MIXTURE
amix      buthp    amix,5000                       ; HIGHPASS FILTER THE SOUND...
amix      buthp    amix,5000                       ; ...AND AGAIN
amix      =        amix*aenv                       ; APPLY THE AMPLITUDE ENVELOPE

;NOISE ELEMENT
anoise    noise    0.8,0                         ; GENERATE SOME WHITE NOISE
aenv      expsega  1,idur,0.001,1,0.001          ; CREATE AN AMPLITUDE ENVELOPE
kcf       expseg   20000,0.7,9000,idur-0.1,9000  ; CREATE A CUTOFF FREQ. ENVELOPE
anoise    butlp    anoise,kcf                    ; LOWPASS FILTER THE NOISE SIGNAL
anoise    buthp    anoise,8000                   ; HIGHPASS FILTER THE NOISE SIGNAL
anoise    =        anoise*aenv                   ; APPLY THE AMPLITUDE ENVELOPE

;MIX PULSE OSCILLATOR AND NOISE COMPONENTS
amix      =        (amix+anoise)*p5*0.55
aL,aR     pan2     amix,p4            ;PAN MONOPHONIC SIGNAL
          outs     aL,aR                ;SEND TO OUTPUTS
endin

instr    105    ;HIGH TOM
ifrq       =       200                           ; FREQUENCY
p3         =       0.5                           ; DURATION OF THIS NOTE

;SINE TONE SIGNAL
aAmpEnv   transeg  1,p3,-10,0.001                ; AMPLITUDE ENVELOPE FOR SINE TONE SIGNAL
afmod     expsega  5,0.125/ifrq,1,1,1            ; FREQUENCY MODULATION ENVELOPE. GIVES THE TONE MORE OF AN ATTACK.
asig      oscili   -aAmpEnv*0.6,ifrq*afmod,gisine ; SINE TONE SIGNAL

;NOISE SIGNAL
aEnvNse   transeg  1,p3,-6,0.001          ; AMPLITUDE ENVELOPE FOR NOISE SIGNAL
anoise    dust2    0.4, 8000                ; GENERATE NOISE SIGNAL
anoise    reson    anoise,400,800,1         ; BANDPASS FILTER THE NOISE SIGNAL
anoise    buthp    anoise,100               ; HIGHPASS FILTER THE NOSIE SIGNAL
anoise    butlp    anoise,1000              ; LOWPASS FILTER THE NOISE SIGNAL
anoise    =        anoise * aEnvNse         ; SCALE NOISE SIGNAL WITH AMPLITUDE ENVELOPE

;MIX THE TWO SOUND COMPONENTS
amix      =        (asig + anoise)*p5
aL,aR     pan2     amix,p4                  ; PAN MONOPHONIC SIGNAL
          outs     aL,aR                    ; SEND AUDIO TO OUTPUTS
endin

instr    106    ;MID TOM
ifrq      =        133                             ; FREQUENCY
p3        =        0.6                             ; DURATION OF THIS NOTE

;SINE TONE SIGNAL
aAmpEnv   transeg  1,p3,-10,0.001                  ; AMPLITUDE ENVELOPE FOR SINE TONE SIGNAL
afmod     expsega  5,0.125/ifrq,1,1,1              ; FREQUENCY MODULATION ENVELOPE. GIVES THE TONE MORE OF AN ATTACK.
asig      oscili   -aAmpEnv*0.6,ifrq*afmod,gisine  ; SINE TONE SIGNAL

;NOISE SIGNAL
aEnvNse   transeg  1,p3,-6,0.001        ; AMPLITUDE ENVELOPE FOR NOISE SIGNAL
anoise    dust2    0.4, 8000               ; GENERATE NOISE SIGNAL
anoise    reson    anoise, 400,800,1       ; BANDPASS FILTER THE NOISE SIGNAL
anoise    buthp    anoise,100              ; HIGHPASS FILTER THE NOSIE SIGNAL
anoise    butlp    anoise,600              ; LOWPASS FILTER THE NOISE SIGNAL
anoise    =    anoise * aEnvNse            ; SCALE NOISE SIGNAL WITH AMPLITUDE ENVELOPE

;MIX THE TWO SOUND COMPONENTS
amix    =    (asig + anoise)*p5
aL,aR    pan2    amix,p4                  ; PAN MONOPHONIC SIGNAL
    outs    aL,aR                         ; SEND AUDIO TO OUTPUTS
endin

instr    107    ;LOW TOM
ifrq         =    90                              ; FREQUENCY
p3          =    0.7                              ; DURATION OF THIS NOTE

;SINE TONE SIGNAL
aAmpEnv    transeg    1,p3,-10,0.001              ; AMPLITUDE ENVELOPE FOR SINE TONE SIGNAL
afmod    expsega    5,0.125/ifrq,1,1,1            ; FREQUENCY MODULATION ENVELOPE. GIVES THE TONE MORE OF AN ATTACK.
asig    oscili    -aAmpEnv*0.6,ifrq*afmod,gisine  ; SINE TONE SIGNAL

;NOISE SIGNAL
aEnvNse   transeg  1,p3,-6,0.001         ; AMPLITUDE ENVELOPE FOR NOISE SIGNAL
anoise    dust2    0.4, 8000             ; GENERATE NOISE SIGNAL
anoise    reson    anoise,40,800,1       ; BANDPASS FILTER THE NOISE SIGNAL
anoise    buthp    anoise,100            ; HIGHPASS FILTER THE NOSIE SIGNAL
anoise    butlp    anoise,600            ; LOWPASS FILTER THE NOISE SIGNAL
anoise    =    anoise * aEnvNse          ; SCALE NOISE SIGNAL WITH AMPLITUDE ENVELOPE

;MIX THE TWO SOUND COMPONENTS
amix    =    (asig + anoise)*p5
aL,aR    pan2    amix,p4                 ; PAN MONOPHONIC SIGNAL
    outs    aL,aR                        ; SEND AUDIO TO OUTPUTS
endin

instr    108    ;CYMBAL
kFrq1    =      296                      ; FREQUENCIES OF THE 6 OSCILLATORS
kFrq2    =      285
kFrq3    =      365
kFrq4    =      348     
kFrq5    =      420
kFrq6    =      835
p3       =      2                        ; DURATION OF THE NOTE

;SOUND CONSISTS OF 6 PULSE OSCILLATORS MIXED WITH A NOISE COMPONENT
;PITCHED ELEMENT
aenv    expon   1,p3,0.0001              ; AMPLITUDE ENVELOPE FOR THE PULSE OSCILLATORS 
ipw     =       0.25                     ; PULSE WIDTH      
a1      vco2    0.5,kFrq1,2,ipw          ; PULSE OSCILLATORS...  
a2      vco2    0.5,kFrq2,2,ipw
a3      vco2    0.5,kFrq3,2,ipw
a4      vco2    0.5,kFrq4,2,ipw
a5      vco2    0.5,kFrq5,2,ipw                                                                   
a6      vco2    0.5,kFrq6,2,ipw
amix    sum     a1,a2,a3,a4,a5,a6        ; MIX THE PULSE OSCILLATORS
amix    reson   amix,5000,5000,1         ; BANDPASS FILTER THE MIXTURE
amix    buthp   amix,10000               ; HIGHPASS FILTER THE SOUND
amix    butlp   amix,12000               ; LOWPASS FILTER THE SOUND...
amix    butlp   amix,12000               ; AND AGAIN...
amix    =       amix*aenv                ; APPLY THE AMPLITUDE ENVELOPE

;NOISE ELEMENT
anoise  noise   0.8,0                      ; GENERATE SOME WHITE NOISE
aenv    expsega 1,0.3,0.07,p3-0.1,0.00001  ; CREATE AN AMPLITUDE ENVELOPE
kcf     expseg  14000,0.7,7000,p3-0.1,5000 ; CREATE A CUTOFF FREQ. ENVELOPE
anoise  butlp   anoise,kcf                 ; LOWPASS FILTER THE NOISE SIGNAL
anoise  buthp   anoise,8000                ; HIGHPASS FILTER THE NOISE SIGNAL
anoise  =       anoise*aenv                ; APPLY THE AMPLITUDE ENVELOPE            

;MIX PULSE OSCILLATOR AND NOISE COMPONENTS
amix    =       (amix+anoise)*p5*0.85
aL,aR   pan2    amix,p4                    ; PAN MONOPHONIC SIGNAL
        outs    aL,aR                      ; SEND TO OUTPUTS
endin

</CsInstruments>

<CsScore>
i 1 0 3600    ;INSTRUMENT 1 PLAYS A NOTE FOR 1 HOUR (AND KEEPS REAL-TIME PERFORMANCE GOING)
</CsScore>

</CsoundSynthesizer>