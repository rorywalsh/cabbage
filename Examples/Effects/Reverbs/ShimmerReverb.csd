
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; Shimmer Reverb
; Author: Iain McCurdy. 2021.

; A feedback delay network (FDN) reverb with a pitch shifter within the feedback loop.

; R E V E R B
; Pre Delay   - a delay applied to the signal before it enters the FDN reverb
; Smear       - amount of scattering of the signal entering the FDN reverb (this is carried out by a series of allpass filters)
; Reverb Time - the reverb time of the FDN reverb
; Damping     - the cutoff frequency of a lowpass filter (6dB/oct) applied to the signal in the feedback loop of the FDN reverb
; Mod.Rate    - rate of modulation of internal delays within the FDN reverb. This is heard in the blurring of frequencies as they are sustained within the reverb.
; Mod.Depth   - depth of modulation of internal delays within the FDN reverb. This is heard in the blurring of frequencies as they are sustained within the reverb.

; P I T C H   S H I F T
; Interval    - the interval of a time domain pitch shifter inserted in the FDN reverb feedback loop
; Delay Time  - the delay time of the delay buffer used to implement the time domain pitch shifter

; M I X
; Input Gain  - input into the effect
; Pitch Mix   - amount of pitch shifted signal in the feedback mix
; Dry/Wet     - mix between the wet (effected) and dry signal

<Cabbage>
form caption("Shimmer Reverb") size(1090,145), colour(0,0,0) pluginId("ShRv"), guiRefresh(16) 

image    bounds( 10, 10,590,125), colour(0,0,0,0), outlineThickness(1) {
label    bounds(  0,  5,590, 15), text("R E V E R B")
rslider  bounds( 20, 25, 70, 90), text("Pre Delay"),  channel("PreDelay"),  range(0.01,1,0.01,0.5),              $RSliderStyle
rslider  bounds(100, 25, 70, 90), text("Smear"),  channel("Smear"),  range(0.01,3,1,0.5),              $RSliderStyle
rslider  bounds(180, 25, 70, 90), text("Reverb Time"),  channel("RvbTime"),  range(0.1,18,7,0.5),              $RSliderStyle
rslider  bounds(260, 25, 70, 90), text("Damping"),  channel("Damping"),  range(200,18000,18000,0.5),              $RSliderStyle
rslider  bounds(340, 25, 70, 90), text("Mod.Rate"),  channel("ModRate"),  range(0.1,6,1,0.5),              $RSliderStyle
rslider  bounds(420, 25, 70, 90), text("Mod.Depth"),  channel("ModDep"),  range(0,10,1,0.5),              $RSliderStyle
rslider  bounds(500, 25, 70, 90), text("Size"),  channel("Size"),  range(0.1,10,1,0.5),              $RSliderStyle
}

image    bounds(610, 10,190,125), colour(0,0,0,0), outlineThickness(1) {
label    bounds(  0,  5,190, 15), text("P I T C H   S H I F T")
rslider  bounds( 20, 25, 70, 90), text("Interval"),  channel("Interval"),  range(-24,24,12,1,0.1),       $RSliderStyle
rslider  bounds(100, 25, 70, 90), text("Delay Time"),  channel("DelTime"),  range(0.05,5,0.5,0.5),              $RSliderStyle
}

image    bounds(810, 10,270,125), colour(0,0,0,0), outlineThickness(1) {
label    bounds(  0,  5,270, 15), text("M I X")
rslider  bounds( 20, 25, 70, 90), text("Input Gain"),  channel("InputGain"),  range(0,1,0.95),              $RSliderStyle
rslider  bounds(100, 25, 70, 90), text("Pitch Mix"),  channel("PitchMix"),  range(0,1,1),              $RSliderStyle
rslider  bounds(180, 25, 70, 90), text("Dry/Wet"),  channel("DryWet"),  range(0,1,0.5),              $RSliderStyle
}
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-dm0 -n -+rtmidi=NULL -M0 --displays
</CsOptions>

<CsInstruments>

; sr set by host
ksmps  = 16
nchnls = 2
0dbfs  = 1


; UDO for the pitch shifter
opcode  PitchShifter, a, akki
ain,ktrans,kdlt,iMaxDlt     xin
ihalfsine   ftgen     0, 0, 1025, 9, 0.5, 1, 0        ; HALF SINE  WINDOW FUNCTION USED FOR AMPLITUDE ENVELOPING
koctfract   =         ktrans/12                       ; TRANSPOSITION AS FRACTION OF AN OCTAVE
kratio      =         octave(koctfract)               ; RATIO TO PRODUCE PITCH SHIFT
krate       =         (kratio-1)/kdlt                 ; SUBTRACT 1/1 SPEED
            
aphase1     phasor    -krate                          ; MOVING PHASE 1-0
aphase2     phasor    -krate, .5                      ; MOVING PHASE 1-0 - PHASE OFFSET BY 180 DEGREES (.5 RADIANS)

agate1      tablei    aphase1, ihalfsine, 1, 0, 1     ; WINDOW FUNC = HALF SINE
agate2      tablei    aphase2, ihalfsine, 1, 0, 1     ; WINDOW FUNC = HALF SINE
    
abuffer     delayr    iMaxDlt                         ; DECLARE DELAY BUFFER
adelsig1    deltap3   aphase1 * a(kdlt)               ; VARIABLE TAP
aGatedSig1  =         adelsig1 * agate1
            delayw    ain                             ; WRITE AUDIO TO THE BEGINNING OF THE DELAY BUFFER, MIX IN FEEDBACK SIGNAL - PROPORTION DEFINED BY gkFB

abuffer     delayr    iMaxDlt                         ; DECLARE DELAY BUFFER
adelsig2    deltap3   aphase2 * a(kdlt)               ; VARIABLE TAP
aGatedSig2  =         adelsig2 * agate2 
            delayw    ain                             ; WRITE AUDIO TO THE BEGINNING OF THE DELAY BUFFER, MIX IN FEEDBACK SIGNAL - PROPORTION DEFINED BY gkFB
aout        =         (aGatedSig1 + aGatedSig2) * 0.5
            xout      aout                            ; SUM AND RESCALE PITCH SHIFTER OUTPUTS (LEFT CHANNEL)
endop



instr       1
;aL,aR      diskin2   "AcousticGuitar.wav", 1,0,0
; READ STEREO INPUT
aL,aR      ins 

; INITIALISE AUDIO VARIABLES USED IN FEEDBACK (NEEDED FOR FIRST ITERATION)
afil1, afil2, afil3, afil4, afil5, afil6, afil7, afil8 init        0

; DEFINE THE NUMBER OF DELAYS IN THE FDN, BUT IF THIS IS CHANGED, OTHER CODE RELATING TO THE NUMBER OF DELAYS WILL ALSO NEED CHANGED
iNumberOfDelays =           8
; CREATE MIXED FEEDBACK SIGNAL
aGFB       =       (afil1 + afil2 + afil3 + afil4 + afil5 + afil6 + afil7 + afil8) * 2/iNumberOfDelays   ; global feedback

; PORTAMENTO FUNCTION (A RAMP THAT RISES QUICKLY FROM 0 TO 0.1
kPortTime  linseg 0, 0.001, 0.1

; READ IN WIDGETS
kInputGain chnget "InputGain"
kRvbTime   chnget "RvbTime"
kInterval  chnget "Interval"
kDelTime   chnget "DelTime"
kPitchMix  chnget "PitchMix"
kDamping   chnget "Damping"
kDryWet    chnget "DryWet"
kPreDelay  chnget "PreDelay"
kSmear     chnget "Smear"
kModRate   chnget "ModRate"
kModDep    chnget "ModDep"
kSize      chnget "Size"

; SMOOTH CHANGES IN SELECTED VARIABLES
kInterval  portk  kInterval, kPortTime
kDelTime   portk  kDelTime, kPortTime
kSize      portk  kSize, kPortTime

; SCALE INPUT AUDIO SIGNAL ACCORDING TO WIDGET
aDryL  =   aL * kInputGain
aDryR  =   aR * kInputGain

; ONLY APPLY 'TIME SMEARING' USING ALLPASS FILTERS TO THE INPUT SIGNAL IF smear time IS GREATER THAN 0.1
if kSmear>0.01 then
kRvt       =          0.5 * kSmear ; feedback ratio for the allpass filters
kLptMlt    =          10 * kSmear  ; delay time within the allpass filters
; ALLPASS FILTERS ARE IN SERIES FOR EACH CHANNEL
; aInL -> ALLPASS1 -> ALLPASS2 -> ALLPASS3 -> ALLPASS4 -> FDN_IN_L
; aInR -> ALLPASS1 -> ALLPASS2 -> ALLPASS3 -> ALLPASS4 -> FDN_IN_R
aL         valpass     aDryL, kRvt, 0.005 * kLptMlt, 1
aL         valpass     aL, kRvt, 0.0017 * kLptMlt, 1
aL         valpass     aL, kRvt, 0.0037 * kLptMlt, 1
aL         valpass     aL, kRvt, 0.0047 * kLptMlt, 1

aR         valpass     aDryR, kRvt, 0.005 * kLptMlt, 1
aR         valpass     aR, kRvt, 0.0017 * kLptMlt, 1
aR         valpass     aR, kRvt, 0.0037 * kLptMlt, 1
aR         valpass     aR, kRvt, 0.0047 * kLptMlt, 1
endif

; APPLY THE PITCH SHIFTER TO THE FEEDBACK SIGNAL
;                      INPUT     SEMITONES   DELAY_TIME
aGFB_PS   PitchShifter aGFB*0.5, kInterval,  kDelTime, 5 + 1/kr

; MIX BETWEEN PITCH_SHIFTED AND UNPITCH-SHIFTED SOUNDS ACCORDING TO THE GUI WIDGET
aGFB      ntrpol       aGFB, aGFB_PS, kPitchMix

; CREATE THE DELAY TIME VALUES FOR EACH DELAY
; THESE ARE MODULATED BY SLOWLY SHIFTING RANDOM FUNCTIONS BUT ARE ALSO OFFSET WITH RESPECT TO EACH OTHER
; rspline IS USED TO CREATE A COMPLEX MOVEMENT
klpt1  =           rspline:k(0.08, 0.08 + (0.0006*kModDep), 5*kModRate, 8*kModRate)
klpt2  =           rspline:k(0.08, 0.08 + (0.0006*kModDep), 5*kModRate, 8*kModRate) * 0.9137
klpt3  =           rspline:k(0.08, 0.08 + (0.0006*kModDep), 5*kModRate, 8*kModRate) * 0.81678
klpt4  =           rspline:k(0.08, 0.08 + (0.0006*kModDep), 5*kModRate, 8*kModRate) * 0.72190
klpt5  =           rspline:k(0.08, 0.08 + (0.0006*kModDep), 5*kModRate, 8*kModRate) * 0.627189
klpt6  =           rspline:k(0.08, 0.08 + (0.0006*kModDep), 5*kModRate, 8*kModRate) * 0.526177
klpt7  =           rspline:k(0.08, 0.08 + (0.0006*kModDep), 5*kModRate, 8*kModRate) * 0.321567
klpt8  =           rspline:k(0.08, 0.08 + (0.0006*kModDep), 5*kModRate, 8*kModRate) * 0.1372891

; APPLY PRE-DELAY TO THE INPUT SIGNAL
aLDel  vdelay      aL, a(kPreDelay)*1000, (1+(1/kr))*1000
aRDel  vdelay      aR, a(kPreDelay)*1000, (1+(1/kr))*1000

; THE 8 DELAYS THAT COMPRISE THE FDN
adel1  vdelay      aLDel + aGFB - afil1, a(klpt1*1000*kSize), 2*1000
adel2  vdelay      aRDel + aGFB - afil2, a(klpt2*1000*kSize), 2*1000
adel3  vdelay      aLDel + aGFB - afil3, a(klpt3*1000*kSize), 2*1000
adel4  vdelay      aRDel + aGFB - afil4, a(klpt4*1000*kSize), 2*1000
adel5  vdelay      aLDel + aGFB - afil5, a(klpt5*1000*kSize), 2*1000
adel6  vdelay      aRDel + aGFB - afil6, a(klpt6*1000*kSize), 2*1000
adel7  vdelay      aLDel + aGFB - afil7, a(klpt7*1000*kSize), 2*1000
adel8  vdelay      aRDel + aGFB - afil8, a(klpt8*1000*kSize), 2*1000

; DAMPING FILTERS (1ST ORDER LOW-PASS FILTERS, -6dB per octave)
afil1  tone        adel1 * ampdbfs(-60/(kRvbTime/klpt1)), kDamping
afil2  tone        adel2 * ampdbfs(-60/(kRvbTime/klpt2)), kDamping
afil3  tone        adel3 * ampdbfs(-60/(kRvbTime/klpt3)), kDamping
afil4  tone        adel4 * ampdbfs(-60/(kRvbTime/klpt4)), kDamping   
afil5  tone        adel5 * ampdbfs(-60/(kRvbTime/klpt5)), kDamping
afil6  tone        adel6 * ampdbfs(-60/(kRvbTime/klpt6)), kDamping
afil7  tone        adel7 * ampdbfs(-60/(kRvbTime/klpt7)), kDamping
afil8  tone        adel8 * ampdbfs(-60/(kRvbTime/klpt8)), kDamping 

; MIX DRY AND WET SIGNALS ACCORDING TO GUI WIDGET. 
; NOTE THAT DELAY OUTPUTS ARE SENT TO ALTERNATE OUTPUTS BUT WILL STILL EXPERIENCE CROSS-CHANNEL MIXING VIA THE FDN
aMixL  ntrpol      aDryL, (adel1 + adel3 + adel5 + adel7) * 0.3, kDryWet
aMixR  ntrpol      aDryR, (adel2 + adel4 + adel6 + adel8) * 0.3, kDryWet

; OUTPUT
       outs        aMixL*2, aMixR*2
endin

</CsInstruments>

<CsScore>
i 1 0 5000
;f 0 [3600*24*365]
</CsScore>

</CsoundSynthesizer>
