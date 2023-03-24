
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; Spectral Etcher
; Iain McCurdy 2021

; Applies a range of spectral procedures to a chosen sound file

; OPEN FILE - browse for a sound file
; FILE      - play your sound file (once loaded) in a loop
; LIVE      - process live input. This mode is also recommended if you are using Spectral Etching as a plugin in a DAW
; REC OUT - render your work to a sound file (file will be placed in the same directory as the source sound file)

; FFT Size - Equates to frequency resolution (number of freq divisions) and inversely to time resolution

; Speed - speed at which sound file is played back (ratio w.r.t normal speed)
; Pitch - pitch at which sound file is played back (scaling factor)

; FREEZE
; FREQ - freeze frequencies
; AMP - freeze amplitudes
; BOTH - freeze both frequencies and amplitudes
; LOCK - lock phases. Can improve quality and inhibit modulation artefacts when freezing sound.

; HPF - cutoff point of a highpass filter ('brickwall' filter)
; LPF - cutoff point of a lowpass filter ('brickwall' filter)

; LFO: Depth - depth of modulation of of HPF and LPF
; LFO: Rate  - depth of modulation of of HPF and LPF
; LFO: Shape - Rate of modulation of of HPF and LPF (sine, triangle, square, log, exp, wobble, sample and hold)

; On/off switch - activate or bypass erode function
; Erode - number of significant partials left remaining

; On/off switch - activate or bypass gate function
; Threshold - raising this will gate more low amplitude bins


; Scale - scale frequencies (multiplicative)
; Shift - shift frequencies (additive)

; Blur  - average adjacent frequency and amplitude data to create a smoothing, blurring effect 
; two modes are offered using the small radio buttons:
; mode 1 uses averaging of adjacent data bins
; mode 2 uses time smearing by sustaining and mixing value over a define time period 0 to 5 seconds (multiple slider value by 5)
;   (this mode is much more CPU hungry, particularly at high 'blur' values)

; The three button along the right edge of the spectrogram adjust the vertical zoom of the spectrogram

<Cabbage>
form caption("Spectral Etcher") size(1155,260), colour(0,0,0) pluginId("SpEt"), guiMode("queue")
image                 bounds(  0,  0,1155,260), colour(50,50,60), outlineColour("White"), line(3), shape("sharp")
soundfiler            bounds(  5,  5,570,150), channel("beg","len"), channel("filer1"),  colour(0, 255, 255, 255), fontColour(160, 160, 160, 255)
image                 bounds(579,  5,565,150), colour("black")
signaldisplay         bounds(579,  5,565,150), colour("cornflowerblue"), channel("fft"), displayType("spectrogram"), signalVariable("asig","ascan")
label                 bounds(  6,  4,990, 14), text(""), align(left), colour(0,0,0,0), fontColour(200,200,200), channel("stringbox")

button                bounds(1145,  5, 10, 50), text("",""), channel("SpecHigh"), value(0), latched(1), radioGroup(2), colour:1(200,200,100)
button                bounds(1145, 55, 10, 50), text("",""), channel("SpecMed"),  value(0), latched(1), radioGroup(2), colour:1(200,200,100)
button                bounds(1145,105, 10, 50), text("",""), channel("SpecLow"),  value(1), latched(1), radioGroup(2), colour:1(200,200,100)

nslider               bounds(580,128, 60, 25), text("Fundamental"), channel("Fund"),  range(0,20000,0,1,0.01), alpha(0.5)              $RSliderStyle

#define RSliderStyle trackerInsideRadius(.8), trackerOutsideRadius(1), trackerColour(200,200,200), colour( 50, 60, 80), fontColour(200,200,200), textColour(200,200,200),  markerColour(220,220,220), outlineColour(50,50,50), valueTextBox(1)

#define RSliderStyle2 trackerInsideRadius(.8), trackerOutsideRadius(1), trackerColour(200,200,200), colour( 50, 60, 80), fontColour(200,200,200), textColour(200,200,200),  markerColour(220,220,220), outlineColour(50,50,50), valueTextBox(0)

filebutton bounds(  7,158, 83, 20), text("OPEN FILE","OPEN FILE"), fontColour(255,255,100) channel("filename"), shape("ellipse")
button     bounds(  7,180, 41, 20), text("PLAY","PLAY"), channel("PlayLoop"), value(0), latched(1), fontColour:0(50,50,50), fontColour:1(205,255,205), colour:0(0,10,0), colour:1(0,150,0)
button     bounds( 49,180, 41, 20), text("LIVE","LIVE"), channel("Live"), value(1), latched(1), fontColour:0(50,50,50), fontColour:1(155,155,255), colour:0(0,0,20), colour:1(50,50,150)
button     bounds(  7,202, 83, 20), text("REC OUT","REC OUT"), channel("RecOut"), value(0), latched(1), fontColour:0(50,50,50), fontColour:1(255,205,205), colour:0(10,0,0), colour:1(150,0,0)
label      bounds(  7,223, 83, 12), text("FFT Size:"), align(centre), fontColour("white")
combobox   bounds( 17,236, 63, 17), channel("FFTSize"), items("16384", "8192", "4096", "2048", "1024", "512", "256", "128", "64"), value(4), fontColour("white")

rslider  bounds( 95,160, 70, 90), text("Speed"),  channel("Speed"),  range(-1,1,1,1,0.01),              $RSliderStyle
rslider  bounds(165,160, 70, 90), text("Pitch"),  channel("Pitch"),  range(0.25,2,1,1,0.01),              $RSliderStyle

line   bounds(245,170, 2, 70), colour("silver")

label      bounds(250,165,80,12), text("FREEZE"), fontColour(200,200,200)
button     bounds(265,185, 50, 14), text("FREQ","FREQ"), channel("FFreq"), value(0), latched(1), fontColour:0(50,50,70), fontColour:1(205,205,255), colour:0(0,0,10), colour:1(50,50,200)
button     bounds(265,200, 50, 14), text("AMP","AMP"), channel("FAmp"), value(0), latched(1), fontColour:0(50,50,70), fontColour:1(205,205,255), colour:0(0,0,10), colour:1(50,50,200)
button     bounds(265,215, 50, 14), text("BOTH","BOTH"), channel("FBoth"), value(0), latched(1), fontColour:0(50,50,70), fontColour:1(205,205,255), colour:0(0,0,10), colour:1(50,50,200)
button     bounds(265,230, 50, 14), text("LOCK","LOCK"), channel("Lock"), value(0), latched(1), fontColour:0(70,70,50), fontColour:1(255,255,200), colour:0(10,10,0), colour:1(200,200,50)

line   bounds(335,170, 2, 70), colour("silver")

rslider  bounds(345,160, 70, 90), text("HPF"),  channel("HPF"),  range(0,1,0,0.333,0.0001),              $RSliderStyle
rslider  bounds(415,160, 70, 90), text("LPF"),  channel("LPF"),  range(0,1,1,0.333,0.0001),              $RSliderStyle

label    bounds(490,163,100,12), text("L  F  O"), fontColour(200,200,200)
rslider  bounds(485,178, 55, 55), text("Depth"),  channel("LFODep"),  range(0,1,0,0.5,0.001),              $RSliderStyle2
rslider  bounds(540,178, 55, 55), text("Rate"),  channel("LFORat"),   range(0.01,50,0.333,0.5,0.001),              $RSliderStyle2
combobox bounds(510,239, 55, 16), channel("LFOShape"), items("Sine", "Tri", "Square", "Log", "Exp", "Wobble", "S&H"), value(1), fontColour("white")

line     bounds(600,170, 2, 70), colour("silver")

rslider  bounds(615,160, 70, 90), text("Erode"),  channel("Erode"),  range(1,200,200,0.75,1),              $RSliderStyle, alpha(1)
checkbox bounds(615,165, 10, 10), channel("ErodeOn"), value(1), shape("square"), colour("yellow")

line     bounds(695,170, 2, 70), colour("silver")

rslider  bounds(710,160, 70, 90), text("Gate"),  channel("GateThresh"),  range(0,0.1,0,0.25,0.0000001),              $RSliderStyle, alpha(0.1)
checkbox bounds(710,165, 10, 10), channel("GateOn"), value(1), shape("square"), colour("yellow")

line     bounds(790,170, 2, 70), colour("silver")

rslider  bounds(805,160, 70, 90), text("Scale"),  channel("Scale"),  range(0.125,4,1,0.5,0.01),              $RSliderStyle
rslider  bounds(875,160, 70, 90), text("Shift"),  channel("Shift"),  range(-4000,4000,0,1,1),              $RSliderStyle

line     bounds(950,170, 2, 70), colour("silver")

rslider  bounds(970,160, 70, 90), text("Blur"),  channel("Blur"),  range(0,1,0,0.5,0.001),              $RSliderStyle
checkbox bounds(960,162, 30, 10), text("1") channel("BlurMode1"), value(1), shape("square"), colour("yellow"), radioGroup(1)
checkbox bounds(960,176, 30, 10), text("2"), channel("BlurMode2"), value(0), shape("square"), colour("yellow"), radioGroup(1)

line     bounds(1040,170, 2, 70), colour("silver")

rslider  bounds(1055,160, 70, 90), text("Gain"),  channel("Gain"),  range(0,50,1,0.5,0.001),              $RSliderStyle

vmeter   bounds(1135,160, 10, 95) channel("vMeter1") value(0) overlayColour(0, 0, 0, 255) meterColour:0(255, 255, 0) meterColour:1(255, 103, 171) meterColour:2(250,250, 0) outlineThickness(1)
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-dm0 -n -+rtmidi=NULL -M0 --displays
</CsOptions>

<CsInstruments>

; sr set by host
ksmps  = 64
nchnls = 2
0dbfs  = 1

massign 0,3

gichans       init     0        ; 
giReady       init     0        ; flag to indicate function table readiness
gSfilepath    init     ""

giFFTSizes[] array     16384, 8192, 4096, 2048, 1024, 512, 256, 128, 64    ; an array is used to store FFT window sizes

giGateFN     ftgen     0,0,16384, 7, 1, 16384, 1

giSine       ftgen     501, 0, 4096, 10, 1
giTri        ftgen     502, 0, 4096, 7,  0, 1024, 1, 2048, -1, 1024, 0
giSqu        ftgen     503, 0, 4096, 7,  1, 2048, 1, 0, -1, 2048, -1
giExp        ftgen     504, 0, 4096, -19, 0.5, 2, 0, -1
giLog        ftgen     505, 0, 4096, -19, 0.5, 2, 180, 1



opcode FileNameFromPath,S,S                ; Extract a file name (as a string) from a full path (also as a string)
 Ssrc        xin                          ; Read in the file path string
 icnt        strlen    Ssrc               ; Get the length of the file path string
 LOOP:                                     ; Loop back to here when checking for a backslash
 iasc        strchar   Ssrc, icnt         ; Read ascii value of current letter for checking
 if iasc==92 igoto ESCAPE                  ; If it is a backslash, escape from loop
             loop_gt   icnt,1,0,LOOP      ; Loop back and decrement counter which is also used as an index into the string
 ESCAPE:                                   ; Escape point once the backslash has been found
 Sname       strsub    Ssrc, icnt + 1, -1 ; Create a new string of just the file name
             xout      Sname              ; Send it back to the caller instrument
endop


instr    1

; LOAD FILE 
 gSfilepath  cabbageGetValue   "filename"
 kNewFileTrg changed  gSfilepath        ; if a new file is loaded generate a trigger
 if kNewFileTrg==1 then                    ; if a new file has been loaded...
             event    "i",99,0,0.01     ; call instrument to update sample storage function table 
 endif   

; READ IN WIDGETS
gkSpeed      cabbageGetValue   "Speed"
gkPitch      cabbageGetValue   "Pitch"
gkHPF        cabbageGetValue   "HPF"
gkLPF        cabbageGetValue   "LPF"
gkFFreq      cabbageGetValue   "FFreq"
gkFAmp       cabbageGetValue   "FAmp"
gkFFreq      cabbageGetValue   "FFreq"
gkFBoth      cabbageGetValue   "FBoth"
gkLock       cabbageGetValue   "Lock"
gkGain       cabbageGetValue   "Gain"
gkGain       port     gkGain, 0.1
gkLFODep     cabbageGetValue   "LFODep"
gkLFORat     cabbageGetValue   "LFORat"
gkLFOShape   cabbageGetValue   "LFOShape"
gkGateOn     cabbageGetValue   "GateOn"
if trigger:k(gkFBoth,0.5,0)==1 then
             cabbageSetValue   "FFreq", k(1)
             cabbageSetValue   "FAmp", k(1)
elseif trigger:k(gkFBoth,0.5,1)==1 then
             cabbageSetValue   "FFreq", k(0)
             cabbageSetValue   "FAmp", k(0)
endif
gkErode      cabbageGetValue   "Erode"
gkErodeOn    cabbageGetValue   "ErodeOn"
if changed:k(gkErodeOn)==1 then
 if gkErodeOn==1 then
             cabbageSet        k(1),"Erode", "alpha", 1
 else
             cabbageSet        k(1),"Erode", "alpha", 0.1
 endif
endif
if changed:k(gkGateOn)==1 then
 if gkGateOn==1 then
             cabbageSet        k(1),"GateThresh","alpha", 1
 else
             cabbageSet        k(1),"GateThresh","alpha", 0.1
 endif
endif

gkScale      =        port:k(cabbageGetValue:k("Scale"),0.1)
gkShift      =        port:k(cabbageGetValue:k("Shift"),0.1)
gkBlurMode1  cabbageGetValue   "BlurMode1"
gkBlurMode2  cabbageGetValue   "BlurMode2"
gkBlur       cabbageGetValue   "Blur"
gkSmooth     =        (1 - gkBlur)^8

kPlayLoop    cabbageGetValue   "PlayLoop"
if changed:k(kPlayLoop)==1 then
 if kPlayLoop==1 then
             event    "i", 2, 0, -1
 else
             turnoff2 2, 0, 1
 endif
endif

kLive cabbageGetValue "Live"
if changed:k(kLive)==1 then
 if kLive==1 then
             event    "i", 4, 0, -1
 else
             turnoff2 4, 0, 1
 endif
endif

if changed:k(cabbageGetValue:k("FFTSize"))==1&&timeinstk()>1&&kPlayLoop==1 then
             event    "i", 2, 0, -1
endif 

;Record
kRecOut      cabbageGetValue   "RecOut"
if changed:k(kRecOut)==1 then
 if kRecOut==1 then
             event    "i", 1000, 0, -1
 else
             turnoff2 1000, 0, 1
 endif
endif

endin

instr    99    ; load sound file
 
 /* write file selection to function tables */
 gichans       filenchnls gSfilepath                ; derive the number of channels (mono=1,stereo=2) in the sound file
 gitableL      ftgen      1,0,0,1,gSfilepath,0,0,1
 if gichans==2 then
  gitableR     ftgen      2,0,0,1,gSfilepath,0,0,2
 endif
 giReady       =    1                               ; if no string has yet been loaded giReady will be zero
 Smessage      sprintfk   "file(%s)", gSfilepath    ; print sound file image to fileplayer
               ;chnset     Smessage, "filer1"
               cabbageSet "filer1","file",gSfilepath
 /* write file name to GUI */
 Sname FileNameFromPath   gSfilepath                ; Call UDO to extract file name from the full path
 Smessage      sprintfk   "text(%s)",Sname          ; create string to update text() identifier for label widget
               cabbageSet "stringbox","text",Sname
endin






instr 2 ; triggered by button

if strlen:i(gSfilepath)==0 then
 turnoff2 p1,0,0
endif

kScale     =         gkScale

#define SPECTRAL_ETCHER
#
iFFTSize    cabbageGetValue    "FFTSize"
iFFTSize    =    giFFTSizes[iFFTSize-1]

; PLAYBACK
kamp      =         1
ktabL     =         1
ktabR     =         2
kdetect   =         0
kwrap     =         1
ioffset   =         0
idbthresh =         1
fsrc      pvstanal  gkSpeed, kamp, gkPitch, ktabL, kdetect, kwrap, ioffset, iFFTSize, iFFTSize/4, idbthresh

; FREEZE
fFreeze   pvsfreeze fsrc, gkFAmp, gkFFreq
fLock     pvslock   fFreeze, gkLock

;LFO
if changed:k(gkLFOShape)==1 then
          reinit    LFO_REINIT
endif
LFO_REINIT:
if i(gkLFOShape)>5 then
 if i(gkLFOShape)==6 then
  kLFO    randi     gkLFODep, gkLFORat
 else
  kLFO    randh     gkLFODep, gkLFORat
 endif
else
 kLFO     oscili    gkLFODep, gkLFORat, 500 + i(gkLFOShape)
endif
rireturn

; LPF/HPF
kHPF      =         int(limit:k((gkHPF+kLFO)*((iFFTSize/2) + 1), 1,(iFFTSize/2) + 1))
kHPF      init      1
kLPF      =         int(limit:k((gkLPF + kLFO)*((iFFTSize/2) + 1), kHPF+1,(iFFTSize/2) + 1))
kLPF      init      (iFFTSize/2) + 1
if changed:k(kHPF,kLPF)==1 then
          reinit    UPDATE_FILTER
endif
UPDATE_FILTER:
iMask     ftgen     201, 0, (iFFTSize/2) + 1, 17,    0, 0,   i(kHPF), 1, i(kLPF), 0
fFilt     pvsmaska  fLock, iMask, 1
rireturn

kBins[]   init      (iFFTSize/2) + 1

; Erode
if gkErodeOn==1 then
 fTrace, kBins[] pvstrace fFilt, gkErode, 1
else
 fTrace   pvstrace  fFilt, iFFTSize/2 + 1
endif

; Gate
gkGateThresh  cabbageGetValue "GateThresh"
if gkGateOn==1 then
 fGate    pvstencil fTrace, k(0), gkGateThresh, giGateFN
else
 fGate    =         fTrace
endif

; Scale
fScal    pvscale    fGate, kScale

; Shift
fScal    pvshift    fScal, gkShift, 0

; Blur/Smooth
if gkBlurMode1==1 then
 fBlur   pvsmooth   fScal, gkSmooth, gkSmooth                       
else
 fBlur   pvsblur    fScal, gkBlur*3, 3
endif
              
a1       pvsynth    fBlur

if gichans==1 then
 a2      =          a1
else
ifn      =          gitableR
; PLAYBACK
fsrcR    pvstanal   gkSpeed, kamp, gkPitch, ktabR, kdetect, kwrap, ioffset, iFFTSize, iFFTSize/4, idbthresh

; FREEZE
fFreezeR pvsfreeze  fsrcR, gkFAmp, gkFFreq
;fFreeze pvsmooth  fsrc, 0.005, 0.005
fLockR   pvslock    fFreezeR, gkLock

; LPF/HPF
if changed:k(kHPF,kLPF)==1 then
         reinit     UPDATE_FILTER2
endif
UPDATE_FILTER2:
fFiltR   pvsmaska   fLockR, iMask, 1
rireturn

; Erode
if gkErodeOn==1 then
 fTraceR pvstrace   fFiltR, gkErode
else
 fTraceR pvstrace   fFiltR, iFFTSize/2 + 1
endif

; Gate
if gkGateOn==1 then
 fGateR  pvstencil  fTraceR, k(0), gkGateThresh, giGateFN
else
 fGateR  =          fTraceR
endif

; Scale
fScalR   pvscale    fGateR, kScale

; Shift
fScalR   pvshift    fScalR, gkShift, 0

; Blur/Smooth
if gkBlurMode1==1 then
 fBlurR  pvsmooth   fScalR, gkSmooth, gkSmooth                       
else
 fBlurR  pvsblur    fScalR, gkBlur*3, 3
endif
                                  
a2       pvsynth    fBlurR

endif

aEnv     linsegr    0,0.1,0,0.01,1,0.05,0                                                
a1       *=         gkGain*aEnv
a2       *=         gkGain*aEnv

         outs       a1, a2

#
$SPECTRAL_ETCHER

 ; PITCH TRACKING
 kamp,kfr pvsbin fBlur, kBins[0]
         cabbageSetValue "Fund",kfr*kScale + gkShift
 ; SCRUBBER
 iLen    =          ftlen(gitableL)/sr
 kPtr    phasor     gkSpeed/iLen
 ;Smsg    sprintfk   "scrubberPosition(%d)", kPtr * sr
         cabbageSet "filer1","scrubberPosition",kPtr * sr
endin


instr 3 ; MIDI triggered

if strlen:i(gSfilepath)==0 then
         turnoff2   p1,0,0
endif

iCPS     cpsmidi

kScale   =          gkScale * iCPS/cpsmidinn(60)

$SPECTRAL_ETCHER

endin




; Live
instr 4

iFFTSize    cabbageGetValue    "FFTSize"
iFFTSize    =          giFFTSizes[iFFTSize-1]

aInL,aInR   ins

; ANALYSIS
fsrcL       pvsanal    aInL, iFFTSize, iFFTSize/4, iFFTSize, 1
fsrcR       pvsanal    aInR, iFFTSize, iFFTSize/4, iFFTSize, 1

; FREEZE
fFreezeL    pvsfreeze  fsrcL, gkFAmp, gkFFreq
fFreezeR    pvsfreeze  fsrcR, gkFAmp, gkFFreq

fLockL      pvslock    fFreezeL, gkLock
fLockR      pvslock    fFreezeR, gkLock

;LFO
if changed:k(gkLFOShape)==1 then
            reinit     LFO_REINIT
endif
LFO_REINIT:
if i(gkLFOShape)>5 then
 if i(gkLFOShape)==6 then
  kLFO      randi      gkLFODep, gkLFORat
 else
  kLFO      randh      gkLFODep, gkLFORat
 endif
else
 ;kLFO  lfo  gkLFODep,gkLFORat,giLFOShapes[i(gkLFOShape)-1]
 kLFO       oscili     gkLFODep, gkLFORat, 500 + i(gkLFOShape)
endif
rireturn

; LPF/HPF
kHPF        =          int(limit:k((gkHPF+kLFO)*((iFFTSize/2) + 1), 1,(iFFTSize/2) + 1))
kHPF        init       1
kLPF        =          int(limit:k((gkLPF+kLFO)*((iFFTSize/2) + 1), kHPF+1,(iFFTSize/2) + 1))
kLPF        init       (iFFTSize/2) + 1
if changed:k(kHPF,kLPF)==1 then
            reinit     UPDATE_FILTER
endif
UPDATE_FILTER:
iMask       ftgen      201, 0, (iFFTSize/2) + 1, 17,    0, 0,   i(kHPF), 1, i(kLPF), 0
fFiltL      pvsmaska   fLockL, iMask, 1
fFiltR      pvsmaska   fLockR, iMask, 1
rireturn

kBins[]     init       (iFFTSize/2) + 1

; Erode
if gkErodeOn==1 then
 fTraceL, kBins[] pvstrace fFiltL, gkErode, 1
 fTraceR    pvstrace   fFiltR, gkErode
else
 fTraceL    pvstrace   fFiltL, iFFTSize/2 + 1
 fTraceR    pvstrace   fFiltR, iFFTSize/2 + 1
endif

; Scale
fScalL      pvscale    fTraceL, gkScale
fScalR      pvscale    fTraceR, gkScale

; Shift
fScalL      pvshift    fScalL, gkShift, 0
fScalR      pvshift    fScalR, gkShift, 0

; Blur/Smooth
if gkBlurMode1==1 then
 fBlurL     pvsmooth   fScalL, gkSmooth, gkSmooth                       
 fBlurR     pvsmooth   fScalR, gkSmooth, gkSmooth                       
else
 fBlurL     pvsblur    fScalL, gkBlur*3, 3
 fBlurR     pvsblur    fScalR, gkBlur*3, 3
endif
              
a1          pvsynth    fBlurL
a2          pvsynth    fBlurR

a1          *=         gkGain
a2          *=         gkGain

            outs       a1, a2

 ; PITCH TRACKING
 kamp,kfr   pvsbin     fBlurL, kBins[0]
            ;chnset     kfr*gkScale + gkShift,"Fund"
            cabbageSetValue "Fund",kfr*gkScale + gkShift
endin



; record
instr 1000
  a1, a2         monitor
  ;ilen           strlen    gSfilepath                     ; Derive string length.
  ;SOutputName    strsub    gSfilepath,0,ilen-4            ; Remove ".wav"
  ;SOutputName    strcat    SOutputName,"_SpectralEtching" ; Add suffix
  ;iDate          date
  ;SDate          sprintf   "%i",iDate
  ;SOutputName    strcat    SOutputName,SDate              ; Add date
  ;SOutputName    strcat    SOutputName,".wav"             ; Add extension
  SPath          chnget    "USER_HOME_DIRECTORY"
  SName          =         "/SpectralEtching.wav"
  SPathName      strcat    SPath,SName
                 fout      SPathName, 8, a1, a2
endin



instr 2000 ; meter and spectrogram

a1,a2     monitor

; meter
if        metro:k(10)==1 then
          reinit REFRESH_METER
          endif
REFRESH_METER:
kres      init      0
kres      limit     kres-0.001,0,1 
kres      peak      a1                            
          rireturn
          cabbageSetValue "vMeter1",kres
if release:k()==1 then
          cabbageSetValue "vMeter1",k(0)
endif

; spectrogram
 aLFO     oscil     0.1, 1
 ascan    oscil     1, p4*0.9+aLFO
 iperiod  init      0.0005
 iwsize   =         2
 imaxbin  init      3
 kFFTGain =         10
 iwsize   =         2048
 asig     =         a1 * kFFTGain
 ascan    =         0
          display   asig, iperiod
 
 kSpecHigh cabbageGetValue "SpecHigh"
 kSpecMed  cabbageGetValue "SpecMed"
 kSpecLow  cabbageGetValue "SpecLow" 
 if changed:k(kSpecHigh,kSpecMed,kSpecLow)==1 then
           reinit RESTART
 endif
 RESTART:
 if i(kSpecHigh)==1 then
  iDiv = 16
 elseif i(kSpecMed)==1 then
  iDiv = 8
 else
  iDiv = 4
 endif
           dispfft         asig, iperiod, iwsize, 0, 0, 0, 0, iwsize/iDiv
 rireturn
           display         ascan, iperiod

endin

</CsInstruments>  

<CsScore>
i 1    0 z
i 2000 0 z
</CsScore>

</CsoundSynthesizer>
