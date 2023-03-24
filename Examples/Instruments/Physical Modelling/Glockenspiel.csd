
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; Glockenspiel (physical model)
; Iain McCurdy, 2022

; CONTROLS

 ; M A L L E T
 ; Hardness     - hardness of the intial strike. Equivalent to using beaters of different hardness
 ; Hold Damping - As this is increased, the result is as if the beater is held against the note, dampening its resonance
 ;                 As the control is increased further, this is as if the beater is held more firmly against the note, increasing the rate and amount of dampening./
 
 ; B A R S
 ; Sustain      - sustain time of the resonance
 ; Release      - dampening that is applied when a note is released. At its maximum, no dampening is applied upon note release.
 ; Level        - output level.

<Cabbage>
form caption("Glockenspiel") size(485, 235), colour(20,20,30), pluginId("Glck"), guiMode("queue")

#define RSliderStyle trackerInsideRadius(.8), trackerOutsideRadius(1), textColour(50,50,50), trackerColour(200,200,150)

image   bounds(  5,  5,190,125), corners(15), outlineColour(50,50,50), outlineThickness(1) shape("square"), colour(255,255,255,100), plant("Mallet") 
{
label   bounds(  0,  4,190, 15), align("centre"), colour(0,0,0,0), fontColour(50,50,50), text("M  A  L  L  E  T")
rslider bounds(  5, 30, 90, 90) range(0, 1, 0.75, 1, 0.001), channel("MHard"), text("Hardness"), $RSliderStyle  
rslider bounds( 95, 30, 90, 90) range(0, 1, 0, 1, 0.001), channel("MDamp"), text("Hold Damping"), $RSliderStyle
}

image   bounds(200, 5, 280,125), corners(15), outlineColour(50,50,50), outlineThickness(1) shape("square"), colour(255,255,255,100), plant("Bars") 
{
label   bounds(  0,  4,280, 15), align("centre"), colour(0,0,0,0), fontColour(50,50,50), text("B  A  R  S")
rslider bounds(  5, 30, 90, 90) range(-4, 4, 0, 1, 0.001), channel("BSus"), text("Sustain"), $RSliderStyle
rslider bounds( 95, 30, 90, 90) range(-8, 0, 0, 1, 0.001), channel("BRel"), text("Release"), $RSliderStyle
rslider bounds(185, 30, 90, 90) range(0, 2, 1, 0.5, 0.001), channel("BLev"), text("Level"), $RSliderStyle
}

keyboard bounds(  5,135,475, 95), keypressBaseOctave(8),  whiteNoteColour("Silver"), blackNoteColour("Silver"), keyWidth(20), value(84),  mouseOverKeyColour(80,80,80),

</Cabbage>
<CsoundSynthesizer>

<CsOptions>
-n -d -+rtmidi=NULL -M0 -m0d --midi-key-cps=4 --midi-velocity-amp=5
</CsOptions>

<CsInstruments>
; Initialize the global variables. 
ksmps = 32
nchnls = 2
0dbfs = 1

massign 0, 1

; function tables containing modal ratios, Q values and dB values
; Must be in this order: Ratios, Qs, Amps(dBs)
; Must all contain the same number of members
giGlockenspielRatios ftgen  0, 0, 0, -2, 1567.982/1567.982, 4400/1567.982, 6430/1567.982, 8370/1567.982, 3140/1567.982, 10420/1567.982, 12420/1567.982, 13840/1567.982, 17025/1567.982, 20515/1567.982
giGlockenspielQs     ftgen  0, 0, 0, -2, 3000, 600, 1700, 1000, 2000, 800, 4000, 2000, 3300 ,3000
giGlockenspieldBs    ftgen  0, 0, 0, -2, -10, -19, -27, -8, -37, -31, -35, -23, -28, -27

giTestRatios ftgen  0, 0, 0, -2, 1
giTestQs     ftgen  0, 0, 0, -2, 1000
giTestdBs    ftgen  0, 0, 0, -2, -6

;giRndPitches f # time size 21 type level [arg1  [arg2]]
giRndRatios ftgen 0, 0, 1028576, 21, 6, 1
giNotesActive[] init 128

; UDO that defines a single mode partial (recursive according to members in function table)
opcode ModePartial,a,aikio
aImp, iCPS, kSus, iRatios, iCount xin                ; read in variables
iRandOS  =         0.01                             ; random offset ratio of partials to add a bit of variation
iCPSL    =         iCPS * table:i(iCount, iRatios) * ( octave( table(iCount+int(iCPS),giRndRatios) *iRandOS) )  ; derive freq value for this partial
iQL      =         table:i(iCount, iRatios + 1) * 2  ; derive Q value for this partial
idBL     =         table:i(iCount, iRatios + 2)      ; derive dB value for this partial

kRMS     init      1
if kRMS>0.00001 then
aMode    mode      aImp*ampdbfs(idBL), iCPSL, iQL * kSus ; create mode filter
kRMS     rms       aMode
else
aMode = 0
endif

if iCount < ftlen(iRatios) then                          ; if more partials are needed...
 aMix    ModePartial aImp, iCPS, kSus, iRatios, iCount+1 ; conditionally call UDO again
endif
         xout      aMode + aMix                          ; mix subsequent partials with partials form this recursion and send back to caller instrument
endop

instr 1
iCPS      cpsmidi                       ; read in MIDI note pitch in cycles per second 
iVel      ampmidi 1                     ; read in MIDI velocity
iNote     notnum

kBSus     cabbageGetValue "BSus"        ; bars sustain
kBRel     cabbageGetValue "BRel"        ; bars release
kBLev     cabbageGetValue "BLev"        ; bars level

; IMPULSE SOUND
kMHard    cabbageGetValue "MHard"                                ; mallet hardess (controls the cutoff frequency of a lowpass filter)
iMDamp    cabbageGetValue "MDamp"                                ; mallet damping as the mallet is held on the note
kEnv      expon           1, 0.001, 0.001                        ; a quickly decaying amplitude envelope for the impulse sound
aImp      rand            kEnv                                   ; create some noise
aImp      buthp           aImp, iCPS                             ; remove some low frequencies
aImp      butlp           aImp, cpsoct(6 + 8 * kMHard * iVel^3)  ; remove high frequencies according to the note velocity 

; HOLDING A STRIKE WILL CAUSE A DAMPED HIT
if iMDamp>0 then
 kQ       expon           1, 1.02 - iMDamp, 0.01
else
 kQ       =               1
endif

if release:k()==1 then
 kQ       =               1
 iRel     limit           10*1567.982/iCPS, 1, 30
 aRel     linsegr         1, iRel, 0
else
 aRel     =               1
endif

if release:k()==1 then
 kQ       *=              octave(kBRel)
endif

kPortTime linseg          0,0.001,0.02
kQ        portk           kQ, kPortTime

; call mode partials... (a recursive UDO)
aMix      ModePartial     aImp, iCPS, kQ*octave(kBSus), giGlockenspielRatios
aMix      *=              aRel
aMix      *=             .3 * kBLev

          outs           aMix, aMix
endin

</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
f 0 z
</CsScore>
</CsoundSynthesizer>