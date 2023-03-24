
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; Rhythmicon.csd
; Written by Iain McCurdy, 2013, 2021.

; Recreation of Henry Cowell and Leon Theremin's Rhythmicon instrument (http://en.wikipedia.org/wiki/Rhythmicon)

; CONTROLS
; --------
; BASE    --    note number of the fundamental (lowest note)
; RATE    --    base rate (same as rate for fundamental)
; LEVEL    --    output level

; OPEN FILE -- load a sound file to be used in place of the internal synthesiser. Choose a short sound file, mono or stereo.
;           -- choose between the internal synth and a chosen sound file using the drop-down menu

; DESCRIPTION
; -----------
; Each note above the fundamental (C3) divides the base rate increasingly according to the arithmetic progression: 2,3,4,5...
; Therefore if C3 plays crotchets (quarter notes), C#3 plays quavers (eighth notes), D3 triplets and so on.
; The pitches of each note (in cycles per second) follow a simlar progression.
; If C3 plays a 200Hz tone, C#3 plays 400Hz, D 600Hz and so on.

; The 17th note (E4) does not play a note but instead engages a syncopation effect.  
; I have been unable to find a clear technical specification of how this was implemented but have nonetheless made my best guess.

<Cabbage>
form caption("...") size(159,230), pluginId("Ryth")

label   bounds(  5,  3, 18, 22), text("R"), fontColour(red)
label   bounds( 20,  5, 18, 22), text("H"), fontColour(yellow)
label   bounds( 35,  2, 18, 22), text("Y"), fontColour(orchid)
label   bounds( 50,  4, 18, 22), text("T"), fontColour(LightGreen)
label   bounds( 65,  2, 18, 22), text("H"), fontColour(OrangeRed)
label   bounds( 82,  6, 18, 22), text("M"), fontColour(orange)
label   bounds( 97,  1, 18, 22), text("I"), fontColour(SkyBlue)
label   bounds(110,  4, 18, 22), text("C"), fontColour(Tomato)
label   bounds(125,  1, 18, 22), text("O"), fontColour(LightGreen)
label   bounds(140,  3, 18, 22), text("N"), fontColour(violet)

label   bounds( 81, 24,  78, 8), text("Iain McCurdy [2013]"), fontColour("white")

rslider bounds(  3, 45, 55, 55), channel("base"), textBox(1), valueTextBox(1), range(0, 127,36, 1,1),fontColour("white"),textColour("white"),colour(orange), trackerColour(orange)
label   bounds( 19, 34, 30, 10), text("BASE"), fontColour(white)
rslider bounds( 52, 45, 55, 55), channel("rate"), textBox(1), valueTextBox(1), range(0.01, 5.00, 0.5, 0.5, 0.01),fontColour("white"),textColour("white"),colour(orange), trackerColour(orange)
label   bounds( 68, 34, 30, 10), text("RATE"), fontColour(white)
rslider bounds(102, 45, 55, 54), channel("level"), textBox(1), valueTextBox(1), range(0, 1.00, 0.5, 0.5, 0.01),fontColour("white"),textColour("white"),colour(orange), trackerColour(orange)
label   bounds(115, 34, 30, 10), text("LEVEL"), fontColour(white)

filebutton bounds(  8,115, 70, 20), text("OPEN FILE","OPEN FILE") channel("filename"), shape("ellipse"), fontColour:0(150,150,250), fontColour:1(205,205,255), colour:0(10,10,80), colour:1(0,0,150)
combobox   bounds( 83,115, 70, 20),  channel("Sound"),text("Synth","File"), value(1)

checkbox bounds(  0,151,  9, 10), channel("k1")  value(0) shape(square) colour:0( 50, 50,  0) colour:1(255,255, 50)
checkbox bounds(  9,151, 10, 10), channel("k2")  value(0) shape(square) colour:0( 50, 50,  0) colour:1(255,255, 50)
checkbox bounds( 19,151,  9, 10), channel("k3")  value(0) shape(square) colour:0( 50, 50,  0) colour:1(255,255, 50)
checkbox bounds( 28,151, 10, 10), channel("k4")  value(0) shape(square) colour:0( 50, 50,  0) colour:1(255,255, 50)
checkbox bounds( 38,151, 10, 10), channel("k5")  value(0) shape(square) colour:0( 50, 50,  0) colour:1(255,255, 50)
checkbox bounds( 48,151,  8, 10), channel("k6")  value(0) shape(square) colour:0( 50, 50,  0) colour:1(255,255, 50)
checkbox bounds( 56,151, 10, 10), channel("k7")  value(0) shape(square) colour:0( 50, 50,  0) colour:1(255,255, 50)
checkbox bounds( 66,151,  8, 10), channel("k8")  value(0) shape(square) colour:0( 50, 50,  0) colour:1(255,255, 50)
checkbox bounds( 74,151, 10, 10), channel("k9")  value(0) shape(square) colour:0( 50, 50,  0) colour:1(255,255, 50)
checkbox bounds( 84,151,  9, 10), channel("k10") value(0) shape(square) colour:0( 50, 50,  0) colour:1(255,255, 50)
checkbox bounds( 93,151, 10, 10), channel("k11") value(0) shape(square) colour:0( 50, 50,  0) colour:1(255,255, 50)
checkbox bounds(103,151,  9, 10), channel("k12") value(0) shape(square) colour:0( 50, 50,  0) colour:1(255,255, 50)
checkbox bounds(112,151,  9, 10), channel("k13") value(0) shape(square) colour:0( 50, 50,  0) colour:1(255,255, 50)
checkbox bounds(121,151, 10, 10), channel("k14") value(0) shape(square) colour:0( 50, 50,  0) colour:1(255,255, 50)
checkbox bounds(131,151,  9, 10), channel("k15") value(0) shape(square) colour:0( 50, 50,  0) colour:1(255,255, 50)
checkbox bounds(140,151, 10, 10), channel("k16") value(0) shape(square) colour:0( 50, 50,  0) colour:1(255,255, 50)
checkbox bounds(150,151, 10, 10), channel("k17") value(0) shape(square) colour:0( 50,  0,  0) colour:1(255, 50, 50)                                                                                          

keyboard bounds(-561, 160,1200, 70)
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-dm0 -n -+rtmidi=NULL -M0
</CsOptions>

<CsInstruments>

nchnls   =   2
0dbfs    =   1
massign  0,1

; Author: Iain McCurdy (2013)

giHalfSine ftgen 0,0,4096,9,0.5,1,0

opcode    SwitchPort, k, kkk
 kin,kUpPort,kDnPort    xin
 kold        init   0
 kporttime   =      (kin<kold?kDnPort:kUpPort)
 kout        portk  kin, kporttime
 kold        =      kout
             xout   kout
endop

gSfilepath    init    ""
giReady       init    0               ; if no string has yet been loaded giReady will be zero
giVels[]      init    128  

instr    1                            ; Listen for midi note changes. Turn lights on or off. Start the rhythmicon of at least 1 note is held.
 gSfilepath    chnget    "filename"   ; read in file path string from filebutton widget    
 if changed:k(gSfilepath)==1 then     ; call instrument to update waveform viewer  
  event "i", 99, 0, 0
 endif
 
 gkSound  chnget  "Sound"
 
 inum    notnum                    ; read in midi note number
 ivel    ampmidi     1
 
#define  LightToggle(N)
#
if inum=(59+$N) then              ; if note number played on the keyboard corresponds to 'this' toggle light... (note that first note will be C3 (59 + 1)
 ik$N    chnget    "k$N"          ; read current value for 'this' toggle light (at i-time)
         chnset    1 - ik$N,"k$N" ; write the inverse to it (i.e. toggle it)
 giVels[inum-59]   =  ivel           
endif
#

$LightToggle(1)                   ; expand macro for all 16 layers
$LightToggle(2)
$LightToggle(3)
$LightToggle(4)
$LightToggle(5)
$LightToggle(6)
$LightToggle(7)
$LightToggle(8)
$LightToggle(9)
$LightToggle(10)
$LightToggle(11)
$LightToggle(12)
$LightToggle(13)
$LightToggle(14)
$LightToggle(15)
$LightToggle(16)
$LightToggle(17)

kk1    chnget    "k1"
kk2    chnget    "k2"
kk3    chnget    "k3"
kk4    chnget    "k4"
kk5    chnget    "k5"
kk6    chnget    "k6"
kk7    chnget    "k7"
kk8    chnget    "k8"
kk9    chnget    "k9"
kk10    chnget    "k10"
kk11    chnget    "k11"
kk12    chnget    "k12"
kk13    chnget    "k13"
kk14    chnget    "k14"
kk15    chnget    "k15"
kk16    chnget    "k16"

ksum    =    kk1 + kk2 + kk3 + kk4 + kk5 + kk6 + kk7 + kk8 + kk9 + kk10 + kk11 + kk12 + kk13 + kk14 + kk15 + kk16
if trigger(ksum,0.5,0)==1 && timeinstk()>1 then
 event "i", 2, 0, -1
endif
if trigger(ksum,0.5,1)==1 then
  turnoff2      2, 0, 1
endif

endin




instr    99    ; load sound file
 gichans     filenchnls gSfilepath                 ; derive the number of channels (mono=1,stereo=2) in the sound file
 gitableL    ftgen      1,0,0,1,gSfilepath,0,0,1
 giFileLen   filelen    gSfilepath                 ; derive the file duration in seconds
 if gichans==2 then
  gitableR   ftgen      2,0,0,1,gSfilepath,0,0,2
 endif
 giReady     =    1                                ; if no string has yet been loaded giReady will be zero
endin



instr    2
kporttime  linseg  0,0.001,0.05
kbase      chnget  "base"             ; base pitch as a note number
krate      chnget  "rate"             ; rate of pulsation of fundemental
krate      portk   krate, kporttime
klevel     chnget  "level"            ; output amplitude level
ksync      chnget  "k17"              ; syncopation mode
kBaseFrq   =       cpsmidinn(kbase)
kBaseFrq   portk   kBaseFrq,kporttime
gkAmp      chnget  "level"
amix       =       0

#define    LAYER(N)
#
kk$N       chnget     "k$N"
kk$N       SwitchPort kk$N, kporttime * 0.01, kporttime * 0.5
ktrig      metro      krate * $N
kn         =          $N                                               ; layer number as a variable
if ksync=1 && kn>1 then                                                ; if syncopation is on and this isn't the first layer (syncopation irrelevant for first layer)
 ktrig     vdelayk    ktrig, (1/(krate*$N))/$N, 1/0.1                  ; add a syncopation delay for the metronome trigger for this layer
endif
;kenv       loopseg   $N/60, ktrig, 0, 0,0.125/krate, 1, 0.875/krate, 0,60, 0  ; retriggering envelope
kenv       loopseg   $N/60, ktrig, 0, 0,1/(32*krate), 1, 31/(32*krate), 0,60, 0  ; retriggering envelope
;kenv       pow       kenv, 0.25
aenv       interp    kenv * kk$N                                       ; interpolate to a-rate (for better smoothness)
kcps       =         kBaseFrq * $N                                     ; pitch for this layer (in cycles per second)
asig       vco2      gkAmp, kcps, 2, 0.1                                ; generate a tone (pulse wave)
kcf        limit     cpsoct( octcps(kcps * 3) *kenv) ,1, 10000         ; filter envelope
asig       clfilt    asig,kcf,0,2                                      ; butterworth lowpass filter the sound
asig       =         asig * aenv                                       ; apply amplitude envelope
amix       =         amix + asig
#

; play file
#define    LAYER_FILE(N)
#
kk$N       chnget     "k$N"
;kk$N       SwitchPort kk$N, kporttime * 0.01, kporttime * 0.5
ktrig      metro      krate * $N
kn         =          $N                                               ; layer number as a variable
if ksync=1 && kn>1 then                                                ; if syncopation is on and this isn't the first layer (syncopation irrelevant for first layer)
 ktrig     vdelayk    ktrig, (1/(krate*$N))/$N, 1/0.1                  ; add a syncopation delay for the metronome trigger for this layer
endif
kmlt       =         (kBaseFrq/cpsoct(6)) * $N                         ; pitch for this layer (in cycles per second)
schedkwhen ktrig*kk$N,0,0,3,0,0.01,kmlt,$N
#

if gkSound==1 then
$LAYER(1)                        
$LAYER(2)
$LAYER(3)
$LAYER(4)
$LAYER(5)
$LAYER(6)
$LAYER(7)
$LAYER(8)
$LAYER(9)
$LAYER(10)
$LAYER(11)
$LAYER(12)
$LAYER(13)
$LAYER(14)
$LAYER(15)
$LAYER(16)
elseif gkSound==2 && giReady==1 then
$LAYER_FILE(1)                        
$LAYER_FILE(2)
$LAYER_FILE(3)
$LAYER_FILE(4)
$LAYER_FILE(5)
$LAYER_FILE(6)
$LAYER_FILE(7)
$LAYER_FILE(8)
$LAYER_FILE(9)
$LAYER_FILE(10)
$LAYER_FILE(11)
$LAYER_FILE(12)
$LAYER_FILE(13)
$LAYER_FILE(14)
$LAYER_FILE(15)
$LAYER_FILE(16)
endif

    xtratim    0.5
    outs       amix * klevel, amix * klevel
    endin    

instr 3
 iMlt =        p4
 iAmp =        giVels[p5]
 iCF  =        iAmp*10 + 4
 iLen filelen  gSfilepath
 p3   =        iLen/iMlt
 aPtr line     0,p3,1
 aL   tablei   aPtr,1,1
 if gichans==2 then
  aR   tablei   aPtr,1,2
 else
  aR   =        aL
 endif
 aL    tone     aL, cpsoct(iCF)
 aR    tone     aR, cpsoct(iCF)
 
       outs     aL*iAmp*gkAmp, aR*iAmp*gkAmp
endin

</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
;f 0 3600
</CsScore>

</CsoundSynthesizer>