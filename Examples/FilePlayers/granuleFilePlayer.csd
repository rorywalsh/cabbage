
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; granuleFilePlayer.csd
; Written by Iain McCurdy, 2023

; Level       - output level control
; N.Voices    - number of simultaneous granular synthesis voices (layers). 
;                This is limited to 128 layers but if more are needed just trigger multiple notes.
; Speed Ratio - speed of movement through the sound file as a ratio with normal speed
; Grain Mode  - directoin of the playback of each individual grain: Fwd, Bwd or randomly Fwd or Bwd
; Start       - where to start reading grains from within the chosen sound file as a ratio of its full length
; Start OS    - random offset of Start in seconds (different for each layer)
; Grain Size  - size of grains in seconds
; Gr.Size OS  - random offset of the grain size (percentage)
; Att. Time   - rise time for each individual grain as a percentage of the full duration of the grain
; Dec. Time   - decay time for each individual grain as a percentage of the full duration of the grain
; Grain Gap   - gap between consecutive grains in a layer
; Gap OS      - random offset of the gap between grains in each layer (percentage)
; N. Pitches  - number of pitch transpositions (zero = random transpositions between -1 and +1 octaves)
; Pitch 1 to 4 - four user-definable transpositions (in semitones)
; MIDI Ref.   - unison MIDI note (number)
; Att. Time   - Attack Time over each note (seconds)
; Rel. Time   - Release Time over each note (seconds)






<Cabbage>
form caption("Granule File Player") size(810,505), colour("Silver") pluginId("SWPl"), guiRefresh(32)

#define SliderStyleK #textColour("Black"), textColour("Black"), fontColour("Black"), valueTextBox(1), colour(50,110, 80), trackerColour(150,210,180)#
#define SliderStyleI #textColour("Black"), textColour("Black"), fontColour("Black"), valueTextBox(1), colour(250,110,110), trackerColour(255,200,200)#

soundfiler bounds(  5,  5,800,175), channel("beg","len"), identChannel("filer1"), colour(0, 255, 255, 255), fontColour(160, 160, 160, 255)
keyboard   bounds(  5,420,800, 80)

filebutton bounds(  5,205, 80, 22), text("Open File","Open File"), fontColour("White") channel("filename"), shape("ellipse")
checkbox   bounds(  5,240, 95, 22), channel("PlayStop"), text("Play/Stop"), fontColour:0("Black"), fontColour:1("Black")

rslider    bounds( 80,202, 90, 90), channel("Amp"),    range(0,1,0.5,0.5), text("Level"), $SliderStyleK
rslider    bounds(145,202, 90, 90), channel("NVoice"), range(1,128,32,1,1), text("N. Voices"), $SliderStyleI
rslider    bounds(210,202, 90, 90), channel("SpeedRatio"), text("Speed Ratio"), range(0.01,20,1,0.5), $SliderStyleI
label      bounds(295,208, 74, 12), text("Grain Mode"), fontColour("Black"), align("centre")
combobox   bounds(295,220, 74, 17), channel("mode"), items("Reverse", "Random", "Forward"), value(3), fontColour("white")
rslider    bounds(365,202, 90, 90), channel("Inskip"), text("Start"), range(0,0.99,0), $SliderStyleI
rslider    bounds(430,202, 90, 90), channel("InskipOS"), text("Start OS"), range(0,1,0), $SliderStyleI

image      bounds(520,185,285,110), colour(0,0,0,0), outlineThickness(1), outlineColour("Grey")
{  
label      bounds(  0,  2,285, 13), text("G R A I N   S I Z E"), fontColour("Black")
rslider    bounds(  0, 17, 90, 90), channel("GSize"), text("Grain Size"), range(0.01,1,0.05,0.5,0.0001), $SliderStyleK
rslider    bounds( 65, 17, 90, 90), channel("GSizeOS"), text("Gr.Size OS"), range(0,100,30,1,1), $SliderStyleI
rslider    bounds(130, 17, 90, 90), channel("att"), text("Gr. Att."), range(0,100,30,1,1), $SliderStyleI
rslider    bounds(195, 17, 90, 90), channel("dec"), text("Gr. Dec."), range(0,100,30,1,1), $SliderStyleI
}

image      bounds(  5,300,155,110), colour(0,0,0,0), outlineThickness(1), outlineColour("Grey")
{
label      bounds(  0,  2,155, 13), text("D E N S I T Y"), fontColour("Black")
rslider    bounds(  0, 17, 90, 90), channel("Gap"), text("Grain Gap"), range(0,10,0,0.25,0.00001), $SliderStyleK
rslider    bounds( 65, 17, 90, 90), channel("GapOS"), text("Gap OS"), range(0,100,10,1,1), $SliderStyleI
}

image      bounds(165,300,350,110), colour(0,0,0,0), outlineThickness(1), outlineColour("Grey")
{  
label      bounds(  0,  2,350, 13), text("T R A N S P O S I T I O N S"), fontColour("Black")
rslider    bounds(  0, 17, 90, 90), channel("NPitches"), range(0,4,1,1,1), text("N.Pitches"), $SliderStyleI
rslider    bounds( 65, 17, 90, 90), channel("Pitch1"), text("Pitch 1"), range(-24,24,0,1,0.001), $SliderStyleI
rslider    bounds(130, 17, 90, 90), channel("Pitch2"), text("Pitch 2"), range(-24,24,0,1,0.001), $SliderStyleI
rslider    bounds(195, 17, 90, 90), channel("Pitch3"), text("Pitch 3"), range(-24,24,0,1,0.001), $SliderStyleI
rslider    bounds(260, 17, 90, 90), channel("Pitch4"), text("Pitch 4"), range(-24,24,0,1,0.001), $SliderStyleI
}

image      bounds(525,300,280,110), colour(0,0,0,0), outlineThickness(1), outlineColour("Grey")
{  
label      bounds(  0,  2,280, 13), text("C O N T R O L"), fontColour("Black")
rslider    bounds(  0, 17, 90, 90), channel("MidiRef"),   range(0,127,60, 1, 1),  text("MIDI Ref."), $SliderStyleI
rslider    bounds( 65, 17, 90, 90), channel("AttTim"),    range(0, 5.00, 0.01, 0.5, 0.001), text("Att. Time"), $SliderStyleI
rslider    bounds(130, 17, 90, 90), channel("RelTim"),    range(0.01, 5, 0.05, 0.5, 0.001), text("Rel. Time"), $SliderStyleI


}

</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-n -+rtmidi=NULL -M0 -dm0
</CsOptions>

<CsInstruments>

ksmps  = 64
nchnls = 2
0dbfs  = 1

               massign    0,3
gichans        init       0
giFileLen      init       0
giReady        init       0
gSfilepath     init       ""


instr    1
 kporttime      linseg     0,0.001,0.02
 gkPlayStop     chnget     "PlayStop"
 gkfreeze       chnget     "freeze"
 gkfreeze       =          1-gkfreeze
 gkAmp          chnget     "Amp"
 gkNVoice       chnget     "NVoice"
 gkAmp          /=         gkNVoice^0.8
 gkNPitches     chnget     "NPitches"
 gkNVoice       limit      gkNVoice,gkNPitches,128 ; can't be lower than number of pitch shift voices
 gkSpeedRatio   chnget     "SpeedRatio"
 gkmode         chnget     "mode"
 gkGSize        chnget     "GSize"
 gkGSize        portk      gkGSize, kporttime
 gkGSizeOS      chnget     "GSizeOS"
 katt           chnget     "att"
 kdec           chnget     "dec"
 if (katt+kdec)>100 then
  gkatt = int(katt * (100/(katt+kdec)))
  gkdec = int(kdec * (100/(katt+kdec)))
 else
  gkatt = katt
  gkdec = kdec
 endif
 gkGap        chnget     "Gap"
 gkGapOS      chnget     "GapOS"
 gkPitch1     chnget     "Pitch1"
 gkPitch2     chnget     "Pitch2"
 gkPitch3     chnget     "Pitch3"
 gkPitch4     chnget     "Pitch4"
 gkInskip     chnget     "Inskip"
 gkInskipOS   chnget     "InskipOS"

 gSfilepath     chnget     "filename"
 kNewFileTrg    changed    gSfilepath    ; if a new file is loaded generate a trigger
 if kNewFileTrg==1 then                 ; if a new file has been loaded...
                event      "i",99,0,0    ; call instrument to update sample storage function table 
 endif  

 ktrig         trigger    gkPlayStop,0.5,0
               schedkwhen ktrig,0,0,2,0,-1

 
endin



instr    99    ; load sound file
 gichans       filenchnls gSfilepath                ; derive the number of channels (mono=1,stereo=2) in the sound file
 giFileLen     filelen    gSfilepath                ; derive the number of channels (mono=1,stereo=2) in the sound file
 gitableL      ftgen      1,0,0,1,gSfilepath,0,0,1
 if gichans==2 then
  gitableR     ftgen      2,0,0,1,gSfilepath,0,0,2
 endif
 giReady       =          1                         ; if no string has yet been loaded giReady will be zero

 Smessage      sprintfk   "file(%s)", gSfilepath
               chnset     Smessage, "filer1"    

endin



instr    2    ; triggered by 'play/stop' button

 if gkPlayStop==0 then
               turnoff
 endif
 if giReady = 1 then                              ; i.e. if a file has been loaded
  iAttTim      chnget     "AttTim"                ; read in widgets
  iRelTim      chnget     "RelTim"
  if iAttTim>0 then                               ; is amplitude envelope attack time is greater than zero...
   kenv        linsegr    0,iAttTim,1,iRelTim,0   ; create an amplitude envelope with an attack, a sustain and a release segment (senses realtime release)
  else            
   kenv        linsegr    1,iRelTim,0             ; create an amplitude envelope with a sustain and a release segment (senses realtime release)
  endif
  kenv         expcurve   kenv,8                  ; remap amplitude value with a more natural curve
  aenv         interp     kenv                    ; interpolate and create a-rate envelope
  ithd      =        0
  iLen      =        ftlen(gitableL)/sr
  iseed     =        0
  ifenv     =        0

  if changed:k(gkNVoice,gkNPitches,gkSpeedRatio,gkmode,gkGSizeOS,gkatt,gkdec,gkGapOS,gkPitch1,gkPitch2,gkPitch3,gkPitch4,gkInskip,gkInskipOS)==1 then
   reinit UPDATE
  endif
  UPDATE:
  igskip    =        i(gkInskip)*iLen
  igskip_os =        i(gkInskipOS)
  ilength   =        iLen-igskip
 
  if gichans==1 then                                                    ; if mono...
  a1        granule  gkAmp, i(gkNVoice), i(gkSpeedRatio), i(gkmode)-2, ithd, gitableL, i(gkNPitches), igskip, igskip_os, ilength, gkGap, i(gkGapOS), gkGSize, i(gkGSizeOS), i(gkatt), i(gkdec), iseed, semitone(i(gkPitch1)), semitone(i(gkPitch2)), semitone(i(gkPitch3)), semitone(i(gkPitch4)) ;, ifnenv] 
  a1        *=       aenv
            outs     a1,a1
  elseif gichans==2 then                                                ; otherwise, if stereo...
  a1        granule  gkAmp, i(gkNVoice), i(gkSpeedRatio), i(gkmode)-2, ithd, gitableL, i(gkNPitches), igskip, igskip_os, ilength, gkGap, i(gkGapOS), gkGSize, i(gkGSizeOS), i(gkatt), i(gkdec), iseed, semitone(i(gkPitch1)), semitone(i(gkPitch2)), semitone(i(gkPitch3)), semitone(i(gkPitch4)) ;, ifnenv] 
  a2        granule  gkAmp, i(gkNVoice), i(gkSpeedRatio), i(gkmode)-2, ithd, gitableR, i(gkNPitches), igskip, igskip_os, ilength, gkGap, i(gkGapOS), gkGSize, i(gkGSizeOS), i(gkatt), i(gkdec), iseed, semitone(i(gkPitch1)), semitone(i(gkPitch2)), semitone(i(gkPitch3)), semitone(i(gkPitch4)) ;, ifnenv] 
  a1        *=       aenv
  a2        *=       aenv
            outs     a1,a2
  endif
  rireturn
 endif

endin

instr    3 ; MIDI triggered instrument
 icps          cpsmidi                            ; read in midi note data as cycles per second
 iMidiRef      chnget    "MidiRef"
 iFrqRatio     =         icps/cpsmidinn(iMidiRef) ; derive playback speed from note played in relation to a reference note (MIDI note 60 / middle C)
 iamp          ampmidi   1                        ; read in midi velocity (as a value within the range 0 - 1)

 if giReady = 1 then                              ; i.e. if a file has been loaded
  iAttTim      chnget     "AttTim"                ; read in widgets
  iRelTim      chnget     "RelTim"
  if iAttTim>0 then                               ; is amplitude envelope attack time is greater than zero...
   kenv        linsegr    0,iAttTim,1,iRelTim,0   ; create an amplitude envelope with an attack, a sustain and a release segment (senses realtime release)
  else            
   kenv        linsegr    1,iRelTim,0             ; create an amplitude envelope with a sustain and a release segment (senses realtime release)
  endif
  kenv         expcurve   kenv,8                  ; remap amplitude value with a more natural curve
  aenv         interp     kenv                    ; interpolate and create a-rate envelope
  ithd      =        0
  iLen      =        ftlen(gitableL)/sr
  iseed     =        0
  ifenv     =        0

  igskip    =        i(gkInskip)*iLen
  igskip_os =        i(gkInskipOS)
  ilength   =        iLen-igskip
 
  if gichans==1 then                                                    ; if mono...
  a1        granule  gkAmp, i(gkNVoice), i(gkSpeedRatio), i(gkmode)-2, ithd, gitableL, i(gkNPitches), igskip, igskip_os, ilength, gkGap, i(gkGapOS), gkGSize, i(gkGSizeOS), i(gkatt), i(gkdec), iseed, semitone(i(gkPitch1))*iFrqRatio, semitone(i(gkPitch2))*iFrqRatio, semitone(i(gkPitch3))*iFrqRatio, semitone(i(gkPitch4))*iFrqRatio ;, ifnenv] 
  a1        *=       aenv
            outs     a1,a1
  elseif gichans==2 then                                                ; otherwise, if stereo...
  a1        granule  gkAmp, i(gkNVoice), i(gkSpeedRatio), i(gkmode)-2, ithd, gitableL, i(gkNPitches), igskip, igskip_os, ilength, gkGap, i(gkGapOS), gkGSize, i(gkGSizeOS), i(gkatt), i(gkdec), iseed, semitone(i(gkPitch1))*iFrqRatio, semitone(i(gkPitch2))*iFrqRatio, semitone(i(gkPitch3))*iFrqRatio, semitone(i(gkPitch4))*iFrqRatio ;, ifnenv] 
  a2        granule  gkAmp, i(gkNVoice), i(gkSpeedRatio), i(gkmode)-2, ithd, gitableR, i(gkNPitches), igskip, igskip_os, ilength, gkGap, i(gkGapOS), gkGSize, i(gkGSizeOS), i(gkatt), i(gkdec), iseed, semitone(i(gkPitch1))*iFrqRatio, semitone(i(gkPitch2))*iFrqRatio, semitone(i(gkPitch3))*iFrqRatio, semitone(i(gkPitch4))*iFrqRatio ;, ifnenv] 
  a1        *=       aenv
  a2        *=       aenv
            outs     a1,a2
  endif
 endif

endin

</CsInstruments>  

<CsScore>
i 1 0 z
</CsScore>

</CsoundSynthesizer>
