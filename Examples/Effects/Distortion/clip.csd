; clip.csd
; Written by Iain McCurdy, 2012.

<Cabbage>
form caption("clip") size(315, 90), pluginID("clip")
image        bounds(0, 0, 315, 90), colour( 20, 30, 40), shape("rounded"), outlinecolour("white"), outlinethickness(4) 
rslider  bounds( 10, 11, 70, 70), text("Limit"), channel("limit"), range(0.001, 1, 1, 0.5,0.001)     , colour(0,10,20), fontcolour(silver), trackercolour(lightblue)
rslider  bounds( 75, 11, 70, 70), text("Argument"), channel("arg"), range(0, 1.00, 0.5)              , colour(0,10,20), fontcolour(silver), trackercolour(lightblue)
combobox bounds(150, 15, 80,20), channel("method"), value(1), text("B.D.J.", "Sine", "Tanh")
checkbox bounds(150, 45, 100, 20), channel("clip_light"), text("clip"), colour("red"), fontcolour("white"), shape("rounded"), active(0)
rslider  bounds(235, 11, 70, 70), text("Level"), channel("level"), range(0, 10.00, 0.7)              , colour(0,10,20), fontcolour(silver), trackercolour(lightblue)
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
	gklimit		chnget	"limit"				;READ WIDGETS...
	gkmethod	chnget	"method"			;
	gkmethod	init	1
	gkmethod	init	i(gkmethod)-1
	gkarg		chnget	"arg"
	gklevel		chnget	"level"				;
	;asigL, asigR	diskin2	"Seashore.wav",1,0,1		;OPTIONALLY USE A BUILT-IN WAVEFORM FOR TESTING
	asigL, asigR	ins
	kSwitch		changed		gklimit, gkmethod, gkarg	;GENERATE A MOMENTARY '1' PULSE IN OUTPUT 'kSwitch' IF ANY OF THE SCANNED INPUT VARIABLES CHANGE. (OUTPUT 'kSwitch' IS NORMALLY ZERO)
	if	kSwitch=1	then	;IF A VARIABLE CHANGE INDICATOR IS RECEIVED...
		reinit	START		;...BEGIN A REINITIALISATION PASS FROM LABEL 'START' 
	endif				;END OF CONDITIONAL BRANCHING
	START:				;LABEL
	
	/* clip meter */
	krmsL		rms		asigL
	krmsR		rms		asigR
	kon	=	1
	koff	=	0
	if(krmsL>gklimit||krmsR>gklimit) then
	 	chnset	kon,"clip_light"
	else
	 	chnset	koff,"clip_light"
	endif		
	/*------------*/
	
	
	aL		clip 		asigL, i(gkmethod), i(gklimit), i(gkarg)
	aR		clip 		asigR, i(gkmethod), i(gklimit), i(gkarg)
	rireturn			;RETURN TO PERFORMANCE PASSES FROM INITIALISATION PASS
	;		outs		aL*gklevel*(1/(gklimit^0.5)), aR*gklevel*(1/(gklimit^0.5))		;pdclip OUTPUTS ARE SENT TO THE SPEAKERS
			outs		aL*gklevel, aR*gklevel							;pdclip OUTPUTS ARE SENT TO THE SPEAKERS
endin
		
</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
</CsScore>


</CsoundSynthesizer>



























