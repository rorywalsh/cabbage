; pvsAdsyn.csd
; Written by Iain McCurdy, 2012.

; Encapsulation of the pvsadsyn opcode.
; This opcode takes a streaming phase vocoding analysis and reconstitutes it as an audio signal with user-definable parameters
;  for the number of bins to include, the bin from which to begin resynthesis (bin offset) and the option of skipping bins and not
;  resynthesising them one after another (Bin Incr.)

<Cabbage>
form caption("pvsAdsyn") size(580, 90), pluginID("adsy")
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

</CsoundSynthesizer>