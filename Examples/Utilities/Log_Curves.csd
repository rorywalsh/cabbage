
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; Log_Curves.csd
; Iain McCurdy, 2015

<Cabbage>
form size(1015,510), caption("Log Curves"), pluginId("LgCu"), colour("Black")
gentable bounds( 15,  5,990,500), tableNumber(1,2,3), tableColour("red","green","blue"), , tableColour:0("red") tableColour:1("green") tableColour:2("silver"), identChannel("table"), ampRange(-5,5,-1), zoom(-1), fill(0), outlineThickness(2)
label    bounds( 22,  6, 80, 15), align("left"), text("1. log"),   fontColour("red")
label    bounds( 22, 22, 80, 15), align("left"), text("2. log2"),  fontColour("green")
label    bounds( 22, 38, 80, 15), align("left"), text("3. log10"), fontColour("silver")
label    bounds(  0,  0, 14, 12), text("5"), fontColour("white")
label    bounds(  0, 50, 14, 12), text("4"),  fontColour("white")
label    bounds(  0,100, 14, 12), text("3"),  fontColour("white")
label    bounds(  0,150, 14, 12), text("2"),  fontColour("white")
label    bounds(  0,200, 14, 12), text("1"),  fontColour("white")

label    bounds(   0,300, 14, 12), text("-1"),  fontColour("white")
label    bounds(   0,350, 14, 12), text("-2"),  fontColour("white")
label    bounds(   0,400, 14, 12), text("-3"),  fontColour("white")
label    bounds(   0,450, 14, 12), text("-4"),  fontColour("white")
label    bounds(   0,500, 14, 12), text("-5"),  fontColour("white")
label    bounds( 108,258, 14, 12), text("1"),  fontColour("white")
label    bounds( 207,258, 14, 12), text("2"),  fontColour("white")
label    bounds( 306,258, 14, 12), text("3"),  fontColour("white")
label    bounds( 405,258, 14, 12), text("4"),  fontColour("white")
label    bounds( 504,258, 14, 12), text("5"),  fontColour("white")
label    bounds( 603,258, 14, 12), text("6"),  fontColour("white")
label    bounds( 702,258, 14, 12), text("7"),  fontColour("white")
label    bounds( 801,258, 14, 12), text("8"),  fontColour("white")
label    bounds( 900,258, 14, 12), text("9"),  fontColour("white")
label    bounds( 999,258, 15, 12), text("10"),  fontColour("white")

image    bounds(436,461,200, 40), shape("sharp"), colour(100,100,100), outlineColour("white"), outlineThickness(2), plant("values") {
combobox bounds(  5, 10, 65, 20), text("log","log2","log10"), channel("function"), value(1)
nslider bounds( 75, 5, 60, 30), text("Input"),  channel("input"), range(0,1000.000,0,1,0.001), textColour("black")
nslider bounds(135, 5, 60, 30), text("Output"), channel("output"), range(0,1000.000,0,1,0.001), textColour("black") 
}

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
gi2        ftgen 2,0,2048,10,1    ; table for storage of functions
gi3        ftgen 3,0,2048,10,1    ; table for storage of functions

              
instr 1
 iftlen =       ftlen(gi1)            ; length of the function table

 iscale    =    10

 icount        =    0                    ; reset counter. (Moves for each index of the function table)
  loop1:                        ; loop beginning                                            
  ix    =    (icount/iftlen) * iscale                ; shift x range to -1 to 1                                  
  iy    limit    log(ix),-5,5                        ; apply formula to derive y                                 
      tableiw iy,icount,gi1                           ; write y value to table                                    
  loop_lt,icount,1,iftlen,loop1                         ; loop back and increment counter if we are not yet finished
      chnset    "tableNumber(1)","table"        ; update table
 
 icount        =    0                    ; reset counter. (Moves for each index of the function table)
  loop2:                        ; loop beginning                                            
  ix    =    (icount/iftlen) * iscale                ; shift x range to -1 to 1                                  
  iy    limit    log2(ix),-5,5                                ; apply formula to derive y                                 
      tableiw iy,icount,gi2                           ; write y value to table                                    
  loop_lt,icount,1,iftlen,loop2                         ; loop back and increment counter if we are not yet finished
      chnset    "tableNumber(2)","table"        ; update table
 
 icount        =    0                    ; reset counter. (Moves for each index of the function table)
  loop3:                        ; loop beginning                                            
  ix    =    (icount/iftlen) * iscale                ; shift x range to -1 to 1                                  
  iy    limit    log10(ix),-5,5                               ; apply formula to derive y                                 
  ;iy    log    ix, 10                                  ; apply formula to derive y                                 
      tableiw iy,icount,gi3                           ; write y value to table                                    
  loop_lt,icount,1,iftlen,loop3                         ; loop back and increment counter if we are not yet finished
      chnset    "tableNumber(3)","table"        ; update table


 kfunction    chnget    "function"                    ; function log=1 log2=2 log10=3
 kinput        chnget    "input"                ; input value for conversion
 if changed(kinput)==1 || changed(kfunction)==1 then    ; if input value or function type are changed... 
  if kfunction==1 then                    ; if log (natural)...
   koutput    =    log(kinput)            ; derive output value
           chnset    koutput,"output"        ; send to widget
  elseif kfunction==2 then                ; if log2...
   koutput    =    log2(kinput)            ; derive output value
           chnset    koutput,"output"                ; send to widget     
  elseif kfunction==3 then                ; if log10...
   koutput    =    log10(kinput)            ; derive output value
           chnset    koutput,"output"                ; send to widget     
  endif
 endif

endin

</CsInstruments>

<CsScore>
i 1 0 3600
</CsScore>

</CsoundSynthesizer>
