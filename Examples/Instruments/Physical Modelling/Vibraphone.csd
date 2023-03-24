
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; Vibraphone (physical model)

; Iain McCurdy. 2019

; CONTROLS

; V  I  B  R  A  T  O
; Vibrato    - depth and speed of vibrato (also controllable using MIDI modulation wheel
; Depth      - depth of vibrato
; Rate       - rate of vibrato

; M  A  L  L  E  T
; Hardness   - hardness of the beater
; Size       - size of the beater
; Prominence - audibility of the beater noise

; P  E  D  A  L
; Time       - duration of pedal damper impulse
; Noise      - amount of noise that the damper injects into the bars

; B  A  R  S
; Sustain    - sustain time of the bars
; Release    - how quickly bars are damped when the sustain pedal is released
; Level      - output level control (of all sound components)

 ; R  O  O  M (screverb)
 ; Distance  - reverb amount
 ; Size      - reverb size
 ; Colour    - frequency of a low-pass filter within the reverb feedback loop

<Cabbage>
form caption("Vibraphone") size(970, 205), colour(200,200,210), pluginId("Vbes")

image bounds(5,  5, 200, 95), corners(15), outlineColour(50,50,50), outlineThickness(1) shape("square"), colour(255,255,255,100), plant("Vibrato") 
{
label   bounds(  0,  4,190, 12), align("centre"), colour(0,0,0,0), fontColour(50,50,50), text("V  I  B  R  A  T  O")
rslider bounds(  5, 20, 60, 60), range(0, 1, 0, 1, 0.001), channel("Vibrato"), text("Vibrato"), textColour(50,50,50), trackerColour(200,200,150)
rslider bounds( 70, 20, 60, 60) range(0.2, 1, 0.5, 1, 0.001), channel("VDepth"), text("Depth"), textColour(50,50,50), trackerColour(200,200,150)
rslider bounds(135, 20, 60, 60) range(0.5, 11, 4.2, 1, 0.001), channel("VRate"), text("Rate"), textColour(50,50,50), trackerColour(200,200,150)
}

image bounds(210, 5,  200, 95), corners(15), outlineColour(50,50,50), outlineThickness(1) shape("square"), colour(255,255,255,100), plant("Mallet") 
{
label   bounds(  0,  4, 200, 12), align("centre"), colour(0,0,0,0), fontColour(50,50,50), text("M  A  L  L  E  T")
rslider bounds(  5, 20, 60, 60) range(0, 1, 0.75, 1, 0.001), channel("MHardness"), text("Hardness"), textColour(50,50,50), trackerColour(200,200,150)
rslider bounds( 70, 20, 60, 60) range(0, 1, 0.5, 1, 0.001), channel("MSize"), text("Size"), textColour(50,50,50), trackerColour(200,200,150)
rslider bounds(135, 20, 60, 60) range(0, 4, 2, 1, 0.001), channel("MMix"), text("Prominence"), textColour(50,50,50), trackerColour(200,200,150)
}

image bounds(415, 5,  140, 95), corners(15), outlineColour(50,50,50), outlineThickness(1) shape("square"), colour(255,255,255,100), plant("Pedal") 
{
label   bounds(  0,  4, 140, 12), align("centre"), colour(0,0,0,0), fontColour(50,50,50), text("P  E  D  A  L")
rslider bounds( 10, 20, 60, 60) range(0.01, 0.2, 0.1, 1, 0.001), channel("PTime"), text("Time"),   textColour(50,50,50), trackerColour(200,200,150)
rslider bounds( 70, 20, 60, 60) range(0, 4, 1, 1, 0.001), channel("PMix"), text("Noise"), textColour(50,50,50), trackerColour(200,200,150)
}

image bounds(560, 5, 200, 95), corners(15), outlineColour(50,50,50), outlineThickness(1) shape("square"), colour(255,255,255,100), plant("Bars") 
{
label   bounds(  0,  4, 200, 12), align("centre"), colour(0,0,0,0), fontColour(50,50,50), text("B  A  R  S")
rslider bounds(  5, 20, 60, 60) range(-2, 2, 0, 1, 0.001), channel("BSus"), text("Sustain"),   textColour(50,50,50), trackerColour(200,200,150)
rslider bounds( 70, 20, 60, 60) range(0.005, 0.05, 0.01, 0.5, 0.001), channel("BRel"), text("Release"), textColour(50,50,50), trackerColour(200,200,150)
rslider bounds(135, 20, 60, 60) range(0, 2, 1, 0.5, 0.001), channel("BLev"), text("Level"), textColour(50,50,50), trackerColour(200,200,150)
}

image bounds(765, 5, 200, 95), corners(15), outlineColour(50,50,50), outlineThickness(1) shape("square"), colour(255,255,255,100), plant("Bars") 
{
label   bounds(  0,  4,200, 12), align("centre"), colour(0,0,0,0), fontColour(50,50,50), text("R  O  O  M")
rslider bounds(  5, 20, 60, 60) range(0, 1, 0.6, 1, 0.001), channel("RDist"), text("Distance"),   textColour(50,50,50), trackerColour(200,200,150)
rslider bounds( 70, 20, 60, 60) range(0.3, 0.7, 0.4, 1, 0.001), channel("RSize"), text("Size"),   textColour(50,50,50), trackerColour(200,200,150)
rslider bounds(135, 20, 60, 60) range(500, 8000, 3000, 0.5, 0.001), channel("RColour"), text("Colour"),   textColour(50,50,50), trackerColour(200,200,150)
}

keyboard bounds(  5,105,960, 95)

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

; initial position of controller
initc7 1, 71, 0.75 ; mallet hardness

; UDO that defines a single mode partial (recursive according to members in function table)
opcode ModePartial,a,aiikio
aImp, iCPS, iBSus, kSus, iRatios, iCount xin         ; read in variables
iCPSL    =         iCPS * table:i(iCount,iRatios)    ; derive freq value for this partial
iQL      =         table:i(iCount,iRatios+1)*2       ; derive Q value for this partial
idBL     =         table:i(iCount,iRatios+2)         ; derive dB value for this partial

aMode    mode      aImp*ampdbfs(idBL), iCPSL, iQL * kSus * iBSus    ; create mode filter
if iCount < ftlen(iRatios) then                                     ; if more partials are needed...
 aMix    ModePartial aImp, iCPS, iBSus, kSus, iRatios, iCount+1     ; conditionally call UDO again
endif
         xout      aMode + aMix                                     ; mix subsequent partials with partials form this recursion and send back to caller instrument
endop

; function tables containing modal ratios, Q values and dB values
; Must be in this order: Ratios, Qs, Amps(dBs)
; Must all contain the same number of members
giVibesRatios ftgen  0, 0, 0, -2, 174.614/174.614, 523.842/174.614, 698.456/174.614, 1966/174.614, 3477/174.614, 3687/174.614, 4641/174.614, 5818/174.614, 6651/174.614, 8888/174.614, 11387/174.614
giVibesQs     ftgen  0, 0, 0, -2, 3000, 3000, 2000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000
giVibesdBs    ftgen  0, 0, 0, -2, -15, -30, -18, -11, -25, -19, -20, -22, -25, -26, -27

; damper pedal envelope
giEnv         ftgen  0, 0, 4096, 16, 0, 128, 4, 1, 4096-128, -4, 0

instr 1

iCPS  cpsmidi                       ; read in MIDI note pitch in cycles per second 
iVel  ampmidi 1                     ; read in MIDI velocity

; Mallet
iMalletMix chnget "MMix"                          ; mix of mallet sound in output
iVel   =       iVel * 0.8 + 0.2                   ; map velocity range
iMSize =       2 ^ ( (chnget:i("MSize") * 2) - 1) ; read in mallet size widget and remap the value
kEnv   expon   1, 0.01 * iMSize, 0.001            ; a quickly decaying amplitude envelope for the impulse sound
aImp   rand    kEnv * iVel ^ 2 * 0.3 * ampdbfs(rnd:i(3)) ; generate some noise
aImp   buthp   aImp, iCPS * 0.5                   ; remove some low frequencies
iMHardness chnget "MHardness"                     ; read in variable for mallet hardness
iCF        =      (iMHardness*5) + 7.5 + rnd:i(0.5)              ; lowpass filter cutoff frequency for mallet sound, controls nature of mallet/beater
aImp       butlp  aImp * ampdbfs((1-iMHardness)*16), cpsoct(iCF) * iVel ^ 3    ; remove high frequencies according to the note velocity 

; Pedal Damper Noise
kRMS init 0                               
if trigger:k(release:k(),0.5,0)==1 then
 kRelRMS   =      kRMS
endif
iPTime    chnget   "PTime"
iPMix     chnget   "PMix"
aRelIndx  linsegr  0, iPTime, 1
aRelEnv   tablei   aRelIndx, giEnv, 1, 0, 0
aRel      rand     aRelEnv
aRel      butlp    aRel, 200
aRel      buthp    aRel, 80
aRel      *=       kRelRMS * iPMix

; Bars
iBSus     chnget   "BSus"
iBSus     =        2^iBSus
iBRel     chnget   "BRel"
kBLev     chnget   "BLev"
kSus      linsegr  1, 0.1, iBRel                                ; damping upon note release
xtratim (174.614*iBRel*iBSus)/(iCPS*0.01) + 0.1                 ; extend duration to allow note releases to complete
; call mode partials... (a recursive UDO)
aMix  ModePartial aImp + aRel, iCPS, iBSus, kSus, giVibesRatios
aMix      +=       aImp * iMalletMix                            ; mix in the mallet sound
aMix      *=       a(kBLev)
kRMS      rms      aMix

; slightly pan notes according to their note number
iPan      =        (notnum() / 127) * 0.7 + 0.15
aL        =        aMix * (1 - iPan)
aR        =        aMix * iPan

; send some signal to the reverb instrument
          chnmix   aL, "SendL"                                  ; some some signal to the reverb instrument using a named 'channel'
          chnmix   aR, "SendR"

endin

instr 2   ; vibrato, reverb and audio output
; read in sent audio
aL       chnget    "SendL"
aR       chnget    "SendR"
; clear channels
         chnclear  "SendL"
         chnclear  "SendR"
     
; modulation controller (mod wheel)
kVibrato chnget    "Vibrato"
kVDepth  chnget    "VDepth"
kVRate   chnget    "VRate"

kMod   ctrl7     1, 1, 0, 1
if changed:k(kMod)==1 then
 chnset kMod, "Vibrato"
endif
; smooth changes made to the mod wheel
kVibrato port      kVibrato, 0.05, 0
; modulation (Vibrato function
aMod     poscil    kVibrato, kVRate*kVibrato
aTrm     =         (aMod*kVDepth) + 1
aL       *=        aTrm
aR       *=        aTrm

; Doppler
kDep     =         0.0001
aDop     =         (aMod*kDep) + kDep + ksmps/sr

aBufL    delayr    ksmps/sr + 0.001
aTapL    deltapi   aDop
         delayw    aL

aBufR    delayr    ksmps/sr + 0.001
aTapR    deltapi   aDop
         delayw    aR

         outs      aTapL, aTapR

; reverb
kRDist   chnget "RDist"
kRSize   chnget "RSize"
kRColour chnget "RColour"

aL,aR    reverbsc  aTapL * kRDist, aTapR * kRDist, kRSize, kRColour    ; stereo reverb
         outs      aL, aR

endin

</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
i 2 0 z
</CsScore>
</CsoundSynthesizer>