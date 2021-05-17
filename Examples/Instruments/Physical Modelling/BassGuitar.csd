; BassGuitar.csd
; Written by Iain McCurdy, 2011

; Physical model of a plucked string with a pickup point. 
; Model is created from first principles in order to implement some improvements over the existing Csound opcodes for plucked strings.
; A bandpass filter is employed within the delay buffer used to implement the pluck which facilitates filtering to specific harmonics. 
; Conventional damping effects are still possible when cutoff frequency ratio = 1.

<Cabbage>
form caption("Bass Guitar"), size(550, 200), pluginId("basg") style("legacy")

#define RSliderStyle # colour(150,205,150), textColour("LightGreen"), outlineColour(0,0,0), trackerColour(yellow)#

image    bounds(  0,  0,550,200),  colour("DarkGreen"), shape("sharp"), outlineColour("white"), outlineThickness(3)
rslider  bounds( 10, 10, 60, 60),  text("Sustain"),      channel("feedback"), range(0.9,1, 0.999, 2, 0.001), $RSliderStyle
rslider  bounds( 70, 10, 60, 60),  text("Filt. Ratio"), channel("FiltRatio"), range(0.5, 32, 1, 0.5), $RSliderStyle
rslider  bounds(130, 10, 60, 60),  text("B.width"),     channel("bw"), range(1, 32, 16), $RSliderStyle
rslider  bounds(190, 10, 60, 60),  text("Att"),         channel("att"), range(0, 3, 1,0.5), $RSliderStyle
checkbox bounds(260, 20, 30, 30),  text("Legato")       channel("legato"),FontColour("LightGreen"), colour("yellow")  value(1)
label    bounds(252, 54, 43, 14),  text("Legato"), FontColour("LightGreen")
rslider  bounds(300, 10, 60, 60),  text("Leg.Speed"),        channel("LegSpeed"),  range(0.01,1,0.05,0.5), $RSliderStyle
rslider  bounds(360, 10, 60, 60),  text("Vib.Depth"),        channel("VibDep"),    range(0, 1, 0.25, 0.75, 0.001), $RSliderStyle
rslider  bounds(420, 10, 60, 60),  text("Vib.Rate"),         channel("VibRte"),    range(0.5, 20, 3, 0.5), $RSliderStyle
rslider  bounds(480, 10, 60, 60),  text("Level"),            channel("level"),     range(0, 1, 0.7), $RSliderStyle
hslider  bounds( 15, 70,525, 40),  text("Pickup Position"), channel("PickupPos"), range(0.01, 0.99, 0.1), $RSliderStyle
keyboard bounds(  5,115,540, 80)
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-dm0 -n -+rtmidi=null -M0
</CsOptions>

<CsInstruments>

;sr is set by the host
ksmps 		= 	64
nchnls 		= 	2
0dbfs		=	1	;MAXIMUM AMPLITUDE
massign	0,2

gkNoteTrig	init	0
giwave	ftgen	0,0,4097,11,20,1,0.5	;waveform used by excitation (pluck) signal
gisine	ftgen	0,0,4097,10,1		;sine wave (used by lfos)

gkactive	init	0	; Will contain number of active instances of instr 3 when legato mode is chosen. NB. notes in release stage will not be regarded as active. 

;UDOs
;UDO for plucked electric string - using a UDO facilitates the use of ksmps=1 to improve sound quality
opcode	PluckedElectricString,a,aakkkak
	asig,acps,kcutoff,kbw,kfeedback,aPickupPos,kporttime	xin
	setksmps	1
	;smooth krate variables according to the local ksmps and kr
	
	kcutoff	portk		kcutoff,kporttime
	kbw	portk		kbw,kporttime
	
	acutoff	interp		kcutoff
	kcutoff	downsamp	acutoff
	abw	interp		kbw
	kbw	downsamp	abw
	kbw		limit	kcutoff*kbw,0.001,10000	;limit bandwidth values to prevent explosion
	aDelTim	limit	1/acps,0,1	;derive required delay time from cycles per second value (reciprocal) and limit range
		
	afb	init	0					;audio feedback signal used by delay buffer
	atap1	vdelay	asig+afb,aDelTim*aPickupPos*1000,1000	;tap 1. Nut

	atap2	vdelay	-atap1,aDelTim*(1-aPickupPos)*1000,1000	;tap 2, Tailpiece
	
	atap2	butbp	atap2,kcutoff,kbw			;bandpass filter (nb. within delay buffer)

	afb	=	atap2*-kfeedback			;create feedback signal to add to input for next iteration

		xout	atap1+atap2				;return audio to caller instrument. NB. audio at pickup is a mixture (with positive and negative interference) of wave reflected from the bridge and the nut (the two points of fixture of the string) 
endop

;UDO for lowpass filter attack enveloping - using a UDO permits setting ksmps=1 in order to improve sound quality
opcode	butlpsr,a,aii
	setksmps	1
	asig,icps,idur	xin
	kcfenv		expseg	icps,idur,15000,1,15000
	asig	butlp	asig,kcfenv
		xout	asig
endop

instr	1	;read in widgets - this instrument runs constantly during performance
	gkfeedback	chnget	"feedback"
	gkFiltRatio	chnget	"FiltRatio"
	gkbw		chnget	"bw"
	gkatt		chnget	"att"
	gklegato	chnget	"legato"
	gkLegSpeed	chnget	"LegSpeed"
	gkVibDep	chnget	"VibDep"
	gkVibRte	chnget	"VibRte"
	gklevel		chnget	"level"
	gkPickupPos	chnget	"PickupPos"
endin

instr	2	;triggered via MIDI
	gkNoteTrig	init	1	;at the beginning of a new note set note trigger flag to '1'
	icps		cpsmidi		;read in midi note pitch in cycles per second
	givel		veloc	0,1	;read in midi note velocity

	gkcps	=	icps		;update a global krate variable for note pitch

	if i(gklegato)==0 then		;if we are *not* in legato mode...
	 inum	notnum						; read midi note number (0 - 127)
	 	event_i	"i",p1+1+(inum*0.001),0,-1,icps		; call sound producing instr
	 krel	release						; release flag (1 when note is released, 0 otherwise)
	 if krel==1 then					; when note is released...
	  turnoff2	p1+1+(inum*0.001),4,1			; turn off the called instrument
	 endif							; end of conditional
	else				;otherwise... (i.e. legato mode)
	 iactive	=	i(gkactive)			;number of active notes of instr 3 (note in release are disregarded)
	 if iactive==0 then					;...if no notes are active
	  event_i	"i",p1+1,0,-1				;...start a new held note
	 endif
	endif
endin

instr	3
	kporttime	linseg	0,0.001,1		;portamento time function rises quickly from zero to a held value
	kporttime	=	kporttime*gkLegSpeed	;scale portamento time function with value from GUI knob widget
	
	if i(gklegato)==1 then				;if we are in legato mode...
	 krel	release					;sense when  note has been released
	 gkactive	=	1-krel			;if note is in release, gkactive=0, otherwise =1
	 kcps	portk	gkcps,kporttime			;apply portamento smooth to changes in note pitch (this will only have an effect in 'legato' mode)
	 acps	interp	kcps				;create a a-rate version of pitch (cycles per second)
	 kactive	active	p1-1			;...check number of active midi notes (previous instrument)
	 if kactive==0 then				;if no midi notes are active...
	  turnoff					;... turn this instrument off
	 endif
	else						;otherwise... (polyphonic / non-legato mode)
	 acps	=	p4		 		;pitch equal to the original note pitch
	endif
	
	aptr	line	0,1/i(gkcps),1			;pointer that will be used to read excitation signal waveform function table
	asig	tablei	aptr,giwave,1,0,0		;create excitation (pluck) signal
	asig	butlp	asig,cpsoct(4+(givel*8))	;lowpass filter excitation signal according to midi note velocity of this note 
	asig	buthp	asig,i(gkcps)			;highpass filter excitation signal (this could possibly be made adjustable using a GUI widget)
	
	kcutoff		limit	gkcps*gkFiltRatio,20,20000	;cutoff of frequency of the bandpass filter will be relative to the pitch of the note. Limit it to prevent out of range values that would cause filter expolosion.
	
	;In legato mode modulations are reinitialised
	if gkNoteTrig==1&&gklegato==1 then
	 reinit	RESTART_ENVELOPE
	endif
	RESTART_ENVELOPE:
	krise	linseg	0,0.3,0,1.5,1			;build-up envelope - modulations do not begin immediately
	rireturn
	arise	interp	krise				;interpolation prevents discontinuities (clicks) when oscili lfos are reinitialised
	avib	oscili	0.8*arise*gkVibDep,gkVibRte,gisine	;vibrato
	acps	=	acps*semitone(avib)
	atrm	oscili	0.8*arise*gkVibDep,gkVibRte,gisine,0	;tremolo

	gkPickupPos	portk	gkPickupPos,kporttime		;smooth changes made to pickup position slider
	aPickupPos	interp	gkPickupPos			;interpolate k-rate pickup position variable to create an a-rate version
	ares 		PluckedElectricString   asig, acps, kcutoff, gkbw, gkfeedback, aPickupPos,kporttime	;call UDO - using a UDO facilitates the use of a different ksmps value (ksmps=1 and kr=sr) to optimise sound quality
	
	aenv		linsegr	0.7,0.05,0			;amplitude envelope
	
	if i(gkatt)>0 then					;if attack time is anything greater than zero call the lowpass filter envelope
	 ares		butlpsr	ares,i(gkcps),i(gkatt)		;a UDO is used again to allow the use of ksmps=1
	endif
	
	ares		=	ares*aenv*(1+atrm)*gklevel	;scale amplitude of audio signal with envelope, tremolo and level knob widget
			outs	ares,ares
	gkNoteTrig	=	0				;reset new-note trigger (in case it was '1')
endin

</CsInstruments>

<CsScore>
i 1 0 [60*60*24*7]	;instrument that reads in widget data
</CsScore>

</CsoundSynthesizer>