<Cabbage>
form caption("MIDI Out"), size(400,80), pluginid("Mot2")
keyboard bounds(0,0,400,80)
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-dm0 -odac -Q2 --midi-key=4 --midi-velocity-amp=5
</CsOptions>

<CsInstruments>

sr 	= 	44100
ksmps 	= 	16
nchnls 	= 	2
0dbfs	=	1

instr	1
 ktrig	init	1
 	midion2 1, 60, 70, ktrig
 ktrig	=	0
endin

</CsInstruments>

<CsScore>
i 1 0 2 60 70
</CsScore>


</CsoundSynthesizer>
