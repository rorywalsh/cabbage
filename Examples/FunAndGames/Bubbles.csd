
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; Bubbles.csd
; Written by Iain McCurdy, 2014

; A simple game that is intended to demonstrate moving Cabbage widgets in this case 'image' and interacting with those moving widgets using the mouse.

; The user bursts rising bubbles by clicking on them (a 'burst' is rewraded with a pizzicato-type sound).
; The higher up the panel a bubble is burst, the higher the note it produces.
; Horizontal position across the panel corresponds to panning location of the sound produced.
; The size of the bubble corresponds to the loudness of the burst sound it produces: The larger the bubble, the louder the sound.
; The number of bubbles in circulation gradually increases from 1 to 10 
  
<Cabbage>
form caption("Bubbles"), scrollBars(0), size(1000, 540), pluginId("Bubl"),colour(170,170,255), guiRefresh(32)    ; guiRefresh might have some bearing on the smoothness of movement of the balloons (traded against CPU efficiency)
label bounds(  0,250,1000, 30), text("Burst the Bubbles..."), align(centre), fontColour(0,0,0,0), identChannel("instructions")  ; Instructions

image    bounds(  0,  0,  0, 0), colour(0,0,0), shape("ellipse"), widgetArray("bubble",10)                 ; The bubbles
image    bounds(  0,  0,  0, 0), colour(0,0,0), shape("ellipse"), widgetArray("specular",10)               ; Specular highlights
label    bounds(  5,  2, 45, 15), text("Burst:"), align(left), fontColour(0,0,0,100)                       ; Burst label
label    bounds( 50,  2, 40, 15), text("0"), align(left), fontColour(0,0,0,150), identChannel("burstID")   ; Burst value
label    bounds( 90,  2, 55, 15), text("Missed:"), align(left), fontColour(0,0,0,100)                      ; Missed label
label    bounds(145,  2, 40, 15), text("0"), align(left), fontColour(0,0,0,150), identChannel("missedID")  ; Missed value

label    bounds(  1,530,100, 10), text("Iain McCurdy 2014"), align(left), fontColour(0,0,0,150)
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-dm0 -n
</CsOptions>

<CsInstruments>

; sr set by host
ksmps  =  32    ; NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls =  2     ; NUMBER OF CHANNELS (2=STEREO)
0dbfs  =  1
seed 0

gaSendL,gaSendR init 0    ; Global audio send variables used to send audio to the reverb effect.
gisine          ftgen  0, 0, 131072, 10, 1
giImp           ftgen  0, 0, 4097, 9, 0.5, 1, 0   ; Shape for the impulse.
gipanel_width   =      1000                       ; Panel width (should correspond with the value given in the Cabbage GUI code)
gipanel_height  =      540                        ; Panel height (should correspond with the value given in the Cabbage GUI code)
giNBalloons     =      10                         ; Number of balloons.
giBurstScore    =      0                          ; Number of bubbles burst
giMissedScore   =      0                          ; Number of bubbles missed
gkactive        init   0
giPolyLimit     =      3
giWobbleShape   ftgen  0, 0, 4096, 10, 1, 0.3

;    yellow violet red green d.blue l.blue orange pink purple white black
giR            ftgen   0, 0, -11, -2, 255, 255, 255, 0, 0, 100, 255, 255, 100, 255, 20
giG            ftgen   0, 0, -11, -2, 255, 0, 0, 255, 0, 100, 100, 75, 0, 255, 20
giB            ftgen   0, 0, -11, -2, 255, 255, 0, 0, 255, 255, 9, 255, 100, 255, 20
giColourIndex  init    0

opcode Oscil1a,a,iii     ; an oscillator that plays a single cycle of an audio waveform at a-rate
 iamp,ifrq,ifn xin
 aptr          line    0, 1/ifrq, 1
 asig          tablei  aptr, ifn, 1
 aenv          linseg  1, 1/ifrq,1, 0.001, 0
               xout    asig * iamp * aenv
endop

opcode scale_i,i,iii     ; UDO for an i-rate version of the 'scale' opcode
 ival,imax,imin xin
 ival           =      (ival * (imax-imin)) + imin
                xout   ival
endop


instr 1 ; Always on
 ; Attributes change over time. I.e. game gets harder.
 gkSizeMin      linseg  60 ,30, 60 ,180, 30     ; Minimum possible size for a bubble
 gkSizeMax      linseg  250 ,30, 250 ,180, 150  ; Maximum possible size for a bubble
 gkMinTime      linseg  9 ,30, 9 ,180, 3        ; Minimum possible time it might take a bubble to rise to the top of the panel
 gkMaxTime      linseg  16 ,30, 16 ,180, 4      ; Maximum possible time it might take a bubble to rise to the top of the panel

 gkSizeMin      init    60
 gkSizeMax      init    250
 gkMinTime      init    9
 gkMaxTime      init    16

 gkMOUSE_X         chnget   "MOUSE_X"           ; Read in mouse X position (in pixels across the panel)
 gkMOUSE_Y         chnget   "MOUSE_Y"           ; Read in mouse Y position (in pixels across the panel)
 gkMOUSE_DOWN_LEFT chnget   "MOUSE_DOWN_LEFT"   ; Read in mouse left click status

 gkRefreshTrig     metro    ksmps/2             ; Rate of GUI refresh
 
 icount = 1           
 LOOP:
                event_i    "i", icount + 1, 20 * (icount - 1), rnd(i(gkMaxTime) - i(gkMinTime)) + i(gkMinTime)  ; Start balloons
                loop_le    icount,1,giNBalloons,LOOP
 
 gkbreeze       rspline    -0.02, 0.02, 0.2, 0.4 ; A bit of breeze pushing all bubbles off course simultaneously.
endin

#define BUBBLE(INSTR'N)
#
instr $INSTR
 isize_norm     =          rnd(1)^2                             ; Normalised size for this bubble (i.e. range 0 - 1).
 isize_x        scale_i    isize_norm,i(gkSizeMax),i(gkSizeMin) ; Actual size (in pixels).
 isize_y        =          isize_x                              ; Vertical size.
 ix             random     gipanel_width*0.1,gipanel_width*0.9  ; Starting horizontal position for this bubble.
 ky             line       625, p3, -isize_y-5                  ; Vertical position for this bubble.
 
 if trigger(ky,-isize_y,1)==1 then                              ; If bubble reaches the top...
                event      "i", 101, 0, 0                       ; increment and print to 'missed' counter
 endif 
               
 giColourIndex wrap giColourIndex + 1 + int(rnd(4)), 0, 11
 iR             table       giColourIndex, giR                  ; Colours randomised each time bubble is relaunched.
 iG             table       giColourIndex, giG
 iB             table       giColourIndex, giB
 iAlpha         random      150, 225
 kWobFreq       randomi     2, 6, 0.5, 1                        ; Size wobble
 ksize_x        oscil       0.02, kWobFreq, giWobbleShape, 0
 ksize_x        =           (1 + ksize_x) * isize_x
 ksize_y        oscil       0.02, kWobFreq, giWobbleShape, 0.5
 ksize_y        =           (1 + ksize_y) * isize_y
 
 krel  release    ; Sense note ending (either by bursting or by exiting the panel).
 if krel==1 then
                event       "i", 100, 0, 0.01, p1               ; Call instrument that will restart this instrument (after some delay).
 endif

 
 kx$N           init        ix
 iSizeRat$N     =           isize_x / (i(gkSizeMax)-i(gkSizeMin))       ; Size ratio (0 = smallest possible, 1 = largest possible)
 kx$N           limit       kx$N + (gkbreeze*(1+(iSizeRat$N*2))), -isize_x*1.1, gipanel_width + (isize_x*0.1) ; Blown by the breeze. Limit the bubbles from leaving the panel completely. Larger bubbles are affected more than small ones.
 
 if gkRefreshTrig==1 then                                               ; If a trigger has been generated...
  Sbubble       sprintfk    "bubble_ident%d",$N
  Smessage      sprintfk    "bounds(%d,%d,%d,%d),colour(%d,%d,%d,%d), outlineThickness(0)", kx$N, ky, ksize_x, ksize_y,iR,iG,iB,int(iAlpha) ; Create a string with updated attributes that will be sent to the widget.
                chnset      Smessage, Sbubble                           ; Send updated attributes to the bubble widget.

  Sspecular     sprintfk    "specular_ident%d",$N
  Smessage      sprintfk    "bounds(%d,%d,%d,%d),colour(255,255,255,%d)", kx$N+(ksize_x*0.6), ky+(ksize_y*0.22), ksize_x*0.2, ksize_y*0.2,40 ; Create a string with updated attributes that will be sent to the widget.
                chnset      Smessage, Sspecular                         ; Send updated attributes to the widget.

 endif
 
 ktrig trigger gkMOUSE_DOWN_LEFT,0.5,0
 if ktrig==1 then

  kx_centre     =           kx$N + (isize_x*0.5)           ; x centre of bubble
  ky_centre     =           ky + (isize_y*0.5)             ; y centre of bubble
  kx_dist       =           abs(gkMOUSE_X - kx_centre)     ; x distance from centre of this bubble of mouse click
  ky_dist       =           abs(gkMOUSE_Y - ky_centre)     ; y distance from centre of this bubble of mouse click
  kdist         =           (kx_dist^2 + ky_dist^2) ^ 0.5  ; apply pythagoras to discern distance of mouse click from the centre of this bubble
  
  if kdist<=(isize_x*0.5) then                                                                                                          ; if distance of mouse click is less than or equal to radius of the bubble...
                event       "i", 200, 0, 0.4, (gkMOUSE_X - kx$N) / isize_x, 1 - (ky / gipanel_height), isize_norm, kx$N / gipanel_width ; Create a 'burst' sound and send current mouse x and y locations and size (normalised)
                chnset      "outlineThickness(1), colour(0,0,0,0),outlineColour(240,240,240)", Sbubble                                  ; Send updated attributes to the widget, i.e. 'burst' bubble.
                chnset      "bounds(0,0,0,0)", Sspecular                                                                                ; Send updated attributes to the widget, i.e.render specular highligh invisiblee.
               ;chnset      "bounds(0,0,0,0)", Sbubble                                                                                  ; Send updated attributes to the widget, i.e. 'burst' bubble.
   turnoff
  endif
 endif 
endin
#

;Expand macro for the number of bubbles required
;        INSTR COUNTER
$BUBBLE(2'1)
$BUBBLE(3'2)
$BUBBLE(4'3)
$BUBBLE(5'4)
$BUBBLE(6'5)
$BUBBLE(7'6)
$BUBBLE(8'7)
$BUBBLE(9'8)
$BUBBLE(10'9)
$BUBBLE(11'10)


instr 100 ; Trigger the start of a new bubble
  p3            =           0.1
                event_i     "i",p4,p3,rnd(i(gkMaxTime)-i(gkMinTime))+i(gkMinTime)
endin

instr 101 ; Missed bubble
 ; Update score
 giMissedScore  +=          1
 S1             sprintf     "text(%d)",giMissedScore
                chnset      S1,"missedID"
endin

instr 200 ; Burst sound
 /* POLYPHONY CONTROL */
 gkactive init i(gkactive) + 1                      ; INCREMENT NOTE COUNTER
 if gkactive>giPolyLimit then                       ; IF POLYPHONY IS EXCEEDED (THROUGH THE ADDITION OF NEW NOTE)
                turnoff                             ; REMOVE THIS NOTE
 endif
 krel           release                             ; IF NOTE HELD = 0, IF NOTE RELEASED = 1
 ktrig          trigger     krel, 0.5, 0            ; WHEN RELEASE FLAG CROSSES 0.5 UPWARDS, I.E. NOTE HAS BEEN RELEASED... 
 if ktrig==1 then  
  gkactive      =           gkactive - 1            ;...DECREMENT ACTIVE NOTES COUNTER
 endif

 ; Update score
 giBurstScore   +=          1
 S1             sprintf     "text(%d)",giBurstScore
                chnset      S1, "burstID"


 iX_Burst_Loc =             p4                      ; Where within the width of the bubble it was burst. Normalised 0 - 1. (not currently used for anything).
 iY           =             p5                      ; Where within the height of the panel the bubble was burst.  Normalised 0 - 1.
 iSize        =             p6                      ; Bubble size (normalised 0 - 1).
 iX           =             p7                      ; Location across the panel this bubble was travelling in (normalised 0 - 1).
 
 icps = cpsmidinn(int(iY*72)+12)

 ; Impulse signal
 iAmpVel      =             (((iSize) * 0.5) + 0.5) ^ 3
 ifrq         limit         icps, 200, 600                ; Freq. of impulse
 aImpls       Oscil1a       iAmpVel * 2, ifrq, giImp      ; Single cycle half sine impulse (UDO)
 aImpls       buthp         aImpls, 300                   ; Remove some low freq. energy

 ; Waveguide impulse processer
 aWg1         wguide1       aImpls, icps, icps * 4, 0.999995                                  ; Pizz note
 aWg2         wguide1       aImpls, icps*cent(rnd(50)), limit(icps * 4, 20, 3000), 0.999995   ; Pizz note unison detuned
 aWg3         wguide1       aImpls, icps*2, limit(icps * 8, 20, 3000), 0.999995               ; Pizz note up an octave
 aWg4         wguide1       aImpls, icps*2*cent(rnd(50)), limit(icps * 8, 20, 3000), 0.999995 ; Pizz note up an octave detuned
 aWg2         delay         aWg2, rnd(0.02) + 0.0001                                          ; 2nd note delayed
 aWg3         delay         aWg3, rnd(0.05) + 0.0001                                          ; 3rd note delayed
 aWg4         delay         aWg4, rnd(0.05) + 0.0001                                          ; 4th note delayed
 aMix         sum           aWg1, aWg2, aWg3, aWg4                                            ; Mix all 4 notes
 aEnv         linsegr       1, p3, 0, 0.05, 0
 aMix         =             aMix * aEnv
 aL,aR        pan2          aMix, iX                                                          ; Create panned stereo signal, location based on x-location of balloon
              outs          aL, aR
 
 gaSendL      =             gaSendL + (aL*0.3)                                                ; Send some to the reverb instrument
 gaSendR      =             gaSendR + (aR*0.3)
endin





instr 1000 ; Reverb
 aL,aR        reverbsc      gaSendL, gaSendR, 0.9, 5000
              outs          aL, aR
              clear         gaSendL, gaSendR
endin





instr 2000 ; Print initial text instructions then fading away.
 kAlpha       transeg       0, 0.75, 4, 255, p3 - 1.5, 0, 255, 0.75, -4, -1                   ; Colour alpha decreases as event progresses, i.e. transparency increases

 ktrig        metro         ksmps * 0.5                                                       ; Create a metronome which will define the rate of graphical update of the text
 
 if ktrig==1  then                                                                            ; If a trigger has been generated...
   Smess1     sprintfk      "fontColour(0,0,50,%d)",kAlpha                                    ; Create a string with updated attributes that will be sent to the instructions widget.
              chnset        Smess1, "instructions"                                            ; Send updated attributes to the widget.
 endif

              event_i       "i", 1, p3 - 2, 3600                                              ; Start bubbles once instructions have vanished.
endin

</CsInstruments>

<CsScore>
i 1000 0 [3600*24*7] ; Reverb
i 2000 0 3           ; Instructions fade up then down (currently not working)
</CsScore>

</CsoundSynthesizer>
