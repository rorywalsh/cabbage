
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; CellularAutomata.csd
; Written by Iain McCurdy, 2015

; This example makes use of the vcella opcode to implement 1 dimensional cellular automata.
; For a more detailed explanation of cellular automata and the vcella opcode can be found in the Csound manual.

; It is also used to demonstrate the use of the gentable widget with GEN02 and binary values (only on or off) 
;      to create a button bank.
; If the function table is formed with a minus gen number (-2) then it can be edited through the GUI.
; The 'Initial States' table (red) and the 'Rule' (yellow) tables can be edited through the GUI.
; The 'Output States' table (green), formed with a positive gen number (2), cannot be edited and is for display only.

; The sonification employs the output table as if it were a looping step sequencer.
; Each step (cell) is associated with a particular pitch or tone so that we can here its repetition.
; These variations in tone, cell to cell, are randomly chosen but are fixed once set. They can be re-randomised by
; clicking 'Resend Initial State'

; The maximum group size is 64 cells
; The maximum rule length is 27 (bits)


; Trigger Change sets how the cellular automata algorithm will be triggered to apply a change
; --------------
; Trigger    --    this button triggers an update manually
; Auto        --    if this checkbox is active, triggers will be generated automatically by a metronome at a rate 'Rate'
; Rate        --    the rate at which triggers will be generated if 'Auto' is active

; Sonification allows for some setting of how the 'Output States' table is sonified
; ------------
; Note Centre    --    defines the central note of the distribution (MIDI note number)
; Note Range    --    defines the range (+ and -) of the distribution of notes
; Duration    --    defines the duration of notes
; Amplitude    --    ampltude of note (there is some randomisation of amplitude in order the make the results slightly less mechanical)
; Free/Sync. (combobox)    --    sets how the rate of the metronome for generation of notes is controlled
;            if 'free' then rate is independent of the cell tranformation metronome and is defined by the adjacent 'Rate' nslider
;            if 'sync' then it is synced to the cell transformation metronome. In this mode the actual rate of note/rest generation will be: cell_rate x n_elements x repeats

; Rule defines the rule that will be applied to determine future statuses of cells in the output group
; ----
; Length    --    length of the rule (the white bracket over the table indicates how many elements of the table will be used)
; Radius    --    Number of cells (to the left and to the right) to include when applying the rule
; Elements    --    Number of elements to apply the rule to when triggered (the white bracket over the input and output groups indicates how many elements of the tables will be used)


; Initial States defines the initial states (alive(on) / dead(off) of each of the 32 cells)
; --------------
; Resend Initial State    --    pressing this button will reset the cellular group to its original state
;            If cell evolution gets stuck (e.g. all on, all off or alternating between all on and all off)
;               click this the reset.

; Output States is for the display of cells living (on) or dead (off) in the output group
; -------------
; Iterations    --    this number box is for output only and displays the number of iterations we are away from the original state.

; A simple sonification is created by reading through the output cell group to play a rhythm (on=play a note, off=rest)
; Each cell is associated with a particular note.

<Cabbage>
form caption("Cellular Automata [vcella]"), size(830, 360),pluginId("CeAu"), guiRefresh(64), colour("Black")

; Triggering
image      bounds(  5,  5,205, 70), colour(0,0,0,0), outlineColour("white"), outlineThickness(1), shape("sharp"), plant("Trigger") {
label      bounds( 10,  5,100, 13), text("Trigger Change"), align("left")
button     bounds( 10, 35, 60, 20), text("Manual","Manual"), channel("generate"), latched(0)
checkbox   bounds( 80, 35, 50, 15), text("Auto"), channel("AutoGenerate"), value(1), colour("DarkSlateBlue")
nslider  bounds(135, 25, 60, 30), text("Rate"), channel("rate"), range(0,8,0.2,1,0.001)
}

; Sonification
image      bounds(215,  5,610, 70), colour(0,0,0,0), outlineColour("white"), outlineThickness(1), shape("sharp"), plant("Sonification") {
label      bounds( 10,  5,100, 13), text("Sonification"), align("left")
checkbox   bounds( 10, 35, 60, 15), text("On/Off"), channel("sonify"), value(1), colour("DarkSlateBlue")
nslider  bounds( 80, 25, 70, 30), text("Note Centre"), channel("NoteCentre"), range(0,127,100,1,1)
nslider  bounds(155, 25, 70, 30), text("Note Range"), channel("NoteRange"), range(0,72,64,1,1)
nslider  bounds(235, 25, 70, 30), text("Duration"), channel("NoteDur"), range(0.01,0.5,0.1,0.5,0.01)
nslider  bounds(310, 25, 70, 30), text("Amplitude"), channel("NoteAmp"), range(0,1,0.3,0.5,0.01)
combobox   bounds(390, 35, 60, 20), text("Free","Sync."), channel("sync"), value(2)
nslider  bounds(455, 25, 70, 30), text("Rate"), channel("NRate"), range(1,72,8,1,1), identChannel("NRateID")
nslider  bounds(530, 25, 70, 30), text("Repeats"), channel("SyncRepeats"), range(1,16,4,1,1), identChannel("SyncRepeatsID")
}

; Rule
image     bounds(  5, 80,820, 70), colour(0,0,0,0), outlineColour("white"), outlineThickness(1), shape("sharp"), plant("RuleTable") {
label     bounds( 10,  5,100, 13), text("Rule"), align("left")
nslider bounds( 10, 20, 55, 30), text("Length"), channel("RuleLen"), range(1,27,8,1,1)
nslider bounds( 75, 20, 55, 30), text("Radius"), channel("radius"), range(1,2,2,1,1)
image     bounds(135, 20,200, 50), colour(30,30,30), outlineColour("white"), outlineThickness(1), shape("sharp"), identChannel("RuleRange")
gentable  bounds(135, 30,675, 25),  tableNumber(3), tableColour("GoldenRod"), ampRange(0,1,3,1), zoom(-1), active(1)
}

; Initial State
image    bounds(  5,155,820, 85), colour(0,0,0,0), outlineColour("white"), outlineThickness(1), shape("sharp"), plant("InitTable") {
label    bounds( 10,  5,100, 13), text("Initial States"), align("left")
image    bounds( 10, 26,800, 32), colour(60,60,60), outlineColour("white"), outlineThickness(1), shape("sharp"), identChannel("InputRange")
gentable bounds( 10, 35,800, 15), tableNumber(1), tableColour("FireBrick"), ampRange(0,1,1,1), zoom(-1), active(1)
button   bounds( 10, 60,140, 20), text("Resend Initial State","Resend Initial State"), channel("reinit")
}

image     bounds(  5,245,820, 95), colour(0,0,0,0), outlineColour("white"), outlineThickness(1), shape("sharp"), plant("OutTable") {
label     bounds( 10,  5,100, 13), text("Output States"), align("left")
image     bounds( 10, 26,800, 32), colour(60,60,60), outlineColour("white"), outlineThickness(2), shape("sharp"), identChannel("OutputRange")
gentable  bounds( 10, 35,800, 15),  tableNumber(2), tableColour("green"), ampRange(0,1,2,1), zoom(-1), identChannel("OutTable")
nslider bounds( 10, 60, 60, 30), text("Elements"), channel("elements"), range(1,64,32,1,1)
nslider bounds( 80, 60, 60, 30), text("Iterations"), channel("iter"), range(0,100000,0,1,1), active(0), textColour(255,255,255), fontColour(255,255,255)
}

label bounds(5,345,110,11), text("Iain McCurdy |2015|")
</Cabbage>                                                   
                    
<CsoundSynthesizer>                                                                                                 

<CsOptions>                                                     
-dm0 -n
</CsOptions>
                                  
<CsInstruments>

; sr set by host
ksmps         =     32    ;NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls         =     2    ;NUMBER OF CHANNELS (1=MONO)
0dbfs        =    1    ;MAXIMUM AMPLITUDE     
seed    0

giInitTable    ftgen    1,0, 64,-2,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0    ; initial states table
giOutTable    ftgen    2,0, 64, 2,0            ; output states table
giRuleFunc    ftgen    3,0,-27,-2,0,1,0,1,0,0,0,1 ,0,0,1,0,1,0,0,1, 0,0,1,1,0,1,0,0    ; rule function table

opcode TriggerToGatei,k,ki
 ktrig,idur xin
 kdlytrig  delayk    ktrig,idur
 kgate       samphold    ktrig,ktrig + kdlytrig
           xout         kgate
endop

instr    1
 kgenerate    chnget    "generate"            ; manual trigger
 kreinit    chnget    "reinit"            ; reinitialise cell group trigger
 kAutoGenerate    chnget    "AutoGenerate"            ; metronome trigger on/off
 kCRate        chnget    "rate"                ; rate of cell metronome
 
 ktrig        trigger    kgenerate,0.5,0            ; generate a 'bang' if manual trigger button (Trigger) is pressed
 kReinitTrig    trigger    kreinit,0.5,0            ; generate a 'bang' if reinitialise cell group trigger button (Restart) is pressed

 if kAutoGenerate==1 then                ; if 'Auto' is 'on'...
  kAutoTrig    metro    kCRate                ; metronome
  ktrig        +=    kAutoTrig            ; add metronome triggers to manual triggers
 endif 
 
 kRuleLen    chnget    "RuleLen"            ; rule length (in bits)
 kRuleLen    init    8                ; initial value for rule length (zero would cause a crash)
 kradius    chnget    "radius"            ; radius of test group each time rule is applied to a cell
 kradius    init    1                ; initial value for radius (zero would cause a crash)
 kelements    chnget    "elements"            ; number of elements to use (in the output group)
 kelements    init    32                ; initial value for number of elements (zero would cause a crash)
 kiter        init    0                ; number of iterations we are away from the initial state
 
 if changed(kelements)==1 then                ; if number of elements counter has been changed...
  Smsg    sprintfk    "size(%d,33)",25*kelements*0.5    ; new length string
      chnset        Smsg,"InputRange"        ; send to input group bracket
      chnset        Smsg,"OutputRange"        ; send to output group bracket
 endif

 if changed(kRuleLen)==1 then                ; if rule length counter has been changed...
  Smsg    sprintfk    "size(%d,45)",25*kRuleLen    ; new length string
      chnset        Smsg,"RuleRange"        ; send to rule function bracket
 endif
 
 kIChange    changed    kRuleLen,kradius,kelements    ;
 if kIChange==1 then                    ; if any of these i-rate parameters has been changed
  reinit UPDATE                        ; begin a reinitialisation pass from the label
 endif
 if kReinitTrig==1||kIChange==1 then            ; if 'Restart' button has been pressed
  kiter        =    0                ; reset iterations counter
  chnset    kiter,"iter"                ; send zero value to GUI counter
 endif  

 UPDATE:                        ; reinitialise from here
     vcella ktrig, kReinitTrig, giOutTable, giInitTable, giRuleFunc, i(kelements), i(kRuleLen), i(kradius)

 if ktrig==1 then                    ; if a trigger has been generated...
  kiter        +=    1                ; ...increment iterations counter
  chnset    kiter,"iter"                ; send new value to counter
  chnset    "tableNumber(2)","OutTable"        ; update table display of cell output states
 endif
 
 ; SONIFICATION
 ksonify    chnget    "sonify"
 if ksonify==1 then
  if kReinitTrig==1 then
   reinit NEW_RANDOM_ROW
  endif
  NEW_RANDOM_ROW:
  girnd    ftgen    999,0,128,21,1                ; set random distribution (32 uniform random values in the range 0-1)
 
  rireturn
  ksync        chnget    "sync"                ; sync to cell metronome
  ksync        init    2
  if changed(ksync)==1 then
   if ksync==1 then
    chnset    "visible(1)","NRateID"
    chnset    "visible(0)","SyncRepeatsID"
   else
    chnset    "visible(1)","NRateID"
    chnset    "visible(0)","SyncRepeatsID"
   endif
  endif
  
  if ksync==2&&kAutoGenerate==1 then            ; sync to cell metronome (only also if it as running)
   if ktrig==1 then                     ; resync note metronome to cell metronome
    reinit RESET_METRO
   endif
   RESET_METRO:
   iSyncRepeats    chnget    "SyncRepeats"
   kNTrig    metro    kCRate*iSyncRepeats*kelements    ; mstronome synced to cell metronome
   rireturn
  else                            ; free metronome
   kNRate    chnget    "NRate"                ; note rate (notes per second)
   kNTrig    metro    kNRate                ; metronome to trigger notes
  endif
  kstep    init    0                    ; initial step
  if kNTrig==1 then                    ; if metronome trigger is received
   kact    table    kstep,giOutTable            ; read cell state 1=alive 0=dead
   if kact==1 then                    ; if 'alive'
    kNoteDur    chnget    "NoteDur"
    event    "i",2,0,kNoteDur,kstep            ; play a note
   endif
   kstep    wrap    kstep+1,0,kelements        ; increment step counter
  endif
 endif
endin


instr    2    ; sonification
 iRndAmp    random    0.5,1                ; random amplitude (to make sonification a little less mechanical)
 iAmp    chnget    "NoteAmp"
 iNC    chnget    "NoteCentre"
 iNR    chnget    "NoteRange"
 irnd    table    p4,girnd                ; read fixed random value for this cell
 inote    limit    iNC + ((irnd-0.5)*iNR),0,127        ; note number (offset + random offset). Limited 0 - 127.
 p3    =    p3 * octave((52-inote)/24)        ; scale duration according to note number. (Higher notes shorter, lower notes longer)
 aenv    expon    1,p3,0.0001                ; amplitude envelope
 acps    expon    cpsmidinn(inote),p3,cpsmidinn(inote)/8    ; glissando
 asig    poscil    (aenv-0.0001)*iAmp*iRndAmp,acps    ; audio oscillator
 asig    *=    poscil:a(1,100)                ; ring modulate
 ipan    random    0.4,0.6                    ; random pan value (to make sonification a little less mechanical)
 aL,aR    pan2    asig,ipan                ; stereo panned
     outs    aL,aR                    ; send audio to outputs
endin


</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
</CsScore>                            

</CsoundSynthesizer>
