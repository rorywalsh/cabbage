
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; Spring Reverb
; Iain McCurdy, 2021

; I N P U T
; INPUT - how the stereo input is matrixed into the effect:
;  1. Left channel only (send to both outputs)
;  2. Sum L&R (left and right channels are mixed then sent into a mono spring reverb, same signal sent to both outputs)
;  3. Dual Stereo (dual spring reverbs process left and right channels independently)

; D E L A Y S
; The three main echos/springs (in parallel) that comprise the reverb.
; Times can be adjusted independently and each spring can be activated or deactivated.
; Hardware spring reverbs typically contain 2 or 3 springs.
; Feedback - ratio of the signal that is fed back into the input (applies to all three delays/springs)

; O U T P U T
; Mix - mix between the dry and wet signal

; F E E D B A C K   F I L T E R
; The spectral envelope of a filter applied to the signal through the springs and applied iteratively via the feedback loop
; This is an envelope with two break-points and three segments. 
; The curvature of the first two segments can be warped.
; The third segment functions as a simple lowpass/highcut filter

; B O I N G
; A key characteristic of the spring reverb is the manner in which it spectrally smears the sound 
;   as different frequencies are delayed by differring amounts as they move up and down the springs.
; Time  - time of the smear between 0Hz  to the Nyquist frequency (sr/2).
; Invert - flips the direction of the smear (from low-to-high to high-to-low) 
; Shape - shape of the smear, concave to convex

<Cabbage>
#define RSLIDER_ATTRIBUTES trackerInsideRadius(.85), trackerOutsideRadius(1), colour( 80, 80, 100), trackerColour("silver"), textColour("silver"), markerColour("white")
form caption("Spring Reverb") size(650,330), colour( 20, 20, 25) pluginId("Sprg"), guiMode("queue") ;, guiRefresh(10)

; INPUT
groupbox  bounds( 10, 10,220,110), text("I N P U T") {
label     bounds( 10, 40, 75, 13), text("INPUT")
combobox  bounds( 10, 55, 75, 20), text("L","Sum L&R","Dual Stereo"), channel("Input"), value(2)
rslider   bounds( 92, 42, 56, 56), text("Low Cut"), channel("LoCut"), range(20,2000,50,0.5,1) $RSLIDER_ATTRIBUTES
rslider   bounds(152, 42, 56, 56), text("High Cut"), channel("HiCut"), range(100,15000,7000,0.5,1) $RSLIDER_ATTRIBUTES
}

; DELAYS
groupbox  bounds(240, 10,290,110), text("D E L A Y S") {
checkbox  bounds( 10, 27, 60, 10), text("ACTIVE"), channel("Active1"), colour:0(50,50,0), colour:1(250,250,100), value(1)
checkbox  bounds( 80, 27, 60, 10), text("ACTIVE"), channel("Active2"), colour:0(50,50,0), colour:1(250,250,100), value(1)
checkbox  bounds(150, 27, 60, 10), text("ACTIVE"), channel("Active3"), colour:0(50,50,0), colour:1(250,250,100)
rslider   bounds( 10, 40, 60, 60), text("Time 1"), channel("Time1"), range(0.001,1,0.045,0.5,0.001) $RSLIDER_ATTRIBUTES
rslider   bounds( 80, 40, 60, 60), text("Time 2"), channel("Time2"), range(0.001,1,0.053,0.5,0.001) $RSLIDER_ATTRIBUTES
rslider   bounds(150, 40, 60, 60), text("Time 3"), channel("Time3"), range(0.001,1,0.083,0.5,0.001) $RSLIDER_ATTRIBUTES
rslider   bounds(220, 40, 60, 60), text("Feedback"), channel("Feedback"), range(0,1,0.68) $RSLIDER_ATTRIBUTES
}

; OUTPUT
groupbox  bounds(540, 10,100,110), text("O U T P U T") {
rslider   bounds( 20, 35, 60, 60), text("Dry/Wet"), channel("Mix"), range(0,1,0.5) $RSLIDER_ATTRIBUTES
}

; FEEDBACK FILTER
groupbox  bounds( 10,130,380,110), text("F E E B B A C K   F I L T E R") {
rslider   bounds(  5, 35, 60, 60), text("Low Shape"), channel("DampingShape1"), range(-40,40,-20), popupText(0), $RSLIDER_ATTRIBUTES
rslider   bounds( 75, 35, 60, 60), text("Low Cut"), channel("DampingCF"), range(0,0.4,0.01,1,0.001), popupText(0), $RSLIDER_ATTRIBUTES
rslider   bounds(145, 35, 60, 60), text("High Shape"), channel("DampingShape2"), range(-40,40,0.8), popupText(0), $RSLIDER_ATTRIBUTES
rslider   bounds(215, 35, 60, 60), text("High Cut"), channel("DampingHiCut"), range(0,1,1), popupText(0), $RSLIDER_ATTRIBUTES
gentable  bounds(285, 35, 70, 60), tableNumber(11), channel("DampingID"), zoom(-1), ampRange(0,1.1,11), tableColour(180,180,210)
}

; BOING
groupbox  bounds(400,130,240,110), text("B O I N G") {
rslider   bounds( 10, 35, 60, 60), text("Time"), channel("Boing"), range(0.01,4,1.4,1,0.01), popupText(0), $RSLIDER_ATTRIBUTES
checkbox  bounds( 51, 25, 60, 10), text("Invert"), channel("Invert"), colour:0(50,50,0), colour:1(250,250,100)
rslider   bounds( 80, 35, 60, 60), text("Shape"), channel("BoingShape"), range(-8,8,3), popupText(0), $RSLIDER_ATTRIBUTES
gentable  bounds(150, 35, 70, 60), tableNumber(12), channel("BoingID"), zoom(-1), ampRange(0,4.4,12), tableColour(180,180,210)
}

; SPRINGS GRAPHIC
image     bounds( 10, 250,630, 70), colour( 0, 0, 0,255), outlineThickness(4), corners(8) outlineColour("silver") {
gentable  bounds(  5,   5,620, 20), tableNumber(1), fill(0), zoom(-1), ampRange(-1,1,1), tableColour("silver"), tableGridColour(0,0,0,0), channel("Spring1ID")
gentable  bounds(  5,  25,620, 20), tableNumber(2), fill(0), zoom(-1), ampRange(-1,1,2), tableColour("silver"), tableGridColour(0,0,0,0), channel("Spring2ID")
gentable  bounds(  5,  45,620, 20), tableNumber(3), fill(0), zoom(-1), ampRange(-1,1,3), tableColour("silver"), tableGridColour(0,0,0,0), channel("Spring3ID"), visible(0)
}

</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-n -+rtmidi=NULL -M0 -dm0
</CsOptions>

<CsInstruments>

ksmps  = 128
nchnls = 2
0dbfs  = 1

giFFTSizes[] fillarray 512,1024,2048,4096

giSpring1 ftgen 1,0,1024,9,111,1,0
giSpring2 ftgen 2,0,1024,9,131,1,0
giSpring3 ftgen 3,0,1024,9,121,1,0

instr 1

kInput              cabbageGetValue          "Input"
kLoCut              cabbageGetValue          "LoCut"
kHiCut              cabbageGetValue          "HiCut"
kFeedback           cabbageGetValue          "Feedback"
kTime1              cabbageGetValue          "Time1"
kTime2              cabbageGetValue          "Time2"
kTime3              cabbageGetValue          "Time3"
kActive1            cabbageGetValue          "Active1"
kActive2            cabbageGetValue          "Active2"
kActive3            cabbageGetValue          "Active3"
kDampingShape1      cabbageGetValue          "DampingShape1"     ; curvature of the (spectral) envelope segment in the masking table
kDampingShape2      cabbageGetValue          "DampingShape2"     ; curvature of the (spectral) envelope segment in the masking table
kDampingCF          cabbageGetValue          "DampingCF"
kDampingHiCut       cabbageGetValue          "DampingHiCut"
kDampingHiCut       =               1- kDampingHiCut
kMix                cabbageGetValue          "Mix"
kBoing              cabbageGetValue          "Boing"
kBoingShape         cabbageGetValue          "BoingShape"
kInvert             cabbageGetValue          "Invert"
iFFT                =               512                ; FFT size affects quality. Larger values may blur transients however.
iBufSize            =               5                  ; max.delay time. Must be i-rate, sum both buffer delay and max spectral delay time

; Adjust the graphics of the 3 springs if 'active' or 'time' controls are adjusted
#define SPRING_GRAPHIC(N)                        ; create a macro to reduce code repetition
#
if changed:k(kTime$N, kActive$N, kBoing)==1 then
 reinit TABLE$N
endif
TABLE$N:
iAmp$N = (((i(kBoing) - 0.01) / 3.99) * 0.6) + 0.2
i_  ftgen $N, 0, 1024, -9, i(kTime$N)*80+60, iAmp$N, 0 ; function table for spring. Harmonic number is related to delay time
rireturn
cabbageSet  k(1),"Spring$N.ID","tableNumber",k($N)
cabbageSet  k(1),"Spring$N.ID","visible",kActive$N

#
$SPRING_GRAPHIC(1)                               ; expand macro for each spring
$SPRING_GRAPHIC(2)
$SPRING_GRAPHIC(3)

aL, aR ins                  ; read stereo input
if kInput==2 then           ; input option 2: sum left and right input channels
 aL     sum  aL, aR
endif

aL     buthp  aL, kLoCut    ; low cut input filter
aL     butlp  aL, kHiCut    ; high cut input filter

if kInput==3 then           ; if input option 3 is chosen (Dual stereo), filter the right input channel also          
 aR     buthp  aR, kLoCut
 aR     butlp  aR, kHiCut
endif

; spectral smearing envelopes (boing)
if changed:k(kBoing,kBoingShape,kInvert)==1 then
 reinit REBUILD_TAB
endif
REBUILD_TAB:
 iftamps             ftgen           12, 0, iFFT, -16, i(kBoing)*i(kInvert), iFFT, i(kBoingShape), i(kBoing)*(1-i(kInvert))  ; amplitudes delay table
 iftfrqs             ftgen           13, 0, iFFT, -16, i(kBoing)*i(kInvert), iFFT, i(kBoingShape), i(kBoing)*(1-i(kInvert))  ; frequencies delay table
 cabbageSet k(1),"BoingID","tableNumber",12
rireturn

; filter envelope, 'mask' (iteratively applied within the feedback loop)
if changed(kDampingShape1,kDampingShape2,kDampingCF,kDampingHiCut)==1 then      ; if these controls are changed (i.e. they are i-rate controls)
 reinit SET_MASK_FUNCTION                                                       ; force a reinitialisation from the given label
endif
SET_MASK_FUNCTION:                                                                   ; reinit from here
iMaskTabSize      =                 iFFT/2 + 1                                       ; size of table (in points)
id1               limit             iMaskTabSize * i(kDampingCF), 1, iMaskTabSize    ; duration of first segment - low cut (in points)
id3               limit             iMaskTabSize * i(kDampingHiCut), 1, iMaskTabSize ; duration of third segment - high cut (in points)
id2               =                 iMaskTabSize - id1 - id3                         ; duration of second segment (in points)
iMask             ftgen             11, 0, iFFT/2 + 1, 16, 0, id1, i(kDampingShape1), 1, id2, i(kDampingShape2), 0    ; masking table
 cabbageSet k(1),"DampingID","tableNumber",11
rireturn


#define SPRING(N'CHAN)
#
fanal$CHAN                    pvsanal         a$CHAN,iFFT,iFFT/4,iFFT,1                          ; analyse signal (left channel)
fFB$N$CHAN                    pvsinit         iFFT,iFFT/4,iFFT,1                                 ; initialise feedback signal
fDelIn$N$CHAN                 pvsmix          fanal$CHAN, fFB$N$CHAN                             ; mix input with feedback signal
ihandle$N$CHAN, kPtr$N$CHAN   pvsbuffer       fDelIn$N$CHAN, iBufSize                            ; write into PV buffer
fSmear$N$CHAN                 pvsbufread2     kPtr$N$CHAN-kTime$N,   ihandle$N$CHAN, iftamps, iftfrqs ; read from buffer (with delay)
fMask$N$CHAN                  pvsmaska        fSmear$N$CHAN, iMask, 1                            ; apply spectral mask
fFB$N$CHAN                    pvsgain         fMask$N$CHAN, kFeedback                            ; create feedback signal for next pass by applying gain to the output
fSpring$N$CHAN                pvsgain         fMask$N$CHAN, kActive$N                            ; apply on/off switch for this spring
#
$SPRING(1'L)
$SPRING(2'L)
$SPRING(3'L)

; mix left channel signals
fMixL             pvsmix          fSpring1L, fSpring2L                       ; mix masked outputs (1st delay iteration will be filtered)
fMixAllL          pvsmix          fMixL, fSpring3L                           ; mix masked outputs
awetL             pvsynth         fMixAllL                                   ; resynthesise into an audio signal  
amixL             ntrpol          aL, awetL, kMix                            ; dry/wet mix

if kInput!=3 then ; if dual stereo has *not* been selected (option 1 or 2)
                  outs            amixL, amixL
else              ; otherwise must be dual stereo
 $SPRING(1'R)
 $SPRING(2'R)
 $SPRING(3'R)

 ; mix right channel signals
 fMixR             pvsmix          fSpring1R, fSpring2R                       ; mix masked outputs (1st delay iteration will be filtered)
 fMixAllR          pvsmix          fMixR, fSpring3R                           ; mix masked outputs
 awetR             pvsynth         fMixAllR                                   ; resynthesise into an audio signal  
 amixR             ntrpol          aR, awetR, kMix                            ; dry/wet mix
                   outs            amixL, amixR
 

endif

 kVib              rms             amixL + amixR
 kVib1             limit           (gauss:k(10)*kVib^0.25)-1,0,10
 kVib2             limit           (gauss:k(10)*kVib^0.25)-1,0,10
 kVib3             limit           (gauss:k(10)*kVib^0.25)-1,0,10
 cabbageSet k(1), "Spring1ID", "bounds", 5,5 + kVib1, 620, 20
 cabbageSet k(1), "Spring2ID", "bounds", 5,25 + kVib2, 620, 20
 cabbageSet k(1), "Spring3ID", "bounds", 5,45 + kVib3, 620, 20

endin                                                                                     

</CsInstruments>

<CsScore>
i 1 0 z
</CsScore>

</CsoundSynthesizer>
