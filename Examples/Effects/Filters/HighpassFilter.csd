; HighpassFilter.csd
; Written by Iain McCurdy, 2012.

<Cabbage>
form caption("Highpass Filter"), size(435, 90), pluginid("HPFl") style("legacy")
image pos(0, 0),                size(435, 90), colour(  70,120, 90), shape("rounded"), outlinecolour("white"), outlinethickness(4) 
label    bounds(17, 22, 40, 11), text("INPUT"), fontcolour("white")
combobox bounds(10, 33, 60, 20), channel("input"), value(1), text("Live","Tone","Noise")
rslider  bounds( 75, 11, 70, 70), channel("cf"),        text("Freq."), colour(  0, 60, 30), trackercolour(200,255,200), 	textcolour("white"), 	range(20, 20000, 20, 0.333)
rslider  bounds(140, 11, 70, 70), channel("res"),       text("Res."),  colour(  0, 60, 30), trackercolour(200,255,200), 	textcolour("white"),	range(0,1.00,0)
rslider  bounds(205, 11, 70, 70), channel("mix"),       text("Mix"),   colour(  0, 60, 30), trackercolour(200,255,200),		textcolour("white"), 	range(0,1.00,1)
button  bounds(280,  10, 80, 20), text("12dB/oct","24dB/oct"), channel("steepness"), value(0)
label   bounds(280,  30, 80, 12), text("Steepness"), fontcolour("white")
checkbox bounds(280, 50, 80, 15), channel("ResType"),colour(yellow), fontcolour("white"),  value(0), text("Resonant")
rslider  bounds(360, 11, 70, 70), text("Level"),    colour(  0, 60, 30), trackercolour(200,255,200),		textcolour("white"), 	channel("level"), 	range(0, 1.00, 1)
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-d -n
</CsOptions>

<CsInstruments>

sr 	= 	48000	;SAMPLE RATE
ksmps 	= 	32	;NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls 	= 	2	;NUMBER OF CHANNELS (2=STEREO)
0dbfs	=	1
 
;Author: Iain McCurdy (2012)

instr	1
	kporttime	linseg	0,0.001,0.05
	/* READ IN WIDGETS */
	kcf			chnget	"cf"
	kres		chnget	"res"
	kmix		chnget	"mix"
	ksteepness	chnget	"steepness"
	kResType	chnget	"ResType"
	klevel		chnget	"level"
	klevel		portk	klevel,kporttime
	alevel		interp	klevel
	kcf	portk	kcf,kporttime
	acf	interp	kcf
	/* INPUT */
	kinput		chnget	"input"
	if kinput=1 then
	 aL,aR	ins
	elseif kinput=2 then
	 aL	vco2	0.2, 100
	 aR	=	aL
	else
	 aL	pinkish	0.2
	 aR	pinkish	0.2
	endif
	/* FILTER */
	if ksteepness==0&&kResType!=1 then
	 aFiltL	atone	aL,kcf
	 aFiltR	atone	aR,kcf
        elseif ksteepness==1&&kResType!=1 then
	 aFiltL	buthp	aL,acf
	 aFiltR	buthp	aR,acf
        elseif kResType==1 then
	 aFiltL	bqrez	aL,acf,1+(kres*40),1
	 aFiltR	bqrez	aR,acf,1+(kres*40),1
	endif
	aL	ntrpol	aL,aFiltL,kmix
	aR	ntrpol	aR,aFiltR,kmix
		outs	aL*alevel,aR*alevel
endin
		
</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
e
</CsScore>

</CsoundSynthesizer>
