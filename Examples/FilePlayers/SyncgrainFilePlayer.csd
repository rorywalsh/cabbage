
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; SyncgrainFilePlayer.csd
; Written by Iain McCurdy, 2014
; 
; The internal pointer used by syncgrain to track progress through the sound file is affected by grain size and density as well as speed 
;  so on accaount of this complication a scrubber line in the waveform view is not yet implemented.

<Cabbage>
form caption("Syncgrain File Player") size(965,380), colour(0,0,0) pluginId("SGFP")
image                       bounds(  0,  0,965,380), colour( 90, 60, 30), outlineColour("White"), shape("sharp"), line(3)    
soundfiler                  bounds(  5,  5,955,175), channel("beg","len"), identChannel("filer1"),  colour(0, 255, 255, 255), fontColour(160, 160, 160, 255), 
keyboard                    bounds(  5,295,955, 80)

image                       bounds(  0,180,955,190), colour(0,0,0,0), outlineColour("white"), line(0), shape("sharp"), plant("controls"){
filebutton bounds(  5, 20, 80, 25), text("Open File","Open File"), fontColour("white") channel("filename"), shape("ellipse")
checkbox   bounds(  5, 60, 95, 25), channel("PlayStop"), text("Play/Stop"), fontColour:0("white"), fontColour:1("white")
label      bounds(280,  4, 70, 9), text("G   R   A   I   N   S"), fontColour("silver")
rslider    bounds( 90, 15, 90, 90), channel("density"),   range( 0.5,400.00,40, 0.5),          colour( 70, 40, 10), text("Density"),     textColour("white"), trackerColour(190,170,130), outlineColour(100,100,100), valueTextBox(1)
rslider    bounds(150, 15, 90, 90), channel("DensOS"),     range( 0, 5.00, 0),                 colour( 70, 40, 10), text("Dens.OS"),     textColour("white"), trackerColour(190,170,130), outlineColour(100,100,100), valueTextBox(1)
rslider    bounds(210, 15, 90, 90), channel("grsize"),   range( 0.001,1.00, 0.1, 0.5),        colour( 70, 40, 10), text("Size"),        textColour("white"), trackerColour(190,170,130), outlineColour(100,100,100), valueTextBox(1)
rslider    bounds(270, 15, 90, 90), channel("SizeOS"),   range( 0, 5.00, 0, 0.5),              colour( 70, 40, 10), text("Size OS"),     textColour("white"), trackerColour(190,170,130), outlineColour(100,100,100), valueTextBox(1)
rslider    bounds(330, 15, 90, 90), channel("transpose"), range(-24, 24, 0,1,1),               colour( 70, 40, 10), text("Transpose"),   textColour("white"), trackerColour(190,170,130), outlineColour(100,100,100), valueTextBox(1)
rslider    bounds(390, 15, 90, 90), channel("TransposeOS"), range(0, 12.00, 0),                colour( 70, 40, 10), text("Trans.OS"),    textColour("white"), trackerColour(190,170,130), outlineColour(100,100,100), valueTextBox(1)
rslider    bounds(450, 15, 90, 90), channel("speed"),     range( -2.00, 2.00, 1),              colour( 70, 40, 10), text("Speed"),       textColour("white"), trackerColour(190,170,130), outlineColour(100,100,100), valueTextBox(1)
label      bounds(530, 12, 74, 10), text("S h a p e"), fontColour("white"), align("centre")
combobox   bounds(530, 22, 74, 18), channel("shape"), items("Hanning", "Perc.1", "Perc.2", "Gate", "Rev.Perc.1", "Rev.Perc.2"), value(1),fontColour("white")
checkbox   bounds(530, 45,100, 18), channel("freeze"), text("Freeze"), colour("LightBlue"), fontColour:0("white"), fontColour:1("white")

line       bounds(615, 10,  2, 95), colour("Grey")

label      bounds(620,  4,145, 9), text("E   N   V   E   L   O   P   E"), fontColour("silver"), align("centre")
rslider    bounds(620, 15, 90, 90), channel("AttTim"),    range(0, 5, 0, 0.5, 0.001),       colour( 70, 40, 10), text("Att.Tim"),   textColour("white"), trackerColour(190,170,130), outlineColour(100,100,100), valueTextBox(1)
rslider    bounds(680, 15, 90, 90), channel("RelTim"),    range(0.01, 5, 0.05, 0.5, 0.001), colour( 70, 40, 10), text("Rel.Tim"),   textColour("white"), trackerColour(190,170,130), outlineColour(100,100,100), valueTextBox(1)

line       bounds(760, 10,  2, 95), colour("Grey")

label      bounds(765,  4,200, 9), text("C   O   N   T   R   O   L"), fontColour("silver"), align("centre")
rslider    bounds(760, 15, 90, 90), channel("MidiRef"),   range(0,127,60, 1, 1),            colour( 70, 40, 10), text("MIDI Ref."), textColour("white"), trackerColour(190,170,130), outlineColour(100,100,100), valueTextBox(1)
rslider    bounds(820, 15, 90, 90), channel("PchBnd"),     range(  0,  24, 2, 1,0.1),       colour( 70, 40, 10), text("Pch.Bend"),     textColour("white"), trackerColour(190,170,130), outlineColour(100,100,100), valueTextBox(1)
rslider    bounds(880, 15, 90, 90), channel("level"),     range(  0,  3.00, 1, 0.5),        colour( 70, 40, 10), text("Level"),     textColour("white"), trackerColour(190,170,130), outlineColour(100,100,100), valueTextBox(1)

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

massign    0,3
gichans        init    0
giReady        init    0
gSfilepath    init    ""

; WINDOWING FUNCTIONS USED TO DYNAMICALLY SHAPE THE GRAINS
; NUM | INIT_TIME | SIZE | GEN_ROUTINE | PARTIAL_NUM | STRENGTH | PHASE
; GRAIN ENVELOPE WINDOW FUNCTION TABLES:
giwfn1    ftgen    0,  0, 131072,  20,   2, 1                     ; HANNING
giwfn2    ftgen    0,  0, 131072,  7,    0, 3072,   1, 128000,    0        ; PERCUSSIVE - STRAIGHT SEGMENTS
giwfn3    ftgen    0,  0, 131072,  5, .001, 3072,   1, 128000, .001        ; PERCUSSIVE - EXPONENTIAL SEGMENTS
giwfn4    ftgen    0,  0, 131072,  7,    0, 1536,   1, 128000,    1, 1536, 0    ; GATE - WITH DE-CLICKING RAMP UP AND RAMP DOWN SEGMENTS
giwfn5    ftgen    0,  0, 131072,  7,    0, 128000, 1, 3072,      0        ; REVERSE PERCUSSIVE - STRAIGHT SEGMENTS
giwfn6    ftgen    0,  0, 131072,  5, .001, 128000, 1, 3072,   .001        ; REVERSE PERCUSSIVE - EXPONENTIAL SEGMENTS

instr    1
 gkloop         chnget     "loop"
 gkPlayStop     chnget     "PlayStop"
 gktranspose    chnget     "transpose"
 gkTransposeOS  chnget     "TransposeOS"
 gkdensity      chnget     "density"
 gkDensOS       chnget     "DensOS"
 gkgrsize       chnget     "grsize"
 gkSizeOS       chnget     "SizeOS"
 gkshape        chnget     "shape"
 gkspeed        chnget     "speed"
 gklevel        chnget     "level"
 gkfreeze       chnget     "freeze"
 gkfreeze       =          1-gkfreeze
 gkPchBndRng    chnget     "PchBnd"
        
 gSfilepath     chnget      "filename"
 kNewFileTrg    changed     gSfilepath  ; if a new file is loaded generate a trigger
 if kNewFileTrg==1 then                 ; if a new file has been loaded...
                event       "i",99,0,0                   ; call instrument to update sample storage function table 
 endif  

 ktrig          trigger     gkPlayStop,0.5,0
                schedkwhen  ktrig,0,0,2,0,-1
endin

instr    99    ; load sound file
 gichans        filenchnls  gSfilepath            ; derive the number of channels (mono=1,stereo=2) in the sound file
 gitableL       ftgen       1,0,0,1,gSfilepath,0,0,1
 if gichans==2 then
  gitableR      ftgen       2,0,0,1,gSfilepath,0,0,2
 endif
 giReady        =           1                    ; if no string has yet been loaded giReady will be zero
 Smessage       sprintfk    "file(%s)", gSfilepath            ; print sound file to viewer
                chnset      Smessage, "filer1"
endin

instr    2    ; triggered by 'play/stop' button
 if gkPlayStop==0 then
                turnoff
 endif
 if giReady = 1 then                                ; i.e. if a file has been loaded
  iAttTim       chnget      "AttTim"                ; read in widgets
  iRelTim       chnget      "RelTim"
  if iAttTim>0 then                                 ; is amplitude envelope attack time is greater than zero...
   kenv         linsegr     0,iAttTim,1,iRelTim,0   ; create an amplitude envelope with an attack, a sustain and a release segment (senses realtime release)
  else            
   kenv         linsegr     1,iRelTim,0             ; create an amplitude envelope with a sustain and a release segment (senses realtime release)
  endif
  kenv          expcurve    kenv,8                  ; remap amplitude value with a more natural curve
  aenv          interp      kenv                    ; interpolate and create a-rate envelope
  kporttime     linseg      0,0.001,0.05            ; portamento time function. (Rises quickly from zero to a held value.)
  kdensity      portk       gkdensity,kporttime     ; apply portamento smoothing to changes in speed
  kprate        portk       gkspeed,kporttime
  klevel        portk       gklevel,kporttime       ; apply portamento smoothing to changes in level

  kDensOS       gauss       gkDensOS
  kDensMlt      =           octave(kDensOS)
  kdensity      =           kdensity * kDensMlt
  
  ktranspose    portk       gktranspose,kporttime            ; 
  kTransposeOS  gauss       gkTransposeOS
  ktranspose    =           ktranspose + kTransposeOS
  
  kSizeOS       rand        gkSizeOS
  kgrsize       =           gkgrsize * octave(kSizeOS)
  
  giolaps       =           5000
  
  ktrig         changed     gkshape
  if ktrig==1 then
   reinit    UPDATE
  endif
  UPDATE:
  
  iwfn          =           i(gkshape) + giwfn1 - 1
  if gichans==1 then                        ; if mono...
   a1           syncgrain   klevel, kdensity, semitone(ktranspose), kgrsize, (kprate*gkfreeze)/(kdensity*kgrsize), gitableL, iwfn, giolaps
                outs        a1*aenv,a1*aenv                    ; send mono audio to both outputs 
  elseif gichans==2 then                        ; otherwise, if stereo...
   a1           syncgrain   klevel, kdensity, semitone(ktranspose), kgrsize, (kprate*gkfreeze)/(kdensity*kgrsize), gitableL, iwfn, giolaps
   a2           syncgrain   klevel, kdensity, semitone(ktranspose), kgrsize, (kprate*gkfreeze)/(kdensity*kgrsize), gitableR, iwfn, giolaps
                outs        a1*aenv,a2*aenv                    ; send stereo signal to outputs
  endif
  rireturn
 endif        
endin

instr    3
 icps           cpsmidi                              ; read in midi note data as cycles per second
 iamp           ampmidi     1                        ; read in midi velocity (as a value within the range 0 - 1)
 kPchBnd        pchbend     0, 1                     ; read in pitch bend
 kPchBnd        *=          gkPchBndRng
 iAttTim        chnget      "AttTim"                 ; read in widgets
 iRelTim        chnget      "RelTim"
 iMidiRef       chnget      "MidiRef"
 iFrqRatio      =           icps/cpsmidinn(iMidiRef) ; derive playback speed from note played in relation to a reference note (MIDI note 60 / middle C)

 if giReady = 1 then                                 ; i.e. if a file has been loaded
  iAttTim       chnget      "AttTim"                 ; read in widgets
  iRelTim       chnget      "RelTim"
  if iAttTim>0 then                                  ; is amplitude envelope attack time is greater than zero...
   kenv         linsegr     0,iAttTim,1,iRelTim,0    ; create an amplitude envelope with an attack, a sustain and a release segment (senses realtime release)
  else            
   kenv         linsegr     1,iRelTim,0              ; create an amplitude envelope with a sustain and a release segment (senses realtime release)
  endif
  kenv          expcurve    kenv,8                   ; remap amplitude value with a more natural curve
  aenv          interp      kenv                     ; interpolate and create a-rate envelope
  kporttime     linseg      0,0.001,0.05             ; portamento time function. (Rises quickly from zero to a held value.)
  kdensity      portk       gkdensity,kporttime      ; apply portamento smoothing to changes in speed
  kprate        portk       gkspeed,kporttime
  klevel        portk       gklevel,kporttime        ; apply portamento smoothing to changes in level
  kPchBnd       portk       kPchBnd, kporttime
 
  kDensOS       gauss       gkDensOS
  kDensMlt      =           octave(kDensOS)
  kdensity      =           kdensity * kDensMlt
    
  kSizeOS       rand        gkSizeOS
  kgrsize       =           gkgrsize * octave(kSizeOS)

  giolaps       =           5000
  
  ktrig         changed     gkshape
  if ktrig==1 then
   reinit    UPDATE
  endif
  UPDATE:
  
  iwfn        =             i(gkshape) + giwfn1 - 1
  if gichans==1 then                                           ; if mono...
   a1         syncgrain     klevel*iamp, kdensity, iFrqRatio*semitone:k(kPchBnd), kgrsize, (kprate*gkfreeze)/(kdensity*kgrsize), gitableL, iwfn, giolaps
              outs          a1*aenv,a1*aenv                    ; send mono audio to both outputs 
  elseif gichans==2 then                                       ; otherwise, if stereo...
   a1         syncgrain     klevel*iamp, kdensity, iFrqRatio*semitone:k(kPchBnd), kgrsize, (kprate*gkfreeze)/(kdensity*kgrsize), gitableL, iwfn, giolaps
   a2         syncgrain     klevel*iamp, kdensity, iFrqRatio*semitone:k(kPchBnd), kgrsize, (kprate*gkfreeze)/(kdensity*kgrsize), gitableR, iwfn, giolaps
              outs          a1*aenv,a2*aenv                    ; send stereo signal to outputs
  endif
  rireturn
 endif

endin

</CsInstruments>  

<CsScore>
i 1 0 z
</CsScore>

</CsoundSynthesizer>
