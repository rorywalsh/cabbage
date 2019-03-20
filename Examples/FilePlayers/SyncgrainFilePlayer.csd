; SyncgrainFilePlayer.csd
; Written by Iain McCurdy, 2014
; 
; The internal pointer used by syncgrain to track progress through the sound file is affected by grain size and density as well as speed 
;  so on accaount of this complication a scrubber line in the waveform view is not yet implemented.

<Cabbage>
form caption("Syncgrain File Player") size(800,340), colour(0,0,0) pluginid("SGFP") style("legacy")
image                       bounds(  0,  0,800,340), colour( 90, 60, 30), outlinecolour("White"), shape("sharp"), line(3)	; main panel colouration    
soundfiler                  bounds(  5,  5,790,175), channel("beg","len"), identchannel("filer1"),  colour(0, 255, 255, 255), fontcolour(160, 160, 160, 255), 

image    bounds(  0,180,800,160), colour(0,0,0,0), outlinecolour("white"), line(0), shape("sharp"), plant("controls"){
filebutton bounds(  5, 10, 80, 25), text("Open File","Open File"), fontcolour("white") channel("filename"), shape("ellipse")
checkbox   bounds(  5, 40, 95, 25), channel("PlayStop"), text("Play/Stop"), fontcolour("white")
label      bounds(225,  4, 65, 8), text("G   R   A   I   N   S"), fontcolour("white")
rslider    bounds( 90, 15, 60, 60), channel("density"),   range( 0.5,400.00,20, 0.5),          colour( 70, 40, 10), text("Density"),     textcolour("white"), trackercolour(190,170,130), outlinecolour(100,100,100)
rslider    bounds(145, 15, 60, 60), channel("DensOS"),     range( 0, 5.00, 0),                 colour( 70, 40, 10), text("Dens.OS"),     textcolour("white"), trackercolour(190,170,130), outlinecolour(100,100,100)
rslider    bounds(200, 15, 60, 60), channel("grsize"),   range( 0.001,1.00, 0.04, 0.5),        colour( 70, 40, 10), text("Size"),        textcolour("white"), trackercolour(190,170,130), outlinecolour(100,100,100)
rslider    bounds(255, 15, 60, 60), channel("SizeOS"),   range( 0, 5.00, 0, 0.5),              colour( 70, 40, 10), text("Size OS"),     textcolour("white"), trackercolour(190,170,130), outlinecolour(100,100,100)
rslider    bounds(310, 15, 60, 60), channel("transpose"), range(-24, 24, 0,1,1),               colour( 70, 40, 10), text("Transpose"),   textcolour("white"), trackercolour(190,170,130), outlinecolour(100,100,100)
rslider    bounds(365, 15, 60, 60), channel("TransposeOS"), range(0, 12.00, 0),                colour( 70, 40, 10), text("Trans.OS"),    textcolour("white"), trackercolour(190,170,130), outlinecolour(100,100,100)
rslider    bounds(420, 15, 60, 60), channel("speed"),     range( -2.00, 2.00, 1),              colour( 70, 40, 10), text("Speed"),       textcolour("white"), trackercolour(190,170,130), outlinecolour(100,100,100)
label      bounds(495, 12, 40, 10), text("S h a p e"), fontcolour("white")
combobox   bounds(478, 22, 74, 18), channel("shape"), items("Hanning", "Perc.1", "Perc.2", "Gate", "Rev.Perc.1", "Rev.Perc.2"), value(1),fontcolour("white")
checkbox   bounds(478, 45,100, 18), channel("freeze"), text("Freeze"), colour("LightBlue"), fontcolour("white")

line       bounds(559, 10,  2, 65), colour("Grey")

label      bounds(578,  4, 86, 8), text("E   N   V   E   L   O   P   E"), fontcolour("white")
rslider    bounds(565, 15, 60, 60), channel("AttTim"),    range(0, 5, 0, 0.5, 0.001),       colour( 70, 40, 10), text("Att.Tim"),   textcolour("white"), trackercolour(190,170,130), outlinecolour(100,100,100)
rslider    bounds(620, 15, 60, 60), channel("RelTim"),    range(0.01, 5, 0.05, 0.5, 0.001), colour( 70, 40, 10), text("Rel.Tim"),   textcolour("white"), trackercolour(190,170,130), outlinecolour(100,100,100)

line       bounds(684, 10,  2, 65), colour("Grey")

label      bounds(702,  4, 76, 8), text("C   O   N   T   R   O   L"), fontcolour("white")
rslider    bounds(685, 15, 60, 60), channel("MidiRef"),   range(0,127,60, 1, 1),            colour( 70, 40, 10), text("MIDI Ref."), textcolour("white"), trackercolour(190,170,130), outlinecolour(100,100,100)
rslider    bounds(740, 15, 60, 60), channel("level"),     range(  0,  3.00, 1, 0.5),        colour( 70, 40, 10), text("Level"),     textcolour("white"), trackercolour(190,170,130), outlinecolour(100,100,100)

keyboard bounds(5, 80,790, 75)
}
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-n -+rtmidi=NULL -M0 -dm0
</CsOptions>

<CsInstruments>

sr = 48000
ksmps = 64
nchnls = 2
0dbfs=1

massign	0,3
gichans		init	0
giReady		init	0
gSfilepath	init	""

; WINDOWING FUNCTIONS USED TO DYNAMICALLY SHAPE THE GRAINS
; NUM | INIT_TIME | SIZE | GEN_ROUTINE | PARTIAL_NUM | STRENGTH | PHASE
; GRAIN ENVELOPE WINDOW FUNCTION TABLES:
giwfn1	ftgen	0,  0, 131072,  20,   2, 1 					; HANNING
giwfn2	ftgen	0,  0, 131072,  7,    0, 3072,   1, 128000,    0		; PERCUSSIVE - STRAIGHT SEGMENTS
giwfn3	ftgen	0,  0, 131072,  5, .001, 3072,   1, 128000, .001		; PERCUSSIVE - EXPONENTIAL SEGMENTS
giwfn4	ftgen	0,  0, 131072,  7,    0, 1536,   1, 128000,    1, 1536, 0	; GATE - WITH DE-CLICKING RAMP UP AND RAMP DOWN SEGMENTS
giwfn5	ftgen	0,  0, 131072,  7,    0, 128000, 1, 3072,      0		; REVERSE PERCUSSIVE - STRAIGHT SEGMENTS
giwfn6	ftgen	0,  0, 131072,  5, .001, 128000, 1, 3072,   .001		; REVERSE PERCUSSIVE - EXPONENTIAL SEGMENTS

instr	1
 gkloop		chnget	"loop"
 gkPlayStop	chnget	"PlayStop"
 gktranspose	chnget	"transpose"
 gkTransposeOS	chnget	"TransposeOS"
 gkdensity	chnget	"density"
 gkDensOS	chnget	"DensOS"
 gkgrsize	chnget	"grsize"
 gkSizeOS	chnget	"SizeOS"
 gkshape	chnget	"shape"
 gkspeed	chnget	"speed"
 gklevel	chnget	"level"
 gkfreeze	chnget	"freeze"
 gkfreeze	=	1-gkfreeze
        
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
endin

instr	2	; triggered by 'play/stop' button
 if gkPlayStop==0 then
  turnoff
 endif
 if giReady = 1 then						; i.e. if a file has been loaded
  iAttTim	chnget	"AttTim"				; read in widgets
  iRelTim	chnget	"RelTim"
  if iAttTim>0 then						; is amplitude envelope attack time is greater than zero...
   kenv	linsegr	0,iAttTim,1,iRelTim,0				; create an amplitude envelope with an attack, a sustain and a release segment (senses realtime release)
  else            
   kenv	linsegr	1,iRelTim,0					; create an amplitude envelope with a sustain and a release segment (senses realtime release)
  endif
  kenv	expcurve	kenv,8					; remap amplitude value with a more natural curve
  aenv	interp		kenv					; interpolate and create a-rate envelope
  kporttime	linseg	0,0.001,0.05				; portamento time function. (Rises quickly from zero to a held value.)
  kdensity	portk	gkdensity,kporttime			; apply portamento smoothing to changes in speed
  kprate	portk	gkspeed,kporttime
  klevel	portk	gklevel,kporttime			; apply portamento smoothing to changes in level

  kDensOS	gauss	gkDensOS
  kDensMlt	=	octave(kDensOS)
  kdensity	=	kdensity * kDensMlt
  
  ktranspose	portk	gktranspose,kporttime			; 
  kTransposeOS	gauss	gkTransposeOS
  ktranspose	=	ktranspose + kTransposeOS
  
  kSizeOS	rand	gkSizeOS
  kgrsize	=	gkgrsize * octave(kSizeOS)
  
  giolaps	=	5000
  
  ktrig	changed	gkshape
  if ktrig==1 then
   reinit	UPDATE
  endif
  UPDATE:
  
  iwfn		=	i(gkshape) + giwfn1 - 1
  if gichans==1 then						; if mono...
   a1	syncgrain	klevel, kdensity, semitone(ktranspose), kgrsize, kprate*gkfreeze, gitableL, iwfn, giolaps
 	outs	a1*aenv,a1*aenv					; send mono audio to both outputs 
  elseif gichans==2 then						; otherwise, if stereo...
   a1	syncgrain	klevel, kdensity, semitone(ktranspose), kgrsize, kprate*gkfreeze, gitableL, iwfn, giolaps
   a2	syncgrain	klevel, kdensity, semitone(ktranspose), kgrsize, kprate*gkfreeze, gitableR, iwfn, giolaps
 	outs	a1*aenv,a2*aenv					; send stereo signal to outputs
  endif
  rireturn
 endif
endin

instr	3
 icps	cpsmidi							; read in midi note data as cycles per second
 iamp	ampmidi	1						; read in midi velocity (as a value within the range 0 - 1)
 iAttTim	chnget	"AttTim"				; read in widgets
 iRelTim	chnget	"RelTim"
 iMidiRef	chnget	"MidiRef"
 iFrqRatio		=	icps/cpsmidinn(iMidiRef)	; derive playback speed from note played in relation to a reference note (MIDI note 60 / middle C)

 if giReady = 1 then						; i.e. if a file has been loaded
  iAttTim	chnget	"AttTim"				; read in widgets
  iRelTim	chnget	"RelTim"
  if iAttTim>0 then						; is amplitude envelope attack time is greater than zero...
   kenv	linsegr	0,iAttTim,1,iRelTim,0				; create an amplitude envelope with an attack, a sustain and a release segment (senses realtime release)
  else            
   kenv	linsegr	1,iRelTim,0					; create an amplitude envelope with a sustain and a release segment (senses realtime release)
  endif
  kenv	expcurve	kenv,8					; remap amplitude value with a more natural curve
  aenv	interp		kenv					; interpolate and create a-rate envelope
  kporttime	linseg	0,0.001,0.05				; portamento time function. (Rises quickly from zero to a held value.)
  kdensity	portk	gkdensity,kporttime			; apply portamento smoothing to changes in speed
  kprate	portk	gkspeed,kporttime
  klevel	portk	gklevel,kporttime			; apply portamento smoothing to changes in level

  kDensOS	gauss	gkDensOS
  kDensMlt	=	octave(kDensOS)
  kdensity	=	kdensity * kDensMlt
    
  kSizeOS	rand	gkSizeOS
  kgrsize	=	gkgrsize * octave(kSizeOS)

  giolaps	=	5000
  
  ktrig	changed	gkshape
  if ktrig==1 then
   reinit	UPDATE
  endif
  UPDATE:
  
  iwfn		=	i(gkshape) + giwfn1 - 1
  if gichans==1 then						; if mono...
   a1	syncgrain	klevel*iamp, kdensity, iFrqRatio, kgrsize, kprate*gkfreeze, gitableL, iwfn, giolaps
 	outs	a1*aenv,a1*aenv					; send mono audio to both outputs 
  elseif gichans==2 then						; otherwise, if stereo...
   a1	syncgrain	klevel*iamp, kdensity, iFrqRatio, kgrsize, kprate*gkfreeze, gitableL, iwfn, giolaps
   a2	syncgrain	klevel*iamp, kdensity, iFrqRatio, kgrsize, kprate*gkfreeze, gitableR, iwfn, giolaps
 	outs	a1*aenv,a2*aenv					; send stereo signal to outputs
  endif
  rireturn
 endif

endin

</CsInstruments>  

<CsScore>
i 1 0 [60*60*24*7]
</CsScore>

</CsoundSynthesizer>
