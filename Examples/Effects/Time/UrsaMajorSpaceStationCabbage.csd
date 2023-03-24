
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; URSA MAJOR SPACE STATION
; Iain McCurdy, 2021

; Clone of Ursa Major Space Station SST-282 multitap delay

<Cabbage>
form caption("Ursa Major Space Station"), size(680, 415), pluginId("UMSS"), colour("white")
#define RSliderStyle  trackerInsideRadius(.9), trackerOutsideRadius(1), markerColour(10,10, 55), markerThickness(2), trackerColour(130,130,255), trackerThickness(4), colour(160,165,180), fontColour(50,50,50), textColour(50,50,50), outlineColour(50,50,50), valueTextBox(1)
label   bounds(  0, 10, 680,20), text("U  R  S  A     M  A  J  O  R  -  S  P  A  C  E     S  T  A  T  I  O  N"), align("centre"), fontColour("black")
line    bounds(  5, 35, 670, 2)
rslider bounds( 10, 40, 90, 90), text("Low Cut"), channel("LowCut"), range(50, 1000, 200, 0.5, 1), $RSliderStyle
rslider bounds( 90, 40, 90, 90), text("High Cut"), channel("HighCut"), range(1000, 15000, 8000, 0.5, 1), $RSliderStyle
rslider bounds(170, 40, 90, 90), text("Feedback"), channel("FBRatio"), range(0, 1, 0.85), $RSliderStyle
rslider bounds(250, 40, 90, 90), text("Damping"), channel("Damping"), range(500, 15000, 10000, 0.5, 1), $RSliderStyle
rslider bounds(330, 40, 90, 90), text("Size"), channel("Size"), range(0.1, 1, 0.5), $RSliderStyle
rslider bounds(410, 40, 90, 90), text("Mix"), channel("DryWet"), range(0, 1, 0.7), $RSliderStyle
rslider bounds(490, 40, 90, 90), text("Decay"), channel("Decay"), range(0, 24, 24,1,1), $RSliderStyle
rslider bounds(570, 40, 90, 90), text("Modulation"), channel("Modulation"), range(0.1, 10, 1), $RSliderStyle
line    bounds(  5,140, 670, 2)
image   bounds(115,150,450,250), file("Schematic.png") 
</Cabbage>

<CsoundSynthesizer>
<CsOptions>
-dm0 -n -+rtmidi=NULL -M0
</CsOptions>
<CsInstruments>

ksmps = 32
nchnls = 2
0dbfs = 1

seed 0		; this line ensures that certain random number generators will be seeded from the system clock each time Csound is run, ensuring that the results will be different each time.

opcode	ListenerTaps, a, kkiip																						; define the form of the user defined opcode	
 kSize, kDecay, iMaxDelay, iNLisTaps, iCount	xin												; read the input variables
 iTime	=										(iCount/iNLisTaps) * iMaxDelay				; spread the taps evenly across the delay range
 iTime	+=									random:i(-iMaxDelay/iNLisTaps,iMaxDelay/iNLisTaps) * 0.5		; add some randomness to the delay time of each tap
 iTime	limit						iTime,1/kr,iMaxDelay													; limit the delay times in case the randomness caused any to exceed the buffer.
 aTap		deltapi					iTime*kSize																				; tap the delay buffer, scale according to the 'Size' control.
 aTap		*=									ampdbfs((iCount/iNLisTaps)* kDecay)			; scale the amplitude of each tap. Later taps will be quieter.
 if (iCount < iNLisTaps) then																									; conditionally call the UDO again
  aMix	ListenerTaps	kSize, kDecay, iMaxDelay, iNLisTaps, iCount+1	; call the UDO again and increment the counter (iCount)
 endif
 									xout				aTap + aMix																				; send audio back to caller instrument	, both the audio from the first layer mixed with all subsequent layers
endop

opcode	FeedbackTaps, a, kkiip																						; define the form of the user defined opcode	
 kSize, kModulation, iMaxDelay, iNFBTaps, iCount	xin													; read the input variables

; aTime	rspline					0.08, iMaxDelay-0.08, 0.4, 0.8
; aTime	limit						aTime, 1/kr, iMaxDelay 
 kRate	rspline					kModulation, kModulation*2, 0.2, 0.5														; rate of change of the random fucntion in the next line
 aTime	randomi					0, iMaxDelay, kRate, 1											; a random function (linear segments) defines the delay tap position (time)
 aTap		deltapi					aTime*kSize																				; tap the delay buffer, scale according to the 'Size' control.
 if (iCount < iNFBTaps) then																						; conditionally call the UDO again
  aMix	FeedbackTaps	kSize, kModulation, iMaxDelay, iNFBTaps, iCount+1	; call the UDO again and increment the counter (iCount)
 endif
 									xout				aTap + aMix																			; send audio back to caller instrument	
endop

instr		1
 
 ; retrigger a sample periodically
 /*
 if metro(0.3)==1 then
  reinit RESTART
 endif
 RESTART:
 aIn				diskin	"Snare.wav"
 rireturn
 */
 
 aInL, aInR  ins
 
 
 iNLisTaps		=		8				; number of listener taps (stationary)
 iNFBTaps			=		15			; number of feedback taps (modulated)
 
 iMaxDelay	=	 0.255
 kCF						chnget		"Damping"
 kFBRatio		chnget		"FBRatio"
 kFBRatio		*=						0.4
 kPortTime	linseg		0,0.01,0.05
 kSize				chnget		"Size"
 kSize				portk			kSize, kPortTime
 kDecay				chnget		"Decay"
 kModulation	chnget	"Modulation"
 
 aInLFilt			buthp	aInL, chnget:k("LowCut")
 aInLFilt			butlp	aInLFilt, chnget:k("HighCut")

 aInRFilt			buthp	aInR, chnget:k("LowCut")
 aInRFilt			butlp	aInRFilt, chnget:k("HighCut")
 
 
 aBuf	delayr	iMaxDelay
 aLisL	ListenerTaps	kSize, -kDecay, iMaxDelay, iNLisTaps
 aFBL		FeedbackTaps	kSize, kModulation, iMaxDelay, iNLisTaps
 aFBL		dcblock2					aFBL
 aFBL		clip								aFBL, 0, 0.9
 aFBL		tone								aFBL, kCF
 					delayw aInLFilt + ( (aFBL/(iNFBTaps^0.02)) * kFBRatio )

 aBuf	delayr	iMaxDelay
 aLisR	ListenerTaps	kSize, -kDecay, iMaxDelay, iNLisTaps
 aFBR		FeedbackTaps	kSize, kModulation, iMaxDelay, iNLisTaps
 aFBR		dcblock2					aFBR
 aFBR		clip								aFBR, 0, 0.9
 aFBR		tone								aFBR, kCF
 					delayw aInRFilt + ( (aFBR/(iNFBTaps^0.02)) * kFBRatio )
 	
 	kDryWet	chnget	"DryWet"
 	aMixL	ntrpol	aInL, (aLisL*0.3), kDryWet
 	aMixR	ntrpol	aInR, (aLisR*0.3), kDryWet
 	
 					outs				aMixL, aMixR
 					

endin

</CsInstruments>

<CsScore>
i 1 0 [3600*24*365]
</CsScore>

</CsoundSynthesizer>
