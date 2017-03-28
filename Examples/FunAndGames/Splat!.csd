; Splat.csd
; Written by Iain McCurdy, 2014.
; 
; Left click somewhere within the swarm to splat the flies. 
; Left click again somewhere within the field of splatted flies to unsplat them and bring them back to life.
; 
; Population	-	the number of flies in the swarm
; Liveliness	-	the speed with which flies will move about within the swarm
; Excursion	-	how far flies will move away from the centre of the swarm, therefore this also defines the area covered by the swarm.
; Fly Size	-	doesn't affect the physical size of flies but instead affects the central frequency of the buzzing tone they produce.
; Swarm Speed	-	the speed with which the swarm moves around the panel. The swarm can exit the panel completely.
; 
; If the swarm area (excursion) is very small it can become difficult to splat (and unsplat) it. 
; If this becomes a problem, simply raise the value for excursion, even while the swarm is splatted.

<Cabbage>
form caption("Splat!"), size(1000,530), colour(230,230,230),guirefresh(64)
image bounds(0,   0, 0, 0), colour(white), shape(ellipse), widgetarray("fly",50)
numberbox  bounds(  5,495,60,34), channel("Population"),    range(1, 50, 10,1,1),       textcolour(black), fontcolour(black), text("Population"), colour(white)
numberbox  bounds( 70,495,60,34), channel("Liveliness"),    range(0.1,10.00, 4,1,0.1),  textcolour(black), fontcolour(black), text("Liveliness"), colour(white)
numberbox  bounds(135,495,60,34), channel("Excursion"),     range(2,300, 50,1,1),       textcolour(black), fontcolour(black), text("Excursion"),  colour(white)
numberbox  bounds(200,495,60,34), channel("FlySize"),       range(2,12, 5,1,0.001),     textcolour(black), fontcolour(black), text("Fly Size"),   colour(white)
numberbox  bounds(265,495,80,34), channel("SwarmSpeed"),    range(0.1,10, 0.3,1,0.001), textcolour(black), fontcolour(black), text("Swarm Speed"),colour(white)
label    bounds(  0, 0,  0, 0), text("Click swarm to splat. Click again to unsplat."), align(centre), fontcolour(white), identchannel("instructions1")		; Instructions
label    bounds(350,519,100, 10), text("Iain McCurdy 2014"), align(left), fontcolour(0,0,0,150)
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-n -dm0
</CsOptions>

<CsInstruments>

sr = 44100
ksmps = 32
nchnls = 2
0dbfs = 1
seed	0

giPanelWidth	=	1000
giPanelHeight	=	530
gisine	ftgen	0,0,131072,10,1
giMaxFlies	=	50

; UDO that provide different portamento times whether a value is rising or falling
opcode	SwitchPort, k, kkk
	kin,kUpPort,kDnPort	xin
	kold			init	0
	kporttime		=	(kin<kold?kDnPort:kUpPort)
	kout			portk	kin, kporttime
	kold			=	kout
				xout	kout
endop

; UDO for an i-rate version of the 'scale' opcode
opcode	scale_i,i,iii
 ival,imax,imin	xin
 ival	=	(ival * (imax-imin)) + imin
	xout	ival
endop


instr	1
 kMOUSE_X		chnget	"MOUSE_X"		; track mouse activity
 kMOUSE_Y		chnget	"MOUSE_Y"
 kMOUSE_DOWN_LEFT	chnget	"MOUSE_DOWN_LEFT"
 kMOUSE_DOWN_RIGHT	chnget	"MOUSE_DOWN_RIGHT"

 kSplat	init	0					; splat status: 0 = alive, 1 = splatted
 
 aMixL,aMixR	init	0				; audio mix of all buzzing sounds (initialised)
 kLiveliness	chnget	"Liveliness"			; speed with which flies move within the swarm 
 kExcursion	chnget	"Excursion"			; range (in pixels) that individual flies might go from the the centre of the swarm
 kFlySize	chnget	"FlySize"			; fly size (affects pitch of buzzing only)
 koct		scale	(kFlySize-1)/9,7,12		; scale buzzing pitch (oct format)
 kporttime	linseg	0,0.001,0.05			; portamento time rises from zero to a held value
 koct		portk	koct,kporttime			; smooth chages to buzzing frequency
 kSwarmSpeed	chnget	"SwarmSpeed"			; 

 if kSplat==0 then					; if flies are alive...
  gkPopulation	chnget	"Population"			; read changes to population. This widget is only read while flies are alive so that splatted flies don't appear or disappear while splatted.
 endif
 
 if changed(gkPopulation)==1 then			; if a change to the number of flies is made by the user. (Changes made while flies are dead will only register once the flies are unsplatted.)
  event	"i",10,0,0					; call instrument that hides all active flies. This is done so to ensure that deactivated flies are no longer visible. Flies that should be active flies will be made visible immediately after in this instrument.
 endif
 
 if trigger(kSplat,0.5,0)==1 then			; if a succesful splatting has been made...
  event	"i",20,0,0					; call the instrument that splats all active flies
 endif

 if kSplat==0 then					; if flies are living...
  kOffX		rspline	0,giPanelWidth,kSwarmSpeed,kSwarmSpeed*2		; generate variables that move the entire swarm. i.e. the swarm should not move while all flies are splatted.
  kOffY		rspline	0,giPanelHeight,kSwarmSpeed,kSwarmSpeed*2
 endif
 
 ; if a left click is made within the boundaries of the swarm it will be alternately splatted or unsplatted as appropriate
 if trigger(kMOUSE_DOWN_LEFT,0.5,0)==1 && kMOUSE_X>=(kOffX-kExcursion) && kMOUSE_X<=(kOffX+kExcursion)  && kMOUSE_Y>=(kOffY-kExcursion) && kMOUSE_Y<=(kOffY+kExcursion) then
  kSplat	=	(kSplat==1?0:1)
 endif
  
 ; macro for moving and printing an individual fly and creating its buzzing sound
 #define Fly(N)
 # 
 iN	=	$N
 if gkPopulation < $N goto SKIP						; if this fly is not active skip passed all fly generation code to the end of the instrument

 if kSplat==0 then							; if flies are living...
  kX	rspline	-kExcursion,kExcursion,kLiveliness,kLiveliness*2	; variables for fly movement within the swarm
  kY	rspline	-kExcursion,kExcursion,kLiveliness,kLiveliness*2
  kDist	rspline	0.5,1.5,0.02,0.03						; z-plane distance of the fly. Really just a bit of size modulation.
 endif

 kamp	=	((kY*0.5)/kExcursion) + 0.5				; amplitude of the buzzing sound varie according to the distance of the fly along the y-axis from the front of the swarm
 kamp	scale	kamp,1,0.1						; amplitude value scaled so that flies never audibly disappear entirely
 asig	vco2	0.01*kamp,cpsoct(koct)*semitone((kX+kY)*0.02),6		; the buzzing sound: a pulse wave
 
 kpan	=	((kX*0.5)/kExcursion) + 0.5				; buzzing sounds are panned according to their left/right position within the swarm
 aL,aR	pan2	asig,kpan						; panned
 aMixL	+=	aL							; add fly's buzz to the stereo mix
 aMixR	+=	aR

 kFlySize$N	scale	(kFlySize-1)/9,3,0.5	

 kRefreshTrig	metro	ksmps,$N/giMaxFlies				; frame rate with which flies will be graphically moved. A different trigger (each with a different phase offset) for each fly so that all chnsets don't happen at the same time and cripple performance.
 if kRefreshTrig==1 && kSplat==0 then					; if a refresh trigger has occured...
 	Smess	sprintfk	"bounds(%d,%d,%d,%d), colour(0,0,0,200)",kOffX+kX,kOffY+kY,kDist*kFlySize$N,kDist*kFlySize$N	; generate message for fly widget
 	Sident	sprintfk	"fly_ident%d", $N							; generate ident string appropriate to this fly
 	chnset Smess, Sident						; update the fly
 endif
 #
 ; expand macro for each fly needed
 $Fly(1)
 $Fly(2)
 $Fly(3)
 $Fly(4)
 $Fly(5)
 $Fly(6)
 $Fly(7)
 $Fly(8)
 $Fly(9)
 $Fly(10)
 $Fly(11)
 $Fly(12)
 $Fly(13)
 $Fly(14)
 $Fly(15)
 $Fly(16)
 $Fly(17)
 $Fly(18)
 $Fly(19)
 $Fly(20)
 $Fly(21)
 $Fly(22)
 $Fly(23)
 $Fly(24)
 $Fly(25)
 $Fly(26)
 $Fly(27)
 $Fly(28)
 $Fly(29)
 $Fly(30)
 $Fly(31)
 $Fly(32)
 $Fly(33)
 $Fly(34)
 $Fly(35)
 $Fly(36)
 $Fly(37)
 $Fly(38)
 $Fly(39)
 $Fly(40)
 $Fly(41)
 $Fly(42)
 $Fly(43)
 $Fly(44)
 $Fly(45)
 $Fly(46)
 $Fly(47)
 $Fly(48)
 $Fly(49)
 $Fly(50)
 SKIP:		; escape to here when not all flies are needed

 kenv	linseg	0,4,1	; initial rise in amplitude
 aMixL	*=	SwitchPort((1-kSplat),0.5,0.001)*kenv	; turn audio on an off according to splat status. When splatted audio is interrupted suddenly, when unsplatted audio rises more slowly.
 aMixR	*=	SwitchPort((1-kSplat),0.5,0.001)*kenv	;
	outs	aMixL,aMixR	; send audio to outputs
	clear	aMixL,aMixR	; clear audio mix variables
endin

instr	10	; hide all flies
 #define Hide_Fly(N)
 #
 chnset	"bounds(-100,-100,0,0), colour(0,0,0,0)","fly_ident$N"
 #
 $Hide_Fly(1)
 $Hide_Fly(2)
 $Hide_Fly(3)
 $Hide_Fly(4)
 $Hide_Fly(5)
 $Hide_Fly(6)
 $Hide_Fly(7)
 $Hide_Fly(8)
 $Hide_Fly(9)
 $Hide_Fly(10)
 $Hide_Fly(11)
 $Hide_Fly(12)
 $Hide_Fly(13)
 $Hide_Fly(14)
 $Hide_Fly(15)
 $Hide_Fly(16)
 $Hide_Fly(17)
 $Hide_Fly(18)
 $Hide_Fly(19)
 $Hide_Fly(20)
 $Hide_Fly(21)
 $Hide_Fly(22)
 $Hide_Fly(23)
 $Hide_Fly(24)
 $Hide_Fly(25)
 $Hide_Fly(26)
 $Hide_Fly(27)
 $Hide_Fly(28)
 $Hide_Fly(29)
 $Hide_Fly(30)
 $Hide_Fly(31)
 $Hide_Fly(32)
 $Hide_Fly(33)
 $Hide_Fly(34)
 $Hide_Fly(35)
 $Hide_Fly(36)
 $Hide_Fly(37)
 $Hide_Fly(38)
 $Hide_Fly(39)
 $Hide_Fly(40)
 $Hide_Fly(41)
 $Hide_Fly(42)
 $Hide_Fly(43)
 $Hide_Fly(44)
 $Hide_Fly(45)
 $Hide_Fly(46)
 $Hide_Fly(47)
 $Hide_Fly(48)
 $Hide_Fly(49)
 $Hide_Fly(50)
endin


instr	20	; graphically splat flies
 event_i "i",21,0,0.1	; call instrument that produces the sound effect
 
 ; a macro that contains the code to graphically splat a single fly 
 #define Splat_Fly(N)
 #
 if gkPopulation<$N goto SKIP	; if this fly is not active jump to label SKIP and don't bother splatting any more flies
 isize	random	4,20		; splat size randomised
 ialpha	random	80,180		; alpha (transparency) randomised
 ired	random	30,150		; add in a random amount of red to give it a bit of gore
 Smess	sprintf	"size(%d,%d), colour(%d,0,0,%d)",isize,isize,ired,ialpha	; create message for graphical splat
 chnset	Smess,"fly_ident$N"	; send message to the fly widget
 #
 ; expand macro for each fly that might be required 
 $Splat_Fly(1)
 $Splat_Fly(2)
 $Splat_Fly(3)
 $Splat_Fly(4)
 $Splat_Fly(5)
 $Splat_Fly(6)
 $Splat_Fly(7)
 $Splat_Fly(8)
 $Splat_Fly(9)
 $Splat_Fly(10)
 $Splat_Fly(11)
 $Splat_Fly(12)
 $Splat_Fly(13)
 $Splat_Fly(14)
 $Splat_Fly(15)
 $Splat_Fly(16)
 $Splat_Fly(17)
 $Splat_Fly(18)
 $Splat_Fly(19)
 $Splat_Fly(20)
 $Splat_Fly(21)
 $Splat_Fly(22)
 $Splat_Fly(23)
 $Splat_Fly(24)
 $Splat_Fly(25)
 $Splat_Fly(26)
 $Splat_Fly(27)
 $Splat_Fly(28)
 $Splat_Fly(29)
 $Splat_Fly(30)
 $Splat_Fly(31)
 $Splat_Fly(32)
 $Splat_Fly(33)
 $Splat_Fly(34)
 $Splat_Fly(35)
 $Splat_Fly(36)
 $Splat_Fly(37)
 $Splat_Fly(38)
 $Splat_Fly(39)
 $Splat_Fly(40)
 $Splat_Fly(41)
 $Splat_Fly(42)
 $Splat_Fly(43)
 $Splat_Fly(44)
 $Splat_Fly(45)
 $Splat_Fly(46)
 $Splat_Fly(47)
 $Splat_Fly(48)
 $Splat_Fly(49)
 $Splat_Fly(50)
 SKIP:
endin

instr	21	; splat sound effect 
 idur	scale_i	rnd(1)^2,0.15,0.08	; random duration
 p3	=	idur			; assign to p3
 imin	random	8,10			; minimum frequency for splat filter (in oct)
 anoise	dust2	10,100*gkPopulation	; some crackly noise, the density of which is dependent upon the fly population
 kcf	random	cpsoct(imin),cpsoct(imin+3)	; cutoff frequency is a random function moving to a new value every k-cycle
 anoise	moogladder	anoise,kcf,0.7	; filter the crackly noise using moogladder to give it a bit of squelch
 anoise	buthp	anoise,200		; highpass filter to remove some of the lower frequencies
 aenv	expon	1,p3,0.1		; amplitude envelope which will give the splat sound a percussive shape
 anoise	*=	aenv			; apply envelope
 	outch	1,anoise		; send this signal to the left channel
 
 anoise	dust2	10,1000			; repeat for the left channel. Doing both channels completely seperately create a nice stereo effect
 kcf	random	cpsoct(imin),cpsoct(imin+3)
 anoise	moogladder	anoise,kcf,0.7
 anoise	buthp	anoise,200
 aenv	expon	1,p3,0.1
 anoise	*=	aenv
 	outch	2,anoise
endin


instr	1001	; Print and then hide instructions
 kMOUSE_DOWN_LEFT	chnget	"MOUSE_DOWN_LEFT"	; sense left click
 kEscapeTrig	trigger	kMOUSE_DOWN_LEFT,0.5,0		; trigger of left click is pressed
 krel release						; release flag - normally 0, 1 at note release
 kalpha	transeg	0,0.2,4,255,p3-0.4,0,255,0.2,-4,-1	; colour alpha channel: fades in, holds, fades out

 if metro(ksmps)==1||krel==1 then			; peg updates or interrupt if mouse has been clicked
  Smess1	sprintfk	"bounds(%d,%d,400,17), fontcolour(0,0,0,%d)", (giPanelWidth*0.5) - 200, (giPanelHeight*0.5)-10, kalpha*(1-krel)
  chnset	Smess1,"instructions1"
 endif

 ktime		times					; note elapsed time in seconds
 if trigger(ktime,p3-0.2,0)==1||kEscapeTrig==1 then	; if time is up mouse has been clicked...
  event "i",1,0,3600*24*7	; start swarm
  turnoff
 endif

endin

</CsInstruments>  

<CsScore>
i 1001 0 3		; Instructions fade up then down (currently not working)
</CsScore>

</CsoundSynthesizer>
