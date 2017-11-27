<Cabbage>
form caption("String Sequencer") size(400, 400), pluginID("def1")
stringsequencer bounds(10, 10, 300, 320), channels("step", "track1", "track2", "track3", "track4"), identchannel("trackerIdent"), textcolour(200, 200, 200), highlightcolour(60, 60, 60) outlinecolour(80,80,80), bpm(180), fontcolour("white") backgroundcolour(20, 20, 20) showstepnumbers(4), numberofsteps(16)
rslider bounds(314, 10, 70, 70) channel("bpm") range(10, 300, 180, 1, 0.001) text("BPM") 
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -m0d --sample-accurate
</CsOptions>
<CsInstruments>
; Initialize the global variables. 
sr = 44100
ksmps = 32
nchnls = 2
0dbfs = 1

instr 1 
kBeat chnget "step"
kBPM chnget "bpm"

if changed(kBeat) == 1 then
    event "i", "Track1", 0, 1
    event "i", "Track2", 0, 1
    event "i", "Track3", 0, 1
    event "i", "Track4", 0, 1
endif

if changed(kBPM) == 1 then
    SMessage sprintfk "bpm(%d)", kBPM
    chnset SMessage, "trackerIdent"
endif

endin
 
instr Track1
    SCellData chnget "track1"
    if strlen(SCellData)>0 then
        iFreq = strtod:i(chnget:S("track1"))
        p3 = 1
        aEnv expon .2, p3, .001
        aFreq expon 100, p3, 50
        aOscil oscil aEnv, cpsmidinn(iFreq), 2
        outs aOscil, aOscil
    endif
endin

instr Track2
    SCellData chnget "track2"
    if strlen(SCellData)>0 then
        iFreq = strtod:i(chnget:S("track2"))
        p3 = .1
        aEnv expon .2, p3, .001
        aOscil oscil aEnv, cpsmidinn(iFreq), 1
        outs aOscil, aOscil
    endif
endin

instr Track3
    SCellData chnget "track3"
    if strlen(SCellData)>0 then
        iFreq = strtod:i(chnget:S("track3"))
        p3 = .1
        aEnv expon .2, p3, .001
        aOscil oscil aEnv, cpsmidinn(iFreq), 2
        outs aOscil, aOscil 
    endif
endin


instr Track4
    SCellData chnget "track4"
    if strlen(SCellData)>0 then
        iFreq = strtod:i(chnget:S("track4"))
        p3 = .1
        aEnv expon .2, p3, .001
        aOscil oscil aEnv, cpsmidinn(iFreq), 2
        outs aOscil, aOscil 
    endif
endin
</CsInstruments>
<CsScore>
f1 0 16 10 1
f2 0 8 10 1 1
;starts instrument 1 and runs it for a week
i1 0 z
</CsScore>
</CsoundSynthesizer>
