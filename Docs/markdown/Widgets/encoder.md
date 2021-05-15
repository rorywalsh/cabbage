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
form caption("Encoder Example") size(400, 300), colour(220, 220, 220), pluginID("def1")
label bounds(8, 6, 368, 20), text("Basic Usage"), fontColour("black")
encoder bounds(8, 38, 369, 50), channel("gain"), text("Gain") min(0), max(1), increment(.01) fontColour(91, 46, 46, 255) textColour(29, 29, 29, 255)
groupbox bounds(8, 110, 380, 177), text("Randomly Updated Identifiers")
encoder bounds(70, 140, 41, 119) identChannel("widgetIdent"),  
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 -m0d 
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
    aTone oscili chnget:k("gain"), 300
    outs aTone, aTone    
endin

;WIDGET_ADVANCED_USAGE

</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
f0 z
;starts instrument 1 and runs it for a week
i1 0 z
i2 0 z
</CsScore>
</CsoundSynthesizer>
```
<!--(Widget Example)/-->
