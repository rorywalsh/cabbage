; FractalNoise.csd
; Written by Iain McCurdy, 2015

; GUI colour indicates noise type: 'white' - 'pink' - 'brown'

<Cabbage>
form caption("Fractal Noise"), size(290,275), pluginid("fnse"), guirefresh(32)
image bounds(  0,  0,290,275), colour("white"), shape("sharp"), , identchannel(BackgroundColour)
checkbox bounds( 20, 10, 15, 15), channel("onoff"), value(0), fontcolour("black"), colour("yellow")
label    bounds( 37, 11, 50, 13), text("On/Off"), fontcolour(30,30,30), identchannel("label1")
rslider  bounds(100, 10, 50, 50), channel("amp"),     range(0, 2, 0.2, 0.5, 0.001), textcolour("black"), trackercolour("yellow"), colour(30,30,30)
label    bounds(100, 60, 50, 13), text("Amp."), fontcolour(30,30,30), identchannel("label2")
rslider  bounds(160, 10, 50, 50), channel("beta"),    range(-2, 5, 0, 1, 0.001),    textcolour("black"), trackercolour("yellow"), colour(30,30,30)
label    bounds(160, 60, 50, 13), text("Beta"), fontcolour(30,30,30), identchannel("label3")
rslider  bounds(220, 10, 50, 50), channel("width"),   range(0,0.05, 0, 0.5, 0.0001),    textcolour("black"), trackercolour("yellow"), colour(30,30,30)
label    bounds(220, 60, 50, 13), text("Width"), fontcolour(30,30,30), identchannel("label4")
gentable bounds(  5, 85 ,280, 90), tablenumber(10), amprange(-1,1,1), identchannel("table"),zoom(-1)
label    bounds(  7, 85, 100, 11), text("Amp.Waveform"), fontcolour(255,255,255,150), align(left)
gentable bounds(  5,180 ,280, 90), tablenumber(11), identchannel("FFT"), amprange(0,1,-1), outlinethickness(0), samplerange(0, 128), tablecolour("yellow"), zoom(-1)
label    bounds(  7,180, 100, 11), text("FFT Spectrum"), fontcolour(255,255,255,150), align(left)
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-dm0 -n -+rtmidi=null -M0
</CsOptions>

<CsInstruments>

sr 		= 	44100
ksmps 		= 	16
nchnls 		= 	2
0dbfs		=	1

ginoise		ftgen	10,0,128,10,1
giFFT		ftgen	11,0,256,10,1

giFracScal	ftgen	1,0,-700,-27, 0,1.1, 100,1.1, 200,1.1, 300,0.42, 400,0.08, 500,0.008, 600,0.001, 700,0.0002 
;			              white    white     pink     brown     black
giR		ftgen	2,0,-700,-27,  0,255, 200,255,  300,255, 400,80,   700,1 
giG		ftgen	3,0,-700,-27,  0,255, 200,255,  300,130, 400,40,   700,1 
giB		ftgen	4,0,-700,-27,  0,255, 200,255,  300,130, 400,0 ,   700,1 



instr	1
	konoff	chnget	"onoff"		;read in on/off switch widget value
	kamp		chnget	"amp"
	kamp		port	kamp,0.01
	kbeta		chnget	"beta"
	kbeta		port	kbeta,0.1
	kbeta		init	0
	kwidth		chnget	"width"
	kwidth		portk	kwidth,0.05
	kscal		table	(kbeta+2)/7,giFracScal,1
	aL	fractalnoise	kscal*0.5*konoff, kbeta
	if kwidth>0.0001 then
	 aR	vdelay		aL,kwidth*1000,100
	else
	 aR	=	aL
	endif
		outs	aL*kamp, aR*kamp

	kptr	init	0
		tabw k(aL), kptr, 10
	kptr	=	(kptr + 1) % 128

	if metro(16)==1 then
	 if changed(kbeta)==1 then
	  kR	tab	(kbeta+2)/7,giR,1
	  kG	tab	(kbeta+2)/7,giG,1
	  kB	tab	(kbeta+2)/7,giB,1
	  kR	init	255
	  kG	init	255
	  kB	init	255
	  Smsg	sprintfk	"colour(%d,%d,%d)",kR,kG,kB
	  	chnset		Smsg,"BackgroundColour"
	 endif

		chnset	"tablenumbers(10)","table"

	endif

	
	kFlickOn	trigger	kbeta,2,0
	kFlickOff	trigger	kbeta,2,1
	if kFlickOn==1 then
	  	chnset		"fontcolour(255,255,255)","label1"
	  	chnset		"fontcolour(255,255,255)","label2"
	  	chnset		"fontcolour(255,255,255)","label3"
	  	chnset		"fontcolour(255,255,255)","label4"
	elseif kFlickOff==1 then
	  	chnset		"fontcolour(0,0,0)","label1"
	  	chnset		"fontcolour(0,0,0)","label2"
	  	chnset		"fontcolour(0,0,0)","label3"
	  	chnset		"fontcolour(0,0,0)","label4"
	endif
	
	fsig	pvsanal	aL*3, 256,64,256,1
  	kflag	pvsftw fsig, 11
 	if kflag==1 then
 	 chnset	"tablenumber(11)", "FFT"
 	endif

endin


</CsInstruments>

<CsScore>
i 1 0 [60*60*24*7]	;instrument that reads in widget data
</CsScore>

</CsoundSynthesizer>