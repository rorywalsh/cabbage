<Cabbage>
form caption("Presets") size(370, 180), pluginId("MPre")
rslider bounds(12, 8, 85, 79), channel("att"), range(0, 1, 0.01), text("Att.")
rslider bounds(98, 8, 85, 79), channel("dec"), range(0, 1, 0.4), text("Dec.")
rslider bounds(184, 8, 85, 79), channel("sus"), range(0, 1, 0.7), text("Sus.")
rslider bounds(270, 8, 85, 79), channel("rel"), range(0, 1, 0.8), text("Rel.")

filebutton bounds(16, 92, 60, 28), channel("saveFile"), text("Save", "Save"), populate("*.pres"), mode("snapshot") value(0)
filebutton bounds(78, 92, 60, 28), channel("openFile") text("Open", "Open"), populate("*.pres"), value(0)
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 --midi-key-cps=4 --midi-velocity-amp=5
</CsOptions>
<CsInstruments>
; Initialize the global variables. 
;sr is set by the host
ksmps = 32
nchnls = 2
0dbfs = 1


;always on and listening for preset updates..
instr 1000
    if changed:k(chnget:S("openFile")) == 1 then
        event "i", 1002, 0, .1
    endif
    
    if changed:k(chnget:S("saveFile")) == 1 then
        event "i", 1003, 0, .1
    endif
endin

;================================================
instr 1002 ;restore presets
    SFileName chnget "openFile"
    prints SFileName
    if strrindex(SFileName, ".pres") == -1 then
        SFileName strcat SFileName, ".pres"
    endif
    i1, i2, i3, i4 init 0
    fini SFileName, 0, 0, i1, i2, i3, i4
    chnset i1, "att"
    chnset i1, "dec"
    chnset i1, "sus"
    chnset i1, "rel"
endin

;================================================
instr 1003 ;save presets
    SFileName chnget "saveFile"

    if strrindex(SFileName, ".pres") == -1 then
        SFileName strcat SFileName, ".pres"
    endif

    fprints SFileName, "%f %f %f %f", chnget:i("att"), chnget:i("dec"), chnget:i("sus"), chnget:i("rel")
endin

</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
i1000 0 z
</CsScore>
</CsoundSynthesizer>