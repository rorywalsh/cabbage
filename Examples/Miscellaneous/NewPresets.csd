<Cabbage>
form caption("Presets") size(370, 580), guiMode("queue") pluginId("MPre")
csoundoutput bounds(0, 300, 370, 300)
keyboard bounds(14, 122, 345, 95)
rslider bounds(16, 40, 85, 79), channel("att"), range(0, 1, 0.01), text("Att.")
rslider bounds(102, 40, 85, 79), channel("dec"), range(0, 1, 0.4), text("Dec.")
rslider bounds(188, 40, 85, 79), channel("sus"), range(0, 1, 0.7), text("Sus.")
rslider bounds(274, 40, 85, 79), channel("rel"), range(0, 1, 0.7, 1, 0.001), text("Rel.")
presetbutton bounds(16, 6, 86, 27), corners(5)  channel("presetChannel"), fontSize(20), textColour(200), fontColour:0(47, 210, 10), presetNameAsText(1) userFolder("Presets", "*.psts"), highlightedItemColour(10, 147, 210) highlightedTextColour(255, 255, 255)
label bounds(16, 224, 343, 26) channel("presetLabel"), colour(10, 10, 10, 255), text("Preset: No preset selected"), ,fontSize(18), fontColour(255, 255, 255, 255)
button bounds(270, 6, 89, 29) channel("setPreset"), text("Set Preset"), corners(5)
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

;instrument will be triggered by keyboard widget
instr 1
    ;update label with preset name
    SPresetFile, kTrig cabbageGetValue "presetChannel"
    printf "PresetName: %s", kTrig, SPresetFile
    cabbageSet kTrig, "presetLabel", sprintfk("text(\"Preset: %s\")", cabbageGetFileNoExtension(SPresetFile))
    
    kVal, kUpdate cabbageGetValue "setPreset"
    SPresetPath strcat chnget:S("CSD_PATH"), "/Presets/Abc.psts"
    cabbageSetValue "presetChannel", SPresetPath, kUpdate
endin

</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
i1 0 z
</CsScore>
</CsoundSynthesizer>