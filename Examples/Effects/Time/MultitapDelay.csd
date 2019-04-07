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
form caption("Multitap Delay") size(300,385), pluginid("MtDl") style("legacy")
image pos(0, 0),               size(300,385), colour( 170, 90,10), shape("rounded"), outlinecolour("white"), outlinethickness(4) 

label     bounds( 10, 45, 70, 14), fontcolour("white"), text("%")
nslider bounds( 10, 10, 70, 35), channel("DelTim1"),   text("Time 1"), colour(  0, 40, 50), textcolour("white"), range(0, 100, 12.5, 1, 0.001)
rslider   bounds( 10, 65, 70, 70), channel("Tone1"), text("Tone 1"),   colour(  0, 40, 50), trackercolour(200,240,250), textcolour("white"), range(0, 20000, 12000, 0.5)
rslider   bounds( 10,145, 70, 70), channel("Pan1"), text("Pan 1"),     colour(  0, 40, 50), trackercolour(200,240,250), textcolour("white"), range(0, 1, 0.2)
rslider   bounds( 10,215, 70, 70), channel("Level1"), text("Level 1"), colour(  0, 40, 50), trackercolour(200,240,250), textcolour("white"), range(0, 1, 0.8, 0.5,0.001)

label     bounds( 80, 45, 70, 14), fontcolour("white"), text("%")
nslider bounds( 80, 10, 70, 35), channel("DelTim2"),  text("Time 2"), colour(  0, 40, 50), textcolour("white"), range(0, 100, 50, 1, 0.001)
rslider   bounds( 80, 65, 70, 70),   channel("Tone2"),  text("Tone 2"), colour(  0, 40, 50), trackercolour(200,240,250), textcolour("white"), range(0, 20000, 5000, 0.5)
rslider   bounds( 80,145, 70, 70),    channel("Pan2"),  text("Pan 2"),  colour(  0, 40, 50), trackercolour(200,240,250), textcolour("white"), range(0, 1, 0.4)
rslider   bounds( 80,215, 70, 70),  channel("Level2"),  text("Level 2"),colour(  0, 40, 50), trackercolour(200,240,250), textcolour("white"), range(0, 1, 0.25, 0.5,0.001)

label     bounds(150, 45, 70, 14), fontcolour("white"), text("%")
nslider bounds(150, 10, 70, 35), channel("DelTim3"),  text("Time 3"), colour(  0, 40, 50), textcolour("white"), range(0, 100, 87.5, 1, 0.001)
rslider   bounds(150, 65, 70, 70),   channel("Tone3"),  text("Tone 3"), colour(  0, 40, 50), trackercolour(200,240,250), textcolour("white"), range(0, 20000, 16000, 0.5)
rslider   bounds(150,145, 70, 70),    channel("Pan3"),  text("Pan 3"),  colour(  0, 40, 50), trackercolour(200,240,250), textcolour("white"), range(0, 1, 0.8)
rslider   bounds(150,215, 70, 70),  channel("Level3"),  text("Level 3"),colour(  0, 40, 50), trackercolour(200,240,250), textcolour("white"), range(0, 1, 0.5, 0.5,0.001)

label     bounds(220, 45, 70, 14), fontcolour("white"), text("secs")
nslider bounds(220, 10, 70, 35), channel("DelTim4"),  text("Time 4"), colour(  0, 40, 50), textcolour("white"), range(0.001, 30, 1.5, 1, 0.001)
rslider   bounds(220, 65, 70, 70),   channel("Tone4"),  text("Tone 4"), colour(  0, 40, 50), trackercolour(200,240,250), textcolour("white"), range(0, 20000, 8000, 0.5)
rslider   bounds(220,145, 70, 70),    channel("Pan4"),  text("Pan 4"),  colour(  0, 40, 50), trackercolour(200,240,250), textcolour("white"), range(0, 1, 0.6)
rslider   bounds(220,215, 70, 70),  channel("Level4"),  text("Level 4"),colour(  0, 40, 50), trackercolour(200,240,250), textcolour("white"), range(0, 1, 0.7, 0.5,0.001)

line      bounds( 10,295,280,2)
label     bounds( 10,310, 70, 14), fontcolour("white"), text("Type")
combobox  bounds( 10,325, 70, 20),  channel("Mode"),text("Mode 1","Mode 2","Mode 3"), colour(  0, 40, 50), textcolour("white")
rslider   bounds( 80,305, 70, 70),  channel("Mix"),text("Mix"), colour(  0, 40, 50), trackercolour(200,240,250), textcolour("white"), range(0, 1, 0.5)
rslider   bounds(150,305, 70, 70),  channel("Feedback"),text("Feedback"), colour(  0, 40, 50), trackercolour(200,240,250), textcolour("white"), range(0, 1, 0.75)
rslider   bounds(220,305, 70, 70),  channel("Level"),text("Level"), colour(  0, 40, 50), trackercolour(200,240,250), textcolour("white"), range(0, 1, 1)

</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-d -n
</CsOptions>

<CsInstruments>

sr 		= 	44100	;SAMPLE RATE
ksmps 		= 	32	;NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls 		= 	2	;NUMBER OF CHANNELS (2=STEREO)
0dbfs		=	1

;Author: Iain McCurdy (2015)

instr	1
 kDelTim1	chnget	"DelTim1"
 aDelTim1	interp	kDelTim1*0.01
 kTone1		chnget	"Tone1"
 kPan1		chnget	"Pan1"
 kLevel1	chnget	"Level1"

 kDelTim2	chnget	"DelTim2"
 aDelTim2	interp	kDelTim2*0.01
   kTone2	chnget	  "Tone2"
 kPan2		chnget	"Pan2"
  kLevel2	chnget	 "Level2"

 kDelTim3	chnget	"DelTim3"
 aDelTim3	interp	kDelTim3*0.01
   kTone3	chnget	  "Tone3"
 kPan3		chnget	"Pan3"
  kLevel3	chnget	 "Level3"

 kDelTim4	chnget	"DelTim4"
 aDelTim4	interp	kDelTim4
   kTone4	chnget	  "Tone4"
 kPan4		chnget	"Pan4"
  kLevel4	chnget	 "Level4"

  kFeedback	chnget	 "Feedback"
  kLevel	chnget	 "Level"
  kMix		chnget	 "Mix"
  kMode		chnget	"Mode"

 aInL,aInR	ins


 if kMode==1 then
 
 /* left channel */
 abufL	delayr	30
 aTapL1	deltapi	aDelTim1 * aDelTim4
 aTapL1	tone	aTapL1, kTone1
 aTapL2	deltapi	aDelTim2 * aDelTim4
 aTapL2	tone	aTapL2, kTone2
 aTapL3	deltapi	aDelTim3 * aDelTim4
 aTapL3	tone	aTapL3, kTone3
 aTapL4	deltapi	aDelTim4
 aTapL4	tone	aTapL4, kTone4
	delayw	aInL + (aTapL4*kFeedback)

 /* right channel */
 abufR	delayr	30
 aTapR1	deltapi	aDelTim1 * aDelTim4
 aTapR1	tone	aTapR1, kTone1
 aTapR2	deltapi	aDelTim2 * aDelTim4
 aTapR2	tone	aTapR2, kTone2
 aTapR3	deltapi	aDelTim3 * aDelTim4
 aTapR3	tone	aTapR3, kTone3
 aTapR4	deltapi	aDelTim4
 aTapR4	tone	aTapR4, kTone4
	delayw	aInR + (aTapR4*kFeedback)


 elseif kMode==2 then
  
 /* left channel */
  abuf	delayr	30
 aTapL1	deltapi	aDelTim1 * aDelTim4
 aTapL1	tone	aTapL1, kTone1
 	delayw	aInL + (aTapL1*kFeedback)	
 abuf	delayr	30
 aTapL2	deltapi	aDelTim2 * aDelTim4
 aTapL2	tone	aTapL2, kTone2
 	delayw	aInL + (aTapL2*kFeedback)	
 abuf	delayr	30
 aTapL3	deltapi	aDelTim3 * aDelTim4
 aTapL3	tone	aTapL3, kTone3
 	delayw	aInL + (aTapL3*kFeedback)	
 abuf	delayr	30
 aTapL4	deltapi	aDelTim4
 aTapL4	tone	aTapL4, kTone4
 	delayw	aInL + (aTapL4*kFeedback)	

 /* right channel */
 abuf	delayr	30
 aTapR1	deltapi	aDelTim1 * aDelTim4
 aTapR1	tone	aTapR1, kTone1
 	delayw	aInR + (aTapR1*kFeedback)	
 abuf	delayr	30
 aTapR2	deltapi	aDelTim2 * aDelTim4
 aTapR2	tone	aTapR2, kTone2
 	delayw	aInR + (aTapR2*kFeedback)	
 abuf	delayr	30
 aTapR3	deltapi	aDelTim3 * aDelTim4
 aTapR3	tone	aTapR3, kTone3
 	delayw	aInR + (aTapR3*kFeedback)	
 abuf	delayr	30
 aTapR4	deltapi	aDelTim4
 aTapR4	tone	aTapR4, kTone4
 	delayw	aInR + (aTapR4*kFeedback)	


 elseif kMode==3 then
  
 aFBmixL,aFBmixR	init	0 
 aTapL1	vdelay	aInL+aFBmixL, aDelTim1*aDelTim4*1000, 30000
 aTapL2	vdelay	aInL+aFBmixL, aDelTim2*aDelTim4*1000, 30000
 aTapL3	vdelay	aInL+aFBmixL, aDelTim3*aDelTim4*1000, 30000
 aTapL4	vdelay	aInL+aFBmixL, aDelTim4         *1000, 30000
 aTapR1	vdelay	aInR+aFBmixR, aDelTim1*aDelTim4*1000, 30000
 aTapR2	vdelay	aInR+aFBmixR, aDelTim2*aDelTim4*1000, 30000
 aTapR3	vdelay	aInR+aFBmixR, aDelTim3*aDelTim4*1000, 30000
 aTapR4	vdelay	aInR+aFBmixR, aDelTim4         *1000, 30000
 
 aTapL1	tone	aTapL1,kTone1
 aTapL2	tone	aTapL2,kTone2
 aTapL3	tone	aTapL3,kTone3
 aTapL4	tone	aTapL4,kTone4
 aTapR1	tone	aTapR1,kTone1
 aTapR2	tone	aTapR2,kTone2
 aTapR3	tone	aTapR3,kTone3
 aTapR4	tone	aTapR4,kTone4
  
 aFBmixL	=	(aTapL1 + aTapL2 + aTapL3 + aTapL4) * kFeedback / 4
 aFBmixR	=	(aTapR1 + aTapR2 + aTapR3 + aTapR4) * kFeedback / 4
 
endif

 aMixL	sum	aTapL1*kLevel1*kPan1, aTapL2*kLevel2*kPan2, aTapL3*kLevel3*kPan3, aTapL4*kLevel4*kPan4 
 aMixR	sum	aTapR1*kLevel1*(1-kPan1), aTapR2*kLevel2*(1-kPan2), aTapR3*kLevel3*(1-kPan3), aTapR4*kLevel4*(1-kPan4)




	outs	((aMixL*kMix) + (aInL*(1-kMix)))*kLevel, ((aMixR*kMix) + (aInR*(1-kMix)))*kLevel
endin
		
</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
</CsScore>

</CsoundSynthesizer>
