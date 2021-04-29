<Cabbage>
form size(600, 200), caption("Random Checkbox"), guiMode("queue"), pluginID("plu1")
checkbox bounds(0, 0, 50, 50), corners(10), channel("checkbox1"), colour:0("lime")
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d
</CsOptions>
<CsInstruments>
sr = 44100 
ksmps = 64
nchnls = 2
0dbfs=1
  
instr 1
	kXPos randi 600, 1, 2
	kYPos randi 200, 1, 2
	cabbageSet metro(5), "checkbox1", "bounds", abs(kXPos), abs(kYPos), 70, 30
endin

</CsInstruments>  
<CsScore> 
i1 0 1000
</CsScore>
</CsoundSynthesizer> 