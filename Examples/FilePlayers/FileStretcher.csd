
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; FileStretcher.csd
; Written by Iain McCurdy, 2014/2021

; Used to create a time-stretched version of an input file
; Often when time-stretching a sound when wishing to retain the priniciple characteristics of the source sound,
;  we play the attack portion of the sound at normal speed (as well, often, as the release portion) and only stretch
;  sound in the middle of the source - often called the 'sustain' portion.
; This example facilitates this technique by allowing the user to select the portion of sound to stretch by 
;  clicking and dragging upon the waveform view creating a highlighted area. Time stretching will only be applied to this 
;  portion of the sound.

; The user defines a time-stretching zone by clicking and dragging on the waveform view once a file has been opened.
; 'Duration' defines the total duration of the output file.

; OPEN FILE - open a new file for stretching
; PLAY STR. - play the file with stretching
; RECORD    - render the stretched file OFFLINE
; PLAY NORM.- play the file normally without stretching (useful for auditioning the source file)
; Duration  - duration of the stretched segment in seconds (goes from 0.1 seconds to 1 hour)
; Semitones - transposition of the stretched file in semitones (1/8 tone resolution).
; Cents     - transposition in cents -100 to +100
; HARMONY (button) - engages a second stretched layer that will be unaffected by 'Semitones' and 'Cents'. 
;                    This is useful for beating/detuning/harmony effects.
; Rise      - envelope attack and release times
; FFT Size  - larger values favour frequency resolution, smaller values favour time resolution.
; Phase lock- can prevent modulation artefacts during stretched segments.
; Level     - amplitude level during live performance (rendered file is unaffected by this)
; Jit.Dep   - amount of added jitter during stretched segment (adding jitter can offset the synthetic quality produced by FFT stretcing)
; Jit.Rte.  - rate of jitter modulation

; 'PLAY' auditions the file stretch. 
; 'RECORD' renders the time-stretched audio to disk offline.
; The console reports the name of the output file and a message: 'Done!' when the rendering is complete. 
;    The output file will be created in the same directory as the input file and with the same name but with the word '_stretched' and a unique number suffixed. 

; 'LEVEL' provides a amplitude level control (the rendered file is unaffected and always renders at full amplitude.

; Adding a small amount of jitter to the file reading pointer when time-stretching can reduce the sense of a frozen spectrum 
;  evident during long time stretches. Jitter depth and rate can be modified.

<Cabbage>
form caption("File Stretcher") size(700,315), colour(0,0,0) pluginId("FiSt"), guiRefresh(10) 
image      bounds(  0,  0,700,315), colour(50,50,60), outlineColour("White"), line(3), shape("sharp")

soundfiler bounds(  5,  5,690,140), channel("beg","len"), identChannel("filer1"),  colour(0, 255, 255, 255), fontColour(160, 160, 160, 255)
image      bounds(  5,  5,  1,140), colour(150,150,160), shape("sharp"), identChannel("Scrubber")

;hrange    bounds(  5,145,690,15), text(""), channel("Strt","End"), range(0, 1, 0.2:0.4), colour(30,30,40), trackerColour("white"), fontColour("white"), outlineColour(150,150,150)

label bounds(6, 4, 560, 14), text(""), align(left), colour(0,0,0,0), fontColour(200,200,200), identChannel("stringbox")
label      bounds( 6, 20,450, 13), text("Click and drag on waveform to select sound portion for time stretching..."), align(left), colour("black"), fontColour("white"), visible(0), identChannel("InstructionID")

filebutton bounds(  7,160, 83, 20), text("OPEN FILE","OPEN FILE"), fontColour(255,255,100) channel("filename"), shape("ellipse")
button     bounds(  7,183, 83, 20), text("PLAY STR.","PLAY STR."), channel("PlayStr"), value(0), latched(1), fontColour:0(  0,105, 0), fontColour:1(100,255,100)
button     bounds(  7,206, 83, 20), text("RECORD","RECORD"), channel("Record"), value(0), latched(0), fontColour:0(105,  0,  0), fontColour:1(255,100,100)

combobox   bounds( 96,160, 83, 20), channel("Tuning"), items("12-TET", "24-TET", "12-TET rev.", "24-TET rev.", "10-TET", "36-TET", "Just C", "Just C#", "Just D", "Just D#", "Just E", "Just F", "Just F#", "Just G", "Just G#", "Just A", "Just A#", "Just B"), value(1),fontColour("white")
button     bounds( 96,183, 83, 20), text("PLAY NORM.","PLAY NORM."), channel("PlayNorm"), value(0), latched(1), fontColour:0(  50,  50,105), fontColour:1(150,150,255)
button     bounds( 96,206, 83, 20), text("STOP","STOP"), channel("Stop"), value(0), latched(0), fontColour:0( 50, 50, 55), fontColour:1(150,150,155)

rslider    bounds(178,160, 60, 60), text("Duration"), channel("Duration"), range(0.1, 3600.00, 3,0.25,0.001), colour(30,30,40), trackerColour("white"), fontColour("white"), outlineColour(150,150,150)
rslider    bounds(241,160, 54, 54), text("Semitones"), channel("Semitones"), range(-48, 48.00, 0,1,0.25), colour(30,30,40), trackerColour("white"), fontColour("white"), outlineColour(150,150,150)
rslider    bounds(301,160, 54, 54), text("Cents"), channel("Cents"), range(-100, 100.00, 0,1, 1), colour(30,30,40), trackerColour("white"), fontColour("white"), outlineColour(150,150,150)
button     bounds(251,215,100, 12), text("H A R M O N Y", "H A R M O N Y"), channel("Harmony"), fontColour:0(50,50,50), fontColour:1(205,255,205), colour:0(0,10,0), colour:1(0,150,0), value(0), latched(1)
rslider    bounds(358,160, 60, 60), text("Rise"), channel("Rise"), range(0, 8, 0.02, 0.5, 0.001), colour(30,30,40), trackerColour("white"), fontColour("white"), outlineColour(150,150,150)

label      bounds(420,148, 80, 12), text("FFT Size"), fontColour("white")
combobox   bounds(420,161, 80, 17), channel("FFTSize"), items("32768", "16384", "8192", "4096", "2048", "1024", "512", "256", "128", "64", "32"), value(5), fontColour("white")
checkbox   bounds(420,181, 95, 15), channel("lock"), text("Phase Lock"), fontColour:0("white"), fontColour:1("white"), colour(255,0,0)
label      bounds(420,199, 80, 12), text("Stretch/Freeze"), fontColour("white")
combobox   bounds(420,212, 83, 20), channel("StrFr"), items("Stretch","Freeze"), value(1),fontColour("white")


rslider    bounds(510,160, 60, 60), text("Level"),    channel("Level"),  range(0, 1.00, 0.9),       colour(30,30,40), trackerColour("white"), fontColour("white"), outlineColour(150,150,150)
rslider    bounds(570,160, 60, 60), text("Jit.Dep."), channel("JitDep"), range(0, 1.00, 0.2),       colour(30,30,40), trackerColour("white"), fontColour("white"), outlineColour(150,150,150)
rslider    bounds(630,160, 60, 60), text("Jit.Rte."), channel("JitRte"), range(0.5, 100.00, 2,0.5), colour(30,30,40), trackerColour("white"), fontColour("white"), outlineColour(150,150,150)

;csoundoutput bounds(  0,230,700,200), text("Csound Output")
keyboard     bounds(  5,235,690, 75)

</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-dm0 -n -+rtmidi=NULL -M0
</CsOptions>

<CsInstruments>

; sr set by host
ksmps  = 64
nchnls = 2
0dbfs  = 1

massign 0,2

gichans       init    0        ; 
giReady       init    0        ; flag to indicate function table readiness
gSfilepath    init    ""
giFFTSizes[]  array   32768, 16384, 8192, 4096, 2048, 1024, 512, 256, 128, 64, 32    ; an array is used to store FFT window sizes

;			         FN_NUM | INIT_TIME | SIZE | GEN_ROUTINE | NUM_GRADES | REPEAT |  BASE_FREQ  | BASE_KEY_MIDI | TUNING_RATIOS:-0-|----1----|---2----|----3----|----4----|----5----|----6----|----7----|----8----|----9----|----10-----|---11----|---12---|---13----|----14---|----15---|---16----|----17---|---18----|---19---|----20----|---21----|---22----|---23---|----24----|----25----|----26----|----27----|----28----|----29----|----30----|----31----|----32----|----33----|----34----|----35----|----36----|
giTTable1	ftgen      0,         0,       64,       -2,          12,          2,   cpsmidinn(60),      60,                       1, 1.059463,1.1224619,1.1892069,1.2599207,1.33483924,1.414213,1.4983063,1.5874001,1.6817917,1.7817962, 1.8877471,     2	;STANDARD
giTTable2	ftgen      0,         0,       64,       -2,          24,          2,   cpsmidinn(60),      60,                       1, 1.0293022,1.059463,1.0905076,1.1224619,1.1553525,1.1892069,1.2240532,1.2599207,1.2968391,1.33483924,1.3739531,1.414213,1.4556525,1.4983063, 1.54221, 1.5874001, 1.6339145,1.6817917,1.73107,  1.7817962,1.8340067,1.8877471,1.9430623,    2	;QUARTER TONES
giTTable3	ftgen      0,         0,       64,       -2,          12,        0.5,   cpsmidinn(60),      60,                       2, 1.8877471,1.7817962,1.6817917,1.5874001,1.4983063,1.414213,1.33483924,1.2599207,1.1892069,1.1224619,1.059463,      1	;STANDARD REVERSED
giTTable4	ftgen      0,         0,       64,       -2,          24,        0.5,   cpsmidinn(60),      60,                       2, 1.9430623,1.8877471,1.8340067,1.7817962,1.73107, 1.6817917,1.6339145,1.5874001,1.54221,  1.4983063, 1.4556525,1.414213,1.3739531,1.33483924,1.2968391,1.2599207,1.2240532,1.1892069,1.1553525,1.1224619,1.0905076,1.059463, 1.0293022,    1	;QUARTER TONES REVERSED
giTTable5	ftgen      0,         0,       64,       -2,          10,          2,   cpsmidinn(60),      60,                       1, 1.0717734,1.148698,1.2311444,1.3195079, 1.4142135,1.5157165,1.6245047,1.7411011,1.8660659,     2	;DECATONIC
giTTable6	ftgen      0,         0,       64,       -2,          36,          2,   cpsmidinn(60),      60,                       1, 1.0194406,1.0392591,1.059463,1.0800596, 1.1010566,1.1224618,1.1442831,1.1665286,1.1892067,1.2123255,1.2358939,1.2599204,1.284414,1.3093838, 1.334839, 1.3607891,1.3872436,1.4142125,1.4417056,1.4697332,1.4983057,1.5274337,1.5571279,1.5873994, 1.6182594,1.6497193, 1.6817909, 1.7144859, 1.7478165, 1.7817951, 1.8164343, 1.8517469, 1.8877459, 1.9244448, 1.9618572,      2	;THIRD TONES
giTTable7	ftgen      0,         0,       64,       -2,          12,          2,   cpsmidinn(60),      60,                       1,   16/15,     9/8,     6/5,      5/4,       4/3,     45/32,     3/2,     8/5,      5/3,      9/5,       15/8,    2	;JUST INTONATION                                                                                                                                                                                                                                   
giTTable8	ftgen      0,         0,       64,       -2,          12,          2,   cpsmidinn(61),      61,                       1,   16/15,     9/8,     6/5,      5/4,       4/3,     45/32,     3/2,     8/5,      5/3,      9/5,       15/8,    2	;JUST INTONATION                                                                                                                                                                                                                                   
giTTable9	ftgen      0,         0,       64,       -2,          12,          2,   cpsmidinn(62),      62,                       1,   16/15,     9/8,     6/5,      5/4,       4/3,     45/32,     3/2,     8/5,      5/3,      9/5,       15/8,    2	;JUST INTONATION                                                                                                                                                                                                                                   
giTTable10	ftgen      0,         0,       64,       -2,          12,          2,   cpsmidinn(63),      63,                       1,   16/15,     9/8,     6/5,      5/4,       4/3,     45/32,     3/2,     8/5,      5/3,      9/5,       15/8,    2	;JUST INTONATION                                                                                                                                                                                                                                   
giTTable11	ftgen      0,         0,       64,       -2,          12,          2,   cpsmidinn(64),      64,                       1,   16/15,     9/8,     6/5,      5/4,       4/3,     45/32,     3/2,     8/5,      5/3,      9/5,       15/8,    2	;JUST INTONATION                                                                                                                                                                                                                                   
giTTable12	ftgen      0,         0,       64,       -2,          12,          2,   cpsmidinn(65),      65,                       1,   16/15,     9/8,     6/5,      5/4,       4/3,     45/32,     3/2,     8/5,      5/3,      9/5,       15/8,    2	;JUST INTONATION                                                                                                                                                                                                                                   
giTTable13	ftgen      0,         0,       64,       -2,          12,          2,   cpsmidinn(66),      66,                       1,   16/15,     9/8,     6/5,      5/4,       4/3,     45/32,     3/2,     8/5,      5/3,      9/5,       15/8,    2	;JUST INTONATION                                                                                                                                                                                                                                   
giTTable14	ftgen      0,         0,       64,       -2,          12,          2,   cpsmidinn(67),      67,                       1,   16/15,     9/8,     6/5,      5/4,       4/3,     45/32,     3/2,     8/5,      5/3,      9/5,       15/8,    2	;JUST INTONATION                                                                                                                                                                                                                                   
giTTable15	ftgen      0,         0,       64,       -2,          12,          2,   cpsmidinn(68),      68,                       1,   16/15,     9/8,     6/5,      5/4,       4/3,     45/32,     3/2,     8/5,      5/3,      9/5,       15/8,    2	;JUST INTONATION                                                                                                                                                                                                                                   
giTTable16	ftgen      0,         0,       64,       -2,          12,          2,   cpsmidinn(69),      69,                       1,   16/15,     9/8,     6/5,      5/4,       4/3,     45/32,     3/2,     8/5,      5/3,      9/5,       15/8,    2	;JUST INTONATION                                                                                                                                                                                                                                   
giTTable17	ftgen      0,         0,       64,       -2,          12,          2,   cpsmidinn(70),      70,                       1,   16/15,     9/8,     6/5,      5/4,       4/3,     45/32,     3/2,     8/5,      5/3,      9/5,       15/8,    2	;JUST INTONATION                                                                                                                                                                                                                                   
giTTable18	ftgen      0,         0,       64,       -2,          12,          2,   cpsmidinn(71),      71,                       1,   16/15,     9/8,     6/5,      5/4,       4/3,     45/32,     3/2,     8/5,      5/3,      9/5,       15/8,    2	;JUST INTONATION                                                                                                                                                                                                                                   



opcode FileNameFromPath,S,S        ; Extract a file name (as a string) from a full path (also as a string)
 Ssrc    xin                       ; Read in the file path string
 icnt    strlen    Ssrc            ; Get the length of the file path string
 LOOP:                             ; Loop back to here when checking for a backslash
 iasc    strchar Ssrc, icnt        ; Read ascii value of current letter for checking
 if iasc==92 igoto ESCAPE          ; If it is a backslash, escape from loop
 loop_gt    icnt,1,0,LOOP          ; Loop back and decrement counter which is also used as an index into the string
 ESCAPE:                           ; Escape point once the backslash has been found
 Sname    strsub Ssrc, icnt+1, -1  ; Create a new string of just the file name
    xout    Sname                  ; Send it back to the caller instrument
endop


instr    1
 gkPlayStr     chnget    "PlayStr"
 gkPlayNorm    chnget    "PlayNorm"
 gkRecord      chnget    "Record"
 gSfilepath    chnget    "filename"
 kNewFileTrg   changed   gSfilepath        ; if a new file is loaded generate a trigger
 gkLevel       chnget    "Level"
 gkSemitones   chnget    "Semitones"
 gkCents       chnget    "Cents"
 gkTranspose   =         gkSemitones + gkCents*0.01
 kporttime     linseg    0,0.01,0.05
 gkTranspose   portk     semitone(gkTranspose),kporttime
 gklock        chnget    "lock"            ; Phase locking in mincer opcode
 gkStrFr       chnget    "StrFr"
 gkAttack      chnget    "beg"
 gkLen         chnget    "len"
 gkRise        chnget    "Rise" 
 gkHarmony     chnget    "Harmony"
   
 if trigger(gkAttack,1,0)==1 then         ; Hide instruction once stretch area has been selected
               chnset    "visible(0)", "InstructionID" 
 endif
 
 if kNewFileTrg==1 then                   ; if a new file has been loaded...
               event     "i",99,0,0.01    ; call instrument to update sample storage function table 
 endif   
 
 ktrig         trigger    gkPlayStr,0.5,0         ; Generate an impulse if PLAY STRETCHED button is clicked
               schedkwhen ktrig,0,0,2,0,0.01,60

 ktrig         trigger    gkPlayNorm,0.5,0        ; Generate an impulse if PLAY NORM. button is clicked
               schedkwhen ktrig,0,0,3,0,0.01

 ktrig         trigger    gkRecord,0.5,0          ; Generate an impulse if RECORD button is clicked
               schedkwhen ktrig,0,0,4,0,0.1
 
endin

instr    99    ; load sound file
 
 /* write file selection to function tables */
 gichans       filenchnls gSfilepath            ; derive the number of channels (mono=1,stereo=2) in the sound file
 gitableL      ftgen      1,0,0,1,gSfilepath,0,0,1
 if gichans==2 then
  gitableR     ftgen      2,0,0,1,gSfilepath,0,0,2
 endif
 giReady       =          1                    ; if no string has yet been loaded giReady will be zero
 Smessage      sprintfk   "file(%s)", gSfilepath            ; print sound file image to fileplayer
               chnset     Smessage, "filer1"

 /* write file name to GUI */
 Sname FileNameFromPath   gSfilepath                    ; Call UDO to extract file name from the full path
 Smessage      sprintfk   "text(%s)",Sname              ; create string to update text() identifier for label widget
               chnset     Smessage, "stringbox"         ; send string to  widget

 if i(gkAttack)==0 then                                 ; Reveal instruction when a file is opened for the first time
               chnset     "visible(1)", "InstructionID"
 endif    

endin

instr    2    ; play stretched
 
 if p3==-1 then ; MIDI
  iNum        notnum
  iTuning     chnget    "Tuning"
  iCPS        cpstmid   giTTable1 + iTuning - 1    
  iMlt        =         iCPS/cpsmidinn(60)
 else
  iMlt        =         1
 endif
 
 if giReady = 1 then                ; i.e. if a file has been loaded
  kStop       chnget    "Stop"
  ktrig       trigger   kStop,0.5,0        ; Generate an impulse if 'STOP' is clicked
  if ktrig==1 then
   chnset k(0),"PlayStr"
   turnoff
  endif

  iFileLen    =    ftlen(gitableL)/sr                                            ; File length in seconds
  iAttPnt    =    (i(gkAttack) == 0 ? 0.01 : (i(gkAttack)/sr))                   ; If stretch points haven't been set, replace with defaults. 
  iRelPnt    =    (i(gkAttack) == 0 ? 0.01 : ((i(gkAttack) + i(gkLen))/sr))
  ;iAttPnt     =    i(gkStrt)*iFileLen
  ;iRelPnt     =    i(gkEnd)*iFileLen
  
  iDuration   chnget    "Duration"
    
  iFFTSize    chnget    "FFTSize"
  iFFTSize    =         giFFTSizes[iFFTSize-1]
  
  ; File reading pointer main shape
  aptr        linseg    0,iAttPnt,iAttPnt, iDuration-iAttPnt-(iFileLen-iRelPnt), iRelPnt, iFileLen-iRelPnt, iFileLen
  p3          =         -1
  if k(aptr)>=iFileLen then
   turnoff
  endif
  
 ; print scrubber 
 Smsg         sprintfk  "pos(%d,5)", ((k(aptr)/iFileLen) * 690) + 5
              chnset    Smsg, "Scrubber"  

  ; File reading pointer jitter
  kJitDep     chnget    "JitDep"
  kJitRte     chnget    "JitRte"
  irise       =         0.01
  kJitEnv     linseg    0,iAttPnt,0, irise, .1, p3-iAttPnt-(iFileLen-iRelPnt) - (2*irise), .1, irise, 0 
  ajit        jspline   kJitDep*kJitEnv,kJitRte*0.5,kJitRte*2

  if gichans=1 then                        ; Mono
   a1         mincer    aptr+ajit, 1, gkTranspose*iMlt, gitableL, gklock, iFFTSize
   a1h        mincer    aptr+ajit, 1, 1*iMlt, gitableL, gklock, iFFTSize
   if gkHarmony==1 then
    a1        +=        a1h
   endif
   if i(gkRise)>0 then
    aEnv      cossegr   0, i(gkRise), 1, i(gkRise)*2, 0
    aCF       cossegr   4, i(gkRise), 14, i(gkRise)*2, 4
    a1        tone      a1, cpsoct(aCF)
    a1        *=        aEnv
   endif
              outs      a1*gkLevel,a1*gkLevel

  elseif gichans=2 then                        ; Stereo
   a1         mincer    aptr+ajit, 1, gkTranspose*iMlt, gitableL, gklock, iFFTSize
   a1h        mincer    aptr+ajit, 1, 1*iMlt, gitableL, gklock, iFFTSize
   if gkHarmony==1 then
    a1        +=        a1h
   endif
   a2         mincer    aptr+ajit, 1, gkTranspose*iMlt, gitableR, gklock, iFFTSize
   a2h        mincer    aptr+ajit, 1, 1*iMlt, gitableR, gklock, iFFTSize
   if gkHarmony==1 then
    a2        +=        a2h
   endif
   if i(gkRise)>0 then
    aEnv      cossegr   0, i(gkRise), 1, i(gkRise)*2, 0
    aCF       cossegr   4, i(gkRise), 14, i(gkRise)*2, 4
    a1        tone      a1, cpsoct(aCF)
    a1        *=        aEnv
    a2        tone      a2, cpsoct(aCF)
    a2        *=        aEnv
   endif
              outs      a1*gkLevel,a2*gkLevel
  endif
 endif

 if trigger:k(gkPlayStr,0.5,1)==1 then
              turnoff
 endif
 if release:k()==1 then
              chnset    k(0),"PlayStr"
 endif
endin

instr    3    ; normal speed playback
 iMlt  =    ftsr(gitableL)/sr                      ; scale playback speed for mismatching Cabbage sr and file sample rate
 p3    =    ftlen(gitableL)/(sr*i(gkTranspose)*iMlt)
 aptr    line    0,p3,1

 ; print scrubber 
 Smsg  sprintfk  "pos(%d,5)", (k(aptr) * 690) + 5
 chnset Smsg, "Scrubber"  
    
 aL    table3    aptr,gitableL,1
 if gichans==1 then
     outs    aL*gkLevel,aL*gkLevel
 else
 aR    table3    aptr,gitableR,1
    outs    aL*gkLevel,aR*gkLevel 
 endif 
 kStop        chnget    "Stop"
 ktrig        trigger    kStop,0.5,0        ; Generate an impulse if 'STOP' is clicked
 if ktrig==1 then
  chnset k(0),"PlayNorm"
  turnoff
 endif

 if trigger:k(gkPlayNorm,0.5,1)==1 then
  turnoff
 endif
  
 if release:k()==1 then
  chnset k(0),"PlayNorm"
 endif

endin


instr    4    ; record/render
 if giReady = 1 then                ; i.e. if a file has been loaded

  iFileLen    =    ftlen(gitableL)/sr                        ; File length in seconds
  iAttPnt    =    (i(gkAttack) == 0 ? 0.01 : (i(gkAttack)/sr))            ; If stretch points haven't been set, replace with defaults. 
  iRelPnt    =    (i(gkAttack) == 0 ? 0.01 : ((i(gkAttack) + i(gkLen))/sr))
;  iAttPnt     =    i(gkStrt)*iFileLen
;  iRelPnt     =    i(gkEnd)*iFileLen
  
  iDuration    chnget    "Duration"
  
  p3        =    iDuration
             
  ilen         strlen    gSfilepath                 ; Derive string length.
  SOutputName  strsub    gSfilepath,0,ilen-4        ; Remove ".wav"
  SOutputName  strcat    SOutputName,"_stretched"   ; Add suffix
  iDate        date
  SDate        sprintf    "%i",iDate
  SOutputName  strcat    SOutputName,SDate          ; Add date
  SOutputName  strcat    SOutputName,".wav"         ; Add extension
  
  
  iFFTSize    chnget    "FFTSize"
  iFFTSize    =    giFFTSizes[iFFTSize-1]
  
  iNKCycles    =    (iDuration * sr) / ksmps
  kCount    init    0
  
  loop:
  
  ; File reading pointer main shape
  aptr        linseg    0,iAttPnt,iAttPnt, p3-iAttPnt-(iFileLen-iRelPnt), iRelPnt, iFileLen-iRelPnt, iFileLen

  ; File reading pointer jitter
  kJitDep     chnget    "JitDep"
  kJitRte     chnget    "JitRte"
  irise       =         0.01
  kJitEnv     linseg    0,iAttPnt,0, irise, .1, p3-iAttPnt-(iFileLen-iRelPnt) - (2*irise), .1, irise, 0 
  ajit        jspline   kJitDep*kJitEnv,kJitRte*0.5,kJitRte*2

  if gichans=1 then                        ; Mono
   a1         mincer    aptr+ajit, 1, gkTranspose, gitableL, gklock, iFFTSize
   a1h        mincer    aptr+ajit, 1, 1, gitableL, gklock, iFFTSize
   if gkHarmony==1 then
    a1    +=            a1h
   endif
   if i(gkRise)>0 then
    aEnv      cossegr   0, i(gkRise), 1, i(gkRise)*2, 0
    aCF       cossegr   4, i(gkRise), 14, i(gkRise)*2, 4
    a1        tone      a1, cpsoct(aCF)
    a1        *=        aEnv
   endif
      fout    SOutputName,8,a1

  elseif gichans=2 then                        ; Stereo
   a1         mincer    aptr+ajit, 1, gkTranspose, gitableL, gklock, iFFTSize
   a1h        mincer    aptr+ajit, 1, 1, gitableL, gklock, iFFTSize
   if gkHarmony==1 then
    a1    +=            a1h
   endif
   a2         mincer    aptr+ajit, 1, gkTranspose, gitableR, gklock, iFFTSize
   a2h        mincer    aptr+ajit, 1, 1, gitableR, gklock, iFFTSize
   if gkHarmony==1 then
    a2        +=        a2h
   endif
   ;kptr    linseg    0,iAttPnt,iAttPnt, p3-iAttPnt-(iFileLen-iRelPnt), iRelPnt, iFileLen-iRelPnt, iFileLen    ; experimental
   ;a1    pvoc        kptr, gkTranspose, $ANALYSISFILE_L                            ; experimental
   ;a2    pvoc        kptr, gkTranspose, $ANALYSISFILE_R                            ; experimental
   if i(gkRise)>0 then
    aEnv      cossegr   0, i(gkRise), 1, i(gkRise)*2, 0
    aCF       cossegr   4, i(gkRise), 14, i(gkRise)*2, 4
    a1        tone      a1, cpsoct(aCF)
    a1        *=        aEnv
    a2        tone      a2, cpsoct(aCF)
    a2        *=        aEnv
   endif
              fout      SOutputName,8,a1,a2
  endif

              loop_lt   kCount, 1, iNKCycles, loop
              printks   SOutputName,0
              printks   "\nDone!\n",0
              turnoff
    
 endif
endin

</CsInstruments>  

<CsScore>
i 1 0 10000
</CsScore>

</CsoundSynthesizer>
