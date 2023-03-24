
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; MIDI_Delay.csd
; Written by Iain McCurdy, 2015

; Note that the delay needs to be set up at i-time so changes to parameters force the delay to clear and restart

; Delay Mode  -    method how the delay time is derived: 
;                   1. from a Time (s) dial
;                   2. from a metrical value derived from Tempo and a note value fraction 
; Time (s)    -    delay time (initial delay time if 'Warp' is anything other than '1')
; Tempo (BPM) -    if Delay mode is Tempo this is used as the base for the delay time
; Note fraction - a fraction expressing the note value for the delay unit. E.g.: 1/4 = crotchet or quarter note
; Warp        -    warp ratio of the delay time - each delay iteration will have its delay time multiplied by this value
; Repeats     -    number of repeats/echoes. Bear in mind that having many repeats will put greater polyphony strain on subsequent synths responding to the MIDI.
; Decay       -    velocity decay of subsequent delays. Each subsequent velocity of each delay iteration is multipled by this value.
; Arpeggio    -    note number increase (additive) of each subsequent delay iteration.

<Cabbage>
#define SliderDesign #, fontColour("White"), valueTextBox(1)#
form caption("MIDI Delay"), size(500, 110), pluginId("MiDl") colour(40,40,40), guiMode("queue")
label    bounds( 10,35,80,13) text("Delay Mode"), fontColour("White")
combobox bounds( 10,50,80,20), channel("DelayMode"), items("Time","Tempo"), value(1)
image    bounds( 85,10, 90, 90), channel("TimeMode"), colour(0,0,0,0), visible(1)
{
rslider bounds(  0,  0, 90, 90), channel("DelTim"), range(0.01,2,0.2,0.5,0.01), text("Time (s)") $SliderDesign
}
image    bounds( 85,10,190, 90), colour(0,0,0,0), channel("TempoMode"), visible(0)
{
rslider bounds(  0,  0, 90, 90), channel("Tempo"), range(40,500,100,1,0.1), text("Tempo (BPM)") $SliderDesign
nslider bounds( 90, 20, 40, 25), channel("Num"),   range(1,16,1,1,1)
nslider bounds( 90, 50, 40, 25), channel("Den"),   range(1,16,4,1,1)
}
rslider bounds(215, 10, 90, 90), channel("warp"), range(0.5,2,1,0.5,0.01), text("Warp") $SliderDesign
rslider bounds(280, 10, 90, 90), channel("rpts"), range(1,50,7,1,1), text("Repeats") $SliderDesign
rslider bounds(345, 10, 90, 90), channel("decay"), range(0,1,0.3,1,0.01), text("Decay") $SliderDesign
rslider bounds(410, 10, 90, 90), channel("arp"), range(-12,12,0,1,1), text("Arpeggio") $SliderDesign
;checkbox bounds(310,15,90,15), channel("mono"), text("No Overlaps")

</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-dm0 -+rtmidi=NULL -Q0 -M0 --midi-key=4 --midi-velocity=5 -n    ; cabbage
</CsOptions>

<CsInstruments>

; sr set by host
ksmps     =     16
nchnls     =     2
0dbfs    =    1

opcode    MIDI_Delay_Layer,0,kkkkiiiiii
 kstatus,kchan,kdata1,kdata2,iDelTim,iwarp,idecay,iarp,icount,irpts    xin
 kstatus2    delayk    kstatus,iDelTim    
 kchan2        delayk    kchan,iDelTim    
 kdata1_2    delayk    kdata1,iDelTim    
 kdata2_2    delayk    kdata2,iDelTim    
 if changed(kstatus2)==1 then
  midiout kstatus2, kchan2, limit(kdata1_2+iarp,0,127), kdata2_2*idecay
 endif
 if icount<irpts then
  MIDI_Delay_Layer    kstatus2,kchan2,limit(kdata1_2+iarp,0,127),kdata2_2*idecay,iDelTim*iwarp,iwarp,idecay,iarp,icount+1,irpts
 endif
endop


instr    1
 kstatus, kchan, kdata1, kdata2 midiin
 if changed(kstatus)==1 then
  midiout kstatus, kchan, kdata1, kdata2
 endif
 kDelTim    cabbageGetValue    "DelTim"
 kwarp      cabbageGetValue    "warp"
 kdecay     cabbageGetValue    "decay"
 karp       cabbageGetValue    "arp"
 krpts      cabbageGetValue    "rpts"
 kTempo     cabbageGetValue    "Tempo"
 kDelayMode cabbageGetValue    "DelayMode"
 kDelayMode init               1
            cabbageSet         changed:k(kDelayMode),"TimeMode","visible",1 - (kDelayMode - 1)
            cabbageSet         changed:k(kDelayMode),"TempoMode","visible",(kDelayMode-1)
 kNum       cabbageGetValue    "Num"
 kDen       cabbageGetValue    "Den"
 kDelTim    =                  kDelayMode==1?kDelayMode:(kTempo/60) * (kNum/kDen)
 
; kmono        cabbageGetValue    "mono"
 if metro(16)==1 then
  if changed(kDelTim,kwarp,kdecay,karp,krpts)==1 then
   reinit REINIT
  endif
 endif
 REINIT:
 MIDI_Delay_Layer    kstatus,kchan,kdata1,kdata2,i(kDelTim),i(kwarp),1-i(kdecay),i(karp),1,i(krpts)
endin



</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
</CsScore>

</CsoundSynthesizer>