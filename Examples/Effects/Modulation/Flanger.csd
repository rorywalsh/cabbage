; Flanger.csd
; Written by Iain McCurdy, 2012.

<Cabbage>
form caption("Flanger") size(510, 90), pluginID("flan")
image pos(0, 0), size(510, 90), colour("lightgreen"), shape("rounded"), outlinecolour("white"), outlinethickness(4) 
rslider bounds(  5, 10, 70, 70), text("Rate"),     channel("rate"),  range(0.001, 40, 0.15, 0.5, 0.001),      colour("DarkGreen"), trackercolour( 50,150, 50), textcolour(  0, 30,  0)
rslider bounds( 75, 10, 70, 70), text("Depth"),    channel("depth"), range(0, 0.01, 0.005,1,0.0001),          colour("DarkGreen"), trackercolour( 50,150, 50), textcolour(  0, 30,  0)
rslider bounds(145, 10, 70, 70), text("Delay"),    channel("delay"), range(0.0001, 0.1, 0.0001, 0.5, 0.0001), colour("DarkGreen"), trackercolour( 50,150, 50), textcolour(  0, 30,  0)
rslider bounds(215, 10, 70, 70), text("Feedback"), channel("fback"), range(-1, 1, 0),                         colour("DarkGreen"), trackercolour( 50,150, 50), textcolour(  0, 30,  0)
rslider bounds(285, 10, 70, 70), text("Mix"),      channel("mix"),   range(0, 1.00, 0.5),                     colour("DarkGreen"), trackercolour( 50,150, 50), textcolour(  0, 30,  0)
checkbox bounds(355, 10, 80, 15), colour("yellow"), channel("ThruZero"),  value(1), text("Thru.Zero"), fontcolour(  0, 30,  0)
label    bounds(360, 37, 65, 12), text("LFO Shape:"), fontcolour(  0, 30,  0)
combobox bounds(355, 50, 80, 18), channel("lfoshape"), value(1), text("parabola", "sine", "triangle", "randomi", "randomh")
rslider bounds(435, 10, 70, 70), text("Level"), channel("level"), range(0, 1.00, 1), colour("DarkGreen"), trackercolour( 50,150, 50), textcolour(  0, 30,  0)
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
	
	;a1,a2	ins					;read live stereo audio input
	a1	pinkish	0.2				;for testing...
	a2	pinkish	0.2

	kporttime	linseg	0,0.001,0.1
	kdelay	portk	kdelay,kporttime

	afla1	Flanger	a1,krate,kdepth,kdelay,kfback,klfoshape	;call udo (left channel)
	afla2	Flanger	a2,krate,kdepth,kdelay,kfback,klfoshape	;call udo (right channel)

	if kThruZero==1 then				;if 'Thru.Zero' mode is selected...
	 a1	delay	a1,0.0001
	 a2	delay	a2,0.0001
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

</CsoundSynthesizer>