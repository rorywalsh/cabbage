
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; Demolition
; Iain McCurdy, 2020

; dust noise source - decaying in amplitude and density
; two frequency bands: main (100 Hz) and sub (at 20 Hz)
; Mix of two frequency bands and clipped to create an overloading sound character when the explosion is loudest
; finally the explosion is lowpass filtered to remove some of the excessive edginess as a result of the clipping


; DETONATE - activate an explosion

; Fixed/Random - choose between random variation sounds and a sound determined by the on-screen controls



; If 'fixed' is selected:

; GLOBAL
; Duration - duration of the explosion
; Clip - amount of tanh clipping applied to the explosion sound
; Level
; Reverb

; LOW (sparse dust noise)
; Freq - Frequency of a bandpass filter applied to the dust noise
; Bandwidth - bandwidth of the bandpass filter

; HIGH
; Freq - Frequency of a bandpass filter applied to the dust noise
; Bandwidth - bandwidth of the bandpass filter

; Mix - mixed between the 'Low' and 'High' components




<Cabbage>
form caption("Demolition") size(1200, 690), pluginId("Exlp") colour(40,40,60)

#define RSliderStyle  trackerInsideRadius(.9), trackerOutsideRadius(1), markerColour(255,100,100), markerThickness(2), trackerColour(255,130,130), trackerThickness(4), colour(60,65,80), fontColour(200,200,200), textColour(200,200,200), outlineColour(50,50,50), valuetextBox(1)
#define HSliderStyle  trackerInsideRadius(.9), trackerOutsideRadius(1), markerColour(40,40,60), markerThickness(2), trackerColour(40,40,60), trackerThickness(4), colour(40,40,60), fontColour(200,200,200), textColour(200,200,200), outlineColour(40,40,60), valuetextBox(1)

; Buildings
image      bounds( 0, 0,0,0), colour(60,60,60), widgetArray("Building", 150), shape("sharp")

; Windows
image      bounds( 0, 0, 0, 0), colour(200,200,0), widgetArray("Window", 1000), shape("sharp")

; Falling Debris
image      bounds( 0, 0, 0, 0), colour(200,200,200), widgetArray("Debris", 50), shape("sharp")

; explosion widget array. Up to 100 explosions are available
image      bounds( 0, 0, 0, 0), colour(0,0,0), shape("ellipse"), widgetArray("Explosion", 1000)

; Lower Panel
image      bounds(  5,565,1190,125), colour(35,35,35), plant("High"), outlineColour("silver"), outlineThickness(4) corners(7) {
}

; Global controls
image      bounds( 10,570,260,115), colour(0,0,0,0), plant("Main"), outlineColour("silver"), outlineThickness(1) {
label      bounds(  0,  5,260, 15), text("G  L  O  B  A  L"), fontColour(200,200,200), align("centre")
rslider    bounds(  5, 30, 70, 70), text("Duration"), channel("Duration"),  range(1, 15, 5, 0.5),            $RSliderStyle colour(60, 65, 80, 255) fontColour(200, 200, 200, 255) markerColour(100, 100, 255, 255) outlineColour(50, 50, 50, 255) textColour(200, 200, 200, 255) trackerColour(130, 130, 255, 255) trackerInsideRadius(0.9) trackerThickness(4) valuetextBox(1)
rslider    bounds( 65, 30, 70, 70), text("Clip"),  channel("Clip"),  range(0, 0.7, 0.4),            $RSliderStyle colour(60, 65, 80, 255) fontColour(200, 200, 200, 255) markerColour(100, 100, 255, 255) outlineColour(50, 50, 50, 255) textColour(200, 200, 200, 255) trackerColour(130, 130, 255, 255) trackerInsideRadius(0.9) trackerThickness(4) valuetextBox(1)
rslider    bounds(125, 30, 70, 70), text("Level"), channel("Level"),  range(0, 5, 2.5, 0.5),            $RSliderStyle colour(60, 65, 80, 255) fontColour(200, 200, 200, 255) markerColour(100, 100, 255, 255) outlineColour(50, 50, 50, 255) textColour(200, 200, 200, 255) trackerColour(130, 130, 255, 255) trackerInsideRadius(0.9) trackerThickness(4) valuetextBox(1)
rslider    bounds(185, 30, 70, 70), text("Reverb"), channel("Reverb"),  range(0, 1, 0.3, 0.5),            $RSliderStyle colour(60, 65, 80, 255) fontColour(200, 200, 200, 255) markerColour(100, 100, 255, 255) outlineColour(50, 50, 50, 255) textColour(200, 200, 200, 255) trackerColour(130, 130, 255, 255) trackerInsideRadius(0.9) trackerThickness(4) valuetextBox(1)
}

; 'High' component controls
image      bounds(280,570,140,115), colour(0,0,0,0), plant("High"), outlineColour("silver"), outlineThickness(1) {
label      bounds(  0,  5,140, 15), text("L  O  W"), fontColour(200,200,200), align("centre")
rslider    bounds(  5, 30, 70, 70), text("Freq"),  channel("SubCF"),  range(5, 100, 20, 0.5, 0.01),            $RSliderStyle colour(60, 65, 80, 255) fontColour(200, 200, 200, 255) markerColour(100, 100, 255, 255) outlineColour(50, 50, 50, 255) textColour(200, 200, 200, 255) trackerColour(130, 130, 255, 255) trackerInsideRadius(0.9) trackerThickness(4) valuetextBox(1)
rslider    bounds( 65, 30, 70, 70), text("B.width"),  channel("SubBW"),  range(0.1, 10, 1, 0.5, 0.01),            $RSliderStyle colour(60, 65, 80, 255) fontColour(200, 200, 200, 255) markerColour(100, 100, 255, 255) outlineColour(50, 50, 50, 255) textColour(200, 200, 200, 255) trackerColour(130, 130, 255, 255) trackerInsideRadius(0.9) trackerThickness(4) valuetextBox(1)
}

; Mix slider
image      bounds(430, 570,130,115), colour(0,0,0,0), plant("Mix"), outlineColour("silver"), outlineThickness(1) {
label      bounds(  5, 35,105, 12), text("Mix"), fontColour(200,200,200), align("centre")
hslider    bounds(  5, 65,105, 20),    channel("Mix"),  range(0, 1, 0.5)
}

; 'High' component controls
image      bounds(570,570,140,115), colour(0,0,0,0), plant("High"), outlineColour("silver"), outlineThickness(1) {
label      bounds(  0,  5,140, 15), text("H  I  G  H"), fontColour(200,200,200), align("centre")
rslider    bounds(  5, 30, 70, 70), text("Freq"),  channel("MainCF"),  range(40, 1000, 100, 0.5, 1),    $RSliderStyle colour(60, 65, 80, 255) fontColour(200, 200, 200, 255) markerColour(100, 100, 255, 255) outlineColour(50, 50, 50, 255) textColour(200, 200, 200, 255) trackerColour(130, 130, 255, 255) trackerInsideRadius(0.9) trackerThickness(4) valuetextBox(1)
rslider    bounds( 65, 30, 70, 70), text("B.width"),  channel("MainBW"),  range(0.1,10, 1, 0.5, 0.001), $RSliderStyle colour(60, 65, 80, 255) fontColour(200, 200, 200, 255) markerColour(100, 100, 255, 255) outlineColour(50, 50, 50, 255) textColour(200, 200, 200, 255) trackerColour(130, 130, 255, 255) trackerInsideRadius(0.9) trackerThickness(4) valuetextBox(1)
}


; Red/white striped area and detonate button
image      bounds(620,505,500,350), colour(0,0,0,0), plant("Detonate") {
image      bounds(  66,179, 25,  54), colour("white"),   rotate(15,50,3.14)
image      bounds( 100,180, 25, 100), colour(200,50,50), rotate(15,50,3.14)
image      bounds( 133,180, 25, 100), colour("white"),   rotate(15,50,3.14)
image      bounds( 166,180, 25, 100), colour(200,50,50), rotate(15,50,3.14)
image      bounds( 199,180, 25, 100), colour("white"),   rotate(15,50,3.14)
image      bounds( 232,180, 25, 100), colour(200,50,50), rotate(15,50,3.14)
image      bounds( 230,140, 25,  50), colour("white"),   rotate(15,50,3.14)

image      bounds(102,  72, 20, 100), colour(200, 50, 50, 255)
image      bounds(102,  72, 210, 20), colour(200,50,50)
image      bounds(302,  72, 20, 100), colour(200,50,50)
image      bounds(102, 152, 210, 20), colour(200,50,50)

button     bounds( 170,108, 80, 30), text("DETONATE","DETONATE"), outlineThickness(4), outlineColour("silver"), shape("ellipse"), channel("Detonate"), value(0), latched(0), fontColour:0(200,255,255), fontColour:1( 55, 55, 55), colour:0(200,0,0), colour:1(0,0,0)
}

; Fixed/random toggle
image      bounds( 955,570,110,125), colour(0,0,0,0), plant("Toggle"), outlineColour("silver"), outlineThickness(1) {
checkbox   bounds(  20, 40, 80, 15), text("FIXED"),  shape("sharp"), channel("Fixed"),  value(0), fontColour:1(220,220, 20), radioGroup(1), colour(255,255,50)
checkbox   bounds(  20, 60, 80, 15), text("RANDOM"), shape("sharp"), channel("Random"), value(1), fontColour:1(220,220, 20), radioGroup(1), colour(255,255,50)
}

; Dust
image      bounds( 1075,570,110,115), colour(0,0,0,0), plant("Toggle"), outlineColour("silver"), outlineThickness(1) {
label      bounds(    0,  5,110, 15), text("D  U  S  T"), fontColour(200,200,200), align("centre")
rslider    bounds(   20, 30, 70, 70), text("Level"),  channel("DustLevel"),  range(0, 2, 1, 0.5, 0.01),            $RSliderStyle colour(60, 65, 80, 255) fontColour(200, 200, 200, 255) markerColour(100, 100, 255, 255) outlineColour(50, 50, 50, 255) textColour(200, 200, 200, 255) trackerColour(130, 130, 255, 255) trackerInsideRadius(0.9) trackerThickness(4) valuetextBox(1)
}
</Cabbage>


<CsoundSynthesizer>

<CsOptions>
-n -d -+rtmidi=NULL -M0 -m0d --midi-key-cps=4 --midi-velocity-amp=5
</CsOptions>

<CsInstruments>

ksmps  =    32
nchnls =    2
0dbfs  =    1

       seed 0 

giExplosionNumber init 1

instr  1

; START LOOP
iCnt = 1
; BUILD SKYLINE
while iCnt<=150 do
iHeight random 0, 1
iHeight  =      iHeight^5 * 470 + 60
iHeight  =      int(iHeight)
iY       =      560-int(iHeight)
iWidth   =      int(random:i(50, 80))
iCol     random 20, 20
iX       random -100, 1200
Smsg     sprintf "bounds(%d,%d,%d,%d), colour(%d,%d,%d)", iX, iY, iWidth, iHeight, iCol, iCol, iCol
SID      sprintf "Building_ident%d", iCnt
         chnset Smsg, SID

; CREATE WINDOWS
iWWidth random 1, 4
iWHeight random 3, 6
iWX random iX + 3, iX + iWidth - iWWidth - 3
iWY linrand 1
iWY =       (iWY * iHeight * 0.25) + iY + 5
iWR = 200
iWG = 200
iWB =   0
iAlpha random 100, 255
Smsg sprintf  "bounds(%d,%d,%d,%d), colour(%d,%d,%d,%d)", iWX, iWY, iWWidth, iWHeight, iWR, iWG, iWB, iAlpha
SID  sprintf  "Window_ident%d", iCnt * 2
     chnset   Smsg, SID

iCnt += 1
od
; END OF LOOP

; READ IN WIDGETS
 gkMix      chnget  "Mix"
 gkClip     chnget  "Clip"
 gkMainCF   chnget  "MainCF"
 gkMainBW   chnget  "MainBW"
 gkLevel    chnget  "Level"
 gkDuration chnget  "Duration"
 gkSubCF    chnget  "SubCF"
 gkSubBW    chnget  "SubBW"
 gkReverb   chnget  "Reverb"
 kFixed     chnget "Fixed"
 kRandom    chnget "Random"
 gkDustLevel chnget "DustLevel"
 
; DETONATE AN EXPLOSION
 kDetonate chnget "Detonate"
; kDetonate metro  5
 if trigger:k(kDetonate,0.5,0)==1 && kFixed==1 then
  ;                          p4            p5            p6              p7                p8       p9      p10              p11     p12      p3
  event "i",2,0, gkDuration, gkDuration*10,gkMix,        gkClip,         gkMainCF,         gkMainBW,gkLevel,gkSubCF,         gkSubBW,gkReverb,gkDustLevel
 endif

; DETONATE AN EXPLOSION (RANDOM)
 if trigger:k(kDetonate,0.5,0)==1 && kRandom==1 then
  kDuration  =        exprand:k(4) + 1 
  ;                          p4          p5            p6              p7                p8       p9      p10               p11    p12                 p13
  event "i",2,0, kDuration, kDuration*10,random:k(0,1),random:k(0,0.7),exprand:k(200)+40,1,       0.3,    exprand:k(30) + 5,1,     random:k(0.05,0.25),random:k(0,2)
 endif

endin



instr  2 ; Explosion

; explosion graphic
kR     transeg   40, p3*0.05,  4, 105,  p3*0.05, 4,  200, p3*0.05, 4, 205, p3*0.2, -4, 40
kG     transeg   40, p3*0.05,  4, 55,   p3*0.05, 4,  200, p3*0.05, 4, 205, p3*0.2, -4, 40
kB     transeg   60, p3*0.05,  4, 0,    p3*0.05, 4,  0,   p3*0.05, 4, 205, p3*0.2, -4, 60
kAlp   transeg   30, p3*0.05,  4, 80,   p3*0.05, 4,  150, p3*0.05, 4, 255, p3*0.2, -4, 255 ; alpha
iSize  =        p4
iInitX random   0, 1200
iPan   =        iInitX / 1200
iInitY random   80, 450-iSize*0.5

kSize  transeg   0, 5, -18, iSize
kX     =         iInitX - (kSize * 0.5)
kY     =         iInitY - (kSize * 0.5)

SID    sprintf "Explosion_ident%d", giExplosionNumber

if metro:k(20)==1 then
 Smsg  sprintfk  "bounds(%d,%d,%d,%d), colour(%d,%d,%d,%d)",kX,kY,kSize,kSize,kR,kG,kB,kAlp
 chnset Smsg, SID
endif

; Falling debris graphics
iDebrisCount = 1
iDebrisDens =  (limit:i(iSize/100,0,1) ^ 2) * 100

while iDebrisCount<=iDebrisDens do
event_i "i", 4, random(0.2,1), 7, iInitX, iInitY, iSize, iDebrisCount
iDebrisCount += 1
od

giExplosionNumber += 1

kAmp   expon  3, p3, 0.0001              ; amplitude envelope
kDens  expon  5000, p3, 1                ; density envelope

iMix    = p5
iClip   = p6
iMainCF = p7
iMainBW = p8
iLevel  = p9
iSubCF  = p10
iSubBW  = p11
iReverb = p12
iDustLevel = p13

; TRIGGER DUST SHOWER
;                                               p4                p5       p6
event_i "i",3,random:i(0.5,1.2),random:i(13,20),iLevel*iDustLevel,iReverb, iPan


; Explosion sound
#define EXPLOSION(CHAN)
#
aNoise dust2  kAmp*gkLevel, kDens ; create some dust noise
aSub   reson  aNoise*500, iSubCF, iSubCF * iSubBW, 1                   ; sub bass filtered band
aMain  reson  aNoise*10, iMainCF, iMainCF * iMainBW, 1  ; main freq filtered band
aMix   sum    aMain*iMix + aSub*(1-iMix)                          ; mix sub and main frequency bands
aMix   clip   aMix, 0, 0.9 - iClip                              ; clip the signal
aMix   tone   aMix, expon:k(3000, p3, 1)                            ; lowpass filter the signal
a$CHAN =      aMix * iLevel
#

$EXPLOSION(1)
$EXPLOSION(2)

a1     *=     (1-iPan)
a2     *=     iPan
       outs    a1, a2
       chnmix a1 * iReverb, "Send1"
       chnmix a2 * iReverb, "Send2"
endin


; Falling Debris (sound)
instr 3
iLevel  = p4
iReverb = p5
iPan    = p6
iDens =           linrand:i(2000) + 10
kAmp  expseg       0.1, 0.3, 1, p3-0.3, 0.01
kDens expseg      10, 0.5, iDens, p3-0.5, 10
aSig  dust2       kAmp*iLevel*0.08*gkLevel, kDens
iCF   random      400, 2000
aCF   exprand     iCF
aCF   limit       aCF, 20, 5000
aBW   exprand     1
aF    reson       aSig, aCF, aCF*(1+aBW), 1
aLPF  expon       6000, p3, 20
aF    butlp       aF, aLPF
aL,aR pan2       aF, iPan + randi(0.2,50)
       outs      aL*0.3, aR*0.3
       chnmix    aL * iReverb * 2, "Send1"
       chnmix    aR * iReverb * 2, "Send2"
endin

; Falling Debris (graphics)
instr 4
iInitX = p4
iInitY = p5
iSize  = p6
iDebrisCount = p7
iCol  = 255
kAl   expon  100,6,1
iXOff gauss  iSize/2
iYOff gauss  iSize/2
kX transeg iInitX+iXOff, 2, -4, iInitX + iXOff + bexprnd:i(0.3*iSize)
kY init iInitY + iYOff
iWid = random:i(1,3)
iHei = random:i(1,3)
kHei expon iHei,2,iHei*8
Smsg  sprintfk "bounds(%d,%d,%d,%d), colour(%d,%d,%d,%d)",kX,kY,iWid,kHei,iCol,iCol,iCol,kAl
SID   sprintf "Debris_ident%d", iDebrisCount
      chnset   Smsg, SID
kY += random:i(0.04,0.07)

endin

instr 99
aInL  chnget   "Send1"
      chnclear "Send1"
aInR  chnget   "Send2"
      chnclear "Send2"

aDelL,aDelR init 0
iFB   =        0.3
aDelL delay    aInL + aDelL * iFB, 0.6371
aDelR delay    aInL + aDelL * iFB, 0.9371


aL,aR reverbsc  aInL+aDelR, aInR+aDelL, 0.8, 6000

aL    butlp     aL, 2000
aR    butlp     aR, 2000 
      
      outs      aL, aR
endin

</CsInstruments>

<CsScore>
i 1 0 3600
i 99 0 3600
</CsScore>

</CsoundSynthesizer>
