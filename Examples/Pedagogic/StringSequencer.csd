<Cabbage>
form caption("Event Sequencer") size(600, 400), pluginID("def1")
eventsequencer bounds(10, 10, 500, 320), channels("step"), active(0), identchannel("trackerIdent"), orientation("horizontal"), colprefix(0:1:2:3, "i\"Sine\" 0"), showstepnumbers(4), matrixsize(16, 4) textcolour(200, 200, 200), highlightcolour(30, 30, 30) outlinecolour(80,80,80), bpm(180), fontcolour("white") backgroundcolour(20, 20, 20)
rslider bounds(514, 10, 70, 70) channel("bpm") range(10, 400, 180, 1, 0.001) text("BPM") 
button bounds(514, 82, 70, 27) channel("startPlayback") text("Start", "Stop")  
button bounds(514, 132, 70, 27) channel("shuffle") text("Shuffle")  
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -m0d -m0d --sample-accurate
</CsOptions>
<CsInstruments>
; Initialize the global variables. 
sr = 44100 
ksmps = 32
nchnls = 2
0dbfs = 1
;The following instrument listens for the current step and 
;trigger instruments accordingly 
instr 1 
    kBeat init 0    
    kBpm chnget "bpm"
    
    if metro(kBpm/60) == 1 && chnget:k("startPlayback") ==1 then      
        chnset kBeat, "step" 
        kBeat = kBeat<15 ? kBeat+1 : 0
    endif
    
    
    if changed(kBpm) == 1 then
        SMessage sprintfk "bpm(%d)", kBpm
        chnset SMessage, "trackerIdent"
    endif
    
    if changed:k(chnget:k("shuffle")) == 1 then 
            event "i", "FillCells", 0, 0
    endif
endin
 
;Simple synthesiser
instr Sine
    aEnv expon .2, p3, .001
    aOscil oscil aEnv, cpsmidinn(p4), 2
    outs aOscil, aOscil
endin


;fills cell with random note data 
instr FillCells
    iNotes[] fillarray 60, 62, 64, 65, 67, 72, 48, 36, 74, 75
    iRowCnt, iColCnt init 0
    print lenarray(iNotes)
    iNumRow = 16
    iNumCols = 4
    SCellData init ""
    while iRowCnt < iNumRow do
        iColCnt = 0
        while iColCnt < iNumCols do
            iNoteIndex random 0, lenarray(iNotes)
            if random:i(0, 100)>70 then
                SScoreEvent sprintf "1 %d", iNotes[int(iNoteIndex)]
                SMessage sprintf "celldata(%d, %d, %s) ", iColCnt, iRowCnt, SScoreEvent 
            else
                SMessage sprintf "celldata(%d, %d, \"\") ", iColCnt, iRowCnt
            endif
            SCellData strcat SCellData, SMessage
        iColCnt+=1
        od
        iRowCnt+=1
    od  
    ;when SCellData strings has been constructed, send it to Cabbage
    ;prints SCellData
    chnset SCellData, "trackerIdent"    
endin
</CsInstruments>
<CsScore>
f1 0 16 10 1
f2 0 8 10 1 1
;starts instrument 1 and runs it for a week
i"FillCells" 0 1
i1 1 z
</CsScore>
</CsoundSynthesizer>