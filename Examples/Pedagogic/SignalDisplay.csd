<Cabbage>
form size(520, 450), caption("SignalDisplay")
signaldisplay bounds(10, 0, 500, 300), colour("lime"), displaytype("waveform"), signalvariable("a1", "a2"), identchannel("displayIdent")
combobox bounds(388, 368, 120, 25), align("centre"), channel("displayCombo"), items("Waveform", "Spectroscope", "Spectrogram", "Lissajous")
keyboard bounds(12, 304, 497, 60)
combobox bounds(12, 368, 100, 30), align("centre"), channel("waveshape"), items("Sine", "Saw", "Square")
hslider bounds(152, 368, 200, 30), channel("lfoRateSlider"), visible(0), range(0, .5, 0.2), identchannel("lfoSliderIdent"), text("LFO freq")
</Cabbage>
<CsoundSynthesizer> 
<CsOptions>
-n --displays -+rtmidi=NULL -M0 --midi-key-cps=4 --midi-velocity-amp=5
</CsOptions>
<CsInstruments>
sr = 44100
ksmps = 16
nchnls = 2
0dbfs=1


instr 1
a1, a2 init 0

if chnget:k("waveshape") == 1 then
	a1 oscili 1, p4
elseif chnget:k("waveshape") == 2 then
	a1 vco2 1, p4
elseif chnget:k("waveshape") == 3 then
	a1 vco2 1, p4, 10
endif
outs a1, a1


aLFO oscil .1, chnget:k("lfoRateSlider")
a2 oscil 1, p4*.9+aLFO

display	a1, .1, 1
dispfft a1, .1, 1024
display	a2, .1, 1  
endin


instr 2
SDisplayIdentString strcpyk ""
kChangeDisplay chnget "displayCombo"
if changed:k(kChangeDisplay)==1 then
	if kChangeDisplay==1 then
		chnset "visible(0)", "lfoSliderIdent"
		SDisplayIdentString strcpyk "displaytype(\"waveform\")"
	elseif kChangeDisplay==2 then
		chnset "visible(0)", "lfoSliderIdent"
		SDisplayIdentString strcpyk "displaytype(\"spectroscope\")"
	elseif kChangeDisplay==3 then
		chnset "visible(0)", "lfoSliderIdent"
		SDisplayIdentString strcpyk "displaytype(\"spectrogram\")"
	elseif kChangeDisplay==4 then
		SDisplayIdentString strcpyk "displaytype(\"lissajous\")"
		chnset "visible(1)", "lfoSliderIdent"
	endif
endif

kTrigger changed kChangeDisplay
if kTrigger == 1 then
	chnset SDisplayIdentString, "displayIdent"
endif 

endin

</CsInstruments>  
<CsScore>
;i1 0 3600
f0 3600
i2 0 3600
</CsScore>
</CsoundSynthesizer>