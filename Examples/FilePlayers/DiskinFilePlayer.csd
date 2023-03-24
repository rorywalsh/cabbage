
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; DiskinFilePlayer.csd
; Written by Iain McCurdy

; Plays a user selected sound file using diskin2 opcode. 
; This file player is best suited for the playback of very long sound files and is less well suited for polyphonic playback.

; The sound file can be played back using the Play/Stop button (and the 'Transpose' / 'Speed' buttons to implement pitch/speed change)
;  or it can be played back using the MIDI keyboard.

; Note that for 'reverse' to be effective either 'loop' needs to be active or inskip needs to be something other than zero

; The stretch function works by reducing the speed during 'silent' sections. 
; 'Threshold' defines the RMS value beneath which audio is regarded as silence.
; During 'silent' sections, audio will be muted completely and 'Str.Ratio' will be multipled to the main 'Speed' 

<Cabbage>
form caption("Diskin File Player") size(1025, 310), pluginId("DkPl")
image                    bounds(  0,  0,1025, 310), colour( 70, 30, 30), shape("sharp"), outlineColour("White"), line(3)    ; main panel colouration    

soundfiler           bounds(  5,  5,1015,140), channel("beg","len"), identChannel("filer1"),  colour(0, 255, 255, 255), fontColour(160, 160, 160, 255)

label bounds(6, 4, 560, 14), text(""), align(left), colour(0,0,0,0), fontColour(200,200,200), identChannel("stringbox")

image bounds(0,150,1025,160), colour(0,0,0,0), plant("controls"){
filebutton bounds(  5,  5, 80, 25), text("Open File","Open File"), fontColour("white") channel("filename"), shape("ellipse")
checkbox   bounds(  5, 40, 95, 25), channel("PlayStop"), text("Play/Stop"), colour("lime"), fontColour:0("white"), fontColour:1("white")
checkbox   bounds(105,  5,100, 15), channel("loop"), text("Loop On/Off"), colour("yellow"), fontColour:0("white"), fontColour:1("white")
checkbox   bounds(105, 22,100, 15), channel("reverse"), text("Reverse"), colour("yellow"), fontColour:0("white"), fontColour:1("white")
label      bounds(118, 40, 75, 12), text("Interpolation"), fontColour("white")
combobox   bounds(105, 53,100, 20), channel("interp"), items("No interp.", "Linear", "Cubic", "Point Sinc"), value(3), fontColour("white")
rslider    bounds(215,  5, 70, 70), channel("transpose"), range(-48, 24, 0,1,1),            colour( 90, 50, 50), trackerColour("silver"), text("Transpose"), textColour("white")
rslider    bounds(280,  5, 70, 70), channel("speed"),     range( -4,  4.00, 1),             colour( 90, 50, 50), trackerColour("silver"), text("Speed"),     textColour("white")
rslider    bounds(345,  5, 70, 70), channel("inskip"),    range(  0,  1.00, 0),             colour( 90, 50, 50), trackerColour("silver"), text("In Skip"),   textColour("white")
rslider    bounds(410,  5, 70, 70), channel("AttTim"),    range(0, 5, 0, 0.5, 0.001),       colour( 90, 50, 50), trackerColour("silver"), text("Att.Tim"),   textColour("white")
rslider    bounds(475,  5, 70, 70), channel("RelTim"),    range(0.01, 5, 0.05, 0.5, 0.001), colour( 90, 50, 50), trackerColour("silver"), text("Rel.Tim"),   textColour("white")
rslider    bounds(540,  5, 70, 70), channel("MidiRef"),   range(0,127,60, 1, 1),            colour( 90, 50, 50), trackerColour("silver"), text("MIDI Ref."), textColour("white")
rslider    bounds(605,  5, 70, 70), channel("level"),     range(  0,  3.00, 1, 0.5),        colour( 90, 50, 50), trackerColour("silver"), text("Level"),     textColour("white")
}

image      bounds(675,155,140, 65), colour(0,0,0,0), outlineColour("silver"), outlineThickness(1), shape("sharp"), plant("stretch") {
checkbox   bounds(  5,  5, 80, 15), channel("StretchOnOff"), text("Stretch"), fontColour("white"), popupText("Reduce speed during silences")
nslider  bounds(  5, 25, 60, 30), channel("threshold"), range(0,1,0.005,1,0.001),  colour( 90, 50, 50), text("Threshold"), textColour("white")
nslider  bounds( 75, 25, 60, 30), channel("stretchratio"), range(0.01,8.00,0.25,1,0.01),  colour( 90, 50, 50), trackerColour("silver"), text("Str.Ratio"), textColour("white")
}

image bounds(820, 155,200, 65), colour(0,0,0,0), outlineColour("silver"), outlineThickness(1), shape("sharp"), plant("speedmod") {
label      bounds(  0,  5,200, 12), text("Speed Modulation"), fontColour("white")
nslider  bounds(  5, 25, 60, 30), channel("ModRange"), range(0,4,0,1,0.001),  colour( 90, 50, 50), text("Mod.Range"), textColour("white")
nslider  bounds( 70, 25, 60, 30), channel("Rate1"), range(0,99,10,1,0.001),  colour( 90, 50, 50), text("Rate 1"), textColour("white")
nslider  bounds(135, 25, 60, 30), channel("Rate2"), range(0,99,20,1,0.001),  colour( 90, 50, 50), text("Rate 2"), textColour("white")
}

keyboard bounds( 5,230,1015, 75)


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

massign    0,3    ; all midi notes on all channels sent to instrument 3

giInterpArr[] array 1, 2, 4, 8
gSfilepath    init    ""


opcode FileNameFromPath,S,S        ; Extract a file name (as a string) from a full path (also as a string)
 Ssrc    xin                ; Read in the file path string
 icnt    strlen    Ssrc            ; Get the length of the file path string
 LOOP:                    ; Loop back to here when checking for a backslash
 iasc    strchar Ssrc, icnt        ; Read ascii value of current letter for checking
 if iasc==92 igoto ESCAPE        ; If it is a backslash, escape from loop
 loop_gt    icnt,1,0,LOOP        ; Loop back and decrement counter which is also used as an index into the string
 ESCAPE:                ; Escape point once the backslash has been found
 Sname    strsub Ssrc, icnt+1, -1        ; Create a new string of just the file name
    xout    Sname            ; Send it back to the caller instrument
endop



instr    1
 gkPlayStop    chnget    "PlayStop"        ; read in widgets
 gkloop        chnget    "loop"
 gktranspose    chnget    "transpose"
 gkspeed    chnget    "speed"
 gkinterp    chnget    "interp"
 gkreverse    chnget    "reverse"
 gklevel    chnget    "level"
 gSfilepath    chnget    "filename"        ; read in file path string from filebutton widget
 gkStretchOnOff    chnget    "StretchOnOff"
 gkstretchratio    chnget    "stretchratio"
 gkthreshold    chnget    "threshold"
 gkModRange    chnget    "ModRange"
 gkRate1    chnget    "Rate1"
 gkRate2    chnget    "Rate2"
 
 if changed:k(gSfilepath)==1 then        ; call instrument to update waveform viewer  
  event "i",99,0,0
 endif
 
 ktrig        trigger    gkPlayStop,0.5,0    ; if play/stop button toggles from low (0) to high (1) generate a '1' trigger
 schedkwhen    ktrig,0,0,2,0,-1        ; start instrument 2
 
 ktrig1    changed    gktranspose            ; if 'transpose' button is changed generate a '1' trigger
 ktrig2    changed    gkspeed                ; if 'speed' button is changed generate a '1' trigger
 
 if ktrig1==1 then                ; if transpose control has been changed...
  chnset    semitone(gktranspose),"speed"    ; set speed according to transpose value
 elseif ktrig2==1&&gkspeed>=0 then        ; if speed control has been changed...
  chnset    log2(gkspeed)*12,"transpose"    ; set transpose control according to speed value
 endif
endin

instr    2

 if gkPlayStop==0 then                ; if play/stop is off (stop)...
  turnoff                    ; turn off this instrument
 endif                        
 iStrLen     strlen gSfilepath        ; derive string length
 if iStrLen > 0 then                ; if string length is greater than zero (i.e. a file has been selected) then...
  iAttTim    chnget    "AttTim"        ; read in amplitude envelope attack time widget
  iRelTim    chnget    "RelTim"        ; read in amplitude envelope attack time widget
  if iAttTim>0 then                ; 
   kenv    linsegr    0,iAttTim,1,iRelTim,0
  else                                
   kenv    linsegr    1,iRelTim,0            ; attack time is zero so ignore this segment of the envelope (a segment of duration zero is not permitted
  endif
  kenv    expcurve    kenv,8            ; remap amplitude value with a more natural curve
  aenv    interp        kenv            ; interpolate and create a-rate envelope
  iFileLen    filelen    gSfilepath        ; derive chosen sound file length
  iNChns    filenchnls    gSfilepath    ; derive the number of channels (mono=1 / stereo=2) from the chosen  sound file
  iinskip    chnget    "inskip"        ; read in inskip widget
  iloop    chnget    "loop"                ; read in 'loop mode' widget
  ktrig    changed    gkloop,gkinterp            ; if loop setting or interpolation mode setting
  if ktrig==1 then                ; if loop setting has been changed...
   reinit RESTART                ; reinitialise from label RESTART
  endif                        
  RESTART:                    
  kporttime    linseg    0,0.001,0.05        ; portamento time function. (Rises quickly from zero to a held value.)
  kspeed    portk    gkspeed,kporttime    ; apply portamento smoothing to changes in speed
  
  kMod        jspline    gkModRange,gkRate1,gkRate2
  kspeed    *=    octave(kMod)
  
  if gkStretchOnOff!=1 kgoto SKIP_STRETCH
   a1,a2    init    0
   krms    rms    a1+a2
   if krms<gkthreshold then
    kspeed    *=    gkstretchratio
    kmute    =    0
   else
    kmute    =    1
   endif
   amute        interp    kmute
   aenv        *=    amute
  SKIP_STRETCH:
  
  if iNChns==2 then                ; if stereo...
   a1,a2    diskin2    gSfilepath,kspeed*(1-(gkreverse*2)),iinskip*iFileLen,i(gkloop),0,giInterpArr[i(gkinterp)-1]    ; use stereo diskin2
      outs    a1*gklevel*aenv, a2*gklevel*aenv    ; send audio to outputs
  elseif iNChns==1 then                ; if mono
   a1    diskin2    gSfilepath,kspeed*(1-(gkreverse*2)),iinskip*iFileLen,i(gkloop),0,giInterpArr[i(gkinterp)-1]    ; use mono diskin2
      outs    a1*gklevel*aenv, a1*gklevel*aenv    ; send audio to outputs
  endif
 endif
endin


instr    3
 icps    cpsmidi                            ; read in midi note data as cycles per second
 iamp    ampmidi    1                        ; read in midi velocity (as a value within the range 0 - 1)
 iStrLen     strlen    gSfilepath                ; derive string length
 if iStrLen > 0 then                        ; if string length is greater than zero (i.e. a file has been selected) then...
  iMidiRef    chnget    "MidiRef"                ; MIDI unison reference note
  iinskip    chnget    "inskip"                ; read in inskip widget
  iloop        chnget    "loop"                    ; read in 'loop mode' widget
  iAttTim    chnget    "AttTim"                ; read in amplitude envelope attack time widget
  iRelTim    chnget    "RelTim"                ; read in amplitude envelope attack time widget
  if iAttTim>0 then                        ; 
   kenv    linsegr    0,iAttTim,1,iRelTim,0
  else                                
   kenv    linsegr    1,iRelTim,0                    ; attack time is zero so ignore this segment of the envelope (a segment of duration zero is not permitted
  endif
  kenv    expcurve    kenv,8                    ; remap amplitude value with a more natural curve
  aenv    interp        kenv                    ; interpolate and create a-rate envelope
  iFileLen    filelen    gSfilepath                ; derive chosen sound file length
  ispeed        =    icps/cpsmidinn(iMidiRef)    ; derive playback speed from note played in relation to a reference note (MIDI note 60 / middle C)
  iNChns    filenchnls    gSfilepath            ; derive the number of channels (mono=1 / stereo=2) from the chosen  sound file
  if iNChns==2 then                        ; if stereo...
   a1,a2    diskin2    gSfilepath,ispeed*(1-(gkreverse*2)),iinskip*iFileLen,i(gkloop),0,giInterpArr[i(gkinterp)-1]    ; use stereo diskin2
      outs    a1*gklevel*aenv*iamp, a2*gklevel*aenv*iamp    ; send audio to outputs
  elseif iNChns==1 then                        ; if mono
   a1        diskin2    gSfilepath,ispeed*(1-(gkreverse*2)),iinskip*iFileLen,i(gkloop),0,giInterpArr[i(gkinterp)-1]    ; use mono diskin2
      outs    a1*gklevel*aenv*iamp, a1*gklevel*aenv*iamp    ; send audio to outputs
  endif
 endif
endin

instr    99
 Smessage sprintfk "file(%s)", gSfilepath            ; print sound file image to fileplayer
 chnset Smessage, "filer1"

 /* write file name to GUI */
 Sname FileNameFromPath    gSfilepath                ; Call UDO to extract file name from the full path
 Smessage sprintfk "text(%s)",Sname                ; create string to update text() identifier for label widget
 chnset Smessage, "stringbox"                    ; send string to  widget

endin

</CsInstruments>  

<CsScore>
i 1 0 10000
</CsScore>

</CsoundSynthesizer>
