
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; Time Bandit
; Iain McCurdy, 2021

; This instrument is intended for the processing of short samples via a gestural envelope drawn over the waveform.
; This envelope can be used to control:
;  Playback speed (varispeed style)
;  Playback speed (timestretch style)
;  Pitch shift
;  Filter (lowpass-highpass in series)
;  vari/filter - combination of varispeed and filter

; As well as drawing envelopes over the waveform using the mouse, a number of options are offered for automating the process of drawing this envelope. 

; Unison position slider - this is the vertical slider on the left of of the panel and is used to set the unison point (no change) 
;   for speed change, timestretch and filters

; CONTROLS
; OPEN FILE - browse for and select a sound
; (mode) - chose a playback mode from 'Varispeed', 'timestretch, 'pitch shift', 'filter', Varispeed and Filter'
; PLAY - play the selected sound using the method chosen
; REC OUT - render the current setup as a sound file (to the same location as the selected input file

; RANGE
; Range covered by the drawn envelope (varispeed, timestretch and pitch shift - not filter)
; Min - minimum ratio change
; Max - maximum ratio change

; QUALITY
; Aspect of the sound quality relating to the timestretch and pitch shift modes
; FFT Size - larger values favour frequency resolution, smaller values favour time resolution
; Lock - lock phases, can improve quality in freeze frames

; FILTER
; Min - minimum filter frequency when envelope is at the lowest position
; Max - maximum filter frequency when envelope is at the highest position
; Shape - response mapping of the filter with respect to the envelope
; N.Poles

; RANDOM/LFO
; Generates a random or sinusoidal function across the envelope panel
; Range - amplitude (positive and negative of the function
; Rate - rate of LFO or random wobble

; FOLLOW
; Create the envelope by amplitude following the chosen sound file
; Listen Ahead - using this control the envelope followed will be offset with respect to where the envelope will be drawn.
;  This can be useful when creating slow attack/laggy envelopes
; Attack - rising response time of the envelope follower
; Release - falling response time of the envelope follower
; Shape - Curvature of the envelope produced (<1 concave, 1=linear, >1 convex)
; Sensitivity - will determine the positive amplitude of the envelope

; TRIGGER
; Creates a sample and hold random function in which each change is triggered by transient of the sound file
; Listen Ahead - similar to the control in FOLLOW section
; Time - sample time for assessing transient (use larger values for sources with slower attacks)
; Thresh - threshold above which triggers will be generated. 
;  Lower values will recult in a more sensitive triggering, a denser rate of change but possibly false triggerings.
; Wait - wait time to prevent double triggerings - a debouncer. For a slower source this can be increased to prevent false triggerings.
;   Should be reduced for faster source.
; Sensitivity - amplitude of the random numbers generated

<Cabbage>
form caption("Time Bandit") size(945, 470), pluginId("TiBn") colour(20,25,30) guiRefresh(10)

#define RSliderStyle  trackerInsideRadius(.9), trackerOutsideRadius(1), markerColour(255,100,100), markerThickness(2), trackerColour(255,130,130), trackerThickness(4), colour(60,65,80), fontColour(200,200,200), textColour(200,200,200), outlineColour(50,50,50), valueTextBox(1)
#define RSliderStyle2 trackerColour(200,200,200), colour( 50, 55, 50), fontColour(200,200,200), textColour(200,200,200),  markerColour(220,220,220), outlineColour(50,50,50), valueTextBox(0)

soundfiler bounds( 20, 17, 915, 200), channel("beg","len"), identChannel("filer1"),  colour(255, 100, 100), fontColour(160, 160, 160, 255)
gentable   bounds( 20, 15, 915, 200), tableNumbers(101), tableColour:0(155,155,255,255), tableBackgroundColour(0,0,0,0), fill(0), outlineThickness(2), tableGridColour(0,0,0,0), identChannel("IDSpeedGraph"), ampRange(0,1,-1,0.01), zoom(-1), active(1)
label      bounds( 20,  4, 915, 14), text(""), align(left), colour(0,0,0,0), fontColour(200,200,200), identChannel("stringbox")
line       bounds( -20, -85, 915, 1), identChannel("IDUnisonLine")

vslider    bounds( 0,   9,20, 219), channel("UniLine"), range(0, 1, 0.5),  colour(250,230,250,250), trackerColour("silver")

combobox   bounds( 25,190, 90, 25), channel("Mode"), items("Varispeed", "Timestretch", "Pitch Shift", "Filter", "Vari/Filter"), value(1), fontColour("white")

image      bounds( 10,225,100,115), colour(0,0,0,0), plant("controls"), outlineColour("silver"), outlineThickness(1) {
label      bounds(  0,  5,100, 10), text("C O N T R O L S"), fontColour(200,200,200), align("centre")
filebutton bounds( 10, 30, 80, 20), text("OPEN FILE","OPEN FILE"), fontColour("white") channel("filename"), shape("ellipse"), fontColour:0(100,100,150), fontColour:1(205,205,255), colour:0(0,0,10), colour:1(0,0,150)
button     bounds( 10, 55, 80, 20), text("PLAY","PLAY"), channel("PlayStop"), value(0), latched(1), fontColour:0(50,100,50), fontColour:1(205,255,205), colour:0(0,10,0), colour:1(0,150,0)
button     bounds( 10, 80, 80, 20), text("REC OUT","REC OUT"), channel("RecOut"), value(0), latched(1), fontColour:0(100,50,50), fontColour:1(255,205,205), colour:0(10,0,0), colour:1(150,0,0)
}

image      bounds(115,225,160,115), colour(0,0,0,0), plant("Range"), outlineColour("silver"), outlineThickness(1) {
label      bounds(  0,  5,160, 10), text("R A N G E"), fontColour(200,200,200), align("centre")
rslider    bounds( 10, 15, 70, 90), text("Min"),  channel("MinSpd"),  range(0.01,16,0.5,0.5,0.0001),            $RSliderStyle
rslider    bounds( 80, 15, 70, 90), text("Max"),  channel("MaxSpd"),  range(0.01,16,2,  0.5,0.0001),          $RSliderStyle
}

image      bounds(280,225, 80,115), colour(0,0,0,0), plant("Quality"), outlineColour("silver"), outlineThickness(1) {
label      bounds(  0,  5, 80, 10), text("Q U A L I T Y"), fontColour(200,200,200), align("centre")
label      bounds( 10, 32, 63, 12), text("FFT Size"), fontColour(200,200,200)
combobox   bounds( 10, 45, 63, 20), channel("FFTSize"), items("16384", "8192", "4096", "2048", "1024", "512", "256", "128", "64"), value(4), fontColour("white")
checkbox   bounds( 10, 70, 80, 15), text("Lock"), fontColour:0("white"), fontColour:1("white"), channel("Lock"), colour(255,255, 0)
}

image      bounds(365,225,300,115), colour(0,0,0,0), plant("Filter"), outlineColour("silver"), outlineThickness(1) {
label      bounds(  0,  5,300, 10), text("F I L T E R"), fontColour(200,200,200), align("centre")
rslider    bounds( 10, 15, 70, 90), text("Min"),    channel("BaseFilt"),  range(4,14,4),          $RSliderStyle
rslider    bounds( 80, 15, 70, 90), text("Max"),     channel("MaxFilt"),   range(4,14,14),          $RSliderStyle
rslider    bounds(150, 15, 70, 90), text("Shape"),   channel("ShapeFilt"), range(0.25,2,1, 0.5),          $RSliderStyle
rslider    bounds(220, 15, 70, 90), text("N.Poles"),   channel("NPoles"), range(2,80, 2, 1, 2),          $RSliderStyle
}

image      bounds(670,225,265,115), colour(0,0,0,0), plant("RandomLFO"), outlineColour("silver"), outlineThickness(1) {
label      bounds(  0,  5,260, 10), text("R A N D O M / L F O"), fontColour(200,200,200), align("centre")
button     bounds( 15, 35, 70, 25), text("RANDOM","RANDOM"), fontColour("white") channel("Randomise"), latched(0), fontColour:0(100,100,50), fontColour:1(255,255,105), colour:0(10,10,0), colour:1(150,150,0)
button     bounds( 15, 65, 70, 25), text("LFO","LFO"), fontColour("white") channel("LFO"), latched(0), fontColour:0(100,100,50), fontColour:1(255,255,105), colour:0(10,10,0), colour:1(150,150,0)
rslider    bounds(100, 15, 70, 90), text("Range"),  channel("Range"),  range(0,0.5,0.1),          $RSliderStyle
rslider    bounds(170, 15, 70, 90), text("Rate"),   channel("Rate"),   range(1,250,10,0.5,0.1),   $RSliderStyle
}

image      bounds( 10,345,460,115), colour(0,0,0,0), plant("Follow"), outlineColour("silver"), outlineThickness(1) {
label      bounds(  0,  5,460, 10), text("F O L L O W"), fontColour(200,200,200), align("centre")
button     bounds( 15, 50, 80, 25), text("FOLLOW","FOLLOW"), fontColour("white") channel("Follow"), latched(1), fontColour:0(100,100,50), fontColour:1(255,255,105), colour:0(10,10,0), colour:1(150,150,0)
rslider    bounds(100, 15, 70, 90), text("Listen Ahead"),  channel("ListenAhead"),  range(-0.5,0.5,0.01,1,0.001),       $RSliderStyle
rslider    bounds(170, 15, 70, 90), text("Attack"),  channel("Attack"),  range(0,100,0.01,0.25,0.001),     $RSliderStyle
rslider    bounds(240, 15, 70, 90), text("Release"),  channel("Release"),  range(0,100,50,0.25,0.001),    $RSliderStyle
rslider    bounds(310, 15, 70, 90), text("Shape"),   channel("FollowShape"),  range(0.25,8,1,0.3),           $RSliderStyle
rslider    bounds(380, 15, 70, 90), text("Sensitivity"),   channel("FollowSens"),  range(0,1,0.25),           $RSliderStyle
}

image      bounds(475,345,460,115), colour(0,0,0,0), plant("Follow"), outlineColour("silver"), outlineThickness(1) {
label      bounds(  0,  5,460, 10), text("T R I G G E R"),     fontColour(200,200,200), align("centre")
button     bounds( 15, 50, 80, 25), text("TRIGGER","TRIGGER"), fontColour("white") channel("Trigger"), latched(1), fontColour:0(100,100,50), fontColour:1(255,255,105), colour:0(10,10,0), colour:1(150,150,0)
rslider    bounds(100, 15, 70, 90), text("Listen Ahead"),      channel("ListenAheadTrig"), range(-0.5,0.5,0.01,1,0.001),       $RSliderStyle
rslider    bounds(170, 15, 70, 90), text("Time"),              channel("TimeTrig"),        range(0.001,0.2,0.01,0.5,0.001),     $RSliderStyle
rslider    bounds(240, 15, 70, 90), text("Thresh"),            channel("ThreshTrig"),      range(0.01,0.1,0.04,1,0.001),    $RSliderStyle
rslider    bounds(310, 15, 70, 90), text("Wait"),              channel("WaitTrig"),        range(10,10000,1000,0.3,1),           $RSliderStyle
rslider    bounds(380, 15, 70, 90), text("Rand.Amp."),       channel("SensTrig"),      range(0,1,0.9),           $RSliderStyle
}

</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-n -+rtmidi=NULL -M0 -dm0
</CsOptions>

<CsInstruments>

; sr set by host
ksmps = 64
nchnls = 2
0dbfs=1


giSpeedGraph    ftgen    101, 0,   16384, -7, 0.5, 16384, 0.5
giSpeedScale    ftgen    201, 0,   4096, -16, 0.5, 2048, 0, 1, 2048, 0, 2 
giFiltScale     ftgen    202, 0,   4096, -16, 0, 2048, 0, 0.5, 2048, 0, 1 

giFFTSizes[]  array   16384, 8192, 4096, 2048, 1024, 512, 256, 128, 64    ; an array is used to store FFT window sizes

gSfilepath    init    ""


opcode FileNameFromPath,S,S           ; Extract a file name (as a string) from a full path (also as a string)
 Ssrc      xin                        ; Read in the file path string
 icnt      strlen  Ssrc               ; Get the length of the file path string
 LOOP:                                ; Loop back to here when checking for a backslash
 iasc      strchar Ssrc, icnt         ; Read ascii value of current letter for checking
 if iasc==92 igoto ESCAPE             ; If it is a backslash, escape from loop
 loop_gt    icnt,1,0,LOOP             ; Loop back and decrement counter which is also used as an index into the string
 ESCAPE:                              ; Escape point once the backslash has been found
 Sname     strsub  Ssrc, icnt+1, -1   ; Create a new string of just the file name
           xout    Sname              ; Send it back to the caller instrument
endop



instr    1
 gkPlayStop    chnget    "PlayStop"        ; read in widgets
 gSfilepath    chnget    "filename"        ; read in file path string from filebutton widget
 gkUniLine     chnget    "UniLine"
 kMinSpd       chnget    "MinSpd"
 kMaxSpd       chnget    "MaxSpd"
 gkRange       chnget    "Range"
 gkRate        chnget    "Rate"
 gkLock        chnget    "Lock"
 gkRecOut      chnget    "RecOut"
 gkBaseFilt    chnget    "BaseFilt"
 gkMaxFilt     chnget    "MaxFilt"
 gkShapeFilt   chnget    "ShapeFilt"
 gkNPoles      chnget    "NPoles"
 gkListenAhead chnget    "ListenAhead"
 gkAttack      chnget    "Attack"
 gkRelease     chnget    "Release"
 gkFollowShape chnget    "FollowShape"
 gkFollowSens  chnget    "FollowSens"

gkListenAheadTrig chnget "ListenAheadTrig"
gkTimeTrig        chnget "TimeTrig"
gkThreshTrig      chnget "ThreshTrig"
gkWaitTrig        chnget "WaitTrig"
gkSensTrig        chnget "SensTrig"
    
 if changed:k(gkUniLine,kMinSpd,kMaxSpd)==1 then
  reinit UPDATE_UNILINE
 endif
 UPDATE_UNILINE:
  Smsg  sprintf "pos(20,%d)",(1-i(gkUniLine))*200 + 15
  chnset Smsg, "IDUnisonLine"
  giSpeedScale    ftgen    201, 0,   4096, -16, i(kMinSpd), limit:i(4096*i(gkUniLine),1,4096), 0,   1, limit:i(4096*(1-i(gkUniLine)),0,4096), 0, i(kMaxSpd) 
  giFiltScale     ftgen    202, 0,   4096, -16, 0,          limit:i(4096*i(gkUniLine),1,4096), 0, 0.5, limit:i(4096*(1-i(gkUniLine)),0,4096), 0, 1
 rireturn
 if changed:k(gSfilepath)==1 then          ; call instrument to update waveform viewer  
  event "i", 99, 0, 0
 endif
 
 gkMode        chnget    "Mode"
 ktrig         trigger    gkPlayStop,0.5,0  ; if play/stop button toggles from low (0) to high (1) generate a '1' trigger
 schedkwhen    ktrig, 0, 0, 1 + gkMode,0,-1            ; start instrument 2

 
 kRandomise chnget "Randomise"
 if trigger:k(kRandomise,0.5,0)==1 then
  event "i", 10, 0, 0.1
 endif

 kLFO chnget "LFO"
 if trigger:k(kLFO,0.5,0)==1 then
  event "i", 11, 0, 0.1
 endif

 kFollow chnget "Follow"
 if trigger:k(kFollow,0.5,0)==1 then
  event "i", 12, 0, -1
  chnset k(0),"Follow"
 endif

 kTrigger chnget "Trigger"
 if trigger:k(kTrigger,0.5,0)==1 then
  event "i", 13, 0, -1
  chnset k(0),"Trigger" 
 endif

 if trigger:k(gkRecOut,0.5,0)==1 then
  chnset k(1),"PlayStop"
  event "i", 1000, 0, -1
 endif

endin





instr    99    ; load sound file
 gichans    filenchnls    gSfilepath            ; derive the number of channels (mono=1,stereo=2) in the sound file
 gitableL    ftgen    1,0,0,1,gSfilepath,0,0,1
 giFileSamps    =        nsamp(gitableL)            ; derive the file duration in samples
 giFileLen    filelen        gSfilepath            ; derive the file duration in seconds
 gkTabLen    init        ftlen(gitableL)            ; table length in sample frames
 if gichans==2 then
  gitableR    ftgen    2,0,0,1,gSfilepath,0,0,2
 endif
 giReady     =    1                                ; if no string has yet been loaded giReady will be zero
 Smessage sprintfk "file(%s)", gSfilepath            ; print file to viewer
 chnset Smessage, "filer1"    

 /* WRITE FILE NAME TO GUI */
 Sname FileNameFromPath    gSfilepath                ; Call UDO to extract file name from the full path
 Smessage sprintfk "text(%s)",Sname
 chnset Smessage, "stringbox"

endin




; varispeed
instr    2

 if gkPlayStop==0  then                ; if play/stop is off (stop)...
  turnoff                    ; turn off this instrument
 endif                        

 if strlen:i(gSfilepath)==0 then
  turnoff2 p1,0,0
 endif 

 iLen filelen  gSfilepath
 kNdx init     0
 kSpdGrph tablei   kNdx, giSpeedGraph
 kSpd     tablei   kSpdGrph, giSpeedScale, 1

 if gichans==1 then
  aL diskin2 gSfilepath,kSpd,0,0,0,128
  aR =       aL
 else
  aL,aR diskin2 gSfilepath,kSpd,0,0,0,128
 endif
       outs    aL, aR

 kNdx  +=      (ftlen(giSpeedGraph)  /   (iLen * kr)) * kSpd

 ;scrubber
 Smsg sprintfk "scrubberPosition(%d)", iLen * sr * kNdx/ftlen(giSpeedGraph)
 chnset Smsg, "filer1"

 if kNdx>ftlen(giSpeedGraph) then
  chnset k(0), "PlayStop"
  turnoff
 endif

endin



; timestretch
instr    3

 if gkPlayStop==0  then                ; if play/stop is off (stop)...
  turnoff                    ; turn off this instrument
 endif                        

 if strlen:i(gSfilepath)==0 then
  turnoff2 p1,0,0
 endif 

 ifftsize    chnget    "FFTSize"
 ifftsize    =     giFFTSizes[ifftsize-1]
  
 iLen     =        ftlen(gitableL)/sr
 kNdx     init     0
 kSpdGrph table    kNdx, giSpeedGraph
 kSpd     table    kSpdGrph, giSpeedScale, 1

 kamp  =          1
 kpitch =         1
 idecim = 8

 kPnt  init       0
 if trigger:k(kPnt,iLen,0)==1 then
  chnset k(0),"PlayStop"
 endif
 
 kPnt  limit      kPnt, 0, iLen
 if gichans==1 then
  aL    mincer     a(kPnt), kamp, kpitch, gitableL, gkLock, ifftsize, idecim
  aR =       aL
 else
  aL    mincer     a(kPnt), kamp, kpitch, gitableL, gkLock, ifftsize, idecim
  aR    mincer     a(kPnt), kamp, kpitch, gitableR, gkLock, ifftsize, idecim
 endif
       outs    aL, aR 

 ;scrubber
 Smsg sprintfk "scrubberPosition(%d)", kPnt * sr
 chnset Smsg, "filer1"

 if kPnt-5*ksmps/kr>iLen then
  chnset k(0), "PlayStop"
 endif 
 
 kPnt  +=      (1/kr) * kSpd
 
 kNdx  +=      (ftlen(101)  /   (iLen * kr)) * kSpd


endin




; pitch shift
instr    4

 if gkPlayStop==0  then                ; if play/stop is off (stop)...
  turnoff                    ; turn off this instrument
 endif                        

 if strlen:i(gSfilepath)==0 then
  turnoff2 p1,0,0
 endif 

 ifftsize    chnget    "FFTSize"
 ifftsize    =    giFFTSizes[ifftsize-1]
 
 iLen     =        ftlen(gitableL)/sr
 kNdx     init     0
 kSpdGrph table    kNdx, giSpeedGraph
 kSpd     table    kSpdGrph, giSpeedScale, 1


 kamp  =          1
 kpitch =         1
 idecim = 8

 aPnt  line       0, iLen, iLen     
 
 if trigger:k(k(aPnt),iLen,0)==1 then
  chnset k(0),"PlayStop"
 endif
 
 aPnt  limit      aPnt, 0, iLen

 if gichans==1 then
  aL    mincer     aPnt, kamp, kSpd, gitableL, gkLock, ifftsize, idecim
  aR =       aL
 else
  aL    mincer     aPnt, kamp, kSpd, gitableL, gkLock, ifftsize, idecim
  aR    mincer     aPnt, kamp, kSpd, gitableR, gkLock, ifftsize, idecim
 endif
       outs    aL, aR 
 
 ;scrubber
 Smsg sprintfk "scrubberPosition(%d)", k(aPnt)*sr
 chnset Smsg, "filer1"
 
 if k(aPnt)-5*ksmps/kr>iLen then
  chnset k(0), "PlayStop"
 endif 
 
 kNdx  +=      (ftlen(101)  /   (iLen * kr))

endin





; filter
instr    5

 if gkPlayStop==0  then                ; if play/stop is off (stop)...
  turnoff                    ; turn off this instrument
 endif                        

 if strlen:i(gSfilepath)==0 then
  turnoff2 p1,0,0
 endif 

 iLen filelen  gSfilepath
 kNdx init     0
 kSpdGrph tablei   kNdx, giSpeedGraph

 kPortTime linseg 0, 0.001, 0.005

 kFilt   table  kSpdGrph, giFiltScale, 1
 kLPF    limit  kFilt * 2, 0, 1 
 kLPF    scale  (kLPF^gkShapeFilt), 14, gkBaseFilt
 kLPF    limit  cpsoct(kLPF), 20, 20000
 kLPF    portk  kLPF, kPortTime
 
 kHPF    limit  (kFilt*2) - 1, 0, 1
 kHPF    scale  (kHPF^(1/gkShapeFilt)), gkMaxFilt, 4 
 kHPF    limit  cpsoct(kHPF), 20, 20000
 kHPF    portk  kHPF, kPortTime
 
 if gichans==1 then
  aL diskin2 gSfilepath,1,0,0,0,4
  aR =       aL
 else
  aL,aR diskin2 gSfilepath,1,0,0,0,4
 endif
 
 if changed(gkNPoles)==1 then
  reinit REINIT_FILTER1
 endif
 REINIT_FILTER1:
 aL   clfilt  aL, kLPF, 0, i(gkNPoles)
 aR   clfilt  aR, kLPF, 0, i(gkNPoles)

 aL   clfilt  aL, kHPF, 1, i(gkNPoles)
 aR   clfilt  aR, kHPF, 1, i(gkNPoles)
 rireturn
 
      outs    aL, aR

 kNdx  +=      (ftlen(giSpeedGraph)  /   (iLen * kr))

 ;scrubber
 Smsg sprintfk "scrubberPosition(%d)", iLen * sr * kNdx/ftlen(giSpeedGraph)
 chnset Smsg, "filer1"

 if kNdx>ftlen(giSpeedGraph) then
  chnset k(0), "PlayStop"
  turnoff
 endif

endin













; vari/filter
instr    6

 if gkPlayStop==0  then                ; if play/stop is off (stop)...
  turnoff                    ; turn off this instrument
 endif                        

 if strlen:i(gSfilepath)==0 then
  turnoff2 p1,0,0
 endif 

 iLen filelen  gSfilepath
 kNdx init     0
 kSpdGrph tablei   kNdx, giSpeedGraph
 kSpd     tablei   kSpdGrph, giSpeedScale, 1

 kPortTime linseg 0, 0.001, 0.005

 kFilt   table  kSpdGrph, giFiltScale, 1
 kLPF    limit  kFilt * 2, 0, 1 
 kLPF    scale  (kLPF^gkShapeFilt), 14, gkBaseFilt
 kLPF    limit  cpsoct(kLPF), 20, 20000
 kLPF    portk  kLPF, kPortTime
 
 kHPF    limit  (kFilt*2) - 1, 0, 1
 kHPF    scale  (kHPF^(1/gkShapeFilt)), gkMaxFilt, 4 
 kHPF    limit  cpsoct(kHPF), 20, 20000
 kHPF    portk  kHPF, kPortTime
  
 if gichans==1 then
  aL diskin2 gSfilepath,kSpd,0,0,0,4
  aR =       aL
 else
  aL,aR diskin2 gSfilepath,kSpd,0,0,0,4
 endif
  
 if changed(gkNPoles)==1 then
  reinit REINIT_FILTER1
 endif
 REINIT_FILTER1:
 aL   clfilt  aL, kLPF, 0, i(gkNPoles)
 aR   clfilt  aR, kLPF, 0, i(gkNPoles)

 aL   clfilt  aL, kHPF, 1, i(gkNPoles)
 aR   clfilt  aR, kHPF, 1, i(gkNPoles)
 rireturn
 
      outs    aL, aR

 kNdx  +=      (ftlen(giSpeedGraph)  /   (iLen * kr)) * kSpd

 ;scrubber
 Smsg sprintfk "scrubberPosition(%d)", iLen * sr * kNdx/ftlen(giSpeedGraph)
 chnset Smsg, "filer1"

 if kNdx>ftlen(giSpeedGraph) then
  chnset k(0), "PlayStop"
  turnoff
 endif

endin





; Randomise
instr 10

 iLen = ftlen(giSpeedGraph)

 kCnt = 0
 while kCnt<iLen do
 
 kRandom jspline  gkRange, gkRate, gkRate
 tablew kRandom+0.5, kCnt, giSpeedGraph
 
 
 kCnt += 1
 od
 
 chnset "tableNumber(101)", "IDSpeedGraph"
 turnoff
endin




; LFO
instr 11

 iLen = ftlen(giSpeedGraph)

 kCnt = 0
 while kCnt<iLen do
 
 kLFO   oscili     gkRange, (gkRate-0.7)/4, -1, rnd(1)
 tablew kLFO+0.5, kCnt, giSpeedGraph
 
 kCnt += 1
 od
 
 chnset "tableNumber(101)", "IDSpeedGraph"
 turnoff 
endin







; Follow
instr 12

 iLen = ftlen(gitableL)

 iLookahead = 0.005

 kCnt = 0
 while kCnt<iLen do

 aSig  tablei a(kCnt) - (i(gkListenAhead) * sr * 0.1), gitableL
 aFollow follow2 aSig, gkAttack, gkRelease
 aFollow limit   (aFollow^gkFollowShape)*(gkFollowSens*4), 0, 1
 tablew aFollow, a(kCnt) * ftlen(giSpeedGraph)/ftlen(gitableL), giSpeedGraph
 
 kCnt += 1
 od
 
 chnset "tableNumber(101)", "IDSpeedGraph"
 turnoff
endin





; Trigger
instr 13

 iLen = ftlen(gitableL)

 kCnt = 0
 while kCnt<iLen do

 aL   tablei a(kCnt) - (i(gkListenAheadTrig) * sr * 0.1), gitableL
 aR   tablei a(kCnt) - (i(gkListenAheadTrig) * sr * 0.1), gitableL
 aSig sum    aL,aR
 
 kWait    init     0
 
 aFollow  follow2  aSig, 0.001, 50
 kRMS     downsamp aFollow
 kThresh  scale    expcurve:k(gkAttack,8), 0.02, 0.001
 kDelRMS  vdel_k   kRMS, gkTimeTrig, 0.2
 kTrig    trigger  kRMS - kDelRMS, gkThreshTrig, 0
 
 kTrig    init     1

 if kTrig==1 && kWait>gkWaitTrig then
  kVal     random  0.5 - (gkSensTrig * 0.5), 0.5 + (gkSensTrig * 0.5)
  kWait    =       0
 endif
 kTrig     =       0
 ;aFollow follow2 aSig, gkAttack, gkRelease
 ;aFollow limit   (aFollow^gkFollowShape)*gkFollowSens, 0, 1
 tablew kVal, kCnt * ftlen(giSpeedGraph)/ftlen(gitableL), giSpeedGraph
 
 kCnt += 1
 kWait += 1
 od
 
 chnset "tableNumber(101)", "IDSpeedGraph"
 turnoff
endin





; record
instr 1000
  a1, a2         monitor
  SHome          chnget    "USER_HOME_DIRECTORY"
  ;ilen           strlen    gSfilepath                     ; Derive string length.
  ;SOutputName    strsub    gSfilepath,0,ilen-4            ; Remove ".wav"
  SOutputName    strcat    SHome,"/ScrubberPlayer"         ; Add suffix
  iDate          date
  SDate          sprintf    "%i",iDate
  SOutputName    strcat    SOutputName,SDate              ; Add date
  SOutputName    strcat    SOutputName,".wav"             ; Add extension
                 fout      SOutputName, 8, a1, a2

  if active:k(1+i(gkMode))==0 && timeinstk()>1 then
   chnset k(0),"RecOut"
   turnoff
  endif

endin





</CsInstruments>  

<CsScore>
i 1 0 10000
</CsScore>

</CsoundSynthesizer>
