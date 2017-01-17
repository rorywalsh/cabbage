; Calculated_F_Tables.csd
; Written by Iain McCurdy, 2015

; GEN tables are calculated point-by-point within i-time loops based on a number of simple formulae relating y and x.
; The y-axis location is only a graphical indication of what x values were used in calculating y values. It does not 
; imply a shift in table index values (which are always positive).
; n.b. tan function output exceeds the range of the gentable viewer

<Cabbage>
form size(500,540), text("Calculated F-Table"), guirefresh(32)
gentable bounds(  0,  0,500,500), tablenumber(1), tablecolours("lime"), identchannel("table"), amprange(-1,1,-1), zoom(-1)
image    bounds(  0,250,500,  1), shape("sharp"), colour("white")
image    bounds(500,  0,  1,300), shape("sharp"), colour("white"), identchannel("y_axis")
combobox bounds(  5,510,120, 20), channel("formula"), text("y = x","y = x ^ 2","y = x ^ 3","y = x ^ 4","y = x ^ 5","y = x / 2","y = sin[x * pi]","y = cos[x * pi]","y = tan[x * pi]","y = sqrt[x]","y = [x / 2] + 0.5","y = -x","y = - [x ^ 2]","y = - [x ^ 3]","y = - sqrt[x] + 1","y = log[x]","y = log2[x]","y = log10[x]","y = sinh[x * pi]","y = cosh[x * pi]","y = tanh[x * pi]","y = sininv[x]","y = cosinv[x]","y = taninv[x * pi]")
label    bounds(130,513,100, 14), text("normalised"), visible(0), identchannel("normalised")
label    bounds(220,513,100, 14), text("x = 0 to 10"), visible(0), identchannel("x = 0 to 10")
numberbox bounds(340,505,60,30), text("x value"), channel("xval"), range(-10, 10,0,1,0.00001)
numberbox bounds(405,505,60,30), text("y value"), channel("yval"), range(-100,100,0,1,0.00001)
</Cabbage>

<CsoundSynthesizer>

<CsOptions>   
-n -dm0
</CsOptions>

<CsInstruments>

sr = 44100
ksmps = 16
nchnls = 1
0dbfs = 1

gi1        ftgen 1,0,2048,10,1	; table for storage of functions
giprenorm  ftgen 2,0,2048,10,1	; table for storage of functions (pre-normalisation)


	      opcode         TabNormalise,0,i		; UDO for normalising a table
itabnum       xin     
inumitems     =              ftlen(itabnum)             ; derive number of items in table
imax          table          0,itabnum                  ; maximum value starts as first table items
icount        init           1                          ; counter starts at 1 (we've already read item 0)
loop:                                                   ; loop 1 beginning
ival          table          icount,itabnum             ; read value from table
if abs(ival)>=imax then					; if absolute value read from table is higher than (or equal to) current maximum...
 imax	=	abs(ival)				; ...value becomes new maximum
 indx	=	icount					; index of maximum becomes the index of this value
endif							; end of conditional branch
              loop_lt        icount,1,inumitems,loop    ; conditionally loop back
icount        =              0	
loop2:
ival          table           icount,gi1
   	      tableiw         ival/imax, icount,itabnum
              loop_lt         icount,1,ftlen(itabnum),loop2
              endop



              
instr 1
 iftlen =       ftlen(gi1)			; length of the function table

 kformula	chnget	"formula"		; formula choice
 kformula	init	1
 
 if changed(kformula)==1 then			; if formula choice changes...
  reinit UPDATE					; start a reinitilisation
 endif

 UPDATE:
 iformula	=	i(kformula)		; i-time state of kformula
 icount		=	0			; reset counter. (Moves for each index of the function table)
 chnset	"bounds(250,  0,1,500)","y_axis"	; default set position of y-axis
 chnset	"visible(0)","normalised"		; default hide 'normalised' label
 chnset	"visible(0)","x = 0 to 10"		; default hide label
 
 if iformula==1 then					; y = x
  loop1:						; loop beginning
  ix	=	((icount/iftlen) * 2) -1		; shift x range to -1 to 1
  iy	=	ix					; apply formula to derive y
  	tableiw iy,icount,gi1				; write y value to table
  loop_lt,icount,1,iftlen,loop1				; loop back and increment counter if we are not yet finished
  
 elseif iformula==2 then				; y = x ^ 2
  loop2:						; loop beginning                                            
  ix	=	((icount/iftlen) * 2) -1                ; shift x range to -1 to 1                                  
  iy	=	ix ^ 2                                  ; apply formula to derive y                                 
  	tableiw iy,icount,gi1                           ; write y value to table                                    
  loop_lt,icount,1,iftlen,loop2                         ; loop back and increment counter if we are not yet finished
  
 elseif iformula==3 then				; y = x ^ 3
  loop3:						; loop beginning                                            
  ix	=	((icount/iftlen) * 2) -1                ; shift x range to -1 to 1                                  
  iy	=	ix ^ 3                                  ; apply formula to derive y                                 
  	tableiw iy,icount,gi1                           ; write y value to table                                    
  loop_lt,icount,1,iftlen,loop3                         ; loop back and increment counter if we are not yet finished
     
 elseif iformula==4 then				; y = x ^ 4
  loop4:						; loop beginning                                            
  ix	=	((icount/iftlen) * 2) -1                ; shift x range to -1 to 1                                  
  iy	=	ix ^ 4                                  ; apply formula to derive y                                 
  	tableiw iy,icount,gi1                           ; write y value to table                                    
  loop_lt,icount,1,iftlen,loop4                         ; loop back and increment counter if we are not yet finished

 elseif iformula==5 then				; y = x ^ 5
  loop5:						; loop beginning                                            
  ix	=	((icount/iftlen) * 2) -1                ; shift x range to -1 to 1                                  
  iy	=	ix ^ 5                                  ; apply formula to derive y                                 
  	tableiw iy,icount,gi1                           ; write y value to table                                    
  loop_lt,icount,1,iftlen,loop5                         ; loop back and increment counter if we are not yet finished

 elseif iformula==6 then				; y = x / 2
  loop6:						; loop beginning                                            
  ix	=	((icount/iftlen) * 2) -1                ; shift x range to -1 to 1                                  
  iy	=	ix / 2                                  ; apply formula to derive y                                 
  	tableiw iy,icount,gi1                           ; write y value to table                                    
  loop_lt,icount,1,iftlen,loop6                         ; loop back and increment counter if we are not yet finished

 elseif iformula==7 then				; y = sin(x  * pi)
  loop7:						; loop beginning                                            
  ix	=	((icount/iftlen) * 2) -1                ; shift x range to -1 to 1                                  
  iy	=	sin(ix*$M_PI)                           ; apply formula to derive y                               
  	tableiw iy,icount,gi1                           ; write y value to table                                    
  loop_lt,icount,1,iftlen,loop7                         ; loop back and increment counter if we are not yet finished

 elseif iformula==8 then				; y = cos(x  * pi)
  loop8:						; loop beginning                                            
  ix	=	((icount/iftlen) * 2) -1                ; shift x range to -1 to 1                                  
  iy	=	cos(ix*$M_PI)                           ; apply formula to derive y                                 
  	tableiw iy,icount,gi1                           ; write y value to table                                    
  loop_lt,icount,1,iftlen,loop8                         ; loop back and increment counter if we are not yet finished

 elseif iformula==9 then				; y = tan(x  * pi)
  loop9:						; loop beginning                                            
  ix	=	((icount/iftlen) * 2) -1                ; shift x range to -1 to 1                                  
  iy	=	tan(ix*$M_PI)                           ; apply formula to derive y                                 
  	tableiw iy,icount,gi1                           ; write y value to table                                    
  loop_lt,icount,1,iftlen,loop9                         ; loop back and increment counter if we are not yet finished

 elseif iformula==10 then				; y = sqrt(x)
  loop10:						; loop beginning                                            
  ix	=	icount/iftlen                           ; shift x range to 0 to 1                                  
  iy	=	sqrt(ix)                                ; apply formula to derive y                                 
  	tableiw iy,icount,gi1                           ; write y value to table                                    
  loop_lt,icount,1,iftlen,loop10                        ; loop back and increment counter if we are not yet finished
  chnset	"bounds(0,  0,1,500)","y_axis"		; set position of y-axis

 elseif iformula==11 then				; y = (x / 2) + 0.5
  loop11:						; loop beginning                                            
  ix	=	((icount/iftlen) * 2) -1                ; shift x range to -1 to 1                                  
  iy	=	(ix/2) + 0.5                            ; apply formula to derive y                                 
  	tableiw iy,icount,gi1                           ; write y value to table                                    
  loop_lt,icount,1,iftlen,loop11                        ; loop back and increment counter if we are not yet finished

 elseif iformula==12 then				; y = -x
  loop12:						; loop beginning                                            
  ix	=	((icount/iftlen) * 2) -1                ; shift x range to -1 to 1                                  
  iy	=	-ix                                     ; apply formula to derive y                                 
  	tableiw iy,icount,gi1                           ; write y value to table                                    
  loop_lt,icount,1,iftlen,loop12                        ; loop back and increment counter if we are not yet finished

 elseif iformula==13 then				; y = - (x ^ 2)
  loop13:						; loop beginning                                            
  ix	=	((icount/iftlen) * 2) -1                ; shift x range to -1 to 1                                  
  iy	=	- (ix ^ 2)                              ; apply formula to derive y                                 
  	tableiw iy,icount,gi1                           ; write y value to table                                    
  loop_lt,icount,1,iftlen,loop13                        ; loop back and increment counter if we are not yet finished

 elseif iformula==14 then				; y = - (x ^ 3)
  loop14:						; loop beginning                                            
  ix	=	((icount/iftlen) * 2) -1                ; shift x range to -1 to 1                                  
  iy	=	- (ix ^ 3)                              ; apply formula to derive y                                 
  	tableiw iy,icount,gi1                           ; write y value to table                                    
  loop_lt,icount,1,iftlen,loop14                        ; loop back and increment counter if we are not yet finished

 elseif iformula==15 then				; y = - sqrt(x) + 0.5
  loop15:						; loop beginning                                            
  ix	=	icount/iftlen                           ; shift x range to 0 to 1                                  
  iy	=	- sqrt(ix) + 1                          ; apply formula to derive y                                 
  	tableiw iy,icount,gi1                           ; write y value to table                                    
  loop_lt,icount,1,iftlen,loop15                        ; loop back and increment counter if we are not yet finished
  chnset	"bounds(0,  0,1,500)","y_axis"		; set position of y-axis

 elseif iformula==16 then				; y = log(x)
  loop16:						; loop beginning                                            
  ix	=	(icount/iftlen)*10                      ; shift x range to 0 to 10                                  
  iy	=	log(ix)                                 ; apply formula to derive y                                 
  	tableiw iy,icount,gi1                           ; write y value to table                                    
  loop_lt,icount,1,iftlen,loop16                        ; loop back and increment counter if we are not yet finished
  tableicopy giprenorm, gi1
	TabNormalise	gi1
  chnset	"visible(1)","normalised"		; show 'normalised' label
  chnset	"visible(1)","x = 0 to 10"		; default hide label
  chnset	"bounds(0,  0,1,500)","y_axis"		; set position of y-axis

 elseif iformula==17 then				; y = log2(x)
  loop17:						; loop beginning                                            
  ix	=	(icount/iftlen)*10                      ; shift x range to 0 to 10                                  
  iy	=	log2(ix)                                 ; apply formula to derive y                                 
  	tableiw iy,icount,gi1                           ; write y value to table                                    
  loop_lt,icount,1,iftlen,loop17                        ; loop back and increment counter if we are not yet finished
  tableicopy giprenorm, gi1
	TabNormalise	gi1
  chnset	"visible(1)","normalised"		; show 'normalised' label
  chnset	"visible(1)","x = 0 to 10"		; default hide label
  chnset	"bounds(0,  0,1,500)","y_axis"		; set position of y-axis

 elseif iformula==18 then				; y = log10(x)
  loop18:						; loop beginning                                            
  ix	=	(icount/iftlen)*10                      ; shift x range to 0 to 10                                  
  iy	=	log10(ix)                                 ; apply formula to derive y                                 
  	tableiw iy,icount,gi1                           ; write y value to table                                    
  loop_lt,icount,1,iftlen,loop18                        ; loop back and increment counter if we are not yet finished
  tableicopy giprenorm, gi1
	TabNormalise	gi1
  chnset	"visible(1)","normalised"		; show 'normalised' label
  chnset	"visible(1)","x = 0 to 10"		; default hide label
  chnset	"bounds(0,  0,1,500)","y_axis"		; set position of y-axis

 elseif iformula==19 then				; y = sinh(x * pi)
  loop19:						; loop beginning                                            
  ix	=	((icount/iftlen) * 2) -1                ; shift x range to -1 to 1                                  
  iy	=	sinh(ix*$M_PI)                          ; apply formula to derive y                                 
  	tableiw iy,icount,gi1                           ; write y value to table                                    
  loop_lt,icount,1,iftlen,loop19                        ; loop back and increment counter if we are not yet finished
  tableicopy giprenorm, gi1
	TabNormalise	gi1
  chnset	"visible(1)","normalised"		; show 'normalised' label

 elseif iformula==20 then				; y = cosh(x * pi)
  loop20:						; loop beginning                                            
  ix	=	((icount/iftlen) * 2) -1                ; shift x range to -1 to 1                                  
  iy	=	cosh(ix*$M_PI)                          ; apply formula to derive y                                 
  	tableiw iy,icount,gi1                           ; write y value to table                                    
  loop_lt,icount,1,iftlen,loop20                        ; loop back and increment counter if we are not yet finished
  tableicopy giprenorm, gi1
	TabNormalise	gi1
  chnset	"visible(1)","normalised"		; show 'normalised' label

 elseif iformula==21 then				; y = tanh(x * pi)
  loop21:						; loop beginning                                            
  ix	=	((icount/iftlen) * 2) -1                ; shift x range to -1 to 1                                  
  iy	=	tanh(ix*$M_PI)                          ; apply formula to derive y                                 
  	tableiw iy,icount,gi1                           ; write y value to table                                    
  loop_lt,icount,1,iftlen,loop21                        ; loop back and increment counter if we are not yet finished

 elseif iformula==22 then				; y = sininv(x)
  loop22:						; loop beginning                                            
  ix	=	icount/iftlen                           ; shift x range to -1 to 1                                  
  iy	=	sininv(ix)                              ; apply formula to derive y                                 
  	tableiw iy,icount,gi1                           ; write y value to table                                    
  loop_lt,icount,1,iftlen,loop22                        ; loop back and increment counter if we are not yet finished
  tableicopy giprenorm, gi1
	TabNormalise	gi1
  chnset	"visible(1)","normalised"		; show 'normalised' label
  chnset	"bounds(0,  0,1,500)","y_axis"		; set position of y-axis

 elseif iformula==23 then				; y = cosinv(x)
  loop23:						; loop beginning                                            
  ix	=	icount/iftlen                           ; shift x range to -1 to 1                                  
  iy	=	cosinv(ix)                              ; apply formula to derive y                                 
  	tableiw iy,icount,gi1                           ; write y value to table                                    
  loop_lt,icount,1,iftlen,loop23                        ; loop back and increment counter if we are not yet finished
  tableicopy giprenorm, gi1
	TabNormalise	gi1
  chnset	"visible(1)","normalised"		; show 'normalised' label
  chnset	"bounds(0,  0,1,500)","y_axis"		; set position of y-axis

 elseif iformula==24 then				; y = taninv(x * pi)
  loop24:						; loop beginning                                            
  ix	=	((icount/iftlen) * 2) -1                ; shift x range to -1 to 1                                  
  iy	=	taninv(ix*$M_PI)                          ; apply formula to derive y                                 
  	tableiw iy,icount,gi1                           ; write y value to table                                    
  loop_lt,icount,1,iftlen,loop24                        ; loop back and increment counter if we are not yet finished
  tableicopy giprenorm, gi1
	TabNormalise	gi1
  chnset	"visible(1)","normalised"		; show 'normalised' label

 endif
     
  	chnset	"tablenumber(1)","table"		; update table



 ; read y values and print to numberbox
 kMOUSE_X	chnget	"MOUSE_X"	; read mouse x position. range: 0 - panel_width(in pixels)
 kndx		=	(kMOUSE_X/500)	; range: 0 - 1
 if metro(8)==1 then			; restrict maximum rate of updates
  if changed(kndx)==1 then		; if index has change. i.e. if mouse has moved horizontally...
   if kformula==16 || kformula==17 || kformula==18 || kformula==19 || kformula==20 || kformula==22 || kformula==23 || kformula==24 then	; special table read for normalised function tables
    kyval	table	kndx,giprenorm,1   ; read value from prenormalised table
   else
    kyval	table	kndx,gi1,1	; otherwise just read from the viewed table
   endif
   if kformula==10||kformula==15||kformula==22||kformula==23 then
    kxval	=	kndx
   elseif kformula==16||kformula==17||kformula==18 then
    kxval	=	kndx*10    
   else
    kxval	=	kndx*2 - 1
   endif
   	chnset	kxval,"xval"		; write value to number box
   	chnset	kyval,"yval"		; write value to number box
  endif
 endif
 
 
endin

</CsInstruments>

<CsScore>
i 1 0 3600
</CsScore>

</CsoundSynthesizer>
