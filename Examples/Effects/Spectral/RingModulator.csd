; RingModulator.csd
; Written by Iain McCurdy, 2012.

; Ring Modulator
; --------------
; Modulating waveform is a harmonic waveform based on a sawtooth waveform.
;  The number of harmonics included is defined using 'Harms'.
;  An offset of harmonics to omit is defined using 'Offset'.
;  Therefore Harms=1 and Offset=0 will produce a sine wave.

; Input		-	input mode 

; Freq.		-	frequency of the ring modulator (if input mode is 'Knob')
;			 If input mode is 'Kybd.' then base frequency is derived from the note played and the 'Freq.' knob is disregarded.
; Harms.	-	number of harmonics included in the modulating waveform
; Offset	-	number of harmonics to skip before building the harmonic waveform used by the modulator

; Env.		-	amount of amplitude following used to influence the modulator frequency
; Att.		-	attack time used in amplitude following
; Dec.		-	decay time used in amplitude following

; Thresh	-	threshold which, if the rms (amplitude measurement) crosses through, will trigger the generation of a new random modulator frequency.
;			 If thresh is zero, the random triggering of new modulating frequencies is bypassed
; Min.		-	minimum of the range within which a new modulating frequency will be generated (oct format)
; Max.		-	maximum of the range within which a new modulating frequency will be generated (oct format)

; Width		-	create stereo width by offsetting the phase of the left channel modulator with respect to that of the right
; Mix		-	dry/wet mix
; Level		-	output level

<Cabbage>
form caption("Ring Modulator") size(755,170), pluginID("rmod")
image               pos(0, 0), size(755, 90), colour("black"), shape("rounded"), outlinecolour("lime"), outlinethickness(5) 
label    bounds( 10, 20, 65, 13), text("Input"), textcolour(white)
combobox bounds( 10, 34, 65, 18), text("Knob","Keybd."), channel("input"), textcolour(white)
rslider bounds( 75, 12, 70, 70), text("Freq."),    channel("freq"),  range(0, 15000, 800, 0.25),   colour("yellow"),    trackercolour(white), textcolour(white)
rslider bounds(145, 12, 70, 70), text("Harms"),    channel("harms"), range(1, 40, 1,1,1),          colour("yellow"),    trackercolour(white), textcolour(white)
rslider bounds(215, 12, 70, 70), text("Offset"),   channel("offset"),range(0, 20, 0,1,1),          colour("yellow"),    trackercolour(white), textcolour(white)

line    bounds(290,  2, 3, 86), colour("lime")
rslider bounds(300, 12, 70, 70), text("Env."),     channel("env"),   range(0, 1.00, 0.5, 0.5,0.001), colour("orange"),    trackercolour(white), textcolour(white)
rslider bounds(370,  6, 40, 40), text("Att."),     channel("att"),   range(0.001, 2, 0.01, 0.5),   colour("orange"),    trackercolour(white), textcolour(white)
rslider bounds(370, 46, 40, 40), text("Dec."),     channel("dec"),   range(0.001, 2, 0.1,  0.5),   colour("orange"),    trackercolour(white), textcolour(white)
line    bounds(415,  2, 3, 86), colour("lime")

rslider bounds(425, 12, 70, 70), text("Thresh"),   channel("thresh"),   range(0, 1.00, 0, 0.5,0), colour("orange"),    trackercolour(white), textcolour(white)
rslider bounds(490,  6, 40, 40), text("Min."),     channel("min"),   range(0, 13, 5),   colour("orange"),    trackercolour(white), textcolour(white)
rslider bounds(490, 46, 40, 40), text("Max."),     channel("max"),   range(0, 13, 8),   colour("orange"),    trackercolour(white), textcolour(white)
line    bounds(535,  2, 3, 86), colour("lime")


rslider bounds(540, 12, 70, 70), text("Width"),    channel("width"), range(0, 1.0, 0.3,1,0.001),   colour("chocolate"), trackercolour(white), textcolour(white)
rslider bounds(610, 12, 70, 70), text("Mix"),      channel("mix"),   range(0, 1.00, 1),            colour("chocolate"), trackercolour(white), textcolour(white)
rslider bounds(680, 12, 70, 70), text("Level"),    channel("level"), range(0, 1.00, 1),            colour("chocolate"), trackercolour(white), textcolour(white)

keyboard bounds(0,92,755,78)
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
;-d -n
-dm0 -n -+rtmidi=null -M0
</CsOptions>
<CsInstruments>
sr = 44100
ksmps = 32
nchnls = 2
0dbfs = 1
massign	0,2

;Author: Iain McCurdy (2012)
;http://iainmccurdy.org/csound.html

gisaw	ftgen	0,0,4096,7,1,4096,-1	; source modulating waveform (unbandlimited sawtooth waveform) before harmonic filtering

opcode	RingModulator,a,akkkkkii
	ain,kmix,kfreq,kenv,katt,kdec,ifn,iphs	xin			;READ IN INPUT ARGUMENTS
	iWet	ftgentmp	0,0,1024,-7,0,512,1,512,1		;RESCALING FUNCTION FOR WET LEVEL CONTROL
	iDry	ftgentmp	0,0,1024,-7,1,512,1,512,0		;RESCALING FUNCTION FOR DRY LEVEL CONTROL
	kWet	table	kmix, iWet, 1					;RESCALE WET LEVEL CONTROL ACCORDING TO FUNCTION TABLE iWet
	kDry	table	kmix, iDry, 1					;RESCALE DRY LEVEL CONTROL ACCORDING TO FUNCTION TABLE iDry
	aFollow	follow2		ain, katt, kdec				;AMPLITUDE FOLLOWING kModFrq + (cpsoct(kFollow*kenv*30))     	;CREATE A LEFT CHANNEL MODULATING FREQUENCY BASE ON THE STATIC VALUE CREATED BY kfreq AND THE AMOUNT OF DYNAMIC ENVELOPE FOLLOWING GOVERNED BY kenv
	kFollow	downsamp	aFollow
	kFollow	logcurve	kFollow/0dbfs,2
	kfreq	=	kfreq + (kFollow*kenv*8000)     		;CREATE A LEFT CHANNEL MODULATING FREQUENCY BASE ON THE STATIC VALUE CREATED BY kfreq AND THE AMOUNT OF DYNAMIC ENVELOPE FOLLOWING GOVERNED BY kenv
	aMod	poscil	1, kfreq, ifn, iphs  			;CREATE RING MODULATING SIGNAL
	aout	sum	ain*kDry, ain*aMod*kWet				;MIX DRY AND WET SIGNALS
		xout	aout						;SEND AUDIO BACK TO CALLER INSTRUMENT
endop

instr	1
 kporttime	linseg	0,0.001,0.1
 gkfreq	chnget	"freq"
 gkfreq	portk	gkfreq,kporttime
 gkmix 	chnget	"mix"
 gkwidth	chnget	"width"
 gkenv 	chnget	"env"
 gkatt 	chnget	"att"
 gkdec 	chnget	"dec"
 gkthresh 	chnget	"thresh"
 gkmin 	chnget	"min"
 gkmax 	chnget	"max"
 gklevel 	chnget	"level"
 gkharms		chnget	"harms"
 gkoffset	chnget	"offset"

 ga1,ga2	ins
 ;ga1,ga2	diskin2	"808loop.wav",1,0,1

 gkinput	chnget	"input"
 gkinput	init	1
 if changed(gkinput)==1&&gkinput==1 then
  event	"i",2,0,-1
 endif
endin

instr	2
 /* MIDI AND GUI INTEROPERABILITY */
 iMIDIflag	=	0			; IF MIDI ACTIVATED = 1, NON-MIDI = 0
 mididefault	1, iMIDIflag			; IF NOTE IS MIDI ACTIVATED REPLACE iMIDIflag WITH '1'
 
 if iMIDIflag==1 then				; IF THIS IS A MIDI ACTIVATED NOTE...
  icps	cpsmidi					; READ MIDI PITCH VALUES - THIS VALUE CAN BE MAPPED TO GRAIN DENSITY AND/OR PITCH DEPENDING ON THE SETTING OF THE MIDI MAPPING SWITCHES
  kfreq	=		icps
 else
  kfreq 	=	gkfreq
 endif						; END OF THIS CONDITIONAL BRANCH

 if gkinput!=1&&iMIDIflag!=1 then
  turnoff
 endif
 
 if gkthresh>0 then
  a1,a2	init	0
  krms	rms	a1+a2
  kRTrig	init	1
  kfreq	trandom	kRTrig,gkmin,gkmax
  kfreq	=	cpsoct(kfreq)
  if changed(kfreq)==1 then
   chnset	kfreq,"freq"
  endif
  kRTrig	trigger	krms,gkthresh,0
 endif
 
 if metro(8)==1 then 
  if changed(gkharms,gkoffset,gkwidth)==1 then
   reinit UPDATE
  endif
  UPDATE:
 endif
 ifn	ftgen	1, 0, 4096, 30, gisaw, 1+i(gkoffset), i(gkharms)+i(gkoffset)		; generate a waveform based on chosen source waveform with user-set modifications
 ;ifn	ftgen	1, 0, 4096, 10, 1
 a1	RingModulator	ga1,gkmix,kfreq,gkenv,gkatt,gkdec,ifn,i(gkwidth)
 a2	RingModulator	ga2,gkmix,kfreq,gkenv,gkatt,gkdec,ifn,0
 rireturn
 aEnv	linsegr	0,0.01,1,0.01,0
 a1	=	a1 * gklevel * aEnv
 a2	=	a2 * gklevel * aEnv
 	outs	a1,a2
endin

</CsInstruments>

<CsScore>
i 1 0 [60*60*24*7]
</CsScore>

</CsoundSynthesizer>