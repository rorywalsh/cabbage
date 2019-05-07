<Cabbage>
form size(500, 535), caption(""), pluginid("CGAr")
checkbox bounds(-100, -100, 50, 50), text("Push"), widgetarray("test", 100), value(0)
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 -m0d --midi-key=4
</CsOptions>
<CsInstruments>
;sr is set by the host
ksmps = 64
nchnls = 2
0dbfs=1
  
instr 1
iCnt init 0
iCntRows init 0
kMetro metro kr
	until iCnt > 100 do
		S1 sprintfk "pos(%d, %d)", iCnt%10*50, iCntRows*50
		S2 sprintfk "test_ident%d", iCnt+1
		chnset S1, S2
		iCnt=iCnt+1
		iCntRows = (iCnt%10==0 ? iCntRows+1 : iCntRows)
	enduntil
	if kMetro==1 then
		;event "i", 2, 0, .01
	endif		
endin


instr 2
iCnt init 0
until iCnt > 100 do
	S2 sprintfk "test%d", iCnt+1
	iValue random 0, 100
	chnset iValue<50 ? 0 : 1, S2
	iCnt=iCnt+1
	iCntRows = (iCnt%10==0 ? iCntRows+1 : iCntRows)
enduntil
endin

</CsInstruments>  
<CsScore> 
i1 0 1000
</CsScore>
</CsoundSynthesizer> 