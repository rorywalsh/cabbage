; pvsWarp.csd
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
