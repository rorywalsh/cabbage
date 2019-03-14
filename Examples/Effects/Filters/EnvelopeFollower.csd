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
; Limiter.csd
; Written by Iain McCurdy, 2016.

; A simple 'brick-wall' limiter

; Threshold	-	dB threshold below above which signals will be aggressively limited
; Smoothing	-	response of the amplitude follower. Higher values result in a slower, but possibly smoother, response
; Delay		-	delay applied to the input sound before it is limited (the tracked signal is always undelayed)
;			This can be useful for compensating for a limiter than is not responding fast enough to sudden dynamic transients 
; Gain		-	Make up gain. Useful for compensating for gain loss.
   
<Cabbage>
#define SLIDER_APPEARANCE textcolour("black"), trackercolour("LightSlateGrey")
form caption("Limiter") size(435,105), pluginid("lmtr") style("legacy")
image         bounds(  0,  0,435,105), outlinethickness(6), outlinecolour("white"), colour("silver")
rslider  bounds( 10,15, 80, 80), channel("thresh"), text("Threshold [dB]"), range(-120,0,-24), $SLIDER_APPEARANCE
rslider  bounds( 90,15, 80, 80), channel("smooth"), text("Smoothing"), range(0.01,1,0.1,0.5), $SLIDER_APPEARANCE
rslider  bounds(170,15, 80, 80), channel("delay"), text("Delay [s]"), range(0,0.2,0,0.5), $SLIDER_APPEARANCE
rslider  bounds(250,15, 80, 80), channel("gain"), text("Gain [dB]"), range(-48,48,0), $SLIDER_APPEARANCE
checkbox bounds(335,35,100, 20), channel("limiting"), text("Limiting"), shape("ellipse"), colour("red"), fontcolour("black"), active(0)
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
 aL,aR		ins							; read live audio in
 kthresh	chnget		"thresh" 		; read in widgets
 ksmooth	chnget		"smooth" 		; this is needed as an i-time variable so will have to be cast as an i variable and a reinitialisation forced
 kthresh	=		ampdbfs(kthresh)	; convert threshold to an amplitude value
 if changed(ksmooth)==1 then			; if Smoothing slider is moved...
  reinit REINIT							; ... force a reinitialisation
 endif
 REINIT:								; reinitialise from here
 krmsL		rms		aL,1/i(ksmooth)		; scan both channels
 krmsR		rms		aR,1/i(ksmooth)		; ...
 rireturn								; return to performance pass from reinitialisation pass (if applicable)
 krms		max		krmsL,krmsR			; but only used the highest rms

 kdelay		chnget	"delay"				
 if kdelay>0 then						; if Delay value is anything above zero ...
  aL	vdelay	aL, kdelay*1000, 200	; delay audio signals before limiting
  aR	vdelay	aR, kdelay*1000, 200
 endif

 if krms>kthresh then					; if current RMS is above threshold; i.e. limiting required
  kfctr		=	kthresh/krms			; derive less than '1' factor required to attenuate audio signal to limiting value
  afctr		interp	kfctr				; smooth changes (and interpolate from k to a)
  aL_L		=	aL * afctr			; apply scaling factor
  aL_R		=	aR * afctr
  klimiting	=	1				; switch value used by GUI indicator (on)
 else
  aL_L		=	aL				; pass audio signals unchanged
  aL_R		=	aR				; ...
  klimiting	=	0				; switch value used by GUI indicator (off)
 endif

 kgain		chnget	"gain"				; make up gain control
 kgain		=	ampdb(kgain)			; derive gain value as an amplitude factor
 aL_L		*=	kgain				; make up gain
 aL_R		*=	kgain

 if metro(16)==1 then					; peg rate if updates of limiting indicator (to save a bit of CPU)
  		chnset	klimiting,"limiting"		; send value for limiting indicator
 endif

	outs	aL_L, aL_R				; send limited audio signals to outputs
endin

</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
</CsScore>

</CsoundSynthesizer>; BandFilter.csd
; Written by Iain McCurdy, 2012.

<Cabbage>
form caption("Band Filter"), colour(10,10,10), size(470, 360), pluginid("BdFl") style("legacy")
xypad bounds(5, 5, 350, 350), channel("cf", "bw"), rangex(0, 1, 0.5), rangey(0, 1, 0.3), text("x:cutoff | y:bandwidth"), colour(200,200,200)
checkbox bounds(370, 10, 20, 20), channel("balance"), FontColour("white"),  value(0)
label    bounds(395, 15, 55, 15), text("Balance"), FontColour("white")

label   bounds(375,  43, 75, 15), text("Filter Type"), FontColour("white")
combobox bounds(370,  60,85, 20), channel("type"), value(1), text("reson", "butterbp", "areson", "butterbr")

rslider bounds(368, 93, 90, 90), text("Mix"),		colour(27,59,59),trackercolour(127,159,159),textcolour("white"),fontcolour("white"), 		channel("mix"), 	range(0, 1.00, 1)
rslider bounds(368,190, 90, 90), text("Level"),		colour(27,59,59),trackercolour(127,159,159),textcolour("white"),fontcolour("white"), 		channel("level"), 	range(0, 1.00, 1)

nslider  bounds(360,283, 50, 30), text("CF"), textcolour("white"), channel("cfDisp"), range(1, 20000, 1, 1, 1)
nslider  bounds(415,283, 50, 30), text("BW"), textcolour("white"), channel("bwDisp"), range(1, 20000, 1, 1, 1)

</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-dm0 -n -+rtmidi=null -M0
</CsOptions>

<CsInstruments>
sr 	= 	44100
ksmps 	= 	32
nchnls 	= 	2
0dbfs	=	1

;Author: Iain McCurdy (2012)

instr	1
	kcf		chnget	"cf"
	kbw		chnget	"bw"
	kbalance	chnget	"balance"
	ktype		chnget	"type"
	kmix		chnget	"mix"
	klevel		chnget	"level"
	kporttime	linseg	0,0.001,0.05

	kcf	expcurve	kcf, 4
	kcf	scale	kcf,18000,20

	kbw	expcurve	kbw, 16
	kbw	scale	kbw,3,0.01

	aL,aR	ins
	;aL,aR	diskin2	"seashore.wav",1,0,1
	;aL	pinkish	1	;USE FOR TESTING
	;aR	pinkish	1
	
	kbw	limit	kbw*kcf,1,20000
	
	chnset	kcf,"cfDisp"	;send actual values for cutoff and bandwidth to GUI value boxes
	chnset	kbw,"bwDisp"

	kcf	portk	kcf,kporttime
	kbw	portk	kbw,kporttime	

	if ktype==1 then		;if reson chosen...
	 aFiltL	reson	aL,kcf,kbw,1
	 aFiltR	reson	aR,kcf,kbw,1
	elseif ktype==2 then		;or if butterworth bandpass is chosen
	 aFiltL	butbp	aL,kcf,kbw
	 aFiltR	butbp	aR,kcf,kbw
	elseif ktype==3 then		;or if areson  is chosen...
	 aFiltL	areson	aL,kcf,kbw,1
	 aFiltR	areson	aR,kcf,kbw,1
	else				;otherwise must be butterworth band reject
	 aFiltL	butbr	aL,kcf,kbw
	 aFiltR	butbr	aR,kcf,kbw
	endif
	if kbalance==1 then		;if 'balance' switch is on...
	 aFiltL	balance	aFiltL,aL,0.3	
	 aFiltR	balance	aFiltR,aR,0.3
	endif
	amixL	ntrpol	aL,aFiltL,kmix	;create wet/dry mix
	amixR	ntrpol	aR,aFiltR,kmix
		outs	amixL*klevel,amixR*klevel
endin

</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
</CsScore>

</CsoundSynthesizer>
; BandFilterII.csd
; Written by Iain McCurdy, 2015.

; Modify filter limits using the xypad

; The limit double slider is more just for display.
                                                                                   
<Cabbage>                                                                                                                   
form caption("Bandfilter II"), colour("LightSlateGrey"), size(510, 655), pluginid("BFII"), scrollbars(0) style("legacy")

checkbox bounds(9,3,90,15), channel("TestSound"), text("Test Sound"), value(0), fontcolour("black")

xypad bounds(5,20, 500, 500), channel("shift", "width"), rangex(0, 1, 0.5), rangey(0, 1, 0.5), text("x:cutoff y:bandwidth"), colour("LightSlateGrey"), fontcolour("white")

;hslider2 bounds(  5,513,500, 40), channel("Lim1","Lim2"), min(4), max(14), range(4, 14, 0, 1, 0.001)
hrange   bounds(  5,513, 500, 40), channel("Lim1","Lim2"), range(4, 14, 4:14, 1, 0.001)
;hslider   bounds(  5,513,500,15),   channel("Lim1"), range(4,14,4)
;hslider   bounds(  5,528,500,15),   channel("Lim2"), range(4,14,14)

label    bounds(  5,543,500, 13), text("L I M I T S"), fontcolour("black")

image    bounds(  5,560,155, 90), shape("sharp"), outlinecolour("black"), colour(0,0,0,0), outlinethickness(1)
checkbox bounds( 15,570, 80, 11), channel("HPF_active"), text("HPF Active"), fontcolour("black"), value(1)
label    bounds( 15,595, 80, 13), text("HPF Type"), fontcolour("black")
combobox bounds( 15,610, 80, 20), value(2), channel("HPF_type"),   text("6dB/oct","12dB/oct","24dB/oct","36dB/oct","48dB/oct","Resonant"), colour("black")
rslider  bounds( 95,560, 60, 85), range(0,1,0),  channel("HPF_res"),  identchannel("HPF_resID"),   text("Res"),   textbox(1), colour("black"), fontcolour("black"), textcolour("black"), fontcolour("black")

image    bounds(165,560,155, 90), shape("sharp"), outlinecolour("black"), colour(0,0,0,0), outlinethickness(1)
checkbox bounds(175,570, 80, 11), channel("LPF_active"), text("LPF Active"), fontcolour("black"), value(1)
label    bounds(175,595, 80, 13), text("LPF Type"), fontcolour("black")
combobox bounds(175,610, 80, 20), value(2), channel("LPF_type"),  text("6dB/oct","12dB/oct","24dB/oct","36dB/oct","48dB/oct","Resonant"), colour("black")
rslider  bounds(255,560, 60, 85), range(0,1,0),  channel("LPF_res"),  identchannel("LPF_resID"),   text("Res"),   textbox(1), colour("black"), fontcolour("black"), textcolour("black")

label    bounds(370,550, 95, 13), fontcolour("black"), text("Mode")
combobox bounds(370,565, 95, 20), channel("PassRej"), text("Bandpass","Bandreject","Bypass"), value(1)

label   bounds(330,591,170,13), fontcolour("black"), text("Smoothing")
hslider bounds(330,605,170,15), range(0,0.1,0.03,0.75,0.0001),  channel("Smoothing"), textcolour("black")

label   bounds(330,621,170,13), fontcolour("black"), text("Gain")
hslider bounds(330,635,170,15), range(0,1,1),  channel("Gain"), textcolour("black")
}
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-dm0 -n -+rtmidi=null -M0
</CsOptions>

<CsInstruments>

sr	=	44100
ksmps	=	32
nchnls	=	2
0dbfs	=	1

;Author: Iain McCurdy (2015)

opcode HighpassNetwork,aa,aakkk
 aL,aR,kHPF,kHPF_res,kHPF_type	xin
 if kHPF_type==1 then 
  aL	atone	aL, cpsoct(kHPF)
  aR	atone	aR, cpsoct(kHPF)
 elseif kHPF_type==2 then 
  aL	buthp	aL, cpsoct(kHPF)
  aR	buthp	aR, cpsoct(kHPF)
 elseif kHPF_type==3 then 
  aL	buthp	aL, cpsoct(kHPF)
  aR	buthp	aR, cpsoct(kHPF)
  aL	buthp	aL, cpsoct(kHPF)
  aR	buthp	aR, cpsoct(kHPF)
 elseif kHPF_type==4 then 
  aL	buthp	aL, cpsoct(kHPF)
  aR	buthp	aR, cpsoct(kHPF)
  aL	buthp	aL, cpsoct(kHPF)
  aR	buthp	aR, cpsoct(kHPF)
  aL	buthp	aL, cpsoct(kHPF)
  aR	buthp	aR, cpsoct(kHPF)
 elseif kHPF_type==5 then 
  aL	buthp	aL, cpsoct(kHPF)
  aR	buthp	aR, cpsoct(kHPF)
  aL	buthp	aL, cpsoct(kHPF)
  aR	buthp	aR, cpsoct(kHPF)
  aL	buthp	aL, cpsoct(kHPF)
  aR	buthp	aR, cpsoct(kHPF)
  aL	buthp	aL, cpsoct(kHPF)
  aR	buthp	aR, cpsoct(kHPF)
 elseif kHPF_type==6 then  
  aL	bqrez	aL,cpsoct(kHPF),1+(kHPF_res*40),1
  aR	bqrez	aR,cpsoct(kHPF),1+(kHPF_res*40),1
 endif
 	xout	aL,aR
endop

opcode LowpassNetwork,aa,aakkk
 aL,aR,kLPF,kLPF_res,kLPF_type	xin
 if kLPF_type==1 then                                           
  aL	tone	aL, cpsoct(kLPF)                                
  aR	tone	aR, cpsoct(kLPF)
 elseif kLPF_type==2 then 
  aL	butlp	aL, cpsoct(kLPF)
  aR	butlp	aR, cpsoct(kLPF)
 elseif kLPF_type==3 then 
  aL	butlp	aL, cpsoct(kLPF)
  aR	butlp	aR, cpsoct(kLPF)
  aL	butlp	aL, cpsoct(kLPF)
  aR	butlp	aR, cpsoct(kLPF)
 elseif kLPF_type==4 then 
  aL	butlp	aL, cpsoct(kLPF)
  aR	butlp	aR, cpsoct(kLPF)
  aL	butlp	aL, cpsoct(kLPF)
  aR	butlp	aR, cpsoct(kLPF)
  aL	butlp	aL, cpsoct(kLPF)
  aR	butlp	aR, cpsoct(kLPF)
 elseif kLPF_type==5 then 
  aL	butlp	aL, cpsoct(kLPF)
  aR	butlp	aR, cpsoct(kLPF)
  aL	butlp	aL, cpsoct(kLPF)
  aR	butlp	aR, cpsoct(kLPF)
  aL	butlp	aL, cpsoct(kLPF)
  aR	butlp	aR, cpsoct(kLPF)
  aL	butlp	aL, cpsoct(kLPF)
  aR	butlp	aR, cpsoct(kLPF)
 elseif kLPF_type==6 then
  aL	moogladder	aL,cpsoct(kLPF),kLPF_res
  aR	moogladder	aR,cpsoct(kLPF),kLPF_res
 endif
 	xout	aL,aR
endop

instr	1
 kshift	chnget	"shift"
 kwidth	chnget	"width"
 kgain	chnget	"Gain"
 kPassRej	chnget	"PassRej"
 kPassRej	init	1
 kTestSound	chnget	"TestSound"
 kSmoothing	chnget	"Smoothing"
 
 kLPF_type	chnget	"LPF_type"
 kHPF_type	chnget	"HPF_type"
 kLPF_type	init	2
 kHPF_type	init	2
 kLPF_res	chnget	"LPF_res"
 kHPF_res	chnget	"HPF_res"

 kLPF_active	chnget	"LPF_active"
 kHPF_active	chnget	"HPF_active"

 if changed(kHPF_type)==1 then
  if kHPF_type==6 then
   	chnset	"visible(1)","HPF_resID"
  else
   	chnset	"visible(0)","HPF_resID"
  endif
 endif  

 if changed(kLPF_type)==1 then
  if kLPF_type==6 then
   	chnset	"visible(1)","LPF_resID"
  else
   	chnset	"visible(0)","LPF_resID"
  endif
 endif  

 kLim1	limit	scale:k(kshift-(kwidth*kshift),14,4), 4, 14
 kLim2	limit	scale:k(kshift+(kwidth*kshift),14,4), 4, 14
 
 	chnset	kLim1,"Lim1"
 	chnset	kLim2,"Lim2"

 if kTestSound==1 then
  aL	pinkish	0.5	; use for testing
  aR	pinkish	0.5	; use for testing
 else
  aL,aR	ins
 endif
 
 kporttime	linseg	0,0.01,1
 kporttime	*=	kSmoothing
 kLim1	portk	kLim1,kporttime
 kLim2	portk	kLim2,kporttime
 
 if kPassRej==1 then
  if kHPF_active==1 then
   aL,aR	HighpassNetwork	aL,aR,kLim1,kHPF_res,kHPF_type
  endif
  if kLPF_active==1 then
   aL,aR	LowpassNetwork	aL,aR,kLim2,kLPF_res,kLPF_type
  endif
 elseif kPassRej==2 then
  if kHPF_active==1 then
   aL1,aR1	HighpassNetwork	aL,aR,kLim2,kHPF_res,kHPF_type
  endif
  if kLPF_active==1 then  
   aL2,aR2	LowpassNetwork	aL,aR,kLim1,kLPF_res,kLPF_type
  endif
  aL1,aR1,aL2,aR2	init	0
  aL	sum	aL1,aL2
  aR	sum	aR1,aR2
  	clear	aL1,aR1,aL2,aR2
 endif
 
 


 	outs	aL*kgain, aR*kgain

endin

</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
</CsScore>

</CsoundSynthesizer>
; clfilt.csd
; Written by Iain McCurdy, 2012.

; Port.		-	portamento applied to changes that are made to 'Freq.'

<Cabbage>
form caption("clfilt - Multi-Mode Filter") size(655, 90), pluginid("clfl"), scrollbars(0) style("legacy")
image                              bounds(0, 0, 655, 90), colour( 40,40,100), shape("rounded"), outlinecolour("white"), outlinethickness(4) 
rslider bounds( 10, 11, 70, 70), text("Freq."),       colour( 20,20, 80), 	fontcolour("white"), 	channel("cf"), 		range(20, 20000, 2000, 0.333), trackercolour("white"), outlinecolour(140,140,170), textcolour("white")
rslider bounds( 75, 16, 60, 60), text("Port."),       colour( 20,20, 80), 	fontcolour("white"), 	channel("port"), 	range(0,  50, 0.1, 0.5,0.01), trackercolour("white"), outlinecolour(140,140,170), textcolour("white")

rslider bounds(130, 11, 70, 70), text("N.Poles"),     colour( 20,20, 80), 	fontcolour("white"), 	channel("npol"),	range(2,80,24,1,2),            trackercolour("white"), outlinecolour(140,140,170), textcolour("white")
rslider bounds(195, 11, 70, 70), text("Ripple"),      colour( 20,20, 80), 	fontcolour("white"), 	channel("pbr"),		range(0.1,50,14),              trackercolour("white"), outlinecolour(140,140,170), textcolour("white")
rslider bounds(260, 11, 70, 70), text("Attenuation"), colour( 20,20, 80), 	fontcolour("white"), 	channel("sba"),		range(-120,-1,-60),            trackercolour("white"), outlinecolour(140,140,170), textcolour("white")
label                      bounds(365, 10, 40, 12), text("Type:"), fontcolour("white")
combobox channel("type"),  bounds(328, 23, 128, 18), value(1), text("Lowpass","Highpass")
label                      bounds(363, 46, 50, 12), text("Method:"), fontcolour("white")
combobox channel("kind"),  bounds(328, 59, 128, 18), value(2), text("Butterworth","Chebychev type I","Chebychev type II")
rslider bounds(455, 11, 70, 70), text("Mix"),	      colour( 20,20, 80),	fontcolour("white"), 	channel("mix"), 	range(0,1.00,1),   trackercolour("white"), outlinecolour(140,140,170), textcolour("white")
rslider bounds(520, 11, 70, 70), text("Level"),	      colour( 20,20, 80),	fontcolour("white"), 	channel("level"), 	range(0,1.00,0.3), trackercolour("white"), outlinecolour(140,140,170), textcolour("white")
checkbox channel("test"), pos(595, 35), text("TEST"), colour("yellow"), fontcolour("white"), size(60, 15), value(0)
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

;Author: Iain McCurdy (2013)

instr	1	; widgets input
	gkcf		chnget	"cf"
	gkport		chnget	"port"
	gknpol		chnget	"npol"
	gknpol		init	2
	gkpbr		chnget	"pbr"
	gksba		chnget	"sba"
	gktype		chnget	"type"
	gktype		=	gktype - 1
	gktype		init	0
	gkkind		chnget	"kind"
	gkkind		=	gkkind - 1
	gkkind		init	0
	gkmix		chnget	"mix"
	gklevel		chnget	"level"
	gktest		chnget	"test"
endin

instr	2	; clfilt - multimode filter
	kporttime	linseg	0,0.001,1
	kcf	portk	gkcf,kporttime*gkport
	kmix	portk	gkmix,kporttime*0.1
	klevel	portk	gklevel,kporttime
	if gktest=0 then
	 aL,aR	ins
	else
	 aL	pinkish	0.5
	 aR	=	aL
	endif
	
	ktrig	changed	gktype, gknpol, gkkind, gkpbr, gksba
	if ktrig==1 then
	 reinit	UPDATE
	endif
	UPDATE:
	aFiltL	clfilt	aL, kcf, i(gktype), i(gknpol), i(gkkind), i(gkpbr), i(gksba)
	aFiltR	clfilt	aR, kcf, i(gktype), i(gknpol), i(gkkind), i(gkpbr), i(gksba)
	rireturn

	aL	ntrpol	aL,aFiltL,kmix
	aR	ntrpol	aR,aFiltR,kmix
		outs	aL*klevel,aR*klevel
endin
		
</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
i 2 0.01 [3600*24*7]
</CsScore>

</CsoundSynthesizer>
; EnvelopeFollower.csd
; Written by Iain McCurdy, 2012.

<Cabbage>
form caption("Envelope Follower") size(530, 100), pluginid("envf") style("legacy")
image                  pos(0, 0), size(530, 100), colour("brown"), shape("rounded"), outlinecolour("white"), outlinethickness(4)
vmeter   bounds(20, 10, 15, 80) channel("Meter") value(0) outlinecolour("black"), overlaycolour(20, 3, 3,255) metercolour:0(255,100,100,255) metercolour:1(255,150,155, 255) metercolour:2(255,255,123, 255) outlinethickness(3) 

rslider bounds( 40, 11, 75, 75), text("Sensitivity"), channel("sens"),  range(0, 1, 0.65),                   colour(255,100,100), textcolour(255,255,200), trackercolour(255,255,150)
rslider bounds(110,  6, 45, 45), text("Att."),        channel("att"),   range(0.001, 0.5, 0.01, 0.5, 0.001), colour(255,200,100), textcolour(255,255,200), trackercolour(255,255,150)
rslider bounds(110, 51, 45, 45), text("Dec."),        channel("rel"),   range(0.001, 0.5, 0.2, 0.5, 0.001),  colour(255,200,100), textcolour(255,255,200), trackercolour(255,255,150)
rslider bounds(150, 11, 75, 75), text("Frequency"),   channel("freq"),  range(10, 10000, 1000, 0.5),         colour(255,100,100), textcolour(255,255,200), trackercolour(255,255,150)
label    bounds(225, 15, 85, 14), text("Type"), fontcolour(255,255,200)
combobox bounds(225, 30, 85, 20), text("lpf18","moogladder","butlp","tone"), value("1"), channel("type")
rslider bounds(310, 11, 75, 75), text("Resonance"),   channel("res"),   range(0,  1, 0.75),                  colour(255,100,100), textcolour(255,255,200), trackercolour(255,255,150), identchannel("resID")
rslider bounds(380, 11, 75, 75), text("Distortion"),  channel("dist"),  range(0,  1.00, 0),                  colour(255,100,100), textcolour(255,255,200), trackercolour(255,255,150), identchannel("distID")
rslider bounds(450, 11, 75, 75), text("Level"),       channel("level"), range(0, 1.00, 1),                   colour(255,200,100), textcolour(255,255,200), trackercolour(255,255,150)
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-d -n
</CsOptions>
<CsInstruments>
sr = 44100
ksmps = 64
nchnls = 2
0dbfs = 1

; Author: Iain McCurdy (2012)

opcode	EnvelopeFollower,a,akkkkkkk
	ain,ksens,katt,krel,kfreq,ktype,kres,kdist	xin			; READ IN INPUT ARGUMENTS
	setksmps	4
	;				     ATTCK.REL.  -  ADJUST THE RESPONSE OF THE ENVELOPE FOLLOWER HERE
	aFollow		follow2		ain, katt, krel			; AMPLITUDE FOLLOWING AUDIO SIGNAL
	kFollow		downsamp	aFollow				; DOWNSAMPLE TO K-RATE
	kFollow		expcurve	kFollow/0dbfs,0.5		; ADJUSTMENT OF THE RESPONSE OF DYNAMICS TO FILTER FREQUENCY MODULATION
	kFrq		=		kfreq + (kFollow*ksens*10000)	; CREATE A LEFT CHANNEL MODULATING FREQUENCY BASE ON THE STATIC VALUE CREATED BY kfreq AND THE AMOUNT OF DYNAMIC ENVELOPE FOLLOWING GOVERNED BY ksens
	kFrq		limit		kFrq, 20,sr/2			; LIMIT FREQUENCY RANGE TO PREVENT OUT OF RANGE FREQUENCIES  
	if ktype==1 then
	 aout		lpf18		ain, kFrq, kres, kdist		; REDEFINE AUDIO SIGNAL AS FILTERED VERSION OF ITSELF
	elseif ktype==2 then
	 aout		moogladder	ain, kFrq, kres			; REDEFINE AUDIO SIGNAL AS FILTERED VERSION OF ITSELF
	elseif ktype==3 then
	 aFrq	interp	kFrq
	 aout		butlp		ain, aFrq			; REDEFINE AUDIO SIGNAL AS FILTERED VERSION OF ITSELF
	elseif ktype==4 then
	 aout		tone		ain, kFrq			; REDEFINE AUDIO SIGNAL AS FILTERED VERSION OF ITSELF
	endif
			xout		aout				; SEND AUDIO BACK TO CALLER INSTRUMENT
endop

opcode	SwitchPort, k, kii
	kin,iupport,idnport	xin
	kold			init	0
	kporttime		=	(kin<kold?idnport:iupport)
	kout			portk	kin, kporttime
	kold			=	kout
				xout	kout
endop

instr 1
ksens chnget "sens"
katt chnget "att"
krel chnget "rel"
kfreq chnget "freq"
ktype chnget "type"
ktype	init	1
kres chnget "res"
kdist chnget "dist"
klevel chnget "level"
a1,a2	ins


if changed:k(ktype)==1 then
 if ktype==1 then
  chnset	"visible(1)","distID"
  chnset	"visible(1)","resID"
 elseif ktype==2 then
  chnset	"visible(0)","distID"
  chnset	"visible(1)","resID"
 else
  chnset	"visible(0)","distID"
  chnset	"visible(0)","resID"
 endif
endif

;a1,a2	diskin2	"808loop.wav",1,0,1
;a1	=	a1*0.4
;a2	=	a2*0.4

/*level meter*/
amix	sum	a1,a2
krms	rms	amix*0.5
krms	pow	krms,0.75
krms	SwitchPort	krms,0.01,0.05
		chnset	krms,"Meter"

a1	EnvelopeFollower	a1,ksens,katt,krel,kfreq,ktype,kres*0.95,kdist*100
a2	EnvelopeFollower	a2,ksens,katt,krel,kfreq,ktype,kres*0.95,kdist*100
a1	=	a1 * klevel * (1 - ((kdist*0.3)^0.02))	;scale amplitude according to distortion level (to compensate for gain increases it applies)
a2	=	a2 * klevel * (1 - ((kdist*0.3)^0.02))
;	outs	a1,a2
endin

</CsInstruments>
<CsScore>
i 1 0 [60*60*24*7]
</CsScore>
</CsoundSynthesizer>