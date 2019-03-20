<Cabbage>
form size(405, 90), caption("clip"), pluginid("Clip") style("legacy")
image        bounds(0, 0, 405, 90), colour( 20, 30, 40), shape("rounded"), outlinecolour("white"), outlinethickness(4) 
rslider  bounds( 10, 11, 70, 70), text("Input Gain"), channel("InGain"), range(1, 10, 1, 0.5,0.01), colour(0,10,20), fontcolour("silver"), trackercolour("lightblue")
rslider  bounds( 75, 11, 70, 70), text("Limit"), channel("limit"), range(0.001, 1, 1, 0.5,0.001), colour(0,10,20), fontcolour("silver"), trackercolour("lightblue")
rslider  bounds(140, 11, 70, 70), text("Argument"), channel("arg"), range(0, 1.00, 0.5), colour(0,10,20), fontcolour("silver"), trackercolour("lightblue")
checkbox bounds(128,  7,  8,  8), channel("clip_light"), text("clip"), colour("red"), fontcolour("white"), shape("ellipse"), value(0), active(0)
label    bounds(137,  6, 25, 10), text("CLIP")
combobox bounds(210, 15, 80, 20), channel("method"), value(1), text("B.D.J.","Sine","Tanh")
checkbox bounds(210, 50, 15, 15), channel("GainComp"), text("Gain Compensation"), colour("lime"), fontcolour("white")
label    bounds(225, 52,100, 10), text("GAIN COMPENSATION")
rslider  bounds(325, 11, 70, 70), text("Level"), channel("level"), range(0.00, 10.00, 0.7), colour(0,10,20), fontcolour("silver"), trackercolour("lightblue")
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-d -n
</CsOptions>

<CsInstruments>

sr 		= 	44100	; SAMPLE RATE
ksmps 	= 	32		; NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls 	= 	2		; NUMBER OF CHANNELS (2=STEREO)
0dbfs	=	1

; Author: Iain McCurdy (2012)

instr	1
	kInGain	chnget	"InGain"
	kGainComp	chnget	"GainComp"
	klimit		chnget	"limit"
	kmethod	chnget	"method"
	kmethod	init	1
	karg		chnget	"arg"
	klevel		chnget	"level"
	asigL	=	inch:a(1) * kInGain
	asigR	=	inch:a(2) * kInGain

	; CLIP METER
	krmsL		=	rms:k(asigL,10) * 3
	krmsR		=	rms:k(asigR,10) * 3
	if ( trigger:k(krmsL,klimit,0)==1 || trigger:k(krmsR,klimit,0)==1 ) then
		chnset	k(1),"clip_light"
	elseif ( trigger:k(krmsL,klimit,1)==1 ||trigger:k(krmsR,klimit,1)==1 ) then
		chnset	k(0),"clip_light"
	endif

	if	changed(klimit,kmethod,karg)==1	then	; IF A VARIABLE CHANGE INDICATOR IS RECEIVED...
		reinit	UPDATE		; ...BEGIN A REINITIALISATION PASS FROM LABEL 'START' 
	endif				; END OF CONDITIONAL BRANCHING
	UPDATE:				; LABEL
	asigL		clip 		asigL, i(kmethod)-1, i(klimit), i(karg)
	asigR		clip 		asigR, i(kmethod)-1, i(klimit), i(karg)
	rireturn			; RETURN TO PERFORMANCE PASSES FROM INITIALISATION PASS
	if (kGainComp==1) then
	 asigL		=			asigL * (1/sqrt(klimit))
	 asigR		=			asigR * (1/sqrt(klimit))
	endif
			outs		asigL*klevel, asigR*klevel
endin

</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
</CsScore>

</CsoundSynthesizer><Cabbage>
form size(405, 90), caption("clip"), pluginid("Clip")  style("legacy") style("legacy")
image        bounds(0, 0, 405, 90), colour( 20, 30, 40), shape("rounded"), outlinecolour("white"), outlinethickness(4) 
rslider  bounds( 10, 11, 70, 70), text("Input Gain"), channel("InGain"), range(1, 10, 1, 0.5,0.01), colour(0,10,20), fontcolour("silver"), trackercolour("lightblue")
rslider  bounds( 75, 11, 70, 70), text("Limit"), channel("limit"), range(0.001, 1, 1, 0.5,0.001), colour(0,10,20), fontcolour("silver"), trackercolour("lightblue")
rslider  bounds(140, 11, 70, 70), text("Argument"), channel("arg"), range(0, 1.00, 0.5), colour(0,10,20), fontcolour("silver"), trackercolour("lightblue")
checkbox bounds(128,  7,  8,  8), channel("clip_light"), text("clip"), colour("red"), fontcolour("white"), shape("ellipse"), value(0), active(0)
label    bounds(137,  6, 25, 10), text("CLIP")
combobox bounds(210, 15, 80, 20), channel("method"), value(1), text("B.D.J.","Sine","Tanh")
checkbox bounds(210, 50, 15, 15), channel("GainComp"), text("Gain Compensation"), colour("lime"), fontcolour("white")
label    bounds(225, 52,100, 10), text("GAIN COMPENSATION")
rslider  bounds(325, 11, 70, 70), text("Level"), channel("level"), range(0.00, 10.00, 0.7), colour(0,10,20), fontcolour("silver"), trackercolour("lightblue")
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-d -n
</CsOptions>

<CsInstruments>

sr 		= 	44100	; SAMPLE RATE
ksmps 	= 	32		; NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls 	= 	2		; NUMBER OF CHANNELS (2=STEREO)
0dbfs	=	1

; Author: Iain McCurdy (2012)

instr	1
	kInGain	chnget	"InGain"
	kGainComp	chnget	"GainComp"
	klimit		chnget	"limit"
	kmethod	chnget	"method"
	kmethod	init	1
	karg		chnget	"arg"
	klevel		chnget	"level"
	asigL	=	inch:a(1) * kInGain
	asigR	=	inch:a(2) * kInGain

	; CLIP METER
	krmsL		=	rms:k(asigL,10) * 3
	krmsR		=	rms:k(asigR,10) * 3
	if ( trigger:k(krmsL,klimit,0)==1 || trigger:k(krmsR,klimit,0)==1 ) then
		chnset	k(1),"clip_light"
	elseif ( trigger:k(krmsL,klimit,1)==1 ||trigger:k(krmsR,klimit,1)==1 ) then
		chnset	k(0),"clip_light"
	endif

	if	changed(klimit,kmethod,karg)==1	then	; IF A VARIABLE CHANGE INDICATOR IS RECEIVED...
		reinit	UPDATE		; ...BEGIN A REINITIALISATION PASS FROM LABEL 'START' 
	endif				; END OF CONDITIONAL BRANCHING
	UPDATE:				; LABEL
	asigL		clip 		asigL, i(kmethod)-1, i(klimit), i(karg)
	asigR		clip 		asigR, i(kmethod)-1, i(klimit), i(karg)
	rireturn			; RETURN TO PERFORMANCE PASSES FROM INITIALISATION PASS
	if (kGainComp==1) then
	 asigL		=			asigL * (1/sqrt(klimit))
	 asigR		=			asigR * (1/sqrt(klimit))
	endif
			outs		asigL*klevel, asigR*klevel
endin

</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
</CsScore>

</CsoundSynthesizer>; LoFi.csd
; Written by Iain McCurdy, 2012.

<Cabbage>
form size(230, 120), caption("Lo Fi"), pluginid("lofi"), scrollbars(0) style("legacy")
image pos(0, 0), size(230, 120), colour(0,0,0), shape("sharp")
image   bounds( 30, 94, 30, 15), shape("rounded"), colour("black")
image   bounds( 85, 70, 60, 15), shape("rounded"), colour("black")
image   bounds(165, 94, 40, 15), shape("rounded"), colour("black")
rslider bounds(  5, 31, 80, 80), text("Bits"),     channel("bits"),  range(1, 16, 16),        textcolour("White"),    colour("orange"), trackercolour("brown"), outlinecolour("grey")
rslider bounds( 75,  7, 80, 80), text("Foldover"), channel("fold"),  range(1, 1024, 0, 0.25), textcolour("White"),    colour("orange"), trackercolour("brown"), outlinecolour("grey")
rslider bounds(145, 31, 80, 80), text("Level"),    channel("level"), range(0, 1.00, 1),       textcolour("White"),    colour("orange"), trackercolour("brown"), outlinecolour("grey")
}
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-d -n
</CsOptions>
<CsInstruments>
sr = 44100
ksmps = 32
nchnls = 2
0dbfs = 1

;Iain McCurdy, 2012
;http://iainmccurdy.org/csound.html


opcode	LoFi,a,akk
	ain,kbits,kfold	xin									;READ IN INPUT ARGUMENTS
	kvalues		pow		2, kbits					;RAISES 2 TO THE POWER OF kbitdepth. THE OUTPUT VALUE REPRESENTS THE NUMBER OF POSSIBLE VALUES AT THAT PARTICULAR BIT DEPTH
	aout		=	(int((ain/0dbfs)*kvalues))/kvalues	;BIT DEPTH REDUCE AUDIO SIGNAL
	aout		fold 	aout, kfold							;APPLY SAMPLING RATE FOLDOVER
		xout	aout									;SEND AUDIO BACK TO CALLER INSTRUMENT
endop

instr 1 
kbits chnget "bits"
kfold chnget "fold"
klevel chnget "level"
a1,a2	ins
kporttime	linseg	0,0.001,0.01
kfold	portk	kfold,kporttime
a1	LoFi	a1,kbits*0.6,kfold
a2	LoFi	a2,kbits*0.6,kfold
a1	=	a1 * klevel
a2	=	a2 * klevel
	outs	a1,a2
endin

</CsInstruments>

<CsScore>
i 1 0 [60*60*24*7]
</CsScore>

</CsoundSynthesizer>; pdclip.csd
; Written by Iain McCurdy, 2012.

<Cabbage>
form caption("pdclip") size(380, 90), pluginid("pdcl") style("legacy")
label        bounds(  5,-12,380,100), colour(20,100,100, 0), fontcolour(70,150,150,200), text("pdclip"), shape("rounded"), outlinecolour("white"), outlinethickness(4) 
image        bounds(  0,  0,380, 90), colour(10,100,200,200), shape("rounded"), outlinecolour("white"), outlinethickness(4) 
rslider      bounds( 10, 11, 70, 70), text("Input Gain"),  channel("InGain"),  range(0, 5.00, 1,0.375),     textcolour("white"), colour(5, 30,80), trackercolour(255,255,25), outlinecolour(30,30,50)
rslider      bounds( 75, 11, 70, 70), text("Width"),  channel("width"),  range(0, 1.00, 0),     textcolour("white"), colour(5, 30,80), trackercolour(255,255,25), outlinecolour(30,30,50)
rslider      bounds(140, 11, 70, 70), text("Centre"), channel("center"), range(-1.00, 1.00, 0), textcolour("white"), colour(5, 30,80), trackercolour(255,255,25), outlinecolour(30,30,50)
rslider      bounds(300, 11, 70, 70), text("Level"),  channel("level"),  range(0, 1.00, 0.7),   textcolour("white"), colour(5, 30,80), trackercolour(255,255,25), outlinecolour(30,30,50)
combobox     bounds(215, 20, 80, 20), channel("bipolar"), value(2), text("Unipolar", "Bipolar")
checkbox     bounds(215, 48,130, 12), channel("TestTone"), FontColour("white"),  value(0), text("Sine Tone"), colour(yellow)
}
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

;Author: Iain McCurdy (2012)

gisine	ftgen	0,0,4096,10,1

instr	1
	kporttime	linseg	0,0.001,0.05				; portamento time ramps up from zero
	gkInGain	chnget	"InGain"			;READ WIDGETS...
	gkInGain	portk	gkInGain,kporttime
	gkwidth		chnget	"width"
	gkwidth		portk	gkwidth,kporttime
	gkcenter	chnget	"center"			;
	gkcenter	portk	gkcenter,kporttime
	gkbipolar	chnget	"bipolar"			;
	gkbipolar	init	1
	gkbipolar	=	gkbipolar-1
	gklevel		chnget	"level"				;
	gklevel		portk	gklevel,kporttime
	gkTestTone	chnget	"TestTone"
	if gkTestTone==1 then						; if test tone selected...
	 koct	rspline	4,8,0.2,0.5						
	 asigL		poscil	1,cpsoct(koct),gisine			; ...generate a tone
	 asigR		=	asigL					; right channel equal to left channel
	else								; otherwise...
	 asigL, asigR	ins						; read live inputs
	endif

	ifullscale	=	0dbfs	;DEFINE FULLSCALE AMPLITUDE VALUE
	kSwitch		changed		gkbipolar	;GENERATE A MOMENTARY '1' PULSE IN OUTPUT 'kSwitch' IF ANY OF THE SCANNED INPUT VARIABLES CHANGE. (OUTPUT 'kSwitch' IS NORMALLY ZERO)
	if	kSwitch=1	then	;IF A VARIABLE CHANGE INDICATOR IS RECEIVED...
		reinit	START		;...BEGIN A REINITIALISATION PASS FROM LABEL 'START' 
	endif				;END OF CONDITIONAL BRANCHING
	START:				;LABEL
	;CLIP THE AUDIO SIGNAL USING pdclip
	aL		pdclip		asigL*gkInGain, gkwidth, gkcenter, i(gkbipolar)	; [, ifullscale]]
	aR		pdclip		asigR*gkInGain, gkwidth, gkcenter, i(gkbipolar)	; [, ifullscale]]
	rireturn			;RETURN TO PERFORMANCE PASSES FROM INITIALISATION PASS
	alevel		interp		gklevel
			outs		aL * alevel, aR * alevel		;pdclip OUTPUT ARE SENT TO THE SPEAKERS
endin
		
</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
</CsScore>


</CsoundSynthesizer>



























; Powershape.csd
; Iain McCurdy 2012
;
; Encapsulation of the powershape opcode used for wave distortion
;
; CONTROLS
; --------
; Test Tone	--	a glissandoing test tone
; Amount	--	amount of distortion
; Level		--	output level

<Cabbage>
form caption("Powershape") size(300,160), pluginid("pshp") style("legacy")
image               bounds(0, 0,300,160), colour("Brown"), shape("rounded"), outlinecolour("white"), outlinethickness(4) 
checkbox bounds(10,  8,130, 12), channel("TestTone"), FontColour("white"),  value(0), text("Sine Tone"), colour(yellow)
hslider bounds(  5, 18,290, 40),          colour("white"), trackercolour("white"), channel("amount"), range(0.1, 1000, 1, 0.25,0.001)
label   bounds(129, 50, 40, 11), text("Amount"), fontcolour("white")
nslider  bounds(100, 65,100, 40), text("Amount [type]"),  channel("amount"),  range(0.1, 1000, 1,1,0.001), textcolour(white)
hslider bounds(  5,108,290, 40),          colour("white"), trackercolour("white"),  channel("level"),  range(0, 50, 0.5, 0.25,0.000001)    
label   bounds(135,140, 27, 11), text("Level"), fontcolour("white")
}
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

gisine	ftgen	0,0,4096,10,1 ;,0,1/2,0,1/4,0,1/8,0,1/16,0,1/32,0,1/64

instr	1
	kporttime	linseg	0,0.001,0.05				; portamento time ramps up from zero
	gkshape		chnget	"amount"				; READ WIDGETS...
	gkshape		portk	gkshape,kporttime
	gklevel		chnget	"level"					;
	gklevel		portk	gklevel,kporttime
	gklevel		portk	gklevel,kporttime
	gkTestTone	chnget	"TestTone"
	if gkTestTone==1 then						; if test tone selected...
	 koct	rspline	4,8,0.2,0.5						
	 asigL		poscil	1,cpsoct(koct),gisine			; ...generate a tone
	 asigR		=	asigL					; right channel equal to left channel
	else								; otherwise...
	 asigL, asigR	ins						; read live inputs
	endif
	ifullscale	=	0dbfs	;DEFINE FULLSCALE AMPLITUDE VALUE
	aL 		powershape 	asigL, gkshape, ifullscale	;CREATE POWERSHAPED SIGNAL
	aR 		powershape 	asigR, gkshape, ifullscale	;CREATE POWERSHAPED SIGNAL
	alevel		interp		gklevel
			outs		aL * alevel, aR * alevel		;WAVESET OUTPUT ARE SENT TO THE SPEAKERS
endin
		
</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
</CsScore>


</CsoundSynthesizer>



























; Compressor.csd
; Written by Iain McCurdy, 2016.

; Encapsulation of the compressor opcode

; A compressor is a dynamics processor that essentially applies waveshaping to an audio signal.
; In the case of a typical compressor, increasingly high dynamics will be increasingly attenuated thereby reducing the dynamic range of a signal.
; This can be useful in forcing a dynamically expressive signal to sit better within a mix of signals or for processing a final mix so that it functions better when listened to in an environment with background noise. 

; When reference is made to a soft knee, this is the part of the dynamic mapping function that separates the lower response region and the upper response region. 
; Below the 'knee' the dynamic tranformation is 1:1, i.e. no change. 

; Threshold	-	Lowest decibel level that will be allowed through. Can be used to remove low level noise from a signal. A setting of zero will defeat this feature.  
; Low Knee	-	Decibel point at which the 'soft knee' 
; High Knee	-	Decibel point at which the 'soft knee' terminates
; Attack	-	Attack time of the compressor
; Release	-	Release time of the compressor
; Ratio		-	Compression ratio of the upper compressor response region
; Lookahead	-	Essentially this is a delay that will be applied to the signal that will be compressed (the track signal will always be un-delayed).
;			This can be useful for making sure that the compressor responds appropriately to fast attacks (at the expense of some added latency by the delay).
;			This is an initialisation time parameter so making changes to it will interrupt the realtime audio stream
; Gain		-	A make-up gain control. Use this to compensate for loss of power caused by certain ocmpressor settings.

<Cabbage>
#define SLIDER_APPEARANCE trackercolour("DarkSlateGrey"), textcolour("black") 
form caption("Compressor") size(440,130), pluginid("Comp") style("legacy")
image            bounds(  0,  0,440,130), outlinethickness(6), outlinecolour("white"), colour("grey")
rslider bounds( 10, 10, 70, 70), channel("thresh"), text("Threshold"), range(0,120,0), $SLIDER_APPEARANCE
rslider bounds( 80, 10, 70, 70), channel("att"), text("Attack"),  range(0,1,0.01,0.5), $SLIDER_APPEARANCE
rslider bounds(150, 10, 70, 70), channel("rel"), text("Release"), range(0,1,0.05,0.5), $SLIDER_APPEARANCE
rslider bounds(220, 10, 70, 70), channel("ratio"), text("Ratio"), range(1,300,10000,0.5), $SLIDER_APPEARANCE
rslider bounds(290, 10, 70, 70), channel("look"), text("Lookahead"), range(0,1,0.01,0.5), $SLIDER_APPEARANCE
rslider bounds(360, 10, 70, 70), channel("gain"), text("Gain"), range(-36,36,0), $SLIDER_APPEARANCE
hrange   bounds( 10, 80,420, 30), channel("LowKnee","HighKnee"), range(0, 120, 48:60), $SLIDER_APPEARANCE
label    bounds( 10,108,420, 13), text("Soft Knee"), fontcolour("black")
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-d -n
</CsOptions>

<CsInstruments>

sr = 44100
ksmps = 32
nchnls = 2
0dbfs = 1

; Author: Iain McCurdy (2016)

instr 1
 aL,aR		ins									; read in live audio
 kthresh	chnget		"thresh"						; read in widgets
 kLowKnee	chnget		"LowKnee"
 kHighKnee	chnget		"HighKnee"
 katt		chnget		"att"
 krel		chnget		"rel"
 kratio 	chnget		"ratio"
 kgain	 	chnget		"gain"
 klook 		chnget		"look"  						; look-ahead time (this will have to be cast as an i-time variable)
 klook		init		0.01							
 if changed(klook)==1 then								; if slider is moved...
  reinit REINIT										; ... reinit
 endif
 REINIT:
 aC_L 	compress aL, aL, kthresh, kLowKnee, kHighKnee, kratio, katt, krel, i(klook)	; compress both channels
 aC_R 	compress aR, aR, kthresh, kLowKnee, kHighKnee, kratio, katt, krel, i(klook)
 aC_L	*=	ampdb(kgain)								; apply make up gain
 aC_R	*=	ampdb(kgain)
	outs	aC_L,aC_R
endin

</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]									; play instr 1 for 1 week
</CsScore>

</CsoundSynthesizer>; Compressor/Expander.csd
; Written by Iain McCurdy, 2015.

; A dynamics processor
; Read point on the transfer function is indicated by a moving dot.

; Threshold	-	threshold point at which the main dynamics processing curve begins
; Compress	-	degree of dynamic compression of the dynamic area beneath the threshold
; Curve		-	curve of the main dynamics processing segment
; Ceiling	-	dynamic ceiling. Shifts main dynamic curve to the left in the transfer function.
; Attack	-	Attack time of the dynamic tracker. Increase to smooth out dynamic changes carried out.
; Release	-	Release time of the dynamic tracker. Increase to smooth out dynamic changes carried out.
; Smoothing	-	A lowpass filter is applied to the output gain function. increasing 'Smoothing' will smooth changes in dynamics.
;			This is in addition to the attack and release parameters used in the follow2 opcode but may produce nicer smoothing.
; Pre-gain	-	Boost the input signal before dynamic tracking takes place. 
;			This does not affect the output level but merely forces reading further across the transfer function - this is confirmed by observing the dot on the graph. 
; Sensitivity	-	Adjusts the range of dynamic change the effect will apply (without having to adjust the transfer function.  
; Mix		-	Dry/wet mix. This should normally be set at maximum '1' but switching back to '0' (dry) may be informative in auditioning changes.  
; Level		-	Output level

<Cabbage>
form caption("Compressor/Expander") size(890, 100), pluginid("CoEx") style("legacy")
image                       bounds(0, 0, 890, 100), colour("tan"), shape("rounded"), outlinecolour("white"), outlinethickness(6)
rslider  bounds( 10, 13, 75, 75), text("Threshold"), channel("thresh"), range(0,1,0.143,0.5,0.001),     textcolour("black"), colour(37,59,59), trackercolour("Silver")
rslider  bounds( 80, 13, 75, 75), text("Compress"),  channel("comp"),   range(0,1,0.25),               textcolour("black"), colour(37,59,59), trackercolour("Silver")
rslider  bounds(150, 13, 75, 75), text("Curve"),     channel("curve"),  range(-20,20,2.6),             textcolour("black"), colour(37,59,59), trackercolour("Silver")
rslider  bounds(220, 13, 75, 75), text("Ceiling"),   channel("ceil"),   range(0,1,0),                 textcolour("black"), colour(37,59,59), trackercolour("Silver")
gentable bounds(300, 10, 80, 80), tablenumber(1), tablecolour("black"), identchannel("table"), amprange(0,1,-1), zoom(-1), tablegridcolour(white), tablebackgroundcolour(white), outlinethickness(1)
image    bounds(0, 0, 6, 6), colour("black"), shape("ellipse"), outlinethickness(0), identchannel("Dot")
rslider  bounds(385, 13, 75, 75), text("Attack"),    channel("att"),    range(0.01,1,0.3,0.5,0.01),   textcolour("black"), colour(37,59,59), trackercolour("Silver")
rslider  bounds(455, 13, 75, 75), text("Release"),   channel("rel"),    range(0.01,1,0.3,0.5,0.01),   textcolour("black"), colour(37,59,59), trackercolour("Silver")
rslider  bounds(525, 13, 75, 75), text("Smoothing"), channel("smooth"), range(0.01,1,0.1,0.5,0.001),  textcolour("black"), colour(37,59,59), trackercolour("Silver")
rslider  bounds(595, 13, 75, 75), text("Pre-gain"),  channel("PreGain"),range(0.5,10,2.4,0.5,0.001),  textcolour("black"), colour(37,59,59), trackercolour("Silver")
rslider  bounds(665, 13, 75, 75), text("Sensitivity"),channel("sense"), range(1,10,3),               textcolour("black"), colour(37,59,59), trackercolour("Silver")
rslider  bounds(735, 13, 75, 75), text("Mix"),       channel("mix"),    range(0,1,1),                 textcolour("black"), colour(37,59,59), trackercolour("Silver")
rslider  bounds(805, 13, 75, 75), text("Level"),     channel("lev"),    range(0,5,0.5,0.5,0.001),     textcolour("black"), colour(37,59,59), trackercolour("Silver")

</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-d -n
</CsOptions>

<CsInstruments>
sr = 44100
ksmps = 32
nchnls = 2
0dbfs = 1


instr 1
 kthresh	chnget "thresh"
 kcomp		chnget "comp"
 kcurve	 	chnget "curve"
 kceil	 	chnget "ceil"
 katt	 	chnget "att"
 krel	 	chnget "rel"
 ksmooth 	chnget "smooth"
 kPreGain 	chnget "PreGain"
 ksense 	chnget "sense" 
 kmix	 	chnget "mix"
 klev	 	chnget "lev"
 
 kpeg	metro	2				; maximum rate at which table will be rebuilt upon receipt of changes to Threshold, Compress, Curve or Ceiling
 if kpeg==1 then
  if changed(kthresh,kcomp,kcurve,kceil)==1 then
   reinit REBUILD_TABLE
  endif
 endif
 REBUILD_TABLE:
 icurve		=	i(kcurve)
 ithresh	=	i(kthresh)
 iTabLen	=	4096
 ithreshi	=	int(iTabLen*ithresh)	; threshold as a table point x value
 irem		=	iTabLen-ithreshi	; remainder table points
 iceil		=	i(kceil)
 irem1		=	int(irem*(1-iceil))
 irem2		=	irem-irem1
 icomp		=	i(kcomp)
 giTransferFunc	ftgen 1, 0, iTabLen, 16, 0, ithreshi, 0, ithresh*icomp, irem1, icurve, 1, irem2, 0, 1
 chnset	"tablenumber(1)","table"
 
 aL,aR	ins
 ;aL,aR	diskin2	"ClassicalGuitar.wav",1,0,1
 
 aFollow	follow2		(aL+aR)*kPreGain,katt,krel
 ;aFollow	pow	aFollow,0.5
 kRMS		downsamp	aFollow

 /* apply transfer function */
 aMap	table	aFollow,giTransferFunc,1		; read transfer function value
 aDiff	=	1+(aMap-aFollow)			; divergence from flat dynamic response 
 aDiff	pow	aDiff,ksense				; raising to a power greater than 1 increases range of change: greater attenuation, greater amplification
 aDiff	tone	aDiff,1/ksmooth				; smoothing. This is in addition to the attack and release parameters used in the follow2 opcode but may produce nicer smoothing. Reduce this value for greater smoothing, increase it for less.
 aDynL	=	aL * aDiff				; apply dynamic change
 aDynR	=	aR * aDiff
 aMixL	ntrpol	aL,aDynL,kmix				; dry / wet mix
 aMixR	ntrpol	aR,aDynR,kmix
 	outs	aMixL*klev, aMixR*klev			; send audio to outputs

 /* DISPLAY */
 if metro(ksmps*0.5)==1 then
  kx	limit	kRMS,0,1				; limit x position
  ky	table	kx,giTransferFunc,1			; derice y position (0 to 1)
  Smsg	sprintfk	"pos(%d,%d)", 297+int(kx*80),7+(80-int(ky*80))	; create message string
  	chnset	Smsg,"Dot"				; send message string to gentable widget
 endif

endin

</CsInstruments>

<CsScore>                                              
i 1 0 [60*60*24*7]
</CsScore>

</CsoundSynthesizer>                                                  ; Exciter.csd
; Written by Iain McCurdy, 2015.
                
; An exciter is a device that uses harmonic distortion to add a high frequency shimmer to a signal. It is a common tool in mastering.
; Csound includes an opcode called 'exciter' which is an implementation of the 'Calf' exciter plugin. 
; 'Frequency' and 'Ceiling' define the frequency range across which harmonics will be created.      
; We also have control over the amount of harmonics created and the blend between the 2nd and 3rd order harmonics. 
; The effect of these parameters is subtle and the user might find it useful to at first set 'Dry/Wet Mix' to maximum (100% wet) in order to hear the effect more clearly.
             
<Cabbage>
form caption("Exciter") size(445, 100), pluginid("Ectr") style("legacy")
image           bounds(0, 0, 445, 100), colour("LightSlateGrey"), shape("rounded"), outlinecolour("white"), outlinethickness(6)
rslider  bounds( 10, 13, 75, 75), text("Frequency"), channel("freq"), range(20,20000,3000,0.5),  textcolour("white"), colour(37,59,59), trackercolour("Silver")
rslider  bounds( 80, 13, 75, 75), text("Ceiling"), channel("ceil"), range(20,20000,20000,0.5), textcolour("white"), colour(37,59,59), trackercolour("Silver")
rslider  bounds(150, 13, 75, 75), text("Harmonics"), channel("harms"), range(0.1,10.00,10),     textcolour("white"), colour(37,59,59), trackercolour("Silver")
rslider  bounds(220, 13, 75, 75), text("Blend"), channel("blend"), range(-10,10,10,0.5,0.0001),     textcolour("white"), colour(37,59,59), trackercolour("Silver")
rslider  bounds(290, 13, 75, 75), text("Mix"), channel("mix"), range(0, 1.00, 0.5),      textcolour("white"), colour(37,59,59), trackercolour("Silver")
rslider  bounds(360, 13, 75, 75), text("Level"), channel("level"), range(0, 1.00, 0.5),      textcolour("white"), colour(37,59,59), trackercolour("Silver")
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-d -n
</CsOptions>

<CsInstruments>
sr = 44100
ksmps = 32
nchnls = 2
0dbfs = 1



instr 1
 kporttime	linseg	0,0.001,0.05  			; ramping up value used for portamento time                                                   
 kfreq	chnget "freq"					; read in widgets
 kceil	chnget "ceil"
 kharms chnget "harms"
 kblend	chnget "blend"
 klevel chnget "level"
 kmix  	chnget "mix"
 kmix	portk	kmix,kporttime
 klevel	portk	klevel,kporttime
 
 a1,a2	ins
; a1,a2	diskin2	"ClassicalGuitar.wav",1,0,1		; test with a sound file
 
 aE1	exciter a1, kfreq, kceil, kharms, kblend
 aE2	exciter a1, kfreq, kceil, kharms, kblend

 a1	ntrpol	a1, aE1, kmix				; dry/wet mix
 a2	ntrpol	a2, aE2, kmix
 	outs	a1*klevel, a2*klevel			; send to outputs and apply amplitude level control
endin

</CsInstruments>

<CsScore>                                              
i 1 0 [60*60*24*7]					; play note for a week
</CsScore>

</CsoundSynthesizer>                                                  ; Gate.csd
; Written by Iain McCurdy, 2015.

; Two 'Processing Configutions' are available:
;  'Stereo Mixed' 		-	the two input channels are mixed before being sent to a single envelope follower. 
;					Subsequent processing (gating filtering) is still carried out on the stereo input, just the gate open/closed control data will be the same on both channels
;  'Stereo Separate' 		-	the two input channels are sent to independent envelope followers. 
;					Therefore gate open/closed control data for the two channels can differ.  
; Right Channel Sidechain	-	In this mode, the gate is triggered by the signal received in the right channel but the gated signal is the left channel 

; Input signal is sent to an envelope follower and a gate state (open/closed) is assessed according to user defined thresholds.
; Independent thresholds and time durations can be defined for gate opening and closing.

; Pre-filter filters the input signal through a highpass filter and lowpass filter in series. 
; (Note this is only the signal sent into the envelope follower and the signal sent to the actual gate and the then the output is unfiltered.)
; This feature can be used to fine tune gate opening for particular frequency bands. 
; E.G. Opening for a voice singing but not for low frequency rumble picked up through the microphone stand. 

; The 'Filter Gate' can gate the signal using a lowpass filter (either 12 dB/oct or 24 dB/oct). 
; The user sets the 'Min.' (gate closed) and 'Max.' (gate open) cutoff values for the filter (in oct format)

; 'Atten.' sets the amount of amplitude attenuation to be applied.
; 'Delay' delays the audio (after envelope following but before gating is applied)
;	This can be used to recover the attack of a sound, particularly if the attack time of the gate is long.
;	If delay time is zero then the delay is completely bypassed.  
                                                                       
<Cabbage>                                                                                                                   
form caption("Gate"), colour( 20, 20, 30), size(430, 305), pluginid("Gate"), scrollbars(0) style("legacy")

image     bounds(  5, 10,420, 40), shape("sharp"), outlinecolour("white"), colour(0,0,0,0), outlinethickness(1), plant("Input") {
label     bounds( 20, 10,180, 14), fontcolour("white"), text("Processing Configuration:"), colour(0,0,0,0), align("right")
combobox  bounds(202, 10,180, 16), channel("InputMode"), text("Stereo Mixed","Stereo Separate","Right Channel Side Chain"), value(1), colour( 70, 70, 70), fontcolour("white")
}

image     bounds(  5, 55,207, 90), shape("sharp"), outlinecolour("white"), colour(0,0,0,0), outlinethickness(1), plant("OnThresh") {
label     bounds(  0,  5,207, 12), fontcolour("white"), text("On Threshold"), colour(0,0,0,0)
rslider   bounds( 10, 20, 60, 60), range(0,0.1,0.01,0.5,0.0001),  channel("OnThresh"), text("Level"), textcolour("white")
nslider bounds( 65, 35, 45, 15), channel("OnThresh_dB"), range(-90,120,-90,1,0.1)
label     bounds(110, 37, 20, 12), fontcolour("white"), text("dB"), colour(0,0,0,0)
rslider   bounds(135, 20, 60, 60), range(0,0.3,0.04,0.5,0.0001),  channel("AttTime"), text("Time"), textcolour("white")
}

image     bounds(218, 55,207, 90), shape("sharp"), outlinecolour("white"), colour(0,0,0,0), outlinethickness(1), plant("OffThresh") {
label     bounds(  0,  5,207, 12), fontcolour("white"), text("Off Threshold"), colour(0,0,0,0)
rslider   bounds( 10, 20, 60, 60), range(0,0.1,0.007,0.5,0.0001),  channel("OffThresh"), text("Level"), textcolour("white")
nslider bounds( 65, 35, 45, 15), channel("OffThresh_dB"), range(-90,120,-90,1,0.1)
label     bounds(110, 37, 20, 12), fontcolour("white"), text("dB"), colour(0,0,0,0)
rslider   bounds(135, 20, 60, 60), range(0,0.3,0.03,0.5,0.0001),  channel("RelTime"), text("Time"), textcolour("white")
}

image    bounds(  5,150,420, 50), shape("sharp"), outlinecolour("white"), colour(0,0,0,0), outlinethickness(1), plant("PreFilter") {
checkbox bounds( 10, 15, 70, 12), channel("PreFiltOnOff"), text("Pre-Filter"), fontcolour("white")
;hslider2 bounds( 85,  0,330, 40), channel("HPF","LPF"), min(200), max(12000), range(20,20000, 0, 0.5, 1), identchannel("Filter")
hrange   bounds(  85,  5,330, 20), channel("HPF","LPF"), range(20, 20000, 200:12000, 0.5, 1), identchannel("Filter")
label    bounds( 85, 30,330, 12), text("Highpass / Lowpass"), fontcolour("white"), identchannel("FilterL")
}

image    bounds(  5,205,210, 95), shape("sharp"), outlinecolour("white"), colour(0,0,0,0), outlinethickness(1), plant("FilterGate") {
label    bounds(  5, 22, 80, 12), text("Filter Gate"), fontcolour("white")
combobox bounds(  5, 35, 80, 20), channel("FilterGate"), text("Bypass","12 dB/Oct","24 dB/Oct"), value(1)
rslider  bounds( 75,  5, 80, 80), range(2,14,2,0.5,0.01),  channel("FiltGateMin"), text("Min"), textcolour("white"), textbox(1)
rslider  bounds(135,  5, 80, 80), range(2,14,14,0.5,0.01),  channel("FiltGateMax"), text("Max"), textcolour("white"), textbox(1)
}

image    bounds(220,205,205, 95), shape("sharp"), outlinecolour("white"), colour(0,0,0,0), outlinethickness(1), plant("Master") {
checkbox bounds( 14, 30, 15, 13), channel("GateIndicOp"), shape("ellipse"), colour( 50,255, 50), active(0)
checkbox bounds( 14, 50, 15, 13), channel("GateIndicCl"), shape("ellipse"), colour(255, 50, 50), value(1), active(0)
rslider  bounds( 20,  5, 80, 80), range(0,90,90,0.5,0.1),  channel("Atten"), text("Atten."), textcolour("white"), textbox(1)
rslider  bounds( 75,  5, 80, 80), range(0,0.1,0,0.8,0.001),  channel("DelTim"), text("Delay"), textcolour("white"), textbox(1)
rslider  bounds(130,  5, 80, 80), range(0,2,1,0.5,0.01),  channel("Gain"), text("Gain"), textcolour("white"), textbox(1)
}

</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-dm0 -n
</CsOptions>

<CsInstruments>

sr	=	44100
ksmps	=	32
nchnls	=	2
0dbfs	=	1

;Author: Iain McCurdy (2015)

opcode	SwitchPort, k, kkk
	kin,kupport,kdnport	xin
	kold			init	0
	kporttime		=	(kin<kold?kdnport:kupport)
	kout			portk	kin, kporttime
	kold			=	kout
				xout	kout
endop

instr	1
 /* READ IN WIDGETS */
 kOnThresh	chnget	"OnThresh"
 kOffThresh	chnget	"OffThresh"
 kAttTime	chnget	"AttTime"
 kRelTime	chnget	"RelTime"
 kAtten		chnget	"Atten"
 kGain		chnget	"Gain"
 kFilterGate	chnget	"FilterGate"
 kFiltGateMin	chnget	"FiltGateMin"
 kFiltGateMax	chnget	"FiltGateMax"
 kInputMode	chnget	"InputMode"
 kHPF		chnget	"HPF"
 kLPF		chnget	"LPF"
 kPreFiltOnOff		chnget	"PreFiltOnOff"
 kDelTim	chnget	"DelTim"
 
 kporttime	linseg	0,0.001,0.05		;
 kGain		portk	kGain,kporttime

 /* INPUT */
 ;aL,aR	diskin2	"029_HPF.wav",1,0,1	; use diskin for testing
 aL,aR	ins
 
 /* SHOW OR HIDE PRE-FILTER SLIDERS */
 if changed(kPreFiltOnOff)==1 then
  if kPreFiltOnOff==1 then
   	chnset	"visible(1)","Filter"
   	chnset	"visible(1)","FilterL"
  else
   	chnset	"visible(0)","Filter"  
   	chnset	"visible(0)","FilterL"  
  endif
 endif
 


 /* STEREO MIX MODE */
 if kInputMode==1 then
 
  aInSig	sum	aL,aR		; mix left and right inputs

  if kPreFiltOnOff==1 then		; if pre-filter switch is on...
   aInSig	buthp	aInSig,kHPF	; highpass filter
   aInSig	buthp	aInSig,kHPF	; and again to steepen slope
   aInSig	butlp	aInSig,kLPF	; lowpass filter
   aInSig	butlp	aInSig,kLPF	; and again to steepen slope
  endif


  kRMS	rms	aInSig			; scan rms of input signal

  /* OPEN AND CLOSE GATE*/
  kGate	init	1
  if kRMS < kOffThresh && kGate ==1 then
   kGate	=	0
  elseif kRMS >= kOnThresh && kGate == 0 then
   kGate	=	1
  endif
  
  /* TURN GATE STATUS INDICATORS ON AND OFF */
  if changed(kGate)==1 then
  	chnset	1-kGate,"GateIndicCl"
  	chnset	kGate,"GateIndicOp"
  endif
   
  /* SMOOTH GATE OPENING AND CLOSING (CALL UDO) */
  kGateD SwitchPort kGate, kAttTime, kRelTime	; smooth opening and closing
  
  /* AMPLITUDE GATE */
  kGateDA	scale	kGateD,1,ampdb(-kAtten)	; modify gating function according to user defined attenuation setting
  aGate	interp	kGateDA				; create an arate version (smoother)
    
  /* DELAY */
  if kDelTim>0 then
   aL	vdelay	aL, kDelTim*1000, 100
   aR	vdelay	aR, kDelTim*1000, 100
  endif
   
  /* APPLY GATE */
  aL	*=	aGate
  aR	*=	aGate
  
  /* FILTER GATE */
  if kFilterGate>1 then				
   kcfoct	scale	kGateD,kFiltGateMax,kFiltGateMin
   acf	interp	cpsoct(kcfoct)
   if kFilterGate==2 then
    aL	tone	aL,acf
    aR	tone	aR,acf
   else
    aL	butlp	aL,acf
    aR	butlp	aR,acf
   endif
  endif
    
    
    
    
    
 /* STEREO SEPARATE MODE */
 elseif kInputMode==2 then
 
 aInSigL	=	aL
 aInSigR	=	aR

  if kPreFiltOnOff==1 then
   aInSigL	buthp	aInSigL,kHPF
   aInSigL	buthp	aInSigL,kHPF
   aInSigL	butlp	aInSigL,kLPF
   aInSigL	butlp	aInSigL,kLPF
   aInSigR	buthp	aInSigR,kHPF
   aInSigR	buthp	aInSigR,kHPF
   aInSigR	butlp	aInSigR,kLPF
   aInSigR	butlp	aInSigR,kLPF
  endif


  kRMSL	rms	aInSigL*2
  kRMSR	rms	aInSigR*2

  kGateL,kGateR	init	1

  if kRMSL < kOffThresh && kGateL ==1 then
   kGateL	=	0
  elseif kRMSL >= kOnThresh && kGateL == 0 then
   kGateL	=	1
  endif

  if kRMSR < kOffThresh && kGateR ==1 then
   kGateR	=	0
  elseif kRMSR >= kOnThresh && kGateR == 0 then
   kGateR	=	1
  endif
  
  if changed(kGateL)==1 then
  	chnset	1-kGateL,"GateIndicCl"
  	chnset	kGateL,"GateIndicOp"
  endif
   
  kGateDL SwitchPort kGateL, kAttTime, kRelTime	; smooth opening and closing
  kGateDR SwitchPort kGateR, kAttTime, kRelTime	; smooth opening and closing
  
  kGateDAL	scale	kGateDL,1,ampdb(-kAtten)
  kGateDAR	scale	kGateDR,1,ampdb(-kAtten)
  
  aGateL	interp	kGateDAL
  aGateR	interp	kGateDAR
   
  /* DELAY */
  if kDelTim>0 then
   aL	vdelay	aL, kDelTim*1000, 100
   aR	vdelay	aR, kDelTim*1000, 100
  endif
   
  /* APPLY GATE */
  aL	*=	aGateL
  aR	*=	aGateR
  
  if kFilterGate>1 then
   kcfoctL	scale	kGateDL,kFiltGateMax,kFiltGateMin
   kcfoctR	scale	kGateDR,kFiltGateMax,kFiltGateMin
   acfL	interp	cpsoct(kcfoctL)
   acfR	interp	cpsoct(kcfoctR)
   if kFilterGate==2 then
    aL	tone	aL,acfL
    aR	tone	aR,acfR
   else
    aL	butlp	aL,acfL
    aR	butlp	aR,acfR
   endif
  endif



 ; Right Channel Side Chain
 else
 
  if kPreFiltOnOff==1 then		; if pre-filter switch is on...
   aR	buthp	aR,kHPF	; highpass filter
   aR	buthp	aR,kHPF	; and again to steepen slope
   aR	butlp	aR,kLPF	; lowpass filter
   aR	butlp	aR,kLPF	; and again to steepen slope
  endif


  kRMS	rms	aR			; scan rms of input signal

  /* OPEN AND CLOSE GATE*/
  kGate	init	1
  if kRMS < kOffThresh && kGate ==1 then
   kGate	=	0
  elseif kRMS >= kOnThresh && kGate == 0 then
   kGate	=	1
  endif
  
  /* TURN GATE STATUS INDICATORS ON AND OFF */
  if changed(kGate)==1 then
  	chnset	1-kGate,"GateIndicCl"
  	chnset	kGate,"GateIndicOp"
  endif
   
  /* SMOOTH GATE OPENING AND CLOSING (CALL UDO) */
  kGateD SwitchPort kGate, kAttTime, kRelTime	; smooth opening and closing
  
  /* AMPLITUDE GATE */
  kGateDA	scale	kGateD,1,ampdb(-kAtten)	; modify gating function according to user defined attenuation setting
  aGate	interp	kGateDA				; create an arate version (smoother)
    
  /* DELAY */
  if kDelTim>0 then
   aL	vdelay	aL, kDelTim*1000, 100
  endif
   
  /* APPLY GATE */
  aL	*=	aGate
  
  /* FILTER GATE */
  if kFilterGate>1 then				
   kcfoct	scale	kGateD,kFiltGateMax,kFiltGateMin
   acf	interp	cpsoct(kcfoct)
   if kFilterGate==2 then
    aL	tone	aL,acf
   else
    aL	butlp	aL,acf
   endif
  endif
  aR	=	aL

 endif
 	outs	aL*kGain, aR*kGain

endin

</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
</CsScore>

</CsoundSynthesizer>
