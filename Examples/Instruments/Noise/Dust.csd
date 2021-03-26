Dust.csd
Written by Iain McCurdy, 2013.

A simple encapsulation of the 'dust' opcode.
Added features are stereo panning (spread) of the dust, a random tonal variation (lowpass filter with jumping cutoff frequency) and constant low and highpass filters.


<Cabbage>
form caption("Dust"), size(410, 10), pluginId("dust") style("legacy")
image    bounds(  0,  0,410, 80), colour(155, 50, 50), shape("sharp"), outlineColour("white"), outlineThickness(2) 
checkbox bounds( 10, 10, 80, 15), text("On/Off"), channel("onoff"), value(0), fontColour("white")
rslider  bounds( 70, 10, 60, 60), text("Amplitude"), channel("amp"),     range(0, 1.00, 0.5, 0.5, 0.001),   textColour("white"), colour(105, 20, 20), outlineColour(155,100,100), trackerColour(220,160,160)
rslider  bounds(125, 10, 60, 60), text("Freq."),     channel("freq"),    range(0.1, 20000, 500, 0.5, 0.01), textColour("white"), colour(105, 20, 20), outlineColour(155,100,100), trackerColour(220,160,160)
rslider  bounds(180, 10, 60, 60), text("Spread"),    channel("spread"),  range(0, 1.00, 1),                 textColour("white"), colour(105, 20, 20), outlineColour(155,100,100), trackerColour(220,160,160)
rslider  bounds(235, 10, 60, 60), text("Tone Var."), channel("ToneVar"), range(0, 1.00, 0),                 textColour("white"), colour(105, 20, 20), outlineColour(155,100,100), trackerColour(220,160,160)
rslider  bounds(290, 10, 60, 60), text("Lowpass"),   channel("LPF"),     range(20,20000,20000,0.5),         textColour("white"), colour(105, 20, 20), outlineColour(155,100,100), trackerColour(220,160,160)
rslider  bounds(345, 10, 60, 60), text("Highpass"),  channel("HPF"),     range(20,20000,20,0.5),            textColour("white"), colour(105, 20, 20), outlineColour(155,100,100), trackerColour(220,160,160)
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-dm0 -n -+rtmidi=null -M0
</CsOptions>

<CsInstruments>

;sr is set by the host
ksmps 		= 	64
nchnls 		= 	2
0dbfs		=	1	;MAXIMUM AMPLITUDE
massign	0,0

instr	1
	konoff	chnget	"onoff"		;read in on/off switch widget value
	if konoff==0 goto SKIP		;if on/off switch is off jump to 'SKIP' label
	kamp	chnget	"amp"
	kfreq	chnget	"freq"
	kspread	chnget	"spread"
	asig	dust2	kamp, kfreq	;GENERATE 'dust2' IMPULSES

	; tone variation
	kToneVar	chnget	"ToneVar"
	if(kToneVar>0) then
 	 kcfoct	random		14-(kToneVar*10),14
	 asig	tonex		asig,cpsoct(kcfoct),1
	endif

	kpan	random	0.5-(kspread*0.5), 0.5+(kspread*0.5)
	asigL,asigR	pan2	asig,kpan

	kporttime	linseg	0,0.001,0.05

	; Lowpass Filter
	kLPF	chnget	"LPF"
	if kLPF<20000 then
	 kLPF	portk	kLPF,kporttime
	 asigL	clfilt	asigL,kLPF,0,2
	 asigR	clfilt	asigR,kLPF,0,2
	endif
	
	; Highpass Filter
	kHPF	chnget	"HPF"
	if kHPF>20 then
	 kHPF	limit	kHPF,20,kLPF
	 kHPF	portk	kHPF,kporttime
	 asigL	clfilt	asigL,kHPF,1,2
	 asigR	clfilt	asigR,kHPF,1,2
	endif

		outs	asigL,asigR	;SEND AUDIO SIGNAL TO OUTPUT
	SKIP:				;A label. Skip to here is on/off switch is off 
endin


</CsInstruments>

<CsScore>
i 1 0 [60*60*24*7]	;instrument that reads in widget data
</CsScore>

</CsoundSynthesizer>