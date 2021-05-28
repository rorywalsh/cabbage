<Cabbage>
form caption("Listbox Example") size(380, 520), guiMode("queue"), colour(2, 145, 209) pluginId("def1")

texteditor bounds(11, 300, 361, 205) channel("infoText"), readOnly(1), wrap(1), scrollbars(1)

soundfiler bounds(12, 16, 354, 160), channel("soundfiler1"), file("808loop.wav") zoom(.5), colour(147, 210, 0), tableBackgroundColour(0, 0, 0, 0)
soundfiler bounds(12, 16, 354, 160), channel("soundfiler2"),file("808loop.wav") zoom(.5), alpha(0), colour(255, 255, 0), tableBackgroundColour(0, 0, 0, 0)
listbox bounds(18, 186, 346, 79) channel("samples"), align("left"), channelType("string"), populate("*.wav", ".") colour(34, 34, 34, 0), value("1")
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

    SText  = "A listbox can be used to display a list of items. The items can be set using the 'items()' identifier, or by using the 'populate()' identifier, and pointing to a directory on disk. To send the item index, or item string if you set the channel type to 'string', users must double-click the item.\n\nIn this example the listbox points at the current directory, and lists all the '.wav' files it finds there. There are two 'soundfiler' widgets that are used to display the different files. When a user double-clicks a sound file in the list box, it triggers a amplitude fade out on the current audio sample, and a fade in on the next. At the same time, the two soundfiler widgets have their alpha() values modified so they also fade in and out."  

    cabbageSet "infoText", "text", SText
    kSoundfilerIndex init 1
    SSample, kSampleTrig cabbageGetValue "samples"
    if kSampleTrig == 1 then
        
        if active:k(nstrnum("SamplePlayback")) > 0 then
               turnoff2 nstrnum("SamplePlayback"), 1, 1
        endif
               event "i", "SamplePlayback", 0, 1000, kSoundfilerIndex
        
        kSoundfilerIndex = kSoundfilerIndex < 2 ? kSoundfilerIndex+1 : 1
    endif


endin
                
instr SamplePlayback
    kEnv madsr 1, 0, 1, 1
    print p4
    SFilename cabbageGetValue "samples"
    SSoundfiler sprintf "soundfiler%d", p4
    cabbageSet SSoundfiler, sprintfk("file(\"%s\")", SFilename)
    if filevalid(SFilename) ==1 then 
        iLen = filelen(SFilename)*sr
        kScrubber phasor 1/(iLen/sr)
        cabbageSet metro(20), SSoundfiler, "scrubberPosition", kScrubber*iLen
        a1, a2 diskin2 SFilename, 1, 0, 1
        outs a1*kEnv, a1*kEnv
    endif
    
    if kEnv < 1 then
        cabbageSet 1, SSoundfiler, "alpha", kEnv
    endif
        
endin

</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
i1 0 z
</CsScore>
</CsoundSynthesizer>
