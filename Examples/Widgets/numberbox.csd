<Cabbage>
form caption("nslider Example") size(570, 500), guiMode("queue"), colour(2, 145, 209) pluginId("def1")

texteditor bounds(24, 324, 530, 147) channel("infoText"), readOnly(1), wrap(1), scrollbars(1)
image bounds(78, 8, 29, 239) channel("scrubber"), corners(3)
button bounds(478, 260, 76, 26) channel("play"), corners(5), text("Play", "Stop")
button bounds(74, 256, 82, 20) channel("shuffle"), corners(5), text("Shuffle")
button bounds(74, 278, 82, 20) channel("clear"), corners(5), text("Clear")
nslider bounds(12, 10, 57, 22) channel("note0")  range(38, 88, 72, 1, 1) velocity(50)
nslider bounds(12, 40, 57, 22) channel("note1")  range(38, 88, 67, 1, 1) velocity(50)
nslider bounds(12, 70, 57, 22) channel("note2")  range(38, 88, 60, 1, 1) velocity(50)
nslider bounds(12, 100, 57, 22) channel("note3") range(38, 88, 59, 1, 1) velocity(50)
nslider bounds(12, 130, 57, 22) channel("note4") range(38, 88, 57, 1, 1) velocity(50)
nslider bounds(12, 160, 57, 22) channel("note5") range(38, 88, 52, 1, 1) velocity(50)
nslider bounds(12, 190, 57, 22) channel("note6") range(38, 88, 48, 1, 1) velocity(50)
nslider bounds(12, 220, 57, 22) channel("note7") range(38, 88, 36, 1, 1) velocity(50)


rslider bounds(16, 252, 51, 51) channel("randomWeight"), text("Weight") range(0, 100, 90, 1, 0.001)
rslider bounds(224, 252, 51, 51) channel("decay") range(0, 1, 0.3, 1, 0.001), text("Decay")
rslider bounds(278, 252, 91, 51) channel("pw") range(0.2, 0.8, 0.2, 1, 0.001), text("Pulse Width")
rslider bounds(168, 252, 51, 51) channel("tempo") range(2, 8, 4, 1, 2), text("Tempo")
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 -m0d --midi-key=4 --midi-velocity-amp=5
</CsOptions>e
<CsInstruments>
; Initialize the global variables. 
ksmps = 16
nchnls = 2
0dbfs = 1

; Rory Walsh 2021 
;
; License: CC0 1.0 Universal
; You can copy, modify, and distribute this file, 
; even for commercial purposes, all without asking permission. 


instr 1

    SText  = "An nslider widget function in the same way a number box does in Pure Data or Max/MSP. You can click and drag to make changes to it. Like all sliders in Cababge, you can press the Alt key while dragging to slow down the speed of increments. You can also double click to enter a value with the keyboard.\n\n\This example create 128 checkbox using the 'cabbageCreate' opcode. A perf-time loop runs through each checkbox to see what its value is. If it is on it will trigger a corresponding note. The 8 nslider widgets set the note of each row. The synth here is a simple square wave synth with some PWM. The output of this gets sent to a master FX instrument that adds a little dynamic shaping a two recursive delay lines."
    cabbageSet "infoText", "text", SText
    
    iX, iY init 0
    iWidgetCount init 0
    iNumSteps = 16
    iNumRows = 8
    
    while iY < iNumRows do
        while iX < iNumSteps do
            SWidget sprintf "bounds(%d, %d, 25, 25), channel(\"check%d\"), colour:1(147, 210, 0)", 80+iX*30, 10+iY*30, iWidgetCount
            cabbageCreate "checkbox", SWidget
            iWidgetCount += 1
            iX += 1
        od
        iX = 0
        iY += 1
    od
    
    ;shuffler
    kShuf, kShufTrig cabbageGetValue "shuffle"
    if kShufTrig ==1 then
        kWidget = 0
        while kWidget < 128 do
            SBoxChannel sprintfk "check%d", kWidget
            cabbageSetValue SBoxChannel, random:k(0, 100) > cabbageGet:k("randomWeight") ? 1 : 0
            kWidget += 1
        od
    endif
    
    ;clear
    kClear, kClearTrig cabbageGetValue "clear"
    if kClearTrig ==1 then
        kWidget = 0
        while kWidget < 128 do
            SBoxChannel sprintfk "check%d", kWidget
            cabbageSetValue SBoxChannel, k(0)
            kWidget += 1
        od
    endif
    
    kPlay cabbageGet "play"
    kCol init 0
    kRow = 0
    if metro(cabbageGet:k("tempo")) == 1 && cabbageGetValue:k("play") == 1 then
        while kRow < 8 do
            SBoxChannel sprintfk "check%d", kCol+(kRow*iNumSteps)
            SNoteChannel sprintfk "note%d", kRow
            if cabbageGetValue:k(SBoxChannel) == 1 then
                event "i", "Synth", 0, 1, cabbageGet:k(SNoteChannel), .1
            endif
            kRow += 1
        od
        cabbageSet 1, "scrubber", "bounds", 78+(kCol*30), 8, 29, 239
        kCol = (kCol < iNumSteps - 1 ? kCol+1 : 0) 
    endif

endin

instr Synth
    kEnv expsegr 0.001, 0.01, .5, .1, 0.00001
    aEnv madsr .1, cabbageGetValue:i("decay"), .3, .1
    aSig vco2 kEnv, cpsmidinn(p4), 2, cabbageGetValue:i("pw") 
    aLp K35_lpf aSig, random:i(100, 2000)*aEnv, .1;[, inlp, isaturation, istor]
    outs aLp*p5, aLp*p5
    ;outs aSig, aSig
    chnmix aLp*p5, "out"
endin

instr MasterFx
    aSnapper oscili 1, 2, 1
    aIn chnget "out"
    a1 comb aIn*aSnapper, 3, .5
    a2 comb aIn*aSnapper, 3, .75
    outs a1, a2
    chnclear "out"
endin
</CsInstruments>
<CsScore>
f1 0 4096 7 0 100 1 2000 1 47 0
i1 0 z
i"MasterFx" 0 z
</CsScore>
</CsoundSynthesizer>
