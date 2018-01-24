<Cabbage>
form caption("MIDI Out"), size(400,120), pluginID("Mout")
keyboard bounds(0,0,400,80)
hslider bounds(14, 80, 381, 40) range(0, 10, 5, 1, 0.001), channel("tempo"), text("Tempo")
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-dm0 -n -+rtmidi=NULL -M0 -Q0 --midi-key=4 --midi-velocity=5
</CsOptions>
<CsInstruments>
sr 	= 	44100
ksmps 	= 	16
nchnls 	= 	2
0dbfs	=	1

instr	1
    prints "\n"
    kNoteIndex init 0
    kTempo chnget "tempo"
    kNoteArray[] fillarray 0, 5, 7, 12
    kMetro metro kTempo

    if kMetro == 1 then
        midion2 1, p4+kNoteArray[kNoteIndex], p5, 1
        kNoteIndex = kNoteIndex<3 ? kNoteIndex + 1 : 0
    endif
endin

</CsInstruments>

<CsScore>
f0 z
</CsScore>


</CsoundSynthesizer>