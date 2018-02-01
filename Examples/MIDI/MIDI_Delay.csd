; MIDI_Delay.csd
; Written by Iain McCurdy, 2015

; Time		-	delay time (initial delay time if 'Warp' is anything other than '1'
; Warp		-	warp ratio of the delay time - each delay iteration will have its delay time multiplied by this value
; Repeats 	-	number of repeats/echoes. Bear in mind that having many repeats will put greater polyphony strain on subsequent synths responding to the MIDI.
; Decay		-	velocity decay of subsequent delays. Each subsequent velocity of each delay iteration is multipled by this value.
; Arpeggio	-	note number increase (additive) of each subsequent delay iteration.

<Cabbage>
form caption("MIDI Delay"), size(300,100), pluginid("MiDl")
rslider bounds(0,5,60,60), channel("DelTim"), range(0.01,2,0.2,0.5,0.01), text("Time")
rslider bounds(60,5,60,60), channel("warp"), range(0.5,2,1,0.5,0.01), text("Warp")
rslider bounds(120,5,60,60), channel("rpts"), range(1,50,7,1,1), text("Repeats")
rslider bounds(180,5,60,60), channel("decay"), range(0,1,0.3,1,0.01), text("Decay")
rslider bounds(240,5,60,60), channel("arp"), range(-12,12,0,1,1), text("Arpeggio")
;checkbox bounds(310,15,90,15), channel("mono"), text("No Overlaps")

</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-dm0 -+rtmidi=null -Q0 -M0 --midi-key=4 --midi-velocity=5 -n	; cabbage
</CsOptions>

<CsInstruments>

sr 	= 	44100
ksmps 	= 	16
nchnls 	= 	2
0dbfs	=	1

opcode	MIDI_Delay_Layer,0,kkkkiiiiii
 kstatus,kchan,kdata1,kdata2,iDelTim,iwarp,idecay,iarp,icount,irpts	xin
 kstatus2	delayk	kstatus,iDelTim	
 kchan2		delayk	kchan,iDelTim	
 kdata1_2	delayk	kdata1,iDelTim	
 kdata2_2	delayk	kdata2,iDelTim	
 if changed(kstatus2)==1 then
  midiout kstatus2, kchan2, limit(kdata1_2+iarp,0,127), kdata2_2*idecay
 endif
 if icount<irpts then
  MIDI_Delay_Layer	kstatus2,kchan2,limit(kdata1_2+iarp,0,127),kdata2_2*idecay,iDelTim*iwarp,iwarp,idecay,iarp,icount+1,irpts
 endif
endop


instr	1
 kstatus, kchan, kdata1, kdata2 midiin
 if changed(kstatus)==1 then
  midiout kstatus, kchan, kdata1, kdata2
 endif
 kDelTim	chnget	"DelTim"
 kwarp		chnget	"warp"
 kdecay		chnget	"decay"
 karp		chnget	"arp"
 krpts		chnget	"rpts"
; kmono		chnget	"mono"
 if metro(16)==1 then
  if changed(kDelTim,kwarp,kdecay,karp,krpts)==1 then
   reinit REINIT
  endif
 endif
 REINIT:
 MIDI_Delay_Layer	kstatus,kchan,kdata1,kdata2,i(kDelTim),i(kwarp),1-i(kdecay),i(karp),1,i(krpts)
endin



</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
</CsScore>

</CsoundSynthesizer>