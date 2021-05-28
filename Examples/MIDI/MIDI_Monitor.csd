
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; MidiMonitor.csd
; Written by Iain McCurdy, 2014

; display of MIDI notes, key velocity and controllers (MIDI channel 1)


<Cabbage>
form caption("Midi Monitor"), size(610, 425), colour( 50, 50, 50),pluginId("MiMo"), guiRefresh(16)



; NOTES AND VELOCITIES
image     bounds(  0,  0, 610,110), colour(75, 85, 90, 100), line(2), shape(sharp)
gentable     bounds(  5,  5, 600, 74),  tableNumber(1), tableColour("yellow"), identChannel("table1"), ampRange(0,127,1), zoom(-1), tableGridColour(0,0,0,0)
label     bounds(  5, 83,600, 12), text("Notes and Velocities"), fontColour("white")
label     bounds(  5, 82, 70, 12), text("Last Note:"), fontColour("white")
nslider bounds( 75, 80, 35, 16), fontColour("white"), channel("LastNote"), range(0,127,0,1,1)
label     bounds(125, 82, 80, 12), text("Last Velocity:"), fontColour("white")
nslider bounds(205, 80, 35, 16), fontColour("white"), channel("LastVel"), range(0,127,0,1,1)

; ALL CONTROLLERS ON ONE CHANNEL
image    bounds(  0,115,610,110), colour(75, 85, 90, 100), line(2), shape(sharp)
gentable    bounds(  5,120,600, 74), tableNumber(2), tableColour("lime"), identChannel("table2"), ampRange(0,127,2), zoom(-1), tableGridColour(0,0,0,0)
label    bounds(  5,198,600, 12), text("Controllers by Channel"), fontColour("white")
label    bounds(  5,197,130, 12), text("Last Controller Value:"), fontColour("white")
nslider  bounds(135,195, 35, 15), fontColour("white"), channel("LastCtrl"), range(0,127,0,1,1)

label    bounds(  5,212,130, 12), text("Last Controller Number:"), fontColour("white")
nslider  bounds(135,210, 35, 15), fontColour("white"), channel("LastCtrlN"), range(0,127,0,1,1)

label    bounds(413,201, 90, 12),  text("MIDI Channel:"), fontColour("white")
combobox bounds(500,200, 33, 15), channel("CtrlChannel"), value(1), text("1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16")

; THE SAME CONTROLLER ON ALL CHANNELS
image    bounds(  0,230, 610,110), colour(75, 85, 90, 100), line(2), shape(sharp)
gentable    bounds(  5,235,600, 74), tableNumber(3), tableColour("LightBlue"), identChannel("table3"), ampRange(0,127,3), zoom(-1), tableGridColour(0,0,0,0)
label    bounds(  5,313,600, 12), text("Controllers by Number"), fontColour("white")
hslider  bounds(380,308,220, 23), fontColour("white"), textBox(1), channel("CtrlNumber"), range(0,127,1,1,1)
label    bounds(380,324,220, 11), text("Controller Number"), fontColour("white")

keyboard bounds(  0,345,610, 80)

</Cabbage>                                                   
                    
<CsoundSynthesizer>                                                                                                 

<CsOptions>                                                     
-dm0 -n -+rtmidi=NULL -M0 -+raw_controller_mode=1
</CsOptions>
                                  
<CsInstruments>

; sr set by host
ksmps         =     32    ;NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls         =     2    ;NUMBER OF CHANNELS (1=MONO)
0dbfs        =    1    ;MAXIMUM AMPLITUDE     

massign    0,1

gisine        ftgen    0,0,4096,10,1

; tables for notes active with velocities
ginotes        ftgen    1,0, 128,-2, 0
gicontrollers    ftgen    2,0, 128,-2, 0
gicontrollers2    ftgen    3,0, 16,-2, 0

instr    1
 inum    notnum                     ; read in midi note number
     chnset    inum,"LastNote"            ; print note number to number box
 
 ivel    veloc    0,127                ; read in note velocity
     chnset    ivel,"LastVel"            ; print velocity value to number box

     tableiw    ivel,inum,ginotes        ; write note to the notes function table

     chnset    "tableNumber(1)","table1"    ; print the function table to the gentable widget

 krel    release
 if krel==1 then
     tablew    1-krel,inum,ginotes
 endif  
endin                                                                                                                     

opcode CtrlRead,0,iiii
 ichan,icount,ilast,itab    xin
 kval    ctrl7    ichan,icount,0,127
 if changed(kval)==1 then
  chnset    kval,"LastCtrl"
  chnset    k(icount),"LastCtrlN"
 endif
     tablew    kval,icount,itab
 if icount<ilast then
  CtrlRead    ichan,icount+1,ilast,itab
 endif
endop

opcode CtrlReadByNumber,0,iii
 ictrl,icount,itab    xin
 kval    ctrl7    icount,ictrl,0,127
 ;if changed(kval)==1 then
 ; chnset    kval,"LastCtrl"
 ;endif
     tablew    kval,icount-1,itab
 if icount<16 then
  CtrlReadByNumber    ictrl,icount+1,itab
 endif
endop

instr    2
 kCtrlChannel    chnget    "CtrlChannel"
 kCtrlChannel    init    1
 
 if changed(kCtrlChannel)==1 then
  reinit RESTART
 endif
 RESTART:
 gicontrollers    ftgen    2,0, 128,-2, 0 
 CtrlRead    i(kCtrlChannel),0,127,gicontrollers
 rireturn

 kCtrlNumber    chnget    "CtrlNumber"
 if changed(kCtrlNumber)==1 then
  reinit RESTART2
 endif
 RESTART2:
 gicontrollers2    ftgen    3,0, 16,-2, 0 
 CtrlReadByNumber    i(kCtrlNumber),1,gicontrollers2
 rireturn

 if metro(20)==1 then
     chnset    "tableNumber(2)", "table2"    ; update table display    
     chnset    "tableNumber(3)", "table3"    ; update table display    
 endif

endin

</CsInstruments>

<CsScore>
i 2 0 [3600*24*7]        ; instrument to update function tables and table displays
</CsScore>                            

</CsoundSynthesizer>
