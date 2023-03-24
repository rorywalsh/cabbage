
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; LoscilFilePlayer.csd
; Written by Iain McCurdy, 2014
; 
; Load a user selected sound file into a GEN 01 function table and plays it back using loscil3. 
; This file player is best suited for polyphonic playback and is less well suited for the playback of very long sound files .
; 
; The sound file can be played back using the Play/Stop button (and the 'Transpose' / 'Speed' buttons to implement pitch/speed change)
;  or it can be played back using the MIDI keyboard.
; 
; The loop points can be set either by using the loop 'Start' and 'End' sliders or by clicking and dragging on the waveform view -
;  - loscil will take the values from the last control input moved.

<Cabbage>
form caption("Loscil File Player") size(685,340), colour(0,0,0) pluginId("Losc")
image                    bounds(  0,  0,685,340), colour(30, 70, 30), outlineColour("White"), line(3), shape("sharp")    ; main panel colouration    

soundfiler bounds(  5,  5,675,175), channel("beg","len"), identChannel("filer1"),  colour(0, 255, 255, 255), fontColour(160, 160, 160, 255), 

image    bounds(  0,180,685,160), colour(0,0,0,0), outlineColour("white"), line(2), shape("sharp"), plant("controls"){
filebutton bounds(  5, 10, 80, 25), text("Open File","Open File"), fontColour("white") channel("filename"), shape("ellipse")
checkbox   bounds(  5, 40, 95, 25), channel("PlayStop"), text("Play/Stop"), colour("yellow"), fontColour:0("white"), fontColour:1("white")

label      bounds(110, 12, 80, 12), text("Looping Mode"), fontColour("white")
combobox   bounds(110, 25, 80, 20), channel("loop"), items("None", "Forward", "Fwd./Bwd."), value(1), fontColour("white")

label      bounds(241,  4, 43, 8), text("L   O   O   P"), fontColour("white")
rslider    bounds(205, 15, 60, 60), channel("LoopStart"), range(0, 1, 0),                   colour( 50, 90, 50), text("Start"),     textColour("white")
rslider    bounds(260, 15, 60, 60), channel("LoopEnd"),   range(0, 1, 1),                   colour( 50, 90, 50), text("End"),       textColour("white")
line       bounds(320, 10,  2, 65), colour("Grey")

label      bounds(357,  4, 53, 8), text("S   P   E   E   D"), fontColour("white")
rslider    bounds(325, 15, 60, 60), channel("transpose"), range(-24, 24, 0,1,1),            colour( 50, 90, 50), text("Transpose"), textColour("white")
rslider    bounds(380, 15, 60, 60), channel("speed"),     range( 0, 4.00, 1, 0.5),          colour( 50, 90, 50), text("Speed"),     textColour("white")
line       bounds(440, 10,  2, 65), colour("Grey")

label      bounds(456,  4, 90, 8), text("E   N   V   E   L   O   P   E"), fontColour("white")
rslider    bounds(445, 15, 60, 60), channel("AttTim"),    range(0, 5, 0, 0.5, 0.001),       colour( 50, 90, 50), text("Att.Tim"),   textColour("white")
rslider    bounds(500, 15, 60, 60), channel("RelTim"),    range(0.01, 5, 0.05, 0.5, 0.001), colour( 50, 90, 50), text("Rel.Tim"),   textColour("white")
line       bounds(560, 10,  2, 65), colour("Grey")

label      bounds(580,  4, 80, 8), text("C   O   N   T   R   O   L"), fontColour("white")
rslider    bounds(565, 15, 60, 60), channel("MidiRef"),   range(0,127,60, 1, 1),            colour( 50, 90, 50), text("MIDI Ref."), textColour("white")
rslider    bounds(620, 15, 60, 60), channel("level"),     range(  0,  3.00, 1, 0.5),        colour( 50, 90, 50), text("Level"),     textColour("white")

keyboard bounds(5, 80, 675, 75)
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
0dbfs = 1

        massign    0,3
gichans        init    0
giReady        init    0
gSfilepath    init    ""
gkTabLen    init    2

instr    1
 gkloop        chnget    "loop"
 kLoopStart    chnget    "LoopStart"
 kLoopEnd    chnget    "LoopEnd"
 kLoopEnd    limit    kLoopEnd,kLoopStart+0.01,1    ; limit loop end to prevent crashes
 kbeg        chnget    "beg"                ; click and drag
 klen        chnget    "len"                ;  "

 ; loop points defined by sliders or click and drag...
 kTrigSlid    changed    kLoopStart,kLoopEnd    
 kTrigCAD    changed    kbeg,klen
 if kTrigSlid==1 then
  gkLoopStart    =    kLoopStart
  gkLoopEnd    =    kLoopEnd
 elseif kTrigCAD==1 then
  gkLoopStart    =    kbeg/gkTabLen
  gkLoopEnd    =    (kbeg+klen)/gkTabLen
 endif


 gkPlayStop    chnget    "PlayStop"
 gktranspose    chnget    "transpose"
 gkspeed    chnget    "speed"
 gklevel    chnget    "level"

 gSfilepath    chnget    "filename"
 kNewFileTrg    changed    gSfilepath        ; if a new file is loaded generate a trigger
 if kNewFileTrg==1 then                ; if a new file has been loaded...
  event    "i",99,0,0                ; call instrument to update sample storage function table 
 endif  
 
 ktrig    trigger    gkPlayStop,0.5,0        ; if play button changes to 'play', generate a trigger
 schedkwhen    ktrig,0,0,2,0,-1        ; start instr 2 playing a held note

 ktrig1    changed    gktranspose            ; if 'transpose' button is changed generate a '1' trigger
 ktrig2    changed    gkspeed                ; if 'speed' button is changed generate a '1' trigger
 
 if ktrig1==1 then                ; if transpose control has been changed...
  chnset    semitone(gktranspose),"speed"    ; set speed according to transpose value
 elseif ktrig2==1 then        ; if speed control has been changed...
  chnset    log2(gkspeed)*12,"transpose"    ; set transpose control according to speed value
 endif

endin

instr    99    ; load sound file
 gitable    ftgen    1,0,0,1,gSfilepath,0,0,0        ; load sound file into a GEN 01 function table 
 gichans    filenchnls    gSfilepath            ; derive the number of channels (mono=1,stereo=2) in the sound file
 giReady     =    1                    ; if no string has yet been loaded giReady will be zero
 gkTabLen    init        ftlen(gitable)/gichans        ; table length in sample frames
 Smessage sprintfk "file(%s)", gSfilepath            ; print sound file to viewer
 chnset Smessage, "filer1"    
endin

instr    2    ; sample triggered by 'play/stop' button
 if gkPlayStop==0 then
  turnoff
 endif
 ktrig changed    gkloop,gkLoopStart,gkLoopEnd
 if ktrig==1 then
  reinit RESTART
 endif
 RESTART:
 if giReady = 1 then                        ; i.e. if a file has been loaded
  iAttTim    chnget    "AttTim"                ; read in widgets
  iRelTim    chnget    "RelTim"
  if iAttTim>0 then                        ; is amplitude envelope attack time is greater than zero...
   kenv    linsegr    0,iAttTim,1,iRelTim,0                ; create an amplitude envelope with an attack, a sustain and a release segment (senses realtime release)
  else
   kenv    linsegr    1,iRelTim,0                    ; create an amplitude envelope with a sustain and a release segment (senses realtime release)
  endif
  kenv    expcurve    kenv,8                    ; remap amplitude value with a more natural curve
  aenv    interp        kenv                    ; interpolate and create a-rate envelope
  kporttime    linseg    0,0.001,0.05                ; portamento time function. (Rises quickly from zero to a held value.)
  kspeed    portk    gkspeed,kporttime            ; apply portamento smoothing to changes in speed
  klevel    portk    gklevel,kporttime            ; apply portamento smoothing to changes in level
  if gichans==1 then                        ; if mono...
   a1    loscil3    klevel,kspeed,gitable,1,i(gkloop)-1,nsamp(gitable)*i(gkLoopStart),nsamp(gitable)*i(gkLoopEnd)    ; use a mono loscil3
     outs    a1*aenv,a1*aenv                    ; send mono audio to both outputs 
  elseif gichans==2 then                        ; otherwise, if stereo...
   a1,a2    loscil3    klevel,kspeed,gitable,1,i(gkloop)-1,nsamp(gitable)*i(gkLoopStart),nsamp(gitable)*i(gkLoopEnd)    ; use stereo loscil3
     outs    a1*aenv,a2*aenv                    ; send stereo signal to outputs
  endif               
 endif
endin

instr    3    ; sample triggered by midi note
 icps    cpsmidi                            ; read in midi note data as cycles per second
 iamp    ampmidi    1                        ; read in midi velocity (as a value within the range 0 - 1)
 iMidiRef    chnget    "MidiRef"

 if giReady = 1 then                        ; i.e. if a file has been loaded
  iAttTim    chnget    "AttTim"                ; read in widgets
  iRelTim    chnget    "RelTim"
  if iAttTim>0 then                        ; is amplitude envelope attack time is greater than zero...
   kenv    linsegr    0,iAttTim,1,iRelTim,0                ; create an amplitude envelope with an attack, a sustain and a release segment (senses realtime release)
  else
   kenv    linsegr    1,iRelTim,0                    ; create an amplitude envelope with a sustain and a release segment (senses realtime release)
  endif
  kenv    expcurve    kenv,8                    ; remap amplitude value with a more natural curve
  aenv    interp        kenv                    ; interpolate and create a-rate envelope
  kporttime    linseg    0,0.001,0.05                ; portamento time function. (Rises quickly from zero to a held value.)
  ispeed    =    icps/cpsmidinn(iMidiRef)    ; derive playback speed from note played in relation to a reference note (MIDI note 60 / middle C)
  klevel    portk    gklevel,kporttime        ; apply portamento smoothing to changes in level
  if gichans==1 then                        ; if mono...
   a1    loscil3    klevel*aenv*iamp,ispeed,gitable,1,i(gkloop)-1,nsamp(gitable)*i(gkLoopStart),nsamp(gitable)*i(gkLoopEnd)    ; use a mono loscil3
     outs    a1,a1                        ; send mono audio to both outputs 
  elseif gichans==2 then                        ; otherwise, if stereo...
   a1,a2    loscil3    klevel*aenv*iamp,ispeed,gitable,1,i(gkloop)-1,nsamp(gitable)*i(gkLoopStart),nsamp(gitable)*i(gkLoopEnd)    ; use stereo loscil3
     outs    a1,a2                        ; send stereo signal to outputs
  endif
 endif
endin
 
</CsInstruments>  

<CsScore>
i 1 0 [60*60*24*7]
</CsScore>

</CsoundSynthesizer>
