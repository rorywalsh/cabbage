; BounceDelay.csd
; Written by Iain McCurdy, 2015

; Creates layers of delays, their delay times mathematically related so as to create the impression of sounds bouncing and coming to rest (and other sound effects).
; The gesture can be further shaped by applying progessive amounts of amplitude reduction, lowpass filtering and highpass filtering.

; Total time	-	total time of the 'bounce' gesture / the length of the longest delay.
; Number	-	number of delays
; Warp		-	amount of progressive time warping of the delays:
;			 If 'Warp' = 0 then delays are uniformly spaced
;			 If 'Warp' < 0 then delays are progressively more narrowly spaced (the typical 'bounce' structure)
;			 If 'Warp' > 0 then delays are progressively more widely spaced (a reverse 'bounce' structure)
; Feedback	-	The amount output -> input feedback in each individual delay
; Mix		-	Dry/wet mix.
; Level		-	Output level
; Amp.Curve	-	Delay by delay amplitude shaping	
;			 If 'Amp.Curve' = 0, no shaping is applied.
;			 If 'Amp.Curve' < 0, amplitudes are progressively attenuated. The more negative this value is, the more rapidly amplitudes are reduced / the steeper the slope of the curve.
;			 If 'Amp.Curve' > 0, amplitudes are progressively increased (starting at silence). The larger this value is, the more rapidly amplitudes are increased / the steeper the slope of the curve.

; Lowpass Filter
; --------------
; Curve		-	Delay by delay lowpass filtering. Curve shaping operates in a similar fashion to 'Amp.Curve'
; Slope		-	Steepness of the cutoff slope of the filter.

; Highpass Filter
; --------------
; Curve		-	Delay by delay highpass filtering. Curve shaping operates in a similar fashion to 'Amp.Curve'
; Slope		-	Steepness of the cutoff slope of the filter.

; Note that the dry signal is included in amplitude and filtering shaping so depending on the three curve settings it may be filtered or even attenuated completely.
	
<Cabbage>
form caption("Bounce Delay"), size(490,385), pluginID("BnDl")
rslider   bounds( 10, 10, 70, 70), channel("TotalTime"), text("Total Time"), colour(  0, 40, 50), trackercolour(200,240,250), textcolour("white"), range(0.01, 4, 1.7, 0.5)
rslider   bounds( 90, 10, 70, 70), channel("Number"), text("Number"),        colour(  0, 40, 50), trackercolour(200,240,250), textcolour("white"), range(1, 32,20, 1,1)
rslider   bounds(170, 10, 70, 70), channel("Warp"), text("Warp"),            colour(  0, 40, 50), trackercolour(200,240,250), textcolour("white"), range(-3, 3, -0.5)
rslider   bounds(250, 10, 70, 70), channel("Feedback"), text("Feedback"),    colour(  0, 40, 50), trackercolour(200,240,250), textcolour("white"), range(0, 1, 0)
rslider   bounds(330, 10, 70, 70), channel("Mix"), text("Mix"),              colour(  0, 40, 50), trackercolour(200,240,250), textcolour("white"), range(0, 1, 0.5)
rslider   bounds(410, 10, 70, 70), channel("Level"), text("Level"),          colour(  0, 40, 50), trackercolour(200,240,250), textcolour("white"), range(0, 1, 1)

rslider   bounds( 10,125, 70, 70), channel("AmpCurve"), text("Amp.Curve"),   colour(  0, 40, 50), trackercolour(200,240,250), textcolour("white"), range(-1, 1, -0.7)

image     bounds( 90,100,180,100), colour(0,0,0,0), outlinecolour("white"), outlinethickness(1), plant("LPF"){
label     bounds(  0,  3,180, 14), fontcolour("white"), text("L o w p a s s   F i l t e r")
rslider   bounds( 10, 25, 70, 70), channel("LPFCurve"), text("Curve"),   colour(  0, 40, 50), trackercolour(200,240,250), textcolour("white"), range(-1, 1, -0.8)
label     bounds( 80, 30, 80, 14), fontcolour("white"), text("Slope")
combobox  bounds( 80, 45, 80, 20),  channel("LPFType"),text("Bypass","6dB/oct","12dB/oct"), colour(  0, 40, 50), textcolour("white"), value(2)
}

image     bounds(285,100,180,100), colour(0,0,0,0), outlinecolour("white"), outlinethickness(1), plant("HPF"){
label     bounds(  0,  3,180, 14), fontcolour("white"), text("H i g h p a s s   F i l t e r")
rslider   bounds( 10, 25, 70, 70), channel("HPFCurve"), text("Curve"),   colour(  0, 40, 50), trackercolour(200,240,250), textcolour("white"), range(-1, 1, 0)
label     bounds( 80, 30, 80, 14), fontcolour("white"), text("Slope")
combobox  bounds( 80, 45, 80, 20),  channel("HPFType"),text("Bypass","6dB/oct","12dB/oct"), colour(  0, 40, 50), textcolour("white"), value(1)
}

label     bounds( 10,370,110, 10), fontcolour(150,150,150), text("Iain McCurdy |2015|")

; display bars 
image bounds( 18,218,454,124), colour(250,250,250), shape("sharp")
image bounds( 20,220,450,120), colour( 20, 20, 20), shape("sharp"), plant("display") {
image bounds(  0,  0,  4,100), colour(250,250,250), shape(sharp), visible(1), widgetarray("Bars",32)
}
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-dm0 -n
</CsOptions>

<CsInstruments>

sr 		= 	44100	;SAMPLE RATE
ksmps 		= 	32	;NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls 		= 	2	;NUMBER OF CHANNELS (2=STEREO)
0dbfs		=	1

giCurveRange	=	4	; extent of the control over shaping curves. This governs all curves: amplitude, LPF and HPF but it might be preferrable to have independent global variables for independent control.
giMaxDelay	=	4	; maximum delay time (this should correspond with the maximum value used for the 'Total Time' widget) 

; UDO that defines a single delay layer
; recursion is used to generate multiple delays. Layer counting is used to differentiate between layers.
opcode	EchoDelays,aa,aakkkkkkkkii
 aInL,aInR,kTotTim,kWarp,kFeedback,kAmpCurve,kLPFCurve,kLPFType,kHPFCurve,kHPFType,iNumber,iCount	xin	; read in input arguments
 ;	setksmps	1
 kRatio	pow	(iCount-1)/(iNumber-1),kWarp		; define delay time ratio according to layer number and 'Warp' amount. 
 kTim	=	kTotTim * kRatio		; scale ratio by total delay time
 aTim	interp	kTim				; create interpolated a-rate version. Creates higher quality results than just using a k-rate variable in the delay.  
 abuf	delayr	giMaxDelay			; establish delay buffer (establish memory/RAM needed)
 atapL	deltapi	aTim				; read from within delay buffer
 	delayw	aInL + (atapL*kFeedback)	; write into delay buffer
 abuf	delayr	giMaxDelay			; do the same for the right channel
 atapR	deltapi	aTim
 	delayw	aInR + (atapR*kFeedback)

 /* define amplitude */
 if kAmpCurve<=0 then											; negative or zero curve values
  kAmpVal	pow	1-(iCount/iNumber), octave((abs(kAmpCurve)*giCurveRange)-giCurveRange/2)	; calculate amplitude scaling value (will be a value between zero and 1). Each layer will have its own value. 
 else												 	; positive curve values
  kAmpVal	pow	(iCount/iNumber), octave((abs(kAmpCurve)*giCurveRange)-giCurveRange/2)
 endif

 /* define LPF */
 if kLPFCurve<-0.01||kLPFCurve>0.01 then								; only apply filtering if curve value is moved away from zero (!=0 didn't seem to be working)
  if kLPFCurve<=0 then											; when curve widget is negative or zero
   kLPFVal	pow	1-(iCount/iNumber), octave((abs(kLPFCurve)*giCurveRange)-giCurveRange/2)	; calculate lowpass filtering ratio (a value between 0 and 1)
  else													; positive values
   kLPFVal	pow	iCount/iNumber, octave((kLPFCurve*giCurveRange)-giCurveRange/2)
  endif
  if kLPFType==2 then											; choose between 6db and 12dB/oct filter slopes i.e. tone or butlp opcodes 
   atapL		tone	atapL,cpsoct((kLPFVal*10)+4)
   atapR		tone	atapR,cpsoct((kLPFVal*10)+4)
  elseif kLPFType==3 then
   atapL		butlp	atapL,cpsoct((kLPFVal*10)+4)
   atapR		butlp	atapR,cpsoct((kLPFVal*10)+4)
  endif  
 endif

 /* define HPF */
 if kHPFCurve<-0.01||kHPFCurve>0.01 then
  if kHPFCurve<=0 then
   kHPFVal	pow	1-(iCount/iNumber), octave((abs(kHPFCurve)*giCurveRange)-giCurveRange/2)
  else
   kHPFVal	pow	iCount/iNumber, octave((kHPFCurve*giCurveRange)-giCurveRange/2)
  endif
  if kHPFType==2 then
   atapL		atone	atapL,cpsoct(((1-kHPFVal)*10)+4)
   atapR		atone	atapR,cpsoct(((1-kHPFVal)*10)+4)
  elseif kHPFType==3 then
   atapL		buthp	atapL,cpsoct(((1-kHPFVal)*10)+4)
   atapR		buthp	atapR,cpsoct(((1-kHPFVal)*10)+4)
  endif  
 endif

 /* call the next echo (if required) */ 
 if iCount<iNumber then
  aMixL,aMixR	EchoDelays	aInL,aInR,kTotTim,kWarp,kFeedback,kAmpCurve,kLPFCurve,kLPFType,kHPFCurve,kHPFType,iNumber,iCount+1
 endif
 
 /* send back to instrument */
 	xout	(atapL*kAmpVal)+aMixL,(atapR*kAmpVal)+aMixR
 aMixL	=	0
 aMixR	=	0
endop


instr	1
 kporttime	linseg	0,0.001,0.05
 kTotTim	chnget	"TotalTime"
 kTotTim	portk	kTotTim,kporttime
 kNumber	chnget	"Number"
 kNumber	init	10
 kWarp		chnget	"Warp"   
 kWarp		portk	octave(kWarp),kporttime
 kWarp		init	1
 kFeedback	chnget	"Feedback"   
 kAmpCurve	chnget	"AmpCurve"
 kAmpCurve	init	0
 kLPFCurve	chnget	"LPFCurve"
 kLPFType	chnget	"LPFType"
 kHPFCurve	chnget	"HPFCurve"
 kHPFType	chnget	"HPFType" 
 kMix		chnget	"Mix"
 kLevel		chnget	"Level"
  
 aInL,aInR	ins

 /* reinitialise if 'Number' (of layers) changes */
 if changed(kNumber)==1 then
  reinit UPDATE
 endif
 UPDATE:
 
 iNumber	=	i(kNumber)
 if iNumber>1 then			; only call UDO if more than one layer is asked for (the dry/undelayed signal is the first layer)
  aDelL,aDelR	EchoDelays	aInL,aInR,kTotTim,kWarp,kFeedback,kAmpCurve,kLPFCurve,kLPFType,kHPFCurve,kHPFType,iNumber,1
 else
  aDelL	=	0
  aDelR	=	0
 endif
 
 /* define amplitude */
 if kAmpCurve>0 then	; attenuate dry signal completely if 'Amp.Curve' is positive	
  aInL	=	0
  aInR	=	0
 endif
 
 /* define LPF */
 if kLPFCurve>0 then			; if (Lowpass) curve is positive we need to apply maximum lowpass filtering to the first sound iteration (the dry signal)
  if kLPFType==2 then			; choose between filter types
   aInL		tone	aInL,cpsoct(4)
   aInR		tone	aInR,cpsoct(4)
  elseif kLPFType==3 then
   aInL		butlp	aInL,cpsoct(4)
   aInR		butlp	aInR,cpsoct(4)
  endif  
 endif

 /* define HPF */			; if (Highpass) curve is positive we need to apply maximum highpass filtering to the first sound iteration (the dry signal)
 if kHPFCurve>0 then
  if kHPFType==2 then
   aInL		atone	aInL,cpsoct(14)
   aInR		atone	aInR,cpsoct(14)
  elseif kHPFType==3 then
   aInL		buthp	aInL,cpsoct(14)
   aInR		buthp	aInR,cpsoct(14)
  endif  
 endif
 
 aMixL	ntrpol		aInL, aDelL, kMix	; dry/wet mixing
 aMixR	ntrpol		aInR, aDelR, kMix

	outs	aMixL*kLevel, aMixR*kLevel




 /* move display bars */
 if changed(kNumber)==1 then		; hide all
  reinit HideAndShow
 endif
 HideAndShow:
 icount	=	1
 loop0:
 Sident	sprintf	"Bars_ident%i",icount
 	chnset	"visible(0)",Sident
 	loop_le	icount,1,32,loop0
 icount	=	1
 loop1:
 Sident	sprintf	"Bars_ident%i",icount
 	chnset	"visible(1)",Sident
 	loop_lt	icount,1,i(kNumber),loop1
 rireturn
 
 if metro(5)==1 then
  if changed(kNumber,kTotTim,kWarp,kAmpCurve)==1 then
   reinit MoveBars
  endif
 endif
 MoveBars:
 icount	=	1
 inum	=	i(kNumber)
 iwid	=	450 * (i(kTotTim)/giMaxDelay)
 ipos	=	0
 loop:
 Sident	sprintf	"Bars_ident%i",icount 
 iRatio	pow	(icount-1)/(inum-1),i(kWarp)
 ipos	=	iwid * iRatio

 iAmpCurve	=	i(kAmpCurve)
 if iAmpCurve>-0.01&&iAmpCurve<0.01 then
  iAmpVal	=	1 
 elseif iAmpCurve<=0 then											; negative or zero curve values
  iAmpVal	pow	1-(icount/inum), octave((abs(iAmpCurve)*giCurveRange)-giCurveRange/2)		; calculate amplitude scaling value (will be a value between zero and 1). Each layer will have its own value. 
 else									 	; positive curve values
  iAmpVal	pow	(icount/inum), octave((abs(iAmpCurve)*giCurveRange)-giCurveRange/2)
 endif
 
 Smess	sprintf	"bounds(%i, %i,  1,%i)",ipos,20+(100*(1-iAmpVal)),100*iAmpVal
 	chnset	Smess,Sident
 	loop_le	icount,1,inum,loop
endin
		
</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
</CsScore>

</CsoundSynthesizer>