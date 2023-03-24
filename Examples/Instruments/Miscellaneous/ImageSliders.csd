
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; ImageSliders.csd
; Written by Iain McCurdy, 2014

; This example demonstrates a method to create sliders from the 'image' widget.
; This opens a number of possibilities in terms of how we can customise the look and behaviour of a slider
; A distinct disadvantage (for the moment anyway) is that it is not possible to click and drag a 'slider' of this type.
; This is because Cabbage will not read changes in mouse position when the mouse button is held.
; Therefore the sliders in this example change simply when the mouse is hovered over their area in the panel.
; In addition to changing size, the image/slider also increases in colour intensity as it is raised.

; Macros are used to faciliate the reuse of the relevant code in this example in other csds.
; Separate macros are used for vertically up and vertically down sliders.
; The two rows of 10 sliders vary the strengths of 10 partials for two additive tones.

<Cabbage>
form caption("Image Sliders"), scrollBars(0), size(610, 540), pluginId("ExSl"),colour(0,0,0), guiRefresh(64)
image bounds(0,0,0,0), widgetArray("slider",20), shape("rounded")
label      bounds(170,255,250, 20), text("Click and drag anywhere..."), fontColour("white"), visible(1), identChannel("instruction")
label bounds(5,528,110, 11), text("Iain McCurdy |2013|"), fontColour(75,75,75), align("left")
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-dm0 -n
</CsOptions>

<CsInstruments>

; sr set by host
ksmps       =     32    ;NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls      =     2    ;NUMBER OF CHANNELS (2=STEREO)
0dbfs       =     1

giwav           ftgen   0,0,4096,9, 1,1,0,  0.5,0.3,0,  2,0.1,0,  4,0.05,0, 8,0.01,0  ; Waveform used by the oscillator.
giAmpCurve      ftgen   1,0,-20000,-16, 1,100,0,1, 900,-3,0.2,19000,0,0.2
gaSendL,gaSendR init    0                                                             ; Global audio send variables used to send audio to the reverb effect.

giwidth         =       50     ; Slider width
giheight        =       250    ; Slider height

instr    1 
 kMOUSE_X           chnget  "MOUSE_X"         ; Read in mouse X position (in pixels across the panel)
 kMOUSE_Y           chnget  "MOUSE_Y"         ; Read in mouse Y position (in pixels across the panel)
 gkMOUSE_DOWN_LEFT  chnget  "MOUSE_DOWN_LEFT" ; Read in mouse left click status
 
; Define a macro containing all the code relevant to the graphical representation of an upwardly moving image-slider.
#define    SLIDER_UP(X'Y'R'G'B'N)
#
 if    kMOUSE_X>=$X && kMOUSE_X<=($X+giwidth) && kMOUSE_Y>=$Y && kMOUSE_Y <= ($Y+giheight) && gkMOUSE_DOWN_LEFT==1 then    ; If the mouse is within the panel area for this image widget and left mouse button is down...
  ky$N        =    kMOUSE_Y                                    ; Location of top of image-slider.
  kheight$N   =    giheight - kMOUSE_Y + 10                            ; Height of image-slider derived. 
  gkslider$N  =    kheight$N/giheight                                ; Normalised slider value (0 - 1)
  kalpha$N    =    (gkslider$N * 200) + 55                                ; Derive an 'alpha' (colour intensity) value from slider value.
 endif
 if changed(ky$N)==1 then                                        ; If a trigger has been generated...
  Smessage$N sprintfk "bounds($X,%d,50,%d),colour($R,$G,$B,%d)", ky$N, kheight$N, kalpha$N        ; Create a string with updated attributes that will be sent to the image-slider widget.
  chnset Smessage$N, "slider_ident$N"                                    ; Send updated attributes to the widget.
 endif
#

; Expand the macro for each slider with relevant substitution values
;          x   y  r   g   b   n
$SLIDER_UP(10 '10'250' 50' 50'1)
$SLIDER_UP(70 '10' 50'250' 50'2)
$SLIDER_UP(130'10' 50' 50'250'3)
$SLIDER_UP(190'10'255'  0'  0'4)
$SLIDER_UP(250'10'255'255' 50'5)
$SLIDER_UP(310'10' 50'150'100'6)
$SLIDER_UP(370'10' 50' 50'250'7)
$SLIDER_UP(430'10'150'  0' 50'8)
$SLIDER_UP(490'10'204'102'  0'9)
$SLIDER_UP(550'10'100'200' 30'10)



; Define a macro containing all the code relevant to the graphical representation of an downwardly moving image-slider.
#define    SLIDER_DOWN(X'Y'R'G'B'N)
#
 if    kMOUSE_X>=$X && kMOUSE_X<=($X+giwidth) && kMOUSE_Y>=$Y && kMOUSE_Y <= ($Y+giheight) && gkMOUSE_DOWN_LEFT==1 then    ; If the mouse is within the panel area for this image widget and left mouse button is down...
  kheight$N    =    kMOUSE_Y - $Y                                    ; Height of image-slider derived. 
  gkslider$N    =    kheight$N/giheight                                ; Normalised slider value (0 - 1)
  kalpha$N    =    (gkslider$N * 200) + 55                                ; Derive an 'alpha' (colour intensity) value from slider value.
 endif
 if changed(kheight$N)==1 then                                            ; If a trigger has been generated...
  Smessage$N sprintfk "bounds($X,$Y,50,%d),colour($R,$G,$B,%d)", kheight$N, kalpha$N            ; Create a string with updated attributes that will be sent to the image-slider widget.
  chnset Smessage$N, "slider_ident$N"                                        ; Send updated attributes to the widget.
 endif
#

; Expand the macro for each slider with relevant substitution values
;            x    y   r   g   b   n
$SLIDER_DOWN( 10'270'255'255'  0'11)
$SLIDER_DOWN( 70'270'255'  0'127'12)
$SLIDER_DOWN(130'270'128'255'  0'13)
$SLIDER_DOWN(190'270'128'128'128'14)
$SLIDER_DOWN(250'270'204'102'  0'15)
$SLIDER_DOWN(310'270'153'  0' 76'16)
$SLIDER_DOWN(370'270'  0'255'  0'17)
$SLIDER_DOWN(430'270'255'153'153'18)
$SLIDER_DOWN(490'270'178'102'255'19)
$SLIDER_DOWN(550'270'255'255'255'20)



; Create some oscillators. Strengths controlled by the sliders.
; First create a macro to avoid limit repetition.
#define OSCILLATOR(N'F)
#
 kamp    portk    (gkslider$N^2)*0.2, kporttime    ; Apply portamento smoothing to slider changes.
 aamp    interp    kamp                ; Interpolate to create an a-rate (smoother) amplitude value
 kdtn    rspline    -15,15,0.05,0.3            ; Slow and slight detuning (in cents)
 
 iAmpScl$N    =1 ;**doesn't work***tablei    $F, giAmpCurve        ; Scale amplitude according to frequency.
 asig    oscil    aamp*iAmpScl$N, $F * cent(kdtn), giwav    ; Create the oscillator.
 kpan    rspline    0.1,0.9,0.2,0.7            ; Slow panning position
 aL,aR    pan2    asig, kpan            ; Create stereo panned output
 amixL    =    amixL + aL            ; Add to stereo mix signal. 
 amixR    =    amixR + aR            ;
#
amixL    =    0                ; Create and clear stereo mix audio variables
amixR    =    0
kporttime    linseg    0,0.001,0.05        ; Portamento time ramps up from zero

; Expand the macro for each oscillator with relevant substitution values
;           N  Freq.
$OSCILLATOR(1 '100)        ; Tone 1    
$OSCILLATOR(2 '100*2)           ; Tone 2
$OSCILLATOR(3 '100*3)           ; Tone 3
$OSCILLATOR(4 '100*4)           ; Tone 4
$OSCILLATOR(5 '100*5)           ; Tone 5
$OSCILLATOR(6 '100*6)           ; Tone 6
$OSCILLATOR(7 '100*7)           ; Tone 7
$OSCILLATOR(8 '100*8)           ; Tone 8
$OSCILLATOR(9 '100*9)           ; Tone 9
$OSCILLATOR(10'100*10)          ; Tone 10
$OSCILLATOR(11'133)        ; Tone 11
$OSCILLATOR(12'133*2)           ; Tone 12
$OSCILLATOR(13'133*3)           ; Tone 13
$OSCILLATOR(14'133*4)           ; Tone 14
$OSCILLATOR(15'133*5)           ; Tone 15
$OSCILLATOR(16'133*6)           ; Tone 16
$OSCILLATOR(17'133*7)           ; Tone 17
$OSCILLATOR(18'133*8)           ; Tone 18
$OSCILLATOR(19'133*9)           ; Tone 19
$OSCILLATOR(20'133*10)          ; Tone 20

 aRvbL,aRvbR    reverbsc    amixL*0.3,amixR*0.3,0.8,4000    ; Create a reverberated verison of the audio so far.
     outs    amixL+aRvbL, amixR+aRvbR            ; Send dry and reverberated signals to the outputs.

endin

instr    99 ; melt instruction after the first click
 kOffInstruction    trigger    gkMOUSE_DOWN_LEFT,0.5,0
 iTime    =    5
 kalpha    expsegr    255,iTime,1
 ky    linsegr    255,iTime,355
 Sfade    sprintfk    "fontColour(255,255,255,%d), bounds(170,%d,250, 20)",kalpha,ky
 chnset    Sfade,"instruction"
 if kOffInstruction==1 then
  event    "i",100,iTime,0
  turnoff
 endif
endin

instr    100    ; hide widget completely
  chnset    "visible(0)","instruction"
endin

</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
i 99 0 [3600*24*7]
</CsScore>


</CsoundSynthesizer>
