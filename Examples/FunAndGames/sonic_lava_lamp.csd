; SonicLavaLamp.csd
; Written by Iain McCurdy, 2012.

<Cabbage>
form caption("Sonic Lava Lamp") size(510, 450), pluginId("sonl") style("legacy")
image bounds(  5,  0,250, 210), colour(255,255, 50,100),shape("rounded"), oulinecolour("white"), line(0)
image bounds(255,  0,250, 210), colour(  0,  0,255,100),shape("rounded"), oulinecolour("white"), line(0)
image bounds(  5,210,250, 210), colour(  5,255,  0,100),shape("rounded"), oulinecolour("white"), line(0)
image bounds(255,210,250, 210), colour(255,  0,200,100),shape("rounded"), oulinecolour("white"), line(0)

xypad bounds(5, 0, 250, 210), channel("LFOspeed", "LFO_OS"), rangeX(0, 1.00, 0.01), rangeY(5, 11, 8), text("Speed/OS")
xypad bounds(255, 0, 250, 210), channel("LFOshape", "LFOdepth"), rangeX(0, 1, 0.7), rangeY(0.1, 4, 2), text("Shape/Depth")
xypad bounds(5, 210, 250, 210), channel("pan", "amp"), rangeX(0, 1, 0.5), rangeY(0, 1, 0.8), text("pan/amp")
xypad bounds(255, 210, 250, 210), channel("DelTim", "DelSnd"), rangeX(0, 1.00, 0.5), rangeY(0, 1.00, 0.5), text("Delay Time/Send")
infobutton bounds(5,425, 100, 20), text("Help"), file("SonicLavaLampHelp.html")
image bounds(105, 425, 300, 20), colour(75, 85, 90, 100), plant("credit"), line(0){
label bounds(0.03, 0.1, .9, .7), text("Author: Iain McCurdy |2012|"), fontColour("white")
}
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-dm0 -n -+rtmidi=null -M0
</CsOptions>
<CsInstruments>
;sr is set by the host
ksmps 	= 	32
nchnls 	= 	2
0dbfs	=	1

;Author: Iain McCurdy (2012)

gisine		ftgen	0, 0, 4096, 10, 1				;SINE WAVE
giSawUp		ftgen	0, 0, 4096, -7, -1, 4096, 1
giTri		ftgen	0, 0, 4096, -7,  -1, 2048, 1, 2048, -1
giSawDn		ftgen	0, 0, 4096, -7, 1, 4096, -1
giSq		ftgen	0, 0, 4096, -7, 1, 2048, 1, 0, -1, 2048, -1
giMorfTab	ftgen	0, 0, 4096, -7, 1, 2048, 1, 0, -1, 2048, -1
giTabs4Morf	ftgen	0, 0, 8, -2, giSq, giSawUp, giTri, giSawDn, giSq, giSq, giSq, giSq
giAmpCurve	ftgen	0, 0, 16, -16,   1, 6, 0, 1,   5, -4, 0.2,   5, 0, 0.2
giEnv		ftgen	0, 0, 4096, 20,3,1	;9, 0.5, 1, 0

opcode	PingPongDelay, aa,aakk
	ainL,ainR,ktime,kfeedback	xin
	atime		interp	ktime					;INTERPOLATED A-RATE VERSION OF DELAY TIME
	aL_OS		vdelay	ainL,(atime*1000)/2,(10*1000)/2	;DELAYED OFFSET OF LEFT CHANNEL (FIRST 'PING')
	;LEFT CHANNEL
	abuf		delayr	10					;ESTABLISH DELAY BUFFER
	aDelL		deltapi	atime					;TAP BUFFER
			delayw	aL_OS+(aDelL*kfeedback)			;WRITE INPUT AUDIO INTO BUFFER
	;RIGHT CHANNEL
	abuf		delayr	10					;ESTABLISH DELAY BUFFER
	aDelR		deltapi	atime					;TAP BUFFER
			delayw	ainR+(aDelR*kfeedback)			;WRITE INPUT AUDIO INTO BUFFER
			xout	aDelL+aL_OS,aDelR
endop

instr	InitialiseXYpads
; xypads output zeroes until they are moved (regardless of any initialisation done within the widget)
; a workaround is to initialise the xypad using chnset within an instrument
; here I have used a macro so cut down on code repetition
#define	SET_XYPAD_INIT(NAME'VAL)#
	kval	=	$VAL
	chnset	kval,"$NAME"#
$SET_XYPAD_INIT(LFOspeed'0.01)
$SET_XYPAD_INIT(LFO_OS'8)
$SET_XYPAD_INIT(LFOshape'0.7)
$SET_XYPAD_INIT(LFOdepth'2)
$SET_XYPAD_INIT(pan'0.5)
$SET_XYPAD_INIT(amp'0.8)
$SET_XYPAD_INIT(DelTim'0.5)
$SET_XYPAD_INIT(DelSnd'0.5)
endin

instr	1
	kLFOspeed	chnget	"LFOspeed"
	kLFO_OS		chnget	"LFO_OS"
	kLFOshape	chnget	"LFOshape"
	kLFOdepth	chnget	"LFOdepth"
	kamp		chnget	"amp"
	kpan		chnget	"pan"
	kDelTim		chnget	"DelTim"
	kDelSnd		chnget	"DelSnd"

	kLFOspeed	expcurve	kLFOspeed,6
	kLFOspeed	scale	kLFOspeed, 40,0.01
	kDelTim		expcurve	kDelTim,6
	kDelTim		scale	kDelTim, 0.5,0.01

	kporttime	linseg	0,0.01,0.03
	kDelSnd		portk	kDelSnd,kporttime*3
	kLFOdepth	portk	kLFOdepth,kporttime
	kLFO_OS		portk	kLFO_OS,kporttime
	kLFOshape	portk	kLFOshape,kporttime
	kamp		portk	kamp,kporttime
	kamp		expcurve	kamp,2
	kpan		portk	kpan,kporttime
			ftmorf	kLFOshape*5, giTabs4Morf, giMorfTab
	aoctLFO		oscili	kLFOdepth,kLFOspeed,giMorfTab
	aoct		=	aoctLFO+kLFO_OS
	koct		downsamp	aoct
	kAmpCurve	tablei	koct,giAmpCurve
	kcps		=	cpsoct(koct)
	aamp		interp	kAmpCurve/3
	asig		oscili	aamp,kcps,gisine	;AUDIO OSCILLATOR
	aenv		oscili	1,kLFOspeed,giEnv
	kEnvXFade	jspline	1,0.2,0.6
	kEnvXFade	limit	kEnvXFade,0,1
	asig		ntrpol	asig*aenv,asig,kEnvXFade

	
	aL,aR		pan2	asig,kpan
	aRvbL,aRvbR	reverbsc	aL,aR,0.8,4000
	aL		sum	(aL*kamp),(aRvbL*0.1)
	aR		sum	(aR*kamp),(aRvbR*0.1)
	kDelTim		portk	kDelTim,kporttime*10
	aDlyL,aDlyR	PingPongDelay	aL*kDelSnd,aR*kDelSnd,2*kDelTim,0.6
	aL		sum	aL,(aDlyL*0.5)
	aR		sum	aR,(aDlyR*0.5)

			outs	aL, aR
endin

</CsInstruments>

<CsScore>
i "InitialiseXYpads" 0.1 0.01
i 1 0.2 [3600*24*7]
</CsScore>

</CsoundSynthesizer>
