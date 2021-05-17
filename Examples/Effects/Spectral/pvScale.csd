; pvScale.csd
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
form caption("pvscale Pitch Shifter") size(690,  90), pluginId("scal") style("legacy")
image                         bounds(0, 0, 690, 90), colour("SlateGrey"), outlineColour("silver"), outlineThickness(4)

image bounds(  6, 23,678, 30), colour("silver"), shape("ellipse"), outlineThickness(3)

label    bounds( 10, 24, 75, 12), text("Interval"), fontColour("black")
combobox bounds( 10, 37, 75, 20), channel("IntervalMode"), text("Semitone","Ratio"), value(1)

image   bounds( 90, 10,290, 80), colour(0,0,0,0), plant("Semitone"), identChannel("semitone_ident") {
image   bounds(  8,  1, 54, 53), colour(30,30,30), shape("ellipse"), outlineThickness(0)
rslider bounds(  0,  0, 70, 70),  text("Semitones"), channel("semis"), range(-24, 24, 7, 1, 1),   textColour("black"),colour("DarkSlateGrey"), trackerColour("LightBlue")
image   bounds( 78,  1, 54, 53), colour(30,30,30), shape("ellipse"), outlineThickness(0)
rslider bounds( 70,  0, 70, 70),  text("Cents"),     channel("cents"), range(-100, 100, 0, 1, 1), textColour("black"),colour("DarkSlateGrey"), trackerColour("LightBlue")
}

image     bounds(130, 20, 35, 58), plant("Ratio"), colour(0,0,0,0), visible(0), identChannel("ratio_ident") {
nslider bounds(  5,  5, 25, 18), channel("Numerator"),        range(1,99,3,1,1)
image     bounds(  0, 26, 35,  1), shape("sharp"), colour("black")
nslider bounds(  5, 30, 25, 18), channel("Denominator"),      range(1,99,2,1,1)
}

image   bounds(238, 11, 54, 53), colour(30,30,30), shape("ellipse"), outlineThickness(0)	; circles around rsliders
rslider bounds(230, 10, 70, 70), text("Feedback"),  channel("FB"), range(0.00, 0.99, 0),         textColour("black"),colour("DarkSlateGrey"), trackerColour("LightBlue"), identChannel("feedback_ident")
rslider bounds(230, 10, 70, 70), text("Iter."),     channel("Iter"), range(1, 10, 1,1,1),        textColour("black"),colour("DarkSlateGrey"), trackerColour("LightBlue"), identChannel("iter_ident"), visible(0)

label    bounds(310,  8, 70, 12), text("F.back Mode"), FontColour("black")
combobox bounds(310, 20,  70,20), channel("FB_mode"), value(1), text("F Sig.", "Audio", "Iter.")

label    bounds(310, 45,  70, 12), text("Formants"), FontColour("black")
combobox bounds(310, 57,  70, 20), channel("formants"), value(1), text("Ignore", "Keep 1", "Keep 2")

label    bounds(400, 24, 60, 12), text("FFT Size"), fontColour("black")
combobox bounds(400, 37, 60, 20), channel("FFTsize"), text("64","128","256","512","1024","2048","4096","8192"), value(6)

image   bounds(478, 11, 54, 53), colour(30,30,30), shape("ellipse"), outlineThickness(0)
rslider bounds(470, 10, 70, 70), text("Port."),     channel("port"),      range(0,30.00, 0.05,0.5,0.01), textColour("black"),colour("DarkSlateGrey"), trackerColour("LightBlue")
image   bounds(548, 11, 54, 53), colour(30,30,30), shape("ellipse"), outlineThickness(0)
rslider bounds(540, 10, 70, 70), text("Mix"),       channel("mix"),       range(0, 1.00, 0.5),      textColour("black"),colour("DarkSlateGrey"), trackerColour("LightBlue")
image   bounds(618, 11, 54, 53), colour(30,30,30), shape("ellipse"), outlineThickness(0)
rslider bounds(610, 10, 70, 70), text("Level"),     channel("lev"),       range(0, 1.00, 0.5, 0.5), textColour("black"),colour("DarkSlateGrey"), trackerColour("LightBlue")
</Cabbage>

<CsoundSynthesizer>
<CsOptions>
-d -n
</CsOptions>
<CsInstruments>

;sr is set by the host
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

</CsoundSynthesizer>