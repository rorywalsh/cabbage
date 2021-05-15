<Cabbage>
form size(430, 140), caption("MidiInput") guiMode("queue"), pluginId("mdgt")


image bounds(340, 10, 12, 12), corners(2), colour(20, 20, 20), channel("midOff")
image bounds(340, 10, 12, 12), corners(2), colour(147, 210, 0), alpha(0), channel("midiOn")

label bounds(344, 8, 80, 15), text("Midi Input"), fontStyle("plain")
keyboard bounds(16, 32, 400, 60)
</Cabbage>

<CsoundSynthesizer>
<CsOptions>
-n -+rtmidi=null -M0 -m0d
</CsOptions>
<CsInstruments>

;sr is set by the host
ksmps = 32
nchnls = 2
0dbfs = 1

massign 1, 100

instr 100
	;when midi has actived instrument, flash the LED
	kToggle init 0
	if metro(10) == 1 then
		cabbageSet 1, "midiOn", "alpha", kToggle
		kToggle = kToggle==1 ? 0 : 1
	endif
	
	;reset LED once MIDI has stopped
	kRel release
	if kRel==1 then
		cabbageSet 1, "midiOn", "alpha", 0
	endif

endin
        
</CsInstruments>
<CsScore>
f0 z
</CsScore>
</CsoundSynthesizer>
