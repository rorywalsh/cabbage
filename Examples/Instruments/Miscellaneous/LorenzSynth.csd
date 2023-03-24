
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; LorenzSynth.csd
; Iain McCurdy, 2015

; Lorenz generator used as an LFO (see Csound manual for further information and a link to furter reading)

; The output (x,y,z) is visualised using coloured balls (red,green,blue, and sonified as follows:
; red   : fof         x:fundemental_freq.  y:formant_freq./panning      z:amplitude
; green : vco_square  x:freq.              y:moogladder_cutoff/panning  z:amplitude
; blue  : hsboscil    x:tone.              y:brightness/panning         z:amplitude

; All 3 lorenz instruments use the same input parameters

; Parameters
; ----------
; Sigma      --  the Prandtl number or sigma 
; Rayleigh   --  the Rayleigh number 
; Len:Wid    --  the ratio of the length and width of the box in which the convection currents are generated 
; Step Size  --  the step size used in approximating the differential equation. This can be used to control the speed/pitch of the output.
; X Scale    --  scaling of x output
; Y Scale    --  scaling of y output  
; Z Scale    --  scaling of z output
; Skip Step  --  can be used to speed up the output

; x output visualised by the horizonatal location of the balls, y output by vertical position and z output by the size of the balls.

; Three visualisations/sonifications are implemented represented by the three balls: red, green, blue
 


<Cabbage> 
form size(1000,550), guiRefresh(32), caption("Lorenz Synth"), colour(220,220,220), pluginId("Lore")

image bounds(0,0,-30,-30), shape("ellipse"), identChannel("IndicatorBallRed"), colour("red"), visible(0), alpha(0.85)
image bounds(0,0,-30,-30), shape("ellipse"), identChannel("IndicatorBallGreen"), colour("green"), visible(0), alpha(0.85)
image bounds(0,0,-30,-30), shape("ellipse"), identChannel("IndicatorBallBlue"), colour("blue"), visible(0), alpha(0.85)

checkbox  bounds(28,  5,15,15), channel("RedBall"), colour("red"), shape("ellipse"), value(1)
checkbox  bounds(28, 25,15,15), channel("GreenBall"), colour("green"), shape("ellipse")
checkbox  bounds(28, 45,15,15), channel("BlueBall"), colour("blue"), shape("ellipse")

nslider bounds(5, 65,60,30), text("Sigma"),     channel("sv"), range(0.001,1000,2,1,0.001),           colour(220,220,220), textColour("black"), fontColour("black")
nslider bounds(5,100,60,30), text("Rayleigh"),  channel("rv"), range(0.001,1000,40,1,0.001),           colour(220,220,220), textColour("black"), fontColour("black")
nslider bounds(5,135,60,30), text("Len:Wid"),   channel("bv"), range(0.001,0.999,0.663,1,0.001),     colour(220,220,220), textColour("black"), fontColour("black")
nslider bounds(5,170,60,30), text("Step Size"), channel("h"),  range(0.000001,0.001,0.00002,1,0.000001), colour(220,220,220), textColour("black"), fontColour("black")
nslider bounds(5,205,60,30), text("X Scale"), channel("x_scal"),  range(0.001,10,0.03,1,0.001),           colour(220,220,220), textColour("black"), fontColour("black")
nslider bounds(5,240,60,30), text("Y Scale"), channel("y_scal"),  range(0.001,10,0.03,1,0.001),           colour(220,220,220), textColour("black"), fontColour("black")
nslider bounds(5,275,60,30), text("Z Scale"), channel("z_scal"),  range(0.001,10,0.01,1,0.001),           colour(220,220,220), textColour("black"), fontColour("black")
nslider bounds(5,310,60,30), text("Skip Step"), channel("skip"),  range(0,10000,0,1,1),      colour(220,220,220), textColour("black"), fontColour("black")
button    bounds(5,350,60,18), text("Restart","Restart"), channel("restart"),                          colour:0(220,220,220), colour:1(220,220,220), fontColour:0("black"), fontColour:1("black"), latched(0)

label bounds(0,530,100,10), text("Iain McCurdy |2015|"), fontColour(50,50,50)
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-n -dm0
</CsOptions>

<CsInstruments>

; sr set by host
ksmps = 4
nchnls = 2
0dbfs = 1

giPanel_Width    =    1000
giPanel_Height    =    550
giHalfSine    ftgen    0,0,4096,-19, 0.5,1,0,0

instr    1

#define TURN_BALL_ON_OFF(COLOUR)
#
 k$COLOUR.Ball    chnget    "$COLOUR.Ball"
 if changed(k$COLOUR.Ball)==1 then
  if k$COLOUR.Ball==1 then
   chnset    "visible(1)","IndicatorBall$COLOUR"
   event "i","$COLOUR",0,-1
  else
   turnoff2 "$COLOUR",0,1
   chnset    "visible(0)","IndicatorBall$COLOUR"
  endif
 endif
#
$TURN_BALL_ON_OFF(Red)
$TURN_BALL_ON_OFF(Green)
$TURN_BALL_ON_OFF(Blue)

endin












#define LORENZ_GENERATOR(COLOUR)
#
 ksv    chnget    "sv"                ; Prandtl number or sigma 
 krv    chnget    "rv"                ; Rayleigh number 
 kbv    chnget    "bv"                ; ratio of the length and width of the box in which the convection currents are generated 
 kh    chnget    "h"                ; step size used in approximating the differential equation
 krestart    chnget    "restart"               ; restart button
 krestart    trigger    krestart,0.5,0        ; trigger generated whenever restart button is clicked
 kskip        chnget    "skip"            ; skip generated values. Can be used to speed up output changes.
 if changed(krestart,kskip)==1 then        ; test for reiniting for i-rate input parameters
  reinit UPDATE                    
 endif
 UPDATE:
  ax, ay, az lorenz ksv, krv, kbv, kh, (rnd(1)*2)-1, (rnd(1)*2)-1, rnd(1), i(kskip)
 rireturn
 kx    downsamp    ax            ; create k-rate versions
 ky    downsamp    ay
 kz    downsamp    az
 kx_scal    chnget        "x_scal"    ; read scaling values
 ky_scal    chnget        "y_scal"    
 kz_scal    chnget        "z_scal"
 kx        *=    kx_scal            ; scale x, y and z values
 ky        *=    ky_scal    
 kz        *=    kz_scal    
 kx_norm    limit    kx + 0.5, 0, 1        ; create normalised x, y and z values
 ky_norm    limit    ky + 0.5, 0, 1    
 kz_norm    limit    kz      , 0, 1    
 if metro(32)==1 then
  krel    release
  Smsg    sprintfk    "visible(%d), pos(%d,%d), size(%d,%d)",1-krel, (giPanel_Width*0.5) + (kx * giPanel_Width*0.6), (giPanel_Height*0.5)  + (ky * giPanel_Height*0.5), 1 + (kz^2)*200, 1 + (kz^2)*200
  chnset    Smsg,"IndicatorBall$COLOUR"
 endif    
#







instr    Red    ; red / fof instrument
 $LORENZ_GENERATOR(Red)
 ; sonify
 kNumMin    =    4
 kNumMax    =    72
 knum    scale    kx_norm,kNumMax,kNumMin
 knum    limit    knum,kNumMin,kNumMax
 kpan    scale    kx_norm,0,1
 kpan    limit    kpan,0,1
 kCFmin    =    6
 kCFmax    =    10
 kcfoct    scale    1-ky_norm,kCFmax,kCFmin    ; invert
 kcfoct    limit    kcfoct,kCFmin,kCFmax
 kamp    scale    kz_norm,1,0
 kamp    limit    kamp,0,1
 aamp    interp    kamp
 aenv    linsegr    1,0.1,0 
 kamp    tablei    kx_norm,giHalfSine,1
 kamp    pow    kamp,0.5
 gisine    ftgenonce    0, 0, 1024, 10, 1        ;SINE WAVE
 giexp    ftgenonce    0, 0, 1024, 19, .5, .5, 270, .5    ;EXPONENTIAL CURVE
 ;SOUND IS PRODUCED BY THE FOF OPCODE:
 ;ASIG     FOF     GKAMP  |      KFUND      |      KFORM    | GKOCT | GKBAND | GKRIS | GKDUR | GKDEC | IOLAPS | IFNA |  IFNB |  ITOTDUR
 asig     fof     0.2*kamp,      cpsmidinn(knum),  cpsoct(kcfoct),    0,    100,    0.003,  0.1,   0.07,   500,  gisine,  giexp,     3600
 asig    *=    aamp*aenv
 aL,aR    pan2    asig,kpan
     outs    aL,aR
endin









instr    Green    ; green / vco moogladder
 $LORENZ_GENERATOR(Green)
 ; sonify
 kNumMin    =    24
 kNumMax    =    96
 knum    scale    kx_norm,kNumMax,kNumMin
 knum    limit    knum,kNumMin,kNumMax
 kpan    scale    kx_norm,0,1
 kpan    limit    kpan,0,1
 kCFmin    =    8
 kCFmax    =    10
 kcfoct    scale    1-ky_norm,kCFmax,kCFmin    ; invert
 kcfoct    limit    kcfoct,kCFmin,kCFmax
 kamp    scale    kz_norm,1,0
 kamp    limit    kamp,0,1
 aenv    linsegr    1,0.1,0
 aamp    interp    kamp 
 kamp    tablei    kx_norm,giHalfSine,1
 kamp    pow    kamp,0.5
 asig    vco2    kamp*0.2,cpsmidinn(knum),2,0.5
 asig    moogladder    asig,cpsoct(kcfoct),0.6
 asig    *=    aamp*aenv
 aL,aR    pan2    asig,kpan
     outs    aL,aR
endin







giwfn    ftgen    1, 0, 131072, 10, 1            ;WAVEFORM USED BY THE HSBOSCIL OSCILLATOR
gioctfn    ftgen    0, 0, 4096, -19, 1, 0.5, 270, 0.5    ;A HANNING-TYPE WINDOW


instr    Blue ; blue / hsbocil
 $LORENZ_GENERATOR(Blue)
 ; sonify
 kToneMin    =    -4
 kToneMax    =    4
 ktone    scale    kx_norm,kToneMax,kToneMin
 kpan    scale    kx_norm,0,1
 kpan    limit    kpan,0,1
 kBriteMin    =    -2
 kBriteMax    =    4
 kbrite    scale    1-ky_norm,kBriteMax,kBriteMin; invert
 kbrite    limit    kbrite,kBriteMin,kBriteMax
 kamp    scale    kz_norm,1,0
 kamp    limit    kamp,0,1
 aenv    linsegr    1,0.1,0
 aamp    interp    kamp 
 kamp    tablei    kx_norm,giHalfSine,1
 kamp    pow    kamp,0.5
 ibasfreq    =    100
 ioctcnt    =    2
 iphs        =    0
 asig    hsboscil    kamp*0.2, ktone, kbrite, ibasfreq, giwfn, gioctfn, ioctcnt, iphs    ;CREATE AN hsboscil TONE
 asig    *=    aamp*aenv
 aL,aR    pan2    asig,kpan
     outs    aL,aR
endin

</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
</CsScore>

</CsoundSynthesizer>
