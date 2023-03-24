
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; MIDI Arpeggiator.csd
; Written by Iain McCurdy, 2015. Based on the Arpeggiator (2012) example. 

; Interval   - interval (in semitones) between repeats of the arpeggio
; Cycles     - number of repeats of the arpeggio
; Cycle Mode - shifting mode for the cyclical repetitions (Up or Up/Down)
; Arp Mode   - way in which held notes are repeated:
;              1. Up, from low to high
;              2. Down, from high to low
;              3. Up-Down, first from low to high, then from high to low
;              4. Random Dir., as in a Markov Chain or a Drunk Walk
;              5. Random Pick., as in 'item heap', no repetitions until all have been selected within an arpeggio
;              6. Seq. Play following the order in which the notes were added
; Tempo      - tempo in BPM
; Tempo Mlt. - tempo multiplier in order to make some metrical modulations easier
; Swing      - straight or dotted rhythms between pairs of notes in the arpeggio
; Octave     - transposition of all notes in octaves
; Semitone   - transposition of all notes in semitone
; Duration   - duration for which notes in the arpeggio are held (in seconds)
; Clock Source - tempo taken from Tempo dial widget (INTERNAL) or host tempo (EXTERNAL)
; On/Off     - start/stop the arpeggiator
; Hold       - hold notes even when released (as in a sustain pedal)
;              this is particularly useful if the GUI keyboard is used
; Pause      - pause the arpeggio. It picks up again where it left off when this is deactivated again.


<Cabbage>
form caption("Arpeggiator") size(720, 180), pluginId("MArp"), colour("Black")

#define RSliderStyle #trackerColour(0,255,255), colour(100,100,140), outlineColour(20,20,20)#

                      
groupbox bounds( 0,  0, 720, 100), text("Arpeggiator"), plant("gui"){
rslider  bounds( 10, 30, 60, 60), text("Interval"), channel("intvl"), range(-36, 36, 5, 1, 1), $RSliderStyle
rslider  bounds( 60, 30, 60, 60), text("Cycles"), channel("cycles"), range(0, 32, 5, 1, 1), $RSliderStyle

label    bounds(128, 26, 74, 12), text("Cycle Mode") 
combobox bounds(128, 39, 74, 20), channel("CyUpDn"), value(2), text("Up","Up-Down")

label    bounds(120, 62, 90, 12), text("Arp.Mode")
combobox bounds(120, 75, 90, 20), channel("mode"), value(6), text("Up","Down","Up<->Down","Random Dir.","Random Pick","Seq. Play")

rslider  bounds(210, 30, 60, 60), text("Tempo"), channel("tempo"), range(1, 500,120, 1, 1), $RSliderStyle
label    bounds(263, 29, 65, 12), text("Tempo Mlt."), 
combobox bounds(270, 42, 50, 20), channel("TempoMlt"), value(4), text("1/4","1/3","1/2","1","3/2","2","3","4")

rslider  bounds(320, 30, 60, 60), text("Swing"),    channel("swing"),    range(0, 1.00, 0), $RSliderStyle

rslider  bounds(375, 30, 60, 60), text("Octave"),   channel("octave"),   range(-4,4,0,1,1), $RSliderStyle
rslider  bounds(430, 30, 60, 60), text("Semitone"), channel("semitone"), range(-11,11,0,1,1), $RSliderStyle
rslider  bounds(485, 30, 60, 60), text("Duration"), channel("duration"), range(0.25,4,1,0.5,0.01), $RSliderStyle

label    bounds(550, 29, 78, 13), text("Clock Source")
button   bounds(550, 42, 80, 20), text("Internal","External"), channel("ClockSource"), value(0)
checkbox bounds(645, 35, 60, 15), colour("Cyan"),   channel("onoff"),  text("On/Off"), value(1)
checkbox bounds(645, 55, 60, 15), colour("yellow"), channel("hold"),   text("Hold"),   value(1)
checkbox bounds(645, 75, 60, 15), colour("red"),    channel("pause"),  text("Pause"),  value(0)
}

keyboard bounds(0,100,720,80)

</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-dm0 -n -+rtmidi=NULL -M0 -Q0
</CsOptions>

<CsInstruments>

; sr set by host
ksmps         =     32    ;NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls         =     2    ;NUMBER OF CHANNELS (2=STEREO)
0dbfs        =    1
massign    0,0
    
;Author: Iain McCurdy (2012)
;http://www.iainmccurdy.org    

giseqorder    ftgen    0,0,1024,-2,0    ;ORDERED ROW OF NOTES (IN THE ORDER IN WHICH THEY WERE PLAYED)
giseqascend    ftgen    0,0,1024,-2,0    ;ORDERED ROW OF NOTES (IN ASCENDING ORDER)
ginoteactive    ftgen    0,0,128,-2,0    ;TABLE OF NOTE ON STATUSES 1=ON 0=OFF
givelocities    ftgen    0,0,128,-2,0    ;TABLE OF NOTE VELOCITIES
giblank        ftgen    0,0,128,-2,0    ;BLANK TABLE
gisine        ftgen    0,0,4096,10,1
giFletcherMunsonCurve    ftgen    0,0,-20000,-16,1,4000,-8,0.15, 20000-4000,0,0.15

giTempoMlt    ftgen    0,0,-8,-2,1/4,1/3,1/2,1,3/2,2,3,4    ;TABLE OF TEMPO MULTIPLIERS - USED TO SCALE THE ARPEGGIO RATE WHEN IN TEMPO MODE
giMIDIchn    =    1

;UDO THAT SORTS A TABLE OF NUMBERS INTO ASCENDING ORDER
opcode    tabsort_ascnd2,0,ii
    iNumItems,ifn    xin
    iTabLen        =        ftlen(ifn)
    imax        table        0,ifn
    icount        =        1
    loop1:
      ival        table        icount,ifn
      imax         =         (ival>=imax?ival:imax)
              loop_lt        icount,1,iNumItems,loop1
    iTableBuffer    ftgentmp    0,0,iTabLen,-2, 0
    icount1        =        0
    loop2:
      icount2    =        0
      imin        =        imax
      loop3:
        ival    table        icount2,ifn
        if ival<=imin then            
          imin     =         ival
          iloc     =         icount2 
        endif
                loop_lt        icount2,1,iNumItems,loop3
            tableiw        imin,icount1,iTableBuffer
            tableiw        imax,iloc,ifn
            loop_lt        icount1,1,iNumItems,loop2
    icount        =        0
    loop4:
    ival        table        icount,iTableBuffer
            tableiw        ival,icount,ifn
            loop_lt        icount,1,iNumItems,loop4
endop


;This instrument needs to be last in order for the 'hold' function to work properly. I'm not sure why.
instr    ScanWidgets
    gkintvl        chnget    "intvl"
    gkcycles    chnget    "cycles"
    gktempo        chnget    "tempo"        ;INTERNAL GUI CONTROL
    gkbpm        chnget    "HOST_BPM"    ;HOST TEMPO
    gkClockSource    chnget    "ClockSource"    ;CLOCK SOURCE SELECTOR SWITCH
    if gkClockSource==1 then
     gktempo = gkbpm
    endif
    gkCyUpDn    chnget    "CyUpDn"
    gkTempoMlt    chnget    "TempoMlt"
    gkhold        chnget    "hold"
    gkmode        chnget    "mode"
    gkswing        chnget    "swing"    
    gkpause        chnget    "pause"
    gkonoff        chnget    "onoff"
endin

instr    ScanMIDI    ;SCAN INCOMING MIDI AND TRIGGER NOTES IN INSTRUMENT 'NoteLayer'
    insno     nstrnum "NoteLayer"
    kstatus, kchan, kdata1, kdata2  midiin; read in midi
    if kstatus==144||kstatus==128 then            ;144=note_on 128=note_off
     if kdata2>0&&kstatus==144 then                ;IF VELOCITY IS GREATER THAN 0 (I.E. FOR SOME KEYBOARDS VELOCITY ZERO IS A NOTE OFF)
      tablew    kdata2,kdata1,givelocities        ;WRITE VELOCITY TO TABLE
      kAlreadyActiveStatus    table    kdata1,ginoteactive    ;CHECK IF THIS NOTE IS ALREADY ACTIVE (POSSIBLE IF HOLD IS ON).
      if kAlreadyActiveStatus==0 then            ;IF THIS NOTE IS NOT ALREADY ACTIVE...
       event "i",insno+(kdata1*0.001),0,3600,kdata1    ;
      endif
     else
      if gkhold==0 then
       turnoff2    insno+(kdata1*0.001),4,1
      endif
     endif
    elseif kstatus==128&&gkhold==0 then        ;IF MIDI KEYBOARD USES NOTE OFF STATUS BYTE
     turnoff2    insno+(kdata1*0.001),4,1
    endif
endin    

instr    NoteLayer                ;THIS INSTRUMENT IS TRIGGERED FOR EACH NOTE PLAYED ON THE KEYBOARD
    inum    =    p4            ;READ IN MIDI NOTE NUMBER        
    tableiw    1,inum,ginoteactive        ;WRITE NOTE NUMBER TO TABLE
    iNNotes    active    "NoteLayer"        ;SENSE THE NUMBER INSTANCES OF THIS INSTRUMENT (I.E. MIDI NOTES) ARE BEING HELD AT I-TIME.
    tableiw    inum,iNNotes-1,giseqorder    ;WRITE THE NOTE NUMBER OF THIS NOTE TO THE NEXT LOCATION IN THE ORDERED ROW OF NOTES (giseqorder)
    tableicopy    giseqascend,giseqorder    ;COPY THE LIST OF NOTES (IN THE ORDER IN WHICH THEY WERE PLAYED) INTO THE TABLE OF NOTES TO BE SORTED INTO ASCENDING ORDER
    tabsort_ascnd2    iNNotes,giseqascend    ;UDO CALLED THAT ORDERS THE LIST OF NOTES INTO ASCENDING ORDER
    
    krelease    release            ;SENSE WHEN THIS NOTE HAS BEEN RELEASED
    if krelease==1 then            ;IF THIS NOTE HAS BEEN RELEASED (FINAL K-RATE CYCLE)...
     tablew    0,inum,ginoteactive
     ;AS THIS NOTE HAS BEEN RELEASED IT WILL HAVE TO BE REMOVED FROM THE ORDERED ROW OF NOTE (giseqorder). ALL NOTES *AFTER* IT IN THE ROW WILL HAVE TO BE SHUNTED BACK ONE PLACE.
     kShuntNdx    =    iNNotes        ;INITIAL SHUNT INDEX (THE LOCATION TO WHICH THE NOTE NUMBER FOR THIS NOTE WAS WRITTEN)
     kNNotes    active    "NoteLayer"    ;FIND THE NUMBER OF INSTANCES OF THIS INSTRUMENT (I.E. NOTES BEING HELD) NOW
     SHUNT_ROW:                ;A LABEL. THE SHUNTING PROCEDURE LOOPS FROM HERE.
     kval    table    kShuntNdx, giseqorder    ;READ THE NOTE NUMBER JUST AFTER THIS ONE IN THE SEQUENCE ROW...
         tablew    kval, kShuntNdx-1, giseqorder    ;AND MOVE IT BACK ONE PLACE
     loop_lt    kShuntNdx,1,kNNotes,SHUNT_ROW    ;LOOP BACK AND REPEAT THE SHUNTING PROCEDURE UNTIL THE NEW ROW IS COMPLETE
    endif
    
    kHoldOff    trigger    gkhold,0.5,1    ;IF HOLD GOES FROM '1'/'ON' TO '0'/'OFF' GENERATE A TRIGGER IMPULSE
    if kHoldOff==1 then            ;IF HOLD GOES FROM 'ON' TO 'OFF'... 
     turnoff                ;TURN THIS (AND ALL OTHER) NOTES OFF
    endif                    ;END OF CONDITIONAL BRANCH

    if iNNotes==1 then            ;IF THIS IS THE FIRST NOTE OF AN ARPEGGIO TO BE PLAYED...
     event_i "i","Arpeggiator",0,-1        ;START ARPEGGIATOR INSTRUMENT WITH A 'HELD' NOTE. SEND IT THE MIDI NOTE NUMBER.
    endif                    ;END OF CONDITIONAL BRANCH

    insnoArp     nstrnum "Arpeggiator"
    if gkonoff==0 then
     turnoff2    insnoArp,0,1
     tablecopy    ginoteactive,giblank        ;ERASE NOTE STATUSES TABLE
     turnoff
    endif
endin


instr    Arpeggiator    
    krelease    release                ;SENSE END OF NOTE (1)
    kporttime    linseg    0,0.001,0.05

    kHoldOff    trigger    gkhold,0.5,1        ;IF HOLD GOES FROM '1'/'ON' TO '0'/'OFF' GENERATE A TRIGGER IMPULSE
    if kHoldOff==1 then                ;IF HOLD GOES FROM 'ON' TO 'OFF'... 
     tablecopy    ginoteactive,giblank        ;ERASE NOTE STATUSES TABLE
     turnoff                    ;TURN THIS INSTRUMENT OFF
    endif                        ;END OF CONDITIONAL BRANCH

    kNNotes    active    "NoteLayer"            ;NUMBER OF INSTR 1 (MIDI NOTES) BEING HELD. SPECIFICALLY WE ARE INTERESTED IN WHETHER ALL NOTES HAVE BEEN RELEASED    
    
    if kNNotes==0&&gkhold==0 then            ;IF ALL MIDI KEYBOARD NOTES HAVE BEEN RELEASED...
     turnoff                    ;...TURN THIS INSTRUMENT OFF
    endif                        ;END OF CONDITIONAL BRANCH

    kswingval1    scale    gkswing,1/1.5,1
    kswingval2    scale    gkswing,1/0.5,1
    kswingstep    init    0
    kswingval    init    (i(gkswing)*0.5) + 1
    
    kTempoMlt    table    gkTempoMlt-1,giTempoMlt
    
    gkrate    =    (gktempo/60)*kTempoMlt*4*(1-gkpause)        ;DERIVE TEMPO FROM BPM AND TEMPO MULTIPLIER
    
    ktrigger    metro    gkrate*kswingval            ;METRONOME TO TRIGGER NOTES OF THE ARPEGGIO. PHASE OFFSET (P2) PREVENTS A SEARCH FOR A NEW NOTE WHEN THE FIRST NOTE OF AN ARPEGGIO HAS BEEN PLAYED

    ;SET REQUIRED INITIAL SETTINGS DEPENDING ON ARPEGGIATOR MODE
    ktrig    changed    gkmode
    if ktrig==1 then
     reinit    RESET_START_VALS
    endif
    RESET_START_VALS:
    kcount1    init    0
    kcount2    init    0
    kndx    init    0
    kcycle    init    0
    kCycleDir    init    0
    if i(gkmode)==1 then                ;IF UP MODE...
     kcount1    init    0
     kcount2    init    0
     kndx    init    0
     kdir    init    1
    elseif i(gkmode)==2 then            ;IF DOWN MODE...
     kcount1    init    0
     kcount2    init    0
     kndx    init    i(kNNotes)-1
     kdir    init    -1
    elseif i(gkmode)==3 then            ;IF UP AND DOWN MODE...
     kdir    init    1
     kcount1    init    0
     kcount2    init    0
     kndx    init    0
    elseif i(gkmode)==4 then            ;RANDOM DIRECTION MODE...
     kdir    init    1
     kcount1    init    0
     kcount2    init    0
     kndx    init    0
    elseif i(gkmode)==5 then            ;RANDOM PICK MODE...
     kcount1    init    0
     kcount2    init    0
     kndx    init    0
    elseif i(gkmode)==6 then            ;SEQUENCE PLAY MODE...
     kcount1    init    0
     kcount2    init    0
     kndx    init    0
     kdir    init    1
    endif
    rireturn
    
    if ktrigger==1&&krelease!=1 then        ;IF A TRIGGER FOR A NEW NOTE HAS BEEN ISSUED AND WE ARE NOT IN A RELEASE STAGE...

     kswingstep    =    abs(kswingstep-1)
     kswingval    =    (kswingstep=0?kswingval1:kswingval2)
    
     ;UP
     if gkmode==1 then                ;IF 'UP' DIRECTION MODE IS SELECTED...
      knumSrc        table    kndx,giseqascend    ;READ NOTE NUMBER
      knum        =    knumSrc+(gkintvl*kcycle)
      kcount1    =    kcount1 + 1
      ktrig        changed    kNNotes
      if ktrig==1 then                ;IF NOTES ARE ADDED OR TAKEN AWAY FROM THE ROW SINCE THE LAST ITERATION
       kcount1    =    kndx+1
      endif      
      if kndx==kNNotes-1 then
       kcount2    =    kcount2+1
       if gkCyUpDn==1 then
        kcycle    wrap    kcount2,0,gkcycles+1
       else
        kcycle    mirror    kcount2,0,gkcycles
       endif        
      endif      
      kndx        wrap    kcount1,-0.5,kNNotes-0.5  
      


     ;DOWN
     elseif gkmode==2 then                ;IF 'DOWN' DIRECTION MODE IS SELECTED...
      knumSrc    table    kndx,giseqascend    ;READ NOTE NUMBER
      knum        =    knumSrc+(gkintvl*kcycle)
      if kndx==0 then
       kcount2    =    kcount2+1
       if gkCyUpDn==1 then
        kcycle    wrap    kcount2,0,gkcycles+1
       else
        kcycle    mirror    kcount2,0,gkcycles
       endif        
      endif      
      kcount1    =    kcount1 - 1
      ktrig        changed    kNNotes
      if ktrig==1 then                ;IF NOTES ARE ADDED OR TAKEN AWAY FROM THE ROW SINCE THE LAST ITERATION
       kcount1    =    kndx-1
      endif      
      kndx        wrap    kcount1,-0.5,kNNotes-0.5  



    
     ;UP-DOWN
     elseif gkmode==3 then                ;IF 'UP<->DOWN' DIRECTION MODE IS SELECTED...
      if gkcycles==0 then
       if kNNotes==1 then
        kdir    =    0
       elseif kndx=0 then                ;OR IF... THE INDEX IS POINTING TO THE FIRST NOTE IN THE LIST, WE NEED TO CHANGE DIRECTION TO UP FOR THE NEXT NOTE AFTER THIS ONE
        kdir    =    1            ;CHANGE DIRECTION TO UP
       elseif kndx=(kNNotes-1) then            ;IF THE INDEX IS POINTING TO THE LAST NOTE IN THE LIST, WE NEED TO CHANGE DIRECTION TO DOWN FOR THE NEXT NOTE AFTER THIS ONE
        kdir    =    -1            ;CHANGE DIRECTION TO DOWN
       endif                    ;END OF CONDITIONAL
       kndx    =    kndx + kdir
       knumSrc    table    kndx,giseqascend    ;READ NOTE NUMBER
       knum        =    knumSrc
       kgoto PLAY_A_NOTE                ;GO STRAIGHT TO PLAYING THIS NEW NOTE

      else
       kndx        limit    kndx,0,kNNotes-1
       knumSrc    table    kndx,giseqascend    ;READ NOTE NUMBER
       knum        =    knumSrc+(gkintvl*kcycle)
       kcount    init    i(kndx)
       kcount    =    kcount + 1
       kndx        mirror    kcount,0,kNNotes-1
       if kndx==0 then
        kcount2    init    i(kcycle)
        kcount2    =    kcount2+1        
        if gkCyUpDn==1 then
         kcycle    wrap    kcount2,0,gkcycles+1
        else
         kcycle    mirror    kcount2,0,gkcycles
        endif        
       endif
       
       kgoto PLAY_A_NOTE                ;GO STRAIGHT TO PLAYING THIS NEW NOTE
      endif
     
     
     
     
     
     
     
     
     
     
     
     
     
     ;RANDOM DIRECTION
     elseif gkmode==4 then
      if kndx==(kNNotes-1) then            ;IF THE INDEX IS POINTING TO THE LAST NOTE IN THE LIST, WE NEED TO CHANGE DIRECTION TO DOWN FOR THE NEXT NOTE AFTER THIS ONE
       kdir    =    -1                ;CHANGE DIRECTION TO DOWN
      elseif kndx==0 then                ;OR IF... THE INDEX IS POINTING TO THE FIRST NOTE IN THE LIST, WE NEED TO CHANGE DIRECTION TO UP FOR THE NEXT NOTE AFTER THIS ONE
       kdir    =    1                ;CHANGE DIRECTION TO UP
      endif                        ;END OF CONDITIONAL
      knumSrc    table    kndx,giseqascend    ;READ NOTE NUMBER
      knum        =    knumSrc    
      if gkcycles>0 then
       knum    =    knum+(gkintvl*kcycle)
      endif    
      if kndx==(kNNotes-1) then
       kcycle    wrap    kcycle+1,0,gkcycles+1
      elseif kndx==0 then    
       kcycle    wrap    kcycle-1,0,gkcycles+1
      endif
      kndx    =    kndx+kdir            ;INCREMENT INDEX FOR THE NEXT NOTE AFTER THIS ONE
      kgoto PLAY_A_NOTE                ;GO STRAIGHT TO PLAYING THIS NEW NOTE


     
     ;RANDOM PICK (RANDOMLY CHOOSE ANY NOTE CURRENTLY BEING HELD)
     elseif gkmode==5 then
      kRndNdx    random    0,kNNotes        ;CREATE RANDOM INDEX
      knumSrc    table    kRndNdx,giseqorder    ;READ NOTE FROM NOTE LIST USING RANDOM INDEX
      knum    =    knumSrc+(gkintvl*kcycle)
      if kndx==kNNotes-1 then
       kcount2    =    kcount2 + 1
       if gkCyUpDn==1 then
        kcycle    wrap    kcount2,0,gkcycles+1
       else
        kcycle    mirror    kcount2,0,gkcycles
       endif
      endif
      kndx    wrap    kndx+1,0,kNNotes        ;INCREMENT THE COUNTER BUT WRAP IT AROUND IF IT STRAYS BEYOND THE VALUE CORRESPONDING TO THE NUMBER OF NOTES BEING HELD (I.E. THE LENGTH OF THE SEQUENCE)
      kgoto    PLAY_A_NOTE                ;GO STRAIGHT TO PLAYING THIS NEW NOTE


    
     ;SEQUENCE PLAY (PLAYS NOTES IN THE ORDER IN WHICH THEY WERE ORIGINALLY PLAYED)
     else
      knumSrc    table    kndx,giseqorder        ;READ NOTE VALUE FROM TABLE  
      knum        =    knumSrc
      if gkcycles!=0 then
       knum    =    knum+(gkintvl*kcycle)
      endif    
      if kndx==kNNotes-1 then
       kcount2    =    kcount2 + 1
       if gkCyUpDn==1 then
        kcycle    wrap    kcount2,0,gkcycles+1
       else
        kcycle    mirror    kcount2,0,gkcycles
       endif
      endif
      kndx    wrap    kndx+1,0,kNNotes        ;INCREMENT THE COUNTER BUT WRAP IT AROUND IF IT STRAYS BEYOND THE VALUE CORRESPONDING TO THE NUMBER OF NOTES BEING HELD (I.E. THE LENGTH OF THE SEQUENCE) 
      kgoto PLAY_A_NOTE                ;GO STRAIGHT TO PLAYING THIS NEW NOTE
     endif
    endif
    
    
    
    
    
    PLAY_A_NOTE:

    if gkmode==4&&ktrigger==1 then            ;IF RANDOM DIRECTION MODE HAS BEEN CHOSEN, CHOOSE A NEW RANDOM DIRECTION FOR THE NEXT STEP 
     kdir    random    0,1.999999            ;
     kdir    =    (int(kdir)*2)-1            ;kdir WILL BE EITHER -1 OR 1
    endif

    if ktrigger==1 then
     event "i","MIDI_out",0,60/(gkrate*kswingval*16*3.75),knum,knumSrc
    endif

endin



instr    MIDI_out
 ioctave    chnget    "octave"
 isemitone    chnget    "semitone"
 iduration    chnget    "duration"
 ivel        table    p5,givelocities
 p3        =    p3 * iduration
 inote        limit    p4+(ioctave*12)+isemitone,0,127
 noteondur giMIDIchn, inote, ivel, p3
endin

</CsInstruments>

<CsScore>
i "ScanWidgets" 0 [3600*24*7]        ;SCAN GUI WIDGETS
i "ScanMIDI" 0 [3600*24*7]        ;SCANS FOR MIDI NOTE EVENTS
</CsScore>

</CsoundSynthesizer>