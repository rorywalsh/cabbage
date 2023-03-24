
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; HRTF3D.csd
; Written by Iain McCurdy, 2012.

; 3D rendering of a mono input from the left channel.
; Uses head related transfer function transformation (HRTF) and the hrtfmove2 opcode
; Azimuth (horizontal angle with respect to a front-facing listener) and
;  elevation (vertical angle) are both specified in degrees and can be controlled using the XY pad control

; N STFT Overlaps (number of short time fourier transform overlaps)  - larger numbers provide smoother 
;  interpolation at the expense of computation speed

; Head radius (in cm) varies the interaural time delay and is particularly impactful on lower frequencies.
;  the default is 9 cm which is the average for an adult human.

; The display widgets on right attempts to visualise the movement (left-right, up-down, near-far) 
;   of the source sound.
; Brightness of the source sound widget (red circle) varies according to the intensity of the input signal.
; Size of the source sound widget reflects distance front to back.

<Cabbage>
form caption("HRTF 3D"), size(800, 450), pluginId("HRTF"), colour(0,0,0) guiMode("queue")
xypad bounds(0,  0, 400, 400), channel("Az","Elev"), text("X=Azimuth (degs) | Y=Elev. (degs)"), rangeX(-180,  180,   0), rangeY(-40, 90, 0)

image bounds(500,180,  30, 30), colour(200,50,50), shape("ellipse"), channel("SoundSource")

image bounds(560,145,100,100) colour(0,0,0,0) channel("Face") {
image bounds(   2,  0,  81, 100), colour(200,200,150), shape("ellipse")
image bounds(   0, 30,   6,  30), colour(200,200,150), shape("ellipse")
image bounds(  79, 30,   6,  30), colour(200,200,150), shape("ellipse")
image bounds(   20,  33,  20,  10), colour(10,10,10), shape("ellipse")
image bounds(   50,  33,  20,  10), colour(10,10,10), shape("ellipse")

image bounds(   38,  55,  15,   6), colour(10,10,10), shape("ellipse")
image bounds(   38,  52,  15,   6), colour(200,200,150), shape("ellipse")


image bounds(   25,  65,  40,  12), colour(10,10,10), shape("ellipse")
image bounds(   25,  62,  40,   8), colour(200,200,150)
}

image bounds(  0,400,800,50), colour(40,40,40), outlineThickness(2), outlineColour("Silver")
{
hslider bounds(  5, 10, 390, 30), channel("HeadRadius"), text("Head Radius (cm)"), range(1,30,9) valueTextBox(1)
hslider bounds(405, 10, 390, 30), channel("Overlaps"), text("N. STFT Overlaps"), range(2,16,4,1,1) valueTextBox(1)
}

</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-d -n
</CsOptions>

<CsInstruments>

; sr set by host
ksmps    =     16
nchnls   =     2
0dbfs    =     1

;Author: Iain McCurdy (2012)

instr    1

kAz        cabbageGetValue        "Az"                                  ; READ WIDGETS...
;kAz        scale                  kAz, 180, -180
kElev      cabbageGetValue        "Elev"
;kElev      scale                  kElev, 90, -40

; audio input
;asrc       diskin2                 "Benjamin.wav",1,0,1
asrc       inch                    1
iSR   =    sr
if iSR==44100 then
 SdataL = "hrtf-44100-left.dat"
 SdataR = "hrtf-44100-right.dat"
elseif iSR==48000 then
 SdataL = "hrtf-48000-left.dat"
 SdataR = "hrtf-48000-right.dat"
else
 SdataL = "hrtf-96000-left.dat"
 SdataR = "hrtf-96000-right.dat"
endif

kHeadRadius, kTHeadRadius cabbageGetValue "HeadRadius"
kHeadRadius               init            9
kOverlaps, kTOverlaps     cabbageGetValue "Overlaps"
kOverlaps                 init            4

if (kTHeadRadius+kTOverlaps)>0 then
 reinit RESTART
endif
RESTART:
aleft, arig hrtfmove2    asrc, kAz, kElev, SdataL, SdataR, i(kOverlaps), i(kHeadRadius), sr
rireturn
           outs          aleft, arig                       ; SEND AUDIO TO OUTPUTS

kRMS       rms           asrc
;printk     0.1,kRMS

kX  mirror  kAz/90, -1, 1
kSize =     (180 - abs(kAz))/180

kSizeE   =  (180 - abs(kElev))/180

; front/back
kFB   =     round(kSize>0.5?1:0)

cabbageSet changed:k(kAz,kElev), "SoundSource", "bounds", kX*180 + 590, 180 - kElev*2, 15 + kSize*30*kSizeE, 15 + kSize*30*kSizeE
cabbageSet changed:k(kRMS), "SoundSource", "colour", 100 + kRMS*55*4, 10 + kRMS*205*4, 10 + kRMS*205*4
cabbageSet trigger:k(kFB,0.5,0), "SoundSource", "toFront"
cabbageSet trigger:k(kFB,0.5,1), "Face", "toFront"

endin
        
</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
</CsScore>

</CsoundSynthesizer>