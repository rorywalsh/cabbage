<Cabbage>
form caption("Meter Example") size(380, 500), guiMode("queue"), colour(2, 145, 209) pluginId("def1")

texteditor bounds(18, 281, 341, 204) channel("infoText"), readOnly(1), wrap(1), scrollbars(1)

soundfiler bounds(18, 16, 285, 182), channel("soundfiler1"), file("8BitStyle.wav") colour(147, 210, 0), tableBackgroundColour(0, 0, 0, 0)
button bounds(18, 218, 84, 32) corners(5), channel("play"), text("Play", "Stop")

vmeter bounds(332, 16, 10, 160) channel("vu1") value(0) outlineColour(0, 0, 0), overlayColour(0, 0, 0) meterColour:0(255, 0, 0) meterColour:1(255, 255, 0) meterColour:2(0, 255, 0) outlineThickness(1) 
vmeter bounds(351, 16, 10, 160) channel("vu2") value(0) outlineColour(0, 0, 0), overlayColour(0, 0, 0) meterColour:0(255, 0, 0) meterColour:1(255, 255, 0) meterColour:2(0, 255, 0) outlineThickness(1) 

</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 -m0d --midi-key=4 --midi-velocity-amp=5
</CsOptions>
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

    SText  = "A meter widget can be used to display a audio level meter. It can accept up to three different colours to make up the level colour gradient. You need to send data to the widget from Csound in order for it to work. It does not calculate RMS values itself.\n\nIn this example, a sound file is loaded. The 'diskin2' opcode reads it back when the user hits play. The output from the file is sent to a pair of 'rms' opcodes which converts the audio signal to a k-rate rms value. A small portamento is applied to the RMS values in order to smoothen out the signal that is sent to the meter widgets."

    cabbageSet "infoText", "text", SText
    
    ;trigger playback of sample
    kPlayState, kPlayTrig cabbageGetValue "play"
    if kPlayTrig == 1 then
        if kPlayState == 1 then
            event "i", "SamplePlayback", 0, -1
        else
            turnoff2 nstrnum("SamplePlayback"), 0, 0
        endif
    endif   
        
endin
                
instr SamplePlayback
    
    SFilename = "8BitStyle.wav"
    iLen = filelen(SFilename)*sr
    kScrubber phasor 1/(iLen/sr)
    cabbageSet metro(20), "soundfiler1", "scrubberPosition", kScrubber*iLen
    a1, a2 diskin2 SFilename, 1, 0, 1
    
    k1 rms a1, 20
    k2 rms a2, 20
    
    cabbageSetValue "vu1", portk(k1*10, .25), metro(10)
    cabbageSetValue "vu2", portk(k2*10, .25), metro(10)
    
    outs a1, a1
        
endin

</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
i1 0 z
</CsScore>
</CsoundSynthesizer>
