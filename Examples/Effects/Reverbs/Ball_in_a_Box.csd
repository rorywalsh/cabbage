; Ball_in_a_Box.csd
; Written by Iain McCurdy, 2015

; Ball in a Box is a physical model reverb based on the notional idea of a ball (sound) within a bax (reverberant space)

; Room Size (X, Y, Z)		-	room size in metres
; Source Location (X, Y, Z)	-	location of the sound as a ratio 0 to 1 of the entire space
; Receive Location (X, Y, Z)	-	receiver location - in metres - from the centre of the space
; Reverb Decay			-	main decay of the resonator (default: 0.99)
; High Frequency Diffusion	-	is the coefficient of diffusion at the walls, which regulates the amount of diffusion (0-1, where 0 = no diffusion, 1 = maximum diffusion - default: 1)
; Direct Signal Attenuation	-	the attenuation of the direct signal (0-1, default: 0.5)
; Early Reflection Diffusion	-	the attenuation coefficient of the early reflections (0-1, default: 0.8)
; Pick-up Separation		-	the distance in meters between the two pickups (your ears, for example - default: 0.3)

<Cabbage>
form caption("Ball in a Box") size(800,395), pluginid("BABO"), colour(100,100,120) style("legacy")

image   bounds(  3,  5,394,125), outlinecolour("white"), outlinethickness(1), colour(0,0,0,0), plant("RoomSize") {
hslider bounds(  5, 10,390, 30), textcolour(white), channel("rx"), range(0.1,20.00, 5), colour(220,220,250), trackercolour(240,210,170)
label   bounds(  5, 33,390, 12), text("Room Size X"), fontcolour(200,200,200)
hslider bounds(  5, 45,390, 30), textcolour(white), channel("ry"), range(0.1,20.00, 6), colour(220,220,250), trackercolour(240,210,170)
label   bounds(  5, 68,390, 12), text("Room Size Y"), fontcolour(200,200,200)
hslider bounds(  5, 80,390, 30), textcolour(white), channel("rz"), range(0.1,20.00, 4), colour(220,220,250), trackercolour(240,210,170)
label   bounds(  5,103,390, 12), text("Room Size Z"), fontcolour(200,200,200)
}

image   bounds(  3,135,394,125), outlinecolour("white"), outlinethickness(1), colour(0,0,0,0), plant("SourceLocation") {
hslider bounds(  5, 10,390, 30), textcolour(white), channel("srcx"), range(0,1.00, 0.131), colour(220,220,250), trackercolour(240,210,170)
label   bounds(  5, 33,390, 12), text("Source Location X"), fontcolour(200,200,200)
hslider bounds(  5, 45,390, 30), textcolour(white), channel("srcy"), range(0,1.00, 0.243), colour(220,220,250), trackercolour(240,210,170)
label   bounds(  5, 68,390, 12), text("Source Location Y"), fontcolour(200,200,200)
hslider bounds(  5, 80,390, 30), textcolour(white), channel("srcz"), range(0,1.00, 0.717), colour(220,220,250), trackercolour(240,210,170)
label   bounds(  5,103,390, 12), text("Source Location Z"), fontcolour(200,200,200)
}

image   bounds(  3,265,394,125), outlinecolour("white"), outlinethickness(1), colour(0,0,0,0), plant("ReceiveLocation") {
hslider bounds(  5, 10,390, 30), textcolour(white), channel("rcvx"), range(-10,10,7.331), colour(220,220,250), trackercolour(240,210,170)
label   bounds(  5, 33,390, 12), text("Receive Location X"), fontcolour(200,200,200)
hslider bounds(  5, 45,390, 30), textcolour(white), channel("rcvy"), range(-10,10,-3.973), colour(220,220,250), trackercolour(240,210,170)
label   bounds(  5, 68,390, 12), text("Receive Location Y"), fontcolour(200,200,200)
hslider bounds(  5, 80,390, 30), textcolour(white), channel("rcvz"), range(-10,10, 6.791), colour(220,220,250), trackercolour(240,210,170)
label   bounds(  5,103,390, 12), text("Receive Location Z"), fontcolour(200,200,200)
}

image   bounds(403,  5,394,105), outlinecolour("white"), outlinethickness(1), colour(0,0,0,0), plant("Mixer") {
hslider bounds(  5, 10,390, 30), textcolour(white), channel("mix"), range(0, 1.00, 0.5), colour(220,220,250), trackercolour(240,210,170)
label   bounds(  5, 35,390, 12), text("Dry/Wet Mix"), fontcolour(200,200,200)
hslider bounds(  5, 55,390, 30), textcolour(white), channel("level"), range(0, 1.00, 0.5), colour(220,220,250), trackercolour(240,210,170)
label   bounds(  5, 80,390, 12), text("Level"), fontcolour(200,200,200)
}

image   bounds(403,115,394,275), outlinecolour("white"), outlinethickness(1), colour(0,0,0,0), plant("Filters") {
hslider bounds(  5, 20,390, 30), textcolour(white), channel("decay"), range(0.01, 1.00, 0.3), colour(220,220,250), trackercolour(240,210,170)
label   bounds(  5, 45,390, 12), text("Reverb Decay"), fontcolour(200,200,200)
hslider bounds(  5, 60,390, 30), textcolour(white), channel("diff"), range(0.01, 1.00, 1), colour(220,220,250), trackercolour(240,210,170)
label   bounds(  5, 85,390, 12), text("High Frequency Diffusion"), fontcolour(200,200,200)
hslider bounds(  5,100,390, 30), textcolour(white), channel("hydecay"), range(0.001, 1.00, 0.1), colour(220,220,250), trackercolour(240,210,170)
label   bounds(  5,125,390, 12), text("High Frequency Decay"), fontcolour(200,200,200)
hslider bounds(  5,140,390, 30), textcolour(white), channel("direct"), range(0, 1.00, 0.5), colour(220,220,250), trackercolour(240,210,170)
label   bounds(  5,165,390, 12), text("Direct Signal Attenuation"), fontcolour(200,200,200)
hslider bounds(  5,180,390, 30), textcolour(white), channel("early_diff"), range(0, 1.00, 0.8), colour(220,220,250), trackercolour(240,210,170)
label   bounds(  5,205,390, 12), text("Early Reflection Diffusion"), fontcolour(200,200,200)
hslider bounds(  5,220,390, 30), textcolour(white), channel("rdistance"), range(0.001, 9.00, 0.3), colour(220,220,250), trackercolour(240,210,170)
label   bounds(  5,245,390, 12), text("Pick-up Separation"), fontcolour(200,200,200)
}

</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-d -n
</CsOptions>

<CsInstruments>

sr 		= 	44100
ksmps 		= 	32
nchnls 		= 	2
0dbfs		=	1

;Author: Iain McCurdy (2015)

instr	1	; read widgets
 gkrx		chnget	"rx"        
 gkry           chnget	"ry"        
 gkrz           chnget	"rz"        
 gksrcx         chnget	"srcx"      
 gksrcy         chnget	"srcy"      
 gksrcz         chnget	"srcz"      
 gkdiff         chnget	"diff"      
 gkdecay        chnget	"decay"     
 gkrdistance    chnget	"rdistance" 
 gkhydecay      chnget	"hydecay"   
 gkdirect       chnget	"direct"    
 gkearly_diff   chnget	"early_diff"
 gkrcvx         chnget	"rcvx"      
 gkrcvy         chnget	"rcvy"      
 gkrcvz         chnget	"rcvz"
 gkmix          chnget	"mix"
 gklevel	chnget	"level"
endin

instr	2	;REVERB
 aL,aR	ins	; read 
 kSwitch		changed		gkrx, gkry, gkrz, gksrcx, gksrcy, gksrcz, gkdiff, gkdecay, gkrdistance, gkhydecay, gkdirect, gkearly_diff, gkrcvx, gkrcvy, gkrcvz	;GENERATE A MOMENTARY '1' PULSE IN OUTPUT 'kSwitch' IF ANY OF THE SCANNED INPUT VARIABLES CHANGE. (OUTPUT 'kSwitch' IS NORMALLY ZERO)
 if	kSwitch=1	then	;IF kSwitch=1 THEN
 	reinit	UPDATE		;BEGIN A REINITIALIZATION PASS FROM LABEL 'UPDATE'
 endif				;END OF CONDITIONAL BRANCHING
 UPDATE:				;A LABEL
 irx		init	i(gkrx)	;CREATE I-RATE VARIABLES FROM K-RATE VARIABLES
 iry		init	i(gkry) ;CREATE I-RATE VARIABLES FROM K-RATE VARIABLES
 irz		init	i(gkrz) ;CREATE I-RATE VARIABLES FROM K-RATE VARIABLES
 ksrcx		init	i(gksrcx) * irx	;THE ACTUAL LOCATION OF THE SOURCE SOUND IS DEFINED RELATIVE TO THE SIZE OF THE ROOM
 ksrcy		init	i(gksrcy) * iry	;THE ACTUAL LOCATION OF THE SOURCE SOUND IS DEFINED RELATIVE TO THE SIZE OF THE ROOM
 ksrcz		init	i(gksrcz) * irz	;THE ACTUAL LOCATION OF THE SOURCE SOUND IS DEFINED RELATIVE TO THE SIZE OF THE ROOM
 idiff		init	i(gkdiff) ;CREATE I-RATE VARIABLES FROM K-RATE VARIABLES
 giBaboVals	ftgen	1, 0, 8, -2, i(gkdecay), i(gkhydecay), i(gkrcvx), i(gkrcvy), i(gkrcvz), i(gkrdistance), i(gkdirect), i(gkearly_diff)
 aRvbL, aRvbR	babo	aL + aR, ksrcx, ksrcy, ksrcz, irx, iry, irz, idiff, giBaboVals	;BABO REVERBERATOR
 rireturn			;RETURN TO PERFORMANCE TIME PASSES
 		outs	((aRvbL*gkmix)+(aL*(1-gkmix)))*gklevel, ((aRvbR*gkmix)+(aR*(1-gkmix)))*gklevel
endin

		
</CsInstruments>

<CsScore>
i 1 0   3600
i 2 0.1 3600	;REVERB INSTRUMENT PLAYS FOR 1 HOUR (AND KEEPS PERFORMANCE GOING)
</CsScore>

</CsoundSynthesizer>
