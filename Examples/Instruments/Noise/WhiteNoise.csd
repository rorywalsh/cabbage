<Cabbage>
form caption("White Noise"), size(230, 90), pluginID("wnse")
image bounds(  0,  0,230, 90), colour("white"), shape("rounded"), outlinecolour("black"), outlinethickness(4) 
checkbox bounds( 20, 10, 80, 15), text("On/Off"), channel("onoff"), value(0), fontcolour("black")
rslider  bounds( 80, 10, 70, 70), text("Amplitude"), channel("amp"),  outlinecolour("DarkGrey"), colour("black"), range(0, 1, 0.1, 0.5, 0.001),   textcolour("black"), trackercolour("grey")
rslider  bounds(150, 10, 70, 70), text("Beta"),      channel("beta"), outlinecolour("DarkGrey"), colour("black"), range(-0.999, 0.999,0,1,0.001), textcolour("black"), trackercolour("grey")
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-dm0 -n -+rtmidi=null -M0
</CsOptions>

<CsInstruments>

sr 		= 	44100
ksmps 		= 	64
nchnls 		= 	2
0dbfs		=	1	;MAXIMUM AMPLITUDE
massign	0,0

instr	1
	kporttime	linseg	0,0.001,0.05
	konoff	chnget	"onoff"		;read in on/off switch widget value
	if konoff==0 goto SKIP		;if on/off switch is off jump to skip label
	kamp	chnget	"amp"		;read in widgets...
	kamp	portk	kamp,kporttime
	kbeta	chnget	"beta"		;
	asigL 	noise 	kamp, kbeta	;GENERATE WHITE NOISE
	asigR 	noise 	kamp, kbeta	;GENERATE WHITE NOISE
		outs	asigL,asigR	;SEND AUDIO SIGNAL TO OUTPUT
	SKIP:				;A label. Skip to here is on/off switch is off 
endin


</CsInstruments>

<CsScore>
i 1 0 [60*60*24*7]	;instrument that reads in widget data
</CsScore>

</CsoundSynthesizer>