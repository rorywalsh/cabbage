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

</CsoundSynthesizer>