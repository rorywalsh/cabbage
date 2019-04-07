; wguide1.csd
; Written by Iain McCurdy, 2013,2015.

; Encapsulation of the 'wguide1' opcode, used for filtering and synthesis.
; A waveguide filter produces resonance peaks that follow the frequencies of the harmonic series with diminishing strength above its defined base frequency.

; frequency of the wave guide can be determined either in hertz of as a note number

; Activating the 'Keyboard Input' switch will allow frequency control from the keyboard (internal or external) and polyphony. 
;  In this mode, 'Frequency' and 'Note Num.' controls will be disabled.

; Source input sound can be either 'live audio input', or synthetic gravel, dust, air, a click or a pluck.
;  Click and pluck are intended to used with 'Keyboard Input' activated.

; Frequency	-	frequency of the waveguide
; Note Num.	-	frequency of the waveguide expressed as a note number
; Width		-	offsets the frequencies of the left and right channels to imbue added stereo width
; Cutoff	-	cutoff frequency of a lowpass filter within the feedback loop of the waveguide
;			 cutoff can be defined as a fixed value (in hertz) or as a ratio with the frequency of the waveguide (Ratio x Frequency = cutoff)
; Feedback	-	feedback ratio of the waveguide unit.
;			 negative feedback will shift the fundemental down one octave and only odd harmonics will be preset

; Bandpass Filter
; ---------------
; A double butterworth bandpass filter can be inserted, the cutoff frequency of which relates to the frequency defined for the waveguide.
; Bandpass On/Off	-	turns the bandpass filter on and off
; Balance	-	inserts a dynamic balancing to compensate for power loss on account of the bandpass filtering
; Width		-	bandwidth of the filters
; Harm.		-	ratio of bandpass filter cutoff to waveguide frequency: Harm x Frequency = bandpass cutoff frequency

; Mix	-	dry/wet mix
; Level	-	amplitude scaling

<Cabbage>
form caption("wguide1") size(650,200), pluginid("WGu1"), scrollbars(0) style("legacy")
image           bounds(0, 0, 650,120), colour(125, 95, 55), shape("sharp"), outlinecolour(225,195,155), outlinethickness(2) 
label   bounds(505,  4,136, 17), align("centre"), text("W A V E G U I D E"), fontcolour(silver), align("left")
image   bounds(510,  7,125,  1), colour(200,170,130), shape("sharp"), outlinecolour(silver), outlinethickness(1)
image   bounds(512, 18,124,  1), colour(200,170,130), shape("sharp"), outlinecolour(silver), outlinethickness(1)
checkbox bounds( 20,  7,120, 12), text("Keyboard Input"), channel("input"), fontcolour("white"), colour(yellow)
checkbox bounds( 20, 22,120, 12), text("Low Cut"), channel("LowCut") fontcolour("white") colour(yellow) value(0)
label    bounds(140,  5, 62, 11), text("Source"), fontcolour("white")
combobox bounds(140, 16, 62, 16), channel("source"), value(1), text("Live","Gravel","Dust","Air","Click","Pluck")
image    bounds( 10, 41,145, 70), colour(0,0,0,0), plant("GUIcontrol"), identchannel("NoKeyboardID"), visible(1) {
rslider  bounds(  0,  0, 70, 70),  text("Frequency"),  channel("freq"),      trackercolour(225,195,155), range(8.2, 12542, 160, 0.25),  colour( 85, 55,15), textcolour(white), fontcolour("silver")
rslider  bounds( 65,  0, 70, 70),  text("Note Num."),  channel("notnum"),    trackercolour(225,195,155), range(0, 127, 51, 1,1),        colour( 85, 55,15), textcolour(white), fontcolour("silver")
}
rslider  bounds(140, 41, 70, 70),  text("Width"),      channel("StWidth"),   trackercolour(225,195,155), range(-0.2, 0.2, 0, 1,0.001),        colour( 85, 55,15), textcolour(white), fontcolour("silver")

image    bounds(207,  4, 66,112), colour(125, 95, 55), outlinecolour("silver"), outlinethickness(1), line(1)
label    bounds(212,  8, 56, 11),  text("Cutoff"), fontcolour("white")
combobox bounds(212, 20, 56, 15),  text("Fixed","Ratio"),channel("CutoffMode"), value(1)
rslider  bounds(205, 41, 70, 70),  text("Hertz"),     channel("cutoff"),     trackercolour(225,195,155), range(20,20000,8000,0.25), visible(1), colour( 85, 55,15), textcolour(white), fontcolour("silver"), identchannel("cutoff_ident")
rslider  bounds(205, 41, 70, 70),  text("Ratio"),     channel("CutoffRatio"),trackercolour(225,195,155), range(1,40,8),             visible(0), colour( 85, 55,15), textcolour(white), fontcolour("silver"), identchannel("CutoffRatio_ident")

rslider  bounds(270, 41, 70, 70),  text("Feedback"),   channel("feedback"),   trackercolour(225,195,155), range(-0.9999999, 0.9999999, 0.8), colour( 85, 55,15), textcolour(white), fontcolour("silver")

line     bounds(345,  5,  2,110)
checkbox bounds(365,  7,120, 12), text("Bandpass On/Off"), channel("BPFOnOff") fontcolour("white") colour(yellow) value(0)
checkbox bounds(365, 22,120, 12), text("Balance"),         channel("BalanceOnOff") fontcolour("white") colour(yellow) value(0)
rslider  bounds(355, 41, 70, 70),  text("Bandwidth"),      channel("Bandwidth"),   trackercolour(225,195,155), range(0.001,100.00,0.2,0.5,0.001),             colour( 85, 55,15), textcolour(white), fontcolour("silver")
rslider  bounds(420, 41, 70, 70),  text("Harm."),          channel("HarmRatio"),trackercolour(225,195,155), range(1,48,1,1,1),             colour( 85, 55,15), textcolour(white), fontcolour("silver")
line     bounds(495,  5,  2,110)

rslider  bounds(505, 41, 70, 70),  text("Mix"),        channel("mix"),       trackercolour(225,195,155), range(0, 1.00, 1),             colour( 85, 55,15), textcolour(white), fontcolour("silver")
rslider  bounds(570, 41, 70, 70),  text("Level"),      channel("level"),     trackercolour(225,195,155), range(0, 5.00, 0.7,0.5,0.001),           colour( 85, 55,15), textcolour(white), fontcolour("silver")

keyboard bounds(  0,210, 650, 80), visible(1), identchannel("keyboardID")

</Cabbage>

<CsoundSynthesizer>

<CsOptions>
;-d -n
-dm0 -n -+rtmidi=null -M0
</CsOptions>

<CsInstruments>

sr 		= 	44100	; SAMPLE RATE
ksmps 		= 	32	; NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls 		= 	2	; NUMBER OF CHANNELS (2=STEREO)
0dbfs		=	1
massign	0,2

; Author: Iain McCurdy (2012)


instr	1
	kporttime	linseg	0,0.01,0.03				; CREATE A VARIABLE THAT WILL BE USED FOR PORTAMENTO TIME
	gkinput		chnget	"input"					; READ WIDGETS...
	gkLowCut	chnget	"LowCut"
	gkfreq		chnget	"freq"
	gknotnum	chnget	"notnum"
	gkBPFOnOff	chnget	"BPFOnOff"
	gkBalanceOnOff	chnget	"BalanceOnOff"
	gkBandwidth	chnget	"Bandwidth"
	gkHarmRatio	chnget	"HarmRatio"
	gkHarmRatio	port	gkHarmRatio,0.5
	gkCutoffMode	chnget	"CutoffMode"
	gkCutoffMode	init	1
	gkcutoff	chnget	"cutoff"
	gkCutoffRatio	chnget	"CutoffRatio"
	gkfeedback	chnget	"feedback"
	gkmix		chnget	"mix"
	gkmix		portk	gkmix,kporttime
	gklevel		chnget	"level"
	gklevel		portk	gklevel,kporttime
	gkfreq		portk	gkfreq,kporttime
	gafreq		interp	gkfreq
	gkStWidth	chnget	"StWidth"

	if changed(gkinput)==1||timeinstk()==1 then
	 chnset	sprintfk("visible(%d)",1-gkinput),"NoKeyboardID"
;	 chnset	sprintfk("visible(%d)",gkinput),"keyboardID"	 
	endif

	schedkwhen	trigger(gkinput,0.5,0),0,0,99,0,1,210,115
	schedkwhen	trigger(gkinput,0.5,1),0,0,99,0,1,120,210

	ktrig1		changed	gkfreq
	ktrig2		changed	gknotnum
	if ktrig1==1 then						; DUAL FREQUENCY AND NOTE NUMBER CONTROLS
	 koct	=	octcps(gkfreq)
	 chnset	(koct-3)*12,"notnum"
	elseif ktrig2==1 then
	 chnset	cpsmidinn(gknotnum),"freq"
	endif
	
	event_i	"i",2,0,-1
	ktrig	trigger	gkinput,0.5,2
	if ktrig=1 then
	 if gkinput=0 then
	  event	"i",2,0,-1
	 else
	  turnoff2	2,0,0
	 endif
	endif

	/* SOURCE SOUND */
	gksource	chnget	"source"
	if gksource==1 then
 	 gasigL, gasigR	ins
 	elseif gksource==2 then
 	 gasigL	gausstrig	0.5, 100, 8, 0
 	 gasigR	gausstrig	0.5, 100, 8, 0
 	elseif gksource==3 then
 	 kdens	random	475,525
	 gasigL	dust2	0.5, kdens
 	 kdens	random	525,475
	 gasigR	dust2	0.5, kdens
 	elseif gksource==4 then
	 gasigL	pinkish	0.2
	 gasigR	pinkish	0.2
	endif	
endin


instr	2
	/* MIDI AND GUI INTEROPERABILITY */
	iMIDIflag	=	0			; IF MIDI ACTIVATED = 1, NON-MIDI = 0
	mididefault	1, iMIDIflag			; IF NOTE IS MIDI ACTIVATED REPLACE iMIDIflag WITH '1'

	if iMIDIflag==1 then				; IF THIS IS A MIDI ACTIVATED NOTE...
	 inum	notnum
	 ivel	veloc	0,1
	 p1	=	p1 + (rnd(1000)*0.0001)
	 if gkinput=0 then
	  turnoff
	 endif
	 icps	cpsmidi					; READ MIDI PITCH VALUES - THIS VALUE CAN BE MAPPED TO GRAIN DENSITY AND/OR PITCH DEPENDING ON THE SETTING OF THE MIDI MAPPING SWITCHES
	 kfreq	init		icps
	 afreq	init		icps
	else
	 kfreq	=		gkfreq
	 afreq	=		gafreq 
	endif						; END OF THIS CONDITIONAL BRANCH

	if gksource==5 then
	 asigL	mpulse	ivel*5,0,0.001
	 asigR	=	asigL
	elseif gksource==6 then
 	 kenv	expseg	1,0.08,0.001,1,0.001
 	 asigL	dust2	20*(kenv-0.001),100
 	 icf	=	icps*(1+(4*ivel))
 	 icf	limit	icf,20,4000
 	 asigL	butlp	asigL,icf
 	 asigR	=	asigL
	else
	 aenv		linsegr	0,0.05,1,0.05,0
	 asigL	=	gasigL*aenv
	 asigR	=	gasigR*aenv
	endif
	
	if changed(gkCutoffMode)==1 then
	 if gkCutoffMode==1 then
	  chnset	"visible(1)","cutoff_ident"
	  chnset	"visible(0)","CutoffRatio_ident"
	 else
	  chnset	"visible(0)","cutoff_ident"
	  chnset	"visible(1)","CutoffRatio_ident"
	 endif
	endif


	if gkCutoffMode==2 then
	 kcutoff	limit	gkCutoffRatio*kfreq,20,sr/2
	else
	 kcutoff	=	gkcutoff
	endif	

	kLDiff		=	semitone(-gkStWidth)
	kRDiff		=	semitone(gkStWidth)	
	
	aresL 		wguide1 asigL, afreq*kLDiff, kcutoff, gkfeedback
	aresR 		wguide1 asigR, afreq*kRDiff, kcutoff, gkfeedback
	aresL 		dcblock	aresL
	aresR 		dcblock	aresR
	
	/* LOW CUT */
	if gkLowCut==1 then
	 aresL		buthp	aresL, kfreq
	 aresR		buthp	aresR, kfreq
	 aresL		buthp	aresL, kfreq
	 aresR		buthp	aresR, kfreq
	endif
	
	/* BANDPASS FILTER */
	if gkBPFOnOff==1 then
	 aFiltL		butbp	aresL,kfreq*gkHarmRatio,kfreq*gkBandwidth
	 aFiltR		butbp	aresR,kfreq*gkHarmRatio,kfreq*gkBandwidth
	 aFiltL		butbp	aFiltL,kfreq*gkHarmRatio,kfreq*gkBandwidth
	 aFiltR		butbp	aFiltR,kfreq*gkHarmRatio,kfreq*gkBandwidth
	 if gkBalanceOnOff==1 then
	  aresL	balance	aFiltL,aresL
	  aresR	balance	aFiltR,aresR
	 else
	  aresL	=	aFiltL
	  aresR	=	aFiltR
	 endif
	endif
	
	amixL		ntrpol	gasigL*aenv, aresL, gkmix
	amixR		ntrpol	gasigR*aenv, aresR, gkmix

	kenv		linsegr	1,15,0
	amixL		=	amixL * gklevel * kenv
	amixR		=	amixR * gklevel * kenv
	ktime		timeinsts
	krms		rms	amixL,3
	if krms<0.00001&&ktime>0.2&&iMIDIflag==1 then
	 turnoff2	p1,4,0
	endif

			outs	amixL, amixR		; WGUIDE1 OUTPUTS ARE SENT OUT
endin

instr	99
	ky	line	p4,p3,p5
	if metro(16)==1 then
	 chnset	sprintfk("pos(0,%d)",ky),"keyboardID"
	endif
endin
</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
</CsScore>


</CsoundSynthesizer>



























