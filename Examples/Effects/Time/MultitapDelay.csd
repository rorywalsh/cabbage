
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; MultitapDelay.csd
; Written by Iain McCurdy, 2015

; Tone controls control the cutoff frequency of a lowpass filter on each tap

; Three modes are offered which implement feedback in different ways:

; Mode 1
; ------
; All 4 taps are taken from the same buffer **
; Only the 4th (and longest) tap is fed back into the input

; Mode 2
; ------
; All 4 delays are independent buffers
; All four feedback into themselves independently

; Mode 3
; ------
; All 4 delays are independent buffers
; Feedback into each delay is a mix of all 4 delays outputs.

<Cabbage>
#define NSliderStyle colour(55,55,55), textColour(20,20,20), fontColour(255,255,220)
#define RSliderStyle ,  colour( 174,170,165), trackerColour(234,240,230), textColour( 20, 20, 20)

form caption("Multitap Delay") size(580,420), pluginId("MtDl"), colour("Black"), guiRefresh(16)
image pos(0, 0),               size(580,420), colour( 214,210,205), shape("rounded"), outlineColour("white"), outlineThickness(4) 

label     bounds( 10, 40, 70, 14), fontColour(20,20,20),text("%")
nslider   bounds( 15, 10, 60, 30), channel("DelTim1"),  text("Time 1"),  range(0, 100, 12.5, 1, 0.001), $NSliderStyle
rslider   bounds( 10, 60, 70, 70), channel("Tone1"),    text("Tone 1"),  range(0, 20000, 12000, 0.5), $RSliderStyle
rslider   bounds( 10,140, 70, 70), channel("Pan1"),     text("Pan 1"),   range(0, 1, 0.2), $RSliderStyle
rslider   bounds( 10,220, 70, 70), channel("Level1"),   text("Level 1"), range(0, 1, 0.8, 0.5,0.001), $RSliderStyle
button    bounds( 20,295, 50, 20), text("MUTE","MUTE"), channel("Mute1"), value(0), latched(1), fontColour:0(50,50,50), fontColour:1(255,205,205), colour:0(10,0,0), colour:1(150,0,0)

label     bounds( 80, 40, 70, 14), fontColour(20,20,20),text("%")
nslider   bounds( 85, 10, 60, 30), channel("DelTim2"),  text("Time 2"), range(0, 100, 31.25, 1, 0.001), $NSliderStyle
rslider   bounds( 80, 60, 70, 70), channel("Tone2"),    text("Tone 2"), range(0, 20000, 5000, 0.5), $RSliderStyle
rslider   bounds( 80,140, 70, 70), channel("Pan2"),     text("Pan 2"), range(0, 1, 0.4), $RSliderStyle
rslider   bounds( 80,220, 70, 70), channel("Level2"),   text("Level 2"), range(0, 1, 0.25, 0.5,0.001), $RSliderStyle
button    bounds( 90,295, 50, 20), text("MUTE","MUTE"), channel("Mute2"), value(0), latched(1), fontColour:0(50,50,50), fontColour:1(255,205,205), colour:0(10,0,0), colour:1(150,0,0)

label     bounds(150, 40, 70, 14), fontColour(20,20,20),text("%")
nslider   bounds(155, 10, 60, 30), channel("DelTim3"),  text("Time 3"), range(0, 100, 37.5, 1, 0.001), $NSliderStyle
rslider   bounds(150, 60, 70, 70), channel("Tone3"),    text("Tone 3"), range(0, 20000, 16000, 0.5), $RSliderStyle
rslider   bounds(150,140, 70, 70), channel("Pan3"),     text("Pan 3"), range(0, 1, 0.8), $RSliderStyle
rslider   bounds(150,220, 70, 70), channel("Level3"),   text("Level 3"), range(0, 1, 0.5, 0.5,0.001), $RSliderStyle
button    bounds(160,295, 50, 20), text("MUTE","MUTE"), channel("Mute3"), value(0), latched(1), fontColour:0(50,50,50), fontColour:1(255,205,205), colour:0(10,0,0), colour:1(150,0,0)

label     bounds(220, 40, 70, 14), fontColour(20,20,20),text("%")
nslider   bounds(225, 10, 60, 30), channel("DelTim4"),  text("Time 4"), range(0, 100, 43.75, 1, 0.001), $NSliderStyle
rslider   bounds(220, 60, 70, 70), channel("Tone4"),    text("Tone 4"), range(0, 20000, 8000, 0.5), $RSliderStyle
rslider   bounds(220,140, 70, 70), channel("Pan4"),     text("Pan 4"), range(0, 1, 0.6), $RSliderStyle
rslider   bounds(220,220, 70, 70), channel("Level4"),   text("Level 4"), range(0, 1, 0.7, 0.5,0.001), $RSliderStyle
button    bounds(230,295, 50, 20), text("MUTE","MUTE"), channel("Mute4"), value(0), latched(1), fontColour:0(50,50,50), fontColour:1(255,205,205), colour:0(10,0,0), colour:1(150,0,0)

label     bounds(290, 40, 70, 14), fontColour(20,20,20),text("%")
nslider   bounds(295, 10, 60, 30), channel("DelTim5"),  text("Time 5"), range(0, 100,  62.5, 1, 0.001), $NSliderStyle
rslider   bounds(290, 60, 70, 70), channel("Tone5"),    text("Tone 5"), range(0, 20000, 8000, 0.5), $RSliderStyle
rslider   bounds(290,140, 70, 70), channel("Pan5"),     text("Pan 5"), range(0, 1, 0.6), $RSliderStyle
rslider   bounds(290,220, 70, 70), channel("Level5"),   text("Level 5"), range(0, 1, 0.7, 0.5,0.001), $RSliderStyle
button    bounds(300,295, 50, 20), text("MUTE","MUTE"), channel("Mute5"), value(0), latched(1), fontColour:0(50,50,50), fontColour:1(255,205,205), colour:0(10,0,0), colour:1(150,0,0)

label     bounds(360, 40, 70, 14), fontColour(20,20,20),text("%")
nslider   bounds(365, 10, 60, 30), channel("DelTim6"),  text("Time 6"), range(0, 100, 75, 1, 0.001), $NSliderStyle
rslider   bounds(360, 60, 70, 70), channel("Tone6"),    text("Tone 6"), range(0, 20000, 8000, 0.5), $RSliderStyle
rslider   bounds(360,140, 70, 70), channel("Pan6"),     text("Pan 6"), range(0, 1, 0.6), $RSliderStyle
rslider   bounds(360,220, 70, 70), channel("Level6"),   text("Level 6"), range(0, 1, 0.7, 0.5,0.001), $RSliderStyle
button    bounds(370,295, 50, 20), text("MUTE","MUTE"), channel("Mute6"), value(0), latched(1), fontColour:0(50,50,50), fontColour:1(255,205,205), colour:0(10,0,0), colour:1(150,0,0)

label     bounds(430, 40, 70, 14), fontColour(20,20,20),text("%")
nslider   bounds(435, 10, 60, 30), channel("DelTim7"),  text("Time 7"), range(0, 100, 78.125.5, 1, 0.001), $NSliderStyle
rslider   bounds(430, 60, 70, 70), channel("Tone7"),    text("Tone 7"), range(0, 20000, 8000, 0.5), $RSliderStyle
rslider   bounds(430,140, 70, 70), channel("Pan7"),     text("Pan 7"), range(0, 1, 0.6), $RSliderStyle
rslider   bounds(430,220, 70, 70), channel("Level7"),   text("Level 7"), range(0, 1, 0.7, 0.5,0.001), $RSliderStyle
button    bounds(440,295, 50, 20), text("MUTE","MUTE"), channel("Mute7"), value(0), latched(1), fontColour:0(50,50,50), fontColour:1(255,205,205), colour:0(10,0,0), colour:1(150,0,0)

label     bounds(500, 40, 70, 14), fontColour(20,20,20),text("secs")
nslider   bounds(505, 10, 60, 30), channel("DelTim8"),  text("Time 8"), range(0.001, 30, 2, 1, 0.001), $NSliderStyle
rslider   bounds(500, 60, 70, 70), channel("Tone8"),    text("Tone 8"), range(0, 20000, 8000, 0.5), $RSliderStyle
rslider   bounds(500,140, 70, 70), channel("Pan8"),     text("Pan 8"), range(0, 1, 0.6), $RSliderStyle
rslider   bounds(500,220, 70, 70), channel("Level8"),   text("Level 8"), range(0, 1, 0.7, 0.5,0.001), $RSliderStyle
button    bounds(510,295, 50, 20), text("MUTE","MUTE"), channel("Mute8"), value(0), latched(1), fontColour:0(50,50,50), fontColour:1(255,205,205), colour:0(10,0,0), colour:1(150,0,0)

line      bounds( 10,330,560,2)

label     bounds( 10,345, 60, 14), fontColour(20,20,20), text("Type")
combobox  bounds( 10,370, 60, 20),  channel("Mode"),text("Mode 1","Mode 2","Mode 3"), colour(  0, 40, 50), textColour("white"), value(1)
rslider   bounds( 80,340, 70, 70),  channel("Mix"),text("Mix"), range(0, 1, 0.5), $RSliderStyle
rslider   bounds(150,340, 70, 70),  channel("Feedback"),text("Feedback"), range(0, 1, 0.75), $RSliderStyle
rslider   bounds(220,340, 70, 70),  channel("Level"),text("Level"), range(0, 1, 1), $RSliderStyle

gentable  bounds(310,340, 250, 70), identChannel("table1"),  tableNumber(1), tableColour("green"), ampRange(0,1,1,0.001), zoom(-1), tableGridColour(0,0,0,0), active(1)

</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-dm0 -n
</CsOptions>

<CsInstruments>

; sr set by host
ksmps        =    16    ; NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls       =    2     ; NUMBER OF CHANNELS (2=STEREO)
0dbfs        =    1

;Author: Iain McCurdy (2015)

giTimes  ftgen  1, 0, 8, 2,  0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7

instr    1
 aInL,aInR    ins
; outs aInL,aInR
; aInL *= 0
; aInR *= 0

 iCnt = 1
 while iCnt<=8 do
 Snme sprintf "Pan%d",iCnt
 chnset random:i(0,1),Snme
 Snme sprintf "Level%d",iCnt
 chnset random:i(0,1),Snme
 iCnt += 1
 od

 kDelTim1    chnget    "DelTim1"
 kDelTim2    chnget    "DelTim2"
 kDelTim3    chnget    "DelTim3"
 kDelTim4    chnget    "DelTim4"
 kDelTim5    chnget    "DelTim5"
 kDelTim6    chnget    "DelTim6"
 kDelTim7    chnget    "DelTim7"
 kDelTim8    chnget    "DelTim8"
 
 aDelTim1    interp    kDelTim1*0.01
 aDelTim2    interp    kDelTim2*0.01
 aDelTim3    interp    kDelTim3*0.01
 aDelTim4    interp    kDelTim4*0.01
 aDelTim5    interp    kDelTim5*0.01
 aDelTim6    interp    kDelTim6*0.01
 aDelTim7    interp    kDelTim7*0.01
 aDelTim8    interp    kDelTim8

 kTone1        chnget    "Tone1"
 kTone2        chnget    "Tone2"
 kTone3        chnget    "Tone3"
 kTone4        chnget    "Tone4"
 kTone5        chnget    "Tone5"
 kTone6        chnget    "Tone6"
 kTone7        chnget    "Tone7"
 kTone8        chnget    "Tone8"

 kPan1        chnget    "Pan1"
 kPan2        chnget    "Pan2"
 kPan3        chnget    "Pan3"
 kPan4        chnget    "Pan4"
 kPan5        chnget    "Pan5"
 kPan6        chnget    "Pan6"
 kPan7        chnget    "Pan7"
 kPan8        chnget    "Pan8"

 kLevel1    chnget    "Level1"
 kLevel2    chnget    "Level2"
 kLevel3    chnget    "Level3"
 kLevel4    chnget    "Level4"
 kLevel5    chnget    "Level5"
 kLevel6    chnget    "Level6"
 kLevel7    chnget    "Level7"
 kLevel8    chnget    "Level8"

 kMute1    = 1 - chnget:k("Mute1")
 kMute2    = 1 - chnget:k("Mute2")
 kMute3    = 1 - chnget:k("Mute3")
 kMute4    = 1 - chnget:k("Mute4")
 kMute5    = 1 - chnget:k("Mute5")
 kMute6    = 1 - chnget:k("Mute6")
 kMute7    = 1 - chnget:k("Mute7")
 kMute8    = 1 - chnget:k("Mute8")

 kFeedback  chnget    "Feedback"
 kLevel     chnget    "Level"
 kMix       chnget    "Mix"
 kMode      chnget    "Mode"

 if changed:k(kDelTim1, kDelTim2, kDelTim3, kDelTim4, kDelTim5, kDelTim6, kDelTim7)==1 then
  reinit REFRESH_TABLE
 endif
 REFRESH_TABLE:
 giTimes  ftgen  1, 0, 8, 2,  i(kDelTim1)*0.01, i(kDelTim2)*0.01, i(kDelTim3)*0.01, i(kDelTim4)*0.01, i(kDelTim5)*0.01, i(kDelTim6)*0.01, i(kDelTim7)*0.01, 1
 chnset  "tableNumber(1)", "table1"
 
 if kMode==1 then
 
 ; left channel
 abufL     delayr   30
 aTapL1    deltapi  aDelTim1 * aDelTim8
 aTapL1    tone     aTapL1, kTone1
 aTapL2    deltapi  aDelTim2 * aDelTim8
 aTapL2    tone     aTapL2, kTone2
 aTapL3    deltapi  aDelTim3 * aDelTim8
 aTapL3    tone     aTapL3, kTone3
 aTapL4    deltapi  aDelTim4 * aDelTim8
 aTapL4    tone     aTapL4, kTone4
 aTapL5    deltapi  aDelTim5 * aDelTim8
 aTapL5    tone     aTapL5, kTone5
 aTapL6    deltapi  aDelTim6 * aDelTim8
 aTapL6    tone     aTapL6, kTone6
 aTapL7    deltapi  aDelTim7 * aDelTim8
 aTapL7    tone     aTapL7, kTone7
 aTapL8    deltapi  aDelTim8
 aTapL8    tone     aTapL8, kTone8
           delayw   aInL + (aTapL8*kFeedback)

 ; right channel
 abufR     delayr    30
 aTapR1    deltapi   aDelTim1 * aDelTim8
 aTapR1    tone      aTapR1, kTone1
 aTapR2    deltapi   aDelTim2 * aDelTim8
 aTapR2    tone      aTapR2, kTone2
 aTapR3    deltapi   aDelTim3 * aDelTim8
 aTapR3    tone      aTapR3, kTone3
 aTapR4    deltapi   aDelTim4 * aDelTim8
 aTapR4    tone      aTapR4, kTone4
 aTapR5    deltapi   aDelTim5 * aDelTim8
 aTapR5    tone      aTapR5, kTone5
 aTapR6    deltapi   aDelTim6 * aDelTim8
 aTapR6    tone      aTapR6, kTone6
 aTapR7    deltapi   aDelTim7 * aDelTim8
 aTapR7    tone      aTapR7, kTone7
 aTapR8    deltapi   aDelTim8
 aTapR8    tone      aTapR8, kTone8
           delayw    aInR + (aTapR8*kFeedback)

 elseif kMode==2 then
  
 ; left channel
  abuf   delayr    30
 aTapL1  deltapi   aDelTim1 * aDelTim8
 aTapL1  tone      aTapL1, kTone1
         delayw    aInL + (aTapL1*kFeedback)    
 abuf    delayr    30
 aTapL2  deltapi   aDelTim2 * aDelTim8
 aTapL2  tone      aTapL2, kTone2
         delayw    aInL + (aTapL2*kFeedback)    
 abuf    delayr    30
 aTapL3  deltapi   aDelTim3 * aDelTim8
 aTapL3  tone      aTapL3, kTone3
         delayw    aInL + (aTapL3*kFeedback)    
 abuf    delayr    30
 aTapL4  deltapi   aDelTim4 * aDelTim8
 aTapL4  tone      aTapL4, kTone4
         delayw    aInL + (aTapL4*kFeedback)    
 abuf    delayr    30
 aTapL5  deltapi   aDelTim5 * aDelTim8
 aTapL5  tone      aTapL5, kTone5
         delayw    aInL + (aTapL5*kFeedback)    
 abuf    delayr    30
 aTapL6  deltapi   aDelTim6 * aDelTim8
 aTapL6  tone      aTapL6, kTone6
         delayw    aInL + (aTapL6*kFeedback)    
 abuf    delayr    30
 aTapL7  deltapi   aDelTim7 * aDelTim8
 aTapL7  tone      aTapL7, kTone7
         delayw    aInL + (aTapL7*kFeedback)    
 abuf    delayr    30
 aTapL8  deltapi   aDelTim8
 aTapL8  tone      aTapL8, kTone8
         delayw    aInL + (aTapL8*kFeedback)    

 ; right channel
 abuf    delayr    30
 aTapR1  deltapi   aDelTim1 * aDelTim8
 aTapR1  tone      aTapR1, kTone1
         delayw    aInR + (aTapR1*kFeedback)    
 abuf    delayr    30
 aTapR2  deltapi   aDelTim2 * aDelTim8
 aTapR2  tone      aTapR2, kTone2
         delayw    aInR + (aTapR2*kFeedback)    
 abuf    delayr    30
 aTapR3  deltapi   aDelTim3 * aDelTim8
 aTapR3  tone      aTapR3, kTone3
         delayw    aInR + (aTapR3*kFeedback)    
 abuf    delayr    30
 aTapR4  deltapi   aDelTim4 * aDelTim8
 aTapR4  tone      aTapR4, kTone4
         delayw    aInR + (aTapR4*kFeedback)    
 abuf    delayr    30
 aTapR5  deltapi   aDelTim5 * aDelTim8
 aTapR5  tone      aTapR5, kTone5
         delayw    aInR + (aTapR5*kFeedback)    
 abuf    delayr    30
 aTapR6  deltapi   aDelTim6 * aDelTim8
 aTapR6  tone      aTapR6, kTone6
         delayw    aInR + (aTapR6*kFeedback)    
 abuf    delayr    30
 aTapR7  deltapi   aDelTim7 * aDelTim8
 aTapR7  tone      aTapR7, kTone7
         delayw    aInR + (aTapR7*kFeedback)    
 abuf    delayr    30
 aTapR8  deltapi   aDelTim8
 aTapR8  tone      aTapR8, kTone8
         delayw    aInR + (aTapR8*kFeedback)    


 elseif  kMode==3  then
  
 aFBmixL,aFBmixR    init    0 
 aTapL1    vdelay    aInL+aFBmixL, aDelTim1*aDelTim1*1000, 30000
 aTapL2    vdelay    aInL+aFBmixL, aDelTim2*aDelTim2*1000, 30000
 aTapL3    vdelay    aInL+aFBmixL, aDelTim3*aDelTim3*1000, 30000
 aTapL4    vdelay    aInL+aFBmixL, aDelTim4*aDelTim4*1000, 30000
 aTapL5    vdelay    aInL+aFBmixL, aDelTim5*aDelTim5*1000, 30000
 aTapL6    vdelay    aInL+aFBmixL, aDelTim6*aDelTim6*1000, 30000
 aTapL7    vdelay    aInL+aFBmixL, aDelTim7*aDelTim7*1000, 30000
 aTapL8    vdelay    aInL+aFBmixL, aDelTim8         *1000, 30000
 aTapR1    vdelay    aInR+aFBmixR, aDelTim1*aDelTim1*1000, 30000
 aTapR2    vdelay    aInR+aFBmixR, aDelTim2*aDelTim2*1000, 30000
 aTapR3    vdelay    aInR+aFBmixR, aDelTim3*aDelTim3*1000, 30000
 aTapR4    vdelay    aInR+aFBmixR, aDelTim4*aDelTim4*1000, 30000
 aTapR5    vdelay    aInR+aFBmixR, aDelTim5*aDelTim5*1000, 30000
 aTapR6    vdelay    aInR+aFBmixR, aDelTim6*aDelTim6*1000, 30000
 aTapR7    vdelay    aInR+aFBmixR, aDelTim7*aDelTim7*1000, 30000
 aTapR8    vdelay    aInR+aFBmixR, aDelTim8         *1000, 30000
 
 aTapL1    tone    aTapL1,kTone1
 aTapL2    tone    aTapL2,kTone2
 aTapL3    tone    aTapL3,kTone3
 aTapL4    tone    aTapL4,kTone4
 aTapL5    tone    aTapL5,kTone5
 aTapL6    tone    aTapL6,kTone6
 aTapL7    tone    aTapL7,kTone7
 aTapL8    tone    aTapL8,kTone8
 aTapR1    tone    aTapR1,kTone1
 aTapR2    tone    aTapR2,kTone2
 aTapR3    tone    aTapR3,kTone3
 aTapR4    tone    aTapR4,kTone4
 aTapR5    tone    aTapR5,kTone5
 aTapR6    tone    aTapR6,kTone6
 aTapR7    tone    aTapR7,kTone7
 aTapR8    tone    aTapR8,kTone8
  
 aFBmixL    =    (aTapL1 + aTapL2 + aTapL3 + aTapL4 + aTapL5 + aTapL6 + aTapL7 + aTapL8) * kFeedback / 8
 aFBmixR    =    (aTapR1 + aTapR2 + aTapR3 + aTapR4 + aTapR5 + aTapR6 + aTapR7 + aTapR8) * kFeedback / 8
 
endif

 aMixL    sum    aTapL1*kLevel1*kMute1*kPan1, aTapL2*kLevel2*kMute2*kPan2, aTapL3*kLevel3*kMute3*kPan3, aTapL4*kLevel4*kMute4*kPan4, aTapL5*kLevel5*kMute5*kPan5, aTapL6*kLevel6*kMute6*kPan6, aTapL7*kLevel7*kMute7*kPan7, aTapL8*kLevel8*kMute8*kPan8
 aMixR    sum    aTapR1*kLevel1*kMute1*(1-kPan1), aTapR2*kLevel2*kMute2*(1-kPan2), aTapR3*kLevel3*kMute3*(1-kPan3), aTapR4*kLevel4*kMute4*(1-kPan4), aTapR5*kLevel5*kMute5*(1-kPan5), aTapR6*kLevel6*kMute6*(1-kPan6), aTapR7*kLevel7*kMute7*(1-kPan7), aTapR8*kLevel8*kMute8*(1-kPan8)

    outs    ((aMixL*kMix) + (aInL*(1-kMix)))*kLevel, ((aMixR*kMix) + (aInR*(1-kMix)))*kLevel
endin
        
</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
</CsScore>

</CsoundSynthesizer>