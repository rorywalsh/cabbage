; FileRecorder.csd
; Written by Iain McCurdy, 2014
; 
; Records audio to a file on disk. 
; 
; The audio file name includes the date and time. (This idea comes courtesy of Rory Walsh.)
; 
; Toggling record off and on will append the new recording onto the end of the previously recorded audio.
; To start a new file, click 'New File'.


<Cabbage>
form caption("File Recorder") size(230, 45), colour(0,0,0) pluginId("FRec") style("legacy")
image               bounds(  0,  0,230, 45), colour(100,100,100), outlineColour("White"), line(3)
checkbox bounds( 10, 10, 75, 25), channel("record"), text("Record"), colour("red"), fontColour("white")
button   bounds( 90, 10, 65, 25), colour("red"), text("New File","New File"), channel("reset"), latched(0)
checkbox bounds(165, 10, 75, 25), channel("play"), text("Play"), colour("green"), fontColour("white")
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-n -dm0
</CsOptions>

<CsInstruments>

;sr is set by the host
ksmps = 64
nchnls = 2
0dbfs = 1

gkRecordingActiveFlag	init	0
gkFileRecorded		init	0

instr	1
 gaL,gaR	ins
 gkrecord	chnget	"record"
 gkreset	chnget	"reset"
 gkplay		chnget	"play"
 kRecStart	trigger	gkrecord,0.5,0

 if kRecStart==1 && gkRecordingActiveFlag==0 then
  event	"i",9000,0,-1
  gkRecordingActiveFlag	=	1
 endif
 
 kPlayStart	trigger	gkplay,0.5,0
 if kPlayStart==1 && gkFileRecorded==1 then
  event	"i",9001,ksmps/sr,3600
 endif

 kResetTrig	trigger	gkreset,0.5,1
 if kResetTrig==1 && gkRecordingActiveFlag==1 then
  event	"i",9000,0,-1
 endif  
endin


instr 9000	; record file
 print	p1
 if gkplay==1 then
  chnset	k(0),"record"
  turnoff
 endif
 gkFileRecorded		init	1
 itim     date
 Stim     dates     itim
 itim     date
 Stim     dates     itim
 Syear    strsub    Stim, 20, 24
 Smonth   strsub    Stim, 4, 7
 Sday     strsub    Stim, 8, 10
 iday     strtod    Sday
 Shor     strsub    Stim, 11, 13
 Smin     strsub    Stim, 14, 16
 Ssec     strsub    Stim, 17, 19
 Sfilnam  sprintf  "%s_%s_%02d_%s_%s_%s.wav", Syear, Smonth, iday, Shor,Smin, Ssec
 gSname   sprintf "FileRecorder_%s", Sfilnam
 if gkrecord==1 then			; record
  fout gSname, 8, gaL, gaR
 endif
 gkRecordingActiveFlag	=	1 - release()
endin

instr	9001	; play file
 if gkplay==0 then
  turnoff
 endif 
   aL,aR	diskin2	gSname,1
  	outs	aL,aR
 iFileLen	filelen	gSname
 p3	=	iFileLen
 xtratim	0.1 
 krelease	release
 	chnset	1-krelease,"play"
endin

</CsInstruments>  

<CsScore>
i 1 0 [60*60*24*7]
</CsScore>

</CsoundSynthesizer>