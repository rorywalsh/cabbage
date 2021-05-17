<Cabbage>
form caption("MIDI CC Out") size(420, 160), pluginId("def1")
rslider bounds(6, 6, 100, 100), channel("ctrl70"), range(0, 127, 0, 1, 1), text("Ctrl70-Chan1"), textColour("white")
rslider bounds(106, 6, 100, 100), channel("ctrl71"), range(0, 127, 0, 1, 1), text("Ctrl71-Chan1"), textColour("white")
rslider bounds(206, 6, 100, 100), channel("ctrl72"), range(0, 127, 0, 1, 1), text("Ctrl72-Chan1"), textColour("white")
rslider bounds(306, 6, 100, 100), channel("ctrl73"), range(0, 127, 0, 1, 1), text("Ctrl73-Chan1"), textColour("white")
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 -m0d -Q0
</CsOptions>
<CsInstruments>
; Initialize the global variables. 
ksmps = 32
nchnls = 2
0dbfs = 1

; this instrument will send CC message to its MIDI putput
; whenever a slider changes value. 


instr MIDIOUT
    kCtrl71 chnget "ctrl71"
    kCtrl72 chnget "ctrl72"
    kCtrl73 chnget "ctrl73"
    kCtrl74 chnget "ctrl74"

    if changed(kCtrl71) == 1 then
        midiout	176, 1, 70, kCtrl71
    endif
    
    if changed(kCtrl72) == 1 then
        midiout	176, 1, 71, kCtrl72
    endif
    
    if changed(kCtrl73) == 1 then
        midiout	176, 1, 72, kCtrl73
    endif
    
    if changed(kCtrl74) == 1 then
        midiout	176, 1, 73, kCtrl74
    endif   
endin


</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
f0 z
;starts instrument 1 and runs it for a week
i"MIDIOUT" 0 [60*60*24*7] 
</CsScore>
</CsoundSynthesizer>
