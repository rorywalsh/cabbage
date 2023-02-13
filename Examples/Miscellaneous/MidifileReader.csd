<Cabbage>
form caption("Midi playback") size(400, 100), colour(40), guiMode("queue") pluginId("def1")
rslider bounds(114, 8, 60, 60) channel("transpose") range(-24, 24, 0, 1, 1), text("Transpose")
checkbox bounds(8, 8, 100, 30) channel("play"), text("Play"), 
rslider bounds(178, 8, 60, 60) channel("speed") range(0.5, 2, 1, 1, 0.001), text("Speed")
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -m0d
</CsOptions>
<CsInstruments>010 
; Initialize the global variables. 
ksmps = 128
nchnls = 2
0dbfs = 1

instr 1
    SCsdPath chnget "CSD_PATH"

    SMidifile init sprintf("%s/Prelude1.mid", SCsdPath)
    cabbageMidiInfo SMidifile
    kEventIndex = 0
    kNoteIndex init 0
    kStatus[], kChan[], kNote[], kVel[], kNumEvents, kTrig cabbageMidiReader SMidifile, 1, cabbageGetValue("play"), cabbageGetValue("speed"), 0
    knotelength    init    0
    knoteontime    init    0


    if kTrig == 1 then
        while kEventIndex < kNumEvents do
            if (kStatus[kEventIndex] == 128) then            //note off
                turnoff2 2, 1, 1
            elseif (kStatus[kEventIndex] == 144) then        //note on 
                event "i", 2, 0, 10, kNote[kEventIndex]+cabbageGetValue:k("transpose"), kVel[kEventIndex]/127       
            endif
            kEventIndex += 1
        od
    endif
endin

instr 2
    kEnv madsr 0.1, .2, .6, .1 
    a1 oscili kEnv*p5, cpsmidinn(p4), 1
    outs a1*.01, a1*.01
endin


</CsInstruments>
<CsScore>
f1 0 4096 10 1 0 .25 0 .17
;causes Csound to run for about 7000 years...
f0 z
;starts instrument 1 and runs it for a week
i1 0 [60*60*24*7] 
;i3 0 10
</CsScore>
</CsoundSynthesizer>
