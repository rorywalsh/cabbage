
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; Calculated_F_Tables.csd
; Written by Iain McCurdy, 2015

; GEN tables are calculated point-by-point within i-time loops based on a number of simple formulae relating y and x.
; The y-axis location is only a graphical indication of what x values were used in calculating y values. It does not 
; imply a shift in table index values (which are always positive).
; n.b. tan function output exceeds the range of the gentable viewer


; Some of these functions might be useful for control mapping or waveshaping.

<Cabbage>
form size(500,570), caption("Calculated F-Table"), guiRefresh(32), colour(80,80,80), pluginId("CaFT")
gentable bounds(  0,  0,500,500), tableNumber(1), tableColour("Orange"), identChannel("table"), ampRange(-1,1,-1), zoom(-1), fill(0)
image    bounds(  0,250,500,  1), shape("sharp"), colour("white")
image    bounds(500,  0,  1,300), shape("sharp"), colour("white"), identChannel("y_axis")
combobox bounds(  5,505,120, 20), channel("formula"), text("y = x","y = -x","y = x / a","y = x * a","y = x ^ a","y = - [x ^ a]","y = x ^ [1/a]","y = - x ^ [1/a] + 1","y = sin[x * pi]","y = cos[x * pi]","y = tan[x * pi]","y = log[x]","y = log2[x]","y = log10[x]","y = sinh[x * a]","y = cosh[x * a]","y = tanh[x * pi * a]","y = sininv[x]","y = cosinv[x]","y = taninv[x * a]","y = a ^ x","y = [a ^ x] - 1","y = [a ^ -x] - 1"), value(1)

image  bounds( 5,525, 66, 40) plant("a") identChannel("aID") colour(0,0,0,0)
{
nslider bounds( 0,  0, 40, 30), text("a ="), channel("a"), range(1,100,2,1,1) textColour("white")
button    bounds(41,  8, 20, 14), text("^","^") channel("aIncr") latched(0)
button    bounds(41,  8, 20, 14), text("^","^") channel("aDecr") latched(0), rotate(3.147,10,15)
}

label    bounds(130,513,100, 14), text("normalised"), visible(0), identChannel("normalised")
label    bounds(220,513,100, 14), text("x = 0 to 10"), visible(0), identChannel("x = 0 to 10")
nslider bounds(340,505,70,30), text("x value"), channel("xval"), range(-10, 10,0,1,0.00001)
nslider bounds(415,505,70,30), text("y value"), channel("yval"), range(-100,100,0,1,0.00001)
</Cabbage>

<CsoundSynthesizer>

<CsOptions>   
-n -dm0
</CsOptions>

<CsInstruments>

; sr set by host
ksmps = 16
nchnls = 1
0dbfs = 1

gi1        ftgen 1,0,2048,10,1    ; table for storage of functions
giprenorm  ftgen 2,0,2048,10,1    ; table for storage of functions (pre-normalisation)


          opcode         TabNormalise,0,i        ; UDO for normalising a table
itabnum       xin     
inumitems     =              ftlen(itabnum)             ; derive number of items in table
imax          table          0,itabnum                  ; maximum value starts as first table items
icount        init           1                          ; counter starts at 1 (we've already read item 0)
loop:                                                   ; loop 1 beginning
ival          table          icount,itabnum             ; read value from table
if abs(ival)>=imax then                    ; if absolute value read from table is higher than (or equal to) current maximum...
 imax    =    abs(ival)                ; ...value becomes new maximum
 indx    =    icount                    ; index of maximum becomes the index of this value
endif                            ; end of conditional branch
              loop_lt        icount,1,inumitems,loop    ; conditionally loop back
icount        =              0    
loop2:
ival          table           icount,gi1
             tableiw         ival/imax, icount,itabnum
              loop_lt         icount,1,ftlen(itabnum),loop2
              endop



              
instr 1
 iftlen =       ftlen(gi1)            ; length of the function table

 kformula    chnget    "formula"        ; formula choice
 kformula    init    1
 ka            chnget    "a"
 
 if trigger:k(chnget:k("aIncr"),0.5,0)==1 then
  chnset    limit:k(ka+1,1,100),"a"
 elseif trigger:k(chnget:k("aDecr"),0.5,0)==1 then
  chnset    limit:k(ka-1,1,100),"a"
 endif
 
 
 if changed(kformula)==1 then            ; if formula choice changes...
  reinit UPDATE                    ; start a reinitilisation
 endif

 UPDATE:
 iformula    =    i(kformula)        ; i-time state of kformula
 icount        =    0            ; reset counter. (Moves for each index of the function table)
 chnset    "bounds(250,  0,1,500)","y_axis"    ; default set position of y-axis
 chnset    "visible(0)","normalised"        ; default hide 'normalised' label
 chnset    "visible(0)","x = 0 to 10"        ; default hide label

 if iformula==1 then                    ; y = x
  while icount<iftlen do                ; set up a while loop
  kNorm        init    0    ; Normalised/unnormalised
  kXScale    init    0    ; x axis scaling
  ix    =    ((icount/iftlen) * 2) -1        ; shift x range to -1 to 1
  iy    =    ix                    ; apply formula to derive y
      tableiw iy,icount,gi1                ; write y value to table
  icount    +=    1                       ; increment counter
  od                                    ; end of while loop
    chnset    "visible(0)","aID"        ; show/hide a widgets
  
 elseif iformula==2 then                ; y = -x
  while icount<iftlen do                ; set up a while loop
  kNorm        init    0    ; Normalised/unnormalised
  kXScale    init    0    ; x axis scaling
  ix    =    ((icount/iftlen) * 2) -1                ; shift x range to -1 to 1                                  
  iy    =    -ix                                     ; apply formula to derive y                                 
      tableiw iy,icount,gi1                           ; write y value to table                                    
  icount    +=    1                       ; increment counter
  od                                    ; end of while loop
  chnset    "visible(0)","aID"        ; show/hide a widgets

 elseif iformula==3 then                ; y = x / a
  if changed:k(ka)==1 then
   reinit RestartForm3
  endif
  RestartForm3:
  icount    =    0
  while icount<iftlen do                ; set up a while loop
  kNorm        init    0                    ; Normalised/unnormalised
  kXScale    init    0                    ; x axis scaling
  ix    =    ((icount/iftlen) * 2) -1    ; shift x range to -1 to 1                                  
  iy    =    ix / i(ka)                  ; apply formula to derive y                                 
      tableiw iy,icount,gi1               ; write y value to table                                    
  icount    +=    1                       ; increment counter
  od                                    ; end of while loop
  chnset    "visible(1)","aID"            ; show/hide a widgets

 elseif iformula==4 then                ; y = x * a
  if changed:k(ka)==1 then
   reinit RestartForm4
  endif
  RestartForm4:
  icount    =    0
  while icount<iftlen do                ; set up a while loop
  kNorm        init    0                    ; Normalised/unnormalised
  kXScale    init    0                    ; x axis scaling
  ix    =    ((icount/iftlen) * 2) -1    ; shift x range to -1 to 1                                  
  iy    =    ix * i(ka)                  ; apply formula to derive y                                 
      tableiw iy,icount,gi1               ; write y value to table                                    
  icount    +=    1                       ; increment counter
  od                                    ; end of while loop
  chnset    "visible(1)","aID"            ; show/hide a widgets

  elseif iformula==5 then                ; y = x ^ a
  if changed:k(ka)==1 then
   reinit RestartForm5
  endif
  RestartForm5:
  icount    =    0
  while icount<iftlen do                            ; set up a while loop
  kNorm        init    0                                ; Normalised/unnormalised
  kXScale    init    0                                ; x axis scaling
  ix    =    ((icount/iftlen) * 2) -1                ; shift x range to -1 to 1                                  
  iy    =    ix ^ i(ka)                              ; apply formula to derive y                                 
      tableiw iy,icount,gi1                           ; write y value to table                                    
  icount    +=    1                                   ; increment counter
  od                                                ; end of while loop
  chnset    "visible(1)","aID"                        ; show/hide a widgets

 elseif iformula==6 then                            ; y = - (x ^ a)
  if changed:k(ka)==1 then
   reinit RestartForm6
  endif
  RestartForm6:
  icount    =    0
  while icount<iftlen do                            ; set up a while loop
  kNorm        init    0                                ; Normalised/unnormalised
  kXScale    init    0                                ; x axis scaling
  ix    =    ((icount/iftlen) * 2) -1                ; shift x range to -1 to 1                                  
  iy    =    - (ix ^ i(ka))                          ; apply formula to derive y                                 
      tableiw iy,icount,gi1                           ; write y value to table                                    
  icount    +=    1                                   ; increment counter
  od                                                ; end of while loop
  chnset    "visible(1)","aID"                        ; show/hide a widgets

  elseif iformula==7 then                            ; y = x ^ (1/a)
  if changed:k(ka)==1 then
   reinit RestartForm7
  endif
  RestartForm7:
  icount    =    0
  while icount<iftlen do                            ; set up a while loop
  kNorm        init    0                                ; Normalised/unnormalised
  kXScale    init    1                                ; x axis scaling
  ix    =    icount/iftlen                           ; shift x range to 0 to 1                                  
  iy    =    pow:i(ix,1/i(ka))                       ; apply formula to derive y                                 
      tableiw iy,icount,gi1                           ; write y value to table                                    
  icount    +=    1                                   ; increment counter
  od                                                ; end of while loop
  chnset    "bounds(0,  0,1,500)","y_axis"            ; set position of y-axis
  chnset    "visible(1)","aID"                        ; show/hide a widgets

 elseif iformula==8 then                            ; y = - x ^ (1/a) + 1
  if changed:k(ka)==1 then
   reinit RestartForm8
  endif
  RestartForm8:
  icount    =    0
  kNorm        init    0                                ; Normalised/unnormalised
  kXScale    init    1                                ; x axis scaling
  while icount<iftlen do                            ; set up a while loop
  ix    =    icount/iftlen                           ; shift x range to 0 to 1                                  
  iy    =    - pow:i(ix,1/i(ka)) + 1                 ; apply formula to derive y                                 
      tableiw iy,icount,gi1                           ; write y value to table                                    
  icount    +=    1                                   ; increment counter
  od                                                ; end of while loop
  chnset    "bounds(0,  0,1,500)","y_axis"            ; set position of y-axis
  chnset    "visible(1)","aID"                        ; show/hide a widgets

 elseif iformula==9 then                            ; y = sin(x  * pi)
  while icount<iftlen do                            ; set up a while loop
  kNorm        init    0                                ; Normalised/unnormalised
  kXScale    init    0                                ; x axis scaling
  ix    =    ((icount/iftlen) * 2) -1                ; shift x range to -1 to 1                                  
  iy    =    sin(ix*$M_PI)                           ; apply formula to derive y                               
      tableiw iy,icount,gi1                           ; write y value to table                                    
  icount    +=    1                                   ; increment counter
  od                                                ; end of while loop
  chnset    "visible(0)","aID"                        ; show/hide a widgets

 elseif iformula==10 then                            ; y = cos(x  * pi)
  while icount<iftlen do                            ; set up a while loop
   kNorm    init    0                                ; Normalised/unnormalised
  kXScale    init    0                                ; x axis scaling
 ix    =    ((icount/iftlen) * 2) -1                    ; shift x range to -1 to 1                                  
  iy    =    cos(ix*$M_PI)                           ; apply formula to derive y                                 
      tableiw iy,icount,gi1                           ; write y value to table                                    
  icount    +=    1                                   ; increment counter
  od                                                ; end of while loop
  chnset    "visible(0)","aID"                        ; show/hide a widgets

 elseif iformula==11 then                ; y = tan(x  * pi)
  while icount<iftlen do                ; set up a while loop
  kNorm        init    0    ; Normalised/unnormalised
  kXScale    init    0    ; x axis scaling
  ix    =    ((icount/iftlen) * 2) -1                ; shift x range to -1 to 1                                  
  iy    =    tan(ix*$M_PI)                           ; apply formula to derive y                                 
      tableiw iy,icount,gi1                           ; write y value to table                                    
  icount    +=    1                       ; increment counter
  od                                    ; end of while loop
  chnset    "visible(0)","aID"        ; show/hide a widgets

 elseif iformula==12 then                ; y = log(x)
  while icount<iftlen do                ; set up a while loop
  kNorm        init    1    ; Normalised/unnormalised
  kXScale    init    10    ; x axis scaling
  ix    =    (icount/iftlen)*10                      ; shift x range to 0 to 10                                  
  iy    =    log(ix)                                 ; apply formula to derive y                                 
      tableiw iy,icount,gi1                           ; write y value to table                                    
  icount    +=    1                       ; increment counter
  od                                    ; end of while loop
  tableicopy giprenorm, gi1
    TabNormalise    gi1
  chnset    "visible(1)","normalised"        ; show 'normalised' label
  chnset    "visible(1)","x = 0 to 10"        ; default hide label
  chnset    "bounds(0,  0,1,500)","y_axis"        ; set position of y-axis
  chnset    "visible(0)","aID"        ; show/hide a widgets

 elseif iformula==13 then                ; y = log2(x)
  while icount<iftlen do                ; set up a while loop
  kNorm        init    1    ; Normalised/unnormalised
  kXScale    init    10    ; x axis scaling
  ix    =    (icount/iftlen)*10                      ; shift x range to 0 to 10                                  
  iy    =    log2(ix)                                 ; apply formula to derive y                                 
      tableiw iy,icount,gi1                           ; write y value to table                                    
  icount    +=    1                       ; increment counter
  od                                    ; end of while loop
  tableicopy giprenorm, gi1
    TabNormalise    gi1
  chnset    "visible(1)","normalised"        ; show 'normalised' label
  chnset    "visible(1)","x = 0 to 10"        ; default hide label
  chnset    "bounds(0,  0,1,500)","y_axis"        ; set position of y-axis
  chnset    "visible(0)","aID"        ; show/hide a widgets

 elseif iformula==14 then                    ; y = log10(x)
  while icount<iftlen do                    ; set up a while loop
  kNorm        init    1                        ; Normalised/unnormalised
  kXScale    init    10                        ; x axis scaling
  ix    =    (icount/iftlen)*10              ; shift x range to 0 to 10                                  
  iy    =    log10(ix)                       ; apply formula to derive y                                 
      tableiw iy,icount,gi1                   ; write y value to table                                    
  icount    +=    1                           ; increment counter
  od                                        ; end of while loop
  tableicopy giprenorm, gi1
    TabNormalise    gi1
  chnset    "visible(1)","normalised"        ; show 'normalised' label
  chnset    "visible(1)","x = 0 to 10"        ; default hide label
  chnset    "bounds(0,  0,1,500)","y_axis"    ; set position of y-axis
  chnset    "visible(0)","aID"                ; show/hide a widgets

 elseif iformula==15 then                    ; y = sinh(x * a)
  if changed:k(ka)==1 then
   reinit RestartForm16
  endif
  RestartForm16:
  icount    =    0
  while icount<iftlen do                ; set up a while loop
  kNorm        init    1                    ; Normalised/unnormalised
  kXScale    init    0                    ; x axis scaling
  ix    =    ((icount/iftlen) * 2) -1    ; shift x range to -1 to 1                                  
  iy    =    sinh(ix*i(ka))              ; apply formula to derive y                                 
      tableiw iy,icount,gi1               ; write y value to table                                    
  icount    +=    1                       ; increment counter
  od                                    ; end of while loop
  tableicopy giprenorm, gi1
    TabNormalise    gi1
  chnset    "visible(1)","normalised"    ; show 'normalised' label
  chnset    "visible(1)","aID"            ; show/hide a widgets

 elseif iformula==16 then                ; y = cosh(x * a)
  if changed:k(ka)==1 then
   reinit RestartForm16
  endif
  RestartForm16:
  icount    =    0
  while icount<iftlen do                ; set up a while loop
  kNorm        init    1                    ; Normalised/unnormalised
  kXScale    init    0                    ; x axis scaling
  ix    =    ((icount/iftlen) * 2) -1    ; shift x range to -1 to 1                                  
  iy    =    cosh(ix*i(ka))              ; apply formula to derive y                                 
      tableiw iy,icount,gi1               ; write y value to table                                    
  icount    +=    1                       ; increment counter
  od                                    ; end of while loop
  tableicopy giprenorm, gi1
    TabNormalise    gi1
  chnset    "visible(1)","normalised"    ; show 'normalised' label
  chnset    "visible(1)","aID"            ; show/hide a widgets

 elseif iformula==17 then                ; y = tanh(x * pi * a)
  if changed:k(ka)==1 then
   reinit RestartForm17
  endif
  RestartForm17:
  icount    =    0
  while icount<iftlen do                ; set up a while loop
  kNorm        init    0                    ; Normalised/unnormalised
  kXScale    init    0                    ; x axis scaling
  ix    =    ((icount/iftlen) * 2) -1                ; shift x range to -1 to 1                                  
  iy    =    tanh(ix*$M_PI*i(ka))                    ; apply formula to derive y                                 
      tableiw iy,icount,gi1                           ; write y value to table                                    
  icount    +=    1                                   ; increment counter
  od                                                ; end of while loop
  chnset    "visible(1)","aID"                        ; show/hide a widgets

 elseif iformula==18 then                            ; y = sininv(x)
  while icount<iftlen do                            ; set up a while loop
  kNorm        init    1                                ; Normalised/unnormalised
  kXScale    init    1                                ; x axis scaling
  ix    =    icount/iftlen                           ; shift x range to -1 to 1                                  
  iy    =    sininv(ix)                              ; apply formula to derive y                                 
      tableiw iy,icount,gi1                           ; write y value to table                                    
  icount    +=    1                                   ; increment counter
  od                                                ; end of while loop
  tableicopy giprenorm, gi1
    TabNormalise    gi1
  chnset    "visible(1)","normalised"                ; show 'normalised' label
  chnset    "bounds(0,  0,1,500)","y_axis"            ; set position of y-axis
  chnset    "visible(0)","aID"                        ; show/hide a widgets

 elseif iformula==19 then                            ; y = cosinv(x)
  while icount<iftlen do                            ; set up a while loop
  kNorm        init    1                                ; Normalised/unnormalised
  kXScale    init    1                                ; x axis scaling
  ix    =    icount/iftlen                           ; ix will be evenly spaced fractions from 0 to 1                                  
  iy    =    cosinv(ix)                              ; apply formula to derive y                                 
      tableiw iy,icount,gi1                           ; write y value to table                                    
  icount    +=    1                                   ; increment counter
  od                                                ; end of while loop
  tableicopy giprenorm, gi1
    TabNormalise    gi1
  chnset    "visible(1)","normalised"                ; show 'normalised' label
  chnset    "bounds(0,  0,1,500)","y_axis"            ; set position of y-axis
  chnset    "visible(0)","aID"                        ; show/hide a widgets

 elseif iformula==20 then                            ; y = taninv(x * a)
  if changed:k(ka)==1 then
   reinit RestartForm20
  endif
  RestartForm20:
  icount    =    0
  while icount<iftlen do                            ; set up a while loop
  kNorm        init    1                                ; Normalised/unnormalised
  kXScale    init    0                                ; x axis scaling
  ix    =    ((icount/iftlen) * 2) -1                ; shift x range to -1 to 1                                  
  iy    =    taninv(ix*i(ka))                        ; apply formula to derive y                                 
      tableiw iy,icount,gi1                           ; write y value to table                                    
  icount    +=    1                                   ; increment counter
  od                                                ; end of while loop
  tableicopy giprenorm, gi1
    TabNormalise    gi1
  chnset    "visible(1)","normalised"        ; show 'normalised' label
  chnset    "visible(1)","aID"        ; show/hide a widgets

 elseif iformula==21 then                ; a ^ x
  imax    init    0                        ; initialise maximum y value
  if changed:k(ka)==1 then
   reinit RestartForm21
  endif
  RestartForm21:
  icount    =    0
  while icount<iftlen do                ; set up a while loop
  ix    =    ((icount/iftlen) * 2) -1    ; shift x range to -1 to 1                                  
  iy    =    i(ka) ^ ix                  ; apply formula to derive y
  if abs(iy)>imax then                    ; if current y value is a new (absolute) maximum
   imax    =    iy                            ; overwrite maximum
  endif
      tableiw iy,icount,gi1               ; write y value to table                                    
  icount    +=    1                       ; increment counter
  od                                    ; end of while loop
  tableicopy giprenorm, gi1
    TabNormalise    gi1
  chnset    "visible(1)","normalised"                ; show 'normalised' label
  kNorm        init    1
  kXScale    init    0
  chnset    "visible(1)","aID"        ; show/hide a widgets

 elseif iformula==22 then                ; y = (a ^ x) -1
  if changed:k(ka)==1 then
   reinit RestartForm22
  endif
  RestartForm22:
  icount    =    0
  imax    init    0                        ; initialise maximum y value
  while icount<iftlen do                ; set up a while loop
  ix    =    ((icount/iftlen) * 2) -1                ; shift x range to -1 to 1                                  
  iy    =    (i(ka) ^ ix) - 1                                  ; apply formula to derive y
  if abs(iy)>imax then                                ; if current y value is a new (absolute) maximum
   imax    =    iy                                        ; overwrite maximum
  endif
      tableiw iy,icount,gi1                           ; write y value to table                                    
  icount    +=    1                       ; increment counter
  od                                    ; end of while loop
  tableicopy giprenorm, gi1
    TabNormalise    gi1
  chnset    "visible(1)","normalised"                ; show 'normalised' label
  kNorm        init    1
  kXScale    init    0
  chnset    "visible(1)","aID"        ; show/hide a widgets

 elseif iformula==23 then                ; y = (a ^ -x) - 1
  if changed:k(ka)==1 then
   reinit RestartForm23
  endif
  RestartForm23:
  icount    =    0
  imax    init    0                        ; initialise maximum y value
  while icount<iftlen do                ; set up a while loop
  ix    =    ((icount/iftlen) * 2) -1                ; shift x range to -1 to 1                                  
  iy    =    (i(ka) ^ (-ix))-1                                  ; apply formula to derive y
  if abs(iy)>imax then                                ; if current y value is a new (absolute) maximum
   imax    =    iy                                        ; overwrite maximum
  endif
      tableiw iy,icount,gi1                           ; write y value to table                                    
  icount    +=    1                       ; increment counter
  od                                    ; end of while loop
  tableicopy giprenorm, gi1
    TabNormalise    gi1
  chnset    "visible(1)","normalised"                ; show 'normalised' label
  kNorm        init    1
  kXScale    init    0
  chnset    "visible(1)","aID"        ; show/hide a widgets

 endif


  chnset    "tableNumber(1)","table"        ; update table

 ; read y values and print to nslider
 kMOUSE_X    chnget    "MOUSE_X"    ; read mouse x position. range: 0 - panel_width(in pixels)
 kndx        =    (kMOUSE_X/500)    ; range: 0 - 1
 if metro(8)==1 then            ; restrict maximum rate of updates
  if changed(kndx)==1 then        ; if index has change. i.e. if mouse has moved horizontally...

   if kNorm    =    1 then            ; normalised to fit values vertically, therefore read actual value from an unnormalised version
    kyval    table    kndx,giprenorm,1   ; read value from prenormalised table
   else
    kyval    table    kndx,gi1,1    ; otherwise just read from the viewed table
   endif
   
   if kXScale==1 then        ; range 0 to 1
    kxval    =    kndx
   elseif kXScale==10 then    ; range 0 to 10
    kxval    =    kndx*10    
   elseif kXScale==0 then    ; range -1 to 1
    kxval    =    kndx*2 - 1
   endif
   
   
       chnset    kxval, "xval"                ; write value to number box
       chnset    kyval, "yval"        ; write value to number box
  endif
 endif
 
 
endin

</CsInstruments>

<CsScore>
i 1 0 3600
</CsScore>

</CsoundSynthesizer>
