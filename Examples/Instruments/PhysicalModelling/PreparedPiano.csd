
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; PreparedPiano.csd
; Written by Iain McCurdy, 2016

; This instrument is an encapsulation of the 'prepiano' opcode, an emulation of a prepared piano string modified with a damping piece of rubber and a rattling element (e.g. a bolt) in loose contact with the string.

; This physical model emplys complex mathematics and therefore places a high demand upon the CPU. It will be unrealistic to expect more that a few notes of polyphony being possible.
; Increasing the 'Num.Strings' paramter increases the CPU demand per note as do the addition of rattles and rubbers.

; The mathematical output can extremely dynamic depending on the input values used. Put simply, the audio output can overload and explode depending on the input paramters chosen.
; A gating mechanism is implemented to remove the worst effects of exploding amplitudes but care should nonetheless be employed.

; CONTROLS
; --------
; Num. Strings     -    the number of strings struck as a cluster by the hammer. (NB. this setting is less apparent if 'Detuning' is zero.)
;                       i.e. in a traditional piano design each hammer will string between 1 and 3 strings. This is done in order to thicken the sound produced.
; Detuning        -    detuning between the strings struck by each hammer
; Stiffness       -    stiffness of the strings. As this is increased the sound produced becomes more inharmonic.
; Duration        -    decay time - time for the sound level to decay by 30dB.
; Release         -    release time (after a MIDI key is released).
; Hi.Freq.Loss    -    rate at which high frequencies decay.
; Upper Lim.      -    note number limit above which notes will not sound. This can be useful in removing higher notes that explode from an instrument setup.
; Hamm.Mass       -    mass of the hammer. This equates to amplitude and is useful in compensating for power gains and losses caused by changes made to other parameters. This value is also scaled by MIDI key velocity.
; Hamm.Freq.      -    frequency (brightness) of the striking hammer. This value is also scaled by MIDI key velocity.
; Strike.Pos.     -    position along the length of the string at which the hammer strikes
; String.Vel.     -    normalised string velocity
; Scan Freq       -    rate at which the pickup (listening) point oscillates
; Scan Spread     -    spread of phases of scanning points across the different strings. (The effects of this are most apparent if 'Scan Freq.' is low

; Left B.C.       -    method by which the left end of each string is fixed: 'fixed', 'pivoting' or 'free'
; Right B.C.      -    method by which the right end of each string is fixed: 'fixed', 'pivoting' or 'free'

; Rattle
; On/Off          -    whether a rattle is present or not on each string
; Position        -    location of the rattle along the length of the string
; Mass Dens.Ratio -    Ratio between the density of the rattle and the frequency at which it rattles. In general if this parameter is high, rattle frequency should be low.
; Frequency       -    Frequency of the rattling. In general if this parameter is high, 'Mass Dens.Ratio' should be low.
; Length          -    Length of the rattle.

; Rubber
; On/Off          -    whether a rubber is present or not on each string
; Position        -    location of the rubber along the length of the string.
; Mass Dens.Ratio -    Ratio between the density of the rubber and the frequency at which it damps In general if this parameter is high, rubber damping frequency should be low.
; Frequency       -    Frequency of the rubber. In general if this parameter is high, 'Mass Dens.Ratio' should be low.
; Loss            -    rate of energy loss induced by the rubber.

<Cabbage>
form caption("Prepared Piano") size(795,245), colour(250,235,255), pluginId("PreP")
nslider  bounds(  5,5,70,34), channel("NS"),    range(2, 50, 3,1,1),       textColour(black), fontColour(black), text("Num.Strings"), colour(white)
nslider  bounds( 85,5,70,34), channel("D"),    range(0,20000, 0),       textColour(black), fontColour(black), text("Detuning"), colour(white)
nslider  bounds(165,5,70,34), channel("K"),    range(1, 40, 1),       textColour(black), fontColour(black), text("Stiffness"), colour(white)
nslider  bounds(245,5,70,34), channel("T30"),   range(0.1, 30, 12),       textColour(black), fontColour(black), text("Decay"), colour(white)
nslider  bounds(325,5,70,34), channel("rel"),      range(0.001,8,0.1,1,0.001),       textColour(black), fontColour(black), text("Release Time"), colour(white)
nslider  bounds(405,5,70,34), channel("B"),        range(0.0001, 1, 0.002,1,0.0001),       textColour(black), fontColour(black), text("Hi.Freq.Loss"), colour(white)
nslider  bounds(485,5,70,34), channel("UpperLim"),range(0,127,127,1,1),       textColour(black), fontColour(black), text("Upper Lim."), colour(white)

nslider  bounds(  5, 45, 70, 34), channel("mass"),    range(0.0001,1000,1,1,0.0001),       textColour(black), fontColour(black), text("Hamm.Mass."), colour(white)
nslider  bounds( 85, 45, 70, 34), channel("freqH"),   range(1,4000000,20000.1,1,1),       textColour(black), fontColour(black), text("Hamm.Freq."), colour(white)
nslider  bounds(165, 45, 70,34), channel("pos"),    range(0.004,0.996,0.09),       textColour(black), fontColour(black), text("Strike Pos."), colour(white)
nslider  bounds(245, 45, 70,34), channel("vel"),   range(0,1000, 50),       textColour(black), fontColour(black), text("String Vel."), colour(white)
nslider  bounds(325, 45, 70, 34), channel("sfreq"),    range(0,100,0,1,0.001),       textColour(black), fontColour(black), text("Scan Freq"), colour(white)
nslider  bounds(405, 45, 70, 34), channel("sspread"),  range(0,1,0.1,1,0.001),       textColour(black), fontColour(black), text("Scan Spread"), colour(white)
nslider  bounds(485, 45, 70, 34), channel("OctShift"),range(-4,0,0,1,1),       textColour(black), fontColour(black), text("Oct.Shift"), colour(white)

label      bounds(570, 10, 70, 12), text("Left B.C."), fontColour("black")
checkbox   bounds(570, 28, 70, 14), text("Clamped"),  channel("BCL1"), fontColour:0("black"), fontColour:1("black"), colour("yellow"), value(0), radioGroup(1)
checkbox   bounds(570, 43, 70, 14), text("Pivoting"), channel("BCL2"), fontColour:0("black"), fontColour:1("black"), colour("yellow"), value(1), radioGroup(1) 
checkbox   bounds(570, 58, 70, 14), text("Free"),     channel("BCL3"), fontColour:0("black"), fontColour:1("black"), colour("yellow"), value(0), radioGroup(1)

label      bounds(650, 10, 70, 12), text("Right B.C."), fontColour("black")
checkbox   bounds(650, 28, 70, 14), text("Clamped"),  channel("BCR1"), fontColour:0("black"), fontColour:1("black"), colour("yellow"), value(0), radioGroup(2)
checkbox   bounds(650, 43, 70, 14), text("Pivoting"), channel("BCR2"), fontColour:0("black"), fontColour:1("black"), colour("yellow"), value(1), radioGroup(2) 
checkbox   bounds(650, 58, 70, 14), text("Free"),     channel("BCR3"), fontColour:0("black"), fontColour:1("black"), colour("yellow"), value(0), radioGroup(2)

label      bounds(740, 10, 40, 12), text("Preset"), fontColour("black")
combobox   bounds(740, 28, 40, 15), channel("preset"), text("1","2","3","4","5","6","7","8","9","10"),  textColour("white"), fontColour("white"), colour("black")

image     bounds(  5, 90,390, 65), colour(0,0,0,0), outlineColour(0,0,0), outlineThickness(1), plant("rattle1") {
label     bounds(  0,  2,390, 15), fontColour("black"), text("Rattle") 
checkbox  bounds( 10,  7, 60, 11), text("On/Off"), channel("rattle1"), fontColour:0("black"), fontColour:1("black"), colour("yellow"), value(1)
nslider bounds( 10, 22, 85, 34), channel("posRat1"), range(0, 1, 0.6),               textColour(black), fontColour(black), text("Position"), colour("white")
nslider bounds(105, 22, 85, 34), channel("MDRRat1"), range(0,10000,1000,1,0.01),     textColour(black), fontColour(black), text("Mass Dens.Ratio"), colour(white)
nslider bounds(200, 22, 85, 34), channel("FrqRat1"), range(0.1,10000,30,1,0.1),      textColour(black), fontColour(black), text("Freq."), colour(white)
nslider bounds(295, 22, 85, 34), channel("LenRat1"), range(0.001,1, 0.001,1,0.001),  textColour(black), fontColour(black), text("Length"), colour("white")
}

image     bounds(400, 90,390, 65), colour(0,0,0,0), outlineColour(0,0,0), outlineThickness(1), plant("rubber1") {
label     bounds(  5,  2,390, 15), fontColour("black"), text("Rubber") 
checkbox  bounds( 10,  7, 60, 11), text("On/Off"), channel("rubber1"), fontColour("black"), colour("yellow"), value(1)
nslider bounds( 10, 22, 85, 34), channel("posRub1"), range(0, 1, 0.7),           textColour(black), fontColour(black), text("Position"), colour("white")
nslider bounds(105, 22, 85, 34), channel("MDRRub1"), range(0,10000,1000,1,0.01), textColour(black), fontColour(black), text("Mass.Dens.Ratio"), colour(white)
nslider bounds(200, 22, 85, 34), channel("FrqRub1"), range(0.1,10000,40,1,0.1),  textColour(black), fontColour(black), text("Freq."), colour(white)
nslider bounds(295, 22, 85, 34), channel("LosRub1"), range(0,10000, 1000,1,1),   textColour(black), fontColour(black), text("Loss"), colour("white")
}

keyboard  bounds( 0,165,800,80)
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-dm0 -n -+rtmidi=NULL -M0
</CsOptions>

<CsInstruments>

; sr set by host
ksmps         =     32        ;NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls         =     2        ;NUMBER OF CHANNELS (2=STEREO)
0dbfs        =    1
massign    0,2                ;ALL MIDI TO INSTR 2

gkinit        init    -0.01
gkMuteThresh    init    0.9

giVelCurve    ftgen    0,0,128,-16,0,128,4,1    ; VELOCITY CURVE. ADJUST CURVE VALUE ACCORDING TO THE HARDWARE KEYBOARD USED!

instr    1    ; READ IN WIDGETS
 krattle1    chnget    "rattle1"
 kposRat1    chnget    "posRat1"
 kMDRRat1    chnget    "MDRRat1"
 kFrqRat1    chnget    "FrqRat1"
 kLenRat1    chnget    "LenRat1"

 krubber1    chnget    "rubber1"
 kposRub1    chnget    "posRub1"
 kMDRRub1    chnget    "MDRRub1"
 kFrqRub1    chnget    "FrqRub1"
 kLosRub1    chnget    "LosRub1"

 if changed(krattle1,kposRat1,kMDRRat1,kFrqRat1,kLenRat1,krubber1, kposRub1,kMDRRub1,kFrqRub1,kLosRub1)==1 then
  reinit RebuildTables
 endif
 RebuildTables:
 girattles    ftgen    1, 0, -5, -2, i(krattle1), i(kposRat1), i(kMDRRat1), i(kFrqRat1), i(kLenRat1)
 girubbers    ftgen    2, 0, -5, -2, i(krubber1), i(kposRub1), i(kMDRRub1), i(kFrqRub1), i(kLosRub1)
 rireturn
 
 ; PRESETS
 kpreset    chnget    "preset"
 #define    SEND_PRESET(N'NS'D'K'T30'rel'B'UpperLim'mass'freqH'pos'vel'sfreq'sspread'OctShift'BCL1'BCL2'BCL3'BCR1'BCR2'BCR3'rattle1'posRat1'MDRRat1'FrqRat1'LenRat1'rubber1'posRub1'MDRRub1'FrqRub1'LosRub1)
 #
 if kpreset==$N then
     chnset    k($NS),"NS"
     chnset    k($D),"D"
     chnset    k($K),"K"
     chnset    k($T30),"T30"
     chnset    k($rel),"rel"
     chnset    k($B),"B"
     chnset    k($UpperLim),"UpperLim"
     chnset    k($mass),"mass"
     chnset    k($freqH),"freqH"
     chnset    k($pos),"pos"
     chnset    k($vel),"vel"
     chnset    k($sfreq),"sfreq"
     chnset    k($sspread),"sspread"
     chnset    k($OctShift),"OctShift"
     chnset    k($BCL1),"BCL1"
     chnset    k($BCL2),"BCL2"
     chnset    k($BCL3),"BCL3"
     chnset    k($BCR1),"BCR1"
     chnset    k($BCR2),"BCR2"
     chnset    k($BCR3),"BCR3"
     chnset    k($rattle1),"rattle1"
     chnset    k($posRat1),"posRat1"
     chnset    k($MDRRat1),"MDRRat1"
     chnset    k($FrqRat1),"FrqRat1"
     chnset    k($LenRat1),"LenRat1"
     chnset    k($rubber1),"rubber1"
     chnset    k($posRub1),"posRub1"
     chnset    k($MDRRub1),"MDRRub1"
     chnset    k($FrqRub1),"FrqRub1"
     chnset    k($LosRub1),"LosRub1"
 endif
 #
 if changed(kpreset)==1 then
  ;            preset N.Str|Detn|Stiff|Decay|Rel|Hi.F.Loss|Uppr Lim.|Hamm.Mass|Hamm.Freq.|Str.Pos.|Str.Vel.|Scan.Frq.|Scan.Sprd.|OctShift|BC.L.1|BC.L.2|BC.L.3|BC.R.1|BC.R.2|BC.R.3|rattle|posRat|MDRRat|FrqRat|LenRat|rubber|posRub|MDRRub|FrqRub|LosRub
  $SEND_PRESET(1'     3'    0'   1'    12'   0.1'0.002'    127'      1'        800000'    0.09'    50'      0'        0.1'       0'       0'     1'     0'     0'     1'     0'     1'     0.6'   1000'  30'    0.001' 1'     0.7'   1000'  40'    1000)
  $SEND_PRESET(2'     3'    10'  23'   12'   0.1'0.002'    127'      1'        80000'     0.06'    50'      0.5'      1'         0'       0'     1'     0'     0'     1'     0'     0'     0.6'   1000'  30'    0.001' 0'     0.7'   1000'  40'    1000)
  $SEND_PRESET(3'     2'    20'  3'    3'    0.1'0.6'      127'      5'        5000'      0.06'    50'      0.05'     1'         0'       0'     1'     0'     0'     0'     1'     1'     0.1'   20'    3000'  0.6'   0'     0.7'   1000'  40'    1000)
  $SEND_PRESET(4'     2'    48'  5'    1.6'  0.3'0.2'      127'      10'       500'       0.06'    25'      0'        1'         0'       0'     1'     0'     0'     0'     1'     1'     0.6'   10000' 30'    0.01'  0'     0.7'   1000'  40'    1000)
  $SEND_PRESET(5'     3'    0'   8'    12'   0.1'0.011'    127'      5'        40000'     0.09'    10'      0'        0.1'       0'       0'     0'     1'     0'     0'     1'     0'     0.6'   1000'  30'    0.001' 1'     0.057' 1000'  40'   10000)
  $SEND_PRESET(6'     3'    0'   40'   12'   0.1'0.0097'   127'      10'       100000'    0.09'    7'       0'        0.1'       0'       0'     1'     0'     0'     1'     0'     0'     0.6'   1000'  30'    0.001' 1'     0.866' 223'  107'   10000)
  $SEND_PRESET(7'     3'    0'   8'    12'   0.1'0.005'    127'      15'       1000000'   0.00'    10'      0'        0.1'       0'       0'     0'     1'     0'     0'     1'     0'     0.6'   1000'  30'    0.001' 1'     0.057' 50'   300'    1000)
  $SEND_PRESET(8'     3'    0'   1'    12'   0.1'0.002'    127'      50'       80000'     0.05'    20'      0'        0.1'       0'       0'     0'     1'     0'     0'     1'     1'     0.6'   5'     10'    0.244' 1'     0.822' 10'   300'    1440)
  $SEND_PRESET(9'     3'    14'  5'     8'   0.1'0.016'    127'      4'        80000'     0.05'    20'      0'        0.1'       0'       1'     0'     0'     1'     0'     0'     0'     0.6'   1000'  30'    0.001' 1'     0.89'  5'   1296'    1440)
  $SEND_PRESET(10'    3'    0'   35'   12'   0.1'0.1'      127'      1'        40000'     0.09'    50'      0'        0.1'       0'       0'     1'     0'     0'     1'     0'     1'     0.6'   0.5'   506'   0.001' 1'     0.7'   1000'  40'    1000)
 endif

endin

instr    2
 inum        notnum
 iOctShift    chnget    "OctShift"
 inum        -=    inum - (iOctShift*12)
 iUpperLim    chnget    "UpperLim"
 if inum>iUpperLim then
  turnoff
  igoto    END
 endif
 iNS        chnget    "NS"
 iD            chnget    "D"
 iK            chnget    "K"
 iT30        chnget    "T30"
 iB            chnget    "B"
 isfreq        chnget    "sfreq"
 isspread    chnget    "sspread"
 irel        chnget    "rel"
 ifreqH        chnget    "freqH"
 imass        chnget    "mass"
 ipos        chnget    "pos"
 ivel        chnget    "vel"
 iBCL1        chnget    "BCL1"
 iBCL2        chnget    "BCL2"
 iBCL3        chnget    "BCL3"
 iBCR1        chnget    "BCR1"
 iBCR2        chnget    "BCR2"
 iBCR3        chnget    "BCR3"
 iBCL        =        iBCL1+(iBCL2*2)+(iBCL3*3)
 iBCR        =        iBCR1+(iBCR2*2)+(iBCR3*3)

 ifreq        cpsmidi
 ifreq        *=    octave(iOctShift)
 iMIDIvel    ampmidi    1,giVelCurve
     
 ;OUTPUTS     OPCODE        BASE-FREQ | NUM_OF_STRINGS | DETUNING | STIFFNESS | 30 DB DECAY TIME | HIGH_FREQUENCY_LOSS | LEFT_BOUNDARY_CONDITION | RIGHT_BOUNDARY_CONDITION |  HAMMER_MASS   | HAMMER_FREQUENCY | HAMMER_INITIAL_POSITION | POSITION_ALONG_STRING | HAMMER_VELOCITY  | SCANNING_FREQ | SCANNING_FREQ_SPREAD | RATTLES_FUNCTION_TABLE | RUBBERS_FUNCTION_TABLE             
 al,ar        prepiano     ifreq,           iNS,           iD,        iK,            iT30,               iB,                    iBCL,                     iBCR,             imass*iMIDIvel,    ifreqH*iMIDIvel,          i(gkinit),                ipos,            ivel*iMIDIvel,     isfreq,            isspread,              girattles,               girubbers

 al    dcblock2    al
 ar    dcblock2    ar
    
 aenv    linsegr        1,irel,0

 ; EMERGENCY GATE - CUTS AUDIO OUT COMPLETELY IF prepiano STARTS TO EXPLODE ON ACCOUNT OF A PARTICULAR COMBINATION OF INPUT PARAMETERS
 krms    rms    al            ; scan RMS of left channel
 kgate    =    krms>i(gkMuteThresh)?0:1    ; gate 'on' or 'off' (1 or zero) depending on rms value
 ;if trigger(kgate,0.5,0)==1 then
 ; turnoff
 ;endif
 agate    interp    kgate            ; interpolate to a-rate. agate will ramp from 1 to zero (or vice versa) across the duration of ksmps. This will be a bit smoother and less 'clicky' than a simple zero to 1 switch (i.e. if using 'upsamp')
 ktrig    changed    kgate            ; trigger for updating FLTK indicator
 krel    release                ; sense release
 if krel==1 then                ; always deactivate indicator when note is released
 endif
    
 al    =    al * aenv * agate
 ar    =    ar * aenv * agate
    outs         al,ar
 END:
endin

</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
</CsScore>


</CsoundSynthesizer>