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
</CsoundSynthesizer>; FilterLFO.csd
; Author: Iain McCurdy (2013)

; INTRODUCTION
; ------------
; multimode filter with a 2 multimode LFOs on the cutoff frequency
; additional controls for specific filter models are accessed using the pop-up buttons

; The outputs of both LFOs are added to the main cutoff frequency (Freq.)
; LFO amplitude are in 'octaves'
; LFO rates are in beats per minute
; 'Smoothing' adds a small amount of portamento to changes in cutoff frequency
;   this can be useful in square wave-type modulations 

; 'Type' is ignored when 'moogladder', 'resonz', 'phaser2' or 'resony' are chosen as 'model'

; some subtlety is required when using the more complex filter models (e.g. phaser2, resony)
;    often low LFO amplitudes and rate work better with these

; CONTROL
; -------
; Freq.		--	global manual frequency control. Like an LFO offset value.
; Res.		--	resonance control. Affects moogladder, resonz and phaser2 models
; Mix		--	dry/wet (filtered/unfiltered) mix
; Level		--	global output level control
; Model		--	(drop down menu) filter model
; Type		--	Filter type: highpass/lowpass - affects tone, butterworth, cl-butterworth, cl-Chebychev models only
; Input		--	choose between live input and (for testing) sawtooth tone and pink noise
; Resync	--	resync. (i.e. restart) the two LFOs
; clfilt/phaser2/resony	--	pop-up panels with further controls for these models
; LFO Type	--	sine, triangle, square (bipolar), square (unipolar), sawtooth up, sawtooth down, random sample & hold and random spline
; LFO Amp	--	amplitude of the LFOs
; LFO Rate	--	rates of the LFOs in beats per minute
; Link Rates	--	when this switch is activate Rate controls for the two LFOs will be linked
; LFO Rate Div.	--	integer division of Rate control value (unaffected by 'Link Rates' setting)
; Smooth	--	a small amount of smoothing can be appied to the LFO to smooth abrupt changes in value (may be useful and 'square' and 'rand.S&H' type modulations

<Cabbage>
form caption("Filter LFO") size(790,170), pluginid("FLFO") style("legacy")
image pos(0, 0),           size(790,170), colour(0,0,0,170), shape("rounded"), outlinecolour("white"), outlinethickness(4) 
rslider bounds( 10, 11, 70, 70), text("Freq."), colour( 30, 30, 30),	trackercolour("white"),	fontcolour("white"), 		channel("cf"), 		range(1, 20000, 300, 0.333)
rslider bounds( 75, 11, 70, 70), text("Res."),  colour( 30, 30, 30),	trackercolour("white"),	fontcolour("white"), 		channel("res"),		range(0,1.00,0.75)
rslider bounds(140, 11, 70, 70), text("Mix"),   colour( 30, 30, 30),	trackercolour("white"),	fontcolour("white"), 		channel("mix"), 	range(0,1.00,1)
rslider bounds(205, 11, 70, 70), text("Level"), colour( 30, 30, 30),	trackercolour("white"),	fontcolour("white"), 		channel("level"), 	range(0, 1.00, 0.2)

label    bounds( 40, 90, 50, 12), text("MODEL"), fontcolour("white")
combobox bounds( 20,102, 100, 18), channel("model"), value(6), text("Tone","Butterworth","Moogladder","cl-Butterworth","cl-Chebychev I","resonz","phaser2","resony")

label    bounds(160, 90, 50, 12), text("TYPE"),  fontcolour("white")
combobox bounds(140,102, 100, 18), channel("type"), value(1), text("Low-pass","High-pass")

label    bounds( 40,127, 50, 12), text("INPUT"), fontcolour("white")
combobox bounds( 20,139,100, 18), channel("input"), value(2), text("Live","Tone","Noise")

button   bounds(140,139, 80, 18), colour("Green"), text("RESYNC.", "RESYNC."), channel("resync"), value(1)

; controls pertaining to the setup of clfilt accessed in a pop-up panel.
button   bounds(280, 15,100, 30), channel("clfiltButton"), text("clfilt")
groupbox bounds(280, 15,150, 90),  colour("black"), plant("clfilt"), outlinethickness(0), popup(1), identchannel("clfiltPlant");, fontcolour("white")
{
rslider bounds(  5, 16, 70, 70), text("N.Poles"), colour( 30 , 30, 30),	trackercolour("white"), fontcolour("white"), channel("npol"),   range(2, 80, 2, 1, 2)
rslider bounds( 75, 16, 70, 70), text("Ripple"),  colour( 30 , 30, 30),	trackercolour("white"), fontcolour("white"), channel("pbr"),    range(0.1, 50.00, 1, 0.5, 0.001)
}

; controls pertaining to the setup of phaser2 accessed in a pop-up panel.
button   bounds(280, 55,100, 30), channel("phaser2Button"), text("phaser2")
groupbox bounds(280, 55,315, 90), colour("black"), plant("phaser2"), outlinethickness(0), popup(1), identchannel("phaser2Plant");, fontcolour(white)
{
rslider  bounds(  5, 16, 70, 70), text("Q"),       channel("q"),   range(0.0001,4,3),       colour( 30 , 30, 30),	trackercolour("white"), fontcolour("white")
rslider  bounds( 75, 16, 70, 70), text("N.Ords."), channel("ord"), range(1, 256, 8, 0.5,1), colour( 30 , 30, 30),	trackercolour("white"), fontcolour("white")
label    bounds(145, 20, 90, 14), text("Sep. Mode:"), fontcolour("white")
combobox bounds(145, 35, 90, 20), channel("mode"), value(1), text("Equal", "Power"), colour( 30 , 30, 30),	trackercolour("white"), fontcolour("white")
rslider  bounds(240, 16, 70, 70), text("Separation"), channel("sep"), range(-3, 3.00, 0.9), colour( 30 , 30, 30),	trackercolour("white"), fontcolour("white")
}

; controls pertaining to the setup of resony accessed in a pop-up panel.
button   bounds(280, 95,100, 30), channel("resonyButton"), text("resony")
groupbox bounds(280, 95,565, 90),  colour("black"), plant("resony"), outlinethickness(0), popup(1), identchannel("resonyPlant");, fontcolour(white)
{
rslider  bounds(  5, 16, 70, 70), text("BW."),           fontcolour("white"), channel("bw"),    range(0.01, 1000, 13, 0.5), colour( 30 , 30, 30),	trackercolour("white")
rslider  bounds( 75, 16, 70, 70), text("Num."),          fontcolour("white"), channel("num"),   range(1, 80, 10, 1,1),      colour( 30 , 30, 30),	trackercolour("white")
rslider  bounds(145, 16, 70, 70), text("Sep.oct."),      fontcolour("white"), channel("sepR"),  range(-11, 11, 2),          colour( 30 , 30, 30),	trackercolour("white")
rslider  bounds(215, 16, 70, 70), text("Sep.semi."),     fontcolour("white"), channel("sepR2"), range(-48, 48, 24,1,1),     colour( 30 , 30, 30),	trackercolour("white")
label    bounds(285, 20,130, 14), text("Scaling Mode"), fontcolour("white")
combobox bounds(285, 35,130, 20), channel("scl"), value(2), text("none", "peak response", "RMS")
label    bounds(425, 20,130, 14), text("Separation Mode"), fontcolour("white")
combobox bounds(425, 35,130, 20), channel("sepmode"), value(1), text("octs.total", "hertz", "octs.adjacent")
}

;checkbox bounds(400, 50,100, 15), text("Balance") channel("balance"), fontcolour("white"), colour("yellow")  value(0)


line bounds(390, 10,  2,150), colour("Grey")

;LFO
label    bounds(405, 11, 45, 17), text("LFO 1"), fontcolour("white")
combobox bounds(405, 50, 100, 18), channel("LFOtype"), value(3), text("Sine","Triangle","Square[bi]","Square[uni]","Saw Up","Saw Down","Rand.S&H","Rand.Spline")
rslider  bounds(515, 11, 70, 70), text("Amp"),      colour( 30, 30 ,30),	trackercolour("white"), fontcolour("white"), channel("LFOamp"), range(0, 9.00, 0.67)
rslider  bounds(580, 11, 70, 70), text("Rate"),     colour( 30, 30 ,30),	trackercolour("white"), fontcolour("white"), channel("LFOBPM"), range(0, 480, 60, 1, 1)
rslider  bounds(645, 11, 70, 70), text("Rate Div."),colour( 30, 30 ,30),	trackercolour("white"), fontcolour("white"), channel("LFOBPMDiv"), range(1, 64, 1, 1, 1)
rslider  bounds(710, 11, 70, 70), text("Smoothing"),colour( 30, 30 ,30),	trackercolour("white"), fontcolour("white"), channel("LFOport"), range(0, 0.1, 0.005, 0.25, 0.000001)
checkbox bounds(405, 31, 80, 12), text("Link Rates"), channel("RateLink"),colour(yellow), fontcolour("white"),  value(0)

;LFO2
label    bounds(405, 91, 45, 17), text("LFO 2"), fontcolour("white")
combobox bounds(405,130, 100, 18), channel("LFOtype2"), value(8), text("Sine","Triangle","Square[bi]","Square[uni]","Saw Up","Saw Down","Rand.S&H","Rand.Spline")
rslider  bounds(515, 91, 70, 70), text("Amp"), colour( 30, 30 ,30),	        trackercolour("white"), fontcolour("white"), channel("LFOamp2"), range(0, 9.00, 2.5)
rslider  bounds(580, 91, 70, 70), text("Rate"),colour( 30, 30 ,30),	        trackercolour("white"), fontcolour("white"), channel("LFOBPM2"), range(0, 480,  1, 1, 1)
rslider  bounds(645, 91, 70, 70), text("Rate Div."),colour( 30, 30 ,30),	trackercolour("white"), fontcolour("white"), channel("LFOBPMDiv2"), range(1, 64, 1, 1, 1)
rslider  bounds(710, 91, 70, 70), text("Smoothing"),colour( 30, 30 ,30),	trackercolour("white"), fontcolour("white"), channel("LFOport2"), range(0, 0.1, 0.001, 0.25, 0.000001)
checkbox bounds(405,111, 80, 12), text("Link Rates"), channel("RateLink"),colour(yellow), fontcolour("white"),  value(0)

label   bounds(220,150, 170, 12), text("Author: Iain McCurdy |2013|"), fontcolour("grey")
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

opcode	resony2,a,akkikii
	ain, kbf, kbw, inum, ksep , isepmode, iscl	xin

	;IF 'Octaves (Total)' MODE SELECTED...
	if isepmode==0 then
	 irescale	divz	inum,inum-1,1	;PREVENT ERROR IF NUMBER OF FILTERS = ZERO
	 ksep = ksep * irescale			;RESCALE SEPARATION
	
	;IF 'Hertz' MODE SELECTED...	
	elseif isepmode==1 then
	 inum	=	inum + 1
	 ksep	=	inum

	;IF 'Octaves (Adjacent)' MODE SELECTED...
	elseif isepmode==2 then 
	 irescale	divz	inum,inum-1,1	;PREVENT ERROR IF NUMBER OF FILTERS = ZERO
	 ksep = ksep * irescale			;RESCALE SEPARATION
	 ksep = ksep * (inum-1)			;RESCALE SEPARATION INTERVAL ACCORDING TO THE NUMBER OF FILTERS CHOSEN
	 isepmode	=	0
	endif

	aout 		resony 	ain, kbf, kbw, inum, ksep , isepmode, iscl, 0
			xout	aout
endop


instr	1
	kporttime	linseg	0,0.001,1

	kcf		chnget	"cf"				;
	kcf	portk	kcf,kporttime*0.05
	kres		chnget	"res"				;
	kmodel		chnget	"model"				;
	ktype		chnget	"type"				;
	kresync		chnget	"resync"				;

	kmix		chnget	"mix"				;
	klevel		chnget	"level"				;

	kpbr		chnget	"pbr"				;
	kpbr		init	1
	knpol		chnget	"npol"				;
	knpol		init	2

	kq		chnget	"q"					;
	kmode		chnget	"mode"					;
	kmode		init	1
	kmode		init	i(kmode)-1
	ksep		chnget	"sep"					;
	kfeedback	chnget	"feedback"				;
	kord		chnget	"ord"					;

	;resony
	kbw	chnget	"bw"
	knum	chnget	"num"
	ksepR	chnget	"sepR"
	ksepR2	chnget	"sepR2"
	ksepmode	chnget	"sepmode"
	ksepmode	=	ksepmode - 1
	ksepmode	init	1
	kscl	chnget	"scl"
	kscl	=	kscl - 1
	kscl	init	1

	;kbalance	chnget	"balance"			;

	kLFOtype	chnget	"LFOtype"
	kLFOamp		chnget	"LFOamp"
	kLFOamp		portk	kLFOamp, kporttime*0.05
	kLFOBPM		chnget	"LFOBPM"
	kLFOBPMDiv	chnget	"LFOBPMDiv"
	kLFOcps		=	(kLFOBPM*4)/(60*kLFOBPMDiv)
	kLFOport	chnget	"LFOport"

	kLFOtype2	chnget	"LFOtype2"
	kLFOamp2	chnget	"LFOamp2"
	kLFOamp2	portk	kLFOamp2, kporttime*0.05
	kLFOBPM2	chnget	"LFOBPM2"
	kLFOBPMDiv2	chnget	"LFOBPMDiv2"
	kLFOcps2	=	(kLFOBPM2*4)/(60*kLFOBPMDiv2)
	kLFOport2	chnget	"LFOport2"
	
	kRateLink		chnget	"RateLink"
	if kRateLink=1 then
	 ktrig1	changed	kLFOBPM
	 ktrig2	changed	kLFOBPM2
	 if ktrig1=1 then
	  chnset	kLFOBPM,"LFOBPM2"
	 elseif ktrig2=1 then
	  chnset	kLFOBPM2,"LFOBPM"
	 endif
	endif
	/* INPUT */
	kinput		chnget	"input"
	if kinput=1 then
	 aL,aR	ins
	elseif kinput=2 then
	 aL	vco2	0.2, 150
	 aR	=	aL
	else
	 aL	pinkish	0.2
	 aR	pinkish	0.2
	endif

	; RETRIGGERING FOR I-RATE VARIABLE
	kLFOtype	init	1
	kLFOtype2	init	1

	ktrig	changed	knpol,kpbr,kLFOtype,kLFOtype2,kmodel,ktype,kord,kmode, kscl, knum, ksepmode, kresync,kLFOBPMDiv,kLFOBPMDiv2,kRateLink
	if ktrig==1 then
	 reinit UPDATE
	endif
	UPDATE:
	
	; LFO
	if i(kLFOtype)==7 then
	 klfo	randomh	-kLFOamp, kLFOamp, kLFOcps
	elseif	i(kLFOtype)==8 then
	 klfo	jspline	kLFOamp, kLFOcps, kLFOcps*5
	else
	 klfo		lfo	kLFOamp, kLFOcps, i(kLFOtype)-1
	endif
	klfo		portk	klfo,kporttime*kLFOport
	;kcf		limit	kcf * octave(klfo),20,sr/2

	; LFO2
	if i(kLFOtype2)==7 then
	 klfo2	randomh	-kLFOamp2, kLFOamp2, kLFOcps2
	elseif	i(kLFOtype2)==8 then
	 klfo2	jspline	kLFOamp2, kLFOcps2, kLFOcps2*5
	else
	 klfo2		lfo	kLFOamp2, kLFOcps2, i(kLFOtype2)-1
	endif
	klfo2		portk	klfo2,kporttime*kLFOport2
	kcf		limit	kcf * octave(klfo) * octave(klfo2),20,sr/2


	; FILTER MODEL SELECT
	if i(kmodel)==1 then
	 if i(ktype)==1 then			; tone
	  aFiltL	tone	aL, kcf
	  aFiltR	tone	aR, kcf
	 else
	  aFiltL	atone	aL, kcf
	  aFiltR	atone	aR, kcf
	 endif
	elseif i(kmodel)==2 then		; butterworth
	 if i(ktype)==1 then
	  aFiltL	butlp	aL, kcf
	  aFiltR	butlp	aR, kcf
	 else
	  aFiltL	buthp	aL, kcf
	  aFiltR	buthp	aR, kcf        
	 endif
	elseif i(kmodel)==3 then			; moogladder
	 kres	scale		kres,0.95,0
	 aFiltL	moogladder	aL,kcf,kres
	 aFiltR	moogladder	aR,kcf,kres        
	elseif i(kmodel)==4 then			; cl-butterworth
	 if i(ktype)==1 then
	  aFiltL	clfilt	aL, kcf, 0, i(knpol)
	  aFiltR	clfilt	aR, kcf, 0, i(knpol)
	 else
	  aFiltL	clfilt	aL, kcf, 1, i(knpol)
	  aFiltR	clfilt	aR, kcf, 1, i(knpol)
	 endif
	elseif i(kmodel)==5 then			; cl-chebychev I
	 if i(ktype)==1 then
	  aFiltL	clfilt	aL, kcf, 0, i(knpol), 1, i(kpbr)
	  aFiltR	clfilt	aR, kcf, 0, i(knpol), 1, i(kpbr)
	 else
	  aFiltL	clfilt	aL, kcf, 1, i(knpol), 1, i(kpbr)
	  aFiltR	clfilt	aR, kcf, 1, i(knpol), 1, i(kpbr)
	 endif
	elseif i(kmodel)==6 then			; resonz
	 kres	logcurve	kres,4
	 kbw	scale	1-kres,3,0.1
	 aFiltL	resonz	aL, kcf, kcf*kbw, 1
	 aFiltR	resonz	aR, kcf, kcf*kbw, 1
	elseif i(kmodel)==7 then			; phaser2
	 kfeedback	scale	kres,0.99,0
	 aFiltL	phaser2		aL, kcf, kq, kord, kmode, ksep, kfeedback	; PHASER2 IS APPLIED TO THE LEFT CHANNEL
	 aFiltR	phaser2		aR, kcf, kq, kord, kmode, ksep, kfeedback	; PHASER1 IS APPLIED TO THE RIGHT CHANNEL
	elseif i(kmodel)==8 then			; resony	
	 ;CALL resony2 UDO
	 aFiltL 		resony2 aL, kcf, kbw, i(knum), ksepR , i(ksepmode), i(kscl)
	 aFiltR 		resony2	aR, kcf, kbw, i(knum), ksepR , i(ksepmode), i(kscl)
	endif
	
	rireturn

	/*
	if kbalance==1 then
	 aFiltL	balance	aFiltL,aL
	 aFiltR	balance	aFiltR,aR
	endif
	*/

	
	aL	ntrpol	aL,aFiltL,kmix
	aR	ntrpol	aR,aFiltR,kmix
		outs	aL*klevel,aR*klevel
endin

instr	UpdateWidgets
	ksepR	chnget	"sepR"
	ksepR2	chnget	"sepR2"
	ktrig1	changed	ksepR
	ktrig2	changed	ksepR2
	if ktrig1==1 then
	 chnset	ksepR*12, "sepR2"
	elseif  ktrig2==1 then
	 chnset	ksepR2/12, "sepR"
	endif
endin

instr 1000				; launches plant popups

klaunch	init	0

#define LAUNCH_PLANT(name)
#
kpressed	chnget "$name.Button"
if changed(kpressed)==1 then
  Smsg sprintfk "show(%d), pos(1, 19)", klaunch
  chnset Smsg, "$name.Plant"
endif
#

$LAUNCH_PLANT(clfilt)
$LAUNCH_PLANT(phaser2)
$LAUNCH_PLANT(resony)

klaunch	=	1

endin
	
</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
i "UpdateWidgets" 0 [3600*24*7]
i 1000 0 [60*60*24*7]	; plant pop-ups
</CsScore>

</CsoundSynthesizer>
; FilterShaper.csd
; Iain McCurdy, 2015

; Gestural shaping using rspline random function generators applied to lowpass filter, highpass filter, panning and amplitude

<Cabbage>
form caption("Filter Shaper") size(595, 200), pluginid("FlSh") style("legacy")
image               bounds(  0, 0, 595, 200), colour("Chocolate"), outlinecolour("white"), outlinethickness(2), shape("sharp")

line     bounds( 15,  4,125, 15)
label    bounds( 15,  5,125, 12), fontcolour("white"), text("R A T E")
rslider  bounds(  5, 20, 75, 75), text("Min"),  channel("RateMin"), range(0.01,20,0.5,0.5,0.01), textcolour("white"), colour(37,59,59), trackercolour("Silver")
rslider  bounds( 75, 20, 75, 75), text("Max"),  channel("RateMax"), range(0.01,20,3,0.5,0.01), textcolour("white"), colour(37,59,59), trackercolour("Silver")
checkbox bounds(160, 20, 75, 15), text("Bypass"),  channel("Bypass"), fontcolour("white")

image    bounds(220,  0,145, 95), colour(0,0,0,0), plant("panning") {
line     bounds( 15,  4,125, 15)
label    bounds( 15,  5,125, 12), fontcolour("white"), text("P A N N I N G")
rslider  bounds(  5, 20, 75, 75), text("Width"),channel("PanWidth"), range(0,1,1), textcolour("white"), colour(37,59,59), trackercolour("Silver")
rslider  bounds( 75, 20, 75, 75), text("Depth"),channel("PanDepth"), range(0,1,1), textcolour("white"), colour(37,59,59), trackercolour("Silver")
}

image    bounds(445,  0,145, 95), colour(0,0,0,0), plant("amplitude") {
line     bounds( 10,  4,125, 15)
label    bounds( 10,  5,125, 12), fontcolour("white"), text("A M P L I T U D E")
rslider  bounds(  0, 20, 75, 75), text("Min."),channel("AmpMin"), range(0,2,0.4,0.5,0.01), textcolour("white"), colour(37,59,59), trackercolour("Silver")
rslider  bounds( 70, 20, 75, 75), text("Max."),channel("AmpMax"), range(0,2,0.8,0.5,0.01), textcolour("white"), colour(37,59,59), trackercolour("Silver")
}

image    bounds( 15,100,285, 95), colour(0,0,0,0), plant("lowpass") {
line     bounds(  0,  4,275, 15)
label    bounds(  0,  5,275, 12), fontcolour("white"), text("L O W P A S S   F I L T E R")
label    bounds(  0, 28, 75, 12), fontcolour("white"), text("LPF Type"), fontcolour(100,100,100)
combobox bounds(  0, 40, 75, 20), channel("LPFtype"), text("Bypass","6 db/oct","12 db/oct","24 db/oct","Resonant"), textcolour("white"), colour(37,59,59), value(3)
rslider  bounds( 70, 20, 75, 75), text("Res."),channel("LPFres"), range(0,1,0.3,0.5,0.01), textcolour("white"), colour(37,59,59), trackercolour("Silver"), identchannel("LPFresID")
rslider  bounds(140, 20, 75, 75), text("Min."),channel("LPFmin"), range(4,14, 6), textcolour("white"), colour(37,59,59), trackercolour("Silver"), identchannel("LPFminID")
rslider  bounds(210, 20, 75, 75), text("Max."),channel("LPFmax"), range(4,14,14), textcolour("white"), colour(37,59,59), trackercolour("Silver"), identchannel("LPFmaxID")
}

image    bounds(305,100,285, 95), colour(0,0,0,0), plant("highpass") {
line     bounds(  0,  4,275, 15)
label    bounds(  0,  5,275, 12), fontcolour("white"), text("H I G H P A S S   F I L T E R")
label    bounds(  0, 28, 75, 12), fontcolour("white"), text("HPF Type"), fontcolour(100,100,100)
combobox bounds(  0, 40, 75, 20), channel("HPFtype"), text("Bypass","6 db/oct","12 db/oct","24 db/oct","Resonant"), textcolour("white"), colour(37,59,59), value(3)
rslider  bounds( 70, 20, 75, 75), text("Res."),channel("HPFres"), range(0,1,0.3,0.5,0.01), textcolour("white"), colour(37,59,59), trackercolour("Silver"), identchannel("HPFresID")
rslider  bounds(140, 20, 75, 75), text("Min."),channel("HPFmin"), range(4,14, 4), textcolour("white"), colour(37,59,59), trackercolour("Silver"), identchannel("HPFminID")
rslider  bounds(210, 20, 75, 75), text("Max."),channel("HPFmax"), range(4,14, 6), textcolour("white"), colour(37,59,59), trackercolour("Silver"), identchannel("HPFmaxID")
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

instr 1                                                         
 kRateMin	chnget "RateMin"
 kRateMax	chnget "RateMax"
 kLPFtype	chnget "LPFtype"
 kLPFres	chnget "LPFres"
 kLPFmin	chnget "LPFmin"
 kLPFmax	chnget "LPFmax"

 kHPFtype	chnget "HPFtype"
 kHPFres	chnget "HPFres"
 kHPFmin	chnget "HPFmin"
 kHPFmax	chnget "HPFmax"

 kPanWidth	chnget "PanWidth"
 kPanDepth	chnget "PanDepth"

 kAmpMin	chnget "AmpMin"
 kAmpMax	chnget "AmpMax"

 kBypass	chnget	"Bypass"
 
 aL,aR	ins
 
 if kBypass==1 then
  	outs	aL,aR
  	kgoto BYPASS
 endif
 
 ;aL	vco2	0.1,100
 ;aR	vco2	0.1,100
 
 kLPFoct	rspline	kLPFmin,kLPFmax,kRateMin,kRateMax
 kHPFoct	rspline	5,14,kRateMin,kRateMax

 kLPFoct	limit	kLPFoct,4,14
 kHPFoct	limit	kHPFoct,4,14
 
 if kLPFtype==2 then
  aL	tone	aL,cpsoct(kLPFoct)
  aR	tone	aR,cpsoct(kLPFoct)
 elseif kLPFtype==3 then
  aLPFoct	interp	kLPFoct
  aL	butlp	aL,cpsoct(aLPFoct)
  aR	butlp	aR,cpsoct(aLPFoct)
 elseif kLPFtype==4 then
  aLPFoct	interp	kLPFoct
  aL	butlp	aL,cpsoct(aLPFoct)
  aR	butlp	aR,cpsoct(aLPFoct)
  aL	butlp	aL,cpsoct(aLPFoct)
  aR	butlp	aR,cpsoct(aLPFoct)
 elseif kLPFtype==5 then
  aL	moogladder	aL,cpsoct(kLPFoct),kLPFres
  aR	moogladder	aR,cpsoct(kLPFoct),kLPFres
 endif

 if kHPFtype==2 then
  aL	atone	aL,cpsoct(kHPFoct)
  aR	atone	aR,cpsoct(kHPFoct)
 elseif kHPFtype==3 then
  aHPFoct	interp	kHPFoct
  aL	buthp	aL,cpsoct(aHPFoct)
  aR	buthp	aR,cpsoct(aHPFoct)
 elseif kHPFtype==4 then
  aHPFoct	interp	kHPFoct
  aL	buthp	aL,cpsoct(aHPFoct)
  aR	buthp	aR,cpsoct(aHPFoct)
  aL	buthp	aL,cpsoct(aHPFoct)
  aR	buthp	aR,cpsoct(aHPFoct)
 elseif kHPFtype==5 then
  aL	bqrez	aL,cpsoct(kHPFoct),1+(kHPFres*40),1
  aR	bqrez	aR,cpsoct(kHPFoct),1+(kHPFres*40),1
 endif

 aPan	rspline	-kPanWidth, kPanWidth, kRateMin, kRateMax
 aPan	=	(aPan*0.5) + 0.5 
 aOutL  =     aL * sin((aPan + 0.5) * $M_PI_2)
 aOutR  =     aR * cos((aPan + 0.5) * $M_PI_2)

 if kPanDepth>0 then
  aOutL	vdelay	aOutL, 0.1+(aPan*kPanDepth*3), 20
  aOutR	vdelay	aOutR, 0.1+((1-aPan)*kPanDepth*3), 20
 endif

 aAmp	rspline	kAmpMin, kAmpMax, kRateMin, kRateMax

	outs	aOutL*aAmp,aOutR*aAmp
 BYPASS:
endin

</CsInstruments>

<CsScore>                                              
i 1 0 [60*60*24*7]
</CsScore>

</CsoundSynthesizer>                                                  ; FormantFilter.csd

; reson 1 uses reson with scaling method 1.
; reson 2 uses reson with scaling method 2.
; 'gain' controls the gain the the bandpass filtered sound only.
; 'BW.Mult' is a factor which all five bandwidth values are multiplied by.
; 'Freq.Mult.' is a factor by which all cutoff frequencies are multiplied by.

<Cabbage>
form caption("Formant Filter"), colour("SlateGrey"), size(550, 280), pluginid("form")   style("legacy")

xypad bounds(5, 5, 350, 260), channel("x", "y"), rangex(0, 1, 0.5), rangey(0, 1, 0), text("upper edge:A E I | lower :U O"), fontcolour("white")

vslider bounds(360,  0, 30,140), text("f1"), channel("f1"), range(0, 1.00, 1), textcolour("white")
vslider bounds(380,  0, 30,140), text("f2"), channel("f2"), range(0, 1.00, 1), textcolour("white")
vslider bounds(400,  0, 30,140), text("f3"), channel("f3"), range(0, 1.00, 1), textcolour("white")
vslider bounds(420,  0, 30,140), text("f4"), channel("f4"), range(0, 1.00, 1), textcolour("white")
vslider bounds(440,  0, 30,140), text("f5"), channel("f5"), range(0, 1.00, 1), textcolour("white")
combobox bounds(365, 150,100, 20), channel("voice"), value(1), text("bass", "tenor", "countertenor", "alto", "soprano")
combobox bounds(365, 180,100, 20), channel("filter"), value(1), text("reson 1", "reson 2", "butterworth")
checkbox bounds(365, 210,100, 15), colour("yellow"), channel("balance"),  value(0), text("Balance"), fontcolour("white")

label    bounds(365, 227,100, 12), text("Input Source"), fontcolour("white")
combobox bounds(365, 240,100, 20), channel("input"), value(1), text("Live", "Noise")

rslider bounds(480,  5, 60, 60), text("BW.Mult"),   channel("BWMlt"),  range(0.01, 4, 1, 0.4), textcolour("white")
rslider bounds(480, 70, 60, 60), text("Freq.Mult"), channel("FrqMlt"), range(0.25, 4, 1, 0.4), textcolour("white")
rslider bounds(480,140, 60, 60), text("Mix"),       channel("mix"),    range(0, 1.00, 1),      textcolour("white")
rslider bounds(480,210, 60, 60), text("Gain"),      channel("gain"),   range(0, 5.00, 1, 0.5), textcolour("white")
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

;FUNCTION TABLES STORING DATA FOR VARIOUS VOICE FORMANTS
;THE FIRST VALUE OF EACH TABLE DEFINES THE NUMBER OF DATA ELEMENTS IN THE TABLE
; THIS IS NEEDED BECAUSE TABLES SIZES MUST BE POWERS OF 2 TO FACILITATE INTERPOLATED TABLE READING (tablei) 
;BASS
giBFA	ftgen	0,  0, 8, -2,	4, 600,		400,	250,	350	;FREQ
giBFE	ftgen	0,  0, 8, -2,	4, 1040,	1620,	1750,	600	;FREQ
giBFI	ftgen	0,  0, 8, -2,	4, 2250,	2400,	2600,	2400	;FREQ
giBFO	ftgen	0,  0, 8, -2,	4, 2450,	2800,	3050,	2675	;FREQ
giBFU	ftgen	0,  0, 8, -2,	4, 2750,	3100,	3340,	2950	;FREQ

giBDbA	ftgen	0, 0, 8, -2,	4, 0,	0,	0,	0	;dB
giBDbE	ftgen	0, 0, 8, -2,	4, -7,	-12,	-30,	-20	;dB
giBDbI	ftgen	0, 0, 8, -2,	4, -9,	-9,	-16,	-32	;dB
giBDbO	ftgen	0, 0, 8, -2,	4, -9,	-12,	-22,	-28	;dB
giBDbU	ftgen	0, 0, 8, -2,	4, -20,	-18,	-28,	-36	;dB

giBBWA	ftgen	0, 0, 8, -2,	4, 60,	40,	60,	40	;BAND WIDTH
giBBWE	ftgen	0, 0, 8, -2,	4, 70,	80,	90,	80	;BAND WIDTH
giBBWI	ftgen	0, 0, 8, -2,	4, 110,	100,	100,	100	;BAND WIDTH
giBBWO	ftgen	0, 0, 8, -2,	4, 120,	120,	120,	120	;BAND WIDTH
giBBWU	ftgen	0, 0, 8, -2,	4, 130,	120,	120,	120	;BAND WIDTH
;TENOR
giTFA	ftgen	0, 0, 8, -2,	5, 650, 	400,	290,	400,	350	;FREQ
giTFE	ftgen	0, 0, 8, -2,	5, 1080, 	1700,   1870,	800,	600	;FREQ
giTFI	ftgen	0, 0, 8, -2,	5, 2650,	2600,   2800,	2600,	2700	;FREQ
giTFO	ftgen	0, 0, 8, -2,	5, 2900,	3200,   3250,	2800,	2900	;FREQ
giTFU	ftgen	0, 0, 8, -2,	5, 3250,	3580,   3540,	3000,	3300	;FREQ

giTDbA	ftgen	0, 0, 8, -2,	5, 0,	0,	0,	0,	0	;dB
giTDbE	ftgen	0, 0, 8, -2,	5, -6,	-14,	-15,	-10,	-20	;dB
giTDbI	ftgen	0, 0, 8, -2,	5, -7,	-12,	-18,	-12,	-17	;dB
giTDbO	ftgen	0, 0, 8, -2,	5, -8,	-14,	-20,	-12,	-14	;dB
giTDbU	ftgen	0, 0, 8, -2,	5, -22,	-20,	-30,	-26,	-26	;dB

giTBWA	ftgen	0, 0, 8, -2,	5, 80,	70,	40,	40,	40	;BAND WIDTH
giTBWE	ftgen	0, 0, 8, -2,	5, 90,	80,	90,	80,	60	;BAND WIDTH
giTBWI	ftgen	0, 0, 8, -2,	5, 120,	100,	100,	100,	100	;BAND WIDTH
giTBWO	ftgen	0, 0, 8, -2,	5, 130,	120,	120,	120,	120	;BAND WIDTH                                         
giTBWU	ftgen	0, 0, 8, -2,	5, 140,	120,	120,	120,	120	;BAND WIDTH
;COUNTER TENOR
giCTFA	ftgen	0, 0, 8, -2,	5, 660,		440,	270,	430,	370	;FREQ
giCTFE	ftgen	0, 0, 8, -2,	5, 1120,	1800,	1850,	820,	630	;FREQ
giCTFI	ftgen	0, 0, 8, -2,	5, 2750,	2700,	2900,	2700,	2750	;FREQ
giCTFO	ftgen	0, 0, 8, -2,	5, 3000,	3000,	3350,	3000,	3000	;FREQ
giCTFU	ftgen	0, 0, 8, -2,	5, 3350,	3300,	3590,	3300,	3400	;FREQ

giTBDbA	ftgen	0, 0, 8, -2,	5, 0,	0,	0,	0,	0	;dB
giTBDbE	ftgen	0, 0, 8, -2,	5, -6,	-14,	-24,	-10,	-20	;dB
giTBDbI	ftgen	0, 0, 8, -2,	5, -23,	-18,	-24,	-26,	-23	;dB
giTBDbO	ftgen	0, 0, 8, -2,	5, -24,	-20,	-36,	-22,	-30	;dB
giTBDbU	ftgen	0, 0, 8, -2,	5, -38,	-20,	-36,	-34,	-30	;dB

giTBWA	ftgen	0, 0, 8, -2,	5, 80,	70,	40,	40,	40	;BAND WIDTH
giTBWE	ftgen	0, 0, 8, -2,	5, 90,	80,	90,	80,	60	;BAND WIDTH
giTBWI	ftgen	0, 0, 8, -2,	5, 120,	100,	100,	100,	100	;BAND WIDTH
giTBWO	ftgen	0, 0, 8, -2,	5, 130,	120,	120,	120,	120	;BAND WIDTH
giTBWU	ftgen	0, 0, 8, -2,	5, 140,	120,	120,	120,	120	;BAND WIDTH
;ALTO
giAFA	ftgen	0, 0, 8, -2,	5, 800,		400,	350,	450,	325	;FREQ
giAFE	ftgen	0, 0, 8, -2,	5, 1150,	1600,	1700,	800,	700	;FREQ
giAFI	ftgen	0, 0, 8, -2,	5, 2800,	2700,	2700,	2830,	2530	;FREQ
giAFO	ftgen	0, 0, 8, -2,	5, 3500,	3300,	3700,	3500,	2500	;FREQ
giAFU	ftgen	0, 0, 8, -2,	5, 4950,	4950,	4950,	4950,	4950	;FREQ

giADbA	ftgen	0, 0, 8, -2,	5, 0,	0,	0,	0,	0	;dB
giADbE	ftgen	0, 0, 8, -2,	5, -4,	-24,	-20,	-9,	-12	;dB
giADbI	ftgen	0, 0, 8, -2,	5, -20,	-30,	-30,	-16,	-30	;dB
giADbO	ftgen	0, 0, 8, -2,	5, -36,	-35,	-36,	-28,	-40	;dB
giADbU	ftgen	0, 0, 8, -2,	5, -60,	-60,	-60,	-55,	-64	;dB

giABWA	ftgen	0, 0, 8, -2,	5, 50,	60,	50,	70,	50	;BAND WIDTH
giABWE	ftgen	0, 0, 8, -2,	5, 60,	80,	100,	80,	60	;BAND WIDTH
giABWI	ftgen	0, 0, 8, -2,	5, 170,	120,	120,	100,	170	;BAND WIDTH
giABWO	ftgen	0, 0, 8, -2,	5, 180,	150,	150,	130,	180	;BAND WIDTH
giABWU	ftgen	0, 0, 8, -2,	5, 200,	200,	200,	135,	200	;BAND WIDTH
;SOPRANO
giSFA	ftgen	0, 0, 8, -2,	5, 800,		350,	270,	450,	325	;FREQ
giSFE	ftgen	0, 0, 8, -2,	5, 1150,	2000,	2140,	800,	700	;FREQ
giSFI	ftgen	0, 0, 8, -2,	5, 2900,	2800,	2950,	2830,	2700	;FREQ
giSFO	ftgen	0, 0, 8, -2,	5, 3900,	3600,	3900,	3800,	3800	;FREQ
giSFU	ftgen	0, 0, 8, -2,	5, 4950,	4950,	4950,	4950,	4950	;FREQ

giSDbA	ftgen	0, 0, 8, -2,	5, 0,	0,	0,	0,	0	;dB
giSDbE	ftgen	0, 0, 8, -2,	5, -6,	-20,	-12,	-11,	-16	;dB
giSDbI	ftgen	0, 0, 8, -2,	5, -32,	-15,	-26,	-22,	-35	;dB
giSDbO	ftgen	0, 0, 8, -2,	5, -20,	-40,	-26,	-22,	-40	;dB
giSDbU	ftgen	0, 0, 8, -2,	5, -50,	-56,	-44,	-50,	-60	;dB

giSBWA	ftgen	0, 0, 8, -2,	5, 80,	60,	60,	70,	50	;BAND WIDTH
giSBWE	ftgen	0, 0, 8, -2,	5, 90,	90,	90,	80,	60	;BAND WIDTH
giSBWI	ftgen	0, 0, 8, -2,	5, 120,	100,	100,	100,	170	;BAND WIDTH
giSBWO	ftgen	0, 0, 8, -2,	5, 130,	150,	120,	130,	180	;BAND WIDTH
giSBWU	ftgen	0, 0, 8, -2,	5, 140,	200,	120,	135,	200	;BAND WIDTH

instr	1
	gkx		chnget	"x"
	gky		chnget	"y"
	gkf1		chnget	"f1"
	gkf2		chnget	"f2"
	gkf3		chnget	"f3"
	gkf4		chnget	"f4"
	gkf5		chnget	"f5"
	gkvoice		chnget	"voice"
	gkvoice		init	1
	gkBWMlt		chnget	"BWMlt"
	gkFrqMlt	chnget	"FrqMlt"
	gkmix		chnget	"mix"
	gkgain		chnget	"gain"
	gkfilter	chnget	"filter"
	gkbalance	chnget	"balance"
	gkinput		chnget	"input"
	gkinput		init	1
endin

instr	2	
	kporttime	linseg	0,0.001,0.1                                                     
	
	if gkinput==1 then
	 asigL,asigR	ins
	else
	 asigL	pinkish	1
	 asigR	pinkish	1
	endif
		
	kx	portk	gkx,kporttime
	ky	portk	gky,kporttime	
	
	kSwitch		changed	gkvoice	;GENERATE A MOMENTARY '1' PULSE IN OUTPUT 'kSwitch' IF ANY OF THE SCANNED INPUT VARIABLES CHANGE. (OUTPUT 'kSwitch' IS NORMALLY ZERO)
	if	kSwitch=1	then		;IF I-RATE VARIABLE CHANGE TRIGGER IS '1'...
		reinit	START			;BEGIN A REINITIALISATION PASS FROM LABEL 'START'
	endif
	START:		
	;A TEXT MACRO IS DEFINED THAT WILL BE THE CODE FOR DERIVING DATA FOR EACH FORMANT. A MACRO IS USED TO AVOID HAVING TO USING CODE REPETITION AND TO EASIER FACICLITATE CODE MODIFICATION
#define	FORMANT_DATA(N)	
	#
	invals		table		0, giBFA+((i(gkvoice)-1)*15)+$N-1					;NUMBER OF DATA ELEMENTS IN EACH TABLE
	invals	=	invals-1									;
	kfreq$N._U	tablei		1+(kx*(3/5)*invals),giBFA+((i(gkvoice)-1)*15)+$N-1			;READ DATA FOR FREQUENCY (UPPER EDGE OF PANEL)
	kfreq$N._L	tablei		1+(((1-kx)*(1/5))+(4/5)*invals),giBFA+((i(gkvoice)-1)*15)+$N-1	;READ DATA FOR FREQUENCY (LOWER EDGE OF PANEL)
	kfreq$N		ntrpol		kfreq$N._L, kfreq$N._U, ky					;INTERPOLATE BETWEEN UPPER VALUE AND LOWER VALUE (DETERMINED BY Y-LOCATION ON PANEL)                          
	kfreq$N		=		kfreq$N * gkFrqMlt						;MULTIPLY FREQUENCY VALUE BY VALUE FROM 'Frequency Multiply' SLIDER
	kdbamp$N._U	tablei		1+(kx*(3/5)*invals),giBDbA+((i(gkvoice)-1)*15)+$N-1		;READ DATA FOR INTENSITY (UPPER EDGE OF PANEL)                                      
	kdbamp$N._L	tablei		1+(((1-kx)*(1/5))+(4/5)*invals),giBDbA+((i(gkvoice)-1)*15)+$N-1	;READ DATA FOR INTENSITY (LOWER EDGE OF PANEL)                                      
	kdbamp$N	ntrpol		kdbamp$N._L, kdbamp$N._U, ky                   			;INTERPOLATE BETWEEN UPPER VALUE AND LOWER VALUE (DETERMINED BY Y-LOCATION ON PANEL)
	kbw$N._U	tablei		1+(kx*(3/5)*invals),giBBWA+((i(gkvoice)-1)*15)+$N-1		;READ DATA FOR BANDWIDTH (UPPER EDGE OF PANEL)                                      
	kbw$N._L	tablei		1+(((1-kx)*(1/5))+(4/5)*invals),giBBWA+((i(gkvoice)-1)*15)+$N-1	;READ DATA FOR BANDWIDTH (LOWER EDGE OF PANEL)                                      
	kbw$N		ntrpol		kbw$N._L, kbw$N._U, ky						;INTERPOLATE BETWEEN UPPER VALUE AND LOWER VALUE (DETERMINED BY Y-LOCATION ON PANEL)
	kbw$N		=		kbw$N * gkBWMlt							;MULTIPLY BANDWIDTH VALUE BY VALUE FROM 'Bandwidth Multiply' SLIDER
	#												;END OF MACRO!

;READING DATA FOR FORMANTS (MACROS IMPLEMENTED)
	$FORMANT_DATA(1)
	$FORMANT_DATA(2)
	$FORMANT_DATA(3)
	$FORMANT_DATA(4)
	$FORMANT_DATA(5)

	rireturn	;RETURN FROM REINITIALISATION PASS TO PERFORMANCE TIME PASSES
	
	if gkfilter==1 then
	 aBPF1L	reson	asigL, kfreq1, kbw1, 1			;FORMANT 1
	 aBPF1R	reson	asigR, kfreq1, kbw1, 1			;FORMANT 1
	 ;                                                        
	 aBPF2L	reson	asigL, kfreq2, kbw2, 1			;FORMANT 2
	 aBPF2R	reson	asigR, kfreq2, kbw2, 1			;FORMANT 2
	 ;                                                       
	 aBPF3L	reson	asigL, kfreq3, kbw3, 1			;FORMANT 3
	 aBPF3R	reson	asigR, kfreq3, kbw3, 1			;FORMANT 3
	 ;                                                        
	 aBPF4L	reson	asigL, kfreq4, kbw4, 1			;FORMANT 4
	 aBPF4R	reson	asigR, kfreq4, kbw4, 1			;FORMANT 4
	 ;                                                       
	 aBPF5L	reson	asigL, kfreq5, kbw5, 1			;FORMANT 5
	 aBPF5R	reson	asigR, kfreq5, kbw5, 1			;FORMANT 5
	elseif gkfilter==2 then
	 aBPF1L	reson	asigL, kfreq1, kbw1, 2			;FORMANT 1
	 aBPF1R	reson	asigR, kfreq1, kbw1, 2			;FORMANT 1
	 ;                                                        
	 aBPF2L	reson	asigL, kfreq2, kbw2, 2			;FORMANT 2
	 aBPF2R	reson	asigR, kfreq2, kbw2, 2			;FORMANT 2
	 ;                                                       
	 aBPF3L	reson	asigL, kfreq3, kbw3, 2			;FORMANT 3
	 aBPF3R	reson	asigR, kfreq3, kbw3, 2			;FORMANT 3
	 ;                                                        
	 aBPF4L	reson	asigL, kfreq4, kbw4, 2			;FORMANT 4
	 aBPF4R	reson	asigR, kfreq4, kbw4, 2			;FORMANT 4
	 ;                                                       
	 aBPF5L	reson	asigL, kfreq5, kbw5, 2			;FORMANT 5
	 aBPF5R	reson	asigR, kfreq5, kbw5, 2			;FORMANT 5
	else
	 aBPF1L	butbp	asigL, kfreq1, kbw1			;FORMANT 1
	 aBPF1R	butbp	asigR, kfreq1, kbw1			;FORMANT 1
	 ;                                                        
	 aBPF2L	butbp	asigL, kfreq2, kbw2			;FORMANT 2
	 aBPF2R	butbp	asigR, kfreq2, kbw2			;FORMANT 2
	 ;                                                       
	 aBPF3L	butbp	asigL, kfreq3, kbw3			;FORMANT 3
	 aBPF3R	butbp	asigR, kfreq3, kbw3			;FORMANT 3
	 ;                                                        
	 aBPF4L	butbp	asigL, kfreq4, kbw4			;FORMANT 4
	 aBPF4R	butbp	asigR, kfreq4, kbw4			;FORMANT 4
	 ;                                                       
	 aBPF5L	butbp	asigL, kfreq5, kbw5			;FORMANT 5
	 aBPF5R	butbp	asigR, kfreq5, kbw5			;FORMANT 5
	endif	
	
	if gkbalance==1 then
	 aBPF1L	balance	aBPF1L,asigL,0.1
	 aBPF1R	balance	aBPF1R,asigR,0.1
	 aBPF2L	balance	aBPF2L,asigL,0.1
	 aBPF2R	balance	aBPF2R,asigR,0.1
	 aBPF3L	balance	aBPF3L,asigL,0.1
	 aBPF3R	balance	aBPF3R,asigR,0.1
	 aBPF4L	balance	aBPF4L,asigL,0.1
	 aBPF4R	balance	aBPF4R,asigR,0.1
	 aBPF5L	balance	aBPF5L,asigL,0.1
	 aBPF5R	balance	aBPF5R,asigR,0.1
	endif

	;FORMANTS ARE MIXED AND MULTIPLIED BOTH BY INTENSITY VALUES DERIVED FROM TABLES AND BY THE ON-SCREEN GAIN CONTROLS FOR EACH FORMANT 
	aMixL	sum	aBPF1L*(ampdbfs(kdbamp1))*gkf1, aBPF2L*(ampdbfs(kdbamp2))*gkf2, aBPF3L*(ampdbfs(kdbamp3))*gkf3, aBPF4L*(ampdbfs(kdbamp4))*gkf4, aBPF5L*(ampdbfs(kdbamp5))*gkf5
	aMixR	sum	aBPF1R*(ampdbfs(kdbamp1))*gkf1, aBPF2R*(ampdbfs(kdbamp2))*gkf2, aBPF3R*(ampdbfs(kdbamp3))*gkf3, aBPF4R*(ampdbfs(kdbamp4))*gkf4, aBPF5R*(ampdbfs(kdbamp5))*gkf5

	aOutMixL	ntrpol	asigL, aMixL*gkgain, gkmix	;MIX BETWEEN DRY AND WET SIGNALS
	aOutMixR	ntrpol	asigR, aMixR*gkgain, gkmix	;MIX BETWEEN DRY AND WET SIGNALS

		outs	aOutMixL, aOutMixR				;SEND AUDIO TO OUTPUTS
endin

instr	99			; fix cabbage bug
	kon	=	1
		chnset	kon,"f1"
endin

</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
i 2 0 [3600*24*7]
;i 99 0 0.03			; fix for cabbage bug
</CsScore>

</CsoundSynthesizer>
; HarpFilter.csd

; Creates a stack of waveguides simulating a resonating bank of strings
; ** WARNING ** 	THIS EFFECT CAN APPLY VAST AMOUNTS OF RESONATING FEEDBACK THEREFORE LARGE AMOUNTS OF GAIN ARE POSSIBLE.
;					TAKE PARTICULAR CARE WHEN USING A LIVE AUDIO INPUT.
;					IF IN DOUBT, REDUCE THE FEEDBACK VALUE.

; Tunings of strings are not controlled individually but are instead defined using several global controls.

; Frequency		-	Base frequency of the stack of waveguide filters
; Cutoff		-	Cutoff frequency of a 1st order lowpass filter within the feedback loop of each waveguide unit
; Spacing		-	The spacing method used between adjacent waveguide filters: Geometric or Arithmetic
; Interval		-	Interval factor between adjacent filters.
;				 If 'Spacing' is geometric then Interval is applied geometrically, each time multiplying it to the previous frequency to derive the next.
;				 In this mode the value Interval actually defines an interval in semitones so an interval of 12 will produce a ratio of 2
;				 e.g. if base frequency is 200 and interval is 12, the sequence is 200,400,800,1600 
;				 If 'Spacing' is 'Arithmetic' then this is applied arithmetically each time adding base_frequency to the frequency of the previous filter to derive the frequency of the next.
;				 e.g. if base frequency is 200, interval is 1, the sequence is 200,400,600,800 etc... i.e. harmonic
; Number		-	The number of waveguides to be created
; Lowest		-	The Lowest filter in the sequence. i.e. shift the stacks up in steps as this is increased.
; Reflect		-	If activated, additional waveguide filters are created at frequencies reflected beneath the base frequency according to the geometric or arithmetric rules. Activating 'Reflect' will double the number of filters used.
; Strength		-	number of series iterations of the filters (single/double/triple). Increasing numbers of iterations sharpens the filtering effect and increases the resonance.
; Filter Type	-	choose between wguide1 and streson. streson will provide better tuning but wguide1 will provide smoother results when modulating its cutoff frequency.
; Width			-	offsets the frequencies of the left and right channels to imbue added stereo width
; Random		-	range of random offsets added to waveguide frequencies
; Lowcut		-	Inserts a 24dB/oct highpass filter after each waveguide, the cutoff of which corresponds to the cutoff of that filter.
; Port.			-	Portamento time applied to changes made to frequency for each waveguide (and by extension also changes made to 'Interval'). Large portamento times are possible thereby permitting slow morphs. 
; Feedback		-	feedback ratio of each waveguide unit.
;				 negative feedback will shift the fundemental down one octave and only odd harmonics will be preset
; Attack		-	Attack time of each new note played 
; Decay			-	Decay time of the input sound once a note is released
; Release		-	Release time of the outputs of the waveguides after a note is released
; Level			-	Output amplitude control

<Cabbage>
form caption("Harp Filter") size(1080,205), pluginid("HaFi") style("legacy")
image             bounds(  0,  0,1080,125), colour("silver"), shape("sharp")

;checkbox  bounds(  7,  5, 70, 12), text("Keyboard"), channel("input"), fontcolour("black")

label     bounds(  7, 20, 80, 13), text("Freq.Input"), fontcolour("black")
combobox  bounds(  7, 34, 80, 16), text("Freq.","Note Number","Keyboard"), channel("input"), value(1)  

label     bounds(  7, 60, 80, 13), text("Audio Input"), fontcolour("black")
combobox  bounds(  7, 74, 80, 16), text("Live","Dust","P.Noise","W.Noise"), channel("InSigMode"), value(2)  

label     bounds( 92, 42, 66, 13), text("Frequency"), fontcolour("black"), identchannel("freqID0")
rslider   bounds(102, 58, 42, 42),  channel("freq"), fontcolour("black"), textcolour("black"), trackercolour("DarkSlateGrey"), range(1,5000,150,0.5,0.01), identchannel("freqID1")
nslider bounds( 93,100, 60, 17),  channel("freq"), colour("white"), fontcolour("black"), textcolour("black"), trackercolour("DarkSlateGrey"), range(1,5000,150,0.5,0.01), identchannel("freqID2")

label     bounds( 92, 42, 66, 13), text("Note Num."), fontcolour("black"), identchannel("NNID0"), visible(0)
rslider   bounds(102, 58, 42, 42),  channel("NoteNumber"), fontcolour("black"), textcolour("black"), trackercolour("DarkSlateGrey"), range(10,110,50,1,0.01), identchannel("NNID1"), visible(0)
nslider bounds( 93,100, 60, 17),  channel("NoteNumber"), colour("white"), fontcolour("black"), textcolour("black"), trackercolour("DarkSlateGrey"), range(10,110,50,1,0.01), identchannel("NNID2"), visible(0)

image    bounds(160,  5, 75,115), outlinethickness(1), line(1), outlinecolour("darkslategrey"), shape("sharp"), plant("cutoff") {
label    bounds(  5,  3, 65, 11), text("Cutoff"), fontcolour("black")
combobox bounds(  5, 15, 65, 16), text("Fixed","Ratio"), channel("CutoffMode"), value(1)  
rslider  bounds(  0, 35, 75, 75), text("Hertz"),      channel("cutoff"),      valuetextbox(1), textbox(1), fontcolour("black"), textcolour("black"), trackercolour("DarkSlateGrey"), range(20,22000,8000,0.5,1), identchannel("cutoff_ident")
rslider  bounds(  0, 35, 75, 75), text("Ratio"),      channel("CutoffRatio"), valuetextbox(1), textbox(1), fontcolour("black"), textcolour("black"), trackercolour("DarkSlateGrey"), range(1,60,21,1,1), identchannel("CutoffRatio_ident")
}

image    bounds(245,  5,395,115), outlinethickness(1), line(1), outlinecolour("darkslategrey"), shape("sharp")
label    bounds(260,  8, 80, 11), text("Spacing"), fontcolour("black")
combobox bounds(260, 20, 80, 16), text("Geometric","Arithmetic"), channel("type"), value(2)  
checkbox bounds(360, 20, 55, 12), text("Reflect"),      channel("dual"),  fontcolour("black")
label    bounds(440,  8,  80, 11), text("Strength"), fontcolour("black")
combobox bounds(440, 20, 80, 16), text("Single","Double","Triple","Quadruple"), channel("Iterations")
label    bounds(545,  8,  80, 11), text("Filter Type"), fontcolour("black")
combobox bounds(545, 20, 80, 16), text("wguide1","streson"), channel("FilterType"), value(1)
rslider   bounds(250, 40, 75, 75), text("Interval"),  channel("interval"),   valuetextbox(1), textbox(1), fontcolour("black"), textcolour("black"), trackercolour("DarkSlateGrey"), range(-12,12,0.25)
nslider bounds(260,100, 55, 16),                    channel("interval"),   range(-24,24,0.25,1,0.0001), colour("white"), fontcolour("black")
rslider  bounds(320, 40, 75, 75), text("Number"),     channel("max"),         valuetextbox(1), textbox(1), fontcolour("black"), textcolour("black"), trackercolour("DarkSlateGrey"), range(1,100,11,1,1)
rslider  bounds(380, 40, 75, 75), text("Lowest"),     channel("min"),      valuetextbox(1), textbox(1), fontcolour("black"), textcolour("black"), trackercolour("DarkSlateGrey"), range(1,100,1,1,1)
rslider  bounds(440, 41, 75, 75), text("Width"),      channel("StWidth"),   range(-0.5, 0.5, 0, 1,0.001), valuetextbox(1), textbox(1), fontcolour("black"), textcolour("black"), trackercolour("DarkSlateGrey")
rslider  bounds(500, 41, 75, 75), text("Random"),     channel("RndFactor"),   range(0, 5, 0, 0.5,0.001), valuetextbox(1), textbox(1), fontcolour("black"), textcolour("black"), trackercolour("DarkSlateGrey")
rslider  bounds(560, 41, 75, 75), text("Tune"),       channel("Tune"),       range(-100,100, 0, 1,1), valuetextbox(1), textbox(1), fontcolour("black"), textcolour("black"), trackercolour("DarkSlateGrey")

checkbox bounds(660, 20, 70, 12), text("Low Cut"),     channel("LowCut"),  fontcolour("black")
rslider  bounds(640, 40, 75, 75), text("Port."),   channel("Portamento"), valuetextbox(1), textbox(1), fontcolour("black"), textcolour("black"), trackercolour("DarkSlateGrey"), range(0.1,99,0.1,0.5,0.01)
nslider bounds(710, 55, 55, 35), text("Feedback"),  channel("feedback"), range(-0.99999,0.99999,0.99,1,0.00001), colour("white"), fontcolour("black"), textcolour("black")
rslider  bounds(760, 40, 75, 75), text("Attack"), channel("Att"),         valuetextbox(1), textbox(1), fontcolour("black"), textcolour("black"), trackercolour("DarkSlateGrey"), range(0.05,10,0.05,0.5)
rslider  bounds(820, 40, 75, 75), text("Decay"),  channel("Dec"),         valuetextbox(1), textbox(1), fontcolour("black"), textcolour("black"), trackercolour("DarkSlateGrey"), range(0.05,2,0.05,0.5)
rslider  bounds(880, 40, 75, 75), text("Release"),channel("Rel"),         valuetextbox(1), textbox(1), fontcolour("black"), textcolour("black"), trackercolour("DarkSlateGrey"), range(0.05,20,15,0.5,0.01)
rslider  bounds(940, 40, 75, 75), text("Mix"),    channel("Mix"),         valuetextbox(1), textbox(1), fontcolour("black"), textcolour("black"), trackercolour("DarkSlateGrey"), range(0,1,1,0.5,0.001)
rslider  bounds(1000, 40, 75, 75), text("Level"),  channel("amp"),        valuetextbox(1), textbox(1), fontcolour("black"), textcolour("black"), trackercolour("DarkSlateGrey"), range(0,2,0.7,0.5,0.001)

keyboard bounds(  0,125,1080, 80)

</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-dm0 -n -+rtmidi=null -M0
</CsOptions>

<CsInstruments>

sr 		= 	44100	;SAMPLE RATE
ksmps 		= 	32	;NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls 		= 	2	;NUMBER OF CHANNELS (2=STEREO)
0dbfs		=	1
		seed	0
		massign	0,2

giTriDist	ftgen	0,0,1024,21,3,1

gkFilterType	init	1

;A RECURSIVE UDO IS USED TO CREATE THE STACK OF WGUIDE1S
opcode	filterstack, a, akkkkkkkkii					;OPCODE DEFINITION
ain,kfreq,kRndFactor,kcutoff,kLowCut,kfeedback,kint,kPortTime,ktype,icount,imax	xin		;INPUT ARG NAMES
	amix	=	0
	iRnd	trirand	1
	kRnd	=	iRnd * kRndFactor
	if ktype==0 then						;IF GEOMETRIC MODE HAS BEEN CHOSEN...
	 kfreq2	=	kfreq*semitone(kint*(icount-1) + kRnd)			;DEFINE FREQUENCY FOR THIS WGUIDE1 ACCORDING TO THE BASE FREQUENCY, INTERVAL AND THE COUNTER (LOCATION IN SERIES)
	else								;OTHERWISE MUST BE ARITHMETIC MODE
	 kfreq2	=	(kfreq+(kfreq*(icount-1)*kint)) * semitone(kRnd)			;DEFINE FREQUENCY FOR THIS WGUIDE1 ACCORDING TO THE BASE FREQUENCY, INTERVAL AND THE COUNTER (LOCATION IN SERIES)
	endif								;END OF CONDITIONAL
	if abs(kfreq2)>sr/3||abs(kfreq2)<20 then			;IF FREQUENCY IS OUTSIDE OF A SENSIBLE RANGE JUMP THE CREATION OF THE WGUIDE1 ALTOGETHER
	 asig	=	0
	else
	 kramp	linseg	0,0.001,1
	 kfreq2	portk	kfreq2,kPortTime*kramp
	 if gkFilterType==1 then
	  asig	wguide1 ain, kfreq2, kcutoff, kfeedback			;CREATE THE WGUIDE1 SIGNAL
	 else
	  asig	streson	ain, kfreq2, kfeedback
;	  asig	butlp	asig,a(kcutoff)
	  asig	clfilt	asig,kcutoff,0,2
	 endif
	 if kLowCut==1 then
	  asig	buthp	asig,kfreq2
	  asig	buthp	asig,kfreq2	  
	 endif
	endif
	if icount<imax then						;IF THERE ARE STILL MORE WGUIDES TO CREATE IN THE STACK...
	  amix		filterstack	ain,kfreq,kRndFactor,kcutoff,kLowCut,kfeedback,kint,kPortTime,ktype,icount+1,imax	;CALL THE UDO AGAIN
	endif								;END OF CONDITIONAL
	skip:								;LABEL - SKIP TO HERE IF THE FREQUENCY WAS OUT OF RANGE
			xout		asig + amix			;SEND MIX OF ALL AUDIO BACK TO CALLER INSTRUMENT
endop									;END OF UDO


instr	1
	gkInSigMode		chnget	"InSigMode"				; input audio signal
	gkinput		chnget		"input"					; frequency input method (slider/keyboard)
	gkinput		init		1
	if gkInSigMode==1 then
	 gasigL,gasigR	ins
	elseif gkInSigMode==2 then						; &&gkinput!=1(temporaraily shelved) ; don't generate dust if 'keyboard' input is selected. It will be generated in instr 2.
	 gasigL	dust2	0.5,10*randomh:k(0.5,2,4)
	 gasigR	dust2	0.5,10*randomh:k(0.5,2,4)
	 gasigL	tone	gasigL,cpsoct(randomh:k(4,14,50))
	 gasigR	tone	gasigR,cpsoct(randomh:k(4,14,50))
	elseif gkInSigMode==3 then
	 gasigL		=	pinker()*0.2
	 gasigR		=	pinker()*0.2
	else
	 gasigL	noise	0.2,0
	 gasigR	noise	0.2,0
	endif
	
	kporttime	linseg		0,0.001,0.03
	
	gkfreq		chnget		"freq"
	gkNoteNumber		chnget		"NoteNumber"
	;gkfreq		portk		gkfreq,kporttime
	gkCutoffMode	chnget		"CutoffMode"
	gkcutoff	chnget		"cutoff"
	gkcutoff	portk		gkcutoff,kporttime
	gkCutoffRatio	chnget		"CutoffRatio"
	gkfeedback	chnget		"feedback"
	gkfeedback	portk		gkfeedback,kporttime
	gkinterval	chnget		"interval"
	;gkinterval	portk		gkinterval,kporttime
	gkmax		chnget		"max"
	gkmin		chnget		"min"
	ktype		chnget		"type"
	ktype		init		2
	gktype		=		ktype - 1	; COMBOBOX TO 0-1
	gkAtt		chnget		"Att"
	gkDec		chnget		"Dec"
	gkRel		chnget		"Rel"
	gkMix		chnget		"Mix"
	gkamp		chnget		"amp"
	gkPortamento	chnget		"Portamento"
	gkdual		chnget		"dual"
	gkLowCut	chnget		"LowCut"
	gkStWidth	chnget		"StWidth"
	gkRndFactor	chnget		"RndFactor"
	gkTune		chnget		"Tune"
	gkTune		*=		0.01				; CONVERT FROM CENTS TO SEMITONES
	gkLDiff		=		semitone(-gkStWidth+gkTune)
	gkRDiff		=		semitone(gkStWidth+gkTune)	
    gkIterations	chnget	"Iterations"
    gkFilterType	chnget	"FilterType"
	if changed(gkCutoffMode)==1 then
	 if gkCutoffMode==1 then
	  chnset	"visible(1)","cutoff_ident"
	  chnset	"visible(0)","CutoffRatio_ident"
	 else
	  chnset	"visible(0)","cutoff_ident"
	  chnset	"visible(1)","CutoffRatio_ident"
	 endif   
	endif

	if changed(gkinput)==1 then
	 if gkinput==1 then					; frequency
	  chnset	"visible(1)","freqID0"
	  chnset	"visible(1)","freqID1"
	  chnset	"visible(1)","freqID2"
	  chnset	"visible(0)","NNID0"
	  chnset	"visible(0)","NNID1"
	  chnset	"visible(0)","NNID2"
	 elseif gkinput==2 then				; note number
	  chnset	"visible(0)","freqID0"
	  chnset	"visible(0)","freqID1"
	  chnset	"visible(0)","freqID2"
	  chnset	"visible(1)","NNID0"
	  chnset	"visible(1)","NNID1"
	  chnset	"visible(1)","NNID2"
	 else								; keyboard
	  chnset	"visible(0)","freqID0"
	  chnset	"visible(0)","freqID1"
	  chnset	"visible(0)","freqID2"
	  chnset	"visible(0)","NNID0"
	  chnset	"visible(0)","NNID1"
	  chnset	"visible(0)","NNID2"	 
	 endif   
	endif
	
	event_i	"i",2,0.001,-1				; start instr 2 at startup

	if changed(gkinput)==1 then			; 
	 if gkinput<3 then
	  event	"i",2,0,-1
	 endif
	endif
endin
		
instr	2
	/* MIDI AND GUI INTEROPERABILITY */
	iMIDIflag	=	0			; IF MIDI ACTIVATED = 1, NON-MIDI = 0
	mididefault	1, iMIDIflag			; IF NOTE IS MIDI ACTIVATED REPLACE iMIDIflag WITH '1'

	if iMIDIflag==1 then				; IF THIS IS A MIDI ACTIVATED NOTE...
	 inum	notnum
	 ivel	veloc	0,1
	 p1	=	p1 + (rnd(1000)*0.0001)
	 if gkinput<3 then
	  turnoff
	 endif
	 icps	cpsmidi					; READ MIDI PITCH VALUES - THIS VALUE CAN BE MAPPED TO GRAIN DENSITY AND/OR PITCH DEPENDING ON THE SETTING OF THE MIDI MAPPING SWITCHES
	 kfreq	init		icps
	else
	 if gkinput==1 then						; frequency input
	  kfreq	=		gkfreq
	 elseif gkinput==2 then					; note number input
	  kfreq	=		cpsmidinn(gkNoteNumber)
	 endif
	endif						; END OF THIS CONDITIONAL BRANCH

	if trigger:k(gkinput,2.5,0)==1&&iMIDIflag==0 then		; turnoff non-midi notes if keyboard mode is selected
	 turnoff
	endif
	
	kRelease	release
	
	/* INPUT SIGNAL ENVELOPE */
	aenv		linsegr	0,i(gkAtt),1,i(gkDec),0
	
	
	; generating asig within MIDI instrument seems unreliable so is shelved in the meantime...
	/*
	;print	i(gkInSigMode)
	if i(gkInSigMode)==2&&iMIDIflag==1 then				; ensure a unique dust distribution if keyboard mode is selected
	 ;asigL	dust2	0.5,10*randomh:k(0.5,2,4)			; dust2 seems unreliable at the moment, use gausstrig instead...
	 ;asigR	dust2	0.5,10*randomh:k(0.5,2,4)
	 asigL	gausstrig 0.5, 15, 5
	 asigR	gausstrig 0.5, 15, 5
	 asigL	tone	asigL,cpsoct(randomh:k(4,14,50))
	 asigR	tone	asigR,cpsoct(randomh:k(4,14,50))
	 asigL	=		asigL*aenv
	 asigR	=		asigR*aenv
	else
	 asigL	=		gasigL * aenv
	 asigR	=		gasigR * aenv
	endif
	*/
	
	asigL	=		gasigL * aenv
	asigR	=		gasigR * aenv
	
	
	/* DERIVE LOWPASS FILETR CUTOFF DEPENDING UPON MODE SELECTION */
	if gkCutoffMode==2 then
	 kcutoff	limit	gkCutoffRatio*kfreq,20,sr/2
	else
	 kcutoff	=	gkcutoff
	endif	

	/*PORTAMENTO TIME FUNCTION*/
	kPortTime	linseg	0,0.001,1
	kPortTime	*=	gkPortamento

	kchange		changed		gkmax,gkmin,gkIterations,gkdual,gkFilterType,gkLowCut		;reiniting can also smooths interruptions and prevent very loud clicks
	if kchange==1 then					;IF NUMBER OF WGUIDE1S NEEDED OR THE START POINT IN THE SERIES HAS CHANGED...
	  reinit	update					;REINITIALISE THE STACK CREATION
	endif			
	update:							;REINIT FROM HERE
	;CALL THE UDO. (ONCE FOR EACH CHANNEL.)
	aresL		filterstack	asigL, kfreq*gkLDiff, gkRndFactor, kcutoff, gkLowCut, gkfeedback, gkinterval, kPortTime, gktype, i(gkmin), i(gkmax)+i(gkmin)-1
	aresR		filterstack	asigR, kfreq*gkRDiff, gkRndFactor, kcutoff, gkLowCut, gkfeedback, gkinterval, kPortTime, gktype, i(gkmin), i(gkmax)+i(gkmin)-1
	if i(gkIterations)>1 then
	 aresL		filterstack	aresL*0.03, kfreq*gkLDiff, gkRndFactor, kcutoff, gkLowCut, gkfeedback, gkinterval, kPortTime, gktype, i(gkmin), i(gkmax)+i(gkmin)-1
	 aresR		filterstack	aresR*0.03, kfreq*gkRDiff, gkRndFactor, kcutoff, gkLowCut, gkfeedback, gkinterval, kPortTime, gktype, i(gkmin), i(gkmax)+i(gkmin)-1
	endif
	if i(gkIterations)>2 then
	 aresL		filterstack	aresL*0.03, kfreq*gkLDiff, gkRndFactor, kcutoff, gkLowCut, gkfeedback, gkinterval, kPortTime, gktype, i(gkmin), i(gkmax)+i(gkmin)-1
	 aresR		filterstack	aresR*0.03, kfreq*gkRDiff, gkRndFactor, kcutoff, gkLowCut, gkfeedback, gkinterval, kPortTime, gktype, i(gkmin), i(gkmax)+i(gkmin)-1
	endif
	if i(gkIterations)>3 then
	 aresL		filterstack	aresL*0.03, kfreq*gkLDiff, gkRndFactor, kcutoff, gkLowCut, gkfeedback, gkinterval, kPortTime, gktype, i(gkmin), i(gkmax)+i(gkmin)-1
	 aresR		filterstack	aresR*0.03, kfreq*gkRDiff, gkRndFactor, kcutoff, gkLowCut, gkfeedback, gkinterval, kPortTime, gktype, i(gkmin), i(gkmax)+i(gkmin)-1
	endif

	if i(gkdual)==1 then	;DUAL DIRECTION WGUIDE1S SELECTED (NOTE NEGATIVE 'kinterval'
	 if i(gkmin)==1 then	;DON'T DOUBLE UP FUNDEMENTAL IF 'Lowest' IS '1'
	  imin	=	i(gkmin)+1
	  imax	=	i(gkmax)+i(gkmin)-2
	 else
	  imin	=	i(gkmin)
	  imax	=	i(gkmax)+i(gkmin)-1
	 endif
	 if gkmin==1&&gkmax==1 kgoto skip	;IF 'Num.wguides' AND 'Lowest' ARE BOTH '1', DON'T CREATE ANY REFLECTED WGUIDE1S AT ALL 	
	 aresL2	filterstack	asigL, kfreq*gkLDiff, gkRndFactor, kcutoff, gkLowCut, gkfeedback, -gkinterval, kPortTime, gktype, imin, imax
	 aresR2	filterstack	asigR, kfreq*gkRDiff, gkRndFactor, kcutoff, gkLowCut, gkfeedback, -gkinterval, kPortTime, gktype, imin, imax
	 if i(gkIterations)>1 then
	  aresL2	filterstack	aresL2*0.03, kfreq*gkLDiff, gkRndFactor, kcutoff, gkLowCut, gkfeedback, -gkinterval, kPortTime, gktype, imin, imax
	  aresR2	filterstack	aresR2*0.03, kfreq*gkRDiff, gkRndFactor, kcutoff, gkLowCut, gkfeedback, -gkinterval, kPortTime, gktype, imin, imax
	 endif
	 if i(gkIterations)>2 then
	  aresL2	filterstack	aresL2*0.03, kfreq*gkLDiff, gkRndFactor, kcutoff, gkLowCut, gkfeedback, -gkinterval, kPortTime, gktype, imin, imax
	  aresR2	filterstack	aresR2*0.03, kfreq*gkRDiff, gkRndFactor, kcutoff, gkLowCut, gkfeedback, -gkinterval, kPortTime, gktype, imin, imax
	 endif
	 if i(gkIterations)>3 then
	  aresL2	filterstack	aresL2*0.03, kfreq*gkLDiff, gkRndFactor, kcutoff, gkLowCut, gkfeedback, -gkinterval, kPortTime, gktype, imin, imax
	  aresR2	filterstack	aresR2*0.03, kfreq*gkRDiff, gkRndFactor, kcutoff, gkLowCut, gkfeedback, -gkinterval, kPortTime, gktype, imin, imax
	 endif
	 aresL	+=				aresL2
	 aresR	+=				aresR2
	 skip:
	endif
	aresL	dcblock2	aresL
	aresR	dcblock2	aresR
	rireturn						;RETURN FROM REINIT
	

	/* EXTEND RELEASE */
	kenv		linsegr	1,i(gkRel),0
	aresL		=	aresL * kenv
	aresR		=	aresR * kenv
	ktime		timeinsts
	krms		rms	aresL,3
	if krms<0.00001&&ktime>0.2&&iMIDIflag==1 then
	 turnoff2	p1,4,0
	endif

	/* WET_DRY MIX */
	aOutL	ntrpol	asigL, aresL, gkMix
	aOutR	ntrpol	asigR, aresR, gkMix
	
			outs	aOutL*gkamp, aOutR*gkamp	;SEND wguide OUTPUT TO THE AUDIO OUTPUTS AND SCALE USING THE FLTK SLIDER VARIABLE gkamp
endin

</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
</CsScore>

</CsoundSynthesizer>; HighpassFilter.csd
; Written by Iain McCurdy, 2012.

<Cabbage>
form caption("Highpass Filter"), size(435, 90), pluginid("HPFl") style("legacy")
image pos(0, 0),                size(435, 90), colour(  70,120, 90), shape("rounded"), outlinecolour("white"), outlinethickness(4) 
label    bounds(17, 22, 40, 11), text("INPUT"), fontcolour("white")
combobox bounds(10, 33, 60, 20), channel("input"), value(1), text("Live","Tone","Noise")
rslider  bounds( 75, 11, 70, 70), channel("cf"),        text("Freq."), colour(  0, 60, 30), trackercolour(200,255,200), 	textcolour("white"), 	range(20, 20000, 20, 0.333)
rslider  bounds(140, 11, 70, 70), channel("res"),       text("Res."),  colour(  0, 60, 30), trackercolour(200,255,200), 	textcolour("white"),	range(0,1.00,0)
rslider  bounds(205, 11, 70, 70), channel("mix"),       text("Mix"),   colour(  0, 60, 30), trackercolour(200,255,200),		textcolour("white"), 	range(0,1.00,1)
button  bounds(280,  10, 80, 20), text("12dB/oct","24dB/oct"), channel("steepness"), value(0)
label   bounds(280,  30, 80, 12), text("Steepness"), fontcolour("white")
checkbox bounds(280, 50, 80, 15), channel("ResType"),colour(yellow), fontcolour("white"),  value(0), text("Resonant")
rslider  bounds(360, 11, 70, 70), text("Level"),    colour(  0, 60, 30), trackercolour(200,255,200),		textcolour("white"), 	channel("level"), 	range(0, 1.00, 1)
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-d -n
</CsOptions>

<CsInstruments>

sr 	= 	48000	;SAMPLE RATE
ksmps 	= 	32	;NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls 	= 	2	;NUMBER OF CHANNELS (2=STEREO)
0dbfs	=	1
 
;Author: Iain McCurdy (2012)

instr	1
	kporttime	linseg	0,0.001,0.05
	/* READ IN WIDGETS */
	kcf			chnget	"cf"
	kres		chnget	"res"
	kmix		chnget	"mix"
	ksteepness	chnget	"steepness"
	kResType	chnget	"ResType"
	klevel		chnget	"level"
	klevel		portk	klevel,kporttime
	alevel		interp	klevel
	kcf	portk	kcf,kporttime
	acf	interp	kcf
	/* INPUT */
	kinput		chnget	"input"
	if kinput=1 then
	 aL,aR	ins
	elseif kinput=2 then
	 aL	vco2	0.2, 100
	 aR	=	aL
	else
	 aL	pinkish	0.2
	 aR	pinkish	0.2
	endif
	/* FILTER */
	if ksteepness==0&&kResType!=1 then
	 aFiltL	atone	aL,kcf
	 aFiltR	atone	aR,kcf
        elseif ksteepness==1&&kResType!=1 then
	 aFiltL	buthp	aL,acf
	 aFiltR	buthp	aR,acf
        elseif kResType==1 then
	 aFiltL	bqrez	aL,acf,1+(kres*40),1
	 aFiltR	bqrez	aR,acf,1+(kres*40),1
	endif
	aL	ntrpol	aL,aFiltL,kmix
	aR	ntrpol	aR,aFiltR,kmix
		outs	aL*alevel,aR*alevel
endin
		
</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
e
</CsScore>

</CsoundSynthesizer>
; LowpassFilter.csd
; Written by Iain McCurdy, 2012

<Cabbage>
form caption("Lowpass Filter") size(435, 90), pluginid("LPFl") style("legacy")
image    bounds(0, 0, 435, 90), colour(  70, 90,100), shape("rounded"), outlinecolour("white"), outlinethickness(4) 
label    bounds( 22, 22, 38, 11), text("INPUT:"), fontcolour("white")
combobox bounds( 10, 33, 60, 20), channel("input"), value(1), text("Live","Tone","Noise")
rslider  bounds( 75, 11, 70, 70), channel("cf"),        text("Freq."), colour(  0, 40, 50), trackercolour(200,240,250), 	textcolour("white"), 	range(20, 20000, 20000, 0.333)
rslider  bounds(140, 11, 70, 70), channel("res"),       text("Res."),  colour(  0, 40, 50), trackercolour(200,240,250), 	textcolour("white"),	range(0,1.00,0)
rslider  bounds(205, 11, 70, 70), channel("mix"),       text("Mix"),   colour(  0, 40, 50), trackercolour(200,240,250),		textcolour("white"), 	range(0,1.00,1)
label    bounds(285, 10, 70, 12), text("Steepness:"), fontcolour("white")
button   bounds(280, 23, 80, 20), channel("steepness"), text("12dB/oct","24dB/oct"), value(0)
checkbox bounds(280, 50, 80, 15), channel("ResType"), fontcolour("white"),  value(0), text("Resonant"), colour(yellow)
rslider  bounds(360, 11, 70, 70), text("Level"),                       colour(  0, 40, 50), trackercolour(200,240,250),		textcolour("white"), 		channel("level"), 	range(0, 1.00, 1)
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-d -n
</CsOptions>

<CsInstruments>

sr 		= 	48000	;SAMPLE RATE
ksmps 		= 	32	;NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls 		= 	2	;NUMBER OF CHANNELS (2=STEREO)
0dbfs		=	1

;Author: Iain McCurdy (2012)

instr	1
	kporttime	linseg	0,0.001,0.05
	kcf		chnget	"cf"				;
	kres		chnget	"res"				;
	kmix		chnget	"mix"				;
	ksteepness	chnget	"steepness"			;
	kResType	chnget	"ResType"			;
	klevel		chnget	"level"				;
	klevel		portk	klevel,kporttime
	alevel		interp	klevel
	kcf	portk	kcf,kporttime
	acf	interp	kcf
	/* INPUT */
	kinput		chnget	"input"
	if kinput=1 then
	 aL,aR	ins
	elseif kinput=2 then
	 aL	vco2	0.2, 100
	 aR	=	aL
	else
	 aL	pinkish	0.2
	 aR	pinkish	0.2
	endif
	if ksteepness==0&&kResType!=1 then
	 aFiltL	tone	aL,kcf
	 aFiltR	tone	aR,kcf
        elseif ksteepness==1&&kResType!=1 then
	 aFiltL	butlp	aL,acf
	 aFiltR	butlp	aR,acf
        elseif kResType==1 then
	 aFiltL	moogladder	aL,acf,kres
	 aFiltR	moogladder	aR,acf,kres        
	endif
	aL	ntrpol	aL,aFiltL,kmix
	aR	ntrpol	aR,aFiltR,kmix
		outs	aL*alevel,aR*alevel
endin
		
</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
</CsScore>

</CsoundSynthesizer>
; phaser1.csd
; Written by Iain McCurdy, 2012.

<Cabbage>
form caption("phaser1") size(400, 90), pluginid("phs1") style("legacy")
image        pos(0, 0), size(400, 90), colour(80,20,20), shape("rounded"), outlinecolour("white"), outlinethickness(4) 
label     bounds( 10, 15, 55, 12), text("INPUT"), fontcolour("white")
checkbox  bounds( 10, 30, 55, 12), text("Live"),  fontcolour("white"), channel("input"),  value(1), radiogroup(1)
checkbox  bounds( 10, 45, 55, 12), text("Noise"), fontcolour("white"), channel("input2"), value(0), radiogroup(1)
rslider  bounds( 60, 11, 70, 70),  text("Frequency"), channel("freq"),     range(20.0, 5000, 160, 0.25), colour(160,40,40), textcolour("white"), trackercolour(white)
rslider  bounds(125, 11, 70, 70),  text("Feedback"),  channel("feedback"), range(-0.99, 0.99, 0.9),      colour(160,40,40), textcolour("white"), trackercolour(white)
rslider  bounds(190, 11, 70, 70), text("N.Ords."),   channel("ord"),      range(1, 256, 32, 0.5,1),     colour(160,40,40), textcolour("white"), trackercolour(white)
rslider  bounds(255, 11, 70, 70), text("Mix"),       channel("mix"),      range(0, 1.00, 1),            colour(160,40,40), textcolour("white"), trackercolour(white)
rslider  bounds(320, 11, 70, 70), text("Level"),     channel("level"),    range(0, 1.00, 0.7),          colour(160,40,40), textcolour("white"), trackercolour(white)
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

instr	1
	gkfreq		chnget	"freq"					;READ WIDGETS...
	gkfeedback	chnget	"feedback"				;
	gkord		chnget	"ord"					;
	gkmix		chnget	"mix"					;
	gklevel		chnget	"level"					;

	gkinput		chnget	"input"					;
	if gkinput==1 then
	 asigL,asigR	ins
	else
	 asigL	pinker
	 asigR	pinker
	endif

	kporttime	linseg	0,0.01,0.03				;CREATE A VARIABLE THAT WILL BE USED FOR PORTAMENTO TIME
	kfreq		portk	gkfreq, kporttime			;PORTAMENTO IS APPLIED TO 'SMOOTH' SLIDER MOVEMENT	
	kSwitch		changed	gkord					;GENERATE A MOMENTARY '1' PULSE IN OUTPUT 'kSwitch' IF ANY OF THE SCANNED INPUT VARIABLES CHANGE. (OUTPUT 'kSwitch' IS NORMALLY ZERO)
	if	kSwitch=1	then					;IF I-RATE VARIABLE CHANGE TRIGGER IS '1'...
		reinit	UPDATE						;BEGIN A REINITIALISATION PASS FROM LABEL 'UPDATE'
	endif								;END OF CONDITIONAL BRANCH
	UPDATE:								;BEGIN A REINITIALISATION PASS FROM HERE
	aphaserl	phaser1		asigL, kfreq, gkord, gkfeedback	;PHASER1 IS APPLIED TO THE LEFT CHANNEL
	aphaserr	phaser1		asigR, kfreq, gkord, gkfeedback	;PHASER1 IS APPLIED TO THE RIGHT CHANNEL
	rireturn							;RETURN FROM REINITIALISATION PASS TO PERFORMANCE TIME PASSES
	amixL		ntrpol	asigL,aphaserl,gkmix
	amixR		ntrpol	asigR,aphaserr,gkmix
			outs	amixL*gklevel, amixR*gklevel		;PHASER OUTPUT ARE SENT TO THE SPEAKERS
endin
		
</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
</CsScore>


</CsoundSynthesizer>



























; phaser.csd
; Written by Iain McCurdy, 2012.

<Cabbage>
form caption("phaser2") size(685, 90), pluginid("phs2") style("legacy")
image pos(0, 0),        size(685, 90), colour(0,0,25), shape("rounded"), outlinecolour("white"), outlinethickness(4) 
label     bounds( 15, 15, 55, 12), text("INPUT"), fontcolour("white")
checkbox  bounds( 15, 30, 55, 12), text("Live"),  fontcolour("white"), channel("input"),  value(1), radiogroup(1)
checkbox  bounds( 15, 45, 55, 12), text("Noise"), fontcolour("white"), channel("input2"), value(0), radiogroup(1)
rslider  bounds( 70, 11, 70, 70),  text("Frequency"),  channel("freq"),     range(20.0, 5000, 100, 0.25), colour(100,100,200), trackercolour(silver), textcolour("white")
rslider  bounds(140, 16, 60, 60),  text("Port."),  channel("port"),     range(0, 30, 0.1, 0.5,0.01), colour(100,100,200), trackercolour(silver), textcolour("white")
rslider  bounds(195, 11, 70, 70),  text("Q"),          channel("q"),        range(0.01,10,1,0.5),            colour(100,100,200), trackercolour(silver), textcolour("white")
rslider  bounds(260, 11, 70, 70), text("N.Ords."),    channel("ord"),      range(1, 256, 8, 0.5,1),      colour(100,100,200), trackercolour(silver), textcolour("white")
label    bounds(335, 20, 61,12),  text("Sep. Mode:"), fontcolour("white")
rslider  bounds(410, 11, 70, 70), text("Separation"), channel("sep"),      range(-3, 3.00, 1),         colour(100,100,200), trackercolour(silver), textcolour("white")
rslider  bounds(475, 11, 70, 70), text("Feedback"),   channel("feedback"), range(-0.99, 0.99, 0.9),      colour(100,100,200), trackercolour(silver), textcolour("white")
rslider  bounds(540, 11, 70, 70), text("Mix"),        channel("mix"),      range(0, 1.00, 1),            colour(100,100,200), trackercolour(silver), textcolour("white")
rslider  bounds(605, 11, 70, 70), text("Level"),      channel("level"),    range(0, 1.00, 0.7),          colour(100,100,200), trackercolour(silver), textcolour("white")
combobox bounds(330, 34, 80,25), channel("mode"), value(1), text("Equal", "Power"), fontcolour("white")
}
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-d -n
</CsOptions>

<CsInstruments>

sr 		= 	48000	;SAMPLE RATE
ksmps 		= 	32	;NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls 		= 	2	;NUMBER OF CHANNELS (2=STEREO)
0dbfs		=	1

;Author: Iain McCurdy (2012)

instr	1
	gkport		chnget	"port"
	kRampUp		linseg	0,0.001,1
	kfreq		chnget	"freq"					;READ WIDGETS...
	gkfreq		portk	kfreq,kRampUp*gkport
	kq		chnget	"q"					;
	gkq		portk	kq,kRampUp*gkport
	gkmode		chnget	"mode"					;
	gkmode		init	1
	gkmode		init	i(gkmode)-1
	ksep		chnget	"sep"					;
	gksep		portk	ksep,kRampUp*gkport
	gkfeedback	chnget	"feedback"				;
	gkord		chnget	"ord"					;
	gkmix		chnget	"mix"					;
	gklevel		chnget	"level"					;
	gkinput		chnget	"input"					;
	if gkinput==1 then
	 asigL,asigR	ins
	else
	 asigL	pinker
	 asigR	pinker
	endif
	kporttime	linseg	0,0.01,0.03				;CREATE A VARIABLE THAT WILL BE USED FOR PORTAMENTO TIME
	kfreq		portk	gkfreq, kporttime			;PORTAMENTO IS APPLIED TO 'SMOOTH' SLIDER MOVEMENT	
	kq		portk	gkq, kporttime				;PORTAMENTO IS APPLIED TO 'SMOOTH' SLIDER MOVEMENT
	ksep		portk	gksep, kporttime				;PORTAMENTO IS APPLIED TO 'SMOOTH' SLIDER MOVEMENT
	kSwitch		changed	gkord,gkmode				;GENERATE A MOMENTARY '1' PULSE IN OUTPUT 'kSwitch' IF ANY OF THE SCANNED INPUT VARIABLES CHANGE. (OUTPUT 'kSwitch' IS NORMALLY ZERO)
	if	kSwitch=1	then					;IF I-RATE VARIABLE CHANGE TRIGGER IS '1'...
		reinit	UPDATE						;BEGIN A REINITIALISATION PASS FROM LABEL 'UPDATE'
	endif								;END OF CONDITIONAL BRANCH
	UPDATE:								;BEGIN A REINITIALISATION PASS FROM HERE
	aphaserl	phaser2		asigL, kfreq, kq, gkord, gkmode, ksep, gkfeedback	; PHASER2 IS APPLIED TO THE LEFT CHANNEL
	aphaserr	phaser2		asigR, kfreq, kq, gkord, gkmode, ksep, gkfeedback	; PHASER1 IS APPLIED TO THE RIGHT CHANNEL
	rireturn							;RETURN FROM REINITIALISATION PASS TO PERFORMANCE TIME PASSES

	aphaserl	dcblock2	aphaserl			;PHASER2 CAN TEND TO PRODUCE A DC OFFSET
	aphaserr	dcblock2	aphaserr
	
	amixL		ntrpol	asigL,aphaserl,gkmix
	amixR		ntrpol	asigR,aphaserr,gkmix
			outs	amixL*gklevel, amixR*gklevel		;PHASER OUTPUTS ARE SENT OUT
endin
		
</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
</CsScore>


</CsoundSynthesizer>



























; resony.csd
; Written by Iain McCurdy, 2012.

; Resony is an implementation of a stack of second-order bandpass filters whose centre frequencies are arithmetically 
; related. 
; The 'bandwidth' and 'scaling mode' parameters are as they are in the reson opcode. 

; CONTROLS
; --------
; 'Base Freq.' (base frequency) defines the centre frequency of the first filter. 

; 'Sep' (separation) normally defines the separation between the lowest and highest filter in the stack in octaves. 
; How this relates to what the actual frequencies of filters will be depends upon which separation mode has been selected. 
; This is explained below. Note that in this example the operation of 'ksep' has been modified slightly to allow the 
; opcode to be better controlled from the GUI. These modifications are clarified below. Separation can be defined in 
; octaves using the knob 'Sep.oct.' or in semitones using the 'Sep.semi.' knob.
; Making changes to 'Sep.Semi', either via the slider or the number box, will causes changes in the 'Sep.Oct' slider and 
; number box (but not vice versa).

; 'Separatation Mode' defines the the way additional reson filters are arranged according to the 'Sep' value'. In 
; 'oct.total' separation mode, the pitch interval between the base frequency and (base frequency + separation is divided 
; into equal intervals according to the number of filters that have been selected. Note that no filter is created at the 
; frequency of (base frequency + separation). For example: if separation=1 and num.filters=2, filters will be created at 
; the base frequency and a tritone above the base frequency (i.e. an interval of 1/2 and an octave). I suspect this is a 
; mistake in the opcode implementation so in this example I rescale the separation interval before passing it to the 
; resony opcode so that the interval between the lowest and highest filter in this mode will always be the interval 
; defined in the GUI. 
; If 'hertz' separation mode is selected behaviour is somewhat curious. I have made some other modifications to the 
; values passed to the opcode to make this mode more controllable. Without these modifications, if number of filters is 
; '1' no filters would be created. The frequency relationship between filters in the stack always follows the harmonic 
; series. Both 'Base Frequency' and 'Separation' normally shift this harmonic stack of filters up or down, for this reason 
; I have disabled user control of 'Separation' in this mode, instead a value equal to the 'Number of Filters' will always 
; be used for 'Separation'. This ensures that a harmonic stack will always be created built upon 'Base Frequency' as the 
; fundamental. Negative values for 'separation' are allowed whenever 'separation mode' is 'octaves' (if this is the case, 
; the stack of frequencies will extend below the base frequency). Negative values for 'separation' when 'separation mode' 
; is 'hertz' will cause filters to 'explode'. As 'Separation' is fixed at 'Number of Filters' in this example this 
; explosion will not occur.

; A third option I have provided allows the defined interval to be the interval between adjacent filters rather than the 
; interval from lowest to highest. In this mode it is probably wise to keep the separation value lowish, or reduce the 
; number of filters (or both), otherwise filters with very high frequencies might be being requested. This could potentially
; lead to some rather unpleasant distortion. In actuality there is some protection against this (discussed later on).

; 'B.width'	-	bandwidth of the filters (in hertz).

; 'Num.' (number of filters) defines the number of filters in the stack. 

; 'Scaling Mode'	-	provides options for scaling the amplitude of the filters. If 'none' is chosen output
; 				amplitude can increase greatly, particularly if bandwidth is narrow (a low value).
;				

; In addition a lowpass and highpass filter have been added after the resony filter
;
;         +--------+   +----------+   +---------+
; INPUT---+ RESONY +---+ HIGHPASS +---+ LOWPASS +---OUTPUT
;         +--------+   +----------+   +---------+
;
; The cutoff frequencies of each of these filters are defined as ratios relative to the base frequency. 
;
; Unpleasant sounds can result if reson filters are given too high cutoff frequencies. This can occur through a combination
; of the settings for 'BF' (base frequency), 'Num.' (number of filters), 'Sep.' (separation) and 'Separation Mode'. This 
; instrument features a safety measure whereby if this is going to happen the 'Num.' value will be reduced. If you notice
; the 'Num.' slider moving by itself while you are making adjustments to other controls this is the reason. This safety 
; mechanism is most likely to cut when 'Octs Adjacent' mode is selected. 


<Cabbage>
form caption("resony") size(750,180), pluginid("rsny"), colour(255,100,0) style("legacy")
image        bounds(  0,  0,750,180), colour("black"), shape("rounded"), outlinecolour(255,100,0), outlinethickness(2) 
label     bounds( 10, 20, 80, 12), text("INPUT:")
button    bounds( 10, 35, 80, 35), text("Live","Noise"),     channel("input"), value(0)
rslider   bounds( 90, 10, 70, 70), text("Base Freq"),       fontcolour("white"), channel("bf"),   range(20, 20000, 909, 0.5), colour(255,100,0,255), trackercolour(255,250,100)
nslider bounds(160, 15, 70, 40), text("Base Freq"),        channel("bf"),   range(20, 20000, 909, 0.5)
rslider   bounds(230, 10, 70, 70), text("B.width"),         fontcolour("white"), channel("bw"),   range(0.01, 500, 13, 0.375, 0.0001), colour(255,100,0,255), trackercolour(255,250,100)
rslider   bounds(300, 10, 70, 70), text("Num."),            fontcolour("white"), channel("num"),  range(1, 80, 10, 1,1),      colour(255,100,0,255), trackercolour(255,250,100)
rslider   bounds(370, 10, 70, 70), text("Sep.oct."),        fontcolour("white"), channel("sep"),  range(-11, 11, 2,1,0.001),          colour(255,100,0,255), trackercolour(255,250,100)
nslider bounds(440, 15, 70, 40), text("Sep.oct"),          channel("sep"),  range(-11, 11, 2,1,0.001)
rslider   bounds(510, 10, 70, 70), text("Sep.semi."),       fontcolour("white"), channel("sep2"), range(-48, 48, 24,1,1),     colour(255,100,0,255), trackercolour(255,250,100)
nslider bounds(580, 15, 70, 40), text("Sep.semi."),        channel("sep2"),  range(-48, 48, 24,1,0.001)

rslider   bounds(660, 10, 70, 70), text("Level"),           fontcolour("white"), channel("gain"), range(0,2,1,0.25,0.00001),  colour(255,100,0,255), trackercolour(255,250,100)

label     bounds(450,100,130, 13), text("Separation Mode")
combobox  bounds(450,115,130, 25), channel("sepmode"), value(1), text("octs.total", "hertz", "octs.adjacent")
label     bounds(600,100,130, 13), text("Scaling Mode")
combobox  bounds(600,115,130, 25), channel("scl"), value(2), text("none", "peak response", "RMS")


image    bounds( 30, 90,180, 80), colour(0,0,0,0), outlinecolour(150,150,150), outlinethickness(1), plant("highpass"), {
checkbox bounds( 20, 15,100, 20), text("Highpass"), channel("HPF_OnOff")
rslider  bounds(100,  5, 70, 70), text("Ratio"),           fontcolour("white"), channel("HPF_Ratio"), range(0.1, 16, 0.1, 0.5,0.0001),  colour(255,100,0,255), trackercolour(255,250,100)
}

image    bounds(240, 90,180, 80), colour(0,0,0,0), outlinecolour(150,150,150), outlinethickness(1), plant("lowpass"), {
checkbox bounds( 20, 15,100, 20), text("Lowpass"), channel("LPF_OnOff")
rslider  bounds(100,  5, 70, 70), text("Ratio"),           fontcolour("white"), channel("LPF_Ratio"), range(0.1, 32, 32, 0.25,0.00001),  colour(255,100,0,255), trackercolour(255,250,100)
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

;A UDO IS CREATED WHICH ENCAPSULATES THE MODIFICATIONS TO THE resony OPCODE DISCUSSED IN THIS EXAMPLE 
opcode	resony2,a,akkikii
	ain, kbf, kbw, inum, ksep , isepmode, iscl	xin

	;IF 'Octaves (Total)' MODE SELECTED...
	if isepmode==0 then
	 irescale	divz	inum,inum-1,1	;PREVENT ERROR IF NUMBER OF FILTERS = ZERO
	 ksep	=	ksep * irescale		;RESCALE SEPARATION
	  
	;IF 'Hertz' MODE SELECTED...	
	elseif isepmode==1 then
	 inum	=	inum + 1		;AMEND QUIRK WHEREBY NUMBER RESONANCES PRODUCED IN THIS MODE WOULD ACTUALLY BE 1 FEWER THAN THE VALUE DEMANDED
	 ksep	=	inum			;ksep IS NOT ESSESNTIAL IN THIS MODE, IT MERELY DOUBLES AS A BASE FREQUENCY CONTROL. THEREFORE SETTING IT TO NUMBER OF BANDS ENSURES THAT BASE FREQUENCY WILL ALWAYS BE DEFINED ACCURATELY BY kbf VALUE
			 
	;IF 'Octaves (Adjacent)' MODE SELECTED...
	elseif isepmode==2 then 
	 irescale	divz	inum,inum-1,1	;PREVENT ERROR IF NUMBER OF FILTERS = ZERO
	 ksep = ksep * irescale			;RESCALE SEPARATION
	 ksep = ksep * (inum-1)			;RESCALE SEPARATION INTERVAL ACCORDING TO THE NUMBER OF FILTERS CHOSEN
	 isepmode	=	0		;ESSENTIALLY WE ARE STILL USING MODE:0, JUST WITH THE ksep RESCALING OF THE PREVIOUS LINE ADDED	 

	endif
	
	aout 		resony 	ain, kbf, kbw, inum, ksep , isepmode, iscl
			xout	aout
endop


instr	1	
	kporttime	linseg	0,0.001,0.005,1,0.05	;CREATE A VARIABLE FUNCTION THAT RAPIDLY RAMPS UP TO A SET VALUE	
	
	loop:
	gkbf	chnget	"bf"
	gkbw	chnget	"bw"
	gkgain	chnget	"gain"
	gknum	chnget	"num"
	gksep	chnget	"sep"				; octaves
	gksepmode	chnget	"sepmode"
	gksepmode	=	gksepmode - 1
	gksepmode	init	1
	gkscl	chnget	"scl"
	gkscl	=	gkscl - 1
	gkscl	init	1
	gkinput		chnget	"input"
	gkHPF_OnOff	chnget	"HPF_OnOff"
	kHPF_Ratio	chnget	"HPF_Ratio"
	gkLPF_OnOff	chnget	"LPF_OnOff"
	kLPF_Ratio	chnget	"LPF_Ratio"
	
	kbf		portk	gkbf, kporttime		; SMOOTH MOVEMENT OF SLIDER VARIABLES
	ksep		portk	gksep, kporttime
	kLPF_Ratio	portk	kLPF_Ratio, kporttime
	kHPF_Ratio	portk	kHPF_Ratio, kporttime

	if gkinput==0 then
	 asigL,asigR	ins
	else
	 asigL	pinkish	1
	 asigR	pinkish	1
	endif

	; DISCERN FREQUENCY OF HIGHEST RESON DEPENDING ON SEPARATION MODE CHOSEN
	if gksepmode==0 then
	 kmax	=	gknum==1 ? kbf : kbf * abs(ksep) * 2
	elseif gksepmode==1 then
	 kmax	=	kbf * gknum
	else
	 kmax	=	gknum==1 ? kbf : kbf * (gknum-1) * abs(ksep) * 2
	endif

	; TEST HIGHEST RESON FREQUENCY. 
	; IF IT IS TOO HIGH, UNPLEASANT NOISE WILL BE PRODUCED SO REDUCE THE NUMBER OF RESONS THEN LOOP BACK (AND TEST AGAIN)
	if kmax>(sr/5) then	; things seems to start to sound unpleasant if resons are placed at frequencies sr/5 or higher	
	 chnset	gknum-1,"num"	; decrement 'Num.' slider...
	 kgoto	loop		; and loop back (in order to test again)
	endif


	kSwitch		changed	gkscl, gknum, gksepmode		;GENERATE A MOMENTARY '1' PULSE IN OUTPUT 'kSwitch' IF ANY OF THE SCANNED INPUT VARIABLES CHANGE. (OUTPUT 'kSwitch' IS NORMALLY ZERO)
	if	kSwitch=1	then		;IF I-RATE VARIABLE CHANGE TRIGGER IS '1'...
		reinit	START			;BEGIN A REINITIALISATION PASS FROM LABEL 'START'
	endif
	START:
	
	isepmode	init	i(gksepmode)
	inum		init	i(gknum)	
	iscl		init	i(gkscl)
	
	;CALL resony2 UDO
	aresL 		resony2 asigL, kbf, gkbw, inum, ksep , isepmode, iscl
	aresR 		resony2	asigR, kbf, gkbw, inum, ksep , isepmode, iscl
		
	rireturn	;RETURN FROM REINITIALISATION PASS TO PERFORMANCE TIME PASSES

	if gkHPF_OnOff==1 then
	 aresL	buthp	aresL, kbf*kHPF_Ratio
	 aresR	buthp	aresR, kbf*kHPF_Ratio
	endif

	if gkLPF_OnOff==1 then
	 kcf	limit	kbf*kLPF_Ratio, 0, sr/2
	 aresL	butlp	aresL, kcf
	 aresR	butlp	aresR, kcf
	endif

			outs	aresL * gkgain, aresR * gkgain	;SEND FILTER OUTPUT TO THE AUDIO OUTPUTS AND SCALE USING THE FLTK SLIDER VARIABLE gkgain
endin

instr	UpdateWidgets
	ksep2	chnget	"sep2"
	ktrig2	changed	ksep2
	if  ktrig2==1 then
	 chnset	ksep2/12, "sep"
	endif
endin

</CsInstruments>

<CsScore>
i 1 0.1 [3600*24*7]
i "UpdateWidgets" 0 3600	;UPDATE SEPARATION DISPLAY BOX
</CsScore>

</CsoundSynthesizer>
; Vocoder.csd
; Written by Iain McCurdy, 2012

<Cabbage>
form caption("vocoder"), size(530, 230), pluginid("voco") style("legacy")

image pos(0, 0), size(530, 200), colour(255,255,150), shape("rounded"), outlinecolour("white"), line(1)

image 	bounds(140,  6,180, 31), colour( 10,225, 90,100), shape("ellipse"), outlinecolour(205,205, 70), line(1)
label   bounds(158,  5, 33, 33), text("v"), FontColour(100,100,100)
label   bounds(178,  5, 33, 33), text("o"), FontColour(100,100,100)
label   bounds(198,  5, 33, 33), text("c"), FontColour(100,100,100)
label   bounds(218,  5, 33, 33), text("o"), FontColour(100,100,100)
label   bounds(238,  5, 33, 33), text("d"), FontColour(100,100,100)
label   bounds(258,  5, 33, 33), text("e"), FontColour(100,100,100)
label   bounds(278,  5, 33, 33), text("r"), FontColour(100,100,100)
label   bounds(161,  4, 30, 30), text("v"), FontColour(255,  0,  0)
label   bounds(181,  4, 30, 30), text("o"), FontColour(235,  0, 20)
label   bounds(201,  4, 30, 30), text("c"), FontColour(215,  0, 40)
label   bounds(221,  4, 30, 30), text("o"), FontColour(195,  0, 60)
label   bounds(241,  4, 30, 30), text("d"), FontColour(175,  0, 80)
label   bounds(261,  4, 30, 30), text("e"), FontColour(155,  0,100)
label   bounds(281,  4, 30, 30), text("r"), FontColour(135,  0,120)

label    bounds( 12,  9, 75, 11), text("Carrier Source"), FontColour("black")
image 	 bounds(  9, 20, 87, 18), colour("black"), shape("sharp")
combobox bounds( 10, 21, 85, 16), channel("CarSource"), value(1), text("synth","external"), fontcolour(black), colour(255,255,200)
label    bounds( 10, 39, 85, 11), text("Filter Steepness"), FontColour("black")
image 	 bounds(  9, 50, 87, 18), colour("black"), shape("sharp")
combobox bounds( 10, 51, 85, 16), channel("steepness"), value(2), text("12dB/oct","24dB/oct"), fontcolour(black), colour(255,255,200)
label    bounds( 20, 69, 60, 11), text("Synth Type"), FontColour("black")
image 	 bounds(  9, 80, 87, 18), colour("black"), shape("sharp")
combobox bounds( 10, 81, 85, 16), channel("SynType"), value(1), text("saw","square","pulse","noise"), fontcolour(black), colour(255,255,200)

rslider bounds(105, 38, 70, 70), text("Base"),    colour( 55,105,50), textcolour("Black"), channel("base"),  range(24, 80, 40,1,1),  trackercolour(150,255,150)
rslider bounds(165, 38, 70, 70), text("Num."),    colour( 55,105,50), textcolour("Black"), channel("num"),   range(1, 100, 16,1,1),  trackercolour(150,255,150)
rslider bounds(225, 38, 70, 70), text("B.width"), colour( 55,105,50), textcolour("Black"), channel("bw"),    range(0.01, 1, 0.1,0.5),  trackercolour(150,255,150)
rslider bounds(285, 38, 70, 70), text("Spacing"), colour( 55,105,50), textcolour("Black"), channel("incr"),  range(1, 12, 5,0.5),  trackercolour(150,255,150)
rslider bounds(445, 38, 70, 70), text("Level"),   colour( 55,105,50), textcolour("Black"), channel("level"), range(0, 5.00, 1,0.5),  trackercolour(150,255,150)

vslider bounds(350, 10, 50, 90), colour( 55,105,50), textcolour("Black"), channel("BPGain"), range(0, 1, 0.6, 0.5),  trackercolour(150,255,150)
vslider bounds(400, 10, 50, 90), colour( 55,105,50), textcolour("Black"), channel("HPGain"), range(0, 1, 0.25, 0.5), trackercolour(150,255,150)
label   bounds(365, 95, 25, 12), text("BPF"), FontColour("black")
label   bounds(415, 95, 25, 12), text("HPF"), FontColour("black")

checkbox bounds(440, 10, 80, 13), channel("gate"), text("Gate Input"), value(0), shape("square"), fontcolour("black")

keyboard bounds(10, 112, 510,80)
infobutton bounds(5,205, 100, 20), text("Help"), file("VocoderHelp.html")
image bounds(105, 205, 320, 20), colour(75, 85, 90, 50), plant("credit"){
label bounds(0.03, 0.1, .6, .7), text("Author: Iain McCurdy |2012|"), FontColour("LightGreen")
}

</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-dm0 -n -+rtmidi=null -M0
</CsOptions>

<CsInstruments>

sr 		= 	44100
ksmps 		= 	64
nchnls 		= 	2
0dbfs		=	1	;MAXIMUM AMPLITUDE
massign	0,2

gisine	ftgen	0,0,4096,10,1
gaSyn	init	0

opcode VocoderChannel, a, aakiiiii				;MODE UDO 
	aMod,aCar,ksteepness,ibase,ibw,iincr,icount,inum  xin	;NAME INPUT VARIABLES
	icf	=	cpsmidinn(ibase+(icount*iincr))		;DERIVE FREQUENCY FOR *THIS* BANDPASS FILTER BASED ON BASE FREQUENCY AND FILTER NUMBER (icount)
	icount	=	icount + 1				;INCREMENT COUNTER IN PREPARTION FOR NEXT FILTER
	
	if	icf>15000 goto SKIP				;IF FILTER FREQUENCY EXCEEDS A SENSIBLE LIMIT SKIP THE CREATION OF THIS FILTER AND END RECURSION
	
	aModF	butbp	aMod,icf,ibw*icf			;BANDPASS FILTER MODULATOR
	
	if ksteepness=2 then					;IF 24DB PER OCT MODE IS CHOSEN...
	  aModF	butbp	aModF,icf,ibw*icf			;...BANDPASS FILTER AGAIN TO SHARPEN CUTOFF SLOPES
	endif							;END OF THIS CONDITIONAL BRANCH
	aEnv 	follow2	aModF, 0.05, 0.05			;FOLLOW THE ENVELOPE OF THE FILTERED AUDIO

	aCarF	butbp	aCar,icf,ibw*icf			;BANDPASS FILTER CARRIER
	if ksteepness=2 then					;IF 24 DB PER OCT IS CHOSEN...
	  aCarF	butbp	aCarF,icf,ibw*icf			;...BANDPASS FILTER AGAIN TO SHARPEN CUTOFF SLOPES
	endif							;END OF THIS CONDITIONAL BRANCH

	amix	init	0					;INITIALISE MIX VARIABLE CONTAINING ALL SUBSEQUENT BANDS
	
	if	icount < inum	then					;IF MORE FILTERS STILL NEED TO BE CREATED...
		amix	VocoderChannel	aMod,aCar,ksteepness,ibase,ibw,iincr,icount,inum	;...CALL UDO AGAIN WITH INCREMENTED COUNTER
	endif								;END OF THIS CONDITIONAL BRANCH
	SKIP:							;LABEL
		xout	amix + (aCarF*aEnv)			;MIX LOCAL BAND WITH SUBSEQUENT BANDS GENERATED VIA RECURSION
endop								;END OF UDO

instr	1	;READ IN WIDGETS
	gkCarSource	chnget	"CarSource"
	gkbase		chnget	"base"
	gknum           chnget	"num"
	gkbw            chnget	"bw" 	
	gkincr          chnget	"incr"	
	gkBPGain        chnget	"BPGain"
	gkHPGain        chnget	"HPGain"
	gksteepness     chnget	"steepness"
	gkSynType	chnget	"SynType"
	gkgate		chnget	"gate"
	gklevel		chnget	"level"
endin

instr	2	;SIMPLE SYNTH
	icps	cpsmidi				;READ MIDI NOTE IN CPS FORMAT
	icps	=	icps*0.5		;TRANSPOSE DOWN AND OCTAVE
	aenv	linsegr	0,0.01,1,0.02,0		;CREATE A SIMPLE GATE-TYPE ENVELOPE

	if gkSynType==1 then			;IF SYNTH TYPE CHOSEN FROM BUTTON BANK GUI IS SAWTOOTH...
	 a1	vco2	1,icps			;...CREATE A SAWTOOTH WAVE TONE
	 a1	tone	a1,12000		;LOWPASS FILTER THE SOUND
	elseif gkSynType=2 then			;IF SYNTH TYPE CHOSEN FROM BUTTON BANK GUI IS SQUARE...
	 a1	vco2	1,icps,2,0.5		;...CREATE A SQUARE WAVE TONE
	 a1	tone	a1,12000		;LOWPASS FILTER THE SOUND
	elseif gkSynType=3 then			;IF SYNTH TYPE CHOSEN FROM BUTTON BANK GUI IS PULSE...
	 a1	vco2	1,icps,2,0.1		;...CREATE A PULSE WAVE TONE
	 a1	tone	a1,12000		;LOWPASS FILTER THE SOUND
	else					;OTHERWISE...
	 a1	pinkish	10			;...CREATE SOME PINK NOISE
	 a1	butbp	a1,icps,icps		;BANDPASS FILTER THE SOUND. BANDWIDTH = 1 OCTAVE. NARROW BANDWIDTH IF YOU WANT MORE OF A SENSE OF PITCH IN THE NOISE SIGNAL.
	endif					;END OF THIS CONDITIONAL BRANCH
	gaSyn	=	gaSyn + (a1*aenv)	;APPLY ENVELOPE
endin

instr	3
	ktrig	changed	gkbase,gkbw,gknum,gkincr	;IF ANY OF THE INPUT VARIABLE ARE CHANGED GENERATE A MOMENTARY '1' VALUE (A BANG IN MAX-MSP LANGUAGE)
	if ktrig=1 then					;IF A CHANGED VALUE TRIGGER IS RECEIVED...
	  reinit UPDATE					;REINITIALISE THIS INSTRUMENT FROM THE LABEL 'UPDATE'
	endif						;END OF THIS CONDITIONAL BRANCH
	UPDATE:						;LABEL
	ibase	init	i(gkbase)			;CREATE AN INITIALISATION TIME VARIABLE FROM FLTK GUI CONTROL
	inum	init	i(gknum)			;CREATE AN INITIALISATION TIME VARIABLE FROM FLTK GUI CONTROL
	ibw	init	i(gkbw)				;CREATE AN INITIALISATION TIME VARIABLE FROM FLTK GUI CONTROL
	iincr	init	i(gkincr)			;CREATE AN INITIALISATION TIME VARIABLE FROM FLTK GUI CONTROL
	
	aMod	inch	1				;READ LIVE AUDIO FROM THE COMPUTER'S LEFT INPUT CHANNEL
	
	;GATE MODULATOR SIGNAL
	if gkgate==1 then				;IF 'Gate Modulator' SWITCH IS ON....
	 krms	rms	aMod				;SCAN RMS OF MODUALTOR SIGNAL
	 kgate	=	(krms<0.05?0:1)			;IF RMS OF MODULATOR SIGNAL IS BELOW A THRESHOLD, GATE WILL BE CLOSED (ZERO) OTHERWISE IT WILL BE OPEN ('1'). LOWER THE THRESHOLD IF THE GATE IS CUTTING OUT TOO MUCH DESIRED SIGNAL, RAISE IT IF TOO MUCH EXTRANEOUS NOISE IS ENTERING THE OUTPUT SIGNAL.
	 kgate	port	kgate,0.01			;DAMP THE OPENING AND CLOSING OF THE GATE SLIGHTLY
	 agate	interp	kgate				;INTERPOLATE GATE VALUE AND CREATE AN A-RATE VERSION
	 aMod	=	aMod * agate			;APPLY THE GATE TO THE MODULATOR SIGNAL
	endif
	
	if gkCarSource==1 then				;IF 'SYNTH' IS CHOSEN AS CARRIER SOURCE...
	 aCar	=	gaSyn				;...ASSIGN SYNTH SIGNAL FROM INSTR 2 AS CARRIER SIGNAL
	else						;OTHERWISE...
	 aCar	inch	2				;READ AUDIO FROM RIGHT INPUT CHANNEL FOR CARRIER SIGNAL
	endif
		
	icount	init	0				;INITIALISE THE FILTER COUNTER TO ZERO
		amix	VocoderChannel	aMod,aCar,gksteepness,ibase,ibw,iincr,icount,inum	;CALL 'VocoderChannel' UDO - (WILL RECURSE WITHIN THE UDO ITSELF FOR THEW REQUIRED NUMBER OF FILTERS
		
	;HIGH-PASS CHANNEL
	iHPcf	=	cpsmidinn(ibase+(inum*iincr)+1)	;HIGHPASS FILTER CUTOFF (ONE INCREMENT ABOVE THE HIGHEST BANDPASS FILTER)
	iHPcf	limit	iHPcf,2000,18000		;LIMIT THE HIGHPASS FILTER TO BE WITHIN SENSIBLE LIMITS

	aModHP	buthp	aMod, iHPcf			;HIGHPASS FILTER THE MODULATOR
	aEnv	follow2	aModHP,0.01,0.01		;FOLLOW THE HIGHPASS FILTERED MODULATOR'S AMPLITUDE ENVELOPE
	aCarHP	buthp	aCar, iHPcf			;HIGHPASS FILTER THE CARRIER
	amix	=	((amix*gkBPGain*5)+(aCarHP*aEnv*gkHPGain*3))*gklevel	;MIX THE HIGHPASS FILTERED CARRIER WITH THE BANDPASS FILTERS. APPLY THE MODULATOR'S ENVELOPE.

		outs	amix,amix			;SEND AUDIO TO THE OUTPUTS
		clear	gaSyn				;CLEAR THE INTERNAL SYNTH ACCUMULATING GLOBAL VARIABLE, READ FOR THE NEXT PERF. PASS
	rireturn					;RETURN FROM REINITIALISATION PASS. (NOT REALLY NEED AS THE endin FULFILS THE SAME FUNCTION.)
endin

</CsInstruments>

<CsScore>
i 1 0 [60*60*24*7]	;READ IN WIDGETS
i 3 0 [60*60*24*7]	;VOCODER
</CsScore>

</CsoundSynthesizer>; wguide1.csd
; Written by Iain McCurdy, 2013,2015.

; Encapsulation of the 'wguide1' opcode, used for filtering and synthesis.
; A waveguide filter produces resonance peaks that follow the frequencies of the harmonic series with diminishing strength above its defined base frequency.

; frequency of the wave guide can be determined either in hertz of as a note number

; Activating the 'Keyboard Input' switch will allow frequency control from the keyboard (internal or external) and polyphony. 
;  In this mode, 'Frequency' and 'Note Num.' controls will be disabled.

; Source input sound can be either 'live audio input', or synthetic gravel, dust, air, a click or a pluck.
;  Click and pluck are intended to used with 'Keyboard Input' activated.

; Frequency	-	frequency of the waveguide
; Note Num.	-	frequency of the waveguide expressed as a note number
; Width		-	offsets the frequencies of the left and right channels to imbue added stereo width
; Cutoff	-	cutoff frequency of a lowpass filter within the feedback loop of the waveguide
;			 cutoff can be defined as a fixed value (in hertz) or as a ratio with the frequency of the waveguide (Ratio x Frequency = cutoff)
; Feedback	-	feedback ratio of the waveguide unit.
;			 negative feedback will shift the fundemental down one octave and only odd harmonics will be preset

; Bandpass Filter
; ---------------
; A double butterworth bandpass filter can be inserted, the cutoff frequency of which relates to the frequency defined for the waveguide.
; Bandpass On/Off	-	turns the bandpass filter on and off
; Balance	-	inserts a dynamic balancing to compensate for power loss on account of the bandpass filtering
; Width		-	bandwidth of the filters
; Harm.		-	ratio of bandpass filter cutoff to waveguide frequency: Harm x Frequency = bandpass cutoff frequency

; Mix	-	dry/wet mix
; Level	-	amplitude scaling

<Cabbage>
form caption("wguide1") size(650,200), pluginid("WGu1"), scrollbars(0) style("legacy")
image           bounds(0, 0, 650,120), colour(125, 95, 55), shape("sharp"), outlinecolour(225,195,155), outlinethickness(2) 
label   bounds(505,  4,136, 17), align("centre"), text("W A V E G U I D E"), fontcolour(silver), align("left")
image   bounds(510,  7,125,  1), colour(200,170,130), shape("sharp"), outlinecolour(silver), outlinethickness(1)
image   bounds(512, 18,124,  1), colour(200,170,130), shape("sharp"), outlinecolour(silver), outlinethickness(1)
checkbox bounds( 20,  7,120, 12), text("Keyboard Input"), channel("input"), fontcolour("white"), colour(yellow)
checkbox bounds( 20, 22,120, 12), text("Low Cut"), channel("LowCut") fontcolour("white") colour(yellow) value(0)
label    bounds(140,  5, 62, 11), text("Source"), fontcolour("white")
combobox bounds(140, 16, 62, 16), channel("source"), value(1), text("Live","Gravel","Dust","Air","Click","Pluck")
image    bounds( 10, 41,145, 70), colour(0,0,0,0), plant("GUIcontrol"), identchannel("NoKeyboardID"), visible(1) {
rslider  bounds(  0,  0, 70, 70),  text("Frequency"),  channel("freq"),      trackercolour(225,195,155), range(8.2, 12542, 160, 0.25),  colour( 85, 55,15), textcolour(white), fontcolour("silver")
rslider  bounds( 65,  0, 70, 70),  text("Note Num."),  channel("notnum"),    trackercolour(225,195,155), range(0, 127, 51, 1,1),        colour( 85, 55,15), textcolour(white), fontcolour("silver")
}
rslider  bounds(140, 41, 70, 70),  text("Width"),      channel("StWidth"),   trackercolour(225,195,155), range(-0.2, 0.2, 0, 1,0.001),        colour( 85, 55,15), textcolour(white), fontcolour("silver")

image    bounds(207,  4, 66,112), colour(125, 95, 55), outlinecolour("silver"), outlinethickness(1), line(1)
label    bounds(212,  8, 56, 11),  text("Cutoff"), fontcolour("white")
combobox bounds(212, 20, 56, 15),  text("Fixed","Ratio"),channel("CutoffMode"), value(1)
rslider  bounds(205, 41, 70, 70),  text("Hertz"),     channel("cutoff"),     trackercolour(225,195,155), range(20,20000,8000,0.25), visible(1), colour( 85, 55,15), textcolour(white), fontcolour("silver"), identchannel("cutoff_ident")
rslider  bounds(205, 41, 70, 70),  text("Ratio"),     channel("CutoffRatio"),trackercolour(225,195,155), range(1,40,8),             visible(0), colour( 85, 55,15), textcolour(white), fontcolour("silver"), identchannel("CutoffRatio_ident")

rslider  bounds(270, 41, 70, 70),  text("Feedback"),   channel("feedback"),   trackercolour(225,195,155), range(-0.9999999, 0.9999999, 0.8), colour( 85, 55,15), textcolour(white), fontcolour("silver")

line     bounds(345,  5,  2,110)
checkbox bounds(365,  7,120, 12), text("Bandpass On/Off"), channel("BPFOnOff") fontcolour("white") colour(yellow) value(0)
checkbox bounds(365, 22,120, 12), text("Balance"),         channel("BalanceOnOff") fontcolour("white") colour(yellow) value(0)
rslider  bounds(355, 41, 70, 70),  text("Bandwidth"),      channel("Bandwidth"),   trackercolour(225,195,155), range(0.001,100.00,0.2,0.5,0.001),             colour( 85, 55,15), textcolour(white), fontcolour("silver")
rslider  bounds(420, 41, 70, 70),  text("Harm."),          channel("HarmRatio"),trackercolour(225,195,155), range(1,48,1,1,1),             colour( 85, 55,15), textcolour(white), fontcolour("silver")
line     bounds(495,  5,  2,110)

rslider  bounds(505, 41, 70, 70),  text("Mix"),        channel("mix"),       trackercolour(225,195,155), range(0, 1.00, 1),             colour( 85, 55,15), textcolour(white), fontcolour("silver")
rslider  bounds(570, 41, 70, 70),  text("Level"),      channel("level"),     trackercolour(225,195,155), range(0, 5.00, 0.7,0.5,0.001),           colour( 85, 55,15), textcolour(white), fontcolour("silver")

keyboard bounds(  0,210, 650, 80), visible(1), identchannel("keyboardID")

</Cabbage>

<CsoundSynthesizer>

<CsOptions>
;-d -n
-dm0 -n -+rtmidi=null -M0
</CsOptions>

<CsInstruments>

sr 		= 	44100	; SAMPLE RATE
ksmps 		= 	32	; NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls 		= 	2	; NUMBER OF CHANNELS (2=STEREO)
0dbfs		=	1
massign	0,2

; Author: Iain McCurdy (2012)


instr	1
	kporttime	linseg	0,0.01,0.03				; CREATE A VARIABLE THAT WILL BE USED FOR PORTAMENTO TIME
	gkinput		chnget	"input"					; READ WIDGETS...
	gkLowCut	chnget	"LowCut"
	gkfreq		chnget	"freq"
	gknotnum	chnget	"notnum"
	gkBPFOnOff	chnget	"BPFOnOff"
	gkBalanceOnOff	chnget	"BalanceOnOff"
	gkBandwidth	chnget	"Bandwidth"
	gkHarmRatio	chnget	"HarmRatio"
	gkHarmRatio	port	gkHarmRatio,0.5
	gkCutoffMode	chnget	"CutoffMode"
	gkCutoffMode	init	1
	gkcutoff	chnget	"cutoff"
	gkCutoffRatio	chnget	"CutoffRatio"
	gkfeedback	chnget	"feedback"
	gkmix		chnget	"mix"
	gkmix		portk	gkmix,kporttime
	gklevel		chnget	"level"
	gklevel		portk	gklevel,kporttime
	gkfreq		portk	gkfreq,kporttime
	gafreq		interp	gkfreq
	gkStWidth	chnget	"StWidth"

	if changed(gkinput)==1||timeinstk()==1 then
	 chnset	sprintfk("visible(%d)",1-gkinput),"NoKeyboardID"
;	 chnset	sprintfk("visible(%d)",gkinput),"keyboardID"	 
	endif

	schedkwhen	trigger(gkinput,0.5,0),0,0,99,0,1,210,115
	schedkwhen	trigger(gkinput,0.5,1),0,0,99,0,1,120,210

	ktrig1		changed	gkfreq
	ktrig2		changed	gknotnum
	if ktrig1==1 then						; DUAL FREQUENCY AND NOTE NUMBER CONTROLS
	 koct	=	octcps(gkfreq)
	 chnset	(koct-3)*12,"notnum"
	elseif ktrig2==1 then
	 chnset	cpsmidinn(gknotnum),"freq"
	endif
	
	event_i	"i",2,0,-1
	ktrig	trigger	gkinput,0.5,2
	if ktrig=1 then
	 if gkinput=0 then
	  event	"i",2,0,-1
	 else
	  turnoff2	2,0,0
	 endif
	endif

	/* SOURCE SOUND */
	gksource	chnget	"source"
	if gksource==1 then
 	 gasigL, gasigR	ins
 	elseif gksource==2 then
 	 gasigL	gausstrig	0.5, 100, 8, 0
 	 gasigR	gausstrig	0.5, 100, 8, 0
 	elseif gksource==3 then
 	 kdens	random	475,525
	 gasigL	dust2	0.5, kdens
 	 kdens	random	525,475
	 gasigR	dust2	0.5, kdens
 	elseif gksource==4 then
	 gasigL	pinkish	0.2
	 gasigR	pinkish	0.2
	endif	
endin


instr	2
	/* MIDI AND GUI INTEROPERABILITY */
	iMIDIflag	=	0			; IF MIDI ACTIVATED = 1, NON-MIDI = 0
	mididefault	1, iMIDIflag			; IF NOTE IS MIDI ACTIVATED REPLACE iMIDIflag WITH '1'

	if iMIDIflag==1 then				; IF THIS IS A MIDI ACTIVATED NOTE...
	 inum	notnum
	 ivel	veloc	0,1
	 p1	=	p1 + (rnd(1000)*0.0001)
	 if gkinput=0 then
	  turnoff
	 endif
	 icps	cpsmidi					; READ MIDI PITCH VALUES - THIS VALUE CAN BE MAPPED TO GRAIN DENSITY AND/OR PITCH DEPENDING ON THE SETTING OF THE MIDI MAPPING SWITCHES
	 kfreq	init		icps
	 afreq	init		icps
	else
	 kfreq	=		gkfreq
	 afreq	=		gafreq 
	endif						; END OF THIS CONDITIONAL BRANCH

	if gksource==5 then
	 asigL	mpulse	ivel*5,0,0.001
	 asigR	=	asigL
	elseif gksource==6 then
 	 kenv	expseg	1,0.08,0.001,1,0.001
 	 asigL	dust2	20*(kenv-0.001),100
 	 icf	=	icps*(1+(4*ivel))
 	 icf	limit	icf,20,4000
 	 asigL	butlp	asigL,icf
 	 asigR	=	asigL
	else
	 aenv		linsegr	0,0.05,1,0.05,0
	 asigL	=	gasigL*aenv
	 asigR	=	gasigR*aenv
	endif
	
	if changed(gkCutoffMode)==1 then
	 if gkCutoffMode==1 then
	  chnset	"visible(1)","cutoff_ident"
	  chnset	"visible(0)","CutoffRatio_ident"
	 else
	  chnset	"visible(0)","cutoff_ident"
	  chnset	"visible(1)","CutoffRatio_ident"
	 endif
	endif


	if gkCutoffMode==2 then
	 kcutoff	limit	gkCutoffRatio*kfreq,20,sr/2
	else
	 kcutoff	=	gkcutoff
	endif	

	kLDiff		=	semitone(-gkStWidth)
	kRDiff		=	semitone(gkStWidth)	
	
	aresL 		wguide1 asigL, afreq*kLDiff, kcutoff, gkfeedback
	aresR 		wguide1 asigR, afreq*kRDiff, kcutoff, gkfeedback
	aresL 		dcblock	aresL
	aresR 		dcblock	aresR
	
	/* LOW CUT */
	if gkLowCut==1 then
	 aresL		buthp	aresL, kfreq
	 aresR		buthp	aresR, kfreq
	 aresL		buthp	aresL, kfreq
	 aresR		buthp	aresR, kfreq
	endif
	
	/* BANDPASS FILTER */
	if gkBPFOnOff==1 then
	 aFiltL		butbp	aresL,kfreq*gkHarmRatio,kfreq*gkBandwidth
	 aFiltR		butbp	aresR,kfreq*gkHarmRatio,kfreq*gkBandwidth
	 aFiltL		butbp	aFiltL,kfreq*gkHarmRatio,kfreq*gkBandwidth
	 aFiltR		butbp	aFiltR,kfreq*gkHarmRatio,kfreq*gkBandwidth
	 if gkBalanceOnOff==1 then
	  aresL	balance	aFiltL,aresL
	  aresR	balance	aFiltR,aresR
	 else
	  aresL	=	aFiltL
	  aresR	=	aFiltR
	 endif
	endif
	
	amixL		ntrpol	gasigL*aenv, aresL, gkmix
	amixR		ntrpol	gasigR*aenv, aresR, gkmix

	kenv		linsegr	1,15,0
	amixL		=	amixL * gklevel * kenv
	amixR		=	amixR * gklevel * kenv
	ktime		timeinsts
	krms		rms	amixL,3
	if krms<0.00001&&ktime>0.2&&iMIDIflag==1 then
	 turnoff2	p1,4,0
	endif

			outs	amixL, amixR		; WGUIDE1 OUTPUTS ARE SENT OUT
endin

instr	99
	ky	line	p4,p3,p5
	if metro(16)==1 then
	 chnset	sprintfk("pos(0,%d)",ky),"keyboardID"
	endif
endin
</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
</CsScore>


</CsoundSynthesizer>



























; wguide2.csd
; Written by Iain McCurdy, 2013.

; For a more extensive use of this opcode see the example "Harmonics.csd"

<Cabbage>
form caption("wguide2") size(595, 90), pluginid("WGu2") style("legacy")
image        pos(0, 0), size(595, 90), colour(25,0,25), shape("rounded"), outlinecolour("white"), outlinethickness(4)
button  bounds( 10, 30, 55, 25), text("PLUCK"), channel("pluck"), toggle(0)
rslider bounds( 65, 11, 70, 70), text("Freq. 1"),   channel("freq1"),     range(20, 8000, 160, 0.25), colour(150,110,110), trackercolour(white)
rslider bounds(130, 11, 70, 70), text("Freq. 2"),   channel("freq2"),     range(20, 8000, 160, 0.25), colour(150,110,110), trackercolour(white)
rslider bounds(195, 11, 70, 70), text("Cutoff 1"), channel("cutoff1"),   range(20,20000,8000,0.25),  colour(150,110,110), trackercolour(white)
rslider bounds(260, 11, 70, 70), text("Cutoff 1"), channel("cutoff2"),   range(20,20000,8000,0.25),  colour(150,110,110), trackercolour(white)
rslider bounds(325, 11, 70, 70), text("F.back 1"), channel("feedback1"), range(-0.999, 0.999, 0.2),  colour(150,110,110), trackercolour(white)
rslider bounds(390, 11, 70, 70), text("F.back 2"), channel("feedback2"), range(-0.999, 0.999, 0.2),  colour(150,110,110), trackercolour(white)
rslider bounds(455, 11, 70, 70), text("Mix"),      channel("mix"),       range(0, 1.00, 0.7),        colour(150,110,110), trackercolour(white)
rslider bounds(520, 11, 70, 70), text("Level"),    channel("level"),     range(0, 1.00, 0.7),        colour(150,110,110), trackercolour(white)
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

instr	1
	kporttime	linseg	0,0.001,0.05
	gkfreq1		chnget	"freq1"					;READ WIDGETS...
	gkfreq1		portk	gkfreq1,kporttime
	afreq1		interp	gkfreq1
	gkfreq2		chnget	"freq2"					;READ WIDGETS...
	gkfreq2		portk	gkfreq2,kporttime
	afreq2		interp	gkfreq2
	gkcutoff1	chnget	"cutoff1"				;
	gkcutoff2	chnget	"cutoff2"				;
	gkfeedback1	chnget	"feedback1"				;
	gkfeedback2	chnget	"feedback2"				;
	gkmix		chnget	"mix"					;
	gklevel		chnget	"level"					;
	;asigL, asigR	diskin2	"Seashore.wav",1,0,1			;USE SOUND FILE FOR TESTING
	asigL, asigR	ins

 	kFBtot	=	gkfeedback1 + gkfeedback2		; protect against combined feedbacks greater than 0.5
 	if kFBtot>0.5 then
 	 gkfeedback1	=	gkfeedback1 / (kFBtot*2)
 	 gkfeedback2	=	gkfeedback2 / (kFBtot*2)
 	else
 	 gkfeedback1	=	gkfeedback1
 	 gkfeedback2	=	gkfeedback2
 	endif

	aplk	init	0
	kpluck	chnget	"pluck"					; pluck button
	if changed(kpluck)==1 then
	 aplk	=	1
	 asigL	+=	aplk
	 asigR	+=	aplk
	endif

	aresL	wguide2 asigL, afreq1, afreq2, gkcutoff1, gkcutoff2, gkfeedback1, gkfeedback2
	aresR	wguide2 asigR, afreq1, afreq2, gkcutoff1, gkcutoff2, gkfeedback1, gkfeedback2
	aresL	dcblock2	aresL	;BLOCK DC OFFSET
	aresR	dcblock2	aresR	;BLOCK DC OFFSET
	amixL		ntrpol	asigL,aresL,gkmix
	amixR		ntrpol	asigR,aresR,gkmix
			outs	amixL*gklevel, amixR*gklevel		;WGUIDE1 OUTPUTS ARE SENT OUT
endin

</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
</CsScore>


</CsoundSynthesizer>
; AutopanTremolo.csd
; 
; An autopan/tremolo effect in which a variety of LFO shapes can be employed.
; 
; LFO Shapes
; ----------
; sine
; triangle
; square
; Randomi	-	an interpolating random function
; Randomh	-	a 'sample and hold' type function
; Rspline	-	random spline
; User	-	an LFO shape (best understood when using tremolo function) 
; 		in which the user can define the bias (percussive type : reverse percussive) 
; 		and duty emphasis (emphasis of high state/loud against low state/quiet)

<Cabbage>
form caption("Autopan / Tremolo") size(565, 102), pluginid("aptr"), guirefresh(32) style("legacy")
image pos( 0,  0),                size(565, 102),     colour("Maroon"), shape("rounded"), outlinecolour("white"), outlinethickness(4) 
rslider  bounds(  5,  6, 90, 90), text("Freq.[Hz]"),  channel("rate"),  range(0.1, 50, 0.5, 0.5),   valuetextbox(1), textbox(1), trackercolour("tomato")
rslider  bounds( 80,  6, 90, 90), text("Tempo[BPM]"), channel("tempo"), range(6, 3000, 30, 0.5, 1), valuetextbox(1), textbox(1), trackercolour("tomato")
rslider  bounds(175,  6, 90, 90), text("Depth"),      channel("depth"), range(0, 1.00, 1, 0.5),     valuetextbox(1), textbox(1), trackercolour("tomato")

line     bounds(360, 10, 107, 2), colour("Grey")
label    bounds(393,  5, 43, 12), text("USER"), colour("Maroon")
rslider  bounds(345, 16, 80, 80), text("Bias"),      channel("bias"), range(-1, 1.00, 0),          valuetextbox(1), textbox(1), trackercolour("tomato")
rslider  bounds(405, 16, 80, 80), text("Duty"),      channel("duty"), range(0.01, 2000.00, 1, 0.25, 0.001),          valuetextbox(1), textbox(1), trackercolour("tomato")

rslider  bounds(470,  6, 90, 90), text("Level"),      channel("level"), range(0, 1.00, 1),          valuetextbox(1), textbox(1), trackercolour("tomato")

checkbox bounds(160, 40, 25, 25), colour("yellow"), channel("indicator"),  value(0), shape("ellipse"), active(0)
combobox bounds(260, 13,  90,20), channel("mode"), value(1), text("Autopan", "Tremolo")
combobox bounds(260, 38,  90,20), channel("wave"), value(1), text("Sine", "Triangle", "Square", "Randomi", "Randomh", "Rspline", "User")
checkbox bounds(260, 63, 90, 15), text("TEST TONE"), colour("lime"), channel("test"),  value(0)
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

;Author: Iain McCurdy (2012)

gihanning	ftgen	0, 0, 8192, 20, 2, 1

opcode	PanTrem,aa,aakkkKkK
	ainL,ainR,krate,kdepth,kmode,kwave,kbias,kduty	xin	;READ IN INPUT ARGUMENTS
	ktrig	changed	kwave				;IF LFO WAVEFORM TYPE IS CHANGED GENERATE A MOMENTARY '1' (BANG)
	if ktrig=1 then					;IF A 'BANG' HAS BEEN GENERATED IN THE ABOVE LINE
		reinit	UPDATE				;BEGIN A REINITIALIZATION PASS FROM LABEL 'UPDATE' SO THAT LFO WAVEFORM TYPE CAN BE UPDATED
	endif						;END OF THIS CONDITIONAL BRANCH
	UPDATE:						;LABEL CALLED UPDATE
	iwave	init		i(kwave)
	iwave	limit	iwave,	0, 6			;
	if iwave==3 then				;if 'randomi' is chosen...
	 klfo	randomi	-kdepth,kdepth,krate,1
	elseif iwave==4 then				;or if 'randomh' is chosen...
	 klfo	randomh	-kdepth,kdepth,krate,1		
	elseif iwave==5 then				;or if 'rspline' is chosen...
	 klfo	rspline	-kdepth,kdepth,krate,krate*2		
	elseif iwave==6 then				;or if 'user' has been chosen...
	 aphs	phasor	krate				;create a linear pointer from 0 to 1 
	 aphs 	pdhalf	aphs, kbias			;distort the linearity using pdhalf
	 kphs	downsamp	aphs			;downsample to krate
	 klfo	tablei	kphs,gihanning,1		;read 
	 klfo	pow	klfo,kduty
	 klfo	=	((klfo*2)-1)*kdepth
	else						;otherwise (use lfo opcode)
	 klfo	lfo	kdepth, krate, iwave		;CREATE AN LFO
	endif
	rireturn					;RETURN FROM REINITIALIZATION PASS
	klfo	=	(klfo*0.5)+0.5			;RESCALE AND OFFSET LFO SO IT STAYS WITHIN THE RANGE 0 - 1 ABOUT THE VALUE 0.5
	if iwave=2||iwave==4 then			;IF SQUARE WAVE MODULATION HAS BEEN CHOSEN...
		klfo	portk	klfo, 0.001		;SMOOTH THE SQUARE WAVE A TINY BIT TO PREVENT CLICKS
	endif						;END OF THIS CONDITIONAL BRANCH	
	if kmode=0 then	;PAN				;IF PANNING MODE IS CHOSEN FROM BUTTON BANK...
		alfo	interp	klfo			;INTERPOLATE K-RATE LFO AND CREATE A-RATE VARIABLE
		aoutL	=	ainL*sqrt(alfo)		;REDEFINE GLOBAL AUDIO LEFT CHANNEL SIGNAL WITH AUTO-PANNING
		aoutR	=	ainR*(1-sqrt(alfo))	;REDEFINE GLOBAL AUDIO RIGHT CHANNEL SIGNAL WITH AUTO-PANNING
		kindicator	=	(klfo>0.5?1:0)
		chnset	kindicator,"indicator"
	elseif kmode=1 then	;TREM			;IF TREMOLO MODE IS CHOSEN FROM BUTTON BANK...
		kindicator	=	(klfo>0.5?1:0)
		if changed:k(kindicator)==1 then
		 chnset	kindicator,"indicator"
		endif
		klfo	=	klfo+(0.5-(kdepth*0.5))	;MODIFY LFO AT ZERO DEPTH VALUE IS 1 AND AT MAX DEPTH CENTRE OF MODULATION IS 0.5
		alfo	interp	klfo			;INTERPOLATE K-RATE LFO AND CREATE A-RATE VARIABLE
		aoutL	=	ainL*(alfo^2)		;REDEFINE GLOBAL AUDIO LEFT CHANNEL SIGNAL WITH TREMELO
		aoutR	=	ainR*(alfo^2)		;REDEFINE GLOBAL AUDIO RIGHT CHANNEL SIGNAL WITH TREMELO
	endif						;END OF THIS CONDITIONAL BRANCH
		xout	aoutL,aoutR			;SEND AUDIO BACK TO CALLER INSTRUMENT
endop

instr 1
krate chnget "rate"
ktempo chnget "tempo"
kdepth chnget "depth"
kmode chnget "mode"
kwave chnget "wave"
kbias chnget "bias"
kduty chnget "duty"
klevel chnget "level"
ktest	chnget	"test"

ktrig	changed	krate
ktrig2	changed	ktempo
if ktrig=1 then
 chnset	krate*60,"tempo"
elseif ktrig2=1 then
 chnset	ktempo/60,"rate"
endif

if ktest=1 then
 a1	vco2	0.2,300,4,0.5
 a2	=	a1
else
 a1,a2	ins
endif

a1,a2	PanTrem	a1,a2,krate,kdepth,kmode-1,kwave-1,kbias,kduty
a1	=	a1 * klevel
a2	=	a2 * klevel
	outs	a1,a2
endin

</CsInstruments>

<CsScore>
i 1 0 [60*60*24*7]
</CsScore>

</CsoundSynthesizer>; doppler.csd
; Written by Iain McCurdy, 2013

; CONTROLS
; Input		-	select input: either left channel, right channel, or a mix of both channels
; Shape		-	shape of the LFO moving the source: either sine, triangle, random spline OR manual
			   note, if 'random' is chosen, 'speed' can take a little while to respond to changes
			   made to the 'speed' control is the speed was previously slow
			   If 'manual' is selected source position is controlled using the on screen slider
; Room Size	-	effectively the dpeth of the doppler pitch modulating effect
; Speed		-	speed of the LFO moving the source with respect to the mic. position
; Depth		-	amplitude of the LFO moving the source
; Smoothing	-	a smoothing filter applied to doppler pitch modulation. Its effect can be subtle.
; Mix		-	a dry/wet mixer. Mixing the dry and wet signals can be used to create chorus effects.
; Ampscale	-	amount of amplitude drop off as the source moves away from the source. 
			 Kind of like another room size control
; Pan Depth	-	Amount of left-right movement in the output as the source swings past the microphone
; Out Amp	-	scales the output signal
; Mic.Position	-	Position of the microphone
; Source Position-	Location of the source (for display only unless 'manual' shape is chosen)

<Cabbage>
form caption("-oOo-"), size(610, 180), pluginid("dopp")   style("legacy")
label    bounds(20, 10, 35,11), text("Input:")
combobox bounds(10, 22, 60,18), channel("input"), value(4), text("left","right","mixed","test")
label    bounds(20, 50, 35,11), text("Shape:")
combobox bounds(10, 62, 60,18), channel("shape"), value(1), text("sine","triangle","random","manual")

rslider bounds( 75, 10, 80, 80), channel("RoomSize"), range(0.1,100,40,0.5,0.5), text("Room Size"), textbox(1), valuetextbox(1), colour( 45, 45, 45), trackercolour(200,200,200)
rslider bounds(140, 10, 80, 80), channel("speed"), range(0,10,0.08,0.5,0.01), text("Speed"), textbox(1), valuetextbox(1),        colour( 45, 45, 45), trackercolour(200,200,200)
rslider bounds(205, 10, 80, 80), channel("depth"), range(0,0.5,0.5,0.5,0.01), text("Depth"), textbox(1), valuetextbox(1),        colour( 45, 45, 45), trackercolour(200,200,200)
rslider bounds(270, 10, 80, 80), channel("filtercutoff"), range(1,20,6,1,1), text("Smooth"), textbox(1), valuetextbox(1),     colour( 45, 45, 45), trackercolour(200,200,200)
rslider bounds(335, 10, 80, 80), channel("ampscale"), range(0,1,0.98), text("Amp.Scl."), textbox(1), valuetextbox(1),           colour( 45, 45, 45), trackercolour(200,200,200)
rslider bounds(400, 10, 80, 80), channel("PanDep"), range(0,0.5,0.4), text("Pan Depth"), textbox(1), valuetextbox(1),            colour( 45, 45, 45), trackercolour(200,200,200)
rslider bounds(465, 10, 80, 80), channel("mix"), range(0,1,1), text("Mix"), textbox(1), valuetextbox(1),                         colour( 45, 45, 45), trackercolour(200,200,200)
rslider bounds(530, 10, 80, 80), channel("OutAmp"), range(0,1,0.5), text("Level"), textbox(1), valuetextbox(1),                  colour( 45, 45, 45), trackercolour(200,200,200)

hslider bounds( 10, 86,590, 40), channel("microphone"), range(0,1.00,0.5), text("Mic. Position"),   textbox(1), valuetextbox(1), colour(100,100,100), trackercolour(200,200,200)
hslider bounds( 10,111,590, 40), channel("source"),     range(0,1.00,0.5), text("Source Position"), textbox(1), valuetextbox(1), colour(100,100,100), trackercolour(200,200,200)

label    bounds( 7, 155,120,18), text("D O P P L E R"),  fontcolour(100,100,100)
label    bounds( 5, 153,120,18), text("D O P P L E R"),  fontcolour(180,180,180)

label   bounds(232,160, 165, 12), text("Author: Iain McCurdy |2013|"), FontColour("grey")

</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-dm0 -n -+rtmidi=null -M0
</CsOptions>

<CsInstruments>

sr 		= 	44100	;SAMPLE RATE
ksmps 		= 	32	;NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls 		= 	2	;NUMBER OF CHANNELS (2=STEREO)
0dbfs		=	1	;MAXIMUM AMPLITUDE VALUE

;AMPLITUDE SCALING CURVE
giampcurve	ftgen	0,0,131072,5,0.01,131072*0.5,1,131072*0.5,0.01

instr	1
	gkinput      	chnget  "input"  
	gkRoomSize      chnget  "RoomSize"  
	gkspeed         chnget  "speed"     
	gkdepth		chnget	"depth"     
	gkfiltercutoff	chnget	"filtercutoff"
	gkfiltercutoff	init	6
	gkampscale	chnget	"ampscale"
	gkPanDep	chnget	"PanDep"
	kmix		chnget	"mix"				;READ IN DRY/WET CROSSFADER WIDGET
	gkOutAmp	chnget	"OutAmp"
	gkmicrophone	chnget	"microphone"
	gkshape		chnget	"shape"
	gkshape		init	1
	
	/* INPUT */
	aL,aR		ins
	if gkinput=1 then
	 asig	=	aL
	elseif gkinput=2 then
	 asig	=	aR
	elseif gkinput=3 then
	 asig	=	(aL+aR)*0.677
	else
 	 ;INPUT TONE=============================================(for testing)
	 asig		vco2	.5, 300		;GENERATE TONE
	 asig		tone	asig, 1200	;LOW PASS FILTER TO SOFTEN THE TONE
	 ;=======================================================
	endif	 

	
	;LFO(modulates source position)=========================
	ktrig		changed	gkshape				;IF I-RATE VARIABLE SLIDER IS CHANGED GENERATE A '1'
	if ktrig=1 then						;IF TRIGGER IS '1'...
		reinit RESTART_LFO				;BEGIN A REINITIALISATION PASS FROM LABEL 'UPDATE' 
	endif							;END OF THIS CONDITIONAL BRANCH
	RESTART_LFO:						;LABEL CALLED 'UPDATE'
	if i(gkshape)=3 then					;IF 'RANDOM' SHAPE IS SELECTED...
	 gksource	rspline	0.5-gkdepth, 0.5+gkdepth, gkspeed,gkspeed*2
	elseif i(gkshape)=4 then				;IF 'MANUAL' SHAPE IS SELECTED...
	 gksource	chnget	"source"			;READ SOURCE POSITION FROM SLIDER 
	else
	 gksource	lfo	gkdepth, gkspeed, i(gkshape)-1	;LFO
	 gksource	=	gksource + 0.5			;OFFSET INTO THE POSITIVE DOMAIN
	endif
	rireturn
	chnset	gksource,"source"
	;======================================================
	
	kporttime	linseg	0, 0.001, 0.1		;RAMPING UP PORTAMENTO TIME VARIABLE	
	
	;DOPPLER================================================
	ispeedofsound   init	340.29				;SPEED OF SOUND DEFINED
	if gkshape==4 then
	 ksource	portk	gksource,kporttime		;SMOOTH SOURCE POSITION MOVEMENT
	else
	 ksource	=	gksource
	endif
	kmicrophone	portk	gkmicrophone, kporttime		;SMOOTH MICROPHOPNE POSITION MOVEMENT
	ktrig		changed	gkfiltercutoff			;IF I-RATE VARIABLE SLIDER IS CHANGED GENERATE A '1'
	if ktrig=1 then						;IF TRIGGER IS '1'...
		reinit UPDATE					;BEGIN A REINITIALISATION PASS FROM LABEL 'UPDATE' 
	endif							;END OF THIS CONDITIONAL BRANCH
	UPDATE:							;LABEL CALLED 'UPDATE'
	kdisp		limit	ksource-(kmicrophone-0.5), 0, 1	;CALCULATE DISPLACEMENT (DISTANCE) BETWEEN SOURCE AND MICROPHONE AND LIMIT VALUE TO LIE BETWEEN ZERO AND 1
	kamp		table	kdisp, giampcurve,1		;READ AMPLITUDE SCALING VALUE FROM TABLE
	kamp		ntrpol	1, kamp, gkampscale		;CALCULATE AMOUNT OF AMPLITUDE SCALING DESIRED BY THE USER FROM THE ON SCREEN SLIDER
	aout		doppler	asig*kamp, ksource*gkRoomSize, kmicrophone*gkRoomSize, ispeedofsound, i(gkfiltercutoff)	;APPLY DOPPLER EFFECT
	rireturn						;RETURN FROM REINITIALISATION PASS
	kpan		=	(gksource<gkmicrophone?0.5+gkPanDep:0.5-gkPanDep)	;CALCULATE PAN VALUE ACCORDING TO SOURCE AND MIC POSITION
	kpan		portk	kpan, kporttime			;APPLY PORTAMENTO SMOOTHING TO PAN POSITION VALUE 
	aL		ntrpol	asig,aout*sqrt(kpan)*gkOutAmp,kmix	;DRY/WET MIX LEFT CHANNEL
	aR		ntrpol	asig,aout*sqrt(1-kpan)*gkOutAmp,kmix	;DRY/WET MIX RIGHT CHANNEL
			outs	aL, aR				;SEND AUDIO TO OUTPUTS AND APPLY PANNING
endin

</CsInstruments>

<CsScore>
i 1 0 3600	;DUMMY SCORE EVENT - PERMITS REAL-TIME PERFORMANCE FOR 1 HOUR
</CsScore>

</CsoundSynthesizer>; DopplerSpin.csd
; Written by Iain McCurdy, 2013

; CONTROLS

; Speed				--	Frequency of the spinning
; Loc.				--	Listening position in relation to the circular motion (range from ('centre' to 'edge'). Can be thought of as a control for the amount of amplitude modulation.
; Orient.			--	direction of listening (in radians)
; Width				--	width of panning directionality in the modulation 
; Depth				--	depth of doppler effect (circle size).
; (Reverb) Scaling		--	a dry/wet mixer. Mixing the dry and wet signals can be used to create chorus effects.
; (Reverb) Time			--	amount of amplitude drop off as the source moves away from the source. 
; (Reverb) Damping		--	Amount of left-right movement in the output as the source swings past the microphone
; Mix				--	scales the output signal
; Level				--	Output Level
; Auto-Frequency		--	If this button is activated the frequency of the spinning moves randomly between -Frequency of Rotation and +Frequency of Rotation

; This example uses three LFOs to create the effect of a sound moving in a circle around the listener. 
; The three parameters controlled by these LFOs are amplitude, panning and delay time.
; The modulation of the delay time also results in a modulation of pitch which is sometimes referred to as the Doppler effect.                                             
; Note that in this example the 'vdelayxw' opcode is used to implement the delay and doppler shift. 
; This opcode is unique in that the delay time of the write pointer rather than the read pointer is modulated. 
; This is appropriate here as it is the sound source that is moving, not the listener.          
; Crucial to this effect is that that all three LFOs share the same frequency value. 
; Negative frequency values are also allowed - this would represent a change in direction of the source sounds motion around us.                             
; Also of crucial importance is the phase relationship between the three LFOs as this defines exactly where the sound source is in relation to the listener.                             
; The panning LFO should be at its points of minimum rate of change when the sound source is moving parallel to the direction in which the listener is facing, i.e. directly to the left or to the right of the listener.                   
; The delay time LFO (pitch modulation/doppler) should be at its points of minimum rate of change when the sound source is  moving perpendicular to the direction in which the listener is facing, i.e. directly in front of or behind the listener.
; The phase difference between these two LFOs is either 90 or 270 degrees, depending on whether the source sound is moving in a clockwise or anticlockwise direction around us.        
; Amplitude modulation comes into play whenever we are not listening from the centre of the circle of motion. 
; The close to the edge of the circle we are the greater the amount of amplitude modulation we will experience. 
; If the amplitude modulation is extreme then the circle of the source sound's motion must be extremely large. The phase of the amplitude modulation LFO is also adjustable ('Orientation' slider -   
; this define which edge of the circle we are closest to, e.g. upper, lower, left, right etc. It is probably best to always include at least a small amount of amplitude modulation as we perceive sounds directly to our left or to our right to be
; louder, even if they remain equidistant from us. In this case the amplitude LFO phase ('Orientation') should be 0.5 (radians).                                                  
; The waveform for all three LFOs is a sine wave. 
; This defines the object's motion as being circular. 
; If we were to use a different waveform this would model non-circular motion. 
; There is interesting potential in experimentation in this direction with this example.                                
; Finally as the moving signal becomes more distant, i.e. when the amplitude scaling function is at its minimum, a reverberated version of the signal can become more evident. 
; The degree to which this is present can be scaled using the 'Reverb Scaling' slider.                                    

<Cabbage>
form caption("Doppler Spin"), size(700,265), pluginid("SDop") style("legacy")

label    bounds( 8 , 4, 70, 15), text("i  n  p  u  t"), fontcolour("white")
combobox bounds(10, 30, 60,20), channel("input"), value(1), text("left","right","mixed","test")

line     bounds( 85,  5,  2, 70), colour("Grey")
label    bounds(185,  4, 90, 15), text("r  e  v  e  r  b"), fontcolour("white")
checkbox bounds(100, 33,110, 20), text("Reverb On/Off") channel("RvbOnOff"), FontColour("White"), colour("lime")  value(1)
rslider  bounds(210, 23, 55, 55), channel("RvbScaling"), range(0,1.000,0.3,1,0.001),     text("Scaling"),               TextBox(0), colour( 95, 45,115), trackercolour(white)
rslider  bounds(270, 23, 55, 55), channel("RvbTime"),    range(0.3,0.990,0.7,1,0.001),   text("Time"),                  TextBox(0), colour( 85, 45,125), trackercolour(white)
rslider  bounds(330, 23, 55, 55), channel("RvbFilt"), range(20,20000,4000,0.5,0.001),    text("Damping"),               TextBox(0), colour( 75, 45,135), trackercolour(white)

line     bounds(390,  5,  2, 70), colour("Grey")
label    bounds(410,  4, 90, 15), text("o  u  t  p  u  t"), fontcolour("white")
rslider  bounds(400, 23, 55, 55), channel("mix"),        range(0,1.000,1,1,0.001),       text("Mix"),                   TextBox(0), colour( 65, 45,145), trackercolour(white)
rslider  bounds(460, 23, 55, 55), channel("OutGain"),    range(0,1.000,0.7,1,0.001),     text("Level"),                 TextBox(0), colour( 55, 45,155), trackercolour(white)
line     bounds(520,  5,  2, 70), colour("Grey")

checkbox bounds(530, 33,120, 20), text("Random Speed") channel("RandSpeed"), FontColour("White"), colour("lime")  value(0)

hslider bounds(  5, 80,690, 35), channel("freq"),        range(-10,10.0,0.2,1,0.01),     text("Speed"),         TextBox(1), colour(145, 45, 65), trackercolour(white)	;Frequency of Rotation
hslider bounds(  5,110,690, 35), channel("AmpDepth"),    range(0,1.000,0.7,1,0.001),        text("Loc."),          TextBox(1), colour(135, 45, 75), trackercolour(white)	;Central/Edge
hslider bounds(  5,140,690, 35), channel("AmpPhase"),    range(0,1.000,0.5,1,0.001),        text("Orient."),       TextBox(1), colour(125, 45, 85), trackercolour(white)	;Orientation [radians]
hslider bounds(  5,170,690, 35), channel("PanDepth"),    range(0,1.000,1,1,0.001),          text("Width"),         TextBox(1), colour(115, 45, 95), trackercolour(white)	;Panning Width
hslider bounds(  5,200,690, 35), channel("DopDep"),      range(0,0.030,0.003,1,0.0001), text("Depth"),         TextBox(1), colour(105, 45,105), trackercolour(white)	;Doppler Depth

label   bounds( 5,247, 170, 12), text("Author: Iain McCurdy |2013|"), FontColour("grey")

</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-dm0 -n -+rtmidi=null -M0
</CsOptions>

<CsInstruments>

sr 		= 	44100	;SAMPLE RATE
ksmps 		= 	32	;NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls 		= 	2	;NUMBER OF CHANNELS (2=STEREO)
0dbfs		=	1	;MAXIMUM AMPLITUDE VALUE

gisine	ftgen	0,0,131072,10,1

instr	1
	gkinput      	chnget  "input"  
	kfreq      	chnget	"freq"
	kAmpDepth  	chnget	"AmpDepth"  	
	kAmpPhase  	chnget	"AmpPhase"  	
	kPanDepth  	chnget	"PanDepth"  	
	kDopDep    	chnget	"DopDep"    	
	kRvbScaling	chnget	"RvbScaling"	
	kRvbTime   	chnget	"RvbTime"   	
	kRvbFilt   	chnget	"RvbFilt"   	
	kmix  	 	chnget	"mix"   	
	kOutGain   	chnget	"OutGain"   	
	kRvbOnOff	chnget	"RvbOnOff"
	kRandSpeed   	chnget	"RandSpeed"   	
	
	/* INPUT */
	aL,aR		ins
	if gkinput=1 then
	 asig	=	aL
	elseif gkinput=2 then
	 asig	=	aR
	elseif gkinput=3 then
	 asig	=	(aL+aR)*0.677
	else
 	 ;INPUT TONE=============================================(for testing)
	 asig		vco2	.5, 300		;GENERATE TONE
	 asig		tone	asig, 1200	;LOW PASS FILTER TO SOFTEN THE TONE
	 ;=======================================================
	endif	 

	if kRandSpeed=1 then
	 kfreq	jspline	kfreq,1,4
	endif


	
	kporttime	linseg	0,0.001,0.1			;CREATE 'PORTAMENTO TIME'. A FUNCTION THAT RISES QUICKLY FROM ZERO TO A HELD VALUE.
	kAmpPhase	portk	kAmpPhase, kporttime		;APPLY PORTAMENTO TO gkAmpPhase. CREATE NEW OUTPUT VARIABLE kAmpPhase (GLOBAL VARIABLES CAN'T BE BOTH INPUT AND OUTPUT)
	kDopDep		portk	kDopDep, kporttime		;APPLY PORTAMENTO TO gkDopDep. CREATE NEW OUTPUT VARIABLE kDopDep (GLOBAL VARIABLES CAN'T BE BOTH INPUT AND OUTPUT)

	aAmp 		osciliktp kfreq, gisine, kAmpPhase	;AN LFO DEFINES A VARIABLE USED TO MODULATE AMPLITUDE (NOTE: VARIABLE PHASE). THIS MODELS THE LOCATION WITHIN THE CIRCLE FROM WHICH WE ARE LISTENING. NO AMPLITUDE MODULATION REPRESENT REPRESENTS OUR LISTENING POSITION BEING EXACTLY CENTRAL, MAXIMUM MODULATION REPRESENTS US BEING NEAR TO THE EDGE OF A LARGE CIRCLE. THE CONTROL OVER PHASE OF THIS LFO REPRESENTS WHICH EDGE WE ARE CLOSER TO.
	aAmp		=	(aAmp * 0.5 * kAmpDepth) + 0.5	;RESCALE AND OFFSET AMPLITUDE MODULATION LFO
	
	aPan		oscili	(kPanDepth * 0.5), kfreq, gisine, 0.75	;AN LFO DEFINES A VARIABLE FOR PANNING CONTROL - I.E. WHETHER SOUND IS CURRENTLY TO OUT LEFT OR TO OUT RIGHT. NOTE THAT PHASE IS 0.75 AND THEREFORE 0.75 RADIANS (OR 270 DEGREES OUT OF PHASE) WITH THE DELAY MODULATION 
	aPan		=	aPan + 0.5					;OFFSET PANNING LFO

	iMaxDelay	=	1		;DEFINE A VARIABLE THAT WILL BE USE FOR 'MAXIMUM DELAY TIME' (BUFFER LENGTH)
	aDelTim		oscili	kDopDep, kfreq, gisine, 0	;AN LFO DEFINES A VARIABLE FOR DELAY TIME (NOTE PHASE AT ZERO)
	aDelTim		=	aDelTim + kDopDep		;DELAY TIME  VARIABLE 'aDelay' IS OFFSET TO STAY WITHIN THE POSITIVE DOMAIN	
		
	;vdelayxw IS USED FOR THE DELAY READ/WRITE AS IT MODULATES THE WRITE POINTER RATHER THAN THE READ POINTER.
	;THIS IS MORE APPROPRIATE IN THIS EXAMPLE AS THE SOURCE IS MOVING BUT THE POINT OF LISTENING IS STATIONARY
	aDelTap	vdelayxw	asig, aDelTim, iMaxDelay, 16
	
	aL, aR	pan2 	aDelTap, aPan, 1	;APPLY PANNING TO SIGNAL OUTPUT FROM DELAY USING pan2 OPCODE. CREATE A NEW 

	aL	=	aL * (aAmp^0.5)		;APPLY AMPLITUDE MODULATION (CREATE A NEW AUDIO SIGNAL - DRY (UN-REVERBERATED) SIGNAL)
	aR	=	aR * (aAmp^0.5)		;APPLY AMPLITUDE MODULATION (CREATE A NEW AUDIO SIGNAL - DRY (UN-REVERBERATED) SIGNAL)

	if kRvbOnOff=1 then
	 aRvbL, aRvbR	reverbsc	aL, aR, kRvbTime, kRvbFilt	;REVERB (UNAFFECTED BY AMPLITUDE MODULATION)
	 aL	=	(aL+(aRvbL*kRvbScaling))
	 aR	=	(aR+(aRvbL*kRvbScaling))
	endif
	
	aL	ntrpol	asig,aL,kmix
	aR	ntrpol	asig,aR,kmix
	
	outs	aL * kOutGain , aR * kOutGain 	;SEND AUDIO TO OUTPUTS. MIX DRY AND REVERBERATED SIGNALS.
endin

</CsInstruments>

<CsScore>
i 1 0 [3600*24*7*52]
</CsScore>

</CsoundSynthesizer>; Flanger.csd
; Written by Iain McCurdy, 2012.

<Cabbage>
form caption("Flanger") size(510,100), pluginid("flan") style("legacy")
image pos(0, 0), size(510,100), colour("lightgreen"), shape("rounded"), outlinecolour("white"), outlinethickness(4) 
line	 bounds( 15, 10,190,  2)
label    bounds( 75,  7, 70, 10), text("MODULATION"), fontcolour("white"), colour(100,110,130)
rslider  bounds(  5, 20, 70, 70), text("Rate"),     channel("rate"),  range(0.001, 40, 0.15, 0.5, 0.001),      colour("DarkGreen"), trackercolour( 50,150, 50), textcolour(  0, 30,  0)
rslider  bounds( 75, 20, 70, 70), text("Depth"),    channel("depth"), range(0, 0.01, 0.005,1,0.0001),          colour("DarkGreen"), trackercolour( 50,150, 50), textcolour(  0, 30,  0)
rslider  bounds(145, 20, 70, 70), text("Offset"),    channel("delay"), range(0.00002, 0.1, 0.0001, 0.5, 0.0001), colour("DarkGreen"), trackercolour( 50,150, 50), textcolour(  0, 30,  0)
rslider  bounds(215, 20, 70, 70), text("Feedback"), channel("fback"), range(-1, 1, 0),                         colour("DarkGreen"), trackercolour( 50,150, 50), textcolour(  0, 30,  0)
checkbox bounds(290, 20, 80, 15), colour("yellow"), channel("ThruZero"),  value(1), text("Thru.Zero"), fontcolour(  0, 30,  0)
label    bounds(295, 47, 65, 12), text("LFO Shape:"), fontcolour(  0, 30,  0)
combobox bounds(290, 60, 80, 18), channel("lfoshape"), value(1), text("parabola", "sine", "triangle", "randomi", "randomh")
rslider  bounds(370, 20, 70, 70), text("Mix"),      channel("mix"),   range(0, 1.00, 0.5),                     colour("DarkGreen"), trackercolour( 50,150, 50), textcolour(  0, 30,  0)
rslider  bounds(435, 20, 70, 70), text("Level"), channel("level"), range(0, 1.00, 1), colour("DarkGreen"), trackercolour( 50,150, 50), textcolour(  0, 30,  0)
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

;Author: Iain McCurdy (2012)

opcode	Flanger,a,akkkkk
	ain,krate,kdepth,kdelay,kfback,klfoshape	xin			;read in input arguments
	iparabola	ftgentmp	0, 0, 131072, 19, 0.5, 1, 180, 1	;u-shape parabola for lfo
	isine		ftgentmp	0, 0, 131072, 19, 1, 0.5, 0,   0.5 	;sine-shape for lfo
	itriangle	ftgentmp	0, 0, 131072, 7, 0,131072/2,1,131072/2,0;triangle-shape for lfo
	adlt		interp		kdelay							;a new a-rate variable 'adlt' is created by interpolating the k-rate variable 'kdlt'
	if klfoshape==1 then
	 amod		oscili		kdepth, krate, iparabola			;oscillator that makes use of the positive domain only u-shape parabola
	elseif klfoshape==2 then
	 amod		oscili		kdepth, krate, isine			;oscillator that makes use of the positive domain only sine wave
	elseif klfoshape==3 then
	 amod		oscili		kdepth, krate, itriangle			;oscillator that makes use of the positive domain only triangle
	elseif klfoshape==4 then	
	 amod		randomi		0,kdepth,krate,1
	else	
	 amod		randomh		0,kdepth,krate,1
	endif
	adlt		sum		adlt, amod				;static delay time and modulating delay time are summed
	adelsig		flanger 	ain, adlt, kfback , 1.2			;flanger signal created
	adelsig		dcblock		adelsig
	aout		sum		ain*0.5, adelsig*0.5			;create dry/wet mix 
			xout		aout					;send audio back to caller instrument
endop

instr 1
	krate chnget "rate"				;read in widgets
	kdepth chnget "depth"
	kdelay chnget "delay"
	kfback chnget "fback"
	klevel chnget "level"
	klfoshape chnget "lfoshape"
	kThruZero chnget "ThruZero"
	kmix chnget "mix"
	
	a1,a2	ins					;read live stereo audio input
	;a1	pinkish	0.2				;for testing...
	;a2	pinkish	0.2

	kporttime	linseg	0,0.001,0.1
	kdelay	portk	kdelay,kporttime

	afla1	Flanger	a1,krate,kdepth,kdelay,kfback,klfoshape	;call udo (left channel)
	afla2	Flanger	a2,krate,kdepth,kdelay,kfback,klfoshape	;call udo (right channel)

	if kThruZero==1 then				;if 'Thru.Zero' mode is selected...
	 a1	delay	a1,0.00002
	 a2	delay	a2,0.00002
	 a1	ntrpol	-a1,afla1,kmix			;invert delayed dry signal and mix with flanger signal
	 a2	ntrpol	-a2,afla2,kmix
	else						;otherwise... (standard flanger)
	 a1	ntrpol	a1,afla1,kmix			;create mix between dry signal and flanger signal
	 a2	ntrpol	a2,afla2,kmix
	endif
		outs	a1*klevel,a2*klevel		;send audio to outputs, scale amplitude according to GUI knob value
endin

</CsInstruments>

<CsScore>
i 1 0 [60*60*24*7]
</CsScore>

</CsoundSynthesizer>; LeslieSpeaker.csd
; Written by Iain McCurdy, 2016.

; An emulation of a Leslie rotating speaker as commonly used with a Hammond organ

; A Leslie speaker consists of two speaker drivers, a high frequency tweeter and a low frequency woofer, which rotate at on of two speed according to a switch.
;  (For clarification, the woofer itself does not actually spin, instead a baffle rotates around it.)

; The main interface is simply two main switches which switch between the slow and fast speeds, and a 'setup' button which accesses advanced controls.
; The default advanced settings represent typical characteristics for a real Leslie speaker.

; This .csd works as an effect upon the audio input, but it can also be tested by playing a basic built-in organ using a connected MIDI keyboard

; Setup
; -----
; The same set of parameters are provided for the woofer and the tweeter.

; Cutoff	-	the cutoff frequency of the crossover (lowpass for the woofer, highpass for the tweeter)
; Damping	-	change from one speed to the other is not instantaneous.
;				'Damping' defines the amount of mechanical resistance to changing speed.
;				Typically the woofer takes longer to change speed (higher damping).
; RPM Slow	-	speed (in revolutions per minute) when speed is 'Slow'
; RPM Fast	-	speed (in revolutions per minute) when speed is 'Fast'
; Doppler	-	amount of doppler shift (pitch shift) during rotation
; Panning	-	width of panning modulation
; Amplitude	-	amount of amplitude modulation

<Cabbage>
form caption("Leslie Speaker") size(260,120), pluginid("Lsli") style("legacy")

image     bounds(  0,  0,260,120), outlinethickness(4), outlinecolour("white"), file("DarkWood.jpg");, colour(75,50,50)
image     bounds(  0,  0,260,120), outlinethickness(4), outlinecolour("silver"), colour(0,0,0,0)
button    bounds( 40, 85, 60, 25), fontcolour:0(80,55,55), fontcolour:1(255,245,245), colour:0(75,50,50), colour:1(250,170,170), text("Slow","Slow"), channel("Slow"), latched(1), radiogroup(1), value(1)
button    bounds(100, 85, 60, 25), fontcolour:0(80,55,55), fontcolour:1(255,245,245), colour:0(75,50,50), colour:1(250,170,170), text("Stop","Stop"), channel("Stop"), latched(1), radiogroup(1)
button    bounds(160, 85, 60, 25), fontcolour:0(80,55,55), fontcolour:1(255,245,245), colour:0(75,50,50), colour:1(250,170,170), text("Fast","Fast"), channel("Fast"), latched(1), radiogroup(1)

image     bounds( 132,  7,16,16), identchannel("TweeterID"), shape("ellipse"), colour(170,150,150)
image     bounds( 110, 30,40,40), identchannel("WooferID"),  shape("ellipse"), colour(170,150,150)

button bounds(200,10,50, 18), colour:0(80,55,55), colour:1(80,55,55), channel("setup_Button"), text("SETUP"), fontcolour:0(200,200,200), fontcolour:1(200,200,200)
groupbox bounds(360,28,340,190), plant("Setup"), outlinethickness(0), popup(1), identchannel("setupPlant")
{
image     bounds(0,0,340,190), outlinethickness(4), outlinecolour("silver"), colour(75,50,50), file("DarkWood.jpg")

image     bounds(0,0,170,190), outlinethickness(4), outlinecolour("silver"), colour(0,0,0,0)
label     bounds(0,4,170, 14), text("Woofer"), fontcolour(200,200,200)
nslider bounds(10, 20,70,40), channel("CF_Low"), text("Cutoff"),     range(20,8000,800,1,1)
nslider bounds(90, 20,70,40), channel("LF_Damp"), text("Damping"),   range(0.01,8,1.5,1,0.01)
nslider bounds(10, 60,70,40), channel("LF_Slow"), text("RPM Slow"),  range(5,1000, 40,1,1)
nslider bounds(90, 60,70,40), channel("LF_Fast"), text("RPM Fast"),  range(5,1000,342,1,1)
nslider bounds(10,100,70,40), channel("LF_Dop"), text("Doppler"),    range(0,2,0.1)
nslider bounds(90,100,70,40), channel("LF_Pan"), text("Panning"),    range(0,1,0.2,1,0.01)
nslider bounds(10,140,70,40), channel("LF_Amp"), text("Amplitude"),  range(0,1,0.3,1,0.01)

image     bounds(170,0,170,190), outlinethickness(4), outlinecolour("silver"), colour(0,0,0,0)
label     bounds(170,4,170, 14), text("Tweeter"), fontcolour(200,200,200)
nslider bounds(180, 20,70,40), channel("CF_High"), text("Cutoff"),    range(20,8000,800,1,1)
nslider bounds(260, 20,70,40), channel("HF_Damp"), text("Damping"),   range(0.01,8,0.5,1,0.01)
nslider bounds(180, 60,70,40), channel("HF_Slow"), text("RPM Slow"),  range(5,1000, 48,1,1)
nslider bounds(260, 60,70,40), channel("HF_Fast"), text("RPM Fast"),  range(5,1000,400,1,1)
nslider bounds(180,100,70,40), channel("HF_Dop"), text("Doppler"),    range(0,2,0.2)
nslider bounds(260,100,70,40), channel("HF_Pan"), text("Panning"),    range(0,1,0.4,1,0.01)
nslider bounds(180,140,70,40), channel("HF_Amp"), text("Amplitude"),  range(0,1,0.3,1,0.01)
}

</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-dm0 -n -+rtmidi=null -M0
</CsOptions>

<CsInstruments>

sr 		= 	44100
ksmps 	= 	32
nchnls 	= 	2
0dbfs	=	1
massign	0,1

;Author: Iain McCurdy (2016)

opcode	DopplerSpin,aa,akkkkki
	asig,kfreq,kAmpDepth,kAmpPhase,kPanDepth,kDopDep,ishape	xin
	
	isine		ftgen	0,0,1024,10,1
	aAmp 		osciliktp kfreq, ishape, kAmpPhase		; AN LFO DEFINES A VARIABLE USED TO MODULATE AMPLITUDE (NOTE: VARIABLE PHASE). THIS MODELS THE LOCATION WITHIN THE CIRCLE FROM WHICH WE ARE LISTENING. NO AMPLITUDE MODULATION REPRESENT REPRESENTS OUR LISTENING POSITION BEING EXACTLY CENTRAL, MAXIMUM MODULATION REPRESENTS US BEING NEAR TO THE EDGE OF A LARGE CIRCLE. THE CONTROL OVER PHASE OF THIS LFO REPRESENTS WHICH EDGE WE ARE CLOSER TO.
	aAmp		=	(aAmp * 0.5 * kAmpDepth) + 0.5		; RESCALE AND OFFSET AMPLITUDE MODULATION LFO
	
	aPan		oscili	(kPanDepth * 0.5), kfreq, ishape, 0.75	; AN LFO DEFINES A VARIABLE FOR PANNING CONTROL - I.E. WHETHER SOUND IS CURRENTLY TO OUT LEFT OR TO OUT RIGHT. NOTE THAT PHASE IS 0.75 AND THEREFORE 0.75 RADIANS (OR 270 DEGREES OUT OF PHASE) WITH THE DELAY MODULATION 
	aPan		=	aPan + 0.5				; OFFSET PANNING LFO

	iMaxDelay	=	4					; DEFINE A VARIABLE THAT WILL BE USE FOR 'MAXIMUM DELAY TIME' (BUFFER LENGTH)
	aDelTim		oscili	kDopDep, kfreq, ishape, 0		; AN LFO DEFINES A VARIABLE FOR DELAY TIME (NOTE PHASE AT ZERO)
	aDelTim		=	aDelTim + kDopDep			; DELAY TIME  VARIABLE 'aDelay' IS OFFSET TO STAY WITHIN THE POSITIVE DOMAIN	
		
	;vdelayxw IS USED FOR THE DELAY READ/WRITE AS IT MODULATES THE WRITE POINTER RATHER THAN THE READ POINTER.
	;THIS IS MORE APPROPRIATE IN THIS EXAMPLE AS THE SOURCE IS MOVING BUT THE POINT OF LISTENING IS STATIONARY
	aDelTap		vdelay	asig*aAmp, aDelTim, iMaxDelay;, 16
	
	aL, aR	pan2 	aDelTap, aPan, 1	;APPLY PANNING TO SIGNAL OUTPUT FROM DELAY USING pan2 OPCODE. CREATE A NEW 

		xout	aL,aR	
endop


gisine		ftgen	0,0,4096,10,1													; sine wave
gitri	ftgen	0,0,1024,-7,0,512,1,512,0											; triangle wave

giSrc		ftgen	0,0,4096,10,1,0.5,0.4,0.01,0.01,0.01,0.01,0,0.01				; waveform of a single tonewheel (not quite a sine wave)
;giAllOut	ftgen	0,0,4096,31, giSrc, 1,1,0, 2,1,0, 3,1,0, 4,1,0, 8,1,0	; composite of several tonewheels
giAllOut	ftgen	0,0,4096,9, 1,1,0, 2,1,0, 3,1,0, 4,1,0, 5,.1,0, 6,.1,0, 7,.1,0, 8,1,0	; composite of several tonewheels

instr	1	; a basic MIDI triggered organ sound
 aEnv		linsegr	0,0.005,1,0.01,0					; anti-click envelope
 aMix		poscil	0.5*aEnv,cpsmidi(),giAllOut			; basic organ sound
 chnmix		aMix,"send"									; mix into send 'channel'
endin

instr	99	; a Leslie speaker effect
 kSlow	chnget	"Slow"									; read 'Slow' button widget
 kStop	chnget	"Stop"									; read 'Stop' button widget
 kFast	chnget	"Fast"									; read 'Faset' button widget
 
 if kSlow==1 then
  kSpeed	=	0
 elseif kStop==1 then
  kSpeed	=	1
 else
  kSpeed	=	2
 endif
 														; 0=slow 1=stop 2=fast
 aMix		chnget	"send"								; read in organ sound from instr 1

 aL,aR		ins											; read live audio in
 aMix		+=	aL+aR									; mix both live audio in channels into the organ sound
 
 kCF_High	chnget	"CF_High"							;crossover cutoff frequencies
 kCF_Low	chnget	"CF_Low"
 
 kHF_Slow	=	chnget:k("HF_Slow")/60					; tweeter chorale (slow) speed
 kHF_Fast	=	chnget:k("HF_Fast")/60					; tweeter tremolo (fast) speed

 kLF_Slow	=	chnget:k("LF_Slow")/60					; tweeter chorale (slow) speed
 kLF_Fast	=	chnget:k("LF_Fast")/60					; woofer tremolo (fast) speed
 
 kHF_Damp	chnget	"HF_Damp"							; tweeter speed change time
 kLF_Damp	chnget	"LF_Damp"							; woofer speed change time
 
 kHF_Trem	chnget	"HF_Trem"							; tremolo modulation depth
 kLF_Trem	chnget	"LF_Trem"
 
 kHF_Dop	chnget	"HF_Dop"							; doppler modulation depth
 kLF_Dop	chnget	"LF_Dop"
 
 kHF_Pan	chnget	"HF_Pan"							; panning modulation depth
 kLF_Pan	chnget	"HF_Pan"
 
 kHF_Amp	chnget	"HF_Amp"							; amplitude modulation depth
 kLF_Amp	chnget	"LF_Amp"

 aLF		butlp	aMix,kCF_Low						; woofer audio signal
 aHF		buthp	aMix,kCF_High						; tweeter audio signal

 if kSpeed==0 then										; if speed is slow
  kLF_Speed	=	kLF_Slow								; set woofer speed to slow speed as defined in the set-up pop-up
  kHF_Speed	=	kHF_Slow								; set tweeter speed to slow speed as defined in the set-up pop-up
 elseif kSpeed==2 then
  kLF_Speed	=	kLF_Fast								; set woofer speed to slow speed as defined in the set-up pop-up
  kHF_Speed	=	kHF_Fast								; set tweeter speed to slow speed as defined in the set-up pop-up
 else
  kLF_Speed	=	0										; set woofer speed to slow speed as defined in the set-up pop-up
  kHF_Speed	=	0										; set tweeter speed to slow speed as defined in the set-up pop-up
 endif
 
 kportramp	linseg	0,0.01,1							; portamento time ramps up quickly from zero
 kLF_Speed	portk	kLF_Speed,kLF_Damp					; damp speed change from 
 kHF_Speed	portk	kHF_Speed,kHF_Damp

 aHF_L,aHF_R	DopplerSpin	aHF,kHF_Speed,kHF_Amp,0.5,kHF_Pan,kHF_Dop,gisine	; call UDO
 aLF_L,aLF_R	DopplerSpin	aLF,kLF_Speed,kLF_Amp,0.5,kLF_Pan,kLF_Dop,gisine
 
		outs	aHF_L,aHF_R
 		outs	aLF_L,aLF_R

 ; GRAPHICAL ROTATING SPEAKERS 
 kPhsTweeter	poscil	1,kHF_Speed,gitri				
 kPhsWoofer	poscil	1,kLF_Speed,gitri
 ktrig	metro	16
 if ktrig==1 then
  Smsg	sprintfk	"bounds(%d,10,%d,16)",122+(8*(1-kPhsTweeter)),kPhsTweeter*16
  	chnset		Smsg,"TweeterID"
  Smsg	sprintfk	"bounds(%d,33,%d,40)",110+(20*(1-kPhsWoofer)),kPhsWoofer*40
  	chnset		Smsg,"WooferID"
 endif 
 		
 		chnclear	"send" 								; clear audio send channel (organ)
endin


instr 1000				; launches plant popups
 klaunch	init		0
 #define LAUNCH_PLANT(name)
 #
 kpressed	chnget 		"$name._Button"
 if changed(kpressed)==1 then
   Smsg 	sprintfk 	"show(%d), pos(1, 19)", klaunch
   		chnset 		Smsg, "$name.Plant"
 endif
 #
 $LAUNCH_PLANT(setup)
 klaunch	=		1
endin

</CsInstruments>

<CsScore>
i 99   0 [3600*24*7]
i 1000 0 [3600*24*7]
</CsScore>

</CsoundSynthesizer>; PhaserLFO.csd
; Written by Iain McCurdy, 2012.

<Cabbage>
form caption("Phaser LFO") size(580, 95), pluginid("phsr") style("legacy")
image          bounds(0, 0, 580, 95), colour( 100, 110, 130), shape("rounded"), outlinecolour("white"), outlinethickness(4)
label    bounds(10,  8, 75, 10), text("INPUT"), fontcolour(0,10,30)
combobox bounds(10, 18, 75, 20), channel("input"), value(1), text("Live","Tone","Noise")
label    bounds(10, 43, 75, 10), text("LFO SHAPE"), fontcolour(0,10,30)
combobox bounds(10, 53, 75, 20), channel("shape"), value(1), text("Triangle","Sine","Square","Saw Up","Saw Down","Rand.Int","Rand.S&H")
line	bounds( 95, 10,190,  2)
label   bounds(155,  7, 70, 10), text("MODULATION"), fontcolour("white"), colour(100,110,130)
rslider bounds( 85, 20, 70, 70), text("Rate"), 		channel("rate"), 	range(0,100.00,0.5,0.5, 0.0001),    colour(50,40,110), textcolour(0,10,30), trackercolour(255,255,200), midiCtrl(1,1)
rslider bounds(155, 20, 70, 70), text("Depth"), 	channel("depth"), 	range(0, 1.00, 0.5, 1, .01),       colour(50,40,110), textcolour(0,10,30), trackercolour(255,255,200)
rslider bounds(225, 20, 70, 70), text("Offset"), 	channel("freq"), 	range(0, 1.00, 0.4, 1, .01),       colour(50,40,110), textcolour(0,10,30), trackercolour(255,255,200)
rslider bounds(295, 20, 70, 70), text("Feedback"), 	channel("fback"), 	range(0, 1.00, 0.4, 1, .01),       colour(50,40,110), textcolour(0,10,30), trackercolour(255,255,200)
rslider bounds(365, 20, 70, 70), text("Stages"),	channel("stages"), 	range(1, 64,8, 1, 1),              colour(50,40,110), textcolour(0,10,30), trackercolour(255,255,200)
rslider bounds(435, 20, 70, 70), text("Mix"), 	        channel("mix"), 	range(0, 1.00,0.5, 1, .01),        colour(50,40,110), textcolour(0,10,30), trackercolour(255,255,200)
rslider bounds(505, 20, 70, 70), text("Level"), 	channel("level"), 	range(0, 1.00, 1, 1, .01),         colour(50,40,110), textcolour(0,10,30), trackercolour(255,255,200)
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

;Author: Iain McCurdy (2012)
;http://iainmccurdy.org/csound.html


opcode	Phaser,a,akkkKki
	ain,krate,kdepth,kfreq,kstages,kfback,ishape	xin		;READ IN INPUT ARGUMENTS
	if ishape=1 then
	 klfo	lfo	kdepth*0.5, krate, 1				;LFO FOR THE PHASER (TRIANGULAR SHAPE)
	elseif ishape=2 then
	 klfo	lfo	kdepth*0.5, krate, 0				;LFO FOR THE PHASER (SINE SHAPE)
	elseif ishape=3 then
	 klfo	lfo	kdepth*0.5, krate, 2				;LFO FOR THE PHASER (SQUARE SHAPE)
	elseif ishape=4 then
	 klfo	lfo	kdepth*0.5, krate, 4				;LFO FOR THE PHASER (SAWTOOTH)
	elseif ishape=5 then
	 klfo	lfo	kdepth*0.5, krate, 5				;LFO FOR THE PHASER (SAWTOOTH)
	elseif ishape=6 then
	 klfo	randomi	-kdepth*0.5, kdepth*0.5, krate*8		;LFO FOR THE PHASER (RANDOMI SHAPE)
	 klfo	portk	klfo, 1/(krate*8)				;SMOOTH CHANGES OF DIRECTION
	elseif ishape=7 then
	 klfo	randomh	-kdepth*0.5, kdepth*0.5, krate			;LFO FOR THE PHASER (RANDOMH SHAPE)
	endif	
	aout	phaser1	ain, cpsoct((klfo+(kdepth*0.5)+kfreq)), kstages, kfback	;PHASER1 IS APPLIED TO THE INPUT AUDIO
		xout	aout							;SEND AUDIO BACK TO CALLER INSTRUMENT
endop

opcode	PhaserSt,aa,aakkkKki
	ainL,ainR,krate,kdepth,kfreq,kstages,kfback,ishape	xin		;READ IN INPUT ARGUMENTS
	if ishape=1 then
	 klfo	lfo	kdepth*0.5, krate, 1				;LFO FOR THE PHASER (TRIANGULAR SHAPE)
	elseif ishape=2 then
	 klfo	lfo	kdepth*0.5, krate, 0				;LFO FOR THE PHASER (SINE SHAPE)
	elseif ishape=3 then
	 klfo	lfo	kdepth*0.5, krate, 2				;LFO FOR THE PHASER (SQUARE SHAPE)
	elseif ishape=4 then
	 klfo	lfo	kdepth*0.5, krate, 4				;LFO FOR THE PHASER (SAWTOOTH)
	elseif ishape=5 then
	 klfo	lfo	kdepth*0.5, krate, 5				;LFO FOR THE PHASER (SAWTOOTH)
	elseif ishape=6 then
	 klfo	randomi	-kdepth*0.5, kdepth*0.5, krate*8		;LFO FOR THE PHASER (RANDOMI SHAPE)
	 klfo	portk	klfo, 1/(krate*8)				;SMOOTH CHANGES OF DIRECTION
	elseif ishape=7 then
	 klfo	randomh	-kdepth*0.5, kdepth*0.5, krate			;LFO FOR THE PHASER (RANDOMH SHAPE)
	endif
	aoutL	phaser1	ainL, cpsoct((klfo+(kdepth*0.5)+kfreq)), kstages, kfback	;PHASER1 IS APPLIED TO THE INPUT AUDIO
	aoutR	phaser1	ainR, cpsoct((klfo+(kdepth*0.5)+kfreq)), kstages, kfback	;PHASER1 IS APPLIED TO THE INPUT AUDIO
		xout	aoutL,aoutR							;SEND AUDIO BACK TO CALLER INSTRUMENT
endop

		instr 1
kporttime	linseg		0,0.001,0.05
krate		chnget		"rate"
kdepth 		chnget 		"depth"
kdepth		portk		kdepth,kporttime
kfreq 		chnget 		"freq"
kfreq		portk		kfreq,kporttime
kfback 		chnget 		"fback"
kstages 	chnget 		"stages"
klevel 		chnget 		"level"
kmix		chnget		"mix"
kshape		chnget		"shape"
kshape		init		1

/* INPUT */
kinput		chnget	"input"
if kinput=1 then
 a1,a2		ins
elseif kinput=2 then
 a1		vco2		0.1,200
 a2		=		a1
else
 a1		pinkish		0.1
 a2		pinkish		0.1
endif

ktrig		changed		kshape,kstages							; reinitialise for i-rate parms
if ktrig=1 then
 reinit 	RESTART_PHASER
endif
RESTART_PHASER:
;aPhs1		Phaser		a1,krate,kdepth*8,(kfreq*5)+6,kstages,kfback*0.9,i(kshape)	; call UDO
;aPhs2		Phaser		a2,krate,kdepth*8,(kfreq*5)+6,kstages,kfback*0.9,i(kshape)
aPhs1,aPhs2	PhaserSt	a1,a2,krate,kdepth*8,(kfreq*10)+4,kstages,kfback*0.9,i(kshape)	; use stereo version to ensure lfo sync for random lfos

rireturn
a1		ntrpol		a1,aPhs1,kmix							; wet/dry mix
a2		ntrpol		a2,aPhs2,kmix
		outs		a1* klevel, a2* klevel
		endin

</CsInstruments>

<CsScore>
i 1 0 [60*60*24*7]
</CsScore>

</CsoundSynthesizer>; StereoChorus.csd
; Written by Iain McCurdy, 2012.

; Two LFO modes are offered, sines and rsplines.
; 

; Dereg. (deregulate) adds a random modulation to both LFO rate and depth in both LFO modes

<Cabbage>
form caption("Stereo Chorus") size(595, 100), pluginid("StCh") style("legacy")
image                 bounds(0, 0, 595, 100), colour("DarkSlateGrey"), shape("rounded"), outlinecolour("white"), outlinethickness(6)
label    bounds( 15, 22, 75, 12), text("LFO Type:"), fontcolour("white")
combobox bounds( 15, 35, 75, 20), text("Sine","RSpline"), channel("type"), textcolour("white"), colour( 7,29,29), fontcolour("white")
rslider  bounds( 90, 13, 75, 75), text("Rate"), channel("rate"), range(0.001, 7, 0.5,0.5),  textcolour("white"), colour(37,59,59), trackercolour("Silver")
rslider  bounds(160, 13, 75, 75), text("Dereg"), channel("dereg"), range(0, 4, 0,0.5,0.01), textcolour("white"), colour(37,59,59), trackercolour("Silver")
rslider  bounds(230, 13, 75, 75), text("Depth"), channel("depth"), range(0, 1.00, 0.2),     textcolour("white"), colour(37,59,59), trackercolour("Silver")
rslider  bounds(300, 13, 75, 75), text("Offset"), channel("offset"), range(0.0001,0.1,0.001,0.5,0.0001),     textcolour("white"), colour(37,59,59), trackercolour("Silver")
rslider  bounds(370, 13, 75, 75), text("Width"), channel("width"), range(0, 1.00, 0.75),    textcolour("white"), colour(37,59,59), trackercolour("Silver")
rslider  bounds(440, 13, 75, 75), text("Mix"), channel("mix"), range(0, 1.00, 0.5),      textcolour("white"), colour(37,59,59), trackercolour("Silver")
rslider  bounds(510, 13, 75, 75), text("Level"), channel("level"), range(0, 1.00, 1),      textcolour("white"), colour(37,59,59), trackercolour("Silver")
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

;Author: Iain McCurdy (2012)
;http://iainmccurdy.org/csound.html

opcode	StChorus,aa,aakkakk
	ainL,ainR,krate,kdepth,aoffset,kwidth,kmix	xin			;READ IN INPUT ARGUMENTS
	ilfoshape	ftgentmp	0, 0, 131072, 19, 1, 0.5, 0,  0.5	;POSITIVE DOMAIN ONLY SINE WAVE
	kporttime	linseg	0,0.001,0.02					;RAMPING UP PORTAMENTO VARIABLE
	kChoDepth	portk	kdepth*0.01, kporttime				;SMOOTH VARIABLE CHANGES WITH PORTK
	aChoDepth	interp	kChoDepth					;INTERPOLATE TO CREATE A-RATE VERSION OF K-RATE VARIABLE
	amodL 		osciliktp 	krate, ilfoshape, 0			;LEFT CHANNEL LFO
	amodR 		osciliktp 	krate, ilfoshape, kwidth*0.5		;THE PHASE OF THE RIGHT CHANNEL LFO IS ADJUSTABLE
	amodL		=		(amodL*aChoDepth)+aoffset			;RESCALE AND OFFSET LFO (LEFT CHANNEL)
	amodR		=		(amodR*aChoDepth)+aoffset			;RESCALE AND OFFSET LFO (RIGHT CHANNEL)
	aChoL		vdelay	ainL, amodL*1000, 1.2*1000			;CREATE VARYING DELAYED / CHORUSED SIGNAL (LEFT CHANNEL) 
	aChoR		vdelay	ainR, amodR*1000, 1.2*1000			;CREATE VARYING DELAYED / CHORUSED SIGNAL (RIGHT CHANNEL)
	aoutL		ntrpol 	ainL*0.6, aChoL*0.6, kmix			;MIX DRY AND WET SIGNAL (LEFT CHANNEL)
	aoutR		ntrpol 	ainR*0.6, aChoR*0.6, kmix			;MIX DRY AND WET SIGNAL (RIGHT CHANNEL)
			xout	aoutL,aoutR					;SEND AUDIO BACK TO CALLER INSTRUMENT
endop


giPow3  ftgen 1,0,2048,10,1			; table for storage of functions
iftlen =       ftlen(giPow3)			; length of the function table
icount	=	0				; reset counter. (Moves for each index of the function table)
loop3:						; loop beginning                                            
ix	=	((icount/iftlen) * 2) -1        ; shift x range to -1 to 1                                  
iy	=	ix ^ 3                          ; apply formula to derive y                                 
	tableiw iy,icount,giPow3                ; write y value to table                                    
loop_lt,icount,1,iftlen,loop3                   ; loop back and increment counter if we are not yet finished

opcode	StChorusRspline,aa,aakkkkkk
	ainL,ainR,krate,kdereg,kdepth,koffset,kwidth,kmix	xin		;READ IN INPUT ARGUMENTS
	kporttime	linseg	0,0.001,0.02					;RAMPING UP PORTAMENTO VARIABLE
	kChoDepth	portk	kdepth*0.01, kporttime				;SMOOTH VARIABLE CHANGES WITH PORTK	
	kmod1		rspline	koffset,kChoDepth, krate*4+0.01, ((krate*4*kdereg)+0.01)
	kmod2		rspline	kChoDepth,koffset, krate*4+0.01, ((krate*4*kdereg)+0.01)
	kmod1		limit	kmod1,0.0001,1.2
	kmod2		limit	kmod2,0.0001,1.2
	amod1		interp	kmod1	
	amod2		interp	kmod2
	aCho1		vdelay	ainL, amod1*1000, 1.2*1000			;CREATE VARYING DELAYED / CHORUSED SIGNAL (LEFT CHANNEL) 
	aCho2		vdelay	ainR, amod2*1000, 1.2*1000			;CREATE VARYING DELAYED / CHORUSED SIGNAL (RIGHT CHANNEL)
	kpan		rspline	0,1,krate,2*krate*kdereg			;PANNING
	kpan		limit	kpan,0,1
	apan		interp	kpan
	aChoL		=	(aCho1*apan)+(aCho2*(1-apan))
	aChoR		=	(aCho2*apan)+(aCho1*(1-apan))
	aChoL		ntrpol	aChoL,aCho1,kwidth				;WIDTH PROCESSING BETWEEN AUTO-PANNED AND HARD-PANNED
	aChoR		ntrpol	aChoR,aCho2,kwidth
	aoutL		ntrpol 	ainL*0.6, aChoL*0.6, kmix			;MIX DRY AND WET SIGNAL (LEFT CHANNEL)
	aoutR		ntrpol 	ainR*0.6, aChoR*0.6, kmix			;MIX DRY AND WET SIGNAL (RIGHT CHANNEL)
			xout	aoutL,aoutR					;SEND AUDIO BACK TO CALLER INSTRUMENT
endop

instr 1
 kporttime	linseg	0,0.001,0.05                                                     
 krate chnget "rate"
 kdereg chnget "dereg"
 kdepth chnget "depth"
 koffset chnget "offset"
 kwidth chnget "width"
 klevel chnget "level"
 kmix  	chnget "mix"
 ktype	chnget	"type"
 ktype	init	1
 kmix	portk	kmix,kporttime
 klevel	portk	klevel,kporttime
 koffset	portk	koffset,kporttime*0.5
 aoffset	interp	koffset
 
 a1,a2	ins
 ;a1	pinkish	0.1
 ;a2	pinkish	0.1
 ;a1	vco2	0.1,50,4,0.5
 ;a2	vco2	0.1,50,4,0.5
 ;a1	poscil	0.1,100
 ;a2	poscil	0.1,100
 
 
 if ktype==1 then
  kdereg	rspline	-kdereg, kdereg, 0.1, 0.5
  ktrem	rspline	0,-1,0.1,0.5
  ktrem	pow	2,ktrem
  a1,a2	StChorus	a1,a2,krate*octave(kdereg),kdepth*ktrem,aoffset,kwidth,kmix
 else
  a1,a2	StChorusRspline	a1,a2,krate,kdereg,kdepth,koffset,kwidth,kmix
 endif
 a1	=	a1 * klevel
 a2	=	a2 * klevel
 	outs	a1,a2
endin

</CsInstruments>

<CsScore>                                              
i 1 0 [60*60*24*7]
</CsScore>

</CsoundSynthesizer>                                                  ; Ball_in_a_Box.csd
; Written by Iain McCurdy, 2015

; Ball in a Box is a physical model reverb based on the notional idea of a ball (sound) within a bax (reverberant space)

; Room Size (X, Y, Z)		-	room size in metres
; Source Location (X, Y, Z)	-	location of the sound as a ratio 0 to 1 of the entire space
; Receive Location (X, Y, Z)	-	receiver location - in metres - from the centre of the space
; Reverb Decay			-	main decay of the resonator (default: 0.99)
; High Frequency Diffusion	-	is the coefficient of diffusion at the walls, which regulates the amount of diffusion (0-1, where 0 = no diffusion, 1 = maximum diffusion - default: 1)
; Direct Signal Attenuation	-	the attenuation of the direct signal (0-1, default: 0.5)
; Early Reflection Diffusion	-	the attenuation coefficient of the early reflections (0-1, default: 0.8)
; Pick-up Separation		-	the distance in meters between the two pickups (your ears, for example - default: 0.3)

<Cabbage>
form caption("Ball in a Box") size(800,395), pluginid("BABO"), colour(100,100,120) style("legacy")

image   bounds(  3,  5,394,125), outlinecolour("white"), outlinethickness(1), colour(0,0,0,0), plant("RoomSize") {
hslider bounds(  5, 10,390, 30), textcolour(white), channel("rx"), range(0.1,20.00, 5), colour(220,220,250), trackercolour(240,210,170)
label   bounds(  5, 33,390, 12), text("Room Size X"), fontcolour(200,200,200)
hslider bounds(  5, 45,390, 30), textcolour(white), channel("ry"), range(0.1,20.00, 6), colour(220,220,250), trackercolour(240,210,170)
label   bounds(  5, 68,390, 12), text("Room Size Y"), fontcolour(200,200,200)
hslider bounds(  5, 80,390, 30), textcolour(white), channel("rz"), range(0.1,20.00, 4), colour(220,220,250), trackercolour(240,210,170)
label   bounds(  5,103,390, 12), text("Room Size Z"), fontcolour(200,200,200)
}

image   bounds(  3,135,394,125), outlinecolour("white"), outlinethickness(1), colour(0,0,0,0), plant("SourceLocation") {
hslider bounds(  5, 10,390, 30), textcolour(white), channel("srcx"), range(0,1.00, 0.131), colour(220,220,250), trackercolour(240,210,170)
label   bounds(  5, 33,390, 12), text("Source Location X"), fontcolour(200,200,200)
hslider bounds(  5, 45,390, 30), textcolour(white), channel("srcy"), range(0,1.00, 0.243), colour(220,220,250), trackercolour(240,210,170)
label   bounds(  5, 68,390, 12), text("Source Location Y"), fontcolour(200,200,200)
hslider bounds(  5, 80,390, 30), textcolour(white), channel("srcz"), range(0,1.00, 0.717), colour(220,220,250), trackercolour(240,210,170)
label   bounds(  5,103,390, 12), text("Source Location Z"), fontcolour(200,200,200)
}

image   bounds(  3,265,394,125), outlinecolour("white"), outlinethickness(1), colour(0,0,0,0), plant("ReceiveLocation") {
hslider bounds(  5, 10,390, 30), textcolour(white), channel("rcvx"), range(-10,10,7.331), colour(220,220,250), trackercolour(240,210,170)
label   bounds(  5, 33,390, 12), text("Receive Location X"), fontcolour(200,200,200)
hslider bounds(  5, 45,390, 30), textcolour(white), channel("rcvy"), range(-10,10,-3.973), colour(220,220,250), trackercolour(240,210,170)
label   bounds(  5, 68,390, 12), text("Receive Location Y"), fontcolour(200,200,200)
hslider bounds(  5, 80,390, 30), textcolour(white), channel("rcvz"), range(-10,10, 6.791), colour(220,220,250), trackercolour(240,210,170)
label   bounds(  5,103,390, 12), text("Receive Location Z"), fontcolour(200,200,200)
}

image   bounds(403,  5,394,105), outlinecolour("white"), outlinethickness(1), colour(0,0,0,0), plant("Mixer") {
hslider bounds(  5, 10,390, 30), textcolour(white), channel("mix"), range(0, 1.00, 0.5), colour(220,220,250), trackercolour(240,210,170)
label   bounds(  5, 35,390, 12), text("Dry/Wet Mix"), fontcolour(200,200,200)
hslider bounds(  5, 55,390, 30), textcolour(white), channel("level"), range(0, 1.00, 0.5), colour(220,220,250), trackercolour(240,210,170)
label   bounds(  5, 80,390, 12), text("Level"), fontcolour(200,200,200)
}

image   bounds(403,115,394,275), outlinecolour("white"), outlinethickness(1), colour(0,0,0,0), plant("Filters") {
hslider bounds(  5, 20,390, 30), textcolour(white), channel("decay"), range(0.01, 1.00, 0.3), colour(220,220,250), trackercolour(240,210,170)
label   bounds(  5, 45,390, 12), text("Reverb Decay"), fontcolour(200,200,200)
hslider bounds(  5, 60,390, 30), textcolour(white), channel("diff"), range(0.01, 1.00, 1), colour(220,220,250), trackercolour(240,210,170)
label   bounds(  5, 85,390, 12), text("High Frequency Diffusion"), fontcolour(200,200,200)
hslider bounds(  5,100,390, 30), textcolour(white), channel("hydecay"), range(0.001, 1.00, 0.1), colour(220,220,250), trackercolour(240,210,170)
label   bounds(  5,125,390, 12), text("High Frequency Decay"), fontcolour(200,200,200)
hslider bounds(  5,140,390, 30), textcolour(white), channel("direct"), range(0, 1.00, 0.5), colour(220,220,250), trackercolour(240,210,170)
label   bounds(  5,165,390, 12), text("Direct Signal Attenuation"), fontcolour(200,200,200)
hslider bounds(  5,180,390, 30), textcolour(white), channel("early_diff"), range(0, 1.00, 0.8), colour(220,220,250), trackercolour(240,210,170)
label   bounds(  5,205,390, 12), text("Early Reflection Diffusion"), fontcolour(200,200,200)
hslider bounds(  5,220,390, 30), textcolour(white), channel("rdistance"), range(0.001, 9.00, 0.3), colour(220,220,250), trackercolour(240,210,170)
label   bounds(  5,245,390, 12), text("Pick-up Separation"), fontcolour(200,200,200)
}

</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-d -n
</CsOptions>

<CsInstruments>

sr 		= 	44100
ksmps 		= 	32
nchnls 		= 	2
0dbfs		=	1

;Author: Iain McCurdy (2015)

instr	1	; read widgets
 gkrx		chnget	"rx"        
 gkry           chnget	"ry"        
 gkrz           chnget	"rz"        
 gksrcx         chnget	"srcx"      
 gksrcy         chnget	"srcy"      
 gksrcz         chnget	"srcz"      
 gkdiff         chnget	"diff"      
 gkdecay        chnget	"decay"     
 gkrdistance    chnget	"rdistance" 
 gkhydecay      chnget	"hydecay"   
 gkdirect       chnget	"direct"    
 gkearly_diff   chnget	"early_diff"
 gkrcvx         chnget	"rcvx"      
 gkrcvy         chnget	"rcvy"      
 gkrcvz         chnget	"rcvz"
 gkmix          chnget	"mix"
 gklevel	chnget	"level"
endin

instr	2	;REVERB
 aL,aR	ins	; read 
 kSwitch		changed		gkrx, gkry, gkrz, gksrcx, gksrcy, gksrcz, gkdiff, gkdecay, gkrdistance, gkhydecay, gkdirect, gkearly_diff, gkrcvx, gkrcvy, gkrcvz	;GENERATE A MOMENTARY '1' PULSE IN OUTPUT 'kSwitch' IF ANY OF THE SCANNED INPUT VARIABLES CHANGE. (OUTPUT 'kSwitch' IS NORMALLY ZERO)
 if	kSwitch=1	then	;IF kSwitch=1 THEN
 	reinit	UPDATE		;BEGIN A REINITIALIZATION PASS FROM LABEL 'UPDATE'
 endif				;END OF CONDITIONAL BRANCHING
 UPDATE:				;A LABEL
 irx		init	i(gkrx)	;CREATE I-RATE VARIABLES FROM K-RATE VARIABLES
 iry		init	i(gkry) ;CREATE I-RATE VARIABLES FROM K-RATE VARIABLES
 irz		init	i(gkrz) ;CREATE I-RATE VARIABLES FROM K-RATE VARIABLES
 ksrcx		init	i(gksrcx) * irx	;THE ACTUAL LOCATION OF THE SOURCE SOUND IS DEFINED RELATIVE TO THE SIZE OF THE ROOM
 ksrcy		init	i(gksrcy) * iry	;THE ACTUAL LOCATION OF THE SOURCE SOUND IS DEFINED RELATIVE TO THE SIZE OF THE ROOM
 ksrcz		init	i(gksrcz) * irz	;THE ACTUAL LOCATION OF THE SOURCE SOUND IS DEFINED RELATIVE TO THE SIZE OF THE ROOM
 idiff		init	i(gkdiff) ;CREATE I-RATE VARIABLES FROM K-RATE VARIABLES
 giBaboVals	ftgen	1, 0, 8, -2, i(gkdecay), i(gkhydecay), i(gkrcvx), i(gkrcvy), i(gkrcvz), i(gkrdistance), i(gkdirect), i(gkearly_diff)
 aRvbL, aRvbR	babo	aL + aR, ksrcx, ksrcy, ksrcz, irx, iry, irz, idiff, giBaboVals	;BABO REVERBERATOR
 rireturn			;RETURN TO PERFORMANCE TIME PASSES
 		outs	((aRvbL*gkmix)+(aL*(1-gkmix)))*gklevel, ((aRvbR*gkmix)+(aR*(1-gkmix)))*gklevel
endin

		
</CsInstruments>

<CsScore>
i 1 0   3600
i 2 0.1 3600	;REVERB INSTRUMENT PLAYS FOR 1 HOUR (AND KEEPS PERFORMANCE GOING)
</CsScore>

</CsoundSynthesizer>
; ConvolutionReverb.csd
; Written by Iain McCurdy, 2012.
; 
; ------------------
; You are encouraged to experiment with different impulse files.
; You can try this one to start with: http://www.iainmccurdy.org/CsoundRealtimeExamples/SourceMaterials/Stairwell.wav
; You can find some here: http://www.openairlib.net/
; NOTE THAT ZERO LATENCY (or close to zero) CONV0LUTION WILL NOT BE POSSIBLE.
; 
; Instructions
; ------------
; 'Size Ratio' compresses the duration of the impulse file (provided that 'compressed' is chosen for 'resize' mode).
; 'Curve' defines the shape of an envelope applied to compressed impulse files using GEN16
; 	zero		=	straight line decaying
; 	positive vals. 	=	increasingly convex decaying
; 	negative vals	=	increasingly concave decaying
; Increase 'iplen' (must be a power of 2) if experiencing performance problems, or reduce it to reduce latency.
; 'Delay OS' fine tunes the delay applied to the dry signal. Normally this should be zero but adjust it to modify how the dry signal lines up with the convoluted signal.

<Cabbage>
form caption("Convolution Reverb") size(505,230), pluginid("Conv") style("legacy")
image bounds(0, 0, 505,230), colour(135, 30, 30,220), shape("rounded"), outlinecolour(255,255,150), outlinethickness(4) 
label   bounds( 29,  10, 55, 12),       text("direction:"), fontcolour(250,250,250)
button  bounds( 10,  22, 90, 20),       text("forward","backward"),                            channel("FwdBwd"), value(0), fontcolour(250,250,250)
label   bounds( 33,  43, 45, 12),       text("resize:"), fontcolour(250,250,250)
button  bounds( 10,  55, 90, 20),       text("normal","compressed"),                           channel("resize"), value(0), fontcolour(250,250,250)
rslider bounds(105, 11, 70, 70),        text("Size Ratio"),     colour(115, 10, 10), trackercolour(255,255,150), outlinecolour( 75, 35,  0), textcolour(250,250,250),  channel("CompRat"),     range(0, 1.00, 1)
rslider bounds(170, 11, 70, 70),        text("Curve"),          colour(115, 10, 10), trackercolour(255,255,150), outlinecolour( 75, 35,  0), textcolour(250,250,250),  channel("Curve"),       range(-8.00, 8.00, 0)
rslider bounds(235, 11, 70, 70),        text("In Skip"),        colour(115, 10, 10), trackercolour(255,255,150), outlinecolour( 75, 35,  0), textcolour(250,250,250),  channel("skipsamples"), range(0, 1.00, 0)
rslider bounds(300, 11, 70, 70),        text("Del.OS."),        colour(115, 10, 10), trackercolour(255,255,150), outlinecolour( 75, 35,  0), textcolour(250,250,250),  channel("DelayOS"),     range(-1.00, 1.00, 0)
rslider bounds(365, 11, 70, 70),        text("Mix"),            colour(115, 10, 10), trackercolour(255,255,150), outlinecolour( 75, 35,  0), textcolour(250,250,250),  channel("mix"),         range(0, 1.00, 0.25)
rslider bounds(430, 11, 70, 70),        text("Level"),          colour(115, 10, 10), trackercolour(255,255,150), outlinecolour( 75, 35,  0), textcolour(250,250,250),  channel("level"),       range(0, 1.00, 0.4)

filebutton bounds( 10, 90, 90, 25), text("Open File","Open File"), fontcolour("white") channel("filename"), shape("ellipse")
soundfiler bounds(110, 90,380,125), channel("beg","len"), identchannel("ImpulseFile"),  colour(0, 255, 255, 255), fontcolour(160, 160, 160, 255)
label      bounds(114, 92,300, 16), text(""), align(left), colour(0,0,0,0), fontcolour(255,255,255,150), identchannel("stringbox")
label      bounds(114, 92,350, 16), text("First open a short stereo impulse sound file..."), align(left), fontcolour(255,255,255,150), visible(1), identchannel("InstructionID")
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

;Author: Iain McCurdy (2012)

;giImpulse	ftgen	1,0,0,1,"D:/Csound/SSDIR/stairwell.wav",0,0,0	; load stereo file
giImpulse	ftgen	1,0,2,-2,0
giDisplay	ftgen	2,0,ftlen(giImpulse),2,0						; display table table
tableicopy 2, 1
gkReady	init	0

; reverse function table UDO
opcode	tab_reverse,i,i
ifn             xin
iTabLen         =               ftlen(ifn)
iTableRev       ftgen           ifn + 100,0,-iTabLen,-2, 0
icount          =               0
loop:
ival            table           iTabLen-icount-1, ifn
                tableiw         ival,icount,iTableRev
                loop_lt         icount,1,iTabLen,loop
                xout   	        iTableRev
endop

; compress function table UDO
opcode	tab_compress,i,iii
ifn,iCompRat,iCurve    xin
iTabLen         =               ftlen(ifn)
iTabLenComp     =               int(ftlen(ifn)*iCompRat)
iTableComp      ftgen           ifn+200,0,-iTabLenComp,-2, 0
iAmpScaleTab	ftgen		ifn+300,0,-iTabLenComp,-16, 1,iTabLenComp,iCurve,0
icount          =               0
loop:
ival            table           icount, ifn
iAmpScale	table		icount, iAmpScaleTab
                tableiw         ival*iAmpScale,icount,iTableComp
                loop_lt         icount,1,iTabLenComp,loop
                xout   	        iTableComp
endop

; compress reverse function table UDO
opcode	tab_compress_rev,i,iii
ifn,iCompRat,iCurve    xin
iTabLen         =               nsamp(ifn)
iTabLenComp     =               int(nsamp(ifn)*iCompRat)
iTableComp      ftgen           ifn+400,0,-iTabLenComp,-2, 0
iAmpScaleTab	ftgen		ifn+500,0,-iTabLenComp,-16, 1,iTabLenComp,iCurve,0
icount          =               0
loop:
ival            table           icount, ifn
iAmpScale	table		icount, iAmpScaleTab
                tableiw         ival*iAmpScale, iTabLenComp-icount-1,iTableComp
                loop_lt         icount,1,iTabLenComp,loop
                xout   	        iTableComp
endop

opcode FileNameFromPath,S,S		; Extract a file name (as a string) from a full path (also as a string)
 Ssrc	xin				; Read in the file path string
 icnt	strlen	Ssrc			; Get the length of the file path string
 LOOP:					; Loop back to here when checking for a backslash
 iasc	strchar Ssrc, icnt		; Read ascii value of current letter for checking
 if iasc==92 igoto ESCAPE		; If it is a backslash, escape from loop
 loop_gt	icnt,1,0,LOOP		; Loop back and decrement counter which is also used as an index into the string
 ESCAPE:				; Escape point once the backslash has been found
 Sname	strsub Ssrc, icnt+1, -1		; Create a new string of just the file name
	xout	Sname			; Send it back to the caller instrument
endop

instr	1
	gSfilepath	chnget	"filename"
	kNewFileTrg	changed	gSfilepath		; if a new file is loaded generate a trigger
	if kNewFileTrg==1 then				; if a new file has been loaded...
	 event	"i",99,0,0				; call instrument to update sample storage function table 
	endif
	
	if trigger:k(gkReady,0.5,0)==1 then		; when a file is loaded for the first time do this conditional branch...
	 event	"i",2,0,3600*24*7			; start the convolution instrument
	endif
endin

instr	2	;CONVOLUTION REVERB INSTRUMENT
	chnset	"visible(0)","InstructionID"		; hide the instruction

	kFwdBwd		chnget	"FwdBwd"
	kresize		chnget	"resize"
	kmix		chnget	"mix"
	klevel		chnget	"level"
	kCompRat	chnget	"CompRat"
	kCurve		chnget	"Curve"
	kskipsamples	chnget	"skipsamples"
	kDelayOS	chnget	"DelayOS"
	kCompRat       init	1 			;IF THIS IS LEFT UNINITIALISED A CRASH WILL OCCUR! 

	
	ainL,ainR	ins				;READ STEREO AUDIO INPUT
	;ainL,ainR	diskin2	"808loop.wav",1,0,1	;USE A SOUND FILE FOR TESTING
	ainMix		sum	ainL,ainR
	
	;CREATE REVERSED TABLES
	irev	tab_reverse	giImpulse
        
        kSwitchStr	changed	gSfilepath
	kSwitchStr	delayk	kSwitchStr,1
	kSwitch	changed		kskipsamples,kFwdBwd,kDelayOS,kCompRat,kCurve,kresize	;GENERATE A MOMENTARY '1' PULSE IN OUTPUT 'kSwitch' IF ANY OF THE SCANNED INPUT VARIABLES CHANGE. (OUTPUT 'kSwitch' IS NORMALLY ZERO)
	kSwitch	+=	kSwitchStr
	if	kSwitch=1	then			;IF I-RATE VARIABLE IS CHANGED...
		reinit	UPDATE				;BEGIN A REINITIALISATION PASS IN ORDER TO EFFECT THIS CHANGE. BEGIN THIS PASS AT LABEL ENTITLED 'UPDATE' AND CONTINUE UNTIL rireturn OPCODE 
	endif						;END OF CONDITIONAL BRANCHING
	UPDATE:						;LABEL
	
	;CREATE COMPRESSED TABLES
	icomp	tab_compress	giImpulse,i(kCompRat),i(kCurve)
        
	;CREATE COMPRESSED REVERSED TABLES
	icomprev	tab_compress_rev	giImpulse,i(kCompRat),i(kCurve)
        
	iplen		=	1024				;BUFFER LENGTH (INCREASE IF EXPERIENCING PERFORMANCE PROBLEMS, REDUCE IN ORDER TO REDUCE LATENCY)
	itab		=	giImpulse			;DERIVE FUNCTION TABLE NUMBER OF CHOSEN TABLE FOR IMPULSE FILE
	iirlen		=	nsamp(itab)*0.5			;DERIVE THE LENGTH OF THE IMPULSE RESPONSE IN SAMPLES. DIVIDE BY 2 AS TABLE IS STEREO.
	iskipsamples	=	nsamp(itab)*0.5*i(kskipsamples)	;DERIVE INSKIP INTO IMPULSE FILE. DIVIDE BY 2 (MULTIPLY BY 0.5) AS ALL IMPULSE FILES ARE STEREO
	
	;FORWARDS REVERB
	if kFwdBwd==0&&kresize==0 then
	 aL,aR	ftconv	ainMix, itab, iplen,iskipsamples, iirlen		;CONVOLUTE INPUT SOUND
	 adelL	delay	ainL, abs((iplen/sr)+i(kDelayOS)) 	;DELAY THE INPUT SOUND ACCORDING TO THE BUFFER SIZE
	 adelR	delay	ainR, abs((iplen/sr)+i(kDelayOS)) 	;DELAY THE INPUT SOUND ACCORDING TO THE BUFFER SIZE
        
        ;BACKWARDS REVERB
        elseif kFwdBwd==1&&kresize==0 then
	 aL,aR	ftconv	ainMix, irev, iplen, iskipsamples, iirlen				;CONVOLUTE INPUT SOUND
	 adelL	delay	ainL,abs((iplen/sr)+(iirlen/sr)-(iskipsamples/sr)+i(kDelayOS))	;DELAY THE INPUT SOUND ACCORDING TO THE BUFFER SIZE AND THE DURATION OF THE IMPULSE FILE
	 adelR	delay	ainR,abs((iplen/sr)+(iirlen/sr)-(iskipsamples/sr)+i(kDelayOS))	;DELAY THE INPUT SOUND ACCORDING TO THE BUFFER SIZE AND THE DURATION OF THE IMPULSE FILE
        
        ;FORWARDS COMPRESSED
	elseif kFwdBwd==0&&kresize==1 then
	 aL,aR	ftconv	ainMix, icomp, iplen,iskipsamples, iirlen*i(kCompRat)		;CONVOLUTE INPUT SOUND
	 adelL	delay	ainL, abs((iplen/sr)+i(kDelayOS)) 				;DELAY THE INPUT SOUND ACCORDING TO THE BUFFER SIZE
	 adelR	delay	ainR, abs((iplen/sr)+i(kDelayOS)) 				;DELAY THE INPUT SOUND ACCORDING TO THE BUFFER SIZE
        
        ;BACKWARDS COMPRESSED
	elseif kFwdBwd==1&&kresize==1 then
	 aL,aR	ftconv	ainMix, icomprev, iplen, iskipsamples, iirlen*i(kCompRat)		;CONVOLUTE INPUT SOUND
	 adelL	delay	ainL,abs((iplen/sr)+((iirlen*i(kCompRat))/sr)-(iskipsamples/sr)+i(kDelayOS))	;DELAY THE INPUT SOUND ACCORDING TO THE BUFFER SIZE AND THE DURATION OF THE IMPULSE FILE
	 adelR	delay	ainR,abs((iplen/sr)+((iirlen*i(kCompRat))/sr)-(iskipsamples/sr)+i(kDelayOS))	;DELAY THE INPUT SOUND ACCORDING TO THE BUFFER SIZE AND THE DURATION OF THE IMPULSE FILE
	endif
                 
        rireturn
        
	; CREATE A DRY/WET MIX
	aMixL	ntrpol	adelL,aL*0.1,kmix
	aMixR	ntrpol	adelR,aR*0.1,kmix
        	outs	aMixL*klevel,aMixR*klevel

endin




instr	99	; load sound file
 	giImpulse	ftgen	1,0,0,1,gSfilepath,0,0,0	; load stereo file
 	gkReady 	init	1					; if no string has yet been loaded giReady will be zero
 	Smessage sprintfk "file(%s)", gSfilepath			; print file to viewer
 	chnset Smessage, "filer1"	

 	Smessage sprintfk "file(%s)", gSfilepath			; print sound file image to fileplayer
 	chnset Smessage, "ImpulseFile"

	Sname	FileNameFromPath	gSfilepath				; Call UDO to extract file name from the full path
	Smessage sprintfk "text(%s)",Sname
	chnset Smessage, "stringbox"
endin


;instr	99	; load sound file
; Sname FileNameFromPath	gSfilepath				; Call UDO to extract file name from the full path
; Smessage sprintfk "text(%s)",Sname
; chnset Smessage, "stringbox"
;endin
		
</CsInstruments>

<CsScore>
i 1 0 [3600*7*24] 
</CsScore>

</CsoundSynthesizer>
; DistanceEmulator.csd
; Written by Iain McCurdy, 2015
	
<Cabbage>
form caption("Distance Emulator") size(700,300), colour(200,210,180), pluginid("DsEm") style("legacy")
hslider   bounds(  5,  5,240, 30), channel("Amp1"), text("Amp.1"),    colour(100,140,150), trackercolour(200,240,250), textcolour("DarkSlateGrey"), range(0, 1, 1, 0.5, 0.001)
hslider   bounds(255,  5,240, 30), channel("Amp2"), text("Amp.2"),    colour(100,140,150), trackercolour(200,240,250), textcolour("DarkSlateGrey"), range(0, 1, 0.25, 0.5, 0.001)
hslider   bounds(  5, 55,240, 30), channel("Rvb1"), text("Rvb.1"),    colour(100,140,150), trackercolour(200,240,250), textcolour("DarkSlateGrey"), range(0, 1, 0, 0.5, 0.001)
hslider   bounds(255, 55,240, 30), channel("Rvb2"), text("Rvb.2"),    colour(100,140,150), trackercolour(200,240,250), textcolour("DarkSlateGrey"), range(0, 1, 1, 0.5, 0.001)
nslider bounds(505, 55, 70, 30), channel("RvbSize"), text("Size"),  colour(100,140,150), trackercolour(200,240,250), textcolour("DarkSlateGrey"), range(0,0.999, 0.8,0.5,0.001)
nslider bounds(585, 55, 70, 30), channel("RvbLPF"), text("RvbLPF"), colour(100,140,150), trackercolour(200,240,250), textcolour("DarkSlateGrey"), range(20,20000,5000,0.5)
hslider   bounds(  5,105,240, 30), channel("LPF1"), text("LPF.1"),    colour(100,140,150), trackercolour(200,240,250), textcolour("DarkSlateGrey"), range(0, 1, 0)
hslider   bounds(255,105,240, 30), channel("LPF2"), text("LPF.2"),    colour(100,140,150), trackercolour(200,240,250), textcolour("DarkSlateGrey"), range(0, 1, 1)
nslider bounds(505,105, 70, 30), channel("LPF_CF"), text("Cutoff"), colour(100,140,150), trackercolour(200,240,250), textcolour("DarkSlateGrey"), range(20,20000,5000,0.5)
hslider   bounds(  5,155,240, 30), channel("HPF1"), text("HPF.1"),    colour(100,140,150), trackercolour(200,240,250), textcolour("DarkSlateGrey"), range(0, 1, 0)
hslider   bounds(255,155,240, 30), channel("HPF2"), text("HPF.2"),    colour(100,140,150), trackercolour(200,240,250), textcolour("DarkSlateGrey"), range(0, 1, 1)
nslider bounds(505,155, 70, 30), channel("HPF_CF"), text("Cutoff"), colour(100,140,150), trackercolour(200,240,250), textcolour("DarkSlateGrey"), range(20,20000,150,0.5)
hslider   bounds(  5,205,240, 30), channel("Wid1"), text("Width"),    colour(100,140,150), trackercolour(200,240,250), textcolour("DarkSlateGrey"), range(0, 1, 1)
hslider   bounds(255,205,240, 30), channel("Wid2"), text("Width"),    colour(100,140,150), trackercolour(200,240,250), textcolour("DarkSlateGrey"), range(0, 1, 1)
hslider   bounds(  5,255,690, 30), channel("Mor"), text("Morph"),     colour(100,140,150), trackercolour(200,240,250), textcolour("DarkSlateGrey"), range(0, 1, 0)

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


instr	1
 kPortTime	linseg	0,0.001,0.2
 kAmp1		chnget	"Amp1"
 kAmp2		chnget	"Amp2"
 kRvb1		chnget	"Rvb1"
 kRvb2		chnget	"Rvb2"
 kRvbSize	chnget	"RvbSize"
 kRvbLPF	chnget	"RvbLPF"
 kLPF1		chnget	"LPF1"
 kLPF2		chnget	"LPF2"
 kLPF_CF	chnget	"LPF_CF"
 kHPF1		chnget	"HPF1"
 kHPF2		chnget	"HPF2"
 kHPF_CF	chnget	"HPF_CF"
 kWid1		chnget	"Wid1"
 kWid2		chnget	"Wid2"
 kMor		chnget	"Mor"
 kMor		portk	kMor,kPortTime
 
 aL,aR	ins
 ;aL,aR	diskin2	"ClassicalGuitar.wav",1,0,1
 
 kAmp	ntrpol	kAmp1,kAmp2,kMor
 kRvb	ntrpol	kRvb1,kRvb2,kMor
 kLPF	ntrpol	kLPF1,kLPF2,kMor
 kHPF	ntrpol	kHPF1,kHPF2,kMor
 kWid	ntrpol	kWid1,kWid2,kMor
 kHPF	pow	kHPF,0.2
 kLPF	pow	kLPF,0.2
 
 aL	*=	kAmp^2
 aR	*=	kAmp^2

 aLPF_L	butlp	aL,kLPF_CF
 aLPF_R	butlp	aR,kLPF_CF
 aL		ntrpol		aL,aLPF_L,kLPF 
 aR		ntrpol		aR,aLPF_R,kLPF 
 	
 aHPF_L	buthp	aL,kHPF_CF
 aHPF_R	buthp	aR,kHPF_CF
 aL		ntrpol		aL,aHPF_L,kHPF 
 aR		ntrpol		aR,aHPF_R,kHPF 

 aRvbL,aRvbR	reverbsc	aL,aR,kRvbSize,kRvbLPF
 aL		ntrpol		aL,aRvbL,kRvb 
 aR		ntrpol		aR,aRvbR,kRvb 
 
 aWidL		ntrpol		aL,(aL+aR)*0.75,kWid
 aWidR		ntrpol		aR,(aL+aR)*0.75,kWid
 aL		=		aWidL
 aR		=		aWidR
 
 	outs	aL,aR
 
 

endin
		
</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
</CsScore>

</CsoundSynthesizer>; GateReverb.csd
; Written by Iain McCurdy, 2015.

; This effect implements a gate reverb effect using a multitap delay.
; The number of delay taps can be very large so their times are not defined explicitly but instead are derived from a number of global input parameters.

; Duration	-	the duration (in seconds) of a gate period
; N.Taps	-	the number of taps within the gate period (NB. increasing this parameter increases CPU and RAM demand)

; Scat.Amp.	-	random scattering of tap amplitudes
; Shape		-	envelope shaping of the gate reverb: 0=flat, <0=decay, >0=build
; Scat.Tim.	-	Scattering of delay tap times.

; Input/Ouput	-	input/output modes:
;			 Mono		-	input taken from left channel input, a single processed channel is sent to both outputs
;			 Stereo Match	-	stereo in / stereo out. 
;						Random values for left and right channels used for amplitude scattering and delay tap time scattering match resulting in a narrower stereo effect.
;			 Stereo Diff	-	stereo in / stereo out. 
;						Random values for left and right channels used for amplitude scattering and delay tap time scattering differ resulting in a wider stereo effect.
;			 (NB both stereo modes will employ double the number of taps specified by 'N.Taps' thereby doubling CPU and RAM usage for this function with respect to the mono mode.)
; Delay Dry	-	When activated the dry signal is delayed by the amount specified by 'Duration'. This can be particularly useful when using a 'build' reverb shape with a 50:50 dry/wet mix.

; Filter option filters each tap at a randomly selected cutoff frequency
; **Note that N.Taps may need to be reduced if CPU overloads**
; Choose between 'Bypass' (no filter), 'Lowpass' (moogvcf2 resonant filter) or 'bandpass' (butbp)
; Min.		-	minimum limit for random cutoff frequency selection
; Max.		-	maximum limit for random cutoff frequency selection
; Q.		-	Filter quality: Lowpass=resonance Bandpass=bandwidth


; Mix		-	dry/wet mix. 
;			 Note that on account of the accumulation of overlapping delay taps, the wet signal is liable to be much louder than the dry signal.
;			 This can be compensated for using the dry/wet control.
; Level		-	output level

<Cabbage>
form caption("Gate Reverb"), size(850, 85), pluginid("GtRv"), colour(13, 50, 67,50) style("legacy")
image              bounds(  0,  0,850, 85), colour(DarkSlateGrey),  shape("sharp")

#define RSliderStyle # colour(150,205,150), textcolour(200,250,200), outlinecolour(140,140,140), trackercolour(yellow), fontcolour(250,250,150)#

image   bounds(  0,  0,135, 85), colour(0,0,0,0), outlinecolour("white"), outlinethickness(2), shape("sharp"), plant("delays"){
rslider bounds(  0,  5, 75, 75), channel("TotDel"), text("Duration"), range(0.01,4,0.4,0.5,0.01), valuetextbox(1), textbox(1), $RSliderStyle
rslider bounds( 60,  5, 75, 75), channel("TotNum"), text("N.Taps"),   range(1,300,100,1,1), valuetextbox(1), textbox(1), $RSliderStyle
}

image   bounds(133,  0,194, 85), colour(0,0,0,0), outlinecolour("white"), outlinethickness(2), shape("sharp"), plant("scatter"){
rslider bounds(  0,  5, 75, 75), channel("ScatAmp"),text("Scat.Amp."), range(0,1,0.5),  valuetextbox(1), textbox(1), $RSliderStyle
rslider bounds( 60,  5, 75, 75), channel("Shape"),text("Shape"), range(-20,20,0,1,0.1), valuetextbox(1), textbox(1), $RSliderStyle
rslider bounds(120,  5, 75, 75), channel("ScatTim"),text("Scat.Tim."), range(0,1,0.5),  valuetextbox(1), textbox(1), $RSliderStyle
}

image    bounds(325,  0,112, 85), colour(0,0,0,0), outlinecolour("white"), outlinethickness(2), shape("sharp"), plant("InputOutputMode"){
label    bounds(  8,  8, 95, 12), text("Input/Output"), fontcolour(200,250,200)
combobox bounds(  8, 21, 95, 20), channel("InOutMode"), text("Mono","Stereo Match","Stereo Diff."), value(3), fontcolour(250,250,150)
checkbox bounds(  8, 50, 95, 12), channel("DelayDry"), text("Delay Dry"), value(0), fontcolour(250,250,150), colour("yellow")
}

image    bounds(435,  0,282, 85), colour(0,0,0,0), outlinecolour("white"), outlinethickness(2), shape("sharp"), plant("Filter"){
label    bounds(  8,  8, 85, 12), text("Filter"), fontcolour(200,250,200)
combobox bounds(  8, 21, 85, 20), channel("FiltSelect"), text("Bypass","Lowpass","Bandpass"), value(1), fontcolour(250,250,150)
rslider bounds( 90,  5, 75, 75), channel("FiltMin"),    text("Min."),      range(4,13.5,4,1,0.1),    valuetextbox(1), textbox(1), $RSliderStyle
rslider bounds(150,  5, 75, 75), channel("FiltMax"),    text("Max."),      range(4,13.5,13.5,1,0.1), valuetextbox(1), textbox(1), $RSliderStyle
rslider bounds(210,  5, 75, 75), channel("Q"),          text("Q."),        range(0,1,0.1),           valuetextbox(1), textbox(1), $RSliderStyle
}

image   bounds(715,  0,135, 85), colour(0,0,0,0), outlinecolour("white"), outlinethickness(2), shape("sharp"), plant("mixer"){
rslider bounds(  0,  5, 75, 75), channel("Mix"),    text("Mix"),      range(0,1.00,0.5), valuetextbox(1), textbox(1), $RSliderStyle
rslider bounds( 60,  5, 75, 75), channel("Level"),  text("Level"),    range(0,1,0.7),    valuetextbox(1), textbox(1), $RSliderStyle
}

</Cabbage>
                    
<CsoundSynthesizer>

<CsOptions>   
-dm0 -n
</CsOptions>

<CsInstruments>

sr 		= 	44100
ksmps 		= 	32
nchnls 		= 	2
0dbfs		=	1
		seed	0
		
; RANDOM DISTRIBUTION - UNIFORM VALUES 0 TO 1
giRnd	ftgen	0, 0, 4096, 21, 1, 1

; UDO
opcode	Taps,a,iiiiiikkkko
 iInOutMode,iTotDel,iScatAmp,iShape,iScatTim,iTotNum,kFiltSelect,kFiltMin,kFiltMax,kQ,iCount	xin
 aMix,aOut	init	0								; INITIALISE THIS TAP AND MIX OF ALL OTHER TAPS
 if iInOutMode==2 then									; IF STEREO MATCHED INPUT/OUTPUT MODE
  iRndDel	table	iCount,giRnd							; READ A RANDOM DELAY OFFSET VALUE FROM RANDOM DISTRIBUTION TABLE (FIXED LAYER BY LAYER) 
  iRndDel	=	((iRndDel*2)-1) * ((iTotDel*iScatTim)/iTotNum)			; RESCALE DELAY OFFSET VALUE
  iRndAmp	table	iCount+1000,giRnd						; READ RANDOM AMPLITUDE VALUE FROM TABLE
  iRndAmp	=	1 - (iRndAmp * iScatAmp)					; RESCALE RANDOM AMPLITUDE VALUE
 else											; OTHERWISE (RANDOM DIFF. OR MONO MODE SELECTED) 
  iRndAmp	random	(1-iScatAmp),1 							; CREATE A RANDOM AMPLITUDE VALUE
  iRndDel	random	-(iTotDel*iScatTim)/iTotNum, (iTotDel*iScatTim)/iTotNum		; CREATE A RANDOM DELAY TAP OFFSET VALUE
 endif 
 iDel		limit	((iTotDel/iTotNum)*iCount)+iRndDel,0.001,iTotDel		; LIMIT DELAY TIMES SO THAT IT WON'T EXCEED BUFFER SIZE OR BE ZERO OR LESS
 aOut		deltapi	iDel								; CREATE DELAY TAP

 /* FILTER */
 iRndCFOct	random	i(kFiltMin),i(kFiltMax)
 iCF		=	cpsoct(iRndCFOct)
 if kFiltSelect==2 then
  aOut		moogvcf2	aOut,iCF,kQ*0.9
 elseif kFiltSelect==3 then
  aOut		butbp	aOut,iCF,(((1-kQ)*2) + 0.05)*iCF, 1
 endif

 if iShape!=0 then									; IF 'SHAPE' VALUE IS NOT ZERO
  if iShape>0 then									; IF A POSITIVE VALUE... (BUILD SHAPE)
   aOut	*=	(iCount/iTotNum)^iShape							; AMPLITUDE SCALED ACCORDING TO TAP NUMBER
  else											; OTHERWISE... (DECAY SHAPE)
   aOut	*=	((iTotNum-iCount)/iTotNum)^abs(iShape)  				; AMPLITUDE SCALED ACCORDING TO TAP NUMBER (REVERSED)
  endif
 endif
 if iCount<iTotNum then									; IF NUMBER OF REQUIRED TAPS IS NOT YET COMPLETED...
  aMix		Taps	iInOutMode,iTotDel,iScatAmp,iShape,iScatTim,iTotNum,kFiltSelect,kFiltMin,kFiltMax,kQ,iCount+1	; CALL FURTHER LAYER(S) FOR ADDITIONAL TAPS AS REQUIRED (INCREMENT COUNTER)
 endif
 		xout	(aOut*(iRndAmp^2))+aMix						; ADD THIS TAP (AND APPLY RANDOM AMPLITUDE FOR THIS TAP) WITH ALL SUBSEQUENT TAPS
endop

instr	1
 kInOutMode	chnget	"InOutMode"							; READ IN WIDGETS
 kInOutMode	init	1								; INIT (TO PREVENT) A ZERO AT I-TIME
 kTotNum	chnget	"TotNum"
 kTotNum	init	100
 kTotDel	chnget	"TotDel"
 kTotDel	init	0.4
 kMix		chnget	"Mix"
 kLevel		chnget	"Level"                                                                       
 kScatAmp	chnget	"ScatAmp"
 kShape		chnget	"Shape"
 kScatTim	chnget	"ScatTim"
 kDelayDry	chnget	"DelayDry"
 kFiltSelect	chnget	"FiltSelect"
 kFiltMin	chnget	"FiltMin"		
 kFiltMax	chnget	"FiltMax"
 kQ		chnget	"Q"
 		
 aL,aR		ins									; READ IN STEREO INPUT 

 if changed(kInOutMode,kTotNum,kTotDel,kScatAmp,kShape,kScatTim,kDelayDry,kFiltMin,kFiltMax)==1 then		; FORCE A REINITIALISATION IF ANY OF THE LISTED K-VARIABLES CHANGE
  		reinit	RESTART
 endif
 RESTART:

 if i(kInOutMode)==1 then									; IF MONO MODE IS SELECTED CREATE JUST A SINGLE CHANNEL AND SEND IT TO BOTH OUTPUTS
  aBuf		delayr	i(kTotDel) 								; SET UP BUFFER
  aDelL		Taps	i(kInOutMode),i(kTotDel),i(kScatAmp),i(kShape),i(kScatTim),i(kTotNum),kFiltSelect,kFiltMin,kFiltMax,kQ	; CALL UDO
 		delayw	aL									; WRITE INTO BUFFER
  if i(kDelayDry)==1 then
   aL	delay	aL,i(kTotDel)
  endif
  aMix		ntrpol	aL,aDelL,kMix								; MIX DRY AND WET SIGNALS
 	outs	aMix*kLevel, aMix*kLevel							; SEND SAME SIGNAL TO BOTH CHANNELS
 else												; OTHERWISE (EITHER OF THE STEREO MODES HAS BEEN SELECTED)
  aBuf		delayr	i(kTotDel) 								; LEFT CHANNEL...
  aDelL		Taps	i(kInOutMode),i(kTotDel),i(kScatAmp),i(kShape),i(kScatTim),i(kTotNum),kFiltSelect,kFiltMin,kFiltMax,kQ	;
 		delayw	aL									;
  aBuf		delayr	i(kTotDel) 								; RIGHT CHANNEL...
  aDelR		Taps	i(kInOutMode),i(kTotDel),i(kScatAmp),i(kShape),i(kScatTim),i(kTotNum),kFiltSelect,kFiltMin,kFiltMax,kQ	;
 		delayw	aR									;
  if i(kDelayDry)==1 then
   aL	delay	aL,i(kTotDel)
   aR	delay	aR,i(kTotDel)
  endif
  aMixL		ntrpol	aL,aDelL,kMix								; MIX DRY AND WET SIGNALS FOR BOTH CHANNELS SEPARATELY
  aMixR		ntrpol	aR,aDelR,kMix								;
 	outs	aMixL*kLevel, aMixR*kLevel							; STEREO OUTPUT
 endif
 
endin

</CsInstruments>

<CsScore>
i 1 0 3600
</CsScore>

</CsoundSynthesizer>
; MultiReverb.csd
; Written by Iain McCurdy, 2012

; Reverb effect that employs screverb and freeverb opcodes.
; Pitch Mod. is only used by reverbsc.

<Cabbage>
form caption("Multi Reverb") size(415, 90), pluginid("Rvrb") style("legacy")
image  bounds(  7, 22, 73, 45), colour( 39, 31, 20), shape("rounded"), outlinecolour(white), outlinethickness(1) 
image  bounds(  0,  0,415, 90), colour( 39, 31, 20), shape("rounded"), outlinecolour("white"), outlinethickness(4) 
button bounds( 11, 20, 66, 20), text("screverb","screverb"), channel("scType"),   value(1), fontcolour:0(50,50,50), fontcolour:1("lime"), radiogroup(1)
button bounds( 11, 40, 66, 20), text("freeverb","freeverb"), channel("freeType"), value(0), fontcolour:0(50,50,50), fontcolour:1("lime"), radiogroup(1)
rslider bounds( 80, 11, 70, 70), text("Size"), 		textcolour(white), 		channel("fblvl"), 	range(0, 1.00, 0.8), colour( 79, 81, 70), trackercolour(240,210,170)
rslider bounds(145, 11, 70, 70), text("Damping"), 	textcolour(white), 		channel("fco"), 	range(0, 1.00, 0.6), colour( 79, 81, 70), trackercolour(240,210,170)
rslider bounds(210, 11, 70, 70), text("Pitch Mod."), 	textcolour(white), 		channel("pitchm"), 	range(0, 20.0, 1),   colour( 79, 81, 70), trackercolour(240,210,170), identchannel("pitchmID")
rslider bounds(275, 11, 70, 70), text("Mix"), 		textcolour(white), 		channel("mix"), 	range(0, 1.00, 1),   colour( 79, 81, 70), trackercolour(240,210,170)
rslider bounds(340, 11, 70, 70), text("Level"),	 	textcolour(white), 		channel("amp"), 	range(0, 1.00, 1),   colour( 79, 81, 70), trackercolour(240,210,170)
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-d -n
</CsOptions>

<CsInstruments>

sr 	= 	44100	;SAMPLE RATE
ksmps 	= 	32	;NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls 	= 	2	;NUMBER OF CHANNELS (2=STEREO)
0dbfs	=	1

; Author: Iain McCurdy (2012)

instr	1
	kscType		chnget	"scType"
	kfreeType	chnget	"freeType"
	ktype		=	(kscType==1?0:1)
	kfblvl		chnget	"fblvl"
	kfco		chnget	"fco"
	kpitchm		chnget	"pitchm"
	kmix		chnget	"mix"
	kamp		chnget	"amp"

	
	if changed(ktype)==1||timeinstk()==1 then
	 Smsg	sprintfk	"visible(%d)",1-ktype
	 chnset	Smsg,"pitchmID"
	endif

	ainL,ainR	ins
	denorm		ainL, ainR	;DENORMALIZE BOTH CHANNELS OF AUDIO SIGNAL
	if ktype==0 then
	 kfco	expcurve	kfco, 4	;CREATE A MAPPING CURVE TO GIVE A NON LINEAR RESPONSE
	 kfco	scale	kfco,20000,20	;RESCALE 0 - 1 TO 20 TO 20000
	 kSwitch		changed		kpitchm	;GENERATE A MOMENTARY '1' PULSE IN OUTPUT 'kSwitch' IF ANY OF THE SCANNED INPUT VARIABLES CHANGE. (OUTPUT 'kSwitch' IS NORMALLY ZERO)
	 if	kSwitch=1	then	;IF kSwitch=1 THEN
		reinit	UPDATE		;BEGIN A REINITIALIZATION PASS FROM LABEL 'UPDATE'
	 endif				;END OF CONDITIONAL BRANCHING
	 UPDATE:				;A LABEL
	 arvbL, arvbR 	reverbsc 	ainL, ainR, kfblvl, kfco, sr, i(kpitchm)
	 rireturn			;RETURN TO PERFORMANCE TIME PASSES
	else
	 arvbL, arvbR 	freeverb 	ainL, ainR, kfblvl, 1-kfco
	endif
	amixL		ntrpol		ainL, arvbL, kmix	;CREATE A DRY/WET MIX BETWEEN THE DRY AND THE REVERBERATED SIGNAL
	amixR		ntrpol		ainR, arvbR, kmix	;CREATE A DRY/WET MIX BETWEEN THE DRY AND THE REVERBERATED SIGNAL
			outs		amixL * kamp, amixR * kamp
endin
		
</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
</CsScore>

</CsoundSynthesizer>
; BufferFreeze.csd
; Written by Iain McCurdy, 2014, updated 2016

<Cabbage>
form caption("Buffer Freeze"), size(610, 250), pluginid("BfFr"), colour(50,50,75) style("legacy")
gentable      bounds(  5,  5,600,120), tablenumber(1), tablecolour("LightBlue"), amprange(-1,1,1), identchannel(table), zoom(-1)

hslider    bounds(  0,120, 610, 25), channel("ptr"),    range(0, 1.00, 1), trackercolour("LightBlue")
label      bounds(205,143,200,  12), text("Pointer"), fontcolour("white")

checkbox bounds( 15,180, 80, 30), channel("freeze"), text("Freeze"), fontcolour("white"), colour("LightBlue")

rslider    bounds(100,170, 70, 70), channel("InGain"), text("Input Gain"), textcolour("white"), range(0, 8.00, 1,0.5), trackercolour("LightBlue")
rslider    bounds(170,170, 70, 70), channel("DryGain"), text("Dry Gain"), textcolour("white"), range(0, 8.00, 1,0.5), trackercolour("LightBlue")
rslider    bounds(240,170, 70, 70), channel("FreezeGain"), text("Freeze Gain"), textcolour("white"), range(0, 8.00, 1), trackercolour("LightBlue")

label      bounds(325,175, 60,13), text("FFT Size"), fontcolour("white")
combobox   bounds(325,190, 60,20), text("128","256","512","1024","2048","4096","8192"), channel("FFTSize"), value(4), fontcolour(255,255,255)

rslider    bounds(395,170, 70, 70), channel("PtrRand"), text("Ptr.Rand."), textcolour("white"), range(0, 1.00, 0), trackercolour("LightBlue")
rslider    bounds(465,170, 70, 70), channel("Transpose"), text("Transpose"), textcolour("white"), range(-24, 24.00, 0), trackercolour("LightBlue")
rslider    bounds(535,170, 70, 70), channel("TransRand"), text("Trans.Rand."), textcolour("white"), range(0, 24.00, 0), trackercolour("LightBlue")

</Cabbage>
                    
<CsoundSynthesizer>

<CsOptions>   
-dm0 -n
</CsOptions>

<CsInstruments>

sr 		= 	44100	; SAMPLE RATE
ksmps 		= 	16	; NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls 		= 	2	; NUMBER OF CHANNELS
0dbfs		=	1	; MAXIMUM AMPLITUDE
			 
giDispBuffer	ftgen	1,0, -600,-2, 0	; define live audio buffer table for display. It makes sense if this corresponds to the display size in pixels.
giAudBuffer	ftgen	2,0, 2^18,10, 0	; define live audio buffer table

instr	1
	aL,aR	ins
	kfreeze	chnget	"freeze"

	koffset	init	0	
	kInGain	chnget	"InGain"
	aL	*=	kInGain
	aR	*=	kInGain
	kDryGain	chnget	"DryGain"
	kFreezeGain	chnget	"FreezeGain"
		
	if kfreeze==0 then							; if writing to table mode...

	 ; write audio to table
	 aptr	phasor	sr/ftlen(giAudBuffer)				
		tablew	aL,aptr,giAudBuffer,1

	koffset			downsamp	aptr				; amount of offset added to freeze read pointer (NB. audio buffer does not scroll)
	
	if metro(sr*ftlen(giDispBuffer)/ftlen(giAudBuffer))==1 then		; update according to size of display table and size of audio buffer	 
	 	tablew	k(aL),ftlen(giDispBuffer),giDispBuffer			; write current audio sample value (as a downsampled krate value) to table
	 	
	 ; SHUNT ENTIRE TABLE CONTENTS ONE STEP TO THE RIGHT
	 kcount	=	0
	 loop:
	 kval	table	kcount+1,giDispBuffer
	 	tablew	kval,kcount,giDispBuffer
	 loop_lt	kcount,1,ftlen(giDispBuffer),loop	

       	 chnset	"tablenumber(1)", "table"					; update table display	

	endif
	 		outs	aL*kDryGain,aR*kDryGain
	 
	else									; otherwise freeze/read mode
	
	 kptr	chnget	"ptr"
	 kporttime linseg 0,0.001,0.01
	 kptr portk kptr, kporttime
	 kPtrRand	gauss	chnget:k("PtrRand")
	 kptr	wrap	kptr+koffset+kPtrRand,0,1
	 kFFTSize	chnget	"FFTSize"
	 kFFTSize	init	4
	 kPitch	=	semitone( chnget:k("Transpose") + gauss:k(chnget:k("TransRand")) )
	 if changed(kFFTSize)==1 then
	  reinit UPDATE
	 endif
	 UPDATE:
	 iFFTSize	=	2^(i(kFFTSize)+6)
	 asig 	mincer 	a(kptr*(ftlen(giAudBuffer)/sr)), kFreezeGain, kPitch, giAudBuffer, 0, iFFTSize
	 rireturn 
		outs	asig,asig

	endif

endin

</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
</CsScore>

</CsoundSynthesizer>
; FrequencyShifter.csd
; Iain McCurdy, 2013.
; 
; Frequency shifting using the hilbert filter
; 
; CONTROLS
; --------
; Input			--	Choose audio input: Live, sine tone or pink noise
; Polarity		--	3 options: 'Positive' = multiply 'Freq.' by 1, 'Negative' = multiply 'Freq.' by -1, 'Dual' = sum of 'Positive' and 'Negative' outputs
; Mix			--	Dry/Wet mix control
; Freq.			--	Principle frequency of the shifting frequency (before modulation by other controls)
; Mult.			--	multipler of shifting frequency. Can be useful for finer control of shifting frequency around zero.
; Feedback		--	Amount of frequency shifter that is fed back into its input
; Level			--	Output level
; Dual Mono / Stereo	--	'Dual Mono' = both channels treated in the same way. 'Stereo' = right channel 180 degrees out of phase with respect to the left
;				 Stereo mode most apparent if shifting frequency is close to zero
; zero freq		--	set 'Freq.' to zero
;  [LFO~]
; Modulate Frequency	--	Switch to activate LFO modulation  of shifting frequency
; Shape			--	Shape of the LFO: sine / triangle / square / random sample and hold / random splines
; Rate			--	Rate of LFO (in hertz)
; Min			--	Minimum frequency of the LFO modulation
; Max			--	Maximum frequency of the LFO modulation
; Pan Mod			--	Amount of panning modulation (strictly speaking, stereo balance modulation). Rate of modulation governed also by 'Rate'
; Sync LFO		--	Restart LFO. Can be useful if 'random spline' modulation becomes 'stuck' at a low frequency

<Cabbage>
form caption("Frequency Shifter") size(560,180), pluginid("fshi") style("legacy")
image                   bounds( 0,  0, 560,180), colour("darkslategrey"), outlinecolour("silver"), outlinethickness(6), shape("rounded")
label    bounds(15,  7, 70, 11), text("INPUT"), fontcolour("white")
combobox bounds(15, 18, 70, 20), channel("input"), value(1), text("Live","Tone","Noise")
label    bounds(15, 42, 70, 11), text("POLARITY"), fontcolour("white")
combobox bounds(15, 53, 70, 20), channel("polarity"), value(1), text("Positive","Negative","Dual")
rslider bounds( 80, 10, 70, 70), text("Mix"),      channel("mix"),    range(0, 1.00, 0.5),     colour(27,59,59), textcolour("white"), trackercolour(255,255,100)
rslider bounds(150, 10, 70, 70), text("Freq."),    channel("freq"),   range(-4000, 4000, -50), colour(27,59,59), textcolour("white"), trackercolour(255,255,100)
rslider bounds(220, 10, 70, 70), text("Mult."),    channel("mult"),   range(-1, 1.00, 0.1),    colour(27,59,59), textcolour("white"), trackercolour(255,255,100)
rslider bounds(285, 15, 60, 60), text("Port."),    channel("port"),   range(0, 30.00, 0.1),    colour(27,59,59), textcolour("white"), trackercolour(255,255,100)

rslider bounds(340, 10, 70, 70), text("Feedback"), channel("fback"),  range(0, 0.75, 0.6),     colour(27,59,59), textcolour("white"), trackercolour(255,255,100)
rslider bounds(410, 10, 70, 70), text("Level"),    channel("level"),  range(0, 1.00, 1),       colour(27,59,59), textcolour("white"), trackercolour(255,255,100)

checkbox bounds(480, 20, 12, 12), channel("r1") fontcolour("white") colour(yellow) value(1)
checkbox bounds(480, 32, 12, 12), channel("r2") fontcolour("white") colour(yellow) 
label    bounds(493, 21, 55,  9), text("DUAL MONO"), fontcolour("white")
label    bounds(492, 33, 40,  9), text("STEREO"), fontcolour("white")

button   bounds(480, 50, 65, 20), colour("Green"), text("Zero Freq", "Zero Freq"), channel("Zerofreq"), value(0), latched(0)

;----

line     bounds( 10, 90, 540, 2), colour("Grey")

;----

checkbox bounds( 40,100,150, 20), channel("ModOnOff") text("LFO Modulate Freq."), fontcolour("white") colour(lime) value(0)
label    bounds( 53,127, 45, 11), text("SHAPE"), fontcolour("white")
combobox bounds( 40,138, 85, 20), channel("LFOShape"), value(7), text("Sine","Triangle","Square","Saw Up","Saw Down","Rand.S&H","Rand.Spline")
rslider bounds( 175,100, 70, 70), text("Rate"),     channel("LFORate"),  range(0, 30,  1.5, 0.5, 0.001), colour("lightslategrey"), textcolour("white"), trackercolour(255,255,100)
rslider bounds( 245,100, 70, 70), text("Min"),      channel("LFOMin"),   range(-2000, 2000, -600),       colour("lightslategrey"), textcolour("white"), trackercolour(255,255,100)
rslider bounds( 315,100, 70, 70), text("Max"),      channel("LFOMax"),   range(-2000, 2000,  600),       colour("lightslategrey"), textcolour("white"), trackercolour(255,255,100)
rslider bounds( 385,100, 70, 70), text("Pan Mod."), channel("PanSpread"),range(0, 1.00, 1),              colour("lightslategrey"), textcolour("white"), trackercolour(255,255,100)
button   bounds(455,100, 65, 20), colour("Green"), text("Sync LFO", "Sync LFO"), channel("SyncLFO"), value(0), latched(0)

</Cabbage>
<CsoundSynthesizer>

<CsOptions>
-d -n -+rtmidi=null -M0
</CsOptions>

<CsInstruments>
sr 	= 	48000
ksmps 	= 	32
nchnls 	= 	2
0dbfs 	= 	1

;Iain McCurdy, 2012
;http://iainmccurdy.org/csound.html
;Frequency shifter effect based around the hilbert filter.

gisine		ftgen	0,0,4096,10,1			;A SINE WAVE SHAPE
gicos		ftgen	0,0,4096,11,1			;A COSINE WAVE SHAPE
gishapes	ftgen	0,0,8,-2,0,1,2,4,5

opcode	FreqShifter,a,akkki
	adry,kmix,kfshift,kfback,ifn	xin			;READ IN INPUT ARGUMENTS
	kWet	limit	kmix*2,0,1
	kDry	limit	(1-kmix)*2,0,1
	aFS	init	0					;INITILISE FEEDBACK SIGNAL (FOR FIRST K-PASS)
	ain	=	adry + (aFS * kfback)			;ADD FEEDBACK SIGNAL TO INPUT (AMOUNT OF FEEDBACK CONTROLLED BY 'Feedback Gain' SLIDER)
	areal, aimag hilbert ain				;HILBERT OPCODE OUTPUTS TWO PHASE SHIFTED SIGNALS, EACH 90 OUT OF PHASE WITH EACH OTHER
	;QUADRATURE OSCILLATORS. I.E. 90 OUT OF PHASE WITH RESPECT TO EACH OTHER
	;OUTUTS	OPCODE	AMPLITUDE | FREQ. | FUNCTION_TABLE | INITIAL_PHASE (OPTIONAL;DEFAULTS TO ZERO)
	asin 	oscili       1,    kfshift,     ifn,           0
	acos 	oscili       1,    kfshift,     ifn,           0.25	
	;RING MODULATE EACH SIGNAL USING THE QUADRATURE OSCILLATORS AS MODULATORS
	amod1	=		areal * acos
	amod2	=		aimag * asin	
	;UPSHIFTING OUTPUT
	aFS	= (amod1 - amod2)
	aout	sum	aFS*kWet, adry*kDry		;CREATE WET/DRY MIX
		xout	aout				;SEND AUDIO BACK TO CALLER INSTRUMENT
endop


opcode	CabbageRadio2,k,SS			; change opcode name and number is string variable inputs
S1,S2	xin					; add string inputs for the required number of inputs 
kon		=	0
koff		=	0
ksum		=	0
ktrigsum	=	0
#define READ_CHANGES(NAME)	#
k$NAME	chnget	$NAME
ksum	=	ksum + k$NAME
kon$NAME	trigger	k$NAME,0.5,0
ktrigsum	=	ktrigsum + kon$NAME#

#define WRITE_CHANGES(NAME'COUNT)	#
if kon$NAME!=1 then
 chnset	koff,$NAME
else
 kval	=	$COUNT
endif#

 $READ_CHANGES(S1) 
 $READ_CHANGES(S2)				; add macro expansions for the required number of radio buttons

if ktrigsum>0 then

 $WRITE_CHANGES(S1'1)
 $WRITE_CHANGES(S2'2)				; add macro expansions for the required number of radio buttons

endif

kval	=	(ksum=0?0:kval)
	xout	kval
endop


instr	1
kporttime	linseg	0,0.001,1
koff	=	0
kmix		chnget	"mix"			; read input widgets
kfreq		chnget	"freq"
kmult		chnget	"mult"
kport		chnget	"port"
kfshift		portk	kfreq*kmult,kporttime*kport
kfback		chnget	"fback"
klevel		chnget	"level"
kpolarity	chnget	"polarity"
kStereoMode	CabbageRadio2	"r1","r2"	; call UDO
kZeroFreq	chnget	"Zerofreq"
ktrig changed	kZeroFreq
if ktrig=1 then
 chnset	koff,"freq"
endif
kModOnOff	chnget	"ModOnOff"	
kLFOShape        chnget	"LFOShape"        	
kLFORate         chnget	"LFORate"         	
kLFOMin          chnget	"LFOMin"          	
kLFOMax          chnget	"LFOMax"          	
kPanSpread	chnget	"PanSpread"		
kSyncLFO	chnget	"SyncLFO"


/* INPUT */
kinput		chnget	"input"
if kinput=1 then
 a1,a2	ins
elseif kinput=2 then
 a1	oscils	0.2, 300, 0
 a2	=	a1
else
 a1	pinkish	0.2
 a2	pinkish	0.2
endif



/* LFO */
if kModOnOff=1 then
 ktrig	changed	kLFOShape,kSyncLFO
 if ktrig==1 then
  reinit RESTART_LFO
 endif
 RESTART_LFO:
 if i(kLFOShape)=6 then
  kLFOFreq	randomh	kLFOMin,kLFOMax,kLFORate
 elseif i(kLFOShape)=7 then				; random spline
  ;kLFOFreq	randomi	kLFOMin,kLFOMax,kLFORate,1
  ;kLFOFreq	portk	kLFOFreq,1/kLFORate
  kLFOFreq	rspline	kLFOMin,kLFOMax,kLFORate,kLFORate*2
 else
  ishape	table	i(kLFOShape)-1,gishapes
  kLFOFreq	lfo	1,kLFORate,ishape
  kLFOFreq	scale	(kLFOFreq*0.5)+0.5,kLFOMin,kLFOMax
 endif
 rireturn
endif

 
 
kfshift	=	kfshift+kLFOFreq

/* FREQUENCY SHIFTERS */
ktrig	changed	kStereoMode
if ktrig=1 then
 reinit RESTART_FREQUENCY_SHIFTERS
endif
RESTART_FREQUENCY_SHIFTERS:
if kpolarity=1 then						; polarity is positive...
 a1	FreqShifter	a1,kmix,kfshift,kfback,gisine	
 if i(kStereoMode)=2 then
  a2	FreqShifter	a2,kmix,kfshift,kfback,gicos	; cosine version
 else
  a2	FreqShifter	a2,kmix,kfshift,kfback,gisine	
 endif 
elseif kpolarity=2 then						; polarity is negative...
 a1	FreqShifter	a1,kmix,-kfshift,kfback,gisine	
 if i(kStereoMode)=2 then
  a2	FreqShifter	a2,kmix,-kfshift,kfback,gicos	; cosine version
 else
  a2	FreqShifter	a2,kmix,-kfshift,kfback,gisine	
 endif
else								; polarity is dual...
 aa	FreqShifter	a1,kmix,kfshift,kfback,gisine	; positive
 if i(kStereoMode)=2 then
  ab	FreqShifter	a2,kmix,kfshift,kfback,gicos	; cosine version
 else
  ab	FreqShifter	a2,kmix,kfshift,kfback,gisine	
 endif 
 ac	FreqShifter	a1,kmix,-kfshift,kfback,gisine	; negative
 if i(kStereoMode)=2 then
  ad	FreqShifter	a2,kmix,-kfshift,kfback,gicos	; cosine version
 else
  ad	FreqShifter	a2,kmix,-kfshift,kfback,gisine	
 endif
rireturn

 a1	=		(aa+ac)*0.5				; sum positive and negative and attenuate
 a2	=		(ab+ad)*0.5
endif


/* PANNING */
if kModOnOff=1 then
 kpan	randomi	0.5-(kPanSpread*0.5),0.5+(kPanSpread*0.5),kLFORate,1
 kpan	portk	kpan,1/kLFORate
 a1  =     a1 * sin(kpan*$M_PI_2)
 a2  =     a2 * cos(kpan*$M_PI_2)
 ;a1	=	a1*kpan
 ;a2	=	a2*(1-kpan)
endif



a1	=	a1 * klevel					; scale using level control
a2	=	a2 * klevel
	outs	a1,a2
endin

</CsInstruments>
<CsScore>
i 1 0 [60*60*24*7]
</CsScore>
</CsoundSynthesizer>; PitchShifter.csd
; Written by Iain McCurdy, 2015.

; A pitch shifter using the traditional delay buffer write-read method.
; Two buffers are used per channel (therefore four in total).
; Each overlapping buffer is gated using a hanning window in order to prevent discontinuities
; Snap		-	when activated forces 'Semitones' to an integer.

; Three modes are provided:
; 	Feedback	-	a standard single pitchshifter unit with feedback (2 feedback modes are offered)
;	Interative	-	the user can choose the number of simultaneous pitchshifter units. 
;				Pitchshifting intervals of the various units are related additively, e.g. if semitones=12, the first unit is 12, the second 24, the third 36 etc. (similar to feedback mode)
;	Harmonic	-	Iterative units are harmonically related. E.g. normal harmonic series of intervals is generated if semitones=12. i.e. first interval of the harmonic series is 1 octave

; FB.Method	-	selects from two different feedback methods:
;			  'Indiv.' - each buffer feeds back into itself
;			  'Global' - the gated and mixed buffers from each channel are fed back into both of the buffers for that channel.
; Smooth	-	Basically portamento time governing the filtering of changes made to 'Semitones' and 'Delay'. If zero, portmento filtering is bypassed entirely.  
; Window	-	Selects the window shape used to envelope the overlapping buffers

<Cabbage>
form caption("Pitch Shifter") size(840,  90), pluginid("PShf"), scrollbars(0) style("legacy")

#define RSliderStyle # colour("Grey"), textcolour("white"), outlinecolour("DarkGrey"), trackercolour("Silver")#

label    bounds( 10, 15, 80, 13), fontcolour("white"), text("Interval")
combobox bounds( 10, 30, 80, 20), text("Semitone","Ratio"), channel("IntervalMode"),       value(1)

image     bounds(120, 15, 35, 58), plant("Ratio"), colour(0,0,0,0), visible(0), identchannel("RatioPlant_ident") {
nslider bounds(  5,  0, 25, 18), channel("Numerator"),        range(1,99,3,1,1)
image     bounds(  0, 26, 35,  1), shape("sharp") 
nslider bounds(  5, 35, 25, 18), channel("Denominator"),      range(1,99,2,1,1)
}

image    bounds(110, 10,130, 70), plant("Semitones"), colour(0,0,0,0), visible(1), identchannel("SemitonesPlant_ident") {
rslider  bounds(  0,  0, 70, 70), text("Semitones"),  channel("Semitones"),      range(-48,48,7,1,0.001), $RSliderStyle
checkbox bounds( 70, 20, 60, 15), text("Snap"),       channel("Snap"), value(1), fontcolour("white")
}

label     bounds(240, 15, 80, 13), fontcolour("white"), text("Mode")
combobox  bounds(240, 30, 80, 20), text("Feedback","Iteration","Harmonic"), channel("Mode"),       value(1)
rslider  bounds(330, 10, 70, 70), text("Feedback"),    channel("Feedback"),       range(0,1,0.9,2,0.0001), visible(1), identchannel("Feedback_ident"),  $RSliderStyle
label    bounds(400, 15, 70, 13), text("FB.Method"), fontcolour("White"), visible(1), identchannel("FBLabel_ident"),
combobox bounds(400, 30, 70, 20), text("Indiv.","Global"),       channel("FBMethod"), visible(1), identchannel("FBMethod_ident"),
rslider  bounds(400, 10, 70, 70), text("Iterations"),    channel("NIter"),       range(1,10,3,1,1), identchannel("NIter_ident"), visible(0)        $RSliderStyle

rslider  bounds(470, 10, 70, 70), text("Delay"),       channel("Delay"),          range(0.01,4.00, 0.1,0.5),      $RSliderStyle
rslider  bounds(540, 10, 70, 70), text("Smooth"),      channel("Smooth"),         range(0,99,0,0.5),              $RSliderStyle
label    bounds(610, 15, 80, 13), text("Window"), fontcolour("White")
combobox bounds(610, 30, 80, 20), text("Hanning","Triangle","Half Sine","Square","Pulse","Perc.","Rev.Perc."),  channel("Window")
rslider  bounds(690, 10, 70, 70), text("Mix"),         channel("Mix"),            range(0, 1,0.5),                $RSliderStyle
rslider  bounds(760, 10, 70, 70), text("Level"),       channel("Level"),          range(0, 1.00,1),               $RSliderStyle

</Cabbage>

<CsoundSynthesizer>
<CsOptions>
-d -n
</CsOptions>
<CsInstruments>

sr 		= 	44100
ksmps 		= 	32
nchnls 		= 	2
0dbfs		=	1	;MAXIMUM AMPLITUDE


giHanning	ftgen	0, 0, 4097,  20, 2
giTriangle	ftgen	0, 0, 4097,  20, 3
giHalfSine	ftgen	0, 0, 4097,   9, 0.5, 1, 0
giSquare	ftgen	0, 0, 4097,   7, 0, 64, 1, 4096-128, 1, 64, 0
giPulse		ftgen	0, 0, 4097,  20, 6, 1, 0.5
giPerc		ftgen	0, 0, 4097,  16, 0, 64, -2, 1,   4096-64, -2, 0
giRevPerc	ftgen	0, 0, 4097,  16, 0, 4096-64, -2, 1, 64, -2, 0



opcode	pitchshifter, aa, aakkkkii	; individual buffer feedback
	ainL,ainR,kratio,kfeedback,kDelay,kSmooth,imaxdelay,iwfn	xin
	setksmps	1

	kPortTime	linseg	0,0.001,1
	;if kSmooth>0 then					; portamento smoothing
	 kratio		portk	kratio, kPortTime*kSmooth	
	 kDelay		portk	kDelay, kPortTime*kSmooth	
	;endif

	aDelay		interp	kDelay

	arate		=	(kratio-1)/kDelay		;SUBTRACT 1/1 SPEED

	aphase1		phasor	-arate				;MOVING PHASE 1-0
	aphase2		phasor	-arate, .5			;MOVING PHASE 1-0 - PHASE OFFSET BY 180 DEGREES (.5 RADIANS)
	
	agate1		tablei	aphase1, iwfn, 1, 0, 1		;
	agate2		tablei	aphase2, iwfn, 1, 0, 1		;
	
	abuf1		delayr	imaxdelay			;DECLARE DELAY BUFFER
	adelsig1	deltap3	aphase1 * aDelay		;VARIABLE TAP
	aGatedSig1	=	adelsig1 * agate1
			delayw	ainL + (aGatedSig1*kfeedback)	;WRITE AUDIO TO THE BEGINNING OF THE DELAY BUFFER, MIX IN FEEDBACK SIGNAL - PROPORTION DEFINED BY gkFB
	
	abuf2		delayr	imaxdelay			;DECLARE DELAY BUFFER
	adelsig2	deltap3	aphase2 * aDelay		;VARIABLE TAP
	aGatedSig2	=	adelsig2 * agate2
			delayw	ainL + (aGatedSig2*kfeedback)	;WRITE AUDIO TO THE BEGINNING OF THE DELAY BUFFER, MIX IN FEEDBACK SIGNAL - PROPORTION DEFINED BY gkFB

	abuf3		delayr	imaxdelay			;DECLARE DELAY BUFFER
	adelsig3	deltap3	aphase1 * aDelay		;VARIABLE TAP
	aGatedSig3	=	adelsig3 * agate1
			delayw	ainR + (aGatedSig3*kfeedback)	;WRITE AUDIO TO THE BEGINNING OF THE DELAY BUFFER, MIX IN FEEDBACK SIGNAL - PROPORTION DEFINED BY gkFB
	
	abuf4		delayr	imaxdelay			;DECLARE DELAY BUFFER
	adelsig4	deltap3	aphase2 * aDelay		;VARIABLE TAP
	aGatedSig4	=	adelsig4 * agate2
			delayw	ainR + (aGatedSig4*kfeedback)	;WRITE AUDIO TO THE BEGINNING OF THE DELAY BUFFER, MIX IN FEEDBACK SIGNAL - PROPORTION DEFINED BY gkFB
			
	aGatedMixL	=	(aGatedSig1 + aGatedSig2) * 0.5
	aGatedMixR	=	(aGatedSig3 + aGatedSig4) * 0.5
			xout	aGatedMixL, aGatedMixR
endop


opcode	pitchshifter2, aa, aakkkkii	; global feedback signal
	ainL,ainR,kratio,kfeedback,kDelay,kSmooth,imaxdelay,iwfn	xin
	setksmps	1

	kPortTime	linseg	0,0.001,1
	;if kSmooth>0 then					; portamento smoothing
	 kratio		portk	kratio, kPortTime*kSmooth	
	 kDelay		portk	kDelay, kPortTime*kSmooth	
	;endif

	aDelay		interp	kDelay

	arate		=	(kratio-1)/kDelay		;SUBTRACT 1/1 SPEED
	aphase1		phasor	-arate				;MOVING PHASE 1-0
	aphase2		phasor	-arate, .5			;MOVING PHASE 1-0 - PHASE OFFSET BY 180 DEGREES (.5 RADIANS)
	
	agate1		tablei	aphase1, iwfn, 1, 0, 1		;
	agate2		tablei	aphase2, iwfn, 1, 0, 1		;

	aGatedMixL,aGatedMixR	init	0
	
	abuf1		delayr	imaxdelay			;DECLARE DELAY BUFFER
	adelsig1	deltap3	aphase1 * aDelay		;VARIABLE TAP
	aGatedSig1	=	adelsig1 * agate1
			delayw	ainL + (aGatedMixL*kfeedback)	;WRITE AUDIO TO THE BEGINNING OF THE DELAY BUFFER, MIX IN FEEDBACK SIGNAL - PROPORTION DEFINED BY gkFB
	
	abuf2		delayr	imaxdelay			;DECLARE DELAY BUFFER
	adelsig2	deltap3	aphase2 * aDelay		;VARIABLE TAP
	aGatedSig2	=	adelsig2 * agate2
			delayw	ainL + (aGatedMixL*kfeedback)	;WRITE AUDIO TO THE BEGINNING OF THE DELAY BUFFER, MIX IN FEEDBACK SIGNAL - PROPORTION DEFINED BY gkFB

	abuf3		delayr	imaxdelay			;DECLARE DELAY BUFFER
	adelsig3	deltap3	aphase1 * aDelay		;VARIABLE TAP
	aGatedSig3	=	adelsig3 * agate1
			delayw	ainR + (aGatedMixR*kfeedback)		;WRITE AUDIO TO THE BEGINNING OF THE DELAY BUFFER, MIX IN FEEDBACK SIGNAL - PROPORTION DEFINED BY gkFB
	
	abuf4		delayr	imaxdelay			;DECLARE DELAY BUFFER
	adelsig4	deltap3	aphase2 * aDelay		;VARIABLE TAP
	aGatedSig4	=	adelsig4 * agate2
			delayw	ainR + (aGatedMixR*kfeedback)		;WRITE AUDIO TO THE BEGINNING OF THE DELAY BUFFER, MIX IN FEEDBACK SIGNAL - PROPORTION DEFINED BY gkFB
			
	aGatedMixL	=	(aGatedSig1 + aGatedSig2) * 0.5
	aGatedMixR	=	(aGatedSig3 + aGatedSig4) * 0.5
	
			xout	aGatedMixL, aGatedMixR
endop


; ITERATIVE PITCHSHIFTER
opcode	pitchshifter3, aa, aakikkiip	; global feedback signal
	ainL,ainR,kratio,iNIter,kDelay,kSmooth,imaxdelay,iwfn,iCount	xin
	setksmps	1

	kPortTime	linseg	0,0.001,1
	;if kSmooth>0 then					; portamento smoothing
	 kratioL	portk	kratio^iCount, kPortTime*kSmooth	
	 kDelay		portk	kDelay, kPortTime*kSmooth	
	;endif

	aDelay		interp	kDelay

	arate		=	(kratioL-1)/kDelay		;SUBTRACT 1/1 SPEED
	aphase1		phasor	-arate				;MOVING PHASE 1-0
	aphase2		phasor	-arate, .5			;MOVING PHASE 1-0 - PHASE OFFSET BY 180 DEGREES (.5 RADIANS)
	
	agate1		tablei	aphase1, iwfn, 1, 0, 1		;
	agate2		tablei	aphase2, iwfn, 1, 0, 1		;

	aGatedMixL,aGatedMixR	init	0
	
	abuf1		delayr	imaxdelay			;DECLARE DELAY BUFFER
	adelsig1	deltap3	aphase1 * aDelay		;VARIABLE TAP
	aGatedSig1	=	adelsig1 * agate1
			delayw	ainL				;WRITE AUDIO TO THE BEGINNING OF THE DELAY BUFFER, MIX IN FEEDBACK SIGNAL - PROPORTION DEFINED BY gkFB
	
	abuf2		delayr	imaxdelay			;DECLARE DELAY BUFFER
	adelsig2	deltap3	aphase2 * aDelay		;VARIABLE TAP
	aGatedSig2	=	adelsig2 * agate2
			delayw	ainL				;WRITE AUDIO TO THE BEGINNING OF THE DELAY BUFFER, MIX IN FEEDBACK SIGNAL - PROPORTION DEFINED BY gkFB

	abuf3		delayr	imaxdelay			;DECLARE DELAY BUFFER
	adelsig3	deltap3	aphase1 * aDelay		;VARIABLE TAP
	aGatedSig3	=	adelsig3 * agate1
			delayw	ainR				;WRITE AUDIO TO THE BEGINNING OF THE DELAY BUFFER, MIX IN FEEDBACK SIGNAL - PROPORTION DEFINED BY gkFB
	
	abuf4		delayr	imaxdelay			;DECLARE DELAY BUFFER
	adelsig4	deltap3	aphase2 * aDelay		;VARIABLE TAP
	aGatedSig4	=	adelsig4 * agate2
			delayw	ainR				;WRITE AUDIO TO THE BEGINNING OF THE DELAY BUFFER, MIX IN FEEDBACK SIGNAL - PROPORTION DEFINED BY gkFB
			
	aGatedMixL	=	(aGatedSig1 + aGatedSig2) * 0.5
	aGatedMixR	=	(aGatedSig3 + aGatedSig4) * 0.5
	
	aMixL,aMixR	init	0
	if iCount<iNIter then
	 aMixL,aMixR	pitchshifter3	ainL,ainR,kratio,iNIter,kDelay,kSmooth,imaxdelay,iwfn,iCount+1
	endif
	
			xout	aGatedMixL + aMixL, aGatedMixR + aMixR
endop


; HARMONIC ITERATIVE
opcode	pitchshifter4, aa, aakikkiip	; global feedback signal
	ainL,ainR,kratio,iNIter,kDelay,kSmooth,imaxdelay,iwfn,iCount	xin
	setksmps	1

	kratio	=	kratio * (iCount+1)/iCount
	iratio	=	i(kratio) * (iCount+1)/iCount

	kPortTime	linseg	0,0.001,1
	if kSmooth>0 then					; portamento smoothing
	 kratio		portk	kratio, kPortTime*kSmooth	
	 kDelay		portk	kDelay, kPortTime*kSmooth	
	endif

	aDelay		interp	kDelay
	;koctfract	=	kSemitones/12			;TRANSPOSITION AS FRACTION OF AN OCTAVE
	;kratio		=	cpsoct(8+koctfract)/cpsoct(8)	;RATIO OF NEW FREQ TO A DECLARED BASE FREQUENCY (MIDDLE C)

	arate		=	(kratio-1)/kDelay		;SUBTRACT 1/1 SPEED
	aphase1		phasor	-arate				;MOVING PHASE 1-0
	aphase2		phasor	-arate, .5			;MOVING PHASE 1-0 - PHASE OFFSET BY 180 DEGREES (.5 RADIANS)
	
	agate1		tablei	aphase1, iwfn, 1, 0, 1		;
	agate2		tablei	aphase2, iwfn, 1, 0, 1		;

	aGatedMixL,aGatedMixR	init	0
	
	abuf1		delayr	imaxdelay			;DECLARE DELAY BUFFER
	adelsig1	deltap3	aphase1 * aDelay		;VARIABLE TAP
	aGatedSig1	=	adelsig1 * agate1
			delayw	ainL				;WRITE AUDIO TO THE BEGINNING OF THE DELAY BUFFER, MIX IN FEEDBACK SIGNAL - PROPORTION DEFINED BY gkFB
	
	abuf2		delayr	imaxdelay			;DECLARE DELAY BUFFER
	adelsig2	deltap3	aphase2 * aDelay		;VARIABLE TAP
	aGatedSig2	=	adelsig2 * agate2
			delayw	ainL				;WRITE AUDIO TO THE BEGINNING OF THE DELAY BUFFER, MIX IN FEEDBACK SIGNAL - PROPORTION DEFINED BY gkFB

	abuf3		delayr	imaxdelay			;DECLARE DELAY BUFFER
	adelsig3	deltap3	aphase1 * aDelay		;VARIABLE TAP
	aGatedSig3	=	adelsig3 * agate1
			delayw	ainR				;WRITE AUDIO TO THE BEGINNING OF THE DELAY BUFFER, MIX IN FEEDBACK SIGNAL - PROPORTION DEFINED BY gkFB
	
	abuf4		delayr	imaxdelay			;DECLARE DELAY BUFFER
	adelsig4	deltap3	aphase2 * aDelay		;VARIABLE TAP
	aGatedSig4	=	adelsig4 * agate2
			delayw	ainR				;WRITE AUDIO TO THE BEGINNING OF THE DELAY BUFFER, MIX IN FEEDBACK SIGNAL - PROPORTION DEFINED BY gkFB
			
	aGatedMixL	=	(aGatedSig1 + aGatedSig2) * 0.5
	aGatedMixR	=	(aGatedSig3 + aGatedSig4) * 0.5
	
	aMixL,aMixR	init	0
	if iCount<iNIter then
	 aMixL,aMixR	pitchshifter4	ainL,ainR,kratio,iNIter,kDelay,kSmooth,imaxdelay,iwfn,iCount+1
	endif
	
			xout	aGatedMixL + aMixL, aGatedMixR + aMixR
endop


instr	1
	aInL,aInR	ins
	;aInL		vco2	0.2,100,4,0.5
	;aInR		=	aInL
	
	kFeedback	chnget	"Feedback"
	kNIter		chnget	"NIter"
	kNIter		init	1
	kDelay		chnget	"Delay"
	kSmooth		chnget	"Smooth"
	kMode		chnget	"Mode"
	kIntervalMode	chnget	"IntervalMode"
	
	if changed(kIntervalMode)==1 then				; semitones mode
	 if kIntervalMode==1 then
	 	chnset	"visible(0)","RatioPlant_ident"
	 	chnset	"visible(1)","SemitonesPlant_ident"
	 else								; ratio mode
	 	chnset	"visible(1)","RatioPlant_ident"
	 	chnset	"visible(0)","SemitonesPlant_ident"
	 endif
	endif
	
	if kIntervalMode==1 then
		kSemitones	chnget	"Semitones"
		kSnap		chnget	"Snap"
		if kSnap==1 then
		 if frac(kSemitones)!=0 then
		  kSemitones	=	round(kSemitones)
		  		chnset	kSemitones,"Semitones"
		 endif
		endif
	 	kRatio	=	semitone(kSemitones)	
	else
		kNumerator	chnget	"Numerator"
		kDenominator	chnget	"Denominator"
		kRatio		=	kNumerator/kDenominator
		kSemitones	=	log2(kRatio)*12	
	endif
	
	if changed(kMode)==1 then
	 if kMode==1 then
	  chnset	"visible(1)","Feedback_ident"
	  chnset	"visible(1)","FBLabel_ident"
	  chnset	"visible(1)","FBMethod_ident"
	  chnset	"visible(0)","NIter_ident"
	 elseif kMode==2 then
	  chnset	"visible(0)","Feedback_ident"
	  chnset	"visible(0)","FBLabel_ident"
	  chnset	"visible(0)","FBMethod_ident"
	  chnset	"visible(1)","NIter_ident"
	 else
	  chnset	"visible(0)","Feedback_ident"
	  chnset	"visible(0)","FBLabel_ident"
	  chnset	"visible(0)","FBMethod_ident"
	  chnset	"visible(1)","NIter_ident"
	 endif
	endif
	
	
	iMaxDelay	=	4
	kWindow		chnget	"Window"
	kWindow		init	1
	if changed(kWindow)==1 then
	 reinit UPDATE
	endif
	UPDATE:
	iWfn	=	giHanning + i(kWindow) - 1

	if kMode==1 then
	 kFBMethod	chnget	"FBMethod"
	 kFBMethod	init	1
	 if kFBMethod==1 then
	  aPS_L,aPS_R	pitchshifter	aInL,aInR,kRatio,kFeedback,kDelay,kSmooth,iMaxDelay,iWfn
	 elseif kFBMethod==2 then
	  aPS_L,aPS_R	pitchshifter2	aInL,aInR,kRatio,kFeedback,kDelay,kSmooth,iMaxDelay,iWfn	
	 endif
	elseif kMode==2 then
	 if changed(kNIter)==1 then
	  reinit UPDATE3
	 endif
	 UPDATE3:
	 aPS_L,aPS_R	pitchshifter3	aInL,aInR,kRatio,i(kNIter),kDelay,kSmooth,iMaxDelay,iWfn	
	elseif kMode==3 then		; HARMONIC ITERATIVE
	 if changed(kNIter)==1 then
	  reinit UPDATE4
	 endif
	 ;aFBL,aFBR	init	0
	 UPDATE4:
	 ;aPS_L,aPS_R	pitchshifter4	aInL+aFBL,aInR+aFBL,semitone(kSemitones-12),i(kNIter),kDelay,kSmooth,iMaxDelay,iWfn	
	 aPS_L,aPS_R	pitchshifter4	aInL,aInR,semitone(kSemitones-12),i(kNIter),kDelay,kSmooth,iMaxDelay,iWfn	
	 ;aFBL	=	aPS_L*kFeedback
	 ;aFBR	=	aPS_R*kFeedback
	endif

	rireturn
	kMix		chnget	"Mix"
	aMixL		ntrpol	aInL,aPS_L,kMix
	aMixR		ntrpol	aInR,aPS_R,kMix
	kLevel		chnget	"Level"
			outs	aMixL*kLevel, aMixR*kLevel
endin

</CsInstruments>

<CsScore>
i 1 0 [60*60*24*7]
</CsScore>

</CsoundSynthesizer>; pvsAdsyn.csd
; Written by Iain McCurdy, 2012.

; Encapsulation of the pvsadsyn opcode.
; This opcode takes a streaming phase vocoding analysis and reconstitutes it as an audio signal with user-definable parameters
;  for the number of bins to include, the bin from which to begin resynthesis (bin offset) and the option of skipping bins and not
;  resynthesising them one after another (Bin Incr.)

<Cabbage>
form caption("pvsAdsyn") size(580, 90), pluginid("adsy") style("legacy")
image pos(0, 0), size(580, 90), colour(200,100,50,150), shape("rounded"), outlinecolour("white"), outlinethickness(5) 
rslider bounds(10, 11, 70, 70),  text("Freq.Mod."),  channel("fmod"),      range(0.25, 4, 1),        textcolour("white"), colour(200,100,50,250), trackercolour(tan)
rslider bounds(80, 11, 70, 70),  text("Num.Osc.s"),  channel("noscs"),     range(1, 1024, 256,1,1),  textcolour("white"), colour(200,100,50,250), trackercolour(tan)
rslider bounds(150, 11, 70, 70), text("Bin Offset"), channel("binoffset"), range(0, 256, 1,1,1),     textcolour("white"), colour(200,100,50,250), trackercolour(tan)
rslider bounds(220, 11, 70, 70), text("Bin Incr."),  channel("binincr"),   range(1, 32, 1,1,1),      textcolour("white"), colour(200,100,50,250), trackercolour(tan)
label    bounds(295,20, 60,13), text("FFT Size"), fontcolour("white")
combobox bounds(295,35, 60,18), text("64","128","256","512","1024","2048","4096","8192"), channel("att_table"), value(5), fontcolour(255,255,200)
rslider bounds(360, 11, 70, 70), text("Feedback"),   channel("feedback"),  range(0, 0.99, 0),        textcolour("white"), colour(200,100,50,250), trackercolour(tan)
rslider bounds(430, 11, 70, 70), text("Mix"),        channel("mix"),       range(0, 1.00, 1),        textcolour("white"), colour(200,100,50,250), trackercolour(tan)
rslider bounds(500, 11, 70, 70), text("Level"),      channel("lev"),       range(0, 5.00, 0.5, 0.5), textcolour("white"), colour(200,100,50,250), trackercolour(tan)
</Cabbage>

<CsoundSynthesizer>
<CsOptions>
-d -n
</CsOptions>
<CsInstruments>

sr 		= 	44100
ksmps 		= 	32
nchnls 		= 	2
0dbfs		=	1	;MAXIMUM AMPLITUDE

;Author: Iain McCurdy (2012)
;http://iainmccurdy.org/csound.html

/* FFT attribute tables */
giFFTattributes1	ftgen	0, 0, 4, -2,   64,  32,   64, 1
giFFTattributes2	ftgen	0, 0, 4, -2,  128,  64,  128, 1
giFFTattributes3	ftgen	0, 0, 4, -2,  256, 128,  256, 1
giFFTattributes4	ftgen	0, 0, 4, -2,  512, 128,  512, 1
giFFTattributes5	ftgen	0, 0, 4, -2, 1024, 256, 1024, 1
giFFTattributes6	ftgen	0, 0, 4, -2, 2048, 512, 2048, 1
giFFTattributes7	ftgen	0, 0, 4, -2, 4096,1024, 4096, 1
giFFTattributes8	ftgen	0, 0, 4, -2, 8192,2048, 8192, 1

opcode	pvsadsyn_module,a,akkkkkkiiii
	ain,kfmod,knoscs,kbinoffset,kbinincr,kfeedback,kmix,iFFTsize,ioverlap,iwinsize,iwintype	xin
	aresyn	init	0
	f_anal  	pvsanal	ain+(aresyn*kfeedback), iFFTsize, ioverlap, iwinsize, iwintype		;ANALYSE AUDIO INPUT SIGNAL AND OUTPUT AN FSIG
	knossc	init	256
	ktrig		changed	knoscs,kbinoffset,kbinincr
	if ktrig==1 then
	 reinit	UPDATE
	endif
	UPDATE:
	inoscs	init	i(knoscs)	
	ibinoffset	init	i(kbinoffset)
	ibinincr	init	i(kbinincr)
	inoscs	limit	inoscs, 1, (((iFFTsize*0.5)+1)-ibinoffset)/ibinincr
	aresyn 		pvsadsyn f_anal, inoscs, kfmod , i(kbinoffset), i(kbinincr)	;RESYNTHESIZE FROM THE fsig USING pvsadsyn
	rireturn
	amix		ntrpol		ain, aresyn, kmix					;CREATE DRY/WET MIX
			xout		amix	
endop

instr	1
	; audio input
	ainL,ainR	ins
	;ainL,ainR	diskin	"SynthPad.wav",1,0,1	;USE FOR TESTING
	kmix		chnget	"mix"
	kfmod		chnget	"fmod"
	knoscs		chnget	"noscs"
	kbinoffset	chnget	"binoffset"
	kbinincr	chnget	"binincr"
	klev		chnget	"lev"
	kfeedback	chnget	"feedback"
	
	/* SET FFT ATTRIBUTES */
	katt_table	chnget	"att_table"	; FFT atribute table
	katt_table	init	5
	ktrig		changed	katt_table
	if ktrig==1 then
	 reinit update
	endif
	update:
	iFFTsize	table	0, giFFTattributes1 + i(katt_table) - 1
	ioverlap	table	1, giFFTattributes1 + i(katt_table) - 1
	iwinsize	table	2, giFFTattributes1 + i(katt_table) - 1
	iwintype	table	3, giFFTattributes1 + i(katt_table) - 1
	/*-------------------*/

	aoutL		pvsadsyn_module	ainL,kfmod,knoscs,kbinoffset,kbinincr,kfeedback,kmix,iFFTsize,ioverlap,iwinsize,iwintype
	aoutR		pvsadsyn_module	ainR,kfmod,knoscs,kbinoffset,kbinincr,kfeedback,kmix,iFFTsize,ioverlap,iwinsize,iwintype
			outs	aoutR*klev,aoutR*klev
endin

</CsInstruments>

<CsScore>
i 1 0 [60*60*24*7]
</CsScore>

</CsoundSynthesizer>; pvsBlur.csd
; Written by Iain McCurdy, 2012.

<Cabbage>
form caption("pvsBlur"), size(235,125) colour( 70, 90,100), pluginid("blur") style("legacy")
image             bounds(0, 0,235,125), colour( 70, 90,100), shape("rounded"), outlinecolour("white"), outlinethickness(5) 
label    bounds(15,20, 60,13), text("FFT Size"), fontcolour("white")
combobox bounds(15,35, 60,20), text("128","256","512","1024","2048","4096","8192"), channel("att_table"), value(4), fontcolour(220,220,255)
rslider bounds( 80, 10, 70, 70), text("Mix"),       channel("mix"),       range(0, 1.00, 1),                textcolour("white"), colour( 30, 50, 60),trackercolour("white")
rslider bounds(150, 10, 70, 70), text("Level"),     channel("lev"),       range(0, 1.00, 0.5, 0.5),         textcolour("white"), colour( 30, 50, 60),trackercolour("white")
hslider bounds( 10, 70,210, 40), channel("blurtime"),  range(0, 2.00, 0.0, 0.5, 0.0001),                    textcolour("white"), colour( 10, 30, 40),trackercolour("white")
label   bounds( 85,103, 60, 11), text("Blur Time"), fontcolour("white")
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-d -n
</CsOptions>
<CsInstruments>
sr 		= 	44100
ksmps 		= 	64
nchnls 		= 	2
0dbfs		=	1	;MAXIMUM AMPLITUDE

; Author: Iain McCurdy (2012)
; http://iainmccurdy.org/csound.html

/* FFT attribute tables */
giFFTattributes1	ftgen	0, 0, 4, -2,  128,  64,  128, 1
giFFTattributes2	ftgen	0, 0, 4, -2,  256, 128,  256, 1
giFFTattributes3	ftgen	0, 0, 4, -2,  512, 128,  512, 1
giFFTattributes4	ftgen	0, 0, 4, -2, 1024, 256, 1024, 1
giFFTattributes5	ftgen	0, 0, 4, -2, 2048, 512, 2048, 1
giFFTattributes6	ftgen	0, 0, 4, -2, 4096,1024, 4096, 1
giFFTattributes7	ftgen	0, 0, 4, -2, 8192,2048, 8192, 1

opcode	pvsblur_module,a,akkkiiii
	ain,kblurtime,kmix,klev,iFFTsize,ioverlap,iwinsize,iwintype	xin
	f_anal  	pvsanal	ain, iFFTsize, ioverlap, iwinsize, iwintype		;ANALYSE AUDIO INPUT SIGNAL AND OUTPUT AN FSIG
	f_blur		pvsblur	f_anal, kblurtime, 2					;BLUR AMPLITUDE AND FREQUENCY VALUES OF AN F-SIGNAL
	aout		pvsynth f_blur                      				;RESYNTHESIZE THE f-SIGNAL AS AN AUDIO SIGNAL
	amix		ntrpol		ain, aout, kmix					;CREATE DRY/WET MIX
			xout		amix*klev	
endop

instr	1
	kblurtime	chnget	"blurtime"
	kmix		chnget	"mix"
	klev		chnget	"lev"

	ainL,ainR	ins
	;ainL,ainR	diskin	"808loop.wav",1,0,1	;USE FOR TESTING

	/* SET FFT ATTRIBUTES */
	katt_table	chnget	"att_table"	; FFT atribute table
	katt_table	init	5
	ktrig		changed	katt_table
	if ktrig==1 then
	 reinit update
	endif
	update:
	iFFTsize	table	0, giFFTattributes1 + i(katt_table) - 1
	ioverlap	table	1, giFFTattributes1 + i(katt_table) - 1
	iwinsize	table	2, giFFTattributes1 + i(katt_table) - 1
	iwintype	table	3, giFFTattributes1 + i(katt_table) - 1
	/*-------------------*/
	
	kporttime	linseg	0,0.001,0.02
	kblurtime	portk	kblurtime,kporttime
	aoutL		pvsblur_module	ainL,kblurtime,kmix,klev,iFFTsize,ioverlap,iwinsize,iwintype
	aoutR		pvsblur_module	ainR,kblurtime,kmix,klev,iFFTsize,ioverlap,iwinsize,iwintype
				outs	aoutR,aoutR
endin

</CsInstruments>

<CsScore>
i 1 0 [60*60*24*7]
</CsScore>

</CsoundSynthesizer>; pvsBuffer.csd
; Writes audio into a circular FFT buffer.
; Read speed can be modified as can the frequencies.
; Take Care! Feedback values above 1 are intended to be used only when transposition is not unison. 

<Cabbage>
form caption("pvsBuffer") size(580,135), pluginid("buff") style("legacy")
image             bounds(0, 0, 580, 135), colour(100, 80, 80,125), shape("rounded"), outlinecolour("white"), outlinethickness(4) 

label             bounds(10, 97, 48,10), text("RECORD"), align(right), fontcolour(white)
label             bounds(10,112, 48,10), text("PLAY"),   align(right), fontcolour(white)
image             bounds( 70,101, 490, 3), colour(255, 50, 50), shape("sharp"), identchannel("RecIndic")
image             bounds( 70,116, 490, 3), colour( 55,250, 50), shape("sharp"), identchannel("PlayIndic")

rslider bounds(10, 11, 70, 70),  text("Speed"),     channel("speed"),     range(0, 4, 1, 0.5, 0.0001), textcolour("white"),    colour(100, 80, 80,  5) trackercolour(silver)
rslider bounds(80, 11, 70, 70),  text("Buf. Size"), channel("buflen"),    range(0.10,20.00, 5, 0.5),   textcolour("white"),    colour(100, 80, 80,  5) trackercolour(silver)
rslider bounds(150, 11, 70, 70), text("Semitones"), channel("semis"),     range(-24, 24, 0, 1, 1),     textcolour("white"),    colour(100, 80, 80,  5) trackercolour(silver)
rslider bounds(220, 11, 70, 70), text("Cents"),     channel("cents"),     range(-100, 100, 0, 1, 1),   textcolour("white"),    colour(100, 80, 80,  5) trackercolour(silver)
rslider bounds(290, 11, 70, 70), text("Feedback"),  channel("FB"),        range(0, 1.50, 0),           textcolour("white"),    colour(100, 80, 80,  5) trackercolour(silver)
label    bounds(360,20, 60,13), text("FFT Size"), fontcolour("white")
combobox bounds(360,35, 60,20), text("64","128","256","512","1024","2048","4096","8192"), channel("att_table"), value(5), fontcolour(255,255,190)

rslider bounds(430, 11, 70, 70), text("Mix"),       channel("mix"),       range(0, 1.00, 1),           textcolour("white"),    colour(100, 80, 80,  5) trackercolour(silver)
rslider bounds(500, 11, 70, 70), text("Level"),     channel("lev"),       range(0, 1.00, 0.5),         textcolour("white"),    colour(100, 80, 80,  5) trackercolour(silver)
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-d -n
</CsOptions>
<CsInstruments>

sr 		= 	44100
ksmps 		= 	32
nchnls 		= 	2
0dbfs		=	1	;MAXIMUM AMPLITUDE

;Author: Iain McCurdy (2012)
;http://iainmccurdy.org/csound.html

/* FFT attribute tables */
giFFTattributes1	ftgen	0, 0, 4, -2,   64,  32,   64, 1
giFFTattributes2	ftgen	0, 0, 4, -2,  128,  64,  128, 1
giFFTattributes3	ftgen	0, 0, 4, -2,  256, 128,  256, 1
giFFTattributes4	ftgen	0, 0, 4, -2,  512, 128,  512, 1
giFFTattributes5	ftgen	0, 0, 4, -2, 1024, 256, 1024, 1
giFFTattributes6	ftgen	0, 0, 4, -2, 2048, 512, 2048, 1
giFFTattributes7	ftgen	0, 0, 4, -2, 4096,1024, 4096, 1
giFFTattributes8	ftgen	0, 0, 4, -2, 8192,2048, 8192, 1

opcode	pvsbuffer_module,akk,akkkkkkiiii
	ain,kspeed,kbuflen,kscale,kfeedback,kmix,klev,iFFTsize,ioverlap,iwinsize,iwintype	xin
	kPhOffset	=	0
	ktrig		changed		kbuflen
	ibuflen	init	1
	kspeed	init	1
	kscale	init	1
	if ktrig==1 then
	 reinit	UPDATE
	endif
	UPDATE:
	ibuflen		=	i(kbuflen)
	iphasor		ftgen		0, 0, 65536, 7, 0, 65536, 1			;WAVE SHAPE FOR A MOVING PHASE POINTER
	aread 		osciliktp 	kspeed/ibuflen, iphasor, kPhOffset		;CREATE MOVING POINTER TO READ FROM BUFFER
	kread		downsamp	aread
	kread		=		kread * ibuflen
	aFB		init		0
	f_anal 		pvsanal		ain+aFB, iFFTsize, ioverlap, iwinsize, iwintype	;ANALYSE THE AUDIO SIGNAL THAT WAS CREATED IN INSTRUMENT 1. OUTPUT AN F-SIGNAL.
	ibuffer,ktime  	pvsbuffer   	f_anal, ibuflen					;BUFFER FSIG	
	rireturn
	khandle		init 		ibuffer						;INITIALISE HANDLE TO BUFFER
	f_buf  		pvsbufread  	kread , khandle			;READ BUFFER
	f_scale		pvscale 	f_buf, kscale					;RESCALE FREQUENCIES
	aresyn 		pvsynth  	f_scale			                   	;RESYNTHESIZE THE f-SIGNAL AS AN AUDIO SIGNAL	
	aFB		dcblock2	aresyn * kfeedback				;CREATE FEEDBACK SIGNAL FOR NEXT PASS AND BLOCK DC OFFSET ACCUMULATION
	amix		ntrpol		ain, aresyn, kmix				;CREATE DRY/WET MIX
			xout		amix*klev,ktime/ibuflen,kread/ibuflen
endop

instr	1
	ainL,ainR	ins
	;ainL,ainR	diskin	"SynthPad.wav",1,0,1	;USE FOR TESTING
	kspeed		chnget	"speed"
	kbuflen		chnget	"buflen"
	ksemis		chnget	"semis"
	kcents		chnget	"cents"
	ksemis		init	0
	kcents		init	0
	kscale		=	semitone(ksemis)*cent(kcents)
	kscale		init	1
	kbuflen		init	1
	kspeed		init	1
	kmix		chnget	"mix"
	kfeedback	chnget	"FB"
	klev		chnget	"lev"
	kmix		init	1
	kfeedback	init	0
	klev		init	0.5

	/* SET FFT ATTRIBUTES */
	katt_table	chnget	"att_table"	; FFT atribute table
	katt_table	init	5
	ktrig		changed	katt_table
	if ktrig==1 then
	 reinit update
	endif
	update:
	iFFTsize	table	0, giFFTattributes1 + i(katt_table) - 1
	ioverlap	table	1, giFFTattributes1 + i(katt_table) - 1
	iwinsize	table	2, giFFTattributes1 + i(katt_table) - 1
	iwintype	table	3, giFFTattributes1 + i(katt_table) - 1
	/*-------------------*/
	
	aoutL,kwrite,kread		pvsbuffer_module	ainL,kspeed,kbuflen,kscale,kfeedback,kmix,klev,iFFTsize,ioverlap,iwinsize,iwintype
	aoutR,k1,k1			pvsbuffer_module	ainR,kspeed,kbuflen,kscale,kfeedback,kmix,klev,iFFTsize,ioverlap,iwinsize,iwintype
			outs	aoutL,aoutR

	ktrig	metro	kr/(ksmps*2)
	if ktrig==1 then
	 Smsg	sprintfk	"size(%d,3)",kwrite*490
	 chnset	Smsg,"RecIndic" 
	 Smsg	sprintfk	"size(%d,3)",kread*490
	 chnset	Smsg,"PlayIndic" 
	endif
endin

</CsInstruments>
<CsScore>
i 1 0.1 [60*60*24*7]
</CsScore>
</CsoundSynthesizer>; pvScale.csd
; Written by Iain McCurdy, 2012. Updated 2015.

; Pitch scaling interval can be defined either in semitones and cents or as a ratio (fraction).

; Three methods of feedback are possible:
;  1: "F Sig" Direct feeding back of the fft signal
;  2: "Audio" feeding back of the audio signal, therefore each time the signal will be resynthesized anf then re-analysed. Additional delay will also be induced when feeding back.
;  3: "Iter. A number of iterations are defined, creating a stack of pitch shifted signals. This method will be CPU costly if "Iter." is high. Some CPU can be saved by reducing "FFT Size"

; FFT Size	-	Larger values will retain spectral accuracy at the expense of time accuracy
;			Smaller values will improve time accuracy at the expense of spectral accuracy.
;			In general smaller values are preferrable with rhythmic material and larger values are preferrable with melodic material. 

; Port.		-	Portamento time applied to changed made to the pitch scaling interval

<Cabbage>
form caption("pvscale Pitch Shifter") size(690,  90), pluginid("scal") style("legacy")
image                         bounds(0, 0, 690, 90), colour("SlateGrey"), outlinecolour("silver"), outlinethickness(4)

image bounds(  6, 23,678, 30), colour("silver"), shape("ellipse"), outlinethickness(3)

label    bounds( 10, 24, 75, 12), text("Interval"), fontcolour("black")
combobox bounds( 10, 37, 75, 20), channel("IntervalMode"), text("Semitone","Ratio"), value(1)

image   bounds( 90, 10,290, 80), colour(0,0,0,0), plant("Semitone"), identchannel("semitone_ident") {
image   bounds(  8,  1, 54, 53), colour(30,30,30), shape("ellipse"), outlinethickness(0)
rslider bounds(  0,  0, 70, 70),  text("Semitones"), channel("semis"), range(-24, 24, 7, 1, 1),   textcolour("black"),colour("DarkSlateGrey"), trackercolour("LightBlue")
image   bounds( 78,  1, 54, 53), colour(30,30,30), shape("ellipse"), outlinethickness(0)
rslider bounds( 70,  0, 70, 70),  text("Cents"),     channel("cents"), range(-100, 100, 0, 1, 1), textcolour("black"),colour("DarkSlateGrey"), trackercolour("LightBlue")
}

image     bounds(130, 20, 35, 58), plant("Ratio"), colour(0,0,0,0), visible(0), identchannel("ratio_ident") {
nslider bounds(  5,  5, 25, 18), channel("Numerator"),        range(1,99,3,1,1)
image     bounds(  0, 26, 35,  1), shape("sharp"), colour("black")
nslider bounds(  5, 30, 25, 18), channel("Denominator"),      range(1,99,2,1,1)
}

image   bounds(238, 11, 54, 53), colour(30,30,30), shape("ellipse"), outlinethickness(0)	; circles around rsliders
rslider bounds(230, 10, 70, 70), text("Feedback"),  channel("FB"), range(0.00, 0.99, 0),         textcolour("black"),colour("DarkSlateGrey"), trackercolour("LightBlue"), identchannel("feedback_ident")
rslider bounds(230, 10, 70, 70), text("Iter."),     channel("Iter"), range(1, 10, 1,1,1),        textcolour("black"),colour("DarkSlateGrey"), trackercolour("LightBlue"), identchannel("iter_ident"), visible(0)

label    bounds(310,  8, 70, 12), text("F.back Mode"), FontColour("black")
combobox bounds(310, 20,  70,20), channel("FB_mode"), value(1), text("F Sig.", "Audio", "Iter.")

label    bounds(310, 45,  70, 12), text("Formants"), FontColour("black")
combobox bounds(310, 57,  70, 20), channel("formants"), value(1), text("Ignore", "Keep 1", "Keep 2")

label    bounds(400, 24, 60, 12), text("FFT Size"), fontcolour("black")
combobox bounds(400, 37, 60, 20), channel("FFTsize"), text("64","128","256","512","1024","2048","4096","8192"), value(6)

image   bounds(478, 11, 54, 53), colour(30,30,30), shape("ellipse"), outlinethickness(0)
rslider bounds(470, 10, 70, 70), text("Port."),     channel("port"),      range(0,30.00, 0.05,0.5,0.01), textcolour("black"),colour("DarkSlateGrey"), trackercolour("LightBlue")
image   bounds(548, 11, 54, 53), colour(30,30,30), shape("ellipse"), outlinethickness(0)
rslider bounds(540, 10, 70, 70), text("Mix"),       channel("mix"),       range(0, 1.00, 0.5),      textcolour("black"),colour("DarkSlateGrey"), trackercolour("LightBlue")
image   bounds(618, 11, 54, 53), colour(30,30,30), shape("ellipse"), outlinethickness(0)
rslider bounds(610, 10, 70, 70), text("Level"),     channel("lev"),       range(0, 1.00, 0.5, 0.5), textcolour("black"),colour("DarkSlateGrey"), trackercolour("LightBlue")
</Cabbage>

<CsoundSynthesizer>
<CsOptions>
-d -n
</CsOptions>
<CsInstruments>

sr 		= 	44100
ksmps 		= 	32
nchnls 		= 	2
0dbfs		=	1	;MAXIMUM AMPLITUDE

;Iain McCurdy
;http://iainmccurdy.org/csound.html
;Pitch shifting effect using pvs scale opcode.

/* FFT attribute tables */
giFFTsizes	ftgen	0, 0, 8, -2, 64, 128, 256, 512, 1024, 2048, 4096, 8192

opcode	pvscale_module,a,akkkkkki
	ain,kscale,kformants,kfeedback,kFB_mode,kmix,klev,iFFTsize	xin
	if(kFB_mode==0) then
	 f_FB		pvsinit iFFTsize, iFFTsize/4, iFFTsize, 1, 0			;INITIALISE FEEDBACK FSIG
	 f_anal  	pvsanal	ain, iFFTsize, iFFTsize/4, iFFTsize, 1			;ANALYSE AUDIO INPUT SIGNAL AND OUTPUT AN FSIG
	 f_mix		pvsmix	f_anal, f_FB						;MIX AUDIO INPUT WITH FEEDBACK SIGNAL
	 f_scale		pvscale f_mix, kscale					;RESCALE FREQUENCIES
	 f_FB		pvsgain f_scale, kfeedback 					;CREATE FEEDBACK F-SIGNAL FOR NEXT PASS
	 aout		pvsynth f_scale                      				;RESYNTHESIZE THE f-SIGNAL AS AN AUDIO SIGNAL
	else
	 aFB		init	0
	 f_anal  	pvsanal	ain+aFB, iFFTsize, iFFTsize/4, iFFTsize, 1		;ANALYSE AUDIO INPUT SIGNAL AND OUTPUT AN FSIG
	 f_scale		pvscale f_anal, kscale, kformants-1			;RESCALE FREQUENCIES
	 aout		pvsynth f_scale                      				;RESYNTHESIZE THE f-SIGNAL AS AN AUDIO SIGNAL
	 aFB		=	aout*kfeedback
	endif	
	amix		ntrpol		ain, aout, kmix					;CREATE DRY/WET MIX
			xout		amix*klev	
endop

opcode	pvscale_module_iter,a,akkikkip
	ain,kscale,kformants,iIter,kFB_mode,kporttime,iFFTsize,iCount	xin
	aout,amix	init	0
	f_anal  	pvsanal	ain, iFFTsize, iFFTsize/4, iFFTsize, 1		;ANALYSE AUDIO INPUT SIGNAL AND OUTPUT AN FSIG
	kscaleL		portk	kscale^iCount, kporttime 
	f_scale		pvscale f_anal, kscaleL, kformants-1		;RESCALE FREQUENCIES
	aout		pvsynth f_scale
	if iCount<iIter then
	 amix	pvscale_module_iter	ain,kscale,kformants,iIter,kFB_mode,kporttime,iFFTsize,iCount+1
	endif	
	                      				;RESYNTHESIZE THE f-SIGNAL AS AN AUDIO SIGNAL
			xout		aout+amix	
endop

instr	1
	/* GUI WIDGETS SHOWING AND HIDING FOR INTERVAL DEFINITION CONTROLS */
	kIntervalMode	chnget	"IntervalMode"
	if changed(kIntervalMode)==1 then
	 if kIntervalMode==1 then
	  	chnset	"visible(1)","semitone_ident"
	  	chnset	"visible(0)","ratio_ident"
	 else
	  	chnset	"visible(0)","semitone_ident"	 
	  	chnset	"visible(1)","ratio_ident"
	 endif
	endif

	/* GUI WIDGETS SHOWING AND HIDING FOR FEEDBACK CONTROLS */
	kFB_mode	chnget	"FB_mode"
	if changed(kFB_mode)==1 then
	 if kFB_mode==3 then
	  	chnset	"visible(0)","feedback_ident"
	  	chnset	"visible(1)","iter_ident"
	 else
	  	chnset	"visible(1)","feedback_ident"
	  	chnset	"visible(0)","iter_ident"
	 endif
	endif
	
	ainL,ainR	ins
	;ainL,ainR	diskin2	"SynthPad.wav",1,0,1
	;ainL	vco2	0.3,300,4,0.5
	ainR	=	ainL
	kmix	chnget	"mix"
	kFB	chnget	"FB"
	kIter	chnget	"Iter"
	kformants	chnget	"formants"
	
	/* SET FFT ATTRIBUTES */
	kFFTsize	chnget	"FFTsize"
	kFFTsize	init	6
	ktrig		changed	kFFTsize,kformants,kIter
	if ktrig==1 then
	 reinit update
	endif
	update:

	/* PORTAMENTO TIME FUNCTION */
	kporttime	linseg	0,0.001,1	; ramp-up function
	kport		chnget	"port"		; widget		
	kporttime	*=	kport		; combine ramp-up and widget value

	iFFTsize	table	i(kFFTsize)-1, giFFTsizes
	/*-------------------*/
	
	kfeedback	chnget	"FB"
	ksemis		chnget	"semis"
	kcents		chnget	"cents"
	kNumerator	chnget	"Numerator"
	kDenominator	chnget	"Denominator"
	
	kmix		chnget	"mix"
	klev		chnget	"lev"
	
	kscale		=	kIntervalMode = 1 ? semitone(ksemis)*cent(kcents) : kNumerator/kDenominator

	if kFB_mode==3 then
	 aoutL		pvscale_module_iter	ainL,kscale,kformants,i(kIter),kFB_mode,kporttime,iFFTsize
	 aoutR		pvscale_module_iter	ainR,kscale,kformants,i(kIter),kFB_mode,kporttime,iFFTsize
	 aoutL		ntrpol	ainL,aoutL,kmix
	 aoutR		ntrpol	ainR,aoutR,kmix
	 aoutL		*=	klev
	 aoutR		*=	klev	 
	else
	 kscale		portk	kscale, kporttime
	 aoutL		pvscale_module	ainL,kscale,kformants,kfeedback,kFB_mode,kmix,klev,iFFTsize
	 aoutR		pvscale_module	ainR,kscale,kformants,kfeedback,kFB_mode,kmix,klev,iFFTsize
	endif
	
			outs	aoutR,aoutR
endin

</CsInstruments>

<CsScore>
i 1 0 [60*60*24*7]
</CsScore>

</CsoundSynthesizer>; pvsCross.csd
; Written by Iain McCurdy, 2012.

; This effect implements cross-synthesis between the left and right inputs.
; By default, the left input is the 'source' signal and the right signal is the 'destination' signal.
;  This arrangement can be swapped with the swap button.

; LEFT----+
;         |
;      (SOURCE)                 +---LEFT
;         |                     |
;         +-CROSS-SYNTHESIS-----+
;         |                     |
;    (DESTINATION)              +---RIGHT
;         |
; RIGHT---+


; Source	-	level of the source signal sent to the output.
; Dest.		-	level of the destination signal sent to the output.
; Cross		-	level of the cross-synthesis signal sent to the output.
; Level		-	global control of the level of all signal that may be present at the output.

<Cabbage>
form caption("pvsCross") size(370, 90), pluginid("crss") style("legacy")
image            bounds(0, 0, 370, 90), colour("Cream"), outlinecolour("silver"), outlinethickness(5)

; filter switches
checkbox bounds( 10, 20, 94, 15), text("Swap Inputs"), channel("swap"), fontcolour(138, 54, 15), colour("lime")

label    bounds(15, 41, 60,13), text("FFT Size"), fontcolour(138, 54, 15)
combobox bounds(15, 55, 60,20), text("64","128","256","512","1024","2048","2048","4096","8192","16384"), channel("att_table"), value(5)

line     bounds(115, 10,165, 3), colour("Grey")

label   bounds(160,  5, 75, 12), text("M  I  X  E  R"), fontcolour(78, 24,  7), colour("Cream")
rslider bounds(100,18, 75, 65), text("Source"),     channel("src_lev"),   range(0, 1.00, 0),   textcolour(138, 54, 15), colour("chocolate"), trackercolour(138, 54, 15)
rslider bounds(160,18, 75, 65), text("Dest."),      channel("dst_lev"),   range(0, 1.00, 0),   textcolour(138, 54, 15), colour("chocolate"), trackercolour(138, 54, 15)
rslider bounds(220,18, 75, 65), text("Cross"),      channel("cross_lev"), range(0, 1.00, 1),   textcolour(138, 54, 15), colour("chocolate"), trackercolour(138, 54, 15)
rslider bounds(285, 8, 75, 75), text("Level"),      channel("lev"),       range(0, 1.00, 0.5), textcolour(138, 54, 15), colour("chocolate"), trackercolour(138, 54, 15)
</Cabbage>
<CsoundSynthesizer>

<CsOptions>
-d -n
</CsOptions>

<CsInstruments>
sr 		= 	44100
ksmps 		= 	64
nchnls 		= 	2
0dbfs		=	1	;MAXIMUM AMPLITUDE

;Iain McCurdy
;http://iainmccurdy.org/csound.html

/* FFT attribute tables */
giFFTattributes1	ftgen	0, 0, 4, -2,   64,  32,   64, 1
giFFTattributes2	ftgen	0, 0, 4, -2,  128,  64,  128, 1
giFFTattributes3	ftgen	0, 0, 4, -2,  256, 128,  256, 1
giFFTattributes4	ftgen	0, 0, 4, -2,  512, 128,  512, 1
giFFTattributes5	ftgen	0, 0, 4, -2, 1024, 128, 1024, 1
giFFTattributes6	ftgen	0, 0, 4, -2, 2048, 256, 2048, 1
giFFTattributes7	ftgen	0, 0, 4, -2, 2048,1024, 2048, 1
giFFTattributes8	ftgen	0, 0, 4, -2, 4096,1024, 4096, 1
giFFTattributes9	ftgen	0, 0, 4, -2, 8192,2048, 8192, 1
giFFTattributes10	ftgen	0, 0, 4, -2,16384,4096,16384, 1

opcode	pvscross_module,a,aakkkiiii
	a_src,a_dst,kampS,kampD,klev,iFFTsize,ioverlap,iwinsize,iwintype	xin
	f_src  	pvsanal	a_src, iFFTsize, ioverlap, iwinsize, iwintype		;ANALYSE AUDIO INPUT SIGNAL AND OUTPUT AN FSIG
	f_dst  	pvsanal	a_dst, iFFTsize, ioverlap, iwinsize, iwintype		;ANALYSE AUDIO INPUT SIGNAL AND OUTPUT AN FSIG
	f_cross 	pvscross f_src, f_dst, kampS, kampD			;IMPLEMENT fsig CROSS SYNTHESIS
	aout		pvsynth f_cross                      				;RESYNTHESIZE THE f-SIGNAL AS AN AUDIO SIGNAL
			xout		aout*klev	
endop



instr	1

	katt_table	chnget	"att_table"	; FFT atribute table
	katt_table	init	5
	ktrig		changed	katt_table
	if ktrig==1 then
	 reinit update
	endif
	update:
	iFFTsize	table	0, giFFTattributes1 + i(katt_table) - 1
	ioverlap	table	1, giFFTattributes1 + i(katt_table) - 1
	iwinsize	table	2, giFFTattributes1 + i(katt_table) - 1
	iwintype	table	3, giFFTattributes1 + i(katt_table) - 1
		
	kfeedback	chnget	"FB"
	kampS		=	0;chnget	"ampS"
	kampD		=	1;chnget	"ampD"
	kporttime	linseg	0,0.001,0.02
	kmix		chnget	"mix"
	klev		chnget	"lev"
	ksrc_lev		chnget	"src_lev"
	kdst_lev		chnget	"dst_lev"
	kcross_lev		chnget	"cross_lev"
	klev		chnget	"lev"
	klisten		chnget	"listen"
	kswap		chnget	"swap"
	
	a1		inch	1
	a2		inch	2

	if kswap==0 then
	 a_src	=	a1
	 a_dst	=	a2
	else
	 a_src	=	a2
	 a_dst	=	a1
	endif
	
	aoutL		pvscross_module	a_src,a_dst,kampS,kampD,klev,iFFTsize,ioverlap,iwinsize,iwintype
	aoutR		pvscross_module	a_src,a_dst,kampS,kampD,klev,iFFTsize,ioverlap,iwinsize,iwintype

	aMixL	=	(aoutL*kcross_lev)+(a_src*ksrc_lev)+(a_dst*kdst_lev)
	aMixR	=	(aoutR*kcross_lev)+(a_src*ksrc_lev)+(a_dst*kdst_lev)

		outs	aMixL,aMixR
endin

</CsInstruments>

<CsScore>
i 1 0 [60*60*24*7]
</CsScore>

</CsoundSynthesizer>; pvsFreeze.csd
; Freeze either amplitude stream or frequency stream (or both) of a streaming FFT analysis
; Changing FFT size can modify the sound quality of the frozen sound

; 'Auto' can be used to automatically trigger a new freeze frame (both amplitude and frequency) if the amplitude of the sound crosses a threshold
; 'Delay' specifies the time delay at which the sound will actually be frozen after the threshold triggering. This can be employed to capture the sustain portion of a sound (and not the attack).
; 'Threshold' specifies the threshold level at which triggering will occur. In a noisy environment, raise 'Threshold'

<Cabbage>
form caption("pvsfreeze"), size(660,100) colour(  0,160,80), pluginid("frez") style("legacy")

image            bounds(  0,  0,270, 80), colour(  0,160,80), shape("rounded"), outlinecolour("white"), outlinethickness(4), plant("Freeze") {
checkbox bounds( 15, 10, 180, 25), channel("freezb"), text("Freeze Both"), fontcolour("white"), shape("ellipse"), colour("red")
checkbox bounds( 20, 50, 180, 15), channel("lock"), text("Lock Phases"), fontcolour("white"), shape("ellipse"), colour("LightBlue")
checkbox bounds(120, 10, 180, 25), channel("freeza"), text("Freeze Amplitudes"), fontcolour("white"), shape("ellipse"), colour("yellow")
checkbox bounds(120, 45, 180, 25), channel("freezf"), text("Freeze Frequencies"), fontcolour("white"), shape("ellipse"), colour("orange")
}

image bounds(270,  0,190, 80), colour(  0,160,80), shape("rounded"), outlinecolour("white"), outlinethickness(4), plant("Auto") {
button  bounds( 15, 25, 40, 20), channel("auto"), text("Auto","Auto"), colour:0(  0,  0,  0), fontcolour:0(100,100,100), colour:1(255,100,100), fontcolour:1(255,200,200)
rslider bounds( 60, 10, 60, 60), text("Delay"),     channel("delay"),  textcolour("white"), range(0.0001, 0.5, 0.1, 1,0.001), colour(  0,100, 50,255), trackercolour("white")
rslider bounds(120, 10, 60, 60), text("Threshold"), channel("thresh"), textcolour("white"), range(0.001, 0.5, 0.1,1,0.001),   colour(  0,100, 50,255), trackercolour("white")
}

image   bounds(460,  0,200, 80), colour(  0,160,80), shape("rounded"), outlinecolour("white"), outlinethickness(4), plant("Mix") {
label    bounds(10,15, 60,13), text("FFT Size"), fontcolour("white")
combobox bounds(10,30, 60,18), text("64","128","256","512","1024","2048","4096","8192"), channel("att_table"), value(5)
rslider bounds( 70, 10, 60, 60), text("Mix"),      channel("mix"),       range(0, 1.00, 1),        textcolour("white"), colour(  0,100, 50,255), trackercolour("white")
rslider bounds(130, 10, 60, 60), text("Level"),    channel("lev"),       range(0, 1.00, 0.5, 0.5), textcolour("white"), colour(  0,100, 50,255), trackercolour("white")
}




label   bounds( 2,85,100,10), text("Iain McCurdy . 2017"), fontcolour("white")
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-d -n
</CsOptions>
<CsInstruments>
sr 		= 	44100
ksmps 		= 	64
nchnls 		= 	2
0dbfs		=	1	;MAXIMUM AMPLITUDE

;Author: Iain McCurdy (2013)
;http://iainmccurdy.org/csound.html

/* FFT attribute tables */
giFFTattributes1	ftgen	0, 0, 4, -2,   64,  32,   64, 1
giFFTattributes2	ftgen	0, 0, 4, -2,  128,  64,  128, 1
giFFTattributes3	ftgen	0, 0, 4, -2,  256, 128,  256, 1
giFFTattributes4	ftgen	0, 0, 4, -2,  512, 128,  512, 1
giFFTattributes5	ftgen	0, 0, 4, -2, 1024, 256, 1024, 1
giFFTattributes6	ftgen	0, 0, 4, -2, 2048, 512, 2048, 1
giFFTattributes7	ftgen	0, 0, 4, -2, 4096,1024, 4096, 1
giFFTattributes8	ftgen	0, 0, 4, -2, 8192,2048, 8192, 1

opcode	pvsfreeze_module,a,akkkkiiiik
	ain,kfreeza,kfreezf,kmix,klev,iFFTsize,ioverlap,iwinsize,iwintype,klock	xin

	f_anal  	pvsanal	ain, iFFTsize, ioverlap, iwinsize, iwintype		;ANALYSE AUDIO INPUT SIGNAL AND OUTPUT AN FSIG
	f_freeze	pvsfreeze f_anal, kfreeza, kfreezf
	f_lock 		pvslock f_freeze, klock
	aout		pvsynth f_lock
	amix		ntrpol		ain, aout, kmix					;CREATE DRY/WET MIX
			xout		amix*klev	
endop

instr	1
	kmix		chnget	"mix"			; read in widgets
	klev		chnget	"lev"
	kfreeza		chnget	"freeza"
	kfreezf		chnget	"freeza"
	kfreezb		chnget	"freezb"
	klock		chnget	"lock"
	
	; triggering of 'Freeze All' mode
	kon		=	1
	koff		=	0
	ktrigon		trigger	kfreezb,0.5,0
	ktrigoff	trigger	kfreezb,0.5,1
	if(ktrigon==1) then
	 chnset		kon,"freeza"
	 chnset		kon,"freezf"
	elseif(ktrigoff==1) then
	 chnset		koff,"freeza"
	 chnset		koff,"freezf"
	endif

	; audio input
	ainL,ainR	ins

	; auto freeze triggering
	kauto	chnget	"auto"				; read in widgets
	kthresh	chnget	"thresh"
	kdelay	chnget	"delay"
	if kauto==1 then				; if 'Auto' is on
	 krms	rms	ainL+ainR			; scan RMS of audio signal
	 ktrig	trigger	krms,kthresh,0			; if signal crosses threshold upwards																																																																													
	 ktrigdel	vdel_k	ktrig,kdelay,0.5	; delayed version of the trigger
	 if ktrig==1 then				; if initial threshold crossing occurs...
	  chnset		koff,"freeza"		; turn freezing off
	  chnset		koff,"freezf"
	 endif
	 if ktrigdel==1 then				; if delayed trigger is received...
	  chnset		kon,"freeza"		; turn freezing on
	  chnset		kon,"freezf"
	 endif
	endif

	kofftrig	trigger	kauto,0.5,1		; when 'Auto' is turned off generate a trigger
	if kofftrig==1 then				; if 'Auto' is turned off...
	 chnset		koff,"freeza"			; turn freezing off
	 chnset		koff,"freezf"
	endif	

	/* SET FFT ATTRIBUTES */
	katt_table	chnget	"att_table"	; FFT atribute table
	katt_table	init	5
	ktrig		changed	katt_table
	if ktrig==1 then
	 reinit update
	endif
	update:
	iFFTsize	table	0, giFFTattributes1 + i(katt_table) - 1
	ioverlap	table	1, giFFTattributes1 + i(katt_table) - 1
	iwinsize	table	2, giFFTattributes1 + i(katt_table) - 1
	iwintype	table	3, giFFTattributes1 + i(katt_table) - 1
	/*-------------------*/
	
	aoutL		pvsfreeze_module	ainL,kfreeza,kfreezf,kmix,klev,iFFTsize,ioverlap,iwinsize,iwintype, klock
	aoutR		pvsfreeze_module	ainR,kfreeza,kfreezf,kmix,klev,iFFTsize,ioverlap,iwinsize,iwintype, klock
				outs				aoutR,aoutR
endin

</CsInstruments>

<CsScore>
i 1 0 [60*60*24*7]
</CsScore>

</CsoundSynthesizer>; pvShift.csd
; Written by Iain McCurdy, 2012.

; Streaming phase vocoding frequency shifter.

<Cabbage>
form caption("pvshift Frequency Shifter") size(510,  90), pluginid("shft"), scrollbars(0) style("legacy")

image bounds( 15,  9, 60, 60), colour(30,30,30,100), shape("ellipse"), outlinethickness(0)
image bounds( 85,  9, 60, 60), colour(30,30,30,100), shape("ellipse"), outlinethickness(0)
image bounds(155,  9, 60, 60), colour(30,30,30,100), shape("ellipse"), outlinethickness(0)
image bounds(225,  9, 60, 60), colour(30,30,30,100), shape("ellipse"), outlinethickness(0)
image bounds(295,  9, 60, 60), colour(30,30,30,100), shape("ellipse"), outlinethickness(0)
image bounds(365,  9, 60, 60), colour(30,30,30,100), shape("ellipse"), outlinethickness(0)
image bounds(435,  9, 60, 60), colour(30,30,30,100), shape("ellipse"), outlinethickness(0)
image bounds(-60,-5,630,110), colour(100,205,60), outlinecolour("silver"), outlinethickness(6), shape("ellipse")
rslider bounds( 10, 15, 70, 70), text("Coarse"),     channel("coarse"),    range(-4000, 4000, 0, 1),    colour("orange"), trackercolour("yellow"), textcolour("black")
rslider bounds( 80, 15, 70, 70), text("Multiplier"), channel("fine"),      range(-1, 1, 1, 1, 0.0001),  colour("orange"), trackercolour("yellow"), textcolour("black")
rslider bounds(150, 15, 70, 70), text("Lowest"),     channel("lowest"),    range( 20, 20000, 20, 0.5),  colour("orange"), trackercolour("yellow"), textcolour("black")
rslider bounds(220, 15, 70, 70), text("Feedback"),   channel("FB"),        range(0, 1.00, 0),           colour("orange"), trackercolour("yellow"), textcolour("black")
label    bounds(295,20, 60,13), text("FFT Size"), fontcolour("black")
combobox bounds(295,35, 60,20), text("64","128","256","512","1024","2048","4096","8192"), channel("att_table"), value(5), fontcolour(220,220,255)
rslider bounds(360, 15, 70, 70), text("Mix"),        channel("mix"),       range(0, 1.00, 1),           colour("orange"), trackercolour("yellow"), textcolour("black")
rslider bounds(430, 15, 70, 70), text("Level"),      channel("lev"),       range(0, 1.00, 0.5, 0.5),    colour("orange"), trackercolour("yellow"), textcolour("black")
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-d -n
</CsOptions>
<CsInstruments>

sr 		= 	44100
ksmps 		= 	32
nchnls 		= 	2
0dbfs		=	1	;MAXIMUM AMPLITUDE

;Iain McCurdy
;http://iainmccurdy.org/csound.html
;Frequency shifting effect using pvshift opcode.

/* FFT attribute tables */
giFFTattributes1	ftgen	0, 0, 4, -2,   64,  32,   64, 1
giFFTattributes2	ftgen	0, 0, 4, -2,  128,  64,  128, 1
giFFTattributes3	ftgen	0, 0, 4, -2,  256, 128,  256, 1
giFFTattributes4	ftgen	0, 0, 4, -2,  512, 128,  512, 1
giFFTattributes5	ftgen	0, 0, 4, -2, 1024, 256, 1024, 1
giFFTattributes6	ftgen	0, 0, 4, -2, 2048, 512, 2048, 1
giFFTattributes7	ftgen	0, 0, 4, -2, 4096,1024, 4096, 1
giFFTattributes8	ftgen	0, 0, 4, -2, 8192,2048, 8192, 1

opcode	pvshift_module,a,akkkkkiiii
	ain,kshift,klowest,kfeedback,kmix,klev,iFFTsize,ioverlap,iwinsize,iwintype	xin
	f_FB		pvsinit iFFTsize,ioverlap,iwinsize,iwintype, 0			; INITIALISE FEEDBACK FSIG
	f_anal  	pvsanal	ain, iFFTsize, ioverlap, iwinsize, iwintype		; ANALYSE AUDIO INPUT SIGNAL AND OUTPUT AN FSIG
	f_mix		pvsmix	f_anal, f_FB									; MIX AUDIO INPUT WITH FEEDBACK SIGNAL
	f_shift		pvshift f_mix, kshift, klowest							; SHIFT FREQUENCIES
	f_FB		pvsgain f_shift, kfeedback 								; CREATE FEEDBACK F-SIGNAL FOR NEXT PASS
	aout		pvsynth f_shift                      					; RESYNTHESIZE THE f-SIGNAL AS AN AUDIO SIGNAL
	amix		ntrpol		ain, aout, kmix								; CREATE DRY/WET MIX
			xout		amix*klev	
endop

instr	1
	ainL,ainR	ins
	;gicos	ftgen	0,0,131072,11,1			; test tone
	;ainL	gbuzz	0.1,330,80,1,0.7,gicos	; test tone
	ainR	=		ainL
	
	
	;ainL,ainR	diskin2	"SynthPad.wav",1,0,1
	kcoarse	chnget	"coarse"	; coarse freq. shift
	kfine	chnget	"fine"		; fine freq. control (multipler)
	kporttime	linseg	0,0.001,0.2
	kshift	lineto	kcoarse*kfine, kporttime	; ultimate freq. shift is sum of coarse and fine controls
	klowest	chnget	"lowest"	; lowest shifted frequency
	kFB	chnget	"FB"		; feedback amount
	kmix		chnget	"mix"
	klev		chnget	"lev"

	/* SET FFT ATTRIBUTES */
	katt_table	chnget	"att_table"	; FFT atribute table
	katt_table	init	5
	ktrig		changed	katt_table
	if ktrig==1 then
	 reinit update
	endif
	update:
	iFFTsize	table	0, giFFTattributes1 + i(katt_table) - 1
	ioverlap	table	1, giFFTattributes1 + i(katt_table) - 1
	iwinsize	table	2, giFFTattributes1 + i(katt_table) - 1
	iwintype	table	3, giFFTattributes1 + i(katt_table) - 1
	/*-------------------*/
	
	aoutL		pvshift_module	ainL,kshift,klowest,kFB,kmix,klev,iFFTsize,ioverlap,iwinsize,iwintype	; call UDO for each channel
	aoutR		pvshift_module	ainR,kshift,klowest,kFB,kmix,klev,iFFTsize,ioverlap,iwinsize,iwintype

			outs	aoutR,aoutR
endin

</CsInstruments>

<CsScore>
i 1 0 [60*60*24*7]
</CsScore>

</CsoundSynthesizer>; pvSmooth.csd
; Written by Iain McCurdy, 2012.
; FFT feedback is disabled if amplitude smoothing is increased beyond zero. If this is not done the instrument will fail. 

; link	-	links 'Amp.Smooth' and 'Frq.Smooth'

<Cabbage>
form caption("pvSmooth") size(505, 90), pluginid("smoo"), scrollbars(0) style("legacy")
image bounds(0, 0, 505, 90), colour("Cream"), outlinecolour("silver"), outlinethickness(5)
label pos(-52, -30), size(615, 150), fontcolour(210,105, 30, 80), text("smooth"), shape("rounded"), outlinecolour("white"), outlinethickness(4)
rslider bounds( 10, 8, 75, 75), text("Amp.Smooth"), channel("acf"),       range(0, 1.00, 0,16, 0.0001), textcolour(138, 54, 15), colour("chocolate"), trackercolour(138, 54, 15)
checkbox bounds( 70, 8, 70,10), channel("link"), text("Link"), fontcolour(138, 54, 15)
rslider bounds( 90, 8, 75, 75), text("Frq.Smooth"), channel("fcf"),       range(0, 1.00, 0,16, 0.0001), textcolour(138, 54, 15), colour("chocolate"), trackercolour(138, 54, 15)
rslider bounds(170, 8, 75, 75), text("Feedback"),   channel("FB"),        range(0, 0.999, 0, 1,0.001),    textcolour(138, 54, 15), colour("chocolate"), trackercolour(138, 54, 15)
label    bounds(250,20, 70,13), text("FFT Size"), fontcolour(138, 54, 15)
combobox bounds(250,35, 70,20), text("64","128","256","512","1024","2048","2048_2","4096","8192","16384"), channel("att_table"), value(5), fontcolour(255,255,200)
checkbox bounds(250,60, 90,10), channel("delay"), text("Delay Comp."), fontcolour(138, 54, 15)
rslider bounds(330, 8, 75, 75), text("Mix"),        channel("mix"),       range(0, 1.00, 1),              textcolour(138, 54, 15), colour("chocolate"), trackercolour(138, 54, 15)
rslider bounds(410, 8, 75, 75), text("Level"),      channel("lev"),       range(0, 1.00, 0.5),            textcolour(138, 54, 15), colour("chocolate"), trackercolour(138, 54, 15)
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-d -n
</CsOptions>
<CsInstruments>
sr 		= 	44100
ksmps 		= 	32
nchnls 		= 	2
0dbfs		=	1	;MAXIMUM AMPLITUDE

;Iain McCurdy
;http://iainmccurdy.org/csound.html
;Spectral smoothing effect.

/* FFT attribute tables */
giFFTattributes1	ftgen	0, 0, 4, -2,   64,  32,   64, 1
giFFTattributes2	ftgen	0, 0, 4, -2,  128,  64,  128, 1
giFFTattributes3	ftgen	0, 0, 4, -2,  256, 128,  256, 1
giFFTattributes4	ftgen	0, 0, 4, -2,  512, 128,  512, 1
giFFTattributes5	ftgen	0, 0, 4, -2, 1024, 128, 1024, 1
giFFTattributes6	ftgen	0, 0, 4, -2, 2048, 256, 2048, 1
giFFTattributes7	ftgen	0, 0, 4, -2, 2048,1024, 2048, 1
giFFTattributes8	ftgen	0, 0, 4, -2, 4096,1024, 4096, 1
giFFTattributes9	ftgen	0, 0, 4, -2, 8192,2048, 8192, 1
giFFTattributes10	ftgen	0, 0, 4, -2,16384,4096,16384, 1

opcode	pvsmooth_module,a,akkkkkiiiii
	ain,kacf,kfcf,kfeedback,kmix,klev,iFFTsize,ioverlap,iwinsize,iwintype,idelay	xin
	f_FB		pvsinit iFFTsize,ioverlap,iwinsize,iwintype, 0			;INITIALISE FEEDBACK FSIG
	f_anal  	pvsanal	ain, iFFTsize, ioverlap, iwinsize, iwintype		;ANALYSE AUDIO INPUT SIGNAL AND OUTPUT AN FSIG
	f_mix		pvsmix	f_anal, f_FB						;MIX AUDIO INPUT WITH FEEDBACK SIGNAL
	f_smooth	pvsmooth	f_mix, kacf, kfcf				;BLUR AMPLITUDE AND FREQUENCY VALUES OF AN F-SIGNAL
	f_FB		pvsgain f_smooth, kfeedback 					;CREATE FEEDBACK F-SIGNAL FOR NEXT PASS
	aout		pvsynth f_smooth                      				;RESYNTHESIZE THE f-SIGNAL AS AN AUDIO SIGNAL
	if idelay==1 then
	 ain	delay	ain,(iFFTsize)/sr
	endif
	amix		ntrpol		ain, aout, kmix					;CREATE DRY/WET MIX
			xout		amix*klev	
endop

instr	1
	ainL,ainR	ins
	;ainL,ainR	diskin	"808loop.wav",1,0,1	;USE FOR TESTING
	;ainL,ainR	diskin	"SynthPad.wav",1,0,1	;USE FOR TESTING

	katt_table	chnget	"att_table"	; FFT atribute table
	katt_table	init	5
	kdelay		chnget	"delay"
	ktrig		changed	katt_table,kdelay
	if ktrig==1 then
	 reinit update
	endif
	update:
	iFFTsize	table	0, giFFTattributes1 + i(katt_table) - 1
	ioverlap	table	1, giFFTattributes1 + i(katt_table) - 1
	iwinsize	table	2, giFFTattributes1 + i(katt_table) - 1
	iwintype	table	3, giFFTattributes1 + i(katt_table) - 1
		
	kfeedback	chnget	"FB"
	kacf		chnget	"acf"
	kfcf		chnget	"fcf"
	klink		chnget	"link"
	if klink==1 then
	 if changed(kacf)==1 then
	  chnset	kacf,"fcf"
	 elseif changed(kfcf)==1 then
	  chnset	kfcf,"acf"
	 endif
	endif
	kfeedback	=	(kacf>0?0:kfeedback)		; feedback + amplitude smoothing can cause failure so we must protect against this
	kacf		=	1-kacf
	kfcf		=	1-kfcf
	kporttime	linseg	0,0.001,0.02
	kmix		chnget	"mix"
	klev		chnget	"lev"
	aoutL		pvsmooth_module	ainL,kacf,kfcf,kfeedback,kmix,klev,iFFTsize,ioverlap,iwinsize,iwintype,i(kdelay)
	aoutR		pvsmooth_module	ainR,kacf,kfcf,kfeedback,kmix,klev,iFFTsize,ioverlap,iwinsize,iwintype,i(kdelay)
			outs	aoutR,aoutR
endin

</CsInstruments>

<CsScore>
i 1 0 [60*60*24*7]
</CsScore>

</CsoundSynthesizer>; pvsWarp.csd
; Written by Iain McCurdy, 2012.

<Cabbage>
form caption("pvsWarp") size(535, 90), pluginid("warp") style("legacy")
image pos(0, 0), size(535, 90), colour( 80, 80,135,220), shape("rounded"), outlinecolour("white"), outlinethickness(4) 
rslider bounds( 10, 11, 70, 70), text("Scale"),    channel("scal"),      range(0.1, 4, 1, 0.5, 0.001), colour("LightSlateGrey"), textcolour("white"), trackercolour("white")
rslider bounds( 80, 11, 70, 70), text("Shift"),    channel("shift"),     range(-5000, 5000, 0),        colour("LightSlateGrey"), textcolour("white"), trackercolour("white")
rslider bounds(150, 11, 70, 70), text("Feedback"), channel("FB"),        range(0, 0.99, 0),            colour("LightSlateGrey"), textcolour("white"), trackercolour("white")
label    bounds(220,20, 60,13), text("FFT Size"), fontcolour("white")
combobox bounds(220,35, 60,20), text("128","256","512","1024","2048","4096","8192"), channel("att_table"), value(4), fontcolour("lightblue")
checkbox bounds(290,30, 95,15), channel("DelayComp"), text("Delay Comp."), fontcolour("white"), colour("lightblue")
rslider bounds(385, 11, 70, 70), text("Mix"),      channel("mix"),       range(0, 1.00, 1),            colour("LightSlateGrey"), textcolour("white"), trackercolour("white")
rslider bounds(455, 11, 70, 70), text("Level"),    channel("lev"),       range(0, 1.00, 0.5),          colour("LightSlateGrey"), textcolour("white"), trackercolour("white")
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-d -n
</CsOptions>

<CsInstruments>

sr 		= 	44100
ksmps 		= 	32
nchnls 		= 	2
0dbfs		=	1	;MAXIMUM AMPLITUDE

;Author: Iain McCurdy (2012)
;http://iainmccurdy.org/csound.html

/* FFT attribute tables */
giFFTattributes1	ftgen	0, 0, 4, -2,   64,  32,   64, 1
giFFTattributes2	ftgen	0, 0, 4, -2,  128,  64,  128, 1
giFFTattributes3	ftgen	0, 0, 4, -2,  256, 128,  256, 1
giFFTattributes4	ftgen	0, 0, 4, -2,  512, 128,  512, 1
giFFTattributes5	ftgen	0, 0, 4, -2, 1024, 256, 1024, 1
giFFTattributes6	ftgen	0, 0, 4, -2, 2048, 512, 2048, 1
giFFTattributes7	ftgen	0, 0, 4, -2, 4096,1024, 4096, 1
giFFTattributes8	ftgen	0, 0, 4, -2, 8192,2048, 8192, 1

opcode	pvswarp_module,a,akkkkiiiii
	ain,kscal,kshift,kfeedback,kmix,iFFTsize,ioverlap,iwinsize,iwintype,iDelayComp	xin
	aout		init	0
	;f_FB		pvsinit iFFTsize,ioverlap,iwinsize,iwintype, 0			;INITIALISE FEEDBACK FSIG
	f_anal  	pvsanal	ain+(aout*kfeedback), iFFTsize, ioverlap, iwinsize, iwintype		;ANALYSE AUDIO INPUT SIGNAL AND OUTPUT AN FSIG
	;f_mix		pvsmix	f_anal, f_FB						;MIX AUDIO INPUT WITH FEEDBACK SIGNAL
	f_warp		pvswarp	f_anal, kscal, kshift					;WARP SPECTRAL ENVELOPE VALUES OF AN F-SIGNAL USING BOTH SCALING AND SHIFTING
	;f_FB		pvsgain f_warp, kfeedback 					;CREATE FEEDBACK F-SIGNAL FOR NEXT PASS
	aout		pvsynth f_warp                      				;RESYNTHESIZE THE f-SIGNAL AS AN AUDIO SIGNAL
	if(kfeedback>0) then
	 aout		clip	aout,0,0dbfs
	endif
	if iDelayComp==1 then
	 ain	delay	ain,iwinsize/sr
	endif
	amix		ntrpol		ain, aout, kmix					;CREATE DRY/WET MIX
			xout		amix	
endop

instr	1
	ainL,ainR	ins
	;ainL,ainR	diskin	"SynthPad.wav",1,0,1	;USE FOR TESTING
	kscal		chnget	"scal"
	kshift		chnget	"shift"
	kfeedback	chnget	"FB"
	kmix		chnget	"mix"
	klev		chnget	"lev"
	kDelayComp	chnget	"DelayComp"
	ainL		=	ainL*klev
	ainR		=	ainR*klev

	/* SET FFT ATTRIBUTES */
	katt_table	chnget	"att_table"	; FFT atribute table
	katt_table	init	5
	ktrig		changed	katt_table,kDelayComp
	if ktrig==1 then
	 reinit update
	endif
	update:
	iFFTsize	table	0, giFFTattributes1 + i(katt_table) - 1
	ioverlap	table	1, giFFTattributes1 + i(katt_table) - 1
	iwinsize	table	2, giFFTattributes1 + i(katt_table) - 1
	iwintype	table	3, giFFTattributes1 + i(katt_table) - 1
	/*-------------------*/

	aoutL		pvswarp_module	ainL,kscal,kshift,kfeedback,kmix,iFFTsize,ioverlap,iwinsize,iwintype,i(kDelayComp)
	aoutR		pvswarp_module	ainR,kscal,kshift,kfeedback,kmix,iFFTsize,ioverlap,iwinsize,iwintype,i(kDelayComp)
			outs	aoutL,aoutR
endin

</CsInstruments>

<CsScore>
i 1 0 [60*60*24*7]
</CsScore>

</CsoundSynthesizer>
; RingModulator.csd
; Written by Iain McCurdy, 2012.

; Ring Modulator
; --------------
; Modulating waveform is a harmonic waveform based on a sawtooth waveform.
;  The number of harmonics included is defined using 'Harms'.
;  An offset of harmonics to omit is defined using 'Offset'.
;  Therefore Harms=1 and Offset=0 will produce a sine wave.

; Input		-	input mode 

; Freq.		-	frequency of the ring modulator (if input mode is 'Knob')
;			 If input mode is 'Kybd.' then base frequency is derived from the note played and the 'Freq.' knob is disregarded.
; Harms.	-	number of harmonics included in the modulating waveform
; Offset	-	number of harmonics to skip before building the harmonic waveform used by the modulator

; Env.		-	amount of amplitude following used to influence the modulator frequency
; Att.		-	attack time used in amplitude following
; Dec.		-	decay time used in amplitude following

; Thresh	-	threshold which, if the rms (amplitude measurement) crosses through, will trigger the generation of a new random modulator frequency.
;			 If thresh is zero, the random triggering of new modulating frequencies is bypassed
; Min.		-	minimum of the range within which a new modulating frequency will be generated (oct format)
; Max.		-	maximum of the range within which a new modulating frequency will be generated (oct format)

; Width		-	create stereo width by offsetting the phase of the left channel modulator with respect to that of the right
; Mix		-	dry/wet mix
; Level		-	output level

<Cabbage>
form caption("Ring Modulator") size(755,170), pluginid("rmod") style("legacy")
image               pos(0, 0), size(755, 90), colour("black"), shape("rounded"), outlinecolour("lime"), outlinethickness(5) 
label    bounds( 10, 20, 65, 13), text("Input"), textcolour(white)
combobox bounds( 10, 34, 65, 18), text("Knob","Keybd."), channel("input"), textcolour(white)
rslider bounds( 75, 12, 70, 70), text("Freq."),    channel("freq"),  range(0, 15000, 800, 0.25),   colour("yellow"),    trackercolour(white), textcolour(white)
rslider bounds(145, 12, 70, 70), text("Harms"),    channel("harms"), range(1, 40, 1,1,1),          colour("yellow"),    trackercolour(white), textcolour(white)
rslider bounds(215, 12, 70, 70), text("Offset"),   channel("offset"),range(0, 20, 0,1,1),          colour("yellow"),    trackercolour(white), textcolour(white)

line    bounds(290,  2, 3, 86), colour("lime")
rslider bounds(300, 12, 70, 70), text("Env."),     channel("env"),   range(0, 1.00, 0.5, 0.5,0.001), colour("orange"),    trackercolour(white), textcolour(white)
rslider bounds(370,  6, 40, 40), text("Att."),     channel("att"),   range(0.001, 2, 0.01, 0.5),   colour("orange"),    trackercolour(white), textcolour(white)
rslider bounds(370, 46, 40, 40), text("Dec."),     channel("dec"),   range(0.001, 2, 0.1,  0.5),   colour("orange"),    trackercolour(white), textcolour(white)
line    bounds(415,  2, 3, 86), colour("lime")

rslider bounds(425, 12, 70, 70), text("Thresh"),   channel("thresh"),   range(0, 1.00, 0, 0.5,0), colour("orange"),    trackercolour(white), textcolour(white)
rslider bounds(490,  6, 40, 40), text("Min."),     channel("min"),   range(0, 13, 5),   colour("orange"),    trackercolour(white), textcolour(white)
rslider bounds(490, 46, 40, 40), text("Max."),     channel("max"),   range(0, 13, 8),   colour("orange"),    trackercolour(white), textcolour(white)
line    bounds(535,  2, 3, 86), colour("lime")


rslider bounds(540, 12, 70, 70), text("Width"),    channel("width"), range(0, 1.0, 0.3,1,0.001),   colour("chocolate"), trackercolour(white), textcolour(white)
rslider bounds(610, 12, 70, 70), text("Mix"),      channel("mix"),   range(0, 1.00, 1),            colour("chocolate"), trackercolour(white), textcolour(white)
rslider bounds(680, 12, 70, 70), text("Level"),    channel("level"), range(0, 1.00, 1),            colour("chocolate"), trackercolour(white), textcolour(white)

keyboard bounds(0,92,755,78)
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
;-d -n
-dm0 -n -+rtmidi=null -M0
</CsOptions>
<CsInstruments>
sr = 44100
ksmps = 32
nchnls = 2
0dbfs = 1
massign	0,2

;Author: Iain McCurdy (2012)
;http://iainmccurdy.org/csound.html

gisaw	ftgen	0,0,4096,7,1,4096,-1	; source modulating waveform (unbandlimited sawtooth waveform) before harmonic filtering

opcode	RingModulator,a,akkkkkii
	ain,kmix,kfreq,kenv,katt,kdec,ifn,iphs	xin			;READ IN INPUT ARGUMENTS
	iWet	ftgentmp	0,0,1024,-7,0,512,1,512,1		;RESCALING FUNCTION FOR WET LEVEL CONTROL
	iDry	ftgentmp	0,0,1024,-7,1,512,1,512,0		;RESCALING FUNCTION FOR DRY LEVEL CONTROL
	kWet	table	kmix, iWet, 1					;RESCALE WET LEVEL CONTROL ACCORDING TO FUNCTION TABLE iWet
	kDry	table	kmix, iDry, 1					;RESCALE DRY LEVEL CONTROL ACCORDING TO FUNCTION TABLE iDry
	aFollow	follow2		ain, katt, kdec				;AMPLITUDE FOLLOWING kModFrq + (cpsoct(kFollow*kenv*30))     	;CREATE A LEFT CHANNEL MODULATING FREQUENCY BASE ON THE STATIC VALUE CREATED BY kfreq AND THE AMOUNT OF DYNAMIC ENVELOPE FOLLOWING GOVERNED BY kenv
	kFollow	downsamp	aFollow
	kFollow	logcurve	kFollow/0dbfs,2
	kfreq	=	kfreq + (kFollow*kenv*8000)     		;CREATE A LEFT CHANNEL MODULATING FREQUENCY BASE ON THE STATIC VALUE CREATED BY kfreq AND THE AMOUNT OF DYNAMIC ENVELOPE FOLLOWING GOVERNED BY kenv
	aMod	poscil	1, kfreq, ifn, iphs  			;CREATE RING MODULATING SIGNAL
	aout	sum	ain*kDry, ain*aMod*kWet				;MIX DRY AND WET SIGNALS
		xout	aout						;SEND AUDIO BACK TO CALLER INSTRUMENT
endop

instr	1
 kporttime	linseg	0,0.001,0.1
 gkfreq	chnget	"freq"
 gkfreq	portk	gkfreq,kporttime
 gkmix 	chnget	"mix"
 gkwidth	chnget	"width"
 gkenv 	chnget	"env"
 gkatt 	chnget	"att"
 gkdec 	chnget	"dec"
 gkthresh 	chnget	"thresh"
 gkmin 	chnget	"min"
 gkmax 	chnget	"max"
 gklevel 	chnget	"level"
 gkharms		chnget	"harms"
 gkoffset	chnget	"offset"

 ga1,ga2	ins
 ;ga1,ga2	diskin2	"808loop.wav",1,0,1

 gkinput	chnget	"input"
 gkinput	init	1
 if changed(gkinput)==1&&gkinput==1 then
  event	"i",2,0,-1
 endif
endin

instr	2
 /* MIDI AND GUI INTEROPERABILITY */
 iMIDIflag	=	0			; IF MIDI ACTIVATED = 1, NON-MIDI = 0
 mididefault	1, iMIDIflag			; IF NOTE IS MIDI ACTIVATED REPLACE iMIDIflag WITH '1'
 
 if iMIDIflag==1 then				; IF THIS IS A MIDI ACTIVATED NOTE...
  icps	cpsmidi					; READ MIDI PITCH VALUES - THIS VALUE CAN BE MAPPED TO GRAIN DENSITY AND/OR PITCH DEPENDING ON THE SETTING OF THE MIDI MAPPING SWITCHES
  kfreq	=		icps
 else
  kfreq 	=	gkfreq
 endif						; END OF THIS CONDITIONAL BRANCH

 if gkinput!=1&&iMIDIflag!=1 then
  turnoff
 endif
 
 if gkthresh>0 then
  a1,a2	init	0
  krms	rms	a1+a2
  kRTrig	init	1
  kfreq	trandom	kRTrig,gkmin,gkmax
  kfreq	=	cpsoct(kfreq)
  if changed(kfreq)==1 then
   chnset	kfreq,"freq"
  endif
  kRTrig	trigger	krms,gkthresh,0
 endif
 
 if metro(8)==1 then 
  if changed(gkharms,gkoffset,gkwidth)==1 then
   reinit UPDATE
  endif
  UPDATE:
 endif
 ifn	ftgen	1, 0, 4096, 30, gisaw, 1+i(gkoffset), i(gkharms)+i(gkoffset)		; generate a waveform based on chosen source waveform with user-set modifications
 ;ifn	ftgen	1, 0, 4096, 10, 1
 a1	RingModulator	ga1,gkmix,kfreq,gkenv,gkatt,gkdec,ifn,i(gkwidth)
 a2	RingModulator	ga2,gkmix,kfreq,gkenv,gkatt,gkdec,ifn,0
 rireturn
 aEnv	linsegr	0,0.01,1,0.01,0
 a1	=	a1 * gklevel * aEnv
 a2	=	a2 * gklevel * aEnv
 	outs	a1,a2
endin

</CsInstruments>

<CsScore>
i 1 0 [60*60*24*7]
</CsScore>

</CsoundSynthesizer>; Shredulator.csd
; Written by Iain McCurdy, 2016

; This effect implements an FFT delay, the delay time of which can be shifted using a random sample and gold function.
; In sync with this random function, amplitude can also be randomised ('Granulate' control), as can transposition.

; CONTROLS
; --------

; FFT Size		-	size of FFT window: smaller sizes provides better time resolution but possible distortion of frequency components
; Max Delay		-	Maximum delay time as defined by the circular (pvs) buffer. This is i-rate so altering it will cause discontinuities in the realtime audio stream.
;					 Note that maximum delay time can modulated at k-rate using the 'Depth' control
; Rate			-	Rate at which new random delay times are generated. This also controls the rate at which new random amplitude ('Granulation') 
;  					 and random transposition ('Randomise') values are generated.
;					This control can also be thought of as 'grain size'.
; Depth			-	Amplitude of the random delay time generator. This, along with 'Max.Delay', controls the maximum random delay time possible.
; Granulation	-	Amount of random amplitude variation
; Feedback		-	Ratio of output (pvs) signal that is fed back into the input

; Semitones		-	Number of semitones transposition
; Cents			-	Number of cents transposition
; Pre/Post		-	If 'Pre' is selected, the signal before transposition is sent to the output (transposition are only heard via the feedback signal), 
;					 if 'Post' is selected the, the transposed signal is sent directly to the output
; Randomise		-	If raised above zero, rather than transposition being a fixed value as defined by 'Semitones' and 'Cents' 
;               	 it will be a bipolar random value in the range +/- the offset defined by 'Semitones' and 'Cents'.


<Cabbage>
form caption("") size(460,395), colour(225,230,255), pluginid("Shrd"), scrollbars(0) style("legacy")
image   bounds(  0,-10,460, 45), colour(0,0,0,0), outlinethickness(0), plant("title")
{
label   bounds(  0,  1,460, 51), text("SHREDULATOR"), fontcolour(155,155,155)
image   bounds( 55, 21,325,  5),   colour(225,230,255), shape("sharp"), rotate(0.1,162,2)
image   bounds( 90, 21,200,  7),   colour(225,230,255), shape("sharp"), rotate(-0.18,100,3)

label   bounds(  1,  3,459, 50), text("SHREDULATOR"), fontcolour(  5,  5,  5)
image   bounds( 45, 21, 80,  2),   colour(225,230,255), shape("sharp"), rotate(-0.8,40,1)
image   bounds(110, 21, 55,  2),   colour(225,230,255), shape("sharp"), rotate( 0.8,22,1)
image   bounds( 85, 34,100,  3),   colour(225,230,255), shape("sharp"), rotate(-0.4,22,1)
image   bounds(160, 34,180,  3),   colour(225,230,255), shape("sharp"), rotate(-0.2,45,2)
image   bounds(215, 17, 70,  4),   colour(225,230,255), shape("sharp"), rotate( 0.5,22,2)
image   bounds(250, 21,150,  4),   colour(225,230,255), shape("sharp"), rotate( 0.2,60,2)
image   bounds(271, 31, 53,  2),   colour(225,230,255), shape("sharp"), rotate(-0.1,22,1)
image   bounds(340, 21, 60,  2),   colour(225,230,255), shape("sharp"), rotate( 0.7,22,1)
}

label bounds(358, 39,97,10), fontcolour("black"), text("Iain McCurdy |2016|") align("right")

image    bounds(5,50,450,110), colour(225,230,255), outlinecolour("Black"), outlinethickness(5), plant("Delay")
{  
label    bounds(  0,  3,440, 13), text("D    E    L    A    Y"), fontcolour("Black")
label    bounds( 10, 20, 70, 14), text("FFT Size"), fontcolour("Black")
combobox bounds( 10, 35, 70, 20), text("128","256","512","1024","2048","4096"), channel("FFTindex"), value(4)
rslider  bounds( 75, 15, 90,90), text("Max.Delay"), textbox(1), valuetextbox(1), channel("MaxDelay"), range(0.1, 8, 3.7,0.5), fontcolour("Black"), textcolour("Black"), colour(20,20,155), trackercolour(150,150,225)
rslider  bounds(145, 15, 90,90), text("Rate"), textbox(1), valuetextbox(1), channel("Rate"), range(0.1, 500, 10,0.5,0.1), fontcolour("Black"), textcolour("Black"), colour(20,20,155), trackercolour(150,150,225)
rslider  bounds(215, 15, 90,90), text("Depth"), textbox(1), valuetextbox(1), channel("Depth"), range(0, 1, 0.2), fontcolour("Black"), textcolour("Black"), colour(20,20,155), trackercolour(150,150,225)
rslider  bounds(285, 15, 90,90), text("Granulation"), textbox(1), valuetextbox(1), channel("Granulation"), range(0, 1, 0.2), fontcolour("Black"), textcolour("Black"), colour(20,20,155), trackercolour(150,150,225)
rslider  bounds(355, 15, 90,90), text("Feedback"), textbox(1), valuetextbox(1), channel("Feedback"), range(0, 1, 0.5), fontcolour("Black"), textcolour("Black"), colour(20,20,155), trackercolour(150,150,225)
}

image    bounds(5,165,450,110), colour(225,230,255), outlinecolour("Black"), outlinethickness(5), plant("Transpose")
{  
label    bounds(  0,  3,450, 13), text("T    R    A    N    S    P    O    S    E"), fontcolour("Black")
rslider  bounds( 60, 15, 90,90), text("Semitones"), textbox(1), valuetextbox(1), channel("Semitones"), range(-12, 12, 5,1,1), fontcolour("Black"), textcolour("Black"), colour(20,20,155), trackercolour(150,150,225)
rslider  bounds(130, 15, 90,90), text("Cents"), textbox(1), valuetextbox(1), channel("Cents"), range(-100, 100, 0,1,1), fontcolour("Black"), textcolour("Black"), colour(20,20,155), trackercolour(150,150,225)
label    bounds(220, 20, 70, 14), text("Pre/Post"), fontcolour("Black")
combobox bounds(220, 35, 70, 20), text("Pre","Post"), channel("PrePost"), value(2)
rslider  bounds(290, 15, 90,90), text("Randomise"), textbox(1), valuetextbox(1), channel("TransRand"), range(0, 1, 0.35), fontcolour("Black"), textcolour("Black"), colour(20,20,155), trackercolour(150,150,225)
}

image    bounds(5,280,450,110), colour(225,230,255), outlinecolour("Black"), outlinethickness(5), plant("Output")
{  
label    bounds(  0,  3,450, 13), text("O    U    T    P    U    T"), fontcolour("Black")
rslider  bounds(110, 15, 90,90), text("Width"), textbox(1), valuetextbox(1), channel("Width"), range(0, 1, 1), fontcolour("Black"), textcolour("Black"), colour(20,20,155), trackercolour(150,150,225)
rslider  bounds(180, 15, 90,90), text("Dry/Wet Mix"), textbox(1), valuetextbox(1), channel("DryWetMix"), range(0, 1, 1), fontcolour("Black"), textcolour("Black"), colour(20,20,155), trackercolour(150,150,225)
rslider  bounds(250, 15, 90,90), text("Level"), textbox(1), valuetextbox(1), channel("Level"), range(0, 1, 1), fontcolour("Black"), textcolour("Black"), colour(20,20,155), trackercolour(150,150,225)
}

</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-n -d -+rtmidi=NULL -M0 -m0d 
</CsOptions>
                                           
<CsInstruments>

sr = 44100
ksmps = 16
nchnls = 2
0dbfs = 1


instr	1
 kMaxDelay			chnget		"MaxDelay"
 kMaxDelay			init		1
 kSemitones			chnget		"Semitones"
 kCents				chnget		"Cents"
 kTransRand			chnget		"TransRand"
 kDepth				chnget		"Depth"
 kRate				chnget		"Rate"
 kTranspose			=			kSemitones + kCents*0.01
 kFeedback			chnget		"Feedback"
 kWidth				chnget		"Width"
 kDryWetMix			chnget		"DryWetMix"
 kLevel				chnget		"Level"
 kFFTindex			chnget		"FFTindex"
 kFFTindex			init		4
 kGranulation		chnget		"Granulation"
 kPrePost			chnget		"PrePost"
 kPrePost			init		1
 iFFTsizes[]		fillarray	128,256,512,1024,2048,4096	; array of FFT size values
 
 ;aL		diskin	"JAG.wav",1,0,1											; read in sound file
 ;aL		poscil	0.4,300
 ;aR		=		aL
 aL,aR		ins
 
 if changed(kMaxDelay,kFFTindex)==1 then
  reinit RESTART
 endif
 RESTART:

 iFFTsize			=			iFFTsizes[i(kFFTindex)-1]	; retrieve FFT size value from array

 fsigInL			pvsanal		aL, iFFTsize, iFFTsize/4, iFFTsize, 1	; FFT analyse audio
 fsigInR			pvsanal		aR, iFFTsize, iFFTsize/4, iFFTsize, 1	; FFT analyse audio
 fsigFB				pvsinit 	iFFTsize									; initialise feedback signal
 fsigMixL 			pvsmix 		fsigInL, fsigFB								; mix feedback with input
 fsigMixR 			pvsmix 		fsigInR, fsigFB								; mix feedback with input

 iHandle1, kTime	pvsbuffer	fsigMixL, i(kMaxDelay)								; create a circular fsig buffer
 kDly1				randomh		0,i(kMaxDelay)*kDepth, kRate, 1				; delay time
 kAmp1				trandom		changed(kDly1),-kGranulation*60,0
 kTranspose1_2		trandom		changed(kDly1),kTranspose-(2*kTranspose*kTransRand),kTranspose
 fsigOut			pvsbufread 	kTime-kDly1, iHandle1						; read from buffer
 fsigGran			pvsgain		fsigOut,ampdbfs(kAmp1)
 fScale				pvscale		fsigGran,semitone(kTranspose1_2)
 fsigFB				pvsgain 	fScale, kFeedback			; create feedback signal for next pass
 if kPrePost==1 then
  aDly				pvsynth		fsigGran 									; resynthesise read buffer output
 else
  aDly				pvsynth		fScale 									; resynthesise read buffer output
 endif
 aMix				ntrpol		aL,aDly,kDryWetMix 						; dry/wet audio mix
 					outs		aMix*kLevel*(1-kWidth),aMix*kLevel

 iHandle2, kTime	pvsbuffer	fsigMixR, i(kMaxDelay)								; create a circular fsig buffer
 kDly2				randomh		0,i(kMaxDelay)*kDepth, kRate, 1				; delay time
 kAmp2				trandom		changed(kDly2),-kGranulation*60,0
 kTranspose2_2		trandom		changed(kDly2),kTranspose-(2*kTranspose*kTransRand),kTranspose
 fsigOut			pvsbufread 	kTime-kDly2, iHandle2 						; read from buffer
 fsigGran			pvsgain		fsigOut,ampdbfs(kAmp2)
 fScale				pvscale		fsigGran,semitone(kTranspose2_2)
 fsigFB				pvsgain 	fScale, kFeedback			; create feedback signal for next pass
 if kPrePost==1 then
  aDly				pvsynth		fsigGran 									; resynthesise read buffer output
 else
  aDly				pvsynth		fScale 									; resynthesise read buffer output
 endif
 aMix				ntrpol		aR,aDly,kDryWetMix 						; dry/wet audio mix
 					outs		aMix*kLevel,aMix*kLevel*(1-kWidth)
endin

</CsInstruments>

<CsScore>
i 1 0 -1
f 0 3700
</CsScore>

</CsoundSynthesizer>
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
form caption("Bounce Delay"), size(490,385), pluginid("BnDl") style("legacy")
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
image bounds(  -100,  0,  4,100), colour(250,250,250), shape(sharp), visible(1), widgetarray("Bars",32)
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
 kRatio	pow	(iCount)/(iNumber),kWarp		; define delay time ratio according to layer number and 'Warp' amount. 
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

</CsoundSynthesizer>; BreakBeatCutter.csd
; Iain McCurdy, 2013.

; Break Beat Cut Up using the bbcut opcode with additional processing

; ==BBCUT=================================================================================================================
; 'Sub-division' determines the note duration used as the base unit in  cut-ups. 
; For example a value of 8 represents quavers (eighth notes), 16 represents semiquavers (sixteenth notes) and so on.                                                   
; 
; 'Bar Length' represents the number of beats per bar. For example, a value of 4 represents a 4/4 bar and so on. 
; 
; 'Phrase' defines the number of bars that will elapse before the cutting up pattern restarts from the beginning.          
; 
; 'Stutter' is a separate cut-up process which occasionally will take a very short fragment of the input audio and repeat
; it many times. 
; 
; 'Stutter Speed' defines the duration of each stutter in relation to 'Sub-division'. 
; If subdivision is 8 (quavers / eighth notes) and 'Stutter Speed' is 2 then each stutter will be a semiquaver / sixteenth note.
; 
; 'Stutter Chance' defines the frequency of stutter moments. 
; The range for this parameter is 0 to 1. Zero means stuttering will be very unlikely, 1 means it will be very likely.       
; 'Repeats' defines the number of repeats that will be employed in normal cut-up events.                                     
; When processing non-rhythmical, unmetered material it may be be more interesting to employ non-whole numbers for parameters such as 'Sub-division', 'Phrase' and 'Stutter Speed'.                                                      
; ========================================================================================================================




; ==FILTER================================================================================================================
; Additionally in this example a randomly moving band-pass filter has been implemented. 
; 
; 'Filter Mix' crossfades between the unfiltered bbcut signal and the filtered bbcut signal.   
; 
; 'Cutoff Freq.' consists of two small sliders which determine the range from which random cutoff values are derived.       
; 
; 'Interpolate<=>S&H' fades continuously between an interpolated random function and a sample and hold type random function. 
; 
; 'Filter Div.' controls the frequency subdivision with which new random cutoff frequency values are generated - a value of '1' means that new values are generated once every bar.                                    
; ========================================================================================================================



; ==WGUIDE================================================================================================================
; A waveguide effect can randomly and rhythmically cut into the audio stream
; 'Chance' defines the probability of this happening. 0=never 1=always
; The range of frequencies the effect will choose from is defined by the user as note values.
; Frequencies are quatised to adhere to equal temperament.
; ========================================================================================================================



; ==SQUARE MOD. (Square wave ring modulation)=============================================================================
; This effect can similarly randomly and rhythmically cut into the audio stream using the 'chance' control
; The range of frequencies the modulator waveform can move between is defined as 'oct' values.
; ========================================================================================================================


; ==F.SHIFT (Frequency Shifter)===========================================================================================
; Similar to the above except using a frequency shifter effect.
; ========================================================================================================================

<Cabbage>
form          size(440,470), caption("Break Beat Cutter"), pluginid("bbct") style("legacy")


groupbox bounds( 0,  0,440,170), text("CUTTER"), plant("cutter"),colour(20,20,20), FontColour(silver){
rslider bounds( 10, 25, 70, 70), colour("Tan"), trackercolour("Tan"), fontcolour("silver"), text("Sub-div."),       channel("subdiv"),  range(1,  512,  8, 1, 1)
rslider bounds( 80, 25, 70, 70), colour("Tan"), trackercolour("Tan"), fontcolour("silver"), text("Bar Length"),     channel("barlen"),  range(1,   16,  2, 1, 1)
rslider bounds(150, 25, 70, 70), colour("Tan"), trackercolour("Tan"), fontcolour("silver"), text("Phrase"),         channel("phrase"),  range(1, 512, 8, 1, 1)
rslider bounds(220, 25, 70, 70), colour("Tan"), trackercolour("Tan"), fontcolour("silver"), text("Repeats"),        channel("repeats"), range(1, 32, 2, 1, 1)
rslider bounds(290, 25, 70, 70), colour("Tan"), trackercolour("Tan"), fontcolour("silver"), text("Stut. Speed"),  channel("stutspd"), range(1, 32, 4, 1, 1)
rslider bounds(360, 25, 70, 70), colour("Tan"), trackercolour("Tan"), fontcolour("silver"), text("Stut. Chance"), channel("stutchnc"), range(0, 1.00, 0.5)
hslider bounds(  5, 95,425, 40), colour("Tan"), trackercolour("Tan"), fontcolour("silver"), textBox(1)    channel("BPM"), range(10,  500, 110,1,1)
label   bounds(198,127, 25, 11), text("BPM"), FontColour("silver")
label  bounds( 10,142, 80,12), text("Clock Source:")
button bounds( 90,140, 60,18), text("Internal","External"), channel("ClockSource"), value(0)
label   bounds(338,158, 100, 10), text("Iain McCurdy [2013]"), FontColour("grey"), align("right")
}



groupbox bounds( 0,170,440,100), text("FILTER"), plant("filter"), FontColour(silver),colour(20,20,20){
rslider bounds( 10, 25, 70, 70), colour(200,100,50,255), trackercolour(200,100,50,255), fontcolour("silver"), text("Mix"),    channel("FltMix"), range(0, 1.00, 0.6)
rslider bounds( 80, 25, 70, 70), colour(200,100,50,255), trackercolour(200,100,50,255), fontcolour("silver"), text("Division"),    channel("fltdiv"), range(1, 16, 1,1,1)
rslider bounds(150, 25, 70, 70), colour(200,100,50,255), trackercolour(200,100,50,255), fontcolour("silver"), text("Bandwidth"),    channel("bw"), range(0.1, 10, 1, 0.5, 0.001)
hslider bounds(220, 25,140, 35), colour(200,100,50,255), trackercolour(200,100,50,255), fontcolour("silver"),    channel("cfmin"), range(50, 10000, 50  ,0.5,0.1)
hslider bounds(220, 50,140, 35), colour(200,100,50,255), trackercolour(200,100,50,255), fontcolour("silver"),    channel("cfmax"), range(50, 10000, 10000,0.5,0.1)
label   bounds(254, 77, 80, 12), text("Cutoff Freq."), FontColour("white")
rslider bounds(360, 25, 70, 70), colour(200,100,50,255), trackercolour(200,100,50,255), fontcolour("silver"), text("Int./S&H"),    channel("i_h"), range(0, 1, 0)
}

groupbox bounds( 0,270,220,100), text("WAVE GUIDE"), plant("waveguide"), FontColour(silver),colour(20,20,20){
rslider bounds( 10, 25, 70, 70), colour(150,150,50,255), trackercolour(150,150,50,255), fontcolour("silver"), text("Chance"),    channel("WguideChnc"), range(0, 1.00, 0.2)
hslider bounds( 80, 25,140, 35), colour(150,150,50,255), trackercolour(150,150,50,255), fontcolour("silver"),    channel("wguidemin"), range(22, 100, 50,1,1)
hslider bounds( 80, 50,140, 35), colour(150,150,50,255), trackercolour(150,150,50,255), fontcolour("silver"),    channel("wguidemax"), range(22, 100, 70,1,1)
label   bounds(108, 77, 85, 12), text("Pitch Range"), FontColour("white")
}

groupbox bounds(220,270,220,100), text("SQUARE MOD."), plant("sqmod"), FontColour(silver),colour(20,20,20){
rslider bounds( 10, 25, 70, 70), colour(200,150,200,255), trackercolour(200,150,200,255), fontcolour("silver"), text("Chance"),    channel("SqModChnc"), range(0, 1.00, 0.2)
hslider bounds( 80, 25,140, 35), colour(200,150,200,255), trackercolour(200,150,200,255), fontcolour("silver"),    channel("sqmodmin"), range(1, 14.0,  6)
hslider bounds( 80, 50,140, 35), colour(200,150,200,255), trackercolour(200,150,200,255), fontcolour("silver"),    channel("sqmodmax"), range(1, 14.0, 12)
label   bounds(108, 77, 85, 12), text("Freq.Range"), FontColour("white")
}

groupbox bounds(0,370,220,100), text("FREQUENCY SHIFT"), plant("fshift"), FontColour(silver),colour(20,20,20){
rslider bounds( 10, 25, 70, 70), colour(250,110,250,255), trackercolour(250,110,250,255), fontcolour("silver"), text("Chance"),    channel("FshiftChnc"), range(0, 1.00, 0.2)
hslider bounds( 80, 25,140, 35), colour(250,110,250,255), trackercolour(250,110,250,255), fontcolour("silver"),    channel("fshiftmin"), range(-4000, 4000,-1000)
hslider bounds( 80, 50,140, 35), colour(250,110,250,255), trackercolour(250,110,250,255), fontcolour("silver"),    channel("fshiftmax"), range(-4000, 4000, 1000)
label   bounds(108, 77, 85, 12), text("Freq.Range"), FontColour("white")
}

groupbox bounds(220,370,220,100), text("OUTPUT"), plant("output"), FontColour(silver),colour(20,20,20){
rslider bounds( 10, 25, 70, 70), colour("Tan"), trackercolour("Tan"), fontcolour("silver"), text("Layers"),       channel("layers"), range(1, 20, 1,1,1)
rslider bounds( 80, 25, 70, 70), colour("Tan"), trackercolour("Tan"), fontcolour("silver"), text("Dry/Wet"),      channel("DryWet"), range(0, 1.00, 0.6)
rslider bounds(150, 25, 70, 70), colour("Tan"), trackercolour("Tan"), fontcolour("silver"), text("Level"),        channel("gain"),   range(0, 1.00, 0.75)
}
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-n -dm0
</CsOptions>

<CsInstruments>
sr = 44100
ksmps = 64
nchnls = 2
0dbfs=1
seed	0

gisine	ftgen	0,0,131072,10,1

opcode	BBCutIteration,aa,aaiiiiiiiii
 aL,aR,iBPS, isubdiv,  ibarlen,  iphrase, irepeats, istutspd, istutchnc, icount, ilayers	xin
 abbcutL		bbcutm	aL,   iBPS, isubdiv,  ibarlen,  iphrase, irepeats, istutspd, istutchnc
 abbcutR		bbcutm	aR,   iBPS, isubdiv,  ibarlen,  iphrase, irepeats, istutspd, istutchnc
 amixL	=	0
 amixR	=	0
 if icount<ilayers then
   amixL,amixR	BBCutIteration	aL,aR, iBPS, isubdiv,  ibarlen,  iphrase, irepeats, istutspd, istutchnc, icount+1, ilayers
 endif
 xout	abbcutL+amixL,abbcutR+amixL
endop

opcode	FreqShifter,a,aki
	ain,kfshift,ifn	xin					;READ IN INPUT ARGUMENTS
	areal, aimag hilbert ain				;HILBERT OPCODE OUTPUTS TWO PHASE SHIFTED SIGNALS, EACH 90 OUT OF PHASE WITH EACH OTHER
	asin 	oscili       1,    kfshift,     ifn,          0
	acos 	oscili       1,    kfshift,     ifn,          0.25	
	;RING MODULATE EACH SIGNAL USING THE QUADRATURE OSCILLATORS AS MODULATORS
	amod1	=		areal * acos
	amod2	=		aimag * asin	
	;UPSHIFTING OUTPUT
	aFS	= (amod1 - amod2)
		xout	aFS				;SEND AUDIO BACK TO CALLER INSTRUMENT
endop

instr 1	; read widgets
 gksubdiv 	chnget	"subdiv"	; read in widgets
 gkbarlen 	chnget	"barlen"
 gkphrase 	chnget	"phrase"
 gkrepeats	chnget	"repeats"
 gkstutspd	chnget	"stutspd"
 gkstutchnc	chnget	"stutchnc"

 gkClockSource	chnget	"ClockSource"
 if gkClockSource==0 then
  gkBPM    	chnget	"BPM"
 else
  gkBPM    	chnget	"HOST_BPM"
  gkBPM		limit	gkBPM, 10,500	
 endif

 gkfltdiv	chnget	"fltdiv"
 gkDryWet    	chnget	"DryWet"
 gkFltMix 	chnget	"FltMix"
 gkbw     	chnget	"bw"
 gkcfmin  	chnget	"cfmin"
 gkcfmax  	chnget	"cfmax"
 gki_h    	chnget	"i_h"
 gklayers    	chnget	"layers"
 gkgain		chnget	"gain"
 konoff		chnget	"onoff"
endin

instr	2
 kmetro		metro		4
 kSwitch	init	0
 if kmetro==1 then
  kSwitch	changed		gkBPM, gkrepeats, gkphrase, gkstutspd, gkstutchnc, gkbarlen, gksubdiv, gkfltdiv, gklayers	;GENERATE A MOMENTARY '1' PULSE IN OUTPUT 'kSwitch' IF ANY OF THE SCANNED INPUT VARIABLES CHANGE. (OUTPUT 'kSwitch' IS NORMALLY ZERO)
 endif
 if	kSwitch==1	then		;IF I-RATE VARIABLE CHANGE TRIGGER IS '1'...
 	reinit	UPDATE			;BEGIN A REINITIALISATION PASS FROM LABEL 'UPDATE'
 endif
 UPDATE:

 /* INPUT */
 aL,aR ins					; live input

;;;;  use a loop sound file for testing
;#define SOUNDFILE	#"loop.wav"#		; macro
; ilen	filelen	$SOUNDFILE			; length of sound file
; ibeats =	8				; number of beats in the drum loop
; ispd	=	(ilen/ibeats) * (i(gkBPM)/60)	; speed ratio for this sample to sync with master clock
; aL	diskin	$SOUNDFILE,ispd,0,1		; read sound file
; aR	=	aL				; right channel same as left

 iBPS	=	i(gkBPM)/60
 kmetro	metro	iBPS		; metronome used for triggering random parameter changes
 
 ; call UDO
 ;OUTPUT		OPCODE	        INPUT |   BPM      | SUBDIVISION | BAR_LENGTH | PHRASE_LENGTH | NUM.OF_REPEATS | STUTTER_SPEED | STUTTER_CHANCE	
 abbcutL,abbcutR	BBCutIteration	aL,aR, i(gkBPM)/60, i(gksubdiv),  i(gkbarlen),   i(gkphrase),    i(gkrepeats),   i(gkstutspd),   i(gkstutchnc),  1, i(gklayers)
 
 ;FILTER=================================================================================================================================================================
 ifreq	=	iBPS * i(gkfltdiv)			; FREQUENCY WITH WHICH NEW FILTER CUTOFF VALUES ARE GENERATED
 
 kcf1h		randomh	gkcfmin, gkcfmax, ifreq		; sample and hold random frequency values
 kcf1i		lineto	kcf1h, 1/ifreq			; interpolate values
 kcf1		ntrpol	kcf1i, kcf1h, gki_h   		; crossfade between interpolating and sample and hold type random values
 abbFltL	resonz	abbcutL, kcf1, kcf1*gkbw, 2	; band-pass filter
 abbMixL	ntrpol	abbcutL, abbFltL, gkFltMix	; crossfade between unfiltered and filter audio signal
 kcf2h		randomh	gkcfmin, gkcfmax, ifreq		;   RIGHT CHANNEL
 kcf2i		lineto	kcf2h, 1/ifreq			; 
 kcf2		ntrpol	kcf2i, kcf2h, gki_h   		; 
 abbFltR	resonz	abbcutR, kcf2, kcf2*gkbw, 2	; 
 abbMixR	ntrpol	abbcutR, abbFltR, gkFltMix	; 
 ;=======================================================================================================================================================================

 ;WGUIDE1================================================================================================================================================================
 kchance	chnget	"WguideChnc"
 kdice		trandom	kmetro,0,1
 if kdice<kchance then
  kwguidemin	chnget	"wguidemin"
  kwguidemax	chnget	"wguidemax"
  knum		randomh	kwguidemin,kwguidemax,iBPS
  afrq		interp	cpsmidinn(int(knum))
  kfb		randomi	0.8,0.99,iBPS/4
  kcf		randomi	800,4000,iBPS
  abbMixL	wguide1	abbMixL*0.7,afrq,kcf,kfb
  abbMixR	wguide1	abbMixR*0.7,afrq,kcf,kfb
 endif  
 ;=======================================================================================================================================================================

 ;SQUARE MOD==============================================================================================================================================================
 kchance	chnget	"SqModChnc"			; read in widgets
 ksqmodmin	chnget	"sqmodmin"			;
 ksqmodmax	chnget	"sqmodmax"			; 
 kDiceRoll	trandom	kmetro,0,1			; new 'roll of the dice' upon each new time period
 if kDiceRoll<kchance then				; if 'roll of the dice' is within chance boundary... 
  kratei	randomi	ksqmodmin,ksqmodmax,iBPS	; interpolating random function for modulating waveform frequency
  krateh	randomh	ksqmodmin,ksqmodmax,iBPS	; sample and hold random function for modulating waveform frequency
  kcross	randomi	0,1,iBPS			; crossfader for morphing between interpolating and S&H functions
  krate		ntrpol	kratei,krateh,kcross		; create crossfaded rate function
  amod		lfo	1,cpsoct(krate),2		; modulating waveform (square waveform)
  kcf		limit	cpsoct(krate)*4,20,sr/3		; cutoff freq for filtering some of the high freq. content of the square wave
  amod		clfilt	amod,kcf,0,2			; low-pass filter square wave
  abbMixL	=	abbMixL*amod			; ring modulate audio
  abbMixR	=	abbMixR*amod			;
 endif
 ;=======================================================================================================================================================================

 ;FSHIFT=================================================================================================================================================================
 kchance	chnget	"FshiftChnc"			; read in widgets                         
 kdice		trandom	kmetro,0,1			; new 'roll of the dice' upon each new time period                                                                         
 if kdice<kchance then					; if 'roll of the dice' is within chance boundary...                                                                           
  kfshiftmin	chnget	"fshiftmin"                     ; read in widgets                           
  kfshiftmax	chnget	"fshiftmax"			; 
  kfsfrqi	randomi	kfshiftmin,kfshiftmax,iBPS*2	; interpolating random function for modulating waveform frequency          
  kfsfrqh	randomh	kfshiftmin,kfshiftmax,iBPS*2	; sample and hold random function for modulating waveform frequency            
  kcross	randomi	0,1,iBPS*2			; crossfader for morphing between interpolating and S&H functions          
  kfsfrq	ntrpol	kfsfrqi,kfsfrqh,kcross          ; create crossfaded rate function  modulating waveform (square waveform)
  abbMixL	FreqShifter	abbMixL,kfsfrq,gisine   ;                                                 
  abbMixR	FreqShifter	abbMixR,kfsfrq,gisine   ;                                                         
 endif                                                  ;                                                                            
 ;=======================================================================================================================================================================

 rireturn						; RETURN FROM REINITIALISATION PASS TO PERFORMANCE TIME PASSES
 
 amixL	sum	aL*(1-gkDryWet), abbMixL*gkDryWet	;SUM AND MIX DRY SIGNAL AND BBCUT SIGNAL (LEFT CHANNEL)
 amixR	sum	aR*(1-gkDryWet), abbMixR*gkDryWet	;SUM AND MIX DRY SIGNAL AND BBCUT SIGNAL (RIGHT CHANNEL)




 outs	amixL*gkgain, amixR*gkgain			;SEND AUDIO TO OUTPUTS
endin

</CsInstruments>  
<CsScore>
i1 0 36000
i2 0 36000
</CsScore>
</CsoundSynthesizer>; CrossedDelays.csd
; Written by Iain McCurdy, 2016

; This effect implements a dual delay effect in which the feedbacks between the two channels are crossed over.
; This becomes particularly useful when we add in separate ring modulators or frequency shifters on each channel.

; It is intended that the delays will be used in a coherent rhythmic function therefore delay times are set by means of a tempo control 
;  (either by the Cabbage widget or from a host) and subsequent subdivisions for each channel.
; An assumption is made that the base time signature 4/4, therefore a tempo of 60 bpm with divisions of 1/4 will result in delays of 1 second

; Feedback can be greater than a ratio of 1 without overloading occuring, this is on account of the gain reducing effect of ring modulating.
; Nonetheless a BDJ clip is activated if feedback is raised above 1 to protect against samples out of range is feedback is pushed even further.
; It should be born in mind that feeding back ring modulators can produce aliasing artefacts - in this context they are regarded as a characteristic of the effect.

; --LEFT_INPUT---+---DELAY_1----MOD_1---+---OUT_1--->
;                 \                    /
;                  \ /----[F.BACK]----/
;                   X
;                  / \----[F.BACK]----\
;                 /                    \
; --RIGHT_INPUT--+---DELAY_2----MOD_2---+---OUT_2--->

; Controls
; --------
;  [DELAYS]
; Clock Source	-	tempo source can be from the built-in 'Tempo' control or from the host's tempo, if used as a VST plugin
; Tempo			-	Cabbage defined tempo used in defining actual delay times (in conjunction with subdivision values)
; Mix			-	dry/wet mix for the effect
; Feedback		-	delay feedback. Caution: this can be set above 100%, feedback will be attenuated by ring modulation (not frequency shifting),
; 					 but if feedback is greater than '1', soft clipping will be applied to protect against samples out of range
; Subdiv.1/Subdiv.2	-	metrical subdivisionsof the two delay times

;  [MODULATION]
; Type			-	choose between ring modulation and frequency shifting
; Freq.1/Freq.2	-	the frequencies of the ring modulators or frequency shifters

;  [OUTPUT]
; Pan1/Pan2 	-	pan locations of the outputs of the two delays (OUT_1 and OUT_2 in the diagram above)
; Level			-	output level (dry and wet signals)

<Cabbage>
form caption("Crossed Delays") size(815, 130), pluginid("CrDl") style("legacy")


image   bounds(  5,  5,370,120), outlinethickness(1), outlinecolour("white"), colour("black"), plant("Delays_Plant")
{
label    bounds(  0,  4,370, 12), text("D E L A Y S")
label    bounds(  5, 20, 80, 12), text("Clock Source")
combobox bounds(  5, 35, 80, 20), channel("ClockSource"), text("Cabbage","Host")
rslider  bounds( 75, 20, 90, 90), text("Tempo"), channel("Tempo"), valuetextbox(1), textbox(1), range(20,500,60,1,0.1)
rslider  bounds(140, 20, 90, 90), text("Mix"), channel("Mix"), valuetextbox(1), textbox(1), range(0,1,0.5,1,0.01)
rslider  bounds(205, 20, 90, 90), text("Feedback"), channel("Feedback"), valuetextbox(1), textbox(1), range(0,3,0.5,1,0.01)
label    bounds(285, 20, 70, 13), text("Subdiv.1")
combobox bounds(285, 33, 70, 20), channel("Div1"), text("1/1","1/2","1/3","1/4","1/6","1/8","1/12","1/16","1/24","1/32","1/48","1/64","1/96","1/128"), value(4)
label    bounds(285, 60, 70, 13), text("Subdiv.2")
combobox bounds(285, 73, 70, 20), channel("Div2"), text("1/1","1/2","1/3","1/4","1/6","1/8","1/12","1/16","1/24","1/32","1/48","1/64","1/96","1/128"), value(3)
}

image   bounds(380,  5,225,120), outlinethickness(1), outlinecolour("white"), colour("black"), plant("RM_Plant")
{
label    bounds(  0,  4,225, 12), text("M O D U L A T I O N")
label    bounds(  5, 55, 80, 12), text("Type")
combobox bounds(  5, 70, 80, 20), channel("ModType"), text("Ring Mod.","Freq.Shift"), value(2)
checkbox bounds(  5, 30,120, 12), text("On/Off"), channel("ModOnOff"), value(1)
rslider  bounds( 75, 20, 90, 90), text("Freq.1"), channel("Freq1"), valuetextbox(1), textbox(1), range(-6000,6000,200,1,1)
rslider  bounds(140, 20, 90, 90), text("Freq.2"), channel("Freq2"), valuetextbox(1), textbox(1), range(-6000,6000,333,1,1)
}

image   bounds(610,  5,200,120), outlinethickness(1), outlinecolour("white"), colour("black"), plant("Output_Plant")
{
label    bounds(  0,  4,200, 12), text("O U T P U T")
rslider  bounds(-10, 20, 90, 90), text("Pan 1"), channel("Pan1"), valuetextbox(1), textbox(1), range(0,1,0)
rslider  bounds( 55, 20, 90, 90), text("Pan 2"), channel("Pan2"), valuetextbox(1), textbox(1), range(0,1,1)
rslider  bounds(120, 20, 90, 90), text("Level"), channel("Level"), valuetextbox(1), textbox(1), range(0,1,1)
}

</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-n -d -+rtmidi=NULL -M0 -m0d 
</CsOptions>

<CsInstruments>

sr = 44100
ksmps = 16
nchnls = 2
0dbfs = 1

gkDivs[]	fillarray	1,2,3,4,6,8,12,16,24,32,48,64,96,128

opcode	FreqShifterSimple,a,aki					; SIMPLE FREQUENCY SHIFTER
	asig,kfshift,ifn	xin				; READ IN INPUT ARGUMENTS
	areal, aimag hilbert asig				; HILBERT OPCODE OUTPUTS TWO PHASE SHIFTED SIGNALS, EACH 90 OUT OF PHASE WITH EACH OTHER
	asin 	oscili       1,    kfshift,     ifn,          0
	acos 	oscili       1,    kfshift,     ifn,          0.25	
	;RING MODULATE EACH SIGNAL USING THE QUADRATURE OSCILLATORS AS MODULATORS
	amod1	=		areal * acos
	amod2	=		aimag * asin	
	;UPSHIFTING OUTPUT
	ares	= (amod1 - amod2)				; MIX OUTPUTS (INVERTED WITH RESPECT TO EACH OTHER)
		xout	ares					; SEND AUDIO BACK TO CALLER INSTRUMENT
endop

gisine	ftgen	0,0,131072,10,1

instr	1
; aSig		diskin2	"808loopMono.wav",1,0,1
; ilen		filelen	"808loopMono.wav"
; klen		init	ilen

 kbpm		chnget	"HOST_BPM"
 kClockSource	chnget	"ClockSource"
 kClockSource	init	1
 if kClockSource==1 then				;if internal clock source has been chosen...
  ktempo	chnget	"Tempo"				;tempo taken from GUI knob control
 else
  ktempo	chnget	"bpm"				;tempo taken from host BPM
  ktempo	limit	ktempo,20,500		;limit range of possible tempo values. i.e. a tempo of zero would result in a delay time of infinity.
 endif

 aL,aR		ins
 klen		=		60*2/ktempo

 kMix		chnget	"Mix" 
 kFeedback	chnget	"Feedback"
 kModOnOff	chnget	"ModOnOff"
 kFreq1		chnget	"Freq1"
 kFreq2		chnget	"Freq2" 
 kDiv1		chnget	"Div1"
 kDiv2		chnget	"Div2"
 kModType	chnget	"ModType"
 
 aTap1,aTap2	init	0
 
 aBuf		delayr	60*2/20
 aTap1		deltapi	klen/gkDivs[kDiv1]
 aMod1		poscil	1,kFreq1
 if kModOnOff==1 then
  if kModType==1 then
   aTap1		*=		aMod1
  else
   aTap1		FreqShifterSimple	aTap1,kFreq1,gisine
  endif
 endif
 			delayw	aL + (aTap2 * kFeedback)
 
 aBuf		delayr	60*2/20
 aTap2		deltapi	klen/gkDivs[kDiv2]
 aMod2		poscil	1,kFreq2
 if kModOnOff==1 then
  if kModType==1 then
   aTap2		*=		aMod2
  else
   aTap2		FreqShifterSimple	aTap2,kFreq2,gisine
  endif                                               
 endif
 			delayw	aR + (aTap1 * kFeedback)
 
 if kFeedback>1 then
  aTap1 clip 	aTap1, 0, 0dbfs, 0dbfs
  aTap2 clip 	aTap2, 0, 0dbfs, 0dbfs
 endif

 kPan1		chnget	"Pan1"
 kPan2		chnget	"Pan2"
 kLevel		chnget	"Level"
 			
 aMixL		ntrpol	aL, aTap1*(1-kPan1) + aTap2*(1-kPan2), kMix
 aMixR		ntrpol	aR, aTap1*kPan1 + aTap2*kPan2, kMix
			outs	aMixL*kLevel, aMixR*kLevel
endin

</CsInstruments>

<CsScore>
i 1 0 [60*60*24*7] 
</CsScore>

</CsoundSynthesizer>
; DelayGrain.csd
; Iain McCurdy, 2013
; 
; CONTROLS
; --------
; Grain Size		--	range of possible grain sizes (in seconds)
; Density			--	grain density in grains per second (note that the addition of delay will disrupt the regularity of grains)
; Delay			--	range of delay times possible (in seconds)
; Transpose		--	range of transpositions (in semitones)
; Spread			--	random panning spread of grains
; Amplitude Decay		--	the larger this value, the more grains are delayed, the more their amplitudes will be lowered
; Reversal Proability	--	probability of material within the grains being played backwards: 0 = all forwards
; 			                                                                          1 = all backwards
; 			                                                                          0.5 = 50:50
; 			        reversal might be hard to hear unless grain size is large
; Mix			--	dry/wet mix
; Level			--	output level (both dry and wet)
; Grain Env.		--	sets the amplitude enveloping window for each grain
; 				Hanning: natural sounding soft attack, soft decay envelope
; 				Half-sine: like the hanning but with a slightly sharper attack and decay
; 				Decay 1: a percussive decay envelope with linear segments
; 				Decay 2: a percussive decay envelope with a exponential decay segment. Probably more natural sounding than 'Decay 1' but longer grain sizes may be necessary
; 				Gate: sharp attack and decay. Rather synthetic sounding.
; Delay Distr.		--	random delay time distribution: exponential, linear or logarithmic. Effect are quite subtle but exponential might be most natural sounding.

<Cabbage>
form caption("Delay Grain") size(410, 520), pluginid("DGrn") style("legacy")

;hslider2 bounds(  5,  5, 400, 30), channel("GSize1","GSize2"), min(0.01), max(0.09)    range(0.005, 2, 0.01, 0.5, 0.0001)
hrange   bounds(  5,  5, 400, 30), channel("GSize1","GSize2"), range(0.005, 2, 0.01:0.09, 0.5, 0.0001)
label    bounds(161, 29,  90, 11), text("G R A I N   S I Z E"), fontcolour(105,105,255)

hslider  bounds(  5, 50, 400, 25), channel("Dens"), range(0.2, 2000, 50,0.5,0.001)
label    bounds(172, 69,  65, 11), text("D E N S I T Y"), fontcolour(105,105,255)

;hslider2 bounds(  5, 90, 400, 30), channel("Dly1","Dly2"), min(0.01), max(0.5)    range(0, 5, 0.01, 0.5, 0.0001)
hrange   bounds(  5,  90, 400, 30), channel("Dly1","Dly2"), range(0, 5, 0.01:0.5, 0.5, 0.0001)
label    bounds(180,114,  50, 11), text("D E L A Y"), fontcolour(105,105,255)

;hslider2 bounds(  5,140, 400, 30), channel("Trns1","Trns2"), min(0), max(0)    range(-12, 12, 0, 1, 0.001)
hrange   bounds(  5, 140, 400, 30), channel("Trns1","Trns2"), range(-12, 12, 0:0, 1, 0.001)
label    bounds(162,164,  88, 11), text("T R A N S P O S E"), fontcolour(105,105,255)

hslider  bounds(  5,195, 400, 25), channel("PanSpread"), range(0, 1.00, 0.5,1,0.001)
label    bounds(158,214,  95, 11), text("P A N   S P R E A D"), fontcolour(105,105,255)

hslider  bounds(  5,240, 400, 25), channel("AmpSpread"), range(0, 1.00, 0.5,1,0.001)
label    bounds(158,259,  95, 11), text("A M P   S P R E A D"), fontcolour(105,105,255)

hslider  bounds(  5,280, 400, 25), channel("FiltSpread"), range(0, 1.00, 0.5,1,0.001)
label    bounds(147,299, 120, 11), text("F I L T E R   S P R E A D"), fontcolour(105,105,255)

hslider  bounds(  5,320, 400, 25), channel("ampdecay"), range(0, 1.00, 0.5,1,0.001)
label    bounds(131,339, 150, 11), text("A M P L I T U D E   D E C A Y"), fontcolour(105,105,255)

hslider  bounds(  5,360, 400, 25), channel("reverse"), range(0, 1.00, 0,1,0.001)
label    bounds(112,379, 190, 11), text("R E V E R S A L   P R O B A B I L I T Y"), fontcolour(105,105,255)

hslider  bounds(  5,400, 400, 25), channel("mix"), range(0, 1.00, 1,1,0.001)
label    bounds(189,419,  30, 11), text("M I X"), fontcolour(105,105,255)

hslider  bounds(  5,440, 400, 25), channel("level"), range(0, 2.00, 1, 0.5, 0.001)
label    bounds(180,459,  50, 11), text("L E V E L"), fontcolour(105,105,255)

label    bounds( 19,475,  70, 11), text("GRAIN ENV."), fontcolour(105,105,255)
combobox bounds( 10,486,  90, 17), channel("window"), value(1), text("Hanning","Half Sine","Decay 1","Decay 2","Gate")

label    bounds(117,475,  75, 11), text("DELAY DISTR."), fontcolour(105,105,255)
combobox bounds(110,486,  90, 17), channel("DlyDst"), value(1), text("Exp.","Lin.","Log")

label    bounds(  5,508, 160, 11), text("Author: Iain McCurdy |2013|"), FontColour("grey")
</Cabbage>

<CsoundSynthesizer>
<CsOptions>
-dm0 -n
</CsOptions>
<CsInstruments>
sr = 44100
ksmps = 32
nchnls = 2
0dbfs = 1

;Author: Iain McCurdy (2013)

; window functions
giwfn1	ftgen	0,  0, 131072,  9,   .5, 1, 	0 				; HALF SINE
giwfn2	ftgen	0,  0, 131072,  7,    0, 3072,  1, 128000,     0		; PERCUSSIVE - STRAIGHT SEGMENTS
giwfn3	ftgen	0,  0, 131072, 16,    0, 3072,0,1, 128000,-2,  0		; PERCUSSIVE - EXPONENTIAL SEGMENTS
giwfn4	ftgen	0,  0, 131072,  7,     0, 3536,  1, 124000,     1, 3536, 0	; GATE - WITH ANTI-CLICK RAMP UP AND RAMP DOWN SEGMENTS
giwfn5	ftgen	0,  0, 131072,  7,    0, 128000,1, 3072,       0		; REVERSE PERCUSSIVE - STRAIGHT SEGMENTS
giwfn6	ftgen	0,  0, 131072,  5, .001, 128000,1, 3072,   0.001		; REVERSE PERCUSSIVE - EXPONENTIAL SEGMENTS
giwfn7	ftgen	0,  0, 131072,  20,   2, 1					; HANNING WINDOW
giwindows	ftgen	0,0,8,-2,giwfn7,giwfn1,giwfn2,giwfn3,giwfn4

giBufL	ftgen	0,0,1048576,-2,0	; function table used for storing audio
giBufR	ftgen	0,0,1048576,-2,0	; function table used for storing audio

gigaussian	ftgen	0,0,4096,20,6,1,1	; a gaussian distribution

gaGMixL,gaGMixR	init	0	; initialise stereo grain signal

	instr	1			; grain triggering instrument
kGSize1	chnget	"GSize1"		; grain size limit 1
kGSize2	chnget	"GSize2"		; grain size limit 2
kDens	chnget	"Dens"			; grain density
kDly1	chnget	"Dly1"			; delay time limit 1
kDly2	chnget	"Dly2"                  ; delay time limit 2
kTrns1	chnget	"Trns1"			; transposition in semitones
kTrns2	chnget	"Trns2"
kPanSpread	chnget	"PanSpread"	; random panning spread
kAmpSpread	chnget	"AmpSpread"	; random amplitude spread
kFiltSpread	chnget	"FiltSpread"	; random filter spread
kreverse	chnget	"reverse"	; reversal probability
kampdecay	chnget	"ampdecay"	; amount of delay->amplitude attenuation
kwindow	chnget	"window"		; window
kDlyDst	chnget	"DlyDst"		; delay time distribution
kmix	chnget	"mix"			; dry/wet mix
klevel	chnget	"level"			; output level (both dry and wet signals)

aL, aR	ins				; read audio input
	outs	aL*klevel*(1-kmix),aR*klevel*(1-kmix)

/* WRITE TO BUFFER TABLES */
ilen	=	ftlen(giBufL)		; table length (in samples)
aptr	phasor	sr/ilen			; phase pointer used to write to table
aptr	=	aptr*ilen		; rescale pointer according to table size
	tablew	aL, aptr, giBufL	; write audio to table
	tablew	aR, aptr, giBufR	; write audio to table
kptr	downsamp	aptr		; downsamp pointer to k-rate

ktrig		metro	kDens		; grain trigger

/* GRAIN SIZE */
kGSize		random	0,1		; random value 0 - 1
;kGSize	expcurve	kGSize,50	; exponentially redistribute range 0 - 1
kMinGSize	min	kGSize1,kGSize2	; find minimum grain size limit
kMaxGSize	max	kGSize1,kGSize2	; find maximum grain size limit
kGSize		scale	kGSize,kMaxGSize,kMinGSize	; rescale random value according to minimum and maximum limits

/* DELAY TIME */
kDly	random	0,1			; uniform random value 0 - 1
if kDlyDst=1 then			; if delay time distribution is exponential
 kDly	expcurve	kDly,100	; exponential distrubution range 0 - 1
elseif kDlyDst=3 then			; .. or if logarithmic
 kDly	logcurve	kDly,100	; exponential distrubution range 0 - 1
endif					; (other linear so do not alter)
if kDly1=kDly2 then
 kMinDly	=	kDly1		; delays can't be the same value!!	 
 kMaxDly	=	kDly2+0.001
else	
 kMinDly	min	kDly1,kDly2		; find minimum delay time limit
 kMaxDly	max	kDly1,kDly2		; find maximum delay time limit
endif

ioffset	=	1/sr			; delay offset (can't read at same location as write pointer!)
kDly	scale		kDly,kMaxDly,kMinDly; distribution rescaled to match the user defined limits

/* CALL GRAIN */
;		          p1 p2         p3     p4   p5         p6       p7   p8              p9             p10       p11         p12     p13        p14         p15    p16
schedkwhen	ktrig,0,0,2,kDly+0.0001,kGSize,kptr,kPanSpread,kreverse,kDly,kMinDly+ioffset,kMaxDly+0.0001,kampdecay,klevel*kmix,kwindow,kAmpSpread,kFiltSpread,kTrns1,kTrns2	; call grain instrument
	endin

instr	2				; grain instrument
iGStart	=	p4			; grain start position (in samples)
ispread	=	p5			; random panning spread
ireverse=	(rnd(1)>p6?1:-1)	; decide fwd/bwd status
iwindow	table	p12-1,giwindows		; amplitude envelope shape for this grain

/* AMPLITUDE CONTROL */
idly	=	p7					; delay time
iMinDly	=	p8					; minimum delay
iMaxDly	=	p9					; maximum delay
iampdecay=	p10					; amount of delaytime->amplitude attenuation
ilevel	=	p11					; grain output level
iAmpSpread	=	p13
iFiltSpread	=	p14

iRto	divz	idly-iMinDly , iMaxDly-iMinDly, 0	; create delay:amplitude ration (safely)
iamp	=	(1 - iRto) ^ 2				; invert range
iamp	ntrpol	1,iamp,iampdecay			; mix flat amplitude to scaled amplitude according to user setting
iRndAmp	random	1-iAmpSpread, 1				; random amplitude value for this grain
iamp	=	iamp*iRndAmp				; apply random amplitude

/* TRANSPOSITION */
iTrns	random	p15,p16
iRto	=	semitone(iTrns)
if iRto>1 then
 iStrtOS	=	(iRto-1)  * sr * p3
else
 iStrtOS	=	0
endif

aline	line	iGStart-iStrtOS,p3,iGStart-iStrtOS+(p3*iRto*sr*ireverse)	; grain pointer
aenv	oscili	iamp,1/p3,iwindow			; amplitude envelope
aL	tablei	aline,giBufL,0,0,1			; read audio from table 
aR	tablei	aline,giBufR,0,0,1			; read audio from table

if iFiltSpread>0 then
 iRndCfOct	random	14-(iFiltSpread*10),14
 iRndCf		=	cpsoct(iRndCfOct)
 aL		butlp	aL,iRndCf
 aR		butlp	aR,iRndCf
endif


ipan	random	0.5-(ispread*0.5),0.5+(ispread*0.5)	; random pan position for this grain
gaGMixL	=	gaGMixL + (aL*aenv*ipan*ilevel)		; left channel mix added to global variable
gaGMixR	=	gaGMixR + (aR*aenv*(1-ipan)*ilevel)	; right channel mix added to global variable
endin

instr	3						; output instrument (always on)
	outs	gaGMixL,gaGMixR				; send global audio signals to output
	clear	gaGMixL,gaGMixR				; clear global audio variables
endin

</CsInstruments>

<CsScore>
i 1 0 [60*60*24*7]	; read audio, write to buffers, call grains.
i 3 0 [60*60*24*7]	; output
</CsScore>

</CsoundSynthesizer>; LiveSndwarp.csd
; Iain McCurdy (2012)
; 
; Description and Instructions
; ----------------------------
; 
; This instrument implements live granulation of an audio input stream using the sndwarp opcode.                         
; 
; Live audio from the first input channel (left input if stereo) is written into a function table from which sndwarp reads 
; audio. (If 'stereo in/out' is chosen from 'In/Out Mode' then audio from the second/right channel is written into a second 
; table.) The key is that manual pointer read mode is used with sndwarp (as opposed to time-stretch mode) and that the read 
; pointer follows on behind the pointer which is used to write audio into the function table(s). Some care is needed to ensure 
; that the read pointer does not 'overtake' the write pointer (which would result in dicontinuities in the audio it reads). 
; This could be possible if pitch transposition upwards of grains is used as the grain pointer is then moving faster than the 
; write pointer. This example prevents this from happening internally so the user does not need to worry. The user can also 
; define a random offset for the grain read pointer using the 'Grain Delay' settings. Delay times are randomly chosen 
; according to a 'betarand' distribution the 'beta' of which the user can set: if distribution shape is 1 the distribution is 
; uniform, if it is 2 the distribution is linear and beyond 2 it is increasingly exponential.
; 
; Note that 'Size' (grain size) and 'Size Rnd.' (random grain size) are i-rate variables so that changing them requires 
; reinitialisation in the orchestra. For this reason discontinuity in the audio output can be heard when they are modified.
; Grain Size and Size Random (bandwith) are in sample frames. Divide by sample rate to derive a value in seconds.
; 
; Pitch transposition can be set using either the 'Pitch' knob (ratio multiplier) or 'Semis' (transposition in semitones).
; Changes made to 'Semis' will be reflected in the setting of the 'Pitch' knob, but not vice versa.
; Pitch can also be controlled through MIDI input (in which case 'Pitch' and 'Semis' will be ignored). Using MIDI will 
; polyphony will be possible. If you intend to use MIDI to start and stop sndwarp instances, turn 'On/Off [MIDI]' off.
; You can also adjust the MIDI note at which unison (no transposition) will occur using the 'Uni.Note' knob.
; 
; Sound output from sndwarp can be fed back into the input to be mixed with the live audio in. The amount of feedback can be 
; controlled using the 'Feedback' slider. Using high levels of feedback can result in overloading but this will also be 
; dependent upon other factors such as random delay time, grain size (window size), density and transposition so user caution 
; is advised. If the 'clip' button is activated the feedback signal will be clipped at the clip level set (a ratio of then 
; maximum amplitude) providing at least some control over a runaway feedback loop. Note that 'Clip Lev.' defines the amplitude 
; at which clipping begins, therefore lower settings will result in the signal being clipped sooner. The feedback signal can 
; also be filtered by a lowpass filter.
; 
; If 'balance' is activated the output of sndwarp is dynamically balanced with the input signal. This can be useful for 
; compensating for increases in amplitude caused when 'Number of Overlaps' (grain density) is increased. There are 3 
; mono/stereo modes: 'mono in - mono out (x2)', 'mono in - stereo out' (stereo effect is created using sndwarp's built-in 
; window/grain size randomisation, and 'stereo in - stereo out' mode.
; 
; The buffer size used in the example is just under 23 seconds long (function table size 1048576 at sr=44100). This could be 
; enlarged if required but bear in mind that sndwarp needs a power of two table size.        
; 
; Activating 'Freeze' will pause writing of live audio to the function table and allow the user to manually navigate through 
; the buffered audio. The feedback loop will also be deactivated when 'freeze' is active.

<Cabbage>
form caption("Live Sndwarp") size(530, 495), pluginid("lwrp") style("legacy")

groupbox bounds(  0,  0, 300,100), text("Master"), colour(30, 30, 40), fontcolour(255,125,125), plant("Master")
{
checkbox bounds( 10, 28, 110, 20), colour("yellow"), channel("OnOff"),  value(1), text("On/Off [MIDI]"), trackercolour("red")
label    bounds( 22, 53,  80, 13), text("In/Out Mode")
combobox bounds( 10, 69, 100, 20), channel("monostereo"), value(2), text("mono","stereo out","stereo in/out")
rslider  bounds(115, 30,  60, 60), text("In Gain"),  channel("InGain"), range(0, 2.00, 1, 0.5), colour( 67, 99, 99), trackercolour(207,239,239)
rslider  bounds(175, 30,  60, 60), text("Out Gain"), channel("amp"),    range(0, 2.00, 1, 0.5), colour( 67, 99, 99), trackercolour(207,239,239)
rslider  bounds(235, 30,  60, 60), text("Mix"),      channel("mix"),    range(0, 1.00, 1),      colour( 67, 99, 99), trackercolour(207,239,239)
}

groupbox bounds(300,  0, 230,100), text("Feedback - CAUTION!"), colour(25, 25, 35), fontcolour(250,120,120), plant("Feedback")
{
rslider  bounds(  5, 30,  60, 60), text("Amount"), channel("feedback"), range(0, 1.00, 0), colour( 67, 99, 99), trackercolour(207,239,239)
checkbox bounds( 65, 35,  70, 20), colour("yellow"), channel("clip"),  value(1), text("Clip"), trackercolour("red")
checkbox bounds( 65, 65,  70, 20), colour("yellow"), channel("LPF_On"),  value(0), text("LPF"), trackercolour("red")
rslider  bounds(110, 30,  60, 60), text("Clip Lev."), channel("ClipLev"), range(0.01, 1, 0.5, 0.5), colour( 67, 99, 99), trackercolour(207,239,239)
rslider  bounds(165, 30,  60, 60), text("LPF"), channel("Cutoff"), range(20, 20000,  4000, 0.5), colour( 67, 99, 99), trackercolour(207,239,239)
}

groupbox bounds(  0,100, 530,100), text("Grains"), colour(35, 35, 45), fontcolour(255,130,130), plant("Grains")
{
rslider  bounds(  5, 30,  60, 60), text("Size"), channel("wsize"), range(1, 88200, 7000, 0.25, 1), colour( 67, 99, 99), trackercolour(207,239,239)
rslider  bounds( 65, 30,  60, 60), text("Size Rnd."), channel("rnd"), range(0, 30000, 1000, 0.375, 1), colour( 67, 99, 99), trackercolour(207,239,239)
rslider  bounds(125, 30,  60, 60), text("Pitch"), channel("pch"), range(0.01, 8, 1, 0.5), colour( 67, 99, 99), trackercolour(207,239,239)
rslider  bounds(185, 30,  60, 60), text("Semis"), channel("semis"), range(-48, 48, 0,1,1), colour( 67, 99, 99), trackercolour(207,239,239)
rslider  bounds(245, 30,  60, 60), text("Density"), channel("olaps"), range(1, 100, 10, 1, 1), colour( 67, 99, 99), trackercolour(207,239,239)
label    bounds(305, 25, 100, 13), text("Grain Envelope")
combobox bounds(305, 41, 100, 20), channel("wfn"), value(1), text("Half Sine","Perc. 1","Perc. 2","Gate","Rev. Perc. 1 ","Rev. Perc. 2")
checkbox bounds(315, 70, 100, 20), colour("yellow"), channel("balance"),  value(0), text("Balance")
rslider  bounds(405, 30,  60, 60), text("Delay"), channel("dly"), range(0, 5, 0.01, 0.5), colour( 67, 99, 99), trackercolour(207,239,239)
rslider  bounds(465, 30,  60, 60), text("Distr."), channel("beta"), range(1, 16.0, 1, 0.5), colour( 67, 99, 99), trackercolour(207,239,239)
}

groupbox bounds(  0,200,530,165), text("Freeze"), colour(45, 45, 55), fontcolour(255,100,100), plant("Freeze")
{
button   bounds( 10, 25, 80, 20), fontcolour:0(50,50,100), fontcolour:1(205,205,255), colour:0(0,0,10), colour:1(100,100,150), channel("freeze"), text("FREEZE","FREEZE"),  value(0)
hslider  bounds( 90, 29,430, 10), text("Port.Time"), channel("ManPtrPort"), range(0, 1.00, 0.5), colour( 67, 99, 99), trackercolour(207,239,239)
gentable bounds( 10, 50,510, 80), tablenumber(1), tablecolour("lime"), amprange(-1,1,1), identchannel(table), zoom(-1), fill(0)
image    bounds(518, 50,  2, 80), colour(200,200,200,100), identchannel("wiper")
hslider  bounds(  5,132,522, 11), channel("ManPtr"), range(-1.00, 0, 0, 1, 0.001), colour( 67, 99, 99), trackercolour(207,239,239)
label    bounds(220,145,100, 13), text("Manual Pointer")
}

groupbox bounds(  0,365,530,100), text("MIDI"), colour(30, 30, 40), fontcolour(255,100,100), plant("MIDI")
{
rslider  bounds( 10, 35, 60, 60), text("Uni.Note"), channel("UniNote"), range(0, 127, 72,1,1), colour( 67, 99, 99), trackercolour(207,239,239)
keyboard bounds( 80,  0,450,100)
}
image bounds( 5, 470, 215, 20), colour(75, 85, 90, 100), plant("credit"), outlinethickness(0){
label bounds( 3,   3, 210, 14), text("Author: Iain McCurdy |2012|"), fontcolour("white")
}
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-dm0 -n -+rtmidi=null -M0
</CsOptions>

<CsInstruments>

sr 		= 	44100	;SAMPLE RATE
ksmps 		= 	32	;NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls 		= 	2	;NUMBER OF CHANNELS (2=STEREO)
0dbfs		=	1
massign	0,2

giview		ftgen	1, 0, 512, 2, 0		; display buffer

gibuffer	ftgen	0, 0, 1048576, 2, 0	; Buffer table. Roughly 23 seconds duration.
gibufferR	ftgen	0, 0, 1048576, 2, 0	; right channel

;GRAIN ENVELOPE WINDOW FUNCTION TABLES:
giwfn1	ftgen	0,  0, 131072,  9,   .5, 1, 	0 							; HALF SINE
giwfn2	ftgen	0,  0, 131072,  7,    0, 3072,  1, 128000,     0			; PERCUSSIVE - STRAIGHT SEGMENTS
giwfn3	ftgen	0,  0, 131072,  5, .001, 3072,  1, 128000, 0.001			; PERCUSSIVE - EXPONENTIAL SEGMENTS
giwfn4	ftgen	0,  0, 131072,  7,    0, 1536,  1, 128000,     1, 1536, 0	; GATE - WITH ANTI-CLICK RAMP UP AND RAMP DOWN SEGMENTS
giwfn5	ftgen	0,  0, 131072,  7,    0, 128000,1, 3072,       0			; REVERSE PERCUSSIVE - STRAIGHT SEGMENTS
giwfn6	ftgen	0,  0, 131072,  5, .001, 128000,1, 3072,   0.001			; REVERSE PERCUSSIVE - EXPONENTIAL SEGMENTS

instr	1
	gkOnOff		chnget	"OnOff"
	ktrigger	trigger	gkOnOff,0.5,0
	schedkwhen	ktrigger,0,0,2,0,-1

	ginsamp		=	ftlen(gibuffer)-1			;index of the final sample in the function table

	gkamp		chnget	"amp"
	gkInGain	chnget	"InGain"
	gkmix		chnget	"mix"
	gkbalance	chnget	"balance"
	gkmonostereo	chnget	"monostereo"
	gkfback		chnget	"feedback"
	gkclip  	chnget	"clip"
	gkClipLev  	chnget	"ClipLev"
	gkCutoff	chnget	"Cutoff"
	gkpch		chnget	"pch"
	gkLPF_On  	chnget	"LPF_On"
	gkwsize		chnget	"wsize"
	gkwsize		=	int(gkwsize)	; workaround
	gkrnd		chnget	"rnd"
	gkornd		=	int(gkrnd)	; workaround
	gkolap		chnget	"olaps"
	gkolap		=	int(gkolap)	; workaround
	gkwfn		chnget	"wfn"
	gkdly		chnget	"dly"
	gkbeta		chnget	"beta"
	gkfreeze	chnget	"freeze"
	gkManPtrPort	chnget	"ManPtrPort"
	gkManPtr	chnget	"ManPtr"
	gkUniNote	chnget	"UniNote"

	ktrigger	trigger	gkfreeze,0.5,1				; if 'freeze' switch is turned off...
	if ktrigger==1 then						
	 chnset	1-ktrigger,"ManPtr"					; reset Manual Pointer slider to its default (maximum) position
	endif

	ain	inch	1						; read audio input from the left input channel
	ain	=	ain*gkInGain					; scale input signal according to 'In Gain' control position
		outch	1,ain*(1-gkmix)					; send some dry signal to output according to dry/wet 'Mix' control position
	gaFBackSig,gaFBackSigR	init	0				; audio feedback signal (initialised for first performance iteration)
	
	/*DC offset filter feedback signal*/
	aFBackSig	dcblock2	gaFBackSig			; filter dc offset from left channel feedback signal
	if gkmonostereo==3 then						; if 'stereo in' mode is active...
	 aFBackSigR	dcblock2	gaFBackSigR			; filter dc offset from right channel feedback signal
	endif
	
	/*lowpass filter feedback signal*/
	if gkLPF_On==1 then						; if lowpass filter button is on...
	 aFBackSig tone	aFBackSig,gkCutoff				; ...filter left feedback channel
	 if gkmonostereo==3 then					; if 'stereo in' mode is active...
	  aFBackSigR tone	aFBackSigR,gkCutoff			; lowpass filter the right channel
	 endif
	endif

	/*clip feedback signal*/
	if gkclip==1 then						; if clip switch is on...
	 ktrig	changed	gkClipLev					; if clip level control is adjusted generate a trigger impulse (momentary '1')
	 if ktrig==1 then						; if a trigger impulse has been received...
	  reinit	UPDATE_CLIP_L					; reinitialise clip opcode (clip level is i-rate only)
	 endif
	 UPDATE_CLIP_L:
	 aFBackSig	clip	aFBackSig, 0, 0dbfs*i(gkClipLev)	; clip left feedback signal at maximum amplitude using bram de jong method
	 if gkmonostereo==3 then					; and if stereo in/out mode is also chosen
	  aFBackSigR	clip	aFBackSigR, 0, 0dbfs*i(gkClipLev)	; clip right channel feedback signal
	  rireturn
	 endif
	endif
	gaphsW	phasor	(sr*(1-gkfreeze))/ginsamp			; pointer 0 - 1	;create a moving phase value that will be used to point to locations in a function table where input audio signal will be written

	/*write audio from left input to function table*/
	if gkfreeze==0 then
		tablew	ain+aFBackSig,gaphsW,gibuffer,1			; write input audio to table
		tablew	k(ain)*5,1-k(gaphsW),giview,1			; write a value to the view table (k-rate is sufficient). Boost its amplitude before writing.
	 if metro(32)==1 then								; peg rate of table updates
       	  chnset	"tablenumber(1)", "table"			; update table display	
	 endif

	endif
	
	/*if stereo in - stereo out mode*/
	if gkmonostereo==3 then						; if stereo in/out mode has been chosen...
	 aR	inch	2						; read right channel audio input
	 aR	=	aR*gkInGain					; rescale its amplitude with 'Input Gain' slider
	 if gkfreeze==0 then
		tablew	aR+aFBackSigR,gaphsW,gibufferR,1		; write right channel audio input audio to table
	 endif
		outch	2,aR*(1-gkmix)					; if 'stereo in' mode is selected, send some right channel dry signal to output according to dry/wet 'Mix' control position
	else
		outch	2,ain*(1-gkmix)					; otherwise not 'stereo in' mode so just send some left channel dry signal to output
	endif
		clear	gaFBackSig,gaFBackSigR				; clear feedback signals
		
	if changed(gkManPtr)==1 then
	 Smsg	sprintfk	"pos(%d,250)",10+((gkManPtr+1)*508)
	 	chnset		Smsg,"wiper"
	endif
endin

instr	2
	iMIDIActiveValue	=	1							; IF MIDI ACTIVATED
	iMIDIflag		=	0							; IF FLTK ACTIVATED
	mididefault	iMIDIActiveValue, iMIDIflag						; IF NOTE IS MIDI ACTIVATED REPLACE iMIDIflag WITH iMIDIActiveValue 
	kMIDIflag	init	iMIDIflag
	if gkOnOff==0&&iMIDIflag==0 then
	 turnoff
	endif

	if iMIDIflag==1 then
	 icps	cpsmidi
	 kpch	=	icps/cpsmidinn(gkUniNote)
	else
	 kpch		=	gkpch
	endif	

	kporttime	linseg	0,0.001,0.03							; portamento time. Rises quickly from zero to a held value.
	kpch		portk	kpch,kporttime							; Apply portamento smoothing to changes made to the pitch multiplier
	apch	interp	kpch									; interpolate pitch multiplier variable to create an a-rate version. This will produce higher quality results when pitch is modulated.
	
	kManPtr	portk	gkManPtr,kporttime*10*gkfreeze*gkManPtrPort

	kmetro	metro	5									; peg rate of reinits
	if kmetro==1 then
	 ktrig	changed	gkwsize,gkrnd,gkolap,gkwfn						; if any of the list of input args. change, generate a trigger impulse (momentary '1'). The input args are all i-rate in sndwarp so reinitialisation will be required for their changes to register.
	endif
	if ktrig==1 then									; if a trigger has been generated... 
	 reinit	UPDATE_SNDWARP									; ... begin a reinitialisation pass from the given label
	endif
	UPDATE_SNDWARP:										; a label. Reinitialisation begins from here.
	imode	=	1									; sndwarp mode. (1=pointer mode, timestretch mode not so useful in a live audio in application)
	ibeg	=	0									; point in the function table from which to begin reading audio (0=beginning)
	iwsize	=	i(gkwsize)								; window (grain) size in samples
	irnd	=	i(gkrnd)								; window (grain) size randomisation bandwidth in samples
	iolap	=	i(gkolap)								; number of grain overlaps
	kRndDly	betarand	gkdly,1,gkbeta							; random grain delay time
	if gkmonostereo!=0 then
	 kRndDlyR	betarand	gkdly,1,gkbeta						; random grain delay time
	endif
	
	iMaxDur	=	(iwsize+irnd)/sr							; maximum grain duration in seconds
	kTransComp	limit	iMaxDur*(kpch-1),0,ginsamp/sr
	kdelay	=	(kTransComp+kRndDly) / (ginsamp/sr)					; delay time required when reading grains from the function table
	if gkmonostereo!=0 then
	 kdelayR	=	(kTransComp+kRndDlyR) / (ginsamp/sr)				; delay time required when reading grains from the function table
	endif
	if gkfreeze==1 then
	 kdelay		=	kdelay + (sr/(ginsamp)*1.75*iwsize/sr)				; if freeze mode is active regress the read pointer a small amount
	 if gkmonostereo!=0 then
	  kdelayR	=	kdelayR + (sr/(ginsamp)*1.75*iwsize/sr)				; if freeze mode is active regress the read pointer a small amount
	 endif
	endif
	aphsR	wrap	(gaphsW-kdelay+kManPtr)*(ginsamp/sr),0,(ginsamp-iwsize-irnd)/sr		; location from which to read grain. This is always directly related to the poistion of the write pointer.
	aphsR_R	wrap	(gaphsW-kdelayR+kManPtr)*(ginsamp/sr),0,(ginsamp-iwsize-irnd)/sr	; location from which to read grain. This is always directly related to the poistion of the write pointer.
	iwfn	=	giwfn1+i(gkwfn)-1							; Grain amplitude windowing shape
	
	/*sndwarp*/
	asig,ac	sndwarp 1, aphsR, apch, gibuffer, ibeg, iwsize, irnd, iolap, iwfn, imode
	if gkbalance==1 then									; if 'balance switch is on...
	 asig	balance	asig,ac									; ... amplitude balance the signal
	endif
	if gkmonostereo==1 then									; if 'mono' mode seleced...
	 gaFBackSig	=	gaFBackSig+(asig*gkfback)					; create feedback signal for next iteration. (This will be written into the function table along with the live audio in.)
	 aR	=	asig
	elseif gkmonostereo==2 then								; or if 'stereo out' mode
	 aR,acR	sndwarp 1, aphsR_R, apch, gibuffer, ibeg, iwsize, irnd, iolap, iwfn, imode
	 if gkbalance==1 then
	  aR	balance	aR,acR
	 endif
	 gaFBackSig	=	gaFBackSig+((asig+aR)*gkfback)					; create feedback signal, a mixture of the left and right sndwarp output channels
	else											; otherwise 'stereo in/out' mode
	 aR,acR	sndwarp 1, aphsR_R, apch, gibufferR, ibeg, iwsize, irnd, iolap, iwfn, imode
	 if gkbalance==1 then
	  aR	balance	aR,acR
	 endif
	 gaFBackSig	=	gaFBackSig+(asig*gkfback)					; left channel feedback signal
	 gaFBackSig	=	gaFBackSig+(aR*gkfback)						; right channel feedback signal
	endif	
	rireturn										; return from reinitialisation
	aAntiClick	linsegr	0,0.03,1,0.03,0
		outs	asig*gkamp*gkmix*aAntiClick, aR*gkamp*gkmix*aAntiClick			; send audio to outputs
endin

instr	UpdateWidgets
	ksemis	chnget	"semis"									; read in 'semis' widget
	ktrig1	changed	ksemis									; if 'semis' knob is moved...
	if ktrig1==1 then			
	 chnset	semitone(ksemis), "pch"								; update 'Pitch' knob with the value of semis (converted to a ratio)
	endif
endin

</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
;i 2 0 [3600*24*7]
i "UpdateWidgets" 0 [3600*24*7]
</CsScore>


</CsoundSynthesizer>



























; Modulating Delays -  randomly modulating delays

; GATE (for gating the incoming signal)
; On Thrsh / Off Thrsh	-	thresholds at which the gate will open and close
; Rise Time		-	time it takes for the gate to ramp open
; Rel Time		-	time it takes for the gate to ramp closed

; SETUP
; Pan Width		-	extent of the random panning effect (0 = all layers centred)
; Spread		-	spread of the delay times of the different layers (if only 1 layer is active this control isn't really useful)
;				 if 'spread' is zero, all layers' delay time modulated between  'Min Time' and 'Max Time'
; Layers		-	Number of layers of modulating delays

; REVERB - there is a reverb effect within the each delay buffer
; Amount		-	amount of reverb signal added to the output of the delay buffer - within the delay buffer -, this will therefore be mixed into the feedback signal and fed back into to reverb also
; Time			-	reverb time
; Damping		-	damping of high frequencies within the delay effect
; ** CAUTION MUST BE TAKEN WHEN USING HIGH LEVELS OF DELAY FEEDBACK WITH A HIGH REVERB AMOUNT AS OVERLOADING CAN QUICKLY OCCUR **

; OUTPUT
; Dry			-	level control for the dry (post gate) signal
; Wet			-	level control for the wet (delayed) signal

; SLIDERS
; Min Time		-	Minimum delay time in the delay time random modulation
; Max Time		-	Maximum delay time in the delay time random modulation
; Min Rate		-	Minimum rate in the random modulation
; Max Rate		-	Maximum rate in the random modulation
; Min F.back		-	Minimum feedback value in the modulation of delay feedback values
; Max F.back		-	Maximum feedback value in the modulation of delay feedback values
; Min Tone		-	Minimum cutoff frequency in a randomly modulting low-pass filter within each delay buffer
; Max Tone		-	Maximum cutoff frequency in a randomly modulting low-pass filter within each delay buffer


<Cabbage>
form size(960, 260), caption("Modulating Delays"), pluginid("mdel") style("legacy")

label    bounds(200,  5, 80, 11), text("G A T E"), fontcolour("white")
checkbox bounds( 10, 35,100, 15), text("Gate On/Off") channel("GateActive"), FontColour("White"), colour("lime")  value(1)
rslider  bounds(100, 20, 60, 60), channel("OnThreshold"),  text("On Thrsh."),  range(0, 1, 0.05, 0.5, 0.001),  colour(30,30,30) trackercolour(white)
rslider  bounds(160, 20, 60, 60), channel("OffThreshold"), text("Off Thrsh."), range(0, 1, 0.01, 0.5, 0.001),  colour(30,30,30) trackercolour(white)
rslider  bounds(220, 20, 60, 60), channel("RiseTime"),     text("Rise Time"),  range(0, 10, 1, 0.5, 0.001),    colour(30,30,30) trackercolour(white)
rslider  bounds(280, 20, 60, 60), channel("RelTime"),      text("Rel.Time"),   range(0, 10, 0.01, 0.5, 0.001), colour(30,30,30) trackercolour(white)
checkbox bounds(340, 35, 90, 15), text("Gating") channel("gating"), FontColour("White"), colour("red")  value(0) shape(ellipse)

label    bounds(487,  5, 80, 11), text("S E T U P"), fontcolour("white")
line     bounds(405, 25,  3, 50), colour("Grey")
rslider  bounds(420, 20, 60, 60), channel("width"),  text("Pan Width"),range(0, 0.5, 0.5),         colour(100,100,100) trackercolour(white)
rslider  bounds(480, 20, 60, 60), channel("spread"), text("Spread"),   range(0, 4, 1, 0.5, 0.001), colour(100,100,100) trackercolour(white)
rslider  bounds(540, 20, 60, 60), channel("layers"), text("Layers"),   range(1, 22, 8,1,1),        colour(100,100,100) trackercolour(white)
line     bounds(610, 25,  3, 50), colour("Grey")
label    bounds(680,  5, 80, 11), text("R E V E R B"), fontcolour("white")
rslider  bounds(620, 20, 60, 60), channel("RvbAmt"),  text("Amount"),   range(0, 0.2, 0.06,1,0.001), colour(100,100,100) trackercolour(white)
rslider  bounds(680, 20, 60, 60), channel("RvbTime"), text("Time"),     range(0.01,10,    7),        colour(100,100,100) trackercolour(white)
rslider  bounds(740, 20, 60, 60), channel("damping"), text("Damping"),  range(0, 1, 0.5),            colour(100,100,100) trackercolour(white)
line     bounds(810, 25,  3, 50), colour("Grey")
label    bounds(856,  5, 80, 11), text("O U T P U T"), fontcolour("white")
rslider  bounds(825, 20, 60, 60), channel("dry"),    text("Dry"),      range(0, 4, 0, 0.5, 0.001), colour(160,160,160) trackercolour(white)
rslider  bounds(885, 20, 60, 60), channel("wet"),    text("Wet"),      range(0, 4, 1, 0.5, 0.001), colour(160,160,160) trackercolour(white)

line     bounds(  0, 95,960, 3), colour("Grey")

;hslider2 bounds(10,110, 940, 30), channel("MinTime","MaxTime"), text(Time), min(0.131), max(0.9)    range(0.001, 10,1,0.5),  colour("blue"),   trackercolour(blue)
hrange    bounds(10,110, 940, 30), channel("MinTime","MaxTime"), text("Time"), range(0.001, 10, 0.131:0.9, 0.5),  colour("blue"),   trackercolour("blue")
;hslider2 bounds(10,140, 940, 30), channel("MinRate","MaxRate"), min(0.1),  max(0.2),   text("Rate"),    range(0.001, 10,1,0.5),   colour("red"),    trackercolour(red)
hrange    bounds(10,140, 940, 30), channel("MinRate","MaxRate"), text("Rate"), range(0.001, 10, 0.1:0.2, 0.5),  colour("red"),   trackercolour("red")
;hslider2 bounds(10,170, 940, 30), channel("MinFB","MaxFB"),     min(0.95), max(0.975), text("F.back"),  range(0, 0.999, 0.95,1,0.001),  colour("yellow"), trackercolour(yellow)
hrange    bounds(10,170, 940, 30), channel("MinFB","MaxFB"), text("F.back"), range(0, 0.999, 0.95:0.975, 0.5),  colour("yellow"),   trackercolour("yellow")
;hslider2 bounds(10,200, 940, 30), channel("MinTone","MaxTone"), min(11),   max(14),    text("Tone"),    range(4, 14, 11),               colour(purple),   trackercolour(purple)
hrange    bounds(10,200, 940, 30), channel("MinTone","MaxTone"), text("Tone"), range(4, 14, 11:14, 1),  colour("purple"),   trackercolour("purple")

label   bounds(  5,240, 170, 12), text("Author: Iain McCurdy |2013|"), FontColour("grey")

</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-n -d
</CsOptions>

<CsInstruments>

sr = 44100
ksmps = 16
nchnls = 2
0dbfs=1


opcode	DelayIteration,aa,aakkkkkkkkkkkKiii
 aL,aR,kMinTime,kMaxTime,kMinRate,kMaxRate,kMinFB,kMaxFB,kwidth,kMinTone,kMaxTone,kRvbAmt,kRvbTime,kdamping,ispread,icount,ilayers	xin

 kdel		rspline	kMinTime,kMaxTime,kMinRate,kMaxRate
 kdel		limit	kdel,0.001,10
 adel		interp	kdel
 kFB		rspline	kMinFB,kMaxFB,kMinRate,kMaxRate
 kFB		limit	kFB,0,1
 
 iMaxDel	=	10
 iws		=	4
 aout1, aout2	init	0

 ktone		rspline	kMinTone, kMaxTone,kMinRate,kMaxRate
 ktone		=	cpsoct(ktone)
 
 idelOS		random	0,ispread
  
 abuf		delayr	iMaxDel+idelOS
 aout1		deltap3	adel+idelOS
 arvb 		nreverb aout1, kRvbTime, kdamping
 aout1		=	aout1 + arvb*kRvbAmt
 aout1		dcblock2	aout1
 aout1		tone	aout1,ktone
 		delayw	aL + aout1*kFB
 		
 abuf		delayr	iMaxDel+idelOS
 aout2		deltap3	adel+idelOS
 arvb 		nreverb aout2, kRvbTime, kdamping
 aout2		=	aout2 + arvb*kRvbAmt
 aout2		dcblock2	aout2
 aout2		tone	aout2,ktone
 		delayw	aR + aout2*kFB

 kpan		rspline	0.5-kwidth,0.5+kwidth,kMinRate,kMaxRate
 kpan		limit	kpan,0,1
 apan		interp	kpan
 aout1		=	aout1*(1-apan)
 aout2		=	aout2*(apan)
 amix1,amix2	init	0
 
 amix1	=	0
 amix2	=	0
 
 if icount<ilayers then
  amix1,amix2	DelayIteration	aL,aR,kMinTime,kMaxTime,kMinRate,kMaxRate,kMinFB,kMaxFB,kwidth,kMinTone,kMaxTone,kRvbAmt,kRvbTime,kdamping,ispread,icount+1,ilayers
 endif
 
		xout	aout1+amix1, aout2+amix2
endop

opcode	SwitchPort, k, kki
	kin,kupport,idnport	xin
	kold			init	0
	kporttime		=	(kin<kold?idnport:kupport)
	kout			portk	kin, kporttime
	kold			=	kout
				xout	kout
endop


instr	1
 kporttime	linseg	0,0.001,1
 aL,aR	ins
 ;aL,aR	diskin2	"Synthpad.wav",1,0,1
 ;aL	diskin2	"loop.wav",1,0,1
 ;aR	=	aL
 
 kMinTime	chnget	"MinTime"
 kMaxTime	chnget	"MaxTime"
 kMinTime	portk	kMinTime,kMinTime			
 kMaxTime	portk	kMaxTime,kMaxTime			
 kspread	chnget	"spread"
 kMinRate	chnget	"MinRate"
 kMaxRate	chnget	"MaxRate"
 kMinFB		chnget	"MinFB"
 kMaxFB		chnget	"MaxFB"
 kwidth		chnget	"width"
 kMinTone	chnget	"MinTone"
 kMaxTone	chnget	"MaxTone"
 kres		chnget	"res"
 klayers	chnget	"layers"
 klayers	init	1
 
 kdry		chnget	"dry"
 kwet		chnget	"wet"
 kGateActive	chnget	"GateActive"
 kOnThreshold	chnget	"OnThreshold"
 kOffThreshold	chnget	"OffThreshold"
 kRiseTime	chnget	"RiseTime"
 kRelTime	chnget	"RelTime"

 kRvbAmt	chnget	"RvbAmt"
 kRvbTime	chnget	"RvbTime"
 kdamping	chnget	"damping"
 
 /* GATE */
 if kGateActive=1 then
  krms	rms	(aL+aR)*0.5
  kthreshold	init	i(kOnThreshold)
  kon	=	1
  koff	=	0
  if krms>kthreshold then
   kgate	=	1
   kthreshold	=	kOnThreshold
   		chnset	koff,"gating"
  else
   kgate	=	0
   kthreshold	=	kOffThreshold
   		chnset	kon,"gating"
  endif    
  kgate	SwitchPort	kgate,kRiseTime,0.1
  kgate	expcurve	kgate,8
  agate	interp	kgate
  aL	=	aL * agate
  aR	=	aR * agate
 endif
 ktrig	trigger	kGateActive,0.5,1
 if ktrig=1 then
  chnset	koff,"gating"
 endif


 /* REINITIALISING */
 ktrig	changed	klayers,kspread
 if ktrig=1 then
  reinit UPDATE
 endif
 UPDATE:
 
 /* CALL THE UDO */
 aout1,aout2	DelayIteration	aL,aR,kMinTime,kMaxTime,kMinRate,kMaxRate,kMinFB,kMaxFB,kwidth,kMinTone,kMaxTone,kRvbAmt,kRvbTime,kdamping,i(kspread),1,i(klayers)
 rireturn

 		outs	(aout1 * kwet) + (aL * kdry), (aout2 * kwet) + (aR * kdry)
endin

</CsInstruments>  

<CsScore>
i 1 0 360000
</CsScore>

</CsoundSynthesizer>; MultiModeDelay.csd
; Written by Iain McCurdy, 2012.

<Cabbage>
form caption("Multi-mode Delay") size(435, 90), pluginid("MMDl") style("legacy")
image                    bounds(0, 0,435, 90), colour("Maroon"), shape("rounded"), outlinecolour("white"), outlinethickness(4)
label    bounds( 10, 22, 80, 12), text("Type"), fontcolour("white")
combobox bounds( 10, 35, 80, 20),  text("Ping Pong","Stereo"), channel("type"), value(1)
rslider  bounds( 95, 11, 70, 70),  text("Time"),     channel("time"),     range(0.001, 10, 0.4, 0.5), colour( 85,  0,  0) trackercolour("silver"), textcolour("silver")
rslider  bounds(160, 11, 70, 70),  text("Cutoff"),   channel("cutoff"),   range(20,20000,20000,0.5),  colour( 85,  0,  0) trackercolour("silver"), textcolour("silver")
rslider  bounds(225, 11, 70, 70), text("Feedback"), channel("feedback"), range(0, 1.00, 0.5),        colour( 85,  0,  0) trackercolour("silver"), textcolour("silver")
rslider  bounds(290, 11, 70, 70), text("Mix"),      channel("mix"),      range(0, 1.00, 0.5),        colour( 85,  0,  0) trackercolour("silver"), textcolour("silver")
rslider  bounds(355, 11, 70, 70), text("Level"),    channel("level"),    range(0, 1.00, 0.7),        colour( 85,  0,  0) trackercolour("silver"), textcolour("silver")

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

instr	1
	ktime		chnget	"time"					;READ WIDGETS...
	kcutoff		chnget	"cutoff"				;
	kfeedback	chnget	"feedback"				;
	kmix		chnget	"mix"					;
	klevel		chnget	"level"					;
	asigL, asigR	ins
	kporttime	linseg	0,0.01,0.03				;CREATE A VARIABLE THAT WILL BE USED FOR PORTAMENTO TIME
	ktime		portk	ktime,kporttime				;PORTAMENTO SMOOTHING OF DELAT TIME
	atime		interp	ktime					;INTERPOLATED A-RATE VERSION OF DELAY TIME
	ktype		chnget	"type"
	if ktype==1 then
	 aL_OS		vdelay	asigL,(atime*1000)/2,(10*1000)/2	;DELAYED OFFSET OF LEFT CHANNEL (FIRST 'PING')
	 ;LEFT CHANNEL
	 abuf		delayr	10					;ESTABLISH DELAY BUFFER
	 aDelL		deltapi	atime					;TAP BUFFER
	 aDelL		tone	aDelL,kcutoff				;LOWPASS FILTER DELAY TAP 
	 		delayw	aL_OS+(aDelL*kfeedback)			;WRITE INPUT AUDIO INTO BUFFER
	 ;RIGHT CHANNEL
	 abuf		delayr	10					;ESTABLISH DELAY BUFFER
	 aDelR		deltapi	atime					;TAP BUFFER
	 aDelR		tone	aDelR,kcutoff				;LOWPASS FILTER DELAY TAP
	 		delayw	asigR+(aDelR*kfeedback)			;WRITE INPUT AUDIO INTO BUFFER
	 amixL		ntrpol	asigL,aDelL+aL_OS,kmix			;MIX DRY AND WET SIGNALS (LEFT CHANNEL)
	 amixR		ntrpol	asigR,aDelR,kmix			;MIX DRY AND WET SIGNALS (RIGHT CHANNEL)
	elseif ktype==2 then
	 ;LEFT CHANNEL
	 abuf		delayr	10					;ESTABLISH DELAY BUFFER
	 aDelL		deltapi	atime					;TAP BUFFER
	 aDelL		tone	aDelL,kcutoff				;LOWPASS FILTER DELAY TAP 
	 		delayw	asigL+(aDelL*kfeedback)			;WRITE INPUT AUDIO INTO BUFFER
	 ;RIGHT CHANNEL
	 abuf		delayr	10					;ESTABLISH DELAY BUFFER
	 aDelR		deltapi	atime					;TAP BUFFER
	 aDelR		tone	aDelR,kcutoff				;LOWPASS FILTER DELAY TAP
	 		delayw	asigR+(aDelR*kfeedback)			;WRITE INPUT AUDIO INTO BUFFER
	 amixL		ntrpol	asigL,aDelL,kmix			;MIX DRY AND WET SIGNALS (LEFT CHANNEL)
	 amixR		ntrpol	asigR,aDelR,kmix			;MIX DRY AND WET SIGNALS (RIGHT CHANNEL) 		
	endif
			outs	amixL*klevel, amixR*klevel		;PING PONG DELAY OUTPUTS ARE SENT OUT
endin
		
</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
</CsScore>

</CsoundSynthesizer>; MultitapDelay.csd
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
; Peeaaoooh.csd
; Written by Iain McCurdy, 2015

; Generates reverberation effects reminiscent of spring reverbs.

; The main element is a stack of delays with geometrically related delay times

; NB. although delays are used, their delay times are typically quite short and are expressed here in hertz, i.e. frequency of echoes (freq = 1/delay_time).
; Furthermore, ratios between delay times of parallel delays are expressed in semitones.

; Base		-	Base Frequency of the stack of delays (in hertz)	
; Layers	-	Number of layers (delays). Bear in mind that increasing this increaeses CPU and memory demand.
; Shape		-	Shapes the amplitude relationship between layers. If Shape=0, all layers are equal in amplitude, as Shape is increased, longer delays have their amplitudes attenuated.
; Interval	-	Interval between consecutive delays (in semitones).
; Scatter	-	Adds a fixed random value to the delay times of the delays. Increasing this value will gradually obliterate the distinctive 'peeaaoo' sound and also add a stereo broadening effect.
; Spread	-	Amount by which left and right fo the same delat layer are alternately panned left or right. This can be used as an alternative to 'Scatter' to create stereo broadening but without the obliteration of the 'peeaaoo' sound.
; Feedback	-	Ratio of the output signal from each delay that is fed back into the input. The knob turns red to warn of greater than 100% feedback.
; HPF		-	Cutoff frequency of a highpass filter within the feedback loop of each delay
; LPF		-	Cutoff frequency of a lowpass filter within the feedback loop of each delay
; Test Noise-	When activated, the live audio input is replaced with a sparse 'dust' source sound. Good for testing the effect.
; Sep.Mode	-	There are two separation modes: Linear: sequential delays in the stack are separated by equal pitch intervals
;							Exponential: intervals between sequential delays increase exponentially
;			Linear offers a more distinctive 'peeaaoooh' sound and arpeggios when 'Base' is low. Exponential offers a more 'scattered' sounding reverb.
;			When 'Interval' is low, both modes sound quite similar.
; Post-Filter
; HPF		-	Cutoff frequency of a highpass filter applied to the output signal (dry and wet signals)
; LPF		-	Cutoff frequency of a lowpass filter applied to the output signal (dry and wet signals)

; Mix		-	Dry/wet mix
; Level		-	Output level (dry and wet signals)

; NB. Feedback greater than 1 (i.e. more than 100% of the signal) are possible. 
; Normally this would result in a runaway increase in amplitude but control is still possible if the main highpass and lowpass filters also remove part of the audio in the feedback loop.
; Nonetheless surity is not guaranteed so therefore experimentation and caution is advised. 
; There is however a safety mechanism which will reduce the feedback control if the amplitude starts to runaway.

;                   (feedback)
;             +----[LPF]--[HPF]---+
;             |                   |  
;             v   +-----------+   | (mix)  +-----------+
;  --IN--+----+---|DELAY_STACK|---+---+----|POST FILTER|--OUT-->
;        |        +-----------+       ^    +-----------+
;        |                            |
;        +----------------------------+

<Cabbage>
form caption("Peeaaoooh") size(650,207), pluginid("Peao") style("legacy")
image pos(0, 0),          size(650,207), colour( 10, 20, 30), shape("sharp"), outlinecolour("white"), outlinethickness(0) 

rslider   bounds( 10, 10, 70, 70),  channel("Base"),text("Base"), colour( 20, 60, 70), trackercolour(200,240,250), textcolour("white"), range(0.5, 1000, 50, 0.5, 0.001)
rslider   bounds( 80, 10, 70, 70),  channel("Layers"),text("Layers"), colour( 20, 60, 70), trackercolour(200,240,250), textcolour("white"), range(1, 60, 8,1,1)
rslider   bounds(150, 10, 70, 70),  channel("Interval"),text("Interval"), colour( 20, 60, 70), trackercolour(200,240,250), textcolour("white"), range(0.001, 1, 0.07,0.5,0.001)
rslider   bounds(220, 10, 70, 70),  channel("Shape"),text("Shape"), colour( 20, 60, 70), trackercolour(200,240,250), textcolour("white"), range(0, 3, 0,1,0.001)
rslider   bounds(290, 10, 70, 70),  channel("Scatter"),text("Scatter"), colour( 20, 60, 70), trackercolour(200,240,250), textcolour("white"), range(0, 1, 0,0.5,0.0001)
rslider   bounds(360, 10, 70, 70),  channel("Spread"),text("Spread"), colour( 20, 60, 70), trackercolour(200,240,250), textcolour("white"), range(0, 1, 0)
rslider   bounds(430, 10, 70, 70),  channel("Feedback"),text("Feedback"), colour( 20, 60, 70), trackercolour(200,240,250), textcolour("white"), range(0, 1.5, 0.95), identchannel("FeedbackID")
checkbox  bounds(430, 82, 80, 14),  channel("Overload"),text("Overload"), colour(255, 60, 60), textcolour("white") shape("Ellipse"), active(0)
rslider   bounds(500, 10, 70, 70),  channel("HPF"),   text("HPF"), colour( 20, 60, 70), trackercolour(200,240,250), textcolour("white"), range(20,20000,20,0.5,1)
rslider   bounds(570, 10, 70, 70),  channel("LPF"),   text("LPF"), colour( 20, 60, 70), trackercolour(200,240,250), textcolour("white"), range(20,20000,20000,0.5,1)

checkbox  bounds(320,112,110, 15), text("Test Clicks"), channel("TestNoise"), fontcolour("white")

label     bounds(320,137,110, 12), text("Separation Mode"), fontcolour("white") 
combobox  bounds(320,150,110, 20),  channel("SepMode"),text("Linear","Exponential"), textcolour("white"), value(1)

image     bounds( 20,100,245, 95), outlinecolour("white"), outlinethickness(1), colour(50,50,50,100), plant("PostFilter") {
label     bounds(  0,  6,245, 14), text("G l o b a l    F i l t e r"), fontcolour("white") 
combobox  bounds( 10, 40, 75, 20),  channel("PrePostFilter"),text("Pre-mix","Post-mix"), textcolour("white"), value(2)
rslider   bounds( 85, 22, 70, 70),  channel("PostHPF"),   text("HPF"), colour( 20, 60, 70), trackercolour(200,240,250), textcolour("white"), range(20,20000,20,0.5,1)
rslider   bounds(155, 22, 70, 70),  channel("PostLPF"),   text("LPF"), colour( 20, 60, 70), trackercolour(200,240,250), textcolour("white"), range(20,20000,20000,0.5,1)
}

rslider   bounds(480,110, 70, 70),  channel("Mix"),text("Mix"), colour( 20, 60, 70), trackercolour(200,240,250), textcolour("white"), range(0, 1, 1)
rslider   bounds(560,110, 70, 70),  channel("Level"),text("Level"), colour( 20, 60, 70), trackercolour(200,240,250), textcolour("white"), range(0, 2, 1,0.5,0.001)

label     bounds(530,195,100, 10), text("Iain McCurdy |2015|"), fontcolour(200,200,200) 

</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-d -n
</CsOptions>

<CsInstruments>

sr 		= 	44100	;SAMPLE RATE
ksmps 		= 	16	;NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls 		= 	2	;NUMBER OF CHANNELS (2=STEREO)
0dbfs		=	1
		seed	0
		
opcode	DelayLayer,a,akkkkkkkkkip
 aInSig,kBase,kShape,kInterval,kScatter,kSpread,kSepMode,kHPF,kLPF,kFeedback,iLayers,iCount	xin
 iRnd	random	-0.5,0.5			; i-time random value. A unique fixed value for each delay layer (and channel).
 kRnd	=	octave(iRnd * kScatter)		; scale random value by GUI widget control
 /* SELECT THE SEPARATION MODE */
 if kSepMode==1 then								; linear
  aDel	interp	limit:k(kRnd/(kBase * semitone(kInterval*(iCount-1))),1/kr,2)
 else										; exponential
  aDel	interp	limit:k(kRnd/(kBase * semitone((kInterval+1)^iCount)),1/kr,2)
 endif
 kAmp	=	(iCount/iLayers) ^ kShape
 abuf	delayr	2
 aWG	deltapi	aDel
 aWG	atone	aWG,kHPF
 aWG	tone	aWG,kLPF
 aWG	dcblock2	aWG
 	delayw	aInSig + (aWG*kFeedback)
 if iCount<iLayers then
  aMix	DelayLayer	aInSig,kBase,kShape,kInterval,kScatter,1-kSpread,kSepMode,kHPF,kLPF,kFeedback,iLayers,iCount+1
 endif
	xout	aWG*limit:k(kSpread*2,0,1)*kAmp + aMix
 aMix	=	0
endop

instr	1

 /* READ IN WIDGETS */
 kporttime	linseg	0,0.001,0.1
 kBase		chnget	"Base"
 iBase		chnget	"Base"
 kBase		init	iBase
 kBase		portk	kBase,kporttime
 kInterval	chnget	"Interval"
 iInterval	chnget	"Interval"
 kinterval	init	iInterval
 kInterval	portk	kInterval,kporttime
 kScatter	chnget	"Scatter"
 kScatter	portk	kScatter,kporttime
 kSpread	chnget	"Spread"
 kFeedback	chnget	"Feedback"
 kLevel		chnget	"Level"
 kLayers	chnget	"Layers"
 kShape		chnget	"Shape"
 kHPF		chnget	"HPF"
 kLPF		chnget	"LPF"
 kSepMode	chnget	"SepMode"
 kPostHPF	chnget	"PostHPF"
 kPostLPF	chnget	"PostLPF"
 kMix		chnget	"Mix"
 kPrePostFilter		chnget	"PrePostFilter"
 kTestNoise	chnget	"TestNoise"
 
 if kTestNoise==1 then
  aL	dust2	1,0.5*randomi:k(0.5,2,1,1)
  aR	dust2	1,0.5*randomi:k(0.5,2,1,1)
  aL	butlp	aL,cpsoct(randomh:k(5,11,10))
  aR	butlp	aR,cpsoct(randomh:k(5,11,10))
 else
  aL,aR	ins
 endif
  
 /* CALL DELAY STACK (VIA A UDO) */
 if changed(kLayers)==1 then
  reinit	UPDATE
 endif
 UPDATE:
 iAmpScl	=	sqrt(i(kLayers))							; amplitude will be scaled according to the number of layers selected
 aWG_L	DelayLayer	aL,kBase,kShape,kInterval,kScatter,0.5+kSpread/2,kSepMode,kHPF,kLPF,kFeedback,i(kLayers)
 aWG_R	DelayLayer	aR,kBase,kShape,kInterval,kScatter,0.5-kSpread/2,kSepMode,kHPF,kLPF,kFeedback,i(kLayers)
 aWG_L	/=	iAmpScl										; scale amplitude according to the number of layers chosen
 aWG_R	/=	iAmpScl
 rireturn
        
 /* TURN FEEDBACK KNOB RED IF GREATER THAN 1 */       
 if trigger:k(kFeedback,1,0)==1 then		; trigger to turn red
  chnset "colour(170, 60, 70)","FeedbackID"	; send message
 elseif trigger:k(kFeedback,1,1)==1 then	; trigger to turn back to normal
  chnset "colour( 20, 60, 70)","FeedbackID"	; send message
 endif
         
 /* PREVENT OVERLOADS */
 kRMS	rms	aWG_L				; scan rms
 kOn	=	1				; on value (needs to be a k-rate var.)
 kOff	=	0				; off value (needs to be a k-rate var.)
 kOverload	chnget	"Overload"		; read in value of 'Overload' LED
 if kRMS>0.9 then				; if rms exceeds a given value...
  chnset	kOn,"Overload"			; turn on 'Overload' LED
  chnset	(kFeedback - (0.0003)) - ((kFeedback-1)*0.001),"Feedback"	; decrement 'Feedback' slider. This will be repeated every k-cycle until rms is back below the threshold value prescribed above.
 elseif kOverload==1 then			; otherwise..., i.e. if 'Overload' LED is on but rms is beneath the threshold...  
  chnset	kOff,"Overload"			; turn 'overload' LED off
 endif	

 /* GLOBAL FILTERING CAN BE APPLIED TO JUST THE WET SIGNAL OR A MIXTURE OF THE WET AND DRY SIGNALS, ACCORDING TO A SELECTION MADE USING A COMBOBOX */ 
 /* PRE-MIXER FILTERING OPTION */
 if kPrePostFilter==1 then
  if kPostHPF>20 then			; if HPF is at its minimum value (20) bypass filtering to conserve CPU
   aWG_L	buthp	aWG_L,kPostHPF	; highpass filter left channel
   aWG_R	buthp	aWG_R,kPostHPF	; highpass fiter right channel
  endif
  if kPostLPF<20000 then 		; if HPF is at its minimum value (20) bypass filtering to conserve CPU
   aWG_L	butlp	aWG_L,kPostLPF		
   aWG_R	butlp	aWG_R,kPostLPF		
  endif
  aMixL	ntrpol	aL, aWG_L, kMix
  aMixR	ntrpol	aR, aWG_R, kMix
 /* PRE-MIXER FILTERING OPTION */
 elseif kPrePostFilter==2 then
  aMixL	ntrpol	aL, aWG_L, kMix
  aMixR	ntrpol	aR, aWG_R, kMix
  if kPostHPF>20 then
   aMixL	buthp	aMixL,kPostHPF		
   aMixR	buthp	aMixR,kPostHPF		
  endif
  if kPostLPF<20000 then 
   aMixL	butlp	aMixL,kPostLPF		
   aMixR	butlp	aMixR,kPostLPF		
  endif
 endif

	outs	aMixL*kLevel, aMixR*kLevel
endin
		
</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
</CsScore>

</CsoundSynthesizer>
; Reverse.csd
; Iain McCurdy [2012]
; 
; Buffers audio for reversed (and forward) playback.
; 
; INSTRUCTIONS
; ------------
; Time L  	--	length of the left channel delay buffer 
; Time R  	--	length of the right channel delay buffer
; Spread		--	stereo spread of the two channel. 1 = hard left and right
; Mix		--	dry/wet mix
; Level		--	output level
; Reverse 	--	(switch) activate reversed buffer
; Forward	--	(switch) activate forward buffer
; Link L&R	--	(switch) pair the left and right Time controls (functions differently if 'Time Mod' is activated)
; Time Mod	--	(switch) if this switch is on the delay times for both channels will modulate randomly between 'Time L' and 'Time R'. If 'Link L&R' is active this modulation is done in tandem for both channels.
; Pan Mod	--	(switch) if this switch is on the panning modulates randomly



<Cabbage>
form caption("Reverse") size(455, 95), pluginid("rvrs") style("legacy")
image           bounds(0, 0, 455, 95), colour("darkslategrey"), shape("rounded"), outlinecolour("white"), line(4)
rslider  bounds( 10, 10,  75, 75), text("Time L"),    channel("timeL"),   range(0.010, 4, 1, 0.5,0.001),colour(37,59,59)   textcolour(255,255,200), trackercolour(lightblue)
rslider  bounds( 80, 10,  75, 75), text("Time R"),    channel("timeR"),   range(0.010, 4, 1, 0.5,0.001),colour(37,59,59)   textcolour(255,255,200), trackercolour(lightblue)
rslider  bounds(150, 10, 75, 75),  text("Spread"),    channel("spread"),  range(0, 1.00, 1),            colour(37,59,59)   textcolour(255,255,200), trackercolour(lightblue)
rslider  bounds(220, 10, 75, 75),  text("Mix"),       channel("mix"),     range(0, 1.00, 1),            colour(37,59,59)   textcolour(255,255,200), trackercolour(lightblue)
rslider  bounds(290, 10, 75, 75),  text("Level"),     channel("level"),   range(0, 1.00, 1, 0.5),       colour(37,59,59)   textcolour(255,255,200), trackercolour(lightblue)
checkbox bounds(370, 12, 100, 12), text("Reverse"),   channel("reverse"),  value(1),                    colour(255,255, 50)     fontcolour(255,255,200)
checkbox bounds(370, 27, 100, 12), text("Forward"),   channel("forward"),  value(0),                    colour(255,255, 50)     fontcolour(255,255,200)
checkbox bounds(370, 42, 100, 12), text("Link L&R"),  channel("link"),     value(0),                    colour(255,255, 50)     fontcolour(255,255,200)
checkbox bounds(370, 57, 100, 12), text("Time Mod."), channel("TMod"),    value(0),                     colour(255,255, 50)     fontcolour(255,255,200)
checkbox bounds(370, 72, 100, 12), text("Pan Mod."),  channel("PMod"),    value(0),                     colour(255,255, 50)     fontcolour(255,255,200)
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

;Author: Iain McCurdy (2012)
;http://iainmccurdy.org/csound.html

opcode	Reverse, a, aKkkk			;nb. CAPITAL K CREATE A K-RATE VARIABLE THAT HAS A USEFUL VALUE ALSO AT I-TIME
	ain,ktime,kreverse,kforward,klink	xin			;READ IN INPUT ARGUMENTS
	;four windowing envelopes. An appropriate one will be chosen based on the reversed chunk duration
	ienv1	ftgenonce			0, 0, 131072, 7, 0, 1024,           1, 131072-(1024*2),           1,  1024,       0	;for longest chunk times
	ienv2	ftgenonce			0, 0, 131072, 7, 0, 4096,           1, 131072-(4096*2),           1,  4096,       0
	ienv3	ftgenonce			0, 0, 131072, 7, 0,16384,           1, 131072-(16384*2),          1, 16384,       0
	ienv4	ftgenonce			0, 0, 131072, 7, 0,32768,           1, 131072-(32768*2),          1, 32768,       0	;for shortest chunk times

	atime	interp	ktime			;INTERPOLATE TO CREATE A-RATE VERSION OF K-TIME
		
	iratio	=	octave(1)
	
	ktrig	trigger	klink,0.5,0		;if 'Link L&R' is turned on restart delay time phasors to ensure sync between the two channels
	if ktrig=1 then
	 reinit	RESTART_PHASOR
	endif
	RESTART_PHASOR:
	aptr	phasor	(2/ktime)		;CREATE A MOVING PHASOR THAT WITH BE USED TO TAP THE DELAY BUFFER
	rireturn
	if ktime<0.2 then			;IF CHUNK TIME IS LESS THAN 0.2... (VERY SHORT) 
	 aenv	table3	aptr,ienv4,1		;CREATE AMPLITUDE ENVELOPE
	elseif ktime<0.4 then
	 aenv	table3	aptr,ienv3,1
	elseif ktime<2 then
	 aenv	table3	aptr,ienv2,1
	else					;other longest bracket of delay times
	 aenv	table3	aptr,ienv1,1
	endif
	aptr	=	aptr*atime		;SCALE PHASOR ACCORDING TO THE LENGTH OF THE DELAY TIME CHOSEN BY THE USER
 
 	abuffer	delayr	4 ;+ 0.01		;CREATE A DELAY BUFFER
	abwd	deltap3	aptr			;READ AUDIO FROM A TAP WITHIN THE DELAY BUFFER
	afwd	deltap3	atime			;FORWARD DELAY
		delayw	ain			;WRITE AUDIO INTO DELAY BUFFER
	
	;rireturn				;RETURN FROM REINITIALISATION PASS
	xout	(abwd*aenv*kreverse)+(afwd*kforward)	;SEND AUDIO BACK TO CALLER INSTRUMENT. APPLY AMPLITUDE ENVELOPE TO PREVENT CLICKS.
endop


instr 1
ktimeL   chnget "timeL"
ktimeR   chnget "timeR"
kspread  chnget "spread"
kmix     chnget "mix"
klevel   chnget "level"
kreverse chnget "reverse"
kforward chnget "forward"
kTMod    chnget "TMod"
kPMod    chnget "PMod"

/* LINK */
klink chnget "link"			; if 'Link L&R' is selected
if klink=1&&kTMod=0 then		
 ktrigL	changed	ktimeL,klink
 ktrigR	changed	ktimeR
 if ktrigL=1 then
  chnset	ktimeL,"timeR"
 elseif ktrigR=1 then
  chnset	ktimeR,"timeL"
 endif
endif

a1,a2	ins

if kTMod=1 then						; if time modulation is selected....
 if klink=0 then					; and if 'link L&R' is off...
  ktime1	rspline	ktimeL,ktimeR,0.2,1		; generate delay time value: random spline between ktimeL and ktimeR 
  ktime2	rspline	ktimeL,ktimeR,0.2,1
  ktimeL	limit	ktime1,0.01,4			; assign to delay time variable and limit to prevent out of range values (possible with rspline)
  ktimeR	limit	ktime2,0.01,4
 else
  ktime		rspline	ktimeL,ktimeR,0.2,1		
  ktimeL	limit	ktime,0.01,4
  ktimeR	=	ktimeL				; right channel delay the same as left
 endif
endif

arev1	Reverse	a1,ktimeL,kreverse,kforward,klink		; call UDO
arev2	Reverse	a2,ktimeR,kreverse,kforward,klink

if kPMod=1 then						; if panning modulation is on...
 kpan	rspline	0,1,0.2,1				; pan position generated as a random spline
 ap1	=	(arev1*kpan)     + (arev2*(1-kpan))	; create new left channel
 ap2	=	(arev1*(1-kpan)) + (arev2*kpan)		; create new right channel
 arev1	=	ap1					; reassign left channel to new left channel
 arev2	=	ap2 					; reassign right channel to new right channel
endif

a1	ntrpol	a1,arev1,kmix			; dry/wet mix
a2	ntrpol	a2,arev2,kmix
a1	=	a1 * klevel			; apply level control
a2	=	a2 * klevel
kspread	scale	kspread,1,0.5 			; rescale from range 0 - 1 to 0.5 - 1
aL	sum	a1*kspread,a2*(1-kspread)	; create stereo mix according to Spread control
aR	sum	a2*kspread,a1*(1-kspread)	; create stereo mix according to Spread control
	outs	aL,aR
endin

</CsInstruments>

<CsScore>
i 1 0 [60*60*24*7]
</CsScore>

</CsoundSynthesizer>; SpectralDelay.csd
; Written by Iain McCurdy, 2016

; This effect implements a spectral delay wherein banded analysed data - amplitudes and frequencies - can be delayed on a band by band basis 
;  and independently for each data type.

; Delay times are typically controlled using function tables as individual controls for each delay time would be impractical.
; In this example the user can choosed from 9 preset table types for delaying amplitude and frequency data. 
; In the future I will add further tables and a user-drawable table option.

; Max.Delay		-	Maximum delay time (before scaling by the table). This is an i-rate control.
; FFT Size		-	FFT size affects the quality of the effect. Small FFT sizes will result in distortions of frequency data.
; Feedback		-	feedback around the spectral delay is performed while the signal is stoll in the format of an f-signal
; Dry/Wet		-	dry/wet control of the effect
; Level			-	output level

<Cabbage>
form caption("") size(530,345), colour("black"), pluginid("SpDl") style("legacy")
image     pos(0, 0), size(530,345), colour("black"), shape("rounded"), outlinecolour(125,130,155), outlinethickness(5) 
;line      bounds(15, 53, 610,  1), colour(150,150,150)

label   bounds(  0,  1,530, 58), text("SPECTRAL DELAY"), fontcolour( 70, 50, 50)
label   bounds(  0,  4,530, 52), text("SPECTRAL DELAY"), fontcolour( 75, 95, 75)
label   bounds(  0,  7,530, 46), text("SPECTRAL DELAY"), fontcolour(150,150,200)
label   bounds(  0, 10,530, 40), text("SPECTRAL DELAY"), fontcolour(220,220,220)

rslider bounds( 25, 60, 90,120), text("Max.Delay"), textbox(1), valuetextbox(1), channel("MaxDelay"), range(0.01, 8, 1, 0.5), colour(105,70,70), trackercolour(205,170,170)

label    bounds(125, 65, 80, 14), text("FFT Size")
combobox bounds(125, 80, 80, 20), text("64","128","256","512","1024","2048"), channel("FFTindex"), value(3)

rslider  bounds(215, 60, 90,120), text("Feedback"), textbox(1), valuetextbox(1), channel("Feedback"), range(0, 1, 0.85), colour(105,70,70), trackercolour(205,170,170)
rslider  bounds(315, 60, 90,120), text("Dry/Wet Mix"), textbox(1), valuetextbox(1), channel("DryWetMix"), range(0, 1, 1), colour(105,70,70), trackercolour(205,170,170)
rslider  bounds(415, 60, 90,120), text("Level"), textbox(1), valuetextbox(1), channel("Level"), range(0, 1, 0.5), colour(105,70,70), trackercolour(205,170,170)

label    bounds( 20,193, 80, 14), text("Amp.Table")
combobox bounds( 20,208, 80, 20), text("Hi to Lo","Lo to Hi","Random","Peak 1","Peak 2","Peak 3","Comb 1","Comb 2","Spring","Flat"), channel("AmpTable"), value(3)
gentable bounds( 20,230,235, 90), identchannel("AmpTableID"), tablenumber(101), amprange(0,1,101), tablecolour("DarkBlue"), zoom(-1), tablebackgroundcolour(200,200,200), tablegridcolour(100,100,100)
label    bounds( 20,322,235, 14), text("Amplitudes Table")

label    bounds(275,193, 80, 14), text("Freq.Table")
combobox bounds(275,208, 80, 20), text("Hi to Lo","Lo to Hi","Random","Peak 1","Peak 2","Peak 3","Comb 1","Comb 2","Spring","Flat"), channel("FrqTable"), value(7)
gentable bounds(275,230,235, 90), identchannel("FrqTableID"), tablenumber(102), amprange(0,1,102), tablecolour("DarkGreen"), zoom(-1), tablebackgroundcolour(200,200,200), tablegridcolour(100,100,100)
label    bounds(275,322,235, 14), text("Frequencies Table")

</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-n -d -+rtmidi=NULL -M0 -m0d 
</CsOptions>

<CsInstruments>

sr = 44100
ksmps = 16
nchnls = 2
0dbfs = 1

giViewTabSize	=	-235*2
giAmpTableView	ftgen	101,0,giViewTabSize,10,1
giFrqTableView	ftgen	102,0,giViewTabSize,10,1

; Tables used purely for viewing
giV1				ftgen			201,0,giViewTabSize,-7,1,abs(giViewTabSize),0									; Hi to Lo
giV2				ftgen			202,0,giViewTabSize,-7,0,abs(giViewTabSize),1									; Lo to Hi
giV3				ftgen			203,0,giViewTabSize,-21,1,1													; Random
giV4				ftgen			204,0,giViewTabSize,-16,0,abs(giViewTabSize)*0.25,-4,1,abs(giViewTabSize)*0.75,4,0	; Peak 1
giV5				ftgen			205,0,giViewTabSize,-16,0,abs(giViewTabSize)*0.125,-4,1,abs(giViewTabSize)*0.75,4,0	; Peak 2
giV6				ftgen			206,0,giViewTabSize,-16,0,abs(giViewTabSize)*0.0125,-4,1,abs(giViewTabSize)*0.075,4,0	; Peak 3
giV7				ftgen			207,0,giViewTabSize,-19,8,1*0.5,0,1*0.5										; Comb 1
giV8				ftgen			208,0,giViewTabSize,-19,16,1*0.5,0,1*0.5									; Comb 2
giV9				ftgen			209,0,giViewTabSize,-7,0.9,abs(giViewTabSize),1			; Spring
giV9				ftgen			210,0,giViewTabSize,-7,1,abs(giViewTabSize),1			; Flat

instr	1
 ; read in widgets
 kFFTindex		chnget				"FFTindex"
 kFFTindex		init				1
 kAmpTable		chnget				"AmpTable"
 kAmpTable		init				1
 kFrqTable		chnget				"FrqTable"
 kFrqTable		init				1
 kMaxDelay		chnget				"MaxDelay"
 kDryWetMix		chnget				"DryWetMix"
 kFeedback		chnget				"Feedback"
 kLevel			chnget				"Level"

 iFFTsizes[]	fillarray	64,128,256,512,1024,2048	; array of FFT size values

 aL,aR				ins
 
 ; if changed reinitialise mechanism
 if changed(kFFTindex,kAmpTable,kFrqTable,kMaxDelay)==1 then	; if any of the variables in the brackets change...
  reinit	RESTART													; begin a reinitialisation pass from the label
 endif
 RESTART:															; a label
 
 iFFT				=				iFFTsizes[i(kFFTindex)-1]		; retrieve FFT size value from array
 iMaxDelay			limit			i(kMaxDelay), iFFT/sr,8			; max.delay time must be i-rate

 iftamps1			ftgen			1,0,iFFT,-7,iMaxDelay,iFFT,0							; Hi to Lo
 iftamps2			ftgen			2,0,iFFT,-7,0,iFFT,iMaxDelay							; Lo to Hi
 iftamps3			ftgen			3,0,iFFT,-21,1,iMaxDelay								; Random
 iftamps4			ftgen			4,0,iFFT,-16,0,iFFT*0.25,-4,iMaxDelay,iFFT*0.75,4,0		; Peak 1
 iftamps5			ftgen			5,0,iFFT,-16,0,iFFT*0.125,-4,iMaxDelay,iFFT*0.75,4,0	; Peak 2
 iftamps6			ftgen			6,0,iFFT,-16,0,iFFT*0.0125,-4,iMaxDelay,iFFT*0.075,4,0	; Peak 3
 iftamps7			ftgen			7,0,iFFT,-19,8,iMaxDelay*0.5,0,iMaxDelay*0.5			; Comb 1
 iftamps8			ftgen			8,0,iFFT,-19,16,iMaxDelay*0.5,0,iMaxDelay*0.5			; Comb 2
 iftamps9			ftgen			9,0,iFFT,-7,iMaxDelay-(iMaxDelay*0.1),iFFT,iMaxDelay	; Spring
 iftamps10			ftgen			10,0,iFFT,-7,iFFT,iFFT,iFFT								; Flat
 
 iftfrqs1			ftgen			51,0,iFFT,-7,iMaxDelay,iFFT,0							; Hi to Lo		
 iftfrqs2			ftgen			52,0,iFFT,-7,0,iFFT,iMaxDelay							; Lo to hi
 iftfrqs3			ftgen			53,0,iFFT,-21,1,iMaxDelay								; Random
 iftfrqs4			ftgen			54,0,iFFT,-16,0,iFFT*0.25,-4,iMaxDelay,iFFT*0.75,4,0	; Peak 1
 iftfrqs5			ftgen			55,0,iFFT,-16,0,iFFT*0.125,-4,iMaxDelay,iFFT*0.75,4,0	; Peak 2
 iftfrqs6			ftgen			56,0,iFFT,-16,0,iFFT*0.0125,-4,iMaxDelay,iFFT*0.075,4,0	; Peak 3
 iftfrqs7			ftgen			57,0,iFFT,-19,8,iMaxDelay*0.5,0,iMaxDelay*0.5			; Comb 1
 iftfrqs8			ftgen			58,0,iFFT,-19,16,iMaxDelay*0.5,0,iMaxDelay*0.5			; Comb 2
 iftfrqs9			ftgen			59,0,iFFT,-7,iMaxDelay-(iMaxDelay*0.1),iFFT,iMaxDelay	; Spring
 iftfrqs10			ftgen			60,0,iFFT,-7,iFFT,iFFT,iFFT								; Flat

 					tablecopy		101,200+i(kAmpTable)
 					tablecopy		102,200+i(kFrqTable)
 					chnset			"tablenumber(101)","AmpTableID"
 					chnset			"tablenumber(102)","FrqTableID"
 					
 fsig_outL			pvsinit			iFFT,iFFT/4,iFFT,1
 fsig_inL			pvsanal			aL,iFFT,iFFT/4,iFFT,1									; analyse signal
 fsig_FBL			pvsgain			fsig_outL,kFeedback
 fsig_mixL			pvsmix			fsig_inL,fsig_FBL
 ihandleL, ktimeL	pvsbuffer		fsig_mixL, iMaxDelay									; write into PV buffer
 fsig_outL 			pvsbufread2		ktimeL, ihandleL, i(kAmpTable), 50+i(kFrqTable) 		; read from buffer (with delays)
 awetL				pvsynth			fsig_outL												; resynthesise
 amixL				ntrpol			aL,awetL,kDryWetMix										; dry/wet mix

 fsig_outR			pvsinit			iFFT,iFFT/4,iFFT,1
 fsig_inR			pvsanal			aL,iFFT,iFFT/4,iFFT,1									; analyse signal
 fsig_FBR			pvsgain			fsig_outR,kFeedback
 fsig_mixR			pvsmix			fsig_inR,fsig_FBR
 ihandleR, ktimeR	pvsbuffer		fsig_mixR, iMaxDelay									; write into PV buffer
 fsig_outR 			pvsbufread2		ktimeR, ihandleR, i(kAmpTable), 50+i(kFrqTable) 		; read from buffer (with delays)
 awetR				pvsynth			fsig_outR												; resynthesise
 amixR				ntrpol			aR,awetR,kDryWetMix										; dry/wet mix
 
 					outs			amixL*kLevel, amixR*kLevel
endin

</CsInstruments>

<CsScore>
i 1 0 -1
f 0 3700
</CsScore>

</CsoundSynthesizer>
; StuckBuffer.csd
; Written by Iain McCurdy 2014

; Imitates the sound of a audio stuck buffer.

; CONTROLS
; Thresh.	-	probability of there being a stuck buffer
; Freq.Min	-	Rate of probability test (minimum)
; Freq.Max	-	Rate of probability test (maximum)
;			Threshold setting will also affect the frequency with which new buffer sizes are generated. i.e. a threshold setting of '1' will mean that a stuck buffer will be held indefinitely.
; Hold (button)	-	Hold the current buffer indefinitely
; Size.Min.	-	Random Buffer Size (minimum)
; Size.Max.	-	Random Buffer Size (maximum) - (these values raised to the power of 2 give the actual buffer size in samples)
; Intergerise (button)	- round generated buffer size values to the nearest integer. (Fractional offset of Size.Min is retained)
;			This option also integerises transposition values (see below) 
; Trans.Min.	-	Random Transpose (minimum) in octaves 
; Trans.Max.	-	Random Transpose (maximum) in octaves. Transposition is achieved by playing a stuck buffer faster or slower
; Quality (combobox)	-	interpolation used in playing back the buffer: none/linear/cubic. 
;			This will affect the quality of playback when transposed. Lower quality may be desired for 'Lo Fi' effects.
; Width		-	width of random panning of stuck buffers

<Cabbage>
#define RSliderStyle # colour(150,150,150), trackercolour(white), outlinecolour(85,85,85)#

form caption("Stuck Buffer") size(600,110), colour(50,50,50), pluginid("StBu") style("legacy")

image pos(0, 0), size(600, 110), colour(50,50,50), shape("sharp"), outlinecolour("grey"), outlinethickness(1)

line     bounds( 10, 10,160, 2), colour("grey")
label    bounds( 50,  5, 80, 12), text("PROBABILITY"), colour(50,50,50)
rslider  bounds(  0, 20, 60, 60), text("Thresh."),   channel("thresh"),  range(0, 1.00, 0.4), $RSliderStyle
checkbox bounds( 10, 85,100, 12), text("HOLD"), colour("lime"), channel("hold"),  value(0)
rslider  bounds( 60, 20, 60, 60), text("Freq.Min."), channel("FreqMin"), range(0.1,100.00, 2, 0.5), $RSliderStyle
rslider  bounds(120, 20, 60, 60), text("Freq.Max."), channel("FreqMax"), range(0.1,100.00, 8, 0.5), $RSliderStyle

button   bounds( 95, 84, 50, 16), text("STUCK","STUCK"), active(0), channel("STUCK"), fontcolour:0(40,40,40), fontcolour:1(255,200,200), colour:1(255,80,80)

line     bounds(190, 10,100,  2), colour("grey")
label    bounds(203	,  5, 75, 12), text("BUFFER SIZE"), colour(50,50,50)
rslider  bounds(180, 20, 60, 60), text("Size.Min."), channel("SizeMin"), range(2.00, 16, 10), $RSliderStyle
rslider  bounds(240, 20, 60, 60), text("Size.Max."), channel("SizeMax"), range(2.00, 16, 14), $RSliderStyle

line     bounds(310, 10,100,  2), colour("grey")
label    bounds(325,  5, 70, 12), text("TRANSPOSE"), colour(50,50,50)
rslider  bounds(300, 20, 60, 60), text("Trans.Min."), channel("TransMin"), range(-7.00, 7, 0), $RSliderStyle
rslider  bounds(360, 20, 60, 60), text("Trans.Max."), channel("TransMax"), range(-7.00, 7, 0), $RSliderStyle
combobox bounds(315, 82, 90, 18	), channel("interp"), value(2), text("No Interp.", "Linear", "Cubic")

line     bounds(430, 10,160,  2), colour("grey")
label    bounds(484,  5, 50, 12), text("OUTPUT"), colour(50,50,50)
rslider  bounds(420,20, 60, 60), text("Width"), channel("PanWidth"), range(0, 1.00, 0.5), $RSliderStyle
rslider  bounds(480,20, 60, 60), text("Mix"), channel("mix"), range(0, 1.00, 0.85), $RSliderStyle
rslider  bounds(540,20, 60, 60), text("Level"), channel("level"), range(0, 1.00, 1), $RSliderStyle

checkbox bounds(190, 85,100, 12), text("INTEGERISE"), colour("lime"), channel("integer"),  value(0)

</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-n -+rtmidi=NULL -M0
;-odac -dm0
</CsOptions>

<CsInstruments>

sr = 44100
ksmps = 32
nchnls = 2
0dbfs=1

gibufferL	ftgen	0,0,131072,2,0	; buffer tables
gibufferR	ftgen	0,0,131072,2,0


instr	1

 ;aInL,aInR	diskin	"ClassicalGuitar.wav",1,0,1	; for testing
 ;aL	diskin	"MFEM.wav",1,0,1	; for testing
 ;aR	=	aL
 aInL,aInR	ins

 kporttime	linseg	0,0.01,0.05	; portamento time
 kthresh	chnget	"thresh"
 khold		chnget	"hold"
 kthresh	+=	khold		; add hold value to threshold value
 kFreqMin	chnget	"FreqMin"
 kFreqMax	chnget	"FreqMax"
 kSizeMin	chnget	"SizeMin"
 kSizeMax	chnget	"SizeMax"
 kinteger	chnget	"integer"
 kTransMin	chnget	"TransMin"
 kTransMax	chnget	"TransMax"
 kinterp	chnget	"interp"
 kPanWidth	chnget	"PanWidth"
 kmix		chnget	"mix"
 klevel		chnget	"level"

 kwet		limit	kmix*2,0,1
 kdry		limit	2-(kmix*2),0,1
 
 kSizeMin	portk	kSizeMin,kporttime		; smooth changes to widgets
 kSizeMax	portk   kSizeMax,kporttime	
 kTransMin	portk	kTransMin,kporttime		
 kTransMax	portk   kTransMax,kporttime	

 krate		randomh kFreqMin,kFreqMax,1	; rate of generation of random values
 kTestVal	randomh	0,1,krate		; generate a random value for testing
 
 ktrig	trigger	kTestVal,kthresh,1		; if we are exiting a stuck buffer event, generate a trigger (i.e. new buffer sizes are only generated when a stuck buffer becomes unstuck)
 ksize	trandom	ktrig,kSizeMin,kSizeMax		; generate a new buffer size when a stuck buffer unsticks

 kpan	trandom	ktrig,0.5-(kPanWidth*0.5),0.5+(kPanWidth*0.5)	; generate a random pan value when triggered
 ktrans	trandom	ktrig,kTransMin,kTransMax			; generate a random transposition value when triggered

 if kinteger==1 then				; if 'INTEGERISE' is on...
  kfrac	=	frac(kSizeMin)			; remember fractional value of Size.Min. 
  ksize	=	int(ksize)+kfrac
  ktrans	=	int(ktrans)
 endif

 ksize	limit	ksize,kSizeMin,kSizeMax		; limit buffer size value
 if kinterp==1 then
  ksize	=	int(2 ^ ksize)			; create power of two value (integer)
 else
  ksize	=	2 ^ ksize			; create power of two value
 endif
 
 ktrans	limit	ktrans,kTransMin,kTransMax	; limit transposition
 
 ; write to buffer
 
 if kTestVal>=kthresh then			; normal
  kPhsFrq	=	sr/ksize
 else
  kPhsFrq	=	(sr*octave(ktrans))/ksize
 endif 
 
 aphsr	phasor	kPhsFrq			; create a moving phasor
 aphsr	=	aphsr * ksize		; rescale scope of phasor according to buffer size
 
 if kTestVal>=kthresh then		; normal
  	tablew	aInL,aphsr,gibufferL	; write to buffer
  	tablew	aInR,aphsr,gibufferR
 else					; stuck buffer
  if kinterp==1 then			; choose interpolation method:
   aL	table	aphsr,gibufferL		; none
   aR	table	aphsr,gibufferR
  elseif kinterp==2 then		; linear
   aL	tablei	aphsr,gibufferL
   aR	tablei	aphsr,gibufferR
  else					; cubic
   aL	table3	aphsr,gibufferL
   aR	table3	aphsr,gibufferR
  endif
  aL	=	aL * kpan * kwet	; pan audio
  aR	=	aR * (1-kpan) * kwet
 endif

  aL	sum	aL*kwet, aInL*kdry
  aR	sum	aR*kwet, aInR*kdry
 
 	outs	aL*klevel, aR*klevel
  aL	=	0
  aR	=	0

  ; toggle GUI "STUCK" indicator
  if trigger:k(kTestVal-khold,kthresh,0)==1 then
   chnset	k(0),"STUCK"
  elseif  trigger:k(kTestVal+khold,kthresh,1)==1 then
   chnset	k(1),"STUCK"
  endif
endin

</CsInstruments>  

<CsScore>
i 1 0 3600
e
</CsScore>

</CsoundSynthesizer>
; TempoDelay.csd
; Written by Iain McCurdy, 2012.

; Units for the delay are assumed to be demi-semiquavers.
; Knob for Rhy.Mult. will be replaced with a combobox once comboboxes work in plugins within hosts.
; Width control only applicable when ping-pong delay selected.
; If 'external' is selected as clock source tempo is taken from the host's BPM. 

<Cabbage>
form caption("Tempo Delay") size(565, 90), pluginid("TpDl") style("legacy")
image pos(0, 0), size(565, 90), colour("LightBlue"), shape("rounded"), outlinecolour("white"), outlinethickness(4) 
rslider bounds(10, 11, 70, 70), text("Tempo"), 		textcolour("black"), 		channel("tempo"), 	range(40, 500, 90, 1, 1),   colour(100,100,255),trackercolour(100,100,150)
rslider bounds(75, 11, 70, 70), text("Rhy.Mult."),	textcolour("black"), 		channel("RhyMlt"), 	range(1, 16, 4, 1, 1),      colour(100,100,255),trackercolour(100,100,150)
rslider bounds(140, 11, 70, 70), text("Damping"), 	textcolour("black"), 		channel("damp"), 	range(20,20000, 20000,0.5), colour(100,100,255),trackercolour(100,100,150)
rslider bounds(205, 11, 70, 70), text("Feedback"), 	textcolour("black"), 		channel("fback"), 	range(0, 1.00, 0.8),        colour(100,100,255),trackercolour(100,100,150)
rslider bounds(270, 11, 70, 70), text("Width"),	textcolour("black"), 			channel("width"), 	range(0,  1.00, 1),         colour(100,100,255),trackercolour(100,100,150)
label  bounds(342,  10, 75, 12), text("Clock Source"), FontColour("black")
button bounds(340,  22, 80, 20), text("Internal","External"), channel("ClockSource"), value(0), fontcolour:0("yellow"), fontcolour:1("yellow")
label  bounds(345,  45, 70, 12), text("Delay Type"), FontColour("black")
button bounds(340,  57, 80, 20), text("Simple","Ping-pong"), channel("DelType"), value(1), fontcolour:0("yellow"), fontcolour:1("yellow")
rslider bounds(420, 11, 70, 70), text("Mix"), 		textcolour("black"), 		channel("mix"), 	range(0, 1.00, 0.5), colour(100,100,255),trackercolour(100,100,150)
rslider bounds(485, 11, 70, 70), text("Level"),		textcolour("black"), 		channel("level"), 	range(0, 1.00, 1),   colour(100,100,255),trackercolour(100,100,150)
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

instr	1
	kfback		chnget	"fback"				;read in widgets
	kdamp		chnget	"damp"				;
	kmix		chnget	"mix"				;
	klevel		chnget	"level"				;
	kbpm		chnget	"HOST_BPM"			;
	kRhyMlt		chnget	"RhyMlt"			;
	kClockSource	chnget	"ClockSource"			;
	kDelType	chnget	"DelType"			;
	kwidth		chnget	"width"				;
	if kClockSource==0 then				;if internal clock source has been chosen...
	 ktempo	chnget	"tempo"				;tempo taken from GUI knob control
	else
	 ktempo	chnget	"bpm"				;tempo taken from host BPM
	 ktempo	limit	ktempo,40,500			;limit range of possible tempo values. i.e. a tempo of zero would result in a delay time of infinity.
	endif

	ktime	divz	(60*kRhyMlt),(ktempo*8),0.1		;derive delay time. 8 in the denominator indicates that kRhyMult will be in demisemiquaver divisions
	atime	interp	ktime				;interpolate k-rate delay time to create an a-rate version which will give smoother results when tempo is modulated
	
	ainL,ainR	ins				;read stereo inputs
	
	if kDelType==0 then				;if 'simple' delay type is chosen...
	 abuf	delayr	5
	 atapL	deltap3	atime
	 atapL	tone	atapL,kdamp
		delayw	ainL+(atapL*kfback)

	 abuf	delayr	5
	 atapR	deltap3	atime
	 atapR	tone	atapR,kdamp
		delayw	ainR+(atapR*kfback)	
	else						;otherwise 'ping-pong' delay type must have been chosen
	 ;offset delay (no feedback)
	 abuf	delayr	5
	 afirst	deltap3	atime
	 afirst	tone	afirst,kdamp
		delayw	ainL

	 ;left channel delay (note that 'atime' is doubled) 
	 abuf	delayr	10			;
	 atapL	deltap3	atime*2
	 atapL	tone	atapL,kdamp
		delayw	afirst+(atapL*kfback)

	 ;right channel delay (note that 'atime' is doubled) 
	 abuf	delayr	10
	 atapR	deltap3	atime*2
	 atapR	tone	atapR,kdamp;		delayw	ainR+(atapR*kfback)
		delayw	ainR+(atapR*kfback)
	
	 ;create width control. note that if width is zero the result is the same as 'simple' mode
	 atapL	=	afirst+atapL+(atapR*(1-kwidth))
	 atapR	=	atapR+(atapL*(1-kwidth))

	endif
	
	amixL		ntrpol		ainL, atapL, kmix	;CREATE A DRY/WET MIX BETWEEN THE DRY AND THE EFFECT SIGNAL
	amixR		ntrpol		ainR, atapR, kmix	;CREATE A DRY/WET MIX BETWEEN THE DRY AND THE EFFECT SIGNAL
			outs		amixL * klevel, amixR * klevel
endin
		
</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
</CsScore>

</CsoundSynthesizer>
; TimeSlip.csd
; Written by Iain McCurdy, 2015

; Implements realtime time stretching by buffering the realtime audio stream and then triggering event grains reading from the buffer.

; Time Stretch	-	amount of time stretching.
;			e.g. 
;			 1 = no time stretching
;			 2 = half speed
;			 4 = quarter speed etc...
;			 As soon as 'Time Stretch' returns to '1' a quick crossfade is made back to the live audio stream.
; Freeze	-	Activating this button if time stretching is already in progress will freeze time stretching completely (time stretch = infinity)
; 			 Sliding Time Stretch all the way to the right will also trigger the freeze button. 
;			 Density and grain size will be unaffected. 

; Overlaps	-	Number of grains allowed to overlaps simultaneously. Normally set to 2.
; Density	-	Initial density when time stretching begins. 
;			Density also depends upon time stretching factor 
;			Density reduces as stretch increases while grain duration increases, therefore overlaps remains constant.
; Rand.When	-	Adds a random factor to when grains will start. This can be used to reduce or prevent artefacts produced through strict periodic production of grains.
; Rand.Where	-	Adds a random factor to from where in the buffer grains will be read. This can be used to reduce or prevent artefacts produced through strict periodic production of grains.
; Wet		-	Level control of the wet (time stretching) sound. 
;			 This should be adjusted to set a good balance between the dry sound (time stretch=1) and when the time stretching begins (time stretch>1)
;			 Higher densities will result in higher amplitudes when stretching.

<Cabbage>
form caption("Time Slip"), size(700,210), pluginid("TmSl") style("legacy")
image                bounds(0,0,700,210), colour( 40, 40, 60), shape("sharp")
groupbox bounds(  0,  0,700, 90), plant("SlowSlider"), text("Time Ratio"), colour(0,0,0,10) {
hslider  bounds(  5, 15,690, 60), range(1,20,1,0.5,0.0001), channel("stretch"), trackercolour(140,140,160)
label    bounds(  5, 65,690, 14), text("Time Stretch")
button   bounds(  5, 65, 60, 18), fontcolour:0(50,50,50), fontcolour:1(205,255,205), colour:0(0,10,0), colour:1(150,250,150), text("Realtime","Realtime"),channel("realtime")
button   bounds(630, 65, 60, 18), fontcolour:0(50,50,50), fontcolour:1(205,205,255), colour:0(0,0,10), colour:1(150,150,250), text("Freeze","Freeze"), channel("freeze")
}

button   bounds( 60,145,90,18), fontcolour:0(50,50,50), fontcolour:1(255,205,205), colour:0(10,0,0), colour:1(250,150,150), text("Grain Scale","Grain Scale"), channel("GrainScale"), latched(1), value(1)
rslider  bounds(170,100,80,95), range(1,50,4,1,1),         channel("overlaps"), valuetextbox(1), textbox(1), trackercolour(140,140,160), text("Overlaps")
rslider  bounds(250,100,80,95), range(1,50,8,1,1),         channel("dens"),     valuetextbox(1), textbox(1), trackercolour(140,140,160), text("Density")
rslider  bounds(330,100,80,95), range(0,1,0.05,0.5,0.001), channel("RndWhen"),  valuetextbox(1), textbox(1), trackercolour(140,140,160), text("Rand.When")
rslider  bounds(410,100,80,95), range(0,1,0.05,0.5,0.001), channel("RndWhere"), valuetextbox(1), textbox(1), trackercolour(140,140,160), text("Rand.Where")
rslider  bounds(490,100,80,95), range(0,1,0.5),            channel("wet"),      valuetextbox(1), textbox(1), trackercolour(140,140,160), text("Wet")
rslider  bounds(570,100,80,95), range(0,1,1),              channel("level"),    valuetextbox(1), textbox(1), trackercolour(140,140,160), text("Level")

</Cabbage>
                    
<CsoundSynthesizer>

<CsOptions>   
-dm0 -n
</CsOptions>

<CsInstruments>

sr 		= 	44100	; SAMPLE RATE
ksmps 		= 	32	; NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls 		= 	2	; NUMBER OF CHANNELS (1=MONO)
0dbfs		=	1	; MAXIMUM AMPLITUDE
			
giBuffL	ftgen	1,0,(2^24)+1,2,0
giBuffR	ftgen	2,0,(2^24)+1,2,0
gihanning	ftgen	0,  0, 131073,  20,   2, 1					; HANNING WINDOW

instr	1
 kporttime	linseg	0,0.01,0.1
 gkstretch	chnget	"stretch"
 gkstretchP	portk	gkstretch,kporttime
 gkoverlaps	chnget	"overlaps"
 gkdens		chnget	"dens"
 gkRndWhen	chnget	"RndWhen"
 gkRndWhere	chnget	"RndWhere"
 gkwet		chnget	"wet"
 gklevel	chnget	"level"
 gkfreeze	chnget	"freeze"
 gkrealtime	chnget	"realtime"
 gkGrainScale	chnget	"GrainScale"
 gkfreeze	chnget	"freeze"
  
 gaInL,gaInR	ins							; READ REALTIME AUDIO INPUT
 
 gaWPhasor	phasor	sr/ftlen(giBuffL)		; WRITE PHASOR
 kWPhasor	downsamp	gaWPhasor			; K RATE VERSION OF WRITE PHASOR
 		tablew	gaInL,gaWPhasor,giBuffL,1	; WRITE STEREO AUDIO TO TABLES
 		tablew	gaInR,gaWPhasor,giBuffR,1	;
  
 if	gkstretch==1&&active:k(2)==0 then		; IF TIME STRETCH SLIDER IS AT '1' AND 'REALTIME' MODE IS NOT YET ACTIVE, ACTIVATE IT
  event	"i",2,0,-1
  kOn	=	1								; TURN ON REALTIME BUTTON
 	chnset	kOn,"realtime"
 elseif	gkstretch>1&&active:k(3)==0 then	; IF TIME STRETCH SLIDER IS INCREASED BEYOND '1' AND 'TIME STRETCH' MODE IS NOT YET ACTIVE, ACTIVATE IT
  kOff	=	0
 	chnset	kOff,"realtime"					; TURN OFF REALTIME BUTTON
  event	"i",3,0,-1,kWPhasor					; TURN ON TIME STRETCH INSTRUMENT AND SEND IT CURRENT WRITE PHASOR POSITION
 endif
 
endin

instr	2	; NORMAL PLAYBACK
 if gkstretch>1 then						; IF TIME STRETCH SLIDER IS INCREASED BEYOND '1' (LEFT-MOST) TURN OFF THIS NOTE
  turnoff
 endif
 aenv	linsegr	0,0.1,1,4/i(gkdens),0				; RELEASE ENVELOPE
 	outs	gaInL*aenv*gklevel, gaInR*aenv*gklevel
endin

instr	3	; SLOWED PLAYBACK (TRIGGER SOUND GRAINS)
 gkRPhasor	phasor	(sr*(1-gkfreeze))/(ftlen(1)*gkstretchP),p4	; READ START POINT PHASOR
 
 if gkstretch==1 then				; IS TIME STRETCH SLIDER IS RETURNED TO '1' (LEFT-MOST), DEACTIVATE FREEZE BUTTON
  kOff	=	0
  	chnset	kOff,"freeze"
  turnoff
 endif
 
 if gkstretch==20&&gkfreeze==0 then		; TURN ON FREEZE BUTTON IF TIME STRETCH SLIDER IS MOVED ALL THE WAY TO THE RIGHT 
  kOn	=	1
  	chnset	kOn,"freeze"
 elseif trigger:k(gkstretch,19.999,1)==1 then
  	chnset	kOff,"freeze"	
 endif

 if trigger:k(gkrealtime,0.5,0)==1&&gkstretch>1 then	; IF REALTIME BUTTON IS ACTIVATED RETURN TIME STRETCH SLIDER TO '1' (LEFT-MOST)
  kReset	=	1
  		chnset	kReset,"stretch"
 endif

 ksize	 	=	gkGrainScale==1 ? (2*gkstretchP)/gkdens          : 1/gkdens
 kdens		=	gkGrainScale==1 ? (gkdens*gkoverlaps)/gkstretchP : (gkdens*gkoverlaps)/4
 
 ktrig	metro	kdens						; METRONOME TO TRIGGER GRAINS
 schedkwhen	ktrig,0,0,4,random:k(0,gkRndWhen),ksize 	; TRIGGER GRAINS
endin

instr	4	; SOUND GRAINS
 aenv	poscil	1,1/p3,gihanning
 aPtr	line	0,p3,p3*sr
 aPtr	+=	(i(gkRPhasor)*ftlen(giBuffL)) + (random:i(-i(gkRndWhere),0) * sr)
 aSigL	tablei	aPtr,giBuffL
 aSigR	tablei	aPtr,giBuffR

 if gkstretch==1 then			; RELEASE ENVELOPE - USED IF GRAINS ARE INTERRUPTED BY THE TIME STRETCH SLIDER IS RETURNED TO '1'/REALTIME EITHER BY DRAGGING THE SLIDER OR BY ACTIVATING THE REALTIME BUTTON
  arel	linsegr	1,0.2,0
  aSigL	*=	arel
  aSigR	*=	arel
  	turnoff
 endif

 	outs	aSigL*aenv*gklevel*gkwet, aSigR*aenv*gklevel*gkwet
endin

</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
</CsScore>

</CsoundSynthesizer>
; TriggerDelay.csd
; Written by Iain McCurdy, 2012.

; This example works best with sharp percussive sounds

; A trigger impulse is generated each time the rms of the input signal crosses the defined 'Threshold' value.
; Each time a new trigger is generated a new random delay time (between user definable limits)
; - and a new random feedback value (again between user definable limits) are generated.

; It is possible to generate feedback values of 1 and greater (which can lead to a continuous build-up of sound)
;  this is included intentionally and the increasing sound will be clipped and can be filtered by reducing the 'Damping' control to produce increasing distortion of the sound within the delay buffer as it repeats 

; Increasing the 'Portamento' control damps the otherwise abrupt changes in delay time.

; 'Width' allows the user to vary the delay from a simple monophonic delay to a ping-pong style delay

<Cabbage>
form caption("Trigger Delay") size(540,240), pluginid("TrDl") style("legacy")
image                  bounds(0, 0,540,240), colour(150,150,205), shape("rounded"), outlinecolour("white"), outlinethickness(4) 

rslider  bounds(  5, 11, 70, 70),  text("Threshold"), textcolour("black"),  channel("threshold"), range(0, 1.00, 0.1, 0.5),      colour( 40, 40, 95),trackercolour("white")
                     
line     bounds( 83, 10, 95, 3), colour("Grey")
label    bounds(100,  6, 60, 10), text("DELAY TIME"), fontcolour("black"), colour(150,150,205)

rslider bounds( 72, 18, 63, 63),  text("Min."),	    textcolour("black"),  channel("dly1"), 	range(0.0001, 2, 0.001,0.5), colour( 40, 40, 95),trackercolour("white")
rslider bounds(130, 18, 63, 63), text("Max."), 	    textcolour("black"),  channel("dly2"), 	range(0.0001, 2, 0.1, 0.5),  colour( 40, 40, 95),trackercolour("white")

line     bounds(202, 10,  95,  3), colour("Grey")
label    bounds(222,  6,  55, 10), text("FEEDBACK"), fontcolour("black"), colour(150,150,205)

rslider bounds(190, 18, 63, 63), text("Min."),      textcolour("black"),  channel("fback1"), 	range(0, 1.200, 0.5),        colour( 40, 40, 95),trackercolour("white")
rslider bounds(248, 18, 63, 63), text("Max."),      textcolour("black"),  channel("fback2"), 	range(0, 1.200, 0.9),        colour( 40, 40, 95),trackercolour("white")

rslider bounds(  5, 81, 70, 70), text("Portamento"), textcolour("black"),  channel("porttime"), 	range(0,  5.00, 0,0.5),         colour( 40, 40, 95),trackercolour("white")
rslider bounds( 65, 81, 70, 70), text("Cutoff"),     textcolour("black"),  channel("cf"), 	range(50,10000, 5000,0.5), colour( 40, 40, 95),trackercolour("white")
rslider bounds(125, 81, 70, 70), text("Bandwidth"),  textcolour("black"),  channel("bw"), 	range(600,22050, 4000,0.5), colour( 40, 40, 95),trackercolour("white")

rslider bounds(  5,151, 70, 70), text("Width"),	    textcolour("black"),  channel("width"), 	range(0,  1.00, 1),         colour( 40, 40, 95),trackercolour("white")
rslider bounds( 65,151, 70, 70), text("Mix"), 	    textcolour("black"),  channel("mix"), 	range(0, 1.00, 0.5),        colour( 40, 40, 95),trackercolour("white")
rslider bounds(125,151, 70, 70), text("Level"),	    textcolour("black"),  channel("level"), 	range(0, 1.00, 1),          colour( 40, 40, 95),trackercolour("white")


xypad bounds(315, 5, 210, 230), channel("cf", "bw"), rangex(50, 10000, 5000), rangey(600, 22050, 4000), text("CF/BW")
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

instr	1
	kthreshold	chnget	"threshold"				;read in widgets
	kdly1		chnget	"dly1"				;read in widgets
	kdly2		chnget	"dly2"				;read in widgets
	kfback1		chnget	"fback1"				;read in widgets
	kfback2		chnget	"fback2"				;read in widgets
	kwidth		chnget	"width"				;
	kmix		chnget	"mix"				;
	klevel		chnget	"level"				;
	kporttime	chnget	"porttime"				;
	kcf		chnget	"cf"
	kbw		chnget	"bw"
	ainL,ainR	ins				;read stereo inputs

	krms	rms	(ainL+ainR)*0.5
	
	ktrig	trigger	krms, kthreshold, 0
	
	kdly	trandom	ktrig, 0, 1
	kdly	expcurve	kdly,8
	kMinDly	min	kdly1,kdly2
	kdly	=	(kdly * abs(kdly2 - kdly1) ) + kMinDly
	
	
	kramp	linseg	0,0.001,1
	
	kcf	portk	kcf, kramp * 0.05
	kbw	portk	kbw, kramp * 0.05
	
	kdly	portk	kdly, kporttime*kramp
	atime	interp	kdly

	kfback	trandom	ktrig, kfback1, kfback2
	
	;offset delay (no feedback)
	abuf	delayr	5
	afirst	deltap3	atime
	afirst	butbp	afirst,kcf,kbw
		delayw	ainL

	;left channel delay (note that 'atime' is doubled) 
	abuf	delayr	10			;
	atapL	deltap3	atime*2
	atapL	clip	atapL,0,0.9
	atapL	butbp	atapL,kcf,kbw
		delayw	afirst+(atapL*kfback)

	 ;right channel delay (note that 'atime' is doubled) 
	 abuf	delayr	10
	 atapR	deltap3	atime*2
	 atapR	clip	atapR,0,0.9
	 atapR	butbp	atapR,kcf,kbw
		delayw	ainR+(atapR*kfback)
	
	 ;create width control. note that if width is zero the result is the same as 'simple' mode
	 atapL	=	afirst+atapL+(atapR*(1-kwidth))
	 atapR	=	atapR+(atapL*(1-kwidth))
	
	amixL		ntrpol		ainL, atapL, kmix	;CREATE A DRY/WET MIX BETWEEN THE DRY AND THE EFFECT SIGNAL
	amixR		ntrpol		ainR, atapR, kmix	;CREATE A DRY/WET MIX BETWEEN THE DRY AND THE EFFECT SIGNAL
			outs		amixL * klevel, amixR * klevel
endin
		
</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
</CsScore>

</CsoundSynthesizer>
; waveset.csd

; 'freeze' is not technically a freeze but instead a very large number of repeats.

; Waveset opcode can be reinitialised by three different methods:
; Manually, using the 'Reset' button, 
; by a built-in metronome, the rate of which can be adjusted by the user
; or by the dynamics of the input sound (the threshold of this dynamic triggereing can be adjusted by the user)
; 'Metro' resetting is disabled when 'Metro Rate' = 0
; 'Threshold' (retrigering by input signal dynamics) is disabled when 'Threshold' = 1 (maximum setting)
; (resetting the opcode will reset its internal buffer and cancel out any time displacement induced by wavelet repetitions) 

<Cabbage>
form caption("waveset") size(510, 90), pluginid("wset") style("legacy")
image pos(0, 0), size(510, 90), colour("Green"), shape("rounded"), outlinecolour("Grey"), outlinethickness(4) 
rslider bounds(5, 10, 70, 70),   text("Repeats"), channel("repeats"), range(1, 100, 1, 1, 1),   colour("yellow"), textcolour("white"), trackercolour("white")
rslider bounds(70, 10, 70, 70),  text("Mult."),   channel("mult"),    range(1, 100, 1, 0.5, 1), colour("yellow"), textcolour("white"), trackercolour("white")
checkbox bounds(140, 23, 100, 30),          channel("freeze"), text("Freeze"), value(0), colour("red"), fontcolour("white"), shape("ellipse")
line bounds(220, 2, 3, 86), colour("Grey")
button bounds(235, 15, 45,45), channel("reset"), text("Reset","Reset"), fontcolour:0("grey"), latched(0)
rslider bounds(290, 10, 70, 70), text("Threshold"),  channel("thresh"), range(0, 1.00, 1), colour("orange"), textcolour("white"), trackercolour("white")
rslider bounds(355, 10, 70, 70), text("Metro Rate"), channel("rate"),   range(0, 5.00, 0), colour("orange"), textcolour("white"), trackercolour("white")
line bounds(430, 2, 3, 86), colour("Grey")
rslider bounds(435,  10, 70, 70), text("Level"), channel("level"), range(0, 1.00, 0.7), colour(255,150, 50), textcolour("white"), trackercolour("white")
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

instr	1
	krep		chnget	"repeats"				;READ WIDGETS...
	kmult		chnget	"mult"					;
	klevel		chnget	"level"					;
	kreset		chnget	"reset"					;
	kthresh		chnget	"thresh"				;
	krate		chnget	"rate"					;
	ktrigger	trigger	kreset,0.5,0			;
	kmetro		metro	krate, 0.99
	kfreeze		chnget	"freeze"
	;asigL, asigR	diskin2	"Songpan.wav",1,0,1			;USE SOUND FILE FOR TESTING
	asigL, asigR	ins
	krms		rms	(asigL+asigR)*0.5
	kDynTrig	trigger	krms,kthresh,0

	if (ktrigger+kmetro+kDynTrig)>0 then
	 reinit UPDATE
	endif
	UPDATE:
	aL 		waveset 	asigL,(krep*kmult)+(kfreeze*1000000000),5*60*sr		;PASS THE AUDIO SIGNAL THROUGH waveset OPCODE. Input duration is defined in samples - in this example the expression given equats to a 5 minute buffer
	aR 		waveset 	asigR,(krep*kmult)+(kfreeze*1000000000),5*60*sr		;PASS THE AUDIO SIGNAL THROUGH waveset OPCODE. Input duration is defined in samples - in this example the expression given equats to a 5 minute buffer
	rireturn
			outs		aL*klevel, aR*klevel		;WAVESET OUTPUT ARE SENT TO THE SPEAKERS
endin
		
</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
</CsScore>


</CsoundSynthesizer>



























DiskinFilePlayer.csd

Plays a user selected sound file using diskin2 opcode. 
This file player is best suited for the playback of very long sound files and is less well suited for polyphonic playback.

The sound file can be played back using the Play/Stop button (and the 'Transpose' / 'Speed' buttons to implement pitch/speed change)
 or it can be played back using the MIDI keyboard.

Note that for 'reverse' to be effective either 'loop' needs to be active or inskip needs to be something other than zero

The stretch function works by reducing the speed during silent sections. 
'Threshold' defines the RMS value beneath which audio is regarded as silence.
During 'silent' sections, audio will be muted completely and 'Str.Ratio' will be multipled to the main 'Speed' 

<Cabbage>
form caption("Diskin File Player") size(1025, 310), pluginid("DkPl") style("legacy")
image                    bounds(  0,  0,1025, 310), colour( 70, 30, 30), shape("sharp"), outlinecolour("White"), line(3)	; main panel colouration    

soundfiler           bounds(  5,  5,1015,140), channel("beg","len"), identchannel("filer1"),  colour(0, 255, 255, 255), fontcolour(160, 160, 160, 255)
label bounds(6, 4, 560, 14), text(""), align(left), colour(0,0,0,0), fontcolour(200,200,200), identchannel("stringbox")

image bounds(0,150,1025,160), colour(0,0,0,0), plant("controls"){
filebutton bounds(  5,  5, 80, 25), text("Open File","Open File"), fontcolour("white") channel("filename"), shape("ellipse")
checkbox   bounds(  5, 40, 95, 25), channel("PlayStop"), text("Play/Stop"), colour("lime"), fontcolour("white")
checkbox   bounds(105,  5,100, 15), channel("loop"), text("Loop On/Off"), colour("yellow"), fontcolour("white")
checkbox   bounds(105, 22,100, 15), channel("reverse"), text("Reverse"), colour("yellow"), fontcolour("white")
label      bounds(118, 40, 75, 12), text("Interpolation"), fontcolour("white")
combobox   bounds(105, 53,100, 20), channel("interp"), items("No interp.", "Linear", "Cubic", "Point Sinc"), value(3), fontcolour("white")
rslider    bounds(215,  5, 70, 70), channel("transpose"), range(-48, 24, 0,1,1),            colour( 90, 50, 50), trackercolour("silver"), text("Transpose"), textcolour("white")
rslider    bounds(280,  5, 70, 70), channel("speed"),     range( -4,  4.00, 1),             colour( 90, 50, 50), trackercolour("silver"), text("Speed"),     textcolour("white")
rslider    bounds(345,  5, 70, 70), channel("inskip"),    range(  0,  1.00, 0),             colour( 90, 50, 50), trackercolour("silver"), text("In Skip"),   textcolour("white")
rslider    bounds(410,  5, 70, 70), channel("AttTim"),    range(0, 5, 0, 0.5, 0.001),       colour( 90, 50, 50), trackercolour("silver"), text("Att.Tim"),   textcolour("white")
rslider    bounds(475,  5, 70, 70), channel("RelTim"),    range(0.01, 5, 0.05, 0.5, 0.001), colour( 90, 50, 50), trackercolour("silver"), text("Rel.Tim"),   textcolour("white")
rslider    bounds(540,  5, 70, 70), channel("MidiRef"),   range(0,127,60, 1, 1),            colour( 90, 50, 50), trackercolour("silver"), text("MIDI Ref."), textcolour("white")
rslider    bounds(605,  5, 70, 70), channel("level"),     range(  0,  3.00, 1, 0.5),        colour( 90, 50, 50), trackercolour("silver"), text("Level"),     textcolour("white")
}

image      bounds(675,155,140, 65), colour(0,0,0,0), outlinecolour("silver"), outlinethickness(1), shape("sharp"), plant("stretch") {
checkbox   bounds(  5,  5, 80, 15), channel("StretchOnOff"), text("Stretch"), fontcolour("white")
nslider  bounds(  5, 25, 60, 30), channel("threshold"), range(0,1,0.005,1,0.001),  colour( 90, 50, 50), text("Threshold"), textcolour("white")
nslider  bounds( 75, 25, 60, 30), channel("stretchratio"), range(0.01,8.00,0.25,1,0.01),  colour( 90, 50, 50), trackercolour("silver"), text("Str.Ratio"), textcolour("white")
}

image bounds(820, 155,200, 65), colour(0,0,0,0), outlinecolour("silver"), outlinethickness(1), shape("sharp"), plant("speedmod") {
label      bounds(  0,  5,200, 12), text("Speed Modulation"), fontcolour("white")
nslider  bounds(  5, 25, 60, 30), channel("ModRange"), range(0,4,0,1,0.001),  colour( 90, 50, 50), text("Mod.Range"), textcolour("white")
nslider  bounds( 70, 25, 60, 30), channel("Rate1"), range(0,30,1,1,0.001),  colour( 90, 50, 50), text("Rate 1"), textcolour("white")
nslider  bounds(135, 25, 60, 30), channel("Rate2"), range(0,30,2,1,0.001),  colour( 90, 50, 50), text("Rate 2"), textcolour("white")
}

keyboard bounds( 5,230,1015, 75)


</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-n -+rtmidi=NULL -M0
</CsOptions>

<CsInstruments>

sr = 44100
ksmps = 64
nchnls = 2
0dbfs=1

massign	0,3	; all midi notes on all channels sent to instrument 3

giInterpArr[] array 1, 2, 4, 8
gSfilepath	init	""


opcode FileNameFromPath,S,S		; Extract a file name (as a string) from a full path (also as a string)
 Ssrc	xin				; Read in the file path string
 icnt	strlen	Ssrc			; Get the length of the file path string
 LOOP:					; Loop back to here when checking for a backslash
 iasc	strchar Ssrc, icnt		; Read ascii value of current letter for checking
 if iasc==92 igoto ESCAPE		; If it is a backslash, escape from loop
 loop_gt	icnt,1,0,LOOP		; Loop back and decrement counter which is also used as an index into the string
 ESCAPE:				; Escape point once the backslash has been found
 Sname	strsub Ssrc, icnt+1, -1		; Create a new string of just the file name
	xout	Sname			; Send it back to the caller instrument
endop



instr	1
 gkPlayStop	chnget	"PlayStop"		; read in widgets
 gkloop		chnget	"loop"
 gktranspose	chnget	"transpose"
 gkspeed	chnget	"speed"
 gkinterp	chnget	"interp"
 gkreverse	chnget	"reverse"
 gklevel	chnget	"level"
 gSfilepath	chnget	"filename"		; read in file path string from filebutton widget
 gkStretchOnOff	chnget	"StretchOnOff"
 gkstretchratio	chnget	"stretchratio"
 gkthreshold	chnget	"threshold"
 gkModRange	chnget	"ModRange"
 gkRate1	chnget	"Rate1"
 gkRate2	chnget	"Rate2"
 
 if changed:k(gSfilepath)==1 then		; call instrument to update waveform viewer  
  event "i",99,0,0
 endif
 
 ktrig		trigger	gkPlayStop,0.5,0	; if play/stop button toggles from low (0) to high (1) generate a '1' trigger
 schedkwhen	ktrig,0,0,2,0,-1		; start instrument 2
 
 ktrig1	changed	gktranspose			; if 'transpose' button is changed generate a '1' trigger
 ktrig2	changed	gkspeed				; if 'speed' button is changed generate a '1' trigger
 
 if ktrig1==1 then				; if transpose control has been changed...
  chnset	semitone(gktranspose),"speed"	; set speed according to transpose value
 elseif ktrig2==1&&gkspeed>=0 then		; if speed control has been changed...
  chnset	log2(gkspeed)*12,"transpose"	; set transpose control according to speed value
 endif
endin

instr	2

 if gkPlayStop==0 then				; if play/stop is off (stop)...
  turnoff					; turn off this instrument
 endif						
 iStrLen 	strlen gSfilepath		; derive string length
 if iStrLen > 0 then				; if string length is greater than zero (i.e. a file has been selected) then...
  iAttTim	chnget	"AttTim"		; read in amplitude envelope attack time widget
  iRelTim	chnget	"RelTim"		; read in amplitude envelope attack time widget
  if iAttTim>0 then				; 
   kenv	linsegr	0,iAttTim,1,iRelTim,0
  else								
   kenv	linsegr	1,iRelTim,0			; attack time is zero so ignore this segment of the envelope (a segment of duration zero is not permitted
  endif
  kenv	expcurve	kenv,8			; remap amplitude value with a more natural curve
  aenv	interp		kenv			; interpolate and create a-rate envelope
  iFileLen	filelen	gSfilepath		; derive chosen sound file length
  iNChns	filenchnls	gSfilepath	; derive the number of channels (mono=1 / stereo=2) from the chosen  sound file
  iinskip	chnget	"inskip"		; read in inskip widget
  iloop	chnget	"loop"				; read in 'loop mode' widget
  ktrig	changed	gkloop,gkinterp			; if loop setting or interpolation mode setting
  if ktrig==1 then				; if loop setting has been changed...
   reinit RESTART				; reinitialise from label RESTART
  endif						
  RESTART:					
  kporttime	linseg	0,0.001,0.05		; portamento time function. (Rises quickly from zero to a held value.)
  kspeed	portk	gkspeed,kporttime	; apply portamento smoothing to changes in speed
  
  kMod		jspline	gkModRange,gkRate1,gkRate2
  kspeed	*=	octave(kMod)
  
  if gkStretchOnOff!=1 kgoto SKIP_STRETCH
   a1,a2	init	0
   krms	rms	a1+a2
   if krms<gkthreshold then
    kspeed	*=	gkstretchratio
    kmute	=	0
   else
    kmute	=	1
   endif
   amute		interp	kmute
   aenv		*=	amute
  SKIP_STRETCH:
  
  if iNChns==2 then				; if stereo...
   a1,a2	diskin2	gSfilepath,kspeed*(1-(gkreverse*2)),iinskip*iFileLen,i(gkloop),0,giInterpArr[i(gkinterp)-1]	; use stereo diskin2
  	outs	a1*gklevel*aenv, a2*gklevel*aenv	; send audio to outputs
  elseif iNChns==1 then				; if mono
   a1	diskin2	gSfilepath,kspeed*(1-(gkreverse*2)),iinskip*iFileLen,i(gkloop),0,giInterpArr[i(gkinterp)-1]	; use mono diskin2
  	outs	a1*gklevel*aenv, a1*gklevel*aenv	; send audio to outputs
  endif
 endif
endin


instr	3
 icps	cpsmidi							; read in midi note data as cycles per second
 iamp	ampmidi	1						; read in midi velocity (as a value within the range 0 - 1)
 iStrLen 	strlen	gSfilepath				; derive string length
 if iStrLen > 0 then						; if string length is greater than zero (i.e. a file has been selected) then...
  iMidiRef	chnget	"MidiRef"				; MIDI unison reference note
  iinskip	chnget	"inskip"				; read in inskip widget
  iloop		chnget	"loop"					; read in 'loop mode' widget
  iAttTim	chnget	"AttTim"				; read in amplitude envelope attack time widget
  iRelTim	chnget	"RelTim"				; read in amplitude envelope attack time widget
  if iAttTim>0 then						; 
   kenv	linsegr	0,iAttTim,1,iRelTim,0
  else								
   kenv	linsegr	1,iRelTim,0					; attack time is zero so ignore this segment of the envelope (a segment of duration zero is not permitted
  endif
  kenv	expcurve	kenv,8					; remap amplitude value with a more natural curve
  aenv	interp		kenv					; interpolate and create a-rate envelope
  iFileLen	filelen	gSfilepath				; derive chosen sound file length
  ispeed		=	icps/cpsmidinn(iMidiRef)	; derive playback speed from note played in relation to a reference note (MIDI note 60 / middle C)
  iNChns	filenchnls	gSfilepath			; derive the number of channels (mono=1 / stereo=2) from the chosen  sound file
  if iNChns==2 then						; if stereo...
   a1,a2	diskin2	gSfilepath,ispeed*(1-(gkreverse*2)),iinskip*iFileLen,i(gkloop),0,giInterpArr[i(gkinterp)-1]	; use stereo diskin2
  	outs	a1*gklevel*aenv*iamp, a2*gklevel*aenv*iamp	; send audio to outputs
  elseif iNChns==1 then						; if mono
   a1		diskin2	gSfilepath,ispeed*(1-(gkreverse*2)),iinskip*iFileLen,i(gkloop),0,giInterpArr[i(gkinterp)-1]	; use mono diskin2
  	outs	a1*gklevel*aenv*iamp, a1*gklevel*aenv*iamp	; send audio to outputs
  endif
 endif
endin

instr	99
 Smessage sprintfk "file(%s)", gSfilepath			; print sound file image to fileplayer
 chnset Smessage, "filer1"

 /* write file name to GUI */
 Sname FileNameFromPath	gSfilepath				; Call UDO to extract file name from the full path
 Smessage sprintfk "text(%s)",Sname				; create string to update text() identifier for label widget
 chnset Smessage, "stringbox"					; send string to  widget

endin

</CsInstruments>  

<CsScore>
i 1 0 10000
</CsScore>

</CsoundSynthesizer>
; FileRecorder.csd
; Written by Iain McCurdy, 2014
; 
; Records audio to a file on disk. 
; 
; The audio file name includes the date and time. (This idea comes courtesy of Rory Walsh.)
; 
; Toggling record off and on will append the new recording onto the end of the previously recorded audio.
; To start a new file, click 'New File'.


<Cabbage>
form caption("File Recorder") size(230, 45), colour(0,0,0) pluginid("FRec") style("legacy")
image               bounds(  0,  0,230, 45), colour(100,100,100), outlinecolour("White"), line(3)
checkbox bounds( 10, 10, 75, 25), channel("record"), text("Record"), colour("red"), fontcolour("white")
button   bounds( 90, 10, 65, 25), colour("red"), text("New File","New File"), channel("reset"), latched(0)
checkbox bounds(165, 10, 75, 25), channel("play"), text("Play"), colour("green"), fontcolour("white")
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-n -dm0
</CsOptions>

<CsInstruments>

sr = 44100
ksmps = 64
nchnls = 2
0dbfs = 1

gkRecordingActiveFlag	init	0
gkFileRecorded		init	0

instr	1
 gaL,gaR	ins
 gkrecord	chnget	"record"
 gkreset	chnget	"reset"
 gkplay		chnget	"play"
 kRecStart	trigger	gkrecord,0.5,0

 if kRecStart==1 && gkRecordingActiveFlag==0 then
  event	"i",9000,0,-1
  gkRecordingActiveFlag	=	1
 endif
 
 kPlayStart	trigger	gkplay,0.5,0
 if kPlayStart==1 && gkFileRecorded==1 then
  event	"i",9001,ksmps/sr,3600
 endif

 kResetTrig	trigger	gkreset,0.5,1
 if kResetTrig==1 && gkRecordingActiveFlag==1 then
  event	"i",9000,0,-1
 endif  
endin


instr 9000	; record file
 print	p1
 if gkplay==1 then
  chnset	k(0),"record"
  turnoff
 endif
 gkFileRecorded		init	1
 itim     date
 Stim     dates     itim
 itim     date
 Stim     dates     itim
 Syear    strsub    Stim, 20, 24
 Smonth   strsub    Stim, 4, 7
 Sday     strsub    Stim, 8, 10
 iday     strtod    Sday
 Shor     strsub    Stim, 11, 13
 Smin     strsub    Stim, 14, 16
 Ssec     strsub    Stim, 17, 19
 Sfilnam  sprintf  "%s_%s_%02d_%s_%s_%s.wav", Syear, Smonth, iday, Shor,Smin, Ssec
 gSname   sprintf "FileRecorder_%s", Sfilnam
 if gkrecord==1 then			; record
  fout gSname, 8, gaL, gaR
 endif
 gkRecordingActiveFlag	=	1 - release()
endin

instr	9001	; play file
 if gkplay==0 then
  turnoff
 endif 
   aL,aR	diskin2	gSname,1
  	outs	aL,aR
 iFileLen	filelen	gSname
 p3	=	iFileLen
 xtratim	0.1 
 krelease	release
 	chnset	1-krelease,"play"
endin

</CsInstruments>  

<CsScore>
i 1 0 [60*60*24*7]
</CsScore>

</CsoundSynthesizer>; FileStretcher.csd
; Written by Iain McCurdy, 2014

; Used to create a time-stretched version of an input file
; Often when time-stretching a sound when wishing to retain the priniciple characteristics of the source sound,
;  we play the attack portion of the sound at normal speed (as well, often, as the release portion) and only stretch
;  sound in the middle of the source - often called the 'sustain' portion.
; This example facilitates this technique by allowing the user to select the portion of sound to stretch by 
;  clicking and dragging upon the waveform view creating a highlighted area. Time stretching will only be applied to this 
;  portion of the sound.

; The user defines a time-stretching zone by clicking and dragging on the waveform view once a file has been opened.
; 'Duration' defines the total duration of the output file.

; 'Play' auditions the file stretch. 
; 'Record' renders the time-stretched audio to disk offline.
; The console reports the name of the output file and a message: 'Done!' when the rendering is complete. 
;    The output file will be created in the same directory as the input file and with the same name but with the word '_stretched' and a unique number suffixed. 

; 'Aud.Lev.' provides a amplitude level control (the rendered file is unaffected and always renders at full amplitude.

; Adding a small amount of jitter to the file reading pointer when time-stretching can reduce the sense of a frozen spectrum 
;  evident during long time stretches. Jitter depth and rate can be modified.

<Cabbage>
form caption("File Stretcher") size(580,430), colour(0,0,0) pluginid("FiSt"), guirefresh(10)  style("legacy")
image                bounds(  0,  0,580,430), colour(50,50,60), outlinecolour("White"), line(3), shape("sharp")

soundfiler           bounds(  5,  5,570,150), channel("beg","len"), identchannel("filer1"),  colour(0, 255, 255, 255), fontcolour(160, 160, 160, 255)
label bounds(6, 4, 560, 14), text(""), align(left), colour(0,0,0,0), fontcolour(200,200,200), identchannel("stringbox")
label      bounds( 6, 20,450, 13), text("Click and drag on waveform to select sound portion for time stretching..."), align(left), colour("black"), fontcolour("white"), visible(0), identchannel("InstructionID")

filebutton bounds(  7,160, 83, 20), text("OPEN FILE","OPEN FILE"), fontcolour(255,255,100) channel("filename"), shape("ellipse"), channel("beg","len")
button     bounds(  7,183, 83, 20), text("PLAY STR.","PLAY STR."), channel("PlayStr"), value(0), latched(0), fontcolour(100,255,100)
button     bounds( 96,183, 83, 20), text("PLAY NORM.","PLAY NORM."), channel("PlayNorm"), value(0), latched(0), fontcolour(150,150,255)
button     bounds(  7,206, 83, 20), text("RECORD","RECORD"), channel("Record"), value(0), latched(0), fontcolour(255,100,100)
button     bounds( 96,206, 83, 20), text("STOP","STOP"), channel("Stop"), value(0), latched(0), fontcolour(150,150,255)

rslider    bounds(178,160, 60, 60), text("Duration"), channel("Duration"), range(0.1, 60.00, 3,0.5,0.001), colour(30,30,40), trackercolour("white"), fontcolour("white"), outlinecolour(150,150,150)
rslider    bounds(238,160, 60, 60), text("Transpose"), channel("Transpose"), range(-48, 48.00, 0,1,0.5), colour(30,30,40), trackercolour("white"), fontcolour("white"), outlinecolour(150,150,150)
;nslider  bounds(180,160,110, 35), text("Stretched Duration"), channel("Duration"), range(0.1, 800.00, 3,1,0.001), colour(0,0,0), fontcolour("white"), textbox(1)
label      bounds(300,160, 80, 13), text("FFT Size"), fontcolour("white")
combobox   bounds(300,177, 80, 17), channel("FFTSize"), items("32768", "16384", "8192", "4096", "2048", "1024", "512", "256", "128", "64", "32"), value(5), fontcolour("white")
checkbox   bounds(300,198, 95, 15), channel("lock"), text("Phase Lock"), fontcolour("white"), colour(255,0,0)

rslider    bounds(390,160, 60, 60), text("Aud.Lev."), channel("Level"),  range(0, 1.00, 0.9),       colour(30,30,40), trackercolour("white"), fontcolour("white"), outlinecolour(150,150,150)
rslider    bounds(450,160, 60, 60), text("Jit.Dep."), channel("JitDep"), range(0, 1.00, 0.2),       colour(30,30,40), trackercolour("white"), fontcolour("white"), outlinecolour(150,150,150)
rslider    bounds(510,160, 60, 60), text("Jit.Rte."), channel("JitRte"), range(0.5, 100.00, 2,0.5), colour(30,30,40), trackercolour("white"), fontcolour("white"), outlinecolour(150,150,150)

csoundoutput bounds(  0,230,580,200), text("Csound Output")

</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-n -dm0
</CsOptions>

<CsInstruments>

sr = 44100
ksmps = 64
nchnls = 2
0dbfs=1

gichans		init	0		; 
giReady		init	0		; flag to indicate function table readiness
gSfilepath	init	""
giFFTSizes[]	array	32768, 16384, 8192, 4096, 2048, 1024, 512, 256, 128, 64, 32	; an array is used to store FFT window sizes
;maxalloc	2,1	; Only allow one sound playing/stretching instrument at a time
gkactive	init	0


opcode FileNameFromPath,S,S		; Extract a file name (as a string) from a full path (also as a string)
 Ssrc	xin						; Read in the file path string
 icnt	strlen	Ssrc			; Get the length of the file path string
 LOOP:							; Loop back to here when checking for a backslash
 iasc	strchar Ssrc, icnt		; Read ascii value of current letter for checking
 if iasc==92 igoto ESCAPE		; If it is a backslash, escape from loop
 loop_gt	icnt,1,0,LOOP		; Loop back and decrement counter which is also used as an index into the string
 ESCAPE:						; Escape point once the backslash has been found
 Sname	strsub Ssrc, icnt+1, -1	; Create a new string of just the file name
	xout	Sname				; Send it back to the caller instrument
endop

instr	1
 gkPlayStr	chnget	"PlayStr"
 gkPlayNorm	chnget	"PlayNorm"
 gkRecord	chnget	"Record"
 gSfilepath	chnget	"filename"
 kNewFileTrg	changed	gSfilepath		; if a new file is loaded generate a trigger
 gkLevel	chnget	"Level"
 gkTranspose	chnget	"Transpose"
 kporttime	linseg	0,0.01,0.05
 gkTranspose	portk	semitone(gkTranspose),kporttime
 gklock		chnget	"lock"			; Phase locking in mincer opcode
 gkAttack	chnget	"beg"
 gkLen		chnget	"len" 
 
 if trigger(gkAttack,1,0)==1 then		; Hide instruction once stretch area has been selected
  chnset "visible(0)", "InstructionID" 
 endif
 
 if kNewFileTrg==1 then				; if a new file has been loaded...
  event	"i",99,0,0.01				; call instrument to update sample storage function table 
 endif   
 
 ktrig	trigger	gkPlayStr,0.5,0			; Generate an impulse if PLAY STRETCH button is clicked
 schedkwhen	ktrig,0,0,2,0,0.01

 ktrig	trigger	gkPlayNorm,0.5,0		; Generate an impulse if PLAY NORM. button is clicked
 schedkwhen	ktrig,0,0,3,0,0.01

 ktrig	trigger	gkRecord,0.5,0			; Generate an impulse if RECORD button is clicked
 schedkwhen	ktrig,0,0,4,0,0.1
 
endin

instr	99	; load sound file
 
 /* write file selection to function tables */
 gichans	filenchnls	gSfilepath			; derive the number of channels (mono=1,stereo=2) in the sound file
 gitableL	ftgen	1,0,0,1,gSfilepath,0,0,1
 if gichans==2 then
  gitableR	ftgen	2,0,0,1,gSfilepath,0,0,2
 endif
 giReady 	=	1					; if no string has yet been loaded giReady will be zero
 Smessage sprintfk "file(%s)", gSfilepath			; print sound file image to fileplayer
 chnset Smessage, "filer1"

 /* write file name to GUI */
 Sname FileNameFromPath	gSfilepath				; Call UDO to extract file name from the full path
 Smessage sprintfk "text(%s)",Sname				; create string to update text() identifier for label widget
 chnset Smessage, "stringbox"					; send string to  widget

 if i(gkAttack)==0 then						; Reveal instruction when a file is opened for the first time
  chnset "visible(1)", "InstructionID"
 endif	

 /* write to pvx files */					; experimental
 ;#define	ANALYSISFILE_L	#"pvswriteFileL.pvx"#		; experimental
 ;#define	ANALYSISFILE_R	#"pvswriteFileR.pvx"#		; experimental
 ;kcount	=	0					; experimental
 ;loop:								; experimental
 ;acount	interp	kcount					; experimental
 ;asampL	table3	acount,1				; experimental	
 ;asampR	table3	acount,2                                ; experimental
 ;fsigL	pvsanal		asampL, 1024, 256, 1024, 1  		; experimental
 ;fsigR	pvsanal		asampR, 1024, 256, 1024, 1      	; experimental
 ;pvsfwrite 	fsigL, $ANALYSISFILE_L				; experimental
 ;pvsfwrite 	fsigR, $ANALYSISFILE_R				; experimental
 ;loop_lt	kcount,1,ftlen(1),loop				; experimental
 ;turnoff							; experimental

endin

instr	2	; play stretched
 if giReady = 1 then				; i.e. if a file has been loaded
  
  gkactive init i(gkactive) + 1	;INCREMENT NOTE COUNTER
  if gkactive>1 then		;IF POLYPHONY IS EXCEEDED (THROUGH THE ADDITION OF NEW NOTE)
   turnoff			;REMOVE THIS NOTE
  endif
  krel release			;IF NOTE HELD = 0, IF NOTE RELEASED = 1
  ktrig trigger krel,0.5,0	;WHEN RELEASE FLAG CROSSES 0.5 UPWARDS, I.E. NOTE HAS BEEN RELEASED...	
  if ktrig==1 then		
   gkactive = gkactive - 1	;...DECREMENT ACTIVE NOTES COUNTER
  endif
  
  kStop		chnget	"Stop"
  ktrig		trigger	kStop,0.5,0		; Generate an impulse if 'STOP' is clicked
  if ktrig==1 then
   turnoff
  endif

  iFileLen	=	ftlen(gitableL)/sr											; File length in seconds
  iAttPnt	=	(i(gkAttack) == 0 ? 0.01 : (i(gkAttack)/sr))				; If stretch points haven't been set, replace with defaults. 
  iRelPnt	=	(i(gkAttack) == 0 ? 0.01 : ((i(gkAttack) + i(gkLen))/sr))
  
  iDuration	chnget	"Duration"
  
  p3		limit	iDuration, iFileLen, iDuration
  
  iFFTSize	chnget	"FFTSize"
  iFFTSize	=	giFFTSizes[iFFTSize-1]
  
  ; File reading pointer main shape
  aptr		linseg	0,iAttPnt,iAttPnt, p3-iAttPnt-(iFileLen-iRelPnt), iRelPnt, iFileLen-iRelPnt, iFileLen

  ; File reading pointer jitter
  kJitDep	chnget	"JitDep"
  kJitRte	chnget	"JitRte"
  irise		=	0.01
  kJitEnv	linseg	0,iAttPnt,0, irise, .1, p3-iAttPnt-(iFileLen-iRelPnt) - (2*irise), .1, irise, 0 
  ajit		jspline	kJitDep*kJitEnv,kJitRte*0.5,kJitRte*2

  if gichans=1 then						; Mono
   a1	mincer		aptr+ajit, 1, gkTranspose, gitableL, gklock, iFFTSize
   ;kptr	linseg	0,iAttPnt,iAttPnt, p3-iAttPnt-(iFileLen-iRelPnt), iRelPnt, iFileLen-iRelPnt, iFileLen	; experimental
   ;a1	pvoc		kptr, gkTranspose, $ANALYSISFILE_L							; experimental
  	outs	a1*gkLevel,a1*gkLevel

  elseif gichans=2 then						; Stereo
   a1	mincer		aptr+ajit, 1, gkTranspose, gitableL, gklock, iFFTSize
   a2	mincer		aptr+ajit, 1, gkTranspose, gitableR, gklock, iFFTSize
   ;kptr	linseg	0,iAttPnt,iAttPnt, p3-iAttPnt-(iFileLen-iRelPnt), iRelPnt, iFileLen-iRelPnt, iFileLen	; experimental
   ;a1	pvoc		kptr, gkTranspose, $ANALYSISFILE_L							; experimental
   ;a2	pvoc		kptr, gkTranspose, $ANALYSISFILE_R							; experimental
  	outs	a1*gkLevel,a2*gkLevel
  endif
 endif
endin

instr	3	; normal speed playback
 p3	=	ftlen(gitableL)/(sr*i(gkTranspose))
 aptr	line	0,p3,1
 aL	table3	aptr,gitableL,1
 if gichans==1 then
 	outs	aL*gkLevel,aL*gkLevel
 else
 aR	table3	aptr,gitableR,1
	outs	aL*gkLevel,aR*gkLevel 
 endif 
 kStop		chnget	"Stop"
 ktrig		trigger	kStop,0.5,0		; Generate an impulse if 'STOP' is clicked
 if ktrig==1 then
  turnoff
 endif
endin


instr	4	; record/render
 if giReady = 1 then				; i.e. if a file has been loaded

  iFileLen	=	ftlen(gitableL)/sr						; File length in seconds
  iAttPnt	=	(i(gkAttack) == 0 ? 0.01 : (i(gkAttack)/sr))			; If stretch points haven't been set, replace with defaults. 
  iRelPnt	=	(i(gkAttack) == 0 ? 0.01 : ((i(gkAttack) + i(gkLen))/sr))
  
  iDuration	chnget	"Duration"
  
  p3		=	iDuration
     		
  ilen		strlen	gSfilepath			; Derive string length.
  SOutputName	strsub	gSfilepath,0,ilen-4		; Remove ".wav"
  SOutputName	strcat	SOutputName,"_stretched"	; Add suffix
  iDate		date
  SDate		sprintf	"%i",iDate
  SOutputName	strcat	SOutputName,SDate		; Add date
  SOutputName	strcat	SOutputName,".wav"		; Add extension
  
  
  iFFTSize	chnget	"FFTSize"
  iFFTSize	=	giFFTSizes[iFFTSize-1]
  
  iNKCycles	=	(iDuration * sr) / ksmps
  kCount	init	0
  
  loop:
  
  ; File reading pointer main shape
  aptr		linseg	0,iAttPnt,iAttPnt, p3-iAttPnt-(iFileLen-iRelPnt), iRelPnt, iFileLen-iRelPnt, iFileLen

  ; File reading pointer jitter
  kJitDep	chnget	"JitDep"
  kJitRte	chnget	"JitRte"
  irise		=	0.01
  kJitEnv	linseg	0,iAttPnt,0, irise, .1, p3-iAttPnt-(iFileLen-iRelPnt) - (2*irise), .1, irise, 0 
  ajit		jspline	kJitDep*kJitEnv,kJitRte*0.5,kJitRte*2

  if gichans=1 then						; Mono
   a1	mincer		aptr+ajit, 1, gkTranspose, gitableL, gklock, iFFTSize
   ;kptr	linseg	0,iAttPnt,iAttPnt, p3-iAttPnt-(iFileLen-iRelPnt), iRelPnt, iFileLen-iRelPnt, iFileLen	; experimental
   ;a1	pvoc		kptr, gkTranspose, $ANALYSISFILE_L							; experimental
  	fout	SOutputName,8,a1

  elseif gichans=2 then						; Stereo
   a1	mincer		aptr+ajit, 1, gkTranspose, gitableL, gklock, iFFTSize
   a2	mincer		aptr+ajit, 1, gkTranspose, gitableR, gklock, iFFTSize
   ;kptr	linseg	0,iAttPnt,iAttPnt, p3-iAttPnt-(iFileLen-iRelPnt), iRelPnt, iFileLen-iRelPnt, iFileLen	; experimental
   ;a1	pvoc		kptr, gkTranspose, $ANALYSISFILE_L							; experimental
   ;a2	pvoc		kptr, gkTranspose, $ANALYSISFILE_R							; experimental
  	fout	SOutputName,8,a1,a2
  endif

	loop_lt	kCount, 1, iNKCycles, loop
	printks	SOutputName,0
	printks	"\nDone!\n",0
	turnoff
	
 endif
endin

</CsInstruments>  

<CsScore>
i 1 0 10000
</CsScore>

</CsoundSynthesizer>
; Flooper2FilePlayer.csd
; Written by Iain McCurdy, 2014

; Load a user selected sound file into a GEN 01 function table and plays it back using flooper2. 
; This file player is best suited for polyphonic playback and is less well suited for the playback of very long sound files .
; 
; The sound file can be played back using the Play/Stop button (and the 'Transpose' / 'Speed' buttons to implement pitch/speed change)
;  or it can be played back using the MIDI keyboard.
; 
; The loop points can be set either by using the loop 'Start' and 'End' sliders or by clicking and dragging on the waveform view -
;  - flooper2 will take the values from the last control input moved.

<Cabbage>
form caption("Flooper2 File Player") size(805,340), colour(0,0,0) pluginid("FlFP") style("legacy")
image                     bounds(  0,  0,805,340), colour(155, 50,  0), outlinecolour("White"), line(3), shape("sharp")	; main panel colouration    

soundfiler bounds(  5,  5,795,175), channel("beg","len"), identchannel("filer1"),  colour(0, 255, 255, 255), fontcolour(160, 160, 160, 255), 
label bounds(6, 4, 560, 14), text(""), align(left), colour(0,0,0,0), fontcolour(200,200,200), identchannel("stringbox")

image    bounds(  0,180,805,160), colour(155,30,0,0), outlinecolour("white"), line(2), shape("sharp"), plant("controls"){
filebutton bounds(  5, 10, 80, 25), text("Open File","Open File"), fontcolour("white") channel("filename"), shape("ellipse")
checkbox   bounds(  5, 40, 95, 25), channel("PlayStop"), text("Play/Stop"), colour("yellow"), fontcolour("white")

label      bounds(110, 12, 80, 12), text("Looping Mode"), fontcolour("white")
combobox   bounds(110, 25, 80, 20), channel("mode"), items("Forward", "Backward", "Fwd./Bwd."), value(1), fontcolour("white")

line       bounds(207, 10,  2, 65), colour("Grey")
                        
label      bounds(302,  4, 43, 8), text("L   O   O   P"), fontcolour("white")
rslider    bounds(210, 15, 60, 60), channel("LoopStart"), range(0, 1, 0),                   colour(100, 30, 10), text("Start"),     textcolour("white"), trackercolour(175,130,110)
rslider    bounds(265, 15, 60, 60), channel("LoopEnd"),   range(0, 1, 1),                   colour(100, 30, 10), text("End"),       textcolour("white"), trackercolour(175,130,110)
rslider    bounds(320, 15, 60, 60), channel("crossfade"), range(0, 1.00, 0.01,0.5),         colour(100, 30, 10), text("Fade"),      textcolour("white"), trackercolour(175,130,110)
rslider    bounds(375, 15, 60, 60), channel("inskip"),    range(0, 1.00, 0),                colour(100, 30, 10), text("inskip"),    textcolour("white"), trackercolour(175,130,110)
line       bounds(440, 10,  2, 65), colour("Grey")

label      bounds(475,  4, 53, 8), text("S   P   E   E   D"), fontcolour("white")
rslider    bounds(445, 15, 60, 60), channel("transpose"), range(-24, 24, 0,1,1),            colour(100, 30, 10), text("Transpose"), textcolour("white"), trackercolour(175,130,110)
rslider    bounds(500, 15, 60, 60), channel("speed"),     range( 0, 4.00, 1, 0.5),          colour(100, 30, 10), text("Speed"),     textcolour("white"), trackercolour(175,130,110)
line       bounds(560, 10,  2, 65), colour("Grey")

label      bounds(576,  4, 90, 8), text("E   N   V   E   L   O   P   E"), fontcolour("white")
rslider    bounds(565, 15, 60, 60), channel("AttTim"),    range(0, 5, 0, 0.5, 0.001),       colour(100, 30, 10), text("Att.Tim"),   textcolour("white"), trackercolour(175,130,110)
rslider    bounds(620, 15, 60, 60), channel("RelTim"),    range(0.01, 5, 0.05, 0.5, 0.001), colour(100, 30, 10), text("Rel.Tim"),   textcolour("white"), trackercolour(175,130,110)
line       bounds(680, 10,  2, 65), colour("Grey")

label      bounds(702,  4, 80, 8), text("C   O   N   T   R   O   L"), fontcolour("white")
rslider    bounds(685, 15, 60, 60), channel("MidiRef"),   range(0,127,60, 1, 1),            colour(100, 30, 10), text("MIDI Ref."), textcolour("white"), trackercolour(175,130,110)
rslider    bounds(740, 15, 60, 60), channel("level"),     range(  0,  3.00, 1, 0.5),        colour(100, 30, 10), text("Level"),     textcolour("white"), trackercolour(175,130,110)

keyboard bounds(5, 80, 795, 75)
}
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-n -+rtmidi=NULL -M0 -dm0
</CsOptions>

<CsInstruments>

sr = 44100
ksmps = 64
nchnls = 2
0dbfs = 1

		massign	0,3
gichans		init	0
giReady		init	0
gSfilepath	init	""

gitableL	ftgen	1,0,2,2,0
gkTabLen	init	ftlen(gitableL)

opcode FileNameFromPath,S,S		; Extract a file name (as a string) from a full path (also as a string)
 Ssrc	xin						; Read in the file path string
 icnt	strlen	Ssrc			; Get the length of the file path string
 LOOP:							; Loop back to here when checking for a backslash
 iasc	strchar Ssrc, icnt		; Read ascii value of current letter for checking
 if iasc==92 igoto ESCAPE		; If it is a backslash, escape from loop
 loop_gt	icnt,1,0,LOOP		; Loop back and decrement counter which is also used as an index into the string
 ESCAPE:						; Escape point once the backslash has been found
 Sname	strsub Ssrc, icnt+1, -1	; Create a new string of just the file name
	xout	Sname				; Send it back to the caller instrument
endop

instr	1
 gkmode		chnget	"mode"
 kLoopStart	chnget	"LoopStart"		; sliders
 kLoopEnd	chnget	"LoopEnd"		;  "
 kbeg		chnget	"beg"			; click and drag
 klen		chnget	"len"			;  "
 kTrigSlid	changed	kLoopStart,kLoopEnd
 kTrigCAD	changed	kbeg,klen
 if kTrigSlid==1 then
  gkLoopStart	=	kLoopStart
  gkLoopEnd	=	kLoopEnd
 elseif kTrigCAD==1 then
  gkLoopStart	=	kbeg/gkTabLen
  gkLoopEnd	=	(kbeg+klen)/gkTabLen
 endif

 gkLoopEnd	limit	gkLoopEnd,gkLoopStart+0.01,1	; limit loop end to prevent crashes
 gkcrossfade	chnget	"crossfade"
 gkinskip	chnget	"inskip"
 gkPlayStop	chnget	"PlayStop"
 gktranspose	chnget	"transpose"
 gkspeed	chnget	"speed"
 gklevel	chnget	"level"

 gSfilepath	chnget	"filename"
 kNewFileTrg	changed	gSfilepath		; if a new file is loaded generate a trigger
 if kNewFileTrg==1 then				; if a new file has been loaded...
  event	"i",99,0,0				; call instrument to update sample storage function table 
 endif  
 
 ktrig	trigger	gkPlayStop,0.5,0		; if play button changes to 'play', generate a trigger
 schedkwhen	ktrig,0,0,2,0,-1		; start instr 2 playing a held note

 ktrig1	changed	gktranspose			; if 'transpose' button is changed generate a '1' trigger
 ktrig2	changed	gkspeed				; if 'speed' button is changed generate a '1' trigger
 
 if ktrig1==1 then				; if transpose control has been changed...
  chnset	semitone(gktranspose),"speed"	; set speed according to transpose value
 elseif ktrig2==1 then		; if speed control has been changed...
  chnset	log2(gkspeed)*12,"transpose"	; set transpose control according to speed value
 endif

endin



instr	99	; load sound file
 gichans	filenchnls	gSfilepath			; derive the number of channels (mono=1,stereo=2) in the sound file
 gitableL	ftgen	1,0,0,1,gSfilepath,0,0,1
 giFileLen	filelen		gSfilepath			; derive the file duration
 gkTabLen	init		ftlen(gitableL)			; table length in sample frames
 if gichans==2 then
  gitableR	ftgen	2,0,0,1,gSfilepath,0,0,2
 endif
 giReady 	=	1					; if no string has yet been loaded giReady will be zero

 Smessage sprintfk "file(%s)", gSfilepath			; print sound file to viewer
 chnset Smessage, "filer1"	

 /* write file name to GUI */
 Sname FileNameFromPath	gSfilepath				; Call UDO to extract file name from the full path
 Smessage sprintfk "text(%s)",Sname				; create string to update text() identifier for label widget
 chnset Smessage, "stringbox"					; send string to  widget

endin



instr	2	; sample triggered by 'play/stop' button
 if gkPlayStop==0 then
  turnoff
 endif
 ktrig changed	gkmode
 if ktrig==1 then
  reinit RESTART
 endif
 RESTART:
 if giReady = 1 then						; i.e. if a file has been loaded
  iAttTim	chnget	"AttTim"				; read in widgets
  iRelTim	chnget	"RelTim"
  if iAttTim>0 then						; is amplitude envelope attack time is greater than zero...
   kenv	linsegr	0,iAttTim,1,iRelTim,0				; create an amplitude envelope with an attack, a sustain and a release segment (senses realtime release)
  else
   kenv	linsegr	1,iRelTim,0					; create an amplitude envelope with a sustain and a release segment (senses realtime release)
  endif
  kenv	expcurve	kenv,8					; remap amplitude value with a more natural curve
  aenv	interp		kenv					; interpolate and create a-rate envelope
  kporttime	linseg	0,0.001,0.05				; portamento time function. (Rises quickly from zero to a held value.)
  kspeed	portk	gkspeed,kporttime			; apply portamento smoothing to changes in speed
  klevel	portk	gklevel,kporttime			; apply portamento smoothing to changes in level
  kcrossfade	=	0.01
  istart	=	0
  ifenv		=	0
  iskip		=	0
  if gichans==1 then						; if mono...
   a1	flooper2	klevel,kspeed, gkLoopStart*giFileLen, gkLoopEnd*giFileLen, gkcrossfade, gitableL, i(gkinskip)*giFileLen, i(gkmode)-1, ifenv, iskip
	outs	a1*aenv,a1*aenv					; send mono audio to both outputs 
  elseif gichans==2 then						; otherwise, if stereo...
   a1	flooper2	klevel,kspeed, gkLoopStart*giFileLen, gkLoopEnd*giFileLen, gkcrossfade, gitableL, i(gkinskip)*giFileLen, i(gkmode)-1, ifenv, iskip
   a2	flooper2	klevel,kspeed, gkLoopStart*giFileLen, gkLoopEnd*giFileLen, gkcrossfade, gitableR, i(gkinskip)*giFileLen, i(gkmode)-1, ifenv, iskip
 	outs	a1*aenv,a2*aenv					; send stereo signal to outputs
  endif               
 endif
endin

instr	3	; sample triggered by midi note
 icps	cpsmidi							; read in midi note data as cycles per second
 iamp	ampmidi	1						; read in midi velocity (as a value within the range 0 - 1)
 iMidiRef	chnget	"MidiRef"

 if giReady = 1 then						; i.e. if a file has been loaded
  iAttTim	chnget	"AttTim"				; read in widgets
  iRelTim	chnget	"RelTim"
  if iAttTim>0 then						; is amplitude envelope attack time is greater than zero...
   kenv	linsegr	0,iAttTim,1,iRelTim,0				; create an amplitude envelope with an attack, a sustain and a release segment (senses realtime release)
  else
   kenv	linsegr	1,iRelTim,0					; create an amplitude envelope with a sustain and a release segment (senses realtime release)
  endif
  kenv	expcurve	kenv,8					; remap amplitude value with a more natural curve
  aenv	interp		kenv					; interpolate and create a-rate envelope
  kporttime	linseg	0,0.001,0.05				; portamento time function. (Rises quickly from zero to a held value.)
  klevel	portk	gklevel,kporttime			; apply portamento smoothing to changes in level
  kcrossfade	=	0.01
  istart	=	0
  ifenv		=	0
  iskip		=	0
  if gichans==1 then						; if mono...
   a1	flooper2	klevel*iamp,icps/cpsmidinn(iMidiRef), gkLoopStart*giFileLen, gkLoopEnd*giFileLen, gkcrossfade, gitableL, i(gkinskip)*giFileLen, i(gkmode)-1, ifenv, iskip
	outs	a1*aenv,a1*aenv			; send mono audio to both outputs 
  elseif gichans==2 then					; otherwise, if stereo...
   a1	flooper2	klevel*iamp,icps/cpsmidinn(iMidiRef), gkLoopStart*giFileLen, gkLoopEnd*giFileLen, gkcrossfade, gitableL, i(gkinskip)*giFileLen, i(gkmode)-1, ifenv, iskip
   a2	flooper2	klevel*iamp,icps/cpsmidinn(iMidiRef), gkLoopStart*giFileLen, gkLoopEnd*giFileLen, gkcrossfade, gitableR, i(gkinskip)*giFileLen, i(gkmode)-1, ifenv, iskip
 	outs	a1*aenv,a2*aenv			; send stereo signal to outputs
  endif               
 endif

endin
 
</CsInstruments>  

<CsScore>
i 1 0 [60*60*24*7]
</CsScore>

</CsoundSynthesizer>
; FogFilePlayer.csd
; Written by Iain McCurdy, 2015

; File player based around the granular synthesis opcode, 'fog'.
; A second voice can be activated (basically another parallel granular synthesiser) with parameter variations of density, transposition, pointer location (Phs) and delay.
; Two modes of playback are available: manual pointer and speed
; The pointer and grain density can also be modulated by clicking and dragging on the waveform view.
;  * This will also start and stop the grain producing instrument.
;  * In click-and-drag mode mouse X position equates to pointer position and mouse Y position equates to grain density. 
; If played from the MIDI keyboard, note number translates to 'Transposition' and key velocity translates to amplitude for the grain stream for that note.

; In 'Pointer' mode pointer position is controlled by the long 'Manual' slider with an optional amount of randomisation determined ny the 'Phs.Mod' slider.  

; Selecting 'Speed' pointer mode bring up some additional controls:
; Speed		-	speed ratio
; Freeze	-	freezes the pointer at its present locations 
; Range		-	ratio of the full sound file duration that will be played back. 1=the_entire_file, 0.5=half_the_file, etc. 
; Shape		-	shape of playback function: 	'Phasor' looping in a single direction
;												'Tri' back and forth looping
;												'Sine' back and forth looping using a sinudoidal shape - i.e. slowing at the extremes of the oscillation
; The 'Manual' control functions as an pointer offset when using 'Speed' pointer mode

; Density	-	grains per second
; Oct.Div	-	thinning the density in overlapping octave steps. I.e. density is halved and then halved again etc. 
; Transpose	-	transposition as a ratio. Negative values result in grains playing in reverse.
; Transposition Mode - timing of transposition changes: 'Grain by Grain' 	- grains always maintain the transposition with which they began
;														'Continuous'		- even grains in progress can be altered by changes made to 'Transpose' 

; --Randomisation--
; Trans.Mod.	-	randomisation of transposition (in octaves)
; Ptr.Mod.	-	randomisation of pointer position
; Dens Mod.	-	randomisation of grain density
; Amp.Mod.	-	randomisation of grain amplitude. Note that this is done on a grain by grain basis, grains retain the amplitude with which they start.

; --Density LFO--
; Depth		-	depth of LFO modulation of grain density
; Rate		-	rate of LFO modulation of grain density

; --Voice 2--
; Dens.Ratio	-	ratio of grain density of voice 2 with respect to the main voice (also adjustable using the adjacent number box for precise value input)
; Ptr.Diff.	-	pointer position offset of voice 2 with respect to the main voice (also adjustable using the adjacent number box for precise value input)
; Trans.Diff.	-	transposition offset of voice 2 with respect to the main voice (also adjustable using the adjacent number box for precise value input)
; Delay		-	a delay applied to voice 2 which is defined as a ratio of the gap between grains (therefore delay time will be inversely proportional to garin density)
;			 This is a little like a phase offset for voice 2 with respect to that of the main voice.
;			 When using this control 'Dens.Ratio' should be '1' otherwise continuous temporally shifting between the grains of voice 2 and the main voice will be occurring anyway.

; --Envelope--
; Attack	-	amplitude envelope attack time for the envelope applied to complete notes
; Release	-	amplitude envelope release time for the envelope applied to complete notes

; --Control--
; MIDI Ref.	-	MIDI note that represent unison (no transposition) for when using the MIDI keyboard
; Level	-	output amplitude control


<Cabbage>
form caption("fog File Player") size(1130,445), colour(0,0,0), pluginid("FgFP"), guirefresh(16)	; guirefresh value adjusts how often the cabbage valuators are read. This can affect the usability of parameters used in realtime gesturing. style("legacy")

#define RSliderStyle # trackercolour(130,135,170), textcolour("white"), outlinecolour( 10, 15, 50), colour( 50, 45, 90)#

image       bounds(  0,  0,1130,445), colour( 30, 35, 70), outlinecolour("White"), shape("sharp"), line(3)	; main panel colouration    
soundfiler  bounds(  5,  5,1120,175), channel("beg","len"), identchannel("filer1"),  colour(0, 255, 255, 255), fontcolour(160, 160, 160, 255), 
label bounds(7, 5, 560, 14), text(""), align(left), colour(0,0,0,0), fontcolour(200,200,200), identchannel("stringbox")

hslider    bounds(  0,180,1120, 15), channel("phs"),   range( 0,1,0,1,0.0001), $RSliderStyle
label      bounds(  0,195,1120, 13), text("Manual"), fontcolour("white")

filebutton bounds(  5,210,  80, 25), text("Open File","Open File"), fontcolour("white") channel("filename"), shape("ellipse")
checkbox   bounds(  5,240,  95, 20), channel("PlayStop"), text("Play/Stop"), fontcolour("white")
label      bounds(  5,263, 145, 12), text("[or right-click and drag]"), fontcolour("white"), align("left")

label       bounds( 90,215, 75, 13), text("Ptr.Mode"), fontcolour("white")
combobox    bounds( 90,230, 75, 18), channel("PhsMode"), items("Manual", "Speed"), value(2),fontcolour("white")

rslider     bounds(170,215, 60, 60), channel("port"),     range( 0, 30.00, 0.01,0.5,0.01), text("Port."), identchannel("portID"), visible(0), $RSliderStyle

rslider     bounds(170,215, 60, 60), channel("spd"),     range( -2.00, 2.00, 1), text("Speed"), identchannel("spdID"), visible(0), $RSliderStyle
button      bounds(230,230, 60, 18), channel("freeze"),  colour:0(  0,  0,  0), colour:1(50,55,150), text("Freeze","Freeze"), fontcolour:0(50,50,50), fontcolour:1(200,200,255), identchannel("freezeID"), visible(0)
rslider     bounds(290,215, 60, 60), channel("range"),   range(0.01,  1,  1),              text("Range"),  identchannel("rangeID"), visible(0), $RSliderStyle
label       bounds(350,215, 60, 13), text("Shape"), fontcolour("white"), identchannel("shapelabelID")
combobox    bounds(350,230, 60, 18), channel("shape"), items("phasor","tri.","sine"), value(1),fontcolour("white"), identchannel("shapeID"), visible(0)

rslider     bounds(410,215, 60, 60), channel("dens"),    range(  1,500,   30, 0.5),  text("Density"), $RSliderStyle
rslider     bounds(470,215, 60, 60), channel("OctDiv"),  range(  0,  8,    0, 0.5),  text("Oct.Div."), $RSliderStyle
rslider     bounds(530,215, 60, 60), channel("pch"),     range(-2,2,1,1,0.001),              text("Transpose"), $RSliderStyle
label       bounds(595,210,120, 13), text("Transposition Mode"), fontcolour("white")
combobox    bounds(595,225,120, 18), channel("TransMode"), items("Grain by Grain","Continuous"), value(1),fontcolour("white")

image       bounds(730,202,260,75), colour(0,0,0,0), outlinecolour("grey"), outlinethickness(1), shape("sharp"), plant("GrainEnv"), { 
label       bounds(  0,  3,260, 8), text("G  R  A  I  N     E  N  V  E  L  O  P  E"), fontcolour("white")
rslider     bounds( 10, 13, 60, 60), channel("dur"),     range(0.01, 2,    0.1, 0.5,0.0001),                    text("Duration"),  $RSliderStyle
rslider     bounds( 70, 13, 60, 60), channel("ris"),     range(0.001,0.2,  0.01,0.5,0.0001),  text("Rise"),      $RSliderStyle
rslider     bounds(130, 13, 60, 60), channel("dec"),     range(0.001,0.2,  0.01,0.5,0.0001),                    text("Decay"),     $RSliderStyle
rslider     bounds(190, 13, 60, 60), channel("band"),    range(0,    100,  10,  0.5,0.0001),                    text("Bandwidth"), $RSliderStyle
}

image      bounds(995,202,130,75), colour(0,0,0,0), outlinecolour("grey"), outlinethickness(1), shape("sharp"), plant("envelope"), { 
label      bounds(  0,  3,130, 8), text("E   N   V   E   L   O   P   E"), fontcolour("white")
rslider    bounds(  5, 13, 60, 60), channel("AttTim"),    range(0, 5, 0, 0.5, 0.001),       text("Attack"), $RSliderStyle
rslider    bounds( 65, 13, 60, 60), channel("RelTim"),    range(0.01, 5, 0.05, 0.5, 0.001), text("Release"), $RSliderStyle
}

image       bounds(  5,282,250,75), colour(0,0,0,0), outlinecolour("grey"), outlinethickness(1), shape("sharp"), plant("randomise"), { 
label       bounds(  0,  3,250, 8), text("R  A  N  D  O  M  I  S  E"), fontcolour("white")
rslider     bounds(  5, 13, 60, 60), channel("fmd"),     range(    0, 1,    0), text("Trans.Mod."), $RSliderStyle
rslider     bounds( 65, 13, 60, 60), channel("pmd"),     range(    0, 1,    0.0055,0.25,0.00001),  text("Ptr.Mod."), $RSliderStyle
rslider     bounds(125, 13, 60, 60), channel("DensRnd"), range(    0, 2,    0), text("Dens.Mod."), $RSliderStyle
rslider     bounds(185, 13, 60, 60), channel("AmpRnd"), range(    0, 1,    0), text("Amp.Mod."), $RSliderStyle
}

image      bounds(260,282,190,75), colour(0,0,0,0), outlinecolour("grey"), outlinethickness(1), shape("sharp"), plant("LFO"), { 
label      bounds(  0,  3,190, 8), text("L  F  O"), fontcolour("white")
rslider    bounds(  5, 13, 60, 60), channel("DensLFODep"), range(-2, 2, 0, 1, 0.001),       text("Density"), $RSliderStyle
rslider    bounds( 65, 13, 60, 60), channel("AmpLFODep"),  range(-1, 1, 0, 1, 0.001),  text("Amplitude"),  $RSliderStyle
rslider    bounds(125, 13, 60, 60), channel("LFORte"),     range(0.01, 8, 0.1, 0.5, 0.001),  text("Rate"),  $RSliderStyle
}
                              
image      bounds(455,282,485,75), colour(0,0,0,0), outlinecolour("grey"), outlinethickness(1), shape("sharp"), plant("dual"), { 
label      bounds(  0,  3,485, 8), text("V  O  I  C  E     2"), fontcolour("white")
checkbox   bounds( 10, 10, 70, 15), channel("DualOnOff"), text("On/Off"), fontcolour("white")
rslider    bounds( 60, 13, 60, 60), channel("DensRatio"),   range(0.5,2,1,0.64,0.00001), text("Dens.Ratio"), $RSliderStyle
nslider  bounds(120, 25, 60, 20), channel("DensRatio"),   range(0.5,2,1,0.64,0.00001),  textcolour("white"), trackercolour(190,170,130), outlinecolour(100,100,100)
rslider    bounds(180, 13, 60, 60), channel("PtrDiff"),   range(-1,1,0,1,0.00001), text("Ptr.Diff."), $RSliderStyle
nslider  bounds(240, 25, 60, 20), channel("PtrDiff"),   range(-1,1,0,1,0.00001), textcolour("white"), trackercolour(190,170,130), outlinecolour(100,100,100)
rslider    bounds(300, 13, 60, 60), channel("TransDiff"),   range(-2,2,0,1,0.00001), text("Trans.Diff."), $RSliderStyle
nslider  bounds(360, 25, 60, 20), channel("TransDiff"),   range(-2,2,0,1,0.00001), textcolour("white"), trackercolour(190,170,130), outlinecolour(100,100,100)
rslider    bounds(420, 13, 60, 60), channel("Delay"),       range(0,1,0,1,0.00001), text("Delay"), $RSliderStyle
}

image      bounds(945,282,130,75), colour(0,0,0,0), outlinecolour("grey"), outlinethickness(1), shape("sharp"), plant("control"), { 
label      bounds(  0,  3,130, 8), text("C   O   N   T   R   O   L"), fontcolour("white")
rslider    bounds(  5, 13, 60, 60), channel("MidiRef"),   range(0,127,60, 1, 1),   text("MIDI Ref."), $RSliderStyle
rslider    bounds( 65, 13, 60, 60), channel("level"),     range(  0,  3.00, 0.7, 0.5, 0.001), text("Level"), $RSliderStyle
}

keyboard bounds(5,365,1120, 75)
}
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-n -+rtmidi=NULL -M0 -dm0
</CsOptions>
                         
<CsInstruments>

sr = 44100
ksmps = 64
nchnls = 2
0dbfs=1

massign	0,3
gichans		init	0
giReady		init	0
gSfilepath	init	""

giTriangle	ftgen	0, 0, 4097,  20, 3

; CURVE USED TO FORM ATTACK AND DECAY PORTIONS OF EACH GRAIN
;              NUM | INIT_TIME | SIZE | GEN_ROUTINE |  PARTIAL_NUMBER_1 | STRENGTH_1 | PHASE_1 | DC_OFFSET_1
giattdec ftgen  0,        0,     524288,     19,             0.5,             0.5,        270,         0.5	; I.E. A RISING SIGMOID

opcode	NextPowerOf2i,i,i
 iInVal	xin
 icount	=	1
 LOOP:
 if 2^icount>iInVal then
  goto DONE
 else
  icount	=	icount + 1
  goto LOOP
 endif
 DONE:
 	xout	2^icount
endop

opcode FileNameFromPath,S,S		; Extract a file name (as a string) from a full path (also as a string)
 Ssrc	xin				; Read in the file path string
 icnt	strlen	Ssrc			; Get the length of the file path string
 LOOP:					; Loop back to here when checking for a backslash
 iasc	strchar Ssrc, icnt		; Read ascii value of current letter for checking
 if iasc==92 igoto ESCAPE		; If it is a backslash, escape from loop
 loop_gt	icnt,1,0,LOOP		; Loop back and decrement counter which is also used as an index into the string
 ESCAPE:				; Escape point once the backslash has been found
 Sname	strsub Ssrc, icnt+1, -1		; Create a new string of just the file name
	xout	Sname			; Send it back to the caller instrument
endop

instr	1
 kRampUp	linseg	0,0.001,1
 gkport		chnget	"port"
 gkloop		chnget	"loop"
 gkPlayStop	chnget	"PlayStop"
 gkPhsMode	chnget	"PhsMode"
 gkPhsMode	init	1
 gklevel	chnget	"level"
 gklevel	port	gklevel,0.05
 gkpch		chnget	"pch" 
 gkspd		chnget	"spd"
 gkfreeze	chnget	"freeze"
 gkrange	chnget	"range"
 gkshape	chnget	"shape"
 gkTransMode	chnget	"TransMode"
 gkTransMode	init	1
 gkOctDiv	chnget	"OctDiv"
 gkband		chnget	"band"
 gkris		chnget	"ris"
 gkdec		chnget	"dec"
 gkdur		chnget	"dur"   
 gkfmd		chnget	"fmd"
 gkpmd		chnget	"pmd"
 gkDensRnd	chnget	"DensRnd"
 gkAmpRnd	chnget	"AmpRnd"
 gkDensLFODep	chnget	"DensLFODep"
 gkAmpLFODep	chnget	"AmpLFODep"
 gkLFORte	chnget	"LFORte"
 gkDualOnOff	chnget	"DualOnOff"
 gkDensRatio	chnget	"DensRatio"
 gkPtrDiff	chnget	"PtrDiff"
 gkPtrDiff	port	gkPtrDiff,0.1
 gkTransDiff	chnget	"TransDiff"
 gkDelay	chnget	"Delay"
 gkDelay	port	gkDelay,0.1
      
 if changed(gkPhsMode)==1 then
  if gkPhsMode==1 then
   	chnset	"visible(0)", "spdID"
   	chnset	"visible(0)", "freezeID"
   	chnset	"visible(0)", "rangeID"
   	chnset	"visible(0)", "shapeID"
   	chnset	"visible(0)", "shapelabelID"
   	chnset	"visible(1)", "portID"
  elseif gkPhsMode==2 then
   	chnset	"visible(1)", "spdID"
   	chnset	"visible(1)", "freezeID"
   	chnset	"visible(1)", "rangeID"
   	chnset	"visible(1)", "shapeID"
   	chnset	"visible(1)", "shapelabelID"
   	chnset	"visible(0)", "portID"
  endif
 endif
          
 gSfilepath	chnget	"filename"
 kNewFileTrg	changed	gSfilepath	; if a new file is loaded generate a trigger
 if kNewFileTrg==1 then				; if a new file has been loaded...
  event	"i",99,0,0					; call instrument to update sample storage function table 
 endif  

 /* START/STOP SOUNDING INSTRUMENT */
 ktrig	trigger	gkPlayStop,0.5,0
 schedkwhen	ktrig,0,0,2,0,-1

 /* MOUSE SCRUBBING */
 gkMOUSE_DOWN_RIGHT	chnget	"MOUSE_DOWN_RIGHT"	; Read in mouse left click status
 kStartScrub		trigger	gkMOUSE_DOWN_RIGHT,0.5,0
 if gkMOUSE_DOWN_RIGHT==1 then
  if kStartScrub==1 then 
   reinit RAMP_FUNC
  endif
  RAMP_FUNC:
  krampup	linseg	0,0.001,1
  rireturn
  kMOUSE_X	chnget	"MOUSE_X"
  kMOUSE_X	portk	kMOUSE_X,gkport*kRampUp
  kMOUSE_Y	chnget	"MOUSE_Y"
  kMOUSE_X	=	(kMOUSE_X - 5) / 1120
  kMOUSE_Y	portk	1 - ((kMOUSE_Y - 5) / 175), krampup*0.05		; SOME SMOOTHING OF DENSITY CHANGES VIA THE MOUSE ENHANCES PERFORMANCE RESULTS. MAKE ANY ADJUSTMENTS WITH ADDITIONAL CONSIDERATION OF guirefresh VALUE 
  gkphs		limit	kMOUSE_X,0,1
  gkdens	limit	((kMOUSE_Y^3) * 502) - 2, 0, 500
  schedkwhen	kStartScrub,0,0,2,0,-1
 else
  gkphs		chnget	"phs"
  gkphs		portk	gkphs,gkport*kRampUp
  gkdens	chnget	"dens"
 endif
 
endin

instr	99	; load sound file
 gichans	filenchnls	gSfilepath			; derive the number of channels (mono=1,stereo=2) in the sound file
 iFtlen		NextPowerOf2i	filelen:i(gSfilepath)*sr
 gitableL	ftgen	1,0,iFtlen,1,gSfilepath,0,0,1
 if gichans==2 then
  gitableR	ftgen	2,0,iFtlen,1,gSfilepath,0,0,2
 endif
 giReady 	=	1					; if no string has yet been loaded giReady will be zero
 Smessage sprintfk "file(%s)", gSfilepath			; print sound file to viewer
 chnset Smessage, "filer1"

 /* WRITE FILE NAME TO GUI */
 Sname FileNameFromPath	gSfilepath				; Call UDO to extract file name from the full path
 Smessage sprintfk "text(%s)",Sname
 chnset Smessage, "stringbox"

endin

instr	2	; triggered by 'play/stop' button
 if gkPlayStop==0&&gkMOUSE_DOWN_RIGHT==0 then
  turnoff
 endif
 if giReady = 1 then						; i.e. if a file has been loaded
  
  /* ENVELOPE */
  iAttTim	chnget	"AttTim"				; read in widgets
  iRelTim	chnget	"RelTim"
  if iAttTim>0 then						; is amplitude envelope attack time is greater than zero...
   kenv	linsegr	0,iAttTim,1,iRelTim,0				; create an amplitude envelope with an attack, a sustain and a release segment (senses realtime release)
  else            
   kenv	linsegr	1,iRelTim,0					; create an amplitude envelope with a sustain and a release segment (senses realtime release)
  endif
  kenv	expcurve	kenv,8					; remap amplitude value with a more natural curve
  aenv	interp		kenv					; interpolate and create a-rate envelope

  kporttime	linseg	0,0.001,0.05				; portamento time function. (Rises quickly from zero to a held value.)

  kSwitch		changed	gkTransMode
  if	kSwitch==1	then		;IF I-RATE VARIABLE CHANGE TRIGGER IS '1'...
  	reinit	START			;BEGIN A REINITIALISATION PASS FROM LABEL 'START'
  endif
  START:

  kPmdRnd	unirand	gkpmd
  
  if gkPhsMode==1||gkMOUSE_DOWN_RIGHT==1 then
   kptr		portk	gkphs+kPmdRnd, kporttime	; PORTAMENTO IS APPLIED TO SMOOTH VALUE CHANGES VIA THE FLTK SLIDERS
   kptr		mirror	kptr,0,1
   kptr		=	kptr * (nsamp(1)/ftlen(1))
   aptr		interp	kptr				; A NEW A-RATE VARIABLE (aptr) IS CREATED BASE ON kptr
  else
   if gkshape==1 then
    kptr		phasor	(gkspd * sr * (1-gkfreeze))/(nsamp(1) * gkrange)
   elseif gkshape==2 then
    kptr		oscili	1,(gkspd * sr * (1-gkfreeze))/(nsamp(1) * gkrange),giTriangle
   elseif gkshape==3 then
    kptr		oscili	0.5,(gkspd * sr * (1-gkfreeze))/(nsamp(1) * gkrange)
    kptr		+=		0.5
   endif
   kptr	=	kptr * (nsamp(1)/ftlen(1)) * gkrange
   kptr	+=	(gkphs+kPmdRnd) * (nsamp(1)/ftlen(1))
   kptr	mirror	kptr,0,nsamp(1)/ftlen(1)
  endif
  aptr		interp	kptr
  
  iNumOverLaps	=	2000
  itotdur		=	3600
  
  kPchRnd	bexprnd	gkfmd					; random pitch
  kpch		=	gkpch * octave(kPchRnd)

  kRndTrig	init	1
  ;kdens		init	30					; set to initial widget value
  kDensRnd	bexprnd	gkDensRnd	;randomh	-gkDensRnd, gkDensRnd, kdens
  kdens	=	gkdens * octave(kDensRnd)
  kRndTrig	metro	kdens

  /* amplitude random modulation */
  kAmpRnd	random	0,gkAmpRnd
  klevel	=		gklevel-sqrt(kAmpRnd)

  /* LFO */
  kDensLFO	poscil	gkDensLFODep,gkLFORte
  kdens	=		kdens * octave(kDensLFO)
  kAmpLFO	poscil	gkAmpLFODep*0.5,gkLFORte
  klevel	*=		1-((kAmpLFO+(abs(gkAmpLFODep)*0.5))^2)

  if gichans==1 then						; if mono...
  a1	fog	klevel, kdens, kpch, aptr, gkOctDiv, gkband, gkris, gkdur, gkdec, iNumOverLaps, 1, giattdec, itotdur, 0, i(gkTransMode)-1, 1
   if gkDualOnOff==1 then
    a1b	fog	klevel, kdens*gkDensRatio, kpch*octave(gkTransDiff), aptr+(gkPtrDiff*nsamp(1)/ftlen(1)), gkOctDiv, gkband, gkris, gkdur, gkdec, iNumOverLaps, 1, giattdec, itotdur, 0, i(gkTransMode)-1, 1
    if gkDelay>0 then
     a1b	vdelay	a1b,(gkDelay*1000)/gkdens,1000
    endif
    a1	+=	a1b
   endif
 	outs	a1*aenv,a1*aenv				; send mono audio to both outputs 
  elseif gichans==2 then						; otherwise, if stereo...
   a1	fog	klevel, kdens, kpch, aptr, gkOctDiv, gkband, gkris, gkdur, gkdec, iNumOverLaps, 1, giattdec, itotdur, 0, i(gkTransMode)-1, 1
   a2	fog	klevel, kdens, kpch, aptr, gkOctDiv, gkband, gkris, gkdur, gkdec, iNumOverLaps, 2, giattdec, itotdur, 0, i(gkTransMode)-1, 1
   if gkDualOnOff==1 then
    a1b	fog	klevel, kdens*gkDensRatio, kpch*octave(gkTransDiff), aptr+(gkPtrDiff*nsamp(1)/ftlen(1)), gkOctDiv, gkband, gkris, gkdur, gkdec, iNumOverLaps, 1, giattdec, itotdur, 0, i(gkTransMode)-1, 1
    a2b	fog	klevel, kdens*gkDensRatio, kpch*octave(gkTransDiff), aptr+(gkPtrDiff*nsamp(1)/ftlen(1)), gkOctDiv, gkband, gkris, gkdur, gkdec, iNumOverLaps, 2, giattdec, itotdur, 0, i(gkTransMode)-1, 1
    if gkDelay>0 then
     a1b	vdelay	a1b,(gkDelay*1000)/gkdens,1000
     a2b	vdelay	a2b,(gkDelay*1000)/gkdens,1000
    endif
    a1	+=	a1b
    a2	+=	a2b
   endif
 	outs	a1*aenv,a2*aenv				; send stereo signal to outputs
  endif
  rireturn

 endif
endin

instr	3
 icps	cpsmidi							; read in midi note data as cycles per second
 iamp	ampmidi	1						; read in midi velocity (as a value within the range 0 - 1)
 iAttTim	chnget	"AttTim"				; read in widgets
 iRelTim	chnget	"RelTim"
 iMidiRef	chnget	"MidiRef"
 iFrqRatio		=	icps/cpsmidinn(iMidiRef)	; derive playback speed from note played in relation to a reference note (MIDI note 60 / middle C)

 if giReady = 1 then						; i.e. if a file has been loaded
  iAttTim	chnget	"AttTim"				; read in widgets
  iRelTim	chnget	"RelTim"
  if iAttTim>0 then						; is amplitude envelope attack time is greater than zero...
   kenv	linsegr	0,iAttTim,1,iRelTim,0				; create an amplitude envelope with an attack, a sustain and a release segment (senses realtime release)
  else            
   kenv	linsegr	1,iRelTim,0					; create an amplitude envelope with a sustain and a release segment (senses realtime release)
  endif
  kenv	expcurve	kenv,8					; remap amplitude value with a more natural curve
  aenv	interp		kenv					; interpolate and create a-rate envelope

  kporttime	linseg	0,0.001,0.05				; portamento time function. (Rises quickly from zero to a held value.)

  kSwitch		changed	gkTransMode
  if	kSwitch==1	then		;IF I-RATE VARIABLE CHANGE TRIGGER IS '1'...
  	reinit	START			;BEGIN A REINITIALISATION PASS FROM LABEL 'START'
  endif
  START:

  kPmdRnd	unirand	gkpmd
  
  if gkPhsMode==1 then
   kptr		portk	gkphs+kPmdRnd, kporttime	; PORTAMENTO IS APPLIED TO SMOOTH VALUE CHANGES VIA THE FLTK SLIDERS
   kptr		mirror	kptr,0,1
   kptr		=	kptr * (nsamp(1)/ftlen(1))
   aptr		interp	kptr				; A NEW A-RATE VARIABLE (aptr) IS CREATED BASE ON kptr
  else
   if gkshape==1 then
    kptr		phasor	(gkspd * sr * (1-gkfreeze))/(nsamp(1) * gkrange)
   elseif gkshape==2 then
    kptr		oscili	1,(gkspd * sr * (1-gkfreeze))/(nsamp(1) * gkrange),giTriangle
   elseif gkshape==3 then
    kptr		oscili	0.5,(gkspd * sr * (1-gkfreeze))/(nsamp(1) * gkrange)
    kptr		+=		0.5
   endif
   kptr	=	kptr * (nsamp(1)/ftlen(1)) * gkrange
   kptr	+=	(gkphs+kPmdRnd) * (nsamp(1)/ftlen(1))
   kptr	mirror	kptr,0,nsamp(1)/ftlen(1)
  endif
  aptr		interp	kptr
  
  iNumOverLaps	=	2000
  itotdur		=	3600
  
  kPchRnd	bexprnd	gkfmd					; random pitch
  kpch		=	iFrqRatio * octave(kPchRnd)
  gklevel	*=	iamp
  
  kRndTrig	init	1
  kDensRnd	bexprnd	gkDensRnd
  kdens	=	gkdens * octave(kDensRnd)
  kRndTrig	metro	kdens

  /* amplitude random modulation */
  kAmpRnd	random	0,gkAmpRnd
  klevel	=		gklevel-sqrt(kAmpRnd)

  /* LFO */
  kDensLFO	poscil	gkDensLFODep,gkLFORte
  kdens	=		gkdens * octave(kDensLFO)
  kAmpLFO	poscil	gkAmpLFODep*0.5,gkLFORte
  klevel	*=		1-((kAmpLFO+(abs(gkAmpLFODep)*0.5))^2)
    
  if gichans==1 then						; if mono...
  a1	fog	klevel, kdens, kpch, aptr, gkOctDiv, gkband, gkris, gkdur, gkdec, iNumOverLaps, 1, giattdec, itotdur, 0, i(gkTransMode)-1, 1
   if gkDualOnOff==1 then
    a1b	fog	klevel, kdens*gkDensRatio, kpch*octave(gkTransDiff), aptr+(gkPtrDiff*nsamp(1)/ftlen(1)), gkOctDiv, gkband, gkris, gkdur, gkdec, iNumOverLaps, 1, giattdec, itotdur, 0, i(gkTransMode)-1, 1
    if gkDelay>0 then
     a1b	vdelay	a1b,(gkDelay*1000)/gkdens,1000
    endif
    a1	+=	a1b
   endif
 	outs	a1*aenv,a1*aenv				; send mono audio to both outputs 
  elseif gichans==2 then						; otherwise, if stereo...
   a1	fog	klevel, kdens, kpch, aptr, gkOctDiv, gkband, gkris, gkdur, gkdec, iNumOverLaps, 1, giattdec, itotdur, 0, i(gkTransMode)-1, 1
   a2	fog	klevel, kdens, kpch, aptr, gkOctDiv, gkband, gkris, gkdur, gkdec, iNumOverLaps, 2, giattdec, itotdur, 0, i(gkTransMode)-1, 1
   if gkDualOnOff==1 then
    a1b	fog	klevel, kdens*gkDensRatio, kpch*octave(gkTransDiff), aptr+(gkPtrDiff*nsamp(1)/ftlen(1)), gkOctDiv, gkband, gkris, gkdur, gkdec, iNumOverLaps, 1, giattdec, itotdur, 0, i(gkTransMode)-1, 1
    a2b	fog	klevel, kdens*gkDensRatio, kpch*octave(gkTransDiff), aptr+(gkPtrDiff*nsamp(1)/ftlen(1)), gkOctDiv, gkband, gkris, gkdur, gkdec, iNumOverLaps, 2, giattdec, itotdur, 0, i(gkTransMode)-1, 1
    if gkDelay>0 then
     a1b	vdelay	a1b,(gkDelay*1000)/gkdens,1000
     a2b	vdelay	a2b,(gkDelay*1000)/gkdens,1000
    endif
    a1	+=	a1b
    a2	+=	a2b
   endif
 	outs	a1*aenv,a2*aenv				; send stereo signal to outputs
  endif
  rireturn


 endif

endin

</CsInstruments>  

<CsScore>
i 1 0 [60*60*24*7]
</CsScore>

</CsoundSynthesizer>
; Grain3FilePlayer.csd
; Written by Iain McCurdy, 2015

; File player based around the granualr synthesis opcode, 'grain3'.
; A second voice can be activated (basically another parallel granular synthesiser) with parameter variations of density, transposition, pointer location (Phs) and delay.
; Two modes of playback are available: manual pointer and speed
; The pointer and grain density can also be modulated by clicking and dragging on the waveform view.
;  * This will also start and stop the grain producing instrument.
;  * In click-and-drag mode mouse X position equates to pointer position and mouse Y position equates to grain density. 
; If played from the MIDI keyboard, note number translates to 'Transposition' and key velocity translates to amplitude for the grain stream for that note.

; In 'Pointer' mode pointer position is controlled by the long 'Manual' slider with an optional amount of randomisation determined ny the 'Phs.Mod' slider.  

; Selecting 'Speed' pointer mode bring up some additional controls:
; Speed		-	speed ratio
; Freeze	-	freezes the pointer at its present locations 
; Range		-	ratio of the full sound file duration that will be played back. 1=the_entire_file, 0.5=half_the_file, etc. 
; Shape		-	shape of playback function: 	'Phasor' looping in a single direction
;							'Tri' back and forth looping
; The 'Manual' control functions as an pointer offset when using 'Speed' pointer mode

; Size		-	size of the grains
; Density	-	grains per second
; Transpose	-	transposition in semitones
; Window	-	window shape that envelopes each individual grain

; --Randomisation--
; Trans.Mod.	-	randomisation of transposition (in octaves)
; Ptr.Mod.	-	randomisation of pointer position
; Dens Mod.	-	randomisation of grain density
; Size Mod.	-	randomisation of grain size

; --Density LFO--
; Density	-	depth of LFO modulation of grain density
; Amplitude	-	depth of LFO modulation of grain amplitude
; Size		-	depth of LFO modulation of grain size
; Rate		-	rate of LFO modulation of grain density

; --Voice 2--
; Dens.Ratio	-	ratio of grain density of voice 2 with respect to the main voice (also adjustable using the adjacent number box for precise value input)
; Ptr.Diff.	-	pointer position offset of voice 2 with respect to the main voice (also adjustable using the adjacent number box for precise value input)
; Trans.Diff.	-	transposition offset of voice 2 with respect to the main voice (also adjustable using the adjacent number box for precise value input)
; Delay		-	a delay applied to voice 2 which is defined as a ratio of the gap between grains (therefore delay time will be inversely proportional to garin density)
;			 This is a little like a phase offset for voice 2 with respect to that of the main voice.
;			 When using this control 'Dens.Ratio' should be '1' otherwise continuous temporally shifting between the grains of voice 2 and the main voice will be occurring anyway.

; --Envelope--
; Attack	-	amplitude envelope attack time for the envelope applied to complete notes
; Release	-	amplitude envelope release time for the envelope applied to complete notes

; --Control--
; MIDI Ref.	-	MIDI note that represent unison (no transposition) for when using the MIDI keyboard
; Level	-	output amplitude control


<Cabbage>
form caption("grain3 File Player") size(1040,440), colour(0,0,0), pluginid("G3FP"), guirefresh(16)	; guirefresh value adjusts how often the cabbage valuators are read. This can affect the usability of parameters used in realtime gesturing. style("legacy")

#define RSliderStyle # trackercolour(170,135,130), textcolour("white"), outlinecolour( 50, 15, 10), colour( 90, 45, 50)#

image       bounds(  0,  0,1040,440), colour( 70, 35, 30), outlinecolour("White"), shape("sharp"), line(3)	; main panel colouration    
soundfiler  bounds(  5,  5,1030,175), channel("beg","len"), identchannel("filer1"),  colour(0, 255, 255, 255), fontcolour(160, 160, 160, 255), 
label bounds(7, 5, 560, 14), text(""), align(left), colour(0,0,0,0), fontcolour(200,200,200), identchannel("stringbox")

hslider    bounds(  0,180,1030, 15), channel("phs"),   range( 0,1,0,1,0.0001), $RSliderStyle
label      bounds(  0,195,1030, 13), text("Manual"), fontcolour("white")

filebutton bounds(  5,210,  80, 25), text("Open File","Open File"), fontcolour("white") channel("filename"), shape("ellipse")
checkbox   bounds(  5,240,  95, 20), channel("PlayStop"), text("Play/Stop"), fontcolour("white")
label      bounds(  5,263, 145, 12), text("[or right-click and drag]"), fontcolour("white"), align("left")

label       bounds( 90,215, 75, 13), text("Ptr.Mode"), fontcolour("white")
combobox    bounds( 90,230, 75, 18), channel("PhsMode"), items("Manual", "Speed"), value(2),fontcolour("white")

rslider     bounds(170,215, 60, 60), channel("spd"),     range( -2.00, 2.00, 1), text("Speed"), identchannel("spdID"), visible(0), $RSliderStyle
button      bounds(230,230, 60, 18), channel("freeze"),  colour(  0,  0,  0), text("Freeze","Freeze"), fontcolour:0(70,70,70), fontcolour:1(255,255,255), identchannel("freezeID"), visible(0)
rslider     bounds(290,215, 60, 60), channel("range"),   range(0.01,  1,  1),              text("Range"),  identchannel("rangeID"), visible(0), $RSliderStyle
label       bounds(350,215, 60, 13), text("Shape"), fontcolour("white"), identchannel("shapelabelID")
combobox    bounds(350,230, 60, 18), channel("shape"), items("phasor", "tri."), value(1),fontcolour("white"), identchannel("shapeID"), visible(0)

rslider     bounds(410,215, 60, 60), channel("dur"),     range(0.01,5.00,0.05,0.5,0.001), text("Size"), $RSliderStyle
rslider     bounds(470,215, 60, 60), channel("dens"),    range( 0.2, 500,  20, 0.5),     text("Density"), $RSliderStyle
rslider     bounds(530,215, 60, 60), channel("pch"),     range(-2,2,1,1,0.001),              text("Transpose"), $RSliderStyle
label       bounds(595,210, 75, 13), text("Window"), fontcolour("white")
combobox    bounds(595,225, 75, 18), channel("wfn"), items("Hanning","Perc. 1","Perc. 2","Perc. 3","Gate","Rev Perc. 1","Rev.Perc 2","Rev.Perc 3"), value(1),fontcolour("white")

image       bounds(680,202,260,75), colour(0,0,0,0), outlinecolour("grey"), outlinethickness(1), shape("sharp"), plant("randomise"), { 
label       bounds(  0,  3,260, 8), text("R  A  N  D  O  M  I  S  E"), fontcolour("white")
rslider     bounds( 10, 13, 60, 60), channel("fmd"),     range(    0, 8,    0), text("Trans.Mod."), $RSliderStyle
rslider     bounds( 70, 13, 60, 60), channel("pmd"),     range(    0, 1,    0,0.25,0.00001),  text("Ptr.Mod."), $RSliderStyle
rslider     bounds(130, 13, 60, 60), channel("DensRnd"), range(    0, 2,    0), text("Dens.Mod."), $RSliderStyle
rslider     bounds(190, 13, 60, 60), channel("SizeRnd"), range(    0, 8,    0), text("Size.Mod."), $RSliderStyle
}

image      bounds(  5,282,505,75), colour(0,0,0,0), outlinecolour("grey"), outlinethickness(1), shape("sharp"), plant("dual"), { 
label      bounds(  0,  3,505, 8), text("V  O  I  C  E     2"), fontcolour("white")
checkbox   bounds( 10, 30, 70, 20), channel("DualOnOff"), text("On/Off"), fontcolour("white")
rslider    bounds( 80, 13, 60, 60), channel("DensRatio"),   range(0.5,2,1,0.64,0.00001), text("Dens.Ratio"), $RSliderStyle
nslider  bounds(140, 25, 60, 20), channel("DensRatio"),   range(0.5,2,1,0.64,0.00001),  textcolour("white"), trackercolour(190,170,130), outlinecolour(100,100,100)
rslider    bounds(200, 13, 60, 60), channel("PtrDiff"),   range(-1,1,0,1,0.00001), text("Ptr.Diff."), $RSliderStyle
nslider  bounds(260, 25, 60, 20), channel("PtrDiff"),   range(-1,1,0,1,0.00001), textcolour("white"), trackercolour(190,170,130), outlinecolour(100,100,100)
rslider    bounds(320, 13, 60, 60), channel("TransDiff"),   range(-2,2,0,1,0.00001), text("Trans.Diff."), $RSliderStyle
nslider  bounds(380, 25, 60, 20), channel("TransDiff"),   range(-2,2,0,1,0.00001), textcolour("white"), trackercolour(190,170,130), outlinecolour(100,100,100)
rslider    bounds(440, 13, 60, 60), channel("Delay"),       range(0,1,0,1,0.00001), text("Delay"), $RSliderStyle
}

image      bounds(515,282,250,75), colour(0,0,0,0), outlinecolour("grey"), outlinethickness(1), shape("sharp"), plant("LFO"), { 
label      bounds(  0,  3,250, 8), text("L  F  O"), fontcolour("white")
rslider    bounds(  5, 13, 60, 60), channel("DensLFODep"), range(-4, 4, 0, 1, 0.001),       text("Density"),   $RSliderStyle
rslider    bounds( 65, 13, 60, 60), channel("AmpLFODep"),  range(-1, 1, 0, 1, 0.001),       text("Amplitude"), $RSliderStyle
rslider    bounds(125, 13, 60, 60), channel("SizeLFODep"), range(-2, 2, 0, 1, 0.001),       text("Size"),      $RSliderStyle
rslider    bounds(185, 13, 60, 60), channel("LFORte"),     range(0.01, 8, 0.1, 0.5, 0.001), text("Rate"),      $RSliderStyle
}

image      bounds(770,282,130,75), colour(0,0,0,0), outlinecolour("grey"), outlinethickness(1), shape("sharp"), plant("envelope"), { 
label      bounds(  0,  3,140, 8), text("E   N   V   E   L   O   P   E"), fontcolour("white")
rslider    bounds(  5, 13, 60, 60), channel("AttTim"),    range(0, 5, 0, 0.5, 0.001),       text("Attack"), $RSliderStyle
rslider    bounds( 65, 13, 60, 60), channel("RelTim"),    range(0.01, 5, 0.05, 0.5, 0.001), text("Release"), $RSliderStyle
}

image      bounds(905,282,130,75), colour(0,0,0,0), outlinecolour("grey"), outlinethickness(1), shape("sharp"), plant("control"), { 
label      bounds(  0,  3,140, 8), text("C   O   N   T   R   O   L"), fontcolour("white")
rslider    bounds(  5, 13, 60, 60), channel("MidiRef"),   range(0,127,60, 1, 1),   text("MIDI Ref."), $RSliderStyle
rslider    bounds( 65, 13, 60, 60), channel("level"),     range(  0,  3.00, 0.7, 0.5, 0.001), text("Level"), $RSliderStyle
}
                                  
keyboard bounds(5,360,1030, 75)
}
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-n -+rtmidi=NULL -M0 -dm0
</CsOptions>

<CsInstruments>

sr = 44100
ksmps = 64
nchnls = 2
0dbfs=1

massign	0,3
gichans		init	0
giReady		init	0
gSfilepath	init	""

; WINDOWING FUNCTIONS USED TO DYNAMICALLY SHAPE THE GRAINS
; NUM | INIT_TIME | SIZE | GEN_ROUTINE | PARTIAL_NUM | STRENGTH | PHASE
; GRAIN ENVELOPE WINDOW FUNCTION TABLES:
giwfn1	ftgen	0,  0, 131072,  20,   2, 1 									; HANNING
iTime	=	15000
giwfn2	ftgen	0,  0, 131072,  7,    0, iTime, 1, 131072-iTime, 0			; PERCUSSIVE - GEN08
giwfn3	ftgen	0,  0, 131072,  7,    0, 3072,   1, 128000,    0			; PERCUSSIVE - STRAIGHT SEGMENTS
giwfn4	ftgen	0,  0, 131072,  5,0.001, 3072,   1, 128000,0.001			; PERCUSSIVE - EXPONENTIAL SEGMENTS
giwfn5	ftgen	0,  0, 131072,  7,    0, 1536,   1, 128000,    1, 1536, 0	; GATE - WITH DE-CLICKING RAMP UP AND RAMP DOWN SEGMENTS
giwfn6	ftgen	0,  0, 131072,  7,    0, 131072-iTime, 1, iTime, 0			; REVERSE PERCUSSIVE - GEN08
giwfn7	ftgen	0,  0, 131072,  7,    0, 128000, 1, 3072,      0			; REVERSE PERCUSSIVE - STRAIGHT SEGMENTS
giwfn8	ftgen	0,  0, 131072,  5,0.001, 128000, 1, 3072,  0.001			; REVERSE PERCUSSIVE - EXPONENTIAL SEGMENTS

giTriangle	ftgen	0, 0, 4097,  20, 3


opcode	Grain3b,a,kkkkkkkkkkiiikkiiikk
	kpch, kphs, kspd, kfreeze, krange, kshape, kfmd, kpmd, kdur, kdens, imaxovr, isfn, iwfn, kfrpow, kprpow , iseed, imode, iPhsMode, kDensRnd, kSizeRnd	xin

	if iPhsMode==1 then
	 kptr 		= 	kphs * (nsamp(isfn)/ftlen(isfn))	;MATHEMATICALLY REINTERPRET USER INPUTTED PHASE VALUE INTO A FORMAT THAT IS USABLE AS AN INPUT ARGUMENT  BY THE grain3 OPCODE

	elseif iPhsMode==2 then
	 kspd	*=	1-kfreeze
	 if kshape==1 then
	  kptr		phasor	(kspd*sr)/(nsamp(isfn)*krange)
	  kpch		=	kpch-kspd
	 elseif kshape==2 then
	  kptr		oscili	1,(kspd*sr)/(nsamp(isfn)*krange*2),giTriangle
	  kptrPrev	init	0
	  kpch		=	kptr>kptrPrev?kpch-kspd:kpch+kspd
	  kptrPrev	=	kptr
	 endif
	 kptr		*=	krange	 
	 kptr		mirror	kptr+kphs,0,1

	 kptr 		= 	kptr*(nsamp(isfn)/ftlen(isfn))	;MATHEMATICALLY REINTERPRET USER INPUTTED PHASE VALUE INTO A FORMAT THAT IS USABLE AS AN INPUT ARGUMENT  BY THE grain3 OPCODE

	endif

	kpch 		= 	(sr*kpch)/(ftlen(isfn))		;MATHEMATICALLY REINTERPRET USER INPUTTED PITCH RATIO VALUE INTO A FORMAT THAT IS USABLE AS AN INPUT ARGUMENT BY THE grain3 OPCODE - ftlen(x) FUNCTION RETURNS THE LENGTH OF A FUNCTION TABLE (no. x), REFER TO MANUAL FOR MORE INFO.	
	kfmd		=	(sr*(kfmd*kpch))/ftlen(isfn)

	ktrig		metro	kdens				;TRIGGERS IN SYNC WITH GRAIN GENERATION
	
	kDensRnd 	trandom ktrig,-kDensRnd,kDensRnd	;CREATE A RANDOM OFFSET FACTOR THAT WILL BE APPLIED TO FOR DENSITY
	kdens		*=	octave(kDensRnd)

	kSizeRnd 	trandom ktrig,-kSizeRnd,kSizeRnd	;CREATE A RANDOM OFFSET FACTOR THAT WILL BE APPLIED TO FOR DENSITY
	kdur		*=	octave(kSizeRnd)
	
	asig	grain3	kpch, kptr, kfmd, kpmd, kdur, kdens, imaxovr, isfn, iwfn, kfrpow, kprpow , iseed, imode
	xout	asig

endop

opcode	NextPowerOf2i,i,i
 iInVal	xin
 icount	=	1
 LOOP:
 if 2^icount>iInVal then
  goto DONE
 else
  icount	=	icount + 1
  goto LOOP
 endif
 DONE:
 	xout	2^icount
endop

opcode FileNameFromPath,S,S		; Extract a file name (as a string) from a full path (also as a string)
 Ssrc	xin				; Read in the file path string
 icnt	strlen	Ssrc			; Get the length of the file path string
 LOOP:					; Loop back to here when checking for a backslash
 iasc	strchar Ssrc, icnt		; Read ascii value of current letter for checking
 if iasc==92 igoto ESCAPE		; If it is a backslash, escape from loop
 loop_gt	icnt,1,0,LOOP		; Loop back and decrement counter which is also used as an index into the string
 ESCAPE:				; Escape point once the backslash has been found
 Sname	strsub Ssrc, icnt+1, -1		; Create a new string of just the file name
	xout	Sname			; Send it back to the caller instrument
endop

instr	1
 gkloop		chnget	"loop"
 gkPlayStop	chnget	"PlayStop"

 gkPhsMode	chnget	"PhsMode"
 gkPhsMode	init	1
 gklevel	chnget	"level"
 gklevel	port	gklevel,0.05
 gkdur		chnget	"dur"
 gkpch		chnget	"pch"
 
 gkwfn		chnget	"wfn"
 gkwfn		init	1
 gkspd		chnget	"spd"
 gkfreeze	chnget	"freeze"
 gkrange	chnget	"range"
 gkshape	chnget	"shape"
 gkfmd		chnget	"fmd"
 gkpmd		chnget	"pmd"
 gkDensRnd	chnget	"DensRnd"
 gkSizeRnd	chnget	"SizeRnd"
 gkDensLFODep	chnget	"DensLFODep"
 gkAmpLFODep	chnget	"AmpLFODep"
 gkSizeLFODep	chnget	"SizeLFODep"
 gkLFORte	chnget	"LFORte"
 gkDualOnOff	chnget	"DualOnOff"
 gkDensRatio	chnget	"DensRatio"
 gkPtrDiff	chnget	"PtrDiff"
 gkTransDiff	chnget	"TransDiff"
 gkDelay	chnget	"Delay"
 gkDelay	port	gkDelay,0.1
 
 if changed(gkPhsMode)==1 then
  if gkPhsMode==1 then
   	chnset	"visible(0)", "spdID"
   	chnset	"visible(0)", "freezeID"
   	chnset	"visible(0)", "rangeID"
   	chnset	"visible(0)", "shapeID"
   	chnset	"visible(0)", "shapelabelID"
  elseif gkPhsMode==2 then
   	chnset	"visible(1)", "spdID"
   	chnset	"visible(1)", "freezeID"
   	chnset	"visible(1)", "rangeID"
   	chnset	"visible(1)", "shapeID"
   	chnset	"visible(1)", "shapelabelID"
  endif
 endif
 
 gSfilepath	chnget	"filename"
 kNewFileTrg	changed	gSfilepath		; if a new file is loaded generate a trigger
 if kNewFileTrg==1 then				; if a new file has been loaded...
  event	"i",99,0,0				; call instrument to update sample storage function table 
 endif  

 /* START/STOP SOUNDING INSTRUMENT */
 ktrig	trigger	gkPlayStop,0.5,0
 schedkwhen	ktrig,0,0,2,0,-1

 /* MOUSE SCRUBBING */
 gkMOUSE_DOWN_RIGHT	chnget	"MOUSE_DOWN_RIGHT"	; Read in mouse left click status
 kStartScrub		trigger	gkMOUSE_DOWN_RIGHT,0.5,0
 if gkMOUSE_DOWN_RIGHT==1 then
  if kStartScrub==1 then 
   reinit RAMP_FUNC
  endif
  RAMP_FUNC:
  krampup	linseg	0,0.001,1
  rireturn
  kMOUSE_X	chnget	"MOUSE_X"
  kMOUSE_Y	chnget	"MOUSE_Y"
  kMOUSE_X	=	(kMOUSE_X - 5) / 930
  kMOUSE_Y	portk	1 - ((kMOUSE_Y - 5) / 170), krampup*0.05		; SOME SMOOTHING OF DENSITY CHANGES VIA THE MOUSE ENHANCES PERFORMANCE RESULTS. MAKE ANY ADJUSTMENTS WITH ADDITIONAL CONSIDERATION OF guirefresh VALUE 
  gkphs		limit	kMOUSE_X,0,1
  gkdens	limit	((kMOUSE_Y^3) * 499) + 1, 1, 500
  schedkwhen	kStartScrub,0,0,2,0,-1
 else
  gkphs		chnget	"phs"
  gkdens	chnget	"dens"
 endif
 
endin

instr	99	; load sound file
 gichans	filenchnls	gSfilepath			; derive the number of channels (mono=1,stereo=2) in the sound file
 iFtlen		NextPowerOf2i	filelen:i(gSfilepath)*sr
 gitableL	ftgen	1,0,iFtlen,1,gSfilepath,0,0,1
 if gichans==2 then
  gitableR	ftgen	2,0,iFtlen,1,gSfilepath,0,0,2
 endif
 giReady 	=	1					; if no string has yet been loaded giReady will be zero
 Smessage sprintfk "file(%s)", gSfilepath			; print sound file to viewer
 chnset Smessage, "filer1"

 /* WRITE FILE NAME TO GUI */
 Sname FileNameFromPath	gSfilepath				; Call UDO to extract file name from the full path
 Smessage sprintfk "text(%s)",Sname
 chnset Smessage, "stringbox"

endin

instr	2	; triggered by 'play/stop' button
 if gkPlayStop==0&&gkMOUSE_DOWN_RIGHT==0 then
  turnoff
 endif
 if giReady = 1 then						; i.e. if a file has been loaded
  
  /* ENVELOPE */
  iAttTim	chnget	"AttTim"				; read in widgets
  iRelTim	chnget	"RelTim"
  if iAttTim>0 then							; is amplitude envelope attack time is greater than zero...
   kenv	linsegr	0,iAttTim,1,iRelTim,0		; create an amplitude envelope with an attack, a sustain and a release segment (senses realtime release)
  else            
   kenv	linsegr	1,iRelTim,0					; create an amplitude envelope with a sustain and a release segment (senses realtime release)
  endif
  kenv	expcurve	kenv,8					; remap amplitude value with a more natural curve
  aenv	interp		kenv					; interpolate and create a-rate envelope

  kporttime	linseg	0,0.001,0.05			; portamento time function. (Rises quickly from zero to a held value.)

  kSwitch		changed	gkPhsMode, gkwfn
  if	kSwitch==1	then					; IF I-RATE VARIABLE CHANGE TRIGGER IS '1'...
  	reinit	START							; BEGIN A REINITIALISATION PASS FROM LABEL 'START'
  endif
  START:
  iwfn		=	giwfn1 + i(gkwfn) - 1
  
  /* LFO */
  kDensLFO	poscil	gkDensLFODep,gkLFORte
  kdens	=		gkdens * octave(kDensLFO)
  kAmpLFO	poscil	gkAmpLFODep*0.5,gkLFORte
  klevel	=		gklevel*(1-((kAmpLFO+(abs(gkAmpLFODep)*0.5))^2))
  kSizeLFO	poscil	gkSizeLFODep,gkLFORte
  kdur	=		gkdur * octave(kSizeLFO)
  
  iPhsMode	=	i(gkMOUSE_DOWN_RIGHT)==1?1:i(gkPhsMode)
  
  if gichans==1 then						; if mono...
   a1	Grain3b		gkpch, gkphs, gkspd, gkfreeze, gkrange, gkshape, gkfmd, gkpmd, kdur, kdens, 600, 1, iwfn, -2, -2 , rnd(1000), 8, iPhsMode, gkDensRnd, gkSizeRnd
   if gkDualOnOff==1 then
    a1b	Grain3b		gkpch+gkTransDiff, gkphs+gkPtrDiff, gkspd, gkfreeze, gkrange, gkshape, gkfmd, gkpmd, kdur, kdens*gkDensRatio, 600, 1, iwfn, -2, -2 , rnd(1000), 8, iPhsMode, gkDensRnd, gkSizeRnd
    if gkDelay>0 then
     a1b	vdelay	a1b,(gkDelay*1000)/kdens,5000
    endif
    a1	+=	a1b
   endif
 	outs	a1*aenv*klevel,a1*aenv*klevel		; send mono audio to both outputs 
  elseif gichans==2 then						; otherwise, if stereo...
   a1	Grain3b		gkpch, gkphs, gkspd, gkfreeze, gkrange, gkshape, gkfmd, gkpmd, kdur, kdens, 600, 1, iwfn, -2, -2 , rnd(1000), 8, iPhsMode, gkDensRnd, gkSizeRnd
   a2	Grain3b		gkpch, gkphs, gkspd, gkfreeze, gkrange, gkshape, gkfmd, gkpmd, kdur, kdens, 600, 2, iwfn, -2, -2 , rnd(1000), 8, iPhsMode, gkDensRnd, gkSizeRnd
   if gkDualOnOff==1 then
    a1b	Grain3b		gkpch+gkTransDiff, gkphs+gkPtrDiff, gkspd, gkfreeze, gkrange, gkshape, gkfmd, gkpmd, kdur, kdens*gkDensRatio, 600, 1, iwfn, -2, -2 , rnd(1000), 8, iPhsMode, gkDensRnd, gkSizeRnd
    a2b	Grain3b		gkpch+gkTransDiff, gkphs+gkPtrDiff, gkspd, gkfreeze, gkrange, gkshape, gkfmd, gkpmd, kdur, kdens*gkDensRatio, 600, 2, iwfn, -2, -2 , rnd(1000), 8, iPhsMode, gkDensRnd, gkSizeRnd
    if gkDelay>0 then
     a1b	vdelay	a1b,(gkDelay*1000)/kdens,5000
     a2b	vdelay	a2b,(gkDelay*1000)/kdens,5000
    endif
    a1	+=	a1b
    a2	+=	a2b
   endif
 	outs	a1*aenv*klevel,a2*aenv*klevel		; send stereo signal to outputs
  endif
  rireturn

 endif
endin

instr	3
 icps	cpsmidi									; read in midi note data as cycles per second
 iamp	ampmidi	1								; read in midi velocity (as a value within the range 0 - 1)
 iAttTim	chnget	"AttTim"					; read in widgets
 iRelTim	chnget	"RelTim"
 iMidiRef	chnget	"MidiRef"
 iFrqRatio		=	icps/cpsmidinn(iMidiRef)	; derive playback speed from note played in relation to a reference note (MIDI note 60 / middle C)

 if giReady = 1 then							; i.e. if a file has been loaded
  iAttTim	chnget	"AttTim"					; read in widgets
  iRelTim	chnget	"RelTim"
  if iAttTim>0 then								; is amplitude envelope attack time is greater than zero...
   kenv	linsegr	0,iAttTim,1,iRelTim,0			; create an amplitude envelope with an attack, a sustain and a release segment (senses realtime release)
  else            
   kenv	linsegr	1,iRelTim,0						; create an amplitude envelope with a sustain and a release segment (senses realtime release)
  endif
  kenv	expcurve	kenv,8						; remap amplitude value with a more natural curve
  aenv	interp		kenv						; interpolate and create a-rate envelope
  kporttime	linseg	0,0.001,0.05				; portamento time function. (Rises quickly from zero to a held value.)

  kSwitch		changed	gkPhsMode, gkwfn
  if	kSwitch==1	then						; IF I-RATE VARIABLE CHANGE TRIGGER IS '1'...
  	reinit	START								; BEGIN A REINITIALISATION PASS FROM LABEL 'START'
  endif
  START:
  iwfn		=	giwfn1 + i(gkwfn) - 1

  /* LFO */
  kDensLFO	poscil	gkDensLFODep,gkLFORte
  kdens	=		gkdens * octave(kDensLFO)
  kAmpLFO	poscil	gkAmpLFODep*0.5,gkLFORte
  klevel	=		gklevel*(1-((kAmpLFO+(abs(gkAmpLFODep)*0.5))^2))
  kSizeLFO	poscil	gkSizeLFODep,gkLFORte
  kdur	=		gkdur * octave(kSizeLFO)

  iPhsMode	=	i(gkMOUSE_DOWN_RIGHT)==1?1:i(gkPhsMode)

  if gichans==1 then							; if mono...
   a1	Grain3b		iFrqRatio, gkphs, gkspd, gkfreeze, gkrange, gkshape, gkfmd, gkpmd, kdur, kdens, 600, 1, iwfn, -2, -2 , rnd(1000), 8, iPhsMode, gkDensRnd, gkSizeRnd
   if gkDualOnOff==1 then
    a1b	Grain3b		iFrqRatio+gkTransDiff, gkphs+gkPtrDiff, gkspd, gkfreeze, gkrange, gkshape, gkfmd, gkpmd, kdur, kdens*gkDensRatio, 600, 1, iwfn, -2, -2 , rnd(1000), 8, iPhsMode, gkDensRnd, gkSizeRnd
    if gkDelay>0 then
     a1b	vdelay	a1b,(gkDelay*1000)/kdens,5000
    endif
    a1	+=	a1b
   endif
 	outs	a1*aenv*klevel,a1*aenv*klevel		; send mono audio to both outputs 
  elseif gichans==2 then						; otherwise, if stereo...
   a1	Grain3b		iFrqRatio, gkphs, gkspd, gkfreeze, gkrange, gkshape, gkfmd, gkpmd, kdur, kdens, 600, 1, iwfn, -2, -2 , rnd(1000), 8, iPhsMode, gkDensRnd, gkSizeRnd
   a2	Grain3b		iFrqRatio, gkphs, gkspd, gkfreeze, gkrange, gkshape, gkfmd, gkpmd, kdur, kdens, 600, 2, iwfn, -2, -2 , rnd(1000), 8, iPhsMode, gkDensRnd, gkSizeRnd
   if gkDualOnOff==1 then
    a1b	Grain3b		iFrqRatio+gkTransDiff, gkphs+gkPtrDiff, gkspd, gkfreeze, gkrange, gkshape, gkfmd, gkpmd, kdur, kdens*gkDensRatio, 600, 1, iwfn, -2, -2 , rnd(1000), 8, iPhsMode, gkDensRnd, gkSizeRnd
    a2b	Grain3b		iFrqRatio+gkTransDiff, gkphs+gkPtrDiff, gkspd, gkfreeze, gkrange, gkshape, gkfmd, gkpmd, kdur, kdens*gkDensRatio, 600, 2, iwfn, -2, -2 , rnd(1000), 8, iPhsMode, gkDensRnd, gkSizeRnd
    if gkDelay>0 then
     a1b	vdelay	a1b,(gkDelay*1000)/kdens,5000
     a2b	vdelay	a2b,(gkDelay*1000)/kdens,5000
    endif
    a1	+=	a1b
    a2	+=	a2b
   endif
 	outs	a1*aenv*klevel,a2*aenv*klevel		; send stereo signal to outputs
  endif
  rireturn

 endif

endin

</CsInstruments>  

<CsScore>
i 1 0 [60*60*24*7]
</CsScore>

</CsoundSynthesizer>
; LoscilFilePlayer.csd
; Written by Iain McCurdy, 2014
; 
; Load a user selected sound file into a GEN 01 function table and plays it back using loscil3. 
; This file player is best suited for polyphonic playback and is less well suited for the playback of very long sound files .
; 
; The sound file can be played back using the Play/Stop button (and the 'Transpose' / 'Speed' buttons to implement pitch/speed change)
;  or it can be played back using the MIDI keyboard.
; 
; The loop points can be set either by using the loop 'Start' and 'End' sliders or by clicking and dragging on the waveform view -
;  - loscil will take the values from the last control input moved.

<Cabbage>
form caption("Loscil File Player") size(685,340), colour(0,0,0) pluginid("Losc") style("legacy")
image                    bounds(  0,  0,685,340), colour(30, 70, 30), outlinecolour("White"), line(3), shape("sharp")	; main panel colouration    

soundfiler bounds(  5,  5,675,175), channel("beg","len"), identchannel("filer1"),  colour(0, 255, 255, 255), fontcolour(160, 160, 160, 255), 

image    bounds(  0,180,685,160), colour(0,0,0,0), outlinecolour("white"), line(2), shape("sharp"), plant("controls"){
filebutton bounds(  5, 10, 80, 25), text("Open File","Open File"), fontcolour("white") channel("filename"), shape("ellipse")
checkbox   bounds(  5, 40, 95, 25), channel("PlayStop"), text("Play/Stop"), colour("yellow"), fontcolour("white")

label      bounds(110, 12, 80, 12), text("Looping Mode"), fontcolour("white")
combobox   bounds(110, 25, 80, 20), channel("loop"), items("None", "Forward", "Fwd./Bwd."), value(1), fontcolour("white")

label      bounds(241,  4, 43, 8), text("L   O   O   P"), fontcolour("white")
rslider    bounds(205, 15, 60, 60), channel("LoopStart"), range(0, 1, 0),                   colour( 50, 90, 50), text("Start"),     textcolour("white")
rslider    bounds(260, 15, 60, 60), channel("LoopEnd"),   range(0, 1, 1),                   colour( 50, 90, 50), text("End"),       textcolour("white")
line       bounds(320, 10,  2, 65), colour("Grey")

label      bounds(357,  4, 53, 8), text("S   P   E   E   D"), fontcolour("white")
rslider    bounds(325, 15, 60, 60), channel("transpose"), range(-24, 24, 0,1,1),            colour( 50, 90, 50), text("Transpose"), textcolour("white")
rslider    bounds(380, 15, 60, 60), channel("speed"),     range( 0, 4.00, 1, 0.5),          colour( 50, 90, 50), text("Speed"),     textcolour("white")
line       bounds(440, 10,  2, 65), colour("Grey")

label      bounds(456,  4, 90, 8), text("E   N   V   E   L   O   P   E"), fontcolour("white")
rslider    bounds(445, 15, 60, 60), channel("AttTim"),    range(0, 5, 0, 0.5, 0.001),       colour( 50, 90, 50), text("Att.Tim"),   textcolour("white")
rslider    bounds(500, 15, 60, 60), channel("RelTim"),    range(0.01, 5, 0.05, 0.5, 0.001), colour( 50, 90, 50), text("Rel.Tim"),   textcolour("white")
line       bounds(560, 10,  2, 65), colour("Grey")

label      bounds(580,  4, 80, 8), text("C   O   N   T   R   O   L"), fontcolour("white")
rslider    bounds(565, 15, 60, 60), channel("MidiRef"),   range(0,127,60, 1, 1),            colour( 50, 90, 50), text("MIDI Ref."), textcolour("white")
rslider    bounds(620, 15, 60, 60), channel("level"),     range(  0,  3.00, 1, 0.5),        colour( 50, 90, 50), text("Level"),     textcolour("white")

keyboard bounds(5, 80, 675, 75)
}
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-n -+rtmidi=NULL -M0 -dm0
</CsOptions>

<CsInstruments>

sr = 44100
ksmps = 64
nchnls = 2
0dbfs = 1

		massign	0,3
gichans		init	0
giReady		init	0
gSfilepath	init	""
gkTabLen	init	2

instr	1
 gkloop		chnget	"loop"
 kLoopStart	chnget	"LoopStart"
 kLoopEnd	chnget	"LoopEnd"
 kLoopEnd	limit	kLoopEnd,kLoopStart+0.01,1	; limit loop end to prevent crashes
 kbeg		chnget	"beg"				; click and drag
 klen		chnget	"len"				;  "

 ; loop points defined by sliders or click and drag...
 kTrigSlid	changed	kLoopStart,kLoopEnd	
 kTrigCAD	changed	kbeg,klen
 if kTrigSlid==1 then
  gkLoopStart	=	kLoopStart
  gkLoopEnd	=	kLoopEnd
 elseif kTrigCAD==1 then
  gkLoopStart	=	kbeg/gkTabLen
  gkLoopEnd	=	(kbeg+klen)/gkTabLen
 endif


 gkPlayStop	chnget	"PlayStop"
 gktranspose	chnget	"transpose"
 gkspeed	chnget	"speed"
 gklevel	chnget	"level"

 gSfilepath	chnget	"filename"
 kNewFileTrg	changed	gSfilepath		; if a new file is loaded generate a trigger
 if kNewFileTrg==1 then				; if a new file has been loaded...
  event	"i",99,0,0				; call instrument to update sample storage function table 
 endif  
 
 ktrig	trigger	gkPlayStop,0.5,0		; if play button changes to 'play', generate a trigger
 schedkwhen	ktrig,0,0,2,0,-1		; start instr 2 playing a held note

 ktrig1	changed	gktranspose			; if 'transpose' button is changed generate a '1' trigger
 ktrig2	changed	gkspeed				; if 'speed' button is changed generate a '1' trigger
 
 if ktrig1==1 then				; if transpose control has been changed...
  chnset	semitone(gktranspose),"speed"	; set speed according to transpose value
 elseif ktrig2==1 then		; if speed control has been changed...
  chnset	log2(gkspeed)*12,"transpose"	; set transpose control according to speed value
 endif

endin

instr	99	; load sound file
 gitable	ftgen	1,0,0,1,gSfilepath,0,0,0		; load sound file into a GEN 01 function table 
 gichans	filenchnls	gSfilepath			; derive the number of channels (mono=1,stereo=2) in the sound file
 giReady 	=	1					; if no string has yet been loaded giReady will be zero
 gkTabLen	init		ftlen(gitable)/gichans		; table length in sample frames
 Smessage sprintfk "file(%s)", gSfilepath			; print sound file to viewer
 chnset Smessage, "filer1"	
endin

instr	2	; sample triggered by 'play/stop' button
 if gkPlayStop==0 then
  turnoff
 endif
 ktrig changed	gkloop,gkLoopStart,gkLoopEnd
 if ktrig==1 then
  reinit RESTART
 endif
 RESTART:
 if giReady = 1 then						; i.e. if a file has been loaded
  iAttTim	chnget	"AttTim"				; read in widgets
  iRelTim	chnget	"RelTim"
  if iAttTim>0 then						; is amplitude envelope attack time is greater than zero...
   kenv	linsegr	0,iAttTim,1,iRelTim,0				; create an amplitude envelope with an attack, a sustain and a release segment (senses realtime release)
  else
   kenv	linsegr	1,iRelTim,0					; create an amplitude envelope with a sustain and a release segment (senses realtime release)
  endif
  kenv	expcurve	kenv,8					; remap amplitude value with a more natural curve
  aenv	interp		kenv					; interpolate and create a-rate envelope
  kporttime	linseg	0,0.001,0.05				; portamento time function. (Rises quickly from zero to a held value.)
  kspeed	portk	gkspeed,kporttime			; apply portamento smoothing to changes in speed
  klevel	portk	gklevel,kporttime			; apply portamento smoothing to changes in level
  if gichans==1 then						; if mono...
   a1	loscil3	klevel,kspeed,gitable,1,i(gkloop)-1,nsamp(gitable)*i(gkLoopStart),nsamp(gitable)*i(gkLoopEnd)	; use a mono loscil3
 	outs	a1*aenv,a1*aenv					; send mono audio to both outputs 
  elseif gichans==2 then						; otherwise, if stereo...
   a1,a2	loscil3	klevel,kspeed,gitable,1,i(gkloop)-1,nsamp(gitable)*i(gkLoopStart),nsamp(gitable)*i(gkLoopEnd)	; use stereo loscil3
 	outs	a1*aenv,a2*aenv					; send stereo signal to outputs
  endif               
 endif
endin

instr	3	; sample triggered by midi note
 icps	cpsmidi							; read in midi note data as cycles per second
 iamp	ampmidi	1						; read in midi velocity (as a value within the range 0 - 1)
 iMidiRef	chnget	"MidiRef"

 if giReady = 1 then						; i.e. if a file has been loaded
  iAttTim	chnget	"AttTim"				; read in widgets
  iRelTim	chnget	"RelTim"
  if iAttTim>0 then						; is amplitude envelope attack time is greater than zero...
   kenv	linsegr	0,iAttTim,1,iRelTim,0				; create an amplitude envelope with an attack, a sustain and a release segment (senses realtime release)
  else
   kenv	linsegr	1,iRelTim,0					; create an amplitude envelope with a sustain and a release segment (senses realtime release)
  endif
  kenv	expcurve	kenv,8					; remap amplitude value with a more natural curve
  aenv	interp		kenv					; interpolate and create a-rate envelope
  kporttime	linseg	0,0.001,0.05				; portamento time function. (Rises quickly from zero to a held value.)
  ispeed	=	icps/cpsmidinn(iMidiRef)	; derive playback speed from note played in relation to a reference note (MIDI note 60 / middle C)
  klevel	portk	gklevel,kporttime		; apply portamento smoothing to changes in level
  if gichans==1 then						; if mono...
   a1	loscil3	klevel*aenv*iamp,ispeed,gitable,1,i(gkloop)-1,nsamp(gitable)*i(gkLoopStart),nsamp(gitable)*i(gkLoopEnd)	; use a mono loscil3
 	outs	a1,a1						; send mono audio to both outputs 
  elseif gichans==2 then						; otherwise, if stereo...
   a1,a2	loscil3	klevel*aenv*iamp,ispeed,gitable,1,i(gkloop)-1,nsamp(gitable)*i(gkLoopStart),nsamp(gitable)*i(gkLoopEnd)	; use stereo loscil3
 	outs	a1,a2						; send stereo signal to outputs
  endif
 endif
endin
 
</CsInstruments>  

<CsScore>
i 1 0 [60*60*24*7]
</CsScore>

</CsoundSynthesizer>
; MincerFilePlayer.csd
; Written by Iain McCurdy, 2014

; Three modes of playback are offered:
; 1. Manual Pointer
;     Pointer position is determined by the long horizontal slider 'Manual Pointer'.
; 2. Mouse Scrubber
;     Pointer position is determined by the mouse's X position over the waveform view. Playback is also started and stopped using right-click.
; 3. Loop Region
;     A region that has been highlighted using left-click and drag is looped using a method and speed chosen in the 'LOOP REGION' GUI area.
;     Speed can be random-modulated by increasing Mod.Range. The nature of the modulation is changed using 'Rate 1' and 'Rate 2'. The random function generator is jspline.

; Transpose can be expressed either in semitones or a simple ratio. Select mode 'Semitones' or 'Ratio'

; MOD. POINTER section contains controls which modulate the pointer position using a 'sample-and-hold' type random function
; 
 
; All three of the above methods are playable from a MIDI keyboard (first activate the 'MIDI' checkbox).
;  Transposition for MIDI activated notes is governed bu both the MIDI key played and the setting for 'Transpose'


<Cabbage>
form caption("Mincer File Player") size(1055,370), colour(  0,  0,  0) pluginid("Minc"), guirefresh(10) style("legacy")
image                    bounds(  0,  0,1055,370), colour( 50,100,100), outlinecolour("White"), line(3), shape("sharp")

soundfiler bounds(  5,  5,1045,150), channel("beg","len"), channel("pos1","end1"), identchannel("filer1"),  colour(0, 255, 255, 255), fontcolour(160, 160, 160, 255), 
label bounds(6, 4, 560, 14), text(""), align(left), colour(0,0,0,0), fontcolour(200,200,200), identchannel("stringbox")

hslider    bounds(  0,150,1055, 30), channel("pointer"), range( 0,  1.00, 0.1), colour( 40, 80, 80),  trackercolour("white"), fontcolour("white")
label      bounds(  0,172,1055, 12), text("Manual Pointer"), fontcolour("white")

filebutton bounds(  5,185, 80, 22), text("Open File","Open File"), fontcolour("white"), channel("filename"), shape("ellipse")

checkbox bounds(  5,218,120, 12), text("Manual Pointer"), channel("r1"), fontcolour("white"), colour(yellow), radiogroup(1)
checkbox bounds(  5,232,120, 12), text("Mouse Scrubber"), channel("r2"), fontcolour("white"), colour(yellow), radiogroup(1), value(1) 
label    bounds( 19,246,100, 10), text("[right click and drag]"), fontcolour("white"), align("left")
checkbox bounds(  5,256,120, 12), text("Loop Region"),    channel("r3"), fontcolour("white"), colour(yellow), radiogroup(1) 
label    bounds( 19,270,100, 10), text("[left click and drag]"), fontcolour("white"), align("left")

checkbox   bounds(125,223, 60, 12), channel("lock"), text("Lock"), colour("yellow"), fontcolour("white"), value(1)
checkbox   bounds(125,243, 60, 12), channel("MIDI"), text("MIDI"), colour("yellow"), fontcolour("white")

label      bounds(105,183, 48, 12), text("FFT Size"), fontcolour("white")
combobox   bounds( 95,195, 70, 20), channel("FFTSize"), text("32768", "16384", "8192", "4096", "2048", "1024", "512", "256", "128", "64", "32", "16", "8", "4"), value(5), fontcolour("white")

combobox   bounds(170,175, 80, 20), text("Semitone","Ratio"), channel("IntervalMode"),       value(1)

image      bounds(175,200, 70, 70), colour(0,0,0,0), plant("Semitones"), identchannel("SemitonesPlant_ident"), visible(1) {
rslider    bounds(  0,  0, 70, 70), channel("Semitones"), range(-48, 48, 0,1,1), colour( 40, 80, 80), trackercolour("white"), text("Semitones"), textcolour("white")
}

image      bounds(175,200, 70, 70), colour(0,0,0,0), plant("Ratio"), identchannel("RatioPlant_ident"), visible(0) {
nslider bounds( 20,  0, 25, 18), channel("Numerator"),        range(1,99,3,1,1)
image     bounds( 15, 26, 35,  1), shape("sharp") 
nslider bounds( 20, 35, 25, 18), channel("Denominator"),      range(1,99,2,1,1)
}


rslider    bounds(240,200, 70, 70), channel("portamento"),range(0, 20,0.05,0.5,0.01), colour( 40, 80, 80), trackercolour("white"), text("Port.Time"), textcolour("white")

image      bounds(320,188, 305,90), colour(0,0,0,0), outlinecolour("silver"), outlinethickness(1), shape("sharp"), plant("LoopRegion") {
label      bounds(  0,  2, 305,10), text("L   O   O   P       R   E   G   I   O   N"), fontcolour("white")
label      bounds( 10, 24, 85, 12), text("Shape"), fontcolour("white")
combobox   bounds( 10, 37, 85, 20), channel("LoopMode"), text("Forward","Backward","Triangle","Sine"), value(1), fontcolour("white")
rslider    bounds(100, 17, 70, 70), channel("Speed"), range(-2, 2, 1,1,0.001),            colour( 40, 80, 80)), trackercolour("white"), text("Speed"), textcolour("white")
nslider  bounds(170, 35, 60, 30), channel("ModRange"), range(0,2,0,1,0.001),  colour(  0,  0,  0), text("Mod.Range"), textcolour("white")
nslider  bounds(235, 20, 60, 30), channel("Rate1"),    range(0,30,1,1,0.001), colour(  0,  0,  0), text("Rate 1"),    textcolour("white")
nslider  bounds(235, 50, 60, 30), channel("Rate2"),    range(0,30,2,1,0.001), colour(  0,  0,  0), text("Rate 2"),    textcolour("white")
}

image      bounds(630,188, 145,90), colour(0,0,0,0), outlinecolour("silver"), outlinethickness(1), shape("sharp"), plant("ModPtr") {
label      bounds(  0,  2, 135,10), text("M  O  D.     P  O  I  N  T  E  R"), fontcolour("white")
nslider  bounds( 10, 35, 60, 30), channel("PtrModRange"), range(0,1,0,1,0.001),  colour(  0,  0,  0), text("Mod.Range"), textcolour("white")
nslider  bounds( 75, 20, 60, 30), channel("PtrRate1"),    range(0,500,1,1,0.001), colour(  0,  0,  0), text("Rate 1"),    textcolour("white")
nslider  bounds( 75, 50, 60, 30), channel("PtrRate2"),    range(0,500,2,1,0.001), colour(  0,  0,  0), text("Rate 2"),    textcolour("white")
}

image      bounds(785,200,265, 70), colour(0,0,0,0), plant("output") {
rslider    bounds(  0,  0, 70, 70), channel("AttTim"),    range(0, 5, 0, 0.5, 0.001),       colour( 40, 80, 80),  trackercolour("white"), text("Att.Tim"),   textcolour("white")
rslider    bounds( 65,  0, 70, 70), channel("RelTim"),    range(0.01, 5, 0.05, 0.5, 0.001), colour( 40, 80, 80),  trackercolour("white"), text("Rel.Tim"),   textcolour("white")
rslider    bounds(130,  0, 70, 70), channel("MidiRef"),   range(0,127,60, 1, 1),            colour( 40, 80, 80),  trackercolour("white"), text("MIDI Ref."), textcolour("white")
rslider    bounds(195,  0, 70, 70), channel("level"),     range(  0,  1.00, 1, 0.5),        colour( 40, 80, 80),  trackercolour("white"), text("Level"),     textcolour("white")
}

keyboard bounds(  5,290,1045, 75)
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-n -+rtmidi=NULL -M0 -d
</CsOptions>

<CsInstruments>

sr = 44100
ksmps = 64
nchnls = 2
0dbfs=1

massign	0,3

gichans		init	0		; 
giReady		init	0		; flag to indicate function table readiness

giFFTSizes[]	array	32768, 16384, 8192, 4096, 2048, 1024, 512, 256, 128, 64, 32, 16, 8, 4	; an array is used to store FFT window sizes
giTriangle		ftgen	0,0,4097,7,0,2048,1,2048,0
giRectSine		ftgen	0,0,4097,19,1,0.5,0,1
gSfilepath	init	""
gkFileLen	init	0

opcode FileNameFromPath,S,S		; Extract a file name (as a string) from a full path (also as a string)
 Ssrc	xin				; Read in the file path string
 icnt	strlen	Ssrc			; Get the length of the file path string
 LOOP:					; Loop back to here when checking for a backslash
 iasc	strchar Ssrc, icnt		; Read ascii value of current letter for checking
 if iasc==92 igoto ESCAPE		; If it is a backslash, escape from loop
 loop_gt	icnt,1,0,LOOP		; Loop back and decrement counter which is also used as an index into the string
 ESCAPE:				; Escape point once the backslash has been found
 Sname	strsub Ssrc, icnt+1, -1		; Create a new string of just the file name
	xout	Sname			; Send it back to the caller instrument
endop

instr	1
 /* PORTAMENTO TIME FUNCTION */
 kporttimeW	chnget	"portamento"
 krampup	linseg	0,0.001,1
 kporttime	=	krampup * kporttimeW	

 /* SHOW HIDE INTERVAL MODE (SEMITONES OR RATIO) WIDGETS */
 kIntervalMode	chnget	"IntervalMode"
 if changed(kIntervalMode)==1 then				; semitones mode
  if kIntervalMode==1 then
  	chnset	"visible(0)","RatioPlant_ident"
  	chnset	"visible(1)","SemitonesPlant_ident"
  else								; ratio mode
  	chnset	"visible(1)","RatioPlant_ident"
  	chnset	"visible(0)","SemitonesPlant_ident"
  endif
 endif
 /* DEFINE TRANSPOSITION RATIO BASED ON INTERVAL MODE CHOICE */
 if kIntervalMode==1 then					; semitones mode
 	kSemitones	chnget	"Semitones"
 	;kSnap		chnget	"Snap"
 	;if kSnap==1 then
 	; if frac(kSemitones)!=0 then
 	;  kSemitones	=	round(kSemitones)
 	;  		chnset	kSemitones,"Semitones"
 	; endif
 	;endif
  	kSemitones	portk	kSemitones,kporttime
  	gktranspose	=	semitone(kSemitones)	
 else								; ratio mode
 	kNumerator	chnget	"Numerator"
 	kDenominator	chnget	"Denominator"
 	gkRatio		=	kNumerator/kDenominator
 	gktranspose	portk	gkRatio,kporttime	
 endif

 gkr1		chnget	"r1"	; pointer/note mode select (radio buttons):	manual
 gkr2		chnget	"r2"	; 						mouse
 gkr3		chnget	"r3"	; 						loop
 gkmode		=	(gkr1) + (gkr2*2) + (gkr3*3)
 gkloop		chnget	"loop"
 gkMIDI		chnget	"MIDI"
 gklock		chnget	"lock"
 gkfreeze	chnget	"freeze"
 gkfreeze	=	1-gkfreeze
 gklevel	chnget	"level"
 gkFFTSize	chnget	"FFTSize"
 gSfilepath	chnget	"filename"
 kNewFileTrg	changed	gSfilepath		; if a new file is loaded generate a trigger
 gkLoopStart	chnget	"beg"			; Click-and-drag region beginning in sample frames
 gkLoopLen	chnget	"len"			; Click-and-drag region length in sample frames
 gkLoopLen	limit	gkLoopLen,1,gkFileLen	
 gkLoopMode	chnget	"LoopMode"
 gkSpeed	chnget	"Speed"
 gkModRange	chnget	"ModRange"
 if gkModRange>0 then
  gkRate1	chnget	"Rate1"
  gkRate2	chnget	"Rate2"
  kMod		jspline	gkModRange,gkRate1,gkRate2
  kSpeed2	=	gkSpeed + kMod
  gkSpeed	=	kSpeed2
 endif
 gkPtrModRange	chnget	"PtrModRange"
 gkPtrRate1	chnget	"PtrRate1"
 gkPtrRate2	chnget	"PtrRate2"

 gkMOUSE_DOWN_RIGHT	chnget	"MOUSE_DOWN_RIGHT"		; Read in mouse right click status
 kStartScrub		trigger	gkMOUSE_DOWN_RIGHT,0.5,0	; generate a momentary trigger whenver right mouse button is clicked
 
 if gkMOUSE_DOWN_RIGHT==1 && gkmode==2 then
  kMOUSE_X	chnget	"MOUSE_X"
  kMOUSE_Y	chnget	"MOUSE_Y"
  if kStartScrub==1 then 					; prevent initial portamento when a new note is started using right click
   reinit RAMP_FUNC
  endif
  RAMP_FUNC:
  krampup	linseg	0,0.001,1
  rireturn
  kMOUSE_X	portk	(kMOUSE_X - 5) / 1045, kporttime			; Mouse X to pointer position
  kMOUSE_Y	limit	1 - ((kMOUSE_Y - 5) / 150), 0, 1		; Mouse Y transposition
  gapointer	interp	kMOUSE_X
  kSemitones	chnget	"Semitones"
  gktranspose	portk	((kMOUSE_Y*2)-1)*kSemitones,kporttime		; Transposition is scaled using transposition value derived either from 'Semitone' slider or 'Ratio' nslideres
  gktranspose	=	semitone(gktranspose)
  gklevel	portk	kMOUSE_Y*gklevel + (1-gklevel), kporttime*krampup
  schedkwhen	kStartScrub,0,0,2,0,-1
 else
  kpointer	chnget	"pointer"
  kpointer	portk	kpointer,kporttime
  gapointer	interp	kpointer
 endif
                                
 if kNewFileTrg==1 then				; if a new file has been loaded...
  event	"i",99,0,0				; call instrument to update sample storage function table 
 endif  
 
 if changed(gkmode+gkMIDI)==1 then
  if gkmode==1||gkmode==3&&gkMIDI==0 then
   event	"i",2,0,-1
  endif
 endif
endin

instr	99	; load sound file
 gichans	filenchnls	gSfilepath			; derive the number of channels (mono=1,stereo=2) in the sound file
 gitableL	ftgen	1,0,0,1,gSfilepath,0,0,1
 if gichans==2 then
  gitableR	ftgen	2,0,0,1,gSfilepath,0,0,2
 endif
 giReady 	=	1					; if no string has yet been loaded giReady will be zero
 gkFileLen	init	ftlen(1)
 
 Smessage sprintfk "file(%s)", gSfilepath			; print sound file image to fileplayer
 chnset Smessage, "filer1"
 
 /* WRITE FILE NAME TO GUI */
 Sname FileNameFromPath	gSfilepath				; Call UDO to extract file name from the full path
 Smessage sprintfk "text(%s)",Sname
 chnset Smessage, "stringbox"

endin

instr	2	; non-midi
 if gkmode!=1 && gkmode!=3 && gkMOUSE_DOWN_RIGHT!=1 || gkMIDI==1 then
  turnoff
 endif
 if giReady = 1 then				; i.e. if a file has been loaded
  aenv	linsegr	0,0.01,1,0.01,0			; simple de-click envelope
    
  iFileLen	=	ftlen(gitableL)/sr
  
  if i(gkmode)==3 then
   if gkLoopMode==1 then
    apointer	phasor	(sr*gkSpeed)/gkLoopLen
   elseif gkLoopMode==2 then
    apointer	phasor	-(sr*gkSpeed)/gkLoopLen
   elseif gkLoopMode==3 then
    apointer	poscil	1,(sr*gkSpeed*0.5)/gkLoopLen,giTriangle
   elseif gkLoopMode==4 then
    apointer	poscil	1,(sr*gkSpeed*0.5)/gkLoopLen,giRectSine
   endif
   apointer	=		(apointer * (gkLoopLen/sr)) + (gkLoopStart/sr)
  else
   apointer	=	gapointer*iFileLen
  endif

  /* RANDOM POINTER MODULATION */
  if gkPtrModRange>0 then
   kRndPtrRate	init	random(i(gkPtrRate1),i(gkPtrRate2))
   kRndPtrTrig	metro	kRndPtrRate
   kRndPtrRate	trandom	kRndPtrTrig,gkPtrRate1,gkPtrRate2
   kRndPtrPos	trandom	kRndPtrTrig,-gkPtrModRange*iFileLen,gkPtrModRange*iFileLen
   apointer	+=	interp(kRndPtrPos)
  endif
  
  ktrig	changed		gkFFTSize
  if ktrig==1 then
   reinit RESTART
  endif
  RESTART:
  if gichans=1 then
   a1	mincer		apointer, gklevel, gktranspose, gitableL, gklock, giFFTSizes[i(gkFFTSize)-1]
  	outs	a1*aenv,a1*aenv                                                                                                                                          
  elseif gichans=2 then
   a1	mincer		apointer, gklevel, gktranspose, gitableL, gklock, giFFTSizes[i(gkFFTSize)-1]
   a2	mincer		apointer, gklevel, gktranspose, gitableR, gklock, giFFTSizes[i(gkFFTSize)-1]
  	outs	a1*aenv,a2*aenv
 endif
endif
endin

instr	3	; midi triggered instrument
 if giReady = 1 then						; i.e. if a file has been loaded
  icps	cpsmidi							; read in midi note data as cycles per second
  iamp	ampmidi	1						; read in midi velocity (as a value within the range 0 - 1)
  iMidiRef	chnget	"MidiRef"				; MIDI unison reference note
  iFrqRatio		=	icps/cpsmidinn(iMidiRef)	; derive playback speed from note played in relation to a reference note (MIDI note 60 / middle C)                                          
 
  iAttTim	chnget	"AttTim"		; read in amplitude envelope attack time widget
  iRelTim	chnget	"RelTim"		; read in amplitude envelope attack time widget                                                                                                                                   
  if iAttTim>0 then				;                                       
   kenv	linsegr	0,iAttTim,1,iRelTim,0
  else								
   kenv	linsegr	1,iRelTim,0			; attack time is zero so ignore this segment of the envelope (a segment of duration zero is not permitted
  endif
  kenv	expcurve	kenv,8			; remap amplitude value with a more natural curve
  aenv	interp		kenv			; interpolate and create a-rate envelope

  iFileLen	=	ftlen(gitableL)/sr

  if i(gkmode)==3 then
   if gkLoopMode==1 then
    apointer	phasor	(sr*gkSpeed)/gkLoopLen
   elseif gkLoopMode==2 then
    apointer	phasor	-(sr*gkSpeed)/gkLoopLen
   elseif gkLoopMode==3 then
    apointer	poscil	1,(sr*gkSpeed*0.5)/gkLoopLen,giTriangle
   elseif gkLoopMode==4 then
    apointer	poscil	1,(sr*gkSpeed*0.5)/gkLoopLen,giRectSine
   endif
   apointer	=		(apointer * (gkLoopLen/sr)) + (gkLoopStart/sr)
  else                                                
   apointer	=	gapointer*iFileLen
  endif

  /* RANDOM POINTER MODULATION */
  if gkPtrModRange>0 then
   kRndPtrRate	init	random(i(gkPtrRate1),i(gkPtrRate2))
   kRndPtrTrig	metro	kRndPtrRate
   kRndPtrRate	trandom	kRndPtrTrig,gkPtrRate1,gkPtrRate2
   kRndPtrPos	trandom	kRndPtrTrig,-gkPtrModRange*iFileLen,gkPtrModRange*iFileLen
   apointer	+=	interp(kRndPtrPos)
  endif
                                                                        
  ktrig	changed		gkFFTSize
  if ktrig==1 then
   reinit RESTART
  endif
  RESTART:
  if gichans=1 then
   a1	mincer		apointer, gklevel*iamp, iFrqRatio*gktranspose, gitableL, gklock, giFFTSizes[i(gkFFTSize)-1]
  	outs	a1*aenv,a1*aenv
  elseif gichans=2 then
   a1	mincer		apointer, gklevel*iamp, iFrqRatio*gktranspose, gitableL, gklock, giFFTSizes[i(gkFFTSize)-1]
   a2	mincer		apointer, gklevel*iamp, iFrqRatio*gktranspose, gitableR, gklock, giFFTSizes[i(gkFFTSize)-1]
  	outs	a1*aenv,a2*aenv
  endif
 endif
endin

</CsInstruments>  

<CsScore>
i 1 0 10000
</CsScore>

</CsoundSynthesizer>
; SoundwarpFilePlayer.csd
; Written by Iain McCurdy, 2014

; Player can also be activated by using right-click and drag upon the waveform display panel.
;  In this mode, X position equates to pointer position and Y position equates to amplitude (level) and transposition.
;   (Transposition range when using mouse clicking-and-dragging is controlled using the 'Transposition' knob. Therefore if 'Transposition' = zero, no transposition occurs.)

<Cabbage>
form caption("Soundwarp File Player") size(835,340), colour(0,0,0) pluginid("SWPl"), guirefresh(32) style("legacy")
image                       bounds(  0,  0,835,340), colour( 30, 90, 60), outlinecolour("White"), line(3), shape("sharp")	; main panel colouration    

soundfiler bounds(  5,  5,825,175), channel("beg","len"), channel("pos1","end1"), identchannel("filer1"),  colour(0, 255, 255, 255), fontcolour(160, 160, 160, 255), 

filebutton bounds(  5,185, 80, 22), text("Open File","Open File"), fontcolour("white") channel("filename"), shape("ellipse")
checkbox   bounds(  5,210, 95, 17), channel("PlayStop"), text("Play/Stop"), fontcolour("white")
label      bounds(  5,228,115, 10), text("[or right-click-and-drag]"), fontcolour("white")
checkbox   bounds(  5,240,100, 17), channel("freeze"), text("Freeze"), colour("LightBlue"), fontcolour("white")

label      bounds(245,184,180, 8), text("G   R   A   I   N   S"), fontcolour("white")
rslider    bounds(120,195, 60, 60), channel("overlap"),     range( 1, 100, 20, 1,1),            colour( 50,110, 80), text("Overlaps"),     textcolour("white"), trackercolour(150,210,180)
rslider    bounds(175,195, 60, 60), channel("grsize"),      range( 1, 40000, 800, 0.5,1),       colour( 50,110, 80), text("Size"),         textcolour("white"), trackercolour(150,210,180)
rslider    bounds(230,195, 60, 60), channel("grsizeOS"),    range( 0, 2.00,   0.5,  0.5),       colour( 50,110, 80), text("Size OS"),      textcolour("white"), trackercolour(150,210,180)
rslider    bounds(285,195, 60, 60), channel("transpose"),   range(-48, 48, 0,1,0.001),          colour( 50,110, 80), text("Transpose"),    textcolour("white"), trackercolour(150,210,180)

label      bounds(367,198, 35, 10), text("M o d e"), fontcolour("white")
combobox   bounds(348,208, 74, 17), channel("mode"), items("Speed", "Pointer", "Region"), value(1), fontcolour("white")
label      bounds(364,227, 40, 10), text("S h a p e"), fontcolour("white")
combobox   bounds(348,237, 74, 17), channel("shape"), items("Hanning", "Perc.1", "Perc.2", "Gate", "Rev.Perc.1", "Rev.Perc.2"), value(1), fontcolour("white")

rslider    bounds(425,195, 60, 60), channel("speed"),       range( 0.00, 5.00, 1,0.5,0.001),           colour( 50,110, 80), text("Speed"),   textcolour("white"), trackercolour(150,210,180), visible(1), identchannel("SpeedID")
rslider    bounds(425,195, 60, 60), channel("ptr"),         range(     0,1.00, 0.5,1,0.001),           colour( 50,110, 80), text("Pointer"), textcolour("white"), trackercolour(150,210,180), visible(0), identchannel("PtrID")
rslider    bounds(480,195, 60, 60), channel("ptrOS"),       range(     0, 1.000, 0, 0.5, 0.001),       colour( 50,110, 80), text("Ptr.OS"),  textcolour("white"), trackercolour(150,210,180), visible(0), identchannel("PtrOSID")
rslider    bounds(535,195, 60, 60), channel("port"),        range(     0,30.000,0.01, 0.5,0.001),      colour( 50,110, 80), text("Port."),   textcolour("white"), trackercolour(150,210,180), visible(0), identchannel("PortID")
rslider    bounds(535,195, 60, 60), channel("inskip"),      range(     0, 1.00, 0),                    colour( 50,110, 80), text("inskip"),  textcolour("white"), trackercolour(150,210,180), visible(1), identchannel("inskipID")

line       bounds(595,190,  2, 65), colour("Grey")

label      bounds(595,184,120, 8), text("E   N   V   E   L   O   P   E"), fontcolour("white")
rslider    bounds(598,195, 60, 60), channel("AttTim"),    range(0, 5.00, 0.01, 0.5, 0.001),  colour( 50,110, 80), text("Att.Tim"), textcolour("white"), trackercolour(150,210,180)
rslider    bounds(650,195, 60, 60), channel("RelTim"),    range(0.01, 5, 0.05, 0.5, 0.001),  colour( 50,110, 80), text("Rel.Tim"), textcolour("white"), trackercolour(150,210,180)

line       bounds(711,190,  2, 65), colour("Grey")

label      bounds(732,184, 80, 8), text("C   O   N   T   R   O   L"), fontcolour("white")
rslider    bounds(715,195, 60, 60), channel("MidiRef"),   range(0,127,60, 1, 1),            colour( 50,110, 80), text("MIDI Ref."), textcolour("white"), trackercolour(150,210,180)
rslider    bounds(770,195, 60, 60), channel("level"),     range(  0,  1.00, 0.4, 0.5),      colour( 50,110, 80), text("Level"),     textcolour("white"), trackercolour(150,210,180)

keyboard   bounds(5,260,825, 75)
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-n -+rtmidi=NULL -M0 -dm0
</CsOptions>

<CsInstruments>

sr = 48000
ksmps = 64
nchnls = 2
0dbfs=1

massign	0,3
gichans		init	0
giFileLen	init	0
giReady		init	0
gSfilepath	init	""

; WINDOWING FUNCTIONS USED TO DYNAMICALLY SHAPE THE GRAINS
; NUM | INIT_TIME | SIZE | GEN_ROUTINE | PARTIAL_NUM | STRENGTH | PHASE
; GRAIN ENVELOPE WINDOW FUNCTION TABLES:
;giwfn1	ftgen	0,  0, 131072,  20,   1, 1 					; HANNING
giwfn1	ftgen	0,  0, 131072,  9,    0.5, 1,0 					; HALF SINE
giwfn2	ftgen	0,  0, 131072,  7,    0, 3072,   1, 128000,    0		; PERCUSSIVE - STRAIGHT SEGMENTS
giwfn3	ftgen	0,  0, 131072,  5, .001, 3072,   1, 128000, .001		; PERCUSSIVE - EXPONENTIAL SEGMENTS
giwfn4	ftgen	0,  0, 131072,  7,    0, 1536,   1, 128000,    1, 1536, 0	; GATE - WITH DE-CLICKING RAMP UP AND RAMP DOWN SEGMENTS
giwfn5	ftgen	0,  0, 131072,  7,    0, 128000, 1, 3072,      0		; REVERSE PERCUSSIVE - STRAIGHT SEGMENTS
giwfn6	ftgen	0,  0, 131072,  5, .001, 128000, 1, 3072,   .001		; REVERSE PERCUSSIVE - EXPONENTIAL SEGMENTS

instr	1
 gkloop		chnget	"loop"
 gkPlayStop	chnget	"PlayStop"
 gkfreeze	chnget	"freeze"
 gkfreeze	=	1-gkfreeze
 gktranspose	chnget	"transpose"
 gkoverlap	chnget	"overlap"
 gkgrsize	chnget	"grsize"
 gkgrsizeOS	chnget	"grsizeOS"
 gkshape	chnget	"shape"
 gkmode		chnget	"mode"
 gkmode		init	1
 gkspeed	chnget	"speed"
 gkptrOS	chnget	"ptrOS"
 gkport		chnget	"port"
 kporttime	linseg	0,0.001,1
 gkinskip	chnget	"inskip"
 gklevel	chnget	"level"
 gkLoopStart	chnget	"beg"		;from soundfiler
 gkLoopEnd	chnget	"len"			;from soundfiler
 
 
 ; SHOW OR HIDE WIDGETS -------------------------------------
 kchange	changed	gkmode
 if(kchange==1) then
	if gkmode==1 then
	 chnset "visible(1)", "SpeedID"
	 chnset "visible(0)", "PtrOSID"
	 chnset "visible(0)", "PtrID"
	 chnset "visible(0)", "PortID"
	 chnset "visible(1)", "inskipID"
	endif
	if gkmode==2 then
	 chnset "visible(0)", "SpeedID"
	 chnset "visible(1)", "PtrOSID"
	 chnset "visible(1)", "PtrID"
	 chnset "visible(1)", "PortID"
	 chnset "visible(0)", "inskipID"
	endif
	if gkmode==3 then
	 chnset "visible(0)", "SpeedID"
	 chnset "visible(0)", "PtrOSID"
	 chnset "visible(0)", "PtrID"
	 chnset "visible(0)", "PortID"
	 chnset "visible(0)", "inskipID"
	endif
endif
; -----------------------------------------------------------

 gSfilepath	chnget	"filename"
 kNewFileTrg	changed	gSfilepath		; if a new file is loaded generate a trigger
 if kNewFileTrg==1 then				; if a new file has been loaded...
  event	"i",99,0,0				; call instrument to update sample storage function table 
 endif  

 ktrig	trigger	gkPlayStop,0.5,0
 schedkwhen	ktrig,0,0,2,0,-1

 /* MOUSE SCRUBBING */
 gkMOUSE_DOWN_RIGHT	chnget	"MOUSE_DOWN_RIGHT"	; Read in mouse left click status
 kStartScrub		trigger	gkMOUSE_DOWN_RIGHT,0.5,0
 if gkMOUSE_DOWN_RIGHT==1 then
  gkmode	=	2
  if kStartScrub==1 then 
   reinit RAMP_FUNC
  endif
  RAMP_FUNC:
  krampup	linseg	0,0.001,1
  rireturn
  kMOUSE_X	chnget	"MOUSE_X"
  kMOUSE_Y	chnget	"MOUSE_Y"
  kMOUSE_X	=	(kMOUSE_X - 5) / 825
  kMOUSE_Y	portk	1 - ((kMOUSE_Y - 5) / 175), krampup*0.05		; SOME SMOOTHING OF DENSITY CHANGES VIA THE MOUSE ENHANCES PERFORMANCE RESULTS. MAKE ANY ADJUSTMENTS WITH ADDITIONAL CONSIDERATION OF guirefresh VALUE 
  gkptr		limit	kMOUSE_X,0,1
  gklevel	limit	kMOUSE_Y^2, 0, 1
  gktranspose	=	((kMOUSE_Y*2)-1)*gktranspose	;, -gktranspose, gktranspose
  schedkwhen	kStartScrub,0,0,2,0,-1
 else
  kptr		chnget	"ptr"
  gkptr		portk	kptr,gkport*kporttime
  gklevel	chnget	"level"
 endif 
 
endin



instr	99	; load sound file
 gichans	filenchnls	gSfilepath			; derive the number of channels (mono=1,stereo=2) in the sound file
 giFileLen	filelen		gSfilepath			; derive the number of channels (mono=1,stereo=2) in the sound file
 gitableL	ftgen	1,0,0,1,gSfilepath,0,0,1
 if gichans==2 then
  gitableR	ftgen	2,0,0,1,gSfilepath,0,0,2
 endif
 giReady 	=	1					; if no string has yet been loaded giReady will be zero

 Smessage sprintfk "file(%s)", gSfilepath
 chnset Smessage, "filer1"	

endin



instr	2	; triggered by 'play/stop' button
 if gkPlayStop==0&&gkMOUSE_DOWN_RIGHT==0 then
  turnoff
 endif
 if giReady = 1 then						; i.e. if a file has been loaded
  iAttTim	chnget	"AttTim"				; read in widgets
  iRelTim	chnget	"RelTim"
  if iAttTim>0 then						; is amplitude envelope attack time is greater than zero...
   kenv	linsegr	0,iAttTim,1,iRelTim,0				; create an amplitude envelope with an attack, a sustain and a release segment (senses realtime release)
  else            
   kenv	linsegr	1,iRelTim,0					; create an amplitude envelope with a sustain and a release segment (senses realtime release)
  endif
  kenv	expcurve	kenv,8					; remap amplitude value with a more natural curve
  aenv	interp		kenv					; interpolate and create a-rate envelope

  kporttime	linseg	0,0.001,0.02				; portamento time function. (Rises quickly from zero to a held value.)

  kspeed	portk	gkspeed, kporttime
  kptr		portk	gkptr, kporttime
  
  ktranspose	portk	gktranspose,kporttime*3			; 
  
  ktrig	changed	gkshape,gkoverlap,gkgrsize,gkgrsizeOS,gkmode,gkinskip
  if ktrig==1 then
   reinit	UPDATE
  endif
  UPDATE:
  
  iwfn		=	i(gkshape) + giwfn1 - 1
  imode		=	i(gkmode) - 1

  if imode==0 then						; timestretch mode
   kwarp	=	1/(kspeed*gkfreeze)
   ibeg		=	i(gkinskip) * giFileLen
   kscrubber	init	ibeg*sr
   kscrubber	+=	kspeed*ksmps*gkfreeze
  elseif imode==1 then						; pointer mode
   kwarp	=	giFileLen * kptr
   ibeg		=	0
   kptrOS	random	0, (giFileLen - kwarp) * gkptrOS
   kwarp	=	kwarp + kptrOS
   kscrubber	=	(kwarp + kptrOS) * sr
  else								; region mode
   imode	=	1					; sndwarp mode used in region mode will be pointer mode
   kwarp	random	gkLoopStart/sr,(gkLoopStart+gkLoopEnd)/sr
   ibeg		=	0
   kscrubber	=	kwarp * sr
  endif

  apch		interp	semitone(ktranspose)
  klevel	portk	gklevel/(i(gkoverlap)^0.25), kporttime		; apply portamento smoothing to changes in level
  
  if gichans==1 then						; if mono...   
   a1	sndwarp		klevel, kwarp, apch, gitableL, ibeg, i(gkgrsize), i(gkgrsize) * i(gkgrsizeOS), i(gkoverlap), iwfn, imode
 	outs	a1*aenv,a1*aenv					; send mono audio to both outputs 
  elseif gichans==2 then						; otherwise, if stereo...
   a1	sndwarp		klevel, kwarp, apch, gitableL, ibeg, i(gkgrsize), i(gkgrsize) * i(gkgrsizeOS), i(gkoverlap), iwfn, imode
   a2	sndwarp		klevel, kwarp, apch, gitableR, ibeg, i(gkgrsize), i(gkgrsize) * i(gkgrsizeOS), i(gkoverlap), iwfn, imode
 	outs	a1*aenv,a2*aenv					; send stereo signal to outputs
  endif
  rireturn
 endif

 ; print scrubber
 if(metro(10)==1) then
  Smessage sprintfk "scrubberposition(%d)", kscrubber
  chnset Smessage, "filer1"
 endif

endin

instr	3
 icps	cpsmidi							; read in midi note data as cycles per second
 iamp	ampmidi	1						; read in midi velocity (as a value within the range 0 - 1)
 iAttTim	chnget	"AttTim"				; read in widgets
 iRelTim	chnget	"RelTim"
 iMidiRef	chnget	"MidiRef"
 iFrqRatio		=	icps/cpsmidinn(iMidiRef)	; derive playback speed from note played in relation to a reference note (MIDI note 60 / middle C)

 if giReady = 1 then						; i.e. if a file has been loaded
  iAttTim	chnget	"AttTim"				; read in widgets
  iRelTim	chnget	"RelTim"
  if iAttTim>0 then						; is amplitude envelope attack time is greater than zero...
   kenv	linsegr	0,iAttTim,1,iRelTim,0				; create an amplitude envelope with an attack, a sustain and a release segment (senses realtime release)
  else            
   kenv	linsegr	1,iRelTim,0					; create an amplitude envelope with a sustain and a release segment (senses realtime release)
  endif
  kenv	expcurve	kenv,8					; remap amplitude value with a more natural curve
  aenv	interp		kenv					; interpolate and create a-rate envelope

  kporttime	linseg	0,0.001,0.05				; portamento time function. (Rises quickly from zero to a held value.)

  kspeed	portk	gkspeed,kporttime
  kptr		portk	gkptr, kporttime
  
  ktranspose	portk	gktranspose,kporttime			; 
  
  ktrig	changed	gkshape,gkoverlap,gkgrsize,gkgrsizeOS,gkmode,gkinskip
  if ktrig==1 then
   reinit	UPDATE
  endif
  UPDATE:
  
  iwfn		=	i(gkshape) + giwfn1 - 1
  imode		=	i(gkmode) - 1
  
  if imode==0 then						; timestretch mode
   kwarp	=	1/(kspeed*gkfreeze)
   ibeg		=	i(gkinskip) * giFileLen
   kscrubber	init	ibeg*sr
   kscrubber	+=	kspeed*ksmps*gkfreeze
  elseif imode==1 then						; pointer mode
   kwarp	=	giFileLen * kptr
   kptrOS	random	0, (giFileLen - kwarp) * gkptrOS
   kwarp	=	kwarp + kptrOS
   ibeg		=	0
   kscrubber	=	(kwarp + kptrOS) * sr
  else								; region mode
   imode	=	1					; sndwarp mode used in region mode will be pointer mode
   kwarp	random	gkLoopStart/sr,(gkLoopStart+gkLoopEnd)/sr
   ibeg		=	0
   kscrubber	=	kwarp * sr
  endif


  apch		interp	semitone(ktranspose)
  klevel	portk	gklevel/(i(gkoverlap)^0.25), kporttime		; apply portamento smoothing to changes in level

  if gichans==1 then						; if mono...
   a1	sndwarp		klevel*iamp, kwarp, iFrqRatio, gitableL, ibeg, i(gkgrsize), i(gkgrsize) * i(gkgrsizeOS), i(gkoverlap), iwfn, imode
 	outs	a1*aenv,a1*aenv					; send mono audio to both outputs 
  elseif gichans==2 then						; otherwise, if stereo...
   a1	sndwarp		klevel*iamp, kwarp, iFrqRatio, gitableL, ibeg, i(gkgrsize), i(gkgrsize) * i(gkgrsizeOS), i(gkoverlap), iwfn, imode
   a2	sndwarp		klevel*iamp, kwarp, iFrqRatio, gitableR, ibeg, i(gkgrsize), i(gkgrsize) * i(gkgrsizeOS), i(gkoverlap), iwfn, imode
 	outs	a1*aenv,a2*aenv					; send stereo signal to outputs
  endif
  rireturn
 endif


 iactive	active	p1
 if iactive==1 then 
  if imode==0 then						; timestretch mode
   kscrubber	init	0
   kscrubber	+=	kspeed*ksmps*gkfreeze
  else								; pointer mode
   kptrOS	random	0, (giFileLen - kwarp) * gkptrOS
   kwarp	=	kwarp + kptrOS
   kscrubber	=	(kwarp + kptrOS) * sr
  endif
 
  ; print scrubber
 if(metro(10)==1) then
  Smessage sprintfk "scrubberposition(%d)", kscrubber
  chnset Smessage, "filer1"
 endif

 endif
endin

/*
event_i	"i",999,0,3600
instr	999
 aL,aR	monitor
 		fout	"SndWarpOut.wav",4,aL,aR
endin
*/

</CsInstruments>  

<CsScore>
i 1 0 [60*60*24*7]
</CsScore>

</CsoundSynthesizer>
; SyncgrainFilePlayer.csd
; Written by Iain McCurdy, 2014
; 
; The internal pointer used by syncgrain to track progress through the sound file is affected by grain size and density as well as speed 
;  so on accaount of this complication a scrubber line in the waveform view is not yet implemented.

<Cabbage>
form caption("Syncgrain File Player") size(800,340), colour(0,0,0) pluginid("SGFP") style("legacy")
image                       bounds(  0,  0,800,340), colour( 90, 60, 30), outlinecolour("White"), shape("sharp"), line(3)	; main panel colouration    
soundfiler                  bounds(  5,  5,790,175), channel("beg","len"), identchannel("filer1"),  colour(0, 255, 255, 255), fontcolour(160, 160, 160, 255), 

image    bounds(  0,180,800,160), colour(0,0,0,0), outlinecolour("white"), line(0), shape("sharp"), plant("controls"){
filebutton bounds(  5, 10, 80, 25), text("Open File","Open File"), fontcolour("white") channel("filename"), shape("ellipse")
checkbox   bounds(  5, 40, 95, 25), channel("PlayStop"), text("Play/Stop"), fontcolour("white")
label      bounds(225,  4, 65, 8), text("G   R   A   I   N   S"), fontcolour("white")
rslider    bounds( 90, 15, 60, 60), channel("density"),   range( 0.5,400.00,20, 0.5),          colour( 70, 40, 10), text("Density"),     textcolour("white"), trackercolour(190,170,130), outlinecolour(100,100,100)
rslider    bounds(145, 15, 60, 60), channel("DensOS"),     range( 0, 5.00, 0),                 colour( 70, 40, 10), text("Dens.OS"),     textcolour("white"), trackercolour(190,170,130), outlinecolour(100,100,100)
rslider    bounds(200, 15, 60, 60), channel("grsize"),   range( 0.001,1.00, 0.04, 0.5),        colour( 70, 40, 10), text("Size"),        textcolour("white"), trackercolour(190,170,130), outlinecolour(100,100,100)
rslider    bounds(255, 15, 60, 60), channel("SizeOS"),   range( 0, 5.00, 0, 0.5),              colour( 70, 40, 10), text("Size OS"),     textcolour("white"), trackercolour(190,170,130), outlinecolour(100,100,100)
rslider    bounds(310, 15, 60, 60), channel("transpose"), range(-24, 24, 0,1,1),               colour( 70, 40, 10), text("Transpose"),   textcolour("white"), trackercolour(190,170,130), outlinecolour(100,100,100)
rslider    bounds(365, 15, 60, 60), channel("TransposeOS"), range(0, 12.00, 0),                colour( 70, 40, 10), text("Trans.OS"),    textcolour("white"), trackercolour(190,170,130), outlinecolour(100,100,100)
rslider    bounds(420, 15, 60, 60), channel("speed"),     range( -2.00, 2.00, 1),              colour( 70, 40, 10), text("Speed"),       textcolour("white"), trackercolour(190,170,130), outlinecolour(100,100,100)
label      bounds(495, 12, 40, 10), text("S h a p e"), fontcolour("white")
combobox   bounds(478, 22, 74, 18), channel("shape"), items("Hanning", "Perc.1", "Perc.2", "Gate", "Rev.Perc.1", "Rev.Perc.2"), value(1),fontcolour("white")
checkbox   bounds(478, 45,100, 18), channel("freeze"), text("Freeze"), colour("LightBlue"), fontcolour("white")

line       bounds(559, 10,  2, 65), colour("Grey")

label      bounds(578,  4, 86, 8), text("E   N   V   E   L   O   P   E"), fontcolour("white")
rslider    bounds(565, 15, 60, 60), channel("AttTim"),    range(0, 5, 0, 0.5, 0.001),       colour( 70, 40, 10), text("Att.Tim"),   textcolour("white"), trackercolour(190,170,130), outlinecolour(100,100,100)
rslider    bounds(620, 15, 60, 60), channel("RelTim"),    range(0.01, 5, 0.05, 0.5, 0.001), colour( 70, 40, 10), text("Rel.Tim"),   textcolour("white"), trackercolour(190,170,130), outlinecolour(100,100,100)

line       bounds(684, 10,  2, 65), colour("Grey")

label      bounds(702,  4, 76, 8), text("C   O   N   T   R   O   L"), fontcolour("white")
rslider    bounds(685, 15, 60, 60), channel("MidiRef"),   range(0,127,60, 1, 1),            colour( 70, 40, 10), text("MIDI Ref."), textcolour("white"), trackercolour(190,170,130), outlinecolour(100,100,100)
rslider    bounds(740, 15, 60, 60), channel("level"),     range(  0,  3.00, 1, 0.5),        colour( 70, 40, 10), text("Level"),     textcolour("white"), trackercolour(190,170,130), outlinecolour(100,100,100)

keyboard bounds(5, 80,790, 75)
}
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-n -+rtmidi=NULL -M0 -dm0
</CsOptions>

<CsInstruments>

sr = 48000
ksmps = 64
nchnls = 2
0dbfs=1

massign	0,3
gichans		init	0
giReady		init	0
gSfilepath	init	""

; WINDOWING FUNCTIONS USED TO DYNAMICALLY SHAPE THE GRAINS
; NUM | INIT_TIME | SIZE | GEN_ROUTINE | PARTIAL_NUM | STRENGTH | PHASE
; GRAIN ENVELOPE WINDOW FUNCTION TABLES:
giwfn1	ftgen	0,  0, 131072,  20,   2, 1 					; HANNING
giwfn2	ftgen	0,  0, 131072,  7,    0, 3072,   1, 128000,    0		; PERCUSSIVE - STRAIGHT SEGMENTS
giwfn3	ftgen	0,  0, 131072,  5, .001, 3072,   1, 128000, .001		; PERCUSSIVE - EXPONENTIAL SEGMENTS
giwfn4	ftgen	0,  0, 131072,  7,    0, 1536,   1, 128000,    1, 1536, 0	; GATE - WITH DE-CLICKING RAMP UP AND RAMP DOWN SEGMENTS
giwfn5	ftgen	0,  0, 131072,  7,    0, 128000, 1, 3072,      0		; REVERSE PERCUSSIVE - STRAIGHT SEGMENTS
giwfn6	ftgen	0,  0, 131072,  5, .001, 128000, 1, 3072,   .001		; REVERSE PERCUSSIVE - EXPONENTIAL SEGMENTS

instr	1
 gkloop		chnget	"loop"
 gkPlayStop	chnget	"PlayStop"
 gktranspose	chnget	"transpose"
 gkTransposeOS	chnget	"TransposeOS"
 gkdensity	chnget	"density"
 gkDensOS	chnget	"DensOS"
 gkgrsize	chnget	"grsize"
 gkSizeOS	chnget	"SizeOS"
 gkshape	chnget	"shape"
 gkspeed	chnget	"speed"
 gklevel	chnget	"level"
 gkfreeze	chnget	"freeze"
 gkfreeze	=	1-gkfreeze
        
 gSfilepath	chnget	"filename"
 kNewFileTrg	changed	gSfilepath		; if a new file is loaded generate a trigger
 if kNewFileTrg==1 then				; if a new file has been loaded...
  event	"i",99,0,0				; call instrument to update sample storage function table 
 endif  

 ktrig	trigger	gkPlayStop,0.5,0
 schedkwhen	ktrig,0,0,2,0,-1
endin

instr	99	; load sound file
 gichans	filenchnls	gSfilepath			; derive the number of channels (mono=1,stereo=2) in the sound file
 gitableL	ftgen	1,0,0,1,gSfilepath,0,0,1
 if gichans==2 then
  gitableR	ftgen	2,0,0,1,gSfilepath,0,0,2
 endif
 giReady 	=	1					; if no string has yet been loaded giReady will be zero
 Smessage sprintfk "file(%s)", gSfilepath			; print sound file to viewer
 chnset Smessage, "filer1"
endin

instr	2	; triggered by 'play/stop' button
 if gkPlayStop==0 then
  turnoff
 endif
 if giReady = 1 then						; i.e. if a file has been loaded
  iAttTim	chnget	"AttTim"				; read in widgets
  iRelTim	chnget	"RelTim"
  if iAttTim>0 then						; is amplitude envelope attack time is greater than zero...
   kenv	linsegr	0,iAttTim,1,iRelTim,0				; create an amplitude envelope with an attack, a sustain and a release segment (senses realtime release)
  else            
   kenv	linsegr	1,iRelTim,0					; create an amplitude envelope with a sustain and a release segment (senses realtime release)
  endif
  kenv	expcurve	kenv,8					; remap amplitude value with a more natural curve
  aenv	interp		kenv					; interpolate and create a-rate envelope
  kporttime	linseg	0,0.001,0.05				; portamento time function. (Rises quickly from zero to a held value.)
  kdensity	portk	gkdensity,kporttime			; apply portamento smoothing to changes in speed
  kprate	portk	gkspeed,kporttime
  klevel	portk	gklevel,kporttime			; apply portamento smoothing to changes in level

  kDensOS	gauss	gkDensOS
  kDensMlt	=	octave(kDensOS)
  kdensity	=	kdensity * kDensMlt
  
  ktranspose	portk	gktranspose,kporttime			; 
  kTransposeOS	gauss	gkTransposeOS
  ktranspose	=	ktranspose + kTransposeOS
  
  kSizeOS	rand	gkSizeOS
  kgrsize	=	gkgrsize * octave(kSizeOS)
  
  giolaps	=	5000
  
  ktrig	changed	gkshape
  if ktrig==1 then
   reinit	UPDATE
  endif
  UPDATE:
  
  iwfn		=	i(gkshape) + giwfn1 - 1
  if gichans==1 then						; if mono...
   a1	syncgrain	klevel, kdensity, semitone(ktranspose), kgrsize, kprate*gkfreeze, gitableL, iwfn, giolaps
 	outs	a1*aenv,a1*aenv					; send mono audio to both outputs 
  elseif gichans==2 then						; otherwise, if stereo...
   a1	syncgrain	klevel, kdensity, semitone(ktranspose), kgrsize, kprate*gkfreeze, gitableL, iwfn, giolaps
   a2	syncgrain	klevel, kdensity, semitone(ktranspose), kgrsize, kprate*gkfreeze, gitableR, iwfn, giolaps
 	outs	a1*aenv,a2*aenv					; send stereo signal to outputs
  endif
  rireturn
 endif
endin

instr	3
 icps	cpsmidi							; read in midi note data as cycles per second
 iamp	ampmidi	1						; read in midi velocity (as a value within the range 0 - 1)
 iAttTim	chnget	"AttTim"				; read in widgets
 iRelTim	chnget	"RelTim"
 iMidiRef	chnget	"MidiRef"
 iFrqRatio		=	icps/cpsmidinn(iMidiRef)	; derive playback speed from note played in relation to a reference note (MIDI note 60 / middle C)

 if giReady = 1 then						; i.e. if a file has been loaded
  iAttTim	chnget	"AttTim"				; read in widgets
  iRelTim	chnget	"RelTim"
  if iAttTim>0 then						; is amplitude envelope attack time is greater than zero...
   kenv	linsegr	0,iAttTim,1,iRelTim,0				; create an amplitude envelope with an attack, a sustain and a release segment (senses realtime release)
  else            
   kenv	linsegr	1,iRelTim,0					; create an amplitude envelope with a sustain and a release segment (senses realtime release)
  endif
  kenv	expcurve	kenv,8					; remap amplitude value with a more natural curve
  aenv	interp		kenv					; interpolate and create a-rate envelope
  kporttime	linseg	0,0.001,0.05				; portamento time function. (Rises quickly from zero to a held value.)
  kdensity	portk	gkdensity,kporttime			; apply portamento smoothing to changes in speed
  kprate	portk	gkspeed,kporttime
  klevel	portk	gklevel,kporttime			; apply portamento smoothing to changes in level

  kDensOS	gauss	gkDensOS
  kDensMlt	=	octave(kDensOS)
  kdensity	=	kdensity * kDensMlt
    
  kSizeOS	rand	gkSizeOS
  kgrsize	=	gkgrsize * octave(kSizeOS)

  giolaps	=	5000
  
  ktrig	changed	gkshape
  if ktrig==1 then
   reinit	UPDATE
  endif
  UPDATE:
  
  iwfn		=	i(gkshape) + giwfn1 - 1
  if gichans==1 then						; if mono...
   a1	syncgrain	klevel*iamp, kdensity, iFrqRatio, kgrsize, kprate*gkfreeze, gitableL, iwfn, giolaps
 	outs	a1*aenv,a1*aenv					; send mono audio to both outputs 
  elseif gichans==2 then						; otherwise, if stereo...
   a1	syncgrain	klevel*iamp, kdensity, iFrqRatio, kgrsize, kprate*gkfreeze, gitableL, iwfn, giolaps
   a2	syncgrain	klevel*iamp, kdensity, iFrqRatio, kgrsize, kprate*gkfreeze, gitableR, iwfn, giolaps
 	outs	a1*aenv,a2*aenv					; send stereo signal to outputs
  endif
  rireturn
 endif

endin

</CsInstruments>  

<CsScore>
i 1 0 [60*60*24*7]
</CsScore>

</CsoundSynthesizer>
; Table3FilePlayer.csd
; Written by Iain McCurdy, 2014

; Load a user selected sound file into a GEN 01 function table and plays it back using loscil3. 
; This file player is suited for polyphonic playback and is less well suited for the playback of very long sound files .
;                                 
; Loop points can be edited by using either the sliders 'Start' and 'End' or by clicking and dragging on the waveform view.
;  Edit mode is selected automatically according to the last method used.
; 
; 
; Things still to do:
;  Add loop direction function when using sliders to edit loop points
;  Portamento control for loop points when using sliders to edit loop points
;  Add instructions in viewer panel.
;  MIDI note loop play in click-and-drag mode not working properly
 
 
<Cabbage>
form caption("Table3 File Player") size(740,340), colour(0,0,0) pluginid("T3Pl"), guirefresh(64) style("legacy")
image                    bounds(  0,  0,740,340), colour(30, 30, 70), outlinecolour("White"), line(3), shape("sharp")	; main panel colouration    
soundfiler               bounds(  5,  5,730,175), channel("beg","len"), identchannel("filer1"),  colour(0, 255, 255, 255), fontcolour(160, 160, 160, 255), 
label bounds(6, 4, 560, 14), text(""), align(left), colour(0,0,0,0), fontcolour(200,200,200), identchannel("stringbox")

filebutton bounds(  5,190, 80, 25), text("Open File","Open File"), fontcolour("white") channel("filename"), shape("ellipse")
checkbox   bounds(  5,220, 95, 25), channel("PlayStop"), text("Play/Stop"), colour("yellow"), fontcolour("white")

label      bounds(178,184, 45, 8), text("L   O   O   P"), fontcolour("white")

groupbox   bounds(100,200,100, 50), plant("looping"), text("Looping Mode"), fontcolour("white"){
combobox   bounds( 10, 25, 80, 20), channel("mode"), items("Forward", "Backward", "Fwd./Bwd."), value(1), fontcolour("white")
}
rslider    bounds(205,195, 60, 60), channel("LoopStart"), range(0, 1, 0),                   colour(60, 60,100), text("Start"),     textcolour("white"), trackercolour(210,210,250)
rslider    bounds(260,195, 60, 60), channel("LoopEnd"),   range(0, 1, 1),                   colour(60, 60,100), text("End"),       textcolour("white"), trackercolour(210,210,250)
rslider    bounds(315,195, 60, 60), channel("Portamento"),   range(0,0.1,0.01),                   colour(60, 60,100), text("Portamento"),       textcolour("white"), trackercolour(210,210,250)

line       bounds(375,190,  2, 65), colour("Grey")

label      bounds(409,184, 55, 8), text("S   P   E   E   D"), fontcolour("white")
rslider    bounds(380,195, 60, 60), channel("transpose"), range(-24, 24, 0,1,1),            colour(60, 60,100), text("Transpose"), textcolour("white"), trackercolour(210,210,250)
rslider    bounds(435,195, 60, 60), channel("speed"),     range(0, 4.00, 1, 0.5, 0.001),    colour(60, 60,100), text("Speed"),     textcolour("white"), trackercolour(210,210,250)

line       bounds(495,190,  2, 65), colour("Grey")

label      bounds(511,184, 90, 8), text("E   N   V   E   L   O   P   E"), fontcolour("white")
rslider    bounds(500,195, 60, 60), channel("AttTim"),    range(0, 5, 0.01, 0.5, 0.001),    colour(60, 60,100), text("Att.Tim"),   textcolour("white"), trackercolour(210,210,250)
rslider    bounds(555,195, 60, 60), channel("RelTim"),    range(0.01, 5, 0.05, 0.5, 0.001), colour(60, 60,100), text("Rel.Tim"),   textcolour("white"), trackercolour(210,210,250)
line       bounds(615,190,  2, 65), colour("Grey")

label      bounds(636,184, 80, 8), text("C   O   N   T   R   O   L"), fontcolour("white")
rslider    bounds(620,195, 60, 60), channel("MidiRef"),   range(0,127,60, 1, 1),            colour(60, 60,100), text("MIDI Ref."), textcolour("white"), trackercolour(210,210,250)
rslider    bounds(675,195, 60, 60), channel("level"),     range(  0,  3.00, 1, 0.5),        colour(60, 60,100), text("Level"),     textcolour("white"), trackercolour(210,210,250)

keyboard bounds(5,260, 730, 75)
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-n -+rtmidi=NULL -M0 -dm0
</CsOptions>

<CsInstruments>

sr = 44100
ksmps = 32
nchnls = 2
0dbfs = 1

		massign	0,3
gichans		init	0
giFileLen	init	0
giReady		init	0
gSfilepath	init	""
gkTabLen	init	2
gitri		ftgen	0,0,131072,7,0,131072/2,1,131072/2,0
gkEditMode	init	2	; 1 = CAD 2 = sliders

opcode FileNameFromPath,S,S		; Extract a file name (as a string) from a full path (also as a string)
 Ssrc	xin				; Read in the file path string
 icnt	strlen	Ssrc			; Get the length of the file path string
 LOOP:					; Loop back to here when checking for a backslash
 iasc	strchar Ssrc, icnt		; Read ascii value of current letter for checking
 if iasc==92 igoto ESCAPE		; If it is a backslash, escape from loop
 loop_gt	icnt,1,0,LOOP		; Loop back and decrement counter which is also used as an index into the string
 ESCAPE:				; Escape point once the backslash has been found
 Sname	strsub Ssrc, icnt+1, -1		; Create a new string of just the file name
	xout	Sname			; Send it back to the caller instrument
endop

instr	1	; Read in widgets
 gkMOUSE_DOWN_LEFT	chnget	"MOUSE_DOWN_LEFT"

 gkloop		chnget	"loop"

 gkLoopStart	chnget	"beg"		; Click-and-drag
 gkLoopLen	chnget	"len"
 
 gkLoopStart2	chnget	"LoopStart"	; Sliders
 gkLoopEnd2	chnget	"LoopEnd"
 gkPortamento	chnget	"Portamento"

 gkMOUSE_DOWN_RIGHT	chnget	"MOUSE_DOWN_RIGHT"			; Read in mouse left click status

 if changed(gkLoopStart,gkLoopLen)==1 then
  gkEditMode	=	1	; Click-and-drag
 elseif  changed(gkLoopStart2,gkLoopEnd2)==1||gkMOUSE_DOWN_RIGHT==1 then
  gkEditMode	=	2	; sliders
 endif
 
 gkPlayStop	chnget	"PlayStop"
 gktranspose	chnget	"transpose"
 gkspeed	chnget	"speed"
 gklevel	chnget	"level"
 gkmode		chnget	"mode"
 
 gSfilepath	chnget	"filename"
 kNewFileTrg	changed	gSfilepath		; if a new file is loaded generate a trigger
 if kNewFileTrg==1 then					; if a new file has been loaded...
  event	"i",99,0,0						; call instrument to update sample storage function table 
 endif  
 
 ktrig	trigger	gkPlayStop,0.5,0		; if play button changes to 'play', generate a trigger
 schedkwhen	ktrig,0,0,2,0,-1			; start instr 2 playing a held note

 ktrig1	changed	gktranspose				; if 'transpose' button is changed generate a '1' trigger
 ktrig2	changed	gkspeed					; if 'speed' button is changed generate a '1' trigger
 
 if ktrig1==1 then								; if transpose control has been changed...
  chnset	semitone(gktranspose),"speed"		; set speed according to transpose value
 elseif ktrig2==1 then							; if speed control has been changed...
  chnset	log2(abs(gkspeed))*12,"transpose"	; set transpose control according to speed value
 endif

  /* MOUSE SCRUBBING */
 kStartScrub		trigger	gkMOUSE_DOWN_RIGHT,0.5,0
 if gkMOUSE_DOWN_RIGHT==1 then
  if kStartScrub==1 then 
   reinit RAMP_FUNC
  endif
  RAMP_FUNC:
  krampup	linseg	0,0.001,1
  rireturn
  kMOUSE_X	chnget	"MOUSE_X"
  kMOUSE_Y	chnget	"MOUSE_Y"
  kMOUSE_X	=	(kMOUSE_X - 5) / 730
  kMOUSE_Y	portk	1 - ((kMOUSE_Y - 5) / 175), krampup*0.05		; SOME SMOOTHING OF DENSITY CHANGES VIA THE MOUSE ENHANCES PERFORMANCE RESULTS. MAKE ANY ADJUSTMENTS WITH ADDITIONAL CONSIDERATION OF guirefresh VALUE 
  gkLoopStart2		limit	kMOUSE_X,0,1
  gkLoopEnd2		limit	((kMOUSE_Y^2) * (1-kMOUSE_X)) + kMOUSE_X, 0, 1
  gktranspose	=	((kMOUSE_Y*2)-1)*gktranspose	;, -gktranspose, gktranspose
  schedkwhen	kStartScrub,0,0,2,0,-1
 else
  gkptr		chnget	"ptr"
  gklevel	chnget	"level"
 endif 

endin

instr	99	; load sound file
 gichans	filenchnls	gSfilepath			; derive the number of channels (mono=1,stereo=2) in the sound file
 gitableL	ftgen	1,0,0,1,gSfilepath,0,0,1
 giFileSamps	=		nsamp(gitableL)			; derive the file duration in samples
 giFileLen	filelen		gSfilepath			; derive the file duration in seconds
 gkTabLen	init		ftlen(gitableL)			; table length in sample frames
 if gichans==2 then
  gitableR	ftgen	2,0,0,1,gSfilepath,0,0,2
 endif
 giReady 	=	1					; if no string has yet been loaded giReady will be zero
 Smessage sprintfk "file(%s)", gSfilepath			; print file to viewer
 chnset Smessage, "filer1"	

 /* WRITE FILE NAME TO GUI */
 Sname FileNameFromPath	gSfilepath				; Call UDO to extract file name from the full path
 Smessage sprintfk "text(%s)",Sname
 chnset Smessage, "stringbox"

endin




instr	2	; Sample triggered by 'play/stop' button
 if gkPlayStop==0&&gkMOUSE_DOWN_RIGHT==0 then
  turnoff
 endif

 if giReady = 1 then						; i.e. if a file has been loaded

  iAttTim	chnget	"AttTim"				; read in widgets
  iRelTim	chnget	"RelTim"
  if iAttTim>0 then						; is amplitude envelope attack time is greater than zero...
   kenv	linsegr	0,iAttTim,1,iRelTim,0				; create an amplitude envelope with an attack, a sustain and a release segment (senses realtime release)
  else
   kenv	linsegr	1,iRelTim,0					; create an amplitude envelope with a sustain and a release segment (senses realtime release)
  endif
  kenv	expcurve	kenv,8					; remap amplitude value with a more natural curve
  aenv	interp		kenv					; interpolate and create a-rate envelope
  kporttime	linseg	0,0.001,1				; portamento time function. (Rises quickly from zero to a held value.)
  kspeed	portk	gkspeed,kporttime*gkPortamento			; apply portamento smoothing to changes in speed
  klevel	portk	gklevel,kporttime*0.1			; apply portamento smoothing to changes

  if gkEditMode==1 then						; click and drag edit mode
   gkLoopLen	limit	gkLoopLen,1,giFileSamps			; prevent loop lengths of zero
   
   krate		=	(kspeed * sr) / gkLoopLen
   arate		interp	krate
   
   if gkmode==1 then
    aphasor	phasor	arate
   elseif gkmode==2 then
    aphasor	phasor	-arate
   else
    aphasor	poscil	1,-arate*0.5,gitri
   endif
   rireturn
   
   aLoopStart	interp	gkLoopStart
   aLoopEnd	interp	gkLoopLen
   aphasor	=	(aphasor*aLoopEnd)+aLoopStart
   
   if gichans==1 then						; if mono...
    a1	table3	aphasor, gitableL
 	 outs	a1*aenv*klevel, a1*aenv*klevel			; send mono audio to both outputs 
   elseif gichans==2 then					; otherwise, if stereo...
    a1	table3	aphasor, gitableL
    a2	table3	aphasor, gitableR
 	 outs	a1*aenv*klevel, a2*aenv*klevel			; send stereo signal to outputs
   endif               
  
  elseif gkEditMode==2 then		; sliders edit mode
  
   kLoopStart	portk	gkLoopStart2,kporttime*gkPortamento
   kLoopEnd	portk	gkLoopEnd2,kporttime*gkPortamento
   kLoopEnd	=	(kLoopEnd=kLoopStart?kLoopEnd+0.001:kLoopEnd)
   
   kLoopLen	=	abs(kLoopEnd-kLoopStart)
   kdir		=	(kLoopEnd>kLoopStart?1:-1)
   
   krate		divz	kspeed, kLoopLen*giFileLen, 1
   arate		interp	krate
   aphasor	phasor	arate*kdir
   kLoopStart	min	kLoopStart,kLoopEnd
   aLoopStart	interp	kLoopStart
   aLoopLen	interp	kLoopLen
   aphasor	=	(aphasor*aLoopLen)+aLoopStart
   
   if gichans==1 then						; if mono...
    a1	table3	aphasor, gitableL, 1
 	 outs	a1*aenv*klevel, a1*aenv*klevel			; send mono audio to both outputs 
   elseif gichans==2 then					; otherwise, if stereo...
    a1	table3	aphasor, gitableL, 1
    a2	table3	aphasor, gitableR, 1
 	 outs	a1*aenv*klevel, a2*aenv*klevel			; send stereo signal to outputs
   endif               
   
   
  endif
  
 endif

 ; print scrubber
 if(metro(20)==1) then
  kscrubber	downsamp	aphasor
  Smessage sprintfk "scrubberposition(%d)", kscrubber
  chnset Smessage, "filer1"
 endif

endin




instr	3	; sample triggered by midi note
 icps	cpsmidi							; read in midi note data as cycles per second
 iamp	ampmidi	1						; read in midi velocity (as a value within the range 0 - 1)
 iMidiRef	chnget	"MidiRef"

 if giReady = 1 then						; i.e. if a file has been loaded
  iAttTim	chnget	"AttTim"				; read in widgets
  iRelTim	chnget	"RelTim"
  if iAttTim>0 then						; is amplitude envelope attack time is greater than zero...
   kenv	linsegr	0,iAttTim,1,iRelTim,0				; create an amplitude envelope with an attack, a sustain and a release segment (senses realtime release)
  else
   kenv	linsegr	1,iRelTim,0					; create an amplitude envelope with a sustain and a release segment (senses realtime release)
  endif
  kenv	expcurve	kenv,8					; remap amplitude value with a more natural curve
  aenv	interp		kenv					; interpolate and create a-rate envelope
  kporttime	linseg	0,0.001,0.05				; portamento time function. (Rises quickly from zero to a held value.)
  ispeed	=	icps/cpsmidinn(iMidiRef)		; derive playback speed from note played in relation to a reference note (MIDI note 60 / middle C)
  klevel	portk	gklevel,kporttime			; apply portamento smoothing to changes in level
    

  if gkEditMode==1 then						; click and drag edit mode

   gkLoopLen	limit	gkLoopLen,1,giFileSamps			; prevent loop lengths of zero
 
   krate		=	ispeed * sr / gkLoopLen
   arate		interp	krate
 
   if gkmode==1 then
    aphasor	phasor	arate
   elseif gkmode==2 then
    aphasor	phasor	-arate
   else
    aphasor	poscil	1,-arate*0.5,gitri
   endif
   
   aLoopStart	interp	gkLoopStart
   aLoopEnd	interp	gkLoopLen
   aphasor	=	(aphasor*aLoopEnd)+aLoopStart
 
   if gichans==1 then						; if mono...
    a1	table3	aphasor, gitableL
  	outs	a1*aenv*klevel*iamp*(1-gkMOUSE_DOWN_LEFT), a1*aenv*klevel*iamp*(1-gkMOUSE_DOWN_LEFT)	; send mono audio to both outputs 
   elseif gichans==2 then					; otherwise, if stereo...
    a1	table3	aphasor, gitableL
    a2	table3	aphasor, gitableR
  	outs	a1*aenv*klevel*iamp*(1-gkMOUSE_DOWN_LEFT), a2*aenv*klevel*iamp*(1-gkMOUSE_DOWN_LEFT)	; send stereo signal to outputs
   endif

  elseif gkEditMode==2 then		; sliders edit mode

   kLoopStart	portk	gkLoopStart2,kporttime
   kLoopEnd	portk	gkLoopEnd2,kporttime
   kLoopEnd	=	(kLoopEnd=kLoopStart?kLoopEnd+0.001:kLoopEnd)
   
   kLoopLen	=	abs(kLoopEnd-kLoopStart)
   kdir		=	(kLoopEnd>kLoopStart?1:-1)
   
   krate		divz	ispeed, kLoopLen*giFileLen, 1
   arate		interp	krate
   aphasor	phasor	arate*kdir
   kLoopStart	min	kLoopStart,kLoopEnd
   aLoopStart	interp	kLoopStart
   aLoopLen	interp	kLoopLen
   aphasor	=	(aphasor*aLoopLen)+aLoopStart
   
   if gichans==1 then						; if mono...
    a1	table3	aphasor, gitableL, 1
 	 outs	a1*aenv*klevel*iamp, a1*aenv*klevel*iamp	; send mono audio to both outputs 
   elseif gichans==2 then					; otherwise, if stereo...
    a1	table3	aphasor, gitableL, 1
    a2	table3	aphasor, gitableR, 1
 	 outs	a1*aenv*klevel*iamp, a2*aenv*klevel*iamp	; send stereo signal to outputs
   endif               




  endif

 endif

 if active(p1)==1 then						; only print scrubber for first note
  if(metro(20)==1) then
   kscrubber	downsamp	aphasor
   Smessage sprintfk "scrubberposition(%d)", kscrubber
   chnset Smessage, "filer1"
  endif
 endif
endin
 
</CsInstruments>  

<CsScore>
i 1 0 [60*60*24*7]
</CsScore>

</CsoundSynthesizer>