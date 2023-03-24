
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; Spatialiser
; Iain McCurdy 2021

; A moving sound source is tranformed according to:
; - Doppler shift
; - Panning location
; - Amplitude drop off
; - High frequency drop off
; - Reverb diffusion (reverb doesn't drop off as quickly as the direct sound due to diffusion)

; The 'listener' can be in one of two forms:
; - two microphones represented by red and blue boxes
; - a 'head' listener. Interaural time differences are calculated dependent on the angle of the source sound to the listener.

; The sound source - the green circle in the main xy pad area - can be moved in three ways:
; 1 - manually using click and drag
; 2 - using Cabbage's built-in mechanism for flinging thit into motion using right-click and drag
; 3 - using CIRCLE mode - a built-in 2D LFO

; On/Off buttons - toggle between MICS and HEAD listener modes

; MICS
; Separation - spacing between the two speakers
; Distance   - distance of the microphones

; HEAD
; locate the listening head using the xypad

; CIRCLE
; Threes shapes are offered: curve (circle), straight (rhombus) and loop (repeating straight line trajectory) 
; Radius  - radius of the circle
; Speed   - speed of rotation
; Ellipse - distortion of the circle horzontally and vertically
; Skew    - rotational shift of the ellipse

; SCALING - scaling of some of the aspects of the spatialisation
; Doppler   - amount of doppler shift
; Amplitude - rapidity of amplitude fall off
; Panning   - steepness of panning shift when crossing the listener 
; Filter    - amount of high frequency loss with distance from the sound source

; REVERB
; Amount    - amount of reverb with respect to the dry signal
; Size      - size of the reverberant space
; Damping   - high ferquency damping within the reverberant space, i.e. the nature of the reflective surfaces in the space
; Diffusion - amount by which reverb prevails even as the sound source recedes into the distance 

; SETUP
; Follow Time - movement damping applied when moving the sound source manually

<Cabbage>
form caption("Spatialisation"), size(690,590), pluginId("Spat"), guiRefresh(1), colour("Black")

; MAIN XY PAD
image bounds(280, 10,400,460), colour(0,0,0,0) {
xypad bounds(  0,  0,400,460), channel("X", "Y"), rangeX(0, 1, 0.5), rangeY(0, 1, 0.5), colour(20,20,30), fontColour("white")
image bounds(100,200,20,20), shape("sharp"), alpha(0.5), identChannel("Mic1ID"), text("1"), colour(0,0,255), trackerColour(100,100,0)
image bounds(300,200,20,20), shape("sharp"), alpha(0.5), identChannel("Mic2ID"), text("2"), colour(255,0,0)
;label bounds(100,200,20,20), shape("sharp"), alpha(0.5), identChannel("Mic1ID"), text("1")
;label bounds(300,200,20,20), shape("sharp"), alpha(0.5), identChannel("Mic2ID"), text("2")
}

; MICS CONTROLS
groupbox bounds( 10, 10, 260,100), text("M I C S") {
rslider  bounds( 60, 30, 60, 60), channel("Separation"), range(0,0.5,0.25), text("Separation")
rslider  bounds(140, 30, 60, 60), channel("Distance"), range(0,1,0.5), text("Distance")
}
checkbox bounds( 20, 15, 80,10), channel("MicsOnOff"), colour:0(50,100,50), text("On/Off"), radioGroup(1), value(1)

; HEAD CONTROLS
groupbox bounds( 10,120,260,240), text("H E A D") {
xypad    bounds( 35, 30,190,200), channel("HeadX", "HeadY"), rangeX(0, 1, 0.5), rangeY(0, 1, 0.5), colour(20,20,30), fontColour("white")
}
checkbox bounds( 20,125, 80,10), channel("HeadOnOff"), colour:0(50,100,50), text("On/Off"), radioGroup(1), value(0)

; CIRCLE CONTROLS
groupbox bounds( 10,370,260,100), text("C I R C L E") {
checkbox bounds( 10,  5, 80,10), channel("CircleOnOff"), colour:0(50,100,50), text("On/Off");, value(1)
combobox bounds(180,  1, 70,18), channel("CShape"), value(1), text("CURVE","STRAIGHT","LOOP","RANDOM")
rslider  bounds( 10, 30, 60, 60), channel("Radius"), range(0,1,1), text("Radius")
rslider  bounds( 70, 30, 60, 60), channel("CSpeed"), range(-1,1,0.25), text("Speed")
rslider  bounds(130, 30, 60, 60), channel("CSquash"), range(0, 1, 0.5), text("Ellispe")
rslider  bounds(190, 30, 60, 60), channel("CSkew"), range(0, 0.5, 0.25), text("Skew")
}

; SCALING
groupbox bounds( 10,480,260,100), text("S C A L I N G") {
rslider  bounds( 10, 30, 60, 60), channel("Doppler"), range(0,0.5,0.1,0.5), text("Doppler")
rslider  bounds( 70, 30, 60, 60), channel("Amplitude"), range(0,240,60,0.5), text("Amplitude")
rslider  bounds(130, 30, 60, 60), channel("Panning"), range(0,90,24,0.5), text("Panning")
rslider  bounds(190, 30, 60, 60), channel("HFDistLoss"), range(0,10,7), text("HF Loss")
}

; REVERB
groupbox bounds(280,480,260,100), text("R E V E R B") {
rslider  bounds( 10, 30, 60, 60), channel("RvbAmt"), range(0,1,0.05,0.5), text("Amount")
rslider  bounds( 70, 30, 60, 60), channel("RvbSize"), range(0.1,0.99,0.8,2), text("Size")
rslider  bounds(130, 30, 60, 60), channel("RvbDamp"), range(1000,20000,8000,0.5,1), text("Damping")
rslider  bounds(190, 30, 60, 60), channel("RvbDiff"), range(0,1,0.8,0.5), text("Diffusion")
}

; SETUP
groupbox bounds(550,480,130,100), text("S E T U P") {
rslider  bounds(  5, 30, 60, 60), channel("FollowTime"), range(0,1,0.05,0.5), text("Follow Time")
label    bounds( 75, 30, 45, 13), text("In"), align("centre")
button   bounds( 75, 45, 45, 14), channel("MonoIn"), text("Mono"), colour:0(50,50,0), colour:1(200,200,0), radioGroup(2), value(1)
button   bounds( 75, 60, 45, 14), channel("StereoIn"), text("Stereo"), colour:0(50,50,0), colour:1(200,200,0), radioGroup(2)

}

; HEAD
image bounds(495,195,50,50) colour(0,0,0,0), identChannel("HeadID") plant("Head"), visible(0) {
image bounds(  5,  5, 40,40), colour(200,200,200), shape("round")
image bounds(  0, 18, 10,14), colour(200,200,200), shape("round")
image bounds( 40, 18, 10,14), colour(200,200,200), shape("round")
image bounds( 21,  0,  8,10), colour(200,200,200), shape("round")
}

</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-dm0 -n -+rtmidi=NULL -M0
</CsOptions>

<CsInstruments>

; sr set by host
ksmps  =  32
nchnls =  2
0dbfs  =  1

giSine ftgen 1,0,4096,10,1
giTri  ftgen 2,0,4096,7, 0,1024,1,2048,-1,1024,0
giLoop ftgen 3,0,4096,7, -1,4096,1

instr 1
event_i "i",2,0,0.01 ; fix xypad init positions

iDelCF       =        1  ; delay time filter CF used to smooth sudden changes in delay time

kX           chnget   "X"
kY           chnget   "Y"
kHeadX       chnget   "HeadX"
kHeadY       chnget   "HeadY"
kSeparation  chnget   "Separation"
kSeparation  =        0.5 - kSeparation
kDistance    chnget   "Distance"
kDistance    =        1 - kDistance
kRadius      chnget   "Radius"
kCSpeed      chnget   "CSpeed"
kCSpeed      =        (kCSpeed^3) * 20
kCShape      chnget   "CShape"
kCSquash     chnget   "CSquash"
kCSkew       chnget   "CSkew"
kDoppler     chnget   "Doppler"
kAmplitude   chnget   "Amplitude"
kPanning     chnget   "Panning"
kCircleOnOff chnget   "CircleOnOff"
kMicsOnOff   chnget   "MicsOnOff"
kHeadOnOff   chnget   "HeadOnOff"
kFollowTime  chnget   "FollowTime"
kRvbAmt      chnget   "RvbAmt"
gkRvbSize    chnget   "RvbSize"
gkRvbDamp    chnget   "RvbDamp"
kRvbDiff     chnget   "RvbDiff"
kRvbDiff     =        1 - kRvbDiff
kHFDistLoss  chnget   "HFDistLoss"
kHFDistLoss  =        (10 - kHFDistLoss) + 4
kPortRamp    linseg   0, 0.001, 1
kMonoIn      chnget   "MonoIn"
kStereoIn    chnget   "StereoIn"

;toggle mics/head
if changed:k(kMicsOnOff)==1 then
 SMsg        sprintfk "visible(%d)", kMicsOnOff
             chnset   SMsg, "Mic1ID"
             chnset   SMsg, "Mic2ID"
 SMsg        sprintfk "visible(%d)", 1-kMicsOnOff
             chnset   SMsg, "HeadID"
endif

; animate mic positions
if changed:k(kSeparation,kDistance)==1 then
Smsg         sprintfk "pos(%d,%d)", 8 + kSeparation * 357, 9 + kDistance * 362
             chnset   Smsg, "Mic1ID"
Smsg         sprintfk "pos(%d,%d)", (1-kSeparation) * 372, 9 + kDistance * 362
             chnset   Smsg, "Mic2ID"
endif

; animate head position
if changed:k(kHeadX,kHeadY)==1 then
Smsg         sprintfk "pos(%d,%d)", limit:k((kHeadX * 400) + 251,270,640), limit:k( ((1-kHeadY) * 400) - 20, 5, 370)
             chnset   Smsg, "HeadID"
endif

; audio input
;a1, a2       diskin2  "/Users/imccurdy/Documents/SourceSounds/Amen.wav",1,0,1

;a1        vco2     0.1, 100
;a2        =        a1

a1,a2      ins

if kMonoIn==1 then
 a2 = a1
endif


; Circle
if kCircleOnOff==1 then
 if kCShape==4 then
  kAX       =         limit:k(rspline:k(0.5 - kRadius*0.5, 0.5 + kRadius*0.5, kCSpeed*0.5, kCSpeed*2),0,1)
  kAY       =         limit:k(rspline:k(0.5 - kRadius*0.5, 0.5 + kRadius*0.5, kCSpeed*0.5, kCSpeed*2),0,1)
 else
  ;kAX       oscil    limit:k(kRadius * kCSquash,0,0.5), kCSpeed,  i(kCShape), 0.125
  aAX       osciliktp kCSpeed, kCShape, kCSkew
  kAX       limit     k(aAX) * kRadius * limit:k(kCSquash,0,0.5), -0.5, 0.5
  ;kAY       oscil    limit:k(kRadius * (1-kCSquash),0,0.5), kCSpeed, i(kCShape)
  aAY       osciliktp  kCSpeed, kCShape, k(0)
  kAY       limit    k(aAY) * kRadius * limit:k((1-kCSquash),0,0.5), -0.5, 0.5 
  kAX       +=       0.5
  kAY       +=       0.5
 endif
            chnset   kAX, "X"
            chnset   kAY, "Y"
else
 ; Manual
 kAX       portk    kX, kFollowTime * kPortRamp
 kAY       portk    kY, kFollowTime * kPortRamp
endif



; MICS
;calculate distances to mics
if kMicsOnOff==1 then
ka1       =        abs((kSeparation) - kAX)
ko1       =        abs((1-kDistance) - kAY)
kh1       =        (ka1^2 + ko1^2) ^ 0.5
ka2       =        abs((1-kSeparation) - kAX)
kh2       =        (ka2^2 + ko1^2) ^ 0.5
;doppler
kDel1     =        kh1 * kDoppler
kDel2     =        kh2 * kDoppler
aDel1     interp   kDel1
aDel2     interp   kDel2
a1D       vdelayxw a1, (tone:a(aDel1,iDelCF) + 1/kr), 1, 16
a2D       vdelayxw a2, (tone:a(aDel2,iDelCF) + 1/kr), 1, 16
;amplitude
kdB1      =        kh1 * (-kAmplitude)
kdB2      =        kh2 * (-kAmplitude)
;filter
kCF1      scale    limit:k((1-kh1),0,1), 14, kHFDistLoss
kCF2      scale    limit:k((1-kh2),0,1), 14, kHFDistLoss
a1D       tone     a1D, cpsoct(kCF1)
a2D       tone     a2D, cpsoct(kCF2)
;reverb send
          chnmix   a1D*kRvbAmt*a(ampdbfs(kdB1*kRvbDiff)), "SendL"
          chnmix   a2D*kRvbAmt*a(ampdbfs(kdB1*kRvbDiff)), "SendR"
;amplitude scaling
a1D       *=       a(ampdbfs(kdB1))
a2D       *=       a(ampdbfs(kdB2))
endif



; HEAD
if kHeadOnOff==1 then
kHeadX   portk    kHeadX, kFollowTime * kPortRamp
kHeadY   portk    kHeadY, kFollowTime * kPortRamp
; distance
ka       =        abs(kHeadX - kAX)
ko       =        abs(kHeadY - kAY)
kh       =        (ka^2 + ko^2) ^ 0.5
kAng     =        taninv(ko/ka)
kAng     =        (1 - (kAng/($M_PI*0.5))) * (kAX>kHeadY?1:-1)   ; -1 to 1
kIAT1    =        limit:k(kAng,0,1) * 0.007
kIAT2    =        limit:k(kAng+1,0,1) * 0.007
; doppler
kDel     =        kh * kDoppler
aDel1    interp   kDel + kIAT1
aDel2    interp   kDel + kIAT2
a1D	     vdelayxw a1, (tone:a(aDel1,iDelCF) + 1/kr), 1, 16
a2D	     vdelayxw a2, (tone:a(aDel2,iDelCF) + 1/kr), 1, 16
; panning
kDist    =        abs(kHeadY-kAY)
kDist    =        kPanning - ((kDist^0.5)*kPanning)
kPan     =        tanh((kHeadX - kAX)*kDist)
kPan     =        (kPan*0.5) + 0.5
a1D      *=       a(kPan)
a2D      *=       a(1-kPan)
; amplitude
kdB      =        kh^3 * (-kAmplitude)
; filter
kCF      scale    limit:k((1-kh),0,1), 14, kHFDistLoss
a1D      tone     a1D, cpsoct(kCF)
a2D      tone     a2D, cpsoct(kCF)
;reverb send
         chnmix   a1D*kRvbAmt*a(ampdbfs(kdB1*kRvbDiff)), "SendL"
         chnmix   a2D*kRvbAmt*a(ampdbfs(kdB1*kRvbDiff)), "SendR"
;amplitude scaling
a1D      *=       a(ampdbfs(kdB))
a2D      *=       a(ampdbfs(kdB))
endif

         outs     a1D,a2D

endin



instr 2 ; initialise xypads
chnset k(0.501), "X"
chnset k(0.501), "Y"
chnset k(0.501), "HeadX"
chnset k(0.501), "HeadY"
endin

instr 99
aIn1  chnget  "SendL"
aIn2  chnget  "SendR"
      chnclear "SendL"
      chnclear "SendR"
a1,a2 reverbsc aIn1,aIn2,gkRvbSize,gkRvbDamp
      outs     a1,a2
endin

</CsInstruments>

<CsScore>
i 1  0 [3600*24*7*52]
i 99 0 [3600*24*7*52]
</CsScore>

</CsoundSynthesizer>