
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; Flutter Chorus
; Iain McCurdy. 2021.

; Modulates delay time (pitch warping) and amplitude iteratively as delays regenerate
; This means that the longer a sound echoes within the delay, the more pitch and/or amplitude modulated it becomes. 

; Input Gain      - gain control on the audio signal entering the delay
; Delay Time      - delay time in seconds
; Feedback        - ratio of signal leaving the delay that is fed back into the input
; Damping         - cutoff frequency of a 6db/oct lowpass filter within the delay loop
; Cycles          - number of pitch/amplitude modulations within the delay time
; Pitch Mod.Depth - amount of pitch modulation
; Amp.mod.Depth   - amount of amplitude modulation
; Phase OS        - phase offset of the right channel. 0 to 180 degrees. This is used to create a stereo effect.
; Dry/Wet         - mix between dry and wet signal

<Cabbage>
form caption("Flutter Chorus") size(1020,120), colour(0,0,0), pluginId("FlCh"), guiRefresh(16)	; guiRefresh value adjusts how often the cabbage valuators are read. This can affect the usability of parameters used in realtime gesturing. style("legacy")

#define RSliderStyle trackerInsideRadius(.8), trackerOutsideRadius(1), trackerColour(200,200,200), colour( 50, 60, 80), fontColour(200,200,200), textColour(200,200,200),  markerColour(220,220,220), outlineColour(50,50,50), valueTextBox(1)

rslider     bounds(  0, 15, 90, 90), channel("InputGain"), text("Input Gain"), range( 0, 1, 0.5, 0.5), $RSliderStyle
rslider     bounds( 90, 15, 90, 90), channel("DelTime"), text("Delay Time"), range( 0.01, 4, 0.4, 0.5), $RSliderStyle
rslider     bounds(180, 15, 90, 90), channel("Feedback"), text("Feedback"), range( 0, 0.98, 0.9), $RSliderStyle
rslider     bounds(270, 15, 90, 90), channel("Damping"), text("Damping"), range( 1000, 20000, 20000, 0.5, 1), $RSliderStyle
rslider     bounds(360, 15, 90, 90), channel("Cycles"), text("Cycles"), range( 1, 32, 1, 1, 1), $RSliderStyle
rslider     bounds(450, 15, 90, 90), channel("PitchModDepth"), text("Pitch Mod. Depth"), range( 0.0000, 0.1, 0.002, 0.5, 0.0001), $RSliderStyle
rslider     bounds(540, 15, 90, 90), channel("AmpModDepth"), text("Amp.Mod.Depth"), range( 0, 12, 0), $RSliderStyle
rslider     bounds(630, 15, 90, 90), channel("PhaseOS"),   text("Phase OS"), range( 0, 0.5, 0.5), $RSliderStyle
rslider     bounds(720, 15, 90, 90), channel("DryWetMix"), text("Dry/Wet"), range( 0, 1, 0.5), $RSliderStyle

image       bounds(820, 40, 100, 40), colour(200,200,200), identChannel("SpinnerID"), shape("ellipse")

label       bounds(915, 36, 90, 15), text("Pitch Shape") 
combobox    bounds(915, 50, 90, 20), channel("PitchShape"), items("Sine","Tri.","Exp","Log"), value(1)

</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-n -+rtmidi=NULL -M0 -dm0
</CsOptions>

<CsInstruments>

nchnls = 2
0dbfs  = 1

opcode FlutterChorus,a,aaakkki
aIn, aDelTim, aAmpMod, kFeedback, kDamping, kDryWetMix, iMaxDel  xin
; delay buffer
aIgnore         delayr     iMaxDel
aTap            deltapi    aDelTim
aTap            *=         aAmpMod
aTap            tone       aTap, kDamping
                delayw     aIn + (aTap * kFeedback)
aMix            ntrpol     aIn, aTap, kDryWetMix
xout aMix
endop

instr   1

aInL, aInR      ins

kCycles         chnget     "Cycles"
kPitchModDepth  chnget     "PitchModDepth"
kAmpModDepth    chnget     "AmpModDepth"
kDelTime        chnget     "DelTime"
kFeedback       chnget     "Feedback"
kDamping        chnget     "Damping"
kInputGain      chnget     "InputGain"
kDryWetMix      chnget     "DryWetMix"
kPitchShape     chnget     "PitchShape"
kPhaseOS        chnget     "PhaseOS"

kPortTime       linseg     0,0.01,0.1
kDelTime        portk      kDelTime, kPortTime
aDelTime        interp     kDelTime

aInL            *=          a(kInputGain)
aInR            *=          a(kInputGain)

iSin            ftgen      1,0,4096,10,1
iTri            ftgen      2,0,4096,-7,-1,2048,1,2048,-1
iExp            ftgen      3,0,4096,9,0.5,1,0
iLog            ftgen      4,0,4096,9,0.5,1,180

kPitchShape     init       1
kPhaseOS        init       0.5

#define FLUTTER_CHORUS(CHAN'INIT_PHASE)
#

; display spinner
kFreq = 1 / kDelTime
iTri  ftgen  0,0,1024,7,0,512,1,512,0
kPhs  oscil 1,kFreq,iTri
kCol  =      kPhs*255
Smsg sprintfk "pos(%d,20), size(%d,80), colour(%d,%d,%d)", 820 + (1-kPhs)*40, kPhs*80, kCol, kCol, kCol
     chnset   Smsg, "SpinnerID"

if changed:k(kPitchShape,kPhaseOS)==1 then
 reinit RESTART_LFO$CHAN
endif
RESTART_LFO$CHAN:

; delay time (pitch) jitter
aTimeMod        oscili     (kPitchModDepth * kDelTime) / kCycles, kCycles / kDelTime, i(kPitchShape), i(kPhaseOS) * $INIT_PHASE
     
; amplitude modulation
iAmpDep         =          0
aAmpMod         oscili     kAmpModDepth / 2, kCycles / kDelTime, iTri, i(kPhaseOS) * $INIT_PHASE
aAmpMod         -=         kAmpModDepth / 2

rireturn

iMaxDel         = 5
aAmpMod         =             ampdbfs(aAmpMod)
aOut            FlutterChorus aInL, aDelTime + aTimeMod, aAmpMod, kFeedback, kDamping, kDryWetMix, iMaxDel
                outch         $CHAN, aOut       
#

$FLUTTER_CHORUS(1'0)
$FLUTTER_CHORUS(2'1)

endin

</CsInstruments>

<CsScore>
i 1 0 [3600*24]
</CsScore>

</CsoundSynthesizer>
