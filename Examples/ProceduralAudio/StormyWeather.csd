; Stormy Weather
; Iain McCurdy, 2017

<Cabbage>
form caption("Stormy Weather") size(300, 310), pluginId("StWr")  style("legacy")
button  bounds( 10,  5,70, 30), text("Wind","Wind"), latched(1), fontColour:0(100,100,100), channel("Wind"), value(1)

rslider bounds(  5, 40, 80, 80), channel("WindLev"), text("Level"), range(0, 1, .5, 1, .01)
rslider bounds(  5,130, 80, 80), channel("WindLayers"), text("Layers"), range(1,10, 2, 1, 1)

button  bounds(110,  5,70, 30), text("Thunder","Thunder"), fontColour:0(100,100,100), channel("Thunder")
rslider bounds(105, 40, 80, 80), channel("ThunderLev"), text("Level"), range(0, 1, .5, 1, .01)
rslider bounds(105,130, 80, 80), channel("ThunderDur"), text("Duration"), range(4, 25, 11)
rslider bounds(105,220, 80, 80), channel("ThunderDist"), text("Distance"), range(0, 1, 0.1)

button  bounds(210,  5,70, 30), text("Rain","Rain"), latched(1), fontColour:0(100,100,100), channel("Rain"), value(1)
rslider bounds(205, 40, 80, 80), channel("RainLev"), text("Level"), range(0, 1, .5, 1, .01)
rslider bounds(205,130, 80, 80), channel("RainMix"), text("Mix"), range(0, 1, .5)
rslider bounds(205,220, 80, 80), channel("RainDens"), text("Density"), range(0, 1, .5)


</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 -m0d
</CsOptions>
<CsInstruments>
; Initialize the global variables.
;sr is set by the host
ksmps = 16
nchnls = 2
0dbfs = 1
seed 0

instr	1
 gkWind			chnget	"Wind"		; on/off
 gkWindLev		chnget	"WindLev"

 kThunder		chnget	"Thunder"	; on/off
 kThunderDur	chnget	"ThunderDur"	; on/off
 kThunderDist	scale	chnget:k("ThunderDist"),1,1.7

 kRain			chnget	"Rain"		; on/off


 if trigger:k(gkWind,0.5,0)==1 then
  event	"i",2,0,3600
 endif

 if trigger:k(kThunder,0.5,0)==1 then
  event	"i",3,0,kThunderDur,kThunderDist
  event	"i",3,0,kThunderDur,kThunderDist
 endif

  if trigger:k(kRain,0.5,0)==1 then
  event	"i",4,0,3600
 elseif trigger:k(kRain,0.5,1)==1 then
  turnoff2	4,0,1
 endif

endin

opcode	Wind,aa,ip
 iLayers,iCount	xin
 aML		=		0
 aMR		=		0
 kdB		rspline	-3, 0, 5, 15
 kenv		expseg	0.01,4,1,1,1
 aNoise		dust2		0.02*ampdbfs(kdB)*kenv*gkWindLev, 12000
 kCF		rspline	8,12.5,0.1,0.2
 kBW		rspline	0.04,0.2,0.2,2
 aNoise		reson		aNoise, cpsoct(kCF), cpsoct(kCF)*kBW, 2
 kpan		rspline	0.1,0.9,0.4,0.8
 aL,aR		pan2		aNoise,kpan
 if iCount<iLayers then
  aML,aMR	Wind	iLayers,iCount+1
 endif
 		xout		aL+aML, aR+aMR
endop

instr		2	; howling wind
 if gkWind==0 then
  turnoff
 endif
 kLayers	chnget	"WindLayers"
 if changed(kLayers)==1 then
  reinit RELAYER
 endif
 RELAYER:
 aL,aR	Wind	i(kLayers)
 		outs	aL,aR

		chnmix	aL*0.3, "SendL"						; also send to the reverb
 		chnmix	aR*0.3, "SendR"
endin




instr		3	; thunder
 kenv		expseg	0.01, 0.05, 1, 0.1, 0.5, p3-0.01, 0.01
 aNse		pinkish	kenv*0.6
 kCF		expon		p4,p3,0.03
 kCFoct		randomh	2*kCF,6*kCF,20
 aNse		reson		aNse*3,a(cpsoct(kCFoct)),a(cpsoct(kCFoct)*5),1
 aNse		butlp		aNse, 1000
 ipan		random	0,1
 aL,aR	pan2		aNse,ipan
 		outs		aL, aR
		chnmix	aL*0.15, "SendL"						; also send to the reverb
 		chnmix	aR*0.15, "SendR"
endin





instr		4	; rain
 kRainMix	chnget	"RainMix"
 kRainDens	chnget	"RainDens"

 kTrig	dust		1, 500*kRainDens
 kenv		linsegr	0,2,1,5,0
 		schedkwhen	kTrig, 0, 0, 5, 0, 0.008, kenv*(1-sqrt(kRainMix))
 aNse	dust2		0.1*kenv*sqrt(kRainMix),3000*kRainDens
 aNse2	dust2		0.1*kenv*sqrt(kRainMix),1500*kRainDens
 aNse	butlp		aNse, 1000
 aNse2	butlp		aNse2, 1000
 		outs		aNse,aNse2
endin
instr		5	; rain
 iCPS1		random	10,11
 iCPS2		random	13,14
 idB		random	-10,-32
 aCPS		expon		cpsoct(iCPS1),p3,cpsoct(iCPS2)
 aEnv		expon		1,p3,0.001
 aSig		poscil	aEnv*ampdbfs(idB)*p4,aCPS
 aSig		buthp		aSig,9000
 ipan		random	0,1
 aL,aR	pan2		aSig,ipan
 		outs		aL, aR
		chnmix	aL*0.3, "SendL"						; also send to the reverb
 		chnmix	aR*0.3, "SendR"
endin



instr		999	; reverb
 aInL		chnget	"SendL"
 aInR		chnget	"SendR"
 aL, aR	reverbsc	aInL, aInR, 0.9, 12000
 		outs		aL, aR
 		chnclear	"SendL"
 		chnclear	"SendR"
endin

</CsInstruments>
<CsScore>
i 1 0 [60*60*24*7]

i 999 0 [60*60*24*7]
</CsScore>
</CsoundSynthesizer>
