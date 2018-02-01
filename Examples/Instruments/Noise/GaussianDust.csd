GaussianDust.csd
Iain McCurdy [2013]

A simple encapsulation of the 'gausstrig' opcode.
Added features are stereo panning (spread) of the dust, a random tonal variation (lowpass filter with jumping cutoff frequency) and constant low and highpass filters.

<Cabbage>
form caption("Gaussian Dust"), size(490, 350), pluginid("gaus")
image                  bounds(0, 0, 490, 350), colour("DarkSlateGrey"), shape("sharp"), outlinecolour("white"), outlinethickness(4) 
checkbox bounds( 15, 10, 80, 15), text("On/Off"), channel("onoff"), value(1), fontcolour("white")
combobox bounds( 10, 40, 70, 20), channel("mode"), value(2), text("Held", "Reinit")
xypad bounds(  5, 78, 240, 260), text("Freq./Deviation"), channel("freq", "dev"), rangex(0.1, 10000, 10), rangey(0, 10.00, 0)
xypad bounds(245, 78, 240, 260), text("LPF./HPF."), channel("LPF", "HPF"), rangex(20, 20000, 20000), rangey(20, 20000,20)

rslider  bounds(90, 10, 60, 60),  text("Amplitude"), channel("amp"),     range(0, 1, 0.5, 0.5, 0.001),      fontcolour("white"), colour(50,60,70), trackercolour(150,160,170), outlinecolour("SlateGrey")
rslider  bounds(150, 10, 60, 60), text("Freq."),     channel("freq"),    range(0.1, 10000, 10, 0.25, 0.01), fontcolour("white"), colour(50,60,70), trackercolour(150,160,170), outlinecolour("SlateGrey")
rslider  bounds(205, 10, 60, 60), text("Deviation"), channel("dev"),     range(0, 10, 1),                   fontcolour("white"), colour(50,60,70), trackercolour(150,160,170), outlinecolour("SlateGrey")
rslider  bounds(260, 10, 60, 60), text("Spread"),    channel("spread"),  range(0, 1, 1),                    fontcolour("white"), colour(50,60,70), trackercolour(150,160,170), outlinecolour("SlateGrey")
rslider  bounds(315, 10, 60, 60), text("Tone Var."), channel("ToneVar"), range(0, 1.00, 0),                 fontcolour("white"), colour(50,60,70), trackercolour(150,160,170), outlinecolour("SlateGrey")
rslider  bounds(370, 10, 60, 60), text("Lowpass"),   channel("LPF"),     range(20,20000,20000,0.5),         fontcolour("white"), colour(50,60,70), trackercolour(150,160,170), outlinecolour("SlateGrey")
rslider  bounds(425, 10, 60, 60), text("Highpass"),  channel("HPF"),     range(20,20000,20,0.5),            fontcolour("white"), colour(50,60,70), trackercolour(150,160,170), outlinecolour("SlateGrey")
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-dm0 -n -+rtmidi=null -M0
</CsOptions>

<CsInstruments>

sr 		= 	44100
ksmps 		= 	64
nchnls 		= 	2
0dbfs		=	1

instr	1
	konoff	chnget	"onoff"		;read in on/off switch widget value
	if konoff==0 goto SKIP		;if on/off switch is off jump to skip label
	kamp	chnget	"amp"
	kfreq	chnget	"freq"
	kdev	chnget	"dev"
	kporttime	linseg	0,0.01,0.1
	kdev	portk	kdev,kporttime
	kmode	chnget	"mode"
	kmode	=	kmode - 1
	kspread	chnget	"spread"
	ktrig	changed		kmode		;IF gkmode COUNTER IS CHANGED GENERATE A MOMENTARY '1' IMPULSE
	if ktrig==1 then			;THEREFORE IF gkmode HAS BEEN CHANGED...
	 reinit	UPDATE				;BEGIN A REINITIALISATION PASS AT LABEL 'UPDATE'
	endif					;END OF CONDITIONAL BRANCH
	UPDATE:					;LABEL 'UPDATE'. REINITIALISATION BEGINS FROM HERE.
	asig	gausstrig	kamp, kfreq, kdev, i(kmode), 1	;GENERATE GAUSSIAN TRIGGERS
	kpan	random	0.5-(kspread*0.5), 0.5+(kspread*0.5)
	asigL,asigR	pan2	asig,kpan
	rireturn				;RETURN FROM REINITIALISATION PASS


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

		outs	asigL,asigR		;SEND AUDIO SIGNAL TO OUTPUT
	SKIP:					;A label. Skip to here is on/off switch is off 
endin


</CsInstruments>

<CsScore>
i 1 0 [60*60*24*7]	;instrument plays for a week
</CsScore>

</CsoundSynthesizer>