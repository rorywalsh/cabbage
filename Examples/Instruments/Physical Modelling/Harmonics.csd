; Harmonics.csd
; Written by Iain McCurdy, 2014

; Emulation of a piano string-hammer mechanism with a damping node placed some way along the string to force harmonic overtones
; Each note played triggers two strings, which can be detuned with repect to each other.
; As well as emulating the hammer striking the string in the first place, a release hammer (or string damping mechanism is emulated)

; AMPLITUDE
; Amp.		-	Global amplitude control
; Vel.		-	key velocity influence upon hammer amplitude

; FUNDEMENTAL (how midi note maps to fundemental frequency of the tone)
; Offset	-	fixed offset applied to all values in the mapping. For normal keyboard mapping set to '0'
; Rnge.		-	range of the mapping. For normal keyboard mapping set to '1'

; HARMONIC
; Offset	-	offset of harmonic position (0 = no harmonic)
; Vel		-	key velocity influence upon harmonic position
; Kybd.Track	-	the amount to which keyboard tracking influences harmonic position

; STRING
; Detune	-	maximum possible detuning between the two strings (in cents) triggered by each note (the actual detuning will be unique and fixed for each note)
; Release	-	envelope release time once a key is released
; Sustain	-	the sustain time of the strings. (Feedback in the double waveguide.)
; Damping	-	high frequency damping. (Cutoff frequencies of two lowpass filters in the waveguide network.)

; HAMMER (keyboard mapping for the frequency of the hammer that strikes the string)
; Vel.		-	key velocity influence upon hammer frequency (hammer impulse only sustains for one cycle so this effectively controls the duration or period of that cycle)
; Offset	-	Fundemental frequency of the hammer (before keyboard tracking is applied)
; Keybd.Track	-	Amount of keyboard tracking to be applied. Increasing this value causes higher note to use a higher frequency hammer impulse.

; RELEASE HAMMER (string vibration stopping mechanism)
; Ampl.		-	Amplitude of the release hammer. Set to zero to remove the release hammer altogether. 
;			Release hammer amplitude is also affected by the current vibration amplitude of the strings so that the longer a note is allowed to decay the lower the release hammer amplitude will be.
; Offset	-	Fundemental frequency of the release hammer (before keyboard tracking is applied)
; Keybd.Track	-	Amount of keyboard tracking to be applied to the frequency of release hammer. Increasing this value causes higher note to use a higher frequency hammer impulse.

; STEREO (a stereophonic widening effect using a delay of random duration on each channel)
; Width		-	Width of the effect. Effectively the maximum duration of the two random delay times.
; Mix		-	Dry/wet mix between the mono string output and the two delays.

; REVERB (a reverb effect using screverb)
; Mix		-	Dry/wet mix between the dry signal (including stereo signal) and the reverberated signal
; Size		-	Size or decay time of the reverb effect
; Damping	-	Cutoff frequency of the damping of reverberant reflections

<Cabbage>
form caption("Harmonics"), size( 710, 320), colour(100,150,150), pluginID("Harm"), colour( 0,0, 0)

;AMPLITUDE
image    bounds(  5,  5,130,110), colour(100,150,150), outlinecolour("white"), outlinethickness(2), shape("sharp"), plant("amplitude"){
label    bounds(  0,  5,130, 12), text("A M P L I T U D E"), fontcolour(white)
rslider  bounds(  0, 25, 75, 75), text("Amp."),  channel("Amp"), range(0,1.00,0.3,1,0.01),  textBox(1), colour("silver"), trackercolour("silver"), textcolour("white"), fontcolour("white")
rslider  bounds( 55, 25, 75, 75), text("Vel."),  channel("AmpVel"), range(0,1.00,0.5,1,0.01),  textBox(1), colour("silver"), trackercolour("silver"), textcolour("white"), fontcolour("white")
}

;FUNDEMENTAL
image    bounds(140,  5,130,110), colour(100,150,150), outlinecolour("white"), outlinethickness(2), shape("sharp"), plant("fundemental"){
label    bounds(  0,  5,130, 12), text("F U N D E M E N T A L"), fontcolour(white)
rslider  bounds(  0, 25, 75, 75), text("Offset"),  channel("NumOffset"), range(0,127,30,1,0.01),  textBox(1), colour("silver"), trackercolour("silver"), textcolour("white"), fontcolour("white")
rslider  bounds( 55, 25, 75, 75), text("Rnge."),  channel("NumRange"), range(0,127,15,1,0.01),  textBox(1), colour("silver"), trackercolour("silver"), textcolour("white"), fontcolour("white")
}

;HARMONIC
image    bounds(275,  5,185,110), colour(100,150,150), outlinecolour("white"), outlinethickness(2), shape("sharp"), plant("harmonic"){
label    bounds(  0,  5,185, 12), text("H A R M O N I C"), fontcolour(white)
label    bounds( 25, 17, 85, 11), text("Double Strength"), fontcolour(white)
checkbox bounds( 15, 17,  9, 10), channel("DoubleStrength"), fontcolour("white"), text("Double Strength")
rslider  bounds(  0, 25, 75, 75), text("Offset"),  channel("HarmOffset"), range(-1,1,0,1,0.001),  textBox(1), colour("silver"), trackercolour("silver"), textcolour("white"), fontcolour("white")
rslider  bounds( 55, 25, 75, 75), text("Vel."),  channel("HarmRange"), range(0,2.00,0.645,0.5,0.01),  textBox(1), colour("silver"), trackercolour("silver"), textcolour("white"), fontcolour("white")
rslider  bounds(110, 25, 75, 75), text("Kybd.Track"),  channel("HarmKybd"), range(0,1.000,0),  textBox(1), colour("silver"), trackercolour("silver"), textcolour("white"), fontcolour("white")
}

;STRING
image    bounds(465,  5,240,110), colour(100,150,150), outlinecolour("white"), outlinethickness(2), shape("sharp"), plant("main"){
label    bounds(  0,  5,240, 12), text("S T R I N G S"), fontcolour(white)
rslider  bounds(  0, 25, 75, 75), text("Detune"),  channel("detune"), range(0,50.000,1.5,0.35,0.001),  textBox(1), colour("silver"), trackercolour("silver"), textcolour("white"), fontcolour("white")
rslider  bounds( 55, 25, 75, 75), text("Release"),  channel("release"), range(0.01,12.000,0.15,0.5,0.001),  textBox(1), colour("silver"), trackercolour("silver"), textcolour("white"), fontcolour("white")
rslider  bounds(110, 25, 75, 75), text("Sustain"),  channel("feedback"), range(0,1.000,1,8),  textBox(1), colour("silver"), trackercolour("silver"), textcolour("white"), fontcolour("white")
rslider  bounds(165, 25, 75, 75), text("Damping"),  channel("cutoff"), range(0.001,1.000,1,0.5),  textBox(1), colour("silver"), trackercolour("silver"), textcolour("white"), fontcolour("white")
}

;ATTACK HAMMER
image    bounds(  5,120,185,110), colour(100,150,150), outlinecolour("white"), outlinethickness(2), shape("sharp"), plant("hammer"){
label    bounds(  0,  5,185, 12), text("A T T A C K    H A M M E R"), fontcolour(white)
rslider  bounds(  0, 25, 75, 75), text("Vel."),  channel("ToneVel"), range(0,1.00,0.75),  textBox(1), colour("silver"), trackercolour("silver"), textcolour("white"), fontcolour("white")
rslider  bounds( 55, 25, 75, 75), text("Offset"),  channel("HammFrq"), range(1,4000,100,0.5,1),  textBox(1), colour("silver"), trackercolour("silver"), textcolour("white"), fontcolour("white")
rslider  bounds(110, 25, 75, 75), text("Kybd.Track"),  channel("HammTrk"), range(0,1.00,0.33),  textBox(1), colour("silver"), trackercolour("silver"), textcolour("white"), fontcolour("white")
}

;RELEASE HAMMER
image    bounds(195,120,185,110), colour(100,150,150), outlinecolour("white"), outlinethickness(2), shape("sharp"), plant("hammer_rel"){
label    bounds(  0,  5,185, 12), text("R E L E A S E    H A M M E R"), fontcolour(white)
rslider  bounds(  0, 25, 75, 75), text("Ampl."),  channel("RelHammAmp"), range(0,1.00,0.3),  textBox(1), colour("silver"), trackercolour("silver"), textcolour("white"), fontcolour("white")
rslider  bounds( 55, 25, 75, 75), text("Offset"),  channel("RelHammFrq"), range(1,4000,100,0.5,1),  textBox(1), colour("silver"), trackercolour("silver"), textcolour("white"), fontcolour("white")
rslider  bounds(110, 25, 75, 75), text("Kybd.Track"),  channel("RelHammTrk"), range(0,1.00,0.4),  textBox(1), colour("silver"), trackercolour("silver"), textcolour("white"), fontcolour("white")
}

;STEREO
image    bounds(385,120,130,110), colour(100,150,150), outlinecolour("white"), outlinethickness(2), shape("sharp"), plant("stereo"){
label    bounds(  0,  5,130, 12), text("S T E R E O"), fontcolour(white)
rslider  bounds(  0, 25, 75, 75), text("Width"),  channel("StWidth"), range(0.0001,0.1,0.01,0.5),  textBox(1), colour("silver"), trackercolour("silver"), textcolour("white"), fontcolour("white")
rslider  bounds( 55, 25, 75, 75), text("Dry/Wet"),  channel("StMix"), range(0,1,0.5),  textBox(1), colour("silver"), trackercolour("silver"), textcolour("white"), fontcolour("white")
}

;REVERB
image    bounds(520,120,185,110), colour(100,150,150), outlinecolour("white"), outlinethickness(2), shape("sharp"), plant("reverb"){
label    bounds(  0,  5,185, 12), text("R E V E R B"), fontcolour(white)
rslider  bounds(  0, 25, 75, 75), text("Dry/Wet"),  channel("RvbDryWet"), range(0,1.000,0.3),  textBox(1), colour("silver"), trackercolour("silver"), textcolour("white"), fontcolour("white")
rslider  bounds( 55, 25, 75, 75), text("Size"),  channel("RvbSize"), range(0.4,0.999,0.55),  textBox(1), colour("silver"), trackercolour("silver"), textcolour("white"), fontcolour("white")
rslider  bounds(110, 25, 75, 75), text("Damping"),  channel("RvbLPF"), range(50,20000,3000,0.5,1),  textBox(1), colour("silver"), trackercolour("silver"), textcolour("white"), fontcolour("white")
}

keyboard bounds(  5,235,700, 80)

</Cabbage>
                    
<CsoundSynthesizer>

<CsOptions>   
-dm0 -n -+rtmidi=null -M0
</CsOptions>

<CsInstruments>

sr 		= 	44100	; SAMPLE RATE
ksmps 		= 	32	; NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls 		= 	2	; NUMBER OF CHANNELS (1=MONO)
0dbfs		=	1	; MAXIMUM AMPLITUDE

massign	0,2
giImp	ftgen	0,0,4097,9,0.5,1,0			; shape for the hammer inpulse
gidetuning	ftgen	0,0,128,21,1,1			; random array used for fixing unique detune values for each note
giDryMap	ftgen	0,0,4096,7,1,2048,1,2048,0	; dry mixer control mapping
giWetMap	ftgen	0,0,4096,7,0,2048,1,2048,1	; wet mixer control mapping
gaSendM,gaSendL,gaSendR	init	0			; initialise variable used for sending audio between instruments


opcode	Oscil1a,a,iii					; an oscillator that plays a single cycle of an audio waveform at a-rate
 iamp,ifrq,ifn	xin
 aptr	line	0,1/ifrq,1
 asig	tablei	aptr,ifn,1
 aenv	linseg	1,1/ifrq,1,0.001,0
	xout	asig*iamp*aenv
endop


instr	1						; read in widgets
 gkdetune	chnget	"detune"
 gkrelease	chnget	"release"
 gkHarmRange	chnget	"HarmRange"
 gkHarmOffset	chnget	"HarmOffset"
 gkHarmKybd	chnget	"HarmKybd"
 gkNumRange	chnget	"NumRange"
 gkNumOffset	chnget	"NumOffset"
 gkHammFrq	chnget	"HammFrq"
 gkHammTrk	chnget	"HammTrk"
 gkRelHammAmp	chnget	"RelHammAmp"
 gkRelHammFrq	chnget	"RelHammFrq"
 gkRelHammTrk	chnget	"RelHammTrk"
 gkAmpVel	chnget	"AmpVel"
 gkAmp		chnget	"Amp"
 gkToneVel	chnget	"ToneVel"
 gkDryWet	chnget	"RvbDryWet"
 gkDry		table	gkDryWet,giDryMap,1		; map dry/wet control
 gkWet		table	gkDryWet,giWetMap,1		;
 gkRvbSize	chnget	"RvbSize"
 gkRvbLPF	chnget	"RvbLPF"
 gkStWidth	chnget	"StWidth"
 gkStWidth	init	0.01				; (can't be zero at i-time)
 gkStMix	chnget	"StMix"
 gkfeedback	chnget	"feedback"
 gkcutoff	chnget	"cutoff"
 gkDoubleStrength	chnget	"DoubleStrength"
endin

instr	2						; hammered string instrument
 ;==Fundemental==
 inum	notnum
 iNumOffset	=	i(gkNumOffset)
 iNumRange	=	i(gkNumRange)
 icps		=	cpsmidinn(((inum/127) * iNumRange) + iNumOffset)	; derive fundemental frequency
 acps		interp	icps
 
 ;==Impulse==
 ;;(click impulse)
 ;aImpls	mpulse	1,0

 ;;(noise impulse)
 ;aNoise	pinkish	1
 ;iatt	=	0.001
 ;isus	=	0.05
 ;idec	=	0.01
 ;isuslev	veloc	0.5,1
 ;aEnv	linseg	0,iatt,isuslev,isus,isuslev,idec,0
 ;aImpls	=	aNoise * aEnv
 ;aImpls	butlp	aImpls,1000
 
 ;(hammer impulse)
 krel	release
 krel	init	0
 krms	init	0
 ktrig	trigger	krel,0.5,0
 if ktrig==1 then
  reinit RELEASE_HAMMER
 endif

 RELEASE_HAMMER:
 if i(krel)==1 then							; Insert release hammer values
  iAmpVel	=	i(gkRelHammAmp) * (( i(krms) * 3) + 0.03)
  ifrq	=	i(gkRelHammFrq) * semitone(i(gkRelHammTrk)*inum)
 else
  ifrq	=	i(gkHammFrq) * semitone(i(gkHammTrk)*inum)
  iAmpVel	veloc	1-i(gkAmpVel),1
 endif

 aImpls	Oscil1a	iAmpVel,ifrq,giImp
 
 rireturn
 icf	veloc	12-(8*i(gkToneVel)),12
 aImpls	butlp	aImpls,cpsoct(icf)



 ;==Harmonic==
 iHarmVel	veloc	i(gkHarmRange),0
 iHarmKybd	=	(i(gkHarmKybd) * (128-inum))/128
 iHarmRatio	=	1 + i(gkHarmOffset) + iHarmVel + iHarmKybd

 ;==Detuning==
 idetune	table	inum,gidetuning		;=	i(gkdetune)
 idetune	=	idetune * i(gkdetune)
 
 ;==Double Waveguide Filter==
 kcutoff	=	(sr/2)*gkcutoff
 kfeedback	=	0.249999999*gkfeedback
 aWg2	wguide2	aImpls,acps*cent(idetune),  acps*iHarmRatio*cent(idetune) , kcutoff,kcutoff, kfeedback, kfeedback
 aWg2_2	wguide2	aImpls,acps*cent(-idetune), acps*iHarmRatio*cent(-idetune), kcutoff,kcutoff, kfeedback, kfeedback
 if gkDoubleStrength==1 then
  aWg2		wguide2	aWg2*  0.13,acps*cent(idetune),  acps*iHarmRatio*cent(idetune) , kcutoff,kcutoff, kfeedback, kfeedback
  aWg2_2	wguide2	aWg2_2*0.13,acps*cent(-idetune), acps*iHarmRatio*cent(-idetune), kcutoff,kcutoff, kfeedback, kfeedback
 endif
 aWg2	dcblock2	aWg2+aWg2_2

 krms	rms	aWg2
 
 ;==Release==
 irel	=		i(gkrelease)
 kCF	expsegr		sr/3,irel,20
 aEnv	expsegr		1,irel,0.001
 aWg2	tone		aWg2, kCF
 aWg2	=		aWg2 * aEnv

 gaSendM	=	gaSendM + aWg2
endin

instr	98	; spatialising short delays
 ktrig	changed	gkStWidth
 if ktrig==1 then
  reinit	UPDATE
 endif
 UPDATE:
 iDelTimL	random	0.00001,i(gkStWidth)
 aDelSigL	delay	gaSendM, iDelTimL
 iDelTimR	random	0.00001,i(gkStWidth)
 aDelSigR	delay	gaSendM, iDelTimR
 rireturn
 kDry		table	gkStMix,giDryMap,1
 kWet		table	gkStMix,giWetMap,1
 aL		=	((gaSendM*kDry)+(aDelSigL*kWet)) * gkDry
 aR		=	((gaSendM*kDry)+(aDelSigR*kWet)) * gkDry
 gaSendL	=	gaSendL + gaSendM + aDelSigL
 gaSendR	=	gaSendR + gaSendM + aDelSigR
 		outs	aL*gkAmp, aR*gkAmp
 		clear	gaSendM
endin

instr	99
 aL,aR	reverbsc	gaSendL,gaSendR,gkRvbSize,gkRvbLPF
 	outs		aL*gkWet*gkAmp,aR*gkWet*gkAmp
 	clear		gaSendL,gaSendR
endin

</CsInstruments>

<CsScore>
i  1 0 [3600*24*7]
i 98 0 [3600*24*7]
i 99 0 [3600*24*7]
e
</CsScore>

</CsoundSynthesizer>
