; GEN41.csd
; Written by Iain McCurdy, 2014.

; Demonstration of GEN41
; GEN41 can be used to genrate histogram-like function tables in which the number of times each value is repeated is defined by its associated probability value.
; The actual probabilities as fractions of 1 are the value divided by the sum of all probability values. (a total of '1' indicates all possibilities are represented.)

; In this example the histogram is used as a probability distribution from which notes are randomly chosen. Values define (midi) note numbers.

<Cabbage>
form caption("GEN41"), size(420, 395), pluginID("gn41"), colour(120,70,170,150)

gentable bounds(  5,  5, 410, 115), identchannel("table1"), tablenumber(1), tablecolour("yellow"), amprange(24,72,1), zoom(-1)

groupbox bounds(0, 125,420,170), text("Histogram"), plant("histogram"), fontcolour("white"){
rslider bounds(  0, 25, 70, 70), channel("prob1"), text("Prob. 1"), textBox(1), range(0, 100, 24,1,1), colour(160,110,210,200), trackercolour("white")
rslider bounds( 50, 25, 70, 70), channel("prob2"), text("Prob. 2"), textBox(1), range(0, 100, 64,1,1), colour(160,110,210,200), trackercolour("white")
rslider bounds(100, 25, 70, 70), channel("prob3"), text("Prob. 3"), textBox(1), range(0, 100, 64,1,1), colour(160,110,210,200), trackercolour("white")
rslider bounds(150, 25, 70, 70), channel("prob4"), text("Prob. 4"), textBox(1), range(0, 100, 34,1,1), colour(160,110,210,200), trackercolour("white")
rslider bounds(200, 25, 70, 70), channel("prob5"), text("Prob. 5"), textBox(1), range(0, 100, 64,1,1), colour(160,110,210,200), trackercolour("white")
rslider bounds(250, 25, 70, 70), channel("prob6"), text("Prob. 6"), textBox(1), range(0, 100, 84,1,1), colour(160,110,210,200), trackercolour("white")
rslider bounds(300, 25, 70, 70), channel("prob7"), text("Prob. 7"), textBox(1), range(0, 100, 64,1,1), colour(160,110,210,200), trackercolour("white")
rslider bounds(350, 25, 70, 70), channel("prob8"), text("Prob. 8"), textBox(1), range(0, 100, 64,1,1), colour(160,110,210,200), trackercolour("white")

rslider bounds(  0, 95, 70, 70), channel("val1"), text("Value 1"), textBox(1), range(24, 72, 50,1,1), colour(160,110,210,200), trackercolour("white")
rslider bounds( 50, 95, 70, 70), channel("val2"), text("Value 2"), textBox(1), range(24, 72, 62,1,1), colour(160,110,210,200), trackercolour("white")
rslider bounds(100, 95, 70, 70), channel("val3"), text("Value 3"), textBox(1), range(24, 72, 48,1,1), colour(160,110,210,200), trackercolour("white")
rslider bounds(150, 95, 70, 70), channel("val4"), text("Value 4"), textBox(1), range(24, 72, 44,1,1), colour(160,110,210,200), trackercolour("white")
rslider bounds(200, 95, 70, 70), channel("val5"), text("Value 5"), textBox(1), range(24, 72, 66,1,1), colour(160,110,210,200), trackercolour("white")
rslider bounds(250, 95, 70, 70), channel("val6"), text("Value 6"), textBox(1), range(24, 72, 54,1,1), colour(160,110,210,200), trackercolour("white")
rslider bounds(300, 95, 70, 70), channel("val7"), text("Value 7"), textBox(1), range(24, 72, 52,1,1), colour(160,110,210,200), trackercolour("white")
rslider bounds(350, 95, 70, 70), channel("val8"), text("Value 8"), textBox(1), range(24, 72, 46,1,1), colour(160,110,210,200), trackercolour("white")
}

groupbox bounds(0, 295,420,100), text("Synthesiser"), plant("synth"), fontcolour("white"){
checkbox bounds( 15, 50,115, 17), channel("SynthOnOff"), text("On/Off"),  value(0), colour("yellow"), shape("square")
rslider  bounds( 75, 25, 70, 70), channel("lev"),  text("Level"), textBox(1), range(0, 1.00, 0.7), colour(160,110,210,200), trackercolour("white")
rslider  bounds(125, 25, 70, 70), channel("rate"), text("Rate"),  textBox(1), range(0.2, 5.00, 2), colour(160,110,210,200), trackercolour("white")
rslider  bounds(175, 25, 70, 70), channel("dur"),  text("Dur."),  textBox(1), range(0.1, 3.00, 2), colour(160,110,210,200), trackercolour("white")
}

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

; default waveform		
gihist	ftgen	1,0, 512, -41, 0, 100, 84, 256, 72, 128

instr	1
	iftlen	=	ftlen(1)		; length of function  table
	kftlen	init	iftlen
	
	; read in widgets
	kval1	chnget	"val1"
	kval2	chnget	"val2"
	kval3	chnget	"val3"
	kval4	chnget	"val4"
	kval5	chnget	"val5"
	kval6	chnget	"val6"
	kval7	chnget	"val7"
	kval8	chnget	"val8"

	kprob1	chnget	"prob1"
	kprob2	chnget	"prob2"
	kprob3	chnget	"prob3"
	kprob4	chnget	"prob4"
	kprob5	chnget	"prob5"
	kprob6	chnget	"prob6"
	kprob7	chnget	"prob7"
	kprob8	chnget	"prob8"

	kprob1	init	64
	kprob2	init	64
	kprob3	init	64
	kprob4	init	64
	kprob5	init	64
	kprob6	init	64
	kprob7	init	64
	kprob8	init	64

	gklev	chnget	"lev"
	gkSynthOnOff	chnget	"SynthOnOff"
	gkrate	chnget	"rate"
	gkdur	chnget	"dur"

	ktrig	changed		kval1,kval2,kval3,kval4,kval5,kval6,kval7,kval8, kprob1,kprob2,kprob3,kprob4,kprob5,kprob6,kprob7,kprob8
	if ktrig==1 then	; peg rate of update. Tables updated at this rate. If too slow, glitching will be heard in the output, particularly if random movement speed is high. If too high CPU performance will suffer.
	 reinit	UPDATE
	endif
	UPDATE:
	gihist	ftgen	1,0, iftlen, -41, i(kval1), i(kprob1), i(kval2), i(kprob2),   i(kval3),i(kprob3),   i(kval4),i(kprob4),   i(kval5),i(kprob5),   i(kval6),i(kprob6),   i(kval7),i(kprob7),   i(kval8),i(kprob8)
	rireturn

	if ktrig==1 then			; 
	 chnset	"tablenumber(1)", "table1"	; update table display	
	endif

	; TRIGGER SOME NOTES
	kNoteTrig	init	1
	krhy	trandom	kNoteTrig,0,3
	kNoteTrig	metro	gkrate*(2^int(krhy))
	schedkwhen	kNoteTrig*gkSynthOnOff,0,0,2,0,gkdur
endin

; SCALE FOR REFLECTION DEPENDENT UPON MIDI NOTE NUMBER
giScal	ftgen	0,0,128, -27,  0, 0.9, 24, 0.9, 36, 0.85, 48, 0.75, 60, 0.65, 72, 0.35, 84, 0.001, 96, 0.001, 127;, 0.001

opcode	FreqShifter,a,aki					; frequency shifter
	ain,kfshift,ifn	xin					;READ IN INPUT ARGUMENTS
	areal, aimag hilbert ain				;HILBERT OPCODE OUTPUTS TWO PHASE SHIFTED SIGNALS, EACH 90 OUT OF PHASE WITH EACH OTHER
	asin 	oscili       1,    kfshift,     ifn,          0
	acos 	oscili       1,    kfshift,     ifn,          0.25	
	;RING MODULATE EACH SIGNAL USING THE QUADRATURE OSCILLATORS AS MODULATORS
	amod1	=		areal * acos
	amod2	=		aimag * asin	
	;UPSHIFTING OUTPUT
	aFS	= (amod1 - amod2)
		xout	aFS				;SEND AUDIO BACK TO CALLER INSTRUMENT
endop

gisine	ftgen	0,0,4096,10,1

instr	2
	iNote table     rnd(1),gihist,1                 ; read a random value from the function table
	aEnv  linsegr	0, 0.005, 1, p3-0.105, 1, 0.1, 0 ; amplitude envelope
	iPlk  random	0.1, 0.3                         ; point at which to pluck the string
	iDtn  random    -0.05, 0.05                      ; random detune
	irefl	table	iNote, giScal
	aSig  wgpluck2  0.58, 0.5*gklev, cpsmidinn(iNote+iDtn), iPlk, irefl   
	kcf	expon	cpsoct(rnd(6)+6),p3,50
	aSig	clfilt	aSig, kcf, 0, 2					; butterworth lowpass filter	
	aSig	FreqShifter	aSig,44,gisine
	aL,aR pan2      aSig * aEnv, rnd(0.5)+0.5        ; random panning   
	      outs      aL, aR
endin

</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
</CsScore>

</CsoundSynthesizer>
