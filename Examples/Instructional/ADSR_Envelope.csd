
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; ADSR_Envelope.csd
; Written by Iain McCurdy, 2015

; Implements a drawable ADSR envelope
; Envelope code is contained within a UDO to facilitate modular transplantation and reuse.

; The UDO requires the inputs: iTabX,iTabY,iTabWidth,iTabHeight,iNodeSize,ifn,STabIdent,SA,SD,SR 
; iTabX        -    table X location in pixels (from upper panel edge)
; iTabY        -    table Y location in pixels (from left panel edge)
; iTabWidth    -    table width in pixels
; iTabHeight    -    table height in pixels
; iNodeSize    -    node widget radius in pixels
; ifn        -    function table used to store the ADSR (only used for GUI - output values used to create envelope)
; STabIdent    -    a string representing the ident channel of the gentable
; SA        -    a string representing the ident channel of the attack node
; SD        -    a string representing the ident channel of the decay node
; SR        -    a string representing the ident channel of the release node
;    (note that no sustain node is required as the sustain level is defined by the flat segemnt joining the decay and release nodes)

; The UDO outputs six values: gkAX,gkAY,gkDX,gkSY,gkRX
; gkAX        -    attack duration. A value in the range zero to one.
; gkAY        -    attack level. A value in the range zero to one.
; gkDX        -    decay duration. A value in the range zero to one.
; gkSY        -    sustain level. Also the value to which the decay segement leads and ther value from which the release segemnt begins. A value in the range zero to one.
; gkRX        -    release duration. A value in the range zero to one.
;    These values can be used by one of Csound's 'r' (release sensing) opcodes such as linsegr.
;    It is expected that the duration values will be reScaled using a duration constant.




<Cabbage>
form caption("ADSR Envelope"), size(300, 330), pluginId("AdEn"), guiRefresh(32), colour(0,0,0)

gentable bounds( 10, 10,280,190), tableNumber(1), tableColour("silver"), identChannel("table1"), ampRange(0,1,1), fill(1), zoom(-1), tableGridColour(0,0,0,0), tablebackgroundColour(20,20,20), outlineThickness(2), identChannel("table1")
image   bounds( -5, -5, 6, 6), colour("black"), identChannel("A"), shape("ellipse"), outlineThickness("1")
image   bounds( -5, -5, 6, 6), colour("black"), identChannel("D"), shape("ellipse"), outlineThickness("1")
image   bounds( -5, -5, 6, 6), colour("black"), identChannel("R"), shape("ellipse"), outlineThickness("1")
hslider bounds( 5,205,290,10), channel("dur"), range(0.5,10,4,0.5,0.001)
label bounds( 10,217,280,12), text("Scale Durations")
keyboard bounds( 10,240,280,80)
</Cabbage>
                    
<CsoundSynthesizer>

<CsOptions>   
-dm0 -n -+rtmidi=NULL -M0
</CsOptions>

<CsInstruments>

; sr set by host
ksmps         =     32    ;NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls         =     2    ;NUMBER OF CHANNELS (1=MONO)
0dbfs        =    1    ;MAXIMUM AMPLITUDE

massign    0,2

opcode    CabbageADSR,kkkkk,iiiiiiSSSS
 iTabX,iTabY,iTabWidth,iTabHeight,iNodeSize,ifn,STabIdent,SA,SD,SR    xin

 ; read in mouse activity: X, Y and left-click
 kMOUSE_X        chnget    "MOUSE_X"
 kMOUSE_Y        chnget    "MOUSE_Y"
 kMOUSE_DOWN_LEFT    chnget    "MOUSE_DOWN_LEFT"

 iFtLen            =    512    ; length of table in point (really only affects the GUI appearance of the table). Lower table sizes slightly more efficient but if table size is less than gentable size in pixels the envelope shape can appear jagged and ugly.
 
 kinA,kinD,kinS,kinR    init    0                    ; flags to indication whether mouse is 'in' a node and left-click is held
 kclick        trigger    kMOUSE_DOWN_LEFT,0.5,2                ; momentary '1' whenever mouse is left-clicked on or off

 kAX            init    iTabX-(iNodeSize*0.5)+(iTabWidth*0.1)    ; initial attack node X position in pixels from the left panel edge
 kAY            init    iTabY-(iNodeSize*0.5)            ; initial attack node Y position in pixels from the upper panel edge
 kDX            init    iTabX-(iNodeSize*0.5)+(iTabWidth*0.2)    ; initial decay node X position in pixels from the left panel edge
 kSY            init    iTabY-(iNodeSize*0.5)+(iTabHeight*0.5)    ; initial sustain level in pixels from the upper panel edge. Used to define the initial Y positions of the decay and release nodes.
 kRX            init    iTabX-(iNodeSize*0.5)+(iTabWidth*0.8)    ; initial release node X position in pixels from the left panel edge

 ; test whether mouse is in widget when clicked or not
 if kclick==1 then
  kinA    =    (kMOUSE_X>kAX-5 && kMOUSE_X<kAX+10 && kMOUSE_Y>kAY-10 && kMOUSE_Y<kAY+10 && kMOUSE_DOWN_LEFT==1) ? 1 : 0    ; attack node
  kinD    =    (kMOUSE_X>kDX-5 && kMOUSE_X<kDX+10 && kMOUSE_Y>kSY-10 && kMOUSE_Y<kSY+10 && kMOUSE_DOWN_LEFT==1) ? 1 : 0    ; decay node
  kinR    =    (kMOUSE_X>kRX-5 && kMOUSE_X<kRX+10 && kMOUSE_Y>kSY-10 && kMOUSE_Y<kSY+10 && kMOUSE_DOWN_LEFT==1) ? 1 : 0    ; release node
 endif
 
 ; click and drag
 if kinA==1 then                                            ; if 'in' attack node, click-and-drag it
  kAX    limit    kMOUSE_X - (iNodeSize*0.5), iTabX-(iNodeSize*0.5), kDX                    ; restrict its X movement to be within the gentable area and not beyond the decay node
  kAY    limit    kMOUSE_Y - (iNodeSize*0.5), iTabY-(iNodeSize*0.5), iTabY+iTabHeight-(iNodeSize*0.5)    ; restrict its Y movement to be within the gentable area
 elseif kinD==1 then                                            ; if 'in' decay node, click-and-drag it                                              
  kDX    limit    kMOUSE_X - (iNodeSize*0.5), kAX, kRX                                                    ; restrict its X movement to be not before the attack node and not beyond the release node
  kSY    limit    kMOUSE_Y - (iNodeSize*0.5), iTabY-(iNodeSize*0.5), iTabY+iTabHeight-(iNodeSize*0.5)     ; restrict its Y movement to be within the gentable area                              
 elseif kinR==1 then                                                                                    ; if 'in' release node, click-and-drag it                                              
  kRX    limit    kMOUSE_X - (iNodeSize*0.5), kDX, iTabX+iTabWidth-(iNodeSize*0.5)                        ; restrict its X movement to be not before the decay node and not beyond the area of the gentable
  kSY    limit    kMOUSE_Y - (iNodeSize*0.5), iTabY-(iNodeSize*0.5), iTabY+iTabHeight-(iNodeSize*0.5)     ; restrict its Y movement to be within the gentable area
 endif

 ; update widget position
 kmetro    metro    ksmps                ; metronome to restrict the rate of updates
 if kmetro==1 then                ; only check for changes upon metronome impulses
  ktrig    changed    kAX,kAY                ; if attack node has been moved generate a trigger
  if ktrig==1 then                ; if triggered...
   Smsg    sprintfk    "pos(%d,%d)", kAX, kAY    ; new attack node widget position
       chnset    Smsg,SA                ; send new position to attack node widget
   reinit REBUILD_TABLE                ; begin a reinitialisation to update the table
  endif
  ktrig    changed    kDX,kSY                ; if decay node has been moved generate a trigger
  if ktrig==1 then                ; if triggered...                             
   Smsg    sprintfk    "pos(%d,%d)", kDX, kSY  ; new decay node widget position             
       chnset    Smsg,SD                         ; send new position to decay node widget     
   reinit REBUILD_TABLE                         ; begin a reinitialisation to update the table
  endif
  ktrig    changed    kRX,kSY                ; if release node has been moved generate a trigger
  if ktrig==1 then                ; if triggered...                             
   Smsg    sprintfk    "pos(%d,%d)", kRX, kSY  ; new decay node widget position             
       chnset    Smsg,SR                         ; send new position to decay node widget     
   reinit REBUILD_TABLE                         ; begin a reinitialisation to update the table
  endif

  REBUILD_TABLE:
  iAX        limit    round(((i(kAX)+(iNodeSize*0.5)-iTabX)/iTabWidth * iFtLen)),1,iFtLen-1    ; attack breakpoint X position
  iAY        limit    1-((i(kAY)+(iNodeSize*0.5)-iTabY)/iTabHeight), 0, 1            ; attack breakpoint value
  iDX        limit    round(((i(kDX)+(iNodeSize*0.5)-iTabX)/iTabWidth * iFtLen)),1,iFtLen-1    ; decay breakpoint X position
  iSY        limit    1-((i(kSY)+(iNodeSize*0.5)-iTabY)/iTabHeight), 0, 1                 ; sustain value     
  iRX        limit    round(((i(kRX)+(iNodeSize*0.5)-iTabX)/iTabWidth * iFtLen)),1,iFtLen-1   ; release breakpoint value
  
  gitable1    ftgen    ifn,0,iFtLen,-27,    0,0, iAX,iAY, iDX,iSY, iRX,iSY, iFtLen-1,0    ; create the table
  Smsg        sprintf    "tableNumber(%d)",ifn                            ; create the 'update table' message
           chnset    Smsg,STabIdent                                ; send 'update table' message to gentable
  rireturn
 endif

 kAXo    =    (kAX-iTabX+(iNodeSize*0.5))/iTabWidth            ; output attack time
 kAYo    =    1-((kAY-iTabY+(iNodeSize*0.5))/iTabHeight)        ; output attack level
 kDXo    =    ((kDX-iTabX+(iNodeSize*0.5))/iTabWidth)-kAXo        ; output decay  time
 kSYo    =    1-((kSY-iTabY+(iNodeSize*0.5))/iTabHeight)        ; output sustain level
 kRXo    =    1-(kRX-iTabX+(iNodeSize*0.5))/iTabWidth            ; output release time

 xout    kAXo,kAYo,kDXo,kSYo,kRXo                    ; send values back to caller instrument
endop


instr    1    ; create the envelope
 iTabX            init    10        ; table x location in pixels 
 iTabY            init    10        ; table y location in pixels
 iTabWidth        init    280        ; width of the table in pixels
 iTabHeight        init    190        ; height of the table in pixels
 iNodeSize        init    6         ; size of the breakpoint widget in pixels
 ifn            =    1        ; table number
 STabIdent        =    "table1"    ; table ident channel
 SA            =    "A"        ; attack node ident channel
 SD            =    "D"        ; decay node ident channel
 SR            =    "R"        ; release node ident channel

 ; call the UDO. Output values should be global variables so that they can be used in instr 2
 gkAX,gkAY,gkDX,gkDY,gkRX    CabbageADSR    iTabX,iTabY,iTabWidth,iTabHeight,iNodeSize,ifn,STabIdent,SA,SD,SR
endin


instr    2    ; MIDI triggered instrument. Reads envelope created in instr 1
 icps    cpsmidi                                            ; read in midi pitch in cps
 idur    chnget    "dur"                                        ; scale durations
 kenv    linsegr    0,(i(gkAX)*idur)+0.0001,i(gkAY),(i(gkDX)*idur)+0.0001,i(gkDY),(i(gkRX)*idur),0    ; create envelope
 kenv    expcurve    kenv,8                                    ; shape envelope values to be more musical as amplitudes
 aenv    interp        kenv                                    ; interpolate to create a-rate version of envelope
 asig    vco2    0.2,icps,4,0.5                                    ; create a triangle wave oscillator
     outs    asig*aenv,asig*aenv                                ; send audio to outputs and apply amplitude envelope
endin

</CsInstruments>

<CsScore>
i 1 0 3600    ; instr 1 creates envelope and interacts with Cabbage GUI
</CsScore>

</CsoundSynthesizer>
