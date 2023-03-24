
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; Grain3FilePlayer.csd
; Written by Iain McCurdy, 2015

; File player based around the granular synthesis opcode, 'grain3'.
; A second voice can be activated (basically another parallel granular synthesiser) with parameter variations of density, transposition, pointer location (Phs) and delay.
; Two modes of playback are available: manual pointer and speed
; The pointer and grain density can also be modulated by right-clicking and dragging on the waveform view.
;  * This will also start and stop the grain producing instrument.
;  * In click-and-drag mode mouse X position equates to pointer position and mouse Y position equates to grain density. 
; If played from the MIDI keyboard, note number translates to 'Density' and key velocity translates to amplitude for the grain stream for that note.

; In 'Pointer' mode pointer position is controlled by the long 'Manual' slider with an optional amount of randomisation determined ny the 'Phs.Mod' slider.  

; Selecting 'Speed' pointer mode bring up some additional controls:
; Speed        -    speed ratio
; Freeze       -    freezes the pointer at its present locations 
; Range        -    ratio of the full sound file duration that will be played back. 1=the_entire_file, 0.5=half_the_file, etc. 
; Shape        -    shape of playback function:     'Phasor' looping in a single direction
;                            'Tri' back and forth looping
; The 'Manual' control functions as an pointer offset when using 'Speed' pointer mode

; Size         -    size of the grains
; Density      -    grains per second
; Transpose    -    transposition in semitones
; Window       -    window shape that envelopes each individual grain
; Lowpass      -    engages a lowpass filter, the cutoff frequency of which moves according to the setting for 'Transpose'. 
;                    This can be useful in suppressing quantisation artefacts when Transpose is set to a low value.
;                    It is operational both in MIDI keyboard and standard modes of operation.

; --Randomisation--
; Trans.Mod.    -    randomisation of transposition (in octaves)
; Ptr.Mod.      -    randomisation of pointer position
; Dens Mod.     -    randomisation of grain density
; Size Mod.     -    randomisation of grain size

; --Density LFO--
; Density       -    depth of LFO modulation of grain density
; Amplitude     -    depth of LFO modulation of grain amplitude
; Size          -    depth of LFO modulation of grain size
; Rate          -    rate of LFO modulation of grain density

; --Voice 2--
; Dens.Ratio    -    ratio of grain density of voice 2 with respect to the main voice (also adjustable using the adjacent number box for precise value input)
; Ptr.Diff.     -    pointer position offset of voice 2 with respect to the main voice (also adjustable using the adjacent number box for precise value input)
; Trans.Diff.   -    transposition offset of voice 2 with respect to the main voice (also adjustable using the adjacent number box for precise value input)
; Delay         -    a delay applied to voice 2 which is defined as a ratio of the gap between grains (therefore delay time will be inversely proportional to garin density)
;                    This is a little like a phase offset for voice 2 with respect to that of the main voice.
;                    When using this control 'Dens.Ratio' should be '1' otherwise continuous temporally shifting between the grains of voice 2 and the main voice will be occurring anyway.

; --Envelope--
; Attack        -    amplitude envelope attack time for the envelope applied to complete notes
; Release       -    amplitude envelope release time for the envelope applied to complete notes

; --Control--
; MIDI Ref.     -    MIDI note that represent unison (no transposition) for when using the MIDI keyboard
; Level         -    output amplitude control


<Cabbage>
form caption("grain3 File Player") size(1040,440), colour(0,0,0), pluginId("G3FP"), guiRefresh(16), guiMode("polling")

#define RSliderStyle # trackerColour(170,135,130), textColour("white"), outlineColour( 50, 15, 10), colour( 90, 45, 50)#

image       bounds(  0,  0,1040,440), file("DarkBrushedMetal.jpeg"), colour( 70, 35, 30), outlineColour("White"), shape("sharp"), line(3)
soundfiler  bounds(  5,  5,1030,175), channel("beg","len"), identChannel("filer1"),  colour(0, 255, 255, 255), fontColour(160, 160, 160, 255), 
label       bounds(7, 5, 560, 14), text(""), align(left), colour(0,0,0,0), fontColour(200,200,200), identChannel("stringbox")

hslider     bounds(  0,180,1030, 15), channel("phs"),   range( 0,1,0,1,0.0001), $RSliderStyle
label       bounds(  0,195,1030, 13), text("Manual"), fontColour("white")

filebutton  bounds(  5,210,  80, 25), text("Open File","Open File"), fontColour("white") channel("filename"), shape("ellipse")
checkbox    bounds(  5,240,  95, 20), channel("PlayStop"), text("Play/Stop"), fontColour:0("white"), fontColour:1("white")
label       bounds(  5,263, 145, 12), text("[or right-click and drag]"), fontColour("white"), align("left")

label       bounds( 90,215, 75, 13), text("Ptr.Mode"), fontColour("white")
combobox    bounds( 90,230, 75, 18), channel("PhsMode"), items("Manual", "Speed"), value(2),fontColour("white")

image       bounds(170,215,240, 60), colour(0,0,0,0), identChannel("speedControlsID"), {
rslider     bounds(  0,  0, 60, 60), channel("spd"),     range( -2.00, 2.00, 1), text("Speed"), $RSliderStyle
button      bounds( 60, 15, 60, 18), channel("freeze"),  colour(  0,  0,  0), text("Freeze","Freeze"), fontColour:0(70,70,70), fontColour:1(255,255,255)
rslider     bounds(120,  0, 60, 60), channel("range"),   range(0.01,  1,  1),              text("Range"), $RSliderStyle
label       bounds(180,  0, 60, 13), text("Shape"), fontColour("white")
combobox    bounds(180, 15, 60, 18), channel("shape"), items("phasor", "tri."), value(1), fontColour("white")
}

rslider     bounds(410,215, 60, 60), channel("dur"),     range(0.01,5.00,0.15,0.5,0.001), text("Size"), $RSliderStyle
rslider     bounds(470,215, 60, 60), channel("dens"),    range( 0.2, 500,  20, 0.5),     text("Density"), $RSliderStyle
rslider     bounds(530,215, 60, 60), channel("pch"),     range(-2,2,1,1,0.001),              text("Transpose"), $RSliderStyle
label       bounds(595,210, 75, 13), text("Window"), fontColour("white")
combobox    bounds(595,225, 75, 18), channel("wfn"), items("Hanning","Perc. 1","Perc. 2","Perc. 3","Gate","Rev Perc. 1","Rev.Perc 2","Rev.Perc 3"), value(1),fontColour("white")
checkbox    bounds(595,245, 75, 18), channel("LPF"), text("Lowpass"), fontColour:0("White"), fontColour:1("White")

image       bounds(680,202,260,75), colour(0,0,0,0), outlineColour("grey"), outlineThickness(1), shape("sharp"), plant("randomise"), { 
label       bounds(  0,  3,260, 8), text("R  A  N  D  O  M  I  S  E"), fontColour("white")
rslider     bounds( 10, 13, 60, 60), channel("fmd"),     range(    0,0.3,   0,0.5,0.0001), text("Trans.Mod."), $RSliderStyle
rslider     bounds( 70, 13, 60, 60), channel("pmd"),     range(    0, 1,    0,0.25,0.00001),  text("Ptr.Mod."), $RSliderStyle
rslider     bounds(130, 13, 60, 60), channel("DensRnd"), range(    0, 2,    0), text("Dens.Mod."), $RSliderStyle
rslider     bounds(190, 13, 60, 60), channel("SizeRnd"), range(    0, 8,    0), text("Size.Mod."), $RSliderStyle
}

image      bounds(  5,282,505,75), colour(0,0,0,0), outlineColour("grey"), outlineThickness(1), shape("sharp"), plant("dual"), { 
label      bounds(  0,  3,505, 8), text("V  O  I  C  E     2"), fontColour("white")
checkbox   bounds( 10, 30, 70, 20), channel("DualOnOff"), text("On/Off"), fontColour:0("white"), fontColour:1("white")
rslider    bounds( 80, 13, 60, 60), channel("DensRatio"),   range(0.5,2,1,0.64,0.00001), text("Dens.Ratio"), $RSliderStyle
nslider    bounds(140, 25, 60, 20), channel("DensRatioN"),   range(0.5,2,1,0.64,0.00001),  textColour("white"), trackerColour(190,170,130), outlineColour(100,100,100)
rslider    bounds(200, 13, 60, 60), channel("PtrDiff"),   range(-1,1,0,1,0.00001), text("Ptr.Diff."), $RSliderStyle
nslider    bounds(260, 25, 60, 20), channel("PtrDiffN"),   range(-1,1,0,1,0.00001), textColour("white"), trackerColour(190,170,130), outlineColour(100,100,100)
rslider    bounds(320, 13, 60, 60), channel("TransDiff"),   range(-2,2,0,1,0.00001), text("Trans.Diff."), $RSliderStyle
nslider    bounds(380, 25, 60, 20), channel("TransDiffN"),   range(-2,2,0,1,0.00001), textColour("white"), trackerColour(190,170,130), outlineColour(100,100,100)
rslider    bounds(440, 13, 60, 60), channel("Delay"),       range(0,1,0,1,0.00001), text("Delay"), $RSliderStyle
}

image      bounds(515,282,250,75), colour(0,0,0,0), outlineColour("grey"), outlineThickness(1), shape("sharp"), plant("LFO"), { 
label      bounds(  0,  3,250, 8), text("L  F  O"), fontColour("white")
rslider    bounds(  5, 13, 60, 60), channel("DensLFODep"), range(-4, 4, 0, 1, 0.001),       text("Density"),   $RSliderStyle
rslider    bounds( 65, 13, 60, 60), channel("AmpLFODep"),  range(-1, 1, 0, 1, 0.001),       text("Amplitude"), $RSliderStyle
rslider    bounds(125, 13, 60, 60), channel("SizeLFODep"), range(-2, 2, 0, 1, 0.001),       text("Size"),      $RSliderStyle
rslider    bounds(185, 13, 60, 60), channel("LFORte"),     range(0.01, 8, 0.1, 0.5, 0.001), text("Rate"),      $RSliderStyle
}

image      bounds(770,282,130,75), colour(0,0,0,0), outlineColour("grey"), outlineThickness(1), shape("sharp"), plant("envelope"), { 
label      bounds(  0,  3,140, 8), text("E   N   V   E   L   O   P   E"), fontColour("white")
rslider    bounds(  5, 13, 60, 60), channel("AttTim"),    range(0, 5, 0, 0.5, 0.001),       text("Attack"), $RSliderStyle
rslider    bounds( 65, 13, 60, 60), channel("RelTim"),    range(0.01, 5, 0.05, 0.5, 0.001), text("Release"), $RSliderStyle
}

image      bounds(905,282,130,75), colour(0,0,0,0), outlineColour("grey"), outlineThickness(1), shape("sharp"), plant("control"), { 
label      bounds(  0,  3,140, 8), text("C   O   N   T   R   O   L"), fontColour("white")
rslider    bounds(  5, 13, 60, 60), channel("MidiRef"),   range(0,127,60, 1, 1),   text("MIDI Ref."), $RSliderStyle
rslider    bounds( 65, 13, 60, 60), channel("level"),     range(  0,  3.00, 0.7, 0.5, 0.001), text("Level"), $RSliderStyle
}
                                  
keyboard bounds(5,360,1030, 75)
}
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-n -+rtmidi=NULL -M0 -dm0
</CsOptions>

<CsInstruments>

; sr set by host
ksmps  = 64
nchnls = 2
0dbfs  = 1

massign    0,3
gichans       init    0
giReady       init    0
gSfilepath    init    ""

; WINDOWING FUNCTIONS USED TO DYNAMICALLY SHAPE THE GRAINS
; NUM | INIT_TIME | SIZE | GEN_ROUTINE | PARTIAL_NUM | STRENGTH | PHASE
; GRAIN ENVELOPE WINDOW FUNCTION TABLES:
giwfn1    ftgen    0,  0, 131072,  20,   2, 1                                    ; HANNING
iTime     =        15000
giwfn2    ftgen    0,  0, 131072,  7,    0, iTime, 1, 131072-iTime, 0            ; PERCUSSIVE - GEN08
giwfn3    ftgen    0,  0, 131072,  7,    0, 3072,   1, 128000,    0              ; PERCUSSIVE - STRAIGHT SEGMENTS
giwfn4    ftgen    0,  0, 131072,  5,0.001, 3072,   1, 128000,0.001              ; PERCUSSIVE - EXPONENTIAL SEGMENTS
giwfn5    ftgen    0,  0, 131072,  7,    0, 1536,   1, 128000,    1, 1536, 0     ; GATE - WITH DE-CLICKING RAMP UP AND RAMP DOWN SEGMENTS
giwfn6    ftgen    0,  0, 131072,  7,    0, 131072-iTime, 1, iTime, 0            ; REVERSE PERCUSSIVE - GEN08
giwfn7    ftgen    0,  0, 131072,  7,    0, 128000, 1, 3072,      0              ; REVERSE PERCUSSIVE - STRAIGHT SEGMENTS
giwfn8    ftgen    0,  0, 131072,  5,0.001, 128000, 1, 3072,  0.001              ; REVERSE PERCUSSIVE - EXPONENTIAL SEGMENTS

giTriangle    ftgen    0, 0, 4097,  20, 3


opcode    Grain3b,a,kkkkkkkkkkiiikkiiikk
 kpch, kphs, kspd, kfreeze, krange, kshape, kfmd, kpmd, kdur, kdens, imaxovr, isfn, iwfn, kfrpow, kprpow , iseed, imode, iPhsMode, kDensRnd, kSizeRnd    xin
 
 if iPhsMode==1 then
  kptr         =     kphs * (nsamp(isfn)/ftlen(isfn))    ;MATHEMATICALLY REINTERPRET USER INPUTTED PHASE VALUE INTO A FORMAT THAT IS USABLE AS AN INPUT ARGUMENT  BY THE grain3 OPCODE
 
 elseif iPhsMode==2 then
  kspd         *=       1 - kfreeze
  if kshape==1 then
   kptr        phasor   (kspd * sr) / (nsamp(isfn) * krange)
   kpch        =        kpch-kspd
  elseif kshape==2 then
   kptr        oscili   1, (kspd * sr) / (nsamp(isfn) * krange * 2), giTriangle
   kptrPrev    init     0
   kpch        =        kptr > kptrPrev ? kpch - kspd : kpch + kspd
   kptrPrev    =        kptr
  endif
  kptr         *=       krange     
  kptr         mirror   kptr + kphs, 0, 1
 
  kptr         =        kptr * (nsamp(isfn) / ftlen(isfn) )    ; MATHEMATICALLY REINTERPRET USER INPUTTED PHASE VALUE INTO A FORMAT THAT IS USABLE AS AN INPUT ARGUMENT  BY THE grain3 OPCODE
 
 endif
 
 kpch          =        (sr * kpch) / (ftlen(isfn) )            ; MATHEMATICALLY REINTERPRET USER INPUTTED PITCH RATIO VALUE INTO A FORMAT THAT IS USABLE AS AN INPUT ARGUMENT BY THE grain3 OPCODE - ftlen(x) FUNCTION RETURNS THE LENGTH OF A FUNCTION TABLE (no. x), REFER TO MANUAL FOR MORE INFO.    
 ;kfmd          =        (sr * (kfmd * kpch)) / ftlen(isfn)
 kfmd          =        (sr * (kfmd)) / ftlen(isfn)
 
 ktrig         metro    kdens                                   ; TRIGGERS IN SYNC WITH GRAIN GENERATION
 
 kDensRnd      trandom  ktrig, -kDensRnd, kDensRnd              ; CREATE A RANDOM OFFSET FACTOR THAT WILL BE APPLIED TO FOR DENSITY
 kdens         *=       octave(kDensRnd)
 
 kSizeRnd      trandom  ktrig, -kSizeRnd, kSizeRnd              ; CREATE A RANDOM OFFSET FACTOR THAT WILL BE APPLIED TO FOR DENSITY
 kdur          *=       octave(kSizeRnd)
 
 asig          grain3   kpch, kptr, kfmd, kpmd, kdur, kdens, imaxovr, isfn, iwfn, kfrpow, kprpow , iseed, imode
 xout          asig
 
endop

opcode    NextPowerOf2i,i,i
 iInVal    xin
 icount    =     1
 LOOP:
 if 2^icount>iInVal then
  goto DONE
 else
  icount    =    icount + 1
  goto LOOP
 endif
 DONE:
            xout 2 ^ icount
endop

opcode FileNameFromPath,S,S              ; Extract a file name (as a string) from a full path (also as a string)
 Ssrc     xin                            ; Read in the file path string
 icnt     strlen    Ssrc                 ; Get the length of the file path string
 LOOP:                                   ; Loop back to here when checking for a backslash
 iasc     strchar   Ssrc, icnt           ; Read ascii value of current letter for checking
 if iasc==92 igoto ESCAPE                ; If it is a backslash, escape from loop
          loop_gt   icnt,1,0,LOOP        ; Loop back and decrement counter which is also used as an index into the string
 ESCAPE:                                 ; Escape point once the backslash has been found
 Sname    strsub    Ssrc, icnt+1, -1     ; Create a new string of just the file name
          xout      Sname                ; Send it back to the caller instrument
endop

opcode ParallelWidgets, k, SS
SStr1,SStr2 xin
k1  chnget SStr1
k2  chnget SStr2
if changed:k(k1)==1 then
 chnset k1,SStr2
elseif changed:k(k2)==1 then
 chnset k2,SStr1    
endif
xout k1
endop

instr    1
 kPortTime    linseg    0, 0.01, 0.001
 gkloop       chnget    "loop"
 gkPlayStop   chnget    "PlayStop"

 gkPhsMode    chnget    "PhsMode"
 gkPhsMode    init    1
 gklevel      chnget    "level"
 gklevel      port      gklevel, 0.05
 gkdur        chnget    "dur"
 gkpch        chnget    "pch"
 gkpch        portk     gkpch, kPortTime
 gkLPF        chnget    "LPF"
 
 gkwfn        chnget    "wfn"
 gkwfn        init    1
 gkspd        chnget    "spd"
 gkfreeze     chnget    "freeze"
 gkrange      chnget    "range"
 gkshape      chnget    "shape"
 gkfmd        chnget    "fmd"
 gkpmd        chnget    "pmd"
 gkDensRnd    chnget    "DensRnd"
 gkSizeRnd    chnget    "SizeRnd"
 gkDensLFODep chnget    "DensLFODep"
 gkAmpLFODep  chnget    "AmpLFODep"
 gkSizeLFODep chnget    "SizeLFODep"
 gkLFORte     chnget    "LFORte"
 gkDualOnOff  chnget    "DualOnOff"
 gkDensRatio  ParallelWidgets  "DensRatio","DensRatioN"
 gkPtrDiff    ParallelWidgets  "PtrDiff","PtrDiffN"
 gkTransDiff  ParallelWidgets  "TransDiff","TransDiffN"
 gkDelay      chnget    "Delay"
 gkDelay      port      gkDelay,0.1
 
 if changed(gkPhsMode)==1 then
  if gkPhsMode==1 then
              chnset    "visible(0)", "speedControlsID"
  elseif gkPhsMode==2 then
              chnset    "visible(1)", "speedControlsID"
  endif
 endif
 
 gSfilepath   chnget    "filename"
 kNewFileTrg  changed   gSfilepath        ; if a new file is loaded generate a trigger
 if kNewFileTrg==1 then                   ; if a new file has been loaded...
              event     "i",99,0,0        ; call instrument to update sample storage function table 
 endif  

 /* START/STOP SOUNDING INSTRUMENT */
 ktrig        trigger    gkPlayStop, 0.5, 0
              schedkwhen ktrig, 0, 0, 2, 0, -1

 /* MOUSE SCRUBBING */
 gkMOUSE_DOWN_RIGHT  chnget    "MOUSE_DOWN_RIGHT"    ; Read in mouse left click status
 kStartScrub        trigger    gkMOUSE_DOWN_RIGHT,0.5,0
 if gkMOUSE_DOWN_RIGHT==1 then
  if kStartScrub==1 then 
   reinit RAMP_FUNC
  endif
  RAMP_FUNC:
  krampup     linseg      0, 0.001, 1
  rireturn
  kMOUSE_X    chnget      "MOUSE_X"
  kMOUSE_Y    chnget      "MOUSE_Y"
  kMOUSE_X    =           (kMOUSE_X - 5) / 930
  kMOUSE_Y    portk       1 - ((kMOUSE_Y - 5) / 170), krampup * 0.05        ; SOME SMOOTHING OF DENSITY CHANGES VIA THE MOUSE ENHANCES PERFORMANCE RESULTS. MAKE ANY ADJUSTMENTS WITH ADDITIONAL CONSIDERATION OF guiRefresh VALUE 
  gkphs       limit       kMOUSE_X,0,1
  gkdens      limit       ((kMOUSE_Y^3) * 499) + 1, 1, 500
              schedkwhen  kStartScrub, 0, 0, 2, 0, -1
 else
  gkphs       chnget      "phs"
  gkdens      chnget      "dens"
 endif
 
endin

instr    99    ; load sound file
 gichans      filenchnls    gSfilepath                            ; derive the number of channels (mono=1,stereo=2) in the sound file
 iFtlen       NextPowerOf2i filelen:i(gSfilepath)*sr
 gitableL     ftgen         1, 0, iFtlen, 1, gSfilepath, 0, 0, 1
 if gichans==2 then
  gitableR    ftgen         2, 0, iFtlen, 1, gSfilepath, 0, 0, 2
 endif                                                                                 
 giReady      =             1                                     ; if no string has yet been loaded giReady will be zero
 Smessage     sprintfk      "file(%s)", gSfilepath                ; print sound file to viewer
              chnset        Smessage, "filer1"

 /* WRITE FILE NAME TO GUI */
 Sname FileNameFromPath     gSfilepath                            ; Call UDO to extract file name from the full path
 Smessage     sprintfk      "text(%s)",Sname
              chnset        Smessage, "stringbox"

endin

instr    2    ; triggered by 'play/stop' button

 if gkPlayStop==0&&gkMOUSE_DOWN_RIGHT==0 then
  turnoff
 endif
 if giReady = 1 then                                      ; i.e. if a file has been loaded
  
  /* ENVELOPE */
  iAttTim     chnget        "AttTim"                      ; read in widgets
  iRelTim     chnget        "RelTim"
  if iAttTim>0 then                                       ; is amplitude envelope attack time is greater than zero...
   aenv       cossegr       0, iAttTim, 1, iRelTim, 0     ; create an amplitude envelope with an attack, a sustain and a release segment (senses realtime release)
  else            
   aenv       cossegr       1, iRelTim, 0                 ; create an amplitude envelope with a sustain and a release segment (senses realtime release)
  endif

  kporttime   linseg        0, 0.001, 0.05                ; portamento time function. (Rises quickly from zero to a held value.)

  kSwitch     changed       gkPhsMode, gkwfn
  if    kSwitch==1    then                                ; IF I-RATE VARIABLE CHANGE TRIGGER IS '1'...
      reinit    START                                     ; BEGIN A REINITIALISATION PASS FROM LABEL 'START'
  endif
  START:
  iwfn        =             giwfn1 + i(gkwfn) - 1
  
  /* LFO */
  kDensLFO    poscil        gkDensLFODep, gkLFORte
  kdens       =             gkdens * octave(kDensLFO)
  kAmpLFO     poscil        gkAmpLFODep * 0.5, gkLFORte
  klevel      =             gklevel * (1 - ((kAmpLFO + (abs(gkAmpLFODep) * 0.5)) ^ 2))
  kSizeLFO    poscil        gkSizeLFODep, gkLFORte
  kdur        =             gkdur * octave(kSizeLFO)
  
  iPhsMode    =             i(gkMOUSE_DOWN_RIGHT)==1?1:i(gkPhsMode)
  
  if gichans==1 then                        ; if mono...
   a1         Grain3b       gkpch, gkphs, gkspd, gkfreeze, gkrange, gkshape, gkfmd, gkpmd, kdur, kdens, 600, 1, iwfn, -2, -2 , rnd(1000), 8, iPhsMode, gkDensRnd, gkSizeRnd
   if gkDualOnOff==1 then
    a1b       Grain3b       gkpch + gkTransDiff, gkphs + gkPtrDiff, gkspd, gkfreeze, gkrange, gkshape, gkfmd, gkpmd, kdur, kdens*gkDensRatio, 600, 1, iwfn, -2, -2 , rnd(1000), 8, iPhsMode, gkDensRnd, gkSizeRnd
    if gkDelay>0 then
     a1b      vdelay        a1b, (gkDelay * 1000) / kdens, 5000
    endif
    a1    +=    a1b
   endif
   if gkLPF==1 then  ; anti-quantisation filter
    kcf        =             limit:k(abs(gkpch),0.001,1)^2
    a1         butlp         a1, (sr/2) * a(kcf)
   endif
              outs          a1 * aenv * klevel, a1 * aenv * klevel        ; send mono audio to both outputs 
  elseif gichans==2 then                        ; otherwise, if stereo...
   a1         Grain3b       gkpch, gkphs, gkspd, gkfreeze, gkrange, gkshape, gkfmd, gkpmd, kdur, kdens, 600, 1, iwfn, -2, -2 , rnd(1000), 8, iPhsMode, gkDensRnd, gkSizeRnd
   a2         Grain3b       gkpch, gkphs, gkspd, gkfreeze, gkrange, gkshape, gkfmd, gkpmd, kdur, kdens, 600, 2, iwfn, -2, -2 , rnd(1000), 8, iPhsMode, gkDensRnd, gkSizeRnd
   if gkDualOnOff==1 then
    a1b       Grain3b       gkpch+gkTransDiff, gkphs+gkPtrDiff, gkspd, gkfreeze, gkrange, gkshape, gkfmd, gkpmd, kdur, kdens*gkDensRatio, 600, 1, iwfn, -2, -2 , rnd(1000), 8, iPhsMode, gkDensRnd, gkSizeRnd
    a2b       Grain3b       gkpch+gkTransDiff, gkphs+gkPtrDiff, gkspd, gkfreeze, gkrange, gkshape, gkfmd, gkpmd, kdur, kdens*gkDensRatio, 600, 2, iwfn, -2, -2 , rnd(1000), 8, iPhsMode, gkDensRnd, gkSizeRnd
    if gkDelay>0 then
     a1b      vdelay        a1b, (gkDelay * 1000) / kdens, 5000
     a2b      vdelay        a2b, (gkDelay * 1000) / kdens, 5000
    endif
    a1        +=            a1b
    a2        +=            a2b
   endif
   if gkLPF==1 then  ; anti-quantisation filter
    kcf        =             limit:k(abs(gkpch),0.001,1)^2
    a1         butlp         a1, (sr/2) * a(kcf)
    a2         butlp         a2, (sr/2) * a(kcf)
   endif
               outs          a1 * aenv * klevel, a2 * aenv * klevel        ; send stereo signal to outputs
  endif
  rireturn

 endif
endin

instr    3 ; MIDI note triggered version
 icps         cpsmidi                                   ; read in midi note data as cycles per second
 iamp         ampmidi       1                           ; read in midi velocity (as a value within the range 0 - 1)
 kBend        pchbend       0, 12
 iAttTim      chnget        "AttTim"                    ; read in widgets
 iRelTim      chnget        "RelTim"
 iMidiRef     chnget        "MidiRef"

 if giReady = 1 then                                    ; i.e. if a file has been loaded
  iAttTim     chnget        "AttTim"                        ; read in widgets
  iRelTim     chnget        "RelTim"
  if iAttTim>0 then                                     ; is amplitude envelope attack time is greater than zero...
   aenv       cossegr       0, iAttTim, 1, iRelTim, 0   ; create an amplitude envelope with an attack, a sustain and a release segment (senses realtime release)
  else            
   aenv       cossegr       1, iRelTim, 0               ; create an amplitude envelope with a sustain and a release segment (senses realtime release)
  endif

  kporttime   linseg        0, 0.001, 0.05              ; portamento time function. (Rises quickly from zero to a held value.)

  kBend       portk         kBend, kporttime
  kdens       =             icps * semitone(kBend) * (cpsmidinn(iMidiRef)/cpsmidinn(60))
  
  kSwitch     changed       gkPhsMode, gkwfn
  if    kSwitch==1    then                        ; IF I-RATE VARIABLE CHANGE TRIGGER IS '1'...
      reinit    START                                ; BEGIN A REINITIALISATION PASS FROM LABEL 'START'
  endif
  START:
  iwfn        =             giwfn1 + i(gkwfn) - 1

  /* LFO */
  kDensLFO    poscil        gkDensLFODep, gkLFORte
  kdens       =             kdens * octave(kDensLFO)
  kAmpLFO     poscil        gkAmpLFODep * 0.5, gkLFORte
  klevel      =             gklevel * (1 - ((kAmpLFO + (abs(gkAmpLFODep) * 0.5)) ^ 2))
  kSizeLFO    poscil        gkSizeLFODep, gkLFORte
  kdur        =             gkdur * octave(kSizeLFO)

  iPhsMode    =             i(gkMOUSE_DOWN_RIGHT) == 1 ? 1 : i(gkPhsMode)

  if          gichans==1 then                            ; if mono...
   a1         Grain3b       gkpch, gkphs, gkspd, gkfreeze, gkrange, gkshape, gkfmd, gkpmd, kdur, kdens, 600, 1, iwfn, -2, -2 , rnd(1000), 8, iPhsMode, gkDensRnd, gkSizeRnd
   if gkDualOnOff==1 then
    a1b       Grain3b       gkpch + gkTransDiff, gkphs + gkPtrDiff, gkspd, gkfreeze, gkrange, gkshape, gkfmd, gkpmd, kdur, kdens*gkDensRatio, 600, 1, iwfn, -2, -2 , rnd(1000), 8, iPhsMode, gkDensRnd, gkSizeRnd
    if gkDelay>0 then
     a1b      vdelay        a1b, (gkDelay * 1000) / kdens, 5000
    endif
    a1        +=            a1b
   endif
              outs          a1 * aenv * klevel, a1 * aenv * klevel        ; send mono audio to both outputs 
   if gkLPF==1 then  ; anti-quantisation filter
    kcf        =             limit:k(abs(gkpch),0.001,1)^2
    a1         butlp         a1, (sr/2) * a(kcf)
   endif
  elseif gichans==2 then                                                  ; otherwise, if stereo...
   a1         Grain3b        gkpch, gkphs, gkspd, gkfreeze, gkrange, gkshape, gkfmd, gkpmd, kdur, kdens, 600, 1, iwfn, -2, -2 , rnd(1000), 8, iPhsMode, gkDensRnd, gkSizeRnd
   a2         Grain3b        gkpch, gkphs, gkspd, gkfreeze, gkrange, gkshape, gkfmd, gkpmd, kdur, kdens, 600, 2, iwfn, -2, -2 , rnd(1000), 8, iPhsMode, gkDensRnd, gkSizeRnd
   if gkDualOnOff==1 then
    a1b       Grain3b        gkpch+gkTransDiff, gkphs+gkPtrDiff, gkspd, gkfreeze, gkrange, gkshape, gkfmd, gkpmd, kdur, kdens*gkDensRatio, 600, 1, iwfn, -2, -2 , rnd(1000), 8, iPhsMode, gkDensRnd, gkSizeRnd
    a2b       Grain3b        gkpch+gkTransDiff, gkphs+gkPtrDiff, gkspd, gkfreeze, gkrange, gkshape, gkfmd, gkpmd, kdur, kdens*gkDensRatio, 600, 2, iwfn, -2, -2 , rnd(1000), 8, iPhsMode, gkDensRnd, gkSizeRnd
    if gkDelay>0 then
     a1b      vdelay         a1b, (gkDelay * 1000) / kdens, 5000
     a2b      vdelay         a2b, (gkDelay * 1000) / kdens, 5000
    endif
    a1        +=             a1b
    a2        +=             a2b
   endif
   if gkLPF==1 then  ; anti-quantisation filter
    kcf        =             limit:k(abs(gkpch),0.001,1)^2
    a1         butlp         a1, (sr/2) * a(kcf)
    a2         butlp         a2, (sr/2) * a(kcf)
   endif
              outs           a1 * aenv * klevel, a2 * aenv * klevel        ; send stereo signal to outputs
  endif
  rireturn

 endif

endin

</CsInstruments>  

<CsScore>
i 1 0 [60*60*24*7]
</CsScore>

</CsoundSynthesizer>
