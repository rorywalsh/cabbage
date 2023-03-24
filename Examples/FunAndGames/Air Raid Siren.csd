
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; Air Raid Siren / Tornado Siren / Civil Defence Siren

; Iain McCurdy, 2021

; The classic WWII British air-raid siren works by a spinning fan forcing air through a series of slots in an enclosing cylinder. 
; The speed of the fan determines the fundamental pitch of the tone and to a degree also, the intensity. 
; Typically, a single siren would feature two of these, enabling it to emit two tones. 
; One of the cylinders would have 10 slots and the other would have 12 slots and this ratio (5:2) of slots is reflected in the frequency ratio between the two tones.
; The fan mechanism presents a high degree of inertia meaning that when turned on and off, 
; the siren takes some time to arrive at or decay from its high steady tone.

; A range of variation of this design were used in the 20th century as police car sirens, tornado warnings etc.


; CONTROLS

; Top Speed - the speed/pitch of the siren when it reaches maximum
; Rise      - amount of damping while the siren is rising in speed/pitch
; Damp      - amount of damping while the siren is falling in speed/pitch

; For a tone generator a 'squinewave' is used. This starts as a sine wave to which clipping and skew (pulse width shift) is applied
; Clip      - clipping of the peaks of the waveform.
;             if skew and rounding are at minimum, this morphs the waveform from sine to square
; Skew      - skew (pulse width) of the waveform
; Rounding  - defines the duration of smoothing applied to sudden changes in the waveform, e.g. square wave. This can be used to soften a waveform, 

; Low Cut   - position of a high-pass filter (ratio to the current frequency of the siren)
; High Cut  - low-pass filter (Hz)

; Horn Freq. - frequency of a comb filter. Most apparent when Horn Freq. is high.
; Horn Res.  - resonance (feedback) of a comb filter (negative feedback is also possible)

; Pan        - pan position L/R
; Distance   - combines level and reverb/delay send control

<Cabbage>
form caption("Air Raid Siren") size(825,184), colour(120,120,150), pluginId("ARSi"), guiRefresh(10)
image                 bounds( 4, 4, 817,177), corners(15), outlineColour(50,50,50,150), outlineThickness(3) shape("square"), colour(255,255,255,100)
image                 bounds( 8, 8, 809,169), corners(15), outlineColour(50,50,50,150), outlineThickness(3) shape("square"), colour(255,255,255,100)

; Fins
image      bounds( 700,47,100,90), colour(70,65,70,155), plant("Fins"), outlineThickness(4), outlineColour("black") {
image      bounds( -100,-100, 10, 40), colour(55, 55, 55), widgetArray("Fins1", 10), shape("sharp"), outlineThickness(2), outlineColour("black")
image      bounds( -100,-100, 10, 40), colour(55, 55, 55), widgetArray("Fins2", 10), shape("sharp"), outlineThickness(2), outlineColour("black")
}



image    bounds( 16, 16, 68, 28), shape("rounded"), colour(255,100,100,60), identChannel("OnGlow"), visible(0)
button   bounds( 20, 20, 60, 20), fontColour:0(50,50,50), fontColour:1(255,205,205), colour:0(10,0,0), colour:1(200,50,50), text("On","On"), channel("On"), latched(0), value(0), identChannel("OnID")

image    bounds( 19, 42, 21, 21), shape("ellipse"), colour(255,255,100,40), identChannel("LatchGlow"), visible(1)
checkbox bounds( 22, 45, 70, 15), shape("ellipse"), fontColour:0(50,50,50), fontColour:1(50,50,50), colour:0(10,0,0), colour:1(255,255,120), text("Latch","Latch"), channel("Latch"), latched(1), value(1)

label    bounds( 25, 61, 50, 10), text("RATIO"), fontColour(20,20,20)
combobox bounds( 25, 71, 50, 17), channel("Ratio"), align("centre"), items("1/1", "2/1", "3/2", "4/3", "5/4", "6/5", "7/6", "8/7", "3/1", "5/3", "7/5", "9/7", "4/1", "5/2", "7/4", "8/5", "10/7"), value(6)

rslider bounds( 95, 20, 70, 70) range(10, 2000, 470, 0.5, 1),    channel("Speed"), text("Top Speed"),  textColour(50,50,50), trackerColour(150,150,150)

label   bounds(175, 21, 90, 10), text("I N E R T I A"), fontColour(20,20,20)
rslider bounds(175, 29, 40, 60) range(0.01, 10, 0.5, 0.5, 0.001), channel("RiseDamp"), text("Rise"),  textColour(50,50,50), trackerColour(150,150,150)
rslider bounds(225, 29, 40, 60) range(0.01, 10, 4,   0.5, 0.001), channel("FallDamp"), text("Fall"),  textColour(50,50,50), trackerColour(150,150,150)

line    bounds(275, 20,  2, 65)

rslider bounds(280, 20, 65, 65) range(0.0, 1, 0.3,   1, 0.001),   channel("Clip"), text("Clip"),  textColour(50,50,50), trackerColour(150,150,150)
rslider bounds(345, 20, 65, 65) range(0, 1, 0.9,   1, 0.001),   channel("Skew"), text("Skew"),  textColour(50,50,50), trackerColour(150,150,150)
rslider bounds(410, 20, 65, 65) range(4, 64, 24,   1, 1),   channel("MinSweep"), text("Rounding"),  textColour(50,50,50), trackerColour(150,150,150)

gentable bounds(490, 25,90,50), identChannel("table1"), tableNumbers(101), fill(0), outlineThickness(2), tableColours("lightblue"), ampRange(-1.1,1.1,101), zoom(-1)

rslider bounds( 30,100, 70, 70) range(1,     8,    2,   1, 0.001),   channel("LoCut"), text("Low Cut"),  textColour(50,50,50), trackerColour(150,150,150)
rslider bounds( 95,100, 70, 70) range(50, 8000, 3000, 0.5, 1),   channel("HiCut"), text("High Cut"),  textColour(50,50,50), trackerColour(150,150,150)

line    bounds(175,100,  2, 65)

rslider bounds(185,100, 70, 70) range(50, 5000, 500,   0.5, 0.001),   channel("HornFrq"), text("Horn Freq."),  textColour(50,50,50), trackerColour(150,150,150)
rslider bounds(255,100, 70, 70) range(-0.9, 0.9, 0.3,   1, 0.001),   channel("HornRes"), text("Horn Res."),  textColour(50,50,50), trackerColour(150,150,150)

line    bounds(335,100,  2, 65)

rslider bounds(345,100, 70, 70) range(0, 1, 0.4,   1, 0.001),   channel("Pan"), text("Pan"),  textColour(50,50,50), trackerColour(150,150,150)
rslider bounds(410,100, 70, 70) range(0, 0.99, 0.5,   1, 0.001),   channel("Dist"), text("Distance"),  textColour(50,50,50), trackerColour(150,150,150)
}

line    bounds(490,100,  2, 65)

rslider bounds(500,100, 70, 70) range(0, 1, 0.8,   0.5, 0.0001),   channel("Level"), text("Level"),  textColour(50,50,50), trackerColour(150,150,150)

vmeter   bounds(600, 15, 30,140) channel("VU_SPEED") value(0) overlayColour(0, 0, 0, 255) meterColour:0(255, 255, 0) meterColour:1(255, 103, 171) meterColour:2(250,250, 0) outlineThickness(3), outlineColour("black")
label    bounds(600,158, 30, 10), text("SPEED"), fontColour(20,20,20)

vmeter   bounds(650, 15, 30,140) channel("VU_LEVEL") value(0) overlayColour(0, 0, 0, 255) meterColour:0(255, 255, 0) meterColour:1(255, 103, 171) meterColour:2(250,250, 0) outlineThickness(3), outlineColour("black")
label    bounds(650,158, 30, 10), text("LEVEL"), fontColour(20,20,20)

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

giRatios[] fillarray 1/1, 2/1, 3/2, 4/3, 5/4, 6/5, 7/6, 8/7, 3/1, 5/3, 7/5, 9/7, 4/1, 5/2, 7/4, 8/5, 10/7
giRatioN[] fillarray 2, 4, 3, 4, 5, 6, 7, 8, 6, 5, 7, 9, 8, 5, 7, 8, 10
giRatioD[] fillarray 2, 2, 2, 3, 4, 5, 6, 7, 2, 3, 5, 7, 2, 2, 4, 5, 7

giWave   ftgen  101,0,512,10,1

instr 1
kOn        chnget         "On"                                    ; the basic on/off values, only ever 1 (on) or zero (off)
kLatch     chnget         "Latch"
if changed:k(kLatch)==1 then
 Smsg   sprintfk "latched(%d)", kLatch
        chnset   Smsg, "OnID"
 Smsg   sprintfk "visible(%d)", kLatch
        chnset   Smsg, "LatchGlow"
endif

if changed:k(kOn)==1 then
 Smsg   sprintfk "visible(%d)", kOn
        chnset   Smsg, "OnGlow"
endif

kLevel     chnget         "Level"
kSpeed     chnget         "Speed"
kRiseDamp  chnget         "RiseDamp"
kFallDamp  chnget         "FallDamp"
kClip      chnget         "Clip"
kSkew      chnget         "Skew"
kHornFrq   chnget         "HornFrq"
kPortTime  linseg         0, 0.001, 0.05
kHornFrq   portk          kHornFrq, kPortTime
kHornRes   chnget         "HornRes"
kLoCut     chnget         "LoCut"
kHiCut     chnget         "HiCut"
kRatio     =              giRatios[chnget:k("Ratio")-1]
kPan       chnget         "Pan"                                  ; pan position, 0 to 1
kDist      chnget         "Dist"                                 ; distance from listener. As this increases the reverb gets louder and the dry signal get quieter to simoulate distance     
kPTime     =              kOn > 0.5 ? kRiseDamp : kFallDamp      ; it seems helpful to use a different portamento time if the siren is rising or falling
kAmp       portk          kOn, kPTime                            ; apply portamento (lap) to create the amplitude of the siren
kCPS       limit          portk:k(kOn*kSpeed,kPTime), 10, 20000  ; apply portamento (lap) to create the frequency value of the siren (the maximum frequency of the siren is 470 hertz)
kWob       jspline        0.05, 1, 20                            ; a bit of analogue jitter

kMinSweep  chnget         "MinSweep"

if changed:k(kMinSweep)==1 then
 reinit RESTART
endif
RESTART:
; generate the two tones
aSiren1, a_ squinewave     a(kCPS*semitone(kWob)),       a(kClip), a(-kSkew), a(0), i(kMinSweep)
if kRatio>1 then
aSiren2, a_ squinewave     a(kCPS*(kRatio)*semitone(kWob)), a(kClip), a(-kSkew), a(0), i(kMinSweep) 
else
aSiren2 = 0
endif

; update waveform view
aWave, aSync squinewave       a(kCPS),       a(kClip), a(-kSkew), a(0), i(kMinSweep), 0
aPtr    phasor  kCPS
        tablew  aWave, aPtr, 101, 1
        chnset  "tableNumber(101)", "table1"


rireturn
aSiren     =              aSiren1 + aSiren2                      ; mix of the two halves of the siren
aSiren     buthp          aSiren, limit:k(kCPS*port:k(kLoCut,0.05),20,sr/2)   ; highpass filter the siren

; filtering...
aSiren     dcblock2       aSiren                           ; DC block the siren (this is another highpass filter but it seems to help).
aSiren     butlp          aSiren, port:k(kHiCut,0.05)
; a short delay is applied the siren to emulate the pipe. This is like a comb filtering effect
aSiren     flanger        aSiren, a(1/kHornFrq), kHornRes

; stereo placement
aL,aR      pan2           aSiren * kAmp^2 * (1-kDist) * kLevel, kPan      ; pan the siren left or right according to the direction indicated in the score p-field
           out            aL, aR 
; send some of the signal through a reverb
iRvbSend = 2
           chnmix         aL * iRvbSend * kDist, "SendL"
           chnmix         aR * iRvbSend * kDist, "SendR"


chnset kCPS/2000,"VU_SPEED"

; meter
if   metro:k(10)==1 then
reinit REFRESH_METER
endif
REFRESH_METER:
kres init    0
kres peak    aL                    
rireturn
kres   lagud kres,0.01,0.01
chnset kres, "VU_LEVEL"


if changed:k(chnget:k("Ratio"))==1 then

 kCnt = 1
 while kCnt<=10 do
 SID  sprintfk "Fins1_ident%d", kCnt
 chnset "pos(-100,-100)", SID
 SID  sprintfk "Fins2_ident%d", kCnt
 chnset "pos(-100,-100)", SID 
 kCnt += 1
 od
endif

; Draw fins
kPtr  phasor kCPS/100

if metro(30)==1 then
 kCnt   = 1
 ;kNFins = 5 
 kNFins  =              giRatioN[chnget:k("Ratio")-1]
 kWid    =   100/kNFins
 kWid    *=  (100+kWid)/100
 while kCnt<=kNFins do
 kPtr1 wrap      kPtr + ((kCnt-1)/kNFins), 0, 1
 Smsg  sprintfk  "bounds(%d,%d,%d,%d), colour(%d,%d,%d)", ((kPtr1*100)-kWid) * (100+kWid)/100, 4, kWid - 2, 40, kPtr1*150, kPtr1*150, kPtr1*150
 SID  sprintfk "Fins1_ident%d", kCnt
 chnset Smsg, SID
 kCnt += 1
 od

 kCnt  =         1
 ;kNFins = 6
 kNFins  =              giRatioD[chnget:k("Ratio")-1]
 kWid    =   100/kNFins
 kWid    *=  (100+kWid)/100
 while kCnt<=kNFins do
 kPtr1 wrap      kPtr + ((kCnt-1)/kNFins), 0, 1
 Smsg  sprintfk  "bounds(%d,%d,%d,%d), colour(%d,%d,%d)", ((kPtr1*100)-kWid)  * (100+kWid)/100, 46, kWid - 2, 40, kPtr1*150, kPtr1*150, kPtr1*150
 SID  sprintfk "Fins2_ident%d", kCnt
 chnset Smsg, SID
 kCnt += 1
 od


endif


endin

instr 99   ; reverb
aInL       chnget         "SendL"
aInR       chnget         "SendR"
           chnclear       "SendL"
           chnclear       "SendR"
           
aDl,aDR    init           0
aDL        delayr         0.471
           delayw         aInL*0.3 + aDR*0.4
aDR        delayr         0.471
           delayw         aInR*0.3 + aDL*0.4

aL,aR      reverbsc       aInL + aDL, aInR + aDR, 0.8, 3000
           outs           aL, aR
endin

</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
i 1 0 z
i 99 0 z
</CsScore>
</CsoundSynthesizer>