<Cabbage>
form size(260, 290), caption(""), pluginid("WdAr")
checkbox bounds(-100, -100, 25, 25), text("Push"), widgetarray("check", 100), value(0)
button bounds(10, 260, 100, 30), text("Toggle"), channel("toggle")
hslider bounds(157, 253, 103, 37) range(0, 10, 1, 1, 0.001), channel("tempo")
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 -m0d --midi-key=4
</CsOptions>
<CsInstruments>
sr = 44100
ksmps = 32
nchnls = 2
0dbfs=1
  
instr 1
    iCnt init 0
    iCntRows init 0
    kMetro metro 1
    until iCnt > 100 do
        S1 sprintfk "pos(%d, %d)", iCnt%10*25+5, iCntRows*25
        S2 sprintfk "check_ident%d", iCnt+1
        chnset S1, S2
        iCnt=iCnt+1
        iCntRows = (iCnt%10==0 ? iCntRows+1 : iCntRows)
    enduntil

    if changed(chnget:k("toggle")) == 1 then
        printks "Starting toggle", 0
        event "i", "TOGGLE", 0, .01
    endif		
endin

instr SEQUENCER
    kSteps[] init 100
    kIndex = 0
    kColumn init 0
    kRow = 0

    while kIndex<100 do 
        kSteps[kIndex] chnget sprintfk("check%d", kIndex+1)
        kIndex = kIndex+1
    od

    if metro(chnget:k("tempo"))==1 then
    
        while kRow<100 do
                if kSteps[kRow+kColumn]==1 then
                    event "i", "SIMPLE_SYNTH", 0, 1,  ((100-kRow)+4)*10
                endif  
            kRow = kRow+10
        od   
        kColumn = kColumn<10 ? kColumn+1 : 0   
    endif            
endin

instr SIMPLE_SYNTH
    a1 expon .2, p3, .0001
    a2 oscili a1, p4
    outs a2, a2
endin

instr TOGGLE
iCnt init 0
until iCnt > 100 do
	S2 sprintfk "check%d", iCnt+1
	iValue random 0, 100
	chnset iValue<80 ? 0 : 1, S2
	iCnt=iCnt+1
	iCntRows = (iCnt%10==0 ? iCntRows+1 : iCntRows)
enduntil
endin

</CsInstruments>  
<CsScore> 
i1 0 z
i"SEQUENCER" 2 z
</CsScore>
</CsoundSynthesizer> 