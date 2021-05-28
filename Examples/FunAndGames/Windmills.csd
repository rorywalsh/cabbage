
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; Windmills.csd
; Written by Iain McCurdy, 2014
; 
; This doesn't make sound yet!

<Cabbage>
form caption("Windmills") size(700,500), colour(150,150,255), scrollBars(0), guiRefresh(16), pluginId("WnMl")
; SUN
image bounds(610, 20,70,70), colour(250,250,100), shape(ellipse)            ; sun

; CLOUDS
image bounds( 30,50,40,40), colour(240,240,240), shape(ellipse), identChannel(cloud1_a)
image bounds( 50,30,60,60), colour(240,240,240), shape(ellipse), identChannel(cloud1_b)
image bounds( 90,50,40,40), colour(240,240,240), shape(ellipse), identChannel(cloud1_c)
image bounds( 52,70,56,20), colour(240,240,240), shape(sharp),   identChannel(cloud1_d)

image bounds(125, 90, 60,60), colour(240,240,240), shape(ellipse), identChannel(cloud2_a)
image bounds(155, 60, 90,90), colour(240,240,240), shape(ellipse), identChannel(cloud2_b)
image bounds(215, 90, 60,60), colour(240,240,240), shape(ellipse), identChannel(cloud2_c)
image bounds(160,120, 80,30), colour(240,240,240), shape(sharp),   identChannel(cloud2_d)

image bounds(290, 80, 25, 25), colour(240,240,240), shape(ellipse), identChannel(cloud3_a)
image bounds(305, 65, 35, 35), colour(240,240,240), shape(ellipse), identChannel(cloud3_b)
image bounds(330, 80, 25, 25), colour(240,240,240), shape(ellipse), identChannel(cloud3_c)
image bounds(302, 90, 36, 15), colour(240,240,240), shape(sharp),   identChannel(cloud3_d)

; WINDMILL 1
image bounds( 96,150,  8,200), colour(white), shape(sharp)                 ; post
image bounds( 92,142, 16, 16), colour(white), shape(ellipse)                ; joint
image bounds( 98,154,  0,  0), colour(white), shape(ellipse), identChannel("Axle1")    ; axle
image bounds(  0,  0,  0,  0), colour(white), shape(ellipse), widgetArray("CircleSix1",6)


; WINDMILL 3
image bounds(548,220, 4, 200), colour(white), shape(sharp)                 ; post
image bounds(546,216, 8,   8), colour(white), shape(ellipse)                ; joint
image bounds(548,224,  0,  0), colour(white), shape(ellipse),   identChannel("Axle3")    ; axle
image bounds(  0,  0,  0,  0), colour(white), shape(ellipse), widgetArray("CircleNine3",9)


; HILLS
image bounds( 100,310,750,500),  colour(  0,170,100), shape(ellipse)            ; hill
image bounds( 350,280,1050,500), colour(  0,160, 50), shape(ellipse)            ; hill
image bounds(-400,300,900,500),  colour(  0,150,  0), shape(ellipse)            ; hill


; WINDMILL 2
image bounds(296,170, 8, 200), colour(white), shape(sharp)                 ; post
image bounds(296,368, 8,   4), colour(white), shape(ellipse)                 ; base
image bounds(292,162,16,  16), colour(white), shape(ellipse)                ; joint
image bounds(298,174,  0,  0), colour(white), shape(ellipse),   identChannel("Axle2")    ; axle
image bounds(  0,  0,  0,  0), colour(white), shape(ellipse), widgetArray("CircleSeven2",7)


; WINDMILL 4
image bounds(594,470, 12, 200), colour(white), shape(sharp)                 ; post
image bounds(588,458, 24, 24), colour(white), shape(ellipse)                ; joint
image bounds(600,474,  0,  0), colour(white), shape(ellipse),   identChannel("Axle4")    ; axle
image bounds(  0,  0,  0,  0), colour(white), shape(ellipse), widgetArray("CircleSeven4",7)

; BLACK OUT SCREEN
image bounds(  0,  0,700,500), colour("black"), shape(sharp), identChannel("Blackout")

; INSTRUCTIONS
label    bounds(  0, 0,  0, 0), text("Blow into your microphone..."), align(centre), fontColour(white), identChannel("instructions1")        ; Instructions

</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-n -dm0
</CsOptions>

<CsInstruments>

; sr set by host
ksmps = 32
nchnls = 2
0dbfs=1

gisine    ftgen    0,0,4096,10,1
giPanelWidth    =    700
giPanelHeight    =    500
giRefreshRate    =    1024

instr    1
 aL,aR    ins
 krms    rms    (aL+aR)*4

 ; Move clouds
 kCloudOffset1,kCloudOffset2,kCloudOffset3    init    0
 kCloudTrig1    metro    (giRefreshRate/ksmps)*0.05,(kr-1)/kr
 kCloudTrig2    metro    (giRefreshRate/ksmps)*0.1,(kr-1)/kr
 kCloudTrig3    metro    (giRefreshRate/ksmps)*0.15,(kr-1)/kr
 if kCloudTrig1==1 then
  Smess1a    sprintfk    "bounds( %d,50,40,40)",30+kCloudOffset1
  Smess1b    sprintfk    "bounds( %d,30,60,60)",50+kCloudOffset1
  Smess1c    sprintfk    "bounds( %d,50,40,40)",90+kCloudOffset1
  Smess1d    sprintfk    "bounds( %d,70,56,20)",52+kCloudOffset1
          chnset        Smess1a,"cloud1_a"
          chnset        Smess1b,"cloud1_b"
          chnset        Smess1c,"cloud1_c"
          chnset        Smess1d,"cloud1_d"
  kCloudOffset1    wrap    kCloudOffset1+1,-200,giPanelWidth             
 elseif kCloudTrig2==1 then
  Smess2a    sprintfk    "bounds(%d, 90, 60,60)",125+kCloudOffset2
  Smess2b    sprintfk    "bounds(%d, 60, 90,90)",155+kCloudOffset2
  Smess2c    sprintfk    "bounds(%d, 90, 60,60)",215+kCloudOffset2
  Smess2d    sprintfk    "bounds(%d,120, 80,30)",160+kCloudOffset2
          chnset        Smess2a,"cloud2_a"
          chnset        Smess2b,"cloud2_b"
          chnset        Smess2c,"cloud2_c"
          chnset        Smess2d,"cloud2_d"
  kCloudOffset2    wrap    kCloudOffset2+1,-250,giPanelWidth             
 elseif kCloudTrig3==1 then
  Smess3a    sprintfk    "bounds(%d, 80, 25, 25)",290+kCloudOffset3
  Smess3b    sprintfk    "bounds(%d, 65, 35, 35)",305+kCloudOffset3
  Smess3c    sprintfk    "bounds(%d, 80, 25, 25)",330+kCloudOffset3
  Smess3d    sprintfk    "bounds(%d, 90, 40, 15)",302+kCloudOffset3
          chnset        Smess3a,"cloud3_a"
          chnset        Smess3b,"cloud3_b"
          chnset        Smess3c,"cloud3_c"
          chnset        Smess3d,"cloud3_d"
  kCloudOffset3    wrap    kCloudOffset3+1,-300,giPanelWidth             
 endif       
             
 ; MACRO FOR AN INDIVIDUAL CIRCLE
 #define INDIVIDUAL_CIRCLE(N'P'NUMBER'WIDTH'X'Y'NUM)
 #

 kwidth$N    =    $WIDTH
 kspeed2    portk    kspeed,limit((ksize*kMaxWidth)/1000,0,2)
 
 kx$N    oscil    kwidth$N*(1-abs(kSwing2)),kspeed2,gisine,$P
 ky$N    oscil    kwidth$N,kspeed2,gisine,$P+0.25

 ; Create a sound
 ;ktrig    trigger    kx$N,0,0
 ;schedkwhen    ktrig,0,0,100+$NUM,0,0.05,$GroupNum,$N,kspeed2,kSwing2

 if kRefreshTrig==1 then
  Smess$N    sprintfk    "bounds(%d,%d,%d,%d), colour(%d,%d,%d)", kx$N+$X+kX_Offset+(kAxleLenMax*kSwing2)-(ksize*0.5) - (ksize*0.5*abs(kSwing2)), ky$N+$Y-(ksize*0.5), ksize*ktilt, ksize, kR, kG, kB
  Sident    sprintfk    "Circle$NUMBER.%d_ident%d",$NUM,$N
  chnset    Smess$N,Sident
 endif
 #




 #define CIRCLE_OF_FIVE(SIZE'WIDTH'X'Y'NUM)
 #
  $INDIVIDUAL_CIRCLE(1'0/5'Five'$WIDTH'$X'$Y'$NUM)
  $INDIVIDUAL_CIRCLE(2'1/5'Five'$WIDTH'$X'$Y'$NUM)
  $INDIVIDUAL_CIRCLE(3'2/5'Five'$WIDTH'$X'$Y'$NUM)
  $INDIVIDUAL_CIRCLE(4'3/5'Five'$WIDTH'$X'$Y'$NUM)
  $INDIVIDUAL_CIRCLE(5'4/5'Five'$WIDTH'$X'$Y'$NUM)
 #

 #define CIRCLE_OF_SIX(SIZE'WIDTH'X'Y'NUM)
 #
  $INDIVIDUAL_CIRCLE(1'0/6'Six'$WIDTH'$X'$Y'$NUM)
  $INDIVIDUAL_CIRCLE(2'1/6'Six'$WIDTH'$X'$Y'$NUM)
  $INDIVIDUAL_CIRCLE(3'2/6'Six'$WIDTH'$X'$Y'$NUM)
  $INDIVIDUAL_CIRCLE(4'3/6'Six'$WIDTH'$X'$Y'$NUM)
  $INDIVIDUAL_CIRCLE(5'4/6'Six'$WIDTH'$X'$Y'$NUM)
  $INDIVIDUAL_CIRCLE(6'5/6'Six'$WIDTH'$X'$Y'$NUM)
 #

 #define CIRCLE_OF_SEVEN(SIZE'WIDTH'X'Y'NUM)
 #
  $INDIVIDUAL_CIRCLE(1'0/7'Seven'$WIDTH'$X'$Y'$NUM)
  $INDIVIDUAL_CIRCLE(2'1/7'Seven'$WIDTH'$X'$Y'$NUM)
  $INDIVIDUAL_CIRCLE(3'2/7'Seven'$WIDTH'$X'$Y'$NUM)
  $INDIVIDUAL_CIRCLE(4'3/7'Seven'$WIDTH'$X'$Y'$NUM)
  $INDIVIDUAL_CIRCLE(5'4/7'Seven'$WIDTH'$X'$Y'$NUM)
  $INDIVIDUAL_CIRCLE(6'5/7'Seven'$WIDTH'$X'$Y'$NUM)
  $INDIVIDUAL_CIRCLE(7'6/7'Seven'$WIDTH'$X'$Y'$NUM)
 #

 #define CIRCLE_OF_NINE(SIZE'WIDTH'X'Y'NUM)
 #
  $INDIVIDUAL_CIRCLE(1'0/9'Nine'$WIDTH'$X'$Y'$NUM)
  $INDIVIDUAL_CIRCLE(2'1/9'Nine'$WIDTH'$X'$Y'$NUM)
  $INDIVIDUAL_CIRCLE(3'2/9'Nine'$WIDTH'$X'$Y'$NUM)
  $INDIVIDUAL_CIRCLE(4'3/9'Nine'$WIDTH'$X'$Y'$NUM)
  $INDIVIDUAL_CIRCLE(5'4/9'Nine'$WIDTH'$X'$Y'$NUM)
  $INDIVIDUAL_CIRCLE(6'5/9'Nine'$WIDTH'$X'$Y'$NUM)
  $INDIVIDUAL_CIRCLE(7'6/9'Nine'$WIDTH'$X'$Y'$NUM)
  $INDIVIDUAL_CIRCLE(8'7/9'Nine'$WIDTH'$X'$Y'$NUM)
  $INDIVIDUAL_CIRCLE(9'8/9'Nine'$WIDTH'$X'$Y'$NUM)
 #



 #define WINDMILL(NUM'X'Y'SIZE'WIDTH'R'G'B'AXLE_WIDTH'AXLE_LENGTH)
 #
 ; WINDMILL 1
 kX    =    $X
 kY    =    $Y
 ksize    =    $SIZE
 kMaxWidth    =    $WIDTH
 kSwing2    portk kSwing,limit((ksize*kMaxWidth)/1000,0,2)
 ktilt    =    1 - abs(kSwing2)

 ; printk    0.5,ktilt    ; 1 = full 0.5 = tilted
 ; printk    0.5,kspeed        ; 1 = full 0.5 = tilted

 kWidth        =    (1-abs(kSwing2))*kMaxWidth
 kX_Offset    =    (kMaxWidth - kWidth) * 0.5
 
 kColour    =    (kSwing2+1)*0.5
 kR        scale    kColour, $R, 10
 kG        scale    kColour, $G,  0
 kB        scale    kColour, $B,  0
 
 iAxle_Width$NUM    =    $AXLE_WIDTH

 kAxleLenMax    =    $AXLE_LENGTH
 kAxleLen    =    kAxleLenMax*abs(kSwing2)
 kAxleX        =    limit(kSwing2,-1,0) * kAxleLenMax        

 kRefreshTrig    metro    giRefreshRate/ksmps,$NUM/4
 if kRefreshTrig==1 then
  Smess2    sprintfk    "bounds(%d,%d,%d,%d)",kX+kAxleX, kY-(iAxle_Width$NUM*0.5), kAxleLen ,iAxle_Width$NUM
  Sident    sprintfk    "Axle%d",$NUM
  chnset    Smess2,Sident
 endif
 #


 kspeed    rspline    -5,5,0.025,0.1
 kSwing    rspline    -0.95,0.95,0.05,0.2
 kspeed    =    kspeed * krms
 ;kSwing    =    kSwing * krms

 ;         NUM  X   Y SIZE WIDTH R G B Ax_WID AX_LEN
 $WINDMILL(1  '100'150'20'50'255'70'70'8'30)
 ;               SIZE'WIDTH'X'Y
 $CIRCLE_OF_SIX(ksize'kMaxWidth'kX'kY'1)

 ;         NUM  X   Y SIZE WIDTH R G B Ax_WID AX_LEN
 $WINDMILL(2  '300'170'40'80'70'70'255'8'30)
 ;                SIZE'WIDTH'X'Y
 $CIRCLE_OF_SEVEN(ksize'kMaxWidth'kX'kY'2)

 ;         NUM  X   Y SIZE WIDTH R G B Ax_WID AX_LEN
 $WINDMILL(3  '550'220'12'30'255'0'200'4'15)
 ;               SIZE'WIDTH'X'Y
 $CIRCLE_OF_NINE(ksize'kMaxWidth'kX'kY'3)

 ;         NUM  X   Y SIZE WIDTH R G B Ax_WID AX_LEN
 $WINDMILL(4  '600'470'90'200'200'  0'250'12'50)
 ;                SIZE'WIDTH'X'Y
 $CIRCLE_OF_SEVEN(ksize'kMaxWidth'kX'kY'4)
endin





instr    1001    ; Print and then hide instructions
 p3    =    3.5

 kalpha        linseg    1,0.5,255,1,255,0.5,0        ; Writing transparency
 kalpha2    linseg    255,2,255,1.5,0            ; Blacking screen transparency

 if metro(giRefreshRate/ksmps)==1 then            ; peg updates or interrupt if mouse has been clicked
  Smess1    sprintfk    "bounds(%d,%d,400,25), fontColour(255,255,255,%d)", (giPanelWidth*0.5) - 200, (giPanelHeight*0.5)-20, kalpha
  chnset    Smess1,"instructions1"
  Smess2    sprintfk    "colour(0,0,0,%d)", kalpha2
  chnset    Smess2,"Blackout"
 endif

endin


</CsInstruments>  

<CsScore>
i 1 0 [3600*24*7] 
i 1001 0 0
</CsScore>

</CsoundSynthesizer>
