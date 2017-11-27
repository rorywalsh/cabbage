; pvShift.csd
; Written by Iain McCurdy, 2012.

; Streaming phase vocoding frequency shifter.

<Cabbage>
form caption("pvshift Frequency Shifter") size(510,  90), pluginID("shft"), scrollbars(0)

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

</CsoundSynthesizer>