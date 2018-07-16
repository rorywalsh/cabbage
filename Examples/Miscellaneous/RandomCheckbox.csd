<Cabbage>
form size(600, 200), caption("Rnadom Checkbox"), pluginid("RnCB")
checkbox bounds(0, 0, 50, 50), identchannel("checkboxIdent")
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
	if metro(5)==1 then
		SIdentifier sprintfk "bounds(%d, %d, 50, 50)", abs(kXPos), abs(kYPos)
		chnset SIdentifier, "checkboxIdent"
	endif
endin
</CsInstruments>  
<CsScore> 
i1 0 1000
i2 0 1000
</CsScore>
</CsoundSynthesizer> 