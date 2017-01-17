; ImageSwitches.csd
; Written by Iain McCurdy, 2014

; Uses Cabbage image widgets as switches.

; This example presents an alternative way of imagining intervals and chords spread across octaves. 
; Each row represents a complete chromatic scale and there are 9 rows therefore 9 octaves.
; 
; Circles in the grid are colourised algorithmically based upon their position within the grid.

; SOUND    - selects from a range of synthesized sounds
; OCTAVE   - shifts all notes by the the number of octaves
; SEMITONE - shifts all notes by this number of semitones
; MODIFIER - modifies the sound in some way (its precise function is determined by the 'SOUND' selected

; STRIKE   - this button will re-strike percussive sounds (if selected using 'SOUND')


<Cabbage>
form caption("Image Switches") size(500, 435), pluginID("ImSw"), colour(50,50,50); guirefresh(10)

image bounds( 40, 40,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image1")
image bounds( 75, 40,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image2")
image bounds(110, 40,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image3")
image bounds(145, 40,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image4")
image bounds(180, 40,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image5")
image bounds(215, 40,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image6")
image bounds(250, 40,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image7")
image bounds(285, 40,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image8")
image bounds(320, 40,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image9")
image bounds(355, 40,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image10")
image bounds(390, 40,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image11")
image bounds(425, 40,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image12")
image bounds( 40, 75,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image13")
image bounds( 75, 75,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image14")
image bounds(110, 75,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image15")
image bounds(145, 75,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image16")
image bounds(180, 75,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image17")
image bounds(215, 75,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image18")
image bounds(250, 75,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image19")
image bounds(285, 75,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image20")
image bounds(320, 75,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image21")
image bounds(355, 75,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image22")
image bounds(390, 75,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image23")
image bounds(425, 75,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image24")
image bounds( 40,110,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image25")
image bounds( 75,110,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image26")
image bounds(110,110,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image27")
image bounds(145,110,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image28")
image bounds(180,110,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image29")
image bounds(215,110,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image30")
image bounds(250,110,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image31")
image bounds(285,110,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image32")
image bounds(320,110,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image33")
image bounds(355,110,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image34")
image bounds(390,110,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image35")
image bounds(425,110,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image36")
image bounds( 40,145,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image37")
image bounds( 75,145,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image38")
image bounds(110,145,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image39")
image bounds(145,145,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image40")
image bounds(180,145,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image41")
image bounds(215,145,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image42")
image bounds(250,145,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image43")
image bounds(285,145,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image44")
image bounds(320,145,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image45")
image bounds(355,145,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image46")
image bounds(390,145,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image47")
image bounds(425,145,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image48")
image bounds( 40,180,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image49")
image bounds( 75,180,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image50")
image bounds(110,180,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image51")
image bounds(145,180,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image52")
image bounds(180,180,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image53")
image bounds(215,180,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image54")
image bounds(250,180,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image55")
image bounds(285,180,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image56")
image bounds(320,180,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image57")
image bounds(355,180,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image58")
image bounds(390,180,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image59")
image bounds(425,180,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image60")
image bounds( 40,215,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image61")
image bounds( 75,215,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image62")
image bounds(110,215,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image63")
image bounds(145,215,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image64")
image bounds(180,215,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image65")
image bounds(215,215,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image66")
image bounds(250,215,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image67")
image bounds(285,215,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image68")
image bounds(320,215,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image69")
image bounds(355,215,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image70")
image bounds(390,215,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image71")
image bounds(425,215,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image72")
image bounds( 40,250,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image73")
image bounds( 75,250,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image74")
image bounds(110,250,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image75")
image bounds(145,250,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image76")
image bounds(180,250,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image77")
image bounds(215,250,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image78")
image bounds(250,250,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image79")
image bounds(285,250,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image80")
image bounds(320,250,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image81")
image bounds(355,250,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image82")
image bounds(390,250,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image83")
image bounds(425,250,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image84")
image bounds( 40,285,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image85")
image bounds( 75,285,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image86")
image bounds(110,285,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image87")
image bounds(145,285,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image88")
image bounds(180,285,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image89")
image bounds(215,285,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image90")
image bounds(250,285,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image91")
image bounds(285,285,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image92")
image bounds(320,285,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image93")
image bounds(355,285,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image94")
image bounds(390,285,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image95")
image bounds(425,285,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image96")
image bounds( 40,320,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image97")
image bounds( 75,320,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image98")
image bounds(110,320,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image99")
image bounds(145,320,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image100")
image bounds(180,320,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image101")
image bounds(215,320,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image102")
image bounds(250,320,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image103")
image bounds(285,320,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image104")
image bounds(320,320,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image105")
image bounds(355,320,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image106")
image bounds(390,320,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image107")
image bounds(425,320,30,30), colour(0,0,0,0), shape("rounded"), outlinecolour("white"), line(0), identchannel("image108")

; OCTAVE NUMBERING (ROWS)
label bounds(20, 45, 15, 20), text("0")
label bounds(20, 80, 15, 20), text("1")
label bounds(20,115, 15, 20), text("2")
label bounds(20,150, 15, 20), text("3")
label bounds(20,185, 15, 20), text("4")
label bounds(20,220, 15, 20), text("5")
label bounds(20,255, 15, 20), text("6")
label bounds(20,290, 15, 20), text("7")
label bounds(20,325, 15, 20), text("8")

; NOTE NAME LABELLING (COLUMNS)
label bounds( 40, 18, 30, 20), text("C")
label bounds( 75, 18, 30, 20), text("C#")
label bounds(110, 18, 30, 20), text("D")
label bounds(145, 18, 30, 20), text("D#")
label bounds(180, 18, 30, 20), text("E")
label bounds(215, 18, 30, 20), text("F")
label bounds(250, 18, 30, 20), text("F#")
label bounds(285, 18, 30, 20), text("G")
label bounds(320, 18, 30, 20), text("G#")
label bounds(355, 18, 30, 20), text("A")
label bounds(390, 18, 30, 20), text("A#")
label bounds(425, 18, 30, 20), text("B")

; SOUND ATTRIBUTES
rslider  bounds( 25,365, 60, 60), text("SOUND"),           channel("SoundSelect"), range(1,11,2,1,1),    colour("DarkGrey"),  trackercolour("white"), outlinecolour(100,100,100)
rslider  bounds( 85,365, 60, 60), text("LEVEL"),           channel("Level"),       range(0,1.00,0.9),    colour("DarkGrey"),  trackercolour("white"), outlinecolour(100,100,100)
rslider  bounds(145,365, 60, 60), text("OCTAVE"),          channel("Octave"),      range(-3, 3,0,1,1),   colour("DarkGrey"),  trackercolour("white"), outlinecolour(100,100,100)
rslider  bounds(205,365, 60, 60), text("SEMITONE"),        channel("Semitone"),    range(-11, 11,0,1,1), colour("DarkGrey"),  trackercolour("white"), outlinecolour(100,100,100)
rslider  bounds(265,365, 60, 60), text("MODIFY"),          channel("Modifier"),    range(0, 1.00,0),     colour("DarkGrey"),  trackercolour("white"), outlinecolour(100,100,100)
rslider  bounds(325,365, 60, 60), text("REVERB"),          channel("Reverb"),      range(0, 1.00,0.5),   colour("DarkGrey"),  trackercolour("white"), outlinecolour(100,100,100)
button   bounds(390,365, 70, 25), text("STRIKE","STRIKE"), channel("Strike"), value(0), latched(0)

label bounds(2,424,100, 10), text("Iain McCurdy |2013|"), FontColour(110,110,110), align("left")
</Cabbage>                                                                                                          
                                                                                                                    
<CsoundSynthesizer>                                                                                                 
                                                                                                                    
<CsOptions>                                                                                                         
-d -n
</CsOptions>

<CsInstruments>

sr 		= 	44100	;SAMPLE RATE
ksmps 		= 	32	;NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls 		= 	2	;NUMBER OF CHANNELS (2=STEREO)
0dbfs		=	1

opcode	UnlatchedToLatched,k,k			; THIS UDO LATCHES AN UNLATCHED SWITCH.
	kTrigIn		xin			; READ IN INPUT ARGUMENT 
	kTrigOut	init	0		; INITIALISE OUTPUT TRIGGER VALUE
	kcross		trigger	kTrigIn,0,2	; IF INPUT TRIGGER CHANGES FROM ZERO OR BACK TO ZERO SWITCH THE OUTPUT TRIGGER VALUE ACCORDING TO ITS CURRENT VALUE
	if kcross==1 then			; IF INPUT TRIGGER HAS CHANGED FROM ZERO OR BACK TO ZERO...
	 kTrigOut	=	abs(kTrigOut-1)	; FLIP THE OUTPUT TRIGGER FROM 0 TO 1 OR FROM 1 TO 0 (DEPENDING UPON WHAT ITS CURRENT VALUE IS)
	endif					; END OF CONDITIONAL BRANCH
			xout	kTrigOut	; SEND OUTPUT TRIGGER BACK TO CALLER INSTRUMENT
endop

opcode	scale_i,i,iii					; UDO for an i-rate version of the 'scale' opcode
 ival,imax,imin	xin
 ival	=	(ival * (imax-imin)) + imin
	xout	ival
endop


instr	1
 ;; Read in data from the mouse within the Cabbage GUI window
 kMOUSE_X		chnget	"MOUSE_X"
 kMOUSE_Y		chnget	"MOUSE_Y"
 kMOUSE_DOWN_LEFT	chnget	"MOUSE_DOWN_LEFT"
 kMOUSE_DOWN_MIDDLE	chnget	"MOUSE_DOWN_MIDDLE"
 kMOUSE_DOWN_RIGHT	chnget	"MOUSE_DOWN_RIGHT"

 kClick	trigger	kMOUSE_DOWN_LEFT,0.5,2
 
#define	SENSE_INSIDE(N'X'Y'X_SIZE'Y_SIZE)
#
 k$N	init	0	; On / Off value for this circle

 ; SENSE INSIDE
 if kMOUSE_X>=$X && kMOUSE_X<=($X+$X_SIZE) && kMOUSE_Y>=$Y && kMOUSE_Y<=($Y+$Y_SIZE) then
  kInside$N	=	1	; If mouse pointer is within this square indicator flag (kInside$N) is '1'
 else
  kInside$N	=	0	; If mouse pointer is outside this square indicator flag (kInside$N) is '0'
 endif
 

 ; TURN LIGHT ON OR OFF
 if kInside$N==1&&kClick=1 then
  k$N	UnlatchedToLatched	kMOUSE_DOWN_LEFT

  							; (so that clicking can toggle circles on and off)

  if changed(k$N)==1 then						; If switch has been changed...
   if k$N==1 then							; If switch has been turned on...
    ; Calculate RED / GREEN / BLUE values for this button based on its location in the grid
    iR$N	mirror	$Y,0,255
    iG$N	mirror	$X+100,0,255
    iB$N	mirror	$Y+100,0,255
    
    Smessage sprintfk "colour(%d,%d,%d,%d)", iR$N,iG$N,iB$N, 255	; Colour note to its 'on' state.
    event	"i",2+($N*0.001),0,-1,$N				; play a note

   else									; Otherwise (note has been turned off)...
    Smessage sprintfk "colour(%d,%d,%d,%d)", 0, 0, 0, 255		; Remove colour from note.
    turnoff2	2+($N*0.001),4,1					; turnoff a note
   endif
   chnset Smessage, "image$N"
  endif
 endif
 
#

   $SENSE_INSIDE(1' 40' 40'30'30)
   $SENSE_INSIDE(2' 75' 40'30'30)
   $SENSE_INSIDE(3'110' 40'30'30)
   $SENSE_INSIDE(4'145' 40'30'30)
   $SENSE_INSIDE(5'180' 40'30'30)
   $SENSE_INSIDE(6'215' 40'30'30)
   $SENSE_INSIDE(7'250' 40'30'30)
   $SENSE_INSIDE(8'285' 40'30'30)
   $SENSE_INSIDE(9'320' 40'30'30)
  $SENSE_INSIDE(10'355' 40'30'30)
  $SENSE_INSIDE(11'390' 40'30'30)
  $SENSE_INSIDE(12'425' 40'30'30)
  $SENSE_INSIDE(13' 40' 75'30'30)
  $SENSE_INSIDE(14' 75' 75'30'30)
  $SENSE_INSIDE(15'110' 75'30'30)
  $SENSE_INSIDE(16'145' 75'30'30)
  $SENSE_INSIDE(17'180' 75'30'30)
  $SENSE_INSIDE(18'215' 75'30'30)
  $SENSE_INSIDE(19'250' 75'30'30)
  $SENSE_INSIDE(20'285' 75'30'30)
  $SENSE_INSIDE(21'320' 75'30'30)
  $SENSE_INSIDE(22'355' 75'30'30)
  $SENSE_INSIDE(23'390' 75'30'30)
  $SENSE_INSIDE(24'425' 75'30'30)
  $SENSE_INSIDE(25' 40'110'30'30)
  $SENSE_INSIDE(26' 75'110'30'30)
  $SENSE_INSIDE(27'110'110'30'30)
  $SENSE_INSIDE(28'145'110'30'30)
  $SENSE_INSIDE(29'180'110'30'30)
  $SENSE_INSIDE(30'215'110'30'30)
  $SENSE_INSIDE(31'250'110'30'30)
  $SENSE_INSIDE(32'285'110'30'30)
  $SENSE_INSIDE(33'320'110'30'30)
  $SENSE_INSIDE(34'355'110'30'30)
  $SENSE_INSIDE(35'390'110'30'30)
  $SENSE_INSIDE(36'425'110'30'30)
  $SENSE_INSIDE(37' 40'145'30'30)
  $SENSE_INSIDE(38' 75'145'30'30)
  $SENSE_INSIDE(39'110'145'30'30)
  $SENSE_INSIDE(40'145'145'30'30)
  $SENSE_INSIDE(41'180'145'30'30)
  $SENSE_INSIDE(42'215'145'30'30)
  $SENSE_INSIDE(43'250'145'30'30)
  $SENSE_INSIDE(44'285'145'30'30)
  $SENSE_INSIDE(45'320'145'30'30)
  $SENSE_INSIDE(46'355'145'30'30)
  $SENSE_INSIDE(47'390'145'30'30)
  $SENSE_INSIDE(48'425'145'30'30)
  $SENSE_INSIDE(49' 40'180'30'30)
  $SENSE_INSIDE(50' 75'180'30'30)
  $SENSE_INSIDE(51'110'180'30'30)
  $SENSE_INSIDE(52'145'180'30'30)
  $SENSE_INSIDE(53'180'180'30'30)
  $SENSE_INSIDE(54'215'180'30'30)
  $SENSE_INSIDE(55'250'180'30'30)
  $SENSE_INSIDE(56'285'180'30'30)
  $SENSE_INSIDE(57'320'180'30'30)
  $SENSE_INSIDE(58'355'180'30'30)
  $SENSE_INSIDE(59'390'180'30'30)
  $SENSE_INSIDE(60'425'180'30'30)
  $SENSE_INSIDE(61' 40'215'30'30)
  $SENSE_INSIDE(62' 75'215'30'30)
  $SENSE_INSIDE(63'110'215'30'30)
  $SENSE_INSIDE(64'145'215'30'30)
  $SENSE_INSIDE(65'180'215'30'30)
  $SENSE_INSIDE(66'215'215'30'30)
  $SENSE_INSIDE(67'250'215'30'30)
  $SENSE_INSIDE(68'285'215'30'30)
  $SENSE_INSIDE(69'320'215'30'30)
  $SENSE_INSIDE(70'355'215'30'30)
  $SENSE_INSIDE(71'390'215'30'30)
  $SENSE_INSIDE(72'425'215'30'30)
  $SENSE_INSIDE(73' 40'250'30'30)
  $SENSE_INSIDE(74' 75'250'30'30)
  $SENSE_INSIDE(75'110'250'30'30)
  $SENSE_INSIDE(76'145'250'30'30)
  $SENSE_INSIDE(77'180'250'30'30)
  $SENSE_INSIDE(78'215'250'30'30)
  $SENSE_INSIDE(79'250'250'30'30)
  $SENSE_INSIDE(80'285'250'30'30)
  $SENSE_INSIDE(81'320'250'30'30)
  $SENSE_INSIDE(82'355'250'30'30)
  $SENSE_INSIDE(83'390'250'30'30)
  $SENSE_INSIDE(84'425'250'30'30)
  $SENSE_INSIDE(85' 40'285'30'30)
  $SENSE_INSIDE(86' 75'285'30'30)
  $SENSE_INSIDE(87'110'285'30'30)
  $SENSE_INSIDE(88'145'285'30'30)
  $SENSE_INSIDE(89'180'285'30'30)
  $SENSE_INSIDE(90'215'285'30'30)
  $SENSE_INSIDE(91'250'285'30'30)
  $SENSE_INSIDE(92'285'285'30'30)
  $SENSE_INSIDE(93'320'285'30'30)
  $SENSE_INSIDE(94'355'285'30'30)
  $SENSE_INSIDE(95'390'285'30'30)
  $SENSE_INSIDE(96'425'285'30'30)
  $SENSE_INSIDE(97' 40'320'30'30)
  $SENSE_INSIDE(98' 75'320'30'30)
  $SENSE_INSIDE(99'110'320'30'30)
 $SENSE_INSIDE(100'145'320'30'30)
 $SENSE_INSIDE(101'180'320'30'30)
 $SENSE_INSIDE(102'215'320'30'30)
 $SENSE_INSIDE(103'250'320'30'30)
 $SENSE_INSIDE(104'285'320'30'30)
 $SENSE_INSIDE(105'320'320'30'30)
 $SENSE_INSIDE(106'355'320'30'30)
 $SENSE_INSIDE(107'390'320'30'30)
 $SENSE_INSIDE(108'425'320'30'30)
                  
endin             






;; UDOS

opcode	Oscil1a,a,iii					; an oscillator that plays a single cycle of an audio waveform at a-rate
 iamp,ifrq,ifn	xin
 aptr	line	0,1/ifrq,1
 asig	tablei	aptr,ifn,1
 aenv	linseg	1,1/ifrq,1,0.001,0
	xout	asig*iamp*aenv
endop

opcode	uniform_wooden_bar, a, akk
	ain, kbasfrq, kq	xin
	amix	init	0
#define	MODE_PARTIAL(FRQ)
	#
	kfrq	=	kbasfrq*$FRQ
	if (sr/kfrq)>=$M_PI then
	 asig	mode	ain, kfrq, kq
	 amix	=	amix + asig
	endif
	#
	$MODE_PARTIAL(1    )
	$MODE_PARTIAL(2.572)
	$MODE_PARTIAL(4.644)
	$MODE_PARTIAL(6.984)
	$MODE_PARTIAL(9.723)
	$MODE_PARTIAL(12   )
		xout	amix/6
		clear	amix
endop

opcode	red_cedar_wood_plate, a, akk
	ain, kbasfrq, kq	xin
	amix	init	0
#define	MODE_PARTIAL(FRQ)
	#
	kfrq	=	kbasfrq*$FRQ
	if (sr/kfrq)>=$M_PI then
	 asig	mode	ain, kfrq, kq
	 amix	=	amix + asig
	endif
	#
	$MODE_PARTIAL(1   )
	$MODE_PARTIAL(1.47)
	$MODE_PARTIAL(2.09)
	$MODE_PARTIAL(2.56)
		xout	amix/4
		clear	amix
endop




; Global variables and function tables
giImp	ftgen	0,0,4097,9,0.5,1,0			; shape for the hammer inpulse
gaSendL,gaSendR	init	0
gisine	ftgen	0,0,4096,10,1
gicos	ftgen	0,0,131072,11,1
gihann	ftgen	0, 0, 8192,  20, 2, 1			; Hanning window
giSoftSaw	ftgen	0,0,131072,10,1,1/8,1/16,1/32,1/64,1/128,1/256




instr	2	; produce a sound
 kporttime	linseg	0,0.001,1
 
 iCF_Time	=	0.05			; cross-fade time (when changing 'SOUND' selector
 
 kSoundSelect	chnget	"SoundSelect"
 kSoundSelect	init	2
 kSoundSelectD	delayk	 kSoundSelect,iCF_Time	; delayed 'SOUND' selector control

 kOctave	chnget	"Octave" 
 kSemitone	chnget	"Semitone" 
 kStrike	chnget	"Strike"
 kChangeTrans	changed	kOctave,kSemitone,kSoundSelect	; Impulse generated when transposition is changed (used to retrigger percussive sounds)
 kStrike	trigger	kStrike+kChangeTrans, 0.5, 0	; Impulse generated when 'STRIKE' button is clicked
 kModifier	chnget	"Modifier"

 kModifier	portk	kModifier,0.1*kporttime
 kOctave	portk	kOctave,0.002*kporttime
 kSemitone	portk	kSemitone,0.002*kporttime

 ; A MECHANISM IS IMPLEMENTED TO PREVENT CLICKS WHEN CHANGING 'SOUND'
 kEnv2	init	0			;
 ksmooth	init	1		;
 if changed(kSoundSelect)==1 then	; If 'SOUND' switch is changed...
  ksmooth	=	0		; ...smoothing mode set to '0' (ramp amplitude down)
  reinit RAMP_DOWN
 elseif changed(kSoundSelectD)==1 then	; Shortly after 'SOUND' switch has been changed...
  ksmooth	=	1		; ...smoothing mode set to '1' (ramp amplitude back up and hold)
  reinit NORMAL_ENV
 endif
 
 RAMP_DOWN:
 kEnv1	linseg	i(kEnv2),iCF_Time,0	; Ramp amplitude down to prevent clicks when sound is changed
 rireturn				
 
 NORMAL_ENV:				; Ramp amplitude up and hold. Release smoothly if note is released.
 kEnv2	linsegr	0,iCF_Time,1,1,0
 rireturn
 
 kEnv	=	(ksmooth==0?kEnv1:kEnv2); Choose appropriate envelope based on mode: crossfade down / crossfade up. 
 
 
 
 kdtn	jspline	10,0.05,0.2		; A detuning function - producing a slow instability in pitch. Range roughly -10 to +10 cents.

 
 if kSoundSelectD==1 then		; sine modifying to buzz
  kmul	scale	kModifier,0.8,0
  asig	gbuzz	0.04*kEnv,cpsmidinn(p4+11)*cent(kdtn)*octave(kOctave)*semitone(kSemitone),80, 1, kmul, gicos

 elseif kSoundSelectD==2 then	; undulating buzz
  kmul	rspline	0.1,0.8,0.1,0.4
  kmul	limit	kmul+kModifier,0,0.9
  asig	gbuzz	0.04*kEnv,cpsmidinn(p4+11)*cent(kdtn)*octave(kOctave)*semitone(kSemitone),20, 1, kmul^2, gicos


 elseif kSoundSelectD==3 then	; hsboscil
  ktone		=	0
  kbrite	rspline	-2,2,0.1,0.2
  kocts		scale	kModifier,6,1
  if changed(kOctave+kSemitone+kocts)==1 then
   reinit RESTART_HSBOSCIL
  endif
  RESTART_HSBOSCIL:
  asig	hsboscil	0.04*kEnv, ktone+(kdtn/1000), kbrite, cpsmidinn(p4+11)*octave(i(kOctave))*semitone(i(kSemitone)), gisine, gihann, i(kocts), -1
  rireturn


 elseif kSoundSelectD==4 then	; whistle (filtered noise)
  ;asig	pinkish	0.04*kEnv
  asig	noise	0.04*kEnv,0.9
  kbw	scale	kModifier,0.1,0.0005
  asig	reson	asig,cpsmidinn(p4+11)*cent(kdtn)*octave(kOctave)*semitone(kSemitone), cpsmidinn(p4+11)*cent(kdtn)*kbw*octave(kOctave)*semitone(kSemitone), 2
  ;asig	buthp	asig,cpsmidinn(p4+11)*cent(kdtn)*octave(kOctave)*semitone(kSemitone)

 elseif kSoundSelectD==5 then	; wgflute
  kjet	scale	kModifier,1.5,0.28	;0.28
  kngain	=	0.05
  kvibf	=	0.2
  kvamp	=	0.05
  asig	wgflute 0.04*kEnv, cpsmidinn(p4+12)*cent(kdtn)*octave(kOctave)*semitone(kSemitone-1.55), kjet, 0.1, 0.1, kngain, kvibf, kvamp, gisine


 elseif kSoundSelectD==6 then	; wgclar
  kstiff	scale	kModifier,1,-0.3	;-0.3
  kngain	scale	kModifier,0.9,0.05		;0.05
  kvibf	=	0
  kvamp	=	0
  asig	wgclar	0.1, cpsmidinn(p4+12)*cent(kdtn)*octave(kOctave)*semitone(kSemitone-1), kstiff, 0.1, 0.1, kngain, kvibf, kvamp, gisine, 1
  asig	dcblock2	asig*kEnv


 elseif kSoundSelectD==7 then	; wgbow
  kpres	scale	kModifier,10,3	;3
  krat	scale	kModifier,0.8,0.127236	;	0.127236
  kvibf	=	rnd(1)+randomi(1,4,0.2,1)
  kvamp	=	0.001
  asig	wgbow	0.1*kEnv, cpsmidinn(p4+12)*cent(kdtn)*octave(kOctave)*semitone(kSemitone-1.1), kpres, krat, kvibf, kvamp, gisine, 1


 elseif kSoundSelectD==8 then 	; struck wooden bar
  if kStrike==1 then
   reinit UNIFORM_WOODEN_BAR
  endif
  UNIFORM_WOODEN_BAR:
  aImpls	Oscil1a	0.04,cpsmidinn(p4+11)*octave(i(kOctave))*semitone(i(kSemitone)),giImp
  kres	scale	kModifier,2000,250
  asig	uniform_wooden_bar	aImpls,cpsmidinn(p4+11)*octave(kOctave)*semitone(kSemitone),kres 
  asig	delay	asig,rnd(0.05)+0.0001
  rireturn
  kRel	linsegr	1,0.1,0
  asig	=	asig * kRel

 elseif kSoundSelectD==9 then ; struck cedar wood
  if kStrike==1 then
   reinit RED_CEDAR_WOOD_PLATE
  endif
  RED_CEDAR_WOOD_PLATE:
  aImpls	Oscil1a	0.04,cpsmidinn(p4+11)*octave(i(kOctave))*semitone(i(kSemitone)),giImp
  kres	scale	kModifier,2000,250
  asig	red_cedar_wood_plate	aImpls,cpsmidinn(p4+11)*octave(kOctave)*semitone(kSemitone),kres 
  asig	delay	asig,rnd(0.05)+0.0001
  rireturn
  kRel	linsegr	1,0.1,0
  asig	=	asig * kRel


 elseif kSoundSelectD==10 then	; plucked string
  if kStrike==1 then
   reinit WGUIDE2
  endif
  WGUIDE2:
  aImpls	Oscil1a	0.3,300,giImp
  rireturn
  kLPF		scale	kModifier, 20000,3000
  asig		wguide1	aImpls,cpsmidinn(p4+11)*octave(kOctave)*semitone(kSemitone+0.5), kLPF, 0.99995  
  kdltim	trandom	kStrike,0.001,0.0501
  asig		vdelay	asig,kdltim*1000,0.0501*1000
  asig		dcblock2	asig
  kRel	linsegr	1,0.1,0
  asig	=	asig * kRel


 elseif kSoundSelectD==11 then	; FM marimba
  idur	scale_i	(p4/127)^0.5,0.2,10
  kRat	scale	kModifier,5.12,2.15
  print	idur
  if kStrike==1 then
   reinit FM_MARIMBA
  endif
  FM_MARIMBA:
  aAEnv	transeg	1,idur,-8,0
  kIEnv	transeg	1,idur,-100,0
  asig	foscil aAEnv*0.1, cpsmidinn(p4+11)*octave(kOctave)*semitone(kSemitone), 1, kRat, 0.55*kIEnv, gisine
  asig	delay	asig,rnd(0.05)+0.0001
  rireturn
 	outs	asig,asig

 endif

 kLevel	chnget	"Level"
 kLevel	portk	kLevel,0.05*kporttime
 asig	=	asig * kLevel


 iSide	random	0,1
 iDelTim	random	0.001,0.04
 if iSide>0.5 then
  aL	delay	asig,iDelTim
  aR	=	asig
 else
  aR	delay	asig,iDelTim
  aL	=	asig
 endif

	outs aL, aR

 kReverb	chnget	"Reverb"
 gaSendL	=	gaSendL + (aL*kReverb)
 gaSendR	=	gaSendR + (aR*kReverb)
 
endin



instr	99	; Reverb
 
 aL,aR	reverbsc	gaSendL,gaSendR,0.83,6000
 	outs		aL,aR
 	clear		gaSendL,gaSendR
endin

</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
i 99 0 [3600*24*7]
</CsScore>


</CsoundSynthesizer>
