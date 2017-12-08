; FileStretcher.csd
; Written by Iain McCurdy, 2014

; Used to create a time-stretched version of an input file
; Often when time-stretching a sound when wishing to retain the priniciple characteristics of the source sound,
;  we play the attack portion of the sound at normal speed (as well, often, as the release portion) and only stretch
;  sound in the middle of the source - often called the 'sustain' portion.
; This example facilitates this technique by allowing the user to select the portion of sound to stretch by 
;  clicking and dragging upon the waveform view creating a highlighted area. Time stretching will only be applied to this 
;  portion of the sound.

; The user defines a time-stretching zone by clicking and dragging on the waveform view once a file has been opened.
; 'Duration' defines the total duration of the output file.

; 'Play' auditions the file stretch. 
; 'Record' renders the time-stretched audio to disk offline.
; The console reports the name of the output file and a message: 'Done!' when the rendering is complete. 
;    The output file will be created in the same directory as the input file and with the same name but with the word '_stretched' and a unique number suffixed. 

; 'Aud.Lev.' provides a amplitude level control (the rendered file is unaffected and always renders at full amplitude.

; Adding a small amount of jitter to the file reading pointer when time-stretching can reduce the sense of a frozen spectrum 
;  evident during long time stretches. Jitter depth and rate can be modified.

<Cabbage>
form caption("File Stretcher") size(580,430), colour(0,0,0) pluginID("FiSt"), guirefresh(10) 
image                bounds(  0,  0,580,430), colour(50,50,60), outlinecolour("White"), line(3), shape("sharp")

soundfiler           bounds(  5,  5,570,150), channel("beg","len"), identchannel("filer1"),  colour(0, 255, 255, 255), fontcolour(160, 160, 160, 255)
label bounds(6, 4, 560, 14), text(""), align(left), colour(0,0,0,0), fontcolour(200,200,200), identchannel("stringbox")
label      bounds( 6, 20,450, 13), text("Click and drag on waveform to select sound portion for time stretching..."), align(left), colour("black"), fontcolour("white"), visible(0), identchannel("InstructionID")

filebutton bounds(  7,160, 83, 20), text("OPEN FILE","OPEN FILE"), fontcolour(255,255,100) channel("filename"), shape("ellipse"), channel("beg","len")
button     bounds(  7,183, 83, 20), text("PLAY STR.","PLAY STR."), channel("PlayStr"), value(0), latched(0), fontcolour(100,255,100)
button     bounds( 96,183, 83, 20), text("PLAY NORM.","PLAY NORM."), channel("PlayNorm"), value(0), latched(0), fontcolour(150,150,255)
button     bounds(  7,206, 83, 20), text("RECORD","RECORD"), channel("Record"), value(0), latched(0), fontcolour(255,100,100)
button     bounds( 96,206, 83, 20), text("STOP","STOP"), channel("Stop"), value(0), latched(0), fontcolour(150,150,255)

rslider    bounds(178,160, 60, 60), text("Duration"), channel("Duration"), range(0.1, 60.00, 3,0.5,0.001), colour(30,30,40), trackercolour("white"), fontcolour("white"), outlinecolour(150,150,150)
rslider    bounds(238,160, 60, 60), text("Transpose"), channel("Transpose"), range(-48, 48.00, 0,1,0.5), colour(30,30,40), trackercolour("white"), fontcolour("white"), outlinecolour(150,150,150)
;nslider  bounds(180,160,110, 35), text("Stretched Duration"), channel("Duration"), range(0.1, 800.00, 3,1,0.001), colour(0,0,0), fontcolour("white"), textbox(1)
label      bounds(300,160, 80, 13), text("FFT Size"), fontcolour("white")
combobox   bounds(300,177, 80, 17), channel("FFTSize"), items("32768", "16384", "8192", "4096", "2048", "1024", "512", "256", "128", "64", "32"), value(5), fontcolour("white")
checkbox   bounds(300,198, 95, 15), channel("lock"), text("Phase Lock"), fontcolour("white"), colour(255,0,0)

rslider    bounds(390,160, 60, 60), text("Aud.Lev."), channel("Level"),  range(0, 1.00, 0.9),       colour(30,30,40), trackercolour("white"), fontcolour("white"), outlinecolour(150,150,150)
rslider    bounds(450,160, 60, 60), text("Jit.Dep."), channel("JitDep"), range(0, 1.00, 0.2),       colour(30,30,40), trackercolour("white"), fontcolour("white"), outlinecolour(150,150,150)
rslider    bounds(510,160, 60, 60), text("Jit.Rte."), channel("JitRte"), range(0.5, 100.00, 2,0.5), colour(30,30,40), trackercolour("white"), fontcolour("white"), outlinecolour(150,150,150)

csoundoutput bounds(  0,230,580,200), text("Csound Output")

</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-n -dm0
</CsOptions>

<CsInstruments>

sr = 44100
ksmps = 64
nchnls = 2
0dbfs=1

gichans		init	0		; 
giReady		init	0		; flag to indicate function table readiness
gSfilepath	init	""
giFFTSizes[]	array	32768, 16384, 8192, 4096, 2048, 1024, 512, 256, 128, 64, 32	; an array is used to store FFT window sizes
;maxalloc	2,1	; Only allow one sound playing/stretching instrument at a time
gkactive	init	0


opcode FileNameFromPath,S,S		; Extract a file name (as a string) from a full path (also as a string)
 Ssrc	xin						; Read in the file path string
 icnt	strlen	Ssrc			; Get the length of the file path string
 LOOP:							; Loop back to here when checking for a backslash
 iasc	strchar Ssrc, icnt		; Read ascii value of current letter for checking
 if iasc==92 igoto ESCAPE		; If it is a backslash, escape from loop
 loop_gt	icnt,1,0,LOOP		; Loop back and decrement counter which is also used as an index into the string
 ESCAPE:						; Escape point once the backslash has been found
 Sname	strsub Ssrc, icnt+1, -1	; Create a new string of just the file name
	xout	Sname				; Send it back to the caller instrument
endop

instr	1
 gkPlayStr	chnget	"PlayStr"
 gkPlayNorm	chnget	"PlayNorm"
 gkRecord	chnget	"Record"
 gSfilepath	chnget	"filename"
 kNewFileTrg	changed	gSfilepath		; if a new file is loaded generate a trigger
 gkLevel	chnget	"Level"
 gkTranspose	chnget	"Transpose"
 kporttime	linseg	0,0.01,0.05
 gkTranspose	portk	semitone(gkTranspose),kporttime
 gklock		chnget	"lock"			; Phase locking in mincer opcode
 gkAttack	chnget	"beg"
 gkLen		chnget	"len" 
 
 if trigger(gkAttack,1,0)==1 then		; Hide instruction once stretch area has been selected
  chnset "visible(0)", "InstructionID" 
 endif
 
 if kNewFileTrg==1 then				; if a new file has been loaded...
  event	"i",99,0,0.01				; call instrument to update sample storage function table 
 endif   
 
 ktrig	trigger	gkPlayStr,0.5,0			; Generate an impulse if PLAY STRETCH button is clicked
 schedkwhen	ktrig,0,0,2,0,0.01

 ktrig	trigger	gkPlayNorm,0.5,0		; Generate an impulse if PLAY NORM. button is clicked
 schedkwhen	ktrig,0,0,3,0,0.01

 ktrig	trigger	gkRecord,0.5,0			; Generate an impulse if RECORD button is clicked
 schedkwhen	ktrig,0,0,4,0,0.1
 
endin

instr	99	; load sound file
 
 /* write file selection to function tables */
 gichans	filenchnls	gSfilepath			; derive the number of channels (mono=1,stereo=2) in the sound file
 gitableL	ftgen	1,0,0,1,gSfilepath,0,0,1
 if gichans==2 then
  gitableR	ftgen	2,0,0,1,gSfilepath,0,0,2
 endif
 giReady 	=	1					; if no string has yet been loaded giReady will be zero
 Smessage sprintfk "file(%s)", gSfilepath			; print sound file image to fileplayer
 chnset Smessage, "filer1"

 /* write file name to GUI */
 Sname FileNameFromPath	gSfilepath				; Call UDO to extract file name from the full path
 Smessage sprintfk "text(%s)",Sname				; create string to update text() identifier for label widget
 chnset Smessage, "stringbox"					; send string to  widget

 if i(gkAttack)==0 then						; Reveal instruction when a file is opened for the first time
  chnset "visible(1)", "InstructionID"
 endif	

 /* write to pvx files */					; experimental
 ;#define	ANALYSISFILE_L	#"pvswriteFileL.pvx"#		; experimental
 ;#define	ANALYSISFILE_R	#"pvswriteFileR.pvx"#		; experimental
 ;kcount	=	0					; experimental
 ;loop:								; experimental
 ;acount	interp	kcount					; experimental
 ;asampL	table3	acount,1				; experimental	
 ;asampR	table3	acount,2                                ; experimental
 ;fsigL	pvsanal		asampL, 1024, 256, 1024, 1  		; experimental
 ;fsigR	pvsanal		asampR, 1024, 256, 1024, 1      	; experimental
 ;pvsfwrite 	fsigL, $ANALYSISFILE_L				; experimental
 ;pvsfwrite 	fsigR, $ANALYSISFILE_R				; experimental
 ;loop_lt	kcount,1,ftlen(1),loop				; experimental
 ;turnoff							; experimental

endin

instr	2	; play stretched
 if giReady = 1 then				; i.e. if a file has been loaded
  
  gkactive init i(gkactive) + 1	;INCREMENT NOTE COUNTER
  if gkactive>1 then		;IF POLYPHONY IS EXCEEDED (THROUGH THE ADDITION OF NEW NOTE)
   turnoff			;REMOVE THIS NOTE
  endif
  krel release			;IF NOTE HELD = 0, IF NOTE RELEASED = 1
  ktrig trigger krel,0.5,0	;WHEN RELEASE FLAG CROSSES 0.5 UPWARDS, I.E. NOTE HAS BEEN RELEASED...	
  if ktrig==1 then		
   gkactive = gkactive - 1	;...DECREMENT ACTIVE NOTES COUNTER
  endif
  
  kStop		chnget	"Stop"
  ktrig		trigger	kStop,0.5,0		; Generate an impulse if 'STOP' is clicked
  if ktrig==1 then
   turnoff
  endif

  iFileLen	=	ftlen(gitableL)/sr											; File length in seconds
  iAttPnt	=	(i(gkAttack) == 0 ? 0.01 : (i(gkAttack)/sr))				; If stretch points haven't been set, replace with defaults. 
  iRelPnt	=	(i(gkAttack) == 0 ? 0.01 : ((i(gkAttack) + i(gkLen))/sr))
  
  iDuration	chnget	"Duration"
  
  p3		limit	iDuration, iFileLen, iDuration
  
  iFFTSize	chnget	"FFTSize"
  iFFTSize	=	giFFTSizes[iFFTSize-1]
  
  ; File reading pointer main shape
  aptr		linseg	0,iAttPnt,iAttPnt, p3-iAttPnt-(iFileLen-iRelPnt), iRelPnt, iFileLen-iRelPnt, iFileLen

  ; File reading pointer jitter
  kJitDep	chnget	"JitDep"
  kJitRte	chnget	"JitRte"
  irise		=	0.01
  kJitEnv	linseg	0,iAttPnt,0, irise, .1, p3-iAttPnt-(iFileLen-iRelPnt) - (2*irise), .1, irise, 0 
  ajit		jspline	kJitDep*kJitEnv,kJitRte*0.5,kJitRte*2

  if gichans=1 then						; Mono
   a1	mincer		aptr+ajit, 1, gkTranspose, gitableL, gklock, iFFTSize
   ;kptr	linseg	0,iAttPnt,iAttPnt, p3-iAttPnt-(iFileLen-iRelPnt), iRelPnt, iFileLen-iRelPnt, iFileLen	; experimental
   ;a1	pvoc		kptr, gkTranspose, $ANALYSISFILE_L							; experimental
  	outs	a1*gkLevel,a1*gkLevel

  elseif gichans=2 then						; Stereo
   a1	mincer		aptr+ajit, 1, gkTranspose, gitableL, gklock, iFFTSize
   a2	mincer		aptr+ajit, 1, gkTranspose, gitableR, gklock, iFFTSize
   ;kptr	linseg	0,iAttPnt,iAttPnt, p3-iAttPnt-(iFileLen-iRelPnt), iRelPnt, iFileLen-iRelPnt, iFileLen	; experimental
   ;a1	pvoc		kptr, gkTranspose, $ANALYSISFILE_L							; experimental
   ;a2	pvoc		kptr, gkTranspose, $ANALYSISFILE_R							; experimental
  	outs	a1*gkLevel,a2*gkLevel
  endif
 endif
endin

instr	3	; normal speed playback
 p3	=	ftlen(gitableL)/(sr*i(gkTranspose))
 aptr	line	0,p3,1
 aL	table3	aptr,gitableL,1
 if gichans==1 then
 	outs	aL*gkLevel,aL*gkLevel
 else
 aR	table3	aptr,gitableR,1
	outs	aL*gkLevel,aR*gkLevel 
 endif 
 kStop		chnget	"Stop"
 ktrig		trigger	kStop,0.5,0		; Generate an impulse if 'STOP' is clicked
 if ktrig==1 then
  turnoff
 endif
endin


instr	4	; record/render
 if giReady = 1 then				; i.e. if a file has been loaded

  iFileLen	=	ftlen(gitableL)/sr						; File length in seconds
  iAttPnt	=	(i(gkAttack) == 0 ? 0.01 : (i(gkAttack)/sr))			; If stretch points haven't been set, replace with defaults. 
  iRelPnt	=	(i(gkAttack) == 0 ? 0.01 : ((i(gkAttack) + i(gkLen))/sr))
  
  iDuration	chnget	"Duration"
  
  p3		=	iDuration
     		
  ilen		strlen	gSfilepath			; Derive string length.
  SOutputName	strsub	gSfilepath,0,ilen-4		; Remove ".wav"
  SOutputName	strcat	SOutputName,"_stretched"	; Add suffix
  iDate		date
  SDate		sprintf	"%i",iDate
  SOutputName	strcat	SOutputName,SDate		; Add date
  SOutputName	strcat	SOutputName,".wav"		; Add extension
  
  
  iFFTSize	chnget	"FFTSize"
  iFFTSize	=	giFFTSizes[iFFTSize-1]
  
  iNKCycles	=	(iDuration * sr) / ksmps
  kCount	init	0
  
  loop:
  
  ; File reading pointer main shape
  aptr		linseg	0,iAttPnt,iAttPnt, p3-iAttPnt-(iFileLen-iRelPnt), iRelPnt, iFileLen-iRelPnt, iFileLen

  ; File reading pointer jitter
  kJitDep	chnget	"JitDep"
  kJitRte	chnget	"JitRte"
  irise		=	0.01
  kJitEnv	linseg	0,iAttPnt,0, irise, .1, p3-iAttPnt-(iFileLen-iRelPnt) - (2*irise), .1, irise, 0 
  ajit		jspline	kJitDep*kJitEnv,kJitRte*0.5,kJitRte*2

  if gichans=1 then						; Mono
   a1	mincer		aptr+ajit, 1, gkTranspose, gitableL, gklock, iFFTSize
   ;kptr	linseg	0,iAttPnt,iAttPnt, p3-iAttPnt-(iFileLen-iRelPnt), iRelPnt, iFileLen-iRelPnt, iFileLen	; experimental
   ;a1	pvoc		kptr, gkTranspose, $ANALYSISFILE_L							; experimental
  	fout	SOutputName,8,a1

  elseif gichans=2 then						; Stereo
   a1	mincer		aptr+ajit, 1, gkTranspose, gitableL, gklock, iFFTSize
   a2	mincer		aptr+ajit, 1, gkTranspose, gitableR, gklock, iFFTSize
   ;kptr	linseg	0,iAttPnt,iAttPnt, p3-iAttPnt-(iFileLen-iRelPnt), iRelPnt, iFileLen-iRelPnt, iFileLen	; experimental
   ;a1	pvoc		kptr, gkTranspose, $ANALYSISFILE_L							; experimental
   ;a2	pvoc		kptr, gkTranspose, $ANALYSISFILE_R							; experimental
  	fout	SOutputName,8,a1,a2
  endif

	loop_lt	kCount, 1, iNKCycles, loop
	printks	SOutputName,0
	printks	"\nDone!\n",0
	turnoff
	
 endif
endin

</CsInstruments>  

<CsScore>
i 1 0 10000
</CsScore>

</CsoundSynthesizer>
