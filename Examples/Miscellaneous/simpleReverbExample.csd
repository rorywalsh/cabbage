<Cabbage>
form caption("Reverb"), size(220, 150), pluginId("SREx")
groupbox text("Stereo Reverb"), bounds(10, 8, 200, 100)
rslider channel("size"), bounds(10, 34, 70, 70), text("Size"), range(0, 1, 0.2)
rslider channel("fco"), bounds(75, 34, 70, 70), text("Cut-off"), range(0, 22000, 10000)
rslider channel("gain"), bounds(140, 34, 70, 70), text("Gain"), range(0, 1, 0.5)
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-d -n
</CsOptions>
<CsInstruments>
;sr is set by the host
ksmps = 64			;audio samples per control cycle
nchnls = 2			;number of channels
0dbfs = 1			;sets 0dbfs to 1

instr 1
	kFDBack chnget "size"
	kFco chnget "fco"
	kGain chnget "gain"
	aInL inch 1
	aInR inch 2
	aOutL, aOutR reverbsc aInL, aInR, kFDBack, kFco
	outs aOutL*kGain, aOutR*kGain
endin

</CsInstruments>
<CsScore>
f1 0 4096 10 1
i1 0 1000
</CsScore>
</CsoundSynthesizer>
