<Cabbage>
form caption("String Sequencer") size(400, 400), pluginID("def1")
stringsequencer bounds(10, 10, 300, 320), channels("step", "track1", "track2", "track3", "track4"), active(0), identchannel("trackerIdent"), textcolour(200, 200, 200), highlightcolour(60, 60, 60) outlinecolour(80,80,80), bpm(180), fontcolour("white") backgroundcolour(20, 20, 20) showstepnumbers(4), numberofsteps(16)
rslider bounds(314, 10, 70, 70) channel("bpm") range(10, 300, 180, 1, 0.001) text("BPM") 
button bounds(314, 82, 70, 27) channel("startStop") text("Start", "Stop") 
button bounds(314, 132, 70, 27) channel("shuffle") text("Shuffle")  
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -m0d -m0d
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
    kBeat chnget "step"
    kBPM chnget "bpm"

    if changed(kBeat) == 1 then
        event "i", "Track1", 0, 1
        ;event "i", "Track2", 0, 1
        ;event "i", "Track3", 0, 1
        ;event "i", "Track4", 0, 1
    endif

    if changed(kBPM) == 1 then
        SMessage sprintfk "bpm(%d)", kBPM
        chnset SMessage, "trackerIdent"
    endif

    kStartStop chnget "startStop"
    if changed(kStartStop) == 1 then
        if kStartStop == 1 then
        chnsetks "active(1)", "trackerIdent"
        else
        chnsetks "active(0)", "trackerIdent"
        endif
    endif
    
    if changed:k(chnget:k("position")) == 1 then
        SMessage sprintfk "scrubberposition(%d)", chnget:k("position")
        chnsetks SMessage, "trackerIdent"
    endif

    if changed:k(chnget:k("shuffle")) == 1 then 
        event "i", "FillCells", 0, 0
    endif
    
endin
 
;Simple synthesisers
instr Track1
    SCellData chnget "track1"
    if strlen(SCellData)!=0 then
        iFreq = strtod:i(SCellData)
        p3 = .1
        aEnv expon .2, p3, .001
        aFreq expon 100, p3, 50
        aOscil oscil aEnv, cpsmidinn(iFreq), 2
        outs aOscil, aOscil
    endif
endin

instr Track2
    SCellData chnget "track2"
    if strlen(SCellData)!=0 then
        iFreq = strtod:i(SCellData)
        p3 = .1
        aEnv expon .2, p3, .001
        aOscil oscil aEnv, cpsmidinn(iFreq), 1
        outs aOscil, aOscil
    endif
endin

instr Track3
    SCellData chnget "track3"
    if strlen(SCellData)!=0 then
        iFreq = strtod:i(SCellData)
        p3 = .1
        aEnv expon .2, p3, .001
        aOscil oscil aEnv, cpsmidinn(iFreq), 2
        outs aOscil, aOscil 
    endif
endin


instr Track4
    SCellData chnget "track4"
    if strlen(SCellData)!=0 then
        iFreq = strtod:i(SCellData)
        p3 = .1
        aEnv expon .2, p3, .001
        aOscil oscil aEnv, cpsmidinn(iFreq), 2
        outs aOscil, aOscil 
    endif
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
            if random:i(0, 100)>50 then
                SMessage sprintf "celldata(%d, %d, \"%d\") ", iColCnt, iRowCnt, iNotes[int(iNoteIndex)]
            else
                SMessage sprintf "celldata(%d, %d, \"\") ", iColCnt, iRowCnt
            endif
            SCellData strcat SCellData, SMessage
        iColCnt+=1
        od
        iRowCnt+=1
    od  

    ;when SCellData strings has been constructed, send it to Cabbage
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
