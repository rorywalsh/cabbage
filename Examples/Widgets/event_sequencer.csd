<Cabbage>
form caption("Event Sequencer") size(400, 300), colour(220, 220, 220), pluginID("def1")
eventsequencer bounds(10, 30, 320, 260), channels("step"), active(0) identchannel("trackerIdent"), colprefix(0:1:2:3, "i\"Sine\" 0 1 "), showstepnumbers(4), matrixsize(8, 4) textcolour(200, 200, 200), highlightcolour(30, 30, 30) outlinecolour(80,80,80), bpm(180), fontcolour("white") backgroundcolour(20, 20, 20)
rslider bounds(320, 60, 70, 70) channel("bpm") range(10, 400, 180, 1, 0.001) text("BPM") 
button bounds(320, 10, 70, 27) channel("startPlayback") text("Start", "Stop")   
label bounds(12, 8, 299, 12), colour(20, 20, 20) text("Enter some MIDI note numbers in the cells below")
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
        ;event "i", "Metronome", 0, 1
        kBeat = kBeat<7 ? kBeat+1 : 0
    endif
    
    
    if changed(kBpm) == 1 then
        SMessage sprintfk "bpm(%d)", kBpm
        chnset SMessage, "trackerIdent"
    endif
    
endin

 
;Simple synthesiser
instr Sine
    aEnv expon .6, p3, .001
    aOscil oscil aEnv, cpsmidinn(p4)
    outs aOscil, aOscil
endin

</CsInstruments>
<CsScore>
;starts instrument 1 and runs it for a week
i1 1 z
</CsScore>
</CsoundSynthesizer>