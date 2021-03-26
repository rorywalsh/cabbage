; En Garde! (Drawing a sword: precedural audio)
; Iain McCurdy, 2017

; Drawing Sword (groupbox)
; ------------------------
; Density		-	roughness of the texture of the sword
; Draw duration	-	time taken to draw the sword from the scabbard
; Freq.1 [start]/Freq.1 [end]
; Freq.2 [start]/Freq.2 [end]
;  				-	start and end frequencies for the plate modelling for the drawing of the sword sound
; Feedback		-	basically the inverse of the damping of the sword while it is begin drawn
; Draw Gain		-	amplitude gain of the sword being drawn part of the sound

; Drawing Sword (groupbox)
; ------------------------
; Freq. Base		-	lowest partial frequency
; Freq. Range		-	possible extent of frequencies (frequencies are generated randomly)
; Resonance			-	ring time for when the sword is free of the scabbard
; Seed				-	defines the particular set of random values that will be generated. A seed value of zero will produce a different sword sound each time it is drawn.
; N.Partials		-	number of partials of the resonance of the sword
; Ring Offset		-	amount of random deviation from resonance value defined by 'Resonance' for each partial
; Ring Gain			-	amplitude gain for the ringing resonance part of the sound

<Cabbage>
form caption("En garde!") size(600, 310), colour(58, 110, 182), pluginId("EnGa") style("legacy")
button bounds(5,5,590,30) channel("PlaySound") text("Draw Sword") latched(0)
groupbox bounds(5,40,590,130) outlineThickness(1) outlineColour(200,200,200) colour(0,0,0,100) plant("Drawing") text("Drawing Sword")
{
rslider bounds(5,25,70,90) channel("TexturalDensity") text("Density") valueTextBox(1) range(100,10000,1000,0.5,1)
rslider bounds(75,25,70,90) channel("DrawDuration") text("Duration") valueTextBox(1) range(0.01,2,0.7,0.5,0.001)
image   bounds(148,30,292,90) outlineThickness(1) outlineColour(200,200,200) colour(0,0,0,50) shape("rounded")
hslider bounds(150,35,300,20) channel("Freq1a") text("Freq. 1 [Start]") valueTextBox(1) range(100,3000,300,0.5,1)
hslider bounds(150,55,300,20) channel("Freq1b") text("Freq. 1 [End]") valueTextBox(1) range(100,3000,811,0.5,1)
hslider bounds(150,75,300,20) channel("Freq2a") text("Freq. 2 [Start]") valueTextBox(1) range(100,3000,717,0.5,1)
hslider bounds(150,95,300,20) channel("Freq2b") text("Freq. 2 [End]") valueTextBox(1) range(100,3000,933,0.5,1)
rslider bounds(440,25,70,90) channel("Feedback") text("Feedback") valueTextBox(1) range(0.0,0.249,0.17.0)
rslider bounds(510,25,70,90) channel("DrawGain") text("Draw Gain") valueTextBox(1) range(0,2,1,0.5,0.001)
}
groupbox bounds(5,175,590,130) outlineThickness(1) outlineColour(200,200,200) colour(0,0,0,100) plant("Ringing") text("Ringing")
{
rslider bounds(15,25,70,90) channel("Seed") text("Seed") valueTextBox(1) range(0,1000,879,1,1)
rslider bounds(95,25,70,90) channel("RingFreqBase") text("Freq. Base") valueTextBox(1) range(500,6000,1400,0.5,1)
rslider bounds(175,25,70,90) channel("RingFreqRange") text("Freq. Range") valueTextBox(1) range(500,12000,5880,0.5,1)
rslider bounds(255,25,70,90) channel("Resonance") text("Resonance") valueTextBox(1) range(1,8000,800,0.5,1)
rslider bounds(335,25,70,90) channel("ResOffset") text("Res. Offset") valueTextBox(1) range(0,4,2,0.5,0.01)
rslider bounds(415,25,70,90) channel("NPartials") text("N.Partials") valueTextBox(1) range(1,50,20,1,1)
rslider bounds(495,25,70,90) channel("RingGain") text("Ring Gain") valueTextBox(1) range(0,2,1,0.5,0.001)
}

</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -dm0
</CsOptions>
<CsInstruments>
; Initialize the global variables. 
;sr is set by the host
ksmps = 32
nchnls = 2
0dbfs = 1

instr		1	; read button an trigger a note event accordingly
 gkPlaySound	chnget	"PlaySound"
 if trigger:k(gkPlaySound,0.5,0)==1 then
  event	"i",2,0,-1
 endif
endin

; UDO for a single resonance partial
opcode PARTIAL,a,aaiiiip
 aInSig,aQ,iResFreqBase,iResFreqRange,iResOffset,iNPartials,iCount	xin
 aPartial			mode		aInSig, limit:i (exprand(iResFreqRange) + iResFreqBase, 20, sr/$M_PI), aQ * (2 ^ random(-iResOffset,iResOffset))
 aMix	=	0
 if iCount<=iNPartials then
 	aMix	PARTIAL	aInSig,aQ,iResFreqBase,iResFreqRange,iResOffset,iNPartials,iCount+1
 endif
 	xout	aPartial + aMix
endop


instr		2	; 'drawing a sword' sound effect
 
 ; drawing of the sword part of the sound
 iDrawDur		chnget		"DrawDuration"
 iRoughness		chnget		"TexturalDensity"
 kEnv			expseg		0.1, 0.3 * (iDrawDur/0.7), 1, iDrawDur-0.4, 1, 0.1 * (iDrawDur/0.7), 0.1,1,0.1
 aDraw			dust2		0.1*(kEnv-0.1),	 iRoughness
 aDraw			reson		aDraw, 8000,3000,1
 iFB			chnget		"Feedback"
 aFreq1			expseg		chnget:i("Freq1a"),(0.3*(iDrawDur/0.7)) + (iDrawDur-0.4),chnget:i("Freq1b"),1,chnget:i("Freq1b")
 aFreq2			expseg		chnget:i("Freq2a"),(0.3*(iDrawDur/0.7)) + (iDrawDur-0.4),chnget:i("Freq2b"),1,chnget:i("Freq2b")
 aDraw			wguide2		aDraw,aFreq1,aFreq2,10000,10000,iFB,iFB
 aDraw			wguide2		aDraw,aFreq1,aFreq2,10000,10000,iFB,iFB
 aDraw			dcblock2	aDraw
 ;				outs		aDraw * chnget:i("DrawGain"), aDraw * chnget:i("DrawGain")
 
 ; ringing
				seed		chnget:i("Seed")
 aQ				linseg		1, (0.3*(iDrawDur/0.7)) + (iDrawDur-0.4), 1, 0.05, chnget:i("Resonance")
 iResFreqBase	=			chnget:i("RingFreqBase")
 iResFreqRange	=			chnget:i("RingFreqRange") / 5
 iNPartials		=			10
 aRing	 		PARTIAL		aDraw, aQ, iResFreqBase, iResFreqRange, chnget:i("ResOffset"), chnget:i("NPartials")
 aRing			*=			chnget:i("RingGain")
				outs		aRing, aRing
				
 ; wait until resonance has died away before turning off instrument
 if rms:k(aRing)<0dbfs*0.00001 && timeinsts() > 1 then
  turnoff
 endif
endin

</CsInstruments>
<CsScore>
i 1 0 z
</CsScore>
</CsoundSynthesizer>
