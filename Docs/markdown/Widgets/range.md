# Hrange

Create an on-screen slider with a dragable, and user selectable range. Data can be sent to Csound on the channel specified through the channel string.  


<big></pre>
hrange WIDGET_SYNTAX
</pre></big>

### Specific Identifiers

{! ./markdown/Widgets/Properties/channel_range.md !}   

{! ./markdown/Widgets/Properties/max.md !} 

{! ./markdown/Widgets/Properties/min.md !} 

{! ./markdown/Widgets/Properties/outlineColour.md !} 

{! ./markdown/Widgets/Properties/range.md !} 

{! ./markdown/Widgets/Properties/text.md !} 

{! ./markdown/Widgets/Properties/textBox.md !} 

{! ./markdown/Widgets/Properties/textColour.md !} 

{! ./markdown/Widgets/Properties/trackerThickness.md !} 

{! ./markdown/Widgets/Properties/trackerColour.md !} 

### Common Identifiers

{! ./markdown/Widgets/Properties/active.md !}  

{! ./markdown/Widgets/Properties/alpha.md !}

{! ./markdown/Widgets/Properties/automatable.md !}

{! ./markdown/Widgets/Properties/bounds.md !}  

{! ./markdown/Widgets/Properties/caption.md !}  

{! ./markdown/Widgets/Properties/colour.md !}  

{! ./markdown/Widgets/Properties/fontColour.md !}   

{! ./markdown/Widgets/Properties/identChannel.md !}  

{! ./markdown/Widgets/Properties/popup.md !}

{! ./markdown/Widgets/Properties/presetIgnore.md !} 

{! ./markdown/Widgets/Properties/valuePrefix.md !}

{! ./markdown/Widgets/Properties/valuePostfix.md !}

{! ./markdown/Widgets/Properties/rotate.md !}  

{! ./markdown/Widgets/Properties/visible.md !}  

{! ./markdown/Widgets/Properties/toFront.md !} 

{! ./markdown/Widgets/Properties/widgetArray.md !}
 
<!--(End of identifiers)/-->

>Make sure to use two unique channel names when using hslider2 and vslider2, otherwise min and max will be set to the same value. 

![](../images/hrange.gif)

##Example
<!--(Widget Example)/-->
```csharp
<Cabbage>
form caption("HRange Example") size(400, 300), colour(220, 220, 220), pluginID("def1")
label bounds(8, 6, 368, 20), text("Basic Usage"), fontColour("black")
groupbox bounds(8, 110, 380, 177), text("Randomly Updated Identifiers")
hrange bounds(120, 34, 160, 40), channel("hrangeL", "hrangeR"), colour(123, 34, 143), range(100, 1000, 200:300, 1, .01)
hrange bounds(110, 140, 162, 32), channel("range1", "range2"), identChannel("widgetIdent"), channel colour(123, 34, 143), corners(40), range(0, 1, .24:.75, 1 0.001)
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
    aTone1 oscili .25, chnget:k("hrangeL")
    aTone2 oscili .25, chnget:k("hrangeR")
    outs aTone1+aTone2, aTone1+aTone2    
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

