; SoundwarpFilePlayer.csd
; Written by Iain McCurdy, 2014

; Player can also be activated by using right-click and drag upon the waveform display panel.
;  In this mode, X position equates to pointer position and Y position equates to amplitude (level) and transposition.
;   (Transposition range when using mouse clicking-and-dragging is controlled using the 'Transposition' knob. Therefore if 'Transposition' = zero, no transposition occurs.)

<Cabbage>
form caption("Soundwarp File Player") size(835,340), colour(0,0,0) pluginID("SWPl"), guirefresh(32)
image                       bounds(  0,  0,835,340), colour( 30, 90, 60), outlinecolour("White"), line(3), shape("sharp")	; main panel colouration    

soundfiler bounds(  5,  5,825,175), channel("beg","len"), channel("pos1","end1"), identchannel("filer1"),  colour(0, 255, 255, 255), fontcolour(160, 160, 160, 255), 

filebutton bounds(  5,185, 80, 22), text("Open File","Open File"), fontcolour("white") channel("filename"), shape("ellipse")
checkbox   bounds(  5,210, 95, 17), channel("PlayStop"), text("Play/Stop"), fontcolour("white")
label      bounds(  5,228,115, 10), text("[or right-click-and-drag]"), fontcolour("white")
checkbox   bounds(  5,240,100, 17), channel("freeze"), text("Freeze"), colour("LightBlue"), fontcolour("white")

label      bounds(245,184,180, 8), text("G   R   A   I   N   S"), fontcolour("white")
rslider    bounds(120,195, 60, 60), channel("overlap"),     range( 1, 100, 20, 1,1),            colour( 50,110, 80), text("Overlaps"),     textcolour("white"), trackercolour(150,210,180)
rslider    bounds(175,195, 60, 60), channel("grsize"),      range( 1, 40000, 800, 0.5,1),       colour( 50,110, 80), text("Size"),         textcolour("white"), trackercolour(150,210,180)
rslider    bounds(230,195, 60, 60), channel("grsizeOS"),    range( 0, 2.00,   0.5,  0.5),       colour( 50,110, 80), text("Size OS"),      textcolour("white"), trackercolour(150,210,180)
rslider    bounds(285,195, 60, 60), channel("transpose"),   range(-48, 48, 0,1,0.001),          colour( 50,110, 80), text("Transpose"),    textcolour("white"), trackercolour(150,210,180)

label      bounds(367,198, 35, 10), text("M o d e"), fontcolour("white")
combobox   bounds(348,208, 74, 17), channel("mode"), items("Speed", "Pointer", "Region"), value(1), fontcolour("white")
label      bounds(364,227, 40, 10), text("S h a p e"), fontcolour("white")
combobox   bounds(348,237, 74, 17), channel("shape"), items("Hanning", "Perc.1", "Perc.2", "Gate", "Rev.Perc.1", "Rev.Perc.2"), value(1), fontcolour("white")

rslider    bounds(425,195, 60, 60), channel("speed"),       range( 0.00, 5.00, 1,0.5,0.001),           colour( 50,110, 80), text("Speed"),   textcolour("white"), trackercolour(150,210,180), visible(1), identchannel("SpeedID")
rslider    bounds(425,195, 60, 60), channel("ptr"),         range(     0,1.00, 0.5,1,0.001),           colour( 50,110, 80), text("Pointer"), textcolour("white"), trackercolour(150,210,180), visible(0), identchannel("PtrID")
rslider    bounds(480,195, 60, 60), channel("ptrOS"),       range(     0, 1.000, 0, 0.5, 0.001),       colour( 50,110, 80), text("Ptr.OS"),  textcolour("white"), trackercolour(150,210,180), visible(0), identchannel("PtrOSID")
rslider    bounds(535,195, 60, 60), channel("port"),        range(     0,30.000,0.01, 0.5,0.001),      colour( 50,110, 80), text("Port."),   textcolour("white"), trackercolour(150,210,180), visible(0), identchannel("PortID")
rslider    bounds(535,195, 60, 60), channel("inskip"),      range(     0, 1.00, 0),                    colour( 50,110, 80), text("inskip"),  textcolour("white"), trackercolour(150,210,180), visible(1), identchannel("inskipID")

line       bounds(595,190,  2, 65), colour("Grey")

label      bounds(595,184,120, 8), text("E   N   V   E   L   O   P   E"), fontcolour("white")
rslider    bounds(598,195, 60, 60), channel("AttTim"),    range(0, 5.00, 0.01, 0.5, 0.001),  colour( 50,110, 80), text("Att.Tim"), textcolour("white"), trackercolour(150,210,180)
rslider    bounds(650,195, 60, 60), channel("RelTim"),    range(0.01, 5, 0.05, 0.5, 0.001),  colour( 50,110, 80), text("Rel.Tim"), textcolour("white"), trackercolour(150,210,180)

line       bounds(711,190,  2, 65), colour("Grey")

label      bounds(732,184, 80, 8), text("C   O   N   T   R   O   L"), fontcolour("white")
rslider    bounds(715,195, 60, 60), channel("MidiRef"),   range(0,127,60, 1, 1),            colour( 50,110, 80), text("MIDI Ref."), textcolour("white"), trackercolour(150,210,180)
rslider    bounds(770,195, 60, 60), channel("level"),     range(  0,  1.00, 0.4, 0.5),      colour( 50,110, 80), text("Level"),     textcolour("white"), trackercolour(150,210,180)

keyboard   bounds(5,260,825, 75)
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
giFileLen	init	0
giReady		init	0
gSfilepath	init	""

; WINDOWING FUNCTIONS USED TO DYNAMICALLY SHAPE THE GRAINS
; NUM | INIT_TIME | SIZE | GEN_ROUTINE | PARTIAL_NUM | STRENGTH | PHASE
; GRAIN ENVELOPE WINDOW FUNCTION TABLES:
;giwfn1	ftgen	0,  0, 131072,  20,   1, 1 					; HANNING
giwfn1	ftgen	0,  0, 131072,  9,    0.5, 1,0 					; HALF SINE
giwfn2	ftgen	0,  0, 131072,  7,    0, 3072,   1, 128000,    0		; PERCUSSIVE - STRAIGHT SEGMENTS
giwfn3	ftgen	0,  0, 131072,  5, .001, 3072,   1, 128000, .001		; PERCUSSIVE - EXPONENTIAL SEGMENTS
giwfn4	ftgen	0,  0, 131072,  7,    0, 1536,   1, 128000,    1, 1536, 0	; GATE - WITH DE-CLICKING RAMP UP AND RAMP DOWN SEGMENTS
giwfn5	ftgen	0,  0, 131072,  7,    0, 128000, 1, 3072,      0		; REVERSE PERCUSSIVE - STRAIGHT SEGMENTS
giwfn6	ftgen	0,  0, 131072,  5, .001, 128000, 1, 3072,   .001		; REVERSE PERCUSSIVE - EXPONENTIAL SEGMENTS

instr	1
 gkloop		chnget	"loop"
 gkPlayStop	chnget	"PlayStop"
 gkfreeze	chnget	"freeze"
 gkfreeze	=	1-gkfreeze
 gktranspose	chnget	"transpose"
 gkoverlap	chnget	"overlap"
 gkgrsize	chnget	"grsize"
 gkgrsizeOS	chnget	"grsizeOS"
 gkshape	chnget	"shape"
 gkmode		chnget	"mode"
 gkmode		init	1
 gkspeed	chnget	"speed"
 gkptrOS	chnget	"ptrOS"
 gkport		chnget	"port"
 kporttime	linseg	0,0.001,1
 gkinskip	chnget	"inskip"
 gklevel	chnget	"level"
 gkLoopStart	chnget	"beg"		;from soundfiler
 gkLoopEnd	chnget	"len"			;from soundfiler
 
 
 ; SHOW OR HIDE WIDGETS -------------------------------------
 kchange	changed	gkmode
 if(kchange==1) then
	if gkmode==1 then
	 chnset "visible(1)", "SpeedID"
	 chnset "visible(0)", "PtrOSID"
	 chnset "visible(0)", "PtrID"
	 chnset "visible(0)", "PortID"
	 chnset "visible(1)", "inskipID"
	endif
	if gkmode==2 then
	 chnset "visible(0)", "SpeedID"
	 chnset "visible(1)", "PtrOSID"
	 chnset "visible(1)", "PtrID"
	 chnset "visible(1)", "PortID"
	 chnset "visible(0)", "inskipID"
	endif
	if gkmode==3 then
	 chnset "visible(0)", "SpeedID"
	 chnset "visible(0)", "PtrOSID"
	 chnset "visible(0)", "PtrID"
	 chnset "visible(0)", "PortID"
	 chnset "visible(0)", "inskipID"
	endif
endif
; -----------------------------------------------------------

 gSfilepath	chnget	"filename"
 kNewFileTrg	changed	gSfilepath		; if a new file is loaded generate a trigger
 if kNewFileTrg==1 then				; if a new file has been loaded...
  event	"i",99,0,0				; call instrument to update sample storage function table 
 endif  

 ktrig	trigger	gkPlayStop,0.5,0
 schedkwhen	ktrig,0,0,2,0,-1

 /* MOUSE SCRUBBING */
 gkMOUSE_DOWN_RIGHT	chnget	"MOUSE_DOWN_RIGHT"	; Read in mouse left click status
 kStartScrub		trigger	gkMOUSE_DOWN_RIGHT,0.5,0
 if gkMOUSE_DOWN_RIGHT==1 then
  gkmode	=	2
  if kStartScrub==1 then 
   reinit RAMP_FUNC
  endif
  RAMP_FUNC:
  krampup	linseg	0,0.001,1
  rireturn
  kMOUSE_X	chnget	"MOUSE_X"
  kMOUSE_Y	chnget	"MOUSE_Y"
  kMOUSE_X	=	(kMOUSE_X - 5) / 825
  kMOUSE_Y	portk	1 - ((kMOUSE_Y - 5) / 175), krampup*0.05		; SOME SMOOTHING OF DENSITY CHANGES VIA THE MOUSE ENHANCES PERFORMANCE RESULTS. MAKE ANY ADJUSTMENTS WITH ADDITIONAL CONSIDERATION OF guirefresh VALUE 
  gkptr		limit	kMOUSE_X,0,1
  gklevel	limit	kMOUSE_Y^2, 0, 1
  gktranspose	=	((kMOUSE_Y*2)-1)*gktranspose	;, -gktranspose, gktranspose
  schedkwhen	kStartScrub,0,0,2,0,-1
 else
  kptr		chnget	"ptr"
  gkptr		portk	kptr,gkport*kporttime
  gklevel	chnget	"level"
 endif 
 
endin



instr	99	; load sound file
 gichans	filenchnls	gSfilepath			; derive the number of channels (mono=1,stereo=2) in the sound file
 giFileLen	filelen		gSfilepath			; derive the number of channels (mono=1,stereo=2) in the sound file
 gitableL	ftgen	1,0,0,1,gSfilepath,0,0,1
 if gichans==2 then
  gitableR	ftgen	2,0,0,1,gSfilepath,0,0,2
 endif
 giReady 	=	1					; if no string has yet been loaded giReady will be zero

 Smessage sprintfk "file(%s)", gSfilepath
 chnset Smessage, "filer1"	

endin



instr	2	; triggered by 'play/stop' button
 if gkPlayStop==0&&gkMOUSE_DOWN_RIGHT==0 then
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

  kporttime	linseg	0,0.001,0.02				; portamento time function. (Rises quickly from zero to a held value.)

  kspeed	portk	gkspeed, kporttime
  kptr		portk	gkptr, kporttime
  
  ktranspose	portk	gktranspose,kporttime*3			; 
  
  ktrig	changed	gkshape,gkoverlap,gkgrsize,gkgrsizeOS,gkmode,gkinskip
  if ktrig==1 then
   reinit	UPDATE
  endif
  UPDATE:
  
  iwfn		=	i(gkshape) + giwfn1 - 1
  imode		=	i(gkmode) - 1

  if imode==0 then						; timestretch mode
   kwarp	=	1/(kspeed*gkfreeze)
   ibeg		=	i(gkinskip) * giFileLen
   kscrubber	init	ibeg*sr
   kscrubber	+=	kspeed*ksmps*gkfreeze
  elseif imode==1 then						; pointer mode
   kwarp	=	giFileLen * kptr
   ibeg		=	0
   kptrOS	random	0, (giFileLen - kwarp) * gkptrOS
   kwarp	=	kwarp + kptrOS
   kscrubber	=	(kwarp + kptrOS) * sr
  else								; region mode
   imode	=	1					; sndwarp mode used in region mode will be pointer mode
   kwarp	random	gkLoopStart/sr,(gkLoopStart+gkLoopEnd)/sr
   ibeg		=	0
   kscrubber	=	kwarp * sr
  endif

  apch		interp	semitone(ktranspose)
  klevel	portk	gklevel/(i(gkoverlap)^0.25), kporttime		; apply portamento smoothing to changes in level
  
  if gichans==1 then						; if mono...   
   a1	sndwarp		klevel, kwarp, apch, gitableL, ibeg, i(gkgrsize), i(gkgrsize) * i(gkgrsizeOS), i(gkoverlap), iwfn, imode
 	outs	a1*aenv,a1*aenv					; send mono audio to both outputs 
  elseif gichans==2 then						; otherwise, if stereo...
   a1	sndwarp		klevel, kwarp, apch, gitableL, ibeg, i(gkgrsize), i(gkgrsize) * i(gkgrsizeOS), i(gkoverlap), iwfn, imode
   a2	sndwarp		klevel, kwarp, apch, gitableR, ibeg, i(gkgrsize), i(gkgrsize) * i(gkgrsizeOS), i(gkoverlap), iwfn, imode
 	outs	a1*aenv,a2*aenv					; send stereo signal to outputs
  endif
  rireturn
 endif

 ; print scrubber
 if(metro(10)==1) then
  Smessage sprintfk "scrubberposition(%d)", kscrubber
  chnset Smessage, "filer1"
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

  kspeed	portk	gkspeed,kporttime
  kptr		portk	gkptr, kporttime
  
  ktranspose	portk	gktranspose,kporttime			; 
  
  ktrig	changed	gkshape,gkoverlap,gkgrsize,gkgrsizeOS,gkmode,gkinskip
  if ktrig==1 then
   reinit	UPDATE
  endif
  UPDATE:
  
  iwfn		=	i(gkshape) + giwfn1 - 1
  imode		=	i(gkmode) - 1
  
  if imode==0 then						; timestretch mode
   kwarp	=	1/(kspeed*gkfreeze)
   ibeg		=	i(gkinskip) * giFileLen
   kscrubber	init	ibeg*sr
   kscrubber	+=	kspeed*ksmps*gkfreeze
  elseif imode==1 then						; pointer mode
   kwarp	=	giFileLen * kptr
   kptrOS	random	0, (giFileLen - kwarp) * gkptrOS
   kwarp	=	kwarp + kptrOS
   ibeg		=	0
   kscrubber	=	(kwarp + kptrOS) * sr
  else								; region mode
   imode	=	1					; sndwarp mode used in region mode will be pointer mode
   kwarp	random	gkLoopStart/sr,(gkLoopStart+gkLoopEnd)/sr
   ibeg		=	0
   kscrubber	=	kwarp * sr
  endif


  apch		interp	semitone(ktranspose)
  klevel	portk	gklevel/(i(gkoverlap)^0.25), kporttime		; apply portamento smoothing to changes in level

  if gichans==1 then						; if mono...
   a1	sndwarp		klevel*iamp, kwarp, iFrqRatio, gitableL, ibeg, i(gkgrsize), i(gkgrsize) * i(gkgrsizeOS), i(gkoverlap), iwfn, imode
 	outs	a1*aenv,a1*aenv					; send mono audio to both outputs 
  elseif gichans==2 then						; otherwise, if stereo...
   a1	sndwarp		klevel*iamp, kwarp, iFrqRatio, gitableL, ibeg, i(gkgrsize), i(gkgrsize) * i(gkgrsizeOS), i(gkoverlap), iwfn, imode
   a2	sndwarp		klevel*iamp, kwarp, iFrqRatio, gitableR, ibeg, i(gkgrsize), i(gkgrsize) * i(gkgrsizeOS), i(gkoverlap), iwfn, imode
 	outs	a1*aenv,a2*aenv					; send stereo signal to outputs
  endif
  rireturn
 endif


 iactive	active	p1
 if iactive==1 then 
  if imode==0 then						; timestretch mode
   kscrubber	init	0
   kscrubber	+=	kspeed*ksmps*gkfreeze
  else								; pointer mode
   kptrOS	random	0, (giFileLen - kwarp) * gkptrOS
   kwarp	=	kwarp + kptrOS
   kscrubber	=	(kwarp + kptrOS) * sr
  endif
 
  ; print scrubber
 if(metro(10)==1) then
  Smessage sprintfk "scrubberposition(%d)", kscrubber
  chnset Smessage, "filer1"
 endif

 endif
endin

</CsInstruments>  

<CsScore>
i 1 0 [60*60*24*7]
</CsScore>

</CsoundSynthesizer>
