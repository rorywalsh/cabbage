
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; Electricity.csd
; Written by Iain McCurdy, 2015

; click and drag to generate sparks

; This example makes use of Cabbage's ability to rotate widgets and to make them visible and invisible 
; to create the sparking effect.
; It also demonstrates how rate-of-change of mouse position (position delta) is used as a control input (spark density).
; The tonal variation of the sparks is implemented using 2 comb filters, 
;   the loop times of which are linked to the mouse x and y positions.

<Cabbage>
form  size(1000,540), caption("Electricity"), guiRefresh(32), colour(0,0,0), pluginId("Elec")
image bounds(0,0,0,0), shape("sharp"), visible(0), widgetArray("spark",10) ; widget array (10 image widgets with a single line of code)
label bounds(0,530,100,10), text("Iain McCurdy |2015|")
label bounds(  0,230,1000,70), text("Click and Drag..."), fontColour(255,255,255), identChannel("instructions")
</Cabbage>

<CsoundSynthesizer>

<CsOptions>   
-dm0 -n
</CsOptions>

<CsInstruments>

; sr set by host
ksmps = 32
nchnls = 2
0dbfs = 1
seed 0

giPanelWidth  = 1000  ; panel width in pixel (declared here to be used later in the code)
giPanelHeight = 540   ; panel height in pixel (declared here to be used later in the code)

; i-rate version of scale opcode
opcode  scale_i,i,iii
 ival,imax,imin  xin
 ival            =     (ival * (imax-imin)) + imin
                 xout  ival
endop

opcode TriggerToGatek,k,kki
 ktrig,kdur,imax  xin
 kdlytrig         vdelayk    ktrig,kdur,imax
 kgate            samphold   ktrig,ktrig + kdlytrig
                  xout       kgate
endop

instr 1
 gkMOUSE_X    chnget   "MOUSE_X"                  ; read in mouse x position in pixels
 gkMOUSE_Y    chnget   "MOUSE_Y"                  ; read in mouse y position in pixels
 kx_ratio     =        gkMOUSE_X/giPanelWidth     ; mouse x position as a ratio of the entire panel (0-1)
 ky_ratio     =        gkMOUSE_Y/giPanelHeight    ; mouse y position as a ratio of the entire panel (0-1)
 kMOUSE_DOWN_LEFT chnget  "MOUSE_DOWN_LEFT"       ; mouse left click status (0 or 1)
 if trigger(kMOUSE_DOWN_LEFT,0.5,0)==1 then       ; if mouse button is pressed...
  koldx       random   gkMOUSE_X-10,gkMOUSE_X+10  ; reset 'previous' x position value
  koldy       random   gkMOUSE_Y-10,gkMOUSE_Y+10  ; reset 'previous' y position value
 endif
 if kMOUSE_DOWN_LEFT==1 then                      ; if mouse left button is held...
  koldx,koldy   init   0                          ; previous mouse positions initialised
  kdeltax       =      abs(gkMOUSE_X-koldx)       ; mouse x position change (in pixels)
  kdeltay       =      abs(gkMOUSE_Y-koldy)       ; mouse y position change (in pixels)
  kdelta        =      kdeltax + kdeltay          ; sum of mouse position changes
  koldx         =      gkMOUSE_X                  ; set 'previous' mouse positions for next iteration
  koldy         =      gkMOUSE_Y
  kdens         init   300
  ktrig         metro  kdens * kdelta             ; generate a trigger
  if ktrig==1 then
   kdens        =           exprand:k(300) + 50
  endif
                schedkwhen  ktrig, 0, 0,2, 0, 0.01, kx_ratio, ky_ratio   ; spark sound
                schedkwhen  ktrig, 0, 0, 11, 0, 0, 1, kdelta             ; visual sparks...
                schedkwhen  ktrig, 0, 0, 11, rnd(0.01) * 3, 0, 2, kdelta
                schedkwhen  ktrig, 0, 0, 11, rnd(0.02) * 3, 0, 3, kdelta
  if kdelta<1 kgoto SKIP
                schedkwhen  ktrig, 0, 0, 11, rnd(0.03) * 3, 0, 4, kdelta
                schedkwhen  ktrig, 0, 0, 11, rnd(0.04) * 3, 0, 5, kdelta
  if kdelta<5 kgoto SKIP
                schedkwhen  ktrig, 0, 0, 11, rnd(0.05) * 3, 0, 6, kdelta
                schedkwhen  ktrig, 0, 0, 11, rnd(0.06) * 3, 0, 7, kdelta
  if kdelta<10 kgoto SKIP
                schedkwhen  ktrig, 0, 0, 11, rnd(0.07) * 3, 0, 8, kdelta
                schedkwhen  ktrig, 0, 0, 11, rnd(0.08) * 3, 0, 9, kdelta
                schedkwhen  ktrig, 0, 0, 11, rnd(0.09) * 3, 0, 10, kdelta
  SKIP:
 endif  
 
 ; 50 Hz mains hum
 aamp           interp      (1-(ktrig*0.5)) * (0.005 + (kMOUSE_DOWN_LEFT*0.03)) ; amplitude derives from mouse down position and inversely to trigger impulses
 kpw            rspline     0.89-(ky_ratio*0.88),0.99-(ky_ratio*0.98),0.2,0.4   ; slight pulse width shift
 kjit           rspline     -10,10,1,5                                          ; frequency jitter
 asq            vco2        1, 50*cent(kjit), 4, kpw, 0, 0.125                  ; square wave
 asq            *=          aamp                                                ; scale amplitude
 kpan           rspline     kx_ratio * 0.5, 0.5+(kx_ratio * 0.5), 0.5, 5        ; panning function
 aL1,aR1        pan2        asq, kpan                                           ; create stereo output
 ; a second hum signal
 kpw            rspline     0.89 - (ky_ratio * 0.88), 0.99 - (ky_ratio * 0.98), 0.2, 0.4 ; slight pulse width shift
 kjit           rspline     -10, 10, 1, 5                                       ; frequency jitter
 asq            vco2        1, 50*cent(kjit), 4, kpw, 0, 0.125                  ; square wave
 asq            *=  aamp                                                        ; scale amplitude
 kpan           rspline     kx_ratio * 0.5, 0.5 + (kx_ratio * 0.5), 0.5, 5      ; panning function 
 aL2,aR2        pan2        asq, kpan                                           ; create stereo output
 
                outs        aL1 + aL2, aR1 + aR2                                ; mix hum signals and send to output
 
endin

gasendL,gasendR init        0                                                   ; reverb send variables

instr 2
 iamp           exprand     1                                      ; random amplitude
 asig           mpulse      iamp, 0                                ; a click impulse
 asig           buthp       asig, 500                              ; highpass filter it
 icfoct         random      9, 14                                  ; random freq (oct format)
 asig           wguide1     asig, cpsoct(icfoct), sr / 4, 0.5      ; send click through a waveguide filter
 iDT1           scale_i     p4, 0.00001, 0.005                     ; delay time for a comb filter derived from mouse x position
 a1             comb        asig, 0.01, iDT1                       ; comb filter (1)
 iDT2           scale_i     p5, 0.00001, 0.005                     ; delay time for a comb filter derived from mouse y position
 a2             comb        asig, 0.005, iDT2                      ; comb filter (2)
 asig           sum         a1, a2                                 ; mix the two comb filter outputs
 aL,aR          pan2        asig, rnd(1)                           ; random pan click
                outs        aL, aR                                 ; send stereo audio to outputs
 gasendL        +=          aL / (2 + rnd(3))                      ; send some audio to the reverb send channels
 gasendR        +=          aR / (2 + rnd(3))                                        
endin

instr 11 ; show spark and move and set colour
 Sident         sprintf     "spark_ident%d",p4                     ; define indent channel string
 irad           random      0, 2 * $M_PI                           ; random rotation value (range: 0 to 2*pi)
 ilen           unirand     p5 / 30                                ; random length (0-1)
 irnd           random      4, 20
 ilen           =           (ilen * 250) + irnd                    ; random length (in pixels)
 icol           linrand     100                                    ; colour intensity value
 ialpha         random      100, 255
 ; create string with changes
 Smsg           sprintf     "bounds(%d,%d,%d,%d), rotate(%f,0,0), colour(255,255,%d,%d), visible(1)",i(gkMOUSE_X),i(gkMOUSE_Y),ilen,rnd(2)+1,irad,255-icol,ialpha
                chnset      Smsg,Sident                            ; send string to widget
                event_i     "i", 12, 0.04, 0, p4                   ; call instrument to 'hide' spark after a short delay
endin

instr 12 ; hide spark
                Sident      sprintf "spark_ident%d",p4             ; define indent channel string
                chnset      "visible(0)", Sident                   ; send string to widget
endin

instr 99  ; reverb
 aL,aR          reverbsc    gasendL, gasendR, 0.7, 7000
                outs        aL, aR
                clear       gasendL, gasendR
endin

instr 1000  ; flash instructions
 ktrig          init        1                                         ; trigger intially '1' in order to create the first random 'rate' value
 krate          trandom     ktrig, 1, 10                              ; metronome rate values: a new random value upon each metronome beat
 ktrig          metro       krate                                     ; create a metronome
 kdur           trandom     ktrig, 0.03, 0.2                          ; a new 'flash' duration upon each new beat
 kvis           TriggerToGatek  ktrig, kdur, 0.2                      ; extend the trigger to a pulse of 'kdur' duration (using a UDO - see instr 0 above)
 kcol           randomi     0, 255, 2, 1                              ; colour opacity changing continuously and randomly
 if changed(kvis)==1 then                                             ; if flash changing from on to off or vice versa
  Smsg          sprintfk    "visible(%d), fontColour(255, 255, 255, %d)", kvis, kcol ; create string
                chnset      Smsg,"instructions"                       ; send string to widget
 endif

 ; 50 Hz mains hum
 khum TriggerToGatek  ktrig, kdur, 0.2
 aamp           interp      khum*0.4                     ; amplitude derives from mouse down position and inversely to trigger impulses
 kpw            rspline     0.01,0.04,2,6                ; slight pulse width shift
 kjit           rspline     -10,10,.1,.5                 ; frequency jitter
 asq            vco2        1, 50*cent(kjit), 4, kpw, 0  ; square wave
 asq            *=          aamp                         ; scale amplitude
 asq            atone       asq, 2000
 kpan           rspline     0.3, 0.7, 1, 5               ; panning function
 aL1, aR1       pan2        asq, kpan                    ; create stereo output
 ; a second hum signal
 kpw            rspline     0.01, 0.1, 0.2, 0.6            ; slight pulse width shift
 kjit           rspline     -10,10, 0.1, 0.5               ; frequency jitter
 asq            vco2        1, 50 * cent(kjit), 4, kpw, 0  ; square wave
 asq            *=          aamp                           ; scale amplitude
 asq            atone       asq, 2000
 kpan           rspline     0.3, 0.7, 1, 5                 ; panning function
 aL2,aR2        pan2        asq, kpan                      ; create stereo output
 
                outs        aL1 + aL2, aR1 + aR2           ; mix hum signals and send to output

 ; turn off this instrument and hide instructions when the mouse is first left-clicked.
 kMOUSE_DOWN_LEFT chnget  "MOUSE_DOWN_LEFT"                 ; sense mouse left click
 if kMOUSE_DOWN_LEFT==1 then                                ; if left clicked...
                chnset      "visible(0)", "instructions"    ; ...hide instructions
                event       "i", 1, 0, 3600                 ; start the 'sparks and hum' instrument
                turnoff                                     ; turn off this instrument 
 endif
endin

</CsInstruments>

<CsScore>
;i 99 0 [3600*24*7]   ; reverb
i 1000 0 [3600*24*7]  ; instructions
</CsScore>

</CsoundSynthesizer>