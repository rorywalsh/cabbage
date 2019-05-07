<Cabbage>
form caption("MIDI Out"), size(400,80), pluginid("MOt1") style("legacy")
keyboard bounds(0,0,400,80)
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
;-dm0 --midi-key=4 --midi-velocity=5 -n -M0 -Q0
;-dm0 -n -+rtmidi=null -M0 -Q0
-dm0 -n -+rtmidi=null -M0 -Q0
</CsOptions>

<CsInstruments>

;sr is set by the host
ksmps 	= 	16
nchnls 	= 	2
0dbfs	=	1

massign	0,1

instr	1	; receive midi and note on
 prints	"note on\n"
 noteon	1,p4,p5
 krel release
 if krel==1 then
  event "i",3,0,0,p4,p5
 endif
endin

instr	3	; note off
 prints	"note off\n"
 noteoff	1,p4,p5
endin

</CsInstruments>

</CsoundSynthesizer>
