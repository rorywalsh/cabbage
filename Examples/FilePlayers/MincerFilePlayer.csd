
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; MincerFilePlayer.csd
; Written by Iain McCurdy, 2014

; Three modes of playback are offered:
; 1. Manual Pointer
;     Pointer position is determined by the long horizontal slider 'Manual Pointer'.
; 2. Mouse Scrubber
;     Pointer position is determined by the mouse's X position over the waveform view. Playback is also started and stopped using right-click.
; 3. Loop Region
;     A region that has been highlighted using left-click and drag is looped using a method and speed chosen in the 'LOOP REGION' GUI area.
;     Speed can be random-modulated by increasing Mod.Range. The nature of the modulation is changed using 'Rate 1' and 'Rate 2'. The random function generator is jspline.

; Transpose can be expressed either in semitones or a simple ratio. Select mode 'Semitones' or 'Ratio'

; MOD. POINTER section contains controls which modulate the pointer position using a 'sample-and-hold' type random function
; 
 
; All three of the above methods are playable from a MIDI keyboard (first activate the 'MIDI' checkbox).
;  Transposition for MIDI activated notes is governed bu both the MIDI key played and the setting for 'Transpose'


<Cabbage>
#define SLIDER_DESIGN colour( 40, 80, 80), trackerColour("white"), textColour("white"), markerColour("white")


form caption("Mincer File Player") size(1000,370), colour(  0,  0,  0) pluginId("Minc"), guiRefresh(10)
image                    bounds(  0,  0,1000,370), colour( 50,100,100), outlineColour("White"), line(3), shape("sharp")

soundfiler bounds(  5,  5, 990,140), channel("beg","len"), identChannel("filer1"),  colour(0, 255, 255, 255), fontColour(160, 160, 160, 255), 
label      bounds(  6,  4, 998, 14), text(""), align(left), colour(0,0,0,0), fontColour(200,200,200), identChannel("stringbox")

hslider  bounds(  0,138,1000, 30), channel("pointer"), range( 0,  1.00, 0.1), colour( 40, 80, 80),  trackerColour("white"), fontColour("white")
label    bounds(  0,160,1000, 12), text("Manual Pointer"), fontColour("white")

filebutton bounds(  5,185, 80, 22), text("Open File","Open File"), fontColour("white"), channel("filename"), shape("ellipse")

checkbox bounds(  5,218,120, 12), text("Manual Pointer"), channel("r1"), fontColour:0("white"), fontColour:1("white"), colour(yellow), radioGroup(1)
checkbox bounds(  5,232,120, 12), text("Mouse Scrubber"), channel("r2"), fontColour:0("white"), fontColour:1("white"), colour(yellow), radioGroup(1), value(1) 
label    bounds( 19,246,100, 10), text("[right click and drag]"), fontColour("white"), align("left")
checkbox bounds(  5,256,120, 12), text("Loop Region"),    channel("r3"), fontColour:0("white"), fontColour:1("white"), colour(yellow), radioGroup(1) 
label    bounds( 19,270,100, 10), text("[left click and drag]"), fontColour("white"), align("left")

checkbox bounds(125,223, 60, 12), channel("lock"), text("Lock"), colour("yellow"), fontColour:0("white"), fontColour:1("white")
checkbox bounds(125,243, 60, 12), channel("MIDI"), text("MIDI"), colour("yellow"), fontColour:0("white"), fontColour:1("white")

label    bounds(105,183, 48, 12), text("FFT Size"), fontColour("white")
combobox bounds( 95,195, 70, 20), channel("FFTSize"), text("32768", "16384", "8192", "4096", "2048", "1024", "512", "256", "128", "64", "32", "16", "8", "4"), value(5), fontColour("white")

combobox bounds(170,175, 80, 20), text("Semitone","Ratio"), channel("IntervalMode"),       value(1)

image    bounds(175,200, 70, 70), colour(0,0,0,0), plant("Semitones"), identChannel("SemitonesPlant_ident"), visible(1) {
rslider  bounds(  0,  0, 70, 70), channel("Semitones"), range(-48, 48, 0,1,1), text("Semitones"), $SLIDER_DESIGN
}

image    bounds(175,200, 70, 70), colour(0,0,0,0), plant("Ratio"), identChannel("RatioPlant_ident"), visible(0) {
nslider  bounds( 20,  0, 25, 18), channel("Numerator"),        range(1,99,3,1,1)
image    bounds( 15, 26, 35,  1), shape("sharp") 
nslider  bounds( 20, 35, 25, 18), channel("Denominator"),      range(1,99,2,1,1)
}

rslider  bounds(240,200, 70, 70), channel("portamento"),range(0, 20,0.05,0.5,0.01), text("Port.Time"), $SLIDER_DESIGN
rslider  bounds(305,200, 70, 70), channel("Stack"),range(1, 30, 1, 1, 1), text("Stack"), $SLIDER_DESIGN
rslider  bounds(370,200, 70, 70), channel("StackIntvl"),range(-12, 12, 1), text("Interval"), $SLIDER_DESIGN

image    bounds(450,188, 305,90), colour(0,0,0,0), outlineColour("silver"), outlineThickness(1), shape("sharp"), plant("LoopRegion") {
label    bounds(  0,  2, 305,10), text("L   O   O   P       R   E   G   I   O   N"), fontColour("white")
label    bounds( 10, 24, 85, 12), text("Shape"), fontColour("white")
combobox bounds( 10, 37, 85, 20), channel("LoopMode"), text("Forward","Backward","Triangle","Sine"), value(1), fontColour:0("white")
checkbox bounds( 10, 63, 85, 14), channel("SpeedMult"), text("Speed x 10"), value(0), fontColour:0("white"), fontColour:1("white")
rslider  bounds(100, 17, 70, 70), channel("Speed"), range(-2, 2, 1,1,0.001), text("Speed"), $SLIDER_DESIGN
nslider  bounds(170, 35, 60, 30), channel("ModRange"), range(0,2,0,1,0.001),  colour(  0,  0,  0), text("Mod.Range"), textColour("white")
nslider  bounds(235, 20, 60, 30), channel("Rate1"),    range(0,30,1,1,0.001), colour(  0,  0,  0), text("Rate 1"),    textColour("white")
nslider  bounds(235, 50, 60, 30), channel("Rate2"),    range(0,30,2,1,0.001), colour(  0,  0,  0), text("Rate 2"),    textColour("white")
}

image    bounds(760,188, 145,90), colour(0,0,0,0), outlineColour("silver"), outlineThickness(1), shape("sharp"), plant("ModPtr") {
label    bounds(  0,  2, 135,10), text("M  O  D.     P  O  I  N  T  E  R"), fontColour("white")
nslider  bounds( 10, 35, 60, 30), channel("PtrModRange"), range(0,1,0,1,0.001),  colour(  0,  0,  0), text("Mod.Range"), textColour("white")
nslider  bounds( 75, 20, 60, 30), channel("PtrRate1"),    range(0,500,1,1,0.001), colour(  0,  0,  0), text("Rate 1"),    textColour("white")
nslider  bounds( 75, 50, 60, 30), channel("PtrRate2"),    range(0,500,2,1,0.001), colour(  0,  0,  0), text("Rate 2"),    textColour("white")
}

image      bounds(915,200, 70, 70), colour(0,0,0,0), plant("output") {
rslider    bounds(  0,  0, 70, 70), channel("level"),     range(  0,  1.00, 1, 0.5),        text("Level"), $SLIDER_DESIGN
}

image      bounds(  5,290,195, 70), colour(0,0,0,0) {
rslider    bounds(  0,  0, 70, 70), channel("AttTim"),    range(0, 5, 0, 0.5, 0.001),       text("Att.Tim"), $SLIDER_DESIGN
rslider    bounds( 65,  0, 70, 70), channel("RelTim"),    range(0.01, 5, 0.05, 0.5, 0.001), text("Rel.Tim"), $SLIDER_DESIGN
rslider    bounds(130,  0, 70, 70), channel("MidiRef"),   range(0,127,60, 1, 1),            text("MIDI Ref."), $SLIDER_DESIGN
}

keyboard bounds(215,290,780, 75)
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-n -+rtmidi=NULL -M0 -d
</CsOptions>

<CsInstruments>

; sr set by host
ksmps  = 64
nchnls = 2
0dbfs  = 1

massign    0, 3

gichans         init    0        ; 
giReady         init    0        ; flag to indicate function table readiness

giFFTSizes[]    array    32768, 16384, 8192, 4096, 2048, 1024, 512, 256, 128, 64, 32, 16, 8, 4    ; an array is used to store FFT window sizes
giTriangle      ftgen    0, 0, 4097, 7, 0, 2048, 1, 2048, 0
giRectSine      ftgen    0, 0, 4097, 19, 1, 0.5, 0, 1
gSfilepath      init     ""
gkFileLen       init     0

opcode FileNameFromPath,S,S           ; Extract a file name (as a string) from a full path (also as a string)
 Ssrc    xin                          ; Read in the file path string
 icnt    strlen    Ssrc               ; Get the length of the file path string
 LOOP:                                ; Loop back to here when checking for a backslash
 iasc    strchar Ssrc, icnt           ; Read ascii value of current letter for checking
 if iasc==92 igoto ESCAPE             ; If it is a backslash, escape from loop
 loop_gt    icnt,1,0,LOOP             ; Loop back and decrement counter which is also used as an index into the string
 ESCAPE:                              ; Escape point once the backslash has been found
 Sname      strsub  Ssrc, icnt+1, -1  ; Create a new string of just the file name
    xout    Sname                     ; Send it back to the caller instrument
endop


opcode mincerStack,a,akkikikkip
 apointer, klevel, ktranspose, itable, klock, iFFTsize, kStackIntvl, kporttime, iStack, iCount xin
    kStackIntvlL portk kStackIntvl, randomi:k(kporttime*0.5,kporttime*2,1,1), i(kStackIntvl)
    a1       mincer   apointer, klevel, ktranspose* semitone((iCount-1)*kStackIntvlL), itable, klock, iFFTsize, iStack
    aMix     =        0
    if iCount<iStack then
     aMix    mincerStack   apointer, klevel, ktranspose, itable, klock, iFFTsize, kStackIntvl, kporttime, iStack, iCount+1
    endif
xout a1 + aMix
endop


instr    1 ; always on
 /* PORTAMENTO TIME FUNCTION */
 gkporttimeW  chnget    "portamento"
 krampup      linseg    0,0.001,1
 kporttime    =         krampup * gkporttimeW    

 /* SHOW HIDE INTERVAL MODE (SEMITONES OR RATIO) WIDGETS */
 kIntervalMode    chnget    "IntervalMode"
 if changed(kIntervalMode)==1 then                ; semitones mode
  if kIntervalMode==1 then
      chnset    "visible(0)","RatioPlant_ident"
      chnset    "visible(1)","SemitonesPlant_ident"
  else                                ; ratio mode
      chnset    "visible(1)","RatioPlant_ident"
      chnset    "visible(0)","SemitonesPlant_ident"
  endif
 endif
 /* DEFINE TRANSPOSITION RATIO BASED ON INTERVAL MODE CHOICE */
 if kIntervalMode==1 then                    ; semitones mode
     kSemitones    chnget    "Semitones"
     kSemitones    portk    kSemitones,kporttime
     gktranspose   =        semitone(kSemitones)    
 else                                ; ratio mode
     kNumerator     chnget   "Numerator"
     kDenominator   chnget   "Denominator"
     gkRatio        =        kNumerator/kDenominator
     gktranspose    portk    gkRatio,kporttime    
 endif

 gkr1        chnget    "r1"    ; pointer/note mode select (radio buttons):    manual
 gkr2        chnget    "r2"    ;                         mouse
 gkr3        chnget    "r3"    ;                         loop
 gkmode      =         (gkr1) + (gkr2*2) + (gkr3*3)
 gkloop      chnget    "loop"
 gkMIDI      chnget    "MIDI"
 gklock      chnget    "lock"
 gkfreeze    chnget    "freeze"
 gkfreeze    =         1 - gkfreeze
 gklevel     chnget    "level"
 gkFFTSize   chnget    "FFTSize"
 gSfilepath  chnget    "filename"
 kNewFileTrg changed    gSfilepath        ; if a new file is loaded generate a trigger
 gkLoopStart chnget    "beg"            ; Click-and-drag region beginning in sample frames (in sample frames)
 gkLoopLen   chnget    "len"            ; Click-and-drag region length in sample frames
 ;printks  "%f\t   %f\n",1,gkLoopStart,gkLoopLen

; gkLoopLen   limit     gkLoopLen,1,gkFileLen    
 gkLoopMode  chnget    "LoopMode"
 gkSpeed     chnget    "Speed"
 if chnget:k("SpeedMult")==1 then
  gkSpeed *= 10
 endif
 gkModRange  chnget    "ModRange"
 if gkModRange>0 then
  gkRate1    chnget    "Rate1"
  gkRate2    chnget    "Rate2"
  kMod       jspline   gkModRange,gkRate1,gkRate2
  kSpeed2    =         gkSpeed + kMod
  gkSpeed    =         kSpeed2
 endif
 gkPtrModRange chnget    "PtrModRange"
 gkPtrRate1    chnget    "PtrRate1"
 gkPtrRate2    chnget    "PtrRate2"
 gkStack       chnget    "Stack"
 gkStackIntvl  chnget    "StackIntvl"
 
 gkMOUSE_DOWN_RIGHT   chnget    "MOUSE_DOWN_RIGHT"        ; Read in mouse right click status
 kStartScrub          trigger    gkMOUSE_DOWN_RIGHT,0.5,0    ; generate a momentary trigger whenver right mouse button is clicked
 
 if gkMOUSE_DOWN_RIGHT==1 && gkmode==2 then
  kMOUSE_X    chnget    "MOUSE_X"
  kMOUSE_Y    chnget    "MOUSE_Y"
  if kStartScrub==1 then                     ; prevent initial portamento when a new note is started using right click
   reinit RAMP_FUNC
  endif
  RAMP_FUNC:
  krampup    linseg    0,0.001,1
  rireturn
  kMOUSE_X    portk    (kMOUSE_X - 5) / 990, kporttime            ; Mouse X to pointer position
  kMOUSE_Y    limit    ((kMOUSE_Y - 5) / 150), 0, 1        ; Mouse Y transposition
  gapointer   interp    kMOUSE_X
  kSemitones  chnget    "Semitones"
  gktranspose portk    ((kMOUSE_Y*2)-1)*kSemitones,kporttime        ; Transposition is scaled using transposition value derived either from 'Semitone' slider or 'Ratio' nslideres
  gktranspose =    semitone(gktranspose)
  gklevel     portk    kMOUSE_Y*gklevel + (1-gklevel), kporttime*krampup
  schedkwhen  kStartScrub,0,0,2,0,-1
 else
  kpointer    chnget    "pointer"
  kpointer    portk     kpointer, kporttime
  gapointer   interp    kpointer
 endif
                                
 if kNewFileTrg==1 then                ; if a new file has been loaded...
  event    "i",99,0,0                ; call instrument to update sample storage function table 
 endif  
 
 if changed(gkmode+gkMIDI)==1 then
  if gkmode==1||gkmode==3&&gkMIDI==0 then
   event    "i",2,0,-1
  endif
 endif
endin

instr    99    ; load sound file
 gichans    filenchnls    gSfilepath            ; derive the number of channels (mono=1,stereo=2) in the sound file
 gitableL    ftgen    1,0,0,1,gSfilepath,0,0,1
 if gichans==2 then
  gitableR    ftgen    2,0,0,1,gSfilepath,0,0,2
 endif
 giReady     =    1                    ; if no string has yet been loaded giReady will be zero
 gkFileLen    init    ftlen(1)
 
 Smessage sprintfk "file(%s)", gSfilepath            ; print sound file image to fileplayer
 chnset Smessage, "filer1"
 
 /* WRITE FILE NAME TO GUI */
 Sname FileNameFromPath    gSfilepath                ; Call UDO to extract file name from the full path
 Smessage sprintfk "text(%s)",Sname
 chnset Smessage, "stringbox"

endin

instr    2    ; non-midi
 if gkmode!=1 && gkmode!=3 && gkMOUSE_DOWN_RIGHT!=1 || gkMIDI==1 then
  turnoff
 endif
 if giReady = 1 then                                   ; i.e. if a file has been loaded
  aenv            linsegr   0,0.01,1,0.01,0            ; simple de-click envelope
    
  iFileLen        filelen   gSfilepath
  
  if i(gkmode)==3 then
   if gkLoopMode==1 then ; forward
    apointer      phasor    (sr*gkSpeed)/gkLoopLen
   elseif gkLoopMode==2 then ; backward
    apointer      phasor    -(sr*gkSpeed)/gkLoopLen
   elseif gkLoopMode==3 then ; tri
    apointer      poscil    1,(sr*gkSpeed)/gkLoopLen,giTriangle
   elseif gkLoopMode==4 then ; sine
    apointer      poscil    1,(sr*gkSpeed)/gkLoopLen,giRectSine
   endif
   apointer       =         (apointer * (gkLoopLen/sr)) + (gkLoopStart/sr)
  else
   apointer       =         gapointer*iFileLen
  endif

  /* RANDOM POINTER MODULATION */
  if gkPtrModRange>0 then
   kRndPtrRate    init      random(i(gkPtrRate1),i(gkPtrRate2))
   kRndPtrTrig    metro     kRndPtrRate
   kRndPtrRate    trandom   kRndPtrTrig,gkPtrRate1,gkPtrRate2
   kRndPtrPos     trandom   kRndPtrTrig,-gkPtrModRange*iFileLen,gkPtrModRange*iFileLen
   apointer       +=        interp(kRndPtrPos)
  endif
  
  ktrig           changed   gkFFTSize
  if ktrig==1 then
   reinit RESTART
  endif
  RESTART:
  if gichans=1 then

   if gkStack==1 then
    a1             mincer    apointer, gklevel, gktranspose, gitableL, gklock, giFFTSizes[i(gkFFTSize)-1]
   else
    if changed:k(gkStack)==1 then
     reinit RESTART_STACK1
    endif
    RESTART_STACK1:
    a1       mincerStack   apointer, gklevel, gktranspose, gitableL, gklock, giFFTSizes[i(gkFFTSize)-1], gkStackIntvl, gkporttimeW, i(gkStack)
   endif
     
                  outs      a1 * aenv, a1 * aenv                                                                                                                                          
  elseif gichans=2 then
   if gkStack==1 then
    a1             mincer    apointer, gklevel, gktranspose, gitableL, gklock, giFFTSizes[i(gkFFTSize)-1]
    a2             mincer    apointer, gklevel, gktranspose, gitableR, gklock, giFFTSizes[i(gkFFTSize)-1]
   else
    if changed:k(gkStack)==1 then
     reinit RESTART_STACK1
    endif
    RESTART_STACK1:
    a1       mincerStack   apointer, gklevel, gktranspose, gitableL, gklock, giFFTSizes[i(gkFFTSize)-1], gkStackIntvl, gkporttimeW, i(gkStack)
    a2       mincerStack   apointer, gklevel, gktranspose, gitableR, gklock, giFFTSizes[i(gkFFTSize)-1], gkStackIntvl, gkporttimeW, i(gkStack)
   endif   
            outs          a1 * aenv, a2 * aenv
  endif  
 endif
endin

instr    3    ; midi triggered instrument
 if giReady==1 then                                  ; i.e. if a file has been loaded
  icps            cpsmidi                            ; read in midi note data as cycles per second
  iamp            ampmidi   1                        ; read in midi velocity (as a value within the range 0 - 1)
  iMidiRef        chnget    "MidiRef"                ; MIDI unison reference note
  iFrqRatio       =         icps/cpsmidinn(iMidiRef) ; derive playback speed from note played in relation to a reference note (MIDI note 60 / middle C)                                          
 
  iAttTim         chnget    "AttTim"        ; read in amplitude envelope attack time widget
  iRelTim         chnget    "RelTim"        ; read in amplitude envelope attack time widget                                                                                                                                   
  if iAttTim>0 then                ;                                       
   kenv           linsegr   0, iAttTim, 1, iRelTim, 0
  else                                
   kenv           linsegr   1, iRelTim, 0            ; attack time is zero so ignore this segment of the envelope (a segment of duration zero is not permitted
  endif
  kenv            expcurve  kenv,8                   ; remap amplitude value with a more natural curve
  aenv            interp    kenv                     ; interpolate and create a-rate envelope

  iFileLen        filelen   gSfilepath
  
  if i(gkmode)==3 then
   if gkLoopMode==1 then
    apointer      phasor    (sr*gkSpeed)/gkLoopLen
   elseif gkLoopMode==2 then
    apointer      phasor    -(sr*gkSpeed)/gkLoopLen
   elseif gkLoopMode==3 then
    apointer      poscil    1,(sr*gkSpeed*0.5)/gkLoopLen,giTriangle
   elseif gkLoopMode==4 then
    apointer      poscil    1,(sr*gkSpeed*0.5)/gkLoopLen,giRectSine
   endif
   apointer       =         (apointer * (gkLoopLen/sr)) + (gkLoopStart/sr)
  else                                                
   apointer       =         gapointer*iFileLen
  endif

  /* RANDOM POINTER MODULATION */
  if gkPtrModRange>0 then
   kRndPtrRate    init     random(i(gkPtrRate1),i(gkPtrRate2))
   kRndPtrTrig    metro    kRndPtrRate
   kRndPtrRate    trandom  kRndPtrTrig,gkPtrRate1,gkPtrRate2
   kRndPtrPos     trandom  kRndPtrTrig,-gkPtrModRange*iFileLen,gkPtrModRange*iFileLen
   apointer       +=       interp(kRndPtrPos)
  endif
                                                                        
  ktrig     changed        gkFFTSize
  if ktrig==1 then
   reinit RESTART
  endif
  RESTART:
  if gichans=1 then
   if gkStack==1 then
    a1       mincer        apointer, gklevel*iamp, iFrqRatio*gktranspose, gitableL, gklock, giFFTSizes[i(gkFFTSize)-1]
   else
    if changed:k(gkStack)==1 then
     reinit RESTART_STACK1
    endif
    RESTART_STACK1:
    a1       mincerStack   apointer, gklevel*iamp, iFrqRatio*gktranspose, gitableL, gklock, giFFTSizes[i(gkFFTSize)-1], gkStackIntvl, gkporttimeW, i(gkStack)
   endif
            outs          a1 * aenv, a1 * aenv
  elseif    gichans=2 then
   if gkStack==1 then
    a1       mincer        apointer, gklevel*iamp, iFrqRatio*gktranspose, gitableL, gklock, giFFTSizes[i(gkFFTSize)-1]
    a2       mincer        apointer, gklevel*iamp, iFrqRatio*gktranspose, gitableR, gklock, giFFTSizes[i(gkFFTSize)-1]
   else
    if changed:k(gkStack)==1 then
     reinit RESTART_STACK1
    endif
    RESTART_STACK1:
    a1       mincerStack   apointer, gklevel*iamp, iFrqRatio*gktranspose, gitableL, gklock, giFFTSizes[i(gkFFTSize)-1], gkStackIntvl, gkporttimeW, i(gkStack)
    a1       mincerStack   apointer, gklevel*iamp, iFrqRatio*gktranspose, gitableR, gklock, giFFTSizes[i(gkFFTSize)-1], gkStackIntvl, gkporttimeW, i(gkStack)
   endif   
            outs          a1 * aenv, a2 * aenv
  endif
 endif
endin

</CsInstruments>  

<CsScore>
i 1 0 [60*60*24*7]
</CsScore>

</CsoundSynthesizer>