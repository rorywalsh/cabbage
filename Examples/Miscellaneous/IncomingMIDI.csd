<Cabbage>
form size(430, 140), caption("MidiInput"), colour(20, 20, 20), pluginid("mdgt")

image bounds(340, 10, 12, 12), corners(2), identchannel("midiIdent"), colour(20, 20, 20)
label bounds(344, 8, 80, 15), text("Midi Input"), fontstyle("plain")
keyboard bounds(16, 32, 400, 60)
</Cabbage>

<CsoundSynthesizer>
<CsOptions>
-n -+rtmidi=null -M0 -m0d
</CsOptions>
<CsInstruments>

sr = 44100
ksmps = 32
nchnls = 2
0dbfs = 1

massign 1, 100

instr 100
	;when midi has actived instrument, flash the LED
	kToggle init 0
	if metro(5) == 1 then
		if kToggle==1 then
			chnset "colour(20,20, 20)", "midiIdent"
		else
			chnset "colour(0, 255, 0)", "midiIdent"
		endif
		kToggle = kToggle==1 ? 0 : 1
	endif
	
	;reset LED once MIDI has stopped
	kRel release
	if kRel==1 then
		chnset "colour(20, 20, 20)", "midiIdent"
	endif

endin
        
</CsInstruments>
<CsScore>
f0 z
</CsScore>
</CsoundSynthesizer>
