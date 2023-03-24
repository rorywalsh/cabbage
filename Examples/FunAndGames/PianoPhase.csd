
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; Piano Phase
; Iain McCurdy, 2023

; Recreation of Steve Reich's 1967 piece 'Piano Phase'
; A simple 12-semiquaver theme is played by two pianos simultaneously.
; One of the pianists gradually increases their tempo so that their playing moves out of phase and ahead of the other.
; As the semiquavers move into sync in different alignments between the two pianos a range of harmonic possibilities are heard.
; Other interesting effects are observed between these alignments, in particular when the semiquavers are 50% out of sync,
;  demisemiquaver rhythms are produced.


<Cabbage>
form caption("Piano Phase") size(1000,480), colour( 20, 20, 25) pluginId("RePh"), guiRefresh(8)
label bounds(200,10,300,20), text("NEEDS SAMPLE RATE = 44100!")
#define RSLIDER_STYLE valueTextBox(1)

button     bounds( 10, 10, 80, 20), text("PLAY","PLAY"), channel("PlayStop"), value(0), latched(1), fontColour:0(50,100,50), fontColour:1(205,255,205), colour:0(0,10,0), colour:1(0,150,0)
combobox   bounds(100, 10,100, 20),  channel("PlayMode"),text("Freq Domain","Time Domain"), value(1)

image  bounds(  5, 40,490,430), outlineThickness(2), colour(0,0,0,0)
{
nslider bounds(100, 10, 70, 30), text("Tempo 1"), channel("Tempo1"), range(0,480,110,1,0.01)
line    bounds(245, 15,  1, 25), colour("grey")
nslider bounds(305, 10, 70, 30), text("Tempo 2"), channel("Tempo2"), range(0,480,110,1,0.01)
hslider bounds( 10, 50,470, 30), text("Phase 1 (semiquavers)"), channel("Phase1"), range(0,12,0,1,0.01), valueTextBox(1)
hslider bounds( 10, 90,470, 30), text("Phase 2 (semiquavers)"), channel("Phase2"), range(0,12,0,1,0.01), valueTextBox(1)

checkbox bounds( 13,139,125, 15), text("Auto Phase Shift 1"), channel("AutoPhase1"), colour(255,255,100)
hslider  bounds(150,130,320, 30), text("Speed 1"), channel("PhaseSpeed1"), range(-0.1,0.1,0,1,0.001)
checkbox bounds( 13,179,125, 15), text("Auto Phase Shift 2"), channel("AutoPhase2"), colour(255,255,100)
hslider  bounds(150,170,320, 30), text("Speed 2"), channel("PhaseSpeed2"), range(-0.1,0.1,0,1,0.001)

image  bounds(  5,220,480,100), outlineThickness(2), colour(0,0,0,0), file("PianoPhase.png")
image  bounds(  5,220,  4,100), outlineThickness(0), colour(100,0,0), identChannel("PhaseIndic1")
image  bounds(  5,325,480,100), outlineThickness(2), colour(0,0,0,0), file("PianoPhase.png")
image  bounds(  5,325,  4,100), outlineThickness(0), colour(100,0,0), identChannel("PhaseIndic2")

}

image  bounds(505, 40,490,430), outlineThickness(2), colour(0,0,0,0)
{
rslider bounds(  0, 20, 90, 90), text("Amp 1"), channel("Amp1"), range(0,1,0.7), $RSLIDER_STYLE
button  bounds( 90, 55, 60, 22), text("MUTE 1","MUTE 1"), channel("Mute1"), value(0), latched(1), fontColour:0(50,50,100), fontColour:1(205,205,255), colour:0(0,0,10), colour:1(0,0,150)
rslider bounds(150, 20, 90, 90), text("Pan 1"), channel("Pan1"), range(0,1,0.5), $RSLIDER_STYLE

line    bounds(245, 30,1,70), colour("grey")

rslider bounds(250, 20, 90, 90), text("Amp 2"), channel("Amp2"), range(0,1,0.7), $RSLIDER_STYLE
button  bounds(340, 55, 60, 22), text("MUTE 2","MUTE 2"), channel("Mute2"), value(0), latched(1), fontColour:0(50,50,100), fontColour:1(205,205,255), colour:0(0,0,10), colour:1(0,0,150)
rslider bounds(400, 20, 90, 90), text("Pan 2"), channel("Pan2"), range(0,1,0.5), $RSLIDER_STYLE

hslider bounds( 10,140,470, 30), text("Semitone 1"), channel("Semi1"), range(-12,12,0,1,1)
hslider bounds( 10,180,470, 30), text("Semitone 2"), channel("Semi2"), range(-12,12,0,1,1)
hslider bounds( 10,230,470, 30), text("Cents 1"), channel("Cent1"), range(-100,100,0,1,1)
hslider bounds( 10,270,470, 30), text("Cents 2"), channel("Cent2"), range(-100,100,0,1,1)
}

</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-n -+rtmidi=NULL -M0 -dm0
</CsOptions>

<CsInstruments>

; sr is set by host
ksmps = 16
nchnls = 2
0dbfs = 1

giRamp ftgen 0,0,4096,7,0,4096,1
gkBaseTempo init 110
giIndicY = 220

giFFTSize  =         4096
giOverlaps =         4
giFFTWin   =         1

gaSendL  init 0
gaSendR  init 0
gkRvbSend init 0.4

instr 1

 giLen1        = filelen:i("PPhase1.pvx") / ( 1 + 1024/sr)
 giLen2        = filelen:i("PPhase2.pvx") / ( 1 + 1024/sr)

 gkTempo1   chnget    "Tempo1"
 gkTempo2   chnget    "Tempo2"
 gkPhase1   =         chnget:k("Phase1") / 12
 gkPhase2   =         chnget:k("Phase2") / 12
 gkAmp1     chnget    "Amp1"
 gkAmp2     chnget    "Amp2"
 gkPan1     chnget    "Pan1"
 gkPan2     chnget    "Pan2"
 gkMute1    chnget    "Mute1"
 gkMute2    chnget    "Mute2"
 gkSemi1    chnget    "Semi1"
 gkSemi2    chnget    "Semi2"
 gkCent1    chnget    "Cent1"
 gkCent2    chnget    "Cent2"
 gkAutoPhase1    chnget    "AutoPhase1"
 gkAutoPhase2    chnget    "AutoPhase2"
 gkPhaseSpeed1    chnget    "PhaseSpeed1"
 gkPhaseSpeed2    chnget    "PhaseSpeed2"
 kPlayMode        chnget    "PlayMode"
 
 kInsNum  =  3 + kPlayMode

 if changed:k(kPlayMode)==1 then
  turnoff2 4,0,1
  turnoff2 5,0,1
  chnset k(0), "PlayStop"
 endif

 kPlayStop  chnget    "PlayStop"
 if changed:k(kPlayStop)==1 then
  if kPlayStop==1 then
   event "i",kInsNum,0,-1
  else
   turnoff2 kInsNum,0,1
  endif
 endif

endin


instr  4 ; spectral domain playback
 kPortTime  linseg    0, 0.001, 0.1

 ;; PIANO 1
 if gkAutoPhase1==1 then
  kPhs1      phasor   gkPhaseSpeed1
             chnset   kPhs1, "Phase1"
 else
  kPhs1      portk     gkPhase1, kPortTime
 endif
 kPBratio1  =         (gkTempo1/gkBaseTempo)
 aPtr1      osciliktp kPBratio1/giLen1, giRamp, kPhs1
 ;display phase
 if metro:k(32)==1 then
  Smsg   sprintfk  "pos(%d,%d)", k(aPtr1)*480 + 5, giIndicY
         chnset Smsg, "PhaseIndic1"
 endif 
 
 f1L        pvsfread  k(aPtr1)*giLen1, "PPhase1.pvx", 0
 f1R        pvsfread  k(aPtr1)*giLen1, "PPhase1.pvx", 1
 f1TransL   pvscale   f1L, semitone(gkSemi1) * cent(gkCent1)
 f1TransR   pvscale   f1R, semitone(gkSemi1) * cent(gkCent1)
 a1L        pvsynth   f1TransL
 a1R        pvsynth   f1TransR
 aL         =         a1L*gkAmp1*(1-gkPan1) * (1-gkMute1)
 aR         =         a1R*gkAmp1*gkPan1 * (1-gkMute1)
            outs      aL, aR
 gaSendL    +=        aL * gkRvbSend
 gaSendR    +=        aR * gkRvbSend
 
 ;; PIANO 2
 if gkAutoPhase2==1 then
  kPhs2      phasor   gkPhaseSpeed2
             chnset   kPhs2, "Phase2"
 else
  kPhs2      portk     gkPhase2, kPortTime
 endif
 kPBratio2  =         gkTempo2/gkBaseTempo
 aPtr2      osciliktp kPBratio2/giLen2, giRamp, kPhs2
 ;display phase
 if metro:k(32)==1 then
  Smsg   sprintfk  "pos(%d,%d)", k(aPtr2)*480 + 5, giIndicY + 105
         chnset Smsg, "PhaseIndic2"
 endif 
 f2L        pvsfread  k(aPtr2)*giLen2, "PPhase1.pvx", 0
 f2R        pvsfread  k(aPtr2)*giLen2, "PPhase2.pvx", 1
 f2TransL   pvscale   f2L, semitone(gkSemi2) * cent(gkCent2)
 f2TransR   pvscale   f2R, semitone(gkSemi2) * cent(gkCent2)
 a2L        pvsynth   f2TransL
 a2R        pvsynth   f2TransR
 aL         =         a2L*gkAmp2*(1-gkPan2) * (1-gkMute2)
 aR         =         a2R*gkAmp2*gkPan2 * (1-gkMute2)
            outs      aL, aR
 gaSendL    +=        aL * gkRvbSend
 gaSendR    +=        aR * gkRvbSend
 
endin

giFile1L  ftgen  1,0,0,1,"PianoPhase110_1.wav",0,0,1
giFile1R  ftgen  2,0,0,1,"PianoPhase110_1.wav",0,0,2
giFile2L  ftgen  3,0,0,1,"PianoPhase110_2.wav",0,0,1
giFile2R  ftgen  4,0,0,1,"PianoPhase110_2.wav",0,0,2


instr 5 ; time domain playback
kPortTime  linseg    0, 0.001, 0.1
kPBratio1  =         gkTempo1/gkBaseTempo
 if gkAutoPhase1==1 then
  kPhs1      phasor   gkPhaseSpeed1
             chnset   kPhs1, "Phase1"
 else
  kPhs1      = gkPhase1
 endif
aPtr   phasor    (semitone(gkSemi1) * cent(gkCent1) * kPBratio1)/giLen1
a1L    table3    aPtr+a(kPhs1), giFile1L, 1, 0, 1
a1R    table3    aPtr+a(kPhs1), giFile1R, 1, 0, 1
aL     =         a1L*gkAmp1*(1-gkPan1) * (1-gkMute1)
aR     =         a1R*gkAmp1*gkPan1 * (1-gkMute1)
       outs      aL, aR
 gaSendL    +=        aL * gkRvbSend
 gaSendR    +=        aR * gkRvbSend
 ;display phase
 if metro:k(32)==1 then
  kPtr   wrap      k(aPtr) + kPhs1, 0, 1
  Smsg   sprintfk  "pos(%d,%d)", kPtr*480 + 5, giIndicY
         chnset Smsg, "PhaseIndic1"
 endif 

kPBratio2  =         gkTempo2/gkBaseTempo
 if gkAutoPhase1==1 then
  kPhs2      phasor   gkPhaseSpeed2
             chnset   kPhs2, "Phase2"
 else
  kPhs2      = gkPhase2 ;portk     gkPhase2, kPortTime
 endif
aPtr   phasor    (semitone(gkSemi2) * cent(gkCent2) * kPBratio2)/giLen2
a2L    table3    aPtr+a(kPhs2), giFile2L, 1, 0, 1
a2R    table3    aPtr+a(kPhs2), giFile2R, 1, 0, 1
a2L    =         a2L*gkAmp2*(1-gkPan2) * (1-gkMute2)
a2R    =         a2R*gkAmp2*gkPan2 * (1-gkMute2)
       outs      a2L, a2R
 gaSendL    +=        a2L * gkRvbSend
 gaSendR    +=        a2R * gkRvbSend
 ;display phase
 if metro:k(32)==1 then
  kPtr   wrap      k(aPtr) + kPhs2, 0, 1
  Smsg   sprintfk  "pos(%d,%d)", kPtr*480 + 5, giIndicY + 105
         chnset Smsg, "PhaseIndic2"
 endif 
 
endin

instr 99
aL,aR  reverbsc  gaSendL,gaSendR,0.7,12000
       outs      aL,aR
gaSendL =        0
gaSendR =        0
endin

</CsInstruments>

<CsScore>
i 1  0 [3600*24*365]
i 99 0 [3600*24*365]
</CsScore>

</CsoundSynthesizer>