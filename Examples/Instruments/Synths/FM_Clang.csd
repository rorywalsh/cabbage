
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; FM_Clang.csd
; Written by Iain McCurdy, 2014
; 
; An FM synthesis based instrument that specialises in creating percussive metallic 'clang'-like sounds.
; 
; AMPLITUDE   -    controls pertaining to the control of each notes amplitude
; Level       -    Global amplitude control
; Attack      -    Attack time. Actually just a small range of control but increasing it can soften the attack subtly
; Vel.Att.    -    Degree to which lower note velocities lengthen attack time.
; Duration    -    Duration of the overall amplitude decay. Note that depending on other settings notes may actually sound for a shorter time.
; Sustain     -    Similar to the effect of a sustain pedal being depressed.
; 
; MODULATO    -    controls pertaining to the modulator in the FM algorithm
; Ratio       -    Ratio of the modulator frequency to that of the carrier.
; Vel.Index   -    Index of modulation. This is also always dependent upon note velocity.
; Decay       -    Rate of decay of the index. This is heard as the rate of loss of spectral energy or damping.
; 
; CARRIER     -    controls pertaining to the carrier in the FM algorithm
; Kyb.Scale   -    Keyboard scaling of the carrier frequency. Zero means that frequencies follow normal keyboard temperament. 
;                                                                    Values less than zero mean that frequency intervals from note to note will be compressed.
;                                                                    Values greater than zero mean that frequency intervals from note to note will be expanded.
; 
; DETUNE      -    Two FM carrier/modulator pairings can be created with a detuning offset between them to create beating effects. 
;                  Detuning will also be unique (and repeatable) from note to note to give the instrument character.
; Mix         -    Amplitude of the second FM pair. This will control the amplitude of beating (amplitude modulation) effects resulting from the detuning.
; Amount      -    Amplitude of the detuning intervals
; 
; STEREO      -    A stereo broadening effect by applying short delays to the left and right channels. The exact durations of the delays will be unique (and repeatable) from note to note to give the instrument character.
; Dry/Wet     -    A dry/wet control of this effect
; Width       -    A control of the durations of the delays which is heard as a control of the stereo width of the effect.
; 
; NOISE       -    Filtered gaussian noise can be added to the modulator ratio to add discordancy to the tone.
; Amount      -    Amplitude of the gaussian noise signal
; Filter      -    Narrowness of the bandpass filtering applied to the gaussian noise
; Follow Modulator (checkbox)    -    Selects whether the bandpass filter follows the modulator frequency. If 'off' it simply follows the carrier frequency.
; 
; LFO         -    A sine shaped LFO added to the modulator ratio of an auxilliary FM pair which is then mixed to an unmodulated pair. This can introduce spectral modulation, as in a bell swinging back and forth as it rings.
; Mix         -    Amount of modulated signal mixed into the output.
; Depth       -    Depth of the LFO modulation.
; Rate        -    Rate of the LFO modulation.
; Kybd.Follow -    Extend to which the rate of LFO modulation follows keyboard notes - keyboard following means that lower notes will modulate at lower frequencies, higher notes will modulate at higher frequencies.
; 
; FILTER      -    Filtering of the sound
; LPF         -    A lowpass filter that follows the fundemental frequency of the FM pair
; Filter Fundemental (checkbox)    -    If active, the fundemental will be filtered off.
; 
; REVERB      -    a reverb effect using the freeverb opcode®

<Cabbage>
form caption("FM Clang") size(945, 320), pluginId("FMCl"), guiRefresh(256), colour(40,40,40)

#define RSliderStyle # colour("white"), trackerColour("silver"), fontColour("white"), textColour("white"), outlineColour(120,140,140)#

;AMPLITUDE
image    bounds(  5,  5,300,110), colour( 70, 90, 90), outlineColour("white"), outlineThickness(2), shape("sharp"), plant("amplitude"){
line     bounds( 90,  6,120, 14)
label    bounds( 90,  8,120, 10), text("A  M  P  L  I  T  U  D  E"), textColour(white)
rslider  bounds(  0, 25, 75, 75), text("Level"),    channel("Amp"),    range(0,8.00,0.5,0.5),        valueTextBox(1), textBox(1), $RSliderStyle
rslider  bounds( 55, 25, 75, 75), text("Attack"),   channel("AttTim"), range(0,0.1,0,0.5,0.001),     valueTextBox(1), textBox(1), $RSliderStyle
rslider  bounds(110, 25, 75, 75), text("Vel.Att."), channel("AttVel"), range(0,1,0),                 valueTextBox(1), textBox(1), $RSliderStyle
rslider  bounds(165, 25, 75, 75), text("Duration"), channel("Dur"),    range(0.1, 24.00, 12, 0.5,),  valueTextBox(1), textBox(1), $RSliderStyle
checkbox bounds(230, 53,180, 12), channel("Sustain"), value(1), text("Sustain"), fontColour:0("white"), fontColour:1("white")
}

;MODULATOR
image    bounds(310,  5,180,110), colour( 70, 90, 90), outlineColour("white"), outlineThickness(2), shape("sharp"), plant("modulator"){
line     bounds( 30,  6,120, 14)
label    bounds( 30,  8,120, 10), text("M  O  D  U  L  A  T  O  R"), textColour(white)
rslider  bounds(  0, 25, 75, 75), text("Ratio"), channel("mod"), range(1, 17, 2.29, 1,0.01),    valueTextBox(1), textBox(1), $RSliderStyle
rslider  bounds( 55, 25, 75, 75), text("Index"), channel("NdxVel"), range(0, 2.00, 1,0.75),     valueTextBox(1), textBox(1), $RSliderStyle
rslider  bounds(110, 25, 75, 75), text("Decay"), channel("NdxCurve"), range(8, 256.00, 16,0.5), valueTextBox(1), textBox(1), $RSliderStyle
}

;CARRIER
image    bounds(495,  5,105,110), colour( 70, 90, 90), outlineColour("white"), outlineThickness(2), shape("sharp"), plant("carrier"){
line     bounds(  7,  6, 90, 14)
label    bounds(  7,  8, 90, 10), text("C  A  R  R  I  E  R"), textColour(white)
rslider  bounds( 15, 25, 75, 75), text("Kyb.Scale"), channel("CarKyb"), range(-4, 4.00, 0), valueTextBox(1), textBox(1), $RSliderStyle
}

;DETUNE
image    bounds(605,  5,130,110), colour( 70, 90, 90), outlineColour("white"), outlineThickness(2), shape("sharp"), plant("detune"){
line     bounds( 26,  6, 78, 14)
label    bounds( 26,  8, 78, 10), text("D  E  T  U  N  E"), textColour(white)
rslider  bounds(  0, 25, 75, 75), text("Mix"), channel("DtnMix"), range(0,1.00,1), valueTextBox(1), textBox(1), $RSliderStyle
rslider  bounds( 55, 25, 75, 75), text("Amount"), channel("Detune"), range(0.00,25.00,1.5), valueTextBox(1), textBox(1), $RSliderStyle
}

;FILTER
image    bounds(740,  5,200,110), colour( 70, 90, 90), outlineColour("white"), outlineThickness(2), shape("sharp"), plant("filter"){
line     bounds( 60,  6, 80, 14)
label    bounds( 60,  8, 80, 10), text("F  I  L  T  E  R"), textColour(white)
rslider  bounds(  0, 25, 75, 75), text("LPF"), channel("LPF"), range(1, 16.00, 16),  valueTextBox(1), textBox(1), $RSliderStyle
checkbox bounds( 65, 53,180, 12), channel("FiltFund"), value(1), text("Filter Fundamental"), fontColour:0("white"), fontColour:1("white")
}

;NOISE
image    bounds(  5,120,240,110), colour( 70, 90, 90), outlineColour("white"), outlineThickness(2), shape("sharp"), plant("noise"){
line     bounds( 78,  6, 69, 14)
label    bounds( 78,  8, 69, 10), text("N  O  I  S  E"), textColour(white)
rslider  bounds(  0, 25, 75, 75), text("Amount"),  channel("NseAmp"),   range(0,2000,1000,0.5,1),   valueTextBox(1), textBox(1), $RSliderStyle
rslider  bounds( 55, 25, 75, 75), text("Filter"),     channel("NseBW"), range(0.0005,1,0.001,0.35), valueTextBox(1), textBox(1), $RSliderStyle
checkbox bounds(120, 53,120, 12), channel("NseFllw"), value(0), text("Follow Modulator"), fontColour:0("white"), fontColour:1("white")
}

;LFO
image    bounds(250,120,240,110), colour( 70, 90, 90), outlineColour("white"), outlineThickness(2), shape("sharp"), plant("lfo"){
line     bounds( 85,  6, 70, 14)
label    bounds( 85,  8, 70, 10), text("L  .  F  .  O"), textColour(white)
rslider  bounds(  0, 25, 75, 75), text("Mix."), channel("ModMix"), range(0,     1, 1),  valueTextBox(1), textBox(1), $RSliderStyle
rslider  bounds( 55, 25, 75, 75), text("Depth"), channel("ModDep"), range(0, 1.000, 0.1,0.5,0.001),  valueTextBox(1), textBox(1), $RSliderStyle
rslider  bounds(110, 25, 75, 75), text("Rate"), channel("ModRte"), range(0, 32.00, 3,0.5),  valueTextBox(1), textBox(1), $RSliderStyle
rslider  bounds(165, 25, 75, 75), text("Kyb.Follow"), channel("ModRteKyb"), range(-4, 4.00, 0),  valueTextBox(1), textBox(1), $RSliderStyle
}


;STEREO
image    bounds(495,120,130,110), colour( 70, 90, 90), outlineColour("white"), outlineThickness(2), shape("sharp"), plant("stereo"){
line     bounds( 26,  6, 78, 14)
label    bounds( 26,  8, 78, 10), text("S  T  E  R  E  O"), textColour(white)
rslider  bounds(  0, 25, 75, 75), text("Dry/Wet"),  channel("StMix"), range(0,1.00,0.5),  valueTextBox(1), textBox(1), $RSliderStyle
rslider  bounds( 55, 25, 75, 75), text("Width"),  channel("StWidth"), range(0,0.2,0.01,0.5,0.001),  valueTextBox(1), textBox(1), $RSliderStyle
}

;REVERB
image    bounds(630,120,130,110), colour( 70, 90, 90), outlineColour("white"), outlineThickness(2), shape("sharp"), plant("reverb"){
line     bounds( 25,  6, 80, 14)
label    bounds( 25,  8, 80, 10), text("R  E  V  E  R  B"), textColour(white)
rslider  bounds(  0, 25, 75, 75), text("Dry/Wet"),  channel("RvbDryWet"), range(0,1.000,0.3),  valueTextBox(1), textBox(1), $RSliderStyle
rslider  bounds( 55, 25, 75, 75), text("Size"),     channel("RvbSize"),   range(0,1,0.55),     valueTextBox(1), textBox(1), $RSliderStyle
}

;PRESETS
image    bounds(765,120,175,110), colour( 70, 90, 90), outlineColour("white"), outlineThickness(2), shape("sharp"), plant("presets"){
line     bounds( 44,  6, 88, 14)
label    bounds( 44,  8, 88, 10), text("P  R  E  S  E  T  S"), textColour(white)
label    bounds( 68, 32, 40, 13), text("Select"), fontColour(white)
combobox bounds( 68, 45, 40, 19), channel("preset"), value(1), text("1","2","3","4","5","6","7","8","9","10","11","12","13")
}

keyboard bounds( 5, 235,935, 80)

</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-dm0 -n -+rtmidi=NULL -M0
</CsOptions>

<CsInstruments>

; sr set by host
ksmps         =     64
nchnls         =     2
0dbfs        =    1

gisine    ftgen    0,0,4096,10,1    ;A SINE WAVE. USED BY THE LFOs.
massign    0,2
gidetuning    ftgen    0,0,128,21,1,1            ; random array used for fixing unique detune values for each note
giDryMap    ftgen    0,0,4096,7,1,2048,1,2048,0    ; dry mixer control mapping
giWetMap    ftgen    0,0,4096,7,0,2048,1,2048,1    ; wet mixer control mapping
gaSendL,gaSendR    init    0                ; initialise variable used for sending audio between instruments
giImp        ftgen    0,0,4097,9,0.5,1,0        ; shape for the hammer inpulse
gistereo    ftgen    0,0,256,21,1,1            ; random array used for fixing unique stereo values for each note
gidetune    ftgen    0,0,256,21,1,1            ; random array used for fixing unique detune values for each note

giscale    ftgen    0,0,128,-7,-1,128,1
gaSendM,gaSendL,gaSendR    init    0


opcode    Oscil1a,a,iii                    ; an oscillator that plays a single cycle of an audio waveform at a-rate
 iamp,ifrq,ifn    xin
 aptr    line    0,1/ifrq,1
 asig    tablei    aptr,ifn,1
 aenv    linseg    1,1/ifrq,1,0.001,0
    xout    asig*iamp*aenv
endop



instr    1    ; Read in widgets
 iporttime    =    0.05
 gkmod        chnget    "mod"
 gkmod        port    gkmod,iporttime
 gkNdxVel    chnget    "NdxVel"
 gkDur        chnget    "Dur"
 gkFiltFund    chnget    "FiltFund"
 gkNdxCurve    chnget    "NdxCurve"
 gkLPF        chnget    "LPF"
 gkModDep    chnget    "ModDep"
 gkModRte    chnget    "ModRte"
 gkModRteKyb    chnget    "ModRteKyb"
 gkModMix    chnget    "ModMix"
 gkSustain    chnget    "Sustain"
 gkCarKyb    chnget    "CarKyb"
 gkStWidth    chnget    "StWidth"
 gkStWidth    init    0.01
 gkStMix    chnget    "StMix"
 gkDryWet    chnget    "RvbDryWet"
 gkDry        table    gkDryWet,giDryMap,1        ; map dry/wet control
 gkWet        table    gkDryWet,giWetMap,1        ;
 gkRvbSize    chnget    "RvbSize"
 gkDetune    chnget    "Detune"
 gkDtnMix    chnget    "DtnMix"
 gkAmp        chnget    "Amp"
 gkAmp        port    gkAmp,iporttime
 gkAttTim    chnget    "AttTim"
 gkAttVel    chnget    "AttVel"
 gkNseAmp    chnget    "NseAmp"
 gkNseBW    chnget    "NseBW"
 gkNseFllw    chnget    "NseFllw"
endin



instr    2    ; Sound producing instrument. Triggered by MIDI notes.
 icps    cpsmidi
 inum    notnum
 iKybdScl    table    inum,giscale    ; Keyboard scaling value. Note '0' maps to a value of -1, note 64 maps to a value of zero and note 127 maps to a value of 1.

 ; Amplitude Envelope
 idur    =    i(gkDur)
 iAmpCurve    =    -8
 iRelTim    =    (i(gkSustain)==1?idur:0.1)
 iAttVel    veloc    i(gkAttVel)*0.05,0
 iAttTim    =    iAttVel + i(gkAttTim)
 if iAttTim==0 then
  aAEnv    transegr    1,idur,iAmpCurve,0, iRelTim,iAmpCurve,0
 else
  aAEnv    transegr    0,iAttTim,iAmpCurve, 1,idur,iAmpCurve,0, iRelTim,iAmpCurve,0
 endif

 ; Key velocity to amplitude
 iAVel    veloc    0,0.1
 
 ; FM index
 iNdx    veloc    0,i(gkNdxVel)                    ; Key velocity to FM index 
 kIEnv    transeg    iNdx*octave(-iKybdScl*4),idur,-i(gkNdxCurve),0    ; Index envelope. Amplitude influenced by 'Index' widget control and keyboard scaling (index reduced for higher notes, increased for lower notes)
 
 ; Carrier frequency keyboard scaling
 icar    =    1 * octave(iKybdScl*i(gkCarKyb))
 
 ; Carrier ratio modulation
 iModRte    table    inum,giscale
 iModRte    =    octave(iModRte*i(gkModRteKyb))
 kDepEnv    transeg    1,idur,-4,0
 amod    lfo    gkModDep*0.1*kDepEnv, gkModRte * iModRte, 0

 ; Noise bandpass filter cutoff as it respond to the 'Carrier Follow' switch 
 kcf    =    gkNseFllw==1?icps*gkmod:icps
 
 ; Noise amplitude keyboard following
 kNseAmp    =    octave(iKybdScl*-4)*gkNseAmp

 ; Main FM algorithm
 if gkNseAmp>0 then            ; If noise amplitude is greater than zero...
  aNse    gauss    kNseAmp            ; ... generate some gaussian noise
  aNse    butbp    aNse,kcf,kcf*gkNseBW    ; bandpass filter the noise
  aNse    butbp    aNse,kcf,kcf*gkNseBW      ; and again
 else 
 aNse    =    0            ; Otherwise no noise signal!
 endif
 idetune    table    inum,gidetune    ; Read a detuning value from a random table
 asig        foscil    iAVel, icps*cent(-gkDetune*idetune), icar     +aNse, gkmod, kIEnv, gisine    ; FM pair
 aModSig    foscil    iAVel, icps*cent(-gkDetune*idetune), icar+amod+aNse, gkmod, kIEnv, gisine    ; FM pair with modulation
 asig        =    asig + (aModSig * gkModMix)                            ; Mix the two FM pairs.
 
 ; Auxilliary FM algorithm (used for detuning)
 if gkDtnMix>0&&gkDetune>0 then
  if gkNseAmp>0 then
   aNse    gauss    kNseAmp
   aNse    butbp    aNse,kcf,kcf*gkNseBW
   aNse    butbp    aNse,kcf,kcf*gkNseBW  
  else 
   aNse    =    0
  endif
  idetune    table    inum+128,gidetune    ; Read a detuning value from a random table
  asig2        foscil    iAVel, icps*cent(gkDetune*idetune), icar     +aNse, gkmod, kIEnv, gisine    ; Detuning applied to fundemental
  aModSig2    foscil    iAVel, icps*cent(gkDetune*idetune), icar+amod+aNse, gkmod, kIEnv, gisine
  asig2        =    asig2 + (aModSig2 * gkModMix)
  asig        =    asig + asig2*gkDtnMix
 endif
 
 ; Filter Fundemental
 if i(gkFiltFund)==1 then                 ; If filter fundemental switch is active
  asig    butbr    asig,icps*icar,icps*icar*0.3        ; Filter out fundemental
  asig    butbr    asig,icps*icar,icps*icar*0.3        ; and again to make sure it is all gone
 endif
 
 ; Lowpass Filter
 if i(gkLPF)<32 then                    ; If lowpass filter is less than maximum...
  kCF    limit    icps*gkLPF,20,20000            ; ...create the cutoff value in CPS based on fundemental and LPF ratio control. Limit to prevent out of range values. 
  asig    clfilt    asig,kCF,0,2                ; Butterworth lowpass filter
 endif
 
 ; Amplitude Envelope
 asig    =    asig * aAEnv *gkAmp                ; Apply amplitude envelope

 ; Stereo Effect
 iDlyTimL    table    inum,gistereo            ; Read a delay time value from a random table based on note played
 iDlyTimR    table    inum+128,gistereo        ; Read a different delay time value from a random table based on note played
 if gkStWidth>0&&gkStMix>0 then                ; If 'width' is above zero and 'mix' is above zero...
  aL    vdelay    asig, iDlyTimL*gkStWidth*1000, 0.2*1000    ; ...left channel delay
  aR    vdelay    asig, iDlyTimR*gkStWidth*1000, 0.2*1000    ; right channel delay
  aL    ntrpol    asig, aL, gkStMix            ; Mix delayed and dry signal
  aR    ntrpol    asig, aR, gkStMix            ; 
      outs    aL*gkDry,aR*gkDry            ; Send to outputs
  gaSendL    =    gaSendL + aL            ; Send to reverb
  gaSendR    =    gaSendR + aR
 else
     outs    asig*gkDry,asig*gkDry            ; Send to outputs
  gaSendL    =    gaSendL + asig            ; Send to reverb
  gaSendR    =    gaSendR + asig
 endif
endin


instr    99    ; Reverb (always on)
 iRvbHiCut    =    1000
 gaSendL    butlp    gaSendL,iRvbHiCut        ; Remove some high frequencies
 gaSendR    butlp    gaSendR,iRvbHiCut 
 aL,aR    freeverb    gaSendL,gaSendR,gkRvbSize,1
     outs        aL*gkWet*4, aR*gkWet*4
     clear        gaSendL,gaSendR
endin

instr    1000
 gkpreset    chnget    "preset"
 ktrig        changed    gkpreset
 
 #define SET_PRESET(N'Amp'AttTim'AttVel'Dur'Sustain'mod'NdxVel'NdxCurve'CarKyb'DtnMix'Detune'LPF'FiltFund'NseAmp'NseBW'NseFllw'ModMix'ModDep'ModRte'ModRteKyb'StMix'StWidth'RvbDryWet'RvbSize)
 #
 if i(gkpreset)==$N then
  chnset    $Amp      ,"Amp"      
  chnset    $AttTim   ,"AttTim"    
  chnset    $AttVel   ,"AttVel"   
  chnset    $Dur      ,"Dur"
  chnset    $Sustain  ,"Sustain"
  chnset    $mod      ,"mod"  
  chnset    $NdxVel   ,"NdxVel"
  chnset    $NdxCurve ,"NdxCurve"
  chnset    $CarKyb   ,"CarKyb"
  chnset    $DtnMix   ,"DtnMix"
  chnset    $Detune   ,"Detune"   
  chnset    $LPF      ,"LPF"   
  chnset    $FiltFund ,"FiltFund" 
  chnset    $NseAmp   ,"NseAmp" 
  chnset    $NseBW    ,"NseBW"
  chnset    $NseFllw  ,"NseFllw"
  chnset    $ModMix   ,"ModMix"  
  chnset    $ModDep   ,"ModDep"   
  chnset    $ModRte   ,"ModRte"
  chnset    $ModRteKyb,"ModRteKyb"
  chnset    $StMix    ,"StMix"
  chnset    $StWidth  ,"StWidth"
  chnset    $RvbDryWet,"RvbDryWet"
  chnset    $RvbSize  ,"RvbSize"
 endif
 #
 if ktrig==1 then
  reinit SEND_PRESET
 endif
 SEND_PRESET:
 ;           N'Amp'AttTim'AttVel'Dur'Sustain'mod 'NdxVel'NdxCurve'CarKyb'DtnMix'Detune'LPF'FiltFund'NseAmp'NseBW'NseFllw'ModMix'ModDep'ModRte'ModRteKyb'StMix'StWidth'RvbDryWet'RvbSize)
 $SET_PRESET(1'0.5'0     '0     '12 '1      '2.29'1     '16      '0     '1     '1.5   '16 '1       '1000  '0.001'0      '1     '0.1   '3     '0        '0.5  '0.01   '0.3      '0.55   )
 $SET_PRESET(2'0.8'0     '0     '1.7'1      '5.04'0.55  '100     '0     '0     '0     '3.8'0       '0     '0.001'0      '0     '0.1   '3     '0        '0.5  '0.01   '0.1      '0.55   )
 $SET_PRESET(3'0.5'0.1   '0.5   '3.8'1      '1.00'2     '28      '0     '1     '15    '16 '0       '500   '0.0077'1     '1     '0.1   '3     '0        '0.5  '0.01   '0.3      '0.55   )
 $SET_PRESET(4'0.5'0     '0.006 '24 '1      '3.19'1     '15      '0     '0.47  '2.5   '16 '1       '0     '0.001'0      '1     '0.1   '0.33  '1.7      '0.5  '0.01   '0.3      '0.55   )
 $SET_PRESET(5'0.8'0     '1     '6.6'0      '1.12'1     '13      '-2.68 '1     '25    '3.8'0       '0     '0.001'0      '0     '0.1   '3     '0        '1    '0.02   '0.172    '0.21   )
 $SET_PRESET(6'3.25'0.1  '1     '1.28'0     '2.55'0.55  '35      '-2.3  '0     '1.5   '16 '1       '2000  '0.0025'0     '0     '0.1   '3     '0        '0.5  '0.01   '0.68     '0.93   )
 $SET_PRESET(7'4  '0.1   '1     '2.39'1     '2.55'0.55  '35      '-2.3  '0     '1.5   '16 '1       '16    '0.1262'0     '0     '0.1   '3     '0        '0    '0.01   '1.00     '0      )
 $SET_PRESET(8'1  '0     '0     '14.25'1    '6.97'0.55  '35      '-2.3  '0.13  '1.5   '4.78'1      '16    '0.0326'0     '1     '0.1   '3     '0        '0    '0.01   '0.6      '0      )
 $SET_PRESET(9'1.65'0    '0     '1.7'1      '1.20'0.55  '100     '0     '0     '0     '3.8'0       '0     '0.001'0      '0     '0.1   '3     '0        '0.5  '0.01   '0.1      '0.55   )
 $SET_PRESET(10'2'  0    '0     '1  '1      '2.15'0.55  '100     '0     '0.21  '12    '3.8'0       '0     '0.001'0      '0     '0.1   '3     '0        '0.5  '0.01   '0.35     '0.37   )
 $SET_PRESET(11'2  '0.1  '1     '3.44'1     '3.75'0.55  '100     '0     '0.21  '25    '16 '0       '0     '0.001'1      '0     '0.1   '3     '0        '0.5  '0.05   '0        '0.37   )
 $SET_PRESET(12'2  '0.1  '1     '1.45'1     '2.66'1.00  '16      '0     '0.21  '25    '16 '0       '0     '0.001'1      '0     '0.1   '3     '0        '0.5  '0.05   '0.6      '0.37   )
 $SET_PRESET(13'1.9'0    '0     '24  '1     '2.29'1.00  '23.25   '-4    '1     '1.5   '16 '1       '151   '0.0178'1     '1     '0.1   '3     '0        '0.5  '0.01   '0.3      '0.55   )
endin

</CsInstruments>

<CsScore>
i 1 0 3600        ; Read in widgets
i 99 0 [3600*24*7]    ; Reverb instrument
i 1000 0 [3600*24*7]    ; presets
</CsScore>

</CsoundSynthesizer>®