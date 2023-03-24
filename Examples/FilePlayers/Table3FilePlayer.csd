
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; Table3FilePlayer.csd
; Written by Iain McCurdy, 2014

; Load a user selected sound file into a GEN 01 function table and plays it back using loscil3. 
; This file player is suited for polyphonic playback and is less well suited for the playback of very long sound files .
;                                 
; Loop points can be edited by using either the sliders 'Start' and 'End' or by clicking and dragging on the waveform view.
;  Edit mode is selected automatically according to the last method used.
; 
; 
; Things still to do:
;  Add loop direction function when using sliders to edit loop points
;  Portamento control for loop points when using sliders to edit loop points
;  Add instructions in viewer panel.
;  MIDI note loop play in click-and-drag mode not working properly
 
 
<Cabbage>
form caption("Table3 File Player") size(790,340), colour(0,0,0) pluginId("T3Pl"), guiRefresh(64)
image                    bounds(  0,  0,790,340), colour(30, 30, 70), outlineColour("White"), line(3), shape("sharp")    ; main panel colouration    
soundfiler               bounds(  5,  5,780,175), channel("beg","len"), identChannel("filer1"),  colour(0, 255, 255, 255), fontColour(160, 160, 160, 255), 
label bounds(6, 4, 560, 14), text(""), align(left), colour(0,0,0,0), fontColour(200,200,200), identChannel("stringbox")

filebutton bounds(  5,190, 80, 25), text("Open File","Open File"), fontColour("white") channel("filename"), shape("ellipse")
checkbox   bounds(  5,220, 95, 25), channel("PlayStop"), text("Play/Stop"), colour("yellow"), fontColour:0("white"), fontColour:1("white")

label      bounds(178,184, 45, 8), text("L   O   O   P"), fontColour("white")

groupbox   bounds(100,200,100, 50), plant("looping"), text("Looping Mode"), fontColour("white"){
combobox   bounds( 10, 25, 80, 20), channel("mode"), items("Forward", "Backward", "Fwd./Bwd."), value(1), fontColour("white")
}
rslider    bounds(205,195, 60, 60), channel("LoopStart"), range(0, 1, 0),                   colour(60, 60,100), text("Start"),     textColour("white"), trackerColour(210,210,250)
rslider    bounds(260,195, 60, 60), channel("LoopEnd"),   range(0, 1, 1),                   colour(60, 60,100), text("End"),       textColour("white"), trackerColour(210,210,250)
rslider    bounds(315,195, 60, 60), channel("Portamento"),   range(0,0.1,0.01),                   colour(60, 60,100), text("Portamento"),       textColour("white"), trackerColour(210,210,250)

line       bounds(375,190,  2, 65), colour("Grey")

label      bounds(409,184, 55, 8), text("S   P   E   E   D"), fontColour("white")
rslider    bounds(380,195, 60, 60), channel("transpose"), range(-24, 24, 0,1,1),            colour(60, 60,100), text("Transpose"), textColour("white"), trackerColour(210,210,250)
rslider    bounds(435,195, 60, 60), channel("speed"),     range(0, 4.00, 1, 0.5, 0.001),    colour(60, 60,100), text("Speed"),     textColour("white"), trackerColour(210,210,250)

line       bounds(495,190,  2, 65), colour("Grey")

label      bounds(511,184, 90, 8), text("E   N   V   E   L   O   P   E"), fontColour("white")
rslider    bounds(500,195, 60, 60), channel("AttTim"),    range(0, 5, 0.01, 0.5, 0.001),    colour(60, 60,100), text("Att.Tim"),   textColour("white"), trackerColour(210,210,250)
rslider    bounds(555,195, 60, 60), channel("RelTim"),    range(0.01, 5, 0.05, 0.5, 0.001), colour(60, 60,100), text("Rel.Tim"),   textColour("white"), trackerColour(210,210,250)
line       bounds(615,190,  2, 65), colour("Grey")

label      bounds(636,184, 140, 8), text("C   O   N   T   R   O   L"), fontColour("white")
rslider    bounds(620,195, 60, 60), channel("MidiRef"),   range(0,127,60, 1, 1),            colour(60, 60,100), text("MIDI Ref."), textColour("white"), trackerColour(210,210,250)
rslider    bounds(675,195, 60, 60), channel("PchBnd"),    range(  0,  24.00, 2, 1,0.1),     colour(60, 60,100), text("PchBnd"),     textColour("white"), trackerColour(210,210,250)
rslider    bounds(730,195, 60, 60), channel("level"),     range(  0,  3.00, 1, 0.5),        colour(60, 60,100), text("Level"),     textColour("white"), trackerColour(210,210,250)

keyboard bounds(5,260, 780, 75)
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-n -+rtmidi=NULL -M0 -dm0
</CsOptions>

<CsInstruments>

; sr set by host
ksmps = 32
nchnls = 2
0dbfs = 1

             massign 0,3
gichans      init    0
giFileLen    init    0
giReady      init    0
gSfilepath   init    ""
gkTabLen     init    2
gitri        ftgen   0,0,131072,7,0,131072/2,1,131072/2,0
gkEditMode   init    2    ; 1 = CAD 2 = sliders

opcode FileNameFromPath,S,S           ; Extract a file name (as a string) from a full path (also as a string)
 Ssrc      xin                        ; Read in the file path string
 icnt      strlen    Ssrc             ; Get the length of the file path string
 LOOP:                                ; Loop back to here when checking for a backslash
 iasc      strchar   Ssrc, icnt       ; Read ascii value of current letter for checking
 if iasc==92 igoto ESCAPE             ; If it is a backslash, escape from loop
           loop_gt   icnt,1,0,LOOP    ; Loop back and decrement counter which is also used as an index into the string
 ESCAPE:                              ; Escape point once the backslash has been found
 Sname     strsub    Ssrc, icnt+1, -1 ; Create a new string of just the file name
           xout      Sname            ; Send it back to the caller instrument
endop

instr    1    ; Read in widgets

 gkloop          chnget    "loop"

 gkLoopStart     chnget    "beg"        ; Click-and-drag
 gkLoopLen       chnget    "len"
 
 
 gkLoopStart2    chnget    "LoopStart"    ; Sliders
 gkLoopEnd2      chnget    "LoopEnd"
 gkPortamento    chnget    "Portamento"

 gkMOUSE_DOWN_LEFT    chnget    "MOUSE_DOWN_LEFT"
 gkMOUSE_DOWN_RIGHT   chnget    "MOUSE_DOWN_RIGHT"            ; Read in mouse left click status

 if changed(gkLoopStart,gkLoopLen)==1 then
  gkEditMode    =    1    ; Click-and-drag
 elseif  changed(gkLoopStart2,gkLoopEnd2)==1||gkMOUSE_DOWN_RIGHT==1 then
  gkEditMode    =    2    ; sliders
 endif
 
 gkPlayStop    chnget    "PlayStop"
 gktranspose   chnget    "transpose"
 gkspeed       chnget    "speed"
 gkPchBndRng   chnget     "PchBnd"
 gklevel       chnget    "level"
 gkmode        chnget    "mode"
 
 gSfilepath    chnget    "filename"
 kNewFileTrg    changed    gSfilepath        ; if a new file is loaded generate a trigger
 if kNewFileTrg==1 then                    ; if a new file has been loaded...
  event    "i",99,0,0                        ; call instrument to update sample storage function table 
 endif  
 
 ktrig    trigger    gkPlayStop,0.5,0        ; if play button changes to 'play', generate a trigger
 schedkwhen    ktrig,0,0,2,0,-1            ; start instr 2 playing a held note

 ktrig1    changed    gktranspose                ; if 'transpose' button is changed generate a '1' trigger
 ktrig2    changed    gkspeed                    ; if 'speed' button is changed generate a '1' trigger
 
 if ktrig1==1 then                                ; if transpose control has been changed...
  chnset    semitone(gktranspose),"speed"        ; set speed according to transpose value
 elseif ktrig2==1 then                            ; if speed control has been changed...
  chnset    log2(abs(gkspeed))*12,"transpose"    ; set transpose control according to speed value
 endif

  /* MOUSE SCRUBBING */
 kStartScrub        trigger    gkMOUSE_DOWN_RIGHT,0.5,0
 if gkMOUSE_DOWN_RIGHT==1 then
  if kStartScrub==1 then 
   reinit RAMP_FUNC
  endif
  RAMP_FUNC:
  krampup    linseg    0,0.001,1
  rireturn
  kMOUSE_X    chnget    "MOUSE_X"
  kMOUSE_Y    chnget    "MOUSE_Y"
  kMOUSE_X    =    (kMOUSE_X - 5) / 730
  kMOUSE_Y    portk    1 - ((kMOUSE_Y - 5) / 175), krampup*0.05        ; SOME SMOOTHING OF DENSITY CHANGES VIA THE MOUSE ENHANCES PERFORMANCE RESULTS. MAKE ANY ADJUSTMENTS WITH ADDITIONAL CONSIDERATION OF guiRefresh VALUE 
  gkLoopStart2        limit    kMOUSE_X,0,1
  gkLoopEnd2        limit    ((kMOUSE_Y^2) * (1-kMOUSE_X)) + kMOUSE_X, 0, 1
  gktranspose    =    ((kMOUSE_Y*2)-1)*gktranspose    ;, -gktranspose, gktranspose
  schedkwhen    kStartScrub,0,0,2,0,-1
 else
  gkptr        chnget    "ptr"
  gklevel    chnget    "level"
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
 giReady     =    1                    ; if no string has yet been loaded giReady will be zero
 Smessage sprintfk "file(%s)", gSfilepath            ; print file to viewer
 chnset Smessage, "filer1"    

 /* WRITE FILE NAME TO GUI */
 Sname FileNameFromPath    gSfilepath                ; Call UDO to extract file name from the full path
 Smessage sprintfk "text(%s)",Sname
 chnset Smessage, "stringbox"

endin




instr    2    ; Sample triggered by 'play/stop' button
 if gkPlayStop==0&&gkMOUSE_DOWN_RIGHT==0 then
  turnoff
 endif

 if giReady = 1 then                                    ; i.e. if a file has been loaded

  iAttTim      chnget    "AttTim"                       ; read in widgets
  iRelTim      chnget    "RelTim"
  if iAttTim>0 then                                     ; is amplitude envelope attack time is greater than zero...
   kenv        linsegr   0,iAttTim,1,iRelTim,0          ; create an amplitude envelope with an attack, a sustain and a release segment (senses realtime release)
  else
   kenv        linsegr   1,iRelTim,0                    ; create an amplitude envelope with a sustain and a release segment (senses realtime release)
  endif
  kenv         expcurve  kenv,8                         ; remap amplitude value with a more natural curve
  aenv         interp    kenv                           ; interpolate and create a-rate envelope
  kporttime    linseg    0,0.001,1                      ; portamento time function. (Rises quickly from zero to a held value.)
  kspeed       portk     gkspeed,kporttime*gkPortamento ; apply portamento smoothing to changes in speed
  klevel       portk     gklevel,kporttime*0.1          ; apply portamento smoothing to changes

  if gkEditMode==1 then                                 ; click and drag edit mode
   gkLoopLen   limit     gkLoopLen,1,giFileSamps        ; prevent loop lengths of zero
   krate       =         (kspeed * sr) / gkLoopLen
   arate       interp    krate
   if gkmode==1 then
    aphasor    phasor    arate
   elseif gkmode==2 then
    aphasor    phasor    -arate
   else
    aphasor    poscil    1,-arate*0.5,gitri
   endif
   rireturn
   aLoopStart  interp    gkLoopStart
   aLoopEnd    interp    gkLoopLen
   aphasor     =         (aphasor*aLoopEnd)+aLoopStart
   if gichans==1 then                        ; if mono...
    a1         table3    aphasor, gitableL
      outs    a1*aenv*klevel, a1*aenv*klevel            ; send mono audio to both outputs 
   elseif gichans==2 then                    ; otherwise, if stereo...
    a1         table3    aphasor, gitableL
    a2         table3    aphasor, gitableR
      outs    a1*aenv*klevel, a2*aenv*klevel            ; send stereo signal to outputs
   endif               
  
  elseif gkEditMode==2 then        ; sliders edit mode
   kLoopStart  portk     gkLoopStart2,kporttime*gkPortamento
   kLoopEnd    portk     gkLoopEnd2,kporttime*gkPortamento
   kLoopEnd    =         (kLoopEnd=kLoopStart?kLoopEnd+0.001:kLoopEnd)
   
   kLoopLen    =         abs(kLoopEnd-kLoopStart)
   kdir        =         (kLoopEnd>kLoopStart?1:-1)
   
   krate       divz      kspeed, kLoopLen*giFileLen, 1
   arate       interp    krate
   if gkmode==1 then                             ; fwd
    aphasor    phasor    arate*kdir
   elseif gkmode==2 then                         ; bwd
    aphasor    phasor    -arate*kdir
   else                                          ; fwd-bwd
    aphasor    poscil    1,-arate*0.5*kdir,gitri
   endif
   ;aphasor     phasor    arate*kdir
   kLoopStart  min       kLoopStart,kLoopEnd
   aLoopStart  interp    kLoopStart
   aLoopLen    interp    kLoopLen
   aphasor     =         (aphasor*aLoopLen)+aLoopStart
   
   if gichans==1 then                                              ; if mono...
    a1         table3    aphasor, gitableL, 1
               outs      a1*aenv*klevel, a1*aenv*klevel            ; send mono audio to both outputs 
   elseif gichans==2 then                                          ; otherwise, if stereo...
    a1         table3    aphasor, gitableL, 1
    a2         table3    aphasor, gitableR, 1
               outs      a1*aenv*klevel, a2*aenv*klevel            ; send stereo signal to outputs
   endif               
   
   
  endif
  
 endif

 ; print scrubber
 if(metro(20)==1) then
  kscrubber    downsamp    aphasor
  Smessage sprintfk "scrubberPosition(%d)", kscrubber
  chnset Smessage, "filer1"
 endif

endin




instr    3    ; sample triggered by midi note
 kporttime    linseg     0,0.001,0.05              ; portamento time function. (Rises quickly from zero to a held value.)
 icps          cpsmidi                             ; read in midi note data as cycles per second
 iamp          ampmidi    1                        ; read in midi velocity (as a value within the range 0 - 1)
 iMidiRef      chnget     "MidiRef"
 kPchBnd       pchbend   0, 1                       ; read in pitch bend
 kPchBnd       *=        gkPchBndRng
 kPchBnd       portk     kPchBnd, kporttime

 if giReady = 1 then                                ; i.e. if a file has been loaded
  iAttTim      chnget    "AttTim"                   ; read in widgets
  iRelTim      chnget    "RelTim"
  if iAttTim>0 then                                 ; is amplitude envelope attack time is greater than zero...
   kenv        linsegr    0,iAttTim,1,iRelTim,0     ; create an amplitude envelope with an attack, a sustain and a release segment (senses realtime release)
  else
   kenv        linsegr    1,iRelTim,0               ; create an amplitude envelope with a sustain and a release segment (senses realtime release)
  endif
  kenv         expcurve   kenv,8                    ; remap amplitude value with a more natural curve
  aenv         interp     kenv                      ; interpolate and create a-rate envelope
  ispeed       =          icps/cpsmidinn(iMidiRef)  ; derive playback speed from note played in relation to a reference note (MIDI note 60 / middle C)
  klevel       portk      gklevel,kporttime         ; apply portamento smoothing to changes in level
    

  if gkEditMode==1 then                             ; click and drag edit mode
   gkLoopLen   limit      gkLoopLen,1,giFileSamps   ; prevent loop lengths of zero
   krate       =          ispeed * sr / gkLoopLen
   arate       interp     krate
   if gkmode==1 then
    aphasor    phasor     arate
   elseif gkmode==2 then
    aphasor    phasor     -arate
   else
    aphasor    poscil     1,-arate*0.5,gitri
   endif
   aLoopStart  interp     gkLoopStart
   aLoopEnd    interp     gkLoopLen
   aphasor     =          (aphasor*aLoopEnd)+aLoopStart
   if gichans==1 then                        ; if mono...
    a1         table3     aphasor, gitableL
               outs       a1*aenv*klevel*iamp*(1-gkMOUSE_DOWN_LEFT), a1*aenv*klevel*iamp*(1-gkMOUSE_DOWN_LEFT)    ; send mono audio to both outputs 
   elseif gichans==2 then                    ; otherwise, if stereo...
    a1         table3     aphasor, gitableL
    a2         table3     aphasor, gitableR
               outs       a1*aenv*klevel*iamp*(1-gkMOUSE_DOWN_LEFT), a2*aenv*klevel*iamp*(1-gkMOUSE_DOWN_LEFT)    ; send stereo signal to outputs
   endif

  elseif gkEditMode==2 then        ; sliders edit mode
   kLoopStart   portk     gkLoopStart2,kporttime
   kLoopEnd     portk     gkLoopEnd2,kporttime
   kLoopEnd     =         (kLoopEnd=kLoopStart?kLoopEnd+0.001:kLoopEnd)
   kLoopLen     =         abs(kLoopEnd-kLoopStart)
   kdir         =         (kLoopEnd>kLoopStart?1:-1)
   krate        divz      ispeed, kLoopLen*giFileLen, 1
   arate        interp    krate
   if gkmode==1 then                             ; fwd
    aphasor     phasor    arate*kdir
   elseif gkmode==2 then                         ; bwd
    aphasor     phasor    -arate*kdir
   else                                          ; fwd-bwd
    aphasor     poscil    1,-arate*0.5*kdir,gitri
   endif
   kLoopStart   min       kLoopStart,kLoopEnd
   aLoopStart   interp    kLoopStart
   aLoopLen     interp    kLoopLen
   aphasor      =         (aphasor*aLoopLen)+aLoopStart
   
   if gichans==1 then                        ; if mono...
    a1    table3    aphasor, gitableL, 1
      outs    a1*aenv*klevel*iamp, a1*aenv*klevel*iamp    ; send mono audio to both outputs 
   elseif gichans==2 then                    ; otherwise, if stereo...
    a1    table3    aphasor, gitableL, 1
    a2    table3    aphasor, gitableR, 1
      outs    a1*aenv*klevel*iamp, a2*aenv*klevel*iamp    ; send stereo signal to outputs
   endif               
  endif
 endif

 if active(p1)==1 then                        ; only print scrubber for first note
  if(metro(20)==1) then
   kscrubber    downsamp    aphasor
   Smessage     sprintfk    "scrubberPosition(%d)", kscrubber
                chnset      Smessage, "filer1"
  endif
 endif
endin
 
</CsInstruments>  

<CsScore>
i 1 0 z
</CsScore>

</CsoundSynthesizer>