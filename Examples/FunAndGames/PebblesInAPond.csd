
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; PebblesInAPond .csd
; Written by Iain McCurdy, 2014

; This instrument is a demonstration of Cabbage's ability to modify its widgets' appearance in realtime from within the Csound orchestra. 
; Five circles are drawn at start-up but are initially invisible.
; Upon clicking within the panel this will trigger a circle to become visble and to expand from the point where clicking occured until it again becomes invisible.
; In synchrony with this, a 'plip' sound is produced to continue to imitation of dropping pebbles into a pond.
; Various aspects of the 'plip' sound are dependent upon where within the panel the click that triggered it occured.
; Colours for the circles are simply chosen randomly.
; The reason 5 circles created, and triggering cycles through these, is to allow circles to overlap. It is assumed that the user won't trigger more that five at once.

<Cabbage>
form caption("Pebbles in a Pond"), size(900, 520), pluginId("Pond"),colour(0,10,20), guiRefresh(64)
image bounds(0,0,0,0), colour(0,0,0), widgetArray("circle",70), shape("ellipse"), outlineColour("white"), line(0)
label    bounds(  0, 0,0, 0), text("Drop a pebble into the pond..."), align(centre), fontColour(0,0,0,0), identChannel("instructions")
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-dm0 -n
</CsOptions>

<CsInstruments>

; sr set by host
ksmps         =     32    ;NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE (higher values will cause quantisation noise in the mode filter)
nchnls         =     2    ;NUMBER OF CHANNELS (2=STEREO)
0dbfs        =    1

gisine    ftgen    0,0,131072,10,1
giPanelSizeX    =    900        ; Should correspond to the size of the Cabbage panel
giPanelSizeY    =    520        ; Should correspond to the size of the Cabbage panel
giCircleNdx    init    1        ; Starting value for circle index counter. This mechanism allows multiple circles to overlap.
gaSendL,gaSendR    init    0        ; Global audio send variables used to send audio to the reverb effect.
giSizeInit    =    5        ; Initial circle diameter (in pixels)
giSizeFinal    =    1000        ; Final circle diameter (in pixels)
giCurve        =    2


opcode    scale_i,i,iii                ; UDO for an i-rate version of the 'scale' opcode
 ival,imax,imin    xin
 ival    =    (ival * (imax-imin)) + imin
    xout    ival
endop

instr    1    ; Sense mouse activity and trigger note events for instr 2
 kMOUSE_X        chnget    "MOUSE_X"        ; Read in mouse X position (in pixels across the panel)
 kMOUSE_Y        chnget    "MOUSE_Y"        ; Read in mouse Y position (in pixels across the panel)
 kMOUSE_DOWN_LEFT    chnget    "MOUSE_DOWN_LEFT"    ; Read in mouse left click status
 
 kOffTrig    trigger    kMOUSE_DOWN_LEFT,0.5,0        ; Generate a trigger whenever mouse button is clicked (on the way down)
 schedkwhen    kOffTrig,0,0,2,0,4,kMOUSE_X,kMOUSE_Y    ; Trigger a note event when mouse is clicked. Send mouse position X/Y as p4 and p5.
endin



instr    2    ; Draw an expanding circle and create a 'plip' sound 
 ; Named variables for X and Y position (in pixels) derived from p4 and p5
 iHorPosInit    init    p4
 iVertPosInit    init    p5

 ; Normalised x/y position (range 0 - 1)
 iXNorm    limit    iHorPosInit/giPanelSizeX, 0.001,1
 iYNorm    limit    iVertPosInit/giPanelSizeY, 0.001,1
 
 ; Draw an expanding and fading circle 
 kSize        transeg    giSizeInit,p3-0.1,giCurve,giSizeFinal, 0.1, giCurve, 1, 1,giCurve,1        ; Circle diameter increasing as time progress
 kHorPos    transeg    iHorPosInit-(giSizeInit/2),p3,giCurve,iHorPosInit-(giSizeFinal/2)    ; Horizontal position needs to compensate for the expanding circle so that the circle center remains fixed.
 kVertPos    transeg    iVertPosInit-(giSizeInit/2),p3,giCurve,iVertPosInit-(giSizeFinal/2)    ; Vertical position needs to compensate for the expanding circle so that the circle center remains fixed.
 
 iRed        random    100,255                ; Colours are set randomly once per event
 iGreen        random    100,255                ; Colours are set randomly once per event
 iBlue        random    100,255                ; Colours are set randomly once per event
 kAlpha        transeg    255,0.4,0,100 ,p3-0.5,-6, 0,1,0,0    ; Colour alpha decreases as event progresses, i.e. transparency increases
 
 gkRefreshTrig    metro    ksmps                ; Create a metronome which will define the rate of graphical update of the expanding circles
 
 if gkRefreshTrig==1 then                                                            ; If a trigger has been generated...  
  SCircle    sprintf        "circle_ident%d",giCircleNdx
   Smessage     sprintfk     "bounds(%d,%d,%d,%d),colour(%d,%d,%d,%d)", kHorPos, kVertPos, kSize, kSize, iRed, iGreen, iBlue, kAlpha    ; Create a string with updated attributes that will be sent to the circle widget.
   chnset    Smessage,SCircle                                         
 endif 

 ; Ripples echoes (comment out event_i's to alleviate performance issues)
 idel    =    0.35    ; time gap between ripple echoes
 event_i "i",3,idel,   4-0.3  , p4, p5, iRed, iGreen, iBlue, giCircleNdx+10, idel  , 55
 event_i "i",3,idel*2, 4-0.3*2, p4, p5, iRed, iGreen, iBlue, giCircleNdx+20, idel*2, 44
 event_i "i",3,idel*3, 4-0.3*3, p4, p5, iRed, iGreen, iBlue, giCircleNdx+30, idel*3, 33
 event_i "i",3,idel*4, 4-0.3*4, p4, p5, iRed, iGreen, iBlue, giCircleNdx+40, idel*4, 22
 event_i "i",3,idel*5, 4-0.3*5, p4, p5, iRed, iGreen, iBlue, giCircleNdx+50, idel*5, 11
 event_i "i",3,idel*6, 4-0.3*6, p4, p5, iRed, iGreen, iBlue, giCircleNdx+60, idel*6,  1
 
 giCircleNdx    wrap    giCircleNdx+1,1,11    ; Increment circle index counter for next circle. Wrap around whenever last circle in the cycle is reached. This facilitates multiple circles to be drawn.

 ; Produce a sound
 idur        scale_i    iYNorm,2,0.2        ; 'Plip' duration varies according to mouse X position.
 iStartOct    scale_i    (iXNorm+(1-iYNorm))*0.5,11,5.5
 iStartCPS    =    cpsoct(iStartOct)
 iEndOct    scale_i    1-iYNorm,13.8,9.6
 iEndCPS    =    cpsoct(iEndOct)
 iCPSCurve    =    5            ; Shape of CPS envelope. Higher values will delay its curve upwards.
 kCPS    transeg    iStartCPS,idur,iCPSCurve,iEndCPS,1,0,iEndCPS    ; CPS curve used by the mode filter
 aEnv    transeg    1,idur,-6,0,1,0,0        ; Amplitude envelope. Used by both the impulse noise and the filtered output. 
 aNse    pinkish    aEnv*0.05            ; Some pink noise.
 aNse    buthp    aNse,250            ; Highpass filter the noise to remove some of the rumble.
 aImp    mpulse    1,0                ; A Click impulse. Used to augment the attack of the sound.
 aNse    =    aNse+aImp            ; Mix the pink noise and the click.
 kCPS    limit    kCPS, 20, sr/$M_PI
 aDrop    mode    aNse,kCPS,kCPS/3            ; Mode filter the noise and click mixture. Note that Q (p3) will increase as CPS increases - higher frequencies will ring for longer.
 aDrop    =    aDrop * aEnv            ; Apply envelope to mode filtered signal.
 aL,aR    pan2    aDrop,iXNorm            ; Create stereo panned version of 'plip'
 aL    delay    aL,iXNorm  *0.01        ; Some Haas effect delay 
 aR    delay    aR,(1-iXNorm)*0.01        ; Some Haas effect delay
     outs    aL,aR                ; Send dry 'plip' signal to speakers.
 gaSendL    =    gaSendL + aL*0.15    ; Send some to the reverb instrument
 gaSendR    =    gaSendR + aR*0.15    ; "    "    "  "   "      "
endin

instr    3 ; Echo ripple
 ; Named variables for X and Y position (in pixels) derived from p4 and p5
 iHorPosInit    init    p4
 iVertPosInit    init    p5
 iDel        init    p10
 
 ; Normalised x/y position (range 0 - 1)
 iXNorm    =    iHorPosInit/giPanelSizeX
 iYNorm    =    iVertPosInit/giPanelSizeY
 
 ; Draw an expanding and fading circle 
 kSize        transeg    giSizeInit,p3-0.1,giCurve,giSizeFinal, 0.1, giCurve, 1, 1,giCurve,1        ; Circle diameter increasing as time progress
 kHorPos    transeg    iHorPosInit-(giSizeInit/2),p3,giCurve,iHorPosInit-(giSizeFinal/2)        ; Horizontal position needs to compensate for the expanding circle so that the circle center remains fixed.
 kVertPos    transeg    iVertPosInit-(giSizeInit/2),p3,giCurve,iVertPosInit-(giSizeFinal/2)    ; Vertical position needs to compensate for the expanding circle so that the circle center remains fixed.
 
 iRed        =    p6
 iGreen        =    p7
 iBlue        =    p8
 iAlpha        =    p11
 kAlpha    transeg    iAlpha,0.4,0,iAlpha*(100/255) ,p3-0.5,-3, 0,1,0,0    ; Colour alpha decreases as event progresses, i.e. transparency increases
 
 iCircleNdx    =    p9

 if gkRefreshTrig==1 then                                                            ; If a trigger has been generated...  
  SCircle    sprintf        "circle_ident%d",iCircleNdx+1
   Smessage     sprintfk     "bounds(%d,%d,%d,%d),colour(%d,%d,%d,%d)", kHorPos, kVertPos, kSize, kSize, iRed, iGreen, iBlue, kAlpha    ; Create a string with updated attributes that will be sent to the circle widget.
   chnset    Smessage,SCircle                                         
 endif 


endin


instr    99    ; Delay-Reverb
 aL        delayr        0.7
 aR        delayr        0.973
        delayw        gaSendL*0.8 + aL*0.35 + aR*0.35
        delayw        gaSendR*0.8 + aR*0.35 + aL*0.35
 aL,aR    reverbsc    gaSendL+aL,gaSendR+aR,0.8,3000
         outs        aL, aR            ; Send delay signal to outputs
         clear        gaSendL, gaSendR    ; Clear global audio send variables
endin


instr    1000    ; Print initial text instructions, expanding and disappearing.
 iSizeInit    =    14            ; Initial size (in pixels)
 iSizeFinal    =    60            ; Final size (in pixels)
 kSize        linseg    iSizeInit,p3-0.001,iSizeFinal,0.001,0    ; Text Size increasing as time progresses
  
 kAlpha        transeg    0,0.1,4,255,p3-0.1,-4,0,1,0,0        ; Colour alpha decreases as event progresses, i.e. transparency increases
 
 ktrig    metro    ksmps                ; Create a metronome which will define the rate of graphical update of the text
 
 if ktrig==1 then                ; If a trigger has been generated...
  reinit UPDATE
  UPDATE:
  Smessage sprintfk "bounds(-200, 245,%d, %d),fontColour(255,255,255,%d)", giPanelSizeX+400, i(kSize), i(kAlpha)    ; Create a string with updated attributes that will be sent to the instructions widget.
  rireturn
  chnset Smessage, "instructions"                                                        ; Send updated attributes to the widget.
 endif

endin
    
</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]    ; Instrument to sense mouse activity
i 99 0 [3600*24*7]    ; Reverb instrument
i 1000 0 5        ; Instructions
</CsScore>


</CsoundSynthesizer>



























