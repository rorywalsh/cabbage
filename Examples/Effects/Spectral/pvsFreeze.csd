; pvsFreeze.csd
; Freeze either amplitude stream or frequency stream (or both) of a streaming FFT analysis
; Changing FFT size can modify the sound quality of the frozen sound

; 'Auto' can be used to automatically trigger a new freeze frame (both amplitude and frequency) if the amplitude of the sound crosses a threshold
; 'Delay' specifies the time delay at which the sound will actually be frozen after the threshold triggering. This can be employed to capture the sustain portion of a sound (and not the attack).
; 'Threshold' specifies the threshold level at which triggering will occur. In a noisy environment, raise 'Threshold'

<Cabbage>
form caption("pvsfreeze"), size(660,100) colour(  0,160,80), pluginid("frez")

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

</CsoundSynthesizer>