; EnvelopeFollower.csd
; Written by Iain McCurdy, 2012.

<Cabbage>
form caption("Envelope Follower") size(530, 100), pluginID("envf")
image                  pos(0, 0), size(530, 100), colour("brown"), shape("rounded"), outlinecolour("white"), outlinethickness(4)
checkbox bounds(20, 70, 15, 15), channel("lev1"), value(0), colour(200,220,0,255), active(0)
checkbox bounds(20, 55, 15, 15), channel("lev2"), value(0), colour(250,190,0,255), active(0)
checkbox bounds(20, 40, 15, 15), channel("lev3"), value(0), colour(255,120,0,255), active(0)
checkbox bounds(20, 25, 15, 15), channel("lev4"), value(0), colour(255, 60,0,255), active(0)
checkbox bounds(20, 10, 15, 15), channel("lev5"), value(0), colour(255,  0,0,255), active(0)
rslider bounds( 40, 11, 75, 75), text("Sensitivity"), channel("sens"),  range(0, 1, 0.65),                   colour(255,100,100), textcolour(255,255,200), trackercolour(255,255,150)
rslider bounds(110,  6, 45, 45), text("Att."),        channel("att"),   range(0.001, 0.5, 0.01, 0.5, 0.001), colour(255,200,100), textcolour(255,255,200), trackercolour(255,255,150)
rslider bounds(110, 51, 45, 45), text("Dec."),        channel("rel"),   range(0.001, 0.5, 0.2, 0.5, 0.001),  colour(255,200,100), textcolour(255,255,200), trackercolour(255,255,150)
rslider bounds(150, 11, 75, 75), text("Frequency"),   channel("freq"),  range(10, 10000, 1000, 0.5),         colour(255,100,100), textcolour(255,255,200), trackercolour(255,255,150)
label    bounds(225, 15, 85, 14), text("Type"), fontcolour(255,255,200)
combobox bounds(225, 30, 85, 20), text("lpf18","moogladder","butlp","tone"), value("1"), channel("type")
rslider bounds(310, 11, 75, 75), text("Resonance"),   channel("res"),   range(0,  1, 0.75),                  colour(255,100,100), textcolour(255,255,200), trackercolour(255,255,150)
rslider bounds(380, 11, 75, 75), text("Distortion"),  channel("dist"),  range(0,  1.00, 0),                  colour(255,100,100), textcolour(255,255,200), trackercolour(255,255,150)
rslider bounds(450, 11, 75, 75), text("Level"),       channel("level"), range(0, 1.00, 1),                   colour(255,200,100), textcolour(255,255,200), trackercolour(255,255,150)
}
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-d -n
</CsOptions>
<CsInstruments>
sr = 44100
ksmps = 64
nchnls = 2
0dbfs = 1

; Author: Iain McCurdy (2012)

opcode	EnvelopeFollower,a,akkkkkkk
	ain,ksens,katt,krel,kfreq,ktype,kres,kdist	xin			; READ IN INPUT ARGUMENTS
	setksmps	4
	;				     ATTCK.REL.  -  ADJUST THE RESPONSE OF THE ENVELOPE FOLLOWER HERE
	aFollow		follow2		ain, katt, krel			; AMPLITUDE FOLLOWING AUDIO SIGNAL
	kFollow		downsamp	aFollow				; DOWNSAMPLE TO K-RATE
	kFollow		expcurve	kFollow/0dbfs,0.5		; ADJUSTMENT OF THE RESPONSE OF DYNAMICS TO FILTER FREQUENCY MODULATION
	kFrq		=		kfreq + (kFollow*ksens*10000)	; CREATE A LEFT CHANNEL MODULATING FREQUENCY BASE ON THE STATIC VALUE CREATED BY kfreq AND THE AMOUNT OF DYNAMIC ENVELOPE FOLLOWING GOVERNED BY ksens
	kFrq		limit		kFrq, 20,sr/2			; LIMIT FREQUENCY RANGE TO PREVENT OUT OF RANGE FREQUENCIES  
	if ktype==1 then
	 aout		lpf18		ain, kFrq, kres, kdist		; REDEFINE AUDIO SIGNAL AS FILTERED VERSION OF ITSELF
	elseif ktype==2 then
	 aout		moogladder	ain, kFrq, kres			; REDEFINE AUDIO SIGNAL AS FILTERED VERSION OF ITSELF
	elseif ktype==3 then
	 aFrq	interp	kFrq
	 aout		butlp		ain, aFrq			; REDEFINE AUDIO SIGNAL AS FILTERED VERSION OF ITSELF
	elseif ktype==4 then
	 aout		tone		ain, kFrq			; REDEFINE AUDIO SIGNAL AS FILTERED VERSION OF ITSELF
	endif
			xout		aout				; SEND AUDIO BACK TO CALLER INSTRUMENT
endop

opcode	SwitchPort, k, kii
	kin,iupport,idnport	xin
	kold			init	0
	kporttime		=	(kin<kold?idnport:iupport)
	kout			portk	kin, kporttime
	kold			=	kout
				xout	kout
endop

instr 1
ksens chnget "sens"
katt chnget "att"
krel chnget "rel"
kfreq chnget "freq"
ktype chnget "type"
ktype	init	1
kres chnget "res"
kdist chnget "dist"
klevel chnget "level"
a1,a2	ins
;a1,a2	diskin2	"808loop.wav",1,0,1
;a1	=	a1*0.4
;a2	=	a2*0.4


/*level meter*/
amix	sum	a1,a2
krms	rms	amix*0.5
krms	pow	krms,0.75
krms	SwitchPort	krms,0.01,0.2
kon	=	1
koff	=	0
#define	INDICATOR(LEV'N)
#
kOnTrig		trigger	krms,$LEV^1.5,0
kOffTrig	trigger	krms,$LEV^1.5,1
if	kOnTrig==1 then
 chnset	kon,"lev$N"
elseif kOffTrig==1 then
 chnset	koff,"lev$N"
endif
#
$INDICATOR(1/6'1)
$INDICATOR(2/6'2)
$INDICATOR(3/6'3)
$INDICATOR(4/6'4)
$INDICATOR(5/6'5)

a1	EnvelopeFollower	a1,ksens,katt,krel,kfreq,ktype,kres*0.95,kdist*100
a2	EnvelopeFollower	a2,ksens,katt,krel,kfreq,ktype,kres*0.95,kdist*100
a1	=	a1 * klevel * (1 - ((kdist*0.3)^0.02))	;scale amplitude according to distortion level (to compensate for gain increases it applies)
a2	=	a2 * klevel * (1 - ((kdist*0.3)^0.02))
	outs	a1,a2
endin

</CsInstruments>
<CsScore>
i 1 0 [60*60*24*7]
</CsScore>
</CsoundSynthesizer>