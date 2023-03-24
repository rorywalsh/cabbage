
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; RingModSynth.csd
; Iain McCurdy, 2015

; 2 oscillator additive synth with a third oscillator used as a ring modulator upon the first two.
; The second oscillator amplitude always decays faster than that of the first oscillator.
; The amplitude of the ring modulator always decays faster than the amplitude of the first and second oscillators.
; OSC.1 can be thought of as the resonating fundemental of the sound.
; OSC.2 provides a persussive impulse at the beginning of the sound. The 'Ratio' of OSC.2 defines the spectral character of the attack.
; MOD (OSC.3) is responsible for the spectral character (and spectral decay) of the sound.
; This simple arrangment should prove efficient in realtime whilst still providing a range of sounds through an intuitive interface.

; The amount of ring modulation applied is normally reduced as higher and higher notes are played. This is done in order to attenuate the
;  apparent increasing brightness of higher notes (and to prevent resulting aliasing). Settings for this function are made in the SCALE section.

; Higher notes will decay quicker than lower notes. This is implemented as an imitation of what happens in the natural acoustic world and there 
;  are no user controls over this mechanism

; OSC.1
; -----
; Decay        --    decay time in seconds. 
;            Actually this affects the decay times for osc.2 and the modulator oscillator as they will be defined as ratios of this value.
; Release    --    release time

; OSC.2
; -----
; Ratio        --    ratio of the frequency of oscillator 2 to that of oscillator 1. (Oscillator 1 takes the frequency of the note played.)
; Decay        --    decay time for oscillator 2. A ratio of the decay time for oscillator 1.

; MOD
; ---
; Ratio        --    ratio of the frequency of the modulator to that of oscillator 1.
; Decay        --    decay time for the modulator oscillator. A ratio of the decay time for oscillator 1.

; SPACE (spatialising of the monophonic signal using comb filters)
; -----
; Mix        --    Mix of the spacialising effect. 0=mono, 1=100%_stereo
; Width        --    Width of the stereo effect - delay times for the comb filters.
;  NB the delay times will also be biased according to the note played so that lower notes are increasingly biased toward the left channel and higher notes increasingly to the right (using the Haas effect)

; VEL. (velocity response adjustment)
; ----
; Amount    --    amount of velocity influence upon spectrum/brightness
; Offset    --    shift of spectral brightness, independent of velocity received

; RANDOM (note by note random offsets)
; ------
; Fundemental    --    random offsets of the fundemental (as in an out of tune piano)
; Spectrum    --    random offsets of osc.2 freq. ratio and the modulator ratio

; SCALE (keyboard scaling of brightness, it is useful to suppress the brightness of higher notes)
; -----
; Note        --    the note about which scaling pivots
; Amount    --    amount of brightness scaling

; OUTPUT
; ------
; High Cut    --    a static lowpass filter. Bypassed if control is maximum.
; Level        --    output level

; ratio 1 controls the ratio of the frequency of the second oscillator to the fundemental (frequency of the first oscillator.)
; ratio 2 controls the ratio of the frequency of the ring modulator to the fundemental (frequency of the first oscillator.)

<Cabbage>
form caption("Ring Modulation Synthesizer") size(875,267), pluginId("RMSy"), colour(40,40,40)

image      bounds(  5,  5, 70,160), colour(0,0,0,0), outlineColour("white"), outlineThickness(4), shape("rounded"), plant("osc1") {
label      bounds(  5,  5, 60, 14), text("OSC.1")
rslider    bounds(  5, 25, 60, 60), text("Decay"),        channel("Dec"),   range(0.01,12, 2,1,0.001)
rslider    bounds(  5, 95, 60, 60), text("Release"),      channel("Rel"),   range(0.01,12, 0.2,1,0.001)
}

image      bounds( 80,  5, 70,160), colour(0,0,0,0), outlineColour("white"), outlineThickness(4), shape("rounded"), plant("osc2") {
label      bounds(  5,  5, 60, 14), text("OSC.2")
rslider    bounds(  5, 25, 60, 60), text("Ratio"),        channel("PRat"),  range(.1,  30, 3.37,0.5,0.001)
rslider    bounds(  5, 95, 60, 60), text("Decay"),      channel("PDec"),  range(0.001,1, 0.2,1,0.001)
}
image      bounds(155,  5, 70,160), colour(0,0,0,0), outlineColour("white"), outlineThickness(4), shape("rounded"), plant("RM") {
label      bounds(  5,  5, 60, 14), text("MOD.")
rslider    bounds(  5, 25, 60, 60), text("Ratio"),        channel("MRat"),  range(.1,  30, 2.77,0.5,0.001)
rslider    bounds(  5, 95, 60, 60), text("Decay"),      channel("MDec"),  range(0.001,1, 0.1,1,0.001)
}
image      bounds(230,  5, 70,160), colour(0,0,0,0), outlineColour("white"), outlineThickness(4), shape("rounded"), plant("Spat") {
label      bounds(  5,  5, 60, 14), text("SPACE")
rslider    bounds(  5, 25, 60, 60), text("Mix"),           channel("SpatMix"),   range(0,1,1,1,0.001)
rslider    bounds(  5, 95, 60, 60), text("Width"),         channel("SpatWidth"), range(0.001,0.05,0.03,1,0.001)
}
image      bounds(305,  5, 70,160), colour(0,0,0,0), outlineColour("white"), outlineThickness(4), shape("rounded"), plant("Velocity") {
label      bounds(  5,  5, 60, 14), text("VEL.")
rslider    bounds(  5, 25, 60, 60), text("Amount"),      channel("VelAmt"),   range(  0,  1,0.4,0.5,0.001)
rslider    bounds(  5, 95, 60, 60), text("Offset"),      channel("VelOS"),    range(  0,  1,0,0.5,0.001)
}
image      bounds(380,  5, 70,160), colour(0,0,0,0), outlineColour("white"), outlineThickness(4), shape("rounded"), plant("Tuning") {
label      bounds(  5,  5, 60, 14), text("RANDOM")
rslider    bounds(  5, 25, 60, 60), text("Fund."),        channel("RndTune"), range(0,10,0.1,0.5,0.001)
rslider    bounds(  5, 95, 60, 60), text("Spectrum"),     channel("RndSpec"), range(0,10,0.1,0.5,0.001)
}
image      bounds(455,  5, 70,160), colour(0,0,0,0), outlineColour("white"), outlineThickness(4), shape("rounded"), plant("Scale") {
label      bounds(  5,  5, 60, 14), text("SCALE")
rslider    bounds(  5, 25, 60, 60), text("Note"),          channel("KybdNote"), range(  0,127, 64,1,1)
rslider    bounds(  5, 95, 60, 60), text("Amount"),        channel("KybdScal"), range(  0,  5,3.5,1,0.001)
}
image      bounds(530,  5, 70,160), colour(0,0,0,0), outlineColour("white"), outlineThickness(4), shape("rounded"), plant("Attack") {
label      bounds(  5,  5, 60, 14), text("ATTACK")
rslider    bounds(  5, 25, 60, 60), text("Time"),       channel("AttTim"),   range(0,0.5,0,0.5,0.001)
rslider    bounds(  5, 95, 60, 60), text("Smoothing"),  channel("AttSmooth"),range(  0,   1,  0.5,1,0.001)
}
image      bounds(605,  5,130,160), colour(0,0,0,0), outlineColour("white"), outlineThickness(4), shape("rounded"), plant("Detune") {
label      bounds(  5,  5,120, 14), text("DETUNE")
rslider    bounds(  5, 25, 60, 60), text("Random"),       channel("DtnRnd"),   range(0,100,0,1,0.001)
rslider    bounds(  5, 95, 60, 60), text("Manual"),       channel("DtnMan"),   range(-50,50,0,1,0.01)
rslider    bounds( 65, 25, 60, 60), text("Velocity"),     channel("DtnVel"),   range(0,1,0,1,0.001)
}
image      bounds(740,  5,130,160), colour(0,0,0,0), outlineColour("white"), outlineThickness(4), shape("rounded"), plant("Output") {
label      bounds(  5,  5,120, 14), text("GLOBAL")
rslider    bounds(  5, 25, 60, 60), text("High Cut"),      channel("LPF"),    range(100,12000,12000,0.5,1)
rslider    bounds(  5, 95, 60, 60), text("Sus.Lev."),      channel("SusLev"), range(  0,    1,  0,1,0.001)
label      bounds( 75, 30, 40, 10), text("PRESET")
combobox   bounds( 75, 40, 40, 20), text("1","2","3","4","5","6","7","8","9","10"), channel("preset"), value(1)
rslider    bounds( 65, 95, 60, 60), text("Level"),         channel("Level"),  range(  0,    1,  0.7,0.5,0.001)
}

keyboard   bounds(  5,170,865, 85)

label bounds(5,256,97,10), colour("black"), text("Iain McCurdy |2015|") align("right")
</Cabbage>

<CsoundSynthesizer>

<CsoundSynthesizer>

<CsOptions>
-dm0 -n -+rtmidi=NULL -M0 --midi-key=4 --midi-velocity=5
</CsOptions>

<CsInstruments>

; sr set by host
ksmps = 16
nchnls = 2
0dbfs = 1
zakinit    2,2
massign    0,2

; waveforms
gisine    ftgen    1,0,4096,10,1
giwave2    ftgen    2,0,4096,10,1,0.5
giwave3    ftgen    3,0,4096,10,1,1

girnd    ftgen    4,0,512,21,6,1        ; gaussian distribution

opcode    FreqShifterSimple,a,aki                    ; SIMPLE FREQUENCY SHIFTER
    asig,kfshift,ifn    xin                ; READ IN INPUT ARGUMENTS
    areal, aimag hilbert asig                ; HILBERT OPCODE OUTPUTS TWO PHASE SHIFTED SIGNALS, EACH 90 OUT OF PHASE WITH EACH OTHER
    asin     oscili       1,    kfshift,     ifn,          0
    acos     oscili       1,    kfshift,     ifn,          0.25    
    ;RING MODULATE EACH SIGNAL USING THE QUADRATURE OSCILLATORS AS MODULATORS
    amod1    =        areal * acos
    amod2    =        aimag * asin    
    ;UPSHIFTING OUTPUT
    ares    = (amod1 - amod2)                ; MIX OUTPUTS (INVERTED WITH RESPECT TO EACH OTHER)
        xout    ares                    ; SEND AUDIO BACK TO CALLER INSTRUMENT
endop


instr    2 
 inote    =    p4            

 iKybdNote    chnget    "KybdNote"                        ; pivot note for brightness scaling
 iKybdAmt    chnget    "KybdScal"                        ; amount of keyboard to brightness scaling
 iKybdScal    limit    1 + ( ((iKybdNote-inote)/127) *  iKybdAmt), 0, 5    ; scaling value

 iDurScal    limit    1 + ( ((64-inote)/127) * 3), 0.01, 5            ; duration scaling (higher notes shorter, lower notes longer). No user control.

 iNoteOS    table    inote,girnd                        ; random fundemental offset value from table
 iRndTune    chnget    "RndTune"                        ; amount of random tuning offset
 inote    +=    iNoteOS * iRndTune                        ; add random offset

 iSpecOS    table    inote+128,girnd                        ; spectral offset value from table
 iRndSpec    chnget    "RndSpec"                        ; amount of random spectral offset
 iSpec    =    iSpecOS * iRndSpec                        ; scale random value from table by GUI control

 iVelAmt    chnget    "VelAmt"                        ; amount of velocity control
 iVelOS        chnget    "VelOS"                            ; velocity offset

 ivel    =    (p5/127)^4                            ; velocity response shaped by being raised to the power of 4
 iDec    chnget    "Dec"                                ; decay time ratio
 iPDec    chnget    "PDec"                                ; timbral decay (a value 0 - 1 but probably closer to zero)
 iRDec    chnget    "MDec"                                ; timbral decay (a value 0 - 1 but probably closer to zero)
 iRel    chnget    "Rel"                                ; release time (a fixed value)

 ; FREQUENCY RATIOS
 if0    =    1                                ; 1st partial (fundemental)
 if1    chnget    "PRat"                                ; 2nd partial
 imod    chnget    "MRat"                                ; ring modulator frequency ratio
 
 ; AMPLITUDE ENVELOPES
 iSusLev    chnget    "SusLev"
 iSusLev    +=    0.0001
 aenv1    expsegr    1,      iDec*iDurScal,iSusLev,iRel,0.0001            ; f0
 aenv2    expsegr    1,iPDec*iDec*iDurScal,iSusLev,iRel,0.0001            ; f1
 aenv3    expsegr    1,iRDec*iDec*iDurScal,iSusLev,iRel,0.0001            ; mod
 
 ; ENVELOPE SMOOTHING (SPECIFICALLY ATTACK)
 iAttTim    chnget    "AttTim"
 iAttSmooth    chnget    "AttSmooth"
 if iAttTim>0 then
  aenv1    delay    aenv1,rnd(iAttTim*0.5)+(iAttTim*0.5)
  aenv2    delay    aenv2,rnd(iAttTim*0.5)+(iAttTim*0.5)
  aenv3    delay    aenv3,rnd(iAttTim*0.5)+(iAttTim*0.5)   
  iAttSmooth    =    cpsoct(8 - (iAttSmooth*7)) 
  aenv1    tone    aenv1,iAttSmooth
  aenv2    tone    aenv2,iAttSmooth
  aenv3    tone    aenv3,iAttSmooth
  xtratim    iAttTim + iRel
 endif
  
 ; PARTIAL OSCILLATORS
 apart1    oscili    (aenv1-0.0001) * 1   , cpsmidinn(inote) * if0, gisine        ; fundemental oscillato
 apart2    oscili    (aenv2-0.0001) * ((ivel*iVelAmt)+iVelOS) * iKybdScal, cpsmidinn(inote+iSpec) * if1, gisine    ; partial 2 will be doubly affected by velocity (see further down) thereby creating a velocity-timbre mapping 

 ; MIXER
 amix    sum    (apart1 + apart2) * ivel * 0.3                    ; mix the two main oscillators
 
 ; RING MODULATION
 amod    poscil    aenv3,cpsmidinn(inote+iSpec) * imod, gisine            ; ring modulation oscillators
 iLevel    chnget    "Level"
 amix    =    amix * iLevel * (1 - (amod*((ivel*iVelAmt)+iVelOS)*iKybdScal))    ; amount of ring modulation dependent upon velocity

 ; DETUNING
 iDtnRnd    chnget    "DtnRnd"
 iDtnMan    chnget    "DtnMan"
 iDtnVel    chnget    "DtnVel"
 if (iDtnRnd>0) || (iDtnMan!=0) then
  iFSfrq    table    inote+256,girnd
  iFSfrq    ntrpol    iDtnMan + (iFSfrq*iDtnRnd),(iDtnMan + (iFSfrq*iDtnRnd))*ivel,iDtnVel 
  aFS    FreqShifterSimple    amix,iFSfrq,gisine
  amix    =            (amix + aFS) * 0.8
 endif
 
 iSpatMix    chnget    "SpatMix"                        ; amount of spacial width mix
 iSpatWidth    chnget    "SpatWidth"                        ; width of spacial effect
 iDelL    =    0.0001 + ((1-(inote/127)) * iSpatWidth)                ; derive delay times according to note played
 iDelR    =    0.0001 + ((inote/127) * iSpatWidth)
 aCL    comb    amix, 0.01, iDelL
 aCR    comb    amix, 0.01, iDelR
 aL    ntrpol    amix,aCL,iSpatMix                        ; mix
 aR    ntrpol    amix,aCR,iSpatMix
    zawm    aL,1                                ; send to zak channels
    zawm    aR,2
endin

instr    3        ; output instrument (always on)
 aL    zar    1
 aR    zar    2
 kLPF    chnget    "LPF"
 if kLPF<12000 then                                ; low pass filter if control is below maximum
  aL    butlp    aL,kLPF
  aR    butlp    aR,kLPF
 endif
    outs    aL,aR 
    zacl    1,2                                ; clear zak channels
endin


instr    99
#define    PRESET(Dec'Rel'PRat'PDec'MRat'MDec'SpatMix'SpatWidth'VelAmt'VelOS'RndTune'RndSpec'KybdNote'KybdScal'AttTim'AttSmooth'DtnRnd'DtnMan'DtnVel'LPF'SusLev)
#
    chnset    $Dec,         "Dec"
     chnset    $Rel,          "Rel"
     chnset    $PRat,         "PRat"
     chnset    $PDec,         "PDec"
     chnset    $MRat,         "MRat"
     chnset    $MDec,         "MDec"
     chnset    $SpatMix,      "SpatMix"
     chnset    $SpatWidth,    "SpatWidth"
     chnset    $VelAmt,       "VelAmt"
     chnset    $VelOS,        "VelOS"
     chnset    $RndTune,      "RndTune"
     chnset    $RndSpec,      "RndSpec"
     chnset    $KybdNote,     "KybdNote"
     chnset    $KybdScal,     "KybdScal"
     chnset    $AttTim,       "AttTim"
     chnset    $AttSmooth,    "AttSmooth"
     chnset    $DtnRnd,       "DtnRnd"
     chnset    $DtnMan,       "DtnMan"
     chnset    $DtnVel,       "DtnVel"
     chnset    $LPF,          "LPF"
     chnset    $SusLev,    "SusLev"    
#
 kpreset    chnget    "preset"
 if changed(kpreset)==1 then
  reinit UPDATE
 endif
 UPDATE:
 if i(kpreset)==1 then
; $PRESET(Dec'Rel'PRat'PDec'MRat'MDec'SpatMix'SpatWidth'VelAmt'VelOS'RndTune'RndSpec'KybdNote'KybdScal'AttTim'AttSmooth'DtnRnd'DtnMan'DtnVel'LPF  'SusLev) 
  $PRESET(2  '0.2'3.37'0.2 '2.77'0.2 '1      '0.03     '0.4   '0    '0.1    '0.1    '64      '3.5     '0     '0.5      '0     '0     '0     '12000'0     ) 
 elseif i(kpreset)==2 then
  $PRESET(6.4'0.2'5.31'0.2 '2.77'0.2 '1      '0.03     '0.4   '0    '0.1    '0.1    '64      '3.5     '1.5   '0.82     '14    '0     '0     '12000'0     ) 
 elseif i(kpreset)==3 then
  $PRESET(6.4'0.2'5.31'0.2 '2.77'0.2 '1      '0.03     '0.4   '0    '0.1    '0.1    '64      '3.5     '0.001 '0.15     '25    '0     '1     '12000'0     ) 
 elseif i(kpreset)==4 then
  $PRESET(6.4'2.3'3.71'0.09'5.43'0.09'1      '0.03     '1     '0    '0.1    '10     '64      '3.5     '0.5   '1        '25    '-5.6   '0.25 '3000 '0.12  ) 
 elseif i(kpreset)==5 then
  $PRESET(6.4'0.5'2.07'0.2 '1.01'0.34'1      '0.03     '1     '0    '0.1    '0.1    '64      '3.5     '0.005 '0.38     '25    '0     '1     '12000'0.13  ) 
 elseif i(kpreset)==6 then
  $PRESET(9.5'0.2'8   '0.2 '30  '0.2 '1      '0.03     '0.9   '0    '0.1    '0.1    '64      '3.5     '0     '0.5      '8     '0     '0.5   '1200 '0     ) 
 elseif i(kpreset)==7 then
  $PRESET(8  '0.2'2   '0.18'14  '0.15'1      '0.03     '0.9   '0    '0.1    '0.1    '64      '3.5     '0     '0.5      '8     '0     '0.5   '1200 '0     ) 
 elseif i(kpreset)==8 then
  $PRESET(0.18'0.2'5.22'1  '5.33'1    '1     '0.03     '0.9   '0    '10     '10     '60      '5       '0.08  '0.24     '100   '3.6   '1     '12000'0.18  ) 
 elseif i(kpreset)==9 then
  $PRESET(8  '0.2'2   '0.4 '7.5 '0.009'1     '0.05     '1     '0    '0.1    '0.1    '64      '3.5     '0     '0.5      '8     '0     '0.5   '1200 '0     ) 
 elseif i(kpreset)==10 then
  $PRESET(0.18'0.2'5.22'1  '5.33'1    '1     '0.03     '0.9   '0    '10     '10     '60      '5       '0.322 '0.24     '100   '3.6   '1     '12000'0     ) 
 endif
 rireturn
endin


</CsInstruments>  

<CsScore>
i 3  0 [3600*24*7]
i 99 0 [3600*24*7]
</CsScore>

</CsoundSynthesizer>