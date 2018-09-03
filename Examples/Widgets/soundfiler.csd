<Cabbage>
form size(400, 500), caption("Soundfiler"), size(400, 300), colour(220, 220, 220), pluginid("SndF")
soundfiler bounds(10,10, 380, 200), identchannel("soundfilerIdent")
filebutton bounds(10, 215, 100, 30), channel("openFile"), text("Browse")
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 -m0d --displays
</CsOptions>
<CsInstruments>
; Initialize the global variables. 
sr = 44100
ksmps = 32
nchnls = 2
0dbfs = 1

seed 0 

;basic usage
instr 1
    SFile chnget "openFile"
    if changed(SFile) == 1 then
        SIdentString sprintfk "file(\"%s\")", SFile
        chnset SIdentString, "soundfilerIdent"
    endif
endin

</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
f0 z
;starts instrument 1 and runs it for a week
i1 0 z
</CsScore>
</CsoundSynthesizer>
