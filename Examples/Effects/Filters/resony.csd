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
form caption("resony") size(750,180), pluginID("rsny"), colour(255,100,0)
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
