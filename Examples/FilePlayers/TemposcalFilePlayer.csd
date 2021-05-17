TemposcalFilePlayer.csd
Written by Iain McCurdy, 2014.

Load a user selected sound file into a GEN 01 function table and plays it back using temposcal.

The sound file can be played back using the Play/Stop button (and the 'Transpose' and 'Speed' buttons to implement pitch abd speed changes independently.
Playing back using the MIDI keyboard will implement pitch changes based on key played.

<Cabbage>
form caption("Temposcal File Player") size(570,340), colour(0,0,0) pluginId("TScl") style("legacy")
image                       bounds(  0,  0,570,340), colour( 30, 70, 70), outlineColour("White"), shape("sharp"), line(3)

soundfiler bounds(  5,  5,560,175), channel("beg","len"), identChannel("filer1"),  colour(0, 255, 255, 255), fontColour(160, 160, 160, 255), 
label bounds(6, 4, 560, 14), text(""), align(left), colour(0,0,0,0), fontColour(200,200,200), identChannel("stringbox")

image    bounds(  0,180,570,160), colour(0,0,0,0), outlineColour("white"), line(2), shape("sharp"), plant("controls"){
filebutton bounds(  5, 10, 80, 25), text("Open","Open"), fontColour("white") channel("filename"), shape("ellipse")
checkbox   bounds(  5, 40, 95, 25), channel("PlayStop"), text("Play/Stop"), fontColour("white")

checkbox   bounds( 95, 43,100, 15), channel("lock"), text("Phase Lock"), colour("red"), fontColour("white")
checkbox   bounds( 95, 60,100, 15), channel("freeze"), text("Freeze"), colour("LightBlue"), fontColour("white")

label      bounds(105,  8, 48, 12), text("FFT Size"), fontColour("white")
combobox   bounds( 95, 20, 70, 20), channel("FFTSize"), items("32768", "16384", "8192", "4096", "2048", "1024", "512", "256", "128", "64", "32", "16", "8", "4"), value(5), fontColour("white")

rslider    bounds(175,  5, 70, 70), channel("transpose"), range(-48, 48, 0,1,1),            colour( 50, 90, 90)), trackerColour("silver"), text("Transpose"), textColour("white")
rslider    bounds(240,  5, 70, 70), channel("speed"),     range( -2,  2.00, 1),             colour( 50, 90, 90),  trackerColour("silver"), text("Speed"),     textColour("white")
rslider    bounds(305,  5, 70, 70), channel("AttTim"),    range(0, 5, 0, 0.5, 0.001),       colour( 50, 90, 90),  trackerColour("silver"), text("Att.Tim"),   textColour("white")
rslider    bounds(370,  5, 70, 70), channel("RelTim"),    range(0.01, 5, 0.05, 0.5, 0.001), colour( 50, 90, 90),  trackerColour("silver"), text("Rel.Tim"),   textColour("white")
rslider    bounds(435,  5, 70, 70), channel("MidiRef"),   range(0,127,60, 1, 1),            colour( 50, 90, 90), trackerColour("silver"),  text("MIDI Ref."), textColour("white")
rslider    bounds(500,  5, 70, 70), channel("level"),     range(  0,  3.00, 1, 0.5),        colour( 50, 90, 90),  trackerColour("silver"), text("Level"),     textColour("white")

keyboard bounds( 5, 80, 560, 75)
}
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-n -+rtmidi=NULL -M0
</CsOptions>

<CsInstruments>

;sr is set by the host
ksmps = 64
nchnls = 2
0dbfs=1

massign	0,3

gichans		init	0		; 
giReady		init	0		; flag to indicate function table readiness

giFFTSizes[]	array	32768, 16384, 8192, 4096, 2048, 1024, 512, 256, 128, 64, 32, 16, 8, 4	; an array is used to store FFT window sizes

gSfilepath	init	""

opcode FileNameFromPath,S,S		; Extract a file name (as a string) from a full path (also as a string)
 Ssrc	xin				; Read in the file path string
 icnt	strlen	Ssrc			; Get the length of the file path string
 LOOP:					; Loop back to here when checking for a backslash
 iasc	strchar Ssrc, icnt		; Read ascii value of current letter for checking
 if iasc==92 igoto ESCAPE		; If it is a backslash, escape from loop
 loop_gt	icnt,1,0,LOOP		; Loop back and decrement counter which is also used as an index into the string
 ESCAPE:				; Escape point once the backslash has been found
 Sname	strsub Ssrc, icnt+1, -1		; Create a new string of just the file name
	xout	Sname			; Send it back to the caller instrument
endop

instr	1
gkPlayStop	chnget	"PlayStop"
gkloop		chnget	"loop"
gktranspose	chnget	"transpose"
gklevel		chnget	"level"
gkspeed		chnget	"speed"
gklock		chnget	"lock"
gkfreeze	chnget	"freeze"
gkfreeze	=	1-gkfreeze
gkFFTSize	chnget	"FFTSize"
 gSfilepath	chnget	"filename"
 kNewFileTrg	changed	gSfilepath		; if a new file is loaded generate a trigger
 if kNewFileTrg==1 then				; if a new file has been loaded...
  event	"i",99,0,0				; call instrument to update sample storage function table 
 endif  

ktrig	trigger	gkPlayStop,0.5,0
schedkwhen	ktrig,0,0,2,0,-1
endin

instr	99	; load sound file
 gichans	filenchnls	gSfilepath			; derive the number of channels (mono=1,stereo=2) in the sound file
 gitableL	ftgen	1,0,0,1,gSfilepath,0,0,1
 if gichans==2 then
  gitableR	ftgen	2,0,0,1,gSfilepath,0,0,2
 endif
 giReady 	=	1					; if no string has yet been loaded giReady will be zero
 Smessage sprintfk "file(%s)", gSfilepath			; print sound file to viewer
 chnset Smessage, "filer1"

 /* WRITE FILE NAME TO GUI */
 Sname FileNameFromPath	gSfilepath				; Call UDO to extract file name from the full path
 Smessage sprintfk "text(%s)",Sname
 chnset Smessage, "stringbox"

endin

instr	2
 if gkPlayStop==0 then
  turnoff
 endif
 if giReady = 1 then				; i.e. if a file has been loaded
  iAttTim	chnget	"AttTim"		; read in amplitude envelope attack time widget
  iRelTim	chnget	"RelTim"		; read in amplitude envelope attack time widget
  if iAttTim>0 then				; 
   kenv	linsegr	0,iAttTim,1,iRelTim,0
  else								
   kenv	linsegr	1,iRelTim,0			; attack time is zero so ignore this segment of the envelope (a segment of duration zero is not permitted
  endif
  kenv	expcurve	kenv,8			; remap amplitude value with a more natural curve
  aenv	interp		kenv			; interpolate and create a-rate envelope

  kporttime	linseg	0,0.001,0.05
  ktranspose	portk	gktranspose,kporttime
  
  ktrig	changed		gkFFTSize
  if ktrig==1 then
   reinit RESTART
  endif
  RESTART:
  if gichans=1 then
   a1	temposcal	gkspeed*gkfreeze, gklevel, semitone(ktranspose), gitableL, gklock, giFFTSizes[i(gkFFTSize)-1]
  	outs	a1*aenv,a1*aenv
  elseif gichans=2 then
   a1	temposcal	gkspeed*gkfreeze, gklevel, semitone(ktranspose), gitableL, gklock, giFFTSizes[i(gkFFTSize)-1]
   a2	temposcal	gkspeed*gkfreeze, gklevel, semitone(ktranspose), gitableR, gklock, giFFTSizes[i(gkFFTSize)-1]
  	outs	a1*aenv,a2*aenv
 endif
endif

 ; print scrubber
 kscrubber	phasor	(gkspeed*gkfreeze*sr)/ftlen(gitableL)
 if(metro(20)==1) then
  Smessage sprintfk "scrubberPosition(%d)", kscrubber*ftlen(gitableL)
  chnset Smessage, "filer1"
 endif

endin




instr	3	; midi triggered instrument
 if giReady = 1 then						; i.e. if a file has been loaded
  icps	cpsmidi							; read in midi note data as cycles per second
  iamp	ampmidi	1						; read in midi velocity (as a value within the range 0 - 1)
  iMidiRef	chnget	"MidiRef"				; MIDI unison reference note
  iFrqRatio		=	icps/cpsmidinn(iMidiRef)	; derive playback speed from note played in relation to a reference note (MIDI note 60 / middle C)
 
  iAttTim	chnget	"AttTim"		; read in amplitude envelope attack time widget
  iRelTim	chnget	"RelTim"		; read in amplitude envelope attack time widget
  if iAttTim>0 then				; 
   kenv	linsegr	0,iAttTim,1,iRelTim,0
  else								
   kenv	linsegr	1,iRelTim,0			; attack time is zero so ignore this segment of the envelope (a segment of duration zero is not permitted
  endif
  kenv	expcurve	kenv,8			; remap amplitude value with a more natural curve
  aenv	interp		kenv			; interpolate and create a-rate envelope
  
  ktrig	changed		gkFFTSize
  if ktrig==1 then
   reinit RESTART
  endif
  RESTART:
  if gichans=1 then
   a1	temposcal	gkspeed*gkfreeze, gklevel*iamp, iFrqRatio, gitableL, gklock, giFFTSizes[i(gkFFTSize)-1]
  	outs	a1*aenv,a1*aenv
  elseif gichans=2 then
   a1	temposcal	gkspeed*gkfreeze, gklevel*iamp, iFrqRatio, gitableL, gklock, giFFTSizes[i(gkFFTSize)-1]
   a2	temposcal	gkspeed*gkfreeze, gklevel*iamp, iFrqRatio, gitableR, gklock, giFFTSizes[i(gkFFTSize)-1]
  	outs	a1*aenv,a2*aenv
  endif
 endif
endin

</CsInstruments>  

<CsScore>
i 1 0 10000
</CsScore>

</CsoundSynthesizer>
