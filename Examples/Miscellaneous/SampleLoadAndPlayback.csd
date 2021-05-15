<Cabbage>
form caption("Sample load and playback") size(635,430), guiMode("queue"), pluginId("SLaP")
soundfiler bounds(10, 10, 150, 80) channel("soundfiler1") tableNumber(-1) 
soundfiler bounds(165, 10, 150, 80) channel("soundfiler2") tableNumber(-1) 
soundfiler bounds(320, 10, 150, 80) channel("soundfiler3") tableNumber(-1) 
soundfiler bounds(475, 10, 150, 80) channel("soundfiler4") tableNumber(-1) 
soundfiler bounds(10, 95, 150, 80) channel("soundfiler5") tableNumber(-1) 
soundfiler bounds(165, 95, 150, 80) channel("soundfiler6") tableNumber(-1) 
soundfiler bounds(320, 95, 150, 80) channel("soundfiler7") tableNumber(-1) 
soundfiler bounds(475, 95, 150, 80) channel("soundfiler8") tableNumber(-1) 
soundfiler bounds(10, 180, 150, 80) channel("soundfiler9") tableNumber(-1) 
soundfiler bounds(165, 180, 150, 80) channel("soundfiler10") tableNumber(-1) 
soundfiler bounds(320, 180, 150, 80) channel("soundfiler11") tableNumber(-1) 
soundfiler bounds(475, 180, 150, 80) channel("soundfiler12") tableNumber(-1) 
keyboard bounds(10, 266, 615, 86) mouseoeverkeycolour(255, 255, 0, 128) 
filebutton bounds(10, 356, 148, 40) channel("sampleFolderButton"), mode("directory") text("Open sample folder", "Open sample folder") 
label bounds(162, 360, 237, 16) text("Samples are triggered from C4") fontColour(255, 255, 255, 255) channel("label16")
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 --midi-key=4 --midi-velocity-amp=5 -m0d
</CsOptions>

<CsInstruments>
;sr is set by the host
ksmps = 32
nchnls = 2
0dbfs = 1


;---------------------------------------------------------
;always on instrument that waits for user to specify sample folder
instr 100
    SFilepath chnget "sampleFolderButton"
    if changed:k(SFilepath) == 1 then
        gSSamplePath = SFilepath
        printks SFilepath, 0
        event "i", 101, 0, 0.1
    endif
endin

;---------------------------------------------------------
;when user has selected folder, load them into function tables
instr 101
    prints "Loading files to function tables"
    iFirstTableNumber = 100
    SFilepath chnget "sampleFolderButton"
    kTrig = 1   ;using k-rate version because of bug with i-rate version
    gkNumTables ftsamplebank SFilepath, iFirstTableNumber, kTrig, 0, 4, 1
    gSFiles[] directory SFilepath, ".wav"
    cabbageSetStateValue "directoryPath", SFilepath
    event_i "i", 102, 1, 0
endin

;---------------------------------------------------------
;once files have been loaded to tables, update soundfilers
instr 102
    iCnt init 0
    while iCnt<12 do
        SChannel sprintf "soundfiler%d", iCnt+1
        cabbageSet SChannel, sprintf("file(\"%s\")", gSFiles[iCnt])
        iCnt+=1
    od
endin

;---------------------------------------------------------
;this instrument get triggered by the keyboard. It listens for
;a keypress and then trigger instr 2 to play the associated sample
instr 1
    iTable = (p4-72)+100
    iNumSamples = ftlen(iTable)
    event_i "i", 2, 0, iNumSamples/sr, p4
endin

;--------------------------------------------------------
;gets triggered from instr 1. Plays sample and updates scrubber position
instr 2
    iTable = (p4-72)+100
    iNumSamples = ftlen(iTable)
    aPhasor phasor sr/iNumSamples
    aSig tab aPhasor, iTable, 1
    outs aSig, aSig

;    kScrubber	downsamp	aPhasor
;    Smessage sprintfk "scrubberPosition(%d)", kScrubber*iNumSamples
;    SChannel sprintfk "soundfiler%d", p4-72+1 
;    cabbageSet metro(20), SChannel, Smessage

endin

</CsInstruments>
<CsScore>
i100 0 z
</CsScore>
</CsoundSynthesizer>
