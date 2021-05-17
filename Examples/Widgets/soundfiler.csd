<Cabbage>
form caption("Soundfiler Example") size(410, 500), guiMode("queue"), colour(2, 145, 209) pluginId("def1")

texteditor bounds(16, 254, 376, 233) channel("infoText"), readOnly(1), wrap(1), scrollbars(1)

soundfiler bounds(22, 16, 120, 89), channel("soundfiler1"), file("./DoubleBass/1.wav") colour(147, 210, 0), tableBackgroundColour(0, 0, 0, 80)
button bounds(80, 80, 58, 22) corners(5), channel("play1"), text("Play")

soundfiler bounds(146, 16, 120, 89), channel("soundfiler2"), file("./DoubleBass/2.wav") colour(147, 210, 0), tableBackgroundColour(0, 0, 0, 80)
button bounds(206, 80, 58, 22) corners(5), channel("play2"), text("Play")

soundfiler bounds(270, 16, 120, 89), channel("soundfiler3"), file("./DoubleBass/3.wav") colour(147, 210, 0), tableBackgroundColour(0, 0, 0, 80)
button bounds(330, 80, 58, 22) corners(5), channel("play3"), text("Play")

soundfiler bounds(22, 116, 120, 89), channel("soundfiler4"), file("./DoubleBass/4.wav") colour(147, 210, 0), tableBackgroundColour(0, 0, 0, 80)
button bounds(80, 180, 58, 22) corners(5), channel("play4"), text("Play")

soundfiler bounds(146, 116, 120, 89), channel("soundfiler5"), file("./DoubleBass/5.wav") colour(147, 210, 0), tableBackgroundColour(0, 0, 0, 80)
button bounds(206, 180, 58, 22) corners(5), channel("play5"), text("Play")

soundfiler bounds(270, 116, 120, 89), channel("soundfiler6"), file("./DoubleBass/6.wav") colour(147, 210, 0), tableBackgroundColour(0, 0, 0, 80)
button bounds(330, 180, 58, 22) corners(5), channel("play6"), text("Play")
button bounds(271, 216, 114, 27) channel("randomPlayback"), corners(5), text("Random Playback", "Stop")
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


instr GuiControl

    SText  = "A soundfiler widget can be used to display to a sound file; from disk, or from a function table. It does not play back any audio, or produce any signal. All this must be handled using Csound.\n\nIn the example here, 6 samples are loaded from disk. The 'TriggerSamplePlayback' instrument is instantiated 6 times, one for each sample. This saves having to code 6 times.\n\nWhen the 'SamplePlayback' instrument is triggered is plays back the corresponding sample. It also changes the background colour of the soundfiler to give some visual feedback to the user as to when the sample is playing.\n\nFinally, a random button is added which triggers the six different sample to play back at random time intervals." 
    cabbageSet "infoText", "text", SText
        
    kRandom cabbageGetValue "randomPlayback"
    if kRandom == 1 && metro(1) == 1 then
        event "i", "SamplePlayback", int(random:k(1, 10))*.1, 1, int(random:k(1, 7))
    endif
endin


instr TriggerSamplePlayback 

    STrigButtonChannel sprintf "play%d", p4
    ;trigger playback of sample
    kPlayState, kPlayTrig cabbageGetValue STrigButtonChannel
    if kPlayTrig == 1 then
            event "i", "SamplePlayback", 0, .1, p4
    endif    
    
endin


instr SamplePlayback

    SFile sprintf "./DoubleBass/%d.wav", p4
    iLen = filelen(SFile)
    p3 = iLen
    
    SWidgetChannel sprintf "soundfiler%d", p4
    kAlpha expon 180, p3*.3, 80
    cabbageSet metro(20), SWidgetChannel, "tableBackgroundColour", 0, 0, 0, kAlpha     
    
    a1 diskin2 SFile, 1, 0, 1
    outs a1, a1  
        
endin


</CsInstruments>
<CsScore>
i"GuiControl" 0 z
i"TriggerSamplePlayback" 0 z 1
i"TriggerSamplePlayback" 0 z 2
i"TriggerSamplePlayback" 0 z 3
i"TriggerSamplePlayback" 0 z 4
i"TriggerSamplePlayback" 0 z 5
i"TriggerSamplePlayback" 0 z 6
</CsScore>
</CsoundSynthesizer>
