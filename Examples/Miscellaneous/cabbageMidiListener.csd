<Cabbage>
form caption("Untitled") size(400, 300), guiMode("queue"), pluginId("def1")
keyboard bounds(8, 158, 381, 95), keypressBaseOctave(4)
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0
</CsOptions>
<CsInstruments>
; Initialize the global variables. 
ksmps = 32
nchnls = 2
0dbfs = 1


;always on and recieving events from cabbageMidiListener
instr MidiListener, 9999

    kNoteIndex init 0
    kNotes[], kVels[], kChns[], kCnt cabbageMidiListener 1

    if metro(6) == 1 && kCnt > 0 then
        event "i", "Synth", 0, .25, kVels[kNoteIndex]/128, kNotes[kNoteIndex]
        kNoteIndex = kNoteIndex < kCnt-1 ? kNoteIndex+1 : 0
    endif
    
endin


;listens on all MIDI channels
instr 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16

    cabbageMidiSender   ;send midi events to cabbageMidiListener opcode..
    
endin


;simple synth
instr Synth  
    print p4
    kEnv madsr .1, .1, .8, .1
    aSig oscili kEnv*p4, cpsmidinn(p5)
    outs aSig*.5, aSig*.5

endin
</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
f0 z
i9999 0 z
</CsScore>
</CsoundSynthesizer>