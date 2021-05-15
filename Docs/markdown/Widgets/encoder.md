# Endless encoder

Encoder creates an endless rotary encoder similar to those found in certain well-known hardware samplers. Unlike regular sliders, *encoder* doesn't typically have a start and stop point. By setting a very small slider increment these widgets can offer a level of fine grain control not possible with regular sliders. Note that these sliders do not use the range() identifier. But you can set min and max values using `min()` and `max()`.

<big></pre>
encoder WIDGET_SYNTAX
</pre></big>

### Specific Identifiers

{! ./markdown/Widgets/Properties/increment.md !} 

{! ./markdown/Widgets/Properties/max.md !} 

{! ./markdown/Widgets/Properties/min.md !} 

{! ./markdown/Widgets/Properties/outlineColour.md !} 

{! ./markdown/Widgets/Properties/trackerColour.md !} 

{! ./markdown/Widgets/Properties/textBox.md !} 

{! ./markdown/Widgets/Properties/textColour.md !} 

{! ./markdown/Widgets/Properties/text_slider.md !} 

### Common Identifiers

{! ./markdown/Widgets/Properties/active.md !} 

{! ./markdown/Widgets/Properties/alpha.md !}

{! ./markdown/Widgets/Properties/automatable.md !}

{! ./markdown/Widgets/Properties/bounds.md !}  

{! ./markdown/Widgets/Properties/caption.md !}  

{! ./markdown/Widgets/Properties/channel.md !}  

{! ./markdown/Widgets/Properties/colour.md !}  

{! ./markdown/Widgets/Properties/fontColour.md !}   

{! ./markdown/Widgets/Properties/identChannel.md !}  

{! ./markdown/Widgets/Properties/popup.md !}

{! ./markdown/Widgets/Properties/presetIgnore.md !} 

{! ./markdown/Widgets/Properties/rotate.md !}  



{! ./markdown/Widgets/Properties/value.md !}

{! ./markdown/Widgets/Properties/valuePrefix.md !}

{! ./markdown/Widgets/Properties/valuePostfix.md !}

{! ./markdown/Widgets/Properties/visible.md !}  

{! ./markdown/Widgets/Properties/toFront.md !} 

{! ./markdown/Widgets/Properties/widgetArray.md !}  

<!--(End of identifiers)/-->

![Button](../images/encoder.gif)

##Example
<!--(Widget Example)/-->
```csharp
<Cabbage>
form caption("Encoder Example") size(380, 500), guiMode("queue"), colour(2, 145, 209) pluginId("def1")

texteditor bounds(18, 281, 341, 204) channel("infoText"), readOnly(1), wrap(1), scrollbars(1)

soundfiler bounds(18, 16, 339, 182), channel("soundfiler1"), file("Guitar3.wav") colour(147, 210, 0), tableBackgroundColour(0, 0, 0, 0)
button bounds(18, 218, 84, 32) corners(5), channel("play"), text("Play", "Stop")

encoder bounds(296, 204, 60, 60) channel("detune"), popupPrefix("Detune Amount: "), increment(0.001)
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

    SText  = "A rotary controller can provide a high degree of precision, and offer fine tunings over certain parameters. In this example two diskin2 opcode are playing back the same sound file. When we move the endless encoder we can detune one of the samples by manipulating its playback speed. This gives a quick and simple chorus effect.\n\nThe popup value of the encoder is prefixed with \"detune Amount\" to provide more information to the user. All widget can have popup text appear when you hover over them. Only sliders offer popupPrefix and popupPostFix options."    
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
    
    SFilename = "Guitar3.wav"
    kDetune, kTrig cabbageGetValue "detune"    
    iLen = filelen(SFilename)*sr
    kScrubber phasor 1/(iLen/sr)
    cabbageSet metro(20), "soundfiler1", "scrubberPosition", kScrubber*iLen
    a1, a2 diskin2 SFilename, 1, 0, 1
    a3, a4 diskin2 SFilename, 1+kDetune, 0, 1
    aLeft = a1 + a3
    aRight = a2 + a4
    outs aLeft/2, aRight/2    
        
endin


</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
i1 0 z
</CsScore>
</CsoundSynthesizer>

```
<!--(Widget Example)/-->
