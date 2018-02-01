; Rhythmicon.csd
; Written by Iain McCurdy, 2013.

; Recreation of Henry Cowell and Leon Theremin's Rhythmicon instrument (http://en.wikipedia.org/wiki/Rhythmicon)

; CONTROLS
; --------
; BASE	--	note number of the fundemental (lowest note)
; RATE	--	base rate (same as rate for fundemental)
; LEVEL	--	output level

; DESCRIPTION
; -----------
; Each note above the fundemental (C3) divides the base rate increasingly according to the arithmetic progression: 2,3,4,5...
; Therefore if C3 plays crotchets (quarter notes), C#3 plays quavers (eighth notes), D3 triplets and so on.
; The pitches of each note (in cycles per second) follow a simlar progression.
; If C3 plays a 200Hz tone, C#3 plays 400Hz, D 600Hz and so on.

; The 17th note (E4) does not play a note but instead engages a syncopation effect.  
; I have been unable to find a clear technical specification of how this was implemented but have nonetheless made my 'best guess'.

<Cabbage>
form caption("...") size(159, 180), pluginid("basl"), scrollbars(0)

label   bounds(  5,  3, 18, 22), text("R"), fontcolour(red)
label   bounds( 20,  5, 18, 22), text("H"), fontcolour(yellow)
label   bounds( 35,  2, 18, 22), text("Y"), fontcolour(orchid)
label   bounds( 50,  4, 18, 22), text("T"), fontcolour(LightGreen)
label   bounds( 65,  2, 18, 22), text("H"), fontcolour(OrangeRed)
label   bounds( 82,  6, 18, 22), text("M"), fontcolour(orange)
label   bounds( 97,  1, 18, 22), text("I"), fontcolour(SkyBlue)
label   bounds(110,  4, 18, 22), text("C"), fontcolour(Tomato)
label   bounds(125,  1, 18, 22), text("O"), fontcolour(LightGreen)
label   bounds(140,  3, 18, 22), text("N"), fontcolour(violet)

label   bounds( 81, 24,  78, 8), text("Iain McCurdy [2013]"), FontColour("white")

rslider bounds(  3, 45, 55, 55), channel("base"), textBox(1), range(0, 127,36, 1,1),fontcolour("white"),textcolour("white"),colour(orange), trackercolour(orange)
label   bounds( 19, 34, 30, 10), text("BASE"), fontcolour(white)
rslider bounds( 52, 45, 55, 55), channel("rate"), textBox(1), range(0.1, 5.00, 0.5, 0.5, 0.01),fontcolour("white"),textcolour("white"),colour(orange), trackercolour(orange)
label   bounds( 68, 34, 30, 10), text("RATE"), fontcolour(white)
rslider bounds(102, 45, 55, 54), channel("level"), textBox(1), range(0, 1.00, 0.5, 0.5, 0.01),fontcolour("white"),textcolour("white"),colour(orange), trackercolour(orange)
label   bounds(115, 34, 30, 10), text("LEVEL"), fontcolour(white)


checkbox bounds(  0, 101,  9, 10), channel("k1")  value(0) shape(square) colour(yellow)
checkbox bounds(  9, 101, 10, 10), channel("k2")  value(0) shape(square) colour(yellow)
checkbox bounds( 19, 101,  9, 10), channel("k3")  value(0) shape(square) colour(yellow)
checkbox bounds( 28, 101, 10, 10), channel("k4")  value(0) shape(square) colour(yellow)
checkbox bounds( 38, 101, 10, 10), channel("k5")  value(0) shape(square) colour(yellow)
checkbox bounds( 48, 101,  8, 10), channel("k6")  value(0) shape(square) colour(yellow)
checkbox bounds( 56, 101, 10, 10), channel("k7")  value(0) shape(square) colour(yellow)
checkbox bounds( 66, 101,  8, 10), channel("k8")  value(0) shape(square) colour(yellow)
checkbox bounds( 74, 101, 10, 10), channel("k9") value(0) shape(square) colour(yellow)
checkbox bounds( 84, 101,  9, 10), channel("k10") value(0) shape(square) colour(yellow)
checkbox bounds( 93, 101, 10, 10), channel("k11") value(0) shape(square) colour(yellow)
checkbox bounds(103, 101,  9, 10), channel("k12") value(0) shape(square) colour(yellow)
checkbox bounds(112, 101,  9, 10), channel("k13") value(0) shape(square) colour(yellow)
checkbox bounds(121, 101, 10, 10), channel("k14") value(0) shape(square) colour(yellow)
checkbox bounds(131, 101,  9, 10), channel("k15") value(0) shape(square) colour(yellow)
checkbox bounds(140, 101, 10, 10), channel("k16") value(0) shape(square) colour(yellow)
checkbox bounds(150, 101, 10, 10), channel("k17") value(0) shape(square) colour(red)

keyboard bounds(-561, 110,1200, 70)
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-dm0 -n -+rtmidi=null -M0
</CsOptions>

<CsInstruments>
sr 	= 	44100
ksmps 	= 	64
nchnls 	= 	2
0dbfs	=	1
massign	0,1

; Author: Iain McCurdy (2013)

giAmpScl	ftgen	0,0,-20000,-16, 1,100,0,1, 20000-100,-100,0.2

opcode	SwitchPort, k, kkk
	kin,kUpPort,kDnPort	xin
	kold			init	0
	kporttime		=	(kin<kold?kDnPort:kUpPort)
	kout			portk	kin, kporttime
	kold			=	kout
				xout	kout
endop

	instr	1	; Listen for midi note changes. Turn lights on or off. Start the rhymicon of at least 1 note is held.
inum	notnum				; read in midi note number

#define	LightToggle(N)	#		; define a macro to reduce code repetition
if inum=(59+$N) then			; if note number played on the keyboard corresponds to 'this' toggle light... (note that first note will be C3 (59 + 1)
 ik$N	chnget	"k$N"			; read current value for 'this' toggle light (at i-time)
 	chnset	1-ik$N,"k$N"		; write the inverse to it (i.e. toggle it)
endif#
$LightToggle(1)				; expand macro for all 16 layers
$LightToggle(2)
$LightToggle(3)
$LightToggle(4)
$LightToggle(5)
$LightToggle(6)
$LightToggle(7)
$LightToggle(8)
$LightToggle(9)
$LightToggle(10)
$LightToggle(11)
$LightToggle(12)
$LightToggle(13)
$LightToggle(14)
$LightToggle(15)
$LightToggle(16)
$LightToggle(17)

kk1	chnget	"k1"
kk2	chnget	"k2"
kk3	chnget	"k3"
kk4	chnget	"k4"
kk5	chnget	"k5"
kk6	chnget	"k6"
kk7	chnget	"k7"
kk8	chnget	"k8"
kk9	chnget	"k9"
kk10	chnget	"k10"
kk11	chnget	"k11"
kk12	chnget	"k12"
kk13	chnget	"k13"
kk14	chnget	"k14"
kk15	chnget	"k15"
kk16	chnget	"k16"

ksum	=	kk1 + kk2 + kk3 + kk4 + kk5 + kk6 + kk7 + kk8 + kk9 + kk10 + kk11 + kk12 + kk13 + kk14 + kk15 + kk16
if ksum>0 then
 schedkwhen	1,0,1,2,0,-1
else
 turnoff2	2,0,1
endif
	endin

	instr	2
kporttime	linseg	0,0.001,0.05
kbase	chnget	"base"			; base pitch as a note number
krate	chnget	"rate"			; rate of pulsation of fundemental
krate	portk	krate, kporttime
klevel	chnget	"level"			; output amplitude level
ksync	chnget	"k17"			; syncopation mode
kBaseFrq	=	cpsmidinn(kbase)
kBaseFrq	portk	kBaseFrq,kporttime
iamp		=	0.6
amix		=	0

#define	LAYER(N)#					; use a macro to reduce code repetition
kk$N	chnget	"k$N"
kk$N	SwitchPort	kk$N,kporttime*0.01,kporttime*0.5
ktrig	metro	krate * $N
kn	=	$N					; layer number as a variable
if ksync=1&&kn>1 then					; if syncopation is on and this isn't the first layer (syncopation irrelevant for first layer)
 ktrig	vdelayk	ktrig, (1/(krate*$N))/$N    , 1/0.1	; add a syncopation delay for the metronome trigger for this layer
endif
kenv loopseg $N/60, ktrig, 0, 0,0.025, 1,0.84/krate, 0,60, 0 ; retriggering envelope
aenv	interp	kenv*kk$N					; interpolate to a-rate (for better smoothness)
kcps	=	kBaseFrq*$N				; pitch for this layer (in cycles per second)
kscl	table	kcps,giAmpScl				; amp scaling value (to quieten higher sounds)
kamp	=	iamp*kscl				; apply on/off switch for this layer to its amplitude
asig	vco2	kamp,kcps,2,0.1				; generate a tone (pulse wave)
kcf	limit	cpsoct(octcps(kcps*3)*kenv),1,10000	; filter envelope
asig	clfilt	asig,kcf,0,2				; butterworth lowpass filter the sound
asig	=	asig*aenv				; apply amplitude envelope
amix	=	amix + asig#				; add to 'mix' signal with the other layers

$LAYER(1)						; expand macro multiple times. Once for each layer
$LAYER(2)
$LAYER(3)
$LAYER(4)
$LAYER(5)
$LAYER(6)
$LAYER(7)
$LAYER(8)
$LAYER(9)
$LAYER(10)
$LAYER(11)
$LAYER(12)
$LAYER(13)
$LAYER(14)
$LAYER(15)
$LAYER(16)
	xtratim	0.5
	outs	amix*klevel, amix*klevel
	endin	


</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]		;read widgets
;i 2 0 [3600*24*7]		;read widgets
</CsScore>

</CsoundSynthesizer>