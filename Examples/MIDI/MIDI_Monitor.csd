
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; MidiMonitor.csd
; Written by Iain McCurdy, 2014, 2022

; display of MIDI notes, key velocity and controllers (MIDI channel 1)

; The first box displays a histogram of note activity from low notes to high notes along the x axis
;  (MIDI note numbers 0-127)
; Data values on the y-axis represent note velocities

; The second box displays continuous controllers from 0-127 on a single MIDI channel.
; MIDI channel used can be selected using the drop down menu.
; In addition, the CC number and data from the most recently moved slider are shown in number boxes.

; The third box shows all continuous controller activity for a single CC number but on all MIDI channels, 0-16

<Cabbage>
form caption("MIDI Monitor"), size(610, 435), colour( 50, 50, 50), pluginId("MiMo"), guiMode("queue")



; NOTES AND VELOCITIES
;image     bounds(  0,  0, 610,115), colour(75, 85, 90, 100), line(2), shape(sharp)
gentable     bounds(  5,  5, 600, 74),  tableNumber(1), tableColour("yellow"), channel("table1"), ampRange(0,127,1), zoom(-1), tableGridColour(0,0,0,0)

label     bounds(  5, 83,600, 12), text("Notes and Velocities"), fontColour("white")
label     bounds(  5, 82, 70, 12), text("Last Note:"), fontColour("white")
nslider bounds( 75, 80, 35, 20), fontColour("white"), channel("LastNote"), range(0,127,0,1,1), fontSize("18")
label     bounds(125, 82, 80, 12), text("Last Velocity:"), fontColour("white")
nslider bounds(205, 80, 35, 20), fontColour("white"), channel("LastVel"), range(0,127,0,1,1), fontSize("18")

; ALL CONTROLLERS ON ONE CHANNEL
;image    bounds(  0,115,610,110), colour(75, 85, 90, 100), line(2), shape(sharp)
gentable    bounds(  5,120,600, 74), tableNumber(2), tableColour("lime"), channel("AllControllers"), ampRange(0,127,2), zoom(-1), tableGridColour(0,0,0,0)
label    bounds(  5,198,600, 12), text("Controllers by Channel"), fontColour("white")

label    bounds(  5,199,128, 12), text("Last Controller Value:"), fontColour("white"), align("right")
nslider  bounds(135,195, 35, 20), fontColour("white"), channel("LastCtrl"), range(0,127,0,1,1), fontSize("18")

label    bounds(  5,219,128, 12), text("Last Controller Number:"), fontColour("white"), align("right")
nslider  bounds(135,215, 35, 20), fontColour("white"), channel("LastCtrlN"), range(0,127,0,1,1), fontSize("18")

label    bounds(413,204, 90, 12),  text("MIDI Channel:"), fontColour("white")
combobox bounds(500,200, 33, 20), channel("CtrlChannel"), value(1), text("1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16")

; THE SAME CONTROLLER ON ALL CHANNELS
image    bounds(  0,240, 610,110), colour(75, 85, 90, 100), line(2), shape(sharp)
gentable bounds(  5,245,600, 74), tableNumber(3), tableColour("LightBlue"), channel("OneController"), ampRange(0,127,3), zoom(-1), tableGridColour(0,0,0,0)
label    bounds(  5,323,600, 12), text("Controllers by Number"), fontColour("white")
hslider  bounds(380,318,220, 23), fontColour("white"), valueTextBox(1), channel("CtrlNumber"), range(0,127,1,1,1)
label    bounds(350,334,220, 11), text("Controller Number"), fontColour("white")

keyboard bounds(  0,355,610, 80)

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

massign    0,1 ; all MIDI sent to instrument 1


; tables for notes active with velocities
ginotes        ftgen    1,0, 128,-2, 0    ; table for midi notes
gicontrollers    ftgen    2,0, 128,-2, 0  ; table for all controllers on a chosen channel
gicontrollers2    ftgen    3,0, 16,-2, 0  ; table for a single controller CC number but on all channels 

instr    1
 inum    notnum                     ; read in midi note number
 ivel    veloc    0,127             ; read in note velocity

; PRINT NOTE NUMBER
 tableiw ivel, inum, ginotes             ; write note and velocity to function table - note number is location, velocity is value
 cabbageSet "table1", "tableNumber", 1   ; send to Cabbage gentable widget
 if release()==1 then                    ; sense note release so that we can zero the table value for this note
  tablew k(0), inum, ginotes             ; write a velocity zero to the table location corresponding to this note
  cabbageSet 1, "table1", "tableNumber", 1 ; write zero to the gentable widget
 endif

     cabbageSetValue    "LastNote", inum, 1           ; print note number to number box
     cabbageSetValue    "LastVel", ivel, 1            ; print note number to number box
 endin                                                                                                                     

; UDO for reading all controllers, CC numbers 0 to 127, in a loop
opcode CtrlRead,0,iiii
 ichan,icount,ilast,itab    xin
 kval    ctrl7    ichan,icount,0,127
 if changed(kval)==1 then
  cabbageSetValue    "LastCtrl",kval,1
  cabbageSetValue    "LastCtrlN",k(icount),1
 endif
     tablew    kval,icount,itab
 if icount<ilast then
  CtrlRead    ichan,icount+1,ilast,itab
 endif
endop

; UDO for reading a chosen controllers, on all MIDI channels 1 to 16
opcode CtrlReadByNumber,0,iii
 ictrl,icount,itab    xin
 kval    ctrl7    icount,ictrl,0,127
 if changed(kval)==1 then
  cabbageSetValue    "LastCtrl",kval,1
 endif
     tablew    kval,icount-1,itab
 if icount<16 then
  CtrlReadByNumber    ictrl,icount+1,itab
 endif
endop


instr 2
 kCtrlChannel    cabbageGetValue    "CtrlChannel" ; user-chosen MIDI channel
 kCtrlChannel    init    1                        ; initialise to 1 (a zero causes problems)
 
 if changed(kCtrlChannel)==1 then                 ; if MIDI channel changed, trigger a reinitialisation
  reinit RESTART
 endif
 RESTART:
 gicontrollers    ftgen    2,0, 128,-2, 0         ; zero table for controller values, CC number 0 to 127
 CtrlRead    i(kCtrlChannel),0,127,gicontrollers  ; call UDO - it will use recursion for all controllers
 rireturn

 kCtrlNumber    cabbageGetValue    "CtrlNumber"   ; user-chosen controller (CC) number
 if changed(kCtrlNumber)==1 then                  ; if user-chosen CC number changed, trigger a reinitialisation
  reinit RESTART2
 endif
 RESTART2:
 gicontrollers2    ftgen    3,0, 16,-2, 0            ; zero table for controller values, MIDI channels 1 to 16
 CtrlReadByNumber    i(kCtrlNumber),1,gicontrollers2 ; call UDO - it will use recursion for all MIDI channels
 rireturn

 if metro(20)==1 then
     cabbageSet    1, "AllControllers", "tableNumber", 2    ; print the function table to the gentable widget
     cabbageSet    1, "OneController", "tableNumber", 3     ; print the function table to the gentable widget
 endif
endin


</CsInstruments>

<CsScore>
i 2 0 [3600*24*7]        ; instrument to update function tables and table displays
</CsScore>                            

</CsoundSynthesizer>
