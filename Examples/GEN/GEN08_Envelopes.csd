
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; GEN08_Envelopes.csd
; Written by Iain McCurdy, 2015.

; A method of editing GEN08 function tables by clicking and dragging breakpoints.
; GEN08 generates function tables by drawing the smoothest possible curve between defined breakpoints.
; Note that this can often result in the function extending well above the highest breakpoint or well below the lowest breakpoint.
; In the example when the functions are used as amplitude envelopes they are clipped at zero and 1 to prevent out of range amplitudes.
; It is also to normalise the function to fit within the range 0 to 1. In this case the curve will probably not pass through the breakpoints.

; In this example two envelopes are created: one with 3 breakpoints and the other with 4.
; The first and last breakpoints can only be moved up or down.
; The two envelope types are encapsulated within UDOs to facilitate easy transplantation and reuse.

; Input arguments for the 3-breakpoint envelope are: knorm,iTabX,iTabY,iTabWidth,iTabHeight,iNodeSize,ifn,STabIdent,Snode1,Snode2,Snode3
; iTabX        -    table X location in pixels (from upper panel edge)
; iTabY        -    table Y location in pixels (from left panel edge)
; iTabWidth    -    table width in pixels
; iTabHeight    -    table height in pixels
; iNodeSize    -    node widget radius in pixels
; ifn        -    function table used to store the ADSR (only used for GUI - output values used to create envelope)
; STabIdent    -    a string representing the ident channel of the gentable
; Snode1    -    a string representing the ident channel of the first node
; Snode2    -    a string representing the ident channel of the second node
; Snode3    -    a string representing the ident channel of the third node

; The output arguments are: gky1,gkx2,gky2,gky3
; gky1        -    first breakpoint value    
; gkx2        -    second breakpoint location (0 - 1)
; gky2        -    second breakpoint value    
; gky3        -    third breakpoint value

; The input arguments for the 4-breakpoint envelope are the same except for the addition of an extra node ident channel
; The output arguments for the 4-breakpoint envelope are the same except for the addition of the location and value of the additional breakpoint


<Cabbage>
form caption("GEN08 Envelopes"), size(800, 510), pluginId("G8En"), guiRefresh(32), colour(0,0,0)

gentable bounds( 10,  5,380,290), tableNumber(1), tableColour("silver"), identChannel("table1"), ampRange(0,1,1), zoom(-1), tableGridColour(0,0,0,0), tablebackgroundColour(20,20,20), outlineThickness(2), identChannel("table1")
label bounds(10, 5,140,12), text("3 Break-point Envelope"), align("left")

gentable bounds(410,  5,380,290), tableNumber(2), tableColour("silver"), identChannel("table2"), ampRange(0,1,2), zoom(-1), tableGridColour(0,0,0,0), tablebackgroundColour(20,20,20), outlineThickness(2), identChannel("table2")
label bounds(410, 5,140,12), text("4 Break-point Envelope"), align("left")

label   bounds( -5, -5, 10, 10), text("1"), colour("white"), fontColour("black"), identChannel("node1")
label   bounds( -5, -5, 10, 10), text("2"), colour("white"), fontColour("black"), identChannel("node2")
label   bounds( -5, -5, 10, 10), text("3"), colour("white"), fontColour("black"), identChannel("node3")

nslider  bounds( 10,300, 50,30), text("Value 1"), channel("aV1"), range(0,1,0,1,0.001)
nslider  bounds( 60,300, 50,30), text("Dur. 1"),  channel("aD1"), range(0,1024,0,1,1)
nslider  bounds(110,300, 50,30), text("Value 2"), channel("aV2"), range(0,1,0,1,0.001)
nslider  bounds(160,300, 50,30), text("Dur. 2"),  channel("aD2"), range(0,1024,0,1,1)
nslider  bounds(210,300, 50,30), text("Value 3"), channel("aV3"), range(0,1,0,1,0.001)
nslider  bounds(290,300, 50,30), text("Max"), channel("aMax"), range(0,1000,0,1,0.001)
nslider  bounds(340,300, 50,30), text("Min"), channel("aMin"), range(-1000,0,0,1,0.001)

label   bounds( -5, -5, 10, 10), text("1"), colour("white"), fontColour("black"), identChannel("node4")
label   bounds( -5, -5, 10, 10), text("2"), colour("white"), fontColour("black"), identChannel("node5")
label   bounds( -5, -5, 10, 10), text("3"), colour("white"), fontColour("black"), identChannel("node6")
label   bounds( -5, -5, 10, 10), text("4"), colour("white"), fontColour("black"), identChannel("node7")

nslider  bounds(410,300, 50,30), text("Value 1"), channel("bV1"), range(0,1,0,1,0.001)
nslider  bounds(460,300, 50,30), text("Dur. 1"),  channel("bD1"), range(0,1024,0,1,1)
nslider  bounds(510,300, 50,30), text("Value 2"), channel("bV2"), range(0,1,0,1,0.001)
nslider  bounds(560,300, 50,30), text("Dur. 2"),  channel("bD2"), range(0,1024,0,1,1)
nslider  bounds(610,300, 50,30), text("Value 3"), channel("bV3"), range(0,1,0,1,0.001)
nslider  bounds(660,300, 50,30), text("Dur. 2"),  channel("bD3"), range(0,1024,0,1,1)
nslider  bounds(710,300, 50,30), text("Value 3"), channel("bV4"), range(0,1,0,1,0.001)
nslider  bounds(690,335, 50,30), text("Max"),     channel("bMax"), range(0,1000,0,1,0.001)
nslider  bounds(740,335, 50,30), text("Min"),     channel("bMin"), range(-1000,0,0,1,0.001)

image bounds( 10,375,780,1), shape("ellipse")

checkbox bounds( 10,350,100, 15), channel("norm"), text("Normalise")

label    bounds(150,390, 90,15) text("Synth uses:"), align("right")
combobox bounds(240,389,130,17), text("3 point envelope","4 point envelope"), channel("env"), value(1)

hslider  bounds(405,387,390,10), channel("dur"), range(0.2,10,1,0.5,0.001)
label    bounds(405,399,380,12), text("Total Duration")

keyboard bounds( 10, 420, 780, 80)
</Cabbage>
                    
<CsoundSynthesizer>

<CsOptions>   
-dm0 -n -+rtmidi=NULL -M0
</CsOptions>

<CsInstruments>

; sr set by host
ksmps         =     32
nchnls         =     2
0dbfs        =    1
massign    0,2    ; all midi plays instr 2



          opcode         tabmaxk,kk,i        ; UDO for deriving maximum value and its index from a table
itabnum       xin     
inumitems     =              ftlen(itabnum)             ; derive number of items in table
kmax          table          0,itabnum                  ; maximum value starts as first table items
kcount        =              1                          ; counter starts at 1 (we've already read item 0)
loop:                                                   ; loop 1 beginning
kval          table          kcount,itabnum             ; read value from table
if kval>=kmax then                    ; if value read from table is higher than (or equal to) current maximum...
 kmax    =    kval                    ; ...values becomes new maximum
 kndx    =    kcount                    ; index of maximum becomes the index of this value
endif                            ; end of conditional branch
              loop_lt        kcount,1,inumitems,loop    ; conditionally loop back
          xout           kmax,kndx            ; return maximium value to caller instrument
              endop


          opcode         tabmink,kk,i        ; UDO for deriving maximum value and its index from a table
itabnum       xin     
inumitems     =              ftlen(itabnum)             ; derive number of items in table
kmin          table          0,itabnum                  ; maximum value starts as first table items
kcount        =              1                          ; counter starts at 1 (we've already read item 0)
loop:                                                   ; loop 1 beginning
kval          table          kcount,itabnum             ; read value from table
if kval<kmin then                    ; if value read from table is higher than (or equal to) current maximum...
 kmin    =    kval                    ; ...values becomes new maximum
 kndx    =    kcount                    ; index of maximum becomes the index of this value
endif                            ; end of conditional branch
              loop_lt        kcount,1,inumitems,loop    ; conditionally loop back
          xout           kmin,kndx            ; return maximium value to caller instrument
              endop


opcode    GEN08_3NodeEnvelope,kkkk,kiiiiiiSSSS
 knorm, iTabX, iTabY, iTabWidth, iTabHeight, iNodeSize, ifn, STabIdent, Snode1, Snode2, Snode3    xin

 kMOUSE_X        chnget    "MOUSE_X"
 kMOUSE_Y        chnget    "MOUSE_Y"
 kMOUSE_DOWN_LEFT    chnget    "MOUSE_DOWN_LEFT"
 
 ky1    init    iTabY+iTabHeight-(iNodeSize*0.5)
 kx2    init    iTabX+(iTabWidth*0.5)-(iNodeSize*0.5)
 ky2    init    (iTabHeight*0.5)-(iNodeSize*0.5)
 ky3    init    iTabY+iTabHeight-(iNodeSize*0.5)
 kin1,kin2,kin3    init    0

 kclick        trigger    kMOUSE_DOWN_LEFT,0.5,2
 
 ; test whether click is in widget or not
 if kclick==1 then
  kin1    =    (kMOUSE_X>iTabX-(iNodeSize*0.5)            && kMOUSE_X<iTabX+(iNodeSize*0.5)           && kMOUSE_Y>iTabY+ky1-(iNodeSize*0.5) && kMOUSE_Y<iTabY+ky1+(iNodeSize*0.5) && kMOUSE_DOWN_LEFT==1) ? 1 : 0
  kin2    =    (kMOUSE_X>kx2-(iNodeSize)                  && kMOUSE_X<kx2+(iNodeSize)                 && kMOUSE_Y>iTabY+ky2-(iNodeSize*0.5) && kMOUSE_Y<iTabY+ky2+(iNodeSize*0.5) && kMOUSE_DOWN_LEFT==1) ? 1 : 0
  kin3    =    (kMOUSE_X>iTabX+iTabWidth-(iNodeSize*0.5)  && kMOUSE_X<iTabX+iTabWidth+(iNodeSize*0.5) && kMOUSE_Y>iTabY+ky3-(iNodeSize*0.5) && kMOUSE_Y<iTabY+ky3+(iNodeSize*0.5) && kMOUSE_DOWN_LEFT==1) ? 1 : 0
 endif
 
 ; click and drag
 if kin1==1 then
  ky1    limit    kMOUSE_Y - (iNodeSize*0.5), iTabY-(iNodeSize*0.5), iTabY+iTabHeight-(iNodeSize*0.5)
 endif
 if kin2==1 then
  kx2    limit    kMOUSE_X - (iNodeSize*0.5), iTabX-(iNodeSize*0.5), iTabX+iTabWidth-(iNodeSize*0.5)
  ky2    limit    kMOUSE_Y - (iNodeSize*0.5), iTabY-(iNodeSize*0.5), iTabY+iTabHeight-(iNodeSize*0.5)
 endif
 if kin3==1 then
  ky3    limit    kMOUSE_Y - (iNodeSize*0.5), iTabY-(iNodeSize*0.5), iTabY+iTabHeight-(iNodeSize*0.5)
 endif
  
 kmetro    metro    ksmps

 if kmetro==1 then                ; slow down checking
  if changed(knorm)==1 then            ; normalise check box changed
   reinit REBUILD_TABLE
  endif
  ktrig    changed    ky1                ; node 1 (beginning) moved
  if ktrig==1 then
   Smsg    sprintfk    "pos(%d,%d)", iTabX-(iNodeSize*0.5), ky1
       chnset    Smsg,Snode1
   reinit REBUILD_TABLE
  endif
  ktrig    changed    kx2,ky2                ; node 2 moved
  if ktrig==1 then
   Smsg    sprintfk    "pos(%d,%d)", kx2, ky2
       chnset    Smsg,Snode2
   reinit REBUILD_TABLE
  endif
  ktrig    changed    ky3                ; node 3 (end) moved
  if ktrig==1 then
   Smsg    sprintfk    "pos(%d,%d)", iTabX+iTabWidth-(iNodeSize*0.5), ky3
       chnset    Smsg,Snode3
   reinit REBUILD_TABLE
  endif
  
  REBUILD_TABLE:
  inorm        =    (i(knorm)*2)-1
  iy1        limit    1-((i(ky1)+(iNodeSize*0.5)-iTabY)/iTabHeight), 0, 1
  ix2        limit    ((i(kx2)+(iNodeSize*0.5)-iTabX)/iTabWidth * ftlen(ifn)),1,1022
  iy2        limit    1-((i(ky2)+(iNodeSize*0.5)-iTabY)/iTabHeight), 0, 1
  iy3        limit    1-((i(ky3)+(iNodeSize*0.5)-iTabY)/iTabHeight), 0, 1
  gitable1    ftgen    ifn,0,1024,8*inorm,    iy1, ix2, iy2, 1023-ix2, iy3
  Smsg        sprintf    "tableNumber(%d)",ifn
           chnset    Smsg,STabIdent
  rireturn
 endif

 ky1o    =    1-((ky1+iTabY-(iNodeSize*0.5))/iTabHeight)
 kx2o    =    (kx2-(iNodeSize*0.5))/iTabWidth
 ky2o    =    1-((ky2+iTabY-(iNodeSize*0.5))/iTabHeight)
 ky3o    =    1-((ky3+iTabY-(iNodeSize*0.5))/iTabHeight)
 xout    ky1o,kx2o,ky2o,ky3o

endop














opcode    GEN08_4NodeEnvelope,kkkkkk,kiiiiiiSSSSS
 knorm, iTabX, iTabY, iTabWidth, iTabHeight, iNodeSize, ifn, STabIdent, Snode1, Snode2, Snode3, Snode4    xin

 kMOUSE_X        chnget    "MOUSE_X"
 kMOUSE_Y        chnget    "MOUSE_Y"
 kMOUSE_DOWN_LEFT    chnget    "MOUSE_DOWN_LEFT"
 
 ky1    init    iTabY+iTabHeight-(iNodeSize*0.5)
 kx2    init    iTabX+(iTabWidth*0.33)-(iNodeSize*0.5)
 ky2    init    (iTabHeight*0.5)-(iNodeSize*0.5)
 kx3    init    iTabX+(iTabWidth*0.67)-(iNodeSize*0.5)
 ky3    init    (iTabHeight*0.5)-(iNodeSize*0.5)
 ky4    init    iTabY+iTabHeight-(iNodeSize*0.5)
 kin1,kin2,kin3,kin4    init    0

 kclick        trigger    kMOUSE_DOWN_LEFT,0.5,2
 
 ; test whether click is in widget or not
 if kclick==1 then
  kin1    =    (kMOUSE_X>iTabX-(iNodeSize*0.5)            && kMOUSE_X<iTabX+(iNodeSize*0.5)           && kMOUSE_Y>iTabY+ky1-(iNodeSize*0.5) && kMOUSE_Y<iTabY+ky1+(iNodeSize*0.5) && kMOUSE_DOWN_LEFT==1) ? 1 : 0
  kin2    =    (kMOUSE_X>kx2-iNodeSize                    && kMOUSE_X<kx2+iNodeSize                   && kMOUSE_Y>iTabY+ky2-(iNodeSize*0.5) && kMOUSE_Y<iTabY+ky2+(iNodeSize*0.5) && kMOUSE_DOWN_LEFT==1) ? 1 : 0
  kin3    =    (kMOUSE_X>kx3-iNodeSize                    && kMOUSE_X<kx3+iNodeSize                   && kMOUSE_Y>iTabY+ky3-(iNodeSize*0.5) && kMOUSE_Y<iTabY+ky3+(iNodeSize*0.5) && kMOUSE_DOWN_LEFT==1) ? 1 : 0
  kin4    =    (kMOUSE_X>iTabX+iTabWidth-(iNodeSize*0.5)  && kMOUSE_X<iTabX+iTabWidth+(iNodeSize*0.5) && kMOUSE_Y>iTabY+ky4-(iNodeSize*0.5) && kMOUSE_Y<iTabY+ky4+(iNodeSize*0.5) && kMOUSE_DOWN_LEFT==1) ? 1 : 0
 endif

 ; click and drag
 if kin1==1 then
  ky1    limit    kMOUSE_Y - (iNodeSize*0.5), iTabY-(iNodeSize*0.5), iTabY+iTabHeight-(iNodeSize*0.5)
 endif
 if kin2==1 then
  kx2    limit    kMOUSE_X - (iNodeSize*0.5), iTabX-(iNodeSize*0.5), kx3
  ky2    limit    kMOUSE_Y - (iNodeSize*0.5), iTabY-(iNodeSize*0.5), iTabY+iTabHeight-(iNodeSize*0.5)
 endif
 if kin3==1 then
  kx3    limit    kMOUSE_X - (iNodeSize*0.5), kx2, iTabX+iTabWidth-(iNodeSize*0.5)
  ky3    limit    kMOUSE_Y - (iNodeSize*0.5), iTabY-(iNodeSize*0.5), iTabY+iTabHeight-(iNodeSize*0.5)
 endif
 if kin4==1 then
  ky4    limit    kMOUSE_Y - (iNodeSize*0.5), iTabY-(iNodeSize*0.5), iTabY+iTabHeight-(iNodeSize*0.5)
 endif
  
 kmetro    metro    ksmps

 if kmetro==1 then                ; slow down checking
  if changed(knorm)==1 then            ; normalise check box changed
   reinit REBUILD_TABLE
  endif
  ktrig    changed    ky1                ; node 1 (beginning) moved
  if ktrig==1 then
   Smsg    sprintfk    "pos(%d,%d)", iTabX-(iNodeSize*0.5), ky1
       chnset    Smsg,Snode1
   reinit REBUILD_TABLE
  endif
  ktrig    changed    kx2,ky2                ; node 2 moved
  if ktrig==1 then
   Smsg    sprintfk    "pos(%d,%d)", kx2, ky2
       chnset    Smsg,Snode2
   reinit REBUILD_TABLE
  endif
  ktrig    changed    kx3,ky3                ; node 3 moved
  if ktrig==1 then
   Smsg    sprintfk    "pos(%d,%d)", kx3, ky3
       chnset    Smsg,Snode3
   reinit REBUILD_TABLE
  endif
  ktrig    changed    ky4                ; node 4 (end) moved
  if ktrig==1 then
   Smsg    sprintfk    "pos(%d,%d)", iTabX+iTabWidth-(iNodeSize*0.5), ky4
       chnset    Smsg,Snode4
   reinit REBUILD_TABLE
  endif
  
  REBUILD_TABLE:
  inorm        =    (i(knorm)*2)-1
  iy1        limit    1-((i(ky1)+(iNodeSize*0.5)-iTabY)/iTabHeight), 0, 1
  ix2        limit    ((i(kx2)+(iNodeSize*0.5)-iTabX)/iTabWidth * ftlen(ifn)),1,1022
  iy2        limit    1-((i(ky2)+(iNodeSize*0.5)-iTabY)/iTabHeight), 0, 1
  ix3        limit    ((i(kx3)+(iNodeSize*0.5)-iTabX)/iTabWidth * ftlen(ifn))-ix2,1,1022
  iy3        limit    1-((i(ky3)+(iNodeSize*0.5)-iTabY)/iTabHeight), 0, 1
  iy4        limit    1-((i(ky4)+(iNodeSize*0.5)-iTabY)/iTabHeight), 0, 1
  gitable1    ftgen    ifn,0,1024,8*inorm,    iy1, ix2, iy2, ix3, iy3, 1023-ix2-ix3, iy4
  Smsg        sprintf    "tableNumber(%d)",ifn
           chnset    Smsg,STabIdent
  kx2o    init    ix2/ftlen(ifn)
  kx3o    init    ix3/ftlen(ifn)
  rireturn
 endif

 ky1o    =    1-((ky1+iTabY-(iNodeSize*0.5))/iTabHeight)

 ky2o    =    1-((ky2+iTabY-(iNodeSize*0.5))/iTabHeight)
 ky3o    =    1-((ky3+iTabY-(iNodeSize*0.5))/iTabHeight)
 ky4o    =    1-((ky4+iTabY-(iNodeSize*0.5))/iTabHeight)
 xout    ky1o,kx2o,ky2o,kx3o,ky3o,ky4o
endop







instr    1    ; create envelopes and interact with Cabbage GUI (via UDOs)
 knorm            chnget    "norm"        ; normalise value (0 = don't normalise, 1 = do normalise)
 iTabX            init    10        ; table x location in pixels 
 iTabY            init    5        ; table y location in pixels
 iTabWidth        init    380        ; width of the table in pixels
 iTabHeight        init    290        ; height of the table in pixels
 iNodeSize        init    10         ; size of the breakpoint widget in pixels
 ifn            =    1        ; function table number

 gky1,gkx2,gky2,gky3    GEN08_3NodeEnvelope    knorm, iTabX,     iTabY, iTabWidth, iTabHeight, iNodeSize, ifn,   "table1", "node1", "node2", "node3"

 if metro(16)==1 then
  if    changed(gky1,gkx2,gky2,gky3,knorm)==1 then
   chnset    gky1,"aV1"
   chnset    gkx2*1024,"aD1"
   chnset    gky2,"aV2"
   chnset    1024*(1-gkx2),"aD2"
   chnset    gky3,"aV3"
   kmin,kndx    tabmink    ifn
   chnset    kmin,"aMin"
   kmax,kndx    tabmaxk    ifn
   chnset    kmax,"aMax"
  endif
 endif
 
;; optional - print out output arguments from envelope
; if changed(gky1,gkx2,gky2,gky3)==1 then
;  Senv    sprintfk    "y1:%f x2:%f y2:%f y3:%f\n",gky1,gkx2,gky2,gky3
;  printks    Senv,0
; endif

 gky1,gkx2,gky2,gkx3,gky3,gky4    GEN08_4NodeEnvelope    knorm, iTabX+400, iTabY, iTabWidth, iTabHeight, iNodeSize, ifn+1, "table2", "node4", "node5", "node6", "node7"

 if metro(16)==1 then
  if    changed(gky1,gkx2,gky2,gkx3,gky3,gky4,knorm)==1 then
   chnset    gky1,"bV1"
   chnset    gkx2*1024,"bD1"
   chnset    gky2,"bV2"
   chnset    gkx3*1024,"bD2"
   chnset    gky3,"bV3"
   chnset    (1-gkx3-gkx2)*1024,"bD3"
   chnset    gky4,"bV4"
   kmin,kndx    tabmink    ifn+1
   chnset    kmin,"bMin"
   kmax,kndx    tabmaxk    ifn+1
   chnset    kmax,"bMax"
  endif
 endif

;; optional - print out output arguments from envelope
; if changed(gky1,gkx2,gky2,gkx3,gky3,gky4)==1 then
;  Senv    sprintfk    "y1:%f x2:%f y2:%f x3:%f y3:%f y4:%f\n",gky1,gkx2,gky2,gkx3,gky3,gky4
;  printks    Senv,0
; endif
 
endin



instr    2
 idur    chnget    "dur"          ; envelope duration
 
 if timeinsts()>=idur then        ; if full duration is achieved...
  turnoff                         ; turn instrument off
 endif
 
 kphr    phasor    1/idur         ; phasor reads through once
 ienv    chnget    "env"          ; envelope type selection (1 or 2, 1 = 3_point, 2 = 4_point)
 kenv    tablei    kphr,ienv,1    ; read values from function table. Amplitude envelope.
 kenv    limit     kenv,0,1       ; limit to prevent amplitude values that might produce out of range samples
 kenv    expcurve  kenv,8         ; shape the dynamics of the envelope to be more musical
 aenv    interp    kenv           ; interpolate and create a-rate version of envelope. (Less 'zipper' noise)
 icps    cpsmidi                  ; read in midi pitch in cps
 asig    vco2      0.2,icps,4,0.5 ; create a triangle wave oscillator
 aDeClick linsegr  1,0.01,0       ; de-click envelope (if note is stopped before envelope has completed
     outs    asig*aenv*aDeClick, asig*aenv*aDeClick    ; send audio to outputs, apply amplitude envelope and de-clicking envelope
endin

</CsInstruments>

<CsScore>
i 1 0 3600
</CsScore>

</CsoundSynthesizer>