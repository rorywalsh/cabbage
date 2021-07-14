# Check Box

Checkbox creates a checkbox which functions like a button, but the associated caption will not change when the user checks it. As with all widgets capable of sending data to Csound, the channel string is the channel on which the widget will communicate with Csound. 

<video width="800" height="600" controls>
<source src="../../images/docs/checkbox.mp4">
</video> 

<big></pre>
checkbox WIDGET_SYNTAX
</pre></big>

### Specific Identifiers

{! ./markdown/Widgets/Properties/corners.md !} 

{! ./markdown/Widgets/Properties/radioGroup.md !} 

### Common Identifiers

{! ./markdown/Widgets/Properties/active.md !}

{! ./markdown/Widgets/Properties/automatable.md !}

{! ./markdown/Widgets/Properties/alpha.md !} 

{! ./markdown/Widgets/Properties/bounds.md !} 

{! ./markdown/Widgets/Properties/channel.md !} 

{! ./markdown/Widgets/Properties/colour_0.md !} 

{! ./markdown/Widgets/Properties/colour_1.md !} 

>You can create an invisible checkbox by setting the colour to something with an alpha value of 0. Images can then be placed under the checkbox to create an image button. See the PVSampler instrument as an example of this. 

{! ./markdown/Widgets/Properties/fontColour.md !} 

{! ./markdown/Widgets/Properties/identChannel.md !} 

{! ./markdown/Widgets/Properties/parent.md !} 

{! ./markdown/Widgets/Properties/popup.md !} 

{! ./markdown/Widgets/Properties/presetIgnore.md !} 

{! ./markdown/Widgets/Properties/rotate.md !} 

{! ./markdown/Widgets/Properties/shape.md !} 

{! ./markdown/Widgets/Properties/text.md !}

{! ./markdown/Widgets/Properties/value.md !} 

{! ./markdown/Widgets/Properties/visible.md !} 

{! ./markdown/Widgets/Properties/toFront.md !} 

{! ./markdown/Widgets/Properties/moveBehind.md !} 

{! ./markdown/Widgets/Properties/widgetArray.md !} 

<!--(End of identifiers)/-->

>colour:1() can be set using colour(). However, it's recommended that you use the numerated colour identifiers in order to make your code more readable. 


## Example
<!--(Widget Example)/-->
```csharp
<Cabbage>
form caption("Checkbox Example") size(380, 500), guiMode("queue"), colour(2, 145, 209) pluginId("def1")

texteditor bounds(18, 256, 341, 231) channel("infoText"), readOnly(1), wrap(1), scrollbars(1)

soundfiler bounds(18, 16, 339, 182), channel("soundfiler1"), file("808loop.wav") colour(147, 210, 0), tableBackgroundColour(0, 0, 0, 0)
button bounds(18, 202, 84, 32) corners(5), channel("play"), text("Play", "Stop")
checkbox bounds(104, 202, 125, 32) channel("delay"), fontColour:1(255, 255, 255) text("Delay Enabled")
checkbox bounds(232, 202, 129, 32) channel("reverb"), fontColour:1(255, 255, 255) text("Reverb Enabled")
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

instr 1

    SText  = "A checkbox can be used a toggle/switch between 0 and 1. They are typically used to enable or disable certain parameters. In the example, a sample is loaded to a soundfiler. Csound reads the file directly from disk. As the file is playing, the output of the sampler is sent to a master FX instrument. The two checkbox widgets will enable or disable two different effects. \n\nThe soundfiler scrubber position is being updated in real time. To calculate its position, we use a phasor set to a frequency of 1/(file length / sampling rate). This produces a signal that moves from 0 to 1 in the same length of time it takes the file to play. We then multiply this value by the length of the soundfile in samples and use this value to set the position of the tracker."
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

    iLen = filelen("808loop.wav")*sr
    print iLen
    kScrubber phasor 1/(iLen/sr)
    cabbageSet metro(20), "soundfiler1", "scrubberPosition", kScrubber*iLen
    a1, a2 diskin2 "808loop.wav", 1, 0, 1
    outs a1, a2    
    
    ;send output from sampler to effects
    chnset a1, "samplerLeftOut"
    chnset a2, "samplerRightOut"
    
endin


instr Effects
    aInL chnget "samplerLeftOut"
    aInR chnget "samplerRightOut"
    aRevL, aRevR, aDelR, aDelL init 0

    kDelay cabbageGetValue "delay"
    kReverb cabbageGetValue "reverb"
    
    if kDelay == 1 then
        aDelL comb aInL, 2, (1/100)*20
        aDelR comb aInR, 2, (1/50)*20
    else
        aDelR = 0
        aDelL = 0
    endif

    if kReverb == 1 then
        aRevL, aRevR reverbsc aInL, aInR, .99, 15000
    else
        aRevR = 0
        aRevL = 0
    endif  
    
    
    outs (aDelL+aRevL)*.5, (aDelR+aRevR)*.5  
    
    chnclear "samplerLeftOut"
    chnclear "samplerRightOut" 
endin

</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
i1 0 z
i"Effects" 0 z ;always on
</CsScore>
</CsoundSynthesizer>

```
<!--(End Widget Example)/-->


