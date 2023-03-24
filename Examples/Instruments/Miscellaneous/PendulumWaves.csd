
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; Pendulum Waves

; Emulation of popular harmonic pendulum waves

; Note events are triggered by the 12 pendulum bobs as they cross the vertical line.

; Most controls arre adjusted using number boxes. 
; You can adjust these by either clicking and dragging on them 
;   or by clicking then typing a new number on them, the latter is probably easier.

; Time         - an indicator of elapsed time (for this to represent seconds, Speed must be set to '1')
; Play/Pause   - start and stop the pendulums
; Hold Pattern - this will cause all pendulums to adopt the frequency of the 12 pendulum, thereby holding the rhythmic pattern
; Number       - number of pendulums (1 to 12). Lower pendulums are retained
; Init.Angle   - starting position of the pendulums
; Trig.Angle   - the angle of the line over which pendulum bobs will triggee note events
; Damping      - damping of the pendulum excursions (air resistance/friction in the string). This doesn't affect the rhythms produced.
; Restart      - start the sequence again
; Size scale   - scale the length of the pendulums (visual only)

; Pendulum Frequencies [per minute]
; Swings per minute for each pendulum. Pendulums are numbered from bottom to top.

; OPEN FILE    - load a sound file to be triggered instead of the default synthesised sound
; Ping/Sample  - choose between a synthetic sound to be triggered and a chosen sound sample.
;                 for a sound sample it is recomended to choose something short and percussive.

; Sound File Settings
;  Set the transpositions and amplitudes for each pendulum when 'Sample' is chosen.
; The 12 number boxes set a note value relative to note 60. 
; I.e. with a setting of 60, the sample will be played back untransposed.
; The green table allows setting the amplitude and timbre of each pendulum, as in velocity.

<Cabbage>
form size(300,545), guiRefresh(16), caption("Pendulum Waves"), colour("white"), pluginId("PeWa")
image bounds(150,0,1,165), colour(200,200,200), shape("sharp"), rotate(0,1,1), identChannel("TrigAngleID")
image bounds(0,0,0,0), widgetArray("bob",12), shape("ellipse"), colour("black")
nslider bounds(5,170,60,30), text("Time"), channel("time"), range(-100000,10000,0,1,0.01), colour("white"), textColour("black"), fontColour("black")
checkbox bounds( 85,175,90,12), text("Play/Pause"), channel("PlayPause"), fontColour:0("black"), fontColour:1("black"), value(0)
checkbox bounds( 85,190,90,12), text("Hold Pattern"), channel("sync"), fontColour:0("black"), fontColour:1("black")
nslider bounds(175,170,60,30), text("Number"), channel("Number"), range(1,12,12,1,1), colour("white"), textColour("black"), fontColour("black")
nslider bounds(235,170,60,30), text("Speed"), channel("speed"), range(-10,10,0.2,1,0.01), colour("white"), textColour("black"), fontColour("black")

nslider bounds(5,205,55,30), text("Init.Angle"), channel("InitAngle"), range(-90,90,-75,1,1), colour("white"), textColour("black"), fontColour("black")
nslider bounds(65,205,55,30), text("Trig.Angle"), channel("TrigAngle"), range(-90,90,0,1,1), colour("white"), textColour("black"), fontColour("black")
nslider bounds(125,205,55,30), text("Damping"), channel("Damping"), range(0,0.9,0,1,0.0001), colour("white"), textColour("black"), fontColour("black")
button    bounds(185,210,50,20), text("Restart","Restart"), channel("Restart")
nslider bounds(240,205,55,30), text("Size Scale"), channel("MToPix"), range(10,5000,400,1,1), colour("white"), textColour("black"), fontColour("black")

image     bounds(  2,240,296,88), colour(200,200,200), outlineColour("black"), outlineThickness(2), shape("rounded")
label     bounds(  2,242,296,12), text("Pendulum Frequencies (per minute)"), fontColour("black")
nslider bounds( 15,255,40,30), text("1"),  channel("F1"),  range(10,500,49,1,0.1), colour("white"), textColour("black"), fontColour("black")
nslider bounds( 60,255,40,30), text("2"),  channel("F2"),  range(10,500,50,1,0.1), colour("white"), textColour("black"), fontColour("black")
nslider bounds(105,255,40,30), text("3"),  channel("F3"),  range(10,500,51,1,0.1), colour("white"), textColour("black"), fontColour("black")
nslider bounds(150,255,40,30), text("4"),  channel("F4"),  range(10,500,52,1,0.1), colour("white"), textColour("black"), fontColour("black")
nslider bounds(195,255,40,30), text("5"),  channel("F5"),  range(10,500,53,1,0.1), colour("white"), textColour("black"), fontColour("black")
nslider bounds(240,255,40,30), text("6"),  channel("F6"),  range(10,500,54,1,0.1), colour("white"), textColour("black"), fontColour("black")
nslider bounds( 15,290,40,30), text("7"),  channel("F7"),  range(10,500,55,1,0.1), colour("white"), textColour("black"), fontColour("black")
nslider bounds( 60,290,40,30), text("8"),  channel("F8"),  range(10,500,56,1,0.1), colour("white"), textColour("black"), fontColour("black")
nslider bounds(105,290,40,30), text("9"),  channel("F9"),  range(10,500,57,1,0.1), colour("white"), textColour("black"), fontColour("black")
nslider bounds(150,290,40,30), text("10"), channel("F10"), range(10,500,58,1,0.1), colour("white"), textColour("black"), fontColour("black")
nslider bounds(195,290,40,30), text("11"), channel("F11"), range(10,500,59,1,0.1), colour("white"), textColour("black"), fontColour("black")
nslider bounds(240,290,40,30), text("12"), channel("F12"), range(10,500,60,1,0.1), colour("white"), textColour("black"), fontColour("black")

filebutton bounds(  7,335, 83, 20), text("OPEN FILE","OPEN FILE"), fontColour(255,255,100) channel("filename"), shape("ellipse")
combobox   bounds(110,335, 83, 20), channel("Sound"), items("Ping", "Sample"), value(1), fontColour("white")
button     bounds(210,335, 83, 20), text("REC OUT","REC OUT"), channel("RecOut"), value(0), latched(1), fontColour:0(150,50,50), fontColour:1(255,205,205), colour:0(10,0,0), colour:1(150,0,0)

image     bounds(  2,360,296,180), colour(200,200,200), outlineColour("black"), outlineThickness(2), shape("rounded")
{
label     bounds(  2,  2,296,12), text("Transpositions/Amplitudes"), fontColour("black")
nslider bounds( 15, 15,40,30), text("Note 1"),  channel("N1"),  range(0,99,49,1,0.1), colour("white"), textColour("black"), fontColour("black")
nslider bounds( 60, 15,40,30), text("Note 2"),  channel("N2"),  range(0,99,50,1,0.1), colour("white"), textColour("black"), fontColour("black")
nslider bounds(105, 15,40,30), text("Note 3"),  channel("N3"),  range(0,99,51,1,0.1), colour("white"), textColour("black"), fontColour("black")
nslider bounds(150, 15,40,30), text("Note 4"),  channel("N4"),  range(0,99,52,1,0.1), colour("white"), textColour("black"), fontColour("black")
nslider bounds(195, 15,40,30), text("Note 5"),  channel("N5"),  range(0,99,53,1,0.1), colour("white"), textColour("black"), fontColour("black")
nslider bounds(240, 15,40,30), text("Note 6"),  channel("N6"),  range(0,99,54,1,0.1), colour("white"), textColour("black"), fontColour("black")
nslider bounds( 15, 50,40,30), text("Note 7"),  channel("N7"),  range(0,99,55,1,0.1), colour("white"), textColour("black"), fontColour("black")
nslider bounds( 60, 50,40,30), text("Note 8"),  channel("N8"),  range(0,99,56,1,0.1), colour("white"), textColour("black"), fontColour("black")
nslider bounds(105, 50,40,30), text("Note 9"),  channel("N9"),  range(0,99,57,1,0.1), colour("white"), textColour("black"), fontColour("black")
nslider bounds(150, 50,40,30), text("Note 10"), channel("N10"), range(0,99,58,1,0.1), colour("white"), textColour("black"), fontColour("black")
nslider bounds(195, 50,40,30), text("Note 11"), channel("N11"), range(0,99,59,1,0.1), colour("white"), textColour("black"), fontColour("black")
nslider bounds(240, 50,40,30), text("Note 12"), channel("N12"), range(0,99,60,1,0.1), colour("white"), textColour("black"), fontColour("black")
gentable bounds( 25,100,240, 60), identChannel("table1"),  tableNumber(3), tableColour("yellow"), tableBackgroundColour("silver") ampRange(0,1,3,0.1), zoom(-1), tableGridColour(0,0,0,0), active(1)
label    bounds( 25,160, 20, 12), text("1"), fontColour("black")
label    bounds( 45,160, 20, 12), text("2"), fontColour("black")
label    bounds( 65,160, 20, 12), text("3"), fontColour("black")
label    bounds( 85,160, 20, 12), text("4"), fontColour("black")
label    bounds(105,160, 20, 12), text("5"), fontColour("black")
label    bounds(125,160, 20, 12), text("6"), fontColour("black")
label    bounds(145,160, 20, 12), text("7"), fontColour("black")
label    bounds(165,160, 20, 12), text("8"), fontColour("black")
label    bounds(185,160, 20, 12), text("9"), fontColour("black")
label    bounds(205,160, 20, 12), text("10"), fontColour("black")
label    bounds(225,160, 20, 12), text("11"), fontColour("black")
label    bounds(245,160, 20, 12), text("12"), fontColour("black")

}


</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-n -dm0
</CsOptions>

<CsInstruments>

ksmps   =   16
nchnls  =   2
0dbfs   =   1

gichans       init    0        ; 
giReady       init    0        ; flag to indicate function table readiness
gSfilepath    init    ""

i_ ftgen 1,0,16,2,0
i_ ftgen 2,0,16,2,0

giAmps  ftgen  3, 0, 12, -2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1

instr    99    ; load sound file 
 /* write file selection to function tables */
 gichans    filenchnls    gSfilepath            ; derive the number of channels (mono=1,stereo=2) in the sound file
 gitableL    ftgen    1,0,0,1,gSfilepath,0,0,1
 if gichans==2 then
  gitableR    ftgen    2,0,0,1,gSfilepath,0,0,2
 endif
 giReady     =    1                             ; if no string has yet been loaded giReady will be zero
endin


instr   1 ; start pendulums
; LOAD FILE 
 gSfilepath    chnget    "filename"
 kNewFileTrg   changed   gSfilepath        ; if a new file is loaded generate a trigger
 if kNewFileTrg==1 then                ; if a new file has been loaded...
               event     "i",99,0,0.01            ; call instrument to update sample storage function table 
 endif   
 gkSound      chnget    "Sound"       ; sound source

;Record
kRecOut chnget "RecOut"
if changed:k(kRecOut)==1 then
 if kRecOut==1 then
  event "i", 1000, 0, -1
 else
  turnoff2 1000, 0, 1
 endif
endif

 gkRefresh    metro   32          ; refresh rate
 gkFreqs[]    init    12          ; array of pendulum frequency values
 gkRestart    chnget  "Restart"       ; manual restart trigger
 gkMToPix     chnget  "MToPix"        ; scaling sizes to pixels
 gkNumber     chnget  "gkNumber"      ; scaling sizes to pixels

 gkFreqs[0]   =       chnget:k("F1")
 gkFreqs[1]   =       chnget:k("F2")
 gkFreqs[2]   =       chnget:k("F3")
 gkFreqs[3]   =       chnget:k("F4")
 gkFreqs[4]   =       chnget:k("F5")
 gkFreqs[5]   =       chnget:k("F6")
 gkFreqs[6]   =       chnget:k("F7")
 gkFreqs[7]   =       chnget:k("F8")
 gkFreqs[8]   =       chnget:k("F9")
 gkFreqs[9]   =       chnget:k("F10")
 gkFreqs[10]  =       chnget:k("F11")
 gkFreqs[11]  =       chnget:k("F12")

 gkL12        =       ((60/(2*$M_PI*gkFreqs[11]))^2)*9.81 ; length of the last pendulum
 iCount       =       0
 loop:
              event_i "i",2+(iCount/100),0,3600,iCount
              loop_lt iCount,1,12,loop

 kNumber    chnget   "Number"
 if metro:k(5)==1 then
  if changed:k(kNumber)==1 then
   kCount = 1
   while kCount<=12 do
    kVisible = kNumber>=kCount?1:0
     Smsg    sprintfk "visible(%d)", kVisible
     Sident  sprintfk "bob_ident%d", kCount
             chnset   Smsg,Sident
   kCount += 1
   od
  endif
 endif

endin
           
instr   2   ; a pendulum
 if changed(gkRestart)==1 then          ; if a restart trigger has been received
  reinit RESTART                ; reinitialise from label
 endif
 RESTART:
 ix_OS        =   150                 ; x position offset 
 iRad         =   7                   ; radius of bob (visual only)
 iInitAngle   chnget    "InitAngle"   ; initial angle of from which swing begins
 iInitAngle   /=        -90           ; in radians
 kSpeed       chnget    "speed"       ; speed 1=normal, <1=slow_motion etc.
 kPlayPause   chnget    "PlayPause"
 kSpeed       *=        kPlayPause
 ksync        chnget    "sync"        ; if '1' all pendulums adopt the same frequency, i.e. pattern is held and repeated
 kTrigAngle   chnget    "TrigAngle"   ; angle at which sounds are triggered
 kDamping     chnget    "Damping"     ; amount of amplitude damping the pendulums experience
 if changed(kTrigAngle)==1 then       ; if sound trigger angle is changed...
  Smsg        sprintfk  "rotate(%f,1,1)",-kTrigAngle/90 *1.57   ; create a new string for the Cabbage line widget
              chnset    Smsg, "TrigAngleID" ; send string to the widget to update its appearance
 endif
 kFreq        =         gkFreqs[p4]   ; read frequency value for this pendulum from array
 kL           =         ((60/(2*$M_PI*kFreq))^2)*9.81           ; calculate length of pendulum  
 if ksync==0 then               ; is sync button is off (follow normal behaviour)
  kPeriod     =         2 * $M_PI * sqrt(kL/9.81) * (1/kSpeed)  ; period of a pendulum swing - dependent upon pendulum length but not mass
 else
  kPeriod     =         2 * $M_PI * sqrt(gkL12/9.81) * (1/kSpeed)   ; sync is on: all pendulums use the same length value
 endif
 kamp         init      iInitAngle    ; amplitude initialised according to initial angle of swing set by widget   
 kamp         =         kamp / (1+(kDamping/kr))    ; amplit; update elapsed time widget 8 times per secondude of swing, decreased by damping coefficient upon each k-cycle
 kphs         poscil    kamp,1/kPeriod,-1,0.75      ; pendulum phase
 ktime        init      0             ; initialise elapsed time counter
 if metro:k(8)==1 then              
              chnset    ktime,"time"  ; send new time value to nslider widget
 endif
 if ksync==0 then                     ; if sync is off, (i.e. in sync-on mode, freeze elapsed time counter)
  ktime       +=        kSpeed/kr     ; increment time (account for speed modulation)
 endif
 Sident       sprintf   "bob_ident%d",p4+1    ; generate a string for the ident name for this particular pendulum bob
 if gkRefresh==1 then                 ; if a (graphic) refresh trigger is received...
  kx_pix    =           sin($M_PI*kphs*0.5)*kL*gkMToPix ; use trig to calculate x position for bob
  ky_pix    =           cos($M_PI*kphs*0.5)*kL*gkMToPix ; use trig to calculate y position for bob
  Smsg  sprintfk    "bounds(%d,%d,%d,%d)", kx_pix + ix_OS - (iRad*0.5), ky_pix -(iRad*0.5), iRad, iRad  ; generate message for new position of widget
            chnset      Smsg, Sident          ; send new position message to widget
 endif
 if trigger(kphs,kTrigAngle/90,2)==1 && timeinstk()!=1 then ; if pendulum is crossing the sound trigger line
  icps      =           cpsmidinn(20+((p4+1)*6.717))            ; derive a sound frequency value from the length of the pendulum
            event       "i",3+gkSound-1,0,3,p4                   ; trigger a sound event
 endif
endin

giwave  ftgen 0,0,4094,10,0.1,0,1,0,0,0.05

instr   3 ; a synthesised sound ping
 iCount     =        p4 + 1
 iNumber    chnget   "Number"
 if iCount>iNumber then
  turnoff
 endif
 
 iAmp       table   iCount-1, 3
 Schn       sprintf "N%d",iCount
 iNote      chnget  Schn

 aenv       expseg   0.001,0.005,1,p3-0.005,0.001
 asig       poscil  iAmp*aenv,cpsmidinn(iNote),giwave
 ;asig       *=      poscil:a(1,300)
            outs    asig*0.25,asig*0.25
endin

instr   4 ; a sound sample
 iCount     =        p4 + 1
 iNumber    chnget   "Number"
 if iCount>iNumber then
  turnoff
 endif
 iAmp       table   iCount-1, 3
 Schn       sprintf "N%d",iCount
 iNote      chnget  Schn
 iMlt       =       cpsmidinn(iNote)/cpsmidinn(60)
 p3         =       ftlen(1)/(sr*iMlt)
 aPtr       line    0, p3, 1 
 aL         table3  aPtr, 1, 1 
 if gichans==2 then
  aR         tablei  aPtr, 2, 1 
 else
  aR        =     aL
 endif
 iCF        =        iAmp*10 + 4
 aL         tone  aL,cpsoct(iCF)
 aR         tone  aR,cpsoct(iCF)
 
            outs    aL*iAmp*0.25,aR*iAmp*0.25
endin



; record
instr 1000
  a1, a2         monitor

  ilen           strlen    gSfilepath                     ; Derive string length.
  SOutputName    strsub    gSfilepath,0,ilen-4            ; Remove ".wav"
  SOutputName    strcat    SOutputName,"_PendulumWaves" ; Add suffix
  iDate          date
  SDate          sprintf    "%i",iDate
  SOutputName    strcat    SOutputName,SDate              ; Add date
  SOutputName    strcat    SOutputName,".wav"             ; Add extension
                 fout      SOutputName, 8, a1, a2

endin



</CsInstruments>

<CsScore>
i 1    0 3600
</CsScore>

</CsoundSynthesizer>