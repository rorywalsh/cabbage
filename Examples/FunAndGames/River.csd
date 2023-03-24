
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; River Synth
; Iain McCurdy, 2021

; Procedural audio synthesis of a river using granular synthesis and noise
; The sound of a river is separated into three layers:
;  1. Gulp (sparse low pitch glissando grains)
;  2. Roar (dense filtered noise)
;  3. Burble (medium density)

; GULP
; Level    - amplitude level
; Density  - average density of gulps
; Duration - average duration of gulps
; Gliss    - direction and range of glissandi (+ = upwards, - = downwards)
; Pitch    - average starting pitch (frequency) fo gulps
; Spread   - range of randon divergence of starting pitch of gulps
; Low Cut  - frequency of a highpass filter applied to the gulps
; Att/Dec  - balance between the attack and decay portions of the grain envelope

; ROAR
; Level      - amplitude level
; Cent.Freq. - centre frequency of a bandpass filter applied to the noise
; Bandwidth  - bandwidth of a bandpass filter applied to the noise

; BURBLE
; Level    - amplitude level
; Density  - average density of gulps
; Duration - average duration of gulps
; Gliss    - direction and range of glissandi (+ = upwards, - = downwards)
; Pitch    - average starting pitch (frequency) fo gulps
; Spread   - range of randon divergence of starting pitch of gulps

<Cabbage>
#define RSliderStyle  trackerInsideRadius(.9), trackerOutsideRadius(1), markerColour(100,100,255), markerThickness(2), trackerColour(130,130,255), trackerThickness(4), colour(60,65,80), fontColour(200,200,200), textColour(200,200,200), outlineColour(50,50,50), valuetextBox(1)

form caption("River Synth") size(1100, 235), pluginId("RiSy") colour(20,25,30), guiMode("queue")
gentable          bounds(  0, 15,1100, 80), tableNumber(1), tableColour( 55,155,255,155), tableBackgroundColour(0,0,0,0), fill(1), alpha(0.5), outlineThickness(2), tableGridColour(0,0,0,0), channel("IDRiver1"), ampRange(-1.1,1.1,-1), zoom(-1), active(1)
gentable          bounds(  0, 15,1100, 80), tableNumber(2), tableColour( 55,125,255,155), tableBackgroundColour(0,0,0,0), fill(1), alpha(0.5), outlineThickness(2), tableGridColour(0,0,0,0), channel("IDRiver2"), ampRange(-1.1,1.1,-1), zoom(-1), active(1)
gentable          bounds(  0, 15,1100, 80), tableNumber(3), tableColour( 55,125,255,155), tableBackgroundColour(0,0,0,0), fill(1), alpha(0.5), outlineThickness(2), tableGridColour(0,0,0,0), channel("IDRiver3"), ampRange(-1.1,1.1,-1), zoom(-1), active(1)
image             bounds(  0, 55,1100, 70), colour( 45,45,105,155)

image      bounds(5, 125, 500, 105), colour(30, 30, 50, 255), plant("Gulp"), outlineColour(35, 35, 55, 255), outlineThickness(2),  { corners(5)
label      bounds(0, 6, 500, 15), text("G  U  L  P"), , fontColour(192, 192, 192, 255)
checkbox   bounds(10, 7, 13, 13), ,  channel("Gulp"), , value(1) colour:1(255, 255, 0, 255) fontColour:0(255, 255, 255, 255)
rslider    bounds(5, 25, 70, 70), text("Level"),  channel("GulpLev"),  range(0, 6, 3, 1, 0.001),            $RSliderStyle colour(60, 65, 80, 255) fontColour(200, 200, 200, 255) markerColour(100, 100, 255, 255) outlineColour(50, 50, 50, 255) textColour(200, 200, 200, 255) trackerColour(130, 130, 255, 255) trackerInsideRadius(0.9) trackerThickness(4) valuetextBox(1)
rslider    bounds(65, 25, 70, 70), text("Density"),  channel("GulpDens"),  range(1, 1000, 6, 0.33, 0.01),            $RSliderStyle colour(60, 65, 80, 255) fontColour(200, 200, 200, 255) markerColour(100, 100, 255, 255) outlineColour(50, 50, 50, 255) textColour(200, 200, 200, 255) trackerColour(130, 130, 255, 255) trackerInsideRadius(0.9) trackerThickness(4) valuetextBox(1)
rslider    bounds(125, 25, 70, 70), text("Duration"),  channel("GulpDur"),  range(0.005, 1, 0.17, 0.667, 0.001),            $RSliderStyle colour(60, 65, 80, 255) fontColour(200, 200, 200, 255) markerColour(100, 100, 255, 255) outlineColour(50, 50, 50, 255) textColour(200, 200, 200, 255) trackerColour(130, 130, 255, 255) trackerInsideRadius(0.9) trackerThickness(4) valuetextBox(1)
rslider    bounds(185, 25, 70, 70), text("Gliss"),  channel("GulpGliss"),  range(-1, 1, 0.25, 1, 0.001),            $RSliderStyle colour(60, 65, 80, 255) fontColour(200, 200, 200, 255) markerColour(100, 100, 255, 255) outlineColour(50, 50, 50, 255) textColour(200, 200, 200, 255) trackerColour(130, 130, 255, 255) trackerInsideRadius(0.9) trackerThickness(4) valuetextBox(1)
rslider    bounds(245, 25, 70, 70), text("Pitch"),  channel("GulpPitch"),  range(20, 10000, 80, 0.5, 1),            $RSliderStyle colour(60, 65, 80, 255) fontColour(200, 200, 200, 255) markerColour(100, 100, 255, 255) outlineColour(50, 50, 50, 255) textColour(200, 200, 200, 255) trackerColour(130, 130, 255, 255) trackerInsideRadius(0.9) trackerThickness(4) valuetextBox(1)
rslider    bounds(305, 25, 70, 70), text("Spread"),  channel("GulpDiverg"),  range(0, 6, 2, 0.5, 0.01),            $RSliderStyle colour(60, 65, 80, 255) fontColour(200, 200, 200, 255) markerColour(100, 100, 255, 255) outlineColour(50, 50, 50, 255) textColour(200, 200, 200, 255) trackerColour(130, 130, 255, 255) trackerInsideRadius(0.9) trackerThickness(4) valuetextBox(1)
rslider    bounds(365, 25, 70, 70), text("Low Cut"),  channel("GulpHPF"),  range(20, 1000, 300, 0.5, 1),            $RSliderStyle colour(60, 65, 80, 255) fontColour(200, 200, 200, 255) markerColour(100, 100, 255, 255) outlineColour(50, 50, 50, 255) textColour(200, 200, 200, 255) trackerColour(130, 130, 255, 255) trackerInsideRadius(0.9) trackerThickness(4) valuetextBox(1)
rslider    bounds(425, 25, 70, 70), text("Att/Dec"),  channel("GulpAttDec"),  range(0.1, 0.9, 0.3, 1, 0.001),            $RSliderStyle colour(60, 65, 80, 255) fontColour(200, 200, 200, 255) markerColour(100, 100, 255, 255) outlineColour(50, 50, 50, 255) textColour(200, 200, 200, 255) trackerColour(130, 130, 255, 255) trackerInsideRadius(0.9) trackerThickness(4) valuetextBox(1)
}

image      bounds(510,125,200,105), colour(30,30,50), plant("Roar"), outlineColour(35,35,55), outlineThickness(2), shape("rounded") {
label      bounds(  0,  6,200, 15), text("R  O  A  R"), align("centre"), fontColour("silver")
checkbox   bounds( 10,  7, 13, 13), text(""), fontColour("white") channel("Roar"), colour(255,255, 0), value(1)
rslider    bounds(  5, 25, 70, 70), text("Level"),  channel("RoarLev"),  range(0,4,1,0.5),            $RSliderStyle
rslider    bounds( 65, 25, 70, 70), text("Cent.Freq."),  channel("RoarTone"),  range(50,4000,100,0.5,1),            $RSliderStyle
rslider    bounds(125, 25, 70, 70), text("Bandwidth"),  channel("RoarBW"),  range(0.3,5,5,0.5,0.001),            $RSliderStyle
}

image      bounds(715,125,380,105), colour(30,30,50), plant("Burb"), outlineColour(35,35,55), outlineThickness(2), shape("rounded") {
label      bounds(  0,  6,380, 15), text("B  U  R  B  L  E"), align("centre"), fontColour("silver")
checkbox   bounds( 10,  7, 13, 13), text(""), fontColour("white") channel("Burb"), colour(255,255, 0), value(1)
rslider    bounds(  5, 25, 70, 70), text("Level"),  channel("BurbLev"),  range(0,2,1.2,0.5),            $RSliderStyle
rslider    bounds( 65, 25, 70, 70), text("Density"),  channel("BurbDens"),  range(1,2000,1400,0.5,1),            $RSliderStyle
rslider    bounds(125, 25, 70, 70), text("Duration"),  channel("BurbDur"),  range(0.001,0.1,0.034,0.667,0.001),            $RSliderStyle
rslider    bounds(185, 25, 70, 70), text("Gliss"),  channel("BurbGliss"),  range(-1,1,0.3),            $RSliderStyle
rslider    bounds(245, 25, 70, 70), text("Pitch"),  channel("BurbPitch"),  range(20,8000,380,0.5,1),            $RSliderStyle
rslider    bounds(305, 25, 70, 70), text("Spread"),  channel("BurbDiverg"),  range(0,10, 1.5, 0.5, 0.01),            $RSliderStyle
}

</Cabbage>


<CsoundSynthesizer>

<CsOptions>
-n -d -+rtmidi=NULL -M0 -m0d
</CsOptions>

<CsInstruments>

ksmps = 32
nchnls = 2
0dbfs = 1


giSfn    ftgen   0, 0, 262144, 10, 1               ; source waveform for the fof2 burbles (a sine wave)
giAttDec ftgen   0, 0, 262144, 16, 0, 262144, 4, 1   ; envelope shape for fof2 grains

instr  1

; RIVER GRAPHICS
kPhs   phasor  -rspline:k(0.3, 0.4, 0.4, 0.5)
k1     rspline 0.4, 0.9, 2, 3
if metro:k(20)==1 then
 reinit RIVER
endif
RIVER:
 gi1      ftgen   1, 0, 512, -9, 3, i(k1), i(kPhs)*360
 cabbageSet "IDRiver1", "tableNumber", 1
rireturn

kPhs   phasor  -rspline:k(0.2, 0.5, 0.4, 0.5)
k1     rspline 0.4, 0.9, 2, 3
if metro:k(20)==1 then
 reinit RIVER2
endif
RIVER2:
 gi2      ftgen   2, 0, 512, -9, 4, i(k1), i(kPhs)*360
 cabbageSet "IDRiver2", "tableNumber", 2
rireturn

kPhs   phasor  -rspline:k(0.9, 1.2, 0.4, 0.5)
k1     rspline 0.2, 0.4, 2, 3
if metro:k(20)==1 then
 reinit RIVER3
endif
RIVER3:
 gi3      ftgen   3, 0, 512, -9, 2, i(k1), i(kPhs)*360
 cabbageSet "IDRiver3", "tableNumber", 3
rireturn


; READ IN WIDGETS
kGulp     chnget "Gulp"
kGulpLev chnget "GulpLev"
kGulpDens chnget "GulpDens"
kGulpDur chnget "GulpDur"
kGulpGliss chnget "GulpGliss"
kGulpPitch chnget "GulpPitch"
kGulpDiverg chnget "GulpDiverg"
kGulpHPF chnget "GulpHPF"
kGulpAttDec  chnget "GulpAttDec"

kRoar  chnget "Roar"
kRoarLev chnget "RoarLev"
kRoarTone chnget "RoarTone"
kRoarBW  chnget "RoarBW"

kBurb  chnget "Burb"
kBurbLev chnget "BurbLev"
kBurbDens chnget "BurbDens"
kBurbDur chnget "BurbDur"
kBurbGliss chnget "BurbGliss"
kBurbPitch chnget "BurbPitch"
kBurbDiverg chnget "BurbDiverg"

; GULPS
#define GULPS(PAN)
#
kFund   =        kGulpPitch       ; minimum burble pitch
kDiverg gauss    kGulpDiverg                ; divergence in octaves
kFund   *=       2 ^ kDiverg 

kdur    linrand  kGulpDur      ; random duration range of burbles
kdur    +=       kGulpDur*0.5      ; minimum burble duration

kris    =        kdur * kGulpAttDec
kdec    =        kdur * (1-kGulpAttDec)

kDens   randomi  kGulpDens*0.5, kGulpDens*2, kGulpDens, 1 ; randomly wandering burble density

kAmp    exprand  kGulpLev      ; random amplitude range

kGliss exprand  0.4 * kGulpGliss       ; burble glissando
kGliss  +=       8 * kGulpGliss      ; minimum burble glissando

kband   =    10 ;limit    1000*kGulpDur, 2, 20
;ares   fof2     xamp,                                        xfund, xform, koct, kband, kris, kdur,  kdec, iolaps, ifna,  ifnb,     itotdur, kphs, kgliss [, iskip]
aGulp   fof2     (ampdbfs(kGulpLev)-ampdbfs(kAmp))*0.4*kGulp, kDens, kFund, 0,    kband, kris, kdur,  kdec, 2000,   giSfn, giAttDec, 3600,    0,    kGliss
aGulp   buthp    aGulp, port:k(kGulpHPF,0.05)
aL,aR   pan2     aGulp, $PAN
        outs     aL, aR
#
iPan = 0.2
$GULPS(0.5-iPan)
$GULPS(0.5+iPan)


; ROAR
#define ROAR(CHAN)
#
aRoar   dust2    rspline(0.4,0.6,0.2,0.3), rspline:k(10000,12000,0.2,0.3)       ; create some noise
aRoar   butbp    aRoar, kRoarTone, kRoarTone*kRoarBW  ; bandpass filter the noise
kCF     rspline  1200, 2000, 4, 1 ; create a moving cutoff frequency for a lowpass filter
aRoar   tone     aRoar, kCF       ; lowpass filter the noise
outch $CHAN, aRoar*kRoarLev*kRoar
#
$ROAR(1)
$ROAR(2)



; BURBLE
#define BURB(PAN)
#
kFund   =        kBurbPitch       ; minimum burble pitch
kDiverg gauss    kBurbDiverg                ; divergence in octaves
kFund   *=       2 ^ kDiverg 

kdur    linrand  kBurbDur      ; random duration range of burbles
kdur    +=       kBurbDur*0.5      ; minimum burble duration

kris    =        kdur * 0.2
kdec    =        kdur * 0.8

kDens   randomi  kBurbDens*0.5, kBurbDens*2, kBurbDens, 1 ; randomly wandering burble density

kAmp    linrand  kBurbLev      ; random amplitude range

kGliss exprand  0.4 * kBurbGliss       ; burble glissando
kGliss  +=       8 * kBurbGliss      ; minimum burble glissando

kband   =    10 ;limit    1000*kBurbDur, 2, 20
;ares   fof2     xamp,                                        xfund, xform, koct, kband, kris, kdur,  kdec, iolaps, ifna,  ifnb,     itotdur, kphs, kgliss [, iskip]

aBurble fof2     (ampdbfs(kBurbLev)-ampdbfs(kAmp))*0.1*kBurb, kDens, kFund, 0,    kband, kris, kdur,  kdec, 5000,    giSfn, giAttDec, 3600,    0,    kGliss
aL,aR   pan2     aBurble, $PAN
        outs     aL, aR
#
iPan = 0.2
$BURB(0.5-iPan)
$BURB(0.5+iPan)







endin


</CsInstruments>

<CsScore>
i 1 0 3600 ; play a long note...
</CsScore>

</CsoundSynthesizer>
