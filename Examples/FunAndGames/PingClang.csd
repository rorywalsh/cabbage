
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; PingClang.csd
; Written by Iain McCurdy, 2014
; 
; A musical toy.
; 
; Blocks can be moved using left click and drag, and can be resized using right click and drag.
; 
; The nature of the sound produced by the blocks is altered by their shape.
; Thin blocks will produce a more harmonic and sustaining sound. Thick blocks will produce inharmonic and less sustained sounds.
; 
; Once balls come to rest on the floor they are rethrown from the top.
; New balls can also be launched using the 'New Ball' button.
; 
; CONTROLS
; --------
; Damping        -    damping of energy that occurs when the ball hits a surface. e.g. low values = rubber ball, high values = cannon ball
; Friction    -    damping of horizonatal movement that occurs when the ball strikes the floor. e.g. low values = hard surface, high values = thick carpet
; Speed        -    speed of the game
; Size        -    size of the ball. Also has a bearing on the sound produced when the ball rebounds off the walls or the floor.
; Angle        -    range of possible angles (in both directions) in which the ball can be thrown. i.e. when zero, all throws will be straight down.
; Resonance    -    resonating time of the blocks when struck. This will also be affected by the shape of the blocks. Thinner blocks resonate for a longer time.
; Blocks        -    number of blocks used in the game
; Notch        -    turns on a notch filter operating at the fundemental frequency of the 'clang' sounds

<Cabbage>
form caption("Ping Clang") size(610,500), guiRefresh(32), pluginId("PnCl")
image              bounds(0,  0,610,500), shape("sharp"), colour("black") ;background
image              bounds(0,465,610,35), shape("sharp"), colour(100,100,100)         ; floor
image bounds( 0, 0, 0,0), shape("sharp"), colour(255,255, 50), identChannel("block1")    ; blocks: yellow
image bounds( 0, 0, 0,0), shape("sharp"), colour( 75,255, 75), identChannel("block2")    ; green
image bounds( 0, 0, 0,0), shape("sharp"), colour( 65, 65,255), identChannel("block3")    ; blue
image bounds( 0, 0, 0,0), shape("sharp"), colour(255, 15, 15), identChannel("block4")    ; red
image bounds( 0, 0, 0,0), shape("sharp"), colour(255,100, 50), identChannel("block5")    ; orange
image bounds( 0, 0, 0,0), shape("sharp"), colour(155, 50,255), identChannel("block6")    ; purple
image bounds( 0, 0, 0,0), shape("sharp"), colour(  0,230,255), identChannel("block7")    ; turquoise
image bounds( 0, 0, 0,0), shape("sharp"), colour(255,  0,255), identChannel("block8")    ; pink
image bounds( 0, 0, 0,0), shape("sharp"), colour(155,155,155), identChannel("block9")    ; grey
                                                             
image bounds(0,0,0,0), shape("ellipse"), colour(255,230,220), identChannel("ball")    ; ball

nslider  bounds( 20,465,60,28), channel("damping"),    range(0.00, 0.999, 0.1,1,0.001),     textColour(white),         text("Damping"),   colour(0,0,0)
nslider  bounds( 90,465,60,28), channel("friction"),   range(0.001, 0.999, 0.03,1,0.001),   textColour(white),         text("Friction"),  colour(0,0,0)
nslider  bounds(160,465,50,28), channel("speed"),      range(0.01,  20, 1,1,0.01),          textColour(white),         text("Speed"),     colour(0,0,0)
nslider  bounds(220,465,40,28), channel("size"),       range(2, 80, 15,1,1),                textColour(white),         text("Size"),      colour(0,0,0)
nslider  bounds(270,465,50,28), channel("angle"),      range(0, 5, 3,1,0.001),              textColour(white),         text("Angle"),     colour(0,0,0)
nslider  bounds(330,465,70,28), channel("resonance"),  range(0.1, 30, 2,1,0.1),             textColour(white),         text("Resonance"), colour(0,0,0)
;nslider  bounds(410,415,40,34), channel("NBlocks"),    range(0, 7, 5,1,1),                 textColour(white),         text("Blocks"),    colour(0,0,0)
button     bounds(460,473,60,21), text("Notch","Notch"), channel("fundemental"), value(0),    fontColour:0( 50, 50, 50), fontColour:1(white)
button     bounds(530,473,60,21), text("New Ball"), channel("NewBall"), value(0), latched(0), fontColour:0(200,200,200), fontColour:1(white)

label      bounds(410,466,45,14), text("Blocks"), align(centre), fontColour(white)
button     bounds(409,483,16,14), text("<"), channel("NBlocksDn"), value(0), latched(0)
button     bounds(440,483,16,14), text(">"), channel("NBlocksUp"), value(0), latched(0)
label      bounds(425,483,15,14), text("5"), align(centre), colour(black), fontColour(white), identChannel("NBlocksID")

label    bounds(  0, 0,  0, 0), text("Use left click and drag to move blocks."), align(centre), fontColour(white), identChannel("instructions1")        ; Instructions
label    bounds(  0, 0,  0, 0), text("Right click and drag to resize blocks."), align(centre), fontColour(white), identChannel("instructions2")        ; Instructions

label    bounds(  1,454,100, 10), text("Iain McCurdy 2014"), align(left), fontColour(255,255,255,100)
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-n -dm0
</CsOptions>

<CsInstruments>

; sr set by host
ksmps = 16
nchnls = 2
0dbfs=1
seed    0

giNumBlocks        =    9
giPanelWidth    =    610
giPanelHeight    =    500
giFloorDepth    =    35
gisine    ftgen    0,0,131072,10,1
gal,gar    init    0
gkactive0,gkactive1,gkactive2,gkactive3,gkactive4,gkactive5,gkactive6,gkactive7,gkactive8,gkactive9    init    0    ; used for polyphony control. Add further variables if additional blocks are needed. NB each block has its own polyphony control mechanism.
giGravAccel    =    2*ksmps                        ; Acceleration due to gravity
giStopThreshold    =    1.2                        ; Speed threshold below which the current ball will be removed and a new ball will be thrown. Values from 1 to 5 are appropriate. Higher values for ball to be thrown quicker.
giWallFloorAmp    =    0.2                        ; Amplitude of bounce sound effects off the walls and floor

instr    1    ; Track mouse position and clicks and move objects as appropriate
 kMOUSE_X        chnget    "MOUSE_X"
 kMOUSE_Y        chnget    "MOUSE_Y"
 kMOUSE_DOWN_LEFT    chnget    "MOUSE_DOWN_LEFT"
 kMOUSE_DOWN_MIDDLE    chnget    "MOUSE_DOWN_MIDDLE"
 kMOUSE_DOWN_RIGHT    chnget    "MOUSE_DOWN_RIGHT"
 kprevX    init    i(kMOUSE_X)    ; Mouse position from previous k-pass
 kprevY    init    i(kMOUSE_Y)    
 
 gkNBlocks    init    5                ; number of blocks counter
 kNBlocksDn    chnget    "NBlocksDn"            ; read in widgets
 kNBlocksUp    chnget    "NBlocksUp"            ;
 if trigger(kNBlocksDn,0.5,0)==1 then            ; if 'down' button is triggered... 
  gkNBlocks    limit    gkNBlocks-1,0,giNumBlocks            ; increment counter up one step (within limits)
  Smess        sprintfk    "text(%d)",gkNBlocks    ; create message to change value indicator (a text label) 
  chnset    Smess,"NBlocksID"            ; send message to widget
 elseif trigger(kNBlocksUp,0.5,0)==1 then        ; do the same for  the 'up' counter button
  gkNBlocks    limit    gkNBlocks+1,0,giNumBlocks
  Smess        sprintfk    "text(%d)",gkNBlocks
  chnset    Smess,"NBlocksID"
 endif
 
 gkdamping    chnget    "damping"
 gkfriction    chnget    "friction"
 gkspeed    chnget    "speed"
 gksize        chnget    "size"
 gkangle    chnget    "angle"
 gkresonance    chnget    "resonance"
 ;gkNBlocks    chnget    "NBlocks"
 gkfundemental    chnget    "fundemental"
 gkNewBall    chnget    "NewBall"
 
 ktrig    metro    kr/(ksmps)
 if ktrig==1 then

 kMOUSE_DOWN_LEFT_trig    trigger    kMOUSE_DOWN_LEFT,0.5,0
 kMOUSE_DOWN_RIGHT_trig    trigger    kMOUSE_DOWN_RIGHT,0.5,0

 gkDeltaX    =    kMOUSE_X - kprevX    ; Change in mouse x position
 gkDeltaY    =    kMOUSE_Y - kprevY    ; Change in mouse y position

#define BLOCK(N)
 #
 kcount        =    $N
 gkSizeX$N    init    random(1,170)                        ; Initial x-size of object
 gkSizeY$N    init    random(1,170)                        ; Initial y=size of object
 gkLocX$N    init    random(0,giPanelWidth-i(gkSizeX$N))            ; Initial x-location of object
 gkLocY$N    init    random(150,giPanelHeight-i(gkSizeY$N)-giFloorDepth)    ; Initial y-location of object
  
 kMoveFlag$N    init    0
 
 kRelL    trigger    kMOUSE_DOWN_LEFT,0.5,1        ; If either click button is released, exit click and drag mode.
 kRelR    trigger    kMOUSE_DOWN_RIGHT,0.5,1
 if kRelL==1||kRelR==1 then
  kMoveFlag$N    =    0            ; When left or right click are released, exit click and drag mode
 endif 
 
 if ((kMOUSE_DOWN_LEFT_trig==1||kMOUSE_DOWN_RIGHT_trig==1) && kMOUSE_X>=(gkLocX$N-1) && kMOUSE_X<=(gkLocX$N+gkSizeX$N+1) && kMOUSE_Y>=(gkLocY$N-1) && kMOUSE_Y<=(gkLocY$N+gkSizeY$N+1)) || kMoveFlag$N==1 then    ; If we are in click and drag mode
  kMoveFlag$N    =    1    ; Set click and drag flag to 'on'
 
  if changed(gkDeltaX,gkDeltaY)==1 then                                ; if a change in mouse position during click
   if kMOUSE_DOWN_LEFT==1 then                                    ; CHANGE LOCATION
    gkLocX$N    limit    gkLocX$N + gkDeltaX, 0, giPanelWidth - gkSizeX$N            ; Set new x location of object with limits
    gkLocY$N    limit    gkLocY$N + gkDeltaY, 0, giPanelHeight - gkSizeY$N - giFloorDepth    ; Set new y location of object with limits 
   elseif kMOUSE_DOWN_RIGHT==1 then                                ; CHANGE SIZE
    gkSizeX$N    limit    gkSizeX$N + gkDeltaX, 2,giPanelWidth-gkLocX$N                ; Set new x size of object with limits
    gkSizeY$N    limit    gkSizeY$N + gkDeltaY, 2,giPanelHeight-gkLocY$N-giFloorDepth        ; Set new x size of object with limits
   endif
   kSizeX$N    =    (kcount>gkNBlocks?0:gkSizeX$N)                        ; If this block is not needed set size to zero
   kSizeY$N    =    (kcount>gkNBlocks?0:gkSizeY$N)
   Smess    sprintfk    "bounds(%d,%d,%d,%d)",gkLocX$N,gkLocY$N,kSizeX$N,kSizeY$N    ; String for new object bounds
   chnset    Smess,"block$N"                                   ; Send new string to object
  endif
  
 endif

 if changed(gkNBlocks)==1 then
  kSizeX$N    =    (kcount>gkNBlocks?0:gkSizeX$N)                    ; If this block is not needed set size to zero
  kSizeY$N    =    (kcount>gkNBlocks?0:gkSizeY$N)
  Smess    sprintfk    "bounds(%d,%d,%d,%d)",gkLocX$N,gkLocY$N,kSizeX$N,kSizeY$N    ; String for new object bounds
  chnset    Smess, "block$N"                               ; Send new string to object
 endif
  
 #
 $BLOCK(1)
 $BLOCK(2)
 $BLOCK(3)
 $BLOCK(4)
 $BLOCK(5)
 $BLOCK(6)
 $BLOCK(7)
 $BLOCK(8)
 $BLOCK(9)
 
 SKIP:
 
 kprevX    =    kMOUSE_X    ; Set 'previous' mouse x and y for next k-pass
 kprevY    =    kMOUSE_Y
endif

endin


instr    2
 isize    =    i(gksize)    ;15        ; Size of ball
 icps_floor    init    500*(15/isize)        ; Floor bounce noise frequency (also dependent upon ball size)
 icps_wall    init    300*(15/isize)        ; Wall bounce noise frequency (also dependent upon ball size)
 iBlipDur    =    0.05*(isize/15)        ; Duration of wall bounce noise
 Sid    =    "ball"                ; Ball ident
 ky    init    -isize                ; Initial y location of ball
 kx    init    random(isize,giPanelWidth-isize); Initial x location of ball
 kxIncr    init    random(-i(gkangle)*ksmps,i(gkangle)*ksmps)    ; Initial x increment - left/right direction of throw
 kyIncr    init    0                ; Initial y increment. Zero means ball is dropped

 irate    =    ksmps*2                ; Rate of gui refresh and data checking
 ktrig    metro    irate

 if ktrig==1 then
  ky    limit    ky + (kyIncr*gkspeed), -isize, giPanelHeight-giFloorDepth-isize    ; increment x and y locations
  kx     limit    kx + (kxIncr*gkspeed), 0, giPanelWidth - isize

   if( (abs(kyIncr)+abs(kxIncr))<giStopThreshold && ky>=(giPanelHeight-giFloorDepth-isize-1)) || changed(gkNewBall)==1 then    ; if ball has come to rest...
    event    "i",100,0,0    ; launch a new ball
    turnoff
   endif

  if ky>=(giPanelHeight-giFloorDepth-isize) then    ; hitting floor
   event    "i",20,0,iBlipDur,kyIncr,icps_floor,kx/giPanelWidth
   kyIncr    =    -kyIncr*(1-gkdamping)    ; flip direction and absorb some energy in the bounce
   kxIncr    =    kxIncr*(1-gkfriction)    ; absorb some horizontal energy in the bounce
  endif

  if kx>=(giPanelWidth-isize) || kx<=0 then    ; flip x direction when hitting a wall
   kxIncr    =    -kxIncr*(1-gkdamping)
   event    "i",20,0,iBlipDur,kyIncr,icps_wall,kx/giPanelWidth
  endif


  
  #define CHECK_OBJECT_STRIKE(N)
  #
  kcount    =    $N
  if kcount>gkNBlocks kgoto SKIP
  if trigger(ky+isize,gkLocY$N,0)==1 && (kx+isize)>=gkLocX$N && kx<=(gkLocX$N+gkSizeX$N) then            ; Striking upper edge
   kyIncr    =    (-kyIncr*(1-gkdamping)) - gkDeltaY
   kxIncr    =    kxIncr*(1-gkfriction)    ; absorb some horizontal energy in the bounce
   event    "i",90+$N,0,gkresonance,kx,kxIncr,kyIncr
   ky        =    gkLocY$N-isize    
  elseif trigger(kx,gkLocX$N+gkSizeX$N,1)==1 && (ky+isize)>=gkLocY$N && ky<=(gkLocY$N+gkSizeY$N) then        ; Striking right hand edge
   kxIncr    =    (-kxIncr*(1-gkdamping)) - gkDeltaX
   event    "i",90+$N,0,gkresonance,kx,kxIncr,kyIncr
   kx        =    gkLocX$N+gkSizeX$N
  elseif trigger(kx+isize,gkLocX$N,0)==1 && (ky+isize)>=gkLocY$N && ky<=(gkLocY$N+gkSizeY$N) then        ; Striking left hand edge
   kxIncr    =    (-kxIncr*(1-gkdamping)) - gkDeltaX
   event    "i",90+$N,0,gkresonance,kx,kxIncr,kyIncr
   kx        =    gkLocX$N-isize
  elseif trigger(ky,gkLocY$N+gkSizeY$N,1)==1 && (kx+isize)>=gkLocX$N && kx<=(gkLocX$N+gkSizeX$N) then        ; Striking lower edge
   kyIncr    =    (-kyIncr*(1-gkdamping)) - gkDeltaY
   event    "i",90+$N,0,gkresonance,kx,kxIncr,kyIncr
   ky        =    gkLocY$N+gkSizeY$N
  endif
  #
  $CHECK_OBJECT_STRIKE(1)
  $CHECK_OBJECT_STRIKE(2)
  $CHECK_OBJECT_STRIKE(3)
  $CHECK_OBJECT_STRIKE(4)
  $CHECK_OBJECT_STRIKE(5)
  $CHECK_OBJECT_STRIKE(6)
  $CHECK_OBJECT_STRIKE(7)
  $CHECK_OBJECT_STRIKE(8)
  $CHECK_OBJECT_STRIKE(9)
  SKIP:                ; Escape to here if finished checking for block collisions
  Smess    sprintfk    "bounds(%d,%d,%d,%d)",kx,ky,isize,isize
  chnset    Smess,Sid

  kyIncr    =    kyIncr + (giGravAccel/irate)
 endif

endin



instr    20    ; wall ricochet sound effect
 /* POLYPHONY CONTROL */
 gkactive0 init i(gkactive0) + 1    ;INCREMENT NOTE COUNTER
 if gkactive0>1 then            ;IF POLYPHONY IS EXCEEDED (THROUGH THE ADDITION OF NEW NOTE)
  turnoff                ;REMOVE THIS NOTE
 endif
 krel release                ;IF NOTE HELD = 0, IF NOTE RELEASED = 1
 ktrig trigger krel,0.5,0        ;WHEN RELEASE FLAG CROSSES 0.5 UPWARDS, I.E. NOTE HAS BEEN RELEASED...    
 if ktrig==1 then        
  gkactive0 = gkactive0 - 1        ;...DECREMENT ACTIVE NOTES COUNTER
 endif
 aenv    expon    giWallFloorAmp,p3,giWallFloorAmp*0.001
 asig    poscil    aenv*((p4/50)^2)*gkspeed,p5
 al,ar    pan2    asig,p6
     outs    al,ar
     gal    += al
     gar    += ar
endin



; Object ricochet sound effect
#define CLANG_INSTR(I'N)
#
instr    $I    ; object bounce
 p3        =    p3 * ((1-(i(gkSizeY$N)/giPanelHeight))^4)    ; duration of the sound scaled by the height of the object. Taller objects sustain for shorter time.

 /* POLYPHONY CONTROL */
 gkactive$N init i(gkactive$N) + 1    ;INCREMENT NOTE COUNTER
 if gkactive$N>1 then            ;IF POLYPHONY IS EXCEEDED (THROUGH THE ADDITION OF NEW NOTE)
  turnoff                ;REMOVE THIS NOTE
 endif
 krel release                ;IF NOTE HELD = 0, IF NOTE RELEASED = 1
 ktrig trigger krel,0.5,0        ;WHEN RELEASE FLAG CROSSES 0.5 UPWARDS, I.E. NOTE HAS BEEN RELEASED...    
 if ktrig==1 then        
  gkactive$N = gkactive$N - 1        ;...DECREMENT ACTIVE NOTES COUNTER
 endif

 iamp    =    (abs(p5) + abs(p6)) * 0.006                ; amplitude dependent upon the speed of the ball at the time of the collision.
 aenv    expsegr    iamp,p3,iamp*0.001,0.05,iamp*0.001            ; amplitude envelope
 kndx    expsegr    (abs(p5)+abs(p6))*0.01,p3,0.0001,0.05,0.0001        ; index of modulation envelope. Overall envelope amplitude (therefore spectral brightness) influenced by the speed of the ball when the object was struck. 
 kporttime    linseg    0,0.001,0.05
 kcps        =    cpsoct(((1-(gkSizeX$N/giPanelWidth))*8)+4)    ; base frequency defined by width of the object
 kcps    portk    kcps,kporttime
 kmod        =    1 + ((gkSizeY$N-1)/300)                ; modulator frequency defined by the height of the object: the taller the object the more inharmonic the spectrum. At minimum thinness ratio = 1.
 kmod    portk    kmod,kporttime                        
 acar    expseg    0.993,0.04,1,5,1                    ; carrier ratio envelope. Helps to create a bit of spectral distortion when the objecrt is struck

 kampscale    =    (gkspeed-0.01)/(20-0.01)
 kampscale    scale    kampscale,3,0.1
 asig     foscil     aenv*kampscale, kcps, acar, kmod, kndx, gisine, 0

 /* Fundemental notch filtering */
 abr    butbr    asig*3,kcps,kcps*0.1
 asig    ntrpol    asig,abr,gkfundemental

 al,ar    pan2    asig,p4
 gal    +=    al
 gar    +=    ar
     outs    al,ar
endin
#
$CLANG_INSTR(91'1)
$CLANG_INSTR(92'2)
$CLANG_INSTR(93'3)
$CLANG_INSTR(94'4)
$CLANG_INSTR(95'5)
$CLANG_INSTR(96'6)
$CLANG_INSTR(97'7)
$CLANG_INSTR(98'8)
$CLANG_INSTR(99'9)



instr    100    ; Launch a new ball
 event_i    "i",2,0,300
endin


instr    999
    iChoRte        =    1.2
    iChoDep        =    0.01
    kporttime    linseg    0,0.001,1
    kporttime    =    kporttime/iChoRte
    kdlt1    randomi    ksmps/sr,iChoDep,iChoRte,1
    kdlt1    portk    kdlt1,kporttime
    adlt1    interp    kdlt1
    acho1    vdelay    gal,adlt1*1000,1*1000
    
    kdlt2    randomi    ksmps/sr,iChoDep,iChoRte,1
    kdlt2    portk    kdlt2,kporttime
    adlt2    interp    kdlt2
    acho2    vdelay    gal,adlt2*1000,1*1000
    
        outs    acho1, acho2
    gal    +=    acho1
    gar    +=    acho2
endin


instr    1000    ; Reverb
 al,ar    reverbsc    gal,gar,0.7,5000
     outs    al*0.3,ar*0.3
     clear    gal,gar
endin

instr    1001    ; Print and then hide instructions
 kMOUSE_DOWN_LEFT    chnget    "MOUSE_DOWN_LEFT"    ; sense left click
 kEscapeTrig    trigger    kMOUSE_DOWN_LEFT,0.5,0        ; trigger of left click is pressed
 krel release                        ; release flag - normally 0, 1 at note release
 kalpha    transeg    0,0.5,4,255,p3-1,0,255,0.5,-4,-1    ; colour alpha channel: fades in, holds, fades out

 if metro(ksmps)==1||krel==1 then            ; peg updates or interrupt if mouse has been clicked
  Smess1    sprintfk    "bounds(%d,%d,400,17), fontColour(255,255,255,%d)", (giPanelWidth*0.5) - 200, (giPanelHeight*0.5) - 50, kalpha*(1-krel)
  Smess2    sprintfk    "bounds(%d,%d,400,17), fontColour(255,255,255,%d)", (giPanelWidth*0.5) - 200, (giPanelHeight*0.5) - 25, kalpha*(1-krel)
  chnset    Smess1,"instructions1"
  chnset    Smess2,"instructions2"
 endif

 ktime        times                    ; note elapsed time in seconds
 if trigger(ktime,p3-0.2,0)==1||kEscapeTrig==1 then    ; if time is up mouse has been clicked...
  event "i",1,0,3600*24*7    ; track mouse, move and resize blocks, and read in widgets
  event "i",2,0.1,3600        ; first throw
  turnoff
 endif

endin


</CsInstruments>  

<CsScore>
i 1001 0 4        ; Instructions fade up then down (currently not working)
i 999  0    [3600*24*7]    ; chorus
i 1000 0    [3600*24*7]    ; reverb

</CsScore>

</CsoundSynthesizer>
