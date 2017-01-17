; BandFilter.csd
; Written by Iain McCurdy, 2012.

<Cabbage>
form caption("Band Filter"), colour("DarkSlateGrey"), size(470, 360), pluginID("band")  
xypad bounds(5, 5, 350, 350), channel("cf", "bw"), rangex(0, 1, 0.5), rangey(0, 1, 0.3), text("x:cutoff | y:bandwidth"),colour(127,159,159)
checkbox bounds(370, 10, 20, 20), channel("balance"), FontColour("white"),  value(0)
label    bounds(395, 15, 55, 15), text("Balance"), FontColour("white")

label   bounds(375,  43, 75, 15), text("Filter Type"), FontColour("white")
combobox bounds(370,  60,85, 20), channel("type"), value(1), text("reson", "butterbp", "areson", "butterbr")

rslider bounds(368, 93, 90, 90), text("Mix"),		colour(27,59,59),trackercolour(127,159,159),textcolour("white"),fontcolour("white"), 		channel("mix"), 	range(0, 1.00, 1)
rslider bounds(368,190, 90, 90), text("Level"),		colour(27,59,59),trackercolour(127,159,159),textcolour("white"),fontcolour("white"), 		channel("level"), 	range(0, 1.00, 1)

numberbox  bounds(360,283, 50, 30), text("CF"), textcolour("white"), channel("cfDisp"), range(1, 20000, 1, 1, 1)
numberbox  bounds(415,283, 50, 30), text("BW"), textcolour("white"), channel("bwDisp"), range(1, 20000, 1, 1, 1)

image bounds(75, 343, 315, 18), colour(75, 85, 90, 100), plant("credit"), line(0){
}
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-dm0 -n -+rtmidi=null -M0
</CsOptions>

<CsInstruments>
sr 	= 	44100
ksmps 	= 	32
nchnls 	= 	2
0dbfs	=	1

;Author: Iain McCurdy (2012)

instr	1
	kcf		chnget	"cf"
	kbw		chnget	"bw"
	kbalance	chnget	"balance"
	ktype		chnget	"type"
	kmix		chnget	"mix"
	klevel		chnget	"level"
	kporttime	linseg	0,0.001,0.05

	kcf	expcurve	kcf, 4
	kcf	scale	kcf,18000,20

	kbw	expcurve	kbw, 16
	kbw	scale	kbw,3,0.01

	aL,aR	ins
	;aL,aR	diskin2	"seashore.wav",1,0,1
	;aL	pinkish	1	;USE FOR TESTING
	;aR	pinkish	1
	
	kbw	limit	kbw*kcf,1,20000
	
	chnset	kcf,"cfDisp"	;send actual values for cutoff and bandwidth to GUI value boxes
	chnset	kbw,"bwDisp"

	kcf	portk	kcf,kporttime
	kbw	portk	kbw,kporttime	

	if ktype==1 then		;if reson chosen...
	 aFiltL	reson	aL,kcf,kbw,1
	 aFiltR	reson	aR,kcf,kbw,1
	elseif ktype==2 then		;or if butterworth bandpass is chosen
	 aFiltL	butbp	aL,kcf,kbw
	 aFiltR	butbp	aR,kcf,kbw
	elseif ktype==3 then		;or if areson  is chosen...
	 aFiltL	areson	aL,kcf,kbw,1
	 aFiltR	areson	aR,kcf,kbw,1
	else				;otherwise must be butterworth band reject
	 aFiltL	butbr	aL,kcf,kbw
	 aFiltR	butbr	aR,kcf,kbw
	endif
	if kbalance==1 then		;if 'balance' switch is on...
	 aFiltL	balance	aFiltL,aL,0.3	
	 aFiltR	balance	aFiltR,aR,0.3
	endif
	amixL	ntrpol	aL,aFiltL,kmix	;create wet/dry mix
	amixR	ntrpol	aR,aFiltR,kmix
		outs	amixL*klevel,amixR*klevel
endin

</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
</CsScore>

</CsoundSynthesizer>
